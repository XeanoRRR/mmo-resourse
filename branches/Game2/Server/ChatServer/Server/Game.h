#pragma once

#include "../../../Public/ServerPublic/Server/Common/AsynchronousExecute.h"

class CServer;
class Msg;
class CMyAdoBase;
class CMySocket;
class CDataBlockAllocator;

void AddLoginClientList(const char *str);
void DelLoginClientList(const char *str);

enum eNetFlag
{
    NF_Server_GameServer=0x1000000,
    NF_Server_GameClient=0x100,
};
// ��Ϸȫ�ֿ���
class Game
{
public:
    Game();
    ~Game();

public:
    bool Init();
    bool Release();
    bool MainLoop();

    bool ProcessMessage();

    bool InitNetServer4GS();
    bool InitNetServer4Client();

    void DisconnectGS(long socket);
    void DisconnectClient(long socket);

private:
    //���ݿ������
    CDataBlockAllocator *m_pDBAllocator;

    CServer* m_NetServer4GS;        //�������GS��Ϣ���������
    CServer* m_NetServer4Client;    //������ܿͻ�����Ϣ���������

    bool ProcMenus();

public:
    CServer* GetNetServer4GS() const {return m_NetServer4GS;}
    CServer* GetNetServer4Client() const {return m_NetServer4Client;}

    /////////////////////////////////////////////////////////////////////////
    // �̹߳���
    /////////////////////////////////////////////////////////////////////////
public:
    HANDLE m_hEndEvent[2];      // �����¼�
    bool m_bExit;               // �˳���Ϸ

    AsynchronousExecute&    GetAsynchronousExecute(VOID)        {return m_AsynchronousExecute;}
    string&                 GetConnectionString(VOID)           {return m_strConnectionString;}
    string&                 GetFcmConnectionString(VOID)        {return m_strFcmConnectionString;}
    string&                 GetSecurityConnectionString(VOID)   {return m_strSecurityConnectionString;}

private:
    //!                     �첽ִ�ж���
    AsynchronousExecute     m_AsynchronousExecute;
    //!
    string                  m_strConnectionString;
    string                  m_strFcmConnectionString;
    string                  m_strSecurityConnectionString;


    /////////////////////////////////////////////////////////////////////////
    // ��Ϸ����
    /////////////////////////////////////////////////////////////////////////
public:
    struct tagSetup
    {
        long lServerID;             //! AS�Լ���ID
        ulong dwListenPort_Client;  // ����Client��Port
        ulong dwListenPort_GS;    // ����GameServer��Port

        string strSqlConType;       // sql server connection type
        string strSqlServerIP;      // sql server ip address
        string strSqlUserName;
        string strSqlPassWord;
        string strDBName;

        string strSecuritySqlConType;   
        string strSecuritySqlServerIP;
        string strSecuritySqlUserName;
        string strSecuritySqlPassWord;
        string strSecurityDBName;

        string strFcmSqlConType;   
        string strFcmSqlServerIP;
        string strFcmSqlUserName;
        string strFcmSqlPassWord;
        string strFcmDBName;

        // Client����������ò���
        ulong dwDataBlockNum;   // ���ݿ�����
        ulong dwDataBlockSize;  // ���ݿ��С
        ulong dwFreeMsgNum;     // Ԥ�������Ϣ����

        ulong dwFreeSockOperNum_GS;         // �����������Ԥ��������
        ulong dwFreeIOOperNum_GS;           // ��ɶ˿���IO����Ԥ��������
        long lIOOperDataBufNum_GS;          // Ĭ��IO������DataBuf����
        bool bCheckNet_GS;                  // �Ƿ��������м��
        ulong dwBanIPTime_GS;               // ��ֹIP��ʱ��
        ulong dwMaxMsgLen_GS;               // ������������Ϣ����  
        long lMaxConnectNum_GS;             // �ͻ��˵������������
        long lMaxClientsNum_GS;             // ���Ŀͻ��˷��ͻ�������С
        long lPendingWrBufNum_GS;           // ���ķ���IO����Buf�ܴ�С
        long lPendingRdBufNum_GS;           // ���Ľ���IO����Buf�ܴ�С
        long lMaxSendSizePerSecond_GS;      // ��ͻ���ÿ�뷢�͵�����ֽ���
        long lMaxRecvSizePerSecond_GS;      // �ӿͻ��˽��ܵ�ÿ������ֽ���
        long lMaxBlockedSendMsgNum_GS;      // ��������ķ�����Ϣ����
        long lConPendingWrBufNum_GS;        // �ͻ������ķ���IO����Buf�ܴ�С
        long lConPendingRdBufNum_GS;        // �ͻ������Ľ���IO����Buf�ܴ�С
        long lConMaxSendSizePerSecond_GS;   // ����������͵�ÿ������ֽ���
        long lConMaxRecvSizePerSecond_GS;   // �ӷ��������ܵ�ÿ������ֽ���
        long lConMaxBlockedSendMsgNum_GS;   // ��������ķ�����Ϣ����
        long lMaxBlockConNum_GS;
        long lValidDelayRecDataTime_GS;

        ulong dwFreeSockOperNum_Client;         // �����������Ԥ��������
        ulong dwFreeIOOperNum_Client;           // ��ɶ˿���IO����Ԥ��������
        long lIOOperDataBufNum_Client;          // Ĭ��IO������DataBuf����
        bool bCheckNet_Client;                  // �Ƿ��������м��
        ulong dwBanIPTime_Client;               // ��ֹIP��ʱ��
        ulong dwMaxMsgLen_Client;               // ������������Ϣ����  
        long lMaxConnectNum_Client;             // �ͻ��˵������������
        long lMaxClientsNum_Client;             // ���Ŀͻ��˷��ͻ�������С
        long lPendingWrBufNum_Client;           // ���ķ���IO����Buf�ܴ�С
        long lPendingRdBufNum_Client;           // ���Ľ���IO����Buf�ܴ�С
        long lMaxSendSizePerSecond_Client;      // ��ͻ���ÿ�뷢�͵�����ֽ���
        long lMaxRecvSizePerSecond_Client;      // �ӿͻ��˽��ܵ�ÿ������ֽ���
        long lMaxBlockedSendMsgNum_Client;      // ��������ķ�����Ϣ����
        long lConPendingWrBufNum_Client;        // �ͻ������ķ���IO����Buf�ܴ�С
        long lConPendingRdBufNum_Client;        // �ͻ������Ľ���IO����Buf�ܴ�С
        long lConMaxSendSizePerSecond_Client;   // ����������͵�ÿ������ֽ���
        long lConMaxRecvSizePerSecond_Client;   // �ӷ��������ܵ�ÿ������ֽ���
        long lConMaxBlockedSendMsgNum_Client;   // ��������ķ�����Ϣ����
        long lMaxBlockConNum_Client;
        long lValidDelayRecDataTime_Client;

        ulong dwRefeashInfoTime;    // ˢ����ʾʱ��
        ulong dwSaveInfoTime;       // ����һ����Ϣ��ʱ����
        ulong dwDoQueueInter;       // ����ȴ����еļ��ʱ��
        ulong dwSendMsgToQueInter;  // ���ȴ����з�����Ϣ�ļ��ʱ��

        ulong dwWorldMaxPlayers;    // ��½WorldServer���������
        float fWorldBusyScale;      // ��æ����������
        float fWorldFullScale;      // ��������������

        ulong dwPingWorldServerTime;        // ��ʱPingWorldServer�ļ��
        ulong dwPingWorldServerErrorTime;   // PingWorldServer���յ����صĳ���ʱ����

        bool bCheckForbidIP;    // �Ƿ������IP����
        bool bCheckAllowIP;     // �Ƿ���IP������
        bool bCheckBetweenIP;   // ���ip�Ƿ�������IP��

        ulong dwMinDbOptThreadNum;      //! ��СDB�����̱߳�����
        ulong dwMaxDbOptThreadNum;      //! ���DB�����߳���
        ulong dwThreadNumAdjustSpace;   //! �Զ������߳����ļ��ʱ��

        set<string> validGsIp;  //��Ч��game server ip�����ڴ��б��е�gs�޷�����cs��
    };

private:
    tagSetup m_Setup;
public:
    bool        LoadSetup();
    bool        ReLoadSetup();
    tagSetup*   GetSetup() {return &m_Setup;}
};

unsigned __stdcall GameThreadFunc(void* pArguments); // ��Ϸ���߳�