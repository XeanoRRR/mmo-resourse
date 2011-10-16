#pragma once
#include "../../Public/Nets/BaseMessage.h"
#include "../../Public/Nets/Clients.h"
class CMySocket;
class CClient;
class CRegion;
class CPlayer;
class CMemClient;

extern	enum OPERATION_TYPE
{
	OT_ROLL_BACK		= 0,
	OT_MOVE_OBJECT,
	OT_NEW_OBJECT,
	OT_DELETE_OBJECT,
	OT_SWITCH_OBJECT,
	OT_SWITCH_FILL,
	OT_CHANGE_AMOUNT,
};

class CMessage :
	public CBaseMessage
{
public:
	CMessage(void);
	CMessage(long lType);
	virtual ~CMessage(void);

private:
	static CClient	*m_pNetClient;
    static CClient* s_pNetClient_ChatServer;
public:

	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static bool Release();

	static void SetClient(CClient *pClient)	{m_pNetClient=pClient;}

    static void SetClient4Cs(CClient* p) {s_pNetClient_ChatServer = p;}
    static CClient* GetClient4Cs() {return s_pNetClient_ChatServer;}
    static void ResetClient4Cs()
    {
        if (s_pNetClient_ChatServer)
        {
            s_pNetClient_ChatServer->Exit();
            s_pNetClient_ChatServer->Close();
            delete s_pNetClient_ChatServer;
            s_pNetClient_ChatServer = NULL;
        }
    }

	static CBaseMessage* NewMessage();

////////////////////////////////////////////////////////////////////////
//	Player & Region
////////////////////////////////////////////////////////////////////////
private:
	CRegion* m_pRegion;
	CPlayer* m_pPlayer;

public:
	CRegion* GetRegion()	{ return m_pRegion; }
	CPlayer* GetPlayer()	{ return m_pPlayer; }


	void Reset(void);        //������Ϣ��ر���
////////////////////////////////////////////////////////////////////////
//	��Ϣ����
////////////////////////////////////////////////////////////////////////
	virtual void SetSocketID(long lSocketID){}
	virtual long GetSocketID(){return 0;}

	virtual void SetIP(DWORD dwIP){}
	virtual DWORD GetIP(){return 0;}

	long Send(CClient* p=NULL);		// ������Ϣ
    bool SendToChatServer();        // ������Ϣ�����������

	long DefaultRun();   //ȱʡ����Ϣ��������
	virtual long Run();					// ��Ϣ����

//���ڴ�Ӱ���ļ��Ľ���ͨ�Ż���,ʵ��д�ļ����첽
//Author:wangqiao
private:
	//Ӱ���ļ�ʵ��
	//static CMapFile *m_pMapFile;
	static CMemClient *m_pMemClient;
public:
	static void SetMemClient(CMemClient *pMemClient)	{m_pMemClient = pMemClient;}
	//���ļ�
	static void AsyOpenFile(const char* pszFileName);
	//�ر��ļ�
	static void AsyCloseFile(const char* pszFileName);
	//����ļ�����
	static void AsyClearFile(const char* pszFileName);
	//�첽д�ļ��Ľӿ�
	static void AsyWriteFile(const char* pszFileName,const char* pszContent);
	// ����Գ�������Ϣ
	long SendMemToTest(CMemClient *pMemClient = NULL);
	// ���Գ����͵���Ϣ����
	long TestRun();
};