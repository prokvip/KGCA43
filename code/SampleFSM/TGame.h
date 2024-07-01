#pragma once
#include "TScene.h"

class TGame :  public TBaseCore
{
public:
	std::shared_ptr<TFiniteStateMachine> m_pSceneFSM = nullptr;
	TScene* m_pCurrentScene = nullptr;
	std::map<int, std::shared_ptr<TScene>> m_SceneList;
	void   AddScene(std::shared_ptr<TScene> scene, int StateID)
	{
		scene->m_iStateIndex = StateID;
		scene->Init();
		m_SceneList.insert(std::make_pair(StateID, scene));
	}
	TScene* operator [] (int id)
	{
		auto scene = m_SceneList.find(id);
		if (scene != m_SceneList.end())
		{
			return scene->second.get();
		}
		return nullptr;
	}
	//std::shared_ptr<TScene>		m_pTitleScene = nullptr;
	//std::shared_ptr<TScene>		m_pLobbyScene = nullptr;
	//std::shared_ptr<TScene>		m_pIngameScene = nullptr;
	//std::shared_ptr<TScene>		m_pResultScene = nullptr;
	//std::shared_ptr<TScene>		m_pStartScene = nullptr;
	//std::shared_ptr<TScene>		m_pEndScene = nullptr;
	//TScene* m_SceneList[6];
	TScene* Transition(TScene* pScene, int iEvent);
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};

