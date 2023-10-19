/// Stack
#include <iostream>
#define MAX 3
int   g_iStackAray[MAX];
int   g_iTop =-1;

void  push(int t)
{
    if (g_iTop >= MAX - 1)
    {
        std::cout << "Stack Overflow!\n";
        return;
    }
    g_iStackAray[++g_iTop] = t;
}
int   pop()
{
    if (g_iTop < 0)
    {
        std::cout << "Stack Underflow!\n";
        return -1;
    }
    return g_iStackAray[g_iTop--];
}
int main()
{
    for (int i = 0; i < MAX; i++)
    {
        push(i);
    }
    push(10);//"Stack Overflow!

    for (int i = 0; i < MAX; i++)
    {
        int iValue = pop();
        std::cout << iValue <<" ";
    }
    pop();//"Stack Underflow!
 
    std::cout << "Hello World!\n";
}
