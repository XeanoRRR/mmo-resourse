//////////////////////////////////////////////////////////////////////////
//FactionWarSys.cpp
//Function:��սϵͳ��ع���
//Author:Joe
//Create Time:2005.6.27
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\factionwarsys.h"
#include "OrganizingCtrl.h"
#include "Faction.h"
#include "Union.h"
#include "AttackCitySys.h"
#include "..\public\Timer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//CFactionWarSys*		CFactionWarSys::pInstance = NULL;
//
//CFactionWarSys::CFactionWarSys(void)
//{
//	m_dwStartTime = timeGetTime();
//	m_FactionWars.clear();
//}
//
//CFactionWarSys::~CFactionWarSys(void)
//{
//}
//
//CFactionWarSys* CFactionWarSys::GetInstance()
//{
//	if(pInstance == NULL)
//	{
//		pInstance = new CFactionWarSys;
//	}
//	return pInstance;
//}
//
//void CFactionWarSys::Release()
//{
//	if(pInstance)
//	{
//		SAFE_DELETE(pInstance);
//	}
//}
//
//bool   CFactionWarSys::Initialize()
//{
//	AddLogText("���Ͷ�ȡ�ж԰���б���Ϣ...");
//	//	GetGame()->GetRsEnemyFactions()->LoadAllEnemyFactions();
//	GetGame()->SendLoadAllEnemyFactions();
//	//	AddLogText("�ɹ�װ�صж԰���б�...");
//
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();it++)
//	{
//		GetOrganiCtrl()->SetEnemyFactionRelation((*it).FactionGuid1,(*it).FactionGuid2);
//	}
//
//	//��ȡ�����ļ�
//	LoadIni();
//	return true;
//}
//
//bool	CFactionWarSys::LoadIni()
//{
//	m_FactionWars.clear();
//
//	CRFile* prfile = rfOpen("data/FactionWarSys.ini");
//	if(prfile == NULL)
//	{
//		char str[]="data/FactionWarSys.txt can't found!";
//		AddLogText(str);
//		return false;
//	}
//
//	stringstream stream;
//	prfile->ReadToStream(stream);
//	rfClose(prfile);
//
//	while(ReadTo(stream, "#"))
//	{
//		tagWarType WarType;
//		stream>>WarType.lType
//			>>WarType.lFightTime
//			>>WarType.lMoney;
//		WarType.lFightTime *= 60000;//�ѷ���ת���ɺ���
//		m_FactionWars[WarType.lType] = WarType;
//	}
//	return true;
//}
//
////��սϵͳ����
//void	CFactionWarSys::Run()
//{
//	static list<tagEnemyFaction> TemptEnemyFactionList;
//	DWORD dwCurTime = timeGetTime();
//	//һ���Ӵ���һ��
//	if( (dwCurTime-m_dwStartTime) < 60000)	return;
//	DWORD dwDifTime = dwCurTime-m_dwStartTime;
//	m_dwStartTime = dwCurTime;
//	TemptEnemyFactionList.clear();
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();it++)
//	{
//		if((*it).dwDisandTime <= dwDifTime)
//		{
//			TemptEnemyFactionList.push_back((*it));
//		}
//		else
//		{
//			(*it).dwDisandTime -= dwDifTime;
//		}
//	}
//
//	//�����սʱ���ѵ��İ��
//	it = TemptEnemyFactionList.begin();
//	for(;it != TemptEnemyFactionList.end();it++)
//	{
//		OnTimeOut( (*it).FactionGuid1,(*it).FactionGuid2 );
//	}
//}
//
////�õ���սĳ��������Ҫ�Ľ�Ǯ
//long CFactionWarSys::GetDecWarMoneyByType(long lType)
//{
//	if(m_FactionWars.find(lType) == m_FactionWars.end())
//		return 0;
//	return m_FactionWars[lType].lMoney;
//}
//
////�ж���������Ƿ��ǵжԹ�ϵ
//bool CFactionWarSys::IsEnemyRelation(const CGUID& FactionGuid1,const CGUID& FactionGuid2)
//{
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();it++)
//	{
//		if( ((*it).FactionGuid1 == FactionGuid1 && (*it).FactionGuid2 == FactionGuid2) ||
//			((*it).FactionGuid1 == FactionGuid2 && (*it).FactionGuid2 == FactionGuid1) )
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void CFactionWarSys::AddOneEnmeyFaction(const CGUID& FactionGuid1,const CGUID& FactionGuid2,DWORD dwLeaveTime)
//{
//	//����������ʱ��ֵ
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();it++)
//	{
//		if( ((*it).FactionGuid1 == FactionGuid1 && (*it).FactionGuid2 == FactionGuid2) ||
//			((*it).FactionGuid1 == FactionGuid2 && (*it).FactionGuid2 == FactionGuid1) )
//		{
//			(*it).dwDisandTime = dwLeaveTime;
//			return;
//		}
//	}
//
//	tagEnemyFaction EnemyFaction;
//	EnemyFaction.FactionGuid1 = FactionGuid1;
//	EnemyFaction.FactionGuid2 = FactionGuid2;
//	EnemyFaction.dwDisandTime = dwLeaveTime;
//	EnemyFactionList.push_back(EnemyFaction);
//}
//
////�����������ĵжԹ�ϵ
//void	CFactionWarSys::ClearEnemyFaction(const CGUID& FactionGuid1,const CGUID& FactionGuid2)
//{
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();)
//	{
//		if( ((*it).FactionGuid1 == FactionGuid1 && (*it).FactionGuid2 == FactionGuid2) ||
//			((*it).FactionGuid1 == FactionGuid2 && (*it).FactionGuid2 == FactionGuid1) )
//		{
//			it = EnemyFactionList.erase(it);
//		}
//		else
//		{
//			it++;
//		}
//	}
//}
//
////�õ�һ�����ĵж԰���б�
//void CFactionWarSys::GetEnemyFactionsByFactionID(const CGUID& FactionGuid,set<COrganizing*>& FactionList,long& DisTime)
//{
//	list<tagEnemyFaction>::iterator it = EnemyFactionList.begin();
//	for(;it != EnemyFactionList.end();it++)
//	{
//		if( (*it).FactionGuid1 == FactionGuid )
//		{
//			COrganizing* pOrg = GetOrganiCtrl()->GetFactionOrganizing((*it).FactionGuid2);
//			if(pOrg && FactionList.find(pOrg) != FactionList.end())
//				FactionList.insert(pOrg);
//			if((*it).dwDisandTime > DisTime)
//				(*it).dwDisandTime = DisTime;
//		}
//		else if( (*it).FactionGuid2 == FactionGuid )
//		{
//			COrganizing* pOrg = GetOrganiCtrl()->GetFactionOrganizing((*it).FactionGuid1);
//			if(pOrg && FactionList.find(pOrg) != FactionList.end())
//				FactionList.insert(pOrg);
//			if((*it).dwDisandTime > DisTime)
//				(*it).dwDisandTime = DisTime;
//		}
//	}
//}
//
////�����ս
//bool CFactionWarSys::DigUpTheHatchet(const CGUID& DigGuid,const CGUID& BeDigedFactinID,long lLvl,tagTime& Time)
//{
//	//�쿴�Ƿ��ҵõ��˰�ս����
//	if(m_FactionWars.find(lLvl) == m_FactionWars.end())	return false;
//	long lFightTime = m_FactionWars[lLvl].lFightTime;
//	long lMoney     = m_FactionWars[lLvl].lMoney;
//
//	const CGUID& DigFactionID =  GetOrganiCtrl()->IsFactionMaster(DigGuid);
//	//ֻ�а���������ս
//	if(DigFactionID == NULL_GUID)
//	{
//		GetOrganiCtrl()->SendOrgaInfoToClient(DigGuid,string("ֻ�а���������ս��"),string("ϵͳ��ʾ"));
//		return false;
//	}
//
//	//�Ҳ�������ʵ��
//	COrganizing* pDigFactin = GetOrganiCtrl()->GetFactionOrganizing(DigFactionID);
//	COrganizing* pBeDigedFaction = GetOrganiCtrl()->GetFactionOrganizing(BeDigedFactinID);
//
//	//�Ҳ������ʵ��
//	if(pDigFactin == NULL || pBeDigedFaction == NULL) return false;
//
//	//�������ͬһ�������˳� 
//	if(pDigFactin->IsSuperiorOrganizing() != NULL_GUID && 
//		pDigFactin->IsSuperiorOrganizing() == pBeDigedFaction->IsSuperiorOrganizing())
//	{
//		GetOrganiCtrl()->SendOrgaInfoToClient(DigGuid,
//											string("��ͶԷ��������ͬһ���ˣ�������ս��"),
//											string("ϵͳ��ʾ"));
//		return false;
//	}
//
//	CPlayer* pPlayer = GetGame()->GetMapPlayer(DigGuid);
//	if(pPlayer == NULL)	return false;
//
//	if(pPlayer->GetMoney() < lMoney)
//	{
//		char strSend[256]="";
//		sprintf(strSend,"��սʧ�ܣ���û��%d�Ľ�Ǯ��",lMoney);
//		GetOrganiCtrl()->SendOrgaInfoToClient(DigGuid,string(strSend),string("ϵͳ��ʾ"));
//		return false;
//	}
//
//
//	list<COrganizing*>	DigFactionList;DigFactionList.clear();
//	list<COrganizing*>  BeDigedFactionList;BeDigedFactionList.clear();
//
//	//�õ���ս�����еİ���б�
//    const CGUID& DigUnionID = GetOrganiCtrl()->IsFreeFaction(pDigFactin->GetExID());
//	if(DigUnionID == NULL_GUID)
//	{
//		DigFactionList.push_back(pDigFactin);
//	}
//	else
//	{
//		COrganizing* pDigUnion = GetOrganiCtrl()->GetUnionOrganizing(DigUnionID);
//		if(pDigUnion)
//			pDigUnion->GetMemberList(DigFactionList);
//	}
//	//�õ�����ս�����а���б�
//	const CGUID& BeDigUnionID = GetOrganiCtrl()->IsFreeFaction(pBeDigedFaction->GetExID());
//	if(BeDigUnionID == NULL_GUID)
//	{
//		BeDigedFactionList.push_back(pBeDigedFaction);
//	}
//	else
//	{
//		COrganizing* pBeDigedUnion = GetOrganiCtrl()->GetUnionOrganizing(BeDigUnionID);
//		if(pBeDigedUnion)
//			pBeDigedUnion->GetMemberList(BeDigedFactionList);
//	}
//
//	list<COrganizing*>::iterator itDig = DigFactionList.begin();
//	for(;itDig != DigFactionList.end();itDig++)
//	{
//		COrganizing* pDigOrg = (*itDig);
//		list<COrganizing*>::iterator itBeDig = BeDigedFactionList.begin();
//		for(;itBeDig != BeDigedFactionList.end();itBeDig++)
//		{
//			COrganizing* pBeDigedOrg = (*itBeDig);
//			//��ӵжԹ�ϵ
//			AddOneEnmeyFaction(pDigOrg->GetExID(),pBeDigedOrg->GetExID(),lFightTime);
//
//			//��һ�������ӵж���֯
//			pDigOrg->AddEnemyOrganizing(pBeDigedOrg->GetExID());
//			pBeDigedOrg->AddEnemyOrganizing(pDigOrg->GetExID());
//		}
//	}
//
//	string strFacNames;
//	string strFacNames1;
//	//������صж԰����Ϣ����ҺͿͻ���
//	itDig = DigFactionList.begin();
//	for(;itDig != DigFactionList.end();itDig++)
//	{
//		COrganizing* pDigOrg = (*itDig);
//		if(pDigOrg)
//		{
//			pDigOrg->UpdateEnemyFaction();
//
//			strFacNames += pDigOrg->GetName();
//			strFacNames += ",";
//		}
//	}
//	strFacNames = strFacNames.substr(0,strFacNames.length()-1);
//
//	list<COrganizing*>::iterator itBeDig = BeDigedFactionList.begin();
//	for(;itBeDig != BeDigedFactionList.end();itBeDig++)
//	{
//		COrganizing* pBeDigedOrg = (*itBeDig);
//		if(pBeDigedOrg)
//		{
//			pBeDigedOrg->UpdateEnemyFaction();
//
//			strFacNames1 += pBeDigedOrg->GetName();
//			strFacNames1 += ",";
//		}
//	}
//	strFacNames1 = strFacNames1.substr(0,strFacNames1.length()-1);
//
//	//ϵͳ����
//	char strMsg[4096]="";
//	sprintf(strMsg,"ϵͳʱ��%s,{%s}��{%s}��ս�ˡ�",Time.GetDateFormatStr(),strFacNames.c_str(),strFacNames1.c_str());
//
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strMsg),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	PutWarDebugString(strMsg);
//
//	return true;
//}
//
//
////�������ʱ����
//void CFactionWarSys::OnPlayerDied(const CGUID& PlayerID, const CGUID& Killer)
//{
//	//��鱻ɱ�����Ƿ��ǰ���������
//	const CGUID& FactionID = GetOrganiCtrl()->IsFactionMaster(PlayerID);
//	if(FactionID == NULL_GUID)	return;
//	CGUID UnionID = GetOrganiCtrl()->IsFreeFaction(FactionID);
//	if(UnionID != NULL_GUID)
//	{
//		UnionID = GetOrganiCtrl()->IsConferationMaster(FactionID);
//		if(UnionID == NULL_GUID)	return;
//	}
//	
//	//���ɱ�����Ƿ��а��
//	const CGUID& KillFactionID = GetOrganiCtrl()->IsFreePlayer(Killer);
//	if(KillFactionID == NULL_GUID)	return;
//
//
//	COrganizing* pFaction = GetOrganiCtrl()->GetFactionOrganizing(FactionID);
//	COrganizing* pKillFaction = GetOrganiCtrl()->GetFactionOrganizing(KillFactionID);
//
//	if(pFaction == NULL || pKillFaction == NULL)	return;
//
//	//��������������Ƿ��ǵж�״̬
//	if(IsEnemyRelation(FactionID,KillFactionID) == false)	return;
//
//	list<COrganizing*>	FactionList;FactionList.clear();
//	list<COrganizing*>  KillFactionList;KillFactionList.clear();
//
//	//�õ���ս�����еİ���б�
//    UnionID = GetOrganiCtrl()->IsFreeFaction(pFaction->GetExID());
//	if(UnionID == NULL_GUID)
//	{
//		FactionList.push_back(pFaction);
//	}
//	else
//	{
//		COrganizing* pUnion = GetOrganiCtrl()->GetUnionOrganizing(UnionID);
//		if(pUnion)
//			pUnion->GetMemberList(FactionList);
//	}
//	//�õ�����ս�����а���б�
//	const CGUID& KillUnionID = GetOrganiCtrl()->IsFreeFaction(pKillFaction->GetExID());
//	if(KillUnionID == NULL_GUID)
//	{
//		KillFactionList.push_back(pKillFaction);
//	}
//	else
//	{
//		COrganizing* pKillUnion = GetOrganiCtrl()->GetUnionOrganizing(KillUnionID);
//		if(pKillUnion)
//			pKillUnion->GetMemberList(KillFactionList);
//	}
//
//	//��������ĵж�״̬
//	list<COrganizing*>::iterator it = FactionList.begin();
//	for(;it != FactionList.end();it++)
//	{
//		COrganizing* pOrg = (*it);
//		list<COrganizing*>::iterator itKill = KillFactionList.begin();
//		for(;itKill != KillFactionList.end();itKill++)
//		{
//			COrganizing* pKillOrg = (*itKill);
//			//����жԹ�ϵ
//			ClearEnemyFaction(pOrg->GetExID(),pKillOrg->GetExID());
//
//			//��һ�������ӵж���֯
//			pOrg->DelEnemyOrganizing(pKillOrg->GetExID());
//			pKillOrg->DelEnemyOrganizing(pOrg->GetExID());
//		}
//	}
//
//	string strFacNames;
//	string strKillFacNames;
//	//������صж԰����Ϣ����ҺͿͻ���
//	it = FactionList.begin();
//	for(;it != FactionList.end();it++)
//	{
//		COrganizing* pOrg = (*it);
//		if(pOrg)
//		{
//			pOrg->UpdateEnemyFaction();
//			strFacNames += pOrg->GetName();
//			strFacNames += ",";
//		}
//	}
//	strFacNames = strFacNames.substr(0,strFacNames.length()-1);
//	list<COrganizing*>::iterator itKill = KillFactionList.begin();
//	for(;itKill != KillFactionList.end();itKill++)
//	{
//		COrganizing* pKillOrg = (*itKill);
//		if(pKillOrg)
//		{
//			pKillOrg->UpdateEnemyFaction();
//			strKillFacNames += pKillOrg->GetName();
//			strKillFacNames += ",";
//		}
//	}
//	strKillFacNames = strKillFacNames.substr(0,strKillFacNames.length()-1);
//
//	//ϵͳ����
//	char strMsg[4096]="";
//	sprintf(strMsg,"{%s}�ڰ��ս��սʤ��{%s}��",strKillFacNames.c_str(),strFacNames.c_str());
//
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strMsg),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	PutWarDebugString(strMsg);
//}
//
//
////��ֹ��ս
//void CFactionWarSys::StopFactionWar(const CGUID& FactionGuid1,const CGUID& FactionGuid2)
//{
//	//��������������Ƿ��ǵж�״̬
//	if(IsEnemyRelation(FactionGuid1,FactionGuid2) == false)	return;
//
//	COrganizing* pFaction1 = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid1);
//	COrganizing* pFaction2 = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid2);
//
//	if(pFaction1 == NULL || pFaction2 == NULL)
//	{
//		ClearEnemyFaction(FactionGuid1,FactionGuid2);
//		return;
//	}
//
//	list<COrganizing*>	FactionList1;FactionList1.clear();
//	list<COrganizing*>  FactionList2;FactionList2.clear();
//
//	//�õ���ս�����еİ���б�
//	CGUID UnionID = GetOrganiCtrl()->IsFreeFaction(FactionGuid1);
//	if(UnionID == NULL_GUID)
//	{
//		FactionList1.push_back(pFaction1);
//	}
//	else
//	{
//		COrganizing* pUnion = GetOrganiCtrl()->GetUnionOrganizing(UnionID);
//		if(pUnion)
//			pUnion->GetMemberList(FactionList1);
//	}
//	//�õ�����ս�����а���б�
//	UnionID = GetOrganiCtrl()->IsFreeFaction(FactionGuid2);
//	if(UnionID == NULL_GUID)
//	{
//		FactionList2.push_back(pFaction2);
//	}
//	else
//	{
//		COrganizing* pUnion = GetOrganiCtrl()->GetUnionOrganizing(UnionID);
//		if(pUnion)
//			pUnion->GetMemberList(FactionList2);
//	}
//
//	//��������ĵж�״̬
//	list<COrganizing*>::iterator it = FactionList1.begin();
//	for(;it != FactionList1.end();it++)
//	{
//		COrganizing* pOrg = (*it);
//		list<COrganizing*>::iterator it1 = FactionList2.begin();
//		for(;it1 != FactionList2.end();it1++)
//		{
//			COrganizing* pOrg1 = (*it1);
//			//����жԹ�ϵ
//			ClearEnemyFaction(pOrg->GetExID(),pOrg1->GetExID());
//
//			//��һ�������ӵж���֯
//			pOrg->DelEnemyOrganizing(pOrg1->GetExID());
//			pOrg1->DelEnemyOrganizing(pOrg->GetExID());
//		}
//	}
//
//	string strFacNames;
//	string strFacNames1;
//	//������صж԰����Ϣ����ҺͿͻ���
//	it = FactionList1.begin();
//	for(;it != FactionList1.end();it++)
//	{
//		COrganizing* pOrg = (*it);
//		if(pOrg)
//		{
//			pOrg->UpdateEnemyFaction();
//			strFacNames += pOrg->GetName();
//			strFacNames += ",";
//		}
//	}
//	strFacNames = strFacNames.substr(0,strFacNames.length()-1);
//	it = FactionList2.begin();
//	for(;it != FactionList2.end();it++)
//	{
//		COrganizing* pOrg = (*it);
//		if(pOrg)
//		{
//			pOrg->UpdateEnemyFaction();
//			strFacNames1 += pOrg->GetName();
//			strFacNames1 += ",";
//		}
//	}
//	strFacNames1 = strFacNames1.substr(0,strFacNames1.length()-1);
//
//	//ϵͳ����
//	char strMsg[4096]="";
//	sprintf(strMsg,"{%s}��{%s}�İ��ս��������",strFacNames1.c_str(),strFacNames.c_str());
//
//	GetOrganiCtrl()->SendOrgaInfoToClient(string(strMsg),
//		MAKELONG(MAKEWORD(146,254), MAKEWORD(255,255)),MAKELONG(MAKEWORD(0,0), MAKEWORD(255,255)));
//
//	//���ս��������Ϣ
//	PutWarDebugString(strMsg);
//}
////��սʱ���ѵ�����
//void CFactionWarSys::OnTimeOut(const CGUID& FactionGuid1,const CGUID& FactionGuid2)
//{
//	StopFactionWar(FactionGuid1,FactionGuid2);
//}
//
//
//void CFactionWarSys::GenerateSaveData()
//{
////	list<tagEnemyFaction*> ListEnemyFactions;
////	list<tagEnemyFaction>::iterator it  = EnemyFactionList.begin();
////	for(;it != EnemyFactionList.end();it++)
////	{
////		tagEnemyFaction* pEnemyFaction =
////			new tagEnemyFaction((*it).FactionGuid1,(*it).FactionGuid2,(*it).dwDisandTime);
////		ListEnemyFactions.push_back(pEnemyFaction);
////	}
////	GetGame()->SetEnemyFactions(ListEnemyFactions);
//}
//
//CFactionWarSys* GetFactionWarSys()
//{
//	return CFactionWarSys::GetInstance();
//}