# SparkStreaming 源码解析

[参考 - 腾讯酷玩spark](https://github.com/lw-lin/CoolplaySpark)

Spark Streaming 整体分为4个模块

1. DAG 静态定义
   - [DStream, DStreamGraph 详解](https://github.com/lw-lin/CoolplaySpark/blob/master/Spark%20Streaming%20%E6%BA%90%E7%A0%81%E8%A7%A3%E6%9E%90%E7%B3%BB%E5%88%97/1.1%20DStream%2C%20DStreamGraph%20%E8%AF%A6%E8%A7%A3.md)
   - [DStream 生成 RDD 实例详解](https://github.com/lw-lin/CoolplaySpark/blob/master/Spark%20Streaming%20%E6%BA%90%E7%A0%81%E8%A7%A3%E6%9E%90%E7%B3%BB%E5%88%97/1.2%20DStream%20%E7%94%9F%E6%88%90%20RDD%20%E5%AE%9E%E4%BE%8B%E8%AF%A6%E8%A7%A3.md)
2. Job 动态生成 (这篇)
3. 数据产生与导入
   - [数据产生 - Receiver分发和启动](/Receiver分发和启动.md)
   - [数据导入 - ReceiverSupervisor](/ReceiverSupervisor.md)
4. 长时容错

![每个模块涉及的类](/image/SparkStreaming.png)

## 从 quick example 开始

```scala
import org.apache.spark._
import org.apache.spark.streaming._

// 首先配置一下本 quick example 将跑在本机，app name 是 NetworkWordCount
val conf = new SparkConf().setMaster("local[2]").setAppName("NetworkWordCount")
// batchDuration 设置为 1 秒，然后创建一个 streaming 入口
val ssc = new StreamingContext(conf, Seconds(1))

// ssc.socketTextStream() 将创建一个 SocketInputDStream；这个 InputDStream 的 SocketReceiver 将监听本机 9999 端口
val lines = ssc.socketTextStream("localhost", 9999)

val words = lines.flatMap(_.split(" "))      // DStream transformation
val pairs = words.map(word => (word, 1))     // DStream transformation
val wordCounts = pairs.reduceByKey(_ + _)    // DStream transformation
wordCounts.print()                           // DStream output
// 上面 4 行利用 DStream transformation 构造出了 lines -> words -> pairs -> wordCounts -> .print() 这样一个 DStreamGraph
// 但注意，到目前是定义好了产生数据的 SocketReceiver，以及一个 DStreamGraph，这些都是静态的

// 下面这行 start() 将在幕后启动 JobScheduler, 进而启动 JobGenerator 和 ReceiverTracker
// ssc.start()
//    -> JobScheduler.start()
//        -> JobGenerator.start();    开始不断生成一个一个 batch
//        -> ReceiverTracker.start(); 开始往 executor 上分布 ReceiverSupervisor 了，也会进一步创建和启动 Receiver
ssc.start()

// 然后用户 code 主线程就 block 在下面这行代码了
// block 的后果就是，后台的 JobScheduler 线程周而复始的产生一个一个 batch 而不停息
// 也就是在这里，我们前面静态定义的 DStreamGraph 的 print()，才一次一次被在 RDD 实例上调用，一次一次打印出当前 batch 的结果
ssc.awaitTermination()
```

```scala
// StreamingContext.start()
def start(): Unit = synchronized {
    state match {
      case INITIALIZED =>
            // Start the streaming scheduler in a new thread, so that thread local properties
            // like call sites and job groups can be reset without affecting those of the
            // current thread.
            ThreadUtils.runInNewThread("streaming-start") {
              sparkContext.setCallSite(startSite.get)
              sparkContext.clearJobGroup()
              sparkContext.setLocalProperty(SparkContext.SPARK_JOB_INTERRUPT_ON_CANCEL, "false")
              // 调用了 JobScheduler 的 start()
              scheduler.start()
            }
            state = StreamingContextState.ACTIVE

        ...
      case ACTIVE =>
        ...
      case STOPPED =>
        ...
    }
}
```

`JobScheduler` 有两个非常重要的成员：`JobGenerator` 和 `ReceiverTracker`。`JobScheduler` 将每个 batch 的 RDD DAG 具体生成工作委托给 `JobGenerator`，而将源头输入数据的记录工作委托给 `ReceiverTracker`。

```scala
// JobScheduler类中定义：
private val jobGenerator = new JobGenerator(this)

// JobScheduler.start()
def start(): Unit = synchronized {
    if (eventLoop != null) return // scheduler has already been started

    logDebug("Starting JobScheduler")
    eventLoop = new EventLoop[JobSchedulerEvent]("JobScheduler") {
        override protected def onReceive(event: JobSchedulerEvent): Unit = processEvent(event)

        override protected def onError(e: Throwable): Unit = reportError("Error in job scheduler", e)
    }
    eventLoop.start()

    // attach rate controllers of input streams to receive batch completion updates
    for {
        inputDStream <- ssc.graph.getInputStreams
        rateController <- inputDStream.rateController
    } ssc.addStreamingListener(rateController)

    listenerBus.start()
    receiverTracker = new ReceiverTracker(ssc)
    inputInfoTracker = new InputInfoTracker(ssc)

    ...

    // 调用了 ReceiverTracker 的 start()
    receiverTracker.start()
    // 调用了 JobGenerator 的 start()
    jobGenerator.start()
    executorAllocationManager.foreach(_.start())
    logInfo("Started JobScheduler")
}
```

先来看 `JobGenerator` ， JobGenerator包含四个事件

```scala
/** Event classes for JobGenerator */
private[scheduler] sealed trait JobGeneratorEvent
private[scheduler] case class GenerateJobs(time: Time) extends JobGeneratorEvent
private[scheduler] case class ClearMetadata(time: Time) extends JobGeneratorEvent
private[scheduler] case class DoCheckpoint(
    time: Time, clearCheckpointDataLater: Boolean) extends JobGeneratorEvent
private[scheduler] case class ClearCheckpointData(time: Time) extends JobGeneratorEvent
```

```scala
// JobGenerator类中定义：
// JobGenerator 维护了一个定时器，周期就是我们刚刚提到的 batchDuration，定时为每个 batch 生成 RDD DAG 的实例
// 定时器定义
private val timer = new RecurringTimer(clock, ssc.graph.batchDuration.milliseconds,
    longTime => eventLoop.post(GenerateJobs(new Time(longTime))), "JobGenerator")

// eventLoop is created when generator starts.
// This not being null means the scheduler has been started and not stopped
// 事件循环处理器声明
private var eventLoop: EventLoop[JobGeneratorEvent] = null

// JobGenerator.start()

def start(): Unit = synchronized {
    if (eventLoop != null) return // generator has already been started

    // Call checkpointWriter here to initialize it before eventLoop uses it to avoid a deadlock.
    // See SPARK-10125
    checkpointWriter
    // 事件循环处理器定义，并声明 eventLoop的处理函数为 processEvent(event)
    eventLoop = new EventLoop[JobGeneratorEvent]("JobGenerator") {
      override protected def onReceive(event: JobGeneratorEvent): Unit = processEvent(event)

      override protected def onError(e: Throwable): Unit = {
        jobScheduler.reportError("Error in job generator", e)
      }
    }
    // 启动事件循环处理器
    eventLoop.start()

    if (ssc.isCheckpointPresent) {
        // 不是第一次启动，就需要从checkpoint恢复
      restart()
    } else {
        // 第一次启动从 startFirstTime() 开始
      startFirstTime()
    }
  }
```

```scala
/** Starts the generator for the first time */
private def startFirstTime() {
    val startTime = new Time(timer.getStartTime())
    // 告知DstreamGraph 第一个batch 的启动时间
    graph.start(startTime - graph.batchDuration)
    // 启动之前定时器
    timer.start(startTime.milliseconds)
    logInfo("Started JobGenerator at " + startTime)
}
```

定时器启动之后，会定时向 `eventLoop` 里发送当前 `batch(new Time(longTime))` 事件 `GenerateJobs`，时间周期为 `batchDuration`

调用 `eventLoop` 处理函数 `processEvent`

```scala
/** Processes all events */
private def processEvent(event: JobGeneratorEvent) {
    logDebug("Got event " + event)
    event match {
        // GenerateJobs事件 处理函数 generateJobs(time)
        case GenerateJobs(time) => generateJobs(time)
        case ClearMetadata(time) => clearMetadata(time)
        case DoCheckpoint(time, clearCheckpointDataLater) =>
        doCheckpoint(time, clearCheckpointDataLater)
        case ClearCheckpointData(time) => clearCheckpointData(time)
    }
}
```

```scala
// generateJobs
/** Generate jobs and perform checkpointing for the given `time`.  */
private def generateJobs(time: Time) {
    // Checkpoint all RDDs marked for checkpointing to ensure their lineages are
    // truncated periodically. Otherwise, we may run into stack overflows (SPARK-6847).
    ssc.sparkContext.setLocalProperty(RDD.CHECKPOINT_ALL_MARKED_ANCESTORS, "true")
    Try {
        // 步骤1. 要求 ReceiverTracker 将目前已收到的数据进行一次 allocate，即将上次 batch 切分后，新到达的数据切分到到本次新的 batch 里
        jobScheduler.receiverTracker.allocateBlocksToBatch(time) // allocate received blocks to batch
        // 步骤2. 要求 DStreamGraph 复制出一套新的 RDD DAG 的实例
        graph.generateJobs(time) // generate jobs using allocated block
    } match {
        case Success(jobs) =>
        // 步骤3. 获取第 1 步 ReceiverTracker 分配到本 batch 的源头数据的 meta 信息
        val streamIdToInputInfos = jobScheduler.inputInfoTracker.getInfo(time)
        // 步骤4. 将第 2 步生成的本 batch 的 RDD DAG，和第 3 步获取到的 meta 信息，一同提交给 JobScheduler 异步执行
        // 提交信息 time，Seq[job]，块数据meta信息者三者包装为一个JobSet 提交给JobScheduler
        jobScheduler.submitJobSet(JobSet(time, jobs, streamIdToInputInfos))
        case Failure(e) =>
        jobScheduler.reportError("Error generating jobs for time " + time, e)
        PythonDStream.stopStreamingContextIfPythonProcessIsDead(e)
    }
    // 步骤5. 只要提交结束（不管是否已开始异步执行），就马上对整个系统的当前运行状态做一个 checkpoint
    eventLoop.post(DoCheckpoint(time, clearCheckpointDataLater = false))
}
```

`JobScheduler` 收到 `jobSet` 之后处理：

```scala
// 线程池定义
private val numConcurrentJobs = ssc.conf.getInt("spark.streaming.concurrentJobs", 1)
private val jobExecutor =
    ThreadUtils.newDaemonFixedThreadPool(numConcurrentJobs, "streaming-job-executor")

// JobScheduler 的 submitJobSet
def submitJobSet(jobSet: JobSet) {
    if (jobSet.jobs.isEmpty) {
      logInfo("No jobs added for time " + jobSet.time)
    } else {
      listenerBus.post(StreamingListenerBatchSubmitted(jobSet.toBatchInfo))
      jobSets.put(jobSet.time, jobSet)
      // 每个 job 都在 jobExecutor 线程池中，用 new JobHandler 来处理
      jobSet.jobs.foreach(job => jobExecutor.execute(new JobHandler(job)))
      logInfo("Added jobs for time " + jobSet.time)
    }
}
```

```scala
private class JobHandler(job: Job) extends Runnable with Logging {
    import JobScheduler._

    def run() {
      val oldProps = ssc.sparkContext.getLocalProperties
      try {
        ...
        var _eventLoop = eventLoop
        if (_eventLoop != null) {
          // 发布消息 JobStarted
          _eventLoop.post(JobStarted(job, clock.getTimeMillis()))
          // Disable checks for existing output directories in jobs launched by the streaming
          // scheduler, since we may need to write output to an existing directory during checkpoint
          // recovery; see SPARK-4835 for more details.
          SparkHadoopWriterUtils.disableOutputSpecValidation.withValue(true) {
            // 调用job.run()
            //  def run() {
            //    _result = Try(func())
            //  }
            job.run()
          }
          _eventLoop = eventLoop
          if (_eventLoop != null) {
            _eventLoop.post(JobCompleted(job, clock.getTimeMillis()))
          }
        } else {
          // JobScheduler has been stopped.
        }
      } finally {
        ssc.sparkContext.setLocalProperties(oldProps)
      }
    }
  }
```

`job` 的运行逻辑在 `ForEachDstream.generateJob(time)` 时定义，即定义了 `job.func`，而在 `JobHandler` 这里是真正调用了 `Job.run()`

接下来我们分析 `JobScheduler` 有第二个成员： `ReceiverTracker`。`JobScheduler` 将源头输入数据的记录工作委托给 `ReceiverTracker`。

[看这里 - ReceiverTracker](/ReceiverTracker.md)