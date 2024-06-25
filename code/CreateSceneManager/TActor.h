#pragma once
#include "TDxObject.h"
#include "TCollision.h"
#include "TSprite.h"
// ���� ��ġ ������ ������Ʈ 
// ������ ����� ���� ������Ʈ
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
	// ���԰���
	// 1) ������ ���� ������Ʈ
	// 2) �̵�ó�� ���� ������Ʈ
	// 3) �浹ó�� ���� ������Ʈ
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
	float				m_fSpeed = 300.0f;  // �ӷ�	
	T_Math::FVector2	m_vDirection = { 1.0f, 0.0f };
	T_Math::FVector2	m_vOffset;
	T_Math::FVector2	m_vVelocity; // �ӵ�
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


