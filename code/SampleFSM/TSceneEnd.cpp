#include "TSceneEnd.h"
#include "TGame.h"
void    TSceneEnd::Execute()
{
	//m_pSceneFSM->
	g_bGameRun = false;
}
TSceneEnd::TSceneEnd(TGame* pGame) : TScene(pGame)
{
	m_iStateIndex = GAME_END;
}