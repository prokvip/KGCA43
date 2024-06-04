#include "TDxObject.h"
#include "TStd.h"
TDxObject& TDxObject::Move(float dx, float dy)
{
	m_vList[0].p.X += dx;
	m_vList[0].p.Y += dy;
	m_vList[1].p.X += dx;
	m_vList[1].p.Y += dy;
	m_vList[2].p.X += dx;
	m_vList[2].p.Y += dy;
	m_vList[3].p.X += dx;
	m_vList[3].p.Y += dy;
	m_vList[4].p.X += dx;
	m_vList[4].p.Y += dy;
	m_vList[5].p.X += dx;
	m_vList[5].p.Y += dy;

	m_pContext->UpdateSubresource(m_pVertexBuffer,
		0, NULL, &m_vList.at(0), 0, 0);
	return *this;
}
bool   TDxObject::Create(
	ID3D11Device* pd3dDevice,	
	ID3D11DeviceContext* pContext,
	RECT rt, std::wstring texName) 
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			m_pd3dDevice,
			texName.c_str(),
			&m_pTexture,
			&m_pSRV);

	m_vPos.X = (rt.left+rt.right)* 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;

	m_vList.resize(6);
	// �ð�������� ����Ǿ�� �Ѵ�.
	m_vList[0].p = T_Math::FVector2(rt.left, rt.top);
	m_vList[1].p = T_Math::FVector2(rt.right , rt.top);
	m_vList[2].p = T_Math::FVector2(rt.right, rt.bottom );
	m_vList[0].c = T_Math::FVector4(1, 0, 0, 1);
	m_vList[1].c = T_Math::FVector4(0, 1, 0, 1);
	m_vList[2].c = T_Math::FVector4(0, 0, 1, 1);
	m_vList[0].t = T_Math::FVector2(0, 0);
	m_vList[1].t = T_Math::FVector2(1, 0);
	m_vList[2].t = T_Math::FVector2(1, 1);


	m_vList[3].p = T_Math::FVector2(rt.right, rt.bottom);
	m_vList[4].p = T_Math::FVector2(rt.left, rt.bottom);
	m_vList[5].p = T_Math::FVector2(rt.left, rt.top);
	m_vList[3].c = T_Math::FVector4(1, 0, 0, 1);
	m_vList[4].c = T_Math::FVector4(0, 1, 0, 1);
	m_vList[5].c = T_Math::FVector4(0, 0, 1, 1);
	m_vList[3].t = T_Math::FVector2(1, 1);
	m_vList[4].t = T_Math::FVector2(0, 1);
	m_vList[5].t = T_Math::FVector2(0, 0);


	// ȭ����ǥ�踦  NDC��ǥ �����Ѵ�.

	for (auto& V : m_vList)
	{
		// ȭ����ǥ��
		T_Math::FVector2  v = V.p;
		// 0~ 800 -> 0 ~ 1
		v.X = v.X / g_xClientSize;
		v.Y = v.Y / g_yClientSize;
		//NDC ��ǥ��
		// 0 ~ 1  -> -1 ~ +1
		V.p.X = v.X * 2.0f - 1.0f;
		V.p.Y = -(v.Y * 2.0f - 1.0f);

		int k = 0;
		// -1 ~ 1  -> 0 ~ +1
		/*v.X = v.X * 0.5f + 0.5f;
		v.Y = v.Y * 0.5f + 0.5f;*/
	}



	if (CreateVertexBuffer(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	if (LoadShader(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	if (CreateInputLayout(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	return true;
}
bool     TDxObject::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TVertex) * m_vList.size();
	// ���ῡ �뵵�� ����? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vList.at(0);

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		&this->m_pVertexBuffer);
	if (FAILED(hr)) return false;
	return true;
}
bool     TDxObject::LoadShader(ID3D11Device* pd3dDevice)
{
	// HLSL(High Level shader Language) ���(������ ���) : C���� ������.
	// HLSL���� �ۼ��ؼ� �������� �ϰ� ������Ʈ�� ����Ѵ�.

	HRESULT hr;
	
	ID3DBlob* errormsg = nullptr; // ������ �� ���� �޼���
	hr = D3DCompileFromFile(
		L"VS.txt",
		nullptr,
		nullptr,
		 "VSMain",
		 "vs_5_0", // dx11 �������̴� �����Ϸ�
		 0,
		 0,
		 &VS_Bytecode,
		 &errormsg
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, 
			(char*)errormsg->GetBufferPointer(), 
			"ERROR", MB_OK);
		return false;
	}

	// ������Ʈ ������ �����ּ�
	const void* pShaderBytecode = VS_Bytecode->GetBufferPointer();
	// ������Ʈ ������ ũ��
	SIZE_T BytecodeLength = VS_Bytecode->GetBufferSize();

	hr = pd3dDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, nullptr, &m_pVertexShader);
	
	if (FAILED(hr))
	{
		return false;
	}
	if (errormsg) errormsg->Release();

	/// �Ƚ����̴� �ε� �� ����	
	hr = D3DCompileFromFile(
		L"PS.txt",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0", // dx11 �������̴� �����Ϸ�
		0,
		0,
		&PS_Bytecode,
		&errormsg
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,
			(char*)errormsg->GetBufferPointer(),
			"ERROR", MB_OK);
		return false;
	}

	pShaderBytecode = PS_Bytecode->GetBufferPointer();
	BytecodeLength = PS_Bytecode->GetBufferSize();
	hr = pd3dDevice->CreatePixelShader(PS_Bytecode->GetBufferPointer(),
		PS_Bytecode->GetBufferSize(), nullptr,&m_pPixelShader);
	if (FAILED(hr)) return false;
	return true;
}
// GPU �� ó���ϴ� ����
// 1) ������ �������ۿ� �����Ͽ� 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1���� ���� ������ �ε��Ѵ�.UINT pStrides = sizeof(TVertex); // 1���� ���� ũ��
// 3) ������ ��ǲ���̾ƿ��� ���� ���� ������ �������̴��� �����Ѵ�
// �������̴��� ���������� ȣ��ȴ�.
// |x, y, r,g,b,a|,|x, y, r,g,b,a| ,|x, y, r,g,b,a| ,|x, y, r,g,b,a| 
//   0~7  8~11
// |x, y, r,g,b,a, u,v|,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| 
//   0~7  8~11   24~32
bool     TDxObject::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
{"POS",0,	DXGI_FORMAT_R32G32_FLOAT,		0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,8,D3D11_INPUT_PER_VERTEX_DATA,0 },
{"TEX",0,DXGI_FORMAT_R32G32_FLOAT,		    0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};

	UINT NumElements = sizeof(layout) / sizeof(layout[0]);
	HRESULT hr = pd3dDevice->CreateInputLayout(
		layout,
		NumElements,
		VS_Bytecode->GetBufferPointer(),
		VS_Bytecode->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void     TDxObject::Render(ID3D11DeviceContext* pContext)
{
	

	// ������� �ʴ� ������������ ����Ʈ �� �Ǵ� null���ȴ�.
	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(TVertex); // 1���� ���� ũ��
	UINT pOffsets = 0; // ���ۿ� ���� �ε���
	pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	// 0�� �������� �ؽ�ó ����
	pContext->PSSetShaderResources(0, 1, &m_pSRV);
	//Texture2D g_txTexture : register(t0);

	pContext->VSSetShader(m_pVertexShader, nullptr, 0);	
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);	
	pContext->Draw(m_vList.size(), 0);
}
void TDxObject::Release()
{
	if (m_pSRV)
	{
		m_pSRV->Release();
		m_pSRV = nullptr;
	}
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}

	if (VS_Bytecode)
	{
		VS_Bytecode->Release();
		VS_Bytecode = nullptr;
	}
	if (PS_Bytecode)
	{
		PS_Bytecode->Release();
		PS_Bytecode = nullptr;
	}

	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
	if (m_pVertexShader)
	{
		m_pVertexShader->Release();
		m_pVertexShader = nullptr;
	}
	if (m_pPixelShader)
	{
		m_pPixelShader->Release();
		m_pPixelShader = nullptr;
	}
	if (m_pVertexLayout)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}