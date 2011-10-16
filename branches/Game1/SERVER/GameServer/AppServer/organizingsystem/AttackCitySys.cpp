///////////////////////////////////////////////////////////////////////////////////////
//AttackCitySys.cpp
//����սϵͳ
//Function:��ɹ���սϵͳ��ʱ����Ƶȵ�
//Author:Joe
//Create Time:2004.7.29
#include "StdAfx.h"
#include ".\attackcitysys.h"
#include "..\Player.h"
#include "..\serverregion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


map<long,tagACT> CAttackCitySys::m_Attacks;

CAttackCitySys::tagGetTodayTaxTime CAttackCitySys::m_GetTodayTaxTime;
CAttackCitySys* CAttackCitySys::instance = NULL;
CAttackCitySys::CAttackCitySys(void)
{
}

CAttackCitySys::~CAttackCitySys(void)
{
}

//�õ�Ψһʵ�����
CAttackCitySys* CAttackCitySys::getInstance()
{
	if(instance==NULL)
	{
		instance=OBJ_CREATE(CAttackCitySys);
	}
	return instance;
}

//�ͷ���Դ
void   CAttackCitySys::Release()
{
	OBJ_RELEASE(CAttackCitySys, instance);
}

bool CAttackCitySys::DecordFromByteArray(BYTE* pByte, long& pos)
{
	m_Attacks.clear();
	long lSize = _GetLongFromByteArray(pByte,pos);
	for(int i=0;i<lSize;i++)
	{
		tagACT AttackCityTime;
		_GetBufferFromByteArray(pByte,pos,&AttackCityTime,sizeof(tagACT)-sizeof(list<long>));
		long lDecFacNum = _GetLongFromByteArray(pByte,pos);
		CGUID FactionID;
		for(int j=0;j<lDecFacNum;j++)
		{
			_GetBufferFromByteArray(pByte,pos,FactionID);
			AttackCityTime.DecWarFactions.push_back(FactionID);
		}
		m_Attacks[AttackCityTime.lTime] = AttackCityTime;
	}
	return true;
}

void	CAttackCitySys::InitiCityRegionState()
{
	tagTime CurrTime;

	// [RGN TEST]
/*	itAttack it = m_Attacks.begin();
	for(;it != m_Attacks.end();it++)
	{
		if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.AttackCityEndTime )
		{
			CServerRegion* pRegion	=GetGame()->FindRegion( (*it).second.lCityRegionID );
			if(pRegion == NULL)
				pRegion = GetGame()->FindProxyRegion((*it).second.lCityRegionID);
			if( pRegion )
			{
				pRegion->ReSetWarState((*it).first,(*it).second.RegionState);
			}
		}
	}*/
}

bool CAttackCitySys::UpdateApplyWarFacs(BYTE* pByte, long& pos)
{
	long lWarID = _GetLongFromByteArray(pByte,pos);
	itAttack it = m_Attacks.find(lWarID);
	if(it == m_Attacks.end())	return false;

	tagACT &AttackCitySetup = (*it).second;
	AttackCitySetup.DecWarFactions.clear();
	long lSize = _GetLongFromByteArray(pByte,pos);
	CGUID FactionID;
	for(int i=0;i<lSize;i++)
	{
		_GetBufferFromByteArray(pByte,pos,FactionID);
		AttackCitySetup.DecWarFactions.push_back(FactionID);
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

//���ǿ�ʼ
void CAttackCitySys::OnAttackCityStart(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	m_Attacks[lWarID].RegionState=CIS_Fight;

	// [RGN TEST]
/*	//���³����������Ϣ
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
void CAttackCitySys::OnAttackCityTimeOut(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	if( m_Attacks[lWarID].RegionState != CIS_Fight )	return;

	// [RGN TEST]
/*	//���³����������Ϣ
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarTimeOut(lWarID);
	}*/
}
//���ǽ���
void CAttackCitySys::OnAttackCityEnd(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	m_Attacks[lWarID].DecWarFactions.clear();

	// [RGN TEST]
	/*
	//���³����������Ϣ
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarEnd(lWarID);
	}*/
}

//������ս
void CAttackCitySys::OnDeclarWar(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	m_Attacks[lWarID].RegionState=CIS_DUTH;

	// [RGN TEST]
/*	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarDeclare(lWarID);
	}*/
}

//���Ǽ���
void CAttackCitySys::OnMass(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	m_Attacks[lWarID].RegionState=CIS_Mass;

	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if(pRegion == NULL)
		pRegion = GetGame()->FindProxyRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnWarMass(lWarID);
	}*/
}

//���������Ա
void CAttackCitySys::OnClearOtherPlayer(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;
	if(m_Attacks[lWarID].RegionState != CIS_Mass)	return;

	// [RGN TEST]
	/*
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnClearOtherPlayer(lWarID);
	}*/
}

//ˢ�³���
void CAttackCitySys::OnRefreshRegion(long lWarID)
{
	if( m_Attacks.find(lWarID) == m_Attacks.end())	return;

	/*
	// [RGN TEST]
	long lWarRegionID = GetWarRegionIDByTime(lWarID);
	CServerRegion* pRegion	=GetGame()->FindRegion(lWarRegionID);
	if( pRegion )
	{
		pRegion->OnRefreshRegion(lWarID);
	}*/
}

long	CAttackCitySys::GetWarRegionIDByTime(long lWarID)
{
	itAttack it = m_Attacks.find(lWarID);
	if(it == m_Attacks.end())	return 0;
	return (*it).second.lCityRegionID;
}

//�ж�ĳ������Ƿ��Ѿ���ս
bool CAttackCitySys::IsAlreadyDeclarForWar(long lWarID,const CGUID& FactionID)
{
	itAttack it = m_Attacks.find(lWarID);
	if(it == m_Attacks.end())	return false;
	tagACT &AttackCitySetup = (*it).second;
	if( AttackCitySetup.RegionState == CIS_NO )	false;

	if(	find(AttackCitySetup.DecWarFactions.begin(),AttackCitySetup.DecWarFactions.end(),FactionID) !=
		AttackCitySetup.DecWarFactions.end())
		return true;
	return false;
}

//�õ��μ�ս���ĳ�������
bool	CAttackCitySys::GetWarNameForDeclar(const CGUID& FactionID,string &strName)
{
	// [RGN TEST]
	/*
	itAttack it = m_Attacks.begin();
	for(;it != m_Attacks.end();it++)
	{
		tagACT &AttackCitySetup = (*it).second;
		if( AttackCitySetup.RegionState == CIS_NO )	continue;

		if(	find(AttackCitySetup.DecWarFactions.begin(),AttackCitySetup.DecWarFactions.end(),FactionID) !=
																AttackCitySetup.DecWarFactions.end())
		{
			CServerRegion* pRegion	=GetGame()->FindRegion( AttackCitySetup.lCityRegionID );
			if(pRegion == NULL)
				pRegion = GetGame()->FindProxyRegion(AttackCitySetup.lCityRegionID);
			if( pRegion )
			{
				strName = string(pRegion->GetName());
				return true;
			}
		}
	}*/
	return false;
}

//�õ���ǰʱ���WarID 
long CAttackCitySys::GetCurTimeWarNumByRegionID(long lRegionID)
{
	tagTime CurrTime;

	itAttack it = m_Attacks.begin();
	for(;it != m_Attacks.end();it++)
	{
		if( (*it).second.lCityRegionID == lRegionID )
		{
			if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.AttackCityEndTime )
				return (*it).first;
		}
	}
	return 0;
}

long	CAttackCitySys::GetWarStartTime(long lWarRegionID)
{
	tagTime CurrTime;

	tagTime WarStartTime;
	long lResult = 0;
	itAttack it = m_Attacks.begin();
	for(;it != m_Attacks.end();it++)
	{
		if( (*it).second.lCityRegionID == lWarRegionID )
		{
			if( CurrTime < (*it).second.AttackCityEndTime )
			{
				WarStartTime = (*it).second.AttackCityStartTime;
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

//�õ�ս����ʾ��ǰ״̬
eCityState CAttackCitySys::GetCityStateByWarNum(long lWarID)
{

	itAttack it = m_Attacks.find(lWarID);
	if(it != m_Attacks.end() )
		return (*it).second.RegionState;
	return CIS_NO;
}

CAttackCitySys* GetAttackCitySys()
{
	return CAttackCitySys::getInstance();
}
