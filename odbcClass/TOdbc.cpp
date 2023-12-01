#include "TOdbc.h"
void TOdbc::ErrorMsg()
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError = 0;

	SQLRETURN hr;
	// 복합에러
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
		//단순에러
	SQLError(g_hEnv, g_hDbc, g_hStmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH + 1, &msgLen);
	{
		_stprintf(errorMsg, L"SQLSTATE:%s, 진단정보:%s, 에러코드:%d",
			sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"진단정보", 0);
	}
}
void TOdbc::SelectAllRecord()
{

}
void TOdbc::InsertRecord()
{

}
void TOdbc::UpdateRecord()
{

}
void TOdbc::DeleteRecord()
{

}
bool TOdbc::Connect()
{
	// 해당드라이버를 사용하여 해당 데이터베이스에 접속.
	TCHAR inConnect[255] = { 0, };
	TCHAR dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);
	_stprintf(inConnect,
		L"DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\cigarette.accdb;", dir);

	TCHAR outConnect[255] = { 0, };
	SQLSMALLINT   cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL,
		inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		return false;
	}
	hr = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc,
		&g_hStmt);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}

	//prepare
	TCHAR sql3[256] = L"insert into tblCigar (name, price, korean) values(?,?,?)";
	hr = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hInsertStmt);
	hr = SQLPrepare(g_hInsertStmt, (SQLTCHAR*)sql3, SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	hr = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg();
		return false;
	}
	
	hr = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_INTEGER,
		SQL_INTEGER, 0, 0, &m_iInsertPrice,
		0, &m_cbColumn);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	hr = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER,
		SQL_INTEGER, 0, 0, &m_iKorean,
		0, &m_cbColumn);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}


	
	return true;
}
bool TOdbc::Init()
{
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE,
				&g_hEnv);
	if (hr == SQL_SUCCESS)
	{
		hr = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, 
						(SQLPOINTER)SQL_OV_ODBC3_80, SQL_IS_INTEGER);
		if (hr != SQL_SUCCESS)
		{
			return false;
		}

		hr = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv,
			&g_hDbc);
		if (hr == SQL_SUCCESS)
		{
			return true;
		}		
	}
	return false;
}
bool TOdbc::Release()
{
	if (g_hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	}
	if (g_hDbc)
	{
		SQLDisconnect(g_hDbc);
	}
	if(g_hDbc)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	}
	if (g_hEnv)
	{
		SQLFreeEnv(g_hEnv);
		SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
	}
	return true;
}