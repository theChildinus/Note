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

## [ipc & net/http](https://github.com/theChildinus/learngo)
