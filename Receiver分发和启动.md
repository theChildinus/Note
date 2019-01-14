# Receiver分发和启动 解析

## Receiver 分发

启动：

```scala
// JobScheduler类中定义：
var receiverTracker: ReceiverTracker = null

// JobScheduler.start()
def start(): Unit = synchronized {
    ...
    // 调用了 ReceiverTracker 的 start()
    receiverTracker.start()
    ...
}
```

在 `ReceiverTrakcer` 类中：

```scala
  // endpoint is created when generator starts.
  // This not being null means the tracker has been started and not stopped
private var endpoint: RpcEndpointRef = null

/** Start the endpoint and receiver execution thread. */
def start(): Unit = synchronized {
    if (isTrackerStarted) {
        throw new SparkException("ReceiverTracker already started")
    }

    if (!receiverInputStreams.isEmpty) {
        // 用内部类 ReceiverTrackerEndpoint 初始化 endpoint，用来接收和处理来自 ReceiverTracker 和 receivers 发送的消息
        endpoint = ssc.env.rpcEnv.setupEndpoint(
        "ReceiverTracker", new ReceiverTrackerEndpoint(ssc.env.rpcEnv))
        // 将各个 receivers 分发到 executors 上
        if (!skipReceiverLaunch) launchReceivers()
        logInfo("ReceiverTracker started")
        trackerState = Started
    }
}
```

在 `launchReceivers()` 中：

```scala
// receiverInputStreams 定义
private val receiverInputStreams = ssc.graph.getReceiverInputStreams()
/**
* Get the receivers from the ReceiverInputDStreams, distributes them to the
* worker nodes as a parallel collection, and runs them.
*/
private def launchReceivers(): Unit = {

    // 第一步：
    // 获取DStreamGraph.inputStreams 中继承了 ReceiverInputDStream 的input streams 的 receivers，即数据接收器
    // receiverInputStreams 的定义是一个 Array，所以这里返回的 receivers 也是一个 Array
    val receivers = receiverInputStreams.map { nis =>
        // 获取Receiver
        val rcvr = nis.getReceiver()
        rcvr.setReceiverId(nis.id)
        rcvr
    }

    runDummySparkJob()

    logInfo("Starting " + receivers.length + " receivers")
    // 第二步：
    // 给 消息接收处理器 endpoint 发送 StartAllReceivers 事件，直接返回，不等待消息被处理
    endpoint.send(StartAllReceivers(receivers))
}
```

第一步中：

```scala
/**
* Gets the receiver object that will be sent to the worker nodes
* to receive data. This method needs to defined by any specific implementation
* of a ReceiverInputDStream.
*/
def getReceiver(): Receiver[T]
```

以 `SocketInputDStream` 为例：

```scala
private[streaming]
class SocketInputDStream[T: ClassTag](
    _ssc: StreamingContext,
    host: String,
    port: Int,
    bytesToObjects: InputStream => Iterator[T],
    storageLevel: StorageLevel
  ) extends ReceiverInputDStream[T](_ssc) {

// 实现 `getReceiver` 方法
  def getReceiver(): Receiver[T] = {
      // 创建 SocketReceiver
    new SocketReceiver(host, port, bytesToObjects, storageLevel)
  }
}

private[streaming]
class SocketReceiver[T: ClassTag](
    host: String,
    port: Int,
    bytesToObjects: InputStream => Iterator[T],
    storageLevel: StorageLevel
  ) extends Receiver[T](storageLevel) with Logging {

  private var socket: Socket = _

// 实现父类 Receiver的方法
  def onStart() {
    try {
      socket = new Socket(host, port)
    } catch {
      ...
    }

    // Start the thread that receives data over a connection
    // 新起一个线程来接收数据，onStart方法很快就返回了
    new Thread("Socket Receiver") {
      setDaemon(true)
      override def run() { receive() }
    }.start()
  }

  def onStop() {
    // in case restart thread close it twice
    ...
  }

  /** Create a socket connection and receive data until receiver is stopped */
  def receive() {
    try {
      val iterator = bytesToObjects(socket.getInputStream())
      while(!isStopped && iterator.hasNext) {
        store(iterator.next())
      }
      if (!isStopped()) {
        restart("Socket data stream had no more data")
      } else {
        logInfo("Stopped receiving")
      }
    } catch {
      case NonFatal(e) =>
        logWarning("Error receiving data", e)
        restart("Error receiving data", e)
    } finally {
      onStop()
    }
  }
}
```

第二步中：

事件 `StartAllReceiver` 的定义：

```scala
/**
 * This message is sent to ReceiverTrackerEndpoint when we start to launch Spark jobs for receivers
 * at the first time.
 */
private[streaming] case class StartAllReceivers(receiver: Seq[Receiver[_]])
  extends ReceiverTrackerLocalMessage
```

父类 `ReceiverTrackerLocalMessage`

```scala
/**
 * Messages used by the driver and ReceiverTrackerEndpoint to communicate locally.
 */
private[streaming] sealed trait ReceiverTrackerLocalMessage
```

可见 `StartAllReceiver` 等事件是用于本地通信

endpoint 接收到 `StartAllReceiver` 事件，会调用 `ReceiverTrackerEndpoint` 内部类中的 `receive` 方法：

```scala
override def receive: PartialFunction[Any, Unit] = {
      // Local messages
    case StartAllReceivers(receivers) =>
    //ReceiverSchedulingPolicy 的主要目的，是在 Spark Streaming 层面添加对 Receiver 的分发目的地的计算，相对于之前版本依赖 Spark Core 的 TaskScheduler 进行通用分发，新的 ReceiverSchedulingPolicy 会对 Streaming 应用的更好的语义理解，也能计算出更好的分发策略。
    // scheduledLocations 是一个 Map
    // receivers 是之前获取的 所有 数据接收器
    // getExecutors 获取所有的executor 作为候选目的地
        val scheduledLocations = schedulingPolicy.scheduleReceivers(receivers, getExecutors)
        for (receiver <- receivers) {
            val executors = scheduledLocations(receiver.streamId)
            updateReceiverScheduledExecutors(receiver.streamId, executors)
            // 如果 receiver 的preferredLocation本身不均匀，以preferredLocation为准
            receiverPreferredLocations(receiver.streamId) = receiver.preferredLocation
            // 遍历每个 receiver，根据第1步中得到的目的 executors 调用 startReceiver 方法
            startReceiver(receiver, executors)
        }
    ...
```

`startReceiver` 方法：

```scala
/**
    * Start a receiver along with its scheduled executors
    */
// 一个ReceiverInputDStream 实例只有一个receiver，但receiver可能需要在多个workers上启动接收数据的线程
private def startReceiver(
    receiver: Receiver[_],
    scheduledLocations: Seq[TaskLocation]): Unit = {
    def shouldStartReceiver: Boolean = {
    // It's okay to start when trackerState is Initialized or Started
    !(isTrackerStopping || isTrackerStopped)
    }

    val receiverId = receiver.streamId
    if (!shouldStartReceiver) {
    onReceiverJobFinish(receiverId)
    return
    }

    val checkpointDirOption = Option(ssc.checkpointDir)
    val serializableHadoopConf = new SerializableConfiguration(ssc.sparkContext.hadoopConfiguration)

    // Function to start the receiver on the worker node
    // 定义要在 receiverRDD上执行的 startReceiverFunc
    val startReceiverFunc: Iterator[Receiver[_]] => Unit =
    (iterator: Iterator[Receiver[_]]) => {
        if (!iterator.hasNext) {
        throw new SparkException(
            "Could not start receiver as object not found.")
        }
        if (TaskContext.get().attemptNumber() == 0) {
            val receiver = iterator.next()
            assert(iterator.hasNext == false)
            // 该函数在 executor上 新建一个 ReceiverSupervisorImpl实例 supervisor
            // 调用supervisor.start() 来启动 receiver，并阻塞，这里传入 receiver
            val supervisor = new ReceiverSupervisorImpl(
                receiver, SparkEnv.get, serializableHadoopConf.value, checkpointDirOption)
            supervisor.start()
            supervisor.awaitTermination()
        } else {
        // It's restarted by TaskScheduler, but we want to reschedule it again. So exit it.
        }
    }

    // Create the RDD using the scheduledLocations to run the receiver in a Spark job
    // 构造 Receiver RDD，将 receiver对应的目的executors转换成RDD
    val receiverRDD: RDD[Receiver[_]] =
    if (scheduledLocations.isEmpty) {
        ssc.sc.makeRDD(Seq(receiver), 1)
    } else {
        val preferredLocations = scheduledLocations.map(_.toString).distinct
        ssc.sc.makeRDD(Seq(receiver -> preferredLocations))
    }
    receiverRDD.setName(s"Receiver $receiverId")
    ssc.sparkContext.setJobDescription(s"Streaming job running receiver $receiverId")
    ssc.sparkContext.setCallSite(Option(ssc.getStartSite()).getOrElse(Utils.getCallSite()))

// 分发RDD 和 func 到具体的executor上
    val future = ssc.sparkContext.submitJob[Receiver[_], Unit, Unit](
    receiverRDD, startReceiverFunc, Seq(0), (_, _) => Unit, ())
    // We will keep restarting the receiver job until ReceiverTracker is stopped
    future.onComplete {
    case Success(_) =>
        if (!shouldStartReceiver) {
        onReceiverJobFinish(receiverId)
        } else {
        logInfo(s"Restarting Receiver $receiverId")
        self.send(RestartReceiver(receiver))
        }
    case Failure(e) =>
        if (!shouldStartReceiver) {
        onReceiverJobFinish(receiverId)
        } else {
        logError("Receiver has been stopped. Try to restart it.", e)
        logInfo(s"Restarting Receiver $receiverId")
        self.send(RestartReceiver(receiver))
        }
    }(ThreadUtils.sameThread)
    logInfo(s"Receiver ${receiver.streamId} started")
}
```

这里 Spark Streaming 下层的 Spark Core 对 Receiver 分发是毫无感知的，它只是执行了“应用层面” -- 对 Spark Core 来讲，Spark Streaming 就是“应用层面”-- 的一个普通 Job；但 Spark Streaming 只通过这个普通 Job 即可完“特殊功能”的 Receiver 分发，可谓巧妙巧妙。

Spark 1.4 的 `Receiver` 的消息分发存在这样的问题：

- 如果某个 Task 失败超过 spark.task.maxFailures(默认=4) 次的话，整个 Job 就会失败。这个在长时运行的 Spark Streaming 程序里，Executor 多失效几次就有可能导致 Task 失败达到上限次数了。重跑executor可能导致 Receiver分发不平衡

从 1.5.0 开始，Spark Streaming 添加了增强的 Receiver 分发策略。对比之前的版本，主要的变更在于：

- 添加可插拔的 ReceiverSchedulingPolicy
- 把 1 个 Job（包含 x 个 Task），改为 x 个 Job（每个 Job 只包含 1 个 Task）
- 添加对 Receiver 的监控重启机制

## Receiver 启动

```scala
// 父类 ReceiverSupervisor
// 实现类 ReceiverSupervisorImpl 没有实现 start() 方法，故调用父类的start方法
/** Start the supervisor */
def start() {
    onStart()
    startReceiver()
}
```

`onStart` 声明，`BlockGenerator` 用于聚集多条细小数据

```scala
/**
* Called when supervisor is started.
* Note that this must be called before the receiver.onStart() is called to ensure
* things like [[BlockGenerator]]s are started before the receiver starts sending data.
*/
protected def onStart() { }
```

父类 `ReceiverSupervisor`

```scala
def startReceiver(): Unit = synchronized {
    try {
        // onReceiverStart 发送注册事件
        if (onReceiverStart()) {
        logInfo(s"Starting receiver $streamId")
        receiverState = Started
        // 启动 receiver onStart方法直接返回
        receiver.onStart()
        logInfo(s"Called receiver $streamId onStart")
        } else {
        // The driver refused us
        stop("Registered unsuccessfully because Driver refused to start receiver " + streamId, None)
        }
    } catch {
        case NonFatal(t) =>
        stop("Error starting receiver " + streamId, Some(t))
    }
}
```

先看 `onReceiverStart()` 方法，该方法将分发到 executor 上的 Receiver 注册到 ReceiverTracker 上

```scala
/** Called when receiver is started. Return true if the driver accepts us */
// 父类 ReceiverSupervisor 中声明
protected def onReceiverStart(): Boolean


// 子类 ReceiverSupervisorImpl 中实现
/** Remote RpcEndpointRef for the ReceiverTracker */
private val trackerEndpoint = RpcUtils.makeDriverRef("ReceiverTracker", env.conf, env.rpcEnv)

override protected def onReceiverStart(): Boolean = {
    // 向 ReceiverTracker 发送 RegisterReceiver 注册事件
    val msg = RegisterReceiver(
        streamId, receiver.getClass.getSimpleName, host, executorId, endpoint)
    trackerEndpoint.askSync[Boolean](msg)
}
```

再看 `receiver.onStart()`，例如继承了 Receiver 的 `SocketReceiver`

```scala
def onStart() {
    // Start the thread that receives data over a connection
    // 新起一个线程来接收数据，onStart方法很快就返回了
    new Thread("Socket Receiver") {
        setDaemon(true)
        override def run() { receive() }
    }.start()
}
```

至此 Receiver 便真正的启动了，可以开始接收数据，接收的数据怎么存到SparkStreaming中呢？ 来看这篇：