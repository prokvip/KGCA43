#include "THero.h"
#include "TInput.h"
void   THero::HitOverlap(TActor* pActor, DWORD dwState)
{
	pActor->m_bDead = true;	
}
void   THero::SetVertexData(RECT rt)
{
	TDxObject2D* obj = (TDxObject2D*)m_dxObj;
	m_dxObj->SetVertexData(rt);

	// 0 [91] 400(w), 0 [1] ~ 300(h)
	POINT  imageSize = { 400, 300 };
	RECT texRT = { 91, 2, 91 + 40, 1 + 59 };
	// 91,1             131, 1

	// 91,61            131,61
	// 91,61            131,61
	obj->m_vListScreen[0].t.X = (float)texRT.left / (float)imageSize.x;
	obj->m_vListScreen[0].t.Y = (float)texRT.top / (float)imageSize.y;

	obj->m_vListScreen[1].t = { (float)texRT.right / (float)imageSize.x, (float)texRT.top / (float)imageSize.y };
	obj->m_vListScreen[2].t = { (float)texRT.right / (float)imageSize.x, (float)texRT.bottom / (float)imageSize.y };


	obj->m_vListScreen[3].t = obj->m_vListScreen[2].t;
	obj->m_vListScreen[4].t = { (float)texRT.left / (float)imageSize.x, (float)texRT.bottom / (float)imageSize.y };
	obj->m_vListScreen[5].t = obj->m_vListScreen[0].t;

	obj->m_vList = obj->m_vListScreen;
	
	// �簢���� ���͸� ����������	
	m_vPos.X = (rt.left + rt.right) * 0.5f;
	m_vPos.Y = (rt.bottom + rt.top) * 0.5f;

	T_Math::FVector2 vCenter = { -m_vPos.X, -m_vPos.Y };
	// �ʱ� ��ġ�� �������� �� ������ ��ġ�� �������� �Ѵ�.
	SetCenterMove(vCenter);
	SetTrans(m_vPos);
	m_rt = rt;
}
void  THero::Front()
{
	// �ð��� ����ȭ
	// �������� ��ȭ�� �����ϰ� �ð��� ������ �Ÿ��� �����δ�.)
	float fSpeed = m_fSpeed * g_fSecondPerFrame;
	// ������ ������ ������
	// q(���� ��ġ) = p(������ġ) + d(�̵�����:����ȭ)*s(ũ��);
	T_Math::FVector2 p = m_vPos;
	T_Math::FVector2 d = { 0.0f, -1.0f };
	T_Math::FVector2 vOffset = d * fSpeed;
	//T_Math::FVector2 q = p + vOffset;		
	m_vPos = p + vOffset;
	m_vOffset += vOffset;
};
void  THero::Back()
{
	T_Math::FVector2 d = { 0.0f, 1.0f };
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos = m_vPos + vOffset;
	m_vOffset += vOffset;
};
void  THero::Left()
{
	T_Math::FVector2 d = { -1.0f, 0.0f };
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos = m_vPos + vOffset;
	m_vOffset += vOffset;
};
void  THero::Right()
{
	T_Math::FVector2 d = { 1.0f, 0.0f };
	T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
	m_vPos = m_vPos + vOffset;
	m_vOffset += vOffset;
}
void  THero::Frame()
{	
	m_vOffset = { 0.0f,0.0f };
	
	
	if (I_Input.KeyCheck('W') == KEY_HOLD)
	{
		if (-900.0f  < m_vPos.Y)
		{			
			//m_Cam.Up();
			Move({ 0.0f, -1.0f });
			//Front();
		}		
	}
	if (I_Input.KeyCheck('S') == KEY_HOLD)
	{
		if (900.0f > m_vPos.Y)
		{			
			//m_Cam.Down();
			Move({ 0.0f, 1.0f });
			//Back();
		}
	}
	if (I_Input.KeyCheck('D') == KEY_HOLD)
	{
		if (900.0f  > m_vPos.X)
		{			
			Move({ 1.0f, 0.0f });
			//Right();		
			//m_Cam.Right(-m_Hero.m_vOffset.X);
		}
	}
	if (I_Input.KeyCheck('A') == KEY_HOLD)
	{
		if (-900.0f  < m_vPos.X)
		{			
			Move({ -1.0f, 0.0f });
			//m_Hero.Left();		
		}
	}
	TPawn::Frame();
}
THero::THero()
{
	m_fSpeed = 100.0f;  // �ӷ�	
}