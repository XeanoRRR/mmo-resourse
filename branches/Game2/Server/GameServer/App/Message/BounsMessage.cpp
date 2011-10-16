#include "StdAfx.h"

#include "../Container/CTestContainer.h"
#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../MessagePackage/CS2CContainerObjectAmountChange.h"
#include "../Player.h"
#include "../Session/CSession.h"
#include "../Session/CSessionFactory.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


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
                if(pBD->bType == 1)
                {				
                    //	DWORD dwGoodsIndex = CGoodsFactory::QueryGoodsIDByOriginalName(pBD->strName.c_str());
                    DWORD dwGoodsIndex = atoi(pBD->strName.c_str());
                    if(dwGoodsIndex == 0)
                    {
                        // �Ƿ���Ʒ�������϶�ʧ��
                        SendBounsRollBackMsg(tgBInfo.guid);
                        return;
                    }
                    vector<CGoods*> vOut;
                    CGoodsFactory::CreateGoods(dwGoodsIndex, pBD->dwValue, vOut, 32);
                    if(vOut.size() == 0)
                    {
                        // ��ƷIndex����Ч��
                        SendBounsRollBackMsg(tgBInfo.guid);
                       Log4c::Warn(ROOT_MODULE,"ERROR: BounsID=%d Invalid GoodsIndex='%s'!!!",pBD->dwBounsID,pBD->strName.c_str());
                        return;
                    }

                    CTestContainer testContainer;
                    testContainer.Init(pPlayer);
                    vector<tagTestResult> vecResult;
                    BOOL bSuccess = testContainer.AddTest(vOut, &vecResult);
                    //��ʽ��ӵ�����
                    if (bSuccess && vecResult.size() == vOut.size())
                    {
                        for(int i=0;i<(int)vecResult.size();i++)
                        {
                            //�����Ʒ�п���ɾ��vOut[i]ָ��,�ȼ�������������־
                            CGUID goodsguid=vOut[i]->GetExID();
                            //���ʧ�������
                            // ע��!!! ���AddGoodsByTest��ӵ���OT_CHANGE_AMOUNT��ʽ,
                            // vOut[i]�ᱻ���,��¼��־��ʱ��Ͳ�������
                            if(!pPlayer->AddGoodsByTest(&vecResult[i],vOut[i]))
                            {
                                CGoodsFactory::GarbageCollect(&vOut[i],176);
                            }
                            else
                            {
                                // ��ȡ��Ʒ�ɹ�����¼��־
                                GetLogicLogInterface()->logT026_goods_log_bouns(pPlayer, dwGoodsIndex, tgBInfo.guid,
                                    GoodsSetup::QueryGoodsName(dwGoodsIndex),pBD->dwValue, pBD->dwBounsID);
                            }
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
                        pPlayer->SendNotifyMessage(AppFrame::GetText("1"));
                        SendBounsRollBackMsg(tgBInfo.guid);
                        return;
                    }


				}
				else if(pBD->bType == 2)
				{
					// �������ֵ�ӳ�		
					bool bChange = pPlayer->ChangeAttribute(pBD->strName, pBD->dwValue);
					if(!bChange)
					{
						SendBounsRollBackMsg(tgBInfo.guid);
						return;
					}
					pPlayer->UpdateAttribute();
					
				}
				else if(pBD->bType == 3)
				{
					// �ű�����
					char strFilePath[MAX_PATH];
					_snprintf(strFilePath, MAX_PATH, "scripts/bouns/%d.lua",pBD->dwValue);
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
			if( (long)(dwCurTime - pPlayer->GetBounsTime()) < GlobalSetup::GetSetup()->lOperBounsTime)
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
			if((long)(dwCurTime - pPlayer->GetBounsTime()) < GlobalSetup::GetSetup()->lOperBounsTime)
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
				if(pBD->bType == 1)
				{
					CTestContainer testContainer;
					vector<CGoods*> vOut;
					testContainer.Init(pPlayer);
					vector<tagTestResult> vecResult;		
			//		DWORD dwGoodsIndex = GoodsSetup::QueryGoodsIDByOriginalName(pBD->strName.c_str());
					DWORD dwGoodsIndex = atoi(pBD->strName.c_str());
					CGoodsFactory::CreateGoods(dwGoodsIndex, pBD->dwValue, vOut, 32);

					if( vOut.size() == 0 )
					{
						// ��ƷIndex����Ч��
						Log4c::Warn(ROOT_MODULE,"ERROR: BounsID=%d Invalid GoodsIndex='%s'!!!",pBD->dwBounsID,pBD->strName.c_str());
						return;
					}

					BOOL bSuccess = testContainer.AddTest(vOut, &vecResult);
					bSuccess = (bSuccess && (vecResult.size() == vOut.size())) ? TRUE:FALSE;

					// �ͷ�Goods
					for(int i=0; i<(int)vOut.size(); i++)
					{
						CGoodsFactory::GarbageCollect(&vOut[i], 32);
					}

					if(!bSuccess)
					{
						pPlayer->SendNotifyMessage(AppFrame::GetText("11"));
						return;
					}
				}
				else if(pBD->bType == 2)
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
