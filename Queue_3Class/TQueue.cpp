#include "TQueue.h"

void AQueue::Enqueue(int data)
{
    item[++rear] = data;
}
int AQueue::Dequeue()
{
    return item[front++];
}
int AQueue::Size()
{
    return (rear - front + 1);
}
bool AQueue::IsEmpty()
{
    if (front > rear)
    {
        return true;
    }
    return false;
}
bool AQueue::IsFull()
{
    if (Size() >= MAX_SIZE)
    {
        return true;
    }
    return false;
}
void AQueue::Display()
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