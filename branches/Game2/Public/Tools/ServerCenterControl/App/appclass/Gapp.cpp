

#include "stdafx.h"
#include "Gapp.h"
#include "UserManage.h"



Gapp::Gapp(tagGappBaseInfo &GappBaseInfo, DWORD dwSMID)
:tagGappBaseInfo(GappBaseInfo)
,m_dwSMID(dwSMID)
,m_lRoomUser(-1)
,m_dwNum(0)
,m_pDbGameInfo(NULL)
{

}

Gapp::~Gapp(VOID)
{
	WriteGameInfoToDB();
}

Gapp* Gapp::CreateGapp(tagGappBaseInfo &GappBaseInfo, DWORD dwSMID)
{
	Gapp *pRe = NULL;
	pRe = (Gapp*)M_ALLOC(sizeof(Gapp));
	if(NULL != pRe)
		new(pRe)Gapp(GappBaseInfo, dwSMID);
	return pRe;
}
VOID Gapp::ReleaseGapp(Gapp **ppGapp)
{
	if(NULL != ppGapp && NULL != (*ppGapp))
	{
		(*ppGapp)->~Gapp();
		M_FREE((*ppGapp), sizeof(Gapp));
		(*ppGapp) = NULL;
	}
}

VOID Gapp::SetVersion(LPCTSTR pNewVersion)
{
	if(NULL != pNewVersion)
	{
		LONG lCopySize = min((LONG)strlen(pNewVersion) + 1, MAX_GAPP_NAME_SZIE);
		memcpy(_szGappVersion, pNewVersion, MAX_GAPP_NAME_SZIE);
	}
}

//! ����״̬
BOOL Gapp::ChangeRunState(DWORD dwNewState)
{
	if(_dwGappState == dwNewState)
		return FALSE;
	
	_dwGappState = dwNewState;
	return TRUE;
}

//! ����echo�ӳ�ʱ��
VOID Gapp::ChangeDelayTime(DWORD dwNewDelayTime)
{
	_dwGappDelayTime = dwNewDelayTime;
}

//! ����������Ϣ���ͻ���
VOID Gapp::SendInfoToSocket(LONG lSocket)
{
	CMessage msg(MSG_SCC2SUC_INFO_Notify_Gapp_BaseInfo);
	/*DBWriteSet setWriteSet;
	msg.GetDBWriteSet(setWriteSet);
	setWriteSet.AddToByteArray(m_dwSMID);
	setWriteSet.AddToByteArray(this, sizeof(tagGappBaseInfo));*/

    Json::Value var;
    var["smID"] = (int)m_dwSMID;
    var["gappTime"] = (double)_dwGappDelayTime;
    var["gappID"] = (int)_dwGappID;
    var["gappType"] = (int)_dwGappType;
    var["gappVersion"] = _szGappVersion;
    var["gappName"] = _szGappName;
    var["gappState"] = (int)_dwGappState;
    var["gappDelayTime"] = (double)_dwGappDelayTime;
    msg.AddJson(var);

	msg.SendToSuc(lSocket);
}

//! ����������Ϣ�����пͻ���
VOID Gapp::SendInfoToAll(VOID)
{
	CMessage msg(MSG_SCC2SUC_INFO_Notify_Gapp_BaseInfo);
	/*DBWriteSet setWriteSet;
	msg.GetDBWriteSet(setWriteSet);
	setWriteSet.AddToByteArray(m_dwSMID);
	setWriteSet.AddToByteArray(this, sizeof(tagGappBaseInfo));*/
    Json::Value var;
    var["smID"] = (int)m_dwSMID;
    var["gappTime"] = (double)_dwGappDelayTime;
    var["gappID"] = (int)_dwGappID;
    var["gappType"] = (int)_dwGappType;
    var["gappVersion"] = _szGappVersion;
    var["gappName"] = _szGappName;
    var["gappState"] = (int)_dwGappState;
    var["gappDelayTime"] = (double)_dwGappDelayTime;
    msg.AddJson(var);

	UserManage::GetInstance().SendToAll(ePT_Penniless, msg);
}

//! ���������Ϣ
VOID Gapp::ShowAllInfo(VOID)
{
	LogTools::OutLogAndTxt("	Gapp��ID=%d Name=%s Version=%s DelayTime=%d", _dwGappID, _szGappName, _szGappVersion, (long)_dwGappDelayTime);
}

//! �������̨
BOOL Gapp::InCtrlRoom(LONG lUserID, BOOL bIn)
{
	if(bIn)
	{
		if(-1 == m_lRoomUser)
		{
			m_lRoomUser = lUserID;
			return TRUE;
		}
	}
	else
	{
		m_lRoomUser = -1;
		return TRUE;
	}
	
	return FALSE;
}


//! ���һ��������Ϣ
VOID Gapp::AddGameInfo(tagDbGameInfo &GameInfoData)
{
	if(MAX_GAME_INFO_NUM <= m_dwNum)
	{
		WriteGameInfoToDB();
	}

	if(NULL == m_pDbGameInfo)
	{
		m_pDbGameInfo = (tagDbGameInfo*)M_ALLOC(sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);
		if(NULL == m_pDbGameInfo)
		{
			LogTools::OutErrLog("alloc GameInfo lost!!");
			return;
		}
		m_dwNum = 0;
	}

	memcpy(&m_pDbGameInfo[m_dwNum], &GameInfoData, sizeof(tagDbGameInfo));
	++m_dwNum;
}


//! ��������Ϣд��DB
VOID Gapp::WriteGameInfoToDB(VOID)
{
	if(NULL != m_pDbGameInfo && 0 != m_dwNum)
	{
		DbOperator *pDbOperator = DbOperator::CreateGameInfoOperator(m_dwSMID, _dwGappID, m_pDbGameInfo, m_dwNum);
		if(NULL != pDbOperator)
			pDbOperator->Begin();
		else
			M_FREE(m_pDbGameInfo, sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);
	}
	else if(NULL != m_pDbGameInfo)
		M_FREE(m_pDbGameInfo, sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);

	m_pDbGameInfo = NULL;
	m_dwNum = 0;
}

//! ��ӦDB�������
VOID Gapp::OnDBWriteFinish(tagDbGameInfo *pGameInfoData, DWORD dwNum, BOOL bExecuteOK)
{
	if(0 == dwNum)
		return;
	if(!bExecuteOK)
	{
		//! ������ֵ˵�����ݿ����ʧ����
		LogTools::OutErrLog("GameInfo write db lost��lose log[%d]!!!", dwNum);
	}

	if(NULL != pGameInfoData)
		M_FREE(pGameInfoData, sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);
}

//! ����һ��ʱ���ڵ�������Ϣ���û�
VOID Gapp::FindGameInfoToUser(DWORD dwInfoType, DWORD dwBeginTime, DWORD dwEndTime, LONG lUserID)
{
	//! ����δ������Ϣ���ش���
	DWORD dwCurrTime = (DWORD)time(NULL);
	if(dwCurrTime <= dwBeginTime)
	{
		CMessage msg(MSG_SCC2SUC_OTHER_Notify_SendText);
	//	msg.Add("error! request unborn date.");
        Json::Value var;
        var["errText"] = "error! request unborn date.";
        msg.AddJson(var);
		msg.SendToSuc(lUserID);
		return;
	}

	//! �����ڴ��в���
	if(0 != m_dwNum && NULL != m_pDbGameInfo)
	{
		//! ������Ҫ�����ݶ����ڴ���
		if(dwBeginTime <= (DWORD)m_pDbGameInfo[0]._tTime)
		{
			CGameInfo GameInfo(dwInfoType);
			for (DWORD i = 0; i < m_dwNum; ++i)
			{
				if(dwInfoType == m_pDbGameInfo[i].dwInfoType)
				{
					GameInfo.AddInfo(m_pDbGameInfo[i]);
				}
			}

			CMessage msg(MSG_SCC2SUC_INFO_Re_Show);
			/*DBWriteSet dbWriteSet;
			msg.GetDBWriteSet(dbWriteSet);
			GameInfo.Code(dbWriteSet);*/
            Json::Value var;
            GameInfo.Code(var);
            msg.AddJson(var);
			msg.SendToSuc(lUserID);
			return;
		}
		//! �����������ڴ���
		else if(dwEndTime >= (DWORD)m_pDbGameInfo[0]._tTime)
		{
			//! �ȼ�¼�ڴ��е���Ϣ����
			DWORD pTmpNum = 0;
			tagDbGameInfo *pTmpDbGameInfo = (tagDbGameInfo*)M_ALLOC(sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);
			if(NULL == m_pDbGameInfo)
			{
				LogTools::OutErrLog("alloc GameInfoList1 lost!!");
				CMessage msg(MSG_SCC2SUC_OTHER_Notify_SendText);
				//msg.Add("system error!");
                Json::Value var;
                var["errText"] = "system error!";
                msg.AddJson(var);
				msg.SendToSuc(lUserID);
				return;
			}

			for (DWORD i = 0; i < m_dwNum; ++i)
			{
				if(dwInfoType == m_pDbGameInfo[i].dwInfoType)
				{
					memcpy(&pTmpDbGameInfo[pTmpNum], &m_pDbGameInfo[i], sizeof(tagDbGameInfo));
					++pTmpNum;
				}
			}

			//! �ٽ���ѯ����ͼ�¼������һ���ύ��DBģ��
			DbOperator *pDbOperator = DbOperator::CreateLoadGameInfo(m_dwSMID, GetID(), dwInfoType, pTmpDbGameInfo, pTmpNum, dwBeginTime, (DWORD)m_pDbGameInfo[0]._tTime, lUserID);
			if(NULL != pDbOperator)
				pDbOperator->Begin();
			else
			{
				M_FREE(pTmpDbGameInfo, sizeof(tagDbGameInfo) * MAX_GAME_INFO_NUM);
				LogTools::OutErrLog("alloc GameInfoList2 lost!!");
				CMessage msg(MSG_SCC2SUC_OTHER_Notify_SendText);
			//	msg.Add("system error!");
                Json::Value var;
                var["errText"] = "system error!";
                msg.AddJson(var);
				msg.SendToSuc(lUserID);
			}


			return;
		}
	}

	//! �ڴ���û������
	//! ����ѯ�����ύ��DBģ��
	DbOperator *pDbOperator = DbOperator::CreateLoadGameInfo(m_dwSMID, GetID(), dwInfoType, NULL, 0, dwBeginTime, dwEndTime, lUserID);
	if(NULL != pDbOperator)
		pDbOperator->Begin();
	else
	{
		LogTools::OutErrLog("alloc GameInfoList3 lost!!");
		CMessage msg(MSG_SCC2SUC_OTHER_Notify_SendText);
	//	msg.Add("system error!");
        Json::Value var;
        var["errText"] = "system error!";
        msg.AddJson(var);
		msg.SendToSuc(lUserID);
	}

}