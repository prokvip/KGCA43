#define USED_MAP
#undef USED_MAP

#include "TStd.h"
#include "TNonPersonCharacter.h"
#include "TPersonCharacter.h"

std::wstring g_FindName;
bool CheckName(TCharacter* p)
{
    if (p->m_csName == g_FindName)
    {
        return true;
    }
    return false;
}
class TEnemy
{
public:
    TEnemy() {}
};
int main()
{
#ifdef USED_MAP
    std::map<std::wstring, TCharacter*> g_CharacterList;
#else
    std::vector<TCharacter*> g_CharacterList;
#endif
    TCharacter* findObjAdd = nullptr;
    for (int i = 0; i < 10; i++)
    {        
        TCharacter* list0 = new TNonPersonCharacter;
        if (i == 0)
        {
            findObjAdd = list0;
        }
        TPoint p = { 30 * i,30 * i };
        list0->SetPos(p);
        list0->m_csName = L"000" + std::to_wstring(i);
#ifndef USED_MAP
        g_CharacterList.push_back(list0);
#else
        g_CharacterList.insert(std::make_pair(list0->m_csName, list0));
#endif
    }

#ifdef USED_MAP
    auto bFindObj = g_CharacterList.find(L"0002");
    if (bFindObj != g_CharacterList.end())
    {
        std::cout << " find=" << bFindObj->second;
    }
#else
    g_FindName = L"0002";
    auto bFindObj = std::find_if(  g_CharacterList.begin(),
                                g_CharacterList.end(), CheckName);
    if (bFindObj != g_CharacterList.end())
    {
        std::cout << " find=" << *bFindObj;
    }
#endif

#ifdef USED_MAP
    for (std::pair<const std::wstring, TCharacter*>& data : g_CharacterList)
#else
    for (auto pNode : g_CharacterList)
#endif
    {
#ifdef USED_MAP
        TCharacter* pNode = data.second;
        pNode->SetHealth(50);
#else
        pNode->SetHealth(50);
#endif
        TNonPersonCharacter* pc1 = dynamic_cast<TNonPersonCharacter*>(pNode);
        TPersonCharacter* pc2 = dynamic_cast<TPersonCharacter*>(pNode);

        TNonPersonCharacter* pc3 = static_cast<TNonPersonCharacter*>(pNode);
        TPersonCharacter* pc4 = static_cast<TPersonCharacter*>(pNode);

        if (pc2 != nullptr)
        {
            pc2->SetExperience(5);
        }
    }

#ifdef USED_MAP
    for (auto& pNode : g_CharacterList)
    {
        std::cout << pNode.second << std::endl;
    }
    for (auto& pNode : g_CharacterList)
    {
        delete pNode.second;
    }
#else
    for (auto pNode : g_CharacterList)
    {
        std::cout << pNode << std::endl;
    }
    for (auto pNode : g_CharacterList)
    {
        delete pNode;
    }
#endif   

    std::cout << "Hello World!\n";
}
