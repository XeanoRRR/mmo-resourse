#include "stdafx.h"
#include "SelectRoleEvent.h"
#include "SelectRolePage.h"
#include "../../../GameClient/Game.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../../GameClient/ClientApp/Other/AudioList.h"
#include "../../../ui/UIExtendComponent/3DIconEffect.h"
#include "../../../ui/UIDef.h"
/////////////////////////////////////////////////
// zhaohang  2010/7/13 
// 
#include "../../../ui/UIManager.h"
#include "../../../ui/Windows/CreateRoleDef.h"
////////////////////////////////////////////////

vector<CPlayer*>    SREvent::m_vecPlayer;
ulong               SREvent::m_SelectIndex = 0;

void SelectRolePageDeleRol(MsgEvent& event,eMsgResult UserID)
{
    if(UserID == 1)
    {
        SelectRolePage& srl = GetInst(SelectRolePage);
        GetGame()->C2L_DeleteRole_Send(srl.GetSelPlayer()->GetExID());
    }
}

void SelectRolePageRestorRol(MsgEvent& event,eMsgResult UserID)
{
    if(UserID == 1)
    {
        SelectRolePage& srl = GetInst(SelectRolePage);
        GetGame()->C2L_RestoreRole_Send(srl.GetSelPlayer()->GetExID());
    }
}
//���½�ɫ��ɾ��ʣ������
bool UpdateDelRemainTime(const CEGUI::EventArgs& e);
SREvent::SREvent()
{

}

SREvent::SREvent(EVENTTYPE type)
{
    SetEventType(type);
}

void SREvent::OnPageLoad(GamePage *pPage)
{
    pPage->LoadPageWindow();

    // ��ȡ��ҵĳ�ʼװ��
    m_listOrginEquip.clear();
    CRFile * prfile = rfOpen("data/PlayerOrginEquip.ini");
    if( prfile )
    {
        stringstream streamOE;
        prfile->ReadToStream(streamOE);
        rfClose(prfile);

        tagOrginEquip stOE;
        while (ReadTo(streamOE, "*"))
        {
            int tmp=0;
            streamOE >> tmp >> stOE.wPlacePos >> stOE.strOrginName;
            stOE.lOccupation = tmp;
            m_listOrginEquip.push_back(stOE);
        }
    }

    CEGUI::Window *pPageWin = pPage->GetPageWindow();
    CEGUI::Window *pRoleInfoWin = pPageWin->getChild("RoleInfo");

    CEGUI::PushButton *pCreateBtn = static_cast<CEGUI::PushButton*>(pRoleInfoWin->getChild("CreateRoleBtn"));
    pCreateBtn->setText(ToCEGUIString(AppFrame::GetText("SelctRole_3")));
    pCreateBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SREvent::OnCreateRoleBtn, this));

	CEGUI::PushButton* pDeleteBtn = static_cast<CEGUI::PushButton*>(pPageWin->getChild("DeleteRole"));
    pDeleteBtn->setText(ToCEGUIString(AppFrame::GetText("SelctRole_1")));
	pDeleteBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SREvent::OnDeleteRoleBtn,this));

    CEGUI::PushButton *pBackLoginBtn = static_cast<CEGUI::PushButton*>(pPageWin->getChild("backToLogin"));
    pBackLoginBtn->setText(ToCEGUIString(AppFrame::GetText("SelctRole_2")));
    pBackLoginBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SREvent::GoBackBtn, this));

    CEGUI::PushButton *pEnterGameBtn = static_cast<CEGUI::PushButton*>(pPageWin->getChild("EnterGS"));
    pEnterGameBtn->setText(ToCEGUIString(AppFrame::GetText("Login_1")));
    pEnterGameBtn->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SREvent::OnEnterGameBtn, this));

    CEGUI::Window *LeftRotBtn = pPageWin->getChild("left");
    LeftRotBtn->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&SREvent::TurnLeftBtn, this));
    LeftRotBtn->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&SREvent::StopRotation, this));

    CEGUI::Window *RightRotBtn = pPageWin->getChild("right");
    RightRotBtn->subscribeEvent(CEGUI::PushButton::EventMouseButtonDown,CEGUI::Event::Subscriber(&SREvent::TurnRightBtn, this));
    RightRotBtn->subscribeEvent(CEGUI::PushButton::EventMouseButtonUp,CEGUI::Event::Subscriber(&SREvent::StopRotation, this));

	pPageWin->subscribeEvent("UpdateDelRemainTime",CEGUI::Event::Subscriber(&SREvent::UpdateDelRemainDays,this));
	CEGUI::Window* DelRemainTime = pPageWin->getChild("DelRemainTime");
	DelRemainTime->setVisible(false);
	DelRemainTime->subscribeEvent("UpdateDelRemainTime",CEGUI::Event::Subscriber(&SREvent::UpdateDelRemainDays,this));
}

void SREvent::OnPageOpen(GamePage *pPage)
{
   
}

void SREvent::OnPageClose(GamePage *pPage)
{

}

void SREvent::OnPageUpdate(GamePage *pPage)
{
    
}

void SREvent::OnPageRelease(GamePage *pPage)
{
    ClearPlayer();
}

//��������ɫ��ť
bool SREvent::OnCreateRoleBtn(const CEGUI::EventArgs &e)
{
	/////////////////////////////////////////////////
	// zhaohang  2010/7/13 
	// /������ɫʱ���������ͷţ� ʹ�ý����ض�
	UIManager& mgr = GetInst(UIManager);
	mgr.ReleasePage(CREATEROLE_PAGE_NAME);
	////////////////////////////////////////////////	
    GetGame()->SetGameState(GS_CREATE);
    return true;
}

//ɾ����ɫ��ť����
bool SREvent::OnDeleteRoleBtn(const CEGUI::EventArgs &e)
{
    GetInst(MsgEventManager).PushEvent(Msg_YesNo,"ȷ��ɾ����ɫ?",SelectRolePageDeleRol);
    return true;
}

//������ϷGameserver
bool SREvent::OnEnterGameBtn(const CEGUI::EventArgs &e)
{
    GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-36.wav"); 
    // �Ƿ񴴽��˽�ɫ
    if (m_vecPlayer.size() <1)
    {
		GetInst(MsgEventManager).PushEvent(Msg_Ok,
            AppFrame::GetText("Base_32"));  //���ȴ���һ����ɫ��
        return false;
    }

    
    //�ж��Ƿ��Ѿ�ɾ��
    if (m_vecPlayer[0]->GetPlayerRemainDays() != -1)
    {
		GetInst(MsgEventManager).PushEvent(Msg_YesNo,
			AppFrame::GetText("Base_33"),SelectRolePageRestorRol);//��ɫ�Ѿ���ɾ����Ҫ�ָ��ý�ɫ��?
    }
    else
    {
        g_LoginTime = timeGetTime() + 20000;
        GetGame()->C2L_QuestDetail_Send(m_vecPlayer[m_SelectIndex]->GetExID());
    }
    return true;
}

//�ص���ҳ����
bool SREvent::GoBackBtn(const CEGUI::EventArgs &e)
{
    ClearPlayer();
    GetGame()->CloseNetClient();
    GetGame()->SetGameState(GS_LOGIN);
    return true;
}

//ģ������ת
bool SREvent::TurnLeftBtn(const CEGUI::EventArgs &e)
{
    GetInst(SelectRolePage).m_bRoleLeftRotate = true;
    GetInst(SelectRolePage).m_bRoleRightRotate = false;
    return true;
}

//ģ������ת
bool SREvent::TurnRightBtn(const CEGUI::EventArgs &e)
{
    GetInst(SelectRolePage).m_bRoleLeftRotate = false;
    GetInst(SelectRolePage).m_bRoleRightRotate = true;
    return true;
}

bool SREvent::StopRotation(const CEGUI::EventArgs &e)
{
    GetInst(SelectRolePage).m_bRoleLeftRotate = false;
    GetInst(SelectRolePage).m_bRoleRightRotate = false;
    return true;
}
void SREvent::AddPlayer(CPlayer *pPlayer)
{
    if(pPlayer)
        m_vecPlayer.push_back(pPlayer);
}

void SREvent::ClearPlayer()
{
    if(m_vecPlayer.size()!=0)
    {
        vector<CPlayer*>::iterator it = m_vecPlayer.begin();
        for(;it!=m_vecPlayer.end();it++)
        {
            CPlayer *pPlayer = *it;
            SAFE_DELETE (pPlayer);
        }
        m_vecPlayer.clear();
    }
}

CPlayer *SREvent::GetPlayer(ulong index)
{
    if(index >= 0 &&index < m_vecPlayer.size())
        return m_vecPlayer[index];
    else
        return NULL;
}

CPlayer *SREvent::GetSelPlayer()
{
	if(m_vecPlayer.size() <=0 || m_SelectIndex >= m_vecPlayer.size())
		return NULL;
	else
		return m_vecPlayer[m_SelectIndex];
}

bool SREvent::UpdateDelRemainDays(const CEGUI::EventArgs& e)
{
	CEGUI::WindowManager& mgr = GetWndMgr();
	CEGUI::Window* wnd = mgr.getWindow("DelRemainTime");
	if(!wnd)
		return false;
	CPlayer* player = GetSelPlayer();
	if(!player)
	{
		wnd->setVisible(false);
		return false;
	}
	char nRemainDays = player->GetPlayerRemainDays();
	if(nRemainDays != -1)
	{
		char temp[256];
		sprintf(temp,"����%d�챻ɾ��",nRemainDays);
		wnd->setText(temp);
		wnd->setVisible(true);
	}
	else
	{
		wnd->setVisible(false);
	}
	return true;
}