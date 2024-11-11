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
	//m_pCubeSRV.Attach(
	//	I_Tex.Load(L"..\\..\\data\\sky\\grassenvmap1024.dds")->m_pSRV.Get());
	m_pCubeSRV.Attach(
		I_Tex.Load(L"..\\..\\data\\sky\\Default_reflection.dds")->m_pSRV.Get());

	return Create(texList, hlsl);
};
bool   TSkyBox::Create(std::vector<std::wstring> texList, std::wstring hlsl)
{
	SetRasterizerState();
	for (int i = 0; i < texList.size(); i++)
	{
		m_pTex[i] = I_Tex.Load(texList[i]).get();
		m_pSRVArray[i].Attach(m_pTex[i]->m_pSRV.Get());
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
	PostRenderCube(TDevice::m_pContext);
	TDevice::m_pContext->RSSetState(pSaveRS);
};
void   TSkyBox::Release() {};
void   TSkyBox::PostRender1(ID3D11DeviceContext* pContext)
{
	pContext->PSSetSamplers(0, 1, m_pSSPoint.GetAddressOf());
	pContext->PSSetShaderResources(1, 6, m_pSRVArray[0].GetAddressOf());
	if (m_Mesh.m_pIndexBuffer != nullptr)
	{
		pContext->DrawIndexed(m_Mesh.m_vIndexList.size(), 0, 0);
	}
	else
	{
		// ONLY VB
		pContext->Draw(m_Mesh.m_vVertexList.size(), 0);
	}
	TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSLinear.GetAddressOf());
}
void   TSkyBox::PostRenderCube(ID3D11DeviceContext* pContext)
{
	pContext->PSSetSamplers(0, 1, m_pSSLinear.GetAddressOf());
	pContext->PSSetSamplers(1, 1, m_pSSPoint.GetAddressOf());
	pContext->PSSetShaderResources(3, 1, m_pCubeSRV.GetAddressOf());
	if (m_Mesh.m_pIndexBuffer != nullptr)
	{
		pContext->DrawIndexed(m_Mesh.m_vIndexList.size(), 0, 0);
	}
	else
	{
		// ONLY VB
		pContext->Draw(m_Mesh.m_vVertexList.size(), 0);
	}
	TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSLinear.GetAddressOf());
}
void   TSkyBox::PostRender6(ID3D11DeviceContext* pContext)
{
	TDevice::m_pContext->PSSetSamplers(0, 1, m_pSSPoint.GetAddressOf());
	for (int i = 0; i < 6; i++)
	{
		pContext->PSSetShaderResources(0, 1, m_pTex[i]->m_pSRV.GetAddressOf());

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