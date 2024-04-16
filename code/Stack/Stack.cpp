#include <stdio.h>
#include <iostream>
int g_iStack[5];
int top;
void InitStack(); // 데이터초기화
int  push(int data);
int  pop();
void print();

int main()
{
    InitStack();
    push(1);
    push(2);
    int iRet = pop();
    push(3);
    push(4);
    push(-1);
    print();

    push(6);
    push(7);
    push(8);

    iRet = pop();
    iRet = pop();
    push(9);
    print();
    iRet = pop();
    iRet = pop();
    iRet = pop();
}

void InitStack()
{

}
int push(int data)
{
    //_ASSERT(top < 5);
    //int ni = sizeof(g_iStack) / sizeof(g_iStack[0]);
   // int n = _countof(g_iStack);
    if (top >= 5)
    {
        printf("\nStack Overflow!");        
        printf(" %s:%d", __FUNCTION__, __LINE__);
        return -1 ;
    }
    g_iStack[top++] = data; 
    printf("\npush:%d", data);
    return 0;
}
int  pop()
{
    //_ASSERT(top > 0);
    if (top <= 0)
    {
        printf("\nStack Underflow!");
        printf(" %s:%d", __FUNCTION__, __LINE__);
        return -99999;
    }
    printf("\npop");
    return g_iStack[--top];
}
void print()
{
    for (int i = top-1; i >= 0; i--)
    {
        printf("\n%d ", g_iStack[i]);
    }
}