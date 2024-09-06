#include "Sample.h"
void   Sample::Init()
{		
	m_fbxLoader.Init();	
	/*tModel tModel1;
	if (m_fbxLoader.Load("../../data/fbx/box.fbx", tModel1))
	{
		m_pModelList.push_back(tModel1);
	}
	tModel tModel2;
	if (m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx", tModel2))
	{
		m_pModelList.push_back(tModel2);
	}*/

	// ������͸��� ������
	tModel tModel3;
	if (m_fbxLoader.Load("../../data/fbx/MultiCameras.fbx", tModel3))
	{
		m_pModelList.push_back(tModel3);
	}
	/*tModel tModel4;
	if (m_fbxLoader.Load("../../data/fbx/ship.fbx", tModel4))
	{
		m_pModelList.push_back(tModel4);
	}*/
	/*m_fbxLoader.Load("../../data/fbx/MultiCameras.fbx");
	m_fbxLoader.Load("../../data/fbx/ship.fbx");
	m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx");
	m_fbxLoader.Load("../../data/fbx/Greystone.fbx");*/

	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++ )
	{
		auto pModel = m_pModelList[iFbx];
		for (int iObj = 0; iObj < pModel.size(); iObj++)
		{
			std::wstring name= L"../../data/";			
			if (m_pModelList[iFbx][iObj]->m_szTexFileName.size() > 0)
			{
				std::wstring texPath = to_mw(m_pModelList[iFbx][iObj]->m_szTexFileName[0]);
				wchar_t  szDrive[MAX_PATH] = { 0, };
				wchar_t  szDir[MAX_PATH] = { 0, };
				wchar_t  szFileName[MAX_PATH] = { 0, };
				wchar_t  szFileExt[MAX_PATH] = { 0, };
				_tsplitpath_s(texPath.c_str(), szDrive, szDir, szFileName, szFileExt);
				name += szFileName;
				name += szFileExt;
			}
			m_pModelList[iFbx][iObj]->Create(
				name,
				L"vertexcolor.hlsl");
			
		}
	}
	T::TVector3 eye = { 0.0f, 0.0f, -300.0f };
	T::TVector3 target = { 0.0f, 0.0f, 0.0f };
	T::TVector3 up = { 0.0f, 1.0f, 0.0f };
	// ���� '=': ������ �ǿ����ڷ� 'T_Math::FMatrix' ������ ����ϴ� �����ڰ� ���ų� ���Ǵ� ��ȯ�� �����ϴ�.
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
}
void    Sample::Render()
{
	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++)
	{
		auto pModel = m_pModelList[iFbx];
		for (int iObj = 0; iObj < pModel.size(); iObj++)
		{
			T::TMatrix matWorld;
			D3DXMatrixTranslation(&matWorld, 0.0f, 0, 0);
			m_pModelList[iFbx][iObj]->SetMatrix(&matWorld, &m_MainCamera.m_matView, &m_matProj);
			m_pModelList[iFbx][iObj]->Render(TDevice::m_pContext);			
		}
	}
}
void    Sample::Release()
{
	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++)
	{
		auto pModel = m_pModelList[iFbx];
		for (int iObj = 0; iObj < pModel.size(); iObj++)
		{
			m_pModelList[iFbx][iObj]->Release();
		}
	}
	m_fbxLoader.Release();
}

T_GAME_START(800, 600);