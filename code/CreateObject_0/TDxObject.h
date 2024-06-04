#pragma once
#include <d3d11.h> 
#include <d3dcompiler.h> 
//../../dxtk/include  �⺻��η� �����Ѵ�. (�Ӽ�)
#include "WicTextureLoader.h"

//#include "../../dxtk/include/WicTextureLoader.h"
#include <vector>
#include <string>
#include "TMath.h"
#pragma comment ( lib, "d3dcompiler.lib")

#ifdef _DEBUG
	#pragma comment(lib, "DirectXTK_D.lib")
#else
	#pragma comment(lib, "DirectXTK_R.lib")
#endif
// p, n, c, t
struct TVertex
{
	T_Math::FVector2 p; // ������ġ
	T_Math::FVector4 c; // �����÷�
	T_Math::FVector2 t; // �����ؽ�ó��ǥ
};
class TDxObject 
{
	ID3D11Device*		 m_pd3dDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	ID3D11ShaderResourceView* m_pSRV = nullptr;
	// �ؽ�ó(�̹���) �ε� ������
	ID3D11Resource* m_pTexture = nullptr;
public:	
	T_Math::FVector2		m_vPos;
	// �ý��� �޸𸮿� �Ҵ�� ����.
	std::vector<TVertex>  m_vList;
	std::vector<TVertex>  m_vListNDC;
	TDxObject& Move(float dx, float dy);
public:
	T_Math::FVector2 ConvertScreenToNDC(T_Math::FVector2 v);

	virtual bool   Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext* pContext,
		RECT rt, std::wstring texName);

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

