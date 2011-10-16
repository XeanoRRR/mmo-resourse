//////////////////////////////////////////////////////////////////////////
//SocketCommands.cpp
//Fun:��Socket�Ĳ��������б�
//Create Time:2004.12.17
//Author:Joe
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include ".\socketcommands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSocketCommands::CSocketCommands(void)
{
	InitializeCriticalSection(&m_CriticalSectionCommans);
}

CSocketCommands::~CSocketCommands(void)
{
	Clear();
	DeleteCriticalSection(&m_CriticalSectionCommans);
}

// ѹ���������
bool CSocketCommands::Push_Front(tagSocketOper* pCommand)
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	if( pCommand )
	{
		m_Commands.push_front(pCommand);
		LeaveCriticalSection(&m_CriticalSectionCommans);
		return true;
	}

	LeaveCriticalSection(&m_CriticalSectionCommans);
	return false;
}

// ѹ��������к��
bool CSocketCommands::Push_Back(tagSocketOper* pCommand)
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	if( pCommand )
	{
		m_Commands.push_back(pCommand);
		LeaveCriticalSection(&m_CriticalSectionCommans);
		return true;
	}

	LeaveCriticalSection(&m_CriticalSectionCommans);
	return false;
}

// �����������
tagSocketOper* CSocketCommands::Pop_Front()
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	if (m_Commands.empty())
	{
		LeaveCriticalSection(&m_CriticalSectionCommans);
		return NULL;
	}

	tagSocketOper* pCommand = *m_Commands.begin();
	m_Commands.pop_front();
	LeaveCriticalSection(&m_CriticalSectionCommans);
	return pCommand;
}

// �õ�������г���
long CSocketCommands::GetSize()
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	long lSize = (long)m_Commands.size();
	LeaveCriticalSection(&m_CriticalSectionCommans);
	return lSize;
}

// ����������
void CSocketCommands::Clear()
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	for (CommandsQueue::iterator it = m_Commands.begin(); it!=m_Commands.end(); it++)
	{
		if((*it)->OperaType==SOCKETOPERATYPE_SENDTOSOCKET ||
			(*it)->OperaType==SOCKETOPERATYPE_SENDTOMAPID ||
			(*it)->OperaType== SOCKETOPERATYPE_SENDALL)
		{
			//����Ƿ��ͻ�������Ҫɾ��new[]�Ļ�����
			delete [](*it)->pBuf;
		}
		if((*it)->OperaType==SOCKETOPERATYPE_SENDTOMAPSTR)
		{
			delete [](*it)->pStrID;
			delete [](*it)->pBuf;
		}
		if((*it)->OperaType == SOCKETOPERATYPE_CDKEYJOIN ||
			(*it)->OperaType == SOCKETOPERATYPE_QUITBYMAPSTR)
		{
			delete [](*it)->pStrID;
		}
		delete (*it);
	}
	m_Commands.clear();
	LeaveCriticalSection(&m_CriticalSectionCommans);
}

void CSocketCommands::CopyAllCommand(CommandsQueue& TemptCommandsQueue)
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	if(m_Commands.size() > 0)
	{
		TemptCommandsQueue = m_Commands;
		m_Commands.clear();
	}
	LeaveCriticalSection(&m_CriticalSectionCommans);
}

void CSocketCommands::AddCommandsQueueToFront(CommandsQueue& TemptCommandsQueue)
{
	EnterCriticalSection(&m_CriticalSectionCommans);
	m_Commands.insert(m_Commands.begin(),TemptCommandsQueue.begin(),TemptCommandsQueue.end());
	LeaveCriticalSection(&m_CriticalSectionCommans);
}
