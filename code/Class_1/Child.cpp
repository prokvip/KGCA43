#include "Child.h"
Child::Child() : Parent(433333)
{
	std::cout << "Child 생성자" << std::endl;
};

Child::~Child()
{
	std::cout << "Child 해제자" << std::endl;
}
void Child::Humanity(void)
{
	cout << "넘치는 인간미" << endl;
}

void Child::Character(void)
{
	cout << "불 같은 성품" << endl;
}

void Child::Appearance(void)
{
	Parent::Appearance();		// 기반 클래스에서 정의한 기능
	cout << "훤칠한 키" << endl; 	// 파생 클래스에서 확장된 기능
}


/// <summary>
/// 
/// </summary>
Child1::Child1() : Parent(433333)
{
	std::cout << "Child 생성자" << std::endl;
};

Child1::~Child1()
{
	std::cout << "Child 해제자" << std::endl;
}
void Child1::Humanity(void)
{
	cout << "넘치는 인간미" << endl;
}

void Child1::Character(void)
{
	cout << "불 같은 성품" << endl;
}

void Child1::Appearance(void)
{
	Parent::Appearance();		// 기반 클래스에서 정의한 기능
	cout << "훤칠한 키" << endl; 	// 파생 클래스에서 확장된 기능
}
