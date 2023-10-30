#pragma once
#include "TCharacter.h"
class TNonPersonCharacter : public TCharacter
{
public:
	void		SetHealth(float p);
	void		SetMana(float p);
	void		SetAttackPower(float p);
	void		SetDefensivePower(float p);
	virtual~TNonPersonCharacter();
};

