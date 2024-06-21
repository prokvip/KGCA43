#include "TTexMgr.h"
RUNTIME_IMPLEMENT(TTexture)
//TResource* TTexture::CreateObject() { return new TTexture; };
//CRuntimeClass TTexture::classTTexture = { "TTexture", sizeof(TTexture), TTexture::CreateObject };
//CRuntimeClass* TTexture::GetRuntimeClass() const { return &classTTexture; }
void TTexture::Init(std::wstring name)
{
	m_csName = name;
}
void TTexture::Release()
{
	m_pSRV.Reset();
	m_pTexture.Reset();
}
bool TTexture::Load(std::wstring filename)
{
	HRESULT hr =
		DirectX::CreateWICTextureFromFile(
			TDevice::m_pd3dDevice.Get(),
			filename.c_str(),
			m_pTexture.GetAddressOf(),//&m_pTexture
			m_pSRV.GetAddressOf());
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}