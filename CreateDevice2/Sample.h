#pragma once
#include "TDevice.h"


struct TVertex
{
	float x;
	float y;
	TVertex() { x = 0; y = 0; }
	TVertex(float _x, float _y) { x = _x; y = _y; }
};

class Sample : public TDevice
{
	std::vector<TVertex>  m_VertexList;
	std::vector<DWORD>    m_IndexList;
public:
	bool    Init()		override;
	bool    Render()	override;
	bool    Release()	override;
};