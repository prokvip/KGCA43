#include "TScene.h"
#include "TGame.h"
std::shared_ptr<TFiniteStateMachine> TScene::m_pSceneFSM;


void    TScene::CreateFSM() {
	if (m_pSceneFSM == nullptr)
	{
		m_pSceneFSM = std::make_shared<TFiniteStateMachine>();
		//// GAME_START	-> EVENT_TIMER			-> GAME_TITLE
		//// GAME_TITLE	-> EVENT_CLICK			-> GAME_INGAME
		//// GAME_TITLE	-> EVENT_TIMER:3		-> GAME_INGAME
		//// GAME_INGAME	-> EVENT_TIMER:30		-> GAME_END
		m_pSceneFSM->AddStateTransition(GAME_START, EVENT_AUTO, GAME_TITLE);
		m_pSceneFSM->AddStateTransition(GAME_START, EVENT_TIMER, GAME_TITLE);
		m_pSceneFSM->AddStateTransition(GAME_TITLE, EVENT_CLICK, GAME_INGAME);
		//m_pSceneFSM->AddStateTransition(GAME_TITLE, EVENT_TIMER, GAME_INGAME);
		m_pSceneFSM->AddStateTransition(GAME_INGAME, EVENT_TIMER, GAME_END);
	}
};
void    TScene::Init(){}
void    TScene::Frame() {};
void    TScene::Render() {};
void    TScene::Release() {};

void    TSceneStart::Execute()
{
	m_fEventTimer += g_fSecondPerFrame;
	if (m_fEventTimer > 3.0f)
	{
		int iOutState =
			m_pSceneFSM->GetTransition(m_iStateIndex, EVENT_TIMER);
		m_pGame->m_pCurrentScene = m_pGame->m_SceneList[iOutState];
		m_fEventTimer = 0.0f;
	}
}
void    TSceneEnd::Execute()
{
	//m_pSceneFSM->
	g_bGameRun = false;
}