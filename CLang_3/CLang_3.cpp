/// Queue
#include <iostream>
#define MAX 10
int   g_iQuqueArray[MAX];
int   g_iIn = 0;
int   g_iOut = 0;

void  push(int t)
{
    // 항상 1개의 방은 나머지가 존재한다.
    if ((g_iIn+1) % MAX == g_iOut)
    {
        std::cout << "Queue Overflow!\n";
        return;
    }
    g_iQuqueArray[g_iIn] = t;
    g_iIn = ++g_iIn % MAX;
}
int   pop()
{
    if (g_iIn == g_iOut)
    {
        std::cout << "Stack Underflow!\n";
        return -1;
    }
    int iData = g_iQuqueArray[g_iOut];
    g_iOut = ++g_iOut % MAX;
    return iData;
}
int main()
{
    for (int i = 0; i < MAX-1; i++)
    {
        push(i);
    }
    push(10);//"Stack Overflow!

    for (int i = 0; i < MAX-1; i++)
    {
        int iValue = pop();
        std::cout << iValue << " ";
    }
    pop();//"Stack Underflow!

    std::cout << "Hello World!\n";
}
