#include "TSession.h"
#include "TNetwork.h"
bool        TSession::Recv()
{
    // �ް�       
    ZeroMemory(&packet, sizeof(packet));

    int RecvByte = recv(sock, &buf[iBeginPos], PACKET_HEADER_SIZE - iBeginPos, 0);
    if (RecvByte == 0)
    {
        closesocket(sock);
        std::cout << "���� �������� : " << inet_ntoa(addr.sin_addr) << std::endl;
        bDisConnected = true;
        return false;
    }
    if (RecvByte < 0)
    {
        if (TNetwork::CheckError())
        {
            closesocket(sock);
            std::cout << "������ ���� ����!" << std::endl;
            bDisConnected = true;
        }
        return false;
    }

    iBeginPos += RecvByte;

    if (iBeginPos == PACKET_HEADER_SIZE)
    {
        memcpy(&packet, &buf[0], PACKET_HEADER_SIZE);
        while (packet.ph.len > iBeginPos)
        {
            int dataByte = recv(sock, &buf[0], packet.ph.len - iBeginPos, 0);
            iBeginPos += dataByte;
        }
        if (iBeginPos == packet.ph.len)
        {
            memcpy(&packet.msg, &buf[0], packet.ph.len - PACKET_HEADER_SIZE);
            iBeginPos = 0;
        }
    }
    else
    {
        return false;
    }
    return true;
}
void        TSession::SendPacket(UPACKET& packet)
{
    if (bDisConnected == false)
    {
        int SendByte = send(sock, (char*)&packet, packet.ph.len, 0);
        if (SendByte < 0)
        {
            if (TNetwork::CheckError())
            {
                closesocket(sock);
                std::cout << "������ ���� ����!" << std::endl;
                bDisConnected = true;
            }
        }
    }
}
TSession::TSession(SOCKET sock, SOCKADDR_IN addr)
{
    //RecvByte = 0;
    this->sock = sock;
    this->addr = addr;
    buf.resize(256);
    bDisConnected = false;
}