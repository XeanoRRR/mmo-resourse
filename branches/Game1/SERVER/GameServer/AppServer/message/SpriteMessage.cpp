#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"
#include "..\player.h"
#include "../SpriteSystem.h"
#include "../Region.h"
#include "..\Goods\CGoods.h"
#include "..\Goods\CGoodsFactory.h"
#include "..\serverregion.h"
#include "../../public/PlayerSetupOnServer.h"


//! ��Ӧ�ͻ��˵�С������Ϣ
void OnSpriteMessage_ForClient(CMessage *pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_C2S_SPRITE_AutoSupply_OnOff_Request://! ���ݲ�ʹ�ã��Զ�����ϵͳ�����ܿ�������
		{

		}
		break;
	case MSG_C2S_SPRITE_AutoSupply_ChangeSetup_Request://! �Զ�����ϵͳ���޸�һ��������������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				pPlayer->m_SpriteSystem.OnChangeAutoSupply(pMsg);
				
			}	
		}
		break;
    case MSG_C2S_SPRITE_AutoFight_ChangeSetup_Request://! �Զ�����ϵͳ���޸�һ�������һ���������
        {
            CPlayer *pPlayer = pMsg->GetPlayer();
            if (NULL != pPlayer)
            {
                pPlayer->m_SpriteSystem.OnChangeAutoFight(pMsg);
            }
        }
        break;
    case MSG_C2S_SPRITE_AutoRepair://! �Զ�����ϵͳ���Զ���������
        {
            CPlayer *pPlayer = pMsg->GetPlayer();
            if (NULL != pPlayer)
            {
                //pPlayer->m_SpriteSystem.OnAutoRepair(pMsg);

                if( pPlayer -> IsDied() )
                {
                    //CancelTrading( pPlayer );
                    pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,1));
                    return;
                }

                bool bCan = pPlayer->m_SpriteSystem.CanAutoRepair();
                if (!bCan)
                {
                    //���Ҳ���
                    pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,35));
                    return;
                }

                CServerRegion* pRegion = pMsg -> GetRegion();
                //##��Σ���ʼ������Ʒ
                LONG lContainerId=pMsg->GetLong();
                LONG lPos=pMsg->GetLong();			


                //����IDȡ��������
                CGoodsContainer* pContainer = NULL;
                CGoods* pGoods=NULL;
                switch(lContainerId)
                {
                case CS2CContainerObjectMove::PEI_PACKET:
                    pContainer = pPlayer -> getOriginPack();
                    break;
                case CS2CContainerObjectMove::PEI_EQUIPMENT:
                    pContainer = &( pPlayer -> m_cEquipment );
                    break;						
                    /*			case CS2CContainerObjectMove::PEI_DEPOT:										
                    pContainer = &( pPlayer -> m_cDepot);					
                    break;	*/			
                case CS2CContainerObjectMove::PEI_PACK:
                    pContainer =  pPlayer ->getSubpackContainer();
                    break;
                case CS2CContainerObjectMove::PEI_PACK1:
                    pContainer=pPlayer->getSubpackContainer()->GetSubpack(0)->pSubpackContainer;
                    break;
                case CS2CContainerObjectMove::PEI_PACK2:
                    pContainer=pPlayer->getSubpackContainer()->GetSubpack(1)->pSubpackContainer;
                    break;
                case CS2CContainerObjectMove::PEI_PACK3:
                    pContainer=pPlayer->getSubpackContainer()->GetSubpack(2)->pSubpackContainer;
                    break;
                case CS2CContainerObjectMove::PEI_PACK4:
                    pContainer=pPlayer->getSubpackContainer()->GetSubpack(3)->pSubpackContainer;
                    break;
                case CS2CContainerObjectMove::PEI_PACK5:
                    pContainer=pPlayer->getSubpackContainer()->GetSubpack(4)->pSubpackContainer;
                    break;
                }
                if( pContainer )
                {
                    pGoods=pContainer->GetGoods(lPos);
                }
                //��ƷGuid
               /* CGUID goodsID;
                pMsg->GetGUID(goodsID);                
                CServerRegion* pRegion = pMsg -> GetRegion();
                CGoods *pGoods = NULL;
                pGoods = pPlayer->GetGoodsById(goodsID);*/

                if(pGoods && pGoods -> CanReparied())
                {
                    LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAXIMUM_DURABILITY, 2 );
                    DWORD dwPrice = CGoodsFactory::CalculateRepairPrice( pGoods );
                    dwPrice = static_cast<DWORD>( dwPrice * ( 1 + pRegion -> GetTaxRate() / 100 ) );
                    if( pPlayer ->GetSilver() >= dwPrice )
                    {
                        if( CGoodsFactory::RepairEquipment(pGoods) )
                        {
                            GetGameLogInterface()->logT023_goods_log_repair(pPlayer,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),1);
                            pPlayer ->SetSilver( pPlayer ->GetSilver() - dwPrice );
                            if (pContainer==&( pPlayer -> m_cEquipment )&&lCurrentDurability==0)
                            {
                                DWORD  dwPosition = 0;
                                pPlayer->GetEquipmentContainer()->QueryGoodsPosition(pGoods,dwPosition);
                                pPlayer->MountEquipEx(pGoods,true);
                                pPlayer->UpdateAttribute(TRUE);
                                pPlayer->ProduceEvent(ET_EquipMount,
                                    CS2CContainerObjectMove::PEI_EQUIPMENT,
                                    dwPosition,
                                    pGoods->GetGoodsBaseType(), 
                                    pGoods-> GetAddonPropertyValues( GAP_WEAPON_CATEGORY, 1 ),
                                    (void*)CGoodsFactory::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex()));
                            }
                            CMessage msg(MSG_S2C_SHOP_REPAIR);
                            msg.Add((char)SHOP_REPAIR_SUCCEED);
                            msg.Add(lContainerId);
                            msg.Add(lPos);
                            msg.Add(dwPrice);
                            msg.SendToPlayer( pPlayer -> GetExID() );
                            return;
                        }
                        else
                        {
                            pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,8));
                        }
                    }
                    else
                    {
                        pPlayer -> SendNotifyMessage(CStringReading::LoadString(22,9));
                    }
                }
            }
        }
        break;
	//!-------------------------------------------------------------------------------------------
	//! ��С����ϵͳ��Ϣ
	case MSG_C2S_SaveNewSetupData_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				tagSetupOnServer SetupOnServer;
				pMsg->GetEx(&SetupOnServer, sizeof(tagSetupOnServer));
				BOOL bRevert = pPlayer->m_SetupOnServer.SetSetup(SetupOnServer);
				CMessage msg(MSG_S2C_SaveNewSetupData_Re);
				msg.Add((LONG)bRevert);
				msg.SendToPlayer(pPlayer->GetExID());
			}
		}
		break;
		  //! ������ճ̱�
	case	MSG_C2S_Calendar_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = CGlobeSetup::GetSetup()->pszCalendar;
				if(strScript)
				{
					stRunScript sScript;
					sScript.pszFileName = (char*)strScript;
					sScript.desShape = pPlayer;
					sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					sScript.srcShape	= pPlayer;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
				}
			}
		}
		break;
			//! ���������άħ��
	case	MSG_C2S_JanaweBox_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = CGlobeSetup::GetSetup()->pszMagicBox;
				if(strScript)
				{
					stRunScript sScript;
					sScript.pszFileName = (char*)strScript;
					sScript.desShape = pPlayer;
					sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					sScript.srcShape	= pPlayer;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
				}
			}
		}
		break;
			//! �ʴ��뽱��
	case	MSG_C2S_Answer_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = CGlobeSetup::GetSetup()->pszAnswerRequest;
				if(strScript)
				{
					stRunScript sScript;
					sScript.pszFileName = (char*)strScript;
					sScript.desShape = pPlayer;
					sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					sScript.srcShape	= pPlayer;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
				}
			}
		}
	break;
		//����
	case	MSG_C2S_Help_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = CGlobeSetup::GetSetup()->pszHelpRequest;
				if(strScript)
				{
					stRunScript sScript;
					sScript.pszFileName = (char*)strScript;
					sScript.desShape = pPlayer;
					sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					sScript.srcShape	= pPlayer;
					RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
				}
			}
		}
		break;
	}
}