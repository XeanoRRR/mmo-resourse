#include "stdafx.h"
#include "Message.h"
#include "Other\TestMsgType.h"
#include "Player.h"
#include "Monster.h"
#include "Clientregion.h"
#include "Other\Audiolist.h"
#include "Other\RegionList.h"
#include "../GameClient/GameControl.h"
#include "Depot.h"
#include "../../Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ģ�������Ϣ
void OnTestMouseMessage( CMessage *pMsg )
{
#ifdef _CLENTTOTEST_
	if(!pMsg)
		return;
	UINT message = 0;
	WPARAM wParam = 0;
	LPARAM lParam = 0;
	long type = pMsg->GetType();
	// �����������
	int x = pMsg->GetLong();
	int y = pMsg->GetLong();
	/*char str1[256]="";
	pMsg->GetStr(str1,256);
	PutDebugString(str1);
	char str2[256]="";
	pMsg->GetStr(str2,256);
	PutDebugString(str2);*/
	switch( type )
	{
	case MSG_T2C_MOUSE_MOVE:						// ����ƶ�
		message = WM_MOUSEMOVE;
		break;
	case MSG_T2C_MOUSE_LBUTTONDOWN:					// �������
		message = WM_LBUTTONDOWN;
		break;
	case MSG_T2C_MOUSE_LBUTTONUP:					// ����ͷ�
		message = WM_LBUTTONUP;
		break;
	case MSG_T2C_MOUSE_LBUTTONDBLCLK:				// ���˫��
		message = WM_LBUTTONDBLCLK;
		break;
	case MSG_T2C_MOUSE_RBUTTONDOWN:					// �Ҽ�����
		message = WM_RBUTTONDOWN;
		break;
	case MSG_T2C_MOUSE_RBUTTONUP:					// �Ҽ��ͷ�
		message = WM_RBUTTONUP;
		break;
	case MSG_T2C_MOUSE_RBUTTONDBLCLK:				// �Ҽ�˫��
		message = WM_RBUTTONDBLCLK;
		break;
	case MSG_T2C_MOUSE_MOUSEWHEEL:					// ����м����
		message = WM_MOUSEWHEEL;
		break;
	default:
		return;
	}
	if(message != 0)
	{
		if(x > 0 && x < SCREEN_WIDTH && y > 0 && y < SCREEN_HEIGHT)
		{
			POINT point;
			point.x = x;
			point.y = y;
			ClientToScreen(g_hWnd,&point);
			SetCursorPos(point.x,point.y);
		}
		PostMessage(g_hWnd,message,0,0);
	}
#endif
}

// ģ�������Ϣ
void OnTestKeyMessage( CMessage *pMsg )
{
#ifdef _CLENTTOTEST_
	if(!pMsg)
		return;
	UINT message = 0;
	WPARAM wParam = 0;
	LPARAM lParam = 0;
	switch( pMsg -> GetType() )
	{
	case MSG_T2C_KEY_DOWN:							// ���̰���
		message = WM_KEYDOWN;
		break;
	case MSG_T2C_KEY_UP:							// �����ͷ�
		message = WM_KEYUP;
		break;
	case MSG_T2C_KEY_CHAR:							// ���̲���
		message = WM_CHAR;
		break;
	default:
		return;
	}
	// �����ļ�ֵ
	wParam = (WPARAM)pMsg->GetByte();
	if(message != 0)
	{
		PostMessage(g_hWnd,message,wParam,0);
	}
#endif
}

// ���Գ�����߼��¼���Ϣ����
void OnTestLogicMessage( CMessage *pMsg )
{
#ifdef _CLENTTOTEST_
	if(!pMsg)
		return;
	switch( pMsg -> GetType() )
	{
	case MSG_T2C_LOGIC_MOVE:							// �ƶ���ĳ�������
		{
			long lRegionID = pMsg->GetLong();
			long x = pMsg->GetLong();
			long y = pMsg->GetLong();
			CPlayer * pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer && GetGame()->GetGameControl())
			{
				long lCurRegionID = pPlayer->GetRegionID();
				// �������ƶ�
				if(lRegionID == lCurRegionID)
					GetGame()->GetGameControl()->AutoMoveTo((float)x,(float)y);
				// �糡���ƶ�
				else
				{
					// ����Ѱ·�ĳ����ڵ�
					CRegionList::FindRegionPath(lCurRegionID,lRegionID);
					// ����Ѱ·
					if(!CRegionList::GetRegionPathList().empty())
					{
						// ��ȡ��һ�������ڵ���Ϣ
						CRegionList::tagRegionPathInfo stPathInfo = CRegionList::GetRegionPathList().front();
						GetGame()->GetGameControl()->FindPathToOtherRegion(lCurRegionID,pPlayer->GetTileX(),pPlayer->GetTileY(),
							lRegionID,x,y);
					}
					//else
					//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddChatWords(CStringReading::LoadText(eIDS_QUEST,eIDS_QUEST_NOTMOVETO),0xffff0000);
				}
			}
		}
		break;
	case MSG_T2C_LOGIC_SETPOS:							// ˲�Ƶ�ĳ�������
		{
			CPlayer * pMainPlayer = GetGame()->GetMainPlayer();
			long x = pMsg->GetLong();
			long y = pMsg->GetLong();
			if(pMainPlayer)
			{
				CMessage msg(MSG_C2S_SHAPE_SETPOS);
				msg.Add(pMainPlayer->GetType());
				msg.Add(pMainPlayer->GetExID());
				msg.Add(x);
				msg.Add(y);
				msg.Send();
			}
			if(GetGame()->GetGameControl())
			{
				GetGame()->GetGameControl()->AutoAttactStop();
				GetGame()->GetGameControl()->AutoMoveStop();
			}
		}
		break;
	case MSG_T2C_LOGIC_SELECTEDSHAPE:					// ѡ��ĳ������
		{
			long type = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CMoveShape* pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(type, guid);
			if(pMoveShape && GetGame()->GetGameControl())
			{
				GetGame()->GetGameControl()->SetSelectedShape(pMoveShape);
			}
		}
		break;
	case MSG_T2C_LOGIC_UIISOPENED:						// ����ͻ��˷���һ��ָ�������Ƿ�������Ϣ
		{
			BYTE bIsOpen = 0;
			//��������
			char pszInterfaceName[256];
			pMsg->GetStr(pszInterfaceName,256);
			// ��������
			if(strcmp(pszInterfaceName,"ItemPage") == 0)
			{
				//if(GetGame()->GetCGuiEx()->GetItemPageEx() && 
				//	GetGame()->GetCGuiEx()->GetItemPageEx()->IsOpen())
					bIsOpen = 1;
			}
			else
			{
				// ���Ҷ�Ӧ��������
				//CGUIComp * pParentComp = IGUIRoot::GetGUIInstance()->GetGUIComp();
				/*if(pParentComp)
				{
					CComponent * pComp = pParentComp->GetComponentByName(pszInterfaceName);
					if(pComp && pComp->IsVisible() && pComp->IsEnabled())
					{
						bIsOpen = 1;
					}
				}*/
			}
			// ����Գ����ͽ��
			CMessage msgtot(MSG_C2T_LOGIC_UIISOPENED);
			msgtot.Add(pszInterfaceName);
			msgtot.Add(bIsOpen);
			msgtot.SendMemToTest();
		}
		break;
		// ����ͻ���ʰȡָ����Ʒ
	case MSG_T2C_LOGIC_PICKGOODS:
		{
			CClientRegion *pRegion = GetGame()->GetRegion();
			CGameControl * pCtl = GetGame()->GetGameControl();
			if(!pRegion || !pCtl)
				break;
			long lNum = pMsg->GetLong();
			for (long i=0; i<lNum; ++i)
			{
				CGUID id;
				pMsg->GetGUID(id);
				CGoods *pGoods = dynamic_cast<CGoods *>(pRegion->FindChildGoods(id));
				if(pGoods)
					pCtl->PickupGoods(pGoods);
			}
		}
		break;
		// ����ͻ�������������ͽű����ù���
	case MSG_T2C_LOGIC_CALLSCREAPT:
		{
			// Ҫ���еĽű�����
			char pszstr[256];
			pMsg->GetStr(pszstr,256);
			CPlayer * pPlayer = GetGame()->GetMainPlayer();
			if(g_bDebug && pPlayer)
			{
				// ������Ϣ
				CMessage msg(MSG_C2S_OTHER_TALK);
				msg.Add((BYTE)0);
				msg.Add((long)RUN_SCRIPT);
				msg.Add(pPlayer->GetType());		// ������type
				msg.Add(pPlayer->GetExID());		// ������ID
				//==========================��Ʒ��Ϣ========================================
				msg.Add((long)0);
				msg.Add((char *)pszstr);
				msg.Send();
			}
		}
		break;
		// ����ͻ�������������Ͳ����Ʒ��Ϣ
	case MSG_T2C_LOGIC_SPLITGOODS:
		{
			CPlayer * pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer == NULL)
				return;
			// ��ƷԴ����������
			long lSorceObjectExtendID = pMsg->GetLong();
			long lSorceObjectPos = pMsg->GetLong();
			// ��ƷĿ������������
			long lDesObjectExtendID = pMsg->GetLong();
			long lDesObjectPos = pMsg->GetLong();
			// ����
			long lObjectNum = pMsg->GetLong();

			CGoods *pGoods = NULL;
			// ��Ʒ���ͺ�ID
			long lObjectType = 0;
			CGUID ObjectGUID = CGUID::GUID_INVALID;
			// �����е���Ʒ
			if(lSorceObjectExtendID == PEI_PACKET || 
				(lSorceObjectExtendID > PEI_SUBPACK &&
				lSorceObjectExtendID >= PEI_PACK5))
			{
				CPlayer::tagGoods *pstGoods = pPlayer->GetGoodsOnPlayer((WORD)lSorceObjectExtendID,(WORD)lSorceObjectPos);
				if(pstGoods)
					pGoods = pstGoods->pGoods;
			}
			// �ֿ�
			else if(IS_DEPOT_CONTAINER(lSorceObjectExtendID))
			{
				pGoods = CGCMana::Inst()->GetGoods((DWORD)lSorceObjectExtendID, (DWORD)lSorceObjectPos);
			}
			// ����Ʒ
			if(pGoods)
			{
				lObjectType = pGoods->GetType();
				ObjectGUID = pGoods->GetExID();
			}
			CMessage msg(MSG_C2S_CONTAINER_OBJECT_MOVE);
			msg.Add(pPlayer->GetType());
			msg.Add(pPlayer->GetExID());
			msg.Add((LONG)lSorceObjectExtendID);
			msg.Add((LONG)lSorceObjectPos);
			msg.Add(pPlayer->GetType());
			msg.Add(pPlayer->GetExID());
			msg.Add( lDesObjectExtendID);
			msg.Add( lDesObjectPos );
			msg.Add((LONG)lObjectType);
			msg.Add(ObjectGUID);
			msg.Add((LONG)lObjectNum);
			msg.Send();
		}
		break;
	default:
		return;
	}
#endif
}