#pragma once
#include "TScene.h"
class TSceneStart :  public TScene
{
public:
	float				m_fEventTimer = 0.0f;
	TActor				objScreen; // »ï°¢Çü
public:
	void    Init();
	void    Frame();
	void    Render();
	void    Release();
public:
	void    Execute() override;

	TSceneStart(TGame* pGame);
};

