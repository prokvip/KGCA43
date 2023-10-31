#include "TPersonCharacter.h"
TPersonCharacter::TPersonCharacter()
{
	std::cout << "생성자호출" << std::endl;
}
TPersonCharacter::~TPersonCharacter()
{
	std::cout << "해제자호출" << std::endl;
}
void		TPersonCharacter::SetExperience(float p)
{
	m_fExperience = p;
}
float		TPersonCharacter::GetExperience()
{
	return m_fExperience;
}
void		TPersonCharacter::SetHealth(float p)
{
	TCharacter::SetHealth(p + 50.0f);
}
void		TPersonCharacter::SetMana(float p)
{
	TCharacter::SetMana(p + 50.0f);
}
void		TPersonCharacter::SetAttackPower(float p)
{
	TCharacter::SetAttackPower(p + 50.0f);
}
void		TPersonCharacter::SetDefensivePower(float p)
{
	TCharacter::SetDefensivePower(p + 50.0f);
}