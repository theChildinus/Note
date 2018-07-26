# Hadoop权威指南

## 第一章 初始Hadoop

### Hadoop 三种运行模式

- 独立（本地）模式：无需运行任何守护进程，所有程序都在同一个JVM上执行
- 伪分布模式：Hadoop守护进程运行在本地机器上，模拟一个小规模的集群
- 全分布模式：Hadoop守护进程运行在一个集群上

### 数据的存储与分析

对多个硬盘中的数据并行进行读／写，有很多问题需要解决

- 硬件故障问题 RAID，replica，HDFS
- 大多数分析任务需要以某种方式结合大部分数据来共同完成分析，MapReduce

Hadoop为我们提供了一个可靠的且可拓展的存储和分析平台

### 查询所有数据

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
        // 传递一个类，Hadoop利用这个类来查找包含它的jar文件，进而找到相关的jar文件
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

#### 数据块

HDFS块默认大小128MB，与磁盘传输速率有关

块抽象好处：

- 一个文件的大小可以大于网络中任意一个磁盘的容量
- 以抽象块为存储单元而非整个文件，大大简化了存储子系统的设计 

#### namenode 和 datanode

- namenode：管理节点，管理文件系统的命名空间，维护着文件系统树及整棵树内所有的文件和目录，这些信息以两个文件形式永久保存在本地磁盘上 **命名空间镜像文件（namespace image）** **编辑日志文件（edit log）**
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

启动HDFS：`start-dfs.sh`

`hadoop fs -ls [path]`

没有指定[path]的时候，在hdfs中hadoop扩展目录到 /home/[username]，其中[username]被执行命令的linux username所代替例如

`ubuntu@lenovo:~$ hadoop fs -ls`

hadoop查询的路径为：/home/ubuntu

`ubuntu@lenovo:~$ hadoop fs -ls hdfs://localhost:9000`

hadoop查询的路径为指定的[path]，其是hdfs的根目录类似于：`ubuntu@lenovo:~$ hadoop fs -ls /`

### **3.4 Hadoop文件系统**

Hadoop是由Java写的，通过Java API可以调用大部分Hadoop文件系统的交互操作

WebHDFS协议童工HTTP REST API使得其他语言开发的应用能够方便的于HDFS交互

通过HTTP访问HDFS有两种方式

- 直接访问：HDFS守护进程接收客户端的HTTP请求
  - namenode和datanode内嵌的web服务器作为WebHDFS的端节点运行
  - 元数据由namenode管理，文件读写先发往namenode，由namenode发送一个HTTP重定向到某个客户端，指定以流方式传输文件数据的目的或源datanode
- 代理访问：客户端通过DistributedFileSystem API访问HDFS
  - 所有到集群的网络通信都需要经过代理，代理服务器可以部署更加严格的防火墙策略和带宽限制策略

![webHDFS](image/WebHDFS.png)

### **3.5 Java接口**

#### 通过URLStreamHandler访问HDFS文件

```java
import org.apache.hadoop.fs.FsUrlStreamHandlerFactory;
import org.apache.hadoop.io.IOUtils;

import java.io.InputStream;
import java.net.URL;

public class URLCat {
    static {
        URL.setURLStreamHandlerFactory(new FsUrlStreamHandlerFactory()); //每个java虚拟机只能调用一次这个方法
    }

    public static void main(String[] args) throws Exception {
        InputStream in = null;
        try {
            in = new URL(args[0]).openStream();
            IOUtils.copyBytes(in, System.out, 4096, false);
        } finally {
            IOUtils.closeStream(in);
        }
    }
}
```

#### 通过FileSystem访问HDFS文件

```java
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.fs.FileSystem;
import sun.nio.ch.IOUtil;

import java.io.InputStream;
import java.net.URI;

public class FileSystemCat {
    public static void main(String[] args) throws Exception {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);
        InputStream in = null;
        try {
            in = fs.open(new Path(uri));
            IOUtils.copyBytes(in, System.out, 4096, false);
        } finally {
            IOUtils.closeStream(in);
        }

    }
}
```

#### 写入数据

将本地文件复制到Hadoop文件系统，调用create()方法写入文件时会自动创建父目录

```java
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.util.Progressable;
import org.apache.hadoop.fs.Path;

public class FileCopyWithProgress {
    public static void main(String[] args) throws Exception {
        String localSrc = args[0];
        String dst = args[1];
        InputStream in = new BufferedInputStream(new FileInputStream(localSrc));

        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(dst), conf);
        OutputStream out = fs.create(new Path(dst), new Progressable() {
            @Override
            public void progress() {
                System.out.println(".");
            }
        });

        IOUtils.copyBytes(in, out, 4096, true);
    }

}
```

#### 查询文件系统

1. 文件元数据 FileStatus
    - FileStatus封装了文件系统中文件和目录的元数据，包括文件长度、块大小、复本、修改时间、所有者以及权限信息

2. 列出文件 listStatus

3. 文件模式 globStatus
    - 返回路径格式与指定模式匹配的所有FileStatus对象组成的数组，并按路径排序
4. PathFilter对象
    - 用于排除匹配正则表达式的路径

### **3.6 数据流**

#### 剖析文件读取

1. 客户端调用FileSystem对象的open()方法打开希望读取的文件，对于HDFS来说，这个对象是 DistributedFileSystem 的一个实例
2. DistributedFileSystem 通过远程过程调用(RPC)来调用namenode，确定文件起始块位置
    - 对于每个块，namenode返回存有该块副本的datanode地址，这些datanode根据它们与客户端的距离来排序，如果该客户端本身就是一个datanode，那么该客户端将会从保存有相应数据的块副本的本地datanode中读取数据
3. DistributedFileSystem 类返回一个FSDataInputStream对象给客户端以便读取数据，FSDataInputStream 类转而封装 DFSInputStream 对象，该对象管理着 datanode 和 namenode 的I/O，接着客户端对这个输入流调用read()方法
4. 存储文件起始几个块的datanode地址的 DFSInputStream 随即连接距离最近的文件中第一个块所在的 datanode 通过反复调用 read() 方法，可以将数据从 datanode 传输到客户端
5. 到达块的末端时，DFSInputStream 关闭与该 datanode的连接，然后寻找下一块最佳 datanode
6. 客户端也会根据需要询问namenode来检索下一批数据块的 datanode 的位置，客户端读取完毕，就对FSDataInputStream调用close()方法

![客户端读取HDFS中的数据](image/readdata.png)

这个设计的重点是：客户端可以直接连接到datanode检索数据，由于数据分散在集群中的所有datanode中，所以这种设计能使HDFS 扩展到大量的并发客户端，同时namenode只要响应块位置的请求，无需响应数据请求，否则随着客户端的请求，namenode会成为瓶颈

#### 剖析文件写入

1. 客户端通过DistributedFileSystem 对象调用 create() 来新建文件
2. DistributedFileSystem 对象调用 create() 创建一个RPC调用，在文件系统的命名空间中新建一个文件，此时此刻该文件还没有响应的数据块
3. namenode 执行各种不同的检查以确保这个文件不存在以及客户端有新建该文件的权限，检查通过，namenode会为创建新文件记录一条记录，否则抛出IOException异常
    - DistributedFileSystem 向客户端返回一个 FSDataOutputStream 对象，由此可以开始写入数据，FSDataOutputStream 封装一个 DFSOutpurStream 对象，该对象负责处理 datanode 和 namenode 之间的通信
4. DFSOutputStream 将数据分成一个个数据包，写入数据队列(data
 queue)，DataStreamer处理数据队列，它的责任是挑选出适合存储复本的一组datanode，并据此来要求namenode分配新的数据块
5. DFSOutputStream 也维护着一个内部数据包队列来等待datanode的收到确认回执，称为确认队列(ack queue)，收到管道中所有datanode确认信息后，该数据包才会从确认队列删除
6. 客户端完成数据的写入后，对数据流调用 close() 方法
7. 该操作将剩余的所有数据包写入datanode管线，并在联系到namenode告知其文件写入完成之前，等待确认

![客户端将数据写入HDFS](image/writedata.png)

namenode 已经知道文件是由那些块组组成(因为 DataStreamer 请求分配数据块) 所以它在返回成功前只需要等待数据块进行最小量的复制

#### 一致模型

hflush() 方法不保证 datanode 已经将数据写到磁盘上，仅保证数据据在 datanode 的内存上

hsync() 方法将数据写入到磁盘上

### **3.7 通过distcp并行复制**

代替 `hadoop fs -cp`

`% hadoop distcp file1 file2`，也可以复制目录：`hadoop distcp dir1 dir2`

`hadoop distcp -update dir1 dir2` 修改了 dir1 子树中的一个文件，将修改同步到dir2

:floppy_disk:

## 第四章 关于YARN

Yet Another Resource Negotiator 是 Hadoop 的集群资源管理系统，最初是为了改善MapReduce的实现，但它具有足够的通用性，同样可以支持其他的分布式计算模式

![yarn](image/yarn.png)

### **4.1 剖析YARN应用运行机制**

YARN提供核心服务的守护进程有：

- 管理集群上资源使用的资源管理器
- 运行在集群中所有节点上且能够启动和监控容器的节点管理器

![yarnapp](image/yarnapp.png)

1. 客户端联系资源管理器，要求它运行一个 application master 进程
2. 资源管理器找到一个能够在容器中启动 application master 的节点管理器
3. application mater 一旦启动起来能够做些什么依赖于应用本身，有可能是在所处的容器中简单运行一个计算，并将结果返回给客户端，或是向资源管理器请求更多的容器
4. 以用于运行一个分布式计算

YARN 本身不会为应用的各部分彼此间通信提供任何手段，大多数重要的YARN应用使用某种形式的远程通信机制 (Hadoop的RPC层) 来向客户端传递状态更新和返回结果，但是这些通信机制都是专属于各应用的

#### 资源请求

YARN 有一个灵活的资源请求模型，当请求多个容器时，可以指定每个容器需要的计算机资源数量，还可以指定对容器的本地限制要求

Spark采用在集群上启动固定数量的执行器，MapReduce则分两步走，在最开始时申请map任务容器，reduce任务容器的启动放在后期

#### 应用生命期

按照应用到用户运行的作业之间的映射关系对应用进行分类

最简单的模型：一个用户作业对应一个应用，这是MapReduce采取的方式

第二种模型：作业的每个工作流或每个用户对话对应一个应用，这种方法要比第一种情况效率高，因为容器可以在作业之间重用，并且可能缓存作业之间的中间结果，Spark采取的是这种模型

第三种模型：多个用户共享一个长期运行的应用，这个应用通常作为一种协调者的角色在运行

### **4.2 YARN与MapReduce 1 相比**

MapReduce 1 和 YARN在组成上的比较，YARN的很多设计是为了解决MapReduce 1的局限性

MapReduce 1 | YARN
--- | ---
Jobtracker | 资源管理器、application master、时间轴服务器
Tasktracker | 节点管理器
Slot | 容器

使用YARN的好处包括

- 可扩展性
- 可用性
- 利用率
  - MapReduce 1中，每个tasktracker都配置有若干固定长度的slot，这些slot是静态分配的，在配置的时候被划分为 map slot 和 reduce slot
  - YARN 中，一个节点管理器管理一个资源池，而不是指定数目的slot，YARN 上运行的MapReduce任务不会出现由于集群中仅有map slot可用导致reduce任务必须等待的情况
- 多租户
  - 最大的优点是向MapReduce以外其他类型的分布式应用开放了Hadoop

### **4.3 YARN中的调度**

YARN有三种调度器可用：FIFO调度器、容量调度器、公平调度器

FIFO调度器

- 优点是简单易懂，不行也要任何配置，但是不适合共享集群，大的应用会占用集群中的所有资源，所以每个应用必须等待直到轮到自己运行

容量调度器

- 一个独立的专门队列保证小作业一提交就可以启动，大作业执行时间较长

公平调度器

- 调度器动态平衡资源，第一个大作业启动（唯一运行的作业）获取集群中所有的资源，第二个作业启动时，它被分配到集群的一半资源，这样每个作业都能共享资源

![scheduler](image/scheduler.png)

用户队列间的公平共享

![fairshare](image/fairshare.png)

## 第五章 Hadoop的I/O操作

### **5.1 数据完整性**

HDFS 会对写入的所有数据计算校验和，并在读取数据时验证校验和

- datanode 负责在收到数据后存储该数据及其校验和之前 对数据进行验证，管线中最后一个datanote负责验证校验和
- 客户端从datanode 读取数据时也会验证校验和，将它们与datanode中存储的校验和进行比较
- 每个datanode也会在一个后台线程中运行一个 DataBlockScanner，从而定期验证存储在这个 datanode 上的所有数据块

#### LocalFileSystem

Hadoop 的LocalFileSystem 执行客户端的校验和验证，我们也可以禁用校验和计算，特别是在底层文件系统本身就支持检验和的时候，在这种情况下，使用RawLocalFileSystem 替代 LocalFileSystem

#### ChecksumFileSystem

LocalFileSystem 通过 ChecksumFileSystem 来完成自己的任务，ChecksumFileSystem 类集继承自FileSystem 类，一般用法是：

```java
FileSystem rawFs = ...
FileSystem checksummedFs = new ChecksumFileSystem(rawFs);
```

### **5.2 压缩**

文件压缩的两个好处：

- 减少存储文件所需要的空间
- 加速数据在网络和磁盘上的传输

#### codec

codec是压缩-解压缩算法的一种实现，在Hadoop中，一个对CompressionCodec接口的事件代表一个codec

##### 1. 通过CommpressionCodec对数据流进行压缩和解压缩

- 对写入输出数据流的数据进行压缩，可用`createOutputStream(OutputStream out)`方法在底层的数据流中对需要以压缩格式写在此之前尚未压缩的数据新建一个CommpressionOutputStream对象
- 对输入数据流中读取的数据解压时，调用`createInputStream(Input in)`获取CompressionInputStream可以通过该方法从底层数据流读取解压后的数据

该程序压缩从标准输入读取的数据，然后将其写到标准输出

```java
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.CompressionOutputStream;
import org.apache.hadoop.util.ReflectionUtils;

public class StreamCompressor {
    public static void main(String[] args) throws Exception {
        // 符合CompressionCodec 实现的完全合格名称作为第一个命令行参数
        String codecClassname = args[0];
        Class<?> codecClass = Class.forName(codecClassname);
        Configuration conf = new Configuration();
        // 使用 ReflectionUtils创建一个codec实例
        CompressionCodec codec = (CompressionCodec)ReflectionUtils.newInstance(codecClass, conf);
        // 并由此获得在 System.out 上支持压缩的一个包装方法
        CompressionOutputStream out = codec.createOutputStream(System.out);
        // 将输入数据复制到输出
        IOUtils.copyBytes(System.in, out, 4096, false);
        // 要求压缩方法将写操作完成到压缩数据流上，但不关闭这个数据流
        out.finish();
    }
}
```

执行命令：

- `export HADOOP_CLASSPATH=StreamCompressor.jar`
- `echo "hello" | hadoop StreamCompressor org.apache.hadoop.io.compress.GzipCodec | gunzip -`

##### 2. 通过CompressionCodecFactory推断 CompressionCodec

根据文件拓展名选取 codec 解压文件

```java
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.compress.CompressionCodec;
import org.apache.hadoop.io.compress.CompressionCodecFactory;

import java.io.InputStream;
import java.io.OutputStream;
import java.net.URI;


public class FileDecompressor {

    public void main(String[] args) throws Exception {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);

        Path inputPath = new Path(uri);
        CompressionCodecFactory factory = new CompressionCodecFactory(conf);
        CompressionCodec codec = factory.getCodec(inputPath);
        if (codec == null) {
            System.err.println("no codec found for " + uri);
            System.exit(1);
        }
        // 除去文件拓展名，形成输出文件名
        String outputUri = CompressionCodecFactory.removeSuffix(uri, codec.getDefaultExtension());

        InputStream in = null;
        OutputStream out = null;
        try {
            in = codec.createInputStream(fs.open(inputPath));
            out = fs.create(new Path(outputUri));
            IOUtils.copyBytes(in, out, conf);
        } finally {
            IOUtils.closeStream(in);
            IOUtils.closeStream(out);
        }
    }
}
```

##### 3. 原生类库

为了提高性能，最好使用native类库实现压缩和解压缩

##### 4. CodecPool

如果使用的是原生代码库并且需要在应用中执行大量的压缩和解压缩操作，可以考虑使用CodecPool

#### 压缩和输入分片

文件经过gzip压缩，压缩后的文件大小为1GB，与以前一样，HDFS将这个文件保存为8个数据块，但是将每个数据块单独作为一个输入分片是无法实现工作的，因为无法实现从gzip压缩数据流的任意位置读取数据，所以让map任务独立于其他任务进行数据读取是不可行的

gzip不支持文件切分

bzip2 文件提供不同数据块之间的同步标识，因而它支持切分

对大文件按来说，不要使用不支持切分整个文件的压缩格式，因为会失去数据的本地特性，进而造成MapReduce应用效率地下

##### 对map任务输出进行压缩

尽管MapReduce应用读写的是未经压缩的数据，但是map阶段的中间输入进行压缩可以获得不少好处，例如减少传输的数据量

### **5.3 序列化**

>序列化 是指将结构化对象转化为字节流以便在网络上传输或写到硬盘中进行永久存储的过程，反序列化是指将字节流转回结构化对象的逆过程

序列化的是对象的状态

序列化用于分布式数据处理的两大领域：

- 进程间通信
- 永久存储

Hadoop中，系统中多个节点上进程间通信是通过“远程过程调用”(RPC)实现的，RPC协议将消息序列化成二进制流后发送到远程节点，远程节点接着将二进制流反序列化为原始消息，RPC序列化格式如下：

- 紧凑
  - 紧凑格式能充分利用网络带宽
- 快速
  - 减少序列化和反序列化的开销
- 可扩展
  - 为了满足新需求，协议不断变化，可读取老格式的数据
- 支持互操作
  - 支持不同语言写的客户端和服务器的交互

#### Writable接口

Writable 接口定义了两种方法

- `write(DataOutput out)` 将其状态写入DataOutput二进制流
- `readFileds(DataInput in)` 从DataInput二进制流读取状态

#### Writable类

##### 1. Java基本类型的Writable封装器

- 定长格式编码很是和数值在整个值域空间中分布非常均匀的情况，例如哈希函数
- 大部分数值变量的分布都不均匀，使用变长格式会更节省空间

##### 2. Text类型

针对UTF-8序列的Writable类，可以理解为等价于Writable类的`java.lang.String`

对Text类的索引是根据编码后字节序列中的位置实现的，并非字符串中的Unicode字符，也不是 java char 的编码单元，Text的charAt返回一个该编码位置的int类型值，String返回该位置的char类型值，charAt()方法的用法
如下：

```java
Text t = new Text("Hadoop");
assertThat(t.getLength(), is (6));
assertThat(t.getBytes().length, is(6));

assertThat(t.charAt(2), is((int)'d'));
assertThat("Out of bounds", t.charAt(100), is(-1));
```

Text还有一个find()方法，该方法类似于String的 indexOf()

```java
Text t = new Text("hadoop");
assertThat("Find a substring", t.find("do"), is(2));
assertThat("Finds first 'o'", t.find("o"), is(3));
assertThat("Finds 'o' from position 4 or later", t.find("o", 4), is(4));
assertThat("No match", t.find("pig"), is(-1));
```

- String 的长度是器所含char编码单元的个数，indexOf()方法返回char编码单元中的索引位置
- Text对象的长度却是其UTF-8编码的字节数，find()方法返回字节偏移量
- 与String另一个区别是Text是可变的，通过调用set()方法，Text通过toString()方法实现String对象

##### 3. BytesWritable

BytesWritable 是对二进制数据数组的封装，序列化格式为：一个指定所含数据字节数的整数域，后跟数据内容本身

长度为2的字节数组包含数值3和5，序列化形式为一个4字节的整数（00000002）和该数组中的两个字节（03和05）

```java
BytesWriable b = new BytesWritable(new byte[]{3, 5});
byte[] bytes = serialize(b);
assertThat(StringUtils.byteToHeString(bytes), is("000000020305"));
```

BytesWritable 是可变的

##### 4. NullWritable

NullWritable是Writable的特殊类型，它的序列化长度为0，它并不从数据流中读取数据，也不写入数据，在MapReduce中，如果不需要使用键或值的序列化地址，就可以将键或值声明为NullWritable，这样可高效存储常量空值

##### 5. ObjectWritable 和 GenericWritable

ObjectWritable是对Java基本类型（String，enum，Writable，null或这些类型组成的数组）的一个通用封装，他在Hadoop RPC中用于对方法的参数和返回类型进行封装和解封装

##### 6. Writable 集合类

org.apache.hadoop.io 中有6个Writable集合类，分别是 ArrayWritable，ArrayPrimitiveWritable，TwoDArrayWritable，MapWritable，SortedMapWritable，EnumMapWritable

- ArrayWritable 和 TwoDWritable 是对 Writable 的数组和二维数组的实现
- ArrayPrimitiveWritable 是对java基本数组类型的一个封装
- MapWritable，SortedMapWritable 分别实现了 java.util.Map<Writable, Writable> 和 java.util.SortedMap<WritableComparable, Writable>

#### 实现定制的Writable集合

有了定制的Writable类型，就可以完全控制二进制表示的排序顺序

```java
import java.io.*;
import org.apache.hadoop.io.*;
public class TextPair implements WritableComparable<TextPair> {
    private Text first;
    private Text second;

    public TextPair() {
        set(new Text(), new Text());
    }

    public TextPair(String first, String second) {
        set(new Text(first), new Text(second));
    }

    public TextPair(Text first, Text second) {
        set(first, second);
    }

    public void set(Text first, Text second) {
        this.first = first;
        this.second = second;
    }

    public Text getFirst() {
        return this.first;
    }

    public Text getSecond() {
        return this.second;
    }

    // 依次将每个Text对象序列化到输出流中
    @Override
    public void write(DataOutput out) throws IOException {
        first.write(out);
        second.write(out);
    }

    // 将来自输入流的字节进行反序列化
    @Override
    public void readFields(DataInput in) throws IOException {
        first.readFields(in);
        second.readFields(in);
    }

    @Override
    public int hashCode() {
        return first.hashCode() * 163 + second.hashCode();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof TextPair) {
            TextPair tp = (TextPair)o;
            return first.equals(tp.first) && second.equals(tp.second);
        }
        return false;
    }

    @Override
    public String toString() {
        return first + "\t" + second;
    }

    @Override
    public int compareTo(TextPair tp) {
        int cmp = first.compareTo(tp.first);
        if (cmp != 0) {
            return cmp;
        }
        return second.compareTo(tp.second);
    }
}
```

##### 1. 为提高速度实现一个RawComparator

当TextPair被用作MapReduce的键时，需要将数据流反序列化为对象，然后调用compareTo()方法进行比较

其实我们还可以看序列化表示比较两个TextPair对象

```java
 public static class Comparator extends WritableComparator {
        private static final Text.Comparator TEXT_COMPARATOR = new Text.Comparator();

        public Comparator() {
            super(TextPair.class);
        }

        @Override
        public int compare(byte[] b1, int s1, int l1,
                           byte[] b2, int s2, int l2) {
            try {
                // firstL1 和 firstL2 表示每个字节流中第一个Text字段的长度
                // WritableUtils.decodeVIntSize表示变长整数的长度，readVInt表示编码值
                int firstL1 = WritableUtils.decodeVIntSize(b1[s1]) + readVInt(b1, s1);
                int firstL2 = WritableUtils.decodeVIntSize(b2[s2]) + readVInt(b2, s2);

                int cmp = TEXT_COMPARATOR.compare(b1, s1, firstL1, b2, s2, firstL2);

                if (cmp != 0) {
                    return cmp;
                }
                return TEXT_COMPARATOR.compare(b1, s1 + firstL1, l1 - firstL1,
                                                b2, s2 + firstL2, l2 - firstL2);
            } catch (IOException e) {
                throw new IllegalArgumentException(e);
            }
        }
    }

    static {
        WritableComparator.define(TextPair.class, new Comparator());
    }
```

##### 2. 定制的comparator

定制的RawComparator用于比较TextPair对象字节表示的第一个字段

```java
    public static class FirstComparator extends WritableComparator {
        private static final Text.Comparator TEXT_COMPARATOR = new Text.Comparator();

        public FirstComparator() {
            super(TextPair.class);
        }

        @Override
        public int compare(byte[] b1, int s1, int l1,
                           byte[] b2, int s2, int l2) {
            try {
                int firstL1 = WritableUtils.decodeVIntSize(b1[s1]) + readVInt(b1, s1);
                int firstL2 = WritableUtils.decodeVIntSize(b2[s2]) + readVInt(b2, s2);

                return TEXT_COMPARATOR.compare(b1, s1, firstL1, b2, s2, firstL2);
            } catch (IOException e) {
                throw new IllegalArgumentException(e);
            }
        }

        @Override
        public int compare(WritableComparable a, WritableComparable b) {
            if (a instanceof TextPair && b instanceof TextPair) {
                return ((TextPair)a).first.compareTo(((TextPair)b).first);
            }
            return super.compare(a, b);
        }
    }
```

#### 序列化框架

Hadoop有一个针对可替换序列化框架的API，序列化框架用一个Serialization实现来表示，例如 WritableSerialization 类是对 Writable 类型的 Serialization 实现

Serialization 对象定义了从类型到 Serializer 实例（将对象转换成为字节流）和 Deserializer实例（将字节流转换为对象） 的映射方式

不建议使用 Java Object Serialization

##### 序列化IDL

不通过代码来定义类型，而是接口定义语言（IDL）以不依赖于具体语言的方式进行声明

比较流行的序列化框架

- Apache Thrift
- Google Protocal Buffers
- 常用作二进制数据的永久存储格式，MapReduce格式对该类的支持有限
- Avro是一个基于IDL的序列化框架，非常适合于Hadoop的大规模数据处理

### **5.4 基于文件的数据结构**

需要特殊的数据结构来存储自己的数据

#### 关于SequenceFile

纯文本不适合记录二进制类型的数据，SequenceFile类非常适合，为二进制键-值对提供了一个持久数据结构

SequenceFile也可以作为小文件的容器

##### 1. SequenceFile 的写操作

```java
import java.io.IOException;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Text;

public class SequenceFileWriteDemo {
    private static final String[] DATA = {
        "One, two, buckle my shoe",
        "Three, four, shut the door",
        "Five, six, pick up sticks",
        "Seven, eight, lay them straight",
        "Nine, ten, a big fat hen"
    };

    public static void main(String[] args) throws IOException {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);
        Path path = new Path(uri);
        // 存储在SequenceFile中的键和值并不一定需要是Writable类型，只要能被Serialization序列化和反序列化就可以
        IntWritable key = new IntWritable();
        Text value = new Text();
        SequenceFile.Writer writer = null;
        try {
            // 创建SequenceFile对象，返回SequenceFile.Writer实例
            writer = SequenceFile.createWriter(fs, conf, path, key.getClass(), value.getClass());

            for (int i = 0; i < 100; i++) {
                key.set(100 - i);
                value.set(DATA[i % DATA.length]);
                System.out.printf("[%s]\t%s\t%s\n", writer.getLength(), key, value);
                writer.append(key, value);
            }
        } finally {
            IOUtils.closeStream(writer);
        }
    }
}
```

- 运行 `start-dfs.sh` 启动HDFS
- `export HADOOP_CLASSPATH=SequenceFileWriteDemo.jar`
- `hadoop SequenceFileWriteDemo numbers.seq`

##### 2. SequenceFile 的读操作

该程序显示了如何读取包含Writable类型键值对的顺序文件

```java
import java.io.IOException;
import java.net.URI;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IOUtils;
import org.apache.hadoop.io.SequenceFile;
import org.apache.hadoop.io.Writable;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.util.ReflectionUtils;

public class SequenceFileReadDemo {
    public static void main(String[] args) throws IOException {
        String uri = args[0];
        Configuration conf = new Configuration();
        FileSystem fs = FileSystem.get(URI.create(uri), conf);
        Path path = new Path(uri);

        SequenceFile.Reader reader = null;
        try {
            reader = new SequenceFile.Reader(fs, path, conf);
            // getKeyClass getValueClass 发现SequenceFile中所使用的类型
            // ReflectionUtils对象生成常见键值实例
            Writable key = (Writable)ReflectionUtils.newInstance(reader.getKeyClass(), conf);
            Writable value = (Writable)ReflectionUtils.newInstance(reader.getValueClass(), conf);
            long position = reader.getPosition();
            while (reader.next(key, value)) {
                //  同步点
                String syncSeen = reader.syncSeen() ? "*" : "";
                System.out.printf("[%s%s]\t%s\t%s\n", position, syncSeen, key, value);
                position = reader.getPosition();
            }
        } finally {
            IOUtils.closeStream(reader);
        }
    }
}
```

同步点是指 数据读取迷路后能够再一次与记录边界同步的数据流中的某个位置，例如在数据流中搜索而跑到任意位置后可采取此动作

在顺序文件中搜索给定位置有两种方法。

- 第一种是调用seek()方法，该方法将读指针指向文件中指定的位置

```java
reader.seek(359);
assertThat(reader.next(key, value), is(true));
assertThat(((IntWritable)key).get(), is(95));
```

- 第二种方法通过同步点查找记录边界，SequenceFile.Reader 对象的sync(long position)方法 可以将读取位置定位到position之后的下一个同步点

可以将加入同步点的顺序文件作为MapReduce的输入，因为该类顺序文件允许切分，由此该文件的不同部分可以由独立的map任务单独处理

MapReduce是对多个顺序文件进行排序（或合并）最有效的方法，MapReduce本身是并行的，并且可由用户指定使用多少个reducer

##### SequenceFile 的格式

SequenceFile 由文件头和随后一条或多条记录组成

- SequenceFile的前三个字节为SEQ(顺序文件代码)
- 紧随其后的一个字节表示SequenceFile的版本号
- 文件头还包括其他字段，键值类的名称，数据压缩细节，用户定义的元数据以及同步标识

![sequencefile](image/sequencefile.png)

记录的内部结构取决于是否启用压缩，如果启用压缩，其结构取决于是记录压缩还是数据块压缩

- 记录压缩格式与无压缩情况基本相同，只不过值是用文件头中定义的codec压缩的，注意，键没有被压缩
- 块压缩是指一次性压缩多条记录，因为它可以利用记录间的相似性进行压缩

![blockcompression](image/blockcompression.png)

#### 关于MapFile

MapFile 是已经排过序的 SequenceFile，它有索引，所以可以按键查询，索引自身就是一个 SequenceFile 包含了map中的一小部分键，主数据文件是另一个 SequenceFile，包含了所有的 map 条目，这些条目都按照键顺序进行了排序

MapFile变种

- SetFile
- ArrayFile
- BloomMapFile

#### 其他文件格式和面向列的格式

Avro数据文件是面向大规模数据而设计的，又是可移植的，它们可以跨越不同的编程语言使用

SequenceFile，MapFile，Avro数据文件都是面向行的格式，面向列的存储布局可以使一个查询跳过那些不必访问的列

面向列的文件格式有：

- Hive的ORCFile
- 基于Google Dremel的Parquet
- Avro的Trevni

## 第六章 MapReduce 应用开发