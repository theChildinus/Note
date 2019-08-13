# GO BLOG

## 值接收者和指针接收者区别

### 方法

```golang
type Person struct {
    age int
}

func (p Person) howOld() int {
    return p.age
}

func (p *Person) growUp() {
    p.age += 1
}

func main() {
    // qcrao 是值类型
    qcrao := Person{age: 18}
    // 值类型 调用接收者也是值类型的方法
    fmt.Println(qcrao.howOld())
    // 值类型 调用接收者是指针类型的方法
    qcrao.growUp()
    fmt.Println(qcrao.howOld())

    // ----------------------

    // stefno 是指针类型
    stefno := &Person{age: 100}
    // 指针类型 调用接收者是值类型的方法
    fmt.Println(stefno.howOld())
    // 指针类型 调用接收者也是指针类型的方法
    stefno.growUp()
    fmt.Println(stefno.howOld())
}
```

调用了 `growUp` 函数后，不管调用者是值类型还是指针类型，它的 `Age` 值都改变了。

| | 值接收者 | 指针接收者 |
| --- | --- | --- |
| 值类型调用者| 方法会使用调用者的一个副本，类似于“传值” | 使用值的引用来调用方法，上例中，`qcrao.growUp()` 实际上是 `(&qcrao).growUp()` |
| 指针类型调用者| 指针被解引用为值，上例中，`stefno.howOld()` 实际上是 `(*stefno).howOld()` | 实际上也是“传值”，方法里的操作会影响到调用者，类似于指针传参，拷贝了一份指针 |

### 接收者

语法糖：实现了接收者是值类型的方法，相当于自动实现了接收者是指针类型的方法；而实现了接收者是指针类型的方法，不会自动生成对应接收者是值类型的方法。

理解：当实现了一个接收者是值类型的方法，就可以自动生成一个接收者是对应指针类型的方法，因为两者都不会影响接收者。但是，当实现了一个接收者是指针类型的方法，如果此时自动生成一个接收者是值类型的方法，原本期望对接收者的改变（通过指针实现），现在无法实现，因为值类型会产生一个拷贝，不会真正影响调用者。

### 两者分别在何时使用

如果方法的接收者是值类型，无论调用者是对象还是对象指针，修改的都是对象的副本，不影响调用者；如果方法的接收者是指针类型，则调用者修改的是指针指向的对象本身。

使用指针作为方法接收者的理由：

1. 方法能够修改接收者指向的值
2. 避免在每次调用方法时复制该值，在值的类型为大型结构体时这样做会更加高效

## GO 反射

### 什么是反射？

反射是指计算机程序在运行时（Run time）可以访问、检测和修改它本身状态或行为的一种能力。用比喻来说，反射就是程序在运行的时候能够“观察”并且修改自己的行为。

Go 语言提供了一种机制在运行时更新变量和检查它们的值、调用它们的方法，但是在编译时并不知道这些变量的具体类型，这称为反射机制。

### 为什么要用反射

使用反射的两个场景

1. 有时你需要编写一个函数，但是并不知道传给你的参数类型是什么，可能是没约定好；也可能是传入的类型很多，这些类型并不能统一表示。这时反射就会用的上了；
2. 有时候需要根据某些条件决定调用哪个函数，比如根据用户的输入来决定。这时就需要对**函数和函数的参数进行反射**，在运行期间动态地执行函数。

### 反射是如何实现的

`interface{}` 当向接口变量赋予一个实体类型的时候，接口会存储实体的类型信息，反射就是通过接口的类型信息实现的，反射建立在类型的基础上，Go 语言在 reflect 包里定义了各种类型，实现了反射的各种函数，通过它们可以在运行时检测类型的信息、改变类型的值。

#### types 和 interface

GO 语言中，每个变量都有一个静态类型，在编译阶段就确定了，注意，**这个类型是声明时的类型，不是底层数据类型**

接口变量可以存储任何实现了接口定义的所有方法的变量

interface{} 类型定义，其中 `itab` 由具体类型 `_type` 以及 `interfacetype` 组成，`_type` 表示具体类型，而 `interfacetype` 表示具体类型实现的接口类型

![interface类型](images/15631050373170.png)

Go 语言中最常见的就是 Reader 和 Writer 接口：

```golang
type Reader interface {
    Read(p []byte) (n int, err error)
}
type Writer interface {
    Write(p []byte) (n int, err error)
}
```

```golang
var r io.Reader
tty, err := os.OpenFile("/Users/qcrao/Desktop/test", os.O_RDWR, 0)
if err != nil {    
    return nil, err
}
r = tty
```

先声明 `r` 的类型是 `io.Reader`，注意，这是 `r` 的静态类型，此时它的动态类型为 `nil`，并且它的动态值也是 `nil`。

之后，`r = tty` 这一语句，将 `r` 的动态类型变成 `*os.File`，动态值则变成非空，表示打开的文件对象。这时，`r` 可以用 `<value, type>` 对来表示为：`<tty, *os.File>`

![底层结构1](images/15630967918763.png)

注意看上图，此时虽然 `fun` 所指向的函数只有一个 `Read` 函数，其实 `*os.File` 还包含 `Write` 函数，也就是说 `*os.File` 其实还实现了 `io.Writer` 接口。因此下面的断言语句可以执行：

```golang
var w io.Writer
w = r.(io.Writer)
```

这样，`w` 也可以表示成 `<tty, *os.File>`，仅管它和 `r` 一样，但是 `w` 可**调用的函数取决于它的静态类型** `io.Writer`，也就是说它只能有这样的调用形式： `w.Write()` 。`w` 的内存形式如下图：

![底层结构2](images/15631045003291.png)

最后再来一个赋值：

```golang
var empty interface{}
empty = w
```

由于 empty 是一个空接口，因此所有的类型都实现了它，w 可以直接赋给它，不需要执行断言操作。

![底层结构2](images/15631043703718.png)

### 反射的基本函数

reflect 包里面包含了一个接口和一个结构体，即 `reflect.Type` 和 `reflect.Value`，它们提供很多函数来获取存储在接口里面的信息

`reflect.Type` 主要提供关于类型相关的信息，所以它和 `_type` 关联紧密，`reflect.Value` 则结合 `_type` 和 `data` 两者

reflect包提供了两个基础的关于反射的函数来获取上述的接口和结构体

```golang
func TypeOf(i interface{}) Type // 提取接口中值的类型信息
func ValueOf(i interface{}) Value // 提供实际变量的各种信息
```

总结一下：`TypeOf()` 函数返回一个接口，这个接口定义了一系列方法，利用这些方法可以获取关于类型的所有信息； `ValueOf()` 函数返回一个结构体变量，包含类型信息以及实际值。

![reflect](images/15631071252641.png)

### 反射的三大定律

1. Reflection goes from interface value to reflection object. 反射是一种检测存储在 `interface` 中的类型和值机制。这可以通过 `TypeOf` 函数和 `ValueOf` 函数得到
2. Reflection goes from reflection object to interface value. 它将 `ValueOf` 的返回值通过 `Interface()` 函数反向转变成 `interface` 变量
3. To modify a reflection object, the value must be settable. 如果想要操作原变量，反射变量 `Value` 必须要 hold 住原变量的地址才行

举个例子：

```golang
var x float64 = 3.4
v := reflect.ValueOf(x)
v.SetFloat(7.1) // Error: will panic
```

执行上面的代码会产生 panic，原因是反射变量 v 不能代表 x 本身，为什么？因为调用 `reflect.ValueOf(x)` 这一行代码的时候，传入的参数在函数内部只是一个拷贝，是值传递，所以 v 代表的只是 x 的一个拷贝，因此对 v 进行操作是被禁止的。

可设置是反射变量 `Value` 的一个性质，但不是所有的 `Value` 都是可被设置的

```golang
var x float64 = 3.4
p := reflect.ValueOf(&x)
fmt.Println("type of p:", p.Type())
fmt.Println("settability of p:", p.CanSet())

// type of p: *float64
// settability of p: false
```

p 还不是代表 x，`p.Elem()` 才真正代表 x，这样就可以真正操作 x 了

```golang
v := p.Elem()
v.SetFloat(7.1)
fmt.Println(v.Interface()) // 7.1
fmt.Println(x) // 7.1
```

## tcmalloc

### tcmalloc 是什么

tcmalloc是google推出的一种内存分配器，常见的内存分配器还有glibc的ptmalloc和google的jemalloc。相比于ptmalloc，tcmalloc性能更好，特别适用于高并发场景。

### tcmalloc 具体策略

tcmalloc分配的内存主要来自两个地方：全局缓存堆和进程的私有缓存。对于一些小容量的内存申请试用进程的私有缓存，私有缓存不足的时候可以再从全局缓存申请一部分作为私有缓存。对于大容量的内存申请则需要从全局缓存中进行申请。而大小容量的边界就是32k。缓存的组织方式是一个单链表数组，数组的每个元素是一个单链表，链表中的每个元素具有相同的大小。