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
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TVertex) * m_vList.size();
	// 연결에 용도가 뭐냐? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
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
	// HLSL(High Level shader Language) 언어(컴파일 언어) : C언어와 유사함.
	// HLSL언어로 작성해서 컴파일을 하고 오브젝트를 사용한다.

	HRESULT hr;
	
	ID3DBlob* errormsg = nullptr; // 컴파일 중 오류 메세지
	hr = D3DCompileFromFile(
		L"VS.txt",
		nullptr,
		nullptr,
		 "VSMain",
		 "vs_5_0", // dx11 정점쉐이더 컴파일러
		 0,
		 0,
		 &VS_Bytecode,
		 &errormsg
	);
	if (FAILED(hr)) return false;

	// 오브젝트 파일의 시작주소
	const void* pShaderBytecode = VS_Bytecode->GetBufferPointer();
	// 오브젝트 파일의 크기
	SIZE_T BytecodeLength = VS_Bytecode->GetBufferSize();

	hr = pd3dDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, nullptr, &m_pVertexShader);
	
	if (FAILED(hr))
	{
		return false;
	}
	if (errormsg) errormsg->Release();

	/// 픽쉘쉐이더 로드 및 생성	
	hr = D3DCompileFromFile(
		L"PS.txt",
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0", // dx11 정점쉐이더 컴파일러
		0,
		0,
		&PS_Bytecode,
		&errormsg
	);
	if (FAILED(hr)) return false;

	pShaderBytecode = PS_Bytecode->GetBufferPointer();
	BytecodeLength = PS_Bytecode->GetBufferSize();
	hr = pd3dDevice->CreatePixelShader(PS_Bytecode->GetBufferPointer(),
		PS_Bytecode->GetBufferSize(), nullptr,&m_pPixelShader);
	if (FAILED(hr)) return false;
	return true;
}
bool     TDxObject::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{   "POS",  
			0, 
			DXGI_FORMAT_R32G32_FLOAT, 
			0, 
			0, 
			D3D11_INPUT_PER_VERTEX_DATA, 
			0 },
	};

	UINT NumElements = 1;
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
	// 사용하지 않는 파이프라인은 디폴트 값 또는 null사용된다.
	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(TVertex); // 1개의 정점 크기
	UINT pOffsets = 0; // 버퍼에 시작 인덱스
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