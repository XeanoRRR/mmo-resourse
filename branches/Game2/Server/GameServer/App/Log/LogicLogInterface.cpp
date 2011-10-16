#include "StdAfx.h"
#include "LogicLogInterface.h"

#include "../Player.h"
#include "../OrganizingSystem/GSTeam.h"

#include "../../../../Public/Nets/BaseMessage.h"
#include "../../../../Public/Setup/GoodsSetup.h"


LogicLogInterface::LogicLogInterface()
{
	m_pLogClient = new LogClient();
}

LogicLogInterface::~LogicLogInterface()
{
    SAFE_DELETE(m_pLogClient);
}

void LogicLogInterface::InitLogClient(CBaseMessage *pMsg, LogClientInfo &logClientInfo)
{
   m_pLogClient->Init(pMsg, logClientInfo);
}

//!							�õ���ǰʱ���ַ�����00-00-00 00:00:00��ʽ��
void LogicLogInterface::GetCurrTimeString(string &strTime)
{
	SYSTEMTIME	stTime;
	char		szTime[128] = {0};
	GetLocalTime(&stTime);
	sprintf(szTime,"%d-%d-%d %d:%d:%d" ,
		stTime.wYear,
		stTime.wMonth,
		stTime.wDay,
		stTime.wHour,
		stTime.wMinute,
		stTime.wSecond);

	strTime = string(szTime);
}

//<!--��Ʒ������־-->
//<Table name="t010_goods_log_trade" log_no="10" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name="sponsor"></item>
//<item type="player"   head_name="participator"></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0����ͨ��1�����̵꣩-->
//<item type="alone"    Item_name="goods_price"     data_type="int32"></item><!--��Ʒ�۸�-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
bool LogicLogInterface::logT010_goods_log_trade(CPlayer* pPlayer1,CPlayer* pPlayer2,DWORD dwGoodsId,const CGUID& goodsGuid,
                                                const char* strGoodsName,long lActType,long lPrice,long lNum,DWORD dwMoneyType)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName = GoodsSetup::QueryGoodsOriginalName(dwGoodsId);
    if(!szOriginName)
        return false;
    string strOriginName(szOriginName);
    if(strOriginName!="GDMONEY" && strOriginName.substr(0,2)=="GD")
        return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(pPlayer1==NULL || pPlayer2==NULL)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer1->GetRegionGUID()));
    if(!pRegion)
        return false;


    long posX = pPlayer1->GetTileX();
    long posY = pPlayer1->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(10, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer1->GetExID()),pPlayer1->GetName(),
        &(pPlayer2->GetExID()),pPlayer2->GetName(),
        dwGoodsId, &goodsGuid,strGoodsName,
        lActType,lPrice,lNum,dwMoneyType);
}

//<Table name="t020_goods_log_pickup" log_no="20" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0��ʰȡ��1��������-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
bool LogicLogInterface::logT020_goods_log_pickup(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,long lActType,long lNum)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName = GoodsSetup::QueryGoodsOriginalName(dwGoodsId);
    if(!szOriginName)
        return false;
    string strOriginName(szOriginName);
    if(strOriginName!="GDMONEY" && strOriginName.substr(0,2)=="GD")
        return false;

    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(20, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()),pPlayer->GetName(),dwGoodsId,&goodsGuid,strGoodsName,lActType,lNum,pPlayer->GetMoney(),pPlayer->GetSilver());
}

//<!--��Ʒ��Ƕ��־-->
//<Table name="t021_goods_log_enchase" log_no="21" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item>
//<!--�������ͣ�0���⣻1����-->
//<item type="alone"    Item_name="hole_pos"       data_type="int32"></item>
//<!--��λ��-->
//<item type="alone"    Item_name="card_id"       data_type="int32"></item>
//<!--��ID-->
//<item type="alone"    Item_name="card_name"       data_type="str128"></item>
//<!--�Ƿ�ʹ��Ӱ�أ�0��û�У�1��ʹ���ˣ�-->
//<item type="alone"    Item_name="reserve_card"  data_type="int32"></item>
//<!--������-->
//</Table>
bool LogicLogInterface::logT021_goods_log_enchase(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,
                                                  long lActType,DWORD dwHolePos,DWORD dwCardId,const char* strCardName,DWORD dwReserveCard)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName = GoodsSetup::QueryGoodsOriginalName(dwGoodsId);
    if(!szOriginName)
        return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(21, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()),pPlayer->GetName(),dwGoodsId,&goodsGuid,strGoodsName,lActType,dwHolePos,dwCardId,strCardName,dwReserveCard);
}

//<!--��Ʒ������־-->
//<Table name="t022_goods_log_consume" log_no="22" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>    
//<item type="alone"    Item_name="num"  data_type="int32"></item>
//<item type="alone"    Item_name="used_num" data_type="int32"></item>
//<!--����-->
//<item type="alone"    Item_name="container_id"       data_type="int32"></item>
//<!--container id-->
//<item type="alone"    Item_name="container_pos"       data_type="int32"></item>
//<!--container pos-->
//</Table>
bool LogicLogInterface::logT022_goods_log_consume(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,
                                                  const char* strGoodsName,DWORD dwNum,DWORD dwUsedNum,DWORD dwPlace,DWORD dwPos)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName = GoodsSetup::QueryGoodsOriginalName(dwGoodsId);
    if(!szOriginName)
        return false;
    string strOriginName(szOriginName);
    if(strOriginName.substr(0,3)=="PPH" || strOriginName.substr(0,3)=="PPM")
        return false;

    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(22, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()),pPlayer->GetName(),dwGoodsId,&goodsGuid,strGoodsName,dwNum,dwUsedNum,dwPlace,dwPos);
}

//<!--��Ʒ������־-->
//<Table name="t023_goods_log_repair" log_no="23" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone" Item_name="act_type"        data_type="int32"></item>
//</Table>
bool LogicLogInterface::logT023_goods_log_repair(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,int nActType)
{	
    if(NULL == m_pLogClient)return false;
    string strGoodsName="-";
    const char* szGoodsName = GoodsSetup::QueryGoodsName(dwGoodsId);
    if(szGoodsName)
        strGoodsName=szGoodsName;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);


    return m_pLogClient->SendLogToServer(23,strTime.c_str(), &(pPlayer->GetExID()),pPlayer->GetName(),dwGoodsId,&goodsGuid,strGoodsName.c_str(),nActType);
}

//<!--������ȡ��־-->
//<Table name="t026_goods_log_bouns" log_no="26" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>   
//<item type="goods"    head_name=""></item> 
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"    Item_name="bouns_id"       data_type="int32"></item><!--�������-->
//</Table>
bool LogicLogInterface::logT026_goods_log_bouns(CPlayer* pPlayer, DWORD dwGoodsId, const CGUID& goodsGuid, const char* strGoodsName, int nGoodsNum,int nBounsID)
{
    if(0 == GetGame()->GetSetup()->lUseLogServer)
        return true;

    if(NULL == m_pLogClient)
        return false;

    if (NULL == pPlayer) 
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(
        26, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        dwGoodsId, &goodsGuid,strGoodsName, 
        nGoodsNum,nBounsID);
}

//<!--��NPC����Ʒ������־-->
//<Table name="t030_goods_log_npc" log_no="30" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--�������ͣ�0�������1���������-->
//<item type="alone"    Item_name="goods_price"     data_type="int32"></item><!--��Ʒ�۸�-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
//*/
bool LogicLogInterface::logT030_goods_log_npc(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,
                                              const char* strGoodsName,long lActType,long lPrice,long lNum)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName=GoodsSetup::QueryGoodsOriginalName(dwGoodsId);
    if(!szOriginName)
        return false;
    string strOriginName(szOriginName);
    if(strOriginName!="GDMONEY" && strOriginName!="GDMONEYS" && strOriginName.substr(0,2)=="GD")
        return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(!pRegion)
        return false;


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);


    return m_pLogClient->SendLogToServer(30, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()),pPlayer->GetName(),
        dwGoodsId, &goodsGuid,strGoodsName,
        lActType,lPrice,lNum,pPlayer->GetMoney(),pPlayer->GetSilver());

}

///*<!--��NPCͨ��λ�澫��������־-->
//<Table name="t031_goods_wm_npc" log_no="31" log_pos="1">	
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>  
//<item type="alone"	  Item_name="Goods_Num"		  data_type="int32"></item><!--��ȡ��Ʒ������-->
//<item type="alone"	  Item_name="wm_Num"		  data_type="int32"></item><!--λ������-->
//</Table>*/
bool LogicLogInterface::logT031_goods_wm_npc(CPlayer* pPlayer,DWORD dwGoodsID,const CGUID& guid,LONG lGoodsNum,LONG lWmNum)
{
    if(NULL == m_pLogClient)return false;
    const char* szOriginName=GoodsSetup::QueryGoodsOriginalName(dwGoodsID);
    if(!szOriginName)
        return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(31, strTime.c_str(),
        &(pPlayer->GetExID()),pPlayer->GetName(),
        dwGoodsID,&guid,szOriginName,lGoodsNum,lWmNum);
}

///*
//<!--��Ʒ��ʧ��־-->
//<Table name="t040_goods_log_disappear" log_no="40" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���𻵣�1��������ʧ��-->
//</Table>
//*/
bool LogicLogInterface::logT040_goods_log_disappear(CPlayer* pPlayer,CGoods* pGoods,long lActType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pGoods)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(40, strTime.c_str(),
        &(pPlayer->GetExID()),pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
        lActType);
}

///*
//<!--��Ʒ������־-->
//<Table name="t050_goods_log_fall" log_no="50" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//</Table>
//*/
bool LogicLogInterface::logT050_goods_log_fall(CPlayer* pPlayer,CGoods* pGoods)
{
    if(NULL == m_pLogClient)return false;
    if(!pGoods)
        return false;
    const char* szOriginName=GoodsSetup::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
    if(!szOriginName)
        return false;
    string strOriginName(szOriginName);
    if(strOriginName!="GDMONEY" && strOriginName!="GDMONEYS" && strOriginName.substr(0,2)=="GD")
        return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(!pPlayer)
        return false;

    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(!pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(50, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()),pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName());
}

///*
//<!--================================================================-->
//<!--�ɼ���־-->
//<Table name="t055_collect" log_no="55" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="opt_id"          data_type="guid"></item><!--����ID-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
//*/
bool LogicLogInterface::logT055_collect( CPlayer* pPlayer,CGoods* pGoods,const CGUID& guid,int nAmount )
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pGoods)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(55, strTime.c_str(),
        pRegion->GetID(), pRegion->GetName(), posX, posY,
        &(pPlayer->GetExID()), pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(),
        &(pGoods->GetExID()), pGoods->GetName(),
        &guid,
        nAmount
        );
}

///*
//<!--================================================================-->
//<!--������־-->
//<Table name="t056_facture" log_no="56" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="opt_id"          data_type="guid"></item><!--����ID-->
//<item type="alone"    Item_name="act_type"          data_type="int32"></item><!--��������,0��ɾ��,1�����-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
//*/
bool LogicLogInterface::logT056_facture( CPlayer* pPlayer,CGoods* pGoods,const CGUID& guid,int nType, int nAmount )
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pGoods)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(56, strTime.c_str(),
        &(pPlayer->GetExID()), pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(),
        &(pGoods->GetExID()), pGoods->GetName(),
        &guid,
        nType,
        nAmount
        );
}

//<!--�ֿ������־-->
//<Table name="t060_goods_log_depository" log_no="60" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="goods"    head_name=""></item>
//	<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���棻1��ȡ��-->
//	<item type="alone"    Item_name="goods_num"       data_type="int32"></item><!--��Ʒ����-->
//</Table>
//*/
bool LogicLogInterface::logT060_goods_log_depository(CPlayer *pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,
                                                     const char* strGoodsName,DWORD dwGoodsNum, long lActType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;


    if (NULL == pPlayer) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(60, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        dwGoodsId, &goodsGuid,strGoodsName, 
        lActType, dwGoodsNum
        );
}


//<!--���в�����־-->
//<Table name="t070_goods_log_bank" log_no="70" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���ͣ�0���棻1��ȡ��-->
//	<item type="alone"    Item_name="number"          data_type="int32"></item><!--����-->
//</Table>
bool LogicLogInterface::logT070_goods_log_bank(CPlayer *pPlayer, long lActType, long lNum)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer) 
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(70, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        lActType, lNum
        );
}


///*
//<!--��һ�þ���-->
//<Table name="t080_player_getexp_log" log_no="80" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���Ŀ�����ͣ�0�����1��NPC��3�����帣����-->
//<item type="alone"    Item_name="delta_exp"       data_type="int32"></item><!--�仯�ľ���ֵ-->
//<item type="alone"    Item_name="delta_expMul"    data_type="int32"></item><!--�仯�ľ��鱶��-->
//<item type="alone"    Item_name="old_exp"         data_type="int32"></item><!--�仯ǰ�ľ���ֵ-->
//<item type="alone"    Item_name="old_expMul"	  data_type="int32"></item><!--�仯ǰ�ľ��鱶��-->
//<item type="alone"    Item_name="new_exp"         data_type="int32"></item><!--�仯��ľ���ֵ-->
//<item type="alone"    Item_name="new_expMul"	  data_type="int32"></item><!--�仯��ľ��鱶��-->
//<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������Դ��˵��-->
//</Table>
//*/
bool LogicLogInterface::logT080_player_getexp_log(CPlayer *pPlayer, DWORD dwType, LONG lChangeExp, LONG lChangeExpMul, 
                                                  LONG lOldExp, LONG lOldExpMul, LONG lNewExp, LONG lNewExpMul, const char *pSrcDepict)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pSrcDepict)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(80, strTime.c_str(),
        &(pPlayer->GetExID()), pPlayer->GetName(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        dwType,
        lChangeExp, 
        lChangeExpMul,
        lOldExp,
        lOldExpMul,
        lNewExp,
        lNewExpMul,
        pSrcDepict);
}

//
///*<!--��һ�þ���-->
//<Table name="t081_player_GetOccuExp_log" log_no="81" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="alone"    Item_name="act_type"        data_type="int32"></item><!--���Ŀ�����ͣ�0�����1��NPC��-->
//<item type="alone"    Item_name="cur_occu"        data_type="int32"></item><!--��ǰְҵ-->
//<item type="alone"    Item_name="delta_exp"       data_type="int32"></item><!--�仯�ľ���ֵ-->
//<item type="alone"    Item_name="old_exp"         data_type="int32"></item><!--�仯ǰ�ľ���ֵ-->
//<item type="alone"    Item_name="new_exp"         data_type="int32"></item><!--�仯��ľ���ֵ-->
//<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������Դ��˵��-->
//</Table>*/
bool LogicLogInterface::logT081_player_getoccuexp_log(CPlayer *pPlayer,DWORD dwType, LONG byOccu, LONG lChangeExp, 
                                                      LONG lOldExp, LONG lNewExp, const char *pSrcDepict)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pSrcDepict)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(81, strTime.c_str(),
        &(pPlayer->GetExID()), pPlayer->GetName(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        dwType,
        byOccu,
        lChangeExp, 
        lOldExp,
        lNewExp,
        pSrcDepict);
}

///*
//<!--���������־-->
//<Table name="t100_player_level_log" log_no="100" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--����ֵ����-->
//<item type="alone"    Item_name="last_level"      data_type="int32"></item><!--֮ǰ����-->
//<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--�����󼶱�-->
//<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������˵��-->
//</Table>
//*/
bool LogicLogInterface::logT100_player_level_log(CPlayer *pPlayer, LONG lLastLevel, const char *pSrcDepict)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pSrcDepict)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(100, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()), pPlayer->GetName(),
        pPlayer->GetExp(),
        lLastLevel,
        pPlayer->GetLevel(),
        pSrcDepict
        );
}

///*
//<!--���ְҵ�ȼ�������־-->
//<Table name="t101_player_Occulevel_log" log_no="101" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="curr_occu"       data_type="int32"></item><!--��ǰְҵ-->
//<item type="alone"    Item_name="curr_exp"        data_type="int32"></item><!--����ֵ����-->
//<item type="alone"    Item_name="last_level"      data_type="int32"></item><!--֮ǰ����-->
//<item type="alone"    Item_name="curr_level"      data_type="int32"></item><!--�����󼶱�-->
//<item type="alone"    Item_name="src_depict"      data_type="str128"></item><!--������˵��-->
//</Table>
//*/
bool LogicLogInterface::logT101_player_Occulevel_log(CPlayer *pPlayer, LONG lLastLevel, const char *pSrcDepict)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pSrcDepict)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(101, strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetExID()), pPlayer->GetName(),
        (LONG)pPlayer->GetOccupation(),
        pPlayer->GetExp(),
        lLastLevel,
        pPlayer->GetLevel(),
        pSrcDepict
        );
}



///*
//<!--================================================================-->
//<!--���PK��־-->
//<Table name="t130_player_attack_log" log_no="130" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name="sponsor"></item>
//<item type="player"   head_name="participator"></item>
//</Table>
//*/
bool LogicLogInterface::logT130_player_attack_log(CPlayer *pSponsor, CPlayer *pParticipator)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pSponsor || NULL == pParticipator)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pParticipator->GetRegionGUID()));
    if(!pRegion)
        return false;

    long posX = pParticipator->GetTileX();
    long posY = pParticipator->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(130, strTime.c_str(),
        pRegion->GetID(), pRegion->GetName(), posX, posY,
        &(pSponsor->GetExID()), pSponsor->GetName(),
        &(pParticipator->GetExID()), pParticipator->GetName()
        );
}

///*
//<!--================================================================-->
//<!--���������־-->
//<Table name="t140_player_pkdie_log" log_no="140" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="player"   head_name="decedent"></item>
//<item type="objcet"   head_name="attacker"></item>
//</Table>
//*/
bool LogicLogInterface::logT140_player_pkdie_log(CPlayer *pPlayer, CMoveShape *pMoveShape)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer || NULL == pMoveShape)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(!pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(140, strTime.c_str(),
        pRegion->GetID(), pRegion->GetName(), posX, posY,
        &(pPlayer->GetExID()), pPlayer->GetName(),
        &(pMoveShape->GetExID()), pMoveShape->GetName()
        );
}


//<!--�������-->
//<Table name="t350_team_invite" log_no="35">
//<item type="date"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="object"   head_name="team"></item>
//<item type="player"   head_name="sponsor"></item>
//<item type="player"   head_name="participator"></item>
//</Table>
bool LogicLogInterface::LogT350_team_join(GSTeam *pTeam, CPlayer *pPlayer)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;


    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));


    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    if(NULL != pRegion)
    {
        return m_pLogClient->SendLogToServer(350, strTime.c_str(),
            pRegion->GetID(),		pRegion->GetName(), 
            posX,					posY, 
            &(pTeam->GetExID()),	pTeam->GetName(), 
            &(pPlayer->GetExID()),	pPlayer->GetName()
            );
    }
    else
    {
        long lID = 0;
        return m_pLogClient->SendLogToServer(350, strTime.c_str(),
            lID,		"������ͼ���ܱ�ע��", 
            &posX,					&posY, 
            &(pTeam->GetExID()),	pTeam->GetName(), 
            &(pPlayer->GetExID()),	pPlayer->GetName()
            );
    }
}


//<!--���鿪��-->
//<Table name="t360_team_fire" log_no="36">
//<item type="date"     head_name=""></item>
//<item type="pos"      head_name=""></item>
//<item type="object"   head_name="team"></item>
//<item type="player"   head_name="sponsor"></item>
//<item type="player"   head_name="participator"></item>
//</Table>
bool LogicLogInterface::LogT360_team_leave(GSTeam *pTeam, CPlayer *pPlayer)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    if (NULL != pRegion)
    {
        return m_pLogClient->SendLogToServer(360, strTime.c_str(),
            pRegion->GetID(),		pRegion->GetName(), 
            posX,					posY, 
            &(pTeam->GetExID()),	pTeam->GetName(), 
            &(pPlayer->GetExID()),	pPlayer->GetName()
            );
    }
    else
    {
        long lID = 0;
        return m_pLogClient->SendLogToServer(360, strTime.c_str(),
            lID,		"������ͼ���ܱ�ע��", 
            posX,					posY, 
            &(pTeam->GetExID()),	pTeam->GetName(), 
            &(pPlayer->GetExID()),	pPlayer->GetName()
            );
    }
}


//<!--��Ʒ�ֽ���־-->
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="goods"    head_name=""></item><!--��Ʒ-->
//	<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
//	<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
//	<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
//</Table>
//*/
bool LogicLogInterface::logT380_goods_disassemble_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;


    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(380, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
        &guid, bIsAdd, lAddType
        );
}

//
///* --------------------------------------------------------------------
//<!--��Ʒ�ϳ���־-->
//<Table name="t390_goods_synthesize_log" log_no="39" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="goods"    head_name=""></item><!--��Ʒ-->
//	<item type="alone"    Item_name="opt_id"            data_type="guid"></item><!--�ֽ������ID��-->
//	<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=ɾ���ģ�1=��ӵ�-->
//	<item type="alone"    Item_name="change_type"       data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
//</Table>
//*/
bool LogicLogInterface::logT390_goods_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, const CGUID& guid, BOOL bIsAdd)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(390, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(), 
        &guid, bIsAdd, lAddType
        );
}

//
///* --------------------------------------------------------------------
//<!--��Ʒ������־-->
//<Table name="t400_goods_upgrade_log" log_no="40" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
//	<item type="goods"    head_name=""></item><!--����������Ʒ-->
//	<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
//	<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
//</Table>
//*/
bool LogicLogInterface::logT400_goods_upgrade_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lOldLevel, long lNewLevel)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(400, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        &guid,
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
        lOldLevel, lNewLevel
        );
}

//
///* --------------------------------------------------------------------
//<!--��Ʒħ����־-->
//<Table name="t41_goods_plugevil_log" log_no="41" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="alone"    Item_name="opt_id"            data_type="guid"></item>  <!--������ID��-->
//	<item type="goods"    head_name=""></item><!--����������Ʒ-->
//	<item type="alone"    Item_name="item_index"     data_type="int32"></item> <!--�޸ĵ�ħ����λ-->
//	<item type="alone"    Item_name="old_level"      data_type="int32"></item><!--����ǰ�ȼ�-->
//	<item type="alone"    Item_name="new_level"      data_type="int32"></item><!--������ȼ�-->
//</Table>
//*/
bool LogicLogInterface::logT410_goods_plugevil_log(CPlayer *pPlayer, const CGUID& guid, CGoods *pGoods, long lItemIndex, long lOldLevel, long lNewLevel)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(410, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        &guid,
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(),
        lItemIndex, lOldLevel, lNewLevel
        );
}

///*
//<!--ħ�����ϻ����־-->
//<Table name="t420_evil_stuff_synthesize_log" log_no="42" log_pos="1">
//	<item type="client_head"     head_name=""></item>
//	<item type="player"   head_name=""></item>
//	<item type="goods"    head_name=""></item><!--���ϳɵ���Ʒ-->
//	<item type="alone"    Item_name="add_type"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
//	<item type="alone"    Item_name="get_num"      data_type="int32"></item><!--1=�������Ʒ��2=�޸�����-->
//</Table>
//*/
bool LogicLogInterface::logT420_evil_stuff_synthesize_log(CPlayer *pPlayer, CGoods *pGoods, long lAddType, long lGetNum)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(420, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName(), lAddType, lGetNum
        );
}

///* --------------------------------------------------------------------
//<!--��Ʒ�󶨺Ͷ�����־-->
//<Table name="t421_goods_bind_and_frost" log_no="421" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item><!--����������Ʒ-->
//<item type="goods"    head_name=""></item><!--���ĵ���Ʒ-->
//<item type="alone"    Item_name="opt_type"          data_type="int32"></item><!--0=�󶨣�1=����󶨣� 2=���ᣬ3=�������-->
//</Table>
//*/
bool LogicLogInterface::logT421_goods_bind_and_frost(CPlayer *pPlayer, CGoods *pAim, CGoods *pStuff, long lType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if (NULL == pPlayer || NULL == pAim || NULL == pStuff) 
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(421, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pAim->GetBasePropertiesIndex(), &(pAim->GetExID()), pAim->GetName(), 
        pStuff->GetBasePropertiesIndex(), &(pStuff->GetExID()), pStuff->GetName(), 
        lType
        );
}


//<!--�Ǹ�ְҵ������־-->
//<Table name="t422_goods_tailor_system" log_no="422" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name="aim"></item><!--����������Ʒ-->
//</Table>
bool LogicLogInterface::logT422_goods_tailor_system(CPlayer *pPlayer, CGoods *pGoods)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer || NULL == pGoods) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(422, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName()
        );
}

//<!--������־-->
//<Table name="t430_chat_log" log_no="430" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name="sender"></item>
//<item type="alone"	  Item_name="_accepter_name" data_type="str128"></item>
//<item type="alone"	  Item_name="_chat_text" 	data_type="str512"></item>
//<item type="alone"    Item_name="region_id" data_type="int32"></item>
//<item type="alone"    Item_name="msg_type"      data_type="int32"></item>
//<!--
//��Ϣ����
//# 1. ��Ļ
//#	2. ����
//#	3. ���
//#	4. ����
//#	5. ���
//#	6. ����
//#	7. GMָ��
//-->
//</Table>
bool LogicLogInterface::logT430_chat_log(CPlayer* pPlayer,const char* pAccepterName,const char* pChatText,long lType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;


    if (NULL == pPlayer || pAccepterName == NULL || pChatText == NULL) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(430, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),
        pAccepterName, pChatText,pPlayer->GetRegionID(),lType
        );
}

/*<!--GM������־-->
<Table name="t431_GM_log" log_no="431" log_pos="1">
<item type="client_head"     head_name=""></item>
<item type="player"   head_name="GM"></item>
<item type="alone" Item_name="region_id" data_type="int32"></item>
<item type="alone"	  Item_name="cmd_line" data_type="str128"></item>
</Table>*/	
bool LogicLogInterface::logT431_GM_log(CPlayer* pPlayer,const char* pCommandLine)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (NULL == pPlayer ||  pCommandLine == NULL) return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(431, strTime.c_str(),
        &(pPlayer->GetExID()),	pPlayer->GetName(),pPlayer->GetRegionID(),
        pCommandLine
        );
}

///*
//<!--================================================================-->
//<!--�л�������־-->
//<Table name="t440_change_map_log" log_no="440" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="src_region_id"      data_type="int32"></item>
//<item type="alone"    Item_name="aim_region_id"      data_type="int32"></item>
//<item type="alone"    Item_name="switch_type"      data_type="int32"></item>
//<!--
//�л�����
//# 1. ������
//#	2. ��������
//#	3. ����������
//-->
//</Table>
//*/
bool LogicLogInterface::logT440_change_map_log(CPlayer *pPlayer, LONG lSrcRegionID, LONG lAimRegionID, LONG lType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(440,strTime.c_str(),
        &(pPlayer->GetExID()), pPlayer->GetName(),
        lSrcRegionID,
        lAimRegionID,
        lType
        );
}

//
//<!--�������-->
//<Table name="t460_quest_opt_log" log_no="460" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<!--����Ŀ��-->
//<item type="alone"    Item_name="quest_id"      data_type="int32"></item><!--��������id-->
//<item type="alone"    Item_name="opt_type"      data_type="int32"></item><!--�������ͣ�1=��ӡ�2=��� 3=ɾ��-->
//<item type="alone"    Item_name="TeamID"        data_type="guid"></item><!--����GUID-->
//</Table>
//
bool LogicLogInterface::logT460_quest_log(CPlayer* pPlayer,DWORD dwQuestID ,DWORD dwQuestOperType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

    if (pPlayer== NULL)
    {
        return false;
    }

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(460, strTime.c_str(),
        &(pPlayer->GetExID()),pPlayer->GetName(),
        dwQuestID,dwQuestOperType,&(pPlayer->GetTeamID()));
}

//
//<!--�ͽ�����ˢ�²���-->
//<Table name="t461_mercquest_log" log_no="461" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="pos"      head_name=""></item>
//<!--����Ŀ��-->
//<item type="alone"    Item_name="TeamID"         data_type="guid"></item><!--����GUID-->
//<item type="alone"    Item_name="usegoods"       data_type="int32"></item><!--�Ƿ�ʹ����Ʒ��1�ǣ�0��-->
//<item type="alone"    Item_name="quest_id1"      data_type="int32"></item><!--ˢ��������1-->
//<item type="alone"    Item_name="quest_id2"      data_type="int32"></item><!--ˢ��������2-->
//<item type="alone"    Item_name="quest_id3"      data_type="int32"></item><!--ˢ��������3-->
//<item type="alone"    Item_name="quest_id4"      data_type="int32"></item><!--ˢ��������4-->
//</Table>

bool LogicLogInterface::logT461_mercquest_log(CPlayer* pPlayer,DWORD bUsegoods,DWORD quest1,DWORD quest2,DWORD quest3,DWORD quest4)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;
    if(NULL == pPlayer)
        return false;
    CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));
    if(NULL == pRegion)
        return false;

    long posX = pPlayer->GetTileX();
    long posY = pPlayer->GetTileY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(461,strTime.c_str(),
        &(pPlayer->GetExID()),pPlayer->GetName(),
        pRegion->GetID(),pRegion->GetName(),posX,posY,
        &(pPlayer->GetTeamID()),
        bUsegoods,quest1,quest2,quest3,quest4);
}

//<!--��ȯ������־-->
//<Table name="t501_Ticket_consume_log" log_no="501" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="goods"    head_name=""></item>
//<item type="alone"    Item_name="goods_price"     data_type="int32"></item>
//<!--��Ʒ�۸�-->
//<item type="alone"    Item_name="goods_num"       data_type="int32"></item>
//<!--��Ʒ����-->
//<item type="alone"    Item_name="gold_remain"   data_type="int32"></item>
//<!--ʣ����-->
//<item type="alone"    Item_name="ticket_remain"   data_type="int32"></item>
//<!--ʣ���ȯ-->
//<item type="alone"    Item_name="act_type"        data_type="int32"></item>
//<!--1����ȯ��2:����� -->
//</Table>
bool LogicLogInterface::logT501_Ticket_consume_log(CPlayer* pPlayer,DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,
                                                   DWORD dwPrice,DWORD dwGoodsNum,DWORD dwMoney,DWORD dwTicket,DWORD dwActType)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(501,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),dwGoodsId,&goodsGuid,strGoodsName,dwPrice,dwGoodsNum,dwMoney,dwTicket,dwActType);
}

//<!--��ȯ��ȡ��־-->
//<Table name="t502_Ticket_gain_log" log_no="502" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="gain_num"       data_type="int32"></item>
//<item type="alone"    Item_name="ticket_num"       data_type="int32"></item>
//</Table>
bool LogicLogInterface::logT502_Ticket_gain_log(CPlayer* pPlayer,int nGainNum)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(502,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),nGainNum,pPlayer->GetTicket());
}

///*<!--��һ�ȡ��־-->
//	  <Table name="t503_Gold_gain_log" log_no="503" log_pos="1">
//		<item type="client_head"     head_name=""></item>
//		<item type="player"   head_name=""></item>    
//		<item type="alone"    Item_name="old_num"       data_type="int32"></item>
//		<item type="alone"    Item_name="new_num"       data_type="int32"></item>
//		<item type="alone"    Item_name="flag"          data_type="int32"></item>
//	  </Table>*/
bool LogicLogInterface::logT503_Gold_gain_log(CPlayer* pPlayer,int nOldNum,int nNewNum,int nFlag)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(503,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),nOldNum,nNewNum,nFlag);
}

//<!--λ��仯��־-->
//<Table name="t504_weimian_log" log_no="504" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="weimian_num"       data_type="int32"></item>
//<!--0���� 1����-->
//<item type="alone"    Item_name="act_type"       data_type="int32"></item>
//<item type="alone"    Item_name="flag"          data_type="int32"></item>
//</Table>
bool LogicLogInterface::logT504_weimian_log(CPlayer* pPlayer,int nNum,int nActType,int nFlag)
{
    if(NULL == m_pLogClient)return false;
    if(GetGame()->GetSetup()->lUseLogServer==0)
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(504,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),nNum,nActType,nFlag);
}


//<!--gs����SNDA url�Ʒ�����-->
//<Table name="t521_snda_url_send_log" log_no="521" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="orderId" data_type="str128"></item>
//<item type="alone"    Item_name="cdkey"   data_type="str128"></item>
//<item type="alone"    Item_name="total_amount"    data_type="int32"></item> 
//<item type="alone"    Item_name="goodsinfo"   data_type="str128"></item>    
//</Table>
bool LogicLogInterface::logT521_snda_url_send_log(CPlayer* pPlayer,const char* szOrderId,int nTotalAmount,
                                                  const char* szGoodsInfo)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(521,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),
        szOrderId,pPlayer->GetAccount(),nTotalAmount,szGoodsInfo);	
}

//<!--gs����SNDA url�Ʒ�����-->
//<Table name="t522_snda_ticket_consume_log" log_no="522" log_pos="1">
//<item type="client_head"     head_name=""></item>
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="orderId" data_type="str128"></item>
//<item type="alone"    Item_name="cdkey"   data_type="str128"></item>
//<item type="alone"    Item_name="sendTimes"    data_type="str128"></item> 
//<item type="alone"    Item_name="goodsinfo"    data_type="str128"></item> 
//<item type="alone"    Item_name="payerrormsg"  data_type="str128"></item> 
//<item type="alone"    Item_name="retcode"  data_type="int32"></item> 
//</Table>
bool LogicLogInterface::logT522_snda_ticket_consume_log(CPlayer* pPlayer,const char* szOrderId,const char* szSendTime,
                                                        const char* szGoodsInfo,const char* payErrorMsg,int retcode)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(522,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),
        szOrderId,pPlayer->GetAccount(),szSendTime,szGoodsInfo,payErrorMsg,retcode);	
}

//<!--��ȡ�ʼ���־-->
//<Table name="t600_receive_mail_log" log_no="600" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
//<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
//<item type="alone"    Item_name="flag"          	   data_type="int32"></item><!--�������ͣ�1=������ȡ��2=���߽���-->
//</Table>
bool LogicLogInterface::logT600_Receive_mail_log(CPlayer *pPlayer,const CGUID &MailGuid,const char *strWriter,
                                                 int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/,int nFlag)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(600,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),&MailGuid,strWriter,nType,lGold,lGoodsNum/*,lIncGoodsNum*/,nFlag);
}

///*
//<Table name="t601_send_mail_log" log_no="601" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
//<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����--> 
//<item type="alone"	  Item_name="goodsinfo"				   data_type="str512"></item><!--��Ʒ-->    
//</Table>
//*/
bool LogicLogInterface::logT601_Send_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,
                                              int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/,const char *strGoodsInfo)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(601, strTime.c_str(),&MailGuid,strReceiver, strWriter,nType,lGold,lGoodsNum/*,lIncGoodsNum*/,strGoodsInfo);
}

///*<!--���ʼ���ȡ��Ʒ��־-->
//<Table name="t602_getgoods_mail_log" log_no="602" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
//<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->
//<item type="goods"    head_name=""></item>
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
//</Table>
bool LogicLogInterface::logT602_GetGoods_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,
                                                  DWORD dwGoodsId,const CGUID& goodsGuid,const char* strGoodsName,int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(602, strTime.c_str(),&MailGuid,strReceiver, 
        strWriter,dwGoodsId,&goodsGuid,strGoodsName,nType,lGold,lGoodsNum/*,lIncGoodsNum*/);
}

//<!--���ʼ���ȡ�����־-->
//<Table name="t603_getgold_mail_log" log_no="603" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->
//<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->    
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
//</Table>
bool LogicLogInterface::logT603_GetGold_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,
                                                 int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(603, strTime.c_str(),&MailGuid,strReceiver, strWriter,nType,lGold,lGoodsNum/*,lIncGoodsNum*/);
}

//<!--��ȡ�ʼ���־-->
//<Table name="t604_read_mail_log" log_no="604" log_pos="1">
//<item type="client_head"     head_name=""></item>    
//<item type="player"   head_name=""></item>
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->		
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
//<item type="alone"    Item_name="flag"          	   data_type="int32"></item><!--�������ͣ�1=�ɹ���0=ʧ��-->
//</Table>
bool LogicLogInterface::logT604_read_mail_log(CPlayer *pPlayer,const CGUID &MailGuid,int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/,int nFlag)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(604,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(), &MailGuid,nType,lGold,lGoodsNum/*,lIncGoodsNum*/,nFlag);
}

///*
//<!--ɾ���ʼ���־-->
//<Table name="t605_delete_mail_log" log_no="605" log_pos="1">
//<item type="client_head"     head_name=""></item> 
//<item type="alone"    Item_name="mail_guid"            data_type="guid"></item><!--�ʼ�guid-->   
//<item type="alone"	  Item_name="receiver"			   data_type="str128"></item><!--������-->
//<item type="alone"	  Item_name="writer"			   data_type="str128"></item><!--������-->    
//<item type="alone"	  Item_name="type"			   	   data_type="int32"></item><!--�ʼ�����-->
//<item type="alone"	  Item_name="gold"			   	   data_type="int32"></item><!--��Ǯ--> 
//<item type="alone"	  Item_name="goodsnum"			   data_type="int32"></item><!--��Ʒ����-->
//<item type="alone"	  Item_name="incgoodsnum"		   data_type="int32"></item><!--��ֵ��Ʒ����-->    
//</Table>*/
bool LogicLogInterface::logT605_delete_mail_log(const CGUID &MailGuid,const char *strReceiver,const char *strWriter,
                                                int nType,long lGold,long lGoodsNum/*,long lIncGoodsNum*/)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(605, strTime.c_str(),&MailGuid,strReceiver, strWriter,nType,lGold,lGoodsNum/*,lIncGoodsNum*/);
}

/*
<!--���������NPC��������־-->
<Table name="t700_business_npc_log" log_no="700" log_pos="1">
<item type="client_head" head_name=""></item>
<item type="pos" head_name=""></item>
<item type="player" head_name=""></item>
<item type="goods" head_name=""></item>
<item type="alone" Item_name="act_type" data_type="int32"></item> <!--�������ͣ�1����2����-->
<item type="alone" Item_name="goods_amount" data_type="int32"></item> <!--��������-->
<item type="alone" Item_name="trade_spot" data_type="int32"></item> <!--ó�׵�ID-->
<item type="alone" Item_name="spent_money" data_type="int32"></item> <!--���ѵ�Ǯ-->
</Table>
*/
bool LogicLogInterface::logT700_business_npc( CPlayer *pPlayer, CGoods *pGoods, int iTradeAmount, 
                                             int iTradeMoney, int iActType, int iTradeSpot )
{
    if(NULL == m_pLogClient)return false;
    if( 0 == GetGame()->GetSetup()->lUseLogServer )
    {
        return true;
    }

    if( NULL == pPlayer || NULL == pGoods )
    {
        return false;
    }
    CRegion *pRegion = static_cast<CRegion*>( GetGame()->FindRegion( pPlayer->GetRegionGUID() ) );
    if( pRegion == NULL )
    {
        return false;
    }

    string strTime;
    GetCurrTimeString(strTime);

    float fPosX = pPlayer->GetPosX();
    float fPosY = pPlayer->GetPosY();

    return m_pLogClient->SendLogToServer(700,strTime.c_str(),
        pRegion->GetID(), pRegion->GetName(), &fPosX, &fPosY,
        &pPlayer->GetExID(), pPlayer->GetName(), 
        pGoods->GetBasePropertiesIndex(), &pGoods->GetExID(), pGoods->GetName(), 
        iActType, 
        iTradeAmount,
        iTradeSpot,
        iTradeMoney );
}


///*
//<!--================================================================-->
//<!--ÿ��ͼ�ϵ��������¼-->
//<Table name="t800_player_num_in_region" log_no="800" log_pos="1">
//<item type="alone" Item_name="gs_id" data_type="int32"></item>
//<item type="client_head" head_name=""></item>
//<item type="alone" Item_name="region_name" data_type="int32"></item>
//<item type="alone" Item_name="region_id" data_type="int32"></item>
//<item type="alone" Item_name="region_guid" data_type="int32"></item>
//<item type="alone" Item_name="player_num" data_type="int32"></item> <!--�������-->
//</Table>
//*/
//
bool LogicLogInterface::logT800_player_num_in_region(LONG lGsID, CServerRegion *pCServerRegion)
{

    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;
    if(NULL == m_pLogClient)
        return false;
    if(NULL == pCServerRegion)
        return false;

    SYSTEMTIME	stTime;
    char		szTime[128] = {0};
    GetLocalTime(&stTime);

    LONG lYu = stTime.wMinute % 5;
    if(0 != lYu)
    {
        stTime.wMinute -= (ushort)lYu;
    }

    _snprintf(szTime,128, "%d-%d-%d %d:%d:00" ,
        stTime.wYear,
        stTime.wMonth,
        stTime.wDay,
        stTime.wHour,
        stTime.wMinute,
        stTime.wSecond);

    return m_pLogClient->SendLogToServer(
        800, szTime,
        lGsID,
        pCServerRegion->GetName(),
        pCServerRegion->GetID(),
        &(pCServerRegion->GetExID()),
        pCServerRegion->GetPlayerAmout()
        );

}


//<!--�������־��¼��T���߼��ս����־-->
//<Table name="t900_anticheat_player_log" log_no="900" log_pos="1">
//<item type="client_head" head_name=""></item>			<!-- ʱ�� -->
//<item type="pos" head_name=""></item>				<!-- λ�� -->
//<item type="player" head_name=""></item>			<!-- ��� -->	
//<item type="alone" Item_name="log_type" data_type="str128"></item> <!-- �������� -->
//<item type="alone" Item_name="ACC_WRONG" data_type="int32"></item> <!--�ۼ����ֵ-->
//<item type="alone" Item_name="CUR_WRONG" data_type="int32"></item> <!--��ǰ���ֵ-->
//<item type="alone" Item_name="CUR_BONUS" data_type="int32"></item> <!--�����ۼƽ�������-->
//</Table>
bool LogicLogInterface::logT900_anticheat_player_log(CPlayer * player, const char *log_type, 
                                                     int acc_wrong, int cur_wrong, int acc_bonus)
{
    if(NULL == m_pLogClient)return false;
    if ( 0 == GetGame()->GetSetup()->lUseLogServer )
    {
        return true;
    }

    if ( NULL == player )
    {
        return false;
    }
    CRegion *pRegion = static_cast<CRegion*>( GetGame()->FindRegion(player->GetRegionGUID()) );
    if (pRegion == NULL)
    {
        return false;
    }

    float fPosX = player->GetPosX();
    float fPosY = player->GetPosY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(900,strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),&fPosX,&fPosY,
        &player->GetExID(),player->GetName(),
        log_type,
        acc_wrong, cur_wrong, acc_bonus );
}

//<!--�������־��¼�����������־-->
//<Table name="t901_anticheat_player_log" log_no="901" log_pos="1">
//<item type="client_head" head_name=""></item>			<!-- ʱ�� -->
//<item type="pos" head_name=""></item>				<!-- λ�� -->
//<item type="player" head_name=""></item>			<!-- ��� -->	
//<item type="alone" Item_name="oper_type" data_type="str128"></item> <!-- ��������:ANSWER, TIMEOUT, REFUSED, EXIT -->
//<item type="alone" Item_name="ACC_WRONG" data_type="int32"></item> <!--�ۼ����ֵ-->
//<item type="alone" Item_name="CUR_WRONG" data_type="int32"></item> <!--��ǰ���ֵ-->
//<item type="alone" Item_name="CUR_RIGHT" data_type="int32"></item> <!--��ǰ������Դ���-->
//<item type="alone" Item_name="CUR_BONUS" data_type="int32"></item> <!--�����ۼƽ�������-->
//<item type="alone" Item_name="PK" data_type="int32"></item> <!--��ǰPKֵ-->
//<item type="alone" Item_name="RHAnswered" data_type="int32"></item> <!--�Ƿ��ڻƽ�ʱ�������ȷ��-->
//</Table>
bool LogicLogInterface::logT901_anticheat_answer_log( CPlayer *player, const char *log_type, 
                                                     int acc_wrong, int cur_wrong, int cur_right, int acc_bonus, int pk, bool RHAnswered )
{
    if(NULL == m_pLogClient)return false;
    if ( 0 == GetGame()->GetSetup()->lUseLogServer )
    {
        return true;
    }

    if ( NULL == player )
    {
        return false;
    }
    CRegion *pRegion = static_cast<CRegion*>( GetGame()->FindRegion(player->GetRegionGUID()) );
    if (pRegion == NULL)
    {
        return false;
    }

    float fPosX = player->GetPosX();
    float fPosY = player->GetPosY();

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(901,strTime.c_str(),
        pRegion->GetID(),pRegion->GetName(),&fPosX,&fPosY,
        &player->GetExID(),player->GetName(),
        log_type,
        acc_wrong, cur_wrong, cur_right, acc_bonus, pk, ( RHAnswered ? 1 : 0 ) );
}

//<!--�������־��¼�����������־-->
//<Table name="t902_player_progress_chg_log" log_no="902" log_pos="1">
//<item type="client_head" head_name=""></item>    
//<item type="player" head_name=""></item>    
//<!--��״̬-->
//<item type="alone" Item_name="old_progress" data_type="int32"></item>
//<!--��״̬-->
//<item type="alone" Item_name="new_progress" data_type="int32"></item>    
//</Table>
bool LogicLogInterface::logT902_player_progress_chg_log(CPlayer* pPlayer,int nOldProgress,int nNewProgress)
{
    if(NULL == m_pLogClient)return false;
    if(0 == GetGame()->GetSetup()->lUseLogServer) 
        return true;

    if(NULL == pPlayer)
        return false;

    string strTime;
    GetCurrTimeString(strTime);

    return m_pLogClient->SendLogToServer(902,strTime.c_str(),&(pPlayer->GetExID()),pPlayer->GetName(),nOldProgress,nNewProgress);
}

bool LogicLogInterface::logT987_auction_house_log( const CGUID &playerID, const char *opDesc, int cnt, int price )
{
    if( m_pLogClient == NULL )
    {
        return false;
    }
    CPlayer *player = GetGame()->FindPlayer( playerID );
    if( player == NULL )
    {
        return false;
    }

    string strTime;
    GetCurrTimeString(strTime);
    return m_pLogClient->SendLogToServer( 987, strTime.c_str(), &player->GetExID(), player->GetName(), opDesc, cnt, price );
}
