#pragma once
#include "TScene.h"
class TSceneTitle : public TScene
{
	TActor				objScreen; // »ï°¢Çü
	bool				m_bFadeOut = false;
	std::vector<TActor> m_UIList;
public:
	virtual void    Init() override;
	virtual void    Frame() override;
	virtual void    Render() override;
	virtual void    Release() override;
};

