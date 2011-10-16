//////////////////////////////////////////////////////////////////////////
//Servervillageregion.cpp
//��սս��ͼ�࣬�����ս�����ݴ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <MMSystem.h>
#include ".\servervillageregion.h"
#include "Player.h"

#include "appserver\Organizingsystem\VillageWarSys.h"

#include "..\nets\netserver\message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServerVillageRegion::CServerVillageRegion(void)
:CServerWarRegion(0)
{
	m_listGoods.clear();
	m_FlagOwnerFacID = NULL_GUID;
}
CServerVillageRegion::CServerVillageRegion(const CGUID& id)
:CServerWarRegion(id)
{
	m_listGoods.clear();
	m_FlagOwnerFacID = NULL_GUID;
}
CServerVillageRegion::CServerVillageRegion(long id)
:CServerWarRegion(id)
{
	m_listGoods.clear();
	m_FlagOwnerFacID = NULL_GUID;
}

CServerVillageRegion::~CServerVillageRegion(void)
{
}

// ����
bool CServerVillageRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CServerWarRegion::DecordFromByteArray(pByte,pos,bExData);
	return true;
}

//�ж�ĳ������Ƿ�������
bool CServerVillageRegion::IsOwner(const CGUID& FactionID)
{
	//[RGN TEST]
/*	long lVilRegionID = GetVilWarSys()->GetVilRegionIDByTime(GetWarNum());
	CServerRegion* pVilRegion	=GetGame()->FindRegion(lVilRegionID);
	if(pVilRegion == NULL)
		pVilRegion = GetGame()->FindProxyRegion(lVilRegionID);
	if(pVilRegion == NULL)	return false;

	//�жϸ���ҵİ���Ƿ�����Ĳμ�	
	if( NULL_GUID != FactionID && FactionID == pVilRegion->GetOwnedCityFaction() )
	{
		return true;
	}*/
	return false;
}

void CServerVillageRegion::OnWarDeclare(long lWarNum)
{
	CServerWarRegion::OnWarDeclare(lWarNum);

	m_FlagOwnerFacID = GetOwnedCityFaction();
}
//ĳ������ʱ�䵽
void CServerVillageRegion::OnContendTimeOver(tagContend Contend)
{
	CServerWarRegion::OnContendTimeOver(Contend);
}

//�����Ҫ�ĵ�����
void CServerVillageRegion::AddNeedGood(string& strGoodName)
{
	if( strGoodName == "")	return;

	m_listGoods.push_back(strGoodName);
}


//���ǿ�ʼ
void CServerVillageRegion::OnWarStart(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	CServerWarRegion::OnWarStart(lWarNum);
	m_bKickOutPlayer = false;
}

//����ʱ�����
void CServerVillageRegion::OnWarTimeOut(long lWarNum)
{
	//CMessage msg(MSG_S2W_WAR_FactionWinVillage);
	//msg.Add(GetWarNum());
	//msg.Add(GetID());
	//msg.Add(m_FlagOwnerFacID);
	//msg.Add((long)0);
	//msg.Send();
}

//ս������
void  CServerVillageRegion::OnWarEnd(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	if(GetCityState() == CIS_NO)	return;
	//����������б�
	CServerWarRegion::OnWarEnd(lWarNum);
	ClearRegion();
	//�����޹���
	SetCountry(0);
	//������ӵ����
	SetOwnedCityOrg(NULL_GUID, NULL_GUID);
	m_FlagOwnerFacID = NULL_GUID;
}

//��һ�����Ӯ����һ����־�ﴥ��
void CServerVillageRegion::OnFactionWinOneSymbol(const CGUID& FactionID,long lSymbolID)
{
	if(lSymbolID == 0)
	{
		m_FlagOwnerFacID = FactionID;
	}
}
//��һ������ʤʱ
void  CServerVillageRegion::OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID)
{
	if(GetCityState() == CIS_NO)	return;

	CServerWarRegion::OnFactionVictory(FactionID,UnionID);
}

//����ʱ�峡��
void CServerVillageRegion::ClearRegion()
{
	CServerWarRegion::ClearRegion();	

	list<string>::iterator it = m_listGoods.begin();
	for(; it != m_listGoods.end();it++)
	{
		DelOneGoodFromAllPlayer((char*)(*it).c_str());
	}
	//��ʼ������һ���ӵ���ʱ
	StartClearPlayerOut(60000);
}

bool CServerVillageRegion::IsApplyWarFacsMem(const CGUID& FactionID)
{
	return GetVilWarSys()->IsAlreadyDeclarForWar( GetWarNum(),FactionID );
}
