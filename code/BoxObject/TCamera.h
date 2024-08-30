#pragma once
#include "TStd.h"
using namespace T_Math;
class TCamera
{
public:
	FMatrix		m_matView;
	float		m_fYaw = 0.0f;
	float		m_fPitch = 0.0f;
	float		m_fRoll = 0.0f;

public:
	void Update(float x, float y, float z, float d);
	void SetView(FVector3 eye, FVector3 target, FVector3 up);
};

