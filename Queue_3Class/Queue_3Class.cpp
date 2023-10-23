#include "TQueue.h"

int main()
{
    AQueue g_Queue;
    int g_iValue = 0;

    for (int i = 0; i < 5; i++)
    {
        if (g_Queue.IsFull())
        {
            std::cout << "Queue Overflow" << std::endl;
            break;
        }
        g_Queue.Enqueue(g_iValue++);
    }
    for (int i = 0; i < 6; i++)
    {
        if (g_Queue.IsEmpty())
        {
            std::cout << "Queue Underflow" << std::endl;
            break;
        }
        std::cout << g_Queue.Dequeue() << std::endl;
    }
    for (int i = 0; i < 5; i++)
    {
        if (g_Queue.IsFull())
        {
            std::cout << "Queue Overflow" << std::endl;
            break;
        }
        g_Queue.Enqueue(g_iValue++);
    }

    g_Queue.Display();

    int iNumCounter = g_Queue.Size();
    for (int i = 0; i < iNumCounter; i++)
    {
        if (g_Queue.IsEmpty())
        {
            break;
        }
        std::cout << g_Queue.Dequeue() << std::endl;
    }

    g_Queue.Display();
    std::cout << "Hello World!\n";
}



