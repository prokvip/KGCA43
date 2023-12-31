﻿/// Stack
#include <windows.h>
#include <iostream>
#define MAX 30
int   g_iStackAray[MAX];
int   g_iTop;
void  InitStack()
{
    g_iTop = -1;
    memset(g_iStackAray, 0, sizeof(int) * MAX);
}
int  push(int t)
{
    if (g_iTop >= MAX - 1)
    {       
        return -1;
    }
    g_iStackAray[++g_iTop] = t;
    return t;
}
int   pop()
{
    if (g_iTop < 0)
    {        
        return -1;
    }
    return g_iStackAray[g_iTop--];
}
int main()
{
    InitStack();

    int iWorkCounter = 100;
    int iWork = 0;
    srand(time(NULL));// 난수(무작위수) 발생 초기화
    while (iWorkCounter-- > 0)
    {
        iWork = rand() % 3;
        if (iWork == 0 || iWork== 1) // input
        {
            std::cout << "input ->";
            int iData = rand() % 100;// 0 ~ 99
            int iRet = push(iData);
            if (iRet == -1)
            {
                std::cout << "Stack Overflow!\n";
            }
            else
            {
                std::cout << iData << " Input top=" << 
                      g_iTop << std::endl;
            }
        }
        else  // output
        {
            std::cout << "output ->";
            int iRet = pop();
            if (iRet == -1)
            {
                std::cout << "Stack Underflow!\n";
            }
            else
            {
                std::cout << iRet << " Output! top=" << 
                    g_iTop << std::endl;
            }
        }
        Sleep(1000); //  1초 마다
    }
    std::cout << "Hello World!\n";
}
