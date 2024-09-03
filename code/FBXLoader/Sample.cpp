#include "Sample.h"
void   Sample::Init()
{		
	m_fbxLoader.Init();
	m_fbxLoader.Load("../../data/fbx/box.fbx"); 
	m_fbxLoader.Load("../../data/fbx/MultiCameras.fbx");
	m_fbxLoader.Load("../../data/fbx/ship.fbx");
	m_fbxLoader.Load("../../data/fbx/SM_Rock.fbx");
	m_fbxLoader.Load("../../data/fbx/Greystone.fbx");
}
void  Sample::Reset()
{
}
void  Sample::PreRender()
{
}
void    Sample::Frame()
{		
	m_fbxLoader.Frame();
}
void    Sample::Render()
{
	m_fbxLoader.Render();
}
void    Sample::Release()
{
	m_fbxLoader.Release();
}

T_GAME_START(800, 600);
