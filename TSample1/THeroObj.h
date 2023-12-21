#pragma once
#include "TPlaneShape.h"
struct TInitSet
{
	W_STR name;
	W_STR szMaskTex;
	TVector2 p; 
	TVector2 uvPos;
	TVector2 size;
};
class THeroObj : public TPlaneShape
{
	TInitSet	m_InitSet;
public:
	TVector2    m_vPos;
	TTexture*   m_ptexMask = nullptr;
public:	
	bool   CreateVertexBuffer() override;
	bool   Frame() override;
	bool   PreRender() override;
public:
	bool   Create(TInitSet info, W_STR texFileName);
	bool   SetVB(TVector2 p,TVector2 uvPos,	TVector2 size,TVector2 texSize);	
	void   SetPosion(TVector2 p);
	void   SetPosion(float x, float y);

};

