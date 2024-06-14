#include "THero.h"
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
THero::THero()
{
	m_fSpeed = 100.0f;  // �ӷ�	
}