
#include "TLinkedList.h"

struct TStudent
{
    int iID;
    int iKor;
    TStudent&  operator << (int i)
    {
        std::cout << iKor+i << std::endl;
        return *this;
    }
    TStudent& operator << (float i)
    {
        std::cout << iKor + i << std::endl;
        return *this;
    }
    /*friend std::ostream& operator << (std::ostream& os, 
        const TStudent& t)
    {
        os << t.iID << " "<< t.iKor << std::endl;
        return os;
    }*/
};
// 리스트 형태라면 고려사항
// - 1출력, 2순회, 3생성(연결), 4삭제, 
// - 5추가(임의의 위치), 6수정, 7검색, 8정렬
// 제한사항(사용하기 불편하다.) : 
// 재사용성 고려.
TLinkedList<TStudent> list;

int main()
{    
    TStudent a0;
    a0.iID = 999;
    a0.iKor = 100;
    a0 << 1 << 11.0f;

    long fPI = 100;
    for (int i = 0; i < 5; i++)
    {
        TStudent tStudent;
        tStudent.iID = i;
        tStudent.iKor = i * 10;
        //list.BackwardAdd(list.NewNode(i*100));
        list.ForwardAdd(list.NewNode(tStudent));
    }
    list.Show();
    list.ReverseShow();
    std::cout << "End!\n";
}
