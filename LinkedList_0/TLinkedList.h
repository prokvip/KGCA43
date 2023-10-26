#pragma once
#include <iostream>
class TNode
{
public:
	int		m_iData;
	TNode*  m_pNext;
	TNode()
	{
		m_iData = -1;
		m_pNext = nullptr;
	}
};
class TLinkedList
{
public:
    TNode* g_pList = nullptr;
    TNode* g_pEnd = nullptr;
public:
    TLinkedList();
    ~TLinkedList();
    TNode*  NewNode(int iData);
    void    Add(TNode* pNewNode);
    void    Show();
    void    AllDelete();
};

