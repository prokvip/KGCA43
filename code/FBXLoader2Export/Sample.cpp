#include "Sample.h"
bool Sample::LoadFileDlg(TCHAR* szExt, TCHAR* szTitle)
{
	OPENFILENAME    ofn;
	TCHAR           szFile[MAX_PATH] = { 0, };
	TCHAR			szFileTitle[MAX_PATH] = { 0, };
	static TCHAR* szFilter;

	TCHAR lpCurBuffer[256] = { 0, };
	GetCurrentDirectory(256, lpCurBuffer);

	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	_tcscpy_s(szFile, _T("*."));
	_tcscat_s(szFile, szExt);
	_tcscat_s(szFile, _T("\0"));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = _T("../../data/Character/");
	ofn.lpstrTitle = szTitle;
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = szExt;

	if (!GetOpenFileName(&ofn))
	{
		return false;
	}
	TCHAR* load = _tcstok(szFile, _T("\n"));
	T_STR dir = szFile;
	load = &load[_tcslen(load) + 1];
	if (*load == 0)
	{
		m_LoadFiles.push_back(dir);
	}

	while (*load != 0)
	{
		T_STR dir = szFile;
		load = _tcstok(load, _T("\n"));
		dir += _T("\\");
		dir += load;
		m_LoadFiles.push_back(dir);
		load = &load[_tcslen(load) + 1];
	}
	SetCurrentDirectory(lpCurBuffer);
	return true;
}
std::wstring   Sample::ExportPath(std::wstring loadfile, std::wstring expPath)
{
	std::wstring ret;
	
	std::wstring texPath = loadfile;
	wchar_t  szDrive[MAX_PATH] = { 0, };
	wchar_t  szDir[MAX_PATH] = { 0, };
	wchar_t  szFileName[MAX_PATH] = { 0, };
	wchar_t  szFileExt[MAX_PATH] = { 0, };
	_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
	
	if (!expPath.empty())
	{
		ret = expPath;
		ret += szFileName;
		ret += L".kgc";
	}
	else
	{
		ret += szDrive;
		ret += szDir;
		ret += szFileName;
		ret += L".kgc";
	}
	return ret;
}
void   Sample::Init()
{
	m_fbxLoader.Init();
	/// <summary>
	/// fbx loader -> Export (*.kgc)
	/// class TKgcFileFormat
	/// </summary>
	std::wstring expFilename;
	auto tKgcFileModel = std::make_shared<TKgcFileFormat>();
	//if (m_fbxLoader.Load("../../data/fbx/Swat@walking.fbx", 
	//if (m_fbxLoader.Load("../../data/fbx/Swat.fbx",
	//if (m_fbxLoader.Load("../../data/fbx/Man.fbx",
	if (m_fbxLoader.Load("../../data/fbx/ThirdPersonIdle.fbx",
		tKgcFileModel.get()))
	{
		expFilename = ExportPath(L"../../data/fbx/ThirdPersonIdle.fbx",
			L"D:\\00_43\\data\\kgc\\");
		TKgcFileFormat::Export(tKgcFileModel.get(),expFilename);
	}

	/// <summary>
	/// kgc import ->  render
	/// class TFbxModel
	/// </summary>
	tModel model1 = std::make_shared<TFbxModel>();
	if (TKgcFileFormat::Import(expFilename,L"vertexcolor.hlsl", model1))
	{
		m_pFbxfileList.emplace_back(model1);
	}
	
	T::TVector3 eye = { 30.0f, 0.0f, 0.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// 이항 '=': 오른쪽 피연산자로 'T_Math::FMatrix' 형식을 사용하는 연산자가 없거나 허용되는 변환이 없습니다.
	m_MainCamera.SetView(eye, target, up);
}
void  Sample::Reset()
{
}
void  Sample::PreRender()
{
}
void    Sample::Frame()
{
	if (TInput::Get().KeyCheck(VK_HOME) == KEY_UP)
	{
		m_LoadFiles.clear();
		/*for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
		{
			auto pModel = m_pFbxfileList[iFbx];
			pModel->Release();
		}
		m_pFbxfileList.clear();*/
		wchar_t szExt[] = L"fbx";
		wchar_t szFileModel[] = L"kgc Viewer";
		if (LoadFileDlg(szExt, szFileModel))
		{
			for (int iLoad = 0; iLoad < m_LoadFiles.size(); iLoad++)
			{
				std::wstring expFilename;
				auto tModel = std::make_shared<TKgcFileFormat>();
				if (m_fbxLoader.Load(to_wm(m_LoadFiles[iLoad]), tModel.get()))
				{
					expFilename = ExportPath(m_LoadFiles[iLoad],
						L"D:\\00_43\\data\\kgc\\");
					TKgcFileFormat::Export(tModel.get(), expFilename);
				}

				std::shared_ptr<TFbxModel> model1 = std::make_shared<TFbxModel>();
				if (TKgcFileFormat::Import(expFilename, L"vertexcolor.hlsl",model1))
				{
					m_pFbxfileList.emplace_back(model1);
				}
			}
		}
	}
	for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
	{
		m_pFbxfileList[iFbx]->Frame();
	}
}
void    Sample::Render()
{
	for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
	{		
		m_pFbxfileList[iFbx]->SetMatrix(nullptr, &m_MainCamera.m_matView, 
												 &m_MainCamera.m_matProj);
		m_pFbxfileList[iFbx]->Render(TDevice::m_pContext);
	}
}
void    Sample::Release()
{
	for (int iFbx = 0; iFbx < m_pFbxfileList.size(); iFbx++)
	{
		auto pModel = m_pFbxfileList[iFbx];
		pModel->Release();
	}
	m_fbxLoader.Release();
}

T_GAME_START(800, 600);
