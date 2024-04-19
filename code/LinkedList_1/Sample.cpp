#include "Sample.h"
int main()
{
    Init();

    // 후위연결리스트
    // TDATA(1,2* address) ->  TDATA(2, 3* add) -> TDATA(3, nullptr) 
    for (int iData = 0; iData < 30; iData++)
    {
        push_back(iData);
    }
    print(true);
    AllDeleteData();

    // 전위연결리스트
    // TDATA(2, 1 address) ->TDATA(1, 0 address) -> TDATA(0, nullptr) 
    for (int iData = 0; iData < 30; iData++)
    {
        push_front(iData);
    }
    print(true);
    AllDeleteData();

    Release();
}

void Release()
{
    free(g_pHead);    
    free(g_pTail);
    g_pTail = nullptr;
    g_pHead = nullptr;
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
     g_pHead = (TDATA*)malloc(sizeof(TDATA));
     memset(g_pHead, 0, sizeof(TDATA));
     g_pHead->iValue = 1111;

     g_pTail = (TDATA*)malloc(sizeof(TDATA));
     memset(g_pTail, 0, sizeof(TDATA));
     g_pTail->iValue = 9999;
     Reset();
}
void Reset()
{
    // Head -> Tail
    g_pHead->pNext = g_pTail;
    // Tail -> prev
    g_pTail->pPrev = g_pHead;

    g_pEndData = g_pHead;// 후위연결
}

void AllDeleteData()
{   
   TDATA* pNextData = g_pHead->pNext;
   for (TDATA* pBegin = g_pHead->pNext;
        pBegin != g_pTail;
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
    // H<->N1<->T
    // H<->N1<->N2<->T
    TDATA* pNewData = NewData(data);
    TDATA* pPreData = g_pTail->pPrev;
    // N1 -> N2    T
    pPreData->pNext = pNewData;
    // N1    N2 -> T
    pNewData->pNext = g_pTail;
    // N1    N2 <- T
    g_pTail->pPrev = pNewData;
    // N1<- N2   T
    pNewData->pPrev = pPreData;   
    

    g_pEndData->pNext = pNewData;
    g_pEndData = pNewData;    
    return true;  
}
bool push_front(int data)
{    
    TDATA* pNewData = NewData(data);
    TDATA* pPreData = g_pHead->pNext; //(N1)
    // N1(pre)<-> N2 <-> H
    g_pHead->pNext = pNewData;
    pNewData->pNext = pPreData;
    pPreData->pPrev = pNewData; //(N2)   
    pNewData->pPrev = g_pHead;
    return true;   
}
void Show(TDATA* a, TDATA* b, bool bForword)
{
    for (TDATA* pBegin = a;
        pBegin != b;
        pBegin = (bForword) ? (pBegin->pNext) :(pBegin->pPrev))
    {
        printf("%d ", pBegin->iValue);
    }
}
void print(bool bforward)
{
    printf("\n");
    if (bforward)
    {
        Show(g_pHead->pNext, g_pTail, true);
    }
    else    
    {
        Show(g_pTail->pPrev, g_pHead,false);
    }
}

