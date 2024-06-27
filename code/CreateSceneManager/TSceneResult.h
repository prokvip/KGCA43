#pragma once
#include "TScene.h"
class TSceneResult : public TScene
{
public:
	float				m_fEventTimer = 0.0f;
	TActor				m_bkScreen; 
	TActor				m_StartBtn;

public:
	void    Init();
	void    Frame();
	void    Render();
	void    Release();
public:
	void    Execute() override;
	TSceneResult(TGame* pGame);
};

