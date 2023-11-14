#pragma once
#include <winsock2.h>
#include <windows.h>
#include <iostream>
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

#define PrintA(fmt,...)         ConsolePrintA( fmt, __VA_ARGS__ )
#define PrintDetailA(fmt,...)   ConsolePrintA( "[%s %s %d] : " fmt, __FILE__,__FUNCTION__, __LINE__, ##__VA_ARGS__ )
#define PrintW(fmt,...)         ConsolePrintW( fmt, __VA_ARGS__ )
#define PrintDetailW(fmt,...)   ConsolePrintW( L"[%s %d] : " fmt, __FILEW__,__LINE__, ##__VA_ARGS__ )

