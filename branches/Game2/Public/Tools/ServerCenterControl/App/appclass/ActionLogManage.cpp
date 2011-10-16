


#include "stdafx.h"
#include "ActionLogManage.h"

#include "DbOperator.h"


ActionLogManage ActionLogManage::c_ActionLogManage;
set<tagDbLog*>	ActionLogManage::m_setWriting;

ActionLogManage::ActionLogManage(VOID)
:m_pCurrtagDbLog(NULL)
,m_dwCurrUseNum(0)
{

}
ActionLogManage::~ActionLogManage(VOID)
{
	Release();
}

//! ��ʼ������
BOOL ActionLogManage::Init(VOID)
{
	return TRUE;
}
//! �ͷŶ���
VOID ActionLogManage::Release(VOID)
{
	if(NULL != m_pCurrtagDbLog)
	{
		RequestWriteDB(m_pCurrtagDbLog, m_dwCurrUseNum);
		m_pCurrtagDbLog = NULL;
		m_dwCurrUseNum = 0;
	}
}


//! ��DBд��һ����־
VOID ActionLogManage::ActionLog(DWORD dwSmID, DWORD dwGappID, const char * pUserName, const char *pInfo, ...)
{

	if(NULL == pInfo || NULL == pUserName)
		return;

	if(NULL == m_pCurrtagDbLog)
	{
		m_dwCurrUseNum = 0;
		m_pCurrtagDbLog = (tagDbLog*)M_ALLOC(sizeof(tagDbLog) * MAX_LOG_GROUP_SIZE);
		if(NULL == m_pCurrtagDbLog)
			return;
	}

	if(MAX_LOG_GROUP_SIZE <= m_dwCurrUseNum)
	{
		RequestWriteDB(m_pCurrtagDbLog, m_dwCurrUseNum);
		m_dwCurrUseNum = 0;
		m_pCurrtagDbLog = (tagDbLog*)M_ALLOC(sizeof(tagDbLog) * MAX_LOG_GROUP_SIZE);
		if(NULL == m_pCurrtagDbLog)
			return;
	}

	m_pCurrtagDbLog[m_dwCurrUseNum].dwSmID		= dwSmID;
	m_pCurrtagDbLog[m_dwCurrUseNum].dwGappID	= dwGappID;
	strcpy(m_pCurrtagDbLog[m_dwCurrUseNum].szUserName, pUserName);

	va_list va;
	va_start(va, pInfo);
	vsprintf(m_pCurrtagDbLog[m_dwCurrUseNum].szInfo, pInfo, va);
	va_end(va);

	m_dwCurrUseNum++;

}


//! ���������ݿ�д������
VOID ActionLogManage::RequestWriteDB(tagDbLog *pDbLog, DWORD dwNum)
{
	if(NULL == pDbLog || 0 == dwNum)
	{
		return;
	}

	DbOperator *pDbOpt = DbOperator::CreateIDBLogOperator(pDbLog, dwNum);
	if(NULL != pDbOpt)
	{
		m_setWriting.insert(pDbLog);
		pDbOpt->Begin();
	}
	else
	{
		M_FREE(pDbLog, sizeof(tagDbLog) * MAX_LOG_GROUP_SIZE);
	}
}

//! ��Ӧд�����
VOID ActionLogManage::OnDBWriteFinish(tagDbLog *pDbLog, DWORD dwNum, BOOL bSucceed)
{
	if(0 == dwNum)
		return;
	if(!bSucceed)
	{
		//! ������ֵ˵�����ݿ����ʧ����
		LogTools::OutErrLog("[%d] log write db lose", dwNum);
	}

	set<tagDbLog*>::iterator ite = m_setWriting.find(pDbLog);
	if(m_setWriting.end() != ite)
	{
		m_setWriting.erase(ite);
		M_FREE(pDbLog, sizeof(tagDbLog) * MAX_LOG_GROUP_SIZE);
	}
	else
	{
		LogTools::OutErrLog("ActionLogManage::OnDBWriteFinish find error!!!may be memory leak.");
	}
}