#pragma once
#include <d3d11.h> 
#include <d3dcompiler.h> 

#include <vector>
#include "TMath.h"
#pragma comment ( lib, "d3dcompiler.lib")

// p, n, c, t
struct TVertex
{
	T_Math::FVector2 p; // 정점위치
	T_Math::FVector4 c; // 정점컬러
	T_Math::FVector2 t; // 정점텍스처좌표
};
class TDxObject 
{
public:	
	// 시스템 메모리에 할당된 버퍼.
	std::vector<TVertex>  m_vList;
	TDxObject& Move(float dx, float dy);
public:
	// GPU 메모리에 할당된 버퍼.
	ID3D11Buffer* m_pVertexBuffer=nullptr;
	bool     CreateVertexBuffer(ID3D11Device* pd3dDevice);
	
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3DBlob* VS_Bytecode = nullptr; // 오브젝트 파일
	ID3DBlob* PS_Bytecode = nullptr; // 오브젝트 파일
	bool     LoadShader(ID3D11Device* pd3dDevice);

	ID3D11InputLayout* m_pVertexLayout = nullptr;
	bool     CreateInputLayout(ID3D11Device* pd3dDevice);

	void     Render(ID3D11DeviceContext* pContext);
	void     Release();
};

