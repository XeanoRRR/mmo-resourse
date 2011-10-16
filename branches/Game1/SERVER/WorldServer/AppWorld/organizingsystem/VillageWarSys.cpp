///////////////////////////////////////////////////////////////////////////////////////
//VillageWarSys.cpp
//��սϵͳ
//Function:��ɰ�սϵͳ����ع���
//Author:Joe
//Create Time:2005.7.13
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\villagewarsys.h"
#include ".\attackcitysys.h"
#include "..\nets\networld\message.h"
#include "OrganizingCtrl.h"
#include "..\WorldRegion.h"
#include "..\Player.h"
#include "Organizing.h"
#include "organizingparam.h"
//#include "..\dbaccess\worlddb\RsVillageWar.h"
#include "../GlobalRgnManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
//
//CVillageWarSys* CVillageWarSys::instance = NULL;
//
//CVillageWarSys::CVillageWarSys(void)
//{
//	m_VillageWars.clear();
//}
//
//CVillageWarSys::~CVillageWarSys(void)
//{
//}
//
////�õ�Ψһʵ�����
//CVillageWarSys* CVillageWarSys::getInstance()
//{
//	if(instance == NULL)
//	{
//		instance = new CVillageWarSys;
//	}
//	return instance;
//}
////��ʼ��
//bool   CVillageWarSys::Initialize()
//{
//	m_VillageWars.clear();
//	return true;
//
//	char filename[]="setup/VillageWarSys.ini";	
//	ifstream stream;
//	stream.open(filename);
//	if( !stream.is_open() )
//	{
//		char str[]="setup/VillageWarSys.ini can't found!";
//		AddLogText(str);
//		return false;
//	}
//
//	tagTime CurrTime;
//
//	string strDate;
//	long lTime = 0;
//	map<long,tagTime> LastEndWarTime;LastEndWarTime.clear();
//	while(ReadTo(stream, "*"))
//	{
//		lTime++;
//		tagVilWarSetup VilWarSetup;
//		long lStartInfoTime,lDeclareTime,lEndInfoTime,lLastTime,lClearTime;
//		tagTime	StartTime,EndTime;
//		stream>>VilWarSetup.lWarRegionID
//			>>strDate
//			>>lStartInfoTime
//			>>lDeclareTime
//			>>lEndInfoTime
//			>>lLastTime
//			>>lClearTime
//			>>VilWarSetup.lVilRegionID;
//        
//		VilWarSetup.lID = lTime;
//		VilWarSetup.RegionState = CIS_NO;
//		VilWarSetup.DecWarFactions.clear();
//		tagTime Time(strDate.c_str());
//		VilWarSetup.WarStartTime = Time;
//
//		//��վ��ʼ��ʾ��Ϣʱ��
//		tagTime TemptTime = Time;
//		TemptTime.AddMinute(lStartInfoTime);
//		VilWarSetup.WarStartInfoTime = TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lDeclareTime);
//		VilWarSetup.DeclarWarTime = TemptTime;
//
//		//��վ������ʾʱ��
//		TemptTime = Time;
//		TemptTime.AddMinute(lEndInfoTime);
//		VilWarSetup.WarEndInfoTime = TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lLastTime);
//		VilWarSetup.WarEndTime = TemptTime;
//
//		TemptTime = Time;
//		TemptTime.AddMinute(lClearTime);
//		VilWarSetup.ClearPlayerTime = TemptTime;
//	
//		//������
//		if( VilWarSetup.DeclarWarTime >= VilWarSetup.WarStartTime ||
//			VilWarSetup.DeclarWarTime >= VilWarSetup.WarEndTime )
//		{
//			AddLogText("Load VillageWarSys.ini Error!");
//			return false;
//		}
//		if( VilWarSetup.WarStartTime>=VilWarSetup.WarEndTime )
//		{
//			AddLogText("Load VillageWarSys.ini Error!");
//			return false;
//		}
//
//		if(VilWarSetup.WarStartInfoTime >= VilWarSetup.WarStartTime)
//		{
//			AddLogText("Load VillageWarSys.ini Error!");
//			return false;
//		}
//		if(VilWarSetup.WarEndInfoTime >= VilWarSetup.WarEndTime)
//		{
//			AddLogText("Load VillageWarSys.ini Error!");
//			return false;
//		}
//
//		map<long,tagTime>::iterator itEndTime = LastEndWarTime.find( VilWarSetup.lWarRegionID );
//		if( itEndTime  != LastEndWarTime.end() )
//		{
//			if( VilWarSetup.WarStartTime <= (*itEndTime).second )
//			{
//				AddLogText("Load VillageWarSys.ini Error!");
//				return false;
//			}
//		}
//		LastEndWarTime[VilWarSetup.lWarRegionID] = VilWarSetup.WarEndTime;
//		
//		m_VillageWars[lTime] = VilWarSetup;
//	}
//	stream.close();
//
//	//ע���¼�
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		long lTemptTime = (*it).first;
//		if((*it).second.WarEndTime > CurrTime)
//		{
//			//ע�������ʾʱ��
//			if( (*it).second.WarEndInfoTime >= CurrTime )
//			{
//				(*it).second.lEndInfoEventID = GetTimer()->SetTimeEvent((*it).second.WarEndInfoTime,
//				(CallBackFun)CVillageWarSys::OnAttackVillageEndInfo,lTemptTime);
//			}
//			else
//			{
//				(*it).second.lEndInfoEventID = GetTimer()->SetTimeEvent(CurrTime,
//				(CallBackFun)CVillageWarSys::OnAttackVillageEndInfo,lTemptTime);
//			}
//
//
//			//ע��ʱ���¼�
//			(*it).second.lEndEventID =GetTimer()->SetTimeEvent((*it).second.WarEndTime,
//				(CallBackFun)CVillageWarSys::OnAttackVillageEnd,lTemptTime);
//			if((*it).second.WarStartTime > CurrTime)
//			{
//				(*it).second.lClearEventID = GetTimer()->SetTimeEvent((*it).second.ClearPlayerTime,
//					(CallBackFun)CVillageWarSys::OnClearPlayer,lTemptTime);
//
//				(*it).second.lStartEventID =GetTimer()->SetTimeEvent((*it).second.WarStartTime,
//
//				(CallBackFun)CVillageWarSys::OnAttackVillageStart,lTemptTime);
//				if((*it).second.DeclarWarTime > CurrTime)
//				{
//					(*it).second.lDeclarWarEventID =GetTimer()->SetTimeEvent((*it).second.DeclarWarTime,
//					(CallBackFun)CVillageWarSys::OnDelcareWar,lTemptTime);
//				}
//				else
//				{
//					(*it).second.RegionState = CIS_DUTH;//��ս״̬
//				}
//
//				
//				//ע�Ὺʼ��ʾʱ��
//				if( (*it).second.WarStartInfoTime >= CurrTime )
//				{
//					(*it).second.lStartInfoEventID = GetTimer()->SetTimeEvent((*it).second.WarStartInfoTime,
//					(CallBackFun)CVillageWarSys::OnAttackVillageStartInfo,lTemptTime);
//				}
//				else
//				{
//					(*it).second.lStartInfoEventID = GetTimer()->SetTimeEvent(CurrTime,
//					(CallBackFun)CVillageWarSys::OnAttackVillageStartInfo,lTemptTime);
//				}
//			}
//			else
//			{
//				(*it).second.RegionState = CIS_Fight;	//ս��״̬
//			}
//		}
//	}
//	return true;
//}
//
//
////����ս�ʹ��ӵ�ͼ�Ƿ���һ����������
//bool   CVillageWarSys::CheckSetup()
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		//CGame::tagRegion* pVilRegion = GetGame()->GetRegion((*it).second.lVilRegionID);
//		CGlobalRgnManager::tagRegion* pVilRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion((*it).second.lVilRegionID);
//		//CGame::tagRegion* pWarRegion = GetGame()->GetRegion((*it).second.lWarRegionID);
//		CGlobalRgnManager::tagRegion* pWarRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion((*it).second.lWarRegionID);
//		if(pVilRegion)
//		{
//			if(pWarRegion == NULL || pVilRegion->dwGameServerIndex != pWarRegion->dwGameServerIndex)
//			{
//				AddLogText("װ�� VillageWarSys.ini Error!���Ӻʹ�ս��ͼ����һ���������ϡ�");
//					return false;
//			}
//		}
//	}
//	return true;
//}
//
////list���Һ���
//bool	CVillageWarSys::FindInList(list<long>& TemptList,long lID)
//{
//	return find(TemptList.begin(),TemptList.end(),lID)  != TemptList.end();
//}
//
//void	CVillageWarSys::SetDecWarFactionsFromDB(long lRegionID,tagTime& WarStartTime,list<CGUID>& DecWarFactions)
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if( (*it).second.lWarRegionID == lRegionID && WarStartTime == (*it).second.WarStartTime )
//		{
//			(*it).second.DecWarFactions = DecWarFactions;
//			return;
//		}
//	}
//}
//
////��ӵ�CByteArray
//bool CVillageWarSys::AddToByteArray(vector<BYTE>& ByteArray)
//{
//	_AddToByteArray(&ByteArray,(long)m_VillageWars.size());
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		tagVilWarSetup &VilWarSetup = (*it).second;
//		_AddToByteArray( &ByteArray,&((*it).second), 
//			sizeof(tagVilWarSetup)-sizeof(list<CGUID>) );												   
//		//�����ս����б�
//		_AddToByteArray(&ByteArray,(long)VilWarSetup.DecWarFactions.size());
//		list<CGUID>::iterator itFac= VilWarSetup.DecWarFactions.begin();
//		for(;itFac != VilWarSetup.DecWarFactions.end();itFac++)
//		{
//			_AddToByteArray(&ByteArray,(*itFac));
//		}
//	}
//	return true;
//}
//
////�����ս��ᵽCByteArray
//bool CVillageWarSys::UpdateApplyWarFacsToGameServer(long lWarID,vector<BYTE>& ByteArray)
//{
//	list<CGUID>	DecFacs;
//	itVelWar it = m_VillageWars.find(lWarID);
//	if(it != m_VillageWars.end())
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
//void CVillageWarSys::GenerateSaveData()
//{
//	list<tagVilWarSetup*> TemptVilWarSetupList;
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		tagVilWarSetup* pSetup = new tagVilWarSetup;
//		pSetup->lWarRegionID = (*it).second.lWarRegionID;
//		pSetup->WarStartTime = (*it).second.WarStartTime;
//		pSetup->DecWarFactions = (*it).second.DecWarFactions;
//		TemptVilWarSetupList.push_back(pSetup);
//	}
////	GetGame()->SetVillageWarParam(TemptVilWarSetupList);
//}
//
////����
//bool   CVillageWarSys::ReLoad()
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		tagVilWarSetup&	VilWarTime=(*it).second;
//		GetTimer()->KillTimeEvent(VilWarTime.lDeclarWarEventID);
//		GetTimer()->KillTimeEvent(VilWarTime.lStartInfoEventID);
//		GetTimer()->KillTimeEvent(VilWarTime.lStartEventID);
//		GetTimer()->KillTimeEvent(VilWarTime.lEndInfoEventID);
//		GetTimer()->KillTimeEvent(VilWarTime.lEndEventID);
//		GetTimer()->KillTimeEvent(VilWarTime.lClearEventID);
//	}
//
//	map<long,tagVilWarSetup> TemptVillageWars = m_VillageWars;
//	itVelWar itVil = TemptVillageWars.begin();
//	for(; itVil != TemptVillageWars.end();itVil++)
//	{
//		if( (*itVil).second.RegionState != CIS_NO )
//		{
//			OnVillageWarEnd((*itVil).first,(*itVil).second.lWarRegionID);
//		}
//	}
//	Initialize();
//	return true;
//}
//void   CVillageWarSys::Release()
//{
//	if(instance)
//	{
//		SAFE_DELETE(instance);
//	}
//}
//
//
//long	CVillageWarSys::GetWarRegionIDByTime(long lTime)
//{
//	itVelWar it = m_VillageWars.find(lTime);
//	if(it == m_VillageWars.end())	return 0;
//	return (*it).second.lWarRegionID;
//}
//
////�õ�ĳ�ε�ս���Ĳ���
//bool	CVillageWarSys::GetVilWarSetupByTime(tagVilWarSetup& VilWarSetup,long lTime)
//{
//	itVelWar it = m_VillageWars.find(lTime);
//	if(it == m_VillageWars.end())	return false;
//	VilWarSetup = (*it).second;
//	return true;
//}
//
//long	CVillageWarSys::GetWarTimeByRegionID(long lRegionID)
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if((*it).second.lWarRegionID == lRegionID &&  (*it).second.RegionState == CIS_Fight )
//		{
//			return (*it).first;
//		}
//	}
//	return 0;
//}
//
////��ս��ʼ
//void CALLBACK CVillageWarSys::OnDelcareWar(LPARAM lparam)
//{
//	//�����߼�
//	long lTime = (long)lparam;
//	if( GetVilWarSys()->IsExitWarID(lTime) == false)	return;	
//	GetVilWarSys()->SetVilWarState(lTime,CIS_DUTH);
//
//	CMessage msg(MSG_W2S_VILLAGE_DeclareWar);
//	msg.Add(lTime);
//	msg.SendAll();
//
//	//���������Ϣ
//	long lWarRegionID = GetVilWarSys()->GetWarRegionIDByTime(lTime);
//	if(lWarRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strSend[256]="";
//	sprintf(strSend,"%s�ݵ�����ս�ѿ�ʼ������",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//}
//
////���ǿ�ʼ
//void CALLBACK CVillageWarSys::OnAttackVillageStart(LPARAM lparam)
//{
//	//�����߼�
//	long lTime = (long)lparam;
//	if( GetVilWarSys()->IsExitWarID(lTime) == false)	return;	
//	
//	GetVilWarSys()->SetVilWarState(lTime,CIS_Fight);
//	CMessage msg(MSG_W2S_VILLAGE_WarStart);
//	msg.Add(lTime);
//	msg.SendAll();	
//
//	long lWarRegionID = GetVilWarSys()->GetWarRegionIDByTime(lTime);
//	if(lWarRegionID == 0)	return;
////	CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	char strSend[256]="";
//	sprintf(strSend,"%s�ݵ�����ս�ѿ�ʼ��",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//}
////���ǽ���
//void CALLBACK CVillageWarSys::OnAttackVillageEnd(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if( GetVilWarSys()->IsExitWarID(lTime) == false)	return;		
//
//	//GetVilWarSys()->OnVillageWarEnd(lTime,lWarRegionID);
//	GetVilWarSys()->SetVilWarState(lTime,CIS_NO);
//	CMessage msg(MSG_W2S_VILLAGE_TimeOut);
//	msg.Add(lTime);
//	msg.SendAll();	
//
//	long lWarRegionID = GetVilWarSys()->GetWarRegionIDByTime(lTime);
//	if(lWarRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//
//	char strSend[256]="";
//	sprintf(strSend,"ʱ���ѵ���%s�ݵ�����ս������",pRegion->pRegion->GetName());
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//}
//
////���������Ա
//void CALLBACK CVillageWarSys::OnClearPlayer(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	if( GetVilWarSys()->IsExitWarID(lTime) == false)	return;
//
//	CMessage msg(MSG_W2S_AVILLAGE_ClearPlayer);
//	msg.Add(lTime);
//	msg.SendAll();
//}
//
////���忪ʼ��Ϣ
//void CALLBACK CVillageWarSys::OnAttackVillageStartInfo(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	long lWarRegionID = GetVilWarSys()->GetWarRegionIDByTime(lTime);
//	if(lWarRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	tagVilWarSetup VilWar;
//	if(GetVilWarSys()->GetVilWarSetupByTime(VilWar,lTime) == false) return;
//
//	char strTempt[256]="";
//	sprintf(strTempt,"��%s�����ݵ�����ս[[[��ս]]]����",pRegion->pRegion->GetName());
//
//	tagTime CurrTime;
//	if(VilWar.WarStartTime <= CurrTime) return;
//	tagTime StartTime = VilWar.WarStartTime;
//
//	tagTime DifTime = StartTime.GetTimeDiff(CurrTime);
//	long lDifTime = (DifTime.Minute()*60+DifTime.Second())*1000;
//
//	long lTopInfoID = GetOrganiCtrl()->AddOneTopInfo(2,lDifTime,string(strTempt));
//	GetOrganiCtrl()->SendTopInfoToClient(lTopInfoID,2,lDifTime,string(strTempt));
//}
////���������Ϣ
//void CALLBACK CVillageWarSys::OnAttackVillageEndInfo(LPARAM lparam)
//{
//	long lTime = (long)lparam;
//	long lWarRegionID = GetVilWarSys()->GetWarRegionIDByTime(lTime);
//	if(lWarRegionID == 0)	return;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	tagVilWarSetup VilWar;
//	if(GetVilWarSys()->GetVilWarSetupByTime(VilWar,lTime) == false) return;
//
//	char strTempt[256]="";
//	sprintf(strTempt,"��%s�����ݵ�����ս[[[����]]]����",pRegion->pRegion->GetName());
//
//	tagTime CurrTime;
//	if(VilWar.WarEndTime <= CurrTime) return;
//	tagTime EndTime = VilWar.WarEndTime;
//
//	tagTime DifTime = EndTime.GetTimeDiff(CurrTime);
//	long lDifTime = (DifTime.Minute()*60+DifTime.Second())*1000;
//
//	long lTopInfoID = GetOrganiCtrl()->AddOneTopInfo(2,lDifTime,string(strTempt));
//	GetOrganiCtrl()->SendTopInfoToClient(lTopInfoID,2,lDifTime,string(strTempt));
//}
//
////�õ��ó����Ƿ�������뱨��
//long	CVillageWarSys::IsApplyForWarByRegionID(long lRegionID)
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if((*it).second.lWarRegionID == lRegionID && ( (*it).second.RegionState == CIS_DUTH || (*it).second.RegionState == CIS_Fight) )
//		{
//			return (*it).first;
//		}
//	}
//	return 0;
//}
//
////�õ�һ�������ĵ�ǰ״̬
//eCityState CVillageWarSys::GetRegionState(long lRegionID)
//{
//	tagTime CurrTime;
//
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if((*it).second.lWarRegionID == lRegionID || (*it).second.lVilRegionID == lRegionID)
//		{
//			if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.WarEndTime )
//				return (*it).second.RegionState;
//		}
//	}
//	return CIS_NO;
//}
//
////�õ�ս����ʾ��ǰ״̬
//eCityState CVillageWarSys::GetCityStateByWarNum(long lWarTime)
//{
//
//	itVelWar it = m_VillageWars.find(lWarTime);
//	if(it != m_VillageWars.end() )
//		return (*it).second.RegionState;
//	return CIS_NO;
//}
//
////�ж��Ƿ�������WarID
//bool	CVillageWarSys::IsExitWarID(long lWarID)
//{
//	if(m_VillageWars.find(lWarID) != m_VillageWars.end())
//		return true;
//	return false;
//}
//
////�õ���ǰʱ���һ�������Ĺ��Ǳ�ʾ
//long CVillageWarSys::GetWarNumByRegionIDAndCurTime(long lRegionID)
//{
//	tagTime CurrTime;
//
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if((*it).second.lWarRegionID == lRegionID || (*it).second.lVilRegionID == lRegionID)
//		{
//			if( CurrTime >= (*it).second.DeclarWarTime && CurrTime <= (*it).second.WarEndTime )
//				return (*it).first;
//		}
//	}
//	return -1;
//}
//
////�õ�һ����������ս���
//void CVillageWarSys::GetDecWarFactions(long lRegionID,list<CGUID>& Factions)
//{
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if((*it).second.lWarRegionID == lRegionID )
//		{
//			Factions =  (*it).second.DecWarFactions;
//			return;
//		}
//	}
//}
//
////�õ�һ��ս������ս���
//void CVillageWarSys::GetDecWarFactionsByWarNum(long lWarNum,list<CGUID>& Factions)
//{
//	itVelWar it = m_VillageWars.find(lWarNum);
//	if( it != m_VillageWars.end() )
//	{
//		Factions = (*it).second.DecWarFactions;
//	}
//}
//
////����������ݵ�����ս
//bool CVillageWarSys::ApplyForVillageWar(const CGUID& PlayerGuid,long lWarID,long lMoney)
//{
//	/*ZKZ20080408
//	itVelWar it = m_VillageWars.find(lWarID);
//	if( it == m_VillageWars.end() )	return false;
//	tagVilWarSetup VilWarSetup = (*it).second;
//
//	const CGUID& FactionGuid = GetOrganiCtrl()->IsFactionMaster(PlayerGuid);
//	if(FactionGuid == NULL_GUID)	return false;
//	COrganizing* pDecWarOrg = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
//	if(pDecWarOrg == NULL)	return false;
//
//	long lWarRegionID = VilWarSetup.lWarRegionID;
//	if(lWarRegionID == 0)	return false;
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return false;
//	//CGame::tagRegion* pVillage = GetGame()->GetRegion(VilWarSetup.lVilRegionID);
//	CGlobalRgnManager::tagRegion* pVillage = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(VilWarSetup.lVilRegionID);
//	if(pVillage == NULL || pVillage->pRegion == NULL)	return false;
//
//	DWORD dwColor = MAKELONG(MAKEWORD(255,255), MAKEWORD(255,255));
//	DWORD dwBkColor = MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255));
//	//�жϰ��ȼ��Ƿ�����Ҫ��
//	if( pDecWarOrg->GetLvl()  < GetOrganizingParam()->GetAttackVillageMinLvl() )
//	{
//		GetOrganiCtrl()->SendOrgaInfoToClient(PlayerGuid,string("��İ��û�ﵽ�㹻�ĵȼ������ܲξݵ�����ս��"),string("ϵͳ��ʾ"),-1,dwColor,dwBkColor);
//		return false;
//	}
//
//	//�жϸô��Ƿ���Ա���
//	if( GetCityStateByWarNum(lWarID) == CIS_NO )	return false;
//
//	//�ж��Ƿ�ͳ�����ͬ��
//	const CGUID& OwnedFactionID = pVillage->pRegion->GetOwnedCityFaction();
//	if(OwnedFactionID == FactionGuid)	return false;
//	const CGUID& UnionID = GetOrganiCtrl()->IsFreeFaction(FactionGuid);
//	const CGUID& OwnedUnionID = GetOrganiCtrl()->IsFreeFaction(OwnedFactionID);
//	if(UnionID != NULL_GUID && OwnedUnionID == UnionID)	return false;
//
//	//�ж��Ƿ��Ѿ�ӵ�����ǻ�ݵ�
//	if(pDecWarOrg->GetOwnedCities().size() > 0)	return false;
//	
//	//�ж��Ƿ��ѱ����μ���һ���ݵ�����ս
//	if( IsAlreadyDeclarForWar(FactionGuid) == true)	return false;
//	//�ж��Ƿ����μ���һ����ս
//	if(GetAttackCitySys()->IsAlreadyDeclarForWar(FactionGuid) == true) return false;
//
//	//����ս���
//	m_VillageWars[lWarID].DecWarFactions.push_back(FactionGuid);
//
//	//��������İ���б�
//	vector<BYTE>	vecWarFacData;
//	UpdateApplyWarFacsToGameServer(lWarID,vecWarFacData);
//	CMessage msg(MSG_W2S_VIILWAR_UpdateDecWarFacs);
//	if(vecWarFacData.size() > 0)
//		msg.AddEx(&vecWarFacData[0],vecWarFacData.size());
//	msg.SendAll();
//
//	char strSend[500]="";
//	sprintf(strSend,"%s��ᱨ���μ���%s�ݵ�����ս.",
//					pDecWarOrg->GetName().c_str(),pRegion->pRegion->GetName() );
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//		*/
//	return true;
//}
//
////��һ�����Ӯ���˴�ս
//void	CVillageWarSys::OnFacWinVillage(long lWarNum,long lWarRegionID,const CGUID& FactionGuid,const CGUID& UnionGuid)
//{
//	char strWarInfo[256]="";
//	if(lWarNum == 0)
//		return;
//
//	itVelWar it = m_VillageWars.find(lWarNum);
//	if( it == m_VillageWars.end() )	return;
//
//	//ɾ����ʱ��
//	tagVilWarSetup VilWarSetup = (*it).second;
//	GetTimer()->KillTimeEvent(VilWarSetup.lEndInfoEventID);
//	GetTimer()->KillTimeEvent(VilWarSetup.lEndEventID);	
//
//	//CGame::tagRegion* pRegion = GetGame()->GetRegion(lWarRegionID);
//	CGlobalRgnManager::tagRegion* pRegion = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(lWarRegionID);
//	if(pRegion == NULL || pRegion->pRegion == NULL)	return;
//	//CGame::tagRegion* pVillage = GetGame()->GetRegion(VilWarSetup.lVilRegionID);
//	CGlobalRgnManager::tagRegion* pVillage = GetGame()->GetGlobalRgnManager()->GetTemplateNormalRegion(VilWarSetup.lVilRegionID);
//
//	if(pVillage == NULL || pVillage->pRegion == NULL)	return;
//
//	const CGUID& OwnedFactionID = pVillage->pRegion->GetOwnedCityFaction();
//	COrganizing* pOwnedFaction = NULL;
//	if(OwnedFactionID != NULL_GUID)
//		pOwnedFaction = GetOrganiCtrl()->GetFactionOrganizing(OwnedFactionID);
//
//	//��ս����
//	OnVillageWarEnd(lWarNum,lWarRegionID);  
//
//	//��Ӷ�����ʾ��Ϣ
//	char strTopInfo[256]="";
//	//û���κΰ���ʤ
//	if(FactionGuid == NULL_GUID)
//	{
//		if(pOwnedFaction != NULL)
//		{
//			pOwnedFaction->DelOwnedCity(VilWarSetup.lVilRegionID);
//			sprintf(strWarInfo,"ɾ��%s���ӵ�еľݵ�%s.",pOwnedFaction->GetName().c_str(),pVillage->pRegion->GetName());
//			PutWarDebugString(strWarInfo);
//		}
//	
//		sprintf(strTopInfo,"%s�ݵ�����ս�������˴�û�л�ʤ����",pRegion->pRegion->GetName());
//		GetOrganiCtrl()->SendOrgaInfoToClient(string(strTopInfo),
//			MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//	}
//	else
//	{
//		COrganizing* pWinFaction = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
//		if(pWinFaction == NULL)	return;
//		//�쿴�������Ƿ�����ս�б���
//		
//		const CGUID& UnionGuid2 = GetOrganiCtrl()->IsFreeFaction(FactionGuid);
//
//		//�����ʤ�İ�᲻��ԭ��ӵ�еİ��
//		if(pWinFaction->GetExID() != OwnedFactionID)
//		{
//			GetGame()->RefreshOwnedCityOrg(VilWarSetup.lVilRegionID,FactionGuid,UnionGuid2);
//			pWinFaction->AddOwnedCity(VilWarSetup.lVilRegionID);
//
//			//���ս��������Ϣ
//			sprintf(strWarInfo,"��%s�������µľݵ�%s.",pWinFaction->GetName().c_str(),pVillage->pRegion->GetName());
//			PutWarDebugString(strWarInfo);
//
//			if(pOwnedFaction != NULL)
//			{
//				pOwnedFaction->DelOwnedCity(VilWarSetup.lVilRegionID);
//				sprintf(strWarInfo,"ɾ��%s���ӵ�еľݵ�%s.",pOwnedFaction->GetName().c_str(),pVillage->pRegion->GetName());
//				PutWarDebugString(strWarInfo);
//			}
//		}
//		//��Ӿݵ�����սʤ������
//		pWinFaction->AddVillageWarVictorCounts();
//
//		char strSend[256]="";
//		sprintf(strSend,"%s�����%s�ݵ������л�ʤ����ö�%s��˰�չ���Ȩ����",
//			pWinFaction->GetName().c_str(),pRegion->pRegion->GetName(),pVillage->pRegion->GetName());
//		GetOrganiCtrl()->SendOrgaInfoToClient(string(strSend),
//			MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//		//���ս��������Ϣ
//		PutWarDebugString(strSend);
//
//		sprintf(strTopInfo,"%sȡ����%s�����ݵ�����ս������ʤ��",
//			pWinFaction->GetName().c_str(),pRegion->pRegion->GetName());
//	}
//	//��Ӷ�����ʾ��Ϣ
//	GetOrganiCtrl()->SendTopInfoToClient(-1,1,2,string(strTopInfo));
//}
//
////�ݵ�����ս����
//void	CVillageWarSys::OnVillageWarEnd(long lWarNum,long lWarRegionID)
//{
//	itVelWar it = m_VillageWars.find(lWarNum);
//	if(it == m_VillageWars.end())	return;
//
//	CMessage msg(MSG_W2S_VILLAGE_WarEnd);
//	msg.Add(lWarNum);
//	msg.SendAll();
//
//	(*it).second.DecWarFactions.clear();
//}
//
////����ĳ��ս����״̬
//void CVillageWarSys::SetVilWarState(long lTime,eCityState CityState)
//{
//	itVelWar it = m_VillageWars.find(lTime);
//	if(it == m_VillageWars.end())	return;
//	(*it).second.RegionState = CityState;
//}
//
////�ж�һ������Ƿ���ս
//bool	CVillageWarSys::IsAlreadyDeclarForWar(long lWarNum,const CGUID& FactionGuid)
//{
//	if(m_VillageWars.find(lWarNum) == m_VillageWars.end() )	return false;
//	tagVilWarSetup VilWarSetup = m_VillageWars[lWarNum];
//	if( find(VilWarSetup.DecWarFactions.begin(),VilWarSetup.DecWarFactions.end(),FactionGuid) ==
//														VilWarSetup.DecWarFactions.end() )
//		return false;
//	return true;
//}
//
////�ж���һ���壬���ҹ��������ҵ�ʱ��
//bool CVillageWarSys::IsVilRegionLeftClearTime(long lRegionID)
//{
//	tagTime CurrTime;
//
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
//	{
//		if( (*it).second.lVilRegionID == lRegionID)
//		{
//			//�ж�ʱ��
//			if(CurrTime >= (*it).second.ClearPlayerTime &&
//				CurrTime < (*it).second.WarEndTime)
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
////�ж�һ������Ƿ���ս
//bool	CVillageWarSys::IsAlreadyDeclarForWar(const CGUID& FactionGuid)
//{
//	list<CGUID>	DecFacs;
//	itVelWar it = m_VillageWars.begin();
//	for(;it != m_VillageWars.end();it++)
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
//CVillageWarSys* GetVilWarSys()
//{
//	return CVillageWarSys::getInstance();
//}
