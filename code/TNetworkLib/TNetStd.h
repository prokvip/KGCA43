#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <thread>
#include <memory>
#include "TProtocol.h"
#include "TObjectPool.h"
#pragma comment(lib,"ws2_32.lib")

struct tOV : TObjectPool<tOV>
{
	enum { t_READ, t_SEND, };
	OVERLAPPED ov;
	int iIOFlag = t_READ;
};
