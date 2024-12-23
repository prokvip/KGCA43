#pragma once
#include "TCollision.h"
struct TRayPick
{
	T::TVector3 vOrigin;
	T::TVector3 vDirection;
};
class TSelect
{
public:
	TRayPick	m_Ray;
	T::TMatrix  m_matWorld;
	T::TMatrix  m_matView;
	T::TMatrix  m_matProj;
	T::TVector3 m_vIntersection;
public:
	void		Init();
	void		Frame();
	void		Render();
	void		Release();
	void		SetMatrix(T::TMatrix* pWorld, T::TMatrix* pView, T::TMatrix* pProj);
public:
	BOOL		ChkBoxToRay(T_Box& box);
	BOOL		ChkBoxToSphere(T_Sphere& sphere);
public:
	bool		GetIntersection(T::TVector3 v0,
								T::TVector3 v1,
								T::TVector3 v2,
								T::TVector3 n,
								T::TVector3 s,
								T::TVector3 e);
	bool        PointInPolygon( T::TVector3 vIntersection, 
								T::TVector3 n,
								T::TVector3 v0,
								T::TVector3 v1,
								T::TVector3 v2);
	bool		IntersectTriangle(const TVector3& orig,
								const TVector3& dir,
								TVector3& v0,
								TVector3& v1,
								TVector3& v2,
								FLOAT* t, FLOAT* u, FLOAT* v);
};

