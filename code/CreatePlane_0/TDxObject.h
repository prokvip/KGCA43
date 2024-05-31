#pragma once
#include <vector>
#include "TMath.h"

class TDxObject 
{
public:	
	std::vector<T_Math::FVector2> v;
	T_Math::FVector2& Move(float dx, float dy);
};

