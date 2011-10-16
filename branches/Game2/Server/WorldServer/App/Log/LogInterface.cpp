#include "StdAfx.h"

#include "../GlobalRgnManager.h"
#include "../OrganizingSystem/WSTeam.h"

#include "../../Net/Message.h"

#include "LogicLogInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//<!--��ᴴ��/��ɢ��־-->
//<Table name="t150_faction_log" log_no="150" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="faction"  head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��������1��ɢ��-->
//</Table>
//*/
bool LogicLogInterface::LogT150_faction_log(LPCSTR pOprName, const CGUID &pOprGUID, CFaction *pFaction, LONG lAct)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pOprName || NULL == pFaction) return false;

    string strTime;
    GetCurrTimeString(strTime);


    return m_pLogClient->SendLogToServer(150, strTime.c_str(),
        &pOprGUID,	pOprName,
        &(pFaction->GetExID()),	pFaction->GetName(),
        lAct
        );
}
//
///* --------------------------------------------------------------------
//<!--��Ա�������/�˳���־-->
//<Table name="t160_faction_join_leave_log" log_no="160" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="faction"  head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0�����룻1�˳���-->
//</Table>
//*/
bool LogicLogInterface::LogT160_faction_join_leave_log(const char *pOpterName, const CGUID *pOpterGuid, CFaction *pFaction, LONG lAct)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pOpterGuid || NULL == pOpterName || NULL == pFaction) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer( 
        160, strTime.c_str(),
        pOpterGuid,	pOpterName,
        &(pFaction->GetExID()),	pFaction->GetName(),
        lAct
        );
}
//
//
///* --------------------------------------------------------------------
//<!--ͬ�����/������Ա��־-->
//<Table name="t170_faction_agree_kick_log" log_no="170" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="player"   head_name="aim"></item>
//<item type="faction"  head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��ͬ�⣻1������-->
//</Table>
//*/
bool LogicLogInterface::LogT170_faction_agree_kick_log(const char *pOpterName, const CGUID *pOpterGuid, const char *pAimName, const CGUID *pAimGuid, CFaction *pFaction, LONG lAct)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pOpterName || NULL == pOpterGuid || NULL == pAimName || NULL == pAimGuid || NULL == pFaction) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer( 
        170, strTime.c_str(),
        pOpterGuid, pOpterName,
        pAimGuid,	pAimName,
        &(pFaction->GetExID()),	pFaction->GetName(),
        lAct
        );
}

///* --------------------------------------------------------------------
//<!--�������-->
//<Table name="t250_faction_master_log" log_no="250" log_pos="1">
//<item type="client_head"    head_name=""></item>
//<item type="faction"		head_name=""></item>
//<item type="player"			head_name="old"></item>
//<item type="player"			head_name="new"></item>
//</Table>
//*/
bool LogicLogInterface::LogT250_faction_master_log(CFaction *pFaction, tagFacMemInfo *pOld, tagFacMemInfo *pNew)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pFaction || NULL == pOld || NULL == pNew) return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer( 
        250, strTime.c_str(),							
        &(pFaction->GetExID()),			pFaction->GetName(), 
        &(pOld->MemberGuid),			pOld->szName,
        &(pNew->MemberGuid),			pNew->szName
        );
}
//
///*
//<!--================================================================-->
//<!--��ᾭ��仯-->
//<Table name="t260_faction_experience_log" log_no="260" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="faction"  head_name=""></item>
//<item type="alone"    Item_name="before_exp"      data_type="int32"></item><!--֮ǰ����-->
//<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--��ǰ����-->
//</Table>
//*/
bool LogicLogInterface::LogT260_faction_experience_log(CFaction *pFaction, long lBeforeExp)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pFaction) return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(260, strTime.c_str(),
        &(pFaction->GetExID()),			pFaction->GetName(), 
        lBeforeExp,						pFaction->GetExp()
        );
}
//
///*
//<!--================================================================-->
//<!--���ȼ��仯-->
//<Table name="t270_faction_level_log" log_no="270" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="faction"  head_name=""></item>
//<item type="alone"    Item_name="before_level"    data_type="int32"></item><!--֮ǰ�ȼ�-->
//<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--��ǰ�ȼ�-->
//<item type="alone"    Item_name="level_type"      data_type="int32"></item><!--�ȼ����� 0=���� 1=���� 2=�ֿ� 3=���� 4���� 5=����-->
//</Table>
//*/
bool LogicLogInterface::LogT270_faction_level_log(CFaction *pFaction, long lCurrLevel, long lLvlType)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pFaction) return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(270, strTime.c_str(),
        &(pFaction->GetExID()),			pFaction->GetName(), 
        pFaction->GetLvl(lLvlType),		lCurrLevel,
        lLvlType
        );
}

////! ��־��¼�ӿڶ���
////////////////////////////////////////////////////////////////////////////////////////////////
///* 
//��־���ã�
//<!--�����齨-->
//<Table name="t340_team_create" log_no="34">
//<item type="date"     head_name=""></item>
//<item type="object"   head_name="team"></item>
//<item type="player"   head_name="sponsor"></item>
//<item type="player"   head_name="participator"></item>
//</Table>
//*/
////! ��¼�����齨��־
bool LogicLogInterface::LogT340_team_create(	
    WSTeam *pTeam,			//! ����
    CPlayer *pSponsor,		//! ������
    CPlayer *pParticipator	//! ������
    )
{
    if(NULL == m_pLogClient) return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer( 340, strTime.c_str(),
        &(pTeam->GetExID()),			"", 
        &(pSponsor->GetExID()),			pSponsor->GetName(),
        &(pParticipator->GetExID()),	pParticipator->GetName()
        );
}
//
///* 
//��־���ã�
//<!--�����ɢ-->
//<Table name="t370_team_disband" log_no="37">
//<item type="date"     head_name=""></item>
//<item type="object"   head_name="team"></item>
//</Table>
//*/
////! ��¼�����ɢ��־
bool LogicLogInterface::LogT370_team_disband(WSTeam *pTeam)
{
	if(NULL == m_pLogClient) return true;

    string strTime;
    GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 370, strTime.c_str(),
		&(pTeam->GetExID()), ""
		);
}


///*
//<!--��ϵ�˲�����־-->
//<Table name="t450_linkman_opt_log" log_no="50" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name="opter"></item><!--������-->
//<item type="player"   head_name="aim"></item><!--����Ŀ��-->
//<item type="alone"    Item_name="opt_type"      data_type="int32"></item><!--�������ͣ�1=��ӡ�2=ɾ��-->
//<item type="alone"    Item_name="group_id"      data_type="int32"></item><!--����������ϵ�����ID-->
//</Table>
//*/
bool LogicLogInterface::logT450_linkman_opt_log(CPlayer *pOpter, const CGUID &AimGuid, const char *pAimName, long lOptType, long lGroupID)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;

    if(NULL == m_pLogClient)return false;
    if (NULL == pOpter || NULL == pAimName) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer( 
        450,strTime.c_str(),
        &(pOpter->GetExID()),	pOpter->GetName(),
        &AimGuid,				pAimName,
        lOptType,				lGroupID
        );
}

///*
//<!--�ͻ�����������־-->
//<Table name="t550_client_in_game_log" log_no="550" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="alone"    Item_name="cdkey"         data_type="str128"></item><!--IP-->
//	<item type="alone"    Item_name="ip"            data_type="int32"></item><!--CDKEY-->
//	<item type="player"   head_name=""></item>
//	<item type="pos"      head_name=""></item>
//	<item type="alone"    Item_name="region_guid"   data_type="int32"></item><!--level-->
//	<item type="alone"    Item_name="level"         data_type="int32"></item><!--level-->
//	<item type="alone"    Item_name="gold"          data_type="int32"></item><!--gold-->
//	<item type="alone"    Item_name="silver"        data_type="int32"></item><!--silver-->
//	<item type="alone"    Item_name="flag"          data_type="int32"></item><!--�������ͣ�1=���ߡ�2=����-->
//</Table>
//*/
bool LogicLogInterface::logT550_client_in_game_log(CPlayer *pPlayer, LONG lState)
{
    if(0 == GetNetHandler().GetServerSetup().useLogServer) return true;
    if(NULL == m_pLogClient)return false;
    if(NULL == pPlayer) return false;

    const char *pRegionName = "";
    map<LONG, CGlobalRgnManager::tagRegion> &mapRegionList = *(GetGame().GetGlobalRgnManager()->GetTemplateRegionListMap());
    map<LONG, CGlobalRgnManager::tagRegion>::iterator ite = mapRegionList.find(pPlayer->GetRegionID());
    if (ite != mapRegionList.end())
    {
        CWorldRegion *pWorldRegion = ite->second.pRegion;
        if (NULL != pWorldRegion)
        {
            pRegionName = pWorldRegion->GetName();
        }
    }

    float fPosX = pPlayer->GetPosX();
    float fPosY = pPlayer->GetPosY();

    //! ת��IP
    char szTmp[128] = {0};
    UCHAR arrIp[4] = {0};
    LONG lIP = pPlayer->GetIP();
    memmove(arrIp, &lIP, sizeof(LONG));
    sprintf(szTmp, "%u.%u.%u.%u", (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);

    string strTime;
    GetCurrTimeString(strTime);


    LONG lBankMoney = 0;
    CGoodsContainer* pBank = pPlayer->m_PlayerDepot.FindContainer(eDCT_Gold);
    if(NULL != pBank)
        lBankMoney = pBank->GetGoodsAmount();

    return m_pLogClient->SendLogToServer( 
        550, strTime.c_str(),
        pPlayer->GetAccount(),
        szTmp,
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pPlayer->GetRegionID(),
        pRegionName,
        &fPosX,
        &fPosY,
        &(pPlayer->GetRegionExID()),
        pPlayer->GetLevel(),
        pPlayer->GetMoney() + lBankMoney,
        pPlayer->GetSilver(),
        lState
        );
}