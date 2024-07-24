#include <windows.h>
#include <iostream>
TCHAR* pFileBuffer = 0;
OVERLAPPED readOV;
OVERLAPPED writeOV;
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
		pFileBuffer = new TCHAR[FileSize.LowPart];
		DWORD dwLength = FileSize.LowPart;
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
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED| FILE_FLAG_WRITE_THROUGH, 
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
			BOOL bRet = GetOverlappedResult(hWriteFile, &readOV, &dwWritten, FALSE);
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
int main()
{
	// 동기 파일 입출력
	//DWORD dwFileSize = Load(L"mssql.ISO");
	//Copy(L"mssqlCopy.ISO", dwFileSize);
	// 비동기 파일 입출력
	DWORD dwFileSize = AsyncLoad(L"3dsmax.zip");
	AsyncCopy(L"Copy3dsmax.zip", dwFileSize);

    std::cout << "Hello World!\n";
}
