#pragma once
#include "TCore.h"
#include "TPlaneObj.h"

class Sample : public TCore
{
	TPlaneObj	m_obj;
	TPlaneObj	m_objFar;
	ComPtr<ID3D11DepthStencilView> m_pDSV = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthEnable = nullptr;
	ComPtr<ID3D11DepthStencilState> m_pDepthDisable = nullptr;
	HRESULT CreateDepthBuffer();
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};