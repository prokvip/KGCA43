#pragma once
#include "TBaseManager.h"
class TShader : public TResource
{
	ID3D11Device* m_pd3dDevice = nullptr;
public:
	virtual void Set(ID3D11Device* pDevice) override
	{
		m_pd3dDevice = pDevice;
	};
public:
	std::wstring m_csName;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3DBlob* VS_Bytecode = nullptr; // 오브젝트 파일
	ID3DBlob* PS_Bytecode = nullptr; // 오브젝트 파일
public:
	void Release() override;
	bool Load(std::wstring filename)override;
};


class TShaderMgr : public TBaseManager<TShader, TShaderMgr>
{
	friend class TSingleton<TShaderMgr>;
private:
	TShaderMgr() {}
};
#define I_Shader TShaderMgr::Get()

