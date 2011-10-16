#include "stdafx.h"
#include "Message.h"
#include "ClientRegion.h"
#include "Player.h"
#include "Build.h"
#include "Citygate.h"
#include "Monster.h"
#include "CollectionGoods.h"
#include "../Goods/Goods.h"
#include "../GameClient/Game.h"
#include "Other/DeputyOccuSystemClient.h"
#include "PetSystem/Pet.h"
#include "../GameClient/GameControl.h"
#include "../MsgBoxSys/MsgEventDef.h"
#include "../MsgBoxSys/MsgEventManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void RelivePlayerSelf(MsgEvent& event,eMsgResult UserId);
/*
 * ����: ��Ӧ������Ϣ
 * ժҪ: -
 * ����: pMsg - ���������͵���Ϣ
 * ����ֵ: -
 * ����: -
 * ��������: -
 * �޸���־:
 *	2008.10.21 - lpf
 *		��MSG_S2C_SHAPE_RELIVE��Ϣ�������˶������»������ǰ��������;
 *	2008.11.18 - lpf
 *		��MSG_S2C_SHAPE_CHANGEMOVEMODE��Ϣ���������ڸı����ʵĺ����SetAction(),���������޷���������
 */
void CMoveShape::OnMessage(CMessage* pMsg)
{
	if(!pMsg->GetRegion())
		return;
	switch(pMsg->GetType())
	{
		// �ı���ҷ���
	case MSG_S2C_SHAPE_CHANGEDIR:
		{
			BYTE lDir = pMsg->GetByte();
			float fDir = pMsg->GetFloat();
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);

			CMoveShape* pMoveShape =  dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, ID));
			if( pMoveShape )
			{
				if(pMoveShape->GetAction() == CShape::ACT_WALK  ||
					 pMoveShape->GetAction() == CShape::ACT_RUN )
				{
					pMoveShape->SetEndMoveTime(timeGetTime());
					pMoveShape->SetPosXY(pMoveShape->GetDestX(),pMoveShape->GetDestY());
					pMoveShape->SetAction(CShape::ACT_STAND);
				}
				pMoveShape->SetDirEx(fDir);
				// �ж��Ƿ������,�����������λ��
				if (GetGame()->GetMainPlayer()->GetExID() == ID)
					GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();
			}
		}
		break;
	case MSG_S2C_SHAPE_CHANGE_PROPERTIES:
		{
			//CCharacterPageEx* pCharacterPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
			//CItemPageEx*	  pItemPage = GetGame()->GetCGuiEx()->GetItemPageEx();
			//CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			//CFacturePageEx* pFacturePage = GetGame()->GetCGuiEx()->GetFacturePageEx();
			//CNPCShopPageEx* pNPCpage = GetGame()->GetCGuiEx()->GetNPCShopPageEx();
			long lType=pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			CShape *pShape = (CShape *)GetGame()->GetRegion()->FindChildObject(lType,ID);
			if (lType == TYPE_PLAYER)
			{
				CPlayer *m_pPlayer=(CPlayer *)pShape;
				if (m_pPlayer)
				{
					long lOldCollectExp = m_pPlayer->GetCollectSkillExp();
					//long lOldCollectLevel = m_pPlayer->GetCollectSkillLevel();
					m_pPlayer->UpdateProperty(pMsg);
					if (m_pPlayer->GetExID()==GetGame()->GetMainPlayer()->GetExID())
					{
						/*if (pCharacterPage!=NULL&&pCharacterPage->IsOpen())
						{
							pCharacterPage->UpdatePlayerInfor();	
						}
						if (pItemPage!=NULL&&pItemPage->IsOpen())
						{
							pItemPage->YBMoneyInit();
							pItemPage->UpdataAllItems();
							pItemPage->UpdataAllSubPack();
						}
						if (pMainBarPage!=NULL)
						{
							pMainBarPage->UpdatePlayerInforHar();
							pMainBarPage->UpdataPlayerExp();
                            pMainBarPage->UpdateSpExpBar();
						}*/
						long lNewCollectExp = m_pPlayer->GetCollectSkillExp();
						long lCollectLevel = m_pPlayer->GetCollectSkillLevel();
						long lCollectExp = 0;
						if (lNewCollectExp>lOldCollectExp)
						{
							lCollectExp = lNewCollectExp-lOldCollectExp;
						}
						//else if(lNewCollectExp<lOldCollectExp)
						//{
						//	lCollectExp = CDeputyOccuSystemClient::GetCollectionNeededExp(lCollectLevel)-lOldCollectExp+lNewCollectExp;
						//}
						if (lCollectExp>0)
						{
							char strExp[64];
                            sprintf(strExp,AppFrame::GetText("Deputy_24")   //�������%d�ɼ�����
                                ,lCollectExp);
							//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(strExp);
						}
						//if (pFacturePage!=NULL&&pFacturePage->IsOpen())
						//{
						//	DWORD dwFactureID = pFacturePage->GetSelectFactureItemsID();
						//	DWORD dwOldFactureExp = pFacturePage->GetFactureExpProgressbar()->GetCurValue();
						//	pFacturePage->UpdateFactureLevelExpProcessBar();
						//	if (dwOldFactureExp!=pFacturePage->GetFactureExpProgressbar()->GetCurValue())
						//	{
						//		const CFacture* pSelectFacture = CDeputyOccuSystemClient::GetFacture(dwFactureID);
						//		if (pSelectFacture!=NULL)
						//		{
						//			//�����������Ϣ
						//			char strExp[64];
						//			DWORD dwExp = CDeputyOccuSystemClient::GetFactureExp((LONG)(m_pPlayer->GetFactureSkillLevel()-pSelectFacture->GetFactureLevel()));	
						//			if (dwExp>0)
						//			{
						//				sprintf(strExp,CStringReading::LoadText(205,23),dwExp);
						//				GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddOtherInfo(strExp);
						//			}
						//		}
						//	}
						//	pFacturePage->UpdateFactureTree();
						//	pFacturePage->SetSelectFactureID(dwFactureID);
						//	pFacturePage->UpdateMaterial();
						//}
						//������ҵ���ܰ�ť����ʾ
						//GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetOccuButtonVisible();
      //                  long lOccType = (long)m_pPlayer->GetAssOccu();
						////������ѧϰ����ҵ���ܣ����ð�ť��˸
						//if(GetGame()->GetCGuiEx()->GetMainBarPageEx()->GetOuccType()!=lOccType)
						//{
						//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetOccuButtonSpecialState(true,lOccType);
						//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetOcuuType(lOccType);
						//}
					}

                    FireUIEvent("MainRole", "RoleAttr");
				}
			}
			else if (lType == TYPE_MONSTER)
			{
				CMonster* m_pMonster = (CMonster*)pShape;
				if (m_pMonster)
				{
					m_pMonster->UpdateProperty(pMsg);
				}
			}
			else if (lType == TYPE_BUILD||lType == TYPE_CITYGATE)
			{
				CCityGate* m_pCityGate = (CCityGate*)pShape;
				if (m_pCityGate)
				{
                    m_pCityGate->UpdateProperty(pMsg);
				}
			}
			else if (lType == TYPE_PET)
			{
				CPet* pet;
				if (pShape==NULL)
				{
					pet = GetGame()->GetMainPlayer()->GetPet(ID);
				}
				else
					pet = (CPet*)pShape;
				if (pet)
					pet->UpdateProperty(pMsg);
			}
			// ������µ�Shape�ǵ�ǰѡ��Ķ��󣬸�����Ϣ
			//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
			//if(pObjPage && (pObjPage->GetObjShape()==pShape))
			//{
			//	pObjPage->SetObjectHpInfo();
			//	pObjPage->SetObjectPKCP();
			//}
            if ( pShape == GetGame()->GetGameControl()->GetSelectedShape() )
            {
                FireUIEvent("ObjectPage", "ObjectAttr");
            }
		}
		break;
	case MSG_S2C_SHAPE_SETPOS:	// ˲��
		{
			long	  lType		  = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			float x = pMsg->GetFloat();
			float y = pMsg->GetFloat();
			long lUseGoods = pMsg->GetLong();
            static char Debugstring[256];
			CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, ID));
			if( pShape )
			{	
				if (pShape->GetType() == TYPE_PET)
				{
					CPet *pPet = (CPet *)pShape;
					if (pPet->IsMainPlayerPet())
					{	
						pPet->ClearFindPath();
					}
				}
				if(pShape->GetTileX() != x || pShape->GetTileY() != y)
				{
					pShape->SetPosXY(x, y);
				}
				CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pShape);
				if (pMoveShape)
				{
					pMoveShape->SetDestXY(pShape->GetPosX(), pShape->GetPosY());
				}
				pShape->SetEndMoveTime(timeGetTime());
                sprintf_s<256>(Debugstring,"���:%s ˲�Ƶ� X: %f Y:%f \n",pShape->GetName(),x,y);
                if (pShape == GetGame()->GetMainPlayer())
                {
                    // �ж��Ƿ������,�����������λ�ú͸������������Ƿ�ر�
                    GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();
                    // ��������
                    //GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdataCoordinate();
                    GetGame()->GetRegion()->GetRegionCamera()->SetSmooth(false);
                    GetGame()->GetRegion()->GetRegionCamera()->SetCamera(GetGame()->GetRegion()->GetCamera(),pShape->GetPosX(),pShape->GetPosY());
                    GetGame()->GetRegion()->GetRegionCamera()->SetSmooth(true);
                    //���ø���λ��
                    GetGame()->GetGameControl()->SetUpdateXY(x,y);
                    //����Զ�Ѱ·��·��
                    GetGame()->GetGameControl()->GetFindPath()->GetPathList()->clear();

                    GetGame()->GetGameControl()->SetSendedMoveMsgCount(0);

                    // @todo ������
                    CGUID activePetGuid = GetGame()->GetMainPlayer()->GetActivePetGUID();
                    CPet *pPet = GetGame()->GetMainPlayer()->GetPet(activePetGuid);
                    if (pPet)
                    {
                        pPet->UpdateTargetPos(pShape);
                    }
                    sprintf_s<256>(Debugstring,"�㱻˲�Ƶ� X: %f Y:%f \n", x,y);
                }
                OutputDebugString(Debugstring);
                PutStrToFileSaveToDir("log","MoveDebug",Debugstring);
			}
		}
		break;
	case MSG_S2C_SHAPE_MOVE:
		{
			CGUID guid;
			pMsg->GetGUID(guid);

			CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, guid));
			if(!pShape)
			{
				pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_MONSTER, guid));
				if(!pShape)
					pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PET, guid));
			}
			if(pShape)
			{
				long lType =pShape->GetType();
				//�����Լ�
				if(pShape != GetGame()->GetMainPlayer())
				{
					if (lType== TYPE_PET)
					{
						CPet *pPet =(CPet*)pShape;
						if (pPet->IsMainPlayerPet())
						{
							return;
						}
					}
                    //�Ƿ���Ҫ�����ƶ�
                    BYTE bStopFlag = pMsg->GetByte();
                    if (bStopFlag)
                    {
                        //ֹͣ�ƶ�
                        float fStopPosX = pMsg->GetFloat();
                        float fStopPosY = pMsg->GetFloat();
                        pShape->SetPosXY(fStopPosX,fStopPosY);
                        pShape->SetDestXY(fStopPosX,fStopPosY);
                        pShape->SetAction(ACT_STAND);
                    } 
                    else//��Ҫ�����ƶ�
                    {
                        float fDestX = pMsg->GetFloat();
                        float fDestY = pMsg->GetFloat();
                        float fx =  pShape->GetPosX();
                        float fy = pShape->GetPosY();

                        DWORD dwCurTime = GetCurTickCount();
                        if(pShape->GetAction() != CShape::ACT_WALK &&
                            pShape->GetAction() != CShape::ACT_RUN)
                        {
                            //������ڵĶ����������߶���,�����ϴεĸ���ʱ��
                            pShape->SetLastUpdatePosTime(dwCurTime);
                        }
                        //���Ŀ��λ�����̫��,������λ�á�
                        /*if(abs(pShape->GetTileX() - fDestX) >=5 ||
                            abs(pShape->GetTileY() - fDestY) >= 5 )
                        {
                            pShape->SetPosXY(fDestX,fDestY);
                            pShape->SetDestXY(fDestX,fDestY);
                            pShape->SetAction(ACT_STAND);
                            break;
                        }*/

                        //��������߹�����				
                        WORD action = CShape::ACT_WALK;
                        if(pShape->GetIsRun())
                            action = CShape::ACT_RUN;
                        if(pShape->GetAction() != action)
                            pShape->SetAction(action);

                        float fRadianDir = pShape->GetLineDirF(fx,fy,fDestX,fDestY);
                        //�����µķ���
                        pShape->SetDirEx(fRadianDir);
                        pShape->SetDestXY(fDestX,fDestY);
                        //�ó��ƶ�ʱ��
                        float fTotalDistance = pShape->RealDistance(fDestX,fDestY);
                        float dwMoveTime = fTotalDistance/pShape->GetSpeed();
                        pShape->SetEndMoveTime(dwCurTime+(DWORD)dwMoveTime);
                        pShape->SetMoveOffset((fDestX-fx)/dwMoveTime, (fDestY-fy)/dwMoveTime);
                    }
				}
				else
				{
					long lCount = GetGame()->GetGameControl()->GetSendedMoveMsgCount();
					if(lCount > 0)
						GetGame()->GetGameControl()->SetSendedMoveMsgCount(lCount-1);
				}
			}
		}
		break;
	// ���壺��������ս��Ч��
	case MSG_S2C_SHAPE_LEAVEFIGHT:
		{
			// Shape �����ͺ�GUID
			long typeShape = pMsg->GetLong();
			CGUID guidShape;
			pMsg->GetGUID( guidShape );

			// Ŀ������
			long	xDest = pMsg->GetLong();
			long	yDest = pMsg->GetLong();

			// ��Чʱ��
			long	time = pMsg->GetLong();

			// �ҵ� ����
			if( CMonster* pShape = dynamic_cast<CMonster*>( pMsg->GetRegion()->FindChildObject( typeShape, guidShape ) ) )
			{
				// �˳�ս��ԭ·��������
				pShape->RunLeaveFightAI( xDest, yDest, time );
			}
		}
		break;
	case MSG_S2C_SHAPE_STOPMOVE:
		{			
			//CGUID guid;
			//pMsg->GetGUID(guid);
			//CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, guid));
			//if(!pShape)
			//{
			//	pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_MONSTER, guid));
			//	if(!pShape)
			//		pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PET, guid));
			//}
			//if(pShape == GetGame()->GetMainPlayer())
			//	break;
			//if(pShape)
			//{
			//	long lType = pShape->GetType();
			//	long lDestX = pMsg->GetShort();
			//	long lDestY = pMsg->GetShort();
			//	long lDir = pMsg->GetByte();

			//	float fDestX = lDestX+0.5f;
			//	float fDestY = lDestY+0.5f;

			//	bool bFlag = false;
			//	//��������߹�����
			//	if(pShape->GetAction() == CShape::ACT_RUN ||
			//		pShape->GetAction()== CShape::ACT_WALK)
			//	{	//���Ŀ�겻һ��������λ��
			//		if(pShape->GetDestX() != lDestX ||
			//			pShape->GetDestY() != lDestY )
			//		{
			//			pShape->SetPosXY(fDestX,fDestY);
			//			bFlag = true;
			//		}
			//	}
			//	else
			//	{
			//		if(pShape->GetTileX() != (long)fDestX || pShape->GetTileY() != (long)fDestY)
			//		{
			//			pShape->SetPosXY(fDestX,fDestY);
			//		}
			//		bFlag = true;
			//	}
			//	if(bFlag)
			//	{
			//		pShape->SetAction(CShape::ACT_STAND);					
			//		pShape->SetDir((CShape::eDIR)lDir);
			//		pShape->SetDestXY(fDestX,fDestY);
			//	}
			//}
		}
		break;
		//shape�������ٶȷ����仯
	case MSG_S2C_SHAPE_CHANGEMOVEMODE:
		{
			long lType = pMsg->GetLong();
			CGUID shapeGuid;
			pMsg->GetGUID(shapeGuid);
			char bRun = pMsg->GetChar();
			//�����ٶ�
			float fSpeed = pMsg->GetFloat();
			float fRatio = pMsg->GetFloat();

			//�仯��ֵ
			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(lType,shapeGuid));
			if(pMoveShape)
			{
				bool bOldRun = pMoveShape->GetIsRun();
				pMoveShape->SetIsRun(bRun==0?false:true);
				//���������ٶ�
				pMoveShape->SetSpeed(fSpeed);
				pMoveShape->SetActionSpeedRate(ACT_WALK,fRatio);
				pMoveShape->SetActionSpeedRate(ACT_RUN,fRatio);

				extern bool g_bForceUpdate;
				g_bForceUpdate = true;
				pMoveShape->SetAction(pMoveShape->GetAction());
				if(pMoveShape == GetGame()->GetMainPlayer())
				{
					if(pMoveShape->GetIsRun() != bOldRun)
					{
						if(!bRun)
						{
							//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER
							//	,CStringReading::LoadText(eIDS_CONTROL,eIDS_CONTROL_CHANGETOWALKMODE),D3DCOLOR_ARGB(255,255,255,0));
						}
						else
						{
							//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER
							//	,CStringReading::LoadText(eIDS_CONTROL,eIDS_CONTROL_CHANGETORUNMODE),D3DCOLOR_ARGB(255,255,255,0));
						}
					}
					//�ó��ƶ�ʱ��
					float fTotalDistance = pMoveShape->RealDistance(pMoveShape->GetDestX(),pMoveShape->GetDestY());
					DWORD dwMoveTime = (DWORD)(fTotalDistance/pMoveShape->GetSpeed());
					DWORD dwCurTime = GetCurTickCount();
					pMoveShape->SetEndMoveTime(dwCurTime+dwMoveTime);
					if(pMoveShape->GetAction() != CShape::ACT_WALK &&
						pMoveShape->GetAction() != CShape::ACT_RUN)
					{
						//������ڵĶ����������߶���,�����ϴεĸ���ʱ��
						pMoveShape->SetLastUpdatePosTime(dwCurTime);
					}
				}
			}
		}
		break;
	case MSG_S2C_SHAPE_ATK_DAMAGE:
		{
			// ��Ϣ˳��lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			long lTargType = pMsg->GetLong();			// Ŀ��������
			CGUID TargetID;
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType,TargetID);
			if(pTargetShape)
			{
				tagAttackEvent st;
				st.cType = AE_DAMAGE;							// ��Ѫ�˺�

				char cSize = pMsg->GetChar();
				for(int i = 0; i<cSize; i++)
				{
					tagLost stLost;
					stLost.cLostType = pMsg->GetChar();			// �˺�����
					stLost.lLostPoint = pMsg->GetLong();		// �˺�����
					st.vtLost.push_back(stLost);
					st.lTotalLostPoint += stLost.lLostPoint;	// �����ϼ�
				}
				st.lFinalHP = pMsg->GetDWord();					//��������ʣ���HP
				st.bBlastAttack = (pMsg->GetChar()==0)?false:true;// �Ƿ񱬻�

				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else		// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// �յ�����ָ��
	case MSG_S2C_SHAPE_ATK_BREAK:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
			{
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();
				//�������ң��������װ�����;ö�
				//GetGame()->GetCGuiEx()->GetEquipEwearPageEx()->CheckEquip();
			}


			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape)
			{
				if(pTargetShape->IsDied())
					break;
				CGUID	MainPlayerGuid = GetGame()->GetMainPlayer()->GetExID();
				if (MainPlayerGuid == TargetID		// �Լ��ܵ��˺������ﻹ��	
					|| MainPlayerGuid == ActorID )	// �Լ����𹥻�ʹĿ������,����Эͬ������ս
				{
					// @todo ������
					CGUID activePetGuid = GetGame()->GetMainPlayer()->GetActivePetGUID();
					CPet *pPet = GetGame()->GetMainPlayer()->GetPet(activePetGuid);
					if (pPet && pPet->GetPetFightMode() != PET_FIGHETYPE_DEFEND)
					{
						CMoveShape *pAtkShape = pPet->GetCurAttackShape();
						if (pAtkShape == NULL || (pAtkShape && pAtkShape->IsDied()))
						{
							pPet->SetIsRetaliateUponNextShape(true);
							pPet->PetContAttRecved();
							if(MainPlayerGuid == TargetID)	//���ﻹ��
							{
								CMoveShape *pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
								pPet->SetCurAttackShape(pActorShape);
								pPet->UpdateTargetPos(pActorShape);
							}
							else if (MainPlayerGuid == ActorID)	//����Эͬ������ս
							{
								pPet->SetCurAttackShape(pTargetShape);
								pPet->UpdateTargetPos(pTargetShape);
							}
						}
					}
				}
				
				if (pTargetShape->GetType() == TYPE_PET)
				{
					CPet *pPet = (CPet*)pTargetShape;
					if (pPet->GetCurAttackShape() == NULL)
					{
						pPet->SetIsRetaliateUponNextShape(true);
						pPet->PetContAttRecved();
					}
				}

				// ����һ�����˵��¼�
				tagAttackEvent st;
				st.cType = AE_BREAK;			// ����
				st.Guidattracker = ActorID;
				char cSize = pMsg->GetChar();
				for(int i=0; i<cSize; i++)
				{
					tagLost stLost;
					stLost.cLostType = pMsg->GetChar();			// �˺�����
					stLost.lLostPoint = pMsg->GetLong();		// �˺�����
					st.vtLost.push_back(stLost);
					st.lTotalLostPoint += stLost.lLostPoint;	// �����ϼ�
				}

				st.lFinalHP = pMsg->GetDWord();					//��������ʣ���HP
				st.bBlastAttack = pMsg->GetChar()==0?false:true;// �Ƿ񱬻�
				//���뼼������
				st.dwSkillID = pMsg->GetLong();				// ���ܱ��
				st.bySkillLevel = pMsg->GetByte();				// ���ܵȼ�

				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;

		// ��ȫ����
	case MSG_S2C_SHAPE_ATK_FULLMISS:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();		// �����ߵ�����
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();		// Ŀ��������
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_FULLMISS;
				st.bBlastAttack = pMsg->GetChar()==0?false:true;		// �Ƿ�Ϊ����
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// �м�
	case MSG_S2C_SHAPE_ATK_PARRY:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_PARRY;				// �м�
				st.bBlastAttack = pMsg->GetChar()==0?false:true;		// �Ƿ�Ϊ����
				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// ��
	case MSG_S2C_SHAPE_ATK_BLOCK:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();			

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_BLOCK;				// ��
				char cSize = pMsg->GetChar();
				for(int i=0; i<cSize; i++)
				{
					tagLost stLost;
					stLost.cLostType = pMsg->GetChar();		// �˺�����
					stLost.lLostPoint = pMsg->GetLong();	// �˺�����
					st.vtLost.push_back(stLost);
					st.lTotalLostPoint += stLost.lLostPoint;	// �����ϼ�
				}

				st.lFinalHP = pMsg->GetDWord();					//��������ʣ���HP
				st.bBlastAttack = pMsg->GetChar()==0?false:true;// �Ƿ񱬻�
				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// �ֿ�
	case MSG_S2C_SHAPE_ATK_RESIST:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|bBlastAttack
			long lActorType = pMsg->GetLong();		// �����ߵ�����
			CGUID ActorID,TargetID;
			pMsg->GetGUID(ActorID);		// ������ID
			long lTargType = pMsg->GetLong();		// Ŀ��������
			pMsg->GetGUID(TargetID);;		// Ŀ����ID

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_RESIST;				// �ֿ�
				st.bBlastAttack = pMsg->GetChar()==0?false:true;		// �Ƿ�Ϊ����
				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// ����
	case MSG_S2C_SHAPE_ATK_REBOUND:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_REBOUND;				// ����
				char cSize = pMsg->GetChar();
				for(int i=0; i<cSize; i++)
				{
					tagLost stLost;
					stLost.cLostType = pMsg->GetChar();		// �˺�����
					stLost.lLostPoint = pMsg->GetLong();	// �˺�����
					st.vtLost.push_back(stLost);
					st.lTotalLostPoint += stLost.lLostPoint;	// �����ϼ�
				}
				st.lFinalHP = pMsg->GetDWord();					//��������ʣ���HP
				st.bBlastAttack = pMsg->GetChar()==0?false:true;// �Ƿ񱬻�
				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
	case MSG_S2C_SHAPE_ATK_SUCK:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape )
			{
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_SUCK;					// ����
				char cSize = pMsg->GetChar();
				for(int i=0; i<cSize; i++)
				{
					tagLost stLost;
					stLost.cLostType = pMsg->GetChar();		// �˺�����
					stLost.lLostPoint = pMsg->GetLong();	// �˺�����
					st.vtLost.push_back(stLost);
					st.lTotalLostPoint += stLost.lLostPoint;	// �����ϼ�
				}

				st.lFinalHP = pMsg->GetDWord();					//��������ʣ���HP
				st.bBlastAttack = pMsg->GetChar()==0?false:true;// �Ƿ񱬻�
				// ��ȡ���˵ķ���
				CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType, ActorID);
				if (pActorShape)
				{
					st.fHurtedDir = CShape::GetLineDirF(pActorShape->GetPosX(), pActorShape->GetPosY(), pTargetShape->GetPosX(), pTargetShape->GetPosY());
				}
				pTargetShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// �ܵ�����ָ��
	case MSG_S2C_SHAPE_ATK_DIED:
		{
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);

			// �ж��Ƿ������,�����������λ��

			if (GetGame()->GetMainPlayer()->GetExID() == TargetID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType,TargetID);
			if( pTargetShape )
			{
				// ����һ���������¼�
				tagAttackEvent st;
				st.Guidattracker = ActorID;
				st.cType = AE_DIED;
				st.lHitNum = -1;
				pTargetShape->GetAttackEventQueue()->push(st);

				// �����monster������������Ч
				if (pTargetShape->GetType() == TYPE_MONSTER || pTargetShape->GetType() == TYPE_PET)
				{
					long lGraphicsID = 900000 + pTargetShape->GetGraphicsID();
					char strFile[MAX_PATH];
					sprintf(strFile, "model/effect/%d/model", lGraphicsID);
					bool bExist = GetGame()->IsFileExist(strFile);
					if (bExist)
					{
						GetGame()->GetRegion()->PlayEffect(lGraphicsID, pTargetShape->GetPosX(), pTargetShape->GetPosY());
					}
					// ����֮�����ٶԻ�
					((CMonster *)pTargetShape)->SetIsCanTalk(0);
				}
				else if(pTargetShape->GetType()==TYPE_BUILD || pTargetShape->GetType()==TYPE_CITYGATE)
				{
					long lGraphicsID = 1000000 + pTargetShape->GetGraphicsID();
					char strFile[MAX_PATH];
					sprintf(strFile, "model/effect/%d/model", lGraphicsID);
					bool bExist = GetGame()->IsFileExist(strFile);
					if (bExist)
					{
						GetGame()->GetRegion()->PlayEffect(lGraphicsID, pTargetShape->GetPosX(), pTargetShape->GetPosY());
					}
				}
				else if(pTargetShape==GetGame()->GetMainPlayer())	// �Լ�����
				{
					//�������м������ ���������״̬
					CPet* pet = GetGame()->GetMainPlayer()->GetActivePet();
					if (pet)
						pet->OnHostDied();
					/***********************************************************************/
					/*  zhaohang 2010-10-12 �򵥼��븴��� */
					/***********************************************************************/

					GetInst(MsgEventManager).PushEvent(Msg_YesNo,
						AppFrame::GetText("RELIVE_1"),RelivePlayerSelf);
				}
				else if (pTargetShape->GetType() == TYPE_PET && ((CPet*)pTargetShape)->IsMainPlayerPet())
				{
					((CPet*)pTargetShape)->OnDied();
					CPlayer *pMaster = ((CPet*)pTargetShape)->GetMaster();
					if(pMaster)
						//pMaster->RemovePet(pTargetShape->GetExID()); 
					{}
				}
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lTargType);
				msg.Add(TargetID);
				msg.Send();*/
			}
		}
		break;
		// ��ʧ
	case MSG_S2C_SHAPE_ATK_LOSE:
		{
			// ��ʧ�ߵ�ID������
			long lActorType = pMsg->GetLong();
			CGUID ActorID;
			pMsg->GetGUID(ActorID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == ActorID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pActorShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lActorType,ActorID);
			if( pActorShape )
			{
				// ����һ����ʧ���¼�
				tagAttackEvent st;
				st.cType = AE_LOST;
				pActorShape->GetAttackEventQueue()->push(st);
			}
			else	// û���ҵ����shape������Ϣ������������shape������
			{
				/*CMessage msg(MSG_C2S_SHAPE_QUESTINFO);
				msg.Add(lActorType);
				msg.Add(ActorID);
				msg.Send();*/
			}
		}
		break;

		// �ı�״̬
	case MSG_S2C_SHAPE_CHANGESTATE:
		{
			long lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			long lState = pMsg->GetLong();

			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, guid) );
			if (pMoveShape)
			{
				if (lState==CShape::STATE_FIGHT)
				{
					if (pMoveShape == GetGame()->GetMainPlayer())
					{
						long PetNum = GetGame()->GetMainPlayer()->GetFightPetNum();
						if (PetNum)
						{
							//@todo ������
							CGUID activePetGuid = GetGame()->GetMainPlayer()->GetActivePetGUID();
							CPet *pPet = GetGame()->GetMainPlayer()->GetPet(activePetGuid);
							if (pPet && pPet->GetCurAttackShape() == NULL)
							{
								pPet->SetIsRetaliateUponNextShape(true);
								pPet->PetContAttRecved();
							}
						}
					}
					if (pMoveShape->GetType() == TYPE_PET)
					{
						CPet *pPet = (CPet*)pMoveShape;
						if (pPet->GetCurAttackShape() == NULL)
						{
							pPet->SetIsRetaliateUponNextShape(true);
							pPet->PetContAttRecved();
						}
					}
				}
				if(pMoveShape->GetState()==lState)
					break;

				WORD wAct = pMoveShape->GetAction();

				// �Ӻ�ƽ����ս��״̬
				if (lState==CShape::STATE_FIGHT)
				{
					if (wAct == CShape::ACT_IDLE)
						wAct = CShape::ACT_STAND;
				}
				// ��ս�������ƽ״̬
				else
				{
					// �ָ���վ��״̬
					if (wAct != CShape::ACT_WALK &&
						wAct != CShape::ACT_RUN &&
						wAct != CShape::ACT_DIED &&
						wAct != CShape::ACT_ALREADYDIED)
					{
						wAct = CShape::ACT_STAND;
					}
				}

				pMoveShape->SetState((WORD)lState);
				extern bool g_bForceUpdate;
				g_bForceUpdate = true;
				if (pMoveShape->GetType() == TYPE_PET)
				{
					CPet *pPet = (CPet*)pMoveShape;
					if (pPet->IsInBornState())
						return;
				}
				if( pMoveShape->GetState() != CShape::STATE_DIED)
				{
					pMoveShape->SetAction(wAct);
				}
			}
		}
		break;

		// �ı䷸��״̬
	case MSG_S2C_SHAPE_SINSTATE:
		{
			CGUID ID;
			pMsg->GetGUID(ID);
			BYTE bState = pMsg->GetByte();

			CPlayer* pPlayer = (CPlayer*)pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, ID);
			if (pPlayer)
			{
				pPlayer->SetSinState(bState!=0);
			}
		}
		break;

		// ǿ���ƶ�
	case MSG_S2C_SHAPE_FORCEMOVE:
		{
			CGUID ID;
			pMsg->GetGUID(ID);
			long lType = pMsg->GetLong();
			CMoveShape* pShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lType, ID);
			if (pShape)
			{
				float fDestX = pMsg->GetLong()+0.5f;
				float fDestY = pMsg->GetLong()+0.5f;

				DWORD dwCurTime = GetCurTickCount();
				if(pShape->GetAction()==CShape::ACT_RUN ||
					pShape->GetAction()==CShape::ACT_WALK)
				{
					if((long)(pShape->GetDestX()) != pShape->GetTileX() ||
						(long)(pShape->GetDestY()) != pShape->GetTileY() )
					{
						pShape->SetPosXY(pShape->GetDestX(),pShape->GetDestY());
					}
				}
				else
				{
					//������ڵĶ����������߶���,�����ϴεĸ���ʱ��
					pShape->SetLastUpdatePosTime(dwCurTime);
				}

				float fx =  pShape->GetPosX();
				float fy = pShape->GetPosY();

				float fRadianDir = pShape->GetLineDirF(fx,fy,fDestX,fDestY);
				//�����µķ���
				pShape->SetDirEx(fRadianDir);
				pShape->SetDestXY(fDestX,fDestY);
				//�ó��ƶ�ʱ��
				float fTotalDistance = pShape->RealDistance(fDestX,fDestY);
				DWORD dwMoveTime = DWORD(fTotalDistance/pShape->GetSpeed());
				pShape->SetEndMoveTime(dwCurTime+dwMoveTime);
				WORD action = CShape::ACT_WALK;
				if(pShape->GetIsRun())
					action = CShape::ACT_RUN;
				if(pShape->GetAction() != action)
					pShape->SetAction(action);
				pShape->SetInForceMove(true);
				if(pShape == GetGame()->GetMainPlayer())
				{
					//���ø���λ��
					GetGame()->GetGameControl()->SetUpdateXY(fDestX,fDestY);
				}
			}
		}
		break;

	case MSG_S2C_SHAPE_Refresh:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			WORD wAction = pMsg->GetWord();
			DWORD dwMaxHp = pMsg->GetDWord();
			DWORD dwHp = pMsg->GetDWord();
			CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, ID));
			if (pShape)
			{
				pShape->SetAction(wAction);
				//pShape->SetMaxHP(wMaxHp);
				pShape->SetHp(dwHp);
			}
		}
		break;

		// ����
	case MSG_S2C_SHAPE_EMOTION:
		{
			long lType=pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			long lIndex=pMsg->GetLong();
			CMoveShape *pMoveShape=(CMoveShape*)GetGame()->GetRegion()->FindChildObject(lType,guid);
			if(pMoveShape)
			{
				//���ű���
				char strInfo[256]="";
                _snprintf(strInfo,256, AppFrame::GetText("Base_114")    //%s��ʼ���鶯��%d
                    ,pMoveShape->GetName(),lIndex);
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_LEFT_BOTTOM, strInfo, 0xffffff00 );
			}
		}
		break;

		// ����
	case MSG_S2C_SHAPE_RELIVE:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);

			// �ж��Ƿ������,�����������λ��
			if (GetGame()->GetMainPlayer()->GetExID() == ID)
				GetGame()->GetRegion()->GetRegionCamera()->ResumeCamera();

			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, ID));
			if (pMoveShape)
			{
				WORD dwAction = pMsg->GetWord();
				WORD wState = pMsg->GetWord();

				if (pMoveShape->GetIsNeedAlphaFade())
				{
					GetGame()->GetRegion()->GetAlphaFadeObjectList().AddFadeOutObject((CShape *)pMoveShape);
					pMoveShape->SetIsAddAlphaFaded(true);
				}

				pMoveShape->SetState(wState);
				pMoveShape->SetAction(dwAction);
			}
		}
		break;
		// ���ù����ɼ�������񱣻���Ϣ
	case MSG_S2C_SHAPE_CHANGE_FIRSTATTACKER:
		{
			CGUID ShapID,CreatorID;
			long lShapType = 0,lCreatorType = 0;
			pMsg->GetGUID(ShapID);			// ���񱣻�����
			lShapType = pMsg->GetLong();
			pMsg->GetGUID(CreatorID);	// ������ID
			lCreatorType = pMsg->GetLong();
			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lShapType, ShapID));
			if (pMoveShape)
			{
				// ����
				if(pMoveShape->GetType() == TYPE_MONSTER)
				{
					((CMonster *)pMoveShape)->SetCreatorType(lCreatorType);
					((CMonster *)pMoveShape)->SetCreatorID(CreatorID);
					// ���Ҫɾ���Ķ����ǵ�ǰѡ���
					//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
					//if(pObjPage)
					//{
					//	if(TYPE_MONSTER == pObjPage->GetObjType() && ShapID == pObjPage->GetObjGUID())
					//		pObjPage->UpdataObjInfo();
					//}
				}
				// �ռ���
				else if(pMoveShape->GetType() == TYPE_COLLECTION)
				{
					((CCollectionGoods *)pMoveShape)->SetCreatorType(lCreatorType);
					((CCollectionGoods *)pMoveShape)->SetCreatorID(CreatorID);
				}
			}
		}
		break;
		// ƽ����λ
	case MSG_S2C_SHAPE_SLIPPOS:
		{
			long	  lType		  = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			float x = pMsg->GetFloat();
			float y = pMsg->GetFloat();
			float fSpeed = pMsg->GetFloat();
			float fDir = pMsg->GetFloat();
			CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(lType, ID));
			if( pShape )
			{
                float fx = pShape->GetPosX();
                float fy = pShape->GetPosY();
				
				DWORD dwCurTime = GetCurTickCount();
				pShape->SetIsSlipState(true);
				pShape->SetSlipSpeed(fSpeed);
				pShape->SetSlipDestPos(x,y);
				pShape->SetSlipLastTime(dwCurTime);
				pShape->SetSlipDir(fDir);
				//�ó��ƶ�ʱ��
				float fTotalDistance = pShape->RealDistance(x+0.5f,y+0.5f);
				DWORD dwMoveTime = DWORD(fTotalDistance/fSpeed);
				pShape->SetSlipEndMoveTime(dwCurTime+dwMoveTime);
				// ���֮ǰ���ƶ�Ŀ�������Ϣ
				pShape->SetDestXY(pShape->GetPosX(), pShape->GetPosY());
				pShape->SetEndMoveTime(dwCurTime);
                pShape->SetMoveOffset((x-fx)/dwMoveTime, (y-fy)/dwMoveTime);
			}
		}
		break;
		// ��Ѫ
	case MSG_S2C_SHAPE_ATK_ADDBLOOD:
		{
			// ��Ϣ˳��lActorType|lActorID|lTargType|lTargetID|cSize|cLostType|lLostPoint|lFinalHP|bBlastAttack
			CGUID ActorID,TargetID;
			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorID);
			long lTargType = pMsg->GetLong();
			pMsg->GetGUID(TargetID);
			CMoveShape* pTargetShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(lTargType, TargetID);
			if( pTargetShape)
			{
				if(pTargetShape->IsDied())
					break;
				// ����һ���������¼�
				tagAttackEvent st;
				st.cType = AE_CURE;								// ��Ѫ
				st.Guidattracker = ActorID;
				st.lTotalLostPoint = pMsg->GetLong();			// ��Ѫ����
				pTargetShape->GetAttackEventQueue()->push(st);
			}
		}
		break;
		// ���ѡ��Ŀ��
	case MSG_S2C_SHAPE_GETTSELECTEDSHAPE:
		{
			long type = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			CMoveShape* pMoveShape = (CMoveShape*)pMsg->GetRegion()->FindChildObject(type, guid);
			if(pMoveShape && GetGame()->GetGameControl())
			{
				GetGame()->GetGameControl()->SetSelectedShape(pMoveShape,false);
			}
		}
		break;
	}
}
