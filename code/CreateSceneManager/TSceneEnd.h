#pragma once
#include "TScene.h"
class TSceneEnd :  public TScene
{
public:
	void    Execute() override;
	TSceneEnd(TGame* pGame);
};

