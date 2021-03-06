/*
试题编号： 201803-2 
试题名称： 碰撞的小球 
时间限制： 1.0s 
内存限制： 256.0MB 
问题描述： 问题描述
　　数轴上有一条长度为L（L为偶数)的线段，左端点在原点，右端点在坐标L处。有n个不计体积的小球在线段上，开始时所有的小球都处在偶数坐标上，速度方向向右，速度大小为1单位长度每秒。
　　当小球到达线段的端点（左端点或右端点）的时候，会立即向相反的方向移动，速度大小仍然为原来大小。
　　当两个小球撞到一起的时候，两个小球会分别向与自己原来移动的方向相反的方向，以原来的速度大小继续移动。
　　现在，告诉你线段的长度L，小球数量n，以及n个小球的初始位置，请你计算t秒之后，各个小球的位置。

提示
　　因为所有小球的初始位置都为偶数，而且线段的长度为偶数，可以证明，不会有三个小球同时相撞，小球到达线段端点以及小球之间的碰撞时刻均为整数。
　　同时也可以证明两个小球发生碰撞的位置一定是整数（但不一定是偶数）。

输入格式
　　输入的第一行包含三个整数n, L, t，用空格分隔，分别表示小球的个数、线段长度和你需要计算t秒之后小球的位置。
　　第二行包含n个整数a1, a2, …, an，用空格分隔，表示初始时刻n个小球的位置。

输出格式
　　输出一行包含n个整数，用空格分隔，第i个整数代表初始时刻位于ai的小球，在t秒之后的位置。

样例输入
3 10 5
4 6 8

样例输出
7 9 9

样例说明
　　初始时，三个小球的位置分别为4, 6, 8。

　　一秒后，三个小球的位置分别为5, 7, 9。

　　两秒后，第三个小球碰到墙壁，速度反向，三个小球位置分别为6, 8, 10。

　　三秒后，第二个小球与第三个小球在位置9发生碰撞，速度反向（注意碰撞位置不一定为偶数），三个小球位置分别为7, 9, 9。

　　四秒后，第一个小球与第二个小球在位置8发生碰撞，速度反向，第三个小球碰到墙壁，速度反向，三个小球位置分别为8, 8, 10。

　　五秒后，三个小球的位置分别为7, 9, 9。

样例输入
10 22 30
14 12 16 6 10 2 8 20 18 4

样例输出
6 6 8 2 4 0 4 12 10 2

数据规模和约定
　　对于所有评测用例，1 ≤ n ≤ 100，1 ≤ t ≤ 100，2 ≤ L ≤ 1000，0 < ai < L。L为偶数。
　　保证所有小球的初始位置互不相同且均为偶数。 




解题思路:
根据LRJ书上蚂蚁在木棍上爬行的那个思路写的:
当A和B相撞时,可以看做A穿透了B, 各自还是朝着原来的方向移动,仅当到达两端时才转向. 就是可以等效地把所有的球都看成是
相互独立的个体, 不受其他球的影响, 这种等效只是最终位置的等效, 在实际碰撞过程中, 每个球的前后顺序都始终保持不变,因为
每个球都不会越过与之相邻的球.

那么如何将每个球最初的位置与其最终的位置对应呢, 这个其实很简单, 我们只需要对球按照初始位置进行排序.
在计算出每个球的最终位置后, 将这些位置按照先后顺序进行排序. 那么最初排在最前面的球, 其最终位置就是
计算出的排在第一的那个位置, 同理, 最初排在第二个位置的球, 其最终位置也是计算出来的排序后的第二个位置.

接下来讨论如何计算一个球在x位置, 经过t时间后, 在长度为L的线段上的位置.(球在一个单位时间里移动一个单位的距离,初始时向右移动)
经过观察可以发现一个规律, 就是如果t==L, 则t时间后, x的位置变为L-x, 且移动方向向左
如果t==2L, 则t时间后,x又回到了原位置, 且移动方向向右, 根据这两条规律, 即可计算任何t时间后球的位置.
1.设 c = t / L, d = t % L;
    如果c为偶数, 则球相当于直接从原位置向右移动d个单位,但是要考虑x+d是否超过了L.
        如果x+d<=L, 则球的最终位置为x+d, 否则为L - (x + d - L).
    如果c为奇数, 则球相当于直接从原位置向左移动d个单位,但时要考虑是否会超过左端点.
        如果x-d>=0, 则球的最终位置为x-d, 否则为d-x.

*/


#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn = 105;

struct Ball
{
    int i, pos, j;
}a[maxn];

int b[maxn];

bool sort_by_i(const Ball& a, const Ball& b){ return a.i < b.i; }
bool sort_by_pos(const Ball& a, const  Ball& b){ return a.pos < b.pos; }


//x:求的初始位置, L:线段长度, t:时间跨度
int calculate(int x, int L, int t)
{
    int c = t / L, d = t % L;
    x = c & 1 ?  L - x : x ;
    if( c & 1 ) return x - d >=0 ? x - d : d - x;
    else return d + x <= L ? d + x : L - (x + d - L);
}


int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int n, L, t;
    scanf("%d%d%d",&n,&L,&t);
    for(int i=0; i<n; ++i)
    {
        a[i].i = i;
        scanf("%d", &a[i].pos);
    }
    sort(a, a + n, sort_by_pos );

    for(int i=0; i<n; ++i) a[i].j = i;

    for(int i=0; i<n; ++i) b[i] = calculate(a[i].pos, L, t);

    sort(b, b + n);

    sort(a, a+n, sort_by_i);
    
    for(int i=0; i<n; ++i) printf("%d ", b[ a[i].j ]);

    return 0;
}