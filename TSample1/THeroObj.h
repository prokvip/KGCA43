#pragma once
#include "TPlaneShape.h"
struct TInitSet
{
	W_STR name;
	TVector2 p; 
	TVector2 uvPos;
	TVector2 size;
};
class THeroObj : public TPlaneShape
{
	TInitSet	m_InitSet;
public:
	bool   Create(TInitSet info, W_STR texFileName);
	bool   CreateVertexBuffer() override;
	bool   SetVB(TVector2 p,
		TVector2 uvPos,
		TVector2 size,
		TVector2 texSize);
};

