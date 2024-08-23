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
	FVector2 FVector2::operator *(const FMatrix3x3& matrix)
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


	///
	FVector3 FVector3::ZeroVector3(0.0f, 0.0f, 0.0f);
	FVector3 FVector3::operator -(void)
	{
		FVector3 ret = *this;
		ret.X *= -1.0f;
		ret.Y *= -1.0f;
		ret.Z *= -1.0f;
		return ret;
	}
	FVector3 FVector3::operator *(const FMatrix3x3& matrix)
	{
		FVector3 ret;
		ret.X = X * matrix.m[0][0] + 
			    Y * matrix.m[1][0] + 
				Z * matrix.m[2][0] + 
				1.0f * matrix.m[3][0];
		ret.Y = X * matrix.m[0][1] + 
				Y * matrix.m[1][1] + 
				Z * matrix.m[2][1] +
				1.0f * matrix.m[3][1];
		ret.Z = X * matrix.m[0][2] +
				Y * matrix.m[1][2] +
				Z * matrix.m[2][2] +
				1.0f * matrix.m[3][2];
		return ret;
	}
	bool     FVector3::IsZero(float& fLentgh)
	{
		fLentgh = Length();
		if (fLentgh <= 0.00001f)
		{
			return true;
		}
		return false;
	}
	bool     FVector3::IsZero()
	{
		float fLength = Length();
		if (fLength <= 0.00001f)
		{
			return true;
		}
		return false;
	}
	FVector3 FVector3::operator +=(const FVector3& v)
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	}
	FVector3 FVector3::operator +(const FVector3& v) const
	{
		return FVector3(X + v.X, Y + v.Y, Z + v.Z);
	};
	FVector3 FVector3::operator -(const FVector3& v) const
	{
		return FVector3(X - v.X, Y - v.Y, Z - v.Z);
	};
	FVector3 FVector3::operator *(const float s) const
	{
		return FVector3(X * s, Y * s, Z * s);
	};
	FVector3 FVector3::operator /(const float s) const
	{
		return FVector3(X / s, Y / s, Z / s);
	};

	float    FVector3::Length()
	{
		return sqrt(X * X + Y * Y + Z * Z);
	}

	FVector3    FVector3::Normalize()
	{
		float fLength = 0.0f;
		if (IsZero(fLength)) return ZeroVector3;
		return FVector3(X / fLength, Y / fLength, Z / fLength);
	}
};