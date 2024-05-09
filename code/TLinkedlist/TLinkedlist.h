#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

template< class T>
class TNode
{
public:
    T*     data;
    TNode* pNext; // �ڷ�
    TNode* pPrev; // ������
};

template< class T>
class TLinkedlist
{
private:
    TNode<T>* g_pHead;
    TNode<T>* g_pTail;
    int    g_iDataCounter;
public:
    TNode<T>* head() { return g_pHead; }
    TNode<T>* begin() { return g_pHead->pNext; }
    TNode<T>* end() { return g_pTail; }
    int    size() { return g_iDataCounter; }
public:
	TLinkedlist();
    virtual ~TLinkedlist();
public:
    //// �������ʱ�ȭ
    void    Init(); // �������ʱ�ȭ
    void    Reset();
    void    Release();
    void    AllDeleteData();
    bool    push_back(TNode<T>* pPos, T* pNewData);
    bool    push_front(TNode<T>* pPos, T* pNewData);
    TNode<T>*  Find(int data);
    bool    Delete(int data);
    bool    Delete(TNode<T>* pDel);
};