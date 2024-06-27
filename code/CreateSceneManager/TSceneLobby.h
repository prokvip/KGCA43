#pragma once
#include "TScene.h"
class TSceneLobby :   public TScene
{
public:
	float				m_fEventTimer = 0.0f;
	TActor				objScreen; 
	TActor				m_StartBtn;
public:
	void    Init();
	void    Frame();
	void    Render();
	void    Release();
public:
	void    Execute() override;
	TSceneLobby(TGame* pGame);
};

