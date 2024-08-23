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
	class FMatrix3x3;

	struct float2
	{
		union {
			struct {
				float  X;
				float  Y;
			};
			float m[2];
		};
	};
	struct float3
	{
		union {
			struct {
				float  X;
				float  Y;
				float  Z;
			};
			float m[3];
		};
	};
	class FVector2 : public float2
	{
	public:
		static FVector2 ZeroVector2;
	public:
		FVector2 operator -(void);
		FVector2 operator *(const FMatrix3x3& v);

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
		float    Dot(const FVector2& v)
		{
			return X * v.X + Y * v.Y;
		}
		// a = b | c;
		float operator | (const FVector2& v) const
		{
			return X * v.X + Y * v.Y;
		}
		// À°½ÊºÐ¹ý(degree)
		float Angle(FVector2& v)
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
	class FVector3 : public float3
	{
	public:
		static FVector3 ZeroVector3;
	public:
		FVector3 operator -(void);
		FVector3 operator *(const FMatrix3x3& v);

		FVector3 operator +(const FVector3& v) const;
		FVector3 operator -(const FVector3& v) const;
		FVector3 operator *(const float s) const;
		FVector3 operator /(const float s) const;
		FVector3 operator +=(const FVector3& v);
		float    Length();
		FVector3 Normalize();
		bool     IsZero();
		bool     IsZero(float& fLentgh);
		// float fDot = a.Dot(b);
		float    Dot(const FVector2& v)
		{
			return X * v.X + Y * v.Y;
		}
		// a = b | c;
		float operator | (const FVector3& v) const
		{
			return X * v.X + Y * v.Y + Z * v.Z;
		}
		FVector3 operator ^ (const FVector3& v) const
		{
			float x = (Y * v.Z - Z * v.Y);
			float y = (Z * v.X - X * v.Z);
			float z = (X * v.Y - Y * v.X);
			return FVector3(x, y, z);
		}
		// À°½ÊºÐ¹ý(degree)
		float Angle(FVector3& v)
		{
			FVector3 a = Normalize();
			FVector3 b = v.Normalize();
			float fdot = a | b; //a.Dot(b); // cos
			float acosValue = (float)acos(fdot); // radian
			float angle = (float)RadianToDegree(acosValue);
			return angle;
		}
	public:
		FVector3()
		{
			X = Y = Z = 0.0f;
		}
		FVector3(float x, float y, float z)
		{
			X = x;
			Y = y;
			Z = z;
		}
		FVector3(const FVector3& v)
		{
			X = v.X;
			Y = v.Y;
			Z = v.Z;
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

	struct float3x3
	{
		union {
			struct {
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
			float f[9];
		};
	};
	struct float4x4
	{
		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float f[16];
		};
	};
	class FMatrix3x3 : public float3x3
	{
	public:
		FMatrix3x3()
		{
			Identity();
		}
		void Identity()
		{
			_11 = _12 = _13 = 0.0f;
			_21 = _22 = _23 = 0.0f;
			_31 = _32 = _33 = 0.0f;

			_11 = _22 = _33 = 1.0f;
		}
		FMatrix3x3 operator *(const FMatrix3x3& matrix)
		{
			// [3 * 2] * [ 3 * 2] =   
			// m[0][0], m[0][1]  1.0f
			// m[1][0], m[1][1]  1.0f
			// m[2][0], m[2][1]  1.0f
			FMatrix3x3 ret;
			for (int iColumn = 0; iColumn < 2; iColumn++)
			{
				for (int iRow = 0; iRow < 3; iRow++)
				{
					// m[0][0], m[0][1], m[0][2] 
					//            dot
					// matrix.m[0][0], matrix.m[1][0], matrix.m[2][0]
					ret.m[iRow][iColumn] =
						m[iRow][0] * matrix.m[0][iColumn] +
						m[iRow][1] * matrix.m[1][iColumn] +
						m[iRow][2] * matrix.m[2][iColumn];
				}
			}
			return ret;
		}

		void Translation(const FVector2& v)
		{
			_31 = v.X;
			_32 = v.Y;
		}
		void Scale(const FVector2& v)
		{
			_11 = v.X;
			_22 = v.Y;
		}
		void rotate(float fRadian)
		{
			float fCosTheta = (float)cos(fRadian);
			float fSinTheta = (float)sin(fRadian);
			_11 = fCosTheta; _12 = fSinTheta;
			_21 = -fSinTheta; _22 = fCosTheta;
		}
	};

	class FMatrix : public float4x4
	{
	public:
		FMatrix()
		{
			Identity();
		}
		void Identity()
		{
			_11 = _12 = _13 = _14 = 0.0f;
			_21 = _22 = _23 = _24 = 0.0f;
			_31 = _32 = _33 = _34 = 0.0f;
			_41 = _42 = _43 = _44 = 0.0f;

			_11 = _22 = _33 = _44 = 1.0f;
		}
		FMatrix operator *(const FMatrix& matrix)
		{
			FMatrix ret;
			for (int iColumn = 0; iColumn < 4; iColumn++)
			{
				for (int iRow = 0; iRow < 4; iRow++)
				{
					ret.m[iRow][iColumn] =
						m[iRow][0] * matrix.m[0][iColumn] +
						m[iRow][1] * matrix.m[1][iColumn] +
						m[iRow][2] * matrix.m[2][iColumn] +
						m[iRow][3] * matrix.m[3][iColumn];
				}
			}
			return ret;
		}

		void Translation(const FVector3& v)
		{
			_41 = v.X;
			_42 = v.Y;
			_43 = v.Z;
		}
		void Translation(float x, float y, float z)
		{
			_41 = x;
			_42 = y;
			_43 = z;
		}
		void Scale(const FVector3& v)
		{
			_11 = v.X;
			_22 = v.Y;
			_33 = v.Z;
		}
		void rotateX(float fRadian)
		{
			float fCosTheta = (float)cos(fRadian);
			float fSinTheta = (float)sin(fRadian);
			_22 = fCosTheta; _23 = fSinTheta;
			_32 = -fSinTheta; _33 = fCosTheta;
		}
		void rotateY(float fRadian)
		{
			float fCosTheta = (float)cos(fRadian);
			float fSinTheta = (float)sin(fRadian);
			_11 = fCosTheta; _13 = -fSinTheta;
			_31 = fSinTheta; _33 = fCosTheta;
		}
		void rotateZ(float fRadian)
		{
			float fCosTheta = (float)cos(fRadian);
			float fSinTheta = (float)sin(fRadian);
			_11 = fCosTheta; _12 = fSinTheta;
			_21 = -fSinTheta; _22 = fCosTheta;
		}

		FMatrix Transpose()
		{
			FMatrix ret;
			ret._11 = _11; ret._12 = _21; ret._13 = _31; ret._14 = _41;
			ret._21 = _12; ret._22 = _22; ret._23 = _32; ret._24 = _42;
			ret._31 = _13; ret._32 = _23; ret._33 = _33; ret._34 = _43;
			ret._41 = _14; ret._42 = _24; ret._43 = _34; ret._44 = _44;
			return ret;
		}
		static FMatrix CreateViewTransform(FVector3 p, FVector3 t, FVector3 up)
		{
			FMatrix ret;
			FVector3 vLook = (t-p).Normalize();
			FVector3 vRight = (up ^ vLook).Normalize();
			FVector3 vUp = (vLook ^ vRight).Normalize();
			ret._11 = vRight.X; ret._12 = vUp.X; ret._13 = vLook.X;
			ret._21 = vRight.Y; ret._22 = vUp.Y; ret._23 = vLook.Y;
			ret._31 = vRight.Z; ret._32 = vUp.Z; ret._33 = vLook.Z;

			ret._41 = -(p | vRight);
			ret._42 = -(p | vUp);
			ret._43 = -(p | vLook);
			return ret;
		}
		static FMatrix CreateProjTransform(float fNear, float fFar, float fFov,
			float fAspect)
		{
			FMatrix ret;
			float h, w, q;
			h = 1.0f / tan(fFov * 0.5f);
			w = h / fAspect;
			q = fFar / (fFar - fNear);

			ret._44 = 0.0f;
			ret._34 = 1.0f;

			ret._11 = w;
			ret._22 = h;
			ret._33 = q;
			ret._43 = -q * fNear;
			return ret;
		}
	};
}

