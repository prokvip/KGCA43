#pragma once
#include "TScene.h"
class TButton : public TActor
{
public:
	void   HitOverlap(TActor* pActor, DWORD dwState)
	{

	}
	void   HitSelect(TActor* pActor, DWORD dwState)
	{
		m_dwSelectState = dwState;
	}
};
class TSceneTitle : public TScene
{
	TActor				m_bkScreen; // »ï°¢Çü
	bool				m_bFadeOut = false;
	TButton				m_StartBtn;
	float				m_fEventTimer = 0.0f;
	std::vector<TActor> m_UIList;
public:
	virtual void    Init() override;
	virtual void    Frame() override;
	virtual void    Render() override;
	virtual void    Release() override;
	void    Execute() override;
public:
	TSceneTitle(TGame* pGame) : TScene(pGame)
	{
		m_iStateIndex = GAME_TITLE;
	}
};

