#pragma once
#include "TNetStd.h"

class TIocpModel
{
public:
	HANDLE	        g_hKillEvent;
	HANDLE          g_hWorkerThread[MAX_WORKER_THREAD];
	HANDLE          g_hIOCP;
};