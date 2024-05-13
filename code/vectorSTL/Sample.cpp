#include "TDataManager.h"
#include <vector> // 
#include <iterator> 

/// <summary>
/// 
//�̵������� : ���۾� �ʿ�(���� �����ϰ� ���� �޸� �ʱ�ȭ) 
// ���� �����͸� �������� �ʰ� ���� ��ü�� �������� 
// �ű� ��ü�� ���ο� ������ �̵��ϴ� Ư�� ��� �Լ��̴�.

//�̵������ڴ� �ش� ��ü�� �ʿ���� ��(���Ÿ� ������ ��)
// �̵� ������ ������ -> ���� ������( ȣ��ȴ�.
//             ������ �Ǵ� �̵� ���� ������ -> ���� �����ڴ� ������ ����.

/// </summary>
enum WORK {
    SCREEN_PRINT = 0,
    NEW_DATA,
    FILE_SAVE,
    FILE_LOAD,
    DATA_UPDATE,
    DEL_DATA,
    SORT_DATA,
    CREATE_SAMPLE_DATA = 9,
    EXIT = 99
};


void TitlePrint()
{
    printf("\n<<--------(����)�������α׷�-------->>\n");
}
void MenuPrint()
{
    printf("\n���(0),�ű�(1),����(2),�ε�(3),����(4),����(5),����(6),���ÿ�(9),����(99)");
}

void  FunCopy(TCharacter src )
{
    std::cout << src.jValue << std::endl;
}
void  FunRef(TCharacter& src)
{
    std::cout << src.jValue << std::endl;
}
void  FunRef(TCharacter&& src)
{
    std::cout << src.jValue << std::endl;
}
void  FunPoint(TCharacter* src)
{
    std::cout << src->jValue << std::endl;
}
TCharacter  FunRet()
{
    TCharacter ret(1111);
    return ret;
}

void FunText(int& value)
{

}
void FunText(int&& value)
{

}
int main()
{   
    {
            int a = 7;
            FunText(a);
            FunText(4);
            TCharacter A2;
            FunRef(A2);
            FunRef(TCharacter(555));
            FunRef(FunRet());
            FunRef(std::move(A2));
            FunRef(666);
            FunRef({ 666 });

            TCharacter t1(100);
            TCharacter t2 = t1;
            TCharacter t3 = std::move(t1);
            TCharacter t4(200);
            t4 = FunRet();  
    }
    {
        std::vector<TCharacter> intList;
        intList.reserve(10); // �޸� Ȯ��(����)�Ѵ�.
        //intList.resize(3);  // �޸� �����Ѵ�.(�߿�, ��ü�� ��� �ִ�.)
        intList.push_back(1); // ���� �� �̵��Ѵ�.
        intList.push_back(2);
        intList.push_back({ 3, 3.14f });
        // ��ü�� �����ڸ� ���Ͽ� �����ȴ�.
        // �޸� Ȯ���Ǿ� ������ ���縦 ���Ѵ�.
        intList.emplace_back(4); // ������ �Ѵ�.
        intList.emplace_back(5);
        intList.emplace_back(6, 3.14f);
        intList.emplace(intList.end(), 7, 2.234f);

        std::vector<TCharacter> intListMoveCopy;
        // intList�� ����ϴ� ��� �޸��� �ּҰ� intListMoveCopy �ּҰ� ���Եȴ�.
        // �޸��� ��ġ�� ������� �ʴ´�.
        intListMoveCopy = std::move(intList);
        intListMoveCopy.clear();

        intList.clear();
    }
    {
        ////////////////////////////////////////////////////////////////////////
        // �迭(�ε�������) -> �޸� ��������(�����迭)�� �����ϴ�.
        std::vector<TItem> intList2;
        for (int i = 0; i < 10; i++)
        {
            TItem ch(rand() % 100);  // �⺻������ ȣ��
            intList2.push_back(ch);     // ��������� ȣ��
        }
        intList2.erase(--intList2.end());
        intList2.erase(intList2.begin());
        intList2.erase(std::begin(intList2));

        std::vector<TItem> intList3;
        TItem ch7(rand() % 100);
        intList3.insert(intList3.begin(), { ch7, ch7 });
        intList3.insert(intList3.begin(), intList2.begin(), intList2.end());


        //typedef std::vector<TItem>::iterator cIter;
        using Iter = std::vector<TItem>::iterator;
        //Iter iterFind = intList3.begin();
        TItem chInsert(99);
        TItem ch8(rand() % 100);
        Iter currentIter = intList3.insert(intList3.begin(), chInsert);
        intList3.insert(currentIter, { ch8,ch8,ch8 });


        // ��ȸ�����.
        std::cout << std::endl;
        for (int i = 0; i < intList3.size(); i++)
        {
            std::cout << intList3[i].iValue << " ";
        }
        std::cout << std::endl;
        for (Iter iter = intList3.begin();
            iter != intList3.end();
            iter++)
        {
            TItem& ch = *iter;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        using rIter = std::vector<TItem>::reverse_iterator;
        for (rIter iter = intList3.rbegin();
            iter != intList3.rend();
            iter++)
        {
            TItem& ch = *iter;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        using cIter = std::vector<TItem>::const_iterator;
        for (cIter iter = intList3.begin();
            iter != intList3.end();
            iter++)
        {
            const TItem& ch = *iter;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (cIter cIter2 = intList3.cbegin();
            cIter2 != intList3.cend();
            cIter2++)
        {
            const TItem& ch = *cIter2;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        using rcIter = std::vector<TItem>::const_reverse_iterator;
        for (rcIter iter = intList3.crbegin();
            iter != intList3.crend();
            iter++)
        {
            const TItem& ch = *iter;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (TItem ch : intList3)
        {
            ch.iValue = 99;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (TItem& ch : intList3)
        {
            ch.iValue = ch.iValue + 100;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (auto ch : intList3)
        {
            ch.iValue = 99;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (auto& ch : intList3)
        {
            ch.iValue = 99;
            std::cout << ch.iValue << " ";
        }
        std::cout << std::endl;
        for (int i = 0; i < intList3.size(); i++)
        {
            TItem& chRef = intList3.front();
            intList3.pop_back(); // ���(1)
            chRef = intList3.back();
            intList3.pop_back(); // ���(1)
            // [0][1][2] -> [1][2]
            //intList3.pop_front();//����.���(n)
        }
        std::cout << std::endl;
        for (int i = 0; i < intList3.size(); i++)
        {
            std::cout << intList3[i].iValue << " ";
        }


        intList3.clear();
    }
}
