#pragma once
#include "THero.h"
#include "TNpc.h"
#include "TSound.h"
#include "TSprite.h"
#include "TCamera.h"
#include "TFiniteStateMachine.h"

class TGameObj;


enum TGameState  // TFiniteState
{
	GAME_START = 0,
	GAME_TITLE,
	GAME_INGAME,
	GAME_END,
};
enum TGameEvent
{
	EVENT_AUTO = 0,
	EVENT_CLICK,
	EVENT_TIMER,
};

class TScene
{
protected:
	static std::shared_ptr<TFiniteStateMachine> m_pSceneFSM;
public:
	TFiniteStateMachine* GetFSM() { return m_pSceneFSM.get(); }
	TGameObj*	m_pGame = nullptr;
	bool		m_bSceneChange = false;
	std::shared_ptr<TSound>				m_pBGSound = nullptr;
	float   m_fSceneTime = 0.0f;
	int		m_iStateIndex = -1;
public:
	static  void    CreateFSM();
	virtual void    Init();
	virtual void    Frame();
	virtual void    Render();
	virtual void    Release();
	virtual void    Execute() = 0;
public:
	TScene(TGameObj* pGame) : m_pGame(pGame) {}
};

class TSceneStart : public TScene
{
public:
	float				m_fEventTimer = 0.0f;
	TActor				objScreen; // »ï°¢Çü
public:
	void   Init()
	{
		RECT rtBk = { 0, 0, 800.0f, 600.0f };
		objScreen.Create(TDevice::m_pd3dDevice.Get(),
			TDevice::m_pContext, rtBk,
			L"../../data/kgca3.png",
			L"intro.txt");		
		objScreen.SetAnim(3.0f, I_Sprite.GetPtr(L"kgca"));
	}
	void    Frame()
	{
		objScreen.Frame();
	}
	void    Render()
	{
		objScreen.UpdateSprite();
		//objScreen.SetViewTransform(m_Cam.GetMatrix());
		objScreen.Render(TDevice::m_pContext);	
	}
	void    Release()
	{
		objScreen.Release();
	}
public:
	void    Execute() override;

	TSceneStart(TGameObj* pGame) : TScene(pGame) {
		m_iStateIndex = GAME_START;
	}
};
class TSceneEnd : public TScene
{
public:
	void    Execute() override;
	TSceneEnd(TGameObj* pGame) : TScene(pGame) {
		m_iStateIndex = GAME_END;
	}
};