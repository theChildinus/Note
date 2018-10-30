# 知识点

本md记录一些平时看到的琐碎知识点

## I/O 模式 (Linux network IO)

[IO - 同步，异步，阻塞，非阻塞 （亡羊补牢篇）](https://blog.csdn.net/historyasamirror/article/details/5778378)

[也谈IO模型](http://www.rowkey.me/blog/2016/01/18/io-model/)

- blocking IO
- non-blocking IO
- IO multiplexing
- signal driven IO
- asynchronous IO

**blocking vs non-blocking**: 前面的介绍中其实已经很明确的说明了这两者的区别。调用blocking IO会一直block住对应的进程直到操作完成，而non-blocking IO在kernel还准备数据的情况下会立刻返回。

**synchronous IO和asynchronous IO**: 

- A synchronous I/O operation causes the requesting process to be blocked until that **I/O operation** completes
  - blocking IO
  - non-blocking IO
  - IO multiplexing
- An asynchronous I/O operation does not cause the requesting process to be blocked;