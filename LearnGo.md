<!-- TOC -->

- [学习Go](#学习go)
    - [配置](#配置)
        - [安装 Go](#安装-go)
        - [Go开发环境](#go开发环境)
    - [基本语法](#基本语法)
        - [变量、常量](#变量常量)
        - [选择、循环语句](#选择循环语句)
        - [函数、指针](#函数指针)
        - [数组、容器](#数组容器)
        - [面向对象](#面向对象)
        - [接口](#接口)
    - [函数式编程](#函数式编程)
    - [资源管理和错误处理](#资源管理和错误处理)
    - [并发编程](#并发编程)
        - [协程 Coroutine](#协程-coroutine)
        - [goroutine](#goroutine)
        - [channel](#channel)
        - [select](#select)
        - [同步机制](#同步机制)
    - [ipc & net/http](#ipc--nethttp)
    - [REST](#rest)
        - [什么是REST](#什么是rest)
    - [RPC](#rpc)

<!-- /TOC -->

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

### Go开发环境

#### IDEA

`IDEA plugin` 安装GO插件

安装 go 包管理工具：`go get -v github.com/gpmgo/gopm`

IDEA 安装 `goimports` 插件

- `gopm get -g -v -u golang.org/x/tools/cmd/goimports`
- `go install golang.org/x/tools/cmd/goimports`

#### vscode

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

### 变量、常量

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

### 数组、容器

#### 数组

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

#### slice切片

基于数组，切片添加了一系列管理功能，可以随时动态扩充存放空间，并且可以被随意传递而不会导致所管理的元素被复制，**slice本身没有数据，是对底层array的一个view**

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
Append returns the updated slice.
It is therefore necessary to store the result of append,
often in the variable holding the slice itself:
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

#### map

map的基本使用

```go
m1 := map[string]string{
    "name":    "ccmouse",
    "course":  "golang",
    "site":    "imooc",
    "quality": "good",
}

m2 = make(map[string]string) // m2 = empty map

var m3 map[string]string // m3 == nil

for k, v := range m1 {
    fmt.Println(k, v)
}

if courseName, ok := m1["course"]; ok {
    fmt.Println(courseName, ok)
} else {
    fmt.Println("key does not exist")
}

delete(m1, "name")
```

map实现使用哈希表，取map中不存在的key的值，返回value的初始零值，map的key除了 `slice`，`map`，`function` 的内建类型都可以作为key，`struct` 类型不包含上述字段（编译器检查）也可以作为key

#### rune

```go
s := "abc我是谁!" // UTF-8编码
fmt.Println(s)

for _, b := range []byte(s) {
    fmt.Printf("%x ", b)
}
fmt.Println()

for i, ch := range s { // ch is a rune(int32)
    fmt.Printf("(%d %x) ", i, ch)
}
fmt.Println()

// abc我是谁!
// 61 62 63 e6 88 91 e6 98 af e8 b0 81 21 --- utf-8
// (0 61) (1 62) (2 63) (3 6211) (6 662f) (9 8c01) (12 21) --- unicode

fmt.Println("Rune count:", utf8.RuneCountInString(s))

bytes := []byte(s)
for len(bytes) > 0 {
    ch, size := utf8.DecodeRune(bytes) // ch is a rune
    bytes = bytes[size:]
    fmt.Printf("%c ", ch)
}
fmt.Println()

// Rune count: 7
// a b c 我 是 谁 !

for i, ch := range []rune(s) {
    fmt.Printf("(%d %c) ", i, ch)
}
fmt.Println()
// (0 a) (1 b) (2 c) (3 我) (4 是) (5 谁) (6 !) --- unicode
```

其他对字符串操作的函数在 `strings` 中

关键字`range`，类似迭代器，可以遍历数组，字符串，map等

- range作用于 `string`，第一个返回值是index，第二个是char
- range作用于 `array`，第一个返回值是index,第二个是value
- range作用于 `slice`，第一个返回值是index，第二个是value
- range作用于 `map`，第一个返回值是key，第二个是value

注意：

- `range` 会复制对象，而不是直接在原对象上操作
- 使用 `range` 迭代遍历引用类型时，底层数据不会被复制

### 面向对象

#### 结构体

go语言仅支持封装，不支持继承和多态，无构造函数，如需要控制结构体的构造，可是用工厂函数实现，**返回的是局部变量的地址**

结构实例的创建在堆上还是栈上是由编译器和运行环境决定的

```go
type treeNode struct {
    value       int
    left, right *treeNode
}

func createTreeNode(value int) *treeNode {
    return &treeNode{value: value}
}

func main() {
    var root treeNode
    root = treeNode{value: 3}
    root.left = &treeNode{}
    root.right = &treeNode{5, nil, nil}
    root.right.left = new(treeNode)
    root.left.right = createTreeNode(2)

    node := []treeNode{
        {value: 3},
        {},
        {6, nil, &root},
    }
    fmt.Println(node)
}
```

为结构定义方法需要显示定义和命名方法的接收者，函数名之前为接收者，接收者是值传递，所以只有使用指针才可以改变结构内容

- 要改变内容必须使用指针接收者
- 结构过大也考虑使用指针接收者
- 一致性：如有指针接收者，最好都是指针接收者
- 值接受者是go独有的
- 值/指针接收者均可接收值/指针

nil指针也可以调用方法

```go
func (node treeNode) print() {
    fmt.Print(node.value)
}

func (node *treeNode) setValue(value int) {
    node.value = value
}

func (node *treeNode) traverse() {
    if node == nil {
        return
    }
    node.left.traverse()
    node.print()
    node.right.traverse()
}
```

#### 封装

- 名字一般使用CamelCase
- 首字母大写代表public，首字母小写代表private，public和private 是针对包而言的
- **每个目录一个包**，目录名和包名可以不相同
- 为结构定义的方法必须放在同一个包内，但是可以在不同的文件相同的包内

如何扩充系统类型或者别人的类型：

- 定义别名 `type Queue []int`
- 使用组合 `type myNode struct { node *tree.Node }`

### 接口

`duck typing` 的概念

>"当看到一只鸟走起来像鸭子、游泳起来像鸭子、叫起来也像鸭子，那么这只鸟就可以被称为鸭子。"

在面向对象的编程语言中，当某个地方（比如某个函数的参数）需要符合某个条件的变量（比如要求这个变量实现了某种方法）时，什么是判断这个变量是否“符合条件”的标准？

python中的 duck typing，**运行时**才知道传入的 retriever 有没有 get，需要注释来说明接口

```python
def download(retriever):
    return retriever.get("www.baidu.com")
```

c++中的 duck typing，**编译时**才知道传入的 retriever 有没有 get，需要注释来说明接口

```cpp
template<class R>
string download(const R& retriever) {
    return retriever.get("www.baidu.com");
}
```

java中类似的写法，Retriever是一个接口，所以实现者必须要实现Retriever接口，解决了运行时、编译时发现错误，但不是 duck typing，因为必须实现 Retriever 接口，

```java
<R extends Retriever>
String download(R r) {
    return r.get("www.baidu.com");
}
```

go中的 dyck typing 采取了折中的办法：

- 静态类型系统
- 一个类型不需要显式地声明它实现了某个接口
- 仅当某个变量的类型实现了某个接口的方法，这个变量才能用在要求这个接口的地方

go中**接口由使用者定义**，接口的实现是隐式的，接口变量中有：实现者的类型 和 实现者的值/指针，并且

- 接口变量自带指针
- 接口变量同样采用值传递，几乎不需要使用接口的指针
- 只能以指针方式使用指针接收者的实现，值接收者都可以

```go

// package main
type Retriever interface {
    Get(url string) string
}

func download(r Retriever) string {
    return r.Get("www.baidu.com")
}

func main() {
    var r Retriever
    r = mock.Retriever{"mock"}
    fmt.Println("%T, %v\n", r, r)   // mock.Retriever {mock}

    r = &real.Retriever{UserAgent:"real"}
    fmt.Println("%T, %v\n", r, r)   // *real.Retriever &{real}
}

// package mock
type Retriever struct {
    Contents string
}

func (r Retriever) Get(url string) string {
    return r.Contents
}

// package real
type Retriever struct {
    UserAgent string
}

func (r *Retriever) Get(url string) string {
    return r.UserAgent
}
```

#### 接口的组合

接口组合由使用者组合

```go
// package main

const baiduURL = "www.baidu.com"
// 接口1
type Retriever interface {
    Get(url string) string
}

// 接口2
type Poster interface {
    Post(url string, form map[string]string) string
}

func download(r Retriever) string {
    return r.Get(baiduURL)
}

func post(p Poster) {
    p.Post(baiduURL, map[string]string{
        "name":   "zhao",
        "school": "bupt",
    })
}

// 组合接口
type RetrieverPoster interface {
    Retriever
    Poster
}

func session(rp RetrieverPoster) string {
    rp.Post(baiduURL, map[string]string{
        "contents": "hhhhhhhh",
    })
    return rp.Get(baiduURL)
}

func main() {
    rp := mock.RetrieverPosterImpl{"helloworld"}
    fmt.Println(session(&rp)) // hhhhhhhh
}

// package mock
type RetrieverPosterImpl struct {
    Contents string
}
// 对接口1方法的实现，注意要使用指针接收者，这样对数据的修改才能影响到方法外部
func (rp *RetrieverPosterImpl) Post(url string, form map[string]string) string {
    rp.Contents = form["contents"]
    return "ok"
}

// 对接口2方法的实现
func (rp *RetrieverPosterImpl) Get(url string) string {
    return rp.Contents
}
```

## 函数式编程

函数式编程：函数是一等公民，参数、变量、返回值都可以是函数

1. 匿名函数

匿名函数就是由一个不带函数名的函数声明和函数体组成

```go
func(a, b int, z float64) bool {
    return a*b < int(z)
}
```

匿名函数可以直接赋值给一个变量或者直接执行

```go
f := func(x, y int) int {
    return x + y
}

func (ch chan int) {
    ch <- ACK
} (reply_chan) // 花括号后直接跟参数列表表示函数调用
```

2. 闭包

go语言的匿名函数是一个闭包，闭包就是能够**读取**其他函数内部变量的函数。

**闭包的概念**：是可以包含自由（未绑定到特定对象）变量的代码块，这些变量不在这个代码块内或者任何全局上下文中定义，而是在定义代码块的环境中定义。要执行的代码块（由于自由变量包含在代码块中，所以这些自由变量以及它们引用的对象没有被释放）为自由变量提供绑定的计算环境（作用域）。

**闭包的价值**：闭包的价值在于可以作为函数对象或者匿名函数，对于类型系统而言，这意味着不仅要表示数据还要表示代码。支持闭包的多数语言都将函数作为第一级对象，就是说这些函数可以存储到变量中作为参数传递给其他函数，最重要的是能够被函数动态创建和返回。

Go语言中的闭包同样也会引用到函数外的变量。闭包的实现确保只要闭包还被使用，那么被闭包引用的变量会一直存在。

```go
var j int = 5
a := func() func() {
    var i int = 10
    return func() {
        fmt.Printf("i, j: %d, %d\n", i, j)
    }
}()

a()
j *= 2
a()

// i, j: 10, 5
// i, j: 10, 10
```

## 资源管理和错误处理

关键字 `defer`

一个函数中可以存在多个defer语句，它们遵循先进后出的原则，即最后一个defer语句最先被执行

```go
func CopyFile(dst, src string) (w int64, err error) {
    srcFile, err := os.Open(src)
    if err != nil {
        return
    }
    defer srcFile.Close()

    dstFile, err := os.Create(dstName)
    if err != nil {
        return
    }
    defer dstFile.Close()

    return io.Copy(dstFile, srcFile)
}

// defer 后 也可以用 匿名函数

defer func() {
    ...
}()
```

`error` 在go中的定义：

```go
type error interface {
    Error() string
}
```

我们在做错误处理时，例如在 `file.go` 中：

```go
// If there is an error, it will be of type *PathError.
func OpenFile(name string, flag int, perm FileMode) (*File, error) {
    testlog.Open(name)
    return openFileNolog(name, flag, perm)
}
```

可以这样做，具体到处理某一种错误：

```go
file, err := os.OpenFile(filename, os.O_EXCL|os.O_CREATE, 0666)
if err != nil {
    if pathError, ok := err.(*os.PathError); !ok {
        panic(err)
    } else {
        fmt.Printf("%s, %s, %s\n", pathError.Op, pathError.Path, pathError.Err)
    }
}
defer file.Close()
```

但是错误情况很多的时候，会导致代码冗余，我们这时就需要做统一的错误处理：

```go
// package main
type appHandler func(w http.ResponseWriter, r *http.Request) error

// 包装Handle方法，统一处理错误
func errWrapper(handler appHandler) func(http.ResponseWriter, *http.Request) {
    return func(w http.ResponseWriter, r *http.Request) {
        defer func() {
            if r := recover(); r != nil {
                log.Printf("Panic %v", r)
                http.Error(w,
                    http.StatusText(http.StatusInternalServerError),
                    http.StatusInternalServerError)
            }
        }()
        err := handler(w, r)
        if err != nil {
            log.Printf("Error handling request %s", err.Error())
            // 返回用户自定义的错误信息
            if userError, ok := err.(userError); ok {
                http.Error(w, userError.Message(), http.StatusBadRequest)
                return
            }

            // 返回系统的错误信息
            code := http.StatusOK
            switch {
            case os.IsNotExist(err):
                code = http.StatusNotFound
            case os.IsPermission(err):
                code = http.StatusForbidden
            default:
                code = http.StatusInternalServerError
            }
            http.Error(w, http.StatusText(code), code)
        }
    }
}

// 定义用户错误接口
type userError interface {
    error
    Message() string
}

func main() {
    http.HandleFunc("/list/", errWrapper(HandleFileList))
    http.ListenAndServe(":8888", nil)
}


// package handler
// Handle方法，异常时只需要返回err即可
type userError string

// 实现接口定义的方法
func (e userError) Error() string {
    return e.Message()
}

func (e userError) Message() string {
    return string(e)
}

func HandleFileList(w http.ResponseWriter, r *http.Request) error {
    if strings.Index(r.URL.Path, prefix) != 0 {
        return userError("Path Math start with " + prefix)
    }
    path := r.URL.Path[len(prefix):]
    file, err := os.Open(path)
    if err != nil {
        return err
    }
    defer file.Close()

    all, err := ioutil.ReadAll(file)
    if err != nil {
        return err
    }
    w.Write(all)
    return nil
}
```

关键字 `panic`

- 停止当前函数的执行
- 一直向上返回，执行每一层的defer
- 如果没有遇见 `recover`，程序退出

关键字 `recover`

- 仅在defer调用中使用
- 获取panic的值
- 如果无法处理，可重新panic

```go
func tryRecover() {
    defer func() {
        r := recover()
        if err, ok := r.(error); ok {
            fmt.Println("Error occurred: ", err)
        } else {
            panic(r)
        }
    }
    panic(errors.New("this is an error"))
}
```

## 并发编程

### 协程 Coroutine

- 轻量级线程，用于并发执行任务，在特定的时间只有一个任务在运行，即并非真正地并行
- **是非抢占式多任务处理**，由协程主动交出控制权
- 每个协程都有自己的堆栈和局部变量，每个协程都包含3种运行状态：挂起，运行和停止
- 多个协程可以在一个或多个线程上运行
- **子程序是协程的一个特例**

![协程](https://raw.githubusercontent.com/theChildinus/Note/master/image/20190514220002.png)

非抢占式处理：

```go
for i := 0; i < 10; i++ {
    go func(ii int) {
        for {
            // IO操作，会引起上下文切换，导致协程交出控制权，所以程序正常输出
            fmt.Printf("hello from goroutine %d\n", ii)
        }
    }(i)
}
time.Sleep(time.Millisecond)

var a [10]int
for i := 0; i < 10; i++ {
    go func(ii int) {
        for {
            // 协程不会交出控制权，程序进入死循环
            a[ii]++
            // runtime.Gosched() 手动交出控制权
        }
    }(i)
}
time.Sleep(time.Millisecond)
fmt.Println(a)
```

以下代码会报错：`panic: runtime error: index out of range`，不把 `i` 传入匿名函数中，匿名函数 `i` 为外部的，当 `i` 增长到10，协程中访问数组会导致越界

```go
var a [10]int
for i := 0; i < 10; i++ {
    go func() {
        for {
            a[i]++
        }
    }()
}
time.Sleep(time.Millisecond)
fmt.Println(a)
```

通过命令 `go run -race main.go` 查看数据冲突

```txt
==================
WARNING: DATA RACE
Read at 0x00c420090400 by goroutine 6:     // goroutine 6这时在读数据 i
  main.main.func1()
      /home/kong/goProject/src/myclient/main.go:111 +0x4f

Previous write at 0x00c420090400 by main goroutine: // main goroutine这时在写数据 i
  main.main()
      /home/kong/goProject/src/myclient/main.go:108 +0x11b

Goroutine 6 (running) created at:
  main.main()
      /home/kong/goProject/src/myclient/main.go:109 +0xf1
==================
panic: runtime error: index out of range

goroutine 20 [running]:
main.main.func1(0xc4200d2050, 0xc420090400)
        /home/kong/goProject/src/myclient/main.go:111 +0xcc
created by main.main
        /home/kong/goProject/src/myclient/main.go:109 +0xf2
exit status 2
```

### goroutine

- 任何函数只需加上go就能送给调度器运行
- 不需要在定义时区分是否是异步函数
- 调度器在合适的点进行切换，**可能**切换的点
  - I/O，select，clannel，等待锁
  - 函数调用（有时），runtime.Gosched()
- 使用 `-race` 检测数据冲突

### channel

channel 存在与协程与协程之间

在下面代码中，如果不添加 `time.Sleep(time.Millisecond)` 这条语句，当 `c <- 2` 写入channel时，匿名函数所对应的协程还没来得及输出，chanDemo协程就已经退出了，所以 `2` 不打印

```go
func chanDemo() {
    c := make(chan int)
    go func() {
        for {
            n := <-c
            fmt.Println(n)
        }
    }()
    c <- 1
    c <- 2
    time.Sleep(time.Millisecond)
}
```

创建10个 goroutine 进行消息接收，第一种写法，将 channel 创建放在调用函数中：

```go
func worker(id int, c chan int) {
    for {
        fmt.Printf("Worker %d, received %c\n", id, <-c)
    }
}

func chanDemo() {
    var channels [10]chan int
    for i := 0; i < 10; i++ {
        channels[i] = make(chan int)
        go worker(i, channels[i])
    }

    for i := 0; i < 10; i++ {
        channels[i] <- 'a' + i
    }
    time.Sleep(time.Millisecond)
}
```

第二种写法，将 channel 作为参数返回，还可以将 channel 改为单向channel 或者 缓存channel

接收数据的协程会一直接收数据，直到 chanDemo 函数执行结束，main函数也结束了

```go
func createWorker(id int) chan int {
    c := make(chan int)
    go func() {
        for {
            fmt.Printf("Worker %d, received %c\n", id, <-c)
        }
    }()
    return c
}

func chanDemo() {
    var channels [10]chan int
    for i := 0; i < 10; i++ {
        channels[i] = createWorker(i)
    }

    for i := 0; i < 10; i++ {
        channels[i] <- 'a' + i
    }
    time.Sleep(time.Millisecond)
}
```

接收数据的协程会接收数据直到通道关闭

```go
func worker(id int, c chan int) {
    for v := range c {
        fmt.Printf("Worker: %d, received %c\n", id, v)
    }
    // 另外一种写法
    //for {
    //  v, ok := <-c
    //  if !ok {
    //      break
    //  }
    //  fmt.Printf("Worker: %d, received %c\n", id, v)
    //}
}

// 发送方 close channel
func channelClose() {
    c := make(chan int)
    go worker(0, c)
    c <- 'a'
    c <- 'b'
    c <- 'c'
    c <- 'd'
    close(c)
    time.Sleep(time.Millisecond)
}
```

不要通过共享内存来通信，要通过通信来共享内存

```go
// 定义工作者 done为工作完成时发送
type worker struct {
    in   chan int
    done chan bool
}

func doWork(id int, w worker) {
    for v := range w.in {
        fmt.Printf("Worker: %d, received %c\n", id, v)
        // 当10个 done chan 都已阻塞，均在等待外部读取数据，
        // 这时如果再向 in chan 中写入数据会导致当前协程死锁
        // 将 done chan 放在新的协程中，就不会阻塞当前协程，不影响从 in chan 读取
        go func() { w.done <- true }()
    }
}

func createWorker(id int) worker {
    w := worker{in: make(chan int), done: make(chan bool)}
    go doWork(id, w)
    return w
}

func chanDemo() {
    var workers [10]worker
    for i := 0; i < 10; i++ {
        workers[i] = createWorker(i)
    }

    for i, worker := range workers {
        worker.in <- 'a' + i
    }

    // 如果不将 done chan 放入新的协程中，这里会造成当前协程死锁
    for i, worker := range workers {
        worker.in <- 'A' + i
    }

    for _, worker := range workers {
        // 直到这里才开始从 done chan中读取数据
        <-worker.done
        <-worker.done
    }
}

func main() {
    chanDemo()
}
```

第二种方法 使用 WaitGroup实现同步，类似与java里的 CountDownLatch

```go
type worker struct {
    in   chan int
    done func()
}

func doWork(id int, w worker) {
    for v := range w.in {
        fmt.Printf("Worker: %d, received %c\n", id, v)
        w.done()
    }
}

func createWorker(id int, wg *sync.WaitGroup) worker {
    w := worker{in: make(chan int), done: func() { wg.Done() }}
    go doWork(id, w)
    return w
}

func chanDemo() {
    var workers [10]worker
    var wg sync.WaitGroup
    for i := 0; i < 10; i++ {
        workers[i] = createWorker(i, &wg)
    }

    wg.Add(20)
    for i, worker := range workers {
        worker.in <- 'a' + i
    }

    for i, worker := range workers {
        worker.in <- 'A' + i
    }
    wg.Wait()
}

func main() {
    chanDemo()
}
```

### select

select 最大的限制是每个case语句必须是一个channel操作

```go
func generator() chan int {
    out := make(chan int)
    go func() {
        i := 0
        for {
            time.Sleep(time.Duration(rand.Intn(1500)) * time.Millisecond)
            out <- i
            i++
        }
    }()
    return out
}

func createWorker(id int) chan<- int {
    c := make(chan int)
    go func() {
        for n := range c {
            time.Sleep(time.Second)
            fmt.Println("Worker: ", id, "received: ", n)
        }
    }()
    return c
}

func main() {
    var c1, c2 = generator(), generator()
    var queue []int
    var worker = createWorker(0)
    // 计时器
    tm := time.After(20 * time.Second)
    // 定时器
    tick := time.Tick(time.Second)
    for {
        var activeWorker chan<- int // nil channel
        var activeValue int
        // 有值才初始化
        if len(queue) > 0 {
            activeWorker = worker
            activeValue = queue[0]
        }
        select {
        case v := <-c1:
            queue = append(queue, v)
        case v := <-c2:
            queue = append(queue, v)
        case activeWorker <- activeValue: // nil channel 会阻塞对该channel的所有读写
            queue = queue[1:]
        case <-time.After(800 * time.Millisecond): // 超时
            fmt.Println("timeout")
        case <-tick:
            fmt.Println("len(queue): ", len(queue))
        case <-tm:
            fmt.Println("Bye")
            return
        }
    }
}
```

### 同步机制

Mutex


## ipc & net/http

[net/http](https://github.com/theChildinus/learngo)

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