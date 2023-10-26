#pragma once
#include <iostream>
class TNode
{
public:
    int		DATA;
	TNode*  m_pNext;
    TNode*  m_pPrev;
	TNode()
	{
		//DATA = -1;
		m_pNext = nullptr;
        m_pPrev = nullptr;
	}
};
class TLinkedList
{
public:
    TNode* m_pHead = nullptr;
    TNode* m_pTail = nullptr;
public:
    TLinkedList();
    ~TLinkedList();
    TNode*  NewNode(int data);
    void    ForwardAdd(TNode* pNewNode);
    void    BackwardAdd(TNode* pNewNode);
    void    Show();
    void    ReverseShow();
    void    AllDelete();
};

