#pragma once
#include "TUiObj.h"

class Sample : public TDevice
{
	TUiObj			m_bk;
	TUiObj			m_SkillLayout;
	TUiObj			m_Skill2;

	TUiNumber		m_Number;
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};