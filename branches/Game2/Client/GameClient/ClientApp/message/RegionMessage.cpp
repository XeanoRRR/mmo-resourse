#include "stdafx.h"
#include "Message.h"
#include "clientRegion.h"
#include "player.h"
#include "Monster.h"
#include "../Goods/Goods.h"
#include "Build.h"
#include "Effect.h"
#include "CollectionGoods.h"
#include "Citygate.h"
#include "../../EngineExtend/WeatherManager.h"
#include "Skills/SkillXml.h"
#include "Skills/SkillAI.h"
#include "other/RegionList.h"
#include "PetSystem/Pet.h"
//#include "PetSystem/CFairyPet.h"
//#include "PetSystem/CFighterPet.h"
//#include "PetSystem/CSummonPet.h"
//#include "PetSystem/CTrap.h"
//#include "PetSystem/OrnaPet.h"
#include "../Other/FaceActionList.h"
#include "../GameClient/GameControl.h"
#include "Other/AudioList.h"
#include "MainPlayerHand.h"
#include "../GameClient/Game.h"
#include "Other/GlobeSetup.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/*
* ����: ��Ӧ������Ϣ
* ժҪ: -
* ����: pMsg - ���������͵���Ϣ
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.10.18 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ�������˶Լ��볡���Ķ�������Ƿ�Alpha����Ĵ���;
*	2008.10.27 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ�������˲ɼ�������趨��ʾģ��Ϊͬ����ʾ(������һ�ν���Ϸ,���ڲɼ��������趨�ȼ�,��������޷���ʾģ������);
*	2008.12.10 - lpf
*		��MSG_S2C_RGN_CHANGEWEATHER��Ϣ��,������������ʱ��Billboard Y�����ݴ���;
*	2009.02.20 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ��,"�Ƿ��Ѿ����ڸ�SHAPE"���봦��,������MoveShape����ʱ,Ѱ����������Ч11114,��ͬʱ������������,��������ˢ�º�,��Ч�Ͳɼ���ģ�Ϳ��ܲ���ͬһ��������;
*	2009.03.03 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ��,�޸���������Ʒģ�͵ȼ��趨,�����˵�һ�ν��볡��ʱ,�޷��������ڵ��ϵ���Ʒģ�͵ĵȼ�Ч��;
*	2009.03.03 - lpf
*		��MSG_S2C_RGN_CHANGE��Ϣ��,�����˳����Ƿ���Ըı�װ���������趨;
*	2009.03.20 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ��,������"����Ҵ��ڷ���״̬����Ҫ�������¶���"�Ĵ���;
*	2009.04.03 - lpf
*		��MSG_S2C_RGN_ADDSHAPE��Ϣ��,������"����Ҵ��ڷ���״̬����Ҫ�������¶���"�Ĵ���,��Ϊ��״̬��,����Ѿ����ڰ�̯״̬,����ֱ�Ӳ������ڵ��ϵĶ���,����ͨ����̬�������������µĹ���;
*	2009.11.13 - lpf
*		��MSG_S2C_RGN_CHANGE��Ϣ��,�����˶Ե�ǰ�����Ƿ�ΪPK������־�Ķ�ȡ,���Գ����������˶�Ӧ���趨;
*/
void CClientRegion::OnMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2C_RGN_ENTER:
		{			
		}
		break;

	case MSG_S2C_RGN_ADDSHAPE:
		{
			long lType = pMsg->GetLong();
			CGUID ID,guID;
			pMsg->GetGUID(ID);
#ifdef _RUNSTACKINFO_
			char str[1024] = "";
			_snprintf(str,1024,"Add Shape Object(Type:%d) Start",lType);
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),str);
#endif

			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);			
            CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			CBaseObject* pObj = 0;
			// �Ƿ��Ѿ����ڸ�SHAPE
			pObj = FindChildObject(lType, ID);
			if(pObj)
			{
				/////////////////////////////////////////////////////////////start
				//test add or delete shape,(author:wangqiao)
				char pszTestInfo[1024]="";
				char pszGUID[64]="";
				pObj->GetExID().tostring(pszGUID);
				//_snprintf(pszTestInfo,1024,"when Add the Shape,it exist(type:%d,id:%s,name:%s)(mx:%d,my:%d)",
				//	pObj->GetType(),pszGUID,pObj->GetName(),
				//	GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//PutStringToFile("AddDelShapeTest",pszTestInfo);
				////////////////////////////////////////////////////////////end
				bool isSelf = false;
				switch(lType)
				{
				case TYPE_PET:
					{
						pMsg->GetLong();				//���½��ճ�������
						isSelf = (pMsg->GetChar()>0);	//���½����Ƿ����Լ���
					}
					break;
				}

				pObj->DecodeFromDataBlock(setReadDB, isSelf);

				CShape* pShape = dynamic_cast<CShape*>(pObj);
				if(pShape)
				{
					pShape->SetIsDeleted(false);
					CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pShape);
					if(pMoveShape)
					{
						pMoveShape->SetDestXY(pMoveShape->GetPosX(),pMoveShape->GetPosY());
						CEffect * pEffect = pMoveShape->FindEffect(11114);
						if (pEffect != NULL)
							pEffect->SetPosXY(pMoveShape->GetPosX(), pMoveShape->GetPosY());
						if (pShape->GetType() == TYPE_PET)	//���¼����Լ��ĳ���
						{
							CPet *pPet = dynamic_cast<CPet*>(pShape);
							if (isSelf)
							{
								pMainPlayer->SetPetFighting(pPet);
								pShape->GetAnimInfo()->GetActionInfo()->dwCurAction = MAKEFOURCC('B','O','R','N');
								pShape->GetAnimInfo()->GetActionInfo()->dwCurActionLoopCount = 0;
								pShape->GetAnimInfo()->GetActionInfo()->dwCurActionStartTime = GetCurTickCount();
								pShape->GetAnimInfo()->GetActionInfo()->bCurActionLooped = false;
								pPet->SetInBornState(true);
								pPet->EnterRegion(isSelf);
							}
						}
					}
				}
				break;
			}

			// �¼���
			CShape* pShape = NULL;
			char bSelf = 0;
			switch(lType)
			{
			case TYPE_PLAYER:
				pShape = new CPlayer;

				if (CGlobeSetup::GetSetup()->dwPlayerAlphaFadeTimeLength != 0)
				{
					pShape->SetIsNeedAlphaFade(true);
					pShape->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwPlayerAlphaFadeTimeLength);
					pShape->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
				}
				break;

			case TYPE_MONSTER:
				{
					pShape = new CMonster;

					if (CGlobeSetup::GetSetup()->dwMonsterAlphaFadeTimeLength != 0)
					{
						pShape->SetIsNeedAlphaFade(true);
						pShape->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwMonsterAlphaFadeTimeLength);
						pShape->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
					}
				}
				break;

			case TYPE_GOODS:
				pShape = new CGoods;
				bSelf = 0;
				pShape->SetExID(ID);
				if (CGlobeSetup::GetSetup()->dwGoodsAlphaFadeTimeLength != 0)
				{
					pShape->SetIsNeedAlphaFade(true);
					pShape->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwGoodsAlphaFadeTimeLength);
					pShape->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
				}
				break;

			case TYPE_BUILD:
				pShape = new CBuild;
				break;
			case TYPE_COLLECTION:
				{
					pShape = new CCollectionGoods;	

					if (CGlobeSetup::GetSetup()->dwCollectionAlphaFadeTimeLength != 0)
					{
						pShape->SetIsNeedAlphaFade(true);
						pShape->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwCollectionAlphaFadeTimeLength);
						pShape->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
					}
				}break;

			case TYPE_CITYGATE:
				{
					pShape = new CCityGate;

					if (CGlobeSetup::GetSetup()->dwCollectionAlphaFadeTimeLength != 0)
					{
						pShape->SetIsNeedAlphaFade(true);
						pShape->SetAlphaFadeInTimeLength(CGlobeSetup::GetSetup()->dwMonsterAlphaFadeTimeLength);
						pShape->SetAlphaFadeMode(CAlphaFadeObjectList::ALPHA_FADE_IN);
					}
				}break;
			case TYPE_PET:    //AddBy: nikai  
				{
					long lType = setReadDB.GetLongFromByteArray();
					bSelf = setReadDB.GetCharFromByteArray();		//�Ƿ����Լ���
					switch (lType)
					{
					case PET_TYPE_SERVANT:
						if (bSelf)
						{
							pShape = pMainPlayer->GetPet(ID);
                            pMainPlayer->SetPetFighting((CPet*)pShape);
						}
						else
							pShape = new CPet;
						break;
					default:
						break;
					}
				}
				break;
			}

			if( pShape )
			{
				pShape->SetAppearTime( GetGame()->GetAITick() );
				if(bSelf>0)
					pShape->DecodeFromDataBlock( setReadDB, true);
				else
					pShape->DecodeFromDataBlock( setReadDB, false);
				CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
				if (lType == TYPE_PET)   //AddBy:nikai
				{
					CPet *pPet = dynamic_cast<CPet*>(pShape);
					pShape->GetAnimInfo()->GetActionInfo()->dwCurAction = MAKEFOURCC('B','O','R','N');
					pShape->GetAnimInfo()->GetActionInfo()->bCurActionLooped = false;
					pPet->EnterRegion(bSelf>0);
				}

				/////////////////////////////////////////////////////////////start
				//test add or delete shape,(author:wangqiao)
				char pszTestInfo[1024]="";
				char pszGUID[64]="";
				pShape->GetExID().tostring(pszGUID);
				//_snprintf(pszTestInfo,1024,"Add the Shape Message(type:%d,id:%s,name:%s,x:%d,y:%d)(mx:%d,my:%d)",
				//	pShape->GetType(),pszGUID,pShape->GetName(),pShape->GetTileX(),pShape->GetTileY(),
				//	GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//PutStringToFile("AddDelShapeTest",pszTestInfo);
				////////////////////////////////////////////////////////////end


				assert( pShape->GetPosX() >=0 && pShape->GetPosX() <=GetWidth() &&
					pShape->GetPosY() >=0 && pShape->GetPosY() <= GetHeight() );

				//pShape->SetDisplayModel();
				//pShape->SetDisplayModelGroup();

				AddObject( pShape );

				//--------------07.10.16����----------
				if(pShape->GetType()==TYPE_MONSTER)
				{
					CMonster* mShape = dynamic_cast<CMonster*>(pShape);
					if(mShape)
					{
						// �ռ���NPC���������ʾ��Ϣ
						if(mShape->GeteNpcType() == NT_Normal)
						{
							pMainPlayer->CollectionQuestClewInfo(mShape);
						}
						mShape->SetBlock(mShape->GetTileX(), mShape->GetTileY(), mShape->GetBlockType());
					}
				}
				//------------------------------------

				// ����ǲɼ���͸������Ƿ�����ҿɲɵĲɼ��������Ч
				if (pShape->GetType()==TYPE_COLLECTION)
				{
					CQuestQueue* pQueueList = pMainPlayer->GetRoleQueue();
					CCollectionGoods* pCollectGoods = dynamic_cast<CCollectionGoods*>(pShape);
					if (pMainPlayer!=NULL)
					{
						if (pCollectGoods->GetOccuID()==pMainPlayer->GetAssOccu())
						{
							pShape->AddEffect(11114,pShape->GetPosX(),pShape->GetPosY(),pShape->GetHeight(),true);
						}
						else if (pCollectGoods->GetOccuID()==4&&pQueueList!=NULL)
						{
							const vector<tagQuestGoods>* pQustList = pCollectGoods->GetRoleInfo();
							if (pQustList!=NULL)
							{
								vector<tagQuestGoods>::const_iterator QustIter = pQustList->begin();
								for (;QustIter!=pQustList->end();QustIter++)
								{
									if (pQueueList->GetRecordByID(QustIter->nQuestId)!=NULL)
									{
										pShape->AddEffect(11114,pShape->GetPosX(),pShape->GetPosY(),pShape->GetHeight(),true);
										break;
									}
								}
							}
						}
					}
				}

				if(pShape->GetType()==TYPE_MONSTER||pShape->GetType()==TYPE_PLAYER)
				{
					CMoveShape* mShape = dynamic_cast<CMoveShape*>(pShape);
					mShape->UseCurSkill();		// �����ʩ�ŵ�ǰ����
				}
				if(pShape->GetType()==TYPE_GOODS)
				{
					CGoods* pTempGoods=dynamic_cast<CGoods*>(pShape);

					// ��Ǯ��GraphicsID
					//pShape->m_cAnimInfo.GetActionInfo()->dwCurAction = MAKEFOURCC('g','r','n','d');
					if(pTempGoods->GetProperty() && pTempGoods->GetProperty()->dwType==GBT_MONEY)
					{						
						long lGraphicsID = CGoodsList::GetMoneyGraphics(pTempGoods->GetNum());
						pTempGoods->SetGraphicsID(lGraphicsID);
					}

					//��ʼ��������Ч
					pTempGoods->IniFlashEffect();
				}

				//����״̬��������ִ�Խʱ�յ����
				if( pShape->GetAction() == CShape::ACT_WALK || pShape->GetAction() == CShape::ACT_RUN )
				{
					pShape->SetAction(CShape::ACT_STAND);
				}

				if (pShape->GetType() == TYPE_PLAYER || pShape->GetType() == TYPE_MONSTER)
				{
					extern bool g_bForceUpdate;
					g_bForceUpdate = true;
					CPlayer *pPlayer=(CPlayer*)pShape;
					if(pPlayer->IsInDieAwayState())	// ������������ͼ���״̬		
					{
						pShape->SetAction(CShape::ACT_ALREADYDIED);
					}
					else if(pShape->GetType()!=TYPE_PLAYER||!pPlayer->IsEmotionShowing())
					{	
						pShape->SetAction(pShape->GetAction());
					}
				}

				if( pShape->GetType() == TYPE_PLAYER )
				{
					if(pMainPlayer->IsTeammate(pShape->GetExID()))
					{
						//GetGame()->GetCGuiEx()->GetTeamPageEx()->Set3DIconShape(pShape->GetExID(),pShape);
					}
					// �¼ӵ�PLAYER�ѱ�ѡ��
					//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
					//if(pObjPage && pShape->GetExID() == pObjPage->GetObjGUID())
					//{
					//	GetGame()->GetGameControl()->SetSelectedShape(pShape);
					//}
					CPlayer* pPlayer = (CPlayer*)pShape;

					//�ж��Ƿ��������ս��,��һ��һ��Ч
					if(pPlayer->GetIsContendState() == true)
					{
						pPlayer->AddEffect(2000000,pPlayer->GetPosX(),pPlayer->GetPosY(),pPlayer->GetHeight(),true);
					}
					if(pPlayer->GetCityWarDiedState() == true)
					{
						pPlayer->AddEffect(2000001,pPlayer->GetPosX(),pPlayer->GetPosY(),pPlayer->GetHeight(),true);
					}

					pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WEAPON);
					pPlayer->UpdateWeaponShow(CPlayer::EQUIP_WING);
					pPlayer->UpdateSpecialEquipment();					
					pPlayer->UpdateRideShow();

					//��鳡���ϳ������Ӹ����Ե�����
					itBaseObject it;
					for (it = m_listObject.begin(); it != m_listObject.end(); ++it)
					{
						CBaseObject* pObject = (*it);
						if (pObject->GetType() == TYPE_PET)
						{
							CPet *pPet = (CPet*)pObject;
							if (pPet->GetMasterGUID() == pPlayer->GetExID())
							{
								pPet->EnterRegion(false);
							}
						}
					}

					// ����Ҵ��ڷ���״̬����Ҫ�������¶���
					if (pPlayer->IsPlayerShopOpened())
					{
						pPlayer->SetAction(801);
						pPlayer->GetAnimInfo()->GetActionInfo()->bInterpolation = FALSE;
						pPlayer->GetAnimInfo()->m_tActionInfo.bCurActionLooped  = TRUE;
					}
				}

				if(pShape->GetType() == TYPE_CITYGATE)
				{
					if(pShape->GetState()==CShape::STATE_PEACE||pShape->GetAction()==CShape::ACT_OPEN)
					{
						pShape->SetAction(CShape::ACT_ALREADYOPEN);
					}else if(pShape->GetState()==CShape::STATE_FIGHT||pShape->GetAction()==CShape::ACT_IDLE)
					{
						pShape->SetAction(CShape::ACT_STAND);
					}
				}

				pShape->OnActionChange();

				if (pShape->GetType() == TYPE_COLLECTION)
				{
					pShape->SetDisplayModel(false);
					pShape->SetDisplayModelGroup();
				}else if (pShape->GetType() == TYPE_GOODS)
				{
					//add by ��ǫ:����������Ʒ��ʱ����ʾ�ȼ���Ϣ,����ṹ���Ǻܺ�
					//			  ������ʱֻ��������,��������Ʒ�����������ʱ��
					//            �ܰ�ͬ������Ʒ(����GOODS)�Ĳ���ͳһ��һ��...
					CGoods* pTempGoods=dynamic_cast<CGoods*>(pShape);
					pTempGoods->SetDisplayModel(false);
					if (pTempGoods->GetDisplayModel())
						pTempGoods->GetDisplayModel()->ShowAllGroupbyLevel(pTempGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1), false);
				}else 
				{
					pShape->SetDisplayModel(); 
					pShape->SetDisplayModelGroup();
				}
			}
			char bGMCreate = pMsg->GetChar();	// �Ƿ���GM�����

#ifdef _RUNSTACKINFO_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Add Shape Object End");
#endif
		}
		break;

	case MSG_S2C_RGN_DELSHAPE:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);

			//��������ƶ���������Ч(�ƶ�ǰ������)
			long lUseGoods = pMsg->GetLong();
			if (lUseGoods == 1 && lType == TYPE_PLAYER && ID != GetGame()->GetMainPlayer()->GetExID() ) 
			{
#pragma message("-------- ����ӿ�������ƶ���������Ч --------")
			}

			// ��Ϊ�������ڰ�̯���������ʱ������Χ��ҷ�����Ϣ������ֻ�ܿͻ����������Լ��ж��²��رմ򿪵��̵�ҳ��
			//CPlayerShopPageEx* pPlayerShopPage = GetGame()->GetCGuiEx()->GetPlayerShopPageEx();
			//if (pPlayerShopPage!=NULL&&pPlayerShopPage->IsOpen()
			//	&&lType==TYPE_PLAYER&&ID==pPlayerShopPage->GetMasterID())
			//{
			//	pPlayerShopPage->Close();
			//}

			CShape* pShape = (CShape*)FindChildObject(lType, ID);
			if( pShape)
			{
				/////////////////////////////////////////////////////////////start
				//test add or delete shape,(author:wangqiao)
				char pszTestInfo[1024]="";
				char pszGUID[64]="";
				pShape->GetExID().tostring(pszGUID);
				//_snprintf(pszTestInfo,1024,"Delete the Shape Message(type:%d,id:%s,name:%s,x:%d,y:%d)(mx:%d,my:%d)",
				///	pShape->GetType(),pszGUID,pShape->GetName(),pShape->GetTileX(),pShape->GetTileY(),
				//	GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//PutStringToFile("AddDelShapeTest",pszTestInfo);
				////////////////////////////////////////////////////////////end
				//CLookPageEx *pPage = GetGame()->GetCGuiEx()->GetLookPageEx();
				//if(pShape==pPage->GetSelectPlayer()&&pPage->IsOpen()) //��������ڲ鿴����
				//{
				//pPage->Close();  //�رս��� 
				//}
				// ɾ���赲
				if(lType == TYPE_MONSTER)
					pShape->SetBlock(pShape->GetTileX(),pShape-> GetTileY(), CRegion::BLOCK_NO);

				// ����ҵĳ�������б���ɾ��
				if (pShape->GetType() == TYPE_PET && ((CPet*)pShape)->IsMainPlayerPet())
				{
					RemoveObject(pShape);
				}
				else if( pShape != GetGame()->GetMainPlayer())
				{
					pShape->SetIsDeleted(true);
				}

				// ɾ�����ָ�����
				if( pShape==GetGame()->GetGameControl()->GetMouseShape() )
					GetGame()->GetGameControl()->SetMouseShape( NULL );
				// ɾ����ǰ�������Ĺ���Ŀ��
				if( pShape==GetGame()->GetGameControl()->GetCurAttractShape() )
					GetGame()->GetGameControl()->SetCurAttractShape( NULL );
				// ���Ҫɾ���Ķ����ǵ�ǰѡ���
                if ( pShape == GetGame()->GetGameControl()->GetSelectedShape() )
                {
                    FireUIEvent("ObjectPage", "Closed");
                }
				//ɾ������
				if(lType == TYPE_PET)
				{
					CPet	*pPet = (CPet*)pShape;
					CPlayer *pMaster = pPet->GetMaster();
					if (pMaster)
					{
						//ɾ��ǰ�ȱ��漼������
						pMaster->SavePetSkillSetting();
						//ִ�г���ɾ������
						//pMaster->RemovePet(pPet->GetExID());
						//Ч�鵱ǰ������ؽ���򿪻�ر�
						pMaster->ValidatePetInterface();
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
				//_snprintf(pszTestInfo,1024,"When delete the Shape Message,not exist.(type:%d,id:%s)(mx:%d,my:%d)",
				//	lType,pszGUID,
				//	GetGame()->GetMainPlayer()->GetTileX(),GetGame()->GetMainPlayer()->GetTileY());
				//PutStringToFile("AddDelShapeTest",pszTestInfo);
				////////////////////////////////////////////////////////////end
			}
		}
		break;

	case MSG_S2C_RGN_CHANGE:
		{
			long lType		= pMsg->GetLong();
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			long lUseGoods = pMsg->GetLong();

			//�������뿪�Ķ���
			if (lUseGoods) 
			{
#pragma message("-------- ���������뿪�Ķ��� --------")
			}

			CPlayer * pPlayer = GetGame()->GetMainPlayer();
			if(!pPlayer)
				return;
			// ����Լ�
			if( pPlayer->GetExID() == PlayerID )
			{
				CGUID RegionID;
				pMsg->GetGUID(RegionID);
				long lRegionID = pMsg->GetLong();
				long lPlayerX	= pMsg->GetLong();
				long lPlayerY	= pMsg->GetLong();
				long lPlayerDir	= pMsg->GetLong();
				char strRegionName[ 256 ] = "";
				pMsg->GetStr( strRegionName ,256);
				long lTaxRate = pMsg->GetLong();
				eRWT eWarRegionType = (eRWT)pMsg->GetByte();//�õ�ս����������
				eRST eSpaceType=(eRST)pMsg->GetByte();//�Ƿ���ˮ�³���
				bool bChangeEquip = pMsg->GetByte()==0?false:true;
				long lResourceID = pMsg->GetLong();
				float fExpScale = pMsg->GetFloat();
				eRPKT ePKRegionType = (eRPKT)pMsg->GetByte();

				// ������У����¶�����Ϣ
				//if(pPlayer->GetTeamID() != CGUID::GUID_INVALID)
				//	GetGame()->GetCGuiEx()->GetTeamPageEx()->Set3DIconShape();
				//GetGame()->GetCGuiEx()->GetCObjectPageEx()->Close();
                FireUIEvent("ObjectPage", "Closed");
				GetGame()->GetGameControl()->SetMouseShape( NULL );
				GetGame()->GetGameControl()->SetSelectedShape( NULL );
				GetGame()->GetGameControl()->SetCurAttractShape( NULL );
				GetGame()->GetGameControl()->SetSendedMoveMsgCount(0);
				GetGame()->GetGameControl()->SetSendedUseSkillMsgCount(0);
				//GetGame()->GetCGuiEx()->GetWorldMapPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetMailPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetReadMailPageEx()->Close();
				//CMessageBoxPageEx* pMessPage = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
				//if (pMessPage!=NULL&&pMessPage->IsOpen())
				//{
				//	pMessPage->Clear();
				//	pMessPage->Close();
				//}
				// �����ұ����������ӱ������Ǵ򿪵ģ���ʱҪ������ȫ���ر�
				// ��Ϊ������л�������ʱ���������Ὣ������ϵ���Ʒ���±��뵽�ͻ��ˣ���ʱ��������Ǵ򿪵ģ�
				// �ͻ�ʹ�����д�ŵ���Ʒ��ַ���ڴ��еĵ�ַ���������Ҫ�رձ������������ٴδ�ʱ�ͻ�ˢ��������Ʒ�ĵ�ַ��
				CMainPlayerHand* pMainPlayerHand = 0;//GetGame()->GetCGuiEx()->GetPMainPlayerHand();
				if (pMainPlayerHand!=NULL)
				{
					if (pMainPlayerHand->GetPGoodsOfMainPlayerHand()!=NULL)
					{
						pMainPlayerHand->GetPGoodsOfMainPlayerHand()->SetHaveShadowState(false);
					}
					pMainPlayerHand->ClearMainPlayerHand();
				}

				//CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
				//if (pItemPage!=NULL)
				//{
				//for (int i = 0;i<5;i++)
				//{
				//		pItemPage->ClearPackData(i);
				//		pItemPage->CloseSubPack(i);
				//	}
				//	pItemPage->Close();
				//}

				//GetGame()->GetCGuiEx()->GetPassWordBoxPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetTextBoxPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->Close();
				//CNPCShopPageEx* pNPCShop = GetGame()->GetCGuiEx()->GetNPCShopPageEx();
				//if (pNPCShop!=NULL&&pNPCShop->IsOpen())
				//{
				//	pNPCShop->Close();
				//}
				//CCardMutifunPageEx* pMutiCardPage = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
				//if (pMutiCardPage!=NULL&&pMutiCardPage->IsOpen())
				//{
				//	pMutiCardPage->Close();
				//}

				assert( lPlayerX >=0 &&	lPlayerY >=0 );

				//////////////////////////��÷�����Ϣ////////////////////////////////////
				//long lDataSize = pMsg->GetLong();
				//if(lDataSize==0)
				//{
				//	LPBYTE	pData = new BYTE[ lDataSize ];
				//	pMsg->GetEx(pData,lDataSize);
				//	long lPos = 0;
				//	GetGame()->GetCGuiEx()->GetSelectLinePageEx()->DecordFromByteArray(pData,lPos);
				//	 //�ͷ���Ϣ���ݿ�
				//	SAFE_DELETE_ARRAY( pData );
				//}
				DBReadSet setReadDB;
				pMsg->GetDBReadSet(setReadDB);
				//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->SetCurRegionID(lRegionID);
				//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->DecodeFromDataBlock(setReadDB);
				//���ݷ������ý����ϵķ��߰�ť
				//GetGame()->GetCGuiEx()->GetSelectLinePageEx()->SetSelectBotton();


				// ���壺�ر�֮ǰ�򿪵Ľ���
				//GetGame()->GetCGuiEx()->GetComposePageEx()->Close();
				//GetGame()->GetCGuiEx()->GetDecomposePageEx()->Close();
				//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->Close();
				//GetGame()->GetCGuiEx()->GetEquipMagicPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->Close();
				//GetGame()->GetCGuiEx()->GetDepotPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetFriendsPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetFactionPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetFactionListPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetUnionPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetTopMsgPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetLookPageEx()->Close();
				//GetGame()->GetCGuiEx()->GetMadalPageEx()->Close();

				//�رչ��������Ч
				GetGame()->GetAudioList()->SetOppearSoundEnble(false);
				//}

				////�����NPC���׿�δ�رգ���ر�
				//if(GetGameClient()->GetInterface()->
				//	FindWindow(GetGameClient()->GetInterface()->GetNPCTradePage()))
				//{
				//	CMessage msg(MSG_OTHER_SENDTALKBOXSELECT);
				//	msg.Add( (long)-1 );	// -1 ��ʾ�˳� 0-100��ʾѡ���ѡ��
				//	msg.Send();

				//	GetGameClient()->GetInterface()->GetNPCTradePage()->Repeat();
				//	GetGameClient()->GetInterface()->PopWindow(GetGameClient()->GetInterface()->GetNPCTradePage());
				//}

				////�������ҽ��׿�δ�رգ���ر�
				//if(GetGameClient()->GetInterface()->
				//	FindWindow(GetGameClient()->GetInterface()->GetPlayerTradePage()))
				//{
				//	CMessage msg( MSG_PLAYER_QUESTSWAPITEM_CANCEL );
				//	msg.Add( GetGameClient()->GetMainPlayer()->GetID() );
				//	msg.Send();
				//}

				////�������ҽ��׿�δ�رգ���ر�
				//if(GetGameClient()->GetInterface()->
				//	FindWindow(GetGameClient()->GetInterface()->GetDepotPage()))
				//{
				//	CMessage msg( MSG_OTHER_SENDTALKBOXSELECT );
				//	msg.Add( (long)-1 );	// -1 ��ʾ�˳� 0-100��ʾѡ���ѡ��
				//	msg.Send();

				//	GetGameClient()->GetInterface()->GetNPCTradePage()->Repeat();
				//	GetGameClient()->GetInterface()->PopWindow(GetGameClient()->GetInterface()->GetNPCTradePage());
				//}

				GetGame()->QuestEnterRegion(lRegionID,RegionID,lResourceID,strRegionName, lPlayerX, lPlayerY, lPlayerDir,lUseGoods);
				CClientRegion * pRegion = GetGame()->GetRegion();
				if (pRegion)
				{
					pRegion->SetTaxRate(lTaxRate);
					pRegion->SetWarType(eWarRegionType);
					pRegion->SetExpScale(fExpScale);
					pRegion->SetSpaceType(eSpaceType);
					pRegion->SetChangeEquip(bChangeEquip);
					pRegion->SetPKRegionType(ePKRegionType);

					//�����ǳɹ����볡��
					pRegion->OnMainPlayerEnter();
					//GetGame()->GetCGuiEx()->OnPlayerMoving(1,1);
				}
				// **************************�����ľݵ������Ϣ��09.9.29���ƣ�*********************
				BYTE	bOpenFlag = pMsg->GetByte();
				long	lPointNum = pMsg->GetLong();
				char	szPointName[256] = "";
				if(pRegion)
				{
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetIsOpenSPoint(bOpenFlag>0);
					CClientRegion::tagSPointInfo stSPInfo;
					for ( long num = 0; num < lPointNum; ++num)
					{
						stSPInfo.strPointName = pMsg->GetStr(szPointName,256);
						stSPInfo.lPosX		= pMsg->GetLong();
						stSPInfo.lPosY		= pMsg->GetLong();
						stSPInfo.bState		= (BYTE)pMsg->GetLong();
						pRegion->AddSPointInfo(stSPInfo);
					}
				}
				// ********************************************************************************
			}
			else	// �������
			{
				CShape* pShape = (CShape*)FindChildObject( lType , PlayerID );
				if (pShape)
				{
					pShape->SetIsDeleted(true);
					//�жϸ�����Ƿ��ǵ�ǰ���ڲ鿴�Ķ���
					//CLookPageEx *pViwePage = GetGame()->GetCGuiEx()->GetLookPageEx();
					//if(pViwePage && pViwePage->GetSelectPlayer())
					//{
					//��õ�ǰ���ڲ鿴��playerID
					//	CGUID ViwePlayerID = pViwePage->GetSelectPlayer()->GetExID();
					//	if(PlayerID==ViwePlayerID)
					//GetGame()->GetCGuiEx()->GetLookPageEx()->Close();
					//}
				}
			}
			//// ��������
			//CMainBarPageEx *pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//if(pPage)
			//{
			//	pPage->UpdataCoordinate();
			//	pPage->UpdataPlaceName();
			//}
		}
		break;
		// �ı䵱ǰ����������
	case MSG_S2C_RGN_CHANGENAME:
		{
			long lRgnType = pMsg->GetLong();
			CGUID RgnID;
			pMsg->GetGUID(RgnID);
			long lRegionID = pMsg->GetLong();
			long lResourceID = pMsg->GetLong();
			char strRegionName[ 256 ] = "";
			pMsg->GetStr( strRegionName ,256);
			if(lRgnType == GetGame()->GetRegion()->GetRegionType()
				&& RgnID == GetGame()->GetRegion()->GetExID()
				&& lResourceID == GetGame()->GetRegion()->GetResourceID())
			{
				GetGame()->GetRegion()->SetName(strRegionName);
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdataPlaceName();
			}
		}
		break;
		// ���³����вɼ������Ч
	case MSG_S2C_RGN_UPDATEEFFECT:
		{
			CPlayer *pMainPlayer = GetGame()->GetMainPlayer();
			list<CBaseObject*>* pBaseObjectList = GetObjectList();
			list<CBaseObject*>::iterator ObjectIter = pBaseObjectList->begin();
			for (;ObjectIter!=pBaseObjectList->end();ObjectIter++)
			{
				if ((*ObjectIter)->GetType()==TYPE_COLLECTION)
				{
					CPlayer* pMainPlayer = GetGame()->GetMainPlayer();
					CQuestQueue* pQueueList = pMainPlayer->GetRoleQueue();

					bool bPlayEffect = false;
					DWORD dwOccuID = ((CCollectionGoods*)(*ObjectIter))->GetOccuID();

					if (dwOccuID!=0&&dwOccuID==pMainPlayer->GetAssOccu())
					{
						bPlayEffect = true;
					}
					const vector<tagQuestGoods>* pQustList = ((CCollectionGoods*)(*ObjectIter))->GetRoleInfo();
					if (pQustList!=NULL)
					{
						vector<tagQuestGoods>::const_iterator QustIter = pQustList->begin();
						for (;QustIter!=pQustList->end();QustIter++)
						{
							if (QustIter->nQuestId!=-1&&QustIter->nQuestId!=0)
							{
								if ((((CCollectionGoods*)(*ObjectIter))->GetOccuID()==DOccu_Other&&pQueueList!=NULL&&
									pQueueList->GetRecordByID(QustIter->nQuestId)!=NULL))
								{
									bPlayEffect = true;
									break;
								}
							}
							else if (QustIter->nQuestId==-1)
							{
								bPlayEffect = true;
								break;
							}
						}
					}

					if (bPlayEffect)
					{
						((CShape*)(*ObjectIter))->AddEffect(11114,((CShape*)(*ObjectIter))->GetPosX(),((CShape*)(*ObjectIter))->GetPosY(),((CShape*)(*ObjectIter))->GetHeight(),true);
					}
					else
						((CShape*)(*ObjectIter))->DelEffect(11114);
				}
			}
		}
		break;

		// �л�������
	case MSG_S2C_RGN_CHANGESERVER:
		{
			//��ȡ��֤��
			long lChangeServerCode = pMsg->GetLong();
			GetGame()->SetChangeServerCode(lChangeServerCode);
			char strIP[256];
			pMsg->GetStr(strIP,256);
			DWORD dwPort = pMsg->GetDWord();

			CGUID PlayerID = GetGame()->GetMainPlayer()->GetExID();

			FireUIEvent("ObjectPage", "Closed");
			GetGame()->GetGameControl()->SetMouseShape( NULL );
			GetGame()->GetGameControl()->SetSelectedShape( NULL );
			GetGame()->GetGameControl()->SetCurAttractShape( NULL );
			GetGame()->GetGameControl()->SetSendedMoveMsgCount(0);
			//GetGame()->GetCGuiEx()->GetWorldMapPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetMailPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetReadMailPageEx()->Close();

			//CMessageBoxPageEx* pMessPage = GetGame()->GetCGuiEx()->GetMessageBoxPageEx();
			//if (pMessPage!=NULL&&pMessPage->IsOpen())
			//{
			//	pMessPage->Clear();
			//	pMessPage->Close();
			//}
			//���÷����ϵͳ
			GetGame()->ResetCheatSystem();
			// �����ұ����������ӱ������Ǵ򿪵ģ���ʱҪ������ȫ���ر�
			// ��Ϊ������л�������ʱ���������Ὣ������ϵ���Ʒ���±��뵽�ͻ��ˣ���ʱ��������Ǵ򿪵ģ�
			// �ͻ�ʹ�����д�ŵ���Ʒ��ַ���ڴ��еĵ�ַ���������Ҫ�رձ������������ٴδ�ʱ�ͻ�ˢ��������Ʒ�ĵ�ַ��
			CMainPlayerHand* pMainPlayerHand = 0;//GetGame()->GetCGuiEx()->GetPMainPlayerHand();
			if (pMainPlayerHand!=NULL)
			{
				if (pMainPlayerHand->GetPGoodsOfMainPlayerHand()!=NULL)
				{
					pMainPlayerHand->GetPGoodsOfMainPlayerHand()->SetHaveShadowState(false);
				}
				pMainPlayerHand->ClearMainPlayerHand();
			}
			/*CItemPageEx* pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
			if (pItemPage!=NULL)
			{
			for (int i = 0;i<5;i++)
			{
			pItemPage->ClearPackData(i);
			pItemPage->CloseSubPack(i);
			}
			pItemPage->Close();
			}
			CNPCShopPageEx* pNPCShop = GetGame()->GetCGuiEx()->GetNPCShopPageEx();
			if (pNPCShop!=NULL&&pNPCShop->IsOpen())
			{
			pNPCShop->Close();
			}
			CCardMutifunPageEx* pMutiCardPage = GetGame()->GetCGuiEx()->GetCardMutifunPageEx();
			if (pMutiCardPage!=NULL&&pMutiCardPage->IsOpen())
			{
			pMutiCardPage->Close();
			}*/

			// ���壺�ر�֮ǰ�򿪵Ľ���
			//GetGame()->GetCGuiEx()->GetComposePageEx()->Close();
			//GetGame()->GetCGuiEx()->GetDecomposePageEx()->Close();
			//GetGame()->GetCGuiEx()->GetEquipUpgradePageEx()->Close();
			//GetGame()->GetCGuiEx()->GetEquipMagicPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetMagicDecomposePageEx()->Close();
			//GetGame()->GetCGuiEx()->GetDepotPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetFriendsPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetFactionPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetFactionListPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetUnionPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetTopMsgPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetLookPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetMadalPageEx()->Close();
			//GetGame()->GetCGuiEx()->GetCharacterPageEx()->Close();

			CClient* pTemptClient = GetGame()->GetNetClient();
			CClient* pNetClient = GetGame()->ConnectGameServer(strIP, dwPort,PlayerID);
			//����GameServer�ɹ�
			if(pNetClient != NULL)
			{
				GetGame()->SetNetClient(pNetClient);
				CMessage::SetClient(pNetClient);

				//��ģ�����������ʱ����ɾ����������̲߳����Ƿ�
				extern bool g_bDebug;
				//if( !g_bDebug )
				//{
				pTemptClient->Exit();
				pTemptClient->Close();
				SAFE_DELETE(pTemptClient);
				//}
			}
			else
			{
				GetGame()->SetNetClient(NULL);
				CMessage::SetClient(NULL);

				//����GameServerʧ��
				CMessage msg(MSG_C2S_OTHER_CONNECTGAMESERVER);
				msg.Add((long)0);
				msg.Send(pTemptClient);

				//��ģ�����������ʱ����ɾ����������̲߳����Ƿ�
				//extern bool g_bDebug;
				//if( !g_bDebug )
				//{
				pTemptClient->Close();
				SAFE_DELETE(pTemptClient);
				//}

				//GetGame()->GetInterface()->GetMsgBox()->Run("������Ϸ������ʧ��,�����½�����Ϸ!");
				GetGame()->SetGameState(GS_LOGIN);
			}
			//// ��������
			//CMainBarPageEx *pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//if(pPage)
			//{
			//	pPage->UpdataCoordinate();
			//	pPage->UpdataPlaceName();
			//}
			//GetGame()->GetCGuiEx()->OnPlayerMoving(1,1);
		}
		break;

		// �ı�����
	case MSG_S2C_RGN_CHANGEWEATHER:
		{
			GetGame()->GetWeatherManager()->EnableRain(false);
			GetGame()->GetWeatherManager()->EnableSnow(false);
			GetGame()->GetWeatherManager()->EnableFog(false);
			GetGame()->GetWeatherManager()->EnableFlutter(false);
			//GetGame()->GetWeatherManager()->SetEnvFogColor(D3DXCOLOR(0.5, 0.5, 0.5, 1));

			long lNum = pMsg->GetLong();
			for (int i=0; i<lNum; i++)
			{
				long lWeatherIndex = pMsg->GetLong();
				DWORD dwFogColor = pMsg->GetDWord();

				char str[MAX_PATH];
				utility::File file;
				switch(lWeatherIndex/100)
				{
				case 1:
					{
						sprintf(str, "data/weather/rain/%d", lWeatherIndex);

						file.Open(str);
						char *buf = new char[file.GetLength()+1];
						memcpy(buf, file.GetData(), file.GetLength());
						buf[file.GetLength()] = 0;
						file.Close();

						GetGame()->GetWeatherManager()->ModifyRainParameter(buf);
						GetGame()->GetWeatherManager()->EnableRain();

						SAFE_DELETE(buf);
					}
					break;
				case 2:
					{
						sprintf(str, "data/weather/snow/%d", lWeatherIndex);

						file.Open(str);
						char *buf = new char[file.GetLength()+1];
						memcpy(buf, file.GetData(), file.GetLength());
						buf[file.GetLength()] = 0;
						file.Close();

						GetGame()->GetWeatherManager()->ModifySnowParameter(buf);
						GetGame()->GetWeatherManager()->EnableSnow();

						SAFE_DELETE(buf);
					}
					break;
				case 3:
					{
						sprintf(str, "data/weather/fog/%d", lWeatherIndex);

						file.Open(str);
						char *buf = new char[file.GetLength()+1];
						memcpy(buf, file.GetData(), file.GetLength());
						buf[file.GetLength()] = 0;
						file.Close();

						GetGame()->GetWeatherManager()->ModifyFogParameter(buf);
						GetGame()->GetWeatherManager()->EnableFog();

						SAFE_DELETE(buf);
					}
					break;
				case 4:
					{
						//sprintf(str, "data/weather/envfog/%d", lWeatherIndex);

						//file.Open(str);
						//char *buf = new char[file.GetLength()+1];
						//memcpy(buf, file.GetData(), file.GetLength());
						//buf[file.GetLength()] = 0;
						//file.Close();

						//GetGame()->GetWeatherManager()->ModifyEnvFogParameter(buf);
						////GetGame()->GetWeatherManager()->EnableEnvFog();

						//SAFE_DELETE(buf);

						//GetGame()->GetWeatherManager()->SetEnvFogColor(dwFogColor);
						//GetGame()->UpdateSetup();
					}
					break;
				case 5:
					{
						sprintf(str, "data/weather/Flutter/%d", lWeatherIndex);

						file.Open(str);
						char * buf = new char[file.GetLength() + 1];
						memcpy(buf, file.GetData(), file.GetLength());
						buf[file.GetLength()] = 0;
						file.Close();

						GetGame()->GetWeatherManager()->ModifyFlutterParameter(buf);

						D3DXMATRIX matBillboard, matBillboardY, matWorld;
						D3DXVECTOR3 * vView = GetGame()->GetRegion()->GetCamera()->GetView();
						matBillboard = *GetGame()->GetRegion()->GetCamera()->GetBillboard();
						matBillboardY = *GetGame()->GetRegion()->GetCamera()->GetBillboardY();
						D3DXMatrixTranslation(&matWorld, vView->x, vView->y, vView->z);

						GetGame()->GetWeatherManager()->SetWorldMatrix(&matWorld);
						GetGame()->GetWeatherManager()->SetBillboard(&matBillboard, &matBillboardY);
						GetGame()->GetWeatherManager()->EnableFlutter();

						SAFE_DELETE(buf);
					}
					break;
				}
			}
		}
		break;

		// ���Ŷ���
	case MSG_S2C_RGN_PLAYACTION:
		{
			long lType	= pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);

			CShape* pShape = (CShape*)FindChildObject(lType, ID);
			if (pShape)
			{
				WORD wAction = pMsg->GetWord();
				pShape->SetAction(wAction);

				if (wAction>=CShape::ACT_ATTACK)
					pShape->SetState(CShape::STATE_FIGHT);
			}
		}
		break;

		// ��������
	case MSG_S2C_RGN_PLAYSOUND:
		{
			char strFile[MAX_PATH]="";
			pMsg->GetStr(strFile, MAX_PATH);
			GetGame()->GetAudioList()->Play(strFile, false, true);
		}
		break;

		// ����3D����
	case MSG_S2C_RGN_PLAY3DSOUND:
		{
			char strFile[MAX_PATH]="";
			pMsg->GetStr(strFile, MAX_PATH);
			long lLoop = pMsg->GetLong();    //ѭ������
			long lPosX = pMsg->GetLong();    //����X
			long lPosY = pMsg->GetLong();    //����Y
			long lPosH = pMsg->GetLong();    //����Z
			if(lLoop==0)
			{
				GetGame()->GetAudioList()->Stop3DSound(strFile,-1);  //lLoop=0ʱֹͣ���Ÿ���Ч
			}else{
				GetGame()->GetAudioList()->Play3DSound(strFile,lLoop,(float)lPosY,(float)lPosH,(float)lPosX); 
			}
		}
		break;
		//��������
	case MSG_S2C_RGN_PLAYMUSIC:
		{
			char strFile[MAX_PATH]="";
			pMsg->GetStr(strFile, MAX_PATH);
			long lLoop = pMsg->GetLong();    //ѭ������
			//�ͷ���ǰ������
			GetGame()->GetAudioList()->ReleaseMusic(GetGame()->GetAudioList()->GetBackgroundMusic());
			map<long,CGame::tagRegion>::iterator RegionDataIter=GetGame()->GetRegionSetup()->find(
				GetGame()->GetRegion()->GetResourceID());
			if(RegionDataIter!=GetGame()->GetRegionSetup()->end())
			{
				RegionDataIter->second.bgMusicPlaying=false;	
			}
			GetGame()->GetAudioList()->FadeInMusic(strFile,8000,lLoop); 
			GetGame()->GetAudioList()->SetBackgroundMusic(strFile);
		}
		break;
		//ֹͣ��������
	case MSG_S2C_RGN_STOPMUSIC:
		{
			//�ͷ���ǰ������
			GetGame()->GetAudioList()->ReleaseMusic(GetGame()->GetAudioList()->GetBackgroundMusic());
			map<long,CGame::tagRegion>::iterator RegionDataIter=GetGame()->GetRegionSetup()->find(
				GetGame()->GetRegion()->GetResourceID());
			if(RegionDataIter!=GetGame()->GetRegionSetup()->end())
			{
				if(!RegionDataIter->second.bgMusicPlaying)
				{
					//�³����������ֽ���
					RegionDataIter->second.bgMusicPlaying=true;	
					const char *filename=RegionDataIter->second.strBgMusic.c_str();
					GetGame()->GetAudioList()->FadeInMusic(filename,8000,0);
					GetGame()->GetAudioList()->SetBackgroundMusic(filename);
				}
			}
		}
		break;

		// ������Ч
	case MSG_S2C_RGN_PLAYEFFECT:
		{
			BYTE bInRegion = pMsg->GetByte();
			long lEffectID = pMsg->GetLong();
			long lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);

			float fx = pMsg->GetFloat();
			float fy = pMsg->GetFloat();
			long lPos = pMsg->GetLong();
			if(bInRegion)
			{
				PlayEffect(lEffectID, fx, fy,0.f,-1.0f,false);
			}
			else
			{
				CShape* pShape = (CShape*)FindChildObject(lType,guid);
				if (pShape)
				{
					CShape::eLOCATOR ePos = CShape::LOCATOR_FOOTCENTER;
					switch(lPos)
					{
					case 0: ePos = CShape::LOCATOR_WIND; break;
					case 1: ePos = CShape::LOCATOR_HEAD; break;
					case 2: ePos = CShape::LOCATOR_FOOTCENTER; break;
					case 3: ePos = CShape::LOCATOR_LEFTHAND; break;
					case 4: ePos = CShape::LOCATOR_RIGHTHAND; break;
					case 5: ePos = CShape::LOCATOR_LEFTFOOT; break;
					case 6: ePos = CShape::LOCATOR_RIGHTFOOT; break;
					default: ePos = CShape::LOCATOR_FOOTCENTER; break;
					}
					pShape->AddEffectLocator(lEffectID,pShape,ePos,false,0.f,0,false);
				}
			}
		}
		break;
	case MSG_S2C_RGN_ANSWER_CREATE:
		{
			long rgnType = pMsg->GetLong();
			long templateRgnID = pMsg->GetLong();
			long lResourceID = pMsg->GetLong();
			CGUID rgnID,ownerID;
			pMsg->GetGUID(rgnID);
			pMsg->GetGUID(ownerID);
		}
		break;
	case MSG_S2C_RGN_ANSWER_DELETE:
		{

		}
		break;
		// �ı�ݵ�״̬
	case MSG_S2C_RGN_STRONGPOINT_STATE_CHANGE:
		{
			string	strName = "";
			char	szName[256] = "";
			long	lState = 0;
			long lRgnID = pMsg->GetLong();
			CGUID rgnID;
			pMsg->GetGUID(rgnID);
			if (rgnID == GetExID())
			{
				strName = pMsg->GetStr(szName,256);
				lState	= pMsg->GetLong();
				ChangeSPointState(strName.c_str(),(BYTE)lState);
			}
		}
		break;
		//case MSG_S2C_RGN_CLEARDUPRGNPLAYER:
		//	{
		//		//���������ر�֪ͨ
		//		BYTE bType = pMsg->GetByte();
		//		DWORD dwTime = pMsg->GetDWord();
		//		CTimeTextPageEx *pPage =  GetGame()->GetCGuiEx()->GetTimeTextPageEx();
		//        if(pPage==NULL)
		//			return;
		//		pPage->SetBoxType(CTimeTextPageEx::TIME_TEXTBOX);
		//		if(bType==RGN_CLEARDUPRGNPLAYER)
		//		{
		//              pPage->SetTimeBoxOpen(2,dwTime);
		//		}else if(bType==FCM_KICK_PLAYER)
		//		{
		//              pPage->SetTimeBoxOpen(3,dwTime);
		//		}


		//	}
		//	break;
	}
}
