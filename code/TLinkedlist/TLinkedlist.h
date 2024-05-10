#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

template< class T>
class TIterator
{    
public:    
    T* data;
    T& operator*(void) { return *data; }
    // 순회( 앞<->뒤 )
    void operator++(void) {}
    void operator++(int) {}
    TIterator* pNext; // 뒤로
    TIterator* pPrev; // 앞으로
};

template< class T>
class TLinkedlist
{    
private:
    TIterator<T>* g_pHead;
    TIterator<T>* g_pTail;
    int    g_iDataCounter;
public:    
    TIterator<T>* head() { return g_pHead; }
    TIterator<T>* begin() { return g_pHead->pNext; }
    TIterator<T>* end() { return g_pTail; }
    int    size() { return g_iDataCounter; }
public:
	TLinkedlist();
    virtual ~TLinkedlist();
public:
    //// 데이터초기화
    void    Init(); // 데이터초기화
    void    Reset();
    void    Release();
    void    AllDeleteData();
    bool    push_back(TIterator<T>* pPos, T* pNewData);
    bool    push_front(TIterator<T>* pPos, T* pNewData);
    TIterator<T>*  Find(int data);
    bool    Delete(int data);
    bool    Delete(TIterator<T>* pDel);
};