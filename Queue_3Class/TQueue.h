#pragma once
#include <iostream>
#define MAX_SIZE  10
class AQueue
{
    int     item[MAX_SIZE];
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

