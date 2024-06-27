#include "TGame.h"
#include "TSceneTitle.h"
#include "TSceneLobby.h"
#include "TSceneIngame.h"
#include "TSceneResult.h"
#include "TSceneStart.h"
#include "TSceneEnd.h"

TScene* TGame::Transition(TScene* pScene, int iEvent)
{
	int iOutState = m_pSceneFSM->GetTransition(pScene->m_iStateIndex, iEvent);
	m_pCurrentScene = (*this)[iOutState];
	return m_pCurrentScene;
}
void   TGame::Init()
{
	m_pSceneFSM = std::make_shared<TFiniteStateMachine>();
	//// GAME_START	-> EVENT_TIMER			-> GAME_TITLE
	//// GAME_TITLE	-> EVENT_CLICK			-> GAME_INGAME
	//// GAME_TITLE	-> EVENT_TIMER:3		-> GAME_INGAME
	//// GAME_INGAME	-> EVENT_TIMER:30		-> GAME_END
	m_pSceneFSM->AddStateTransition(GAME_START, EVENT_AUTO, GAME_TITLE);
	m_pSceneFSM->AddStateTransition(GAME_START, EVENT_TIMER, GAME_TITLE);

	m_pSceneFSM->AddStateTransition(GAME_TITLE, EVENT_TIMER, GAME_LOBBY);
	m_pSceneFSM->AddStateTransition(GAME_TITLE, EVENT_CLICK, GAME_LOBBY);
	m_pSceneFSM->AddStateTransition(GAME_TITLE, EVENT_GAME_EXIT, GAME_END);

	m_pSceneFSM->AddStateTransition(GAME_LOBBY, EVENT_CLICK, GAME_INGAME);
	m_pSceneFSM->AddStateTransition(GAME_LOBBY, EVENT_GAME_EXIT, GAME_END);

	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_TIMER, GAME_RESULT);	
	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_MISSION_COMPLETE, GAME_RESULT);
	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_MISSION_FAILED, GAME_RESULT);

	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_RECHALLENGE, GAME_INGAME);
	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_GAME_EXIT, GAME_END);
	m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_SCENE_EXIT, GAME_LOBBY);

	m_pSceneFSM->AddStateTransition(GAME_RESULT, EVENT_TIMER, GAME_LOBBY);
	m_pSceneFSM->AddStateTransition(GAME_RESULT, EVENT_CLICK, GAME_LOBBY);
	m_pSceneFSM->AddStateTransition(GAME_RESULT, EVENT_RECHALLENGE, GAME_INGAME);
	m_pSceneFSM->AddStateTransition(GAME_RESULT, EVENT_GAME_EXIT, GAME_END);
	m_pSceneFSM->AddStateTransition(GAME_RESULT, EVENT_SCENE_EXIT, GAME_LOBBY);

	AddScene(std::make_shared<TSceneStart>(this), GAME_START);
	AddScene(std::make_shared<TSceneTitle>(this), GAME_TITLE);
	AddScene(std::make_shared<TSceneLobby>(this), GAME_LOBBY);
	AddScene(std::make_shared<TSceneIngame>(this), GAME_INGAME);
	AddScene(std::make_shared<TSceneResult>(this), GAME_RESULT);
	AddScene(std::make_shared<TSceneEnd>(this), GAME_END);

	/*m_pStartScene = std::make_shared<TSceneStart>(this);
	m_pTitleScene = std::make_shared<TSceneTitle>(this);
	m_pIngameScene = std::make_shared<TSceneIngame>(this);
	m_pEndScene = std::make_shared<TSceneEnd>(this);	
	m_pResultScene = std::make_shared<TSceneResult>(this);
	m_pLobbyScene = std::make_shared<TSceneLobby>(this);*/

	/*m_SceneList[m_pStartScene->m_iStateIndex] = m_pStartScene.get();
	m_SceneList[m_pTitleScene->m_iStateIndex] = m_pTitleScene.get();
	m_SceneList[m_pLobbyScene->m_iStateIndex] = m_pLobbyScene.get();
	m_SceneList[m_pIngameScene->m_iStateIndex] = m_pIngameScene.get();
	m_SceneList[m_pResultScene->m_iStateIndex] = m_pResultScene.get();
	m_SceneList[m_pEndScene->m_iStateIndex] = m_pEndScene.get();*/

	//m_pStartScene->Init();
	//m_pEndScene->Init();
	//m_pTitleScene->Init();
	//m_pLobbyScene->Init();
	//m_pIngameScene->Init();
	//m_pResultScene->Init();

	m_pCurrentScene = (*this)[GAME_START];

}
void    TGame::Frame()
{	
	m_pCurrentScene->Execute();
	m_pCurrentScene->Frame();
}
void    TGame::Render()
{
	m_pCurrentScene->Render();	
}
void    TGame::Release()
{
	for (auto scene : m_SceneList)
	{
		scene.second->Release();
	}
	m_SceneList.clear();

	/*m_pStartScene->Release();
	m_pTitleScene->Release();
	m_pIngameScene->Release();
	m_pEndScene->Release();
	m_pResultScene->Release();
	m_pLobbyScene->Release();*/
}
