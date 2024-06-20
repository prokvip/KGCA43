#pragma once
#include "TBaseManager.h"
class TTexture : public TResouce
{
	ID3D11Device* m_pd3dDevice = nullptr;
public:
	virtual void Set(ID3D11Device* pDevice) override
	{
		m_pd3dDevice = pDevice;
	};
public:
	std::wstring m_csName;
	// 렌더링 파이브라인(픽쉘쉐이더)에 전송 데이터
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// 텍스처(이미지) 로드 데이터
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
public:
	void Release();
	bool Load(std::wstring filename);
};
// TTexture객체를 리스트로 하는 싱글톤 TTexMgr 생성
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;
private:
	TTexMgr(){}
};
#define I_Tex TTexMgr::Get()
