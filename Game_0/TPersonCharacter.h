#pragma once
#include "TCharacter.h"
class TPersonCharacter : public TCharacter
{
public:
	float		m_fExperience;// EXP(°æÇèÄ¡)
public:
	void		SetExperience(float p);
	float		GetExperience();

	void		SetHealth(float p);
	void		SetMana(float p);
	void		SetAttackPower(float p);
	void		SetDefensivePower(float p);
	virtual ~TPersonCharacter();
};

