#include "TDxObject.h"
TDxObject& TDxObject::Move(float dx, float dy)
{
	m_vList[0].v.X = m_vList[0].v.X + dx;
	m_vList[0].v.Y = m_vList[0].v.Y + dy;
	m_vList[1].v.X = m_vList[1].v.X + dx;
	m_vList[1].v.Y = m_vList[1].v.Y + dy;
	m_vList[2].v.X = m_vList[2].v.X + dx;
	m_vList[2].v.Y = m_vList[2].v.Y + dy;
	return *this;
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
bool     TDxObject::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
{"POS",0,	DXGI_FORMAT_R32G32_FLOAT,		0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,8,D3D11_INPUT_PER_VERTEX_DATA,0 },
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

	pContext->VSSetShader(m_pVertexShader, nullptr, 0);	
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);	
	pContext->Draw(m_vList.size(), 0);
}
void TDxObject::Release()
{
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