#pragma once
#include "TBaseManager.h"
class TTexture : public TResource
{
public:
	std::wstring m_csName;
	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// �ؽ�ó(�̹���) �ε� ������
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
// TTexture��ü�� ����Ʈ�� �ϴ� �̱��� TTexMgr ����
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;

private:
	TTexMgr(){}
};
#define I_Tex TTexMgr::Get()
