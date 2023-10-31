#pragma once
#include "TStd.h"
class TPoint
{
public:
	float    m_x;
	float    m_y;
};
struct TStat
{
	float       m_fHealth = 10; // HP
	float		m_fMana = 10;   // MP
	float		m_fAttackPower = 10;
	float		m_fDefensivePower = 10;
	friend std::ostream& operator << (std::ostream& os,
		const TStat& t)
	{
		os << " HP=" << t.m_fHealth
			<< " MP=" << t.m_fMana
			<< " AP=" << t.m_fAttackPower
			<< " DP=" << t.m_fDefensivePower
			<< std::endl;
		return os;
	}
};
class TCharacter
{
private:
	TPoint		 m_Position;
	TStat		 m_Stat;
public:
	std::wstring m_csName;
public:
	virtual void		SetPos(TPoint p);
	TPoint		GetPos();
	virtual void		SetHealth(float p);
	float		GetHealth();
	virtual void		SetMana(float p);
	float		GetMana();
	virtual void		SetAttackPower(float p);
	float		GetAttackPower();
	virtual void		SetDefensivePower(float p);
	float		GetDefensivePower();
	friend std::ostream& operator << (std::ostream& os,
		const TCharacter& t);
	friend std::ostream& operator << (std::ostream& os,
		const TCharacter* t);

	bool operator == (const TCharacter& p)
	{
		return m_csName == p.m_csName;
	}
public:
	TCharacter();
	TCharacter(TPoint	p);
	TCharacter(const TCharacter& c);
	virtual ~TCharacter();
};

