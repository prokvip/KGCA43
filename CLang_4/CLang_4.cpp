/// Stack
#include <windows.h>
#include <iostream>
#define MAX 3
int   g_iQuqueArray[MAX];
int   g_iIn;
int   g_iOut;
void  InitQuque()
{
    g_iIn = g_iOut = 0;
    memset(g_iQuqueArray, 0, sizeof(int) * MAX);
}
int  push(int t)
{
    // 항상 1개의 방은 나머지가 존재한다.
    if ((g_iIn + 1) % MAX == g_iOut)
    {        
        return -1;
    }
    g_iQuqueArray[g_iIn] = t;
    g_iIn = ++g_iIn % MAX;
    return t;
}
int   pop()
{
    if (g_iIn == g_iOut)
    {       
        return -1;
    }
    int iData = g_iQuqueArray[g_iOut];
    g_iOut = ++g_iOut % MAX;
    return iData;
}
int main()
{
    InitQuque();

    int iWorkCounter = 10;
    int iWork = 0;
    srand(time(NULL));// 난수(무작위수) 발생 초기화
    while (iWorkCounter-- > 0)
    {
        iWork = rand() % 3;
        if (iWork == 0 || iWork == 1) // input
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
                std::cout << iData << " Input in=" 
                    << g_iIn << " "
                    << " out=" << g_iOut << std::endl;
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
                std::cout << iRet << " output in="
                    << g_iIn << " "
                    << " out=" << g_iOut << std::endl;
            }
        }
        Sleep(1000); //  1초 마다
    }
    std::cout << "Hello World!\n";
}
