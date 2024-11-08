#include "TCamera.h"
#include "TInput.h"
void TCamera::Init()
{
	m_Frustum.m_Box.Create(L"", L"../../data/shader/DrawColor.txt");
}
void TCamera::Frame()
{
	if (TInput::Get().KeyCheck(VK_SPACE))	m_fSpeed += g_fSecondPerFrame * 10.0f;
	else						m_fSpeed -= g_fSecondPerFrame * 10.0f;
	// 최소값으로 고정
	if (m_fSpeed < 1.0f) m_fSpeed = 1.0f;

	if (TInput::Get().KeyCheck('W'))
	{
		MoveLook(g_fSecondPerFrame * 50.0f*m_fSpeed);
	}
	if (TInput::Get().KeyCheck('S'))
	{
		MoveLook(-g_fSecondPerFrame * 50.0f * m_fSpeed);
	}
	if (TInput::Get().KeyCheck('A'))
	{
		MoveSide(-g_fSecondPerFrame * 50.0f * m_fSpeed);
	}
	if (TInput::Get().KeyCheck('D'))
	{
		MoveSide(g_fSecondPerFrame * 50.0f * m_fSpeed);
	}

	if (TInput::Get().KeyCheck('Q'))
	{
		MoveUp(g_fSecondPerFrame * 50.0f * m_fSpeed);
	}
	if (TInput::Get().KeyCheck('E'))
	{
		MoveUp(-g_fSecondPerFrame * 50.0f * m_fSpeed);
	}
	Update(	TInput::Get().GetDeltaY(),
			TInput::Get().GetDeltaX(),
			0.0f, 0);
}
void TCamera::Update(float pitch, float yaw, float roll, float d)
{
	m_fYaw += yaw;
	m_fPitch += pitch;
	m_fRoll += roll;

	T::TMatrix		matWorld;
	m_vCamera += m_vLook * d;

	// 오일러각을 사원수 생성
	T::D3DXQuaternionRotationYawPitchRoll(&m_qRoation,m_fYaw, m_fPitch, m_fRoll);
	// 사원수(회전행렬) + 이동행렬 + 스케일행렬 -> 월드행렬
	T::D3DXMatrixAffineTransformation(&matWorld, 1.0f, nullptr, &m_qRoation, &m_vCamera);
	// 역행렬
	T::D3DXMatrixInverse(&m_matView, nullptr, &matWorld);
	UpdateVector();
}
void TCamera::MoveLook(float d)
{
	m_vCamera += m_vLook * d;
}
void TCamera::MoveUp(float d)
{
	m_vCamera += m_vUp * d;
}
void TCamera::MoveSide(float d)
{
	m_vCamera += m_vSide * d;
}
//void TCamera::Update(float pitch, float yaw, float roll, float d)
//{
//	m_fYaw += yaw;
//	m_fPitch += pitch;
//	m_fRoll += roll;
//
//	FMatrix matY, matX;
//	matY.rotateY(m_fYaw);
//	matX.rotateX(m_fPitch);
//	FMatrix matRotate = matX * matY;
//	float x = m_matView._41;
//	float y = m_matView._42;
//	float z = m_matView._43;
//	m_matView = matRotate.Transpose();
//	m_matView._41 = x;
//	m_matView._42 = y;
//	m_matView._43 = z;
//}

void TCamera::SetView(T::TVector3 eye, T::TVector3 target, T::TVector3 up)
{
	/*T::TVector3* vEye = (T::TVector3*)&eye;
	T::TVector3* vAt = (T::TVector3*)&target;
	T::TVector3* vUp = (T::TVector3*)&up;
	//m_matView = FMatrix::CreateViewTransform(eye, target, up);*/
	m_vCamera = eye;
	T::D3DXMatrixLookAtLH(&m_matView, &eye, &target, &up);

	// object tm (x,y,z axis)
	// x axis
	// y axis
	// z axis
	// camera tm( transpse )
	UpdateVector();
}

void TCamera::UpdateVector()
{
	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;
	m_vUp.x = m_matView._12;
	m_vUp.y = m_matView._22;
	m_vUp.z = m_matView._32;
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	// 행렬을 분해해서 오일러 값을 얻는다.
	// 역행렬
	T::TMatrix   matInvView;
	D3DXMatrixInverse(&matInvView , nullptr, &m_matView);
	TVector3* pZAxis = (TVector3*)&matInvView._31;

	float fLen = sqrtf(pZAxis->z * pZAxis->z + pZAxis->x * pZAxis->x);
	this->m_fPitch = -atan2f(pZAxis->y, fLen);
	this->m_fYaw = atan2f(pZAxis->x, pZAxis->z);
	

	m_Frustum.Set(nullptr, &m_matView, &m_matProj);

}
void TCamera::FrustumRender(T::TMatrix& matView, T::TMatrix& matProj)
{	
	m_Frustum.Draw(matView, matProj);
}
void TCamera::Release()
{
	m_Frustum.m_Box.Release();
}
void  TCamera::SetProj(float fFov, float fAspect, float fNear, float fFar)
{
	T::D3DXMatrixPerspectiveFovLH(&m_matProj, fFov,
		fAspect, fNear, fFar);
}