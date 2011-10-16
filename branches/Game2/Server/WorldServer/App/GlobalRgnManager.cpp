//GlobalRgnManager.h/////////////////////////////////////////////////////////////////////
//������:�ö���������еĳ�������ʵ��
//Author:������
//Create Time:2008.11.03
#include "StdAfx.h"
#include "GlobalRgnManager.h"
#include "WorldRegion.h"
#include "script/VariableList.h"
#include "DBEntity/EntityManager.h"      
#include "../Net/Message.h"

#include "../../../Public/Setup/GlobalSetup.h"
#include "../../../Public/Setup/RegionPlugSetup.h"

CGlobalRgnManager::CGlobalRgnManager()
{
	m_mapRgn.clear();
}
CGlobalRgnManager::~CGlobalRgnManager()
{
	MapRgnItr itr = m_mapRgn.begin();
	for(; itr != m_mapRgn.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	m_mapRgn.clear();
	m_mapRegionList.clear();

	// Personal Rgn
	map<long, tagRegion>::iterator tRgnItr = m_mapPersonalRgnList.begin();
	for(; tRgnItr != m_mapPersonalRgnList.end(); tRgnItr++)
	{
		SAFE_DELETE(tRgnItr->second.pRegion);
	}
	m_mapPersonalRgnList.clear();

	// Personal House Rgn
	tRgnItr = m_mapPersonalHouseRgnList.begin();
	for(; tRgnItr != m_mapPersonalHouseRgnList.end(); tRgnItr++)
	{
		SAFE_DELETE(tRgnItr->second.pRegion);
	}
	m_mapPersonalHouseRgnList.clear();

	// Team Rgn
	tRgnItr = m_mapTeamRgnList.begin();
	for(; tRgnItr != m_mapTeamRgnList.end(); tRgnItr++)
	{
		SAFE_DELETE(tRgnItr->second.pRegion);
	}
	m_mapTeamRgnList.clear();
}


//! ��ȡIni�����ļ�����
bool CGlobalRgnManager::LoadRgnSetupIni(const char* strFilePath)
{
	string strTemp;

	CRFile* prfile = rfOpen(strFilePath);
	if( prfile )
	{
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);

		extern long g_lTotalMonster;
		long lM=0;

		// ��������Ϣ
		tagRegion stRegion;
		DWORD dwID = 0;
		DWORD dwResourceID = 0;
		long lSvrResourceID = 0;
		DWORD dwAreaId=0;
		float fExpScale = 1.0f;
		float fOccuExpScale = 1.0f;
		float fSpScale = 1.0f;
		float fMeriScale = 1.0f;
		DWORD dwWarType = RWT_Normal;
		long  nRide = 1;
		long nChangeEquip = 1;
		int nNoPk = 0;
		int nNoContribute = 0;
		string strName;
		DWORD dwIndex = 0;
		int btCountry = 0;
		long lNotify= 0;
		int nSpaceType=0;

		// ����ID,0:��ʾ���Ƿ��߳���,>0��ʾ�Ƿ��߳�����ID
		long lLinedIdFlag = 0;
		long lACRgnType = 0;
		int lRgnHideFlag = 0;
		//  [8/26/2009 chenxianj]
		int nReliveFlag = 0;
		long nDeadReturn = 0;
		long lStrongPointFlag=0;
        long lReJoinTeam=0;

		DWORD dupType = 0;
		while(ReadTo(stream, "#"))
		{
			string szGuid;
			stream >> dupType 
				>> dwID 
				>> strName 
				>> dwResourceID 
				>> lSvrResourceID
				>> dwAreaId 
				>> fExpScale 
				>> fOccuExpScale 
				>> fSpScale 
				>> fMeriScale 
				>> dwWarType
				>> nRide
				>> nChangeEquip
				>> nNoPk 
				>> nNoContribute 
				>> dwIndex 
				>> btCountry 
				>> lNotify
				>> nSpaceType 
				>> szGuid 
				>> lLinedIdFlag
				>> lACRgnType
				>> lRgnHideFlag
				>> nReliveFlag
				>> nDeadReturn
				>> lStrongPointFlag
                >> lReJoinTeam;
			stRegion.dwGameServerIndex = dwIndex;
			stRegion.RegionType = (eRgnType)dupType;

            if ( dwID < 1E6 || dwID >= 1E7)
            {
                Log4c::Trace(ROOT_MODULE, "RegionID is too small or too big!", dwID);
            }
			CGUID rgnGuid(szGuid.c_str());

			// ���볡��
			CWorldRegion* pRegion = new CWorldRegion;

			if(pRegion)
			{
				pRegion->SetRgnType(dupType);
				pRegion->SetAreaId(dwAreaId);
				pRegion->SetExID(rgnGuid);
				pRegion->SetID(dwID);
				pRegion->SetName(strName.c_str());
				pRegion->SetWarType((eRWT)dwWarType);
				pRegion->SetSpaceType((eRST)nSpaceType);
				pRegion->SetResourceID(dwResourceID);
				pRegion->SetSvrResourceID( lSvrResourceID );
				pRegion->SetExpScale(fExpScale);
				pRegion->SetOccuExpScale(fOccuExpScale);
				pRegion->SetSpScale(fSpScale);
				pRegion->SetMeriScale(fMeriScale);
				pRegion->SetCountry(btCountry);
				pRegion->SetNotify(lNotify);
				pRegion->SetLinedIdFlag(lLinedIdFlag);
				pRegion->SetRide(nRide==0?false:true);
				pRegion->SetChangeEquip(nChangeEquip==0?false:true);
				pRegion->SetRelive(nReliveFlag==0?false:true);
				pRegion->SetDeadReturn(nDeadReturn==0?false:true);
				if(lLinedIdFlag != 0) // �Ƿ��߳���ID
				{
					m_mapLinedRgnIdByLinedId[lLinedIdFlag].push_back(dwID);
				}
				pRegion->SetACRgnType( lACRgnType );
				pRegion->SetRgnHideFlag(lRgnHideFlag);

				pRegion->SetNoPk( nNoPk);
				pRegion->SetNoContribute( nNoContribute==0?false:true );
				pRegion->SetGsid(dwIndex);
                pRegion->SetStrongPointFlag( lStrongPointFlag>0 ? true : false );
                pRegion->SetRgnRejoinFlag( lReJoinTeam );

				if( pRegion->Load() )
				{
					stRegion.pRegion = pRegion;

					//!!!GS��NormalRgnʹ��ģ���NormalRgn����ָ��!!!
					switch((eRgnType)dupType) 
					{
					case RGN_NORMAL:// Normal Rgn
						{
							// ����ģ��map
							m_mapRegionList[dwID] = stRegion;

							pRegion->SetGsid(dwIndex);

							// ����RgnMap
							MapRgnItr rgnItr = m_mapRgn.find(rgnGuid);
							if(rgnItr == m_mapRgn.end())// δ�ҵ�
							{
								m_mapRgn[rgnGuid] = pRegion;
							}
							else
							{
								Log4c::Warn(ROOT_MODULE,FormatText("WS_RGNMGR_2", szGuid.c_str()));
							}

							// ��������
							pRegion->SetRgnType((long)dupType);
							pRegion->SetIsTemplateRgnFlag(true);

							Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMGR_3", dwID, strName.c_str(), g_lTotalMonster-lM));
						}
						break;
					case RGN_PERSONAL: // Personal Rgn
						{
							// ����ģ��map
							m_mapPersonalRgnList[dwID] = stRegion;

							// ��������
							pRegion->SetRgnType((long)dupType);
							pRegion->SetIsTemplateRgnFlag(true);

							Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMGR_4", dwID, strName.c_str(), g_lTotalMonster-lM));
						}
						break;
					case RGN_PERSONAL_HOUSE: // Personal Rgn
						{
							// ����ģ��map
							m_mapPersonalHouseRgnList[dwID] = stRegion;

							// ��������
							pRegion->SetRgnType((long)dupType);
							pRegion->SetIsTemplateRgnFlag(true);

							Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMGR_5", dwID, strName.c_str(), g_lTotalMonster-lM));
						}
						break;
					case RGN_TEAM: // Personal Rgn
						{
							// ����ģ��map
							m_mapTeamRgnList[dwID] = stRegion;

							// ��������
							pRegion->SetRgnType((long)dupType);
							pRegion->SetIsTemplateRgnFlag(true);

							Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMGR_6", dwID, strName.c_str(), g_lTotalMonster-lM));
						}
						break;
					}

					lM = g_lTotalMonster;
				}
				else
				{
					Log4c::Warn(ROOT_MODULE,FormatText("WS_RGNMGR_7", dwID, strName.c_str()));
				}
			}
			else
			{
				Log4c::Warn(ROOT_MODULE,FormatText("WS_RGNMGR_8", dwID, strName.c_str()));
			}
		}

		Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMGR_9", g_lTotalMonster));
	}
	else
	{
		Log4c::Warn(ROOT_MODULE,FormatText("WS_RGNMGR_10", strFilePath));
		return false;
	}

	return true;
}
//! ���㵱ǰGS�����͸����Ƿ�ﵽ����,������ЧGS��gsid
long CGlobalRgnManager::GetCurGsDupRgnNumIsMax(eRgnType rgnType)
{
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			for(size_t i=0; i< m_vecFirstPersonalServer.size(); i++)
			{	
				GSConnection* gsInfo = GetNetHandler().GetGS(m_vecFirstPersonalServer[i]);
                if(gsInfo != NULL && CMessage::IsGSReady(gsInfo->index))
				{
					// �жϸ���
					long dupRgnNum = 0;
					MapRgnItr gsRgnItr = m_mapRgn.begin();
					for(; gsRgnItr != m_mapRgn.end(); gsRgnItr++)
					{
						if(gsRgnItr->second->GetGsid() == m_vecFirstPersonalServer[i] && gsRgnItr->second->GetRgnType() == rgnType)
						{
							dupRgnNum++;
						}
					}

					// (����)���������monster������������ ����ѡ��һ�����ʵ�GS
					if( dupRgnNum >= GlobalSetup::GetSetup()->lPDupRgnMaxNum )
					{
						continue;
					}
					else
						return m_vecFirstPersonalServer[i];
				}
			}
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			for(size_t i=0; i<m_vecFirstPersonalHouseServer.size(); i++)
			{	
				GSConnection* gsInfo = GetNetHandler().GetGS(m_vecFirstPersonalHouseServer[i]);
				if(gsInfo != NULL && CMessage::IsGSReady(gsInfo->index))
				{
					// �жϸ���
					long dupRgnNum = 0;
					MapRgnItr gsRgnItr = m_mapRgn.begin();
					for(; gsRgnItr != m_mapRgn.end(); gsRgnItr++)
					{
						if(gsRgnItr->second->GetGsid() == m_vecFirstPersonalHouseServer[i] && gsRgnItr->second->GetRgnType() == rgnType)
						{
							dupRgnNum++;
						}
					}

					// (����)���������monster������������ ����ѡ��һ�����ʵ�GS				
					if( dupRgnNum >= GlobalSetup::GetSetup()->lTDupRgnMaxNum )
					{
						continue;
					}
					else
						return m_vecFirstPersonalHouseServer[i];
				}
			}
		}
		break;
	case RGN_TEAM:
		{
			for(size_t i=0; i<m_vecFirstTeamServer.size(); i++)
			{	
				GSConnection* gsInfo = GetNetHandler().GetGS(m_vecFirstTeamServer[i]);
				if(gsInfo != NULL && CMessage::IsGSReady(gsInfo->index))
				{
					// �жϸ���
					long dupRgnNum = 0;
					MapRgnItr gsRgnItr = m_mapRgn.begin();
					for(; gsRgnItr != m_mapRgn.end(); gsRgnItr++)
					{
						if(gsRgnItr->second->GetGsid() == m_vecFirstTeamServer[i] && gsRgnItr->second->GetRgnType() == rgnType)
						{
							dupRgnNum++;
						}
					}

					// (����)���������monster������������ ����ѡ��һ�����ʵ�GS				
					if( dupRgnNum >= GlobalSetup::GetSetup()->lTDupRgnMaxNum )
					{
						continue;
					}
					else
						return m_vecFirstTeamServer[i];
				}
			}
		}
		break;
	}

	return -1;
}
//! ����ָ��GS��Rgn
bool CGlobalRgnManager::CreateGSRgn(long templateRgnID,
									eRgnType rgnType,
									const CGUID& ownerID,
									const CGUID& scriptID/*����Ľű�ID*/,
									long pMsgGSID,
									const char* szVarName,
									const char* szRgnIDName,
									bool isDupRgnDataFlag)
{
	long retFlag = INVILID_VALUE;
	CGUID rgnID = NULL_GUID;
	CWorldRegion* retRgn = NULL;
	// �������ɸ�����GSID
	long GSID = pMsgGSID;

	// �ȼ���ܷ������ȷ�������������
	GSID = GetCurGsDupRgnNumIsMax(rgnType);

	// ���з�����������
	if(GSID == -1)
	{
		CMessage msg(MSG_W2S_RGN_CREATE);
		//msg.Add(GSID);
		msg.Add(templateRgnID);
		msg.Add((long)rgnType);
		msg.Add(ownerID);
		msg.Add(NULL_GUID);
		msg.Add((long)C_RET_ALL_RGNS_FULL_ERROR); // ���з�����������
		msg.Add((long)GSID); //  ������ID
		msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
		msg.Add((long)0); // �������ݳ���
		msg.Add(scriptID);
		if(szVarName)
			msg.Add(szVarName);  // �첽��������ֵ������
		else
			msg.Add("");
		if(szRgnIDName)
			msg.Add(szRgnIDName);  // �첽��������ֵ������
		else
			msg.Add("");
		msg.Add((BYTE)isDupRgnDataFlag);// �Ƿ��л�������
		msg.SendToGS(pMsgGSID);	

		retFlag = C_RET_ALL_RGNS_FULL_ERROR;
		return false;
	}

	CWorldRegion* wRgn = NULL;
	CPlayer* owner = GetGame().GetMapPlayer(ownerID);
	if(owner == NULL)
	{
		CMessage msg(MSG_W2S_RGN_CREATE);
		//msg.Add(GSID);
		msg.Add(templateRgnID);
		msg.Add((long)rgnType);
		msg.Add(ownerID);
		msg.Add(NULL_GUID);
		msg.Add((long)C_RET_NOT_FIND_OWNER_ERROR); // δ�ҵ����
		msg.Add((long)GSID); //  ������ID
		msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
		msg.Add((long)0); // �������ݳ���
		msg.Add(scriptID);
		if(szVarName)
			msg.Add(szVarName);  // �첽��������ֵ������
		else
			msg.Add("");
		if(szRgnIDName)
			msg.Add(szRgnIDName);  // �첽��������ֵ������
		else
			msg.Add("");
		msg.Add((BYTE)isDupRgnDataFlag);// �Ƿ��л�������
		msg.SendToGS(pMsgGSID);	

		retFlag = C_RET_NOT_FIND_OWNER_ERROR;
		return false;
	}
	wRgn = CreateRgn(isDupRgnDataFlag, pMsgGSID, templateRgnID, rgnType, GSID, ownerID, scriptID, NULL_GUID, szVarName, szRgnIDName); // ����Rgn ����GS������Ϣ
	return false;
}

CWorldRegion* CGlobalRgnManager::GetWorldRegionByRgnId(long lId)
{
	map<long, tagRegion>::iterator it=m_mapRegionList.find(lId);	
	if(it!=m_mapRegionList.end())
	{
		return it->second.pRegion;
	}
	return NULL;
}

//! ɾ��ָ��GS��Rgn
bool CGlobalRgnManager::DeleteGSRgn(eRgnType rgnType, const CGUID& ownerID, const CGUID& rgnID, const CGUID& scriptID/*�ű�ID*/, long pMsgGSID, const char* szRetVarName, const char* szRgnIDName)
{
	DeleteRgn(rgnType, ownerID, rgnID, scriptID, szRetVarName, szRgnIDName); // ɾ��Rgn ����GS������Ϣ
	return true;
}
//! ����GSID by RgnGUID
long CGlobalRgnManager::FindGSIDByRgnID(const CGUID& ownerID)
{
	long ret = -1;

	MapRgnItr itr = m_mapRgn.find(ownerID);
	if(itr != m_mapRgn.end())
	{
		ret = itr->second->GetGsid();
	}

	return ret;
}


//! ����Region
CWorldRegion* CGlobalRgnManager::FindRgnByGUID(const CGUID& ownerID)
{
	CWorldRegion* ret = NULL;

	MapRgnItr itr = m_mapRgn.find(ownerID);
	if(itr != m_mapRgn.end())
	{
		ret = itr->second;
	}

	return ret;
}
//���ݵ�ͼ���ֵõ�tagRegionָ��
CGlobalRgnManager::tagRegion* CGlobalRgnManager::GetTemplateNormalRegion(const char* strName)
{
	map<long,tagRegion>::iterator RegionIter = m_mapRegionList.begin();
	for(; RegionIter != m_mapRegionList.end(); RegionIter++)
	{
		if(strcmp(RegionIter->second.pRegion->GetName(),strName)==0)
		{
			return &(RegionIter->second);
		}
	}
	return NULL;
}
//! ��װ���߳�������
bool CGlobalRgnManager::AddLinedRgnInfoToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, (long)m_mapLinedRgnIdByLinedId.size());
	map<long, vector<long>>::iterator itr = m_mapLinedRgnIdByLinedId.begin();
	for(; itr != m_mapLinedRgnIdByLinedId.end(); itr++)
	{
		_AddToByteArray(&pBA, itr->first);
		_AddToByteArray(&pBA, (long)itr->second.size());
		vector<long>::iterator templateIdItr = itr->second.begin();
		for(; templateIdItr != itr->second.end(); templateIdItr++)
		{
			// template id
			_AddToByteArray(&pBA, (*templateIdItr));
			// rgn guid
			tagRegion* pTRgn = GetTemplateNormalRegion((*templateIdItr));
			if( pTRgn && pTRgn->pRegion )
				_AddToByteArray(&pBA, pTRgn->pRegion->GetExID());
			else
				_AddToByteArray(&pBA, NULL_GUID);
		}
	}
	return true;
}
BOOL CGlobalRgnManager::LoadRegionPlugSetupIni(const char* szPath)
{
	return CRegionPlugSetup::LoadRegionPlugSetup(szPath);
}
// ��ȡ�����������ȷ����������ļ�
bool CGlobalRgnManager::LoadDupRgnServerIni(const char* filename)
{
	//////////////////////////////////////////////////////////////////////////
	// �� data\duprgnserver.ini ��ȡ
	//////////////////////////////////////////////////////////////////////////

	CRFile *prfile = rfOpen(filename);
	if(prfile == NULL)
	{
        Log4c::Error(ROOT_MODULE,"%-15s file '%s' can't found!",__FUNCTION__,filename);
		return false;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	m_vecFirstPersonalHouseServer.clear();
	m_vecFirstPersonalServer.clear();
	m_vecFirstTeamServer.clear();

	string strTemp;
	string strSetupBegin;
	string strSetupEnd;
	string strServerType;
	long tmpServerID;
	// �ȶ�ȡ����һ��Begin
	ReadTo(stream, "<begin>");
	while(1)
	{
		stream >> strSetupEnd;
		if (strSetupEnd=="<end>" || stream.eof())
			break;

		// ��ȡ�����������Ͷ���
		if (strSetupEnd=="*")
		{
			stream >> strServerType;
		}

		// ���˷��ݸ���
		if(strServerType == "PersonalHouseRgnServerID")
		{
			while(1)
			{
				stream >> strSetupEnd;
				if (strSetupEnd=="<phend>" || stream.eof())
					break;

				if (strSetupEnd=="#")
				{	
					stream >> tmpServerID;
					m_vecFirstPersonalHouseServer.push_back(tmpServerID);
				}
			}
		}
		// ���˸���
		if(strServerType == "PersonalRgnServerID")
		{
			while(1)
			{
				stream >> strSetupEnd;
				if (strSetupEnd=="<pend>" || stream.eof())
					break;
				if (strSetupEnd=="#")
				{
					stream >> tmpServerID;
					m_vecFirstPersonalServer.push_back(tmpServerID);
				}
			}
		}
		// ��Ӹ���
		if(strServerType == "TeamRgnServerID")
		{
			while(1)
			{
				stream >> strSetupEnd;
				if (strSetupEnd=="<tend>" || stream.eof())
					break;
				if (strSetupEnd=="#")
				{
					stream >> tmpServerID;
					m_vecFirstTeamServer.push_back(tmpServerID);
				}
			}
		}
	}

	return true;
}

//! ����Rgn,��GS����RGN_CREATE��Ϣ
CWorldRegion* CGlobalRgnManager::CreateRgn(bool isDupRgnDataFlag,
										   long srcGSID/*���ʹ��������GSID*/,
										   long templateRgnID,
										   eRgnType rgnType,
										   long GSID,
										   const CGUID& ownerID,
										   const CGUID& scriptID,
										   const CGUID& dbRgnGUID,
										   const char* szRetVarName,
										   const char* szRetRgnIDName/*�첽����ֵ������*/)
{
	CWorldRegion* ret = NULL;
	CMessage msg(MSG_W2S_RGN_CREATE);
	msg.Add(templateRgnID); // ģ��Rgn��ID
	msg.Add((long)rgnType); // ģ��Rgn��Type
	msg.Add(ownerID);		// �����ߵ�GUID

	CGUID rgnID;
	if(dbRgnGUID == NULL_GUID)
		CGUID::CreateGUID(rgnID);
	else rgnID = dbRgnGUID;
	map<long, CGlobalRgnManager::tagRegion>::iterator tRgnItr;

	bool IsFindTemplateRgnFlag = true;
	switch(rgnType)
	{
	case RGN_PERSONAL:
		{
			tRgnItr = GetTemplatePersonalRgnListMap()->find(templateRgnID);
			if(tRgnItr == GetTemplatePersonalRgnListMap()->end())
				IsFindTemplateRgnFlag = false;
		}
		break;
	case RGN_PERSONAL_HOUSE:
		{
			tRgnItr = GetTemplatePersonalHouseRgnListMap()->find(templateRgnID);
			if(tRgnItr == GetTemplatePersonalHouseRgnListMap()->end())
				IsFindTemplateRgnFlag = false;
		}
		break;
	case RGN_TEAM:
		{
			tRgnItr = GetTemplateTeamRgnListMap()->find(templateRgnID);
			if(tRgnItr == GetTemplateTeamRgnListMap()->end())
				IsFindTemplateRgnFlag = false;
		}
		break;
	}
	if(IsFindTemplateRgnFlag == false)
	{
		msg.Add(rgnID); // RgnID
		msg.Add((long)C_RET_RGN_TEMPLAYTEID_ERROR);	    // δ�ҵ���ģ��RGN

		msg.Add((long)0); // srcgsid
		msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
		msg.Add((long)0); // bufsize

		msg.Add(scriptID);
		if(szRetVarName)
			msg.Add(szRetVarName);
		else
			msg.Add("");
		if(szRetRgnIDName)
			msg.Add(szRetRgnIDName);
		else
			msg.Add("");
		msg.SendToGS(srcGSID);
		return NULL;
	}

	// ȡ��ģ��Rgn
	CWorldRegion* worldRgn = NULL;

	bool canRgnCreated = true;
	CPlayer* pPlayer = GetGame().GetMapPlayer(ownerID);
	if(pPlayer)
	{
		switch(rgnType)
		{
		case RGN_PERSONAL_HOUSE:
			{
				if(pPlayer->GetPersonalHouseRgnGUID() != NULL_GUID && rgnType == RGN_PERSONAL_HOUSE)
					canRgnCreated = false;
			}
			break;
		case RGN_PERSONAL:
			{
			}
			break;
		case RGN_TEAM:
			{
			}
			break;
		}
	}

	// ���˸���δ����
	if(canRgnCreated)
	{
		// ��Ӹø�����������
		MapRgnItr rgnItr = m_mapRgn.find(rgnID);
		if(rgnItr == m_mapRgn.end())
		{
			if(rgnType == RGN_PERSONAL)
			{
				pPlayer->SetPersonalRgnGUID(rgnID);
				AddPersonalRgnGUID(ownerID, rgnID);
			}

			worldRgn = new CWorldRegion;	
			worldRgn->CloneData(tRgnItr->second.pRegion);
			if(!isDupRgnDataFlag) // û�л�������
			{
				worldRgn->New();
				worldRgn->CloneAllData(tRgnItr->second.pRegion);
			}
			// ��ʼ����Rgn
			worldRgn->SetExID(rgnID);
			// ��������
			worldRgn->SetRgnType((long)rgnType);
			m_mapRgn[rgnID] = worldRgn;
			ret = worldRgn;
			if(worldRgn)
				worldRgn->SetGsid(GSID);
			msg.Add(rgnID); // RgnID
			if(srcGSID == GSID)
				msg.Add((long)C_RET_LOCAL_SUCCESS);	    // �Ѿ�������־
			else
				msg.Add((long)C_RET_REMOTE_SUCCESS); // �Ѿ�������־, ��ҪWSID��srcGSID���ͻ�Ӧ��Ϣ

			msg.Add(srcGSID); // SRC GSID

			vector<BYTE> pRgnData;
			if(!isDupRgnDataFlag) // û�л�������
			{
				worldRgn->AddToByteArray(&pRgnData);
			}
			msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
			msg.Add((long)pRgnData.size());
			if(pRgnData.size())
				msg.AddEx(&pRgnData[0], (long)pRgnData.size());

			worldRgn->SetOwnerGUID(ownerID);
			if(pPlayer)
				pPlayer->SetPersonalHouseRgnGUID(worldRgn->GetExID());
		}
		else
		{
			msg.Add(rgnID); // RgnID
			msg.Add((long)C_RET_RGN_HAS_CREATED_ERROR);	    // �Ѿ�������־
			msg.Add(srcGSID); // SRC GSID
			msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
			msg.Add((long)0); // rgn data
		}
	}
	else
	{
		msg.Add(rgnID); // RgnID
		msg.Add((long)C_RET_RGN_HAS_CREATED_ERROR);	    // �Ѿ�������־
		msg.Add(srcGSID); // SRC GSID
		msg.Add((BYTE)isDupRgnDataFlag); // �������ݱ��
		msg.Add((long)0); // rgn data
	}

	msg.Add(scriptID);
	if(szRetVarName)
		msg.Add(szRetVarName);
	else
		msg.Add("");
	if(szRetRgnIDName)
		msg.Add(szRetRgnIDName);
	else
		msg.Add("");
	msg.SendToGS(GSID);

	return ret;
}

//! ɾ��Rgn,��WS����RGN_DELETE��Ϣ
void CGlobalRgnManager::DeleteRgn(eRgnType rgnType,
								  const CGUID&ownerID,
								  const CGUID& RgnID,
								  const CGUID& scriptID,
								  const char* szRetVarName,
								  const char* szRetRgnIDName/*�첽����ֵ������*/)
{
//	char logStr[2048];
	CGUID RgnOwnerID;

	long GSID = -1;

	CMessage msg(MSG_W2S_RGN_DELETE);

	msg.Add((long)rgnType); // ģ��Rgn��Type
	msg.Add(ownerID);
	msg.Add(RgnID); // Rgn ID

	// ɾ���ø����ӹ�����
	MapRgnItr rgnItr = m_mapRgn.find(RgnID);
	if(rgnItr != m_mapRgn.end())
	{
		RgnOwnerID = rgnItr->second->GetOwnerGUID();
		GSID = rgnItr->second->GetGsid();

		SAFE_DELETE(rgnItr->second);
		m_mapRgn.erase(rgnItr);
		msg.Add((BYTE)1); // ok
	}
	else
	{
		msg.Add((BYTE)0); // fail
		char szGUID[128];
		RgnID.tostring(szGUID);
		char szScriptID[128];
		scriptID.tostring(szScriptID);
// 		sprintf(logStr, AppFrame::GetText("WS_RGNMANAGER_DELETERGN"),
// 			szGUID, szScriptID, GSID, 0, -1);
// 		PutStringToFile("Login_WS_Info",logStr);

        Log4c::Trace(ROOT_MODULE,FormatText("WS_RGNMANAGER_DELETERGN",
            szGUID, szScriptID, GSID, 0, -1));
	}

	msg.Add(scriptID);
	if(szRetVarName)
		msg.Add(szRetVarName);
	else
		msg.Add("");
	if(szRetRgnIDName)
		msg.Add(szRetRgnIDName);
	else
		msg.Add("");

	msg.SendToGS(GSID);

	CPlayer* owner = GetGame().GetMapPlayer(ownerID);
	if(owner)
	{
		switch(rgnType)
		{
		case RGN_PERSONAL:
			{
				msg.SetType(MSG_W2S_RGN_DELETE);
				// ��ɾ���ĵ�ǰ����
				if(RgnID == owner->GetPersonalRgnGUID())
				{
					owner->SetPersonalRgnGUID(NULL_GUID);
					DelPersonalRgnGUID(ownerID);
				}
			}
			break;
		case RGN_PERSONAL_HOUSE:
			{
				msg.SetType(MSG_W2S_RGN_DELETE);
				// ��ɾ���ĵ�ǰ����
				if(RgnID == owner->GetPersonalHouseRgnGUID())
					owner->SetPersonalHouseRgnGUID(NULL_GUID);

				// �������ݿ�ɾ��DBDupRgn��Ϣ
				SendDelPHDupRgnMsgToDB(RgnID);
			}
			break;
		case RGN_TEAM:
			{
				msg.SetType(MSG_W2S_RGN_DELETE);
				// ��ɾ���ĵ�ǰ����
				GetGame().GetGlobalRgnManager()->DelTeamRgnGUID(owner->GetExID(), RgnID);
			}
			break;
		}
	}
}


// �������ݿ�ɾ��DBDupRgn��Ϣ
void CGlobalRgnManager::SendDelPHDupRgnMsgToDB(const CGUID& rgnGUID)
{
	CMessage msg(MSG_W2DB_OPER_ENTITY);
	//	msg.Add((long)COM_PERSONALHOUSERGN); // opType			4byte, ������������
	// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
	// DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
	// MSG ID:        4byte, ����Ϣ��MSGID
	// Entity Num:	2byte, ��ʵ�����(X)
	msg.Add((BYTE)0);

	msg.Add((long)-1);
	msg.Add((WORD)1);
	// GUID
	vector<BYTE> pBA;
	_AddToByteArray(&pBA, rgnGUID);
	msg.Add((long)pBA.size());
	if(pBA.size())
		msg.AddEx(&pBA[0], (long)pBA.size());
	msg.SendToServer(NF_DB_SERVER);
}
void CGlobalRgnManager::AddTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID, const CGUID& rgnGuid, BOOL bCover)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.find(lTemplateRgnID);
		if(ite->second.end() != itr && !bCover)
			return;
		ite->second[lTemplateRgnID] = rgnGuid;
	}
	else
	{
		map<LONG, CGUID> teamGuidMap;
		teamGuidMap[lTemplateRgnID] = rgnGuid;
		m_mapTeamRgnGUID[ownerID] = teamGuidMap;
	}
}
void CGlobalRgnManager::DelTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
		ite->second.erase(lTemplateRgnID);
}
void CGlobalRgnManager::DelTeamRgnGUID(const CGUID& ownerID, const CGUID& rgnGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(itr->second == rgnGuid)
			{
				ite->second.erase(itr);
				return;
			}
		}
	}
}
//! ���һ����ҵĶ��鸱��
void CGlobalRgnManager::ClearTeamRgnGUID(const CGUID& ownerGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerGuid);
	if( ite != m_mapTeamRgnGUID.end() )
		ite->second.clear();
}
const CGUID& CGlobalRgnManager::GetTeamRgnGUID(const CGUID& ownerID, LONG lTemplateRgnID)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		map<LONG, CGUID>::iterator itr = ite->second.find(lTemplateRgnID);
		if(ite->second.end() != itr)
			return itr->second;
	}
	return NULL_GUID;
}
//! ͨ��˳��õ�һ������ĸ���GUID
const CGUID& CGlobalRgnManager::GetTeamRgnGuidByIndex(const CGUID& ownerID, LONG lIndex)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		if( (long) ite->second.size() < lIndex || 0 == lIndex)
			return NULL_GUID;
		LONG lCurrIndex = 1;
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(lIndex == lCurrIndex)
			{
				return itr->second;
			}
			++lCurrIndex;
		}
	}
	return NULL_GUID;
}
//! ͨ��˳��õ�һ������ĸ���ģ��ID
LONG CGlobalRgnManager::GetTeamRgnTemplateIdByIndex(const CGUID& ownerID, LONG lIndex)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		if( (long) ite->second.size() < lIndex || 0 == lIndex)
			return -1;
		LONG lCurrIndex = 1;
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			if(lIndex == lCurrIndex)
			{
				return itr->first;
			}
			++lCurrIndex;
		}
	}
	return -1;
}
map<LONG, CGUID>*	CGlobalRgnManager::GetTeamRgnGuidMap(const CGUID& ownerGuid)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerGuid);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		return &ite->second;
	}
	return NULL;
}
VOID CGlobalRgnManager::RgnGuidCode(const CGUID& ownerID, vector<BYTE> &vData)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		_AddToByteArray(&vData, (LONG)ite->second.size());
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			_AddToByteArray(&vData, itr->first);
			_AddToByteArray(&vData, itr->second);
		}
	}
}
VOID CGlobalRgnManager::RgnGuidDecode(const CGUID& ownerID, BYTE *pData, LONG &lPos)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		ite->second.clear();
		LONG lNum = _GetLongFromByteArray(pData, lPos);
		for (LONG i = 0; i < lNum; ++i)
		{
			LONG lTemplateRgnID = _GetLongFromByteArray(pData, lPos);
			CGUID guid;
			_GetBufferFromByteArray(pData, lPos, guid);
			if(ite->second.end() == ite->second.find(lTemplateRgnID))
			{
				ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
				CWorldRegion* pRgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(guid);
				if(pRgn)
					pRgn->SetOwnerGUID(ownerID);
			}
		}
	}
}
//! ���뽻�����鸱������������ͬģ��ĸ�����
VOID CGlobalRgnManager::RgnGuidSwapFromArray(const CGUID& ownerID, BYTE *pData, LONG &lPos)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		LONG lNum = _GetLongFromByteArray(pData, lPos);
		for (LONG i = 0; i < lNum; ++i)
		{
			LONG lTemplateRgnID = _GetLongFromByteArray(pData, lPos);
			CGUID guid;
			_GetBufferFromByteArray(pData, lPos, guid);
			if(ite->second.end() == ite->second.find(lTemplateRgnID))
			{
				ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
				CWorldRegion* pRgn = GetGame().GetGlobalRgnManager()->FindRgnByGUID(guid);
				if(pRgn)
					pRgn->SetOwnerGUID(ownerID);
			}
		}
	}
}


//!					���鸱��GUID����
VOID CGlobalRgnManager::RgnGuidCode(const CGUID& ownerID, DBWriteSet& setWriteDB)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite != m_mapTeamRgnGUID.end() )
	{
		setWriteDB.AddToByteArray((LONG)ite->second.size());
		map<LONG, CGUID>::iterator itr = ite->second.begin();
		for (; itr != ite->second.end(); ++itr)
		{
			setWriteDB.AddToByteArray(itr->first);
			setWriteDB.AddToByteArray(itr->second);
		}
	}
}
//!					���鸱��GUID����
VOID CGlobalRgnManager::RgnGuidDecode(const CGUID& ownerID, DBReadSet& setReadDB)
{
	map<LONG, CGUID> mapRegionID;
	LONG lNum = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lNum; ++i)
	{
		LONG lTemplateRgnID = setReadDB.GetLongFromByteArray();
		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);
		mapRegionID.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
	}

	m_mapTeamRgnGUID[ownerID] = mapRegionID;
}
//!					���뽻�����鸱������������ͬģ��ĸ�����
VOID CGlobalRgnManager::RgnGuidSwapFromArray(const CGUID& ownerID, DBReadSet& setReadDB)
{
	map<CGUID, map<LONG, CGUID>>::iterator ite = m_mapTeamRgnGUID.find(ownerID);
	if( ite == m_mapTeamRgnGUID.end() )
	{
		m_mapTeamRgnGUID.insert(pair<CGUID, map<LONG, CGUID>>(ownerID, map<LONG, CGUID>()));
		ite = m_mapTeamRgnGUID.find(ownerID);
	}

	LONG lNum = setReadDB.GetLongFromByteArray();
	for (LONG i = 0; i < lNum; ++i)
	{
		LONG lTemplateRgnID = setReadDB.GetLongFromByteArray();
		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);
		if(ite->second.end() == ite->second.find(lTemplateRgnID))
			ite->second.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
	}
}
//!					�õ���ҵĸ��˸���GUID�б�
map<CGUID, CGUID>&	CGlobalRgnManager::GetPersonalRgnGuidMap(const CGUID& ownerGuid)
{
	return m_mapPersonalRgnGUID;
}
void CGlobalRgnManager::AddPersonalRgnGUID(const CGUID& ownerGuid, const CGUID& rgnGuid)
{
	m_mapPersonalRgnGUID[ownerGuid] = rgnGuid;
}
//! ɾ��һ����Ҹ��˸���GUID
void CGlobalRgnManager::DelPersonalRgnGUID(const CGUID& ownerGuid)
{
	m_mapPersonalRgnGUID.erase(ownerGuid);
}
//!					�õ�һ�����˸�����GUID
const CGUID& CGlobalRgnManager::GetPersonalRgnGUID(const CGUID& ownerGuid)
{
	map<CGUID, CGUID>::iterator itr = m_mapPersonalRgnGUID.find(ownerGuid);
	if(itr != m_mapPersonalRgnGUID.end())
		return itr->second;
	return NULL_GUID;
}