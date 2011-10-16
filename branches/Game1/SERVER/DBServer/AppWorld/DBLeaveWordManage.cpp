

#include "StdAfx.h"
#include "DBLeaveWordManage.h"
#include "DBentityManager.h"
#include "DBAccessThread.h"

DBLeaveWordManage::DBLeaveWordManage(void)
:m_lMaxSaveTime(0)
{

}

DBLeaveWordManage::~DBLeaveWordManage(void)
{
}

DBLeaveWordManage g_DBLeaveWordManage;



DBLeaveWordManage&	DBLeaveWordManage::GetInstance(void)
{
	return g_DBLeaveWordManage;
}


//! ��Ӧ������Ϣ
void DBLeaveWordManage::OnSetupMessage(CMessage *pMsg)
{
	tagLeaveWordSetup	LeaveWordSetup;
	pMsg->GetEx(&LeaveWordSetup, sizeof(tagLeaveWordSetup));

	_ConnectionPtr& cn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();

	RsLeaveWord::GetRsLeaveWord().CreateInsertPro(LeaveWordSetup, cn);

	//! ��¼��󱣴�ʱ��
	m_lMaxSaveTime = LeaveWordSetup.lMaxSaveTime;
}

//! ��Ӧ������Ϣ
void DBLeaveWordManage::OnSaveMessage(CMessage *pMsg)
{
	tagLeaveWord_W2DB	LeaveWord_W2DB;
	LONG lBufID = pMsg->GetLong();
	pMsg->GetEx(&LeaveWord_W2DB, sizeof(tagLeaveWord_W2DB));
	char szWord[MAX_LEAVE_WORD_SIZE] = {0};
	pMsg->GetStr(szWord, MAX_LEAVE_WORD_SIZE);

	_ConnectionPtr& cn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();

	LONG lWirteResult = RsLeaveWord::GetRsLeaveWord().InsertLeaveWord(LeaveWord_W2DB, szWord, cn);

	CMessage msg(MSG_DB2W_LEAVEWORD_SAVE_RESULT);
	msg.Add(lBufID);
	msg.Add(lWirteResult);
	msg.SendToSocket(GetGame()->GetWSSocket());
}

//! ��Ӧȡ��������Ϣ
void DBLeaveWordManage::OnPopMessage(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	vector<BYTE> vData;
	_ConnectionPtr& cn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();

	if(RsLeaveWord::GetRsLeaveWord().PopLeaveWord_ToByteArray(PlayerGuid, vData, cn))
	{
		CMessage msg(MSG_DB2W_LEAVEWORD_RE_POP);
		msg.Add(PlayerGuid);
		msg.Add((LONG)vData.size());
		msg.AddEx(&vData[0], vData.size());
		msg.SendToSocket(GetGame()->GetWSSocket());
	}
}

//! ��Ӧ�����������
void DBLeaveWordManage::OnClearOuttimeMessage(CMessage *pMsg)
{
	static LONG lLastTimeClear = 0;
	LONG lNow = time(NULL);
	//! ����1��Сʱ��ִ��һ��
	if (60 * 60 * 1000 <= lNow - lLastTimeClear)
	{
		lLastTimeClear = lNow;

		LONG lTime = lNow - m_lMaxSaveTime;
		_ConnectionPtr& cn = GetGame()->GetDBEntityManager()->GetAccessThread()->GetConnPtr();
		RsLeaveWord::GetRsLeaveWord().ClearLeaveWord(lTime, cn);
	}
}

