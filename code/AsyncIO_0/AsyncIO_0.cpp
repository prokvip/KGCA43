#include <windows.h>
#include <iostream>
#include <assert.h>

TCHAR* pFileBuffer = 0;
OVERLAPPED readOV;
OVERLAPPED writeOV;
LARGE_INTEGER FileSize;
// OV
// 중요
OVERLAPPED g_ReadOV;
OVERLAPPED g_WriteOV;
TCHAR* g_pOffsetData = nullptr;
TCHAR* g_pDataBuffer = nullptr;
LARGE_INTEGER g_FileSize;
LARGE_INTEGER g_LargeRead;
LARGE_INTEGER g_LargeWrite;
const DWORD   g_dwMaxReadSize = 8192 * 8192;

DWORD Load(const TCHAR* filename)
{
	HANDLE hReadFile = CreateFile(filename,
		GENERIC_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwRead = 0;
	LARGE_INTEGER FileSize;
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &FileSize);
		pFileBuffer = new TCHAR[FileSize.QuadPart];
		DWORD dwLength = FileSize.QuadPart;
		BOOL ret = ReadFile(hReadFile, pFileBuffer, dwLength,
			&dwRead, NULL);
		if (ret == TRUE)
		{
			INT KKK = 0;
		}
	}
	CloseHandle(hReadFile);
	return dwRead;
}
DWORD Copy(const TCHAR* filename, DWORD dwLength)
{
	HANDLE hWriteFile = CreateFile(filename, GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		NULL);
	DWORD dwWritten = 0;
	if (hWriteFile != NULL)
	{
		BOOL ret = WriteFile(hWriteFile, pFileBuffer,
			dwLength, &dwWritten, NULL);
		if (ret == TRUE)
		{
			std::cout << "출력완료" << std::endl;
		}
	}
	CloseHandle(hWriteFile);
	return dwWritten;
}
DWORD AsyncLoad(const TCHAR* filename)
{	
	ZeroMemory(&readOV, sizeof(OVERLAPPED));
	HANDLE hReadFile = CreateFile(filename,
		GENERIC_READ , 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | 
		FILE_FLAG_OVERLAPPED  // 비동기 입력력 해라.
		| FILE_FLAG_WRITE_THROUGH, // 캐시 사용하지 말고 바로 출력 및 입력해라. 
		//| FILE_FLAG_NO_BUFFERING,// 디스크의 섹터크기와 비례되어야 한다.(2의 승수)
		NULL);

	DWORD dwRead = 0;
	LARGE_INTEGER FileSize;
	bool  bPending =false;
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &FileSize);
		pFileBuffer = new TCHAR[FileSize.LowPart];
		DWORD dwLength = FileSize.LowPart;
		BOOL ret = ReadFile(hReadFile, pFileBuffer, dwLength,&dwRead, &readOV);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				std::cout << "로딩 중" << std::endl;
				bPending = true;
			}
		}
		if (ret == TRUE)
		{
			std::cout << "로딩완료" << std::endl;
		}
		while (bPending)
		{
			// 원하는 작업
			// 비동기 입출력의 결과를 조회
			BOOL bRet = GetOverlappedResult(hReadFile, &readOV, &dwRead, FALSE);
			if (bRet == TRUE)
			{
				std::cout << "로딩완료" << std::endl;
				bPending = false;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << "로딩 중" << std::endl;					
				}
				else
				{
					std::cout << "에러!" << std::endl;
					break;
				}
			}
		}
	}
	CloseHandle(hReadFile);
	return dwRead;
}
DWORD AsyncCopy(const TCHAR* filename, DWORD dwLength)
{
	bool bPending = false;
	ZeroMemory(&writeOV, sizeof(OVERLAPPED));
	HANDLE hWriteFile = CreateFile(filename, GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	DWORD dwWritten = 0;
	if (hWriteFile != NULL)
	{
		BOOL ret = WriteFile(hWriteFile, pFileBuffer,dwLength, &dwWritten, &writeOV);
		if (ret == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				std::cout << "출력 중" << std::endl;
				bPending = true;
			}
		}
		if (ret == TRUE)
		{
			std::cout << "출력완료" << std::endl;
		}
		while (bPending)
		{
			// 원하는 작업
			// 비동기 입출력의 결과를 조회
			BOOL bRet = GetOverlappedResult(hWriteFile, &writeOV, &dwWritten, FALSE);
			if (bRet == TRUE)
			{
				std::cout << "출력완료" << std::endl;
				bPending = false;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << "출력 중" << std::endl;
				}
				else
				{
					std::cout << "에러!" << std::endl;
					break;
				}
			}
		}
	}
	CloseHandle(hWriteFile);
	return dwWritten;
}

DWORD FileRead(HANDLE hHandle, DWORD offset)
{
	g_LargeRead.QuadPart += offset;
	readOV.Offset = g_LargeRead.LowPart;
	readOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(hHandle,
		g_pOffsetData, g_dwMaxReadSize,
		&dwReadByte, &readOV);

	if (bResult == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//std::cout << "읽기 중!" << std::endl;
			//bPending = TRUE;
		}
	};
	if (bResult == TRUE)
	{
		std::cout << " 로딩완료" << std::endl;
	}
	return 1;
}
DWORD FileWrite(HANDLE hHandle, DWORD offset)
{
	g_LargeWrite.QuadPart += offset;
	writeOV.Offset = g_LargeWrite.LowPart;
	writeOV.OffsetHigh = g_LargeWrite.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeWrite.QuadPart];

	DWORD dwOffset = 0;
	if ((g_FileSize.QuadPart - g_LargeWrite.QuadPart) < g_dwMaxReadSize)
	{
		dwOffset = g_dwMaxReadSize - (g_FileSize.LowPart - g_LargeWrite.LowPart);
	}

	DWORD dwWriteByte = 0;
	BOOL bResult = WriteFile(hHandle,
		g_pOffsetData, g_dwMaxReadSize - dwOffset,
		&dwWriteByte, &writeOV);
	return bResult;
}
DWORD AsyncLoadOverlapped(const TCHAR* filename)
{
	DWORD dwTrans = 0;
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);


	ZeroMemory(&readOV, sizeof(OVERLAPPED));

	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hFile, &g_FileSize);
		g_pDataBuffer = new TCHAR[g_FileSize.LowPart];
		DWORD dwCnt = g_FileSize.LowPart / g_dwMaxReadSize;
		FileRead(hFile, 0);

		BOOL bPending = TRUE;
		BOOL bResult = FALSE;

		while (bPending)
		{
			bResult = WaitForSingleObject(hFile, 0);
			if (bResult == WAIT_OBJECT_0)
			{
				bResult = GetOverlappedResult(hFile, &readOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeRead.QuadPart + dwTrans < g_FileSize.LowPart)
					{
						std::cout << ".";
						FileRead(hFile, dwTrans);
					}
					else
					{
						std::cout << " Loading Finish!" << std::endl;
						bPending = FALSE;
					}
				}
			}
			else if (bResult != WAIT_TIMEOUT)
			{
				assert(0);
			}
			//std::cout << "_";
		}
	}
	CloseHandle(hFile);
	return g_FileSize.LowPart;
}
DWORD AsyncWriteOverlapped(const TCHAR* filename)
{
	g_LargeRead.QuadPart = 0;
	DWORD dwTrans = 0;
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	ZeroMemory(&writeOV, sizeof(OVERLAPPED));

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FileWrite(hFile, 0);

		BOOL bPending = TRUE;
		BOOL bResult = FALSE;

		while (bPending)
		{
			bResult = WaitForSingleObject(hFile, 0);
			if (bResult == WAIT_OBJECT_0)
			{
				bResult = GetOverlappedResult(hFile, &writeOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeWrite.QuadPart + dwTrans < g_FileSize.LowPart)
					{
						std::cout << ".";
						FileWrite(hFile, dwTrans);
					}
					else
					{
						std::cout << "Write Finish!" << std::endl;
						bPending = FALSE;
					}
				}
			}
			else if (bResult != WAIT_TIMEOUT)
			{
				assert(0);
			}
			//std::cout << ".";
		}
	}
	CloseHandle(hFile);
	return g_FileSize.LowPart;
}
int main()
{
	// 동기 파일 입출력
	//DWORD dwFileSize = Load(L"mssql.ISO");
	//Copy(L"mssqlCopy.ISO", dwFileSize);
	// 비동기 파일 입출력
	//DWORD dwFileSize = AsyncLoad(L"3dsmax.zip");
	//AsyncCopy(L"Copy3dsmax.zip", dwFileSize);
	// 대용량 파일을   쪼개서 입출력
	DWORD dwFileSize = AsyncLoadOverlapped(L"data.7z");
	AsyncWriteOverlapped(L"Copydata.7z");

	/*pFileBuffer = g_pDataBuffer;
	AsyncCopy(L"Copydata.7z", dwFileSize);*/
    std::cout << "Hello World!\n";
}
