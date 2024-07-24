#include <windows.h>
#include <iostream>
#include <assert.h>

//byte* g_pDataBuffer = 0;
//OVERLAPPED g_ReadOV;
//OVERLAPPED g_WriteOV;
//LARGE_INTEGER FileSize;
// OV
// 중요
OVERLAPPED g_ReadOV;
OVERLAPPED g_WriteOV;
byte* g_pOffsetData = nullptr;
byte* g_pDataBuffer = nullptr;
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
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &g_FileSize);
		g_pDataBuffer = new byte[g_FileSize.QuadPart];
		BOOL ret = ReadFile(hReadFile, g_pDataBuffer, g_FileSize.QuadPart,	&dwRead, NULL);
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
		BOOL ret = WriteFile(hWriteFile, g_pDataBuffer,	dwLength, &dwWritten, NULL);
		if (ret == TRUE)
		{
			std::cout << "출력완료" << std::endl;
		}
	}

	CloseHandle(hWriteFile);
	delete [] g_pDataBuffer;
	return dwWritten;
}
DWORD AsyncLoad(const TCHAR* filename)
{	
	ZeroMemory(&g_ReadOV, sizeof(OVERLAPPED));
	HANDLE hReadFile = CreateFile(filename,
		GENERIC_READ , 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | 
		FILE_FLAG_OVERLAPPED  // 비동기 입력력 해라.
		| FILE_FLAG_WRITE_THROUGH, // 캐시 사용하지 말고 바로 출력 및 입력해라. 
		//| FILE_FLAG_NO_BUFFERING,// 디스크의 섹터크기와 비례되어야 한다.(2의 승수)
		NULL);

	DWORD dwRead = 0;
	bool  bPending =false;
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &g_FileSize);
		g_pDataBuffer = new byte[g_FileSize.LowPart];
		DWORD dwLength = g_FileSize.LowPart;
		BOOL ret = ReadFile(hReadFile, g_pDataBuffer, dwLength,&dwRead, &g_ReadOV);
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
			BOOL bRet = GetOverlappedResult(hReadFile, &g_ReadOV, &dwRead, FALSE);
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
	ZeroMemory(&g_WriteOV, sizeof(OVERLAPPED));
	HANDLE hWriteFile = CreateFile(filename, GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);
	DWORD dwWritten = 0;
	if (hWriteFile != NULL)
	{
		BOOL ret = WriteFile(hWriteFile, g_pDataBuffer,dwLength, &dwWritten, &g_WriteOV);
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
			BOOL bRet = GetOverlappedResult(hWriteFile, &g_WriteOV, &dwWritten, TRUE);
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
	delete [] g_pDataBuffer;
	return dwWritten;
}

DWORD FileRead(HANDLE hHandle, DWORD offset)
{
	g_LargeRead.QuadPart += offset;
	g_ReadOV.Offset = g_LargeRead.LowPart;
	g_ReadOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwReadByte = 0;
	DWORD dwOffset = 0;
	if ((g_FileSize.QuadPart - g_LargeRead.QuadPart) < g_dwMaxReadSize)
	{
		dwOffset = g_dwMaxReadSize - (g_FileSize.QuadPart - g_LargeRead.QuadPart);
	}

	// 주의 : 버퍼의 잔여량이 g_dwMaxReadSize 보다 적을 때는 [998] Invalid access to memory location. 발생한다.
	//        충분한 버퍼의 용량을 확보하고 작업하면 된다. 또는 위 처럼 정확한 잔여량만 로드하면 된다.
	BOOL bResult = ReadFile(hHandle,g_pOffsetData, g_dwMaxReadSize- dwOffset,	&dwReadByte, &g_ReadOV);

	if (bResult == FALSE)
	{
		int iError = GetLastError();
		if (iError == ERROR_IO_PENDING)
		{
			std::cout << "로딩 중" << std::endl;
			return true;
		}		
		else
		{			
			std::cout << iError << std::endl; //ERROR_NOACCESS
			exit(1);
		}
	};
	if (bResult == TRUE)
	{
		std::cout << " 로딩완료" << std::endl;
	}
	return true;
}
DWORD FileWrite(HANDLE hHandle, DWORD offset)
{	
	g_LargeWrite.QuadPart += offset;
	g_WriteOV.Offset = g_LargeWrite.LowPart;
	g_WriteOV.OffsetHigh = g_LargeWrite.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeWrite.QuadPart];

	DWORD dwOffset = 0;
	if ((g_FileSize.QuadPart - g_LargeWrite.QuadPart) < g_dwMaxReadSize)
	{
		dwOffset = g_dwMaxReadSize - (g_FileSize.QuadPart - g_LargeWrite.QuadPart);
	}

	DWORD dwWriteByte = 0;
	BOOL bResult = WriteFile(hHandle,g_pOffsetData, g_dwMaxReadSize - dwOffset,	&dwWriteByte, &g_WriteOV);
	return bResult;
}
DWORD AsyncLoadOverlapped(const TCHAR* filename)
{
	g_LargeRead.QuadPart = 0;
	ZeroMemory(&g_ReadOV, sizeof(OVERLAPPED));

	
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hFile, &g_FileSize);
		g_pDataBuffer = new byte[g_FileSize.QuadPart];
		DWORD dwCnt = g_FileSize.QuadPart / g_dwMaxReadSize;

		FileRead(hFile, 0);

		BOOL bPending = TRUE;
		BOOL bResult = FALSE;

		while (bPending)
		{
			bResult = WaitForSingleObject(hFile, 0);
			if (bResult == WAIT_OBJECT_0)
			{
				DWORD dwTrans = 0;
				bResult = GetOverlappedResult(hFile, &g_ReadOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeRead.QuadPart + dwTrans < g_FileSize.QuadPart)
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
	return g_FileSize.QuadPart;
}
DWORD AsyncWriteOverlapped(const TCHAR* filename)
{
	g_LargeWrite.QuadPart = 0;
	ZeroMemory(&g_WriteOV, sizeof(OVERLAPPED));

	DWORD dwTrans = 0;
	HANDLE hFile = CreateFile(filename,
		GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	

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
				bResult = GetOverlappedResult(hFile, &g_WriteOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeWrite.QuadPart + dwTrans < g_FileSize.QuadPart)
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
	delete[] g_pDataBuffer;
	return g_FileSize.QuadPart;
}
int main()
{
	// 동기 파일 입출력
	//DWORD dwFileSize = Load(L"data.7z");
	//Copy(L"data1.7z", dwFileSize);
	// 비동기 파일 입출력
	//DWORD dwFileSize = AsyncLoad(L"data.7z");
	//AsyncCopy(L"data2.7z", dwFileSize);
	// 
	// 대용량 파일을   쪼개서 입출력
	DWORD dwFileSize = AsyncLoadOverlapped(L"data.7z");
	AsyncWriteOverlapped(L"data3.7z");

	//g_pDataBuffer = g_pDataBuffer;
	//Copy(L"data4.7z", dwFileSize);
    std::cout << "Hello World!\n";
}
