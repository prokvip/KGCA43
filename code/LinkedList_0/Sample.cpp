#include <stdio.h>
#include <iostream>
struct TDATA
{
    int    iValue;
    TDATA* pNext;
};
TDATA* g_pBeginData= nullptr;
TDATA* g_pEndData = nullptr;



int top;
void Init(); // 데이터초기화
void Reset();
void NewData();
void AllDelete();
bool push_back(int data);
bool push_front(int data);
int  pop();
void print();

int main()
{
    //int    id = 3;
    //int    pa;
    //int    pb;
    //pa = id;
    //pb = id;

    //TDATA  d;
    //TDATA* pA = nullptr;
    //TDATA* pB = nullptr;
    //pA = &d;
    //pB = &d;
   
    Init();

    // 후위연결리스트
    // TDATA(1,2* address) ->  TDATA(2, 3* add) -> TDATA(3, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        push_back(iData);
    }
    print();
    AllDelete();

    // 전위연결리스트
    // TDATA(2, 1 address) ->TDATA(1, 0 address) -> TDATA(0, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        push_front(iData);
    }
    print();
    AllDelete();
}
TDATA* NewData(int data)
{
    //Heap Memory
    TDATA* pNewData = (TDATA*)malloc(sizeof(TDATA));
    if (pNewData == nullptr)
    {
        return nullptr;
    }
    pNewData->iValue = data;
    pNewData->pNext = nullptr;
    return pNewData;
}
// 메모리 단편화
// [ 0 0 0 0 0 0 ] -> 6 -> 3byte
// [ 0 1 0 1 1 0 ] -> [ 1 1 1 0 0 0] 
void Init()
{    
     g_pBeginData = nullptr;
     g_pEndData = nullptr;
}
void Reset()
{
    g_pBeginData = nullptr;
    g_pEndData = nullptr;
}
//bool Init()
//{    
//    g_pBeginData = (TDATA*)malloc(sizeof(TDATA));
//    if (g_pBeginData == nullptr)
//    {
//        return false;
//    }
//    return true;
//}
void AllDelete()
{
    //TDATA* pDelData  = g_pBeginData;
    //TDATA* pNextData = g_pBeginData;
    //while (pNextData)
    //{
    //    pDelData = pNextData;
    //    pNextData = pNextData->pNext;
    //    free(pDelData);
    //    pDelData = nullptr;
    //}
   
   TDATA* pNextData = g_pBeginData;
   for (TDATA* pBegin = g_pBeginData;
        pBegin != nullptr;
        pBegin = pNextData)
    {
        pNextData = pBegin->pNext;
        free(pBegin);
        pBegin = nullptr;
    }    
    Reset();
}
bool push_back(int data)
{
    TDATA* pNewData = NewData(data);   
    if (g_pBeginData == nullptr)
    {
        g_pBeginData = pNewData;    
        g_pEndData = pNewData;
    }
    else
    {
        g_pEndData->pNext = pNewData; 
        g_pEndData = pNewData;
    }
    return true;
    // 
   // //_ASSERT(top < 5);
   // //int ni = sizeof(g_pBeginData) / sizeof(g_pBeginData[0]);
   //// int n = _countof(g_pBeginData);
   // if (top >= 5)
   // {
   //     printf("\nStack Overflow!");        
   //     printf(" %s:%d", __FUNCTION__, __LINE__);
   //     return -1 ;
   // }
   // g_pBeginData[top++] = data; 
   // printf("\npush:%d", data);   
}
bool push_front(int data)
{
    TDATA* pNewData = NewData(data);
    if (g_pBeginData == nullptr)
    {
        g_pBeginData = pNewData;
        g_pEndData   = pNewData;
    }
    else
    {
        pNewData->pNext = g_pBeginData;
        g_pBeginData = pNewData;        
    }   
    return true;   
}
int  pop()
{
    //_ASSERT(top > 0);
   /* if (top <= 0)
    {
        printf("\nStack Underflow!");
        printf(" %s:%d", __FUNCTION__, __LINE__);
        return -99999;
    }
    printf("\npop");
    return g_pBeginData[--top];*/
    return 1;
}
void print()
{
    for (TDATA* pBegin= g_pBeginData;
         pBegin != nullptr;
         pBegin = pBegin->pNext)
    {
        printf("\n%d ", pBegin->iValue);
    }
}