#include <windows.h>
#include <iostream>
#include <assert.h>
#include <thread>
#include <vector>
#define MAX_NUM_WORKDER_THREAD 4
// 비동기 입출력
// 단점 : 작업을 시작한 스레드만 결과 확인이 가능하다.
// 해결 : -> 어떤 스레드에서 비동기 작업[10]을 시작하더라도
//        -> 원하는 스레드에서 작업의 결과를 확인해야 한다.
// IOCP : -> 작업스레드(3스레드:스레드풀)가 있고 각각 비동기 작업을 시작하더라도
//        -> 작업스레드(3스레드)의 임의의 스레드가 작업의 결과를 확인 할 수 있다.
OVERLAPPED		g_ReadOV;
OVERLAPPED		g_WriteOV;
byte* g_pOffsetData = nullptr;
byte* g_pDataBuffer = nullptr;
LARGE_INTEGER	g_FileSize;
LARGE_INTEGER	g_LargeRead;
LARGE_INTEGER	g_LargeWrite;
const DWORD		g_dwMaxReadSize = 8192 * 8192;
HANDLE			g_hReadFile;
HANDLE			g_hWriteFile;

HANDLE   g_hIOCP;// input, output, completion , port
HANDLE   g_hWorkerThread[2];

// * 비동기 작업 당 오버랩드 구조체는 1개씩 배정되어야 한다.

bool DispatchRead(DWORD dwTransfer, LPOVERLAPPED  Overlapped)
{
	// 다음 로드 트랙 갱신
	g_LargeRead.QuadPart += dwTransfer;

	// 로드 완료 -> 출력해라.
	g_WriteOV.Offset = g_LargeWrite.LowPart;
	g_WriteOV.OffsetHigh = g_LargeWrite.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeWrite.QuadPart];

	DWORD dwWriteByte = 0;
	BOOL bResult = WriteFile(g_hWriteFile,
		g_pOffsetData, dwTransfer, &dwWriteByte, &g_WriteOV);

	if (bResult)
	{
		DWORD iError = GetLastError();
	}
	return true;
}
bool DispatchWrite(DWORD dwTransfer, LPOVERLAPPED  Overlapped)
{
	g_LargeWrite.QuadPart += dwTransfer;
	if (g_LargeWrite.QuadPart == g_FileSize.QuadPart)
	{
		return false;
	}
	g_ReadOV.Offset = g_LargeRead.LowPart;
	g_ReadOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwOffset = 0;
	if ((g_FileSize.QuadPart - g_LargeRead.QuadPart) < g_dwMaxReadSize)
	{
		dwOffset = g_dwMaxReadSize - (g_FileSize.QuadPart - g_LargeRead.QuadPart);
	}

	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(g_hReadFile, g_pOffsetData, g_dwMaxReadSize - dwOffset, &dwReadByte, &g_ReadOV);
	if (bResult)
	{
		DWORD iError = GetLastError();
	}

	return true;
}

bool g_bRun = true;
DWORD  WINAPI  WorkerProc(LPVOID lpThreadParameter)
{
	DWORD		dwTransfer;
	ULONG_PTR	CompletionKey;
	OVERLAPPED  Overlapped;
	LPOVERLAPPED lpOverlapped = nullptr;
	while (g_bRun)
	{
		// 비동기 작업이 마치면 IOCP 큐에 저장된다. 이때. 반환한다.
		/*_In_ HANDLE CompletionPort,
		_Out_ LPDWORD lpNumberOfBytesTransferred,
		_Out_ PULONG_PTR lpCompletionKey,
		_Out_ LPOVERLAPPED* lpOverlapped,
		_In_ DWORD dwMilliseconds*/
		BOOL ret = GetQueuedCompletionStatus(g_hIOCP,
			&dwTransfer, &CompletionKey,
			&lpOverlapped, 1000);

		if (ret == TRUE)
		{
			if (CompletionKey == 1000)
			{
				DispatchRead(dwTransfer, lpOverlapped);
			}
			if (CompletionKey == 2000)
			{
				if (DispatchWrite(dwTransfer, lpOverlapped) == false)
				{
					g_bRun = false;
					break;
				}
			}
		}
		else
		{
			DWORD iError = GetLastError();
			if (iError != WAIT_TIMEOUT)
			{
				g_bRun = false;
				break;
			}
		}
	}
	return TRUE;
}

int main()
{
	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;
	ZeroMemory(&g_ReadOV, sizeof(OVERLAPPED));
	ZeroMemory(&g_WriteOV, sizeof(OVERLAPPED));

	g_hReadFile = CreateFile(L"data.7z",
		GENERIC_READ, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (g_hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(g_hReadFile, &g_FileSize);
		g_pDataBuffer = new byte[g_FileSize.QuadPart];
		DWORD dwCnt = g_FileSize.QuadPart / g_dwMaxReadSize;
	}


	g_hWriteFile = CreateFile(L"copy.7z",
		GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (g_hWriteFile != INVALID_HANDLE_VALUE)
	{
	}
	//   100 byte
	//read  : [0]0~10, [2]10~20
	//write : [1]0~10, [3]10~20


	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	//_In_ HANDLE FileHandle,
	//_In_opt_ HANDLE ExistingCompletionPort,
	//_In_ ULONG_PTR CompletionKey,
	//_In_ DWORD NumberOfConcurrentThreads
	::CreateIoCompletionPort(g_hReadFile, g_hIOCP, 1000, 0);
	::CreateIoCompletionPort(g_hWriteFile, g_hIOCP, 2000, 0);

	DWORD threadID;
	for (int iThread = 0; iThread < MAX_NUM_WORKDER_THREAD; iThread++)
	{
		g_hWorkerThread[iThread] = CreateThread(0, 0, WorkerProc,
			nullptr, 0, &threadID);
	}


	g_LargeRead.QuadPart = 0;
	g_ReadOV.Offset = g_LargeRead.LowPart;
	g_ReadOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(g_hReadFile, g_pOffsetData, g_dwMaxReadSize, &dwReadByte, &g_ReadOV);

	WaitForMultipleObjects(MAX_NUM_WORKDER_THREAD, g_hWorkerThread, TRUE, INFINITE);
	// th.join();

	for (int iThread = 0; iThread < MAX_NUM_WORKDER_THREAD; iThread++)
	{
		CloseHandle(g_hWorkerThread[iThread]);
	}
	CloseHandle(g_hReadFile);
	CloseHandle(g_hWriteFile);
	std::cout << "Hello World!\n";
}
