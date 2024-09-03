#include "TDxObj3D.h"
/// <summary>
/// 
/// </summary>
void   TDxObject3D::UpdateVertexBuffer()
{
	//// NDC ��ǥ�� ��ȯ
	//for (int i = 0; i < m_vVertexList.size(); i++)
	//{
	//	m_vListNDC[i].p = ConvertScreenToNDC(m_vVertexList[i].p);
	//	m_vListNDC[i].c = m_vVertexList[i].c;
	//	m_vListNDC[i].t = m_vVertexList[i].t;
	//}
	//if (m_pVertexBuffer != nullptr)
	//{
	//	m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);
	//}
}
void   TDxObject3D::Frame()
{

}
bool   TDxObject3D::Create(std::wstring texName, std::wstring hlsl)
{
	m_pd3dDevice = TDevice::m_pd3dDevice.Get();
	m_pContext = TDevice::m_pContext;

	m_szShaderFilename = hlsl;

	m_pTexture = I_Tex.Load(texName).get();
	if (m_pTexture != nullptr)
	{
		m_pSRV = m_pTexture->m_pSRV;
	}

	SetVertexData();
	if (CreateVertexBuffer(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	SetIndexData();
	if (CreateIndexBuffer(m_pd3dDevice) == false)
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
	if (CreateConstantBuffer(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	return true;
}
bool     TDxObject3D::CreateVertexBuffer(ID3D11Device* pd3dDevice)
{
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_Vertex) * m_vVertexList.size();
	// ���ῡ �뵵�� ����? 
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vVertexList.at(0);

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		&this->m_pVertexBuffer);
	if (FAILED(hr)) return false;
	return true;
}

bool     TDxObject3D::CreateIndexBuffer(ID3D11Device* pd3dDevice)
{
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * m_vIndexList.size();
	// ���ῡ �뵵�� ����? 
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vIndexList.at(0);

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		&this->m_pIndexBuffer);
	if (FAILED(hr)) return false;
	return true;
}

bool	 TDxObject3D::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	// ���� �Ҵ� ũ�⸦ �����Ѵ�.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(FRAME_CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// �Ҵ�� ���ۿ� 
	// �ý��۸޸𸮰� ->GPU�޸𸮷� ü������ ������
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_cb;

	HRESULT hr = pd3dDevice->CreateBuffer(
		&bd,
		&sd,
		this->m_pConstantBuffer.GetAddressOf());
	if (FAILED(hr)) return false;
	return true;
}
//
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
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pContext->IASetInputLayout(m_pVertexLayout);
	//pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->VSSetShader(m_pShader->m_pVertexShader, nullptr, 0);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
	pContext->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	pContext->PSSetShader(m_pShader->m_pPixelShader, nullptr, 0);
}
void     TDxObject3D::PostRender(ID3D11DeviceContext* pContext)
{	
	if (m_pIndexBuffer != nullptr)
	{
		pContext->DrawIndexed(m_vIndexList.size(), 0, 0);
	}
	else
	{
		// ONLY VB
		pContext->Draw(m_vVertexList.size(), 0);
	}
}
void TDxObject3D::SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
{
	if (pWorld != nullptr)
		m_matWorld = *pWorld;
	if (pView != nullptr)
		m_matView = *pView;
	if (pProj != nullptr)
		m_matProj = *pProj;

	/*m_cb.g_matWorld = m_matWorld.Transpose();
	m_cb.g_matView = m_matView.Transpose();
	m_cb.g_matProj = m_matProj.Transpose();*/

	T::D3DXMatrixTranspose(&m_cb.g_matWorld, &m_matWorld);
	T::D3DXMatrixTranspose(&m_cb.g_matView, &m_matView);
	T::D3DXMatrixTranspose(&m_cb.g_matProj, &m_matProj);

	if (m_pConstantBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0,
			NULL, &m_cb, 0, 0);
	}
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