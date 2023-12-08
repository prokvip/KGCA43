#pragma once
#include "TDevice.h"
//#include "../../include/DirectXTex.h"
//프로젝트 속성에서 C++-> 추가 포함 디렉토리에 ../../include 한다.
#include "DirectXTex.h"

//프로젝트 속성에 링커->입력->추가종속성 d3d11.lib; d3dcompiler.lib; DirectXTex.lib;
//#pragma commnet을 이용하면 프로젝트으로 포함할 수 있다.
#pragma comment (lib, "DirectXTex.lib")

struct TVector2
{
	float x;
	float y;
	TVector2() { x = 0; y = 0;  }
	TVector2(float _x, float _y) { x = _x; y = _y;  }
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
	TVector4() { x = 0; y = 0; z = 0; w = 0;}
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

class Sample : public TDevice
{
	std::unique_ptr<DirectX::ScratchImage> m_tex;
	ID3D11ShaderResourceView* m_pTextureSRV = nullptr;//bk.png

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

	bool	LoadTexture(std::wstring texFileName);
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};