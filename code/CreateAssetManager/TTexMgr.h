#pragma once
#include "TAssetManager.h"
class TTexture : public TResource
{
public:
	RUNTIME_DECLARE(TTexture)
public:
	std::wstring m_csName;
	// ������ ���̺����(�Ƚ����̴�)�� ���� ������
	ComPtr<ID3D11ShaderResourceView> m_pSRV = nullptr;
	// �ؽ�ó(�̹���) �ε� ������
	ComPtr<ID3D11Resource> m_pTexture = nullptr;
public:
	void Init(std::wstring name) override;
	void Release()override;
	bool Load( std::wstring filename);
};
// TTexture��ü�� ����Ʈ�� �ϴ� �̱��� TTexMgr ����
class TTexMgr : public TBaseManager<TTexture,TTexMgr>
{
	friend class TSingleton<TTexMgr>;
private:
	TTexMgr(){}
};
#define I_Tex TTexMgr::Get()
