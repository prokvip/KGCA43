#include "Child.h"
Child::Child() : Parent(433333)
{
	std::cout << "Child ������" << std::endl;
};

Child::~Child()
{
	std::cout << "Child ������" << std::endl;
}
void Child::Humanity(void)
{
	cout << "��ġ�� �ΰ���" << endl;
}

void Child::Character(void)
{
	cout << "�� ���� ��ǰ" << endl;
}

void Child::Appearance(void)
{
	Parent::Appearance();		// ��� Ŭ�������� ������ ���
	cout << "��ĥ�� Ű" << endl; 	// �Ļ� Ŭ�������� Ȯ��� ���
}


/// <summary>
/// 
/// </summary>
Child1::Child1() : Parent(433333)
{
	std::cout << "Child ������" << std::endl;
};

Child1::~Child1()
{
	std::cout << "Child ������" << std::endl;
}
void Child1::Humanity(void)
{
	cout << "��ġ�� �ΰ���" << endl;
}

void Child1::Character(void)
{
	cout << "�� ���� ��ǰ" << endl;
}

void Child1::Appearance(void)
{
	Parent::Appearance();		// ��� Ŭ�������� ������ ���
	cout << "��ĥ�� Ű" << endl; 	// �Ļ� Ŭ�������� Ȯ��� ���
}
