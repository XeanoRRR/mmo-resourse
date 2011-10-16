/**
 * @file    NetHandler.h
 * @author  Fox (yulefox@gmail.com)
 * @date    Mar.02, 2010
 * @brief   ��ԭGame���������ģ���������.
 */

#pragma once

#include "../../../Public/Common/NetDef.h"
#include "../../../Public/DefType.h"

class CMessage;

/// WorldServer���ӵķ�������Ϣ
struct ServerConnection
{
    Connection con;                ///< ���ӵĻ�����Ϣ
    HANDLE handle;                 ///< ���Ӿ��
    bool ready;

    ServerConnection()
        : handle(NULL)
        , ready(false)
    {}
};

/// ����WorldServer��GameServer��Ϣ
struct GSConnection
{
    Connection con;                ///< ���ӵĻ�����Ϣ
    uint index;                    ///< GameServer���
    uint receivedPlayerData;       ///< ���յ��������������(��ʱʹ��)
    uint playerNum;                ///< �������

    GSConnection()
        : receivedPlayerData(0)
        , playerNum(0)
    {}
};

typedef std::map<NetFlag, ServerConnection> Servers;
typedef std::map<uint, GSConnection> Clients;
typedef Servers::iterator ServerItr;
typedef Clients::iterator ClientItr;

/// �����ӷ��������� [Setup.ini], [GSSetup.ini] �� [LogSetup.ini]
struct ServerSetup
{
    uint serverNum;                 ///< ���������
    string serverName;              ///< ����������
    Servers servers;                ///< �����������б�
    ServerConnection ws;            ///< WorldServer����
    Clients clients;                ///< GS�ͻ��������б�
    uint useLogServer;              ///< �Ƿ�ʹ��LogServer��0=��ʹ�ã�1=ʹ��
    string errFile;                 ///< �����ļ�·��+�ļ���
    string logFile;                 ///< ������־�ļ�·��+�ļ�����SQL��ʽ��
    string txtLogFile;              ///< ������־�ļ�·��+�ļ��������ĸ�ʽ��
    uint clientNo;                  ///< ��־�ͻ��˱��
};

/// ���������������: Client����������ò��� [SetupEx.ini]
struct NetHandlerSetup
{
    uint dataBlockNum;             ///< ���ݿ�����
    uint dataBlockSize;            ///< ���ݿ��С
    uint freeMsgNum;               ///< Ԥ�������Ϣ����
    uint freeSockOperNum;          ///< �����������Ԥ��������
    uint freeIOOperNum;            ///< ��ɶ˿���IO����Ԥ��������
    uint IOOperDataBufNum;         ///< Ĭ��IO������DataBuf����
    uint checkNet;                 ///< �Ƿ��������м��
    uint banIPTime;                ///< ��ֹIP��ʱ��
    uint maxMsgLen;                ///< ������������Ϣ����		
    uint maxConnectNum;            ///< �ͻ��˵������������
    uint maxClientsNum;            ///< ���Ŀͻ��˷��ͻ�������С
    uint pendingWrBufNum;          ///< ���ķ���IO����Buf�ܴ�С
    uint pendingRdBufNum;          ///< ���Ľ���IO����Buf�ܴ�С
    uint maxSendSizePerSecond;     ///< ��ͻ���ÿ�뷢�͵�����ֽ���
    uint maxRecvSizePerSecond;     ///< �ӿͻ��˽��ܵ�ÿ������ֽ���
    uint maxBlockedSendMsgNum;     ///< ��������ķ�����Ϣ����
    uint conPendingWrBufNum;       ///< �ͻ������ķ���IO����Buf�ܴ�С
    uint conPendingRdBufNum;       ///< �ͻ������Ľ���IO����Buf�ܴ�С
    uint conMaxSendSizePerSecond;  ///< ����������͵�ÿ������ֽ���
    uint conMaxRecvSizePerSecond;  ///< �ӷ��������ܵ�ÿ������ֽ���
    uint conMaxBlockedSendMsgNum;  ///< ��������ķ�����Ϣ����
    uint refreshInfoTime;          ///< ˢ����ʾʱ��
    uint saveInfoTime;             ///< ����һ����Ϣ��ʱ����
    uint releaseLoginPlayerTime;   ///< �ͷ�LoginList���Player�ĳ�ʱʱ��
    uint clearPlayerDataTime;
    uint gappID;                   ///< GAPPID
};

class NetHandler
{
public:
    NetHandler() {}
    ~NetHandler() {}

public:
    /// ��ʼ�����紦��ģ��
    bool Init();

    /// ж�����紦��ģ��
    bool Fini();

    /// ���ط����������ļ�
    bool LoadSetup();

    bool LoadMapFileSetup();

    /// ��Ϊ�ͻ������ӵ�ָ����ʶ�ķ�����
    /// @param flag ��������ʶ
    /// @param first Ϊ��, ��Ҫ���������߳�; Ϊ��, �����������߳�.
    bool ConnectServer(NetFlag flag, bool first = true);

    /// ��Ϣ����
    void ProcessMessage();

    void ServerReady(NetFlag flag, bool ready = true);
    void GSReady(uint id, bool ready = true);
    /// �жϷ������Ƿ���������
    /// @param flag ��������ʶ
    bool IsServerReady(NetFlag flag);

	CServer* GetServer() { return m_server; }

    /// ��ȡ���õ� GS ����
    size_t GetGSNum();

    /// ������Server���ص�ǰ����GS���������
    /// @param flag ��������ʶ, Ĭ��ΪNF_LOGIN_SERVER
    void PlayerNumToServer(NetFlag flag = NF_LOGIN_SERVER);

    /// ��ȡ������������
    const NetHandlerSetup& GetSetup() { return m_netHandlerSetup; }

    /// ��ȡ����������
    const ServerSetup& GetServerSetup() { return m_serverSetup; }

    /// ����IP, port��ȡ��ӦGS��Ϣ
    GSConnection* GetGS(const string& ip, uint port);

    /// ����ID��ȡ��ӦGS��Ϣ
    GSConnection* GetGS(uint id);

    /// ���������������߳�
    /// @param flag ��������ʶ
    void CreateConnectThread(NetFlag flag);

    void PutoutInfo();

    ///  ���õ�ǰ��������ʼ����־
    void SetServerInitSendFlag(NetFlag flag);

    ///  ����Ws��Ls��Db׼�������Ϣ
    void ProcessServerInitSendMsg();

private:
    friend class CMessage;
    friend unsigned __stdcall ConnectServerFunc(void* arg);

    /// ��ʼ�����绷����DBAllocator, Msg, Socket
    void InitNetEnv();

    /// ��ʼ����Ϸ����������ˣ���������������ˣ�װ������
    bool InitNetServer();

    /// ��ȡ����������
    const char* GetServerName(NetFlag flag);

    /// ���ҷ���������ʵ��
    ServerConnection* FindServer(NetFlag flag);

    /// ������[LoginServer], [DBServer], [LogServer]�Ͽ�����
    /// @param flag �����ʶ
    /// @param reconnect �Ƿ�����, Ĭ��Ϊtrue.
    void OnServerLost(NetFlag flag, bool reConnect = true);

    /// �ͻ���[GameServer]�Ͽ�����
    void OnClientLost(uint id);

    CDataBlockAllocator* m_DBAllocator;

    /// ��ΪGameServer�ķ�������
	CServer* m_server;
    NetHandlerSetup m_netHandlerSetup;
    ServerSetup m_serverSetup;
    NetFlag         m_ServerInitSendFlag;
};

/// WorldServer���ӵ�����Server���߳�, ��WorldServer�Ͽ�ʱ����, ���Ӻ�ر�
/// @param server Server��ʶ
unsigned __stdcall ConnectServerFunc(void* arg);

/// ��ȡ NetHandler ����ʵ��
#define GetNetHandler() GetInst(NetHandler)
