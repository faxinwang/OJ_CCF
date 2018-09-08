/*
试题编号：	201709-5
试题名称：	除法
时间限制：	10.0s
内存限制：	256.0MB
问题描述：	
　　小葱喜欢除法，所以他给了你N个数a1, a2, ⋯, aN，并且希望你执行M次操作，每次操作可能有以下两种：
　　给你三个数l, r, v，你需要将al, al+1, ⋯, ar之间所有v的倍数除以v。
　　给你两个数l, r，你需要回答al + al+1 + ⋯ + ar的值是多少。
输入格式
　　第一行两个整数N, M，代表数的个数和操作的次数。
　　接下来一行N个整数，代表N个数一开始的值。
　　接下来M行，每行代表依次操作。每行开始有一个整数opt。如果opt=1，那么接下来有三个数l, r, v，代表这次
    操作需要将第l个数到第r个数中v的倍数除以v；如果opt = 2，那么接下来有两个数l, r，代表你需要回答第l个
    数到第r个数的和。
输出格式
　　对于每一次的第二种操作，输出一行代表这次操作所询问的值。
样例输入
5 3
1 2 3 4 5
2 1 5
1 1 3 2
2 1 5
样例输出
15
14
评测用例规模与约定
　　对于30%的评测用例，1 ≤ N, M ≤ 1000；
　　对于另外20%的评测用例，第一种操作中一定有l = r；
　　对于另外20%的评测用例，第一种操作中一定有l = 1 , r = N；
　　对于100%的评测用例，1 ≤ N, M ≤ 105，0 ≤ a1, a2, ⋯, aN ≤ 106, 1 ≤ v ≤ 106, 1 ≤ l ≤ r ≤ N。

解题思路:
一看完题目就想到了树状数组, 写完提交之后, 只有50分.

然后想了下, 用线段树试试, 因为树状数组每次更新一个值, 都需要O(logN)的时间,M次就需要O(MlogN)的时间,
线段树更新区间的时候,如果要对每个叶子结点进行处理, 其时间复杂度正比于区间的长度,为O(L), 其实也没有太大
的优势, 但就是想试一下, 结果提交之后, 还是只有50分.

树状数组试了, 线段树也试了, 都过不了, 以为需要用什么更快的数据结构, 于是搜了一些博客, 发现几乎都是用的
树状数组, 但是都做了一些优化, 下面是可以优化的地方:

1. 当v==1的时候, 是不需要进行更新处理的, 因为任何数除以1结果不变.
2. 把判断是否进行更新的条件if(b[i] % v==0) 改为 if(b[i] >= v && b[i] % v == 0).
   之所以可以这样优化,是因为如果b[i]==0的时候, 是可以直接跳过的, 因为进行大小判断比进行取模运算
   的速度要快很多, 如果这条语句执行次数非常多, 那么这条优化就会起到较好的效果.
3. 把不需要用long long 的地方改成int, 例如b[]数组, 然后循环变量和有些参数都可以不用long long,
   开始把b[]数组写成了long long, 加上了上述两条优化后, 依然超时, 所以int比long long的计算速度
   确实要快很多后来改成了int通过.
4. 把访问频率很高的变量使用register修饰, 例如循环变量, 还有add()函数里的参数v,每次循环都要用到,
   因此也使用了register修饰, 这个用的恰当可以加速2s左右

最后, 使用了上述的所有优化, 运行时间降到了5.68s, 速度提升非常可观. 

*/

#include<iostream>
#include<cstdio>
#define lowbit(x) (-x&x)
using namespace std;
const int maxn = 1e5+5;
typedef long long LL;

int N,M;
LL a[maxn];
int b[maxn];

LL sum(LL n)
{
    register LL s = 0;
    for(register int i=n; i>0; i -= lowbit(i) ) s += a[i];
    return s;
}

void add(int k, register int v){ for(register int i=k; i<=N; i += lowbit(i)) a[i] += v; }

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int L, R, v, op;
    scanf("%d%d",&N,&M);
    for(register int i=1; i <= N; ++i)
    {
        scanf("%lld", &b[i]);
        add(i, b[i]);
    }
	
    for(register int i=1; i<=M; ++i)
    {
        scanf("%d%d%d",&op, &L, &R);
        if(op == 2) printf("%lld\n", sum(R) - sum(L-1));
        else
        {
            scanf("%d", &v);
            if(v == 1) continue;
            for(register int j=L; j<=R; ++j) if( b[j] >= v && b[j] % v == 0)
            {
                add(j, b[j]/v - b[j]);
                b[j] /= v;
            }
        }
    }

    return 0;
}

