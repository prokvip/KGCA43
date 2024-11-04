#pragma once
#include "TStd.h"
struct T_AABB
{
	T::TVector3 vMin;
	T::TVector3 vMax;
};
struct T_OBB
{
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	float       fDistance[3];
};
struct T_Box
{
	/*T_AABB     aabb;
	T_OBB      obb;*/
	// AABB
	T::TVector3 vMin;
	T::TVector3 vMax;
	// OBB
	T::TVector3 vCenter;
	T::TVector3 vAxis[3];
	float       fDistance[3];
};
struct T_Sphere
{
	T::TVector3 vCenter;
	float m_fRadius;
};
struct T_Plane
{
	float a, b, c, d;
	void  Set(T::TVector3 v0, T::TVector3 v1, T::TVector3 v2);
	void  Set(T::TVector3 v0, T::TVector3 vNormal);
};
class TCollision
{
public:
	static bool RectToRect(RECT& rt1, RECT& rt2);
	static bool RectToPt(RECT& rt1, POINT pt);
};

