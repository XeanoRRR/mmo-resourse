//=============================================================================
/**
 *  file: basemessage.h
 *
 *  Brief:��Ϸ����Ϣ���ӱ�������
 *		����ÿ����Ϣ�Ļ������ݣ���С�����ͺ�ʱ�䣩
 *		�ṩ��������װ��Ϣ��������Ϣ����
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2005-4-15
 *
 *	Modify:2007-4-13,�����˴���ͽṹ���Ż���Ч��
 */
//=============================================================================

#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MsgQueue.h"
#include "..\public\DataBlock.h"
#include "..\public\DataBlockAllocator.h"
#include "..\public\DataBlockSetWriteRead.h"

class CMySocket;
class CMessage;
class CGUID;
struct tagDataBlockWriteSet;
struct tagDataBlockReadSet;

//������Ϣ��������ȼ�
const int MAX_MSG_PRIO = 100;
const int HEAD_SIZE = 4;

enum eBaseMsgType
{
	BASEMSG_Socket_Accept = 1,
	BASEMSG_Socket_Close,
	BASEMSG_Transfer_Congestion,//����ӵ��
	BASEMSG_Transfer_Smoothly,//���䳩ͨ
};

class CBaseMessage
{
public:

	CBaseMessage();
	virtual ~CBaseMessage();

	struct stMsg
	{
		unsigned long lSize;		// ��Ϣ��С
		unsigned long eType;		// ��Ϣ����
		unsigned long lVerifyCode;	// У����
	};

	////////////////////////////////////////////////////////////////////////
	//	��������
	////////////////////////////////////////////////////////////////////////
private:

	//��Ϣ�����ݲ���
	typedef vector<CDataBlock*>::iterator itMsgData;
	vector<CDataBlock*> m_MsgData;
	
	//�����ݴ�С
	
	tagDBOpParam m_ReadParam;
	////��ǰ�����ݿ���
	//long	m_nCurRdNum;
	////��ǰ�����ݿ�Ĵ�С
	//long	m_nCurRdDBSize;
	////��ǰ�����ݵ�λ��
	//long	m_nCurRdPos;
	////��ǰ�����ݿ��ָ��
	//BYTE	*m_pRdDBPtr;

	tagDBOpParam m_WriteParam;
	////��ǰд���ݿ���
	//long	m_nCurWrNum;
	////��ǰд���ݿ�Ĵ�С
	//long	m_nCurWrDBSize;
	////��ǰд���ݵ�λ��
	//long	m_nCurWrPos;
	////��ǰд���ݿ��ָ��
	//BYTE	*m_pWrDBPtr;

	long	m_lNetFlag;
	//����Ϣ�����ȼ���
	long	m_lPriorityLvl;
	//��ʼ���͸���Ϣ���¼�
	DWORD	m_dwStartSendTime;
	//��Ϣ�����ü���
	long m_lRefCount;

protected:
	BYTE* Base()
	{
		if(m_MsgData.size() > 0)
			return m_MsgData[0]->Base();
		return NULL;
	}

	BYTE* GetMsgBuf()
	{
		return m_MsgData[0]->Base()+HEAD_SIZE;
	}
	
	void SetSize(unsigned long l)	{((stMsg*)GetMsgBuf())->lSize = l;}

	void Add(BYTE*, long size);
	void* Get(BYTE*, long size);
public:
	void Init(unsigned long type);
	void Init(vector<CDataBlock*>& MsgData, const unsigned char kn[16][6], BOOL bDecrypt);
	void UnInit();
	//��֤��Ϣ�����Ƿ�Ϸ�
	bool Validate();

	void Clone(CBaseMessage *pMsg);

	void AddWrDataBlock();
	void AddRdDataBlock();

	void SetType(unsigned long t)
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)) )
			return;	
		((stMsg*)GetMsgBuf())->eType = t;
	}

	//void SetTime(unsigned long l)
	//{
	//	if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
	//		return;	
	//	((stMsg*)GetMsgBuf())->lTime = l;
	//}
	void SetCode(unsigned long l)
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
			return;	
		((stMsg*)GetMsgBuf())->lVerifyCode = l;
	}
	unsigned long GetSize()
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
			return 0;	
		return ((stMsg*)GetMsgBuf())->lSize;
	}
	unsigned long GetType()
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
			return 0;	
		return ((stMsg*)GetMsgBuf())->eType;
	}
	/*unsigned long GetMsgSendTime()
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
			return 0;	
		return ((stMsg*)GetMsgBuf())->lTime;
	}*/
	unsigned long GetCode()
	{
		if(m_MsgData.size()==0 || m_MsgData[0]->GetMaxSize() < (HEAD_SIZE+sizeof(stMsg)))
			return 0;		
		return ((stMsg*)GetMsgBuf())->lVerifyCode;
	}

	//�õ��ܴ�С(����ʵ����Ϣ��С����Ϣǰ�ĸ���ͷ)
	unsigned long GetTotalSize(void);/*
	{
		return m_lMsgTotalSize ? m_lMsgTotalSize : (m_lMsgTotalSize = GetSize() + HEAD_SIZE);
	}*/
	void SetTotalSize() { *((long*)Base()) = GetSize()+HEAD_SIZE; }

	vector<CDataBlock*>& GetMsgData() {return m_MsgData;}
	void SetMsgData(vector<CDataBlock*>& MsgData)	{ UnInit(); m_MsgData=MsgData; }
	void ClearMsgData()		{m_MsgData.clear();}

	void SetRefCount(long lNum)	{ m_lRefCount = lNum; }
	void AddRefCount(long lNum)	{ m_lRefCount += lNum; }
	long RemoveRefCount()		{ return --m_lRefCount; }
	void Encrypt(const unsigned char kn[16][6]);	/// ����Ϣ���м���
	void Decrypt(const unsigned char kn[16][6]);	/// ����Ϣ���н���

	////////////////////////////////////////////////////////////////////////
	//	��Ӻͻ�ȡ����
	////////////////////////////////////////////////////////////////////////
public:
	void Add(char);
	void Add(BYTE);
	void Add(short);
	void Add(WORD);
	void Add(long);
	void Add(LONG64);
	void Add(DWORD);
	void Add(float);
	void Add(const char*);
	void Add(const CGUID&	guid);
	void AddEx(void*, long size);//���Ӱ�ȫ�ԣ���GetEx���ʹ��
	bool GetDBWriteSet(tagDataBlockWriteSet& DBWriteSet);

	virtual void Reset(void);           //�����Ϣ����

	
	char GetChar();
	BYTE GetByte();
	short GetShort();
	WORD GetWord();
	long GetLong();
	LONG64 GetLONG64();
	DWORD GetDWord();
	float GetFloat();
	bool  GetGUID(CGUID& guid);
	void* GetEx(void*, long size);
	char* GetStr( char* pStr,long lMaxLen );
	bool GetDBReadSet(tagDataBlockReadSet& DBReadSet);
	
protected:
	bool m_bEncrypted;						/// ��Ϣ������ܻ��Ѽ���
	unsigned long m_lMsgTotalSize;			/// ��ֹ�����ƻ�
	typedef list<CBaseMessage*>	listBaseMsgs;
	typedef list<CBaseMessage*>::iterator itBaseMsg;
	static long m_nMaxFreeMsgNum;
	static listBaseMsgs m_FreeBaseMessages;
	static CRITICAL_SECTION m_CSFreeMsg;

	//������Ϣ��������ȼ�����
	typedef map<long,short>	mapMsgPrioLvlParams;
	typedef map<long,short>::iterator itMsgPrio;
	//���沿����Ϣ�����ȼ�����
	static mapMsgPrioLvlParams m_MsgPrioLvl;

	typedef set<long>	setDiscardMsgs;
	typedef set<long>::iterator itDiscaMsg;
	//������Զ�������Ϣ����
	static setDiscardMsgs m_DiscardMsgs;
public:
	virtual void SetSocketID(long lSocketID) = 0;
	virtual long GetSocketID()=0;

	virtual void SetIP(DWORD dwIP)=0;
	virtual DWORD GetIP()=0;

	virtual void SetIP(const char* pszIP){};

	void SetNetFlag(long lFlag)	{m_lNetFlag = lFlag;}
	long GetNetFlag()			{return m_lNetFlag;}

	//������Ϣ�����ȼ�����
	void SetPriorityLvl(long lPrioLvl)	{m_lPriorityLvl=lPrioLvl;}
	void SetStartSendTime(DWORD dwSendTime) {m_dwStartSendTime=dwSendTime;}
	//�õ���������ȼ���ֵ
	long GetPriorityValue(DWORD dwCurTime);
	//�ж�������Ϣ�Ķ�������Ƿ�һ��
	virtual bool IsDiscardFlagEqual(CBaseMessage* pMsg) {return false;}

	static CDataBlockAllocator*	m_pDBAllocator;
	static set<CBaseMessage*>	TestMsg;

	static bool Initial(CDataBlockAllocator* pDBAllocator,long nMaxFreeMsgNum);
	static bool Release();

	static void RegisterMsgPrio(long lMsgType,short nPrio);
	static short GetMsgPrio(long lMsgType);

	static void RegisterDiscardMsg(long lMsgType);
	//�õ�����Ϣ�����Ƿ���Զ���
	static bool GetIsDiscard(long lMsgType);

	//���������Ϣ�ĺ���
	typedef CBaseMessage*(*NEWMSG)();
	static NEWMSG NewMsg; 

	static CBaseMessage* __stdcall AllocMsg();
	static void __stdcall FreeMsg(CBaseMessage* pMsg);

	////////////////////////////////////////////////////////////////////////
	//	��Ϣ����
	////////////////////////////////////////////////////////////////////////
	virtual long DefaultRun() = 0;     //ȱʡ����Ϣ��������
	virtual long Run() = 0;            //���麯������Ϣ���к�����

	friend class CMsgQueue;
};

#endif
