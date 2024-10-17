//#include "TCamera2D.h"
//void TCamera2D::Move(T::TVector2 vOffset)
//{
//	m_vCameraPos.x = m_vCameraPos.x + -vOffset.x;
//	m_vCameraPos.y = m_vCameraPos.y + -vOffset.y;
//}
//void TCamera2D::Frame()
//{	
//	m_matCamera.Translation(m_vCameraPos);
//}
//T_Math::FMatrix3x3& TCamera2D::GetMatrix()
//{
//	return m_matCamera;
//}
//void  TCamera2D::SetTransform(T::TVector2   pos)
//{
//	m_matCamera.Translation(pos);
//}
//void TCamera2D::Up()
//{
//	T::TVector2 vCamDirection = { 0.0f, 1.0f };
//	m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
//}
//void TCamera2D::Down()
//{
//	T::TVector2 vCamDirection = { 0.0f, -1.0f };
//	m_vCameraPos = m_vCameraPos + vCamDirection * 100.0f * g_fSecondPerFrame;
//}
//void TCamera2D::Right(float fValue)
//{
//	m_vCameraPos.x = m_vCameraPos.x + fValue;
//}
//void TCamera2D::Left(float fValue)
//{
//	m_vCameraPos.x = m_vCameraPos.x - fValue;
//}
