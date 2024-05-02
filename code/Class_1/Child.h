#pragma once
#include "Parent.h"
class Child :   public Parent
{
public:
	void Humanity(void);		// 추가된 멤버함수
	void Character(void);		// 성품 출력
	void Appearance(void);		// 확장된 멤버함수
public:
	Child();
	~Child();
	//Child(const Child& copy) {}
};
class Child1 : public Parent
{
public:
	void Humanity(void);		// 추가된 멤버함수
	void Character(void);		// 성품 출력
	void Appearance(void);		// 확장된 멤버함수
public:
	Child1();
	~Child1();
	//Child(const Child& copy) {}
};

