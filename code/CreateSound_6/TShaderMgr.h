#pragma once
#include "TAssetManager.h"
class TShader : public TResouce
{
public:
	RUNTIME_DECLARE(TShader)
public:
	std::wstring m_csName;
	ID3D11VertexShader* m_pVertexShader = nullptr;
	ID3D11PixelShader* m_pPixelShader = nullptr;
	ID3DBlob* VS_Bytecode = nullptr; // ������Ʈ ����
	ID3DBlob* PS_Bytecode = nullptr; // ������Ʈ ����
public:
	void Release() override;
	bool Load(ID3D11Device* pd3dDevice, std::wstring filename)override;
};


class TShaderMgr : public TBaseManager<TShader, TShaderMgr>
{
	friend class TSingleton<TShaderMgr>;
private:
	TShaderMgr() {}
};
#define I_Shader TShaderMgr::Get()

