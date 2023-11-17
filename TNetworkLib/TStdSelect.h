#pragma once
#include "TSelectModel.h"

class TStdSelect : public TSelectModel
{
    FD_SET readSet;
    FD_SET sendSet;
    FD_SET execptSet;
    timeval timeout = { 1, 0 };
public:
    virtual bool Init() override;
    virtual bool Release() override;
	virtual bool Run() override;
public:
	virtual ~TStdSelect();
};
