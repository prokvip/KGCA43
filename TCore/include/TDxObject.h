#pragma once
#include "TTexMgr.h"
#include "TShaderMgr.h"
//using namespace T_Math;
// p, n, c, t
struct P2C4T2_Vertex
{
	T::TVector2 p; // ������ġ
	T::TVector4 c; // �����÷�
	T::TVector2 t; // �����ؽ�ó��ǥ
	P2C4T2_Vertex() = default;
	P2C4T2_Vertex(T::TVector2 p, T::TVector4 c, T::TVector2 t)
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
	T::TVector3 p; // ������ġ
	T::TVector3 n; // ��������
	T::TVector4 c; // �����÷�
	T::TVector2 t; // �����ؽ�ó��ǥ
	bool operator == (const PNCT_Vertex& v) const
	{
		if ( p == v.p && n == v.n && c == v.c && t == v.t)
		{
			return true;
		}
		return false;
	};
	bool operator != (const PNCT_Vertex& v) const
	{
		if (p == v.p || n == v.n || c == v.c || t == v.t)
		{
			return true;
		}
		return false;
	};
	PNCT_Vertex() = default;
	PNCT_Vertex(T::TVector3 p, T::TVector3 n, T::TVector4 c, T::TVector2 t)
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
	std::wstring		 m_szName;
	std::wstring		 m_szParentName;
	BOOL				 m_objType = FALSE;
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

	static ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pd3dDevice,
		void* data, UINT iNumIndex,
		UINT iSize, bool bDynamic=false);

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



