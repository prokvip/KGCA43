#pragma once
#include "TDevice.h"
#include "TUiObj.h"
#include "TInput.h"
#include "TTimer.h"
#include "TEffect.h"
#include "TDxWrite.h"
#include "TSoundMgr.h"

class TGameCore : public TDevice
{
public:
	TDxWrite			m_dxWrite;
	TTimer				m_GameTimer;
	ID3D11SamplerState* m_pDefaultSS = nullptr;
	ID3D11SamplerState* m_pDefaultSSPoint = nullptr;
	ID3D11BlendState* m_pAlphaBlendEnable = nullptr;
	ID3D11BlendState* m_pAlphaBlendDisable = nullptr;
	ID3D11BlendState* m_pDualSourceColorBlending = nullptr;
public:
	TPlaneShape						m_DefaultPlane;
public:
	bool	DualSourceColorBlending();
	bool    AlphaBlendState();
	bool	CreatePixelShader();
	bool	CreateSampleState();
public:
	virtual bool    Init() {
		return true;
	};
	virtual bool    Frame() {
		return true;
	};
	virtual bool    Render() {
		return true;
	};
	virtual bool    Release() {
		return true;
	};
	virtual bool	GameInit() override;
	virtual bool	GameFrame() override;
	virtual bool	GameRender()override;
	virtual bool	GameRelease()override;
};

