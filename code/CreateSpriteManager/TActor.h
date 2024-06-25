#pragma once
#include "TDxObject.h"
#include "TCollision.h"
#include "TSprite.h"
// 씬에 배치 가능한 오브젝트 
// 움직임 기능이 없는 오브젝트
class TActor : public TDxObject
{
public:
	TSprite*			m_pSprite = nullptr;
public:
	TSpriteData		    m_SpriteData;
	void   SetAnim(float fAnimationTimer, TSprite* pSprite)
	{
		_ASSERT(pSprite);
		m_pSprite = pSprite;
		m_SpriteData = m_pSprite->m_SpriteData;

		m_SpriteData.m_iAnimIndex = 0;
		m_SpriteData.m_fAnimationTimer = fAnimationTimer;
		m_SpriteData.m_fChangeTime = m_SpriteData.m_fAnimationTimer / m_pSprite->m_SpriteData.m_fTexCounter;
	}
	//TDxObject   m_dxObject;
	// 포함관계
	// 1) 렌더링 관리 컴포넌트
	// 2) 이동처리 관리 컴포넌트
	// 3) 충돌처리 관리 컴포넌트
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
	virtual void	SetWorldMatrix(T_Math::FMatrix& m);

	virtual void	SetViewTransform(T_Math::FMatrix& matCamera);
	virtual TActor&	SetWorldTransform();
	virtual TActor&	SetWorldTransform(T_Math::FMatrix& m);

	virtual void    UpdateSprite();
	virtual void    Render(ID3D11DeviceContext* pContext) override;
};


