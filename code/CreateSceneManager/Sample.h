#pragma once
#include "TCore.h"
#include "TSceneTitle.h"
#include "TSceneLobby.h"
#include "TSceneIngame.h"
#include "TSceneResult.h"

class Sample : public TCore
{
	TScene*		m_pCurrentScene = nullptr;
	std::shared_ptr<TScene>		m_pTitleScene = nullptr;
	std::shared_ptr<TScene>		m_pLobbyScene = nullptr;
	std::shared_ptr<TScene>		m_pIngameScene = nullptr;
	std::shared_ptr<TScene>		m_pResultScene = nullptr;
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};