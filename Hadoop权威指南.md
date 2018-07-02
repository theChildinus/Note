# Hadoop权威指南

## 第一章 初始Hadoop

Hadoop 三种运行模式

- 独立（本地）模式：无需运行任何守护进程，所有程序都在同一个JVM上执行
- 伪分布模式：Hadoop守护进程运行在本地机器上，模拟一个小规模的集群
- 全分布模式：Hadoop守护进程运行在一个集群上

###　数据的存储与分析

对多个硬盘中的数据并行进行读／写，有很多问题需要解决

- 硬件故障问题 RAID，replica，HDFS
- 大多数分析任务需要以某种方式结合大部分数据来共同完成分析，MapReduce

Hadoop为我们提供了一个可靠的且可拓展的存储和分析平台

###　查询所有数据

MapReduce是一个批量查询处理器，能够在合理的时间范围内处理针对整个数据集的动态查询

YARN是一个集群资源管理系统，允许任何一个分布式程序（不仅仅是MapReduce）基于Hadoop集群的数据而运行

### 较于其他系统的优势

`寻址时间的提升远远不低于传输速率的提升`

MapReduce适合一次写入，多次读取数据的应用，关系型数据库则更适合持续更新的数据集

- 结构化数据：具有既定格式的实体化数据
- 半结构化数据：比较松散，虽然可能有格式但经常被忽略，所以它只能作为对数据结构的一般性指导
- 非结构化数据：没有什么特别的内部结构，如纯文本或图像数据

Hadoop对后两者非常有效

MapReduce以及Hadoop中其他的处理数据是可以随着数据规模线性伸缩的

高性能计算和网格计算主要使用类似于消息传递接口的API

**数据本地化特性是Hadoop数据处理的核心**

MapReduce三大设计目标

1. 为只需要短短几分钟或几小时就可以完成的作业提供服务
2. 运行于同一个内部有高速网络连接的数据中心内
3. 数据中心内的计算机都是可靠的，专门的硬件

## 第二章 关于MapReduce

`一种可用于数据处理的编程框架`

气象数据集

### **2.3 使用Hadoop来分析数据**

将查询表示成MapReduce作业，完成某种本地端的小规模测试之后，就可以把作业部署到集群上运行

#### map 和 reduce

mapreduce任务过程分为两个处理阶段，map阶段和reduce阶段。每阶段都以键值对作为输入和输出，程序员需要写两个函数：map函数和reduce函数

map函数的作用仅限于提取年份和气温信息，输出经由MapReduce框架处理后，最后发给reduce函数，这个处理过程基于键来对键值对进行排序和分组

![逻辑数据流](/image/mapreduce.png)

Mapper类

四个形参类型，分别制定map函数的输入键\值，输出键\值类型

```java
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapred.MapReduceBase;

public class MaxTemperatureMapper extends Mapper<LongWritable, Text, Text, IntWritable> {

    private static final int MISSING = 9999;

    @Override
    public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
        String line = value.toString();
        String year = line.substring(15,19);
        int airTemperature;
        if (line.charAt(87) == '+') {
            airTemperature = Integer.parseInt(line.substring(88, 92));
        } else {
            airTemperature = Integer.parseInt(line.substring(87, 92));
        }

        String quality = line.substring(92, 93);
        if (airTemperature != MISSING && quality.matches("[01459]")) {
            context.write(new Text(year), new IntWritable(airTemperature));
        }
    }
}
```

Reducer类

reduce函数的输入类型必须匹配map函数的输出类型：Text类型和IntWritable类型

```java
import java.io.IOException;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Reducer;

public class MaxTemperatureReducer
        extends Reducer<Text, IntWritable, Text, IntWritable> {
    @Override
    public void reduce(Text key, Iterable<IntWritable> values, Context context) throws IOException, InterruptedException {
        int maxValue = Integer.MIN_VALUE;
        for (IntWritable value : values) {
            maxValue = Math.max(maxValue, value.get());
        }
        context.write(key, new IntWritable(maxValue));
    }
}
```

运行MapReduce作业

Job 对象指定作业执行规范，我们用它来控制整个作业的运行

```java
import java.io.IOException;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class MaxTemperature {
    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.err.println("Usage: MaxTemperature <input path> <output path>");
            System.exit(-1);
        }
        Job job = new Job();
        // 传递一个类，Hadoop利用这个类来查找包含它的jar文件，进而找到香瓜你的jar文件
        job.setJarByClass(MaxTemperature.class);
        job.setJobName("Max temperature");

        // 设置文件输入输出路径
        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        // 指定要用的map类型和reduce类型
        job.setMapperClass(MaxTemperatureMapper.class);
        job.setReducerClass(MaxTemperatureReducer.class);

        // 控制reduce函数的输出类型，并且必须和reduce类产生的相匹配
        // map函数的输出类型默认情况下和reduce函数是相同的
        // 如果不同需调用 setMapOutputKeyClass和setMapOutputValueClass
        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

命令行执行：

- > export HADOOP_CLASSPATH=hadoop-example.jar

- > hadoop MaxTemperature input/ncdc/1901 output

### **2.4 横向扩展**

为实现横向扩展，我们需要把数据存储在分布式文件系统中（HDFS），通过使用Hadoop资源管理系统YARN，Hadoop可以将MapReduce计算转移到存储有部分数据的各台机器上

#### 数据流

分片：MapReduce的输入数据划分成等长的小数据块

最佳分片应该和块大小相同：因为它是确保可以存储在单个节点上的最大输入块的大小

**map任务将其输出写入本地磁盘而非HDFS**：因为map的输出是中间结果，一旦作业完成该结果可删除

图中，a为本地数据map任务，b为本地机架map任务，c为跨机架map任务

![map任务](/image/maptask.png)

reduce任务不具备数据本地化的优势，单个reduce任务的输入通常来自所有mapper的输出，**reduce的输出通常存储在HDFS中以实现可靠存储**

图为一个reduce任务的MapReduce数据流，虚线框表示节点，reduce任务数量是独立指定的

![reduce任务](/image/reduce.png)

如果有好多个reduce任务，每个map任务就会针对输出进行分区，每个reduce任务建立一个分区，每个分区有许多键，每个键对应的键值对都在同一个分区，分区可有用户定义的分区函数控制，通常使用hash函数来分区

多个reduce任务的数据流如下图，map任务和reduce任务之间的数据流称为shuffle（混洗），每个reduce任务的输入都来自许多map任务，**调整混洗参数对作业总执行时间影响非常大**

![reduce任务](/image/reduce2.png)

当数据处理可以完全并行（无需混洗）时，可能会出现无reduce任务的情况，这种情况下，唯一的非本地节点数据传输是map任务将结果写入HDFS

![reduce任务](/image/reduce3.png)

#### combiner函数

combiner函数不能取代reduce函数，我们仍需要reduce函数来处理不同map输出中具有相同键的记录，但combiner函数能帮助减少mapper和reducer之间的数据传输量

combiner是通过reducer类来定义的，在这个例子中，它的实现于MaxTemperatureReducer中的reduce函数相同，唯一改动的是job中设置combiner类

```java
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class MaxTemperatureWithCombiner {
    public static void main(String[] args) throws Exception {
        if (args.length != 2) {
            System.err.println("args ERROR");
            System.exit(-1);
        }

        Job job = new Job();
        job.setJarByClass(MaxTemperatureWithCombiner.class);
        job.setJobName("Max temperature");

        FileInputFormat.addInputPath(job, new Path(args[0]));
        FileOutputFormat.setOutputPath(job, new Path(args[1]));

        job.setMapperClass(MaxTemperatureMapper.class);
        job.setCombinerClass(MaxTemperatureReducer.class);

        job.setReducerClass(MaxTemperatureReducer.class);

        job.setOutputKeyClass(Text.class);
        job.setOutputValueClass(IntWritable.class);

        System.exit(job.waitForCompletion(true) ? 0 : 1);
    }
}
```

### **2.5 Hadoop Streaming**

Hadoop为MapReduce提供了不同的API，可以方便我们使用不同的编程语言来使用MapReduce框架，而不是只局限于Java

map的输入数据通过标准输入流传递给map函数，并且是一行一行地传输，最后将结果行写到标准输出，map输出的键值对是以一个制表符分隔的行，reduce函数的输入格式与之相同，结果写入标准输出

Python版本

map

```python
import re
import sys

for line in sys.stdin:
    val = line.strip()
    (year, temp, q) = (val[15:19], val[87:92], val[92:93])
    if (temp != "+9999" and re.match("[01459]", q)):
        print "%s\t%s" % (year, temp)
```

reduce

```python
import sys

(last_key, max_val) = (None, -sys.maxint)
for line in sys.stdin:
  (key, val) = line.strip().split("\t")
  if last_key and last_key != key:
    print "%s\t%s" % (last_key, max_val)
    (last_key, max_val) = (key, int(val))
  else:
    (last_key, max_val) = (key, max(max_val, int(val)))

if last_key:
  print "%s\t%s" % (last_key, max_val)
```

执行命令

cat input/ncdc/sample.txt | ch02-mr-intro/src/main/python/max_temperature_map.py | ch02-mr-intro/src/main/python/max_temperature_reduce.py 

## 第三章 Hadoop分布式文件系统

`管理网络中跨多台计算机存储的文件系统称为分布式文件系统`

### **3.1 HDFS设计**

- 超大文件
- 流式数据访问
  - 读取整个数据集的时间延迟比读取第一条记录的时间延迟更重要
- 商用硬件
- 低时间延迟的数据访问
  - **HDFS是为高数据通途量应用优化的，可能会以提高时间延迟为代价，低延迟的数据访问HBase更适合**
- 大量的小文件
- 多用户写入，任意修改文件
  - 只支持单个写入者，以添加方式在文件末尾写数据

####　数据块

HDFS块默认大小128MB，与磁盘传输速率有关

块抽象好处：

- 一个文件的大小可以大于网络中任意一个磁盘的容量
- 以抽象块为存储单元而非整个文件，大大简化了存储子系统的设计 

#### namenode 和 datanode

- namenode：管理节点，管理文件系统的命名空间，维护着文件系统树及整棵树内所有的文件和目录，这些信息以来个那个文件形式永久保存在本地磁盘上 **命名空间镜像文件（namespace image）** **编辑日志文件（edit log）**
- datanode：工作节点，根据需要存储并检索数据块，定期向namenode发送它们所存储的块的列表

namenode两种容错机制：

1. 备份哪些组成文件系统元数据持久状态的文件，一般配置是将持久状态写入本地磁盘的同时，写入一个远程挂载的网络文件系统
2. 运行一个辅助namenode，但它不能被用作namenode，定期合并编辑日志与命名空间镜像

#### 块缓存

通常datanode从磁盘中读取块，但对于访问频繁的文件，其对应的块可能被显式地缓存在datanode的内存中

#### 联邦HDFS

允许系统通过添加namenode实现扩展，其中每个namenode管理文件系统命名空间中的一部分

集群中的datanode需要注册到每个namenode，并且存储着来自多个数据块池中的数据块

#### HDFS的高可用性（High Availability）

namenode失效，新的namenode直到满足以下情形才能响应服务：

- 将命名空间的映射导入内存中
- 重演编辑日志
- 接受到足够多的来自datanode的数据块报告并退出安全模式

**活动-备用namenode**：当活动namenode失效，备用namenode就会接管它的任务并开始服务于来自客户端的请求，实现这一目标需要在架构上修改：

- namenode之间通过高可用共享存储实现编辑日志的共享
- datanode需要同时向两个namenode发送数据块处理报告
- 客户端需要使用特定的机制来处理namenode的失效问题
- 辅助namenode的角色被备用namenode所包含，备用的namenode为活动的namenode命名空间设置周期性检查点

![Namenode](image/activeNamenode.png)

**Namenode高可用架构** [参考文章](https://www.ibm.com/developerworks/cn/opensource/os-cn-hadoop-name-node/index.html
)

- Active NameNode和Standby NameNode：两台NameNode形成互备，前者为主NameNode，后者为备NameNode，只有主NameNode才能对外提供读写服务
- 主备切换控制器ZKFailoverController：对NameNode的主备切换进行总体控制，监控NameNode的健康状况，在主NameNode故障时借助ZooKeeper实现自动的主备选举和切换
- Zookeeper集群：为主备切换控制器提供主备选举支持
- 共享存储系统：保存了NameNode在运行过程中产生的HDFS元数据。主备NameNode通过该系统实现元数据同步
- DataNode节点：同时向主备NameNode上报数据块的位置信息

##### *NameNode主备切换实现*

由ZKFailoverController，HealthMonitor和ActiveStandbyElector这三个组件协同完成

ZKFailoverController（zkfc）启动的时候会创建HealthMonitor和ActiveStandbyElector这两个主要的内部组件，创建的同时也会向两者注册相应的回调方法

HealthMonitor主要负责检测NameNode的健康状态，发现NameNode状态变化会回调ZKFailoverController的方法进行自动的主备选举

ActiveStandbyElector主要负责完成自动的主备选举，内部封装了Zookeeper的处理逻辑，一点Zookeeper主备选举完成，会回调ZKFailoverController的方法进行NameNode的主备状态转换

NameNode 实现主备切换的流程如图，有以下几步：

![NameNodeswitch](image/NameNodeswitch.png)

1. HealthMonitor初始化完成之后会启动内部的线程来定时调用对应 NameNode的HAServiceProtocol RPC接口的方法，对NameNode的健康状态进行检测。
2. HealthMonitor如果检测到NameNode的健康状态发生变化，会回调ZKFailoverController注册的相应方法进行处理。
3. 如果ZKFailoverController判断需要进行主备切换，会首先使用ActiveStandbyElector来进行自动的主备选举。
4. ActiveStandbyElector与Zookeeper进行交互完成自动的主备选举。
5. ActiveStandbyElector在主备选举完成后，会回调ZKFailoverController的相应方法来通知当前的NameNode成为主 NameNode或备NameNode。
6. ZKFailoverController调用对应NameNode的HAServiceProtocol RPC接口的方法将NameNode转换为Active状态或Standby状态。

##### *NameNode的共享存储实现*

###### NameNode的元数据存储概述

NameNode在执行HDFS客户端提交的创建文件或移动文件这样的写操作的时候，会首先把这些操作记录在EditLog中，然后更新内存中的文件系统镜像。

- 内存中的文件系统镜像用于NameNode向客户端提供读服务
- EditLog仅仅是数据恢复时起作用

EditLog中的每一个操作称为事务，EditLog被切割为很多段每一段称为Segment

NameNode会定期对内存中的文件系统镜像进行checkpoint操作，在磁盘上生成FSImage文件

NameNode启动时会进行数据恢复，首先把FSImage文件加载到内存中形成文件系统镜像，然后再把EditLog之中FSImage的结束事务id之后的EditLog回放到这个文件系统中

###### 基于QJM的共享系统的总体架构

![qjm](image/qjm.png)

**NameNode:**
- FSEditLog：这个类封装了对 EditLog 的所有操作，是 NameNode 对 EditLog 的所有操作的入口。
- JournalSet：这个类封装了对本地磁盘和 JournalNode 集群上的 EditLog 的操作，内部包含了两类 JournalManager
  - 一类为 FileJournalManager，用于实现对本地磁盘上 EditLog 的操作。
    - FileJornalmanager 封装了对本地磁盘上的 EditLog 文件的操作，不仅 NameNode 在向本地磁盘上写入 EditLog 的时候使用 FileJournalManager，JournalNode 在向本地磁盘写入 EditLog 的时候也复用了 FileJournalManager 的代码和逻辑。
  - 一类为 QuorumJournalManager，用于实现对 JournalNode 集群上共享目录的EditLog的操作。
    - 封装了对 JournalNode 集群上的 EditLog 的操作，它会根据 JournalNode 集群的 URI 创建负责与 JournalNode 集群通信的类 AsyncLoggerSet， QuorumJournalManager 通过 AsyncLoggerSet 来实现对 JournalNode 集群上的 EditLog 的写操作，对于读操作，QuorumJournalManager 则是通过 Http 接口从 JournalNode 上的 JournalNodeHttpServer 读取 EditLog 的数据。
  - FSEditLog 只会调用 JournalSet 的相关方法，而不会直接使用 FileJournalManager 和 QuorumJournalManager。

- AsyncLoggerSet：内部包含了与 JournalNode 集群进行通信的 AsyncLogger 列表，每一个 AsyncLogger 对应于一个 JournalNode 节点，另外 AsyncLoggerSet 也包含了用于等待大多数 JournalNode 返回结果的工具类方法给 QuorumJournalManager 使用。
  - AsyncLogger：具体的实现类是 IPCLoggerChannel，IPCLoggerChannel 在执行方法调用的时候，会把调用提交到一个单线程的线程池之中，由线程池线程来负责向对应的 JournalNode 的 JournalNodeRpcServer 发送 RPC 请求。

**JournalNode:**

- JournalNodeRpcServer：运行在 JournalNode 节点进程中的 RPC 服务，接收 NameNode 端的 AsyncLogger 的 RPC 请求。
- JournalNodeHttpServer：运行在 JournalNode 节点进程中的 Http 服务，用于接收处于 Standby 状态的 NameNode 和其它 JournalNode 的同步 EditLog 文件流的请求。

### **3.3 命令行接口**