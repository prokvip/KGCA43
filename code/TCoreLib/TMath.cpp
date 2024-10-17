//#include "TMath.h"
//namespace T_Math
//{
//	FVector2 FVector2::ZeroVector2(0.0f, 0.0f);
//	FVector2 FVector2::operator -(void)
//	{
//		FVector2 ret = *this;
//		ret.x *= -1.0f;
//		ret.y *= -1.0f;
//		return ret;
//	}
//	FVector2 FVector2::operator *(const FMatrix3x3& matrix)
//	{
//		FVector2 ret;
//		ret.x =.x * matrix.m[0][0] +
//			.y * matrix.m[1][0] +
//				1.0f * matrix.m[2][0];
//		ret.y =.x * matrix.m[0][1] +
//			.y * matrix.m[1][1] +
//				1.0f * matrix.m[2][1];
//		return ret;
//	}
//	bool     FVector2::IsZero(float& fLentgh)
//	{
//		fLentgh = Length();
//		if (fLentgh <= 0.00001f)
//		{
//			return true;
//		}
//		return false;
//	}
//	bool     FVector2::IsZero()
//	{
//		float fLength = Length();
//		if (fLength <= 0.00001f)
//		{
//			return true;
//		}
//		return false;
//	}
//	FVector2 FVector2::operator +=(const FVector2& v)
//	{
//	.x += v.x;
//	.y += v.y;
//		return *this;
//	}
//	FVector2 FVector2::operator +(const FVector2& v) const
//	{
//		return FVector2.x + v.x,.y + v.y);
//	};
//	FVector2 FVector2::operator -(const FVector2& v) const
//	{
//		return FVector2.x - v.x,.y - v.y);
//	};
//	FVector2 FVector2::operator *(const float s) const
//	{
//		return FVector2.x*s,.y*s);
//	};
//	FVector2 FVector2::operator /(const float s) const
//	{
//		return FVector2.x / s,.y / s);
//	};
//
//	float    FVector2::Length()
//	{
//		return sqrt.x *.x +.y *.y);
//	}
//
//	FVector2    FVector2::Normalize()
//	{
//		float fLength = 0.0f;
//		if (IsZero(fLength)) return ZeroVector2;
//		return FVector2.x / fLength,.y/ fLength);
//	}
//
//
//	///
//	FVector3 FVector3::ZeroVector3(0.0f, 0.0f, 0.0f);
//	FVector3 FVector3::operator -(void)
//	{
//		FVector3 ret = *this;
//		ret.x *= -1.0f;
//		ret.y *= -1.0f;
//		ret.z *= -1.0f;
//		return ret;
//	}
//	FVector3 FVector3::operator *(const FMatrix3x3& matrix)
//	{
//		FVector3 ret;
//		ret.x =.x * matrix.m[0][0] + 
//			   .y * matrix.m[1][0] + 
//			.z * matrix.m[2][0] + 
//				1.0f * matrix.m[3][0];
//		ret.y =.x * matrix.m[0][1] + 
//			.y * matrix.m[1][1] + 
//			.z * matrix.m[2][1] +
//				1.0f * matrix.m[3][1];
//		ret.z =.x * matrix.m[0][2] +
//			.y * matrix.m[1][2] +
//			.z * matrix.m[2][2] +
//				1.0f * matrix.m[3][2];
//		return ret;
//	}
//	bool     FVector3::IsZero(float& fLentgh)
//	{
//		fLentgh = Length();
//		if (fLentgh <= 0.00001f)
//		{
//			return true;
//		}
//		return false;
//	}
//	bool     FVector3::IsZero()
//	{
//		float fLength = Length();
//		if (fLength <= 0.00001f)
//		{
//			return true;
//		}
//		return false;
//	}
//	FVector3 FVector3::operator +=(const FVector3& v)
//	{
//	.x += v.x;
//	.y += v.y;
//	.z += v.z;
//		return *this;
//	}
//	FVector3 FVector3::operator +(const FVector3& v) const
//	{
//		return FVector3.x + v.x,.y + v.y,.z + v.z);
//	};
//	FVector3 FVector3::operator -(const FVector3& v) const
//	{
//		return FVector3.x - v.x,.y - v.y,.z - v.z);
//	};
//	FVector3 FVector3::operator *(const float s) const
//	{
//		return FVector3.x * s,.y * s,.z * s);
//	};
//	FVector3 FVector3::operator /(const float s) const
//	{
//		return FVector3.x / s,.y / s,.z / s);
//	};
//
//	float    FVector3::Length()
//	{
//		return sqrt.x *.x +.y *.y +.z *.z);
//	}
//
//	FVector3    FVector3::Normalize()
//	{
//		float fLength = 0.0f;
//		if (IsZero(fLength)) return ZeroVector3;
//		return FVector3.x / fLength,.y / fLength,.z / fLength);
//	}
//};