#pragma once
#include "THero.h"
#include "TNpc.h"
#include "TSound.h"
#include "TSprite.h"
#include "TCamera.h"
class TScene
{
public:
	bool	m_bSceneChange = false;
	std::shared_ptr<TSound>				m_pBGSound = nullptr;
public:
	virtual void    Init() {};
	virtual void    Frame() {};
	virtual void    Render() {};
	virtual void    Release() {};
};

