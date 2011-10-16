///////////////////////////////////////////////////////////////////////////////////////
//AttackCitySys.cpp
//����սϵͳ
//Function:��ɹ���սϵͳ��ʱ����Ƶȵ�
//Author:Joe
//Create Time:2004.7.29
#include "StdAfx.h"
#include ".\attackcitysys.h"
#include ".\villagewarsys.h"
#include "..\nets\networld\message.h"

#include "OrganizingCtrl.h"
#include "..\WorldRegion.h"
#include "..\Player.h"
#include "Organizing.h"
#include "organizingparam.h"
#include "Faction.h"

#include "..\Country\Country.h"
#include "..\Country\CountryHandler.h"
#include "../GlobalRgnManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//map<long,tagACT> CAttackCitySys::m_Attacks;
//CAttackCitySys::tagGetTodayTaxTime CAttackCitySys::m_GetTodayTaxTime;
//CAttackCitySys* CAttackCitySys::instance = NULL;
//CAttackCitySys::CAttackCitySys(void)
//{
//}
//
//CAttackCitySys::~CAttackCitySys(void)
//{
//}
//
////�õ�Ψһʵ�����
//CAttackCitySys* CAttackCitySys::getInstance()
//{
//	if(instance==NULL)
//	{
//		instance=new CAttackCitySys;
//	}
//	return instance;
//}
////��ʼ��
//bool   CAttackCitySys::Initialize()
//{
//	return true;
//	m_Attacks.clear();
//
//	char filename[]="setup/CityWarSys.ini";
//
//	ifstream stream;
//	stream.open(filename);
//	if( !stream.is_open() )
//	{
//		char str[]="setup/CityWarSys.ini can't found!";
//		AddLogText(str);
//		return false;
//	}
//
//	tagTime CurrTime;
//	string strDate;
//	long lTime = 0;
//	map<long,tagTime> LastEndWarTime;LastEndWarTime.clear();
//	while(ReadTo(stream, "*"))
//	{
//		lTime++;
//		tagAttackCityTime	AttackCityTime;
//		AttackCityTime.lTime=lTime;
//		long lStartInfoTime,lDecTime,lEndInfoTime,lLastTime,lMassTime,lClearTime,lRefreshTime;
//		stream>>AttackCityTime.lCityRegionID
//			>>strDate
//			>>lStartInfoTime
//			>>lDecTime
//			>>lEndInfoTime
//			>>lLastTime
//			>>lMassTime
//			>>lClearTime
//			>>lRefreshTime;
//
//		AttackCityTime.RegionState = CIS_NO;
//		AttackCityTime.DecWarFactions.clear();
//		tagTime Time(strDate.c_str());
//		AttackCityTime.AttackCityStartTime=Time;
//
//		//��ʼ��ʾʱ��
//		tagTime TemptTime = Time;
//		TemptTime.AddMinute(lStartInfoTime);
//		AttackCityTime.AttackCityStartInfoTime=TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lDecTime);
//		AttackCityTime.DeclarWarTime=TemptTime;
//
//		//������ʾʱ��
//		TemptTime = Time;
//		TemptTime.AddMinute(lEndInfoTime);
//		AttackCityTime.AttackCityEndInfoTime=TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lLastTime);
//		AttackCityTime.AttackCityEndTime=TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lMassTime);
//		AttackCityTime.MassTime=TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddSecond(lClearTime);
//		AttackCityTime.ClearPlayerTime=TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddSecond(lRefreshTime);
//		AttackCityTime.RefreshRegionTime=TemptTime;
//
//		//������
//		if( AttackCityTime.DeclarWarTime >= AttackCityTime.MassTime  ||
//			AttackCityTime.DeclarWarTime >= AttackCityTime.AttackCityStartTime ||
//			AttackCityTime.DeclarWarTime >= AttackCityTime.AttackCityEndTime )
//		{
//			AddLogText("Load ACityWarSys.ini Error!");
//			return false;
//		}
//
//		if( AttackCityTime.MassTime >= AttackCityTime.AttackCityStartTime ||
//			AttackCityTime.MassTime >= AttackCityTime.AttackCityEndTime )
//		{
//			AddLogText("Load CityWarSys.ini Error!");
//			return false;
//		}
//
//		if( AttackCityTime.AttackCityStartTime >= AttackCityTime.AttackCityEndTime )
//		{
//			AddLogText("Load CityWarSys.ini Error!");
//			return false;
//		}
//
//		if(AttackCityTime.AttackCityStartInfoTime >= AttackCityTime.AttackCityStartTime)
//		{
//			AddLogText("Load CityWarSys.ini Error!");
//			return false;
//		}
//
//		if(AttackCityTime.AttackCityEndInfoTime >= AttackCityTime.AttackCityEndTime)
//		{
//			AddLogText("Load CityWarSys.ini Error!");
//			return false;
//		}
//
//
//		//������
//		m_Attacks[lTime]=AttackCityTime;
//		map<long,tagTime>::iterator itEndTime = LastEndWarTime.find( AttackCityTime.lCityRegionID );
//		if( itEndTime  != LastEndWarTime.end() )
//		{
//			if( AttackCityTime.AttackCityStartTime <= (*itEndTime).second )
//			{
//				AddLogText("Load CityWarSys.ini Error!");
//				return false;
//			}
//		}
//		LastEndWarTime[AttackCityTime.lCityRegionID] = AttackCityTime.AttackCityEndTime;
//	}
//	stream.close();
//
//
//	// ע��ʱ���¼�
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		long lTemptTime = (*it).first;
//		if((*it).second.AttackCityEndTime >= CurrTime)
//		{
//			(*it).second.lEndEventID = GetTimer()->SetTimeEvent((*it).second.AttackCityEndTime,
//				(CallBackFun)CAttackCitySys::OnAttackCityEnd,lTemptTime);
//
//			//ע�������ʾʱ��
//			if( (*it).second.AttackCityEndInfoTime >= CurrTime )
//			{
//				(*it).second.lEndInfoEventID = GetTimer()->SetTimeEvent((*it).second.AttackCityEndInfoTime,
//				(CallBackFun)CAttackCitySys::OnAttackCityEndnfo,lTemptTime);
//			}
//			else
//			{
//				(*it).second.lEndInfoEventID = GetTimer()->SetTimeEvent(CurrTime,
//				(CallBackFun)CAttackCitySys::OnAttackCityEndnfo,lTemptTime);
//			}
//
//			if((*it).second.AttackCityStartTime>=CurrTime )
//			{
//				(*it).second.lClearEventID = GetTimer()->SetTimeEvent((*it).second.ClearPlayerTime,
//					(CallBackFun)CAttackCitySys::OnClearOtherPlayer,lTemptTime);
//
//				(*it).second.lStartEventID =GetTimer()->SetTimeEvent((*it).second.AttackCityStartTime,
//				(CallBackFun)CAttackCitySys::OnAttackCityStart,lTemptTime);
//
//				(*it).second.lRefreshEventID = GetTimer()->SetTimeEvent((*it).second.RefreshRegionTime,
//					(CallBackFun)CAttackCitySys::OnRefreshRegion,lTemptTime);
//
//				//ע�Ὺʼ��ʾʱ��
//				if( (*it).second.AttackCityStartInfoTime >= CurrTime )
//				{
//					(*it).second.lStartInfoEventID = GetTimer()->SetTimeEvent((*it).second.AttackCityStartInfoTime,
//					(CallBackFun)CAttackCitySys::OnAttackCityStartInfo,lTemptTime);
//				}
//				else
//				{
//					(*it).second.lStartInfoEventID = GetTimer()->SetTimeEvent(CurrTime,
//					(CallBackFun)CAttackCitySys::OnAttackCityStartInfo,lTemptTime);
//				}
//
//				if((*it).second.MassTime>=CurrTime )
//				{
//					(*it).second.lMassEventID = GetTimer()->SetTimeEvent((*it).second.MassTime,
//					(CallBackFun)CAttackCitySys::OnMass,lTemptTime);
//
//					if( (*it).second.DeclarWarTime >= CurrTime )
//					{
//						(*it).second.lDeclarWarEventID = GetTimer()->SetTimeEvent((*it).second.DeclarWarTime,
//							(CallBackFun)CAttackCitySys::OnDeclarWar,lTemptTime);
//					}
//					else
//					{
//						(*it).second.RegionState = CIS_DUTH;
//					}
//				}
//				else
//				{
//					(*it).second.RegionState = CIS_Mass;
//				}
//			}
//			else
//			{
//				(*it).second.RegionState = CIS_Fight;
//			}
//		}							
//	}
//	return true;
//}
//
//void	CAttackCitySys::SetDecWarFactionsFromDB(long lRegionID,tagTime& WarStartTime,list<CGUID>& DecWarFactions)
//{
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.lCityRegionID == lRegionID && WarStartTime == (*it).second.AttackCityStartTime &&
//			(*it).second.RegionState != CIS_NO)
//		{
//			(*it).second.DecWarFactions = DecWarFactions;
//			return;
//		}
//	}
//}
//
////��ӵ�CByteArray
//bool CAttackCitySys::AddToByteArray(vector<BYTE>& ByteArray)
//{
//	_AddToByteArray(&ByteArray,(long)m_Attacks.size());
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		tagACT &AttackCityTime = (*it).second;
//		_AddToByteArray(&ByteArray,&AttackCityTime,sizeof(tagACT)-sizeof(list<long>) );
//		//�����ս����б�
//		_AddToByteArray(&ByteArray,(long)AttackCityTime.DecWarFactions.size());
//		list<CGUID>::iterator itFac= AttackCityTime.DecWarFactions.begin();
//		for(;itFac != AttackCityTime.DecWarFactions.end();itFac++)
//		{
//			_AddToByteArray(&ByteArray,(*itFac));
//		}
//	}
//	return true;
//}
//
////�����ս��ᵽCByteArray
//bool CAttackCitySys::UpdateApplyWarFacsToGameServer(long lWarID,vector<BYTE>& ByteArray)
//{
//	list<CGUID>	DecFacs;
//	itAttack it = m_Attacks.find(lWarID);
//	if(it != m_Attacks.end())
//	{
//		DecFacs = (*it).second.DecWarFactions;
//	}
//	_AddToByteArray(&ByteArray,lWarID);
//	_AddToByteArray(&ByteArray,(long)DecFacs.size());
//	list<CGUID>::iterator itFac = DecFacs.begin();
//	for(;itFac != DecFacs.end();itFac++)
//	{
//		_AddToByteArray(&ByteArray,(CGUID)(*itFac));
//	}
//	return true;
//}
//
//void CAttackCitySys::GenerateSaveData()
//{
///*	list<tagAttackCityTime*> TemptCityWarSetupList;
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		tagAttackCityTime* pSetup = new tagAttackCityTime;
//		pSetup->lCityRegionID = (*it).second.lCityRegionID;
//		pSetup->AttackCityStartTime = (*it).second.AttackCityStartTime;
//		if((*it).second.RegionState != CIS_NO)
//			pSetup->DecWarFactions = (*it).second.DecWarFactions;
//		else
//			pSetup->DecWarFactions.clear();
//		TemptCityWarSetupList.push_back(pSetup);
//	}
//	GetGame()->SetCityWarParam(TemptCityWarSetupList);*/
//}
//
//bool  CAttackCitySys::Reload()
//{
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		tagAttackCityTime&	AttackCityTime=(*it).second;
//		GetTimer()->KillTimeEvent(AttackCityTime.lDeclarWarEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lMassEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lClearEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lRefreshEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lStartInfoEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lStartEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lEndInfoEventID);
//		GetTimer()->KillTimeEvent(AttackCityTime.lEndEventID);
//
//		GetTimer()->KillTimeEvent(m_GetTodayTaxTime.lEventID);
//	}
//
//	map<long,tagACT> TemptAttacks = m_Attacks;
//	itAttack itAttack = TemptAttacks.begin();
//	for(;itAttack != TemptAttacks.end();itAttack++)
//	{
//		if( (*itAttack).second.RegionState != CIS_NO )
//		{
//			GetAttackCitySys()->OnCityWarEnd( (*itAttack).first );
//			GetAttackCitySys()->OnCityWarEndToGameServer((*itAttack).second.lCityRegionID,(*itAttack).first);
//		}
//	}
//
//	Initialize();
//	return true;
//}
////�ͷ���Դ
//void   CAttackCitySys::Release()
//{
//	SAFE_DELETE(instance);
//}
//
//long	CAttackCitySys::GetWarRegionIDByTime(long lTime)
//{
//	itAttack it = m_Attacks.find(lTime);
//	if(it == m_Attacks.end())	return 0;
//	return (*it).second.lCityRegionID;
//}
//
////���ǿ�ʼ
//void CALLBACK CAttackCitySys::OnAttackCityStart(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	m_Attacks[lTime].RegionState=CIS_Fight;
//	CMessage msg(MSG_W2S_ATTACKCITY_Start);
//	msg.Add(lTime);
//	msg.SendAll();
//	
//	long lRegionID = GetAttackCitySys()->GetWarRegionIDByTime(lTime);
//	if(lRegionID == 0)	return;
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strSend[256]="";
//	sprintf(strSend,"%s��ս�ѿ�ʼ��",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	char strWarInfo[256];
//	sprintf(strWarInfo,"(Num:%d){%s}��ս�Ѿ���ʼ��",lTime,pRegion->pRegion->GetName());
//	PutWarDebugString(strWarInfo);
//}
////���ǽ���
//void CALLBACK CAttackCitySys::OnAttackCityEnd(LPARAM lparam)
//{	
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//	if(m_Attacks[lTime].RegionState != CIS_Fight)	return;
//
//	m_Attacks[lTime].RegionState = CIS_NO;
//	CMessage msg(MSG_W2S_ATTACKCITY_TimeOut);
//	msg.Add(lTime);
//	msg.SendAll();
//
//	//���ս��������Ϣ
//	char strWarInfo[256];
//	sprintf(strWarInfo,"(Num:%d) ��սʱ�������",lTime);
//	PutWarDebugString(strWarInfo);
//}
//
////��������ս����
//void	CAttackCitySys::OnCityWarEnd(long lWarNum)
//{
//	long lTime = lWarNum;
//	m_Attacks[lTime].DecWarFactions.clear();
//
//	//�ڳ�ս��������°���ս�ж��б�
//	UpdateCityAllFactionEnemyRelation();
//}
//
////���ͽ�����Ϣ��Gameserver
//void    CAttackCitySys::OnCityWarEndToGameServer(long lRegionID,long lTime)
//{
//	CMessage msg(MSG_W2S_ATTACKCITY_End);
//	msg.Add(lTime);
//	msg.SendAll();
//}
//
////������ս
//void CALLBACK CAttackCitySys::OnDeclarWar(LPARAM lparam)
//{
//	//�߼�����
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	m_Attacks[lTime].RegionState=CIS_DUTH;
//	CMessage msg(MSG_W2S_ATTACKCITY_Declar);
//	msg.Add(lTime);
//	msg.SendAll();
//
//
//	//�����ʾ��Ϣ
//	long lRegionID = GetAttackCitySys()->GetWarRegionIDByTime(lTime);
//	if(lRegionID == 0)	return;
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strSend[256]="";
//	sprintf(strSend,"%s��ս�ѿ�ʼ������",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	char strWarInfo[256];
//	sprintf(strWarInfo,"(Num:%d){%s}��ս�ѿ�ʼ������",lTime,pRegion->pRegion->GetName());
//	PutWarDebugString(strWarInfo);
//}
////���Ǽ���
//void CALLBACK CAttackCitySys::OnMass(LPARAM lparam)
//{
//	//�߼�����
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	m_Attacks[lTime].RegionState = CIS_Mass;
//	CMessage msg(MSG_W2S_ATTACKCITY_Mass);
//	msg.Add(lTime);
//	msg.SendAll();
//
//	//�����ʾ��Ϣ
//	long lRegionID = GetAttackCitySys()->GetWarRegionIDByTime(lTime);
//	if(lRegionID == 0)	return;
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strSend[256]="";
//	sprintf(strSend,"%s��ս�ѿ�ʼ���ᡣ",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	char strWarInfo[256];
//	sprintf(strWarInfo,"(Num:%d){%s}��ս�ѿ�ʼ���ᡣ",lTime,pRegion->pRegion->GetName());
//	PutWarDebugString(strWarInfo);
//}
////���������Ա
//void CALLBACK CAttackCitySys::OnClearOtherPlayer(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	CMessage msg(MSG_W2S_ATTACKCITY_ClearOtherPlayer);
//	msg.Add(lTime);
//	msg.SendAll();
//}
////ˢ�³���
//void CALLBACK CAttackCitySys::OnRefreshRegion(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	CMessage msg(MSG_W2S_ATTACKCITY_RefreshRegion);
//	msg.Add(lTime);
//	msg.SendAll();
//}
//
////���ǿ�ʼ����ʱ��ʾ��Ϣ
//void CALLBACK CAttackCitySys::OnAttackCityStartInfo(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	long lRegionID = GetAttackCitySys()->GetWarRegionIDByTime(lTime);
//	if(lRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strTempt[256]="";
//	sprintf(strTempt,"��%s������������ս[[[��ս]]]����",pRegion->pRegion->GetName());
//
//	tagTime CurrTime;
//	tagACT Attack = m_Attacks[lTime];
//	if(Attack.AttackCityStartTime <= CurrTime) return;
//	tagTime StartTime = Attack.AttackCityStartTime;
//
//	tagTime DifTime = StartTime.GetTimeDiff(CurrTime);
//	long lDifTime = (DifTime.Minute()*60+DifTime.Second())*1000;
//
//	long lTopInfoID = GetOrganiCtrl()->AddOneTopInfo(2,lDifTime,string(strTempt));
//	GetOrganiCtrl()->SendTopInfoToClient(lTopInfoID,2,lDifTime,string(strTempt));
//}
//
////���ǽ�������ʱ��ʾ��Ϣ
//void CALLBACK CAttackCitySys::OnAttackCityEndnfo(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if(m_Attacks.find(lTime) == m_Attacks.end())	return;
//
//	long lRegionID = GetAttackCitySys()->GetWarRegionIDByTime(lTime);
//	if(lRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strTempt[256]="";
//	sprintf(strTempt,"��%s������������ս[[[����]]]����",pRegion->pRegion->GetName());
//
//	tagTime CurrTime;
//	tagACT Attack = m_Attacks[lTime];
//	if(Attack.AttackCityEndTime <=  CurrTime)	return;
//	tagTime EndTime = Attack.AttackCityEndTime;
//
//	if(Attack.RegionState != CIS_Fight)	return;
//
//	tagTime DifTime = EndTime.GetTimeDiff(CurrTime);
//	long lDifTime = (DifTime.Minute()*60+DifTime.Second())*1000;
//
//	long lTopInfoID = GetOrganiCtrl()->AddOneTopInfo(2,lDifTime,string(strTempt));
//	GetOrganiCtrl()->SendTopInfoToClient(lTopInfoID,2,lDifTime,string(strTempt));
//}
//
//
////�õ�һ�������ĵ�ǰ״̬
//eCityState CAttackCitySys::GetCityState(long lRegionID)
//{
//	tagTime CurrTime;
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.lCityRegionID == lRegionID )
//		{
//			if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.AttackCityEndTime )
//				return (*it).second.RegionState;
//		}
//	}
//	return CIS_NO;
//}
//
////�õ�ս����ʾ��ǰ״̬
//eCityState CAttackCitySys::GetCityStateByWarNum(long lWarTime)
//{
//
//	itAttack it = m_Attacks.find(lWarTime);
//	if(it != m_Attacks.end() )
//		return (*it).second.RegionState;
//	return CIS_NO;
//}
//
////�õ���ǰʱ���һ�������Ĺ��Ǳ�ʾ
//long CAttackCitySys::GetWarNumByRegionIDAndCurTime(long lRegionID)
//{
//	tagTime CurrTime;
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.lCityRegionID == lRegionID )
//		{
//			if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.AttackCityEndTime )
//				return (*it).first;
//		}
//	}
//	return 0;
//}
//
//
////�õ�һ����������ս���
//void CAttackCitySys::GetDecWarFactions(long lRegionID,list<CGUID>& Factions)
//{
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.lCityRegionID == lRegionID )
//		{
//			Factions =  (*it).second.DecWarFactions;
//			return;
//		}
//	}
//}
//
////�õ�һ��ս������ս���
//void CAttackCitySys::GetDecWarFactionsByWarNum(long lWarNum,list<CGUID>& Factions)
//{
//	itAttack it = m_Attacks.find(lWarNum);
//	if( it != m_Attacks.end() )
//	{
//		Factions = (*it).second.DecWarFactions;
//	}
//}
//
////�ж�һ������Ƿ���ս
//bool	CAttackCitySys::IsAlreadyDeclarForWar(long lWarNum,const CGUID& FactionGuid)
//{
//	if(m_Attacks.find(lWarNum) == m_Attacks.end() )	return false;
//	tagACT Attack = m_Attacks[lWarNum];
//	if( find(Attack.DecWarFactions.begin(),Attack.DecWarFactions.end(),FactionGuid) == Attack.DecWarFactions.end() )
//		return false;
//	return true;
//}
//
////�ж�һ������Ƿ���ս
//bool	CAttackCitySys::IsAlreadyDeclarForWar(const CGUID& FactionGuid)
//{
//	list<CGUID>	DecFacs;
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.RegionState != CIS_NO )
//		{
//			DecFacs = (*it).second.DecWarFactions;
//			if( find(DecFacs.begin(),DecFacs.end(),FactionGuid) != DecFacs.end() )
//				return true;
//		}
//	}
//	return false;
//}
//
////�Ƿ��ڵжԹ�ϵ
//bool	CAttackCitySys::IsEnemyRelation(const CGUID& FactionGuid1,const CGUID& FactionGuid2)
//{
//	const CGUID& UnionGuid1 = GetOrganiCtrl()->IsFreeFaction(FactionGuid1);
//	const CGUID& UnionGuid2 = GetOrganiCtrl()->IsFreeFaction(FactionGuid2);
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		//CGame::tagRegion* pRegion = GetGame()->GetRegion( (*it).second.lCityRegionID );
//		CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion((*it).second.lCityRegionID);
//
//		if(pRegion == NULL || pRegion->pRegion == NULL)	continue;
//		if( find((*it).second.DecWarFactions.begin(),(*it).second.DecWarFactions.end(),FactionGuid1 ) != 
//			(*it).second.DecWarFactions.end() )
//		{
//			if(FactionGuid2 == pRegion->pRegion->GetOwnedCityFaction() ||
//				(NULL_GUID != UnionGuid1 && UnionGuid2 ==  pRegion->pRegion->GetOwnedCityUnion()) )
//				return true;
//		}
//		else if( find((*it).second.DecWarFactions.begin(),(*it).second.DecWarFactions.end(),FactionGuid2 ) != 
//			(*it).second.DecWarFactions.end() )
//		{
//			if(FactionGuid1 == pRegion->pRegion->GetOwnedCityFaction() ||
//				(NULL_GUID != UnionGuid1 && UnionGuid1 ==  pRegion->pRegion->GetOwnedCityUnion()) )
//				return true;
//		}
//	}
//	return false;
//}
//
//
////list���Һ���
//bool	CAttackCitySys::FindInList(list<long>& TemptList,long lID)
//{
//	return find(TemptList.begin(),TemptList.end(),lID)  != TemptList.end();
//}
////��һ�������һ������սʱ
//bool	CAttackCitySys::OnPlayerDeclareWar(const CGUID& PlayerGuid,long lWarID,long lMoney)
//{
///*ZKZ20080408
//	const CGUID& FactionGuid = GetOrganiCtrl()->IsFactionMaster(PlayerGuid);
//	if(FactionGuid == NULL_GUID)	return false;
//	COrganizing* pDecWarOrg = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
//	if(pDecWarOrg == NULL)	return false;
//	long lWarRegionID = GetWarRegionIDByTime(lWarID);
//	if(lWarRegionID == 0)	return false;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return false;
//
//	DWORD dwColor = MAKELONG(MAKEWORD(255,255), MAKEWORD(255,255));
//	DWORD dwBkColor = MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255));
//	//�жϰ��ȼ��Ƿ�����Ҫ��
//	if( pDecWarOrg->GetLvl()  < GetOrganizingParam()->GetAttackCityMinLvl() )
//	{
//		GetOrganiCtrl()->SendOrgaInfoToClient(PlayerGuid,string("��İ��û�ﵽ�㹻�ĵȼ������ܲμӹ�������ս��"),string("ϵͳ��ʾ"),-1,dwColor,dwBkColor);
//		return false;
//	}
//
//	//�ж��Ƿ������ս
//	if( GetCityStateByWarNum(lWarID) == CIS_NO )	return false;
//
//	//�ж��Ƿ�ͳ�����ͬ��
//	const CGUID& OwnedFactionGuid = pRegion->pRegion->GetOwnedCityFaction();
//	if(OwnedFactionGuid == FactionGuid)	return false;
//	const CGUID& UnionGuid = GetOrganiCtrl()->IsFreeFaction(FactionGuid);
//	const CGUID& OwnedUnionGuid = GetOrganiCtrl()->IsFreeFaction(OwnedFactionGuid);
//	if(UnionGuid != NULL_GUID && OwnedUnionGuid == UnionGuid)	return false;
//
//	//�ж��Ƿ��Ѿ�ӵ�����ǻ�ݵ�
//	if(pDecWarOrg->GetOwnedCities().size() > 0)	return false;
//
//	//�ж��Ƿ��ѱ����μ���һ����ս
//	if( IsAlreadyDeclarForWar(FactionGuid) == true)	return false;
//	 //�ж��Ƿ��Ѿ������μ���һ���ݵ�����ս
//	if(GetVilWarSys()->IsAlreadyDeclarForWar(FactionGuid) == true) return false;
//	
//	COrganizing* pOwnedFac = NULL;
//	//�õ��������а���б�
//	if(OwnedFactionGuid != NULL_GUID)
//	{	
//		COrganizing* pOwnedFac = GetOrganiCtrl()->GetFactionOrganizing(OwnedFactionGuid);
//	}
//
//	//�����ս����б�
//	m_Attacks[lWarID].DecWarFactions.push_back( pDecWarOrg->GetExID() );	
//	
//	//��������İ���б�
//	vector<BYTE>	vecWarFacData;
//	UpdateApplyWarFacsToGameServer(lWarID,vecWarFacData);
//	CMessage msg(MSG_W2S_CITYWAR_UpdateDecWarFacs);
//	if(vecWarFacData.size() > 0)
//		msg.AddEx(&vecWarFacData[0],vecWarFacData.size());
//	msg.SendAll();
//
//	//����ս֮����°���ս�ж��б�
//	UpdateCityAllFactionEnemyRelation();
//
//
//	char strSend[1024]="�μ��˹�������ս��";
//	BYTE bCountry = pRegion->pRegion->GetCountry();
//	if(bCountry>=0 && bCountry<=4)
//	{		
//		sprintf(strSend,"%s��%s�������μ�%s��������ս��",CountryName[bCountry],
//		pDecWarOrg->GetName().c_str(),CountryName[bCountry]);
//	}
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//			MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//	//���ս��������Ϣ
//	char strWarInfo[1024];
//	sprintf(strWarInfo,"(Num:%d){%s}���������{%s}��ս��.",
//				lWarID,pDecWarOrg->GetName().c_str(),pRegion->pRegion->GetName() );
//	PutWarDebugString(strWarInfo);
//*/
//	return true;
//}
//
////����һ�����Ӯ���˳�ս
//void	CAttackCitySys::OnFacWinCity(long lWarNum,long lWarRegionID,const CGUID& FactionGuid,const CGUID& UnionGuid)
//{
//	char strWarInfo[256];
//	char szFactionGuid[40] = {0};
//	char szUnionGuid[40] = {0};
//	FactionGuid.tostring(szFactionGuid);
//	UnionGuid.tostring(szUnionGuid);
//	sprintf(strWarInfo,
//		"lWarNum:%d,lWarRegionID:%d,FactionGuid:%s,lUnionID:%s",
//		lWarNum,lWarRegionID,szFactionGuid,szUnionGuid);
//	PutWarDebugString(strWarInfo);
//
//	if(lWarNum == 0 || lWarRegionID == 0)	return;
//
//	itAttack it = m_Attacks.find(lWarNum);
//	if( it == m_Attacks.end() )	return;
//	tagACT Attack = (*it).second;
//	if(Attack.lCityRegionID != lWarRegionID)	return;
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//
//	//ɾ����ʱ����ߵĴ���ʱ��
//	GetTimer()->KillTimeEvent(Attack.lEndInfoEventID);
//	GetTimer()->KillTimeEvent(Attack.lEndEventID);
//
//	//���� 
//	OnCityWarEnd(lWarNum);
//	OnCityWarEndToGameServer(lWarRegionID,lWarNum);
//
//	//��һ�γ�սû���κΰ���ʤ
//	if(FactionGuid == NULL_GUID)
//	{	
//		char strSend[256]="";
//		sprintf(strSend,"%s��������ս������",pRegion->pRegion->GetName());
//		GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//			MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//		//��Ӷ�����ʾ��Ϣ
//		char strTopInfo[256]="";
//		sprintf(strTopInfo,"%s������������ս�������˴γ�սû�л�ʤ��ᡣ",pRegion->pRegion->GetName());
//		GetOrganiCtrl()->SendTopInfoToClient(-1,1,2,string(strTopInfo));
//
//		GetOrganiCtrl()->SendOrgaInfoToClient(string(strTopInfo),
//			MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//		//���ս��������Ϣ
//		sprintf(strWarInfo,"(Num:%d){%s}��������ս�������˴�û�л�ʤ����",lWarNum,pRegion->pRegion->GetName());
//		PutWarDebugString(strWarInfo);
//		return;
//	}
//
//	const CGUID& OrgOwnedFactionGuid = pRegion->pRegion->GetOwnedCityFaction();
//	COrganizing* pWinFaction = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
//	if(pWinFaction == NULL)	return;
//
//	const CGUID& TmpUnionGuid = GetOrganiCtrl()->IsFreeFaction(FactionGuid);
//
//	//�Ƿ��Ƿ��ط����ʤ��
//	bool bDefSide = true;
//
//	//������Ƿ�ʤ��
//	if(OrgOwnedFactionGuid != FactionGuid)
//	{
//		bDefSide = false;
//		GetGame()->RefreshOwnedCityOrg(lWarRegionID,FactionGuid,TmpUnionGuid);
//
//		//����ʤ������ӵ�е�����
//		pWinFaction->AddOwnedCity(lWarRegionID);
//
//		// ���øð����Ϊ�ù�����
//		CFaction *pWinFac = (CFaction*)pWinFaction;
//		if(pWinFac)
//		{
//			GetCountryHandler()->GetCountry(pWinFac->GetCountry())->SetKing(pWinFaction->GetMasterID());
//		}
//		//��ԭ���İ��ɾȥӵ�е�����
//		COrganizing* pOwnedFaction =
//			GetOrganiCtrl()->GetFactionOrganizing(OrgOwnedFactionGuid);
//		if(pOwnedFaction)
//		{
//			pOwnedFaction->DelOwnedCity(lWarRegionID);
//		}
//		
//	}
//
//	//ͳ�ư�����а�
//	GetOrganiCtrl()->StatBillboard();
//
//	char strSend[256]="";
//	sprintf(strSend,"%s��������ս������%s�����%s��ս�л�ʤ����ö�%s��˰�չ���Ȩ����",
//		pRegion->pRegion->GetName(),pWinFaction->GetName().c_str(),
//		pRegion->pRegion->GetName(),pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//��Ӷ�����ʾ��Ϣ
//	char strTopInfo[256]="";
//	sprintf(strTopInfo,"%sȡ����%s������������ս������ʤ��",pWinFaction->GetName().c_str(),pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendTopInfoToClient(-1,1,2,string(strTopInfo));
//
//	//���ս��������Ϣ
//	sprintf(strWarInfo,"(Num:%d){%s}�����{%s}��ս�л�ʤ����ö�{%s}�ǵ�˰�չ���Ȩ����",lWarNum,
//		pWinFaction->GetName().c_str(),pRegion->pRegion->GetName(),pRegion->pRegion->GetName());
//	PutWarDebugString(strWarInfo);
//}
//
////�������е���ս��ᵽGameServer
//void CAttackCitySys::UpdateAllDecFacToGameServer()
//{
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		//CGame::tagRegion* pRegion = GetGame()->GetRegion( (*it).second.lCityRegionID );
//		CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion((*it).second.lCityRegionID);
//		if(pRegion == NULL || pRegion->pRegion == NULL)	continue;
//
//		//��������İ���б�
//		vector<BYTE>	vecWarFacData;
//		UpdateApplyWarFacsToGameServer((*it).first,vecWarFacData);
//		CMessage msg(MSG_W2S_CITYWAR_UpdateDecWarFacs);
//		if(vecWarFacData.size() > 0)
//			msg.AddEx(&vecWarFacData[0],vecWarFacData.size());
//		msg.SendAll();
//	}
//}
//
////������ս������°��ĵжԹ�ϵ
//void	CAttackCitySys::InitialCityAllFactionEnemyRelation()
//{
//	//��������еĳ�ս�ж԰��
//	GetOrganiCtrl()->ClearAllCityEneFacRelation();
//
//	list<CGUID>	DecFacs;
//	//��ӳ�ս�ж԰��
//	itAttack it = m_Attacks.begin();
//	for(;it != m_Attacks.end();it++)
//	{
//		if( (*it).second.RegionState == CIS_NO) continue;
//	
//		DecFacs = (*it).second.DecWarFactions;
//		SetCityWarEnemyFactions((*it).second.lCityRegionID,DecFacs);
//		//��������
//	}
//}
////������ս������°��ĵжԹ�ϵ
//void CAttackCitySys::UpdateCityAllFactionEnemyRelation()
//{
//	//�������а��ĳ�ս�ж԰��ı�־Ϊfalse
//	GetOrganiCtrl()->SetAllCityFacEnemyChanged(false);
//	
//	InitialCityAllFactionEnemyRelation();
//
//	//���������б仯�˵ĳ�ս�жԹ�ϵ�б�
//	GetOrganiCtrl()->UpdateAllCityEneFacRelation();
//}
//
////����װ�ص���ս������õжԹ�ϵ
//void	CAttackCitySys::SetCityWarEnemyFactions(long lRegionID,list<CGUID>& DecWarFactions)
//{
//	if(DecWarFactions.size() == 0)	return;
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion( lRegionID );
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);
//	if( pRegion == NULL || pRegion->pRegion == NULL )	return;
//	const CGUID& OwnedFaction = pRegion->pRegion->GetOwnedCityFaction();
//	if( OwnedFaction == NULL_GUID )	return;
//
//	//�õ�������
//	list<COrganizing*>	OwnedFactiosList;OwnedFactiosList.clear();
//	const CGUID& OwnedUnionID = GetOrganiCtrl()->IsFreeFaction(OwnedFaction);
//	if( OwnedUnionID == NULL_GUID )
//	{
//		COrganizing* pOwnedFaction = GetOrganiCtrl()->GetFactionOrganizing(OwnedFaction);
//		if(pOwnedFaction)
//			OwnedFactiosList.push_back(pOwnedFaction);
//	}
//	else
//	{
//		COrganizing* pOwnedUnion = GetOrganiCtrl()->GetUnionOrganizing(OwnedUnionID);
//		if(pOwnedUnion)
//			pOwnedUnion->GetMemberList(OwnedFactiosList);
//	}
//
//	//�õ���ս�����г�Ա
//	list<COrganizing*>	DecWarFactionList;DecWarFactionList.clear();
//	list<CGUID>::iterator it = DecWarFactions.begin();
//	for(; it != DecWarFactions.end();it++)
//	{
//		const CGUID& FactionGuid = (*it);
//		const CGUID& UnionID = GetOrganiCtrl()->IsFreeFaction(FactionGuid);
//		if(UnionID == NULL_GUID )
//		{
//			COrganizing* pFaction = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
//			if(pFaction)
//				DecWarFactionList.push_back(pFaction);
//		}
//		else
//		{
//			COrganizing* pUnion = GetOrganiCtrl()->GetUnionOrganizing(UnionID);
//			if(pUnion)
//				pUnion->GetMemberList(DecWarFactionList);
//		}
//	}
//
//	//��ụ�ӳ�ս�ж��б�
//	list<COrganizing*>::iterator itDec = DecWarFactionList.begin();
//	for(;itDec != DecWarFactionList.end();itDec++)
//	{
//		COrganizing* pDecOrg = (*itDec);
//		list<COrganizing*>::iterator itOwned= OwnedFactiosList.begin();
//		for(;itOwned != OwnedFactiosList.end();itOwned++)
//		{
//			COrganizing* pOwnedOrg = (*itOwned);		
//			pDecOrg->AddCityWarEnemyOrganizing(pOwnedOrg->GetExID());
//			pOwnedOrg->AddCityWarEnemyOrganizing(pDecOrg->GetExID());
//		}
//	}
//}
//
//CAttackCitySys* GetAttackCitySys()
//{
//	return CAttackCitySys::getInstance();
//}