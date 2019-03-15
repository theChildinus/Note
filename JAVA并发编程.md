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

以 ThreadLocal对象为键，任意对象为值的存储结构 一个线程可以根据一个ThreadLocal对象查询到绑定在这个线程上的值

ThreadLocal

- 在每个线程 Thread 内部有一个ThreadLocal.ThreadLocalMap类型的成员变量threadLocals，这个threadLocals就是用来存储实际的变量副本的，键值为当前ThreadLocal变量，value为变量副本（即T类型的变量）
- 初始时，在Thread里面，threadLocals为空，当通过ThreadLocal变量调用get()方法或者set()方法，就会对Thread类中的threadLocals进行初始化，并且以当前ThreadLocal变量为键值，以ThreadLocal要保存的副本变量为value，存到threadLocals
- 然后在当前线程里面，如果要使用副本变量，就可以通过get方法在threadLocals里面查找

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