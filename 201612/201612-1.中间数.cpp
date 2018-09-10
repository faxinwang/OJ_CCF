/*
试题编号：	201612-1
试题名称：	中间数
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
　　在一个整数序列a1, a2, …, an中，如果存在某个数，大于它的整数数量等于小于它的整数数量，则称其为中间数。在一个序列中，
    可能存在多个下标不相同的中间数，这些中间数的值是相同的。
　　给定一个整数序列，请找出这个整数序列的中间数的值。
输入格式
　　输入的第一行包含了一个整数n，表示整数序列中数的个数。
　　第二行包含n个正整数，依次表示a1, a2, …, an。
输出格式
　　如果约定序列的中间数存在，则输出中间数的值，否则输出-1表示不存在中间数。
样例输入
6
2 6 5 6 3 5

样例输出
5

样例说明
　　比5小的数有2个，比5大的数也有2个。

样例输入
4
3 4 6 7

样例输出
-1

样例说明
　　在序列中的4个数都不满足中间数的定义。

样例输入
5
3 4 6 6 7

样例输出
-1

样例说明
　　在序列中的5个数都不满足中间数的定义。
评测用例规模与约定
　　对于所有评测用例，1 ≤ n ≤ 1000，1 ≤ ai ≤ 1000。

解题思路:
对数据排序之后, 只需要判断中间相同的数字的个数以对称的形式出现,如果是对称的, 则中间数就是中间这个数字,
否则没有中间数. 需要对n为奇数和偶数分别进行判断.
由于数据量交小, 直接暴力应该也是可以通过的.

*/

#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
const int maxn = 1e3+5;

int a[maxn];

int main()
{
    #ifdef WFX
    freopen("in.txt","r",stdin);
    #endif
    int n;
    scanf("%d",&n);
    for(int i=0; i<n; ++i) scanf("%d",a+i);
    sort(a,a+n);
    int mid= n / 2;
    if(n & 1)
    {
        int i = mid-1, j = mid+1;
        while(i>= 0 && a[i] == a[mid]) --i;
        while(j < n && a[j] == a[mid]) ++j;
        printf("%d\n", (mid - i == j - mid)? a[mid] : -1);
    }
    else
    {
        int i=mid-1, j=mid;
        if(a[i] != a[j]) printf("-1\n");
        else
        {
            while(i>= 0 && a[i] == a[mid]) --i;
            while(j < n && a[j] == a[mid]) ++j;
            printf("%d\n", (mid - i - 1 == j - mid)? a[mid] : -1);
        }
    }

    return 0;
}