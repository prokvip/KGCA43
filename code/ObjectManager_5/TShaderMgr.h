#pragma once
#include "TBaseManager.h"
class TShader
{
public:
	std::wstring m_csName;
};
class TShaderMgr : public TBaseManager<TShader, TShaderMgr>
{
	friend class TSingleton<TShaderMgr>;
private:
	TShaderMgr() {}
};
#define I_Shader TShaderMgr::Get()

