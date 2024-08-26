#include "TShapeObj.h"
bool   TShapeObj::Create(std::wstring texName, std::wstring hlsl)
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
void   TShapeObj::PreRender(ID3D11DeviceContext* pContext)
{
	TDxObject3D::PreRender(pContext);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
}
void TShapeObj::SetMatrix(FMatrix* pWorld, FMatrix* pView, FMatrix* pProj)
{
	if(pWorld!= nullptr)
		m_matWorld = *pWorld;
	if (pView != nullptr)
		m_matView = *pView;
	if (pProj != nullptr)
		m_matProj = *pProj;	

	m_cb.g_matWorld = m_matWorld.Transpose();
	m_cb.g_matView = m_matView.Transpose();
	m_cb.g_matProj = m_matProj.Transpose();

	if (m_pConstantBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0,
			NULL, &m_cb, 0, 0);
	}
}
bool TShapeObj::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
	// 버퍼 할당 크기를 세팅한다.
	D3D11_BUFFER_DESC  bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(cb);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// 할당된 버퍼에 
	// 시스템메모리가 ->GPU메모리로 체워지는 데이터
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