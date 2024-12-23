#pragma once
#include "TTexMgr.h"
#include "TShaderMgr.h"
#include "TCollision.h"
// p, n, c, t
struct P2C4T2_Vertex
{
	T::TVector2 p; // 정점위치
	T::TVector4 c; // 정점컬러
	T::TVector2 t; // 정점텍스처좌표
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
	T::TVector3 p; // 정점위치
	T::TVector3 n; // 정점방향
	T::TVector4 c; // 정점컬러
	T::TVector2 t; // 정점텍스처좌표
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
struct IW_Vertex
{
	float i0[4]; // 행렬의 인덱스
	float w0[4]; // 행렬의 가중치	
	float i1[4]; // 행렬의 인덱스
	float w1[4]; // 행렬의 가중치	
	IW_Vertex()
	{
		i0[3] = i0[2] = i0[1] = i0[0] = 0;
		w0[3] = w0[2] = w0[1] = w0[0] = 0;
		i1[3] = i1[2] = i1[1] = i1[0] = 0;
		w1[3] = w1[2] = w1[1] = w1[0] = 0;
	}
};

struct FRAME_CB // 상수버퍼
{
	T::TMatrix g_matWorld;
	T::TMatrix g_matView;
	T::TMatrix g_matProj;
};

namespace DX
{
	void		ClearD3D11DeviceContext(ID3D11DeviceContext* pd3dDeviceContext);

	HRESULT	CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT D3DX11CompileFromFile(LPCWSTR pSrcFile, CONST D3D_SHADER_MACRO* pDefines, LPD3DINCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags1, UINT Flags2,
		/*ID3DX11ThreadPump* pPump, */ID3DBlob** ppShader, ID3DBlob** ppErrorMsgs, HRESULT* pHResult);
	ID3D11VertexShader* LoadVertexShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char* pFuntionName = 0, bool bBinary = false);
	ID3D11PixelShader* LoadPixelShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, const char* pFuntionName = 0, bool bBinary = false, ID3DBlob** pRetBlob = nullptr);
	ID3D11GeometryShader* LoadGeometryShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char* pFuntionName = 0, bool bBinary = false);
	ID3D11HullShader* LoadHullShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char* pFuntionName = 0, bool bBinary = false);
	ID3D11DomainShader* LoadDomainShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char* pFuntionName = 0, bool bBinary = false);
	ID3D11ComputeShader* LoadComputeShaderFile(ID3D11Device* pd3dDevice, const void* pLoadShaderFile, ID3DBlob** ppBlobOut = nullptr, const char* pFuntionName = 0, bool bBinary = false);


	ID3D11InputLayout* CreateInputlayout(ID3D11Device* pd3dDevice, DWORD dwSize, LPCVOID lpData, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);
	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pd3dDevice, void* vertices, UINT iNumVertex, UINT iVertexSize, bool bDynamic = false);
	ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pd3dDevice, void* indices, UINT iNumIndex, UINT iSize, bool bDynamic = false);
	ID3D11Buffer* CreateConstantBuffer(ID3D11Device* pd3dDevice, void* data, UINT iNumIndex, UINT iSize, bool bDynamic = false);

	ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* pDevice, const TCHAR* strFilePath);
	ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const TCHAR* strFilePath);
	ID3D11DepthStencilView* CreateDepthStencilView(ID3D11Device* pDevice, DWORD dwWidth, DWORD dwHeight);
}
class TDxObject 
{
protected:
	ID3D11Device*		 m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	std::wstring		 m_szName;
	std::wstring		 m_szParentName;
	BOOL				 m_objType = FALSE;
public:
	// 렌더링 파이브라인(픽쉘쉐이더)에 전송 데이터
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;	
	// 텍스처(이미지) 로드 데이터
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

	// GPU 메모리에 할당된 버퍼.
	virtual bool   CreateIWVertexBuffer(ID3D11Device* pd3dDevice) { return true; };

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
	virtual void     AnimFrame(float& fAnimFrame) {};
	virtual void     PreRender(ID3D11DeviceContext* pContext);
	virtual void     Render(ID3D11DeviceContext* pContext);
	virtual void     PostRender(ID3D11DeviceContext* pContext);
	virtual void     Release();
	virtual void	 SetVertexData(RECT rt);
	virtual void	 SetVertexData() {};
	virtual void	 SetIndexData() {};
	
};



