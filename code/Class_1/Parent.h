#pragma once
#include <iostream>
using namespace std;
class Parent
{
public:
	Parent(void);			// ������
	Parent(int m);
	virtual ~Parent(void);			// �Ҹ���
public:
	virtual void Character(void);	// ��ǰ ���
	void Appearance(void);	// �ܸ� ���
	void Wealth(void);		// ��� ���
private:
	int money;			// �� ����
};


