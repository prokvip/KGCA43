#pragma once
#include <iostream>
class TQueue
{
    int  MAX_SIZE = 10;
public:    
    TQueue(int iSize) // 기본 생성자
    {
        item = new int[iSize];
        MAX_SIZE = iSize;
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = 0;
        }
    }
    TQueue(const TQueue& queue)
    {
        //item = queue.item; // 옅은 복사
        // 깊은 복사 작업.
        MAX_SIZE = queue.MAX_SIZE;
        item = new int[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = queue.item[i];
        }        
        rear = queue.rear;
        front = queue.front;
    }
    TQueue() // 기본 생성자
    {
        MAX_SIZE = 10;
        item = new int[MAX_SIZE];
        for (int i = 0; i < MAX_SIZE; i++)
        {
            item[i] = 0;
        }
    }
    ~TQueue() // 기본 해제자
    {
        delete[] item;
    }
public:
    int*    item=nullptr;
    int     rear = -1;
    int     front = 0;
public:
    void    Enqueue(int data);
    int     Dequeue();
    int     Size();
    bool    IsEmpty();
    bool    IsFull();
    void    Display();

};

