#include "TLinkedList.h"

TLinkedList::TLinkedList()
{
    m_pHead = new TNode;
    m_pTail = new TNode;
    m_pHead->m_pNext = m_pTail;
    m_pHead->m_pPrev = nullptr;
    m_pTail->m_pNext = nullptr;
    m_pTail->m_pPrev = m_pHead;
}

TLinkedList::~TLinkedList()
{
    AllDelete();
}

TNode* TLinkedList::NewNode(int iData)
{
    TNode* pNewNode = new TNode;
    pNewNode->DATA = iData;
    return pNewNode;
}

void   TLinkedList::BackwardAdd(TNode* pNewNode)
{
    // H   <-> pPrev   <->  T
    // H   <-> pPrev   <->  pNewNode  <-> T
    TNode* pPrev = m_pTail->m_pPrev;//a

    // pPrev   ->  pNewNode
    pPrev->m_pNext = pNewNode;
    // pPrev   <-  pNewNode    
    pNewNode->m_pPrev = pPrev;

    // pNewNode -> T
    pNewNode->m_pNext = m_pTail;
    // pNewNode <- T
    m_pTail->m_pPrev = pNewNode;
}

void   TLinkedList::ForwardAdd(TNode* pNewNode)
{
    // H   <-> pNext    <->  T
    // H   <-> pNewNode <-> pNext   <->  T
    TNode* pNext = m_pHead->m_pNext;
    // H   -> pNewNode
    m_pHead->m_pNext = pNewNode;
    // H   <- pNewNode
    pNewNode->m_pPrev = m_pHead;
    //pNewNode -> pNext
    pNewNode->m_pNext = pNext;
    //pNewNode <- pNext
    pNext->m_pPrev = pNewNode;
}

void  TLinkedList::Show()
{
    for (TNode* pNode = m_pHead->m_pNext;
        pNode != m_pTail;
        pNode = pNode->m_pNext)
    {
        // 2.0f * object
        std::cout << pNode->DATA << std::endl;
    }
}

void  TLinkedList::ReverseShow()
{
    for (TNode* pNode = m_pTail->m_pPrev;
        pNode != m_pHead;
        pNode = pNode->m_pPrev)
    {
        std::cout << pNode->DATA << std::endl;
    }
}

void  TLinkedList::AllDelete()
{
    TNode* pNode = m_pHead;
    while (pNode != nullptr)
    {
        TNode* pDelNode = pNode;
        pNode = pNode->m_pNext;
        delete pDelNode;
    }
}