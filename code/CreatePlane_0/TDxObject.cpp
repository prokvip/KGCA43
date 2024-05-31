#include "TDxObject.h"
T_Math::FVector2& TDxObject::Move(float dx, float dy)
{
	v[0].X = v[0].X + dx;
	v[0].Y = v[0].Y + dy;
	v[1].X = v[1].X + dx;
	v[1].Y = v[1].Y + dy;
	v[2].X = v[2].X + dx;
	v[2].Y = v[2].Y + dy;
}