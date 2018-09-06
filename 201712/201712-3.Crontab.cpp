/*
Crontab
问题描述
Cron (源自希腊语xoovoc.意思是时间)是类Unix系统下基干时间的任务调度器,用于在固定的,时间运行指定的任务(多为系统管理和维护的任务) 。
Cron适合于周期性重复调度的任务,通过crontab文件来描述调度任务的配置。

Crontab文件由若干行组成,每行是一条配置信息,格式如下:
<minutes> <hours> <day of month> <month> <day of week> <command>

表示在满足前5项所指定的时间来运行第6项<command>所描述的命令。前5项用于描述时间,含义和取值范围如下:
. minutes>是分钟数,取值范围是0-59;
. <hours>是小时数,取值范围是0-23;
. day of month>是月份中的天数,取值范围是1-31;
. <month>是月份,取值范围是1-12,或Jan-Dec ;
. <day of week>是星期几,取值范围是0-6,或Sun-Sat。
.
对于前5项,除了可以直接给出数字或者菜文缩写(不区分大小写)外, 还可以出现星号* (表示  ,任何取值) ,逗号, 
(表示多个不同的取值)或减号一 (表示一段连续的取位范围) ,星号只能单独出现,减号和逗号可以配合出现。
Cron每分钟检查一次系统时间,当系统时间同时满足这5项要求时, cron将执行对应的命令。给出,一个时间段,
以及一个crontab文件,请你编程输出在这段时间内的任务调度执行情况。

输入格式
输入第一行有3个整数n、s、t,用空格分隔。n表示接下来有n行,描述一个crontab文件。s和,t分别为系统运行
的开始时间(包含)和结束时间(不包含) ,格式为yyyymmddHHM (年、月、日、小时、分钟) 。
接下来有n行,每行是一条crontab配置信息,相邻两项之间用一个空格分隔。

输出格式
输出有若干行,每行表示一个任务调度,由两部分构成:
第一部分是任务调度的时间,格式同样为  ,yyyymmddHHMM ;
第二部分是调度执行的命令,两部分之间用一个空格分隔。按照时间先后顺序输出。如果同一时刻有多条命令满足调度条件,则按照输入给出的顺序输出。

样例输入
3 201711170032 201711222352
0 7 * * 1,3-5 get_up
30 23 * * Sat,Sun go_to_bed
15 12,18 * * * have_dinner

样例输出
201711170700 get_up
201711171215 have_dinner
201711171815 have_dinner
201711181215 have_dinner
201711181815 have_dinner
201711182330 go_to_bed
201711191215 have_dinner
201711191815 have_dinner
201711192330 go_to_bed
201711200700 get_up
201711201215 have_dinner
201711201815 have_dinner
201711211215 have_dinner
201711211815 have_dinner
201711220700 get_up
201711221215 have_dinner
201711221815 have_dinner



解题思路:
本题有两种解题思路. 
第一种思路是解析出每条规则对应的各个字段中的所有值, 然后枚举这些值所有可能的组合, 也就是枚举所有可能的时间点,
将所有处在给定范围内的条目按要求排序后输出.由于每一条规则对应的合法时间点并不是很多, 因此效率比较高. 我这里采用的
就是这种方法, 执行时间只有31ms.

第二种思路是枚举题目给定时间范围内的每一个时间点(以分钟为单位), 并判断每个时间点上是否有命令会被执行,由于
这种枚举方法可以按照时间和输入顺序进行, 因此枚举到一个可以执行的命令后即可输出, 不需要保存结果. 但是由于
题目给定的时间返回跨度非常大, 因此计算量非常大. 如果代码足够优化,还是可以在10s内跑完的.

这里说一下第一种方法的具体思路:
根据输入的起止时间, 计算出开始年份和结束年份.
1. 按照输入顺序对每一条规则进行如下处理:
    1. 解析出 日, 月, 时, 分, 星期, 这五个字段中每个字段的那些值在规则之内, 将规则之内的值在对应数组中做上标记
    2. 通过一个五重循环, 分别枚举 年, 月, 日, 时, 分 这五个字段; 枚举的时候, 逐级判断当前字段枚举的值是否在
       对应数组中被标记了, 如果被标记了, 则进入下一层循环枚举下一个字段, 否则, 继续枚举当前字段的下一个值
    3. 枚举完年月日后, 计算出该日期是星期几, 并查看星期字段中对应的值是否被标记, 如果没有被标记,如果该日期不在
       规则之内, 可以跳过.
    4. 对枚举到的每一个时间点, 判断其是否在题目规定的时间范围之内, 把在规定时间范围内的条目保存起来
2. 将上述枚举到的所有条目, 先按照时间升序排序, 时间相同时, 按照输入的顺序排序.
3. 按要求输出排序结果

其他:
a.在枚举月份的时候, 要根据年份和月份判断该月有多少天, 枚举的天数值不能超过月份中的天数, 也就是要保证枚举的是一个合法的时间.
b.在枚举完年月日后, 如何根据年月日计算出这一天是星期几, 这里使用蔡勒公式进行计算的, 公式的原理我还没懂, 后面有时间再去学,那么
  考场上如果不会这个公式, 就只能自己推算了.
c.如何解析每个字段:
  对于上述五个字段, 解析方法都是一样的:
    1. 先使用逗号','切割字符串
    2. 对切割后的每一个子串, 在使用减号'-'进行切割
        1.如果切割后得到单个字符串, 说明子串中没有减号'-', 子串表示单个值, 故只需要把该值在对应数组中标记即可
        2.如果切割后得到连个字符串, 说明子串中有减号'-', 子串表示的是一个范围, 故要将该范围在对应数字中标记

*/

#include<iostream>
#include<cstdio>
#include<vector>
#include<map>
#include<algorithm>
#include<cstdlib>
#include<cstring>
using namespace std;
typedef long long LL;

map<string,int> monthMap;
map<string,int> weekdayMap;

bool _months[13], _days[32], _hours[24], _minutes[60], _weekdays[7];

int monthDay[]={0, 31, 29, 31, 30, 31, 30, 31, 31,30,31, 30, 31};

string monthAbbr[] = {"", "jan", "feb", "mar","apr", "may", "jun","jul","aug", "sep","oct", "nov", "dec"};
string weekdayAbbr[] = {"sun","mon","tue","wed","thu","fri","sat"};

struct Item
{
    int order;
    LL time;
    string command;
    Item(int order,LL t, string& cmd):order(order),time(t), command(cmd){}
    bool operator<(const Item& it)const
    {
        if(time != it.time) return time < it.time;
        return order < it.order;
    }
};

void init_maps()
{
    for(int i=1; i<=12; ++i) monthMap[ monthAbbr[i] ] = i;
    for(int i=0; i<=6; ++i) weekdayMap[ weekdayAbbr[i] ] = i;
}

bool isLeapYear(int year){ return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; }

void toLower(string& s){ for(int i=0,n=s.size(); i<n; ++i) s[i] = tolower(s[i]); }

//蔡勒公式, 根据给定的年月日返回星期数, 星期天=0, 星期六=6
int get_day_of_week(int year,int month,int day)
{
    if(month == 1 || month == 2) {
        month += 12;
        year--;
    }
    int c = year / 100;
    int y = year % 100;
    int m = month;
    int d = day;
    int w = c / 4 - 2 * c + y + y / 4 + 26 * (m + 1) / 10 + d - 1;
    return w < 0 ? (w + (-w / 7 + 1) * 7) % 7 : w % 7;
}

//用给定的字符切割字符串
vector<string> split(string & s, char ch)
{
    vector<string> v;
    string tmp;
    for(int i=0, n=s.size(); i<n; ++i)
    {
        if(s[i] == ch)
        {
            v.push_back(tmp);
            tmp = "";
        }else tmp += s[i];
    }
    v.push_back(tmp);

    return v;
}


//将字符串s中表示的范围在bool数组a中用1填充
void parse(string &s, int beg, int end, bool * a, map<string,int> & m=monthMap)
{
    // cout<<"s : "<<s<<endl;
    if(s == "*" ) for(int i=beg; i<=end; ++i) a[i] = 1;
    else
    {
        vector<string> v1 = split(s, ',');
        for(int i=0,n=v1.size(); i<n; ++i)
        {
            vector<string> v2 = split(v1[i], '-');
            toLower(v2[0]);
            if(v2.size() == 2) toLower(v2[1]);
            if(v2.size() == 1)
            {
                isalpha(v2[0][0]) ? a[ m[ v2[0] ] ] = 1 : a[ atoi(v2[0].c_str() ) ] = 1;
            }
            else
            {
                int L = isalpha(v2[0][0]) ? m[v2[0]] : atoi(v2[0].c_str());
                int R = isalpha(v2[1][0]) ? m[v2[1]] : atoi(v2[1].c_str());
                if(L <= R) for(int j=L; j<=R; ++j) a[j] = 1;
                else //题目已经说明不会出现else中的情况, 但如果实际项目中出现了这种情况, 就可以像下面这样处理
                {
                    for(int j=L; j<=end; ++j) a[j] = 1;
                    for(int j=beg; j<=R; ++j) a[j] = 1;
                }
            }
        }
    }
    // for(int i=beg; i<=end; ++i) printf("%d ",a[i]);
    // cout<<endl;
}


int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    #endif
    
    init_maps();
    vector<Item> ans;
    LL N,S,T;
    string minutes, hours, day_of_month, months, day_of_week, cmd;
    ans.reserve(10000);

    cin>>N>>S>>T;
    int minYear = S / 1e8, maxYear = T / 1e8;
    //时间格式: yyyymmddHHMM(年 月 日 时 分)
    for(int i=0; i<N; ++i)
    {
        cin>>minutes>>hours>>day_of_month>>months>>day_of_week>>cmd;
        memset(_minutes, 0, sizeof(_minutes));
        memset(_hours, 0, sizeof(_hours));
        memset(_days, 0, sizeof(_days));
        memset(_months, 0, sizeof(_months));
        memset(_weekdays, 0, sizeof(_weekdays));

        parse(minutes, 0,59,  _minutes);
        parse(hours, 0, 23, _hours);
        parse(day_of_month, 1, 31, _days);
        parse(months, 1,12, _months);
        parse(day_of_week, 0, 6, _weekdays, weekdayMap);
        
        for(LL year=minYear; year<=maxYear; ++year) //枚举年份
        {
            for(int mm=1; mm<=12;  ++mm) //枚举月份
            {
                if(_months[mm] == 0) continue;
                if(mm==2) monthDay[2] = isLeapYear(year) ? 29 : 28;
                for(int dd=1; dd<=monthDay[mm]; ++dd) //枚举天数, 需要根据年份和月份判断该月有多少天
                {
                    if(_days[dd] == 0) continue;
                    int w = get_day_of_week(year, mm, dd);
                    if(_weekdays[w] == 0) continue;

                    for(int HH=0; HH<24; ++HH) //枚举小时
                    {
                        if(_hours[HH] == 0) continue;
                        for(int MM=0; MM < 60; ++MM) //枚举分钟
                        {
                            if(_minutes[MM] == 0) continue;

                            LL y = year * 1e8;
                            LL m = mm * 1e6;
                            LL d = dd * 1e4;
                            LL h = HH * 1e2;
                            LL time = y + m + d + h + MM;
                            
                            if(S <= time && time<T) //如果时间在区间之内, 则存入结果中
                            {
                                // printf("%d-%d-%d %d:%d  %lld %s\n", year, mm,dd, HH, MM, time, cmd.c_str() );
                                ans.push_back(Item(i, time, cmd));
                            } 
                        }
                    }
                }
            }
        }   
    }

    sort(ans.begin(), ans.end());
    for(int i=0, n=ans.size(); i<n; ++i) printf("%lld %s\n", ans[i].time, ans[i].command.c_str());
    

    return 0;
};