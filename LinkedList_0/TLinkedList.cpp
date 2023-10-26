#include "TLinkedList.h"
TLinkedList::TLinkedList()
{
    g_pEnd = g_pList;
}
TLinkedList::~TLinkedList()
{
    AllDelete();
}
TNode* TLinkedList::NewNode(int iData)
{
    TNode* pNewNode = new TNode;
    pNewNode->m_iData = iData;
    return pNewNode;
}
void   TLinkedList::Add(TNode* pNewNode)
{
    if (g_pList == nullptr)
    {
        g_pList = pNewNode;
        g_pEnd = pNewNode;
    }
    else
    {
        if (g_pEnd != nullptr)
        {
            g_pEnd->m_pNext = pNewNode;
            g_pEnd = pNewNode;
        }
    }
}
void  TLinkedList::Show()
{
    for (TNode* pNode = g_pList;
        pNode != nullptr;
        pNode = pNode->m_pNext)
    {
        std::cout << pNode->m_iData << std::endl;
    }
}
void  TLinkedList::AllDelete()
{
    TNode* pNode = g_pList;
    while (pNode != nullptr)
    {
        TNode* pDelNode = pNode;
        pNode = pNode->m_pNext;
        delete pDelNode;
    }
}