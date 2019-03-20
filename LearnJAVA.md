<!-- TOC -->

- [JAVA并发编程](#java并发编程)
    - [缓存一致性](#缓存一致性)
    - [原子性、可见性和有序性](#原子性可见性和有序性)
        - [1.原子性](#1原子性)
        - [2.可见性](#2可见性)
        - [3.有序性](#3有序性)
    - [先行发生原则](#先行发生原则)
    - [Java语言中的线程安全](#java语言中的线程安全)
        - [线程安全的实现方法](#线程安全的实现方法)
            - [1.互斥同步 (阻塞同步)](#1互斥同步-阻塞同步)
            - [2.非阻塞同步](#2非阻塞同步)
            - [3.无同步方案](#3无同步方案)
    - [volatile 的实现原理应用](#volatile-的实现原理应用)
    - [synchronized 的实现原理和应用](#synchronized-的实现原理和应用)
    - [锁优化](#锁优化)
        - [自旋锁，锁消除，锁粗化](#自旋锁锁消除锁粗化)
        - [1.偏向锁](#1偏向锁)
        - [2.轻量级锁](#2轻量级锁)
            - [轻量级锁加锁](#轻量级锁加锁)
            - [轻量级锁解锁](#轻量级锁解锁)
            - [中断与synchronized](#中断与synchronized)
            - [等待唤醒机制与synchronized](#等待唤醒机制与synchronized)
    - [等待/通知机制](#等待通知机制)
        - [等待/通知的经典范式](#等待通知的经典范式)
        - [等待超时模式](#等待超时模式)
    - [Java中的锁](#java中的锁)
        - [队列同步器 AbstractQueuedSynchronizer](#队列同步器-abstractqueuedsynchronizer)
            - [独占式同步状态的获取和释放](#独占式同步状态的获取和释放)
            - [共享式同步状态的获取和释放](#共享式同步状态的获取和释放)
            - [独占式超时获取同步状态](#独占式超时获取同步状态)
        - [具体实现](#具体实现)
        - [重入锁 ReentrantLock](#重入锁-reentrantlock)
        - [读写锁 ReentrantReadWriteLock](#读写锁-reentrantreadwritelock)
        - [Condition 接口](#condition-接口)
    - [Java 并发工具类](#java-并发工具类)
        - [CountDownLatch](#countdownlatch)
        - [CyclicBarrier (循环屏障)](#cyclicbarrier-循环屏障)
        - [Semaphore 信号量](#semaphore-信号量)
        - [Exchanger](#exchanger)
    - [Java并发容器和框架](#java并发容器和框架)
        - [ConcurrentHashMap](#concurrenthashmap)
        - [ConcurrentLinkedQueue](#concurrentlinkedqueue)
        - [阻塞队列](#阻塞队列)
            - [阻塞队列的实现原理](#阻塞队列的实现原理)
    - [Java线程池](#java线程池)
    - [Executor 框架](#executor-框架)
        - [FutureTask](#futuretask)
        - [Java join()方法](#java-join方法)
        - [单例模式](#单例模式)
        - [抽象工厂模式](#抽象工厂模式)
- [Java 基础](#java-基础)
    - [垃圾回收机制](#垃圾回收机制)
    - [类加载机制](#类加载机制)
        - [加载](#加载)
        - [验证](#验证)
        - [准备](#准备)
        - [解析](#解析)
        - [初始化](#初始化)
    - [类和类加载器](#类和类加载器)
    - [对象的创建，对象的内存布局，对象的访问定位](#对象的创建对象的内存布局对象的访问定位)
    - [Java NIO](#java-nio)
    - [Spring](#spring)
        - [Spring MVC 流程](#spring-mvc-流程)

<!-- /TOC -->

# JAVA并发编程

## 缓存一致性

基于高速缓存的存储交互很好的解决了处理器和内存的速度矛盾，但是也为计算机带来了更高的复杂度，因为它引入了一个新的问题：缓存一致性

为了解决缓存一致性问题，需要各处理器访问缓存时遵守一些协议，如 MSI、MESI

这里会涉及到一个概念：**内存模型**，即 在特定的操作协议下，对特定的内存或高速缓存进行读写访问的过程抽象，Java虚拟机也有自己的内存模型

除了增加高速缓存，为了使得处理器内部的运算单元能尽量被充分使用，处理器可能会对输入代码进行乱序执行优化，与处理器的乱序执行优化类似，Java虚拟机的即时编译器也有类似的 指令重排序优化

Java 虚拟机规范试图定义一种Java内存模型来屏蔽掉各种硬件和操作系统的内存访问差异，让Java程序在各个平台下都能达到一致的内存访问效果，C/C++ 直接使用的是物理硬件和操作系统的内存模型，所以在不同平台上由于内存模型的差异，导致程序运行结果的差异

Java内存模型

Java内存模型 | Java虚拟机 | 操作系统
| --- | --- | --- |
主内存 | Java堆中的对象实例数据部分 | 物理硬件的内存
工作内存 | 虚拟机栈的部分区域 | 寄存器或高速缓存

上下文切换：任务从保存到再加载的过程就是依次上下文切换，vmstat查看上下文切换次数

减少上下文切换的方法：

- 无锁并发编程：数据ID按照Hash取模分段，不同的线程处理不同端的数据
- CAS算法：利用CPU的CAS指令，同时借助JNI来完成Java的非阻塞算法。
- 使用最少的线程：线程池的概念
- 协程：单线程里实现多任务调度，并在单线程里维持多个任务间切换

## 原子性、可见性和有序性

### 1.原子性

Java 内存模型保证了 read、load、use、assign、store、write、lock 和 unlock 操作具有原子性，基本数据类型的访问读写是具有原子性的，但是对 int 类型读写操作满足原子性只是说明 load、assign、store 这些单个操作具备原子性，这一系列操作整体上看不具备原子性。

可以使用原子类，synchronized 解决，volatile不保证原子性

### 2.可见性

可见性即当一个线程修改了共享变量的值，其他线程能够立即得知这个修改

java 内存模型通过 在变量修改后将新值同步回主内存，在变量读取前从主内存刷新变量值这种依赖主内存作为传输媒介的方式实现可见性

区别于普通变量，volatile 的特殊规则保证了新值能立即同步到主内存，以及每次使用前立即从主内存刷新

除了 volatile，synchronized（unlock规则：执行 unlock 操作之前，必须把变量值同步回主内存） 和 final 也实现了可见性

### 3.有序性

如果在本线程内观察，所有的操作都是有序的，如果在一个线程中观察另外一个线程，所有的操作都是无序的，指令重排序不会印象不会影响到单线程程序的执行，却会影响到多线程并发执行的正确性。

volatile 关键字通过添加内存屏障的方式来禁止指令重排，即重排序时不能把后面的指令放到内存屏障之前。

也可以通过 synchronized 来保证有序性，它保证每个时刻只有一个线程执行同步代码，相当于是让线程顺序执行同步代码

## 先行发生原则

上面提到了可以用 volatile 和 synchronized 来保证有序性。除此之外，JVM 还规定了先行发生原则，让一个操作无需控制就能先于另一个操作完成。

- 程序次序规则：在一个线程内，在程序前面的操作先行发生于后面的操作。
- 管程锁定规则：一个 unlock 操作先行发生于后面对同一个锁的 lock 操作。
- volatile 变量规则：对一个 volatile 变量的写操作先行发生于后面对这个变量的读操作。
- 线程启动规则：Thread 对象的 start() 方法调用先行发生于此线程的每一个动作。
- 线程加入规则：Thread 对象的结束先行发生于 join() 方法返回。
- 线程中断规则：对线程 interrupt() 方法的调用先行发生于被中断线程的代码检测到中断事件的发生，可以通过 interrupted() 方法检测到是否有中断发生。
- 对象终结规则：一个对象的初始化完成（构造函数执行结束）先行发生于它的 finalize() 方法的开始。
- 传递性：如果操作 A 先行发生于操作 B，操作 B 先行发生于操作 C，那么操作 A 先行发生于操作 C。

时间上先发生 和 先行发生 不能相互推导

我们衡量并发安全问题不要受时间顺序的干扰一切必须以先行发生原则为准

## Java语言中的线程安全

- **1.不可变**
  - 共享数据是基本数据类型，final 修饰就可以保证它是不可变的，如果共享数据是一个对象，那就需要保证对象的行为不会对其状态产生任何影响才行，还包括：String，枚举型，Number部分子类 Long，Double，BigInterger
- **2.绝对线程安全**
  - 不管运行时环境如何，调用者都不需要任何额外的同步措施
- **3.相对线程安全**
  - 保证对这个对象单独的操作是安全的，在调用时不需要做额外的保护措施，但对于特定顺序的连续调用，就可能需要在调用端使用额外的同步手段，比如：Vector，HashTable
- **4.线程兼容**
  - 对象本身并不是线程安全的，但是可以通过调用端正确地使用同步手段来保证对象在并法环境中可以安全地使用 Java API中大部分都属于线程兼容，比如：ArrayList，HashMap
- **5.线程对立**
  - 无论调用端是否采取了同步措施，都无法在多线程环境中并发使用的代码

### 线程安全的实现方法

#### 1.互斥同步 (阻塞同步)

互斥同步最主要的问题就是进行线程阻塞和唤醒所带来的问题，属于一种悲观的并发策略

互斥是方法，同步是目的，互斥的手段包括：临界区，互斥量和信号量

- 临界区：保证在某一时刻只有一个线程能访问数据的简便办法，在任意时刻只允许一个线程对共享资源进行访问
- 互斥量：互斥量跟临界区很相似，只有拥有互斥对象的线程才具有访问资源的权限，由于互斥对象只有一个，因此就决定了任何情况下此共享资源都不会同时被多个线程所访问
- 信号量：信号量对象对线程的同步方式与前面几种方法不同，信号允许多个线程同时使用共享资源，这与操作系统中的PV操作相同。它指出了同时访问共享资源的线程最大数目
- 事件：事件对象也可以通过通知操作的方式来保持线程的同步。并且可以实现不同进程中的线程同步操作

Java 中最基本的互斥同步手段就是 synchronized 关键字，除了 synchronized 之外，我们还可以使用 java.util.concurrent 包中的重入锁来实现同步

相比 synchronized，ReentrantLock 增加了一些高级功能

- 等待可中断
- 可实现公平锁
  - 多个线程在等待同一个锁，必须按照申请锁的时间顺序来依次获取锁
- 锁可以绑定多个条件 Condition

#### 2.非阻塞同步

使用基于冲突检测的乐观并发策略：先进行操作，如果没有其它线程争用共享数据，那操作就成功了，否则采取补偿措施（不断地重试，直到成功为止），这种乐观的并发策略的许多实现都不需要将线程阻塞，因此这种同步操作称为非阻塞同步。

这就需要操作和冲突检测这两个步骤具有原子性

- Test-and-Set
- Fetch-and-Increment
- Swap
- Compare-and-Swap CAS
- Load-Linked / Store-Conditional

乐观锁基本上都是通过CAS实现的，CAS 三个操作数：内存位置V，旧的预期值A，新值B，只有V符合预期A时处理器采用B去更新V的值，否则就不执行更新，无论是否更新，返回V的旧值

CAS无法覆盖互斥同步的所有使用场景

CAS 的三个问题

- ABA问题，JUC包使用 `AtomicStampedReference` 控制变量版本解决，改用传统的互斥同步方法可能会更高效
- 循环时间长开销大
- 只能保证一个共享变量的原子操作

```java
// AtomicInteger 中的实现方式：
private volatile int value;

public final int get() {
    return value;
}

public final int getAndUpdate(int x) {
    int prev, next;
    do {
        prev = get();
        next = x;
    } while (!compareAndSet(prev, next));
    return prev;
}
```

#### 3.无同步方案

同步只是保证共享数据争用时的正确性的手段，如果一个方法本来就不涉及共享数据，那他就自然无需任何同步措施保证正确性，例如：

- 可重入代码
  - 可以在代码执行的任何时刻中断它，转而去执行另外一段代码，方法返回不会出现任何错误。
  - 方法返回结果是可以预测的，只要输入了相同的数据就能放回相同的结果，那他就满足可重入性
  - 可重入代码有一些共同的特征，例如不依赖存储在堆上的数据和公用的系统资源、用到的状态量都由参数中传入、不调用非可重入的方法等。
- 线程本地存储
  - java.lang.ThreadLocal 类

ThreadLocal

- 在每个线程 Thread 内部有一个 `ThreadLocal.ThreadLocalMap` 类型的成员变量threadLocals，这个threadLocals就是用来存储实际的变量副本的，键值为当前ThreadLocal变量，value为变量副本（即T类型的变量）
- 初始时，在线程里面，threadLocals为空，当通过 `ThreadLocal` 变量调用get()方法或者set()方法，就会对Thread类中的threadLocals进行初始化，并且以当前ThreadLocal变量为键值，以ThreadLocal要保存的副本变量为value，存到threadLocals
- 然后在当前线程里面，如果要使用副本变量，就可以通过get方法在threadLocals里面查找

- set方法：向获取当前线程并拿到线程私有的 ThreadLocalMap，将调用该方法的 ThreadLocal 对象作为键，要保存的复本变量作为 value 存入 Map
- get方法：先获取当前线程并拿到线程私有的 ThreadLocalMap，将调用该方法的 ThreadLocal 对象作为键 从 Map 中取出

这个Map 没有使用拉链法解决 Hash 冲突，而是增加一个固定的大小

```java
public T get() {
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null) {
        ThreadLocalMap.Entry e = map.getEntry(this);
        if (e != null) {
            @SuppressWarnings("unchecked")
            T result = (T)e.value;
            return result;
        }
    }
    return setInitialValue();
}

public void set(T value) {
    Thread t = Thread.currentThread();
    ThreadLocalMap map = getMap(t);
    if (map != null)
        map.set(this, value);
    else
        createMap(t, value);
}
```

## volatile 的实现原理应用

volatile 的语义：

- 保证此变量对所有线程的可见性，即当一个线程修改了这个变量的值，新值对于其他线程来说是立即得知的
- 禁止指令重排序优化

volatile 变量修饰的共享变量进行写操作会多出 `lock add1 $0x0 (%esp)` 操作，这个操作相当于一个内存屏障

lock 前缀的指令在多核处理器下会引发：

- 将当前处理器缓存行的数据写回到系统内存
- 这个写回内存的操作会使在其他CPU里缓存了该内存地址的数据无效

这种操作相当于对 Cache 中的变量做了一个Java内存模型中的 store 和 write 操作，所以通过这一个空操作，可以让 volatile 变量的修改对其他CPU 立即可见

volatile 变量只能保证可见性，在不符合以下两条规则的运算场景下，我们仍需要通过加锁来保证原子性

- 运算结果并不依赖变量的当前值，或者能够确保只有单一的线程修改变量值
- 变量不需要与其他状态变量共同参与 不变约束

```java
volatile boolean shutdownRequested;

public void shutdown() {
    shutdownRequested = true;
}

public void doWork() {
    while (!shutdownRequested) {
        // do stuff
    }
}
```

volatile 的两条实现原则

- Lock前缀指令会引起处理器缓存回写到内存
  - Lock# 信号一般不锁总线，而是锁缓存
- 一个处理器的缓存回写到内存会导致其他处理器的缓存无效
  - 处理器使用MESI控制协议去维护内部缓存和其他处理器缓存的一致性
  - 处理器使用嗅探技术保证它的内部缓存、系统内存和其他处理器的缓存的数据在总线上保持一致

volatile 的优化

- 在 `LinkedTransferQueue` 中，追加字节到64字节填满高速缓冲区的缓存行，避免头节点和尾节点加载到同一缓存行，使头尾节点在修改时不会互相锁定

以下场景不适宜用这种方式：

- 缓冲行非64字节宽的处理器
- 共享变量不会被频繁的写

## synchronized 的实现原理和应用

JAVA SE 1.6 为了减少获取锁和释放锁带来的性能消耗引入了 偏向锁和轻量级锁，以及锁的存储结构和升级过程

synchronized 实现同步的基础：Java中的每一个对象都可以作为锁

- 对于普通同步方法，锁是当前实例对象
- 对于静态同步方法，锁是当前类的class对象
- 对于同步方法块，锁是synchronized括号里配置的对象

JVM基于进入和退出 Monitor对象（管程，监视器锁）实现方法同步和代码块同步

synchronized 修饰代码块的实现使用的是 monitorenter 和 monitorexit 指令，synchronized修饰的方法使用的是 ACC_SYNCHRONIZED 标识

monitorenter 和 monitorexit 指令，对于这两条指令需要注意：

- synchronized 同步块对同一条线程来说是可重入的不会出现自己把自己锁死
- Java 线程映射到操作系统的原生线程之上，如果要阻塞或唤醒一个线程，都需要从用户态转移到核心态

synchronized 用的锁存在 Java 对象头里的

在 JAVA SE 1.6 中，锁一共有四种状态，从低到高分别是：无锁状态、偏向锁状态、轻量级锁状态和重量级锁状态

锁可以升级但是不能降级

## 锁优化

### 自旋锁，锁消除，锁粗化

自旋锁：互斥同步进入阻塞状态的开销都很大，应该尽量避免。在许多应用中，共享数据的锁定状态只会持续很短的一段时间。自旋锁的思想是让一个线程在请求一个共享数据的锁时执行忙循环（自旋）一段时间，如果在这段时间内能获得锁，就可以避免进入阻塞状态。

锁消除：锁消除是指对于被检测出不可能存在竞争的共享数据的锁进行消除。

锁粗化：如果一系列的连续操作都对同一个对象反复加锁和解锁，频繁的加锁操作就会导致性能损耗。如果虚拟机探测到由这样的一串零碎的操作都对同一个对象加锁，将会把加锁的范围扩展（粗化）到整个操作序列的外部

### 1.偏向锁

大多数情况下，锁不仅不存在多线程竞争，而且总是由同一线程多次获得，为了让线程获取锁的代价更低从而引入了偏向锁

当一个线程访问同步块并获得锁，会在对象头和栈帧的锁记录里存储锁偏向的线程ID，以后该线程在进入和退出同步块就不需要进行CAS操作来加锁和解锁，只是简单测试对象头里的 `Mark Word` 里是否存储着指向当前线程的偏向锁

- 如果测试成功，表示线程已经获取了偏向锁
- 如果失败
  - 需要在测试以下 `Mark Word` 中偏向锁是否设置成1
  - 如果没有设置则使用CAS竞争锁
  - 如果设置了则尝试使用CAS将对象头的偏向锁指向当前线程

偏向锁使用了一种等到竞争出现才释放锁的机制，所以当其他线程尝试竞争偏向锁时，持有偏向锁的线程才会释放锁

### 2.轻量级锁

#### 轻量级锁加锁

线程在执行同步块之前，JVM会在当前线程的栈帧中创建用于存储锁记录的空间，并将对象头中的 `Mark Word` 复制到锁记录中 `Displaced Mark Word`，然后线程尝试**使用CAS**将对象头中的 `Mark Word` 替换为指向锁记录的指针

- 如果成功，当前线程获得锁 此时锁记录为原始的 `Mark Word`，对象头中的 `Mark Word` 为锁记录指针
- 如果失败，表示其他线程竞争锁，当前线程便尝试使用自旋锁获得锁

#### 轻量级锁解锁

解锁时，会**使用CAS**操作将 `Displaced Mark Word` 替换回到对象头

- 如果成功，则表示没有竞争发生 对象头中的 `Mark Word` 为原始值
- 如果失败，则表示当前锁存在竞争，锁就会膨胀成重量级锁

因为自旋会消耗CPU，为了避免无用的自旋，一旦升级成重量级锁，就不会再恢复到轻量级状态，当锁处于这个状态，其他线程试图获取锁都会被阻塞，当持有锁的线程释放锁之后才会唤醒这些线程

有竞争的情况下，轻量级锁会比传统的重量级锁更慢，因为有额外的CAS操作

轻量级锁能够提升程序同步性能的依据是：**对于绝大部分的锁，在整个同步周期内都是不存在竞争的**，轻量级锁所适应的场景是线程交替执行同步块的场合

#### 中断与synchronized

线程的中断操作对于正在等待获取的锁对象的 synchronized 方法或者代码块并不起作用，也就是对于 synchronized 来说，如果一个线程在等待锁，那么结果只有两种，要么它获得这把锁继续执行，要么它就保存等待，即使调用中断线程的方法，也不会生效

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

#### 等待唤醒机制与synchronized

等待唤醒机制本篇主要指的是 `notify`，`notifyAll()` 和 `wait` 方法，在使用这3个方法时，必须处于synchronized代码块或者synchronized方法中，否则就会抛出IllegalMonitorStateException异常

这是因为调用这几个方法前必须拿到当前对象的监视器monitor对象，也就是说notify/notifyAll和wait方法依赖于monitor对象，在前面的分析中，我们知道monitor 存在于对象头的Mark Word 中(存储monitor引用指针)，而 synchronized 关键字可以获取 monitor ，这也就是为什么notify/notifyAll和wait方法必须在synchronized代码块或者synchronized方法调用的原因。

```java
synchronized (obj) {
    obj.wait();
    obj.notify();
    obj.notifyAll();
}
```

需要特别理解的一点是，与sleep方法不同的是wait方法调用完成后，线程将被暂停，但wait方法将会释放当前持有的监视器锁(monitor)，直到有线程调用notify/notifyAll方法后方能继续执行，而sleep方法只让线程休眠并不释放锁。同时notify/notifyAll方法调用后，并不会马上释放监视器锁，而是在相应的 `synchronized(){}块`， `synchronized方法` 执行结束后才自动释放锁。

## 等待/通知机制

等待通知机制是指一个线程A调用了对象O的 `wait()` 方法进入等待状态，而另一个线程B调用了对象O的 `notify(),notifyAll()` 方法,线程A收到通知后从对象O的 `wait()` 方法返回，进而执行后续操作

```java
public class WaitNotify {
    private static boolean flag = false;
    private static final Object lock = new Object();

    public static void main(String[] args) throws Exception {
        Thread waitThread = new Thread(new Wait(), "waitThread");
        waitThread.start();
        TimeUnit.SECONDS.sleep(5);
        Thread notifyThread = new Thread(new Notify(), "notifyThread");
        notifyThread.start();
    }

    static class Wait implements Runnable {
        @Override
        public void run() {
            // 加锁，拥有lock的Monitor
            synchronized (lock) {
                // 当条件不满足继续wait，同时释放了lock的锁
                while (flag) {
                    try {
                        System.out.println(Thread.currentThread() + "flag is true");
                        lock.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                // 条件满足时，完成工作
                System.out.println(Thread.currentThread() + "flag is false");
            }
        }
    }

    static class Notify implements Runnable {
        @Override
        public void run() {
            // 加锁，拥有lock的Monitor
            synchronized (lock) {
                // 获取lock的锁，然后进行通知，通知时不会释放锁
                // 直到当前线程释放了lock后，WaitThread才能从wait方法中返回
                try {
                    System.out.println(Thread.currentThread() + "hold lock");
                    lock.notifyAll();
                    flag = false;
                    TimeUnit.SECONDS.sleep(5);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            synchronized (lock) {
                System.out.println(Thread.currentThread() + "hold lock again");
                try {
                    TimeUnit.SECONDS.sleep(5);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
```

`WaitThread` 首先获取了对象的锁，然后调用对象的 `wait()` 方法，从而放弃锁进入了对象的 `等待队列 WaitQueue` 中，进入 `Waiting` 状态。

由于 `WaitThread` 释放了对象的锁，`NotifyThread` 随后获取了对象的锁，并调用对象的 `notify()` 方法，将 `WaitThread` 从`WaitQueue` 移到 `SynchronizedQueue` 中，此时 `WaitThread` 的状态变成了 `Blocking` 状态，`NotifyThread` 释放了锁之后，`WaitThread` 再次获取到锁并从 `wait()` 方法返回继续执行

### 等待/通知的经典范式

等待方：

- 获取对象的锁
- 如果条件不满足那么调用对象的wait方法，被通知后仍要检查条件
- 条件满足则执行对应的逻辑

```java
synchronized(对象) {
    while (条件不满足) {
        对象.wait();
    }
    对应的处理逻辑
}
```

通知方：

- 获取对象的锁
- 改变条件
- 通知所有等待在对象上的线程

```java
synchronized(对象) {
    改变条件
    对象.notifyAll();
}
```

### 等待超时模式

```java
public synchronized Object get(long mills) throws InterruptedException {
    long future = System.currentTimeMills() + mills;
    long remaining = mills;

    while ((result == null) && remaining > 0) {
        wait(remaining);
        remaining = future - System.currentTimeMills();
    }

    return result;
}

```

## Java中的锁

Java SE 5之后，并发包中新增了 Lock接口，虽然它缺少了 synchronized 块或者方法提供的隐式获取释放锁的便捷性，但是却拥有了锁获取与释放的可操作性，可中断的获得锁以及超时获得锁等同步特性

```java
Lock lock = new ReentrantLock();
lock.lock();
try {

} finally {
    lock.unlock();
}
```

在 finally 块中释放锁，目的是保证在获得锁之后最终能够被释放

不要将获取锁的过程写在 try 块中，因为如果在获取锁时发生了异常，异常抛出的同时，也会导致锁的无故释放

Lock接口提供了 synchronized 关键字不具备的主要特性

- 尝试非阻塞的获取锁：当前线程尝试获取锁，如果这一时刻没有被其他线程获取到，则成功获取并持有锁
- 能被中断的获取锁：获取锁的线程能够相应中断
- 超时获取锁：在指定的截至时间之前获取锁，如果截至时间到了仍旧无法获取锁则返回

Lock接口的实现基本上都是通过聚合一个同步器的子类来完成线程的访问控制

### 队列同步器 AbstractQueuedSynchronizer

队列同步器是用来构建锁或者其他同步组件的基础框架，它使用一个int成员变量表示同步状态，通过内置的FIFO队列来完成资源获取线程的排队工作

在抽象方法的实现过程中免不了要对同步状态进行更改，这时就需要使用同步器提供的3个方法：`getState()`，`setState(int newState)`，`compareAndSetState(int expect, int update)`

同步器的主要使用方式是继承，子类被推荐定义为一个自定义同步组件的静态内部类

同步器是实现锁的关键，在锁的实现中聚合同步器，利用同步器实现锁的语义

锁和同步器的关系：

- 锁是面向使用者的，它定义了使用者与锁交互的接口，隐藏了实现细节
- 同步器是面向锁的实现者，它简化了锁的实现方式，屏蔽了同步状态管理、线程的排队、等待和唤醒等底层操作

同步器提供的**模板方法**包括三类：

- 独占式获取和释放同步状态
- 共享时获取和释放同步状态
- 查询同步队列中的等待线程情况

同步器依赖内部的同步队列来完成同步状态的管理，当前线程获取同步状态失败时，同步其将当前线程以及等待状态等信息构造成一个节点并加入到同步队列中，同时阻塞当前线程，当同步状态释放时，会把首节点的线程唤醒，使其再次尝试获取同步状态

同步队列中的节点用来保存获取同步状态失败的线程引用、等待状态以及前驱和后继节点

- 向同步队列中添加尾节点这个过程需要保证线程安全，因此同步提供了一种CAS的设置方法 `compareAndSetTail(Node expect, Node update)`
- 设置首节点是通过获取同步状态成功的节点完成的，由于只有一个线程能够成功获取同步状态，因此设置首节点的方法不需要使用CAS

**共享式获取和独占式获取最主要的区别在于同一时间能否有多个线程诶能够同时获取到同步状态**

#### 独占式同步状态的获取和释放

- 独占式获取 `acquire(int arg)`：同步状态如果成功则返回，失败进入同步队列等待，添加尾节点这个过程需要保证线程安全，因此同步提供了一种CAS的设置方法
- 独占式释放 `release(int arg)`：该方法会在释放同步状态之后，唤醒头节点(**同步队列的首节点拥有同步状态**)的后继节点

#### 共享式同步状态的获取和释放

- 共享式获取 `acquireShared(int arg)`：如果当前线程未获取到同步状态将会进入同步队列等待，与独占式获取的主要区别是在同一时刻可以有多个线程获取到同步状态
- 共享式释放 `releaseShared(int arg)`：因为释放同步状态的操作会同时来自多个线程，所以该方法必须确保同步状态(或者资源数)线程安全释放，一般通过循环和CAS保证

#### 独占式超时获取同步状态

响应中断的同步状态获取状态 `acquireInterruptibly(int arg)`：在Java 5之前，当一个线程获取不到锁而被阻塞在synchronized 之外时，对该线程进行中断操作，该线程的中断标志位会被修改，但线程仍然会被阻塞在 synchronized之外，等待获取锁

在Java 5之中，同步器提供了 `acquireInterruptibly(int arg)` 方法，这个方法在等待获取同步状态时，如果当前线程被中断，会立即返回并抛出 `InterruptedException`

独占式超时获取同步状态 `boolean tryAcquireNanos(int arg, long nanos)`：在 `acquireInterruptibly(int arg)` 基础上加了超时限制，如果当前线程在超时时间内没有获到同步状态则返回false，获取到了返回 true

### 具体实现

### 重入锁 ReentrantLock

公平的锁机制往往没有非公平的效率高，但是并不是任何场景都是以TPS作为唯一的指标，公平锁能够减少饥饿发生的概率，等待越久的请求越是能够得到优先满足

1.实现重进入

需要解决两个问题：

- 线程再次获取锁：锁需要去识别获取所得线程是否为当前占据锁的线程，如果是再次获取成功
- 锁的最终释放：获取锁要求对锁计数自增，而锁被释放时计数自减，当计数等于0时表示锁已经成功释放

2.公平和非公平获取锁的区别

多了同步队列中当前节点是否有前驱节点的判断，如果返回true，则表示有线程比当前节点更早地请求获取锁，因此需要等待前驱线程获取并释放之后才能获取锁

公平锁保证了锁获取按照FIFO原则，代价是进行大量的线程切换，非公平锁虽然可能造成线程饥饿，但极少的线程切换保证了更大的吞吐量

### 读写锁 ReentrantReadWriteLock

读写锁维护了一个读锁和一个写锁，允许同一时刻有多个读线程访问，但是写线程访问时，所有的读线程和其他写线程被阻塞

- 读操作获取读锁，写操作获取写锁
- 读写锁比其他排他锁好，因为大多数场景读多于写的，读写锁能够比其他排他锁有更好的并发性和吞吐量

1. 读写状态的设计：一个整型变量上维持两个状态，高16位表示读，低16位表示写
2. 写锁是支持重进入的排他锁，读锁是支持重进入的共享锁
3. 锁降级指写锁降级为读锁：把持当前拥有的写锁，再获取到读锁，随后释放先前的写锁的过程
4. 锁降级中的读锁是必须的，主要是为了保持可见性（非内存可见性），如果不获取读锁而是直接释放写锁，假使此时另一个线程T获取了写锁并修改了数据，那么当前线程无法感知线程T的数据修改，即当前线程并不知道其他线程已经修改了数据
5. 不支持升级（把持住读锁，获取写锁最后释放读锁），也是为了保证数据可见性，如果读锁已经被多个线程获取，其中任意线程成功获取了写锁并更新了数据，对其更新其他获取到读锁的线程是不可见的，即其他线程并不知道有线程对数据进行了更新

LockSupport提供park()和unpark()方法实现阻塞线程和解除线程阻塞

### Condition 接口

任意一个Java对象都拥有一组监视器方法，`wait()` `wait(long timeout)` `notify()` `notifyAll()`

Condition 接口提供了类似Object的监视器方法，和Lock配合也可以实现等待通知方式

Condition 接口中有多个等待队列，监视器方法中只有一个等待队列，并且不支持当前线程释放锁并进入等待队列状态，在等待状态中不响应中断

ConditionObject 实现了 Condition 接口，作为 同步器的 AbstractQueuedSynchronizer 的内部类

```java
Lock lock = new ReentrantLock();
Condition condition = lock.newCondition();

public void conditionWait() throws InterruptedException {
    lock.lock();
    try {
        // 条件判断
        condition.await();
    } finally {
        lock.unlock();
    }
}

public void conditionSignal() throws InterruptedException {
    lock.lock();
    try {
        // 条件判断
        condition.signal();
    } finally {
        lock.unlock();
    }
}
```

## Java 并发工具类

### CountDownLatch

用来控制一个线程等待多个线程。维护了一个计数器 cnt，每次调用 countDown() 方法会让计数器的值减 1，减到 0 的时候，那些因为调用 await() 方法而在等待的线程就会被唤醒。

```java
public class CountdownLatchExample {

    public static void main(String[] args) throws InterruptedException {
        final int totalThread = 10;
        CountDownLatch countDownLatch = new CountDownLatch(totalThread);
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalThread; i++) {
            executorService.execute(() -> {
                System.out.print("run..");
                countDownLatch.countDown();
            });
        }
        countDownLatch.await();
        System.out.println("end");
        executorService.shutdown();
    }
}

//Output: run..run..run..run..run..run..run..run..run..run..end
```

### CyclicBarrier (循环屏障)

用来控制多个线程互相等待，只有当多个线程都到达时，**这些线程**才会继续执行。和 CountDownLatch 相似，都是通过维护计数器来实现的。线程执行 await() 方法之后计数器会减 1，并进行等待，直到计数器为 0，所有调用 await() 方法而在等待的线程才能继续执行。

CyclicBarrier 和 CountdownLatch 的一个区别是，CyclicBarrier 的计数器通过调用 reset() 方法可以循环使用，所以它才叫做循环屏障。

```java
// CyclicBarrier 有两个构造函数，其中 parties 指示计数器的初始值，barrierAction 在所有线程都到达屏障的时候会执行一次。
public CyclicBarrier(int parties, Runnable barrierAction) {
    if (parties <= 0) throw new IllegalArgumentException();
    this.parties = parties;
    this.count = parties;
    this.barrierCommand = barrierAction;
}

public CyclicBarrier(int parties) {
    this(parties, null);
}
```

```java
public class CyclicBarrierExample {

    public static void main(String[] args) {
        final int totalThread = 10;
        CyclicBarrier cyclicBarrier = new CyclicBarrier(totalThread);
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalThread; i++) {
            executorService.execute(() -> {
                System.out.print("before..");
                try {
                    cyclicBarrier.await();
                } catch (InterruptedException | BrokenBarrierException e) {
                    e.printStackTrace();
                }
                System.out.print("after..");
            });
        }
        executorService.shutdown();
    }
}
// Output:before..before..before..before..before..before..before..before..before..before..after..after..after..after..after..after..after..after..after..after..
```

### Semaphore 信号量

Semaphore 类似于操作系统中的信号量，可以控制对互斥资源的访问线程数。

```java
public class SemaphoreExample {

    public static void main(String[] args) {
        final int clientCount = 3;
        final int totalRequestCount = 10;
        Semaphore semaphore = new Semaphore(clientCount);
        ExecutorService executorService = Executors.newCachedThreadPool();
        for (int i = 0; i < totalRequestCount; i++) {
            executorService.execute(()->{
                try {
                    semaphore.acquire();
                    System.out.print(semaphore.availablePermits() + " ");
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } finally {
                    semaphore.release();
                }
            });
        }
        executorService.shutdown();
    }
}
// Output: 2 1 2 2 2 2 2 1 2 2
```

### Exchanger

Exchanger它可以在两个线程之间传输数据，Exchanger中的 `public V exchange(V x)` 方法被调用后等待另一个线程到达交换点（如果当前线程没有被中断），然后将已知的对象传给它，返回接收的对象。

## Java并发容器和框架

### ConcurrentHashMap

线程安全且高效的HashMap

ConcurrentHashMap的锁分段技术可有效提高并发访问率：容器中有多把锁，每一把锁用于锁容器其中一部分数据，那么当多线程访问容器里不同数据段的数据时，线程间不会存在锁竞争，从而可以有效提高并发访问效率，默认的并发级别为 16，也就是说默认创建 16 个 Segment。

ConcurrentHashMap get操作的高效之处在于整个get过程不需要加锁

- get操作：将需要使用的共享变量定义为 volatile，get方法不需要写共享变量，根据java内存模型的先行发生原则，对volatile字段的写入操作先于读操作，即使两个线程同时修改和获取volatile变量，get操作也能拿到最新的值，这是用volatile替换锁的经典应用场景。
- put操作：为了线程安全必须加锁，1.定位到Segment 2.在Segment里进行插入操作 插入操作需要经历两个步骤 1.是否需要扩容 2.定位添加元素的位置，然后将其放到HashEntry数组中
- size操作：先尝试2次通过不锁Segment的方式来统计各个Segment，如果统计的过程中，容器的count发生了变化（在统计size前后比较modCount是否发生变化），再采用加锁方式

ConcurrentHashMap 扩容优于HashMap是在插入元素之前判断是否需要扩容，HashMap是在插入元素后判断是否需要扩容，但是很有可能扩容之后没有新元素插入，这时HashMap就进行一次无效的扩容

ConcurrentHashMap 扩容的时候首先会创建一个两倍于原容量的数组，然后将原数组里的元素进行再hash后插入到新的数组里，为了高效ConcurrentHashMap不会对整个容器进行扩容，而只对某个segment进行扩容。

JDK 1.8 变化：

- put操作：table没有初始化则先初始化，没有hash冲突直接CAS插入，正在扩容则先扩容，如果hash冲突则加锁，链表数量大于8，转换为红黑树结构，添加成功，检查扩容

- JDK 1.7中采用ReentrantLock + Segment + HashEntry的方式进行实现，JDK 1.8 使用 synchronized + CAS + HashEntry + 红黑树，锁变更因为JVM的synchronized优化空间更大，使用内嵌的关键字比使用API更加自然
- JDK 1.7 使用分段锁机制来实现并发更新操作，核心类为 Segment，它继承自重入锁 ReentrantLock，并发度与 Segment 数量相等。
- JDK 1.8 使用了 CAS 操作来支持更高的并发度，在 CAS 操作失败时使用内置锁 synchronized。
- 并且 JDK 1.8 的实现也在链表过长时会转换为红黑树。

### ConcurrentLinkedQueue

ConcurrentLinkedQueue 是一个基于链接节点的非阻塞无界线程安全队列，它采用先进先出的规则对节点进行排序

### 阻塞队列

阻塞队列是支持两个附加操作的队列，这两个附加操作支持阻塞的插入和移除方法

- 支持阻塞的插入方法：意思是当队列满时，队列会阻塞插入元素的线程，直到队列不满
- 支持阻塞的移除方法：意识是在队列为空时，获取元素的线程会等待队列变为非空

阻塞队列常用于生产者消费者的场景，生产者向队列中添加元素的线程，消费者是从队列中取元素的线程，阻塞队列就是生产者用来存放元素，消费者用来获取元素的容器

插入和移除操作的四种方式：

- 抛出异常：add(e), remove(), element()
- 返回特殊值：offer(e), poll(), peek()
- 一直阻塞：put(e), take()
- 超时退出：offer(e, time, unit), poll(time, unit)

- ArrayBlockingQueue 数组结构组成的有界阻塞队列
- LinkedBlockingQueue 由链表结构组成的有界阻塞队列
- PriorityBlockingQueue 支持优先级的无界阻塞队列
- DelayQueue 支持延时获取元素的无界阻塞队列，即可以指定多久才能从队列中获取当前元素
- SynchronousQueue 不存储元素的阻塞队列，每一个put必须等待一个take操作，否则不能继续添加元素。
- LinkedTransferQueue 链表结构的无界阻塞TransferQueue队列，相比与其他阻塞队列多了 `tryTransfer` 和 `transfer` 方法
- LinkedBlockingDeque 链表结构的双向阻塞队列，优势在于多线程入队时，减少一半的竞争

```java
public class Producer implements Runnable {

    private BlockingQueue<Object> queue;
    Producer(BlockingQueue<Object> theQueue) {
        this.queue = theQueue;
    }

    @Override
    public void run() {
        try {
            Object justProduced = new Object();
            queue.put(justProduced);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

public class Consumer implements Runnable {

    private BlockingQueue<Object> queue;

    Consumer(BlockingQueue<Object> queue) {
        this.queue = queue;
    }

    @Override
    public void run() {
        try {
            Object toConsume = queue.take();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}

public static void main(String[] args) {
    BlockingQueue<Object> queue = new LinkedBlockingDeque<Object>(5);
    for (int i = 0; i < 5; i++) {
        new Thread(new Consumer(queue), "consumer: " + i).start();
        new Thread(new Producer(queue), "producer: " + i).start();
    }
}
```

#### 阻塞队列的实现原理

使用通知模式实现，通知模式：当生产者往满的队列中添加元素时会阻塞生产者，当消费者消费了一个队列中的元素后，会通知生产者当前队列可用

```java
public class ArrayBlockQueue<E> {
    private final Condition notFull;
    private final Condition notEmpty;
    final ReentrantLock lock;
    final Object[] items;

    int count;
    int takeIndex;
    int putIndex;

    public ArrayBlockQueue(int capacity, boolean fair) {
        if (capacity <= 0)
            throw new IllegalArgumentException();
        this.items = new Object[capacity];
        lock = new ReentrantLock(fair);
        notEmpty = lock.newCondition();
        notFull =  lock.newCondition();
    }

    public void put(E e) throws InterruptedException {
        if (e == null) {
            throw new NullPointerException();
        }
        final ReentrantLock lock = this.lock;
        lock.lockInterruptibly();
        try {
            while (count == items.length)
                notFull.await();
            enqueue(e);
        } finally {
            lock.unlock();
        }
    }

    private void enqueue(E x) {
        final Object[] items = this.items;
        items[putIndex] = x;
        if (++putIndex == items.length)
            putIndex = 0;
        count++;
        notEmpty.signal();
    }

    public E take() throws InterruptedException {
        final ReentrantLock lock = this.lock;
        lock.lockInterruptibly();
        try {
            while (count == 0)
                notEmpty.await();
            return dequeue();
        } finally {
            lock.unlock();
        }
    }

    private E dequeue() {
        final Object[] items = this.items;
        @SuppressWarnings("unchecked")
        E x = (E) items[takeIndex];
        items[takeIndex] = null;
        if (++takeIndex == items.length)
            takeIndex = 0;
        count--;
        notFull.signal();
        return x;
    }
}
```

## Java线程池

[Java并发编程：线程池的使用](https://www.cnblogs.com/dolphin0520/p/3932921.html)

- 降低资源消耗
- 提高响应速度
- 提高线程的可管理性

线程池处理任务：

- 使用者提交任务
- 核心线程池是否已满，未满创建线程执行任务
- 核心线程池满，任务队列是否已满，未满将任务添加存储在队列中
- 任务队列满，线程池是否已满，未满创建线程执行任务
- 线程池满，按照策略处理无法执行的任务

## Executor 框架

- Executor 和 ExecutorService 这两个接口主要的区别是：ExecutorService 接口继承了 Executor 接口，是 Executor 的子接口
- Executor 和 ExecutorService 第二个区别是：Executor 接口定义了 execute()方法用来接收一个Runnable接口的对象，而 ExecutorService 接口中的 submit()方法可以接受Runnable和Callable接口的对象。
- Executor 和 ExecutorService 接口第三个区别是 Executor 中的 execute() 方法不返回任何结果，而 ExecutorService 中的 submit()方法可以通过一个 Future 对象返回运算结果。
- Executor 和 ExecutorService 接口第四个区别是除了允许客户端提交一个任务，ExecutorService 还提供用来控制线程池的方法。比如：调用 shutDown() 方法终止线程池。
- Executors 类提供工厂方法用来创建不同类型的线程池。比如: newSingleThreadExecutor() 创建一个只有一个线程的线程池，newFixedThreadPool(int numOfThreads)来创建固定线程数的线程池，newCachedThreadPool()可以根据需要创建新的线程，但如果已有线程是空闲的会重用已有线程。

Executor 管理多个异步任务的执行，而无需程序员显式地管理线程的生命周期。这里的异步是指多个任务的执行互不干扰，不需要进行同步操作。

主要有三种 Executor：

- CachedThreadPool：一个任务创建一个线程；
- FixedThreadPool：所有任务只能使用固定大小的线程；
- SingleThreadExecutor：相当于大小为 1 的 FixedThreadPool。

```java
public static void main(String[] args) {
    ExecutorService executorService = Executors.newCachedThreadPool();
    for (int i = 0; i < 5; i++) {
        executorService.execute(new MyRunnable());
    }
    executorService.shutdown();
}
```

### FutureTask

FutureTask 实现了 RunnableFuture 接口，该接口继承自 Runnable 和 Future 接口，这使得 FutureTask 既可以当做一个任务执行，也可以有返回值。

三大功能：

- 判断任务是否完成
- 能够中断完成
- 能够获取任务执行的结果

```java
public class Main {
    private static class Task implements Callable<Integer> {
        @Override
        public Integer call() throws Exception {
            System.out.println("subThread running...");
            Thread.sleep(3000);
            int sum = 0;
            for (int i = 0; i < 100; i++) {
                sum += i;
            }
            return sum;
        }
    }

    public static void main(String[] args) {
        ExecutorService executor = Executors.newCachedThreadPool();
        Task task = new Task();
        FutureTask<Integer> futureTask = new FutureTask<>(task);
        executor.submit(futureTask);
        executor.shutdown();

        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("mainThread running...");

        try {
            System.out.println(futureTask.get());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```

### Java join()方法

`t.join()` 方法阻塞调用此方法的线程，直到线程t完成，此线程再继续，通常用于在 `main()` 主线程内，等待其他线程完成再结束 `main()` 主线程

### 单例模式

不安全的写法：

```java
public class UnsafeLazyInitialization() {
    private static Instance instance;
    public static Instance getInstance() {
        if (instance == null) {
            instance = new Instance;
        }
        return instance;
    }
}
```

安全的写法，但是synchronized性能开销，如果被多个线程频繁调用，导致程序性能下降：

```java
public class SafeLazyInitialization() {
    private static Instance instance;
    public synchronized static Instance getInstance() {
        if (instance == null) {
            instance = new Instance();
        }
        return instance;
    }
}
```

双重检查锁定：

```java
public class DoubleCheckedLocking {
    private static Instance instance;
    public static Instance getInstance() {
        if (instance == null) { // 读到这一行的时候，instance引用的对象可能还没有完成初始化
            synchronized (DoubleCheckedLocking.class) {
                if (instance == null) {
                    instance = new Instance(); // 问题根源，发生指令重排序，导致分配了地址但未初始化完成
                }
            }
        }
        return instance;
    }
}
```

基于volatile的解决方案，禁止指令重排序：

```java
public class SafeDoubleCheckedLocking {
    private volatile static Instance instance;
    public static Instance getInstance() {
        if (instance == null) {
            synchronized (SafeDoubleCheckedLocking.class) {
                if (instance == null) {
                    instance = new Instance();
                }
            }
        }
        return instance;
    }
}
```

基于类初始化的解决方案：

类初始化阶段会执行类的初始化，JVM会去获取一个锁，这个锁可以同步多个线程对同一个类的初始化

```java
public class InstanceFactory {
    private static class InstanceHolder {
        public static Instance instance = new Instance();
    }

    public static Instance getInstance() {
        reutrn InstanceHolder.instance;
    }
}
```

基于volatile的方案优势是：除了对静态字段实现延迟初始化外，还可以对实例字段实现延迟初始化

### 抽象工厂模式

角色：抽象工厂，具体工厂，抽象产品，具体产品，产品使用者

先创建抽象产品（抽象产品A，抽象产品B），具体产品（具体产品A，具体产品B）

```java
// 抽象产品A，B
public interface ProductA {}

public interface ProductB {}
// 具体产品A1，B1，A2，B2
public class CNProductA implements ProductA {}
public class CNProductB implements ProductB {}

public class USProductA implements ProductA {}
public class USProductB implements ProductB {}
```

再创建抽象工厂和具体工厂

```java
// 抽象工厂
public interface Factory {
    public ProductA createProductA(String name);
    public ProductB createProductB(String name);
}

// 具体工厂 CN
public class CNFactory implements Factory {
    @Override
    public ProductA createProductA(String name) {
        return new CNProductA();
    }

    @Override
    public ProductB createProductB(String name) {
        return new CNProductB();
    }
}

// 具体工厂 US
public class USFactory implements Factory {
    @Override
    public ProductA createProductA(String name) {
        return new USProductA();
    }

    @Override
    public ProductB createProductB(String name) {
        return new USProductB();
    }
}
```

客户端：

```java
public class Client {
    public static void main(String[] args) {
        Factory cnFactory = new CNFactory();
        Factory usFactory = new USFactory();

        ProductA cnProductA = cnFactory.createProductA("cn product A");
        ProductB cnProductB = cnFactory.createProductB("cn product B");

        ProductA usProductA = usFactory.createProductA("us product A");
        ProductB usProductB = usFactory.createProductB("us product B");

        cnProductA.printName();
        cnProductB.printName();
        usProductA.printName();
        usProductB.printName();
    }
}
```

# Java 基础

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

## 垃圾回收机制

判断一个对象是否可被回收

- 1.引用计数算法
- 2.可达性分析算法 可达的对象是存活的，不可达的对象是可被回收的
  - 可以作为GCRoot的对象包括：虚拟机栈中引用的对象，方法区中类静态属性引用的对象，方法区中常量引用的对象，本地方法栈中JNI引用的对象

引用类型：

- 强引用：程序代码中普遍存在的，类似 `Object obj = new Object()`，只要强引用在垃圾收集器永远不会回收被引用的对象
- 软引用：描述一些还有用但非必需的对象，对于软引用关联着的对象，在系统将要发生内存溢出之前，将会把这些对象列进回收范围之中，进行第二次回收
- 弱引用：描述非必需的对象，被弱引用关联的对象只能生存到下一个垃圾回收之前
- 虚引用：能够在这个对象被垃圾收集器回收时收到一个系统通知

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

## 类加载机制

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

### 加载

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

### 验证

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

### 准备

正式为类变量（static修饰）分配内存并设置类变量初始值的阶段，这些变量所使用的内存都在方法区中被分配

实例变量不会在这阶段分配内存，它会在对象实例化时随着对象一起被分配在堆中。应该注意到，实例化不是类加载的一个过程，类加载发生在所有实例化操作之前，并且类加载只进行一次，实例化可以进行多次。

```java
// 初始值一般为 0 值，例如下面的类变量 value 被初始化为 0 而不是 123。
public static int value = 123;
// 类变量是常量，那么它将初始化为表达式所定义的值而不是 0。例如下面的常量 value 被初始化为 123 而不是 0。
public static final int value = 123;
```

### 解析

将常量池的符号引用替换为直接引用的过程。

- 符号引用：符号引用和虚拟机的布局无关，一组符号来描述引用的目标，符号引用可以是任何形式的字面量，无歧义地定位到目标即可，目标可能未加载到内存中
- 直接引用：直接引用和虚拟机的布局相关，可以是直接指向目标的指针、相对偏移量、一个间接定位到对象的句柄，目标一定加载到内存中

- 类或接口解析
- 字段解析
- 类方法解析
- 接口方法解析

### 初始化

初始化阶段才真正开始执行类中定义的 Java 程序代码，初始化阶段是虚拟机执行类构造器 `<clinit>()` 方法的过程，在准备阶段，类变量已经赋过一次系统要求的初始值，而在初始化阶段，根据程序员通过程序制定的主观计划去**初始化类变量**和其它资源。

- `<clinit>()` 方法是由编译器自动收集类中所有 类变量的赋值动作 和 静态语句块的语句 合并产生的，编译器收集的顺序是由语句在源文件中出现的顺序所决定的
- `<clinit>()` 保证在子类的 `<clinit>()` 方法执行之前，父类的 `<clinit>()` 方法已经执行了
- 由于父类的 `<clinit>()` 方法先执行，也就意味着父类中定义的静态语句块的执行要优先于子类
- 如果一个类没有静态语句块，也没有对变量的赋值操作，那么编译器可以不为这个类生成 `<clinit>()` 方法
- 接口中不可以使用静态语句块，但仍然有类变量初始化的赋值操作，因此接口与类一样都会生成 `<clinit>()` 方法。但接口与类不同的是，执行接口的 `<clinit>()` 方法不需要先执行父接口的 `<clinit>()` 方法。只有当父接口中定义的变量使用时，父接口才会初始化。另外，接口的实现类在初始化时也一样不会执行接口的 `<clinit>()` 方法。
- 虚拟机会保证一个类的 `<clinit>()` 方法在多线程环境下被正确的加锁和同步，如果多个线程同时初始化一个类，只会有一个线程执行这个类的 `<clinit>()` 方法，其它线程都会阻塞等待，直到活动线程执行 `<clinit>()` 方法完毕。如果在一个类的 `<clinit>()` 方法中有耗时的操作，就可能造成多个线程阻塞，在实际过程中此种阻塞很隐蔽。

## 类和类加载器

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

## 对象的创建，对象的内存布局，对象的访问定位

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

## Java NIO

[Java I/O](https://github.com/CyC2018/CS-Notes/blob/master/docs/notes/Java%20IO.md)

## Spring

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

### Spring MVC 流程

1. 用户发送请求至前端控制器DispatcherServlet。
2. DispatcherServlet收到请求调用HandlerMapping处理器映射器。
3. 处理器映射器找到具体的处理器(可以根据xml配置、注解进行查找)，生成处理器对象及处理器拦截器(如果有则生成)一并返回给DispatcherServlet。
4. DispatcherServlet调用HandlerAdapter处理器适配器。
5. HandlerAdapter经过适配调用具体的处理器(Controller，也叫后端控制器)。
6. Controller执行完成返回ModelAndView。
7. HandlerAdapter将controller执行结果ModelAndView返回给DispatcherServlet。
8. DispatcherServlet将ModelAndView传给ViewReslover视图解析器。
9. ViewReslover解析后返回具体View。
10. DispatcherServlet根据View进行渲染视图（即将模型数据填充至视图中）。
11. DispatcherServlet响应用户