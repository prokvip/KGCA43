#include <iostream>
#include <map>
#include "TPersonCharacter.h"
#include "TNonPersonCharacter.h"

int main()
{
    /// <summary>
 /// map 1
 /// </summary>
 /// <returns></returns>
    std::map<int, int> list;
    for (int key = 0; key < 5; key++)
    {
        list[key] = key *100;
    }
    for (int key = 5; key < 10; key++)
    {
        std::pair<int, int> data;
        data.first = key;
        data.second = key*100;
        list.insert(data);
    }
    // 맵는 인덱싱 지원(배열접근 가능하다.)
    for (int i = 0; i < list.size(); i++)
    {
        std::cout << list[i] << " ";
    }
    // list.begin() -> 실제 첫 데이터 저장.
    // list.end() -> 가상의 꼬리 노드
    for (std::map<int,int>::iterator iter = list.begin();
        iter != list.end();
        iter++)
    {
        std::pair<int, int> data = *iter;        
        std::cout << data.first << " "
                  << data.second;
    }
    list.clear();
    /// <summary>
    /// list2 
    /// </summary>
    /// <returns></returns>
    std::map<int, TCharacter> listChar;
    for (int i = 0; i < 5; i++)
    {
        TCharacter ch;
        TPoint p = { 30 * i,30 * i };
        ch.SetPos(p);
        std::pair<int, TCharacter> data;
        data.first = i;
        data.second = ch;
        listChar.insert(data);
    }
    for (std::map<int,TCharacter>::iterator iter = listChar.begin();
        iter != listChar.end();
        iter++)
    {
        std::pair<int, TCharacter> data = *iter;
        std::cout << data.first << " "
            << data.second;
    }

    using TCHARACTER = std::map<int, TCharacter>;
    TCHARACTER::iterator findData = listChar.find(30);
    if (listChar.end() != findData)
    {
        std::cout << findData->first << " "
            << findData->second;
    }
    listChar.clear();

    /// <summary>
    /// list3 
    /// </summary>
    /// <returns></returns>
    std::map<int,TCharacter*> listCharPoint;
    for (int i = 0; i < 5; i++)
    {
        TCharacter* ch = new TCharacter();
        TPoint p = { 30 * i,30 * i };
        ch->SetPos(p);
        listCharPoint.insert(std::make_pair(i, ch));
    }

    for (std::map<int, TCharacter*>::iterator iter = listCharPoint.begin();
        iter != listCharPoint.end();
        iter++)
    {
        int  iKey = iter->first;
        TCharacter* iValue = iter->second;
        std::cout << iKey << " " << iValue << " ";
    }
    for (std::map<int, TCharacter*>::iterator iter = listCharPoint.begin();
        iter != listCharPoint.end();
        iter++)
    {
        TCharacter* pChar = iter->second;
        delete pChar;
    }
    listCharPoint.clear();
    std::cout << "Hello World!\n";
}
