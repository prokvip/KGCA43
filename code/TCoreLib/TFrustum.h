#pragma once
#include "TBoxObj.h"

class TFrustum 
{
public:
	T::TVector3		m_vFrustum[8];
	T_Plane			m_Plane[6];
	T::TMatrix		m_matWorld;
	T::TMatrix		m_matView;
	T::TMatrix		m_matProj;
	TBoxObj			m_Box;
public:
	void			Set(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj);
	void			Draw(T::TMatrix& matView, T::TMatrix& matProj);
public:
	bool			FrustumInPoint(T::TVector3 v);
	bool			FrustumInBox(T_Box b);
	bool			FrustumInSphere(T_Sphere s);
};

