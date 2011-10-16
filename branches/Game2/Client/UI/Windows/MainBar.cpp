#include "stdafx.h"
#include "MainbarPreDef.h"
#include "SkillDef.h"
#include "../UIDef.h"
#include "../../GameClient/Game.h"
#include "../../GameClient/ClientApp/MainPlayerHand.h"
#include "../../GameClient/ClientApp/Skills/SkillListXml.h"
#include "../../GameClient/ClientApp/Skills/SkillLogicSys.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../ui/UIManager.h"
#include "../../ui/UIExtendComponent/AnimEffectIcon.h"

extern void SubscriberRootEvent();

//�ɼ���ID������DragContainer�����ӿؼ�StaticImage����ʾ(�޸�ͼ������ÿؼ�ID�뼼��ID����)
extern void SetDCIconBySkillID(uint skillID,CEGUI::Window* dragContainer);

//mainbar���¼�ע��
void SubscriberMainBarEvent(CEGUI::Window* pageWnd);

//����ĳ�ʼ��״̬����
void SetInitializedMainBarUIProperty(CEGUI::Window* pageWnd);

//��קͼ�굽mainbar������¼�����
bool OnMainBarDragContainerDropped(const CEGUI::EventArgs& e);

//������ͼ�굽mainbar���棬����DragContainerLeaves�¼�
bool OnDCLeavesFromSkillToMainbar(const CEGUI::EventArgs& e);

//����dc�����ƻ����mainbar�е�����
uint GetDCIndexInMainbar(CEGUI::Window* dc);

//����dc�����ƻ����mainbar�е�����
uint GetDCIndexInMainbar(CEGUI::Window* dc)
{
	if(!dc)
		return 0;
	for(int i = 0 ; i < SHORTCUT_COUNT ; ++i)
	{
		char dcWndName[256];
		sprintf(dcWndName,"MainBarPage/StaticImage%d/DragContainer",i);
		CEGUI::String wndName = dcWndName;
		if(dc->getName()== wndName)
			return i;
	}
	return 0;
}

void SubscriberMainBarEvent(CEGUI::Window* pageWnd)
{
	if(!pageWnd)
		return ;
	//ע��Root�����¼�
	SubscriberRootEvent();
	//������ȴЧ����ͼ��
    CEGUI::Imageset * coolImageSet = NULL;

    //������Դ�ļ�·��
    //CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
    //    (CEGUI::System::getSingleton().getResourceProvider());
    //rp->setResourceGroupDirectory(UIRes, "datafiles/imagesets");
    
    if(CEGUI::ImagesetManager::getSingleton().isDefined("CoolDown") == false)
        coolImageSet = &CEGUI::ImagesetManager::getSingleton().create("CoolDownEffect.imageset","");
    else
        coolImageSet = &CEGUI::ImagesetManager::getSingleton().get("CoolDown");
    //ע��DragItem�Ϸŵ���ݼ��ϵĴ���
	char dcWndName[256];
	for(int i = 0 ; i < SHORTCUT_COUNT ; ++i)
	{
		sprintf(dcWndName,"MainBarPage/StaticImage%d/DragContainer",i);
		CEGUI::Window* simgWnd = pageWnd->getChildRecursive(dcWndName);
		if(simgWnd)
		{
			//DragContainer�϶������沢�ͷ�
			simgWnd->subscribeEvent(CEGUI::Window::EventDragDropItemDropped,CEGUI::Event::Subscriber(OnMainBarDragContainerDropped));
			//����ICont����ԭ���浽mainbar
			simgWnd->subscribeEvent(CEGUI::Window::EventDragDropItemLeaves,CEGUI::Event::Subscriber(OnDCLeavesFromSkillToMainbar));
		}
        sprintf(dcWndName,"MainBarPage/StaticImage%d/CoolImage%d",i,i);
        //sprintf(dcWndName,"MainBarPage/StaticImage%d/DragContainer/sImage",i);
        CEGUI::Window* coolWnd = pageWnd->getChildRecursive(dcWndName);
        AnimEffectIcon *pAnimIcon = new AnimEffectIcon(coolWnd,*coolImageSet);
        UIManager::AddAnimIcon(coolWnd,pAnimIcon);
	}
}

bool OnMainBarDragContainerDropped(const CEGUI::EventArgs& e)
{
	CEGUI::Window* dragTargetWnd = WEArgs(e).window;
	CEGUI::Window* dropWnd = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;
#ifdef _DEBUG
	const char* targetName = dragTargetWnd->getName().c_str();
	const char* dropName = dropWnd->getName().c_str();
#endif
	/////////////////////////////////////////////////
	// zhaohang  2010/5/18 
	// 	//������������DragContainer�϶���mainbar��DragContainer��
	/////////////////////////////////////////////////
	uint targetID = dragTargetWnd->getID();
	uint dropID = dropWnd->getID();
	CEGUI::String dropWndName = dropWnd->getName();
	SkillLogicSys& SkillLS = GetInst(SkillLogicSys);

	//�ж��Ƿ����ϼ��ܵ�mainbar����
	if(dropWndName.find(SKILL_PAGE_NAME) != CEGUI::String::npos)//��
	{
		//�޸�ͼ��͹���ID
		SetDCIconBySkillID(dropID,dragTargetWnd);
		/////////////////////////////////////////////////
		// zhaohang  2010/5/18 
		// ���ÿ�ݼ�
		uint hotKeyIdx = GetDCIndexInMainbar(dragTargetWnd);
		SkillLS.Send_Set_Skill_HotKey(hotKeyIdx+SHORTCUT_COUNT,dropID);//0~11->F1~F12,���Լ�12
		CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
		if(pHand)
			pHand->ClearMainPlayerHand();//�Ϸųɹ��ˣ�����Ҫ�ͷ���������ͼ��
	}
	/////////////////////////////////////////////////
	// zhaohang  2010/5/18 
	// ����mainbar�Ļ��ཻ��
	if(dropWndName.find("MainBarPage") != CEGUI::String::npos)
	{
		SetDCIconBySkillID(dropID,dragTargetWnd);
		SetDCIconBySkillID(targetID,dropWnd);
		//�����ݼ�����
		uint hotkeyIndx = GetDCIndexInMainbar(dragTargetWnd);
		SkillLS.Send_Set_Skill_HotKey(hotkeyIndx+SHORTCUT_COUNT,dropID);
		hotkeyIndx = GetDCIndexInMainbar(dropWnd);
		SkillLS.Send_Set_Skill_HotKey(hotkeyIndx+SHORTCUT_COUNT,targetID);
	}
	/////////////////////////////////////////////////
	return true;
}

//����Icon�ϵ�mainbar��DC�뿪���ܽ���
bool OnDCLeavesFromSkillToMainbar(const CEGUI::EventArgs& e)
{
	CEGUI::Window* targetDC = WEArgs(e).window;
	CEGUI::Window* sourceDC = static_cast<const CEGUI::DragDropEventArgs&>(e).dragDropItem;
#ifdef _DEBUG
	const char* targetName = targetDC->getName().c_str();
	const char* dropName = sourceDC->getName().c_str();
#endif
	//�ж�ԴDc�Ƿ��Ǽ���ICon
	CEGUI::String dropWndname = sourceDC->getName();
	if(dropWndname.find(SKILL_PAGE_NAME)== CEGUI::String::npos)
		return false;
	//�ǵ��������
	CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();//�����
	if(pHand)
	{
		pHand->SetMainPlayerHandBySkill(sourceDC->getID());//������������Ϊ����
	}
	return true;
}

void SetInitializedMainBarUIProperty(CEGUI::Window* pageWnd)
{
	if(!pageWnd)
		return;
	CPlayer* player = GetGame()->GetMainPlayer();
	//1~= ������ĳ�ʼ�� (ͼ����ʾ��
	for(int i = 0 ; i < SHORTCUT_COUNT ; ++i)
	{
		uint hotkey = player->GetHotKey(i + SHORTCUT_COUNT);
		if(hotkey!=0)//�ж�Ӧ�ȼ�
		{
			// �����Ʒ
			if ( ( hotkey & 0x80000000)==0 )
			{
				continue;
			}
			else//����
			{
				uint skillID = hotkey & 0x7fffffff;
				CPlayer::tagSkill* skill = player->GetSkill(skillID);
				if(!skill)
					continue;
				//�ж�ָ���ļ�����ְҵ���Ƿ����
				if( CSkillListXml::OccuJudgeStudy(skillID,player->GetOccupation()) == false)
				{
					//MessageBox(g_hWnd,"��ǰ���ܲ������ְҵ�£����ܲ���ʹ��","Error",MB_OK);
					char strTemp[256];
					sprintf(strTemp,AppFrame::GetText("SDCC_65"),skillID);
					GetInst(MsgEventManager).PushEvent(Msg_Ok,strTemp);
				}
				//����ͼ����ʾ
				char dcWndName[256];
				sprintf(dcWndName,"MainBarPage/StaticImage%d/DragContainer",i);
				CEGUI::Window* dc = pageWnd->getChildRecursive(dcWndName);
				SetDCIconBySkillID(skillID,dc);
			}
		}
	}
}

void UpdateHotKeyCoolDownEffect(long lSkillID)
{
   CPlayer::tagSkill *ptagS = GetGame()->GetMainPlayer()->GetSkill(lSkillID);
   CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
   static char uiName[128] = "";
   DWORD dwReTime = 0;
   int   iHotPos = -1;
   for (int i = 0; i< 12; i++)
   {
       //�������
       DWORD dwHotKey = pMainPlayer->GetHotKey(i+12);
       
       if(dwHotKey != 0)
       {
           // �����Ʒ
           if ( (dwHotKey & 0x80000000)==0 )
           {
               // ������Ʒ��ȴ����ͼ��
               dwReTime = pMainPlayer->GetGoodsRestoreTime((WORD)dwHotKey);
               iHotPos = i;
               break;
           }
           else
           {
               DWORD wSkillID = dwHotKey&0x7fffffff;
               if(wSkillID == lSkillID)
               {
                   iHotPos  = i;
                   dwReTime = ptagS->dwRestoreTime;
                   break;
               }
           }
       }
   }
   if (dwReTime != 0 && iHotPos != -1)
   {
       sprintf_s<128>(uiName,"MainBarPage/StaticImage%d/CoolImage%d",iHotPos,iHotPos);
       CEGUI::Window *pCoolUI = CEGUI::WindowManager::getSingleton().getWindow(uiName);
       UIManager::GetAnimIcon(pCoolUI)->BeginAnimateEffect(dwReTime);
   }
}