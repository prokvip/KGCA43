#include "TDxObj2D.h"
/// <summary>
/// 
/// </summary>
void   TDxObject2D::UpdateVertexBuffer()
{
	// NDC 좌표계 변환
	for (int i = 0; i < m_vVertexList.size(); i++)
	{
		m_vListNDC[i].p = ConvertScreenToNDC(m_vVertexList[i].p);
		m_vListNDC[i].c = m_vVertexList[i].c;
		m_vListNDC[i].t = m_vVertexList[i].t;
	}
	if (m_pVertexBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	}
}
T::TVector2 TDxObject2D::ConvertScreenToNDC(T::TVector2 v)
{
	// 0~ 800 -> 0 ~ 1
	v.x = v.x / g_xClientSize;
	v.y = v.y / g_yClientSize;
	//NDC 좌표계
	// 0 ~ 1  -> -1 ~ +1
	T::TVector2 ret;
	ret.x = v.x * 2.0f - 1.0f;
	ret.y = -(v.y * 2.0f - 1.0f);
	// -1 ~ 1  -> 0 ~ +1
	/*v.x = v.x * 0.5f + 0.5f;
	v.y = v.y * 0.5f + 0.5f;*/
	return ret;
}

void   TDxObject2D::Frame()
{

}

void   TDxObject2D::SetVertexData(RECT rt)
{
	m_vListScreen.resize(6);
	// 시계방향으로 구축되어야 한다.
	m_vListScreen[0] = { (float)rt.left, (float)rt.top,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f };
	m_vListScreen[1] = { T::TVector2(rt.right, rt.top),
					T::TVector4(1, 1, 1, 1),
					T::TVector2(1, 0) };
	m_vListScreen[2] = { T::TVector2(rt.right, rt.bottom),
					T::TVector4(1, 1, 1, 1),
					T::TVector2(1, 1) };
	m_vListScreen[3] = { T::TVector2(rt.right, rt.bottom),
					T::TVector4(1, 1, 1, 1),
					T::TVector2(1, 1) };
	m_vListScreen[4] = { T::TVector2(rt.left, rt.bottom),
					T::TVector4(1, 1, 1, 1),
					T::TVector2(0, 1) };
	m_vListScreen[5].p = T::TVector2(rt.left, rt.top);
	m_vListScreen[5].c = T::TVector4(1, 1, 1, 1),
		m_vListScreen[5].t = T::TVector2(0, 0);

	// 화면좌표계를  NDC좌표 변경한다.
	m_vVertexList = m_vListScreen;
	m_vListNDC = m_vListScreen;
	UpdateVertexBuffer();
}
bool   TDxObject2D::Create(	RECT rt, std::wstring texName, std::wstring hlsl)
{
	m_pd3dDevice = TDevice::m_pd3dDevice.Get();
	m_pContext   = TDevice::m_pContext;

	m_szShaderFilename = hlsl;

	TLoadData ld;
	ld.m_csLoadFileName = texName;
	ld.m_csLoadShaderFileName = hlsl;

	m_pTexture = I_Tex.Load(ld).get();
	if (m_pTexture != nullptr)
	{
		m_pSRV = m_pTexture->m_pSRV;
	}

	SetVertexData(rt);

	if (CreateVertexBuffer(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	if (LoadShader(m_szShaderFilename) == false)
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
bool     TDxObject2D::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P2C4T2_Vertex) * m_vListNDC.size();
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
bool     TDxObject2D::LoadShader(std::wstring filename)
{
	TLoadData ld;
	ld.m_csLoadFileName = filename;
	ld.m_csLoadShaderFileName = filename;

	m_pShader = I_Shader.Load(ld).get();
	if (m_pShader == nullptr) return false;
	return true;
}
// GPU 가 처리하는 순서
// 1) 지정된 정점버퍼에 접근하여 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1개의 정점 단위로 로드한다.UINT pStrides = sizeof(P2C4T2_Vertex); // 1개의 정점 크기
// 3) 지정된 인풋레이아웃에 따라서 정점 성분을 정점쉐이더에 전달한다
// 정점쉐이더는 정점단위로 호출된다.
// |x, y, r,g,b,a|,|x, y, r,g,b,a| ,|x, y, r,g,b,a| ,|x, y, r,g,b,a| 
//   0~7  8~11
// |x, y, r,g,b,a, u,v|,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| 
//   0~7  8~11   24~32
bool     TDxObject2D::CreateInputLayout(ID3D11Device* pd3dDevice)
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
		m_pShader->VS_Bytecode->GetBufferPointer(),
		m_pShader->VS_Bytecode->GetBufferSize(),
		&m_pVertexLayout);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
void     TDxObject2D::PreRender(ID3D11DeviceContext* pContext)
{
	UpdateVertexBuffer();

	// 사용하지 않는 파이프라인은 디폴트 값 또는 null사용된다.	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(P2C4T2_Vertex); // 1개의 정점 크기
	UINT pOffsets = 0; // 버퍼에 시작 인덱스
	pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 0번 슬롯으로 텍스처 전달
	pContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	//Texture2D g_txTexture : register(t0);

	pContext->VSSetShader(m_pShader->m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pShader->m_pPixelShader, nullptr, 0);
}
void     TDxObject2D::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_vListScreen.size(), 0);
}
void     TDxObject2D::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
}
void	 TDxObject2D::Release()
{
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
	if (m_pVertexLayout)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout = nullptr;
	}
}