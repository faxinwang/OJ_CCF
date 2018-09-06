/*
试题编号：	201712-2
试题名称：	游戏
时间限制：	1.0s
内存限制：	256.0MB
问题描述：	
问题描述
　　有n个小朋友围成一圈玩游戏，小朋友从1至n编号，2号小朋友坐在1号小朋友的顺时针方向，3号小朋友坐在2号小朋友的顺时针方向，
    ……，1号小朋友坐在n号小朋友的顺时针方向。游戏开始，从1号小朋友开始顺时针报数，接下来每个小朋友的报数是上一个小朋友报
    的数加1。若一个小朋友报的数为k的倍数或其末位数（即数的个位）为k，则该小朋友被淘汰出局，不再参加以后的报数。当游戏中
    只剩下一个小朋友时，该小朋友获胜。
　　例如，当n=5, k=2时：
　　1号小朋友报数1；
　　2号小朋友报数2淘汰；
　　3号小朋友报数3；
　　4号小朋友报数4淘汰；
　　5号小朋友报数5；
　　1号小朋友报数6淘汰；
　　3号小朋友报数7；
　　5号小朋友报数8淘汰；
　　3号小朋友获胜。

　　给定n和k，请问最后获胜的小朋友编号为多少？
输入格式
　　输入一行，包括两个整数n和k，意义如题目所述。
输出格式
　　输出一行，包含一个整数，表示获胜的小朋友编号。
样例输入
5 2
样例输出
3
样例输入
7 3
样例输出
4
数据规模和约定
　　对于所有评测用例，1 ≤ n ≤ 1000，1 ≤ k ≤ 9。

解题思路:
用链表进行模拟: 用整型变量curNum表示当前要报的数字, 用迭代器pos跟随curNum的步伐, curNum每加一,
pos就往后移动一步, 如果移动到了链表末尾, 则转到begin()上去. 在curNum增加的过程中, 判断curNum这个数字
对应的人是否会被淘汰, 如果是, 就将此时的pos对应位置的编号删除掉,同时pos移动到下一个报数的位置上.
*/

#include<iostream>
#include<cstdio>
#include<list>
using namespace std;

int main()
{
#ifdef WFX
freopen("in.txt","r",stdin);
#endif
    int n,k,curNum=1;
    scanf("%d%d",&n,&k);
    list<int> L;
    for(int i=1; i<=n; ++i) L.push_back(i);

    list<int>::iterator pos = L.begin();
    while(L.size() > 1)
    {
        if(curNum % k == 0 || curNum % 10 == k) pos = L.erase(pos);
        else ++pos;
        if(pos == L.end()) pos = L.begin();
        ++curNum;
    }
    printf("%d\n", *L.begin());

    return 0;
}