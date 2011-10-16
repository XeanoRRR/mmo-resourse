///////////////////////////////////////////////////////////////////////////////
///  MailManager.h
///
///  E:\alienbrainWork\Game_Program_Develop\guangmang\SERVER\WorldServer\AppWorld\Mail
///
///  CREATED: 09/12/2008  by ���Ƚ�
///
///  PURPOSE:   �ʼ�������
///
//////////////////////////////////////////////////////////////////////
#ifndef _MAILSYSTEM_H_
#define _MAILSYSTEM_H_
#pragma once

#include "Mail.h"

typedef CTimerQueue<CGameEvent*> MailTimer;

class CMailSystem :public CGameEvent
{
public:
	struct stTimerParam
	{
		//��ʱ��ID
		long lTimerID;
		//Ԥ������ʱ��
		long lFuture;
		//���ô���
		long lCount;
		//�ѵ��ô���
		long lCalledCount;
		//��ʼʱ��
		DWORD nStartTime;			
	};
public:
	CMailSystem();
	~CMailSystem();
	void	SendMail( CMail *pMail,long lSysID=-1);										//ת���ʼ�
	void	SendMailToGS(CMail *pMail,CPlayer *pPlayer);								//���ʼ���������	
	CMail   *GetMail(const CGUID &guid);												//��ȡ�ʼ�����
	CMail	*GetTempMail(const CGUID &guid);											//��ȡ��ʱ�ʼ�
	bool	SaveMailToDB(CPlayer *pPlayer,CMail *pMail);								//�ʼ����浽DB
	bool	SaveMailToTempMail(CMail *pMail);											//�ʼ����浽��ʱ�ʼ�
	void	DeleteTempMail(CGUID &guid);												//ɾ����ʱ�ʼ�
	void	SendAllMailToGs(CPlayer *pPlayer);											//������������ʼ���GS
	void	SendDeleteMailToDB(const CGUID &PlayerGuid, list<CGUID> &lMailGuid);		//����ɾ���ʼ�to DB
	void    SendDeleteMailToGS(const CGUID &PlayerGuid, list<CGUID> &lMailGuid);		//����ɾ���ʼ�to GS
	void	GetGoodsFromMail(CGUID &guid,long lIndex);									//������ȡ��Ʒ
	void	SaveAndSendMailToReceiver(CMail *pMail);
	void	GetGoldFromMail(CGUID &guid);												//���ʼ���ȡ���
	void	ReadMail(CGUID &guid);														//�Ķ��ʼ�
	void	PayMail(CGUID &guid);														//��������
	void	SendPayMailMsgToGs(CPlayer *pPlayer, CGUID &MailGuid);						//��GS���ظ�������
	bool	OnChangeMailDest(CMail *pMail);												//ת���ʼ�	
	void	RejectMail(CPlayer *pPlayer, CGUID &guid);									//����
	bool	SaveMailToMailSys(CPlayer *pPlayer,CMail *pMail);							//�����ʼ����ʼ�ϵͳ
	bool	StartRegisterTimer(const CGUID &PlayerGuid, CMail *pMail);					//������ʱ��
	void	RegisterTimer(const CGUID &PlayerGuid,const  CGUID &MailGuid,long lFuture);	//ע�ᶨʱ��					
	void	UnRegisterTimer(long lTimerID);												//ע����ʱ��
	bool	RegisterSysMailTimer(DWORD dwID,CMail *pMail);								//ע��ϵͳ�ʼ���ʱ��
	void	UnRegisterSysMailTimer(long lTimerID);										//ע��ϵͳ�ʼ���ʱ��
	void	RegisterCheckTime(const CGUID &PlayerGuid,long lFuture);					//ע���ʼ���鶨ʱ��
	void	UnRegisterCheckTime(const CGUID &PlayerGuid);								//ע���ʼ���鶨ʱ��
	void	SendMailMsgToGS(const CGUID &MailGuid,int nRet);							//��GS���ؼ��Ž��
	void	SuccessSaveMail(CGUID &MailGuid);											//�ʼ�����ɹ�
	void	FailedSendMail(CPlayer *pPlayer,long lValue);								//�ʼ�����ʧ��
	void	UnReisterTimerOnReject(const CGUID &MailGuid);								//���ź�ע����ʱ��
	void	DeleteMailOnPlayerExit(CPlayer *pPlayer);									//������ߺ�ɾ��ws�ϵ��ʼ�
	void	FindNoReceiver(CGUID &MailGuid);											//û���ҵ�������
	void	FailedSaveMail(CGUID &MailGuid);											//�ʼ�����ʧ��
	void	SendSaveMailRet(CGUID &MailGuid,long lRet);									//���ر����ʼ����
	void	Run();
	long	Schedule(	CGameEvent* const pEvent,const void *arg,unsigned long futuretime,unsigned long interval = 0);
	int		Cancel(CGameEvent* const pEvent);
	int		Cancel(long timerid, const void** arg = 0);
	

	//������Ϣ��Ӧ
	virtual void OnMessage(CMessage*);
	//��ʱ��Ӧ
	virtual long OnTimeOut(DWORD timerid,DWORD curtime,const void* var);
	//����ʱ����ȡ���ĵ���
	virtual void OnTimerCancel(DWORD timerid,const void* var);
	//����ʵ�����Ϣ����
	virtual long Proc(eEventType type,const tagProcVar *pVar);
	static CMailSystem *GetInstance();


	void	InitSysMail();															//��ʼ��ϵͳ�ʼ�
	void	InitMailParam(tagMailParam *pParam,tagMailParam *pPamramList);			//��ʼ���ʼ�����
	void	AddSysMail(CMail *pMail,DWORD dwID);									//���ϵͳ�ʼ�
	CMail	*GetSysMail(long lID);													//ͨ��id�õ�ϵͳ�ʼ�
	void	ClearSysMail();															//���ϵͳ�ʼ�
	void	ClearSysMail(CMail *pMail);												//���ϵͳ�ʼ�
	void	SendSysMail(long lSysID);												//����ϵͳ�ʼ�	
	void	InitSysMailParam(tagMailParam *pParam,tagMailParam *pPamramList);		//��ʼ��ϵͳ�ʼ�����
	void	AddPlayerMail(CGUID &PlayetGuid,list<CMail*> &lMail);					//�����DB��ʼ����ȡ���ʼ�
	void	ReleasePlayerMail(CPlayer* pPlayer);									//����뿪�ͷ��ʼ�
	void	OutMailInfo(VOID);

private:
	map<CGUID,CMail*>	m_mapMail;
	map<CGUID,CMail*>   m_mapTempMail;												
	map<DWORD,CMail*>   m_mapSysMail;
	map<CGUID,list<CMail*>>	m_mapPlayerMail;
	map<DWORD,stTimerParam*> m_mapTimerParam;
	map<long, tagTimerVar*>	m_mapTimer;
	map<CGUID,long>		m_mapCheckMailTimer;
	static MailTimer	*m_pMailTimer;	
	static CMailSystem	*instance;
};
#endif//_MAILSYSTEM_H_