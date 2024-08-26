#pragma once
#include "TDxObject.h"
using namespace T_Math;
struct cb // 상수버퍼
{
	FMatrix g_matWorld;
	FMatrix g_matView;
	FMatrix g_matProj;
};

class TPlaneObj : public TDxObject3D
{
	ComPtr<ID3D11Buffer>   m_pConstantBuffer;
	cb					   m_cb;
	FMatrix					m_matWorld;
	FMatrix					m_matView;
	FMatrix					m_matProj;
public:
	FVector3				m_vPos;
public:
	virtual bool		CreateConstantBuffer(ID3D11Device* pd3dDevice);
	virtual void		SetVertexData();
	virtual bool		Create(std::wstring texName, std::wstring hlsl);
	virtual void		PreRender(ID3D11DeviceContext* pContext);
	virtual void		Frame() override;
};
