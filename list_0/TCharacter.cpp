#include "TCharacter.h"
std::ostream& operator << (std::ostream& os,
	const TCharacter& t)
{
	os << "PosX=" <<t.m_Position.m_x
			<< " PosY=" <<t.m_Position.m_y;
	os << t.m_Stat;
	return os;
}
std::ostream& operator << (std::ostream& os,
	const TCharacter* t)
{
	os << "PosX=" << t->m_Position.m_x
		<< " PosY=" << t->m_Position.m_y;
	os << t->m_Stat;
	return os;
}

void		TCharacter::SetPos(TPoint p)
{
	this->m_Position = p;
}
TPoint		TCharacter::GetPos() {
	return this->m_Position;
}
void		TCharacter::SetHealth(float p) {
	this->m_Stat.m_fHealth = p;
}
float		TCharacter::GetHealth() {
	return m_Stat.m_fHealth;
}
void		TCharacter::SetMana(float p) {
	this->m_Stat.m_fMana = p;
}
float		TCharacter::GetMana() {
	return m_Stat.m_fMana;
}
void		TCharacter::SetAttackPower(float p) {
	this->m_Stat.m_fAttackPower = p;
}
float		TCharacter::GetAttackPower() {
	return m_Stat.m_fAttackPower;
}
void		TCharacter::SetDefensivePower(float p) {
	this->m_Stat.m_fDefensivePower = p;
}
float		TCharacter::GetDefensivePower() {
	return m_Stat.m_fDefensivePower;
}
TCharacter::TCharacter() {
	m_Position.m_x = 0;
	m_Position.m_y = 0;
}
TCharacter::~TCharacter() {
	std::cout << "부모 해제자호출" << std::endl;
}