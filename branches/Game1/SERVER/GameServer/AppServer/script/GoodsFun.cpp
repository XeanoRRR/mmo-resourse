//=============================================================================
/**
 *  file: GoodsFun.cpp
 *
 *  Brief:�������Ʒ��صĽű�����
 *			
 *
 *  Authtor:wangqiao
 *	
 *	Create Datae:2008-11-06
 */
//=============================================================================

#include "StdAfx.h"
#include "mmsystem.h"
#include "script.h"
#include "variablelist.h"
#include "../player.h"
#include "../goods/cgoods.h"
#include "../serverregion.h"
#include "../setup/IncrementShopList.h"
#include "../setup/TradeList.h"
#include "../Goods/CGoodsFactory.h"
#include "../Message Packaging/CS2CContainerObjectMove.h"
#include "../Message Packaging/CS2CContainerObjectAmountChange.h"
#include "../Listener/CSeekGoodsListener.h"
#include "..\Session\CSessionFactory.h"
#include "..\Session\CSession.h"
#include "..\Session\CPlug.h"
#include "../../../setup/LogSystem.h"
#include "../../../setup/QuestSystem.h"
#include "../../GameServer/GameManager.h"
#include "../GoodsExManage.h"
#include "../public/StrType.h"
#include "../public/StrUnifyInput.h"
#include "../Public/BounsDef.h"
#include "../container/CTestContainer.h"
#include "../GoodsTailorSystem.h"
#include "../RgnManager.h"
#include "../Country/CountryParam.h"
#include "../../../setup/TradeList.h"
#include "..\Session\CPersonalShopSeller.h"
#include "..\Session\CTrader.h"


double CScript::AddGoods(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer) return 0;

	double lRet = 1;
	char* name=GetStringParam(CmdStr, 0);
	double n = GetIntParam(CmdStr, 1);

	if (name == NULL)
		return false;

	if( n == ERROR_CODE )
	{
		n = 1;
	}

	if (n<1)
	{
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		return false;
	}

	vector<CGoods*> vOut;

	DWORD dwIndexID = CGoodsFactory::QueryGoodsIDByOriginalName(name);

	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(dwIndexID);
	if(!pBaseProperty)
		return 0;
	if(pBaseProperty->IsHasAddonPropertie(GAP_UNIQUE))
	{
		if(!pPlayer->IsGoodsUnique(name))
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
			return 0;
		}
	}

	CGoodsFactory::CreateGoods(dwIndexID, n, vOut,35);	

	//����true
	if(vOut.size() > 0)
	{
		if(vOut[0])
			lRet = 1;
	}
	
	
	
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		if( vOut[i] )
		{			
			DWORD dwGoodsId=vOut[i]->GetBasePropertiesIndex();
			CGUID goodsGuid=vOut[i]->GetExID();
			const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
			DWORD dwGoodsNum=vOut[i]->GetAmount();
			BOOL bAdded=pPlayer->AddGoodsWithProtect(vOut[i]);
			
			if(bAdded)
			{
				//AddGoods Log
				GetGameLogInterface()->logT027_goods_log_pickup(pPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwGoodsNum);
			}
		}
	}	
	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}



//! AddArmament(װ�����֡���������ȵȼ���ħ���ȼ���Ŀ���������)
double CScript::AddArmament(const char* CmdStr, char* retStr)
{
	CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pAimPlayer == NULL) return 0;

	double lRet = 1;
	char* name=GetStringParam(CmdStr, 0);
	double n = GetIntParam(CmdStr, 1);
	LONG lArmamentLevel = GetIntParam(CmdStr, 2);
	lArmamentLevel = (20 >= lArmamentLevel && 0 < lArmamentLevel) ? lArmamentLevel : 0;
	LONG lEvilLevel = GetIntParam(CmdStr, 3);
	lEvilLevel = (10 >= lEvilLevel && 0 < lEvilLevel) ? lEvilLevel : 0;
	
	

	if (name == NULL)
		return false;

	char* pAimName = GetStringParam(CmdStr, 4);
	if(NULL != pAimName)
	{
		pAimPlayer = GetGame()->FindPlayer(pAimName);
		if(NULL == pAimPlayer)
			pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		if (NULL == pAimPlayer)
		{
			M_FREE( name, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
			return false;
	}
	}

	if( n == ERROR_CODE )
	{
		n = 1;
	}

	if (n<1)
	{
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
		return false;
	}

	vector<CGoods*> vOut;
	CGoodsFactory::CreateGoods( CGoodsFactory::QueryGoodsIDByOriginalName(name), n, vOut ,36);

	//����true
	if(vOut.size() > 0)
	{
		if(vOut[0])
			//lRet = vOut[0]->GetExID();
			lRet = 1;
	}



	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		if( vOut[i] )
		{
			//! ������Ʒ
			if (0 != lArmamentLevel)
			{
				CGoodsFactory::SetEquipmentLevel( vOut[i], lArmamentLevel );
			}

			// TEST[δ���]
			GetGame()->UpdateSavePlayer(pAimPlayer, SAVE_DETAIL_ATTRPACKGOODS);


			DWORD dwGoodsId=vOut[i]->GetBasePropertiesIndex();
			CGUID goodsGuid=vOut[i]->GetExID();
			const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
			DWORD dwGoodsNum=vOut[i]->GetAmount();
			BOOL bAdded=pAimPlayer->AddGoodsWithProtect(vOut[i]);
			if(bAdded)
			{
				//AddGoods Log
				GetGameLogInterface()->logT027_goods_log_pickup(pAimPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwGoodsNum);
			}
		}
	}

	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}
double CScript::AddArmamentForGM(const char* CmdStr, char* retStr)
{
	CPlayer *pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pAimPlayer == NULL) return 0;

	double lRet = 1;
	char* name=GetStringParam(CmdStr, 0);
	double n = GetIntParam(CmdStr, 1);
	LONG lArmamentLevel = GetIntParam(CmdStr, 2);
	lArmamentLevel = (20 >= lArmamentLevel && 0 < lArmamentLevel) ? lArmamentLevel : 0;
	LONG lEvilLevel = GetIntParam(CmdStr, 3);
	lEvilLevel = (10 >= lEvilLevel && 0 < lEvilLevel) ? lEvilLevel : 0;



	if (name == NULL)
		return false;

	char* pAimName = GetStringParam(CmdStr, 4);
	if(NULL != pAimName)
	{
		pAimPlayer = GetGame()->FindPlayer(pAimName);
		if(NULL == pAimPlayer)
			pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		if (NULL == pAimPlayer)
		{
			M_FREE( name, sizeof(char)*MAX_VAR_LEN );
			M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
			return false;
	}
	}

	if( n == ERROR_CODE )
	{
		n = 1;
	}

	if (n<1)
	{
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
		return false;
	}

	vector<CGoods*> vOut;
	CGoodsFactory::CreateGoods( CGoodsFactory::QueryGoodsIDByOriginalName(name), n, vOut, 36);

	//����true
	if(vOut.size() > 0)
	{
		if(vOut[0])
			//lRet = vOut[0]->GetExID();
			lRet = 1;
	}

	if(pAimPlayer->GetTotalEmptySpaceNum() < (LONG)vOut.size())
	{
		for( size_t i = 0; i < vOut.size(); i ++ )
		{
			if( vOut[i] )
			{
				CGoodsFactory::GarbageCollect(&vOut[i], 1);
			}
		}
		vOut.clear();
		return -1;
	}

	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		if( vOut[i] )
		{
			//! ������Ʒ
			if (0 != lArmamentLevel)
			{
				CGoodsFactory::SetEquipmentLevel( vOut[i], lArmamentLevel );
			}

			// TEST[δ���]
			GetGame()->UpdateSavePlayer(pAimPlayer, SAVE_DETAIL_ATTRPACKGOODS);


			DWORD dwGoodsId=vOut[i]->GetBasePropertiesIndex();
			CGUID goodsGuid=vOut[i]->GetExID();
			const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
			DWORD dwGoodsNum=vOut[i]->GetAmount();
			BOOL bAdded=pAimPlayer->AddGoodsWithProtect(vOut[i]);
			if(bAdded)
			{
				//AddGoods Log
				GetGameLogInterface()->logT027_goods_log_pickup(pAimPlayer,dwGoodsId,goodsGuid,strGoodsName,5,dwGoodsNum);
			}
		}
	}

	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pAimName, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}
double CScript::DelGoods(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	double lRet=0;
	char* name=GetStringParam(CmdStr,0);
	if(name==NULL)
	{
		return lRet;
	}

	double n=GetIntParam(CmdStr,1);

	if(n==ERROR_CODE)
	{
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		return lRet;
	}
	//�Ƿ��ǰ�̯״̬
	if(pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL)
	{		
		CGUID SessionID = pPlayer->GetPersonalShopSessionID();
		long PlugID = pPlayer->GetPersonalShopPlugID();
		CMessage msg(MSG_S2C_PLAYERSHOP_CLOSE);
		msg.Add( pPlayer->GetExID() );
		msg.Add( SessionID );
		msg.SendToAround(pPlayer);
		CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
		if( pSession && pSession->IsSessionAvailable() )
		{
			CPersonalShopSeller* pSellerPlug = dynamic_cast<CPersonalShopSeller*>( pSession->QueryPlugByID(PlugID) );
			if( pSellerPlug && pSellerPlug->GetOwnerID() == pPlayer->GetExID() )
			{
				pSession->End();
				pPlayer->SetCurrentProgress( CPlayer::PROGRESS_NONE );
				pPlayer->SetMoveable();

				char strFile[64];
				_snprintf(strFile, 64, "scripts/game/close.script");
				stRunScript st;
				st.pszFileName = strFile;
				st.srcShape=dynamic_cast<CPlayer*>(pPlayer);		
				st.pRegion = dynamic_cast<CRegion*>(pPlayer->GetFather());
				st.desShape = NULL;
				st.guUsedItemID = CGUID::GUID_INVALID;
				RunScript(&st, (char*)GetGame()->GetScriptFileData(strFile));
			}
		}
		pPlayer->SetPersonalShopFlag(NULL_GUID, 0);		
	}
	//�Ƿ��ǽ���״̬
	if(pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_TRADING)
	{
			CGUID SessionID = pPlayer->GetTradeSessionId();
			long PlugID = pPlayer->GetTradePlugId();
			CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
			if( pPlayer && pSession )
			{
				CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
					pPlayer -> GetType(), pPlayer -> GetExID() ) );
				if( pPlug && pPlug -> Get32ID() == PlugID )
				{
					if( pSession -> IsSessionAvailable() )
					{
						pSession -> Abort();
					}
				}
			}
	}

	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget( name );
	pPlayer->getOriginPack()->TraversingContainer( &lSeeker );
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		LONG lRemainedGoods = 0;
		if( lRet < n ) lRemainedGoods = n - lRet;
		if( lRemainedGoods == 0 ) break;
		lRet += pPlayer-> DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], lRemainedGoods );
	}

	lSeeker.m_vGoodsID.clear();
	pPlayer -> m_cEquipment.TraversingContainer( &lSeeker );
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		LONG lRemainedGoods = 0;
		if( lRet < n ) lRemainedGoods = n - lRet;
		if( lRemainedGoods == 0 ) break;
		lRet += pPlayer-> DeleteGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lSeeker.m_vGoodsID[i], lRemainedGoods );
	}

	BOOL bEnough=FALSE;
	//CS2CContainerObjectMove::PLAYER_EXTEND_ID
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				LONG lRemainedGoods=0;
				if(lRet<n)
					lRemainedGoods=n-lRet;
				if(lRemainedGoods==0)
				{
					bEnough=TRUE;
					break;
				}
				lRet+=pPlayer->DeleteGoods(static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i),lSeeker.m_vGoodsID[j],lRemainedGoods);
			}
			if(bEnough)
				break;			
		}
	}	
	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}


double CScript::AddGoodsToRgn(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;

	double x = GetIntParam(CmdStr, 0);
	double y = GetIntParam(CmdStr, 1);
	char* pGoodsName = GetStringParam(CmdStr, 2);
	if (pGoodsName==NULL)
		return 0;

	DWORD dwNum = static_cast<DWORD>( GetIntParam(CmdStr, 3) );
	if (dwNum==ERROR_CODE)
		dwNum = 1;

	if (m_ptDropGoods.x != -1 && m_ptDropGoods.y != -1)
	{
		x = m_ptDropGoods.x;
		y = m_ptDropGoods.y;
	}

	vector<CGoods*> vGoods;
	CGoodsFactory::CreateGoods( CGoodsFactory::QueryGoodsIDByOriginalName(pGoodsName), dwNum, vGoods ,37);
	for( size_t i = 0; i < vGoods.size(); i ++ )
	{
		if (vGoods[i])
		{
			double lDropGoodsStartPos = 0;
			long nx, ny;
			lDropGoodsStartPos = ((CServerRegion*)m_pRegion)->GetDropGoodsPos(((long)x), ((long)y), nx, ny, lDropGoodsStartPos);
			if(lDropGoodsStartPos == -1)
			{
				M_FREE( pGoodsName, sizeof(char)*MAX_VAR_LEN );
				for(vector<CGoods*>::size_type idx=i;idx<vGoods.size();idx++)
				{
#ifdef _GOODSLOG1_
					if(vGoods[idx])
					{
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						vGoods[idx]->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:goodsFun273][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							vGoods[idx]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vGoods[idx]->GetBasePropertiesIndex()),srcGoodsGUID,vGoods[idx]->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
					}			
#endif	
					CGoodsFactory::GarbageCollect(&vGoods[idx],204);
				}
				M_FREE( pGoodsName, sizeof(char)*MAX_VAR_LEN );
				return 0;
			}
			

			vGoods[i]->SetPosXY(nx+0.5f, ny+0.5f);
			m_pRegion->AddObject( vGoods[i] );

			//////////////////////////////////////////////////////////////////////////
			// [3/29/2011 chenxianj]
			//����ɾ����ʱ��
			CArea* pArea = dynamic_cast<CServerRegion*>(m_pRegion)->GetArea(nx/AREA_WIDTH, ny/AREA_HEIGHT);
			if(pArea)
			{
				pArea->AddGoodsDelTime(vGoods[i]);
			}
			//////////////////////////////////////////////////////////////////////////

			// ֪ͨÿ�����
			CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,vGoods[i]->GetExID());
			MsgSend.Add( vGoods[i]->GetType() );
			MsgSend.Add( vGoods[i]->GetExID() );
			DBWriteSet setWriteDB;
			MsgSend.GetDBWriteSet(setWriteDB);
			vGoods[i]->SerializeForRgnDisplay(setWriteDB);

			MsgSend.SendToAround(vGoods[i]);
		}
	}

	M_FREE( pGoodsName, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

double CScript::CheckGoods(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	char* name=GetStringParam(CmdStr,0);
	double lRet = 0;	
	if( name )
	{
		CSeekGoodsListener lSeeker;
		lSeeker.SetTarget( name );

		pPlayer->getOriginPack()->TraversingContainer( &lSeeker );
		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( pPlayer->getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
			if( pGoods )
			{
				lRet += pGoods -> GetAmount();
			}
		}

		lSeeker.m_vGoodsID.clear();
		pPlayer-> m_cEquipment.TraversingContainer( &lSeeker );
		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>(pPlayer-> m_cEquipment.Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
			if( pGoods )
			{
				lRet += pGoods -> GetAmount();
			}
		}

		for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
		{
			if(pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
			{			
				lSeeker.m_vGoodsID.clear();
				pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lSeeker);
				for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
				{
					CGoods* pGoods = dynamic_cast<CGoods*>( pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]) );
					lRet+=pGoods->GetAmount();
				}						
			}
		}	

		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	}
	return lRet;
}

double CScript::CheckGoodsInBank(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	char* name=GetStringParam(CmdStr,0);
	double lRet = 0;	
	if( name )
	{
		//! �����ֿ�
		static DWORD arrDepotContainerType[] = 
		{
			eDCT_Primary,
			eDCT_Secondary,
			eDCT_Secondary1,
			eDCT_Secondary2,
			eDCT_Secondary3,
			eDCT_Secondary4
		};

		for (LONG i = 0; i < sizeof(arrDepotContainerType) / sizeof(DWORD); ++i)
		{
			CSeekGoodsListener lSeeker;
			lSeeker.SetTarget( name );

			CGoodsContainer *pContainer = pPlayer->GetPlayerDepot()->FindContainer(arrDepotContainerType[i], true);
			if(NULL != pContainer)
			{
				pContainer->TraversingContainer( &lSeeker );
				for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
				{
					CGoods* pGoods = dynamic_cast<CGoods*>( pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
					if( pGoods )
					{
						lRet += pGoods -> GetAmount();
					}
				}
			}
		}

		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	}
	return lRet;
}


//! ����Ʒ��չ���ֽ⡢�ϳɡ��󶨶��ᣩ�Ի�����
//! �ű��������� OpenGoodsExWindow(1, 20, 500, 300)��
//������1���������ͣ�0=�ֽ⡢1=�ϳɡ�2=�󶨡�3=���ᣩ
//! ����2���Ϸ��Ի���Χ�뾶
//! ����3��4���Ϸ��Ի���Χ��������X��Y
//! ����ֵ��0=ʧ�ܣ�1=�ɹ���
double CScript::OpenGoodsExWindow(const char* cmdStr, char* retStr)
{
	GoodsExManage::tagGoodsExWndSession GoodsExWndSession;
	GoodsExWndSession.uWndType	= GetIntParam(cmdStr, 0);
	GoodsExWndSession.uRadius	= GetIntParam(cmdStr, 1);
	GoodsExWndSession.lPosX		= GetIntParam(cmdStr, 2);
	GoodsExWndSession.lPosY		= GetIntParam(cmdStr, 3);
	//GoodsExWndSession.NpcGuid	= p_SrcShape->GetExID();
	if(NULL != p_DesShape)
		GoodsExWndSession.NpcGuid = p_DesShape->GetExID();

	if(GoodsExManage::GetInstance().OpenGoodsExWindow((CPlayer*)p_SrcShape, GoodsExWndSession))
		return 1.0;
	return 0.0;
}

////! ��Ʒ�󶨻���
////! �ű��������� BindGoods(10001, 1, 1)��
////! ����1��2����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
////! ����3��0=��󶨡�1=�󶨡�����ֵ�Ƿ�
////! ����ֵ��0=ʧ�ܣ�1=�ɹ���

double CScript::BindGoods(const char* cmdStr, char* retStr)
{
	double ret = 0;
	long lContianerID	= GetIntParam(cmdStr, 0); //! ��������
	long lPos			= GetIntParam(cmdStr, 1); //! ��Ʒλ��
	long lOperate		= GetIntParam(cmdStr, 2); //! ������ʽ

	ret = GoodsExManage::GetInstance().SetGoodsBindProperty_Script((CPlayer*)p_SrcShape, lContianerID, lPos, lOperate);

	return ret;
}

//! ��Ʒ�����ⶳ
//! �ű��������� FrostGoods(10001, 1, 1)��
//! ����1��2����Ʒ���ڰ���λ�ã�ֻ������������5���ӱ�����
//! ����3��0=�ⶳ��1=���ᡣ����ֵ�Ƿ�
//! ����ֵ��0=ʧ�ܣ�1=�ɹ���
double CScript::FrostGoods(const char* cmdStr, char* retStr)
{
	double ret = 0;
	long lContianerID	= GetIntParam(cmdStr, 0); //! ��������
	long lPos			= GetIntParam(cmdStr, 1); //! ��Ʒλ��
	long lOperate		= GetIntParam(cmdStr, 2); //! ������ʽ

	ret = GoodsExManage::GetInstance().SetGoodsFrostProperty_Script((CPlayer*)p_SrcShape, lContianerID, lPos, lOperate);

	return ret;
}


double CScript::CheckSpace(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer=NULL;
	pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	char* name=GetStringParam(CmdStr,0);
	double num=GetIntParam(CmdStr,1);
	double lRet = 0;
	if( name )
	{		
		CGoods* pGoods = CGoodsFactory::CreateGoods( CGoodsFactory::QueryGoodsIDByOriginalName(name) ,38);	
		if(!pGoods)
		{
			M_FREE( name, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
		
		pGoods->SetAmount(num);


		CTestContainer testContainer;
		testContainer.Init(pPlayer);
		vector<tagTestResult> vecResult;	
		BOOL bSucced=testContainer.AddTest(pGoods,&vecResult);	
		if(bSucced)
			lRet=1;

#ifdef _GOODSLOG1_
		if(pGoods)
		{
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			pGoods->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:CScript::CheckSpace][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				pGoods->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
		}		
#endif
		CGoodsFactory::GarbageCollect( &pGoods ,51);
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	}
	return lRet;
}
double CScript::GetGoodsNum(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	double lRet=pPlayer ->getOriginPack()->GetGoodsAmount();
	return lRet;
}
double CScript::GetGoodsFree(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	double lRet=pPlayer->GetTotalEmptySpaceNum();

	return lRet;
}

//! ���ָ��װ����λ��װ�����ID
//! ����1��@PlayerGuid
//! ����2��$pos
//! ����ֵ��0=�����û��װ��������ֵ=װ�����ID
double CScript::GetEquipIDByPos(const char* cmd, char* retStr)
{
	CPlayer* pAimPlayer = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
			pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		else
		{
			CGUID guid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			pAimPlayer = GetGame()->FindPlayer(guid);
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL == pAimPlayer)
		return 0;

	int nPos=(int)GetIntParam(cmd, 1);	
	CGoods* pGoods = pAimPlayer->m_cEquipment.GetGoods(nPos);
	if(!pGoods)
		return 0;
	return pGoods->GetBasePropertiesIndex();    
}

//! ���ָ��װ����λ��װ��ԭʼ��
//! ����1��@PlayerGuid
//! ����2��$pos
//! ����3��#RetName
//! ����ֵ��0=�����û��װ����1=ִ�гɹ�
double CScript::GetEquipNameByPos(const char* cmd, char* retStr)
{
	CPlayer* pAimPlayer = NULL;
	const char* szGuid = GetStringParam(cmd, 0);
	if (NULL != szGuid)
	{
		if (0 == strlen(szGuid))
			pAimPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
		else
		{
			CGUID guid = GetScriptGuidValue((CMoveShape*)GetSrcShape(), szGuid);
			pAimPlayer = GetGame()->FindPlayer(guid);
		}
		M_FREE( szGuid, sizeof(char)*MAX_VAR_LEN );
	}

	if(NULL == pAimPlayer)
		return 0;

	int nPos = (int)GetIntParam(cmd,1);

	CGoods* pGoods = pAimPlayer->m_cEquipment.GetGoods(nPos);
	if(!pGoods)
		return 0;

	char* szRetName = GetStringParam(cmd, 2);
	if(szRetName)
	{
		const char *pOriginalName = CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
		if (NULL != pOriginalName)
		{
			SetScriptVarValue(dynamic_cast<CMoveShape*>(pAimPlayer), szRetName, pOriginalName);
			M_FREE( szRetName, sizeof(char)*MAX_VAR_LEN );
			return 1;
		}

		M_FREE( szRetName, sizeof(char)*MAX_VAR_LEN );
	}

	return 0;
}

double CScript::GetPlayerGoodsAddonProperty(const char* CmdStr, char* retStr)
{

	char* pPlayerName =GetStringParam(CmdStr,0);
	if(!pPlayerName)
		return 0;

	CPlayer* pPlayer=GetGame()->FindPlayer(pPlayerName);//dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
	{
		if(!p_SrcShape)
		{
			M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}			
		pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	}
	int nContainerId=(int)GetIntParam(CmdStr,1);
	int nPos=(int)GetIntParam(CmdStr,2);
	int nAddonId=(int)GetIntParam(CmdStr,3);	
	int nValId=(int)GetIntParam(CmdStr,4);	
	CGoods* pGoods=pPlayer->FindGoods(nContainerId,nPos);
	if(!pGoods)
	{
		M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
		
	LONG lVal=pGoods->GetAddonPropertyValues(static_cast<GOODS_ADDON_PROPERTIES>(nAddonId),nValId);
	M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
	return lVal;
}



double CScript::SetPlayerGoodsAddonProperty(const char* CmdStr, char* retStr)
{

	char* pPlayerName =GetStringParam(CmdStr,0);
	if(!pPlayerName)
		return 0;

	CPlayer* pPlayer=GetGame()->FindPlayer(pPlayerName);//dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
	{
		if(!p_SrcShape)
		{
			M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}			
		pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	}
	int nContainerId=(int)GetIntParam(CmdStr,1);
	int nPos=(int)GetIntParam(CmdStr,2);
	int nAddonId=(int)GetIntParam(CmdStr,3);	
	int nValId=(int)GetIntParam(CmdStr,4);	
	int nVal=(int)GetIntParam(CmdStr,5);
	CGoods* pGoods=pPlayer->FindGoods(nContainerId,nPos);
	if(!pGoods)
	{
		M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	//LONG lVal=pGoods->GetAddonPropertyValues(static_cast<GOODS_ADDON_PROPERTIES>(nAddonId),nValId);
	pGoods->SetAddonPropertyBaseValues(static_cast<GOODS_ADDON_PROPERTIES>(nAddonId),nValId,nVal);
	M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}

double CScript::IsEquipEnchased(const char* CmdStr, char* retStr)
{	
	char* pPlayerName =GetStringParam(CmdStr,0);
	if(!pPlayerName)
		return 0;
	CPlayer* pPlayer=GetGame()->FindPlayer(pPlayerName);//dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
	{
		if(!p_SrcShape)
		{
			M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
			return 0;
		}
			
		pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	}
	int nPos=(int)GetIntParam(CmdStr,1);
	CGoods* pGoods=pPlayer->FindGoods(2,nPos);
	if(!pGoods)
	{
		M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
		
	DWORD dwCardNum=pGoods->GetEnchasedCardNum();
	M_FREE( pPlayerName, sizeof(char)*MAX_VAR_LEN );
	return dwCardNum;
}


double CScript::OpenGoodsEnchase(const char* CmdStr, char* retStr)
{	
	CPlayer *pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL) return 0;

	if( pPlayer -> IsDied() ) 
		return (long)false;
	int nWindowType=GetIntParam(CmdStr,0);
	
	if(TYPE_BACKOUT == nWindowType)
	{
		if(CPlayerPwd::ePS_Free != pPlayer->m_CardPwd.GetState())
		{
			CMessage msg(MSG_S2C_ENCHASE_HAS_PWD);
			msg.Add((DWORD)((CPlayerPwd::ePS_Frost == pPlayer->m_CardPwd.GetState()) ? 1 : 0));
			msg.SendToPlayer(pPlayer->GetExID());
			return 0.0f;
		}
	}

	const CGUID& SessionID		= CSessionFactory::CreateSession( 1, 1 );
	CSession* pSession	= const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
	if( pSession && pSession -> Start() )
	{
		LONG lPlugID = CSessionFactory::CreatePlug( CSessionFactory::PT_EQUIPMENT_ENCHASE, TYPE_PLAYER, (pPlayer->GetExID() ));
		if( CSessionFactory::InsertPlug(SessionID, lPlugID) )
		{
			//pPlayer -> SetCurrentProgress( CPlayer::eProgress::PROGRESS_OPEN_STALL );

			CMessage msg( MSG_S2C_OPENGOODSENCHASE );
			msg.Add((BYTE)nWindowType);
			msg.Add( SessionID );
			msg.Add( lPlugID );

			msg.SendToPlayer(pPlayer->GetExID());

			return 1;
		}
	}	
	return 0;
}

double CScript::DelPlayerGoods(const char* CmdStr, char* retStr)
{
	double lRet=0;
	char* strPlayerName=GetStringParam(CmdStr,0);
	char* strGoodsName=GetStringParam(CmdStr,1);
	double lNum=GetIntParam(CmdStr,2);
	if(strPlayerName&& strGoodsName && lNum!=ERROR_CODE)
	{
		CPlayer* pPlayer=GetGame()->FindPlayer(strPlayerName);
		if(pPlayer)
		{
			BOOL bEnough=FALSE;

			CSeekGoodsListener lSeeker;
			lSeeker.SetTarget( strGoodsName );
			pPlayer->getOriginPack()->TraversingContainer( &lSeeker );
			for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
			{
				LONG lRemainedGoods = 0;
				if( lRet < lNum ) lRemainedGoods = lNum - lRet;
				if( lRemainedGoods == 0 ) 
				{
					bEnough = TRUE;
					break;
				}
				lRet += pPlayer-> DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], lRemainedGoods );
			}

			if(!bEnough)
			{
				lSeeker.m_vGoodsID.clear();
				pPlayer -> m_cEquipment.TraversingContainer( &lSeeker );
				for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
				{
					LONG lRemainedGoods = 0;
					if( lRet < lNum ) lRemainedGoods = lNum - lRet;
					if( lRemainedGoods == 0 )
					if( lRemainedGoods == 0 ) 
					{
						bEnough = TRUE;
						break;
					}
					lRet += pPlayer-> DeleteGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lSeeker.m_vGoodsID[i], lRemainedGoods );
				}
			}
			
			if(!bEnough)
			{
				//CS2CContainerObjectMove::PLAYER_EXTEND_ID
				for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
				{
					if(pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
					{			
						lSeeker.m_vGoodsID.clear();
						pPlayer->getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lSeeker);
						for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
						{
							LONG lRemainedGoods=0;
							if(lRet<lNum)
								lRemainedGoods=lNum-lRet;
							if(lRemainedGoods==0)
							{
								bEnough=TRUE;
								break;
							}
							lRet+=pPlayer->DeleteGoods(static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i),lSeeker.m_vGoodsID[j],lRemainedGoods);
						}
						if(bEnough)
							break;			
					}
				}
			}
			
			if(!bEnough)
			{
				//! �����ֿ�
				static DWORD arrDepotContainerType[] = 
				{
					eDCT_Primary,
					eDCT_Secondary,
					eDCT_Secondary1,
					eDCT_Secondary2,
					eDCT_Secondary3,
					eDCT_Secondary4
				};

				for (LONG i = 0; i < sizeof(arrDepotContainerType) / sizeof(DWORD); ++i)
				{
					CGoodsContainer *pContainer = pPlayer->GetPlayerDepot()->FindContainer(arrDepotContainerType[i], true);
					if(NULL != pContainer)
					{
						lSeeker.m_vGoodsID.clear();
						pContainer->TraversingContainer(&lSeeker);
						for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
						{
							LONG lRemainedGoods=0;
							if(lRet<lNum)
								lRemainedGoods=lNum-lRet;
							if(lRemainedGoods==0)
							{
								bEnough=TRUE;
								break;
							}
							lRet+=pPlayer->DeleteDepotGoods(arrDepotContainerType[i],lSeeker.m_vGoodsID[j],lRemainedGoods);
						}
						if(bEnough)
							break;
					}
				}
			}
		}
	}
	M_FREE( strPlayerName, sizeof(char)*MAX_VAR_LEN );
	M_FREE( strGoodsName, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}


double CScript::DelUsedGoods(const char* CmdStr, char* retStr)		//�������ű�ʹ�õ���Ʒ	DelUsedGoods($����);
{
	
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	double lRet=0;
	double n=GetIntParam(CmdStr,0);
	if(n==ERROR_CODE)
	{
		return lRet;
	}

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return 0;

	CGoods *pGoods = pPlayer->GetGoodsById_FromPackage(GetUsedItemID());
	DWORD dwContainerId=0,dwPos=0;
	BOOL bFound=pPlayer->FindGoodsByGuid(GetUsedItemID(),dwContainerId,dwPos);
	if(!bFound)
	{
		return 0;
	}

	lRet = pPlayer -> DeleteGoods(static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(dwContainerId), GetUsedItemID(), n);
	return lRet;
}
double CScript::CheckUsedGoods(const char* CmdStr, char* retStr)		//��鴥���ű�ʹ����Ʒ	CheckUsedGoods(); ������Ʒ����
{
	
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return 0;	
	CGoods *pGoods =pPlayer->GetGoodsById_FromPackage(GetUsedItemID());
	if (pGoods)
	{
		return pGoods->GetAmount();
	}

	return 0;
}
double CScript::GetUsedGoodsPropertiy1(const char* CmdStr, char* retStr)	//��ʹ�õ���Ʒ���Ծ���ֵ1 ���û�и������ԣ�����-1
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return -1;

	GOODS_ADDON_PROPERTIES gap = 
		static_cast<GOODS_ADDON_PROPERTIES>( (long)GetIntParam(CmdStr, 0) );

	if(gap==ERROR_CODE)
	{
		return -1;
	}

	CGoods *pGoods = pPlayer->GetGoodsById_FromPackage(GetUsedItemID());
	if (pGoods)
	{
		double n = pGoods -> GetAddonPropertyValues( gap, 1 );

		// �ж��Ƿ��и�����
		if (n==0)
		{
			bool bHas = pGoods->HasAddonPropertyValues(gap);
			if (bHas)
				return 0;
			else
				return -1;
		}
		else
			return n;
	}
	return 0;
}

double CScript::GetUsedGoodsPropertiy2(const char* CmdStr, char* retStr)	//��ʹ�õ���Ʒ���Ծ���ֵ2 ���û�и������ԣ�����-1
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return -1;

	GOODS_ADDON_PROPERTIES gap = 
		static_cast<GOODS_ADDON_PROPERTIES>( (long)GetIntParam(CmdStr, 0) );

	if(gap==ERROR_CODE)
	{
		return -1;
	}

	CGoods *pGoods =pPlayer->GetGoodsById_FromPackage(GetUsedItemID());
	if (pGoods)
	{
		double n = pGoods -> GetAddonPropertyValues( gap, 2 );

		// �ж��Ƿ��и�����
		if (n==0)
		{
			bool bHas = pGoods->HasAddonPropertyValues(gap);
			if (bHas)
				return 0;
			else
				return -1;
		}
		else
			return n;
	}
	return 0;
}
double CScript::SetUsedGoodsPropertiy1(const char* CmdStr, char* retStr)	//����ʹ�õ���Ʒ���Ծ���ֵ1 ���û�и������ԣ�Ҳ�������
{
	if(p_SrcShape == NULL) return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return -1;

	GOODS_ADDON_PROPERTIES gap = 
		static_cast<GOODS_ADDON_PROPERTIES>( (long)GetIntParam(CmdStr, 0) );
	if(gap==ERROR_CODE)
		return -1;

	double lVal = GetIntParam(CmdStr, 1);
	if (lVal == ERROR_CODE)
		return -1;

	CGoods *pGoods = ((CPlayer*)p_SrcShape)->GetGoodsById_FromPackage(GetUsedItemID());
	if (pGoods)
	{
		LONG lRet =1;// pGoods->SetAddonPropertyModifier( gap, 1, lVal );
		if( lRet )
		{
			// ���¿ͻ�����Ʒ����
			vector<BYTE> vGoods;
			pGoods -> SerializeForOldClient( &vGoods );
			CMessage Msg( MSG_S2C_GOODS_UPDATEPROPERTY );
			Msg.Add( ((CPlayer*)p_SrcShape)->GetExID() );
			Msg.Add( pGoods -> GetExID() );
			Msg.Add( static_cast<LONG>( vGoods.size() ) );
			Msg.AddEx( &vGoods[0], static_cast<LONG>( vGoods.size() ) );
			Msg.SendToPlayer( ((CPlayer*)p_SrcShape)->GetExID() );
			return lRet;
		}
	}
	return 0;
}
double CScript::SetUsedGoodsPropertiy2(const char* CmdStr, char* retStr)	//����ʹ�õ���Ʒ���Ծ���ֵ2 ���û�и������ԣ�Ҳ�������
{
	if(p_SrcShape == NULL) return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return -1;

	GOODS_ADDON_PROPERTIES gap = 
		static_cast<GOODS_ADDON_PROPERTIES>( (long)GetIntParam(CmdStr, 0) );
	if(gap==ERROR_CODE)
		return -1;

	double lVal = GetIntParam(CmdStr, 1);
	if (lVal == ERROR_CODE)
		return -1;

	CGoods *pGoods = ((CPlayer*)p_SrcShape)->GetGoodsById_FromPackage(GetUsedItemID());
	if (pGoods)
	{
		LONG lRet =1;// pGoods->SetAddonPropertyModifier( gap, 2, lVal );
		if( lRet )
		{
			// ���¿ͻ�����Ʒ����
			vector<BYTE> vGoods;
			pGoods -> SerializeForOldClient( &vGoods );
			CMessage Msg( MSG_S2C_GOODS_UPDATEPROPERTY );
			Msg.Add( ((CPlayer*)p_SrcShape)->GetExID() );
			Msg.Add( pGoods -> GetExID() );
			Msg.Add( static_cast<LONG>( vGoods.size() ) );
			Msg.AddEx( &vGoods[0], static_cast<LONG>( vGoods.size() ) );
			Msg.SendToPlayer( ((CPlayer*)p_SrcShape)->GetExID() );
			return lRet;
		}
	}
	return 0;
}

double CScript::BuyGoods(const char* CmdStr, char* retStr)			//������Ʒ		
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;
	DWORD dwAreaId=0;
	CServerRegion* pRegion=dynamic_cast<CServerRegion*>(pPlayer->GetFather());
	if(pRegion)
		dwAreaId=pRegion->GetAreaId();

	char *strNpcName=GetStringParam(CmdStr, 0);
	if (strNpcName)
	{
		CTradeList::tagTrade *pTrade = CTradeList::GetTradeList(strNpcName);
		if (pTrade)
		{
			pPlayer->ProduceEvent(ET_EventBreak);
			CMessage msg(MSG_S2C_SHOP_OPEN);
			if(p_DesShape)
			{
				msg.Add(p_DesShape->GetExID());
			}
			else
			{
				CGUID emptyGuid;
				msg.Add(emptyGuid);
			}

			msg.Add(long(pTrade->shopType));//�̵�����

			msg.Add(BYTE(pTrade->tradeType));//���ͣ���Ǯ���� OR ���ﻻ��
			msg.Add(BYTE(pTrade->_creditType));//������������֯�������� by tanglei 09-9-15
			msg.Add(CGlobeSetup::GetSetup() -> fBasePriceRate);//FLOAT
			msg.Add(CGlobeSetup::GetSetup() -> fTradeInRate);//FLOAT	

			long lNum=pTrade->vecGoods.size();			
			msg.Add(lNum);//��Ʒ����
			if(pTrade->tradeType==CTradeList::ETT_MONEY || pTrade->tradeType==CTradeList::ETT_SILVER)
			{
				for(int i=0;i<pTrade->vecGoods.size();i++)
				{					
					CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties(
						pTrade->vecGoods[i].dwGoodsId);
					if(!pProperties)
					{
						M_FREE( strNpcName, sizeof(char)*MAX_VAR_LEN );
						return 0;	
					}
					msg.Add(pTrade->vecGoods[i].dwGoodsId);//���ƷID		
					msg.Add(pTrade->vecGoods[i].dwGoodsNum);//���Ʒ����
					msg.Add(pTrade->vecGoods[i].dwCreditId);//��������ID
					msg.Add(pTrade->vecGoods[i].dwCreditVal);//��������ֵ
				}
			}
			else if(pTrade->tradeType==CTradeList::ETT_GOODS)
			{
				//struct tagTradeData
				//{
				//	DWORD dwGoodsId;	//��ƷID
				//	DWORD dwGoodsNum;	//��Ʒ����
				//	DWORD dwCreditId;		//��������ID
				//	DWORD dwCreditVal;		//��������ֵ
				//	vector<tagTradeGoods>_vecTreadGoods;//��������������Ʒ�б�	
				//};
				for(int i=0;i<pTrade->vecGoods.size();i++)
				{
					CTradeList::tagTradeData& tradeData=pTrade->vecGoods[i];
					msg.Add(tradeData.dwGoodsId);
					msg.Add(tradeData.dwGoodsNum);
					msg.Add(tradeData.dwCreditId);
					msg.Add(tradeData.dwCreditVal);
					msg.Add((long)tradeData._vecTreadGoods.size());
					for (long i=0;i<(long)tradeData._vecTreadGoods.size();++i)
					{	
						CTradeList::tagTradeGoods& tradeGoods=tradeData._vecTreadGoods[i];
						msg.Add(tradeGoods.dwGoodsChgId);
						msg.Add(tradeGoods.dwChgAmount);
					}
				}
			}
			msg.SendToPlayer(pPlayer->GetExID());
		}
		else 
		{
			pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_NONE );
			pPlayer -> SetMoveable();
		}
		M_FREE( strNpcName, sizeof(char)*MAX_VAR_LEN );
	}
	return 1;
}

double CScript::GetGoodsAddonProperty(const char* CmdStr,char* retStr/* =NULL */)
{
	int nGoodsId=(int)GetIntParam(CmdStr,0);
	int nPropertyId=(int)GetIntParam(CmdStr,1);
	int nValueId=(int)GetIntParam(CmdStr,2);
	CGoodsBaseProperties* pProperty=CGoodsFactory::QueryGoodsBaseProperties(nGoodsId);
	if(pProperty)
	{
		long lVal=pProperty->GetAddonPropertyValue(static_cast<GOODS_ADDON_PROPERTIES>(nPropertyId),nValueId);
		return lVal;
	}
	return 0;
}

double CScript::FullRefine(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;



	((CPlayer*)p_SrcShape) ->AllEquipmentRefineUpdate(100000);
	return 1;
}

double CScript::GetWeaponStr(const char* CmdStr, char* retStr)
{
	long ret=0;
	DWORD dwSrc = GetIntParam(CmdStr,0);
	if (dwSrc!=ERROR_CODE && dwSrc!=INVILID_VALUE )
	{
		CMoveShape *pSrc = (CMoveShape*)(dwSrc);
		if (pSrc)
		{			
			CGoods* pEquip = pSrc->FindGoods(CGoodsBaseProperties::GT_EQUIPMENT,CEquipmentContainer::EC_WEAPON);
			if (pEquip)
			{
				ret = pEquip->GetAddonPropertyValues(GAP_WEAPON_DAMAGE_LEVEL,1);				
			}
		}

	}
	return ret;
}

double CScript::AddMedal(const char* cmd, char* retStr)
{
	char* strGoodsBaseName = GetStringParam(cmd,0);
	if(NULL == strGoodsBaseName)
		return 0;
	LONG lMedalIndex = CGoodsFactory::QueryGoodsIDByOriginalName(strGoodsBaseName);

	double re = 0;
	CPlayer *pPlayer = ((CPlayer*)p_SrcShape);
	if (NULL != pPlayer && lMedalIndex != INVILID_VALUE)
	{
		if(eMCRV_Succeed == pPlayer->m_MedalContainer.AddMedal(lMedalIndex))
		{
			CMessage msg(MSG_S2C_MEDAL_ADD_NOTIFY);
			msg.Add(lMedalIndex);
			msg.SendToPlayer(pPlayer->GetExID());
			re = 1.0;
		}
	}

	M_FREE(strGoodsBaseName, sizeof(char)*MAX_VAR_LEN);
	return re;
}
double CScript::DelMedal(const char* cmd, char* retStr)
{
	char* strGoodsBaseName = GetStringParam(cmd,0);
	if(NULL == strGoodsBaseName)
		return 0;
	LONG lMedalIndex = CGoodsFactory::QueryGoodsIDByOriginalName(strGoodsBaseName);

	double re = 0;
	CPlayer *pPlayer = ((CPlayer*)p_SrcShape);
	if (NULL != pPlayer && lMedalIndex != INVILID_VALUE)
	{
		if(eMCRV_Succeed == pPlayer->m_MedalContainer.DelMedal(lMedalIndex))
		{
			CMessage msg(MSG_S2C_MEDAL_DEL_NOTIFY);
			msg.Add(lMedalIndex);
			msg.SendToPlayer(pPlayer->GetExID());
			re = 1.0;
		}
	}

	M_FREE(strGoodsBaseName, sizeof(char)*MAX_VAR_LEN);
	return re;
}
double CScript::GetMedal(const char* cmd, char* retStr)
{
	char* strGoodsBaseName = GetStringParam(cmd,0);
	if(NULL == strGoodsBaseName)
		return 0;
	LONG lMedalIndex = CGoodsFactory::QueryGoodsIDByOriginalName(strGoodsBaseName);

	double re = 0;
	CPlayer *pPlayer = ((CPlayer*)p_SrcShape);
	if (NULL != pPlayer && lMedalIndex != INVILID_VALUE)
	{
		M_FREE( strGoodsBaseName, sizeof(char)*MAX_VAR_LEN );
		return pPlayer->m_MedalContainer.HasMedal(lMedalIndex);
	}

	M_FREE( strGoodsBaseName, sizeof(char)*MAX_VAR_LEN );
	return 0;
}
double CScript::GetMedalNum(const char* cmd, char* retStr)
{
	CPlayer *pPlayer = ((CPlayer*)p_SrcShape);
	if (NULL != pPlayer)
	{
		return pPlayer->m_MedalContainer.GetMedalNum();
	}

	return 0;
}

double CScript::GetShowMedalName(const char* cmd, char* retStr)
{
	char* szVarName = GetStringParam(cmd, 0);
	if (szVarName== NULL) return -1;

	CPlayer* player= dynamic_cast<CPlayer*>(p_SrcShape);
	if (player==NULL)	
	{
		M_FREE( szVarName, sizeof(char)*MAX_VAR_LEN );
		return -1;
	}

	const char *pMedalName = player->GetShowMedalBaseName();
	if(NULL == pMedalName)
	{
		M_FREE( szVarName, sizeof(char)*MAX_VAR_LEN );
		return -1;
	}

	int ret = SetScriptVarValue((CMoveShape*)p_SrcShape, szVarName, pMedalName);
	M_FREE( szVarName, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

double CScript::GetGoodsDisplayName(const char* CmdStr, char* retStr)
{
	CHAR* pResult = GetStringParam(CmdStr, 0);
	if(pResult == NULL) return 0;

	int ret = 0;
	CPlayer* player = dynamic_cast<CPlayer*>(p_SrcShape);
	if(player == NULL)
	{
		M_FREE( pResult, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	// ���ر�����
	CHAR* szRetStrName = GetStringParam(CmdStr, 1);
	if(szRetStrName == NULL)
	{
		M_FREE( pResult, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}

	const CHAR* strDisName=CGoodsFactory::QueryGoodsName(pResult);
	if(strDisName)
	{
		ret = SetScriptVarValue((CPlayer*)p_SrcShape, szRetStrName, strDisName);
	}
	M_FREE( pResult, sizeof(char)*MAX_VAR_LEN );
	M_FREE( szRetStrName, sizeof(char)*MAX_VAR_LEN );
	return ret;
}

//! ��ʼһ���Ǹ�ְҵ���������䷽���͸���ң�
//! ����1
double CScript::BeginMake(const char* cmd, char* retStr)
{
	LONG lID		= GetIntParam(cmd, 0);
	LONG lType		= GetIntParam(cmd, 1);
	LONG lParam3	= GetIntParam(cmd, 2);

	CPlayer* player = ((CPlayer*)p_SrcShape);
	if(player == NULL)
		return 0;

	switch(lType)
	{
	case eMT_FORM_GOODS:
		{
			if(NULL_GUID == m_guUsedItemID)
				return 0;
			//! �������Ʒ��������lParam3������Ϊ�Ƿ�ɾ������Ʒ��ɾ������Ϊ1
			GoodsTailorSystem::GetInstance().PlayerMakeBeginByGoods(player->GetExID(), m_guUsedItemID, lID, lParam3);
			return 1;
		}
		break;
	case eMT_FROM_NPC:
		{
			//! �����NPC��������lParam3������Ϊ������������NPC��Զ����
			GoodsTailorSystem::GetInstance().PlayerMakeBeginByNpc(player->GetExID(), player->GetRegionGUID(), lParam3, lID);
			return 1;
		}
	    break;
	}

	return 0;
}
//! ���һ���Ǹ�ְҵ���������䷽�ϳ���Ʒ��
double CScript::FinishMake(const char* cmd, char* retStr)
{
	CPlayer* player = ((CPlayer*)p_SrcShape);
	if(player == NULL)
		return 0;
	GoodsTailorSystem::GetInstance().PlayerMakeFinish(player->GetExID());
	return 1;
}

double CScript::BackToCity(const char* cmd, char* retStr)		//�س�
{
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;
	CServerRegion* pRegion=dynamic_cast<CServerRegion*>(pPlayer->GetFather());
	if(!pRegion)
		return 0;
	//��̯�ͽ���ʱ��Ч
	if(pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| pPlayer->GetCurrentProgress()==CPlayer::PROGRESS_TRADING)
		return 0;
	//�ó����Ƿ�����ʹ�ûسǷ�
	if(!pRegion->GetSetup().bRuneStoneIsUsed)
	{
		char* strMsg=CStringReading::LoadString(20,42);
		if(strMsg)
			pPlayer->SendSystemInfo(strMsg);
		return 0;
	}

	//[9/7/2009 chenxianj]	
	if (pPlayer->GetPkValue()>=CGlobeSetup::GetSetup()->lPrisonIslandPkValue)					
	{
		//pkֵ����18000�󣬷ŵ����ŵ�
		pPlayer->ChangeRgnToIsland();
		return 1;
	}

	//�ж����PKֵ
	if(pPlayer->GetPkValue()>20)
	{
		static long con_rgn[7]={0,11007,12007,13007,14007,15007,16007};
		BYTE nCountry=pPlayer->GetCountry();
		if(nCountry>=1 && nCountry<=6)
		{
			long rgnId=con_rgn[nCountry];
			CGUID rgnGUID=NULL_GUID;
			CServerRegion* pReturnRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, rgnId);
			long lRanX=360+::random(10);
			long lRanY=318+::random(10);
			
			if(pReturnRegion)
			{
				rgnGUID=pReturnRegion->GetExID();																
			}
			pPlayer->ChangeRegion(RGN_NORMAL,rgnGUID,lRanX,lRanY,pPlayer->GetDir(),rgnId);
			return 1;
		}
		return 0;
	}
	else
	{
		long region = 0;
		long l = 0;
		long t = 0;
		long r = 0;
		long b = 0;
		long d = -1;
		CGUID rgnGuid=NULL_GUID;

		//�������Һ���ҹ����Ƿ�һ��
		if(pRegion->GetCountry()==pPlayer->GetCountry() || pRegion->GetCountry()==7)
		{
			//��.rs���õĳ���
			if(pRegion->GetSetup().lRuneStoneRegionID == 0) // ԭ��
			{
				return 0;
			}
			else if(pRegion->GetSetup().lRuneStoneRegionID == -1) // �ع���
			{
				BYTE byCountry = pPlayer->GetCountry();
				region = GetCountryParam()->GetCountryMainRegionID(byCountry);
				l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
				t = GetCountryParam()->GetCountryMainRectTop(byCountry);
				r = GetCountryParam()->GetCountryMainRectRight(byCountry);
				b = GetCountryParam()->GetCountryMainRectBottom(byCountry);
			}
			else if(pRegion->GetSetup().lRuneStoneRegionID>0) // > 0 �л�����ID����
			{			
				if(pRegion->GetSetup().bRuneStoneIsUsed)
				{
					region=pRegion->GetSetup().lRuneStoneRegionID;
					l=pRegion->GetSetup().lRuneStonePosX1;
					t=pRegion->GetSetup().lRuneStonePosY1;
				}
			}
		}
		else
		{
			//��countryparam.ini
			BYTE byCountry = pPlayer->GetCountry();
			region = GetCountryParam()->GetCountryMainRegionID(byCountry);
			l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
			t = GetCountryParam()->GetCountryMainRectTop(byCountry);
			r = GetCountryParam()->GetCountryMainRectRight(byCountry);
			b = GetCountryParam()->GetCountryMainRectBottom(byCountry);
		}

		CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, region);
		if(pRgn)
			rgnGuid = pRgn->GetExID();
		pPlayer->ChangeRegion(RGN_NORMAL, rgnGuid, l, t, pPlayer->GetDir(), region);
		return 1;
	}
	return 0;
}

//! ��ʼһ��ͨ����Ʒ��������
//! ����0����ɲ����Ľű�
//! ����1����Ӧ��Ʒ����Ľű�
//! ����2����������ID
//! ����3��������ťID
//! ����4���ı�˵��
//! ����5���رվ���
//! ����6��������������Ʒ����1~4��
//! ����ֵ��1=�ɹ���0=ʧ��
double CScript::BeginContainerGoodsOpt(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pOnFinishScript = GetStringParam(cmd,0);
	if(NULL == pOnFinishScript)
		return 0.0f;

	LPCSTR pOnPutScript = GetStringParam(cmd,1);
	if(NULL == pOnPutScript)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	DWORD dwTitleID = GetIntParam(cmd, 2);
	if (ERROR_CODE == dwTitleID)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}
	DWORD dwButtonID = GetIntParam(cmd, 3);
	if (ERROR_CODE == dwButtonID)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	LPCSTR pTxt = GetStringParam(cmd,4);
	if(NULL == pTxt)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	LONG lRadius = GetIntParam(cmd, 5);
	if (ERROR_CODE == lRadius)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pTxt, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	DWORD dwMaxPushNum = GetIntParam(cmd, 6);
	if (ERROR_CODE == dwMaxPushNum)
	{
		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pTxt, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	if(!pPlayer->GetGoodsAllPurposeOpt().BeginOpt(
		pPlayer->GetExID(), dwTitleID, dwButtonID, 
		pTxt, pOnPutScript, pOnFinishScript, lRadius, 
		pPlayer->GetPosX(), pPlayer->GetPosY(), pPlayer->GetRegionGUID(), dwMaxPushNum
		)
	)
	{
		pPlayer->GetGoodsAllPurposeOpt().Release();

		M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
		M_FREE( pTxt, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	M_FREE( pOnFinishScript, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pOnPutScript, sizeof(char)*MAX_VAR_LEN );
	M_FREE( pTxt, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

//! �õ�������Ʒ
//! ����0��������ƷGUID�ı���
//! ����1��ָ�������ڼ�����λ����Ʒ��1~4��
//! ����ֵ��0=û�в�����Ʒ ����ֵ=������Ʒ�ĵ�����
double CScript::GetOptGoodsInContainer(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	DWORD dwGoodsIdx = GetIntParam(cmd, 1);
	if (ERROR_CODE == dwGoodsIdx)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	const CGUID &GoodsGuid = pPlayer->GetGoodsAllPurposeOpt().GetOptGoodsGuid(dwGoodsIdx - 1);
	if(NULL_GUID == GoodsGuid)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}	

	SetScriptGuidValue(pPlayer, pGuidName, GoodsGuid);

	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
	return pGoods->GetAmount();
}

//! ���������Ʒ��������
//! ����0����ƷGUID
//! ����1��ָ�������ڼ�����λ����Ʒ��1~4��
//! ����ֵ��1=�ɹ���0=ʧ��
double CScript::PutOptGoodsInContainer(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	DWORD dwGoodsIdx = GetIntParam(cmd, 1);
	if (ERROR_CODE == dwGoodsIdx)
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0f;
	}

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	GoodsAllPurposeOpt::eOptResult OptResult = pPlayer->GetGoodsAllPurposeOpt().OnGoodsPut(GoodsGuid, dwGoodsIdx - 1);
	if (GoodsAllPurposeOpt::eOR_Succeed == OptResult)
		return 1.0f;
	if (GoodsAllPurposeOpt::eOR_Lawless == OptResult)
		pPlayer->GetGoodsAllPurposeOpt().Release();

	return 0.0f;
}

//! ��������еĲ�����Ʒ
//! ����0��ָ�������ڼ�����λ����Ʒ��1~4�� 0=ȫ�������
//! ����ֵ��1=�ɹ���0=ʧ��
double CScript::RemoveOptGoodsInContainer(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	DWORD dwGoodsIdx = GetIntParam(cmd, 0);
	if (ERROR_CODE == dwGoodsIdx)
		return 0.0f;

	if(0 == dwGoodsIdx)
		dwGoodsIdx = 0xFFFFFFFF;
	else
		dwGoodsIdx -= 1;
	GoodsAllPurposeOpt::eOptResult OptResult = pPlayer->GetGoodsAllPurposeOpt().OnGoodsPop(dwGoodsIdx);
	if (GoodsAllPurposeOpt::eOR_Lawless == OptResult)
	{
		pPlayer->GetGoodsAllPurposeOpt().Release();
		return 0.0f;
	}

	return 1.0f;
}

//! ˢ�������ı�
//! ����0����������
//! ����ֵ��1=�ɹ���0=ʧ�ܣ������ǻỰ�Ѿ�������Χ��
double CScript::RefreshContainerTxt(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pTxt= GetStringParam(cmd,0);
	if (NULL == pTxt)
		return 0.0f;

	GoodsAllPurposeOpt::eOptResult OptResult = pPlayer->GetGoodsAllPurposeOpt().SetTxt(pTxt);
	M_FREE( pTxt, sizeof(char)*MAX_VAR_LEN );
	if (GoodsAllPurposeOpt::eOR_Lawless == OptResult)
	{
		pPlayer->GetGoodsAllPurposeOpt().Release();
		return 0.0f;
	}

	return 1.0f;
}

//! ˢ��������ʾ��Ǯ
//! ����0����������
//! ����ֵ��1=�ɹ���0=ʧ�ܣ������ǻỰ�Ѿ�������Χ��
double CScript::RefreshContainerGold(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	DWORD dwGoldNum = GetIntParam(cmd, 0);
	if (ERROR_CODE == dwGoldNum)
		return 0.0f;

	GoodsAllPurposeOpt::eOptResult OptResult = pPlayer->GetGoodsAllPurposeOpt().SetGold(dwGoldNum);
	if (GoodsAllPurposeOpt::eOR_Lawless == OptResult)
	{
		pPlayer->GetGoodsAllPurposeOpt().Release();
		return 0.0f;
	}

	return 1.0f;
}

//! �ر�һ��ͨ����Ʒ��������
//! ��������
//! ����ֵ��������
double CScript::EndContainerGoodsOpt(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer != NULL)
		pPlayer->GetGoodsAllPurposeOpt().Release();
	
	return 0.0f;
}

//! ����GUID �õ�ָ����Ʒ��������ϣ���ԭʼ��
//! ����0����ƷGUID
//! ����1��������Ʒԭʼ���ı���
//! ����ֵ��1=�ҵ�����Ʒ��0=û���ҵ�
double CScript::GetGoodsOriginalName(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	LPCSTR pVariableName= GetStringParam(cmd,1);
	if (NULL == pVariableName)
	{
		return 0.0f;
	}

	SetScriptVarValue(pPlayer, pVariableName, CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex()));

	M_FREE( pVariableName, sizeof(char)*MAX_VAR_LEN );
	return 1.0f;
}

//! ����GUID �õ�ָ����Ʒ��������ϣ��ĸ�������ֵ
//! ����0����ƷGUID
//! ����1����������ID
//! ����2��ָ������ֵ1����ֵ2
//! ����ֵ��Ҫ�õ��ĸ�������
double CScript::GetGoodsAddonPropertyValues(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	LONG lAddonPropertyID = GetIntParam(cmd, 1);
	if (ERROR_CODE == lAddonPropertyID)
		return 0.0f;
	LONG lAddonPropertyPos = GetIntParam(cmd, 2);
	if (ERROR_CODE == lAddonPropertyPos || (1 != lAddonPropertyPos && 2 != lAddonPropertyPos))
		return 0.0f;

	return pGoods->GetAddonPropertyValues((GOODS_ADDON_PROPERTIES)lAddonPropertyID, lAddonPropertyPos);
}

double CScript::GetGoodsBaseType(const char* CmdStr, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(CmdStr,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	LONG lVal=pGoods->GetGoodsBaseType();
	
	return lVal;
}

//! ����GUID ����ָ����Ʒ��������ϣ��ĸ�������ֵ
//! ����0����ƷGUID
//! ����1����������ID
//! ����2��ָ���޸�ֵ1����ֵ2��
//! ����3���µ�ֵ
//! ����ֵ�������õ�ֵ���������ֵ���������õ�ֵ���п����Ǵﵽ�˸�ֵ�����޻����ޣ�����û�и�����
double CScript::SetGoodsAddonPropertyValues(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	LONG lAddonPropertyID = GetIntParam(cmd, 1);
	if (ERROR_CODE == lAddonPropertyID)
		return 0.0f;
	LONG lAddonPropertyPos = GetIntParam(cmd, 2);
	if (ERROR_CODE == lAddonPropertyPos || (1 != lAddonPropertyPos && 2 != lAddonPropertyPos))
		return 0.0f;
	LONG lNewValue = GetIntParam(cmd, 3);
	if (ERROR_CODE == lNewValue)
		return 0.0f;

	//! ֵ�ĺϷ��ж�
	//! ���
	if(GAP_WEAPON_LEVEL == lAddonPropertyID && 1 == lAddonPropertyPos)
	{
		lNewValue = max(lNewValue, 0);
		lNewValue = min(lNewValue, 20);
	}
	//! ħ��
	else if (GAP_PLUG_EVIL_LEVEL == lAddonPropertyID && 1 == lAddonPropertyPos)
	{
		lNewValue = max(lNewValue, 0);
		lNewValue = min(lNewValue, 10);
	}

	pGoods->ChangeAttribute((GOODS_ADDON_PROPERTIES)lAddonPropertyID, lNewValue, lAddonPropertyPos, true);
	CGoodsFactory::UpdateEquipmentAttr(pGoods);
	pGoods->UpdateAttribute(pPlayer->GetExID());

	return lNewValue;
}

//! ����GUID ����ָ����Ʒ��������ϣ����������ֵ
//! ����0����ƷGUID
//! ����ֵ��������
double CScript::ResetGoodsStochastic(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	pGoods->EDCalculate();
	pGoods->CalculatePKCP();

	pGoods->ImmunityUpdate();
	pGoods->PierceUpdate();
	pGoods->ConstellationUpdate();
	pGoods->LuckyUpdate();
	pGoods->AddonHpUpdate();

	CGoodsFactory::UpdateEquipmentAttr(pGoods);
	pGoods->UpdateAttribute(pPlayer->GetExID());

	return 1.0f;
}
//! ����GUID ɾ��ָ����Ʒ��������ϣ�
//! ����0����ƷGUID
//! ����ֵ��������
double CScript::DelGoodsByGuid(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	if(!pPlayer->DelGoods(pGoods->GetExID(), pGoods->GetAmount()))
		return 0.0f;

	return 1.0f;
}

//! ������Ʒ�������ص�һ����Ʒ��GUID
//! ����0����Ʒԭʼ��
//! ����1������
//! ����2�����ص�GUID
//! ����ֵ�����ش�����Ʒ�ĸ���
double CScript::AddGoodsReturnGuid(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pOriginalName = GetStringParam(cmd, 0);
	if (NULL == pOriginalName)
		return 0.0f;

	LONG lNum = GetIntParam(cmd, 1);
	if(ERROR_CODE == lNum || 0 >= lNum)
		lNum = 1;

	vector<CGoods*> vOutGoods;
	CGoodsFactory::CreateGoods( CGoodsFactory::QueryGoodsIDByOriginalName(pOriginalName), lNum, vOutGoods ,39);
	M_FREE( pOriginalName, sizeof(char)*MAX_VAR_LEN );

	if(0 == vOutGoods.size())
		return 0.0f;

	for(size_t i = 0; i < vOutGoods.size(); ++i)
	{
		if( vOutGoods[i] )
		{			
			DWORD dwGoodsId=vOutGoods[i]->GetBasePropertiesIndex();
			CGUID goodsGuid=vOutGoods[i]->GetExID();
			const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
			DWORD dwGoodsNum=vOutGoods[i]->GetAmount();
			BOOL bAdded=pPlayer->AddGoodsWithProtect(vOutGoods[i]);

			if(bAdded)
			{
				GetGameLogInterface()->logT027_goods_log_pickup(pPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwGoodsNum);
			}
		}
	}	

	if(NULL == vOutGoods[0])
		return 0.0f;

	LPCSTR pVariableName= GetStringParam(cmd, 2);
	if (NULL == pVariableName)
		return 0.0f;

	SetScriptGuidValue(pPlayer, pVariableName, vOutGoods[0]->GetExID());
	M_FREE( pVariableName, sizeof(char)*MAX_VAR_LEN );

	return 1.0f;
}

//! ����GUID �õ�ָ����Ʒ��������ϣ��Ƿ�Ϊ����
//! ����0����ƷGUID
//! ����ֵ��0=ʧ�� 1=���� 2=δ����
double CScript::IsFrostGoods(const char* cmd, char* retStr)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
	if(pPlayer == NULL)
		return 0.0f;

	LPCSTR pGuidName= GetStringParam(cmd,0);
	if (NULL == pGuidName)
		return 0.0f;

	CGUID GoodsGuid = GetScriptGuidValue((CMoveShape*)p_SrcShape, pGuidName);
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );

	if(NULL_GUID == GoodsGuid)
		return 0.0f;

	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack(GoodsGuid);
	if(NULL == pGoods)
		return 0.0f;

	return ((GoodsExManage::GetInstance().IsFrost(pGoods)) ? 1 : 2);
}

///
/// SetAddonDesc( "@goodsGUID", "#str" );
/// @param goodsGUID : ��ƷGUID
/// @param str : �����ַ���
/// @return 0ʧ�ܣ�1�ɹ�
///
double CScript::SetAddonDesc( const char *cmd, char *retStr )
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( p_SrcShape );
	if( pPlayer == NULL )
	{
		return 0.0;
	}

	const char *pGuidName= GetStringParam( cmd, 0 );
	if( NULL == pGuidName )
	{
		return 0.0;
	}

	const char *pStrDesc = GetStringParam( cmd, 1 );
	if( pStrDesc == NULL )
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}

	const CGUID &GoodsGuid = GetScriptGuidValue( (CMoveShape*)p_SrcShape, pGuidName );
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
	
	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack( GoodsGuid );
	if( pGoods == NULL )
	{
		M_FREE( pStrDesc, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}
	if( !pGoods->HasAddonProperty( GAP_GOODSDESC_RES_ID ) )
	{
		M_FREE( pStrDesc, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}
	if(pGoods->SetScriptAddonDesc( pStrDesc ))
	{
		pGoods->UpdateScriptAddonDesc( pPlayer->GetExID() );
		M_FREE( pStrDesc, sizeof(char)*MAX_VAR_LEN );
		return 1.0;
	}

	M_FREE( pStrDesc, sizeof(char)*MAX_VAR_LEN );
	return 0.0;
}

///
/// GetAddonDesc( "@goodsGUID", "#str" );
/// @param goodsGUID : ��ƷGUID
/// @param VarStr : �����ַ�������
/// @return 0ʧ�ܣ�1�ɹ�
///
double CScript::GetAddonDesc( const char *cmd, char *retStr )
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>( p_SrcShape );
	if( pPlayer == NULL )
	{
		return 0.0;
	}

	const char *pGuidName= GetStringParam( cmd, 0 );
	if( NULL == pGuidName )
	{
		return 0.0;
	}

	const char *pVarDesc = GetStringParam( cmd, 1 );
	if( pVarDesc == NULL )
	{
		M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}

	const CGUID &GoodsGuid = GetScriptGuidValue( (CMoveShape*)p_SrcShape, pGuidName );
	M_FREE( pGuidName, sizeof(char)*MAX_VAR_LEN );
	
	CGoods *pGoods = pPlayer->FindGoodsByGuidInPack( GoodsGuid );
	if( pGoods == NULL )
	{
		M_FREE( pVarDesc, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}
	if( !pGoods->HasAddonProperty( GAP_GOODSDESC_RES_ID ) )
	{
		M_FREE( pVarDesc, sizeof(char)*MAX_VAR_LEN );
		return 0.0;
	}
	SetScriptVarValue( (CMoveShape*)p_SrcShape, pVarDesc, pGoods->GetScriptAddonDesc() );
	M_FREE( pVarDesc, sizeof(char)*MAX_VAR_LEN );
	
	return 1.0;
}

//  �������ʹ�õ���Ʒ��ԭʼ��
double CScript::GetUsedGoodsOriginalName(const char* cmd,char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;
	char* pAttrName=GetStringParam(cmd,0);
	if(!pAttrName)
	{
		return 0;
	}
	CGoods* pGoods=pPlayer->FindGoodByGuid(GetUsedItemID());
	if(!pGoods)
	{
		M_FREE( pAttrName, sizeof(char)*MAX_VAR_LEN );
		return 0;
	}
	const char* szGoodsName=CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
	if(szGoodsName)
	{
		SetScriptVarValue(pPlayer,pAttrName,szGoodsName);
		M_FREE( pAttrName, sizeof(char)*MAX_VAR_LEN );
		return 1;
	}
	else
	{
		SetScriptVarValue(pPlayer,pAttrName,"");	
		M_FREE( pAttrName, sizeof(char)*MAX_VAR_LEN );
	}

	return 0;
}

//  �������ʹ�õ���Ʒ��GUID
double CScript::GetUsedGoodsGuid(const char* cmd,char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	if(p_SrcShape->GetType()!=TYPE_PLAYER)
		return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer)
		return 0;

	if (GetUsedItemID() == CGUID::GUID_INVALID)
		return 0;

	char* cGuidName= GetStringParam(cmd,0);
	if (cGuidName== NULL)	return 0;

	SetScriptGuidValue(pPlayer,cGuidName,GetUsedItemID());

	M_FREE( cGuidName, sizeof(char)*MAX_VAR_LEN );
	return 1;
}

// ���һ����Ʒ��������
double CScript::AddGoodsToServer(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer) return 0;

	double lRet = 1;
	char* name=GetStringParam(CmdStr, 0);
	double n = GetIntParam(CmdStr, 1);

	if (name == NULL)
		return false;

	if( n == ERROR_CODE )
	{
		n = 1;
	}

	if (n<1)
	{
		M_FREE( name, sizeof(char)*MAX_VAR_LEN );
		return false;
	}

	vector<CGoods*> vOut;

	DWORD dwIndexID = CGoodsFactory::QueryGoodsIDByOriginalName(name);

	CGoodsBaseProperties* pBaseProperty=CGoodsFactory::QueryGoodsBaseProperties(dwIndexID);
	if(!pBaseProperty)
		return 0;
	if(pBaseProperty->IsHasAddonPropertie(GAP_UNIQUE))
	{
		if(!pPlayer->IsGoodsUnique(name))
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
			return 0;
		}
	}

	CGoodsFactory::CreateGoods(dwIndexID, n, vOut,35);	

	//����true
	if(vOut.size() > 0)
	{
		if(vOut[0])
			lRet = 1;
	}
	
	
	
	for( size_t i = 0; i < vOut.size(); i ++ )
	{
		if( vOut[i] )
		{			
			DWORD dwGoodsId=vOut[i]->GetBasePropertiesIndex();
			CGUID goodsGuid=vOut[i]->GetExID();
			const char* strGoodsName=CGoodsFactory::QueryGoodsName(dwGoodsId);
			DWORD dwGoodsNum=vOut[i]->GetAmount();
			BOOL bAdded=pPlayer->AddGoodsToServer(vOut[i]);
			
			if(bAdded)
			{
				//AddGoods Log
				GetGameLogInterface()->logT027_goods_log_pickup(pPlayer,dwGoodsId,goodsGuid,strGoodsName,3,dwGoodsNum);
			}
		}
	}	
	M_FREE( name, sizeof(char)*MAX_VAR_LEN );
	return lRet;
}

double CScript::AddGoodsToClient(const char* CmdStr, char* retStr)
{
	if(p_SrcShape == NULL) return 0;
	CPlayer* pPlayer=dynamic_cast<CPlayer*>(p_SrcShape);
	if(!pPlayer) return 0;
	if(pPlayer->AddGoodsToClient())
	{
	   return 1;
	 }
	return 0;
}

// �ϴ���Ʒ
double CScript::UploadGoods(const char* cmd, char* retStr/* =NULL */)
{
    if(!GetGame()->IsBounsSConnected())
    {
        return 0;
    }

    if(p_SrcShape == NULL && p_SrcShape->GetType() != TYPE_PLAYER) return 0;

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(p_SrcShape);
    if(!pPlayer)
        return 0;

    CGUID goodsGuid = GetUsedItemID();
    if(goodsGuid == CGUID::GUID_INVALID)
        return 0;

    CGoods* pGoods = pPlayer->FindGoodByGuid(goodsGuid);
    if(pGoods == NULL)
        return 0;

    // �ϴ���Ʒ����
    DWORD dwGoodsNum = GetIntParam(cmd, 0);
    if(dwGoodsNum <= 0 || dwGoodsNum > pGoods->GetAmount())
        return 0;
    
    string strGoodsName = pGoods->GetName();
    DWORD dwGoodsIndex = pGoods->GetBasePropertiesIndex();   

    // �ϴ���BounsServer
    CMessage msg(MSG_S2BDS_UPLOAD_GOODS);    
    tagUploadGoods stUpload;
    stUpload.playerGUID = pPlayer->GetExID();
    stUpload.strAccount = pPlayer->GetAccount();
    stUpload.strGoodsName = strGoodsName;
    stUpload.dwGoodsIndex = dwGoodsIndex;
    stUpload.dwGoodsCounts = dwGoodsNum;
    stUpload.dwWSNumber = GetGame()->GetWSNumber();
    stUpload.dwGSNumber = GetGame()->GetIDIndex();
    DBWriteSet setWriteDB;
    msg.GetDBWriteSet(setWriteDB);
    stUpload.EncodeToDataBlock(setWriteDB);
    msg.SendToBounsServer();

    return 1;
    
}