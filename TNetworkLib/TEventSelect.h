#pragma once
#include "TSelectModel.h"

class TEventSelect : public TSelectModel
{
	int     g_iCurrentEvent = 0;
	std::vector<HANDLE>  g_hEventArray;// (WSA_MAXIMUM_WAIT_EVENTS);
	std::vector<SOCKET>  g_hSocketArray;// (WSA_MAXIMUM_WAIT_EVENTS);
public:
	virtual bool Run() override;
public:
	virtual ~TEventSelect();
};
