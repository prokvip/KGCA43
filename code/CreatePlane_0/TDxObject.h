#pragma once
#include <d3d11.h> 
#include <d3dcompiler.h> 

#include <vector>
#include "TMath.h"
#pragma comment ( lib, "d3dcompiler.lib")

struct TVertex
{
	T_Math::FVector2 v;
};
class TDxObject 
{
public:	
	// �ý��� �޸𸮿� �Ҵ�� ����.
	std::vector<TVertex>  m_vList;
	TDxObject& Move(float dx, float dy);
public:
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

	void     Render(ID3D11DeviceContext* pContext);
	void     Release();
};

