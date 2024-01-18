#pragma once
#include "TStrategy.h"
class TContext
{
	TStrategy* sA = nullptr; // 객체포함
public:
	void Set(TStrategy* s)
	{
		sA = s;
	}
	int operation()// 의존관계 역전 원칙
	{
		return sA->algorithm();
	}
	int operation(TStrategy* s)// 의존관계 역전 원칙
	{
		return s->algorithm();
	}
	TContext(TStrategy* s) : sA(s)
	{

	}
	TContext() = default;
};

