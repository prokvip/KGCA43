#include "TLinkedList.h"

template<class T>
TLinkedList<T>::TLinkedList()
{
    m_pHead = new TNode<T>;
    m_pTail = new TNode<T>;
    m_pHead->m_pNext = m_pTail;
    m_pHead->m_pPrev = nullptr;
    m_pTail->m_pNext = nullptr;
    m_pTail->m_pPrev = m_pHead;
}
template<class T>
TLinkedList<T>::~TLinkedList()
{
    AllDelete();
}
template<class T>
TNode<T>* TLinkedList<T>::NewNode(T iData)
{
    TNode<T>* pNewNode = new TNode<T>;
    pNewNode->DATA = iData;
    return pNewNode;
}
template<class T>
void   TLinkedList<T>::Add(T* pNewData)
{
    TNode<T>* pNewNode = new TNode<T>();
    pNewNode->DATA = *pNewData;
    // H   <-> pPrev   <->  T
    // H   <-> pPrev   <->  pNewNode  <-> T
    TNode<T>* pPrev = m_pTail->m_pPrev;//a

    // pPrev   ->  pNewNode
    pPrev->m_pNext = pNewNode;
    // pPrev   <-  pNewNode    
    pNewNode->m_pPrev = pPrev;

    // pNewNode -> T
    pNewNode->m_pNext = m_pTail;
    // pNewNode <- T
    m_pTail->m_pPrev = pNewNode;

    m_iCounter++;
}
template<class T>
void   TLinkedList<T>::BackwardAdd(TNode<T>* pNewNode)
{
    // H   <-> pPrev   <->  T
    // H   <-> pPrev   <->  pNewNode  <-> T
    TNode<T>* pPrev = m_pTail->m_pPrev;//a

    // pPrev   ->  pNewNode
    pPrev->m_pNext = pNewNode;
    // pPrev   <-  pNewNode    
    pNewNode->m_pPrev = pPrev;

    // pNewNode -> T
    pNewNode->m_pNext = m_pTail;
    // pNewNode <- T
    m_pTail->m_pPrev = pNewNode;

    m_iCounter++;
}
template<class T>
void   TLinkedList<T>::ForwardAdd(TNode<T>* pNewNode)
{
    // H   <-> pNext    <->  T
    // H   <-> pNewNode <-> pNext   <->  T
    TNode<T>* pNext = m_pHead->m_pNext;
    // H   -> pNewNode
    m_pHead->m_pNext = pNewNode;
    // H   <- pNewNode
    pNewNode->m_pPrev = m_pHead;
    //pNewNode -> pNext
    pNewNode->m_pNext = pNext;
    //pNewNode <- pNext
    pNext->m_pPrev = pNewNode;

    m_iCounter++;
}
template<class T>
void  TLinkedList<T>::Show()
{
    for (TNode<T>* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        pNode = pNode->m_pNext)
    {
        // 2.0f * object
        std::cout << pNode->DATA << std::endl;
    }
}
template<class T>
void  TLinkedList<T>::ReverseShow()
{
    for (TNode<T>* pNode = m_pTail->m_pPrev;
        pNode != m_pHead;
        pNode = pNode->m_pPrev)
    {
        std::cout << pNode->DATA << std::endl;
    }
}
template<class T>
void  TLinkedList<T>::AllDelete()
{
    TNode<T>* pNode = m_pHead;
    while (pNode != nullptr)
    {
        TNode<T>* pDelNode = pNode;
        pNode = pNode->m_pNext;
        delete pDelNode;        
    }
    m_pHead = nullptr;
    m_pTail = nullptr;
}

template<class T>
int TLinkedList<T>::size()
{
    int iCnt = 0;
    for (TNode<T>* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        pNode = pNode->m_pNext)
    {
        iCnt++;
    }
    return iCnt;
}