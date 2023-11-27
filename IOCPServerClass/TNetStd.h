#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <process.h>//_beginthreadex
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <condition_variable>
#include <shared_mutex>
#include "TProtocol.h"
#include "TLog.h"

static std::string wtm(std::wstring data)
{
	char retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data.c_str(), -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data.c_str(), -1,  //  소스
		retData, iLength, // 대상
		NULL, NULL);	
	return retData;
}
static std::wstring mtw(std::string data)
{
	WCHAR retData[4096] = { 0 };
	// 변형되는 문자열의 크기가 반환된다.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data.c_str(), -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data.c_str(), -1,  //  소스
		retData, iLength); // 대상
	return retData;
}

static void ConsolePrintW(const wchar_t* fmt, ...)
{
	va_list ap;
	wchar_t buf[MAX_PATH];

	va_start(ap, fmt);
	vswprintf_s(buf, fmt, ap);
	va_end(ap);

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWriten;
	WriteConsoleW(handle, buf, wcslen(buf), &dwBytesWriten, 0);
}
static void ConsolePrintA(const char* fmt, ...)
{
	va_list ap;
	char buf[MAX_PATH];

	va_start(ap, fmt);
	vsprintf_s(buf, fmt, ap);
	va_end(ap);

	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwBytesWriten;
	WriteConsoleA(handle, buf, strlen(buf), &dwBytesWriten, 0);
}
static void LogPrintW(const wchar_t* fmt, ...)
{
	va_list ap;
	wchar_t buf[MAX_PATH];

	va_start(ap, fmt);
	vswprintf_s(buf, fmt, ap);
	va_end(ap);

	TLog::Get().log(wtm(buf));
}
static void LogPrintA(const char* fmt, ...)
{
	va_list ap;
	char buf[MAX_PATH];

	va_start(ap, fmt);
	vsprintf_s(buf, fmt, ap);
	va_end(ap);

	TLog::Get().log(buf);
}
static void LogErrorA(const char* fmt, ...)
{
	void* lpMsgBuf;
	DWORD dwError = WSAGetLastError();
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	LocalFree(lpMsgBuf);

	va_list ap;
	char buf[MAX_PATH];

	va_start(ap, fmt);
	vsprintf_s(buf, fmt, ap);
	va_end(ap);

	std::string msg = buf;
	msg += "\n";
	msg += "ERRCode[";
	msg += std::to_string(dwError);
	msg += "]:";
	msg += (char*)lpMsgBuf;
	TLog::Get().log(msg);
}
#define PrintA(fmt,...)         ConsolePrintA( fmt, __VA_ARGS__ )
#define PrintDetailA(fmt,...)   ConsolePrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PrintW(fmt,...)         ConsolePrintW( fmt, __VA_ARGS__ )
#define PrintDetailW(fmt,...)   ConsolePrintW( L"[%s %d] : " fmt, __FILEW__,__LINE__, ##__VA_ARGS__ )
#define LogA(fmt,...)			LogPrintA( fmt, __VA_ARGS__ )
#define LogDetailA(fmt,...)		LogPrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define LogErrorA(fmt,...)		LogErrorA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )

static void E_MSG(const char* pMsg)
{
	void* lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);

	LogA((char*)lpMsgBuf);

	MessageBoxA(NULL, (char*)lpMsgBuf, pMsg, MB_OK);
	LocalFree(lpMsgBuf);
	return;
}


template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return  theSingleInstance;
	}
};