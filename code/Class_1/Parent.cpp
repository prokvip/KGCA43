#include "Parent.h"
Parent::Parent(int m)
{
	std::cout << "Parent ������" << std::endl;
	money = m;	// �ʾ� ��
}
Parent::Parent(void)
{
	std::cout << "Parent ������" << std::endl;
	money = 1000000000;	// �ʾ� ��
}
Parent::~Parent(void)
{
	std::cout << "Parent ������" << std::endl;
}
void Parent::Character(void)
{
	std::cout << "������ ��ǰ" << std::endl;
}
void Parent::Appearance(void)
{
	cout << "�߻��� �ܸ�" << endl;
}
void Parent::Wealth(void)
{
	cout << "���: " << money << "��" << endl;
}
