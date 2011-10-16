
//! �򵥷�װ���Լ����͸��Լ�����Ϣ�ࣨAinMessage������Ϣ���У�AinMsgQueue��
//! ֻ����Ϣ���ͺ�����ָ��
//! ���ݵĲ���ʹ�ã�
//!		_AddToByteArray();
//!		_GetBufferFromByteArray();
//!	�Ⱥ���
//!

#include "..\public\UseMemManageLS.h"

#pragma once

enum eAinMsgType
{
	AIN_MSG_ACCOUNT_Validate_Re,
	AIN_MSG_SDOA_VALIDARE_RETURN,	//! ʢ����֤�������
	AIN_MSG_SDOA_FCM_NOTIFY,		//! ʢ�������֪ͨ
	AIN_MSG_ACCOUNT_Business_Re,    //! ʢ�����ϲ�ѯ����
	AIN_MSG_SDOA_Business_RETURN,	//! ʢ����֤�������
	AIN_MSG_SDOA_Business_Validate, //! ʢ����֤����
	AIN_MSG_SDOA_Business_Re_Error, //! ʢ����֤���������ش���
};

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
	BOOL				GetStr(LPSTR pStr, WORD lBufMaxSize);
	BOOL				GetEx(LPVOID pData, DWORD lGetSize);

private:
	LONG			m_lType;
	vector<BYTE>	m_vData;
	DWORD			m_dwPos;

private:
	AinMessage(LONG lType):m_lType(lType),m_dwPos(0){}
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

	AsyncLock						m_Lock;

	map<DWORD, AinMessage*>			m_timeMsgQueue;
	DWORD							m_dwMinBeginTime;
};