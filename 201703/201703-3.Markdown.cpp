/*
试题编号：	201703-3
试题名称：	Markdown
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　Markdown 是一种很流行的轻量级标记语言（lightweight markup language），广泛用于撰写带格式的文档。例如以下这段文本就是用 
    Markdown 的语法写成的：
    (见图p1.png, 在当前文件夹下)
　　这些用 Markdown 写成的文本，尽管本身是纯文本格式，然而读者可以很容易地看出它的文档结构。同时，还有很多工具可以自动把 
    Markdown 文本转换成 HTML 甚至 Word、PDF 等格式，取得更好的排版效果。例如上面这段文本通过转化得到的 HTML 代码如下所示：
    (见图p2.png, 在当前文件夹下)

　　本题要求由你来编写一个 Markdown 的转换工具，完成 Markdown 文本到 HTML 代码的转换工作。简化起见，本题定义的 Markdown 
    语法规则和转换规则描述如下：
　　●区块：区块是文档的顶级结构。本题的 Markdown 语法有 3 种区块格式。在输入中，相邻两个区块之间用一个或多个空行分隔。
     输出时删除所有分隔区块的空行。
　　○段落：一般情况下，连续多行输入构成一个段落。段落的转换规则是在段落的第一行行首插入 `<p>`，在最后一行行末插入 `</p>`。
　　○标题：每个标题区块只有一行，由若干个 `#` 开头，接着一个或多个空格，然后是标题内容，直到行末。`#` 的个数决定了标题的等级。
     转换时，`# Heading` 转换为 `<h1>Heading</h1>`，`## Heading` 转换为 `<h2>Heading</h2>`，以此类推。标题等级最深为 6。
　　○无序列表：无序列表由若干行组成，每行由 `*` 开头，接着一个或多个空格，然后是列表项目的文字，直到行末。转换时，在最开始插
     入一行 `<ul>`，最后插入一行 `</ul>`；对于每行，`* Item` 转换为 `<li>Item</li>`。本题中的无序列表只有一层，不会出现缩进的情况。
　　●行内：对于区块中的内容，有以下两种行内结构。
　　○强调：`_Text_` 转换为 `<em>Text</em>`。强调不会出现嵌套，每行中 `_` 的个数一定是偶数，且不会连续相邻。注意 `_Text_` 的
     前后不一定是空格字符。
　　○超级链接：`[Text](Link)` 转换为 `<a href="Link">Text</a>`。超级链接和强调可以相互嵌套，但每种格式不会超过一层。
输入格式
　　输入由若干行组成，表示一个用本题规定的 Markdown 语法撰写的文档。
输出格式
　　输出由若干行组成，表示输入的 Markdown 文档转换成产生的 HTML 代码。
样例输入
# Hello

Hello, world!

样例输出
<h1>Hello</h1>
<p>Hello, world!</p>

评测用例规模与约定
　　本题的测试点满足以下条件：
　　●本题每个测试点的输入数据所包含的行数都不超过100，每行字符的个数（包括行末换行符）都不超过100。
　　●除了换行符之外，所有字符都是 ASCII 码 32 至 126 的可打印字符。
　　●每行行首和行末都不会出现空格字符。
　　●输入数据除了 Markdown 语法所需，内容中不会出现 `#`、`*`、`_`、`[`、`]`、`(`、`)`、`<`、`>`、`&` 这些字符。
　　●所有测试点均符合题目所规定的 Markdown 语法，你的程序不需要考虑语法错误的情况。
　　每个测试点包含的语法规则如下表所示，其中“√”表示包含，“×”表示不包含。


解题思路:
这一题可以说是稍有规模的一题, 如果不用面向对象的思路编写代码, 恐怕很难通过本题, 即使通过了, 代码也会非常凌乱.

我这里写了11个类(为了保证可扩展性和降低耦合度), 每个类都只负责自己的一部分解析工作, 因此逻辑复杂度得到了分解,
每个类看起来都很简单, 用起来也非常简单方便.下面说一下总体的结构

题目中的Markdown文档主要分为区块和行内两种形式:
1. 行内中可能出现的形式有两种: 强调Em和链接Link, 但有一点很容易让人忽略, 就是纯文本. 纯文本可以看成是最基本的,
   最细粒度的文档元素, 不可再分割. 强调和链接中都包括纯文本,强调和链接之外的部分也是纯文本, 因此十分有必要定义
   一个表示纯文本的类Text.

2. 区块又分为三种:段落, 无序列表和标题; 其中标题只占一行, 而段落和无序列表都可以占多行. 

3. 除了区块和行内元素, 这里还 定义了类Line表示一行, 因为标题, 段落和无序列表都可以分解成一行或者多行, 因此定义一
   个表示一行元素的类也是十分有必要的. 而且, Line又正好由纯文本, 强调和链接组成, 因此, Line的地位其实是非常重要的, 
   它向上层的Heading, Paragraph 和UnorderedList屏蔽了Em, Link和Text的使用细节, 正确地将上层传递下来的一行
   markdown文档元素的解析任务分配到了Text,Em和Link中, 因此Line在这里起到了非常重要的作用.

为了提供统一的接头, 我让Block,InlineElement和Line类都继承自一个共同的基类MarkdownElement
最终,类的继承关系图如下:

MarkdownElement(抽象类,定义print()接口)
        |--------->Block(抽象类)
        |           |------->Heading
        |           |------->Paragraph
        |           |------->UnorderedList
        |--------->InlineElement(抽象类,定义init()接口)
        |               |--------->Em
        |               |--------->Link
        |               |--------->Text
        |--------->Line

最后, 还定义了最高层次的MarkdownDocument类表示一个markdown文档, 一个markdown文档由若干个区块Block组成.

代码实现中遇到的问题:
1. 由于Em和Link可以相互嵌套, 因此在Em的代码实现中要用到Link类, 而在Link的代码实现中又要用到Em类, 如果先编写
   Em类的代码,后编写Link类的代码, 在编译的时候会因为编译器不知道如何创建Link类的对象而报错, 反过来也会出现同样
   的问题, 这里我是这样解决的, 就是在Em类的定义中先不写出用到Link类的函数的代码实现, 而把该函数的代码实现放到
   Link类的定义之后.
2. 其实最复杂的是类结构的合理组织和设计,只有设计出恰当的类组织结构, 才能合理的分配任务, 使整个程序逻辑结构清晰明了.
3. 这个程序写得其实挺爽的, 第一次做一个算法题写了11个类, 还一次性提交通过, 非常开心.

扩展:
这个结构还可以非常方便地进行扩展, 如果行内中还会出现斜体或者粗体, 那么只需要再编写解析斜体或者粗体的类继承自InlineElement,
然后在Line类的init()函数中增加相应的判断语句即可(这里可以有更好的实现, 利用多态去掉if()判断). 如果区块还有其他的类型, 
只需要再编写相应的区块子类继承自Block类,完成相应的功能即可.


给出一组测试数据:
# Hello

*  item1
*    item2
* item3

Hello, _world_!

##   this is second level heading

this is a paragraph,
and it has to lines

[baidu](http://baidu.com)

this is a paragraph which contains _emphasize_ and [link](http:faxinwang.github.com)

this is a _emphasize which contains a [link](http://baidu(2).com)_

this is a [link which contains a _emphasize_](http://baidu.com)


输出结果:
<h1>Hello</h1>
<ul>
<li>item1</li>
<li>item2</li>
<li>item3</li>
</ul>
<p>Hello, <em>world</em>!</p>
<h2>this is second level heading</h2>
<p>this is a paragraph,
and it has to lines</p>
<p><a href="http://baidu.com)">baidu</a></p>
<p>this is a paragraph which contains <em>emphasize</em> and <a href="http:faxinwang.github.com)">link</a></p>
<p>this is a <em>emphasize which contains a <a href="http://baidu(2).com)">link</a></em></p>
<p>this is a <a href="http://baidu.com)">link which contains a <em>emphasize</em></a></p>

*/


#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<cstring>
using namespace std;


//markdown文档元素基类, 所有markdown文档元素都直接或间接继承自该类
class MarkdownElement
{
public:
    virtual void printHTML() = 0; //以HTML的格式打印出相应的元素
};

//行内元素的父类, 子类有Text, Em, Link
class InlineElement : public MarkdownElement
{
public:
    vector<InlineElement*> v;
    //从字符串数组s中解析出各子类所代表的元素
    //start表示进行解析的起始下标, 解释结束后,返回下一个元素的起始下标
    virtual int init(const char* s, int start) = 0;

    virtual ~InlineElement(){ for(int i=0,n=v.size(); i<n; ++i) delete v[i]; }
};

//表示行内的一段纯文本, 内容中不包含任何标签
class Text : public InlineElement
{
public:
    string txt;
    int init(const char *s, int i)
    {
        for(; s[i]; ++i)
        {
            if(s[i] == '\n') return i;
            if(s[i] == '[' ) return i;
            if(s[i] == ']' ) return i;
            if(s[i] == '(' ) return i;
            if(s[i] == ')' ) return i;
            if(s[i] == '_' ) return i;
            txt += s[i];
        }
        return i;
    }

    void printHTML(){ cout<<txt; }
};


//强调标签, 内容中除了纯文本外, 还可能有Link
class Em : public InlineElement
{
public:
    int init(const char* s,int i);
    
    void printHTML()
    {
        cout<<"<em>";
        for(int i=0,n=v.size(); i<n; ++i) v[i]->printHTML();
        cout<<"</em>";
    }
};

//表示一个超链接, 连接文字中可能包含强调标签
class Link : public InlineElement
{
public:
    string url;
    int init(const char* s, int i)
    {
        if(s[i] == '[') ++i;
        while(1)
        {
            if(s[i] == '_')
            {
                //这里创建了强调类的对象, 虽然编译器还没有看到强调类的全部代码, 但已经能够知道它所有的成员变量和成员函数,也就知道
                Em *em = new Em(); 
                i = em->init(s, i);
                v.push_back(em);
            }
            if(s[i] == ']') ++i;
            if(s[i] == '(' )
            {
                int brace = 1;
                for(++i; brace; ++i) //这里考虑了url中还包含括号的情况, 但去掉这个逻辑之后, 依然可以通过, 说明测试用例中没有出现这种情况
                {
                    if( s[i] == '(' ){  ++brace; url+='('; }
                    else if( s[i] == ')' ){  --brace;; url += ')'; }
                    else url += s[i];
                }
                return i; //此时i指向最外层的反括号')'之后
            }
            Text *txt = new Text();
            i = txt->init(s, i);
            v.push_back(txt);
        }
    }

    void printHTML()
    {
        cout<<"<a href=\""<<url<<"\">";
        for(int i=0,n=v.size(); i<n; ++i) v[i]->printHTML();
        cout<<"</a>";
    }
};

/*由于强调标签的init()函数中可能需要创建Link对象, 因此编译器在编译该函数之前,必须要清楚
  Link类的具体实现, 因此把强调标签的init()函数实现拿到了Link类的代码实现的后面
*/
int Em::init(const char* s, int i)
{
    if(s[i] == '_') ++i;
    while(1)
    {
        if(s[i] == '[')
        {
            Link *link = new Link();
            i = link->init(s, i+1);
            v.push_back(link);
        }
        if(s[i] == '_') return i+1; //跳过右边的下划线_
        Text *txt = new Text();
        i = txt->init(s, i);
        v.push_back(txt);
    }
    return i+1;
}

//表示文档中的一行文字元素
class Line : public  MarkdownElement
{
public:
    vector<InlineElement*> v;

    Line(const char* s)
    {
        for(int i=0; s[i]; )
        {
            if(s[i] == '_') //解析强调
            {
                Em *em = new Em();
                i = em->init(s, i);
                v.push_back(em);
            }
            if( s[i] == '[') //解析链接
            {
                Link* link = new Link();
                i = link->init(s, i);
                v.push_back(link);
            }
            if( s[i] ) //解析普通文本
            {
                Text *txt = new Text();
                i = txt->init(s, i);
                v.push_back(txt);
            }
        }
    }

    void printHTML(){ for(int i=0,n=v.size(); i<n; ++i) v[i]->printHTML(); }

    ~Line(){ for(int i=0,n=v.size(); i<n; ++i) delete v[i]; }
};

//表示文档中的各种区块, 不同的区块由相应子类去实现解析工作
class Block : public MarkdownElement
{
public:
    vector<Line*> v;

    virtual ~Block(){ for(int i=0,n=v.size(); i<n; ++i) delete v[i]; }
};

//标题区块
class Heading : public Block
{
public:
    int lv;
    Heading(const string& md)
    {
        int i=0;
        while(md[i] == '#') ++i;
        lv = i;
        while(md[i] == ' ') ++i;        
        v.push_back( new Line(md.c_str() + i) );
    }
    
    void printHTML()
    {
        printf("<h%d>", lv);
        v[0]->printHTML();
        printf("</h%d>\n",lv);
    }
};

//段落区块
class Paragraph : public Block
{
public:
    void addLine(const string & s) { v.push_back(new Line(s.c_str())); }
    void printHTML()
    {
        int n = v.size();
        printf("<p>");
        for(int i=0; i<n-1; ++i)
        {
             v[i]->printHTML();
             printf("\n");
        }
        v[n-1]->printHTML();
        printf("</p>\n");
    }
};

//无序列表区块
class UnorderedList : public Block
{
public:
    void addItem(const string& s)
    {
        int i=1;
        while(s[i] == ' ') ++i;
        v.push_back( new Line(s.c_str() + i) );
    }

    void printHTML()
    {
        printf("<ul>\n");
        for(int i=0,n=v.size(); i<n; ++i)
        {
            printf("<li>");
            v[i]->printHTML();
            printf("</li>\n");
        }
        printf("</ul>\n");
    }
};

//表示一个markdown文档
class MarkdownDocument
{
public:
    vector<Block*> v;
    void addBlock(Block* b){ v.push_back(b); }
    void printHTML() { for(int i=0,n=v.size(); i<n; ++i) v[i]->printHTML();}
    
    ~MarkdownDocument(){ for(int i=0,n=v.size(); i<n; ++i) delete v[i];  }
};


int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    string line;
    MarkdownDocument doc;
    while(getline(cin, line))
    {
        if(line == "") continue;
        if(line[0] == '#') doc.addBlock(new Heading(line));
        else if(line[0] == '*')
        {
            UnorderedList* ul = new UnorderedList();
            ul->addItem(line);
            while(getline(cin, line) && line != "") ul->addItem(line);
            doc.addBlock(ul);
        }
        else
        {
            Paragraph * p = new Paragraph();
            p->addLine(line);
            while(getline(cin, line) && line != "") p->addLine(line);
            doc.addBlock(p);
        }
    }

    doc.printHTML();

    return 0;
}