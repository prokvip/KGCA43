#pragma once
#include "TStd.h"
//using namespace T_Math;
class TCamera
{
public:
	T::TMatrix		m_matView;
	T::TQuaternion	m_qRoation;
	T::TVector3     m_vCamera;
	T::TVector3     m_vLook; // z
	T::TVector3     m_vUp;   // y
	T::TVector3     m_vSide; // x
	float			m_fSpeed = 1.0f;
	float		m_fYaw = 0.0f;
	float		m_fPitch = 0.0f;
	float		m_fRoll = 0.0f;

public:
	void MoveLook(float d);
	void MoveUp(float d);
	void MoveSide(float d);
	void UpdateVector();
	void Frame();
	void Update(float x, float y, float z, float d);
	void SetView(T::TVector3 eye, T::TVector3 target, T::TVector3 up);
};

