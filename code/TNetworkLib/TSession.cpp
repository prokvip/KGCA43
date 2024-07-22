#include "TSession.h"
#include "TNetwork.h"
bool        TSession::Recv()
{
    // 받고       
    ZeroMemory(&m_packet, sizeof(m_packet));

    int RecvByte = recv(m_hSock, &m_buf[m_iBeginPos], PACKET_HEADER_SIZE - m_iBeginPos, 0);
    if (RecvByte == 0)
    {
        std::cout << "정상 접속종료 : " << inet_ntoa(m_addr.sin_addr) << std::endl;
        m_bDisConnected = true;
        return false;
    }
    if (RecvByte < 0)
    {
        if (TNetwork::CheckError())
        {
            std::cout << "비정상 서버 종료!" << std::endl;
            m_bDisConnected = true;
        }
        return false;
    }

    m_iBeginPos += RecvByte;

    if (m_iBeginPos == PACKET_HEADER_SIZE)
    {
        memcpy(&m_packet, &m_buf[0], PACKET_HEADER_SIZE);
        while (m_packet.ph.len > m_iBeginPos)
        {
            int dataByte = recv(m_hSock, &m_buf[0], m_packet.ph.len - m_iBeginPos, 0);
            m_iBeginPos += dataByte;
        }
        if (m_iBeginPos == m_packet.ph.len)
        {
            memcpy(&m_packet.msg, &m_buf[0], m_packet.ph.len - PACKET_HEADER_SIZE);
            m_iBeginPos = 0;
        }
    }
    else
    {
        return false;
    }
    return true;
}
int        TSession::SendPacket(UPACKET& packet)
{
    int SendByte = 0;
    if (m_bDisConnected == false)
    {
        SendByte = send(m_hSock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (TNetwork::CheckError())
            {
                std::cout << "비정상 서버 종료!" << std::endl;
                m_bDisConnected = true;
            }
        }
    }
    return SendByte;
}
TSession::TSession(SOCKET sock, SOCKADDR_IN addr)
{
    //RecvByte = 0;
    this->m_hSock = sock;
    this->m_addr = addr;
    m_buf.resize(256);
    m_bDisConnected = false;
}

void TSession::Disconnect()
{ 
    closesocket(m_hSock);
    m_bDisConnected = true;
    std::cout << "접속종료 : " << inet_ntoa(m_addr.sin_addr) << std::endl;
}