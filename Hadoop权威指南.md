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

### 使用Hadoop来分析数据

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

### 横向扩展

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