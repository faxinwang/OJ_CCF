/*
试题编号：	201803-4
试题名称：	棋局评估
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　Alice和Bob正在玩井字棋游戏。
　　井字棋游戏的规则很简单：两人轮流往3*3的棋盘中放棋子，Alice放的是“X”，Bob放的是“O”，Alice执先。当同一种棋子占据一行、
    一列或一条对角线的三个格子时，游戏结束，该种棋子的持有者获胜。当棋盘被填满的时候，游戏结束，双方平手。
　　Alice设计了一种对棋局评分的方法：
　　- 对于Alice已经获胜的局面，评估得分为(棋盘上的空格子数+1)；
　　- 对于Bob已经获胜的局面，评估得分为 -(棋盘上的空格子数+1)；
　　- 对于平局的局面，评估得分为0；
    1  2  1
    2  1  2
    1  0  0

　　例如上图中的局面，Alice已经获胜，同时棋盘上有2个空格，所以局面得分为2+1=3。
　　由于Alice并不喜欢计算，所以他请教擅长编程的你，如果两人都以最优策略行棋，那么当前局面的最终得分会是多少？

输入格式
　　输入的第一行包含一个正整数T，表示数据的组数。
　　每组数据输入有3行，每行有3个整数，用空格分隔，分别表示棋盘每个格子的状态。0表示格子为空，1表示格子中为“X”，2表示格子中为“O”。保证不会出现其他状态。
　　保证输入的局面合法。(即保证输入的局面可以通过行棋到达，且保证没有双方同时获胜的情况)
　　保证输入的局面轮到Alice行棋。

输出格式
　　对于每组数据，输出一行一个整数，表示当前局面的得分。

样例输入
3
1 2 1
2 1 2
0 0 0
2 1 1
0 2 1
0 0 2
0 0 0
0 0 0
0 0 0

样例输出
3
-4
0

样例说明
　　第一组数据：
　　Alice将棋子放在左下角(或右下角)后，可以到达问题描述中的局面，得分为3。
　　3为Alice行棋后能到达的局面中得分的最大值。
　　第二组数据：
    2 1 1
    0 2 1
    0 0 2
    
　　Bob已经获胜(如图)，此局面得分为-(3+1)=-4。
　　第三组数据：
　　井字棋中若双方都采用最优策略，游戏平局，最终得分为0。
数据规模和约定
　　对于所有评测用例，1 ≤ T ≤ 5。

解题思路:
参考: https://blog.csdn.net/xbb224007/article/details/79935167

1.Alice获胜时得到分是正分，Bob获胜时得到的分是负分，由于两个人都非常聪明，所以在任意局面下：
    1.若当前步是Alice下，那么Alice会使自己可以得到的分数最大：
    2.若当前步是Bob下，那么Bob会使自己可以得到的分数最大，负分表示就是最小；
2.根据以上的得分策略，枚举所有的下棋情况，具体来说：
    1.设DFS(i)为当前局面下，第i(i==1或2)个人下的时候，该人能获得的"最大值"；
    2.若当前步是Alice下，那么遍历Alice能下的所有位置，返回后续所有结果中最终局面的最大值；
    3.若当前步时Bob下，那么遍历Bob能下的所有位置，返回后续所有结果中最终局面的最小值；
    4.这里数据比较小，最多也就9！次，直接搜索即可，当数据比较大时，应该用到alpha-beta剪枝。
*/
#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;

int a[4][4];

//判断第r行或者第c列是否是三个type类型的棋子
bool checkRow(int r, int type){ return a[r][1] == a[r][2] && a[r][2] == a[r][3] && a[r][3] == type; }
bool checkCol(int c, int type){ return a[1][c] == a[2][c] && a[2][c] == a[3][c] && a[3][c] == type; }

//计算空格的个数
int space()
{
    int cnt=0;
    for(int i=1; i<=3; ++i)
        for(int j=1; j<=3; ++j)
            if(a[i][j] == 0) ++cnt;
    return cnt;
}

//检测棋子type(1或2)的局面是否已经获胜, 如果获胜, 返回得分
int checkWin(int type)
{
    int spa = space();
    int score = type == 1 ? spa + 1 : - spa - 1;
    for(int i=1; i<=3; ++i) if( checkRow(i, type) ) return score;
    for(int i=1; i<=3; ++i) if( checkCol(i, type) ) return score;
    if(a[1][1] == a[2][2] && a[2][2] == a[3][3] && a[3][3] == type) return score;
    if(a[1][3] == a[2][2] && a[2][2] == a[3][1] && a[3][1] == type) return score;
    return 0;
}

//返回当前局面下, 先行type类型的棋子, 所能获得的最好得分
int dfs(int type)
{
    int spa =space();
    if(spa == 0 || spa == 9) return 0;
    int Max = -10, Min = 10;
    for(int i=1; i<=3; ++i)
    {
        for(int j=1; j<=3; ++j)
        {
            if(!a[i][j])
            {
                a[i][j] = type;
                int score = checkWin(type);
                if(score)
                {
                    a[i][j] = 0;
                    return score;
                }
                type == 1 ? Max = max(Max, dfs(2) ) : Min = min(Min, dfs(1) ); //如果当前下1, 则下一步下2
                a[i][j] = 0; //回溯
            }
        }
    }
    return type == 1 ? Max : Min; //如果这次下的是1,则返回最Alice的得分, 否则返回Bob的得分
}


int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int T;
    scanf("%d",&T);
    while(T--)
    {
        for(int i=1; i<=3; ++i)
            for(int j=1; j<=3; ++j)
                scanf("%d", &a[i][j]);

        int x = checkWin(1), y = checkWin(2);
        if(x){ cout<<x<<endl; continue; }
        if(y){ cout<<y<<endl; continue; }
        cout<<dfs(1)<<endl; //题目已经保证局面一定轮到Alice行棋
    }

    return 0;
}
