#include "StdAfx.h"
#include ".\GameControl.h"
#include "game.h"
#include "../AppClient/Player.h"
#include "../AppClient/Monster.h"
#include "../AppClient/clientregion.h"
#include "../AppClient/MiniMap.h"
#include "../appclient/RegionCamera.h"
#include "../AppClient/Effect.h"
#include "..\guiextend\CGuiEx.h"
#include "..\nets\netclient\message.h"
#include "..\public\Crc32Static.h"

#include "../AppClient/Organizingsystem/OrganizingCtrl.h"
#include "../AppClient/Organizingsystem/Faction.h"
#include "../AppClient/Organizingsystem/NoOrganizing.h"
#include "../AppClient/Organizingsystem/Organizing.h"
#include "../AppClient/Organizingsystem/Union.h"
#include "../public/SkillRelated.h"
#include "../appclient/other/FindPath.h"
#include "../AppClient/Other/DeputyOccuSystemClient.h"
#include "../AppClient/PetSystem/CPet.h"
#include "../appclient/Other/TestMsgType.h"
#include "..\AppClient\other\globesetup.h"
#include "..\GUIExtend\MainBarPageEx.h"
#include "..\GUIExtend\ObjectPageEx.h"
#include "..\GUIExtend\ItemPageEx.h"
#include "..\GUIExtend\DepotPageEx.h"
#include "..\appclient\Other\RegionList.h"
#include "..\GUIExtend\NPCShopPageEx.h"
#include "..\GUIExtend\PlayerShopPageEx.h"
#include "..\GUIExtend\PlayerTradePageEx.h"
#include "..\GUIExtend\TreasureShopPageEx.h"
#include "..\GUIExtend\PkPageEx.h"
#include "..\GUIExtend\LuviniaPageEx.h"
#include "..\GUIExtend\RecruitBoxPageEx.h"
#include "..\GUIExtend\MailPageEx.h"
#include "..\GUIExtend\PresentPageEx.h"
#include "..\GUIExtend\HairBeautyPageEx.h"
#include "..\GUIExtend\AutoPageEx.h"
#include "../../../PUBLIC/GoodsDef.h"
#include "../../../PUBLIC/FunctionKeyMapSystem.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//һ���ƶ�����������
#define MAX_MOVE_CELLNUM	2
bool CGameControl::m_bRButtonDownValid;
CGameControl::CGameControl(void)
{
	m_dwMoveMsgCounter = 0;
	m_fParameterOfDisFromCamara = 1.125f;
	m_pAAHook = new CAutoAttackHook();
	m_bStartAttack = false;
	m_bAutoHookProtected = false;
	ReNew();
}

CGameControl::~CGameControl(void)
{
	delete m_pAAHook;
}

void CGameControl::ReNew()
{
	m_bRButtonDownValid = true;
	pRegion = NULL;
	pMainPlayer = NULL;
	m_bIsInTryOnState = false;
	m_bControlAble = true;
	m_bMoveAble = true;
	m_bIntonateBreak = false;
	m_lSendedMoveMsgCount = 0;
	m_bChangingMoveMode = 0;
	m_lSendedUseSkillMsgCount = 0;
	m_lLastControlTimer = 0;
	m_dwLeftHoldingTime = 0;
	m_dwRightHoldingTime = 0;
	m_dwSelectedShapeStartTime = 0;
	m_pMouseShapeGoods = NULL;				// ���ָ�����Ʒ
	m_pMouseShape = NULL;					// ���ָ��Ķ���
	m_pSelectedShape = NULL;				// ��ǰѡ�еĶ���
	m_pCurAttractShape = NULL;				// ��ǰ���ڱ������Ķ���
	m_pPressRBtnShape = NULL;				// ��ǰ�����Ҽ�ָ��Ķ���

	m_lSelectedType = 0;
	m_guidSelected = CGUID::GUID_INVALID;

	m_bIsTabTeamMate = false;

	m_lFollowingObjType = 0;
	m_FollowingObjID = CGUID::GUID_INVALID;
	m_PickGoodsID = NULL_GUID;
	m_lFollowMaxDistance = 1;
	m_lStartDestX = 0;
	m_lStartDestY = 0;
	m_fDestX = 0.0f;
	m_fDestY = 0.0f;
	m_MainPlayerActionMode = MPAM_No;
	m_AutoMoveMode = AMM_NO;
	m_bResetAutoMoveMode = false;
	m_AutoAttactMode = AAM_NO;
	m_bHotKeyMove = false;
	m_bCurKeyPress = false;

	m_NextMainPlayerActionMode = MPAM_No;

	m_dwBufferSkillID = 0;
	m_dwAutoSkillID = 0;
	m_nLastPressHotKey = 0;
	m_dwCurSkillID = 0;
	m_lSkillGoodsExtendID = 0;
	m_lSkillGoodsPos = -1;
	m_lTargetRegionID = 0;
	m_lSPosX = 0;
	m_lSPosY = 0;
	m_lTPosX = 0;
	m_lTPosY = 0;

	m_dwLastPingTime = timeGetTime();
	m_dwLocalIP =  0;

	SetUpdateXY(0,0);
}

// ����
void CGameControl::Run()
{
	pRegion = GetGame()->GetRegion();
	pMainPlayer = GetGame()->GetMainPlayer();

	if (pRegion == NULL || pMainPlayer == NULL)
		return;

	//PingServer();

	if(pMainPlayer->IsDied())
	{
		m_AutoMoveMode = AMM_NO;
		m_pAAHook->DeathWarnPlayMusic();
	}
	// ˢ�¼�����ȴʱ��
	pMainPlayer->UpdateSkillCoolDownTime(false);
	if(GetGame()->GetCGuiEx()->GetMainBarPageEx())
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdataHotCompCoolEffect();
	// ���һ��ʱ��û���յ����������صĲ�����Ӧ����ô�ж�Ϊ����ʧ��
	if (IsControlAble()==false && GetLastControlTimer() + 500 < (long)GetCurTickCount() )
	{
		SetControlAble(true);
	}
	// �ӽǿ���
	pRegion->GetRegionCamera()->Run();
	
	// ���ܽ����κβ���
	if(IsEveryThingNotToDo())
	{
		return;
	}
	// ��־: �ڹ� 11-02-16

	if (GetAttackHooKStart())
	{
		m_pAAHook->run();	
	}           
	m_pAAHook->UpdateAutoHookBuff();
	
	

	//////////////////////////////////////////////////////////////////////////
	//	��������
	//////////////////////////////////////////////////////////////////////////
	OnMouseEvent();					// ��괦��
	// ��ʾ��ǰѡ��������Ϣ
	CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	if(pObjPage && m_pSelectedShape)
	{
		if(pObjPage->GetObjShape() != m_pSelectedShape)
		{
			pObjPage->Release();
			pObjPage->SetObjShape(m_pSelectedShape);
			pObjPage->Open();
		}
	}
	OnAnotherEvent();				// ���������Զ�����ȣ�
	OnKeyboardEvent();				// ���̴���	
	// ��ʼ��¼ѡ��Ŀ��
	if(m_dwSelectedShapeStartTime > 0)
	{
		DWORD time = timeGetTime() - m_dwSelectedShapeStartTime;
		if(time > 1000)
		{
			// �����������ѡ��Ŀ��
			CShape *pSelect = dynamic_cast<CShape*>(pRegion->FindChildObject(m_lSelectedType, m_guidSelected));
			if(NULL == pSelect)
			{
				m_lSelectedType = 0;
				m_guidSelected = CGUID::GUID_INVALID;
			}
			// ����������͵�ǰѧ��Ķ���
			CMessage msg(MSG_C2S_SHAPE_SETSELECTEDSHAPE);
			msg.Add(m_lSelectedType);
			msg.Add(m_guidSelected);
			msg.Send();
			m_dwSelectedShapeStartTime = 0;
		}
	}
	CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
	if (pItemPage!=NULL)
	{
		pItemPage->UpdataGoodsCoolEffect();
	}

	/// �����ڱ�������״̬ʱ�ͼ���ִ����ȥ������ֱ��������ϡ���
	static DWORD dwStarTime = 0;
	DWORD dwTime = GetCurTickCount() - dwStarTime;
	if (dwTime > CGlobeSetup::GetSetup()->dwSendFinishBagDelayTime && pItemPage!=NULL &&pItemPage->GetFinishBagState())
	{
		if (pItemPage->CanFinishBag()&&pItemPage->GoodsSort())
		{
			dwStarTime = GetCurTickCount();
			pItemPage->SendMsgForFinishBag();
		}
	}

	// ���壺�ֿ�����
	if( CDepotPageEx* pDepotPageEx = GetGame()->GetCGuiEx()->GetDepotPageEx() )
	{
		pDepotPageEx->SendSortMsg();
	}
	

		
}
void CGameControl::SetAttackHookStart(bool itStart,int initType)
{ 
	
	if(itStart)
	{
		m_pAAHook->InitAutoHook();
	}
	else
	{
		GetGame()->GetCGuiEx()->GetAutoPageEx()->CloseAutoHookButton();

	}
	m_bStartAttack=itStart; 
};

// ��괦��
long CGameControl::OnMouseEvent()
{
	// ��ģ̬�Ի��򵯳�
	IGUIRoot *pRoot = IGUIRoot::GetGUIInstance();
	if(pRoot && pRoot->GetGUIComp()->GetLock())
		return 0;
	switch(GetMouse()->GetCurrentButton())
	{
	case LB_DOWN:
		m_dwLeftHoldingTime = GetCurTickCount();
		OnLeftButtonDown();
		return 1;
	case LB_UP:
		m_dwLeftHoldingTime = 0;
		return 1;
	case LB_DCLICK:
		m_dwLeftHoldingTime = 0;
		OnLeftDoubleButtonDown();
		return 1;
	case RB_DOWN:
		m_dwRightHoldingTime = 0;
		OnRightButtonDown();
		return 1;
	case RB_UP:
		m_dwRightHoldingTime = 0;
		OnRightButtonUp();
		return 1;
	case RB_DCLICK:
		m_dwRightHoldingTime = 0;
		return 1;
	default:
		break;
	}
	// ��������ס
	if (GetMouse()->IsLeftButtonDown())
	{
		OnLeftButtonHolding();
	}
	return 1;
}

// ���̴���
long CGameControl::OnKeyboardEvent()
{
	if(GetGame()->GetIGWFocus())    //���������IGW�ϣ�����Ӧ��ز�����
		return 0;

	// ��ģ̬�Ի��򵯳�
	IGUIRoot *pRoot = IGUIRoot::GetGUIInstance();
	if(pRoot && pRoot->GetGUIComp()->GetLock())
		return 0;

	MAPFKEY& mapkey = GetGame()->GetMainPlayer()->GetCurMapKey();

	// ��ݼ�����Ʒ
	if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_OTHERHOTKEY_PICKGOODS)/*GetKeyboard()->GetCurrentKey() == DIK_SPACE*/)
	{
		if (IsCanPickupGoodsCondition() && !pRoot->GetFocusComp())
		{
			bool bIsPic = false;
			for (int i=0; i<9; i++)
			{
				CGoods *pGoods = pRegion->GetShapeGoods(pMainPlayer->GetTileX() + _Pick_Order[i][0], pMainPlayer->GetTileY() + _Pick_Order[i][1]);
				if (pGoods)
				{
					AutoPickGood(pGoods->GetExID());
					bIsPic = true;
					break;
				}
			}
			// ���ڽ�����û�п�ʰȡ����Ʒ������һ��ķ�Χ������Ʒ
			if(!bIsPic)
				AutoPickGoodEx();
		}
	}
	if((GetKeyboard()->IsKeyDown(DIK_LALT)||GetKeyboard()->IsKeyDown(DIK_RALT)) 
		&& GetKeyboard()->IsKeyDown(DIK_Z))
	{//������ͨ����
		int index=GetGame()->GetCGuiEx()->GetAutoPageEx()->CheckAutoHookToRun();
		if (index)
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(eIDS_SPRITE,index),D3DCOLOR_ARGB(255,255,0,0));
		}
		else
		{
			if(GetGame()->GetGameControl()->GetAutoAttackHook()->GetbStartAttack())//�ж��Ƿ��ڹһ�
				GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_STABLE,false);
			GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_COMMON,true);				
		}	
	}
	else if((GetKeyboard()->IsKeyDown(DIK_LALT)||GetKeyboard()->IsKeyDown(DIK_RALT))
		&& GetKeyboard()->IsKeyDown(DIK_S))
	{//ȡ���һ�
		if (GetGame()->GetGameControl()->GetAutoAttackHook()->GetbStartAttack())
		{
			GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_STABLE,false);
			GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_COMMON,false);
		}		
	}
	else if((GetKeyboard()->IsKeyDown(DIK_LALT)||GetKeyboard()->IsKeyDown(DIK_RALT))
		&& GetKeyboard()->IsKeyDown(DIK_X))
	{//�����ȶ�����
		int index=GetGame()->GetCGuiEx()->GetAutoPageEx()->CheckAutoHookToRun();
		if (index)
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(eIDS_SPRITE,index),D3DCOLOR_ARGB(255,255,0,0));
		}
		else
		{
			if(GetGame()->GetGameControl()->GetAutoAttackHook()->GetbStartAttack())//�ж��Ƿ��ڹһ�
				GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_COMMON,false);
			GetGame()->GetGameControl()->GetAutoAttackHook()->SendAutoHookMsg(eAHT_STABLE,true);				
		}
	}
	//�ܡ����߷�ʽ�л������������������
	if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_RUNORWALK)/*GetKeyboard()->GetCurrentKey() == DIK_R && GetKeyboard()->IsKeyDown(DIK_LCONTROL)*/)
	{
		if(!pRoot->GetFocusComp())
		{
			GetKeyboard()->SetCurrentKey(0);
			if(GetMPActionMode() != MPAM_No)
			{
				//ֹͣ�Զ��������Զ��ƶ�
				GetGame()->GetGameControl()->AutoAttactStop();
				GetGame()->GetGameControl()->AutoMoveStop();
				SetChangingMoveMode(true);
			}
			else
			{
				ChangeMoveMode();
			}
		}
	}
	// ��TAB���ѡ��
	if(GetActiveWindow() == g_hWnd && pRegion && pMainPlayer && CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_OTHERHOTKEY_CHANGETARGET)/*GetKeyboard()->GetCurrentKey() == DIK_TAB*/)
	{
		if (!pRoot->GetFocusComp())
		{
			bool bEFlag = GetGame()->GetTabSet()->bEnemyPlayer;			// �й����
			bool bPFlag = GetGame()->GetTabSet()->bPiratePlayer;			// �������
			bool bMFlag = GetGame()->GetTabSet()->bMonster;				// ����
			bool bNTFlag = GetGame()->GetTabSet()->bNotTeamPlayer;				// �Ǳ������
			if(!bMFlag && !bEFlag && !bPFlag && !bNTFlag && !m_bIsTabTeamMate)		// Tab���ܹر�
				return 1;

			long x = pMainPlayer->GetTileX();
			long y = pMainPlayer->GetTileY();
			long lDir = pMainPlayer->GetDir();
			long lType = -1;
			long lOffLen = 1;
			CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
			if(pSetup)
				lOffLen = pSetup->lAreaByTab;
			// �ж���������
			if(((bEFlag || bPFlag) && !bMFlag) || m_bIsTabTeamMate || bNTFlag )
				lType = TYPE_PLAYER;
			else if(bMFlag && !bEFlag && !bPFlag && !m_bIsTabTeamMate)
				lType = TYPE_MONSTER;
			// ����������������
			CShape *pShape = pRegion->GetLeastShape(m_lsSelectedShape,x,y,lDir,lOffLen,lType);
			if(pShape)
			{
				SetSelectedShape(pShape);
				// �رղ˵�
				CPopMenuPageEx *pMenu = GetGame()->GetCGuiEx()->GetCPopMenuPageEx();
				if(pMenu && pMenu->IsOpen())
					pMenu->Close();
			}
		}
	}
	// ��'F'���ѡ��ǰѡ�ж����ѡ�����
	if (m_pSelectedShape && CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_OTHERHOTKEY_AFFIRMTARGET)/*GetKeyboard()->GetCurrentKey() == DIK_F*/)
	{
		// ��������������󣬻�õ�ǰѡ������ѡ�����
		CMessage msg(MSG_C2S_SHAPE_GETTSELECTEDSHAPE);
		msg.Send();
	}

	// ��������ݼ�
	for(int i = PET_TYPE_ELF; i<= PET_TYPE_CONJURED; ++i)
	{
		CPet *pPet = GetGame()->GetMainPlayer()->GetActivePet(i);
		if(pPet)
			pPet->ProcessPetHotKey();
	}

	// ��ݼ�����
	ProcessHotKey();
	return 0;
}


// ���������Զ�����ȣ�
long CGameControl::OnAnotherEvent()
{
	//�����ǰû����Ϊģʽ,��ȡ���������Ϊģʽִ��
	if( GetMPActionMode() == MPAM_No)
		OnNextPlayerActionMode();
	switch(GetMPActionMode())
	{
	case MPAM_AutoMove:
		{
			OnMainPlayerMove();
		}
		break;
	case MPAM_AutoAttack:
		{
			OnMainPlayerAttack();
		}
		break;
	case MPAM_AutoPickGoods:
		{
			OnAutoPickupGoods();
		}
		break;
	}

	return 1;
}

long CGameControl::OnAutoPickupGoods()
{
	CGoods* pGood = dynamic_cast<CGoods*>(pRegion->FindChildObject(TYPE_GOODS, m_PickGoodsID));
	if(pGood)
		PickupGoods(pGood);
	m_PickGoodsID = NULL_GUID;
	SetMPActionMode(MPAM_No);
	return 1;
}

// �Ƿ������κβ���
bool CGameControl::IsEveryThingNotToDo()
{
	// �����������쳣״̬���������κβ���������ֹ�����Զ����Ʋ���
	if(pMainPlayer->IsDied() || !pMainPlayer->IsInUnControlState())
	{
		SetFollowingObj(0,CGUID::GUID_INVALID);
		SetBufferSkillID(0);		// ȡ�����弼��
		SetAutoSkillID(0);			// ȡ����������
		return true;
	}
	return false;
}

// �Ƿ���Բ�����״̬
bool CGameControl::IsCanControlCondition()
{
	if (
		pMainPlayer->IsPlayerShopOpened()									// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsOpen()			// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetPresentPageEx()->IsOpen()				// �콱����
		)
	{
		return false;
	}

	// ���ɲ�����״̬
	if (IsControlAble() == false)
	{
		return false;
	}

	// �����쳣״̬
	if (pMainPlayer->IsInUnControlState() == false)
	{
		return false;
	}
	if(pMainPlayer->GetAction()==CShape::ACT_MEGRIM)
	{
		return false;
	}

	return true;
}

// �Ƿ�������ߵ�״̬
bool CGameControl::IsCanMoveCondition()
{
	// �����޷����Ƶ�״̬
	if (
		pMainPlayer->IsPlayerShopOpened()								// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsOpen()			// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetTreasureShopPageEx()->IsOpen()		// ��ֵ��Ʒ������
		|| GetGame()->GetCGuiEx()->GetPresentPageEx()->IsOpen()				// �콱����
		)
	{
		SetMPActionMode(MPAM_No);
		SetAutoMoveMode(AMM_NO);
		return false;
	}

	// ���ɲ�����״̬,�־��쳣״̬
	if (!IsControlAble() || !pMainPlayer->IsInDreadState())
	{
		return false;
	}

	return true;
}

// �Ƿ���Լ�����Ʒ��״̬
bool CGameControl::IsCanPickupGoodsCondition()
{
	// �����쳣״̬
	if (GetGame()->GetMainPlayer()->GetIntonateKey()!=0 ||
		pMainPlayer->IsInUnControlState() == false||pMainPlayer->GetAction()==CShape::ACT_PICK)
	{
		return false;
	}
	return true;
}

// �Ƿ����ʹ����Ʒ��״̬
bool CGameControl::IsCanUseGoodsCondition(DWORD dwGoodsIndex)
{
	if (
		GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsOpen()&&
		GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsItemReadyToTrade(dwGoodsIndex)&&
		GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->GetCurShopState()==CPlayerShopPageEx::SET_SHOP	// �����̵����
		)
	{
		return false;
	}

	// �����쳣״̬
	if (!pMainPlayer->IsInUnControlState())
	{
		return false;
	}

	// ����ʹ����Ʒ
	if (!pMainPlayer->IsCanUseItem())
	{
		return false;
	}

	return true;
}

// �Ƿ����ʹ�����������ܵ�״̬
bool CGameControl::IsCanUsePhysicsSkillCondition()
{
	if (GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsOpen()			// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetNPCShopPageEx()->IsOpen()			// ��NPC�̵����
		|| GetGame()->GetCGuiEx()->GetPlayerTradePageEx()->IsOpen()		// �򿪽��׽���
		)
	{
		return false;
	}

	// ���ɲ�����״̬
	if (!IsControlAble())
	{
		return false;
	}

	// �����쳣״̬
	if (!pMainPlayer->IsInUnControlState())
	{
		return false;
	}

	// ����ʹ��������
	if (!pMainPlayer->IsCanUseFighterSkill())
	{
		return false;
	}

	return true;
}

// �Ƿ����ʹ��ħ���������ܵ�״̬
bool CGameControl::IsCanUseMagicSkillCondition()
{
	if (GetGame()->GetCGuiEx()->GetPlayerShopPageEx()->IsOpen()			// �򿪿������̵�
		|| GetGame()->GetCGuiEx()->GetNPCShopPageEx()->IsOpen()			// ��NPC�̵����
		|| GetGame()->GetCGuiEx()->GetPlayerTradePageEx()->IsOpen()		// �򿪽��׽���
		)
	{
		return false;
	}

	// ���ɲ�����״̬
	if (!IsControlAble())
	{
		return false;
	}

	// �����쳣״̬
	if (!pMainPlayer->IsInUnControlState())
	{
		return false;
	}

	// ����ʹ��ħ��
	if (!pMainPlayer->IsCanUseRabbSkill())
	{
		return false;
	}

	return true;
}

void CGameControl::GetUpdateXY(long &lX,long &lY)
{
	if(m_lUpdateX == 0 && m_lUpdateY == 0 && pMainPlayer)
	{
		lX = pMainPlayer->GetTileX();
		lY = pMainPlayer->GetTileY();
		return;
	}	
	lX = m_lUpdateX;
	lY = m_lUpdateY;
}



/*
* ����: �жϵ�ǰ�����Ƿ�����ƶ�
* ժҪ: -
* ����: -
* ����ֵ: �Ƿ�����ƶ�
* ����: -
* ��������: -
* �޸���־:
*	2008.11.11 - lpf
*		��������̬���������ƶ����ж�����
*/
bool CGameControl::IsMoveWithCurAction()
{
	if (pMainPlayer->GetAction() == CShape::ACT_STAND || 
		pMainPlayer->GetAction() == CShape::ACT_IDLE || 
		pMainPlayer->GetAction() == CShape::ACT_BEATTACK ||
		pMainPlayer->GetAction() == CShape::ACT_BLOCK ||
		pMainPlayer->GetAction() == CShape::ACT_PARRY ||
		pMainPlayer->GetAction() == CShape::ACT_MISS || 
		pMainPlayer->GetAction() == CShape::ACT_INTONATE ||
		(pMainPlayer->GetAction() >= CShape::ACT_FACEACTION && pMainPlayer->GetAction() <= CShape::ACT_FACEACTIONMAX) ||               //��̬�����ƶ�����
		pMainPlayer->GetIntonateKey())
	{
		return true;
	}
	return false;
}

// �жϵ�ǰ�����Ƿ���Թ���
bool CGameControl::IsActWithCurAction()
{
	WORD wAct = pMainPlayer->GetAction();
	if (IsMoveWithCurAction())
	{
		return true;
	}
	// ����ʩ������״̬�������ʩ�������Ĺ涨ʱ��ﵽ�����Թ���
	if(wAct >= CShape::ACT_PERFORM && wAct <= CShape::ACT_MAX_PERFORM)
	{
		// �涨�����ﵽ
		if(GetGame()->GetMainPlayer()->IsCurSkillActTimeFinished())
			return true;
	}
	return false;
}

// ����ƶ����ĸ���
//����ֵ:false:��ʾ�Ѿ�����,true:�����ڸø���
bool	CGameControl::AddMovedCell(long lTileX,long lTileY)
{
	static long lLastAddX = 0;
	static long lLastAddY = 0;
	if(lLastAddX == lTileX && lLastAddY == lTileY)
	{
		return true;
	}
	lLastAddX = lTileX;lLastAddY=lTileY;
	POINT point = {lTileX,lTileY};
	itMovedCell it = m_MovedCell.find(point);
	if(it != m_MovedCell.end())
	{
		if( (*it).second >= 2)
			return false;
		(*it).second++;
	}
	else
	{
		m_MovedCell[point]=1;
	}
	return true;
}

bool CGameControl::IsResetAutoMoveMode()
{
	if(m_bResetAutoMoveMode)
	{
		long lTileX = pMainPlayer->GetTileX();
		long lTileY = pMainPlayer->GetTileY();
		if( pRegion->GetBlock(lTileX, lTileY) != CRegion::BLOCK_NO && 
			pRegion->GetBlock(lTileX, lTileY) != CRegion::BLOCK_AIM)
		{
			m_bResetAutoMoveMode = false;
		}
	}
	return m_bResetAutoMoveMode;
}
// �����ƶ�����Ŀ��㴦��
long CGameControl::OnMainPlayerMove()
{	
	// 1��ͬ��������߶��������ƶ�
	if ( IsMoveWithCurAction() ||	//�����߶���
		pMainPlayer->IsArrive()  ||  //�����¸�������
		IsResetAutoMoveMode() )//�����ƶ�ģʽ
	{
		long ret = 1;
		//���Ҫֹͣ�ƶ�
		if(!pMainPlayer->IsInUnMoveState() || (GetAutoMoveMode()&AMM_STOP))
		{
			SetAutoMoveMode(AMM_NO);
			ret = 0;
		}

		switch(GetAutoMoveMode())
		{
		case AMM_A_SEARCH:
			{
				ret = AutoMoveAI();
			}
			break;
		case AMM_TO_DESTDIR:
			{
				ret = AutoMoveAIEx();
			}
			break;
		case AMM_FOLLOW_MOUSE:
			{
				ret = FollowMouseMove();
			}
			break;
		case AMM_FOLLOW_OBJECT:
			{
				ret = FollowObjectAI();
			}
			break;
		};

		//������ز���
		SetResetAutoMoveMode(false);

		// ���壺��������ƶ��� �����Ӱ��
		GetGame()->GetCGuiEx()->OnPlayerMoving(
			GetGame()->GetMainPlayer()->GetTileX(),
			GetGame()->GetMainPlayer()->GetTileY() );

		// �ָ���վ��
		if (ret == 0)
		{
			m_MovedCell.clear();
			if (pMainPlayer->GetAction() == CShape::ACT_WALK ||
				pMainPlayer->GetAction() == CShape::ACT_RUN)
			{
				pMainPlayer->SetAction( CShape::ACT_STAND );
				//ֹͣ�ƶ�
				StopMove();
			}
			//���������
			if(GetAutoMoveMode() == AMM_FOLLOW_OBJECT)
				DoFollowResult();
			if(m_lFollowingObjType != TYPE_PLAYER || m_NextMainPlayerActionMode != MPAM_No)
				SetMPActionMode(MPAM_No);
			if(GetChangingMoveMode())
			{
				SetChangingMoveMode(false);
				ChangeMoveMode();
			}
			return 0;
		}
	}
	return 0;
}

// ���ǹ������Զ�����
CGameControl::eAAMRet CGameControl::OnMainPlayerAttack()
{	
	//-1,ֹͣ�Զ�����ģʽ, 0:��ʾ����׷��,1:��ʼ����
	eAAMRet ret = AAMR_Other;

	long lAAkMode = GetAutoAttackMode()&(0xFFF);
	if(lAAkMode ==  AAM_FIND_ATTACTER)
	{
		ret = AAMR_Trace;
		//ͬ����Ϣ�����ж�
		if( (pMainPlayer->IsArrive()  && GetSendedMoveMsgCount() < 1 ) || IsMoveWithCurAction() )
		{
			DWORD dwCurTime = GetCurTickCount();

			// �����ƶ�
			if(!pMainPlayer->IsInUnMoveState() || (GetAutoAttackMode()&AAM_STOP))
				ret = AAMR_Stop;
			else
			{
				ret = AutoAttackMoveAI();
				if(ret == AAMR_Attack)
				{
					SetAutoAttackMode(AAM_ATTACT);
					lAAkMode = AAM_ATTACT;
					if (pMainPlayer->GetAction() == CShape::ACT_WALK ||
						pMainPlayer->GetAction() == CShape::ACT_RUN)
					{
						pMainPlayer->SetAction( CShape::ACT_STAND );
						//ֹͣ�ƶ�
						StopMove();
					}
				}
			}
		}
	}	

	//����ģʽ
	if(lAAkMode == AAM_ATTACT)
	{
		//�����ж�
		if(IsActWithCurAction() )
		{
			DWORD dwCurTime = GetCurTickCount();
			if(GetAutoAttackMode()&AAM_STOP)
				ret = AAMR_Stop;
			else
			{
				ret = AutoLockAttackAI();
				if(ret == AAMR_Trace)
				{
					SetAutoAttackMode(AAM_FIND_ATTACTER);
				}
			}
		}
	}

	//ֹͣ�Զ�ģʽ
	if(ret == AAMR_Stop)
	{
		SetMPActionMode(MPAM_No);
		SetCurAttractShape(NULL);
		if (pMainPlayer->GetAction() == CShape::ACT_WALK ||
			pMainPlayer->GetAction() == CShape::ACT_RUN)
		{
			pMainPlayer->SetAction( CShape::ACT_STAND );
			//ֹͣ�ƶ�
			StopMove();
		}
		//�����Ҫ�ı��ƶ�ģʽ
		if(GetChangingMoveMode())
		{
			SetChangingMoveMode(false);
			ChangeMoveMode();
		}

		DWORD dwCurTime = GetCurTickCount();
	}
	return ret;
}

// ִ�л������Ϊģʽ
long	CGameControl::OnNextPlayerActionMode()
{
	if(m_NextMainPlayerActionMode != MPAM_No)
	{
		m_MainPlayerActionMode = m_NextMainPlayerActionMode;
		m_NextMainPlayerActionMode = MPAM_No;
	}
	return 1;
}

//	��һ������Զ��ƶ�
#ifdef MOVE_IN_TILE
long CGameControl::AutoMoveAIEx()
{
	float fx = pMainPlayer->GetPosX();
	float fy = pMainPlayer->GetPosY();
	long ret = 1;
	//���Ŀ�����Shape,���Һ�Shape֮��ľ���Ϊ1�Ǿ�ֹͣ�ƶ�
	// ����Ŀ�����ֹ꣬ͣ�Զ��ƶ�
	if ( fx==m_fDestX && fy == m_fDestY )
	{
		ret = 0;
	}
	else
	{
		//����Ƿ������߹��ø���
		//��ͼ�������
		if( AddMovedCell(fx,fy))
		{
			if ( pMainPlayer->RealDistance(m_fDestX,m_fDestY)<1.5f )
			{
				MoveToCell(m_fDestX,m_fDestY);
			}
			else if(GetNextMoveDestCell(m_fDestX,m_fDestY,MAX_MOVE_CELLNUM,fx,fy))
			{
				if ((long)fx == (long)m_fDestX && (long)fy == (long)m_fDestY)
				{
					MoveToCell(m_fDestX,m_fDestY);
				}
				else
				{
					MoveToCell(fx,fy);
				}
			}
			else
				ret = 0;
		}
		//����ֹͣ����
		else
			ret = 0;
	}
	return ret;
}
#else
long CGameControl::AutoMoveAIEx()
{
	long lX = pMainPlayer->GetTileX();
	long lY = pMainPlayer->GetTileY();
	long ret = 1;
	//���Ŀ�����Shape,���Һ�Shape֮��ľ���Ϊ1�Ǿ�ֹͣ�ƶ�
	// ����Ŀ�����ֹ꣬ͣ�Զ��ƶ�
	if ( abs(lX-(long)m_fDestX) < 1 && abs(lY-(long)m_fDestY) < 1 )
	{
		ret = 0;
	}
	else
	{
		//����Ƿ������߹��ø���
		//��ͼ�������
		if( AddMovedCell(lX,lY))
		{
			float fDestX = pMainPlayer->GetPosX();
			float fDestY = pMainPlayer->GetPosY();			
			if(GetNextMoveDestCell(m_fDestX,m_fDestY,MAX_MOVE_CELLNUM,fDestX,fDestY))
			{
				MoveToCell(fDestX,fDestY);
			}
			else
				ret = 0;
		}
		//����ֹͣ����
		else
			ret = 0;
	}
	return ret;
}
#endif


//�õ�ĳ���㷽������һ���ƶ�Ŀ���
long  CGameControl::GetNextMoveDestCell(float fX,float fY,long lMaxDistance,float &fDestX,float &fDestY)
{
	float fSX = pMainPlayer->GetPosX();
	float fSY = pMainPlayer->GetPosY();
	float fDir = CShape::GetLineDirF(fSX,fSY,fX,fY);
	float fDistance = pMainPlayer->RealDistance(fX,fY);
	if(0.0f==fDistance)	
		return 0;
	fDistance = min(((float)lMaxDistance),fDistance);
	return GetNextMoveDestCell(fDir,fDistance,fDestX,fDestY);	
}

//�õ�һ�������ϵ���һ���ƶ�Ŀ���
long	CGameControl::GetNextMoveDestCell(float fRadianDir,float fMaxDistance,float &fDestX,float &fDestY)
{
	//������8������Ӽ�ֵ
	static int SlipOrder[8] = {0,1,-1,2,-2,3,-3,4};
	//����ĳ�ʼ������������
	static int SlipRandom[] = {1,-1};

	float fSX = pMainPlayer->GetPosX();
	float fSY = pMainPlayer->GetPosY();

	//�ڳ�ʼ�������ƶ����赲Ϊֹ
	float fCount = 0.0f;
	float fEndX = fSX,fEndY = fSY;
	int nBlockNum = 0;
	while(fCount <= fMaxDistance)
	{
		float fx = fSX + fCount * cosf(fRadianDir);			
		float fy = fSY + fCount * sinf(fRadianDir);
		if( pRegion->GetBlock((long)fx, (long)fy) != CRegion::BLOCK_NO && 
			pRegion->GetBlock((long)fx, (long)fy) != CRegion::BLOCK_AIM)
			nBlockNum++;
		else
		{
			nBlockNum=0;
			fEndX = fx;fEndY=fy;
		}
		//������5��������赲�����ʾ�����ƶ�ͨ��
		if(nBlockNum >= 1)
		{
			break;
		}
		fCount += 0.1f;
	}
	if( nBlockNum < 1 && ( (long)fSX != (long)fEndX || (long)fSY != (long)fEndY ))
	{
		fDestX=fEndX;
		fDestY= fEndY;
		if(pRegion->GetBlock((long)fDestX, (long)fDestY) == CRegion::BLOCK_UNFLY)
		{
			int x = 8;
		}
		return 1;
	}
	//û���ƶ��ɹ������ڲ���һ��Ļ���	
	long nRandom=random(2);
	long lDir = CShape::RadianDirTo8Dir(fRadianDir);
	int i=0;
	long lDestX = (long)fSX;
	long lDextY = (long)fSY;
	for(; i<8; i++)
	{
		long lMoveDir = lDir+SlipOrder[i]*SlipRandom[nRandom];
		if(lMoveDir>7)	lMoveDir = lMoveDir%8;
		else if(lMoveDir<0) lMoveDir = lMoveDir+8;
		lDestX = (long)fSX + _dir[lMoveDir][0];
		lDextY = (long)fSY + _dir[lMoveDir][1];
		if( pRegion->GetBlock(lDestX, lDextY) == CRegion::BLOCK_NO ||
			pRegion->GetBlock(lDestX, lDextY) == CRegion::BLOCK_AIM)
		{
			break;
		}
	}
	if(i < 8)
	{
		fDestX = (float)lDestX+0.5f;
		fDestY = (float)lDextY+0.5f;
		return 1;
	}
	return 0;
}
//	�Զ���������ƶ�
long CGameControl::FollowMouseMove()
{
	//���㷽��
	POINT pt;
	GetCursorPos(&pt);
	float fdir = pRegion->CalcDirection(pMainPlayer->GetPosX(), pMainPlayer->GetPosY(), pMainPlayer->GetHeight(), pt.x, pt.y);

	float fDestX = pMainPlayer->GetPosX();
	float fDestY = pMainPlayer->GetPosY();	
	if( GetNextMoveDestCell(fdir,MAX_MOVE_CELLNUM,fDestX,fDestY) )
	{
		MoveToCell(fDestX,fDestY);
		return 1;
	}
	return 0;
}



/*
* ����: ���������´���
* ժҪ: -
* ����: -
* ����ֵ: ��
* ����: -
* ��������: -
* �޸���־:
*	2008.11.11 - lpf
*		�ڡ������������ʩ������ʱ���ƶ�����������̬���������ƶ����ж�����
*/
long CGameControl::OnLeftButtonDown()
{
	
	CMainPlayerHand* pMainPlayerHand = GetGame()->GetCGuiEx()->GetPMainPlayerHand();
	CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
	CPkPageEx* pPkPage = GetGame()->GetCGuiEx()->GetPkPageEx();
	CLuviniaPageEx *pLuvPage = GetGame()->GetCGuiEx()->GetLuviniaPageEx();
	CMainBarPageEx * pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
	if (pPkPage!=NULL&&pPkPage->IsOpen())
	{
		pPkPage->Close();
	}
	if(pLuvPage!=NULL&&pLuvPage->IsOpen())
	{
		pLuvPage->Close();
	}
	if(pMainBarPage)
		pMainBarPage->CloseTabSetPage();

	//���ܲ���
	if (!IsCanControlCondition())
	{
		pMainPlayerHand->SetMainPlayerHandByEffect(0,0);
		return 0;
	}

	//������ϵļ���ͼ��
	if (pMainPlayerHand)
	{
		if(pMainPlayerHand->GetObjectTypeOfMainPlayerHand()==TYPE_SKILL)
		{
			pMainPlayerHand->ClearMainPlayerHand();
			return 0;
		}
		// �����з�Χ����
		else if(pMainPlayerHand->GetAreaSkillID() > 0)
		{
			DWORD dwID = pMainPlayerHand->GetAreaSkillID();
			DWORD dwLev = 1;
			// ����ķ�Χ����
			CPlayer::tagSkill *pSkill = pMainPlayer->GetSkill(dwID);
			if (pSkill)
				dwLev = pSkill->wLevel;
			// �������û�д˼��ܣ����ֲ�����Ʒ�����ķ�Χ���ܣ�����
			else if (m_lSkillGoodsExtendID <= 0 || m_lSkillGoodsPos < 0)
			{
				pMainPlayerHand->SetMainPlayerHandByEffect(0,0);
				return 0;
			}
			if(GetMPActionMode() == MPAM_AutoMove)
				AutoMoveStop();
			// �ﵽ�ͷ�����
			long x = 0,y = 0;
			CSkillXml skill;
			// �ͷ���������
			if(skill.StepSend(dwID,dwLev,pMainPlayer,NULL,x,y))
			{
				//���弼��
				SetBufferSkillID(dwID);
				// ��Ŀ��ʹ�ü���
				StartAttack(m_pCurAttractShape);
				m_stCurAreaSkill.dwSkillID = dwID;
				pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), m_stCurAreaSkill.fPosX, m_stCurAreaSkill.fPosY);
			}
			pMainPlayerHand->SetMainPlayerHandByEffect(0,0);
			return 0;
		}
	}

	//��������������Ʒ,����Ʒ�ӵ�����
	if(pMainPlayerHand!=NULL)
	{
		// ���ѣ��״̬�²��ܶ�����
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
				if(pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetProperty()->dwType==GT_MEDAL)
				{
					long lType = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType;
					long lpos = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lPos;
					//Դ�Ƿ�ѫ���б�
					if(lType == PEI_MEDAL)
					{
						pMainPlayerHand->ClearMainPlayerHand();
					}else if(lType == PEI_EQUIPMENT){
						GetGame()->GetCGuiEx()->GetCharacterPageEx()->ChangeEquip(0xffffffff,lpos,PEI_MEDAL,PEI_EQUIPMENT);
						pMainPlayerHand->ClearMainPlayerHand();
					}
					return 1;
				}

				// �жϴ���Ʒ�Ƿ��Ƕ����ͻ���ʧ����Ʒ
				if (pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_PARTICULAR_ATTRIBUTE,1)&16)
				{
					GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = 2;
					GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(GetGame()->GetCGuiEx()->GetItemPageEx()
						,CStringReading::LoadString(208,214),CMessageBoxPageEx::MSG_OK_NO,true);
					return 0;
				}

				// ���壺��������Ʒ
				long dwFreezeVal = pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_FREEZE,1);
				if (dwFreezeVal==eFT_Frost||dwFreezeVal==eFT_Thawing||dwFreezeVal==eFT_CantThaw)
				{
					GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(NULL,CStringReading::LoadString(208,154),CMessageBoxPageEx::MSG_OK,true);
					return 0;
				}

				// ���壺�������Ʒ
				long dwBindVal = pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetEffectVal(GAP_BIND,1);
				if( dwBindVal==eBT_Bind||dwBindVal==eBT_CantUntie||dwBindVal==eBT_GetAndCantUntie||dwBindVal==eBT_GetAndBind )
				{
					GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = 6;
					GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open(GetGame()->GetCGuiEx()->GetItemPageEx(), CStringReading::LoadString(208,155),CMessageBoxPageEx::MSG_OK_NO, true);
					return 0;
				}

				long lPos=pMainPlayer->GetTileY()*pRegion->GetWidth()+pMainPlayer->GetTileX();
				DWORD dwAmount = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lNum;
				long lType = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lType;
				long lpos = pMainPlayerHand->GetTGoodsOfMainPlayerHand()->lPos;
				pMainPlayerHand->SendPutDownMes(TYPE_PLAYER,pMainPlayer->GetExID(),lType,lpos,TYPE_REGION,
					pRegion->GetExID(),0,lPos,TYPE_GOODS,pMainPlayerHand->GetPGoodsOfMainPlayerHand()->GetExID(),dwAmount);
				SetMoveAble(false);

				return 1;
			}
			else if (pMainPlayerHand->GetGoodsIndexOfMainPlayerHand()!=0)
			{
				pMainPlayerHand->ClearMainPlayerHand();
			}
		}
	}

	// ��ǰ���ָ��Ķ���Ϊ�ջ�����ָ����Ʒ(�������)
	if(m_pMouseShape == NULL || m_pMouseShape == pMainPlayer)
	{
		// ����괦������״̬ʱ��ȡ�����״̬
		if (GetGame()->GetMouseType()==MC_STATE_TRADE)
		{
			GetGame()->GetCGuiEx()->GetNPCShopPageEx()->SetRepairState(false);
			GetGame()->SetMouseCursor(MC_NORMAL);
			return 0;
		}
		// ȡ�����״̬
		if (GetGame()->GetMouseType()>=MC_TRYON_GOODS)
		{
			GetGame()->SetMouseCursor(MC_NORMAL);
			return 0;
		}
		// �޸��Ӽ��̲���
		if((GetKeyboard()->IsKeyDown(DIK_LSHIFT) ==false && GetKeyboard()->IsKeyDown(DIK_RSHIFT) == false) &&
			(GetKeyboard()->IsKeyDown(DIK_SPACE) ==false))
		{
			// ��ǰ���ָ�������Ʒ������������״̬��ʰȡ��Ʒ
			if(m_pMouseShapeGoods)
			{
				CGoods *pGoods = dynamic_cast<CGoods*>(m_pMouseShapeGoods);
				if (pGoods)
				{
					// ����ʰȡ
					if (IsCanPickupGoodsCondition())
					{
						// ��Ʒ���������Χ���ܹ�ȥ,ʰȡ���ܵ���Ʒ���󴥷�
						if(IsMoveAble() && pMainPlayer->Distance(pGoods) > 1)
						{
							FollowObject(pGoods->GetType(),pGoods->GetExID(),1);
							return 1;
						}
						if (PickupGoods(pGoods) == 1)
						{
							return 1;
						}
					}
				}
			}
			//�����ǰ��ΪģʽΪ�Զ�����,�������Զ�����ģʽ����ֹͣ�Զ�����
			if(GetMPActionMode() == MPAM_AutoAttack && !( GetAutoAttackMode()&AAM_STOP) )
			{
				//ֹͣ�Զ�����
				AutoAttactStop();
			}
			//�ƶ�
			else
			{
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer)
				{
					// �����������ʩ������ʱ���ƶ�
					if (pPlayer->GetIntonateKey() ||
						pPlayer->GetAction() < CShape::ACT_PERFORM ||
						(pPlayer->GetAction() >= CShape::ACT_FACEACTION && pPlayer->GetAction() <= CShape::ACT_FACEACTIONMAX))
					{
						// ��������������
						float lDestX=0.0f, lDestY=0.0f;
						pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lDestX, lDestY);
						if (GetGame()->GetSetup()->bfreecamera)
						{
							GetGame()->SetScrollCenterX(lDestX);
							GetGame()->SetScrollCenterY(lDestY);
						}
						else
						{
#ifdef MOVE_IN_TILE
							pPlayer->SetCurIntonateTime(0,0);
							pPlayer->SetIntonateSkillKey(0);
							AutoMoveToEx(lDestX, lDestY);
#else
							// ���ǵ����ǰ��վ��λ��
							if(!m_bHotKeyMove&&(pPlayer->GetTileX() != (long)lDestX || pPlayer->GetTileY() != (long)lDestY))
							{
								pPlayer->SetCurIntonateTime(0,0);
								pPlayer->SetIntonateSkillKey(0);
								AutoMoveToEx((long)lDestX, (long)lDestY);
							}
#endif
						}
						GetGame()->GetRegion()->PlayEffect(1,lDestX,lDestY);
					}
				}
			}
			GetMouse()->SetCurrentButton(NONE);
		}
	}
	// ���ָ�����Ϊ�գ��Ҳ��ǵ���Լ�
	else if(m_pMouseShape != pMainPlayer)
	{
		// ����������������ϴ���
		if( OnLeftClickShape() )
		{
			SetMoveAble(false);
			return 1;
		}
	}
	// Debug�µ������  ˲���ƶ�
	if( g_bDebug && 
		GetKeyboard()->IsKeyDown(DIK_SPACE) &&
		(pMainPlayer->GetAction() == CShape::ACT_STAND || pMainPlayer->GetAction() == CShape::ACT_IDLE ||
		pMainPlayer->GetAction() == CShape::ACT_BEATTACK))
	{
		float lMouseX, lMouseY;
		pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lMouseX, lMouseY);

		CMessage msg(MSG_C2S_SHAPE_SETPOS);
		msg.Add(pMainPlayer->GetType());
		msg.Add(pMainPlayer->GetExID());
		msg.Add((long)lMouseX);
		msg.Add((long)lMouseY);
		msg.Send();
		SetMoveAble(false);
		AutoAttactStop();
		AutoMoveStop();
		return 1;
	}

	return 0;
}


// ��������ס����
long CGameControl::OnLeftButtonHolding()
{	
	// ���������ס���������Զ�������귽���ƶ�
	if ( GetMPActionMode() == MPAM_AutoMove &&
		m_dwLeftHoldingTime && !m_bHotKeyMove &&
		GetCurTickCount() - m_dwLeftHoldingTime > 800 )
	{
		m_dwLeftHoldingTime = 0;
		if(m_AutoMoveMode != AMM_FOLLOW_MOUSE)
		{
			m_AutoMoveMode = AMM_FOLLOW_MOUSE;
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_AUTOMOVE),
				D3DCOLOR_ARGB(255,255,255,0));
		}
	}
	return 0;
}

// ������˫��
long CGameControl::OnLeftDoubleButtonDown()
{
	if (m_pMouseShape && m_pMouseShape != pMainPlayer) 
	{
		// ��Ҵ���ǿ�ƹ���״̬���ҵ�ǰ���ָ��Ķ�����Ŀǰ��ѡ��Ķ��󣬷���
		if(!pMainPlayer->IsInDefianceState() && m_pSelectedShape != m_pMouseShape||m_pMouseShape->GetType()==TYPE_COLLECTION)
		{
			return 0;
		}
		//���ܲ���
		if (!IsCanControlCondition())
			return 0;
		// ת��Ϊ����ģʽ
		if(GetMPActionMode() == MPAM_AutoMove)
		{
			//ֹͣ�Զ�����
			AutoMoveStop();
		}

		CShape* pAttackShape = NULL;
		SetSelectedShape(m_pMouseShape);
		pAttackShape = m_pMouseShape;
		SetAutoSkillID(pMainPlayer->GetDefaultAttackSkillID());

		// ˫�����
		if(pAttackShape->GetType() == TYPE_PLAYER)
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>(pAttackShape);
			if (pPlayer->IsDied())
			{
				SetAutoSkillID(0);
			}
			// ���ǵж���ң�����ǿ�й���
			if( GetGame()->GetMainPlayer()->IsCityWarEnemyFaction( pPlayer->GetFactionID() ) == false &&
				GetGame()->GetMainPlayer()->IsEnemyFaction(pPlayer->GetFactionID()) == false && 
				(!GetKeyboard()->IsKeyDown(DIK_LCONTROL) && !GetKeyboard()->IsKeyDown(DIK_RCONTROL)) )
			{
				pAttackShape = NULL;
				StartAttack(pAttackShape);
				return 0;
			}
		}
		// ˫��monster
		if(pAttackShape->GetType() == TYPE_MONSTER)
		{
			CMonster *pMonster = dynamic_cast<CMonster*>(pAttackShape);			
			// ����
			if(pMonster->IsDied())
			{
				SetAutoSkillID(0);
			}
			// ˫��NPC,�Ҳ���ǿ�й���
			if(pMonster->GeteNpcType() == NT_Normal && (!GetKeyboard()->IsKeyDown(DIK_LCONTROL) && !GetKeyboard()->IsKeyDown(DIK_RCONTROL)))
			{
				// ȡ����������͹�������
				m_pCurAttractShape = NULL;
				SetAutoSkillID(0);
				if (pMainPlayer->Distance(pMonster) > 5)
				{
					// �ܹ�ȥ�Ի�
					FollowObject(pMonster->GetType(),pMonster->GetExID(),3);
					return 0;
				}
				else
				{
					SendTouchNpcMes(pMonster->GetExID());
					return 0;
				}
			}
		}

		//��ʼ����
		StartAttack(pAttackShape);
	}
	return 0;
}

// �������SHAPE�ϵ��´���
long CGameControl::OnLeftClickShape()
{
	if (m_pMouseShape) 
	{
		// ��Ҵ���ǿ�ƹ���״̬���ҵ�ǰ���ָ��Ķ�����Ŀǰ��ѡ��Ķ��󣬷���
		if(!pMainPlayer->IsInDefianceState() && m_pSelectedShape != m_pMouseShape||m_pMouseShape->GetType()==TYPE_COLLECTION)
		{
			return 0;
		}

		CShape * pOldSelected = m_pSelectedShape;
		SetSelectedShape(m_pMouseShape);

		// ������ظ���������й���
		if(pOldSelected == m_pSelectedShape)
		{
			if(GetMPActionMode() == MPAM_AutoMove)
			{
				//ֹͣ�Զ�����
				AutoMoveStop();
			}
			StartAttack(m_pSelectedShape);
			m_pCurAttractShape = m_pSelectedShape;
			SetAutoSkillID(pMainPlayer->GetDefaultAttackSkillID());
		}
		// ���������Ҵ��ھ�ֹ״̬�ı���ҷ���
		else if(pMainPlayer->GetAction() == CShape::ACT_STAND || pMainPlayer->GetAction() == CShape::ACT_IDLE)
		{
			float fRadianDir = pMainPlayer->GetLineDirF(pMainPlayer->GetPosX(),pMainPlayer->GetPosY(),
				m_pSelectedShape->GetPosX(),m_pSelectedShape->GetPosY(),pMainPlayer->GetDirEx());
			if (fRadianDir != pMainPlayer->GetDirEx())
			{
				pMainPlayer->SetDirEx(fRadianDir);
				return 1;
			}
		}
		switch( m_pMouseShape->GetType() )
		{
		case TYPE_PLAYER:
			{
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(m_pSelectedShape);
				if (pPlayer==NULL)
				{
					if(pOldSelected == m_pSelectedShape)
					{
						SetSelectedShape(NULL);
						m_pCurAttractShape = NULL;
						SetAutoSkillID(0);
					}
					return 0;
				}
				if (pPlayer->IsDied())
				{
					if(pOldSelected == m_pSelectedShape)
					{
						SetAutoSkillID(0);
					}
					return 0;
				}

				// ���ø���������ù�������Ϊ��
				if ( GetKeyboard()->IsKeyDown( DIK_LALT ) || GetKeyboard()->IsKeyDown( DIK_RALT ) )
				{
					if( GetGame()->GetCGuiEx()->GetTreasureShopPageEx()->IsOpen() )
					{
						GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_CANTFOLLOWPLAYER),D3DCOLOR_ARGB(255,255,255,0));
						return 0;
					}
					m_pCurAttractShape = NULL;
					SetAutoSkillID(0);

					string strName = pPlayer->GetName();
					// ������Ϣ�Ķ��ѣ���ȡ�����е���Ϣ
					if(pPlayer->GetIsHideInfo() && pMainPlayer->IsTeammate(pPlayer->GetExID()))
					{
						tagWSMemberInfo * stInfo = pMainPlayer->GetWsMemberInfo(pPlayer->GetExID());
						if(stInfo)
							strName = stInfo->szName;
					}
					char str[256];
					sprintf(str, CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_FOLLOWPLAYER), strName.c_str());
					GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,str,D3DCOLOR_ARGB(255,255,255,0));
					//�������
					FollowObject(TYPE_PLAYER,pPlayer->GetExID(),1);
					return 0;
				}

				// �ظ������ң�����ǿ�й��������ǵж��л��Ա��Ҳ���ǵй���Ա��ȡ������״̬
				if( pOldSelected == m_pSelectedShape && GetGame()->GetMainPlayer()->IsCityWarEnemyFaction( pPlayer->GetFactionID() ) == false &&
					GetGame()->GetMainPlayer()->IsEnemyFaction(pPlayer->GetFactionID()) == false  && 
					(!GetKeyboard()->IsKeyDown(DIK_LCONTROL) && !GetKeyboard()->IsKeyDown(DIK_RCONTROL)))
				{
					return 0;
				}
			}
			break;

		case TYPE_MONSTER:
			{
				// �������ʱ���Զ���������
				if (pMainPlayer->GetHorse()->IsVisible())
				{
					return 0;
				}

				CMonster *pMonster = dynamic_cast<CMonster*>(m_pMouseShape);
				if (pMonster==NULL)
				{
					if(pOldSelected == m_pSelectedShape)
					{
						SetSelectedShape(NULL);
						m_pCurAttractShape = NULL;
						SetAutoSkillID(0);
					}
					return 0;
				}
				// ����
				if (pMonster->IsDied())
				{
					if(pOldSelected == m_pSelectedShape)
					{
						SetSelectedShape(NULL);
						m_pCurAttractShape = NULL;
						SetAutoSkillID(0);
					}
					return 0;
				}
				// �ظ���NPC,�Ҳ���ǿ�й���
				if( pMonster->GeteNpcType() == NT_Normal && pOldSelected == m_pSelectedShape 
					&& (!GetKeyboard()->IsKeyDown(DIK_LCONTROL) && !GetKeyboard()->IsKeyDown(DIK_RCONTROL))
					)
				{
					// ȡ����������͹�������
					m_pCurAttractShape = NULL;
					SetAutoSkillID(0);
					if (pMainPlayer->Distance(pMonster) > 5)
					{
						// �ܹ�ȥ�Ի�
						FollowObject(pMonster->GetType(),pMonster->GetExID(),3);
						return 0;
					}
					else
					{
						SendTouchNpcMes(pMonster->GetExID());
						return 0;
					}
				}
				return 0;
			}
			break;

		case TYPE_BUILD:
		case TYPE_CITYGATE:
			{
				if(GetGame()->GetMouseType()>=MC_TRYON_GOODS)
				{
					GetGame()->SetMouseCursor(MC_NORMAL);
					return 0;
				}
				// �����ﱻ����
				if ( ((CMoveShape*)m_pMouseShape)->IsDied())
				{
					if(pOldSelected == m_pSelectedShape)
					{
						SetSelectedShape(NULL);
						m_pCurAttractShape = NULL;
						SetAutoSkillID(0);
					}				
					return 1;
				}
				return 0;
			}
			break;

			// ���ɹ����Ķ���
		default:
			{
				m_pCurAttractShape = NULL;
				SetAutoSkillID(0);
			}
			break;
		}
	}
	return 1;
}

// ����Ҽ����´���
long CGameControl::OnRightButtonDown()
{
	// ������״̬,���
	CMainPlayerHand *pHand = GetGame()->GetCGuiEx()->GetPMainPlayerHand();
	if(pHand)
	{
		if((pHand->GetAreaSkillID() > 0 || pHand->GetSkillOfMainPlayerHand() > 0)&&m_bRButtonDownValid)
			pHand->ClearMainPlayerHand();
	}
	m_bRButtonDownValid = true;
	m_pPressRBtnShape = NULL;	
	// ���Լ����ܲ���
	if( m_pMouseShape == pMainPlayer || IsCanControlCondition()==false)
		return 0;
	// ����������������ϴ���
	if (m_pMouseShape)
	{
		// ��Ҵ���ǿ�ƹ���״̬���ҵ�ǰ���ָ��Ķ�����Ŀǰ��ѡ��Ķ��󣬷���
		if(!pMainPlayer->IsInDefianceState() && m_pSelectedShape != m_pMouseShape)
		{
			return 0;
		}
		m_pPressRBtnShape = m_pMouseShape;
	}
	return 0;
}

// ����Ҽ��ɿ�����
long CGameControl::OnRightButtonUp()
{
	// ������ºͷſ�����ͬһ�������ϣ�ִ�е������
	if(m_pMouseShape && m_pMouseShape == m_pPressRBtnShape)
		OnRightClickShape();
	return 0;
}


// ����ҽ���SHAPE�ϵ��´���
long CGameControl::OnRightClickShape()
{
	if (m_pMouseShape) 
	{
		SetSelectedShape(m_pMouseShape);
		if(m_pSelectedShape && pMainPlayer->GetAction() == CShape::ACT_STAND || pMainPlayer->GetAction() == CShape::ACT_IDLE)
		{
			float fRadianDir = pMainPlayer->GetLineDirF(pMainPlayer->GetPosX(),pMainPlayer->GetPosY(),
				m_pSelectedShape->GetPosX(),m_pSelectedShape->GetPosY(),pMainPlayer->GetDirEx());
			if (fRadianDir != pMainPlayer->GetDirEx())
			{
				// ת����
				pMainPlayer->SetDirEx(fRadianDir);
			}
		}
		// �Ҽ����������
		if( m_pMouseShape->GetType() == TYPE_PLAYER )
		{
			// �鿴״̬��ȡ������������Զ���������
			if( GetKeyboard()->IsKeyDown( DIK_LCONTROL ) || GetKeyboard()->IsKeyDown( DIK_RCONTROL ) )
			{
				SetMoveAble(false);
				long mx = GetMouse()->GetMouseX();
				long my = GetMouse()->GetMouseY();
				return 1;
			}
			// ����˽�Ķ���
			if ( GetKeyboard()->IsKeyDown( DIK_LALT ) || GetKeyboard()->IsKeyDown( DIK_RALT ) )
			{
				string strName = m_pMouseShape->GetName();
				SetMoveAble(false);
				return 1;
			}
			// �����Է��ĸ����̵�
			if (((CPlayer*)m_pMouseShape)->IsPlayerShopOpened())
			{
				float fDistance = (float)pMainPlayer->Distance(m_pMouseShape);
				if (fDistance < 6)
				{
					CMessage msg(MSG_C2S_PLAYERSHOP_LOOKGOODS);
					msg.Add(((CPlayer*)m_pMouseShape)->GetPlayerShopSessionID());
					msg.Add(((CPlayer*)m_pMouseShape)->GetExID());
					msg.Send();
					AutoMoveStop();
					return 1;
				}
			}
			//------------------------07.8.5(liuke���)----------------------------
			if(((CPlayer*)m_pMouseShape)->IsRecruitingTeam())
			{
				// ����ж��飬���ܵ�������ҵ���ļ��Ϣ
				if(!GetGame()->GetMainPlayer()->GetTeamMateInfo().empty())
				{
					char str[256]="";
					// ���Ӷӳ���ļ
					if(m_pMouseShape->GetExID() == GetGame()->GetMainPlayer()->GetCaptainID())
						sprintf_s(str,"%s",CStringReading::LoadString(eIDS_TEAM,eIDS_TEAM_INTHISTEAM));
					else
						sprintf_s(str,"%s",CStringReading::LoadString(eIDS_TEAM,eIDS_TEAM_HASTEAM));
					GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,str,D3DCOLOR_ARGB(255,255,0,0));
					return 0;
				}
				CRecruitBoxPageEx *pRecruitBox = GetGame()->GetCGuiEx()->GetRecruitBoxPageEx();
				CMessageBoxPageEx *pMessageBox = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
				if(pRecruitBox && pMessageBox)
				{
					pRecruitBox->SetPlayerGUID(m_pMouseShape->GetExID());
					pMessageBox->Open(pRecruitBox,CStringReading::LoadString(eIDS_TEAM,eIDS_TEAM_ISJOINTEAM),CMessageBoxPageEx::MSG_OK_NO,true);
				}
				SetMoveAble(false);
				return 1;
			}
			// �й���Ŀ��ֱ���л�
			if(m_pCurAttractShape)
				StartAttack(m_pSelectedShape);
			//-----------------------------------------------------------------
		}
		// �Ҽ���������NPC��
		else if(m_pMouseShape->GetType() == TYPE_MONSTER)
		{
			CMonster *pMonster = dynamic_cast<CMonster*>(m_pMouseShape);
			if (pMonster!=NULL)
			{
				// ����
				if(pMonster->IsDied())
				{
					if (((CMoveShape*)m_pMouseShape)->GetCollectGoodsID()!=0)
					{
						SetFollowingObj(0,CGUID::GUID_INVALID);
						SetBufferSkillID(0);		// ȡ�����弼��
						// ȡ����������͹�������
						m_pCurAttractShape = NULL;
						SetAutoSkillID(0);
						CCollectionGoods* pCollectGoods = (CCollectionGoods*)m_pMouseShape;
						if (pMainPlayer->Distance(pCollectGoods) > 1)
						{
							// �ܹ�ȥ�ɼ�
							FollowObject(m_pMouseShape->GetType(),m_pMouseShape->GetExID(),1);
							return 1;
						}
						else
						{
							DoCollect(pCollectGoods);
							return 1;
						}
					}else 
						return 1;
				}
				// �Ҽ����NPC,�Ҳ���ǿ�й�������ȥ�Ի�
				if(pMonster->IsCanTalk())
				{
					SetFollowingObj(0,CGUID::GUID_INVALID);
					SetBufferSkillID(0);		// ȡ�����弼��
					// ȡ����������͹�������
					m_pCurAttractShape = NULL;
					SetAutoSkillID(0);
					if (pMainPlayer->Distance(pMonster) > 5)
					{
						// �ܹ�ȥ�Ի�
						FollowObject(pMonster->GetType(),pMonster->GetExID(),3);
						return 1;
					}
					else
					{
						SendTouchNpcMes(pMonster->GetExID());
						return 1;
					}
				}
				// �й���Ŀ��ֱ���л�
				if(m_pCurAttractShape)
					StartAttack(m_pSelectedShape);
			}
		}
		else if (m_pMouseShape->GetType() == TYPE_COLLECTION)
		{
			CCollectionGoods* pCollectGoods = (CCollectionGoods*)m_pMouseShape;
			if (pMainPlayer->Distance(m_pMouseShape) > 1)
			{
				// �ܹ�ȥ�Ի�
				FollowObject(m_pMouseShape->GetType(),m_pMouseShape->GetExID(),1);
				return 1;
			}
			else
			{
				DoCollect(pCollectGoods);
				return 1;
			}
		}
		else if (m_pMouseShape->GetType() == TYPE_PET)
		{
		}
	}
	return 0;
}

// ���͵��NPC����Ϣ
void CGameControl::SendTouchNpcMes(CGUID NpcGUID)
{
	CMonster *pMonster = dynamic_cast<CMonster *>(pRegion->FindChildObject(TYPE_MONSTER,NpcGUID));
	if(pMonster)
	{
		// �������ɰ�Ƥ
		if(pMonster->IsDied() && pMonster->GetCollectGoodsID()!=0)
		{
			CCollectionGoods* pCollectGoods = (CCollectionGoods*)pMonster;
			if(pCollectGoods)
			{
				DoCollect(pCollectGoods);
				return;
			}
		}
		// �������NPC�����µķ���Ϊ�������
		if(pMainPlayer && pRegion)
		{
			float fRadianDir = pMonster->GetLineDirF(pMonster->GetPosX(),pMonster->GetPosY(),pMainPlayer->GetPosX(),pMainPlayer->GetPosY());
			pMonster->SetDirEx(fRadianDir);
		}
	}
	CNPCShopPageEx* pNPCShop = GetGame()->GetCGuiEx()->GetNPCShopPageEx();
	CPlayerShopPageEx* pPlayerShop = GetGame()->GetCGuiEx()->GetPlayerShopPageEx();
	CPlayerTradePageEx* pPlayerTrade = GetGame()->GetCGuiEx()->GetPlayerTradePageEx();
	if (pNPCShop!=NULL&&pPlayerShop!=NULL&&pPlayerTrade!=NULL&&
		(pNPCShop->IsOpen()||pPlayerShop->IsOpen()||pPlayerTrade->IsOpen()))
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
			CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_CANTTOUCHNPC),
			D3DCOLOR_ARGB(255,255,0,0));
		return;
	}
	CMessage msg(MSG_C2S_PLAYER_TOUCHNPC);
	msg.Add(NpcGUID);
	msg.Send();
	GetGame()->SetLastTouchNpcID(NpcGUID);
	// ��������䣬����guid
	if(pMonster && pMonster->GetOrigName() && strcmp(pMonster->GetOrigName(),"10000Email")==0)
		GetGame()->GetCGuiEx()->GetMailPageEx()->SetMailBoxGUID(NpcGUID);

	// ���������������NPC������guid
	if(pMonster && pMonster->GetOrigName() && strcmp(pMonster->GetOrigName(),"40091Meladire")==0)
		GetGame()->GetCGuiEx()->GetHairBeautyPageEx()->SetNPC(NpcGUID);
}

void CGameControl::DoCollect(CMoveShape* pMoveShape)
{
	if (pMoveShape->GetType()==TYPE_COLLECTION&&pMoveShape->IsDied()) return;
	const CCollectionLayout* pCollectLayout = CDeputyOccuSystemClient::GetCollectionLayout(pMoveShape->GetCollectGoodsID());
	if (pMoveShape!=NULL&&pCollectLayout!=NULL&&pCollectLayout->GetOccuID()!=0)
	{
		if (!pMoveShape->IsBeingCollected()&&pMoveShape->IsCanBeCollected())
		{
			bool bShowResults = true;
			DWORD dwCollectType = CDeputyOccuSystemClient::OB_Collection;
			const list<tgConditionGoods> pConditionList = pCollectLayout->GetConditionList();
			if (pCollectLayout->GetOccuID()!=DOccu_Other&&pCollectLayout->GetOccuID()!=pMainPlayer->GetAssOccu())
			{
				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
					CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_NOOCCU),
					D3DCOLOR_ARGB(255,255,0,0));
				bShowResults = false;
			}
			// �����������
			else if (pCollectLayout->GetOccuID()==DOccu_Other)
			{
				dwCollectType = CDeputyOccuSystemClient::OB_TaskCollection;
				const vector<tagQuestGoods>& QustList = pCollectLayout->GetRoleInfo();
				if (!QustList.empty())
				{
					vector<tagQuestGoods>::const_iterator QustIter = QustList.begin();
					for (;QustIter!=QustList.end();QustIter++)
					{
						if (QustIter->nQuestId!=-1)
						{
							CRoleQueue* pQueueList = pMainPlayer->GetRoleQueue();
							if (pQueueList!=NULL)
							{
								if (pQueueList->GetRecordByID(QustIter->nQuestId)==NULL||
									pQueueList->GetRoleStepNum(QustIter->nQuestId)!=QustIter->dwRoleStep)
								{
									continue;
								}
								else if (pQueueList->GetRecordByID(QustIter->nQuestId)!=NULL&&
									pQueueList->GetRoleStepNum(QustIter->nQuestId)==QustIter->dwRoleStep)
								{
									break;
								}
							}
							else
							{
								continue;
							}
						}
						else
							break;
					}
					if (QustIter==QustList.end())
					{
						bShowResults = false;
						GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadString(eIDS_CONTROL,45),
							D3DCOLOR_ARGB(255,255,0,0));
					}
					else if (QustIter!=QustList.end())
					{
						bShowResults = true;
					}
				}
			}
			// ��Ʒ�������
			if (bShowResults&&pConditionList.size()!=0)
			{
				list<tgConditionGoods>::const_iterator conIter = pConditionList.begin();
				for (;conIter!=pConditionList.end();conIter++)
				{
					long lGoodNum = pMainPlayer->GetGoodsAmount(conIter->cGoodsName);
					if (conIter->iNumType==0&&lGoodNum<conIter->iNum||
						conIter->iNumType==1&&lGoodNum!=conIter->iNum||
						conIter->iNumType==2&&lGoodNum>=conIter->iNum)
					{
						bShowResults = false;
						WORD wChoseIndex = 0;
						switch(conIter->iNumType)
						{
						case 0:
							wChoseIndex = 6;
							break;
						case 1:
							wChoseIndex = 7;
							break;
						case 2:
							wChoseIndex = 8;
							break;
						}
						GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadString(205,wChoseIndex),
							D3DCOLOR_ARGB(255,255,0,0));
						break;
					}
				}
			}
			if (bShowResults)
			{
				UseOccuSkill(dwCollectType,(CShape*)pMoveShape);
			}
		}
		else if (pMoveShape->IsBeingCollected())
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_BEINGCOLLECTED),
				D3DCOLOR_ARGB(255,255,0,0));
		}
		else if (!pMoveShape->IsCanBeCollected())
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_CANTBECOLLECTED),
				D3DCOLOR_ARGB(255,255,0,0));
		}
	}
}

// ��ָ�������ƶ�һ��,������һ��
//dir:����
//action:����(run or walk)
//bDis:�ƶ�����,�ܵ�ʱ�������2,������1
long CGameControl::MoveDir(CMoveShape* pShape, long dir,short nDis)
{
	WORD action = CShape::ACT_WALK;
	if(pShape->GetIsRun())
		action = CShape::ACT_RUN;

	//����ֻ��һ��
	if(action == CShape::ACT_WALK)	nDis = 1;
	// �����ƶ�״̬
	if (IsCanMoveCondition()==false)
		return 0;

	long lOrDir		= dir;
	long x			= pShape->GetTileX();
	long y			= pShape->GetTileY();
	long newx1		= x;
	long newy1		= y;
	long newx2		= x;
	long newy2		= y;

	int i=0;
	for(; i<8; i++)
	{
		newx1 = x + _dir[_slip_order[dir][i]][0];
		newy1 = y + _dir[_slip_order[dir][i]][1];
		if( pRegion->GetBlock(newx1, newy1) == CRegion::BLOCK_NO ||
			pRegion->GetBlock(newx1, newy1) == CRegion::BLOCK_AIM)
		{
			dir = pShape->GetDestDir(x,y, newx1, newy1);
			if( dir != lOrDir && nDis == 2 )
			{
				nDis = 1;
			}
			break;
		}
	}
	//û���ҵ�������
	if(i == 9)	return 0;

	if( action == CShape::ACT_RUN && nDis == 2)
	{
		newx2 = newx1 + _dir[dir][0];
		newy2 = newy1 + _dir[dir][1];
		if( pRegion->GetBlock(newx2, newy2) != CRegion::BLOCK_NO &&
			pRegion->GetBlock(newx2, newy2) != CRegion::BLOCK_AIM )
		{
			nDis = 1;
		}
	}
	pShape->SetAction(action);
	pShape->SetDir((CShape::eDIR)dir);

	if( nDis == 1 )
	{
		pShape->SetDestXY(newx1+0.5f, newy1+0.5f);
	}
	else// nDis == 2 
	{
		pShape->SetDestXY(newx2+0.5f, newy2+0.5f);
	}

	SetControlAble(false);
	// ���ÿ�ʼ�ƶ���ʱ��
	pShape->SetIsArrive(false);
	SetSendedMoveMsgCount(GetSendedMoveMsgCount()+1);
	return 1;

}

//��ĳ�����ƶ�����
//fdir:�Ƕ�
long CGameControl::MoveToCell(float fDestX,float fDestY)
{
	//�ƶ�����
	static long lMoveCellCount = 0;
	//�ϴ�ͳ�Ƶ��ƶ���֤ʱ��
	static DWORD dwLastMoveValiTime = 0;
	static const DWORD dwMaxValiNum = 40;
	// �����ƶ�״̬
	if (IsCanMoveCondition()==false)
		return 0;
	float fx = pMainPlayer->GetPosX();
	float fy = pMainPlayer->GetPosY();

	float fRadianDir = GetRadianDir(fx,fy,fDestX,fDestY);

	////�����µķ���
	pMainPlayer->SetDirEx(fRadianDir);
	pMainPlayer->SetDestXY(fDestX,fDestY);

	pMainPlayer->SetIsArrive(false);
	//�ó��ƶ�ʱ��
	float fTotalDistance = pMainPlayer->RealDistance(fDestX,fDestY);
	DWORD dwMoveTime = fTotalDistance/pMainPlayer->GetSpeed();	
	DWORD dwCurTime = GetCurTickCount();
	pMainPlayer->SetEndMoveTime(dwCurTime+dwMoveTime);	
	if(pMainPlayer->GetAction() != CShape::ACT_WALK &&
		pMainPlayer->GetAction() != CShape::ACT_RUN)
	{
		//������ڵĶ����������߶���,�����ϴεĸ���ʱ��
		pMainPlayer->SetLastUpdatePosTime(dwCurTime);
	}

	//���ø���λ��
	SetUpdateXY(fx,fy);
	WORD action = CShape::ACT_WALK;
	if(pMainPlayer->GetIsRun())
		action = CShape::ACT_RUN;
	if(pMainPlayer->GetAction() != action)
	{
		pMainPlayer->SetAction(action);
	}

	//�ƶ�Ƶ����֤
	lMoveCellCount++;
	//ÿ10�β���ͳ��һ��
	if(lMoveCellCount>dwMaxValiNum)
	{
		if( (dwCurTime-dwLastMoveValiTime) < (float)dwMaxValiNum/pMainPlayer->GetSpeed()*0.7f )
		{
			CMessageBoxPageEx *pMsgPage = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			pMsgPage->Open(pMsgPage,CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_MOVEHIGHFREQ),CMessageBoxPageEx::MSG_OK,true);
		}
		lMoveCellCount = 0;
		dwLastMoveValiTime = dwCurTime;
	}

#ifdef MOVE_IN_TILE
	if(fDestX != fx || fDestY != fy )
	{
#else
	static long lLastSX = 0,lLastSY = 0;
	static long lLastDX = 0,lLastDY = 0;
	if(lLastSX != (long)fx || lLastSY != (long)fy ||
		lLastDX != (long)fDestX || lLastDY != (long)fDestY)
	{
		lLastSX = (long)fx;lLastSY = (long)fy;
		lLastDX = (long)fDestX; lLastDY = (long)fDestY;
#endif
		m_dwMoveMsgCounter++;
		long lCount = 5;
		CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
		if(pSetup)
			lCount = pSetup->lMoveMsgCount;
		if(GetSendedMoveMsgCount() < lCount)
		{
			CMessage msg(MSG_C2S_SHAPE_MOVE);
			msg.Add(fx);
			msg.Add(fy);
			msg.Add(fDestX);
			msg.Add(fDestY);
			msg.Send();
			SetSendedMoveMsgCount(GetSendedMoveMsgCount() + 1);
		}

#ifdef _CLENTTOTEST_
		CMessage msgtot(MSG_C2T_SHAPE_MOVE);
		msgtot.Add((CGUID)CGUID::GUID_INVALID);
		msgtot.Add(fx);
		msgtot.Add(fy);
		msgtot.Add(fDestX);
		msgtot.Add(fDestY);
		msgtot.SendMemToTest();
#endif
	}
	return 1;
}


long	CGameControl::MoveToCellByHotKey(float fDestX,float fDestY)
{
	if (IsCanMoveCondition()==false)
		return 0;
	float fx = pMainPlayer->GetPosX();
	float fy = pMainPlayer->GetPosY();

	float fRadianDir = GetRadianDir(fx,fy,fDestX,fDestY);
	////�����µķ���
	pMainPlayer->SetDirEx(fRadianDir);
	pMainPlayer->SetDestXY(fDestX,fDestY);

	pMainPlayer->SetIsArrive(false);
	//�ó��ƶ�ʱ��
	float fTotalDistance = pMainPlayer->RealDistance(fDestX,fDestY);
	DWORD dwMoveTime = fTotalDistance/pMainPlayer->GetSpeed();	
	DWORD dwCurTime = GetCurTickCount();
	pMainPlayer->SetEndMoveTime(dwCurTime+dwMoveTime);	
	//���ø���λ��
	SetUpdateXY(fx,fy);
	WORD action = CShape::ACT_WALK;
	if(pMainPlayer->GetIsRun())
		action = CShape::ACT_RUN;
	if(pMainPlayer->GetAction() != action)
	{
		pMainPlayer->SetAction(action);
	}
	if(fDestX != fx || fDestY != fy )
	{
		m_dwMoveMsgCounter++;
		CMessage msg(MSG_C2S_SHAPE_MOVE);
		msg.Add(fx);
		msg.Add(fy);
		msg.Add(fDestX);
		msg.Add(fDestY);	
		msg.Send();
		char strInfo[256]="";
		_snprintf(strInfo,256,"SendMoveMsg,curTime:%d,sx:%f,sy:%f,dx:%f,dy:%f",timeGetTime(),fx,fy,fDestX,fDestY);
		PutStringToFile("MoveTest",strInfo);

#ifdef _CLENTTOTEST_
		CMessage msgtot(MSG_C2T_SHAPE_MOVE);
		msgtot.Add((CGUID)CGUID::GUID_INVALID);
		msgtot.Add(fx);
		msgtot.Add(fy);
		msgtot.Add(fDestX);
		msgtot.Add(fDestY);
		msgtot.SendMemToTest();
#endif
	}
	return 1;
}

//ֹͣ�ƶ�
long CGameControl::StopMove()
{
	CMessage msg(MSG_C2S_SHAPE_STOPMOVE);
	msg.Add(pMainPlayer->GetPosX());
	msg.Add(pMainPlayer->GetPosY());
	msg.Add((short)pMainPlayer->GetDir());
	msg.Send();

#ifdef _CLENTTOTEST_
	CMessage msgtot(MSG_C2T_SHAPE_STOPMOVE);
	msgtot.Add((CGUID)CGUID::GUID_INVALID);
	msgtot.Add(pMainPlayer->GetPosX());
	msgtot.Add(pMainPlayer->GetPosY());
	msgtot.SendMemToTest();
#endif
	return 1;
}


// ����Ʒ
// ����: 1��ʾ�ɹ�  0��ʾʧ��
long CGameControl::PickupGoods(CGoods* pGoods)
{
	// ��������״̬�²��ܼ���
	if (pMainPlayer->IsRide())
	{
		if(GetGame()->GetCGuiEx()->GetMainBarPageEx())
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(206,75),0xffffffff);
		return 0;
	}
	// ���ѣ��״̬�²��ܼ���
	if (pMainPlayer->GetAction()==CShape::ACT_MEGRIM)
	{
		return 0;
	}
	if (pMainPlayer->GetAction()==CShape::ACT_PICK)
	{
		return 0;
	}
	// �Д�����Ƿ�����Ʒ����
	long PlayerCellX = pMainPlayer->GetTileX();
	long PlayerCellY = pMainPlayer->GetTileY();
	long GoodsCellX = pGoods->GetTileX();
	long GoodsCellY = pGoods->GetTileY();
	if ( abs(PlayerCellX - GoodsCellX) <= 2 && abs(PlayerCellY - GoodsCellY) <= 2)
	{
		// ��Ǯ
		if (pGoods->GetProperty()->dwType==GT_MONEY||pGoods->GetProperty()->dwType==GT_SILVERMONEY)
		{
			DWORD dwAmount = pGoods->GetNum();
			if ((pGoods->GetProperty()->dwType==GT_MONEY&&pMainPlayer->GetMoney()+dwAmount>2000000000)||
				(pGoods->GetProperty()->dwType==GT_SILVERMONEY&&pMainPlayer->GetSilverMoney()+dwAmount>2000000000))
			{
				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(208,212),D3DCOLOR_ARGB(255,255,0,0));
				return 0;
			}
			long lPos = pGoods->GetTileY()*pRegion->GetWidth()+pGoods->GetTileX();
			CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
			msg.Add((LONG)TYPE_REGION);
			msg.Add(pRegion->GetExID());
			msg.Add((LONG)0);
			msg.Add((LONG)lPos);
			msg.Add((LONG)TYPE_PLAYER);
			msg.Add(pMainPlayer->GetExID());
			if (pGoods->GetProperty()->dwType==GT_MONEY)
			{
				msg.Add((LONG)PEI_WALLET);
			}else if (pGoods->GetProperty()->dwType==GT_SILVERMONEY)
			{
				msg.Add((LONG)PEI_SILVERWALLET);
			}
			msg.Add((LONG)0);
			msg.Add((LONG)TYPE_GOODS);
			msg.Add(pGoods->GetExID());
			msg.Add((LONG)dwAmount);
			msg.Send();
		}
		else
		{
			//��������ռ䲻��//�����ۼӵ���Ʒ
			if(pMainPlayer->IsAllPackageFull())
			{
				if(!CGoods::isGoodsCanAccumulate(pGoods))
				{
					GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadString(200,37),
						D3DCOLOR_ARGB(255,255,0,0));
					return 0;
				}
				else
				{
					WORD ContainerID = pMainPlayer->FindContainerIDOfGoods(pGoods);
					if (ContainerID == -1) return 0;
					DWORD dwAmount=pGoods->GetNum();
					long lPos=pGoods->GetTileY()*pRegion->GetWidth()+pGoods->GetTileX();
					CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
					msg.Add((LONG)TYPE_REGION);
					msg.Add(pRegion->GetExID());
					msg.Add((LONG)0);
					msg.Add((LONG)lPos);
					msg.Add((LONG)TYPE_PLAYER);
					msg.Add(pMainPlayer->GetExID());
					msg.Add((LONG)ContainerID);
					msg.Add((DWORD)0xffffffff);
					msg.Add((LONG)TYPE_GOODS);
					msg.Add(pGoods->GetExID());
					msg.Add((LONG)dwAmount);
					msg.Send();
				}
			}
			else
			{
				DWORD dwAmount=pGoods->GetNum();
				long lPos=pGoods->GetTileY()*pRegion->GetWidth()+pGoods->GetTileX();
				CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
				msg.Add((LONG)TYPE_REGION);
				msg.Add(pRegion->GetExID());
				msg.Add((LONG)0);
				msg.Add((LONG)lPos);
				msg.Add((LONG)TYPE_PLAYER);
				msg.Add(pMainPlayer->GetExID());
				msg.Add((LONG)1);
				msg.Add((DWORD)0xffffffff);
				msg.Add((LONG)TYPE_GOODS);
				msg.Add(pGoods->GetExID());
				msg.Add((LONG)dwAmount);
				msg.Send();
			}
		}
		//ת������Ҫʰȡ����Ʒ����
		long dir = GetLineDir(pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), pGoods->GetTileX(), pGoods->GetTileY());
		if (dir != pMainPlayer->GetDir())
		{
			pMainPlayer->SetDir((CShape::eDIR)dir);
			// ת����
			CMessage msg(MSG_C2S_SHAPE_CHANGEDIR);
			msg.Add((BYTE)dir);
			msg.Add(pMainPlayer->GetDirEx());
			msg.Send();
		}
		pMainPlayer->SetAction(CShape::ACT_PICK);
		return 1;
	}
	return 0;
}

//20090701 �������ƶ�
void CGameControl::ProcessMoveKey()
{
	DWORD dwMoveStyle = MOVE_NONE;
	static bool bIsInMoveState = false;
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	DWORD dwCurTime = GetCurTickCount();
	if (GetGame()->GetKeyboard()->IsKeyDown(DIK_W))
		dwMoveStyle = MOVE_FORWARD;
	if (GetGame()->GetKeyboard()->IsKeyDown(DIK_D))
		dwMoveStyle |= MOVE_RIGHT;
	if (GetGame()->GetKeyboard()->IsKeyDown(DIK_A))
		dwMoveStyle |= MOVE_LEFT;
	if (GetGame()->GetKeyboard()->IsKeyDown(DIK_S))
		dwMoveStyle |= MOVE_BACK;
	DWORD lMoveDir = -1; 
	switch (dwMoveStyle)
	{
	case MOVE_FORWARD:
		lMoveDir = CShape::DIR_UP;
		break;
	case MOVE_LEFT:
		lMoveDir = CShape::DIR_LEFT;
		break;
	case MOVE_RIGHT:
		lMoveDir = CShape::DIR_RIGHT;
		break;
	case MOVE_BACK:
		lMoveDir = CShape::DIR_DOWN;
		break;
	case MOVE_LEFT_FORWARD:
		lMoveDir = CShape::DIR_LEFTUP;
		break;
	case MOVE_RIGHT_FORWARD:
		lMoveDir = CShape::DIR_RIGHTUP;
		break;
	case MOVE_LEFT_BACK:
		lMoveDir = CShape::DIR_LEFTDOWN;
		break;
	case MOVE_RIGHT_BACK:
		lMoveDir = CShape::DIR_RIGHTDOWN;
		break;
	}

	static long lDestX = pPlayer->GetDestX();
	static long lDestY = pPlayer->GetDestY();
	if (lMoveDir != -1)
	{
		if (pPlayer->IsArrive())
		{
			pPlayer->GetPosFromCameraView((CShape::eDIR)lMoveDir,lDestX,lDestY);

			BYTE bBlock = GetGame()->GetRegion()->GetCell(lDestX,lDestY)->bBlock;
			if (bBlock != CRegion::BLOCK_CANFLY || bBlock != CRegion::BLOCK_UNFLY)
			{
				MoveToCellByHotKey(lDestX,lDestY);
				pPlayer->SetLastUpdatePosTime(dwCurTime);
				bIsInMoveState = true;
			}
		}
	}
	else if (bIsInMoveState)
	{
		if (pPlayer->GetAction() == CShape::ACT_RUN || pPlayer->GetAction() == CShape::ACT_WALK)
		{
			pPlayer->SetAction(CShape::ACT_STAND);
			StopMove();
			pPlayer->SetIsArrive(true);
		}
		bIsInMoveState = false;
	}
}

// �����ݼ�����
long CGameControl::ProcessHotKey()
{
	//����WASD�ƶ� add by zhangwei	
	if(IGUIRoot::GetGUIInstance()->GetFocusComp()==NULL)
		HotKeyToMove();
	else if(m_bHotKeyMove){
		if (pMainPlayer->GetAction() == CShape::ACT_RUN || pMainPlayer->GetAction() == CShape::ACT_WALK)
		{
			pMainPlayer->SetAction(CShape::ACT_STAND);
			StopMove();
			pMainPlayer->SetIsArrive(true);
		}
		m_bHotKeyMove = false;
		m_bCurKeyPress = false;
        StopMove();
	}
	//ProcessMoveKey();

	/////////////
	//ʹ�ÿ�ݽ�
	unsigned char Keycode = GetKeyboard()->GetCurrentKey();
	if(Keycode == 0)
		return 0;

	MAPFKEY& mapkey = GetGame()->GetMainPlayer()->GetCurMapKey();
	int nFButton=0;

	if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_313))
		nFButton=1;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_314))
		nFButton=2;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_315))
		nFButton=3;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_316))
		nFButton=4;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_317))
		nFButton=5;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_318))
		nFButton=6;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_319))
		nFButton=7;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_320))
		nFButton=8;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_321))
		nFButton=9;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_322))
		nFButton=10;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_323))
		nFButton=11;
	else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_324))
	{
		nFButton=12;
		if (g_bDebug)
		{
			CSkillListXml::LoadSkillListXml();
			CStateListXml::Load("data/StateXml.xml");
		}
	}
	//switch(Keycode)
	//{
	//case DIK_F1: nFButton=1;break;
	//case DIK_F2: nFButton=2;break;
	//case DIK_F3: nFButton=3;break;
	//case DIK_F4: nFButton=4;break;
	//case DIK_F5: nFButton=5;break;
	//case DIK_F6: nFButton=6;break;
	//case DIK_F7: nFButton=7;break;
	//case DIK_F8: nFButton=8;break;
	//case DIK_F9: nFButton=9;break;
	//case DIK_F10: nFButton=10;break;
	//case DIK_F11: nFButton=11;break;
	//case DIK_F12: 
	//	{
	//		nFButton=12;
	//		if (g_bDebug)
	//		{
	//			CSkillListXml::LoadSkillListXml();
	//			CStateListXml::Load("data/StateXml.xml");
	//		}
	//		break;
	//	}
	//default: break;
	//}

	// ��ǰ�����ڽ���ؼ�����ִ��1~+�Ŀ�ݼ���07.9.21�����޸ģ�
	CComponent *pComp = IGUIRoot::GetGUIInstance()->GetFocusComp();
	if(pComp == NULL)
	{
		if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_301))
			nFButton=13;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_302))
			nFButton=14;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_303))
			nFButton=15;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_304))
			nFButton=16;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_305))
			nFButton=17;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_306))
			nFButton=18;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_307))
			nFButton=19;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_308))
			nFButton=20;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_309))
			nFButton=21;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_310))
			nFButton=22;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_311))
			nFButton=23;
		else if (CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_HOTKEY_312))
			nFButton=24;
		//switch(Keycode)
		//{
		//case DIK_1: nFButton=13;break;
		//case DIK_2: nFButton=14;break;
		//case DIK_3: nFButton=15;break;
		//case DIK_4: nFButton=16;break;
		//case DIK_5: nFButton=17;break;
		//case DIK_6: nFButton=18;break;
		//case DIK_7: nFButton=19;break;
		//case DIK_8: nFButton=20;break;
		//case DIK_9: nFButton=21;break;
		//case DIK_0: nFButton=22;break;
		//case DIK_MINUS: nFButton=23;break;
		//case DIK_EQUALS: nFButton=24;break;
		//default:break;
		//}
	}

	if (nFButton!=0)
	{
		nFButton--;

		return TriggerHotGrid(nFButton);
	}
	return false;
}

// �����������ϼ����ɷ�����Ʒ���߼��ܵ�С��ʹ�õ���Ӧ
bool CGameControl::TriggerHotGrid(int gridIndex)
{
	MAPFKEY& mapkey = GetGame()->GetMainPlayer()->GetCurMapKey();
	CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
	DWORD dwVal=pMainPlayer->GetHotKey(gridIndex);
	// ������ش���ע�� Bugs 2008-9-12
	//unsigned char pPetKey[6] = {DIK_1,DIK_2,DIK_3,DIK_4,DIK_5,DIK_6};
	if ((dwVal&0x80000000)==0)	// ��Ʒ
	{
		if (IsCanUseGoodsCondition(dwVal))
		{
			pMainPlayer->UseItem(dwVal);
			return true;
		}
	}
	else	// ����
	{
		dwVal &= 0x7fffffff;
		CMainPlayerHand* pHand = GetGame()->GetCGuiEx()->GetPMainPlayerHand();
		// ʹ�����＼�ܣ����ϲ��ܵ���Ʒ��������װ����������Ʒ
		if(dwVal >= SKILL_RIDE_START && dwVal <= SKILL_RIDE_END)
		{
			// δװ�������ʾʧ����Ϣ
			if( pMainPlayer->GetEquip(CPlayer::EQUIP_HORSE) == NULL )
			{
				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
					CStringReading::LoadString(eIDS_SKILL,eIDS_ERROR_BYEQHORSE),
					D3DCOLOR_ARGB(255,255,0,0));
				return false;
			}
			if(pHand && pHand->GetPGoodsOfMainPlayerHand() == pMainPlayer->GetEquip(CPlayer::EQUIP_HORSE))
				return false;
		}
		// ������Ӧ�Ŀ�ݼ�
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->UseHotKey(gridIndex,dwVal);
		// ���������޷�ʹ��
		CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(dwVal);
		if (!pSkill)
			return false;
		// ��������
		if (pSkill->dwSkillType == CSkillListXml::SKILLTYPE_PASSIVE)
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_PASSSKILL),
				D3DCOLOR_ARGB(255,255,0,0));
			return false;
		}
		// ��������ҵ�������ܵ�����
		CPlayer::tagSkill *pS = pMainPlayer->GetSkill(dwVal);
		// ��Χ����ħ������������ȴ״̬
		if(pHand && pS && pS->dwCoolDownStartTime <= 0)
		{
			pHand->SetMainPlayerHandByEffect(0,0);
			if(pSkill->dwEffectID > 0)
			{
				pHand->SetMainPlayerHandByEffect(dwVal,pSkill->dwEffectID);
				return false;
			}
		}
		// Ŀ��
		CMoveShape *pAttactObj = NULL;
		if(m_pSelectedShape)
			pAttactObj = dynamic_cast<CMoveShape*>(pRegion->FindChildObject(m_pSelectedShape->GetType(), m_pSelectedShape->GetExID()));
		if(!pAttactObj && m_pCurAttractShape)
			pAttactObj = dynamic_cast<CMoveShape*>(pRegion->FindChildObject(m_pCurAttractShape->GetType(), m_pCurAttractShape->GetExID()));
		// �����Ҫ����Ŀ���ҵ�ǰ������Ŀ�꣬������Ŀ����Ϊ�Լ�
		if (NULL == pAttactObj && pSkill->bLockTarget)
		{
			pAttactObj = GetGame()->GetMainPlayer();
		}
		if(!pS)
			return false;
		// ����Ŀ�꼼�ܣ��������������ܣ�ͬʱ���¡�~���������ͷ�Ŀ�����ó��Լ�����ֹͣĿǰ���Զ�����
		if(!pSkill->bLockSkill && pSkill->bLockTarget && CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_OTHERHOTKEY_FIRESPELLSELF)/*GetKeyboard()->IsKeyDown(DIK_GRAVE)*/)
		{
			AutoAttactStop();
			pAttactObj = GetGame()->GetMainPlayer();
		}
		// ��������ж��������ü������óɻ��弼��
		if(pSkill->bHasAct)
		{
			if(GetMPActionMode() == MPAM_AutoMove)
				AutoMoveStop();
			// �ﵽ�ͷ�����
			long x = 0,y = 0;
			if(pAttactObj)
			{
				x = pAttactObj->GetTileX();
				y = pAttactObj->GetTileY();
			}
			CSkillXml skill;
			// �ͷ���������
			if(skill.StepSend(dwVal,pS->wLevel,pMainPlayer,pAttactObj,x,y))
			{
				//���弼��
				SetBufferSkillID(dwVal);
				// ��Ŀ��ʹ�ü���
				StartAttack(pAttactObj);
			}
			else
				pS->dwUseingStartTime = 0;
		}
		// �޶����ļ���ֱ���ͷ�
		else if(IsActWithCurAction() || 
			pMainPlayer->GetAction() == CShape::ACT_RUN ||
			pMainPlayer->GetAction() == CShape::ACT_WALK)
		{
			if(!StartSkill(dwVal,pAttactObj) && pSkill->bLockSkill)
				pS->dwUseingStartTime = 0;
		}
		SetLastPressHotKey(GetKeyboard()->GetCurrentKey());
		return true;
	}
	return false;
}

// ׷����ǰ���ڱ����������AI
CGameControl::eAAMRet CGameControl::AutoAttackMoveAI()
{
	if(NULL == m_pCurAttractShape)	return AAMR_Stop;
	// ����Ŀ�꼼�ܣ������жϺ�����Ŀ��ľ���ʹ��
	// �л��弼�ܵĻ����ͷŻ��弼��
	if(GetBufferSkillID() != 0)
		m_dwCurSkillID = GetBufferSkillID();
	if(m_dwCurSkillID == 0)
		return AAMR_Stop;
	// �ҵ�������Ϣ
	CPlayer::tagSkill * pSkill = pMainPlayer->GetSkill(m_dwCurSkillID);
	if (pSkill == NULL)
		return AAMR_Stop;
	CSkillListXml::tagSkill *pS = CSkillListXml::GetProperty(m_dwCurSkillID);
	if (pS == NULL)
		return AAMR_Stop;
	// ���������Ŀ��,ֱ�ӷ���Ϊ����ģʽ
	if(!pS->bLockTarget)
		return AAMR_Attack;
	long lDesX = m_pCurAttractShape->GetTileX();
	long lDesY = m_pCurAttractShape->GetTileY();
	CMoveShape *pShape = dynamic_cast<CMoveShape *>(m_pCurAttractShape);
	if(pShape)
	{
		lDesX = pShape->GetTileX();
		lDesY = pShape->GetTileY();
	}
	// ׷��
	// ���㹥��Ŀ�����Ч��Χ
	long lDistance = pMainPlayer->RealDistance(pShape);
	WORD wSkillRangeMax = pSkill->wAffectRangeMax;
	// ��������Ϸ������ع���ģʽ
	if (lDistance >= pSkill->wAffectRangeMin && lDistance <= wSkillRangeMax)
	{
		return AAMR_Attack;
	}

	long lDestX = pMainPlayer->GetTileX();
	long lDestY = pMainPlayer->GetTileY();
	long lDir = GetLineDir(m_pCurAttractShape->GetPosX(),m_pCurAttractShape->GetPosY()
		,pMainPlayer->GetPosX(),pMainPlayer->GetPosY());
	//�õ�Ŀ����Χ�Ŀո���
	CMoveShape* pAttackShape = dynamic_cast<CMoveShape*>(m_pCurAttractShape);
	if(pAttackShape)
		pRegion->GetLeastShapesCellFromCircularSpace(pAttackShape->GetDestX(),pAttackShape->GetDestY(),
		pAttackShape->GetFigure(CShape::DIR_RIGHT),pAttackShape->GetFigure(CShape::DIR_UP),
		wSkillRangeMax,lDir,lDestX,lDestY);
	//ȡ����һ���ƶ���λ��

	float fDestX = pMainPlayer->GetPosX();
	float fDestY = pMainPlayer->GetPosY();			
	if(GetNextMoveDestCell(lDestX+0.5f,lDestY+0.5f,MAX_MOVE_CELLNUM,fDestX,fDestY))
	{
		MoveToCell(fDestX,fDestY);
	}
	else
		return AAMR_Stop;
	return AAMR_Trace;
}


//�������ܹ���AI
//���أ�-1ֹͣ�Զ�����ģʽ 0׷�� 1����
CGameControl::eAAMRet CGameControl::AutoLockAttackAI()
{
	// ����Ŀ�꼼�ܣ������жϺ�����Ŀ��ľ���ʹ��
	m_dwCurSkillID = GetBufferSkillID();
	if(m_dwCurSkillID == 0 && GetAutoSkillID())
		m_dwCurSkillID = GetAutoSkillID();
	if(m_dwCurSkillID == 0)
		return AAMR_Stop;
	static long lUseSkillTime = 0;
	static DWORD time = timeGetTime();
	if(timeGetTime() - time < 1000)
		++lUseSkillTime;
	else
	{
		time = timeGetTime();
		lUseSkillTime = 0;
		if(g_bDebug)
		{
			char strInfo[512]="";
			_snprintf(strInfo,512,"*******SKILL_USE:%d,time:%d,timeout 1 sec, clear 0******",m_dwCurSkillID,time);
			PutStringToFile("SkillTimesTest",strInfo);
		}
	}
	long lCount = 5;
	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
	if(pSetup)
		lCount = pSetup->lUseSkillTime;
	if(lUseSkillTime > lCount)
		return AAMR_Attack;
	if(g_bDebug)
	{
		char strInfo[512]="";
		_snprintf(strInfo,512,"*******SKILL_USE:%d,1 sec %d times, time:%d*******",m_dwCurSkillID,lUseSkillTime,timeGetTime());
		PutStringToFile("SkillTimesTest",strInfo);
	}
	// ׷��
	CPlayer::tagSkill * pSkill = pMainPlayer->GetSkill(m_dwCurSkillID);
	if (pSkill == NULL)
	{
		return AddSkillByGoods(m_dwCurSkillID);
	}
	CSkillListXml::tagSkill *pS = CSkillListXml::GetProperty(m_dwCurSkillID);
	if (pS == NULL)
		return AAMR_Stop;
	// ��������״̬��������ϼ�������
	if(GetGame()->GetMainPlayer()->GetIntonateKey()!=0)
	{
		DWORD dwID = GetGame()->GetMainPlayer()->GetIntonateState();
		// ���������������ֹͣ
		if(dwID >= SKILL_RIDE_START && dwID <= SKILL_RIDE_END)
			return AAMR_Stop;
		return AAMR_Attack;
	}
	// ������ȴʱ��δ��
	if(pSkill->dwCoolDownStartTime > 0)
	{
		// �ü��ܲ��������������ܣ����ز�ֹͣ����
		if(GetAutoSkillID() != m_dwCurSkillID)
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_COOLTIME),
				D3DCOLOR_ARGB(255,255,0,0));
			// ����ü����ǻ��弼�ܣ���ջ��弼��
			if(GetBufferSkillID() == m_dwCurSkillID)
				SetBufferSkillID(0);
			if(GetAutoSkillID() > 0)
				return AAMR_Attack;
			return AAMR_Stop;
		}
		// ��������
		else
		{
			SetBufferSkillID(SKILL_BASE_BUFF);
			return AAMR_Attack;
		}
	}
	// �ü����������ܣ���Ŀǰ�ֲ���ʹ�������ܣ�����
	if(pS->dwSkillUseType == 1 && !IsCanUsePhysicsSkillCondition())
		return AAMR_Stop;
	// �ü�����ħ�����ܣ���Ŀǰ�ֲ���ʹ��ħ�����ܣ�����
	if(pS->dwSkillUseType == 2 && !IsCanUseMagicSkillCondition())
		return AAMR_Stop;
	// �ü��ܲ���Ҫ����Ŀ��
	if(!pS->bLockTarget)
	{
		// ʩ��ʧ��
		if(!StartSkill(m_dwCurSkillID, NULL))
		{
			if(m_dwCurSkillID == GetBufferSkillID())
				SetBufferSkillID(0);
			else if(m_dwCurSkillID == GetAutoSkillID())
				SetAutoSkillID(0);
		}
		return AAMR_Attack;
	}
	// �й���Ŀ��
	else if( m_pCurAttractShape && m_pCurAttractShape->GetExID()!=CGUID::GUID_INVALID && m_pCurAttractShape->GetType()!=0)
	{
		// ׷������Ŀ��
		CMoveShape* pAttactObj = dynamic_cast<CMoveShape*>(pRegion->FindChildObject(m_pCurAttractShape->GetType(), m_pCurAttractShape->GetExID()));
		if(pAttactObj)
		{
			// Ŀ���������Ҹü��ܲ��ܶ��������Ķ���ʹ��
			if(pAttactObj->IsDied() && !pS->bIsUseToDiedObj)
			{
				SetAutoSkillID(0);
				SetBufferSkillID(0);
				return AAMR_Stop;
			}
			// ���Ƕ��Լ��ͷż��ܣ��жϾ���Ϸ���
			if(pAttactObj != pMainPlayer)
			{
				// ���㹥��Ŀ�����Ч��Χ
				long lDistance = pMainPlayer->RealDistance(pAttactObj);
				WORD wSkillRangeMax = pSkill->wAffectRangeMax;
				WORD wSkillRangeMin = pSkill->wAffectRangeMin;
				if(wSkillRangeMax != 0 || wSkillRangeMin != 0)
				{
					//�����,��С׷�ٷ�Χ��
					if (lDistance > wSkillRangeMax || lDistance < wSkillRangeMin)
					{
						// ׷��
						return AAMR_Trace;
					}
				}
			}
			// �ͷż���
			if(!StartSkill(m_dwCurSkillID, pAttactObj))
			{
				if(m_dwCurSkillID == GetBufferSkillID())
					SetBufferSkillID(0);
				else if(m_dwCurSkillID == GetAutoSkillID())
					SetAutoSkillID(0);
			}
			return AAMR_Attack;
		}
	}
	return AAMR_Stop;
}

// ʹ�ø�ְҵ����
bool CGameControl::UseOccuSkill(DWORD dwIndex,CShape* pTagShape)
{
	// ��������״̬
	if(pMainPlayer->GetIntonateKey()!=0)
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_INTTIME));
		return false;
	}
	CPlayerTradePageEx* pTradePage = GetGame()->GetCGuiEx()->GetPlayerTradePageEx();
	if (pTradePage!=NULL&&pTradePage->IsOpen())
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_NOTCOLLECT));
		PutDebugString(CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_NOTCOLLECT));
		return false;
	}
	if(pTagShape==NULL)
	{
		PutDebugString(CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_OBJNULL));
		return false;
	}
	pMainPlayer->SetToolsID(dwIndex,((CCollectionGoods*)pTagShape)->GetActionID());
	CMessage msg(MSG_C2S_DOCCUSKILL_USE);
	msg.Add(dwIndex);
	if (dwIndex==CDeputyOccuSystemClient::OB_Facture)
	{
		msg.Add((DWORD)0);
		msg.Add(CGUID::GUID_INVALID);
		msg.Add((DWORD)1);
	}else if(dwIndex==CDeputyOccuSystemClient::OB_Collection||
		dwIndex==CDeputyOccuSystemClient::OB_TaskCollection)
	{
		msg.Add((DWORD)pTagShape->GetType());
		msg.Add(pTagShape->GetExID());
		msg.Add((DWORD)0);
	}
	msg.Send();
	PutDebugString(CStringReading::LoadString(eIDS_CONTROL,eIDS_CONTROL_MSGSEND));
	return true;
}

// ���һ������Ʒ�����ļ���
CGameControl::eAAMRet CGameControl::AddSkillByGoods(DWORD m_dwCurSkillID)
{
	if (pMainPlayer->GetAction() != CShape::ACT_IDLE && 
		pMainPlayer->GetAction() != CShape::ACT_STAND &&
		pMainPlayer->GetAction() != CShape::ACT_RUN &&
		pMainPlayer->GetAction() != CShape::ACT_WALK &&
		pMainPlayer->GetAction() != CShape::ACT_BEATTACK &&
		pMainPlayer->GetAction() != CShape::ACT_BLOCK &&
		pMainPlayer->GetAction() != CShape::ACT_PARRY &&
		pMainPlayer->GetAction() != CShape::ACT_MISS &&
		!IsActWithCurAction())
	{
		return AAMR_Attack;
	}

	// ����ü���Ϊ���弼��,���û��弼��Ϊ0
	if(m_dwCurSkillID == GetBufferSkillID())
		SetBufferSkillID(0);

	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(m_dwCurSkillID);
	if (pSkill == NULL)
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_NOSKILL));
		return AAMR_Stop;
	}

	// ��������״̬
	if(GetGame()->GetMainPlayer()->GetIntonateKey()!=0)
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_INTTIME));
		return AAMR_Attack;
	}

	// ��Ϣ������Ҫ������
	long x = 0,y = 0;
	long lType = 0;
	CGUID guid = CGUID::GUID_INVALID;
	CMoveShape * pTagShape = NULL;
	// ������Ҫ����Ŀ��,�ҹ���Ŀ�����
	if(pSkill->bLockTarget && m_pCurAttractShape)
	{
		pTagShape = (CMoveShape *)m_pCurAttractShape;
		lType = m_pCurAttractShape->GetType();
		guid = m_pCurAttractShape->GetExID();
		x = m_pCurAttractShape->GetTileX();
		y = m_pCurAttractShape->GetTileY();
	}
	// ��������ǲ���Ҫ����Ŀ��ļ���
	else if(!pSkill->bLockTarget)
	{
		float lMouseX = 0.0f, lMouseY = 0.0f;
		pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lMouseX, lMouseY);
		x = lMouseX;
		y = lMouseY;
	}
	// ��Ҫ����Ŀ�꣬Ŀ���ֲ����ڣ�����ʧ��
	else
		return AAMR_Stop;

	CMessage msg(MSG_C2S_SKILL_USE);
	msg.Add(m_dwCurSkillID);
	msg.Add( lType );
	msg.Add( guid );
	msg.Add( x );
	msg.Add( y );
	msg.Add((BYTE)1);	
	msg.Add((long)m_lSkillGoodsExtendID);
	msg.Add((long)m_lSkillGoodsPos);

	msg.Send();

	m_lSkillGoodsExtendID = 0;
	m_lSkillGoodsPos = -1;

	SetSendedUseSkillMsgCount(GetSendedUseSkillMsgCount()+1);

	return AAMR_Stop;
}

// ��ָ��Ŀ�귢����
bool CGameControl::StartSkill(DWORD dwIndex, CShape* pAttackShape)
{
	// Ҫ����ļ���
	DWORD dwSkillID = dwIndex;

	// ȷ��������������
	if(dwIndex >= SKILL_BASE_BUFF && dwIndex <= SKILL_BASE_BLADEFIST)
		dwSkillID = GetGame()->GetMainPlayer()->GetDefaultAttackSkillID();


	CPlayer::tagSkill *pS = pMainPlayer->GetSkill(dwSkillID);
	if(pS == NULL)
		return false;

	CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(dwSkillID);
	if (pSkill == NULL)
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_NOSKILL));
		return false;
	}
	// �ü����������ܣ���Ŀǰ�ֲ���ʹ�������ܣ�����
	if(pSkill->dwSkillUseType == 1 && !IsCanUsePhysicsSkillCondition())
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_CANNOTPHY));
		return false;
	}
	// �ü�����ħ�����ܣ���Ŀǰ�ֲ���ʹ��ħ�����ܣ�����
	if(pSkill->dwSkillUseType == 2 && !IsCanUseMagicSkillCondition())
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_CANNOTMAGIC));
		return false;
	}
	// ������ȴʱ��δ��
	if(pS->dwCoolDownStartTime > 0)
	{
		if(GetAutoSkillID() != m_dwCurSkillID)
		{
			GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
				CStringReading::LoadString(eIDS_SKILL,eIDS_USE_UNSUCCESS_COOLTIME),
				D3DCOLOR_ARGB(255,255,0,0));
			// ����ü����ǻ��弼�ܣ���ջ��弼��
			if(GetBufferSkillID() == m_dwCurSkillID)
				SetBufferSkillID(0);
		}
		// ��������
		else
			SetBufferSkillID(SKILL_BASE_BUFF);
		return false;
	}
	// ��Ϣ������Ҫ������
	long x = 0,y = 0;
	long lType = 0;
	CGUID guid = CGUID::GUID_INVALID;
	CMoveShape * pTagShape = NULL;
	// ������Ҫ����Ŀ��,�ҹ���Ŀ�����
	if(pSkill->bLockTarget && pAttackShape)
	{
		pTagShape = (CMoveShape *)pAttackShape;
		lType = pAttackShape->GetType();
		guid = pAttackShape->GetExID();
		x = pAttackShape->GetTileX();
		y = pAttackShape->GetTileY();
	}
	// ��������ǲ���Ҫ����Ŀ��ļ���
	else if(!pSkill->bLockTarget)
	{
		// �ͷŵ��ǵ�ǰ�ķ�Χ����
		if(pSkill->dwSkillID == m_stCurAreaSkill.dwSkillID)
		{
			x = m_stCurAreaSkill.fPosX;
			y = m_stCurAreaSkill.fPosY;
			m_stCurAreaSkill.dwSkillID = 0;
		}
		else
		{
			float lMouseX = 0.0f, lMouseY = 0.0f;
			pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lMouseX, lMouseY);
			x = lMouseX;
			y = lMouseY;
			//�����Ϸ������С��ʱ��ͬʱ�ڹһ����Լ���λ�ӻ���ʹ�ü���λ��   ��־  11-4-27
			if (m_pAAHook->GetbSelectAssistSkill()&&(x==0||y==0))
			{
				CPlayer*	 pPlayer= GetGame()->GetMainPlayer();
				x = pPlayer->GetTileX();
				y = pPlayer->GetTileY();
			}
		}
	}
	// ��Ҫ����Ŀ�꣬Ŀ���ֲ����ڣ�����ʧ��
	else
		return false;
	// �жϸü����Ƿ���Զ�Ŀ��ʹ��
	bool bIsUse = false;
	CSkillXml *m_pSkill = new CSkillXml();
	if(m_pSkill->StepSend(dwSkillID,pS->wLevel,pMainPlayer,pTagShape,x,y))
	{
		// ʹ�õ���������������,�Ҵ���ļ��ܲ��ǻ������漼��
		if (pSkill->bLockSkill && dwIndex != SKILL_BASE_BUFF)
		{
			// �������������ܲ���֮ǰ�ģ��滻������������
			if(dwSkillID != GetAutoSkillID())
			{
				SetAutoSkillID(dwSkillID);
			}
		}
		long lCount = 3;
		CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
		if(pSetup)
			lCount = pSetup->lUseSkillMsgCount;
		// ��δ�յ���������Ӧʱ���ۼƷ��ʹ���
		if(GetSendedUseSkillMsgCount() < lCount)
		{
			// �⻷�༼���Ҹü�������ʹ�ã����ͽ�����Ϣ
			if(pMainPlayer->IsHaveSkillByIndex(dwSkillID) && !pSkill->bLockSkill && pSkill->dwSkillState != 0)
			{
				CMessage msg(MSG_C2S_SKILL_USE_END);
				msg.Add((LONG)-1);
				msg.Add((LONG)dwSkillID);
				msg.Send();
			}
			else
			{
				float lMouseX = 0.0f, lMouseY = 0.0f;
				pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lMouseX, lMouseY);
				CMessage msg(MSG_C2S_SKILL_USE);
				msg.Add(dwSkillID);
				msg.Add( lType );
				msg.Add( guid );
				msg.Add( x );
				msg.Add( y );
				msg.Add((BYTE)0);
				msg.Send();
				if(g_bDebug)
				{
					char strInfo[512]="";
					_snprintf(strInfo,512,"========SKILL_USE:%d,%d times=========",dwSkillID,GetSendedUseSkillMsgCount());
					PutStringToFile("SkillTimesTest",strInfo);
				}
			}
			SetSendedUseSkillMsgCount(GetSendedUseSkillMsgCount()+1);
			// ����ü���Ϊ���弼��,���û��弼��Ϊ0
			if(dwIndex == GetBufferSkillID())
				SetBufferSkillID(0);
		}
		bIsUse = true;
	}
	SAFEDELETE(m_pSkill);
	return bIsUse;
}

// ��ָ��Ŀ�귢����
bool CGameControl::StartSkill(DWORD dwIndex, long lType, const CGUID& ID)
{
	CMoveShape* pShape = (CMoveShape*)pRegion->FindChildObject(lType, ID);
	return StartSkill(dwIndex,pShape);
}

//////////////////////////////////////////////////////////////////////////
//	�Զ��ܲ� / �Զ�����
//////////////////////////////////////////////////////////////////////////

// ����
long CGameControl::FollowObjectAI()
{
	CShape* pShape = dynamic_cast<CShape*>(pRegion->FindChildObject(m_lFollowingObjType, m_FollowingObjID));
	if (pShape)
	{
		if(m_lStartDestX != pShape->GetTileX() || m_lStartDestY != pShape->GetTileY())
		{
			long lDestX = pMainPlayer->GetTileX();
			long lDestY = pMainPlayer->GetTileY();
			// ��Ŀ������Ѿ����������Χ֮�ڣ����ٽ���
			if ( abs(lDestX-pShape->GetTileX()) <= m_lFollowMaxDistance && 
				abs(lDestY-pShape->GetTileY()) <= m_lFollowMaxDistance )
			{
				return 0;
			}
			long lDir = GetLineDir(pMainPlayer->GetTileX(),pMainPlayer->GetTileY()
				,pShape->GetTileX(),pShape->GetTileY());
			pRegion->GetLeastShapesCellFromSpace(pShape->GetTileX(),pShape->GetTileY(),
				0,0,m_lFollowMaxDistance,lDir,lDestX,lDestY);

			m_fDestX = lDestX + 0.5f;
			m_fDestY = lDestY + 0.5f;
			m_lStartDestX = pShape->GetTileX();
			m_lStartDestY = pShape->GetTileY();
		}
		long lX = pMainPlayer->GetTileX();
		long lY = pMainPlayer->GetTileY();
		//���Ŀ�����Shape,���Һ�Shape֮��ľ���Ϊ1�Ǿ�ֹͣ�ƶ�
		// ����Ŀ�����ֹ꣬ͣ�Զ��ƶ�
		if ( abs(lX-(long)m_fDestX) < 1 && abs(lY-(long)m_fDestY) < 1 )
		{
			return 0;
		}
		float fDestX = pMainPlayer->GetPosX();
		float fDestY = pMainPlayer->GetPosY();			
		if(GetNextMoveDestCell(m_fDestX,m_fDestY,MAX_MOVE_CELLNUM,fDestX,fDestY))
		{
			MoveToCell(fDestX,fDestY);
			return 1;
		}
	}
	return 0;
}

//�������Ľ��
void CGameControl::DoFollowResult()
{
	CShape* pShape = dynamic_cast<CShape*>(pRegion->FindChildObject(m_lFollowingObjType, m_FollowingObjID));
	if (pShape)
	{
		switch(pShape->GetType())
		{
		case TYPE_MONSTER:
			{
				SendTouchNpcMes(pShape->GetExID());
			}
			break;
		case TYPE_GOODS:
			{
				PickupGoods(dynamic_cast<CGoods*>(pShape));
			}
			break;
		case TYPE_COLLECTION:
			{
				DoCollect(dynamic_cast<CCollectionGoods*>(pShape));
			}
			break;
		case TYPE_PLAYER:
			break;
		}
	}
	else	// ����Ŀ�궪ʧ��ȡ������
		SetFollowingObj(0,CGUID::GUID_INVALID);
}
//////////////////////////////////////////////////////////////////////////
//	�Զ�Ѱ·
//////////////////////////////////////////////////////////////////////////

// �Զ�����(A*)
#ifdef MOVE_IN_TILE
void CGameControl::AutoMoveTo(float fDestX, float fDestY)
{
	// ���֮ǰ�߹��ĸ��Ӽ�¼
	m_MovedCell.clear();
	//�����ǰ�����Զ�����ģʽ�У�����ֹͣ�Զ�����
	if(GetMPActionMode() == MPAM_AutoAttack)
	{
		//ֹͣ�Զ�����
		AutoAttactStop();
	}

	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoMove)
		SetMPNexActionMode(MPAM_AutoMove);

	// ��ʼ�ƶ�(վ���򱻹��������ܡ��񵲡��м�״̬�¿��ƶ�)
	m_fDestX = fDestX;
	m_fDestY = fDestY;
	DWORD dwOld = timeGetTime();
	if(m_AutoMoveMode == AMM_A_SEARCH)
	{
		GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), (long)fDestX, (long)fDestY);
		// Ѱ·
		if(GetFindPath()->GetPathList()->size() > 0)
			GetFindPath()->GetPathList()->pop_front();	// ȥ����һ����
	}
	else
	{
		m_AutoMoveMode= AMM_A_SEARCH;
		// Ѱ·
		GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), (long)fDestX, (long)fDestY);
		if(GetFindPath()->GetPathList()->size() > 0)
			GetFindPath()->GetPathList()->pop_front();	// ȥ����һ����,��Ϊ�����Լ�վ�ĵ�
	}
	SetFollowingObj(0,CGUID::GUID_INVALID);
	// �����ڴ��ͼ����ʾ��Ŀ����ʾ
	CMiniMap *pMiniMap = GetGame()->GetRegion()->GetMiniMap();
	if(pMiniMap)
	{
		pMiniMap->SetDestPos((int)fDestX,(int)fDestY);
	}
	// ��¼�Զ��ƶ�����Ϣ
	CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
	if(pMainPage && pMainPlayer)
		pMainPage->SetMoveInfo(pMainPlayer->GetRegionID(),(long)fDestX,(long)fDestY);
}
#else
void CGameControl::AutoMoveTo(long lDestX, long lDestY)
{
	// ���֮ǰ�߹��ĸ��Ӽ�¼
	m_MovedCell.clear();
	//�����ǰ�����Զ�����ģʽ�У�����ֹͣ�Զ�����
	if(GetMPActionMode() == MPAM_AutoAttack)
	{
		//ֹͣ�Զ�����
		AutoAttactStop();
	}

	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoMove)
		SetMPNexActionMode(MPAM_AutoMove);

	// ��ʼ�ƶ�(վ���򱻹��������ܡ��񵲡��м�״̬�¿��ƶ�)
	m_fDestX = lDestX;
	m_fDestY = lDestY;
	DWORD dwOld = timeGetTime();
	if(m_AutoMoveMode == AMM_A_SEARCH)
	{
		GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), lDestX, lDestY);
		// Ѱ·
		if(GetFindPath()->GetPathList()->size() > 0)
			GetFindPath()->GetPathList()->pop_front();	// ȥ����һ����
	}
	else
	{
		m_AutoMoveMode= AMM_A_SEARCH;
		// Ѱ·
		GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), lDestX, lDestY);
		if(GetFindPath()->GetPathList()->size() > 0)
			GetFindPath()->GetPathList()->pop_front();	// ȥ����һ����,��Ϊ�����Լ�վ�ĵ�
	}
	SetFollowingObj(0,CGUID::GUID_INVALID);
	// �����ڴ��ͼ����ʾ��Ŀ����ʾ
	CMiniMap *pMiniMap = GetGame()->GetRegion()->GetMiniMap();
	if(pMiniMap)
	{
		pMiniMap->SetDestPos(lDestX,lDestY);
	}
	// ��¼�Զ��ƶ�����Ϣ
	CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
	if(pMainPage && pMainPlayer)
		pMainPage->SetMoveInfo(pMainPlayer->GetRegionID(),lDestX,lDestY);
}
#endif

// ��ʼ�Զ�����,ֱ����Ŀ�귽���ƶ�
#ifdef MOVE_IN_TILE
void CGameControl::AutoMoveToEx(float fDestX, float fDestY)
{
	m_fDestX = fDestX;
	m_fDestY = fDestY;
	// ���֮ǰ�߹��ĸ��Ӽ�¼
	m_MovedCell.clear();
	// ���A*Ѱ··��
	CRegionList::GetRegionPathList().clear();
	m_lTargetRegionID = 0;
	if(m_AutoMoveMode == AMM_A_SEARCH)
	{
		// �Զ��ƶ�ͼ������
		CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		if(pMainPage)
			pMainPage->MovePicFlick(true);
	}	
	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoMove)
		SetMPNexActionMode(MPAM_AutoMove);
	else//�����ƶ�ģʽ
		SetResetAutoMoveMode(true);
	m_AutoMoveMode = AMM_TO_DESTDIR;
	SetFollowingObj(0,CGUID::GUID_INVALID);
}
#else
void CGameControl::AutoMoveToEx(long lDestX, long lDestY)
{
	if(lDestX == pMainPlayer->GetTileX() &&
		lDestY == pMainPlayer->GetTileY())
		return;
	long lX = lDestX;
	long lY = lDestY;

	if( pRegion->GetCellObjNum(lX,lY) > 0 || 
		(pRegion->GetBlock(lX,lY) != CRegion::BLOCK_NO && pRegion->GetBlock(lX,lY) != CRegion::BLOCK_AIM))
	{
		long lDir = GetLineDir(pMainPlayer->GetTileX(),pMainPlayer->GetTileY()
			,lDestX,lDestY);
		pRegion->GetLeastShapesCellFromCircularSpace(lDestX,lDestY,0,0,1,lDir,lX,lY);
	}

	m_fDestX = lX + 0.5f;
	m_fDestY = lY + 0.5f;
	// ���֮ǰ�߹��ĸ��Ӽ�¼
	m_MovedCell.clear();
	// ���A*Ѱ··��
	CRegionList::GetRegionPathList().clear();
	m_lTargetRegionID = 0;
	if(m_AutoMoveMode == AMM_A_SEARCH)
	{
		// �Զ��ƶ�ͼ������
		CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		if(pMainPage)
			pMainPage->MovePicFlick(true);
	}	
	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoMove)
		SetMPNexActionMode(MPAM_AutoMove);
	else//�����ƶ�ģʽ
		SetResetAutoMoveMode(true);
	m_AutoMoveMode = AMM_TO_DESTDIR;
	SetFollowingObj(0,CGUID::GUID_INVALID);
}
#endif



// �Զ�ʰȡ��Ʒ
void CGameControl::AutoPickGood(const CGUID& guGoodID)
{
	//�ƶ���Ϊģʽ
	if(GetMPActionMode() == MPAM_AutoMove)
		AutoMoveStop();
	else if(GetMPActionMode() == MPAM_AutoAttack && !( GetAutoAttackMode()&AAM_STOP) )
		AutoAttactStop();
	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoPickGoods)
		SetMPNexActionMode(MPAM_AutoPickGoods);
	m_PickGoodsID = guGoodID;
	return;
}

// ����һ��Ŀ�����
void CGameControl::FollowObject(long lType,const CGUID& guid,long lMaxDistance)
{
	// �����ƶ�
	if(!IsCanMoveCondition())
		return;
	CShape* pShape = dynamic_cast<CShape*>(pRegion->FindChildObject(lType, guid));
	if (NULL == pShape)	return;

	SetFollowingObj(lType,guid);
	m_lFollowMaxDistance = lMaxDistance;
	//�ƶ���Ϊģʽ
	if(GetMPActionMode() != MPAM_AutoMove)
		SetMPNexActionMode(MPAM_AutoMove);
	else//�����ƶ�ģʽ
		SetResetAutoMoveMode(true);
	m_AutoMoveMode = AMM_FOLLOW_OBJECT;


	long lDestX = pMainPlayer->GetTileX();
	long lDestY = pMainPlayer->GetTileY();
	long lDir = GetLineDir(pMainPlayer->GetTileX(),pMainPlayer->GetTileY()
		,pShape->GetTileX(),pShape->GetTileY());
	pRegion->GetLeastShapesCellFromSpace(pShape->GetTileX(),pShape->GetTileY(),
		0,0,lMaxDistance,lDir,lDestX,lDestY);

	m_fDestX = lDestX + 0.5f;
	m_fDestY = lDestY + 0.5f;
	m_lStartDestX = pShape->GetTileX();
	m_lStartDestY = pShape->GetTileY();
}

// ��ʼ���������ù���ģʽ
void CGameControl::StartAttack(CShape* pShape)
{
	SetCurAttractShape(pShape);
	long x=0,y=0;
	string str="";
	if(pShape)
	{
		x = pShape->GetTileX();
		y = pShape->GetTileY();
		str = pShape->GetName();
	}
	if(GetMPActionMode() != MPAM_AutoAttack)
	{
		SetMPNexActionMode(MPAM_AutoAttack);
		m_AutoAttactMode = AAM_ATTACT;
	}
}

// �����м䱻�жϺ�����Զ�����
#ifdef MOVE_IN_TILE
long CGameControl::ContinueMoveTo(float fDestX, float fDestY)
{
	//����·���㶼�����ˣ�Ŀ��㻹�����赲���ƶ�ֹͣ
	m_fDestX = fDestX;
	m_fDestY = fDestY;

	// Ѱ·
	GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), (long)fDestX, (long)fDestY);

	// ȥ����һ����,��Ϊ�����Լ�վ�ĵ�
	if(GetFindPath()->GetPathList()->size() > 0)
		GetFindPath()->GetPathList()->pop_front();
	if(GetFindPath()->GetPathList()->size() == 0)
		return 0;
	return 1;
}
#else
long CGameControl::ContinueMoveTo(long lDestX, long lDestY)
{
	//����·���㶼�����ˣ�Ŀ��㻹�����赲���ƶ�ֹͣ
	m_fDestX = lDestX;
	m_fDestY = lDestY;

	// Ѱ·
	GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), lDestX, lDestY);

	// ȥ����һ����,��Ϊ�����Լ�վ�ĵ�
	if(GetFindPath()->GetPathList()->size() > 0)
		GetFindPath()->GetPathList()->pop_front();
	if(GetFindPath()->GetPathList()->size() == 0)
		return 0;
	return 1;
}
#endif

//===========================08.5.8liuke=============================
// �糡����ļ���Ѱ·
void CGameControl::ContinueMoveToOtherRegion()
{
	pMainPlayer = GetGame()->GetMainPlayer();
	pRegion = GetGame()->GetRegion();
	if(!pRegion || !pMainPlayer)
		return;
	// �Ѿ��������һ��Ŀ�곡��
	if ( GetTargetRegionID() != 0 && GetTargetRegionID() == pMainPlayer->GetRegionID() )
	{
		if(m_lTPosX != 0 && m_lTPosY != 0)
		{
			//�ƶ���Ϊģʽ
			if(GetMPActionMode() != MPAM_AutoMove)
				SetMPNexActionMode(MPAM_AutoMove);
			m_AutoMoveMode= AMM_A_SEARCH;
			m_fDestX = m_lTPosX;
			m_fDestY = m_lTPosY;
			GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), m_fDestX, m_fDestY);
			CRegionList::GetRegionPathList().clear();
		}
	}
	// �����ڵ��б����
	else if (!CRegionList::GetRegionPathList().empty())
	{
		//�ƶ���Ϊģʽ
		if(GetMPActionMode() != MPAM_AutoMove)
			SetMPNexActionMode(MPAM_AutoMove);
		m_AutoMoveMode= AMM_A_SEARCH;
		// �������ң�ֹͣ�ƶ�
		CRegionList::tagRegionPathInfo stPathInfo = CRegionList::GetRegionPathList().front();
		if(stPathInfo.lRegionID != pMainPlayer->GetRegionID())
		{
			SetMPActionMode(MPAM_AutoMove);
			AutoMoveStop();
			SetMPNexActionMode(MPAM_No);
			// �����ڴ��ͼ����ʾ��Ŀ����ʾ
			CMiniMap *pMiniMap = GetGame()->GetRegion()->GetMiniMap();
			if(pMiniMap)
				pMiniMap->SetDestPos();
			return;
		}
		// ȡ����һ�������ڵ��Ŀ��������Ϣ
		else
		{
			CRegionList::GetRegionPathList().erase(CRegionList::GetRegionPathList().begin());
			if (!CRegionList::GetRegionPathList().empty())
			{
				// Ѱ·
				CRegionList::tagRegionPathInfo stPathInfo = CRegionList::GetRegionPathList().front();
				m_fDestX = stPathInfo.stPos.x;
				m_fDestY = stPathInfo.stPos.y;
				GetFindPath()->AddPathPos(pRegion->GetResourceID(),pMainPlayer->GetTileX(), pMainPlayer->GetTileY(), m_fDestX, m_fDestY);
			}
			else
			{
				SetMPActionMode(MPAM_AutoMove);
				AutoMoveStop();
				SetMPNexActionMode(MPAM_No);
				CMiniMap *pMiniMap = GetGame()->GetRegion()->GetMiniMap();
				if(pMiniMap)
					pMiniMap->SetDestPos();
				return;
			}
		}
	}
	else
		return;
	// �����ڴ��ͼ����ʾ��Ŀ����ʾ
	CMiniMap *pMiniMap = GetGame()->GetRegion()->GetMiniMap();
	if(pMiniMap)
	{
		pMiniMap->SetDestPos((int)m_fDestX,(int)m_fDestY);
	}
}
//===================================================================

void CGameControl::AutoMoveStop()
{
	if(m_MainPlayerActionMode == MPAM_AutoMove)
	{
		m_AutoMoveMode = m_AutoMoveMode|AMM_STOP;
		CRegionList::GetRegionPathList().clear();
		m_lTargetRegionID = 0;

		DWORD dwCurTime = GetCurTickCount();
		CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		if(pMainPage)
			pMainPage->MovePicFlick(true);

#ifdef MOVE_IN_TILE
		SetResetAutoMoveMode(true);
#endif

	}
}

// ��ֹ�Զ�����
void CGameControl::AutoAttactStop()
{
	if(m_MainPlayerActionMode == MPAM_AutoAttack)
	{
		m_AutoAttactMode = m_AutoAttactMode|AAM_STOP;
		SetAutoSkillID(0);
		SetBufferSkillID(0);
	}
}

// ��ֹ�Զ�ʰȡ��Ʒ
void CGameControl::AutoPickGoodStop()
{
	if(m_MainPlayerActionMode == MPAM_AutoPickGoods)
	{
		m_PickGoodsID = NULL_GUID;
	}
}

// ����A*Ѱ·���Զ��ƶ�AI
long CGameControl::AutoMoveAI()
{
	// �Ѿ���Ŀ�����
	long lX = pMainPlayer->GetTileX();
	long lY = pMainPlayer->GetTileY();
	long lDistance = 2;

	// Ѱ·��Ŀ�곡�����ڵ�ǰ������ڳ���,��Ŀ�곡��Ϊ0����ʾ�����ڿ糡���ƶ��������ѵ���Ŀ���
	long lCurRegion = GetTargetRegionID();
	if (abs(lX-m_fDestX) <= 2 && abs(lY-m_fDestY) <= 2)
	{
		// ��¼�Զ��ƶ�����Ϣ
		CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		if(pMainPage)
		{
			CMainBarPageEx::stMoveInfo stInfo = pMainPage->GetMoveInfo();
			if(stInfo.lTarRegionID == pMainPlayer->GetRegionID())
				pMainPage->SetMoveInfo(0,0,0);
		}
		// Ŀ�����monster
		CMonster* pMonster = dynamic_cast<CMonster*>(pRegion->GetShape(m_fDestX, m_fDestY));
		if (pMonster)
		{
			// ��npc���Զ�����Ի�
			if(pMonster->IsCanTalk())
			{
				SendTouchNpcMes(pMonster->GetExID());
			}
			return 0;
		}
		else
		{
			// Ŀ�������Ʒ��ʰȡ
			CGoods* pGoods = dynamic_cast<CGoods*>(pRegion->GetShape(m_fDestX, m_fDestY));
			if (pGoods)
			{
				PickupGoods(pGoods);
				return 0;
			}
			// ����Ŀ���
			else if(abs(lX-m_fDestX) < 1 && abs(lY-m_fDestY) < 1)
				return 0;
		}
	}
	// ��·����
	if (GetFindPath()->GetPathList()->size())
	{
		// �����һ������
		list<CFindPath::stPos>::iterator it = GetFindPath()->GetPathList()->begin();
		long lDX = (*it).x;
		long lDY = (*it).y;

		// ����ˣ�����Ѱ·
		if (pRegion->GetBlock(lDX, lDY) != CRegion::BLOCK_NO 
			&& pRegion->GetBlock(lDX, lDY) != CRegion::BLOCK_AIM)
		{
			//�������Ŀ����ӣ�ֹͣ�ƶ�
			ContinueMoveTo(m_fDestX, m_fDestY);
			return 1;
		}

		int dir = GetLineDir(lX, lY, lDX, lDY);

		GetFindPath()->GetPathList()->pop_front();
		lDistance = 1;
		long lDX2 = lDX;
		long lDY2 = lDY;
		// ����ڶ�������
		if (GetFindPath()->GetPathList()->size())
		{
			list<CFindPath::stPos>::iterator it2 = GetFindPath()->GetPathList()->begin();
			lDX2 = (*it2).x;
			lDY2 = (*it2).y;
			int dir2 = GetLineDir(lDX, lDY, lDX2, lDY2);

			// ������ͬ
			if (dir == dir2)
			{
				// ����ˣ�����Ѱ·
				if (pRegion->GetBlock(lDX2, lDY2) != CRegion::BLOCK_NO 
					&& pRegion->GetBlock(lDX2, lDY2) != CRegion::BLOCK_AIM)
				{				
					ContinueMoveTo(m_fDestX, m_fDestY);
					return 1;
				}
				lDistance = 2;
				GetFindPath()->GetPathList()->pop_front();
			}
		}
		if(lDistance == 1)
			MoveToCell(lDX+0.5f,lDY+0.5f);
		else
			MoveToCell(lDX2+0.5f,lDY2+0.5f);
		pMainPlayer->m_lStopCount = 0;
	}
	else	// ·��������,��δ����Ŀ��,����Ѱ·
	{
		// ���Ŀ����ǲ������ߵģ�ֹͣ�ƶ���Ѱ·
		if(ContinueMoveTo(m_fDestX, m_fDestY) == 0)
		{
			// ��¼�Զ��ƶ�����Ϣ
			CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			if(pMainPage)
				pMainPage->SetMoveInfo(0,0,0);
			return 0;
		}
	}
	return 1;
}

CFindPath* CGameControl::GetFindPath()
{
	return pRegion->GetFindPath();
}

void CGameControl::SetFollowingObj(long lType,const CGUID& guid)
{
	m_lFollowingObjType = lType;
	m_FollowingObjID = guid;
	m_lStartDestX = 0;
	m_lStartDestY = 0;
}

// ȡ��ѡ�е�Ŀ��
void CGameControl::CancelSelectedTarget()
{
	SetSelectedShape(m_pSelectedShape);
}

// ������굱ǰѡ��Ķ���
void CGameControl::SetSelectedShape(CShape* pShape,bool bIsCToS)
{
	//�����GM����״̬����ѡ��
	if(pShape && pShape->GetType() == TYPE_PLAYER && ((CPlayer*)pShape)->GetIsGMConcealment() )
		return;
	//�������������γ裬��ѡ��
	if(pShape && pShape->GetType() == TYPE_PET)
	{
		CPet *pPet = (CPet*)pShape;
		if (pPet->GetPetType() == PET_TYPE_TRAP || pPet->GetPetType() == PET_TYPE_ORNA)
			return;
	}

	// ��֮ǰѡ�ж�����ͬ��ֱ�ӷ���
	if(m_pSelectedShape == pShape)
		return;
	m_pSelectedShape = pShape;
	if(pShape)
	{
		m_lSelectedType = pShape->GetType();
		m_guidSelected = pShape->GetExID();
	}
	else
	{
		m_lSelectedType = 0;
		m_guidSelected = CGUID::GUID_INVALID;
		ClearSelectedList();
	}
	if(bIsCToS)
	{
		if(m_dwSelectedShapeStartTime == 0)
			m_dwSelectedShapeStartTime = timeGetTime();
	}
	else
		m_dwSelectedShapeStartTime = 0;

}

bool CGameControl::GetIntonateBreak()
{
	bool bBreak = m_bIntonateBreak;
	m_bIntonateBreak = false;
	return bBreak;
}

// ����ָ�������µ�ָ��λ�õ�Ŀ�곡����ָ��λ�õ�·��
bool CGameControl::FindPathToOtherRegion(long lSourRegion, long lSourx, long lSoury, long lTargRegion, long lTargx, long lTargy)
{
	// ��ͬ��������������
	if(lSourRegion == lTargRegion)
		return false;
	m_lTargetRegionID = lTargRegion;
	if(!CRegionList::GetRegionPathList().empty())
	{
		m_lSPosX = lSourx;
		m_lSPosY = lSoury;
		m_lTPosX = lTargx;
		m_lTPosY = lTargy;
		// ��ȡ��һ�������ڵ���Ϣ
		CRegionList::tagRegionPathInfo stPathInfo = CRegionList::GetRegionPathList().front();
		// Ѱ��Ҫ����ó�������Ҫ������·����
		AutoMoveTo(stPathInfo.stPos.x,stPathInfo.stPos.y);
		// ��¼�Զ��ƶ�����Ϣ
		CMainBarPageEx * pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		if(pMainPage)
			pMainPage->SetMoveInfo(lTargRegion,lTargx,lTargy);
		return true;
	}
	return false;
}

//������Ϣ�ı��ƶ�ģʽ
void CGameControl::ChangeMoveMode()
{
	CMessage msg(MSG_C2S_SHAPE_CHANGEMOVEMODE);
	msg.Add(static_cast<char>(!pMainPlayer->GetIsRun()));
	msg.Send();		
}

// �����Զ��ͷż��ܵ�ID
void CGameControl::SetAutoSkillID(DWORD dw)
{
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	if(pPlayer == NULL || m_dwAutoSkillID == dw)
		return;
	CPlayer::tagSkill *ptagS = NULL;
	// ֮ǰ���Զ�ʩ�ż��ܿ�����Ϲ�Ȧ���ƵĿ�ʼʱ������Ϊ0����ʾ������
	if(m_dwAutoSkillID > 0)
	{
		ptagS = pPlayer->GetSkill(m_dwAutoSkillID);
		if(ptagS)
			ptagS->dwUseingStartTime = 0;
	}
	m_dwAutoSkillID = dw;
	// ���õ�ǰ���Զ�ʩ�ż��ܿ�����Ϲ�Ȧ���ƵĿ�ʼʱ��
	if(m_dwAutoSkillID > 0)
	{
		ptagS = pPlayer->GetSkill(m_dwAutoSkillID);
		if(ptagS)
			ptagS->dwUseingStartTime = timeGetTime();
	}
}
// ���û��弼��ID
void CGameControl::SetBufferSkillID(DWORD dw)
{
	CPlayer *pPlayer = GetGame()->GetMainPlayer();
	if(pPlayer == NULL || m_dwBufferSkillID == dw)
		return;
	CPlayer::tagSkill *ptagS = NULL;
	// ֮ǰ���Զ�ʩ�ż��ܿ�����Ϲ�Ȧ���ƵĿ�ʼʱ������Ϊ0����ʾ������
	if(m_dwBufferSkillID > 0 && m_dwBufferSkillID != m_dwAutoSkillID)
	{
		if(m_dwBufferSkillID != m_dwAutoSkillID && dw == SKILL_BASE_BUFF)
			return;
		ptagS = pPlayer->GetSkill(m_dwBufferSkillID);
		if(ptagS)
			ptagS->dwUseingStartTime = 0;
	}
	m_dwBufferSkillID = dw;
}

// �Զ�ʰȡ�����Ȧ����Ʒ����Ҫ�Զ��ƶ�����Ʒ��ʰȡ��
void CGameControl::AutoPickGoodEx()
{
	if(pMainPlayer)
	{
		// ���Ϊ���ĵ�ĵڶ�������
		static int PickOrder[16][2] = 
		{
			// ����߿�ʼ��˳ʱ������
			-2,0,	-2,-1,	-2,-2,
			-1,-2,	0,-2,	1,-2,	2,-2,
			2,-1,	2,0,	2,1,	2,2,
			1,2,	0,2,	-1,2,	-2,2,
			-2,1
		};
		for (int i=0; i<16; i++)
		{
			CGoods *pGoods = pRegion->GetShapeGoods(pMainPlayer->GetTileX() + PickOrder[i][0], pMainPlayer->GetTileY() + PickOrder[i][1]);
			if (pGoods)
			{
				// �ܹ�ȥ,ʰȡ���ܵ���Ʒ���󴥷�
				AutoPickGood(pGoods->GetExID());
				return;
			}
		}
	}	
}

// ���úͻ���Ƿ���������ѡ��
void CGameControl::SetIsTabTeamMate(bool bLock)
{
	CMainBarPageEx * pMainbar = GetGame()->GetCGuiEx()->GetMainBarPageEx();
	if(pMainbar)
	{
		// �Ӳ����������л����������ѣ�ȫ������ʧЧ
		if(m_bIsTabTeamMate != bLock && bLock)
		{
			pMainbar->SetTabItem(CMainBarPageEx::eTab_Enemy,false);
			pMainbar->SetTabItem(CMainBarPageEx::eTab_Pirate,false);
			pMainbar->SetTabItem(CMainBarPageEx::eTab_Monster,false);
		}
		m_bIsTabTeamMate = bLock;
		// ���ݱ�Ǹı�Tab��ť������Ϣ
		pMainbar->SetTabText(m_bIsTabTeamMate);
	}
}


//��ʱpingServer����
void CGameControl::PingServer()
{
	if(!pRegion || !pMainPlayer)	return;

	DWORD dwCurTime = timeGetTime();
	if(dwCurTime - m_dwLastPingTime > 60*1000)
	{
		char strInfo[512]="";
		_snprintf(strInfo,512,"curr time : %d\t last ping time : %d\t interval: %d",dwCurTime,m_dwLastPingTime,dwCurTime - m_dwLastPingTime);
		PutStringToFile("PingServer",strInfo);

		m_dwLastPingTime = dwCurTime;
		DWORD dwCRC32 = 0;
		CGUID guid = pRegion->GetExID();
		CCrc32Static::DataCrc32(&guid,sizeof(CGUID),dwCRC32);
		DWORD dwValue = pMainPlayer->GetHotKey(0);
		CCrc32Static::DataCrc32(&dwValue,sizeof(DWORD),dwCRC32);
		dwValue = pMainPlayer->GetHotKey(12);
		CCrc32Static::DataCrc32(&dwValue,sizeof(DWORD),dwCRC32);

		CMessage msg(MSG_C2S_OTHER_PING);		
		msg.Add(dwCRC32);
		msg.Send();

	}
}
//�˷�����ȼ��ƶ�
void CGameControl::HotKeyToMove()
{
	if (!IsCanControlCondition()||pMainPlayer->IsInUnMoveState()==false)
		return ;
	//��ȡ��ǰ������ķ���
	float fCaremaDir = GetGame()->GetRegion()->GetRegionCamera()->GetHoriAngle();
	bool bHotKeyPress = false;
	static DWORD dwStartTime = 0;
	float fDir=0.0f;
	//��ȡ��ǰ���µ��ȼ�
	bHotKeyPress = GetHotKeyDir(fDir);
	if(bHotKeyPress)
	{		
		float fDestX = 0.0f;
		float fDestY = 0.0f;
		bool bCanMove = false;
		static DWORD dwStartTime = 0;
		static DWORD dwRuntime = 0;
		if(pMainPlayer->GetAction() == CShape::ACT_WALK)
		{
			dwRuntime=450;
		}else{
			dwRuntime=250;
		}
		if((timeGetTime()-dwStartTime>dwRuntime))    //�����ƶ�ʱÿ400������һ�ΰ���
		{
			// pMainPlayer->SetDirEx(fDir);
			if(GetMPActionMode() == MPAM_AutoMove)
			{
				//ֹͣ�Զ�����
				AutoMoveStop();

			}			  
			//�����ǰ��ΪģʽΪ�Զ�����,�������Զ�����ģʽ����ֹͣ�Զ�����
			if(GetMPActionMode() == MPAM_AutoAttack && !( GetAutoAttackMode()&AAM_STOP) )
			{
				//ֹͣ�Զ�����
				AutoAttactStop();
			}
			bCanMove = true;
		}//�Ƿ�ﵽ�ƶ���������������﷽���ϵ���һ��Ԫ����
		if(bCanMove&&GetNextMoveDestCell(fDir,MAX_MOVE_CELLNUM,fDestX,fDestY))
		{
			BYTE bBlock = GetGame()->GetRegion()->GetCell(fDestX,fDestY)->bBlock;
			if (bBlock != CRegion::BLOCK_CANFLY || bBlock != CRegion::BLOCK_UNFLY)
			{
				//����ƶ������Ƿ����1
				long lTotalDistance = (long)pMainPlayer->RealDistance(fDestX,fDestY);
				MoveToCell(fDestX,fDestY);				 
				m_bHotKeyMove = true;
				dwStartTime = timeGetTime();
			}		 

		}
	}else if(m_bHotKeyMove) //ֹͣ�����ƶ�
	{
		if (pMainPlayer->GetAction() == CShape::ACT_RUN || pMainPlayer->GetAction() == CShape::ACT_WALK)
		{
			pMainPlayer->SetAction(CShape::ACT_STAND);
			StopMove();
			pMainPlayer->SetIsArrive(true);
		}
		m_bHotKeyMove = false;
		m_bCurKeyPress = false;
	}else{
		m_bCurKeyPress = false;
	}
}
//��ð��������ϵĳ���
bool CGameControl::GetHotKeyDir(float &dir)
{
	if(!m_bCurKeyPress)
	{
		if(GetGame()->GetKeyboard()->GetCurrentKey())
		{
            m_bCurKeyPress = true;
		}
	}
	int iProcessToKey = GetProKey(m_bCurKeyPress);

	float fCaremaDir = GetGame()->GetRegion()->GetRegionCamera()->GetHoriAngle();
	switch(iProcessToKey)
	{
	case 0:  //��
		{
	        dir = fCaremaDir+180* (3.1415926f / 180.0f);
			return  true;
		}
		break;  //��
	case 1:
		{
		    dir = fCaremaDir;
			return true;
		}
		break;
	case 2:    //��
		{
		    dir = fCaremaDir+90* (3.1415926f / 180.0f);
			return true;	
		}
		break;
	case 3:   //��
		{
		    dir = fCaremaDir-90* (3.1415926f / 180.0f);
			return true;	
		}
		break;
	case 4:   //����
		{
		    dir = fCaremaDir+135* (3.1415926f / 180.0f);
			return  true;
		}
		break;
	case 5:   //����
		{
		    dir = fCaremaDir-135* (3.1415926f / 180.0f);
			return true;
		}
		break;  
	case 6:   //����
		{
		    dir = fCaremaDir-45* (3.1415926f / 180.0f);
			return true;
		}
		break;
	case 7:   //����
		{
            dir = fCaremaDir+45* (3.1415926f / 180.0f);
			return true;
		}
		break;
	}
	return false;
}

//�жϰ����Ļ������� ������8������-1��ʾ�ް����ƶ�
int CGameControl::GetProKey(bool bCurKeyPress)
{
	for(int i=0;i<4;i++)
	{
		m_HotDirKey[i] = 0;
	}
	if(bCurKeyPress)
	{
		MAPFKEY& mapkey = GetGame()->GetMainPlayer()->GetCurMapKey();
		if(CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_FORWARD,true)
			||CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_UP,true))
		{
			m_HotDirKey[0] = 1;
		}
		if(CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_GOBACK,true)
			||CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_DOWN,true))
		{
			m_HotDirKey[1] = 1;
		}
		if(CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_GOLEFT,true)
			||CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_LEFT,true))
		{
			m_HotDirKey[2] = 1;
		}
		if(CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_GORIGHT,true)
			||CFuncKeyMapSystem::CheckFuncKey(mapkey,FKMS_MOVEKEY_RIGHT,true))
		{
			m_HotDirKey[3] = 1;
		}
	}
	if(m_HotDirKey[0]==1&&m_HotDirKey[1]==1&&m_HotDirKey[2]==1&&m_HotDirKey[3]==1)
		return -1;
	if(m_HotDirKey[0]==1&&m_HotDirKey[1]==1)
	{
		if(m_HotDirKey[2] == 1)
			return 2;
		else if(m_HotDirKey[3] == 1)
			return 3;
		else if(m_HotDirKey[2]==0&&m_HotDirKey[3]==0)
			return -1;
	}else if(m_HotDirKey[2]==1&&m_HotDirKey[3]==1)
	{
		if(m_HotDirKey[0] == 1)
			return 0;
		else if(m_HotDirKey[1] == 1)
			return 1;
		else if(m_HotDirKey[0]==0&&m_HotDirKey[1]==0)
			return -1;
	}else if(m_HotDirKey[0]==1&&m_HotDirKey[2]==1)
	{
		return 4;
	}else if(m_HotDirKey[0]==1&&m_HotDirKey[3]==1)
	{
		return 5;
	}else if(m_HotDirKey[1]==1&&m_HotDirKey[3]==1)
	{
		return 6;
	}else if(m_HotDirKey[1]==1&&m_HotDirKey[2]==1)
	{
		return 7;
	}else if(m_HotDirKey[0]==1)
	{
		return 0;
	}else if(m_HotDirKey[1]==1)
	{
		return 1;
	}else if(m_HotDirKey[2]==1)
	{
		return 2;
	}else if(m_HotDirKey[3]==1)
	{
		return 3;
	}
	return -1;
}

