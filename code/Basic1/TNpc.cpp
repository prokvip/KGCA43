#include "TNpc.h"
void   TNpc::HitOverlap(TActor* pActor, DWORD dwState)
{
	int k = 0;
	//pActor->m_bDead = true;
}
void   TNpc::SetVertexData(RECT rt)
{
	m_dxObj->SetVertexData(rt);

	//// 0 [91] 400(w), 0 [1] ~ 300(h)
	//POINT  imageSize = { 400, 300 };
	////RECT texRT = { 46, 62, 68 + 46, 62 + 79 };
	//RECT texRT = { 116, 63, 114 + 37, 63 + 33 };	
	////RECT texRT = { 0,0,400, 300 };
	//// 91,1             131, 1

	//// 91,61            131,61
	//m_vListScreen[0].t.X = (float)texRT.left / (float)imageSize.x;
	//m_vListScreen[0].t.Y = (float)texRT.top / (float)imageSize.y;

	//m_vListScreen[1].t = { (float)texRT.right / (float)imageSize.x, (float)texRT.top / (float)imageSize.y };
	//m_vListScreen[2].t = { (float)texRT.right / (float)imageSize.x, (float)texRT.bottom / (float)imageSize.y };


	//m_vListScreen[3].t = m_vListScreen[2].t;
	//m_vListScreen[4].t = { (float)texRT.left / (float)imageSize.x, (float)texRT.bottom / (float)imageSize.y };
	//m_vListScreen[5].t = m_vListScreen[0].t;

	//m_vList = m_vListScreen;

	// 사각형의 센터를 포지션으로	
	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;

	T_Math::FVector2 vCenter = { -m_vPos.X, -m_vPos.Y };
	// 초기 배치의 정점버퍼 각 정점의 위치를 중점으로 한다.
	SetCenterMove(vCenter);
	SetTrans(m_vPos);
	m_rt = rt;
}
void    TNpc::Frame() 
{
	if (m_vPos.X > 1000.0f)
	{
		m_vDirection.X = -1.0f;
	}
	if (m_vPos.X < -1000.0f)
	{
		m_vDirection.X = 1.0f;
	}
	if (m_vPos.Y > 1000.0f)
	{
		m_vDirection.Y = -1.0f;
	}
	if (m_vPos.Y < -1000.0f)
	{
		m_vDirection.Y = 1.0f;
	}

	Move(m_vDirection);
	TActor::Frame();
}
TNpc::TNpc()
{
	m_fSpeed = randstep(100.0f, 300.0f);
	m_vDirection.X = randstep(-1.0f, 1.0f);
	m_vDirection.Y = randstep(-1.0f, 1.0f);
}