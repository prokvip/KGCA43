#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include <tchar.h>
class TOdbc
{
public:
	SQLHENV    g_hEnv;
	SQLHDBC    g_hDbc;
	SQLHSTMT   g_hStmt;	
public:
	bool		Connect();
	bool		Init();
	bool		Release();
};

