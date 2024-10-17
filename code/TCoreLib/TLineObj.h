#pragma once
#include "TShapeObj.h"

class TLineObj : public TShapeObj
{
public:
	virtual void		SetVertexData() override;
	virtual void		SetIndexData() override;
	void Draw(T::TVector3 s, T::TVector3 e, T::TVector4 c);
};