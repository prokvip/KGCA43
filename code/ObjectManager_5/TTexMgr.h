#pragma once
#include "TBaseManager.h"
class TTexture : public TResource
{
public:
	std::wstring m_csName;
	// 렌더링 파이브라인(픽쉘쉐이더)에 전송 데이터
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// 텍스처(이미지) 로드 데이터
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
	void Release()
	{
		m_pSRV.Reset();
		m_pTexture.Reset();
	}
	bool Load(ID3D11Device* pd3dDevice, std::wstring filename)
	{
		//m_csName = filename;
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
};
// TTexture객체를 리스트로 하는 싱글톤 TTexMgr 생성
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;

private:
	TTexMgr(){}
};
#define I_Tex TTexMgr::Get()
