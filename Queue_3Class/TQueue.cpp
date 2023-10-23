#include "TQueue.h"

void TQueue::Enqueue(int data)
{
    item[++rear] = data;
}
int TQueue::Dequeue()
{
    return item[front++];
}
int TQueue::Size()
{
    return (rear - front + 1);
}
bool TQueue::IsEmpty()
{
    if (front > rear)
    {
        return true;
    }
    return false;
}
bool TQueue::IsFull()
{
    if (Size() >= MAX_SIZE)
    {
        return true;
    }
    return false;
}
void TQueue::Display()
{
    if (!IsEmpty())
    {
        for (int i = front;
            i <= rear;
            i++)
        {
            std::cout << item[i] << std::endl;
        }
    }
    else
    {
        std::cout << "Queue Underflow" << std::endl;
    }
}