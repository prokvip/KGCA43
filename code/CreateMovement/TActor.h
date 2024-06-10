#pragma once
#include "TDxObject.h"
// 움직임 기능이 없는 오브젝트
class TActor : public TDxObject
{
};
// 움직임 기능이 있는 오브젝트
class TPawn : public TActor
{
};
class THero : public TPawn
{
public:
	float				m_fSpeed = 100.0f;  // 속력	
	T_Math::FVector2	m_vOffset;
	T_Math::FVector2	m_vVelocity; // 속도
public:
	virtual void     Frame() override
	{
		// 화면좌표계
		for (auto& v : m_vList)
		{
			v.p += m_vOffset;
		}
		// NDC 좌표계 변환
		for (int i = 0; i < m_vList.size(); i++)
		{
			m_vListNDC[i].p = ConvertScreenToNDC(m_vList[i].p);
		}
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);

		m_vOffset = { 0.0f,0.0f };
	}

	virtual void  Move(T_Math::FVector2 d)
	{
		T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;;
		m_vPos = m_vPos + vOffset;
		m_vOffset += vOffset;
	}
	virtual void  Front()
	{
		// 시간의 동기화
		// 프레임의 변화를 무시하고 시간당 일정한 거리로 움직인다.)
		float fSpeed = m_fSpeed * g_fSecondPerFrame;
		// 벡터의 직선의 방정식
		// q(계산된 위치) = p(현재위치) + d(이동방향:정규화)*s(크기);
		T_Math::FVector2 p = m_vPos;
		T_Math::FVector2 d = { 0.0f, -1.0f };
		T_Math::FVector2 vOffset = d * fSpeed;
		//T_Math::FVector2 q = p + vOffset;		
		m_vPos = p + vOffset;
		m_vOffset += vOffset;
	};
	virtual void  Back()
	{
		T_Math::FVector2 d = { 0.0f, 1.0f };
		T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
		m_vPos = m_vPos + vOffset;
		m_vOffset += vOffset;
	};
	virtual void  Left()
	{
		T_Math::FVector2 d = { -1.0f, 0.0f };
		T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
		m_vPos = m_vPos + vOffset;
		m_vOffset += vOffset;
	};
	virtual void  Right()
	{
		T_Math::FVector2 d = { 1.0f, 0.0f };
		T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
		m_vPos = m_vPos + vOffset;
		m_vOffset += vOffset;
	}
};

class TNpc : public TPawn
{
public:
	float				m_fSpeed = 300.0f;  // 속력	
	T_Math::FVector2	m_vDirection = { 1.0f, 0.0f };
	T_Math::FVector2	m_vOffset;
	T_Math::FVector2	m_vVelocity; // 속도
	virtual void  Move(T_Math::FVector2 d)
	{
		T_Math::FVector2 vOffset = d * m_fSpeed * g_fSecondPerFrame;
		m_vPos = m_vPos + vOffset;
		m_vOffset += vOffset;
	}
	virtual void     Frame() override
	{
		if (m_vPos.X > g_xClientSize)
		{
			m_vDirection.X *= -1.0f;
			m_vPos.X = g_xClientSize;
		}
		if (m_vPos.X < 0)
		{
			m_vDirection.X *= -1.0f;
			m_vPos.X = 0;
		}
		if (m_vPos.Y > g_yClientSize)
		{
			m_vDirection.Y *= -1.0f;
			m_vPos.Y = g_yClientSize;
		}
		if (m_vPos.Y < 0)
		{
			m_vDirection.Y *= -1.0f;
			m_vPos.Y = 0;
		}

		Move(m_vDirection);


		// 화면좌표계
		for (auto& v : m_vList)
		{
			v.p += m_vOffset;
		}
		// NDC 좌표계 변환
		for (int i = 0; i < m_vList.size(); i++)
		{
			m_vListNDC[i].p = ConvertScreenToNDC(m_vList[i].p);
		}
		m_pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vListNDC.at(0), 0, 0);

		m_vOffset = { 0.0f,0.0f };
	}

	TNpc()
	{
		m_fSpeed = randstep(100.0f, 300.0f);
		m_vDirection.X = 1.0f;// randstep(-1.0f, 1.0f);
		m_vDirection.Y = 1.0f;// randstep(-1.0f, 1.0f);
	}
};

