#include "TNetClient.h"
void   TNetClient::Run() 
{
    std::string Recvbuf;
    Recvbuf.resize(256);

    UINT  iBeginPos = 0;
    bool  bRun = true;

    while (bRun)
    {
        UPACKET packet;
        ZeroMemory(&packet, sizeof(packet));

        int RecvByte = recv(m_hSock, &Recvbuf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
        if (RecvByte == 0)
        {
            std::cout << "서버 종료!" << std::endl;
            break;
        }
        if (RecvByte < 0)
        {
            if (CheckError())
            {
                std::cout << "비정상 서버 종료!" << std::endl;
                break;
            }
            continue;
        }

        iBeginPos += RecvByte;

        if (iBeginPos == PACKET_HEADER_SIZE)
        {
            memcpy(&packet, &Recvbuf[0], PACKET_HEADER_SIZE);
            while (packet.ph.len > iBeginPos)
            {
                int dataByte = recv(m_hSock, &Recvbuf[0], packet.ph.len - iBeginPos, 0);
                if (RecvByte == 0)
                {
                    std::cout << "서버 종료!" << std::endl;
                    bRun = false;
                    break;
                }
                if (dataByte < 0)
                {
                    if (CheckError())
                    {
                        std::cout << "비정상 서버 종료!" << std::endl;
                        bRun = false;
                        break;
                    }
                }
                iBeginPos += dataByte;
            }
            if (bRun)
            {
                if (iBeginPos == packet.ph.len)
                {
                    memcpy(&packet.msg, &Recvbuf[0], packet.ph.len - PACKET_HEADER_SIZE);
                }
                if (packet.ph.type == PACKET_CHAT_MSG)
                {
                    std::cout << packet.msg << std::endl;
                }
                iBeginPos = 0;
            }
        }
    }
}