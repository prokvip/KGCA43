#pragma once
#include "TGameCore.h"
class Sample : public TGameCore
{	
	TSound* m_pBKSound = nullptr;
	TSound* m_pEffectA = nullptr;
public:
	bool  Init();	
	bool  Render();
	bool  Release();
};