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
    //intList.reserve(10); // 메모리 확보(예약)한다.
    //intList.resize(3);  // 메모리 생성한다.(중요, 객체가 들어 있다.)

    // 객체의 생성자를 통하여 생성된다.
    // 메모리 확보되어 있으면 복사를 안한다.
    intList.emplace_back(7);
    intList.emplace_back(9);
    intList.emplace_back( 44, 3.14f );       
    intList.emplace(intList.end(),  55, 2.234f );


    std::vector<TCharacter> intListCopy;
    intListCopy = intList;

    std::vector<TCharacter> intListMoveCopy;
    // intList가 사용하는 모든 메모리의 주소가 intListMoveCopy 주소가 대입된다.
    // 메모리의 위치는 변경되지 않는다.
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

    // 배열(인덱싱지원) -> 메모리 리사이즈(동적배열)가 가능하다.
    std::vector<TCharacter> intList2;
    for (int i = 0; i < 10; i++)
    {
        TCharacter ch(rand()%100);  // 기본생성자 호출
        intList2.push_back(ch);     // 복사생성자 호출
        
        TCharacter ch1(ch);
        FunCopy(ch1);
        TCharacter ch2 = ch;
        FunRef(ch2);
        TCharacter ch3;
        ch3 = ch;    // 생성된 객체에 대입연산자 호출    
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


    // 순회방법들.
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
        intList.pop_back(); // 빅오(1)
        chRef = intList.back();
        intList.pop_back(); // 빅오(1)
        // [0][1][2] -> [1][2]
        //intList.pop_front();//없다.빅오(n)
    }
    for (int i = 0; i < intList.size(); i++)
    {
        std::cout<< intList[i].jValue <<" ";
    }


    intList.clear();
}
