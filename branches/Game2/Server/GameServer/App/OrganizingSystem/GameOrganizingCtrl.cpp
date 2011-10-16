#include "StdAfx.h"
#include "GameOrganizingCtrl.h"

#include "GameFaction.h"
#include "GameUnion.h"
#include "GSTeam.h"
#include "../Player.h"
#include "../Goods/CGoodsFactory.h"
#include "../../../../public/Common/OrganizingParam.h"
#include "../../../../Public/ServerPublic/Server/Common/WordsFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GameOrganizingCtrl* GameOrganizingCtrl::instance = NULL;

GameOrganizingCtrl::GameOrganizingCtrl(void)
:m_bFacDataFull(FALSE), m_PlayerOptSession(10 * 60, TRUE), m_PlayerInviteSession(60, FALSE)
{
	m_TeamOrganizings.clear();
}

GameOrganizingCtrl::~GameOrganizingCtrl(void)
{
}

GameOrganizingCtrl* GameOrganizingCtrl::getInstance()
{
	if(instance == NULL)
	{
		instance = OBJ_CREATE(GameOrganizingCtrl);
	}
	return instance;
}

void GameOrganizingCtrl::Release(void)
{
	if(instance != NULL)
	{
		//�ͷ�����
		for (map<CGUID,GameUnion*>::iterator ite = instance->m_mapUnion.begin(); ite != instance->m_mapUnion.end(); ++ite)
		{
			OBJ_RELEASE(GameUnion, (*ite).second);
		}
		instance->m_mapUnion.clear();

		//�ͷ��л�
		for(map<CGUID,GameFaction*>::iterator ite = instance->m_FactionOrganizings.begin(); ite != instance->m_FactionOrganizings.end(); ++ite)
		{
			OBJ_RELEASE(GameFaction, (*ite).second);
		}
		instance->m_FactionOrganizings.clear();

		//�ͷŶ���
		for(map<CGUID,GSTeam*>::iterator ite = instance->m_TeamOrganizings.begin(); ite != instance->m_TeamOrganizings.end(); ++ite)
		{
			OBJ_RELEASE(GSTeam, (*ite).second);
		}
		instance->m_TeamOrganizings.clear();

		OBJ_RELEASE(GameOrganizingCtrl, instance);
		instance = NULL;
	}

}

//! ��Ӧ����ϵͳ�ĳ�ʼ����Ϣ
void GameOrganizingCtrl::OnInitGsFaction(CMessage *pMsg)
{
	DBReadSet DbReadSet;
	pMsg->GetDBReadSet(DbReadSet);

	//! ��������
	COrganizingParam::getInstance().DecodeFromMsgSet(DbReadSet);

	//! ��������
	m_mapAllApplicant.clear();

	//�ͷ��л�
	for(map<CGUID,GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		OBJ_RELEASE(GameFaction, (*ite).second);
	}
	m_FactionOrganizings.clear();

	LONG lFactionNum = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lFactionNum; ++i)
	{
		GameFaction *pFaction = OBJ_CREATE(GameFaction);
		if(NULL == pFaction) return;
		pFaction->DecodeFromMsgSet(DbReadSet, &m_mapAllApplicant);
		assert(NULL_GUID != pFaction->GetExID());
		assert(m_FactionOrganizings.end() == m_FactionOrganizings.find(pFaction->GetExID()));
		m_FactionOrganizings[pFaction->GetExID()] = pFaction;
	}

	//! ����ͼ��
//	UpdateIconData();

	m_bFacDataFull = TRUE;

	SendInitFacDataToPlayer();
}

////! ����ͼ����ʱ����
//void GameOrganizingCtrl::UpdateIconData(void)
//{
//	m_vCurrIconData.clear();
//	_AddToByteArray(&m_vCurrIconData, (LONG)m_FactionOrganizings.size());
//	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
//	{
//		ite->second->AddIconData_ToArray(&m_vCurrIconData);
//	}
//	CCrc32Static::DataCrc32(&m_vCurrIconData[0], m_vCurrIconData.size(), m_dwCurrIconCRC32Code);
//}

//! ��Ӧ��ʼ�����֪ͨ
void GameOrganizingCtrl::SendInitFacDataToPlayer(const CGUID &PlayerGuid)
{
	
	
	if (m_bFacDataFull)
	{
		CMessage msg(MSG_S2C_ORGSYS_InitOrgSys_Notify);
		vector<BYTE> vData;
		//! ������Ϣ
		if(!COrganizingParam::getInstance().AddToByteArray(vData))
			return;

		msg.Add((LONG)vData.size());
		msg.AddEx(&vData[0], (long)vData.size());

		//! ���￪ʼ����ֱ����ӵ���Ϣ
		//! ��������
		//msg.Add((LONG)m_FactionOrganizings.size());
		//for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
		//{
		//	ite->second->AddPublicData_ToMsgBack(&msg);
		//}

		msg.SendToPlayer(PlayerGuid, false);
	}
	else
	{
		m_setsetWaitingClient.insert(PlayerGuid);
	}
}

void GameOrganizingCtrl::SendInitFacDataToPlayer(void)
{
	assert(m_bFacDataFull);
	
	CMessage msg(MSG_S2C_ORGSYS_InitOrgSys_Notify);
	vector<BYTE> vData;
	//! ������Ϣ
	if(!COrganizingParam::getInstance().AddToByteArray(vData))
		return;

	//! ���
	_AddToByteArray(&vData, (LONG)m_FactionOrganizings.size());
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		ite->second->AddIconData_ToArray(&vData);
	}

	msg.Add((LONG)vData.size());
	msg.AddEx(&vData[0],(long)vData.size());

	//! ���￪ʼ����ֱ����ӵ���Ϣ
	//! ��������
	msg.Add((LONG)m_FactionOrganizings.size());
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		ite->second->AddPublicData_ToMsgBack(&msg);
	}

	for (set<CGUID>::iterator ite = m_setsetWaitingClient.begin(); ite != m_setsetWaitingClient.end(); ++ite)
	{
		msg.SendToPlayer(*ite);
	}
	m_setsetWaitingClient.clear();
}

//! ��Ӧ���ᴴ���ظ�
//! ����һ��WS�Ĺ㲥��Ϣ
void GameOrganizingCtrl::OnCreateFactionRe(CMessage *pMsg)
{
	DBReadSet DbReadSet;
	pMsg->GetDBReadSet(DbReadSet);

	CGUID PlayerGuid;
	DbReadSet.GetBufferFromByteArray(PlayerGuid);

	LONG lResult = DbReadSet.GetLongFromByteArray();

	CMessage msg(MSG_S2C_ORGSYS_CreateFaction_Re);
	msg.Add(lResult);
	
	if (PLAYER_OPT_FAILED == lResult)
	{
		LONG lErrInfo = DbReadSet.GetLongFromByteArray();
		msg.Add(lErrInfo);
		
		if(eFOEI_Err_ReName != lErrInfo)
		{
			PlayerFacOptEnd(PlayerGuid, ePFOT_Create);
		}

		msg.SendToPlayer(PlayerGuid, false);
		return;
	}

	PlayerFacOptEnd(PlayerGuid, ePFOT_Create);

	//! �����л�
	GameFaction *pFaction = OBJ_CREATE(GameFaction);
	if(NULL == pFaction) return;
	LONG lPos = 0;
	pFaction->DecodeFromMsgSet(DbReadSet);
	assert(m_FactionOrganizings.end() == m_FactionOrganizings.find(pFaction->GetExID()));
	m_FactionOrganizings[pFaction->GetExID()] = pFaction;

	//! ���ö��󹤻�ID
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	m_mapWaitCreatePlayer.erase(PlayerGuid);
	if (NULL != pPlayer)
	{
		pPlayer->SetFactionID(pFaction->GetExID());
		
	}
    msg.Add( pFaction->GetExID() );
    msg.SendToPlayer(PlayerGuid, false);

	//! ֪ͨ�������
	RadiateFactionPublic(pFaction);
}

//! ��Ӧ����ɾ��
void GameOrganizingCtrl::OnWsDelFaction(CMessage *pMsg)
{
	CGUID AimGuid;
	pMsg->GetGUID(AimGuid);


	GameFaction *pFaction = NULL;
	map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.find(AimGuid);
	if(ite != m_FactionOrganizings.end())
	{
		pFaction = ite->second;
		m_FactionOrganizings.erase(AimGuid);
	}
	else
		assert(false);

	if (NULL != pFaction)
	{
		pFaction->Disband();
		OBJ_RELEASE(GameFaction, pFaction);
	}
	else
		assert(false);
	

	for (map<CGUID, CGUID>::iterator ite = m_mapAllApplicant.begin(); ite != m_mapAllApplicant.end(); )
	{
		if(ite->second == AimGuid)
		{
			CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
			msg.Add(PLAYER_OPT_SUCCEED);
			msg.Add((LONG)FALSE);
			msg.Add(AimGuid);
			msg.SendToPlayer(ite->first);

			ite = m_mapAllApplicant.erase(ite);
		}
		else
			++ite;
	}

	CMessage msg(MSG_S2C_ORGSYS_DisbandFaction_Notify);
	msg.Add(AimGuid);
	msg.SendAll();
}

//! ��ӦWS����ظ�
void GameOrganizingCtrl::OnWsInvitePlayerRe(CMessage *pMsg)
{
	LONG lResult = pMsg->GetLong();
	if (PLAYER_OPT_FAILED == lResult)
	{
		pMsg->GetLong();
		CGUID InviterGuid;
		pMsg->GetGUID(InviterGuid);
		pMsg->SetType(MSG_S2C_Faction_InvitePlayer_Re);
		pMsg->SendToPlayer(InviterGuid);
	}
	else
	{
		tagFacInviteSession FacInviteSession;

		pMsg->GetGUID(FacInviteSession.InviterGuid);
		char szInviterName[MAX_MEMBER_NAME_SIZE] = {0};
		pMsg->GetStr(szInviterName, MAX_MEMBER_NAME_SIZE);
		pMsg->GetGUID(FacInviteSession.AimGuid);
		pMsg->GetGUID(FacInviteSession.FactionGuid);

		GameFaction *pFaction	= GetOrganiCtrl()->GetFactionOrganizing(FacInviteSession.FactionGuid);
		if (NULL != pFaction)
		{
			if(m_PlayerInviteSession.CreateSession(FacInviteSession))
			{
				CMessage msg(MSG_S2C_Faction_InvitePlayer_Notify);
				msg.Add(FacInviteSession.InviterGuid);
				msg.Add(szInviterName);
				msg.Add(FacInviteSession.FactionGuid);
				msg.Add(pFaction->GetName());
				msg.SendToPlayer(FacInviteSession.AimGuid, false);
			}
		}
	}
}

//! ��ӦWS��ӳ�Ա֪ͨ
void GameOrganizingCtrl::OnWsAddMember(CMessage *pMsg)
{
	tagFacMemInfo FacMemInfo;
	pMsg->GetEx(&FacMemInfo, sizeof(tagFacMemInfo));
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	CGUID InviterGuid;
	pMsg->GetGUID(InviterGuid);

	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	assert(NULL != pFaction);
	if (NULL == pFaction) return;
	
	pFaction->AddMember(FacMemInfo);

	if (NULL_GUID != InviterGuid)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(InviterGuid);
		if(NULL != pPlayer)
		{
			CMessage msg(MSG_S2C_Faction_InvitePlayer_Re);
			msg.Add(PLAYER_OPT_SUCCEED);
			msg.Add(FacMemInfo.szName);
			msg.SendToPlayer(InviterGuid, false);
		}	
	}

	//! ��ӳ�Ա֪ͨ
	CMessage msgMem(MSG_S2C_Faction_AddMember_Notify);
	msgMem.AddEx(&FacMemInfo, sizeof(tagFacMemInfo));
    msgMem.Add(FactionGuid);
	//! ���͸����д��˹������ĳ�Ա
	pFaction->SendToAllConcern(msgMem);

	//! ֪ͨ�¼�����
	msgMem.SendToPlayer(FacMemInfo.MemberGuid, false);
}

//! ��ӦWSְ����������
void GameOrganizingCtrl::OnWsSetJobName(CMessage *pMsg)
{
	LONG lJobLvl = pMsg->GetLong();
	char szNewName[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szNewName, MAX_MEMBER_NAME_SIZE);

	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	if(pFaction->SetJobName(lJobLvl, szNewName))
	{
		pMsg->SetType(MSG_S2C_ORGSYS_SetJobName_Re);
		pFaction->SendToAllConcern(*pMsg);
	}
	
}

//! ��ӦWSְ��Ȩ������
void GameOrganizingCtrl::OnWsSetJobPurview(CMessage *pMsg)
{
	LONG lJobLvl = pMsg->GetLong();
	LONG lPurview = pMsg->GetLong();

	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	pFaction->SetJobPurview(lJobLvl, lPurview);
	pMsg->SetType(MSG_S2C_ORGSYS_SetJobPurview_Notify);
	pFaction->SendToAll(*pMsg);

	CMessage msg(MSG_S2C_ORGSYS_SetJobPurview_Re);
	msg.Add(PLAYER_OPT_SUCCEED);
	msg.SendToPlayer(pFaction->GetMaster(), false);
}


//! ��Ӧ��Ա�˳�֪ͨ
void GameOrganizingCtrl::OnWsMemberQuit(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction)
	{
		assert(false);
		return;
	}

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	
	const tagFacMemInfo *pFacMemInfo =  pFaction->GetMemberData(PlayerGuid);
	if(NULL == pFacMemInfo)
	{
		assert(false);
		return;
	}
	
	CMessage msgNotify(MSG_S2C_ORGSYS_Quit_Notify);
	msgNotify.Add(FactionGuid);
	msgNotify.Add(PlayerGuid);
	msgNotify.Add(pFacMemInfo->szName);
	pFaction->SendToAllConcern(msgNotify);

	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	if(NULL != pPlayer)
	{
		pPlayer->SetFactionID(NULL_GUID);
		pPlayer->SetUnionID(NULL_GUID);
		pPlayer->SetFacMemInfo(NULL);
		msgNotify.SendToAround(pPlayer);
	}

	pFaction->DelMember(PlayerGuid);
}

//! ��Ӧ����༭֪ͨ
void GameOrganizingCtrl::OnWsEditPronounce(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	tagOrgPronounceWord OrgPronounceWord;
	pMsg->GetEx(&OrgPronounceWord, sizeof(tagOrgPronounceWord));

	pFaction->SetPronounce(OrgPronounceWord);

	CPlayer *pPlayer = GetGame()->FindPlayer(OrgPronounceWord.MemberGuid);
	if(NULL == pPlayer) return;

	CMessage msg(MSG_S2C_ORGSYS_SetPlacard_Re);
	msg.Add(PLAYER_OPT_SUCCEED);
	msg.AddEx(&OrgPronounceWord, sizeof(tagOrgPronounceWord));
	msg.SendToPlayer(OrgPronounceWord.MemberGuid, false);

	CMessage msgPro(MSG_S2C_Faction_Placard_Notify);
	pFaction->AddPronounce_ToMsgBack(&msgPro);
	pFaction->SendToAllConcern(msgPro);
}

//! ��Ӧ�����༭֪ͨ
void GameOrganizingCtrl::OnWsEditLeaveword(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	tagOrgPronounceWord OrgPronounceWord;
	pMsg->GetEx(&OrgPronounceWord, sizeof(tagOrgPronounceWord));

	pFaction->SetLeaveword(OrgPronounceWord);

	CPlayer *pPlayer = GetGame()->FindPlayer(OrgPronounceWord.MemberGuid);
	if(NULL == pPlayer) return;

	CMessage msg(MSG_S2C_ORGSYS_SetLeaveword_Re);
	msg.Add(PLAYER_OPT_SUCCEED);
	msg.AddEx(&OrgPronounceWord, sizeof(tagOrgPronounceWord));
	msg.SendToPlayer(OrgPronounceWord.MemberGuid, false);

	CMessage msgPro(MSG_S2C_Faction_Leaveword_Notify);
	pFaction->AddLeaveWord_ToMsgBack(&msgPro);
	pFaction->SendToAllConcern(msgPro);
}

//! ��Ӧ������Ա֪ͨ
void GameOrganizingCtrl::OnWsFire(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID FireID;
	pMsg->GetGUID(FireID);
	CGUID BeFiredID;
	pMsg->GetGUID(BeFiredID);

	const tagFacMemInfo* pFacMemInfo = pFaction->GetMemberData(BeFiredID);
	if(NULL == pFacMemInfo)
	{
		assert(false);
		return;
	}

	//! ֪ͨ�����˹������ĳ�Ա
	CMessage msg(MSG_S2C_ORGSYS_Kick_Notify);
	msg.Add(FactionGuid);
	msg.Add(BeFiredID);
	msg.Add(pFacMemInfo->szName);
	pFaction->SendToAllConcern(msg);

	//! ֪ͨ������
	CPlayer *pPlayer = GetGame()->FindPlayer(BeFiredID);
	if(NULL != pPlayer)
	{
		pPlayer->SetFactionID(NULL_GUID);
		pPlayer->SetUnionID(NULL_GUID);
		pPlayer->SetFacMemInfo(NULL);

		msg.SendToAround(pPlayer);
	}

	pFaction->DelMember(BeFiredID);
}

//! ���ó�Ա��ְ��
void GameOrganizingCtrl::OnWsSetMemJobLvl(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID MmeberGuid;
	pMsg->GetGUID(MmeberGuid);
	LONG lJobLvl = pMsg->GetLong();
	
	pFaction->SetMemJobLvl(MmeberGuid, lJobLvl);

	//! ֪ͨ�����ߣ��᳤��
	CPlayer *pPlayer = GetGame()->FindPlayer(pFaction->GetMaster());
	if(NULL != pPlayer)
	{
		CMessage msg(MSG_S2C_ORGSYS_SetMemberJob_Re);
		msg.Add(PLAYER_OPT_SUCCEED);
		msg.Add(MmeberGuid);
		msg.Add(lJobLvl);
		msg.SendToPlayer(pFaction->GetMaster(), false);
	}

	//! ֪ͨ�����˹������ĳ�Ա
	CMessage msgN(MSG_S2C_ORGSYS_SetMemberJob_Notify);
	msgN.Add(MmeberGuid);
	msgN.Add(lJobLvl);
	msgN.Add((LONG)pFaction->GetPurview(MmeberGuid));
	pFaction->SendToAllConcern(msgN);

	//! ֪ͨ��������
	CMessage msg(MSG_S2C_Faction_BaseData_Notify);
	pFaction->AddBaseData_ToMsgBack(&msg);
	msg.SendToPlayer(MmeberGuid, false);

	msgN.SendToPlayer(MmeberGuid, false);
}

//! ���ó�Ա�ĳƺ�
void GameOrganizingCtrl::OnWsSetMemberTitle(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID OptGuid;
	pMsg->GetGUID(OptGuid);
	CGUID AimGuid;
	pMsg->GetGUID(AimGuid);
	char szTitle[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szTitle, MAX_MEMBER_NAME_SIZE);

	if(pFaction->SetMemberTitle(AimGuid, szTitle))
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(OptGuid);
		if (NULL != pPlayer)
		{
			CMessage msg(MSG_S2C_Faction_SetMemberTitle_Re);
			msg.Add(PLAYER_OPT_SUCCEED);
			msg.Add(AimGuid);
			msg.Add(szTitle);
			msg.SendToPlayer(OptGuid, false);
		}
	}

	//! ֪ͨ�����˹������ĳ�Ա
	CMessage msgN(MSG_S2C_Faction_SetMemberTitle_Notify);
	msgN.Add(AimGuid);
	msgN.Add(szTitle);
	pFaction->SendToAllConcern(msgN);
}

//! ��Ա��ͼ֪ͨ
void GameOrganizingCtrl::OnWsMemberMap(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	char szMapName[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szMapName, MAX_MEMBER_NAME_SIZE);

	if(pFaction->SetMemberMap(PlayerGuid, szMapName))
	{
		CMessage msg(MSG_S2C_Faction_MemberMap_Notifye);
		msg.Add(PlayerGuid);
		msg.Add(szMapName);
		pFaction->SendToAllConcern(msg);
	}
}

//! ��Ա�ȼ�֪ͨ
void GameOrganizingCtrl::OnWsMemberLevel(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	LONG lLevel = pMsg->GetLong();

	if(pFaction->SetMemberLevel(PlayerGuid, lLevel))
	{
		CMessage msg(MSG_S2C_Faction_MemberLevel_Notifye);
		msg.Add(PlayerGuid);
		msg.Add(lLevel);
		pFaction->SendToAllConcern(msg);
	}
}

//! ��Ա����֪ͨ
void GameOrganizingCtrl::OnWsMemberHide(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	BYTE cHide = pMsg->GetByte();

	if(pFaction->SetMemberHide(PlayerGuid, cHide))
	{
		//CMessage msg(MSG_S2C_Faction_MemberLevel_Notifye);
		//msg.Add(PlayerGuid);
		//msg.Add(lLevel);
		//pFaction->SendToAllConcern(msg);
	}
}

//! ��Աְҵ֪ͨ
void GameOrganizingCtrl::OnWsMemberOccu(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	LONG lOccu = pMsg->GetLong();

	if(pFaction->SetMemberOccu(PlayerGuid, lOccu))
	{
		CMessage msg(MSG_S2C_Faction_MemberOccu_Notifye);
		msg.Add(PlayerGuid);
		msg.Add(lOccu);
		pFaction->SendToAllConcern(msg);
	}
}

//! �����»᳤
void GameOrganizingCtrl::OnWsSetMaster(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	tagFacMemInfo NewFacMemInfo;
	pMsg->GetEx(&NewFacMemInfo, sizeof(tagFacMemInfo));
	tagFacMemInfo OldFacMemInfo;
	pMsg->GetEx(&OldFacMemInfo, sizeof(tagFacMemInfo));

	pFaction->SetNewMaster(NewFacMemInfo, OldFacMemInfo);

	CMessage msg(MSG_S2C_Faction_BaseData_Notify);
	pFaction->AddBaseData_ToMsgBack(&msg);
	msg.SendToPlayer(NewFacMemInfo.MemberGuid);
	pFaction->SendToAllConcern(msg);

	CMessage msgN(MSG_S2C_Faction_SetChairman_Notify);
	msgN.Add(OldFacMemInfo.szName);
	msgN.Add(NewFacMemInfo.szName);
	pFaction->SendToAll(msgN);

	CPlayer *pPlayer = GetGame()->FindPlayer(OldFacMemInfo.MemberGuid);
	if (NULL != pPlayer)
	{
		CMessage msg(MSG_S2C_Faction_SetChairman_Re);
		msg.Add(PLAYER_OPT_SUCCEED);
		msg.SendToPlayer(OldFacMemInfo.MemberGuid, false);	
	}

}


//! ����״̬�仯
void GameOrganizingCtrl::OnWsMemOnlineChange(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID MemGuid;
	pMsg->GetGUID(MemGuid);
	
	pFaction->SetMemberLevel(MemGuid, pMsg->GetWord());
	pFaction->SetMemberOccu(MemGuid, pMsg->GetWord());

	char szName[MAX_MEMBER_NAME_SIZE] = {0};
	pMsg->GetStr(szName, MAX_MEMBER_NAME_SIZE);
	pFaction->SetMemberName(MemGuid, szName);
	BOOL bOnline = pMsg->GetLong();

	LONG lLastOnlineTime = 0;
	if(bOnline)
	{
		char szMapName[MAX_MEMBER_NAME_SIZE] = {0};
		pMsg->GetStr(szMapName, MAX_MEMBER_NAME_SIZE);
		pFaction->SetMemberMap(MemGuid, szMapName);
	}
	else
		lLastOnlineTime = pMsg->GetLong();

	if(pFaction->MemberOnlineChange(MemGuid, bOnline, lLastOnlineTime))
	{
		CMessage msg(MSG_S2C_Faction_MemberOnline_Notifye);
		msg.Add(MemGuid);
		msg.Add((LONG)bOnline);
		pFaction->SendToAllConcern(msg);
	}

	
	
}

//! ������Ա�䶯
void GameOrganizingCtrl::OnWsApplicantChange(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);

	BOOL bIsAdd = pMsg->GetLong();

	CGUID AimGuid_buf;
	if (bIsAdd)
	{
		tagFacApplyPerson FacApplyPerson;
		pMsg->GetEx(&FacApplyPerson, sizeof(tagFacApplyPerson));
		AimGuid_buf = FacApplyPerson.PlayerGuid;

		m_mapAllApplicant[FacApplyPerson.PlayerGuid] = FactionGuid;
		if(NULL != pFaction)
			pFaction->AddApplicant(FacApplyPerson);
	}
	else
	{
		CGUID AimGuid;
		pMsg->GetGUID(AimGuid);
		AimGuid_buf = AimGuid;

		m_mapAllApplicant.erase(AimGuid);
		if(NULL != pFaction)
			pFaction->DelApplicant(AimGuid);
	}

	CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
	msg.Add(PLAYER_OPT_SUCCEED);
	msg.Add((LONG)bIsAdd);
	msg.Add(FactionGuid);
	msg.SendToPlayer(AimGuid_buf, false);

	if(NULL != pFaction)
	{
		pMsg->SetType(MSG_S2C_ORGSYS_CheckRequisition_Notify);
		pFaction->SendToAllConcern(*pMsg);
	}
}

//! �л�ȼ��ı�
void GameOrganizingCtrl::OnWsFacLvlChange(CMessage *pMsg)
{
	DWORD	dwUpgradeType	= pMsg->GetDWord();
	LONG	lOptResult		= pMsg->GetLong();

	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	if(PLAYER_OPT_FAILED == lOptResult)
	{
		CPlayer *pMaster = GetGame()->FindPlayer(pFaction->GetMaster());
		if(NULL != pMaster)
		{
			BackRoll_UpgradeFac(pMaster, pFaction->GetLevel(dwUpgradeType), dwUpgradeType);

			LONG lOptErrInfo = pMsg->GetLong();
			CMessage msg(MSG_S2C_ORGSYS_UpgradeFaction_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add(lOptErrInfo);
			msg.Add(dwUpgradeType);
			msg.SendToPlayer(pMaster->GetExID(), false);
		}
		return;
	}

	LONG lLevel = pMsg->GetLong();

	pFaction->SetLevel(lLevel, dwUpgradeType);

	CMessage msg(MSG_S2C_ORGSYS_UpgradeFaction_Notify);
	msg.Add(lLevel);
	msg.Add(dwUpgradeType);
	pFaction->SendToAll(msg);
}

//! �л�ͼ��ı�
void GameOrganizingCtrl::OnWsIconChange(CMessage *pMsg)
{
	CGUID FactionGuid;
	pMsg->GetGUID(FactionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pFaction) return;

	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	LONG lDataSize = pMsg->GetLong();
	assert(MAX_ICON_SIZE >= lDataSize);
	if(MAX_ICON_SIZE < lDataSize) return;
	vector<BYTE> vIconData(lDataSize);
	pMsg->GetEx(&vIconData[0], lDataSize);

	CMessage msg(MSG_S2C_Faction_IconData_Re);
	if(pFaction->SetIcon(&vIconData[0],(long)vIconData.size()))
	{
		msg.Add(PLAYER_OPT_SUCCEED);

		//UpdateIconData();
		CMessage msgRadiate(MSG_S2C_Faction_IconData_Notify);
		//msgRadiate.Add(m_dwCurrIconCRC32Code);
		msgRadiate.Add(FactionGuid);
		msgRadiate.Add((LONG)vIconData.size());
		msgRadiate.AddEx(&vIconData[0],(long)vIconData.size());
		msgRadiate.SendAll();
	}
	else
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_Unknown);
	}
	msg.SendToPlayer(PlayerGuid, false);
}

//! ��Ӧִ�и���
void GameOrganizingCtrl::OnBoonRun(void)
{
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		ite->second->OnBoonRun();
	}
}

GameFaction*	GameOrganizingCtrl::GetFactionOrganizing(const CGUID& GameFactionID)
{
	if(GameFactionID == NULL_GUID) return NULL;

	map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.find(GameFactionID);
	if(ite != m_FactionOrganizings.end())
	{
		return ite->second;
	}
	return NULL;
}

//! �ҳ�һ����ҵĹ���ID
const CGUID& GameOrganizingCtrl::FindPlayerFacionID(const CGUID &PlayerGuid)
{
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		if (NULL_GUID != ite->second->IsMember(PlayerGuid))
		{
			return ite->first;
		}
	}
	return NULL_GUID;
}

//! ����һ����ҵĹ�����Ϣ
void GameOrganizingCtrl::UpdateFacInfo(CPlayer *pPlayer)
{
	GameFaction *pFac = GetFactionOrganizing(pPlayer->GetFactionID());
	if (NULL != pFac)
	{
		pFac->UpdateMemberFacInfo(pPlayer);
	}
}


//! ���һ���������ʱ�䣨�룩
const LONG OPT_TIME = 60 * 10;

//! һ����ҿ�ʼ�������
void GameOrganizingCtrl::PlayerFacOptBegin(const CGUID &PlayerGuid, LONG lOptType)
{
	tagFacOptSession FacOptSession;
	FacOptSession.PlayerGuid	= PlayerGuid;
	FacOptSession.lOptType		= lOptType;

	m_PlayerOptSession.CreateSession(FacOptSession);
}

//! ��Ҵ򿪼����л��������
void GameOrganizingCtrl::OpenApplyBoard(const CGUID &PlayerGuid)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	if(NULL == pPlayer) return;

	CMessage msg(MSG_S2C_Faction_ApplyBoardOpen_Notify);
	if(NULL_GUID != pPlayer->GetFactionID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_HadFaction);
		msg.SendToPlayer(PlayerGuid, false);
		return;
	}

	PlayerFacOptBegin(PlayerGuid, ePFOT_Apply);

	msg.Add(PLAYER_OPT_SUCCEED);
	CGUID ApplyFacGuid;
	map<CGUID,CGUID>::iterator ite = m_mapAllApplicant.find(PlayerGuid);
	if (m_mapAllApplicant.end() != ite)
	{
		ApplyFacGuid = ite->second;
	}
	msg.Add(ApplyFacGuid);


	map<CGUID, GameFaction*> mapTmpFaction;
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		if(ite->second->GetCountry() == pPlayer->GetCountry())
			mapTmpFaction[ite->first] = ite->second;
	}

	msg.Add((LONG)mapTmpFaction.size());

	tagApplyInfo ApplyInfo;
	for (map<CGUID, GameFaction*>::iterator ite = mapTmpFaction.begin(); ite != mapTmpFaction.end(); ++ite)
	{
		ite->second->GetApplyInfo(ApplyInfo);
		msg.AddEx(&ApplyInfo, sizeof(tagApplyInfo));
        msg.Add(ite->second->GetPronounceWord().szBody);
	}

	msg.SendToPlayer(PlayerGuid, false);
}

//! ��Ҵ��ϴ�ͼ�����
void GameOrganizingCtrl::OpenUpLoadIconBoard(const CGUID &PlayerGuid)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	if(NULL == pPlayer) return;

	CMessage msg(MSG_S2C_Faction_IconBoardOpen_Notify);

	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if (NULL == pFaction)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_NotInFaction);
		msg.SendToPlayer(PlayerGuid, false);
		return;
	}

	//! ����Ȩ��
	LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_EditIcon);
	if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_Level_Noenough);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	if (pFaction->GetMaster() != pPlayer->GetExID())
	{
		//! ��ԱȨ��
		if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_EditIcon))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_Purview_Noenough);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
	}

	PlayerFacOptBegin(PlayerGuid, ePFOT_UpLoadIcon);

	msg.Add(PLAYER_OPT_SUCCEED);
	msg.SendToPlayer(PlayerGuid, false);
}

//! ������ù�����ļ״̬
bool GameOrganizingCtrl::SetFactionRecruit(CPlayer *pPlayer, bool bOpen)
{
	if(NULL == pPlayer)
		return false;

	CMessage msg(MSG_S2C_Faction_ChangeRecruitState_Notify);

	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_NotInFaction);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return false;
	}

	if(pFaction->GetMaster() != pPlayer->GetExID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_Purview_Noenough);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return false;
	}

    bool bRe = pFaction->SetRecruitState( bOpen ? true : false);

	if(bRe)
	{
		msg.Add(PLAYER_OPT_SUCCEED);
		msg.Add((LONG)bOpen);

		pFaction->SendToAllConcern(msg);
	}
	else
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_SameValue);
		msg.SendToPlayer(pPlayer->GetExID(), false);
	}


	return bRe;
}

//!							�õ�������ڹ���ȼ�
LONG GameOrganizingCtrl::GetFactionLvl(CPlayer *pPlayer)
{
	if(NULL == pPlayer)
		return -1L;

	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return -1L;

	return pFaction->GetLevel(eUT_FactionLevel);
}

//! ��֤��Ҳ���
BOOL GameOrganizingCtrl::TestFacOpt(const CGUID &PlayerGuid, LONG lOptType)
{
	tagFacOptSession FacOptSession;
	FacOptSession.PlayerGuid	= PlayerGuid;
	FacOptSession.lOptType		= lOptType;
	return m_PlayerOptSession.TestSession(FacOptSession, FALSE);
}

//! һ����ҽ����������
void GameOrganizingCtrl::PlayerFacOptEnd(const CGUID &PlayerGuid, LONG lOptType)
{
	tagFacOptSession FacOptSession;
	FacOptSession.PlayerGuid	= PlayerGuid;
	FacOptSession.lOptType		= lOptType;

	m_PlayerOptSession.TestSession(FacOptSession);
}

//! ��Ӧ��Ҵ�������
void GameOrganizingCtrl::OnPlayerCreateFactionReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if(NULL == pPlayer) return;

	CMessage msg(MSG_S2C_ORGSYS_CreateFaction_Re);

	//! ��������Ѿ����۳��˴�����Դ�����
	if (m_mapWaitCreatePlayer.end() != m_mapWaitCreatePlayer.find(pPlayer->GetExID()))
	{
		//! ��֤�����Ự
		if (!TestFacOpt(pPlayer->GetExID(), ePFOT_Create)) return;
	}

	//! ��֤�»�����
    char szNewFactionName[MAXSIZE_FactionName] = {0},szPCard[MAX_PronounceCharNum];
	pMsg->GetStr(szNewFactionName, MAXSIZE_FactionName);
    pMsg->GetStr(szPCard,MAX_PronounceCharNum);
	if (0 == strlen(szNewFactionName))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_ReName);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! �����ַ�
	string strNewFactionName(szNewFactionName);
	if(!CWordsFilter::GetInstance()->Check(strNewFactionName, false, true))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_SensitiveWord);
		msg.SendToSocket(pMsg->GetSocketID(), false);
		return;
	}

	//! �ո�
	LONG lNameSize = (long)strlen(szNewFactionName);
	for (LONG i = 0; i < lNameSize; ++i)
	{
		if(' ' == szNewFactionName[i])
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_SensitiveWord);
			msg.SendToSocket(pMsg->GetSocketID(), false);
			return;
		}
	}

	//! �ظ�����
	for (map<CGUID, GameFaction*>::iterator ite = m_FactionOrganizings.begin(); ite != m_FactionOrganizings.end(); ++ite)
	{
		if (0 == stricmp(ite->second->GetName(), szNewFactionName))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_ReName);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
	}

	//! ������֤�꣬�ͻ��˾Ͳ����ٲ����ˣ�ɾ���Ự
	PlayerFacOptEnd(pPlayer->GetExID(), ePFOT_Create);

	//! ��֤������
	if (NULL_GUID != pPlayer->GetFactionID())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_HadFaction);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! ����������ִ��
	eFactionOptErrInfo ReFactionOptErrInfo = CreateFac_ByCondition(pPlayer);

	if (eFOEI_NotErr != ReFactionOptErrInfo)
	{
		assert(eFOEI_Err_NotInFaction != ReFactionOptErrInfo);

		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)ReFactionOptErrInfo);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! ���۳������������Ҽ���ȴ������б�
	m_mapWaitCreatePlayer.insert(pPlayer->GetExID());

	//! �ɹ�������WS��������
	msg.SetType(MSG_S2W_ORGSYS_CreateFaction_Reques);
	msg.Add(pPlayer->GetExID());
	msg.Add(szNewFactionName);
    msg.Add(szPCard);
	msg.Send(false);
}

//! ��Ӧ��ҽ�ɢ����
void GameOrganizingCtrl::OnPlayerDisbandFactionReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			if(pPlayer->GetExID() == pFaction->GetMaster())
			{
				eFactionOptErrInfo eOptErrInfo = pFaction->CanDisban();
				if (eFOEI_NotErr == eOptErrInfo)
				{
					CMessage msg(MSG_S2W_ORGSYS_DisbandFaction_Reques);
					msg.Add(pFaction->GetExID());
					msg.Send();
				}
				else
				{
					CMessage msg(MSG_S2C_ORGSYS_DisbandFaction_Re);
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eOptErrInfo);
					msg.SendToPlayer(pPlayer->GetExID(), false);
				}
			}
		}
	}
}

//! ��Ӧ����������
void GameOrganizingCtrl::OnPlayerFacInviteReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CMessage msg(MSG_S2C_Faction_InvitePlayer_Re);

			//! ����Ȩ��
			LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_Invite);
			if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Level_Noenough);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}
			
			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_Invite))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			//! ת������WS
			char szAimName[MAX_MEMBER_NAME_SIZE] = {0};
			pMsg->GetStr(szAimName, MAX_MEMBER_NAME_SIZE);

			msg.SetType(MSG_S2W_Faction_InvitePlayer_Reques);
			msg.Add(pPlayer->GetExID());
			msg.Add(szAimName);
			msg.Send(false);
		}
	}
}

//! ��Ӧ����޸Ĺ���
void GameOrganizingCtrl::OnPlayerFacSetPlacardRequest(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CMessage msg(MSG_S2C_ORGSYS_SetPlacard_Re);

			//! ����Ȩ��
			LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_SetAffiche);
			if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Level_Noenough);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}

			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_SetAffiche))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			//! ת������WS
			char szPronounce[MAX_PronounceCharNum] = {0};
			pMsg->GetStr(szPronounce, MAX_PronounceCharNum);

			//! �����ַ�
			//string strPronounce(szPronounce);
			//CWordsFilter::GetInstance()->Check(strPronounce, true, true);

			msg.SetType(MSG_S2W_Faction_SetPronounce_Reques);
			msg.Add(pPlayer->GetExID());
			//msg.Add(strPronounce.c_str());
			msg.Add(szPronounce);
			msg.Send(false);
		}
	}
}

//! ��Ӧ����޸ı���
void GameOrganizingCtrl::OnPlayerFacSetLeavewordRequest(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CMessage msg(MSG_S2C_ORGSYS_SetLeaveword_Re);

			//! ����Ȩ��
			LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_SetAffiche);
			if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Level_Noenough);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}

			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_SetAffiche))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			//! ת������WS
			char szPronounce[MAX_PronounceCharNum] = {0};
			pMsg->GetStr(szPronounce, MAX_PronounceCharNum);

			//! �����ַ�
			//string strPronounce(szPronounce);
			//CWordsFilter::GetInstance()->Check(strPronounce, true, true);

			msg.SetType(MSG_S2W_Faction_SetLeaveword_Reques);
			msg.Add(pPlayer->GetExID());
			//msg.Add(strPronounce.c_str());
			msg.Add(szPronounce);
			msg.Send(false);
		}
	}
}

//! ��Ӧ�������
void GameOrganizingCtrl::OnPlayerKickReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CGUID AimGuid;
			pMsg->GetGUID(AimGuid);

			CMessage msg(MSG_S2C_ORGSYS_Kick_Re);

			//! ����Ȩ��
			LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_Fire);
			if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Level_Noenough);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}

			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_Fire))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}

				if (pFaction->TestPurview(AimGuid, eMP_Fire) || pFaction->GetMaster() == AimGuid)
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_SamePurview);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			//! ת������WS
			char szPronounce[MAX_PronounceCharNum] = {0};
			pMsg->GetStr(szPronounce, MAX_PronounceCharNum);
			msg.SetType(MSG_S2W_Faction_Fire_Reques);
			msg.Add(pFaction->GetExID());
			msg.Add(pPlayer->GetExID());
			msg.Add(AimGuid);
			msg.Send(false);
		}
	}
}

//! ��Ӧ���óƺ�
void GameOrganizingCtrl::OnPlayerFacSetMemberTitleReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CGUID AimGuid;
			pMsg->GetGUID(AimGuid);
	
			CMessage msg(MSG_S2C_Faction_SetMemberTitle_Re);

			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_EditTitle))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}

				if (pFaction->TestPurview(AimGuid, eMP_EditTitle) || pFaction->GetMaster() == AimGuid)
				{
					if (AimGuid != pPlayer->GetExID())
					{
						msg.Add(PLAYER_OPT_FAILED);
						msg.Add((LONG)eFOEI_Err_SamePurview);
						msg.SendToPlayer(pPlayer->GetExID(), false);
						return;
					}
				}
			}

			char szTitle[MAX_MEMBER_NAME_SIZE] = {0};
			pMsg->GetStr(szTitle, MAX_MEMBER_NAME_SIZE);

			//! �����ַ�
			string strTitle(szTitle);
			if(!CWordsFilter::GetInstance()->Check(strTitle, false, true))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_SensitiveWord);
				msg.SendToSocket(pMsg->GetSocketID(), false);
				return;
			}

			msg.SetType(MSG_S2W_Faction_SetMemberTitle_Reques);
			msg.Add(pFaction->GetExID());
			msg.Add(pPlayer->GetExID());
			msg.Add(AimGuid);
			msg.Add(szTitle);
			msg.Send(false);
		}
	}
}


//! ��Ӧ��λ
void GameOrganizingCtrl::OnPlayerFacSetChairmanReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CGUID NewMasterGuid;
			pMsg->GetGUID(NewMasterGuid);

			LONG lNewMasterLvl = pFaction->GetMemberLevel(NewMasterGuid);
			const tagFacLevelSetup *pFacLevelSetup = COrganizingParam::getInstance().GetLevelSetup(pFaction->GetLevel(eUT_FactionLevel));

			if(NULL == pFacLevelSetup)
			{
				CMessage msgErrRe(MSG_S2C_Faction_SetChairman_Re);
				msgErrRe.Add(PLAYER_OPT_FAILED);
				msgErrRe.Add((LONG)eFOEI_Err_Unknown);
				msgErrRe.SendToSocket(pMsg->GetSocketID(), false);
				return;
			}

			for (LONG i = 0; i < (long)pFacLevelSetup->vCondition.size(); ++i)
			{
				if(eUCT_LeaderLevel == pFacLevelSetup->vCondition[i].lConditionType)
				{
					if (pFacLevelSetup->vCondition[i].lValue > lNewMasterLvl)
					{
						CMessage msgErrRe(MSG_S2C_Faction_SetChairman_Re);
						msgErrRe.Add(PLAYER_OPT_FAILED);
						msgErrRe.Add((LONG)eFOEI_Err_AimLevelTooLow);
						msgErrRe.SendToSocket(pMsg->GetSocketID(), false);
						return;
					}
					break;
				}
			}

			if (pPlayer->GetExID() == pFaction->GetMaster() && pPlayer->GetExID() != NewMasterGuid)
			{
				CMessage msg(MSG_S2W_Faction_SetNewMaster_Reques);
				msg.Add(pFaction->GetExID());
				msg.Add(NewMasterGuid);
				msg.Send(false);
			}
		}
	}
}

//! �����������
void GameOrganizingCtrl::OnPlayerFacCheckRequisition(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_Authorize))
				{
					CMessage msg(MSG_S2C_ORGSYS_CheckRequisition_Re);
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			BOOL bAgree = pMsg->GetLong();
			CGUID AimGuid;
			pMsg->GetGUID(AimGuid);

			if(pFaction->HasApplicant(AimGuid))
			{
				CMessage msg(MSG_S2W_Faction_CheckRequisition_Reques);
				msg.Add(pPlayer->GetFactionID());
				msg.Add((LONG)bAgree);
				msg.Add(AimGuid);
				msg.Add(pPlayer->GetExID());
				msg.Send(false);
			}
			else
			{
				CMessage msg(MSG_S2C_ORGSYS_CheckRequisition_Re);
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_AimNoBeing);
				msg.SendToPlayer(pPlayer->GetExID(), false);
			}
		}
	}
}

//! ����������
void GameOrganizingCtrl::OnPlayerJoinFac(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		if(!TestFacOpt(pPlayer->GetExID(), ePFOT_Apply))
		{
			CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_SessionLost);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
		else
		{
			//! ���һ���µĲ������Ͷ�session��ʱ
			PlayerFacOptBegin(pPlayer->GetExID(), ePFOT_Apply);
		}

		if(NULL_GUID != pPlayer->GetFactionID())
			return;

		BOOL bIsJoin = pMsg->GetLong();

		CGUID AimFacGUID;
		pMsg->GetGUID(AimFacGUID);

		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(AimFacGUID);
		if (NULL == pFaction)
		{
			CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_AimNoBeing);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
		if (bIsJoin)
		{
			if (m_mapAllApplicant.end() != m_mapAllApplicant.find(pPlayer->GetExID()))
			{
				CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_ReJoin);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}

			if (pFaction->GetCountry() != pPlayer->GetCountry())
			{
				CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Country);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}
		}
		else
		{
			if (m_mapAllApplicant.end() == m_mapAllApplicant.find(pPlayer->GetExID()))
				return;
		}

		CMessage msg(MSG_S2W_ORGSYS_JoinFac_Request);
		msg.Add(AimFacGUID);
		msg.Add(pPlayer->GetExID());
		msg.Add((LONG)bIsJoin);
		msg.Send(false);
	}
}

//! ��Ӧ����ϴ�ͼ������
void GameOrganizingCtrl::OnPlayerUpLoadIconReques(CMessage *pMsg)
{
	CPlayer *pPlayer = pMsg->GetPlayer();
	if (NULL != pPlayer)
	{
		//! ��֤�����Ự
		if (!TestFacOpt(pPlayer->GetExID(), ePFOT_UpLoadIcon)) return;
		//! ������֤�꣬�ͻ��˾Ͳ����ٲ����ˣ�ɾ���Ự
		PlayerFacOptEnd(pPlayer->GetExID(), ePFOT_UpLoadIcon);

		GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
		if (NULL != pFaction)
		{
			CMessage msg(MSG_S2C_Faction_IconData_Re);

			//! ����Ȩ��
			LONG lNeedLevel = COrganizingParam::getInstance().GetFacPurviewNeedLevel(eFPI_EditIcon);
			if (lNeedLevel > pFaction->GetLevel(eUT_FactionLevel))
			{
				msg.Add(PLAYER_OPT_FAILED);
				msg.Add((LONG)eFOEI_Err_Level_Noenough);
				msg.SendToPlayer(pPlayer->GetExID(), false);
				return;
			}

			if (pFaction->GetMaster() != pPlayer->GetExID())
			{
				//! ��ԱȨ��
				if (!pFaction->TestPurview(pPlayer->GetExID(), eMP_EditIcon))
				{
					msg.Add(PLAYER_OPT_FAILED);
					msg.Add((LONG)eFOEI_Err_Purview_Noenough);
					msg.SendToPlayer(pPlayer->GetExID(), false);
					return;
				}
			}

			//! ת������WS
			LONG lIconSize = pMsg->GetLong();
			if(MAX_ICON_SIZE < lIconSize)//! ���ݳ��ȳ�������
			{
				return;
			}

			vector<BYTE> vIconData(lIconSize);
			pMsg->GetEx(&vIconData[0], lIconSize);

			msg.SetType(MSG_S2W_Faction_UpLoadIcon_Reques);
			msg.Add(pFaction->GetExID());
			msg.Add(pPlayer->GetExID());
			msg.Add((LONG)vIconData.size());
			msg.AddEx(&vIconData[0],(long)vIconData.size());
			msg.Send(false);
		}
	}
}


//! ��Ӧ�л������ĵ���
void GameOrganizingCtrl::OnPlayerUpgradeFac(CPlayer *pPlayer, DWORD dwUpgradeType)
{
	if (NULL == pPlayer) return;

	CMessage msg(MSG_S2C_ORGSYS_UpgradeFaction_Re);

	if(0 == CMessage::GetWSSocket())
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_CoreServerError);
		msg.Add(dwUpgradeType);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	const CGUID &FactionGuid = pPlayer->GetFactionID();
	if (NULL_GUID == FactionGuid) return;

	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if (NULL == pFaction) return;

	if (pFaction->GetMaster() != pPlayer->GetExID()) return;

	//! ִ������
	eFactionOptErrInfo ReFactionOptErrInfo = UpgradeFac_ByCondition(pPlayer, pFaction->GetLevel(dwUpgradeType) + 1, dwUpgradeType);

	

	if (eFOEI_NotErr != ReFactionOptErrInfo)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)ReFactionOptErrInfo);
		msg.Add(dwUpgradeType);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! �ɹ���֪ͨWS
	msg.SetType(MSG_S2W_ORGSYS_UpgradeFaction_Reques);
	msg.Add(FactionGuid);
	msg.Add(dwUpgradeType);
	msg.Send(false);
}

//! ��Ӧ��һش�����
void GameOrganizingCtrl::OnPlayerFacInviteAnswer(CMessage *pMsg)
{
	tagFacInviteSession FacInviteSession;

	FacInviteSession.AimGuid = pMsg->GetPlayerID();
	BOOL lAnswer = (BOOL)pMsg->GetLong();
	pMsg->GetGUID(FacInviteSession.InviterGuid);

	if(m_PlayerInviteSession.TestAndCopySession(FacInviteSession))
	{
		if(NULL == GetFactionOrganizing(FacInviteSession.FactionGuid))
		{
			CMessage msg(MSG_S2C_ORGSYS_JoinFac_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_AimNoBeing);
			msg.SendToPlayer(FacInviteSession.AimGuid, false);
			return;
		}

		if (lAnswer)
		{
			CMessage msg(MSG_S2W_Faction_AddMember);
			msg.Add(FacInviteSession.InviterGuid);
			msg.Add(FacInviteSession.AimGuid);
			msg.Add(FacInviteSession.FactionGuid);
			msg.Send(false);
		}
		else
		{
			CMessage msg(MSG_S2C_Faction_InvitePlayer_Re);
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_AimRefuse);
			msg.SendToPlayer(FacInviteSession.InviterGuid, false);
		}	
	}
}

//! ����������������
eFactionOptErrInfo GameOrganizingCtrl::TestUpgradeFacCondition(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType)
{
	const VecCondition* pVecCondition = COrganizingParam::getInstance().GetUpgradeVecCondition(dwUpgradeType, lGoalLvl);
	if (NULL == pVecCondition)
		return eFOEI_Err_SetupLoading;

	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());

	const VecCondition& vCondition = *pVecCondition;
	for (LONG i = 0; i < (long)vCondition.size(); ++i)
	{
		switch(vCondition[i].lConditionType)
		{
		case eUCT_Money:
			{
				if(vCondition[i].lValue > (long)pPlayer->GetMoney())
					return eFOEI_Err_Money;
			}
			break;
		case eUCT_LeaderLevel:
			{
				if(vCondition[i].lValue > pPlayer->GetLevel())
					return eFOEI_Err_Level;
			}
			break;
		case eUCT_FacLevel:
			{
				if(vCondition[i].lValue > pFaction->GetLevel(eUT_FactionLevel))
					return eFOEI_Err_Level_Noenough;
			}
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

//!							��ⴴ�����˵�����
eFactionOptErrInfo GameOrganizingCtrl::TestCreateUnionCondition(CPlayer *pPlayer)
{
	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return eFOEI_Err_NotInFaction;
	if(NULL_GUID != pPlayer->GetUnionID())
		return eFOEI_Err_HadUnion;
	if(pPlayer->GetExID() != pFaction->GetMaster())
		return eFOEI_Err_Purview_Noenough;
	

	const VecCondition& vCreateUnionCondition = COrganizingParam::getInstance().GetUnionCreateCondition();

	for (LONG i = 0; i < (long)vCreateUnionCondition.size(); ++i)
	{
		switch(vCreateUnionCondition[i].lConditionType)
		{
		case eUCT_Money:
			{
				if(vCreateUnionCondition[i].lValue > (long)pPlayer->GetMoney())
					return eFOEI_Err_Money;
			}
			break;
		case eUCT_LeaderLevel:
		case eUCT_FacLevel:
			{
				if(vCreateUnionCondition[i].lValue > pFaction->GetLevel(eUT_FactionLevel))
					return eFOEI_Err_Level_Noenough;
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
eFactionOptErrInfo GameOrganizingCtrl::UpgradeFac_ByCondition(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType)
{
	//! ��ʼ�����������
	eFactionOptErrInfo eTestResult = TestUpgradeFacCondition(pPlayer, lGoalLvl, dwUpgradeType);
	if(eFOEI_NotErr != eTestResult)
		return eTestResult;

	const VecCondition *pVecCondition = COrganizingParam::getInstance().GetUpgradeVecCondition(dwUpgradeType, lGoalLvl);
	if (NULL == pVecCondition)
		return eFOEI_Err_SetupLoading;

	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());

	const VecCondition &vCondition = *pVecCondition;
	//! ��ʼ��������ִ��
	for (LONG i = 0; i < (long)vCondition.size(); ++i)
	{
		switch(vCondition[i].lConditionType)
		{
		case eUCT_Money:
			{
				if(vCondition[i].lValue <= (long)pPlayer->GetMoney())
				{
					pPlayer->SetMoney(pPlayer->GetMoney() - vCondition[i].lValue,30);
				}
				else
				{
					return eFOEI_Err_Money;
				}
			}
			break;
		case eUCT_LeaderLevel:
			{
				if(vCondition[i].lValue > pPlayer->GetLevel())
				{
					return eFOEI_Err_Level;
				}
			}
		    break;
		case eUCT_FacLevel:
			{
				if(vCondition[i].lValue > pFaction->GetLevel(eUT_FactionLevel))
				{
					return eFOEI_Err_Level_Noenough;
				}
			}
			break;
		case eUCT_FacRes:
		case eUCT_FacExp:
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

//! �����������ع�GS�ϵ������������
void GameOrganizingCtrl::BackRoll_UpgradeFac(CPlayer *pPlayer, LONG lGoalLvl, DWORD dwUpgradeType)
{
	const VecCondition *pVecCondition = COrganizingParam::getInstance().GetUpgradeVecCondition(dwUpgradeType, lGoalLvl);
	if (NULL == pVecCondition)
		return;
	const VecCondition &vCondition = *pVecCondition;

	//! ��ʼ��������ִ��
	for (LONG i = 0; i < (long)vCondition.size(); ++i)
	{
		switch(vCondition[i].lConditionType)
		{
		case eUCT_Money:
			{
				if(vCondition[i].lValue <= (long)pPlayer->GetMoney())
				{
					pPlayer->SetMoney(pPlayer->GetMoney() + vCondition[i].lValue,31);
				}
			}
			break;
		default:
			break;
		}
	}
}

//! �������ˣ������õ�����
eFactionOptErrInfo GameOrganizingCtrl::CreateUnion_ByCondition(CPlayer *pPlayer)
{
	GameFaction *pFaction = GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL == pFaction)
		return eFOEI_Err_NotInFaction;
	if(NULL_GUID != pPlayer->GetUnionID())
		return eFOEI_Err_HadUnion;
	if(pPlayer->GetExID() != pFaction->GetMaster())
		return eFOEI_Err_Purview_Noenough;

	const VecCondition& vCreateUnionCondition = COrganizingParam::getInstance().GetUnionCreateCondition();

	for (LONG i = 0; i < (long)vCreateUnionCondition.size(); ++i)
	{
		switch(vCreateUnionCondition[i].lConditionType)
		{
		case eUCT_Money:
			{
				if(vCreateUnionCondition[i].lValue <= (long)pPlayer->GetMoney())
				{
					pPlayer->SetMoney(pPlayer->GetMoney() - vCreateUnionCondition[i].lValue,32);
				}
				else
				{
					return eFOEI_Err_Money;
				}
			}
			break;
		case eUCT_LeaderLevel:
		case eUCT_FacLevel:
			{
				if(vCreateUnionCondition[i].lValue > pFaction->GetLevel(eUT_FactionLevel))
				{
					return eFOEI_Err_Level_Noenough;
				}
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
eFactionOptErrInfo GameOrganizingCtrl::CreateFac_ByCondition(CPlayer *pPlayer)
{
	//! ��һ���������������Ǵ�������
	return UpgradeFac_ByCondition(pPlayer, 1, eUT_FactionLevel);
}

//! �㲥һ���л�Ĺ�������
void GameOrganizingCtrl::RadiateFactionPublic(GameFaction *pFaction)
{
	if(NULL == pFaction) return;
	CPlayer *pMaster = GetGame()->FindPlayer(pFaction->GetMaster());
	if (NULL != pMaster)
	{
		CMessage msg(MSG_S2C_Faction_PublicData_Notify);
		pFaction->AddPublicData_ToMsgBack(&msg);
		msg.SendToAround(pMaster);
	}
}

GameUnion*	GameOrganizingCtrl::GetUnion(const CGUID& GameUnionID)
{
	if(GameUnionID == NULL_GUID) return NULL;
	if(m_mapUnion.find(GameUnionID) != m_mapUnion.end())
	{
		return m_mapUnion[GameUnionID];
	}
	return NULL;
}

//! ��Ӧ����ϵͳ�ĳ�ʼ����Ϣ
void GameOrganizingCtrl::OnInitGsUnion(vector<BYTE> &vData)
{
	//�ͷ�����
	for (map<CGUID,GameUnion*>::iterator ite = instance->m_mapUnion.begin(); ite != instance->m_mapUnion.end(); ++ite)
	{
		OBJ_RELEASE(GameUnion, (*ite).second);
	}
	m_mapUnion.clear();

	LONG lPos = 0;
	LONG lUnionNum = _GetLongFromByteArray(&vData[0], lPos);

	for (LONG i = 0; i < lUnionNum; ++i)
	{
		GameUnion *pUnion = OBJ_CREATE(GameUnion);
		pUnion->Decode(&vData[0], lPos);
		
		map<CGUID, GameUnion*>::iterator ite = m_mapUnion.find(pUnion->GetExID());
		if(m_mapUnion.end() != ite)
			OBJ_RELEASE(GameUnion, ite->second);
		m_mapUnion[pUnion->GetExID()] = pUnion;
	}
}

//! ��Ӧ������������Ϣ
void GameOrganizingCtrl::OnCreateNewUnion(vector<BYTE> &vData)
{
	LONG lPos = 0;
	GameUnion *pUnion = OBJ_CREATE(GameUnion);
	pUnion->Decode(&vData[0], lPos);

	map<CGUID,GameUnion*>::iterator ite = m_mapUnion.find(pUnion->GetExID());
	if(m_mapUnion.end() != ite)
	{
		OBJ_RELEASE(GameUnion, ite->second);
	}

	m_mapUnion[pUnion->GetExID()] = pUnion;

	pUnion->OnCreate();
}

//! ��ӦWS��ɢ����
void GameOrganizingCtrl::OnWsDisbandUnion(const CGUID &UnionGuid)
{
	GameUnion *pUnion = GetUnion(UnionGuid);

	if(NULL != pUnion)
	{
		pUnion->Disband();
		OBJ_RELEASE(GameUnion, pUnion);
		m_mapUnion.erase(UnionGuid);
	}
}

//! ��Ӧ�˳�����
void GameOrganizingCtrl::OnWsQuitUnion(const CGUID &FactionGuid, const CGUID &UnionGuid)
{
	GameUnion *pUnion = GetUnion(UnionGuid);
	GameFaction *pFaction = GetFactionOrganizing(FactionGuid);
	if(NULL == pUnion || NULL == pFaction)
		return;

	pUnion->DelMember(FactionGuid);
	pFaction->OnQuitUnion();
}

//! ��Ӧ�������˳�Ա
void GameOrganizingCtrl::OnWsKickUnion(const CGUID &OptGuid, const CGUID &AimGuid, const CGUID &UnionGuid)
{
	GameUnion *pUnion = GetUnion(UnionGuid);
	GameFaction *pFaction = GetFactionOrganizing(AimGuid);
	if(NULL == pUnion || NULL == pFaction)
		return;

	pUnion->DelMember(AimGuid);
	pFaction->OnQuitUnion();

	//! �ظ�������
	GameFaction *pOptFaction = GetFactionOrganizing(OptGuid);
	if(NULL != pOptFaction)
	{
		CPlayer *pPlayer = GetGame()->FindPlayer(pOptFaction->GetMaster());
		if(NULL != pPlayer)
		{
			CMessage msg(MSG_S2C_UNION_Re_Kick);
			msg.Add(OptGuid);
			msg.Add(AimGuid);
			msg.Add(UnionGuid);
			msg.SendToPlayer(pPlayer->GetExID(), false);
		}
	}
}

//! ��Ӧ��λ
void GameOrganizingCtrl::OnWsSetChairman(const CGUID &UnionGuid, const CGUID &NewMasterFacGuid, const char *pNewMasterName)
{
	GameUnion *pUnion = GetUnion(UnionGuid);
	if(NULL == pUnion)
		return;
	
	pUnion->ChangeMaster(NewMasterFacGuid, pNewMasterName);
}

//! ��Ӧ����Ȩ��
void GameOrganizingCtrl::OnWsSetPurviewUnion(const CGUID &UnionGuid, const CGUID &AimFacGuid, DWORD dwPurview)
{
	GameUnion *pUnion = GetUnion(UnionGuid);
	if(NULL == pUnion)
		return;

	pUnion->SetPurview(AimFacGuid, dwPurview);
}


//! ��Ӧ��Ҵ�����������
void GameOrganizingCtrl::OnPlayerCreateUnion(CPlayer *pPlayer, const char *pUnionName)
{
	CMessage msg(MSG_S2C_UNION_Re_Creation);

	//! ��������Ѿ����۳��˴�����Դ�����
	if (m_mapWaitCreateUnionPlayer.end() == m_mapWaitCreateUnionPlayer.find(pPlayer->GetExID()))
	{
		//! ��֤�����Ự
		if (!TestFacOpt(pPlayer->GetExID(), ePFOT_CreateUnion)) return;
	}

	//! ��֤�»�����
	if (0 == strlen(pUnionName))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_ReName);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! �����ַ�
	string strNewUnionName(pUnionName);
	if(!CWordsFilter::GetInstance()->Check(strNewUnionName, false, true))
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)eFOEI_Err_SensitiveWord);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! �ո�
	LONG lNameSize = (long)strlen(pUnionName);
	for (LONG i = 0; i < lNameSize; ++i)
	{
		if(' ' == pUnionName[i])
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_SensitiveWord);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
	}

	//! �ظ�����
	for (map<CGUID, GameUnion*>::iterator ite = m_mapUnion.begin(); ite != m_mapUnion.end(); ++ite)
	{
		if (0 == stricmp(ite->second->GetName(), pUnionName))
		{
			msg.Add(PLAYER_OPT_FAILED);
			msg.Add((LONG)eFOEI_Err_ReName);
			msg.SendToPlayer(pPlayer->GetExID(), false);
			return;
		}
	}

	//! ������֤�꣬�ͻ��˾Ͳ����ٲ����ˣ�ɾ���Ự
	PlayerFacOptEnd(pPlayer->GetExID(), ePFOT_CreateUnion);


	//! ����������ִ��
	eFactionOptErrInfo ReFactionOptErrInfo = CreateUnion_ByCondition(pPlayer);

	if (eFOEI_NotErr != ReFactionOptErrInfo)
	{
		msg.Add(PLAYER_OPT_FAILED);
		msg.Add((LONG)ReFactionOptErrInfo);
		msg.SendToPlayer(pPlayer->GetExID(), false);
		return;
	}

	//! ���۳������������Ҽ���ȴ������б�
	m_mapWaitCreateUnionPlayer.insert(pPlayer->GetExID());

	//! �ɹ�������WS��������
	msg.SetType(MSG_S2W_UNION_Request_Create);
	msg.Add(pPlayer->GetExID());
	msg.Add(pUnionName);
	msg.Send(false);
}

//! ��Ӧ��ҽ�ɢ������Ϣ
void GameOrganizingCtrl::OnPlayerDisbandUnion(CPlayer *pPlayer)
{
	assert(NULL != pPlayer);
	GameUnion *pUnion = GetUnion(pPlayer->GetUnionID());
	if(NULL != pUnion)
	{
		//! ս��״���ж�
		if(TestWarState(pPlayer->GetFactionID()))
		{
			return;
		}

		//! �����ж�
		if(pUnion->GetMasterID() == pPlayer->GetExID())
		{
			CMessage msg(MSG_S2W_UNION_Request_Disband);
			msg.Add(pPlayer->GetExID());
			msg.Send(false);
		}
	}
}

//! ��Ӧ��������˳�����
void GameOrganizingCtrl::OnPlayerQuitUnion(CPlayer *pPlayer)
{
	assert(NULL != pPlayer);
	GameUnion	*pUnion		= GetUnion(pPlayer->GetUnionID());
	GameFaction *pFaction	= GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL != pUnion && NULL != pFaction)
	{
		//! �᳤�ж�
		if(pFaction->GetMaster() != pPlayer->GetExID())
		{
			return;
		}
		//! ս��״���ж�
		if(TestWarState(pPlayer->GetFactionID()))
		{
			return;
		}
		//! �����ж�
		if(pUnion->GetMasterID() == pPlayer->GetExID())
		{
			return;
		}

		CMessage msg(MSG_S2W_UNION_Request_Quit);
		msg.Add(pFaction->GetExID());
		msg.Add(pUnion->GetExID());
		msg.Send(false);
	}
}

//! ��Ӧ��ҿ�������
void GameOrganizingCtrl::OnPlayerKick(CPlayer *pPlayer, const CGUID &AimFactionGuid)
{
	assert(NULL != pPlayer);
	if(NULL_GUID == AimFactionGuid)
		return;
	GameUnion	*pUnion		= GetUnion(pPlayer->GetUnionID());
	GameFaction *pFaction	= GetFactionOrganizing(pPlayer->GetFactionID());
	if(NULL != pUnion && NULL != pFaction)
	{
		//! �᳤�ж�
		if(pFaction->GetMaster() != pPlayer->GetExID())
		{
			return;
		}
	
		//! ��Ա�ж�
		assert(pUnion->IsMember(pPlayer->GetFactionID()));
		if(!pUnion->IsMember(AimFactionGuid))
		{
			return;
		}

		//! ս��״���ж�
		if(TestWarState(AimFactionGuid))
		{
			return;
		}

		//! Ȩ���ж�
		if(0 == (eUMP_Fire & pUnion->GetPurview(pFaction->GetExID())))
		{
			return;
		}

		CMessage msg(MSG_S2W_UNION_Request_Kick);
		msg.Add(pFaction->GetExID());
		msg.Add(AimFactionGuid);
		msg.Add(pUnion->GetExID());
		msg.Send(false);
	}
}

//! ��Ӧ��������
void GameOrganizingCtrl::OnPlayerSetChairman(CPlayer *pPlayer, const CGUID &AimFactionGuid)
{
	assert(NULL != pPlayer);
	if(NULL_GUID == AimFactionGuid)
		return;
	GameUnion	*pUnion		= GetUnion(pPlayer->GetUnionID());
	if(NULL != pUnion)
	{
		//! �����ж�
		if(pUnion->GetMasterID() != pPlayer->GetExID())
		{
			return;
		}

		//! ��Ա�ж�
		if(!pUnion->IsMember(AimFactionGuid))
		{
			return;
		}

		//! ս��״���ж�
		if(TestWarState(AimFactionGuid))
		{
			return;
		}


		CMessage msg(MSG_S2W_UNION_Request_Kick);
		msg.Add(pPlayer->GetExID());
		msg.Add(AimFactionGuid);
		msg.Send(false);
	}
}

//! ��Ӧ�������Ȩ��
void GameOrganizingCtrl::OnPlayerSetPurview(CPlayer *pPlayer, const CGUID &AimFactionGuid, DWORD dwPurview)
{
	assert(NULL != pPlayer);
	if(NULL_GUID == AimFactionGuid)
		return;
	GameUnion	*pUnion		= GetUnion(pPlayer->GetUnionID());
	if(NULL != pUnion)
	{
		//! �����ж�
		if(pUnion->GetMasterID() != pPlayer->GetExID())
		{
			return;
		}

		//! ��Ա�ж�
		if(!pUnion->IsMember(AimFactionGuid))
		{
			return;
		}

		//! Ȩ���ж�
		if(0 != ((~eUMP_MemberTiptop) & dwPurview))
		{
			return;
		}

		//! ս��״���ж�
		if(TestWarState(AimFactionGuid))
		{
			return;
		}

		CMessage msg(MSG_S2W_UNION_Request_SetPurview);
		msg.Add(pPlayer->GetExID());
		msg.Add(AimFactionGuid);
		msg.Add(dwPurview);
		msg.Send(false);
	}
}

//! ��Ӧ��������������
void GameOrganizingCtrl::OnPlayerUnionInvite(CPlayer *pPlayer, const char *pAimPlayerName)
{
	assert(NULL != pPlayer);
	if(NULL == pAimPlayerName)
		return;
	GameUnion	*pUnion		= GetUnion(pPlayer->GetUnionID());
	if(NULL != pUnion)
	{
		//! �����ж�
		if(pUnion->GetMasterID() != pPlayer->GetExID())
		{
			return;
		}

		CMessage msg(MSG_S2W_UNION_Request_Invit);
		msg.Add(pPlayer->GetExID());
		msg.Add(pAimPlayerName);
		msg.Send(false);
	}
}

//! ����һ�������Ƿ���ս��״̬
BOOL GameOrganizingCtrl::TestWarState(const CGUID &FactionGuid)
{
	return FALSE;
}

//! ���������������
void GameOrganizingCtrl::SendPlayerUnionData(CPlayer *pPlayer, const CGUID &UnionGuid)
{
	if(NULL == pPlayer || NULL_GUID == UnionGuid)
		return;
	GameUnion *pUnion = GetUnion(UnionGuid);
	if(NULL == pUnion)
		return;
	CMessage msgBaseData(MSG_S2C_UNION_Notify_BaseData);
	pUnion->AddBaseToMsg(&msgBaseData);
	msgBaseData.SendToPlayer(pPlayer->GetExID(), false);

	CMessage msgMemberData(MSG_S2C_UNION_Notify_MemberData);
	if(pUnion->AddOneMemberToMsg(pPlayer, &msgMemberData))
	{
		msgMemberData.SendToPlayer(pPlayer->GetExID(), false);
	}
}

//Team
/////////////////////////////////////////////////////////////////////////////////////////////////////
GSTeam*	GameOrganizingCtrl::GetGameTeamByID(const CGUID& TeamID)
{
	map<CGUID,GSTeam*>::iterator ite = m_TeamOrganizings.find(TeamID);
	if (NULL_GUID != TeamID && m_TeamOrganizings.end() != ite)
	{
		return ite->second;
	}
	return NULL;
}

//! ����Ϣ�л�ȡȫ�����ж���
void GameOrganizingCtrl::GetTeamData(CMessage *pMsg)
{
	LONG lDataSize = pMsg->GetLong();
	vector<BYTE> vData(lDataSize + 1);
	BYTE* pData = &vData[0];
	pMsg->GetEx(pData, lDataSize);

	LONG lPos = 0;
	LONG lTeamNum = _GetLongFromByteArray(pData, lPos);

	//�ͷŶ���
	for(map<CGUID,GSTeam*>::iterator ite = instance->m_TeamOrganizings.begin(); ite != instance->m_TeamOrganizings.end(); ++ite)
	{
		((GSTeam*)(*ite).second)->Disband(NULL_GUID, FALSE);
		OBJ_RELEASE(GSTeam, (*ite).second);
	}
	instance->m_TeamOrganizings.clear();

	for (LONG i = 0; i < lTeamNum; ++i)
	{
		GSTeam* pTeam = OBJ_CREATE(GSTeam);
		pTeam->GetDataFromArray(pData, lPos);
		m_TeamOrganizings[pTeam->GetExID()] = pTeam;
	}
}

GSTeam*	GameOrganizingCtrl::CreateTeam(CMessage *pMsg)
{
	GSTeam* pTeam = OBJ_CREATE(GSTeam);
	if(pTeam->Initial(pMsg))
	{
		const CGUID& guid = pTeam->GetExID();

		map<CGUID,GSTeam*>::iterator iteTeam = m_TeamOrganizings.find(guid);
		if(m_TeamOrganizings.end() != iteTeam)
		{
			OBJ_RELEASE(GSTeam, iteTeam->second);
			m_TeamOrganizings.erase(iteTeam);
		}
		m_TeamOrganizings[guid] = pTeam;
		return pTeam;
	}

	OBJ_RELEASE(GSTeam, pTeam);
	return NULL;
}

void GameOrganizingCtrl::DisbandTeam(const CGUID& TeamID)
{
	map<CGUID,GSTeam*>::iterator iteTeam = m_TeamOrganizings.find(TeamID);
	if (iteTeam == m_TeamOrganizings.end())
	{
		assert(false);
		return;
	}
	GSTeam *pTeam = iteTeam->second;
	pTeam->Disband(pTeam->GetMasterID());

	m_TeamOrganizings.erase(TeamID);

	OBJ_RELEASE(GSTeam, pTeam);
}

void GameOrganizingCtrl::DisbandAllTeam(void)
{
	//�ͷŶ���
	for(map<CGUID,GSTeam*>::iterator ite = instance->m_TeamOrganizings.begin(); ite != instance->m_TeamOrganizings.end(); ++ite)
	{
		GSTeam *pTeam = ite->second;
		pTeam->Disband(NULL_GUID);
		OBJ_RELEASE(GSTeam, (*ite).second);
	}
	m_TeamOrganizings.clear();
}

void GameOrganizingCtrl::OnTeamSetupInit(CMessage *pMsg)
{
	pMsg->GetEx(&(GSTeam::s_tagDefaultSetup), sizeof(GSTeam::tagDefaultSetup));
}

void GameOrganizingCtrl::UnpdateTeamMemberPos()
{
	GSTeam *pTeam = NULL;
	for (map<CGUID,GSTeam*>::iterator ite = m_TeamOrganizings.begin(); ite != m_TeamOrganizings.end(); ++ite)
	{
		pTeam = (GSTeam*)((*ite).second);
		if (pTeam == NULL)
		{
			//m_TeamOrganizings.erase(ite++);
			continue;
		}
		pTeam->RadiateAllMemberPos();
	}
}
