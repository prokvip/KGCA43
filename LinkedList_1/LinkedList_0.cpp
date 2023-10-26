
#include "TLinkedList.h"

// 리스트 형태라면 고려사항
// - 1출력, 2순회, 3생성(연결), 4삭제, 
// - 5추가(임의의 위치), 6수정, 7검색, 8정렬
// 제한사항(사용하기 불편하다.) : 
// 재사용성 고려.
TLinkedList list;

int main()
{     
    
    for (int i = 0; i < 5; i++)
    {
        //list.BackwardAdd(list.NewNode(i*100));
        list.ForwardAdd(list.NewNode(i * 100));
    }
    list.Show();
    list.ReverseShow();
    std::cout << "End!\n";
}
