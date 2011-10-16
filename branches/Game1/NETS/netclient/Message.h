#pragma once
#include "..\basemessage.h"
#include "..\..\public\MsgType.h"

class CMySocket;
class CClient;
class CRegion;
class CPlayer;
//class CMapFile;
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
public:

	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static bool Release();

	static void SetClient(CClient *pClient)	{m_pNetClient=pClient;}

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