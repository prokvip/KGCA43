#include "Parent.h"
Parent::Parent(int m)
{
	std::cout << "Parent 생성자" << std::endl;
	money = m;	// 십억 원
}
Parent::Parent(void)
{
	std::cout << "Parent 생성자" << std::endl;
	money = 1000000000;	// 십억 원
}
Parent::~Parent(void)
{
	std::cout << "Parent 해제자" << std::endl;
}
void Parent::Character(void)
{
	std::cout << "차분한 성품" << std::endl;
}
void Parent::Appearance(void)
{
	cout << "잘생긴 외모" << endl;
}
void Parent::Wealth(void)
{
	cout << "재산: " << money << "원" << endl;
}
