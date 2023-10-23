#include <iostream>
#include "TQueue.h"

int main()
{
    {
        TQueue queue1;
        TQueue queue(100);
        queue.Enqueue(9);
        queue.Enqueue(6);
        queue.Enqueue(5);
        queue.Enqueue(3);
        queue.Display();

        // 복사생성자 호출
        TQueue queueCopy = queue;
        std::cout << "Hello World!\n";
    }
    std::cout << "Hello World!\n";
}
