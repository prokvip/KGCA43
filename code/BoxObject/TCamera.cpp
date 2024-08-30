#include "TCamera.h"
void TCamera::Update(float pitch, float yaw, float roll, float d)
{
	m_fYaw += yaw;
	m_fPitch += pitch;
	m_fRoll += roll;

	FMatrix matY, matX;
	matY.rotateY(m_fYaw);
	matX.rotateX(m_fPitch);
	FMatrix matRotate = matX * matY;
	float x = m_matView._41;
	float y = m_matView._42;
	float z = m_matView._43;
	m_matView = matRotate.Transpose();
	m_matView._41 = x;
	m_matView._42 = y;
	m_matView._43 = z;
}

void TCamera::SetView(FVector3 eye, FVector3 target, FVector3 up)
{
	m_matView = FMatrix::CreateViewTransform(eye, target, up);
}