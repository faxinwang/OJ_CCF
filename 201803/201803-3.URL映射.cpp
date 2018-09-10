/*
试题编号：	201803-3
试题名称：	URL映射
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　URL 映射是诸如 Django、Ruby on Rails 等网页框架 (web frameworks) 的一个重要组件。对于从浏览器发来的 HTTP 请求，
    URL 映射模块会解析请求中的 URL 地址，并将其分派给相应的处理代码。现在，请你来实现一个简单的 URL 映射功能。
　　本题中 URL 映射功能的配置由若干条 URL 映射规则组成。当一个请求到达时，URL 映射功能会将请求中的 URL 地址按照配置的
    先后顺序逐一与这些规则进行匹配。当遇到第一条完全匹配的规则时，匹配成功，得到匹配的规则以及匹配的参数。若不能匹配任
    何一条规则，则匹配失败。
    本题输入的 URL 地址是以斜杠 / 作为分隔符的路径，保证以斜杠开头。其他合法字符还包括大小写英文字母、阿拉伯数字、减号 -、
    下划线 _ 和小数点 .。例如，/person/123/ 是一个合法的 URL 地址，而 /person/123? 则不合法（存在不合法的字符问号 ?）。
    另外，英文字母区分大小写，因此 /case/ 和 /CAse/ 是不同的 URL 地址。
　　对于 URL 映射规则，同样是以斜杠开始。除了可以是正常的 URL 地址外，还可以包含参数，有以下 3 种：
　　字符串 <str>：用于匹配一段字符串，注意字符串里不能包含斜杠。例如，abcde0123。
　　整数 <int>：用于匹配一个不带符号的整数，全部由阿拉伯数字组成。例如，01234。
　　路径 <path>：用于匹配一段字符串，字符串可以包含斜杠。例如，abcd/0123/。
　　以上 3 种参数都必须匹配非空的字符串。简便起见，题目规定规则中 <str> 和 <int> 前面一定是斜杠，后面要么是斜杠，要么是
    规则的结束（也就是该参数是规则的最后一部分）。而 <path> 的前面一定是斜杠，后面一定是规则的结束。无论是 URL 地址还是规则，
    都不会出现连续的斜杠。

输入格式
　　输入第一行是两个正整数 n 和 m，分别表示 URL 映射的规则条数和待处理的 URL 地址个数，中间用一个空格字符分隔。
　　第 2 行至第 n+1 行按匹配的先后顺序描述 URL 映射规则的配置信息。第 i+1 行包含两个字符串 pi 和 ri，其中 pi 表示 URL 
    匹配的规则，ri 表示这条 URL 匹配的名字。两个字符串都非空，且不包含空格字符，两者中间用一个空格字符分隔。
　　第 n+2 行至第 n+m+1 行描述待处理的 URL 地址。第 n+1+i 行包含一个字符串 qi，表示待处理的 URL 地址，字符串中不包含空格字符。

输出格式
　　输入共 m 行，第 i 行表示 qi 的匹配结果。如果匹配成功，设匹配了规则 pj ，则输出对应的 rj。同时，如果规则中有参数，则在同一
    行内依次输出匹配后的参数。注意整数参数输出时要把前导零去掉。相邻两项之间用一个空格字符分隔。如果匹配失败，则输出 404。

样例输入
5 4
/articles/2003/ special_case_2003
/articles/<int>/ year_archive
/articles/<int>/<int>/ month_archive
/articles/<int>/<int>/<str>/ article_detail
/static/<path> static_serve
/articles/2004/
/articles/1985/09/aloha/
/articles/hello/
/static/js/jquery.js

样例输出
year_archive 2004
article_detail 1985 9 aloha
404
static_serve js/jquery.js

样例说明
　　对于第 1 个地址 /articles/2004/，无法匹配第 1 条规则，可以匹配第 2 条规则，参数为 2004。
　　对于第 2 个地址 /articles/1985/09/aloha/，只能匹配第 4 条规则，参数依次为 1985、9（已经去掉前导零）和 aloha。
　　对于第 3 个地址 /articles/hello/，无法匹配任何一条规则。
　　对于第 4 个地址 /static/js/jquery.js，可以匹配最后一条规则，参数为 js/jquery.js。

数据规模和约定
　　1 ≤ n ≤ 100，1 ≤ m ≤ 100。
　　所有输入行的长度不超过 100 个字符（不包含换行符）。
　　保证输入的规则都是合法的。


解题思路:
把规则和url都通过反斜杠'/'切割成子串保存在vector中, 为了方便, 在vector中的对应位置处依然放入了'/'
如何检测规则与url是否匹配?
将规则和url切割后,如果规则中没有"<path>",则切割后的规则与url数组的长度一定要相等,否则不匹配,具体过程如下:
1.依次查看规则中的每一个字段, 有如下四种情况:
    1. <int>: url对应字段中必须都是数字,可以有前导0, 输出参数的时候要将前导0去掉
    2. <str>: url对应字段中只能出现大小写字母,数字,减号'-',下划线'_',小数点'.'
    3. <path>: url从当前字段开始及以后的字符串都是<path>的参数, 注意如果后面有反斜杠'/'的话,也要带上反斜杠
    4. 普通字符串: url对应字段中的字符串与规则中对应位置的字符串必须严格一致

需要注意的地方:
1. /abc/<str>/ 可以匹配 /abc/abc/ , 此时参数为 abc,  不能匹配 /abc/abc
2. /abc/<str>  可以匹配 /abc/abc  , 此时参数为 abc,  不能匹配 /abc/abc/
3. /abc/<path> 可以匹配 /abc/abc/ , 此时参数为 abc/, 也可以匹配 /abc/abc , 此时参数是 abc
4. 规则中的<path>后面不会出现反斜杠'/'
*/


#include<iostream>
#include<cstdio>
#include<string>
#include<vector>
#include<cstring>
#include<algorithm>
#include<iterator>
using namespace std;
const int maxn = 105;

string names[maxn];
vector<string> rules[maxn];

//判断字符串r是否是一个合法的url
bool check(string& r)
{
    if(r[0] != '/') return 0;
    for(int i=1, n=r.size(); i<n ; ++i )
    {
        if(r[i] == '/' && r[i-1]=='/') return 0;
        if( !( isalpha(r[i]) || isdigit(r[i]) || r[i]=='.'|| r[i]=='-'|| r[i]=='_' || r[i]=='/') ) return 0;
    }
    return 1;
}

//将字符串r从反斜杠'/'处切割成子串保存到vector中,同时在原处插入'/'
void parse(vector<string>& v, string &r)
{
    string field;
    int n=r.size();
    for(int i=1; i<n; ++i )
    {
        if(r[i] == '/')
        {
            v.push_back(field);
            v.push_back("/");
            field = "";
        }else field += r[i];
    }
    if(r[n-1] !='/') v.push_back(field);
}

bool match(vector<string> &fields, vector<string>& rule, vector<string> &param  )
{
    int cnt = fields.size(), i=0;
    for(int n=rule.size(); i<n; ++i)
    {
        if( i >= cnt ) return 0; //如果规则中字段个数大于url中字段个数, 则不匹配
        string field = fields[i];
        if(rule[i] == "<int>")
        {
            int pos=0;
            while(field[pos] == '0') ++pos; //去掉前导0
            field = field.substr(pos);
            for(int j=0, m=field.size(); j < m; ++j) if(!isdigit(field[j])) return 0; //如果数字参数中出现了其他字符,则不匹配
            param.push_back(field);
        }
        else if(rule[i] == "<str>")//判断"<str>"参数中是否包含非法字符,前面已经判断过了,因此这里不再判断
        {
            param.push_back(field);
        }
        else if(rule[i] == "<path>") //当规则中出现"<path>"时 ,则url中剩下的都是"<path>"的参数
        {
            string path = field;
            for(int j=i+1; j<cnt; ++j) path += fields[j];
            param.push_back(path);
            return 1;
        }
        else //朴素路径
        {
            if(rule[i] != field) return 0;
        }
    }
    return i == cnt; //如果规则中没有出现"<path>",则规则和url中字段的个数必须相等
}

int main()
{
#ifdef WFX
freopen("in.txt","r",stdin);
freopen("out.txt","w",stdout);
#endif
    int n,m;
    string r, url;
    vector<string> fields, param;
    
    scanf("%d%d",&n,&m);
    for(int i=0; i<n; ++i)
    {
        cin>>r>>names[i];
        parse(rules[i], r);//解析出每条规则的各个字段
    }
    for(int i=0; i<m; ++i)
    {
        cin>>url;
        if(!check(url)) //判断url是否合法
        {
            printf("404\n");
            continue;
        }
        fields.clear(); param.clear();
        parse(fields, url);

        bool found = false;
        for(int j=0; j<n; ++j)
        {
            param.clear();
            if( match(fields, rules[j], param) )
            {
                cout<<names[j]<<" ";
                copy(param.begin(), param.end() ,ostream_iterator<string>(cout," "));
                printf("\n");
                found = true;
                break;
            }
        }
        if(!found) printf("404\n");
    }

    return 0;
}