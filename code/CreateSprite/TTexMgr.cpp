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
		return false;
	}

	ID3D11Texture2D* pTex = (ID3D11Texture2D*)m_pTexture.Get();
	pTex->GetDesc(&td);
	return true;
}