#include "Child.h"

int main()
{
    {
        Parent* c[2];
        c[0] = new Child;
        c[1] = new Child1;
        
        for (int i = 0; i < 2; i++)
        {
            c[i]->Character();
        }
        delete c[0];
        delete c[1];        
    }
    //Parent p1;//error C2259: 'Parent': 추상 클래스를 인스턴스화할 수 없습니다.
    {
        Parent c;
        //c.money = d;
        c.Appearance();
        //error C2259 : 'Child' : 추상 클래스를 인스턴스화할 수 없습니다.
        // 1 > F:\00_43\code\Class_1\Class_1.cpp(7, 15) : message: 'void Parent::Character(void)' : abstract입니다.
        c.Character();

        c.Wealth();
        //c.Humanity();
    }
    //{
    //    Parent* p = new Parent;
    //    (*p).Appearance();
    //    p->Character();
    //    p->Wealth();
    //    delete p;
    //}

    //{
    //    
    //        Parent p;
    //        p.Appearance();
    //        p.Character();
    //        p.Wealth();
    //        /*Parent* p = new Parent;
    //        (*p).Appearance();
    //        p->Character();
    //        p->Wealth();*/
    //    
    //}
    std::cout << "Hello World!\n";
}
