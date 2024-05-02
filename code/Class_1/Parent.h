#pragma once
#include <iostream>
using namespace std;
class Parent
{
public:
	Parent(void);			// 생성자
	Parent(int m);
	virtual ~Parent(void);			// 소멸자
public:
	virtual void Character(void);	// 성품 출력
	void Appearance(void);	// 외모 출력
	void Wealth(void);		// 재산 출력
private:
	int money;			// 돈 저장
};


