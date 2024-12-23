#pragma once
#include "TDxObject.h"
class TDxObject3D : public TDxObject
{
public:
	T::TMatrix		 m_matInitWorld;
	T::TMatrix		 m_matWorld;
	T::TMatrix		 m_matView;
	T::TMatrix		 m_matProj;
	T::TVector3		 m_vPos;
	T::TVector3		 m_vScale;
	T::TVector3		 m_vRotate;
	T::TMatrix		 m_matParentWorld;
	TDxObject3D*	 m_pParentObject = nullptr;
	void			 SetParentObject(TDxObject3D* pParent)
	{
		_ASSERT(pParent);
		m_pParentObject = pParent;
		m_matParentWorld = pParent->m_matWorld;
	}
public:
	std::vector<PNCT_Vertex>	m_vVertexList;		 // 프레임 화면 정보
	std::vector<DWORD>			m_vIndexList;
public:
	
public:
	virtual void   LoadTexture(std::wstring szPath) override;	
	virtual bool   Create(std::wstring texName,	std::wstring hlsl) override;	
	virtual bool   CreateIWVertexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice) override;
	virtual bool	 CreateConstantBuffer(ID3D11Device* pd3dDevice) override;
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);
public:
	virtual void	 UpdateVertexBuffer();
	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void	 SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
};