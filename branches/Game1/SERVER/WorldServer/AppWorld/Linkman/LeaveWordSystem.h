
/*
*	file:		LeaveWordSystem.h
*	Brief:		����ϵͳ
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-03-06
*/

#include "../../public/LeaveWordDef.h"

#pragma once


class LeaveWordSystem
{
public:
	LeaveWordSystem(void);
	~LeaveWordSystem(void);

public:
	//!								���Ψһʵ��
	static		LeaveWordSystem&	GetInstance					(void);

public:
	//!								��ʼ��
	BOOL							Init						(const char *pSetupFile);
	//!								�������õ�DBS
	void							SendSetupToDBS				(void);
	//!								�ͷ�
	void							Release						(void);

	//!								�������
	BOOL							LeaveWord					(const CGUID &SenderGuid, const char *pReceiverName, char *pWord);

	//!								��Ӧ�������
	void							OnPlayerLogin				(CPlayer *pReceiver);

	//!								��Ӧ�������ȡ��
	void							OnLeaveWordPop				(CMessage *pMsg);
	//!								��Ӧ���Ա�����
	void							OnLeaveWordResult			(CMessage *pMsg);

public:
	//!								��ʱ����ʱ��Ӧ
	long							OnTimeOut					(DWORD timerid,DWORD curtime,const void* var);
	//!								����ʱ����ȡ���ĵ���
	void							OnTimerCancel				(DWORD timerid,const void* var);
	//!								��ʱ����ⴥ��
	void							TimerExpire					(LONG lCurrTime);

private:
	//!								��ȡ����
	BOOL							LoadSetup					(const char *pSetupFile);
	//!								����������ʱ��
	BOOL							StartupTimer				(void);

	//!								֪ͨ������ķ���ʧ��
	void							RePlayerSendFailed			(CPlayer *pSender, const char *pReceiverName);
	//!								���һ�������ID
	LONG							GetBufID					(void);

	//!								����WSDB�ṹ
	tagLeaveWord_W2DB*				CreateLeaveWord_W2DB		(void);
	//!								�ͷ�WSDB�ṹ
	void							ReleaseWord_W2DB			(tagLeaveWord_W2DB* pLeaveWord_W2DB);

private:
	//!								��ʱ��
	CTimerQueue<LeaveWordSystem*>	*m_pTimerQueue;
	//!								������Ϣ
	tagLeaveWordSetup				m_LeaveWordSetup;

	//!								�ȴ���ʼ�������
	set<CGUID>						m_WaitPlayerGuid;
	//!								������Դ�����¼
	map<CGUID, LONG>				m_PlayerLvwdCount;

	//!								д������ʱ�Ļ���
	map<LONG, tagLeaveWord_W2DB*>	m_LeaveWriteBuf;
	//!								����ID������
	LONG							m_BufIdSeed;
};