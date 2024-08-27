#pragma once
#include "TShapeObj.h"

class TPlaneObj : public TShapeObj
{
public:
	virtual void		SetVertexData() override;
	virtual void		SetIndexData() override;
	virtual void		Frame() override;
};
