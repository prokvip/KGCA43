#pragma once
#include "TDxObj3D.h"

class TFrustum : public TDxObject3D
{
public:
	T::TVector3		m_vFrustum[8];
	T_Plane			m_Plane[6];
	void SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj) override;
	bool			FrustumInPoint(T::TVector3 v);
	bool			FrustumInBox(T_Box b);
	bool			FrustumInSphere(T_Sphere s);
};

