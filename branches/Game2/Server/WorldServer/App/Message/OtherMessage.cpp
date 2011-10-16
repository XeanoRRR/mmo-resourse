#include "StdAfx.h"

#include "../GlobalRgnManager.h"
#include "../DBEntity/EntityManager.h"
#include "../Goods/Goods.h"
#include "../Goods/GoodsFactory.h"
#include "../Goods/IncrementShopManager.h"
#include "../OrganizingSystem/OrganizingCtrl.h"
#include "../OrganizingSystem/Faction.h"
#include "../PlayerMan/PlayerMan.h"
#include "../Script/VariableList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// ��Ӧ������Ϣ
void OnOtherMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_S2W_OTHER_UPDATE_HIDE_FLAG:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			BYTE flag = pMsg->GetByte();
			CPlayer* player = GetGame().GetMapPlayer(guid);
			if(player)
			{
				player->SetHideFlag(flag);
				// ˢ�½�ɫ����״̬
				GetInst(CPlayerMan).RefreshElem(PET_HIDENAME, player, (long)flag);
				GetOrganiCtrl()->OnPlayerHideChange(player, flag);
			}
		}
		break;
	case MSG_S2W_OTHER_CHANGENAME:
		{
			char szName[32] = {0};
			pMsg->GetStr(szName, sizeof(szName));
			CGUID guid;
			pMsg->GetGUID(guid);
			GetGame().GetEntityManager()->CreateChangeNameProcSession(guid, szName, pMsg->GetSocketID());
		}
		break;

	case MSG_S2W_OTHER_GETLOG_REQUEST:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CPlayer* pPlayer = GetGame().GetMapPlayer( PlayerGuid );
			if(pPlayer==NULL) break;

			CMessage msg(MSG_W2DB_PSASS_OTHER_GETLOG_REQUEST);
			msg.Add(PlayerGuid);
			msg.Add((long)pMsg->GetGSID()); // gsid
			msg.SendToServer(NF_DB_SERVER);
		}
		break;

	case MSG_S2W_OTHER_TALK:	// �Ի���Ϣ
		{
			BYTE bTalkType= pMsg->GetByte();
			long lType = pMsg->GetLong();
			char strTargetName[256] = "";
			long lsenderType = pMsg->GetLong();	// SenderType
			CGUID PlayerGuid; pMsg->GetGUID(PlayerGuid);	// playerID

			LONG lGoodsNum = pMsg->GetLong();

			//! -----------------������Ʒ��Ϣ����-----------------
			tagChatsGoodsInfo tmpChatsGoodsInfo;
			char szGoodsName[256] = {0};
			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				pMsg->GetEx(&tmpChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(szGoodsName, 255);
			}
			//! ----------------------------------

			static char strSourceName[256]="";
			static char strText[1024]="";
			if(CHAT_PRIVATE == lType)
				pMsg->GetStr(strTargetName,256);
			pMsg->GetStr(strSourceName,256);
			pMsg->GetStr(strText,1024);

			switch(lType)
			{
			case CHAT_PRIVATE:
				{
					CPlayer* pPlayer = GetGame().GetMapPlayer(strTargetName); 
					if (pPlayer != NULL)
					{
						GSConnection *pGameServer = GetGame().GetRegionGameServer(pPlayer->GetRegionExID());
						if (pGameServer != NULL)
						{
							// �ɹ�
							pMsg->SetType(MSG_W2S_OTHER_TALK);
							pMsg->SendToGS(pGameServer->index);// ����Ŀ��
							pMsg->SendToSocket(pMsg->GetSocketID());// ����������
							break;
						}
					}
							
					CPlayer *pSender = GetGame().GetMapPlayer(PlayerGuid);
					if(NULL != pSender)
					{
						CMessage msg(MSG_W2S_OTHER_TALK);
						msg.Add((BYTE)9);		// ʧ��
						msg.Add((LONG)CHAT_PRIVATE);
						msg.Add(pSender->GetType());
						msg.Add(pSender->GetExID());
						msg.Add(0L);
						msg.Add(strTargetName);
						msg.SendToSocket(pSender->GetGsSocket());
					}
				}
				break;

			case CHAT_FACTION:
				{
					CGUID GSFactinID;
					pMsg->GetGUID(GSFactinID);
					CPlayer* pPlayer = GetGame().GetMapPlayer( PlayerGuid );
					if(NULL == pPlayer) return;
					const CGUID& WSFactinID = pPlayer->GetFactionID();
					if(		WSFactinID == NULL_GUID 
						||	GSFactinID != WSFactinID 
						||	GetOrganiCtrl()->GetFactionOrganizing(WSFactinID) == NULL
						)
						break;
					pMsg->SetType(MSG_W2S_OTHER_TALK);
					pMsg->SendToAllGS();
					
				}
				break;
			case CHAT_TEAM:
				{
					CGUID GSTeamGuid;
					pMsg->GetGUID(GSTeamGuid);
					CPlayer* pPlayer = GetGame().GetMapPlayer( PlayerGuid );
					if(NULL == pPlayer) return;
					const CGUID& WSTeamGuid = pPlayer->GetTeamID();
					if(		WSTeamGuid == NULL_GUID 
						||	WSTeamGuid != GSTeamGuid 
						||	GetOrganiCtrl()->GetGameTeamByID(WSTeamGuid) == NULL
						)
						break;
					pMsg->SetType(MSG_W2S_OTHER_TALK);
					pMsg->SendToAllGS();

				}
				break;
			}
		}
		break;
	case MSG_S2W_OTHER_GoodsInfo_Create:
		{
			LONG lInfoNum = pMsg->GetLong();

			CGUID InfoKey;
			vector<BYTE> vGoodsInfo;
			for (LONG i = 0; i < lInfoNum; ++i)
			{
				pMsg->GetGUID(InfoKey);
				LONG lDataSize = pMsg->GetLong();
				vGoodsInfo.resize(lDataSize);
				pMsg->GetEx(&vGoodsInfo[0], lDataSize);
				CGoodsFactory::AddGoodsInfo(InfoKey, vGoodsInfo);
			}
		}
		break;
	case MSG_S2W_OTHER_GoodsInfo_Request:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			CGUID InfoKey;
			pMsg->GetGUID(InfoKey);

			vector<BYTE> vGoodsInfo;
			if(CGoodsFactory::GetGoodsInfo(InfoKey, vGoodsInfo))
			{
				pMsg->SetType(MSG_W2S_OTHER_GoodsInfo_Re);
				pMsg->Add((LONG)vGoodsInfo.size());
				if(vGoodsInfo.size())
					pMsg->AddEx(&vGoodsInfo[0],(long)vGoodsInfo.size());
				pMsg->SendToSocket(pMsg->GetSocketID());
			}
		}
		break;
	case MSG_S2W_OTHER_GOODSLINK://gameserver���͵�һ����Ʒ������Ϣ
		{
			char strChatText[1024];
			char strChatObjName[256];
			char strSenderName[256];
			long lType=pMsg->GetLong();	//chat type
			long lSenderType=pMsg->GetLong();	//sender type
			long lSenderID=pMsg->GetLong();	//sender id
			if( lType == 2 )
			{
				pMsg->GetStr(strChatObjName,256);
			}
			pMsg->GetStr(strSenderName,256);			

			pMsg->GetStr(strChatText,1024);			
			string strTemp=strChatText;
			long lNum=pMsg->GetLong();
			size_t nPos=0;
			for(long i=0;i<lNum;i++)
			{
				long change=pMsg->GetLong();
				CGame::tagGoodsLink stGoodsLink;
				if(change==1)
				{
#ifdef __MEMORY_LEAK_CHECK__
				CBaseObject::SetConstructFlag(1);
#endif
					CGoods* pGoods=new CGoods;
					BYTE *pGoodData = new BYTE[102400];
					pMsg->GetEx(pGoodData,102400);
					long lPos = 0;
					pGoods->DecordFromByteArray(pGoodData,lPos);
					SAFE_DELETE_ARRAY(pGoodData);

					CGoodsFactory::AddCreateGoodsLog(pGoods,26);
					
					stGoodsLink.bChange=true;
					stGoodsLink.dwType=0;
					stGoodsLink.lNum=0;
					stGoodsLink.pGoods=pGoods;
					//stGoodsLink.dwIndex=pGoods->GetID();
					GetGame().AddGoodsLink(&stGoodsLink);
				}
				else
				{					
					stGoodsLink.bChange=false;
					stGoodsLink.dwType=pMsg->GetDWord();
					stGoodsLink.lNum=pMsg->GetByte();
					stGoodsLink.pGoods=NULL;
					stGoodsLink.dwIndex=0;
					GetGame().AddGoodsLink(&stGoodsLink);
				}

				//�޸���Ʒ����ǰ׺
				size_t nIndex=strTemp.find("change=",nPos);

				//ɾ���Ӵ�"change=x "
				strTemp.erase(nIndex,9);
				
				/*
				*�޸�"id=xxx"�Ӵ�
				*/
				nIndex+=3; //"id="

				size_t nPrefixEnd=strTemp.find(">",nIndex);
				//id length=nPrefixEnd - nIndex

                //ɾ��ID
				strTemp.erase(nIndex,nPrefixEnd - nIndex);

				//д����ID
				char strID[32];
				sprintf(strID,"%d",stGoodsLink.dwIndex);
				strTemp.insert(nIndex,strID);

				//update npos
				nIndex=strTemp.find("</goodslink>",nIndex);
				nPos=nIndex+12;

				
			}
			/*
			*	message rebuild
			*/
			CMessage msgGoodsLink(MSG_W2S_OTHER_GOODSLINK);
			msgGoodsLink.Add(lType);
			msgGoodsLink.Add(lSenderType);
			msgGoodsLink.Add(lSenderID);
			if( lType == 2 )
			{
				msgGoodsLink.Add(const_cast<char*>(strChatObjName));
			}
			msgGoodsLink.Add(const_cast<char*>(strSenderName)); //sender name

			msgGoodsLink.Add(const_cast<char*>(strTemp.c_str()));	//updated chat text 

			msgGoodsLink.SendToSocket(pMsg->GetSocketID());
		}
		break;

		//gameserver�������ID����һ����Ʒ����
	case MSG_S2W_OTHER_VIEWGOODSLINK:
		{
			long PlayerGuid=pMsg->GetLong();
			DWORD dwID=pMsg->GetDWord();
			CGame::tagGoodsLink* pstGoodsLink=GetGame().FindGoodsLink(dwID);
			CMessage msg(MSG_W2S_OTHER_VIEWGOODSLINK);
			msg.Add(PlayerGuid);
			if(pstGoodsLink)
			{	
				msg.Add((long)1); //�Ƿ��ҵ�
				if(pstGoodsLink->bChange)
				{					
					vector<BYTE> GoodsData;
					pstGoodsLink->pGoods->AddToByteArray(&GoodsData);
					if(GoodsData.size() > 0)
						msg.AddEx(&GoodsData[0],(long)GoodsData.size());
					
				}
				else
				{							
#ifdef __MEMORY_LEAK_CHECK__
					CBaseObject::SetConstructFlag(51);
#endif
					CGoods* pGoods=CGoodsFactory::CreateGoods(pstGoodsLink->dwType,1);
					if(pGoods)
					{
						pGoods->SetAmount(pstGoodsLink->lNum);

						vector<BYTE> GoodData;
						pGoods->AddToByteArray(&GoodData);
						if( GoodData.size() > 0)
							msg.AddEx(&GoodData[0],(long)GoodData.size());
						CGoodsFactory::GarbageCollect(&pGoods);
					}
				}
			}
			else
			{
				msg.Add((long)0);
			}
			msg.SendToSocket(pMsg->GetSocketID());
		}
		break;

		//�������ָ����ĳһ���ͻ������һ��ϵͳ������Ϣ
	case MSG_S2W_OTHER_ADDINFO:
		{
			DWORD dwGSIndex=pMsg->GetDWord();
			pMsg->SetType(MSG_W2S_OTHER_ADDINFO);
			pMsg->SendToGS(dwGSIndex);
		}
		break;
	//case MSG_S2W_OTHER_CHANGENAME:	// ����gs���ĸ�������
	//	{
	//		CGUID PlayerGuid;					// ������ɫID
	//		char szNewName[32] = "";			// �¸�������
	//		char cRes=1;						// ���������0�ɹ�������ʧ�ܡ�

	//		pMsg->GetGUID(PlayerGuid);
	//		pMsg->GetStr(szNewName, 32);

	//		CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
	//		if(pPlayer==NULL) break;
	//		if(pPlayer) {
	//			cRes = (char)pPlayer->ChangeName(szNewName);
	//		}
	//		// else defualt "res = 1" : false

	//		CMessage msg(MSG_W2S_OTHER_CHANGENAME);
	//		msg.Add(PlayerGuid);
	//		msg.Add(cRes);
	//		msg.Add(szNewName);
	//		msg.SendToSocket(pMsg->GetSocketID());
	//	}
	//	break;
	case MSG_S2W_OTHER_BROADCAST:
		{
			pMsg->SetType(MSG_W2S_OTHER_BROADCAST);
			pMsg->SendToAllGS();
		}
		break;

		// ����s���չ��������纰��
	case MSG_S2W_OTHER_TALK_WORLD:
		{
			pMsg->SetType(MSG_W2S_OTHER_TALK_WORLD);
			pMsg->SendToAllGS();
		}
		break;
		//����GS���������ö����纰��
	case MSG_S2W_OTHER_TALK_TOP_WORLD:
		{
			pMsg->SetType(MSG_W2S_OTHER_TALK_TOP_WORLD);
			pMsg->SendToAllGS();
		}
		break;
		//��������
	case MSG_S2W_OTHER_NEWS_TOP_REGION:
		{
			char strNews[1024];
			long lRegionID = pMsg->GetLong();
			long lRgnType = pMsg->GetLong();
			long lNum = pMsg->GetLong();
			pMsg->GetStr(strNews,1024);
			long lFileNum = pMsg->GetLong();

			GSConnection* gs = GetGame().GetRegionGameServer(lRegionID);

			CMessage msg(MSG_W2S_OTHER_NEWS_TOP_REGION);
			msg.Add(lRegionID);
			msg.Add(lRgnType);
			msg.Add(lNum);
			msg.Add((char*)strNews);
			msg.Add(lFileNum);
			if (gs)
			{
				msg.SendToGS(gs->index);
			}
			else
			{
				msg.SendToAllGS();
			}
		}
		break;
		//��������
	case MSG_S2W_OTHER_NEWS_TOP_WORLD:
		{
			pMsg->SetType(MSG_W2S_OTHER_NEWS_TOP_WORLD);
			pMsg->SendToAllGS();
		}
		break;

			// ����s���չ����Ĺ��Һ���
	case MSG_S2W_OTHER_TALK_COUNTRY:
		{
			pMsg->SetType(MSG_W2S_OTHER_TALK_COUNTRY);
			pMsg->SendToAllGS();
		}
		break;
	case MSG_S2W_OTHER_TALK_COUNTRY_UNION:
		{
			pMsg->SetType(MSG_W2S_OTHER_TALK_COUNTRY_UNION);
			pMsg->SendToAllGS();
		}
		break;

		// ����s���չ����Ĺ���֪ͨ
	case MSG_S2W_OTHER_TALK_NOTIFY:
		{
			pMsg->SetType(MSG_W2S_OTHER_TALK_NOTIFY);
			pMsg->SendToAllGS();
		}
		break;
		// ���б���ͨ��
	case MSG_S2W_OTHER_WARNING:
		{
			pMsg->SetType(MSG_W2S_OTHER_WARNING);
			pMsg->SendToAllGS();
		}
		break;
	case MSG_S2W_OTHER_LIMIT_GOODS_REQUEST:
		{			
			CGUID PlayerGuid;
			pMsg->GetGUID( PlayerGuid );			
			DWORD dwGoodsId=pMsg->GetDWord();
			DWORD dwGoodsNum=pMsg->GetDWord();
			DWORD dwTradeMode = pMsg->GetDWord();
			DWORD dwLimitNum = pMsg->GetLong();
			if ( dwGoodsNum == 0 || dwLimitNum == 0 )
			{
				return;
			}

			//ȡ��manager�и�ID�����¼
			DWORD dwBuyNum=CIncrementShopManager::GetSingleton().GetLimitGoodsBuyNum(dwGoodsId);
			if(dwBuyNum+dwGoodsNum>dwLimitNum)
			{
				//����
				CMessage msg(MSG_W2S_OTHER_LIMIT_GOODS_RESPONSE);
				msg.Add( PlayerGuid );
				msg.Add(static_cast<DWORD>(eICR_WORLD_NUM_LIMIT));
				msg.SendToSocket(pMsg->GetSocketID());
			}
			else
			{
				//�ɹ���
				CMessage msg(MSG_W2S_OTHER_LIMIT_GOODS_RESPONSE);
				msg.Add( PlayerGuid );
				msg.Add(static_cast<DWORD>(eICR_BUY_SUCCEED));
				msg.Add( dwGoodsId );
				msg.Add( dwGoodsNum );
				msg.Add( dwTradeMode );
				msg.Add( dwBuyNum );
				msg.SendToSocket(pMsg->GetSocketID());
				//����
				CIncrementShopManager::GetSingleton().AddLimitGoodsRecord(dwGoodsId,dwGoodsNum);
				//����DB
				GetGame().GetEntityManager()->CreateSaveLimitGoodsRecordSession();

				//��������GS
				CMessage msgToGS( MSG_W2S_OTHER_LIMIT_GOODS_UPDATA );
				msgToGS.Add( dwGoodsId );
				msgToGS.Add( dwGoodsNum );
				msgToGS.SendToAllGS();
			}
		}
		break;
	case MSG_S2W_OTHER_DEL_SPOUSE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CPlayer *pPlayer = GetGame().GetMapPlayer(PlayerGuid);
			if (NULL != pPlayer)
			{
				pMsg->SetType(MSG_W2S_OTHER_DEL_SPOUSE);
				pMsg->SendToSocket(pPlayer->GetGsSocket());
			}
		}
		break;
	default:
		break;
	}
}
