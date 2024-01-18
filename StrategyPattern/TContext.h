#pragma once
#include "TComponent.h"
class TContext
{
	TComponent* sA = nullptr; // 객체포함
public:
	void Set(TComponent* s)
	{
		sA = s;
	}
	void operation()// 의존관계 역전 원칙
	{
		return sA->Operation();
	}
	void operation(TComponent* s)// 의존관계 역전 원칙
	{
		return s->Operation();
	}
	TContext(TComponent* s) : sA(s)
	{

	}
	TContext() = default;
};

