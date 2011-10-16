#include "StdAfx.h"
#include "OrganizingCtrl.h"

#include "Faction.h"
#include "NoOrganizing.h"
#include "OrganizingParam.h"
#include "WSTeam.h"
#include "../DBEntity/EntityManager.h"
#include "../GlobalRgnManager.h"
#include "../Player.h"
#include "../PhaseSystem/PhaseMap.h"
#include "../../../../Public/Setup/GlobalSetup.h"
#include "../../../../Public/Common/PlayerDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//�ȽϷº���
struct KeyCompareByEarly
{
	bool operator()(const tagKey& KeyX, const tagKey& KeyY)
	{
		if(KeyX.l > KeyY.l)
		{
			return true;
		}
		else if(KeyX.l == KeyY.l)
		{
			if(KeyY.Time >= KeyX.Time)
				return true;
		}
		return false;
	}
};

struct KeyCompareByLate
{
	bool operator()(const tagKey& KeyX, const tagKey& KeyY)
	{
		if(KeyX.l > KeyY.l)
		{
			return true;
		}
		else if(KeyX.l == KeyY.l)
		{
			if(KeyX.Time >= KeyY.Time)
				return true;
		}
		return false;
	}
};

COrganizingCtrl* COrganizingCtrl::instance = NULL;



COrganizingCtrl::COrganizingCtrl(VOID)
:m_bFacDataFull(FALSE)
,m_bUnionDataFull(FALSE)
,m_InviteSession(60, FALSE)
,m_UnionSession(60, FALSE)
,m_TimerQueue(4, 5000)
,m_lExpTimerID(0)
{
}

COrganizingCtrl::~COrganizingCtrl(VOID)
{
	m_TimerQueue.Cancel(this);
}

//�õ�Ψһʵ�����
//�ú�����֧�ֶ��̲߳���
inline COrganizingCtrl*COrganizingCtrl:: getInstance()
{
	if(instance == NULL)
	{
		instance = new COrganizingCtrl();
	}
	return instance;
}

//��ʼ��
bool   COrganizingCtrl::Initialize()
{
	if (m_bFacDataFull) return true;
	Log4c::Trace(ROOT_MODULE,GetText("WS_ORGCTL_0"));


	WSTeam::s_tagDefaultSetup.lCorrelativeSkillID	= GlobalSetup::GetSetup()->lCorrelativeSkillID;		//����ӹ����ļ���ID
	WSTeam::s_tagDefaultSetup.lMinSkillLevel		= GlobalSetup::GetSetup()->lMinSkillLevel;			//������ӵ���С���ܵȼ�
	WSTeam::s_tagDefaultSetup.lGoodsAllot			= GlobalSetup::GetSetup()->new_lGoodsAllot;			//Ĭ�Ϸ��䷽ʽ
	WSTeam::s_tagDefaultSetup.lMaxMemberNum			= GlobalSetup::GetSetup()->new_lMaxMemberNum;		//Ĭ���������
	WSTeam::s_tagDefaultSetup.lSessionTimeoutS		= GlobalSetup::GetSetup()->new_lSessionTimeoutS;	//�Ự��ʱ����
	WSTeam::s_tagDefaultSetup.fIncreasedExp_2		= GlobalSetup::GetSetup()->new_fIncreasedExp_2;		//2�˾���ӳɱ���
	WSTeam::s_tagDefaultSetup.fIncreasedExp_3		= GlobalSetup::GetSetup()->new_fIncreasedExp_3;		//3�˾���ӳɱ���
	WSTeam::s_tagDefaultSetup.fIncreasedExp_4		= GlobalSetup::GetSetup()->new_fIncreasedExp_4;		//4�˾���ӳɱ���
	WSTeam::s_tagDefaultSetup.fIncreasedExp_5		= GlobalSetup::GetSetup()->new_fIncreasedExp_5;		//5�˾���ӳɱ���
	WSTeam::s_tagDefaultSetup.fIncreasedExp_6		= GlobalSetup::GetSetup()->new_fIncreasedExp_6;		//6�˾���ӳɱ���
	WSTeam::s_tagDefaultSetup.lExperienceRadii		= GlobalSetup::GetSetup()->new_lExperienceRadii;	//�������뾶����λ����
	WSTeam::s_tagDefaultSetup.lExpDiff				= GlobalSetup::GetSetup()->new_lExpDiff;			//�����
	WSTeam::s_tagDefaultSetup.lExpLimit				= GlobalSetup::GetSetup()->new_lExpLimit;			//��Ҿ���ֵ�ݶ��������
	WSTeam::s_tagDefaultSetup.fExpAmerce			= GlobalSetup::GetSetup()->new_fExpAmerce;			//����ͷ�ϵ��

	Log4c::Trace(ROOT_MODULE,GetText("WS_ORGCTL_1"));

	SYSTEMTIME	stTime;
	GetLocalTime(&stTime);
	//! ��ǰ���賿12���ʱ�䣨���룩
	DWORD dwToZeroTime = (((24 - stTime.wHour) * 60 - stTime.wMinute) * 60 - stTime.wSecond) * 1000;

	return true;
}

//�ͷ���Դ
VOID COrganizingCtrl::Release()
{
	GetTimerQueue().Cancel(this);

	for(map<CGUID, CFaction*>::iterator it = m_FactionOrganizings.begin(); it != m_FactionOrganizings.end(); it++)
	{
		SAFE_DELETE((*it).second);
	}

	for(map<CGUID, CUnion*>::iterator it = m_mapUnion.begin(); it != m_mapUnion.end(); it++)
	{
		SAFE_DELETE((*it).second);
	}

	//�ͷŶ���
	for(map<CGUID,WSTeam*>::iterator ite = m_TeamOrganizings.begin(); ite != m_TeamOrganizings.end(); ++ite)
	{
		SAFE_DELETE((*ite).second);
	}
	m_TeamOrganizings.clear();

	m_bFacDataFull = FALSE;
}

VOID COrganizingCtrl::ClearFaction(VOID)
{
	for(map<CGUID, CFaction*>::iterator it = m_FactionOrganizings.begin(); it != m_FactionOrganizings.end(); it++)
	{
		SAFE_DELETE((*it).second);
	}
	m_FactionOrganizings.clear();
}

VOID COrganizingCtrl::ClearUnion(VOID)
{
	for(map<CGUID, CUnion*>::iterator it = m_mapUnion.begin(); it != m_mapUnion.end(); it++)
	{
		SAFE_DELETE((*it).second);
	}
	m_mapUnion.clear();
}


VOID COrganizingCtrl::OutOrganiInfo(VOID)
{
//	char szTmp[1204] = {0};

	LONG lFacNum		= (LONG)m_FactionOrganizings.size();
	LONG lApplicantNum	= (LONG)m_mapAllApplicant.size();
	LONG lTeamNum		= (LONG)m_TeamOrganizings.size();
	LONG lTeamMemberNum = (LONG)m_MemberIDvsTeamID.size();
	LONG lTeamSession	= m_InviteSession.Size();
// 	sprintf(szTmp, "Faction Num = %d\r\n Applicant Player Num = %d\r\n Team Num = %d\r\n Team's Member = %d\r\n Team Session = %d\r\n\r\n",
// 				   lFacNum, lApplicantNum, lTeamNum, lTeamMemberNum, lTeamSession
// 				   );
// 	PutStringToFile("MemAllocInfo", szTmp);

    Log4c::Trace(ROOT_MODULE,"Faction Num = %d\r\n Applicant Player Num = %d\r\n Team Num = %d\r\n Team's Member = %d\r\n Team Session = %d\r\n\r\n",
        lFacNum, lApplicantNum, lTeamNum, lTeamMemberNum, lTeamSession);
}

//��֯ϵͳ��Run
VOID	COrganizingCtrl::Run(long lInterTime)
{

}

//! ��ӦGS������
VOID COrganizingCtrl::OnGsConnect(LONG lGsSocket)
{
	//! ���͹�����Ϣ��GS
	if (m_bFacDataFull)
	{
		SendFactionDataToGS(lGsSocket);
	}
	else//! ������ݳ�ʼ����δ��ɣ��Ƚ��ͻ��˱����������б�
	{
		m_setWaitFacSocket.insert(lGsSocket);
	}

	//! ����������Ϣ��GS
	if (m_bFacDataFull)
	{
		SendUnionDataToGS(lGsSocket);
	}
	else//! ������ݳ�ʼ����δ��ɣ��Ƚ��ͻ��˱����������б�
	{
		m_setWaitUnionSocket.insert(lGsSocket);
	}

	//! ���������Ϣ��GS
	SendTeamDataToGS(lGsSocket);
}


//�������ʱ����
VOID COrganizingCtrl::OnPlayerDied(const CGUID& PlayerGuid, const CGUID& lKiller)
{
	
}

//����һ����ҽ�����Ϸ
VOID COrganizingCtrl::OnPlayerEnterGame(const CGUID& PlayerGuid)
{
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;

	const CGUID& FactionGuid = FindPlayerFacionID(PlayerGuid);
	CFaction* pFaction = GetFactionOrganizing(FactionGuid);
	if(pFaction != NULL)	
	{
		pPlayer->SetFactionID(FactionGuid);
		CUnion *pUnion = GetUnionOrganizing(pFaction->GetSuperior());
		pFaction->OnMemberEnterGame(pPlayer);
		if(NULL != pUnion)
		{
			pPlayer->SetUnionID(pUnion->GetExID());
		}
	}

	const CGUID& TeamID = FindPlayerTeamID(PlayerGuid);
	pPlayer->SetTeamID(TeamID);
    if ( TeamID != NULL_GUID )
    {
        pPlayer->SetPhaeID( GetInst(PhaseMap).GetPhaseID(PlayerGuid));
        pPlayer->SetPhaseRest( GetInst(PhaseMap).GetPhaseRest(PlayerGuid));
    }
}

VOID COrganizingCtrl::OnPlayerEnterGameLaterDataSend(const CGUID& PlayerGuid)
{
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;

	WSTeam* pTeam = GetGameTeamByID(pPlayer->GetTeamID());
	if(pTeam != NULL)	
	{
		pTeam->OnMemberEnterGame(PlayerGuid);
	}
}

//����һ������뿪��Ϸ
VOID COrganizingCtrl::OnPlayerExitGame(CPlayer *pPlayer, bool IsChangeGS)
{
	if(NULL == pPlayer) return;

	const CGUID& FactionGuid = pPlayer->GetFactionID();
	CFaction* pFaction = GetFactionOrganizing(FactionGuid);
	if(pFaction != NULL && (!IsChangeGS))	
	{
		pFaction->OnMemberExitGame(pPlayer);
	}

	const CGUID& TeamID = pPlayer->GetTeamID();
	WSTeam* pTeam = (WSTeam*)GetGameTeamByID(TeamID);
	if(NULL != pTeam && (!IsChangeGS))	
	{
		if(0 == pTeam->GetOnlineMemberNum())
		{
			DisbandTeam(pTeam);
			return;
		}

		pTeam->OnMemberExitGame(pPlayer->GetExID());
	}

	
	
}
//���ɾ����ɫʱ����
int COrganizingCtrl::OnDeleteRole(const CGUID& PlayerGuid)
{
	///////////////////////////////////////////////////
	const CGUID& FactionGuid = FindPlayerFacionID(PlayerGuid);
	if( FactionGuid == NULL_GUID ) return 0;
	CFaction* pFaction = GetFactionOrganizing(FactionGuid);
	if( pFaction == NULL ) return 0;

	if(pFaction->GetMasterID() == PlayerGuid)
	{
		if(pFaction->GetSuperior() != NULL_GUID)
		{
			CUnion* pUnion = GetUnionOrganizing(pFaction->GetSuperior());
			//���������˳�
			if(pUnion )
			{
				if(pUnion->GetMasterID() == FactionGuid)
				{
					//��ɢͬ��
					return 3;
				}
				else
				{
					//�˳�ͬ��
					return 2;
				}
			}
		}
		else
		{
			//ӵ����������ɾ������ɫ��Ϣ


			//��ɢ���
			GetOrganiCtrl()->OnDelFaction(pFaction->GetExID(), FALSE);
		}
	}
	else
	{	//�˳����
		pFaction->Exit(PlayerGuid);
	}
	return 0;
}

//��Ӧ��ҵ�ͼ�ı�
VOID COrganizingCtrl::OnPlayerMapChange(const CGUID &PlayerGuid, const char* pMapName)
{
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;
	if (NULL_GUID == pPlayer->GetFactionID()) return;

	CFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if (NULL == pFaction) return;

	pFaction->MemberMapChange(PlayerGuid, pMapName);
}

//��Ӧ���ְҵ�ı�
VOID COrganizingCtrl::OnPlayerOccuChange(CPlayer *pPlayer, LONG lOccu)
{
	if (NULL == pPlayer) return;
	if (NULL_GUID == pPlayer->GetFactionID()) return;

	CFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
	if (NULL == pFaction) return;

	pFaction->MemberOccuChange(pPlayer->GetExID(), lOccu);
	
	const CGUID& TeamID = pPlayer->GetTeamID();
	if (NULL_GUID != TeamID)
	{
		WSTeam* pTeam = GetGameTeamByID(TeamID);
		if(pTeam != NULL)	
		{
			CMessage msg(MSG_W2S_TEAM_OCCUCHANGE);
			msg.Add(TeamID);
			msg.Add(pPlayer->GetExID());
			msg.Add(lOccu);
			msg.SendToAllGS();
		}
	}
}

//��Ӧ��ҵȼ��ı�
VOID COrganizingCtrl::OnPlayerLevelChange(CPlayer *pPlayer, LONG lLevel)
{
	if (NULL == pPlayer) return;
	if (NULL_GUID != pPlayer->GetFactionID())
	{
		CFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL == pFaction) return;

		pFaction->MemberLevelChange(pPlayer->GetExID(), lLevel);
	}

	const CGUID& TeamID = pPlayer->GetTeamID();
	if (NULL_GUID != TeamID)
	{
		WSTeam* pTeam = GetGameTeamByID(TeamID);
		if(pTeam != NULL)	
		{
			CMessage msg(MSG_W2S_TEAM_LEVELCHANGE);
			msg.Add(TeamID);
			msg.Add(pPlayer->GetExID());
			msg.Add(lLevel);
			msg.SendToAllGS();
		}
	}
}

//��Ӧ�������״̬�ı�
VOID COrganizingCtrl::OnPlayerHideChange(CPlayer *pPlayer, BYTE cHide)
{
	if (NULL == pPlayer) return;
	if (NULL_GUID != pPlayer->GetFactionID())
	{
		CFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL == pFaction) return;

		pFaction->MemberHideChange(pPlayer->GetExID(), cHide);
	}
}

VOID COrganizingCtrl::SendOrgaInfoToClient(const CGUID& PlayerGuid,string& strSendCon,string& strSender,long lGameServerID,DWORD dwFontColor,DWORD dwBkColor)
{
	if(lGameServerID==-1)
	{
		lGameServerID = GetGame().GetGameServerNumber_ByPlayerID(PlayerGuid);
	}
	if(lGameServerID==-1) return;
	CMessage msg( MSG_W2S_SERVER_ADDINFO );
	msg.Add( PlayerGuid );
	msg.Add(strSendCon.c_str());
	/***********************************************************************/
	/*  zhaohang 2010-11-8 fix*/
	/***********************************************************************/
	//��ɫֱ�Ӽ��뵽�ַ����У��ͻ����ܹ��Զ�����
	/*
	msg.Add( dwFontColor );
	msg.Add( dwBkColor );
	*/
    msg.Add((LONG)eNOTIFYPOS_LEFT_BOTTOM);
	msg.Add(strSender.c_str());
	msg.SendToGS(lGameServerID);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VOID COrganizingCtrl::SaveAllFaction(VOID)
{
	vector<CEntityGroup*> vCEntityGroup;
	
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"SaveAllFaction start");
#endif

	// �����������
	CEntityManager::DBEntityComponentMapItr facMapItr = GetGame().GetEntityManager()->GetBaseEntityMap().find(string("[factiongroup]"));
	if(facMapItr != GetGame().GetEntityManager()->GetBaseEntityMap().end())
	{
		if(facMapItr->second.size() == 0)
		{
			Log4c::Warn(ROOT_MODULE, GetText("WS_ORGCTL_2"));
			return;
		}
		CEntityGroup* pFacGroup = (CEntityGroup*)(facMapItr->second.begin()->second);
		if(pFacGroup)
			pFacGroup->ReleaseChilds();
	}

	for (map<CGUID, CFaction*>::const_iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		CEntityGroup* pTmpFaction = NULL;
		ite->second->Save(&pTmpFaction);
		if(NULL != pTmpFaction)
			vCEntityGroup.push_back(pTmpFaction);
	}

#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"SaveAllFaction create session start");
#endif
	GetGame().GetEntityManager()->CreateSaveFactionSession(vCEntityGroup);
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"SaveAllFaction create session end");
#endif

	Log4c::Trace(ROOT_MODULE,FormatText("WS_ORGCTL_3", m_FactionOrganizings.size()));
}
VOID COrganizingCtrl::SaveAllUnion(VOID)
{
	for (map<CGUID, CUnion*>::const_iterator ite = m_mapUnion.begin(); ite != m_mapUnion.end(); ++ite)
	{
		ite->second->Save();
	}

	Log4c::Trace(ROOT_MODULE,FormatText("WS_ORGCTL_4", m_mapUnion.size()));
}



//! ��Ӧ��ʼ�������β��Ϣ(FORM DBS)
VOID COrganizingCtrl::OnInitFactionEndMsg(VOID)
{
	//! ������ɱ��
	m_bFacDataFull = TRUE;
	GetGame().SetInitFactionLoadFlag(true);
	Log4c::Trace(ROOT_MODULE,FormatText("WS_ORGCTL_5", m_FactionOrganizings.size()));

	if(m_FactionOrganizings.size() > 0)
		BeginTimer();

	//! �뻹�ڵȴ���GS��������
	SendFactionDataToGS(m_setWaitFacSocket);
	m_setWaitFacSocket.clear();
}

//! ��Ӧ����ɾ���Ļظ�
VOID COrganizingCtrl::OnDelFactionFinishMsg(const CGUID &AimGuid, BOOL bIsSysOpt)
{
	CFaction* pFaction = GetFactionOrganizing(AimGuid);
	if (NULL != pFaction)
	{
		if(bIsSysOpt)
			GetLogicLogInterface()->LogT150_faction_log("ϵͳ", NULL_GUID, pFaction, 1);
		else
		{
			CPlayer *pPlayer = GetGame().GetMapPlayer(pFaction->GetMasterID());
			if(NULL != pPlayer)
				GetLogicLogInterface()->LogT150_faction_log(pPlayer->GetName(), pPlayer->GetExID(), pFaction, 1);
			else
				GetLogicLogInterface()->LogT150_faction_log("δ�ҵ������ߣ�", NULL_GUID, pFaction, 1);
		}

		pFaction->Disband();
		m_FactionOrganizings.erase(AimGuid);

		for (map<CGUID, CGUID>::iterator ite = m_mapAllApplicant.begin(); ite != m_mapAllApplicant.end(); )
		{
			if(ite->second == AimGuid)
				ite = m_mapAllApplicant.erase(ite);
			else
				++ite;
		}

		SAFE_DELETE(pFaction);
	}
		
	if(m_FactionOrganizings.size() == 0)
		EndTimer();
}

//! ��ӦGS�Ĺ��ᴴ������
VOID COrganizingCtrl::OnGsCreateFaction (CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;

	CMessage msg(MSG_W2S_ORGSYS_CreateFaction_Re);
	DBWriteSet DbWriteSet;
	msg.GetDBWriteSet(DbWriteSet);

	DbWriteSet.AddToByteArray(PlayerGuid);

	if(NULL_GUID != FindPlayerFacionID(PlayerGuid))
	{
		DbWriteSet.AddToByteArray(PLAYER_OPT_FAILED);
		DbWriteSet.AddToByteArray((LONG)eFOEI_Err_HadFaction);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	//! ��֤�»����ƣ���ֹͬ���������»�ͬʱ���󴴽���
	char szNewFactionName[MAXSIZE_FactionName] = {0},szPCard[MAX_PronounceCharNum];
	pMsg->GetStr(szNewFactionName, MAXSIZE_FactionName);
    pMsg->GetStr(szPCard, MAX_PronounceCharNum);
	for (map<CGUID, CFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		if (0 == stricmp(ite->second->GetName(), szNewFactionName))
		{
			DbWriteSet.AddToByteArray(PLAYER_OPT_FAILED);
			DbWriteSet.AddToByteArray((LONG)eFOEI_Err_ReName);
			msg.SendToSocket(pMsg->GetSocketID());
			return;
		}
	}

	CGUID NewFactionGuid;
	CGUID::CreateGUID(NewFactionGuid);

	CFaction *pFaction = new CFaction(NewFactionGuid, szNewFactionName, pPlayer,szPCard);
	if (NULL != pFaction)
	{
		if(AddFactionOrganizing(pFaction))
		{
			DbWriteSet.AddToByteArray(PLAYER_OPT_SUCCEED);
			pFaction->CodeToMsgSet(DbWriteSet);

			msg.SendToAllGS();

			pFaction->Insert();

			map<CGUID, CGUID>::iterator ite = m_mapAllApplicant.find(PlayerGuid);
			if(m_mapAllApplicant.end() != ite)
			{
				CFaction *pApplyFaction = GetFactionOrganizing(ite->second);
				assert(pApplyFaction);
				if(NULL != pApplyFaction)
				{
					pApplyFaction->DelApplicant(PlayerGuid);
				}
				m_mapAllApplicant.erase(ite);
			}

			BeginTimer();
			GetLogicLogInterface()->LogT150_faction_log(pPlayer->GetName(), pPlayer->GetExID(), pFaction, 0);
			return;
		}
		else
		{
			SAFE_DELETE(pFaction);
		}
	}

	DbWriteSet.AddToByteArray(PLAYER_OPT_FAILED);
	DbWriteSet.AddToByteArray((LONG)eFOEI_CoreServerError);
	msg.SendToSocket(pMsg->GetSocketID());
}

//! ��ӦGS�Ĺ�����������
VOID COrganizingCtrl::OnGsUpgradeFaction(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	DWORD dwUpgradeType = pMsg->GetDWord();
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	LONG lRe = UpgradeFac_ByCondition(pFaction, dwUpgradeType);
	if(eFOEI_NotErr != lRe)
	{
		CMessage msg(MSG_W2S_Faction_LvlChange_Notify);
		msg.Add(dwUpgradeType);
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add(lRe);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	pFaction->SetLvl(pFaction->GetLvl(dwUpgradeType) + 1, dwUpgradeType);
}

//! ��ӦGS��ɾ����������
VOID COrganizingCtrl::OnDelFaction(const CGUID &DelFacGuid, BOOL bIsSysOpt)
{
	if(GetGame().GetEntityManager()->CreateDelFactionSession(DelFacGuid))
		OnDelFactionFinishMsg(DelFacGuid, bIsSysOpt);
	else
	{
		char szGUID[128] = {0};
		DelFacGuid.tostring(szGUID);
// 		char szTMP[256] = {0};
// 		sprintf(szTMP, "��ɢ���[%s]ʧ�ܣ�", szGUID);
// 		PutStringToFile("Organize", szTMP);

        Log4c::Warn(ROOT_MODULE,"��ɢ���[%s]ʧ�ܣ�", szGUID);
	}
}

//! ��Ӧ����������
VOID COrganizingCtrl::OnGsFacInvitePlayer(CMessage *pMsg)
{
	CGUID InviterGuid;
	char szAimName[MAX_MEMBER_NAME_SIZE] = {0};

	pMsg->GetGUID(InviterGuid);
	pMsg->GetStr(szAimName, MAX_MEMBER_NAME_SIZE);

	CPlayer *pInviter = GetGame().GetMapPlayer(InviterGuid);
	if (NULL == pInviter)
		return;

	CFaction *pFaction = GetFactionOrganizing(pInviter->GetFactionID());
	if (NULL == pFaction)
		return;

	CMessage msg(MSG_W2S_Faction_InvitePlayer_Re);

	//! �ж��Ƿ�������Ϊ�������ߡ�
	if(0 == strcmp(szAimName, GlobalSetup::GetSetup()->szHideName))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOET_Err_AimHideName);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	CPlayer *pAim = GetGame().GetMapPlayer(szAimName);
	if (NULL == pAim)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_AimAbsent);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	if( pAim->GetHideFlag() == 1 && !GetGame().PlayerIsGM(pInviter)
		&& (pAim->GetTeamID() != pInviter->GetTeamID() || pAim->GetTeamID()==NULL_GUID || pInviter->GetTeamID()==NULL_GUID) ) // ������
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOET_Err_AimHideName);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	//! ����
	if ((LONG)(pAim->GetCountry()) != pFaction->GetCountry())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_Country);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	if (NULL_GUID != pAim->GetFactionID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_AimHasFac);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	if(pFaction->GetMemberNum() >= WSOrganizingParam::getInstance().GetLevelSetup(pFaction->GetLvl(eUT_FactionLevel))->lMaxMemNum)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_MemberFull);
		msg.Add(InviterGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	msg.Add(PLAYER_OPT_SUCCEED);
	msg.Add(InviterGuid);
	msg.Add(pInviter->GetName());
	msg.Add(pAim->GetExID());
	msg.Add(pFaction->GetExID());
	msg.SendToSocket(pAim->GetGsSocket());
}

//! ��Ӧ�����Ҷ���
VOID COrganizingCtrl::OnGsFacAddMember(CMessage *pMsg)
{
	CGUID InviterGuid;
	pMsg->GetGUID(InviterGuid);
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if(NULL == pPlayer) return;
	if(NULL_GUID != FindPlayerFacionID(PlayerGuid))
	{
		CMessage msg(MSG_W2S_Faction_InvitePlayer_Re);
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_HadFaction);
		msg.Add(PlayerGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	if(pFaction->GetMemberNum() >= WSOrganizingParam::getInstance().GetLevelSetup(pFaction->GetLvl(eUT_FactionLevel))->lMaxMemNum)
	{
		CMessage msg(MSG_W2S_Faction_InvitePlayer_Re);
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_MemberFull);
		msg.Add(PlayerGuid);
		msg.SendToSocket(pMsg->GetSocketID());
		return;
	}

	if(pFaction->AddMeber(pPlayer, InviterGuid))
	{
		map<CGUID, CGUID>::iterator ite = m_mapAllApplicant.find(pPlayer->GetExID());
		if(m_mapAllApplicant.end() != ite)
		{
			CFaction *pTmpFaction = GetFactionOrganizing(ite->second);
			if(NULL != pTmpFaction)
			{
				pTmpFaction->DelApplicant(pPlayer->GetExID());
			}
			m_mapAllApplicant.erase(ite);
		}
	}
}

//! ��Ӧ���ùٽ�����
VOID COrganizingCtrl::OnGsFacSetJobName(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	LONG lJobLvl = pMsg->GetLong();
	char szNewName[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szNewName, MAX_MEMBER_NAME_SIZE);

	pFaction->SetJobName(lJobLvl, szNewName);
}

//! ��Ӧ���ùٽ�Ȩ��
VOID COrganizingCtrl::OnGsFacSetJobPurview(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	LONG lJobLvl = pMsg->GetLong();
	LONG lPurview = pMsg->GetLong();

	pFaction->SetJobPurview(lJobLvl, lPurview);
}

//! ��Ӧ��Ա�˳�
VOID COrganizingCtrl::OnGsFacQuit(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	pFaction->Exit(PlayerGuid);
}

//! ��Ӧ���ù���
VOID COrganizingCtrl::OnGsSetPronounce(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;

	CFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction) return;

	char szPronounce[MAX_PronounceCharNum] = {0};
	pMsg->GetStr(szPronounce, MAX_PronounceCharNum);

	pFaction->Pronounce(pPlayer, szPronounce);
}

//! ��Ӧ���ñ���
VOID COrganizingCtrl::OnGsSetLeaveword(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL == pPlayer) return;

	CFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction) return;

	char szPronounce[MAX_PronounceCharNum] = {0};
	pMsg->GetStr(szPronounce, MAX_PronounceCharNum);

	pFaction->Leaveword(pPlayer, szPronounce);
}

//! ��Ӧ������Ա
VOID COrganizingCtrl::OnGsFire(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID FireGuid;
	pMsg->GetGUID(FireGuid);
	CGUID BeFireGuid;
	pMsg->GetGUID(BeFireGuid);

	pFaction->FireOut(FireGuid, BeFireGuid);
}


//! ���ó�Աְ��
VOID COrganizingCtrl::OnGsSetMemberJob(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID MemberGuid;
	pMsg->GetGUID(MemberGuid);
	LONG lJobLvl = pMsg->GetLong();

	pFaction->SetJobLvl(MemberGuid, lJobLvl);
}

//! ���ó�Ա�ƺ�
VOID COrganizingCtrl::OnGsSetMemberTitle(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID OptGuid;
	pMsg->GetGUID(OptGuid);
	CGUID AimGuid;
	pMsg->GetGUID(AimGuid);
	char szTitle[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szTitle, MAX_MEMBER_NAME_SIZE);

	pFaction->SetMemberTitle(OptGuid, AimGuid, szTitle);
}

//! ��λ
VOID COrganizingCtrl::OnGsSetNewMaster(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID NewMasterGuid;
	pMsg->GetGUID(NewMasterGuid);

	pFaction->Demise(pFaction->GetMasterID(), NewMasterGuid);
}

//! ����Ӧ����
VOID COrganizingCtrl::OnGsCheckRequisition(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	BOOL bAgree = pMsg->GetLong();
	CGUID AimGuid;
	pMsg->GetGUID(AimGuid);
	CGUID OpterGuid;
	pMsg->GetGUID(OpterGuid);

	//CPlayer *pPlayer = GetGame().GetMapPlayer(AimGuid);
	CPlayer *pOpter = GetGame().GetMapPlayer(OpterGuid);
	const tagFacApplyPerson *pFacApplyPerson = pFaction->FindApplicant(AimGuid);
	
	if(NULL != pFacApplyPerson)
	{
		if(bAgree)
		{	
			if(pFaction->GetMemberNum() >= WSOrganizingParam::getInstance().GetLevelSetup(pFaction->GetLvl(eUT_FactionLevel))->lMaxMemNum)
			{
				CMessage msg(MSG_W2S_Faction_InvitePlayer_Re);
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_MemberFull);
				msg.Add(OpterGuid);
				msg.SendToSocket(pMsg->GetSocketID());
				return;
			}
			if(NULL_GUID != FindPlayerFacionID(AimGuid))
			{
				CMessage msg(MSG_W2S_Faction_InvitePlayer_Re);
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_AimHasFac);
				msg.Add(OpterGuid);
				msg.SendToSocket(pMsg->GetSocketID());
				return;
			}
			pFaction->AddMeber(*pFacApplyPerson, OpterGuid);
			GetLogicLogInterface()->LogT160_faction_join_leave_log(pFacApplyPerson->szName, &(pFacApplyPerson->PlayerGuid), pFaction, 0);
		}
		else
		{
			pFaction->DelApplicant(AimGuid);
			GetLogicLogInterface()->LogT160_faction_join_leave_log(pFacApplyPerson->szName, &(pFacApplyPerson->PlayerGuid), pFaction, 1);
		}
	}
	
	pFaction->DelApplicant(AimGuid);
	m_mapAllApplicant.erase(AimGuid);
}

//! �������
VOID COrganizingCtrl::OnGsJoinFac(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID ApplicantGuid;
	pMsg->GetGUID(ApplicantGuid);

	CPlayer *pPlayer = GetGame().GetMapPlayer(ApplicantGuid);
	if (NULL == pPlayer) return;
	if (NULL_GUID != pPlayer->GetFactionID()) return;

	BOOL bIsJoin = pMsg->GetLong();

	if (bIsJoin)
	{
		if(m_mapAllApplicant.end() != m_mapAllApplicant.find(ApplicantGuid))
		{
			CMessage msg(MSG_W2S_Faction_Join_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_ReJoin);
			msg.Add(ApplicantGuid);
			msg.SendToSocket(pMsg->GetSocketID());
			return;
		}

		if(pFaction->AddApplicant(ApplicantGuid))
		{
			m_mapAllApplicant[ApplicantGuid] = pFaction->GetExID();
		}
	}
	else
	{
		map<CGUID, CGUID>::iterator ite = m_mapAllApplicant.find(ApplicantGuid);
		if(m_mapAllApplicant.end() != ite)
		{
			pFaction = GetFactionOrganizing(ite->second);
			if (NULL != pFaction)
			{
				if(NULL != pFaction->DelApplicant(ApplicantGuid))
				{
					m_mapAllApplicant.erase(ite);
				}
			}
		}
	}
}

//! �ϴ�ͼ��
VOID COrganizingCtrl::OnGsUpLoadIcon(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	LONG lDataSize = pMsg->GetLong();
	assert(MAX_ICON_SIZE >= lDataSize);
	if(MAX_ICON_SIZE < lDataSize) return;
	vector<BYTE> vIconData(lDataSize);
	pMsg->GetEx(&vIconData[0], lDataSize);

	if(pFaction->SetIcon(&vIconData[0], (long)vIconData.size()))
	{
		CMessage msg(MSG_W2S_Faction_IconChange_Notify);
		msg.Add(FactionGuid);
		msg.Add(PlayerGuid);
		msg.Add((LONG)vIconData.size());
		msg.AddEx(&vIconData[0], (long)vIconData.size());
		msg.SendToAllGS();
	}
}

//! ��һ���������������
BOOL COrganizingCtrl::AddFactionOrganizing(CFaction *pFaction)
{
	if (NULL != pFaction)
	{
		assert(pFaction->GetExID() != NULL_GUID);
		if(NULL_GUID == pFaction->GetExID())
			return FALSE;
		assert(m_FactionOrganizings.end() == m_FactionOrganizings.find(pFaction->GetExID()));
		if(m_FactionOrganizings.end() != m_FactionOrganizings.find(pFaction->GetExID()))
			return FALSE;

		m_FactionOrganizings[pFaction->GetExID()] = pFaction;
		pFaction->AddApplicantToMap(m_mapAllApplicant);
		pFaction->ResetMember();
		return TRUE;

	}
	return FALSE;
}

CFaction* COrganizingCtrl::GetFactionOrganizing(const CGUID &FacGuid)
{
	map<CGUID,CFaction*>::iterator ite = m_FactionOrganizings.find(FacGuid);
	if (m_FactionOrganizings.end() != ite)
	{
		return ite->second;
	}
	return NULL;
}

//! �ҳ�һ����ҵĹ���ID
const CGUID& COrganizingCtrl::FindPlayerFacionID(const CGUID &PlayerGuid)
{
	for (map<CGUID,CFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		if (NULL_GUID != ite->second->IsMember(PlayerGuid))
		{
			return ite->first;
		}
	}
	return NULL_GUID;
}

//! ���ͳ�ʼ��GS������Ϣ
VOID COrganizingCtrl::SendFactionDataToGS(LONG lGsSocket)
{
	assert(m_bFacDataFull);

	CMessage msg(MSG_W2S_ORGSYS_InitGsFaction);
	DBWriteSet DbWriteSet;
	msg.GetDBWriteSet(DbWriteSet);

	//! ������Ϣ
	if(!WSOrganizingParam::getInstance().CodeToMsgSet(DbWriteSet))
		return;

	//! ��������
	DbWriteSet.AddToByteArray((LONG)m_FactionOrganizings.size());
	for (map<CGUID, CFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		ite->second->CodeToMsgSet(DbWriteSet);
	}
	
	msg.SendToSocket(lGsSocket, false);
}

VOID COrganizingCtrl::SendFactionDataToGS(set<LONG> &setSocket)
{
	assert(m_bFacDataFull);

	CMessage msg(MSG_W2S_ORGSYS_InitGsFaction);
	DBWriteSet DbWriteSet;
	msg.GetDBWriteSet(DbWriteSet);

	//! ������Ϣ
	if(!WSOrganizingParam::getInstance().CodeToMsgSet(DbWriteSet))
		return;

	//! ��������
	DbWriteSet.AddToByteArray((LONG)m_FactionOrganizings.size());
	for (map<CGUID, CFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		ite->second->CodeToMsgSet(DbWriteSet);
	}

	msg.SendToAllGS();
	/*
	for (set<LONG>::iterator ite = setSocket.begin(); ite != setSocket.end(); ++ite)
	{
		msg.SendToSocket(*ite);
	}
	*/
}

//! ����������������
eFactionOptErrInfo COrganizingCtrl::TestUpgradeFacCondition(CFaction *pFaction, DWORD dwUpgradeType)
{
	assert(NULL != pFaction);
	const VecCondition*	pVecCondition = WSOrganizingParam::getInstance().GetUpgradeVecCondition(dwUpgradeType, pFaction->GetLvl(dwUpgradeType) + 1);
	if (NULL == pVecCondition)
		return eFOEI_Err_SetupLoading;
	const VecCondition &vCondition = *pVecCondition;

	for (size_t i = 0; i < vCondition.size(); ++i)
	{
		switch(vCondition[i].lConditionType)
		{
		case eUCT_FacRes:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(vCondition[i].lValue > pFaction->GetRes())
					return eFOEI_FacRes_NotEnough;
			}
			break;
		case eUCT_FacExp:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(vCondition[i].lValue > pFaction->GetExp())
					return eFOEI_FacExp_NotEnough;
			}
			break;
		case eUCT_Money:
		case eUCT_LeaderLevel:
		case eUCT_FacLevel:
			break;
			//ռ��ݳ��������
		case eUCT_BaseNum:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(vCondition[i].lValue > pFaction->GetBaseNum())
					return eFOET_Err_BaseNum;
			}
			break;
		default:
			{
				return eFOEI_Err_Unknown;
			}
		}
	}

	return eFOEI_NotErr;
}
//! �������ᣬ�����õ�����
eFactionOptErrInfo COrganizingCtrl::UpgradeFac_ByCondition(CFaction *pFaction, DWORD dwUpgradeType)
{
	assert(NULL != pFaction);
	eFactionOptErrInfo testRe = TestUpgradeFacCondition(pFaction, dwUpgradeType);
	if(eFOEI_NotErr != testRe)
		return testRe;

	const VecCondition*	pVecCondition = WSOrganizingParam::getInstance().GetUpgradeVecCondition(dwUpgradeType, pFaction->GetLvl(dwUpgradeType) + 1);
	if (NULL == pVecCondition)
		return eFOEI_Err_SetupLoading;
	const VecCondition &vCondition = *pVecCondition;

	for (size_t i = 0; i < vCondition.size(); ++i)
	{
		switch(vCondition[i].lConditionType)
		{
		case eUCT_FacRes:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(!pFaction->AddRes(-(vCondition[i].lValue)))
					return eFOEI_FacRes_NotEnough;
			}
			break;
		case eUCT_FacExp:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(pFaction->GetExp() < vCondition[i].lValue)
					return eFOEI_FacExp_NotEnough;
			}
			break;
		case eUCT_Money:
		case eUCT_LeaderLevel:
		case eUCT_FacLevel:
			break;
			//ռ����������
		case eUCT_BaseNum:
			{
				if(NULL == pFaction)
					return eFOEI_Err_NotInFaction;
				if(vCondition[i].lValue > pFaction->GetBaseNum())
					return eFOET_Err_BaseNum;
			}
			break;
		default:
			{
				return eFOEI_Err_Unknown; 
			}
		}
	}
	return eFOEI_NotErr;
}

//! ��Ӧ�µ�һ�쵽��
void COrganizingCtrl::OnNewDay(DWORD dwTime)
{
	if(m_bFacDataFull)
	{
		list<CGUID> listFacGuid;
		for (map<CGUID, CFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
		{
			if(CFaction::ImmediatelyDisband == ite->second->OnValidate(dwTime))
			{
				listFacGuid.push_back(ite->first);
			}
		}

		for (list<CGUID>::iterator ite = listFacGuid.begin(); listFacGuid.end() != ite; ++ite)
		{
			OnDelFaction(*ite, TRUE);
		}
	}
}

//! ��ʱ����ʱ��Ӧ
long COrganizingCtrl::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	if(m_lExpTimerID == timerid)
	{
		CMessage msg(MSG_W2S_Faction_BoonRun_Notify);
		msg.SendToAllGS();
	}
	
	return 0;
}
//! ����ʱ����ȡ���ĵ���
void COrganizingCtrl::OnTimerCancel(DWORD timerid,const void* var)
{
}

//!
VOID COrganizingCtrl::BeginTimer(VOID)
{
	if(0 == m_lExpTimerID)
		m_lExpTimerID = GetTimerQueue().Schedule(this, NULL, 5000, ADD_EXP_SPACE_TIME);
}
//!
VOID COrganizingCtrl::EndTimer(VOID)
{
	if(0 != m_lExpTimerID)
	{
		GetTimerQueue().Cancel(m_lExpTimerID);
		m_lExpTimerID = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//! ���ͳ�ʼ��GS������Ϣ
VOID COrganizingCtrl::SendUnionDataToGS(LONG lGsSocket)
{
	assert(m_bUnionDataFull);

	vector<BYTE> vData;

	//! ��������
	_AddToByteArray(&vData, (LONG)m_mapUnion.size());
	for (map<CGUID, CUnion*>::iterator ite = m_mapUnion.begin(); ite != m_mapUnion.end(); ++ite)
	{
		ite->second->AddToByteArray(vData);
	}

	CMessage msg(MSG_W2S_Union_InitGs);
	msg.Add((LONG)vData.size());
	msg.AddEx(&vData[0], (long)vData.size());
	msg.SendToSocket(lGsSocket, false);
}

VOID COrganizingCtrl::SendUnionDataToGS(set<LONG> &setSocket)
{
	assert(m_bUnionDataFull);

	vector<BYTE> vData;

	//! ��������
	_AddToByteArray(&vData, (LONG)m_mapUnion.size());
	for (map<CGUID, CUnion*>::iterator ite = m_mapUnion.begin(); ite != m_mapUnion.end(); ++ite)
	{
		ite->second->AddToByteArray(vData);
	}

	CMessage msg(MSG_W2S_Union_InitGs);
	msg.Add((LONG)vData.size());
	msg.AddEx(&vData[0], (long)vData.size());

	for (set<LONG>::iterator ite = setSocket.begin(); ite != setSocket.end(); ++ite)
	{
		msg.SendToSocket(*ite);
	}
}

//! ��һ�����˶���������
BOOL COrganizingCtrl::AddUnionOrganizing(CUnion *pUnion)
{
	if (NULL != pUnion)
	{
		assert(pUnion->GetExID() != NULL_GUID);
		if(NULL_GUID == pUnion->GetExID())
			return FALSE;
		assert(m_mapUnion.end() == m_mapUnion.find(pUnion->GetExID()));
		if(m_mapUnion.end() != m_mapUnion.find(pUnion->GetExID()))
			return FALSE;

		m_mapUnion[pUnion->GetExID()] = pUnion;
		return TRUE;
	}
	return FALSE;
}

CUnion* COrganizingCtrl::GetUnionOrganizing(const CGUID &UnionGuid)
{
	map<CGUID,CUnion*>::iterator ite = m_mapUnion.find(UnionGuid);
	if (m_mapUnion.end() != ite)
	{
		return ite->second;
	}
	return NULL;
}

//! ��Ӧ��ʼ���������
VOID COrganizingCtrl::OnInitUnionEndMsg(VOID)
{
	m_bUnionDataFull = TRUE;
	GetGame().SetInitUnionLoadFlag(true);
	Log4c::Trace(ROOT_MODULE,GetText("WS_ORGCTL_6"));

	SendUnionDataToGS(m_setWaitUnionSocket);
	m_setWaitUnionSocket.clear();
}

//! ��Ӧ��Ҵ�������
VOID COrganizingCtrl::OnGsCreateUnion(CPlayer *pPlayer, const char *pUnionName)
{
	CMessage msg(MSG_W2S_Union_Create_Notify);
	msg.Add(pPlayer->GetExID());

	//! ��֤���ƣ���ֹͬ��������������ͬʱ���󴴽���
	for (map<CGUID, CUnion*>::iterator ite = m_mapUnion.begin(); ite != m_mapUnion.end(); ++ite)
	{
		if (0 == stricmp(ite->second->GetName(), pUnionName))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_ReName);
			msg.SendToSocket(pPlayer->GetGsSocket(), false);
			return;
		}
	}

	CFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_NotInFaction);
		msg.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}
	if(pFaction->GetMasterID() != pPlayer->GetExID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_Purview_Noenough);
		msg.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}

	CGUID NewUnionGuid;
	CGUID::CreateGUID(NewUnionGuid);
	CUnion *pUnion = new CUnion(NewUnionGuid, pUnionName, pFaction);

	if(NULL != pUnion)
	{
		AddUnionOrganizing(pUnion);

		vector<BYTE> vUnionData;
		pUnion->AddToByteArray(vUnionData);

		msg.Add(PLAYER_OPT_SUCCEED);
		msg.Add((LONG)vUnionData.size());
		msg.AddEx(&vUnionData[0], (long)vUnionData.size());
		msg.SendToAllGS();

		pUnion->Insert();

		return;
	}

	msg.Add(PLAYER_OPT_FAILED);
	msg.Add((LONG)eFOEI_CoreServerError);
	msg.SendToSocket(pPlayer->GetGsSocket(), false);
}

//! ��Ӧɾ������ɹ�
VOID COrganizingCtrl::OnDelUnionFinishMsg(const CGUID &UnionGuid)
{
	CUnion *pUnion = GetUnionOrganizing(UnionGuid);
	if(NULL != pUnion)
	{
		pUnion->Disband();

		CMessage msg(MSG_W2S_Union_Disband_Notify);
		msg.Add(UnionGuid);
		msg.SendToAllGS();

		SAFE_DELETE(pUnion);
		m_mapUnion.erase(UnionGuid);
	}
}

//! ��Ӧ��ҽ�ɢ����
VOID COrganizingCtrl::OnGsDisbandUnion(CPlayer *pPlayer)
{
	assert(NULL != pPlayer);
	CUnion *pUnion = GetUnionOrganizing(pPlayer->GetUnionID());
	if(NULL != pUnion)
	{
		//! ս��״���ж�

		//! �����ж�
		if(pUnion->GetMasterID() == pPlayer->GetExID())
			GetGame().GetEntityManager()->CreateDelFactionSession(pUnion->GetExID());
	}	
}

//! ��Ӧ�˳���������
VOID COrganizingCtrl::OnGsQuitUnion(const CGUID &FactionGuid, const CGUID &UnionGuid)
{
	CFaction	*pFaction	= GetFactionOrganizing(FactionGuid);
	CUnion		*pUnion		= GetUnionOrganizing(UnionGuid);
	if(NULL == pFaction || NULL == pUnion)
		return;

	if(pUnion->DelMember(FactionGuid))
	{
		pFaction->SetSuperior(NULL);

		CMessage msg(MSG_W2S_Union_Quit_Notify);
		msg.Add(FactionGuid);
		msg.Add(UnionGuid);
		msg.SendToAllGS();
	}
}

//! ��Ӧ������������
VOID COrganizingCtrl::OnGsKickUnion(const CGUID &OptGuid, const CGUID &AimGuid, const CGUID &UnionGuid)
{
	CFaction	*pAimFaction	= GetFactionOrganizing(AimGuid);
	CUnion		*pUnion		= GetUnionOrganizing(UnionGuid);
	if(NULL == pAimFaction || NULL == pUnion)
		return;

	if(pUnion->DelMember(AimGuid))
	{
		pAimFaction->SetSuperior(NULL);

		CMessage msg(MSG_W2S_Union_Kick_Notify);
		msg.Add(OptGuid);
		msg.Add(AimGuid);
		msg.Add(UnionGuid);
		msg.SendToAllGS();
	}
}

//! ��λ
VOID COrganizingCtrl::OnGsSetChairmanUnion(const CGUID &MasterGuid, const CGUID &AimFacGuid)
{
	CPlayer *pPlayer = GetGame().GetMapPlayer(MasterGuid);
	if(NULL == pPlayer)
		return;

	CUnion *pUnion = GetUnionOrganizing(pPlayer->GetUnionID());
	if(NULL == pUnion)
		return;

	if(pUnion->GetMasterID() != MasterGuid)
		return;
	if(pUnion->ChangeMaster(AimFacGuid))
	{
		CMessage msg(MSG_S2W_Union_SetChairman_Notify);
		msg.Add(pUnion->GetExID());
		msg.Add(AimFacGuid);
		msg.Add(pUnion->GetMasterName());
		msg.SendToAllGS();
	}
}

//! ����Ȩ��
VOID COrganizingCtrl::OnGsSetPurviewUnion(const CGUID &MasterGuid, const CGUID &AimFacGuid, DWORD dwPurview)
{
	CPlayer *pPlayer = GetGame().GetMapPlayer(MasterGuid);
	if(NULL == pPlayer)
		return;

	CUnion *pUnion = GetUnionOrganizing(pPlayer->GetUnionID());
	if(NULL == pUnion)
		return;

	if(pUnion->GetMasterID() != MasterGuid)
		return;

	if(pUnion->SetPurview(AimFacGuid, dwPurview))
	{
		CMessage msg(MSG_S2W_Union_SetPurview_Notify);
		msg.Add(pUnion->GetExID());
		msg.Add(AimFacGuid);
		msg.Add(dwPurview);
		msg.SendToAllGS();
	}
}

//! �����������
VOID COrganizingCtrl::OnGsUnionInvit(const CGUID &MasterGuid, const char *pAimPlayerName)
{
	assert(NULL != pAimPlayerName);
	
	CPlayer *pPlayer = GetGame().GetMapPlayer(MasterGuid);
	CPlayer	*pAimPlayer = GetGame().GetMapPlayer(pAimPlayerName);
	if(NULL == pPlayer || NULL == pAimPlayer)
		return;

	CUnion *pUnion = GetUnionOrganizing(pPlayer->GetUnionID());
	if(NULL == pUnion)
		return;

	if(pUnion->GetMasterID() != MasterGuid)
		return;

	CMessage msgRe(MSG_S2W_Union_Invit_Re);

	if(NULL_GUID != pAimPlayer->GetUnionID())
	{
		msgRe.Add(PLAYER_OPT_FAILED);
		msgRe.Add((LONG)eFOEI_Err_AimHadUnion);
		msgRe.Add(pPlayer->GetExID());
		msgRe.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}

	CFaction *pFaction = GetFactionOrganizing(pAimPlayer->GetFactionID());
	if (NULL == pFaction)
	{
		msgRe.Add(PLAYER_OPT_FAILED);
		msgRe.Add((LONG)eFOEI_Err_NotInFaction);
		msgRe.Add(pPlayer->GetExID());
		msgRe.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}

	if (pFaction->GetMasterID() != pAimPlayer->GetExID())
	{
		msgRe.Add(PLAYER_OPT_FAILED);
		msgRe.Add((LONG)eFOEI_Err_NotInFaction);
		msgRe.Add(pPlayer->GetExID());
		msgRe.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}

	if(!m_UnionSession.CreateSession(tagUnionSession(pPlayer->GetExID(), pAimPlayer->GetExID(), pUnion->GetExID())))
	{
		msgRe.Add(PLAYER_OPT_FAILED);
		msgRe.Add((LONG)eFOEI_Err_ContinualInvite);
		msgRe.Add(pPlayer->GetExID());
		msgRe.SendToSocket(pPlayer->GetGsSocket(), false);
		return;
	}

	msgRe.SetType(MSG_S2W_Union_TransferInvit);
	msgRe.Add(pPlayer->GetExID());
	msgRe.Add(pPlayer->GetName());
	msgRe.Add(pUnion->GetExID());
	msgRe.Add(pUnion->GetName());
	msgRe.Add(pAimPlayer->GetExID());
	msgRe.SendToSocket(pAimPlayer->GetGsSocket(), false);
}

//! Ӧ������
VOID COrganizingCtrl::OnGsUnionAnswerInvit(const CGUID &InviterGuid, const CGUID &RespondentGuid, const CGUID & UnionGuid, LONG lResult)
{
	CPlayer *pRespondent = GetGame().GetMapPlayer(RespondentGuid);
	if(NULL == pRespondent)
		return;
	CFaction *pFaction = GetFactionOrganizing(pRespondent->GetFactionID());
	if (NULL == pFaction)
		return;

	if(!m_UnionSession.TestSession(tagUnionSession(InviterGuid, RespondentGuid, UnionGuid)))
	{
		/*AppFrame::GetText("GS_TEAM_AimHadTeam")*/
		SendInfoToOneClient(RespondentGuid, "�Ự�ѳ�ʱ��" );
		return;
	}

	CPlayer *pInviter = GetGame().GetMapPlayer(InviterGuid);

	if(0 == lResult)
	{
		if(NULL != pInviter)
		{
			CMessage msgRe(MSG_S2W_Union_Invit_Re);
			msgRe.Add(PLAYER_OPT_FAILED);
			msgRe.Add((LONG)eFOEI_Err_AimRefuse);
			msgRe.Add(pInviter->GetExID());
			msgRe.SendToSocket(pInviter->GetGsSocket(), false);
		}
		return;
	}

	CUnion *pUnionGuid = GetUnionOrganizing(UnionGuid);
	if(NULL == pUnionGuid)
	{
		SendInfoToOneClient(RespondentGuid, "Ŀ�������ѽ�ɢ��" );
		return;
	}

	if(pUnionGuid->AddNewMember(pFaction))
	{
		//! ֪ͨ��GS����³�Ա
		CMessage msg(MSG_S2W_Union_NewMemberJoin);
		msg.Add(pFaction->GetExID());
		msg.Add(UnionGuid);
		msg.SendToAllGS();

		//! �����߻����ߵĻ������ظ���Ϣ
		
		if(NULL != pInviter)
		{
			CMessage msgRe(MSG_S2W_Union_Invit_Re);
			msgRe.Add(PLAYER_OPT_SUCCEED);
			msgRe.Add(pFaction->GetName());
			msgRe.Add(InviterGuid);
			msgRe.SendToSocket(pInviter->GetGsSocket(), false);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! ��ָ����ҷ���һ����Ϣ
VOID COrganizingCtrl::SendInfoToOneClient(const CGUID& PlayerGuid, const char* szInfo)
{
	GetOrganiCtrl()->SendOrgaInfoToClient(PlayerGuid,string(szInfo),string("��ʾ"));

}

//! �õ�ָ���Ķ����ָ��
WSTeam*	COrganizingCtrl::GetGameTeamByID(const CGUID& TeamID)
{
	map<CGUID,WSTeam*>::iterator ite = m_TeamOrganizings.find(TeamID);
	if (NULL_GUID != TeamID && m_TeamOrganizings.end() != ite)
	{
		return ite->second;
	}
	return NULL;
}

long COrganizingCtrl::GetGameTeamNum(VOID)
{
	return (long)m_TeamOrganizings.size();
}

VOID COrganizingCtrl::SendTeamSetupToGS(LONG lGsSocket)
{
	CMessage msg(MSG_W2S_TEAM_SetupInit);
	msg.AddEx(&(WSTeam::s_tagDefaultSetup), sizeof(WSTeam::tagDefaultSetup));
	msg.SendToSocket(lGsSocket);
	//msg.SendToAllGS();
}

//! ����ȫ�ֶ������ݵ�GS
VOID COrganizingCtrl::SendTeamDataToGS(LONG lGsSocket)
{
	CMessage msg(MSG_W2S_TEAM_DataInit);
	vector<BYTE> vData;
	_AddToByteArray(&vData, (LONG)m_TeamOrganizings.size());

	map<CGUID,WSTeam*>::iterator ite = m_TeamOrganizings.begin();
	for (; ite != m_TeamOrganizings.end(); ite++)
	{
		ite->second->AddToByteArray(vData);
	}
	msg.Add((LONG)vData.size());
	msg.AddEx(&vData[0], (long)vData.size());

	msg.SendToSocket(lGsSocket);
}




//! ��Ӧ��ҷ����������
VOID COrganizingCtrl::OnQueryJoin(CMessage *pMsg)
{
	CGUID InviteeID;	
	pMsg->GetGUID(InviteeID);
	CGUID CaptainID;
	pMsg->GetGUID(CaptainID);

	if(InviteeID == CaptainID)
	{
		return;
	}

	CPlayer* pInvitee = GetGame().GetMapPlayer( InviteeID );
	CPlayer* pCaptain = GetGame().GetMapPlayer( CaptainID );

	if (NULL == pCaptain || NULL == pInvitee)
	{
		return;
	}

	const CGUID& InviteeTeamID = pInvitee->GetTeamID();
	const CGUID& CaptainTeamID = pCaptain->GetTeamID();

	if(NULL_GUID != InviteeTeamID)//�����������ж���
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_AimHadTeam") );
		return;
	}

	if (NULL_GUID != CaptainTeamID)//�������ж���
	{
		WSTeam* pTeam = GetGameTeamByID(CaptainTeamID);
		if (NULL == pTeam)
		{
			//! GS��Ϣ���͹���˵��������GS�ϴ��ڣ�����WS�����ҵ��ö��飬���ǿ�ƽ�ɢ
			CMessage msg(MSG_W2S_TEAM_Disband);
			msg.Add(CaptainTeamID);
			msg.SendToAllGS();
			return;
		}
		if( NULL_GUID == pTeam->IsMaster(CaptainID) )//���Ƕӳ�
		{
			SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_OperatorNotMaster") );
			return;
		}

		if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
		{
			SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_TeamIsFull") );
			return;
		}
	}
	
	tagPlayerTeamSession PlayerTeamSession;
	PlayerTeamSession.SourceID = CaptainID;
	PlayerTeamSession.TargetID = InviteeID;

	if(!m_InviteSession.CreateSession(PlayerTeamSession))
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_PleaseWait") );
		return;
	}

	CMessage msg( MSG_W2S_TEAM_QUERY_JOIN );
	msg.Add( InviteeID );
	msg.Add( CaptainID );
	msg.Add( pCaptain->GetName() );
	msg.SendToGS( GetGame().GetGameServerNumber_ByPlayerID(InviteeID) );
}

//! ��Ӧ��ҷ����������
VOID COrganizingCtrl::OnQueryJoinByName(CPlayer* pInvitee, CPlayer* pCaptain)
{
	if (NULL == pCaptain || NULL == pInvitee)
	{
		return;
	}

	if(pInvitee->GetExID() == pCaptain->GetExID())
	{
		return;
	}

	const CGUID& InviteeTeamID = pInvitee->GetTeamID();
	const CGUID& CaptainTeamID = pCaptain->GetTeamID();

	const CGUID& CaptainID = pCaptain->GetExID();
	const CGUID& InviteeID = pInvitee->GetExID();

	if(NULL_GUID != InviteeTeamID)//�����������ж���
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_AimHadTeam"));
		return;
	}

	if (NULL_GUID != CaptainTeamID)//�������ж���
	{
		WSTeam* pTeam = GetGameTeamByID(CaptainTeamID);
		if (NULL == pTeam)
		{
			//! GS��Ϣ���͹���˵��������GS�ϴ��ڣ�����WS�����ҵ��ö��飬���ǿ�ƽ�ɢ
			CMessage msg(MSG_W2S_TEAM_Disband);
			msg.Add(CaptainTeamID);
			msg.SendToAllGS();
			return;
		}
		if( NULL_GUID == pTeam->IsMaster(CaptainID) )//���Ƕӳ�
		{
			SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_OperatorNotMaster") );
			return;
		}

		if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
		{
			SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_TeamIsFull"));
			return;
		}
	}
	
	tagPlayerTeamSession PlayerTeamSession;
	PlayerTeamSession.SourceID = CaptainID;
	PlayerTeamSession.TargetID = InviteeID;

	if(!m_InviteSession.CreateSession(PlayerTeamSession))
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_PleaseWait") );
		return;
	}	

	CMessage msg( MSG_W2S_TEAM_QUERY_JOIN );
	msg.Add( InviteeID );
	msg.Add( CaptainID );
	msg.Add( pCaptain->GetName() );
	msg.SendToGS( GetGame().GetGameServerNumber_ByPlayerID(InviteeID) );
}


//long COrganizingCtrl::GetNewTeamID(VOID)
//{
//	do
//	{
//		long lTeamID = timeGetTime();
//		if (m_TeamOrganizings.end() == m_TeamOrganizings.find(lTeamID))
//		{
//			return lTeamID;
//		}
//	}
//	while(true);
//}

//! ��Ӧ��Ҵ��������
VOID COrganizingCtrl::OnAnswerJoin(CMessage *pMsg)
{
	CGUID InviteeID;
	pMsg->GetGUID(InviteeID);
	CGUID CaptainID;
	pMsg->GetGUID(CaptainID);
	LONG lJoin		= pMsg->GetByte();

	tagPlayerTeamSession PlayerTeamSession;
	PlayerTeamSession.SourceID = CaptainID;
	PlayerTeamSession.TargetID = InviteeID;

	if(!m_InviteSession.TestSession(PlayerTeamSession))
	{
		SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_OutTime"));
		return;
	}

	CPlayer *pInvitee = GetGame().GetMapPlayer( InviteeID );
	if(NULL == pInvitee) return;

	if(1 == lJoin) //ͬ��
	{
		if (NULL_GUID != pInvitee->GetTeamID())
		{
			SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_YouHadTeam"));
			return;
		}

		CPlayer *pPlayer = GetGame().GetMapPlayer( CaptainID );
		if(NULL == pPlayer) return;

		WSTeam *pTeam = (WSTeam*)GetGameTeamByID(pPlayer->GetTeamID());;

		if(NULL == pTeam) //û�ж���
		{
			CGUID TeamID;
			CGUID::CreateGUID(TeamID);//GetNewTeamID();
			pTeam = CreateTeam(TeamID, CaptainID, InviteeID);
			if(NULL == pTeam)
			{
				SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_CreateFailedAndTautology"));
				SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_JoinFailedAndTautology"));
			}
		}
		else //�ж���
		{
			if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
			{
				SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_TeamIsFull"));
				return;
			}
			tagTime Time;
			if(!pTeam->DoJoin(CaptainID, InviteeID, lJoin, Time))
			{
				SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_JoinFailedAndTautology"));
				SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_JoinFailedAndTautology"));
			}
			else
			{
				if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
				{
					CloseRecruit(CaptainID);
				}
			}
		}
	}
	else if(0 == lJoin)//��ͬ��
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_AimRefuse"));
	}
	else if(2 == lJoin)//! ��æ
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_Busyness"));
	}
	else if(3 == lJoin)//! û�п���
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_NotOpenTeam"));
	}
	else if(4 == lJoin)//! Ŀ������
	{
		SendInfoToOneClient(CaptainID, AppFrame::GetText("GS_TEAM_AimIsDead"));
	}
}

//! ��Ӧ�ı�ӳ�
VOID	COrganizingCtrl::OnChangLeader(CMessage *pMsg)
{
	CGUID guid;
	pMsg->GetGUID(guid);
	CGUID CaptainID;
	pMsg->GetGUID(CaptainID);

	CPlayer* pInvitee = GetGame().GetMapPlayer( guid );
	CPlayer* pCaptain = GetGame().GetMapPlayer( CaptainID );

	if (NULL == pCaptain || NULL == pInvitee)
	{
		return;
	}

	const CGUID& TeamID = pInvitee->GetTeamID();
	const CGUID& CaptainTeamID = pCaptain->GetTeamID();

	if(TeamID != CaptainTeamID || guid == CaptainID)//���鲻ͬ������ߺͲ���������ͬһ����
	{
		return;
	}

	WSTeam *pTeam = (WSTeam*)GetGameTeamByID(CaptainTeamID);
	if (NULL == pTeam)
	{
		//! GS��Ϣ���͹���˵��������GS�ϴ��ڣ�����WS�����ҵ��ö��飬���ǿ�ƽ�ɢ
		CMessage msg(MSG_W2S_TEAM_Disband);
		msg.Add(CaptainTeamID);
		msg.SendToAllGS();
		return;
	}
	if (NULL_GUID == pTeam->IsMaster(CaptainID))//���Ƕӳ�
	{
		return;
	}

	pTeam->Demise(CaptainID, guid);
	CloseRecruit(CaptainID);
}

//! ��Ӧ����
VOID COrganizingCtrl::OnKickPlayer(CMessage *pMsg)
{
	CGUID guid;
	pMsg->GetGUID(guid);
	CGUID CaptainID;
	pMsg->GetGUID(CaptainID);

	CPlayer* pCaptain = GetGame().GetMapPlayer( CaptainID );

	if (NULL == pCaptain )
	{
		return;
	}

	const CGUID& TeamID = pCaptain->GetTeamID();
	//long lCaptainTeamID = pCaptain->GetTeamID();

	if(TeamID == NULL_GUID)
	{
		return;
	}

	WSTeam *pTeam = (WSTeam*)GetGameTeamByID(TeamID);
	if (NULL == pTeam )
	{
		//! GS��Ϣ���͹���˵��������GS�ϴ��ڣ�����WS�����ҵ��ö��飬���ǿ�ƽ�ɢ
		CMessage msg(MSG_W2S_TEAM_Disband);
		msg.Add(TeamID);
		msg.SendToAllGS();
		return;
	}

	if(!pTeam->PlayerInTeam(guid))
	{
		return;
	}

	if(CaptainID == guid)//���Լ�=�����˳�
	{
		if(pTeam->GetMemberNum() > 2)
		{
			if (NULL_GUID != pTeam->IsMaster(CaptainID))//�Ƕӳ������������¶ӳ�
			{
				const CGUID& MastterID = pTeam->SetNewMastter();
				if (NULL_GUID != MastterID)
				{
					CMessage msgMC(MSG_W2S_TEAM_MastterChange);
					msgMC.Add(pTeam->GetExID());
					msgMC.Add(MastterID);
					msgMC.SendToAllGS();
				}
				else//���˶ӳ������������
				{
					//�ӳ��˳�����ɢ����
					DisbandTeam(pTeam);
					return;
				}
			}
		}
	}
	else
	{
		if (NULL_GUID == pTeam->IsMaster(CaptainID))//���Ƕӳ���������������
		{
			return;
		}
	}

	pTeam->Exit(guid);

	//����������
	if (pTeam->GetMemberNum() < 2)
	{
		DisbandTeam(pTeam);
	}
}


VOID COrganizingCtrl::OnSetGoods(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	long eSet = pMsg->GetLong();

	CPlayer *pPlayer = GetGame().GetMapPlayer( PlayerGuid );

	if(NULL == pPlayer)
	{
		return;
	}

	WSTeam *pTeam = (WSTeam*)GetGameTeamByID(pPlayer->GetTeamID());

	if (NULL == pTeam)
	{
		//! GS��Ϣ���͹���˵��������GS�ϴ��ڣ�����WS�����ҵ��ö��飬���ǿ�ƽ�ɢ
		CMessage msg(MSG_W2S_TEAM_Disband);
		msg.Add(pPlayer->GetTeamID());
		msg.SendToAllGS();
		return;
	}

	if (pTeam->GetMasterID() != PlayerGuid)
	{
		return;
	}

	pTeam->SetGoodsAllot(eSet);
}

VOID COrganizingCtrl::OnRecruit(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	long lFlag = pMsg->GetLong();

	if (0 == lFlag)
	{
		m_RecruitingPlayer.erase(PlayerGuid);
	}
	else
	{
		CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
		if (NULL == pPlayer)
		{
			return;
		}
		WSTeam *pTeam = (WSTeam*)(GetOrganiCtrl()->GetGameTeamByID(pPlayer->GetTeamID()));
		if (NULL != pTeam)
		{
			if (pTeam->GetMasterID() != PlayerGuid)
			{
				return;
			}
			if (WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
			{
				return;
			}
		}

		
		m_RecruitingPlayer[PlayerGuid] = pPlayer;
	}

	pMsg->SetType(MSG_W2S_TEAM_RECRUIT);
	pMsg->SendToGS(pMsg->GetGSID());
}

//! �ر���ҵ���ļ
VOID COrganizingCtrl::CloseRecruit(const CGUID &PlayerGuid)
{
	CMessage msg(MSG_W2S_TEAM_RECRUIT);
	msg.Add(PlayerGuid);
	msg.Add(0L);
	msg.SendToAllGS();
}

VOID COrganizingCtrl::OnBeRecruited(CMessage *pMsg)
{
	CGUID InviteeID;
	pMsg->GetGUID(InviteeID);
	CGUID CaptainID;
	pMsg->GetGUID(CaptainID);
	BOOL bJoin		= pMsg->GetByte();

	CPlayer *pPlayer = GetGame().GetMapPlayer( CaptainID );
	if(NULL == pPlayer) return;
	WSTeam *pTeam = (WSTeam*)GetGameTeamByID(pPlayer->GetTeamID());;

	if(NULL == pTeam) //û�ж���
	{
		CGUID TeamID;
		CGUID::CreateGUID(TeamID);//GetNewTeamID();
		pTeam = CreateTeam(TeamID, CaptainID, InviteeID);
		if(NULL == pTeam)
		{
			SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_CreateFailedAndTautology"));
		}
	}
	else //�ж���
	{
		if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
		{
			SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_TeamIsFull"));
			return;
		}
		tagTime Time;
		if(!pTeam->DoJoin(CaptainID, InviteeID, bJoin, Time))
		{
			SendInfoToOneClient(InviteeID, AppFrame::GetText("GS_TEAM_JoinFailedAndTautology"));
		}
		else
		{
			if(WSTeam::s_tagDefaultSetup.lMaxMemberNum <= pTeam->GetMemberNum())
			{
				CloseRecruit(CaptainID);
			}
		}
	}
}

//! ��Ӧ����
VOID COrganizingCtrl::OnChat(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL != pPlayer)
	{
		pMsg->SetType(MSG_W2S_TEAM_CHAT);
		pMsg->SendToSocket(pPlayer->GetGsSocket());
	}
}

//! ��Ӧ��Ա��ϸ��Ϣת��
VOID COrganizingCtrl::OnTeamMemberAllIdioinfo(CMessage *pMsg)
{
	tagMemberIdioinfo MemberIdioinfo;
	pMsg->GetEx(&MemberIdioinfo, sizeof(tagMemberIdioinfo));
	CGUID guid, ReceiverGuid;
	pMsg->GetGUID(guid);
	pMsg->GetGUID(ReceiverGuid);

	CPlayer *pReceiver = GetGame().GetMapPlayer(ReceiverGuid);
	if(NULL != pReceiver)
	{
		pMsg->SetType(MSG_W2S_TEAM_MemberAllIdioinfo);
		pMsg->SendToSocket(pReceiver->GetGsSocket());
	}
}

//! ��Ӧ��Աλ����Ϣת��
VOID COrganizingCtrl::OnTeamMemberPos(CMessage *pMsg)
{
	LONG lDataSize = pMsg->GetLong();
	vector<BYTE> vData(lDataSize);
	pMsg->GetEx(&vData[0], lDataSize);

	LONG lReceiverNum = pMsg->GetLong();
	set<LONG> setReceiverGsSocket;
	for (LONG i = 0; i < lReceiverNum; ++i)
	{
		CGUID ReceiverGuid;
		pMsg->GetGUID(ReceiverGuid);
		CPlayer *pReceiver = GetGame().GetMapPlayer(ReceiverGuid);
		if(NULL != pReceiver)
		{
			setReceiverGsSocket.insert(pReceiver->GetGsSocket());
		}
	}

	pMsg->SetType(MSG_W2S_TEAM_MemberPos);
	for (set<LONG>::iterator ite = setReceiverGsSocket.begin(); ite != setReceiverGsSocket.end(); ++ite)
	{
		pMsg->SendToSocket(*ite);
	}
}

bool COrganizingCtrl::PlayerJoinTeam(const CGUID& PlayerGuid, const CGUID& TeamGuid)
{
	/*if( m_MemberIDvsTeamID.end() == m_MemberIDvsTeamID.find(PlayerGuid) )
	{
		return false;
	}*/
	if (NULL_GUID == TeamGuid)
	{
		return false;
	}
	m_MemberIDvsTeamID[PlayerGuid] = TeamGuid;
	return true;
}

VOID COrganizingCtrl::PlayerLeaveTeam(const CGUID& PlayerGuid)
{
	m_MemberIDvsTeamID.erase(PlayerGuid);
	CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	if (NULL != pPlayer)
	{
		pPlayer->SetTeamID(NULL_GUID);
        GetInst(PhaseMap).ClearPlayerID(PlayerGuid);
	}
}

const CGUID& COrganizingCtrl::FindPlayerTeamID(const CGUID& PlayerGuid)
{
	if (m_MemberIDvsTeamID.end() == m_MemberIDvsTeamID.find(PlayerGuid))
	{
		return NULL_GUID;
	}
	return m_MemberIDvsTeamID[PlayerGuid];
}

//! �������飬����������б�
WSTeam*	COrganizingCtrl::CreateTeam(const CGUID& TeamGuid, const CGUID& MastterGuid, const CGUID& MemberGuid)
{
	WSTeam* pTeam = new WSTeam();
	if(pTeam->Initial(TeamGuid, MastterGuid, MemberGuid))
	{
		map<CGUID, WSTeam*>::iterator ite = m_TeamOrganizings.find(TeamGuid);
		if(m_TeamOrganizings.end() != ite)
			DisbandTeam(ite->second);
		m_TeamOrganizings[TeamGuid] = pTeam;
		return pTeam;
	}

	delete pTeam;
	return NULL;
}

//------------------------

//! ��ɢ����
VOID COrganizingCtrl::DisbandTeam(WSTeam *pTeam)
{
	assert(NULL != pTeam);
	const CGUID& TeamID = pTeam->GetExID();

	for (map<CGUID, CGUID>::iterator ite = m_MemberIDvsTeamID.begin(); ite != m_MemberIDvsTeamID.end(); )
	{
		if(ite->second == TeamID)
		{
			ite = m_MemberIDvsTeamID.erase(ite);
			continue;
		}
		++ite;
	}

	pTeam->Disband(TeamID);

	m_TeamOrganizings[TeamID] = NULL;
	m_TeamOrganizings.erase(TeamID);

	delete pTeam;
	
}