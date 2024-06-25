#include "TMath.h"
namespace T_Math
{
	FVector2 FVector2::ZeroVector2(0.0f, 0.0f);
	FVector2 FVector2::operator -(void)
	{
		FVector2 ret = *this;
		ret.X *= -1.0f;
		ret.Y *= -1.0f;
		return ret;
	}
	FVector2 FVector2::operator *(const FMatrix& matrix)
	{
		FVector2 ret;
		ret.X = X * matrix.m[0][0] +
				Y * matrix.m[1][0] +
				1.0f * matrix.m[2][0];
		ret.Y = X * matrix.m[0][1] +
				Y * matrix.m[1][1] +
				1.0f * matrix.m[2][1];
		return ret;
	}
	bool     FVector2::IsZero(float& fLentgh)
	{
		fLentgh = Length();
		if (fLentgh <= 0.00001f)
		{
			return true;
		}
		return false;
	}
	bool     FVector2::IsZero()
	{
		float fLength = Length();
		if (fLength <= 0.00001f)
		{
			return true;
		}
		return false;
	}
	FVector2 FVector2::operator +=(const FVector2& v)
	{
		X += v.X;
		Y += v.Y;
		return *this;
	}
	FVector2 FVector2::operator +(const FVector2& v) const
	{
		return FVector2(X + v.X, Y + v.Y);
	};
	FVector2 FVector2::operator -(const FVector2& v) const
	{
		return FVector2(X - v.X, Y - v.Y);
	};
	FVector2 FVector2::operator *(const float s) const
	{
		return FVector2(X*s, Y*s);
	};
	FVector2 FVector2::operator /(const float s) const
	{
		return FVector2(X / s, Y / s);
	};

	float    FVector2::Length()
	{
		return sqrt(X * X + Y * Y);
	}

	FVector2    FVector2::Normalize()
	{
		float fLength = 0.0f;
		if (IsZero(fLength)) return ZeroVector2;
		return FVector2(X / fLength, Y/ fLength);
	}
};