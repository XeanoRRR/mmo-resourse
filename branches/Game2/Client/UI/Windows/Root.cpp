/**
* @filename:Root.h
* @date: 2010/6/9
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: ��Root����Ĵ���
*/
#include "stdafx.h"
#include "../UIDef.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/Player.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../../Public/Common/DepotDef.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/GameControl.h"

extern void ProcessMainbarDCToSkillTemplate(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd);

bool ProcessMainBarDCToRoot(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd);
bool ProcessPackageDCToRoot(CEGUI::Window* targetWnd,CEGUI::Window* SourceWnd);

bool OnDCDroppedRootListener(const CEGUI::EventArgs& e);
//��Root����ע���¼�
void SubscriberRootEvent()
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* root = mgr.getWindow("Root");
	if(!root)
		return;
	root->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,CEGUI::Event::Subscriber(OnDCDroppedRootListener));
}
//������������DC�Ϸŵ������ϵ��¼���Ӧ����
bool OnDCDroppedRootListener(const CEGUI::EventArgs& e)
{
	CEGUI::Window* targetWnd= WEArgs(e).window;
	CEGUI::DragContainer* dropWnd = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;
	if( dropWnd->getName().find("MainBarPage") != CEGUI::String::npos)
		return ProcessMainBarDCToRoot(targetWnd,dropWnd);
	else if( dropWnd->getName().find("Package") != CEGUI::String::npos ||
        CEGUI::WindowManager::getSingleton().getWindow("PackagePage")->isChildRecursive(dropWnd->getID()))
		return ProcessPackageDCToRoot(targetWnd,dropWnd);
	return false;
}

bool ProcessMainBarDCToRoot(CEGUI::Window* targetWnd,CEGUI::Window* sourceWnd)
{
	ProcessMainbarDCToSkillTemplate(targetWnd,sourceWnd);
	return true;
}

bool ProcessPackageDCToRoot(CEGUI::Window* targetWnd,CEGUI::Window* SourceWnd)
{
    CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
    CMainPlayerHand *pMainPlayerHand = GetGame()->GetMainPlayerHand();
    if (pMainPlayer->GetAction()!=CShape::ACT_MEGRIM)
    {
        if (pMainPlayerHand->GetPGoodsOfMainPlayerHand())
        {
            // ���壺��������Ʒ���ܶ���
            if( pMainPlayerHand->GetPGoodsOfMainPlayerHand()->IsLocked() )
            {
                return 0;
            }

            // ���壺����ǲֿ��ڵ���Ʒ���ܶ���
            if( pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType > DEPOT_CONTAINER_TYPE_BEGIN_VALUE  &&   pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType < DEPOT_CONTAINER_TYPE_END_VALUE )
                return 0;

            if( GetGame()->GetSetup()->bTradeDisable)
            {
                return 1;
            }
            //�ж��Ƿ���ѫ��
            if(pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetProperty()->dwType==GBT_MEDAL)
            {
                long lType = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType;
                long lpos = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lPos;
                //Դ�Ƿ�ѫ���б�
                if(lType == PEI_MEDAL)
                {
                    pMainPlayerHand->ClearMainPlayerHand();
                }
                else if(lType == PEI_EQUIPMENT)
                {
                    //GetGame()->GetCGuiEx()->GetCharacterPageEx()->ChangeEquip(0xffffffff,lpos,PEI_MEDAL,PEI_EQUIPMENT);
                    pMainPlayerHand->ClearMainPlayerHand();
                }
                return 1;
            }

            // �жϴ���Ʒ�Ƿ��Ƕ����ͻ���ʧ����Ʒ
            if (pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_PARTICULAR_ATTRIBUTE,1)&16)
            {
                //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = 2;
                //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(GetGame()->GetCGuiEx()->GetItemPageEx()
                //	,CStringReading::LoadText(208,214),CMessageBoxPageEx::MSG_OK_NO,true);
                return 0;
            }

            // ���壺��������Ʒ
            long dwFreezeVal = pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_FREEZE,1);
            if (dwFreezeVal==eFT_Frost||dwFreezeVal==eFT_Thawing||dwFreezeVal==eFT_CantThaw)
            {
                //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(NULL,CStringReading::LoadText(208,154),CMessageBoxPageEx::MSG_OK,true);
                return 0;
            }

            // ���壺�������Ʒ
            long dwBindVal = pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_BIND,1);
            if( dwBindVal==eBT_Bind||dwBindVal==eBT_CantUntie||dwBindVal==eBT_GetAndCantUntie||dwBindVal==eBT_GetAndBind )
            {
                //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = 6;
                //GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(GetGame()->GetCGuiEx()->GetItemPageEx(), CStringReading::LoadText(208,155),CMessageBoxPageEx::MSG_OK_NO, true);
                return 0;
            }
            CClientRegion *pRegion = GetGame()->GetRegion();
            long lPos=pMainPlayer->GetTileY()*pRegion->GetWidth()+pMainPlayer->GetTileX();
            DWORD dwAmount = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lNum;
            long lType = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType;
            long lpos = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lPos;
            pMainPlayerHand->SendPutDownMes(TYPE_PLAYER,pMainPlayer->GetExID(),lType,lpos,TYPE_REGION,
                pRegion->GetExID(),0,lPos,TYPE_GOODS,pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetExID(),dwAmount);
            GetGame()->GetGameControl()->SetMoveAble(false);

            return 1;
        }
        else if (pMainPlayerHand->GetGoodsIndexOfMainPlayerHand()!=0)
        {
            pMainPlayerHand->ClearMainPlayerHand();
        }
    }
	return false;
}