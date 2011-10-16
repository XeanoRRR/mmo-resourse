#include "stdafx.h"
#include "..\nets\netserver\message.h"
#include "..\nets\Servers.h"

#include "..\Session\CSession.h"
#include "..\Session\CSessionFactory.h"

#include "..\player.h"
#include "..\public\BounsDef.h"
#include "..\container\CTestContainer.h"
#include "..\Goods\CGoods.h"
#include "..\Goods\CGoodsFactory.h"
#include "..\message packaging\CS2CContainerObjectAmountChange.h"

#include "..\..\..\setup\globesetup.h"



// ����ʵʩ����ʧ�ܣ��ع�״̬���������״̬
void SendBounsRollBackMsg(const CGUID& guid)
{
	CMessage msg(MSG_S2BDS_BOUNS_ROLLBACK);
	msg.Add(guid);
	msg.SendToBounsServer();
}
void OnBounsMessage(CMessage* pMsg)
{
	if(pMsg == NULL)
	{
		return;
	}

	switch(pMsg->GetType())
	{
		//////////////////////////////////////////////////////////////////////////
		// BounsServer --> GameServer							//////////////////
		//////////////////////////////////////////////////////////////////////////
	case MSG_BDS2S_BOUNS_DES_UPDATE:
		{
			BYTE bType = pMsg->GetByte();
			DBReadSet setReadDB;
			if(bType == 1)	//ȫ������
			{				
				pMsg->GetDBReadSet(setReadDB);
				long lSize = setReadDB.GetLongFromByteArray();
				for(long l=0; l<lSize; l++)
				{
					tagBounsDes *pDB = OBJ_CREATE(tagBounsDes);
					pDB->DecodeFromDataBlock(setReadDB);
					GetGame()->AddBounsDes(pDB);
				}
			}
			else if(bType == 2) //��������
			{
				pMsg->GetDBReadSet(setReadDB);
				tagBounsDes *pDB = OBJ_CREATE(tagBounsDes);
				pDB->DecodeFromDataBlock(setReadDB);
				GetGame()->AddBounsDes(pDB);
			}
		}
		break;

	case MSG_BDS2S_BOUNS_LOAD_RESPOND:
		{
			CGUID guidPlayer;
			DBReadSet setReadDB;
			DBWriteSet setWriteDB;
			pMsg->GetGUID(guidPlayer);
			list<tagBounsInfo> listBounsInfo;
			long lSize = pMsg->GetLong();
			for(long l=0; l<lSize; l++)
			{
				pMsg->GetDBReadSet(setReadDB);
				tagBounsInfo tgBInfo;
				tgBInfo.DecodeFromDataBlock(setReadDB);
				tagBounsDes *pBD = GetGame()->FindBounsDes(tgBInfo.dwBounsID);
				if(pBD)
				{
					listBounsInfo.push_back(tgBInfo);
				}				
			}
			CPlayer *pPlayer = GetGame()->FindPlayer(guidPlayer);
			if(pPlayer == NULL)
				return;	


				pPlayer->SetCurrentProgress(CPlayer::PROGRESS_BOUNS);
			if(pPlayer->CanMove())
			{
				pPlayer->SetMoveable(FALSE);
			}

			// ����������Ϣ�����			
			CMessage msg(MSG_S2C_BOUNS_INFO_RESPOND);
			msg.Add((long)listBounsInfo.size());
			msg.GetDBWriteSet(setWriteDB);
			list<tagBounsInfo>::iterator itor = listBounsInfo.begin();
			for(; itor != listBounsInfo.end(); ++itor)
			{
				(*itor).EncodeToDataBlock(setWriteDB);
				tagBounsDes *pBD = GetGame()->FindBounsDes((*itor).dwBounsID);
				if(pBD)
				{
					pBD->EncodeToDataBlock(setWriteDB);
				}
			}
			msg.SendToPlayer(guidPlayer);
						
		}
		break;

	case MSG_BDS2S_BOUNS_GET_RESPOND:
		{
			long lRet = pMsg->GetLong();
			CGUID guidPlayer;
			char szPlayerIP[128];
			pMsg->GetGUID(guidPlayer);
			pMsg->GetStr(szPlayerIP, 128);
			DBReadSet setReadDB;
			pMsg->GetDBReadSet(setReadDB);
			tagBounsInfo tgBInfo;
			tgBInfo.DecodeFromDataBlock(setReadDB);
			
			CPlayer *pPlayer = GetGame()->FindPlayer(guidPlayer);
			if(pPlayer == NULL)
			{
				// BDS�����ɹ��ģ���Ҫ״̬����
				if(lRet == 1)
				{
					SendBounsRollBackMsg(tgBInfo.guid);
				}
				return;
			}

			tagBounsDes *pBD = GetGame()->FindBounsDes(tgBInfo.dwBounsID);
			if(pBD == NULL)
			{
				if(lRet == 1)
				{
					SendBounsRollBackMsg(tgBInfo.guid);
				}
				return;
			}

			if(lRet == 1)
			{
                if(tgBInfo.bType == 1)
				{				
				//	DWORD dwGoodsIndex = CGoodsFactory::QueryGoodsIDByOriginalName(pBD->strName.c_str());
					DWORD dwGoodsIndex = atoi(tgBInfo.strName.c_str());
					if(dwGoodsIndex == 0)
					{
						// �Ƿ���Ʒ�������϶�ʧ��
						SendBounsRollBackMsg(tgBInfo.guid);
						return;
					}
					vector<CGoods*> vOut;
					CGoodsFactory::CreateGoods(dwGoodsIndex, tgBInfo.dwValue, vOut, 32);
					if(vOut.size() == 0)
					{
						// ��ƷIndex����Ч��
						SendBounsRollBackMsg(tgBInfo.guid);
						AddLogText("ERROR: BounsID=%d Invalid GoodsIndex='%s'!!!",pBD->dwBounsID,tgBInfo.strName.c_str());
						return;
					}

					for(int i=0; i<vOut.size(); i++)
					{
						CTestContainer testContainer;
						testContainer.Init(pPlayer);
						vector<tagTestResult> vecResult;
						BOOL bSuccess = testContainer.AddTest(vOut[i], &vecResult);
						if(bSuccess)
						{
							CVolumeLimitGoodsContainer* pVolContainer=dynamic_cast<CVolumeLimitGoodsContainer*>(pPlayer->FindContainer(vecResult[0].dwContainerId));
							if(pVolContainer)
							{
								DWORD dwGoodsIndex = vOut[i]->GetBasePropertiesIndex();
								const char* strGoodsName = CGoodsFactory::QueryGoodsName(dwGoodsIndex);
								CGUID goodsGuid = vOut[i]->GetExID();
								const char* testGoodsName = vOut[i]->GetName();
								long testGoodsAount = vOut[i]->GetAmount();

								char szOutStr[1024]="";
								if(vecResult[0].eOperType == CS2CContainerObjectMove::OT_NEW_OBJECT)
								{
									//�������Ʒ							
									CS2CContainerObjectMove comMsg;
									if(pVolContainer->Add(vecResult[0].dwPos, vOut[i], NULL, &comMsg) )
									{
										comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
										comMsg.SetDestinationContainerExtendID( vecResult[0].dwContainerId );
										vOut[i]->SerializeForOldClient( &comMsg.GetObjectStream() );
										comMsg.Send( pPlayer->GetExID() );
										if( 0 == vOut[i]->GetAmount())
										{						
											_snprintf(szOutStr,1024,"NEW_GOODS:goodname:%s:%s",testGoodsName,pPlayer->GetName());
											PutStringToFile("BounsGoods",szOutStr);
										}

									}
								}
								else if(vecResult[0].eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT)						
								{
									//�ı���Ʒ����
									if(pVolContainer->Add(vecResult[0].dwPos, vOut[i], NULL) )
									{
										CGoods* pExistedGoods = pVolContainer->GetGoods(vecResult[0].dwPos);
										if(pExistedGoods)
										{
											CS2CContainerObjectAmountChange coacMsg;
											coacMsg.SetSourceContainer( TYPE_PLAYER, pPlayer->GetExID(), 
												vecResult[0].dwPos );
											coacMsg.SetSourceContainerExtendID( vecResult[0].dwContainerId );
											coacMsg.SetObject( pExistedGoods->GetType(), pExistedGoods->GetExID() );
											coacMsg.SetObjectAmount( pExistedGoods->GetAmount() );
											coacMsg.Send( pPlayer->GetExID() );

											if(vOut[i]==NULL)
											{
												_snprintf(szOutStr,1024,"CHANGE_AMOUNT:vOut[%d] is null,goodname:%s:%s",i,testGoodsName,pPlayer->GetName());
												PutStringToFile("BounsGoods",szOutStr);
											}
											else
											{
												if( 0 == vOut[i]->GetAmount())
												{						
													_snprintf(szOutStr,1024,"CHANGE_AMOUNT:vOut[%d] goodname:%s:%s",i,testGoodsName,pPlayer->GetName());
													PutStringToFile("BounsGoods",szOutStr);
												}
											}
										}								
									}
								}

								// ��ȡ��Ʒ�ɹ�����¼��־
								GetGameLogInterface()->logT026_goods_log_bouns(pPlayer, dwGoodsIndex, goodsGuid, strGoodsName, testGoodsAount, pBD->dwBounsID);
							}
						}
						else
						{
							for(vector<CGoods*>::size_type i=0;i<vOut.size();i++)
							{
#ifdef _GOODSLOG1_
								if(vOut[i])
								{
									char pszGoodsLog[1024]="";			
									char srcGoodsGUID[50]="";
									vOut[i]->GetExID().tostring(srcGoodsGUID);					
									_snprintf(pszGoodsLog,1024,"[��Ʒ����:psshopSeller792][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
										vOut[i]->GetBasePropertiesIndex(),CGoodsFactory::QueryGoodsName(vOut[i]->GetBasePropertiesIndex()),srcGoodsGUID,vOut[i]->GetAmount());
									CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
								}		
#endif
								CGoodsFactory::GarbageCollect(&vOut[i],32);
							}
							pPlayer->SendNotifyMessage(CStringReading::LoadString(20,1));
							SendBounsRollBackMsg(tgBInfo.guid);
							return;
						}
					}

				}
				else if(tgBInfo.bType == 2)
				{
					// �������ֵ�ӳ�		
					bool bChange = pPlayer->ChangeAttribute(tgBInfo.strName, tgBInfo.dwValue);
					if(!bChange)
					{
						SendBounsRollBackMsg(tgBInfo.guid);
						return;
					}
					pPlayer->UpdateAttribute();
					
				}
				else if(tgBInfo.bType == 3)
				{
					// �ű�����
					char strFilePath[MAX_PATH];
					_snprintf(strFilePath, MAX_PATH, "scripts/bouns/%d.script",tgBInfo.dwValue);
					pPlayer->PlayerRunScript(strFilePath);
				}
			}

			CMessage msg(MSG_S2BDS_BOUNS_GET);
			msg.Add(pPlayer->GetExID());
			msg.Add((BYTE)1);	// ֪ͨBDS��ȡ�ɹ�
			msg.Add(tgBInfo.guid);
			msg.Add(pPlayer->GetName());
			msg.Add(szPlayerIP);
			msg.Add(GetGame()->GetWSNumber());
			msg.SendToBounsServer();

			CMessage msgRes(MSG_S2C_BOUNS_GET_RESULT);
			msgRes.Add(tgBInfo.guid);
			msgRes.Add(lRet);
			msgRes.SendToPlayer(pPlayer->GetExID());

		}
        break;

    case MSG_BDS2S_NOTICE:
        {
            WORD wType = pMsg->GetWord();
            char szText[512];
            pMsg->GetStr(szText, 512);
            CMessage msg(MSG_S2C_OTHER_BROADCAST);
            msg.Add((long)0);
            msg.Add((long)0);
            // ȡ���纰������ɫ
            msg.Add(MAKELONG(MAKEWORD(241,0), MAKEWORD(255,246)));
            msg.Add(MAKELONG(MAKEWORD(255,0), MAKEWORD(30,0)));
            msg.Add( szText );
            msg.SendAll();
        }
        break;

		//////////////////////////////////////////////////////////////////////////
		///  GameClient --> GameServer							//////////////////
		//////////////////////////////////////////////////////////////////////////
	case MSG_C2S_BOUNS_INFO:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer == NULL)
			{
				return;
			}
			if(pPlayer->IsDied())
			{
				pPlayer->SetCurrentProgress(CPlayer::PROGRESS_NONE);
				pPlayer->SetMoveable();
				return;
			}

			if(pPlayer->GetCurrentProgress() != CPlayer::PROGRESS_BOUNS)
			{
				return;
			}
			// ��һ��ʱ����жϣ��������Ƶ������
			DWORD dwCurTime = timeGetTime();
			if(dwCurTime - pPlayer->GetBounsTime() < CGlobeSetup::GetSetup()->lOperBounsTime)
			{
				return;
			}

			pPlayer->SetBounsTime(dwCurTime);

			CMessage msg(MSG_S2BDS_BOUNS_LOAD);
			msg.Add(pPlayer->GetAccount());
			msg.Add(pPlayer->GetExID());
			msg.Add(GetGame()->GetWSNumber());
			msg.SendToBounsServer();

		}
		break;

	case MSG_C2S_BOUNS_GET:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer == NULL)
			{
				return;
			}
			if(pPlayer->IsDied())
			{
				pPlayer->SetCurrentProgress(CPlayer::PROGRESS_NONE);
				pPlayer->SetMoveable();
				return;
			}

			// ��һ��ʱ����жϣ��������Ƶ������
			DWORD dwCurTime = timeGetTime();
			if(dwCurTime - pPlayer->GetBounsTime() < CGlobeSetup::GetSetup()->lOperBounsTime)
			{
				return;
			}

			pPlayer->SetBounsTime(dwCurTime);
			if(pPlayer->GetCurrentProgress() == CPlayer::PROGRESS_BOUNS)
			{
				DBReadSet setReadDB;
				pMsg->GetDBReadSet(setReadDB);
				tagBounsInfo tgBInfo;
				tgBInfo.DecodeFromDataBlock(setReadDB);

				if(tgBInfo.bStatus != 1)
				{
					pPlayer->SendNotifyMessage("��ȡ״̬����δ��ȡ״̬!������ȡ!");
					return;	
				}

				tagBounsDes *pBD = GetGame()->FindBounsDes(tgBInfo.dwBounsID);
				if(pBD == NULL)
				{
					pPlayer->SendNotifyMessage("��ȡ��Ϣ��������ȡ!");
					return;
				}



				// ������������ ����
				if(tgBInfo.bType == 1)
				{
					CTestContainer testContainer;
					vector<CGoods*> vOut;
					testContainer.Init(pPlayer);
					vector<tagTestResult> vecResult;		
			//		DWORD dwGoodsIndex = CGoodsFactory::QueryGoodsIDByOriginalName(pBD->strName.c_str());
					DWORD dwGoodsIndex = atoi(tgBInfo.strName.c_str());
					CGoodsFactory::CreateGoods(dwGoodsIndex, tgBInfo.dwValue, vOut, 32);

					if( vOut.size() == 0 )
					{
						// ��ƷIndex����Ч��
						AddLogText("ERROR: BounsID=%d Invalid GoodsIndex='%s'!!!",pBD->dwBounsID, tgBInfo.strName.c_str());
						return;
					}

					BOOL bSuccess = testContainer.AddTest(vOut, &vecResult);
					bSuccess = (bSuccess && (vecResult.size() == vOut.size())) ? TRUE:FALSE;

					// �ͷ�Goods
					for(int i=0; i<vOut.size(); i++)
					{
						CGoodsFactory::GarbageCollect(&vOut[i], 32);
					}

					if(!bSuccess)
					{
						pPlayer->SendNotifyMessage(CStringReading::LoadString(20,1));
						return;
					}
				}
				else if(tgBInfo.bType == 2)
				{
					// ��ֵ
					// ��ʱ���ж�
				}


				// ת�����IP����־��¼��
				char szTmp[64] = {0};
				UCHAR arrIp[4] = {0};
				DWORD lIP = pMsg->GetIP();
				memmove(arrIp, &lIP, sizeof(DWORD));
				_snprintf(szTmp, 64, "%u.%u.%u.%u", (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);

				DBWriteSet setWriteDB;
				CMessage msg(MSG_S2BDS_BOUNS_GET);
				msg.Add(pPlayer->GetExID());
				msg.Add((BYTE)2);	// ֪ͨBDS�������Ϣ
				msg.Add(szTmp);
				msg.GetDBWriteSet(setWriteDB);
				tgBInfo.EncodeToDataBlock(setWriteDB);
				msg.SendToBounsServer();

			}

		}
		break;

	case MSG_C2S_BOUNS_CLOSE:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer == NULL)
			{
				return;
			}

			pPlayer->SetCurrentProgress(CPlayer::PROGRESS_NONE);
			pPlayer->SetMoveable();

		}
		break;


	}
}

// ��¼��ȡ������־
bool LogicLogInterface::logT026_goods_log_bouns(CPlayer* pPlayer, DWORD dwGoodsId, const CGUID& goodsGuid, const char* strGoodsName, int nGoodsNum,int nBounsID)
{
	if(0 == GetGame()->GetSetup()->lUseLogServer)
		return true;

	if(NULL == m_pLogClient)
		return false;

	if (NULL == pPlayer) 
		return false;

	string strTime;
	GetCurrTimeString(strTime);

	return m_pLogClient->SendLogToServer(
		26, strTime.c_str(),
		&(pPlayer->GetExID()),	pPlayer->GetName(),
		dwGoodsId, &goodsGuid,strGoodsName, 
		nGoodsNum,nBounsID);
}