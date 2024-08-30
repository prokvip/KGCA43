#pragma once
#include "TStd.h"
class TCamera2D
{
public:
	T_Math::FMatrix3x3     m_matCamera;
	T_Math::FVector2    m_vCameraPos;
	void Frame();
	void Move(T_Math::FVector2 vOffset);
	T_Math::FMatrix3x3& GetMatrix();
	void  SetTransform(T_Math::FVector2   pos);
	void Up();
	void Down();
	void Right(float fValue);
	void Left(float fValue);
};

