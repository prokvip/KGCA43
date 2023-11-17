#include "TStdSelect.h"
bool TStdSelect::Init()
{
    //                           IP                    TCP
    //SOCKET sockTCP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //SOCKET sockUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    m_hSock = socket(AF_INET, SOCK_STREAM, 0);

    /// <summary>
    /// WSAAsyncSelect()
    /// </summary>
    /// <returns></returns>
    WSAAsyncSelect(m_hSock, g_hWnd, NETWORK_MSG,
        FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT);

    SOCKADDR_IN sa;
    sa.sin_family = AF_INET;
    //error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
    sa.sin_addr.s_addr = inet_addr("192.168.0.12");
    sa.sin_port = htons(m_iPort);
    int ret = connect(m_hSock, (SOCKADDR*)&sa, sizeof(sa));

    if (ret == 0)
    {
        printf("서버 접속 ip=%s, Port:%d 했습니다.\n",
            inet_ntoa(sa.sin_addr),
            ntohs(sa.sin_port));
    }
    else
    {
        printf("서버 접속 ip=%s, Port:%d 불가, 서버켜.\n",
            inet_ntoa(sa.sin_addr),
            ntohs(sa.sin_port));
    }

    u_long on = TRUE;
    ioctlsocket(m_hSock, FIONBIO, &on);
    return true;
}
bool TStdSelect::Release()
{
    return true;
}
bool TStdSelect::Run()
{
    // {
    //    FD_ZERO(&readSet);
    //    FD_ZERO(&sendSet);
    //    FD_ZERO(&execptSet);
    //    FD_SET(m_hSock, &readSet);

    //    for (std::list<SOCKET>::iterator iter = g_userlist.begin();
    //        iter != g_userlist.end();
    //        iter++)
    //    {
    //        FD_SET(*iter, &readSet);
    //        FD_SET(*iter, &sendSet);
    //    }
    //    iRet = select(0, &readSet, &sendSet, NULL, &timeout);
    //    if (iRet == SOCKET_ERROR) break;
    //    if (iRet == 0)
    //    {
    //        continue;
    //    }
    //    //accept
    //    if (FD_ISSET(m_hSock, &readSet))
    //    {
    //        int addlen = sizeof(clientaddr);
    //        clientsock = accept(m_hSock, (SOCKADDR*)&clientaddr, &addlen);
    //        if (clientsock == SOCKET_ERROR)
    //        {
    //            int iError = WSAGetLastError();
    //            if (iError != WSAEWOULDBLOCK)
    //            {
    //                break;
    //            }
    //        }
    //        else
    //        {
    //            g_userlist.push_back(clientsock);
    //            printf("클라이언트 접속 ip=%s, Port:%d\n",
    //                inet_ntoa(clientaddr.sin_addr),
    //                ntohs(clientaddr.sin_port));
    //        }
    //    }

    //    //recv or Send
    //    for (std::list<SOCKET>::iterator iter = g_userlist.begin();
    //        iter != g_userlist.end();
    //        )
    //    {
    //        if (FD_ISSET(*iter, &readSet))
    //        {
    //            SOCKET cSock = *iter;
    //            char buf[256] = { 0, };
    //            int iRecvByte = recv(cSock, buf, 256, 0);
    //            if (iRecvByte == SOCKET_ERROR)
    //            {
    //                closesocket(*iter);
    //                iter = g_userlist.erase(iter);
    //                continue;
    //            }
    //            else
    //            {
    //                if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
    //                {
    //                    closesocket(*iter);
    //                    iter = g_userlist.erase(iter);
    //                    continue;
    //                }

    //                printf("[받음]%s\n", buf);
    //                int iLen = strlen(buf);

    //                for (std::list<SOCKET>::iterator iterSend = g_userlist.begin();
    //                    iterSend != g_userlist.end();
    //                    )
    //                {
    //                    SOCKET cSock = *iterSend;

    //                    int iSendByte = send(cSock, buf, iLen, 0);
    //                    if (iSendByte == SOCKET_ERROR)
    //                    {
    //                        closesocket(*iterSend);
    //                        iterSend = g_userlist.erase(iterSend);
    //                        continue;
    //                    }
    //                    iterSend++;
    //                }
    //            }
    //        }
    //        /*for (std::list<SOCKET>::iterator iter = g_userlist.begin();
    //            iter != g_userlist.end();
    //            iter++)
    //        {
    //            if (FD_ISSET(*iter, &sendSet))
    //            {
    //                break;
    //            }
    //        }*/
    //        iter++;
    //    }  
    //}
	return true;
}
TStdSelect::~TStdSelect()
{

}