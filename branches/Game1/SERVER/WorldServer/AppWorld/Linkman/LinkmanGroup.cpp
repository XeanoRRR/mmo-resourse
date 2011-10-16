
#include "StdAfx.h"
#include "LinkmanGroup.h"
#include "../../worldserver/game.h"
#include "../Player.h"
#include "../GlobalRgnManager.h"
#include "..\nets\networld\message.h"
#include "../dbentity/entityManager.h"
#include "../DBEntity/Account.h"

LinkmanGroup::LinkmanGroup(const LinkmanGroupSetup* pSetup)
:
m_pSetup(pSetup)
{
	assert(NULL != pSetup);
}

LinkmanGroup::~LinkmanGroup(void){}

void LinkmanGroup::Release(void)
{
	//! �ͷ����õ�����
	for (map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.begin(); ite != m_mapMeAdd.end(); ++ite)
	{
		ReleaseLinkman_data(&(ite->second._date));
	}

	m_mapMeAdd.clear();
	m_mapAddMe.clear();
}

//!									
BOOL LinkmanGroup::AddMember(tagLinkman_DB &Linkman_DB)
{
	assert(NULL_GUID != Linkman_DB.AimGUID);
	assert(Linkman_DB.OwnerGUID != Linkman_DB.AimGUID);
	if(m_mapMeAdd.end() != m_mapMeAdd.find(Linkman_DB.AimGUID)) return FALSE;

	tagLinkman_data* pLinkman_data = CreateLinkman_data(Linkman_DB.AimGUID, Linkman_DB.szAimName);
	
	tagMeAddPlayer MeAddPlayer;
	MeAddPlayer._date = pLinkman_data;
	if (Linkman_DB.uDelState != eLDS_NotDel)
	{
		MeAddPlayer._date->uState = eLST_Deleted;
	}

	//! �໥��ϵ
	if(m_pSetup->m_bPartMutual)
	{
		if (m_mapAddMe.end() != m_mapAddMe.find(Linkman_DB.AimGUID))
			MeAddPlayer._IsMutual = TRUE;
		else
			MeAddPlayer._IsMutual = FALSE;
	}

	m_mapMeAdd.insert(pair<CGUID, tagMeAddPlayer>(Linkman_DB.AimGUID, MeAddPlayer));
	
	return TRUE;
}

//!									
BOOL LinkmanGroup::DelMember(const CGUID &PlayerGuid)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() == ite) return FALSE;

	ReleaseLinkman_data(&(ite->second._date));
	m_mapMeAdd.erase(ite);

	return TRUE;
}

//!									
CGUID LinkmanGroup::FindMember(const char *pPlayerName)
{
	for (map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.begin(); ite != m_mapMeAdd.end(); ++ite)
	{
		if (0 == stricmp(ite->second._date->szName, pPlayerName))
		{
			return ite->second._date->guid;
		}
	}
	return NULL_GUID;
}
//! ��Ա��ͼ�ı�
void LinkmanGroup::MemberMapChange(const CGUID &PlayerGuid, const char *pMapName)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() != ite)
	{
		memmove(ite->second._date->szMapName, pMapName, MAX_PLAYER_TITLE_SIZE);
		ite->second._date->szMapName[MAX_PLAYER_TITLE_SIZE - 1] = 0;
	}
}

//! ��Ա�ȼ��ı�
void LinkmanGroup::MemberLevelChange(const CGUID &PlayerGuid, LONG lLevel)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() != ite)
		ite->second._date->lLevel = lLevel;
}

//!										��Աְҵ�ı�
void LinkmanGroup::MemberOccuChange(const CGUID &PlayerGuid, LONG lOccu)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() != ite)
		ite->second._date->lOccupation = lOccu;
}

//!
BOOL LinkmanGroup::AddCompriseMePlayer(tagLinkman_DB &Linkman_DB)
{
	assert(NULL_GUID != Linkman_DB.AimGUID);
	assert(NULL_GUID != Linkman_DB.OwnerGUID);
	assert(0 != strlen(Linkman_DB.szAimName));
	assert(0 < Linkman_DB.uGroupID);
	if (NULL_GUID == Linkman_DB.OwnerGUID || NULL_GUID == Linkman_DB.AimGUID || 0 == strlen(Linkman_DB.szAimName))
	{
		return FALSE;
	}
	if(m_mapAddMe.end() != m_mapAddMe.find(Linkman_DB.OwnerGUID)) 
	{
		return FALSE;
	}

	m_mapAddMe[Linkman_DB.OwnerGUID] = Linkman_DB;

	//! �໥��ϵ
	if(m_pSetup->m_bPartMutual)
	{
		map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(Linkman_DB.OwnerGUID);
		if (m_mapMeAdd.end() != ite)
		{
			ite->second._IsMutual = TRUE;
		}
	}

	return TRUE;
}

//!
BOOL LinkmanGroup::DelCompriseMePlayer(const CGUID &PlayerGuid)
{
	if (NULL_GUID == PlayerGuid)
		return FALSE;

	map<CGUID, tagLinkman_DB>::iterator ite = m_mapAddMe.find(PlayerGuid);
	if (m_mapAddMe.end() == ite) return FALSE;

	m_mapAddMe.erase(ite);

	//! �໥��ϵ
	if(m_pSetup->m_bPartMutual)
	{
		map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
		if (m_mapMeAdd.end() != ite)
		{
			ite->second._IsMutual = FALSE;
		}
	}

	return TRUE;
}

BOOL LinkmanGroup::FindCompriseMePlayer(const CGUID &PlayerGuid)
{
	return (m_mapAddMe.end() != m_mapAddMe.find(PlayerGuid));
}

//! һ����ϵ������
void LinkmanGroup::LinkmanLogin(const CGUID &PlayerGuid)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() == ite) 
		return;
	UpdateLinkman_data(ite->second._date);
}

//! һ����ϵ������
void LinkmanGroup::LinkmanLogout(const CGUID &PlayerGuid)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() == ite) 
		return;
	ite->second._date->uState = eLST_Offline;
}

//! ��ϵ�˵�ɾ��״̬�ı�
void LinkmanGroup::LinkmanDelStateChange(const CGUID &PlayerGuid, BOOL bDeleted)
{
	map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.find(PlayerGuid);
	if(m_mapMeAdd.end() == ite) 
		return;
	ite->second._date->uState = (bDeleted) ? eLST_Deleted : eLST_Offline;
}

//! ������ϵ������״̬
void LinkmanGroup::UpdateOnlineState(void)
{
	for (map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.begin(); ite != m_mapMeAdd.end(); ++ite)
	{
		UpdateLinkman_data(ite->second._date);
	}
}

//! ���뵽����
void LinkmanGroup::Serialize_ForGs(CMessage *pMsg)
{
	pMsg->Add((LONG)m_mapMeAdd.size());
	for (map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.begin(); ite != m_mapMeAdd.end(); ++ite)
	{
		tagLinkman Linkman;
		memmove(static_cast<tagLinkman_data*>(&Linkman), ite->second._date, sizeof(tagLinkman_data));
		Linkman.bMutual = ite->second._IsMutual;
		pMsg->AddEx(&Linkman, sizeof(tagLinkman));
	}
	
	pMsg->Add((LONG)m_mapAddMe.size());
	for (map<CGUID, tagLinkman_DB>::iterator ite = m_mapAddMe.begin(); ite != m_mapAddMe.end(); ++ite)
	{
		pMsg->AddEx(&(ite->second), sizeof(tagLinkman_DB));
	}
}

//! ��GS�������������н���
void LinkmanGroup::Decode_FromGs(BYTE *pData, LONG &lPos)
{
	//! ����ӵ����
	m_mapMeAdd.clear();
	LONG lMeAddNum = _GetLongFromByteArray(pData, lPos);
	for (LONG lMeAddIdx = 0; lMeAddIdx < lMeAddNum; ++lMeAddIdx)
	{
		tagLinkman_data Linkman_data;
		_GetBufferFromByteArray(pData, lPos, &Linkman_data, sizeof(tagLinkman_data));
		tagLinkman_DB Linkman_DB;			
		Linkman_DB.AimGUID		= Linkman_data.guid;
		Linkman_DB.uDelState	= eLDS_NotDel;
		memmove(Linkman_DB.szAimName, Linkman_data.szName, MAX_PLAYER_NAME_SIZE);
		AddMember(Linkman_DB);
	}

	//! ����ҵ����
	m_mapAddMe.clear();
	LONG lAddMeNum = _GetLongFromByteArray(pData, lPos);
	for (LONG lAddMeIdx = 0; lAddMeIdx < lAddMeNum; ++lAddMeIdx)
	{
		tagLinkman_DB Linkman_DB;
		_GetBufferFromByteArray(pData, lPos, &Linkman_DB, sizeof(tagLinkman_DB));
		m_mapAddMe[Linkman_DB.OwnerGUID] = Linkman_DB;
	}
}

//! ���뵽����DBS������
void LinkmanGroup::Serialize_ForDBS(vector<tagLinkman_DB> &vLinkman)
{
	for (map<CGUID, tagMeAddPlayer>::iterator ite = m_mapMeAdd.begin(); ite != m_mapMeAdd.end(); ++ite)
	{
		tagLinkman_DB Linkman_DB;
		Linkman_DB.AimGUID = ite->second._date->guid;
		Linkman_DB.uDelState = (eLST_Deleted == ite->second._date->uState) ? eLDS_FlagDel : eLDS_NotDel;
		Linkman_DB.uGroupID = m_pSetup->m_uID;
		memmove(Linkman_DB.szAimName, ite->second._date->szName, MAX_PLAYER_NAME_SIZE);
		
		vLinkman.push_back(Linkman_DB);
	}
}

//��									ת������ҵ����ID�б�����
void LinkmanGroup::AddMeGuidToVector(vector<tagLinkman_DB> &vLinkman_DB)
{
	for (map<CGUID, tagLinkman_DB>::iterator ite = m_mapAddMe.begin(); ite != m_mapAddMe.end(); ++ite)
	{
		vLinkman_DB.push_back(ite->second);
	}
}

//!ȡ������ҵ����
const map<CGUID, tagMeAddPlayer>& LinkmanGroup::GetMeAddMap(void)
{
	return m_mapMeAdd;
}
//!ȡ������ҵ����
const map<CGUID, tagLinkman_DB>& LinkmanGroup::GetAddMeMap(void)
{
	return m_mapAddMe;
}

//! �õ�����ӵ�����
LONG LinkmanGroup::GetMemberNum(void)
{
	return m_mapMeAdd.size();
}

void LinkmanGroup::UpdateLinkman_data(tagLinkman_data* pLinkman_data)
{
	if(NULL == pLinkman_data) return;

	CPlayer *pPlayer = GetGame()->GetMapPlayer(pLinkman_data->guid);
	pLinkman_data->uState = eLST_Offline;
	
	if (pPlayer != NULL)
	{
		strcpy(pLinkman_data->szName, pPlayer->GetName());
		CWorldRegion* pWorldRegion = GetGame()->GetGlobalRgnManager()->FindRgnByGUID(pPlayer->GetRegionExID());
		if(pWorldRegion)
		{	
			const char *pRegionName = pWorldRegion->GetName();
			LONG lNameSize = strlen(pRegionName);
			lNameSize = (lNameSize < MAX_PLAYER_TITLE_SIZE) ? lNameSize : MAX_PLAYER_TITLE_SIZE - 1;
			memmove(pLinkman_data->szMapName, pRegionName, lNameSize);		//! ���ڵ�ͼ
			pLinkman_data->szMapName[lNameSize] = 0;
		}
		else
		{
			CGlobalRgnManager::tagRegion* pTagRgn = GetGame()->GetGlobalRgnManager()->GetTemplatePersonalRegion(pPlayer->GetRegionID());
			if(!pTagRgn)
				pTagRgn = GetGame()->GetGlobalRgnManager()->GetTemplatePersonalHouseRegion(pPlayer->GetRegionID());
			if(!pTagRgn)
				pTagRgn = GetGame()->GetGlobalRgnManager()->GetTemplateTeamRegion(pPlayer->GetRegionID());
			if(!pTagRgn)
			{
				char szGuid[128] = {0};
				pPlayer->GetRegionExID().tostring(szGuid);
				CBaseObject *pFather = pPlayer->GetFather();
				if(NULL != pFather)
					AddErrorLogText("��ȡ��ϵ����Ϣʱ��RgnManager���Ҳ���GUID[%s]�ĵ�ͼ��TemplateID = %d, FatherName = %s������", szGuid, pPlayer->GetRegionID(), pFather->GetName());
				else
					AddErrorLogText("��ȡ��ϵ����Ϣʱ��RgnManager���Ҳ���GUID[%s]�ĵ�ͼ��TemplateID = %d, FatherName = NULL������", szGuid, pPlayer->GetRegionID());
			}
		}
		
		pLinkman_data->szMapName[MAX_PLAYER_TITLE_SIZE - 1] = 0;
		pLinkman_data->lLevel		= pPlayer->GetLevel();							//! �ȼ�
		pLinkman_data->lOccupation	= pPlayer->GetOccupation();						//! ְҵ
		
		if (pPlayer)
		{
			Account* tLoginInfo = GetGame()->GetEntityManager()->FindAccount(pPlayer->GetAccount());
			if(!tLoginInfo)
			{
				char outStr[1024];
				sprintf(outStr, "Err UpdateLinkman_data: LoginInfo[%s] Is Null!", pPlayer->GetAccount());
				AddLogText(outStr);
				return;
			}
			if( tLoginInfo && PLAYER_STATE_BACKTOLOGIN != tLoginInfo->GetPlayerState() )
			{
				pLinkman_data->uState = eLST_Online;
			}
		}
		return;
	}

	
}

tagLinkman_data* LinkmanGroup::CreateLinkman_data(const CGUID &PlayerGuid, const char *pPlayerName)
{
	if (NULL == pPlayerName || NULL_GUID == PlayerGuid) return NULL;

	//! �������ݶ���
	tagLinkman_data* pLinkmanData = OBJ_CREATE(tagLinkman_data); //(tagLinkman_data*)GetGame()->GetMemPool().Multi_Alloc(sizeof(tagLinkman_data), _FILE_AND_LINE_);
	//new(pLinkmanData)tagLinkman_data();

	//! ��д���������Ϣ
	pLinkmanData->guid = PlayerGuid;
	memmove(pLinkmanData->szName, pPlayerName, MAX_PLAYER_NAME_SIZE);

	//! ��ȡ������ϸ��Ϣ
	UpdateLinkman_data(pLinkmanData);

	return pLinkmanData;
}

//! �ͷ�һ��tagLinkman_data����
void LinkmanGroup::ReleaseLinkman_data(tagLinkman_data **ppLinkman_data)
{
	OBJ_RELEASE (tagLinkman_data,*ppLinkman_data);
	//GetGame()->GetMemPool().Multi_Free(*ppLinkman_data, sizeof(tagLinkman_data));
	*ppLinkman_data = NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
LinkmanOptCache::LinkmanOptCache(void){}
LinkmanOptCache::~LinkmanOptCache(void){}



//! ����һ����Ӳ���
void LinkmanOptCache::PushLinkmanAddOpt(tagLinkman_DB &Linkman_DB)
{
	set<tagLinkman_DB, OptCacheData_compare>::iterator ite = m_AddCache.find(Linkman_DB);
	if (m_AddCache.end() == ite)
	{
		m_AddCache.insert(Linkman_DB);
		m_DelCache.erase(Linkman_DB);
	}
}
//! ����һ��ɾ������
void LinkmanOptCache::PushLinkmanDelOpt(tagLinkman_DB &Linkman_DB)
{
	set<tagLinkman_DB, OptCacheData_compare>::iterator ite = m_DelCache.find(Linkman_DB);
	if (m_DelCache.end() == ite)
	{
		m_DelCache.insert(Linkman_DB);
		m_AddCache.erase(Linkman_DB);
	}
}

//! ȷ��һ������û�б�ɾ��
BOOL LinkmanOptCache::AffirmLinkmanDataBeing(tagLinkman_DB &Linkman_DB)
{
	return m_DelCache.end() == m_DelCache.find(Linkman_DB);
}
//! ����һ����ұ�����ӵ�����
void LinkmanOptCache::FindNewAddPlayerData(list<tagLinkman_DB> &listLinkman_DB, const CGUID& AimGuid)
{
	listLinkman_DB.clear();
	set<tagLinkman_DB, OptCacheData_compare>::iterator ite = m_AddCache.begin();
	while (m_AddCache.end() != ite)
	{
		if(AimGuid == ite->AimGUID)
			listLinkman_DB.push_back(*ite);
		++ite;
	}
}

//! ���һ����ҵĻ���
void LinkmanOptCache::ClearOnePlayerData(const CGUID& OwnerGuid)
{
	set<tagLinkman_DB, OptCacheData_compare>::iterator ite = m_AddCache.begin();
	while (m_AddCache.end() != ite)
	{
		if(OwnerGuid == ite->OwnerGUID)
		{
			ite = m_AddCache.erase(ite);
			continue;
		}
		++ite;
	}

	ite = m_DelCache.begin();
	while (m_DelCache.end() != ite)
	{
		if(OwnerGuid == ite->OwnerGUID)
		{
			ite = m_DelCache.erase(ite);
			continue;
		}
		++ite;
	}
}