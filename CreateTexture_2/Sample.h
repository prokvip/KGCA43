#pragma once
#include "TUiObj.h"

class Sample : public TDevice
{	
	ID3D11SamplerState* m_pDefaultSS;
	ID3D11SamplerState* m_pDefaultSSPoint;
	ID3D11BlendState* m_pAlphaBlendEnable;
	ID3D11BlendState* m_pAlphaBlendDisable;
	ID3D11PixelShader* m_pPixelShaderAlphaTest = nullptr;
	TUiObj			m_bk;
	TUiObj			m_SkillLayout;
	TUiObj			m_Skill2;

	TUiNumber		m_Number;
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
	bool    AlphaBlendState();
	bool	CreatePixelShader();
	bool	CreateSampleState();
};