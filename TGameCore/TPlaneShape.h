#pragma once
#include "TTextureMgr.h"

struct TVector2
{
	float x;
	float y;
	float Length()
	{
		return sqrt(LingthSquared());
	}
	float LingthSquared()
	{
		return (x * x, y * y);
	}
	TVector2 Normal()
	{
		float fLength = Length();
		if (fLength <= 0.0f)
		{
			x = y = 0.0f;
			return *this;
		}
		float InvertLength = 1.0 / fLength;
		return TVector2(x* InvertLength, y * InvertLength);
	}
	void Normalized()
	{
		float InvertLength = 1.0 / Length();
		x *= InvertLength;
		y * InvertLength;
	}
	TVector2& operator+=(const TVector2& V) 
	{
		x += V.x;
		y += V.y;
		return *this;
	}
	TVector2& operator-=(const TVector2& V)
	{
		x -= V.x;
		y -= V.y;
		return *this;
	}
	TVector2 operator+(const TVector2& V) const
	{
		return TVector2(x + V.x, y + V.y);
	}
	TVector2 operator-(const TVector2& V) const
	{
		return TVector2(x - V.x, y - V.y);
	}
	TVector2 operator-(float Bias) const
	{
		return TVector2(x * Bias, y * Bias);
	}
	TVector2 operator+(float Bias) const
	{
		return TVector2(x + Bias, y + Bias);
	}
	TVector2 operator*(float Scale) const
	{
		return TVector2(x * Scale, y * Scale );
	}
	TVector2 operator/(float Scale) const
	{
		return TVector2(x / Scale, y / Scale);
	}	
	bool operator==(const TVector2& V) const
	{
		if (fabs(x - V.x) < 0.001f)
		{
			if (fabs(y - V.y) < 0.001f)
			{				
				return true;
			}
		}
		return false;
	}
	bool operator!=(const TVector2& V) const
	{
		return !(*this == V);
	}
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
	W_STR                   m_csDefaultVSFileName;
	W_STR                   m_csDefaultPSFileName;
	C_STR                   m_csDefaultVSEntryName;
	C_STR                   m_csDefaultPSEntryName;
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
	virtual bool	CreateVertexBuffer();
	virtual bool	CreateIndexBuffer();
	ID3D11Buffer* CreateBuffer(UINT ByteWidth, UINT BindFlags, void** pAddress);

	virtual bool	CreateVertexShader();
	virtual bool	CreatePixelShader();
	ID3DBlob* CreateShader(LPCWSTR pFileName, LPCSTR pEntrypoint, LPCSTR pTarget);

	virtual bool	CreateInputLayout();

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
public:
	TPlaneShape()
	{
		Init();
	}
};