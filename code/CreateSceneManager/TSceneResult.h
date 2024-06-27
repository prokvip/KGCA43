#pragma once
#include "TScene.h"
class TSceneResult : public TScene
{
public:
	void    Execute() override;
	TSceneResult(TGameObj* pGame) : TScene(pGame)
	{
	}
};

