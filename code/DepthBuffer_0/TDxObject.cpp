#include "TDxObject.h"
/// <summary>
/// 
/// </summary>
void   TDxObject::UpdateVertexBuffer()
{
}
void   TDxObject::Frame()
{	
}
void   TDxObject::SetVertexData(RECT rt)
{
}
bool   TDxObject::Create(
	ID3D11Device* pd3dDevice,	
	ID3D11DeviceContext* pContext,
	RECT rt, std::wstring texName, std::wstring hlsl) 
{	
	return true;
}
bool     TDxObject::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{	
	return true;
}
bool     TDxObject::LoadShader(std::wstring filename)
{
	m_pShader = I_Shader.Load(filename).get();
	if (m_pShader==nullptr) return false;
	return true;
}
// GPU �� ó���ϴ� ����
// 1) ������ �������ۿ� �����Ͽ� 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1���� ���� ������ �ε��Ѵ�.UINT pStrides = sizeof(P2C4T2_Vertex); // 1���� ���� ũ��
// 3) ������ ��ǲ���̾ƿ��� ���� ���� ������ �������̴��� �����Ѵ�
// �������̴��� ���������� ȣ��ȴ�.
// |x, y, r,g,b,a|,|x, y, r,g,b,a| ,|x, y, r,g,b,a| ,|x, y, r,g,b,a| 
//   0~7  8~11
// |x, y, r,g,b,a, u,v|,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| 
//   0~7  8~11   24~32
bool     TDxObject::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	return true;
}
void     TDxObject::PreRender(ID3D11DeviceContext* pContext)
{	
}
void     TDxObject::PostRender(ID3D11DeviceContext* pContext)
{		
}
void     TDxObject::InRender(ID3D11DeviceContext* pContext)
{
}
void     TDxObject::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);	
}
void TDxObject::Release()
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

/// <summary>
/// 
/// </summary>
void   TDxObject2D::UpdateVertexBuffer()
{
	// NDC ��ǥ�� ��ȯ
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
T_Math::FVector2 TDxObject2D::ConvertScreenToNDC(T_Math::FVector2 v)
{
	// 0~ 800 -> 0 ~ 1
	v.X = v.X / g_xClientSize;
	v.Y = v.Y / g_yClientSize;
	//NDC ��ǥ��
	// 0 ~ 1  -> -1 ~ +1
	T_Math::FVector2 ret;
	ret.X = v.X * 2.0f - 1.0f;
	ret.Y = -(v.Y * 2.0f - 1.0f);
	// -1 ~ 1  -> 0 ~ +1
	/*v.X = v.X * 0.5f + 0.5f;
	v.Y = v.Y * 0.5f + 0.5f;*/
	return ret;
}

void   TDxObject2D::Frame()
{

}

void   TDxObject2D::SetVertexData(RECT rt)
{
	m_vListScreen.resize(6);
	// �ð�������� ����Ǿ�� �Ѵ�.
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
	m_vListScreen[5].c = T_Math::FVector4(1, 1, 1, 1),
		m_vListScreen[5].t = T_Math::FVector2(0, 0);

	// ȭ����ǥ�踦  NDC��ǥ �����Ѵ�.
	m_vList = m_vListScreen;
	m_vListNDC = m_vListScreen;
	UpdateVertexBuffer();
}
bool   TDxObject2D::Create(
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pContext,
	RECT rt, std::wstring texName, std::wstring hlsl)
{
	m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	m_szShaderFilename = hlsl;

	m_pTexture = I_Tex.Load(texName).get();
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
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(P2C4T2_Vertex) * m_vListNDC.size();
	// ���ῡ �뵵�� ����? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
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
	m_pShader = I_Shader.Load(filename).get();
	if (m_pShader == nullptr) return false;
	return true;
}
// GPU �� ó���ϴ� ����
// 1) ������ �������ۿ� �����Ͽ� 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1���� ���� ������ �ε��Ѵ�.UINT pStrides = sizeof(P2C4T2_Vertex); // 1���� ���� ũ��
// 3) ������ ��ǲ���̾ƿ��� ���� ���� ������ �������̴��� �����Ѵ�
// �������̴��� ���������� ȣ��ȴ�.
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

	// ������� �ʴ� ������������ ����Ʈ �� �Ǵ� null���ȴ�.	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(P2C4T2_Vertex); // 1���� ���� ũ��
	UINT pOffsets = 0; // ���ۿ� ���� �ε���
	pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 0�� �������� �ؽ�ó ����
	pContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	//Texture2D g_txTexture : register(t0);

	pContext->VSSetShader(m_pShader->m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pShader->m_pPixelShader, nullptr, 0);
}
void     TDxObject2D::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_vListScreen.size(), 0);
}
void     TDxObject2D::InRender(ID3D11DeviceContext* pContext)
{
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

/// <summary>
/// 
/// </summary>
void   TDxObject3D::UpdateVertexBuffer()
{
	//// NDC ��ǥ�� ��ȯ
	//for (int i = 0; i < m_vList.size(); i++)
	//{
	//	m_vListNDC[i].p = ConvertScreenToNDC(m_vList[i].p);
	//	m_vListNDC[i].c = m_vList[i].c;
	//	m_vListNDC[i].t = m_vList[i].t;
	//}
	//if (m_pVertexBuffer != nullptr)
	//{
	//	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	//}
}
void   TDxObject3D::Frame()
{

}
bool   TDxObject3D::Create(ID3D11Device* pd3dDevice,ID3D11DeviceContext* pContext,RECT rt, std::wstring texName, std::wstring hlsl)
{
	/*m_pd3dDevice = pd3dDevice;
	m_pContext = pContext;
	m_szShaderFilename = hlsl;

	m_pTexture = I_Tex.Load(texName).get();
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
	}*/
	return true;
}
bool     TDxObject3D::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_vList.size();
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
// GPU �� ó���ϴ� ����
// 1) ������ �������ۿ� �����Ͽ� 
	// pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
// 2) 1���� ���� ������ �ε��Ѵ�.UINT pStrides = sizeof(P2C4T2_Vertex); // 1���� ���� ũ��
// 3) ������ ��ǲ���̾ƿ��� ���� ���� ������ �������̴��� �����Ѵ�
// �������̴��� ���������� ȣ��ȴ�.
// |x, y, r,g,b,a|,|x, y, r,g,b,a| ,|x, y, r,g,b,a| ,|x, y, r,g,b,a| 
//   0~7  8~11
// |x, y, r,g,b,a, u,v|,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| ,|x, y, r,g,b,a, u,v| 
//   0~7  8~11   24~32
bool     TDxObject3D::CreateInputLayout(ID3D11Device* pd3dDevice)
{
	const D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POS",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"NOR",0,	DXGI_FORMAT_R32G32B32_FLOAT,		0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"COL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,			0,24,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"TEX",0,DXGI_FORMAT_R32G32_FLOAT,				0,40,D3D11_INPUT_PER_VERTEX_DATA,0 },
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
void     TDxObject3D::PreRender(ID3D11DeviceContext* pContext)
{
	//UpdateVertexBuffer();
	// ������� �ʴ� ������������ ����Ʈ �� �Ǵ� null���ȴ�.	
	UINT StartSlot = 0;
	UINT NumBuffers = 1;
	UINT pStrides = sizeof(PNCT_Vertex); // 1���� ���� ũ��
	UINT pOffsets = 0; // ���ۿ� ���� �ε���
	pContext->IASetVertexBuffers(StartSlot, NumBuffers, &m_pVertexBuffer, &pStrides, &pOffsets);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	pContext->VSSetShader(m_pShader->m_pVertexShader, nullptr, 0);
	pContext->PSSetShader(m_pShader->m_pPixelShader, nullptr, 0);
}
void     TDxObject3D::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(m_vList.size(), 0);
}
void     TDxObject3D::InRender(ID3D11DeviceContext* pContext)
{
}
void     TDxObject3D::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
}
void	 TDxObject3D::Release()
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