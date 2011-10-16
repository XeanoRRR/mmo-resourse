
#include "StdAfx.h"
#include "GameClient.h"
#include "RgnObjMgr.h"
#include "GoodsMgr.h"
#include "..\Script\ScriptSys.h"
#include "..\GameTest\GameTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CGameClient::OnContainerMsg(CMessage* msg)
{
	switch( msg->GetType() )
	{
	case MSG_S2C_CONTAINER_OBJECT_MOVE:
		{
			BYTE oType = msg->GetByte();

			if( oType == OT_ROLL_BACK )
				break;

			CGUID SourceObjectGuid,		AimObjectGuid;
			CGUID SourceContainerGuid,	AimContainerGuid;

			//Դ����
			long lSourceContainerType = msg->GetLong();
			msg->GetGUID(SourceContainerGuid);
			long  lSourceContainerExtendID = msg->GetLong();
			DWORD dwSourceContainerPos = msg->GetDWord();

			//Ŀ������
			long lAimContainerType = msg->GetLong();
			msg->GetGUID(AimContainerGuid);
			long  lAimContainerExtendID = msg->GetLong();
			DWORD dwAimContainerPos = msg->GetDWord();

			//Դ����
			long lSourceObjectType = msg->GetLong();
			msg->GetGUID(SourceObjectGuid);

			//Ŀ�����
			long lAimObjectType = msg->GetLong();
			msg->GetGUID(AimObjectGuid);
			//Դ��������
			DWORD dwSourceAmount = msg->GetDWord();

			//�¼���Ʒ 2
			if (oType == OT_NEW_OBJECT)
			{
				//��ȡ��Ʒ���л�����
				DWORD dwSize = msg->GetDWord();
				if( dwSize==0)
					break;

				BYTE* pBuf = new BYTE[dwSize];
				msg->GetEx(pBuf,dwSize);

				CGoods* pGoods = new CGoods;
				long lPos = 0;

				if ( pGoods )
				{
					pGoods->DecordFromByteArray(pBuf,lPos);
				}
				SAFE_DELETE_ARRAY(pBuf);

				//��ҵõ���Ʒ 540
				if( lAimContainerType==TYPE_PLAYER)
				{
					//����Ŀ�����
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(TYPE_PLAYER,AimContainerGuid));
					if( pPlayer == NULL) 
					{
						SAFE_DELETE(pGoods);
						break;
					}
					pPlayer->GetGoodsMgr()->AddGoods(pGoods,dwAimContainerPos,lAimContainerExtendID);

					// ����Ʒ�ǽ�һ�������ʱ���ڼӵ�������Ϻ��Ҫ����ɾ���������ڴ�й©
					if( lAimContainerExtendID == PEI_WALLET || lAimContainerExtendID == PEI_SILVERWALLET )
					{
						SAFE_DELETE(pGoods);
					}
				}
				//�����õ���Ʒ 54
				else if( lAimContainerType == TYPE_REGION)
				{
					AddGoods(pGoods,dwAimContainerPos,lAimContainerExtendID);
				}
				//�Ự��Ʒ
				else if( lAimContainerType == TYPE_SESSION )
				{
					//����
//  					if( AimContainerGuid == m_pMainPlayer->GetShopMgr()->GetSessionID() )
//  					{
//  						if( lSourceContainerExtendID != PEI_SILVERWALLET && lSourceContainerExtendID != PEI_WALLET)
//  						{
//  							if(SourceContainerGuid == m_pMainPlayer->GetExID())
//  							{
//  								m_pMainPlayer->GetShopMgr()->AddMyTradeGoods(pGoods,(long)dwAimContainerPos,msg->GetSocketID());
//  							}
//  							else
//  							{
//  								m_pMainPlayer->GetShopMgr()->AddYouTradeGoods(pGoods,(long)dwAimContainerPos);
//  							}
//  						}
//  						break;
//  					}
//  					//��Ƕ��Ƭ
//  					else if( AimContainerGuid == GetEnchaseSessionGuid() )
//  					{
//  						if( dwAimContainerPos == 0 )
//  						{
//  							CGUID guid = pGoods->GetExID();
//  							CGoods* pPlayerGoods = m_pMainPlayer->GetGoodsByGUID(guid);
//  							if( pPlayerGoods != NULL )
//  							{
//  								SetEnchaseGoods(pPlayerGoods);
//  							}
//  							SAFE_DELETE(pGoods);
//  						}
//  						else if( dwAimContainerPos == 1)
//  						{
//  							CGUID guid = pGoods->GetExID();
//  							CGoods* pPlayerGoods = m_pMainPlayer->GetGoodsByGUID(guid);
//  							if( pPlayerGoods != NULL )
//  							{
//  								SetEnchaseBaoShi(pPlayerGoods);
//  							}
//  							SAFE_DELETE(pGoods);
//  						}
//  						else if( dwAimContainerPos <= 8 && dwAimContainerPos >= 4 )
//  						{
//  							BYTE nIndex = (BYTE)(dwAimContainerPos - 4);
//  							SetEnchaseCard(pGoods,nIndex);
//  							SetEnchaseAimConPosForMsg(dwAimContainerPos);
//  							SAFE_DELETE(pGoods);
//  						}
//  					}
				}
				else
				{
					if( pGoods->GetUsedTimes() == 0 )
						SAFE_DELETE(pGoods);
				}
			}
			//ɾ����Ʒ 3
			else if( oType == OT_DELETE_OBJECT )
			{
				//����
				DWORD dwAimAmount = msg->GetDWord();
				CGoods* pGoods = NULL;

				//ɾ�������Ʒ
				if(lSourceContainerType==TYPE_PLAYER)
				{
					//�������
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(TYPE_PLAYER,SourceContainerGuid));
					if( pPlayer == NULL )
					{
						break;
					}

					pGoods = pPlayer->GetGoodsMgr()->RemoveGoods(SourceObjectGuid,AimObjectGuid,dwSourceContainerPos,lSourceContainerExtendID,dwSourceAmount);
				}
				//ɾ��������Ʒ
				else if ( lSourceContainerType == TYPE_REGION)
				{
 					pGoods = RemoveGoods(SourceObjectGuid,dwSourceContainerPos,lSourceContainerExtendID,dwSourceAmount);
				}

				if( pGoods && pGoods->GetUsedTimes() == 0 )
				{
					SAFE_DELETE(pGoods);
				}
			}
			//�ƶ���Ʒ 1
			else if( oType==OT_MOVE_OBJECT )
			{
				//�ƶ�����
				DWORD dwAimAmount =msg->GetDWord();
				CGoods* pGoods = NULL;

				//�������������Ʒ
				if( lSourceContainerType == TYPE_PLAYER)
				{
					//�������
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(TYPE_PLAYER,SourceContainerGuid));
					if( pPlayer != m_pMainPlayer )
					{
						break;
					}

					pGoods = pPlayer->GetGoodsMgr()->RemoveGoods(SourceObjectGuid,AimObjectGuid,dwSourceContainerPos,lSourceContainerExtendID,dwSourceAmount);
				}
				// �ӵ��ϼ���
				else if(lSourceContainerType == TYPE_REGION)
				{
					pGoods = RemoveGoods(SourceObjectGuid,dwSourceContainerPos,lSourceContainerExtendID,dwSourceAmount);
				}

				if( pGoods == NULL)
				{
					break;
				}

				if(AimObjectGuid != CGUID::GUID_INVALID)
				{
					pGoods->SetExID(AimObjectGuid);
				}

				//�ƶ���������
				if( lAimContainerType == TYPE_PLAYER)
				{
					CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(TYPE_PLAYER,AimContainerGuid));
					if(pPlayer != m_pMainPlayer) 
					{
						if( pGoods->GetUsedTimes() == 0 )
							SAFE_DELETE(pGoods);
						break;
					}
					//�ӵ��ϻ�ý�Ǯ
					if( (lAimContainerExtendID == PEI_WALLET || lAimContainerExtendID == PEI_SILVERWALLET) && lSourceContainerType == TYPE_REGION )
					{
						pGoods->SetNum( dwSourceAmount);
					}

					m_pMainPlayer->GetGoodsMgr()->AddGoods(pGoods,dwAimContainerPos,lAimContainerExtendID);


					if(lAimContainerExtendID == PEI_WALLET || lAimContainerExtendID == PEI_SILVERWALLET )
					{
						if( pGoods->GetUsedTimes() == 0 )
							SAFE_DELETE(pGoods);
					}
				}
				//�ƶ�������
				else if(lAimContainerType == TYPE_REGION)
				{
					AddGoods(pGoods,dwAimContainerPos,lAimContainerExtendID);
				}
				else
				{
					if( pGoods->GetUsedTimes() == 0 )
						SAFE_DELETE(pGoods);
				}
			}
			// ����������Ʒ(�ӱ����滻ʱ)
			else if(oType == OT_SWITCH_FILL)
			{	
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(lSourceContainerType,SourceContainerGuid));
				if( pPlayer != m_pMainPlayer ) 
					break;
				int iPos = 0;

				// �����Ӳֿ�
				if( lAimContainerExtendID == eDCT_Secondary )
				{
					dwAimContainerPos = dwAimContainerPos - 10000 + eDCT_Secondary;
					if( dwAimContainerPos >= eDCT_Secondary1 &&
						dwAimContainerPos <	 eDCT_Secondary1 + DEPOT_SUBPACK_MAXNUM )
					{
// 						tagDepotPackData* depotPackData = m_pMainPlayer->GetDepotInfo( (eDEPOT_PACK_ID)(lAimContainerExtendID - (long)eDCT_Primary));
// 						DWORD dwNum = msg->GetDWord();
// 						for(long i = 0;i<dwNum;i++)
// 						{
// 							CGUID guid;
// 							msg->GetGUID(guid);
// 							hash_map<CGUID,tagGoods,hash_guid_compare>::iterator it = depotPackData->mapGoodsList.begin();
// 							for(;it!=depotPackData->mapGoodsList.end();it++)
// 							{
// 								it->second.lPos = msg->GetWord();
// 							}
// 						}
					}
				}
				// �����ӱ���
				else
				{
					DWORD dwSize = msg->GetDWord();
					if( dwSize == 0 ) 
						break;

					CGoods* pGoods = NULL;
					tagGoods** ptagGoods = new tagGoods*[dwSize];
					for(int k=0;k<dwSize;k++)
					{
						ptagGoods[k] = NULL;
					}
					WORD *wNewPos = new WORD[dwSize];

					for( int j = 0;j<dwSize;j++)
					{
						CGUID guid;
						msg->GetGUID(guid);

						wNewPos[j] = msg->GetWord();

//						pGoods = pPlayer->GetGoodsMgr()->QueryGoodsByGuid(guid,&iPos);

						if( !pGoods )
							break;

//						if( lAimContainerExtendID == PEI_BUSSINESPACKFIELD )
//							ptagGoods[j] = pPlayer->GetBusinessGoods((WORD)iPos);
//						else
//							ptagGoods[j] = pPlayer->GetSubGoods(dwAimContainerPos - PEI_PACK1,(WORD)iPos);

						if( !ptagGoods )
							break;
					}

					for( int i = 0; i <dwSize ; i++)
					{
						if( ptagGoods[i] != NULL )
						{
							ptagGoods[i]->lPos = (BYTE)wNewPos[i];
						}
					}

					SAFE_DELETE(ptagGoods);
					SAFE_DELETE_ARRAY(wNewPos);
				}

				if( lSourceContainerType != TYPE_PLAYER )	
					break;

				int sPos = 0;
				int aPos = 0;

//				CGoods* pGoods1 = pPlayer->GetGoodsByGUID(SourceObjectGuid,&sPos);
//				CGoods* pGoods2 = pPlayer->GetGoodsByGUID(AimObjectGuid,&aPos);

//				if( pGoods2 != NULL && pGoods1 != NULL )
				{
//					pPlayer->RemoveGoods(AimObjectGuid,AimObjectGuid,aPos,lAimContainerExtendID,pGoods2->GetNum());
//					pPlayer->RemoveGoods(SourceObjectGuid,SourceObjectGuid,sPos,lSourceContainerExtendID,pGoods1->GetNum());
//					pPlayer->AddGoods(pGoods1,aPos,lAimContainerExtendID);
//					pPlayer->AddGoods(pGoods2,sPos,lSourceContainerExtendID);

					CCDKey* pCDKey = CCDKey::GetCDKey(m_strCDkey.c_str());
					if(pCDKey)
					{
//						pCDKey->AddPlayerData(pPlayer,CCDKey::eSAVE_GOODS);
					}
				}
			}
			//j������Ʒ
			else if( oType == OT_SWITCH_OBJECT)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pClientObjMgr->FindObject(lSourceContainerType,SourceContainerGuid));
				if( pPlayer!= m_pMainPlayer) 
				{
					break;
				}

				if( lSourceContainerType != TYPE_PLAYER )	
				{
					break;
				}

				int iSourPos = -1;
				int iAimPos = -1;
				long lSorCon = -1;
				long lAimCon = -1;


				CGoods* pGoods1 = NULL;
				CGoods* pGoods2 = NULL;
				if( lSourceContainerExtendID == PEI_MEDAL)
				{
					//AfxMessageBox("����ѫ��",MB_OK,NULL);
				}
				else
				{
//					/*CGoods* */pGoods1 = pPlayer->GetGoodsByGUID(SourceObjectGuid,&iSourPos,&lSorCon);
//					if( pGoods1 == NULL)
//					{
//						//						OutputContainerInfo("Get Source Goods Failed");
//					}

//					/*CGoods* */pGoods2 = pPlayer->GetGoodsByGUID(AimObjectGuid,&iAimPos,&lAimCon);
//					if( pGoods2 == NULL)
//					{
//						//						OutputContainerInfo("Get Aim Goods Failed");
//					}
				}

				if( pGoods2 != NULL && pGoods1 != NULL )
				{
//					CGoods* pTemp1 = pPlayer->RemoveGoods(AimObjectGuid,AimObjectGuid,iAimPos,lAimContainerExtendID,pGoods2->GetNum());
//					if( pTemp1 == NULL )
//					{
//						//						OutputContainerInfo("Remove Aim Goods Failed");
//					}
//					CGoods* pTemp2 = pPlayer->RemoveGoods(SourceObjectGuid,SourceObjectGuid,iSourPos,lSourceContainerExtendID,pGoods1->GetNum());
//					if( pTemp2 == NULL )
//					{
//						//						OutputContainerInfo("Remove Source Goods Failed");
//					}

					pPlayer->GetGoodsMgr()->AddGoods(pGoods1,dwAimContainerPos,lAimContainerExtendID);
					pPlayer->GetGoodsMgr()->AddGoods(pGoods2,dwSourceContainerPos,lSourceContainerExtendID);		
				}
			}
		}
		break;

		//	������֪ͨ�ͻ�����Ʒ�¼�����Ʒ�������ı�
	case MSG_S2C_CONTAINER_OBJECT_AMOUNT_CHANGE:
		{
			long	lSourceContainerType = msg->GetLong();
			CGUID	SourceContainerGuid;
			msg ->GetGUID(SourceContainerGuid);
			long	lSourceContainerExtendID = msg->GetLong();
			DWORD	dwSourceContainerPos = msg->GetDWord();
			long	lObjectType = msg->GetLong();
			CGUID	guID;
			msg->GetGUID(guID);
			DWORD	dwAmount = msg->GetDWord();

			CGoods* pGoods = NULL;
//			pGoods = m_pMainPlayer->GetGoodsByGUID(guID);
			if( pGoods == NULL || lSourceContainerExtendID != PEI_WALLET && lSourceContainerExtendID != PEI_SILVERWALLET )
				return;
			//
			if( lSourceContainerType == TYPE_PLAYER && SourceContainerGuid == m_pMainPlayer->GetExID())
			{
				DWORD dwOldNum = 0;
				if( lSourceContainerExtendID != PEI_WALLET && lSourceContainerExtendID != PEI_SILVERWALLET && lSourceContainerExtendID != PEI_YUANBAO )
				{
					dwOldNum = pGoods->GetNum();
				}
//				m_pMainPlayer->ChangeObjectAmount(lSourceContainerExtendID,lObjectType,guID,dwAmount);
			}
		}
		break;
	default:
		break;
	}
}
