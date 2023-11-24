#pragma once
#include "TNetStd.h"
#define MAX_WORKER_THREAD 2
class TIocpModel
{
public:
	HANDLE	        g_hKillEvent;
	HANDLE          g_hWorkerThread[MAX_WORKER_THREAD];
	HANDLE          g_hIOCP;
public:
	bool			Init();
};