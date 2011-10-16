#include "stdafx.h"
#include "../../../Net/Message.h"
#include "../../../GameClient/ClientApp/Player.h"
#include "../ClientRegion.h"
#include "../MainPlayerHand.h"
#include "../../../GameClient/ClientApp/Skills/SkillListXml.h"
#include "../../../GameClient/ClientApp/Other/AudioList.h"
#include "../../../GameClient/ClientApp/Depot.h"
#include "../GameClient/Game.h"

#include "../../../UI/Windows/ShopCityDef.h"
#define	PROMPT_Power_lower_limit	20

/*
 * ����: ��ӦGoods��Ϣ
 * ժҪ: -
 * ����: pMsg - ���������͵���Ϣ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.04.01 - lpf
 *		��MSG_S2C_VIEWEQUIP��Ϣ�н�GetHairPic()�޸�ΪSetHairPic();
 *	2008.04.01 - lpf
 *		��MSG_S2C_VIEWEQUIP��Ϣ�н�pClonePlayer->SetDisplayHeadPiece(pPlayer->GetDisplayHeadPiece())�޸�ΪpClonePlayer->SetShowFashion(pPlayer->GetShowFashion())
 */
void OnGoodsMessage(CMessage *pMsg)
{
	switch(pMsg->GetType()) 
	{
	case MSG_S2C_PICKGOOD://��Ҵӳ����м���һ����Ʒ�Ľ������
		{
			CGUID PlayerGUID;
			pMsg->GetGUID(PlayerGUID);
			CPlayer* pPlayer = (CPlayer*)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerGUID);
			if(pPlayer!=NULL&&PlayerGUID!=GetGame()->GetMainPlayer()->GetExID())
			{
				pPlayer->SetAction(CShape::ACT_PICK);
			}
		}
		break;
	case MSG_S2C_EXCHANGEHOTKEY://��ҽ����ȼ��Ľ��������Ϣ
		{
 			int result=pMsg->GetChar();
			int nIndex=pMsg->GetByte();
			int extend = 0;
			int pos = 0;
			CPlayer* pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer && nIndex >= 0 && nIndex < 24)
			{
				pPlayer->SetIsCanSetHotKey(true);
				if(result==HOTKEY_SET_SUCCEED)
				{
					DWORD dwGoodIndex = pMsg->GetDWord();
					/////////////////////////////////////////////////
					// zhaohang  2010/5/19 
					// �ָ����ܿ�ݼ�����
					//CMainPlayerHand* pHand = 0;
					CMainPlayerHand* pHand = GetGame()->GetMainPlayerHand();
					CGoods* pHandPGoods = pHand->GetPGoodsOfMainPlayerHand();
					////////////////////////////////
					if (pHandPGoods!=NULL)
					{
						extend = pHand->GetTGoodsOfMainPlayerHand()->lType;
						pos = pHand->GetTGoodsOfMainPlayerHand()->lPos;
						pHandPGoods->SetHaveShadowState(false);
						
					}
					///////////////////////////////////
					DWORD dwHotKey = pPlayer->GetHotKey(nIndex);
					if (dwHotKey!=0)
					{	
						//����Ŀ����λ����Ʒͼ��
						if ((dwHotKey & 0x80000000)==0)
						{
							if ((pHandPGoods!=NULL&&dwHotKey!=pHandPGoods->GetIndex())||pHand->GetSkillOfMainPlayerHand()!=0||pHand->GetGoodsIndexOfMainPlayerHand()!=0)
							{
								pHand->SetMainPlayerHandByGoodsIndex(dwHotKey,CMainPlayerHand::PlaceType_Fastfield);
							}else
							{
								pHand->ClearMainPlayerHand();
							}
						}
						//����Ŀ����λ�м���ͼ��
						else
						{
							DWORD wSkillID = dwHotKey&0x7fffffff;
							CPlayer::tagSkill *pSkill = pPlayer->GetSkill(wSkillID);
							if(pSkill)
							{
								/***********************************************************************/
								/* zhaohang fix 2010-9-27*/
								/***********************************************************************/
								// ����ͼ�����ͣ��Ҷ�/������
								WORD wType = CPicList::PT_SKILL_ICON_GRAY;
								// ְҵ���ϣ�ʹ�ò�ɫͼ��
								if(CSkillListXml::OccuJudgeStudy(wSkillID,pPlayer->GetOccupation()))
									wType = CPicList::PT_SKILL_ICON;
								//WORD wType = CPicList::PT_SKILL_ICON;
								pHand->SetMainPlayerHandBySkill(wSkillID,wType,CMainPlayerHand::PlaceType_Fastfield);
							}
							else
								pHand->SetMainPlayerHandBySkill(0);
						}		
					}
					else
					{
						pHand->ClearMainPlayerHand();
					}
					pPlayer->SetHotKey(nIndex,dwGoodIndex);
					
				}
			}
		}
		break;
	case MSG_S2C_CLEARHOTKEY://������һ���ȼ����õĽ��������Ϣ
		{
			int result=pMsg->GetChar();
			int nIndex=pMsg->GetByte();
			if(result==HOTKEY_CLEAR_SUCCEED && nIndex>=0 && nIndex<24)
			{
				int extend = 0;
				int pos = 0;
				CPlayer* pPlayer = GetGame()->GetMainPlayer();
				CMainPlayerHand* pHand = 0;
				DWORD dwHotKey = pPlayer->GetHotKey(nIndex);
				if (pHand!=NULL&&dwHotKey!=0)
				{
					if ((dwHotKey & 0x80000000)==0)
					{
						pHand->SetMainPlayerHandByGoodsIndex(pPlayer->GetHotKey(nIndex),CMainPlayerHand::PlaceType_Fastfield);
					}
					else
					{
						DWORD wSkillID = dwHotKey&0x7fffffff;
						CPlayer::tagSkill *pSkill = pPlayer->GetSkill(wSkillID);
						if(pSkill)
						{
							/***********************************************************************/
							/* zhaohang fix */
							/***********************************************************************/
							// ����ͼ�����ͣ��Ҷ�/������
							WORD wType = CPicList::PT_SKILL_ICON_GRAY;
							// ְҵ���ϣ�ʹ�ò�ɫͼ��
							if(CSkillListXml::OccuJudgeStudy(wSkillID,pPlayer->GetOccupation()))
								wType = CPicList::PT_SKILL_ICON;
							//WORD wType = CPicList::PT_SKILL_ICON;
							pHand->SetMainPlayerHandBySkill(wSkillID,wType,CMainPlayerHand::PlaceType_Fastfield);
						}
						else
							pHand->SetMainPlayerHandBySkill(0);
					}
				}
				pPlayer->SetHotKey(nIndex,0);
				
			}
		}
		break;
	case MSG_S2C_SETHOTKEY://�������һ���ȼ����õĽ��������Ϣ
		{
			int result=pMsg->GetChar();
			int nIndex=pMsg->GetByte();
			DWORD dwVal=pMsg->GetDWord();
			if(nIndex>=0 && nIndex<24)
			{
				GetGame()->GetMainPlayer()->SetIsCanSetHotKey(true);
				GetGame()->GetMainPlayer()->SetHotKey(nIndex,dwVal);
			}
		}
		break;
	case MSG_S2C_DROPMONEY://����ӽ�Ǯ�Ľ��������Ϣ
		{
			int result=pMsg->GetChar();
			if(result==GOODS_DROP_SUCCEED)
			{
				DWORD lNum=pMsg->GetDWord();
				if(lNum>=1 && lNum<=GetGame()->GetMainPlayer()->GetMoney())
				{
					GetGame()->GetMainPlayer()->SetMoney(GetGame()->GetMainPlayer()->GetMoney() - lNum);
				}
			}
		}
		break;
	case MSG_S2C_PICKMONEY://���ʰ���Ǯ�Ľ��������Ϣ
		{
			int result=pMsg->GetChar();
			if(result==GOODS_PICK_SUCCEED)
			{
				DWORD lNum=pMsg->GetDWord();//ʰ�������

				//��Ǯ������
				if(lNum>=0 && GetGame()->GetMainPlayer()->GetMoney()+lNum<=MAX_MONEY)
				{
					GetGame()->GetMainPlayer()->SetMoney(GetGame()->GetMainPlayer()->GetMoney()+lNum);
				}
			}
		}
		break;
	case MSG_S2C_EQUIPDAMAGE://��Ʒ���;�ֵ��ȫ��
		{
			int nPos=pMsg->GetByte();
			CGoods *pEquip=GetGame()->GetMainPlayer()->GetEquip(nPos);
			if(pEquip)
			{
				SAFE_DELETE(pEquip);
				GetGame()->GetMainPlayer()->SetEquip(nPos,NULL);
			}
		}
		break;
	case MSG_S2C_SET_YUANBAO:
		{
			DWORD dwBalance=pMsg->GetDWord();
			GetGame()->GetMainPlayer()->SetYuanBao(dwBalance);
			FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
		}
		break;
		// ���õ��
	case MSG_S2C_SET_TICKET:
		{
			DWORD dwTicket=pMsg->GetDWord();
			GetGame()->GetMainPlayer()->SetJiFen(dwTicket);
			FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
		}
		break;
	case MSG_S2C_EQUIPDURUPDATE://��Ʒ�;�ֵ�ĸ���֪ͨ
		{
			int nPos=pMsg->GetByte();
			DWORD dwVal=pMsg->GetDWord();
			CGoods *pEquip=GetGame()->GetMainPlayer()->GetEquip(nPos);	
			
			if(pEquip)
			{
				long lMaxDurValue = pEquip->GetEffectVal(GAP_GOODS_MAX_DURABILITY,1);
				int nPercent = 100;
				if(lMaxDurValue >0)
					nPercent = dwVal*100/lMaxDurValue;
				
				pEquip->SetEffectVal(GAP_GOODS_MAX_DURABILITY,2,dwVal);
			}
		}
		break;
	case MSG_S2C_EQUIP_REFINE_UPDATE://��Ʒ����ֵ�ĸ���֪ͨ
		{
			BYTE nPos=pMsg->GetByte();
			DWORD dwVal=pMsg->GetDWord();
			CGoods *pEquip = GetGame()->GetMainPlayer()->GetEquip(nPos);	

			if(pEquip)
			{
				pEquip->SetEffectVal(GAP_REFINE,1,dwVal);

				// ��Ʒ������ʱ����ʾ��Ϣ
				long val1 = pEquip->GetEffectVal(GAP_REFINE,2);
				long val2 = pEquip->GetEffectVal(GAP_REFINE,1);
				if (val1==val2)
				{
					char str[64];
					if (pEquip->GetEquipType()==EC_E_RIDER)
					{
						sprintf(str,AppFrame::GetText("Goods_470"),pEquip->GetName());
					}else
						sprintf(str,AppFrame::GetText("Goods_215"),pEquip->GetName());
				}
			}
		}
		break;

	case MSG_S2C_DELETEEQUIP://ɾ��һ���ض�λ�õ�װ��
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			int nPos=pMsg->GetByte();
			if(PlayerID == GetGame()->GetMainPlayer()->GetExID())
			{
				CPlayer* pPlayer = GetGame()->GetMainPlayer();
				CGoods *pEquip=pPlayer->GetEquip(nPos);
				if(pEquip)
				{
					SAFE_DELETE(pEquip);
					pPlayer->SetEquip(nPos,NULL);
				}
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
				if(pPlayer)
				{
					pPlayer->SetEquipIndex(nPos,0);
					pPlayer->SetEquipLevel(nPos,0);
				}
			}
		}
		break;

	case MSG_S2C_DELETEGOODS://ɾ��������ָ��λ��ָ����������Ʒ
		{
			BYTE lPos=pMsg->GetByte();//Ҫɾ������Ʒ�����Ͻ�λ��
			BYTE lNum=pMsg->GetByte();//Ҫɾ������
			list<CPlayer::tagGoods>::iterator GoodsIter=GetGame()->GetMainPlayer()->GetGoodsList()->begin();
			for(;GoodsIter!=GetGame()->GetMainPlayer()->GetGoodsList()->end();GoodsIter++)
			{
				if(GoodsIter->lPos==lPos)
				{
					int nXSize=1;//GoodsIter->pGoods->GetBaseProperty()->lX_Size;
					int nYSize=1;//GoodsIter->pGoods->GetBaseProperty()->lY_Size;
					int nTemPos=GoodsIter->lPos;
					for(int i=0;i<nYSize;i++)
					{
						/*for(int j=0;j<nXSize;j++)
						{
							CPlayer::tagGoods *pGoodsTag=GetGame()->GetInterface()->GetItemsPage()->GetGoodsData(nTemPos+j);
							if(pGoodsTag)
							{
								pGoodsTag->lNum-=lNum;
							}
						}*/
						nTemPos+=8;
					}
					GoodsIter->lNum-=lNum;
					break;
				}
			}
		}
		break;
	case MSG_S2C_DELETEALLGOODS://ɾ��������ָ��λ�õ�ȫ����Ʒ
		{
			BYTE lPos=pMsg->GetByte();
			list<CPlayer::tagGoods>::iterator GoodsIter=GetGame()->GetMainPlayer()->GetGoodsList()->begin();
			for(;GoodsIter!=GetGame()->GetMainPlayer()->GetGoodsList()->end();GoodsIter++)
			{
				if(GoodsIter->lPos==lPos)
				{
					//GetGame()->GetInterface()->GetItemsPage()->ClearGoodData(lPos,
					//	1/*GoodsIter->pGoods->GetBaseProperty()->lX_Size*/,
					//	1/*GoodsIter->pGoods->GetBaseProperty()->lY_Size*/);
					SAFE_DELETE(GoodsIter->pGoods);
					GetGame()->GetMainPlayer()->GetGoodsList()->erase(GoodsIter);
					break;
				}
			}
		}
		break;
	case MSG_S2C_VIEWEQUIP://gameserver���ز鿴���װ�����
		{
			BYTE lSize=0;
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			BYTE lHeadPic=pMsg->GetByte();
			BYTE lFacePic=pMsg->GetByte();

			lSize=pMsg->GetByte();
			CPlayer* pPlayer=(CPlayer*)(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID));
			if(pPlayer)
			{
				//�����װ���˵���Ʒ�Ľ���
				for(int i=0;i<lSize;i++)
				{
					BYTE EquipPos=pMsg->GetByte();
					CGoods* pEquip=new CGoods;
					if(pEquip)
					{
						BYTE ByteData[102400];
						pMsg->GetEx(ByteData,102400);
						long lPos = 0;
						pEquip->DecordFromByteArray(ByteData,lPos);
						//GetGame()->GetCGuiEx()->GetLookPageEx()->AddEquip(pEquip,EquipPos);
					}
				}
				BYTE lOccuLvl = pMsg->GetByte();               //��ǰְҵ�ȼ�
				BYTE lConst = pMsg->GetByte();                 //����
				eOccupation eOccuSkillID = pPlayer->GetOccupation();   
				pPlayer->SetOccuLvl(eOccuSkillID,lOccuLvl);
			//	pPlayer->SetConst(lConst);
				//GetGame()->GetCGuiEx()->GetLookPageEx()->Open();
			}
		}
		break;
	case MSG_S2C_OPENGOODSUPGRADE://gameserver֪ͨ�ͻ��˴򿪵�����������
		{
			long lSessionID = pMsg->GetLong();
			long lPlugID = pMsg->GetLong();
			//GetGame()->GetInterface()->GetEquipUpgradePage()->SetSessionID(lSessionID, lPlugID);
			//GetGame()->GetInterface()->GetEquipUpgradePage()->Open();
		}
		break;	
	case MSG_S2C_GOODSUPGRADE_UPGRADE://��Ʒ��������ؼ���
		{
			CGoods* pUpgradeGoods=NULL/*GetGame()->GetInterface()->GetEquipUpgradePage()->GetGoods()*/;
			if(pUpgradeGoods)
			{
				DWORD dwSize=pMsg->GetDWord();
				long lPos=0;
				BYTE *pBytes=new BYTE[dwSize];
				pMsg->GetEx(pBytes,dwSize);
				pUpgradeGoods->DecordFromByteArray(pBytes,lPos);
				delete pBytes;
			}
		}
		break;
	case MSG_S2C_GOODS_UPDATEPROPERTY:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			CGUID guGoodsID;
			pMsg->GetGUID(guGoodsID);

			CPlayer* pPlayer=(CPlayer*)(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID));
			if(pPlayer)
			{
				CGoods* pGoods = pPlayer->GetGoodsByGUID(guGoodsID);
				if(pGoods)
				{
					long lDataSize = pMsg->GetLong();
					BYTE* pData = new BYTE[lDataSize];
					pMsg->GetEx(pData, lDataSize);
					long lPos = 0;
					pGoods->DecordFromByteArray(pData,lPos,false);
					delete [] pData;
				}
			}
		}
		break;
	case MSG_S2C_OPENGOODSENCHASE://�򿪿�Ƭ��Ƕ����ҳ��
		{
			CGUID sessionID;
			BYTE bPageType = pMsg->GetByte();
			pMsg->GetGUID(sessionID);
			long playerID = pMsg->GetLong();
			/*CCardMutifunPageEx* pPage = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			if (pPage!=NULL&&!pPage->IsOpen())
			{
				if (pPage->GetNpcID()==CGUID::GUID_INVALID)
				{
					pPage->SetNpcID(GetGame()->GetLastTouchNpcID());
				}
				pPage->OpenPage(sessionID,playerID,bPageType);
			}*/
		}
		break;
	case MSG_S2C_ENCHASE_ARRANGE:
		{
			//CCardFunctionPageEx* pFunPage = GetGame()->GetCGuiEx()->GetCardFunctionPageEx();
			/*CCardMutifunPageEx*	 pMutiPage = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			if (pMutiPage!=NULL)
			{			
				BYTE bEnchaseOrRemove = pMsg->GetByte();  /// �ж�����Ƕ���ǲ𿨲������ˢ��
				BYTE bCardNum = pMsg->GetByte();
				vector<CGUID> temp;
				CGUID guid;
				for (BYTE i = 0;i<bCardNum;i++)
				{
					pMsg->GetGUID(guid);
					temp.push_back(guid);
				}

				/// �������Ƕ������ֱ�Ӹ��¿�Ƭ���GUID������ǲ𿨾ͱȶԿ�Ƭ��GUID����������Ч�Ŀ�ƬȻ��ˢ�½�����ʾ
				pMutiPage->UpdateGoodsInfor(bEnchaseOrRemove,temp);
				pMutiPage->UpdatePage();			
			}*/
		}
        break;
	case MSG_S2C_ENCHASE_OPEN_HOLE:
		{
			BYTE holeNum = pMsg->GetByte();
			/*CCardMutifunPageEx* page = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			if (page!=NULL)
			{
				if (holeNum>=0&&holeNum<page->GetTargetGoods()->GetEffectVal(CGoods::GAP_HOLE_NUM,2))
				{
					page->StilettoGoods(holeNum+1);
					page->UpdatePage();
					GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( 108 );
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-01.wav");
				}
			}*/
		}
		break;
	case MSG_S2C_ENCHASE_CLOSE_HOLE:
		{
			BYTE holeNum = pMsg->GetByte();
			BYTE holeIndex = pMsg->GetByte();
			//CCardMutifunPageEx* page = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			//if (page!=NULL)
			//{
			//	if (holeNum>=0&&holeNum<page->GetTargetGoods()->GetEffectVal(CGoods::GAP_HOLE_NUM,2))
			//	{
			//		page->JamGoods(holeIndex,holeNum);
			//		page->UpdateAppointHole(holeIndex);
			//		//GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( 109 );
			//		GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-01.wav");
			//	}
			//}
		}
		break;
	case MSG_S2C_ENCHASE_ENCHASECARD:
		{
			//CCardMutifunPageEx* page = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			//if (page!=NULL)
			//{
			//	page->EnchaseCards();
			//	//page->UpdatePage();
			//	GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( 110 );
			//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-10.wav"); 
			//}
		}
		break;
	case MSG_S2C_ENCHASE_REMOVECARD:
		{
			BYTE result = pMsg->GetByte();
			BYTE holeIndex = pMsg->GetByte();
			if (result!=0)
			{
				/*CCardMutifunPageEx* page = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
				if (page!=NULL)
				{
					page->RemoveCard(holeIndex);
					page->RemoveAssGoodsFromPage();
					page->UpdateAssGoodsShow();
					GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( 111 );
					GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-01.wav");
				}*/
			}else
			{
				//ʧ�ܺ����ʾ����δ����
			}
		}
		break;
	// ������
	case MSG_S2C_GOODS_OPEN_CONTAINER:
		{
			char szText[4096];
			pMsg->GetStr(szText, 4096);

			//GetGame()->GetInterface()->GetContainerPage()->Open();
			//GetGame()->GetInterface()->GetContainerPage()->SetContainerType(szText);
		}
		break;


	// ���壺�����ڵĴ򿪲���
	case MSG_S2C_GOODS_OPEN_WINDOWS:
		{
			static long enumPreWndType = -1;
			//static	CGuiExPage* pPrePage = NULL;

			//
			long enumWndType		= pMsg->GetLong();
			long lPlayerMoveRange	= pMsg->GetLong();
			long lOpenedTileX		= pMsg->GetLong();
			long lOpenedTileY		= pMsg->GetLong();

			// ���ǰ�����Ĵ�����ͬ�����
			if( enumWndType == enumPreWndType )
			{
			}
			// �ر��ϸ�����
			else
			{
				/*if( pPrePage )
					pPrePage->Close();*/
			}

			//
			switch( enumWndType )
			{
				// �ֽ�
				case eGWT_DisassembleWindow:
					{
						//
						enumPreWndType = enumWndType;
						//pPrePage = GetGame()->GetCGuiEx()->GetDecomposePageEx();
						//GetGame()->GetCGuiEx()->GetDecomposePageEx()->Open();

						//GetGame()->GetCGuiEx()->GetDecomposePageEx()->
						//	SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );
					}
					break;
				// ��ͨ�ϳ�
				case eGWT_PrescriptionWindow:
				// ħ���ϳ�
//				case eGWT_EvilSynthesizeWindow:
				// ��
				//case eGWT_BindWindow:
				// ����
				//case eGWT_BindAndFrostWindow:
					{
						//
						enumPreWndType = enumWndType;
						//pPrePage = GetGame()->GetCGuiEx()->GetComposePageEx();

						////
						//GetGame()->GetCGuiEx()->GetComposePageEx()->Open();

						//GetGame()->GetCGuiEx()->GetComposePageEx()->
						//	SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );
					}
					break;
				// ǿ��
				case eGWT_UpgradeWindow:
					{
						//
						enumPreWndType = enumWndType;
						//pPrePage = GetGame()->GetCGuiEx()->GetEquipUpgradePageEx();

						////
						//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->Open();

						//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->
						//	SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );
					}
					break;
				// ħ��
				case eGWT_PlusEvilWindow:
					{
						/*if(GetGame()->GetCGuiEx()->GetEnchantEquipmentPageEx()->IsOpen())
						{
		                    GetGame()->GetCGuiEx()->GetEnchantEquipmentPageEx()->Close();
						}
			            GetGame()->GetCGuiEx()->GetEnchantEquipmentPageEx()->Open();
						
						GetGame()->GetCGuiEx()->GetEnchantEquipmentPageEx()->
							SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );*/
					}
					break;
				//����ħ�����ʹ���
				case eGWT_ResetElementTypeWindow:
					{
						/*if(GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->IsOpen())
						{
						   GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->Close();
						}
			            GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->Open();
						GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->SetOpenType(0);
						GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->
							SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );*/
					}
					break;
				//! ����ħ�����Դ���
				case eGWT_ResetElementXiangXingWindow:
					{
						/*if(GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->IsOpen())
						{
						   GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->Close();
						}
						GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->Open();
						GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->SetOpenType(1);
						GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->
							SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );*/
					}
					break;
				// ħ�����
				//case eGWT_EvilBackoutWindow:
				//	{
				//		//
				//		enumPreWndType = enumWndType;
				//		pPrePage = GetGame()->GetCGuiEx()->GetMagicDecomposePageEx();

				//		//
				//		GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->Open();

				//		GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->
				//			SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );
				//	}
				//	break;
				//// �ֿ�
				case eGWT_DepotWindow:
					{
						//
						//enumPreWndType = enumWndType;
						//pPrePage = GetGame()->GetCGuiEx()->GetDepotPageEx();

						////
						//GetGame()->GetCGuiEx()->GetDepotPageEx()->Open();

						//GetGame()->GetCGuiEx()->GetDepotPageEx()->
						//	SetPlayerMoveRange( lOpenedTileX, lOpenedTileY, lPlayerMoveRange );
					}
					break;


				default:
					break;
			}
		}
		break;

	// ���壺Goods �ֽ�����ս��
	case MSG_S2C_GOODS_RE_DISASSEMBLE_DO:
		{
			//GetGame()->GetCGuiEx()->GetDecomposePageEx()->RecvDecomposeMsg( pMsg );
		}
		break;
	// ���壺Goods �ϳɵĽ��
	case MSG_S2C_GOODS_RE_SYNTHESIZE_DO:
		{
			//GetGame()->GetCGuiEx()->GetComposePageEx()->RecvNormalComposeMsg( pMsg );
		}
		break;
	// ���壺Goods �󶨵Ľ��
	case MSG_S2C_GOODS_BIND:
		{
			//GetGame()->GetCGuiEx()->GetComposePageEx()->RecvBindMsg( pMsg );
		}
		break;
	// ���壺Goods ����Ľ��
	case MSG_S2C_GOODS_FROST:
		{
			//GetGame()->GetCGuiEx()->GetComposePageEx()->RecvFreezeMsg( pMsg );
		}
		break;
	// ���壺Goods ���ܵĽ��
	case MSG_S2C_GOODS_RE_REINFORCE_POWER:
		{
			//CMainBarPageEx*	pMainBarPageEx = GetGame()->GetCGuiEx()->GetMainBarPageEx();

			//// ��� �ɹ�ʧ��
			//if( eGTC_Succeed == pMsg->GetLong() )
			//{
			//	// ��� PLYAER ��GUID
			//	CGUID guidPlayer;
			//	pMsg->GetGUID( guidPlayer );

			//	// ����ǿ��
			//	long lPower = pMsg->GetLong();

			//	// ������Լ���ˢ���Լ�װ�����������ɹ���ʾ
			//	if( guidPlayer == GetGame()->GetMainPlayer()->GetExID() )
			//	{
			//		if( CGoods* pgoodsEquip = GetGame()->GetMainPlayer()->GetEquip( CPlayer::EQUIP_WEAPON ) )
			//			pgoodsEquip->SetEffectVal( CGoods::GAP_POWER_INTENSITY, 1, lPower );

			//		// ��ʾ
			//		GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 200 ), 0xff00ffff );
			//		//���ųɹ���Ч
			//		CPlayer *pMainPlayer=GetGame()->GetMainPlayer();
			//		GetGame()->GetAudioList()->Play3DSound("SOUNDS\\interfaces\\i-11.wav",1,pMainPlayer->GetPosY(),pMainPlayer->GetHeight(),pMainPlayer->GetPosX());
			//	} 
			//	// ������ǫ�Ľӿڴ�����Ч��
			//	CPlayer *pPlayerPowerAdd = dynamic_cast<CPlayer*>( GetGame()->GetRegion()->FindChildObject( TYPE_PLAYER, guidPlayer ) );
			//	if( pPlayerPowerAdd )
			//	{
			//		pPlayerPowerAdd->PlayEnergyEffect( lPower );
			//	}
			//}
			//else
			//{
			//	// �������
			//	switch( pMsg->GetLong() )
			//	{
			//		case eGTC_NoArmInHand:
			//			{
			//				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 201 ), 0xff00ffff );
			//			}
			//			break;

			//		case eGTC_NoReinforceArm:
			//			{
			//				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 202 ), 0xff00ffff );
			//			}
			//			break;

			//		case eGTC_LevelError:
			//			{
			//				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 203 ), 0xff00ffff );
			//			}
			//			break;

			//		case eGTC_ArmIsFull:
			//			{
			//				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 204 ), 0xff00ffff );
			//			}
			//			break;

			//		default:
			//			break;
			//	}
			//	//����ʧ����Ч
			//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
			//}
		}
		break;

	// ���壺����ʣ���������
	case MSG_S2C_GOODS_POWER_WARN:
		{
			// ��� PLYAER ��GUID
			CGUID guidPlayer;
			pMsg->GetGUID( guidPlayer );

			// ����ǿ��
			long lPower = pMsg->GetLong();

			// �����Լ���ʣ������
			long lLeavingPower = pMsg->GetLong();

			if( GetGame()->GetMainPlayer()->GetExID() == guidPlayer )
			{
				if( CGoods* pgoodsEquip = GetGame()->GetMainPlayer()->GetEquip( CPlayer::EQUIP_WEAPON ) )
					pgoodsEquip->SetEffectVal( GAP_CURR_POWER, 1, lLeavingPower );

				// ��ʾ��Ϣ
				//CMainBarPageEx*	pMainBarPageEx = GetGame()->GetCGuiEx()->GetMainBarPageEx();

				// ������������
				//if( lLeavingPower <= 0 )
				//{
				//	pMainBarPageEx->AddText( eNOTIFYPOS_CENTER, CStringReading::LoadText( 208, 213 ), 0xffff0000 );
				//}
				//// ����������������
				//else if( lLeavingPower <= PROMPT_Power_lower_limit )
				//{
				//	TCHAR	strPowerPrompt[256];
				//	wsprintf( strPowerPrompt, CStringReading::LoadText( 208, 205 ), lLeavingPower );

				//	pMainBarPageEx->AddOtherInfo( strPowerPrompt );
				//}
			}

			// ������Ч�ӿ�
			if( lLeavingPower > 0 )
			{
				CPlayer *pPlayerPowerAdd = dynamic_cast<CPlayer*>( GetGame()->GetRegion()->FindChildObject( TYPE_PLAYER, guidPlayer ) );
				if( pPlayerPowerAdd )
				{
					pPlayerPowerAdd->PlayEnergyEffect( lPower );
					GetGame()->GetAudioList()->Play3DSound("SOUNDS\\interfaces\\i-34.wav",1,pPlayerPowerAdd->GetPosY(),pPlayerPowerAdd->GetHeight(),pPlayerPowerAdd->GetPosX());
				}
			}
		}
		break;

	// ���壺װ��ǿ��Ԥ���Ľ��
	case MSG_S2C_GOODS_RE_UPGRADE_VIEW:
		{
			//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->RecvPreviewMsg( pMsg );
		}
		break;
	// ���壺װ��ǿ���Ľ��
	case MSG_S2C_GOODS_RE_UPGRADE_DO:
		{
			//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->RecvUpgradeMsg( pMsg );
		}
		break;
	// ���壺װ��ħ��Ԥ���Ľ��
	//case MSG_S2C_GOODS_RE_PLUG_EVIL_VIEW:
	//	{
	//		GetGame()->GetCGuiEx()->GetEquipMagicPageEx()->RecvPreviewMsg( pMsg );
	//	}
	//	break;
	// ���壺װ��ħ���Ľ��
	case MSG_S2C_GOODS_RE_PLUG_EVIL_DO:
		{
			//GetGame()->GetCGuiEx()->GetEnchantEquipmentPageEx()->RecvMagicMsg( pMsg ); 
		}
		break;
	// ���壺װ��ħ���ϳɵĽ��
	//case MSG_S2C_GOODS_RE_STUFF_SYNTHESIZE:
	//	{
	//		GetGame()->GetCGuiEx()->GetComposePageEx()->RecvMagicComposeMsg( pMsg );
	//	}
	//	break;
	// ���壺װ��ħ������Ԥ��
	//case MSG_S2C_GOODS_RE_EVIL_BACKOUT_VIEW:
	//	{
	//		GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->RecvPreviewMsg( pMsg );
	//	}
	//// ���壺װ��ħ�����Ľ��
	//case MSG_S2C_GOODS_RE_EVIL_BACKOUT:
	//	{
	//		GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->RecvDecomposeMsg( pMsg );
	//	}
	//	break;

	// ���壺�򿪾�������
	case MSG_S2C_MAKE_BEGIN:
		{
			//GetGame()->GetCGuiEx()->GetMakeScrollPageEx()->RecvOpenMsg( pMsg );
		}
		break;
	// ���壺�����������
	case MSG_S2C_MAKE_RESULT:
		{
			//GetGame()->GetCGuiEx()->GetMakeScrollPageEx()->RecvMakeMsg( pMsg );
		}
		break;


	// ���壺�ֿ� �ظ��ͻ�����������Ľ��
	case MSG_S2C_DEPOT_RE_INPUT_PWD:
		{
			CDepot::Inst()->RecvEnterMsg( pMsg );
		}
		break;
	// ���壺�ֿ� �ظ��޸�����Ľ��
	case MSG_S2C_DEPOT_RE_CHANGE_PWD:
		{
			CDepot::Inst()->RecvSetPWMsg( pMsg );
		}
		break;
	// ���壺�ֿ� �����ӱ����Ľ��
	case MSG_S2C_DEPOT_RE_BUY_SUBPACK:
		{
			CDepot::Inst()->RecvBuyItemMsg( pMsg );
		}
		break;
	// ���壺�ֿ� ����
	case MSG_S2C_DEPOT_FROSTED:
		{
			CDepot::Inst()->RecvFreezeMsg( pMsg );
		}
		break;
	// ���壺�ֿ� �ر�
	case MSG_S2C_DEPOT_RE_CLOSE:
		{
			//GetGame()->GetCGuiEx()->GetDepotPageEx()->Close();
		}
		break;

	// ���壺ͨ����Ʒ�������� ��
	case MSG_S2C_GOODS_AllPurpose_Open:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvOpenMsg( pMsg );
		}
		break;

	// ���壺ͨ����Ʒ�������� �ر�
	case MSG_S2C_GOODS_AllPurpose_Close:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvCloseMsg( pMsg );
		}
		break;

	// ���壺ͨ����Ʒ�������� �ظ�������Ʒ
	case MSG_S2C_GOODS_AllPurpose_RePush:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvPushMsg( pMsg );
		}
		break;

	// ���壺ͨ����Ʒ�������� �ظ�ȡ����Ʒ
	case MSG_S2C_GOODS_AllPurpose_RePop:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvPopMsg( pMsg );
		}
		break;

	// ���壺ͨ����Ʒ�������� ����˵���ı�
	case MSG_S2C_GOODS_AllPurpose_UpdateTxt:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvDescMsg( pMsg );
		}
		break;
		// ���壺ͨ����Ʒ�������� ���½��
	case MSG_S2C_GOODS_AllPurpose_UpdateGold:
		{
			//GetGame()->GetCGuiEx()->GetGeneralGoodsPageEx()->RecvGoldMsg( pMsg );
		}
		break;

	case MSG_S2C_MEDAL_ADD_NOTIFY:
		{
			long GoodsIndex = pMsg->GetLong();
			//GetGame()->GetCGuiEx()->GetMadalPageEx()->AddMedal(GoodsIndex); 
			CPlayer *pPlayer=GetGame()->GetMainPlayer();
			if(NULL == pPlayer)
				  return;
			pPlayer->AddMedal(GoodsIndex); 
			//ˢ��ѫ���б����ʾ
			//GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdateMedalList();
			//GetGame()->GetCGuiEx()->GetMadalPageEx()->OpenGetTip(GoodsIndex);  //ѫ�»����ʾ
		}
		break;
	case MSG_S2C_MEDAL_DEL_NOTIFY:
		{
			long GoodsIndex = pMsg->GetLong();
			CPlayer *pPlayer=GetGame()->GetMainPlayer();
			if(NULL == pPlayer)
			         return;
			pPlayer->DeleteMedal(GoodsIndex); 
			//ˢ��ѫ���б����ʾ
			//GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdateMedalList(); 
		}
		break;
	case MSG_S2C_GOODS_UPDATE_SCRIPTDESC:      // AddBy X.Y 2009.9.18
		{
			CGUID GoodsGuid;
			char strActiveInfor[512] = "";
			pMsg->GetGUID(GoodsGuid);
			pMsg->GetStr(strActiveInfor,512);
			CPlayer *pPlayer=GetGame()->GetMainPlayer();
			CGoods*pGoods = pPlayer->GetGoodsByGUID(GoodsGuid);
			// �ҳ����������Ʒ
			if( ! pGoods )
			{
				pGoods = dynamic_cast<CGoods*>( GetGame()->GetRegion()->FindChildObject( TYPE_GOODS, GoodsGuid ) );
			}
			if (pGoods!=NULL)
			{
				pGoods->SetManufacturerName(strActiveInfor);
			}
		}
		break;
	case MSG_C2S_GOODS_RE_RESET_ELEMENT_TYPE:
		{
			//GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->RecvMagicMsg( pMsg ); 
		}
		break;
	case MSG_C2S_GOODS_RE_RESET_XIANG_XING:
		{
			//GetGame()->GetCGuiEx()->GetChangeEnchantPageEx()->RecvMagicMsg( pMsg ); 
		}
		break;
	}
}