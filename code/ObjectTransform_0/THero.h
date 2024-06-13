#pragma once
#include "TPawn.h"
class THero : public TPawn
{
public:
	virtual void  Frame();
	virtual void  Front();
	virtual void  Back();
	virtual void  Left();
	virtual void  Right();
	THero();
};
