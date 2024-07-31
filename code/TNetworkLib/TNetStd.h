#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <thread>
#include "TProtocol.h"
#pragma comment(lib,"ws2_32.lib")

struct tOV : public OVERLAPPED
{
	enum { t_READ, t_SEND, };
	int iIOFlag = t_READ;
};
