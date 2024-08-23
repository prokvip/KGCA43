#pragma once
#include "TDxObject.h"
#include "TCollision.h"
#include "TSprite.h"
// ���� ��ġ ������ ������Ʈ 
// ������ ����� ���� ������Ʈ

enum TSelectState
{
	T_DEFAULT = 0,  // Ŀ���� ���� ���� ��(T_FOCUS���¿��� �ٸ� ���� T_ACTIVE�ϸ� ��ȯ�ȴ�.)
	T_HOVER = 1,	// Ŀ���� ���� ���� ��
	T_FOCUS = 2,	// T_ACTIVE���¿��� ���� ��ư�� �ٸ� ������ ������ ��(���)
	T_ACTIVE = 3,	// ���콺 ���� ��ư ������ ���� ��
	T_SELECTED = 4, // T_ACTIVE ���¿��� ���ʹ�ư ������ ��
};

class TActor 
{
public:
	TDxObject*			m_dxObj=nullptr;
public:
	int					m_iSelectID = 0;
	int					m_iCollisionID = 0;
	DWORD				m_dwSelectState = 0;
	DWORD				m_dwCollisionState = 0;
	TSprite*			m_pSprite = nullptr;
public:
	TSpriteData		    m_SpriteData;
	void			ResetSpriteData();
	void			SetAnim(float fAnimationTimer, TSprite* pSprite);	
	// ���԰���
	// 1) ������ ���� ������Ʈ
	// 2) �̵�ó�� ���� ������Ʈ
	// 3) �浹ó�� ���� ������Ʈ
	virtual void   HitOverlap(TActor* pActor, DWORD dwState);
	virtual void   HitSelect(TActor* pActor, DWORD dwState);
	virtual void   SetCollision();
	virtual void   SetSelect();
public:
	bool				m_bDead= false;
	RECT				m_rt;
	T_Math::FVector2	m_vPos;
	T_Math::FMatrix3x3     m_matWorld; // m_matCenter*m_matScale*m_matRotate*m_matTrans;
	T_Math::FMatrix3x3     m_matCenter;
	T_Math::FMatrix3x3     m_matTrans;
	T_Math::FMatrix3x3     m_matScale;
	T_Math::FMatrix3x3     m_matRotate;

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
	virtual void    Frame();
	virtual void    Release();
	virtual void	SetVertexData(RECT rt);
	virtual TActor& Move(float dx, float dy);
	virtual TActor& Move(T_Math::FVector2 d);
	virtual void	SetPos(T_Math::FVector2& p);
	virtual void	SetPos(float& dx, float& dy);
	virtual void	SetWorldMatrix(T_Math::FMatrix3x3& m);

	virtual void	SetViewTransform(T_Math::FMatrix3x3& matCamera);
	virtual TActor&	SetWorldTransform();
	virtual TActor&	SetWorldTransform(T_Math::FMatrix3x3& m);

	virtual void    UpdateSprite(float fReverse=1.0f);
	virtual void    Render(ID3D11DeviceContext* pContext) ;
};


