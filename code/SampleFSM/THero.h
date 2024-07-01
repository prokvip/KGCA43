#pragma once
#include "TPawn.h"
class THero : public TPawn
{
public:
	virtual void  Frame() override;
	virtual void  Front();
	virtual void  Back();
	virtual void  Left();
	virtual void  Right();
	void	SetVertexData(RECT rt) override;
	void    HitOverlap(TActor* pActor, DWORD dwState)override;
	THero();
};
