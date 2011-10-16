#include "stdafx.h"
#include "Player.h"
#include "Message.h"
#include "Effect.h"
#include "ClientRegion.h"
#include "Skills/SkillXml.h"
#include "Skills/SkillListIni.h"
#include "../GameClient/GameControl.h"
#include "Other/AudioList.h"
#include "MainPlayerHand.h"
#include "PetSystem/Pet.h"
#include "Other/DeputyOccuSystemClient.h"
#include "../GameClient/Game.h"
#include "../../../Public/Common/SkillRelated.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern void UpdateHotKeyCoolDownEffect(long lSkillID);
// ��ӦSKILL��Ϣ
void OnSkillMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2C_SKILL_LOCK_GOODS:
		{
			long lExtendID = pMsg->GetLong();
			long lPos = pMsg->GetLong();
			CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
			if (pMainPlayer!=NULL)
			{
				CGoods* pGoods = pMainPlayer->GetGoodsByPosition(lExtendID,lPos);
				if (pGoods!=NULL&&!pGoods->IsLocked())
				{
					pGoods->Lock();
					//CItemPageEx* pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
					//if (pPage!=NULL)
					//{
					//	pPage->UpdateGoodsShow(lExtendID,(int)lPos);
					//}
				}
			}
		}
		break;
	case MSG_S2C_SKILL_UNLOCK_GOODS:
		{
			DWORD lExtendID = pMsg->GetDWord();
			DWORD lPos = pMsg->GetDWord();
			CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
			if (pMainPlayer!=NULL)
			{
				CGoods* pGoods = pMainPlayer->GetGoodsByPosition(lExtendID,lPos);
				if (pGoods!=NULL&&pGoods->IsLocked())
				{
					pGoods->UnLock();
					CMainPlayerHand* pHand = 0;//GetGame()->GetCGuiEx()->GetPMainPlayerHand();
					if (pHand!=NULL&&pHand->GetPGoodsOfMainPlayerHand()!=NULL&&
						pHand->GetPGoodsOfMainPlayerHand()->GetExID()==pGoods->GetExID())
					{
						pGoods->SetHaveShadowState(true);
					}
				}
			}
		}
		break;
	case MSG_S2C_SKILL_STATE_CHANGE:
		//##ͬ��״̬��Ϣ
		{
			LONG lType	= pMsg -> GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			DWORD dwHP	= pMsg -> GetDWord();
			WORD wMP	= pMsg -> GetWord();
			WORD eg		= pMsg -> GetWord();
			if(lType==TYPE_PLAYER)
			{
				CPlayer* pMainPlayer = pMsg -> GetPlayer();
				//���Լ���ı��Լ�
				if(pMainPlayer->GetExID() == ID)
				{
					pMainPlayer -> SetHp( dwHP );
					pMainPlayer -> SetMp(wMP);
					pMainPlayer -> SetEnergy(eg);
					//pMainPlayer -> SetRP( wRP );
					//pMainPlayer -> SetYP( wYP );
				}
				else
				{
					//���Ƿ��Ƕ���
					vector<CPlayer::tagTeammate>& vTeammate = pMainPlayer -> GetTeam();
					for( size_t i = 0; i < vTeammate.size(); i ++ )
					{
						if( vTeammate[i].ID == ID )
						{
							vTeammate[i].wHP = (WORD)dwHP;
							break;
						}
					}
				}
			}
			else
			{
				//�ı����״̬
				CMoveShape* pShape = (CMoveShape*)GetGame()->GetRegion()->FindChildObject(lType, ID);
				if (pShape)
				{
					pShape->SetHp(dwHP);
				}
			}
		}
		break;
	case MSG_S2C_SKILL_USE:
		{
			CHAR cResult = pMsg->GetChar();
			DWORD dwCurTime = GetCurTickCount();
			switch(cResult)
			{
			case SKILL_USE_RESULT_BEGIN:
				{
					CGUID ID;
					pMsg->GetGUID(ID);						

					float fDir = pMsg->GetFloat();			// �ͷ��߳���

					long lSkillKey = pMsg->GetDWord();		// �ӷ�����ȡ�ü��ܼ�ֵ��ID
					long lSkillID = pMsg -> GetLong();
					long lLevel = pMsg->GetByte();			// ���ܵȼ�
					long lCurType = pMsg->GetByte();		// ���ܵ��ͷ�״̬������/�ͷţ�
					long lTime = pMsg->GetLong();			// ���ܵ�����ʱ�䣨�����׶����ã�
					long lRestoreTime = pMsg->GetLong();	// ���ܵ���ȴʱ��

					long lDestX = pMsg->GetShort();			// �ͷ�Ŀ�������(x,y)
					long lDestY = pMsg->GetShort();	
					long lTypeSize = pMsg->GetByte();		// �ͷ�Ŀ��ĸ���
					vector<long> vecType;					// Ŀ�������б�
					vector<CGUID> vecID;					// Ŀ�����͵�GUID
					CGUID destID;
					for (int i =0; i<lTypeSize; i++)
					{
						vecType.push_back(pMsg->GetLong());
						pMsg->GetGUID(destID);
						vecID.push_back(destID);
					}
					// ȷ��������������
					if(lSkillID == SKILL_BASE_BUFF)
						lSkillID = GetGame()->GetMainPlayer()->GetDefaultAttackSkillID();
					// �����ͷ���
					CMoveShape* pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(ID);
					if(pMoveShape)
					{
						long lType = pMoveShape->GetType();
						//�ж��Ƿ��ǹ������ǹ�����Ź�����Ч
						if(lType == TYPE_MONSTER)
						{
							GetGame()->GetAudioList()->Play(pMoveShape,TYPE_MONSTER,0xFFF0,Vector3(pMoveShape->GetPosY(),pMoveShape->GetHeight(),pMoveShape->GetPosX()));
						}

						if(lCurType == 1)		// �����׶�
						{
							CPlayer::tagSkill *ptagS = GetGame()->GetMainPlayer()->GetSkill(lSkillID);
							if(ptagS)
							{
                                UpdateHotKeyCoolDownEffect(lSkillID);
							}
							if(pMoveShape->GetType() == TYPE_PET)
							{
								CPet *pPet = (CPet*)pMoveShape;
								//���µ�ǰ���ܵĿ�ʼ��ȴʱ��
								tagPetSkillInfo *pSkill = pPet->GetPetSkillInfoByID(lSkillID);
								if(pSkill)
									pSkill->lRestorTimeLength = lRestoreTime;

								pPet->SkillSuccReceived();
							}
							/***********************************************************************/
							/* �Ժ� fix */
							//CSkillXml *m_pSkill = new CSkillXml();
							/***********************************************************************/
							ConcreteSkillComp* SkillCom = new ConcreteSkillComp();
							//SkillCom���ڴ潻����CSkillXml������������
							CSkillXml* pSkill = new ConcreteDecSkillXml(SkillCom); 
							CSkillListXml::tagSkill *pTagSkill = CSkillListXml::GetProperty(lSkillID);
							if (pTagSkill && pTagSkill->dwSkillType == CSkillListXml::SKILLTYPE_PLAY)	// ֱ��ʩ��
							{
								// �����༼�������������ʾʱ��
								DWORD dwlev = pTagSkill->vectorLevels.size() >= (DWORD)lLevel? (DWORD)lLevel-1:((DWORD)pTagSkill->vectorLevels.size() - 1);
								long lAiID = pTagSkill->vectorLevels[dwlev].dwSkillLevelAI;
								if(pTagSkill->bIsLead)
									pSkill->IntonateTime(lTime);
								// ֱ�Ӵӿ�ʼ�׶�ִ��
								if(pSkill->StepBegin(lSkillID, lLevel, lType, ID, lDestX, lDestY,fDir,vecType, vecID))
								{
									pSkill->SetSkillKey(lSkillKey);
									pMoveShape->AddCurSkillList(lSkillKey,pSkill);
								}
								else
									SAFE_DELETE(pSkill);
								// �����ʼ�׶γɹ���ֱ��ִ��ʩ�Ž׶�
								if(pSkill&&pSkill->StepRun(lSkillID, lLevel, lType, ID, lDestX, lDestY, fDir, vecType, vecID) == false)
								{
									//pSkill->SetIsDeleted(true);
									pMoveShape->DeleteCurSkill(lSkillKey);
								}
								if(lType == TYPE_PLAYER )
								{
									if (GetGame()->GetMainPlayer()->GetExID()==ID)
									{
										GetGame()->GetGameControl()->SetControlAble(true);
										GetGame()->GetGameControl()->SetMoveAble(true);
										/***********************************************************************/
										/* zhaohang fix */
										/***********************************************************************/
										// �����༼��
										if(pTagSkill->bIsLead)
											GetGame()->GetMainPlayer()->SetIntonateSkillKey(lSkillKey);
										else
											GetGame()->GetMainPlayer()->SetIntonateSkillKey(0);
										// �Զ������ۼƷ��ʹ�����0
										GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
										if(g_bDebug)
										{
											Log4c::Trace(ROOT_MODULE,"ֱ��ʹ�ü��ܣ�������0��%d,key:%d",lSkillID,lSkillKey);
										}
									}else if (lSkillID>=50000&&lSkillID<=50006)
									{
										if (lSkillID>=50000&&lSkillID<=50002)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_Collection);
										}else if (lSkillID>=50003&&lSkillID<=50005)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_Facture);
										}else if (lSkillID==50006)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_TaskCollection,lLevel);
										}
									}
									/////////////////////////////////////////////////////////////////
									//char strInfo[512]="";
									//_snprintf(strInfo,512,"SKILLTYPE_PLAY_Set Intonate Skill Key:%d",0);
									//PutStringToFile("IntonateSkillTest",strInfo);
									/////////////////////////////////////////////////////////////////
								}
								else if (lType == TYPE_PET)
								{
										CPet *pPet = (CPet*)pMoveShape;
										if (pPet->IsMainPlayerPet())
										{
											/***********************************************************************/
											/* zhaohang fix */
											/***********************************************************************/
											if(pTagSkill->bIsLead)
												pPet->SetIntonateSkillKey(lSkillKey);
											else
												pPet->SetIntonateSkillKey(0);
										}
								}
							}
							else if(pTagSkill && pTagSkill->dwSkillType == CSkillListXml::SKILLTYPE_INTONATE) // ����ʩ��
							{
								pSkill->IntonateTime(lTime);
								if(g_bDebug && lType == TYPE_PLAYER)
								{
									dwCurTime = GetCurTickCount();
									Log4c::Trace(ROOT_MODULE,"������������ʱ��:%d,%d,%d",lSkillID,lTime,dwCurTime);
								}
								/***********************************************************************/
								/* zhaohang fix */
								/***********************************************************************/
								bool bIsLead = pTagSkill->bIsLead;
								pTagSkill->bIsLead = false;
								if(pSkill->StepBegin(lSkillID, lLevel, lType, ID, lDestX, lDestY,fDir,vecType, vecID))
								{
									/***********************************************************************/
									/* zhaohang fix */
									/***********************************************************************/
									pTagSkill->bIsLead = bIsLead;
									pSkill->SetSkillKey(lSkillKey);
									pMoveShape->AddCurSkillList(lSkillKey,pSkill);
								}
								else
									SAFE_DELETE(pSkill);
								if( lType == TYPE_PLAYER )
								{
									if (GetGame()->GetMainPlayer()->GetExID()==ID)
									{
										// �����׶���������ƶ�ȡ������
										GetGame()->GetGameControl()->SetControlAble(true);
										GetGame()->GetGameControl()->SetMoveAble(true);
										GetGame()->GetMainPlayer()->SetIntonateSkillKey(lSkillKey);

										// �Զ������ۼƷ��ʹ�����0
										GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
										/*if(g_bDebug)
										{
											char strInfo[512]="";
											_snprintf(strInfo,512,"���������У�������0��%d,key:%d",lSkillID,lSkillKey);
											PutStringToFile("SkillTimesTest",strInfo);
										}*/
									}else if (lSkillID>=50000&&lSkillID<=50006)
									{
										if (lSkillID>=50000&&lSkillID<=50002)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_Collection);
										}else if (lSkillID>=50003&&lSkillID<=50005)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_Facture);
										}else if (lSkillID==50006)
										{
											((CPlayer*)pMoveShape)->SetToolsID(CDeputyOccuSystemClient::OB_TaskCollection,lLevel);
										}
									}

									/////////////////////////////////////////////////////////////////
									//char strInfo[512]="";
									//_snprintf(strInfo,512,"SKILLTYPE_INTONATE_Set Intonate Skill Key:%d",lSkillKey);
									//PutStringToFile("IntonateSkillTest",strInfo);
									/////////////////////////////////////////////////////////////////
								}
							}
							else
							{
								SAFE_DELETE(pSkill);
								return;
							}
							pMoveShape->SetDirEx(fDir);
							// ���¿��������ȴ����
							if(pMoveShape==GetGame()->GetMainPlayer())
							{
								GetGame()->GetMainPlayer()->UpdateSkillCoolDownTime(true);
								//CSkillListXml::tagSkill *pS = CSkillListXml::GetProperty(lSkillID);
								/***********************************************************************/
								/* zhaohang fix */
								/***********************************************************************/
								if(ptagS && pTagSkill && pTagSkill->dwSkillState != 0 && !pTagSkill->bLockSkill)
								{
									ptagS->dwUseingStartTime = timeGetTime();
								}
							}

						}
						else					// ʩ�Ž׶�
						{
							/***********************************************************************/
							/* �Ժ� fix */
							//CSkillXml *m_pSkill = new CSkillXml();
							/***********************************************************************/
							ConcreteSkillComp* SkillCom = new ConcreteSkillComp();
							//SkillCom���ڴ潻����CSkillXml������������
							CSkillXml* pSkill = new ConcreteDecSkillXml(SkillCom); 
							pSkill->IntonateTime(lTime);
							if(pSkill&&pSkill->StepRun(lSkillID, lLevel, lType, ID, lDestX, lDestY, fDir, vecType, vecID) )
							{
								pSkill->SetSkillKey(lSkillKey);
								pMoveShape->AddCurSkillList(lSkillKey,pSkill);
								CSkillListXml::tagSkill *pTagSkill = CSkillListXml::GetProperty(lSkillID);
								// �����༼��
								if(pTagSkill && pTagSkill->bIsLead)
									GetGame()->GetMainPlayer()->SetIntonateSkillKey(lSkillKey);
							}
							else
							{
								SAFE_DELETE(pSkill);
							}

							if(lType == TYPE_PLAYER )
							{
								if (GetGame()->GetMainPlayer()->GetExID()==ID)
								{
									GetGame()->GetGameControl()->SetControlAble(true);

									GetGame()->GetMainPlayer()->SetIntonateSkillKey(0);

									// �Զ������ۼƷ��ʹ�����0
									GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
									if(g_bDebug)
									{
										Log4c::Trace(ROOT_MODULE,"���������ͷŽ׶Σ�������0��������գ�%d,key:%d",lSkillID,lSkillKey);
									}
								}
								if (lSkillID>=50000&&lSkillID<=50006)
								{
									((CPlayer*)pMoveShape)->SetToolsID(0);
								}
								/////////////////////////////////////////////////////////////////
								//char strInfo[512]="";
								//_snprintf(strInfo,512,"STEP_PLAY Set Intonate Skill Key:%d",0);
								//PutStringToFile("IntonateSkillTest",strInfo);
								/////////////////////////////////////////////////////////////////
							}
						}
					}
					else
					{
						/////////////////////////////////////////////////////////////start
						//test add or delete shape,(author:wangqiao)
						char pszTestInfo[1024]="";
						char pszGUID[64]="";
						ID.tostring(pszGUID);
						//_snprintf(pszTestInfo,1024,"When Use skill,the Firer is not exist.(type:%d,id:%s)",
						//	lType,pszGUID);
						//PutStringToFile("AddDelShapeTest",pszTestInfo);
						////////////////////////////////////////////////////////////end
					}
				}
				break;
			case SKILL_USE_RESULT_END:
				{
					CGUID ID;
					pMsg->GetGUID(ID);
					float fDir = pMsg->GetFloat();
					long lSkillKey = pMsg->GetDWord();				// �ӷ�����ȡ�ü��ܼ�ֵ

					long lSkillID = pMsg -> GetLong();
					long lLevel = pMsg->GetByte();

					long lCurType = pMsg->GetByte();
					CMoveShape* pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(ID);
					if(pMoveShape)
					{
						long lType = pMoveShape->GetType();
						CSkillXml* pSkill = pMoveShape->GetCurSkill(lSkillKey);
						if(pSkill)
						{
							pSkill->StepEnd(lSkillID, lLevel, lType, ID, fDir);
							pMoveShape->DeleteCurSkill(lSkillKey);
						}

						if(lType == TYPE_PLAYER )
						{
							if (lSkillID>=50000&&lSkillID<=50006)
							{
								((CPlayer*)pMoveShape)->SetToolsID(0);
							}
							if(GetGame()->GetMainPlayer()->GetExID() == ID)
							{
								// �Զ������ۼƷ��ʹ�����0
								GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
								GetGame()->GetGameControl()->SetControlAble(true);
								// �����ļ��������������ļ��ܣ�ȡ������
								if(lSkillKey == GetGame()->GetMainPlayer()->GetIntonateKey())
									//|| lSkillID == GetGame()->GetMainPlayer()->GetIntonateState())
								{
									GetGame()->GetMainPlayer()->SetCurIntonateTime(0,0);
									GetGame()->GetMainPlayer()->SetIntonateSkillKey(0);
									if(g_bDebug)
									{
										Log4c::Trace(ROOT_MODULE,"���ܽ�����������գ�������0��%d,key:%d",lSkillID,lSkillKey);
									}
								}
								else
								{
									// �⻷�༼������ʹ��״̬
									CPlayer::tagSkill *ptagS = GetGame()->GetMainPlayer()->GetSkill(lSkillID);
									if(ptagS)
										ptagS->dwUseingStartTime = 0;
								}
								// ��ʹ�õ��������ļ���ʱ����������Ĵ���
								if (lSkillID>=50003&&lSkillID<=50005)
								{
									//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
									//if (lCurType==2&&pFacturePage!=NULL)
									//{
									//	pFacturePage->UpdateInputNum();
									//	pFacturePage->SetFactureState(false);
									//	if ((CFacturePageEx::eErrorNotice)pFacturePage->IsCanFacture()==CFacturePageEx::eErrorNotice_NoError)
									//	{
									//		pFacturePage->SendFactureOneMes();
									//	}
										//pFacturePage->UpdateFactureLevelExpProcessBar();
									//}		
								}
							}
						}
						if (lType == TYPE_PET)
						{
							CPet *pPet = (CPet*)pMoveShape;
							if (pPet->IsMainPlayerPet())
							{
								if(lSkillKey == pPet->GetIntonateKey()
									&& lSkillID == pPet->GetIntonateState())
								{
									GetGame()->GetMainPlayer()->SetCurIntonateTime(0,0);
									GetGame()->GetMainPlayer()->SetIntonateSkillKey(0);
								}
								tagPetSkillInfo *pPetSkill = pPet->GetPetSkillInfoByID(lSkillID);
								if(pPetSkill)
									pPetSkill->bIsSkillUsingEnd = true;
							}
						}
					}
				}
				break;
			case SKILL_USE_RESULT_FAILED:
				CGUID ID;
				pMsg->GetGUID(ID);

				long lSkillID = pMsg->GetLong();
				long lLevel = pMsg->GetByte();

				long lSkillKey = pMsg->GetLong();
				long lFaildRet = pMsg->GetByte();
				// ���ü��ܽ���
				CMoveShape* pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(ID);
				if(pMoveShape)
				{
					long lType = pMoveShape->GetType();
					CSkillXml* pSkill = pMoveShape->GetCurSkill(lSkillKey);
					if(pSkill)
					{
						pSkill->StepEnd(lSkillID, lLevel, lType, ID, pMoveShape->GetDirEx());
						pMoveShape->DeleteCurSkill(lSkillKey);
					}
					if(pMoveShape->GetType() == TYPE_PET)
					{
						CPet *pPet = (CPet*)pMoveShape;
						tagPetSkillInfo *pPetSkill = pPet->GetPetSkillInfoByID(lSkillID);
						if(pPetSkill)
							pPetSkill->bIsSkillUsingEnd = true;
						pPet->SkillFailReceived();
					}
					// ��������ļ��ܣ�����ʾ
					if( pMoveShape == GetGame()->GetMainPlayer())
					{
						// �Զ������ۼƷ��ʹ�����0
						GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
						// ����Ǳ�����ͷŵļ��ܣ���ʧ�ܼ���Ϊ�������ܣ�ֹͣ������Զ�����״̬
						CSkillListXml::tagSkill *pS = CSkillListXml::GetProperty(lSkillID);
						if(pS && pS->bLockSkill)
						{
							// �����������ܵ�mp���㣬�л�����������
							if(lFaildRet == SKILL_USE_FAILED_INVALID_MP)
							{
								DWORD dwID = GetGame()->GetMainPlayer()->GetDefaultAttackSkillID();
								GetGame()->GetGameControl()->SetBufferSkillID(dwID);
								CPlayer::tagSkill * pPlayerSkill = GetGame()->GetMainPlayer()->GetSkill(dwID);
								if(pPlayerSkill)
									pPlayerSkill->dwUseingStartTime = timeGetTime();
							}
							if(lFaildRet == SKILL_USE_FAILED_INVALID_HP || lFaildRet == SKILL_USE_FAILED_INVALID_RP)
								GetGame()->GetGameControl()->AutoAttactStop();
						}
						// �⻷�༼������ʹ��״̬
						CPlayer::tagSkill *ptagS = GetGame()->GetMainPlayer()->GetSkill(lSkillID);
						if(ptagS && pS && !pS->bLockSkill)
							ptagS->dwUseingStartTime = 0;
						// �����ļ��������������ļ��ܣ�ȡ������
						if(lSkillKey > 0 && lSkillKey == GetGame()->GetMainPlayer()->GetIntonateKey())
						{
							GetGame()->GetMainPlayer()->SetCurIntonateTime(0,0);
							GetGame()->GetMainPlayer()->SetIntonateSkillKey(0);
							if(g_bDebug)
							{
								Log4c::Trace(ROOT_MODULE,"����ʹ��ʧ�ܣ�������0��������գ�%d,���ͣ�%d,key:%d",lSkillID,lFaildRet,lSkillKey);
							}
						}
						if (lSkillID>=50000&&lSkillID<=50006)
						{
							((CPlayer*)pMoveShape)->SetToolsID(0);
						}
						// �������������������
						//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
						/*if (pFacturePage!=NULL && pFacturePage->IsOpen())
						{
							pFacturePage->SetFactureState(false);
							if (pFacturePage->GetCurFactureNum()!=0 || pFacturePage->GetAllFactureState())
							{
								pFacturePage->ClearFactureNum();
							}
						}*/
					}
				}
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-48.wav");
				if(pMoveShape && (pMoveShape == GetGame()->GetMainPlayer() ||
					pMoveShape->GetType() == TYPE_PET &&
					((CPet*)pMoveShape)->IsMainPlayerPet() ))
				{
					switch (lFaildRet)
					{
					case SKILL_USE_FAILED_INVALID_STATE:
						{
							// �Զ���������
							if(GetGame()->GetGameControl()->GetAutoSkillID() == lSkillID)
								GetGame()->GetGameControl()->SetAutoSkillID(0);
							//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							//CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_STATE),
							//D3DCOLOR_ARGB(255,255,0,0));
						}
						break;
					case SKILL_USE_FAILED_INVALID_HP:
						//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
						//	CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_HP),
						//	D3DCOLOR_ARGB(255,255,0,0));
						break;

					case SKILL_USE_FAILED_INVALID_MP:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MP),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_RP:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_NP),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_YP:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_YVALUE),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_TARGET:
						{
							// �Զ���������
							if(GetGame()->GetGameControl()->GetAutoSkillID() == lSkillID)
								GetGame()->GetGameControl()->SetAutoSkillID(0);
							//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							//	CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_OBJERROR),
							//	D3DCOLOR_ARGB(255,255,0,0));
						}
						break;

					case SKILL_USE_FAILED_INVALID_DISTANT:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
                            CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_OUTAREA),
                            D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_SKILL:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_NOSKILL),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_TIME:			//##�����ٴ�ʹ�ü��ܵ�ʱ��δ����
						{
							/*if(GetGame()->GetGameControl()->GetAutoSkillID() != lSkillID)
								GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_COOLTIME),
								D3DCOLOR_ARGB(255,255,0,0));
							else
								GetGame()->GetGameControl()->SetBufferSkillID(SKILL_BASE_BUFF);*/
							if( GetGame()->GetGameControl()->GetAutoSkillID() == lSkillID)
								GetGame()->GetGameControl()->SetBufferSkillID(SKILL_BASE_BUFF);
						}
						break;

					case SKILL_USE_FAILED_CANNOTCOLLECTION_STATE:	//��ǰ״̬(�����״̬)���ܲɼ�
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(205,18),
								D3DCOLOR_ARGB(255,255,0,0));*/
						}
						break;

					case SKILL_USE_FAILED_CANNOTFACTURE_STATE:		//��ǰ״̬�������״̬����������
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(205,19),
								D3DCOLOR_ARGB(255,255,0,0));*/
						}
						break;

					case SKILL_USE_FAILED_INVALID_EQUIP:		//##��Ч��װ��
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_ARM),
							D3DCOLOR_ARGB(255,255,0,0));*/
						// ȡ��������������
						GetGame()->GetGameControl()->SetAutoSkillID(0);
						break;

					case SKILL_USE_FAILED_BLOCKED:				//##���赲��
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_BLOCK),
							D3DCOLOR_ARGB(255,255,0,0));*/
						// ȡ��������������
						GetGame()->GetGameControl()->SetAutoSkillID(0);
						break;
					case SKILL_USE_FAILED_BREAK:
						{
							if (lSkillID>=50000&&lSkillID<=50007)
							{
								/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
									CStringReading::LoadText(205,40),
									D3DCOLOR_ARGB(255,255,0,0));*/
							}
							else
							{
								/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
									CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_DELSKILL),
									D3DCOLOR_ARGB(255,255,0,0));*/
							}
							// ȡ��������������
							if(lSkillID != GetGame()->GetGameControl()->GetAutoSkillID())
							{
								GetGame()->GetGameControl()->SetAutoSkillID(0);
								GetGame()->GetMainPlayer()->SetAction(CShape::ACT_STAND);
							}
						}
						break;
					case SKILL_USE_FAILED_NOOBJECT:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_NOOBJ),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_INTONATE:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_INTTIME),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_INVALID_OCCUPATION:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_OUCC),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_INVALID_FACTURE:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MADE),
							D3DCOLOR_ARGB(255,255,0,0));
						GetGame()->GetCGuiEx()->GetFacturePageEx()->Close();*/
						break;
					case SKILL_USE_FAILED_INVALID_COLLECT:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_COLLECTION),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_INVALID_ACT:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_ACTING),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_INVALID_MOVING:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_USE_UNSUCCESS_MOVEING),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;

					case SKILL_USE_FAILED_C_NOCOLLECTION:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_NOOBJ),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_STATE_ERROR:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_STATE),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_UNKNOW_COLLECTION:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_NOINFO),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_DOCCU_ERROR:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_OCCUEX),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_LEVEL_ERROR:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_LEV),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_GOODSCONDITION_ERROR:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_GOODS),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_NO_BAGSPACE:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_PACK),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_NOT_ROLECOLLECTION:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_NOTQUESTOBJ),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_QUESTSTEP_ERROR:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_SPECQUEST),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_C_UNKOWN_COLLECTTYPE:
						/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_CONDITION_TYPE),
							D3DCOLOR_ARGB(255,255,0,0));*/
						break;
					case SKILL_USE_FAILED_F_NO_FACTURE:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_NOOBJ),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_DOCCU_ERROR:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_OCCUEX),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_NOTHAS_FACTURE:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_NOSKILL),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_GOODSCONDITION_ERROR:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_TOOL),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_MATERIALS_ERROR:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_GOODS),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_NPC_ERROR:
						{
							/*GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_NPCDEST),
								D3DCOLOR_ARGB(255,255,0,0));*/
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
					case SKILL_USE_FAILED_F_NO_BAGSPACE:
						{
// 							GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 								CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_MADE_PACK),
// 								D3DCOLOR_ARGB(255,255,0,0));
							GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-12.wav");
						}
						break;
						// ��������
					case SKILL_USE_FAILED_INVALID_REGION:
// 						GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 							CStringReading::LoadText(eIDS_SKILL,eIDS_ERROR_BYREGION),
// 							D3DCOLOR_ARGB(255,255,0,0));
						break;
					default: break;
					}
				}
				break;
			}
		}
		break;

	case MSG_S2C_SKILL_ADD_EX_STATE:	// ����һ���쳣״̬
		{
			// ״̬�����ŵ����ͺ�GUID
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			// ״̬ID
			DWORD dwStateID = pMsg->GetDWord();
			DWORD dwLevel = pMsg->GetDWord();			// ״̬�ȼ�
			DWORD dwRemainedTime = pMsg->GetDWord();
			BYTE bFlag = pMsg->GetByte();

			CMoveShape *pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lType, ID);
			if (pMoveShape)
			{
				pMoveShape->AddStateEx(dwStateID, dwLevel, dwRemainedTime,0,bFlag);
			}
		}
		break;

	case MSG_S2C_SKILL_END_EX_STATE:	// ����һ���쳣״̬
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			DWORD dwStateID = pMsg->GetDWord();
			DWORD dwlev = pMsg->GetDWord();
			CMoveShape *pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lType, ID);
			if (pMoveShape)
			{
				pMoveShape->RemoveStateEx(dwStateID,dwlev);
			}
		}
		break;

	case MSG_S2C_SKILL_EX_STATE_ADDITIONAL_DATA:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			long lStateID = pMsg->GetLong();
			DWORD dwValue = pMsg->GetDWord();
			if( lStateID == STATE_TEAM )
			{
				CPlayer *pPlayer = dynamic_cast<CPlayer*>(pMsg->GetRegion()->FindChildObject(lType, ID));
				if (pPlayer)
					pPlayer->SetTeammateAmount(dwValue&0xffff);
			}
		}
		break;
		// �򿪼���ѧϰҳ��
	case MSG_S2C_SKILL_STUDY_BEGIN:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			long num = pMsg->GetLong();
			if(num > 0)
			{
				//GetGame()->GetCGuiEx()->GetLearnSkillPage()->Close();
				for(long i=0; i<num; ++i)
				{
					BYTE bType = pMsg->GetByte();
					DWORD dwSkillID = pMsg->GetDWord();
					//if(dwSkillID > 0)
					//	GetGame()->GetCGuiEx()->GetLearnSkillPage()->PushSkillID(dwSkillID,bType);
				}
				//GetGame()->GetCGuiEx()->GetLearnSkillPage()->Open();
				//GetGame()->GetCGuiEx()->GetLearnSkillPage()->SetNpcGUID(guid);
				//GetGame()->GetCGuiEx()->GetLearnSkillPage()->OpenPage(CLearnSkillPage::TYPE_FIGHTSKILLPAGE);
				GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-20.wav");
			}
			// ְҵ��������������Ի���
			else
			{

			}
		}
		break;
		// ����������ѧϰҳ��
	case MSG_S2C_OCCUSKILL_STUDY_BEGIN:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->Close();
			long num = pMsg->GetDWord();
			for(WORD i=0; i<num; ++i)
			{
				DWORD dwSkillID = pMsg->GetDWord();
				DWORD dwSkillCost = pMsg->GetDWord();
				//if(dwSkillID > 0)
				//	GetGame()->GetCGuiEx()->GetLearnSkillPage()->AddOccuSkill(dwSkillID,dwSkillCost);
			}
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->SetNpcGUID(guid);
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->OpenPage(CLearnSkillPage::TYPE_ASSITSKILLPAGE);
		}
        break;
	case MSG_S2C_OCCUSKILL_STUDY:
		{
			long lBeSucceed = pMsg->GetLong();
			DWORD dwFactureID = pMsg->GetDWord();
			if (lBeSucceed!=0)
			{
				//GetGame()->GetCGuiEx()->GetLearnSkillPage()->ChoseNextSkill(dwFactureID);
			}
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->StudyOccuSkillResult(lBeSucceed,dwFactureID);
			//long lOccType = GetGame()->GetCGuiEx()->GetMainBarPageEx()->GetOuccType();
			//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetOccuButtonSpecialState(true,lOccType);
			//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-27.wav");
		}
        break;
	case MSG_S2C_OCCUSKILL_DELETE:
		{
			GetGame()->GetMainPlayer()->DeleteAllFactureSkill();
			//DWORD dwBeSucceed = pMsg->GetDWord();
			//DWORD dwFactureID = pMsg->GetDWord();
			//GetGame()->GetCGuiEx()->GetLearnSkillPage()->StudyOccuSkillResult(dwBeSucceed,dwFactureID);
		}break;
		// ����ѧϰʧ��
	case MSG_S2C_SKILL_STUDY_FAILED:
		{
			long lFaildRet = pMsg->GetLong();
			switch (lFaildRet)
			{
				//ְҵ������ѧϰ����
			case SKILL_STUDY_FAILED_OCCUPATION:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_OUCC),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//�ȼ�������ѧϰ����
			case SKILL_STUDY_FAILED_RANK:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_LEV),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//SP������ѧϰ����
			case SKILL_STUDY_FAILED_SP:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_SP),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//ǰ�ü��ܲ�����ѧϰ����
			case SKILL_STUDY_FAILED_SKILL:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_PRESKILL),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//ǰ�����Ƽ��ܲ���������
			case SKILL_STUDY_FAILED_LIMIT_SKILL:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_LIMITSKILL),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//��Ʒ
			case SKILL_STUDY_FAILED_GOOD:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_GOODS),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//��Ҳ���
			case SKILL_STUDY_FAILED_GOLD:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_NOGOLDS),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
				//���Ҳ���
			case SKILL_STUDY_FAILED_SILVER:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_NOSILVER),
// 					D3DCOLOR_ARGB(255,255,0,0));
				//ְҵ�ȼ�����������
			case SKILL_STUDY_FAILED_OCCULV:
// 				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,
// 					CStringReading::LoadText(eIDS_SKILL,eIDS_STUDY_FAILED_OCCULV),
// 					D3DCOLOR_ARGB(255,255,0,0));
				break;
			}
		}
		break;
		// ������Ӽ���
	case MSG_S2C_SKILL_ADD_PET:
		{
			//�õ�����GUID
			CGUID	guid;
			pMsg->GetGUID(guid);

			tagPetSkillInfo skill;
			pMsg->GetEx(&skill, sizeof(tagSkill));
			
			//������ȴ��ʱ�䳤��
			skill.lRestorTimeLength = pMsg->GetLong();

			//���ܵ�ǰ��ȴʱ��
			skill.lStartCoolTime = 0;

            // ���ܷ�Χ��Ϊfloat TODO:�޸Ŀͻ��˼��ܷ�Χ����߼�
			//��С��������
			skill.lMinAtkDistance = (long)pMsg->GetFloat();
			
			//��󹥻�����
			skill.lMaxAtkDistance = (long)pMsg->GetFloat();
			
			//��Ҫ����MPֵ
			skill.lCostMp = pMsg->GetLong();
			
			CPet* pPet = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,guid)) ;
			if (pPet == NULL)
				return;
			if (skill.lPos == 0)
			{
				skill.bAuto = true;	//����ǵ�һ������ ����Զ��ͷ�
			}
			pPet->AddSkill(skill);

			//CPetPageEx *pPage = GetGame()->GetCGuiEx()->GetPetPageEx();
			
			//if (pPage)
			//	pPage->LearnedNewSkill(pPet,&skill);
		}
		break;
		/***********************************************************************/
		/*  zhaohang 2010-10-18 //��������*/
		/***********************************************************************/
	//case MSG_S2C_SHAPE_ATK_IMMUNITY:
	//	{

	//	}
	//	break;
	}
}
