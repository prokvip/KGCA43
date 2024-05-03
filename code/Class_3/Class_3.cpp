#include <iostream>
class A {
private:
    A() {};
    ~A() {}
private:
    int value=8;
    friend class B;
};
// class A는 class B에 전용객체(B만 사용가능한 클래스)
class B {
public:
    B(int i) : value(i) {}
    //virtual ~B() {};
    int value = 0;
    int valueB = 0;
public:
    void show(A& obj)
    {
        A a;
        std::cout << a.value << obj.value;
    }
    B&  operator+(B& b)
    {        
        value += b.value;
        return *this;
    }
    B& operator*(int i)
    {
        value += i;
        return *this;
    }
    B& operator-(int i)
    {
        value -= i;
        return *this;
    }
    void*  operator new (size_t size)
    {
        return malloc(size);
    }
    void  operator delete (void *p)
    {
        free(p);
    }

    friend std::ostream& operator << (std::ostream& os, B& b)
    {
        os << b.value;
        return os;
    }

    int operator[] (int i)
    {
        if (i == 0) return value;
        return valueB;
    }
    B& operator() ()
    {
        return *this;
    }
};
int main()
{
    B b1(1);
    B b2(2);
    B b3(3);
    B b4(0);
    b4 = b1 + b2 + b3 * 4 - 3;

    int iRet = b4[0];
    iRet = b4[1];
    B& refRet = b4();
    iRet = refRet[0];
    // 가상함수 사용시 메모리 주소가 변한한다.
    //int* pPoint = (int*)(new B(9));
    //int a1 = pPoint[0];
    //int a2 = pPoint[1];
    B* pB = new B(9);

    std::cout << *pB << b1 << b2 << b3;



    
    

    //error C2248: 'A::A': private 멤버('A' 클래스에서 선언)에 액세스할 수 없습니다.
    //A aObj; // error
    //B bObj;
    //aObj.value = 4; // error
    //bObj.show(aObj);
    std::cout << "Hello World!\n";
}
