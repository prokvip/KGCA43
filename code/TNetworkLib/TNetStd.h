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
#include <atlconv.h> // A2W
#include "TProtocol.h"
#include "TObjectPool.h"
#pragma comment(lib,"ws2_32.lib")

struct tOV : TObjectPool<tOV>
{
	enum { t_READ, t_SEND, t_END };
	OVERLAPPED ov;
	int iIOFlag = t_READ;
};
static std::wstring mw(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

static std::string wm(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};