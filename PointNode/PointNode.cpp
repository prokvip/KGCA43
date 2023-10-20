#include <iostream>
int   Fun1(int pData)
{
    return 0;
}
int   Fun1(int* pData)
{
    if (pData == nullptr) return -999999;
    int iSum = *pData + 100;
    return iSum;
}
// int a = 100;  Fun1(&a)
int*   Fun2(int* pData)
{
    *pData += 100;
    return pData;
}
// int a = 100;  int*c = Fun2(&a)

/// <summary>
/// 값이 복사되어 반환된다.
/// 새로운 반환 변수가 생성되서 값이 복사되어 반환된다.
/// </summary>
int g_iTemp = 1000;
int Fun()
{
    return g_iTemp;
}

int main()
{
    int aa = 0;
    {
        int aa = 1;
        {
            int aa = 2;
        }
        int bb = aa;
    }
    int bb = aa;

    int  iRet = Fun();
    Fun1(NULL);
    Fun1(nullptr);

    int a = 100;  
    Fun1(&a);
    int* c = Fun2(&a);


    int* p1 = &a;
    int* p2 = NULL;
    int* p3 = NULL;
    int* p4 = NULL;
    int* link = p1;

    std::cout << "Hello World!\n";
}
