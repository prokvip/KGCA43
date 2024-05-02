// Class_2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

class Employee {
public:
    virtual int GetSalaly() { return 0; };
    virtual ~Employee() {}
};
class EmployeeHourly : public Employee {
public:
    virtual int GetSalaly() override { return 1; };
};
class EmployeeMontyly : public Employee {
public:
    virtual int GetSalaly()override { return 2; };
};
class EmployeeYearly : public Employee {
public:
    virtual int GetSalaly()override { return 3; };
};

// 직원(시급,월급,연봉)

int main()
{
    Employee* em[3];
    em[0] = new EmployeeHourly;
    em[1] = new EmployeeMontyly;
    em[2] = new EmployeeYearly;
    for (int i = 0; i < 3; i++)
    {
        em[i]->GetSalaly();
    }
    for (int i = 0; i < 3; i++)
    {
        delete em[i];
    }
    std::cout << "Hello World!\n";
}
