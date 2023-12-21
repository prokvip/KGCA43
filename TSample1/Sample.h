#pragma once
#include "TGameCore.h"
#include "THeroObj.h"
class Sample : public TGameCore
{
	std::shared_ptr<THeroObj>			m_Hero;
public:
	bool	   Init() override;
	bool	   Frame() override;
	bool	   Render() override;
	bool	   Release() override;
};

