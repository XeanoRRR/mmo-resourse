//////////////////////////////////////////////////////////////////////////
//serverwarregion.cpp
//ս�����᳡���Ļ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include <MMSystem.h>
#include ".\serverwarregion.h"
#include "Player.h"
#include "..\..\setup\globesetup.h"
#include "country\country.h"

#include "..\nets\netserver\message.h"

CServerWarRegion::CServerWarRegion(void)
:CServerRegion(-1)
{
	m_listContend.clear();
	m_FacWinSymbol.clear();
	m_lSymbolTotalNum = 0;
	m_lWinVicSymbolNum = 0;
}
CServerWarRegion::CServerWarRegion(const CGUID& id)
:CServerRegion(id)
{
	m_listContend.clear();
	m_FacWinSymbol.clear();
	m_lSymbolTotalNum = 0;
	m_lWinVicSymbolNum = 0;
}
CServerWarRegion::CServerWarRegion(long id)
:CServerRegion(id)
{
	m_listContend.clear();
	m_FacWinSymbol.clear();
	m_lSymbolTotalNum = 0;
	m_lWinVicSymbolNum = 0;
}

CServerWarRegion::~CServerWarRegion(void)
{
}

// ��ӵ�CByteArray
bool CServerWarRegion::AddToByteArray(vector<BYTE>* pByteArray, bool bExData )
{
	CServerRegion::AddToByteArray(pByteArray,bExData);
	return true;
}
// ����
bool CServerWarRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData )
{
	CServerRegion::DecordFromByteArray(pByte,pos,bExData);
	m_lSymbolTotalNum = _GetLongFromByteArray(pByte, pos);
	m_lWinVicSymbolNum = _GetLongFromByteArray(pByte, pos);
	m_lVicSymbolNum = _GetLongFromByteArray(pByte, pos);

	//���ó�ʼ��־��Ĺ���
	int i = 0;
	for(;i < m_lSymbolTotalNum;i++)
	{
		m_FacWinSymbol[i]=GetOwnedCityFaction();
	}
	return true;
}

//ĳ������ʱ�䵽
void CServerWarRegion::OnContendTimeOver(tagContend Contend)
{	
	CPlayer* pPlayer = GetGame()->FindPlayer(Contend.PlayerID);
	if(pPlayer == NULL)	return;

	const CGUID& FactionID = pPlayer->GetFactionID();
	if(FactionID == NULL_GUID)	return;
	const CGUID& UnionID = pPlayer->GetUnionID();

	if( IsOwner(FactionID) == false )
	{
		if( IsApplyWarFacsMem(FactionID) == false)
			return;
	}

	//ȡ��������ҶԸñ�־�������
	CancelContendBySymbolID(Contend.lID);

	SetFacWinSymbol(FactionID,UnionID,Contend.lID);

	//ͨ����������ĳ������ȡ�˸ñ�־��
	DWORD dwColor = MAKELONG(MAKEWORD(255,255), MAKEWORD(255,255));
	DWORD dwBkColor = MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255));
	//char strSend[256];
	//_snprintf(strSend,"%s����ȡ��%s��",pPlayer->GetFactionName().c_str(),Contend.strName.c_str());
	//CMessage msg( MSG_S2C_OTHER_ADDINFO );
	//msg.Add((LONG)0);
	//msg.Add( dwColor );
	//msg.Add( dwBkColor );	// ����ɫ
	//msg.Add( strSend );
	//msg.SendToRegion(this);

	//��Ӷ�����ʾ��Ϣ
	//char strTopInfo[256]="";
	//_snprintf(strTopInfo,"%s��ȡ��%s��[[[%s]]],��ʱ�����ս��������Ȩ��",pPlayer->GetFactionName().c_str(),GetName(),Contend.strName.c_str());
	//GetGame()->SendTopInfoToClient(-1,NULL_GUID,1,1,string(strTopInfo));

	//���ս��������Ϣ
	//char strWarInfo[256];
	//_snprintf(strWarInfo,"WarNum:%d,OwnedFac:%d,OwnedUnion:%d :",GetWarNum(),GetOwnedCityFaction(),GetOwnedCityUnion() );
	//PutWarDebugString(strWarInfo);
	//_snprintf(strWarInfo,"%s(FacID:%d) ����ȡ�� %s(ConID:%d)��(lWinFacID:%d,lWinUniID:%d)",
	//	pPlayer->GetFactionName().c_str(),pPlayer->GetFactionID(),Contend.strName.c_str(),Contend.lID,FactionID,UnionID);
	//PutWarDebugString(strWarInfo);
}

//��һ����ҵ�����������־״̬
void CServerWarRegion::OnEnterContend(CPlayer* pPlayer,long lID,const char* chName,long lTime,string strGood1,string strGood2,
								string strGood3,string strGood4)
{
	if(pPlayer == NULL)	return;
	//�жϸ�����Ƿ��а��
	if(pPlayer->GetFactionID() == NULL_GUID)	return;
	//������������״̬�����˳�
	if(pPlayer->GetState() == CShape::STATE_DIED || pPlayer->IsDied()) return;
	
	const CGUID& FactionID = pPlayer->GetFactionID();
	const CGUID& UnionID = pPlayer->GetUnionID();
	
	DWORD dwColor = MAKELONG(MAKEWORD(255,255), MAKEWORD(255,255));
	DWORD dwBkColor = MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255));
	//�жϸð������Ƿ��Ѿ���ȡ�˸ñ�־��
	if(GetIsFacWinSymbol(FactionID,lID) == true)
	{
		char strSend[256]="�����������Ѿ�ӵ���˸ñ�־�";
		pPlayer->SendNotifyMessage(strSend,dwColor,dwBkColor);
		return;
	}

	if( IsOwner(FactionID) == false )
	{
		if(IsApplyWarFacsMem(FactionID) == false)
		{
			char strSend[256]="���������ỹδ�����μ�ս�����޷�ռ��ñ�־�";
			pPlayer->SendNotifyMessage(strSend,dwColor,dwBkColor);
			return;
		}
	}
	//�жϸ�����Ƿ���������ñ�־��]
	if( GetIsPlayerIsContendSymbol(pPlayer->GetExID(),lID) == true )	return;
	//�жϸ�����Ƿ����㹻����Ʒ
	if( ( strGood1 != "" && pPlayer->CheckGoods(strGood1.c_str() ) == 0 ) ||
		( strGood2 != "" && pPlayer->CheckGoods(strGood2.c_str() ) == 0 ) ||
		( strGood3 != "" && pPlayer->CheckGoods(strGood3.c_str() ) == 0 ) ||
		( strGood4 != "" && pPlayer->CheckGoods(strGood4.c_str() ) == 0 ) )
	{
		char strSend[256]="û���㹻��Ʒ���޷���ȡ��";
		pPlayer->SendNotifyMessage(strSend,dwColor,dwBkColor);
		return;
	}

	CancelContendByPlayerID(pPlayer);

	AddContend(pPlayer,lID,chName,lTime);
	pPlayer->SendNotifyMessage("�����������״̬���ƶ�(���ƶ�)��ʹ����Ʒ�����������������������˳���Ϸ���ᵼ�¸�״̬����ϣ��ܵ��˺����ӻ�������ȡ�",dwColor,dwBkColor);

	AddNeedGood(strGood1);
	AddNeedGood(strGood2);
	AddNeedGood(strGood3);
	AddNeedGood(strGood4);
}

//����ĳ�����Ӯ��ĳ����־��
void CServerWarRegion::SetFacWinSymbol(const CGUID& FactionID,const CGUID& UnionID,long lSymbol)
{
	m_FacWinSymbol[lSymbol] = FactionID;

	OnFactionWinOneSymbol(FactionID,lSymbol);
	//�ж��������ȡ�ı�־������Ƿ�ﵽ��ʤ����
	if( FactionID != GetOwnedCityFaction() )
	{
		long lNum = 0;
		itFacSym it = m_FacWinSymbol.begin();
		for(;it != m_FacWinSymbol.end();it++)
		{
			if( (*it).second == FactionID )
				lNum++;
		}

		//����һ�����ʤ��
		if(lNum >= m_lWinVicSymbolNum)
		{
			OnFactionVictory(FactionID,UnionID);
		}
	}
}
//�ж�ĳ������Ƿ�Ӯ����ĳ����־��
bool CServerWarRegion::GetIsFacWinSymbol(const CGUID& FactionID,long lSymbolID)
{
	itFacSym it = m_FacWinSymbol.find(lSymbolID);
	if(it == m_FacWinSymbol.end())	return false;

	if((*it).second  == FactionID )	return true;
	return false;
}

//�ж�ĳ������Ƿ���������ĳ����־����
bool CServerWarRegion::GetIsPlayerIsContendSymbol(const CGUID& PlayerID,long lSymbolID)
{
	itContend it = m_listContend.begin();
	for(;it != m_listContend.end();it++)
	{
		if((*it).PlayerID == PlayerID)
		{
			if( (*it).lID == lSymbolID )
				return true;
			else
				return false;
		}
	}
	return false;
}
//���һ������
void CServerWarRegion::AddContend(CPlayer* pPlayer,long lID,const char* chName,long lTime)
{
	if(pPlayer == NULL)	return;

	bool bFacFirst = true;
	itContend it = m_listContend.begin();
	for(;it != m_listContend.end();it++)
	{
		if((*it).FactionID == pPlayer->GetFactionID())
		{
			bFacFirst = false;
			break;
		}
	}
	
	tagContend Contend = {lID,string(chName),pPlayer->GetExID(),pPlayer->GetFactionID(),0,lTime,timeGetTime()};
	m_listContend.push_back(Contend);
	//��������״̬
	//pPlayer->SetContendState(true);
	//ˢ��ʱ�䵽�ͻ���
	UpdateContentTime(pPlayer->GetExID(),0);

	//����Ǹð��ĵ�һ�Σ�����Ϣ֪ͨ���ĳ����Ὺʼ��ӡ
	if(bFacFirst)
	{
		//DWORD dwColor = MAKELONG(MAKEWORD(255,255), MAKEWORD(255,255));
		//DWORD dwBkColor = MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255));
		//char strSend[256]="";
		//BYTE bCountry = pPlayer->GetCountry();
		//_snprintf(strSend,"%s��%s��Ὺʼ��ӡ%s��",CountryName[bCountry],pPlayer->GetFactionName().c_str(),chName);
		//CMessage msg( MSG_S2C_OTHER_ADDINFO );
		//msg.Add((LONG)0);
		//msg.Add( dwColor );
		//msg.Add( dwBkColor );
		//msg.Add( strSend );
		//msg.SendToRegion(this);
	}
}

//����ҹ����ܵ��˺�
void CServerWarRegion::OnPlayerDamage(CPlayer* pPlayer,long lDamHp)
{
	if(lDamHp <= 0)	return;
	if(pPlayer == NULL)	return;

	float fPercent = (float)lDamHp/(float)pPlayer->GetMaxHP()*CGlobeSetup::GetSetup()->fDecTimeParam;

	list<tagContend>::iterator it = m_listContend.begin();
	for(;it != m_listContend.end();it++)
	{
		if((*it).PlayerID == pPlayer->GetExID())
		{
			long lDecTime = (*it).lMaxTime*fPercent;
			(*it).lCurTime -= lDecTime;
			if((*it).lCurTime < 0)
				(*it).lCurTime = 0;

			long lPercent = 0;
			if( (*it).lMaxTime != 0)
			{
				lPercent = 100*(*it).lCurTime/(*it).lMaxTime;
			}

			//ˢ�¿ͻ���ʱ��
			UpdateContentTime( (*it).PlayerID,lPercent );
			break;
		}
	}
}
//����ʱ��
void CServerWarRegion::DecContendTime(CPlayer* pPlayer,long lDecTime)
{
	if(pPlayer == NULL)	return;

	list<tagContend>::iterator it = m_listContend.begin();
	for(;it != m_listContend.end();it++)
	{
		if((*it).PlayerID == pPlayer->GetExID())
		{
			(*it).lCurTime -= lDecTime;
			if((*it).lCurTime < 0)
				(*it).lCurTime = 0;

			long lPercent = 0;
			if( (*it).lMaxTime != 0)
			{
				lPercent = 100*(*it).lCurTime/(*it).lMaxTime;
			}

			//ˢ�¿ͻ���ʱ��
			UpdateContentTime( (*it).PlayerID,lPercent );
			break;
		}
	}
}

//ȡ��ĳ����ҵ�����
bool CServerWarRegion::CancelContendByPlayerID(CPlayer* pPlayer)
{
	bool bFlag = false;
	if(pPlayer == NULL)	return bFlag;

	list<tagContend>::iterator it = m_listContend.begin();
	for(;it != m_listContend.end();)
	{
		if((*it).PlayerID == pPlayer->GetExID())
		{
			bFlag = true;
			it = m_listContend.erase(it);
		}
		else
		{
			++it;
		}
	}

	//��������״̬
	//pPlayer->SetContendState(false);
	//ˢ��ʱ�䵽�ͻ���
	UpdateContentTime(pPlayer->GetExID(),0);
	return true;
}

//ȡ��������Ҷ�ĳ����־�������
void CServerWarRegion::CancelContendBySymbolID(long lSymbolID)
{
	itContend it = m_listContend.begin();
	for(;it != m_listContend.end();)
	{
		if((*it).lID == lSymbolID )
		{
			//ˢ��ʱ�䵽�ͻ���
			UpdateContentTime((*it).PlayerID,0);
			//��������״̬
			CPlayer* pPlayer = GetGame()->FindPlayer((*it).PlayerID);
			if(pPlayer)
				//pPlayer->SetContendState(false);

			it = m_listContend.erase(it);
		}
		else
		{
			++it;
		}
	}
}


//��������İ������������
void CServerWarRegion::UpdateContendPlayer()
{
	//�����ս��ʱ��
	if(GetCityState() == CIS_Fight)
	{
		itContend it = m_listContend.begin();
		for(;it != m_listContend.end();)
		{
			//��������״̬
			CPlayer* pPlayer = GetGame()->FindPlayer((*it).PlayerID);
			if(pPlayer)
			{
				//����ð��û���������б���
				if(  IsApplyWarFacsMem( pPlayer->GetFactionID() ) == false)
				{
					//ˢ��ʱ�䵽�ͻ���
					UpdateContentTime((*it).PlayerID,0);
					//pPlayer->SetContendState(false);
					it = m_listContend.erase(it);
				}
				else
				{
					it++;
				}
			}
			else
			{
				it++;
			}
		}
	}
}

//ˢ������״̬ʱ�䵽�ͻ���
void CServerWarRegion::UpdateContentTime(const CGUID& PlayerID,long lPercent)
{
	//CMessage msg(MSG_S2C_WAR_UpdateContendTime);
	//msg.Add(lPercent);
	//msg.SendToPlayer(PlayerID);
}

//��ʼ��ս
void CServerWarRegion::OnWarDeclare(long lWarNum)
{
	CServerRegion::OnWarDeclare(lWarNum);

	//���ó�ʼ��־��Ĺ���
	int i = 0;
	m_FacWinSymbol.clear();
	for(;i < m_lSymbolTotalNum;i++)
	{
		m_FacWinSymbol[i]=GetOwnedCityFaction();
	}

}
//���ǿ�ʼ
void CServerWarRegion::OnWarStart(long lWarNum)
{
	CServerRegion::OnWarStart(lWarNum);
}
//���Ǽ���
void CServerWarRegion::OnWarMass(long lWarNum)
{
	CServerRegion::OnWarMass(lWarNum);
}

//ս������
void  CServerWarRegion::OnWarEnd(long lWarNum)
{
	CServerRegion::OnWarEnd(lWarNum);
	m_FacWinSymbol.clear();
}

//��һ������ʤʱ
void CServerWarRegion::OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID)
{
}

//����ʱ�峡��
void CServerWarRegion::ClearRegion()
{
	list<tagContend>::iterator it = m_listContend.begin();
	for(;it != m_listContend.end();it++)
	{
		//�ָ�״̬
		UpdateContentTime((*it).PlayerID,0);
		//CPlayer* pPlayer = dynamic_cast<CPlayer*>(this->FindChildObject(TYPE_PLAYER,(*it).PlayerID));
		//if(pPlayer)
			//pPlayer->SetContendState(false);
	}
	m_listContend.clear();
}

void CServerWarRegion::ReSetWarState(long lWarID,eCityState eState)
{
	CServerRegion::ReSetWarState(lWarID,eState);

	if(eState >= CIS_DUTH && eState <= CIS_Fight)
	{
		//���ó�ʼ��־��Ĺ���
		int i = 0;
		m_FacWinSymbol.clear();
		for(;i < m_lSymbolTotalNum;i++)
		{
			m_FacWinSymbol[i]=GetOwnedCityFaction();
		}
	}
}

