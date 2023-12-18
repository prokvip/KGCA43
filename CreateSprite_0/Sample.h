#pragma once
#include "TDevice.h"
#include "TUiObj.h"
#include "TInput.h"
#include "TTimer.h"
#include "TEffect.h"
#include "TDxWrite.h"
class Sample : public TDevice
{	
public:
	TDxWrite			m_dxWrite;
	TTimer				m_GameTimer;
	ID3D11SamplerState* m_pDefaultSS = nullptr;
	ID3D11SamplerState* m_pDefaultSSPoint = nullptr;
	ID3D11BlendState* m_pAlphaBlendEnable = nullptr;
	ID3D11BlendState* m_pAlphaBlendDisable = nullptr;
	ID3D11PixelShader* m_pPixelShaderAlphaTest = nullptr;

	std::vector<std::shared_ptr<TUiObj>>   m_uiList;
	std::shared_ptr<TUiObj>			m_bk;
	std::shared_ptr<TUiObj>			m_btnStart;
	std::shared_ptr<TUiObj>			m_Item;
	std::shared_ptr<TUiObj>			m_Number;
	TPlaneShape						m_DefaultPlane;
	std::shared_ptr<TEffect>		m_EffectA;
	std::shared_ptr<TEffect>		m_EffectB;
public:
	bool    Init()		override;
	bool    Frame()		override;
	bool    Render()	override;
	bool    Release()	override;
	bool    AlphaBlendState();
	bool	CreatePixelShader();
	bool	CreateSampleState();
};