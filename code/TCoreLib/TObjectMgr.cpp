#include "TObjectMgr.h"

void TObject::Release()
{
	m_pdxObj->Release();
}

bool TObject::Load(TLoadData ld)
{
	std::wstring filename = ld.m_csLoadFileName;
	m_csName = filename;
	wchar_t  szDrive[MAX_PATH] = { 0, };
	wchar_t  szDir[MAX_PATH] = { 0, };
	wchar_t  szFileName[MAX_PATH] = { 0, };
	wchar_t  szFileExt[MAX_PATH] = { 0, };
	_tsplitpath_s(filename.c_str(), szDrive, szDir, szFileName, szFileExt);
	std::wstring ext = szFileExt;
	if (_tcsicmp(szFileExt, L".kgc") == 0)
	{
		auto model = std::make_shared<TKgcObject>();
		model->Load(ld);
		m_pdxObj = std::dynamic_pointer_cast<TDxObject>(model);
		return true;
	}
	/*if (_tcsicmp(szFileExt, L".fbx") == 0 )
	{
		std::wstring expFilename;
		auto tKgcFileModel = std::make_shared<TKgcFileFormat>();
		if (m_fbxLoader.Load("../../data/fbx/Man.fbx",
			tKgcFileModel.get()))
		{
			expFilename = ExportPath(L"../../data/fbx/Man.fbx",
				L"D:\\00_43\\data\\kgc\\");
			TKgcFileFormat::Export(tKgcFileModel.get(),
				expFilename);
		}
	}*/
	/*auto model = std::make_shared<TFbxModel>();
	if (TKgcFileFormat::Import(filename, L"Lightting.hlsl",	model))
	{
		m_pdxObj = std::dynamic_pointer_cast<TDxObject>(model);
		if (m_pdxObj == nullptr) return false;
		return true;
	}*/
	return false;
};