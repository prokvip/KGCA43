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
	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// �ؽ�ó(�̹���) �ε� ������
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
public:
	void Release();
	bool Load(TLoadData ld);
};
// TTexture��ü�� ����Ʈ�� �ϴ� �̱��� TTexMgr ����
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;
private:
	TTexMgr(){}
public:
	static void SaveFile(T_STR name, ID3D11Texture2D* pRes);
};
#define I_Tex TTexMgr::Get()
