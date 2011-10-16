

#include "stdafx.h"
#include "GoodsTailorSystem.h"
#include "Player.h"
#include "goods\CGoodsFactory.h"
#include "container\CTestContainer.h"
#include "Message Packaging\CS2CContainerObjectAmountChange.h"

//!-------------------------------------------------------------------------------------------------

//!		������䷽Ϊ���������Ʒ
DWORD	GsOneMakeTable::MakeGoods(CPlayer *pPlayer) const
{
	if(NULL == pPlayer)
		return eGTC_UnknownErr;

	//! �������б�
	vector<tagTestArg_ID_Num>vecArg;
	for (LONG i = 0; i < m_vExpend.size(); ++i)
	{
		if(!pPlayer->FindGoodsByIndex(m_vExpend[i].lGoodsIndex, m_vExpend[i].lGoodsNum))
			return eGTC_ErrStuff;
		tagTestArg_ID_Num tagTestArg;
		tagTestArg._dwGoodsID = m_vExpend[i].lGoodsIndex;
		tagTestArg._dwNum		= m_vExpend[i].lGoodsNum;
		vecArg.push_back(tagTestArg);
	}

	//! ɾ������
	CTestContainer testContainer;
	testContainer.Init(pPlayer);
	vector<tagDelTestResult> vecDelResult;
	BOOL bDelSucceed = testContainer.DelTest(vecArg,&vecDelResult);
	if(!bDelSucceed)
		return eGTC_ErrStuff;

	//! ���ɲ���
	vector<CGoods*> vAddGoods;
	for (LONG i = 0; i < m_vProduction.size(); ++i)
	{
		//Ψһ���	
		CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(m_vProduction[i].lGoodsIndex);
		if(pBaseProperty && pBaseProperty->IsHasAddonPropertie(GAP_UNIQUE))
		{
			
			if(!pPlayer->IsGoodsUnique(CGoodsFactory::QueryGoodsOriginalName(m_vProduction[i].lGoodsIndex)))
			{
				char* strMsg=CStringReading::LoadString(20,49);
				if(strMsg)
				{
					CMessage msg(MSG_S2C_OTHER_GAMEMESSAGE);
					msg.Add(strMsg);
					msg.Add((long)0);
					msg.Add(CGUID::GUID_INVALID);
					msg.SendToPlayer(pPlayer->GetExID());
				}
				return eGTC_UnknownErr;
			}
		}		


		if(0 >= m_vProduction[i].lOutOdds)
		{
			//! ���Լ������ô����������
			continue;
		}

		if(0 == (rand() % m_vProduction[i].lOutOdds))
		{
			CGoodsFactory::CreateGoods(m_vProduction[i].lGoodsIndex, m_vProduction[i].lGoodsNum, vAddGoods,1);
		}
	}

	//! ��Ӳ���
	vector<tagTestResult> vecResult;
	BOOL bAddSucced = testContainer.AddTest(vAddGoods,&vecResult);
	if(!bAddSucced)
	{
		for( LONG i = 0; i < vAddGoods.size(); i ++ )
		{
			if( vAddGoods[i] )
				CGoodsFactory::GarbageCollect(&(vAddGoods[i]), 0);
		}
		vAddGoods.clear();
		return eGTC_NoEnoughSpace;
	}

	pPlayer->DelGoodsByTestResult(vecDelResult);
	

	//! ɾ������
	//for (LONG i = 0; i < m_vExpend.size(); ++i)
	//{
	//	if(!pPlayer->DelGoodsByIndex(m_vExpend[i].lGoodsIndex, m_vExpend[i].lGoodsNum))
	//	{
	//		AddErrorLogText("GsOneMakeTable::MakeGoods�з������ó��ֵĴ����ҵ�����Ʒ[����%d������%d]����ɾ��!", m_vExpend[i].lGoodsIndex, m_vExpend[i].lGoodsNum);
	//		return eGTC_UnknownErr;
	//	}
	//}


	//! ��Ӳ��ﵽ�������
	for( LONG i = 0; i < vAddGoods.size(); i ++ )
	{
		if( vAddGoods[i] )
		{
			pPlayer->AddGoodsWithProtect(vAddGoods[i]);
			GetGameLogInterface()->logT422_goods_tailor_system(pPlayer, vAddGoods[i]);
		}
	}

	if(0 < vAddGoods.size())
		return eGTC_Succeed;
	else
		return eGTC_UnknownErr;
}



//!-------------------------------------------------------------------------------------------------
/*
MSG_S2C_MAKE_BEGIN,										//! ��ʼһ������
MSG_S2C_MAKE_RESULT,									//! ����һ���������
*/

GoodsTailorSystem GoodsTailorSystem::c_GoodsTailorSystem;


//! ��ҿ���һ����Ʒ����������
BOOL GoodsTailorSystem::PlayerMakeBeginByGoods(const CGUID &PlayerGuid, const CGUID &GoodsGuid, LONG lMakeTableID, BOOL bDelGoodsWhenFinish)
{
	const GsOneMakeTable *pGsOneMakeTable = (const GsOneMakeTable*)GoodsTailorSetup::GetInstance().FindOneMakeTable(lMakeTableID);
	if(NULL == pGsOneMakeTable)
	{
		return FALSE;
	}

	map<CGUID, tagMakeInfo>::iterator ite = m_mapMakingPlayer.find(PlayerGuid);
	if(m_mapMakingPlayer.end() == ite)
	{
		m_mapMakingPlayer.insert(pair<CGUID, tagMakeInfo>(PlayerGuid, tagMakeInfo()));
		ite = m_mapMakingPlayer.find(PlayerGuid);
	}

	ite->second.lType				= eMT_FORM_GOODS;
	ite->second.PlayerGuid			= PlayerGuid;
	ite->second.pGsOneMakeTable		= pGsOneMakeTable;

	ite->second.GoodsGuid			= GoodsGuid;
	ite->second.bDelGoodsWhenFinish	= bDelGoodsWhenFinish;

	CMessage msg(MSG_S2C_MAKE_BEGIN);
	msg.Add(GoodsGuid);
	DBWriteSet writeSet;
	msg.GetDBWriteSet(writeSet);
	pGsOneMakeTable->Code(writeSet);

	msg.Add((LONG)eMT_FORM_GOODS);
	msg.SendToPlayer(PlayerGuid, false);

	return TRUE;
}

//! ��ҿ���һ��NPC����������
BOOL GoodsTailorSystem::PlayerMakeBeginByNpc(const CGUID &PlayerGuid, const CGUID &RegionGuid, LONG lRadius, LONG lMakeTableID)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
	if(NULL == pPlayer || 1 > lRadius)
		return FALSE;

	//! ����䷽
	const GsOneMakeTable *pGsOneMakeTable = (const GsOneMakeTable*)GoodsTailorSetup::GetInstance().FindOneMakeTable(lMakeTableID);
	if(NULL == pGsOneMakeTable)
	{
		return FALSE;
	}

	map<CGUID, tagMakeInfo>::iterator ite = m_mapMakingPlayer.find(PlayerGuid);
	if(m_mapMakingPlayer.end() == ite)
	{
		m_mapMakingPlayer.insert(pair<CGUID, tagMakeInfo>(PlayerGuid, tagMakeInfo()));
		ite = m_mapMakingPlayer.find(PlayerGuid);
	}

	ite->second.lType			= eMT_FROM_NPC;
	ite->second.PlayerGuid		= PlayerGuid;
	ite->second.pGsOneMakeTable	= pGsOneMakeTable;

	ite->second.RegionGuid		= RegionGuid;
	ite->second.fPosX			= pPlayer->GetPosX();
	ite->second.fPosY			= pPlayer->GetPosY();
	ite->second.lRadius			= lRadius;

	CMessage msg(MSG_S2C_MAKE_BEGIN);
	msg.Add(NULL_GUID);
	DBWriteSet writeSet;
	msg.GetDBWriteSet(writeSet);
	pGsOneMakeTable->Code(writeSet);

	msg.Add((LONG)eMT_FORM_GOODS);
	msg.Add((LONG)pPlayer->GetPosX());
	msg.Add((LONG)pPlayer->GetPosY());
	msg.Add(lRadius);
	msg.SendToPlayer(PlayerGuid, false);
	
	return TRUE;
}

//! ��������������
BOOL GoodsTailorSystem::PlayerMakeFinish(const CGUID &PlayerGuid)
{
	map<CGUID, tagMakeInfo>::iterator ite = m_mapMakingPlayer.find(PlayerGuid);
	if(m_mapMakingPlayer.end() == ite)
	{
		return FALSE;
	}

	if (MakeGoods(ite->second))
	{
		m_mapMakingPlayer.erase(ite);

		CMessage msg(MSG_S2C_MAKE_RESULT);
		msg.Add((BYTE)TRUE);
		msg.SendToPlayer(PlayerGuid, false);
		return TRUE;
	}

	m_mapMakingPlayer.erase(ite);
	return FALSE;
}

//! �����������ȡ��
BOOL GoodsTailorSystem::PlayerMakeCancel(const CGUID &PlayerGuid)
{
	m_mapMakingPlayer.erase(PlayerGuid);
	return TRUE;
}

//! ��������
BOOL GoodsTailorSystem::MakeGoods(const tagMakeInfo& MakeInfo)
{
	CPlayer *pPlayer = GetGame()->FindPlayer(MakeInfo.PlayerGuid);
	if(NULL == pPlayer)
		return FALSE;

	//! ��֤�Ϸ���
	switch(MakeInfo.lType)
	{
	case eMT_FORM_GOODS:
		{
			CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(MakeInfo.GoodsGuid);
			if(NULL == pGoods)
				return FALSE;
		}
		break;
	case eMT_FROM_NPC:
		{
			//! ��ͼ�ж�
			if(MakeInfo.RegionGuid != pPlayer->GetRegionGUID())
				return FALSE;
			//! �����ж�
			if(MakeInfo.lRadius < abs(LONG(pPlayer->GetPosX() - MakeInfo.fPosX)) || MakeInfo.lRadius <  abs((LONG)(pPlayer->GetPosY() - MakeInfo.fPosY)))
				return FALSE;
		}
	    break;
	default:
	    return FALSE;
	}

	//! ��ʼ����
	if(NULL == MakeInfo.pGsOneMakeTable)
	{
		assert(NULL != MakeInfo.pGsOneMakeTable);
		AddErrorLogText("GoodsTailorSystem::MakeGoods�з������ó��ֵĴ�����Ʒ�����䷽����Ϊ��!");
		return FALSE;
	}
	DWORD dwResult = MakeInfo.pGsOneMakeTable->MakeGoods(pPlayer);

	//! ������
	if(eGTC_Succeed == dwResult)
	{
		switch(MakeInfo.lType)
		{
		case eMT_FORM_GOODS:
			{
				if(MakeInfo.bDelGoodsWhenFinish)
				{
					if(!pPlayer->DelGoods(MakeInfo.GoodsGuid, 1))
					{
						AddErrorLogText("GoodsTailorSystem::MakeGoods�з������ش�����Ʒ�����Ѿ���ɣ���������Ʒû�б�ɾ���ɹ�������һ�ֿ��ܾ������õ��������а����˴�����������Ʒ!");
					}
				}
			}
			break;
		}
	}
	else
	{
		CMessage msg(MSG_S2C_MAKE_RESULT);
		msg.Add((BYTE)FALSE);
		msg.Add(dwResult);
		msg.SendToPlayer(pPlayer->GetExID(), false);
	}

	return (eGTC_Succeed == dwResult);
}


bool LogicLogInterface::logT422_goods_tailor_system(CPlayer *pPlayer, CGoods *pGoods)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

	if(NULL == m_pLogClient)
		return false;
	if (NULL == pPlayer || NULL == pGoods) return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer( 
		422, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		pGoods->GetBasePropertiesIndex(), &(pGoods->GetExID()), pGoods->GetName()
		);
}