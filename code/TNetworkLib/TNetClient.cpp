#include "TNetClient.h"


bool TNetClient::Connected(std::string ip, USHORT port)
{
    SOCKADDR_IN sa;
    ZeroMemory(&sa, sizeof(sa));
    sa.sin_addr.s_addr = inet_addr(ip.c_str());
    //inet_pton(AF_INET, ip.c_str(), &sa.sin_addr.s_addr);
    sa.sin_port = htons(port);
    sa.sin_family = AF_INET;
    int namelen = sizeof(sa);
    int ret = connect(m_hSock, (sockaddr*)&sa, namelen);
    if (ret != 0)
    {
        if (CheckError() == true)
        {
            return false;
        }
    }
    return true;
}
bool TNetClient::Recv()
{  
    UPACKET packet;
    ZeroMemory(&packet, sizeof(packet));

    int RecvByte = recv(m_hSock, &Recvbuf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
    if (RecvByte == 0)
    {
        std::cout << "서버 종료!" << std::endl;
        return false;
    }
    if (RecvByte < 0)
    {
        if (CheckError())
        {
            std::cout << "비정상 서버 종료!" << std::endl;
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
            }
            if (packet.ph.type == PACKET_CHAT_MSG)
            {
                std::cout << packet.msg << std::endl;
            }
            iBeginPos = 0;
        }
    }
    return true;
}
void   TNetClient::Run() 
{
    while (m_bRun)
    {
        if (Recv() == false)
        {
            break;
        }
    }
}