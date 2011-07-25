#pragma once                    //��//��
#ifndef __CMSPACKETFACTORY_H__  //��//��
#define __CMSPACKETFACTORY_H__  //��//��
//����������������������������������//��
//�ļ����ƣ�CMsPacketFactory.h      //��
//��������������������            //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

class CMsPacketFactory
{
public:
    // ����
    CMsPacketFactory(PACKET_TYPE PacketMaxCount);

    // ����
    ~CMsPacketFactory(VOID);

    // ע����
    BOOL Register(lpCMsBasePacket* lppPacket, DWORD dwPacketCount);

    // ��ȡ���Ĭ�ϳߴ�
    DWORD GetPacketDefaultSize(PACKET_TYPE PacketId);

    // ��ȡ�������Ĭ�ϳߴ�
    DWORD GetPacketDataDefaultSize(PACKET_TYPE PacketId);

    // ��ȡ���
    CMsBasePacket*  GetPacket(PACKET_TYPE PacketId);

    // ������ͷ�õ����
    CMsBasePacket*  ParsePacketHead(PacketHead* lpPacketHead);

private:
    lpCMsBasePacket*    m_ppPacketPool;     // ���ָ���
    PACKET_TYPE         m_PacketMaxCount;   // ���������
};

//����������������������
#endif//#pragma once//��
//����������������������
