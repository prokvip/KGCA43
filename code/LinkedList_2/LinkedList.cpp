#include "LinkedList.h"
void    Test()
{
    g_iValue = 5;
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
    g_iDataCounter--;
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
void    Release()
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
    g_iDataCounter = 0;
    // Head -> Tail
    g_pHead->pNext = g_pTail;
    // Tail -> prev
    g_pTail->pPrev = g_pHead;

    // 원형연결리스트 구축
    g_pHead->pPrev = g_pTail;
    g_pTail->pNext = g_pHead;

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

    g_iDataCounter++;
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
    g_iDataCounter++;
    return true;
}