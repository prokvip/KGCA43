#pragma once
#include "TSelectModel.h"

class TStdSelect : public TSelectModel
{
public:
	FD_SET	m_rSet;
	FD_SET	m_wSet;
	FD_SET	m_eSet;
public:
	virtual bool     Run() override;
public:
	TStdSelect() = default;
	TStdSelect(TNetwork* net);
	virtual ~TStdSelect();
};

