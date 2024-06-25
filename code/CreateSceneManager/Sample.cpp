#include "Sample.h"

void   Sample::Init()
{
	m_pTitleScene = std::make_shared<TSceneTitle>();
	m_pLobbyScene = std::make_shared<TSceneLobby>();
	m_pIngameScene = std::make_shared<TSceneIngame>();
	m_pResultScene = std::make_shared<TSceneResult>();

	m_pCurrentScene = m_pIngameScene.get();

	m_pCurrentScene->Init();
}

void    Sample::Frame()
{	
	m_pCurrentScene->Frame();
}
void    Sample::Render() 
{ 		
	m_pCurrentScene->Render();
}
void    Sample::Release() 
{			
	m_pCurrentScene->Release();
}

T_GAME_START(800, 600);