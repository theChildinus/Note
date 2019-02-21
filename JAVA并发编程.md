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

## 原子性、可见性和有序性

### 原子性

基本数据类型的访问读写是具有原子性的，更大范围的的原子性保证通过 synchronized 实现

### 可见性

可见性即当一个线程修改了共享变量的值，其他线程能够立即得知这个修改

java 内存模型通过 在变量修改后将新值同步回主内存，在变量读取前从主内存刷新变量值这种依赖主内存作为传输媒介的方式实现可见性

区别于普通变量，volatile 的特殊规则保证了新值能立即同步到主内存，以及每次使用前立即从主内存刷新

除了 volatile，synchronized（unlock规则） 和 final 也实现了可见性

### 有序性

如果在本线程内观察，所有的操作都是有序的，如果在一个线程中观察另外一个线程，所有的操作都是无序的

java通过 volatile 和 synchronized（lock规则）保证线程之间操作的有序性

## 先行发生原则

判断数据是否存在竞争，线程是否安全的主要依据

- 程序次序规则
- 官程锁定规则
- volatile 变量规则
- 线程启动规则
- 线程终止规则
- 线程中断规则
- 对象终结规则
- 传递性

时间上先发生 和 先行发生 不能相互推导

我们衡量并发安全问题不要受时间顺序的干扰一切必须以先行发生原则为准

## Java语言中的线程安全

- 不可变
  - 共享数据是基本数据类型，final 修饰就可以保证它是不可变的，如果共享数据是一个对象，那就需要保证对象的行为不会对其状态产生任何影响才行 String 枚举型
- 绝对线程安全
  - 不管运行时环境如何，调用者都不需要任何额外的同步措施
- 相对线程安全
  - 保证对这个对象单独的操作是安全的，在调用时不需要做额外的保护措施，但对于特定顺序的连续调用，就可能需要在调用端使用额外的同步手段 Vector HashTable
- 线程兼容
  - 对象本身并不是线程安全的，但是可以通过调用端正确地使用同步手段来保证对象在并法环境中可以安全地使用 Java API中大部分都属于线程兼容 ArrayList HashMap
- 线程对立
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
- 锁可以绑定多个条件

#### 2.非阻塞同步

基于冲突检测的乐观并发策略，先进行操作，如果没有其他线程争用共享数据，那操作就成功了，如果共享数据有争用，产生冲突再采取其他补偿措施

这就需要操作和冲突检测这两个步骤具有原子性

- Test-and-Set
- Fetch-and-Increment
- Swap
- Compare-and-Swap CAS
- Load-Linked / Store-Conditional

乐观锁基本上都是通过CAS实现的

CAS 的三个问题

- ABA问题
- 循环时间长开销大
- 只能保证一个共享变量的原子操作

#### 3.无同步方案

同步只是保证共享数据争用时的正确性的手段，如果一个方法本来就不涉及共享数据，那他就自然无需任何同步措施保证正确性，例如：

- 可重入代码
  - 可以在代码执行的任何时刻中断它，转而去执行另外一段代码，方法返回不会出现任何错误。
  - 方法返回结果是可以预测的，只要输入了相同的数据就能放回相同的结果，那他就满足可重入性
- 线程本地存储
  - java.lang.ThreadLocal 类

## volatile 的应用

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

### 1.偏向锁

大多数情况下，锁不仅不存在多线程竞争，而且总是由同一线程多次获得，为了让线程获取锁的代价更低从而引入了偏向锁

当一个线程访问同步块并获得锁，会在对象头和栈帧的锁记录里存储锁偏向的线程ID，以后该线程在进入和退出同步块就不需要进行CAS操作来加锁和解锁，只是简单测试对象头里的 Mark Word里是否存储着指向当前线程的偏向锁

- 如果测试成功，表示线程已经获取了偏向锁
- 如果失败
  - 需要在测试以下Mark Word中偏向锁是否设置成1
  - 如果没有设置则使用CAS竞争锁
  - 如果设置了则尝试使用CAS将对象头的偏向锁指向当前线程

偏向锁使用了一种等到竞争出现才释放锁的机制，所以当其他线程尝试竞争偏向锁时，持有偏向锁的线程才会释放锁

### 2.轻量级锁

#### 轻量级锁加锁

线程在执行同步块之前，JVM会在当前线程的栈帧中创建用于存储锁记录的空间，并将对象头中的 Mark Word 复制到锁记录中 （Displaced Mark Word），然后线程尝试使用CAS将对象头中的 Mark Word 替换为指向锁记录的指针

- 如果成功，当前线程获得锁 此时锁记录为原始的 Mark Word，对象头中的 Mark Word 为锁记录指针
- 如果失败，表示其他线程竞争锁，当前线程便尝试使用自旋锁获得锁（自旋锁：不放弃处理器的执行时间，让线程执行一个忙循环）

#### 轻量级锁解锁

解锁时，会使用原子的CAS操作将 Displaced Mark Word 替换回到对象头

- 如果成功，则表示没有竞争发生 对象头中的 Mark Word 为原始值
- 如果失败，则表示当前锁存在竞争，锁就会膨胀成重量级锁

因为自旋会消耗CPU，为了避免无用的自旋，一旦升级成重量级锁，就不会再恢复到轻量级状态，当锁处于这个状态，其他线程试图获取锁都会被阻塞，当持有锁的线程释放锁之后才会唤醒这些线程

有竞争的情况下，轻量级锁会比传统的重量级锁更慢，因为有额外的CAS操作

轻量级锁能够提升程序同步性能的依据是：**对于绝大部分的锁，在整个同步周期内都是不存在竞争的**，轻量级锁所适应的场景是线程交替执行同步块的场合

#### 中断与 synchronized

线程的中断操作对于正在等待获取的锁对象的 synchronized 方法或者代码块并不起作用，也就是对于 synchronized 来说，如果一个线程在等待锁，那么结果只有两种，要么它获得这把锁继续执行，要么它就保存等待，即使调用中断线程的方法，也不会生效

#### 等待唤醒机制与synchronized

等待唤醒机制本篇主要指的是 `notify`，`notifyAll` 和 `wait` 方法，在使用这3个方法时，必须处于synchronized代码块或者synchronized方法中，否则就会抛出IllegalMonitorStateException异常

这是因为调用这几个方法前必须拿到当前对象的监视器monitor对象，也就是说notify/notifyAll和wait方法依赖于monitor对象，在前面的分析中，我们知道monitor 存在于对象头的Mark Word 中(存储monitor引用指针)，而 synchronized 关键字可以获取 monitor ，这也就是为什么notify/notifyAll和wait方法必须在synchronized代码块或者synchronized方法调用的原因。

```java
synchronized (obj) {
    obj.wait();
    obj.notify();
    obj.notifyAll();
}
```

需要特别理解的一点是，与sleep方法不同的是wait方法调用完成后，线程将被暂停，但wait方法将会释放当前持有的监视器锁(monitor)，直到有线程调用notify/notifyAll方法后方能继续执行，而sleep方法只让线程休眠并不释放锁。同时notify/notifyAll方法调用后，并不会马上释放监视器锁，而是在相应的 `synchronized(){}块`，`synchronized方法` 执行结束后才自动释放锁。

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

### 队列同步器

队列同步器是用来构建锁或者其他同步组件的基础框架，它使用一个int成员变量表示同步状态，通过内置的FIFO队列来完成资源获取线程的排队工作

同步器的主要使用方式是继承

同步器是实现锁的关键，在锁的实现中聚合同步器，利用同步器实现锁的语义

锁和同步器的关系：

- 锁是面向使用者的，它定义了使用者与锁交互的接口，隐藏了实现细节
- 同步器是面向锁的实现者，它简化了锁的实现方式，屏蔽了同步状态管理、线程的排队、等待和唤醒等底层操作

同步器依赖内部的同步队列来完成同步状态的管理，同步队列中的节点用来保存获取同步状态失败的线程引用、等待状态以及前驱和后继节点

## Java并发容器和框架

### ConcurrentHashMap

线程安全且高效的HashMap

ConcurrentHashMap的锁分段技术可有效提高并发访问率：容器中有多把锁，每一把锁用于锁容器其中一部分数据，那么当多线程访问容器里不同数据段的数据时，线程间不会存在锁竞争，从而可以有效提高并发访问效率

ConcurrentHashMap get操作的高效之处在于整个get过程不需要加锁

- get操作：高效之处在于只需要读不需要写,将需要使用的共享变量定义为 volatile
- put操作：为了线程安全必须加锁，1.定位到Segment 2.在Segment里进行插入操作 插入操作需要经历两个步骤 1.是否需要扩容 2.定位添加元素的位置，然后将其放到HashEntry数组中
- size操作：先尝试2次通过不锁Segment的方式来统计各个Segment，如果统计的过程中，容器的count发生了变化（在统计size前后比较modCount是否发生变化），再采用加锁方式

### ConcurrentLinkedQueue

ConcurrentLinkedQueue 是一个基于链接节点的非阻塞无界线程安全队列，它采用先进先出的规则对节点进行排序

### 阻塞队列

阻塞队列是支持两个附加操作的队列，这两个附加操作支持阻塞的插入和移除方法

- 支持阻塞的插入方法：意思是当队列满时，队列会阻塞插入元素的线程，直到队列不满
- 支持阻塞的移除方法：意识是在队列为空时，获取元素的线程会等待队列变为非空

阻塞队列常用于生产者消费者的场景，生产者向队列中添加元素的线程，消费者是从队列中取元素的线程，阻塞队列就是生产者用来存放元素，消费者用来获取元素的容器

- ArrayBlockingQueue
- LinkedBlockingQueue
- PriorityBlockingQueue
- DelayQueue
- SynchronousQueue
- LinkedTransferQueue
- LinkedBlockingDeque

#### 阻塞队列的实现原理

使用通知模式实现，通知模式：当生产者往满的队列中添加元素时会阻塞生产者，当消费者消费了一个队列中的元素后，会通知生产者当前队列可用

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

### Java join()方法

`t.join()` 方法阻塞调用此方法的线程，直到线程t完成，此线程再继续，通常用于在 `main()` 主线程内，等待其他线程完成再结束 `main()` 主线程
