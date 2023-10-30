#include <iostream>
#include <list>
#include "TPersonCharacter.h"
#include "TNonPersonCharacter.h"
int main()
{
    /// <summary>
  /// list 1
  /// </summary>
  /// <returns></returns>
    std::list<int> list;
    for (int i = 0; i < 10; i++)
    {
        list.push_back(i);
    }
    // 리스트는 인덱싱 지원 불가(배열접근 불가능하다.)
  /*  for (int i = 0; i < list.size(); i++)
    {
        std::cout << list[i] << " ";
    }*/
    // list.begin() -> 실제 첫 데이터 저장.
    // list.end() -> 가상의 꼬리 노드
    for (std::list<int>::iterator iter = list.begin();
        iter != list.end();
        iter++)
    {
        int iValue = *iter;//TNode<>
        std::cout << iValue << " ";
    }
    list.clear();
    /// <summary>
    /// list2 
    /// </summary>
    /// <returns></returns>
    std::list<TCharacter> listChar;
    for (int i = 0; i < 5; i++)
    {
        TCharacter ch;
        TPoint p = { 30 * i,30 * i };
        ch.SetPos(p);
        listChar.push_back(ch);
    }
    for (std::list<TCharacter>::iterator iter = listChar.begin();
        iter != listChar.end();
        iter++)
    {
        TCharacter iValue = *iter;//TNode<>
        std::cout << iValue << " ";
    }
    listChar.clear();

    /// <summary>
    /// list3 
    /// </summary>
    /// <returns></returns>
    std::list<TCharacter*> listCharPoint;
    for (int i = 0; i < 5; i++)
    {
        TCharacter* ch = new TCharacter();
        TPoint p = { 30 * i,30 * i };
        ch->SetPos(p);
        listCharPoint.push_back(ch);
    }
    
    for (std::list<TCharacter*>::iterator iter = listCharPoint.begin();
        iter != listCharPoint.end();
        iter++)
    {
        TCharacter* iValue = *iter;//TNode<>
        std::cout << iValue << " ";
    }
    for (std::list<TCharacter*>::iterator iter = listCharPoint.begin();
        iter != listCharPoint.end();
        iter++)
    {
        TCharacter* pChar = *iter;
        delete pChar;
    }
    listCharPoint.clear();
    std::cout << "Hello World!\n";
}
