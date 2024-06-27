#pragma once
#include "THero.h"
#include "TNpc.h"
#include "TSound.h"
#include "TSprite.h"
#include "TCamera.h"
#include "TFiniteStateMachine.h"

class TGame;


enum TGameState  // TFiniteState
{
	GAME_START = 0,
	GAME_TITLE,
	GAME_LOBBY,
	GAME_INGAME,
	GAME_RESULT,
	GAME_END,
};
enum TGameEvent
{
	EVENT_AUTO = 0,
	EVENT_CLICK,
	EVENT_TIMER,	
	EVENT_GAME_EXIT,
	EVENT_SCENE_EXIT,
	EVENT_MISSION_COMPLETE,
	EVENT_MISSION_FAILED,
	EVENT_RECHALLENGE,
};

class TScene : public TBaseCore
{
public:
	std::shared_ptr<TSound>		m_pBGSound = nullptr;
	TGame*		m_pGame = nullptr;
	bool		m_bSceneChange = false;	
	float		m_fSceneTime = 0.0f;
	int			m_iStateIndex = -1;
public:
	static  void    CreateFSM();
	virtual void    Init();
	virtual void    Frame();
	virtual void    Render();
	virtual void    Release();
	virtual void    Execute() = 0;
public:
	TScene(TGame* pGame) : m_pGame(pGame) {}
};
