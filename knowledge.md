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

## 浅拷贝和深拷贝

[参考：Java深拷贝和浅拷贝](https://my.oschina.net/jackieyeah/blog/206391)

[使用序列化实现对象的拷贝](https://blog.csdn.net/chenssy/article/details/12952063)

- 浅拷贝是按位拷贝对象，它会创建一个新对象，这个对象有着原始对象属性值的一份精确拷贝。如果属性是基本类型，拷贝的就是基本类型的值；如果属性是内存地址（引用类型），拷贝的就是内存地址 ，因此如果其中一个对象改变了这个地址，就会影响到另一个对象。
- 深拷贝会拷贝所有的属性,并拷贝属性指向的动态分配的内存。当对象和它所引用的对象一起拷贝时即发生深拷贝。深拷贝相比于浅拷贝速度较慢并且花销较大。

理解深拷贝和浅拷贝的关键在于，Java语言在拷贝的时候只存在值拷贝，对于对象或者容器的拷贝也只是将对象或者容器的地址拷贝，在判断修改引用时是否会引起干扰，先要看对地址做了什么的操作，再看对地址的值做了什么操作