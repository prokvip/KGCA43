#pragma once
#include "TDxObject.h"
#include "TCollision.h"
// ������ ����� ���� ������Ʈ
class TActor : public TDxObject
{
public:
	bool				m_bDead= false;
	RECT				m_rt;
	T_Math::FVector2	m_vPos;
	T_Math::FMatrix     m_matWorld;
public:
	float				m_fSpeed = 300.0f;  // �ӷ�	
	T_Math::FVector2	m_vDirection = { 1.0f, 0.0f };
	T_Math::FVector2	m_vOffset;
	T_Math::FVector2	m_vVelocity; // �ӵ�
public:
	virtual void    Frame() override;
	virtual void	SetVertexData(RECT rt) override;
	virtual TActor& Move(float dx, float dy);
	virtual TActor& Move(T_Math::FVector2 d);
	TActor&			Trans(T_Math::FMatrix& m);

	virtual void	SetPos(T_Math::FVector2& p);
	virtual void	SetPos(float& dx, float& dy);
	virtual void	SetWorld(T_Math::FMatrix& m);
protected:
	TActor&			Transform(T_Math::FMatrix& m);
};


