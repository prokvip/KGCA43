#pragma once
#include "Parent.h"
class Child :   public Parent
{
public:
	void Humanity(void);		// �߰��� ����Լ�
	void Character(void);		// ��ǰ ���
	void Appearance(void);		// Ȯ��� ����Լ�
public:
	Child();
	~Child();
	//Child(const Child& copy) {}
};
class Child1 : public Parent
{
public:
	void Humanity(void);		// �߰��� ����Լ�
	void Character(void);		// ��ǰ ���
	void Appearance(void);		// Ȯ��� ����Լ�
public:
	Child1();
	~Child1();
	//Child(const Child& copy) {}
};

