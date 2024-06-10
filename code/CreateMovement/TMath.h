#pragma once
#include <math.h>

#define randf(x) (x*rand()/(float)RAND_MAX)
#define randf2(x,off) (off+x*rand()/(float)RAND_MAX)
#define randstep(fMin,fMax) (fMin+((float)fMax-(float)fMin)*rand()/(float)RAND_MAX)
#define clamp(x,MinX,MaxX) if (x>MaxX) x=MaxX; else if (x<MinX) x=MinX;
#define TBASIS_EPSILON		((float)  0.001f)
#define TBASIS_PI			((float)  3.141592654f)
#define DegreeToRadian( degree ) ((degree) * (TBASIS_PI / 180.0f))
#define RadianToDegree( radian ) ((radian) * (180.0f / TBASIS_PI))

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
		FVector2 operator +=(const FVector2& v);
		float    Length();
		FVector2 Normalize();
		bool     IsZero();
		bool     IsZero(float& fLentgh);
		// float fDot = a.Dot(b);
		float    Dot(const FVector2 & v)
		{
			return X * v.X + Y * v.Y;
		}
		// a = b | c;
		float operator | (const FVector2& v) const
		{
			return X * v.X + Y * v.Y;
		}
		// À°½ÊºÐ¹ý(degree)
		float Angle( FVector2& v)
		{
			FVector2 a = Normalize();
			FVector2 b = v.Normalize();
			float fdot = a | b; //a.Dot(b); // cos
			float acosValue = acos(fdot); // radian
			float angle = (float)RadianToDegree(acosValue);
			return angle;
		}
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

	class FVector4
	{
	public:
		float  X;
		float  Y;
		float  Z;
		float  W;
		FVector4()
		{
			X = Y = Z = W = 1.0f;
		}
		FVector4(float x, float y, float z, float w)
		{
			X = x;
			Y = y;
			Z = z;
			W = w;
		}
		FVector4(const FVector4& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
			W = v.W;
		}
	};
}

