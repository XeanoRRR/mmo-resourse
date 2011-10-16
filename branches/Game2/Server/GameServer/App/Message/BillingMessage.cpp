#include "StdAfx.h"
#include "../Player.h"
#include "../Goods/CGoods.h"
#include "../BillApp/CBillSession.h"
#include "../BillApp/CBillSessionManager.h"

#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool IncShopProcessing( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwNum, DWORD dwTradeMode, long lTotalAmount);
//extern bool IncPrsentProcessing(CPlayer* pPlayer, vector<tagSGoods>, DWORD dwTradeMode, long &lTotalAmount , CGUID &MailGuid);

// ��ӦLOG��Ϣ
void OnBillingMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_B2S_GET_BALANCE:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			DWORD dwBalance=pMsg->GetDWord();
			CPlayer* pPlayer=GetGame()->FindPlayer(guid);
			if(pPlayer)
			{
				pPlayer->SetYuanbao(dwBalance);				
			}
		}
		break;
	case MSG_B2S_ACCOUNT_LOCK:
		{
			long lRet=pMsg->GetLong();
			BYTE btConsumeType=pMsg->GetByte();
			CGUID operGuid;
			pMsg->GetGUID(operGuid);
			CGUID guid;
			pMsg->GetGUID(guid);
			CGUID attachGuid;
			pMsg->GetGUID(attachGuid);
			
			char strCdkey[256];			
			pMsg->GetStr(strCdkey,256);
			DWORD dwNum=pMsg->GetDWord();
			if(dwNum<1)
				return;
// 			vector<tagSGoods> vecGoodsData;
// 			for(int i=0;i<(int)dwNum;i++)
// 			{
// 				tagSGoods tgSGoods;
// 				tgSGoods.lIndex = pMsg->GetDWord();
// 				tgSGoods.lNum = pMsg->GetDWord();
// 				vecGoodsData.push_back(tgSGoods);
// 				//vecGoodsData.push_back(pMsg->GetDWord());
// 			}
			
			long lLockNum=pMsg->GetLong();
			switch(lRet)
			{
			case ACCOUNT_LOCK_OK:
				{
					//Ԥ���ɹ� ���ŵ���					
					CPlayer* pPlayer=GetGame()->FindPlayer(guid);
					if(pPlayer)
					{
						if(btConsumeType==eICT_NORMAL_BUY_ITEM)
						{
							if(dwNum>1)
								return;
							//vector<tagSGoods>::iterator it=vecGoodsData.begin();

							//bool bSucceed=IncShopProcessing(pPlayer,(*it).lIndex,(*it).lNum,2,lLockNum);
							//if(bSucceed)
							//{
							//	

							//	pPlayer->SetYuanbao(pPlayer->GetYuanbao()-lLockNum);

							//	CMessage msg(MSG_S2B_ACCOUNT_UNLOCK);
							//	msg.Add(strCdkey);//cd key
							//	msg.Add(operGuid);//oper guid
							//	msg.Add(pPlayer->GetName());//player name
							//	msg.Add(pPlayer->GetExID());//player guid			
							//	
							//	msg.Add((DWORD)1);
							//	//msg.Add((*it).lIndex);//goods id
							//	//msg.Add((*it).lNum);
							//	msg.Add(lLockNum);//lock num
							//	msg.Add(GetGame()->GetWSNumber());
							//	string strGoodsName="";
							//	if(GoodsSetup::QueryGoodsBaseProperties((*it).lIndex))
							//	{
							//		strGoodsName=GoodsSetup::QueryGoodsName((*it).lIndex);
							//	}								
							//	msg.Add(strGoodsName.c_str());
							//	msg.SendToBS();
							//}	
							//else
							//{
							//	//�ռ䲻��
							//	const char* strRes=AppFrame::GetText("3");
							//	if(strRes)
							//		pPlayer->SendNotifyMessage(strRes);
							//	//Ԥ���ع�
							//	CMessage msg(MSG_S2B_ACCOUNT_LOCK_ROLLBACK);
							//	msg.Add(strCdkey);
							//	msg.Add(lLockNum);
							//	msg.Add(operGuid);
							//	msg.SendToBS();

							//}
						}
						else if(btConsumeType==eICT_MAIL_BUY_ITEM)
						{
						}
						//else if(btConsumeType==eICT_USE_FUNCTION)
						//{
						//	//ʹ����ֵ����
						//	if(vecGoodsData.size()==1)
						//	{
						//		switch(vecGoodsData[0].lIndex)
						//		{
						//		case eIFUNC_RELIVE:
						//			{
						//				//ԭ�ظ���
						//				pPlayer->SetYuanbao(pPlayer->GetYuanbao()-lLockNum);
						//				pPlayer->OnRelive(1);
						//				string str=GlobalSetup::GetSetup()->szReliveGoodsName;
						//				DWORD dwIndex = GoodsSetup::QueryGoodsIDByOriginalName(str.c_str());
						//				CMessage msg(MSG_S2C_PLAYER_RELIVE_RESULT);
						//				msg.Add(BYTE(1));//�ɹ�
						//				msg.Add(dwIndex);
						//				msg.Add(pPlayer->GetExID());

						//				CMessage msg2BS(MSG_S2B_ACCOUNT_UNLOCK);
						//				msg2BS.Add(strCdkey);//cd key
						//				msg2BS.Add(operGuid);//oper guid
						//				msg2BS.Add(pPlayer->GetName());//player name
						//				msg2BS.Add(pPlayer->GetExID());//player guid			

						//				msg2BS.Add((DWORD)1);
						//				msg2BS.Add(vecGoodsData[0].lIndex);//goods id
						//				msg2BS.Add(vecGoodsData[0].lNum);
						//				msg2BS.Add(lLockNum);//lock num
						//				msg2BS.Add(GetGame()->GetWSNumber());
						//				string strGoodsName="FUNC_RELIVE";																	
						//				msg2BS.Add(strGoodsName.c_str());
						//				msg2BS.SendToBS();
						//			}
						//			break;
						//		default:
						//			break;
						//		}
						//	}
						//}
					}
				}
				break;
			case ACCOUNT_LOCK_ERROR_CDKEY:
				{
				}
				break;
			case ACCOUNT_LOCK_NOT_ENOUGH:
				{
					//�˻�����
					CPlayer* pPlayer=GetGame()->FindPlayer(guid);
					if(pPlayer)
					{
						const char* strRes=AppFrame::GetText("22");
						if(strRes)
							pPlayer->SendNotifyMessage(strRes);
					}					
				}
				break;
			case ACCOUNT_LOCK_UNKNOWN_ERROR:
				{
				}
				break;
			}
		}
		break;
	case MSG_B2S_ACCOUNT_UPDATED:
		{
			/*
			char strCdkey[256];			
			pMsg->GetStr(strCdkey,256);
			CGUID sessionGuid;
			pMsg->GetGUID(sessionGuid);
			//ͨ��GUID��billsession
			CBillSession* pBillSession=const_cast<CBillSession*>(CBillSessionManager::QuerySession(sessionGuid));
			if(pBillSession)
			{
				pBillSession->SetBusinessState(eBSTATE_ACCOUNT_UPDATED);
			}*/
		}
		break;
	}
}