#pragma once
#include "TCore.h"
#include "TGame.h"
#include "TFiniteState.h"
class Sample : public TCore
{
	TGameObj		   m_Game;	
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};