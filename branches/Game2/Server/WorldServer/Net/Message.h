/**
* @file    Message.h
* @author  Fox (yulefox@gmail.com)
* @date    Mar.04, 2010
*/

#pragma once

#include "NetHandler.h"

class CMapFile;
class CMySocket;
class CServer;

/**
* @class   CMessage
* @author  Fox (yulefox@gmail.com)
* @date    Mar.04, 2010
* @brief   BaseMessage����, ����WorldServer��Ϣ����
*/

class CMessage : public CBaseMessage
{
public:
	CMessage();
	CMessage(uint type);
	virtual ~CMessage();

public:
	/// ��ʼ��
	static bool Initial(CDataBlockAllocator* allocator, uint maxFreeMsgNum);

	/// �ͷ���Դ
	static bool Release();
	static CBaseMessage* NewMessage();
    static void SetServer(CServer* server) { s_server = server; }
    /// ��ȡ���ӵ� GS ����
    static long GetGSNum() { return (long)s_GSIDs.size(); }

    /// GS �Ƿ�������
    /// @param id 0, �����Ƿ���GS����; -1, �����Ƿ�����GS��������;
    /// other, ���ض�ӦGS�Ƿ�������. 
    static bool IsGSReady(uint id);

	virtual void SetSocketID(long socket) { m_socket = socket; }
	virtual long GetSocketID() { return m_socket; }
	virtual void SetIP(ulong IP) { m_IP = IP; }
	virtual ulong GetIP() { return m_IP;	}

    /// ����������Ϣ����
	virtual long DefaultRun();
	virtual long Run();

    /// ������Ϣ��ر���
	virtual void Reset();

    /// ���͸����пͻ���
	void SendToAllGS();

    /// ������Ϣ��LoginServer
    /// @param flag ��������ʶ
    /// @param prior �Ƿ����ȷ���
	void SendToServer(NetFlag flag, bool prior = false);

	uint GetGSID();

    /// ���͵�GS��һ��ӳ��ID
	void SendToGS(uint id, bool clone = true);

    /// ���͵�GS, LS, DBS��Socket
	void SendToSocket(int socket, bool clone = true);

	void SendToSM();
	uint GetRecvTime() { return m_recvTime; }
	void SetRecvTime(uint time)	{m_recvTime = time; }

public:
	/// ӳ��GSID��SocketID
	static void MapGSIDSocket(uint id, int socket);
	static int GetSocketByGSID(uint id);
	static uint GetGSIDBySocket(int socket);
	static void UnMapSocket(int socket, NetFlag flag);
	static void UnMapGSIDSocket(int socket);

    /// ���ڴ�Ӱ���ļ��Ľ���ͨ�Ż���,ʵ��д�ļ����첽
    /// Author:wangqiao
	/// ���ļ�
	static void AsyOpenFile(const char* file);

	/// �ر��ļ�
	static void AsyCloseFile(const char* file);

	/// ����ļ�����
	static void AsyClearFile(const char* file);

	/// �첽д�ļ��Ľӿ�
	static void AsyWriteFile(const char* file, const char* content);

private:

	int GetServerSocket(NetFlag flag);

	/// dbserver�����׽��ֺ�dbserverID����
	typedef stdext::hash_map<int, uint> Clients;
	typedef Clients::iterator ClientItr;

	static Clients s_GSs;
	static Clients s_GSIDs;

protected:
    /// ������Ϣ������SocketID
	int m_socket;

    /// ���͸���Ϣ��IP
	uint m_IP;

    /// �յ���ʱ��
	uint m_recvTime;

    /// ����ģ�������ʵ��
    static CServer* s_server;
};
