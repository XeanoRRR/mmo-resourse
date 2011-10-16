
//! �򵥷�װ���Լ����͸��Լ�����Ϣ�ࣨAinMessage������Ϣ���У�AinMsgQueue��
//! ֻ����Ϣ���ͺ�����ָ��
//! ���ݵĲ���ʹ�ã�
//!		_AddToByteArray();
//!		_GetBufferFromByteArray();
//!	�Ⱥ���
//!

//#include "../../public/UseMemManageSCC.h"

#pragma once


//////////////////////////////////////////////////
class AinMessage
{
public:
	static AinMessage*	Create(LONG lType);
	static VOID			Release(AinMessage **ppAinMsg);

public:
	LONG				GetType(VOID){return m_lType;}
	VOID				Run(VOID);

public:
	VOID				AddByte(BYTE cData);
	VOID				AddWord(WORD wData);
	VOID				AddDword(DWORD dwData);
	VOID				AddStr(LPCSTR pStr);
	VOID				AddEx(LPCVOID pData, DWORD dwAddSize);

	BYTE				GetByte(VOID);
	WORD				GetWord(VOID);
	DWORD				GetDword(VOID);
	BOOL				GetStr(LPSTR pStr, DWORD lBufMaxSize);
	BOOL				GetEx(LPVOID pData, DWORD lGetSize);
	
private:
	LONG			m_lType;
	vector<BYTE>	m_vData;
	LONG			m_lPos;

private:
	AinMessage(LONG lType):m_lType(lType),m_lPos(0){}
	~AinMessage(VOID){}
	AinMessage(AinMessage&);
	AinMessage& operator=(AinMessage&);
};



class AinMsgQueue
{
public:
	AinMsgQueue(VOID);
	~AinMsgQueue(VOID);

public:
	//!				
	VOID			Release(VOID);
	//!				�õ���Ϣ��Ŀ
	LONG			GetSize(VOID);
	//!				ѹ����Ϣ
	BOOL			PushMessage(AinMessage* pMsg);
	//!				ѹ��һ����ʱ��Ϣ
	BOOL			PushTimeMessage(AinMessage* pMsg, DWORD dwMillisecond);
	//!				������Ϣ
	VOID			PopMessage(list<AinMessage*> &listMsg);
	//!				�����Ϣ�����ͷ��ڴ棩
	VOID			Clear(VOID);

private:
	list<AinMessage*>				m_msgQueue;
	
	LockOfWin32						m_Lock;

	map<DWORD, AinMessage*>			m_timeMsgQueue;
	DWORD							m_dwMinBeginTime;
};