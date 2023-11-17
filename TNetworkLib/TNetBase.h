#pragma once
#include "TNetworkStd.h"
#include "TProtocol.h"
class TNetBase
{
public:
	CRITICAL_SECTION m_cs;
	virtual ~TNetBase();
};