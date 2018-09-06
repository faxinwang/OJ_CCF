/*
试题编号：	201712-4
试题名称：	行车路线
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　小明和小芳出去乡村玩，小明负责开车，小芳来导航。
　　小芳将可能的道路分为大道和小道。大道比较好走，每走1公里小明会增加1的疲劳度。小道不好走，如果连续走小道，小明的疲
    劳值会快速增加，连续走s公里小明会增加s2的疲劳度。例如：有5个路口，1号路口到2号路口为小道，2号路口到3号路口为小道，
    3号路口到4号路口为大道，4号路口到5号路口为小道，相邻路口之间的距离都是2公里。如果小明从1号路口到5号路口，则总疲劳
    值为(2+2)2+2+22=16+2+4=22。
　　现在小芳拿到了地图，请帮助她规划一个开车的路线，使得按这个路线开车小明的疲劳度最小。

输入格式
　　输入的第一行包含两个整数n, m，分别表示路口的数量和道路的数量。路口由1至n编号，小明需要开车从1号路口到n号路口。
　　接下来m行描述道路，每行包含四个整数t, a, b, c，表示一条类型为t，连接a与b两个路口，长度为c公里的双向道路。其中t为0表
    示大道，t为1表示小道。保证1号路口和n号路口是连通的。
    
输出格式
　　输出一个整数，表示最优路线下小明的疲劳度。

样例输入
6 7
1 1 2 3
1 2 3 2
0 1 3 30
0 3 4 20
0 4 5 30
1 3 5 6
1 5 6 1

样例输出
76

样例说明
　　从1走小道到2，再走小道到3，疲劳度为52=25；然后从3走大道经过4到达5，疲劳度为20+30=50；最后从5走小道到6，疲劳度为1。总共为76。

数据规模和约定
　　对于30%的评测用例，1 ≤ n ≤ 8，1 ≤ m ≤ 10；
　　对于另外20%的评测用例，不存在小道；
　　对于另外20%的评测用例，所有的小道不相交；
　　对于所有评测用例，1 ≤ n ≤ 500，1 ≤ m ≤ 10^5，1 ≤ a, b ≤ n，t是0或1，c ≤ 10^5。保证答案不超过10^6。


解题思路:
思路来自博客: https://blog.csdn.net/da_kao_la/article/details/81773485

Floyd + SPFA 求解最短路变形题。

问题的难点在于连续走小路a，b时，产生的疲劳值是 (a+b)^2 而不是 a^2 + b^2.

解决思路是把大路和小路分开在两张图考虑。由于小路的疲劳值是连续计算再平方的，所以首先用一个Floyd算法计算
小路图的点对点最短路。如果直接用最普通的4行Floyd法O(n3)会超时，可以利用小路图矩阵的对称性优化一下常数，就不会超时了。

计算最短路的时候，Dijkstra算法及其变形（堆优化）还是Bellman-Ford算法及其变形（SPFA），都有一个d数组表
示每个点到源的距离。此时因为走完大路可以再走大路或小路，但走完小路只能走大路（连续走小路的情形已被Floyd算
法过程归化），因此要开两个数组db和ds分别表示最后一步走大路/小路时各点到源的距离，并按照上述原则在松弛操作
中更新db和ds. 由于每个点i到源对应有两个距离db[i]和ds[i]，db[i]成为全局最小后ds[i]仍有可能被更新变成全局
最小，同理ds[i]成为全局最小后db[i]仍有可能被更新得到全局最小，因此基于全局最小距离进行松弛操作更新d数组的
Dijkstra算法要修改适应本问题就比较困难。而Bellman-Ford算法及SPFA只用了相邻节点之间的路径信息，并不需要求
全局最小，就可以适用于本问题。

坑点：

1. 虽然题目说了“保证答案不超过10^6”，但是中间过程可能会超过int范围，因此与距离有关的量都要用long long
2. 输入有重边

最后, 如果只考虑所有小路不相连的情况, 使用把疲劳值当做边权, 小路的边权设为疲劳值的平方, 然后求从1到N的最单
距离, 这样依然可以获得60分

*/


#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
#include<algorithm>
using namespace std;
typedef long long LL;
const int maxn = 505;
const LL INF = 1e9;

bool vis[maxn]; //用于记录SPFA算法中那些结点在队列中
LL db[maxn]; //db[i] 保存从源点到结点i,最后一步走大路时,i与源点之间的距离
LL ds[maxn]; //db[i] 保存从源点到结点i,最后一步走小路时,i与源点之间的距离
LL big[maxn][maxn]; //保存由小路构成的图
LL sml[maxn][maxn]; //保存由大路构成的图
int N,M;

void SPFA()
{
    queue<LL> Q;
    for(int i=1; i<=N; ++i) db[i] = ds[i] = INF;
    memset(vis, 0 , sizeof(vis));
    db[1] = ds[1] = 0;
    Q.push(1);
    vis[1] = 1;
    while(!Q.empty())
    {
        int u = Q.front(); Q.pop();
        vis[u] = 0;
        for(int i=1; i<=N; ++i)
        {
            bool updated = false;
            if(big[u][i] < INF) //有从u到i的大路,用这条大路更新db[i],可考虑从db[u]或者ds[u]而来
            {
                if(db[u] + big[u][i] < db[i] || ds[u] + big[u][i] < db[i] )
                {
                    db[i] = min(db[u], ds[u]) + big[u][i];
                    updated = true;
                }
            }
            
            if(sml[u][i] < INF) //有从u到i的小路, 用这条小路更新ds[i], 只能从db[u]而来
            {
                LL cost = sml[u][i] * sml[u][i];
                if(db[u] + cost < ds[i])
                {
                    ds[i] = db[u] + cost;
                    updated = true;
                }
            }
            
            if(updated && !vis[i])
            {
                vis[i] = 1;
                Q.push(i);
            }
        }
    }
}

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    LL t, u,v,w;
    scanf("%lld%lld",&N,&M);
    for(int i=1; i<=N; ++i) 
        for(int j=i; j<=N; ++j) 
            big[i][j] = big[j][i] = sml[i][j] = sml[j][i] = INF;
    
    for(int i=0; i<M; ++i)
    {
        scanf("%lld%lld%lld%lld",&t,&u,&v,&w);
        if(t) sml[u][v] = sml[v][u] = min( sml[u][v], w);
        else  big[u][v] = big[v][u] = min( big[u][v], w);
    }

    for(int i=1; i<=N; ++i)
        for(int j=i+1; j<=N; ++j) //这里根据对称性j直接从i+1开始, 如果j从1开始,只能得40分,剩下的测试点会超时
            for(int k=1; k<=N; ++k)
                if(k!=i && k!=j) sml[i][j] = min(sml[i][j], sml[i][k] + sml[k][j]);

    SPFA();
    printf("%d\n", min(db[N], ds[N]) );

    return 0;
}