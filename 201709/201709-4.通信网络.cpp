/*
试题编号：	201709-4
试题名称：	通信网络
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　某国的军队由N个部门组成，为了提高安全性，部门之间建立了M条通路，每条通路只能单向传递信息，即一条从部门a到部
    门b的通路只能由a向b传递信息。信息可以通过中转的方式进行传递，即如果a能将信息传递到b，b又能将信息传递到c，
    则a能将信息传递到c。一条信息可能通过多次中转最终到达目的地。
　　由于保密工作做得很好，并不是所有部门之间都互相知道彼此的存在。只有当两个部门之间可以直接或间接传递信息时，
    他们才彼此知道对方的存在。部门之间不会把自己知道哪些部门告诉其他部门。

　　上图中给了一个4个部门的例子，图中的单向边表示通路。部门1可以将消息发送给所有部门，部门4可以接收所有部门的消息，
    所以部门1和部门4知道所有其他部门的存在。部门2和部门3之间没有任何方式可以发送消息，所以部门2和部门3互相不知道彼此的存在。
　　现在请问，有多少个部门知道所有N个部门的存在。或者说，有多少个部门所知道的部门数量（包括自己）正好是N。

输入格式
　　输入的第一行包含两个整数N, M，分别表示部门的数量和单向通路的数量。所有部门从1到N标号。
　　接下来M行，每行两个整数a, b，表示部门a到部门b有一条单向通路。

输出格式
　　输出一行，包含一个整数，表示答案。

样例输入
4 4
1 2
1 3
2 4
3 4

样例输出
2

样例说明
　　部门1和部门4知道所有其他部门的存在。

评测用例规模与约定
　　对于30%的评测用例，1 ≤ N ≤ 10，1 ≤ M ≤ 20；
　　对于60%的评测用例，1 ≤ N ≤ 100，1 ≤ M ≤ 1000；
　　对于100%的评测用例，1 ≤ N ≤ 1000，1 ≤ M ≤ 10000。


解题思路:
1. 某个点知道的结点包括所有能到达它的点和所有它能到达的点再加上它自己.
2. 从结点i出发用DFS遍历正向图, 找出所有i能到达的点, 时间复杂度O(N)
3. 从结点i出发用DFS遍历方向图, 找出所有能到达i的点, 时间复杂度O(N)
4. 统计i知道的点的数量, 如果数量为N,则++ans, 时间复杂度O(N)
5. 对所有i个点都进行上述操作, 即可得到最终的ans, 时间复杂度O(N^2), N<=1000, 1s内可以搞定, 实际上,下面的代码实现运行时间只有171ms
*/

#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
using namespace std;
const int maxn = 1005;


vector<int> g1[maxn], g2[maxn]; //分别保存正向图, 反向图
bool mat[maxn][maxn]; //mat[i][j]=1 表示i知道j的存在,等于0表示i不知道j的存在
bool vis[maxn];
int N,M;

void DFS(vector<int> g[maxn], int u, int s)
{
    for(int i=0,n=g[u].size(); i<n; ++i)
    {
        if(!vis[ g[u][i] ])
        {
            vis[g[u][i]] = 1;
            DFS(g, g[u][i], s);
        }
    }
    mat[s][u] = 1; //从s能到达u, s知道u的存在
}

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int u,v;
    scanf("%d%d",&N,&M);
    for(int i=0; i<M; ++i)
    {
        scanf("%d%d",&u,&v);
        g1[u].push_back(v); //正向图
        g2[v].push_back(u); //反向图
    }
    
    for(int i=1; i<=N; ++i) //标记i能到达的点
    {
        memset(vis, 0, sizeof(vis));
        DFS(g1, i, i);
    }
    
    for(int i=1; i<=N; ++i) //标记能到达i的点
    {
        memset(vis, 0, sizeof(vis));
        DFS(g2, i, i);
    }
    
    int ans = 0;
    for(int i=1; i<=N; ++i) //统计i知道的点
    {
        int cnt = 0;
        for(int j=1; j<=N; ++j) if(mat[i][j]) ++cnt;
        if(cnt == N) ++ans;
    }
    cout<<ans<<endl;

    return 0;
}