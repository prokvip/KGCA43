#include "TPlaneObj.h"
bool   TPlaneObj::CreateConstantBuffer(ID3D11Device* pd3dDevice)
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
void   TPlaneObj::SetVertexData()
{
	//local
	m_vList.resize(6);
	// 0,5    1

	// 4     2,3
	m_vList[0].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vList[0].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[0].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[0].t = T_Math::FVector2(0.0f, 0.0f);

	m_vList[1].p = T_Math::FVector3(1.0f, 1.0f, 0.0f);
	m_vList[1].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[1].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[1].t = T_Math::FVector2(1.0f, 0.0f);

	m_vList[2].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vList[2].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[2].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[2].t = T_Math::FVector2(1.0f, 1.0f);


	m_vList[3].p = T_Math::FVector3(1.0f, -1.0f, 0.0f);
	m_vList[3].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[3].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[3].t = T_Math::FVector2(1.0f, 1.0f);

	m_vList[4].p = T_Math::FVector3(-1.0f, -1.0f, 0.0f);
	m_vList[4].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[4].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[4].t = T_Math::FVector2(0.0f, 1.0f);

	m_vList[5].p = T_Math::FVector3(-1.0f, 1.0f, 0.0f);
	m_vList[5].n = T_Math::FVector3(0.0f, 0.0f, 0.0f);
	m_vList[5].c = T_Math::FVector4(1.0f, 1.0f, 1.0f, 1.0f);
	m_vList[5].t = T_Math::FVector2(0.0f, 0.0f);

}
bool   TPlaneObj::Create(std::wstring texName, std::wstring hlsl)
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
	m_matWorld.Translation(0.0f, 0.0f, 0);

	FVector3 eye = { 0.0f, 0.0f, -50.0f };
	FVector3 target = { 0.0f, 0.0f, 0.0f };
	FVector3 up = { 0.0f, 1.0f, 0.0f };
	m_matView = FMatrix::CreateViewTransform(eye, target, up);
	m_matProj = FMatrix::CreateProjTransform(1.0f, 100.0f,
		TBASIS_PI * 0.25f, (float)g_xClientSize / (float)g_yClientSize);
	m_cb.g_matWorld = m_matWorld.Transpose();
	m_cb.g_matView = m_matView.Transpose();
	m_cb.g_matProj = m_matProj.Transpose();

	if (CreateConstantBuffer(m_pd3dDevice) == false)
	{
		Release();
		return false;
	}
	return true;
}
void   TPlaneObj::PreRender(ID3D11DeviceContext* pContext)
{
	TDxObject3D::PreRender(pContext);
	pContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());
}
void   TPlaneObj::Frame() 
{
	/*static FVector3 vPos = { 0, 0, 0 };
	vPos.X = cosf(g_fGameTime) * 2.0f;*/
	
	FMatrix  matScale, matTrans, matRotate;
	matTrans.Translation(m_vPos);
	matScale.Scale(3.0f, 3.0f, 3.0f);
	//matRotate.rotateZ(g_fGameTime);
	m_matWorld = matScale * matRotate * matTrans;

	FVector3 eye = { 0.0f, 0.0f, -10.0f };// cosf(g_fGameTime) * 10.0f - 10.0f
	FVector3 target = { 0.0f, 0.0f, 10.0f };
	FVector3 up = { 0.0f, 1.0f, 0.0f };
	m_matView = FMatrix::CreateViewTransform(eye, target, up);
	m_matProj = FMatrix::CreateProjTransform(0.1f, 100.0f,
		TBASIS_PI * 0.25f, (float)g_xClientSize / (float)g_yClientSize);

	m_cb.g_matWorld = m_matWorld.Transpose();
	m_cb.g_matView = m_matView.Transpose();
	m_cb.g_matProj = m_matProj.Transpose();
	if (m_pConstantBuffer != nullptr)
	{
		m_pContext->UpdateSubresource(m_pConstantBuffer.Get(), 0,
			NULL, &m_cb, 0, 0);
	}
}