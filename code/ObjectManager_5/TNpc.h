#pragma once
#include "TPawn.h"
class TNpc : public TPawn
{
public:
	virtual void     Frame();
	void	SetVertexData(RECT rt) override;
	TNpc();
};