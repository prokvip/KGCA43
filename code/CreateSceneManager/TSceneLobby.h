#pragma once
#include "TScene.h"
class TSceneLobby :   public TScene
{
public:
	void    Execute() override;
	TSceneLobby(TGameObj* pGame) : TScene(pGame)
	{
	}
};

