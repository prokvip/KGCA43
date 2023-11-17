#pragma once
#include "TSelectModel.h"

class TAsyncSelect : public TSelectModel
{
public:
	virtual bool Run() override;
public:
	virtual ~TAsyncSelect();
};