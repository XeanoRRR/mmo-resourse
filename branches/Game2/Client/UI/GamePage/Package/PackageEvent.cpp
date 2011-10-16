#include "stdafx.h"
#include "../../GamePage.h"
#include "PackageEvent.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../../../GameClient/Game.h"
#include "../../../GameClient/ClientApp/Other/PicList.h"
#include "../../../GameClient/ClientApp/MainPlayerHand.h"
#include "../Package/PackagePage.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../../../../Public/Common/MsgType.h"
#include "../../../../Public/Common/DepotDef.h"
#include "../../../GameClient/ClientApp/Goods/Goods.h"
#include "../../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../../GameClient/ClientApp/TradeSystem/PlayerTrade.h"
#include "../../UIDef.h"
#include "../ChatList/ChatListPage.h"
#include "../../../GameClient/GameControl.h"
#include "../../../GameClient/ClientApp/Mail/Mail.h"
using namespace mail;
#include "../../../GameClient/ClientApp/Mail/MailDef.h"
#include "../../../GameClient/ClientApp/GameDesp/gamedespparser.h"

extern CEGUI::Window* GetWindow(const CEGUI::String& name);

namespace PackageMsgBox
{
    struct tagChangeEquipMSG 
    {
        int nIndex;         //Ŀ����λ
        int nSPos;          //Դ��λ
        int nExtendID;      //Ŀ������ID
        int nSExtendID;     //��Դ����ID
    };
}

void  ChangeEquipMsgBox(MsgEvent& event,eMsgResult UserId)
{
    PackageMsgBox::tagChangeEquipMSG *pEquipMSG = 
        (PackageMsgBox::tagChangeEquipMSG*)event.pData;
    GetInst(PackagePage).GetPackageEvent()->SendChangeEquipMSG(
        pEquipMSG->nIndex,pEquipMSG->nSPos,pEquipMSG->nExtendID,pEquipMSG->nSExtendID);
    SAFE_DELETE(pEquipMSG);
}

PackageEvent::PackageEvent(EVENTTYPE type)
{
    SetEventType(type);
}

PackageEvent::~PackageEvent()
{

}

void PackageEvent::OnPageLoad(GamePage *pPage)
{
    pPage->LoadPageWindow();
    CEGUI::FrameWindow *pPageWindow = static_cast<CEGUI::FrameWindow*>(pPage->GetPageWindow());
    CEGUI::DefaultWindow *pPageBackWindow = static_cast<CEGUI::DefaultWindow*>(pPageWindow->getChild("Package/backgrond"));
	CEGUI::PushButton* pOpenPlayerShopBtn = static_cast<CEGUI::PushButton*>(pPageBackWindow->getChild("Package/backgrond/OpenPlayerShop"));
   
    //���±�����Ʒ
    for(int i=0;i<GRIDITEM;i++)
    {
        SetUpdatePos(i);
        OnPageUpdate(pPage);
        CEGUI::GUISheet *pContainer = static_cast<CEGUI::GUISheet*>(pPageBackWindow->getChildAtIdx(i));
        CEGUI::DragContainer *pDrageWin = static_cast<CEGUI::DragContainer*>(pContainer->getChildAtIdx(0));
        try
        {
            // subscribe the handler.
            pDrageWin->subscribeEvent(
                CEGUI::Window::EventDragDropItemDropped,
                CEGUI::Event::Subscriber(&PackageEvent::handle_GoodsDropped, this));
            pDrageWin->subscribeEvent(CEGUI::DragContainer::EventDragStarted,
                CEGUI::Event::Subscriber(&PackageEvent::handle_GoodsPickUp, this));

            pDrageWin->subscribeEvent(CEGUI::Window::EventMouseButtonDown,
                CEGUI::Event::Subscriber(&PackageEvent::ProcessMouseRBtnDown,this));

            pDrageWin->subscribeEvent(CEGUI::Window::EventMouseEnters,
                CEGUI::Event::Subscriber(&PackageEvent::OnMouseEnter,this));

            pDrageWin->subscribeEvent(CEGUI::Window::EventMouseLeaves,
                CEGUI::Event::Subscriber(&PackageEvent::OnMouseLeave,this));
            
        }
        // if something goes wrong, log the issue but do not bomb!
        catch(CEGUI::Exception&)
        {}
    }
    
    pPageWindow->subscribeEvent(
        CEGUI::FrameWindow::EventCloseClicked,
        CEGUI::Event::Subscriber(&PackageEvent::handle_PageClose,this));

	pOpenPlayerShopBtn->subscribeEvent(
		CEGUI::PushButton::EventClicked,
		CEGUI::Event::Subscriber(&PackageEvent::handle_OpenPlayerShop,this));

    pPageWindow->subscribeEvent("UpdateMoney",CEGUI::Event::Subscriber(&PackageEvent::updateMoney,this));
    
}

void PackageEvent::OnPageOpen(GamePage *pPage)
{
    CEGUI::FrameWindow *pPageWindow = static_cast<CEGUI::FrameWindow*>(pPage->GetPageWindow());
    CEGUI::DefaultWindow *pPageBackWindow = static_cast<CEGUI::DefaultWindow*>(pPageWindow->getChild("Package/backgrond"));
    const CEGUI::Image &pBackImage = SetBackGroundImage(pPageBackWindow,"Package","datafiles/imagesets/Package","backpack.tga");
    FireUIEvent("PackagePage","UpdateMoney");
}

void PackageEvent::OnPageClose(GamePage *pPage)
{

}

void PackageEvent::OnPageUpdate(GamePage *pPage)
{
    CEGUI::FrameWindow *pPageWindow = static_cast<CEGUI::FrameWindow*>(pPage->GetPageWindow());
    CEGUI::DefaultWindow *pPageBackWindow = static_cast<CEGUI::DefaultWindow*>(pPageWindow->getChild("Package/backgrond"));
    //�ҵ���Ҫ���µ�DrageContainer�ؼ�
    CEGUI::GUISheet *pContainer = static_cast<CEGUI::GUISheet*>(pPageBackWindow->getChildAtIdx(m_UpdateGoodsPos));
    CEGUI::DragContainer *pCurDrag = static_cast<CEGUI::DragContainer*>(pContainer->getChildAtIdx(0));
    CEGUI::GUISheet *pImageComp = static_cast<CEGUI::GUISheet*>(pCurDrag->getChildAtIdx(0));
    
    pContainer->setProperty("FrameEnabled", "false");
    pContainer->setProperty("BackgroundEnabled", "false");
    pImageComp->setProperty("FrameEnabled", "false");
    pImageComp->setProperty("BackgroundEnabled", "false");
    pImageComp->setProperty("MousePassThroughEnabled","true");
    pImageComp->setProperty("Text","");
    
    CPlayer *pPlayer = GetGame()->GetMainPlayer();
    CPlayer::tagGoods *pPlayerGoods = pPlayer->GetGoodsOnPlayer(PEI_PACKET,(WORD)m_UpdateGoodsPos);
    
    DWORD dwIconId = 0;
    CGoods *pGoods = NULL;
    char strImageFilePath[128] = "";
    char strImageFileName[128] = "";
    if(pPlayerGoods)
    {
        pGoods = pPlayerGoods->pGoods;
        assert(pGoods->GetProperty()!=NULL);
        if (pGoods->GetProperty()!=NULL)
        {
            dwIconId = pGoods->GetProperty()->dwIconId;
        }
        if(dwIconId != 0)
        {
            // ��õ�ǰ��������Ӧ����Ʒͼ�����ݣ�������ͼ�����óɶ�Ӧ��������Ķ���ͼƬ��
            const char *strIconPath = GetGame()->GetPicList()->GetPicFilePathName(CPicList::PT_GOODS_ICON, dwIconId);
            
            if(strIconPath)
            {
                string strQuality = "";
                long lColor = 0;
                pGoods->GetGoodsQuality(strQuality,lColor);
                if (!strQuality.empty())
                {
                    GetFileName(strQuality.c_str(),strImageFileName);
                    const CEGUI::Image &pQuilityImage = SetBackGroundImage(pContainer,"Quality","datafiles/imagesets/Public",strImageFileName);
                    
                }
                else
                {
                    pContainer->setProperty("Image","");
                }
                GetFilePath(strIconPath,strImageFilePath);
                GetFileName(strIconPath,strImageFileName);
                CEGUI::String strImagesetName = "GoodIcon/";
                strImagesetName += strImageFileName;
                const CEGUI::Image &pGoodIcon = SetBackGroundImage(pImageComp,strImagesetName.c_str(),strImageFilePath,strImageFileName);
                float width  = pGoodIcon.getSourceTextureArea().getWidth();
                float height = pGoodIcon.getSourceTextureArea().getHeight();
                pContainer->setSize(CEGUI::UVector2(cegui_absdim(width),cegui_absdim(height)));
                pCurDrag->setSize(CEGUI::UVector2(cegui_absdim(width),cegui_absdim(height)));               
            }
            else
            {
                pImageComp->setProperty("Image","");
            }
            
            // ����Ʒ������1��ʱ�����ʾ����
            if(pPlayerGoods->lNum > 1)
            {
                char strGoodsNum[128];
                sprintf_s(strGoodsNum,"%d",pPlayerGoods->lNum);
                pImageComp->setText(strGoodsNum);
            }
            else
                pImageComp->setText("");
        }
    }
    else
    {
        pImageComp->setProperty("Image","");
        pImageComp->setProperty("Text","");
        pContainer->setProperty("Image","");
    }
}

void PackageEvent::OnPageRelease(GamePage *pPage)
{

}

bool PackageEvent::handle_GoodsDropped(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;
    CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
    CMainPlayerHand *pHand = GetGame()->GetMainPlayerHand();
    CGUID guid = pMainPlayer->GetExID();
    CPlayer::tagGoods* pstGoods = pHand->GetTGoodsOfMainPlayerHand();
    if (pstGoods->lNum == 0)
        return false;
    // cast the args to the 'real' type so we can get access to extra fields
    const DragDropEventArgs& dd_args =
        static_cast<const DragDropEventArgs&>(args);
    //�õ���ǰ������ק����Ʒ����
    DragContainer *pCurDrag = dd_args.dragDropItem;
    //�õ���ǰ����ק��Ʒ�����Ǹ���λ����
    //CEGUI::Window *pPickUpContainer = pCurDrag->getParent();
    //���º󣬱��������ϵ�DrageItem
    CEGUI::Window *pReplaceGoods = dd_args.window;
    //����Ĳ�
    CEGUI::Window *pPutContainer = dd_args.window->getParent();
    //��ȡ��ͼ���õĸ��������ĺ������������Ϊ0����ʾ��������ǿյģ������Ҫ��Ʒ����
    long lChildCount = (long)dd_args.window->getChildCount();
    if (pPutContainer->getName() == "Package/backgrond")
    {
        //������ԭ���ĸ�������
        pHand->ClearMainPlayerHand();
        return false;
    }
    else if(lChildCount == 1 && pPutContainer->getParent()->getName() == "Package/backgrond")    //����������Ʒλ��
    {
        if(pCurDrag == pReplaceGoods) //�Ż�ԭ���ĸ����
        {
            pHand->ClearMainPlayerHand();
            return true;
        }
        //���øղ��������Ʒ
        //pPutContainer->addChildWindow(pCurDrag);
        //pCurDrag->setPosition(uvector2(udim(0, 0),UDim(0, 0)));
        
        //����ԭ�������е���Ʒ���ղ�������Ʒ���Ǹ���������
        //pPickUpContainer->addChildWindow(pReplaceGoods);
        //pPutContainer->removeChildWindow(pReplaceGoods);
        
        //������Ʒͼ�겢��������
        //ԴͼƬ����
        CEGUI::Window *pSrcImageComp = pCurDrag->getChildAtIdx(0);
        //Ŀ��ͼƬ����
        CEGUI::Window *pDestImageComp = pReplaceGoods->getChildAtIdx(0);
        
        CEGUI::String SrcGoodImage = pSrcImageComp->getProperty("Image");
        CEGUI::String DestGoodImage = pDestImageComp->getProperty("Image");
        //��ȡ����
        CEGUI::String srcGoodsNum  = pSrcImageComp->getText();
        CEGUI::String DestGoodsNum = pDestImageComp->getText();
        //�Ƿ��ܽ�����Ʒ
        bool bCanSwap = true;
        bool bBind = false;
        CGoods* pGoodsInItem = GetGame()->GetMainPlayerHand()->GetPGoodsOfMainPlayerHand();
        if (pGoodsInItem)
        {
            if( pstGoods->lType > DEPOT_CONTAINER_TYPE_BEGIN_VALUE  && pstGoods->lType < DEPOT_CONTAINER_TYPE_END_VALUE )
            {
                if( pGoodsInItem->GetEffectVal( GAP_CANNOT_SAVE_DEPOT, 1 ) )
                    bCanSwap = false;
            }

            //
            if( PEI_EQUIPMENT == pstGoods->lType )
            {
                DWORD dwBind = pGoodsInItem->GetEffectVal(GAP_BIND, 1);
                if( eBT_DressAndCantUntie == dwBind  ||  eBT_DressAndBind == dwBind )
                {
                    bBind = true;
                }
            }
        }
        if( bCanSwap )
        {
            
            CEGUI::FrameWindow *pPageWindow = static_cast<CEGUI::FrameWindow*>(GetInst(PackagePage).GetPageWindow());
            CEGUI::DefaultWindow *pPageBackWindow = static_cast<CEGUI::DefaultWindow*>(pPageWindow->getChild("Package/backgrond"));
            long lpos = GetIndexByChild(pPageBackWindow,pPutContainer);
           
            // �Ƿ��� װ�����
            if( bBind )
            {
                //������Ϣ����ʾװ�����
                MsgEvent bindEvent;
                bindEvent.pContext = AppFrame::GetText("Goods_310");    //����Ʒװ���󽫱��󶨣��Ƿ������
                PackageMsgBox::tagChangeEquipMSG *pMSGData = new PackageMsgBox::tagChangeEquipMSG;
                pMSGData->nIndex       = lpos;
                pMSGData->nSPos        = pstGoods->lPos;
                pMSGData->nExtendID    = PEI_PACKET;
                pMSGData->nSExtendID   = pstGoods->lType;
                bindEvent.pData = pMSGData;
                bindEvent.bMode = false;
                bindEvent.pEventDef.FunTable = ChangeEquipMsgBox;
                bindEvent.pEventDef.BoxType = Msg_YesNo;
                GetInst(MsgEventManager).PushEvent(bindEvent);
            }
            else
            {
                 //����ͼ��
                pDestImageComp->setProperty("Image",SrcGoodImage);
                pSrcImageComp->setProperty("Image",DestGoodImage);
                //��������
                pDestImageComp->setText(srcGoodsNum);
                pSrcImageComp->setText(DestGoodsNum);
                long count = pstGoods->lNum;
                const CGUID &sguid = pstGoods->pGoods->GetExID();
                SendPutDownMes(TYPE_PLAYER,guid,pstGoods->lType,pstGoods->lPos,TYPE_PLAYER,guid,PEI_PACKET,lpos,TYPE_GOODS,sguid,count);
                pHand->ClearMainPlayerHand();
            }
        }
        else
        {
            // "����Ʒ��ֹ����ֿ�"
            TCHAR	szPrompt[ 512 ];
            wsprintf( szPrompt, AppFrame::GetText("Depot_35")   //������Ʒ����%s�� ������������У�
                , pGoodsInItem->GetName() );
            GetInst(MsgEventManager).PushEvent(Msg_Ok,szPrompt);
            
        }
        //if ()
        //{
        //   
        //} 
        //else //�ɺϲ�����Ʒ����Ҫ����Ŀ������ ���ԭ����
        //{
        //    //����ͼ��
        //    pDestImageComp->setProperty("Image",SrcGoodImage);
        //    pSrcImageComp->setProperty("Image","");
        //    //��������
        //    long lNewNum = CEGUI::PropertyHelper::stringToInt(srcGoodsNum)
        //        + CEGUI::PropertyHelper
        //    pDestImageComp->setText(srcGoodsNum+);
        //    pSrcImageComp->setText("");
        //}
        //pPickUpContainer->notifyScreenAreaChanged();

          
    }

    return true;
}

bool PackageEvent::handle_GoodsPickUp(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;
    CMainPlayerHand *pHand = GetGame()->GetMainPlayerHand();
    const WindowEventArgs& dd_args = static_cast<const WindowEventArgs&>(args);
    //�õ���ǰ������ק����Ʒ����
    DragContainer *pCurDrag = static_cast<DragContainer*>(dd_args.window);
    long pos = GetCurGridPos(pCurDrag);
    CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
    CPlayer::tagGoods *pPlayerGoods = pMainPlayer->GetGoodsOnPlayer(PEI_PACKET,(WORD)pos);
    if(pPlayerGoods)
    {
        pHand->SetMainPlayerHandByGoods(pPlayerGoods->bChange,pPlayerGoods->lNum,pos,pPlayerGoods->pGoods,PEI_PACKET);
    }
    else
    {
        pHand->ClearMainPlayerHand();
    }
    return true;
}


void PackageEvent::SendPutDownMes(long SorceType/*Դ����*/,const CGUID& SorceGUID/*Դ�����GUID*/,long lSorceObjectExtendID,long lSorceObjectPos,
                                     long DesType,const CGUID& DesGUID,long lDesObjectExtendID,long lDesObjectPos,long lObjectType,const CGUID& ObjectGUID,long lObjectNum)
{
    if (g_bDebug)
    {
        /// �������������Ʒ�ƶ�����Ϣ
        //char strTitle[64] = "";
        //if (GetGame()->GetCGuiEx()->GetItemPageEx()->GetFinishBagState())
        //{
        //    strcpy_s(strTitle,"--�ͻ�������������͵ģ�������Ʒ�ƶ���Ϣ--");
        //}else
        //    strcpy_s(strTitle,"--�ͻ�������������͵���Ʒ����ͨ���ƶ���Ϣ--");
        //char strPrintInfor[512] = "";
        //char strGoodsGUID[64] = "";
        //ObjectGUID.tostring(strGoodsGUID);
        //sprintf_s(strPrintInfor,"%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n",
        //    strTitle,
        //    "��Ʒԭ�����ڵ�����ID:",lSorceObjectExtendID,
        //    "��Ʒ��ԭ�������е�λ��:",lSorceObjectPos,
        //    "��ƷҪ�ƶ�����Ŀ������ID:",lDesObjectExtendID,
        //    "��ƷҪ�ƶ���Ŀ��������λ��:",lDesObjectPos,
        //    "���β�������Ʒ��GUID:",strGoodsGUID,
        //    "���β�������Ʒ����:",lObjectNum);
        //PutStrToFileSaveToDir( "log/GoodsMoveLog", "��¼��Ʒ���ƶ�����", strPrintInfor, false );
        //////////////////////////////
    }

    CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
    msg.Add((LONG)SorceType);
    msg.Add(SorceGUID);
    msg.Add((LONG)lSorceObjectExtendID);
    msg.Add((LONG)lSorceObjectPos);
    msg.Add((LONG)DesType);
    msg.Add(DesGUID);
    msg.Add( lDesObjectExtendID);
    msg.Add( lDesObjectPos );
    msg.Add((LONG)lObjectType);
    msg.Add(ObjectGUID);
    msg.Add((LONG)lObjectNum);
    msg.Send();
}

bool PackageEvent::handle_PageClose(const CEGUI::EventArgs& args)
{
    GetInst(PackagePage).Close();
    return true;
}

bool PackageEvent::handle_OpenPlayerShop(const CEGUI::EventArgs& args)
{
	if(GetGame()->GetMainPlayer()->GetState() == CShape::STATE_DIED)
		return false;

	// �����ͼ��ʱ���ܿ��������̵�

	// ����ҳ���ʱ���ܿ��������̵�

	// �򿪸����̵�ǰҪ�ȹر���ֵ�̵�

	// ��ļ״̬���ܿ���
	if(GetGame()->GetMainPlayer()->IsRecruitingTeam())
	{
        GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Shop_12"),
            0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		return false;
	}

	// ս��״̬���ܿ���
	if (GetGame()->GetMainPlayer()->GetState()==CShape::STATE_FIGHT)
	{
		GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Shop_17"), 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		return false;
	}
	/// �ж������Χ5*5�ķ�Χ���Ƿ��б������Ѿ��ڿ����ˣ��������������
	if (GetGame()->GetMainPlayer()->CheckIsAnyBodyShopOpenedOnRange(2))
	{
		GetInst(ChatListPage).AddChatWords(AppFrame::GetText("Shop_16"), 0xffffffff, 0, 0xff000000,"", eNOTIFYPOS_CENTER);
		return false;
	}

	//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetMoveInfo(0,0,0);
	GetGame()->GetGameControl()->AutoMoveStop();

	CMessage msg(MSG_C2S_PLAYERSHOP_CREATE);
	msg.Send((CClient*)GetGame()->GetNetClient());

	//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-03.wav");
	return true;
}

bool PackageEvent::OnRBClickGoodsForOtherUIPages(CPlayer::tagGoods* pstGoods)
{
	// ���û����Ʒ�򷵻أ������������״̬Ҳ����
	if(!pstGoods  ||  !pstGoods->pGoods)
		return false;

	// ������Ʒ����ֹ����޸�
	CPlayer::tagGoods stGoods = *pstGoods;
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
	if (pHand!=NULL)
	{
		CGoods* pGoods = pHand->GetPGoodsOfMainPlayerHand();
		if (pGoods!=NULL&&pGoods->GetExID()==stGoods.pGoods->GetExID())
		{
			pGoods->SetHaveShadowState(false);
			pHand->DelShadowStateOfGoods();
			pHand->ClearMainPlayerHand();
		}
	}

	//add by shixi 20100921
	if ( GetWindow(PERSONAL_MAIL)->isVisible() )
	{
		GetInst(CMail).AddGoods( &stGoods );
		return true;
	}


	// �򿪽��׽��棬�����Ҽ������Ʒ
	if (GetWindow("PlayerTrade")->isVisible())
	{
		GetInst(PlayerTrade).OnInputGoods(&stGoods);
		return true;
	}
	return false;
}

bool PackageEvent::ProcessMouseRBtnDown(const CEGUI::EventArgs &args)
{
    CMainPlayerHand *pHand = GetGame()->GetMainPlayerHand();
    if (pHand && pHand->GetGoodsIndexOfMainPlayerHand()!=0) 
        return true;
    CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
    if (pMainPlayer->IsDied()) return true;
    
    const CEGUI::MouseEventArgs &e_mouse = static_cast<const CEGUI::MouseEventArgs&>(args);
    CEGUI::DragContainer *pDrug = static_cast<CEGUI::DragContainer*>(e_mouse.window);
    //����Ҽ�����
    
    if(e_mouse.button == CEGUI::RightButton)
    {
        long opPos = GetCurGridPos(pDrug);
        if(opPos == -1)
            return false;
        CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
        long nSExtendId = -1;
        long nIndex = -1;
        long nExtendId = -1;
        if (opPos>= 0 && opPos<20)  //��������Χ��
        {
            nSExtendId = PEI_PACKET;
        }
        CPlayer::tagGoods *ptagGoods = pMainPlayer->GetGoodsOnPlayer((WORD)nSExtendId,(WORD)opPos);
        if(ptagGoods)
        {
			//modify by shixi
			if(ptagGoods->pGoods->IsLocked())
			{
				return false;
			}
            CGoods *pGoods = ptagGoods->pGoods;
            if (pGoods == NULL && !pMainPlayer->GetGoodsUseState((WORD)pGoods->GetIndex()))
                return false;

			if (OnRBClickGoodsForOtherUIPages(ptagGoods))
				return true;

            // ԭ��λ����Ʒ,���Ѽ�¼��ԭ��λ��λ�ú���������
            if(pGoods != NULL && opPos != -1)
            {
                
                if(nSExtendId != -1)
                {
                    CGoodsList::tagGoods2* pstGoods = CGoodsList::GetProperty(pGoods->GetIndex());
                    if(!pstGoods)
                        return false;

                    DWORD dwType = pstGoods->BaseProperty.dwType;
					if (dwType==GBT_CONSUMABLE)
					{
						pMainPlayer->UseItem(pGoods->GetIndex(),nSExtendId,opPos);
						return true;
					}
					else if (dwType==GBT_EQUIPMENT)
					{
						nIndex = (long)pGoods->GetEquipType();
						nExtendId = PEI_EQUIPMENT;
					}
					else if (dwType==GBT_SUBPACK)
					{
						if (pGoods->GetEffectVal(GAP_PACK_TYPE,1)==2)
						{
							CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
							if (pHand!=NULL)
							{
								pHand->SendPutDownMes(TYPE_PLAYER,pMainPlayer->GetExID(),
									nSExtendId,opPos,TYPE_PLAYER,pMainPlayer->GetExID(),
									PEI_BUSINESPACKFIELD,0,TYPE_GOODS,pGoods->GetExID(),
									pGoods->GetNum());
							}
							return true;
						}
					}
                }

                if (nIndex != -1 && nExtendId != -1)
                {
                    WORD wAction = pMainPlayer->GetAction();
                    if(wAction>=CShape::ACT_MEGRIM) return false;

                    // �Ƿ��� װ�����
                    DWORD dwBind = pGoods->GetEffectVal(GAP_BIND, 1);
                    if( eBT_DressAndCantUntie == dwBind  ||  eBT_DressAndBind == dwBind )
                    {   
                        MsgEvent bindEvent;
                        bindEvent.pContext = AppFrame::GetText("Goods_310");    //����Ʒװ���󽫱��󶨣��Ƿ������
                        PackageMsgBox::tagChangeEquipMSG *pMSGData = new PackageMsgBox::tagChangeEquipMSG;
                        pMSGData->nIndex       = nIndex;
                        pMSGData->nSPos        = opPos;
                        pMSGData->nExtendID    = nExtendId;
                        pMSGData->nSExtendID   = nSExtendId;
                        bindEvent.pData = pMSGData;
                        bindEvent.bMode = false;
                        bindEvent.pEventDef.FunTable = ChangeEquipMsgBox;
                        bindEvent.pEventDef.BoxType = Msg_YesNo;
                        GetInst(MsgEventManager).PushEvent(bindEvent);
                    }
                    else
                    {
                        SendChangeEquipMSG(nIndex,opPos,nExtendId,nSExtendId);
                    }
                }
            }
            return true;
        }
    }
    return true;
}


/* 
*  ����װ����һ�����Ҽ����װ���󴥷�
*  ������nIndex��ʾĿ����λ��nExtendId��ʾĿ��������
*        nSPos��ʾԴ��λ��nSExtendId��ʾԴĿ��������
*/
void PackageEvent::SendChangeEquipMSG(int nIndex,int nSPos,int nExtendID,int nSExtendID)
{
    DWORD dwAmount=1;
    CPlayer::tagGoods *pstGoods=NULL;
    CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
    if (pMainPlayer == NULL)
        return;
    // ԭ��λλ�õ���Ʒ
    if(nSExtendID==PEI_PACKET)
        pstGoods= pMainPlayer->GetGoodsOnPlayer(PEI_PACKET,nSPos);
    //else if(nSExtendID>=PEI_SUBPACK)
    //    pstGoods= GetCoteData(nSExtendID-PEI_PACK1,nSPos);
    if(!pstGoods)	return;
    if(!pstGoods->pGoods)	return;
    dwAmount=pstGoods->pGoods->GetNum();
    if(pstGoods==NULL || pstGoods->pGoods==NULL)return;

    CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
    msg.Add((LONG)TYPE_PLAYER);
    msg.Add(pMainPlayer->GetExID());
    msg.Add((LONG)nSExtendID);
    msg.Add((LONG)nSPos);
    msg.Add((LONG)TYPE_PLAYER);
    msg.Add(pMainPlayer->GetExID());
    msg.Add((LONG)nExtendID);
    msg.Add((LONG)nIndex);
    msg.Add((LONG)TYPE_GOODS);
    msg.Add(pstGoods->pGoods->GetExID());
    msg.Add((LONG)dwAmount);
    msg.Send();
}

//�õ���ǰ�����ĵĸ�������
long PackageEvent::GetCurGridPos(CEGUI::DragContainer *pDrag)
{
    
    CEGUI::FrameWindow *pPageWindow = static_cast<CEGUI::FrameWindow*>(GetInst(PackagePage).GetPageWindow());
    CEGUI::DefaultWindow *pPageBackWindow = static_cast<CEGUI::DefaultWindow*>(pPageWindow->getChild("Package/backgrond"));
    
    return GetIndexByChild(pPageBackWindow,pDrag->getParent());    
}

bool PackageEvent::updateMoney(const CEGUI::EventArgs & args)
{
    CPlayer *pMainplayer = GetGame()->GetMainPlayer();
    DWORD dwSilver = pMainplayer->GetSilverMoney(); //����
    DWORD dwGold   = pMainplayer->GetMoney();       //���
    CEGUI::Window *st_silver = GetWndMgr().getWindow("Package/backgrond/st_silver");
    st_silver->setText(CEGUI::PropertyHelper::intToString(dwSilver));
    CEGUI::Window *st_gold = GetWndMgr().getWindow("Package/backgrond/st_Gold");
    st_gold->setText(CEGUI::PropertyHelper::intToString(dwGold));
    return false;
}

bool PackageEvent::OnMouseEnter(const CEGUI::EventArgs & args)
{
    const CEGUI::MouseEventArgs &e_mouse = static_cast<const CEGUI::MouseEventArgs&>(args);
    static CEGUI::Window *pLastWin = 0;
    CEGUI::Window *pHoverWin = e_mouse.window;
    if(!pHoverWin)
        return false;
    if (pHoverWin != pLastWin)
    {    
        pLastWin = pHoverWin;
        CEGUI::Window *pPageWindow = GetInst(PackagePage).GetPageWindow();
        CEGUI::Window *pPageBackWindow = pPageWindow->getChild("Package/backgrond");
        m_UpdateGoodsPos = GetIndexByChild(pPageBackWindow,e_mouse.window->getParent());
        CEGUI::String WndName = pHoverWin->getParent()->getName() + "\n";
        OutputDebugString(WndName.c_str());
        GetGame()->GetGameDespParser()->SetSourceUI(GameDespParser::SI_PACKAGE);
        FireUIEvent("GoodsDesp","updateDesp");
    }
    OutputDebugString("�򿪡�����Ʒ��������\n");
    CGoods* pMouseGoods = GetGame()->GetMainPlayer()->GetGoodsByPosition(PEI_PACKET,m_UpdateGoodsPos);
    if(pMouseGoods)
        GetWndMgr().getWindow("GoodsDesp")->setVisible(true);
    //FireUIEvent("GoodsDesp",EVENT_OPEN);
    return true;
}

bool PackageEvent::OnMouseLeave(const CEGUI::EventArgs & args)
{
    OutputDebugString("�رա�����Ʒ��������\n");
    GetWndMgr().getWindow("GoodsDesp")->setVisible(false);
    //FireUIEvent("GoodsDesp",EVENT_CLOSE);
    return true;
}