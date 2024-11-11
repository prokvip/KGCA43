#include "TSkyObject.h"
#include "TTexMgr.h"
HRESULT  TSkyObject::SetRasterizerState()
{
	HRESULT hr = S_OK;
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.DepthClipEnable = TRUE;	
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	hr = TDevice::m_pd3dDevice->CreateRasterizerState(&rd,
		m_pRSCullNone.GetAddressOf());
	if (FAILED(hr)) return hr;

	return hr;
}
HRESULT  TSkyObject::SetSamplerState()
{
	HRESULT hr = S_OK;
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.MaxLOD = FLT_MAX;
	sd.MinLOD = FLT_MIN;
	hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, m_pSSPoint.GetAddressOf());
	if (FAILED(hr)) return hr;

	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = TDevice::m_pd3dDevice->CreateSamplerState(&sd, m_pSSLinear.GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
bool   TSkyBox::Create(std::wstring texName, std::wstring hlsl)
{
	SetSamplerState();
	SetRasterizerState();
	//return m_Mesh.Create(texName, hlsl);
	std::vector<std::wstring> texList = 
	{
		L"..\\..\\data\\sky\\st00_cm_front.bmp",
		L"..\\..\\data\\sky\\st00_cm_back.bmp",
		L"..\\..\\data\\sky\\st00_cm_right.bmp",
		L"..\\..\\data\\sky\\st00_cm_left.bmp",
		L"..\\..\\data\\sky\\st00_cm_up.bmp",
		L"..\\..\\data\\sky\\st00_cm_down.bmp"
	};	
	return Create(texList, hlsl);
};
bool   TSkyBox::Create(std::vector<std::wstring> texList, std::wstring hlsl)
{
	SetRasterizerState();
	for (int i = 0; i < texList.size(); i++)
	{
		m_pSRV[i] = I_Tex.Load(texList[i]).get();
	}
	return m_Mesh.Create(texList[0], hlsl);
};
void   TSkyBox::SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj)
{
	T::TMatrix matView = *pView;
	matView._41 = 0.0f;
	matView._42 = 0.0f;
	matView._43 = 0.0f;
	m_Mesh.SetMatrix(pWorld, &matView, pProj);
};
void   TSkyBox::Frame() {};
void   TSkyBox::Render()
{
	ID3D11RasterizerState* pSaveRS;
	TDevice::m_pContext->RSGetState(&pSaveRS);
	TDevice::m_pContext->RSSetState(m_pRSCullNone.Get());
	m_Mesh.PreRender(TDevice::m_pContext);
	PostRender(TDevice::m_pContext);
	TDevice::m_pContext->RSSetState(pSaveRS);
};
void   TSkyBox::Release() {};
void   TSkyBox::PostRender(ID3D11DeviceContext* pContext)
{
	TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSPoint.GetAddressOf());
	for (int i = 0; i < 6; i++)
	{
		pContext->PSSetShaderResources(0, 1, m_pSRV[i]->m_pSRV.GetAddressOf());

		if (m_Mesh.m_pIndexBuffer != nullptr)
		{
			pContext->DrawIndexed(
				6, 6*i, 0);
		}
		else
		{
			// ONLY VB
			pContext->Draw(
				6, 6 * i);
		}
	}
	TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSLinear.GetAddressOf());
}