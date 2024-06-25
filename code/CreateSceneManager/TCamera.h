#pragma once
#include "TStd.h"
class TCamera
{
public:
	T_Math::FMatrix     m_matCamera;
	T_Math::FVector2    m_vCameraPos;
	void Frame();
	void Move(T_Math::FVector2 vOffset);
	T_Math::FMatrix& GetMatrix();
	void  SetTransform(T_Math::FVector2   pos);
	void Up();
	void Down();
	void Right(float fValue);
	void Left(float fValue);
};

