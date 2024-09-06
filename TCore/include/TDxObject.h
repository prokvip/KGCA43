#pragma once
#include "TTexMgr.h"
#include "TShaderMgr.h"
using namespace T_Math;
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

struct FRAME_CB // �������
{
	T::TMatrix g_matWorld;
	T::TMatrix g_matView;
	T::TMatrix g_matProj;
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
	virtual void        LoadTexture(std::wstring szPath) {};
public:
	std::wstring		  m_szShaderFilename;
public:	
	virtual void   UpdateVertexBuffer();
	// 2D
	virtual bool   Create(RECT rt, std::wstring texName,std::wstring hlsl);
	// 3D
	virtual bool   Create(std::wstring texName,	std::wstring hlsl);

	// GPU �޸𸮿� �Ҵ�� ����.
	ID3D11Buffer* m_pVertexBuffer=nullptr;
	virtual bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	ID3D11Buffer* m_pIndexBuffer = nullptr;
	virtual bool     CreateIndexBuffer(ID3D11Device* pd3dDevice);
	FRAME_CB			 m_cb;
	ComPtr<ID3D11Buffer>   m_pConstantBuffer;
	virtual bool  CreateConstantBuffer(ID3D11Device* pd3dDevice);

	TShader* m_pShader = nullptr;
	
	virtual bool     LoadShader(std::wstring filename);

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	virtual	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	virtual void     Init() {};
	virtual void     Frame();
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
	virtual void	 SetVertexData() {};
	virtual void	 SetIndexData() {};
	
};



