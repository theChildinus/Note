<!-- TOC -->

- [知识点](#知识点)
    - [I/O 模式 (Linux network IO)](#io-模式-linux-network-io)
        - [阻塞I/O](#阻塞io)
        - [非阻塞I/O](#非阻塞io)
        - [I/O复用](#io复用)
        - [信号驱动I/O](#信号驱动io)
        - [异步I/O](#异步io)
        - [对比](#对比)
    - [浅拷贝和深拷贝](#浅拷贝和深拷贝)
    - [简历相关](#简历相关)
        - [项目1](#项目1)
            - [Serviceability Agent](#serviceability-agent)
        - [项目2](#项目2)
        - [项目3](#项目3)
    - [数据结构](#数据结构)
        - [红黑树](#红黑树)
        - [B/B+树](#bb树)
        - [epool libevent (异步I/O)](#epool-libevent-异步io)
        - [布隆过滤器](#布隆过滤器)
        - [HashMap 和 HashTable](#hashmap-和-hashtable)
        - [字典树 AC自动机 fail节点 KMP](#字典树-ac自动机-fail节点-kmp)
    - [数据库](#数据库)
        - [数据库锁机制](#数据库锁机制)
        - [存储引擎](#存储引擎)
            - [InnoDB](#innodb)
            - [MyISAM](#myisam)
        - [数据库索引](#数据库索引)
        - [索引实现](#索引实现)
        - [聚集索引和非聚集索引](#聚集索引和非聚集索引)
        - [切分](#切分)
            - [水平切分](#水平切分)
            - [垂直切分](#垂直切分)
    - [Linux](#linux)
    - [java基础](#java基础)
        - [垃圾回收机制](#垃圾回收机制)
        - [类加载机制](#类加载机制)
            - [加载](#加载)
            - [验证](#验证)
            - [准备](#准备)
            - [解析](#解析)
            - [初始化](#初始化)
        - [类和类加载器](#类和类加载器)
        - [对象的创建，对象的内存布局，对象的访问定位](#对象的创建对象的内存布局对象的访问定位)
        - [Java 线程中断](#java-线程中断)
        - [Java NIO](#java-nio)
        - [Spring](#spring)
        - [SIP协议](#sip协议)
        - [Web请求](#web请求)
        - [数字认证](#数字认证)

<!-- /TOC -->

# 知识点

本md记录一些平时看到的琐碎知识点

## I/O 模式 (Linux network IO)

两个阶段：

- 等待数据准备好
- 从内核向进程复制数据

对于一个套接字上的输入操作，第一步通常涉及等待数据从网络中到达。当所等待数据到达时，它被复制到内核中的某个缓冲区。第二步就是把数据从内核缓冲区复制到应用进程缓冲区。

[IO - 同步，异步，阻塞，非阻塞 （亡羊补牢篇）](https://blog.csdn.net/historyasamirror/article/details/5778378)

[也谈IO模型](http://www.rowkey.me/blog/2016/01/18/io-model/)

### 阻塞I/O

应用进程被阻塞，直到数据从内核缓冲区复制到应用进程缓冲区中才返回。

应该注意到，在阻塞的过程中，其它应用进程还可以执行，因此阻塞不意味着整个操作系统都被阻塞。因为其它应用进程还可以执行，所以不消耗 CPU 时间，这种模型的 CPU 利用率效率会比较高。

### 非阻塞I/O

应用进程执行系统调用之后，内核返回一个错误码。应用进程可以继续执行，但是需要不断的执行系统调用来获知 I/O 是否完成，这种方式称为轮询（polling）。

由于 CPU 要处理更多的系统调用，因此这种模型的 CPU 利用率比较低。

### I/O复用

使用 select 或者 poll 等待数据，并且可以等待多个套接字中的任何一个变为可读。这一过程会被阻塞，当某一个套接字可读时返回，之后再使用 recvfrom 把数据从内核复制到进程中。

它可以让单个进程具有处理多个 I/O 事件的能力。又被称为 Event Driven I/O，即事件驱动 I/O。

如果一个 Web 服务器没有 I/O 复用，那么每一个 Socket 连接都需要创建一个线程去处理。如果同时有几万个连接，那么就需要创建相同数量的线程。相比于多进程和多线程技术，I/O 复用不需要进程线程创建和切换的开销，系统开销更小。

### 信号驱动I/O

应用进程使用 sigaction 系统调用，内核立即返回，应用进程可以继续执行，也就是说等待数据阶段应用进程是非阻塞的。内核在数据到达时向应用进程发送 SIGIO 信号，应用进程收到之后在信号处理程序中调用 recvfrom 将数据从内核复制到应用进程中。

相比于非阻塞式 I/O 的轮询方式，信号驱动 I/O 的 CPU 利用率更高。

### 异步I/O

应用进程执行 aio_read 系统调用会立即返回，应用进程可以继续执行，不会被阻塞，内核会在所有操作完成之后向应用进程发送信号。

异步 I/O 与信号驱动 I/O 的区别在于，异步 I/O 的信号是通知应用进程 I/O 完成，而信号驱动 I/O 的信号是通知应用进程可以开始 I/O。

### 对比

- 同步IO和异步IO 的区别反映在数据拷贝阶段是由用户线程完成还是内核完成。所以说异步IO必须要有操作系统的底层支持。
- 阻塞IO和非阻塞IO 的区别反映在IO操作的第一个阶段，即查看数据是否就绪时，是如何处理的。

多路复用IO为何比非阻塞IO模型的效率高是因为在非阻塞IO中，不断地询问socket状态时通过用户线程去进行的，而在多路复用IO中，轮询每个socket状态是内核在进行的，这个效率要比用户线程要高的多。

在异步IO模型中，IO操作的两个阶段都不会阻塞用户线程，这两个阶段都是由内核自动完成，然后发送一个信号告知用户线程操作已完成。用户线程中不需要再次调用IO函数进行具体的读写。这点是和信号驱动模型有所不同的，在信号驱动模型中，当用户线程接收到信号表示数据已经就绪，然后需要用户线程调用IO函数进行实际的读写操作；而在异步IO模型中，收到信号表示IO操作已经完成，不需要再在用户线程中调用iO函数进行实际的读写操作。

两种高性能IO设计模式：Reactor和Proactor

- 在Reactor模式中，会先对每个client注册感兴趣的事件，然后有一个线程专门去轮询每个client是否有事件发生，当有事件发生时，便顺序处理每个事件，当所有事件处理完之后，便再转去继续轮询
- 在Proactor模式中，当检测到有事件发生时，会新起一个异步操作，然后交由内核线程去处理，当内核线程完成IO操作之后，发送一个通知告知操作已完成

**synchronous IO和asynchronous IO**:

- A synchronous I/O operation causes the requesting process to be blocked until that **I/O operation** completes
  - blocking IO
  - non-blocking IO
  - IO multiplexing
- An asynchronous I/O operation does not cause the requesting process to be blocked;

## 浅拷贝和深拷贝

[参考：Java深拷贝和浅拷贝](https://my.oschina.net/jackieyeah/blog/206391)

[使用序列化实现对象的拷贝](https://blog.csdn.net/chenssy/article/details/12952063)

- 浅拷贝是按位拷贝对象，它会创建一个新对象，这个对象有着原始对象属性值的一份精确拷贝。如果属性是基本类型，拷贝的就是基本类型的值；如果属性是内存地址（引用类型），拷贝的就是内存地址 ，因此如果其中一个对象改变了这个地址，就会影响到另一个对象。
- 深拷贝会拷贝所有的属性,并拷贝属性指向的动态分配的内存。当对象和它所引用的对象一起拷贝时即发生深拷贝。深拷贝相比于浅拷贝速度较慢并且花销较大。

理解深拷贝和浅拷贝的关键在于，Java语言在拷贝的时候只存在值拷贝，对于对象或者容器的拷贝也只是将对象或者容器的地址拷贝，在判断修改引用时是否会引起干扰，先要看对地址做了什么的操作，再看对地址的值做了什么操作

## 简历相关

遇到过的问题：

如何让 HotSpot 中已有的 Serviceability Agent 应用到 Volatility框架中

### 项目1

项目介绍：物联网服务运行时保障系统 是基于虚拟机入侵检测技术的运行时监控、恢复系统，实现了对 Linux虚拟机中正在运行的服务程序 进行运行时监控和处理反馈的功能，假设这样一个场景，当网络数据、串口数据不再安全可信时，我们还可以分析内存去判断数据的真实性，我在这个项目中主要负责对Linux虚拟机中 正在运行的服务程序 进行内存获取和分析，首先，由于服务程序是运行在Linux虚拟机中，在内存获取部分我使用了 Volatility 内存取证框架和LibVMI内存自省插件获取服务程序内存，其次，在内存分析部分 借助 Serviceability Agent 解析出Java进程及JVM虚拟机栈信息等，最后我还针对缓冲区溢出这种常见的攻击方式进行内存分析，实现了实时获取攻击前后函数调用关系变化，为系统检测模块后续分析提供依据

LibVMI库可以帮助我们通过 `System.map`（存放内核符号表，符号表是所有内核符号及其对应地址的一个列表）获取内核符号的虚拟地址，并找到对应的页表，通过页表找到数据页返回，即内存数据

`Volatility` 框架获取虚拟机中进程内存，需要两个信息，目标进程的逻辑地址 和 进程描述符的结构，前者标识了目标进程在内存中的逻辑位置，后者告诉我们进程运行的全部信息，这两个信息分别由 `System.map` 和 `module.dwarf` 提供，后一个文件是通过 `kernel-header` 生成的

这样我们就可以开始获取到虚拟机内所有进程的信息了

Java服务运行时 在Linux系统内统一的进程名为 java，Linux中进程是以链表的形式保存的，我们遍历链表，就可以找到进程名为 java 的进程了

对于任何一个Java程序 我们可以使用 jstack/jmap 等命令获取Java程序的堆栈信息，这些命令被封装在了 `Serviceability Agent` 中，通过ptrace实现，而且这个包中定义了Java内存模型使用的类，比如栈帧类，这就方便了我们去分析Java程序中的堆栈信息，**我们的目的就是用Volatility已经获取的信息去替换SA初始化过程中需要的信息**（这些信息包括共享库名称，共享库起始地址，所有的线程id），但是Volatility是用Python开发实现的，所以我们需要一个工具，让Python调用Java，即JPype，但是Jpype启动过程需要初始化JVM，这个JVM是在宿主机的，而我们要分析的JVM是在虚拟机的，所有我们需要用虚拟机中的JVM的信息去替换这个宿主机中JVM中初始化需要的信息，这些信息正好是Volatility可以获取到的，替换掉

分析过程：栈是从高地址向低地址生长，Java程序中每一个方法对应一个栈帧，首先需要拿到当前栈底栈帧的地址，在上一步中我们提供线程名，定位到线程对应的虚拟机栈，通过SA的方法可以获取当前虚拟机栈中栈顶栈帧的地址，由于当前栈帧的值就是上一帧的地址，通过遍历获取栈底栈帧的FP地址，这样我们就可以自底向上分析虚拟机栈了

在Volatility中，把当前虚拟机栈的内存全部读出来，从 `fp-48` 的位置就可以获取到方法栈帧中的参数，局部变量，局部对象引用等信息

#### Serviceability Agent

[Serviceability Agent](https://yq.aliyun.com/articles/20231)

在sa-jdi源码中 所有的工具类中都会启动一个 `BugSpotAgent`，在BugSpot的初始化过程中会调用 `setupDebugger` 和 `setupVM` 构造了目标VM的本机表示，通过源码分析，我们需要从这两个方法入手，替换 `setupDebugger` 初始化时的三个native方法，`attach0`，`lookupByName0` 和 `readBytesFromProcess0`  我们用Volatility获取的信息替换

- attach0：初始化VM需要，的共享库名，共享库地址，子线程ID
- lookupByName0：得到符号在内存中的地址，根据库名和符号名称在所有的共享库中遍历，找到符号偏移
- readBytesFromProcess0：得到从某地址开始指定长度的内存字节

`Serviceability Agent` 的实现原理是使用 Linux 的 /proc 和 ptrace，SA的基类是Tool，该方法会new BugSpotAgent，调用 `attach方法` 到指定PID的VM上，在attach方法中会 调用两个方法：

- `setupDebugger()`
- `setupVM()`

`setupDebugger` 中

- 1.设置虚拟机库文件
- 2.new一个LinuxDebuggerLocal
- 3.调用LinuxDebuggerLocal的attach方法
  - 在这里需要初始化 原LinuxDebuggerLocal中的 threadList 和 loadObjectList，我们用Volatility获取的信息替换

`setupVM` 中

- 1.构建HotSpotTypeDataBase
- 2.设置原生类型大小，从目标VM获取
- 3.构建目标VM的本机表示

### 项目2

项目介绍：网站页面分析这个项目主要目的是爬取网站页面内容，提取URL，构建页面与页面之间的连接网络，最后根据pageRank算法计算出权重前十的页面
在这个项目中我主要负责爬虫模块的开发，实现的主要流程是基于 libevent 网络库从待处理URL队列中取出URL建立网络连接，爬取页面内容，使用AC自动机提取页面包含的其他URL，经布隆过滤器去重后，再添加到待处理URL队列中等待处理

### 项目3

项目介绍：个人博客系统是我个人的一个学习项目，通过使用Spring MVC搭建一个简单的博客系统，实现了用户和博客文章的增删改查这些基本功能，学习了Spring MVC开发的基本流程，开发流程分为以下几步：

- 构建Spring基本开发环境，使用Maven导入需要用的jar包
- 在web.xml 中添加一个servlet，用于拦截请求，并交给Spring MVC后台控制器处理请求（Model-java Bean，view-页面展示，controller-处理页面请求），在spring.xml中 添加视图解释器，用于返回视图
- 添加用户请求的controller类，RequestMapping注解定义请求隐射 return "index" 接收到访问请求后返回首页
- 配置Tomcat，导出war包启动验证是否可以访问首页
- 然后配置数据库，使用IDEA工具自动生成关于数据表的Java Bean，在spring.xml 中添加jpa配置，定义JPA操作，新建关于用户博客的Repository 继承 JpaRepository 在这里定义操作数据库的方法
- 开始编写业务逻辑：
  - **增**：GET请求跳转到添加用户管理页面，管理页面上点击添加提交后会触发POST请求，传递过来一个Entity对象，里面包含信息，通过Repository.saveAndFlush写入数据库，重定向到管理页面返回
  - **删**：接收到GET请求，PathVariable收集url的变量，使用Repository删除指定id的用户，刷新数据库，重定向到管理页面返回
  - **改**：接受到GET请求，PathVariable收集url的变量，调用Repository的findOne方法，获取数据后返回到页面上展示，页面上修改提交后会触发POST请求，传递一个Entity对象，通过Repository.flush写入数据库，重定向到管理页面返回
  - **查个人**：接收到GET请求，PathVariable收集url的变量，调用Repository的findById方法访问到指定用户信息，modelMap.addAttribute添加到页面上返回
  - **查所有**：接受到GET请求，调用Repository的findAll方法，获取到数据后，通过modelMap.addAttribute添加到页面上返回

GET 用于获取资源，而 POST 用于传输实体主体。GET 的参数是以查询字符串出现在 URL 中，而 POST 的参数存储在实体主体

## 数据结构

### 红黑树

首先红黑树是不符合AVL树的平衡条件的，即每个节点的左子树和右子树的高度最多差1的二叉查找树。但是提出了为节点增加颜色，红黑是用非严格的平衡来换取增删节点时候旋转次数的降低，任何不平衡都会在三次旋转之内解决，而AVL是严格平衡树，因此在增加或者删除节点的时候，根据不同情况，旋转的次数比红黑树要多。所以红黑树的插入效率更高

红黑树5个约束

- 每个节点或是红色的，或是黑色的。
- 根节点是黑色的。
- 每个叶节点（NULL）是黑色的。
- 如果一个节点是红色的，则它的两个孩子节点都是黑色的。（不能有两个父子节点都为红）
- 对每个节点，从该节点到其所有后代叶节点的简单路径上，均包含相同数目的黑色节点。

插入的几种情况，插入节点必为红节点

- 父黑 直接插入
- 父红 叔红
  - 变父节点为黑，叔节点为黑，祖父节点为红，当前节点指向祖父节点
- 父红 叔黑 插入节点是父的右节点
  - 父节点为新的当前结点，新当前结点为支点左旋
- 父红 叔黑 插入节点是父的左节点
  - 父节点为黑 祖父节点为红 以祖父节点为支点右旋

删除的几种情况

[参考](https://github.com/julycoding/The-Art-Of-Programming-By-July/blob/master/ebook/zh/03.01.md)

只针对删除黑色节点才有修复，思路是：

- 把父亲节点另一边（即删除节点的兄弟树）其中一个节点弄成红色，也少一个黑色
- 或者把另一边多的黑色节点转过来一个

- 当前结点是红+黑色，直接把当前结点染成黑色，结束此时红黑树性质全部恢复。
- 当前结点是黑+黑且是根结点， 解法：什么都不做，结束。

**情况1**：兄弟节点为红，那么父节点和兄弟节点的子节点为黑

- 父变红，兄变黑，左旋转父节点，重新进入算法

**情况2**：兄弟节点为黑，兄弟节点的两个子节点也为黑

- 兄弟节点变为红色，父节点到下面的所有路径都少了一个黑节点，父节点作为当前节点，重新进入算法

**情况3**：兄弟节点为黑，兄弟的左为红，右为黑

- 把兄弟结点染红，兄弟左子结点染黑，之后再在兄弟结点为支点解右旋，之后重新进入算法，此是把当前的情况转化为情况4

**情况4**：兄弟节点为黑，兄弟的右为红，左为任意

- 把兄弟结点染成当前结点父结点的颜色，把当前结点父结点染成黑色，兄弟结点右子染成黑色，之后以当前结点的父结点为支点进行左旋，此时算法结束

### B/B+树

B Tree 指的是 Balance Tree，也就是平衡树。平衡树是一颗查找树，并且所有叶子节点位于同一层。

B+ Tree 是基于 B Tree 和叶子节点顺序访问指针进行实现，它具有 B Tree 的平衡性，并且通过顺序访问指针来提高区间查询的性能。

比较：

红黑树等平衡树也可以用来实现索引，但是文件系统及数据库系统普遍采用 B+ Tree 作为索引结构，主要有以下两个原因：

1. 更少的查找次数

平衡树查找操作的时间复杂度和树高 h 相关，O(h)=O(logdN)，其中 d 为每个节点的出度。

红黑树的出度为 2，而 B+ Tree 的出度一般都非常大，所以红黑树的树高 h 很明显比 B+ Tree 大非常多，查找的次数也就更多。

2. 利用磁盘预读特性

为了减少磁盘 I/O 操作，磁盘往往不是严格按需读取，而是每次都会预读。预读过程中，磁盘进行顺序读取，顺序读取不需要进行磁盘寻道，并且只需要很短的旋转时间，速度会非常快。

操作系统一般将内存和磁盘分割成固定大小的块，每一块称为一页，内存与磁盘以页为单位交换数据。数据库系统将索引的一个节点的大小设置为页的大小，使得一次 I/O 就能完全载入一个节点。并且可以利用预读特性，相邻的节点也能够被预先载入。

### epool libevent (异步I/O)

[libevent](https://www.cnblogs.com/nearmeng/p/4043548.html)

epoll是一种当文件描述符的内核缓冲区非空的时候，发出可读信号进行通知，当写缓冲区不满的时候，发出可写信号通知的机制

执行epoll_create时，创建了红黑树和就绪链表，执行epoll_ctl时，如果增加socket句柄，则检查在红黑树中是否存在，存在立即返回，不存在则添加到树中，然后向内核注册回调函数，用于句柄对应的中断事件来临时向准备就绪链表中插入句柄。执行epoll_wait时立刻返回准备就绪链表里的句柄即可。

epoll的两种模式，水平触发模式下，只要一个句柄上的事件一次没有处理完，会在以后调用epoll_wait时次次返回这个句柄，而边缘触发模式仅在第一次返回。

- 区别与 select I/O 复用的方式，调用epoll_create()方法 内核就已经在内核态开始准备要监控的句柄，epoll_ctl只是往内核的红黑树中塞入新的句柄，然后注册回调函数用于中断事件向就绪链表中插入数据，做到了一次拷贝，返回时仅仅观察就绪链表中是否有数据即可
- select 和 poll 每次调用都需要将全部描述符从应用进程缓冲区复制到内核缓冲区。
- select 和 poll 的返回结果中没有声明哪些描述符已经准备好，所以如果返回值大于 0 时，应用进程都需要使用轮询的方式来找到 I/O 完成的描述符。epoll_wait将准备就绪的句柄直接返回即可
- 另外，当我们调用epoll_ctl往里塞入百万个句柄时，epoll_wait仍然可以飞快的返回，并有效的将发生事件的句柄给我们用户

libevent是一个轻量级的基于事件驱动的高性能的开源网络库，并且支持多个平台，对多个平台的I/O复用技术进行了封装，屏蔽平台不同导致的网络

应用场景：

1. select： select 的 timeout 参数精度为 1ns，而 poll 和 epoll 为 1ms，因此 select 更加适用于实时性要求比较高的场景，比如核反应堆的控制。select 可移植性更好，几乎被所有主流平台所支持。
2. poll： poll 没有最大描述符数量的限制，如果平台支持并且对实时性要求不高，应该使用 poll 而不是 select。
3. epoll： 只需要运行在 Linux 平台上，有大量的描述符需要同时轮询，并且这些连接最好是长连接。

需要同时监控小于 1000 个描述符，就没有必要使用 epoll，因为这个应用场景下并不能体现 epoll 的优势。

需要监控的描述符状态变化多，而且都是非常短暂的，也没有必要使用 epoll。因为 epoll 中的所有描述符都存储在内核中，造成每次需要对描述符的状态改变都需要通过 epoll_ctl() 进行系统调用，频繁系统调用降低效率。并且 epoll 的描述符存储在内核，不容易调试

### 布隆过滤器

[bloomFilter](https://blog.csdn.net/xinzhongtianxia/article/details/81294922)

- hash表中的槽位越多，越浪费空间，槽位越少，效率越低
- 当样本分布极度不均匀的时候，BitSet会造成很大空间上的浪费，当元素不是整型的时候，BitSet就不适用了

```java
public void set(int value) {
  int byteIndex = value / 8;
  int bitIndex = value % 8;
  byte[byteIndex] = byte[byteIndex] | 1 << (7 - bitIndex)
}
```

- 多个hash，增大随机性，减少hash碰撞的概率
- 扩大数组范围，使hash值均匀分布，进一步减少hash碰撞的概率。
- 要想保持错误率低，最好让位数组有一半还空着。

如果对应的bit位值都为1，那么也不能肯定这个url一定存在，也就是说，BloomFilter其实是存在一定的误判的，这个误判的概率显然和数组的大小以及hash函数的个数以及每个hash函数本身的好坏有关

相比于传统的 List、Set、Map 等数据结构，它更高效、占用空间更少，但是缺点是其返回的结果是概率性的，而不是确切的。

应用场景：黑名单，网络爬虫，K-V系统快速判断某个key是否存在 HBase Redis

```java
private static final int SIZE = 1<<24;
BitSet bitSet = new BitSet(SIZE);
Hash[] hashs = new Hash[8];
private static final int seeds[]=new int[]{3,5,7,9,11,13,17,19};
public static void main(String[] args) {
    String email = "zhenlingcn@126.com";
    BloomDemo bloomDemo = new BloomDemo();
    System.out.println(email+"是否在列表中： "+bloomDemo.contains(email));
    bloomDemo.add(email);
    System.out.println(email+"是否在列表中： "+bloomDemo.contains(email));
    email="zhenlingcn@163.com";
    System.out.println(email+"是否在列表中： "+bloomDemo.contains(email));
}
public BloomDemo() {
    for (int i = 0; i < seeds.length; i++) {
        hashs[i] = new Hash(seeds[i]);
    }
}
public void add(String string) {
    for(Hash hash : hashs) {
        bitSet.set(hash.getHash(string), true);
    }
}
public boolean contains(String string) {
    boolean have = true;
    for(Hash hash : hashs) {
        have &= bitSet.get(hash.getHash(string));
    }
    return have;
}
class Hash {
    private int seed = 0;
    public Hash(int seed) {
        this.seed = seed;
    }
    public int getHash(String string) {
        int val = 0;
        int len = string.length();
        for (int i = 0; i < len; i++) {
            val = val * seed + string.charAt(i);
        }
        return val & (SIZE - 1);
    }
}
```

### HashMap 和 HashTable

- HashMap是非线程安全的，HashTable是线程安全的。
- HashMap的键和值都允许有null存在，而HashTable则都不行。
- 因为线程安全、哈希效率的问题，HashMap效率比HashTable的要高

### 字典树 AC自动机 fail节点 KMP

字典树的典型应用是用于文本词频统计，是一颗多叉树，读入文本的同时建立树，每个节点保存了所有的孩子节点，有标记该节点是否为结束节点，利用字符串的公共前缀来降低查询时间的开销以达到提高效率的目的。空间的花费不会超过单词数×单词长度。

AC自动机是多模匹配算法，就是在Trie的基础上构造失效节点 [参考](https://blog.csdn.net/bestsort/article/details/82947639)

[AC自动机算法详解](https://www.cnblogs.com/cmmdc/p/7337611.html)

- 根据模式串建立Trie
- 给Trie添加失效节点 bfs实现
  - fail指针指向他父亲节点 的 fail指针指向的那个节点 的 具有相同字母的子节点
- 输入待处理文本

```java
private int SIZE = 26;
private TrieNode root;//字典树的根

private Trie() {
    root = new TrieNode();
}

private class TrieNode {
    private int num; //有多少单词通过这个节点,即由根至该节点组成的字符串模式出现的次数
    private TrieNode[]  son;
    private boolean isEnd;
    private char val;

    TrieNode() {
        num = 1;
        son = new TrieNode[SIZE];
        isEnd = false;
    }
}

//建立字典树
public void insert(String str) {
    if(str == null || str.length() == 0) {
        return;
    }
    TrieNode node = root;
    char[] letters = str.toCharArray();
    for(int i = 0, len = str.length(); i < len; i++) {
        int pos = letters[i] - 'a';
        if(node.son[pos] == null) {
            node.son[pos] = new TrieNode();
            node.son[pos].val = letters[i];
        } else {
            node.son[pos].num++;
        }
        node = node.son[pos];
    }
    node.isEnd=true;
}
```

KMP：KMP算法的思想是，对匹配串本身先做一个处理，得到一个next数组。next 数组各值的含义：代表当前字符之前的字符串中，有多大长度的相同前缀后缀。例如如果 `next[j] = k`，代表 j 之前的字符串中有最大长度为 k 的相同前缀后缀。next 数组相当于“最大长度值” 整体向右移动一位，然后初始值赋为-1

```java
public static void getNext(String pattern, int next[]) {
    int j = 0;
    int k = -1;
    int len = pattern.length();
    next[0] = -1;

    while (j < len - 1) {
        if (k == -1 || pattern.charAt(k) == pattern.charAt(j)) {
            j++;
            k++;
            next[j] = k;
        } else {
            // 比较到第K个字符，说明p[0——k-1]字符串和p[j-k——j-1]字符串相等，而next[k]表示
            // p[0——k-1]的前缀和后缀的最长共有长度，所接下来可以直接比较p[next[k]]和p[j]
            k = next[k];
        }
    }
}
```

函数式编程中的函数这个术语不是指计算机中的函数（实际上是Subroutine），而是指数学中的函数，即自变量的映射。

函数式编程关心数据的映射，命令式编程关心解决问题的步骤

Hadoop 在一次 MapReduce 运算之后，会将数据的运算结果从内存写到磁盘中，第二次MapReduce运算时从磁盘中读取数据，所以瓶颈在2次运算间的多余IO消耗，Spark则是将数据一致缓存在内存中，直到得到最后的结果

Spark Streaming 是核心Spark API 的扩展，可实现对实时数据流的可扩展、高吞吐量、容错流处理

Spark Streaming 接收实时输入数据流并将数据分成批处理，然后由Spark引擎处理以批量生成最终结果流。

```scala
def doTopK1(lines:RDD[String]):Unit = {
    //计算每一个单词的词频
    val wordCountRDD = lines.flatMap(_.split("\\s+")).map((_, 1)).reduceByKey(_+_)
    //排序
    val sorted = wordCountRDD.map{case(key,value) => (value,key)}.sortByKey(true, 3)
    //得到词频最高的4个单词
    val topk = sorted.top(4)
    //print
    topk.foreach(println)
}
```

## 数据库

数据库范式：

- 第一范式：所有的域都应该是原子性的，即数据库表的每一列都是不可分割的原子数据项
- 第二范式：在1NF的基础上，要求数据库表中的每个实例或记录必须可以被唯一地区分。一个表中只能保存一种数据，不可以把多种数据保存在同一张数据库表中
- 第三范式：在2NF基础上，任何非主属性不依赖于其它非主属性，在2NF基础上消除传递依赖
- BC范式：在3NF基础上，主属性不依赖于主属性

五大约束

- primary KEY:设置主键约束
- UNIQUE：设置唯一性约束，不能有重复值
- DEFAULT：默认值约束
- NOT NULL：设置非空约束，该字段不能为空
- FOREIGN key：设置外键约束

事务指的是满足 ACID 特性的一组操作，可以通过 Commit 提交一个事务，也可以使用 Rollback 进行回滚。

ACID：

- 原子性（Atomicity）
  - 事务被视为不可分割的最小单元，事务的所有操作要么全部提交成功，要么全部失败回滚。
- 一致性（Consistency）
  - 数据库在事务执行前后都保持一致性状态。在一致性状态下，所有事务对一个数据的读取结果都是相同的。
- 隔离性（Isolation）
  - 一个事务所做的修改在最终提交以前，对其它事务是不可见的。
- 持久性（Durability）
  - 一旦事务提交，则其所做的修改将会永远保存到数据库中。即使系统发生崩溃，事务执行的结果也不能丢失。使用重做日志来保证持久性。

并发一致性问题

- 丢失修改：T1 和 T2 两个事务都对一个数据进行修改，T1 先修改，T2 随后修改，T2 的修改覆盖了 T1 的修改。
- 脏读: 事务T1更新了一行记录内容，但并没有提交修改。事务T2读取更新后的行，然后T1执行回滚操作。读取了刚才所做的修改。现在T2读取的行就无效了。（一个事务读取了另一个事务未提交的数据）
- 不可重复读：事务T1读取了一行记录，紧接着T2修改了T1刚才读取的那一行记录，然后T1又再次读取这行记录，发现与刚才读取的结果不同。
- 幻读：事务T1读取一个结果集，然后T2事务在T1结果集范围内插入一行记录。然后T1再次对表进行检索，发现多了T2插入的数据。

产生并发不一致性问题主要原因是破坏了事务的隔离性，解决方法是通过并发控制来保证隔离性，数据库管理系统提供了事务的隔离级别，让用户以一种更轻松的方式处理并发一致性问题。

封锁

MySQL 中提供了两种封锁粒度：行级锁以及表级锁

应该尽量只锁定需要修改的那部分数据，而不是所有的资源。锁定的数据量越少，发生锁争用的可能就越小，系统的并发程度就越高。

但是加锁需要消耗资源，锁的各种操作（包括获取锁、释放锁、以及检查锁状态）都会增加系统开销。因此封锁粒度越小，系统开销就越大。

- 读写锁
  - 排他锁：简写为 X 锁，又称写锁。
  - 共享锁：简写为 S 锁，又称读锁。
- 意向锁：可以更容易地支持多粒度封锁。在存在行级锁和表级锁的情况下，事务 T 想要对表 A 加 X 锁，就需要先检测是否有其它事务对表 A 或者表 A 中的任意一行加了锁，那么就需要对表 A 的每一行都检测一次，这是非常耗时的。引入了IS和IX，都是表锁
  - 一个事务在获得**某个数据行对象**的 S 锁之前，必须先获得表的 IS 锁或者更强的锁
  - 一个事务在获得**某个数据行对象**的 X 锁之前，必须先获得表的 IX 锁

封锁协议

- 三级封锁协议
- 两段锁协议，MySQL 的 InnoDB 存储引擎采用两段锁协议，会根据隔离级别在需要的时候自动加锁，并且所有的锁都是在同一时刻被释放，这被称为隐式锁定。

隔离级别

- 未提交读：事务中的修改，即使没有提交，对其它事务也是可见的。会导致脏读、不可重复读和幻读
- 提交读：一个事务只能读取已经提交的事务所做的修改。换句话说，一个事务所做的修改在提交之前对其它事务是不可见的。会导致不可重复读和幻读
- 可重复读：保证在同一个事务中多次读取同样数据的结果是一样的。会导致幻读
- 可串行化：强制事务串行执行，会导致加锁读

脏读 | 不可重复读| 幻读 | 加锁读
| --- | --- | --- | --- | --- |
未提交读 | YES | YES | YES | NO
提交读 | NO | YES | YES | NO
可重复读 | NO | NO | YES | NO
可串行化 | NO | NO | NO | YES

多版本并发控制（Multi-Version Concurrency Control, MVCC）是 MySQL 的 InnoDB 存储引擎实现隔离级别的一种具体方式，用于实现提交读和可重复读这两种隔离级别。

Next-Key Locks 是 MySQL 的 InnoDB 存储引擎的一种锁实现。

MVCC 不能解决幻读的问题，Next-Key Locks 就是为了解决这个问题而存在的。在可重复读（REPEATABLE READ）隔离级别下，使用 MVCC + Next-Key Locks 可以解决幻读问题。

### 数据库锁机制

数据库锁定机制简单来说就是数据库为了保证数据的一致性而使各种共享资源在被并发访问，访问变得有序所设计的一种规则，MySQL各存储引擎使用了三种类型（级别）的锁定机制，行级锁定(容易发生死锁)，页级锁定和表级锁定

- InnoDB的行级锁同样分为两种，共享锁和排他锁，同样InnoDB也引入了意向锁（表级锁）的概念，所以也就有了意向共享锁和意向排他锁，所以InnoDB实际上有四种锁，即共享锁（S）、排他锁（X）、意向共享锁（IS）、意向排他锁（IX）
- 在MySQL数据库中，使用表级锁定的主要是MyISAM

- 悲观锁：假定会发生并发冲突，屏蔽一切可能违反数据完整性的操作。加锁的时间可能会很长，这样可能会长时间的限制其他用户的访问，也就是说悲观锁的并发访问性不好
- 乐观锁：假设不会发生并发冲突，只在提交操作时检查是否违反数据完整性。乐观锁不能解决脏读的问题。

### 存储引擎

不同的存储引擎决定了MySQL数据库中的表可以用不同的方式来存储

#### InnoDB

是 MySQL 默认的事务型存储引擎，只有在需要它不支持的特性时，才考虑使用其它存储引擎。

实现了四个标准的隔离级别，默认级别是可重复读（REPEATABLE READ）。在可重复读隔离级别下，通过多版本并发控制（MVCC）+ 间隙锁（Next-Key Locking）防止幻影读。

主索引是聚簇索引，在索引中保存了数据，从而避免直接读取磁盘，因此对查询性能有很大的提升。

内部做了很多优化，包括从磁盘读取数据时采用的可预测性读、能够加快读操作并且自动创建的自适应哈希索引、能够加速插入操作的插入缓冲区等。

支持真正的在线热备份。其它存储引擎不支持在线热备份，要获取一致性视图需要停止对所有表的写入，而在读写混合场景中，停止写入可能也意味着停止读取。

#### MyISAM

设计简单，数据以紧密格式存储。对于只读数据，或者表比较小、可以容忍修复操作，则依然可以使用它。

提供了大量的特性，包括压缩表、空间数据索引等。

不支持事务。

不支持行级锁，只能对整张表加锁，读取时会对需要读到的所有表加共享锁，写入时则对表加排它锁。但在表有读取操作的同时，也可以往表中插入新的记录，这被称为并发插入（CONCURRENT INSERT）。

可以手工或者自动执行检查和修复操作，但是和事务恢复以及崩溃恢复不同，可能导致一些数据丢失，而且修复操作是非常慢的。

如果指定了 DELAY_KEY_WRITE 选项，在每次修改执行完成时，不会立即将修改的索引数据写入磁盘，而是会写到内存中的键缓冲区，只有在清理键缓冲区或者关闭表的时候才会将对应的索引块写入磁盘。这种方式可以极大的提升写入性能，但是在数据库或者主机崩溃时会造成索引损坏，需要执行修复操作。

- Memory: 将表中的数据存放在内存中，如果数据库重启或发生崩溃，表中的数据库都将消失，它非常适合存储临时数据的临时表．默认采用哈希索引

比较：

- 事务：InnoDB 是事务型的，可以使用 Commit 和 Rollback 语句。
- 并发：MyISAM 只支持表级锁，而 InnoDB 还支持行级锁。
- 外键：InnoDB 支持外键。
- 备份：InnoDB 支持在线热备份。
- 崩溃恢复：MyISAM 崩溃后发生损坏的概率比 InnoDB 高很多，而且恢复的速度也更慢。
- 其它特性：MyISAM 支持压缩表和空间数据索引（空间数据索引会从所有维度来索引数据，可以有效地使用任意维度来进行组合查询）。

SQL注入：把SQL命令插入到表单提交或输入域名或页面请求的查询字符串，最终达到欺骗服务器执行恶意的SQL命令

- 检验用户输入
- 不使用动态拼装的SQL 参数化SQL
- 每个应用使用单独的权限有限的数据库连接
- hash敏感信息
- 应用异常，模糊提示

### 数据库索引

索引是一个单独存储在磁盘上的数据库结构，它们包含着对数据表里所有记录的引用指针，使用索引可以提高数据库特定数据的查询速度.索引时在存储引擎中实现的，因此每种存储引擎的索引不一定完全相同,并且每种存储引擎也不一定支持所有索引类型

索引失效：

- WHERE字句的查询条件里有不等于号
- 如果WHERE字句的查询条件里使用了函数
- 在JOIN操作中（需要从多个数据表提取数据时），MYSQL只有在主键和外键的数据类型相同时才能使用索引，否则即使建立了索引也不会使用

索引优化

- 独立的列：在进行查询时，索引列不能是表达式的一部分，也不能是函数的参数，否则无法使用索引
- 多列索引：在需要使用多个列作为条件进行查询时，使用多列索引比使用多个单列索引性能更好
- 索引列的顺序：让选择性最强的索引列放在前面
- 前缀索引：对于 BLOB、TEXT 和 VARCHAR 类型的列，必须使用前缀索引，只索引开始的部分字符
- 覆盖索引：索引包含所有需要查询的字段的值。

### 索引实现

MyISAM索引实现

- MyISAM索引使用了B+Tree作为索引结构，叶子结点的data域存放的是数据记录的地址。
- MyISAM中索引检索的算法为首先按照B+Tree搜索算法搜索索引，如果指定的Key存在，则取出其data域的值，然后以data域的值为地址，读取相应数据记录。
- 主索引和辅助索引的存储结构没有任何区别。

InnoDB索引实现

- InnoDB 的 B+Tree 索引分为主索引和辅助索引。主索引的叶子节点 data 域记录着完整的数据记录，这种索引方式被称为聚簇索引。因为无法把数据行存放在两个不同的地方，所以一个表只能有一个聚簇索引。
- 辅助索引的叶子节点的 data 域记录着主键的值，因此在使用辅助索引进行查找时，需要先查找到主键值，然后再到主索引中进行查找。
- 适用于全键值、键值范围和键前缀查找，其中键前缀查找只适用于最左前缀查找。如果不是按照索引列的顺序进行查找，则无法使用索引。

### 聚集索引和非聚集索引

- 如果给表上了主键，那么表在磁盘上的存储结构就由整齐排列的结构转变成了树状结构，也就是上面说的「平衡树」结构，换句话说，就是整个表就变成了一个索引。没错， 再说一遍， 整个表变成了一个索引，也就是所谓的「聚集索引」。
- 索引能让数据库查询数据的速度上升， 而使写入数据的速度下降，原因很简单的， 因为平衡树这个结构必须一直维持在一个正确的状态， 增删改数据都会改变平衡树各节点中的索引数据内容，破坏树结构， 因此，在每次数据改变时， DBMS必须去重新梳理树（索引）的结构以确保它的正确

- 如果给表中多个字段加上索引， 那么就会出现多个独立的索引结构，每个索引（非聚集索引）互相之间不存在关联
- 每次给字段建一个新索引， 字段中的数据就会被复制一份出来， 用于生成索引。 因此给表添加索索引会增加表的体积，占用磁盘存储空间。

非聚集索引和聚集索引的区别在于，通过聚集索引可以查到需要查找的数据，而通过非聚集索引可以查到记录对应的主键值，再使用主键的值通过聚集索引查找到需要的数据，不管以任何方式查询表，最终都会利用主键通过聚集索引来定位到数据， 聚集索引（主键）是通往真实数据所在的唯一路径。

有一种例外可以不使用聚集索引就能查询出所需要的数据，这种非主流的方法 称之为「覆盖索引」查询，也就是平时所说的复合索引或者多字段索引查询，当为字段建立索引以后，字段中的内容会被同步到索引之中，如果为一个索引指定两个字段，那么这个两个字段的内容都会被同步至索引之中。

### 切分

#### 水平切分

水平切分又称为 Sharding，它是将同一个表中的记录拆分到多个结构相同的表中。

当一个表的数据不断增多时，Sharding 是必然的选择，它可以将数据分布到集群的不同节点上，从而缓存单个数据库的压力。

#### 垂直切分

垂直切分是将一张表按列切分成多个表，通常是按照列的关系密集程度进行切分，也可以利用垂直切分将经常被使用的列和不经常被使用的列切分到不同的表中。

在数据库的层面使用垂直切分将按数据库中表的密集程度部署到不同的库中，例如将原来的电商数据库垂直切分成商品数据库、用户数据库等。

## Linux

系统调用是操作系统的最小功能单位，内核向上层应用提供访问的接口

内核从本质上看是一种软件——控制计算机的硬件资源，并提供上层应用程序运行的环境，用户态即上层应用程序的活动空间，应用程序的执行必须依托于内核提供的资源

库函数实现对系统调用的封装，将简单的业务逻辑结构呈现给用户，方便用户调用

用户态到内核态切换

- 系统调用 (软中断)
- 异常事件
- 外围设备的中断 (硬中断)

## java基础

[Java 基础](https://github.com/CyC2018/CS-Notes/blob/master/docs/notes/Java%20%E5%9F%BA%E7%A1%80.md)

[Java 容器](https://github.com/CyC2018/CS-Notes/blob/master/docs/notes/Java%20%E5%AE%B9%E5%99%A8.md)

```java
// 取模操作
// `y & (x - 1)` 等价于 `y % x`
static int indexFor(int h, int length) {
    return h & (length-1);
}
```

```java
// 以下是 HashMap 中计算数组容量的代码
// n = 11011000 计算到 n |= n >>> 4， n = 11111111
// n = 00001010 计算到 n |= n >>> 2， n = 00001111
// 此时 n + 1 是大于原始数字的最小的 2 的 n 次方。
static final int tableSizeFor(int cap) {
    int n = cap - 1;
    n |= n >>> 1;
    n |= n >>> 2;
    n |= n >>> 4;
    n |= n >>> 8;
    n |= n >>> 16;
    return (n < 0) ? 1 : (n >= MAXIMUM_CAPACITY) ? MAXIMUM_CAPACITY : n + 1;
}
```

java String

```java
public class TestString{
    public static void main(String args[]){
        String str1 = "abc";
        String str2 = new String("abc");
        String str3 = s2.intern();

        System.out.println( str1 == str2 );   //false
        System.out.println( str1 == str3 );   //true
    }
}
// String str1 = “abc”，str1 引用的是 常量池（方法区） 的对象；而 String str2 = new String(“abc”)，str2引用的是 堆 中的对象，所以内存地址不一样。但是由于内容一样，所以 str1 和 str3 指向同一对象。
```

```java
String s1 = "abc";
//↑ 在字符串池创建了一个对象  
String s2 = "abc";
//↑ 字符串pool已经存在对象“abc”(共享),所以创建0个对象，累计创建一个对象  
System.out.println("s1 == s2 : "+(s1==s2));
//↑ true 指向同一个对象，  
System.out.println("s1.equals(s2) : " + (s1.equals(s2)));
//↑ true  值相等

String s3 = new String("abc");  
//↑ 创建了两个对象，一个存放在字符串池中，一个存在与堆区中；  
//↑ 还有一个对象引用s3存放在栈中  
String s4 = new String("abc");  
//↑ 字符串池中已经存在“abc”对象，所以只在堆中创建了一个对象  
System.out.println("s3 == s4 : "+(s3==s4));  
//↑false   s3和s4栈区的地址不同，指向堆区的不同地址；  
System.out.println("s3.equals(s4) : "+(s3.equals(s4)));  
//↑true  s3和s4的值相同  
System.out.println("s1 == s3 : "+(s1==s3));  
//↑false 存放的地区都不同，一个方法区，一个堆区  
System.out.println("s1.equals(s3) : "+(s1.equals(s3)));  
//↑true  值相同

String str2 = "ab";  //1个对象  
String str3 = "cd";  //1个对象
String str4 = str2 + str3;
String str5 = "abcd";
System.out.println("str4 = str5 : " + (str4==str5)); // false  
// 引用str4和str5指向的对象的地址必定不一样
// 三个在字符串常量池中的String对象、一个在堆中的String对象和一个在堆中的StringBuilder对象。
// 调用 String 类的静态方法 String.valueOf() 将 str2 转换为字符串表示；
// JVM 在堆中创建一个 StringBuilder对象，同时用str2指向转换后的字符串对象进行初始化；
// 调用StringBuilder对象的append方法完成与str3所指向的字符串对象的合并；
// 调用 StringBuilder 的 toString() 方法在堆中创建一个 String对象；
// 将刚刚生成的String对象的堆地址存赋给局部变量引用str4。

String str1 = "ab" + "cd";  //1个对象  
String str11 = "abcd";
System.out.println("str1 = str11 : "+ (str1 == str11));   // true

final String str8 = "cd";  
String str9 = "ab" + str8;  
String str89 = "abcd";  
System.out.println("str9 = str89 : "+ (str9 == str89));     // true
//↑str8为常量变量，编译期会被优化  
// 常量+字面值”的组合，其值在编译的时候就能够被确定了

String str6 = "b";  
String str7 = "a" + str6;  
String str67 = "ab";  
System.out.println("str7 = str67 : "+ (str7 == str67));     // false
//↑str6为变量，在运行期才会被解析。
```

```java
public static void main(String args[])
{
    String s = "abc";            // 编译期优化
    String s1 = "a";
    String s2 = "b";
    String s3 = "c";
    String s4 = s1 + s2 + s3;

    //底层使用 StringBuilder 进行字符串的拼接
    String s4 = (new StringBuilder(String.valueOf(s1))).append(s2).append(s3).toString();
    System.out.println(s);
    System.out.println(s4);
}
```

```java
public static void main(String args[]) {
    String s = null;
    for(int i = 0; i < 100; i++) {
        s += "a";
        // s = (new StringBuilder(String.valueOf(s))).append("a").toString();
    }
}
```

hashcode() 和 equal()

- 相等（相同）的对象必须具有相等的哈希码（或者散列码）。
- 如果两个对象的hashCode相同，它们并不一定相同。

### 垃圾回收机制

判断一个对象是否可被回收

- 1.引用计数算法
- 2.可达性分析算法 可达的对象是存活的，不可达的对象是可被回收的

方法区回收，主要是对常量池的回收和对类的卸载。

类卸载至少要满足三个条件

- 该类所有的实例都已经被回收，此时堆中不存在该类的任何实例。
- 加载该类的 ClassLoader 已经被回收。
- 该类对应的 Class 对象没有在任何地方被引用，也就无法在任何地方通过反射访问该类方法。

为什么要分代？

- 标记和压缩JVM中的所有对象效率低
- 随着越来越多的对象被分配，对象列表会越来越长，导致垃圾收集时间越来越长
- 大部分对象是短暂的

新生代：复制算法

- Minor GC

老年代：标记清除算法或标记整理算法

- Major GC
- 慢因为涉及到所有的存活对象

永久代：应用程序中使用的类和方法所需的元数据，运行时应用程序使用的类，JavaSE库的类和方法

垃圾回收流程：

- 1.任何新对象分配在新生代Eden空间，两个Survivor空间都是空的
- 2.当Eden空间填满，会触发Minor GC
- 3.引用的对象被移动到第一个Survivor空间 S0，清除Eden空间，删除未引用的对象
- 4.在下一次Minor GC时会发生相同的事，删除未引用对象，它们被移动到第二个Survivor空间 S1
- 此时来自 S0 的最后一个 Minor GC 中的对象年龄增加1，移动到 S1 中，一旦所有幸存的对象都移动到 S1 中，S0 和Eden会被清除，这时在 S1 中有不同年龄的对象
- 5.下一次Minor GC时，重复相同的过程，被引用的对象会被移动到另外一个S0，幸存对象老化，清空Eden和S1
- 6.在Minor GC之后，当老化的对象达到一定的年龄阈值时，它们从年轻代移入老年代
- 7.随着Minor GC发生，对象被陆续移动到老年代中
- 8.最终老年代进行整理和压缩空间，大对象直接进入老年代

动态对象年龄判定

如果在 Survivor 中相同年龄所有对象大小的总和大于 Survivor 空间的一半，则年龄大于或等于该年龄的对象可以直接进入老年代

空间分配担保

在发生 Minor GC 之前，虚拟机先检查老年代最大可用的连续空间是否大于新生代所有对象总空间

- 如果条件成立的话，那么 Minor GC 可以确认是安全的。
- 如果不成立的话虚拟机会查看 HandlePromotionFailure 设置值是否允许担保失败
  - 如果允许那么就会继续检查老年代最大可用的连续空间是否大于历次晋升到老年代对象的平均大小
    - 如果大于，将尝试着进行一次 Minor GC
    - 如果小于，进行 Major GC
  - 不允许冒险，那么就要进行一次 Major GC。

CPU资源敏感，注重高吞吐量的场合 优先考虑 Parallel Scavenge 和 Parallel Old收集器

- **Serial收集器** (复制算法)
  - 新生代单线程收集器，标记和清理都是单线程，优点是简单高效。是client级别默认的GC方式，可以通过 `-XX:+UseSerialGC` 来强制指定。
- **Serial Old收集器** (标记-整理算法)
  - 老年代单线程收集器，Serial收集器的老年代版本。
- **ParNew收集器** (复制算法)
  - 新生代收集器，可以认为是Serial收集器的多线程版本,在多核CPU环境下有着比Serial更好的表现。
- **Parallel Scavenge收集器** (停止-复制算法)
  - 并行收集器，追求高吞吐量，高效利用CPU。吞吐量一般为99%， 吞吐量= 用户线程时间/(用户线程时间+GC线程时间)。适合后台应用等对交互相应要求不高的场景。是server级别默认采用的GC方式，
- **Parallel Old收集器** (标记-整理算法)
  - Parallel Scavenge收集器的老年代版本，并行收集器，吞吐量优先。
- **CMS(Concurrent Mark Sweep)收集器** (标记-清除算法)
  - 获取最短回收停顿时间为目的的收集器
  - 四个过程，初始标记（停顿） - 并发标记 - 重新标记（停顿） - 并发清除
  - 缺点：吞吐量低，cpu利用率不高，无法处理浮动垃圾，标记-清除算法导致空间碎片，老年代无法找到足够大的连续空间，不得不Full GC
- **G1** (整体上是标记-整理算法，局部是基于复制算法实现的，这意味着运行期间不会产生内部空间碎片)
  - 面向服务端应用的垃圾回收器
  - 四个过程，初始标记 - 并发标记 - 最终标记 - 筛选回收
  - 特点：并行与并发（多核环境缩短停顿时间），分代收集（更好的收集效果），空间整合，可预测的停顿（预测停顿时间）

### 类加载机制

虚拟机把描述类的数据从Class文件加载到内存，并对数据进行校验，转换解析和初始化最终形成可以被虚拟机直接使用的Java类型，这就是虚拟机的类加载机制

- 加载（Loading）
- 验证（Verification）
- 准备（Preparation）
- 解析（Resolution）
- 初始化（Initialization）
- 使用（Using）
- 卸载（Unloading）

类加载时机：5种情况必须对类进行初始化（主动引用）

- new getstatic putstatic invokestatic 4条字节码指令，即new关键字实例化对象的时候，读取或设置一个类的静态字段的时候，调用一个类的静态方法的时候
- java.lang.reflect包对类进行反射调用，如果类没有初始化就需要先初始化
- 父类未初始化，触发父类的初始化，接口在初始化时并不要求其父接口全部完成了初始化，只有在真正使用到父接口的时候才会初始化
- 虚拟机启动时，用户需要指定一个要执行的主类，虚拟机会先初始化这个主类
- 方法句柄对应的类没有初始化，需要先触发初始化

被动引用

- 通过子类引用父类的静态字段，不会导致子类的初始化
- 通过数组定义来引用类不会触发类的初始化
- 常量在编译阶段会存入调用类的常量池中，本质上没有直接引用到定义常量的类，因此不会触发定义常量的类的初始化

#### 加载

过程完成以下三件事

- 通过类的完全限定名称获取定义该类的二进制字节流。
- 将该字节流表示的静态存储结构转换为方法区的运行时存储结构。
- 在内存中生成一个代表该类的 java.lang.Class 对象，作为方法区中该类各种数据的访问入口。

其中二进制字节流可以从以下方式中获取：

- 从 ZIP 包读取，成为 JAR、EAR、WAR 格式的基础。
- 从网络中获取，最典型的应用是 Applet。
- 运行时计算生成，例如动态代理技术，在 java.lang.reflect.Proxy 使用 ProxyGenerator.generateProxyClass 的代理类的二进制字节流。
- 由其他文件生成，例如由 JSP 文件生成对应的 Class 类。
- 从数据库中读取 中间件服务器，把程序安装到数据库完成程序代码在集群间的分发

数组加载，如果数组元素是引用类型，数组将在加载该组件类型的类加载器的类名称空间上被表示
如果数组元素不是引用类型，Java虚拟机会把数组C标记为与引导类加载器关联

#### 验证

确保 Class 文件的字节流中包含的信息符合当前虚拟机的要求，并且不会危害虚拟机自身的安全。

- 文件格式验证
  - 是否以魔数开头
  - 常量池的常量中是否有不被支持的常量类型
- 元数据验证
  - 这个类是否有父类
  - 这个类的父类是否继承了不允许被继承的类
- 字节码验证
  - 保证任意时刻操作数栈的数据类型和指令代码序列都能配合工作
- 符号引用验证
  - 符号引用中通过字符串描述的全限定名是否能够找到对应的类
  - 符号引用中类，字段，方法的访问性

访问权限 | 本类 | 本包的类 | 子类 | 非子类的外包类
| --- | --- | --- | --- | --- |
public | 是 | 是 | 是 | 是
protected | 是 | 是 | 是 | 否
default | 是 | 是 | 否 | 否
private | 是 | 否 | 否 | 否

#### 准备

正式为类变量（static修饰）分配内存并设置类变量初始值的阶段，这些变量所使用的内存都在方法区中被分配

实例变量不会在这阶段分配内存，它会在对象实例化时随着对象一起被分配在堆中。应该注意到，实例化不是类加载的一个过程，类加载发生在所有实例化操作之前，并且类加载只进行一次，实例化可以进行多次。

```java
// 初始值一般为 0 值，例如下面的类变量 value 被初始化为 0 而不是 123。
public static int value = 123;
// 类变量是常量，那么它将初始化为表达式所定义的值而不是 0。例如下面的常量 value 被初始化为 123 而不是 0。
public static final int value = 123;
```

#### 解析

将常量池的符号引用替换为直接引用的过程。

- 符号引用：符号引用和虚拟机的布局无关，一组符号来描述引用的目标，符号引用可以是任何形式的字面量，无歧义地定位到目标即可，目标可能未加载到内存中
- 直接引用：直接引用和虚拟机的布局相关，可以是直接指向目标的指针、相对偏移量、一个间接定位到对象的句柄，目标一定加载到内存中

- 类或接口解析
- 字段解析
- 类方法解析
- 接口方法解析

#### 初始化

初始化阶段才真正开始执行类中定义的 Java 程序代码，初始化阶段是虚拟机执行类构造器 `<clinit>()` 方法的过程，在准备阶段，类变量已经赋过一次系统要求的初始值，而在初始化阶段，根据程序员通过程序制定的主观计划去**初始化类变量**和其它资源。

- `<clinit>()` 方法是由编译器自动收集类中所有 类变量的赋值动作 和 静态语句块的语句 合并产生的，编译器收集的顺序是由语句在源文件中出现的顺序所决定的
- `<clinit>()` 保证在子类的 `<clinit>()` 方法执行之前，父类的 `<clinit>()` 方法已经执行了
- 由于父类的 `<clinit>()` 方法先执行，也就意味着父类中定义的静态语句块的执行要优先于子类
- 如果一个类没有静态语句块，也没有对变量的赋值操作，那么编译器可以不为这个类生成 `<clinit>()` 方法
- 接口中不可以使用静态语句块，但仍然有类变量初始化的赋值操作，因此接口与类一样都会生成 `<clinit>()` 方法。但接口与类不同的是，执行接口的 `<clinit>()` 方法不需要先执行父接口的 `<clinit>()` 方法。只有当父接口中定义的变量使用时，父接口才会初始化。另外，接口的实现类在初始化时也一样不会执行接口的 `<clinit>()` 方法。
- 虚拟机会保证一个类的 `<clinit>()` 方法在多线程环境下被正确的加锁和同步，如果多个线程同时初始化一个类，只会有一个线程执行这个类的 `<clinit>()` 方法，其它线程都会阻塞等待，直到活动线程执行 `<clinit>()` 方法完毕。如果在一个类的 `<clinit>()` 方法中有耗时的操作，就可能造成多个线程阻塞，在实际过程中此种阻塞很隐蔽。

### 类和类加载器

两个类相等，需要类本身相等，并且使用同一个类加载器进行加载。这是因为每一个类加载器都拥有一个独立的类名称空间。

这里的相等，包括类的 Class 对象的 equals() 方法、isAssignableFrom() 方法、isInstance() 方法的返回结果为 true，也包括使用 instanceof 关键字做对象所属关系判定结果为 true。

从 Java 虚拟机的角度来讲，只存在以下两种不同的类加载器：

- 启动类加载器（Bootstrap ClassLoader），使用 C++ 实现，是虚拟机自身的一部分；
- 所有其它类的加载器，使用 Java 实现，独立于虚拟机，继承自抽象类 java.lang.ClassLoader。

从 Java 开发人员的角度看，类加载器可以划分得更细致一些：

**启动类加载器（Bootstrap ClassLoader）** 此类加载器负责将存放在 <JRE_HOME>/lib 目录中的，或者被 -Xbootclasspath 参数所指定的路径中的，并且是虚拟机识别的（仅按照文件名识别，如 rt.jar，名字不符合的类库即使放在 lib 目录中也不会被加载）类库加载到虚拟机内存中。启动类加载器无法被 Java 程序直接引用，用户在编写自定义类加载器时，如果需要把加载请求委派给启动类加载器，直接使用 null 代替即可。

**扩展类加载器（Extension ClassLoader）** 这个类加载器是由 ExtClassLoader（sun.misc.Launcher$ExtClassLoader）实现的。它负责将 <JAVA_HOME>/lib/ext 或者被 java.ext.dir 系统变量所指定路径中的所有类库加载到内存中，开发者可以直接使用扩展类加载器。

**应用程序类加载器（Application ClassLoader）** 这个类加载器是由 AppClassLoader（sun.misc.Launcher$AppClassLoader）实现的。由于这个类加载器是 ClassLoader 中的 getSystemClassLoader() 方法的返回值，因此一般称为系统类加载器。它负责加载用户类路径（ClassPath）上所指定的类库，开发者可以直接使用这个类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是程序中默认的类加载器。

**双亲委派模型（Parents Delegation Model）** 该模型要求除了顶层的启动类加载器外，其它的类加载器都要有自己的父类加载器。类加载器之间的父子关系一般通过组合关系（Composition）来实现，而不是继承关系（Inheritance）。

工作流程：一个类加载器首先将类加载请求转发到父类加载器，只有当父类加载器无法完成时才尝试自己加载。

好处：使得 Java 类随着它的类加载器一起具有一种带有优先级的层次关系，从而使得基础类得到统一。

Tomcat:

- /common/*: 类库可被tomcat和所有的web应用程序共同使用
- /server/*: 类库可被tomcat使用，对所有web应用不可见
- /shared/*: 类库可以被所有的web应用使用，对tomcat不可见
- /WEB-INF/*: 仅被此web应用使用，tomcat和其他应用不可见

jsp热替换，当服务器检测到JSP文件被修改，会替换掉目前的JasperLoader的实例，并通过再建立一个新的JSP类加载器来实现

OSGi 灵活的类加载结构

### 对象的创建，对象的内存布局，对象的访问定位

对象的创建时机：

- 使用new关键字创建对象
- 使用Class类的newInstance方法(反射机制)
- 使用Constructor类的newInstance方法(反射机制)
- 使用clone方法创建对象
- 使用(反)序列化机制创建对象

1. 遇到new指令时，首先检查这个指令的参数是否能在常量池中定位到一个类的符号引用，并检查这个类是否已经能够被加载、解析、初始化
2. 为对象分配内存，指针碰撞分配方式和空闲链表两种方式，取决于垃圾回收器是否带有压缩整理的功能
3. 除了如何分配空间的问题，还有线程安全的问题，一种是 CAS配上失败重试保证原子性，另一种是把内存分配的动作按照线程划分不同空间之中进行，TLAB用完才同步
4. 初始化为零值
5. 设置类，如这个对象是哪个类的实例，如何才能找到类的元数据，对象的hash码，gc分代年龄，存在对象头中
6. 执行 `<init>` 方法，把对象按照程序员的意向初始化

- 首先实例化Object类，再依次对以下各类进行实例化，直到完成对目标类的实例化。
- 具体而言，在实例化每个类时，都遵循如下顺序：先依次执行 实例变量初始化 和 实例代码块初始化，再执行构造函数初始化。
- 也就是说，编译器会将 实例变量初始化 和 实例代码块初始化 相关代码 放到类的构造函数中去，并且这些代码会被放在对超类构造函数的调用语句之后，构造函数本身的代码之前。

对象的内存布局

对象在内存中3块区域， 对象头，实例数据和对齐填充

- 对象头 第一部分用于存储对象自身的运行时数据，另一部分是类型指针
- 实例数据 代码中定义的各种类型字段内容
- 补齐

对象的访问定位

栈上的reference数据来操作堆上的具体对象

- 句柄 堆中划分出一块内存作为句柄池，reference存储的是对象的句柄地址，而句柄中包含了对象实例数据和类型数据的具体地址
- 直接指针 reference存储的是对象实例数据，实例数据中包含到对象类型数据的指针

```java
public class Student implements Cloneable, Serializable {

    private int id;

    public Student() {

    }

    public Student(Integer id) {
        this.id = id;
    }

    @Override
    protected Object clone() throws CloneNotSupportedException {
        // TODO Auto-generated method stub
        return super.clone();
    }

    @Override
    public String toString() {
        return "Student [id=" + id + "]";
    }

    public static void main(String[] args) throws Exception {

        System.out.println("使用new关键字创建对象：");
        Student stu1 = new Student(123);
        System.out.println(stu1);
        System.out.println("\n---------------------------\n");


        System.out.println("使用Class类的newInstance方法创建对象：");
        Student stu2 = Student.class.newInstance();    //对应类必须具有无参构造方法，且只有这一种创建方式
        System.out.println(stu2);
        System.out.println("\n---------------------------\n");

        System.out.println("使用Constructor类的newInstance方法创建对象：");
        Constructor<Student> constructor = Student.class
                .getConstructor(Integer.class);   // 调用有参构造方法
        Student stu3 = constructor.newInstance(123);   
        System.out.println(stu3);
        System.out.println("\n---------------------------\n");

        System.out.println("使用Clone方法创建对象：");
        Student stu4 = (Student) stu3.clone();
        System.out.println(stu4);
        System.out.println("\n---------------------------\n");

        System.out.println("使用(反)序列化机制创建对象：");
        // 写对象
        ObjectOutputStream output = new ObjectOutputStream(
                new FileOutputStream("student.bin"));
        output.writeObject(stu4);
        output.close();

        // 读取对象
        ObjectInputStream input = new ObjectInputStream(new FileInputStream(
                "student.bin"));
        Student stu5 = (Student) input.readObject();
        System.out.println(stu5);

    }
}
```

### Java 线程中断

通过调用一个线程的 interrupt() 来中断该线程，如果该线程处于阻塞、限期等待或者无限期等待状态，那么就会抛出 InterruptedException，从而提前结束该线程。但是不能中断 I/O 阻塞和 synchronized 锁阻塞。

```java
//中断线程（实例方法）
public void Thread.interrupt();

//判断线程是否被中断（实例方法）
public boolean Thread.isInterrupted();

//判断是否被中断并清除当前中断状态（静态方法）
public static boolean Thread.interrupted();
```

中断两种情况：

- 一种是当线程处于阻塞状态或者试图执行一个阻塞操作时，我们可以使用实例方法interrupt()进行线程中断，执行中断操作后将会抛出interruptException异常(该异常必须捕捉无法向外抛出)并将中断状态复位
- 另外一种是当线程处于运行状态时，我们也可调用实例方法interrupt()进行线程中断，但同时必须手动判断中断状态，并编写中断线程的代码(其实就是结束run方法体的代码)

兼顾两种情况：

```java
public void run() {
    try {
    //判断当前线程是否已中断,注意interrupted方法是静态的,执行后会对中断状态进行复位
    while (!Thread.interrupted()) {
        TimeUnit.SECONDS.sleep(2);
    }
    } catch (InterruptedException e) {

    }
}
```

### Java NIO

[Java I/O](https://github.com/CyC2018/CS-Notes/blob/master/docs/notes/Java%20IO.md)

### Spring

IOC 控制反转：通过容器来控制业务对象之间的依赖关系，而非传统实现中，由代码直接控制。这也就是“控制反转”概念所在;控制权由应用代码转到了外部容器，控制权的转移，就是反转。控制权转移带来的好处就是降低了业务对象之间的依赖程度

AOP 面向切面：切面实现了横向关注点（跨多个应用对象的逻辑）的模块化，我们在一个地方定义通用功能，可以通过声明的方式定义这个功能要以何种方式在何应用，而无需修改受影响的类

动态代理：

静态代理方式需要为每个接口实现一个代理类，而这些代理类中的代码几乎是一致的。这在大型系统中将会产生很大的维护问题

创建Subject类型接口和我们要代理的真实对象：

```java
public interface Subject {
    public void rent();
    public void hello();
}

public class RealSubject implements Subject {
    public void rent() {
        System.out.println("I want to rent my house");
    }

    @Override
    public void hello(String str) {
        System.out.println("hello: " + str);
    }
}
```

定义动态代理类：

```java
public class DynamicProxy implements InvocationHandler {
    // 这个就是我们要代理的真实对象
    private Object subject;

    // 构造方法，给我们要代理的真实对象赋初值
    public DynamicProxy(Object subject) {
        this.subject = subject;
    }

    @Override
    public Object invoke(Object object, Method method, Object[] args) throws Throwable {
        // 在代理真实对象前我们可以添加一些自己的操作
        System.out.println("before rent house");

        System.out.println("Method:" + method);

        // 当代理对象调用真实对象的方法时，其会自动的跳转到代理对象关联的handler对象的invoke方法来进行调用
        method.invoke(subject, args);

        // 在代理真实对象后我们也可以添加一些自己的操作
        System.out.println("after rent house");

        return null;
    }
}
```

```java
public class Client {
    public static void main(String[] args) {
        // 我们要代理的真实对象
        Subject realSubject = new RealSubject();

        // 我们要代理哪个真实对象，就将该对象传进去，最后是通过该真实对象来调用其方法的
        InvocationHandler handler = new DynamicProxy(realSubject);

        /*
         * 通过Proxy的newProxyInstance方法来创建我们的代理对象，我们来看看其三个参数
         * 第一个参数 handler.getClass().getClassLoader() ，我们这里使用handler这个类的ClassLoader对象来加载我们的代理对象
         * 第二个参数realSubject.getClass().getInterfaces()，我们这里为代理对象提供的接口是真实对象所实行的接口，表示我要代理的是该真实对象，这样我就能调用这组接口中的方法了
         * 第三个参数handler， 我们这里将这个代理对象关联到了上方的 InvocationHandler 这个对象上
         */
        Subject subject = (Subject)Proxy.newProxyInstance(handler.getClass().getClassLoader(), realSubject
                .getClass().getInterfaces(), handler);

        System.out.println(subject.getClass().getName());

        subject.rent();
        subject.hello("world");
    }
}
```

### SIP协议

SIP是一个应用层的信令控制协议。用于创建、修改和释放一个或多个参与者的会话

- **用户代理：** UA 如用于创建和管理 SIP 会话的移动电话、多媒体手持设备、PC、PDA 等。用户代理客户机发出消息。用户代理服务器对消息进行响应。
- **注册服务器：** SIP 注册服务器是包含域中所有用户代理的位置的数据库。在 SIP 通信中，这些服务器会检索出对方的 IP 地址和其他相关信息，并将其发送到 SIP 代理服务器。
- **代理服务器：** SIP 代理服务器接受 SIP UA 的会话请求并查询 SIP 注册服务器，获取收件方 UA 的地址信息。然后，它将会话邀请信息直接转发给收件方 UA（如果它位于同一域中）或代理服务器（如果 UA 位于另一域中）。
- **重定向服务器：** SIP 重定向服务器允许 SIP 代理服务器将 SIP 会话邀请信息定向到外部域。SIP 重定向服务器可以与 SIP 注册服务器和 SIP 代理服务器同在一个硬件上。

注册阶段：

- 用户首次试呼时，终端代理A 向代理服务器发送REGISTER 注册请求；
- 代理服务器通过后端认证中心获知用户信息不在数据库中，便向终端代理回送401Unauthorized 质询信息，其中包含安全认证所需的令牌；
- 终端代理提示用户输入其标识和密码后，根据安全认证令牌将其加密后，再次用REGISTER 消息报告给代理服务器；
- 代理服务器将REGISTER 消息中的用户信息解密，通过认证/计费中心验证其合法后，将该用户信息登记到数据库中，并向终端代理A 返回成功响应消息200 OK。

呼叫建立阶段：

- 发送到代理服务器的INVITE请求负责启动会话。
- 代理服务器发送100 尝试立即响应呼叫者（Alice）以停止INVITE请求的重新发送。
- 代理服务器在位置服务器中搜索Bob的地址。获取地址后，进一步转发INVITE请求。
- 此后，Bob手机生成的180 振铃（临时响应）返回给爱丽丝。
- 鲍勃拿起手机后一个200 OK响应很快产生。
- 一旦200 OK到达Alice，Bob 从Alice 收到一个ACK。
- 同时，会话建立，RTP数据包（会话）从两端开始流动。
- 会话结束后，任何参与者（Alice或Bob）都可以发送一个BYE请求来终止会话。
- BYE直接从Alice到Bob绕过代理服务器。
- 最后，Bob发送200 OK响应来确认BYE，会话终止。

### Web请求

- DHCP配置主机信息：假设主机最开始没有 IP 地址以及其它信息，那么就需要先使用 DHCP 来获取。
- ARP解析MAC地址：主机通过浏览器生成一个 TCP 套接字，套接字向 HTTP 服务器发送 HTTP 请求。为了生成该套接字，主机需要知道网站的域名对应的 IP 地址。
- DNS解析域名：知道了网关路由器的 MAC 地址之后，就可以继续 DNS 的解析过程了。
- HTTP请求页面：有了 HTTP 服务器的 IP 地址之后，主机就能够生成 TCP 套接字，该套接字将用于向 Web 服务器发送 HTTP GET 报文。

### 数字认证

进行 HTTPS 通信时，服务器会把证书发送给客户端。客户端取得其中的公开密钥之后，先使用数字签名进行验证，如果验证通过，就可以开始通信了。

- 服务器端B，计算数据Hash值，将Hash值用B的私钥加密，得到摘要，将摘要，证书，和数据一同发送给客户端
- 客户端A收到服务器端B发来的摘要，证书和数据之后，先从证书中拿到服务器B的公钥，解密摘要，得到一个Hash值1，然后对数据进行Hash，得到Hash值2，比较Hash值1和2，如果相同则证明证书有效，双方开始通信。

在这个过程中，服务器端B是签名者，进行了数字签名。
