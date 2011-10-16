///////////////////////////////////////////////////////////////////////////////////////
//VillageWarSys.cpp
//��սϵͳ
//Function:��ɰ�սϵͳ����ع���
//Author:Joe
//Create Time:2005.7.13
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\villagewarsys.h"
#include "..\Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CVillageWarSys* CVillageWarSys::instance = NULL;

CVillageWarSys::CVillageWarSys(void)
{
	m_VillageWars.clear();
}

CVillageWarSys::~CVillageWarSys(void)
{
}

//�õ�Ψһʵ�����
CVillageWarSys* CVillageWarSys::getInstance()
{
	if(instance == NULL)
	{
		instance = OBJ_CREATE(CVillageWarSys);
	}
	return instance;
}

void   CVillageWarSys::Release()
{
	if(instance)
	{
		OBJ_RELEASE(CVillageWarSys, instance);
	}
}

bool CVillageWarSys::DecordFromByteArray(BYTE* pByte, long& pos)
{
	m_VillageWars.clear();
	long lSize = _GetLongFromByteArray(pByte,pos);
	for(int i=0;i<lSize;i++)
	{
		tagVilWarSetup VilWarSetup;
		_GetBufferFromByteArray( pByte, pos, &VilWarSetup,
			sizeof(tagVilWarSetup)-sizeof(list<CGUID>) );
		long x = sizeof(list<CGUID>);
		long lDecFacNum = _GetLongFromByteArray(pByte,pos);
		CGUID FactionID;
		for(int j=0;j<lDecFacNum;j++)
		{
			_GetBufferFromByteArray(pByte,pos,FactionID);
			VilWarSetup.DecWarFactions.push_back(FactionID);
		}
		m_VillageWars[VilWarSetup.lID] = VilWarSetup;
	}
	return true;
}

void	CVillageWarSys::InitiVillRegionState()
{
	tagTime CurrTime;
	// [RGN TEST]
	/*
	itVelWar it = m_VillageWars.begin();
	for(;it != m_VillageWars.end();it++)
	{
		if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.WarEndTime )
		{
			CServerRegion* pRegion	=GetGame()->FindRegion( (*it).second.lWarRegionID );
			if(pRegion == NULL)
				pRegion = GetGame()->FindProxyRegion((*it).second.lWarRegionID);
			if( pRegion )
			{
				pRegion->ReSetWarState((*it).first,(*it).second.RegionState);
			}

			//����ս����ͼ�Ĺ�������
			CServerRegion* pVilRegion	=GetGame()->FindRegion( (*it).second.lVilRegionID );
			if(pVilRegion == NULL)
				pVilRegion = GetGame()->FindProxyRegion((*it).second.lVilRegionID);
			if( pRegion && pVilRegion)
			{
				pRegion->SetCountry(pVilRegion->GetCountry());
			}
		}
	}*/
}

bool CVillageWarSys::UpdateApplyWarFacs(BYTE* pByte, long& pos)
{
	long lWarID = _GetLongFromByteArray(pByte,pos);
	itVelWar it = m_VillageWars.find(lWarID);
	if(it == m_VillageWars.end())	return false;

	tagVilWarSetup &AVilWarSetup = (*it).second;
	AVilWarSetup.DecWarFactions.clear();
	long lSize = _GetLongFromByteArray(pByte,pos);
	CGUID FactionID;
	for(int i=0;i<lSize;i++)
	{
		_GetBufferFromByteArray(pByte,pos,FactionID);
		AVilWarSetup.DecWarFactions.push_back(FactionID);
	}

	// [RGN TEST]
	//���³����������Ϣ
	//long lWarRegionID = GetWarRegionIDByTime(lWarID);
	//CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	//if( pRegion )
	//{
	//	pRegion->UpdateContendPlayer();
	//}

	return true;
}

//��ս��ʼ
void CVillageWarSys::OnDelcareWar(long lWarID)
{
	if(m_VillageWars.find(lWarID) == m_VillageWars.end() )	return;
	m_VillageWars[lWarID].RegionState = CIS_DUTH; 

	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);

	//����ս�������Ĺ���������ݵ�һ��
	long lVilRegionID = GetVilRegionIDByTime(lWarID);
	CServerRegion* pViRegion	=GetGame()->FindRegion(lVilRegionID);
	if(pViRegion == NULL)
		pViRegion = GetGame()->FindProxyRegion(lVilRegionID);
	if( pRegion && pViRegion)
	{
		pRegion->SetOwnedCityOrg(pViRegion->GetOwnedCityFaction(),
								pViRegion->GetOwnedCityUnion());
		pRegion->SetCountry(pViRegion->GetCountry());
	}

	if( pRegion )
	{
		pRegion->OnWarDeclare(lWarID);
	}*/
}

//���忪ʼ
void CVillageWarSys::OnAttackVillageStart(long lWarID)
{
	if(m_VillageWars.find(lWarID) == m_VillageWars.end() )	return;
	m_VillageWars[lWarID].RegionState = CIS_Fight; 

	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarStart(lWarID);
	}*/
}

//����ʱ�䵽
void CVillageWarSys::OnAttackVillageOutTime(long lWarID)
{
	if(m_VillageWars.find(lWarID) == m_VillageWars.end() )	return;
	
	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarTimeOut(lWarID);
	}*/
}

//�������
void CVillageWarSys::OnAttackVillageEnd(long lWarID)
{
	if(m_VillageWars.find(lWarID) == m_VillageWars.end() )	return;
	m_VillageWars[lWarID].DecWarFactions.clear();
	if( m_VillageWars[lWarID].RegionState != CIS_Fight )	return;

	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarEnd(lWarID);
	}*/
}

//����ݵ����
void CVillageWarSys::OnClearPlayer(long lWarID)
{
	if(m_VillageWars.find(lWarID) == m_VillageWars.end() )	return;

	// [RGN TEST]
	/*
	long lVilRegionID = GetVilRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lVilRegionID);
	if( pRegion )
	{
		DWORD dwColor = MAKELONG(MAKEWORD(254,237), MAKEWORD(218,255));
		char strSend[256]="";
		_snprintf(strSend,"�ݵ�����ս��Ҫ��ʼ���㽫������%s������",pRegion->GetName());
		CMessage msg( MSG_S2C_OTHER_ADDINFO );
		msg.Add((LONG)0);
		msg.Add( dwColor );
		msg.Add( DWORD(0) );	// ����ɫ
		msg.Add( strSend );
		msg.SendToRegion(pRegion);

		//��ʼ����ʱ1�����峡��
		pRegion->StartClearPlayerOut(60000);
	}*/
}

long	CVillageWarSys::GetWarRegionIDByTime(long lTime)
{
	itVelWar it = m_VillageWars.find(lTime);
	if(it == m_VillageWars.end())	return 0;
	return (*it).second.lWarRegionID;
}

long	CVillageWarSys::GetVilRegionIDByTime(long lTime)
{
	itVelWar it = m_VillageWars.find(lTime);
	if(it == m_VillageWars.end())	return 0;
	return (*it).second.lVilRegionID;
}

//�ж�ĳ������Ƿ��Ѿ���ս
bool CVillageWarSys::IsAlreadyDeclarForWar(long lWarID,const CGUID& FactionID)
{
	itVelWar it = m_VillageWars.find(lWarID);
	if(it == m_VillageWars.end())	return false;
	tagVilWarSetup &VilWarSetup = (*it).second;
	if( VilWarSetup.RegionState == CIS_NO )	false;

	if(	find(VilWarSetup.DecWarFactions.begin(),VilWarSetup.DecWarFactions.end(),FactionID) !=
		VilWarSetup.DecWarFactions.end())
		return true;
	return false;
}

//�õ��μ�ս���ĳ�������
bool	CVillageWarSys::GetWarNameForDeclar(const CGUID& FactionID,string &strName)
{
	// [RGN TEST]
	/*
	itVelWar it = m_VillageWars.begin();
	for(;it != m_VillageWars.end();it++)
	{
		tagVilWarSetup &VilWarSetup = (*it).second;
		if( VilWarSetup.RegionState == CIS_NO )	continue;

		if(	find(VilWarSetup.DecWarFactions.begin(),VilWarSetup.DecWarFactions.end(),FactionID) !=
		VilWarSetup.DecWarFactions.end())
		{
			CServerRegion* pRegion	=GetGame()->FindRegion( VilWarSetup.lWarRegionID );
			if(pRegion == NULL)
				pRegion = GetGame()->FindProxyRegion(VilWarSetup.lWarRegionID);
			if( pRegion )
			{
				strName = string(pRegion->GetName());
				return true;
			}
		}
	}*/
	return false;
}

long	CVillageWarSys::GetWarStartTime(long lWarRegionID)
{
	tagTime CurrTime;

	tagTime WarStartTime;
	itVelWar it = m_VillageWars.begin();
	long lResult = 0;
	for(;it != m_VillageWars.end();it++)
	{
		if( (*it).second.lWarRegionID == lWarRegionID )
		{
			if(CurrTime < (*it).second.WarEndTime )
			{
				WarStartTime = (*it).second.WarStartTime;
				lResult=(WarStartTime.Year()-1900);
				lResult<<=4;
				lResult+=(WarStartTime.Month()-1);
				lResult<<=5;
				lResult+=WarStartTime.Day();
				lResult<<=5;
				lResult+=WarStartTime.Hour();
				lResult<<=6;
				lResult+=WarStartTime.Minute();
				lResult<<=3;
				lResult+=WarStartTime.tmTime.tm_wday;
				break;
			}
		}
	}
	return lResult;
}

CVillageWarSys* GetVilWarSys()
{
	return CVillageWarSys::getInstance();
}
