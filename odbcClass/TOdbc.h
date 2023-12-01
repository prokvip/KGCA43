#pragma once
#include "TStd.h"

using RECORD = std::vector<std::wstring>;

struct TColDescription
{
	SQLUSMALLINT        icol;
	SQLWCHAR			szColName[80];
	SQLSMALLINT         cchColNameMax;
	SQLSMALLINT			pcchColName;
	SQLSMALLINT			pfSqlType;
	SQLULEN				pcbColDef;
	SQLSMALLINT			pibScale;
	SQLSMALLINT			pfNullable;

	SQLWCHAR			bindData[80];
	SQLINTEGER			bindValue;
	SQLLEN			    byteSize;
};
class TOdbc
{
public:
	SQLHENV    g_hEnv;
	SQLHDBC    g_hDbc;
	SQLHSTMT   g_hStmt;	
	
	// prepare

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hInsertStmt;
	TCHAR	   m_szInsertName[20] = { 0, };
	SQLINTEGER m_iInsertPrice;
	SQLINTEGER m_iKorean;
	SQLINTEGER m_iDataLength;
	SQLLEN	   m_cbColumn;

	std::vector<RECORD>				g_dbDataList;
	std::vector<TColDescription>	g_descList;
public:
	bool		Connect();
	bool		Init();
	bool		Release();
	void		ErrorMsg();
	void SelectAllRecord();
	void InsertRecord();
	void UpdateRecord();
	void DeleteRecord();
};

