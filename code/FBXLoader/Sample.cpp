#include "Sample.h"
void   Sample::Init()
{		
	m_fbxLoader.Init();	
	////tModel tModel1;
	////if (m_fbxLoader.Load("../../data/fbx/box.fbx", tModel1))
	////{
	////	m_pModelList.push_back(tModel1);
	////}
	////tModel tModel2;
	////if (m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx", tModel2))
	////{
	////	m_pModelList.push_back(tModel2);
	////}

	////// 서브매터리얼 사용샘플
	////tModel tModel3;
	////if (m_fbxLoader.Load("../../data/fbx/MultiCameras.fbx", tModel3))
	////{
	////	m_pModelList.push_back(tModel3);
	////}
	////tModel tModel4;
	////if (m_fbxLoader.Load("../../data/fbx/ship.fbx", tModel4))
	////{
	////	m_pModelList.push_back(tModel4);
	////}
	/*tModel tModel5;
	if (m_fbxLoader.Load("../../data/fbx/Sphere.fbx", tModel5))
	{
		m_pModelList.push_back(tModel5);
	}*/
	tModel tModel6;
	if (m_fbxLoader.Load("../../data/fbx/sphereBox.fbx", tModel6))
	{
		m_pModelList.push_back(tModel6);
	}
	/*	m_fbxLoader.Load("../../data/fbx/sphereBox.fbx");*/

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
				std::wstring ext = szFileExt;

				if (ext == L".tga")
				{
					name += L".dds";
				}else
				{ 
					name += szFileExt;
				}				
			}
			m_pModelList[iFbx][iObj]->Create(
				name,
				L"vertexcolor.hlsl");
			
		}
	}
	T::TVector3 eye = { 0.0f, 0.0f, -300.0f };
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
}
void    Sample::Render()
{
	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++)
	{
		auto pModel = m_pModelList[iFbx];
		for (int iObj = 0; iObj < pModel.size(); iObj++)
		{
			T::TMatrix matWorld;
			D3DXMatrixTranslation(&matWorld, 0, 0, 0);
			m_pModelList[iFbx][iObj]->SetMatrix(
				nullptr, &m_MainCamera.m_matView, &m_matProj);
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
