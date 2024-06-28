#pragma once
#include "TScene.h"
class TUI : public TActor
{
public:
	void   HitSelect(TActor* pActor, DWORD dwState)
	{
		m_dwSelectState = dwState;
	}
};
class TSceneIngame :   public TScene
{
public:
	UINT				m_iLevel = 1;	
	float				m_fEventTimer = 0.0f;
	TSound*				m_pEffectSound = nullptr;
	TCamera				m_Cam;
	TActor				m_bkScreen; // »ï°¢Çü
	
	using T_Effect		= std::shared_ptr<TSprite>;
	using T_Actor		= std::shared_ptr<TActor>;
	using T_NpcCount	= std::shared_ptr<TActor>;
	using T_UI			= std::shared_ptr<TUI>;
	using T_Npc			= std::shared_ptr<TNpc>;

	std::vector<T_Effect>			m_EffectList;
	std::vector<TUI>				m_UISceneTimer;
	std::vector<T_NpcCount>			m_pNpcCounterObj;
	std::map<std::wstring, T_UI>    m_UIList;
	std::map<UINT, T_Npc>			m_npcList;

	THero m_Hero;
	UINT  m_iNpcCounter = 0;
	bool  m_bMissionComplite = false;
	bool  m_bWaveComplite = false;
	void    LevelUp(UINT iNpc);
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
public:
	void  SetSound();
	void  SetUI();
	void  SetPlayer();
	void  Reset();
	void  Execute() override;
public:
	TSceneIngame(TGame* pGame);
};

