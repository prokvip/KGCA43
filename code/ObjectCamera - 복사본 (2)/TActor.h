#pragma once
#include "TDxObject.h"
#include "TCollision.h"
// 움직임 기능이 없는 오브젝트
class TActor : public TDxObject
{
public:
	bool				m_bDead= false;
	RECT				m_rt;
	T_Math::FVector2	m_vPos;
	T_Math::FMatrix     m_matWorld; // m_matCenter*m_matScale*m_matRotate*m_matTrans;
	T_Math::FMatrix     m_matCenter;
	T_Math::FMatrix     m_matTrans;
	T_Math::FMatrix     m_matScale;
	T_Math::FMatrix     m_matRotate;

	void    SetTrans(T_Math::FVector2& p);
	void    SetScale(T_Math::FVector2& v);
	void    SetRotate(float r);
	void    SetCenterMove(T_Math::FVector2& p);
public:
	float				m_fSpeed = 300.0f;  // 속력	
	T_Math::FVector2	m_vDirection = { 1.0f, 0.0f };
	T_Math::FVector2	m_vOffset;
	T_Math::FVector2	m_vVelocity; // 속도
public:
	virtual void    Frame() override;
	virtual void	SetVertexData(RECT rt) override;
	virtual TActor& Move(float dx, float dy);
	virtual TActor& Move(T_Math::FVector2 d);
	virtual void	SetPos(T_Math::FVector2& p);
	virtual void	SetPos(float& dx, float& dy);
	virtual void	SetWorld(T_Math::FMatrix& m);
	virtual void	SetTransform(T_Math::FMatrix& matCamera);
protected:
	TActor&			Transform();
	TActor&			Transform(T_Math::FMatrix& m);	
};


