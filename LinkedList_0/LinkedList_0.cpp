
#include "TLinkedList.h"

TLinkedList list;
int main()
{    
    for (int i = 0; i < 5; i++)
    {
        list.Add(list.NewNode(i*100));
    }
    list.Show();
    std::cout << "End!\n";
}
