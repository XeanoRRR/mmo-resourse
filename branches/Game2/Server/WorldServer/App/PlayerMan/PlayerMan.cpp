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
#include "../Player.h"
#include "../../Net/Message.h"

#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPlayerMan::CPlayerMan(void)
: m_lMaxPlayerNum(1024)
, m_lAreaWidth(15)
, m_lPageLength(8)
, m_lLevelLength(10)
, m_lGSNum(6)
{
}

CPlayerMan::~CPlayerMan(void)
{
}
// Fox@20081120----------------------------------------------
// ��ҹ���, ���ⲿ����
// ������ҹ�����
void CPlayerMan::Initial(void)
{
    m_lGSNum = CMessage::GetGSNum();
	tagPlayerInfo* pInfo = NULL;
	for( long i=0; i<m_lMaxPlayerNum * m_lGSNum; ++i )
	{
		pInfo = new tagPlayerInfo;
		m_PrePlayers.push_back(pInfo);
	}
	Log4c::Trace(ROOT_MODULE, "��ҹ������ѳ�ʼ��.");
}

// �ͷ���ҹ�����
void CPlayerMan::Release(void)
{
	// �ͷ�Ԥ������Ҽ���
	ITR_SET_PLAYER itr_Set = m_PrePlayers.begin();
	for(; itr_Set!=m_PrePlayers.end(); ++itr_Set )
	{
		DELETE_SAFE(*itr_Set);
	}

	// �ͷ��ѷ�����Ҽ���
	ITR_MAP_PLAYER itr_Map = m_mapPlayers.begin();
	for( ; itr_Map!=m_mapPlayers.end(); ++itr_Map )
	{
		DELETE_SAFE(itr_Map->second);
	}

	Log4c::Trace(ROOT_MODULE, "��ҹ��������ͷ�.");
}

bool CPlayerMan::LoadConfig(void)
{
	ConfigParse config("Data/PMConfig.xml");
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
		pInfo = AllocPlayer();
	}

	if( NULL == pInfo )
	{
		Log4c::Error(ROOT_MODULE,"%-15s ���(%s)��ϢΪNULL.",__FUNCTION__, pPlayer->GetName());

		FreePlayer(pInfo);
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
		char strName[64];
		rGuid.tostring(strName);

        Log4c::Error(ROOT_MODULE,"%-15s ���(%s %s)��ϢΪNULL.",__FUNCTION__, pPlayer->GetName(),strName);

		return false;
	}

	return DelPlayerMap(pInfo, true);
}

// �޸���ҵ�����Ϣ : �ַ����Ƚ��鷳, ��Ϊ��ָ���������, ��������ܸ�ֵ.
// �ѵ���������ڼ䲻��������, ���, ����?
long CPlayerMan::RefreshElem(long eType,
							 CPlayer* pPlayer)
{
	if( NULL == pPlayer )
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "NULL == pPlayer.");
		return -1;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		if( AddPlayer(pPlayer) )
		{
            Log4c::Error(ROOT_MODULE,"%-15s ����������(%s)��Ϣ.",__FUNCTION__,pPlayer->GetName());
			return 0;
		}
        Log4c::Error(ROOT_MODULE,"%-15s TYPE(0X%x) : ���(%s)��ϢΪNULL",__FUNCTION__,eType, pPlayer->GetName());
		return -1;
	}

	CGUID& ExID = pInfo->ExID;
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

	default:
		{
			Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "PLAYER_ELEM_TYPE�Ƿ�.");
			return -1;
		}
	}
	return 0;
}

// �޸���ҵ�����Ϣ
long CPlayerMan::RefreshElem(long eType,
							 CPlayer* pPlayer,
							 long lNewVal)
{
	//char strLog[512];
	if( NULL == pPlayer )
	{
        Log4c::Error(ROOT_MODULE,"%-15s TYPE:0X%X(%ld), NULL == pPlayer.",__FUNCTION__,eType,lNewVal);
		return -1;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		if( AddPlayer(pPlayer) )
		{
			//sprintf(strLog, "TYPE:0X%X(%ld), ����������(%s)��Ϣ.",
			//	eType,
			//	lNewVal,
			//	pPlayer->GetName());
			//PutoutLog("PM", LT_ERROR, strLog);

            Log4c::Error(ROOT_MODULE,"%-15s TYPE:0X%X(%ld), ����������(%s)��Ϣ.",__FUNCTION__,eType,lNewVal,pPlayer->GetName());
			return 0;
		}

		/*sprintf(strLog, "TYPE:0X%X(%ld), ���(%s)��ϢΪNULL.",
			eType,
			lNewVal,
			pPlayer->GetName());
		PutoutLog("PM", LT_ERROR, strLog);*/

        Log4c::Error(ROOT_MODULE,"%-15s TYPE:0X%X(%ld), ���(%s)��ϢΪNULL.",__FUNCTION__,eType,lNewVal,pPlayer->GetName());
		return -1;
	}

	CGUID& ExID = pInfo->ExID;
	long lOldVal = -1;
	switch( eType )
	{
	case PET_OCCUPATION:
		{
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
			pInfo->lLevel = lNewVal;
			lNewVal = CalcLevel(pInfo->lLevel);
			if( lOldVal != lNewVal )
			{
				m_Level2Players[lOldVal].remove(ExID);
				m_Level2Players[lNewVal].push_back(ExID);
			}
		}
		break;

	case PET_COUNTRY:
		{
			lOldVal = pInfo->lCountryID;
			pInfo->lCountryID = lNewVal;
			m_Country2Players[lOldVal].remove(ExID);
			m_Country2Players[lNewVal].push_back(ExID);
		}
		break;

	case PEI_CONST:
		{
			lOldVal = pInfo->lConst;
			pInfo->lConst = lNewVal;
			m_Const2Players[lOldVal].remove( ExID );
			m_Const2Players[lNewVal].push_back( ExID );
		}
		break;

	case PET_REGION:
		{
			lOldVal = pInfo->lRegionID;
			m_Region2Players[lOldVal].remove(ExID);
			pInfo->lRegionID = lNewVal;
			m_Region2Players[lNewVal].push_back(ExID);
		}
		break;

	case PET_AREA:
		{
			lOldVal = pInfo->lAreaID;
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
			pInfo->btHideName = (BYTE)lNewVal;
		}
		break;

	default:
		{
			/*sprintf(strLog, "TYPE:0X%X(%ld), ���%s PLAYER_ELEM_TYPE�Ƿ�.",
				eType,
				lNewVal,
				pPlayer->GetName());
			PutoutLog("PM", LT_ERROR, strLog);*/
            Log4c::Error(ROOT_MODULE,"%-15s TYPE:0X%X(%ld), ���%s PLAYER_ELEM_TYPE�Ƿ�.",__FUNCTION__,eType,lNewVal,pPlayer->GetName());
			return -1;
		}
	}
	return lNewVal;
}

// �޸���Ҷ�����Ϣ
bool CPlayerMan::RefreshElems(long eTypes,
							  CPlayer* pPlayer)
{
	if( NULL == pPlayer )
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "NULL == pPlayer.");
		return false;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		/*char strLog[512];
		sprintf(strLog, "TYPES(0X%x) : ���(%s)��ϢΪNULL.",
			eTypes, pPlayer->GetName());
		PutoutLog("PM", LT_ERROR, strLog);*/

        Log4c::Error(ROOT_MODULE,"%-15s TYPES(0X%x) : ���(%s)��ϢΪNULL.",__FUNCTION__,eTypes, pPlayer->GetName());
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
	//	lNewVal = (long)pPlayer->GetConst();
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
		pInfo->Position.lPosX = static_cast<long>(pPlayer->GetPosX());
		pInfo->Position.lPosY = static_cast<long>(pPlayer->GetPosY());
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
		pInfo->btHideName = (BYTE)lNewVal;
	}

	return true;
}

// ˢ�����
bool CPlayerMan::RefreshPlayer(CPlayer* pPlayer, bool bAll)
{
	if( NULL == pPlayer )
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "NULL == pPlayer.");
		return false;
	}

	tagPlayerInfo* pInfo = FindPlayer(pPlayer->GetExID());
	if( NULL == pInfo )
	{
		//char strLog[512];
		//sprintf(strLog, "���(%s)��ϢΪNULL.",
		//	pPlayer->GetName());
		//PutoutLog("PM", LT_ERROR, strLog);
        Log4c::Error(ROOT_MODULE,"%-15s ���(%s)��ϢΪNULL.",__FUNCTION__,pPlayer->GetName());
		return false;
	}

	// ��������ӳ����Ϣ
	CGUID& ExID = pInfo->ExID;

	// ����
	long lOldVal = pInfo->lAreaID;
	pInfo->Position.lPosX = static_cast<long>(pPlayer->GetPosX());
	pInfo->Position.lPosY = static_cast<long>(pPlayer->GetPosY());
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
		//lOldVal = pInfo->lConst;
		//lNewVal = (long) pPlayer->GetConst();
		//if( lOldVal != lNewVal )
		//{
		//	m_Const2Players[lOldVal].remove( ExID );
		//	pInfo->lConst = lNewVal;
		//	m_Const2Players[lNewVal].push_back( ExID );
		//}

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
			pInfo->btHideName = (BYTE)lNewVal;
		}
	}

	return true;
}

// GS�л�
bool CPlayerMan::ChangeGS(long lNew, const CGUID& rGuid)
{
	tagPlayerInfo* pInfo = FindPlayer(rGuid);
	if( NULL == pInfo )
	{
		//char strLog[512];
		char strName[64];
		rGuid.tostring(strName);
		//sprintf(strLog, "���(%s)��ϢΪNULL.",
		//	strName);
		//PutoutLog("PM", LT_ERROR, strLog);

        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,strName);
		return false;
	}

	return true;
}

// �ͻ��˲�ѯ
bool CPlayerMan::Query(CMessage* pMsg)
{
	//char strLogTemp[1024];
	assert(NULL != pMsg);
	long lMsgType = pMsg->GetType();
	long lVal = pMsg->GetLong();
	long lLevel = pMsg->GetLong();
	long lCountryID = pMsg->GetLong();
	long lSex = pMsg->GetLong();
	long lConst = pMsg->GetLong();
	CGUID ExID;
	pMsg->GetGUID(ExID);
	CPlayer *pPlayer = GetGame().GetMapPlayer(ExID);
	if( NULL == pPlayer )
	{
		/*sprintf(strLogTemp,
			"ͨ���쳣: Player == NULL, MsgType : %d",
			lMsgType);
		PutoutLog("PM", LT_ERROR, strLogTemp);*/

        Log4c::Error(ROOT_MODULE,"%-15s ͨ���쳣: Player == NULL, MsgType : %d",__FUNCTION__,lMsgType);


	}

	ITR_MAP_PLAYER_MAP itrPlayers;
	LIST_GUID PlayerList;
	ITR_LIST_GUID itrGUID;
	ITR_MAP_PLAYER itrPlayer;
	tagPlayerInfo* pInfo = NULL;
	if( lLevel != -1 )
	{
		itrPlayers = m_Level2Players.find(lLevel);
		if( m_Level2Players.end() != itrPlayers )
		{
			PlayerList = itrPlayers->second;
			PlayerList.remove(ExID);
			itrGUID = PlayerList.begin();
			while( itrGUID!=PlayerList.end() )
			{
				itrPlayer = m_mapPlayers.find(*itrGUID);
				if( itrPlayer == m_mapPlayers.end() ||
					NULL == (pInfo = itrPlayer->second ) ||
					( lCountryID != -1 && !CheckCountry(pInfo->lCountryID, lCountryID) ) ||
					( lSex != -1 && pInfo->lSex != lSex ) ||
					( lConst != -1 && pInfo->lConst != lConst ) )
				{
					itrGUID = PlayerList.erase(itrGUID);
				}
				else
				{
					++itrGUID;
				}
			}
		}
		else
		{
#ifdef _DEBUG
			/*sprintf(strLogTemp,
				"�����쳣: �ȼ���(%d)û�����.",
				(lLevel+1)*10);
			PutoutLog("PM", LT_WARNING, strLogTemp);*/
            Log4c::Error(ROOT_MODULE,"%-15s �����쳣: �ȼ���(%d)û�����.",__FUNCTION__,(lLevel+1)*10);
#endif
		}
	}
	else if( lSex != -1 )
	{
		itrPlayers = m_Sex2Players.find(lSex);
		if( m_Sex2Players.end() != itrPlayers )
		{
			PlayerList = itrPlayers->second;
			PlayerList.remove(ExID);
			itrGUID = PlayerList.begin();
			while( itrGUID!=PlayerList.end() )
			{
				itrPlayer = m_mapPlayers.find(*itrGUID);
				if( itrPlayer == m_mapPlayers.end() ||
					NULL == (pInfo = itrPlayer->second ) ||
					( lLevel != -1 && CalcLevel(pInfo->lLevel) != lLevel ) ||
					( lCountryID != -1 && !CheckCountry(pInfo->lCountryID, lCountryID) ) ||
					( lConst != -1 && lConst != pInfo->lConst ) )
				{
					itrGUID = PlayerList.erase(itrGUID);
				}
				else
				{
					++itrGUID;
				}
			}
		}
		else
		{
#ifdef _DEBUG
			/*sprintf(strLogTemp,
				"�����쳣: �Ա�(%d)û�����.",
				lSex);
			PutoutLog("PM", LT_WARNING, strLogTemp);*/

            Log4c::Error(ROOT_MODULE,"%-15s �����쳣: �Ա�(%d)û�����.",__FUNCTION__,lSex);
#endif
		}

	}
	else if( lConst != -1 )
	{
		itrPlayers = m_Const2Players.find( lConst );
		if( m_Const2Players.end() != itrPlayers )
		{
			PlayerList = itrPlayers->second;
			PlayerList.remove( ExID );
			for( itrGUID = PlayerList.begin(); itrGUID != PlayerList.end(); )
			{
				itrPlayer = m_mapPlayers.find( *itrGUID );
				if( itrPlayer == m_mapPlayers.end() ||
					NULL == ( pInfo = itrPlayer->second ) ||
					( lLevel != -1 && CalcLevel( pInfo->lLevel ) != lLevel ) ||
					( lCountryID != -1 && !CheckCountry(pInfo->lCountryID, lCountryID) ) ||
					( lSex != -1 && pInfo->lSex != lSex ) )
				{
					itrGUID = PlayerList.erase( itrGUID ); 
				}
				else
				{
					++ itrGUID;
				}
			}
		}
		else
		{
#ifdef _DEBUG
			/*sprintf( strLogTemp, 
				"�����쳣������(%d)û�����.",
				lConst );
			PutoutLog( "PM", LT_WARNING, strLogTemp );*/
            Log4c::Error(ROOT_MODULE,"%-15s �����쳣������(%d)û�����.",__FUNCTION__,lConst );
#endif
		}
	}
	else
	{
		ITR_MAP_PLAYER_MAP itrTemp;
		LIST_GUID listTemp;
		for( long lID=COUNTRY_STAR; lID < COUNTRY_UPPER; ++lID )
		{
			if( CheckCountry(lID, lCountryID) )
			{
				itrTemp = m_Country2Players.find(lID);
				if( m_Country2Players.end() == itrTemp )
				{
#ifdef _DEBUG
					/*sprintf(strLogTemp,
						"�����쳣: ����(%d)û�����.",
						lID);
					PutoutLog("PM", LT_WARNING, strLogTemp);*/

                    Log4c::Error(ROOT_MODULE,"%-15s �����쳣: ����(%d)û�����.",__FUNCTION__,lID);
#endif
				}
				else
				{
					listTemp = itrTemp->second;
					PlayerList.insert(PlayerList.end(), listTemp.begin(), listTemp.end());
				}
			}
		}

		PlayerList.remove(ExID);
		itrGUID = PlayerList.begin();
		while( itrGUID!=PlayerList.end() )
		{
			itrPlayer = m_mapPlayers.find(*itrGUID);
			if( itrPlayer == m_mapPlayers.end() ||
				NULL == (pInfo = itrPlayer->second ) ||
				( lLevel != -1 && CalcLevel(pInfo->lLevel) != lLevel ) ||
				( lSex != -1 && pInfo->lSex != lSex ) ||
				( lConst != -1 && pInfo->lConst != lConst ) )
			{
				itrGUID = PlayerList.erase(itrGUID);
			}
			else
			{
				++itrGUID;
			}
		}
	}

	long lTotal = (long)PlayerList.size();
	long lDiff = lTotal - m_lPageLength;
	LIST_GUID Res;
	if( lDiff > 0 )
	{
		std::set<int> RnSet;
		int nRn = 0;
		while( (long)RnSet.size() < m_lPageLength )
		{
			nRn = random((long)lTotal);
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

	CMessage msg(MSG_W2S_PM_ACK);
	msg.Add(ExID);
	lTotal = (long)Res.size();
	msg.Add(lTotal);
	for( itrGUID=Res.begin(); itrGUID!=Res.end(); ++itrGUID )
	{
		itrPlayer = m_mapPlayers.find(*itrGUID);
		if( itrPlayer == m_mapPlayers.end() ||
			NULL == (pInfo = itrPlayer->second ) )
		{
			/*sprintf(strLogTemp,
				"�����쳣: û�ж�ӦGUID�����.");
			PutoutLog("PM", LT_ERROR, strLogTemp);*/

            Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"�����쳣: û�ж�ӦGUID�����.");

			return false;
		}

		// �滻������Ϣ
		if(pInfo->btHideName == 1)
			msg.Add(GlobalSetup::GetSetup()->szHideName);
		else
			msg.Add(pInfo->strName.c_str());

		msg.Add(pInfo->strFaction.c_str());
		msg.Add(pInfo->strUnion.c_str());
		msg.Add(pInfo->lOccupation);
		msg.Add(pInfo->lSex);
		msg.Add(pInfo->lLevel);
		msg.Add(pInfo->lCountryID);
		msg.Add(pInfo->lRegionID);
		msg.Add(pInfo->lConst);
	}
	msg.SendToSocket(pMsg->GetSocketID());

	return true;
}

// Fox@20081107----------------------------------------------
// ��ҹ���, �����ڲ�ʹ��
// ����CPlayer�������PlayerInfo
bool CPlayerMan::FillPlayerInfo(CPlayer* pPlayer, tagPlayerInfo& rPlayerInfo)
{
	rPlayerInfo.ExID		= pPlayer->GetExID();
	rPlayerInfo.strName		= pPlayer->GetName();
	rPlayerInfo.strFaction	= pPlayer->GetFactionName();
	rPlayerInfo.strUnion	= pPlayer->GetUnionName();
	rPlayerInfo.lOccupation	= pPlayer->GetOccupation();
	rPlayerInfo.lSex		= pPlayer->GetSex();
	rPlayerInfo.lLevel		= pPlayer->GetLevel();
	rPlayerInfo.lCountryID	= pPlayer->GetCountry();
	rPlayerInfo.lRegionID	= pPlayer->GetRegionID();
	rPlayerInfo.btHideName	= pPlayer->GetHideFlag();
	rPlayerInfo.Position.lPosX = static_cast<long>(pPlayer->GetPosX());
	rPlayerInfo.Position.lPosY = static_cast<long>(pPlayer->GetPosY());
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
	CGUID ExID = pInfo->ExID;
	m_Sex2Players[pInfo->lSex].remove(ExID);
	m_Level2Players[CalcLevel(pInfo->lLevel)].remove(ExID);
	m_Country2Players[pInfo->lCountryID].remove(ExID);
	m_Const2Players[pInfo->lConst].remove( ExID );
	m_Region2Players[pInfo->lRegionID].remove(ExID);
	m_Area2Players[pInfo->lAreaID].remove(ExID);

	if( bComplete )			// ����ɾ��
	{
		*pInfo = tagPlayerInfo();
		pInfo->ExID = ExID;
		FreePlayer(pInfo);
		m_mapPlayers.erase(ExID);
	}

	return true;
}

// �������
// �������: δ����Ԥ��������Ŀʱ, ֱ�ӷ�����, ���ȡ��ʼֵ,
// ������Ҫnew�½��, ���ȡsize.
tagPlayerInfo* CPlayerMan::AllocPlayer(void)
{
	tagPlayerInfo* pInfo = NULL;
	if( !m_PrePlayers.empty() )
	{
		pInfo = m_PrePlayers.front();
		m_PrePlayers.pop_front();
	}

	if( NULL == pInfo )
	{
		pInfo = new tagPlayerInfo;
	}

	return pInfo;
}

// �������
// ���ղ���: δ����Ԥ��������Ŀʱ, push��Ԥ������,
// ����ֱ��delete.
void CPlayerMan::FreePlayer(tagPlayerInfo* pInfo)
{
	if( NULL == pInfo )
	{
        Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__, "�����ϢΪNULL.");
		return;
	}

	if( (long)m_PrePlayers.size() >= m_lMaxPlayerNum * m_lGSNum )
	{
		DELETE_SAFE(pInfo);
	}

	else
	{
		m_PrePlayers.push_back(pInfo);
	}
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