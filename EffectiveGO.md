# Effective Go

[原文地址](https://go-zh.org/doc/effective_go.html)

## 命名

### 获取器

Go并不对获取器（getter）和设置器（setter）提供自动支持。 你应当自己提供获取器和设置器，通常很值得这样做，但若要将 Get 放到获取器的名字中，既不符合习惯，也没有必要。若你有个名为 owner （小写，未导出）的字段，其获取器应当名为 Owner（大写，可导出）而非 GetOwner。大写字母即为可导出的这种规定为区分方法和字段提供了便利。 若要提供设置器方法，SetOwner 是个不错的选择。两个命名看起来都很合理：

```golang
owner := obj.Owner()
if owner != user {
	obj.SetOwner(user)
}
```

### 接口名

按照约定，只包含一个方法的接口应当以该方法的名称加上-er后缀来命名，如 Reader、Writer、 Formatter、CloseNotifier 等。

诸如此类的命名有很多，遵循它们及其代表的函数名会让事情变得简单。 Read、Write、Close、Flush、 String 等都具有典型的签名和意义。为避免冲突，请不要用这些名称为你的方法命名， 除非你明确知道它们的签名和意义相同。反之，若你的类型实现了的方法， 与一个众所周知的类型的方法拥有相同的含义，那就使用相同的命名。 请将字符串转换方法命名为 String 而非 ToString。

## 控制结构

### if

代码必须防范一系列的错误条件

```golang
f, err := os.Open(name)
if err != nil {
	return err
}
d, err := f.Stat()
if err != nil {
	f.Close()
	return err
}
codeUsing(f, d)
```

### 重新声明与再次赋值

题外话：上一节中最后一个示例展示了短声明 := 如何使用。 调用了 os.Open 的声明为

```golang
f, err := os.Open(name)
```

该语句声明了两个变量 f 和 err。在几行之后，又通过

```golang
d, err := f.Stat()
```

调用了 f.Stat。它看起来似乎是声明了 d 和 err。 注意，尽管两个语句中都出现了 err，但这种重复仍然是合法的：err 在第一条语句中被声明，但在第二条语句中只是被再次赋值罢了。也就是说，调用 f.Stat 使用的是前面已经声明的 err，它只是被重新赋值了而已。

在满足下列条件时，已被声明的变量 v 可出现在:= 声明中：

本次声明与已声明的 v 处于同一作用域中（若 v 已在外层作用域中声明过，则此次声明会创建一个新的变量§），
在初始化中与其类型相应的值才能赋予 v，且
在此次声明中至少另有一个变量是新声明的。
这个特性简直就是纯粹的实用主义体现，它使得我们可以很方面地只使用一个 err 值，例如，在一个相当长的 if-else 语句链中， 你会发现它用得很频繁。

§值得一提的是，即便Go中的函数形参和返回值在词法上处于大括号之外， 但它们的作用域和该函数体仍然相同。

### for
Go的 for 循环类似于C，但却不尽相同。它统一了 for 和 while，不再有 do-while 了。它有三种形式，但只有一种需要分号

```golang
// 如同 C 的 for 循环
for init; condition; post { }

// 如同 C 的 while 循环
for condition { }

// 如同 C 的 for(;;) 循环
for { }
```

对于字符串，range 能够提供更多便利。它能通过解析UTF-8， 将每个独立的Unicode码点分离出来。错误的编码将占用一个字节，并以符文U+FFFD来代替。

```golang
for pos, char := range "日本\x80語" { // \x80 是个非法的UTF-8编码
	fmt.Printf("字符 %#U 始于字节位置 %d\n", char, pos)
}

输出为：

字符 U+65E5 '日' 始于字节位置 0
字符 U+672C '本' 始于字节位置 3
字符 U+FFFD '�' 始于字节位置 6
字符 U+8A9E '語' 始于字节位置 7
```

最后，Go没有逗号操作符，而 ++ 和 -- 为语句而非表达式。 因此，若你想要在 for 中使用多个变量，应采用平行赋值的方式 （因为它会拒绝 ++ 和 --）

```golang
// 反转 a
for i, j := 0, len(a)-1; i < j; i, j = i+1, j-1 {
	a[i], a[j] = a[j], a[i]
}
```

### switch

Go的 switch 比C的更通用。其表达式无需为常量或整数，case 语句会自上而下逐一进行求值直到匹配为止。若 switch 后面没有表达式，它将匹配 true，因此，我们可以将 if-else-if-else 链写成一个 switch，这也更符合Go的风格

```golang
func unhex(c byte) byte {
	switch {
	case '0' <= c && c <= '9':
		return c - '0'
	case 'a' <= c && c <= 'f':
		return c - 'a' + 10
	case 'A' <= c && c <= 'F':
		return c - 'A' + 10
	}
	return 0
}
```

switch 并不会自动下溯，但 case 可通过逗号分隔来列举相同的处理条件

```golang
func shouldEscape(c byte) bool {
	switch c {
	case ' ', '?', '&', '=', '#', '+', '%':
		return true
	}
	return false
}
```

尽管它们在Go中的用法和其它类C语言差不多，但 break 语句可以使 switch 提前终止。不仅是 switch， 有时候也必须打破层层的循环。在Go中，我们只需将标签放置到循环外，然后 “蹦”到那里即可。下面的例子展示了二者的用法。

```golang
Loop:
	for n := 0; n < len(src); n += size {
		switch {
		case src[n] < sizeOne:
			if validateOnly {
				break
			}
			size = 1
			update(src[n])

		case src[n] < sizeTwo:
			if n+1 >= len(src) {
				err = errShortInput
				break Loop
			}
			if validateOnly {
				break
			}
			size = 2
			update(src[n] + src[n+1]<<shift)
		}
	}
```

### 类型选择

switch 也可用于判断接口变量的动态类型。如 类型选择 通过圆括号中的关键字 type 使用类型断言语法。若 switch 在表达式中声明了一个变量，那么该变量的每个子句中都将有该变量对应的类型。

```golang
var t interface{}
t = functionOfSomeType()
switch t := t.(type) {
default:
	fmt.Printf("unexpected type %T", t)       // %T 输出 t 是什么类型
case bool:
	fmt.Printf("boolean %t\n", t)             // t 是 bool 类型
case int:
	fmt.Printf("integer %d\n", t)             // t 是 int 类型
case *bool:
	fmt.Printf("pointer to boolean %t\n", *t) // t 是 *bool 类型
case *int:
	fmt.Printf("pointer to integer %d\n", *t) // t 是 *int 类型
}
```

## 函数

### defer

被推迟函数的实参在 defer 执行时才会被求值。 跟踪例程可针对反跟踪例程设置实参

```golang
func trace(s string) string {
	fmt.Println("entering:", s)
	return s
}

func un(s string) {
	fmt.Println("leaving:", s)
}

func a() {
	defer un(trace("a"))
	fmt.Println("in a")
}

func b() {
	defer un(trace("b"))
	fmt.Println("in b")
	a()
}

func main() {
	b()
}

输出：
entering: b
in b
entering: a
in a
leaving: a
leaving: b
```

## 数据

### new 分配

Go提供了两种分配原语，即内建函数 new 和 make

new 这个用来分配内存的内建函数， 与其它语言中的同名函数不同，它不会初始化内存，只会将内存置零。 也就是说，new(T) 会为类型为 T 的新项分配已置零的内存空间， 并返回它的地址，也就是一个类型为 *T 的值。用Go的术语来说，它返回一个指针，该指针指向新分配的，类型为 T 的零值。

```golang
type SyncedBuffer struct {
	lock    sync.Mutex
	buffer  bytes.Buffer
}
```

SyncedBuffer 类型的值也是在声明时就分配好内存就绪了。后续代码中， p 和 v 无需进一步处理即可正确工作

```golang
p := new(SyncedBuffer)  // type *SyncedBuffer
var v SyncedBuffer      // type  SyncedBuffer
```

### 构造函数和复合字面

返回一个局部变量的地址完全没有问题，这点与C不同。该局部变量对应的数据 在函数返回后依然有效。实际上，每当获取一个复合字面的地址时，都将为一个新的实例分配内存

```golang
func NewFile(fd int, name string) *File {
	if fd < 0 {
		return nil
	}
	f := File{fd, name, nil, 0}
    return &f
    
    // 合并为 return &File{fd, name, nil, 0}
}
```

复合字面的字段必须按顺序全部列出。但如果以 字段:值 对的形式明确地标出元素，初始化字段时就可以按任何顺序出现，未给出的字段值将赋予零值。 因此，我们可以用如下形式

```golang
return &File{fd: fd, name: name}
```

### make 分配

内建函数 make(T, args) 的目的不同于 new(T)。它只用于创建 slice、map和channel，并返回类型为 T（而非 *T）的一个**已初始化** （而**非置零**）的值

出现这种用差异的原因在于，这三种类型本质上为引用数据类型，它们在使用前必须初始化

```golang
var p *[]int = new([]int)       // 分配切片结构；*p == nil；基本没用
var v  []int = make([]int, 100) // 切片 v 现在引用了一个具有 100 个 int 元素的新数组

// 没必要的复杂：
var p *[]int = new([]int)
*p = make([]int, 100, 100)

// 习惯用法：
v := make([]int, 100)
```

请记住，make 只适用于映射、切片和信道且不返回指针。若要获得明确的指针， 请使用 new 分配内存

### 数组

内存布局时，数组是非常有用的，有时还能避免过多的内存分配， 但它们主要用作切片的构件

- 数组是值。将一个数组赋予另一个数组会复制其所有元素。
- 特别地，若将某个数组传入某个函数，它将接收到该数组的一份副本而非指针。
- 数组的大小是其类型的一部分。类型 [10]int 和 [20]int 是不同的。

数组为值的属性很有用，但代价高昂；若你想要C那样的行为和效率，你可以传递一个指向该数组的指针。

```golang
func Sum(a *[3]float64) (sum float64) {
	for _, v := range *a {
		sum += v
	}
	return
}

array := [...]float64{7.0, 8.5, 9.1}
x := Sum(&array)  // 注意显式的取址操作
```

### 切片

切片通过对数组进行封装，为数据序列提供了更通用、强大而方便的接口。 除了矩阵变换这类需要明确维度的情况外，Go中的大部分数组编程都是通过切片来完成的。

切片保存了对底层数组的引用，若你将某个切片赋予另一个切片，它们会引用同一个数组。 若某个函数将一个切片作为参数传入，则它对该切片元素的修改对调用者而言同样可见， 这可以理解为传递了底层数组的指针。因此，Read 函数可接受一个切片实参 而非一个指针和一个计数；切片的长度决定了可读取数据的上限

```golang
func (file *File) Read(buf []byte) (n int, err error)
```

### 二维切片

Go的数组和切片都是一维的。要创建等价的二维数组或切片，就必须定义一个数组的数组， 或切片的切片

```golang
type Transform [3][3]float64  // 一个 3x3 的数组，其实是包含多个数组的一个数组。
type LinesOfText [][]byte     // 包含多个字节切片的一个切片。
```

分配二维数组，一种是独立分配每一个切片

```golang
// 分配顶层切片。
picture := make([][]uint8, YSize) // 每 y 个单元一行。
// 遍历行，为每一行都分配切片
for i := range picture {
	picture[i] = make([]uint8, XSize)
}
```

另一种是只分配一个数组，将各个切片都指向它

```golang
// 分配顶层切片，和前面一样。
picture := make([][]uint8, YSize) // 每 y 个单元一行。
// 分配一个大的切片来保存所有像素
pixels := make([]uint8, XSize*YSize) // 拥有类型 []uint8，尽管图片是 [][]uint8.
// 遍历行，从剩余像素切片的前面切出每行来。
for i := range picture {
	picture[i], pixels = pixels[:XSize], pixels[XSize:]
}
```

### 映射

映射是方便而强大的内建数据结构，它可以关联不同类型的值。其键可以是任何相等性操作符支持的类型， 如整数、浮点数、复数、字符串、指针、接口（只要其动态类型支持相等性判断）、结构以及数组。 切片不能用作映射键，因为它们的相等性还未定义，与切片一样，映射也是引用类型。 若将映射传入函数中，并更改了该映射的内容，则此修改对调用者同样可见

若试图通过映射中不存在的键来取值，就会返回与该映射中项的类型对应的零值

下面的例子，若 tz 存在， seconds 就会被赋予适当的值，且 ok 会被置为 true； 若不存在，seconds 则会被置为零，而 ok 会被置为 false

```golang
func offset(tz string) int {
	if seconds, ok := timeZone[tz]; ok {
		return seconds
	}
	log.Println("unknown time zone:", tz)
	return 0
}
```

要删除映射中的某项，可使用内建函数 delete，它以映射及要被删除的键为实参。 即便对应的键不在该映射中，此操作也是安全的

```golang
delete(timeZone, "PDT")
```

### 打印

字符串函数（Sprintf 等）会返回一个字符串，而非填充给定的缓冲区

以下示例中各行产生的输出都是一样的：

```golang
fmt.Printf("Hello %d\n", 23)
fmt.Fprint(os.Stdout, "Hello ", 23, "\n")
fmt.Println("Hello", 23)
fmt.Println(fmt.Sprint("Hello ", 23))
```

`%v` 写法

```golang
type T struct {
	a int
	b float64
	c string
}
t := &T{ 7, -2.35, "abc\tdef" }
fmt.Printf("%v\n", t)
fmt.Printf("%+v\n", t)
fmt.Printf("%#v\n", t)

输出：
&{7 -2.35 abc   def}
&{a:7 b:-2.35 c:abc     def}
&main.T{a:7, b:-2.35, c:"abc\tdef"}
```

我们的 String 方法也可调用 Sprintf， 因为打印例程可以完全重入并按这种方式封装。不过要理解这种方式，还有一个重要的细节： 请勿通过调用 Sprintf 来构造 String 方法，因为它会无限递归你的的 String 方法。

```golang
type MyString string

func (m MyString) String() string {
	return fmt.Sprintf("MyString=%s", m) // 错误：会无限递归
}
```

解决办法：将该实参转换为基本的字符串类型，因为基本类型没有这个方法

```golang
type MyString string
func (m MyString) String() string {
	return fmt.Sprintf("MyString=%s", string(m)) // 可以：注意转换
}
```

### append 函数

```golang
// 添加单个元素
x := []int{1,2,3}
x = append(x, 4, 5, 6)

// 追加一个切片
x := []int{1,2,3}
y := []int{4,5,6}
x = append(x, y...)
```

## 初始化

### 常量

Go中的常量就是不变量。它们在编译时创建，即便它们可能是函数中定义的局部变量, 定义它们的表达式必须也是可被编译器求值的常量表达式。例如 1<<3 就是一个常量表达式，而 math.Sin(math.Pi/4) 则不是，因为对 math.Sin 的函数调用在运行时才会发生

```golang
type ByteSize float64

const (
    // 通过赋予空白标识符来忽略第一个值
    _           = iota // ignore first value by assigning to blank identifier
    KB ByteSize = 1 << (10 * iota)
    MB
    GB
    TB
    PB
    EB
    ZB
    YB
)
```

由于可将 String 之类的方法附加在用户定义的类型上， 因此它就为打印时自动格式化任意值提供了可能性，即便是作为一个通用类型的一部分。 尽管你常常会看到这种技术应用于结构体，但它对于像 ByteSize 之类的浮点数标量等类型也是有用的。

```golang
func (b ByteSize) String() string {
    switch {
    case b >= YB:
        return fmt.Sprintf("%.2fYB", b/YB)
    case b >= ZB:
        return fmt.Sprintf("%.2fZB", b/ZB)
    case b >= EB:
        return fmt.Sprintf("%.2fEB", b/EB)
    case b >= PB:
        return fmt.Sprintf("%.2fPB", b/PB)
    case b >= TB:
        return fmt.Sprintf("%.2fTB", b/TB)
    case b >= GB:
        return fmt.Sprintf("%.2fGB", b/GB)
    case b >= MB:
        return fmt.Sprintf("%.2fMB", b/MB)
    case b >= KB:
        return fmt.Sprintf("%.2fKB", b/KB)
    }
    return fmt.Sprintf("%.2fB", b)
}
```

在这里用 Sprintf 实现 ByteSize 的 String 方法很安全（不会无限递归），这倒不是因为类型转换，而是它以 `%f` 调用了 Sprintf，它并不是一种字符串格式：**Sprintf 只会在它需要字符串时才调用 String 方法**，而 `%f` 需要一个浮点数值。

### 变量

变量的初始化与常量类似，但其初始值也可以是在运行时才被计算的一般表达式。

```golang
var (
	home   = os.Getenv("HOME")
	user   = os.Getenv("USER")
	gopath = os.Getenv("GOPATH")
)
```

### init 函数

每个源文件都可以通过定义自己的无参数 init 函数来设置一些必要的状态，只有该包中的所有变量声明都通过它们的初始化器求值后 init 才会被调用， 而那些 init 只有在所有已导入的包都被初始化后才会被求值，init 函数还可以被用在程序真正开始执行前，检查或校正程序的状态

## 方法

### 指针 vs 值

**我们可以为任何已命名的类型（除了指针或接口）定义方法**；接收者可不必为结构体，为此，我们首先要声明一个已命名的类型来绑定该方法， 然后使该方法的接收者成为该类型的值

```golang
// 之前的方法定义
func Append(slice, data []byte) []byte {
	...
}
```

```golang
type ByteSlice []byte

func (slice ByteSlice) Append(data []byte) []byte {
	// 主体和前面相同。
}
```

我们仍然需要该方法返回更新后的切片。为了消除这种不便，我们可通过重新定义该方法， 将一个指向 ByteSlice 的指针作为该方法的接收者， 这样该方法就能重写调用者提供的切片了

```golang
func (p *ByteSlice) Append(data []byte) {
	slice := *p
	// 主体和前面相同，但没有 return。
	*p = slice
}
```

若我们将函数修改为与标准 Write 类似的方法，就像这样，

```golang
func (p *ByteSlice) Write(data []byte) (n int, err error) {
	slice := *p
	// 依旧和前面相同。
	*p = slice
	return len(data), nil
}

// 类型 *ByteSlice 就满足了标准的 io.Writer 接口，这将非常实用。 例如，我们可以通过打印将内容写入
var b ByteSlice
fmt.Fprintf(&b, "This hour has %d days\n", 7)
```

我们将 ByteSlice 的地址传入，因为只有 *ByteSlice 才满足 io.Writer。以指针或值为接收者的区别在于：**值方法可通过指针和值调用， 而指针方法只能通过指针来调用。**

之所以会有这条规则是因为指针方法可以修改接收者；通过值调用它们会导致方法接收到该值的副本， 因此任何修改都将被丢弃，因此该语言不允许这种错误

不过有个方便的例外：若该值是可寻址的， 那么该语言就会自动插入取址操作符来对付一般的通过值调用的指针方法。在我们的例子中，变量 b 是可寻址的，因此我们只需通过 b.Write 来调用它的 Write 方法，编译器会将它重写为 (&b).Write。

## 接口和其他类型

### 接口

每种类型都能实现多个接口。例如一个实现了 sort.Interface 接口的集合就可通过 sort 包中的例程进行排序。该接口包括 Len()、Less(i, j int) bool 以及 Swap(i, j int)，另外，该集合仍然可以有一个自定义的格式化器。 以下特意构建的例子 Sequence 就同时满足这两种情况。

```golang
type Sequence []int

// Methods required by sort.Interface.
// sort.Interface 所需的方法。
func (s Sequence) Len() int {
    return len(s)
}
func (s Sequence) Less(i, j int) bool {
    return s[i] < s[j]
}
func (s Sequence) Swap(i, j int) {
    s[i], s[j] = s[j], s[i]
}

// Method for printing - sorts the elements before printing.
// 用于打印的方法 - 在打印前对元素进行排序。
func (s Sequence) String() string {
    sort.Sort(s)
    str := "["
    for i, elem := range s {
        if i > 0 {
            str += " "
        }
        str += fmt.Sprint(elem)
    }
    return str + "]"
}
```

### 接口转换与类型断言

类型选择是类型转换的一种形式：它接受一个接口，在选择 （switch）中根据其判断选择对应的情况（case）， 并在某种意义上将其转换为该种类型

```golang
type Stringer interface {
	String() string
}

var value interface{} // 调用者提供的值。
switch str := value.(type) {
case string:
	return str
case Stringer:
	return str.String()
}

// 还可以写成
if str, ok := value.(string); ok {
	return str
} else if str, ok := value.(Stringer); ok {
	return str.String()
}
```

第一种情况获取具体的值，第二种将该接口转换为另一个接口。这种方式对于混合类型来说非常完美。

如果我们只关心一种类型，并且防止字符串为空导致的错误崩溃

```golang
str, ok := value.(string)
if ok {
	fmt.Printf("字符串值为 %q\n", str)
} else {
	fmt.Printf("该值非字符串\n")
}
```

### 通用性

若某种现有的类型仅实现了一个接口，且除此之外并无可导出的方法，则该类型本身就无需导出。 仅导出该接口能让我们更专注于其行为而非实现，在这种情况下，构造函数应当返回一个接口值而非实现的类型，例如在 hash 库中，crc32.NewIEEE 和 adler32.New 都返回接口类型 hash.Hash32

同样的方式能将 crypto 包中多种联系在一起的流密码算法与块密码算法分开。 crypto/cipher 包中的 Block 接口指定了块密码算法的行为， 它为单独的数据块提供加密。接着，和 bufio 包类似，任何实现了该接口的密码包都能被用于构造以 Stream 为接口表示的流密码，而无需知道块加密的细节

```golang
type Block interface {
	BlockSize() int
	Encrypt(src, dst []byte)
	Decrypt(src, dst []byte)
}

type Stream interface {
	XORKeyStream(dst, src []byte)
}
```

这是计数器模式CTR流的定义，它将块加密改为流加密，注意块加密的细节已被抽象化了。

```golang
// NewCTR 返回一个 Stream，其加密/解密使用计数器模式中给定的 Block 进行。
// iv 的长度必须与 Block 的块大小相同。
func NewCTR(block Block, iv []byte) Stream
```

NewCTR 的应用并不仅限于特定的加密算法和数据源，它适用于任何对 Block 接口和 Stream 的实现。因为它们返回接口值， 所以用其它加密模式来代替CTR只需做局部的更改。构造函数的调用过程必须被修改， 但由于其周围的代码只能将它看做 Stream，因此它们不会注意到其中的区别。

### 接口和方法

由于几乎任何类型都能添加方法，**因此几乎任何类型都能满足一个接口**。一个很直观的例子就是 http 包中定义的 Handler 接口。任何实现了 Handler 的对象都能够处理HTTP请求。

```golang
type Handler interface {
	ServeHTTP(ResponseWriter, *Request)
}
```

ResponseWriter 接口提供了对方法的访问，这些方法需要响应客户端的请求。 由于**这些方法包含了标准的 Write 方法**，因此 http.ResponseWriter 可用于任何 io.Writer 适用的场景。Request 结构体包含已解析的客户端请求。

```golang
// 简单的计数器服务。
type Counter struct {
	n int
}

func (ctr *Counter) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	ctr.n++
	fmt.Fprintf(w, "counter = %d\n", ctr.n)
}
```

也可写为：

```golang
type Counter int

func (ctr *Counter) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	*ctr++
	fmt.Fprintf(w, "counter = %d\n", *ctr)
}
```

将这样一个服务器添加到URL树的一个节点上

```golang
import "net/http"
...
ctr := new(Counter)
http.Handle("/counter", ctr)
```

当页面被访问时，怎么通知你的程序去更新一个内部状态呢？为web页面绑定一个信道

```golang
// 每次浏览该信道都会发送一个提醒。
// （可能需要带缓冲的信道。）
type Chan chan *http.Request

func (ch Chan) ServeHTTP(w http.ResponseWriter, req *http.Request) {
	ch <- req
	fmt.Fprint(w, "notification sent")
}
```

既然我们可以为除指针和接口以外的任何类型定义方法，同样也能为一个函数写一个方法。 http 包包含以下代码：

```golang
// HandlerFunc 类型是一个适配器，它允许将普通函数用做HTTP处理程序。
// 若 f 是个具有适当签名的函数，HandlerFunc(f) 就是个调用 f 的处理程序对象。
type HandlerFunc func(ResponseWriter, *Request)

// ServeHTTP calls f(c, req).
func (f HandlerFunc) ServeHTTP(w ResponseWriter, req *Request) {
	f(w, req)
}
```

HandlerFunc 是个具有 ServeHTTP 方法的类型， 因此该类型的值就能处理HTTP请求。我们来看看该方法的实现：接收者是一个函数 f，而该方法调用 f，区别在于接收者变成了一个信道，而方法通过该信道发送消息。

为了将 ArgServer 实现成HTTP服务器，首先我们得让它拥有合适的签名。

```golang
// 实参服务器。
func ArgServer(w http.ResponseWriter, req *http.Request) {
	fmt.Fprintln(w, os.Args)
}
```

ArgServer 和 HandlerFunc 现在拥有了相同的签名， 因此我们可将其**转换为这种类型以访问它的方法**，就像我们将 Sequence 转换为 IntSlice 以访问 IntSlice.Sort 那样。 建立代码非常简单

```golang
http.Handle("/args", http.HandlerFunc(ArgServer))
```

在本节中，我们通过一个结构体，一个整数，一个信道和一个函数，建立了一个HTTP服务器， 这一切都是因为接口只是方法的集和，而几乎任何类型都能定义方法

## 空白标识符

### 多重赋值中的空白标识符

for range 循环中对空表标识符的用法是一种具体情况，更一般的情况即为多重赋值。

若某次赋值需要匹配多个左值，但其中某个变量不会被程序使用， 那么用空白标识符来代替该变量可避免创建无用的变量，并能清楚地表明该值将被丢弃

### 未使用的导入和变量

要让编译器停止关于未使用导入的提示，需要空白标识符来引用已导入包中的符号。

```golang
package main

import (
    "fmt"
    "io"
    "log"
    "os"
)

var _ = fmt.Printf // For debugging; delete when done. // 用于调试，结束时删除。
var _ io.Reader    // For debugging; delete when done. // 用于调试，结束时删除。

func main() {
    fd, err := os.Open("test.go")
    if err != nil {
        log.Fatal(err)
    }
    // TODO: use fd.
    _ = fd
}
```

我们应在导入并加以注释后，再使全局声明导入错误静默，这样可以让它们更易找到， 并作为以后清理它的提醒。

### 为副作用导入

在 net/http/pprof 包的 init 函数中记录了HTTP处理程序的调试信息，我们只是需要该处理程序的记录和通过web访问数据

```golang
import _ "net/http/pprof"
```

### 接口检查

若只需要判断某个类型是否实现了某个接口，而不需要实际使用接口本身，就使用空白标识符来忽略类型断言的值：

```golang
if _, ok := val.(json.Marshaler); ok {
	fmt.Printf("value %v of type %T implements json.Marshaler\n", val, val)
}
```

## 内嵌

接口内嵌：Go语言能够在结构或接口中嵌入类型来 借用 实现的各个部分

```golang
type Reader interface {
	Read(p []byte) (n int, err error)
}

type Writer interface {
	Write(p []byte) (n int, err error)
}
```

io 包也导出了一些其它接口，以此来阐明对象所需实现的方法。 例如 io.ReadWriter 就是个包含 Read 和 Write 的接口。我们可以通过显示地列出这两个方法来指明 io.ReadWriter， 但通过将这两个接口内嵌到新的接口中显然更容易且更具启发性

```golang
type ReadWriter interface {
	Reader
	Writer
}
```

它是内嵌接口的联合体 （它们必须是不相交的方法集）。**只有接口能被嵌入到接口中**

同样的基本想法可以应用在结构体中，但其意义更加深远。bufio 包中有 bufio.Reader 和 bufio.Writer 这两个结构体类型， 它们每一个都实现了与 io 包中相同意义的接口

```golang
// ReadWriter 存储了指向 Reader 和 Writer 的指针。
// 它实现了 io.ReadWriter。
type ReadWriter struct {
	*Reader  // *bufio.Reader
	*Writer  // *bufio.Writer
}
```

 bufio.ReadWriter 不仅包括 bufio.Reader 和 bufio.Writer 的方法，它还同时满足下列三个接口： io.Reader、io.Writer 以及 io.ReadWriter。

 还有种区分内嵌与子类的重要手段。当内嵌一个类型时，该类型的方法会成为外部类型的方法， 但当它们被调用时，该方法的接收者是内部类型，而非外部的

 在我们的例子中，当 bufio.ReadWriter 的 Read 方法被调用时， 它与之前写的转发方法具有同样的效果；接收者是 ReadWriter 的 reader 字段，而非 ReadWriter 本身。

## 并发

### 通过通信共享内存

不要通过共享内存来通信，而应通过通信来共享内存

```golang
func Announce(message string, delay time.Duration) {
	go func() {
		time.Sleep(delay)
		fmt.Println(message)
	}()  // 注意括号 - 必须调用该函数。
}
```

在Go中，函数字面都是闭包：其实现在保证了函数内引用变量的生命周期与函数的活动时间相同。

### 信道

信道与映射一样，也需要通过make来分配内存，其结果值充当了对底层数据结构的引用。

```golang
ci := make(chan int)            // 整数类型的无缓冲信道
cj := make(chan int, 0)         // 整数类型的无缓冲信道
cs := make(chan *os.File, 100)  // 指向文件指针的带缓冲信道
```

无缓冲信道在通信时会同步交换数据，它能确保（两个 goroutine）计算处于确定状态

在上一节中，我们在后台启动了排序操作。 信道使得启动的 goroutine 等待排序完成

```golang
c := make(chan int)  // 分配一个信道
// 在 goroutine 中启动排序。当它完成后，在信道上发送信号。
go func() {
	list.Sort()
	c <- 1  // 发送信号，什么值无所谓。
}()
doSomethingForAWhile()
<-c   // 等待排序结束，丢弃发来的值。
```

接收者在收到数据前会一直阻塞。若信道是不带缓冲的，那么在接收者收到值前， 发送者会一直阻塞；若信道是带缓冲的，则发送者直到值被复制到缓冲区才开始阻塞； 若缓冲区已满，发送者会一直等待直到某个接收者取出一个值为止。

带缓冲的信道可被用作信号量，例如限制吞吐量。在此例中，进入的请求会被传递给 handle，它从信道中接收值，处理请求后将值发回该信道中，以便让该 “信号量” 准备迎接下一次请求。信道缓冲区的容量决定了同时调用 process 的数量上限。

```golang
var sem = make(chan int, MaxOutstanding)

func handle(r *Request) {
	sem <- 1 // 等待活动队列清空。
	process(r)  // 可能需要很长时间。
	<-sem    // 完成；使下一个请求可以运行。
}

func Serve(queue chan *Request) {
	for {
		req := <-queue
		go handle(req)  // 无需等待 handle 结束。
	}
}
```

一旦有 MaxOutstanding 个处理器进入运行状态，其他的所有处理器都会在试图发送值到信道缓冲区的时候阻塞，直到某个处理器完成处理并从缓冲区取回一个值为止

### 信道中的信道