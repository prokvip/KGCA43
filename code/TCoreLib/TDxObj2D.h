#pragma once
#include "TDxObject.h"
class TDxObject2D : public TDxObject
{
public:
	std::vector<P2C4T2_Vertex>  m_vListScreen; // 초기 화면 정보
	std::vector<P2C4T2_Vertex>  m_vVertexList;		 // 프레임 화면 정보
	std::vector<P2C4T2_Vertex>  m_vListNDC;	 // NDC
public:
	T::TVector2 ConvertScreenToNDC(T::TVector2 v);
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(RECT rt, std::wstring texName,std::wstring hlsl) override;

	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	virtual bool     LoadShader(std::wstring filename);
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
};
