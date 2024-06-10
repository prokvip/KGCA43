#pragma once
#include "TCore.h"
#include "TActor.h"

class Sample : public TCore
{
	TActor				objScreen; // »ï°¢Çü
	std::vector<TActor> m_UIList;
	std::vector<TNpc>	m_npcList;
	THero hero;	
public:
	void   Init() override;
	void   Frame() override;
	void   Render() override;
	void   Release() override;
};