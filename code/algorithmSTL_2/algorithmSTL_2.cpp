#include <iostream>
#include <functional> //  c+++11 함수포인터 기능
#include <random>
#include <chrono>  //  시간 측정 기능


class TParent
{
public:
    virtual int Add(int a, int b) { 
        return a + b; 
    }
    virtual int Show()
    {
        std::cout << "Show";
        return 1;
    }
    virtual int Test() const
    {
        std::cout << "Show";
        return 1;
    }
};
class TData : public TParent
{
public:
    virtual int Add(int a, int b) override { 
        return a + b; 
    } 
};


int (TData::*Fun2)(int, int);
int (*Fun)(int, int);
int Add(int a, int b)
{
    std::cout << a + b << std::endl;
    return a + b;
}
int main()
{
    /// <summary>
    /// std::function(함수포인터) + std::bind(인자값 연결 및 전달)
    /// </summary>
    /// <returns></returns>
    /// 클래스 멤버함수 및 상속(다형성지원)이 지원된다.
    /// 
    TParent p;    
    std::function<int(TParent&)> c4 = &TParent::Show;
    std::function<int(TParent&, int, int)> c5 = &TParent::Add;
    c4(p);
    c5(p, 3, 3);
    

    TData a,b;
    a.Add(1, 3);// ->TData::Add(TData & obj= a, 1, 3);
    b.Add(2, 4);// ->TData::Add(TData & obj= b, 2, 4);
    std::function<int()> c1 = std::bind(&TData::Add, a, 1,2);
    c1();
    std::function<int()> c2 = std::bind(&TData::Add, b, 1, 2);
    c2(); 
    

    TParent* pObj = new TData;
    pObj->Add(3,4);//TData::Add(TData * obj= a, 1, 3);
   
    std::function<int (TParent*, int, int)> c3 = &TParent::Add;
    c3(pObj,4,5); // 다형성 지원
    delete pObj;

    TParent* pObj2 = new TParent;
    std::function<int()> c6 = std::bind(&TParent::Add, pObj2, 1, 2);
    c6();
    // 추론가능
    auto c11 = std::bind(&TParent::Add, pObj2, 1, 2);
    c11();
    delete pObj2;

    /// <summary>
    /// 일반함수 사용
    /// </summary>
    /// <returns></returns>
    std::function<int(int, int)> f1 = Add;
    Add(1, 2);
    f1(1, 2);

    std::function<int()> f2 = std::bind(Add,2,3);
    f2();
    //f2(3,4); //error


    auto f3 = std::bind(Add, 3, 4);
    f3();

    auto f4 = std::bind(Add, 3, std::placeholders::_1);
    f4(4);

    auto f5 = std::bind(Add, std::placeholders::_1, // a
                             std::placeholders::_2);// b
    //f5(8); // error
    f5(4,5);

    auto f6 = std::bind(Add, std::placeholders::_2, // b
                             std::placeholders::_1);// a
    f6(4, 5);

    auto f7 = std::bind(Add, std::placeholders::_2, // b
                             10);// a
    f7(4, 5); //a=5, b=10

    auto f8 = std::bind(Add, std::placeholders::_1, // b
        10);// a
    f8(4, 5); //a=5, b=10

    auto f9 = std::bind(Add, 10,
                        std::placeholders::_1);
    f9(4, 5); //a=5, b=10

    std::cout << "Hello World!\n";
}
