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
    TCharacter* list[3];
    list[0] = new TNonPersonCharacter;
    TPoint p = { 30,30 };
    list[0]->SetPos(p); // TPoint a = p;
    list[1] = new TPersonCharacter;   
    list[1]->SetPos({50,50}); // TPoint a = {3,3};
    list[2] = new TNonPersonCharacter;
    list[2]->SetPos({ 70,70 });

    for( int i=0; i< 3; i++)
    { 
        list[i]->SetHealth(50);
        TNonPersonCharacter* pc1 = dynamic_cast<TNonPersonCharacter*>(list[i]);
        TPersonCharacter* pc2 = dynamic_cast<TPersonCharacter*>(list[i]);
        
        TNonPersonCharacter* pc3 = static_cast<TNonPersonCharacter*>(list[i]);
        TPersonCharacter* pc4 = static_cast<TPersonCharacter*>(list[i]);

        if (pc2 != nullptr)
        {
            pc2->SetExperience(5);
        }
    }
   
    for (int i = 0; i < 3; i++)
    {
        std::cout << list[i];
    }

    for (int i = 0; i < 3; i++)
    {
        delete  list[i];
    }    
    std::cout << "Hello World!\n";
}
