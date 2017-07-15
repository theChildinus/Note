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
int *beg = begin(a); //ia首元素的指针
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
sizeof Sale_data::revenue; //另一种获取revenue大小的方式

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


