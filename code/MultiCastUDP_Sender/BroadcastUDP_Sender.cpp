#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
//http://www.steves-internet-guide.com/introduction-multicasting/
//멀티캐스트는 Class D IP 주소 범위(224.0.0.0~239.255.255.255)를 사용한다.
//인터넷(WAN), 일반 인터넷 환경에서는 사실상 사용할 수 없다.
void Error(const char* msg)
{
	LPVOID lpMsgBuffer;
	int iError = WSAGetLastError();
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		iError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuffer,
		0, NULL);
	printf("%s", lpMsgBuffer);
	LocalFree(lpMsgBuffer);
	//exit(-1);
}
int SetMulticastLoopBack(SOCKET s, int af, int loopval)
{
	char* optval = NULL;
	int   optlevel, option, optlen, rc;

	optlevel = IPPROTO_IP;
	option = IP_MULTICAST_LOOP;
	optval = (char*)&loopval;
	optlen = sizeof(loopval);

	rc = setsockopt(s, optlevel, option, optval, optlen);
	if (rc == SOCKET_ERROR)
	{
		fprintf(stderr, "SetMulticastLoopBack: setsockopt() failed with error code %d\n", WSAGetLastError());
	}
	else
	{
		printf("Setting multicast loopback to: %d\n", loopval);
	}
	return rc;
}
void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		Error("sock invalid");
	}
	// socket option
	// IP_MULTICAST_LOOP = 전송한 사본을 수신받기 위함.
	SetMulticastLoopBack(sock, AF_INET, true);

	BOOL optval = TRUE;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	//int ttl = 10;
	//setsockopt(sock, IPPROTO_IP,IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));
	// 네트웍카드가 2개 이상 일때 전송 기본 인터페이스를 지정할 수 있다.
	ULONG addr = inet_addr("192.168.0.12");
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&addr, sizeof(addr));

	/*SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.0.12");*/

	SOCKADDR_IN serveradd;
	ZeroMemory(&serveradd, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_port = htons(9000);
	// 멀티케스트 그룹에 가입하려면 반드시 INADDR_ANY를 사용해야 한다.
	serveradd.sin_addr.s_addr = inet_addr("192.168.0.12"); //htonl(INADDR_ANY);
	int iRet = bind(sock, (SOCKADDR*)&serveradd, sizeof(serveradd));
	if (iRet == SOCKET_ERROR)
	{
		Error("sock invalid");
	}

	SOCKADDR_IN multicastAddr;
	ZeroMemory(&multicastAddr, sizeof(multicastAddr));
	multicastAddr.sin_family = AF_INET;
	multicastAddr.sin_port = htons(9000);
	multicastAddr.sin_addr.s_addr = inet_addr("235.7.8.9");	

	bool bMulticastSourcing = false;
	struct ip_mreq mreq;
	struct ip_mreq_source mreqsrc;

	if (bMulticastSourcing)
	{
		// INCLUDE 대상 포함 
		// IP_ADD_SOURCE_MEMBERSHIP <->IP_DROP_SOURCE_MEMBERSHIP
		mreqsrc.imr_interface.s_addr = htonl(INADDR_ANY);
		mreqsrc.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
		mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.40");
		setsockopt(sock, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)&mreqsrc, sizeof(mreqsrc));
		mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.12");
		setsockopt(sock, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)&mreqsrc, sizeof(mreqsrc));
		mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.20");
		setsockopt(sock, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)&mreqsrc, sizeof(mreqsrc));
	}
	else
	{
		// EXCLUDE 대상 삭제
		// IP_ADD_MEMBERSHIP <-> IP_BLOCK_SOURCE		
		mreq.imr_multiaddr.s_addr = inet_addr("235.7.8.9");
		mreq.imr_interface.s_addr = inet_addr("192.168.0.12");
		iRet = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}	
		mreqsrc.imr_interface = mreq.imr_interface;
		mreqsrc.imr_multiaddr = mreq.imr_multiaddr;
		mreqsrc.imr_sourceaddr.s_addr = inet_addr("192.168.0.33");
		iRet = setsockopt(sock, IPPROTO_IP, IP_BLOCK_SOURCE, (char*)&mreqsrc, sizeof(mreqsrc));
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
	}
	printf("\nRunning Host");

	SOCKADDR_IN clientAddr;
	int addlen = sizeof(clientAddr);
	char buf[256] = "kgca";
	iRet = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&multicastAddr, sizeof(multicastAddr));
	if (iRet == SOCKET_ERROR)
	{
		Error("sock invalid");
	}

	iRet = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&clientAddr, &addlen);
	if (iRet == SOCKET_ERROR)
	{
		Error("sock invalid");
	}
	buf[iRet] = 0;
	printf("\nServer->[%s:%d]:%s", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buf);

	while (1)
	{
		char buf[256] = "kgca";
		iRet = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&multicastAddr, sizeof(multicastAddr));
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}

		iRet = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&clientAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
		buf[iRet] = 0;
		printf("\nMulticast->[%s:%d]:%s", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), buf);
		Sleep(1000);

	}
	// 응용프로그램을 종료하거나 소켓을 닫으면 그룹 설정이 지워진다.
	setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	WSACleanup();
}