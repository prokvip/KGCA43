#pragma once
#include "TDevice.h"


struct TVertex
{
	float x;
	float y;
	//float z;
	TVertex() { x = 0; y = 0; /*z = 0;*/ }
	// z의 성분은 0 ~ 1 범위로 되어 있다.
	TVertex(float _x, float _y/*, float _z = 0.0f*/) { x = _x; y = _y; /*z = _z; */}
};

class Sample : public TDevice
{
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
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};