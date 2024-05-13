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
    std::cout << src.jValue;
}
void  FunRef(TCharacter& src)
{
    std::cout << src.jValue;
}
void  FunRef(TCharacter&& src)
{
    std::cout << src.jValue;
}
void  FunPoint(TCharacter* src)
{
    std::cout << src->jValue;
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

    std::vector<TCharacter> intList;    
    //intList.reserve(10); // �޸� Ȯ��(����)�Ѵ�.
    //intList.resize(3);  // �޸� �����Ѵ�.(�߿�, ��ü�� ��� �ִ�.)

    // ��ü�� �����ڸ� ���Ͽ� �����ȴ�.
    // �޸� Ȯ���Ǿ� ������ ���縦 ���Ѵ�.
    intList.emplace_back(7);
    intList.emplace_back(9);
    intList.emplace_back( 44, 3.14f );       
    intList.emplace(intList.end(),  55, 2.234f );


    std::vector<TCharacter> intListCopy;
    intListCopy = intList;

    std::vector<TCharacter> intListMoveCopy;
    // intList�� ����ϴ� ��� �޸��� �ּҰ� intListMoveCopy �ּҰ� ���Եȴ�.
    // �޸��� ��ġ�� ������� �ʴ´�.
    intListMoveCopy = std::move(intList);

    /*TCharacter A1(100);    
    TCharacter A2 = A1;
    TCharacter A3 = std::move(A1);
    TCharacter A4(200);
    A4 = FunRet();    */

    

    intList.erase(--intList.end());
    intList.erase(intList.begin());
    intList.erase(std::begin(intList));


    std::vector<TCharacter> intList3(5);

    // �迭(�ε�������) -> �޸� ��������(�����迭)�� �����ϴ�.
    std::vector<TCharacter> intList2;
    for (int i = 0; i < 10; i++)
    {
        TCharacter ch(rand()%100);  // �⺻������ ȣ��
        intList2.push_back(ch);     // ��������� ȣ��
        
        TCharacter ch1(ch);
        FunCopy(ch1);
        TCharacter ch2 = ch;
        FunRef(ch2);
        TCharacter ch3;
        ch3 = ch;    // ������ ��ü�� ���Կ����� ȣ��    
        FunPoint(&ch3);
    }

    

    TCharacter ch7(rand() % 100);
    intList.insert(intList.begin(), { ch7,ch7,ch7,ch7 });
    intList.insert(intList.begin(), 
        intList2.begin(), 
        intList2.end());

    //typedef std::vector<TCharacter>::iterator cIter;
    using Iter = std::vector<TCharacter>::iterator;
    //Iter iterFind = intList.begin();
    TCharacter chInsert(99);
    TCharacter ch8(rand() % 100);
    Iter currentIter = intList.insert(intList.begin(), chInsert);
    intList.insert(currentIter, { ch8,ch8,ch8 });


    // ��ȸ�����.
    for (int i = 0; i < intList.size(); i++)
    {
        std::cout << intList[i].jValue << " ";
    }
   
    for (Iter iter = intList.begin();
          iter != intList.end();
          iter++)
    {
        TCharacter& ch = *iter;
        std::cout << ch.jValue << " ";
    }
    using rIter = std::vector<TCharacter>::reverse_iterator;
    for (rIter iter = intList.rbegin();
        iter != intList.rend();
        iter++)
    {
        TCharacter& ch = *iter;
        std::cout << ch.jValue << " ";
    }
    using cIter = std::vector<TCharacter>::const_iterator;
    for (cIter iter = intList.begin();
        iter != intList.end();
        iter++)
    {
        const TCharacter& ch = *iter;
        std::cout << ch.jValue << " ";
    }
    for (cIter cIter2 = intList.cbegin();
        cIter2 != intList.cend();
        cIter2++)
    {
        const TCharacter& ch = *cIter2;
        std::cout << ch.jValue << " ";
    }
    using rcIter = std::vector<TCharacter>::const_reverse_iterator;
    for (rcIter iter = intList.crbegin();
        iter != intList.crend();
        iter++)
    {
        const TCharacter& ch = *iter;
        std::cout << ch.jValue << " ";
    }
    for (TCharacter ch : intList)
    {
        ch.jValue = 99;
        std::cout << ch.jValue << " ";
    }
    for (TCharacter& ch : intList)
    {
        ch.jValue = ch.jValue+100;
        std::cout << ch.jValue << " ";
    }
    for (auto ch : intList)
    {
        ch.jValue = 99;
        std::cout << ch.jValue << " ";
    }
    for (auto& ch : intList)
    {
        ch.jValue = 99;
        std::cout << ch.jValue << " ";
    }

    for (int i = 0; i < intList.size(); i++)
    {
        TCharacter& chRef = intList.front();
        intList.pop_back(); // ���(1)
        chRef = intList.back();
        intList.pop_back(); // ���(1)
        // [0][1][2] -> [1][2]
        //intList.pop_front();//����.���(n)
    }
    for (int i = 0; i < intList.size(); i++)
    {
        std::cout<< intList[i].jValue <<" ";
    }


    intList.clear();
}
