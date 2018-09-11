/*
试题编号：	201609-4
试题名称：	交通规划
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	

　　G国国王来中国参观后，被中国的高速铁路深深的震撼，决定为自己的国家也建设一个高速铁路系统。
　　建设高速铁路投入非常大，为了节约建设成本，G国国王决定不新建铁路，而是将已有的铁路改造成高速铁路。现在，请你为G国国王提
    供一个方案，将现有的一部分铁路改造成高速铁路，使得任何两个城市间都可以通过高速铁路到达，而且从所有城市乘坐高速铁路到首
    都的最短路程和原来一样长。请你告诉G国国王在这些条件下最少要改造多长的铁路。

输入格式
　　输入的第一行包含两个整数n, m，分别表示G国城市的数量和城市间铁路的数量。所有的城市由1到n编号，首都为1号。
　　接下来m行，每行三个整数a, b, c，表示城市a和城市b之间有一条长度为c的双向铁路。这条铁路不会经过a和b以外的城市。

输出格式
　　输出一行，表示在满足条件的情况下最少要改造的铁路长度。

样例输入
4 5
1 2 4
1 3 5
2 3 2
2 4 3
3 4 2

样例输出
11

评测用例规模与约定
　　对于20%的评测用例，1 ≤ n ≤ 10，1 ≤ m ≤ 50；
　　对于50%的评测用例，1 ≤ n ≤ 100，1 ≤ m ≤ 5000；
　　对于80%的评测用例，1 ≤ n ≤ 1000，1 ≤ m ≤ 50000；
　　对于100%的评测用例，1 ≤ n ≤ 10000，1 ≤ m ≤ 100000，1 ≤ a, b ≤ n，1 ≤ c ≤ 1000。输入保证每个城市都可以通过铁路达到首都。


解题思路:
"从所有城市乘坐高速铁路到首都的最短路程和原来一样长"说明答案要满足源点是1的单源最短路径, 
"最少要改造多长的铁路"说明在单源最短路径的前提下要满足总花费最小

在求解最短路径的过程中, 出来用d[i]记录i到源点的最短距离外, 还用c[i]记录从源点到i的最短路径的最后一条边的长度
例如在用i的某个相邻结点u更新d[i]的过程中, 
如果d[u] + dist[u][i] < d[i], 则执行 d[i] = d[u] + dist[u][i] 和 c[i] = dist[u][i]; 
如果d[u] + dist[u][i] = d[i], 则执行 c[i] = min( c[i], dist[u][i]); 
*/

#include<iostream>
#include<cstdio>
#include<queue>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;
const int maxn = 1e4+5;
const int maxm = 1e5+5;
const int INF = 1e8;
typedef long long LL;

int N,M;
struct Edge
{
    int to, cost;
    Edge(int t, int c):to(t),cost(c){}
    bool operator>(const Edge& e)const  {return cost > e.cost; }
};

vector<Edge> g[maxn];
int c[maxn], d[maxn];
priority_queue<Edge, deque<Edge>, greater<Edge> > Q;

void opt_dijkstra() //使用堆优化的dijkstra算法
{
    for(int i=1; i<=N; ++i) d[i] = INF;
    d[1] = 0;
    Q.push(Edge(1, 0));
    while(!Q.empty())
    {
        Edge e = Q.top(); Q.pop();
        int u = e.to;
        if(d[u] < e.cost) continue;
        for(int i=0,n=g[u].size(); i<n; ++i)
        {
            e = g[u][i];
            int v = e.to;
            if(d[u] + e.cost < d[v])
            {
                d[v] = d[u] + e.cost;
                c[v] = e.cost; //记录从源点到i的最短路径的最后一条边的长度
                Q.push(Edge(v, d[v]));
            }
            else if(d[u] + e.cost  == d[v]) c[v] = min(c[v], e.cost); //记录从源点到i的最短路径的最后一条边的长度,取最小值
        }
    }
}

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int u, v, w;
    scanf("%d%d",&N, &M);
    for(int i=0; i<M; ++i)
    {
        scanf("%d%d%d",&u,&v,&w);
        g[u].push_back(Edge(v,w));
        g[v].push_back(Edge(u,w));
    }

    opt_dijkstra();
    LL ans = 0;
    for(int i=2; i<=N; ++i) ans += c[i];
    printf("%lld\n", ans);

    return 0;
}