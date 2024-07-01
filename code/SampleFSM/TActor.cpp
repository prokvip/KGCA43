#include "TActor.h"
#include "TObjectMgr.h"
void   TActor::Release()
{
	I_ObjMgr.DelSelectExecute(this);
	I_ObjMgr.DelCollisionExecute(this);
}
void   TActor::SetCollision()
{
	I_ObjMgr.AddCollisionExecute(this,
		std::bind(&TActor::HitOverlap, this,
			std::placeholders::_1,
			std::placeholders::_2));
}
void   TActor::SetSelect()
{
	I_ObjMgr.AddSelectExecute(this,
		std::bind(&TActor::HitSelect, this,
			std::placeholders::_1,
			std::placeholders::_2));
}
void   TActor::HitOverlap(TActor* pActor, DWORD dwState)
{
	int k = 0;
}
void   TActor::HitSelect(TActor* pActor, DWORD dwState)
{
	m_dwSelectState = dwState;
}
void			TActor::ResetSpriteData()
{
	m_SpriteData.m_iAnimIndex = 0;
	m_SpriteData.m_fPlayTimer = 0.0f;
}
void   TActor::SetAnim(float fAnimationTimer, TSprite* pSprite)
{
	_ASSERT(pSprite);
	m_pSprite = pSprite;
	m_SpriteData = m_pSprite->m_SpriteData;

	m_SpriteData.m_iAnimIndex = 0;
	m_SpriteData.m_fAnimationTimer = fAnimationTimer;
	m_SpriteData.m_fChangeTime = m_SpriteData.m_fAnimationTimer / m_pSprite->m_SpriteData.m_fTexCounter;
}
void    TActor::Render(ID3D11DeviceContext* pContext)
{
	TDxObject::PreRender(pContext);
	if (m_pSprite != nullptr)
	{
		pContext->PSSetShaderResources(0, 1,m_pSprite->GetSRV(m_SpriteData.m_iAnimIndex).GetAddressOf());
	}
	TDxObject::PostRender(pContext);
}
void    TActor::Frame()
{
	m_matWorld = m_matCenter * m_matScale * m_matRotate * m_matTrans;
	SetWorldTransform();
	//m_vOffset = { 0.0f,0.0f };	
}
void    TActor::UpdateSprite(float fReverse)
{
	if (m_pSprite != nullptr)
	{
		m_pSprite->Update(m_SpriteData, fReverse);
		if (m_pSprite->m_vList.size() > 0)
		{
			for (int iV = 0; iV < 6; iV++)
			{
				m_vList[iV].t = m_pSprite->m_vList[iV].t;
			}
		}
	}
}
void    TActor::SetTrans(T_Math::FVector2& p)
{
	m_matTrans.Translation(p);
}
void    TActor::SetScale(T_Math::FVector2& s)
{
	m_matScale.Scale(s);
}
void    TActor::SetRotate(float r)
{
	m_matRotate.rotate(r);
}
void    TActor::SetCenterMove(T_Math::FVector2& p)
{	
	m_matCenter.Translation(p);	
}
void    TActor::SetPos(T_Math::FVector2& p)
{
	m_vPos = p;
	m_matWorld._31 = m_vPos.X;
	m_matWorld._32 = m_vPos.Y;	
}
void    TActor::SetPos(float& dx, float& dy)
{
	m_vPos.X = dx;
	m_vPos.Y = dx;
}
void TActor::SetWorldMatrix(T_Math::FMatrix& m)
{
	m_matWorld = m;
}
void TActor::SetViewTransform(T_Math::FMatrix& matCamera)
{	
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vList[iv].p * matCamera;
	}
}
TActor& TActor::SetWorldTransform()
{
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vListScreen[iv].p * m_matWorld;
	}
	return *this;
}
TActor& TActor::SetWorldTransform(T_Math::FMatrix& m)
{
	for (int iv = 0; iv < m_vListScreen.size(); iv++)
	{
		m_vList[iv].p = m_vListScreen[iv].p * m;
	}
	return *this;
}
void   TActor::SetVertexData(RECT rt)
{
	TDxObject::SetVertexData(rt);

	// 사각형의 센터를 포지션으로	
	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;

	T_Math::FVector2 vCenter = { -m_vPos.X, -m_vPos.Y };
	// 초기 배치의 정점버퍼 각 정점의 위치를 중점으로 한다.
	SetCenterMove(vCenter);
	SetTrans(m_vPos);
	m_rt = rt;
}
TActor& TActor::Move(float dx, float dy)
{
	Move({dx,dy});
	//// 화면좌표계
	//for (auto& v : m_vListScreen)
	//{
	//	v.p += { dx, dy };
	//}
	//m_vPos = { dx,dy };

	//UpdateVertexBuffer();
	return *this;
}
TActor& TActor::Move(T_Math::FVector2 d)
{
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos += vOffset;
	m_vOffset += vOffset;

	SetTrans(m_vPos);

	m_rt.left	= m_vList[0].p.X;
	m_rt.right	= m_vList[1].p.X;
	m_rt.top	= m_vList[0].p.Y;
	m_rt.bottom = m_vList[2].p.Y;
	return *this;
}