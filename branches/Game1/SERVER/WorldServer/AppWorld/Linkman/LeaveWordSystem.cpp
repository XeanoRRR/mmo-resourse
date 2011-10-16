

#include "StdAfx.h"
#include "LeaveWordSystem.h"
#include "LinkmanSystem.h"
#include "..\nets\networld\message.h"


LeaveWordSystem::LeaveWordSystem(void)
:m_pTimerQueue(NULL),
m_BufIdSeed(0)
{
	memset(&m_LeaveWordSetup, 0xff, sizeof(tagLeaveWordSetup));
}

LeaveWordSystem::~LeaveWordSystem(void)
{
	Release();
}

LeaveWordSystem	g_LeaveWordSystem;

LeaveWordSystem& LeaveWordSystem::GetInstance(void)
{
	return g_LeaveWordSystem;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��ʼ��
BOOL LeaveWordSystem::Init(const char *pSetupFile)
{
	if (!LoadSetup(pSetupFile))
	{
		AddLogText("����ϵͳ��ʼ�����󣺶�ȡ��������ʧ�ܣ�");
		return FALSE;
	}
	if (!StartupTimer())
	{
		AddLogText("����ϵͳ��ʼ�����󣺳�ʼ����ʱ��ʧ�ܣ�");
		return FALSE;
	}

	//! Ԥ�����ڴ�
	GetGame()->GetMemPool().SetInitialTileNum(sizeof(tagLeaveWord_W2DB), 128);

	return TRUE;
}

//! �������õ�DBS
void LeaveWordSystem::SendSetupToDBS(void)
{
	CMessage msg(MSG_W2DB_LEAVEWORD_SETUP);
	msg.AddEx(&m_LeaveWordSetup, sizeof(tagLeaveWordSetup));
	msg.SendToSocket(GetGame()->GetDBSocketID());	
}

//! �ͷ�
void LeaveWordSystem::Release(void)
{
	if (NULL != m_pTimerQueue)
	{
		//! ȡ������ʱ
		m_pTimerQueue->Cancel(this);
		//! ɾ������
		SAFE_DELETE(m_pTimerQueue);
		
		//! ����������˳�ʱm_LeaveWriteBufӦΪ��
		for (map<LONG, tagLeaveWord_W2DB*>::iterator ite = m_LeaveWriteBuf.begin(); ite != m_LeaveWriteBuf.end(); ++ite)
		{
			ReleaseWord_W2DB(ite->second);
			ite->second = NULL;
		}
		m_LeaveWriteBuf.clear();
	}	
}

//! �������
BOOL LeaveWordSystem::LeaveWord(const CGUID &SenderGuid, const char *pReceiverName, char *pWord)
{	
	CPlayer *pSender = GetGame()->GetMapPlayer(SenderGuid);
	if (NULL == pSender)return FALSE;

	map<CGUID, LONG>::iterator ite = m_PlayerLvwdCount.find(SenderGuid);
	if (m_PlayerLvwdCount.end() != ite)
	{
		if(m_LeaveWordSetup.lMaxLeaveWordNum <= ite->second)
		{
			//! ������Ϣ
			CMessage msg(MSG_W2S_LEAVEWORD_SAVE_RESULT);
			msg.Add((LONG)eLWR_SendMore);
			msg.Add(pReceiverName);
			msg.Add(SenderGuid);
			msg.SendToSocket(pSender->GetGsSocket());
			return FALSE;
		}
	}
	//! ����������ȼ���������GUID
	CGUID ReceiverGuid;
	ULONG uPRI = LinkmanSystem::GetInstance().CanLeaveWord(SenderGuid, pReceiverName, ReceiverGuid);
	
	
	if (eLWPRI_Cant == uPRI)
	{
		RePlayerSendFailed(pSender, pReceiverName);
		return FALSE;
	}
	assert(NULL_GUID != ReceiverGuid);
	if (NULL_GUID == ReceiverGuid) return FALSE;

	tagLeaveWord_W2DB* pLeaveWord_W2DB = CreateLeaveWord_W2DB();
	pLeaveWord_W2DB->dwSenderType	= eSender_Player;
	pLeaveWord_W2DB->ReceiverGuid	= ReceiverGuid;
	pLeaveWord_W2DB->SenderGuid		= SenderGuid;
	pLeaveWord_W2DB->uPRI			= uPRI;
	strcpy(pLeaveWord_W2DB->szSenderName, pSender->GetName());

	LONG lBufID = GetBufID();

	CMessage msg(MSG_W2DB_LEAVEWORD_SAVE);
	msg.Add(lBufID);
	msg.AddEx(pLeaveWord_W2DB, sizeof(tagLeaveWord_W2DB));
	msg.Add(pWord);
	msg.SendToSocket(GetGame()->GetDBSocketID());

	
	assert(m_LeaveWriteBuf.end() == m_LeaveWriteBuf.find(lBufID));
	strcpy(pLeaveWord_W2DB->szReceiverName, pReceiverName);
	m_LeaveWriteBuf[lBufID] = pLeaveWord_W2DB;

	return TRUE;	
}

//! ��Ӧ�������
void LeaveWordSystem::OnPlayerLogin(CPlayer *pReceiver)
{
	if(m_WaitPlayerGuid.find(pReceiver->GetExID()) == m_WaitPlayerGuid.end())
	{
		m_WaitPlayerGuid.insert(pReceiver->GetExID());
		CMessage msg(MSG_W2DB_LEAVEWORD_POP);
		msg.Add(pReceiver->GetExID());
		msg.SendToSocket(GetGame()->GetDBSocketID());
	}
}

//! ��Ӧ�������ȡ��
void LeaveWordSystem::OnLeaveWordPop(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	LONG lDataSize = pMsg->GetLong();

	CPlayer *pPlayer = GetGame()->GetMapPlayer(PlayerGuid);
	if (NULL != pPlayer && sizeof(LONG) < lDataSize)
	{
		pMsg->SetType(MSG_W2S_LEAVEWORD_POP);
		pMsg->SendToSocket(pPlayer->GetGsSocket());
	}
	
	m_WaitPlayerGuid.erase(PlayerGuid);
}

//! ��Ӧ���Ա�����
void LeaveWordSystem::OnLeaveWordResult(CMessage *pMsg)
{
	LONG lBufID		= pMsg->GetLong();
	LONG lResult	= pMsg->GetLong();

	map<LONG, tagLeaveWord_W2DB*>::iterator ite = m_LeaveWriteBuf.find(lBufID);
	assert(ite != m_LeaveWriteBuf.end());
	if (ite == m_LeaveWriteBuf.end())return;

	tagLeaveWord_W2DB* pLeaveWord_W2DB = ite->second;
	m_LeaveWriteBuf.erase(lBufID);

	CPlayer *pSender = GetGame()->GetMapPlayer(pLeaveWord_W2DB->SenderGuid);
	if (NULL != pSender)
	{
		const CGUID &SenderGuid = pSender->GetExID();
		//! ������Ϣ
		CMessage msg(MSG_W2S_LEAVEWORD_SAVE_RESULT);
		msg.Add(lResult);
		msg.Add(pLeaveWord_W2DB->szReceiverName);
		msg.Add(SenderGuid);
		msg.SendToSocket(pSender->GetGsSocket());

		//! ��¼���Դ���
		if (m_PlayerLvwdCount.end() == m_PlayerLvwdCount.find(SenderGuid))
		{
			m_PlayerLvwdCount[SenderGuid] = 1;
		}
		else
		{
			m_PlayerLvwdCount[SenderGuid] += 1;
		}
	}

	ReleaseWord_W2DB(pLeaveWord_W2DB);
}

//! ��ʱ����ʱ��Ӧ
long LeaveWordSystem::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	m_PlayerLvwdCount.clear();
	CMessage msg(MSG_W2DB_LEAVEWORD_CLEAR_OUTTIME);
	msg.SendToSocket(GetGame()->GetDBSocketID());
	return 0;
}
//! ����ʱ����ȡ���ĵ���
void LeaveWordSystem::OnTimerCancel(DWORD timerid,const void* var)
{
	
}

//! ��ʱ����ⴥ��
void LeaveWordSystem::TimerExpire(LONG lCurrTime)
{
	if(NULL != m_pTimerQueue)
		m_pTimerQueue->Expire(lCurrTime);
}



//! private
////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��ȡ����
BOOL LeaveWordSystem::LoadSetup(const char *pSetupFile)
{
	if (NULL == pSetupFile)return FALSE;

	//! �򿪸��ڵ�
	TiXmlNode *pNode = NULL; 		

	CRFile *prfile = rfOpen(pSetupFile);
	if(NULL == prfile)
	{
		//���������Ϣ
		return FALSE;
	}

	TiXmlDocument m_Tdoc(pSetupFile);
	//���뼼�������ļ�
	if( !m_Tdoc.LoadData(prfile->GetData(),prfile->GetDatalen()) )
	{
		//���������Ϣ
		rfClose(prfile);
		return FALSE;
	}
	rfClose(prfile);

	pNode=m_Tdoc.FirstChild("LeaveWordSetup");
	if (pNode==NULL) return FALSE;

	TiXmlElement* pLeaveWordSetup = pNode->ToElement();//��ȡnode ��ָ��
	if (pLeaveWordSetup==NULL) return FALSE;

	const char *pValue = NULL;
	//! ����ʱ���������������
	if(NULL == (pValue = pLeaveWordSetup->Attribute("MaxLeaveWordNum"))) return FALSE;
	m_LeaveWordSetup.lMaxLeaveWordNum = atoi(pValue);
	//! ����ʱ��(�����õķ�ת������)
	if(NULL == (pValue = pLeaveWordSetup->Attribute("OptPeriodOfTime"))) return FALSE;
	m_LeaveWordSetup.lOptPeriodOfTime = 60 * atoi(pValue);
	//! ����͵���һ��ɫ������
	if(NULL == (pValue = pLeaveWordSetup->Attribute("MaxToPlayerLeaveWordNum"))) return FALSE;
	m_LeaveWordSetup.lMaxToPlayerLeaveWordNum = atoi(pValue);
	//! ��󱣴�������
	if(NULL == (pValue = pLeaveWordSetup->Attribute("MaxSaveLeaveWordNum"))) return FALSE;
	m_LeaveWordSetup.lMaxSaveLeaveWordNum = atoi(pValue);
	//! ��󱣴����ޣ������õ�Сʱת�����룩
	if(NULL == (pValue = pLeaveWordSetup->Attribute("MaxSaveTime"))) return FALSE;
	m_LeaveWordSetup.lMaxSaveTime = 60 * 60 * atoi(pValue);

	return TRUE;
}

//! ����������ʱ��
BOOL LeaveWordSystem::StartupTimer(void)
{
	//! ������ʱ����Ԥ��4���ڵ㣬������Ϊ��
	m_pTimerQueue = new CTimerQueue<LeaveWordSystem*>(4, 1000 * 60);

	//! ����������������ʱ����2�����Ժ�ʼ��һ��ִ�У�
	m_pTimerQueue->Schedule(this, NULL, 2 * 60 * 1000, m_LeaveWordSetup.lOptPeriodOfTime * 1000);

	return TRUE;
}

//! ֪ͨ������ķ���ʧ��
void LeaveWordSystem::RePlayerSendFailed(CPlayer *pSender, const char *pReceiverName)
{
	CMessage msg(MSG_W2S_OTHER_TALK);
	msg.Add((BYTE)9);		// ʧ��
	msg.Add((LONG)CHAT_PRIVATE);
	msg.Add(pSender->GetType());
	msg.Add(pSender->GetExID());
	msg.Add(0L);
	msg.Add(pReceiverName);
	msg.SendToSocket(pSender->GetGsSocket());
}

//! ���һ�������ID
LONG LeaveWordSystem::GetBufID(void)
{
	return ++m_BufIdSeed;
}

//! ����WSDB�ṹ
tagLeaveWord_W2DB* LeaveWordSystem::CreateLeaveWord_W2DB(void)
{
	void* pRe = GetGame()->GetMemPool().Multi_Alloc(sizeof(tagLeaveWord_W2DB), _FILE_AND_LINE_);
	new(pRe)tagLeaveWord_W2DB();
	return (tagLeaveWord_W2DB*)pRe;
}
//! �ͷ�WSDB�ṹ
void LeaveWordSystem::ReleaseWord_W2DB(tagLeaveWord_W2DB* pLeaveWord_W2DB)
{
	GetGame()->GetMemPool().Multi_Free(pLeaveWord_W2DB, sizeof(tagLeaveWord_W2DB));
}