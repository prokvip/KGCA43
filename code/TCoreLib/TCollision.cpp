#include "TCollision.h"
void  T_Plane::Set(T::TVector3 v0, T::TVector3 v1, T::TVector3 v2)
{
	T::TVector3 e0 = v1 - v0;
	T::TVector3 e1 = v2 - v0;
	T::TVector3 vNormal;
	D3DXVec3Cross(&vNormal, &e0, &e1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	// -(a*vx + b*vy + c*v.z)  = d;//(-d * -1
	a = vNormal.x;
	b = vNormal.y;
	c = vNormal.z;
	d = -D3DXVec3Dot(&vNormal, &v0);
	//d = -(vNormal.x * v0.x + vNormal.y*v0.y + vNormal.z*v0.z);
}
void  T_Plane::Set(T::TVector3 v0, T::TVector3 vNormal)
{
	a = vNormal.x;
	b = vNormal.y;
	c = vNormal.z;
	d = -D3DXVec3Dot(&vNormal, &v0);
}

// used - >  rt1.minX    rt1.maxX
// no used - >  rt1.x    w
bool TCollision::RectToRect(RECT& rt1, RECT& rt2)
{
	LONG minX, maxX, minY, maxY;
	minX = min(rt1.left, rt2.left);
	maxX = max(rt1.right, rt2.right);
	minY = min(rt1.top, rt2.top);
	maxY = max(rt1.bottom, rt2.bottom);
	LONG sizeX, sizeY;
	sizeX = maxX - minX;
	sizeY = maxY - minY;

	LONG  rt1W, rt1H, rt2W, rt2H;
	rt1W = rt1.right - rt1.left;// rt1.w + rt2.w;
	rt1H = rt1.bottom - rt1.top;
	rt2W = rt2.right - rt2.left;// rt1.w + rt2.w;
	rt2H = rt2.bottom - rt2.top;

	if ( sizeX <= (rt1W + rt2W) &&
		 sizeY <= (rt1H + rt2H))
	{
		return true;
	}
	return false;
}
bool TCollision::RectToPt(RECT& rt1, POINT pt)
{
	if (rt1.left <= pt.x && rt1.right >= pt.x)
	{
		if (rt1.top <= pt.y && rt1.bottom >= pt.y)
		{
			return true;
		}
	}
	return false;
};