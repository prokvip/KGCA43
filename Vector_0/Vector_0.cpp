#include <iostream>
#include <vector>
#include "TPersonCharacter.h"
#include "TNonPersonCharacter.h"

int main()
{
    /// <summary>
    /// vector 1
    /// </summary>
    /// <returns></returns>
    std::vector<int> list;
    for (int i = 0; i < 10; i++)
    {
        //list.push_back(i);
        list.emplace_back(i);
    }
    // 인덱싱 지원(배열접근 가능하다.)
    for (int i = 0; i < list.size(); i++)
    {
        std::cout << list[i] << " ";
    }
    std::cout << std::endl;
    // list.begin() -> 실제 첫 데이터 저장.
    // list.end() -> 가상의 꼬리 노드
    for (std::vector<int>::iterator iter = list.begin();
         iter != list.end();
         iter++)
    {
        int iValue = *iter;//TNode<>
        std::cout << iValue << " ";
    }
    list.clear();
    std::cout << std::endl;
    /// <summary>
    /// vector2 
    /// </summary>
    /// <returns></returns>
    std::vector<TCharacter> listChar;
    for (int i = 0; i < 3; i++)
    {
        TCharacter ch({3,3});
        TPoint p = { 30*i,30*i };
        ch.SetPos(p);
        listChar.push_back(ch);
        //listChar.emplace_back(ch);        
    }
    for (int i = 0; i < 3; i++)
    {
        TPoint p = { 30 * i,30 * i };
        listChar.emplace_back(p);
    }
    for (int i = 0; i < listChar.size(); i++)
    {
        std::cout << listChar[i] << " ";
    }
    listChar.clear();

    /// <summary>
    /// vector3 
    /// </summary>
    /// <returns></returns>
    std::vector<TCharacter*> listCharPoint;
    for (int i = 0; i < 5; i++)
    {
        TCharacter* ch = new TCharacter();
        TPoint p = { 30 * i,30 * i };
        ch->SetPos(p);
        listCharPoint.push_back(ch);
    }
    for (int i = 0; i < listCharPoint.size(); i++)
    {
        std::cout << listCharPoint[i] << " ";
    }
    for (int i = 0; i < listCharPoint.size(); i++)
    {
       delete listCharPoint[i];
    }
    listCharPoint.clear();
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
    std::vector<TCharacter*> g_CharacterList;

    TCharacter* list0 = new TNonPersonCharacter;
    TPoint p = { 30,30 };
    list0->SetPos(p); // TPoint a = p;
    g_CharacterList.push_back(list0);

    TCharacter* list1 = new TNonPersonCharacter;
    list1 = new TPersonCharacter;
    list1->SetPos({ 50,50 }); // TPoint a = {3,3};
    g_CharacterList.push_back(list1);

    TCharacter* list2 = new TNonPersonCharacter;
    list2 = new TNonPersonCharacter;
    list2->SetPos({ 70,70 });
    g_CharacterList.push_back(list2);

    // std::vector 인덱싱 지원
    for (int i=0; i < g_CharacterList.size(); i++)
    {
        g_CharacterList[i]->SetHealth(50);
        TNonPersonCharacter* pc1 = dynamic_cast<TNonPersonCharacter*>(g_CharacterList[i]);
        TPersonCharacter* pc2 = dynamic_cast<TPersonCharacter*>(g_CharacterList[i]);

        TNonPersonCharacter* pc3 = static_cast<TNonPersonCharacter*>(g_CharacterList[i]);
        TPersonCharacter* pc4 = static_cast<TPersonCharacter*>(g_CharacterList[i]);

        if (pc2 != nullptr)
        {
            pc2->SetExperience(5);
        }
    }

    for (int i = 0; i < g_CharacterList.size(); i++)
    {
        std::cout << g_CharacterList[i];
    }
    for (int i = 0; i < g_CharacterList.size(); i++)
    {
        delete g_CharacterList[i];
    }

    g_CharacterList.clear();
    std::cout << "Hello World!\n";
}
