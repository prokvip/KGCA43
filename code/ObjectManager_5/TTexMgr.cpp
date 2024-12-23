#include "TTexMgr.h"

void TTexture::Release()
{
	m_pSRV.Reset();
	m_pTexture.Reset();
}
bool TTexture::Load(ID3D11Device* pd3dDevice, std::wstring filename)
{
	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			pd3dDevice,
			filename.c_str(),
			m_pTexture.GetAddressOf(),//&m_pTexture
			m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}