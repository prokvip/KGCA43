#pragma once
#include "TTexMgr.h"
#include "TShaderMgr.h"
// p, n, c, t
struct P2C4T2_Vertex
{
	T_Math::FVector2 p; // ������ġ
	T_Math::FVector4 c; // �����÷�
	T_Math::FVector2 t; // �����ؽ�ó��ǥ
	P2C4T2_Vertex() = default;
	P2C4T2_Vertex(T_Math::FVector2 p, T_Math::FVector4 c, T_Math::FVector2 t)
	{
		this->p = p;
		this->c = c;
		this->t = t;
	}
	P2C4T2_Vertex(float x, float y, float r, float g, float b, 
		float a, float u, float v)
	{
		this->p = { x, y };
		this->c = { r,g,b,a };
		this->t = { u, v };
	}
};
struct PNCT_Vertex
{
	T_Math::FVector3 p; // ������ġ
	T_Math::FVector3 n; // ��������
	T_Math::FVector4 c; // �����÷�
	T_Math::FVector2 t; // �����ؽ�ó��ǥ
	PNCT_Vertex() = default;
	PNCT_Vertex(T_Math::FVector3 p, T_Math::FVector3 n, T_Math::FVector4 c, T_Math::FVector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
	PNCT_Vertex(float x, float y, float z, 
				float nx, float ny, float nz, 
				float r, float g, float b,float a, 
				float u, float v)
	{
		this->p = { x, y, z };
		this->n = { nx, ny, nz };
		this->c = { r,g,b,a };
		this->t = { u, v };
	}
};
class TDxObject 
{
protected:
	ID3D11Device*		 m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
public:
	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;	
	// �ؽ�ó(�̹���) �ε� ������
	//ComPtr<ID3D11Resource> m_pTexture = nullptr;
	TTexture* m_pTexture=nullptr;
public:
	std::wstring		  m_szShaderFilename;
public:	
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		RECT rt, std::wstring texName,
		std::wstring hlsl);

	// GPU �޸𸮿� �Ҵ�� ����.
	ID3D11Buffer* m_pVertexBuffer=nullptr;
	bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	
	TShader* m_pShader = nullptr;
	
	virtual bool     LoadShader(std::wstring filename);

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     InRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
};
class TDxObject2D : public TDxObject
{
public:
	std::vector<P2C4T2_Vertex>  m_vListScreen; // �ʱ� ȭ�� ����
	std::vector<P2C4T2_Vertex>  m_vList;		 // ������ ȭ�� ����
	std::vector<P2C4T2_Vertex>  m_vListNDC;	 // NDC
public:
	T_Math::FVector2 ConvertScreenToNDC(T_Math::FVector2 v);
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(	ID3D11Device* pd3dDevice,
							ID3D11DeviceContext* pContext,
							RECT rt, std::wstring texName,
							std::wstring hlsl);

	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	virtual bool     LoadShader(std::wstring filename);
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     InRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
};
class TDxObject3D : public TDxObject
{
public:
	std::vector<PNCT_Vertex>  m_vList;		 // ������ ȭ�� ����
public:
	virtual void   UpdateVertexBuffer();
	virtual bool   Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		RECT rt, std::wstring texName,
		std::wstring hlsl);
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     InRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
};

