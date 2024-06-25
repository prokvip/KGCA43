#pragma once
#include "TCore.h"
#include "TSceneTitle.h"
#include "TSceneLobby.h"
#include "TSceneIngame.h"
#include "TSceneResult.h"

class Sample : public TCore
{
	TScene*		m_pCurrentScene = nullptr;
	UINT		m_iLevel = 1;
	using T_Effect = std::shared_ptr<TSprite>;
	std::vector<T_Effect>  m_EffectList;

	std::shared_ptr<TSound>				m_pBGSound = nullptr;
	TSound*				m_pEffectSound = nullptr;
	TCamera				m_Cam;
	TActor				objScreen; // »ï°¢Çü
	std::vector<TActor> m_UIList;
	std::vector<TNpc>	m_npcList;
	THero hero;	
	UINT  m_iNpcCounter=0;
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
};