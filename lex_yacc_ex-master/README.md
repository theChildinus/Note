# lex_yacc_ex


## 项目逻辑
- examples: smv的样例代码
- output: 临时生成的flex和bison的输出文件存放位置
- src: 主要代码
    - smv_lex.l: 词法分析代码,由lex或flex生成词法分析cpp程序(smv_lex.yy.cpp)
    - smv_yacc.y: 语法分析代码,由yacc或bison生成语法分析cpp程序(smv_yacc.tab.h和smv_yacc.tab.cpp)
    - node.h: 自定义结点，指在语法分析的过程中生成由此结点定义的语法树，可修改此结点内容
- old_src: flex和bison的测试样例，与此项目无关
- file.txt: flex和bison的测试样例用文件，与此项目无关
- Makefile: 构建项目用，项目已更改成cmake工程，此文件被CMakeLists.txt代替
- CMakeLists.txt: 构建此项目用，需要安装flex和bison

## 编译

```
# mkdir output
# mkdir build
# cd build
# cmake ..
# make
```

## 执行

```
# ./lex_yacc_ex
```