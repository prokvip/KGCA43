#pragma once
#include <math.h>
namespace T_Math
{
	class FVector2
	{
	public:
		float  X;
		float  Y;
		static FVector2 ZeroVector2;
	public:
		FVector2 operator +(const FVector2& v) const;
		FVector2 operator -(const FVector2& v) const;
		FVector2 operator *(const float s) const;
		FVector2 operator /(const float s) const;
		float    Length();
		FVector2 Normalize();
		bool     IsZero();
		bool     IsZero(float& fLentgh);
	public:
		FVector2()
		{
			X = Y = 0.0f;
		}
		FVector2(float x, float y)
		{
			X = x;
			Y = y;
		}
		FVector2(const FVector2& v)
		{
			X = v.X;
			Y = v.Y;
		}
	};
}

