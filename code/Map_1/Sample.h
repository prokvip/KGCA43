#pragma once
#include "TCore.h"

class Sample : public TCore
{	
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};