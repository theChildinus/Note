# ReceiverSupervisor 详解

即上篇 [数据产生 - Receiver分发和启动](/Receiver分发和启动.md) 成功启动Receiver后，Receiver开始接收数据，接受到的数据会通过 `store` 方法进行存储，如下图：

![数据流](/image/storeandpush.png)

例如在 `SocketReceiver` 中，创建子线程接收数据，调用 `receive` 方法，该方法监听数据流，有新数据调用 `store()` 方法：

```scala
/** Create a socket connection and receive data until receiver is stopped */
def receive() {
    try {
        val iterator = bytesToObjects(socket.getInputStream())
        while(!isStopped && iterator.hasNext) {
            // 存储数据
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
```

在 `ReceiverSuppervisor` 中声明了四种push方法

```scala
// 类 `ReceiverSupervisor` 中定义

/** Push a single data item to backend data store. */
def pushSingle(data: Any): Unit

/** Store the bytes of received data as a data block into Spark's memory. */
def pushBytes(
    bytes: ByteBuffer,
    optionalMetadata: Option[Any],
    optionalBlockId: Option[StreamBlockId]
): Unit

/** Store an iterator of received data as a data block into Spark's memory. */
def pushIterator(
    iterator: Iterator[_],
    optionalMetadata: Option[Any],
    optionalBlockId: Option[StreamBlockId]
): Unit

/** Store an ArrayBuffer of received data as a data block into Spark's memory. */
def pushArrayBuffer(
    arrayBuffer: ArrayBuffer[_],
    optionalMetadata: Option[Any],
    optionalBlockId: Option[StreamBlockId]
): Unit
```

在 `ReceiverSupervisorImpl` 中 重写了这四个方法，第一个方法 `pushSingle()` 用于处理单条小数据，需要借助 `BlockGenerator` ， 将多个单条数据积攒为一个块数据，剩余的三个方法用于处理块数据

## 三个块数据处理方法

先来看处理块数据的三个方法：

```scala
// 类 ReceiverSupervisorImpl 中

/** Store an ArrayBuffer of received data as a data block into Spark's memory. */
def pushArrayBuffer(
    arrayBuffer: ArrayBuffer[_],
    metadataOption: Option[Any],
    blockIdOption: Option[StreamBlockId]
) {
    pushAndReportBlock(ArrayBufferBlock(arrayBuffer), metadataOption, blockIdOption)
}

/** Store an iterator of received data as a data block into Spark's memory. */
def pushIterator(
    iterator: Iterator[_],
    metadataOption: Option[Any],
    blockIdOption: Option[StreamBlockId]
) {
    pushAndReportBlock(IteratorBlock(iterator), metadataOption, blockIdOption)
}

/** Store the bytes of received data as a data block into Spark's memory. */
def pushBytes(
    bytes: ByteBuffer,
    metadataOption: Option[Any],
    blockIdOption: Option[StreamBlockId]
) {
    pushAndReportBlock(ByteBufferBlock(bytes), metadataOption, blockIdOption)
}
```

这三个 `push` 方法即将自己的数据统一包装成 `ReceivedBlock` 对应的子类，然后调用 `pushAndReportBlock()` 方法

`pushAndReportBlock` 方法做两件事情：

- (1) push：将 `ReceivedBlock` 交给 `ReceivedBlockHandler` 来存储，具体的，可以在 `ReceivedBlockHandler` 的两种存储实现里二选一
- (2) report：将已存储好的 `ReceivedBlock` 的块数据 meta 信息报告给 `ReceiverTracker`

```scala
/** Store block and report it to driver */
def pushAndReportBlock(
    receivedBlock: ReceivedBlock,
    metadataOption: Option[Any],
    blockIdOption: Option[StreamBlockId]
) {
    val blockId = blockIdOption.getOrElse(nextBlockId)
    val time = System.currentTimeMillis
    // 调用 ReceivedBlockHandler 存储数据
    val blockStoreResult = receivedBlockHandler.storeBlock(blockId, receivedBlock)
    logDebug(s"Pushed block $blockId in ${(System.currentTimeMillis - time)} ms")
    val numRecords = blockStoreResult.numRecords

    // 构造 AddBlock 事件 发送给 ReceiverTracker
    val blockInfo = ReceivedBlockInfo(streamId, numRecords, metadataOption, blockStoreResult)
    trackerEndpoint.askSync[Boolean](AddBlock(blockInfo))
    logDebug(s"Reported block $blockId")
}
```

变量 `receivedBlockHandler` 定义，根据环境配置决定采用哪种存储
策略

- (1) `BlockManagerBasedBlockHandler`，是直接存到 executor 的内存或硬盘
- (2) `WriteAheadLogBasedBlockHandler`，是先写 WAL，再存储到 executor 的内存或硬盘

```scala
private val receivedBlockHandler: ReceivedBlockHandler = {
    if (WriteAheadLogUtils.enableReceiverLog(env.conf)) {
        if (checkpointDirOption.isEmpty) {
        throw new SparkException(
            "Cannot enable receiver write-ahead log without checkpoint directory set. " +
            "Please use streamingContext.checkpoint() to set the checkpoint directory. " +
            "See documentation for more details.")
        }
        new WriteAheadLogBasedBlockHandler(env.blockManager, env.serializerManager, receiver.streamId,
        receiver.storageLevel, env.conf, hadoopConf, checkpointDirOption.get)
    } else {
        new BlockManagerBasedBlockHandler(env.blockManager, receiver.storageLevel)
    }
}
```

### (1) BlockManagerBasedBlockHandler 实现

`BlockManagerBasedBlockHandler` 主要直接存储到Spark Core 里的 `BlockManager` 里

`BlockManager` 将在 executor 端接收 Block 数据，而在 driver 端维护 Block 的 meta 信息。 BlockManager 根据存储者的 `StorageLevel` 要求来存到本 executor 的 RAM 或者 DISK，也可以同时再额外复制一份到其它 executor 的 RAM 或者 DISK。

```scala
// 内部类 BlockManagerBasedBlockHandler 的 storeBlock 方法

/**
 * Implementation of a [[org.apache.spark.streaming.receiver.ReceivedBlockHandler]] which
 * stores the received blocks into a block manager with the specified storage level.
 */
def storeBlock(blockId: StreamBlockId, block: ReceivedBlock): ReceivedBlockStoreResult = {

    var numRecords: Option[Long] = None

    val putSucceeded: Boolean = block match {
        // 存储数组到 blockManager 里
      case ArrayBufferBlock(arrayBuffer) =>
        numRecords = Some(arrayBuffer.size.toLong)
        blockManager.putIterator(blockId, arrayBuffer.iterator, storageLevel,
          tellMaster = true)
          // 存储 iterator 到 blockManager 里
      case IteratorBlock(iterator) =>
        val countIterator = new CountingIterator(iterator)
        val putResult = blockManager.putIterator(blockId, countIterator, storageLevel,
          tellMaster = true)
        numRecords = countIterator.count
        putResult
        // 存储 ByteBuffer 到 blockManager 里
      case ByteBufferBlock(byteBuffer) =>
        blockManager.putBytes(
          blockId, new ChunkedByteBuffer(byteBuffer.duplicate()), storageLevel, tellMaster = true)
      case o =>
        throw new SparkException(
          s"Could not store $blockId to block manager, unexpected block type ${o.getClass.getName}")
    }
    if (!putSucceeded) {
      throw new SparkException(
        s"Could not store $blockId to block manager with storage level $storageLevel")
    }
    BlockManagerBasedStoreResult(blockId, numRecords)
  }
```

### (2) WriteAheadLogBasedBlockHandler 实现

`WriteAheadLogBasedBlockHandler` 的实现则是同时写到可靠存储的 WAL 中和 executor 的 `BlockManager` 中；在两者都写完成后，再上报块数据的 meta 信息。

`BlockManager` 中的块数据是计算时首选使用的，只有在 executor 失效时，才去 WAL 中读取写入过的数据。

同其它系统的 WAL 一样，数据是完全顺序地写入 WAL 的；在稍后上报块数据的 meta 信息，就额外包含了块数据所在的 WAL 的路径，及在 WAL 文件内的偏移地址和长度。

```scala
// 内部类 WriteAheadLogBasedBlockHandler 的 storeBlock 方法

/**
* This implementation stores the block into the block manager as well as a write ahead log.
* It does this in parallel, using Scala Futures, and returns only after the block has
* been stored in both places.
*/
def storeBlock(blockId: StreamBlockId, block: ReceivedBlock): ReceivedBlockStoreResult = {

    var numRecords = Option.empty[Long]
    // Serialize the block so that it can be inserted into both
    val serializedBlock = block match {
        case ArrayBufferBlock(arrayBuffer) =>
        numRecords = Some(arrayBuffer.size.toLong)
        serializerManager.dataSerialize(blockId, arrayBuffer.iterator)
        case IteratorBlock(iterator) =>
        val countIterator = new CountingIterator(iterator)
        val serializedBlock = serializerManager.dataSerialize(blockId, countIterator)
        numRecords = countIterator.count
        serializedBlock
        case ByteBufferBlock(byteBuffer) =>
        new ChunkedByteBuffer(byteBuffer.duplicate())
        case _ =>
        throw new Exception(s"Could not push $blockId to block manager, unexpected block type")
    }

    // Store the block in block manager
    // 生成向 BlockManager 存储数据的 future
    val storeInBlockManagerFuture = Future {
        val putSucceeded = blockManager.putBytes(
        blockId,
        serializedBlock,
        effectiveStorageLevel,
        tellMaster = true)
        if (!putSucceeded) {
        throw new SparkException(
            s"Could not store $blockId to block manager with storage level $storageLevel")
        }
    }

    // Store the block in write ahead log
    // 生成向 WAL 存储数据的 future
    val storeInWriteAheadLogFuture = Future {
        writeAheadLog.write(serializedBlock.toByteBuffer, clock.getTimeMillis())
    }

    // Combine the futures, wait for both to complete, and return the write ahead log record handle
    // 开始执行两个 future、等待两个 future 都结束
    val combinedFuture = storeInBlockManagerFuture.zip(storeInWriteAheadLogFuture).map(_._2)
    val walRecordHandle = ThreadUtils.awaitResult(combinedFuture, blockStoreTimeout)

    // 返回存储结果，用于后续的块数据 meta 上报
    WriteAheadLogBasedStoreResult(blockId, numRecords, walRecordHandle)
}
```

## 单条小数据处理方法

最后我们来补充一下 `ReceiverSupervisorImpl` 在收到单块条小数据后，委托 `BlockGenerator` 进行积攒，并封装多条小数据为一整个块数据的详细过程。

在 `ReceiverSupervisorImpl` 中，方法 `pushSingle()` 用于处理单条小数据

`BlockGenerator` 类描述时有：

> Note: Do not create BlockGenerator instances directly inside receivers. Use `ReceiverSupervisor.createBlockGenerator` to create a BlockGenerator and use it.

```scala
// 类 ReceiverSupervisorImpl 中
// 创建 BlockGenerator 类
private val defaultBlockGenerator = createBlockGenerator(defaultBlockGeneratorListener)

/** Push a single record of received data into block generator. */
def pushSingle(data: Any) {
    defaultBlockGenerator.addData(data)
}
```

在 `addData()` 方法中，首先会调用 `waitToPush()` 方法，该方法会在加入 `currentBuffer` 数组之前由 `rateLimiter` 检查一下速率，是否加入的频率已经太高。如果太高的话，就需要 block 住，等到下一秒再开始添加。这里的最高频率是由 spark.streaming.receiver.maxRate (default = Long.MaxValue) 控制的，是单个 Receiver 每秒钟允许添加的条数，控制了这个速率，就控制了整个 Spark Streaming 系统每个 batch 需要处理的最大数据量。1.5 依赖加入了动态控制每个 `Receiver` 速率的特性

```scala
// 类 BlockGenerator
/**
* Push a single data item into the buffer.
*/
def addData(data: Any): Unit = {
    if (state == Active) {
        // 检查加入速率
        waitToPush()
        synchronized {
        if (state == Active) {
            // BlockGenerator 在内部主要是维护一个临时的变长数组 currentBuffer，每收到一条 ReceiverSupervisorImpl 转发来的数据就加入到这个 currentBuffer 数组中
            currentBuffer += data
        } else {
            throw new SparkException(
            "Cannot add data as BlockGenerator has not been started or has been stopped")
        }
        }
    } else {
        throw new SparkException(
        "Cannot add data as BlockGenerator has not been started or has been stopped")
    }
}
```

在 `BlockGenerator` 类中定义的成员变量有：

```scala
private val blockIntervalMs = conf.getTimeAsMs("spark.streaming.blockInterval", "200ms")
require(blockIntervalMs > 0, s"'spark.streaming.blockInterval' should be a positive value")

// 定时器
private val blockIntervalTimer =
new RecurringTimer(clock, blockIntervalMs, updateCurrentBuffer, "BlockGenerator")
private val blockQueueSize = conf.getInt("spark.streaming.blockQueueSize", 10)

// blocksForPushing 队列存储老数据，即已经足够大，可以进行存储
private val blocksForPushing = new ArrayBlockingQueue[Block](blockQueueSize)

// blockPushingThread 线程专门从 blockForPushing 队列里取出来已经包装好的块数据
private val blockPushingThread = new Thread() { override def run() { keepPushingBlocks() } }
```

`BlockGenerator` 类维护一个定时器，每隔 `blockInterval` 的时间就生成一个新的空变长数组替换老的数组作为新的 `currentBuffer` ，并把老的数组加入到一个自己的一个 `blocksForPushing` 的队列里。

这个 `blocksForPushing` 队列实际上是一个 `ArrayBlockingQueue`，大小由 `spark.streaming.blockQueueSize(默认 = 10)` 来控制

如定时器定时任务 `updateCurrentBuffer` 中实现的：

```scala
/** Change the buffer to which single records are added to. */
private def updateCurrentBuffer(time: Long): Unit = {
    try {
        var newBlock: Block = null
        synchronized {
            if (currentBuffer.nonEmpty) {
                val newBlockBuffer = currentBuffer
                currentBuffer = new ArrayBuffer[Any]
                val blockId = StreamBlockId(receiverId, time - blockIntervalMs)
                listener.onGenerateBlock(blockId)
                newBlock = new Block(blockId, newBlockBuffer)
            }
        }

        if (newBlock != null) {
            blocksForPushing.put(newBlock)  // put is blocking when queue is full
        }
    } catch {
        case ie: InterruptedException =>
        logInfo("Block updating timer thread was interrupted")
        case e: Exception =>
        reportError("Error in block updating thread", e)
    }
}
```

`blockPushingThread` 线程专门从 `blockForPushing` 队列里取出来已经包装好的块数据

由 `blockPushingThread` 线程的执行任务 `keepPushingBlocks` 可见在类 `BlockGenerator` 中最终会调用方法 `pushBlock()` 发送数据：

```scala
/** Keep pushing blocks to the BlockManager. */
private def keepPushingBlocks() {
    logInfo("Started block pushing thread")

    def areBlocksBeingGenerated: Boolean = synchronized {
        state != StoppedGeneratingBlocks
    }

    try {
        // While blocks are being generated, keep polling for to-be-pushed blocks and push them.
        while (areBlocksBeingGenerated) {
        Option(blocksForPushing.poll(10, TimeUnit.MILLISECONDS)) match {
            case Some(block) => pushBlock(block)
            case None =>
        }
        }

        // At this point, state is StoppedGeneratingBlock. So drain the queue of to-be-pushed blocks.
        logInfo("Pushing out the last " + blocksForPushing.size() + " blocks")
        while (!blocksForPushing.isEmpty) {
            val block = blocksForPushing.take()
            logDebug(s"Pushing block $block")
            pushBlock(block)
            logInfo("Blocks left to push " + blocksForPushing.size())
        }
        logInfo("Stopped block pushing thread")
    } catch {
        case ie: InterruptedException =>
        logInfo("Block pushing thread was interrupted")
        case e: Exception =>
        reportError("Error in block pushing thread", e)
    }
}
```

在 `pushBlock()` 方法中，会调用接口 `BlockGeneratorListener` 的方法 `onPushBlock()`

```scala
private def pushBlock(block: Block) {
    listener.onPushBlock(block.id, block.buffer)
    logInfo("Pushed block " + block.id)
}
```

接口 `BlockGeneratorListener` 的具体实现在类 `ReceiverSupervisorImpl` 中：

```scala
/** Divides received data records into data blocks for pushing in BlockManager. */
private val defaultBlockGeneratorListener = new BlockGeneratorListener {
    def onAddData(data: Any, metadata: Any): Unit = { }

    def onGenerateBlock(blockId: StreamBlockId): Unit = { }

    def onError(message: String, throwable: Throwable) {
        reportError(message, throwable)
    }

    def onPushBlock(blockId: StreamBlockId, arrayBuffer: ArrayBuffer[_]) {
        pushArrayBuffer(arrayBuffer, None, Some(blockId))
    }
}
```

至此，`blockPushingThread` 线程专门从 `blockForPushing` 队列里取出来已经包装好的块数据，最终调用 `ReceiverSupervisorImpl.pushArrayBuffer(...)` 来将块数据交回给 `ReceiverSupervisorImpl`