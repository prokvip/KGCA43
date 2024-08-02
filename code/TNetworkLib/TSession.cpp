#include "TSession.h"
#include "TNetwork.h"

bool        TSession::Recv()
{
    // �ް�       
    ZeroMemory(&m_packet, sizeof(m_packet));

    int RecvByte = recv(m_hSock, &m_buf[m_iBeginPos], PACKET_HEADER_SIZE - m_iBeginPos, 0);
    if (RecvByte == 0)
    {
        std::cout << "���� �������� : " << inet_ntoa(m_addr.sin_addr) << std::endl;
        m_bDisConnected = true;
        return false;
    }
    if (RecvByte < 0)
    {
        if (TNetwork::CheckError())
        {
            std::cout << "������ ���� ����!" << std::endl;
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
                std::cout << "������ ���� ����!" << std::endl;
                m_bDisConnected = true;
            }
        }
    }
    return SendByte;
}
TSession::TSession(SOCKET sock, SOCKADDR_IN addr, TNetwork* pNet)
{
    //RecvByte = 0;
    this->m_hSock = sock;
    this->m_addr = addr;
    m_buf.resize(256);
    m_bDisConnected = false;
    m_pNet = pNet;
}

void TSession::Disconnect()
{ 
    closesocket(m_hSock);
    m_bDisConnected = true;
    std::cout << "�������� : " << inet_ntoa(m_addr.sin_addr) << std::endl;
}

bool        TSession::AsyncRecv()
{
    DWORD dwReadByte = 0;
    DWORD Flags = 0;
    tOV* tov = new tOV;
    ZeroMemory(tov, sizeof(tOV));
    tov->iIOFlag = tOV::t_READ;
    m_wsaDataBuffer.len = sizeof(char) * MAX_NUM_RECV_BUFFER;
    m_wsaDataBuffer.buf = m_szDataBuffer;
    int iRet = WSARecv(m_hSock, &m_wsaDataBuffer, 1, &Flags, &dwReadByte,
        &tov->ov, nullptr);
    if (iRet < 0)
    {
        if (WSAGetLastError() == ERROR_IO_PENDING)
        {
            return true;
        }
        else
        {
            m_bDisConnected = true;
            TNetwork::CheckError();
            return false;
        }
    }
    return true;
}
bool        TSession::Dispatch(DWORD dwTransfer, tOV* ov)
{
    if (ov->iIOFlag == tOV::t_READ)
    {
        // ��Ŷ�ϼ����� üũ
        Put(dwTransfer);
    }
    if (ov->iIOFlag == tOV::t_SEND)
    {

    }
    delete ov;
    AsyncRecv();
    return true;
}
bool TSession::Put(DWORD dwSize)
{
    _ASSERT(m_pNet);
    if (m_dwWritePos + dwSize >= MAX_NUM_PACKET_BUFFER- PACKET_SIZE)
    {
        // ���� �ܿ��� �ִ� ���
        if (m_dwReadPos > 0)
        {
            memmove(m_szPacketBuffer, &m_szPacketBuffer[m_dwStartPacketPos], m_dwReadPos);
        }
        m_dwStartPacketPos = 0;
        m_dwWritePos = m_dwReadPos;
    }
    memcpy(&m_szPacketBuffer[m_dwWritePos], m_szDataBuffer, dwSize);
    m_dwWritePos += dwSize;
    m_dwReadPos += dwSize;
    if (m_dwReadPos >= PACKET_HEADER_SIZE)
    {
        UPACKET* packet = (UPACKET*)&m_szPacketBuffer[m_dwStartPacketPos];
        //���� �����Ͱ� 1���� ��Ŷ���� ũ�ٸ�.
        if (packet->ph.len <= m_dwReadPos)
        {
            // 1�� �̻��� ��Ŷ�� ó���ϱ� ���ؼ�
            do {
                //UPACKET newPacket;
                m_pNet->AddPacket(*packet);
                m_dwReadPos -= packet->ph.len;
                m_dwStartPacketPos += packet->ph.len;
                if (m_dwReadPos < PACKET_HEADER_SIZE)
                {
                    break;
                }
                packet = (UPACKET*)&m_szPacketBuffer[m_dwStartPacketPos];
            } while (packet->ph.len <= m_dwReadPos);
        }
    }
    // �۾����
    // 1���� ����Ʈ ���۸� ���.
    // ���Ḯ��Ʈ ���.
    // std::string szData = m_szDataBuffer;
    // m_dataList.push_back(szData);
    // ����ȭ(����ü, Ŭ����) , ������ȭ    
    return false;
}