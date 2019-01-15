# ReceiverTracker, ReceivedBlockTracker 详解

## ReceiverTracker

`executor` 端 `Receiver` 接收到数据后最终会调用 `ReceiverSupervisorImpl` 的 `pushAndReportBlock` 方法，该方法中会向 `driver` 端 `ReceiverTracker` 上报 `AddBlock` 事件，其中包含 meta 信息，这里的meta信息包含数据的标识 id，数据的位置，数据的条数，数据的大小等信息。

```scala
// ReceiverSupervisorImpl 类
/** Store block and report it to driver */
def pushAndReportBlock(
    receivedBlock: ReceivedBlock,
    metadataOption: Option[Any],
    blockIdOption: Option[StreamBlockId]
) {
    val blockId = blockIdOption.getOrElse(nextBlockId)
    val time = System.currentTimeMillis
    val blockStoreResult = receivedBlockHandler.storeBlock(blockId, receivedBlock)
    logDebug(s"Pushed block $blockId in ${(System.currentTimeMillis - time)} ms")
    val numRecords = blockStoreResult.numRecords
    val blockInfo = ReceivedBlockInfo(streamId, numRecords, metadataOption, blockStoreResult)

    // 向ReceiverTracker发送AddBlock事件
    trackerEndpoint.askSync[Boolean](AddBlock(blockInfo))
    logDebug(s"Reported block $blockId")
}
```

`ReceiverTracker` 收到 `AddBlock` 事件后会先判断数据的存储策略

```scala
// ReceiverTracker 的 receiveAndReply 方法

case AddBlock(receivedBlockInfo) =>
    if (WriteAheadLogUtils.isBatchingEnabled(ssc.conf, isDriver = true)) {
        walBatchingThreadPool.execute(new Runnable {
        override def run(): Unit = Utils.tryLogNonFatalError {
            if (active) {
            context.reply(addBlock(receivedBlockInfo))
            } else {
            throw new IllegalStateException("ReceiverTracker RpcEndpoint shut down.")
            }
        }
        })
    } else {
        context.reply(addBlock(receivedBlockInfo))
}
```

统一调用方法 `addBlock`，将 `receivedBlockInfo` 传递给 `receivedBlockTracker`，由 `ReceivedBlockTracker` 专门管理收到的块数据 meta 信息。

```scala
/** Add new blocks for the given stream */
private def addBlock(receivedBlockInfo: ReceivedBlockInfo): Boolean = {
    receivedBlockTracker.addBlock(receivedBlockInfo)
}
```

至此，结合器前面几篇分析，可以总结 `ReceiverTracker` 的三个主要功能为：

- (1) `ReceiverTracker` 分发和监控 `Receiver`
  - `ReceiverTracker` 负责 `Receiver` 在各个 executor 上的分发
  - 以及 `Receiver` 的失败重启
- (2) `ReceiverTracker` 作为 RpcEndpoint
  - `ReceiverTracker` 作为 `Receiver` 的管理者，是各个 `Receiver` 上报信息的入口
  - 也是 driver 下达管理命令到 `Receiver` 的出口
- (3) `ReceiverTracker` 管理已上报的块数据 meta 信息

整体来看，ReceiverTracker 就是 Receiver 相关信息的中枢。

## ReceivedBlockTracker

`ReceivedBlockTracker` 专门负责已上报的块数据 meta 信息管理，但 `ReceivedBlockTracker` 本身不负责对外交互，一切都是通过 `ReceiverTracker` 来转发 —— 这里 `ReceiverTracker` 相当于是 `ReceivedBlockTracker` 的门面

在 `ReceivedBlockTracker` 类中包含以下几个私有成员变量

```scala
// 维护了上报上来的、但尚未分配入 batch 的 Block 块数据的 meta
// 为每个 Receiver 单独维护一个 queue，所以是一个 HashMap：receiverId → mutable.Queue[ReceivedBlockInfo]
private val streamIdToUnallocatedBlockQueues = new mutable.HashMap[Int, ReceivedBlockQueue]

// 维护了上报上来的、已分配入 batch 的 Block 块数据的 meta
// 按照 batch 进行一级索引、再按照 receiverId 进行二级索引的 queue，所以是一个 HashMap: time → HashMap
private val timeToAllocatedBlocks = new mutable.HashMap[Time, AllocatedBlocks]

// 记录了最近一个分配完成的 batch 是哪个
private var lastAllocatedBatchTime: Time = null
```

上面是用于状态记录的主要数据结构。对这些状态存取主要是 4 个方法：

方法一：`addBlock(receivedBlockInfo: ReceivedBlockInfo)`

- 收到某个 Receiver 上报上来的块数据 meta 信息，将其加入到 streamIdToUnallocatedBlockQueues 里

```scala
/** Add received block. This event will get written to the write ahead log (if enabled). */
// 收到某个 Receiver 上报上来的块数据 meta 信息，将其加入到 streamIdToUnallocatedBlockQueues 里
def addBlock(receivedBlockInfo: ReceivedBlockInfo): Boolean = {
    try {
        val writeResult = writeToLog(BlockAdditionEvent(receivedBlockInfo))
        if (writeResult) {
        synchronized {
                getReceivedBlockQueue(receivedBlockInfo.streamId) += receivedBlockInfo
            }
            logDebug(s"Stream ${receivedBlockInfo.streamId} received " +
                s"block ${receivedBlockInfo.blockStoreResult.blockId}")
        } else {
        logDebug(s"Failed to acknowledge stream ${receivedBlockInfo.streamId} receiving " +
            s"block ${receivedBlockInfo.blockStoreResult.blockId} in the Write Ahead Log.")
        }
        writeResult
    } catch {
        case NonFatal(e) =>
        logError(s"Error adding block $receivedBlockInfo", e)
        false
    }
}


/** Get the queue of received blocks belonging to a particular stream */
private def getReceivedBlockQueue(streamId: Int): ReceivedBlockQueue = {
    streamIdToUnallocatedBlockQueues.getOrElseUpdate(streamId, new ReceivedBlockQueue)
}
```

方法二：`allocateBlocksToBatch(batchTime: Time)`

- 主要是 JobGenerator 在发起新 batch 的计算时，第一步就调用本方法
- 是将 streamIdToUnallocatedBlockQueues 的内容，以传入的 batchTime 参数为 key，添加到 timeToAllocatedBlocks 里
- 并更新 lastAllocatedBatchTime

```scala
// JobGenerator 类的 generateJobs 方法
private def generateJobs(time: Time) {
    ...
    Try {
        // 调用 allocateBlocksToBatch
        jobScheduler.receiverTracker.allocateBlocksToBatch(time) // allocate received blocks to batch
        graph.generateJobs(time) // generate jobs using allocated block
    } match {
        ...
    }
    ...
}
```

方法三：`getBlocksOfBatch(batchTime: Time)`

- 主要是 `JobGenerator` 在发起新 batch 的计算时，由 DStreamGraph 生成 RDD DAG 实例时，将调用本方法
- 调用本方法查 `timeToAllocatedBlocks`，获得划入本 batch 的块数据 meta 信息，由此生成处理对应块数据的 RDD

```scala
// ReceiverInputDStream 类

/**
* Generates RDDs with blocks received by the receiver of this stream. */
override def compute(validTime: Time): Option[RDD[T]] = {
    val blockRDD = {

        if (validTime < graph.startTime) {
        // If this is called for any time before the start time of the context,
        // then this returns an empty RDD. This may happen when recovering from a
        // driver failure without any write ahead log to recover pre-failure data.
        new BlockRDD[T](ssc.sc, Array.empty)
        } else {
        // Otherwise, ask the tracker for all the blocks that have been allocated to this stream
        // for this batch
        val receiverTracker = ssc.scheduler.receiverTracker

        // 调用 getBlockOfBatch 获取划入本 batch 的块数据 meta 信息
        val blockInfos = receiverTracker.getBlocksOfBatch(validTime).getOrElse(id, Seq.empty)

        // Register the input blocks information into InputInfoTracker
        val inputInfo = StreamInputInfo(id, blockInfos.flatMap(_.numRecords).sum)
        ssc.scheduler.inputInfoTracker.reportInfo(validTime, inputInfo)

        // Create the BlockRDD
        createBlockRDD(validTime, blockInfos)
        }
    }
    Some(blockRDD)
}
```

方法四：`cleanupOldBatches(cleanupThreshTime: Time, ...)`

- 主要是当一个 batch 已经计算完成、可以把已追踪的块数据的 meta 信息清理掉时调用
- 将清理 `timeToAllocatedBlocks` 表里对应 `cleanupThreshTime` 之前的所有 batch 块数据 meta 信息