#pragma once
#include "TTextureMgr.h"

struct TVector2
{
	float x;
	float y;
	TVector2() { x = 0; y = 0; }
	TVector2(float _x, float _y) { x = _x; y = _y; }
};
struct TVector3
{
	float x;
	float y;
	float z;
	TVector3() { x = 0; y = 0; z = 0; }
	TVector3(float _x, float _y, float _z = 0.0f) { x = _x; y = _y; z = _z; }
};
struct TVector4
{
	float x;
	float y;
	float z;
	float w;
	TVector4() { x = 0; y = 0; z = 0; w = 0; }
	TVector4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; }
};

struct TVertex
{
	TVector3 pos;
	TVector4 color;
	TVector2 tex; //uv
	TVertex() {}
	TVertex(TVector3 p, TVector4 c, TVector2 t) { pos = p; color = c; tex = t; }
};
class TPlaneShape
{
public:
	W_STR                   m_csName;
	ID3D11Device*			m_pd3dDevice = nullptr;
	ID3D11DeviceContext*	m_pd3dContext = nullptr;
	RECT					m_rtClient;
	TTexture*				m_ptTex;
	std::vector<TTexture*>  m_pTexArray;
	std::vector<TVertex>	m_VertexList;  // 시스템 메모리
	std::vector<DWORD>		m_IndexList;
	ID3D11Buffer*			m_pVertexBuffer;//  비디오카드 메모리
	ID3D11Buffer*			m_pIndexBuffer;//  비디오카드 메모리
	ID3DBlob*				m_pVertexShaderByteCode = nullptr;
	ID3DBlob*				m_pPixelShaderByteCode = nullptr;
	ID3D11VertexShader*		m_pVertexShader = nullptr;
	ID3D11PixelShader*		m_pPixelShader = nullptr;
	ID3D11InputLayout*		m_pVertexlayout = nullptr;
public:
	bool	CreateVertexBuffer();
	bool	CreateIndexBuffer();
	ID3D11Buffer* CreateBuffer(UINT ByteWidth, UINT BindFlags, void** pAddress);

	bool	CreateVertexShader();
	bool	CreatePixelShader();
	ID3DBlob* CreateShader(LPCWSTR pFileName, LPCSTR pEntrypoint, LPCSTR pTarget);

	bool	CreateInputLayout();

	virtual bool	LoadTexture(T_STR_VECTOR texFileName);
	virtual bool	LoadTexture(std::wstring texFileName);
public:
	virtual bool    Init();
	virtual bool    Create(W_STR name, T_STR_VECTOR texFileName);
	virtual bool    Create(W_STR name, W_STR texFileName);
	virtual bool    PreRender();
	virtual bool    Frame();
	virtual bool    Render();
	virtual bool    PostRender();
	virtual bool    Release();
};