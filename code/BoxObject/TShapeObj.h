#pragma once
#include "TDxObject.h"
using namespace T_Math;
struct cb // 상수버퍼
{
	FMatrix g_matWorld;
	FMatrix g_matView;
	FMatrix g_matProj;
};

class TShapeObj : public TDxObject3D
{
public:
	cb			 m_cb;
	FMatrix		 m_matWorld;
	FMatrix		 m_matView;
	FMatrix		 m_matProj;
	FVector3	 m_vPos;
	FVector3	 m_vScale; 
	FVector3	 m_vRotate; 
	ComPtr<ID3D11Buffer>   m_pConstantBuffer;
public:
	virtual void SetMatrix(FMatrix* pWorld, FMatrix* pView, FMatrix* pProj);
	virtual bool CreateConstantBuffer(ID3D11Device* pd3dDevice);
	virtual bool Create(std::wstring texName, std::wstring hlsl);
	virtual void PreRender(ID3D11DeviceContext* pContext);
};

