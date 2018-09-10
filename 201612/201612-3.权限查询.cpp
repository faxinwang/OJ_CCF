/*
试题编号：	201612-3
试题名称：	权限查询
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　授权 (authorization) 是各类业务系统不可缺少的组成部分，系统用户通过授权机制获得系统中各个模块的操作权限。
　　本题中的授权机制是这样设计的：每位用户具有若干角色，每种角色具有若干权限。例如，用户 david 具有 manager 角色，
    manager 角色有 crm:2 权限，则用户 david 具有 crm:2 权限，也就是 crm 类权限的第 2 等级的权限。
　　具体地，用户名和角色名称都是由小写字母组成的字符串，长度不超过 32。权限分为分等级权限和不分等级权限两大类。
    分等级权限由权限类名和权限等级构成，中间用冒号“:”分隔。其中权限类名也是由小写字母组成的字符串，长度不超过 32。
    权限等级是一位数字，从 0 到 9，数字越大表示权限等级越高。系统规定如果用户具有某类某一等级的权限，那么他也将自动
    具有该类更低等级的权限。例如在上面的例子中，除 crm:2 外，用户 david 也具有 crm:1 和 crm:0 权限。不分等级权限
    在描述权限时只有权限类名，没有权限等级（也没有用于分隔的冒号）。
　　给出系统中用户、角色和权限的描述信息，你的程序需要回答多个关于用户和权限的查询。查询可分为以下几类：
　　* 不分等级权限的查询：如果权限本身是不分等级的，则查询时不指定等级，返回是否具有该权限；
　　* 分等级权限的带等级查询：如果权限本身分等级，查询也带等级，则返回是否具有该类的该等级权限；
　　* 分等级权限的不带等级查询：如果权限本身分等级，查询不带等级，则返回具有该类权限的等级；如果不具有该类的任何等
    级权限，则返回“否”。
输入格式
　　输入第一行是一个正整数 p，表示不同的权限类别的数量。紧接着的 p 行被称为 P 段，每行一个字符串，描述各个权限。
    对于分等级权限，格式为 <category>:<level>，其中 <category> 是权限类名，<level> 是该类权限的最高等级。对
    于不分等级权限，字符串只包含权限类名。
　　接下来一行是一个正整数 r，表示不同的角色数量。紧接着的 r 行被称为 R 段，每行描述一种角色，格式为
　　<role> <s> <privilege 1> <privilege 2> ... <privilege s>
　　其中 <role> 是角色名称，<s> 表示该角色具有多少种权限。后面 <s> 个字符串描述该角色具有的权限，格式同 P 段。
　　接下来一行是一个正整数 u，表示用户数量。紧接着的 u 行被称为 U 段，每行描述一个用户，格式为
　　<user> <t> <role 1> <role 2> ... <role t>
　　其中 <user> 是用户名，<t> 表示该用户具有多少种角色。后面 <t> 个字符串描述该用户具有的角色。
　　接下来一行是一个正整数 q，表示权限查询的数量。紧接着的 q 行被称为 Q 段，每行描述一个授权查询，格式为 
    <user> <privilege>，表示查询用户 <user> 是否具有 <privilege> 权限。如果查询的权限是分等级权限，则查询
    中的 <privilege> 可指定等级，表示查询该用户是否具有该等级的权限；也可以不指定等级，表示查询该用户具有该权限
    的等级。对于不分等级权限，只能查询该用户是否具有该权限，查询中不能指定等级。
输出格式
　　输出共 q 行，每行为 false、true，或者一个数字。false 表示相应的用户不具有相应的权限，true 表示相应的用户具有
    相应的权限。对于分等级权限的不带等级查询，如果具有权限，则结果是一个数字，表示该用户具有该权限的（最高）等级。
    如果用户不存在，或者查询的权限没有定义，则应该返回 false。

样例输入
3
crm:2
git:3
game
4
hr 1 crm:2
it 3 crm:1 git:1 game
dev 2 git:3 game
qa 1 git:2
3
alice 1 hr
bob 2 it qa
charlie 1 dev
9
alice game
alice crm:2
alice git:0
bob git
bob poweroff
charlie game
charlie crm
charlie git:3
malice game

样例输出
false
true
false
2
false
true
false
true
false

样例说明
　　样例输入描述的场景中，各个用户实际的权限如下：
　　* 用户 alice 具有 crm:2 权限
　　* 用户 bob 具有 crm:1、git:2 和 game 权限
　　* 用户 charlie 具有 git:3 和 game 权限
　　* 用户 malice 未描述，因此不具有任何权限

评测用例规模与约定
　　评测用例规模：
　　* 1 ≤ p, r, u ≤ 100
　　* 1 ≤ q ≤ 10 000
　　* 每个用户具有的角色数不超过 10，每种角色具有的权限种类不超过 10
　　约定：
　　* 输入保证合法性，包括：
　　1) 角色对应的权限列表（R 段）中的权限都是之前（P 段）出现过的，权限可以重复出现，如果带等级的权限重复出现，以等级最高的为准
　　2) 用户对应的角色列表（U 段）中的角色都是之前（R 段）出现过的，如果多个角色都具有某一分等级权限，以等级最高的为准
　　3) 查询（Q 段）中的用户名和权限类名不保证在之前（U 段和 P 段）出现过
　　* 前 20% 的评测用例只有一种角色
　　* 前 50% 的评测用例权限都是不分等级的，查询也都不带等级

*/


#include<iostream>
#include<cstdio>
#include<map>
#include<set>
#include<algorithm>
using namespace std;

struct Role
{
    map<string, char> pri; //保存该角色所有的权限
    void addPrivilege(string p, char lv)
    {
        if(lv == '-') pri[p] = '-';
        else pri[p] = max(pri[p] , lv);
    }
};

struct User
{
    map<string, char> pri; //保存该用户所有的权限

    void addRole(Role* r)
    {
        map<string,char>::iterator pos = r->pri.begin();
        for(; pos != r->pri.end(); ++pos)
        {
            const string& pname = pos->first;
            char lv = pos->second;
            if(lv == '-') pri[pname] = '-';
            else pri[pname] = max(pri[pname], lv);
        }
    }

    /**对于分等级权限的不带等级查询, lv参数应该传入'-', 如果用户具有相应的权限, 则反向
     * 相应的等级, 否则返回"false"
     * 
     * 对于分等级权限的带等级查询, lv参数应传入要查询的等级, 如果用户具有相应的权限和等级, 
     * 则返回"true", 否则返回"false"
     * 
     * 对于不分等级权限的查询, lv参数应传入'-', 如果用户具有相应的权限, 则返回"true",否则返回"false"
    */
    
    string query(string pname, char lv)
    {
        string ret;
        if(pri.count(pname)) //如果用户具有相应的权限
        {
            char t = pri[pname];
            if(t =='-' && lv == '-' ) return "true"; //不分等级权限的查询
            if(t != '-' && lv == '-') return ret + t; //分等级权限的不带等级查询
            if(t != '-' && lv != '-') return t>=lv? "true" : "false"; //分等级权限的带等级查询
        }
        return "false";
    }
};

map<string, char> priMap; //保存所有权限, 如果权限不分等级, 则用'-'表示, 否则值为相应的等级
map<string, Role*> roles; //保存所有的角色, key为角色名, 值为角色结构体指针
map<string, User*> users; //保存所有的用户, key为用户名, 值为用户结构体指针

int main()
{
    #ifdef WFX
    freopen("in.txt", "r", stdin);
    #endif
    int n,m;
    string str,rname, uname, pname, lv;

    scanf("%d",&n); 
    for(int i=0; i<n; ++i) //读取n个权限的信息
    {
        cin>>str;
        int p = str.find(':');
        pname = str.substr(0, p);
        lv = p < 0 ?  "-" : str.substr(p+1) ;
        priMap[pname] = lv[0];
    }

    scanf("%d",&n);
    for(int i=0; i<n; ++i) //读取n个角色的信息
    {
        Role *r = new Role();
        cin>>rname>>m;
        roles[rname] = r;
        
        for(int j=0; j<m; ++j)
        {
            cin>>str;
            int p = str.find(':');
            pname = str.substr(0, p);
            lv = p < 0 ?  "-" : str.substr(p+1) ;
            r->addPrivilege(pname, lv[0]);
        }
    }

    scanf("%d",&n);
    for(int i=0; i<n; ++i) //读取n个用户的信息
    {
        User* u = new User();
        cin>>uname>>m;
        users[uname] = u;
        for(int j=0; j<m; ++j)
        {
            cin>>rname;
            u->addRole(roles[rname]);
        }
        //打印出该用户具有的所有权限
        // map<string,char>& mp = u->pri;
        // map<string,char>::iterator pos = mp.begin();
        // cout<<uname<<" ";
        // for(; pos != mp.end(); ++pos) cout<<pos->first<<":"<<pos->second<<" ";
        // cout<<endl;
    }

    scanf("%d",&n);
    for(int i=0; i<n; ++i) //读取n个查询, 并输出结果
    {
        cin>>uname>>str;
        int p = str.find(':');
        pname = str.substr(0, p);
        lv = p < 0 ?  "-" : str.substr(p+1) ;
        // cout<<"query : "<<uname<<' '<<pname << ' '<<lv<<endl;
        
        User * u = users[uname];
        if(u) cout<< u->query( pname, lv[0] )<<endl;
        else cout<<"false"<<endl;
    }

    return 0;
}
