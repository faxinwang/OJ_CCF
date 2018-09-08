/*
试题编号：	201709-2
试题名称：	公共钥匙盒
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　有一个学校的老师共用N个教室，按照规定，所有的钥匙都必须放在公共钥匙盒里，老师不能带钥匙回家。每次老师上课前，都从公共钥匙
    盒里找到自己上课的教室的钥匙去开门，上完课后，再将钥匙放回到钥匙盒中。钥匙盒一共有N个挂钩，从左到右排成一排，用来挂N个教
    室的钥匙。一串钥匙没有固定的悬挂位置，但钥匙上有标识，所以老师们不会弄混钥匙。每次取钥匙的时候，老师们都会找到自己所需要
    的钥匙将其取走，而不会移动其他钥匙。每次还钥匙的时候，还钥匙的老师会找到最左边的空的挂钩，将钥匙挂在这个挂钩上。如果有多
    位老师还钥匙，则他们按钥匙编号从小到大的顺序还。如果同一时刻既有老师还钥匙又有老师取钥匙，则老师们会先将钥匙全还回去再取出。
　　今天开始的时候钥匙是按编号从小到大的顺序放在钥匙盒里的。有K位老师要上课，给出每位老师所需要的钥匙、开始上课的时间和上课的时长，
    假设下课时间就是还钥匙时间，请问最终钥匙盒里面钥匙的顺序是怎样的？

输入格式
　　输入的第一行包含两个整数N, K。
　　接下来K行，每行三个整数w, s, c，分别表示一位老师要使用的钥匙编号、开始上课的时间和上课的时长。可能有多位老师使用同一把钥匙，但是
    老师使用钥匙的时间不会重叠。保证输入数据满足输入格式，你不用检查数据合法性。

输出格式
　　输出一行，包含N个整数，相邻整数间用一个空格分隔，依次表示每个挂钩上挂的钥匙编号。

样例输入
5 2
4 3 3
2 2 7

样例输出
1 4 3 2 5

样例说明
　　第一位老师从时刻3开始使用4号教室的钥匙，使用3单位时间，所以在时刻6还钥匙。第二位老师从时刻2开始使用钥匙，使用7单位时间，所以在时刻9还钥匙。
　　每个关键时刻后的钥匙状态如下（X表示空）：
　　时刻2后为1X345；
　　时刻3后为1X3X5；
　　时刻6后为143X5；
　　时刻9后为14325。
样例输入
5 7
1 1 14
3 3 12
1 15 12
2 7 20
3 18 12
4 21 19
5 30 9
样例输出
1 2 3 5 4
评测用例规模与约定
　　对于30%的评测用例，1 ≤ N, K ≤ 10, 1 ≤ w ≤ N, 1 ≤ s, c ≤ 30；
　　对于60%的评测用例，1 ≤ N, K ≤ 50，1 ≤ w ≤ N，1 ≤ s ≤ 300，1 ≤ c ≤ 50；
　　对于所有评测用例，1 ≤ N, K ≤ 1000，1 ≤ w ≤ N，1 ≤ s ≤ 10000，1 ≤ c ≤ 100。


解题思路:
    总共有两种操作, 取钥匙和还钥匙. 将所有取钥匙和还钥匙的操作先按照时间排序, 当时间相同时, 先执行还钥匙的操作,
    再执行取钥匙的操作, 当时间和操作类型都相同时, 先执行钥匙编号较小的操作
    
    用map<int,int> k2p保存钥匙与位置的对应关系, set<int> freePos保存空位置, 按顺序执行排序后的操作
    如果当前操作是取钥匙, 则根据钥匙编号获取该钥匙的位置, 并将该位置号放入freeSet中,表示该位置空着了
    如果当前操作是还钥匙, 则从freePos中拿出编号最小的位置, 并在k2p中设置该钥匙的位置为从freePos中取得的最小编号.

    执行完所有的操作后, k2p中保存了每个钥匙对应的位置号, 但要输出的是1到N号位置上的钥匙号, 因此还需要进行一次钥匙
    要位置的信息提取.
*/
#include<iostream>
#include<cstdio>
#include<map>
#include<set>
#include<vector>
#include<algorithm>
using namespace std;
const int maxn = 1005;

struct GetRetKey
{
    bool op; //get key : 1 , reture key : 0
    int key;
    int time;
    GetRetKey(int key,int time, bool op):key(key), op(op), time(time){}
    bool operator<(const GetRetKey& grk)const
    { 
        if(time != grk.time) return time < grk.time;
        if(op != grk.op) return op < grk.op; //先还钥匙,再取钥匙
        return key < grk.key; //如果有多位老师还钥匙，则他们按钥匙编号从小到大的顺序还
    }
};

int a[maxn];

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int N, K;
    scanf("%d%d",&N,&K);
    vector<GetRetKey> v;
    v.reserve(K<<1);
    int key, start, cnt;
    for(int i=0; i<K; ++i)
    {
        scanf("%d%d%d", &key, &start, &cnt);
        v.push_back( GetRetKey(key, start, 1) ); //借钥匙
        v.push_back( GetRetKey(key, start + cnt, 0 ) ); //还钥匙
    }

    sort(v.begin(), v.end());

    map<int,int> k2p; //key to pos
    set<int> freePos;
    for(int i=1; i<=N; ++i) k2p[i] = i; //初始时, 第i号钥匙放在第i个位置

    for(int i=0,n= K<<1; i<n; ++i )
    {
        int key = v[i].key;
        if(v[i].op) // 借钥匙
        {
            int pos = k2p[key];
            // k2p[key] = 0;
            freePos.insert(pos);
        }
        else //还钥匙
        {
            int pos = *freePos.begin();
            freePos.erase(freePos.begin());
            k2p[key] = pos;
        }
    }

    map<int,int>::iterator it = k2p.begin(), end =k2p.end();
    for(; it != end; ++it) a[ it->second ] = it->first; //second是位置号, first是钥匙号
    for(int i=1; i<=N; ++i) printf("%d ", a[i]);

    return 0;
}