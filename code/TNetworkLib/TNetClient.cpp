#include "TNetClient.h"


bool TNetClient::Recv()
{  
    UPACKET packet;
    ZeroMemory(&packet, sizeof(packet));

    int RecvByte = recv(m_hSock, &Recvbuf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
    if (RecvByte == 0)
    {
        std::cout << "서버 종료!" << std::endl;
        m_bRun = false;
        return false;
    }
    if (RecvByte < 0)
    {
        if (CheckError())
        {
            std::cout << "비정상 서버 종료!" << std::endl;
            m_bRun = false;
            return false;
        }
        return true;
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
                m_bRun = false;
                return false;
            }
            if (dataByte < 0)
            {
                if (CheckError())
                {
                    std::cout << "비정상 서버 종료!" << std::endl;
                    m_bRun = false;
                    return false;
                }
            }
            iBeginPos += dataByte;
        }
        if (m_bRun)
        {
            if (iBeginPos == packet.ph.len)
            {
                memcpy(&packet.msg, &Recvbuf[0], packet.ph.len - PACKET_HEADER_SIZE);
                AddPacket(packet);
            }            
            iBeginPos = 0;
        }
    }
    return true;
}
void   TNetClient::Run() 
{
    _ASSERT(m_pSelectIO);
    while (m_bRun)
    {
        if (!m_pSelectIO->Run())
        {
            break;
        }        
    }
}