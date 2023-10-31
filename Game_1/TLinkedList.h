#pragma once
#include <iostream>
template<class T>
class TNode
{
public:
    T		DATA;
	TNode*  m_pNext;
    TNode*  m_pPrev;
	TNode()
	{
		//DATA = -1;
		m_pNext = nullptr;
        m_pPrev = nullptr;
	}
};
template<class T>
class TLinkedList
{
public:
    int       m_iCounter=0;
    TNode<T>* m_pHead = nullptr;
    TNode<T>* m_pTail = nullptr;
public:
    TLinkedList();
    ~TLinkedList();
    TNode<T>*  NewNode(T data);
    void    Add(T* pNewNode);
    void    ForwardAdd(TNode<T>* pNewNode);
    void    BackwardAdd(TNode<T>* pNewNode);
    void    Show();
    void    ReverseShow();
    void    AllDelete();
    int     size();
};
