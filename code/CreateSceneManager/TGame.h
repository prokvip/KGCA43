#pragma once
#include "TScene.h"

class TGame :  public TBaseCore
{
public:
	std::shared_ptr<TFiniteStateMachine> m_pSceneFSM = nullptr;
	TScene* m_pCurrentScene = nullptr;
	std::shared_ptr<TScene>		m_pTitleScene = nullptr;
	std::shared_ptr<TScene>		m_pLobbyScene = nullptr;
	std::shared_ptr<TScene>		m_pIngameScene = nullptr;
	std::shared_ptr<TScene>		m_pResultScene = nullptr;
	std::shared_ptr<TScene>		m_pStartScene = nullptr;
	std::shared_ptr<TScene>		m_pEndScene = nullptr;
	TScene* m_SceneList[6];
	int		Transition(TScene* pScene, int iEvent);
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};

