
#include "StdAfx.h"
#include "LinkmanSystem.h"
#include "../DBEntity/EntityManager.h"
#include "../DBEntity/EntityGroup.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
//!				����ϵͳʵ������
LONG			g_LinkmanSystemInstanceNum = 0;
#endif
//!				����ϵͳȫ��Ψһʵ��
LinkmanSystem	g_LinkmanSystem;


LinkmanSystem::LinkmanSystem(void)
{
#ifdef _DEBUG
	if (0 != g_LinkmanSystemInstanceNum)
		throw;
	++g_LinkmanSystemInstanceNum;
#endif
}

LinkmanSystem::~LinkmanSystem(void)
{	
}


//! �õ�ȫ��Ψһʵ��
LinkmanSystem& LinkmanSystem::GetInstance(void)
{
	return g_LinkmanSystem;
}


//! �õ����ò��ҳ�ʼ������
bool LinkmanSystem::LoadSetupAndInit(const char* pFilename)
{
	if(m_LGSManage.LoadSetup(pFilename))
	{
		//! DB��ϵ������Ƭ�ĵ��ڴ�ռ�Ԥ���䡢�ƶ��������
		//GetGame().GetMemPool().SetInitialTileNum(sizeof(tagLinkman_data), 1024 * 10);

		return true;
	}

	return false;
}

//! ��������Ϣ���뵽����
void LinkmanSystem::SetupAddToByteArray(vector<BYTE>& veData)
{
	m_LGSManage.AddToByteArray(&veData);
}

//! ��һ����ҵ����ݱ��뵽���飬����DBS����
void LinkmanSystem::AddToByteArray_ForDBS(const CGUID &PlayerGuid, CEntityGroup* linkmanGroupOwner, CEntityGroup* linkmanGroupAim)
{
	assert(NULL != linkmanGroupOwner && NULL != linkmanGroupAim);
	linkmanGroupOwner->SetCurDbOperFlag(1);
	linkmanGroupOwner->SetCurDbOperType(DB_OPER_DELETE_INSERT);

	linkmanGroupAim->SetCurDbOperFlag(1);
	linkmanGroupAim->SetCurDbOperType(DB_OPER_ONLY_MEMSAVE);

	if(m_mapPlayerLinkMan.find(PlayerGuid) == m_mapPlayerLinkMan.end())
	{
		linkmanGroupOwner->ReleaseChilds();
		linkmanGroupOwner->SetCurDbOperFlag(0);

		linkmanGroupAim->ReleaseChilds();
		linkmanGroupAim->SetCurDbOperFlag(0);
		return;
	}

	VecPlayerLinkMan &vPlayerLinkman = m_mapPlayerLinkMan[PlayerGuid];
	vector<tagLinkman_DB> vLinkman;
	vector<tagLinkman_DB> vAddMe;
	for (ulong i = 0; i < vPlayerLinkman.size(); ++i)
	{
		if (eKW_ServerKeep == m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i)->m_uKeepWay)
		{
			vPlayerLinkman[i].Serialize_ForDBS(vLinkman);
			vPlayerLinkman[i].AddMeGuidToVector(vAddMe);
		}
	}
	
	linkmanGroupOwner->ReleaseChilds();

	for (size_t i = 0; i < vLinkman.size(); ++i)
	{
		CGUID tguid;
		CGUID::CreateGUID(tguid);
		CBaseEntity* linkMan = GetGame().GetEntityManager()->NewBaseEntity(string("[player_friend]"), tguid);
		vLinkman[i].OwnerGUID = PlayerGuid;
		linkMan->SetGuidAttr(string("OwnerGUID"), PlayerGuid);
		linkMan->SetGuidAttr(string("AimGUID"), vLinkman[i].AimGUID);
		assert(PlayerGuid != vLinkman[i].AimGUID);
		linkMan->SetLongAttr(string("GroupID"), vLinkman[i].uGroupID);
		linkMan->SetLongAttr(string("DelState"), vLinkman[i].uDelState);
		linkMan->SetStringAttr(string("AimName"), vLinkman[i].szAimName);
		linkmanGroupOwner->AddChild(linkMan);

		linkMan->SetCurDbOperFlag(1);
		linkMan->SetCurDbOperType(DB_OPER_DELETE_INSERT);
	}

	linkmanGroupAim->ReleaseChilds();

	for (size_t i = 0; i < vAddMe.size(); ++i)
	{
		CGUID tguid;
		CGUID::CreateGUID(tguid);
		CBaseEntity* linkMan = GetGame().GetEntityManager()->NewBaseEntity(string("[player_friend]"), tguid);
		linkMan->SetGuidAttr(string("OwnerGUID"), vAddMe[i].OwnerGUID);
		assert(NULL_GUID != vAddMe[i].AimGUID);
		linkMan->SetGuidAttr(string("AimGUID"), vAddMe[i].AimGUID);
		assert(PlayerGuid != vAddMe[i].OwnerGUID);
		linkMan->SetLongAttr(string("GroupID"), vAddMe[i].uGroupID);
		linkMan->SetLongAttr(string("DelState"), vAddMe[i].uDelState);
		linkMan->SetStringAttr(string("AimName"), vAddMe[i].szAimName);
		linkmanGroupAim->AddChild(linkMan);

		linkMan->SetCurDbOperFlag(1);
		linkMan->SetCurDbOperType(DB_OPER_ONLY_MEMSAVE);
	}

	//DelPlayerLinkman(PlayerGuid);
}

//! ��Ӧ�������
void LinkmanSystem::OnPlayerOnline(CPlayer *pPlayer)
{
	if(NULL == pPlayer) return;
	if(m_mapPlayerLinkMan.end() == m_mapPlayerLinkMan.find(pPlayer->GetExID()))
	{
		assert(false); //! ����������˵����ȡ��ҵĺ�����Ϣʱʧ���ˣ�Ϊ�˷�ֹ������������½����������ݽṹ
		VecPlayerLinkMan tmpPlayerLinkMan;

		LONG lGroupNum = m_LGSManage.GetGroupNum();
		for (LONG i = 0; i < lGroupNum; ++i)
		{
			LinkmanGroup tmpLinkmanGroup(m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i));

			tmpPlayerLinkMan.push_back(tmpLinkmanGroup);
		}

		m_mapPlayerLinkMan[pPlayer->GetExID()] = tmpPlayerLinkMan;
	}

	VecPlayerLinkMan &vPlayerLinkMan = m_mapPlayerLinkMan[pPlayer->GetExID()];

	

	CMessage msgInit(MSG_W2S_LINKMAN_PLAYER_INITDATA);
	msgInit.Add((BYTE)TRUE);
	msgInit.Add(pPlayer->GetExID());
	map<LONG, vector<CGUID>> mapLoginInfo;

	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{
		const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);

		if (NULL != pLinkmanGroupSetup && eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			////! ��ӻ�����ϵ�����⴦��
			//if (eLGT_Spouse == pLinkmanGroupSetup->m_uGroupType)
			//{
			//	do 
			//	{
			//		//! �ж��Ƿ������������Ϊ�����У���������
			//		if (1 == vPlayerLinkMan[i].GetMeAddMap().size() && 1 == vPlayerLinkMan[i].GetAddMeMap().size())
			//		{
			//			const CGUID &SpouseGuid = vPlayerLinkMan[i].GetAddMeMap().begin()->first;
			//			//! ����ҵĵ��Է�����ϵ�˶����ԶԷ���������ϢΪ׼
			//			if(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(SpouseGuid))
			//			{
			//				VecPlayerLinkMan &vSpouseLinkMan = m_mapPlayerLinkMan[SpouseGuid];
			//				if(1 == vSpouseLinkMan[i].GetMeAddMap().size())
			//				{
			//					if(pPlayer->GetExID() == vSpouseLinkMan[i].GetMeAddMap().begin()->first)
			//						break;
			//				}
			//			}
			//			//! �Ҳ��������Լ�������Ϊ׼������DB�����ԭ������ж�Ч�����ӳ٣�Ҫ�������һ��ʱ����жϲ���ȷ
			//			else if(vPlayerLinkMan[i].GetAddMeMap().begin()->first == vPlayerLinkMan[i].GetMeAddMap().begin()->first)
			//				break;

			//		}
			//		
			//		//! ǰ���ϵ���ж��У��޷�ȷ����ż�ĺϷ��ԣ�ֱ�����������Ϣ
			//		vPlayerLinkMan[i].Release();
			//	} while(false);		
			//}

			//! ͳ����Ҫ��½��Ϣ����Һ�GS������
			const map<CGUID, tagLinkman_DB>& AddMeMap = vPlayerLinkMan[i].GetAddMeMap();
			for (map<CGUID, tagLinkman_DB>::const_iterator ite = AddMeMap.begin(); ite != AddMeMap.end(); ++ite)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(ite->first);
				if (NULL != pPlayer)
				{
					//! ͳ����Ϣ
					if (mapLoginInfo.find(pPlayer->GetGsSocket()) == mapLoginInfo.end())
					{
						mapLoginInfo.insert(pair<LONG, vector<CGUID>>(pPlayer->GetGsSocket(), vector<CGUID>()));
					}
					mapLoginInfo[pPlayer->GetGsSocket()].push_back(ite->first);	
				}
			}

			//! ��������ҵ���ҵĳ�Ա״̬
			const map<CGUID, tagMeAddPlayer>& MeAddMap = vPlayerLinkMan[i].GetMeAddMap();
			for ( map<CGUID, tagMeAddPlayer>::const_iterator ite = MeAddMap.begin(); ite != MeAddMap.end(); ++ite)
			{
				map<CGUID, VecPlayerLinkMan>::iterator iteMeAddLinkman = m_mapPlayerLinkMan.find(ite->first);
				if (iteMeAddLinkman != m_mapPlayerLinkMan.end())
				{
					iteMeAddLinkman->second[i].LinkmanLogin(ite->first);
				}
			}
		}

		//! ��ȡ���͵��������GS�������ĳ�ʼ������
		vPlayerLinkMan[i].UpdateOnlineState();
		vPlayerLinkMan[i].Serialize_ForGs(&msgInit);
	}

	//! ���ͳ�ʼ������
	msgInit.SendToSocket(pPlayer->GetGsSocket());

	//! ��ȡ��½��Ϣ
	tagLinkman_data Linkman_data;
	Linkman_data.guid = pPlayer->GetExID();
	memmove(Linkman_data.szName, pPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	LinkmanGroup::UpdateLinkman_data(&Linkman_data);

	//! ������ҵ�½֪ͨ
	for (map<LONG, vector<CGUID>>::iterator ite = mapLoginInfo.begin(); ite != mapLoginInfo.end(); ++ite)
	{
		CMessage msgLogin(MSG_W2S_LINKMAN_PLAYER_LOGIN);
		msgLogin.AddEx(&Linkman_data, sizeof(tagLinkman_data));
		msgLogin.Add((LONG)ite->second.size());
		for(size_t i = 0; i < ite->second.size(); ++i)
			msgLogin.Add(ite->second[i]);

		msgLogin.SendToSocket(ite->first);
	}
}


//! ��Ӧ�������
void LinkmanSystem::OnPlayerOffline(CPlayer *pPlayer)
{
	map<CGUID, VecPlayerLinkMan>::iterator iteLinkman = m_mapPlayerLinkMan.find(pPlayer->GetExID());
	if (m_mapPlayerLinkMan.end() == iteLinkman) return;

	VecPlayerLinkMan &vPlayerLinkMan = iteLinkman->second;

	map<LONG, vector<CGUID>> mapLoginInfo;

	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{
		const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			//! ͳ����Ҫ��½��Ϣ����Һ�GS������
			const map<CGUID, tagLinkman_DB>& AddMeMap = vPlayerLinkMan[i].GetAddMeMap();
			for (map<CGUID, tagLinkman_DB>::const_iterator ite = AddMeMap.begin(); ite != AddMeMap.end(); ++ite)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(ite->first);
				if (NULL != pPlayer)
				{
					Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());
						
					if(!tLoginInfo)
					{
						Log4c::Trace(ROOT_MODULE,FormatText("WS_CONTACT_11", pPlayer->GetAccount()));
						return;
					}

					if(tLoginInfo && PLAYER_STATE_BACKTOLOGIN != tLoginInfo->GetPlayerState())
					{
						//! ͳ����Ϣ
						if (mapLoginInfo.find(pPlayer->GetGsSocket()) == mapLoginInfo.end())
						{
							mapLoginInfo.insert(pair<LONG, vector<CGUID>>(pPlayer->GetGsSocket(), vector<CGUID>()));
						}
						mapLoginInfo[pPlayer->GetGsSocket()].push_back(ite->first);	
					}
				}
			}

			//! ��������ҵ���ҵĳ�Ա״̬
			const map<CGUID, tagMeAddPlayer>& MeAddMap = vPlayerLinkMan[i].GetMeAddMap();
			for ( map<CGUID, tagMeAddPlayer>::const_iterator ite = MeAddMap.begin(); ite != MeAddMap.end(); ++ite)
			{
				map<CGUID, VecPlayerLinkMan>::iterator iteMeAddLinkman = m_mapPlayerLinkMan.find(ite->first);
				if (iteMeAddLinkman != m_mapPlayerLinkMan.end())
				{
					iteMeAddLinkman->second[i].LinkmanLogout(ite->first);
				}
			}
		}
	}

	//! ֪ͨGS����������
	/*CMessage msgRelease(MSG_W2S_LINKMAN_PLAYER_RELEASEDATA);
	msgRelease.Add(pPlayer->GetExID());
	msgRelease.SendToSocket(pPlayer->GetGsSocket());*/
	//! �����������֪ͨ
	for (map<LONG, vector<CGUID>>::iterator ite = mapLoginInfo.begin(); ite != mapLoginInfo.end(); ++ite)
	{
		CMessage msgLogout(MSG_W2S_LINKMAN_PLAYER_LOGOUT);
		msgLogout.Add(pPlayer->GetExID());
		msgLogout.Add((LONG)ite->second.size());
		for(size_t i = 0; i < ite->second.size(); ++i)
			msgLogout.Add(ite->second[i]);

		msgLogout.SendToSocket(ite->first);
	}
}

//! ��Ӧ���ɾ��״̬�ı�
void LinkmanSystem::OnPlayerDelStateChange(const CGUID &PlayerGuid, BOOL bDeleted)
{
	//CMessage msgDelState(MSG_W2S_LINKMAN_PLAYER_DELSTATE_CHANGE);
	//msgDelState.Add(PlayerGuid);
	//msgDelState.Add((LONG)bDeleted);
	//msgDelState.Add((LONG)ite->second.size());
	//msgDelState.AddEx(&(ite->second[0]), ite->second.size());

	//msgDelState.SendToSocket(ite->first);
}

//!								��Ӧ��ҵȼ��ı�
void LinkmanSystem::OnPlayerLevelChange(CPlayer *pPlayer)
{
	const CGUID &PlayerGuid = pPlayer->GetExID();
	LONG lLevel = pPlayer->GetLevel();

	map<CGUID, VecPlayerLinkMan>::iterator itePlayerLinkMan = m_mapPlayerLinkMan.find(PlayerGuid);
	if(itePlayerLinkMan == m_mapPlayerLinkMan.end()) return;

	VecPlayerLinkMan &vPlayerLinkMan = itePlayerLinkMan->second;

	//! �����б�
	map<LONG, vector<CGUID>> mapSendTable;

	//! ����WS���ϣ�ͳ�Ʒ����б�
	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{	
		const LinkmanGroupSetup *pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay && eTIT_Show_All == pLinkmanGroupSetup->m_uTimelyInfo)
		{
			//! ͳ����Ҫ��½��Ϣ����Һ�GS������
			const map<CGUID, tagLinkman_DB>& AddMeMap = vPlayerLinkMan[i].GetAddMeMap();
			for (map<CGUID, tagLinkman_DB>::const_iterator ite = AddMeMap.begin(); ite != AddMeMap.end(); ++ite)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(ite->first);
				if (NULL != pPlayer)
				{
					Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());

					if(!tLoginInfo)
					{
						Log4c::Trace(ROOT_MODULE,FormatText("WS_CONTACT_12", pPlayer->GetAccount()));
						return;
					}
					if(tLoginInfo && PLAYER_STATE_BACKTOLOGIN != tLoginInfo->GetPlayerState())
					{
						//! ͳ����Ϣ
						if (mapSendTable.find(pPlayer->GetGsSocket()) == mapSendTable.end())
						{
							mapSendTable.insert(pair<LONG, vector<CGUID>>(pPlayer->GetGsSocket(), vector<CGUID>()));
						}
						mapSendTable[pPlayer->GetGsSocket()].push_back(ite->first);
					}
				}
			}
		}
	}

	//! ����֪ͨ
	for (map<LONG, vector<CGUID>>::iterator ite = mapSendTable.begin(); ite != mapSendTable.end(); ++ite)
	{
		CMessage msgToGs(MSG_W2S_LINKMAN_PLAYER_INFO_CHANGE);
		msgToGs.Add(PlayerGuid);
		msgToGs.Add((LONG)eLIT_Level);
		msgToGs.Add(lLevel);
		msgToGs.Add((LONG)ite->second.size());
		for(size_t i = 0; i < ite->second.size(); ++i)
			msgToGs.Add(ite->second[i]);
		msgToGs.SendToSocket(ite->first);
	}
}

//!								��Ӧ���ְҵ�ı�
void LinkmanSystem::OnPlayerOccuChange(CPlayer *pPlayer)
{
	const CGUID &PlayerGuid = pPlayer->GetExID();
	LONG lOccupation = pPlayer->GetOccupation();

	map<CGUID, VecPlayerLinkMan>::iterator itePlayerLinkMan = m_mapPlayerLinkMan.find(PlayerGuid);
	if(itePlayerLinkMan == m_mapPlayerLinkMan.end()) return;

	VecPlayerLinkMan &vPlayerLinkMan = itePlayerLinkMan->second;

	//! �����б�
	map<LONG, vector<BYTE>> mapSendTable;

	//! ����WS���ϣ�ͳ�Ʒ����б�
	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{	
		const LinkmanGroupSetup *pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay && eTIT_Show_All == pLinkmanGroupSetup->m_uTimelyInfo)
		{
			//! ͳ����Ҫ��½��Ϣ����Һ�GS������
			const map<CGUID, tagLinkman_DB>& AddMeMap = vPlayerLinkMan[i].GetAddMeMap();
			for (map<CGUID, tagLinkman_DB>::const_iterator ite = AddMeMap.begin(); ite != AddMeMap.end(); ++ite)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(ite->first);
				if (NULL != pPlayer)
				{
					Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());

					if(!tLoginInfo)
					{
						Log4c::Trace(ROOT_MODULE,FormatText("WS_CONTACT_13", pPlayer->GetAccount()));
						return;
					}
					if(tLoginInfo && PLAYER_STATE_BACKTOLOGIN != tLoginInfo->GetPlayerState())
					{
						//! ͳ����Ϣ
						if (mapSendTable.find(pPlayer->GetGsSocket()) == mapSendTable.end())
						{
							mapSendTable.insert(pair<LONG, vector<BYTE>>(pPlayer->GetGsSocket(), vector<BYTE>()));
						}
						_AddToByteArray(&mapSendTable[pPlayer->GetGsSocket()], ite->first);	
					}
				}
			}
		}
	}

	//! ����֪ͨ
	for (map<LONG, vector<BYTE>>::iterator ite = mapSendTable.begin(); ite != mapSendTable.end(); ++ite)
	{
		CMessage msgToGs(MSG_W2S_LINKMAN_PLAYER_INFO_CHANGE);
		msgToGs.Add(PlayerGuid);
		msgToGs.Add((LONG)eLIT_Occupation);
		msgToGs.Add(lOccupation);
		msgToGs.Add((LONG)ite->second.size());
		for(size_t i = 0; i < ite->second.size(); ++i)
			msgToGs.Add(ite->second[i]);
		msgToGs.SendToSocket(ite->first);
	}
}

//! �ܷ�����
ULONG LinkmanSystem::CanLeaveWord(const CGUID &SenderGuid, const char *pReceiverName, CGUID &reReceiverGuid)
{
	assert(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(SenderGuid));
	VecPlayerLinkMan &vPlayerLinkManmap = m_mapPlayerLinkMan[SenderGuid];

	ULONG uRe = eLWPRI_Cant;
	for (size_t i = 0; i < vPlayerLinkManmap.size(); ++i)
	{
		const LinkmanGroupSetup *pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			//! �������������ι���
			if (eFFT_Filtrate_All == pLinkmanGroupSetup->m_uFunctionFiltrate)
			{
				if (NULL_GUID != vPlayerLinkManmap[i].FindMember(pReceiverName))
				{
					return eLWPRI_Cant;
				}
			}
			//! ����
			else 
			{
				const CGUID &FindGuid = vPlayerLinkManmap[i].FindMember(pReceiverName);
				if (NULL_GUID != FindGuid)
				{
					reReceiverGuid = FindGuid;
				}

				//! �ҳ���ߵ��������ȼ�
				if (eLWPRI_Cant != pLinkmanGroupSetup->m_uLeaveWordPRI && NULL_GUID != reReceiverGuid)
				{
					uRe = (pLinkmanGroupSetup->m_uLeaveWordPRI > uRe) ? pLinkmanGroupSetup->m_uLeaveWordPRI : uRe;
				}
			}
		}
	}
	return uRe;
}

//! �ܷ��ţ������߱������ߣ�
BOOL LinkmanSystem::CanMail(CPlayer *pReceiver, const char *pSenderName)
{
	if(NULL == pSenderName || NULL == pReceiver)
		return FALSE;

	const CGUID &ReceiverGuid = pReceiver->GetExID();

	assert(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(ReceiverGuid));
	VecPlayerLinkMan &vPlayerLinkManmap = m_mapPlayerLinkMan[ReceiverGuid];

	for (size_t i = 0; i < vPlayerLinkManmap.size(); ++i)
	{
		const LinkmanGroupSetup *pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			//! �������������ι���
			if (eFFT_Filtrate_All == pLinkmanGroupSetup->m_uFunctionFiltrate)
			{
				if (NULL_GUID != vPlayerLinkManmap[i].FindMember(pSenderName))
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

//!-----------------------------------------------------------------------------------------------
//! ��ӦDBS�������ϵ�����ݱ��������Ϣ
void LinkmanSystem::OnPlayerDataSave(const CGUID &PlayerGuid)
{
	m_LinkmanOptCache.ClearOnePlayerData(PlayerGuid);
}

//! ��Ӧϵͳ���������Ϣ
void LinkmanSystem::OnSystemRequestAddMsg(CMessage *pMsg)
{
	CGUID OwnerGuid;
	pMsg->GetGUID(OwnerGuid);
	ULONG lGroupID	= pMsg->GetLong();
	char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

	CMessage msgReAdd(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE);
	//! ȷ�ϲ���˫������
	CPlayer *pRequsetPlayer = GetGame().GetMapPlayer(OwnerGuid);
	if(NULL == pRequsetPlayer) return;
	//! �ж��Ƿ�������Ϊ�������ߡ�
	if(0 == strcmp(szAimName, GlobalSetup::GetSetup()->szHideName))
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimHideName);
		msgReAdd.Add(GlobalSetup::GetSetup()->szHideName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	CPlayer *pAimPlayer = GetGame().GetMapPlayer(szAimName);
	if(NULL == pAimPlayer)
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimNotBeing);
		msgReAdd.Add(szAimName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	if( pAimPlayer->GetHideFlag() == 1 && !GetGame().PlayerIsGM(pRequsetPlayer)
		&& (pAimPlayer->GetTeamID() != pRequsetPlayer->GetTeamID() || pAimPlayer->GetTeamID()==NULL_GUID || pRequsetPlayer->GetTeamID()==NULL_GUID) )
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimHideName);
		msgReAdd.Add(GlobalSetup::GetSetup()->szHideName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	//! ------------ȷ�ϲ����Ϸ���------------
	LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	if (-1 == lIdx) return;
	const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
	assert(pLinkmanGroupSetup);
	if(NULL == pLinkmanGroupSetup) return;
	if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
	const CGUID &AimGuid = pAimPlayer->GetExID();
	if (AimGuid == OwnerGuid)return;

	//! �ҵ������ߵ���ϵ�˶���
	map<CGUID, VecPlayerLinkMan>::iterator ite = m_mapPlayerLinkMan.find(OwnerGuid);
	if (m_mapPlayerLinkMan.end() == ite) return;
	VecPlayerLinkMan &PlayerLinkMan = ite->second;
	if( (long) ite->second.size() <= lIdx) return;
	//! ȷ�����û�б����������������
	if(pLinkmanGroupSetup->m_bMemberIsOnly)
	{
		for (size_t i = 0; i < PlayerLinkMan.size(); ++i)
		{
			const LinkmanGroupSetup *pTmpLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
			if(!pTmpLinkmanGroupSetup->m_bMemberIsOnly)
				continue;
			if(NULL_GUID != PlayerLinkMan[i].FindMember(szAimName)) return;
		}
	}
	
	//! �ж�����
	if (PlayerLinkMan[lIdx].GetMemberNum() >= (long)pLinkmanGroupSetup->m_uMaxMemberNum) return;

	if(pLinkmanGroupSetup->m_bAddAimAffirm)
	{
		assert(false);
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_CONTACT_42", lGroupID, lIdx));
		return;
	}

	//! ��ʼ���
	tagLinkman_DB Linkman_DB;
	Linkman_DB.AimGUID = AimGuid;
	Linkman_DB.OwnerGUID = OwnerGuid;
	Linkman_DB.uGroupID = lGroupID;
	memmove(Linkman_DB.szAimName, szAimName, MAX_PLAYER_NAME_SIZE);
	Linkman_DB.uDelState = eLDS_NotDel;

	//! ��������ߵ�[����ӵ���ϵ��]
	PlayerLinkMan[lIdx].AddMember(Linkman_DB);
	m_LinkmanOptCache.PushLinkmanAddOpt(Linkman_DB);

	//! ���±�����˵�[����ҵ���ϵ��]
	if(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(AimGuid))
		if((long)m_mapPlayerLinkMan[AimGuid].size() > lIdx)
			m_mapPlayerLinkMan[AimGuid][lIdx].AddCompriseMePlayer(Linkman_DB);

	tagLinkman_data Linkman_data;
	Linkman_data.guid = AimGuid;
	memmove(Linkman_data.szName, szAimName, MAX_PLAYER_NAME_SIZE);
	LinkmanGroup::UpdateLinkman_data(&Linkman_data);

	CMessage msgAddNotify(MSG_W2S_LINKMAN_ADD_NOTIFY);
	msgAddNotify.Add(OwnerGuid);
	msgAddNotify.Add(pRequsetPlayer->GetName());
	msgAddNotify.Add(lGroupID);
	msgAddNotify.AddEx(&Linkman_data, sizeof(tagLinkman_data));

	msgAddNotify.SendToSocket(pRequsetPlayer->GetGsSocket());
	if(pAimPlayer->GetGsSocket() != pRequsetPlayer->GetGsSocket())
		msgAddNotify.SendToSocket(pAimPlayer->GetGsSocket());

	if(eLGT_Spouse == pLinkmanGroupSetup->m_uGroupType)
		GetGame().SavePlayer(pRequsetPlayer);

}
//! ��Ӧ������������Ϣ
void LinkmanSystem::OnPlayerRequestAddMsg(CMessage *pMsg)
{
	CGUID OwnerGuid;
	pMsg->GetGUID(OwnerGuid);
	ULONG lGroupID	= pMsg->GetLong();
	char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

	CMessage msgReAdd(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE);
	//! ȷ�ϲ���˫������
	CPlayer *pRequsetPlayer = GetGame().GetMapPlayer(OwnerGuid);
	if(NULL == pRequsetPlayer) return;

	//! �ж��Ƿ�������Ϊ�������ߡ�
	if(0 == strcmp(szAimName, GlobalSetup::GetSetup()->szHideName))
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimHideName);
		msgReAdd.Add(GlobalSetup::GetSetup()->szHideName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	CPlayer *pAimPlayer = GetGame().GetMapPlayer(szAimName);
	if(NULL == pAimPlayer)
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimNotBeing);
		msgReAdd.Add(szAimName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	if( pAimPlayer->GetHideFlag() == 1 && !GetGame().PlayerIsGM(pRequsetPlayer)
		&& (pAimPlayer->GetTeamID() != pRequsetPlayer->GetTeamID() || pAimPlayer->GetTeamID()==NULL_GUID || pRequsetPlayer->GetTeamID()==NULL_GUID) ) // ������
	{
		msgReAdd.Add((LONG)FALSE);
		msgReAdd.Add((LONG)eOET_AimHideName);
		msgReAdd.Add(GlobalSetup::GetSetup()->szHideName);
		msgReAdd.Add(OwnerGuid);
		msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		return;
	}

	//! ------------ȷ�ϲ����Ϸ���------------
	LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	if (-1 == lIdx) return;
	const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
	assert(pLinkmanGroupSetup);
	if(NULL == pLinkmanGroupSetup) return;
	if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
	if (!pLinkmanGroupSetup->m_bUserAdd) return;
	const CGUID &AimGuid = pAimPlayer->GetExID();
	if (AimGuid == OwnerGuid)return;

	//! �ҵ������ߵ���ϵ�˶���
	map<CGUID, VecPlayerLinkMan>::iterator ite = m_mapPlayerLinkMan.find(OwnerGuid);
	if (m_mapPlayerLinkMan.end() == ite) return;
	VecPlayerLinkMan &PlayerLinkMan = ite->second;
	if( (long) PlayerLinkMan.size() <= lIdx) return;
	//! ȷ�����û�б����������������
	if(pLinkmanGroupSetup->m_bMemberIsOnly)
	{
		for (size_t i = 0; i < PlayerLinkMan.size(); ++i)
		{
			const LinkmanGroupSetup *pTmpLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
			if(!pTmpLinkmanGroupSetup->m_bMemberIsOnly)
				continue;
			if(NULL_GUID != PlayerLinkMan[i].FindMember(szAimName)) return;
		}
	}

	//! �ж�����
	if (PlayerLinkMan[lIdx].GetMemberNum() >= (long)pLinkmanGroupSetup->m_uMaxMemberNum) return;

	//! �Ƿ���ҪĿ��ȷ��
	if (pLinkmanGroupSetup->m_bAddAimAffirm)
	{
		//! �����Ự����
		tagOptSession OptSession;
		OptSession._uEndTime	= (ulong)time(NULL) + MAX_LINKMAN_OPT_TIME_SEC;
		OptSession._uOptType	= eLOT_Add;
		OptSession._uSrcGroupID	= INVALID_GROUP_ID;
		OptSession._uDesGroupID	= lGroupID;

		if(CreateAddSession(AimGuid, OwnerGuid, OptSession))
		{
			CMessage msgrEquestAdd(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_NOTIFY);
			msgrEquestAdd.Add(lGroupID);
			msgrEquestAdd.Add(pRequsetPlayer->GetName());
			msgrEquestAdd.Add(OwnerGuid);
			msgrEquestAdd.Add(AimGuid);
			msgrEquestAdd.SendToSocket(pAimPlayer->GetGsSocket());
		}
		else
		{
			msgReAdd.Add((LONG)FALSE);
			msgReAdd.Add((LONG)eOET_OverfullToOne);
			msgReAdd.Add(szAimName);
			msgReAdd.Add(OwnerGuid);
			msgReAdd.SendToSocket(pRequsetPlayer->GetGsSocket());
		}
	}
	else
	{
		tagLinkman_DB Linkman_DB;
		Linkman_DB.AimGUID = AimGuid;
		Linkman_DB.OwnerGUID = OwnerGuid;
		memmove(Linkman_DB.szAimName, szAimName, MAX_PLAYER_NAME_SIZE);
		Linkman_DB.uDelState = eLDS_NotDel;
		Linkman_DB.uGroupID = lGroupID;

		//! ��������ߵ�[����ӵ���ϵ��]
		PlayerLinkMan[lIdx].AddMember(Linkman_DB);
		m_LinkmanOptCache.PushLinkmanAddOpt(Linkman_DB);

		//! ���±�����˵�[����ҵ���ϵ��]
		if(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(AimGuid))
			if((long)m_mapPlayerLinkMan[AimGuid].size() > lIdx)
				m_mapPlayerLinkMan[AimGuid][lIdx].AddCompriseMePlayer(Linkman_DB);

		tagLinkman_data Linkman_data;
		Linkman_data.guid = AimGuid;
		memmove(Linkman_data.szName, szAimName, MAX_PLAYER_NAME_SIZE);
		LinkmanGroup::UpdateLinkman_data(&Linkman_data);

		CMessage msgAddNotify(MSG_W2S_LINKMAN_ADD_NOTIFY);
		msgAddNotify.Add(OwnerGuid);
		msgAddNotify.Add(pRequsetPlayer->GetName());
		msgAddNotify.Add(lGroupID);
		msgAddNotify.AddEx(&Linkman_data, sizeof(tagLinkman_data));
		
		msgAddNotify.SendToSocket(pRequsetPlayer->GetGsSocket());
		if(pAimPlayer->GetGsSocket() != pRequsetPlayer->GetGsSocket())
			msgAddNotify.SendToSocket(pAimPlayer->GetGsSocket());;
		
	}
}

//! ��Ӧ��һظ������Ϣ
void LinkmanSystem::OnPlayerReAddMsg(CMessage *pMsg)
{
	ULONG lGroupID	= pMsg->GetLong();
	CGUID RequestPlayerGuid;
	pMsg->GetGUID(RequestPlayerGuid);
	BOOL lAgree		= pMsg->GetLong();
	CGUID RePlayerGuid;
	pMsg->GetGUID(RePlayerGuid);

	//! ������֤�Ự�Ķ���
	tagOptSession OptSession;
	OptSession._uSrcGroupID = INVALID_GROUP_ID;
	OptSession._uOptType	= eLOT_Add;
	OptSession._uDesGroupID = lGroupID;

	BOOL bLegitimate = FALSE;
	if (EndAddSession(RePlayerGuid, RequestPlayerGuid, OptSession))
	{
		bLegitimate = TRUE;
	}
	else if (eLot_Move == OptSession._uOptType)
	{
		bLegitimate = TRUE;
	}

	if (bLegitimate)
	{
		CPlayer *pRePlayer = GetGame().GetMapPlayer(RePlayerGuid);
		CPlayer *pRequestPlayer = GetGame().GetMapPlayer(RequestPlayerGuid);
		if(NULL == pRePlayer || NULL == pRequestPlayer) return;

		LONG lSrcIdx = m_LGSManage.GetIndexByID(OptSession._uSrcGroupID);
		LONG lDesIdx = m_LGSManage.GetIndexByID(OptSession._uDesGroupID);
		if(-1 == lDesIdx) 
		{
			assert(false);
			return;
		}
		if (lAgree)
		{
			map<CGUID, VecPlayerLinkMan>::iterator iteRequest = m_mapPlayerLinkMan.find(RequestPlayerGuid);
			if(m_mapPlayerLinkMan.end() == iteRequest) return;
			if((long)iteRequest->second.size() <= lDesIdx && (long)iteRequest->second.size() <= lSrcIdx) return;
			LinkmanGroup &RequestLinkmanGroup = (iteRequest->second)[lDesIdx];

			map<CGUID, VecPlayerLinkMan>::iterator iteRe = m_mapPlayerLinkMan.find(RePlayerGuid);
			if (m_mapPlayerLinkMan.end() == iteRe) return;
			if( (long)iteRe->second.size() <= lDesIdx && (long)iteRe->second.size() <= lSrcIdx) return;
			LinkmanGroup &ReLinkmanGroup = (iteRe->second)[lDesIdx];

			//! ����������ˣ��򷵻ش�����ʾ
			const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
			assert(NULL != pLinkmanGroupSetup);
			if(NULL == pLinkmanGroupSetup) return;
			if(RequestLinkmanGroup.GetMemberNum() > (long)pLinkmanGroupSetup->m_uMaxMemberNum)
			{
				if(NULL != pRequestPlayer)
				{
					CMessage msg(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE);
					msg.Add((LONG)FALSE);
					msg.Add((LONG)eOET_ListFull);
					msg.Add(pRePlayer->GetName());
					msg.Add(RequestPlayerGuid);
					msg.SendToSocket(pRequestPlayer->GetGsSocket());
				}
				return;
			}

			//! ��������ƶ�������ϵ�����������򷵻�ʧ��
			if (eLot_Move != OptSession._uOptType)
			{
				pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
				if (NULL == pLinkmanGroupSetup)
					return;
				if (pLinkmanGroupSetup->m_bMemberIsOnly)
				{
					for (size_t i = 0; i < iteRequest->second.size(); ++i)
					{
						pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
						assert(NULL != pLinkmanGroupSetup);
						if(NULL == pLinkmanGroupSetup) continue;

						if(!pLinkmanGroupSetup->m_bMemberIsOnly) continue;
						if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
						{
							if (NULL_GUID != iteRequest->second[i].FindMember(pRePlayer->GetName()))
							{
								if(NULL != pRequestPlayer)
								{
									CMessage msg(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE);
									msg.Add((LONG)FALSE);
									msg.Add((LONG)eOET_AimInOther);
									msg.Add(pRePlayer->GetName());
									msg.Add(RequestPlayerGuid);
									msg.SendToSocket(pRequestPlayer->GetGsSocket());
								}
								return;
							}
						}
					}
				}	
			}

			//! ������ϵ�����ݶ���
			tagLinkman_DB Linkman_DB;
			Linkman_DB.AimGUID = RePlayerGuid;
			Linkman_DB.OwnerGUID = RequestPlayerGuid;
			memmove(Linkman_DB.szAimName, pRePlayer->GetName(), MAX_PLAYER_NAME_SIZE);
			Linkman_DB.uDelState = eLDS_NotDel;
			Linkman_DB.uGroupID = OptSession._uDesGroupID;

			if (eLot_Move == OptSession._uOptType)
			{
				if(-1 == lSrcIdx) 
				{
					assert(false);
					return;
				}
				pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx);
				assert(NULL != pLinkmanGroupSetup);
				if(NULL == pLinkmanGroupSetup) return;
				if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
				{
					(iteRequest->second)[lSrcIdx].DelMember(RePlayerGuid);
					m_LinkmanOptCache.PushLinkmanDelOpt(Linkman_DB);
					GetLogicLogInterface()->logT450_linkman_opt_log(pRequestPlayer, RePlayerGuid, pRePlayer->GetName(), 2, OptSession._uSrcGroupID);

					(iteRe->second)[lSrcIdx].DelCompriseMePlayer(RequestPlayerGuid);
				}

				//! �����ϵ��
				RequestLinkmanGroup.AddMember(Linkman_DB);
				m_LinkmanOptCache.PushLinkmanAddOpt(Linkman_DB);
				GetLogicLogInterface()->logT450_linkman_opt_log(pRequestPlayer, RePlayerGuid, pRePlayer->GetName(), 1, OptSession._uDesGroupID);

				if(NULL != pRequestPlayer)
				{
					tagLinkman_data Linkman_data;
					Linkman_data.guid = RePlayerGuid;
					memmove(Linkman_data.szName, pRePlayer->GetName(), MAX_PLAYER_NAME_SIZE);
					LinkmanGroup::UpdateLinkman_data(&Linkman_data);

					CMessage msgMoveNotify(MSG_W2S_LINKMAN_MOVE_NOTIFY);
					msgMoveNotify.Add((LONG)TRUE);
					msgMoveNotify.Add(RequestPlayerGuid);
					msgMoveNotify.Add(pRequestPlayer->GetName());
					msgMoveNotify.Add(OptSession._uSrcGroupID);
					msgMoveNotify.Add(OptSession._uDesGroupID);
					msgMoveNotify.AddEx(&Linkman_data, sizeof(tagLinkman_data));

					msgMoveNotify.SendToSocket(pRequestPlayer->GetGsSocket());
					if(pRePlayer->GetGsSocket() != pRequestPlayer->GetGsSocket())
						msgMoveNotify.SendToSocket(pRePlayer->GetGsSocket());;
				}
			}
			else
			{	
				//! �����ϵ��
				RequestLinkmanGroup.AddMember(Linkman_DB);
				m_LinkmanOptCache.PushLinkmanAddOpt(Linkman_DB);
		
				GetLogicLogInterface()->logT450_linkman_opt_log(pRequestPlayer, RePlayerGuid, pRePlayer->GetName(), 1, OptSession._uDesGroupID);
				
				tagLinkman_data Linkman_data;
				Linkman_data.guid = RePlayerGuid;
				memmove(Linkman_data.szName, pRePlayer->GetName(), MAX_PLAYER_NAME_SIZE);
				LinkmanGroup::UpdateLinkman_data(&Linkman_data);

				CMessage msgAddNotify(MSG_W2S_LINKMAN_ADD_NOTIFY);
				msgAddNotify.Add(RequestPlayerGuid);
				msgAddNotify.Add(pRequestPlayer->GetName());
				msgAddNotify.Add(OptSession._uDesGroupID);
				msgAddNotify.AddEx(&Linkman_data, sizeof(tagLinkman_data));

				msgAddNotify.SendToSocket(pRequestPlayer->GetGsSocket());
				if(pRePlayer->GetGsSocket() != pRequestPlayer->GetGsSocket())
					msgAddNotify.SendToSocket(pRePlayer->GetGsSocket());;
			}

			//! ���[����ҵ���]
			ReLinkmanGroup.AddCompriseMePlayer(Linkman_DB);
		}
		else
		{
			if(NULL != pRequestPlayer)
			{
				CMessage msg(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE);
				msg.Add((LONG)FALSE);
				msg.Add((LONG)eOET_AimReject);
				msg.Add(pRePlayer->GetName());
				msg.Add(RequestPlayerGuid);
				msg.SendToSocket(pRequestPlayer->GetGsSocket());
			}
		}
	}
}

//! ��Ӧ�������ɾ����Ϣ
void LinkmanSystem::OnPlayerRequestDelMsg(CMessage *pMsg)
{
	CGUID OwnerGuid;
	pMsg->GetGUID(OwnerGuid);
	ULONG lGroupID	= pMsg->GetLong();
	char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);
    bool bIsSysRequest = pMsg->GetByte() ? true : false ;

	LONG lIdx = m_LGSManage.GetIndexByID(lGroupID);
	const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lIdx);
	assert(NULL != pLinkmanGroupSetup);
	if(NULL == pLinkmanGroupSetup) return;

	//! ȷ�ϺϷ���
	if (eKW_ServerKeep != pLinkmanGroupSetup->m_uKeepWay) return;
	if (!bIsSysRequest && !pLinkmanGroupSetup->m_bUserDel) return;

	//! �ҵ���ϵ����
	map<CGUID, VecPlayerLinkMan>::iterator iteOpter = m_mapPlayerLinkMan.find(OwnerGuid);
	if (m_mapPlayerLinkMan.end() == iteOpter) return;
	LinkmanGroup &OpterLinkmanGroup = (iteOpter->second)[lIdx];

	//! �õ���ɾ���ߵ�ID
	CGUID AimGuid = OpterLinkmanGroup.FindMember(szAimName);
	if (NULL_GUID == AimGuid) return;

	//! ɾ����ϵ��
	OpterLinkmanGroup.DelMember(AimGuid);
	tagLinkman_DB Linkman_DB;
	Linkman_DB.AimGUID = AimGuid;
	Linkman_DB.OwnerGUID = OwnerGuid;
	Linkman_DB.uGroupID = lGroupID;
	Linkman_DB.uDelState = eLDS_NotDel;
	strcpy(Linkman_DB.szAimName, szAimName);
	
	m_LinkmanOptCache.PushLinkmanDelOpt(Linkman_DB);
	GetLogicLogInterface()->logT450_linkman_opt_log(GetGame().GetMapPlayer(OwnerGuid), AimGuid, szAimName, 2, lGroupID);

	//! ���±�ɾ���ߵ�[����ҵ���ϵ��]�б�
	map<CGUID, VecPlayerLinkMan>::iterator iteAim = m_mapPlayerLinkMan.find(AimGuid);
	if (m_mapPlayerLinkMan.end() != iteAim) 
	{
		if( (long)iteAim->second.size() <= lIdx)
			(iteAim->second)[lIdx].DelCompriseMePlayer(OwnerGuid);
	
	}

	CPlayer *pOptPlayer = GetGame().GetMapPlayer(OwnerGuid);
	if(NULL == pOptPlayer){assert(false); return;}
	CMessage msgDelNotify(MSG_W2S_LINKMAN_DEL_NOTIFY);
	msgDelNotify.Add(OwnerGuid);
	msgDelNotify.Add(pOptPlayer->GetName());
	msgDelNotify.Add(lGroupID);
	msgDelNotify.Add(AimGuid);
	msgDelNotify.Add(szAimName);

	msgDelNotify.SendToSocket(pOptPlayer->GetGsSocket());

	CPlayer *pAimPlayer = GetGame().GetMapPlayer(AimGuid);
	if(NULL != pAimPlayer)
	{
		if(pAimPlayer->GetGsSocket() != pOptPlayer->GetGsSocket())
			msgDelNotify.SendToSocket(pAimPlayer->GetGsSocket());
	}

	if(eLGT_Spouse == pLinkmanGroupSetup->m_uGroupType)
		GetGame().SavePlayer(pOptPlayer);
}

//! ��Ӧ��������ƶ���Ϣ
void LinkmanSystem::OnPlayerRequestMoveMsg(CMessage *pMsg)
{
	CGUID OwnerGuid;
	pMsg->GetGUID(OwnerGuid);
	ULONG lSrcGroupID	= pMsg->GetLong();
	ULONG lDesGroupID	= pMsg->GetLong();
	char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
	pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

	CMessage msgMoveNotify(MSG_W2S_LINKMAN_MOVE_NOTIFY);

	CPlayer *pOptPlayer = GetGame().GetMapPlayer(OwnerGuid);
	if(NULL == pOptPlayer) return;

	//! �ж��Ƿ�������Ϊ�������ߡ�
	if(0 == strcmp(szAimName, GlobalSetup::GetSetup()->szHideName))
	{
		msgMoveNotify.Add((LONG)FALSE);
		msgMoveNotify.Add((LONG)eOET_AimHideName);
		msgMoveNotify.Add(GlobalSetup::GetSetup()->szHideName);
		msgMoveNotify.Add(OwnerGuid);
		msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
		return;
	}

	CPlayer *pAimPlayer = GetGame().GetMapPlayer(szAimName);
	if(NULL == pAimPlayer)
	{
		msgMoveNotify.Add((LONG)FALSE);
		msgMoveNotify.Add((LONG)eOET_AimNotBeing);
		msgMoveNotify.Add(szAimName);
		msgMoveNotify.Add(OwnerGuid);
		msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
		return;
	}

	if( pAimPlayer->GetHideFlag() == 1 && !GetGame().PlayerIsGM(pOptPlayer) 
		&& (pAimPlayer->GetTeamID() != pOptPlayer->GetTeamID() || pAimPlayer->GetTeamID()==NULL_GUID || pOptPlayer->GetTeamID()==NULL_GUID) ) // ������
	{
		msgMoveNotify.Add((LONG)FALSE);
		msgMoveNotify.Add((LONG)eOET_AimHideName);
		msgMoveNotify.Add(GlobalSetup::GetSetup()->szHideName);
		msgMoveNotify.Add(OwnerGuid);
		msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
		return;
	}

	//! ��������
	LONG lSrcIdx = m_LGSManage.GetIndexByID(lSrcGroupID);
	LONG lDesIdx = m_LGSManage.GetIndexByID(lDesGroupID);
	const LinkmanGroupSetup* pSrcGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lSrcIdx);
	const LinkmanGroupSetup* pDesGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx);
	if(NULL == pSrcGroupSetup || NULL == pDesGroupSetup) {assert(false); return;}

	//! �����ߵ���ϵ�˶���
	map<CGUID, VecPlayerLinkMan>::iterator iteOpter = m_mapPlayerLinkMan.find(OwnerGuid);
	if (m_mapPlayerLinkMan.end() == iteOpter) return;
	if((long)iteOpter->second.size() <= max(lSrcIdx, lDesIdx)) return;
	LinkmanGroup &OptSrcGroup = (iteOpter->second)[lSrcIdx];
	LinkmanGroup &OptDesGroup = (iteOpter->second)[lDesIdx];


	//! ����֤ɾ��
	CGUID AimGuid;
	BOOL bRunDel = TRUE;
	if (eKW_ServerKeep != pSrcGroupSetup->m_uKeepWay)
	{
		bRunDel = FALSE;
	}
	else
	{
		if(!pSrcGroupSetup->m_bUserDel) return;//! ������ͻ���ɾ������
		AimGuid = OptSrcGroup.FindMember(szAimName);
		if (NULL_GUID == AimGuid) return;//! ɾ�������ڷ�����
	}

	//! ����֤���
	BOOL bRunAdd = TRUE;
	if (eKW_ServerKeep != pDesGroupSetup->m_uKeepWay)
	{
		bRunAdd = FALSE;
	}
	else
	{
		if(!pDesGroupSetup->m_bUserAdd) return; //! ������ͻ�����Ӳ���
		for (size_t i = 0; i < (iteOpter->second).size(); ++i)
		{
			if (i != lSrcIdx)
				if (NULL_GUID != (iteOpter->second)[i].FindMember(szAimName)) return;
		}
		if (NULL_GUID != (iteOpter->second)[lDesIdx].FindMember(szAimName)) return;//! ��Ӷ����Ѵ���
		if ( NULL == pAimPlayer) //! ��Ҳ����ߣ��Ҳ�����ҵ�ID���޷�����
		{
			msgMoveNotify.Add((LONG)FALSE);
			msgMoveNotify.Add((LONG)eOET_AimNotBeing);
			msgMoveNotify.Add(szAimName);
			msgMoveNotify.Add(OwnerGuid);
			msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
			return;
		}
		if (pAimPlayer->GetExID() == OwnerGuid)//! ��������ǲ������Լ�
		{
			return;
		}
		//! ��������
		if((iteOpter->second)[lDesIdx].GetMemberNum() >= (long)m_LGSManage.GetLinkmanGroupSetup_ByOrder(lDesIdx)->m_uMaxMemberNum)
		{
			msgMoveNotify.Add((LONG)FALSE);
			msgMoveNotify.Add((LONG)eOET_ListFull);
			msgMoveNotify.Add(szAimName);
			msgMoveNotify.Add(OwnerGuid);
			msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
			return;
		}
		AimGuid = pAimPlayer->GetExID();
	}

	if ((!bRunDel) && (!bRunAdd)) return;//! �����鶼�����ڿͻ��ˣ�����Ҫ������ִ��


	//! ��ʼִ��
	if (bRunAdd)
	{
		if (pDesGroupSetup->m_bAddAimAffirm) //! ���Ҫ��Ŀ��ȷ��
		{
			//! �����Ự����
			tagOptSession OptSession;
			OptSession._uEndTime	= (ulong)time(NULL) + MAX_LINKMAN_OPT_TIME_SEC;
			OptSession._uOptType	= eLot_Move;
			OptSession._uSrcGroupID	= lSrcGroupID;
			OptSession._uDesGroupID	= lDesGroupID;

			if(CreateAddSession(AimGuid, OwnerGuid, OptSession))
			{
				CMessage msgrEquestAdd(MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_NOTIFY);
				msgrEquestAdd.Add(lDesGroupID);
				msgrEquestAdd.Add(pOptPlayer->GetName());
				msgrEquestAdd.Add(OwnerGuid);
				msgrEquestAdd.Add(AimGuid);
				msgrEquestAdd.SendToSocket(pAimPlayer->GetGsSocket());
			}
			else
			{
				msgMoveNotify.Add((LONG)FALSE);
				msgMoveNotify.Add((LONG)eOET_OverfullToOne);
				msgMoveNotify.Add(szAimName);
				msgMoveNotify.Add(OwnerGuid);
				msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
			}

			return;
		}
	}

	map<CGUID, VecPlayerLinkMan>::iterator iteAim = m_mapPlayerLinkMan.find(AimGuid);
	if (m_mapPlayerLinkMan.end() == iteAim) return;
	if((long)iteAim->second.size() <= max(lSrcIdx, lDesIdx)) return;
	LinkmanGroup &AimSrcGroup = (iteAim->second)[lSrcIdx];
	LinkmanGroup &AimDesGroup = (iteAim->second)[lDesIdx];

	if (bRunDel)
	{
		tagLinkman_DB Linkman_DB;
		Linkman_DB.AimGUID = AimGuid;
		Linkman_DB.OwnerGUID = OwnerGuid;
		memmove(Linkman_DB.szAimName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
		Linkman_DB.uDelState = eLDS_NotDel;

		OptSrcGroup.DelMember(AimGuid);
		m_LinkmanOptCache.PushLinkmanDelOpt(Linkman_DB);
		GetLogicLogInterface()->logT450_linkman_opt_log(pOptPlayer, AimGuid, szAimName, 2, lSrcGroupID);
		AimSrcGroup.DelCompriseMePlayer(OwnerGuid);
	}

	//! ������ϵ�����ݶ���
	tagLinkman_DB Linkman_DB;
	Linkman_DB.AimGUID = AimGuid;
	Linkman_DB.OwnerGUID = OwnerGuid;
	memmove(Linkman_DB.szAimName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	Linkman_DB.uDelState = eLDS_NotDel;
	Linkman_DB.uGroupID = lDesGroupID;

	if (bRunAdd)
	{
		OptDesGroup.AddMember(Linkman_DB);
		m_LinkmanOptCache.PushLinkmanAddOpt(Linkman_DB);
		GetLogicLogInterface()->logT450_linkman_opt_log(pOptPlayer, AimGuid, szAimName, 1, lDesGroupID);
		AimDesGroup.AddCompriseMePlayer(Linkman_DB);
	}

	//! ������Ϣ
	tagLinkman_data Linkman_data;
	Linkman_data.guid = AimGuid;
	memmove(Linkman_data.szName, pAimPlayer->GetName(), MAX_PLAYER_NAME_SIZE);
	LinkmanGroup::UpdateLinkman_data(&Linkman_data);

	msgMoveNotify.Add((LONG)TRUE);
	msgMoveNotify.Add(OwnerGuid);
	msgMoveNotify.Add(pOptPlayer->GetName());
	msgMoveNotify.Add(lSrcGroupID);
	msgMoveNotify.Add(lDesGroupID);
	msgMoveNotify.AddEx(&Linkman_data, sizeof(tagLinkman_data));

	msgMoveNotify.SendToSocket(pOptPlayer->GetGsSocket());
	if(pAimPlayer->GetGsSocket() != pOptPlayer->GetGsSocket())
		msgMoveNotify.SendToSocket(pAimPlayer->GetGsSocket());
}

/*
//!-----------------------------------------------------------------------------------------------
//! ��ӦGS�����Ϣ
void OnGsAdd(CMessage *pMsg)
{
	ULONG uGroupID = pMsg->GetLong();
	CGUID PlayerGuid;
	pMsg->GetGUID(PlayerGuid);
	tagLinkman Linkman;
	pMsg->GetEx(&Linkman, sizeof(tagLinkman));

	LONG lIdx = m_LGSManage.GetIndexByID(uGroupID);
	assert(-1 != lIdx);
	if (-1 == lIdx) return;
	
	tagLinkman_DB Linkman_DB;
	Linkman_DB.AimGUID = Linkman.guid;
	Linkman_DB.OwnerGUID = PlayerGuid;
	memmove(Linkman_DB.szAimName, Linkman.szName, MAX_PLAYER_NAME_SIZE);
	Linkman_DB.uDelState = eLDS_NotDel;
	
	//! ��������ߵ�[����ӵ���ϵ��]
	assert(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(PlayerGuid));
	m_mapPlayerLinkMan[PlayerGuid][lIdx].AddMember(Linkman_DB);

	//! ���±�����˵�[����ҵ���ϵ��]
	assert(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(Linkman_DB.AimGUID));
	m_mapPlayerLinkMan[Linkman_DB.AimGUID][lIdx].AddCompriseMePlayer(PlayerGuid);
	
}

//! ��ӦGSɾ����Ϣ
void OnGsDel(CMessage *pMsg)
{
	ULONG uGroupID = pMsg->GetLong();
	CGUID OwnerGuid, AimGuid;
	pMsg->GetGUID(OwnerGuid);
	pMsg->GetGUID(AimGuid);

	LONG lIdx = m_LGSManage.GetIndexByID(uGroupID);
	assert(-1 != lIdx);

	//! ��������ߵ�[����ӵ���ϵ��]
	assert(m_mapPlayerLinkMan.end() != m_mapPlayerLinkMan.find(OwnerGuid));
	m_mapPlayerLinkMan[OwnerGuid][lIdx].DelMember(AimGuid);

	//! ���±�����˵�[����ҵ���ϵ��]
	map<CGUID, VecPlayerLinkMan>::iterator ite = m_mapPlayerLinkMan.find(AimGuid);
	if(m_mapPlayerLinkMan.end() != ite)
		ite->second[lIdx].DelCompriseMePlayer(OwnerGuid);
}
*/

//! ��ӦGS��ͼ�ı���Ϣ
void LinkmanSystem::OnGsMapChange(const CGUID &PlayerGuid, const char* pMapName)
{
	map<CGUID, VecPlayerLinkMan>::iterator itePlayerLinkMan = m_mapPlayerLinkMan.find(PlayerGuid);
	if(itePlayerLinkMan == m_mapPlayerLinkMan.end()) return;

	VecPlayerLinkMan &vPlayerLinkMan = itePlayerLinkMan->second;

	//! �����б�
	map<LONG, vector<BYTE>> mapSendTable;

	//! ����WS���ϣ�ͳ�Ʒ����б�
	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{	
		const LinkmanGroupSetup *pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay && eTIT_Show_All == pLinkmanGroupSetup->m_uTimelyInfo)
		{
			//! ͳ����Ҫ��½��Ϣ����Һ�GS������
			const map<CGUID, tagLinkman_DB>& AddMeMap = vPlayerLinkMan[i].GetAddMeMap();
			for (map<CGUID, tagLinkman_DB>::const_iterator ite = AddMeMap.begin(); ite != AddMeMap.end(); ++ite)
			{
				CPlayer *pPlayer = GetGame().GetMapPlayer(ite->first);
				if (NULL != pPlayer)
				{
					Account* tLoginInfo = GetGame().GetEntityManager()->FindAccount(pPlayer->GetAccount());

					if(!tLoginInfo)
					{
						Log4c::Trace(ROOT_MODULE,FormatText("WS_CONTACT_14", pPlayer->GetAccount()));
						return;
					}
					if(tLoginInfo && PLAYER_STATE_BACKTOLOGIN != tLoginInfo->GetPlayerState())
					{
						//! ͳ����Ϣ
						if (mapSendTable.find(pPlayer->GetGsSocket()) == mapSendTable.end())
						{
							mapSendTable.insert(pair<LONG, vector<BYTE>>(pPlayer->GetGsSocket(), vector<BYTE>()));
						}
						_AddToByteArray(&mapSendTable[pPlayer->GetGsSocket()], ite->first);
					}
				}
			}
		}
	}

	//! ����֪ͨ
	for (map<LONG, vector<BYTE>>::iterator ite = mapSendTable.begin(); ite != mapSendTable.end(); ++ite)
	{
		CMessage msgToGs(MSG_W2S_LINKMAN_PLAYER_INFO_CHANGE);
		msgToGs.Add(PlayerGuid);
		msgToGs.Add((LONG)eLIT_Map);
		msgToGs.Add(pMapName);
		msgToGs.Add((LONG)ite->second.size());
		for(size_t i = 0; i < ite->second.size(); ++i)
			msgToGs.Add(ite->second[i]);
		msgToGs.SendToSocket(ite->first);
	}
}


//! ��ӦGS���ݷ�����Ϣ
void LinkmanSystem::OnGsDataSend(BYTE *pData, LONG &lPos)
{
	//! �õ��������
	LONG lPlayerNum = _GetLongFromByteArray(pData, lPos);

	for (LONG lPlayerIdx = 0; lPlayerIdx < lPlayerNum; ++lPlayerIdx)
	{
		//! �õ�һ�����ID
		CGUID PlayerGuid;
		_GetBufferFromByteArray(pData, lPos, PlayerGuid);
		//! �õ���ϵ������
		LONG lGroupNum = _GetLongFromByteArray(pData, lPos);
		
		//! ������ò�һ��
		if(lGroupNum != m_LGSManage.GetGroupNum())
		{
			//! ���GS����
			return;
		}

		//! ����Ƿ����и���ҵ���ϵ������
		if(m_mapPlayerLinkMan.end() == m_mapPlayerLinkMan.find(PlayerGuid))
		{
			VecPlayerLinkMan tmpPlayerLinkMan;
			LONG lGroupNum = m_LGSManage.GetGroupNum();
			for (LONG i = 0; i < lGroupNum; ++i)
			{
				LinkmanGroup tmpLinkmanGroup(m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i));
				tmpPlayerLinkMan.push_back(tmpLinkmanGroup);
			}
			m_mapPlayerLinkMan[PlayerGuid] = tmpPlayerLinkMan;
		}

		VecPlayerLinkMan &vPlayerLinkMan = m_mapPlayerLinkMan[PlayerGuid];

		for (LONG lGorupIdx = 0; lGorupIdx < lGroupNum; ++lGorupIdx)
		{
			vPlayerLinkMan[lGorupIdx].Decode_FromGs(pData, lPos);
		}
	}
}


void LinkmanSystem::GetPlayerLinkmanData_FromDBS(const CGUID &PlayerGuid, CEntityGroup* linkmanGroupOwner, CEntityGroup* linkmanGroupAim)
{
	//assert(m_mapPlayerLinkMan.end() == m_mapPlayerLinkMan.find(PlayerGuid));
	DelPlayerLinkman(PlayerGuid);

	//! �������ݽṹ
	VecPlayerLinkMan tmpPlayerLinkMan;

	LONG lGroupNum = m_LGSManage.GetGroupNum();
	for (LONG i = 0; i < lGroupNum; ++i)
	{
		LinkmanGroup tmpLinkmanGroup(m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i));

		tmpPlayerLinkMan.push_back(tmpLinkmanGroup);
	}

	map<CGUID, CBaseEntity*>::iterator itr = linkmanGroupOwner->GetEntityGroupMap().begin();
	for(; itr != linkmanGroupOwner->GetEntityGroupMap().end(); itr++)
	{
		tagLinkman_DB Linkman_DB;
		itr->second->GetGuidAttr(string("OwnerGUID"), Linkman_DB.OwnerGUID);
		Linkman_DB.uGroupID = itr->second->GetLongAttr(string("GroupID"));
		itr->second->GetGuidAttr(string("AimGUID"), Linkman_DB.AimGUID);
		strcpy(Linkman_DB.szAimName, itr->second->GetStringAttr(string("AimName")));
		Linkman_DB.uDelState = itr->second->GetLongAttr(string("DelState"));

		if(PlayerGuid == Linkman_DB.OwnerGUID)
		{
			LONG lIdx = m_LGSManage.GetIndexByID(Linkman_DB.uGroupID);
			if(-1 == lIdx)
			{
				assert(false);
				continue;
			}
			
			assert(Linkman_DB.AimGUID != PlayerGuid);
			if(!tmpPlayerLinkMan[lIdx].AddMember(Linkman_DB))
			{
				assert(false);
				continue;
			}
		}
		else
		{
			assert(false);
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_CONTACT_43"));
			//assert(Linkman_DB.AimGUID == PlayerGuid);
			//long groupID = itr->second->GetLongAttr(string("GroupID"));
			//LONG lIdx		= m_LGSManage.GetIndexByID(groupID);
			//if (-1 != lIdx)
			//{
			//	if(m_LinkmanOptCache.AffirmLinkmanDataBeing(Linkman_DB))
			//	{
			//		if(!tmpPlayerLinkMan[lIdx].AddCompriseMePlayer(Linkman_DB))
			//		{
			//			assert(false);
			//			continue;
			//		}
			//	}
			//}
		}
	}

	itr = linkmanGroupAim->GetEntityGroupMap().begin();
	for(; itr != linkmanGroupAim->GetEntityGroupMap().end(); itr++)
	{
		tagLinkman_DB Linkman_DB;
		itr->second->GetGuidAttr(string("OwnerGUID"), Linkman_DB.OwnerGUID);
		Linkman_DB.uGroupID = itr->second->GetLongAttr(string("GroupID"));
		itr->second->GetGuidAttr(string("AimGUID"), Linkman_DB.AimGUID);
		strcpy(Linkman_DB.szAimName, itr->second->GetStringAttr(string("AimName")));
		Linkman_DB.uDelState = itr->second->GetLongAttr(string("DelState"));

		if(PlayerGuid == Linkman_DB.OwnerGUID)
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,GetText("WS_CONTACT_44"));
			//LONG lIdx = m_LGSManage.GetIndexByID(Linkman_DB.uGroupID);
			//if(-1 == lIdx)
			//{
			//	assert(false);
			//	continue;
			//}

			//assert(Linkman_DB.AimGUID != PlayerGuid);
			//if(!tmpPlayerLinkMan[lIdx].AddMember(Linkman_DB))
			//{
			//	assert(false);
			//	continue;
			//}
		}
		else
		{
			assert(Linkman_DB.AimGUID == PlayerGuid);
			long groupID = itr->second->GetLongAttr(string("GroupID"));
			LONG lIdx		= m_LGSManage.GetIndexByID(groupID);
			if (-1 != lIdx)
			{
				if(m_LinkmanOptCache.AffirmLinkmanDataBeing(Linkman_DB))
				{
					if(!tmpPlayerLinkMan[lIdx].AddCompriseMePlayer(Linkman_DB))
					{
						assert(false);
						continue;
					}
				}
			}
		}
	}

	list<tagLinkman_DB> listLinkman_DB;
	m_LinkmanOptCache.FindNewAddPlayerData(listLinkman_DB, PlayerGuid);
	for (list<tagLinkman_DB>::iterator ite = listLinkman_DB.begin(); ite != listLinkman_DB.end(); ++ite)
	{
		LONG lIdx = m_LGSManage.GetIndexByID(ite->uGroupID);
		assert(-1 != lIdx);
		if (-1 != lIdx)
		{
			tmpPlayerLinkMan[lIdx].AddCompriseMePlayer(*ite);
		}	
	}

	m_mapPlayerLinkMan.insert(pair<CGUID, VecPlayerLinkMan>(PlayerGuid, tmpPlayerLinkMan));
}


//! ɾ��һ����ҵ���Ϣ
void LinkmanSystem::DelPlayerLinkman(const CGUID &PlayerGuid)
{
	map<CGUID, VecPlayerLinkMan>::iterator ite = m_mapPlayerLinkMan.find(PlayerGuid);
	if(m_mapPlayerLinkMan.end() != ite)
	{
		for (size_t i = 0; i < ite->second.size(); ++i)
		{
			ite->second[i].Release();
		}
		m_mapPlayerLinkMan.erase(PlayerGuid);
	}
}

//! �������תGS֪ͨ
void LinkmanSystem::OnPlayerChangeGs(CPlayer *pPlayer)
{
	map<CGUID, VecPlayerLinkMan>::iterator iteLinkman = m_mapPlayerLinkMan.find(pPlayer->GetExID());
	if (m_mapPlayerLinkMan.end() == iteLinkman) return;

	CMessage msgInit(MSG_W2S_LINKMAN_PLAYER_INITDATA);
	msgInit.Add((BYTE)FALSE);
	msgInit.Add(pPlayer->GetExID());
	VecPlayerLinkMan &vPlayerLinkMan = iteLinkman->second;

	for (size_t i = 0; i < vPlayerLinkMan.size(); ++i)
	{
		const LinkmanGroupSetup* pLinkmanGroupSetup = m_LGSManage.GetLinkmanGroupSetup_ByOrder((ulong)i);
		if (eKW_ServerKeep == pLinkmanGroupSetup->m_uKeepWay)
		{
			//! ��ȡ���͵��������GS�������ĳ�ʼ������
			vPlayerLinkMan[i].Serialize_ForGs(&msgInit);
		}
	}
	//! ���ͳ�ʼ������
	msgInit.SendToSocket(pPlayer->GetGsSocket());
}

//!-----------------------------------------------------------------------------------------------

//! ����
BOOL LinkmanSystem::CreateAddSession(const CGUID& OwnerGuid, const CGUID& FlagGuid, tagOptSession &OptSession)
{
	if (m_Session.find(OwnerGuid) == m_Session.end())
	{
		map<CGUID, tagOptSession> FlagMap;
		m_Session.insert(pair<CGUID, map<CGUID, tagOptSession>>(OwnerGuid, FlagMap));
	}

	BOOL bRe = FALSE;
	BOOL bRunClear = FALSE;

	map<CGUID, tagOptSession> &FlagMap = m_Session[OwnerGuid];
	LONG lNow = (long)time(NULL);

	map<CGUID, tagOptSession>::iterator iteOptSession = FlagMap.find(FlagGuid);
	if (iteOptSession != FlagMap.end())
	{
		if((long)iteOptSession->second._uEndTime < lNow )
		{
			bRunClear = TRUE;
			bRe = TRUE;
		}
	}
	else
	{
		bRe = TRUE;
	}

	if (bRunClear || FlagMap.size() > 20)
	{
		ClearOutTimeSession(FlagMap);
	}

	if (bRe)
	{
		FlagMap[FlagGuid] = OptSession;
	}


	return bRe;
}

//! �Ƴ�һ����ҵ����лỰ
void LinkmanSystem::EndAddSession(const CGUID& OwnerGuid)
{
	m_Session.erase(OwnerGuid);
}

//! �Ƴ��������֮��ĻỰ
BOOL LinkmanSystem::EndAddSession(const CGUID& OwnerGuid, const CGUID& FlagGuid, tagOptSession &OptSession)
{
	map<CGUID, map<CGUID, tagOptSession>>::iterator ite = m_Session.find(OwnerGuid);
	if (ite != m_Session.end())
	{
		map<CGUID, tagOptSession>::iterator iteChild = (*ite).second.find(FlagGuid);
		if (iteChild != (*ite).second.end())
		{
			BOOL bRe = FALSE;
			if(iteChild->second._uEndTime > (ulong)time(NULL))
			{
				if (iteChild->second._uSrcGroupID == OptSession._uSrcGroupID &&
					iteChild->second._uDesGroupID == OptSession._uDesGroupID &&
					iteChild->second._uOptType == OptSession._uOptType)
				{
					bRe = TRUE;
				}
			}

			OptSession._uSrcGroupID = iteChild->second._uSrcGroupID;
			OptSession._uDesGroupID = iteChild->second._uDesGroupID;
			if (!bRe)
			{
				OptSession._uOptType = iteChild->second._uOptType;
			}

			(*ite).second.erase(iteChild);
			return bRe;
		}	
	}
	return FALSE;
}

//! ���һ����ҵĳ�ʱ�Ự
void LinkmanSystem::ClearOutTimeSession(map<CGUID, tagOptSession> &PlayerSession)
{
	LONG lNow = (long)time(NULL);
	map<CGUID, tagOptSession>::iterator ite = PlayerSession.begin();
	while(ite != PlayerSession.end())
	{
		if(lNow > (long)ite->second._uEndTime)
		{
			ite = PlayerSession.erase(ite);
			continue;
		}
		++ite;
	}
}

void LinkmanSystem::OutLinkmanInfo(VOID)
{
	char szTmp[1204] = {0};
	
	LONG lPlayerNum = (LONG)m_mapPlayerLinkMan.size();
	LONG lMeAddNum = 0;
	LONG lAddMeNum = 0;

	if(6000 > lPlayerNum)
	{
		map<CGUID, VecPlayerLinkMan>::iterator ite = m_mapPlayerLinkMan.begin();
		for (; m_mapPlayerLinkMan.end() != ite; ++ite)
		{
			for (size_t i = 0; i < ite->second.size(); ++i)
			{
				lMeAddNum += (LONG)ite->second[i].GetMeAddMap().size();
				lAddMeNum += (LONG)ite->second[i].GetAddMeMap().size();
			}
		}
	}
	else
	{
		lMeAddNum = -1;
		lAddMeNum = -1;
	}

	//sprintf(szTmp, "LinkmanPlayerNum = %d\r\nLinkmanMeAddNum = %d\r\nLinkmanAddMeNum = %d\r\n\r\n", lPlayerNum, lMeAddNum, lAddMeNum);
	//PutStringToFile("MemAllocInfo", szTmp);

    Log4c::Trace(ROOT_MODULE,"LinkmanPlayerNum = %d\r\nLinkmanMeAddNum = %d\r\nLinkmanAddMeNum = %d\r\n\r\n", lPlayerNum, lMeAddNum, lAddMeNum);
}

