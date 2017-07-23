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





