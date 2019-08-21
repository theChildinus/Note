# GO Web 编程

## 第一章 GO 与 Web 应用

### 1.1 使用GO语言构建Web应用

大规模可拓展的Web应用通常需要具备一下特质：

1. 可拓展
2. 模块化
3. 可维护
4. 高性能

### 1.2 Web应用的工作原理

从纯粹且狭隘的角度看，Web应用应该是这样的计算机程序：它对客户端发送的HTTP请求作出响应，并通过HTTP响应将HTML回传至客户端

应用是一个与用户进行互动并帮助用户执行指定活动的软件程序

一个程序满足以下两个条件，我们就可以把它看作是一个Web应用：

- 这个程序必须向发送命令请求的客户端返回HTML，而客户端则会向用户展示渲染后的HTML
- 这个程序在向客户端传送数据时必须使用HTTP协议

在此基础上，如果一个程序不是向用户渲染并展示HTML，而是向其他程序返回某些非HTML格式的数据，那么这个程序就是一个为其他程序提供服务的Web服务

### 1.3 HTTP简介

HTTP 是一种无状态、由文本构成的请求-响应协议，这种协议使用的是客户端-服务器计算模型

Web 应用是为了通过HTTP向用户发送定制的动态内容而诞生的

### 1.5 HTTP请求

- 请求行(request-line)
- 零个或任意多个请求首部(header)
- 一个空行
- 可选的报文主体(body)

#### 请求方法

- HTTP 0.9 GET
- HTTP 1.0 + POST HEAD
- HTTP 1.1 + PUT DELETE OPTIONS TRACE CONNECT

具体说来：

- GET - 命令服务器返回指定的资源
- HEAD - 与GET方法的作用类似，这个方法用于不获取报文主体的情况下，取得响应的头部
- POST - 命令服务器将报文主体中的数据传输给URI指定的资源，至于服务器具体会对这些资源做什么动作则取决于服务器本身
- PUT - 命令服务器将报文主体中的数据设置为URI指定的资源，存在则替换，不存在则新建
- DELETE - 命令服务器删除URI指定的资源
- TRACE - 命令服务器返回请求本身，通过这个办法，客户端可以知道介于它和服务器之间的其他服务器是如何处理请求的
- OPTIONS - 命令服务器返回它支持的HTTP方法列表
- CONNECT - 命令服务器与客户端建立一个网络链接，通常用于设置SSL隧道以开启HTTPS功能
- PATCH - 命令服务器使用报文主体中的数据对URI指定的资源进行修改

GET、HEAD、OPTIONS和TRACE 它们都是安全的方法，POST、PUT、DELETE都是不安全的方法

安全的方法天生就是幂等的，PUT和DELETE虽然不安全但是是幂等的，POST既不安全也不幂等

#### 请求首部

HTTP请求方法定义了发送请求的客户端想要执行的动作，而HTTP请求的首部则记录了与请求本身以及客户端有关的信息

宿主（Host）首部字段是 HTTP 1.1 唯一强制要求的首部，根据请求使用的方法不同，如果请求的报文中包含有可选的主体，那么请求的首部还需要带有内容长度（Content-Length）字段或传输编码（Transfer-Encoding）字段

| 首部字段       | 作用描述                                                     |
| -------------- | :----------------------------------------------------------- |
| Accept         | 客户端在 HTTP 响应中能够接受的内容类型                       |
| Accept-Charset | 客户端要求服务器使用的字符集编码                             |
| Authorization  | 这个首部用于向服务器发送基本的身份验证证书                   |
| Cookie         | 客户端应该在这个首部中把服务器之前设置的所有cookie回传给服务器，比如说，如果服务器之前在浏览器上设置了时那个cookie，那么Cookie首部字段将在一个字符串里包含这三个cookie，并使用分号对这些cookie进行分隔 |
| Content-Length | 请求主体的字段长度                                           |
| Content-Type   | 当请求包含主体的时候，这个首部用于记录主体内容的类型，在发送POST和PUT请求时，内容的类型默认为`x-www-form-urlen-coded`，但是在上传文件时，内容类型应该设置为`multipart/form-data` |
| Host           | 服务器的名字以及端口号                                       |
| Referrer       | 发起请求的页面所在的地址                                     |
| User-Agent     | 对发起请求的客户端进行描述                                   |

### 1.6 HTTP响应

- 一个状态行
- 零个或任意数量的响应首部
- 一个空行
- 一个可选的报文主体

#### 响应状态码

| 状态码类型 | 作用描述                                                     |
| ---------- | ------------------------------------------------------------ |
| 1XX        | 情报状态码，服务器通过这些来告诉客户端，自己已经接收到了客户端发送的请求，并且已经对请求进行了处理 |
| 2XX        | 成功状态码                                                   |
| 3XX        | 重定向状态码，服务器已经接收到了客户端发送的请求，并且已经成功处理了请求，但为了完成指定的动作，客户端还需要再做一些其他工作，大多用于实现URL重定向 |
| 4XX        | 客户端错误状态码，客户端发送的请求出现了某些问题，`404 Not Found`表示服务器无法从请求指定的URL中知道客户端想要的资源 |
| 5XX        | 服务器错误状态码，当服务器因为某些原因而无法正确的处理请求时，服务器就会通过这类状态码通知客户端 |

#### 响应首部

| 首部字段         | 作用描述                                                     |
| ---------------- | ------------------------------------------------------------ |
| Allow            | 告知客户端，服务器支持那些请求方法                           |
| Content-Length   | 响应主体的字节长度                                           |
| Content-Type     | 如果响应包含可选的主体，那么这个首部记录的就是主体内容的类型 |
| Date             | 当前时间                                                     |
| Location         | 这个首部在重定向时使用，它会告知客户端接下来向哪个URL发送请求 |
| Server           | 返回响应的服务器的域名                                       |
| Set-Cookie       | 在客户端里面设置一个cookie，一个响应里面可以包含多个Set-Cookie首部 |
| WWW-Authenticate | 服务器通过这个首部来告知客户端，在Authorization请求首部中应该提供那些类型的身份信息，服务器通常会把这个首部与`401 Unauthorized`状态行一同发送 |

### 1.7 URI

Tim Berners-Lee 引入使用位置字符串表示互联网资源的概念，对`统一资源标识符（URI）`进行了定义，他描述了一种使用字符串表示资源名字的方法，以及一种使用字符串表示资源所在位置的方法，前一种方法被称为`统一资源名称（URN）`，后一种方法被称为`统一资源定位符（URL）`

URI的一般格式为：

<方案名称>:<分层部分>[ ? <查询参数> ] [ # <片段> ]

- 方案名称记录了URI正在使用的方案，它定义了URI其余部分的结构
- 分层部分包含了资源的识别信息，这些信息会以分层的方式进行组织，如果分层部分以双斜杠//开头，那么说明它包含了可选的用户信息，这些信息将以@符号结尾，后跟分层路径

在URI的各个部分当中，只有方案名称和分层部分是必需的，以问号为前缀的查询参数是可短的，这些参数用于包含无法使用分层方式表示的其他信息，多个查询参数会被组织成一连串的键值对，各个键值对之间使用&符号分隔

URI另一个可选部分是片段，片段使用井号作为前缀，它可以对URI定义的资源中的次级资源进行标识。

使用HTTP方案的URI示例：

```txt
http://sausheong:password@www.example.com/docs/file?name=sausheong&location-singapore#summary
```

这个URI使用的是http方案，跟在方案名之后的是一个冒号，位于@符号之前的分段 `sausheong:password`记录的是用户名和密码，跟在用户信息之后的`www.example.com/docs/file`就是分层部分的剩余部分。跟在分层部分之后的是以问号为前缀的查询参数，这个部分包含了 `name=sausheong`和`location=singapore`这两个键值对，键值对之间使用一个&符号连接，最后这个URI的末尾还带有一个以井号为前缀的片段

### 1.8 HTTP/2 简介

与使用纯文本方式表示的HTTP1.x不同，HTTP/2是一种二进制协议：二进制表示不仅能够让HTTP/2的语法分析变得更为高效，还能让协议变得更加紧凑和健壮

HTTP/2是完全多路复用的，这意味着多个请求和响应可以在同一时间内使用同一个连接，除此之外，HTTP/2还会对首部进行压缩以减少需要传送的数据量，并允许服务器将响应推送至客户端

### 1.9 Web应用的各个组成部分

Web应用就是一个执行以下任务的程序：

1. 通过HTTP协议，以HTTP请求报文的形式获取客户端输入
2. 对HTTP请求报文进行处理，并执行必要的操作
3. 生成HTML，并以HTTP响应报文的形式将其返回给客户端

为了完成这个任务，Web应用被分成了处理器和模板引擎这两个部分

#### 处理器

Web应用中的处理器除了要接收和处理和客户端发来的请求，还要调用模板引擎，然后由模板引擎生成HTML并把数据填充至将要回传给客户端的响应报文当中

用MVC模式来讲，处理器既是控制器（controller），也是模型（model），控制器应该是苗条的，它应该只包含路由代码以及HTTP报文的解包和打包逻辑，而模型则应该是丰满的，它应该包含应用的逻辑以及数据

#### 模板引擎

通过HTTP响应报文回传给客户端的HTML是由模板转换而成的，模板里面可能会包含HTML，但也可能不会，而模板引擎则通过模板和数据来生成最终的HTML

- 静态模板 是一些夹杂着占位符的HTML，静态模板引擎通过将静态模板中的占位符替换成响应的数据来生成最终的HTML
- 动态模板 除了包含HTML和占位符之外，还包含一些编程语言结构，如条件语句、迭代语句和变量 JSP/ASP/ERB 都属于动态模板引擎

### 1.10 Hello Go

```go
import (
	"fmt"
	"log"
	"net/http"
)

func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello World! %s", r.URL.Path[1:])
}

func main() {
	http.HandleFunc("/", handler)
	err := http.ListenAndServe(":8080", nil)
	if err != nil {
		log.Fatal(err.Error())
	}
}
```

## 第二章 ChitChat论坛

### 2.2 应用设计

当请求到达服务器时，多路复用器会对请求进行检查，并将请求重定向到正确的处理器进行处理，处理器在接收到多路复用转发的请求之后，会从请求中去除相应的信息，并根据这些信息对请求进行处理，在处理请求完毕之后，处理器会将所得的数据传递给模板引擎，而模板引擎则会根据这些数据生成将要返回给客户端的HTML，整个过程如下：

![多路复用](image/20190818151333.png)

### 2.3 数据模型

绝大多数应用都需要以某种方式与数据打交道，对ChitChat来说，它的数据包括四种数据结构

- User - 表示论坛的用户信息
- Session - 表示论坛用户当前的登录会话
- Thread - 表示论坛里面的帖子，每一个帖子记录了多个论坛用户之间的对话
- Post - 表示用户在帖子里面添加的回复

### 2.4 请求的接收和处理

Web 应用的工作流程如下：

1. 客户将请求发送到服务器的一个URL上
2. 服务器的多路复用器将接收到的请求重定向到正确的处理器，然后由该处理器请求进行处理
3. 处理器处理请求并执行必要的动作
4. 处理器调用模板引擎，生成相应的HTML并将其返回给客户端

#### 多路复用器

```go
func index(w http.ResponseWriter, r *http.Request) {
	...
}

func main() {
	fmt.Println("Hello world")
    // net/http 提供的默认多路复用器
	mux := http.NewServeMux()
    
    // 服务器接收到一个以/static/开头的URL请求，以下两行会移除URL中的/static/字符串，然后再public目录中查找被请求的文件
	files := http.FileServer(http.Dir("/public"))
	mux.Handle("/static/", http.StripPrefix("/static/", files))

    // 将发送至根URL的请求重定向到处理器
	mux.HandleFunc("/", index)

	server := &http.Server{Addr: "0.0.0.0:8080", Handler: mux}
	err := server.ListenAndServe()
	if err != nil {
		log.Fatal(err.Error())
	}
}
```

因为所有处理器都接受一个 ResponseWriter 和一个指向 Request 结构的指针作为参数，并且所有请求参数都可以通过访问 Request 结构得到，所以程序不需要向处理器显示地传入任何请求参数

#### 服务静态文件

除了负责请求重定向到相应的处理器之外，多路复用器还需要为静态文件提供服务，为了做到这一点，程序使用FileServer函数创建了一个能够为指定目录中的静态文件服务的处理器，并将这个处理器传递给多路复用器的Handle函数，初次之外，程序还是用 StripPrefix 函数去移除请求URL中的指定前缀

#### 创建处理器函数

```go
func index(w http.ResponseWriter, r *http.Request) {
	files := []string {
		"templates/layout.html",
		"templates/navbar.html",
		"templates/index.html",
	}
	templates := template.Must(template.ParseFiles(files...))
	threads, err := data.Threads(); if err != nil {
		templates.ExecuteTemplate(w, "layout", threads)
	}
}
```

index 函数负责生成HTML并将其接入 ResponseWriter 中

#### 使用 cookie 进行访问控制

当一个用户成功登录之后，服务器必须在后续的请求中标示出这是一个已登录的用户，为了做到这一点服务器会在响应的首部写入一个cookie，而客户端在接收到这个cookie之后则会把它存储在浏览器里面

```go
func authenticate(w http.ResponseWriter, r *http.Request) {
	err := r.ParseForm()
	if err != nil {
		log.Fatal(err.Error())
	}
    // 通过给定的邮箱地址获取与之对应的User结构
	user, _ := data.UserByEmail(r.PostFormValue("email"))
	if user.Password == data.Encrypt(r.PostFormValue("password")) {
		session := user.CreateSession()
        
        // 创建Session结构之后，程序又创建了 Cookie 结构
		cookie := http.Cookie{
			Name:     "_cookie",
			Value:    session.Uuid,
			HttpOnly: true,
		}
		http.SetCookie(w, &cookie)
		http.Redirect(w, r, "/", 302)
	} else {
		http.Redirect(w, r, "/login", 302)
	}
}
```

在验证用户身份的时候，程序必须先确保用户是真实存在的，并且提交给处理器的密码在加密之后跟存储在数据库里面已经加密用户密码完全一致，在核实了用户的身份之后，程序会使用User结构的CreateSession方法创建一个Session结构，该结构如下：

```go
type Session struct {
	Id       int
	Uuid     string    // 随机生成的唯一ID
	Email    string    // 邮箱地址
	UserId   int       // 用户表中存储用户信息的行ID
	CreateAt time.Time
}
```

Uuid 是实现会话机制的核心，服务器会通过cookie把这个ID存储到浏览器里，并把Session结构中记录的各项信息存储到数据库中

cookie 的值是将要被存储在浏览器里面的唯一ID，因为程序没有给cookie设置过期时间，所以这个cookie就成了一个会话cookie，在浏览器关闭时自动被移除

在将Cookie存储到浏览器里面之后，程序接下来要做到在处理器函数里面检查当前访问的用户是否已经登录，为此我们需要创建一个名为session的工具函数（util.go），并在各个处理器函数里面复用它

```go
func session(w http.ResponseWriter, r *http.Request) (data.Session, error) {
	cookie, err := r.Cookie("_cookie")
	if err == nil {
		sess = data.Session{Uuid: cookie.Value}
        // 访问数据库并核实会话的唯一ID是否存在
		if ok, _ := sess.Check(); !ok {
			err = errors.New("Invalid session")
		}
		return sess, nil
	}
	return nil, err
}
```

### 2.5 使用模板生成HTML响应

index 处理器函数里面的大部分代码都是用来为客户端生成HTML的，首先函数把每个需要用到的模板文件都放到GO切片里面

```go
func index(w http.ResponseWriter, r *http.Request) {
	_, err := session(w, r)
	public_tmpl_files := []string {
		"templates/layout.html",
		"templates/public.navbar.html",
		"templates/index.html",
	}
	private_tmpl_files := []string{
		"templates/layout.html",
		"templates/private.navbar.html",
		"templates/index.html",
	}
	var templates *template.Template
	if err != nil {
		templates = template.Must(template.ParseFiles(public_tmpl_files...))
	} else {
		templates = template.Must(template.ParseFiles(private_tmpl_files...))
	}
   	threads, err := data.Threads(); if err == nil {
		templates.ExecuteTemplate(w, "layout", threads)
	}
}
```

跟Mustache 和 CTemplate 等其他模板引擎一样，切片指定的这三个HTML文件都包含了特定的嵌入命令（动作）

layout.html：

```html
{{ define "layout" }}

    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=9">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>ChitChat</title>
        <link href="/static/css/bootstrap.min.css" rel="stylesheet">
        <link href="/static/css/font-awesome.min.css" rel="stylesheet">
    </head>
    <body>
     <!-- 引用其他模板文件的 template 动作 . 代表传递给引用模板的数据 -->
     <!-- 传递给layout模板的数据也会传递给navbar模板 -->
    {{ template "navbar" . }}

    <div class="container">

        {{ template "content" . }}

    </div> <!-- /container -->

    <script src="/static/js/jquery-2.1.1.min.js"></script>
    <script src="/static/js/bootstrap.min.js"></script>
    </body>
    </html>

{{ end }}
```

index.html：

```html
{{ define "content" }}
    <p class="lead">
        <a href="/thread/new">Start a thread</a> or join one below!
    </p>

    {{ range . }}
        <div class="panel panel-default">
            <div class="panel-heading">
                <span class="lead"> <i class="fa fa-comment-o"></i> {{ .Topic }}</span>
            </div>
            <!-- {{.User.Name}} 这些动作和之前展示过的index处理器函数有关 -->
            <div class="panel-body">
                Started by {{ .User.Name }} - {{ .CreatedAtDate }} - {{ .NumReplies }} posts.
                <div class="pull-right">
                    <a href="/thread/read?id={{.Uuid }}">Read more</a>
                </div>
            </div>
        </div>
    {{ end }}

{{ end }}
```

在以下这行代码中：

```go
templates.ExecuteTemplate(w, "layout", threads)
```

程序通过调用 ExecuteTemplate 函数，执行已经经过语法分析的layout模板，即把模板文件中的内容和来自其他渠道的数据合并，然后产生最终的HTML



![](image/20190818174900.png)

#### 整理代码

生成HTML的代码会被重复执行多次，我们把这些代码进行整理，空接口让任何类型的之都可以传递给函数作为参数，最后的 ... 表示generateHTML函数是一个可变参数函数

```go
func generateHTML(writer http.ResponseWriter, data interface{}, filenames ...string) {
	var files []string
	for _, file := range filenames {
		files = append(files, fmt.Sprintf("templates/%s.html", file))
	}

	templates := template.Must(template.ParseFiles(files...))
	templates.ExecuteTemplate(writer, "layout", data)
}
```

index处理器函数变为：

```go
func index(writer http.ResponseWriter, request *http.Request) {
	threads, err := data.Threads(); if err == nil {
		_, err := session(writer, request)
		if err != nil {
			generateHTML(writer, threads, "layout", "public.navbar", "index")
		} else {
			generateHTML(writer, threads, "layout", "private.navbar", "index")
		}
	}
}
```

### 2.9 应用运行流程回顾

1. 客户端向服务器发送请求
2. 多路复用器接收到请求，并将其重定向到正确的处理器
3. 处理器对请求进行处理
4. 在需要访问数据库的情况下，处理器会使用一个或多个数据结构，这些数据结构都是根据数据库中的数据建模出来的
5. 当处理器调用与数据结构有关的函数或者方法时，这些数据结构背后的模型回应数据库进行连接，并执行相应的操作
6. 当请求处理完毕时，处理器会调用模板引擎，有时候还会向模板引擎传递一个通过模型获取到的数据
7. 模板引擎会对模板文件进行语法分析并创建相应的模板，而这些模板又会与处理器传递的数据一起合并成最终的HTML
8. 生成HTML会作为相应的一部分回传到客户端

![1566313004353](D:\Note\image\1566313004353.png)

## 第三章 接收请求

### 3.1 Go的net/http标准库

因为HTTP是一个无连接的协议，通过这种协议发送给服务器的请求对服务器之前处理过的请求一无所知，所以应用程序才会以cookie的方式在客户端实现数据持久化，并以会话的方式在服务器上实现数据持久化

为了减低使用cookie和会话带来的复杂性，Web应用框架通常都会提供一个统一的接口用于在连接之间实现持久化，不同的框架提供的接口可能会有所不同

理解哪些隐藏在框架之下的底层概念和基础设施是非常重要的，只要对框架的实现原理有了正确的认识，我们就可以更加清晰地了解到这些约定和模式是如何形成的

对Go语言来说，隐藏在框架之下的通常是 net/http 和 html/template 这两个标准库

net/http标准库可以分为客户端和服务器两个部分，库中的结构和函数有些只支持客户端和服务器这两者之一，而有些则同时支持客户端和服务器

- Client、Response、Header、Request 和 Cookie 对客户端进行支持
- Server、ServeMux、Handler/HandleFunc、ResponseWriter、Header、Request 和 Cookie 则对服务器进行支持

![1566313988982](D:\Note\image\1566313988982.png)

### 3.2 Go Web 服务器

创建一个服务器的步骤是非常简单的，只要调用 `ListenAndServe` 并传入网络地址以及负责处理请求的处理器作为参数即可，如果网络地址参数为空字符串，那么服务器默认使用80端口进行网络连接，如果处理器参数为nil，那么服务器将使用默认的多路复用器 `DefaultServeMux`

```go
package main
import (
	"net/http"
)

func main() {
    http.ListenAndServe("", nil)
}
```

用户出了可以通过 `ListenAndServe`的参数对服务器的网络地址和处理器进行配置之外，还可以通过Server结构对服务器进行更详细的配置

```go
package main
import (
	"net/http"
)

func main() {
    server := http.Server{
        Addr: "127.0.0.1:8080",
        Handler: nil,
    }
    http.ListenAndServe("", nil)
}
```

#### 通过HTTPS提供服务

```go
package main
import (
	"net/http"
)

func main() {
    server := http.Server{
        Addr: "127.0.0.1:8080",
        Handler: nil,
    }
    http.ListenAndServeTLS("cert.pem", "key.pem")
}
```

这段代码中的 `cert.pem`文件是SSL证书，而 `key.pem`则是服务器的私钥，在生产环境中使用的SSL证书需要通过 CA 取得，如果是出于测试目的才使用证书和私钥，那么使用自动生成证书就可以了，生成证书的办法有很多种，其中一种就是使用Go标准库中的 crypto 包群

> HTTPS 需要使用SSL/TLS证书来实现数据加密以及身份验证，SSL证书存储在服务器之上，它是一种使用X.509格式进行格式化的数据，这些数据包含了公钥以及其他一些相关信息，为了保证证书的可靠性，证书一般由CA签发，服务器在接收到客户端发送请求之后，会将证书和响应一并返回给客户端，而客户端在确认证书的真实性之后，就会产生一个随机密钥(random key)，并使用证书中的公钥对随机密钥进行加密，此次加密产生的对称密钥(symmetric key) 就是客户端和服务器在进行通信时，负责对通信实施加密的实际密钥

生成SSL证书和密钥的步骤并不是特别复杂，因为SSL证书实际上就是一个将拓展密钥用法（extended key usage）设置成了服务器身份验证操作的X.509证书

```go
package main

import (
	"crypto/rand"
	"crypto/rsa"
	"crypto/x509"
	"crypto/x509/pkix"
	"encoding/pem"
	"fmt"
	"math/big"
	"net"
	"net/http"
	"os"
	"time"
)

func main() {

	max := new(big.Int).Lsh(big.NewInt(1), 128)
	serialNumber, _ := rand.Int(rand.Reader, max)
	subject := pkix.Name{
		Organization:       []string{"Manning Publications Co."},
		OrganizationalUnit: []string{"Books"},
		CommonName:         "Go Web Programming",
	}

    // 程序使用一个Certificate结构对证书进行配置
	template := x509.Certificate{
		SerialNumber: serialNumber, //CA分发的唯一号码
		Subject:      subject,
		NotBefore:    time.Now(),
		NotAfter:     time.Now().Add(365 * 24 * time.Hour), // 有效期一年
		KeyUsage:     x509.KeyUsageKeyEncipherment | x509.KeyUsageDigitalSignature, // 这个X.509证书是用于进行服务器身份验证操作的
		ExtKeyUsage:  []x509.ExtKeyUsage{x509.ExtKeyUsageServerAuth},
		IPAddresses:  []net.IP{net.ParseIP("127.0.0.1")}, // 证书只能在IP地址 127.0.0.1 上运行
	}
    
	// 产生RSA私钥，该结构里包含了一个能够公开访问的公钥
	pk, _ := rsa.GenerateKey(rand.Reader, 2048)
	// 这个公钥在使用 x509.CreateCertificate函数创建SSL证书时就会用到，创建出一个 经过 DER编码格式化的字节切片
	derBytes, _ := x509.CreateCertificate(rand.Reader, &template, &template, &pk.PublicKey, pk)
    // 使用 encoding/pem 标准将证书编码到 cert.pem
	certOut, _ := os.Create("cert.pem")
	_ = pem.Encode(certOut, &pem.Block{Type: "CERTIFICATE", Bytes: derBytes})
	_ = certOut.Close()

    // 将继续以 pem 编码的方式把之前生成的密钥编码并保存到 key.pem 文件里面
	keyOut, _ := os.Create("key.pem")
	_ = pem.Encode(keyOut, &pem.Block{Type: "RSA PRIVATE KEY", Bytes: x509.MarshalPKCS1PrivateKey(pk)})
	_ = keyOut.Close()
}
```

如果证书是由CA签发的，那么证书文件中将同时包含服务器签名以及CA签名，其中服务器签名在前，CA签名在后

### 3.3 处理器和处理器函数

#### 处理请求

在Go语言中，一个处理器就是一个拥有 ServeHTTP 方法的接口，这个 ServeHTTP方法需要接收两个参数，第一个参数是一个 ResponseWriter 接口，第二个参数是一个指向 Request 结构的指针，换句话说，任何接口只要拥有一个 ServeHTTP 方法，并且该方法带有以下签名，那么它就是一个处理器

```go
ServeHTTP(http.ResponseWriter, *http.Request)
```

问题：既然 ListenAndServe 接受的第二个参数是一个处理器，那么为何它的默认值却是多路复用器 DefaultServeMux？

因为 DefaultServeMux 多路复用器是 ServeMux 结构的一个实例，而后者也拥有上面提到的 ServeHTTP 方法，并且这个方法的签名与成为处理器所需的签名完全一致。换句话说 DefaultServeMux 即是 ServeMux 结构的实例，也是 Handler 结构的示例，因此 DefaultServeMux 不仅是一个多路复用器，它还是一个处理器。作为处理器 DefaultServeMux 唯一要做的就是根据请求的 URL 将请求重定向到不同的处理器

```go
type MyHandler struct{}

func (h *MyHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello World!")
}

func main() {
    handler := MyHandler{}
    server := http.Server{
        Addr: "127.0.0.1:8080",
        Handler: &handler,
    }
    server.ListenAndServe()
}
```

如果我们编写一个处理器并使用它代替默认的多路复用器，这就意味着服务器不会再通过URL匹配来将请求路由到不同的处理器，而是直接使用同一个处理器来处理所有请求

这也是我们在Web应用中使用多路复用器的原因，对某些特殊用途的服务器来说，只使用一个处理器也许就可以很好的完成工作，但是在大部分请求下，我们还是希望服务器可以根据不同的URL请求返回不同的响应，并不是一成不变的只返回一种响应

#### 使用多个处理器

我们不再在 Server 结构的Handler字段中指定处理器，而是让服务器使用默认的 DefaultServeMux 作为处理器，然后通过 http.Handle 函数将处理器绑定到 DefaultServeMux

虽然 Handle 函数来源于 http 包，但它实际上是 ServeMux 结构的方法：这些函数是为了操作便利而创建的函数，调用他们等同于调用 DefaultServeMux 的某个方法，比如，调用 http.Handle 实际上就是在调用 DefaultServeMux 的 Handle 方法

```go
type HelloHandler struct{}

func (h *HelloHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello")
}

type WorldHandler struct{}

func (h *WorldHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "World")
}

func main() {
	hello := HelloHandler{}
	world := WorldHandler{}
	server := http.Server{
		Addr: "127.0.0.1:8080",
	}
	http.Handle("/hello", &hello)
	http.Handle("/world", &world)
	server.ListenAndServe()
}
```

#### 处理器函数

处理器函数实际上就是与处理器拥有相同行为的函数：这些函数与ServeHTTP 方法拥有相同的签名，也就是说他们接受ResponseWriter 和指向 Request 结构的指针作为参数

```go
func hello(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello")
}

func world(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "World")
}

func main() {
	server := http.Server{
		Addr: "127.0.0.1:8080",
	}
	http.HandleFunc("/hello", hello)
	http.HandleFunc("/world", world)
	server.ListenAndServe()
}
```

处理器函数的实现原理是这样的：Go语言拥有一种 HandlerFunc 函数类型，它可以把一个带有正确签名的函数 f 转换成一个带有方法 f 的Handler，比如对下面这个 hello 函数来说：

```go
func hello(w http.ResponseWriter, r *http.Request) {
    fmt.Fprintf(w, "Hello!")
}
```

程序只需要执行以下代码：

```go
helloHandler := HandlerFunc(hello)
```

就可以把 helloHandler 设置成一个 Handler

HandleFunc 函数会将 hello 函数转换成一个 Handler，并将它与 DefaultServeMux 进行绑定，以此来简化创建并绑定 Handler 的工作，换句话说，处理器函数只不过是创建处理器的一种便利方法而已

http.HandleFunc 函数的源代码：

```go
// HandleFunc registers the handler function for the given pattern
// in the DefaultServeMux.
// The documentation for ServeMux explains how patterns are matched.
func HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	DefaultServeMux.HandleFunc(pattern, handler)
}

// HandleFunc registers the handler function for the given pattern.
func (mux *ServeMux) HandleFunc(pattern string, handler func(ResponseWriter, *Request)) {
	if handler == nil {
		panic("http: nil handler")
	}
	mux.Handle(pattern, HandlerFunc(handler))
}
```

处理器函数并不能完全替代处理器，因为在某些情况下，代码可能已经包含了某个接口或者某种类型，这时我们只需要为他添加 ServeHTTP 方法就可以将它们转变为处理器，并且这种转变有助于构建出模块化的 Web 应用

#### 串联多个处理器和处理器函数

程序可以将一个函数传递给另一个函数，又或者通过标识符去引用一个具名函数，这意味着程序可以将函数 f1 传递给另一个函数 f2，然后在函数 f2 执行完某些操作之后调用 f1

![1566392935190](D:\Note\image\1566392935190.png)

诸如日志记录、安全检查和错误处理这样的操作通常被称为 横切关注点，虽然这些操作非常常见，但是为了防止代码重复和代码依赖问题，我们又不希望这些操作额正常的代码搅和在一起，为此我们可以使用串联（chaining）技术分隔代码中的横切关注点

```go
func hello(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello")
}

func log(h http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		name := runtime.FuncForPC(reflect.ValueOf(h).Pointer()).Name()
		fmt.Println("Handler function called - " + name)
		h(w, r)
	}
}

func main() {
	server := http.Server{
		Addr: "127.0.0.1:8080",
	}
	http.HandleFunc("/hello", log(hello))
	server.ListenAndServe()
}
```

除处理器函数 `hello` 之外，这个代码清单还包含了一个 `log` 函数，`log` 函数接受一个 `HandlerFunc` 类型的函数作为参数，然后返回另一个 `HandlerFunc` 类型作为值，因为 `hello` 函数就是一个 `HandlerFunc` 类型的函数，所以代码 `log(hello)`实际上是就是将 `hello` 函数发送至 `log` 函数之内，换句话说，这段代码串联起了 `log` 函数和 `hello` 函数

log 函数的返回值是一个匿名函数，因为这个匿名函数 接受一个 ResponseWriter 和一个 Request 指针作为参数，所以它实际上也是一个 HandlerFunc，在匿名函数内部，程序首先会获取被传入的 HandlerFunc 的名字，然后调用这个 HandlerFunc。

就像搭积木一样，既然我们可以串联起两个函数，那么自然也可以串联起更多函数，串联多个函数可以让程序执行更多动作，这种做法有时候也称为 管道处理

 如果我们还有一个protect 函数，用于验证身份

```go
func protect(h http.HandlerFunc) http.HandlerFunc {
    return func(w http.ResponseWriter, r *http.Request) {
        ...
        h(w, r)
    }
}
```

我们只需要把 protect 函数跟之前的函数串联在一起，就可以正常使用了：

```go
http。HandleFunc("/hello", protect(log(hello)))
```

以上提到的都是串联处理器函数，串联处理器的方法实际上和串联处理器函数的方法是非常相似的

```go
type HelloHandler struct{}

func (h HelloHandler) ServeHTTP (w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello")
}

// log 不再接受和返回 HandlerFunc 类型的函数，而是接受并返回 Handler类型的处理器
func log(h http.Handler) http.Handler {
	return http.HandlerFunc (func(w http.ResponseWriter, r *http.Request) {
		fmt.Printf("Handler called = %T\n", h)
		h.ServeHTTP(w, r)
	})
}
// log和protect函数不再返回匿名函数，而是使用HandlerFunc直接将匿名函数转换成一个Handler，然后返回这个Handler
// 程序现在也不再直接执行处理器函数了，而是调用处理器的ServeHTTP函数
func protect(h http.Handler) http.Handler {
	return http.HandlerFunc (func(w http.ResponseWriter, r *http.Request) {
        ...
		h.ServeHTTP(w, r)
	})
}

func main() {
	server := http.Server{
		Addr: "127.0.0.1:8080",
	}
	hello := HelloHandler{}
	// 程序现在绑定的是处理器而不是处理器函数
	http.Handle("/hello", protect(log(hello)))
	server.ListenAndServe()
}
```

#### ServeMux 和 DefaultServeMux

ServeMux 是一个HTTP请求多路复用器，它负责接受HTTP请求并根据请求中的URL将请求重定向到正确的处理器

ServeMux 结构包含了一个映射，这个映射会将URL映射至响应的处理器，因为 ServeMux 结构也实现了ServeHTTP 方法，所以它也是一个处理器，当 ServeMux 的 ServeHTTP 方法接收到一个请求的时候，他会在结构的映射里面找出与请求URL 最为匹配的 URL，然后调用与之相对应的处理器的 ServeHTTP 方法

![1566392872414](D:\Note\image\1566392872414.png)

因为 ServeMux 是一个结构而不是一个接口，所以 DefaultServeMux 并不是 ServeMux 的实现，DefaultServeMux 实际上是 ServeMux 的一个实例，并且所有引入了 net/http 标准库的程序都可以使用这个实例，当用户没有为 Server 结构指定处理器时，服务器就会使用 DefaultServeMux 作为 ServeMux 的默认实例

因为ServeMux也是一个处理器，所以用户可以在有需要的情况下对其实例实施处理器串联

对于图3-6所示的例子，服务器会用 indexHandler 去处理对 `/hello/there` 的请求

> 最小惊讶原则：指的是我们在进行设计的时候，应该做哪些合乎常理的事情，使事物的行为总是显而易见、始终如一并且合乎情理

产生这种行为的原因在于程序在绑定 `helloHandler` 时使用的URL是 `/hello`，而不是 `/hello/`，如果绑定的URL不是以`/`结尾，那么它只会与完全相同的URL8匹配，如果绑定的URL以 `/` 结尾，那么即使请求的URL只有前缀部分与被绑定URL相同，ServeMux 也会认定这两个URL是匹配的

这也就是说，如果与 helloHandler 处理器绑定的URL是 `/hello/`  而不是  `/hello` ，那么当浏览器请求 `/hello/there` 的时候，服务器在找不到与之完全匹配的处理器时，就会退而求其次开始寻找能够与 `/hello/` 匹配的处理器，并最终找到 helloHandler 处理器

### 3.4 使用HTTP/2

为了让构建的Web服务器用上 HTTP/2，我们需要给这个服务器导入 http2 包，并通过添加一些代码行来让服务器打开对 HTTP/2 的支持

```go
package main

import (
	"fmt"
    "golang.org/x/net/http2"
	"net/http"
)

type MyHandler struct{}

func (h *MyHandler) ServeHTTP (w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hello World!")
}

func main() {
	handler := MyHandler{}
	server := http.Server{
		Addr: "127.0.0.1:8080",
		Handler: &handler,
	}

	http2.ConfigureServer(&server, &http2.Server{})
	server.ListenAndServeTLS("cert.pem", "key.pem")
}
```

cURL 测试代码是否启用了 HTTP/2，--insecure 用于让cURL强制接受我们创建的证书，从而使访问可以顺利进行

```txt
curl -I --http2 --insecure https://localhost:8080/
```

## 第四章 处理请求

### 4.1 请求和响应