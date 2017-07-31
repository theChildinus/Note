int sum = 0, value,
	units_sold = 0;

std::string book("111")

初始化不是赋值， 初始化的含义是创建变量时赋予其一个初始值
而赋值的含义是把对象的当前值擦除 而以一个新值来替代

列表初始化 花括号 初始值存在丢失信息的风险 编译器报错

函数体内的内置类型的对象没有初始化其值未定义 类的对象如果没有显示初始化 值由类确定

extern int i 声明但不显式初始化 显式初始化变成了定义
变量能且只能被定义一次 但可以声明多次

C++静态类型
int ival = 1024;
int &refval = ival;
int &refval2 //报错
引用并非对象 相反的 它只是给一个已经存在的对象所另起的一个名字

refval = 2; 2->refval 也即 2-> ival
int ii = refval;
引用只能绑定在对象上 且 类型相同

int ival = 42;
int *p = &ival; p存放ival的地址 p是指向ival的指针
*p = 0;
cout << *p;
由符号*得到指针p所指的对象，即可经由p为变量ival赋值
解引用操作 仅适用于那些指向了某些对象的有效指针
引用不是对象 不能定义指向引用的指针

int *p1 = nullptr;
定义了对象之后再定义指向它的指针

引用本身并非一个对象，引用一旦定义，就再无法绑定到另外的对象
指针则没有这种限制

赋值语句 改变了指针的值 还是改变了指针所指对象的值
最好的办法就是记住 //赋值永远改变的是等号左侧的对象
pi = &ival; // pi的值被改变 改变了那个存放在pi内的地址值
*pi = 0;    // ival的值被改变 指针pi并没有改变 *pi发生了改变

int ival = 1024;
int *pi = &ival;
int **ppi = &pi;

cout << ival << endl;
cout << *pi << endl;
cout << **ppi << endl;

引用本身不是一个对象，所有不能定义一个指向引用的指针
但指针是对象，所以存在指针的引用

int i = 42;
int *p;      // p是一个int型指针
int *&r = p; // r是一个对指针p的引用   从右往左阅读r的定义

r = &i;  //就是令p指向i
*r = 0;

const 对象必须初始化

extern const int bufSize;
如果想在多个文件之间共享 const对象，必须在变量的定义之前添加extern关键字

const int ci = 1024;
const int &r1 = ci; //true
r1 = 42;  //false
int &r2 = ci; //false

对const的引用 简称为 常量引用

double dval = 3.14;
const int &ri = dval;
实际上ri会绑定一个临时量对象 C++认为非法

int i = 42;
int &r1 = i;
const int &r2 = i;
r1 = 0; //true
r2 = 0; //false r2是一个常量引用

const double pi = 3.14;
double *ptr = &pi;        //false
const double *cptr = &pi; //true
*cptr = 42;               //false 不能给*cptr赋值

double dval = 3.14;
cptr = &dval; // true 但是不能通过cptr改变dval的值


int errNumb = 0;
int *const curErr = &errNumb;  //curErr将一直指向errNumb 可通过curErr改变errNumb的值
const double pi = 3.14159;
const double *const pip = &pi; //pip是一个指向常量对象的常量指针

顶层const 表示指针本身是个常量
底层const 表示指针所指的对象是个常量
int i = 0;
int *const p1 = &i; //不能改变p1值 顶层const
const int ci = 42;  //不能改变ci值 顶层const
const int *p2 = &ci;//不能改变p2值 底层const
const int *const p3 = p2; //右侧const是顶层const 左侧const是底层const
const int &r = ci; //声明引用的const都是底层const

拷入拷出的对象必须具有相同的底层const资格 或者两个对象的数据类型必须能相互转化
非常量可转换为常量

常量表达式是指值不会改变并且 在编译过程中 就能得到结果的表达式

constexpr int mf = 20;   //20是常量表达式
constexpr int limit = mf + 1; //mf + 1是常量表达式
constexpr int sz = size(); //只有当size() 是一个constexpr函数时才是一个正确的声明语句

const int *p = nullptr;  p 指向一个整型常量 的指针
constexpr int *q = nullptr;  q指向一个整数的 常量指针 顶层const

定义在函数体之外

类型别名：
using SI = Sales_item;

auto 类型说明符
auto i = 0, *p = &i;  //true
auto sz = 0, pi = 3.14; //false 一条语句中定义多个变量，符号&和*只从属某个声明符 而非基本数据类型的一部分
因此初始值必须是同一类型


int i = 42, *p = &i, &r = i;
decltype((variable)) 结果是引用
decltype(variable) //结果只有当variable本身是引用时才是引用
struct Sales_data
{
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

#include <iostream>
#include <string>
#include "Sales_data.h"
int main()
{
	Sales_data data1, data2;
	double prise = 0;
	std::cin >> data1.bookNo >> data1.units_sold >> price;
	data1.revenue = data1.units_sold * price;
	std::cin >> data2.bookNo >> data2.units_sold >> price;
	data2.revenue = data2.units_sold * price;
}

#ifndef SALES_DATA_H
#define SALES_DATA_H
#include <string>
struct Sales_data
{
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
#endif

########### 第三章 字符串/向量/数组 ##########

#include <iostream>
using std::cin;
int main()
{
	int i;
	cin >> i;
	cout << i; //false
	std::cout << i;
	return 0;
}

头文件不应包含using声明
#incldue <string>
using std::string;
string s1;
string s2 = s1;
string s2(s1);
string s3 = "hiya"; //拷贝初始化
string s3("value"); //直接初始化
string s4(10, 'c');

s.empty();
s.size();
s[n];

int main()
{
	string s;
	cin >> s;
	cout << s <<endl;
	return 0;
}
读取操作时 string 对象会自动忽视开头的空白 并从第一个真正的字符开始读起
"     hello world    " -> "hello"

int main()
{
	string word;
	while (cin >> word)
	{
		cout << word << endl;
	}
	return 0;
}
int main()
{
	string line;
	while (getline(cin, line))  //getline 直到换行符为止，读入换行符 但是line不存换行符
		cout << line << endl;
	return 0;
}

size() 函数返回的是string::size_type类型的值 是一个无符号类型的值

auto len = line.size()
一条表达式中，如果已经有了 size()函数 就不要使用int 可以避免 int 和 unsigned带来的问题

string对象和字符字面值混在一条语句中时，必须确保每个加法运算符（+）的两侧对象至少有一个是string

string s6 = s1 + "," + "hello"; //true
string s7 = "hello" + "," + s2 // false

范围for语句

for(declartion : expression)
	statement

expression 是一个对象 表示一个序列
declartion 表示一个变量 用于访问序列中的基本元素
每次迭代 declartion部分的变量会被初始化为expresison部分的下一个元素值

string str("some string");
for (auto c : str)
	cout << c << endl;

string s("hello world!!!");
decltype(s.size()) punct_cnt = 0;
for (auto c : s)
	if (ispunct(c))
		++punct_cnt;
cout << punct_cnt;
	 <<	" punctuation characters in " << s << endl;


string s("hello world!!!");
for (auto &c : s)
	c = toupper(c);  // c是一个引用 赋值改变s中字符的值
cout << s << endl;

s下标 0 -> s.size() - 1

for (decltype(s.size()) index = 0; index != s.size() && !isspace(s[index]); ++index)
	s[index] = toupper(s[index]);
设下标类型为string::size_type 是无符号数，可以保证下标不会小于0


const string hexdigits = "0123456789ABCDEF";
string result;
string::size_type n;
while (cin >> n)
{
	if (n < hexdigits.size())
		result += hexdigits[n];
}
cout << result << endl;



vector 表示对象的集合 其中所有对象类型相同 是一个类模板
#include <vector>
using std::vector;

编译器根据模板创建类或者函数的过程称为实例化，当使用模板时需要指出编译器应把类或者函数实例化成何种类型

vector <int> ivec;
vector <Sales_item> Sales_vec;
vector <vector<string>> file;

vector<T> v5{a, b, c...} 初始值个数的元素 每个元素被赋予相应的初始值
vector<T> v3(n, val) v3包含了n个重复的元素，每个元素的值为val

列表初始化vector对象
vector<string> articles = {"a", "an", "the"} //true
vector<string> v2("a", "an", "the"); //false

vector<int> v1(10); 10个元素默认初始化为0
vector<string> v2(5); 5个元素默认初始化为空string对象

vector<int> v1(10);  //v1中有10个元素 每个值为0
vector<int> v2{10};  //v2中有1个元素，元素值为10
vector<int> v3(10, 1); //v3中有是10个元素 每个值为1
vector<int> v4{10，1}；//v4中有2个元素 分别为10 和 1

圆括号表示构造 花括号表示 列表初始化

vector<string> v5{"hi"}; //列表初始化
vector<string> v6("hi"); //false 不能用字面值构造vector对象
vector<string> v7{10}; //v7有10个默认初始化的元素
vector<string> v8{10, "hi"}; //v8 有10个默认初始化为hi的元素

想要列表初始化vector对象 花括号里的值必须与元素类型相同
所以v7 v8 不能作为元素的初始值 确认无法列表初始化后 编译器会尝试使用默认初始化vector对象

vector<int> v2;
for (int i = 0; i != 100; ++i)
	v2.push_back(i);

string word;
vector<string> text;
while (cin >> word)
{
	text.push_back(word);
}

循环体内部有向 vector对象添加元素的语句 就不能使用范围for循环
！！范围for语句体内不能改变其所遍历序列的大小

vector<int> v{1,2,3,4,5,6,7,8,9};
for (auto &i : v)
	i *= i; //求元素的平方
for (auto i : v)
	cout << i << " ";
cout << endl;

vector<int>::size_type
两个vector对象比较按照字典序
不能比较 两个vector<Sales_item>对象

vector<unsigned> scores(11, 0);
unsigned garde;
while (cin >> garde)
{
	if (garde <= 100)
		++scores[garde / 10];  // auto ind = garde / 10; scores[ind]++;
}

vector 和 string 对象的下标运算可以用于访问已存在的元素 而不能用于添加元素
确保下标合法的一个有效手段是尽可能使用 范围 for 语句

迭代器
auto b = v.begin();
auto e = v.end();

*iter 返回迭代器iter指向元素的引用

string s("some string");
if (s.begin() != s.end())
{
	auto it = s.begin();
	*it = toupper(*it);
}

end返回的迭代器并不实际指示某个元素，所以不能对其进行递增或者解引用操作

vector<int>::iterator it;  //it 能读写vector<int>的元素
string::iterator it2;      //it2 能读写string对象中的字符
vector<int>::const_iterator it3; //it3只能读元素 不能写元素
string::const_iterator it4;  //it4只能读字符 不能写字符

vector<int> v;
const vector<int> cv;
auto it1 = v.begin(); //it1 类型是 vector<int>::iterator
auto it2 = cv.begin();//it2 类型是 vector<int>::const_iterator

auto it3 = v.cbegin();
auto it4 = v.cend(); //不论vector对象本身是否是常量 返回值都是const_iterator
(*it).empty() 等价于 it -> empty()

for (auto it = text.cbegin(); it != text.cend() && !it->empty(); ++it)
	cout << *it << endl;
任何一个可能改变vector对象容器的操作比如push_back 都会是vector对象的迭代器失效
但凡使用了迭代器的循环体 就不要向迭代器所属的容器添加元素

二分查找
text有序
auto beg = text.begin(), end = text.end();
auto mid = text.begin() + (end - beg) / 2;
while (mid != end && *mid != sought)
{
	if (sought < *mid)
		end = mid;
	else
		beg = mid + 1;

	mid  = beg + (end - beg) / 2;
}

数组维度必须是一个常量表达式
不存在引用的数组
不允许拷贝和赋值
unsigned cnt = 42;
constexpr unsigned sz = 42;

int arr[10];
int *parr[sz];
string bad[cnt]; //错误 cnt不是常量表达式
string strs[get_size()]; //当get_size是constexpr时正确

char a1[] = {'c', '+', '+'}; //列表初始化 没有空字符
char a2[] = {'c', '+', '+', '\0'}; //列表初始化 含有显示的空字符
char a3[] = "c++"; //自动添加表示字符串结束的空字符
const char a4[6] = "daniel" //错误 没有空间可以存放空字符

从数组名开始由内向外的顺序阅读
int *prts[10];  //prts是一个含有10个整型指针的数组
int &refs[10] = /*?*/;  //错误 不存在引用的数组
int (*parray)[10] = &arr; // parray 指向一个含有10个整数的数组
int (&arrRef)[10] = arr;  //arrRef 引用一个含有10个整数的数组  arrRef是引用

int *(&array)[10] = ptrs; //array是一个含有10个int型指针的数组的引用

for (auto i : scores)
	cout << i << endl;
cout << endl;

string nums[] = {"one", "two", "three"}; // 数组元素是string对象

string *p = &nums[0]; //p指向num的第一个元素
string *p2 = nums; //等价于 p2 = &nums[0]

int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; //ia是一个含有10个整数的数组
auto ia2(ia); //ia2 是一个整型指针 指向ia的第一个元素
ia2 = 42; //ia2是一个指针，不能用int值给指针赋值

###指针也是迭代器
int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int *p = arr;  //p指向arr的第一个元素
++p; //p指向arr[1]

int *e = &arr[10];  //指向arr尾元素的下一个位置的指针
for (int *b = arr; b != e; b++)
	cout << *b << endl;

int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int *beg = begin(ia); //ia首元素的指针
int *last = end(ia); //指向arr尾元素的下一位置的指针
begin end函数定义在iterator头文件中
尾后指针不能执行解引用和递增操作

constexpr size_t sz = 5; //size_t是无符号类型
int arr[sz] = {1, 2, 3, 4, 5};
int *ip = arr;
int *ip = ip + 4;

auto n = end(arr) - begin(arr);
两个指针相减的结果的类型是名为ptrdiff_t的标准库类型 带符号

int *b = arr, *e = arr + sz;
while (b < e)
	++b;

int ia[] = {0, 2, 4, 6, 8};
int last = *(ia + 4); //把last初始化为8，也就是ia[4]的值

last = *ia + 4 //正确 last = 4， 等价于ia[0] + 4 先解引用ia，然后给解引用的结果再加上4

int i = ia[2];  //ia转换成指向数组首元素的指针 ia[2] 得到(ia + 2)所指的元素
int *p = ia;    //p指向 ia的首元素
i = *(p + 2);   //等价于i = ia[2]

int *p = &ia[2]; //p 指向索引为2的元素
int j = p[1];    //p[1] 等价于*(p + 1) 就是ia[3] 表示的那个元素
int k = p[-2];   //p[-2] 是ia[0] 表示的那个元素

内置的下标运算符所用的索引值 不是无符号类型 这一点与 vector 和 string 不一样



string s1 = "a string example";
string s2 = "a different string";
if (s1 < s2)
const char ca1[] = "A string example";
const char ca2[] = "A different string";
if (ca1 < ca2) //未定义 试图比较两个无关地址
if (strcmp(ca1, ca2) < 0)

strcpy(largeStr, ca1);
strcat(largeStr, " ");
strcat(largeStr, ca2);
一个潜在的问题就是largeStr所需的空间是不容易估计的

string s("hello world");
char *str = s; //false 不能用string对象初始化char*
const char *str = s.c_str(); //true;

不允许使用一个数组 为另一个内置类型的数组赋初值
也不允许使用vector对象初始化数组

相反允许数组初始化vector对象
int int_arr[] = {0, 1, 2, 3, 4, 5};
vector<int> ivec(begin(int_arr), end(int_arr));
vector<int> subvec(int_arr + 1, int_arr + 4);



int ia[3][4] =
{
	{0, 1, 2, 3},
	{4, 5, 6, 7},
	{8, 9, 10, 11}
};

其中内层嵌套的花括号并非必须的
int ia[3][4] = {{0}, {4}, {8}}; //初始化每一行的第一个元素
int ix[3][4] = {0, 3, 6, 9};

ia[2][3] = arr[0][0][0]; //用arr的首元素为ia最后一行的最后一个元素赋值
int (&row)[4] = ia[1]; //把row绑定到ia的第二个4元素数组上

constexpr size_t rowCnt = 3, colCnt = 4;
int ia[rowCnt][colCnt];
for (size_t i = 0; i != rowCnt; ++i)
{
	for (size_t j = 0; j != colCnt; ++j)
	{
		ia[i][j] = i * colCnt + j;
	}
}

范围for
size_t cnt = 0;
for (auto &row : ia)
	for(auto &col : row)
	{
		col = cnt;
		++cnt;
	}


for (const auto &row : ia)
	for (auto col : row)
		cout << col << endl;


for (const auto row : ia)
	for (auto col : row)
无法编译 因为初始化row时会自动将这些数组形式的元素转换成指向该数组内首元素的指针
这样得到的 row 类型就是int*

要使用范围for语句处理多维数组，除了最内层的循环外，其他所有循环的控制变量都应该是引用类型


int ia[3][4];
int (*p)[4] = ia;  //p指向含有4个整数的数组 p是指针
p = &ia[2];    //p指向ia的尾元素

int *ip[4];  //整型指针的数组
int (*ip)[4];//指向含有4个整数的数组


for (auto p = ia; p != ia + 3; ++p)   //p指向含有4个整数的数组
{
	for (auto q = *p; q != *p + 4; ++q) //*p是一个含有4个整数的数组 数组名自动转化成指向该数组首元素的指针
	{
		cout << *q << " "; //q是指向4个整数数组的首元素，也就是说，q指向一个整数
	}
	cout << endl;
}

for (auto p = begin(ia); p != end(ia); ++p)
{
	for (auto q = begin(*p); q != end(*p); ++q)
	{
		cout << q << " ";
	}
	cout << endl;
}


using int_array = int[4];
typedef int int_array[4]; //等价  4个整数组成的数组命名为int_array

for (int_array *p = ia; p != ia + 3; ++p)
{
	for (int *q = *p; q != *p + 4; ++q)
	{
		cout << *q << endl;
	}
	cout << endl;
}



######## 第四章  表达式 #########

当一个对象被用作右值的时候，用的是对象的值（内容）
当对象被用作左值的时候，用的是对象的身份（在内存中的位置）

假定p的类型是int*
因为解引用运算符生成左值 所以decltype(*p) 的结果是int&
因为取地址运算符生成右值 所以decltype(&p) 的结果是int**

cout << i << " " << ++i << endl;
<<运算符没有明确规定何时以及如何对运算符对象求值，所以该语句是未定义的

int i = 1024；
int k = -i;     //k是1024
bool b = true;
bool b2 = -b;   //b2是 true

C++ 11规定商一律向0取整 (即直接切除小数部分)

(-m)/n m/(-n)  ===>>>   -(m/n)
m%(-n)   ===>>>   m%n
(-m)%n   ===>>>   -(m%n)

false 会转成0 true会转成1
进行比较运算时除非比较的对象是布尔类型，否则不要使用布尔字面值 false和true作为运算的对象

int i = get_value();
while (i != 42)
{
	...
	i = get_value();
}

转化成

int i;
while ((i = get_value()) != 42)
{
	...
}

int i = 0, j;
j = ++i;   //j = 1, i = 1;
j = i++;   //j = 1, i = 2;

auto pbeg = v.begin();
while (pbeg != v.end() && *beg >= 0)
{
	cout << *pbeg++ << endl;
}


for (auto it = s.begin(); it != s.end() && !isspace(*it); ++it)
{
	*it = toupper(*it);
}

while (beg != s.end(); && !isspace(*beg))
{
	*beg = toupper(*beg++);   //false 该语句未定义
}
*beg = toupper(*beg);  //如果先求左侧的值
*(beg + 1) = toupper(*beg); //如果先求右侧的值



string findalgrade = (grade < 60) ? "fail" : "pass";
findalgrade = (grade > 90) ? "high pass"
							: (grade < 60) ? "fail" : "pass";

cout << ((grade < 60) ? "fail" : "pass");  //输出fail 和 pass
cout << (grade < 60) ? "fail" : "pass";   //输出1 或者 0
cout << grade < 60 ? "fail" : "pass";     //错误 试图比较 cout 和 60

第二条 等价于  cout << (grade < 60);
			   cout ? "fail" : "pass";

第三条 等价于  cout << grade;
			   cout < 60 ? "fail" : "pass";


建议位运算符用于处理无符号类型

quiz1 |= 1UL << 27; //表示学生27通过了测试
等价于 quiz1 = quiz1 | 1UL << 27;

quiz1 &= ~(1UL << 27); //学生27没有通过测试

bool status = quiz1 & (1UL << 27); //学生27是否通过了测试


sizeof 运算符

sizeof(type);
sizeof expr;    表达式结果类型的大小

Sales_data data, *p;
sizeof(Sales_data);      //存储Sales_data 类型的对象所占用的空间
sizeof data;             //data的类型大小，即sizeof(Sales_data);
sizeof p;                //指针所占空间的大小
sizeof *p;               //p所指类型的空间大小，即sizeof(Sales_data);
sizeof data.revenue;     //Sales_data的revenue成员对应类型的大小
sizeof Sales_data::revenue; //另一种获取revenue大小的方式

sizeof(ia) / sizeof(*ia);   //返回ia的元素数量
constexpr size_t sz = sizeof(ia) / sizeof(*ia);
int arr2[sz];


vector<int>::size_type cnt = ivec.size();
for (vector<int>::size_type ix = 0; ix != ivec.size(); ++ix, --cnt)
{
	ivec[ix] = cnt;
}

算数转换的规则定义了一套类型转换的层次，其中运算符的运算对象将转换成最宽的类型

其他隐式类型转换
数组转换成指针
指针的转换   nullptr 能转换成任意指针类型

转换成布尔类型
char *cp = get_string();
if (cp)...   如果cp不是0 条件为真
while (*cp)...  如果*cp不是空字符 条件为真

转换成常量
int i;
const int &j = i;   非常量转换成const int的引用
const int *p = &i;  非常量的地址转换成 const int的地址
int &r = j, *q = p; 错误 不允许const转换成非常量

类类型定义的转换
string s, t = "a value";  //字符串字面值转换成string类型
while (cin >> s)         //while 条件部分把cin 转换成bool值

显示转换

int i, j;
double slope = i / j;
cast-name<type> (expression);
其中type是转换的目标类型而 expression是要转化的值

cast-name 有

1 static_cast  只要不包含底层const 都可以使用static_cast
double slope = static_cast<double> (j) / i;
void *p = &d;   //任何非常量对象的地址都能存入void*
double *dp = static_cast<double*> p; //将void* 转换回 初始的指针类型

2 dynamic_cast  运行时类型识别

3 const_cast   只能改变运算对象的底层const
const char *cp;
char *q = static_cast<char*> (cp); //错误 static_cast不能转换const性质
static_cast<string> (cp) //正确 字符串字面值转换成string类型
const_cast<string>(cp) //错误 const_cast只改变常量属性

4 reinterpret_cast 通常为运算对象的位模式提供较低层次上的重新解释
int *ip;
char *pc = reinterpret_cast<char*>(ip);
pc所指的真实对象是一个int 而非字符


####### 第五章 语句 ########

vector<int> v;
int i;
while (cin >> i)
{
	v.push_back(i);
}
auto beg = v.begin();
while (beg != v.end() && *beg >= 0)
{
	++beg;
}
if (beg == v.end())
{
	//
}

for (decltype(s.size() index = 0;
	 index != s.size() && !isspace(s[index]); ++index)
	 {
		 s[index] = toupper(s[index]);
	 }

for (decltype(v.size()) i = 0, sz = v.size(); i != sz; i++)
{
	v.push_back(v[i]);
}

vector<int> v = {0,1,2,3,4,5,6,7,8,9};
for (auto &r : v)
{
	r *= 2;
}

break语句 终止他最近的while do while for 或switch 语句
从这些语句之后的第一条语句开始执行
continue语句 终止最近的循环中的当前迭代，立即开始下一次迭代

string buf;
while (cin >> buf && !buf.empty())
{
	if (buf[0] != '_')
	{
		continue;
	}
}
goto end;
int ix = 10;
end:
	ix = 42; //false goto绕过了ix的声明

begin:
	int sz = get_size();
	if (sz <= 0)
	{
		goto begin;
	}

if (item1.isbn() != item2.isbn())
{
	throw runtime_error("Data must refer to same isbn"); //defined in stdexcept.h
}
cout << item1 + item2 << endl;

while (cin >> item1 >> item2)
{
	try
	{

	}catch(runtime_error err)
	{
		cout << err.what()
		<< "\n Try Again? Enter y or n" << endl;
		char c;
		cin >> c;
		if (!cin || c != 'n')
		{
			break; //跳出while循环
		}
	}
}

标准异常
exception.h 只报告异常的发生 不提供额外信息
stdexcept.h 定义了几种常用的异常类
new头文件定义了bad_malloc异常类型
type_info定义了bad_cast异常类型




####### 第六章 函数 #######

int fact(int val)
{
	int ret = 1;
	while (val > 1)
	{
		ret *= val--;
	}
	return ret;
}

int main()
{
	int j = fact(5);
	cout << "5! is" << j << endl;
	return 0;
}

fact函数的等价形式：
int val = 5;
int ret = 1;
while (val > 1)
{
	ret *= val--;
}
int j = ret;

int f1(int v1, v2) //false
int f2(int v1, int v2) //true
函数返回的类型不能是数组类型或函数类型
但可以是指向数组的指针 或 函数的指针

只存在于块执行期间的对象称为 自动对象

size_t count_calls()
{
	static size_t ctr = 0;  //局部静态变量
	return ++ctr;
}
int main()
{
	for (size_t i = 0; i != 10; ++i)
	{
		cout << count_calls() << endl;
	}
	return 0;
}  //每次执行count_calls函数时，变量ctr的值 都已经存在并且等于函数上一次退出时 ctr的值

函数声明
void print(vector<int>::const_iterator beg, vector<int>::const_iterator end);
含有函数声明的头文件应该被包含到定义函数的源文件中

void reset(int *ip) //指针形参
{
	*ip = 0; //改变指针ip所指对象的值
	ip = 0;  //只改变了ip的局部拷贝 实参未被改变
}

int i = 42;
reset(&i);   //改变i的值 并非地址
cout << "i = " << i << endl; //输出i = 0

void reset(int &i)  //引用形参
{
	i = 0; //改变了i所引对象的值
}
int j = 42;
reset(j);  //j采用传引用方式 它的值被改变
cout << "j = " << j << endl; //输出j = 0

使用引用 避免拷贝
bool isShorter(const string &s1, const string &s2)
{
	return s1.size() < s2.size();
}
如果函数无须改变引用形参的值 最好将其声明为常量引用

string::size_type find_char(const string &s, char c, string::size_type &occurs)
{
	auto ret = s.size();
	occurs = 0;
	for (decltype(ret) i = 0; i != s.size(); ++i)
	{
		if (s[i] == c)
		{
			if(ret == s.size())
				ret = i;
		}
		++occurs;
	}
	return ret;
}

auto index = find_char(s, 'o', ctr);
调用完成后，ctr表示o出现的次数 index 表示o第一次出现的位置
否则 index 等于s.size() ctr = 0

int i = 0;
const int *cp = &i;  //正确 但是cp不能改变i
const int &r = i;   //正确 但是 r 不能改变i
const int &r2 = 42; //正确
int *p = cp;    //错误 p的类型与cp的类型不匹配
int &r3 = r;    //错误 r3的类型与r的类型不匹配
int &r4 = 42;   //错误 不能用字面值初始化一个非常量引用 允许字面值初始化常量引用

int i = 0;
const int ci = i;
string::size_type ctr = 0;
调用形参类型为int* 的reset函数
reset(&i);   正确
reset(&ci);  错误 不能用指向const int对象的指针初始化int *
调用形参类型为int& 的reset函数
reset(i);    正确
reset(ci);   错误 不能把普通引用绑定到const对象ci上
reset(42);   错误 不能把普通引用绑定到字面值上
reset(ctr);  错误 类型不匹配ctr是无符号类型

void print(const int*)
void print(const int[])
void print(const int[10])
三个print函数等价 每个都有一个const int*类型的形参
int i = 0, j[2] = {0, 1};
print(&i);
print(j);

管理指针形参三种常用技术
使用标记指定数组长度
使用标准库规范
int j[2] = {0, 1};
print(begin(j), end(j));
显式传递一个表示数组大小的形参
void print(const int ia[], size_t size)
{
	for (size_t i = 0; i != size; ++i)
	{
		cout << ia[i] << endl;
	}
}
int j[] = {0, 1};
print(j, end(j) - begin(j));

void print(int (&arr)[10]) //&arr[10] 表示引用的数组 (&arr)[10] 表示10个整数的整形数组的引用
{
	for (autp item : arr)
		cout << item << endl;
}

void print(int (*matrix)[10], int rowSize) {/* */}
等价与 void print(int matrix[][10], int rowSize) {/* */}
matrix声明成指向含有10个整数的数组的指针

实参数量未知但是全部实参的类型都相同 可以使用
initializer_list类型的形参
initializer_list<string> ls;
和vector不一样的是 initializer_list对象中的元素永远都是常量值

void error_msg(initializer_list<string> il)
{
	for (auto beg = il.begin(); beg != il.end(); ++beg)
	{
		cout << *beg << " ";
	}
	cout << endl;
}

if (expected != actual)
{
	error_msg({"functionX", expected, actual});
}
else
{
	error_msg({"functionX", "okay"});
}

void error_msg(ErrCode e, initializer_list<string> il)
{
	cout << e.msg() << endl;
	for (const auto &elem : il)
	{
		cout << elem << " ";
	}
	cout << endl;
}

if (expected != actual)
{
	error_msg(ErrCode(42), {"functionX", expected, actual});
}
else
{
	error_msg(ErrCode(0), {"functionX", "okay"});
}

在含有return语句的循环后面应该有一条return语句
如果没有的话 该程序是错误的 很多编译器都无法发现此类错误

const string &shorterString(const string &s1, const string &s2)
{
	return s1.size() <= s2.size() ? s1 : s2;
}
其中形参和返回类型都是const string的引用，不管是调用函数还是返回结果都不会真正拷贝string对象

不要返回局部对象的 引用或指针 函数完成后 它所占用的存储空间就会被释放

返回类类型的函数和调用运算符
auto sz = shorterString(s1, s2).size()

引用返回左值
char &get_val(string &str, string::size_type ix)
{
	return str[ix];
}
int main()
{
	string s("a value");
	cout << s << endl;
	get_val(s, 0) = 'A';   //将s[0] 改为 ‘A	’
	cout << s << endl;
	shorterString("hi", "bye") = "X"; //错误 返回值是个常量
	return 0;
}

列表初始化返回值
vector<string> process()
{
	if (expected.empty())
		return {};
	else if (expected == actual)
		return {"functionX", "okay"};
	else
		return {"functionX", expected, actual};
}

int main()
{
	if (some_failure)
		return EXIT_FAILURE;
	else
		return EXIT_SUCCESS;
}

typedef int arrT[10];
using arrT = int[10];
arrT* func(int i);  //func返回一个指向含有10个整数的数组的指针

int (*func(int i))[10];
(*func(int i))[10] 表示解引用func的调用将得到一个大小为10的数组

尾置返回类型
auto func(int i) -> int(*)[10];
func 接收一个int类型的实参 返回一个指针 该指针指向含有10个整数的数组  

int odd[] = {1,3,5,7,9};
int even[] = {0,2,4,6,8};
decltype(odd) *arrPtr(int i)
{
	return (i % 2) ? &odd : &even; //返回一个指向数组的指针
}

arrPtr 返回一个指向含有5个整数的数组的指针
decltype 并不负责把数组类型转换成对应的指针



函数重载

void print(const char *op);
void print(const int *beg, const int *end);
void print(const int ia[], size_t size);

Record lookup(const Account&)
bool lookup(const Account&) //false

Record lookup(Phone);
Record lookup(const Phone); //顶层const不影响传入函数的对象

Record lookup(Account&);     函数作用于Account的引用
Record lookup(const Account&); 新函数 作用于常量引用
Record lookup(Account*);     新函数 作用于指向account的指针
Record lookup(const Account*); 新函数 作用于指向常量的指针


string &shorterString(string &s1, string &s2)
{
	auto &r = shorterString(const_cast<const string&>(s1), const_cast<const string&>(s2));
	return const_cast<string&> (r);
}
首先将实参强制转化成对const的引用
然后调用shorterString的const版本
const版本返回对const string的引用 这个引用事实上绑定在了某个初试的非常量实参上
因此我们可以将其再转换回一个普通的string&

<<<<<<< HEAD
=======
string read();
void print(const string &);
void print(double);
void fooBar(int ival);
{
	bool read = false; 新作用域 隐藏了外部的read
	string s = read(); 错误 read是一个bool值而非函数
	void print(int);   新作用域 隐藏了之前的print
	print("Value: ");  错误print(const string &) 被隐藏掉了
	print(ival);       正确
	print(3.14);       正确 调用print(int) print(double) 被隐藏掉了
}

名字查找发生在类型检查之前


void print(const string &);
void print(double);
void print(int);
void fooBar(int ival)
{
	print("Value: ");  调用print(const string &)
	print(ival);       调用print(int)
	print(3.14);       调用print(double)
}

1.默认实参
typedef string::size_type sz;
string screen(sz ht = 24, sz wid = 80, char backgrnd = ' ');
一旦某个形参被赋予了默认值 它后面的所有形参都必须有默认值
设计时  尽量不让默认形参出现在前面

在给定作用域中 一个形参只能被赋予一次默认实参
string screen(sz, sz, char = ' ');
string screen(sz, sz, char = '*');     错误 重复声明
string screen(sz = 24, sz = 80, char); 正确 添加默认实参

局部变量不能作为默认实参


内联函数和constexpr函数
内联函数可避免函数调用的开销 在调用点内联的展开
inline const string & shorterString(const string &s1, const string &s2)
{
	return s1.size() <= s2.size() ? s1 : s2;
}

constexpr int new_sz();
constexpr int foo = new_sz();

constexpr size_t scale(size_t cnt)
{
	return new_sz() * cnt;
}

int arr[scale(2)];  scale(2) 是常量表达式
int i = 2;
int a2[scale(i)]; 错误 scale(i) 不是常量表达式

constexpr 函数不一定返回常量表达式

把内联函数和constexpr函数放在头文件


if (word.size() < threshold)
{
	cerr << "Error: " << __FILE__
	     << ": in function " << __func__
	     << "at line " << __LINE__ << endl;
	     << "Compiled on " << __DATE__
	     << "at " << __TIME__ 
	     << "word read was " << word
	     << ": Length too short " << endl; 
}

void f();
void f(int);
void f(int, int);
void f(double, double = 3.14);
f(5.6); 调用void f(double, double);

对于调用(42, 2.56) 存在二义性
因为重载函数时应尽量避免强制类型转换

void ff(int);
void ff(short);
ff('a');   char提升为int 调用ff(int)

void manip(long);
void manip(float);
manip(3.14);  二义性


函数指针
bool lengthCompare(const string &, const string &);
bool (*pf)(const string &, const string &);

bool *pf(const string &, const string &);
声明一个名为pf的函数 该函数的返回值是bool*

pf = lengthCompare;
pf = &lengthCompare;

bool b1 = pf("hello", "goodbye");
bool b2 = (*pf)("hello", "goodbye");
bool b3 = lengthCompare("hello", "goodbye");
三个等价调用


void ff(int*);
void ff(unsigned int);
void (*pf1)(unsigned int) = ff;  pf1 调用ff(unsigned)

void (*pf2)(int) = ff; 错误 没有任何一个ff与该形参列表匹配
double (*pf3)(int*) = ff; 错误 ff与pf3返回类型不匹配

void useBigger(const string &s1, const string &s2, bool pf(const string &, const string &));
useBigger(s1, s2, lengthCompare);

typedef bool Func(const string&, const string &);
typedef decltype(lengthCompare) Func2;     Func 和 Func2是函数类型

typedef bool (*FuncP)(const string&, const string &);
typedef decltype(lengthCompare) *FuncP2;   FuncP 和 FuncP2是指向函数的指针

useBigger的等价声明
void useBigger(const string&, const string&, Func);
void useBigger(const string&, const string&, FuncP2);

返回指向函数的指针
using F = int(int*, int);  F 是函数类型
using PF = int(*)(int*, int); PF是指针类型

PF f1(int);  f1返回函数的指针
F f1(int);   错误 F是函数类型 f1不能返回一个函数
F *f1(int);  正确 显式的指向返回类型是指向函数的指针

int (*f1(int))(int*, int);

auto f1(int) -> int(*)(int*, int);

string::size_type sumLength(const string&, const string&);
string::size_type largerLength(const string&, const string&);
decltype(sumLength) *getFcn(const string&);
根据形参的取值 getFcn函数返回指向 sumLength或者largerLength 的指针


######### 第七章 类 #########



Sales_data total;
if (read(cin, total))  //读入第一笔交易
{
	Sales_data trans;
	while (read(cin, trans))    //读入剩余交易
	{
		if (total.isbn() == trans.isbn())
		{
			total.combine(trans);
		}
		else
		{
			print(cout, total) << endl;
			total = trans;   //处理下一本书
		}
	}
	print(cout, total) << endl;  //输出最后一条交易
}
else
{
	cerr << "No data?!" << endl;
}
>>>>>>> 0232c5d82aba99156be67bc0d16106a92e83f393


struct Sales_data
{
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
	double avg_price() const;

	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
非成员接口函数
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
定义在类内部的函数是隐式的inline函数


我们不能显式的定义自己的this指针
因为this是指向常量的指针 所以常量成员函数不能改变它的对象的内容
在上例中isbn可以读取调用它的对象的数据成员 但是不能写入新值

double Sales_data::avg_price() const
{
	if (units_sold)
		return revenue / units_sold;
	else
		return 0;
}

Sales_data& Sales_data::combine(const Sales_data &rhs)
{
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;   //返回total的引用
}
total.combine(trans);

istream &read(istream &is, Sales_data &item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;
	return is;
}
ostream &print(ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " "
	   << item.revenue << " " << item.avg_price();

	return os;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
	Sales_data sum = lhs;  //lhs的数据成员拷贝给sum
	sum.combine(rhs);
	return sum;
}

构造函数
构造函数和类型相同 和其他函数不一样的是 构造函数没有返回类型
构造函数不能被声明为const

只有当类没有声明任何构造函数时，编译器才会自动地生成默认构造函数

struct Sales_data
{
	Sales_data() = default;   //合成默认构造函数
	Sales_data(const std::string &s) : bookNo(s){ }   //构造函数初始值列表
	Sales_data(const std::string &s, unsigned n, double p):bookNo(s), units_sold(n), revenue(p * n){ }
	Sales_data(std::istream &);

	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
	double avg_price const;
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

Sales_data::Sales_data(std::istream &is)
{
	read(is, *this);
}

<<<<<<< HEAD
=======
定义在public说明符之后的成员在整个程序内可被访问 public成员定义类的接口

定义在private说明符之后的成员可以被类的成员函数访问，但是不能被使用该类的代码访问
private部分封装了类的实现细节

class Sales_data
{
public:
	Sales_data() = default;   //合成默认构造函数
	Sales_data(const std::string &s) : bookNo(s){ }   //构造函数初始值列表
	Sales_data(const std::string &s, unsigned n, double p):bookNo(s), units_sold(n), revenue(p * n){ }
	Sales_data(std::istream &);
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
private:
	double avg_price const{ return units_sold ? revenue / units_sold : 0; }
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

类可以在它的第一个访问说明符之前定义成员 如果我们用struct 这些成员是public的
如果用class 这些成员是private的

友元
class Sales_data
{
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::istream &read(std::istream&, Sales_data&);
	friend std::ostream &print(std::ostream&, const Sales_data&);

public:
	Sales_data() = default;   //合成默认构造函数
	Sales_data(const std::string &s) : bookNo(s){ }   //构造函数初始值列表
	Sales_data(const std::string &s, unsigned n, double p):bookNo(s), units_sold(n), revenue(p * n){ }
	Sales_data(std::istream &);
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);
private:
	double avg_price const{ return units_sold ? revenue / units_sold : 0; }
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};

友元声明 只出现在类定义的内部 友元不是类的成员 也不受它所在区域访问控制级别的约束

class Screen
{
public:
	typedef std::string::size_type pos; // using pos = std::string::size_type;
	Screen() = default;
	Screen(pos ht, pos wd, char c) : height(ht), width(wd), contents(ht * wd, c) {}
	//cursor成员隐式的使用了类内初始值 如果类内不存在cursor的类内初始值 我们就需要显示初始化
	char get() const { return contents[cursor]; }    //读取光标字符 隐式内联
	//定义在类内部的成员函数是自动inline的
	inline char get(pos ht, pos wd) const;     //显式内联
	Screen &move(pos r, pos c);
private:
	pos cursor = 0;
	pos height = 0, width = 0;
	std::string contents;
};

inline Screen &Screen::move(pos r, pos c)   //在函数定义处制定inline
{
	pos row = r * width;
	cursor = row + c;
	return *this;
}

char Screen::get(pos r, pos c) const   //在类的内部声明成inline
{
	pos row = r * width;
	return contents[row + c];
}

Screen myScreen;
char ch = myScreen.get();  //调用Screen::get()
ch = myScreen.get(0, 0);   //调用Screen::get(pos, pos)

class Screen
{
public:
	void some_member() const;
private:
	mutable size_t access_ctr;   //即使在一个const对象也能被修改
};

void some_member() const
{
	++access_ctr;
}

class Window_mgr
{
private:
	std::vector<Screen> screens{Screen(24, 80, ' ')};
	//我们使用单独的元素值对vector成员执行了列表初始化
};
当我们提供一个类内初始值时，必须以符号= 或者花括号表示

class Screen
{
public:
	Screen &set(char);
	Screen &set(pos, pos, char);
};

inline Screen &Screen::set(char c)
{
	contents[cursor] = c;  //设置当前光标所在位置的新值
	return *this;  //this作为左值返回
}

inline Screen &Screen::set(pos r, pos col, char ch)
{
	contents[r * width + col] = ch;
	return *this;
}

Sales_data item1;
class Sales_data item1; //等价声明


myScreen.move(4, 0).set('#'); 在同一对象上执行
如果 move 返回Screen而非 Screen&

Screen temp = myScreen.move(4, 0);  //对返回值进行拷贝
temp.set("#"); 不会改变myScreen的costents

一个const成员函数 如果以引用的形式返回*this 那么它的返回类型将是常量引用

class Screen
{
public:
	Screen &display(std::ostream &os)
	{
		do_display(os); 
		return *this;
	}
	const Screen &display(std::ostream &os) const
	{
		do_display(os);
		return *this;
	}
private:
	void do_display(std::ostream &os) const { os << contents; }	
};

Screen myScreen(5, 3);
const Screen blank(5, 3);
myScreen.set('#').display(cout); //调用非常量版本
blank.display(cout);   //调用常量版本


class Link_screen
{
	Screen window;
	Link_screen *next;
	Link_screen *prev;
};


class Screen
{
	friend class Window_mgr;
};

class Window_mgr
{
public:
	using ScreenIndex = std::vector<Screen>::size_type;
	void clear(ScreenIndex);
private:
	std::vector<Screen> screens{Screen(24, 80, ' ')};
};


void Window_mgr::clear(ScreenIndex i)
{
	Screen &s = screens[i];
	s.contents = string(s.height * s.width, ' ');
}
当Screen将 Window_mgr 指定为其友元之后Screen 的所有成员对于Window_mgr就变成可见的了
友元不具有传递性

class Screen
{
	friend void Window_mgr::clear(ScreenIndex);
};
某个成员函数作为友元
方法：
首先定义Window_mgr类，其中声明clear函数 但是不能定义它
在clear使用Screen的成员之前必须先声明Screen

接下来定义Screen 包括对于clear的友元声明
最后定义clear 此时才可以使用Screen的成员

extern std::ostream& storeOn(std::ostream &, Screen &);
extern BitMap& StoreOn(BitMap &, Screen &);
class Screen
{
	friend std::ostream& storeOn(std::ostream &, Screen &);
};
接受BitMap&作为参数版本仍然不能访问Screen

struct X
{
	friend void f() {}
	X() { f(); }     //错误 f还没有被声明
	void g();
	void h();
};

void X::g() { return f(); }   //错误 f还没有没声明
void f();					  //声明那个定义在X中的函数
void X::h() { return f(); }   //正确：现在f的声明在作用域中

友元声明的作用是影响访问权限 它本身并非普通意义上的声明

Screen::pos ht = 24, wd = 80;  使用Screen定义的pos类型
Screen scr(ht, wd, ' ');
Screen *p = &scr;
char c = scr.get();     访问scr对象的get成员
c = p->get();           访问p所指对象的get成员

一个类就是一个作用域

class Window_mgr
{
public:
	ScreenIndex addScreen(const Screen&);
};

Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen &s)
{
	screen.push_back(s);
	return screens.size() - 1;
}
返回类型需要指明哪个类定义了它

类的定义分两步处理
首先，编译成员的声明
直到类全部可见后才编译函数体
这种两阶段的处理方式 只适用于成员函数中使用的名字

typedef double Money;
string bal;
class Account
{
public:
	Money balance() { return bal; }  //返回bal成员 而非外层的string
private:
	Money bal;
};

看到balance声明时 将在Account类范围内寻找对Money的声明
没有匹配到会在Account的外层作用域中查找


不要把成员名字作为参数或者其他局部变量使用

int height;
class Screen
{
public:
	typedef std::string::size_type pos;
	void setHeight(pos);
	pos height = 0;
};
Screen::pos verify(Screen::pos);
void Screen::setHeight(pos var)
{
	height = verify(var);
}
全局函数verify的声明在Screen类的定义之前是不可见的
名字查找的第三部包括了成员函数出现之前的全局作用域
在此例中 verify的声明位于setHeight的定义之前 因此可以被正常使用

class ConstRef
{
public:
	ConstRef(int ii);
private:
	int i;
	const int ci;
	int &ri;
};
成员ci和ri都必须初始化

ConstRef::ConstRef(int ii)
{//赋值
	i = ii;    正确
	ci = ii;   错误不能给const赋值
	ri = i;    错误ri没有初始化
}
随着构造函数体一开始执行 初始化就完成了
正确形式：
ConstRef::ConstRef(int ii): i(ii), ci(ii), ri(i) { }
如果成员是const 引用或者属于某种未提供默认构造函数的类类型
我们必须通过构造函数初始值列表为这些成员提供初值

初始化和赋值 事关效率问题
前者直接初始化数据成员  后者先初始化再赋值

成员的初始化顺序与他们在类定义的出现顺序一致

class Sales_data
{
public:
	Sales_data(std::string s = ""): bookNo(s) { }
	//定义默认构造函数 令其与只接受一个string实参的构造函数功能相同
	Sales_data(std::string s, unsigned cnt, double rev):
		bookNo(s), units_sold(cnt), revenue(rev) { }
	Sales_data(std::istream &is) { read(is, *this); }
};

当没有给定实参或者给定一个string实参时 两个版本的类创建了相同的对象
因为我们不提供实参也能调用上述的构造函数 所以该构造函数实际上为我们的类提供了默认构造函数

委托构造函数
class Sales_data
{
public:
	Sales_data(std::string s, unsigned cnt, double price): bookNo(s), units_sold(cnt), revenue(cnt * price) { }
	//非委托构造函数使用对应的实参初始化成员
	Sales_data(): Sales_data("", 0, 0) { }
	//其余构造函数全部委托给另一个构造函数
	Sales_data(std::string s): Sales_data(s, 0, 0) { }
	Sales_data(std::istream &is): Sales_data() { read(is, *this); }
}; 


隐式的类类型转换
string null_book = "9-999-99999-9";
构建一个临时的Sales_data对象
该对象的units_sold revenue = 0, bookNo = null_book
item.combine(null_book);

编译器只允许一步类类型转换
item.combine("9-999-99999-9"); 错误
item.combine(string("9-999-99999-9")); 显示转换成string 隐式转换成 Sales_data
item.combine(Sales_data("9-999-99999-9")); 隐式转换成string 显示转换成 Sales_data

item.combine(cin); cin转换成 Sales_data 执行了一个接受 istream对象的 Sales_data 构造函数

抑制构造函数定义的隐式转换
class Sales_data
{
	Sales_data() = default;
	Sales_data(const std::string &s, unsigned n, double p): bookNo(s), units_sold(n), revenue(p*n) { }
	explicit Sales_data(const std::string &s): bookNo(s) { }
	explicit Sales_data(std::istream &);
};   //explicit 只在类内声明构造函数时使用 在类外部定义不能重复

item.combine(null_book);
item.combine(cin);
两个调用都将错误

explicit 构造函数只能用于直接初始化
Sales_data item1(null_book);   正确 直接初始化
Sales_data item2 = null_book;  错误 不能将explicit 构造函数用于拷贝形式的初始化过程

item.combine(Sales_data(null_book));  正确 实参是一个显式构造的 Sales_data对象
item.combine(static_cast<Sales_data>(cin)); 正确 static_cast 可以使用explicit 的构造函数

聚合类
满足条件：
所有成员都是public
没有定义任何构造函数
没有类内初始值
没有基类 也没有virtual函数
struct Data
{
	int ival;
	string s;
};

Data val1 = { 0, "anna" };
初始化顺序要和声明的顺序一致

字面值常量类
数据成员都必须是字面值类型
类必须至少含有一个constexpr构造函数
如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是一条常量表达式
	如果成员属于某种类类型 则初始值必须使用成员自己的constexpr构造函数
类必须使用析构函数的默认定义 该成员负责销毁对象

class Debug
{
public:
	constexpr Debug(bool b = true): hw(b), io(b), other(b) { }
	constexpr Debug(bool h, bool i, bool o): hw(h), io(i), other(o) { }
	constexpr bool any() { return hw || io || other; }
	void set_io(bool b) { io = b; }
	void set_hw(bool b) { hw = b; }
	void set_other(bool b) { other = b; }
private:
	bool hw;
	bool io;
	bool other;
};
constexpr构造函数必须初始化所有数据成员 初始值或者使用constexpr构造函数 或者一条常量表达式
constexpr构造函数用于生成 constexpr对象 以及 constexpr函数的参数或者返回类型

constexpr Debug io_sub(false, true, false);
if (io_sub.any())   //等于if(ture)
{
	cerr << "print appropriate error messages" << endl;
}
constexpr Debug prod(false);
if (prod.any())   //等于if(false)
{
	cerr << "print an error message" << endl;
}

声明静态成员
class Account
{
public:
	void calculate() { amount += amount * interestRate; }
	static double rate() { return interestRate; }
	static void rate(double);
private:
	std::string owner;
	double amount;
	static double interestRate;
	static double initRate();
};
类的静态成员 存在任何对象之外 对象中的不包含任何与静态数据成员有关的数据
因此每个Account对象将包含两个数据成员 owner和amount 只存在一个interestRate且被所有Account对象共享

静态成员函数不与任何对象绑定在一起  不包含 this指针

double r;
r = Account::rate();  使用作用域运算符访问静态成员

Account ac1;
Account *ac2 = &ac1;
r = ac1.rate();
r = ac2->rate();

类外部定义静态成员时 不能重复static关键字 该关键字只出现在类内部的声明语句
void Account::rate(double newRate)
{
	interestRate = newRate;
}

静态数据成员不是由类的构造函数初始化的 必须在类的外部定义和初始化每个静态成员
和其他对象一样 一个静态数据成员只能定义一次

定义并初始化一个静态成员
double Account::interestRate = initRate();

静态成员的类内初始化

class Account
{
public:
	static double rate() { return interestRate; }
	static void rate(double);
private:
	static constexpr int period = 30; //要求静态成员必须是字面值常量类型的constexpr
	double daily_tbl[period];
};
如果在类的内部提供了一个初始值 则成员的定义 不能再指定一个初始值了

constexpr int Account::period;  不带初始值的 静态成员的定义 初始值在内部提供
即使一个常量静态数据成员在类内部被初始化了 通常情况下也应该在类的外部定义一下该成员

class Bar
{
public:
	//...
private:
	static Bar mem1;  正确 静态成员可以是不完全类型
	Bar *mem2;        正确 指针成员可以是不完全类型
	Bar mem3;         错误 数据成员必须是完全类型
};

class Screen
{
public:
	Screen& clear(char bkground);
private:
	static const char bkground;
};
静态成员可以作为默认实参 非静态数据成员不能作为默认实参

######## 第八章 IO库 #########

iostream  读写流的基本类型
fstream   读写命名文件的基本类型
sstream   读写内存string对象的基本类型

IO对象无 拷贝和赋值
ofstream out1, out2;
out1 = out2;    错误 不能对流对象赋值
ofstream print(ofstream);  错误 不能初始化ofstream参数
out2 = print(out2);   错误 不能拷贝流对象

确定一个流对象的状态 最简单的办法就是将它当做一个对象来使用
while (cin >> word)
{
	//ok ...
}

auto old_state = cin.rdstate(); 记住cin的当前状态
cin.clear();  使cin有效
process_input(cin);   使用cin
cin.setstate(old_state);  将cin设置为原有状态

cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);
复位failbit和badbit 保持其他标志位不变

cout << "hi!" << endl;    输出hi和一个换行 然后刷新缓冲区
cout << "hi!" << flush;   输出hi 然后刷新缓冲区 不附加任何额外字符
cout << "hi!" << ends;    输出hi和一个空字符 然后刷新缓冲区

cout << unitbuf;    所有输出后 都会立即刷新缓冲区 任何输出都立即刷新 无缓冲
cout << nounitbuf;  回到正常的缓冲方式

如果程序崩溃 输出缓冲区不会被刷新

当一个输入流被关联到一个输出流时，任何试图从输入流读取数据的操作都会先刷新 关联的数据流
ostream *old_tie = cin.tie(nullptr); cin不再与其他流关联
cin.tie(&cerr);    读取cin会刷新cerr而不是cout
cin.tie(old_tie);  重建cin 和 cout 的正常关联


ifstream 从一个给定文件读取数据
ofstream 向一个给定文件写入数据
fstream  可以读写给定文件

ifstream in(ifile);  构造一个ifstream并打开给定文件
ofstream out;        输出文件未关联到任何文件

ifstream input(argv[1]);   //打开销售记录文件
ofstream output(argv[2]);  //打开输出文件
Sales_data total;
if (read(input, total))
{
	Sales_data trans;
	while (read(input, trans))
	{
		if (total.isbn() == trans.isbn())
			total.combine(trans);
		else
		{
			print(output, total) << endl;
			total = trans;
		}
	}
	print(output, total) << endl;
}
else
{
	cerr << "NO data?!" << endl;
}
重要的是对read 和 print的调用
虽然两个函数定义时指定的形参分别是 istream& 和 ostream&
但是我们可以向它们传递fstream对象

ifstream in(ifile);
in.close();
in.open(ifile + "2");

for (auto p = argv + 1; p != argv + argc; ++p)
{
	ifstream input(*p);   //创建输出流并打开文件
	if (input)            //如果文件打开成功 处理此文件
	{
		process(input);
	}
	else
	{
		cerr << "couldn't open" + string(*p); 
	}
} //每个循环步input都会离开作用域 因此会被销毁
当一个fstream对象被销毁 close会自动调用

以out模式打开文件会丢弃已有数据
ofstream out("file1");
ofstream out2("file1", ofstream::out);
ofstream out3("file1", ofstream::out | ofstream::trunc);
这几句 file1 都被截断

ofstream app("file2", ofstream::app);
ofstream app2("file2", ofstream::out | ofstream::app);
为了保留文件内容 我们必须显式指定app的模式

ofstream out;    未指定文件打开模式
out.open("scratchpad");  模式隐含设置为输出和截断
out.close();     关闭out 以便我们将其用于其他文件
out.open("percious", ofstream::app); 模式为输出和追加
out.close();

istringstream 从string 读取数据
ostringstream 向string 写入数据
stringstream 即可从string读数据也可向string写数据

morgan 2015552368 8625550123
drew 9735550130
lee 6095550132 2015550175 8005550000

struct PersonInfo
{
	string name;
	vertor<string> phones;
};

string line, word;    
vector<PersonInfo> people;
while (getline(cin, line))
{
	PersonInfo info;
	istringstream record(line); //记录绑定到刚读入的行
	record >> info.name;        //读取名字
	while (record >> word)      //读取电话号码
	{
		info.phones.push_back(word);
	}
	people.push_back(info);
}
此循环 是从一个string而不是标准输入读取数据 当string中的数据全部读出后
同样会触发文件结束信号 在record上的下一个输入操作会失败


for (const auto &entry : people)
{
	ostringstream formatted, badNums;
	for (const auto &nums : entry.phones)
	{
		if (!valid(nums))
		{
			badNums << " " << nums;  //数的字符串形式存入badNums
		}
		else
		{
			formatted << " " << format(nums); //格式化的字符串写入 formatted
		}
	}
	if (badNums.str().empty())  //没有错误的数
	{
		os << entry.name << " "
			<< formatted.str() << endl;
	}
	else
	{
		cerr << "input error: " << entry.name
				<< "invalid number(s) " << badNums.str() << endl;
	}
}

######## 第九章 顺序容器 ########

vector<vector<string>> lines;
此处lines是一个vector 其元素类型是string 的 vector

假定noDefault 是一个没有默认构造默认函数的类型
vector<noDefault> v1(10, init);   正确 提供了元素初始化器
vector<noDefault> v2(10);         错误 必须提供一个元素初始化器

迭代器范围 [begin, end) 范围自begin开始 于end之前结束
while (begin != end)
{
	*begin = val; //正确 范围非空 因此begin指向一个元素
	++begin;      //移动迭代器 获取下一个元素
}

list<string>::iterator iter;
iter是通过list<string>定义的一个迭代器类型
vector<int>::difference_type count;
count是通过vector<int> 定义的一个difference_type 类型  带符号整数 足够保存两个迭代器之间的距离


与const指针和引用类似 可以将一个普通的iterator转换成const_iterator 但反之不行

list<string>::iterator it5 = a.begin(); 显式指定类型
list<string>::const_iterator it6 = a.begin(); 是iterator 还是 const_iterator 取决于a的类型
auto it7 = a.begin();  仅当a是const it7 是const_iterator
auto it8 = a.cbegin(); it8是 const_iterator

auto 与begin 和 end 结合使用的时候  获取迭代器的类型取决于容器类型
但是以c开头的版本 还是可以获得const_iterator的 不管容器的类型是什么


list<string> authors = {"milton", "shakespeare", "austen"}; 列表初始化 对于出array除外的容器类型初始化列表还隐含的定义了容器的大小
vector<const char*> articles = {"a", "an", "the"};
list<string> list2(authors);       正确
deque<string> authlist(authors);   错误 容器类型不匹配
vector<string> words(articles);    错误 容器类型不匹配
forword_list<string> words(articles.begin(), articles.end());   正确 可以将const char*元素转化为string

deque<string> authlist(authors.begin(), it);
it表示authors中的一个元素 拷贝元素 直到 但不包含 it指向的元素

与顺序容器大小相关的构造函数
vector<int> ivec(10, -1);     //十个元素 每个初始化为-1
list<string> sevc(10, "hi!"); //十个strings 每个初始化为 hi
forword_list<int> ivec(10);   //十个元素 每个初始化为0
deque<string> sevc(10);       //十个元素 每个初始化为空 string

只有顺序容器的构造函数才接受大小参数 关联容器并不支持

当定义一个array时 除了指定元素类型还要指定容器大小
array<int, 42>;
array<string, 10>;
使用 array类型 也要指定元素类型和大小
array<int, 10>::size_type i;  正确
array<int>::size_type j; 错误 array<int> 不是一个类型

array不支持普通容器的构造函数
array<int, 10> ia1;    10个默认初始化的int
array<int, 10> ia2 = {0,1,2,3,4,5,6,7,8,9};  列表初始化
array<int, 10> ia3 = {42};   ia3[0] 为 42 剩余元素为0

array 可以进行拷贝和对象赋值
int digs[10] = {0,1,2,3,4,5,6,7,8,9};
int cpy[10] = digs;             错误 内置数组不支持拷贝和赋值
array<int, 10> digits = {0,1,2,3,4,5,6,7,8,9};
array<int, 10> copy = digits;   正确

如果两个容器原来大小不同 赋值运算后两者的大小都与右边容器的原大小相同

array<int, 10> a1 = {0,1,2,3,4,5,6,7,8,9};
array<int, 10> a2 = {0};  所有元素均为0
a1 = a2;     替换a1中的元素
a2 = {0};    错误 不能将一个花括号列表赋值给数组
由于右边运算对象的大小 可能与左边运算对象的大小不同
array类型不支持 assign 也不允许用花括号包围的值列表进行赋值

assign的两种用法
list<string> names;
vector<const char*> oldstyle;
names = oldstyle;   //错误 容器类型不匹配
names.assign(oldstyle.cbegin(), oldstyle.cend());  //正确 将const char转化为string

list<string> slistl(1);  //1个元素 为空string
slistl.assign(10, "hiya!");  //10个元素 每个都是hiya！

vector<string> sevc1(10);
vector<string> sevc2(24);
swap(sevc1, sevc2);

除array外 swap不对任何元素进行 拷贝 删除或者插入操作 因此可以保证在常数时间内完成

除string外 指向容器的迭代器 引用指在swap操作后都不会失效
假定iter在swap之前指向sevc1[3] 的string 在swap之后它指向sevc2[3]的元素
交换两个array所需的时间与array中元素的数目成正比

forword_list 支持max_size 和 empty 但不支持size

如果两个容器都不是另一个的容器的前缀子序列 则他们的比较结果取决于第一个不相等的元素的比较结果

vector<int> v1 = {1,3,5,7,9,12};
vector<int> v2 = {1,3,9};
vector<int> v3 = {1,3,5,7};
vector<int> v4 = {1,3,5,7,9,12};
v1 < v2 true
v1 < v3 false
v1 == v4 true
v1 == v2 false

vector<Sales_data> storeA, storeB;
if (storeA < storeB) 错误 Sales_data没有<运算符

顺序容器操作
forward_list 有自己版本的 insert 和 emplace
forward_list 不支持push_back 和 emplace_back
vector 和 string 不支持 push_front 和 emplace_front

除array和forward_list 之外 每个顺序容器都支持push_back

string word;
while (cin >> word)
{
	container.push_back(word);
}

void pluralize(size_t cnt, string &word)
{
	if (cnt > 1)
	{
		word.push_back('s'); // word += 's'
	}
}

对象初始化容器时 或者将一个对象插入到容器中时 实际上放入到容器中的是对象值的一个拷贝

list<int> ilist;
for (size_t ix = 0; ix != 4; ++ix)
{
	ilist.push_front(ix);
}
最终list为 3,2,1,0

slist.insert(iter, "hello!"); 将hello！添加到iter之前的位置

vector<string> svec;
list<string> slist;
slist.insert(slist.begin(), "hello!");  //等价于 slist.push_front("hello!");

svec.insert(svec.begin(), "hello!");  //vector不支持push_front 但是我们可以插入到begin()之前 
//插入到vector末尾之外的任何位置都可能很慢

svec.insert(svec.end(), 10, "anna"); 将十个元素插入到svec的末尾 并将所有元素都初始化为 string "anna"

vector<string> v = {"quasi", "simba", "frollo", "scar"};
slist.insert(slist.begin(), v.end() - 2, v.end());  //将v的最后两个元素添加到slist的开始位置
slist.insert(slist.end(), {"there", "word", "will", "go", "at", "the", "end"});
slist.insert(slist.begin(), slist.begin(), slist.end()); //运行时错误 迭代器表示拷贝的范围 不能指向与目标位置相同的容器

接受元素个数或者范围的insert版本 返回指向 第一个新加入元素的迭代器 如果范围为空 不插入任何元素 insert操作会将第一个参数返回

list<string> lst;
auto iter = lst.begin();
while (cin >> word)
{
	iter = lst.insert(iter, word); 等价于调用 push_front
}

使用 emplace 操作 这些操作构造而不是拷贝
emplace_front ---- push_front
emplace ---- insert
emplace_back ---- push_back

假定c保存 Sales_data 元素
在c的末尾构造一个 Sales_data 对象
c.emplace_back("978-0590353403", 25, 15.99); //使用三个参数的Sales_data构造函数 
在容器管理的内存空间直接创建对象
c.push_back("978-0590353403", 25, 15.99);  //错误 没有接受三个参数的push_back版本
c.push_back(Sales_data("978-0590353403", 25, 15.99)); // 正确 创建一个临时的Sales_data对象 传递给push_back
创建一个局部临时对象 并将其压入容器中

c.emplace_back(); // 使用Sales_data 的默认构造函数
c.emplace(iter, "999-99999999"); //iter指向c中的一个元素 其中保存了Sales_data元素 使用 Sales_data(string)
c.emplace_front("978-0590353403", 25, 15.99); //使用Sales_data 接受三个类型参数的 构造函数

传递给emplace函数的参数必须与元素类型的构造函数相匹配

if (!c.empty())
{
	auto val = *c.begin(), val2 = c.front(); //val 和 val2 是c中第一个元素值的拷贝
	auto last = c.end();
	auto val3 = *(--last);  //val3 和val4 是c中最后一个元素值的拷贝   不能递减forward_list 迭代器
	auto val4 = c.back();  //forward_list 不支持   获取c中尾元素的引用
}

if (!c.empty())
{
	c.front() = 42;
	auto &v = c.back();  指向最后一个元素的引用
	v = 1024;            改变c中的元素
	auto v2 = c.back();  v2不是一个引用 它是c.back()的拷贝
	v2 = 0;              未改变c中的元素
}

vector<string> svec;  空 vector
cout << svec[0];      运行时错误 svec中没有元素
cout << svec.at(0);   抛出一个out_of_range异常

while (!ilist.empty())
{
	process(ilist.front());  对ilist的首元素进行一些处理
	ilist.pop_front();       完成处理后删除首元素
}

list<int> lst = {0,1,2,3,4,5,6,7,8,9};
auto it = lst.begin();
while (it != lst.end())
{
	if (*it % 2)     元素为奇数 删除此元素
	{
		it = lst.erase(it);
	}
	else
	{
		++it;
	}
}

elem1 = slist.earse(elem1, elem2);
删除两个迭代器表示的范围内的元素
返回指向最后一个被删元素之后位置的迭代器
调用后 elem1 == elem2

slist.clear() ---- slist.erase(slist.begin(), slist.end());

forward_list 没有定义insert emplace 和erase
           而是定义了insert_after emplace_after erase_after

在forward_list 中添加或者删除元素时  必须关注两个迭代器 一个指向我们要处理的元素 一个指向其前驱

forward_list<int> flst = {0,1,2,3,4,5,6,7,8,9};
auto prev = flst.before_begin();   首前元素
auto curr = flst.begin();          第一个元素
while (curr != flst.end())         仍有元素要处理
{
	if (*curr % 2)
	{
		curr = flst.erase_after(prev);   删除它并移动curr   返回一个指向被删元素 之后元素 的迭代器
	}
	else
	{
		prev = curr;
		++curr;
	}
}

改变容器大小
list<int> ilist(10, 42);  10个int 每个值是42
ilist.resize(15);		  5个值为0的元素添加到ilist的末尾
ilist.resize(25, -1);     将10个值为-1的元素添加到ilist的末尾
ilist.resize(5);          从ilist末尾删除20个元素

如果容器保存的是类类型元素 且 resize向容器添加新元素 则我们必须提供初始值
或者元素类型必须提供一个默认构造函数
resize不适用于array


！！！！每次改变容器的操作之后 都正确地重新定位迭代器

改变容器的循环程序
删除偶数元素 复制每个奇数元素
vector<int> vi = {0,1,2,3,4,5,6,7,8,9};
auto iter = vi.begin();   调用begin 而不是 cbegin 因为我们要改变vi
while(iter != vi.end())
{
	if (*iter % 2)
	{
		iter = vi.insert(iter, *iter); 复制当前元素 插到指定元素之前
		iter += 2； 向前移动迭代器 跳过当前元素以及插入到它之前的元素
	}
	else
		iter = vi.erase(iter);   删除偶数元素
								iter指向我们删除元素之后的元素
}

不要保存end返回的迭代器
auto begin = v.begin();
	end = v.end();
while(begin != end)
{
	++begin;
	begin = v.insert(begin, 42);
	++begin;
}
此代码未定义 在循环体内 我们向容器中添加一个元素 这个操作使保存在end中的迭代器 失效了
这个迭代器 不再指向v中的任何元素 或是v中尾元素之后的位置

更安全的方法 在每个循环步添加、删除元素后重新计算end
while (begin != v.end())
{
	++begin;
	begin = v.insert(begin, 42);
	++begin;
}

vector 增长
容器的size是指它已经保存的元素的数目 capacity则是在不分配新的内存空间的前提下它最多可以保存多少元素
vector<int> ivec;
cout << "ivec:size: " << ivec.size()
	<<"capacity: " << ivec.capacity() << endl;
for (vector<int>::size_type ix = 0; ix != 24; ++ix)
{
	ivec.push_back(ix);
}
cout << "ivec:size: " << ivec.size()
	<<"capacity: " << ivec.capacity() << endl;

	ivec:size: 0 capacity: 0
	ivec:size: 24 capacity: 32

ivec.reserve(50); 将capacity 至少设定为50 可能会更大
cout << "ivec:size: " << ivec.size()
	<<"capacity: " << ivec.capacity() << endl;

	ivec:size: 24 capacity: 50

while (ivec.size() != ivec.capacity())
{
	ivec.push_back(0);
}
cout << "ivec:size: " << ivec.size()
	<<"capacity: " << ivec.capacity() << endl;

	ivec:size: 50 capacity: 50

ivec.push_back(42);   再插入一个元素
cout << "ivec:size: " << ivec.size()
	<<"capacity: " << ivec.capacity() << endl;

	ivec:size: 51 capacity: 100

ivec.shrink_to_fit(); 要求归还内存 也只是一个请求

额外的string操作
构造string的方法
string s(cp, n);
string s(s2, pos2);
string s(s2, pos1, pos2);

char noNull[] = {'H', 'i'};
string s3(noNull); 未定义 noNull 不是以空字符结束
如果我们传递给构造函数一个计数值 则数组不必以空字符串结尾

s.substr(pos, n) 返回一个string 包含s中pos开始的n个字符的拷贝 pos默认值为0 n默认值为s.size() - pos

s.insert(s.size(), 5, '!');   s末尾插入5个感叹号
s.erase(s.size() - 5, 5);     s删除最后5个字符

const char *cp = "Stately, plump Buck";
s.assign(cp, 7);      s == "Stately"
s.insert(s.size(), cp + 7);  s == "Stately, plump Buck"  字符插入s[size()]处(不存在的) 元素之前的位置

string s = "some string", s2 = "some other string";
s.insert(0, s2); 在s中位置0之前插入s2的拷贝
s.insert(0, s2, 0, s2.size());  在s[0] 之前插入s2中 s2[0] 开始的s2.size()个字符

string s("c++ primer"), s2 = s;
s.insert(s.size(), " 4th Ed."); s == c++ primer 4th Ed.
s2.append(" 4th Ed."); 等价方法 将"4th Ed."追加到s2; s == s2

s.erase(11, 3); s == c++ primer Ed.
s.insert(11, "5th"); s == c++ primer 5th Ed 
s2.replace(11, 3, "5th"); s == s2
s2.replace(11, 3, "Fifth"); s == c++ primer fifth Ed.

string 搜索操作

string搜索函数返回string::size_type值 该类型是一个unsigned类型
string name("annabelle");
auto pos1 = name.find("anna"); pos1 == 0

string lowercase("annabelle");
pos1 = lowercase.find("Anna"); pos == npos

string numbers("0123456789"), name("r2d2");
auto pos = name.find_first_of(numbers);  返回1 name中第一个数字的下标

string dept("03714p3");
auto pos = dept.find_first_not_of(numbers);  返回5 字符p的下标

string::size_type pos = 0;
while((pos = name.find_first_of(numbers, pos)) != string::npos)
{
	cout << "found number at index: " << pos << "element is " << name[pos] << endl;
	++pos; 移动到下一个字符
}

string river("mississippi");
auto first_pos = river.find("is");  返回1
auto last_pos = river.rfind("is");  返回4

compare函数

数值转换
int i = 42;
string s = to_string(i); i转化为字符表示形式
double d = stod(s);      s转换为浮点数

string s2 = "pi = 3.14";
d = stod(s2.substr(s2.find_first_of("+-.0123456789")));

容器适配器
除了顺序容器外 标准库还定义了三个顺序容器适配器
stack queue 和 priority_queue
一个适配器是一种机制 能使某种事物的行为看起来像另外一种事物一样

deque<int> deq;
stack<int> stk(deq); 从deq拷贝元素到stk

stack<string, vector<string>> str_stk;  在vector上实现的空栈
stack<string, vector<string>> str_stk2(svec); str_stk2在vector上实现 初始化时保存svec的拷贝

栈适配器
stack<int> intStack;
for (size_t ix = 0; ix != 10; ++ix)
	intStack.push(ix);
while (!intStack.empty())
{
	int value = intStack.top();
	intStack.pop();
}

每个容器适配器都基于底层容器类型的操作定义了自己的特殊操作
我们只可以使用适配器操作 而不能使用底层容器类型的操作

队列适配器
quere 默认基于deque实现 priority_queue默认基于vector实现

####### 第十章 泛型算法 #######

int val = 42;
auto result = find(vec.cbegin(), vec.cend(), val);
cout << "the value" << val << (result == vec.cend() ? "is not present" : "is present") << endl;

find 返回第一个等于给定值元素的迭代器
	 如果范围中没有匹配元素 则find返回第二个参数来表示搜索失败

int ia[] = {27, 210, 12, 47, 109, 83};
int val = 83;
int* result = find(begin(ia), end(ia), val);
auto  result = find(ia + 1, ia + 4, val);
从ia[1] 开始 直到(但不包含) ia[4] 的范围内查找元素

迭代器令算法不依赖于容器 但是算法依赖于元素类型的操作

泛型算法只会运行在迭代器之上 执行迭代器的操作
算法永远不会改变底层容器的大小

算法的输入范围  是第一个元素 和尾元素之后位置

只读算法
int sum = accumulate(vec.cbegin(), vec.cend(), 0); 对vec中的元素求和 和的初值是0
第三个参数的类型决定了函数中使用哪个加法运算符 以及返回值的类型

string sum = accumulate(v.cbegin(), v.cend(), string(""));
将vector中的所有 string 元素连接起来
如果是string sum = accumulate(v.cbegin(), v.cend(), ""); 会发生编译错误 原因在于const char* 并没有+运算符

equal(roster1.cbegin(), roster1.cend(), roster2.cbegin());
roster1 可以是vector<string> roster2 可以是list<const char*>  但是要求能够使用==来比较两个序列中的元素
equal 基于一个非常重要的假定 假定第二个序列至少和第一个序列一样长

算法不会执行迭代器的操作 因此他们自身不可能改变容器的大小


写容器的算法
fill(vec.begin(), vec.end(), 0); 将每个元素重置为0
fill(vec.begin(), vec.begin() + vec.size() / 2, 10); 将容器的一个子序列设置为10

算法不检查写操作
vector<int> vec;
fill_n(vec.begin(), vec.size(), 0); 将所有元素重置为0

fill_n(vec.begin(), 10, 0); 错误 修改vec中10个不存在的元素
向目的位置迭代器写入数据的算法假定目标位置足够大 能容纳要写入的元素

插入迭代器 
当我们通过一个迭代器向容器元素赋值时 值被赋予迭代器指向的元素
而我们通过一个插入迭代器赋值时 一个与赋值号右侧值相等的元素被添加到容器中

vector<int> vec; 空向量
auto it = back_inserter(vec);  通过它赋值会将元素添加到vec中
*it = 42; vec中现在有一个元素42
back_inserter返回一个与该容器绑定的插入迭代器

我们常常使用back_inserter来创建一个迭代器 作为算法目的位置来使用
vector<int> vec;
fill_n(back_inserter(vec), 10, 0); 添加10个元素到vec

拷贝算法
int a1[] = {0,1,2,3,4,5,6,7,8,9};
int a2[sizeof(a1)/sizeof(*a1)];   a2与a1大小一样
auto ret = copy(begin(a1), end(a1), a2);  a1内容拷贝给a2
ret指向拷贝到a2的尾元素之后的位置

replace(ilist.begin(), ilist.end(), 0, 42);
replace_copy(ilist.cbegin(), ilist.cend(), back_inserter(ivec), 0, 42); 第三个迭代器参数指出调整后序列的保存位置

重排容器元素的算法
void elimDups(vector<string> &words)
{
	sort(words.begin(), words.end());
	auto end_unique = unique(words.begin(), words.end()); 每个只出现一次的单词出现在前部 返回不重复区域之后一个位置的迭代器
	//words的大小并未改变
	words.erase(end_unique, words.end()); //即使words中没有重复单词 这样调用 erase也是安全的
}

定制操作
向算法传递函数
谓词 元素类型必须能转换为谓词的参数类型
bool isShorter(const string &s1, const string &s2)
{
	return s1.size() < s2.size();
}
sort(words.begin(), words.end(), isShorter); 按长度由短至长排序words

排序算法 stable_sort 这种稳定排序算法维持相等元素的原有顺序
elimDups(words);    //字典序重排 并消除重复单词
stable_sort(words.begin(), words.end(), isShorter);  //按长度重新排序 长度相同的单词维持字典序
for (const auto &s : words)
{
	cout << s << " ";
}
cout << endl;

lambda 表达式
可调度对象（对于一个对象或一个表达式 如果可以对其使用调用运算符 则称它是可调用的）  
两种可调度对象 函数和函数指针 还有其他两种
重载了函数调用运算符的类 以及 lambda表达式

与任何函数类似 一个lambda具有一个返回类型 一个参数列表 和一个 函数体
与函数不同 lambda可以定义在函数内部

[capture list](parameter list) -> return type {function body}

auto f = []{ return 42; }; 定义一个可调用对象f 它不接受参数 返回42

cout << f() << endl; 打印42
如果lambda的函数体包含任意单一return语句之外的内容 且未指定返回类型 则返回void

与普通函数不同 lambda不能有默认参数
与 isShorter 功能完全相同的lambda：
[](const string &a, const string &b) { return a.size() < b.size(); }

调用 stable_sort:
stable_sort(words.begin(), words.end(), [](const string &a, const string &b) { return a.size() < b.size(); });

[sz](const string &a){ return a.size() >= sz; };
一个lambda只有在其捕获列表中捕获一个它所在函数的局部变量 才能在函数体中使用该变量

使用此lambda我们就可以查找第一个长度大于等于sz的元素
auto wc = find_if(words.begin(), words.end(), [sz](const string &a){ return a.size() >= sz; });
这里对find_if的调用返回一个迭代器 指向第一个长度不小于参数sz的元素 否则返回words.end()的一个拷贝

auto count = words.end() - wc;
cout << count << " " << make_plural(count, "word", "s")
     << " of length " << sz << " or longer" << endl;

make_plural 输出word 或者 words 具体输出取决于大小是否等于1

for_each(wc, words.end(), [](const string &s){ cout << s << " ";}); 打印长度大于等于给定值的单词 每个单词后面接一个空格

捕获列表只用于局部非static变量 lambda可以直接使用局部static变量和它所在函数之外声明的名字

完整的biggies
void biggies(vector<string> &words, vector<string>::string_type sz)
{
	//words字典序排序 删除重复单词
	elimDups(words);  

	//按照长度排序，长度相同的单词维持字典序
	stable_sort(words.begin(), words.end(), [](const string &a, const string &b) { return a.size() < b.size(); });
    
    //获取一个迭代器 指向第一个满足size() >= sz的元素 
	auto wc = find_if(words.begin(), words.end(), [sz](const string &a){ return a.size() >= sz; });
	
	//计算满足size>=sz的元素的数目
	auto count = words.end() - wc;  
	cout << count << " " << make_plural(count, "word", "s")
     	<< " of length " << sz << " or longer" << endl;

    //打印长度大于等于给定值的单词 每个单词后面跟一个空格
    for_each(wc, words.end(), [](const string &s){ cout << s << " ";});

    cout << endl;
}

值捕获
void fcn1()
{
	size_t v1 = 42;  局部变量
	auto f = [v1]{ return v1; };  将v1拷贝到名为f的可调用对象
	v1 = 0;
	auto j = f();  j为42 f保存可我们创建它时v1的拷贝
}
由于被捕获变量的值 是在lambda创建时拷贝 因此随后对其修改不会影响到lambda内对应的值

引用捕获
void fcn2()
{
	size_t v1 = 42;  局部变量
	auto f2 = [&v1]{ return v1; }; 对象f2包含v1的引用
	v1 = 0;
	auto j = f2(); j为0 f2保存v1的引用 而非拷贝
}
当我们在lambda函数体内使用此变量时 实际上使用的是引用所绑定的对象
引用捕获与返回引用有着相同的问题和限制 lambda捕获的都是局部变量

void biggies(vector<string> &words, vector<string>::string_type sz, ostream &os = cout, char c = ' ')
{
	//与之前例子一样的重排words的代码
	for_each(words.begin(), words.end(), [&os, c](const string &s){ os << s << c});
}
我们希望biggies函数可以接受一个ostream的引用用来输出数据 并接受一个字符作为分隔符
我们不能捕获ostream对象 因此捕获os的唯一方法就是捕获其引用

当以引用方式捕获一个变量时 必须保证在lambda执行时变量时存在的

隐式捕获
&告诉编译器采用捕获引用方式 =表示采用值捕获方式
wc = find_if(words.begin(), words.end(), [=](const string &s){ return s.size() >= sz; });
可以混用隐式捕获和显式捕获 但捕获列表第一个元素必须是 = 或者 &
void biggies(vector<string> &words, vector<string>::string_type sz, ostream &os = cout, char c = ' ')
{
	//与之前例子一样的重排words的代码
	for_each(words.begin(), words.end(), [&, c](const string &s){ os << s << c});
	for_each(words.begin(), words.end(), [=, &os](const string &s){ os << s << c});
}

可变lambda
void fcn3()
{
	size_t v1 = 42;
	auto f = [v1] () mutable { return ++v1; }; f可以改变它所捕获的变量的值
	v1 = 0;
	auto j = f(); j为43
}
如果我们希望能改变一个被捕获的变量的值 就必须在参数列表首加上关键字 mutable

一个引用捕获的变量是否（如往常一样）可以修改依赖于此引用指向的是一个const类型 还是一个非const类型
void fcn4()
{
	size_t v1 = 42;
	auto f2 = [&v1] {return ++v1; };
	v1 = 0;
	auto j = f2(); j 为1
}

transform(vi.begin(), vi.end(), vi.begin(), [](int i){ return i < 0 ? -i : i; });
将一个序列中的每个负数替换为其绝对值

transform(vi.begin(), vi.end(), vi.begin(), [](int i){ if (i < 0) return -i; else return i; });
错误 不能推断lambda的返回类型 需要使用尾置返回类型

transform(vi.begin(), vi.end(), vi.begin(), [](int i) -> int { if (i < 0) return -i; else return i; });


参数绑定

我们用在find_if调用中的lambda比较一个string和一个给定的大小
我们可以很容易的编写一个完成同样工作的函数
bool check_size(const string &s, string::size_type sz)
{
	return s.size() >= sz;
}
但是find_if接受一个一元谓词 为了用check_size 来代替此lambda 必须解决如何向sz形参传递一个参数的问题

auto newCallable = bind(callable, arg_list);
newCallable本身是一个可调用对象 
arg_list中的参数可能包含形如_n的名字 其中n是一个整数 这些参数是占位符 表示newCallable的参数
它们占据了传递给newCallable的参数的位置 _1表示newCallable的第一个参数

check6 是一个可调用对象 接受一个string类型的参数
并用此string和值6来调用check_size
auto check6 = bind(check_size, _1, 6);
此bind函数只有一个占位符 表示check6只接受单一参数
占位符出现在arg_list 的第一个位置 表示check6 的此参数对应check_size的第一个参数
此参数是一个const string& 因此调用check6必须传递给它一个string 类型的参数
check6会将此参数传递给check_size

string s = "hello";
bool b1 = check6(s);

auto wc = find_if(words.begin(), words.end(), [sz](const string &a))
替换为
auto wc = find_if(words.begin(), words.edn(), bind(check_size, _1, sz));

使用placeholders名字
using std::placeholders::_1;
using namespace std::placeholders;

bind参数
假设f是一个可调用对象
auto g = bind(f, a, b, _2, c, _1);
生成一个新的可调用对象 它有两个参数 分别用占位符_2 和 _1表示
这个新的可调用对象将它自己的参数作为第三个和第五个参数传递给f
第一、二、四个参数分别被绑定到给定的值a,b,c上
这个bind调用会将
g(_1, _2)  映射为  f(a, b, _2, c, _1);

用bind重排参数顺序
sort(words.begin(), words.end(), isShorter);    单词长度由短至长排序
sort(words.begin(), words.end(), bind(isShorter, _2, _1));  单词长度由长至短排序

绑定引用参数
for_each(words.begin(), words.end(), [&os, c](const string &s){ os << s << c});
可以编写函数代替
ostream &print(ostream &os, const string &s, char c)
{
	return os << s << c;
}

但是不能直接用bind来代替对os的捕获
for_each(words.begin(), words.end(), bind(print, os, _1, ' ')); 错误 不能拷贝os
如果我们希望传递给bind一个对象而又不拷贝它 就必须使用标准库ref函数

for_each(words.begin(), words.end(), bind(print, ref(os), _1, ' '));
cref 生成一个保存const引用的类

再探迭代器 额外几种迭代器
插入迭代器
流迭代器
反向迭代器
移动迭代器

插入器是一个迭代器适配器 接受一个容器 生成一个迭代器
back_inserter  创建一个使用push_back的迭代器
front_inserter 创建一个使用push_front的迭代器
inserter       创建一个使用insert的迭代器

如果it是由inserter生成的迭代器 则
*it = val;
效果等于
it = c.insert(it, val);  it指向新加入的元素
++it; 递增it使它指向原来的元素

list<int> lst = {1,2,3,4};
list<int> lst2, lst3;
copy(lst.cbegin(), lst.cend(), front_inserter(lst2));  lst2 = 4 3 2 1 调用 push_front
copy(lst.cbegin(), lst.cend(), inserter(lst3));        lst3 = 1 2 3 4 调用 insert


iostream迭代器    流迭代器不支持递减操作
istream_iterator  读取输入流
ostream_iterator  向一个输出流写数据

istream_iterator<int> int_it(cin);  从cin读取int
istream_iterator<int> int_eof;      尾后迭代器
ifstream in("afile");
istream_iterator<string> str_it(in);从afile读取字符串

istream_iterator<int> in_iter(cin);  cin读取int
istream_iterator<int> eof;           istream尾后迭代器
while (in_iter != eof)               当有数据可供读取时
{
	后置递增运算读取流 返回迭代器的旧值
	解引用迭代器 获取从流读取的前一个值
	vec.push_back(*in_iter++);
}
重写
istream_iterator<int> in_iter(cin), eof; 从cin读取int
vector<int> vec(in_iter, eof);   从迭代器范围构造vec

使用算法操作流迭代器
istream_iterator<int> in(cin), eof;
cout << accumulate(in, eof, 0) << endl;
此时调用会计算出从标准输入的值的和

istream_iterator允许使用惰性求值

必须将ostream_iterator绑定到一个指定的流 不允许空的或者表示尾后位置的ostream_iterator
ostream_iterator<int> out_iter(cout, " ");
for (auto e : vec)
{
	*out_iter++ = e;  赋值语句实际上将元素写到cout
}
cout << endl;

重写
for (auto e : vec)
	out_iter = e;
cout << endl;
推荐第一种写法
可以调用copy来打印vec中的元素

copy(vec.begin(), vec.end(), out_iter);
cout << endl;

使用流迭代器处理 类类型
istream_iterator<Sales_item> item_iter(cin), eof;
ostream_iterator<Sales_item> out_iter(cout, "\n");

//第一笔交易记录存在sum中 并读取下一条
Sales_item sum = *item_iter++;
while (item_iter != eof)
{
	if (item_iter->isbn() == sum.isbn())   //如果当前交易记录 有着相同的isbn号
	{
		sum += *item_iter++;  //将其加到sum上并读取下一条记录
	}
	else
	{
		out_iter = sum;  //输出sum当前值
		sum = *item_iter++;  //读取下一条记录
	}
}
out_iter = sum;  //记得打印最后一组记录的和


反向迭代器
对于反向迭代器而言 递增一个反向迭代器++it 会移动到前一个元素
递减一个反向迭代器 会移动到下一个元素
除了 forward_list 其他容器都支持反向迭代器

vector<int> vec = {0,1,2,3,4,5,6,7,8,9};
for (auto r_iter = vec.crbegin(); r_iter != vec.crend(); ++r_iter)
{
	cout << *r_iter << endl;  9,8,7, .... 0
}
sort(vec.begin(), vec.end());  按照正常序排序vec
sort(vec.rbegin(), vec.rend()); 按照逆序排序 将最小的元素放到vec的末尾

名为line的string 保存着一个逗号分隔的单词列表
打印第一个单词
auto comma = find(line.cbegin(), line.cend(), ',');
cout << string(line.cbegin(), comma) << endl;

查找最后一个单词
auto rcomma = find(line.crbegin(), line.crend(), ',');

但是
cout << string(line.crbegin(), rcomma) << endl;
如果输入是 FIRST,MIDDLE,LAST 
这句话会打印TSAL
cout << string(rcomma.base(), line.cend()) << endl; //base 返回对应的普通迭代器

[line.crbegin(), rcomma)
[rcomma.base(), line.cend())
指向相同的元素范围
rcomma 和 rcomma.base() 必须生成相邻位置而不是相同位置
当我们从一个普通迭代器初始化一个反向迭代器
或者给一个反向迭代器赋值时 结果迭代器与原迭代器指向的并不是相同的元素


泛型算法结构
算法所要求的迭代器操作可以分为5迭代器类别
输入迭代器       只读 不写 单遍扫描 只能递增
输出迭代器       只写 不读 单遍扫描 只能递增
前向迭代器       可读写 多遍扫描 只能递增
双向迭代器       可读写 多遍扫描 可递增递减
随机访问迭代器   可读写 多遍扫描 支持全部迭代器运算

算法除了参数规范 还遵守一套命名和重载规范

一些算法使用重载形式传递一个谓词
unique(beg, end);
unique(beg, end, comp);
_if 版本的算法
find(beg, end, val);   查找输入范围中 val第一次出现的位置
find_if(beg, end, pred);  查找第一个令pred为真的元素
区分拷贝元素的版本和不拷贝的版本
reverse(beg, end);     反转输入范围中元素的顺序
reverse_copy(beg, end, dest);  将元素按逆序拷贝到dest

remove_if(v1.begin(), v1.end(), [](int i){ return i % 2; }); 从v1中删除奇数元素
remove_copy_if(v1.begin(), v1.end(), back_inserter(v2), [](int i){ return i % 2; }); 将偶数元素从v1拷贝到v2 v1不变


特定容器算法
list 和 forward_list成员函数版本的算法 应该优先调用
lst.merge(lst2);   lst2 并入 lst  lst 和 list2 都必须是有序的
lst.merge(lst2, comp);  元素将从lst2中删除 在合并之后 lst2变为空

lst.remove(val);     调用erase删掉与给定值==的或令一元谓词为真的每个元素
lst.remove_if(pred);

lst.reverse()  反转lst中元素的顺序

lst.sort() 使用<或给定比较操作排序元素
lst.sort(comp)

lst.unique()    调用erase删除同一个值的连续拷贝 第一个版本使用==
lst.unique(pred) 第二个版本使用给定的二元谓词

splice成员 链表数据结构特有
lst.splice(args) 
flst.splice_after(args)
(p, lst2)     lst2所有元素移动到lst中p之前的位置 或是 flst中p之后的位置 将元素从lst2中删除 lst2类型与lst或flst相同 且不是同一链表
(p, lst2, p2) p2指向lst2中位置有效的迭代器 p2指向的元素移动到lst中 或将p2之后的元素移动到flst中 lst2可以是与lst 或flst相同的链表
(p, lst2, b, e) b e表示lst2中的合法范围 将给定范围中的元素从lst2移动到lst或flst lst2与lst(flst) 可以是相同的链表 但p不能指向范围中

链表版本算法会改变底层的容器



####### 第十一章 关联容器 #######

按关键字有序保存元素
map    关联数组 保存关键字-值对
set    关键字即值 即只能保存关键字的容器
multimap  关键字可重复出现的map
multiset

无序集合
unordered_map  用哈希函数组织的map
unordered_set
unordered_multimap  哈希组织的map 关键字可重复出现
unordered_multiset

单词计数
map<string, size_t> word_count;
string word;
while (cin >> word)
{
	++word_count[word];
}
for (const auto &w : word_count)
{
	cout << w.first << "occurs" << w.second
	<< ((w.second > 1) ? "times" : "time") << endl;
}

使用set 
std::map<string, size_t> word_count;
set<string> exclude = { "The", "But", "And", "Or", "An", "A",
						"the", "but", "and", "or", "an", "a" };
string word;
while (cin >> word)
{
	if (exclude.find(word) == exclude.end())  //find调用返回一个迭代器
		++word_count[word];  //获取并递增word的计数器 只统计不在exclude中的单词
}

关联容器不支持构造函数或者插入操作

 //空容器
map<string, size_t> word_count;    
//列表初始化
set<string> exclude = { "The", "But", "And", "Or", "An", "A",
						"the", "but", "and", "or", "an", "a" };  
map<string, string> authors = { {"joyce", "james"}, {"austen", "jane"}, {"Dickens", "charles"} };


vector<int> ivec;
for (vector<int>::size_type i = 0; i != 10; ++i)
{
	ivec.push_back(i);
	ivec.push_back(i);
}
set<int> iset(ivec.cbegin(), ivec.cend());  //iset 包含来自ivec的不重复的元素 miset包含所有20个元素
multiset<int> miset(ivec.cbegin(), ivec.cend());
cout << ivec.size() << endl;    20
cout << iset.size() << endl;    10
cout << miset.size() << endl;   20

对于有序容器 关键字类型必须定义元素比较的方法
在集合类型中 关键字类型就是元素类型
在映射类型中 关键字类型就是元素第一部分的类型 word_count的关键字类型是string

提供自己定义的操作来代替关键字上的<运算符 所提供的操作必须在关键字类型上定义一个严格弱序

使用关键字类型的比较函数
我们不能直接定义一个 Sales_data 的 multiset 因为 Sales_data 没有<运算符
为了使用我们自己的操作 在定义 multiset时我们必须提供两种类型 关键字类型 以及 比较操作类型
bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs)
{
	return lhs.isbn() < rhs.isbn();
}
multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
//bookstore中多条记录可以有相同的ISBN
//bookstore中的元素以ISBN的顺序进行排列
用 compareIsbn来初始化bookstore对象 这表示我们向bookstore 添加元素时 通过调用compareIsbn来为这些元素排序


pair类型
pair<string, string> anon;
pair<string, size_t> word_count;
pair<string, vector<int>> line;
pair的默认构造函数对数据成员进行默认初始化

我们也可以为每个成员提供初始化器
pair<string, string> author{"james", "joyce"};

pair的数据成员是public的
map的元素是pair

创建pair对象的函数
一个函数需要返回一个pair 在新标准下我们可以对返回值列表初始化
pair<string, int> process(vector<string> &v)
{
	if (!v.empty())
	{
		return { v.back(), v.back().size() }; // 列表初始化 返回一个由v中最后一个string及其大小构成的pair
	}
	else
		return pair<string, int>();  //隐式构造返回值
}

if (!v.empty())
{
	return make_pair(v.back(), v.back().size());
}
我们还可以使用make_pair来生成pair对象 pair的两个类型来自make_pair的参数

关联容器操作 
set<string>::value_type v1;          v1 --- string
set<string>::key_type v2;            v2 --- string
map<string, int>::value_type v3;     v3 --- pair<const string, int>
map<string, int>::key_type v4;       v4 --- string
map<string, int>::mapped_type v5;    v5 --- int   关键字关联的类型


关联容器迭代器
当解引用一个关联容器迭代器时 我们会得到一个类型为容器的 value_type 的值的引用
auto map_it = word_count.begin();
*map_it 是指向一个pair<const string, size_t> 对象的引用 
cout << map_it->first;        打印关键字
cout << " " << map_it->second;  打印此元素的值

map_it->first = "new key";    错误 关键字是const的
++map_it->second;   正确 我们可以通过迭代器改变元素

我们可以改变pair的值 但是不能改变关键字成员的值

set的迭代器是const的
set<int> iset = {0,1,2,3,4,5,6,7,8,9};
set<int>::iterator set_it = iset.begin();
if (set_it != iset.end())
{
	*set_it = 42;              错误 set中的关键字是只读的
	cout << *set_it << endl;   正确 可以读关键字
}

遍历关联容器
auto map_it = word_count.cbegin();
while (map_it != word_count.cend())
{
	cout << map_it->first << "occurs"
	<< map_it->second << "times" << endl;
	++map_it;
}

我们通常不对关联容器使用泛型算法 因为关键字是const
关联容器可用于只读取元素的算法 关联容器定义find成员 比 调用泛型find快的多

添加元素
关联容器的insert成员
vector<int> ivec = {2,4,6,8,2,4,6,8};  ivec 8 个元素
set<int> set2;                        空集合
set2.insert(ivec.cbegin(), ivec.cend()); set2有四个元素
set2.inset({1,3,5,7,1,3,5,7});           set2现在有8个元素

向map添加元素
word_count.insert({word, 1});
word_count.insert(make_pair(word, 1));
word_count.insert(pair<string, size_t>(word, 1));
word_count.insert(map<string, size_t>::value_type(word, 1));

检测insert的返回值
insert 和 emplace 返回的值依赖于容器类型和参数
对于不包含重复关键字的容器 添加单一元素的insert 和 emplace 版本返回一个pair
pair的first成员 是一个迭代器
pair的second成员 是一个bool值 指出元素是否插入成功还是已经存在容器中
map<string, size_t> word_count;
string word;
while (cin >> word)
{
	auto ret = word_count.insert({word, 1});
	if (!ret.second)   word已在word_count中
	{
		++ret.first->second;   //递增计数器
	}
}

ret.first 是 pair的第一个成员 是一个map迭代器
ret.first->second map中元素的值部分

向 multiset 和 multimap 添加元素
multimap<string, string> authors;
authors.insert({"barth, john", "sot-weed factor"});
authors.insert({"barth, john", "Lost in the funhouse"});

删除元素 
传递给 erase一个迭代器 或者 一个迭代器对 来删除一个元素 或者 一个元素范围
关联容器提供额外的erase操作 接受一个key_value参数 返回实际删除元素的数量
if (word_count.erase(removal_word))  //删除一个关键字 返回删除的元素数量
	cout << "ok:" << removal_word << "removed\n";
else
	cout << "oops: " << removal_word << " not found!\n";

auto cnt = authors.erase("barth, john"); cnt = 2

map的下标操作
set不支持下标  不能对multimap 和 unordered_multimap 进行下标操作

map<string, size_t> word_count;
word_count["anna"] = 1;  //插入一个关键字为anna的元素 关联值进行值初始化 然后将1赋予它

我们只能对非const的map进行下标操作

对一个map使用下标操作 其行为与数组或vector上的下标操作很不相同
使用一个不在容器中的关键字作为下标 会添加一个具有此关键字的元素到map中

c.at(k) 若k不在c中 抛出 out_of_range异常

通常解引用一个迭代器所返回的类型 与 下标运算符返回的类型一样
但是对于 map 对map进行下标操作时  会获得一个mapped_type对象
但当解引用一个map迭代器时 会得到一个value_type对象

cout << word_count["anna"];  1
++word_count["anna"];
cout << word_count["anna"];  2

有时只是想知道一个元素是否在map中 并不想添加元素就不能使用下标运算符

访问元素

set<int> iset = {0,1,2,3,4,5,6,7,8,9};
iset.find(1);   返回一个迭代器 指向key == 1 的元素
iset.find(11);  返回一个迭代器 其值等于iset.end()
iset.count(1);  返回1
iset.count(11); 返回0

需要计数 用 count 不需要用 find

c.lower_bound(k)  返回一个迭代器 指向第一个关键字不小于k的元素  第一个具有给定关键字的元素
c.upper_bound(k)  返回一个迭代器 指向第一个关键字大于k的元素  最后一个匹配给定关键字的元素之后的位置
c.equal_range(k)  返回一个迭代器pair 关键字k的元素范围 若k不存在 pair两个成员均等于c.end()

对map使用find代替下标操作
if (word_count.find("foobar") == word_count.end())
{
	cout << "foobar is not in the map" << endl;
}

在multiset 和 multimap中查找元素
如果在multiset 和 multimap中有多个元素具有相同的关键字 则这些元素在容器中会相邻存储

方法一
string search_item("alain de botton");
auto entries = authors.count(search_item);   有多少著作
auto iter = authors.find(search_item);       指向第一个关键字为此作者的元素
while (entries)
{
	cout << iter->second << endl;
	++iter;
	--entries;
}

一种不同的 面向迭代器的解决方法
方法二
lower_bound返回的迭代器可能指向一个具有给定关键字的元素 但也可能不指向
如果关键字不在容器中 lower_bound会返回关键字第一个安全插入点 不影响容器中元素顺序的插入位置

for (auto beg = authors.lower_bound(search_item), end = authors.upper_bound(search_item);
		beg != end; ++beg)
{
	cout << beg->second << endl;
}
beg 和 end 表示对应此作者的元素范围

如果容器中存在元素 beg定位到第一个search_item匹配的元素
如果容器中不存在元素 beg将指向第一个关键字大于search_item的元素 有可能是尾后迭代器
end指向最后一个匹配指定关键字的元素之后的元素
这两个操作并不报告是否存在 重要的是他们的返回值可作为一个迭代器范围

如果没有元素和关键字匹配 lower_bound 和 upper_bound 会返回相等的迭代器 都指向给定元素的插入点
能保证容器中元素的插入位置

方法三
equal_range函数 返回迭代器pair
for (auto pos = authors.equal_range(search_item); pos.first != pos.second; ++pos.first)
{
	cout << pos.first->second << endl;   pos保存迭代器对 表示与关键字匹配的元素范围 authors是一个map
}

单词转换的map
where r u
y dont u send me a pic 
k thk 18r
------->
where are you
why dont you send me a picture
okay? thanks later 

void word_tansform(ifstream &map_file, ifstream &input)
{
	auto trans_map = buildMap(map_file);  //保存转换规则
	string text;
	while (getline(input, text))     //保存输入中的每一行 读入一行
	{
		istringstream stream(text);   //读取每个单词
		string word;
		bool firstword = true;
		while (stream >> word)
		{
			if (firstword)      //控制是否打印空格
				firstword = false;
			else
			{
				cout << " ";
				cout << transform(word, trans_map); //transform 返回它的第一个参数或其转换后的形式 打印输出
			}
		}
		cout << endl;
	}
}
建立转换映射
map<string, string> buildMap(ifstream &map_file)
{
	map<string, string> trans_map;
	string key;      //要转化的单词
	string value;    //替换后的内容
	while (map_file >> key && getline(map_file, value))
	{
		if (value.size() > 1)   //检测是否有转化规则
			trans_map[key] = value.substr(1); //跳过前导空格
		else
			throw runtime_error("no rule for " + key);
	}
	return trans_map;
}
trans_map中将保存着用来转换输入文本的规则  由map_file 生成

生成转换文本
const string & transform(const string &s, const map<string, string> &m)
{
	auto map_it = m.find(s);
	if (map_it != m.cend())
		return map_it->second;
	else
		return s;
}


无序容器
这些容器不是使用比较运算符来组织的 而是使用一个哈希函数 和关键字 == 运算符
在关键字类型的元素没有明显的序关系的情况下 无序容器是非常有用的

unordered_map<string, size_t> word_count;  统计出现次数 但单词不会按照字典序排列
string word;
while (cin >> word)
	++word_count[word];
for (const auto &w : word_count)
{
	count << w.first << "occurs" << w.second
	  << ((w.second > 1) ? "times:" : "time") << endl;
}

无序容器在存储上组织为一组桶
容器将具有特定哈希值的所有元素都保存在相同的桶中
如果容器允许重复关键字 所有具有相同关键字的元素也会在同一个桶中
因此无序容器的性能依赖于哈希函数的质量 和桶的数量和大小

桶接口
c.bucket_count()
c.max_bucket_count()
c.bucket_size(n)   第n个桶中有多少元素
c.bucket(k)        关键字k在哪个桶中
桶迭代
local_iterator       可以访问桶中元素的迭代器类型
const_local_iterator
c.begin(n), c.end(n)  桶n的首元素迭代器 和尾后迭代器
c.cbegin(n), c.cend(n)
哈希策略
c.load_factor()   每个桶的平均元素数量 返回float值
c.max_load_factor()
c.rehash(n)  重组存储
c.reserve(n) 重组存储

我们不能直接定义关键字类型为 自定义类类型 的无序容器
为了使用 Sales_data用作关键字 我们需要提供函数来代替==运算符和哈希值计算函数
size_t hasher(const, Sales_data &sd)
{
	return hash<string>()(sd.isbn());	
}
bool eqOp (const Sales_data &lhs, const Sales_data &rhs)
{
	return lhs.isbn() == rhs.isbn();
}

using SD_multiset = unordered_multiset<Sales_data, decltype(hasher)*, decltype(eqOp)*>;
参数是桶大小， 哈希函数指针 和相等性判断运算符指针
SD_multiset bookstore(42, hasher, eqOp);

unordered_set<Foo, decltype(FooHash)*> fooSet(10, FooHash);
使用 FooHash 生成哈希值 Foo必须有==运算符


####### 第十二章 动态内存 #######

除了静态内存和栈内存 每个程序还拥有一个内存池 这部分内存被称为自由空间 或 堆

智能指针
智能指针的行为类似常规指针 重要的区别是它负责自动释放所指向的对象
shared_ptr  允许多个指针指向同一个对象
unique_ptr  独占所指向的对象
weak_ptr 伴随类 弱引用指向shared_ptr 所管理的对象

shared_ptr<string> p1;    指向string
shared_ptr<list<int>> p2; 指向int的list

if (p1 && p1->empty())  如果p1不为空 检测它是否指向一个空string
{
	*p1 = "hi";  如果p1指向一个空string 解引用p1 将一个新值赋予string
}

make_shared函数
shared_ptr<int> p3 = make_shared<int>(42);     指向一个值为42的int的shared_ptr
shared_ptr<string> p4 = make_shared<string>(10, '9');  指向一个值为9999999999的string
shared_ptr<int> p5 = make_shared<int>();       指向一个值初始化的int 即 值为0

调用make_shared<string> 时传递的参数必须与string的某个构造函数相匹配

auto p6 = make_shared<vector<string>>(); p6指向一个动态分配的空vector<string>

shared_ptr的拷贝和赋值
auto p = make_shared<int>(42);  p指向的对象只有p一个引用者
auto q(p);   p和q指向相同的对象 此对象有两个引用者

我们可以认为每个shared_ptr都有一个关联的计数器 称为引用计数
一旦一个shared_ptr的计数器为0 它会自动释放自己所管理的对象
auto r = make_shared<int>(42); r 指向 的int只有一个引用值
r = q; 给r赋值 令他指向另一个地址
       递增q指向的对象的引用计数
       递减r原来指向的对象的引用计数
       r原来指向的对象已经没有引用者 会自动释放

shared_ptr自动销毁所管理的对象 还会自动释放相关联的内存
shared_ptr的析构函数会递减它所指对象的引用计数 如果引用计数变为0 析构函数会销毁对象 并释放它的内存

shared_ptr<Foo> factory(T arg)
{
	//处理arg
	//shared_ptr负责释放内存
	return make_shared<Foo>(arg);
}
factory 返回一个 shared_ptr 指向一个动态分配的对象

void use_factory(T arg)
{
	shared_ptr<Foo> p = factory(arg);
	//使用p
}//p离开了作用域 它指向的内存会被自动释放


void use_factory(T arg)
{
	shared_ptr<Foo> p = factory(arg);
	//使用p
	return p; //当我们返回p时 引用计数进行了递增操作
}//p离开了作用域 但它指向的内存不会被自动释放
p被销毁后 指向的内存还有其他的使用者
如果将share_ptr存放在一个容器中 而后不再需要全部元素 而只使用其中一部分 要记住用erase删除不再需要的那些元素


使用了动态生存期的资源 的类
程序使用动态内存出于以下三种原因之一
1.程序不知道自己需要使用多少对象
2.程序不知道所需对象的准确类型
3.程序需要在多个对象间共享数据

容器类是出于第一种原因使用动态内存的例子
它使用动态内存是为了让多个对象能共享相同的底层数据

vector<string> v1;
{
	std::vector<string> v2 = {"a", "an", "the"};
	v1 = v2;
} //v2被销毁 其中的元素也被销毁
//v1有三个元素 是原来v2中元素的拷贝

假设我们希望定义一个名为Blob的类
当我们拷贝一个Blob时 原Blob对象及其拷贝引用相同的底层元素
Blob<string> b1;
{
	Blob<string> b2 = {"a", "an", "the"};
	b1 = b2;  //b1 b2共享相同的元素
}//b2被销毁 但是b2中的元素不能销毁
//b1指向最初由b2创建的元素

定义动态内存的一个常见原因是允许多个对象共享相同的状态

定义StrBlob类
为了保证vector中的元素继续存在 我们将vector保存在动态内存中
为了实现数据共享 我们为每个 StrBlob 设置一个 shared_ptr来管理动态分配的vector
class StrBlob
{
public:
	typedef std::vector<std::string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<std::string> il);
	size_type size() const { return data->size(); }
	bool empty() const { return data->empty(); }
	void push_back(const std::string &t) { data->push_back(t); }
	void pop_back();
	std::string& front();
	std::string& back();
private:
	std::shared_ptr<std::vector<std::string>> data;
	void check(size_type i, const std::string &msg) const;
};

StrBlob构造函数
StrBlob::StrBlob(): data(make_shared<vector<string>>()) { }
StrBlob::StrBlob(initializer_list<string> il): data(make_shared<vector<string>>(il)) { }

元素访问成员函数
void StrBlob::check(size_type i, const string &msg) const
{
	if (i >= data->size())
	{
		throw out_of_range(msg);
	}
}

string& StrBlob::front()
{
	check(0, "front on empty StrBlob");
	return data->front();
}
string& StrBlob::back()
{
	check(0, "back on empty StrBlob");
	return data->back();
}
void StrBlob::pop_back()
{
	check(0, "pop_back on empty StrBlob");
	data->pop_back();
}

将一个 shared_ptr赋值给另一个 shared_ptr 会递增右边shared_ptr的引用计数
递减左边 shared_ptr的引用计数

直接管理内存
int *p1 = new int;  //p1指向一个动态分配 未初始化的无名对象

string *ps = new string;  //初始化为空string
int *pi = new int;  //pi指向一个未初始化的int

int *pi = new int(1024);   //pi指向的对象的值为1024
string *ps = new string(10, '9');  //*ps为 "9999999999"

vector<int> *pv = new vector<int>{0,1,2,3,4,5,6,7,8,9};

string *ps1 = new string;   //默认初始化为空string
string *ps = new string(); //值初始化为空string
string *pi1 = new int;    //默认初始化 *pi1的值未定义
string *pi2 = new int();  //值初始化为0 *pi2为0

对于定义了自己的构造函数的类类型来说 要求值初始化是没有意义的
不管采用什么方式 对象都会通过默认构造函数来初始化
但对于内置类型 两种形式就差别大了
值初始化的内置类型对象有着良好定义的值
而默认初始化的对象的值则是未定义的

如果我们提供一个括号包围的初始化器 就可以使用auto
auto p1 = new auto(obj);     //p指向一个与obj类型相同的对象
							 //该对象用obj进行初始化
auto p2 = new auto{a, b, c}; //错误 括号中只能有单个初始化器

p1类型是指针

动态分配的const对象
const int *pci = new const int(1024);  //分配并初始化一个 const int
const string *pcs = new const string;  //分配并默认初始化一个 const的空 string
一个动态分配的const对象必须进行初始化

int *p1 = new int;   //如果分配失败 new抛出 std::bad_alloc
int *p2 = new (nothrow) int; //如果分配失败 new返回一个空指针

释放动态内存
为了防止内存耗尽
delete表达式接受一个指针 指向我们要释放的对象
delete p; p必须指向一个动态分配的对象或者空指针

int i, *pi1 = &i, *pi2 = nullptr;
double *pd = new double(33), *pd2 = pd;
delete i;   错误 i不是一个指针
delete pi1; 未定义 pi1 指向一个局部变量
delete pd;  正确
delete pd2; 未定义 pd2 指向的内存已经被释放了
delete pi2; 正确 释放一个空指针总是没有错的

const int *pci = new const int(1024);
delete pci; 正确 释放一个const对象

动态内存的管理非常容易出错
1.忘记delete内存
2.使用已经释放掉的对象 通过在释放内存后将指针置为空 有时可以检测出这种错误
3.同一快内存释放两次

delete之后重置指针值 这只是提供了有限的保护
在delete之后 将nullptr赋予指针
int *p(new int(42));  p指向动态内存
auto q = p;           p和q指向相同的内存
delete p;             p和q均变为无效
p = nullptr;          指出p不能再绑定到任何对象

重置p对q没有任何作用 在实际系统中 查找指向相同内存的所有指针异常困难

shared_ptr 和 new 结合使用
shared_ptr<double> p1;   shared_ptr可以指向一个double
shared_ptr<int> p2(new int(42));  p2指向一个值为42的int

shared_ptr<int> p1 = new int(1024);  错误 必须使用直接初始化形式
shared_ptr<int> p2(new int(1024));   正确 使用了直接初始化形式

由于我们不能进行内置指针到智能指针间的隐式转换 因此这条初始化语句是错误的

shared_ptr<int> clone(int p)
{
	return new int(p); 错误 隐式转换为shared_ptr<int>
}

shared_ptr<int> clone(int p)
{
	return shared_ptr<int>(new int(p));
	正确 显式的使用 int*创建shared_ptr<int>
}

不要混合使用普通指针和智能指针....

void process(shared_ptr<int> ptr)  值传递方式 实参拷贝到ptr中
{
	//使用ptr
}//ptr被销毁
shared_ptr<int> p(new int(42));   引用计数为1
process(p);     拷贝p会增加他的引用计数 在process中值为2
int i = *p;     引用计数值为1


int *x(new int(1024));   危险 x是一个普通指针 不是一个智能指针
process(x);  不能由 int*转换成 shared_ptr<int>
process(shared_ptr<int>(x));  合法的 但内存会被释放   临时的shared_ptr 表达式结束 这个对象就被销毁了
int j = *x; 未定义 x是一个空悬指针

....也不要使用get初始化另一个智能指针或为 智能指针赋值

智能指针定义了一个名为get的函数 返回一个内置指针
是为了需要向不能使用智能指针的代码传递一个内置指针 使用get返回的指针代码不能delete此指针

shared_ptr<int> p(new int(42));  引用计数 1
int *q = p.get();   正确 使用q要注意 不能让它管理的指针被释放
{  //新程序块
	//未定义 两个独立的shared_ptr指向相同的内存
	shared_ptr<int>(q);
}//程序块结束 q被销毁 它指向的内存被释放
int foo = *p;   //未定义p指向的内存已经被释放

p q指向相同的内存 由于他们相互独立创建的 因此各自的引用计数都是1
永远不要用get初始化另一个智能指针或者 为另一个智能指针赋值


其他shared_ptr操作
p = new int(1024);   //错误 不能将一个指针赋予shared_ptr
p.reset(new int(1024));  //正确 p指向了一个新对象

if (!p.unique())
{
	p.reset(new string(*p)); 我们不是唯一用户 分配新的拷贝
}
*p += newVal; 现在我们知道自己是唯一的用户 可以改变对象的值

智能指针和异常
一个简单的确保资源被释放的方法是使用智能指针
void f()
{
	shared_ptr<int> sp(new int(42));
	//分配一个对象 这段代码抛出一个异常 且在f中未被捕获
}//在函数结束时 shared_ptr自动释放内存

void f()
{
	int *ip - new int(42); //动态分配一个新对象
	//这段代码抛出一个异常 且在f中未被捕获 
	delete ip;    //在推出之前释放内存
}
如果在 new 和 delete之间 发生了异常 且在f中未被捕获 则内存永远不会被释放了

智能指针和哑类

struct destination;
struct connection;
connection connect(destination*);
void disconnect(connection);
void f(destination &d)
{
	connection c = connect(&d);
	//使用连接 使用完之后要记得关闭它
	//如果我们在f推出前 忘记调用disconnect 就无法关闭c
}
如果connection 有一个析构函数 就可以在f结束时 由析构函数自动关闭连接
但是connection没有析构函数 
使用 shared_ptr 来保证connection被正确关闭 是一种有效的方法

使用自己的释放动作
删除器函数能够完成对 shared_ptr 中保存的指针进行释放的操作
void end_connection(connection *p) { disconnect(*p); }
void f(destination &d)
{
	connection c = connect(&d);
	shared_ptr<connection> p(&c, end_connection);
	//使用连接
	//当f退出时 connect会被正确关闭
}
当p被销毁 他不会调用对自己保存的指针执行delete 而是调用 end_connection
接下来 end_connection 会调用 disconnect

正确使用智能指针：
1.不使用相同的内置指针值初始化 多个智能指针
2.不 delete get()返回的指针
3.不使用 get() 初始化或reset另一个智能指针
4.如果使用get() 返回的指针 最后一个对应的智能指针销毁后 你的指针变得无效
5.如果你使用智能指针管理的资源不是new 分配的内存 记得传递给它一个删除器


unique_ptr
一个unique_ptr 拥有它所指向的对象 某个时刻只能有一个unique_ptr指向一个给定对象
与 shared_ptr 不同 没有类似 make_shared 的标准库函数 返回一个 unique_ptr
unique_ptr<double> p1; //可以指向一个double的unique_ptr
unique_ptr<int> p2(new int(42));  //p2 指向一个值为42的int
初始化unique_ptr 必须采用直接初始化方式
由于unique_ptr 拥有指向它的对象 因此unique_ptr不支持普通拷贝或赋值操作

unique_ptr<string> p1(new string("Stegosaurus"));
unique_ptr<string> p2(p1);   错误 unique_ptr不支持 拷贝
unique_ptr<string> p3;
p3 = p2;                     错误 unique_ptr不支持 赋值

unique_ptr<string> p2(p1.release());  将所有权从p1转移给p2 release将p1 置为空
unique_ptr<string> p3(new string("Trex")); 
p2.reset(p3.release()); 将所有权从p3转移给p2 reset 释放了p2原本指向的内存 将p3对指针的所有权转移给p2 并将p3置空
release成员返回 unique_ptr 当前保护的指针并将其置为空 p2 被初始化为 p1 原来保存的指针 p1被置为空

p2.release(); 错误 p2不会释放内存 而且我们丢失了指针
auto p = p2.release();  正确 但我们必须记得 delete(p)

传递unique_ptr 参数 和 返回 unique_ptr
不能拷贝unique_ptr 的规则有一个例外 我们可以拷贝和赋值一个将要被销毁的 unique_ptr
最常见的是从函数返回一个unique_ptr
unique_ptr<int> clone(int p)
{
	return unique_ptr<int>(new int(p));
}
还可以返回一个局部对象的拷贝
unique_ptr<int> clone(int p)
{
	unique_ptr<int> ret(new int(p));
	//...
	return ret;
}

向unique传递删除器
unique_ptr<objT, delT> p(new objT, fcn);

void f(destination &d)
{
	connection c = connect(&d);
	unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);
}


weak_ptr
weak_ptr是一种不控制对象生存期的智能指针 它指向一个shared_ptr管理的对象
将 weak_ptr 绑定到 shared_ptr 不会改变shared_ptr 的计数
一旦最后一个指向对象shared_ptr被销毁 对象就会被释放 即使有weak_ptr 指向对象 对象也会被释放

创建 weak_ptr 要用一个 shared_ptr 初始化它
auto p = make_shared<int>(42);
weak_ptr<int> wp(p); wp弱共享p p的引用计数未改变

对象可能不存在  我们不能使用weak_ptr 直接访问对象 必须调用lock
if (shared_ptr<int> np = wp.lock())  //如果np 不为空则条件成立
{
	//在if中 np 与 p 共享对象
}

class StrBlobPtr
{
public:
	StrBlobPtr(): curr(0) { }
	StrBlobPtr(StrBlob &a, size_t sz = 0): wptr(a.data), curr(sz) { }
	std::string& deref() const;
	StrBlobPtr& incr();  //前缀递增
private:
	std::shared_ptr<std::vector<std::string>> check(std::size_t, const std::string&) const;
	std::weak_ptr<std::vector<std::string>> wptr; //保存weak_ptr 意味着底层vector可能被销毁
	std::size_t curr; //在数组中的当前位置
};

我们不能将 StrBlobPtr绑定到一个 const StrBlob 对象 这个限制是由于构造函数接受一个非const StrBlob对象的引用导致的

std::shared_ptr<std::vector<std::string>> StrBlobPtr::check(std::size_t i, const std::string &msg) const
{
	auto ret = wptr.lock();  //vector 还存在么
	if (!ret) 
	{
		throw std::runtime_error("unbound StrBlobPtr");
	}
	if (i >= ret->size())
	{
		throw std::out_of_range(msg);
	}
	return ret;  否则返回指向vector的 shared_ptr
}

指针操作 定义deref 和 incr 用来 解引用 和 递增 StrBlobPtr
std::string& StrBlobPtr::deref() const
{
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}
如果check 成功 p就是一个 shared_ptr 指向 StrBlobPtr 所指向的 vector
表达式 (*p)[curr] 解引用 shared_ptr 来获得vector 使用下标提取返回curr位置上的元素

StrBlobPtr& StrBlobPtr::incr()
{
	check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}
返回递增后的对象的引用
为了访问data成员 我们的指针类必须声明为 StrBlob 的friend
class StrBlobPtr;
class StrBlob
{
	friend class StrBlobPtr;
	//返回指向首元素和尾后元素的 StrBlobPtr
	StrBlobPtr begin() { return StrBlobPtr(*this); }
	StrBlobPtr end()
	{
		auto ret = StrBlobPtr(*this, data->size());
		return ret;
	}
};


动态数组
使用容器的类可以使用默认版本的拷贝 赋值 和析构 操作
分配动态数组的类 则必须定义自己版本的操作 在拷贝 复制 以及销毁对象是管理所关联的内存
int *pia = new int[get_size()]; pia 指向第一个 int

typedef int arrT[42];  arrT表示42个int的数组类型
int *p = new arrT;     分配一个42个int的数组 p指向第一个int

new分配一个int数组 并返回指向第一个int的指针 即使这段代码中没有方括号

分配一个数组会得到一个元素类型的指针
由于分配的内存并不是一个数组类型 因此不能对动态数组调用 begin 和 end 也不能用范围for处理动态数组中的元素
我们所说的动态数组 不是 数组类型

int *pia = new int[10];        10个未初始化的int
int *pia2 = new int[10]();     10个值初始化为0的int
int *psa = new string[10];     10个空string
int *psa2 = new string[10]();  10个空string

int *pia3 = new int[10]{0,1,2,3,4,5,6,7,8,9};
string *pia3 = new string[10]{"a", "an", "the", string(3,'x')};  前四个给定的初始化器初始化 剩余的进行值初始化

动态分配一个空数组是合法的
可以用任意表达式来确定要分配的对象的数目
size_t n = get_size();
int *p = new int[n];
for (int *q = p; q != p + n; ++q)
{
	//处理数组
}
但是当n等于0 调用 new[0] 是合法的
char arr[0];    //错误 不能定义长度为0的数组
char *cp = new char[0];   //正确 但cp不能解引用

释放动态数组
delete p;    p必须指向一个动态分配的对象或为空
delete [] pa;  pa必须指向一个动态分配的数组或为空  数组逆序销毁

智能指针和动态数组
用一个unique_ptr管理动态数组
unique_ptr<int[]> up(new int[10]);  up指向一个包含了10个未初始化int的数组
up.release(); 自动用delete[] 销毁其指针

for (size_t i = 0; i != 10; ++i)
{
	up[i] = i;  //为每个元素赋予新值
}

shared_ptr 不能直接管理动态数组 如果希望使用 shared_ptr 管理一个动态数组
必须提供自己定义的删除器
shared_ptr<int> sp(new int[10], [](int *p){ delete [] p; });
sp.reset(); //使用我们提供的lambda 释放数组 它使用 delete[]

for (size_t i = 0; i != 10; i++)
{
	*(sp.get() + i) = i;
}
shared_ptr未定义下标运算符 而且智能指针类型不支持指针算术运算 因此为了访问数组中的元素
必须使用get获取一个内置指针 然后用它来访问数组元素


allocator类

一般将 内存分配和对象构造组合在一起 可能会导致不必要的浪费
string *const p = new string[n];  构造n个空的 string
string s;
string *q = p;                    q指向第一个 string
while (cin >> s && q != p + n)
	*q++ = s;                    赋予*q一个新值
const size_t size = q - p;     计算我们读取了多少个string
//使用数组
delete [] p;  p 指向一个数组 记得用delete[] 来释放

每个使用到的元素都被赋值两次 第一次在默认初始化时 第二次是在赋值时

allocator<string> alloc;             可以分配string的allocator对象
auto const p = alloc.allocate(n);    分配n个 未初始化的string
这个allocate 调用为n个string 分配了内存

allocate 分配未构造的内存

auto q = p;                  
alloc.construct(q++);        *q为空字符串
alloc.construct(q++, 10, 'c'); *q 为cccccccccc
alloc.construct(q++, "hi");    *q 为hi!

construct成员函数 接受一个指针 和 零个或多个额外参数 在给定位置构造一个元素

cout << *p << endl;  正确 使用string 的输出运算符
cout << *q << endl;  灾难 q指向未构造的内存

while (q != p)
	alloc.destroy(--q); 释放我们真正构造的string

我们只对真正构造了的元素进行 destroy 操作

释放内存 alloc.deallocate(p, n);
拷贝和填充未初始化内存的算法
auto p = alloc.allocate(vi.size() * 2);  分配比vi中元素所占空间大一倍的动态内存
auto q = uninitialized_copy(vi.begin(), vi.end(), p); 通过拷贝vi中的元素来构造p开始的元素
uninitialized_fill_n(q, vi.size(), 42);  将剩余元素初始化为42

uninitialized_copy 返回

使用标准库：文本查询程序

void runQueries(ifstream &infile)
{
	TextQuery tq(infile); //保存文件到vector中 并建立单词到所在行号的map
	//与用户交互 提示用户输入要查询的单词 完成查询并打印结果
	while (true)
	{
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(cin >> s) || s == "q") break;  
		print(cout, tq.query(s)) << endl;
	}
}

class QueryResult;  为了定义query的返回类型
class TextQuery
{
public:
	using line_no = std::vector<std::string>::size_type;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;
private:
	std::shared_ptr<std::vector<std::string>> file;  输入文件
	std::map<std::string, std::shared_ptr<std::set<line_no>>> wm;  每个单词到它所在行号的集合的映射
}

TextQuery构造函数
TextQuery::TextQuery(ifstream &is): file(new vector<string>)
{
	string text;
	while (getline(is, text))        文件的每一行
	{
		file->push_back(text);       保存此行文本
		int n = file->size() - 1;    当前行号   每插入一行文字 vector大小加1
 		istringstream line(text);    将行文本 分解为单词
		string word;
		while (line >> word)         对行中每个单词
		{                            如果单词不再wm中 以之为下标在wm中添加一项
			auto &lines = wm[word];  lines是一个 shared_ptr
			if (!lines)              第一次遇到这个单词
			{                                   
				lines.reset(new set<line_no>);  分配一个新的set lines指向它 line_no 是 size_type
			}                                   
			lines->insert(n);        将此行号插入set中
		}
	}
}
如果给定单词在同一行中出现多次 对insert的调用什么都不会做
最终 wm 为  <全体文本中的每个单词, 每个单词行号的set>

QueryResult类
class QueryResult
{
	friend std::ostream& print(std::ostream&, const QueryResult&);
public:
	QueryResult(std::string s, 
				std::shared_ptr<std::set<line_no>> p,
				std::shared_ptr<std::vector<std::string>> f):
		sought(s), lines(p), file(f) { }
private:
	std::string sought;   查询单词
	std::shared_ptr<std::set<line_no>> lines;  出现的行号
	std::shared_ptr<std::vector<std::string>> file;  输入文件
};

QueryResult TextQuery::query(const string &sought) const
{
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	auto loc = wm.find(sought);
	if (loc == wm.end())
	{
		return QueryResult(sought, nodata, file);  未找到
	}
	else
	{
		return QueryResult(sought, loc->second, file);
	}
}

ostream &print(ostream& os, const QueryResult &qr)
{
	os << qr.sought << "occurs" << qr.lines->size() << " "
	 << make_plural(qr.lines->size(), "time", "s") << endl;
	 for (auto num : *qr.lines)
	 {
	 	os << "\t (line" << num + 1 << ") "
	 	<< *(qr.file->begin() + num) << endl;
	 }
	 return os;
}



####### 第十三章 拷贝控制 #######


拷贝和移动构造函数 定义了当用同类型的另一个对象初始化 本对象时 做什么
拷贝和移动运算符 定义了将一个对象赋予同类型的另一个对象时 做什么
析构函数 定义了当此类型对象销毁时 做什么
我们称这些操作为 拷贝控制操作

拷贝构造函数 
class Foo
{
public:
	Foo();              默认构造函数
	Foo(const Foo&);    拷贝构造函数
};
一个构造函数的第一个参数是 自身类类型的引用 且任何额外参数都有默认值 则此构造函数是 拷贝构造函数


合成的拷贝构造函数会将其参数成员逐个拷贝到正在创建的对象中
类类型成员 使用其拷贝构造函数来拷贝
内置类型成员 直接拷贝

class Sales_data
{
public:
	Sales_data(const Sales_data&);  与合成的拷贝构造函数 等价的拷贝构造函数声明
private:
	std::string bookNo;
	int units_sold = 0;
	double revenue = 0.0;
};
Sales_data::Sales_data(const Sales_data *orig): 
			bookNo(orig.bookNo), units_sold(orig.units_sold), revenue(orig.revenue) { }


string dots(10, '.');     直接初始化
string s(dots);           直接初始化
string s2 = dots;         拷贝初始化
string null_book = "9-999-99999-9";    拷贝初始化
string nines = string(100, '9');       拷贝初始化

拷贝初始化依赖 拷贝构造函数或移动构造函数 完成的
拷贝构造函数不仅在用= 定义变量时发生
1.将一个对象作为实参传递给一个 非引用类型的形参
2.从一个返回类型为 非引用类型的函数 返回一个对象
3.用花括号列表初始化 一个数组中的元素 或 一个聚合类中的成员
insert或push成员 容器会对其元素进行拷贝初始化
emplace成员 创建的元素都是直接初始化

拷贝构造函数被用来初始化非引用类类型参数 这一特性解释了为什么拷贝构造函数自己的参数必须是引用类型
如果其参数不是引用类型 则调用永远不会成功


拷贝赋值运算符
Sales_data trans, accum;
trans = accum;  使用Sales_data的拷贝赋值运算符

与拷贝构造函数一样 如果类未定义自己的拷贝赋值运算符 编译器会为它合成一个

赋值运算符通常 应该返回一个指向其左侧运算对象的引用
class Foo
{
public:
	Foo& operator = (const Foo&);
};

Sales_data& Sales_data::operator=(const Sales_data &rhs)
{
	bookNo = rhs.bookNo;
	units_sold = rhs.units_sold;
	revenue = rhs.revenue;
	return *this;
}

析构函数
构造函数初始化对象的非static数据成员
析构函数释放对象使用的资源 并销毁对象的非static数据成员

隐式销毁一个内置指针类型的成员不会delete它所指向的对象
智能指针是类类型 所以具有析构函数

什么时候会调用析构函数 
1.变量在离开其作用域时被销毁
2.当一个对象被销毁时 其成员被销毁
3.容器 无论是标准库容器还是数组 被销毁时 其元素被销毁
4.对于动态分配的对象 当对指向它的指针应用delete运算符时 被销毁
5.对于临时对象 当创建它的完整表达式结束时被销毁

{ 新作用域
Sales_data *p = new Sales_data;        p是一个内置指针
auto p2 = make_shared<Sales_data>();   p2是一个 shared_ptr
Sales_data item(*p);                   拷贝构造函数将*p拷贝到item中
vector<Sales_data> vec;                局部对象
vec.push_back(*p2);                    拷贝p2指向对象
delete p;                              对p指向的对象执行析构函数
}
退出局部作用域 对 item p2 和vec调用析构函数
销毁p2会递减其引用计数 如果引用计数为0 对象被释放 
销毁vec会销毁它的元素

当指向一个对象的引用或指针 离开作用域时 析构函数不会执行

当一个类未定义自己的析构函数时 编译器会为它定义一个合成析构函数
class Sales_data
{
	~Sales_data() { }
}
等于 Sales_data的合成构造函数
析构函数体 自身并不直接销毁成员
成员是在析构函数体之后隐含的 析构阶段中被销毁的
析构函数体 是作为成员销毁步骤之外 的另一部分而进行的

三五法则
三个基本操作控制类的拷贝操作
拷贝构造函数
拷贝赋值运算符
析构函数
新标准下：一个类还可以定义 一个移动构造函数 移动赋值运算符

需要析构函数的类 也需要拷贝和赋值的操作
如果一个类需要一个析构函数 我们几乎可以肯定 它需要一个拷贝构造函数 和一个 拷贝赋值运算符

class HasPtr
{
public:
	HasPtr(const std::string &s = std::string()): ps(new std::string(s)), i(0) { }
	~HasPtr() { delete ps; }
	错误 HasPtr需要一个拷贝构造函数 和一个 拷贝赋值运算符
	//其他成员的定义 如前
};
这个版本的类使用了合成拷贝构造函数 和 合成拷贝赋值运算符
这些函数简单拷贝指针成员 这意味着 多个HasPtr 对象可以指向相同的内存

HasPtr f(HasPtr hp)    HasPtr 是传值参数 所以被拷贝
{
	HasPtr ret = hp;
	return ret;       ret和hp被销毁
}
f 返回时 hp 和ret 都被销毁 在两个对象上都会调用HasPtr的析构函数
这两个对象包含相同的指针值 代码会导致delete两次
HasPtr p("some values");
f(p);      当f结束时 p.ps 指向的内存被释放
HasPtr q(p);  现在p和q都指向无效内存！！

需要拷贝操作的类 也需要赋值操作 反之亦然

使用 =default
我们可以通过将拷贝控制成员定义为=default 来显式的要求编译器 生成合成的版本
class Sales_data
{
public:
	Sales_data() = default;
	Sales_data(const Sales_data&) = default;
	Sales_data& operator=(const Sales_data &);
	~Sales_data() = default;
};
Sales_data& Sales_data::operator=(const Sales_data&) = default;

类内用=default 修饰成员的声明 合成的函数将隐式的 声明为内联
如果我们不希望合成的成员是内联 类外=default

阻止拷贝
新标准下 我们可以通过将拷贝构造函数 和 拷贝赋值运算符 定义为 删除的函数 来阻止拷贝
struct NoCopy
{
	NoCopy() = default;     使用合成的默认构造函数
	NoCopy(const NoCopy&) = delete;   阻止拷贝  
	NoCopy &operator=(const NoCopy&) = delete;  阻止赋值
	~NoCopy() = default;    使用合成的析构函数
};

=delete 出现在函数第一次声明的时候 
我们可以对任何函数指定 =delete
我们只能对编译器可能合成的默认构造函数 或 拷贝控制成员 使用 =default

我们不能删除析构函数

struct NoDtor
{
	NoDtor() = default;  使用合成默认构造函数
	~NoDtor() = delete;  我们不能销毁NoDtor 类型的对象
};

NoDtor nd;    错误 NoDtor 的析构函数是删除的
NoDtor *p = new NoDtor();   正确 但我们不能 delete p
delete p;   错误 NoDtor 的析构函数是删除的

对于析构函数已删除的类型 不能定义该类型的变量或者释放 该类型动态分配对象的指针

如果一个类有数据成员 不能默认构造 拷贝 复制和销毁 则对应的成员函数将被定义为删除的
本质上 当不可能 拷贝 赋值 或者销毁类的成员时 类的合成拷贝控制成员就被定义为删除的

private拷贝控制
新标准之前 类是通过将其拷贝构造函数 和拷贝赋值运算符 声明为private 的来阻止拷贝

class PrivateCopy
{
	PrivateCopy(const PrivateCopy&);    无访问说明符 默认private
	PrivateCopy &operator=(const PrivateCopy&);
public:
	PrivateCopy() = default;  使用合成的默认构造函数
	~PrivateCopy();           用户可以定义此类型的对象 但无法拷贝他们
};

建议使用前者 delete的方式

拷贝控制和资源管理
类的行为像一个值   意味着它应该也有自己的状态 当我们拷贝一个像值 的对象时 副本和原对象时完全独立的
类的行为像一个指针 共享状态 当我们拷贝一个这种类 的对象时 副本和原对象使用相同的底层数据

行为像值的类

类值版本的 HasPtr 需要
1.定义一个拷贝构造函数 完成string的拷贝 而不是拷贝指针
2.定义一个析构函数 释放string 
3.定义一个拷贝赋值运算符 来释放对象当前的string 并从右侧运算对象拷贝string
class HasPtr
{
public:
	HasPtr(const std::string &s = std::string()): ps(new std::string(s)), i(0) { }
	HasPtr(const HasPtr &p): ps(new std::string(*p.ps)), i(p.i) { }
	HasPtr& operator = (const HasPtr &);
	~hasptr() { delete ps; }
private:
	std::string *ps;
	int i;
};
类值 拷贝赋值运算符
HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
	auto newp = new string(*rhs.ps);  拷贝底层string
	delete ps;     释放旧内存
	ps = newp;     右侧运算对象拷贝数据到本对象
	i = rhs.i;
	return *this;  返回本对象
}

编写一个赋值运算符时 一个好的模式是 先将右侧 运算对象拷贝到一个 局部临时对象中
当拷贝完成后 销毁左侧运算对象的现有成员就是安全的了 一旦左侧运算对象的资源被销毁
就剩下数据从临时对象 拷贝到左侧运算对象的成员中了


定义行为像指针的类
我们需要为其定义拷贝构造函数和拷贝赋值运算符 来拷贝指针成员本身 而不是它指向的string

自定义引用计数
解决计数器同步的问题 是将计数器保存在动态内存中
当创建一个对象时 我们也分配一个新的计数器
当拷贝或赋值对象时 我们拷贝指向计数器的指针 使用这种方式 副本和原对象都会指向相同的计数器
class HasPtr
{
public:
	HasPtr(const std::string &s = std::string()): ps(new std::string(s)), i(0), use(new std::size_t(1)) { }
	HasPtr(const HasPtr &p): ps(p.ps), i(p.i), use(p.use) { ++*use; }
	HasPtr& operator=(const HasPtr&);
	~HasPtr();
private:
	std::string *ps;
	int i;
	std::size_t *use;  用来记录有多少个对象共享 *ps成员
};


HasPtr::~HasPtr()
{
	if (--*use == 0)
	{
		delete ps;   释放string内存
		delete use;  释放计数器内存
	}
}

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
	++*rhs.use;
	if (--*use == 0)
	{
		delete ps;
		delete use;
	}
	ps = rhs.ps;
	i = rhs.i;
	use = rhs.use;
	return *this;	
}


交换操作
HasPtr temp = v1;
v1 = v2;
v2 = temp;
理论上这些内存的分配都是不必要的 我们更希望 swap交换指针而不是 分配string的新副本
string *temp = v1.ps;
v1.ps = v2.ps;
v2.ps = temp;

编写我们自己的swap
class HasPtr
{
	friend void swap(HasPtr&, HasPtr&);
};
inline void swap(HasPtr &lhs, HasPtr &rhs)
{
	using std::swap;
	swap(lhs.ps, rhs.ps);
	swap(lhs.i, rhs.i);
}

在赋值运算符中 使用swap
拷贝并交换技术
HasPtr& HasPtr::operator=(HasPtr rhs)
{
	swap(*this, rhs); 交换左侧运算对象和局部变量rhs的内容 rhs现在指向本对象曾经用过的内存
	return *this;  rhs被销毁 从而delete了 rhs中的指针
}
rhs是按值传递 意味着 HasPtr的拷贝构造函数将右侧运算对象中的string拷贝到 rhs中
swap之后 *this中的指针成员将指向新分配的string 即右侧运算对象中string的一个副本

使用拷贝和交换的赋值运算符自动就是异常安全的 且能正确处理 自赋值


拷贝控制示例
拷贝赋值运算符 通常执行拷贝构造函数和析构函数中也要做的工作  这种情况下 公共的工作应该放在private的工具函数中完成

Message类
class Message
{
	friend class Floder;
public:
	explicit Message(const std::string &str = ""): contents(str) { }  folders 被隐式初始化为空集合
	Message(const Message&);                      拷贝构造函数
	Message& operator=(const Message&);           拷贝赋值运算符
	~Message();                                   析构函数

	void save(Floder&);           从给定的Folder 集合中添加删除 本Message
	void remove(Floder&);
private:
	std::string contents;                     实际的消息内容
	std::set<Folder*> folders;                包含本Message的Floder

	void add_to_Folders(const Message&);
	void remove_from_Folders();
};

void Message::save(Floder &f)
{
	folders.insert(&f);
	f.addMsg(this);
}

void Message::remove(Floder &f)
{
	folders.erase(&f);
	f.remMsg(this);
}

void Message::add_to_Folders(const Message &m)
{
	for (auto f : m.folders)  对每个包含m的folders 向该Folder添加一个指向本Message的指针
		f->addMsg(this);  
}

Message::Message(const Message &m): contents(m.contents), floders(m.folders)
{
	add_to_Folders(m);  将本消息添加到m的Floder中
}

void Message::remove_from_Folders()
{
	for (auto f : folders)    对folders中每个指针 从该floder中删除Message
	{
		f->remMsg(this);
	}
}

Message::~Message()
{
	remove_from_Folders();
}

Message& Message::operator=(const Message &rhs)
{
	remove_from_Folders();
	contents = rhs.contents;
	folders = rhs.folders;
	add_to_Folders(rhs);   将本message添加到那些Folder中
	return *this;
}
add_to_Folders不能在 remove_from_Folders 之前 
因为如果左侧和右侧是相同的Message 则会将此Message从它所在的所有Folder中删除

void swap(Message &lhs, Message &rhs)
{
	using std::swap;
	for (auto f : lhs.folders)
		f->remMsg(&lhs);
	for (auto f : rhs.foldres)
		f->remMsg(&rhs);

	swap(lhs.floders, rhs.floders);
	swap(lhs.contents, rhs.contents);

	for (auto f : lhs.folders)
		f->addMsg(&lhs);
	for (auto f : rhs.folders)
		f->addMsg(&rhs);
}

Floder类
class Floder
{
	friend void swap(Folder &, Folder &);
	friend class Message;
public:
	Floder() = default;
	Floder(const Floder&);
	Floder& operator=(const Floder&);
	~Floder();
	void add_to_Message(const Folder&);
	void remove_from_Message();
private:
	std::set<Message*> msgs;

	void addMsg(Message &m) { msgs.insert(m); }
	void remMsg(Message &m) { msgs.erase(m); }
};

void swap(Folder &lhs, Folder &rhs)
{
	using std::swap;
	lhs.remove_from_Message();
	rhs.remove_from_Message();

	swap(lhs.msgs, rhs.msgs);

	lhs.add_to_Message(lhs);
	rhs.add_to_Message(rhs);
}

Folder::Folder(const Folder &f): msg(f.msg)
{
	add_to_Message(f);
}

Folder::~Floder()
{
	remove_from_Message();
}

void Folder::add_to_Message(const Folder &f)
{
	for (auto m : f.msgs)
		m->save(this);
}
void Folder::remove_from_Message()
{
	for (auto m : msgs)
		m->remove(this);
}

Folder &Folder::operator=(const Folder &rhs)
{
	remove_from_Message();
	msg = rhs.msg();
	add_to_Message();
	return *this;
}

动态内存管理类
StrVec类  实现标准库vector类的简化版本
class StrVec
{
public:
	StrVec(): elements(nullptr), first_free(nullptr), cap(nullptr) { }
	StrVec(const StrVec&);
	StrVec &operator=(StrVec &);
	~StrVec();

	void push_back(const std::string&);
	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }
	std::string *begin() const { return elements; }
	std::string &end() const { return first_free; }
private:
	static std::allocator<std::string> alloc;
	void chk_n_alloc(){ if (size() == capacity()) reallocate(); }
	工具函数 被 构造 赋值运算符 和析构函数所使用
	std::pair<std::string*, std::string*> alloc_n_copy (const std::string*, const std::string*);
	void free();                  销毁元素并释放内存
	void reallocate();            获得更多内存并拷贝已有元素
	std::string *elements;        数组首元素的指针
	std::string *first_free;      数组元素第一个空闲元素的指针
	std::string *cap;             指向数组尾后位置的指针
};

使用construct
void StrVec::push_back(const string& s)
{
	chk_n_alloc();
	alloc.construct(first_free++, s);
}
allocator分配内存时 内存是未构造的

pair<string*, string*>StrVec::alloc_n_copy(const string *b, const string *e)
{
	auto data = alloc.allocate(e - b);  分配 e-b个空间 data 指向开始的位置
	return {data, uninitialized_copy(b, e, data)};
}
返回的pair 的 first成员 指向分配的内存的开始位置 
uninitialized_copy 的返回值	是一个指针 指向最后一个构造元素之后的位置

free成员
void StrVec::free()
{
	if (elements)
	{
		for (auto p = first_free; p != elements;  )
		{
			alloc.destroy(--p);
		}
		alloc.deallocate(elements, cap - elements);
	}
}

拷贝控制成员
StrVec::StrVec(const StrVec &s)
{
	auto newdata = alloc_n_copy(s.begin(), s.end());
	elements = newdata.first;
	first_free = newdata.second;
}

StrVec::~StrVec() { free(); }

StrVec &StrVec::operator=(const StrVec &rhs)
{
	auto data = alloc_n_copy(rhs.copy(), rhs.end());
	free();
	elements = data.first;
	first_free = cap = data.second;
	return *this;
}

在重新分配内存的过程中 移动而不是拷贝元素
编写 reallocate 应该 为一个新的更大的string数组分配内存
在内存空间的前一部分构造对象 保存现有元素
销毁原内存空间中的元素 并释放内存 

拷贝这些string中的数据是多余的 如果能在重新分配内存空间时 避免分配和释放string的额外开销
StrVec的性能会好很多

移动构造函数和 std::move
移动构造函数 通常是将资源从给定对象 移动 而不是拷贝到正在创建的对象

reallocate 成员
void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto newdata = alloc.allocate(newcapacity);   分配新内存
	auto dest = newdata;       指向新数组中下一个空闲位置
	auto elem = elements;      指向旧数组中下一个元素
	for (size_t i = 0; i != size(); ++i)
	{
		alloc.construct(dest++, std::move(*elem++));
	}
	free();    释放旧内存空间
	更新数据结构  执行新元素
	elements = newdata;
	first_free = dest;
	cap = elements + newcapacity;
}

对象移动
移动而非拷贝对象会大幅提升性能
标准库容器 string 和 shared_ptr 类即支持移动也支持拷贝
IO类 和 unique_ptr 类可以移动 但是不能拷贝

为了支持移动 引入 右值引用 就是必须绑定到右值的引用 通过 && 获得 
右值引用 重要性质 只能绑定到一个将要销毁的对象
常规引用 称之为左值引用 不能绑定到 要求转移的表达式 字面常量 或返回右值的表达式

int i = 42;             
int &r = i;             正确r引用i     
int &&rr = i;           错误 不能将一个右值引用 绑定到一个 左值上
int &r2 = i * 42;       错误 i * 42 是一个右值
const int &r3 = i * 42; 正确 我们可以将一个const的引用绑定到一个右值上
int &&rr2 = i * 42;     正确 将rr2绑定到乘法结果上

左值持久 右值短暂
变量是左值 因此我们不能将一个右值直接绑定到一个变量上 即使这个变量是右值引用类型也不行
int &&rr1 = 42; 正确 字面常量是右值
int &&rr2 = rr1;错误 表达式rr1是左值

虽然不能将一个右值引用直接绑定到一个左值上 但我们可以显式地将一个左值 转换为对应的右值引用类型
int &&rr3 = std::move(rr1);

移动构造函数 和 移动赋值运算符
StrVec::StrVec(StrVec &&s) noexcept: elements(s.elements), first_free(s.first_free), cap(s.cap)
{ 成员初始化器接管s中的资源
	令s进入这样的状态  对其运行析构函数是安全的
	s.elements = s.first_free = s.cap = nullptr;
}
noexcept 通知标准库 我们的构造函数不抛出异常  在声明和定义中 都需要指定
移动构造函数 不分配任何新内存 它接管 StrVec 中的内存

StrVec &StrVec::operator=(StrVec &&rhs) noexcept
{
	if (this != &rhs)
	{
		free();
		elements = rhs.elements;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.elements = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
移后源对象必须可析构
编写移动操作时 必须确保移后源对象 进入一个可析构的状态

合成的移动构造函数 和移动赋值运算符
只有当一个类没有定义任何自己版本的拷贝控制成员 且类的每个非 static数据成员 都可以移动时
编译器才会为它合成移动构造函数 和移动赋值运算符

struct X
{
	int i;
	string s;
};

struct hasX
{
	X mem;
};

X x, x2 = std::move(x);        使用合成的移动构造函数     
hasX hx, hx2 = std::move(hx);  使用合成的移动构造函数

struct hasY
{
	hasY() = default;
	hasY(hasY&&) = default;
	Y mem;
};
hasY hy, hy2 = std::move(hy); 错误 移动构造函数是 删除的
假定y是一个类 它定义了自己的拷贝构造函数 但未定义自己的移动构造函数

定义了一个移动构造函数或移动赋值运算符的类 必须也定义自己的拷贝操作
否则 这些成员默认被定义为删除的


移动右值 拷贝左值。。。。

StrVec v1, v2;
v1 = v2;                    v2是左值 使用拷贝赋值 
StrVec getVec(istream &);   getVec 返回一个右值
v2 = getVec(cin);           getVec(cin) 是一个右值 使用移动赋值

。。。。但如果没有移动构造函数 右值也被拷贝

class Foo
{
public:
	Foo() = default;
	Foo(const Foo&);
};

Foo x;
Foo y(x);               拷贝构造函数 x是一个左值
Foo z(std::move(x));    拷贝构造函数 因为未定义移动的构造函数

在对z进行初始化时 我们调用了move(x) 它返回一个绑定到x的 Foo&&
Foo 的拷贝构造函数是可行的 因为我们可以将一个Foo&& 转换为一个 const Foo &
因此z的初始化 将使用Foo的拷贝构造函数

如果一个类有一个可用的拷贝构造函数 而没有移动构造函数 则其对象是通过
拷贝构造函数来移动的 拷贝赋值运算符 和 移动赋值运算符 的情况类似

拷贝并交换赋值运算和移动操作

class HasPtr
{
public:
	HasPtr(HasPtr &&p) noexcept: ps(p.ps), i(p.i) { p.ps = 0; }
	HasPtr& operator=(HasPtr rhs)  赋值运算符 即是移动赋值运算符 也是拷贝赋值运算符
	{
		swap(*this, rhs);
		return *this;
	}
}

假定hp 和 hp2 都是HasPtr 对象
hp = hp2;            hp2是一个左值 hp2通过拷贝函数来拷贝
hp = std::move(hp2);  移动构造函数移动hp2

第一个赋值中 右侧运算对象是一个左值 因此移动构造函数是不可行的 rhs将使用拷贝构造函数来初始化
拷贝构造函数将分配一个新的 string 并拷贝hp2 指向string 

在第二个赋值中 我们调用std::move 将一个右值引用绑定到hp2 上 在此情况下
拷贝构造函数和移动构造函数都是可行的 但是实参是一个右值引用 移动构造函数是精确匹配
移动构造函数从hp2拷贝指针 而不会分配任何内存

更新三/五法则
所有五个拷贝控制成员 应该看作一个整体
某些类必须定义了拷贝构造函数 拷贝赋值运算符 和析构函数才能正常工作
这些类通常拥有一个资源 而拷贝成员必须拷贝此资源
一般来说拷贝一个资源会导致一些额外开销 在这种拷贝并非必要的情况下
定义了移动构造函数和移动赋值运算符的类 就可以避免此问题

Message类的移动操作

从本来Message 移动Floder指针
void Message::move_Floders(Message *m)
{
	floders = std::move(m->folders);   使用set的移动赋值运算符
	for (auto f : floders)             对每个Folders 
	{
		f->remMsg(m);                  从Folders中删除旧 Message
		f->addMsg(this);               将本Message 添加 到 Folder中
	}
	m->folders.clear();         确保销毁m是无害的
}

Message& Message::operator=(Message &&rhs)
{
	if (this != &rhs)   检查自赋值情况
	{
		remove_from_Folders();
		contents = std::move(rhs.contents);  移动赋值运算符
		move_Floders(&rhs);    重置Folders指向本messge
	}
	return *this;
}

移动迭代器
移动迭代器的 解引用运算符 生成一个右值引用
我们通过调用标准库的 make_move_iterator 函数将一个普通迭代器转换为移动迭代器

void StrVec::reallocate()
{
	auto newcapacity = size() ? 2 * size() : 1;
	auto first = alloc.allocate(newcapacity);

	auto last = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), first);
	free();
	elements = first;
	first_free = last;
	cap = newcapacity + elements;
}
uninitialized_copy 对输入序列中的每个元素调用 construct 来将元素拷贝到目的位置
此算法使用迭代器的解引用运算符从输入序列中提取元素 由于我们传递给它的是移动迭代器
因此解引用运算符生成的是右值引用 这意味着construct将使用移动构造函数开构造元素

不要随意使用移动操作

右值成员和成员函数
区分移动和拷贝的重载函数 通常有一个版本接受一个 const T& 而另一个版本接受一个 T&&

class StrVec
{
pubilc:
	void push_back(const std::string&);   拷贝元素
	void push_back(std::string&&);        移动元素 
};

void StrVec::push_back(const string& s)
{
	chk_n_alloc();
	alloc.construct(first_free++, s);
}
void StrVec::push_back(string && s)
{
	chk_n_alloc();
	alloc.construct(first_free++, std::move(s));	
}

StrVec vec;
string s = "some string or another";
vec.push_back(s);       调用push_back(const string&)
vec.push_back("done");  调用push_back(string&&)

调用的差别在于实参是一个左值还是一个右值

右值和左值引用成员函数
我们指出this的左值 右值属性的方式与定义const成员函数相同
即在 参数列表后放置一个	引用限定符

class Foo
{
public:
	Foo &operator=(const Foo&) &;   只能向可修改的左值赋值
};
Foo &Foo::operator=(const Foo &rhs) &
{
	return *this;
}

引用限定符 可以是& 也可以是 && 分别指出this可以指向一个左值或右值

Foo &retFoo();  返回一个引用 retFoo是一个左值
Foo retVal();   返回一个值  retVal 是一个右值
Foo i, j;       i j都是左值
i = j;          正确
retFoo() = j;   正确
retVal() = j;   错误 retVal返回一个右值
i = retVal();   正确 我们可以将一个右值作为赋值操作的右侧运算对象

引用限定符必须跟在const 限定符之后

重载和引用函数

引用限定符也可以区分重载版本
class Foo
{
public:
	Foo sorted() &&;
	Foo sorted() const &;
private:
	vector<int> data;
};

本对象为右值 原址排序
Foo Foo::sorted() &&
{
	sort(data.begin(), data.end());
	return *this;
}

本对象是 const 或是一个左值 哪种情况都不能对他原址排序
Foo Foo::sorted() const &
{
	Foo ret(*this);                          拷贝一个副本
	sort(ret.data.begin(), ret.data.end());  排序副本
	return ret;                              返回副本
}

retVal.sorted()  retVal 是一个右值 调用Foo::sorted() &&
retFoo.sorted()  retFoo 是一个左值 调用Foo::sorted() const &

class Foo
{
public:
	Foo sorted() &&;
	Foo sorted() const;  错误 必须加上引用限定符

	using Comp = bool (const int&, const int&);
	Foo sorted(Comp*);       正确 不同的参数列表
	Foo sorted(Comp*) const; 正确 两个版本都没有引用限定符
}

如果一个成员函数有引用限定符  则具有相同参数列表的所有版本 都必须有引用限定符

###### 第十四章 重载运算与类型转换 ######

