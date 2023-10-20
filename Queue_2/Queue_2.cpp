#include <iostream>
#define MAX_SIZE  10
struct TQueue
{
    int item[MAX_SIZE];
    int rear = -1;
    int front= 0;
};
TQueue g_Queue;
void    Enqueue(int data);
int     Dequeue();
int     Size();
bool    IsEmpty();
bool    IsFull();
void    Display();

int g_iValue = 0;
int main()
{
    for (int i = 0; i < 5; i++)
    {
        if (IsFull())
        {
            std::cout << "Queue Overflow" << std::endl;
            break;            
        }
        Enqueue(g_iValue++);
    }
    for (int i = 0; i < 6; i++)
    {
        if (IsEmpty())
        {
            std::cout << "Queue Underflow" << std::endl;
            break;
        }
        std::cout << Dequeue() << std::endl;
    }
    for (int i = 0; i < 5; i++)
    {
        if (IsFull())
        {
            std::cout << "Queue Overflow" << std::endl;
            break;
        }
        Enqueue(g_iValue++);
    }

    Display();

    int iNumCounter = Size();
    for (int i = 0; i < iNumCounter; i++)
    {
        if (IsEmpty())
        {
            break;
        }
        std::cout << Dequeue() << std::endl;
    }

    Display();
    std::cout << "Hello World!\n";
}



void Enqueue(int data)
{
    g_Queue.item[++g_Queue.rear] = data;
}
int Dequeue()
{
    return g_Queue.item[g_Queue.front++];
}
int Size()
{
    return (g_Queue.rear - g_Queue.front + 1);
}
bool IsEmpty()
{
    if (g_Queue.front > g_Queue.rear)
    {
        return true;
    }
    return false;
}
bool IsFull()
{
    if (Size() >= MAX_SIZE)
    {
        return true;
    }
    return false;
}

void Display()
{
    if (!IsEmpty())
    {
        for (int i = g_Queue.front;
            i <= g_Queue.rear;
            i++)
        {
            std::cout << g_Queue.item[i] << std::endl;
        }
    }
    else
    {
        std::cout << "Queue Underflow" << std::endl;
    }
}
