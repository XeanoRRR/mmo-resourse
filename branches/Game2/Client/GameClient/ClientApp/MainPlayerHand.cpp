#include "StdAfx.h"
#include "MainPlayerHand.h"
#include "ClientRegion.h"
#include "Effect.h"
#include "Skills/SkillListXml.h"
#include "../GameClient/Game.h"
#include "../../Input/Mouse.h"
#include "MsgBoxSys/MsgEventManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMainPlayerHand::CMainPlayerHand()
{
	m_lDesExtenID = 0;
	//m_lDesPos = -1;
}


bool CMainPlayerHand::Display(long lGraphicsID,long x, long y, DWORD color /* = 0xffffffff */, RECT *rectClip/* =NULL */)
{
	if (m_Hand.wIconType==CPicList::PT_GOODS_ICON)
	{
		GetGame()->GetPicList()->Display(CPicList::PT_GOODS_ICON, lGraphicsID, x, y);
	}else if (m_Hand.wIconType==CPicList::PT_SKILL_ICON)
	{
		GetGame()->GetPicList()->Display(CPicList::PT_SKILL_ICON, lGraphicsID, x, y);
	}else if (m_Hand.wIconType==CPicList::PT_SKILL_ICON_GRAY)
	{
		// �Ҷȼ���ͼƬ����Ҫ�������ٻ���һ������
		GetGame()->GetPicList()->Display(CPicList::PT_SKILL_ICON, lGraphicsID, x, y);
		GetGame()->DrawSolidQuad(x,y,32,32,0x70000000);
	}
	return true;
}

// ����������ʾ����Ʒ��ͼ��(û��ʵ�ʵ���Ʒ��Ϣ)
void CMainPlayerHand::SetMainPlayerHandByGoodsIndex(DWORD dwGoodsIndex,DWORD dwSourcePlace)
{
	if (dwGoodsIndex!=0)
	{
		SetMainPlayerHandByGoods(false,0,0,NULL);
		m_Hand.dwObjectTypeOnHand = TYPE_GOODS;
		m_Hand.dwGoodsIndex = dwGoodsIndex;
		m_Hand.dwSourcePlace = dwSourcePlace;
		SetMainPlayerHandByEffect(0,0);
	}
}

void CMainPlayerHand::SetMainPlayerHandByGoods(bool bChanged,long lNum,DWORD lPos,CGoods *pGood,DWORD lType)
{
	// ���������ܲ�����Ʒ
	if( pGood )
	{
		if( pGood->IsLocked() )
		{
            GetInst(MsgEventManager).PushEvent(Msg_Ok, AppFrame::GetText("Package_7"));
			return;
		}
	}

	m_Hand.dwObjectTypeOnHand = TYPE_GOODS;
	m_Hand.dwSkillIndex = 0;
	m_Hand.dwGoodsIndex = 0;
	m_Hand.wIconType = CPicList::PT_GOODS_ICON;
	m_Hand.HandGoods.bChange = bChanged;
	m_Hand.HandGoods.lNum = lNum;
	m_Hand.HandGoods.lPos = lPos;
	m_Hand.HandGoods.lType = lType;
	m_Hand.HandGoods.pGoods = pGood;	
	if (pGood!=NULL)
	{
		pGood->SetHaveShadowState(true);
		//CCharacterPageEx* pCharacterPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
		//if (pCharacterPage!=NULL&&pCharacterPage->IsOpen())
		//{
		//	pCharacterPage->ShowEquipRim(true);
		//}
	}
	SetMainPlayerHandByEffect(0,0);
	SetMainPlayerHandByPet(0,NULL);
}

void CMainPlayerHand::SetMainPlayerHandByPet(DWORD dwPetPicID, CPet* pet,WORD wIconType, DWORD dwSourcePlace )
{
	if (pet)
	{
		SetMainPlayerHandByGoods(false,0,0,NULL);
		SetMainPlayerHandBySkill(0,0);
		m_Hand.dwSourcePlace = dwSourcePlace;
		m_Hand.dwGoodsIndex = 0;
		m_Hand.dwObjectTypeOnHand = TYPE_PET;
		m_Hand.wIconType = wIconType;
		SetMainPlayerHandByEffect(0,0);
	}

	m_Hand.dwPetPicID = dwPetPicID;
	m_Hand.handPet = pet;
}

void CMainPlayerHand::SetMainPlayerHandBySkill(DWORD dwSkillIndex,WORD wIconType,DWORD dwSourcePlace)
{
	m_Hand.dwSourcePlace = dwSourcePlace;
	m_Hand.dwGoodsIndex = 0;
	m_Hand.dwObjectTypeOnHand = TYPE_SKILL;
	m_Hand.dwSkillIndex = dwSkillIndex;
	m_Hand.wIconType = wIconType;
	m_Hand.HandGoods.pGoods = NULL;
	m_Hand.HandGoods.bChange= false;
	m_Hand.HandGoods.lNum = 0;
	m_Hand.HandGoods.lPos = 0;
	m_Hand.HandGoods.lType = 0;
	SetMainPlayerHandByEffect(0,0);
}

void CMainPlayerHand::ClearMainPlayerHand()
{
	if (m_Hand.dwObjectTypeOnHand == TYPE_GOODS)
	{
		if( m_Hand.HandGoods.pGoods )
		{
			//CCharacterPageEx* pCharacterPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
			//if (pCharacterPage!=NULL)
			//{
			//	pCharacterPage->ShowEquipRim(false);
			//}
			//SetMainPlayerHandByGoods(false,0,0,NULL);
		}
        else if (m_Hand.dwGoodsIndex!=0)
		{
			m_Hand.dwSourcePlace = PlaceType_Num;
			m_Hand.dwGoodsIndex = 0;
			SetMainPlayerHandByGoods(false,0,0,NULL);
		}
	}
    else if (m_Hand.dwObjectTypeOnHand == TYPE_SKILL)
	{
		SetMainPlayerHandBySkill(0);
	}
	else if (m_Hand.dwObjectTypeOnHand == TYPE_PET)
	{
		SetMainPlayerHandByPet(0,NULL);
	}
	SetMainPlayerHandByEffect(0,0);
	m_Hand.dwObjectTypeOnHand = 0;
}

void CMainPlayerHand::SendPutDownMes(long SorceType/*Դ����*/,const CGUID& SorceGUID/*Դ�����GUID*/,long lSorceObjectExtendID,long lSorceObjectPos,
									 long DesType,const CGUID& DesGUID,long lDesObjectExtendID,long lDesObjectPos,long lObjectType,const CGUID& ObjectGUID,long lObjectNum)
{
	if (g_bDebug)
	{
		/// �������������Ʒ�ƶ�����Ϣ
		char strTitle[64] = "";
		//if (GetGame()->GetCGuiEx()->GetItemPageEx()->GetFinishBagState())
		//{
		//	strcpy_s(strTitle,"--�ͻ�������������͵ģ�������Ʒ�ƶ���Ϣ--");
		//}else
		//	strcpy_s(strTitle,"--�ͻ�������������͵���Ʒ����ͨ���ƶ���Ϣ--");
		char strPrintInfor[512] = "";
		char strGoodsGUID[64] = "";
		ObjectGUID.tostring(strGoodsGUID);
		sprintf_s(strPrintInfor,"%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%s\n%s%d\n",
			strTitle,
			"��Ʒԭ�����ڵ�����ID:",lSorceObjectExtendID,
			"��Ʒ��ԭ�������е�λ��:",lSorceObjectPos,
			"��ƷҪ�ƶ�����Ŀ������ID:",lDesObjectExtendID,
			"��ƷҪ�ƶ���Ŀ��������λ��:",lDesObjectPos,
			"���β�������Ʒ��GUID:",strGoodsGUID,
			"���β�������Ʒ����:",lObjectNum);
		PutStrToFileSaveToDir( "log/GoodsMoveLog", "��¼��Ʒ���ƶ�����", strPrintInfor, false );
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

// ���������������Ʒ����Ӱ״̬
void CMainPlayerHand::DelShadowStateOfGoods()
{
	//CItemPageEx* pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
	//CCharacterPageEx * pCharPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
	//CDepotPageEx* pDepotPage = GetGame()->GetCGuiEx()->GetDepotPageEx();
	//if(pPage && pCharPage && pDepotPage)
	//{
	//	switch(m_Hand.HandGoods.lType)
	//	{
	//	case PEI_PACKET:
	//		{
	//			pPage->UpdataItem(m_Hand.HandGoods.lPos);
	//		}
	//		break;
	//	case PEI_BUSINESSPACK:
	//		{
	//			pPage->UpdateBusinessPack(m_Hand.HandGoods.lPos);
	//		}
	//		break;
	//	case PEI_PACK:
	//		{
	//			pPage->UpdataPack(m_Hand.HandGoods.lPos);
	//		}break;
	//	case PEI_EQUIPMENT:
	//		{
	//			pCharPage->UpdataEquip(m_Hand.HandGoods.lPos);
	//		}break;
	//	case PEI_PACK1:
	//	case PEI_PACK2:
	//	case PEI_PACK3:
	//	case PEI_PACK4:
	//	case PEI_PACK5:
	//		{
	//			pPage->UpdataSubPackData(m_Hand.HandGoods.lType-PEI_PACK1,(short)m_Hand.HandGoods.lPos);
	//		}break;
	//	}

	//	//
	//	if( CDepot::Inst()->IsDepotGC( m_Hand.HandGoods.lType ) )
	//	{
	//		pDepotPage->UpdateItem( m_Hand.HandGoods.lType, m_Hand.HandGoods.lPos );
	//	}
	//}
	
}

// ������Ч
void CMainPlayerHand::SetMainPlayerHandByEffect(DWORD dwSkillID,DWORD dwEffectID)
{
	CClientRegion *pRegion = GetGame()->GetRegion();
	if(pRegion)
	{
		// ɾ��֮ǰ���ϵķ�Χ��Ч
		if(m_Hand.stSkillEffect.gEffectID != CGUID::GUID_INVALID)
			pRegion->DeleteChildObject(TYPE_EFFECT,m_Hand.stSkillEffect.gEffectID);
		if(dwEffectID > 0 && dwSkillID > 0)
		{
			if( m_Hand.dwSkillIndex == 0 && 
				m_Hand.dwGoodsIndex == 0 &&
				m_Hand.HandGoods.pGoods == NULL)
			{
				float lDestX=0.0f, lDestY=0.0f;
				pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lDestX, lDestY);
				CEffect *pEffect = pRegion->PlayEffect(dwEffectID,lDestX,lDestY);
				if(pEffect)
				{
					pEffect->SetLoop(true);
					m_Hand.stSkillEffect.gEffectID = pEffect->GetExID();
					m_Hand.stSkillEffect.dwSkillID = dwSkillID;
				}
			}
		}
		else
		{
			m_Hand.stSkillEffect.gEffectID = CGUID::GUID_INVALID;
			m_Hand.stSkillEffect.dwSkillID = 0;
		}
	}
}

// ����������Ч�Ļ���λ��
bool CMainPlayerHand::DisplayEffectInHand()
{
	CClientRegion *pRegion = GetGame()->GetRegion();
	if(pRegion && m_Hand.stSkillEffect.gEffectID != CGUID::GUID_INVALID)
	{
		CEffect *pEffect = (CEffect *)pRegion->FindChildObject(TYPE_EFFECT,m_Hand.stSkillEffect.gEffectID);
		float lDestX=0.0f, lDestY=0.0f;
		pRegion->CalcMousePoint(GetMouse()->GetMouseX(), GetMouse()->GetMouseY(), lDestX, lDestY);
		// ��Ч��ɾ�������¼���
		if(!pEffect)
		{
			/***********************************************************************/
			/* zhaohang fix */
			/***********************************************************************/
			CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(m_Hand.stSkillEffect.dwSkillID);
			//tagSkillIni* pSkill = SkillAttrIniSetup::GetSkillIni(m_Hand.stSkillEffect.dwSkillID);
			if (pSkill && lDestX > 0.f && lDestY > 0.f)
			{
				/***********************************************************************/
				/* zhaohang fix */
				/***********************************************************************/
				pEffect = pRegion->PlayEffect(pSkill->dwEffectID,lDestX,lDestY);
				pEffect->SetLoop(true);
				m_Hand.stSkillEffect.gEffectID = pEffect->GetExID();
			}			
		}
		if(pEffect)
		{
			pEffect->SetPosXY(lDestX,lDestY);
			return true;
		}
	}
	return false;
}