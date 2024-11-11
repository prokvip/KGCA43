#pragma once
#include "TBoxObj.h"
class TSkyObject
{
public:
	ComPtr<ID3D11RasterizerState>	m_pRSCullNone = nullptr;
	ComPtr<ID3D11SamplerState>		m_pSSPoint = nullptr;
	ComPtr<ID3D11SamplerState>		m_pSSLinear = nullptr;
public:
	HRESULT			SetSamplerState();
	HRESULT			SetRasterizerState();
	virtual bool   Create(std::wstring texName, std::wstring hlsl) = 0;
	virtual bool   Create(std::vector<std::wstring> texList, std::wstring hlsl) = 0;
	virtual void   SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj) = 0;
	virtual void   Frame() = 0;
	virtual void   Render() = 0;
	virtual void   Release() = 0;
};

class TSkyBox : public TSkyObject
{
	TTexture* m_pTex[6];
	ComPtr<ID3D11ShaderResourceView> m_pSRVArray[6];
	ComPtr<ID3D11ShaderResourceView> m_pCubeSRV;
public:	
	TBoxObj		m_Mesh;
	bool   Create(std::wstring texName, std::wstring hlsl);
	bool   Create(std::vector<std::wstring> texList, std::wstring hlsl);
	virtual void   SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj);
	virtual void   Frame();
	virtual void   Render();
	virtual void   PostRender1(ID3D11DeviceContext* pContext);
	virtual void   PostRender6(ID3D11DeviceContext* pContext);
	void		   PostRenderCube(ID3D11DeviceContext* pContext);
	virtual void   Release();
};

class TSkySphere : public TSkyObject
{
public:
	TShapeObj	m_Mesh;
	/*bool   Create(std::wstring texName, std::wstring hlsl)
	{
		return m_Mesh.Create(texName, hlsl);
	};*/
	bool   Create(std::vector<std::wstring> texList, std::wstring hlsl)
	{
		return m_Mesh.Create(texList[0], hlsl);
	};
	virtual void   SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
	{
		m_Mesh.SetMatrix(pWorld, pView, pProj);
	};
	virtual void   Frame() {};
	virtual void   Render()
	{
		m_Mesh.Render(TDevice::m_pContext);
	};
	virtual void   Release() {};
};

