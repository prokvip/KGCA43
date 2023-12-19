#pragma once
#include "TGameCore.h"
class Sample : public TGameCore
{	
	TSound* m_pBKSound = nullptr;
	TSound* m_pEffectA = nullptr;
	ID3D11PixelShader* m_pPixelShaderAlphaTest = nullptr;
	std::shared_ptr<TEffect>		m_EffectA;
	std::shared_ptr<TEffect>		m_EffectB;
public:
	bool  CreatePixelShader();
	bool  Init();	
	bool  Frame();
	bool  Render();
	bool  Release();
};