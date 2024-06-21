#pragma once
#include "TDevice.h"
#include "TTexMgr.h"
#include "TShaderMgr.h"
// p, n, c, t
struct TVertex
{
	T_Math::FVector2 p; // ������ġ
	T_Math::FVector4 c; // �����÷�
	T_Math::FVector2 t; // �����ؽ�ó��ǥ
	TVertex() = default;
	TVertex(T_Math::FVector2 p, T_Math::FVector4 c, T_Math::FVector2 t)
	{
		this->p = p;
		this->c = c;
		this->t = t;
	}
	TVertex(float x, float y, float r, float g, float b, 
		float a, float u, float v)
	{
		this->p = { x, y };
		this->c = { r,g,b,a };
		this->t = { u, v };
	}
};
class TDxObject 
{
protected:
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;	
	TTexture* m_pTexture=nullptr;
	std::vector<TVertex>  m_vListScreen; // �ʱ� ȭ�� ����
	std::vector<TVertex>  m_vList;		 // ������ ȭ�� ����
	std::vector<TVertex>  m_vListNDC;	 // NDC
public:
	std::wstring		  m_szShaderFilename;
public:
	T_Math::FVector2 ConvertScreenToNDC(T_Math::FVector2 v);
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(RECT rt, std::wstring texName,std::wstring hlsl);

	// GPU �޸𸮿� �Ҵ�� ����.
	ComPtr<ID3D11Buffer> m_pVertexBuffer=nullptr;
	bool     CreateVertexBuffer();
	
	TShader* m_pShader = nullptr;	
	virtual bool     LoadShader(std::wstring filename);

	ComPtr<ID3D11InputLayout> m_pVertexLayout = nullptr;
	virtual	bool     CreateInputLayout();

	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
};

