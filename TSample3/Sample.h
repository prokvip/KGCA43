#pragma once
#include "TGameCore.h"
class Sample : public TGameCore
{
public:
	bool	   Init() override;
	bool	   Frame() override;
	bool	   Render() override;
	bool	   Release() override;
};

