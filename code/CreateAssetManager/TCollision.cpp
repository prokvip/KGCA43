#include "TCollision.h"
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