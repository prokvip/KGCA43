#include "TDxObject.h"
#include "TStd.h"
void   TDxObject::UpdateVertexBuffer()
{
	// NDC 좌표계 변환
	for (int i = 0; i < m_vList.size(); i++)
	{
		m_vListNDC[i].p = ConvertScreenToNDC(m_vList[i].p);
		m_vListNDC[i].c = m_vList[i].c;
		m_vListNDC[i].t = m_vList[i].t;
	}
	if (m_pVertexBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	}
}
T_Math::FVector2 TDxObject::ConvertScreenToNDC(T_Math::FVector2 v)
{
	// 0~ 800 -> 0 ~ 1
	v.X = v.X / g_xClientSize;
	v.Y = v.Y / g_yClientSize;
	//NDC 좌표계
	// 0 ~ 1  -> -1 ~ +1
	T_Math::FVector2 ret;
	ret.X = v.X * 2.0f - 1.0f;
	ret.Y = -(v.Y * 2.0f - 1.0f);
	// -1 ~ 1  -> 0 ~ +1
	/*v.X = v.X * 0.5f + 0.5f;
	v.Y = v.Y * 0.5f + 0.5f;*/
	return ret;
}

void   TDxObject::Frame()
{
	
}

void   TDxObject::SetVertexData(RECT rt)
{
	m_vListScreen.resize(6);
	// 시계방향으로 구축되어야 한다.
	m_vListScreen[0] = { (float)rt.left, (float)rt.top,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
	m_vListScreen[1] = { T_Math::FVector2(rt.right, rt.top),
					T_Math::FVector4(1, 1, 1, 1),
					T_Math::FVector2(1, 0) };
	m_vListScreen[2] = { T_Math::FVector2(rt.right, rt.bottom),
					T_Math::FVector4(1, 1, 1, 1),
					T_Math::FVector2(1, 1) };
	m_vListScreen[3] = { T_Math::FVector2(rt.right, rt.bottom),
					T_Math::FVector4(1, 1, 1, 1),
					T_Math::FVector2(1, 1) };
	m_vListScreen[4] = { T_Math::FVector2(rt.left, rt.bottom),
					T_Math::FVector4(1, 1, 1, 1),
					T_Math::FVector2(0, 1) };
	m_vListScreen[5].p = T_Math::FVector2(rt.left, rt.top);
	m_vListScreen[5].c = T_Math::FVector4(0, 0, 1, 1);
	m_vListScreen[5].t = T_Math::FVector2(0, 0);

	// 화면좌표계를  NDC좌표 변경한다.
	m_vList    = m_vListScreen;	
	m_vListNDC = m_vListScreen;
	UpdateVertexBuffer();
}
bool   TDxObject::Create(
	ID3D11Device* pd3dDevice,	
	ID3D11DeviceContext* pContext,
	RECT rt, std::wstring texName, std::wstring hlsl) 
{		
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	m_szShaderFilename = hlsl;

	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			m_pd3dDevice,
			texName.c_str(),
			m_pTexture.GetAddressOf(),//&m_pTexture
			m_pSRV.GetAddressOf());

	SetVertexData(rt);

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
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(TVertex) * m_vListNDC.size();
	// 연결에 용도가 뭐냐? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vListNDC.at(0);

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
		m_szShaderFilename.c_str(),
		nullptr,
		nullptr,
		 "VSMain",
		 "vs_5_0", // dx11 정점쉐이더 컴파일러
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
		m_szShaderFilename.c_str(),
		nullptr,
		nullptr,
		"PSMain",
		"ps_5_0", // dx11 정점쉐이더 컴파일러
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
// GPU 가 처리하는 순서
// 1) 지정된 정점버퍼에 접근하여 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1개의 정점 단위로 로드한다.UINT pStrides = sizeof(TVertex); // 1개의 정점 크기
// 3) 지정된 인풋레이아웃에 따라서 정점 성분을 정점쉐이더에 전달한다
// 정점쉐이더는 정점단위로 호출된다.
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
void     TDxObject::PreRender(ID3D11DeviceContext* pContext)
{
	UpdateVertexBuffer();

	// 사용하지 않는 파이프라인은 디폴트 값 또는 null사용된다.	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(TVertex); // 1개의 정점 크기
	UINT pOffsets = 0; // 버퍼에 시작 인덱스
	pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 0번 슬롯으로 텍스처 전달
	pContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	//Texture2D g_txTexture : register(t0);

	pContext->VSSetShader(m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pPixelShader, nullptr, 0);
}
void     TDxObject::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_vListScreen.size(), 0);
}
void     TDxObject::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);	
}
void TDxObject::Release()
{
	/*if (m_pSRV)
	{
		m_pSRV->Release();
		m_pSRV = nullptr;m_pSRV.Reset();
	}
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}*/

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