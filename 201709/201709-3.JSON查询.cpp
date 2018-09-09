/*
试题编号：	201709-3
试题名称：	JSON查询
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　JSON (JavaScript Object Notation) 是一种轻量级的数据交换格式，可以用来描述半结构化的数据。JSON 格式中的基本单元
    是值 (value)，出于简化的目的本题只涉及 2 种类型的值：
    * 字符串 (string)：字符串是由双引号 " 括起来的一组字符（可以为空）。如果字符串的内容中出现双引号 "，在双引号前面加反斜杠，
      也就是用 \" 表示；如果出现反斜杠 \，则用两个反斜杠 \\ 表示。反斜杠后面不能出现 " 和 \ 以外的字符。例如：""、"hello"、"\"\\"。
　　* 对象 (object)：对象是一组键值对的无序集合（可以为空）。键值对表示对象的属性，键是属性名，值是属性的内容。对象以左花括
      号 { 开始，右花括号 } 结束，键值对之间以逗号 , 分隔。一个键值对的键和值之间以冒号 : 分隔。键必须是字符串，同一个对象所有键
      值对的键必须两两都不相同；值可以是字符串，也可以是另一个对象。
      例如：{}、{"foo": "bar"}、{"Mon": "weekday", "Tue": "weekday", "Sun": "weekend"}。
　　除了字符串内部的位置，其他位置都可以插入一个或多个空格使得 JSON 的呈现更加美观，也可以在一些地方换行，不会影响所表示的数据内容。
    例如，上面举例的最后一个 JSON 数据也可以写成如下形式。
　　{
　　"Mon": "weekday",
　　"Tue": "weekday",
　　"Sun": "weekend"
　　}
　　给出一个 JSON 格式描述的数据，以及若干查询，编程返回这些查询的结果。

输入格式
　　第一行是两个正整数 n 和 m，分别表示 JSON 数据的行数和查询的个数。
　　接下来 n 行，描述一个 JSON 数据，保证输入是一个合法的 JSON 对象。
　　接下来 m 行，每行描述一个查询。给出要查询的属性名，要求返回对应属性的内容。需要支持多层查询，各层的属性名之间用小数点 . 连接。保证查询的格式都是合法的。

输出格式
　　对于输入的每一个查询，按顺序输出查询结果，每个结果占一行。
　　如果查询结果是一个字符串，则输出 STRING <string>，其中 <string> 是字符串的值，中间用一个空格分隔。
　　如果查询结果是一个对象，则输出 OBJECT，不需要输出对象的内容。
　　如果查询结果不存在，则输出 NOTEXIST。

样例输入
10 5
{
"firstName": "John",
"lastName": "Smith",
"address": {
"streetAddress": "2ndStreet",
"city": "NewYork",
"state": "NY"
},
"esc\\aped": "\"hello\""
}
firstName
address
address.city
address.postal
esc\aped

样例输出
STRING John
OBJECT
STRING NewYork
NOTEXIST
STRING "hello"

评测用例规模与约定
　　n ≤ 100，每行不超过 80 个字符。
　　m ≤ 100，每个查询的长度不超过 80 个字符。
　　字符串中的字符均为 ASCII 码 33-126 的可打印字符，不会出现空格。所有字符串都不是空串。
　　所有作为键的字符串不会包含小数点 .。查询时键的大小写敏感。
　　50%的评测用例输入的对象只有 1 层结构，80%的评测用例输入的对象结构层数不超过 2 层。举例来说，{"a": "b"} 是一层结构的对象，
    {"a": {"b": "c"}} 是二层结构的对象，以此类推。


解题思路:
首对输入数据进行预处理
1. 去掉所有的空白符,包含空格,换行符, 制表符
2. 去掉 key 和 value 字符串首尾的冒号, 如果不去掉,在查询输出的时候还是要去掉
3. 如果字符串中出现了引号, 则该引号前面一定有一个反斜杠用于转义(否则该引号不可能出现在字符串中间), 
   去掉用于转义的反斜杠,保留字符串中间的引号
4. 如果连续出现两个反斜杠,则第一个反斜杠用于转义, 第二个反斜杠要作为普通字符出现在字符串中,并且第二个
   反斜杠不在具有转义的功能,也就是说,假如出现了 "\\str\\", 则处理后应该为 \str\

对应输入样例,处理之后就变成了这个样子:
{firstName:John,lastName:Smith,address:{streetAddress:2ndStreet,city:NewYork,state:NY},esc\aped:"hello"}

Json数据结构的定义:
struct Json
{
    map<string,string> strDict;
    map<string,Json*> objDict;
};
Json数据结构里面只有这样两个成员变量, strDict用于保存值是字符串的键值对, objDict用于保存值是对象的键值对,
由于Json数据结构本身就具有递归的结构, 因此这里的对象正好可以是Json对象本身,这里保存对象的指针更方便高效一些.

使用预处理后的json字符串构建json对象
首先找一下规律:
每个处理后的json字符串的首尾都是花括号, 中间都是用逗号','隔开的键值对.
每个键值对的键都是字符串, 值或者是字符串, 或者一个处理后的json字符串.
可以看到json字符串具有递归的性质, 因此, 可以使用递归的方法构建Json对象
1. 去掉json字符串首尾的花括号, 然后使用','切割字符串, 得到所有的键值对,
   这里需要注意的是, 在使用','进行切割的时候, 如果某个键值对的值是对象, 
   要将该对象作为整体, 不能把它给切割了.
2. 对于所有的键值对,用冒号':'切割, 得到键和值, 如果值是字符串, 则将该值
   插入到strDict中, 如果值是对象, 则递归构建该对象, 并把该对象插入到当前
   json对象的objDict中

使用给定的字符串进行查询:
多层查询与单层查询是一样的,都是根据给定的键到strDict和objDict中去查找,
对于某个n层查询, 其前面的n-1层查询, 一定是到objDict中去查找,最后一层
查询可以到两个字典中去查找.

这里还编写了一个打印json对象的方法, 由于Json数据结构本身就具有递归的性质,
因此打印也需要使用递归的方法进行打印.

最后给出一组测试数据和输出结果:
输入:
18 7
{
    "firstName": "John",
    "lastName": "Smith",
    "address": {
        "street":{
            "streetNo":"2nd",
            "streetName" : "bake"
        },
        "city": "NewYork",
        "state": "NY"
    },
    "esc\\aped": "\"hello\"",
    "children":{
        "name1":"\\value1\\",
        "Name1":"value2",
        "name3":"value3"
    }
}

firstName
address.street.streetName
address.city
address.postal
esc\aped
children.name1
children.Name1

输出:
STRING John
STRING bake
STRING NewYork
NOTEXIST
STRING "hello"
STRING \value1\
STRING value2

打印结果:
{
        esc\aped:"hello"
        firstName:John
        lastName:Smith
        address:{
                city:NewYork
                state:NY
                street:{
                        streetName:bake
                        streetNo:2nd
                }
        }
        children:{
                Name1:value2
                name1:\value1\
                name3:value3
        }
}

*/


#include<iostream>
#include<cstdio>
#include<map>
#include<vector>
using namespace std;

struct Json
{
    map<string, string> strDict;
    map<string, Json*> objDict;
    
    static Json* build(string jsonStr)
    {
        Json *json = new Json();
        // cout<<jsonStr<<endl;
    	string str = jsonStr.substr(1, jsonStr.size() - 2); //去掉首尾的花括号
        vector<string> v = split(str.c_str());
        for(int i=0,n=v.size(); i<n; ++i)
        {
            int pos = v[i].find(':');
            string key = v[i].substr(0, pos);
            string value = v[i].substr(pos+1);
            // cout<<key << " -- "<<value<<endl;
            if(value[0] != '{') json->strDict[key] = value;
            else json->objDict[key] = build(value);
        }
        return json;
	}

    static vector<string> split(const char* s) //从逗号','处分开,并且要将花括号{}中的内容当做一个整体,不进行分割
    {
        vector<string> v;
        int brace=0;
        string str;
        for(int i=0; s[i]; ++i)
        {
            if(s[i] == '{') ++brace;
            if(s[i] == '}') --brace;

            if(s[i] != ',') str += s[i];
            else 
            {
                if(brace){ str+=s[i]; continue; }
                v.push_back(str);
                str = "";
            }
        }
        if(str != "") v.push_back(str);
        return v;
    }
    
    //使用给定的字符切割字符串
    static vector<string> split(const char* s, char ch)
    {
        vector<string> v;
        string tmp;
        for(int i=0; s[i]; ++i)
        {
            if(s[i] != ch) tmp += s[i];
            else
            {
                v.push_back(tmp);
                tmp = "";
            }
        }
        if(tmp != "") v.push_back(tmp);
        return v;
    }

    //打印Json数据结构
    static void print(Json* json,int depth=1)
    {
    	string tabs(depth, '\t');
        cout<<"{"<<endl;
        //打印键值对
        map<string, string>::iterator it1 = json->strDict.begin();
        for(; it1 != json->strDict.end(); ++it1) 
            cout<< tabs << it1->first << ":" <<it1->second<<endl;

        //递归打印对象
         map<string, Json*>::iterator it2 = json->objDict.begin();
         for(; it2!= json->objDict.end(); ++it2 )
            cout<<tabs<<it2->first << ":"; print(it2->second, depth+1);
        cout<<string(depth-1,'\t') << "}" <<endl;
    }

    void print(){ print(this); }

    //根据给定的字符串查询
    string query(char query[])
    {
        vector<string> v = split(query, '.');
        Json* json = this;
        for(int i=0,n=v.size(); i<n; ++i)
        {
            if(i == n-1) //最后一段可能是在字符串字典中查询, 也可能是在对象字典中查询, 如果都没有,则返回不存在
            {
                if(json->strDict.count(v[i])) return "STRING " + json->strDict[v[i]];
                else if(json->objDict.count(v[i])) return "OBJECT";
                else return "NOTEXIST";
            }
            else //查询的路径中,前面n-1段一定是在对象字典中查询
            {
                if(json->objDict.count(v[i])) json = json->objDict[v[i]];
                else return "NOTEXIST";
            }
        }
    }
};


inline bool white(char ch){ return  ch ==' ' || ch=='\n' || ch=='\t';}

int main()
{
    #ifdef WFX
    freopen("in.json","r",stdin);
    #endif
    string jsonStr;
    char str[100];
    int N,M;
    scanf("%d%d\n", &N,&M); //这里需要吃掉第一行的换行符,否则会少读取一行
    for(int i=0; i<N; ++i)
    {
        gets(str);
        for(int i=0; str[i]; ++i)
        {
            if( white(str[i]) ) continue; //跳过空白字符
            if(i==0 && str[i] == '\"') continue; //跳过行首的冒号
            if(str[i] == '\"' && i>0 && str[i-1] !='\\') continue; //如果冒号没有出现在反斜杠后面, 则跳过这个冒号

            if(str[i] == '\\' && str[i-1]=='\\')//出现连续两个反斜杠时,取消第二个反斜杠的转义功能,并将第二个反斜杠添加到json字符串中
            {
                jsonStr+='\\';
                str[i]=' '; 
                continue;
            }
            if(str[i] == '\\' && (str[i+1]=='\\' || str[i+1]=='\"')) continue; //跳过用于转意的反斜杠,这一步检查需要在上一步之后, 否则会出错
            
            jsonStr += str[i];
        }
    }

    Json *json = Json::build(jsonStr);

    // json->print(); //打印出Json对象的结构和内部信息

    for(int i=0; i<M; ++i)
    {
        scanf("%s",str);
        printf("%s\n", json->query(str).c_str() );
    }
    

    return 0;
}
