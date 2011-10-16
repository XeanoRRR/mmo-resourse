#pragma once
#include "stdafx.h"
#include "../Player.h"
#include "Message.h"
#include "Effect.h"
#include "Other/AudioList.h"
#include "../GameClient/GameControl.h"
#include "../GameClient/Game.h"
#include "Other/GlobeSetup.h"
#include "../../Public/Common/SkillRelated.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 * ����: ��Ӧ�����Ϣ
 * ժҪ: -
 * ����: pMsg - ���������͵���Ϣ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.10.21 - lpf
 *		��MSG_S2C_PLAYER_ANSWER_RELIVE��Ϣ��������������»������ǰ��������;
 *	2009.02.09 - lpf
 *		��MSG_S2C_PLAYER_ADDEXP��Ϣ����������Ҿ�����ʾ�ı�,�ҹ����˹��ḣ����������;
 *	2009.03.13 - lpf
 *		������MSG_S2C_PLAYER_UpdateShowFashion��Ϣ����;
 */
void CPlayer::OnMessage(CMessage* pMsg)
{
	CPlayer* pPlayer=GetGame()->GetMainPlayer();
	switch(pMsg->GetType())
	{
	case MSG_S2C_PLAYER_COLLECTION_LEVELUP:
		{
			//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
			long lCollectLevel = pMsg->GetLong();
			CGUID PlayerGUID;
			pMsg->GetGUID(PlayerGUID);
			CPlayer* pSPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerGUID);
			if (pPlayer==NULL||pSPlayer==NULL)
				break;
			if (PlayerGUID==pPlayer->GetExID())
			{
				pSPlayer->SetCollectSkillLevel(lCollectLevel);
				char strLevel[64];
				DWORD dwDOccu = pSPlayer->GetAssOccu();
				switch(dwDOccu)
				{
				case DOccu_Smithing:
                    sprintf(strLevel,AppFrame::GetText("Deputy_44"),lCollectLevel); //��������%s
					break;
				case DOccu_Metallurgy:
					sprintf(strLevel,AppFrame::GetText("Deputy_45"),lCollectLevel); //��������%s
					break;
				case DOccu_Sewing:
					sprintf(strLevel,AppFrame::GetText("Deputy_46"),lCollectLevel); //�÷�����%s
				    break;
				}
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(strLevel);
			}
			GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-27.wav");
			// ������Ч
			pSPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwSkillLevelUpEffectID, pSPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
		}
		break;
	case MSG_S2C_PLAYER_FACTURE_LEVELUP:
		{
			//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
			//CLearnSkillPage* pLearnSkillPage = GetGame()->GetCGuiEx()->GetLearnSkillPage();
			long lFactureLevel = pMsg->GetLong();
			CGUID PlayerGUID;
			pMsg->GetGUID(PlayerGUID);
			CPlayer* pSPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerGUID);
			if (pPlayer==NULL||pSPlayer==NULL)
				break;
			if (PlayerGUID==pPlayer->GetExID())
			{
				pSPlayer->SetFactureSkillLevel(lFactureLevel);
				//CFacturePageEx::FactureLevelName* pFacLevelName = CFacturePageEx::GetFactureLevelName(lFactureLevel);
				//if (pFacLevelName!=NULL)
				//{
				//	char strLevel[64];
				//	DWORD dwDOccu = pSPlayer->GetAssOccu();
				//	switch(dwDOccu)
				//	{
				//	case DOccu_Smithing:
				//		sprintf(strLevel,CStringReading::LoadText(205,44),pFacLevelName->strSmithingName.c_str());
				//		break;
				//	case DOccu_Metallurgy:
				//		sprintf(strLevel,CStringReading::LoadText(205,45),pFacLevelName->strMetallurgyName.c_str());
				//		break;
				//	case DOccu_Sewing:
				//		sprintf(strLevel,CStringReading::LoadText(205,46),pFacLevelName->strSewingName.c_str());
				//		break;
				//	}
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(strLevel);
				}
				//if(pFacturePage!=NULL&&pFacturePage->IsOpen())
				//{
					//long lExp = pPlayer->GetFactureSkillExp()-pFacturePage->GetFactureExpProgressbar()->GetCurValue();
					//if (lExp>0)
					//{
					//	char strExp[64];
					//	sprintf(strExp,CStringReading::LoadText(205,23),lExp);
					//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(strExp);
					//}
				//	DWORD dwFactureID = pFacturePage->GetSelectFactureItemsID();
				//	pFacturePage->UpdateFactureTree();
				//	pFacturePage->SetSelectFactureID(dwFactureID);
				//	pFacturePage->UpdateMaterial();
				//}
				//if (pLearnSkillPage!=NULL&&pLearnSkillPage->IsOpen())
				//{
				//	pLearnSkillPage->UpdataLearnSkillList(pLearnSkillPage->GetIsShowAllSkill());
				//	pLearnSkillPage->UpdataSpOrLev();
				//}
				
				//if (pFacLevelName!=NULL&&pFacLevelName->dwMinLevel==lFactureLevel)
				//{
				//	GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-27.wav");
				   // ������Ч
				//   pSPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwSkillLevelUpEffectID, pSPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
				//}
		}
		break;

	case MSG_S2C_PLAYER_ADDPOINT:
		{
			switch (pMsg->GetChar())
			{
			case 0:
				GetStr();
				break;
			case 1:
				GetDex();
				break;
			case 2:
				GetCon();
				break;
			}
		}
		break;

		// �ı侭��ֵ
	case MSG_S2C_PLAYER_ADDEXP:
		{
			// ��������
			DWORD dwType = pMsg->GetDWord();
			DWORD dwOldExp = GetExp()/*, dwOldVigour = GetVigour()*/;
			DWORD dwOldPreasntExp = GetPresentExp();
			DWORD dwOldExpMultiple = GetExpMultiple();
			DWORD dwNewExp = pMsg->GetDWord();
			DWORD dwExpMultiple = pMsg->GetDWord();
			DWORD dwPresentExp = pMsg->GetDWord();
			SetExp(dwNewExp);
			SetPresentExp(dwPresentExp);
			pPlayer->SetExpMultiple(dwExpMultiple);
			//CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			if(dwOldPreasntExp>0&&dwPresentExp==0)
			{
               // pMainPage->PresentExpTip();
			}
			//pMainPage->UpdataPlayerExp();
// 			if(pMainPage->GetPresentExpVisible())
// 			{
// 			      pMainPage->ShowPresentExp(true);
// 			}

			_int64 dbMultipleOldTemp = (_int64)dwOldExpMultiple*(_int64)dwExpMutipleValue;
			double dbExpOld = (double)(dbMultipleOldTemp+dwOldExp);

			_int64 dbMultipleNewTemp = (_int64)dwExpMultiple*(_int64)dwExpMutipleValue;
			double dbExpNew = (double)(dbMultipleNewTemp+dwNewExp);

			// ����ı�
			//long lChange = dwNewExp-dwOldExp;
			long lChange = (long)(dbExpNew-dbExpOld);

			// ���������ַ���
			string strType = "";
			switch( dwType )
			{
				case	eAET_FacBoon:
					if( lChange >= 1 )
                        strType = AppFrame::GetText("FA_506"); //��ḣ������
					break;

				default:
					strType = "";
					break;
			}

			// ��ʾ
			char str[256];
			if (lChange>=0 )
			{
                sprintf(str, AppFrame::GetText("Player_63") , strType.c_str(), lChange); //���õ���%s����:%d��
			}
			else if (lChange<0)
			{
				sprintf(str, AppFrame::GetText("Player_64"), strType.c_str(), -lChange); //����ʧ��%s����:%d��
			}
			//if( dwType != eAET_NPC && dwType != eAET_Script )
			//	pMainPage->AddOtherInfo(str,0xfffff600);

			if (dwType != eAET_FacBoon)
				AddTipText(CPlayer::TT_EXP, lChange);
		}
		break;
		// �ı�ְҵ����ֵ
	case MSG_S2C_PLAYER_ADDOCCUEXP:
		{
			// ��������
			DWORD dwType = pMsg->GetDWord();
			eOccupation byOccu = (eOccupation)pMsg->GetByte();
			DWORD dwNewExp = pMsg->GetDWord();
			DWORD dwOldExp = GetOccuExp(byOccu);
			SetOccuExp(byOccu,dwNewExp);
			CPlayer *pPlayer = GetGame()->GetMainPlayer();
			if(pPlayer==NULL)
				return;
			char str[256];
			if(dwNewExp >= dwOldExp)
			{
                _snprintf(str,256, AppFrame::GetText("Player_63"), //���õ���%s����:%d��
                    CPlayer::s_strOccupation[pPlayer->GetOccupation()].c_str(),dwNewExp-dwOldExp);
			}
			else
			{
                _snprintf(str, 256, AppFrame::GetText("Player_64"), //����ʧ��%s����:%d��
                    CPlayer::s_strOccupation[pPlayer->GetOccupation()].c_str(),dwOldExp-dwNewExp);
			}
			//CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//if(pMainPage)
			//	pMainPage->AddOtherInfo(str,0xfffff600);
			//pMainPage->UpdateSpExpBar();
			//if(pMainPage->GetSpExpVisble()) 
			//{
			//      pMainPage->ShowSpExp(true);
			//}
		}
		break;
	case MSG_S2C_PLAYER_ADDSP:
		{
			eOccupation byOccu = (eOccupation)pMsg->GetByte();
			DWORD dwNewSp = pMsg->GetDWord();
			DWORD dwOldSp = GetOccuSp(byOccu);
			SetOccuSp(byOccu,dwNewSp);
			char str[256];
			if(dwNewSp >= dwOldSp)
			{
                _snprintf(str, 256, AppFrame::GetText("CPlayer_65"), dwNewSp-dwOldSp);//���õ���Sp:%d��
			}
			else
			{
				_snprintf(str, 256, AppFrame::GetText("CPlayer_66"), dwOldSp-dwNewSp);//����ʧ��Sp:%d��
			}
			//CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//if(pMainPage)
			//	pMainPage->AddOtherInfo(str,0xfffff600);

			//CSkillPageEx *pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
			//if(pSkillPage && pSkillPage->IsOpen())
			//{
			//	pSkillPage->UpdataOccupExp();
			//	// ���µ�ǰְҵ��Ӧ�ļ����б�
			//	if(pSkillPage->GetCurActPage() == 0)
			//		pSkillPage->UpdataOccupSkill(pSkillPage->GetCurSelectedOccup(),pSkillPage->GetCurOccupSkillPage());
			//}

			//CLearnSkillPage *pLearnSkill = GetGame()->GetCGuiEx()->GetLearnSkillPage();
			//if(pLearnSkill)
			//	pLearnSkill->UpdataSpOrLev();
		}
		break;
	case MSG_S2C_PLAYER_CREDIT_UPDATE:
		{
			DWORD dwZeroID = pMsg->GetDWord();
			DWORD dwCreditValue = pMsg->GetDWord();
			pPlayer->SetCreditForZone(dwZeroID,dwCreditValue);
			//CCharacterPageEx* pCharacterPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
			//if (pCharacterPage!=NULL&&pCharacterPage->IsOpen())
			//{
			//	pCharacterPage->UpdatePlayerInfor();
			//}
			GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-28.wav");
			// ������Ч
			pPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwReputeLevelUpEffectID, pPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
		}break;
	case MSG_S2C_PLAYER_CHANGE_CONTRIBUTE:
		{
			DWORD dwContribute = pMsg->GetDWord();
			if( dwContribute >= 0 )
			{
				SetCountryContribute(dwContribute);
			}
		}
		break;
		// �յ���������
	case MSG_S2C_PLAYER_LEVELUP:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			DWORD dwMaxHp=pMsg->GetDWord();
			WORD wMaxMp=pMsg->GetWord();
			WORD wLevel=pMsg->GetWord();
			DWORD dwExp=pMsg->GetDWord();
			DWORD dwExpMultiple=pMsg->GetDWord();
			DWORD dwUpExp=pMsg->GetDWord();
			DWORD dwNextExpMultiple=pMsg->GetDWord();
			DWORD dwPresentExp = pMsg->GetDWord();

			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
			if (pPlayer==NULL)
				break;
			pPlayer->SetMaxHp(dwMaxHp);
			pPlayer->SetMaxMp(wMaxMp);

			pPlayer->SetHp(dwMaxHp);
			pPlayer->SetMp(wMaxMp);

			pPlayer->SetPresentExp(dwPresentExp);
			pPlayer->SetLevel(wLevel);

			// �����Լ�
			if( PlayerID == GetGame()->GetMainPlayer()->GetExID() )
			{
				pPlayer->SetExp(dwExp);
				pPlayer->SetUpgradeExp(dwUpExp);
				pPlayer->SetExpMultiple(dwExpMultiple);
		     	pPlayer->SetNextExpMultiple(dwNextExpMultiple);
				//CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
				CPlayer* pPlayer = GetGame()->GetMainPlayer();				
				char str[256];
                sprintf(str, AppFrame::GetText("Player_67"), pPlayer->GetLevel());  //���� Lev:%d
				//if(pMainPage)
				//{
				//	pMainPage->UpdatePlayerInforHar();
				//	pMainPage->AddText(eNOTIFYPOS_RIGHT_BOTTOM,str);
				//}
				//CSkillPageEx * pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
				//if(pSkillPage)
				//	pSkillPage->UpdataTipEffect();
				// ����������������б�
				pPlayer->TidySearchQuestList();

				// ���壺������Ч
				//GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( eID_EFFECT_PlayerLevelUpgrade );
			}
			//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
			//if(pObjPage && pObjPage->GetObjShape() == pPlayer)
			//{
			//	pObjPage->SetObjectName();
			//	pObjPage->SetObjectHpInfo(); 
			//}
            if ( pPlayer == GetGame()->GetGameControl()->GetSelectedShape() )
            {
                FireUIEvent("ObjectPage", "ObjectAttr");
            }

			//CLookPageEx *pViePage = GetGame()->GetCGuiEx()->GetLookPageEx();
			//if(pViePage && pViePage->GetSelectPlayer() == pPlayer)
			//{
            //  pViePage->UpdateRoleInfo();
			//}
			//����������Ч
			GetGame()->GetAudioList()->Play3DSound("sounds\\levelup.wav",1, pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX());
			// ������Ч
			pPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwLevelUpEffectID, pPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
		}
		break;
		// �յ�ְҵ�ȼ���������
	case MSG_S2C_PLAYER_OCCULEVELUP:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			DWORD dwMaxHp=pMsg->GetDWord();
			WORD wMaxMp=pMsg->GetWord();
			eOccupation byOccu = (eOccupation)pMsg->GetByte();
			BYTE bOccuLevel=pMsg->GetByte();
			DWORD dwOccuExp=pMsg->GetDWord();
			DWORD dwOccuUpExp=pMsg->GetDWord();
			
			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
			if (pPlayer == NULL)
				break;
			pPlayer->SetMaxHp(dwMaxHp);
			pPlayer->SetMaxMp(wMaxMp);

			pPlayer->SetHp(dwMaxHp);
			pPlayer->SetMp(wMaxMp);

			pPlayer->SetOccuLvl(byOccu,bOccuLevel);
			// �����Լ�
			if( PlayerID == GetGame()->GetMainPlayer()->GetExID() )
			{
				pPlayer->SetOccuExp(byOccu,dwOccuExp);
				pPlayer->SetOccuUpgradeExp(dwOccuUpExp);
				// ���½���
				/*if(GetGame()->GetCGuiEx()->GetCharacterPageEx())
					GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdatePlayerInfor();
				if(GetGame()->GetCGuiEx()->GetSkillPageEx())
					GetGame()->GetCGuiEx()->GetSkillPageEx()->UpdataOccupExp();

				CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
				if(pMainPage)
				{
					pMainPage->UpdateSpExpBar();
					if(pMainPage->GetSpExpVisble()) 
					{
						pMainPage->ShowSpExp(true);
					}
			    }*/

				// ������ҵ�ְҵ����ְҵ���ϵķ�Ӧ
				//CSkillPageEx *pPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
				//if(pPage && pPage->IsOpen())
				//	pPage->UpdataOccupList();

				// ���壺������Ч
				//GetGame()->GetCGuiEx()->GetEffectUIManager()->AddGroup( eID_EFFECT_OccuLevUpgrade );
		     }
			//CLookPageEx *pViePage = GetGame()->GetCGuiEx()->GetLookPageEx();
			//if(pViePage && pViePage->GetSelectPlayer() == pPlayer)
			//{
             //  pViePage->UpdateRoleInfo();
			//}
			//����������Ч
			GetGame()->GetAudioList()->Play3DSound("sounds\\levelup.wav",1, pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX());
			// ������Ч
			pPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwOccuLevelUpEffectID, pPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
		}
		break;
	case MSG_S2C_PLAYER_USEITEM:  //gameserver����ʹ��һ�������Ե��ߵĽ��
		{
			int result=pMsg->GetChar();
			if(result==PLAYER_USEITEM_SUCCEED)
			{
				CGUID PlayerID;
				pMsg->GetGUID(PlayerID);
				DWORD dwGoodsIndex = pMsg->GetDWord();

				if (PlayerID == pPlayer->GetExID())
				{
					//���ø���������Ʒ����ȴʱ�估״̬
					list<CPlayer::tagShortCutGoods>* ListSCG = GetShortCutGoodsList();
					list<CPlayer::tagShortCutGoods>::iterator ListSCGIter = (*ListSCG).begin();
					for (;ListSCGIter!=(*ListSCG).end();ListSCGIter++)
					{
						if (CGoodsList::GetEffectVal(dwGoodsIndex,GAP_COOLDOWN,2)==0)
						{
							if (GetGoodsUseState(dwGoodsIndex))
							{
								SetGoodsCoolDownStartTime(dwGoodsIndex,timeGetTime());
								SetGoodsUseState(dwGoodsIndex,false);
							}
							break;
						}
						if (CGoodsList::GetEffectVal(dwGoodsIndex,GAP_COOLDOWN,2)==CGoodsList::GetEffectVal(ListSCGIter->wID,GAP_COOLDOWN,2))
						{
							if (GetGoodsUseState(ListSCGIter->wID))
							{
								SetGoodsCoolDownStartTime(ListSCGIter->wID,timeGetTime());
								SetGoodsUseState(ListSCGIter->wID,false);
							}
						}
					}
				}

				// ������Ч
				CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
				if (pPlayer)
				{
					CGoodsList::tagGoods2 *pGoods = CGoodsList::GetProperty(dwGoodsIndex);
					if (pGoods)
					{
						long lEffect = pGoods->BaseProperty.dwEquipID;
						if (lEffect)
						{
							float x = 0.f, y = 0.f, height = 0.f;
							pPlayer->GetLocatorPos(CShape::LOCATOR_HEAD, x, y, height);
							bool bAsyn = true;
							if(pPlayer == GetGame()->GetMainPlayer())
								bAsyn = false;
							((CClientRegion*)pMsg->GetRegion())->PlayEffect(lEffect, x, y, height,-1.f,bAsyn);
						}
						if(pPlayer == GetGame()->GetMainPlayer())
						{
							GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->BaseProperty.dwIndex);
						}

					}
				}
			}
			else
			{
				result=pMsg->GetChar();
				switch(result)
				{
				case PLAYER_USEITEM_SKILLTOOLOW:
					{
						/*GetGame()->GetInterface()->AddOtherInfo("����ѧϰ�ȼ����ڻ���ڵ�ǰ�ȼ��ļ���",
						CGlobeSetup::GetSetup()->colorWarningText);*/
					}
					break;
				case PLAYER_USEITEM_SKILLTOOHIGH:
					{
						//GetGame()->GetInterface()->AddOtherInfo("�������ȼ�ѧϰ����",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case LEVEL_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("�ȼ�����,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case STR_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("��������,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case DEX_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("���ݲ���,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case CON_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("���ʲ���,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case INT_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("��������,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case OCCUPATION_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("ְҵ��ƥ��,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case SEX_NOTFIT:
					{
						//GetGame()->GetInterface()->AddOtherInfo("�Ա�ƥ��,ʹ��ʧ��",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				case REGION_FORBID:
					{
						//GetGame()->GetInterface()->AddOtherInfo("�˳�����ֹʹ�ø���Ʒ",CGlobeSetup::GetSetup()->colorWarningText);
					}
					break;
				}
			}

		}
		break;
	case MSG_S2C_PLAYER_CHANGEMAXATK://���ĵ���������󹥻���
		{
			WORD wMaxAtk=pMsg->GetWord();
			pPlayer->SetMaxAtk(wMaxAtk);
		}
		break;
	case MSG_S2C_PLAYER_CHANGEATKSPEED://���ĵ������������ٶ�
		{
			//WORD wSpeed=pMsg->GetWord();
			//pPlayer->SetAtcSpeed(wSpeed);
		}
		break;
	case MSG_S2C_PLAYER_CHANGEDEF://���ĵ��߸ı������
		{
			WORD wDef=pMsg->GetWord();
			pPlayer->SetDef(wDef);
		}
		break;
	case MSG_S2C_PLAYER_CHANGEELEMENTMODIFY://���ĵ��߸ı䷨���˺�ϵ��
		{
			//short sElementModify=pMsg->GetShort();
			//pPlayer->SetElementModify(sElementModify);
		}
		break;

		// �Զ��ָ�hp��Ϣ
	case MSG_S2C_PLAYER_RESUME:
		{
			short chp = pMsg->GetShort();
			short cmp = pMsg->GetShort();
			SetHp(GetHp()+chp);
			SetMp(GetMp()+cmp);
			//CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//pMainPage->UpdataPlayerExp();
		}
		break;

	case MSG_S2C_PLAYER_HELPRESPONSE://��������Ľ������
		{
			char strContent[2048];
			long lScriptID = pMsg->GetLong();
			pMsg->GetStr(strContent,2048);
			/*GetGame()->GetInterface()->GetHelpPage()->SetMessageText(strContent);
			GetGame()->GetInterface()->GetHelpPage()->SetScriptID(lScriptID);*/
		}
		break;

		// ��Ӽ���
	case MSG_S2C_PLAYER_ADDSKILL:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			long lSkillId = pMsg->GetLong();
			WORD wSkillLevel = pMsg->GetWord();
			DWORD dwRestoreTime = pMsg->GetDWord();
            // ���ܷ�Χ��Ϊfloat TODO:�޸Ŀͻ��˼��ܷ�Χ����߼�
			float fRangeMin = pMsg->GetFloat();
			float fRangeMax = pMsg->GetFloat();
			CPlayer * pCurPlayer = pPlayer;
			// ���˼�������
			if(guid == pPlayer->GetExID())
			{
				if(lSkillId < SKILL_DEPUTY_OCCU)
				{
					pMsg->GetPlayer()->AddSkill(lSkillId, 
						wSkillLevel, 
						dwRestoreTime,
						fRangeMin,
						fRangeMax);
					
					/////////////////////////////////////////////////
					// zhaohang  2010/4/26 
					// �µļ���UI����
					char listboxname[256];
					size_t idxInTabCtl = 0;
					sprintf(listboxname,"SkillTemplate/Tab/Tab %d/ListBox",idxInTabCtl+1);
					FireUIEvent(listboxname,"AddCanUseSkill");
					/////////////////////////////////////////////////
					
					//CSkillPageEx *pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
					//if(pSkillPage && pSkillPage->IsOpen())
					//{
					//	pSkillPage->UpdataOccupExp();
					//	// ����ϰ��ҳ��
					//	if(pSkillPage->GetCurActPage() == 1)
					//		pSkillPage->UpdataPlayerSkillList();
					//	// ������������,���µ�ǰְҵ��Ӧ�ļ����б�
					//	else
					//	{
					//		pSkillPage->PlayLevelUpEffect(10);
					//		pSkillPage->UpdataOccupSkill(pSkillPage->GetCurSelectedOccup(),pSkillPage->GetCurOccupSkillPage());
					//	}
					//}
					//GetGame()->GetCGuiEx()->GetLearnSkillPage()->ReSetLearnSkillList();
				}
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-25.wav");
			}
			// ��������������
			else if(GetGame()->GetRegion())
				pCurPlayer = dynamic_cast<CPlayer *>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,guid));
			// ������Ч
			if(pCurPlayer)
			{
				pCurPlayer->AddEffectLocator(CGlobeSetup::GetSetup()->dwSkillLevelUpEffectID, pCurPlayer, LOCATOR_FOOTCENTER,false,0.0f,0,false);
			}
		}
		break;

		// ɾ������
	case MSG_S2C_PLAYER_DELSKILL:
		{
			/*char strSkillName[256];
			pMsg->GetStr(strSkillName,256);*/
			DWORD dwSkillId = pMsg->GetDWord();
			pMsg->GetPlayer()->DelSkill(dwSkillId);
			//GetGame()->GetCGuiEx()->GetSkillPageEx()->UpdataPlayerSkillList();
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->ReSetLearnSkillList();
			//	GetGame()->GetInterface()->GetSkillPage()->UpdateList();
		}
		break;
	case MSG_S2C_PLAYER_SETLEVEL://������ҵȼ�
		{
			WORD  lLevel=pMsg->GetWord();			
			DWORD dwCurrentExp=pMsg->GetDWord();
            DWORD dwExpMultiple=pMsg->GetDWord();
			DWORD dwNextExp=pMsg->GetDWord();
            DWORD dwNextExpMultiple=pMsg->GetDWord();
			
			pPlayer->SetLevel(lLevel);
			pPlayer->SetExp(dwCurrentExp);
			pPlayer->SetUpgradeExp(dwNextExp);
            pPlayer->SetExpMultiple(dwExpMultiple);
            pPlayer->SetNextExpMultiple(dwNextExpMultiple);
			//	GetGame()->GetInterface()->GetSkillPage()->UpdateList();
		}
		break;

		// ��ʾ������Ŀ
	case MSG_S2C_PLAYER_SYNC_HITS:
		{
			long lHitNum = pMsg->GetLong();
			SetHitNum(lHitNum);
			SetLastHitTimer(GetCurTickCount());
		}
		break;

		// ɱ����
	case MSG_S2C_PLAYER_SYNC_PKCOUNT:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);

			long wPKValue = pMsg->GetLong();
			long wPVPValue = pMsg->GetLong();

			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
			if (pPlayer)
			{
				pPlayer->SetPkValue(wPKValue);
				pPlayer->SetPVPValue(wPVPValue);
			}
		}
		break;
	case MSG_S2C_PLAYER_CHANGE_EQUIPMENT:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			DWORD dwPos=pMsg->GetDWord();
			BYTE bMount=pMsg->GetByte();
			DWORD dwIndex=pMsg->GetDWord();
			LONG lLevel=pMsg->GetLong();
			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
			if ( pPlayer && pPlayer!=GetGame()->GetMainPlayer() )
			{
				if( bMount==1 )
				{
					pPlayer->SetEquipIndex(dwPos,dwIndex);
					pPlayer->SetEquipLevel(dwPos,(BYTE)lLevel);
					pPlayer->UpdateWeaponShow(dwPos);
				}
				else
				{
					pPlayer->RemoveWeaponShow(dwPos);
					pPlayer->SetEquipIndex(dwPos,0);
					pPlayer->SetEquipLevel(dwPos,0);

				}
				pPlayer->SetDisplayModelGroup();
				// ˢ�¶���
				extern bool g_bForceUpdate;
				g_bForceUpdate=true;
				pPlayer->SetAction(ACT_STAND);
				//���²鿴װ���������ʾ
                //GetGame()->GetCGuiEx()->GetLookPageEx()->UpdatePage();
			}
		}
		break;
	case MSG_S2C_PLAYER_ANSWER_RELIVE:
		{
			WORD dwAction = pMsg->GetWord();
			WORD wState = pMsg->GetWord();
			DWORD dwHp = pMsg->GetDWord();
			WORD wMp = pMsg->GetWord();

			if (m_bIsNeedAlphaFade)
			{
				GetGame()->GetRegion()->GetAlphaFadeObjectList().AddFadeOutObject(this);
				m_bIsAddAlphaFaded = true;
			}

			SetState(wState);
			SetAction(dwAction);
			SetHp(dwHp);
			SetMp(wMp);
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
			//GetGame()->GetCGuiEx()->GetRelivePageEx()->Close();
			//GetGame()->GetCGuiEx()->GetRelivePageEx()->PlayReliveSound();
	
			// @todo ������
			//if (GetFightPetNum() > 0  &&  this == GetGame()->GetMainPlayer())
			//{
			//	//GetGame()->GetCGuiEx()->GetPetNamHpPage()->Open();
			//	//GetGame()->GetCGuiEx()->GetPetHotKeyBarPage()->Update();
			//}
		}
		break;
	case MSG_S2C_PLAYER_CHANGE_PROPERTIES:
		{
			//long lType=pMsg->GetLong();
			//CGUID PlayerID;
			//pMsg->GetGUID(PlayerID);			
			CPlayer *pPlayer=GetGame()->GetMainPlayer();
			if(pPlayer==NULL) break;
			CGUID GoodsGUID;
			pMsg->GetGUID(GoodsGUID);
			CGoods*pGoods = pPlayer->GetGoodsByGUID(GoodsGUID);
			// ���壺�ҳ����������Ʒ
			if( ! pGoods )
			{
				pGoods = dynamic_cast<CGoods*>( GetGame()->GetRegion()->FindChildObject( TYPE_GOODS, GoodsGUID ) );
			}
			if (pGoods)
			{
				pGoods->UpdateProperty(pMsg);
			}
		}
		break;

		// ��ʾ��������
	case MSG_S2C_PLAYER_UpdateShowFashion:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			BYTE byShowFashion = pMsg->GetByte();

			CPlayer *pPlayer=(CPlayer *)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,PlayerID);
			if (pPlayer == NULL)
				break;

			pPlayer->SetShowFashion(byShowFashion);

			pPlayer->SetManteauVisible(byShowFashion & FASHION_TYPE_MANTEAU);
			pPlayer->SetGreatcoatVisible(byShowFashion & FASHION_TYPE_GREATCOAT);
		}
		break;

		// �Զ�����
	case MSG_S2C_PLAYER_AUTOMOVE:
		{
			float lX = pMsg->GetFloat();
			float lY = pMsg->GetFloat();
			GetGame()->GetGameControl()->AutoMoveTo((float)lX, (float)lY);
		}
		break;

		// ����ǿ�ƹ���Ŀ��(һ�㴦���������Ƽ��ܵ����)
	case MSG_S2C_PLAYER_FORCE_TARGET:
		{
			CGUID TargetGuid;
			pMsg->GetGUID(TargetGuid);
			long lType = pMsg->GetLong();
			CShape *pShape=(CShape *)GetGame()->GetRegion()->FindChildObject(lType,TargetGuid);
			if(pShape)
				GetGame()->GetGameControl()->SetSelectedShape(pShape,false);
		}
		break;

		//����ʱʹ����ֵ���ߵ���Ϣ���
	case MSG_S2C_PLAYER_RELIVE_RESULT:
		{
			//GetGame()->GetCGuiEx()->GetRelivePageEx()->ReliveOfHearEvent(pMsg); 
		}
		break;
		//����ʱ������ֵ���ߵ���Ϣ���
	case MSG_S2C_PLAYER_BUYRELIVEITEM_RESULT:
		{
			//������ߺ����ش���
			//GetGame()->GetCGuiEx()->GetRelivePageEx()->BuyItemEvent(pMsg);
		}
		break;
		//���ܸ���������
	case MSG_S2C_PLAYER_SKILL_RELIVE:
		{
			//�򿪽��ܸ���������
			//if(!GetGame()->GetCGuiEx()->GetRelivePageEx()->IsOpen())
			//{
			//	GetGame()->GetCGuiEx()->GetRelivePageEx()->Open();
			//}
			//GetGame()->GetCGuiEx()->GetRelivePageEx()->SetAcceptRelivePage(pMsg->GetLong());
		}
		break;
		//�ı�ƺŵķ��ؽ��
	case MSG_S2C_PLAYER_UPDATE_TITLE_TYPE:
		{
			CGUID PlayerID;
			char strFile[MAX_PATH];
			pMsg->GetGUID(PlayerID);
			long lTitleType=pMsg->GetLong();
			pMsg->GetStr(strFile, MAX_PATH);
			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, PlayerID);
			if (pPlayer==NULL)
				break;
			pPlayer->SetTitleType(lTitleType);
			pPlayer->SetPlayerTitle(strFile);

			//���������
			if(pPlayer==GetGame()->GetMainPlayer())
			{
				//GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetTitleShow(lTitleType,strFile);
			}
		}
		break;
		//�ı����ֵ
	case MSG_S2C_PLAYER_ACTIVE_CHANGE:
		{
			long CurCp = pMsg->GetLong();
			pPlayer->SetCurrBaseCP(CurCp);
			long CurExCp = pMsg->GetLong();
			pPlayer->SetCurrExCP(CurExCp);
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdateLivePoint();
		}
		break;
	case MSG_S2C_PLAYER_SHOW_SPOUSENAME:
		{
			BYTE isShowLoverName = pMsg->GetByte();
			GetGame()->GetMainPlayer()->SetIsShowLoverName(isShowLoverName>0);
			//GetGame()->GetCGuiEx()->GetFriendsPageEx()->RefreshLinkmanListUI();
            //GetGame()->GetCGuiEx()->GetCharacterPageEx()->ChangeTitleMsg(ePTT_LOVER);
		}
		break;

	// ���壺ħ��Ԫ�����Ը���
	case MSG_S2C_PLAYER_ELEMENT_PROPERTY_CHANGE:
		{
			BYTE byMT = pMsg->GetByte();
			BYTE byAD = pMsg->GetByte();
			long lVal = pMsg->GetLong();

			GetGame()->GetMainPlayer()->SetMagicElem( (eAttackDefend)byAD, (eMagicElement)byMT, lVal );
			//GetGame()->GetCGuiEx()->GetCharacterPageEx()->UpdateMagicInfo();
		}
		break;
	}

}

