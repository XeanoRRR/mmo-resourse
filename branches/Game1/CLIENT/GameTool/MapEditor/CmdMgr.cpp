#include "StdAfx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "cmdmgr.h"
#include "cmd.h"

IMPLEMENT_SINGLETON(CmdMgr);

CmdMgr::CmdMgr(void)
{
	m_pCurCmd = NULL;
}

CmdMgr::~CmdMgr(void)
{
	Clear();
}

void CmdMgr::Do(Cmd * pCmd)
{
	//�ж��Ƿ񳬳��˳��� (δʵ��)
	//MAX_COMMAND_SAVED

	//����pCmd,
	//����pCmd��Ϊ��ǰ����
	//���뵽������
	pCmd->Execute();

	pCmd->SetPrev(m_pCurCmd);

	Cmd *pPreCmd = m_pCurCmd;
	Cmd *pNextCmd = (pPreCmd)? pPreCmd->GetNext() : NULL;

	pCmd->SetPrev(pPreCmd);
	pCmd->SetNext(pNextCmd);

	if (pPreCmd)
	{
		pPreCmd->SetNext(pCmd);
	}

	if (pNextCmd)
	{
		pNextCmd->SetPrev(pCmd);
	}

	m_pCurCmd = pCmd;
}

void CmdMgr::UnDo()
{
	//Un���е�ǰ
	if (m_pCurCmd)
	{
		if (m_pCurCmd->GetExecuted())
		{
			m_pCurCmd->UnExecute();
		
			//�˻���һ��
			Cmd *pCmd = m_pCurCmd->GetPrev();
			if (pCmd)
			{
				m_pCurCmd = pCmd;
			}
		}
	}
	
}

void CmdMgr::ReDo()
{
    if (m_pCurCmd)
	{	
		if (!m_pCurCmd->GetExecuted())
		{
			m_pCurCmd->Execute();
		}
		else
		{
			//ǰ������һ��
			Cmd *pNextCmd = m_pCurCmd->GetNext();

			if (pNextCmd)
			{
				//��һ����������
				if (!pNextCmd->GetExecuted())
				{
					pNextCmd->Execute();
					m_pCurCmd = m_pCurCmd->GetNext();
				}
			}
		}
	}
}

void CmdMgr::Dump()
{
	if (m_pCurCmd)
	{
		OutputConsole("cur Cmd: ");
		m_pCurCmd->Dump();
		OutputConsole("\n");

		Cmd *pCmd = m_pCurCmd->GetPrev();
		while(pCmd)
		{
			OutputConsole("prev Cmd: ");
			pCmd->Dump();
			OutputConsole("\n");
			pCmd = pCmd->GetPrev();
		}
		pCmd = m_pCurCmd->GetNext();
		while(pCmd)
		{
			OutputConsole("next Cmd: ");
			pCmd->Dump();
			OutputConsole("\n");
			pCmd = pCmd->GetNext();
		}
	}
}

void CmdMgr::Clear()
{
	if (m_pCurCmd)
	{
		//��ǰ
		Cmd *pPrev = m_pCurCmd->GetPrev();
		while(pPrev)
		{
			Cmd *pCmd = pPrev->GetPrev();
			SAFEDELETE(pPrev);
			pPrev = pCmd;
		}
		//���
		Cmd *pNext = m_pCurCmd->GetNext();
		while(pNext)
		{
			Cmd *pCmd = pNext->GetNext();
			SAFEDELETE(pNext);
			pNext = pCmd;
		}
		//�м�
		SAFEDELETE(m_pCurCmd);
	}
}