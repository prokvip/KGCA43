#pragma once
#include "TSelectModel.h"
class TSession;
class TEventSelect :  public TSelectModel
{
public:
	UINT	  m_iCurrentEvent;
	HANDLE    m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	TSession* m_pSessionArray[WSA_MAXIMUM_WAIT_EVENTS];
public:
	void      Accept();
	void      DisConnected(UINT iEvent);
public:
	virtual bool     Run() override;
public:
	TEventSelect() = default;
	TEventSelect(TNetwork * net);
	virtual ~TEventSelect();
};

