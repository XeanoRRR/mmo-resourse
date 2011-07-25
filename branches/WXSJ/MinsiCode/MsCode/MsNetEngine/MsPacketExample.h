#pragma once                    //��//��
#ifndef __MSPACKETEXAMPLE_H__   //��//��
#define __MSPACKETEXAMPLE_H__   //��//��
//����������������������������������//��
//�ļ����ƣ�MsPacketExample.h       //��
//������������������������        //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ָ���ڴ����Ϊ1���ֽڶ���
#pragma pack(push, 1)

// ������
class CMsPacketExample
    :public CMsBasePacket
{
public:
    CMsPacketExample(VOID);

    // ��ȡ��Ч���ݵ�ַ
    virtual LPVOID GetValidPacketData(DWORD* lpSize);

    // ��ȡ��Ч���ݳߴ�
    virtual DWORD GetValidPacketDataSize(VOID);

    // ִ�з��
    virtual BOOL  ExecutePacket(CMsSocket* lpCMsSocket);

public:
    // ��Ч�������
    typedef struct
    {
        DWORD   m_dwData;
    }MS_PACKET_DATA, *LPMS_PACKET_DATA;
};

extern CMsPacketExample g_CMsPacketExample;

// �ָ�ԭ�����ڴ����
#pragma pack(pop)

//����������������������
#endif//#pragma once//��
//����������������������
