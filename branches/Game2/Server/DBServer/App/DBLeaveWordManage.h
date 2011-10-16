

/*
*	file:		DBLeaveWordManage.h
*	Brief:		����ϵͳ���ݿ��������
*	detail:		
*				
*				
*	Authtor:	�ſ���
*	Datae:		2008-03-07
*/

#pragma once

#include "..\..\..\dbaccess\worlddb\RsLeaveWord.h"
#include "..\public\TimerQueue.h"

class DBLeaveWordManage
{
public:
	DBLeaveWordManage(void);
	~DBLeaveWordManage(void);

	static	DBLeaveWordManage&	GetInstance(void);

public:
	//!				��Ӧ������Ϣ
	void			OnSetupMessage			(CMessage *pMsg);
	//!				��Ӧ������Ϣ
	void			OnSaveMessage			(CMessage *pMsg);
	//!				��Ӧȡ��������Ϣ
	void			OnPopMessage			(CMessage *pMsg);
	//!				��Ӧ�����������
	void			OnClearOuttimeMessage	(CMessage *pMsg);

private:



private:
	//!										���������ʱ��
	LONG									m_lMaxSaveTime;
	

};