#pragma once
#include "TBaseManager.h"
#include "WicTextureLoader.h"
#include "DDSTextureLoader.h"
#include <wincodec.h>
#include <wincodecsdk.h>
#include "ScreenGrab.h"
#pragma comment (lib,"windowscodecs.lib")

class TTexture : public TResource
{
	ID3D11Device* m_pd3dDevice = nullptr;
public:
	virtual void Set(ID3D11Device* pDevice) override
	{
		m_pd3dDevice = pDevice;
	};
public:
	D3D11_TEXTURE2D_DESC td;
	std::wstring m_csName;
	// 렌더링 파이브라인(픽쉘쉐이더)에 전송 데이터
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// 텍스처(이미지) 로드 데이터
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
public:
	void Release();
	bool Load(TLoadData ld);
};
// TTexture객체를 리스트로 하는 싱글톤 TTexMgr 생성
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;
private:
	TTexMgr(){}
public:
	static void SaveFile(T_STR name, ID3D11Texture2D* pRes);
};
#define I_Tex TTexMgr::Get()
