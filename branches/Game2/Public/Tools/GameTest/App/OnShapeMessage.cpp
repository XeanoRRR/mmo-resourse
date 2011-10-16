


#include "stdafx.h"
#include "GameClient.h"
#include "RgnObjMgr.h"
#include "..\Script\ScriptSys.h"
#include "..\GameTest\GameTestDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CGameClient::OnShapeMessage(CMessage* pMsg)
{
	long lSocketID = pMsg->GetSocketID();

	switch( pMsg->GetType() )
	{
	case MSG_S2C_SHAPE_CHANGEDIR:
		{
			BYTE bDir = pMsg->GetByte();
			float fDir = pMsg->GetFloat();
			long lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);

			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lType,guid));
			if( pMoveShape )
			{
				if( pMoveShape->GetAction() == CShape::ACT_WALK || 
					pMoveShape->GetAction() == CShape::ACT_RUN )
				{
					pMoveShape->SetPosXY(pMoveShape->GetDestX(),pMoveShape->GetDestY());
					pMoveShape->SetAction(CShape::ACT_STAND);
				}
				pMoveShape->SetDirEx(fDir);
			}
		}
		break;//MSG_S2C_SHAPE_CHANGEDIR

	case MSG_S2C_SHAPE_CHANGE_PROPERTIES:
		{
			long lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);

			CShape* pShape = dynamic_cast<CShape*>(m_pClientObjMgr->FindObject(lType,guid));
			if( lType == TYPE_MONSTER )
			{
				CMonster* pMonster = dynamic_cast<CMonster*>(pShape);
				if( pMonster )
				{
					pMonster->UpdateProperty(pMsg);
				}
			}
			else if( lType == TYPE_PET && pShape )
			{
				//CPet* pPet = dynamic_cast<CPet*>(pShape);
				//pPet->UpdateProperty(pMsg);
			}
			else if( lType == TYPE_PLAYER )
			{
				((CPlayer*)pShape)->UpdateProperty(pMsg);
			}
		}
		break;//MSG_S2C_SHAPE_CHANGE_PROPERTIES

	case MSG_S2C_SHAPE_SETPOS:
		{
   			long lType = pMsg->GetLong();
			CGUID guid;
			pMsg->GetGUID(guid);
			float x = pMsg->GetFloat();
			float y = pMsg->GetFloat();
			long lUseGoods = pMsg->GetLong();

			CShape* pShape = dynamic_cast<CShape*>(m_pClientObjMgr->FindObject(lType,guid));
			if ( pShape )
			{
				if( pShape->GetType() == TYPE_PET )
				{
					//CPet* pPet = dynamic_cast<CPet*>(pShape);
					//if( pPet && pPet->GetMaster() == m_pMainPlayer )
					//{
					//	//	���Ѱ··��
					//	//						pPet->cl
					//}
				}

				if( pShape->GetTileX() != (long)x || pShape->GetTileY() != (long)y )
					pShape->SetPosXY( x, y );

				CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pShape);
				if( pMoveShape )
                {
					pMoveShape->SetDestXY( pShape->GetPosX(), pShape->GetPosY() );
                    pMoveShape->StopMove();
                }
			}
		}
		break;//MSG_S2C_SHAPE_SETPOS

	case MSG_S2C_SHAPE_MOVE:
		{
// 			CGUID guid;
// 			pMsg->GetGUID(guid);
// 
// 			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(guid));
// 			if( pMoveShape )
// 			{
// 				long lType = pMoveShape->GetType();
//				if( pMoveShape != m_pMainPlayer )
// 				{
// 					if( lType == TYPE_PET )
// 					{
// 						//CPet* pPet = dynamic_cast<CPet*>(pMoveShape);
// 						//if( pPet && pPet->GetMaster() == m_pMainPlayer )
// 						//	return;
// 					}
// 
// 					float fDestX = pMsg->GetShort() + 0.3f + (float)(random(5)/10);
// 					float fDestY = pMsg->GetShort() + 0.3f + (float)(random(5)/10);
// 
// 					if( abs(pMoveShape->GetTileX() - fDestX) >= 5 ||
// 						abs(pMoveShape->GetTileY() - fDestY) >= 5)
// 					{
// 						pMoveShape->SetPosXY(fDestX,fDestY);
// 						pMoveShape->SetDestXY(fDestX,fDestY);
// 						pMoveShape->SetAction(CShape::ACT_STAND);
// 						break;
// 					}
// 
// 					WORD wAction = CShape::ACT_WALK;
// 					if( pMoveShape->GetAction() != CShape::ACT_WALK )
// 						wAction = CShape::ACT_RUN;
// 					if( pMoveShape->GetAction() != wAction )
// 						pMoveShape->SetAction(wAction);
// 
// 					pMoveShape->SetDestXY(fDestX,fDestY);
// 				}
// 			}


			CGUID guid;
			pMsg->GetGUID(guid);

			CMoveShape* pShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(guid));

			//CMoveShape* pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PLAYER, guid));
			//if(!pShape)
			//{
			//	pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_MONSTER, guid));
			//	if(!pShape)
			//		pShape = dynamic_cast<CMoveShape*>(pMsg->GetRegion()->FindChildObject(TYPE_PET, guid));
			//}
			if(pShape)
			{
				long lType =pShape->GetType();
				//�����Լ�
				if(pShape != m_pMainPlayer)
				{
					if (lType== TYPE_PET)
					{
						//CPet *pPet =(CPet*)pShape;
						//if (pPet->IsMainPlayerPet())
						//{
						//	return;
						//}
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
						pShape->SetAction(CShape::ACT_STAND);
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
						if(abs(pShape->GetTileX() - fDestX) >=5 ||
						abs(pShape->GetTileY() - fDestY) >= 5 )
						{
						pShape->SetPosXY(fDestX,fDestY);
						pShape->SetDestXY(fDestX,fDestY);
						pShape->SetAction(CShape::ACT_STAND);
						break;
						}

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
// 					long lCount = GetGame()->GetGameControl()->GetSendedMoveMsgCount();
// 					if(lCount > 0)
// 						GetGame()->GetGameControl()->SetSendedMoveMsgCount(lCount-1);
				}
			}
		}
		break;//MSG_S2C_SHAPE_MOVE

	case MSG_S2C_SHAPE_STOPMOVE:
		{	
			//CGUID guid;
			//pMsg->GetGUID(guid);
			//CMoveShape* pShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(guid));
			//if( !pShape || pShape == m_pMainPlayer )
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
			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(shapeGuid));
			if(pMoveShape)
			{
				bool bOldRun = pMoveShape->GetIsRun();
				pMoveShape->SetIsRun(bRun==0?false:true);
				//���������ٶ�
				pMoveShape->SetSpeed(fSpeed);
				pMoveShape->SetActionSpeedRate(CShape::ACT_WALK,fRatio);
				pMoveShape->SetActionSpeedRate(CShape::ACT_RUN,fRatio);

				extern bool g_bForceUpdate;
				g_bForceUpdate = true;

				pMoveShape->SetAction(pMoveShape->GetAction());

				if(pMoveShape == m_pMainPlayer)
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
	case MSG_S2C_SHAPE_ATK_FULLMISS:
	case MSG_S2C_SHAPE_ATK_BREAK:
	case MSG_S2C_SHAPE_ATK_PARRY:
	case MSG_S2C_SHAPE_ATK_BLOCK:
	case MSG_S2C_SHAPE_ATK_RESIST:
	case MSG_S2C_SHAPE_ATK_REBOUND:
	case MSG_S2C_SHAPE_ATK_SUCK:
	case MSG_S2C_SHAPE_ATK_LOSE:
	case MSG_S2C_SHAPE_CHANGESTATE:
	case MSG_S2C_SHAPE_SINSTATE:
		break;//MSG_S2C_SHAPE_ATK_BREAK

	case MSG_S2C_SHAPE_ATK_DIED:
		{
			CGUID ActorGuid,TargetGuid;

			long lActorType = pMsg->GetLong();
			pMsg->GetGUID(ActorGuid);

			long lTargetType = pMsg->GetLong();
			pMsg->GetGUID(TargetGuid);

			CMoveShape* pTargetShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lTargetType,TargetGuid));
			if(pTargetShape)
			{
				//	������Լ����򸴻�
				if( pTargetShape == m_pMainPlayer )
				{
// 					CBaseMessage* msg = CBaseMessage::AllocMsg();
// 					msg->Reset();
// 					msg->Init(MSG_C2S_OTHER_TALK);
// 					CMessage* pMsg = (CMessage*)msg;
// 
// 					pMsg->Add((BYTE)0);
// 					pMsg->Add((long)RUN_SCRIPT);
// 					pMsg->Add((long)TYPE_PLAYER);
// 					pMsg->Add(TargetGuid);
// 					pMsg->Add((long)0);
// 					pMsg->Add("ReLive(1);");
// 					pMsg->SendBySocket(lSocketID);
// 					CBaseMessage::FreeMsg(pMsg);
				}

				//	����ǹ����ɾ��
				else if(pTargetShape->GetType() == TYPE_MONSTER)
				{	
					pTargetShape->SetState(CShape::STATE_DIED);
					this->RemoveObjectFromRgn(pTargetShape);
					if( pTargetShape->GetUsedTimes() == 0 )
					{
						SAFE_DELETE(pTargetShape);
					}
					m_lKilMotrNum++;
// 					CString info("");
// 					info.Format("��� %s ɱ�� %d",m_pMainPlayer->GetName(),m_lKilMotrNum);
// 					CScriptSys::getInstance()->GetTestDialog()->OutputScriptInfo(info);
				}
				//	�������
				else if( pTargetShape->GetType() == TYPE_PLAYER )
				{
					pTargetShape->SetState(CShape::STATE_DIED);
				}
			}

		}
		break;//MSG_S2C_SHAPE_ATK_DIED

	case MSG_S2C_SHAPE_FORCEMOVE:
		{
			CGUID ID;
			pMsg->GetGUID(ID);
			long lType = pMsg->GetLong();
			CMoveShape* pShape = (CMoveShape*)m_pClientObjMgr->FindObject(lType, ID);
			if (pShape)
			{
				float fDestX = pMsg->GetLong()+0.5f;
				float fDestY = pMsg->GetLong()+0.5f;

				if(pShape->GetAction()==CShape::ACT_RUN ||
					pShape->GetAction()==CShape::ACT_WALK)
				{
					if((long)(pShape->GetDestX()) != pShape->GetTileX() ||
						(long)(pShape->GetDestY()) != pShape->GetTileY() )
					{
						pShape->SetPosXY(pShape->GetDestX(),pShape->GetDestY());
					}
				}

				float fx =  pShape->GetPosX();
				float fy = pShape->GetPosY();

				pShape->SetDestXY(fDestX,fDestY);

				WORD action = CShape::ACT_WALK;
				if(pShape->GetAction() != CShape::ACT_WALK)
					action = CShape::ACT_RUN;
				if(pShape->GetAction() != action)
					pShape->SetAction(action);
			}
		}
		break;//MSG_S2C_SHAPE_FORCEMOVE
	case MSG_S2C_SHAPE_Refresh:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			WORD wAction = pMsg->GetWord();
			DWORD dwMaxHp = pMsg->GetDWord();
			DWORD dwHp = pMsg->GetDWord();
			CMoveShape* pShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lType, ID));
			if (pShape)
			{
				pShape->SetAction(wAction);
				pShape->SetHp(dwHp);
			}
		}
		break;//MSG_S2C_SHAPE_Refresh
		// ����
	case MSG_S2C_SHAPE_RELIVE:
		{
			long lType = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);

			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lType, ID));
			if (pMoveShape)
			{
				WORD dwAction = pMsg->GetWord();
				WORD wState = pMsg->GetWord();

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
			CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lShapType, ShapID));
			if (pMoveShape)
			{
				// ����
				if(pMoveShape->GetType() == TYPE_MONSTER)
				{
					((CMonster *)pMoveShape)->SetCreatorType(lCreatorType);
					((CMonster *)pMoveShape)->SetCreatorGuid(CreatorID);
				}
				// �ռ���
				else if(pMoveShape->GetType() == TYPE_COLLECTION)
				{
					((CCollectionGoods *)pMoveShape)->SetCreatorType(lCreatorType);
					((CCollectionGoods *)pMoveShape)->SetCreatorID(CreatorID);
				}
			}
		}
		break;//MSG_S2C_SHAPE_CHANGE_FIRSTATTACKER

		// ƽ����λ
	case MSG_S2C_SHAPE_SLIPPOS:
		{
			long	  lType		  = pMsg->GetLong();
			CGUID ID;
			pMsg->GetGUID(ID);
			long x = pMsg->GetLong();
			long y = pMsg->GetLong();
			float fSpeed = pMsg->GetFloat();
			float fDir = pMsg->GetFloat();
			CMoveShape* pShape = dynamic_cast<CMoveShape*>(m_pClientObjMgr->FindObject(lType, ID));
			if( pShape )
			{
				pShape->SetDestXY(pShape->GetPosX(), pShape->GetPosY());
			}
		}
		break;//MSG_S2C_SHAPE_SLIPPOS
	}
}