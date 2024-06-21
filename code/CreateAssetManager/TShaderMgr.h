#pragma once
#include "TAssetManager.h"
class TShader : public TResource
{
public:
	RUNTIME_DECLARE(TShader)
public:
	std::wstring m_csName;
	ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
	ComPtr<ID3DBlob> VS_Bytecode = nullptr;// 오브젝트 파일
	ComPtr<ID3DBlob> PS_Bytecode = nullptr;// 오브젝트 파일
public:
	void Init(std::wstring name) override;
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

