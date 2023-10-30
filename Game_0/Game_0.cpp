#include <iostream>
#include "TNonPersonCharacter.h"
#include "TPersonCharacter.h"
#include "TLinkedList.cpp"
class TEnemy
{
public:
    TEnemy() {}
};
int main()
{
    //TCharacter a;
    //std::cout << a;

    //TCharacter* pa = new TCharacter;
    //std::cout << *pa;
    //TCharacter* pb = new TCharacter;
    //std::cout << *pb;
    //TCharacter* pc = new TCharacter;
    //std::cout << *pc;

    //TCharacter* list1[3];
    //list1[0] = new TCharacter;
    //list1[1] = new TCharacter;
    //list1[2] = new TCharacter;
    //for (int i = 0; i < 3; i++)
    //{
    //    std::cout << list1[i];
    //}

    TLinkedList<TCharacter*> g_CharacterList;
    //TCharacter* list[3];
    
    TCharacter* list0 = new TNonPersonCharacter;
    TPoint p = { 30,30 };
    list0->SetPos(p); // TPoint a = p;
    g_CharacterList.Add(&list0);

   // TNode<TCharacter*>* node = new TNode<TCharacter*>();
   // g_CharacterList.BackwardAdd(node);
    TCharacter* list1 = new TNonPersonCharacter;
    list1 = new TPersonCharacter;   
    list1->SetPos({50,50}); // TPoint a = {3,3};
    g_CharacterList.Add(&list1);

    TCharacter* list2 = new TNonPersonCharacter;
    list2 = new TNonPersonCharacter;
    list2->SetPos({ 70,70 });
    g_CharacterList.Add(&list2);

    for (TNode<TCharacter*>* pNode = g_CharacterList.m_pHead->m_pNext;
        pNode != g_CharacterList.m_pTail;
        pNode = pNode->m_pNext)
    {        
        pNode->DATA->SetHealth(50);
        TNonPersonCharacter* pc1 = dynamic_cast<TNonPersonCharacter*>(pNode->DATA);
        TPersonCharacter* pc2 = dynamic_cast<TPersonCharacter*>(pNode->DATA);
        
        TNonPersonCharacter* pc3 = static_cast<TNonPersonCharacter*>(pNode->DATA);
        TPersonCharacter* pc4 = static_cast<TPersonCharacter*>(pNode->DATA);

        if (pc2 != nullptr)
        {
            pc2->SetExperience(5);
        }
    }
   
    g_CharacterList.Show();
    g_CharacterList.AllDelete();

    std::cout << "Hello World!\n";
}
