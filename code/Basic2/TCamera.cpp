#include "TCamera.h"
void TCamera::Move(T_Math::FVector2 vOffset)
{
	m_vCameraPos.X = m_vCameraPos.X + -vOffset.X;
	m_vCameraPos.Y = m_vCameraPos.Y + -vOffset.Y;
}
void TCamera::Frame()
{	
	m_matCamera.Translation(m_vCameraPos);
}
T_Math::FMatrix3x3& TCamera::GetMatrix()
{
	return m_matCamera;
}
void  TCamera::SetTransform(T_Math::FVector2   pos)
{
	m_matCamera.Translation(pos);
}
void TCamera::Up()
{
	T_Math::FVector2 vCamDirection = { 0.0f, 1.0f };
	m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
}
void TCamera::Down()
{
	T_Math::FVector2 vCamDirection = { 0.0f, -1.0f };
	m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
}
void TCamera::Right(float fValue)
{
	m_vCameraPos.X = m_vCameraPos.X + fValue;
}
void TCamera::Left(float fValue)
{
	m_vCameraPos.X = m_vCameraPos.X - fValue;
}
