#include "TDataManager.h"
#include <vector> // 
#include <iterator> 

/// <summary>
/// 
//이동생성자 : 수작업 필요(기존 복수하고 기존 메모리 초기화) 
// 원래 데이터를 복사하지 않고 기존 객체의 소유권을 
// 신규 객체의 새로운 변수로 이동하는 특수 멤버 함수이다.

//이동생성자는 해당 객체가 필요없을 때(제거를 예상할 때)
// 이동 생성자 없으면 -> 복사 생성자( 호출된다.
//             있으면 또는 이동 대입 연산자 -> 복사 생성자는 삭제로 본다.

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
    printf("\n<<--------(점수)관리프로그램-------->>\n");
}
void MenuPrint()
{
    printf("\n출력(0),신규(1),저장(2),로드(3),수정(4),삭제(5),정렬(6),샘플용(9),종료(99)");
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
        intList.reserve(10); // 메모리 확보(예약)한다.
        //intList.resize(3);  // 메모리 생성한다.(중요, 객체가 들어 있다.)
        intList.push_back(1); // 생성 및 이동한다.
        intList.push_back(2);
        intList.push_back({ 3, 3.14f });
        // 객체의 생성자를 통하여 생성된다.
        // 메모리 확보되어 있으면 복사를 안한다.
        intList.emplace_back(4); // 생성만 한다.
        intList.emplace_back(5);
        intList.emplace_back(6, 3.14f);
        intList.emplace(intList.end(), 7, 2.234f);

        std::vector<TCharacter> intListMoveCopy;
        // intList가 사용하는 모든 메모리의 주소가 intListMoveCopy 주소가 대입된다.
        // 메모리의 위치는 변경되지 않는다.
        intListMoveCopy = std::move(intList);
        intListMoveCopy.clear();

        intList.clear();
    }
    {
        ////////////////////////////////////////////////////////////////////////
        // 배열(인덱싱지원) -> 메모리 리사이즈(동적배열)가 가능하다.
        std::vector<TItem> intList2;
        for (int i = 0; i < 10; i++)
        {
            TItem ch(rand() % 100);  // 기본생성자 호출
            intList2.push_back(ch);     // 복사생성자 호출
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


        // 순회방법들.
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
            intList3.pop_back(); // 빅오(1)
            chRef = intList3.back();
            intList3.pop_back(); // 빅오(1)
            // [0][1][2] -> [1][2]
            //intList3.pop_front();//없다.빅오(n)
        }
        std::cout << std::endl;
        for (int i = 0; i < intList3.size(); i++)
        {
            std::cout << intList3[i].iValue << " ";
        }


        intList3.clear();
    }
}
