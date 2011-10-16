
#pragma once


//! ��־�ı���󳤶�
const DWORD MAX_ACTION_LOG_TEXT_SIZE	= 1024;
//! һ����־������
const DWORD MAX_LOG_GROUP_SIZE			= 16;

struct tagDbLog
{
	DWORD	dwTime;
	DWORD	dwSmID;
	DWORD	dwGappID;
	char	szUserName[MAX_USER_NAME_SZIE];
	char	szInfo[MAX_ACTION_LOG_TEXT_SIZE];

	tagDbLog(VOID){memset(this, 0, sizeof(tagDbLog));}
};


///////////////////////////////////////////////////////////
class ActionLogManage
{
public:
	ActionLogManage(VOID);
	~ActionLogManage(VOID);

public:
	//!
	static	ActionLogManage&		GetInstance(VOID) {return c_ActionLogManage;}
	//!								��Ӧд�����
	static		VOID				OnDBWriteFinish(tagDbLog *pDbLog, DWORD dwNum, BOOL bSucceed);

public:
	//!								��ʼ������
	BOOL							Init(VOID);
	//!								�ͷŶ���
	VOID							Release(VOID);

public:
	//!								��DBд��һ����־
	VOID							ActionLog(DWORD dwSmID, DWORD dwGappID, const char * pUserName, const char *pInfo, ...);

private:
	//!								���������ݿ�д������
	VOID							RequestWriteDB(tagDbLog *pDbLog, DWORD dwNum);

private:
	//!								��ǰ�������־
	tagDbLog						*m_pCurrtagDbLog;
	//!								��ǰ��ʹ�õĻ�����
	DWORD							m_dwCurrUseNum;

private:
	//!
	static	ActionLogManage			c_ActionLogManage;
	//!								����д�����ݿ������
	static	set<tagDbLog*>			m_setWriting;
};