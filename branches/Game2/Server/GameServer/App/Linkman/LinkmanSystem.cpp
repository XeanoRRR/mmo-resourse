
#include "StdAfx.h"
#include "LinkmanSystem.h"
#include "../Player.h"


#ifdef _DEBUG
//!				����ϵͳʵ������
LONG			g_LinkmanSystemInstanceNum = 0;
#endif
//!				����ϵͳȫ��Ψһʵ��
LinkmanSystem	g_LinkmanSystem;


LinkmanSystem::LinkmanSystem(void)
{
#ifdef _DEBUG
	if(0 != g_LinkmanSystemInstanceNum)
		throw;
	++g_LinkmanSystemInstanceNum;
#endif
}
LinkmanSystem::~LinkmanSystem(void){}


//! �õ�Ψһʵ��
LinkmanSystem& LinkmanSystem::GetInstance(void)
{
	return g_LinkmanSystem;
}

//! ��ʼ������
void LinkmanSystem::InitSetup(BYTE* pByte, LONG& pos)
{
	m_LGSManage.DecordFromByteArray(pByte, pos);
}

//! ��Ӧ��ʼ����Ϣ
void LinkmanSystem::OnInitMessage(CMessage *pMsg)
{
	BOOL bSendToC = pMsg->GetByte();
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);;

	if(m_mapPlayerLinkman.end() != m_mapPlayerLinkman.find(PlayerGuid))
	{
		m_mapPlayerLinkman.erase(PlayerGuid);
#ifdef _DEBUG
		char szGuid[64] = {0};
		PlayerGuid.tostring(szGuid);
		char szPlayerName[64] = {0};
		if(NULL != pPlayer)
		{
			memmove(szPlayerName, pPlayer->GetName(), 20);
		}
        //��ʼ�������ϵ������ʱ������ϵ���Ѿ����ڣ���[������ƣ���%s�������ID����%s��]
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_15", szPlayerName, szGuid));
#endif		
	}

	CMessage msgClient(MSG_S2C_LINKMAN_INIT);

	VecLinkmanGroup vLinkmanGroup;
	LONG lGroupNum = m_LGSManage.GetGroupNum();
	for (LONG i = 0; i < lGroupNum; ++i)
	{
		LinkmanGroup tmpLinkmanGroup(m_LGSManage.GetLinkmanGroupSetup_ByOrder(i), PlayerGuid);
		tmpLinkmanGroup.Decode_FromWs(pMsg);
		if(bSendToC)
			tmpLinkmanGroup.Serialize_ForClient(&msgClient);
		vLinkmanGroup.push_back(tmpLinkmanGroup);
	}

	m_mapPlayerLinkman.insert(pair<CGUID, VecLinkmanGroup>(PlayerGuid, vLinkmanGroup));

	//! ���͵��ͻ���
	if(bSendToC)
		msgClient.SendToPlayer(PlayerGuid);
	
	if(pPlayer)
		pPlayer->UpdateTitle(TRUE);
}

//! ����������֪ͨ
void LinkmanSystem::OnReleaseMessage(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	m_mapPlayerLinkman.erase(PlayerGuid);
}

//! ��Ӧ��ϵ�˵�½��Ϣ
void LinkmanSystem::OnLinkmanLoginMessage(CMessage *pMsg)
{
	tagLinkman_data Linkman_data;
	pMsg->GetEx(&Linkman_data, sizeof(tagLinkman_data));

	LONG lGuidNum = pMsg->GetLong();

	for (LONG i = 0; i < lGuidNum; ++i)
	{
		CGUID ResponserGuid;
		pMsg->GetGUID(ResponserGuid);

		map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(ResponserGuid);
		if (m_mapPlayerLinkman.end() == ite) continue;

		for (LONG j = 0; j < (long)ite->second.size(); ++j)
		{
			if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder(j)->m_uKeepWay)
				ite->second[j].LinkmanLogin(Linkman_data);
		}
	}
}

//! ��Ӧ��ϵ��������Ϣ
void LinkmanSystem::OnLinkmanLogoutMessage(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	LONG lGuidNum = pMsg->GetLong();

	for (LONG i = 0; i < lGuidNum; ++i)
	{	
		CGUID ResponserGuid;
		pMsg->GetGUID(ResponserGuid);

		map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(ResponserGuid);
		if (m_mapPlayerLinkman.end() == ite) continue;

		for (LONG j = 0; j < (long)ite->second.size(); ++j)
		{
			if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder(j)->m_uKeepWay)
				ite->second[j].LinkmanLogout(PlayerGuid);
		}
	}
}

//! ��Ӧ��ϵ��ɾ��״̬�ı�
void LinkmanSystem::OnPlayerDelStateChange(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	BOOL bDeleted = pMsg->GetLong();

	LONG lDataSize = pMsg->GetLong();

	vector<BYTE> vData(lDataSize + 1);
	BYTE *pData = &vData[0];
	pMsg->GetEx(pData, lDataSize);
	LONG lPos = 0;

	for (LONG i = 0; i < lDataSize/(long)sizeof(CGUID); ++i)
	{	
		CGUID ResponserGuid;
		_GetBufferFromByteArray(pData, lPos, ResponserGuid);

		map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(ResponserGuid);
		if (m_mapPlayerLinkman.end() == ite) continue;

		for (LONG j = 0; j < (long)ite->second.size(); ++j)
		{
			if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder(j)->m_uKeepWay)
				ite->second[j].LinkmanDelStateChange(PlayerGuid, lDataSize);
		}
	}
}

//! ��Ӧ�����ϵ�˲���
void LinkmanSystem::OnAddLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lGroupID, tagLinkman_data &Linkman_data)
{
	assert(NULL != pOptName);
	LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	if (-1 == lIdx) return;

	//! ���ӵ�����ڱ���
	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		assert(NULL != GetGame()->FindPlayer(OwnerGuid));
		//! �ҵ���ϵ����
		if((long)ite->second.size() <= lIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_16", szGUID));
			return;
		}
		LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

		//! ������ϵ�����ݶ���
		tagLinkman Linkman;
		memmove(&Linkman, &Linkman_data, sizeof(tagLinkman_data));

		//! �����ϵ��
		tmpLinkmanGroup.AddMember(Linkman, TRUE);	
	}

	//! �����������ڱ���
	ite = m_mapPlayerLinkman.find(Linkman_data.guid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		assert(NULL != GetGame()->FindPlayer(Linkman_data.guid));
		//! �ҵ���ϵ����
		//! �ҵ���ϵ����
		if((long)ite->second.size() <= lIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_17", szGUID));
			return;
		}
		LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

		tagLinkman_DB Linkman_DB;
		Linkman_DB.AimGUID = Linkman_data.guid;
		Linkman_DB.OwnerGUID = OwnerGuid;
		memmove(Linkman_DB.szAimName, Linkman_data.szName, MAX_PLAYER_NAME_SIZE);
		Linkman_DB.uGroupID = lGroupID;
		Linkman_DB.uDelState = eLDS_NotDel;

		tmpLinkmanGroup.AddCompriseMePlayer(Linkman_DB, pOptName);
	}

}
//! ��Ӧɾ����ϵ�˲���
void LinkmanSystem::OnDelLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lGroupID, const CGUID &AimGuid, const char *pAimName)
{
	LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	if (-1 == lIdx) return;

	//! ���ӵ�����ڱ���
	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		assert(NULL != GetGame()->FindPlayer(OwnerGuid));
		if((long)ite->second.size() <= lIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_18", szGUID));
			return;
		}
		//! �ҵ���ϵ����
		LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

		//! ɾ����ϵ��
		tmpLinkmanGroup.DelMember(AimGuid, TRUE, pAimName);	

		//! ��������Ҫ�жϳƺ�
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
		if (NULL != pLinkmanGroupSetup && eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if (eAAT_Spouse == pLinkmanGroupSetup->m_uAutoAddWay)
			{
				CPlayer *pPlayer = GetGame()->FindPlayer(OwnerGuid);
				pPlayer->UpdateTitle(TRUE);
			}
		}
	}

	//! �����������ڱ���
	ite = m_mapPlayerLinkman.find(AimGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		assert(NULL != GetGame()->FindPlayer(AimGuid));
		//! �ҵ���ϵ����
		if((long)ite->second.size() <= lIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_19", szGUID));
			return;
		}
		LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];
		tmpLinkmanGroup.DelCompriseMePlayer(OwnerGuid, pOptName);

	}
}
//! ��Ӧ�ƶ���ϵ�˲���
void LinkmanSystem::OnMoveLinkMan(const CGUID &OwnerGuid, const char *pOptName, LONG lSrcGroupID, LONG lDesGroupID, tagLinkman_data &Linkman_data)
{
	LONG lSrcIdx = m_LGSManage.GetIndexByID(lSrcGroupID);
	LONG lDesIdx = m_LGSManage.GetIndexByID(lDesGroupID);
	if (-1 == lSrcIdx || -1 == lDesIdx)
	{
        //��WS��������Ϣ�У��յ�����ϵ�����Ǵ����!
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_20"));
		return;
	}
	const LinkmanGroupSetup* pSrcGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx);
	const LinkmanGroupSetup* pDesGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
	if(NULL == pSrcGroupSetup || NULL == pDesGroupSetup) {assert(false); return;}

	//! ���ӵ�����ڱ���
	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		if((long)ite->second.size() <= lSrcIdx || (long)ite->second.size() <= lDesIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_21", szGUID));
			return;
		}
		assert(NULL != GetGame()->FindPlayer(OwnerGuid));

		if (eKW_ServerKeep == pSrcGroupSetup->m_uKeepWay)
		{
			(ite->second)[lSrcIdx].DelMember(Linkman_data.guid, FALSE, Linkman_data.szName);
		}

		//! ������ϵ�����ݶ���
		tagLinkman Linkman;
		memmove(&Linkman, &Linkman_data, sizeof(tagLinkman_data));
		if (eKW_ServerKeep == pDesGroupSetup->m_uKeepWay)
		{	
			(ite->second)[lDesIdx].AddMember(Linkman, FALSE);
		}

		CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
		msgReAdd.Add((LONG)TRUE);
		msgReAdd.Add(lSrcGroupID);
		msgReAdd.Add(lDesGroupID);
		msgReAdd.AddEx(&Linkman, sizeof(tagLinkman));
		msgReAdd.SendToPlayer(OwnerGuid);
	}

	//! �����������ڱ���
	ite = m_mapPlayerLinkman.find(Linkman_data.guid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		if((long)ite->second.size() <= lSrcIdx || (long)ite->second.size() <= lDesIdx)
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //���[%s]���ѵ�����С��WS��������������
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_22", szGUID));
			return;
		}
		assert(NULL != GetGame()->FindPlayer(Linkman_data.guid));

		tagLinkman_DB Linkman_DB;
		Linkman_DB.AimGUID = Linkman_data.guid;
		Linkman_DB.OwnerGUID = OwnerGuid;
		memmove(Linkman_DB.szAimName, Linkman_data.szName, MAX_PLAYER_NAME_SIZE);
		Linkman_DB.uDelState = eLDS_NotDel;

		if (eKW_ServerKeep == pSrcGroupSetup->m_uKeepWay)
		{
			Linkman_DB.uGroupID = lSrcGroupID;
			(ite->second)[lSrcIdx].DelCompriseMePlayer(Linkman_DB.AimGUID, pOptName);
		}

		if (eKW_ServerKeep == pDesGroupSetup->m_uKeepWay)
		{
			Linkman_DB.uGroupID = lDesGroupID;
			(ite->second)[lDesIdx].AddCompriseMePlayer(Linkman_DB, pOptName);
		}
	}
}


//! ��Ӧ�����Ϣ�ı���Ϣ
void LinkmanSystem::OnPlayerInfoChange(CMessage *pMsg)
{
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);

	//! �õ��ı����Ϣ���ͺ�ֵ
	LONG lInfoType = pMsg->GetLong();
	LONG lValue	= 0;
	char szValue[MAX_PLAYER_TITLE_SIZE] = {0};

	switch(lInfoType)
	{
	case eLIT_Map://! ��ͼ
		pMsg->GetStr(szValue, MAX_PLAYER_TITLE_SIZE);
		break;
	case eLIT_Level://! �ȼ�
	case eLIT_Occupation://! ְҵ
		lValue = pMsg->GetLong();
	    break;
	default:
	    return;
	}

	LONG lGuidNum = pMsg->GetLong();

	for (LONG i = 0; i < lGuidNum; ++i)
	{
		CGUID ResponserGuid;
		pMsg->GetGUID(ResponserGuid);

		map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(ResponserGuid);
		if (m_mapPlayerLinkman.end() == ite) continue;

		for (LONG j = 0; j < (long)ite->second.size(); ++j)
		{
			const LinkmanGroupSetup *PLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(j);
			assert(PLinkmanGroupSetup);
			if(NULL == PLinkmanGroupSetup) 
			{
				Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"PLinkman GroupSetup index error!!! index value = %d", j);
				continue;
			}
			if (eKW_ServerKeep == PLinkmanGroupSetup->m_uKeepWay && eTIT_Show_All == PLinkmanGroupSetup->m_uTimelyInfo)
			{
				switch(lInfoType)
				{
				case eLIT_Map://! ��ͼ
					ite->second[j].LinkmanMapChange(PlayerGuid, szValue);
					break;
				case eLIT_Level://! �ȼ�
					ite->second[j].LinkmanLevelChange(PlayerGuid, lValue);
					break;
				case eLIT_Occupation://! ְҵ
					ite->second[j].LinkmanOccuChange(PlayerGuid, lValue);
					break;
				}
			}
		}

	}
}




//! ��Ӧ���������ϵ��
void LinkmanSystem::OnRequestAdd(CMessage *pMsg)
{
	//! new code
	{
		const CGUID &PlayerGuid = pMsg->GetPlayerID();
		ULONG lGroupID = pMsg->GetLong();
		char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
		pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

 
		//! -------------ȷ�ϲ����Ϸ���-------------
		LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
		if (-1 == lIdx) return;
		const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
		if(NULL == pLinkmanGroupSetup) 
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"LinkmanSystem::OnRequestAdd������");
			return;
		}
		if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
		if (!pLinkmanGroupSetup->m_bUserAdd) return;
		
		//! -------------��WS������Ϣ-------------
		CMessage msgToWS(MSG_S2W_LINKMAN_PALYER_REQUEST_ADD);
		msgToWS.Add(PlayerGuid);
		msgToWS.Add(lGroupID);
		msgToWS.Add(szAimName);	
		msgToWS.Send();
	}
	return;

	//const CGUID &PlayerGuid = pMsg->GetPlayerID();
	//map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(PlayerGuid);
	//if (m_mapPlayerLinkman.end() == ite) return;
	//
	//ULONG lGroupID = pMsg->GetLong();
	//LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	//if (-1 == lIdx) return;

	//char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	//pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

	//const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);

	////! ȷ�ϺϷ���
	//if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
	//if (!pLinkmanGroupSetup->m_bUserAdd) return;

	////! �ҵ���ϵ����
	//LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

	//CMessage msgReAdd(MSG_S2C_LINKMAN_ADD);

	////! �ҵ����
	//CPlayer *pAimPlayer = GetGame()->FindPlayer(szAimName);

	////! �������ͬһ��GS�ϣ������
	//if (NULL == pAimPlayer)
	//{
	//	msgReAdd.Add((LONG)FALSE);
	//	msgReAdd.Add((LONG)eOET_AimNotBeing);
	//	msgReAdd.Add(szAimName);
	//	msgReAdd.SendToPlayer(PlayerGuid);
	//	return;
	//}

	////! ������߲������Լ�
	//if (pAimPlayer->GetExID() == PlayerGuid)
	//{
	//	return;
	//}
	//
	//const CGUID &AimGuid = pAimPlayer->GetExID();
	////! ȷ�����û�б����������������
	//for (LONG i = 0; i < ite->second.size(); ++i)
	//{
	//	if(ite->second[i].FindMember(AimGuid)) return;
	//}

	////! �ж�����
	//if (tmpLinkmanGroup.GetMemberNum() >= pLinkmanGroupSetup->m_uMaxMemberNum) return;

	////! �Ƿ���ҪĿ��ȷ��
	//if (pLinkmanGroupSetup->m_bAddAimAffirm)
	//{
	//	//! �����Ự����
	//	tagOptSession OptSession;
	//	OptSession._uEndTime	= time(NULL) + MAX_LINKMAN_OPT_TIME_SEC;
	//	OptSession._uOptType	= eLOT_Add;
	//	OptSession._uSrcGroupID	= INVALID_GROUP_ID;
	//	OptSession._uDesGroupID	= lGroupID;

	//	if(CreateAddSession(AimGuid, PlayerGuid, OptSession))
	//	{
	//		CMessage msgrEquestAdd(MSG_S2C_LINKMAN_REQUEST_ADD);
	//		msgrEquestAdd.Add(lGroupID);
	//		msgrEquestAdd.Add(pMsg->GetPlayer()->GetName());
	//		msgrEquestAdd.Add(PlayerGuid);
	//		msgrEquestAdd.SendToPlayer(AimGuid);
	//	}
	//	else
	//	{
	//		msgReAdd.Add((LONG)FALSE);
	//		msgReAdd.Add((LONG)eOET_OverfullToOne);
	//		msgReAdd.Add(szAimName);
	//		msgReAdd.SendToPlayer(PlayerGuid);
	//	}
	//}
	//else
	//{
	//	//! ������ϵ�����ݶ���
	//	tagLinkman Linkman;
	//	assert(m_mapPlayerLinkman.find(AimGuid) != m_mapPlayerLinkman.end());
	//	Linkman.bMutual = m_mapPlayerLinkman[AimGuid][lIdx].FindMember(PlayerGuid);
	//	Linkman.guid = AimGuid;
	//	Linkman.lLevel = pAimPlayer->GetLevel();
	//	Linkman.lOccupation = pAimPlayer->GetOccupation();
	//	Linkman.uState = eLST_Online;
	//	memmove(Linkman.szName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	//	memmove(Linkman.szMapName, pAimPlayer->GetFather()->GetName(), MAX_PLAYER_TITLE_SIZE - 1);
	//	
	//	//! �����ϵ��
	//	tmpLinkmanGroup.AddMember(Linkman, TRUE);
	//	// GetGameLogInterface()->logT450_linkman_opt_log(pMsg->GetPlayer(), AimGuid, Linkman.szName, 1, lGroupID);
	//	//! ���[����ҵ���]
	//	
	//	m_mapPlayerLinkman[AimGuid][lIdx].AddCompriseMePlayer(PlayerGuid);
	//}
}

//! ��Ӧ����ɾ����ϵ��
void LinkmanSystem::OnRequestDel(CMessage *pMsg)
{
	//! new code
	{
		const CGUID &PlayerGuid = pMsg->GetPlayerID();
		ULONG lGroupID = pMsg->GetLong();
		char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
		pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);


		//! -------------ȷ�ϲ����Ϸ���-------------
		LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
		if (-1 == lIdx) return;
		const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
		if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
		if (!pLinkmanGroupSetup->m_bUserDel) return;

		//! -------------��WS������Ϣ-------------
		CMessage msgToWS(MSG_S2W_LINKMAN_PALYER_REQUEST_DEL);
		msgToWS.Add(PlayerGuid);
		msgToWS.Add(lGroupID);
		msgToWS.Add(szAimName);	
		msgToWS.Add((BYTE)false);
		msgToWS.Send();
	}
	return;

	//const CGUID &PlayerGuid = pMsg->GetPlayerID();
	//map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(PlayerGuid);
	//if (m_mapPlayerLinkman.end() == ite) return;

	//ULONG lGroupID = pMsg->GetLong();
	//LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	//if (-1 == lIdx) return;

	//char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	//pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

	//const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);

	////! ȷ�ϺϷ���
	//if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
	//if (!pLinkmanGroupSetup->m_bUserDel) return;

	////! �ҵ���ϵ����
	//LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

	////! �õ���ɾ���ߵ�ID
	//CGUID AimGuid = tmpLinkmanGroup.FindMemberID(szAimName);
	//if (NULL_GUID == AimGuid) return;

	//tmpLinkmanGroup.DelMember(AimGuid, TRUE, szAimName);
	//// GetGameLogInterface()->logT450_linkman_opt_log(pMsg->GetPlayer(), AimGuid, szAimName, 2, lGroupID);

	////! ���±�ɾ���ߵ�[����ҵ���ϵ��]�б�
	//map<CGUID, VecLinkmanGroup>::iterator iteLinkmanGroup =	m_mapPlayerLinkman.find(AimGuid); 
	//if(iteLinkmanGroup != m_mapPlayerLinkman.end())
	//{
	//	iteLinkmanGroup->second[lIdx].DelCompriseMePlayer(PlayerGuid);
	//}
}

//! ��Ӧ�����ƶ���ϵ��
void LinkmanSystem::OnRequestMove(CMessage *pMsg)
{
	//! new code
	{
		const CGUID &OwnerGuid = pMsg->GetPlayerID();
		ULONG lSrcGroupID	= pMsg->GetLong();
		ULONG lDesGroupID	= pMsg->GetLong();
		char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
		pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

		//! -------------ȷ�ϲ����Ϸ���-------------
		LONG lSrcIdx = m_LGSManage.GetIndexByID(lSrcGroupID);
		LONG lDesIdx = m_LGSManage.GetIndexByID(lDesGroupID);
		if (-1 == lSrcIdx || -1 == lDesIdx) return;
		const LinkmanGroupSetup* pSrcGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx);
		const LinkmanGroupSetup* pDesGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
		BOOL bNeedServerOpt = FALSE;
		if (eKW_ServerKeep == pSrcGroupSetup->m_uKeepWay) 
		{
			bNeedServerOpt = TRUE;
			if (!pSrcGroupSetup->m_bUserDel) return;
		}
		if (eKW_ServerKeep == pDesGroupSetup->m_uKeepWay) 
		{	
			bNeedServerOpt = TRUE;
			if (!pDesGroupSetup->m_bUserAdd) return;
		}
		if(!bNeedServerOpt) return;

		//! -------------��WS������Ϣ-------------
		CMessage msgToWS(MSG_S2W_LINKMAN_PALYER_REQUEST_MOVE);
		msgToWS.Add(OwnerGuid);
		msgToWS.Add(lSrcGroupID);
		msgToWS.Add(lDesGroupID);
		msgToWS.Add(szAimName);
		msgToWS.Send();
	}
	return;

	////! ��������
	//ULONG lSrcGroupID	= pMsg->GetLong();
	//ULONG lDesGroupID	= pMsg->GetLong();

	//LONG lSrcIdx = m_LGSManage.GetIndexByID(lSrcGroupID);
	//LONG lDesIdx = m_LGSManage.GetIndexByID(lDesGroupID);

	////! �����������
	//char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	//pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);
	//CPlayer *pAimPlayer = GetGame()->FindPlayer(szAimName);
	//CGUID AimGuid;

	////! ���������
	//const CGUID &OwnerGuid = pMsg->GetPlayerID();

	////! ��������
	//map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	//if (m_mapPlayerLinkman.end() == ite) return;
	//VecLinkmanGroup &vLinkmanGroup = ite->second;

	////! ����֤ɾ��
	//BOOL bRunDel = TRUE;
	//const LinkmanGroupSetup* pDelSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx);
	//if (eKW_ServerKeep != pDelSetup->m_uKeepWay)
	//{
	//	bRunDel = FALSE;
	//}
	//else
	//{
	//	if(!pDelSetup->m_bUserDel) return;//! ������ͻ���ɾ������
	//	AimGuid = vLinkmanGroup[lSrcIdx].FindMemberID(szAimName);
	//	if (NULL_GUID == AimGuid) return;//! ɾ�������ڷ�����
	//}

	////! ����֤���
	//BOOL bRunAdd = TRUE;
	//const LinkmanGroupSetup* pAddSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
	//if (eKW_ServerKeep != pAddSetup->m_uKeepWay)
	//{
	//	bRunAdd = FALSE;
	//}
	//else
	//{
	//	if(!pAddSetup->m_bUserAdd) return; //! ������ͻ�����Ӳ���
	//	for (LONG i = 0; i < vLinkmanGroup.size(); ++i)
	//	{
	//		if (i != lSrcIdx)
	//			if (NULL_GUID != vLinkmanGroup[i].FindMemberID(szAimName)) return;
	//	}
	//	if (NULL_GUID != vLinkmanGroup[lDesIdx].FindMemberID(szAimName)) return;//! ��Ӷ����Ѵ���
	//	if ( NULL == pAimPlayer) //! ��Ҳ����ߣ��Ҳ�����ҵ�ID���޷�����
	//	{
	//		CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
	//		msgReAdd.Add((LONG)FALSE);
	//		msgReAdd.Add((LONG)eOET_AimNotBeing);
	//		msgReAdd.Add(szAimName);
	//		msgReAdd.SendToPlayer(OwnerGuid);
	//		return;
	//	}
	//	if (pAimPlayer->GetExID() == OwnerGuid)//! ��������ǲ������Լ�
	//	{
	//		return;
	//	}
	//	if(vLinkmanGroup[lDesIdx].GetMemberNum() >= m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx)->m_uMaxMemberNum)
	//	{
	//		CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
	//		msgReAdd.Add((LONG)FALSE);
	//		msgReAdd.Add((LONG)eOET_ListFull);
	//		msgReAdd.Add(szAimName);
	//		msgReAdd.SendToPlayer(OwnerGuid);
	//		return;
	//	}
	//	AimGuid = pAimPlayer->GetExID();
	//}

	//if ((!bRunDel) && (!bRunAdd)) return;//! �����鶼�����ڿͻ��ˣ�����Ҫ������ִ��


	////! ��ʼִ��
	//if (bRunAdd)
	//{
	//	if (pAddSetup->m_bAddAimAffirm) //! ���Ҫ��Ŀ��ȷ��
	//	{
	//		//! �����Ự����
	//		tagOptSession OptSession;
	//		OptSession._uEndTime	= time(NULL) + MAX_LINKMAN_OPT_TIME_SEC;
	//		OptSession._uOptType	= eLot_Move;
	//		OptSession._uSrcGroupID	= lSrcGroupID;
	//		OptSession._uDesGroupID	= lDesGroupID;

	//		if(CreateAddSession(AimGuid, OwnerGuid, OptSession))
	//		{
	//			CMessage msgrEquestAdd(MSG_S2C_LINKMAN_REQUEST_ADD);
	//			msgrEquestAdd.Add(lDesGroupID);
	//			msgrEquestAdd.Add(pMsg->GetPlayer()->GetName());
	//			msgrEquestAdd.Add(OwnerGuid);
	//			msgrEquestAdd.SendToPlayer(AimGuid);
	//		}
	//		else
	//		{
	//			CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
	//			msgReAdd.Add((LONG)FALSE);
	//			msgReAdd.Add((LONG)eOET_OverfullToOne);
	//			msgReAdd.Add(szAimName);
	//			msgReAdd.SendToPlayer(OwnerGuid);
	//		}

	//		return;
	//	}
	//}

	//map<CGUID, VecLinkmanGroup>::iterator iteAim = m_mapPlayerLinkman.find(AimGuid);
	//if (m_mapPlayerLinkman.end() == iteAim) return;
	//VecLinkmanGroup &vAimLinkmanGroup = iteAim->second;

	//if (bRunDel)
	//{
	//	vLinkmanGroup[lSrcIdx].DelMember(AimGuid, FALSE, szAimName);
	//	// GetGameLogInterface()->logT450_linkman_opt_log(pMsg->GetPlayer(), AimGuid, szAimName, 2, lSrcGroupID);
	//	vAimLinkmanGroup[lSrcIdx].DelCompriseMePlayer(OwnerGuid);
	//}

	////! ������ϵ�����ݶ���
	//tagLinkman Linkman;
	//memmove(Linkman.szName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	//if (bRunAdd)
	//{
	//	Linkman.bMutual = vAimLinkmanGroup[lDesIdx].FindMember(OwnerGuid);
	//	Linkman.guid = AimGuid;
	//	Linkman.lLevel = pAimPlayer->GetLevel();
	//	Linkman.lOccupation = pAimPlayer->GetOccupation();
	//	Linkman.uState = eLST_Online;
	//	memmove(Linkman.szMapName, pAimPlayer->GetFather()->GetName(), MAX_PLAYER_TITLE_SIZE - 1);

	//	vLinkmanGroup[lDesIdx].AddMember(Linkman, FALSE);
	//	// GetGameLogInterface()->logT450_linkman_opt_log(pMsg->GetPlayer(), AimGuid, szAimName, 1, lDesGroupID);
	//	vAimLinkmanGroup[lDesIdx].AddCompriseMePlayer(OwnerGuid);
	//}

	//CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
	//msgReAdd.Add((LONG)TRUE);
	//msgReAdd.Add(lSrcGroupID);
	//msgReAdd.Add(lDesGroupID);
	//msgReAdd.AddEx(&Linkman, sizeof(tagLinkman));
	//msgReAdd.SendToPlayer(OwnerGuid);

}

//! ��Ӧ��һظ������
void LinkmanSystem::OnAnswerAdd(CMessage *pMsg)
{
	{
		ULONG lGroupID	= pMsg->GetLong();
		CGUID RequestPlayerGuid;
		pMsg->GetGUID(RequestPlayerGuid);
		BOOL lAgree		= pMsg->GetLong();
		const CGUID &RePlayerGuid = pMsg->GetPlayerID();

		CMessage msgToWS(MSG_S2W_LINKMAN_PALYER_RE_ADD);
		msgToWS.Add(lGroupID);
		msgToWS.Add(RequestPlayerGuid);
		msgToWS.Add((LONG)lAgree);
		msgToWS.Add(RePlayerGuid);
		msgToWS.Send();
	}
	return;

	//ULONG lGroupID	= pMsg->GetLong();
	//CGUID PlayerGuid;
	//pMsg->GetGUID(PlayerGuid);
	//BOOL lAgree		= pMsg->GetLong();
	//const CGUID &AimGuid = pMsg->GetPlayerID();

	////! ������֤�Ự�Ķ���
	//tagOptSession OptSession;
	//OptSession._uSrcGroupID = INVALID_GROUP_ID;
	//OptSession._uOptType	= eLOT_Add;
	//OptSession._uDesGroupID = lGroupID;

	//BOOL bLegitimate = FALSE;
	//if (EndAddSession(AimGuid, PlayerGuid, OptSession))
	//{
	//	bLegitimate = TRUE;
	//}
	//else if (eLot_Move == OptSession._uOptType)
	//{
	//	bLegitimate = TRUE;
	//}

	//if (bLegitimate)
	//{
	//	CPlayer *pAimPlayer = pMsg->GetPlayer();
	//	if (lAgree)
	//	{
	//		LONG lDesIdx = m_LGSManage.GetIndexByID(OptSession._uDesGroupID);

	//		map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(PlayerGuid);
	//		if (m_mapPlayerLinkman.end() == ite) return;
	//		LinkmanGroup &DesLinkmanGroup = (ite->second)[lDesIdx];

	//		//! ����������ˣ��򷵻ش�����ʾ
	//		if(DesLinkmanGroup.GetMemberNum() > m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx)->m_uMaxMemberNum)
	//		{
	//			CMessage msg(MSG_S2C_LINKMAN_ADD);
	//			msg.Add((LONG)FALSE);
	//			msg.Add((LONG)eOET_ListFull);
	//			msg.Add(pAimPlayer->GetName());
	//			msg.SendToPlayer(PlayerGuid);
	//			return;
	//		}

	//		//! ��������ƶ�������ϵ�����������򷵻�ʧ��
	//		if (eLot_Move != OptSession._uOptType)
	//		{
	//			for (LONG i = 0; i < ite->second.size(); ++i)
	//			{
	//				if (i != lDesIdx)
	//				{
	//					if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder(i)->m_uKeepWay)
	//					{
	//						if (ite->second[i].FindMember(AimGuid))
	//						{
	//							CMessage msg(MSG_S2C_LINKMAN_ADD);
	//							msg.Add((LONG)FALSE);
	//							msg.Add((LONG)eOET_AimInOther);
	//							msg.Add(pAimPlayer->GetName());
	//							msg.SendToPlayer(PlayerGuid);
	//							return;
	//						}
	//					}
	//				}
	//			}
	//		}

	//		//! ������ϵ�����ݶ���
	//		assert(m_mapPlayerLinkman.find(AimGuid) != m_mapPlayerLinkman.end());
	//		tagLinkman Linkman;
	//		Linkman.bMutual = m_mapPlayerLinkman[AimGuid][lDesIdx].FindMember(PlayerGuid);
	//		Linkman.guid = AimGuid;
	//		Linkman.lLevel = pAimPlayer->GetLevel();
	//		Linkman.lOccupation = pAimPlayer->GetOccupation();
	//		Linkman.uState = eLST_Online;
	//		memmove(Linkman.szName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	//		memmove(Linkman.szMapName, pAimPlayer->GetFather()->GetName(), MAX_PLAYER_TITLE_SIZE - 1);

	//		CPlayer *pSrcPlayer = GetGame()->FindPlayer(PlayerGuid);
	//		if (eLot_Move == OptSession._uOptType)
	//		{
	//			LONG lSrcIdx = m_LGSManage.GetIndexByID(OptSession._uSrcGroupID);
	//			if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx)->m_uKeepWay)
	//			{
	//				LinkmanGroup &SrcLinkmanGroup = (ite->second)[lSrcIdx];

	//				SrcLinkmanGroup.DelMember(AimGuid, FALSE, pAimPlayer->GetName());
	//				// GetGameLogInterface()->logT450_linkman_opt_log(pSrcPlayer, AimGuid, pAimPlayer->GetName(), 2, OptSession._uSrcGroupID);

	//				assert(m_mapPlayerLinkman.find(AimGuid) != m_mapPlayerLinkman.end());
	//				m_mapPlayerLinkman[AimGuid][lSrcIdx].DelCompriseMePlayer(PlayerGuid);
	//			}

	//			//! �����ϵ��
	//			DesLinkmanGroup.AddMember(Linkman, FALSE);
	//			// GetGameLogInterface()->logT450_linkman_opt_log(pSrcPlayer, AimGuid, pAimPlayer->GetName(), 1, OptSession._uDesGroupID);

	//			CMessage msgReAdd(MSG_S2C_LINKMAN_MOVE);
	//			msgReAdd.Add((LONG)TRUE);
	//			msgReAdd.Add(OptSession._uSrcGroupID);
	//			msgReAdd.Add(OptSession._uDesGroupID);
	//			msgReAdd.AddEx(&Linkman, sizeof(tagLinkman));
	//			msgReAdd.SendToPlayer(PlayerGuid);
	//		}
	//		else
	//		{	
	//			//! �����ϵ��
	//			DesLinkmanGroup.AddMember(Linkman, TRUE);
	//			// GetGameLogInterface()->logT450_linkman_opt_log(pSrcPlayer, AimGuid, pAimPlayer->GetName(), 1, lGroupID);
	//		}

	//		//! ���[����ҵ���]
	//		m_mapPlayerLinkman[AimGuid][lDesIdx].AddCompriseMePlayer(PlayerGuid);

	//	}
	//	else
	//	{
	//		CMessage msg(MSG_S2C_LINKMAN_ADD);
	//		msg.Add((LONG)lAgree);
	//		msg.Add((LONG)eOET_AimReject);
	//		msg.Add(pAimPlayer->GetName());
	//		msg.SendToPlayer(PlayerGuid);
	//	}
	//}
}

//! ������ϵ�����ݵ�WS
void LinkmanSystem::SendDataToWs(void)
{
	CMessage msgToWs(MSG_S2W_LINKMAN_DATA);
	vector<BYTE> vLinkmanData;

	//! �ȸ��������ռ��λ��
	_AddToByteArray(&vLinkmanData, (LONG)0L);

	//! ʵ�ʷ�������ͳ��
	LONG lNum = 0;
	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.begin();
	for (; ite != m_mapPlayerLinkman.end(); ++ite)
	{
		if(NULL == GetGame()->FindPlayer(ite->first))
		{
			if(NULL == GetGame()->FindSavePlayer(ite->first))
				continue;
		}

		++lNum;
		//! ���GUID
		_AddToByteArray(&vLinkmanData, ite->first);
		VecLinkmanGroup &vLinkmanGroup = ite->second;
		//! ��ϵ������
		_AddToByteArray(&vLinkmanData, (LONG)vLinkmanGroup.size());
		for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
		{
			//! ��ϵ������
			vLinkmanGroup[i].Serialize_ForWS(&vLinkmanData);
		}
	}

	//! �ڱ������滻ʵ������
	memcpy(&vLinkmanData[0], &lNum, sizeof(LONG));

	msgToWs.Add((LONG)vLinkmanData.size());
	msgToWs.AddEx(&vLinkmanData[0], (long)vLinkmanData.size());
	msgToWs.Send();
}	

//!								��Ӧ����˳�������
void LinkmanSystem::OnPlayerExit(const CGUID &PlayerGuid)
{
	if(GetGame()->GetWSSocket() > 0)
		m_mapPlayerLinkman.erase(PlayerGuid);
}

//! ��¼PK��������
void LinkmanSystem::NotePkPlayer(CPlayer *pDecedent, CPlayer *pMurderer)
{
	assert(NULL != pDecedent && NULL != pMurderer);
	if (NULL == pDecedent || NULL == pMurderer)
	{
        //PK�߻��߱�ɱ���ߵĶ���ָ��Ϊ�գ���ӳ���ʧ�ܣ�
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_23"));
		return;
	}

	assert(pDecedent->GetExID() != pMurderer->GetExID());
	if(pDecedent->GetExID() == pMurderer->GetExID())
	{
        //PK���뱻ɱ����Ϊͬһ���ˣ���ӳ���ʧ�ܣ�
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_24"));
		return;
	}

	if(m_mapPlayerLinkman.find(pDecedent->GetExID()) == m_mapPlayerLinkman.end())
	{
        //��ӳ���ʱ���Ҳ����ܺ���ҵĺ��Ѷ���
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_25"));
		return;
	}
	VecLinkmanGroup &vLinkmanGroup = m_mapPlayerLinkman[pDecedent->GetExID()];

	//! �ҳ��Ƿ������ĳ�����У�����������
	for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
	{
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if(vLinkmanGroup[i].FindMember(pMurderer->GetExID()))
			{
				return;
			}
		}
	}

	//! ���
	for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
	{
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if (eAAT_Attacker == (long)pLinkmanGroupSetup->m_uAutoAddWay && vLinkmanGroup[i].GetMemberNum() < (long)pLinkmanGroupSetup->m_uMaxMemberNum)
			{
				CMessage msgToWS(MSG_S2W_LINKMAN_SYSTEM_REQUEST_ADD);
				msgToWS.Add(pDecedent->GetExID());
				msgToWS.Add(pLinkmanGroupSetup->m_uID);
				msgToWS.Add(pMurderer->GetName());	
				msgToWS.Send();

				return;
			}
		}
	}
}

//! ��¼������
void LinkmanSystem::AddSpouse(CPlayer *pSelf, CPlayer *pSide)
{
	assert(NULL != pSelf && NULL != pSide);
	if (NULL == pSelf || NULL == pSide)
	{
        //��ż����ָ��Ϊ�գ������żʧ��!
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_26"));
		return;
	}

	assert(pSelf->GetExID() != pSide->GetExID());
	if(pSelf->GetExID() == pSide->GetExID())
	{
        //���������Ϊͬһ���ˣ���ӳ���ʧ�ܣ�
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_27"));
		return;
	}

	if(m_mapPlayerLinkman.find(pSelf->GetExID()) == m_mapPlayerLinkman.end())
	{
        //�����żʱ���Ҳ��������ҵĺ��Ѷ���
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PLAYER_28"));
		return;
	}

	//! ���
	VecLinkmanGroup &vLinkmanGroup = m_mapPlayerLinkman[pSelf->GetExID()];
	for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
	{
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if (eAAT_Spouse == (long)pLinkmanGroupSetup->m_uAutoAddWay && vLinkmanGroup[i].GetMemberNum() < (long)pLinkmanGroupSetup->m_uMaxMemberNum)
			{
				CMessage msgToWS(MSG_S2W_LINKMAN_SYSTEM_REQUEST_ADD);
				msgToWS.Add(pSelf->GetExID());
				msgToWS.Add(pLinkmanGroupSetup->m_uID);
				msgToWS.Add(pSide->GetName());	
				msgToWS.Send();

				return;
			}
		}
	}
}

//! ɾ��������
void LinkmanSystem::DelSpouse(CPlayer *pSelf)
{
	if (NULL == pSelf)
		return;

	VecLinkmanGroup &vLinkmanGroup = m_mapPlayerLinkman[pSelf->GetExID()];
	for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
	{
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if (eAAT_Spouse == pLinkmanGroupSetup->m_uAutoAddWay && 0 < vLinkmanGroup[i].GetMemberNum())
			{
				const	tagLinkman *pSpouse = vLinkmanGroup[i].GetFirstMember();
				if(NULL != pSpouse)
				{
					CMessage msgToWS(MSG_S2W_LINKMAN_PALYER_REQUEST_DEL);
					msgToWS.Add(pSelf->GetExID());
					msgToWS.Add(pLinkmanGroupSetup->m_uID);
					msgToWS.Add(pSpouse->szName);
					msgToWS.Add((BYTE)true);
					msgToWS.Send();
				}
				return;
			}
		}
	}
}

//!								�ҵ���������Ϣ
const tagLinkman* LinkmanSystem::FindSpouseInfo(CPlayer *pSelf)
{
	const tagLinkman *pRe = NULL; 

	if (NULL == pSelf)
		return pRe;
	
	VecLinkmanGroup &vLinkmanGroup = m_mapPlayerLinkman[pSelf->GetExID()];
	for (LONG i = 0; i < (long)vLinkmanGroup.size(); ++i)
	{
		const	LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			if (eAAT_Spouse == pLinkmanGroupSetup->m_uAutoAddWay && 0 < vLinkmanGroup[i].GetMemberNum())
			{
				pRe = vLinkmanGroup[i].GetFirstMember();
				break;
			}
		}
	}

	return pRe;
}


//! �ж�һ����ҡ�OwnerGuid���Ƿ�����һ����ҡ�pOtherOneName��������ָ���Ĺ�ϵ
BOOL LinkmanSystem::CheckRelation(const CGUID &OwnerGuid, const char *pOtherOneName, eLinkmanGroupType eRelationType)
{
	LONG lIdx = m_LGSManage.GetIndexByType(eRelationType);
	if (-1 == lIdx) return FALSE;

	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		//! �ҵ���ϵ����
		if((long)ite->second.size() > lIdx)
		{
			LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

			if(NULL_GUID != tmpLinkmanGroup.FindMemberID(pOtherOneName))
				return TRUE;
		}
		else
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //������Ӧ�õĴ���, ��ҵ���ϵ��������û�е�[%d]��,��Ҫ���
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_29", lIdx));
		}	
	}	

	return FALSE;
}

//! �ж�һ����ҡ�OwnerGuid���Ƿ�����һ����ҡ�OtherOneGuid��������ָ���Ĺ�ϵ
BOOL LinkmanSystem::CheckRelation(const CGUID &OwnerGuid, const CGUID &OtherOneGuid, eLinkmanGroupType eRelationType)
{
	LONG lIdx = m_LGSManage.GetIndexByType(eRelationType);
	if (-1 == lIdx) return FALSE;

	map<CGUID, VecLinkmanGroup>::iterator ite = m_mapPlayerLinkman.find(OwnerGuid);
	if (m_mapPlayerLinkman.end() != ite)
	{
		//! �ҵ���ϵ����
		if((long)ite->second.size() > lIdx)
		{
			LinkmanGroup &tmpLinkmanGroup = (ite->second)[lIdx];

			return tmpLinkmanGroup.FindMember(OtherOneGuid);
		}
		else
		{
			char szGUID[256] = {0};
			OwnerGuid.tostring(szGUID);
            //������Ӧ�õĴ���, ��ҵ���ϵ��������û�е�[%d]��,��Ҫ���
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_30", lIdx));
		}	
	}	

	return FALSE;
}