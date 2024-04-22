#include "Sample.h"
TDATA* g_pTemp = nullptr;
int main()
{
    Init();

    // 후위연결리스트
    // TDATA(1,2* address) ->  TDATA(2, 3* add) -> TDATA(3, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        push_back(g_pTail,iData);
    }
    g_pTemp = Find(2);
    push_back(g_pTemp, 4);
    Delete(g_pTemp);
    Delete(4);

    /*ShowFL(nullptr, nullptr);
    ShowFL(nullptr, g_pTail);
    ShowFL(g_pTail, nullptr);
    ShowFL(g_pHead->pNext, nullptr);
    ShowFL(nullptr,g_pHead->pNext);
    ShowFL(g_pHead->pNext, g_pTail );
    ShowFL(g_pTail, g_pHead->pNext);
    ShowFL(g_pHead, g_pHead->pNext);
    ShowFL(g_pHead, g_pHead);*/

    print(true);

    for (int iData = 0; iData < 3; iData++)
    {
        push_back(g_pTail, iData);
    }

    AllDeleteData();

    system("cls");
    // 전위연결리스트
    // TDATA(2, 1 address) ->TDATA(1, 0 address) -> TDATA(0, nullptr) 
    for (int iData = 0; iData < 3; iData++)
    {
        push_front(g_pHead, iData);
    }

    g_pTemp = Find(1);
    push_front(g_pTemp, 4);
    g_pTemp = Find(4);
    push_front(g_pTemp, 4);
    Delete(g_pTemp);
    Delete(1);

    ShowLF(nullptr, nullptr);
    ShowLF(nullptr, g_pTail);
    ShowLF(g_pTail, nullptr);
    ShowLF(g_pHead->pNext, nullptr);
    ShowLF(nullptr, g_pHead->pNext);
    ShowLF(g_pHead->pNext, g_pTail);
    ShowLF(g_pTail, g_pHead->pNext);

    print(false);
    AllDeleteData();

    Release();
}
bool    Delete(TDATA* pDel)
{
    if (pDel == nullptr) return false;
    if (pDel == g_pHead) return false;
    if (pDel == g_pTail) return false;

    // A <->  D  <->   B
    TDATA* A = pDel->pPrev;
    TDATA* B = pDel->pNext;
    free(pDel);
    A->pNext = B;
    B->pPrev = A;  
    return true;
}
bool    Delete(int iValue)
{
    TDATA* pDelData = Find(iValue);
    return Delete(pDelData);
}
TDATA* Find(int iValue)
{
    for (TDATA* pBegin = g_pHead->pNext;
        pBegin != g_pTail;
        pBegin = pBegin->pNext)
    {
        if (pBegin->iValue == iValue)
        {
            return pBegin;
        }
    }
    return nullptr;
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

    // 원형연결리스트 구축
    g_pHead->pPrev = g_pTail;
    g_pTail->pNext = g_pHead;
    
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

//  H->a->b->T
bool push_back(TDATA* pPosition, int data)
{
    if (pPosition == nullptr) return false;
    // H<->N1<->T
    // H<->N1<->N2<->T
    TDATA* pNewData = NewData(data);
    TDATA* pPreData = pPosition->pPrev;
    // N1 -> N2    T
    pPreData->pNext = pNewData;
    // N1    N2 -> T
    pNewData->pNext = pPosition;
    // N1    N2 <- T
    pPosition->pPrev = pNewData;
    // N1<- N2   T
    pNewData->pPrev = pPreData;   
    return true;  
}
bool push_front(TDATA* pPos, int data)
{    
    TDATA* pNewData = NewData(data);
    TDATA* pPreData = pPos->pNext; //(N1)
    // N1(pre)<-> N2 <-> H
    pPos->pNext = pNewData;
    pNewData->pNext = pPreData;
    pPreData->pPrev = pNewData; //(N2)   
    pNewData->pPrev = pPos;
    return true;   
}
void ShowFL(TDATA* a, TDATA* b)
{
    if (a == nullptr ) a = g_pHead->pNext;
    if (b == nullptr ) b = a;

    printf("\n");
    TDATA* pBegin = a;
    TDATA* pEnd = b;
    do {
        if (pBegin != g_pHead && pBegin != g_pTail)
        {
            printf("%d ", pBegin->iValue);
        }
        pBegin = pBegin->pNext;
    } while (pBegin != b);
}
void ShowLF(TDATA* a, TDATA* b)
{
    if (a == nullptr) a = g_pTail->pPrev;
    if (b == nullptr) b = a;

    printf("\n");
    TDATA* pBegin = a;
    TDATA* pEnd = b;
    do {
        if (pBegin != g_pHead && pBegin != g_pTail)
        {
            printf("%d ", pBegin->iValue);
        }
        pBegin = pBegin->pPrev;
    } while (pBegin != b);
}
void print(bool bforward)
{
    printf("\n");
    if (bforward)
    {
        ShowFL(g_pHead->pNext, g_pTail);
    }
    else    
    {
        ShowLF(g_pTail->pPrev, g_pHead);
    }
}

