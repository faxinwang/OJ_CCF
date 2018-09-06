/*
试题编号：	201712-1
试题名称：	最小差值
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
问题描述
　　给定n个数，请找出其中相差（差的绝对值）最小的两个数，输出它们的差值的绝对值。
输入格式
　　输入第一行包含一个整数n。
　　第二行包含n个正整数，相邻整数之间使用一个空格分隔。
输出格式
　　输出一个整数，表示答案。
样例输入
5
1 5 4 8 20
样例输出
1
样例说明
　　相差最小的两个数是5和4，它们之间的差值是1。
样例输入
5
9 3 6 1 3
样例输出
0
样例说明
　　有两个相同的数3，它们之间的差值是0.
数据规模和约定
　　对于所有评测用例，2 ≤ n ≤ 1000，每个给定的整数都是不超过10000的正整数。

解题思路:
对输入数据排序之后,找出相连两个数之间最小的差值就是答案

*/
#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn = 1005;

int a[maxn];

int main()
{
#ifdef WFX
freopen("in.txt","r",stdin);
#endif
    int n, minGap=10000;
    scanf("%d",&n);
    for(int i=0; i<n; ++i) scanf("%d",&a[i]);
    sort(a, a+n);
    for(int i=1; i<n; ++i) minGap = min(minGap, a[i] - a[i-1]);
    printf("%d\n",minGap);

    return 0;
}