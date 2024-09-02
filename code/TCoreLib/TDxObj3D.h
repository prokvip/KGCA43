#pragma once
#include "TDxObject.h"
class TDxObject3D : public TDxObject
{
public:
	T::TMatrix		 m_matWorld;
	T::TMatrix		 m_matView;
	T::TMatrix		 m_matProj;
	T::TVector3		 m_vPos;
	T::TVector3		 m_vScale;
	T::TVector3		 m_vRotate;
public:
	std::vector<PNCT_Vertex>	m_vVertexList;		 // 프레임 화면 정보
	std::vector<DWORD>			m_vIndexList;
public:
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(std::wstring texName,	std::wstring hlsl) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool	 CreateConstantBuffer(ID3D11Device* pd3dDevice) override;
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void	 SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
};