#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#include "TCore.h"
#include "TFbxLoader.h"

class Sample : public TCore
{	
	std::wstring				m_szSelectModelName;
	TFbxLoader					m_fbxLoader;
	using tModel = std::vector<TFbxModel*>;
	std::vector<tModel>			m_pFbxfileList;
public:
	void   Init() override;
	void   PreRender()override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
	void   Reset() override;	
public:
	std::vector<std::wstring> m_LoadFiles;
	bool   LoadFileDlg(TCHAR* szExt, TCHAR* szTitle);
	Sample() = default;
};