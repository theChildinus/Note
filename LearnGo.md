# 学习Go

## 配置

### 安装 Go

```txt
下载：
https://golang.google.cn/dl/
tar -zxvf go1.10.linux-amd64.tar.gz -C /usr/local

配置环境变量:
export GOROOT=/usr/local/go-1.10
export GOBIN=$GOROOT/bin
export GOPATH=/home/kong/goProject

export PATH=$GOPATH/bin:$GOBIN:$GOROOT:$PATH
source ~/.zshrc（或者是 ~/.bashrc）

检查：
go version
go env
```

### vscode + Go 开发环境

在vscode中安装插件Go 由于网络问题会报错：

```txt
Installing github.com/josharian/impl FAILED
```

解决办法：

在 `%GOPATH` 按照下面的目录结构新建缺失的文件夹

```txt
src
├── github.com
|      └── golang
└── golang.org
       └── x
```

下载tools，`git clone git@github.com:golang/tools.git`，放到 `%GOPATH%/src/golang.org/x` 路径下

安装lint，`go get -u golang.org/x/lint/golint`

切换到 `%GOPATH` 下，安装缺失的包：

```txt
go install github.com/mdempsky/gocode
go install github.com/uudashr/gopkgs/cmd/gopkgs
go install github.com/ramya-rao-a/go-outline
go install github.com/acroca/go-symbols
go install golang.org/x/tools/cmd/guru
go install golang.org/x/tools/cmd/gorename
go install github.com/go-delve/delve/cmd/dlv
go install github.com/sqs/goreturns
go install github.com/rogpeppe/godef
go install golang.org/x/lint/golint
```

## 基本语法

[golang注意事项](https://blog.csdn.net/smile_yangyue/article/details/81076993)

### 变量，常量

变量关键字 `var`

变量类型写在变量名之后，函数外部不可使用 `:=`，函数外定义变量的作用域为包

```go
package main

var aa = 3
var bb = "kkk"
cc := true //错误

// 或者如下格式集中定义：

var (
    aa = 3
    bb = "kkk"
    cc = true
)

func main() {
    // 编译器自动决定类型
    var a, b, i, s1, s2 = true, false, 3, "hello", "world"
    // 使用 ：= 定义变量，但只能在方法内使用
    a, b, i, s1, s2 := true, false, 3, "hello", "world"
}
```

内建变量类型

- `bool`, `string`
- `(u)int`, `(u)int8`, `(u)int16`, `(u)int32`, `(u)int64`, `uintptr`
- `byte`， `rune`
- `float32`， `float64`， `complex64`， `complex128`

类型转换是强制的，没有隐式类型转换

常量关键字 `const`

- `const filename = "abc.txt"`
- const 数值可以作为各种类型使用，无需强制类型转换如：`const a, b = 3, 4`， `var c int = int(math.Sqrt(a*a + b*b))`

枚举类型：

```go
const (
    cpp = 0
    java = 1
    python = 2
    golang = 3
)

// 或者写为自增值：
const (
    cpp = iota
    java
    python
    golang
)

// 或者写为自增值：
const (
    b = 1 << (10 * iota)
    kb
    mb
    gb
    tb
    pb
)
// 输出：1 1024 1048576 1073741824 1099511627776 1125899906842624
```

### 选择、循环语句

关键字 `if`

- 不需要括号
- if的条件中可以赋值，条件里赋值的变量作用域就在这个if语句里

```go
func main() {
    const filename = "abc.txt"
    if contents, err := ioutil.ReadFile(filename); err != nil {
        fmt.Println(err)
    } else {
        fmt.Println(contents)
    }
}
```

关键字 `switch`

- `switch` 自动 `break`，除非使用 `fallthrough`，`switch` 后可不加表达式

```go
    switch {
    case score < 0 || score > 100:
        panic("Wrong score")
    case score < 60:
        grade = "F"
    case score < 80:
        grade = "C"
    case score < 90:
        grade = "B"
    case score <= 100:
        grade = "A"
    }
}
```

循环语句关键字 `for`，没有括号，可以当作 `while` 使用

### 函数、指针

函数可以返回多个值，并可以给多个值起名字

```go
func eval(a, b int, op string) (int, error) {
    switch op {
    case "+":
        return a + b, nil
    case "-":
        return a - b, nil
    case "*":
        return a * b, nil
    case "/":
        return a / b, nil
    default:
        return 0, fmt.Errorf("error operation: %s", op)
    }
}
```

函数式，函数可以作为参数：

```go
func apply(op func(int, int) int, a, b int) int {
    return op(a, b)
}
```

可变参数列表：

```go
func sum(numbers ...int) int {
    s := 0
    for i := range numbers {
        s += numbers[i]
    }
    return s
}
```

go指针不能运算，go语言只有值传递

### 数组，容器

数组的定义：

```go
var arr1 [5]int
arr2 := [3]int{1, 3, 5}
arr3 := [...]int{2, 3, 4, 5, 6}
var grid [4][5]bool

// 遍历：

for i, v := range arr3 {
    fmt.Println(i, v)
}
```

**数组是值类型**，数组名非数组的头指针，`[10]int` 和 `[20]int` 是不同类型

```go
func printArray(arr [5]int) {
    arr[0] = 200
    fmt.Println(arr)
}

func printArray2(arr *[5]int) {
    arr[0] = 200
    fmt.Println(arr)
}

func main() {
    var arr1 = [5]int{1, 2, 3, 4, 5}
    printArray(arr1)
    // 此时arr1 为[1,2,3,4,5]
    printArray2(&arr1)
    fmt.Println(arr1)
}

// 输出
[200 2 3 4 5]
&[200 2 3 4 5]
[200 2 3 4 5]
```

基于数组，**切片(slice)**添加了一系列管理功能，可以随时动态扩充存放空间，并且可以被随意传递而不会导致所管理的元素被复制，**slice本身没有数据，是对底层array的一个view**

切片的数据结构可以抽象为以下三个变量：

- 一个指向原生数组的指针
- 切片中的元素个数
- 切片已分配的存储空间

![slice结构](https://raw.githubusercontent.com/theChildinus/Note/master/image/slice2.png)

切片的使用：

```go
var arr1 = [...]int{0, 1, 2, 3, 4, 5, 6, 7}
fmt.Println("arr[2:6] = ", arr1[2:6]) // arr[2:6] =  [2 3 4 5]
fmt.Println("arr[:6] = ", arr1[:6])   // arr[:6] =  [0 1 2 3 4 5]
fmt.Println("arr[2:] = ", arr1[2:])   // arr[2:] =  [2 3 4 5 6 7]
fmt.Println("arr[:] = ", arr1[:])     // arr[:] =  [0 1 2 3 4 5 6 7]

s1 := []int{1, 2, 3, 4, 5}
s2 := make([]int, 16)
s3 := make([]int, 10, 32)
```

```go
func updateSlice(arr []int) {
    arr[0] = 100
}
func main() {
    var arr1 = [...]int{0, 1, 2, 3, 4, 5, 6, 7}
    s1 := arr1[2:]
    fmt.Println("s1: ", s1) // s1:  [2 3 4 5 6 7]
    s2 := arr1[:]
    fmt.Println("s2: ", s2) // s2:  [0 1 2 3 4 5 6 7]

    updateSlice(s1)
    fmt.Println(s1) // [100 3 4 5 6 7]
    fmt.Println(arr1) // [0 1 100 3 4 5 6 7]

    updateSlice(s2)
    fmt.Println(s2) // [100 1 100 3 4 5 6 7]
    fmt.Println(arr1) // [100 1 100 3 4 5 6 7]
}
```

```go
var arr1 = [...]int{0, 1, 2, 3, 4, 5, 6, 7}
fmt.Println("arr1:", arr1) // arr1: [0 1 2 3 4 5 6 7]
s1 := arr1[2:6]
s2 := s1[3:5]
fmt.Println("s1:", s1, "len(s1):", len(s1), "cap(s1):", cap(s1)) // s1: [2 3 4 5] len(s1): 4 cap(s1): 6
fmt.Println("s2:", s2, "len(s2):", len(s2), "cap(s2):", cap(s2)) // s2: [5 6] len(s2): 2 cap(s2): 3
```

slice可以向后扩展，不可以向前扩展，`s[i]` 不可以超越 `len(s)`，向后扩展不可以超越底层数组 `cap(s)`，slice 扩容为两倍扩容

![slice](https://raw.githubusercontent.com/theChildinus/Note/master/image/slice.png)

`append`：添加元素时如果超越cap，系统会重新分配更大的底层数组，由于值传递的缘故，必须接收 `append` 的返回值，因为扩容可能导致cap和ptr改变，基本用法如下：

```txt
Append returns the updated slice. It is therefore necessary to store the result of append, often in the variable holding the slice itself:
    slice = append(slice, elem1, elem2)
    slice = append(slice, anotherSlice...)
```

```go
// s2 = [5, 6]
s3 := append(s2, 10)
s4 := append(s3, 11)
s5 := append(s4, 12)

fmt.Println("s3, s4, s5 =", s3, s4, s5) // s3, s4, s5 = [5 6 10] [5 6 10 11] [5 6 10 11 12]
fmt.Println(arr1) // [0 1 2 3 4 5 6 10]

// 删除s5的下标为2的元素
s6 := append(s5[:2], s5[3:]...)
fmt.Println(s6) // [5 6 11 12]
```

关键字`range`，类似迭代器，可以遍历数组，字符串，map等

- range作用于 `string`，第一个返回值是index，第二个是char
- range作用于 `array`，第一个返回值是index,第二个是value
- range作用于 `slice`，第一个返回值是index，第二个是value
- range作用于 `map`，第一个返回值是key，第二个是value

注意：

- `range` 会复制对象，而不是直接在原对象上操作
- 使用 `range` 迭代遍历引用类型时，底层数据不会被复制

### 面向接口

- 结构体
- duck typing的概念
- 组合的思想

### 函数式编程、并发编程

- 闭包的概念
- goroutine和channel
- 调度器

## [ipc & net/http](https://github.com/theChildinus/learngo)

## REST

### 什么是REST

RESTful，是目前最为流行的一种互联网软件架构，用于Web数据接口的设计，要理解REST(REpresentational State Transfer) 需要理解以下几个概念：

- 资源 Resources：通过URI定位的一张图片、一个文档、一个视频等
- 表现层 Representation：资源是做一个具体的实体信息，他可以有多种的展现方式，而把实体展现出来就是表现层，如 txt文本信息可以输出为 html、json、xml等格式，HTTP请求头信息中的 Accept 和Content-Type 字段是对表现层的描述
- 状态转化 State Transfer：访问一个网站，就代表了客户端和服务器的一个互动过程，在这个过程中，肯定涉及到数据和状态的变化。而HTTP协议是无状态的，那么这些状态肯定保存在服务器端，所以如果客户端想要通知服务器端改变数据和状态的变化，肯定要通过某种方式来通知它。

综上，总结RESTful架构为：

- （1）每一个URI代表一种资源；
- （2）客户端和服务器之间，传递这种资源的某种表现层；
- （3）客户端通过四个HTTP动词（GET、POST、PUT、DELETE），对服务器端资源进行操作，实现"表现层状态转化"。

Web 应用要满足REST最重要的原则是：客户端和服务器之间的交互在请求之间是无状态的,即从客户端到服务器的每个请求都必须包含理解请求所必需的信息。

另一个重要的REST原则是：系统分层，这表示组件无法了解除了与它直接交互的层次以外的组件。通过将系统知识限制在单个层，可以限制整个系统的复杂性，从而促进了底层的独立性。

## RPC

基于Socket和HTTP实现了类似 信息交换的模式，很多独立应用没有采用这种模式，而是采用了类似常规的函数调用的方式来完成想要的功能

RPC就是实现函数调用模式的网络化，客户端就像调用本地函数一样，然后客户端把这些参数打包之后通过网络传递到服务端，服务端解包到处理过程中执行，然后执行的结果反馈给客户端。