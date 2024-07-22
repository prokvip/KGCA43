#pragma once
#include "TSelectModel.h"
class TAsyncSelect : public TSelectModel
{
public:
	virtual bool     Run() override;
public:
	TAsyncSelect() = default;
	TAsyncSelect(TNetwork * net);
	virtual ~TAsyncSelect();
};

