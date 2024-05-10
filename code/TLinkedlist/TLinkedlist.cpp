#include "TLinkedlist.h"

template<class T>
TLinkedlist<T>::TLinkedlist()
{
    Init();
}
template<class T>
TLinkedlist<T>::~TLinkedlist()
{
    Release();
}
template<class T>
void TLinkedlist<T>::AllDeleteData()
{
    TIterator<T>* pNextData = g_pHead->pNext;
    for (TIterator<T>* pBegin = g_pHead->pNext;
        pBegin != g_pTail;
        pBegin = pNextData)
    {
        pNextData = pBegin->pNext;
        free(pBegin);
        pBegin = nullptr;
    }
    Reset();
}
template<class T>
bool    TLinkedlist<T>::Delete(TIterator<T>* pDel)
{
    if (pDel == nullptr) return false;
    if (pDel == g_pHead) return false;
    if (pDel == g_pTail) return false;

    // A <->  D  <->   B
    TIterator<T>* A = pDel->pPrev;
    TIterator<T>* B = pDel->pNext;
    free(pDel);
    A->pNext = B;
    B->pPrev = A;
    g_iDataCounter--;
    return true;
}
template<class T>
bool    TLinkedlist<T>::Delete(int data)
{
    TIterator<T>* pDelData = Find(data);
    return Delete(pDelData);
}
template<class T>
TIterator<T>* TLinkedlist<T>::Find(int data)
{
    /*for (TIterator<T>* pBegin = g_pHead->pNext;
        pBegin != g_pTail;
        pBegin = pBegin->pNext)
    {
        if (pBegin->data-> == data)
        {
            return pBegin;
        }
    }*/
    return nullptr;
}

template<class T>
void TLinkedlist<T>::Init()
{
    g_pHead = new TIterator<T>;
    memset(g_pHead, 0, sizeof(TIterator<T>));
    g_pHead->data = nullptr;

    g_pTail = new TIterator<T>;
    memset(g_pTail, 0, sizeof(TIterator<T>));
    g_pTail->data = nullptr;
    Reset();
}
template<class T>
void TLinkedlist<T>::Reset()
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
template<class T>
void    TLinkedlist<T>::Release()
{
    delete g_pHead;
    delete g_pTail;
    g_pTail = nullptr;
    g_pHead = nullptr;
}
template<class T>
bool TLinkedlist<T>::push_back(TIterator<T>* pPosition, 
    T* pData)
{
    if (pPosition == nullptr) return false;
    TIterator<T>* pNewData = new TIterator<T>;
    pNewData->data = pData;
    // H<->N1<->T
    // H<->N1<->N2<->T
    TIterator<T>* pPreData = pPosition->pPrev;
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
template<class T>
bool TLinkedlist<T>::push_front(TIterator<T>* pPos,
    T* pData)
{
    TIterator<T>* pPreData = pPos->pNext; //(N1)
    TIterator<T>* pNewData = new TIterator<T>;
    pNewData->data = pData;
    // N1(pre)<-> N2 <-> H
    pPos->pNext = pNewData;
    pNewData->pNext = pPreData;
    pPreData->pPrev = pNewData; //(N2)   
    pNewData->pPrev = pPos;
    g_iDataCounter++;
    return true;
}