#pragma once                //��    //��
#ifndef __MSPACKETDEF_H__   //��    //��
#define __MSPACKETDEF_H__   //��    //��
//������������������������������    //��
//�ļ����ƣ�MsPacketDef.h           //��
//������������������������        //��
//ƽ̨��أ�Windows                 //��
//��������������������������������������

// ���ͷ��ʶ��
#define PACKET_IDENTIFIER   (DWORD)0xCCDDAABB

// ������ߴ�
#define MAX_PACKET_LEN      KB_SIZE * 10

// ���������, ��ͬ���û� E_PACKET_TYPE ����
typedef DWORD PACKET_TYPE;

// �����Կ����ö��
enum E_KEY_TYPE
{
    KEY_TYPE_NORMAL = 0,
    KEY_TYPE_USER_ID,
    KEY_TYPE_COUNT,
    KEY_TYPEY_MAX = 0xFF,
};

// �궨��ע��ȫ�ַ��
#define REGISTER_PACKET(pack)   &g_##pack

// ������ID
#define BUILD_PACKET_TYPE(pack) g##pack##_ID
#define BPT BUILD_PACKET_TYPE

// ��ʼ������
#define BUILD_PACKET_BEGIN(pack,type)   static PACKET_TYPE g##pack##_ID = type;                                 \
                                        class pack:public CMsBasePacket{                                        \
                                        public:                                                                 \
                                        pack::pack(VOID)                                                        \
                                        :CMsBasePacket(type, sizeof(MS_PACKET_DATA) + sizeof(PacketHead)){}     \
                                        virtual BOOL ExecutePacket(CMsSocket* lpCMsSocket);                     \
                                        typedef

// ����������
#define BUILD_PACKET_END(pack)          MS_PACKET_DATA, *LPMS_PACKET_DATA;}; static pack g_##pack;


// ָ���ڴ����Ϊ1���ֽڶ���
#pragma pack(push, 1)

// ��ͷ
struct PacketHead
{
    PacketHead(
        DWORD wIdentifier,
        PACKET_TYPE ePackType,
        E_KEY_TYPE eKeyType,
        DWORD dwSize
        );

    PacketHead(VOID)
    {
        ZeroMemory(this, sizeof(PacketHead));
    }

    // ������Ƿ�Ϸ�
    BOOL CheckRegular(VOID);
    DWORD           m_dwIdentifier; // �����ʶ��
    PACKET_TYPE     m_ePackType;    // �������     E_PACKET_TYPE
    E_KEY_TYPE      m_eKeyType;     // �����Կ����
    DWORD           m_dwPackSize;   // �����С
    INT64           m_i64Time;      // ���64λʱ���
};
typedef PacketHead* lpPacketHead;

class CMsSocket;
// ������
class CMsBasePacket
{
public:
    CMsBasePacket(
        PACKET_TYPE ePackType,
        DWORD dwSize,
        DWORD wIdentifier = PACKET_IDENTIFIER,
        E_KEY_TYPE eKeyType = KEY_TYPE_NORMAL
        );

    // �������
    VOID Lock(VOID);

    // ��ȡ����ߴ�
    DWORD GetPacketSize(VOID);

    // ��ȡ�������
    PACKET_TYPE GetPacketType(VOID);

    // ��ȡ���ʱ��
    INT64 GetPacketTime(VOID);

    // ��ȡ�����Կ
    E_KEY_TYPE GetPacketKeyType(VOID);

    // �жϷ���Ƿ�������
    BOOL IsLock(VOID);

    // �����
    BOOL CheckRegular(VOID);

    // ��ȡ��ͷ��ַ
    lpPacketHead  GetPacketHead(DWORD* lpSize = NULL);

    // ���÷��
    virtual VOID  Reset(VOID);

    // ִ�з��
    virtual BOOL  ExecutePacket(CMsSocket* lpCMsSocket);

private:
    // ��ǰ�����Ƿ�����
    BOOL    m_IsLock;

    // ���ͷ
    PacketHead  m_PackHead;
public:
    // ��Ч�������
    typedef struct
    {
    }MS_PACKET_DATA, *LPMS_PACKET_DATA;
};
typedef CMsBasePacket* lpCMsBasePacket;

// �ָ�ԭ�����ڴ����
#pragma pack(pop)

//����������������������
#endif//#pragma once//��
//����������������������
