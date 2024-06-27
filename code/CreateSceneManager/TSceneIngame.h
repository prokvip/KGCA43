#pragma once
#include "TScene.h"
class TSceneIngame :   public TScene
{
public:
	UINT		m_iLevel = 1;
	using T_Effect = std::shared_ptr<TSprite>;
	std::vector<T_Effect>  m_EffectList;
	
	float				m_fEventTimer = 0.0f;
	TSound*				m_pEffectSound = nullptr;
	TCamera				m_Cam;
	TActor				m_bkScreen; // »ï°¢Çü
	std::vector<TActor> m_UISceneTimer;
	std::vector<TActor> m_UIList;
	std::vector<TNpc>	m_npcList;
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

