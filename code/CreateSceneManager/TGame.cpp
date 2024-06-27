#include "TGame.h"
#include "TSceneTitle.h"
#include "TSceneLobby.h"
#include "TSceneIngame.h"
#include "TSceneResult.h"



void   TGameObj::Init()
{
	TScene::CreateFSM();

	m_pStartScene = std::make_shared<TSceneStart>(this);
	m_pTitleScene = std::make_shared<TSceneTitle>(this);
	m_pIngameScene = std::make_shared<TSceneIngame>(this);
	m_pEndScene = std::make_shared<TSceneEnd>(this);
	
	m_pResultScene = std::make_shared<TSceneResult>(this);
	m_pLobbyScene = std::make_shared<TSceneLobby>(this);

	m_SceneList[GAME_START] = m_pStartScene.get();
	m_SceneList[GAME_TITLE] = m_pTitleScene.get();
	m_SceneList[GAME_INGAME] = m_pIngameScene.get();
	m_SceneList[GAME_END] = m_pEndScene.get();

	m_pStartScene->Init();
	m_pEndScene->Init();
	m_pTitleScene->Init();
	m_pLobbyScene->Init();
	m_pIngameScene->Init();
	m_pResultScene->Init();

	m_pCurrentScene = m_pStartScene.get();


}
void    TGameObj::Frame()
{	
	m_pCurrentScene->Execute();
	m_pCurrentScene->Frame();
}
void    TGameObj::Render(ID3D11DeviceContext* pContext)
{
	m_pCurrentScene->Render();	
}
void    TGameObj::Release()
{
	m_pCurrentScene->Release();
}
