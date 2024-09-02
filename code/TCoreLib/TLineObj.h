#pragma once
#include "TShapeObj.h"

class TLineObj : public TShapeObj
{
public:
	virtual void		SetVertexData() override;
	virtual void		SetIndexData() override;
	void Draw(FVector3 s, FVector3 e, FVector4 c);
};