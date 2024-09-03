#include "Sample.h"
void   Sample::Init()
{		
	m_fbxLoader.Init();	
	tModel tModel1;
	if (m_fbxLoader.Load("../../data/fbx/box.fbx", tModel1))
	{
		m_pModelList.push_back(tModel1);
	}
	tModel tModel2;
	if (m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx", tModel2))
	{
		m_pModelList.push_back(tModel2);
	}
	tModel tModel3;
	if (m_fbxLoader.Load("../../data/fbx/ship.fbx", tModel3))
	{
		m_pModelList.push_back(tModel3);
	}

	/*m_fbxLoader.Load("../../data/fbx/MultiCameras.fbx");
	m_fbxLoader.Load("../../data/fbx/ship.fbx");
	m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx");
	m_fbxLoader.Load("../../data/fbx/Greystone.fbx");*/

	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++ )
	{
		for (int iObj = 0; iObj < m_pModelList[iObj].size(); iObj++)
		{
			m_pModelList[iFbx][iObj]->Create(L"../../data/1234.jpg", L"../../data/shader/pnct.hlsl");
		}
	}
	
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
		for (int iObj = 0; iObj < m_pModelList[iObj].size(); iObj++)
		{
			m_pModelList[iFbx][iObj]->SetMatrix(nullptr, &m_MainCamera.m_matView, &m_matProj);
			m_pModelList[iFbx][iObj]->Render(TDevice::m_pContext);			
		}
	}
}
void    Sample::Release()
{
	for (int iFbx = 0; iFbx < m_pModelList.size(); iFbx++)
	{
		for (int iObj = 0; iObj < m_pModelList[iObj].size(); iObj++)
		{
			m_pModelList[iFbx][iObj]->Release();
		}
	}
	m_fbxLoader.Release();
}

T_GAME_START(800, 600);
