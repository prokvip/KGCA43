#include "TTexMgr.h"

void TTexture::Release()
{
	m_pSRV.Reset();
	m_pTexture.Reset();
}
bool TTexture::Load(std::wstring filename)
{
	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			m_pd3dDevice,
			filename.c_str(),
			m_pTexture.GetAddressOf(),//&m_pTexture
			m_pSRV.GetAddressOf());

	if (FAILED(hr))
	{
		hr =DirectX::CreateDDSTextureFromFile(
				m_pd3dDevice,
				filename.c_str(),
				m_pTexture.GetAddressOf(),//&m_pTexture
				m_pSRV.GetAddressOf());
		if (FAILED(hr))
		{
			return false;
		}		
	}

	ID3D11Texture2D* pTex = (ID3D11Texture2D*)m_pTexture.Get();
	pTex->GetDesc(&td);
	return true;
}

void TTexMgr::SaveFile(T_STR name, ID3D11Texture2D* pRes)
{
	HRESULT hr = S_OK;
	ComPtr<ID3D11Texture2D> tex;
	if (pRes == nullptr)
	{
		hr = TDevice::m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex);
	}
	else
	{
		tex = pRes;
	}
	if (SUCCEEDED(hr))
	{
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatPng, (name + L".png").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatJpeg, (name + L".jpg").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatBmp, (name + L".bmp").c_str(), &GUID_WICPixelFormat16bppBGR565);
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str());
		hr = DirectX::SaveWICTextureToFile(TDevice::m_pContext, tex.Get(), GUID_ContainerFormatTiff, (name + L".tif").c_str(), nullptr, [&](IPropertyBag2* props)
			{
				PROPBAG2 options[2] = { 0, 0 };
				options[0].pstrName = const_cast<WCHAR*>(OLESTR("CompressionQuality"));
				options[1].pstrName = const_cast<WCHAR*>(OLESTR("TiffCompressionMethod"));

				VARIANT varValues[2];
				varValues[0].vt = VT_R4;
				varValues[0].fltVal = 0.75f;

				varValues[1].vt = VT_UI1;
				varValues[1].bVal = WICTiffCompressionNone;

				(void)props->Write(2, options, varValues);
			});

		hr = DirectX::SaveDDSTextureToFile(TDevice::m_pContext, tex.Get(), (name + L".dds").c_str());
	}
	if (pRes == nullptr)
	{
		tex->Release();
	}
}