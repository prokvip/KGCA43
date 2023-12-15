#pragma once
#include "TDevice.h"
#include "TUiObj.h"
#include "TInput.h"
#include "TTimer.h"
class Sample : public TDevice
{	
public:
	TTimer				m_GameTimer;
	ID3D11SamplerState* m_pDefaultSS;
	ID3D11SamplerState* m_pDefaultSSPoint;
	ID3D11BlendState* m_pAlphaBlendEnable;
	ID3D11BlendState* m_pAlphaBlendDisable;
	ID3D11PixelShader* m_pPixelShaderAlphaTest = nullptr;

	std::vector<std::shared_ptr<TUiObj>>   m_uiList;
	std::shared_ptr<TUiObj>			m_bk;
	std::shared_ptr<TUiObj>			m_SkillLayout;
	std::shared_ptr<TUiObj>			m_Skill2;
	std::shared_ptr<TUiObj>			m_Number;

	TPlaneShape		m_DefaultPlane;
public:
	bool    Init()		override;
	bool    Frame()		override;
	bool    Render()	override;
	bool    Release()	override;
	bool    AlphaBlendState();
	bool	CreatePixelShader();
	bool	CreateSampleState();
};