#pragma once
#include "TStd.h"

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
	ID3D11Device*		 m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;	
	// �ؽ�ó(�̹���) �ε� ������
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
public:	
	// �ý��� �޸𸮿� �Ҵ�� ����.
	std::vector<TVertex>  m_vListScreen; // �ʱ� ȭ�� ����
	std::vector<TVertex>  m_vList;		 // ������ ȭ�� ����
	std::vector<TVertex>  m_vListNDC;	 // NDC
	std::wstring		  m_szShaderFilename;
public:
	T_Math::FVector2 ConvertScreenToNDC(T_Math::FVector2 v);
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		RECT rt, std::wstring texName,
		std::wstring hlsl);

	// GPU �޸𸮿� �Ҵ�� ����.
	ID3D11Buffer* m_pVertexBuffer=nullptr;
	bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3DBlob* VS_Bytecode = nullptr; // ������Ʈ ����
	ID3DBlob* PS_Bytecode = nullptr; // ������Ʈ ����
	bool     LoadShader(ID3D11Device* pd3dDevice);

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	bool     CreateInputLayout(ID3D11Device* pd3dDevice);
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();

	virtual void	 SetVertexData(RECT rt);
};

