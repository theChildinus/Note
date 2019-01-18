# Review

## **工程：内存重构分析**

[MemoryAnalyse - 工程地址](https://github.com/theChildinus/JavaMemory)

### 必备知识

1. Java、Python编程基础
2. Volatility 实现原理及自带工具，安装和部署
3. libvmi 原理及功能
4. JVM 内存模型，堆栈分配
5. Java多线程编程基础
6. 基于C程序的缓存区溢出攻击原理及实现 - [参考工程: buffer-overflow-attack](https://github.com/theChildinus/buffer-overflow-attack)
7. 本地分析Java程序堆栈信息 - [参考工程：JDI_Local](https://github.com/theChildinus/JavaMemory/tree/master/JDI_Local)
8. Serviceability Agent 原理

### TODOLIST

1. 如何解析编译型栈帧
2. 如何同时解析多线程程序中的多个线程，并根据解析结果判断程序运行状态
3. 脏数据优化
4. 系统性能优化

## **工程：物联网事件监控平台系统**

[物联网事件监控平台系统 - 工程地址](https://github.com/theChildinus/IoTEventMonitorPlatform)

[依赖工程1: tinylog](https://github.com/theChildinus/tinylog) 为系统提供日志服务

[依赖工程2: tinyxml2](https://github.com/theChildinus/tinyxml2) 为系统提供xml解析服务

[依赖工程3: runtime-verification](https://github.com/theChildinus/runtime-verification-system) 基于微软z3求解器，为系统提供事件验证服务

[依赖工程4: netfilter](https://github.com/theChildinus/netfilter) 内核模块，为系统提供处理网络流量的功能

[依赖工程5: mxGraph](https://github.com/theChildinus/mxGraph) 绘图工具，根据需求绘制事件转移关系图

### 必备知识

1. C/C++ 语言编程基础
2. Linux系统使用，基本命令
3. Linux环境下虚拟机安装和部署
4. `Qemu`, `KVM`, `virt-manager` 三者之间的关系及作用
5. 微软z3求解器工具，可使用 C/C++ 实现一些简单求解
6. netfilter内核模块了解其两大模块 `netfilter` 和 `netlink` 作用
7. 会用mxGraph绘制事件状态转移图，熟悉绘图时的注意事项
8. 桥接模式原理及配置方法
9. 串口使用及调试
10. 理解系统界面中配置含义

### TODOLIST

1. 优化网络、串口、内存三模块的处理逻辑
2. 事件验证求解过程中，事件信息、状态转移信息等如何使用z3表达式定义，z3表达式生成过程优化
3. 系统性能优化