#pragma once
#include "TShapeObj.h"
class TBoxObj : public TShapeObj
{
public:
	virtual void		SetVertexData() override;
	virtual void		SetIndexData() override;
};

