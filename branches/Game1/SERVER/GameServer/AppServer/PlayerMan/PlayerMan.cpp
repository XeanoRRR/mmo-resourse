// Fox@20081104----------------------------------------------
// File:    PlayerMan.h
// Brief:   CPlayerMan���ڹ������λ��, �Ա�, �ȼ�, ������
//			��ϵӳ��, Ŀǰ��Ҫ�������������������.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.04, 2008
// Fox@20081104----------------------------------------------

#include "StdAfx.h"
#include "PlayerMan.h"
#include "..\Player.h"
#include "..\..\Public\ConfigParse.h"
#include "..\..\nets\netserver\message.h"
#include "..\..\..\setup\gmlist.h"

CPlayerMan::CPlayerMan(void)
: m_lMaxPlayerNum(1024)
, m_lAreaWidth(15)
, m_lPageLength(8)
, m_lLevelLength(10)
{
}

CPlayerMan::~CPlayerMan(void)
{
}

// ��Ϸ�˳�ʱ�ͷ�������ҹ�����Ϣ
void CPlayerMan::Release(void)
{
	// �ͷ��ѷ�����Ҽ���
	ITR_MAP_PLAYER itr_Map = m_mapPlayers.begin();
	for( ; itr_Map!=m_mapPlayers.end(); ++itr_Map )
	{
		MP_DELETE(itr_Map->second);
	}

	PutoutLog("PM", LT_DEFAULT, "��ҹ��������ͷ�.");
}

bool CPlayerMan::LoadConfig(void)
{
	CConfigParse config("Data/PMConfig.xml");
	if( config.IsLoaded() )
	{
		int nVal;
		config.SetCursor(1, "MaxPlayerNum");
		config.GetAttr("val", &nVal, 1024);
		m_lMaxPlayerNum = nVal;
		config.SetNextSibling();
		config.GetAttr("val", &nVal, 15);
		m_lAreaWidth = nVal;
		config.SetNextSibling();
		config.GetAttr("val", &nVal, 8);
		m_lPageLength = nVal;
		config.SetNextSibling();
		config.GetAttr("val", &nVal, 10);
		m_lLevelLength = nVal;
	}

	return true;
}

// ����
bool CPlayerMan::CodeToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(m_lMaxPlayerNum);
	setWriteDB.AddToByteArray(m_lAreaWidth);
	setWriteDB.AddToByteArray(m_lPageLength);
	setWriteDB.AddToByteArray(m_lLevelLength);

	return true;
}

// ����
bool CPlayerMan::DecodeFromDataBlock(DBReadSet& setReadDB)
{
	m_lMaxPlayerNum		= setReadDB.GetLongFromByteArray();
	m_lAreaWidth		= setReadDB.GetLongFromByteArray();
	m_lPageLength		= setReadDB.GetLongFromByteArray();
	m_lLevelLength		= setReadDB.GetLongFromByteArray();

	return true;
}

// �������
tagPlayerInfo* CPlayerMan::FindPlayer(const CGUID& rGuid)
{
	ITR_MAP_PLAYER itr = m_mapPlayers.find(rGuid);
	if( itr == m_mapPlayers.end() )
	{
		return NULL;
	}

	return itr->second;
}

// ������
bool CPlayerMan::AddPlayer(CPlayer* pPlayer)
{
	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL != pInfo )
	{
		DelPlayerMap(pInfo, false);
	}
	else
	{
		pInfo = MP_NEW tagPlayerInfo;
	}

	if( NULL == pInfo )
	{
		char strLog[64];
		_snprintf(strLog, 64, "���(%s)��ϢΪNULL.",
			pPlayer->GetName());
		PutoutLog("PM", LT_ERROR, strLog);

		return false;
	}

	FillPlayerInfo(pPlayer, *pInfo);
	m_mapPlayers[pInfo->ExID] = pInfo;

	AddPlayerMap(pInfo);		// ��������ӳ����Ϣ
	return true;
}

// �ͷ����
bool CPlayerMan::DelPlayer(CPlayer* pPlayer)
{
	const CGUID& rGuid = pPlayer->GetExID();
	tagPlayerInfo* pInfo = FindPlayer(rGuid);
	if( NULL == pInfo )
	{
		char strLog[64];
		char strName[64];
		rGuid.tostring(strName);
		_snprintf(strLog, 64, "���(%s %s)��ϢΪNULL.",
			pPlayer->GetName(), strName);
		PutoutLog("PM", LT_ERROR, strLog);

		return false;
	}

	return DelPlayerMap(pInfo, true);
}

// �޸���ҵ�����Ϣ
long CPlayerMan::RefreshElem(long eType,
							 CPlayer* pPlayer)
{
	if(NULL == pPlayer)	return 0;
	char strLog[512];
	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		return 0;
	}

	CGUID& ExID = pInfo->ExID;
	long lOldVal = -1;
	long lNewVal = -1;
	switch( eType )
	{
	case PET_NAME:
		{
			pInfo->strName = pPlayer->GetName();
		}
		break;

	case PET_FACTION:
		{
			pInfo->strFaction = pPlayer->GetFactionName();
		}
		break;

	case PET_UNION:
		{
			pInfo->strUnion = pPlayer->GetUnionName();
		}
		break;

	case PET_OCCUPATION:
		{
			lNewVal = pPlayer->GetOccupation();
			pInfo->lOccupation = lNewVal;
		}
		break;

	case PET_SEX:
		{
			lOldVal = pInfo->lSex;
			lNewVal = pPlayer->GetSex();
			pInfo->lSex = lNewVal;
			m_Sex2Players[lOldVal].remove(ExID);
			m_Sex2Players[lNewVal].push_back(ExID);
		}
		break;

	case PET_LEVEL:
		{
			lOldVal = CalcLevel(pInfo->lLevel);
			lNewVal = CalcLevel(pPlayer->GetLevel());
			pInfo->lLevel = pPlayer->GetLevel();
			if( lOldVal != lNewVal )
			{
				m_Level2Players[lOldVal].remove(ExID);
				m_Level2Players[lNewVal].push_back(ExID);
			}
			lNewVal = pInfo->lLevel;
		}
		break;

	case PET_COUNTRY:
		{
			lOldVal = pInfo->lCountryID;
			lNewVal = pPlayer->GetCountry();
			pInfo->lCountryID = lNewVal;
			m_Country2Players[lOldVal].remove(ExID);
			m_Country2Players[lNewVal].push_back(ExID);
		}
		break;

	case PEI_CONST:
		{
			lOldVal = pInfo->lConst;
			lNewVal = (long) pPlayer->GetConst();
			pInfo->lConst = lNewVal;
			m_Const2Players[lOldVal].remove( ExID );
			m_Const2Players[lNewVal].push_back( ExID );
		}
		break;

	case PET_REGION:
		{
			lOldVal = pInfo->lAreaID;
			pInfo->Position.lPosX = pPlayer->GetTileX();
			pInfo->Position.lPosY = pPlayer->GetTileY();
			lNewVal = CalcAreaID(pInfo->Position, pPlayer->GetRegionID());
			m_Area2Players[lOldVal].remove(ExID);
			pInfo->lAreaID = lNewVal;
			m_Area2Players[lNewVal].push_back(ExID);

			lOldVal = pInfo->lRegionID;
			lNewVal = pPlayer->GetRegionID();
			pInfo->lRegionID = lNewVal;
			m_Region2Players[lOldVal].remove(ExID);
			m_Region2Players[lNewVal].push_back(ExID);
		}
		break;

	case PET_AREA:
		{
			lOldVal = pInfo->lAreaID;
			pInfo->Position.lPosX = pPlayer->GetTileX();
			pInfo->Position.lPosY = pPlayer->GetTileY();
			lNewVal = CalcAreaID(pInfo->Position, pPlayer->GetRegionID());
			if( lOldVal != lNewVal )
			{
				m_Area2Players[lOldVal].remove(ExID);
				pInfo->lAreaID = lNewVal;
				m_Area2Players[lNewVal].push_back(ExID);
			}
		}
		break;

	case PET_HIDENAME:
		{
			lOldVal = pInfo->btHideName;
			lNewVal = pPlayer->GetHideFlag();
			pInfo->btHideName = lNewVal;
		}
		break;

	default:
		{
			_snprintf(strLog, 512, "TYPE:0X%X, ���%s PLAYER_ELEM_TYPE�Ƿ�.",
				eType,
				pPlayer->GetName());
			PutoutLog("PM", LT_ERROR, strLog);
			return -1;
		}
	}

	if( !(eType & PET_STR_DESC) &&
		( eType != PET_AREA ) &&
		( eType != PET_HIDENAME ) )
	{
		CMessage msg(MSG_S2W_PM_CHG);
		msg.Add(pPlayer->GetExID());
		msg.Add(eType);
		msg.Add(lNewVal);
		msg.Send();
	}

	return lNewVal;
}

// �޸���Ҷ�����Ϣ
bool CPlayerMan::RefreshElems(long eTypes,
							  CPlayer* pPlayer)
{
	if( NULL == pPlayer )
	{
		PutoutLog("PM", LT_ERROR, "NULL == pPlayer.");
		return false;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		char strLog[64];
		_snprintf(strLog, 64, "TYPES(0X%x) : ���(%s)��ϢΪNULL.",
			eTypes, pPlayer->GetName());
		PutoutLog("PM", LT_ERROR, strLog);
		return false;
	}

	CGUID& ExID = pInfo->ExID;
	long lOldVal = -1;
	long lNewVal = -1;
	if( eTypes & PET_NAME )
	{
		pInfo->strName = pPlayer->GetName();
	}

	if( eTypes & PET_FACTION )
	{
		pInfo->strFaction = pPlayer->GetFactionName();
	}

	if( eTypes & PET_UNION )
	{
		pInfo->strUnion = pPlayer->GetUnionName();
	}

	if( eTypes & PET_OCCUPATION )
	{
		lNewVal = pPlayer->GetOccupation();
		pInfo->lOccupation = lNewVal;
	}

	if( eTypes & PET_SEX )
	{
		lOldVal = pInfo->lSex;
		lNewVal = pPlayer->GetSex();
		pInfo->lSex = lNewVal;
		m_Sex2Players[lOldVal].remove(ExID);
		m_Sex2Players[lNewVal].push_back(ExID);
	}

	if( eTypes & PET_LEVEL )
	{
		lOldVal = CalcLevel(pInfo->lLevel);
		lNewVal = CalcLevel(pPlayer->GetLevel());
		pInfo->lLevel = pPlayer->GetLevel();
		if( lOldVal != lNewVal )
		{
			m_Level2Players[lOldVal].remove(ExID);
			m_Level2Players[lNewVal].push_back(ExID);
		}
	}

	if( eTypes & PET_COUNTRY )
	{
		lOldVal = pInfo->lCountryID;
		lNewVal = pPlayer->GetCountry();
		pInfo->lCountryID = lNewVal;
		m_Country2Players[lOldVal].remove(ExID);
		m_Country2Players[lNewVal].push_back(ExID);
	}

	if( eTypes & PEI_CONST )
	{
		lOldVal = pInfo->lConst;
		lNewVal = pPlayer->GetConst();
		pInfo->lConst = lNewVal;
		m_Const2Players[lOldVal].remove( ExID );
		m_Const2Players[lNewVal].push_back( ExID );
	}

	if( eTypes & PET_REGION )
	{
		lOldVal = pInfo->lRegionID;
		lNewVal = pPlayer->GetRegionID();
		m_Region2Players[lOldVal].remove(ExID);
		pInfo->lRegionID = lNewVal;
		m_Region2Players[lNewVal].push_back(ExID);
	}

	if( eTypes & PET_AREA )
	{
		lOldVal = pInfo->lAreaID;
		pInfo->Position.lPosX = pPlayer->GetTileX();
		pInfo->Position.lPosY = pPlayer->GetTileY();
		long lNewVal = CalcAreaID(pInfo->Position, pPlayer->GetRegionID());
		if( lOldVal != lNewVal )
		{
			m_Area2Players[lOldVal].remove(ExID);
			pInfo->lAreaID = lNewVal;
			m_Area2Players[lNewVal].push_back(ExID);
		}
	}

	if( eTypes & PET_HIDENAME )
	{
		lNewVal = pPlayer->GetHideFlag();
		pInfo->btHideName = lNewVal;
	}

	return true;
}

// ˢ�����
bool CPlayerMan::RefreshPlayer(CPlayer* pPlayer, bool bAll)
{
	if( NULL == pPlayer )
	{
		PutoutLog("PM", LT_ERROR, "NULL == pPlayer.");
		return false;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		char strLog[64];
		_snprintf(strLog, 64, "���(%s)��ϢΪNULL.",
			pPlayer->GetName());
		PutoutLog("PM", LT_ERROR, strLog);
		return false;
	}

	// ��������ӳ����Ϣ
	CGUID& ExID = pInfo->ExID;

	// ����
	long lOldVal = pInfo->lAreaID;
	pInfo->Position.lPosX = pPlayer->GetTileX();
	pInfo->Position.lPosY = pPlayer->GetTileY();
	long lNewVal = CalcAreaID(pInfo->Position, pPlayer->GetRegionID());
	if( lOldVal != lNewVal )
	{
		m_Area2Players[lOldVal].remove(ExID);
		pInfo->lAreaID = lNewVal;
		m_Area2Players[lNewVal].push_back(ExID);
	}

	if( bAll )
	{
		// ����
		string strOldVal = pInfo->strName;
		string strNewVal = pPlayer->GetName();
		if( strOldVal != strNewVal )
		{
			pInfo->strName = strNewVal;
		}

		// ���
		strOldVal = pInfo->strFaction.c_str();
		strNewVal = pPlayer->GetFactionName();
		if( strOldVal != strNewVal )
		{
			pInfo->strFaction = strNewVal;
		}

		// ����
		strOldVal = pInfo->strUnion.c_str();
		strNewVal = pPlayer->GetUnionName();
		if( strOldVal != strNewVal )
		{
			pInfo->strUnion = strNewVal;
		}

		// ְҵ
		lOldVal = pInfo->lOccupation;
		lNewVal = pPlayer->GetOccupation();
		if( lOldVal != lNewVal )
		{
			pInfo->lOccupation = lNewVal;
		}

		// �Ա�
		lOldVal = pInfo->lSex;
		lNewVal = pPlayer->GetSex();
		if( lOldVal != lNewVal )
		{
			m_Sex2Players[lOldVal].remove(ExID);
			pInfo->lSex = lNewVal;
			m_Sex2Players[lNewVal].push_back(ExID);
		}

		// �ȼ�
		lOldVal = CalcLevel(pInfo->lLevel);
		lNewVal = CalcLevel(pPlayer->GetLevel());
		pInfo->lLevel = pPlayer->GetLevel();
		if( lOldVal != lNewVal )
		{
			m_Level2Players[lOldVal].remove(ExID);
			m_Level2Players[lNewVal].push_back(ExID);
		}

		// ����
		lOldVal = pInfo->lCountryID;
		lNewVal = pPlayer->GetCountry();
		if( lOldVal != lNewVal )
		{
			m_Country2Players[lOldVal].remove(ExID);
			pInfo->lCountryID = lNewVal;
			m_Country2Players[lNewVal].push_back(ExID);
		}

		// ����
		lOldVal = pInfo->lConst;
		lNewVal = pPlayer->GetConst();
		if( lOldVal != lNewVal )
		{
			m_Const2Players[lOldVal].remove( ExID );
			pInfo->lConst = lNewVal;
			m_Const2Players[lNewVal].push_back( ExID );
		}

		// ����
		lOldVal = pInfo->lRegionID;
		lNewVal = pPlayer->GetRegionID();
		if( lOldVal != lNewVal )
		{
			m_Region2Players[lOldVal].remove(ExID);
			pInfo->lRegionID = lNewVal;
			m_Region2Players[lNewVal].push_back(ExID);
		}

		// ����
		lOldVal = pInfo->btHideName;
		lNewVal = pPlayer->GetHideFlag();
		if( lOldVal != lNewVal )
		{
			pInfo->btHideName = lNewVal;
		}
	}

	return true;
}

// GS�л�
bool CPlayerMan::ChangeGS(const CGUID& rGuid)
{
	tagPlayerInfo* pInfo = FindPlayer(rGuid);
	if( NULL == pInfo )
	{
		char strLog[64];
		char strName[64];
		rGuid.tostring(strName);
		_snprintf(strLog, 64, "���(%s)��ϢΪNULL.",
			strName);
		PutoutLog("PM", LT_ERROR, strLog);
		return false;
	}

	m_Region2Players[pInfo->lRegionID].remove(rGuid);

	return true;
}

// �ͻ��˲�ѯ
bool CPlayerMan::Query(CMessage* pMsg)
{
	char strLogTemp[256];
	assert(NULL != pMsg);
	long lMsgType = pMsg->GetType();
	CPlayer* pPlayer = pMsg->GetPlayer();
	if( NULL == pPlayer )
	{
		PutoutLog("PM", LT_ERROR, "NULL == pPlayer.");
		return false;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		_snprintf(strLogTemp, 256,
			"ͨ���쳣: Player(%s)��ϢΪNULL, MsgType : %d",
			pPlayer->GetName(),
			lMsgType);
		PutoutLog("Msg", LT_ERROR, strLogTemp);
		return false;
	}

	if ( pPlayer->GetLevel() < static_cast<unsigned short>(CGlobeSetup::GetSetup()->lMLevelEnablePlayerSearch) )
	{
		char strLog[256];
		char szGUID[128];
		pPlayer->GetExID().tostring(szGUID);
		_snprintf(strLog, 256, "�����Ϣ�Ƿ�����: ���:'%s'[ GUID:%s �ȼ�:%d ],��ȼ�̫�������������Ϣ���� ",pPlayer->GetName(), szGUID, pPlayer->GetLevel());
		PutoutLog("PM",LT_ERROR,strLog);
		return false;
	}

	long lVal = pMsg->GetLong();
	if( PRT_NULL == lVal )
	{
		pMsg->SetType(MSG_S2W_PM_REQ);
		pMsg->Add(pPlayer->GetExID());
		pMsg->Send();
		return true;
	}

	ITR_MAP_PLAYER_MAP itrPlayers;
	LIST_GUID PlayerList;

	if( PRT_CUR_REGION == lVal )			// ��ǰ����
	{
		lVal = pInfo->lRegionID;
		itrPlayers = m_Region2Players.find(lVal);
		if( m_Region2Players.end() == itrPlayers )
		{
			_snprintf(strLogTemp, 256,
				"�����쳣: ����(%d)û�����.",
				lVal);
			PutoutLog("Msg", LT_ERROR, strLogTemp);
			return false;
		}

		PlayerList = itrPlayers->second;
	}
	else if( PRT_CUR_AREA == lVal )				// �ٽ�����
	{
		ITR_MAP_PLAYER_MAP itrTemp;
		LIST_GUID listTemp;
		lVal = pInfo->lAreaID;
		long lTemp;
		for( long i=-1; i<2; ++i )
		{
			for( long j=-1; j<2; ++j )
			{
				lTemp = lVal+(i*32)+j;
				itrTemp = m_Area2Players.find(lTemp);
				if( m_Area2Players.end() == itrTemp )
				{
#ifdef _DEBUG
					_snprintf(strLogTemp, 256,
						"�����쳣: ����(%d)û�����.",
						lTemp);
					PutoutLog("Msg", LT_ERROR, strLogTemp);
#endif
				}
				else
				{
					listTemp = itrTemp->second;
					PlayerList.insert(PlayerList.end(), listTemp.begin(), listTemp.end());
				}
			}
		}
	}

	PlayerList.remove(pPlayer->GetExID());

	ITR_LIST_GUID itrGUID = PlayerList.begin();
	long lLevel = pMsg->GetLong();
	long lCountryID = pMsg->GetLong();
	long lSex = pMsg->GetLong();
	long lConst = pMsg->GetLong();
	ITR_MAP_PLAYER itrPlayer;
	while( itrGUID!=PlayerList.end() )
	{
		itrPlayer = m_mapPlayers.find(*itrGUID);
		if( itrPlayer == m_mapPlayers.end() ||
			NULL == (pInfo = itrPlayer->second ) ||
			( lLevel != -1 && CalcLevel(pInfo->lLevel) != lLevel ) ||
			( lCountryID != -1 && !CheckCountry(pInfo->lCountryID, lCountryID) ) ||
			( lSex != -1 && pInfo->lSex != lSex ) ||
			( lConst != -1 && pInfo->lConst != lConst ) ||
			CGMList::GetInfoByName( pInfo->strName.c_str() ) )
		{
			itrGUID = PlayerList.erase(itrGUID);
		}
		else
		{
			++itrGUID;
		}
	}

	long lTotal = PlayerList.size();
	long lDiff = lTotal - m_lPageLength;
	LIST_GUID Res;
	if( lDiff > 0 )
	{
		std::set<int> RnSet;
		int nRn = 0;
		while( RnSet.size() < m_lPageLength )
		{
			nRn = random((int)lTotal);
			RnSet.insert(nRn);
		}

		itrGUID = PlayerList.begin();
		std::set<int>::iterator itrRnSet = RnSet.begin();
		for( int i=0;
			itrRnSet!=RnSet.end() && itrGUID!=PlayerList.end();
			++i, ++itrGUID )
		{
			if( *itrRnSet == i )
			{
				Res.push_back(*itrGUID);
				++itrRnSet;
			}
		}
	}
	else
	{
		Res = PlayerList;
	}

	CMessage msg(MSG_S2C_PM_ACK);
	msg.Add(pPlayer->GetExID());
	msg.Add( (long)Res.size() );
	for( itrGUID=Res.begin(); itrGUID!=Res.end(); ++itrGUID )
	{
		itrPlayer = m_mapPlayers.find( *itrGUID );
		if( itrPlayer != m_mapPlayers.end() )
		{
			pInfo = itrPlayer->second;
		}
		else
		{
			// won't reach here
			static tagPlayerInfo __error_info;
			pInfo = &__error_info;
		}

		// �滻������Ϣ
		if(pInfo->btHideName == 1)
			msg.Add(CGlobeSetup::GetSetup()->szHideName);
		else
			msg.Add( pInfo->strName.c_str() );

		msg.Add( pInfo->strFaction.c_str() );
		msg.Add( pInfo->strUnion.c_str() );
		msg.Add( pInfo->lOccupation );
		msg.Add( pInfo->lSex );
		msg.Add( pInfo->lLevel );
		msg.Add( pInfo->lCountryID );
		msg.Add( pInfo->lRegionID );
		msg.Add( pInfo->lConst );
	}

	msg.SendToPlayer(pPlayer->GetExID());

	return true;
}

// Fox@20081107----------------------------------------------
// ��ҹ���, �����ڲ�ʹ��
// ����CPlayer�������PlayerInfo
bool CPlayerMan::FillPlayerInfo(CPlayer* pPlayer, tagPlayerInfo& rPlayerInfo)
{
	if( NULL == pPlayer )
	{
		PutoutLog("PM", LT_ERROR, "���ΪNULL.");
		return false;
	}

	rPlayerInfo.ExID		= pPlayer->GetExID();
	rPlayerInfo.strName		= pPlayer->GetName();
	rPlayerInfo.strFaction	= pPlayer->GetFactionName();
	rPlayerInfo.strUnion	= pPlayer->GetUnionName();
	rPlayerInfo.lOccupation	= pPlayer->GetOccupation();
	rPlayerInfo.lSex		= pPlayer->GetSex();
	rPlayerInfo.lLevel		= pPlayer->GetLevel();
	rPlayerInfo.lCountryID	= pPlayer->GetCountry();
	rPlayerInfo.lConst		= pPlayer->GetConst();
	rPlayerInfo.lRegionID	= pPlayer->GetRegionID();
	rPlayerInfo.btHideName	= pPlayer->GetHideFlag();
	rPlayerInfo.Position.lPosX		= pPlayer->GetTileX();
	rPlayerInfo.Position.lPosY		= pPlayer->GetTileY();
	rPlayerInfo.lAreaID = CalcAreaID(rPlayerInfo.Position, rPlayerInfo.lRegionID);

	return true;
}

// ����CPlayer�������ӳ��
bool CPlayerMan::AddPlayerMap(tagPlayerInfo* pInfo)
{
	const CGUID& rGuid = pInfo->ExID;
	m_Sex2Players[pInfo->lSex].push_back(rGuid);
	m_Level2Players[CalcLevel(pInfo->lLevel)].push_back(rGuid);
	m_Country2Players[pInfo->lCountryID].push_back(rGuid);
	m_Const2Players[pInfo->lConst].push_back( rGuid );
	m_Region2Players[pInfo->lRegionID].push_back(rGuid);
	m_Area2Players[pInfo->lAreaID].push_back(rGuid);

	return true;
}

// ����CPlayer��������ӳ��
bool CPlayerMan::DelPlayerMap(tagPlayerInfo* pInfo, bool bComplete)
{
	if( NULL == pInfo )
	{
		PutoutLog("PM", LT_ERROR, "���(��Ϣ)ΪNULL.");
		return false;
	}

#ifdef _DEBUG
	char strLogTemp[512];
	_snprintf(strLogTemp, 512,
		"ɾ����ҳɹ�: %s, %d, %d, %d, %d: %d, %d. %d.",
		pInfo->strName.c_str(), pInfo->lSex, pInfo->lLevel,
		pInfo->lCountryID, pInfo->lRegionID,
		pInfo->Position.lPosX, pInfo->Position.lPosY,
		pInfo->lAreaID);
	PutoutPMLog(strLogTemp);
#endif

	CGUID ExID = pInfo->ExID;
	m_Sex2Players[pInfo->lSex].remove(ExID);
	m_Level2Players[CalcLevel(pInfo->lLevel)].remove(ExID);
	m_Country2Players[pInfo->lCountryID].remove(ExID);
	m_Const2Players[pInfo->lConst].remove( ExID );
	m_Region2Players[pInfo->lRegionID].remove(ExID);
	m_Area2Players[pInfo->lAreaID].remove(ExID);

	if( bComplete )			// ����ɾ��
	{
		pInfo->ExID = ExID;
		MP_DELETE(pInfo);
		m_mapPlayers.erase(ExID);
	}

	return true;
}

// ����AreaID
long CPlayerMan::CalcAreaID(tagPosition& rPos, long lRegionID)
{
	long x = rPos.lPosX / m_lAreaWidth;
	long y = rPos.lPosY / m_lAreaWidth;
	long lNewVal	= (y << 5) + x | lRegionID << 10;
	return lNewVal;
}

// ����AreaID
long CPlayerMan::CalcLevel(long lLevel)
{
	return (lLevel - 1) / 10;
}

// ���ұȽ� : ������֤��ҹ����Ƿ��ڲ���֮��
// lIDΪ����֤��ҹ���(1-6)
// lIDsΪ���ҹ���(0x01,0x02,0x04,0x08,0x10,0x20)λ���
bool CPlayerMan::CheckCountry(long lID, long lIDs)
{
	if( (lIDs>>(lID-1))&0x01 )
		return true;
	return false;
}
