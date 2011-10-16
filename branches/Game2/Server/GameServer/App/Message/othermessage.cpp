#include "StdAfx.h"

#include "../AntiCheat/CPlayerManager.h"
#include "../Area.h"
#include "../Goods/CGoods.h"
#include "../Goods/CGoodsFactory.h"
#include "../Goods/IncrementShopManager.h"
#include "../Listener/CSeekGoodsListener.h"
#include "../OrganizingSystem/GSTeam.h"
#include "../OrganizingSystem/GameFaction.h"
#include "../OrganizingSystem/GameOrganizingCtrl.h"
#include "../PKSys.h"
#include "../Player.h"
#include "../PlayerMan/PlayerMan.h"
#include "../RgnManager.h"
//#include "../Script/Script.h"
//#include "../Script/VariableList.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h"
#include "../ServerRegion.h"
#include "../../../../Public/ServerPublic/Server/Common/WordsFilter.h"
#include "../../../../Public/Setup/GoodsSetup.h"
#include "../../../../Public/Setup/GMList.h"
#include "../../../../Public/Setup/GlobalSetup.h"
#include "../../../../Public/Setup/PlayerList.h"

extern void CancelIncrementTrading( CPlayer* pPlayer );
extern void ProcessIncrementTrading( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwBuyNum, DWORD dwTradeMode );

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//! �����п��԰����������Ʒ����
const LONG MAX_GOODS_NUM_IN_CHAT = 200;

struct tagChatsGoodsInfo_tmp
{
	tagChatsGoodsInfo	ChatsGoodsInfo;
	char				szGoodsName[256];
	vector<BYTE>		vGoodsData;
};


// ��Ӧ������Ϣ
void OnOtherMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_W2S_OTHER_WARNING:
		{
			pMsg->SetType(MSG_S2C_OTHER_WARNING);
			pMsg->SendAll();
		}
		break;
	case MSG_W2S_OTHER_LIMIT_GOODS_RESPONSE:
		{
			CPlayer* pPlayer = NULL;
			CGUID guid;
			pMsg->GetGUID(guid);
			pPlayer = GetGame()->FindPlayer(guid);
			DWORD dwIncrementBuyRet = pMsg->GetDWord();
			if ( dwIncrementBuyRet == eICR_WORLD_NUM_LIMIT )
			{
				//����������
				CancelIncrementTrading( pPlayer );
				const char* strRes=AppFrame::GetText("26");
				if(strRes)
					pPlayer->SendNotifyMessage( strRes );
				return;
			}
			else if ( dwIncrementBuyRet == eICR_BUY_SUCCEED )
			{
				//�ɹ���
				DWORD dwGoodsId=pMsg->GetDWord();
				DWORD dwGoodsNum=pMsg->GetDWord();
				DWORD dwTradeMode = pMsg->GetDWord();
				DWORD dwAlreadyBuyNum = pMsg->GetDWord();
				//������
				ProcessIncrementTrading( pPlayer, dwGoodsId, dwGoodsNum, dwTradeMode );
				//��ʱˢ����Ϣ
				long lWorldAllowBuyNum = CIncrementShopList::GetWorldBuyLimitNum( dwTradeMode, dwGoodsId );
				assert( lWorldAllowBuyNum != -1 );

				CMessage msg(MSG_S2C_INCREMENTSHOP_UPDATA);
				msg.Add( (BYTE)1 );																										//ˢ�±�ʾ��0:�����޹�1:��������
				msg.Add( (BYTE)dwTradeMode );																				//�̵�����
				CIncrementShopList::MAP_INCGOODS* pIncGoodsList = CIncrementShopList::GetIncGoodsList();
				CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties( dwGoodsId );
				CIncrementShopList::ITR_INCGOODS itIncGoodsList = pIncGoodsList->find( dwTradeMode );
				CIncrementShopList::MAP_INCSHOP mapIncGoodsList = itIncGoodsList->second;
				CIncrementShopList::ITR_INCSHOP itInc = mapIncGoodsList.find( dwGoodsId );
				if ( pProperties && itInc != mapIncGoodsList.end() )
				{
					msg.Add( itInc->second.dwGoodsType );																	//��Ʒ����
					assert( dwGoodsId == itInc->second.dwGoodsIndex );
					msg.Add( itInc->second.dwGoodsIndex );																	//���ID
				}
				msg.Add( DWORD(lWorldAllowBuyNum - dwAlreadyBuyNum - dwGoodsNum) );		//ʣ��ɹ�������
				msg.SendAll();
			}
		}
		break;
	case MSG_W2S_OTHER_LIMIT_GOODS_UPDATA:
		{
			DWORD dwGoodsId=pMsg->GetDWord();
			DWORD dwGoodsNum=pMsg->GetDWord();
			CIncrementShopManager::AddLimitGoodsRecord( dwGoodsId, dwGoodsNum );
		}
		break;
	case MSG_W2S_OTHER_NEW_DAY:
		{
			DWORD dwData = pMsg->GetDWord();
			GetGame()->OnNewDayStart(dwData);
		}
		break;
	case MSG_W2S_OTHER_DEL_SPOUSE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
			if (NULL != pPlayer)
				pPlayer->SetSpouse(NULL);
		}
		break;
	case MSG_C2S_OTHER_UPDATENAMEANSWER:
		{
			// �����������Ƿ��С�#������
			CGUID scriptid;
			pMsg->GetGUID(scriptid);
			// ��������Ľű�
			//listScript::iterator  itr = g_Scripts.find(scriptid);
			//if(itr == g_Scripts.end())
			//{
			//	char szGuid[128];
			//	scriptid.tostring(szGuid);
			//	return;
			//}
			//ContinueRunScript(scriptid, NULL);
			GetInst( ScriptSys ).ResumeScript( scriptid, 0 );

			CGUID guid;
			pMsg->GetGUID(guid);
			char szName[64] = {0};
			pMsg->GetStr(szName, sizeof(szName));
			if(szName[0] == '\0')
			{
				// ��Ӧ�ͻ�����Ϣ
				CMessage retmsg(MSG_S2C_OTHER_CHANGENAME);
				retmsg.Add(guid);
				retmsg.Add(szName);
				retmsg.Add((BYTE)0); // ���ֲ��Ϸ�
				retmsg.SendToPlayer(guid);
				return;
			}

			std::string strName = szName;
			// ��������Ƿ�Ϸ�
			if(!CWordsFilter::GetInstance()->Check(strName))
			{
				// ��Ӧ�ͻ�����Ϣ
				CMessage retmsg(MSG_S2C_OTHER_CHANGENAME);
				retmsg.Add(guid);
				retmsg.Add(szName);
				retmsg.Add((BYTE)-2); // ���ֲ��Ϸ�
				retmsg.SendToPlayer(guid);
				return;
			}

			// ����������Ƿ��пո�
			std::string::size_type loc = strName.find(' ');
			if( loc != std::string::npos)
			{
				// ��Ӧ�ͻ�����Ϣ
				CMessage retmsg(MSG_S2C_OTHER_CHANGENAME);
				retmsg.Add(guid);
				retmsg.Add(szName);
				retmsg.Add((BYTE)-2); // ���ֲ��Ϸ�
				retmsg.SendToPlayer(guid);
				return;
			}

			CMessage msg(MSG_S2W_OTHER_CHANGENAME);
			msg.Add(szName);
			msg.Add(guid);
			msg.Send();
		}
		break;
	case MSG_W2S_OTHER_CHANGENAME:
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			
			char szName[32] = {0};
			pMsg->GetStr(szName, sizeof(szName));

			BYTE retFlag = pMsg->GetByte();

			

			// ������Ϣ���ͻ���
			CMessage retmsg(MSG_S2C_OTHER_CHANGENAME);
			retmsg.Add(guid);
			retmsg.Add(szName);
			retmsg.Add((BYTE)retFlag); // ���ֲ��Ϸ�

			CPlayer* pPlayer = NULL;
			if(retFlag == 1)
			{
				pPlayer = GetGame()->FindPlayer(guid);
				if(pPlayer)
					pPlayer->SetName(szName);
				retmsg.SendToAround(pPlayer); // �ɹ���֪ͨ��Χ����Ⱥ
				// Fox@20081110 ˢ�½�ɫ����
				GetInst(CPlayerMan).RefreshElem(PET_NAME, pPlayer);
			}
			else
				retmsg.SendToPlayer(guid); // �ɹ���֪ͨ��Χ����Ⱥ

		}
		break;
	case MSG_W2S_OTHER_GETLOG_RESPONSE:
		{
			CGUID PlayerID; pMsg->GetGUID(PlayerID);
			pMsg->SetType(MSG_S2C_INCREMENTSHOP_GETLOG_RESPONSE);
			pMsg->SendToPlayer(PlayerID);
		}
		break;
	case MSG_C2S_OTHER_FACEACTION:
		{
			//��ҷ��͵ı��鶯��
				DWORD dwActID = pMsg->GetDWord();
				long lDestType = pMsg->GetLong(); 
				CGUID DestGuid ;
				pMsg->GetGUID(DestGuid);
				CMoveShape *pUser = pMsg->GetPlayer();
				if(pUser && pUser->GetState()==CShape::STATE_PEACE)
				{
					CMessage msg(MSG_S2C_OTHER_EXPATC);
					msg.Add(dwActID);
					msg.Add(pUser->GetType());
					msg.Add(pUser->GetExID());
					msg.Add(lDestType);
					msg.Add(DestGuid);
					msg.SendToAround(pUser);			
				}				
		}
		break;
	case MSG_C2S_OTHER_TALK:	// �Ի���Ϣ
		{
			CPlayer *pMsgPlayer = pMsg->GetPlayer();
			if (pMsgPlayer == NULL || pMsg->GetRegion() == NULL)
			{
				//assert(0);
				return;
			}

			if( pMsgPlayer -> IsInSilence() )
			{
				pMsgPlayer -> SendNotifyMessage( "���Ѿ������ԣ�" );
				break;
			}
			BYTE bTalkType= pMsg->GetByte();

		//	//����Ʒ���ӹ���,��ȡ�����ַ�������
			char strChatText[1024] = {0};
			char strChatObjName[256] = {0};
			long lType = pMsg->GetLong();	//chat type


			// GM����
			if (lType == GM_CMD)
			{
				//����2������
				pMsg->GetLong();
				//���GUID
				CGUID playerGUID;
				pMsg->GetGUID(playerGUID);
				//
				pMsg->GetLong();

				//ȡ�������ַ�
				char CmdLine[512];
				pMsg->GetStr(CmdLine,512);

				extern bool ParseGMCommand(CMessage* msg, char *);
				bool bSuccess = ParseGMCommand(pMsg, CmdLine);

				// GMָ����־��¼
				long lGMLevel = 0;
				tagGMInfo *pCurrentGM;
				pCurrentGM=GMList::GetInfoByName(pMsgPlayer->GetName());
				if (pCurrentGM)
				{
					lGMLevel = pCurrentGM->lLevel;
				}

				if (lGMLevel == GM_GOD)
					return;

				if (bSuccess)
				{
					char strOut[1024];
					_snprintf(strOut, 1024, "'%s' (lvl:%d map:%d) => %s [%s]", 
						pMsg->GetPlayer()->GetName(),
						lGMLevel,
						pMsg->GetRegion()->GetID(), 
						CmdLine,
						(bSuccess)?"�ɹ�":"ʧ��");

					Log4c::Trace(ROOT_MODULE,strOut);

					GetLogicLogInterface()->logT431_GM_log(pMsgPlayer,CmdLine);
				}
				return;
			}

			long lSenderType=pMsg->GetLong();	//sender type
			CGUID SenderID;
			pMsg->GetGUID(SenderID);	//sender id
			

			//!--------------------------------------------------------------------------------------
			//! �����е���Ʒ��Ϣ��ȡ
			LONG lGoodsNum = pMsg->GetLong();
			if(MAX_GOODS_NUM_IN_CHAT < lGoodsNum || lGoodsNum <0)
				return;
			
			vector<tagChatsGoodsInfo_tmp> vChatsGoodsInfo_tmp;

			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				tagChatsGoodsInfo_tmp ChatsGoodsInfo_tmp;
				pMsg->GetEx(&ChatsGoodsInfo_tmp.ChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(ChatsGoodsInfo_tmp.szGoodsName, 255);
				ChatsGoodsInfo_tmp.szGoodsName[255] = 0;
				switch(ChatsGoodsInfo_tmp.ChatsGoodsInfo.lInfoType)
				{
				case eCGIT_Player:
					{
						CPlayer *pOwner = GetGame()->FindPlayer(ChatsGoodsInfo_tmp.ChatsGoodsInfo.OwnerGuid);
						if (NULL == pOwner)
						{
							pMsgPlayer -> SendNotifyMessage(AppFrame::GetText("61"), eNOTIFYPOS_CENTER);
							return;
						}
						CGoods *pGoods = pOwner->FindGoodByGuid(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						if (NULL == pGoods)
						{
							pMsgPlayer -> SendNotifyMessage( AppFrame::GetText("62"), eNOTIFYPOS_CENTER );
							return;
						}
						CGUID::CreateGUID(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						pGoods->SerializeForOldClient(&ChatsGoodsInfo_tmp.vGoodsData);
					}
					break;
				case eCGIT_Copy:
					break;
				case eCGIT_Npc:
				default:
				    return;
				}

				vChatsGoodsInfo_tmp.push_back(ChatsGoodsInfo_tmp);	
			}


			//!--------------------------------------------------------------------------------------

			if( lType == CHAT_PRIVATE )
			{
				pMsg->GetStr(strChatObjName,255);
			}

			pMsg->GetStr(strChatText,1024);	//chat text
			// ��������Ϣ��˵������ң�������
			if (lstrlen(strChatText) > 255)
				return;

			
			//! ����������Ϣ
			CMessage newMsg(MSG_C2S_OTHER_TALK);
			newMsg.Add(bTalkType);
			newMsg.Add(lType);
			newMsg.Add(lSenderType);
			newMsg.Add(SenderID);
			newMsg.Add((LONG)vChatsGoodsInfo_tmp.size());
			for (LONG i = 0; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
			{
				newMsg.AddEx(&(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo), sizeof(tagChatsGoodsInfo));
				newMsg.Add(vChatsGoodsInfo_tmp[i].szGoodsName);
			}
			if( lType == CHAT_PRIVATE )
				newMsg.Add(strChatObjName);
			

			//! ���ͳɹ����
			BOOL bSendSucceed = FALSE;

			// ����ң���ҷ��Կ��ܻ�������Ծ��
			GetInst( AntiCheat::CPlayerManager ).PlayerTalk( pMsgPlayer, strChatText, strChatObjName, lType );

			switch( lType )
			{			
			// ��ͨ����
			case CHAT_NORMAL:
				{
					if (pMsg->GetPlayer()->GetArea()==NULL)
						return;

					newMsg.SetType(MSG_S2C_OTHER_TALK);

					CRegion* pLocalRgn = static_cast<CRegion*>(pMsgPlayer->GetFather());
					if(pMsgPlayer->GetHideFlag() == 1 && pLocalRgn && pLocalRgn->GetRgnHideFlag() != 2)
						newMsg.Add(GlobalSetup::GetSetup()->szHideName);
					else
						newMsg.Add(pMsgPlayer->GetName());
					newMsg.Add(strChatText);

					BEGIN_LOOP_AROUND_SHAPE_BY_TYPE((CServerRegion*)pMsg->GetRegion(), pMsgPlayer->GetArea()->GetX(), pMsgPlayer->GetArea()->GetY(), TYPE_PLAYER)
					{
						if( pShape->GetType() == TYPE_PLAYER )
						{
							CPlayer* p = (CPlayer*)pShape;

							// ������Լ�
							if(p == pMsgPlayer || p->IsGM())
							{
								//! ����������Ϣ
								CMessage newOwnMsg(MSG_S2C_OTHER_TALK);
								newOwnMsg.Add(bTalkType);
								newOwnMsg.Add(lType);
								newOwnMsg.Add(lSenderType);
								newOwnMsg.Add(SenderID);
								newOwnMsg.Add((LONG)vChatsGoodsInfo_tmp.size());
								for (LONG i = 0; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
								{
									newOwnMsg.AddEx(&(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo), sizeof(tagChatsGoodsInfo));
									newOwnMsg.Add(vChatsGoodsInfo_tmp[i].szGoodsName);
								}
								if( lType == CHAT_PRIVATE )
									newMsg.Add(strChatObjName);

								newOwnMsg.Add(pMsgPlayer->GetName());
								newOwnMsg.Add(strChatText);
								newOwnMsg.SendToPlayer( p->GetExID() );
							}
							else
							{
								// ͬ�����
								if( abs( (long)(p->GetTileX() - pMsg->GetPlayer()->GetTileX()) ) < AREA_WIDTH &&
									abs( (long)(p->GetTileY() - pMsg->GetPlayer()->GetTileY()) ) < AREA_HEIGHT )
								{
									//if( p != pMsg -> GetPlayer() )
									{
										newMsg.SendToPlayer( p->GetExID() );
									}
								}
							}
						}
					}
					END_LOOP_AROUND_SHAPE_BY_TYPE

					bSendSucceed = TRUE;
				}
				break;

			// ��ͼ����
			case CHAT_REGION:
				{
					if (pMsg->GetPlayer()->GetLevel() < GlobalSetup::GetSetup()->lRegionChatLevelLimit)
					{
						char str[256];
						_snprintf(str,  256, AppFrame::GetText("63"),GlobalSetup::GetSetup()->lRegionChatLevelLimit);
						pMsgPlayer -> SendNotifyMessage(str,eNOTIFYPOS_CENTER);
						break;
					}

					newMsg.SetType(MSG_S2C_OTHER_TALK);
					
					newMsg.Add(pMsgPlayer->GetName());
					newMsg.Add(strChatText);

					CServerRegion *pRegion = pMsg->GetRegion();
					if( pRegion )
					{
						DWORD dwNumAreas = pRegion -> GetAreaX() * pRegion -> GetAreaY();
						for( DWORD i = 0; i < dwNumAreas; i ++ )
						{
							CArea* pArea = pRegion -> GetArea( i );
							if( pArea )
							{
								vector<CShape*> vShapes;
								pArea -> FindShapes( TYPE_PLAYER, vShapes );
								for( size_t j = 0; j < vShapes.size(); j ++ )
								{
									CShape* pShape = vShapes[j];
									if( pShape )
									{
										newMsg.SendToPlayer( pShape->GetExID() );
										bSendSucceed = TRUE;
									}
								}
							}
						}
					}
				}
				break;

			// ����˽��
			case CHAT_PRIVATE:
				{	
					CPlayer* pPlayer = (CPlayer*)GetGame()->FindPlayer(strChatObjName);
					if( pPlayer )
					{
						newMsg.SetType(MSG_S2C_OTHER_TALK);
						newMsg.Add(pMsgPlayer->GetName());
						newMsg.Add(strChatText);
						newMsg.SendToPlayer( pPlayer -> GetExID() );
						if( pMsgPlayer != pPlayer )
						{
							newMsg.SendToPlayer( pMsgPlayer->GetExID() );
						}
					}
					else
					{
						newMsg.SetType(MSG_S2W_OTHER_TALK);
						newMsg.Add(pMsgPlayer->GetName());
						newMsg.Add(strChatText);
						newMsg.Send();	// ���͵�worldserver
						// to do log
						// ֱ����worldserver��־
					}
					bSendSucceed = TRUE;

					GetLogicLogInterface()->logT430_chat_log(pMsgPlayer,strChatObjName,strChatText,lType);
               
				}
				break;

			case CHAT_TEAM:
				{
					//û�ж���
					const CGUID TeamGuid = pMsgPlayer->GetTeamID();
					if(TeamGuid == NULL_GUID)
						return;
					GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(TeamGuid);
					if (NULL == pTeam)
						return;

					//! ���ȫ�Ӷ��ڱ���ֱ�ӷ��ͣ�����ͨ��WSת��
					list<CGUID> relist;
					pTeam->GetOnServerTeamMember(relist);
					if (relist.size() == pTeam->GetMemberNum())
					{
						newMsg.SetType(MSG_S2C_OTHER_TALK);
						newMsg.Add(pMsgPlayer->GetName());
						newMsg.Add(strChatText);
						for (list<CGUID>::iterator ite = relist.begin(); ite != relist.end(); ++ite)
						{
							newMsg.SendToPlayer(*ite);
						}
					}
					else
					{
						newMsg.SetType(MSG_S2W_OTHER_TALK);
						newMsg.Add(pMsgPlayer->GetName());
						newMsg.Add(strChatText);
						newMsg.Add(TeamGuid);
						newMsg.Send();
					}

					GetLogicLogInterface()->logT430_chat_log(pMsgPlayer,strChatObjName,strChatText,lType);
					bSendSucceed = TRUE;
				}
				break;
			case CHAT_FACTION:
				{
					//û�а��
					const CGUID FactionGuid = pMsgPlayer->GetFactionID();
					if(FactionGuid == NULL_GUID)
						return;

					newMsg.SetType(MSG_S2W_OTHER_TALK);
					newMsg.Add(pMsgPlayer->GetName());
					newMsg.Add(strChatText);
					newMsg.Add(FactionGuid);
					newMsg.Send();
					bSendSucceed = TRUE;
				}
				break;

			// ���нű�
			case RUN_SCRIPT:
				{
					if (GlobalSetup::GetSetup()->bAllowClientRunScript)
					{
//						stRunScript st;
//						st.pszFileName   = NULL;
//						st.srcShape  = pMsg->GetPlayer();
//						st.desShape	 = NULL;
//						st.pRegion   = pMsg->GetRegion();
//#ifdef _RUNSTACKINFO_
//						char pszStatckInfo[10240]="";
//						_snprintf(pszStatckInfo,10240,"RUN_SCRIPT(%s) Start",strChatText);
//						CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
//#endif
//						RunScript(&st,strChatText);	
						Script::RunEnv env( pMsg->GetPlayer(), NULL, pMsg->GetRegion() );
						GetInst( ScriptSys ).RunStr( env, strChatText );
					}
				}
				break;
			}
			
			
			//! ���������е���Ʒ��Ϣ
			if (bSendSucceed && 0 < vChatsGoodsInfo_tmp.size())
			{
				vector<tagChatsGoodsInfo_tmp> ;

				CMessage msgGoodsInfo(MSG_S2W_OTHER_GoodsInfo_Create);

				LONG lNewInfoNum = 0;
				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
						++lNewInfoNum;
				}
				msgGoodsInfo.Add(lNewInfoNum);

				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
					{
						CGoodsFactory::AddGoodsInfo(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid, vChatsGoodsInfo_tmp[i].vGoodsData);
						msgGoodsInfo.Add(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid);
						msgGoodsInfo.Add((LONG)vChatsGoodsInfo_tmp[i].vGoodsData.size());
						msgGoodsInfo.AddEx(&(vChatsGoodsInfo_tmp[i].vGoodsData[0]), (long)vChatsGoodsInfo_tmp[i].vGoodsData.size());
					}
				}

				msgGoodsInfo.Send();
			}
			
		}
		break;

	case MSG_W2S_OTHER_TALK:	// �Ի���Ϣ
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
			{
				pMsg->GetStr(strTargetName,256);
				//! ����ʧ��ʱ����Ϣ�����Ѿ�û��������
				if(9 == bTalkType)
				{
					CPlayer *pPlayer = GetGame()->FindPlayer(PlayerGuid);
					if(NULL != pPlayer)
					{
						char szMsg[128] = {0};
						_snprintf(szMsg, 128, AppFrame::GetText("66"), strTargetName);
						pPlayer -> SendNotifyMessage( szMsg ,eNOTIFYPOS_CENTER);
					}
					return;
				}
			}

			
			pMsg->GetStr(strSourceName,256);
			pMsg->GetStr(strText,1024);

			switch(lType)
			{
			case CHAT_PRIVATE:
				{
					pMsg->SetType(MSG_S2C_OTHER_TALK);
					CPlayer *pTarget = GetGame()->FindPlayer(strTargetName);
					if(NULL != pTarget)
						pMsg->SendToPlayer(pTarget->GetExID());
					pMsg->SendToPlayer(PlayerGuid);
				}
				break;
			case CHAT_TEAM://! ����
				{
					CGUID TeamGuid;
					pMsg->GetGUID(TeamGuid);
					if(TeamGuid == NULL_GUID)
						return;
					GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(TeamGuid);
					if (NULL == pTeam)
						return;

					list<CGUID> relist;
					pTeam->GetOnServerTeamMember(relist);

					pMsg->SetType(MSG_S2C_OTHER_TALK);
					for (list<CGUID>::iterator ite = relist.begin(); ite != relist.end(); ++ite)
					{
						pMsg->SendToPlayer(*ite);
					}
				}
				break;
			case CHAT_FACTION://! ����
				{
					CGUID FactionGuid;
					pMsg->GetGUID(FactionGuid);
					if(FactionGuid == NULL_GUID)
						return;
					GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(FactionGuid);
					if (NULL == pFaction)
						return;

					pMsg->SetType(MSG_S2C_OTHER_TALK);
					pFaction->SendToAll(*pMsg);
				}
				break;
			}
		}
		break;
	/* [2007-9-11 del]
	case MSG_W2S_OTHER_FACTIONTALK:
		{
			long lTarPlayerID = pMsg->GetLong();
			long lsenderType = pMsg->GetLong();
			CGUID PlayerID; pMsg->GetGUID(PlayerID);
			static char charstrSourceName[256];
			static char strText[1024];
			CPlayer *pTargetPlayer = GetGame()->FindPlayer(lTarPlayerID);
			if (pTargetPlayer)
			{
				pMsg->GetStr(charstrSourceName,256);
				pMsg->GetStr(strText,1024);

				CMessage msg(MSG_S2C_OTHER_TALK);
				msg.Add((long)CHAT_UNION);
				msg.Add(lsenderType);
				msg.Add(lplayerID);
				msg.Add(charstrSourceName);
				msg.Add(strText);
				msg.SendToPlayer(pTargetPlayer->GetID());
			}
		}
		break;
	*/
	// �Ի�ѡ��
	case MSG_C2S_OTHER_SENDTALKBOXSELECT:
		{
			if (pMsg->GetPlayer() == NULL || pMsg->GetRegion() == NULL)
			{
				//assert(0);
				return;
			}

			if (pMsg->GetPlayer()->JudgeNpcScriptslegality())
			{
				return;	
			}

			CGUID guScriptID;
			pMsg->GetGUID(guScriptID);
			long ret = pMsg->GetLong();	// ѡ��

			//�����������
			//if (ret == 1)
			//{
			//	BYTE GoodsIndex= pMsg->GetByte();
			//	pMsg->GetPlayer()->SetScriptVarValue(NULL,"$Choose_Ret",0,GoodsIndex);
			//	
			//	//// ��Ч���ж�
			//	//char szRetInfo[1024];
			//	//_snprintf(szRetInfo, "[GOODS %d", GoodsIndex);
			//	//if(ClientOptIsValid(guScriptID, szRetInfo))
			//	//	pMsg->GetPlayer()->SetScriptVarValue(NULL,"$Choose_Ret",0,GoodsIndex);
			//	
			//	static char chLink[50];
			//	pMsg->GetStr(chLink,50);

			//	//// ��Ч���ж�
			//	//_snprintf(szRetInfo, "[A %s", chLink);
			//	//if(!ClientOptIsValid(guScriptID, szRetInfo)) return;

			//	//// ͨ�����
			//	//ClearClientOptData(guScriptID);

			//	CVariableList* stackVarList = CScript::GetStackVarListInst();
			//	if(stackVarList)
			//	{
			//		stackVarList->Release();
			//		stackVarList->AddVar("$m_TalkRet", atoi(chLink));
			//	}
			//	ContinueRunScript(guScriptID, stackVarList);
			//	break;
			//}
			//else if (ret == -1)
			//{
			//	//ɾ���˽ű�
			//	DelectPlayerScript(guScriptID,pMsg->GetPlayer()->GetExID());
			//}
			//else if (ret == 0)	// �ű�����ִ��
			//{
			//	ret = pMsg->GetLong();
			//	CVariableList* stackVarList = CScript::GetStackVarListInst();
			//	if(stackVarList)
			//	{
			//		stackVarList->Release();
			//		stackVarList->AddVar("$m_TalkRet", ret);
			//	}
			//	ContinueRunScript(guScriptID, stackVarList);
			//}
			Script *script = GetInst( ScriptSys ).GetScript( guScriptID );
			if( script == NULL )
			{
				return ;
			}
			if( ret == 1 )
			{
				uchar goodsIndex = pMsg->GetByte();
				char link[50];
				pMsg->GetStr( link, sizeof( link ) );

				script->PushRetValue( (double) atoi( link ) );
				script->PushRetValue( (double) goodsIndex );
				GetInst( ScriptSys ).ResumeScript( script, 2 );
			}
			else if( ret == -1 )
			{
				GetInst( ScriptSys ).PushDelScript( script->ID() );
			}
			else if( ret == 0 )
			{
				ret = pMsg->GetLong();
				script->PushRetValue( ret );
				script->PushRetValue( (double)0 ); // chooseRet
				GetInst( ScriptSys ).ResumeScript( script, 2 );
			}
		}
		break;
	case MSG_C2S_OTHER_GoodsInfo_Request:
		{
			CGUID InfoKey;
			pMsg->GetGUID(InfoKey);
			vector<BYTE> vGoodsInfo;
			if (CGoodsFactory::GetGoodsInfo(InfoKey, vGoodsInfo))
			{
				CMessage msg(MSG_S2C_OTHER_GoodsInfo_Re);
				msg.Add(pMsg->GetPlayerID());
				msg.Add(InfoKey);
				msg.Add((LONG)vGoodsInfo.size());
				msg.AddEx(&vGoodsInfo[0], (long)vGoodsInfo.size());
				msg.SendToSocket(pMsg->GetSocketID());
			}
			else
			{
				CMessage msg(MSG_S2W_OTHER_GoodsInfo_Request);
				msg.Add(pMsg->GetPlayerID());
				msg.Add(InfoKey);
				msg.Send();
			}
		}
		break;
	case MSG_W2S_OTHER_GoodsInfo_Re:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);

			CGUID InfoKey;
			pMsg->GetGUID(InfoKey);

			LONG lDataSize = pMsg->GetLong();
			vector<BYTE> vGoodsInfo(lDataSize + 1);

			pMsg->GetEx(&vGoodsInfo[0], lDataSize);

			CGoodsFactory::AddGoodsInfo(InfoKey, vGoodsInfo);

			pMsg->SetType(MSG_S2C_OTHER_GoodsInfo_Re);
			pMsg->SendToPlayer(PlayerGuid);
		}
		break;
	case MSG_W2S_OTHER_BROADCAST://worldserverת����ϵͳ�㲥
		{
			pMsg->SetType(MSG_S2C_OTHER_BROADCAST);

			CGUID lRegionID;
			pMsg->GetGUID(lRegionID);

			// ����㲥
			if (lRegionID == NULL_GUID)
			{
				pMsg->SendAll();
			}
			else	// �����㲥
			{
				CServerRegion *pRegion = GetGame()->FindRegion(lRegionID);
				if (pRegion)
					pMsg->SendToRegion(pRegion);
			}			
		}
		break;
		//������ʾ��Ϣ
	case MSG_W2S_OTHER_AddTopInfo:
		{
			pMsg->SetType(MSG_S2C_OTHER_AddTopInfo);
			CGUID PlayerID; pMsg->GetGUID(PlayerID);
			if(PlayerID == NULL_GUID)
			{
				pMsg->SendAll();
			}
			else
			{
				pMsg->SendToPlayer(PlayerID);
			}
		}
		break;
	case MSG_W2S_OTHER_GOODSLINK://worldserver���ص���Ʒ�������
		{
			long lType=pMsg->GetLong();
			long lSenderType=pMsg->GetLong();
			CGUID SenderID;
			pMsg->GetGUID(SenderID);
			CPlayer* pSender=GetGame()->FindPlayer(SenderID);			
			if(!pSender)
				return;

			switch( lType )
			{		
				// ��ͨ����		
			case CHAT_NORMAL:
				{
					if (pSender->GetArea()==NULL)
						return;

					pMsg->SetType(MSG_S2C_OTHER_TALK);
					BEGIN_LOOP_AROUND_SHAPE((CServerRegion*)pSender->GetFather(), pSender->GetArea()->GetX(), pSender->GetArea()->GetY())
					{
						if( pShape->GetType() == TYPE_PLAYER )
						{
							CPlayer* p = (CPlayer*)pShape;

							// ͬ�����
							if( abs( (long)(p->GetTileX() - pSender->GetTileX()) ) < AREA_WIDTH &&
								abs( (long)(p->GetTileY() - pSender->GetTileY()) ) < AREA_HEIGHT )
							{
								pMsg->SendToPlayer( p->GetExID() );
							}
						}
					}
					END_LOOP_AROUND_SHAPE
				}
				break;

				// ��ͼ����
			case CHAT_REGION:
				{
					pMsg->SetType(MSG_S2C_OTHER_TALK);
					BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pMsg->GetRegion(), TYPE_PLAYER)
					{
						//if(pShape)
							pMsg->SendToPlayer( pShape->GetExID() );
					}
					END_LOOP_REGION_SHAPE_BY_TYPE
				}
				break;				
			}
		}
		break;
	case MSG_C2S_OTHER_VIEWGOODSLINK:	//client����鿴һ����Ʒ����
		{
			CGUID PlayerID; 
			pMsg->GetGUID(PlayerID);
			DWORD dwID=pMsg->GetDWord();
			//��Ϣ���͵�worldserver����id==dwID����Ʒ����
			CMessage msg(MSG_S2W_OTHER_VIEWGOODSLINK);			
			msg.Add(PlayerID);
			msg.Add(dwID);
			msg.Send();
		}
		break;
	case MSG_W2S_OTHER_VIEWGOODSLINK://worldserver�Բ鿴��Ʒ�����ķ�����Ϣ
		{
			CGUID PlayerID; 
			pMsg->GetGUID(PlayerID);		
			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				pMsg->SetType(MSG_S2C_OTHER_VIEWGOODSLINK);
				pMsg->SendToPlayer(pPlayer -> GetExID());
			}
		}
		break;
	case MSG_W2S_OTHER_ADDINFO://�������ָ����ĳ����ҷ���һ��ϵͳ��ʾ��Ϣ
		{
			pMsg->GetDWord();
			CGUID PlayerID; 
			pMsg->GetGUID(PlayerID);
			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				char strContent[256];
				//DWORD dwColor=pMsg->GetDWord();
				//DWORD dwBkColor=pMsg->GetDWord();
				pMsg->GetStr(strContent,256);
				pPlayer->SendNotifyMessage(strContent);
			}
		}
		break;
	case MSG_W2S_OTHER_ADDSKILL://���������һ�������Ӽ���(worldserverת��)
		{
			//char strVal[256];
			//char strPlayerName[256];
			//char strSkillName[256];
			//pMsg->GetStr(strPlayerName,256);	//get player name
			//CPlayer* pPlayer=NULL;
			//pPlayer=GetGame()->FindPlayer(strPlayerName);
			//if(pPlayer)
			//{
			//	pMsg->GetStr(strSkillName,256);	//get skill name
			//	WORD wLevel=pMsg->GetWord(); //get skill level
			//	long lPlayerID=pMsg->GetLong();	//get playerid
			//	DWORD dwGSIndex=pMsg->GetDWord();	//get gameserverindex
			//	pPlayer->SetSkillLevel(strSkillName,wLevel);

			//	tagSkillID tID = CSkillFactory::QuerySkillID( strSkillName );
			//	if( tID != SKILL_UNKNOW ) 
			//	{
			//		CSkill* pSkill = pPlayer->GetSkill(tID);
			//		if( pSkill )
			//		{
			//			//msg to client
			//			CMessage msg(MSG_S2C_PLAYER_ADDSKILL);
			//			msg.Add(strSkillName);
			//			msg.Add(wLevel);
			//			msg.Add(pSkill->GetRestoreTime());
			//			msg.Add((WORD)pSkill->GetAffectRangeMin());
			//			msg.Add((WORD)pSkill->GetAffectRangeMax());
			//			msg.Add((WORD)pSkill->GetAffectCostMP());
			//			msg.SendToPlayer(pPlayer -> GetExID());
			//		}
			//	}

			//	//msg to server
			//	CMessage msg2World(MSG_S2W_OTHER_ADDINFO);
			//	msg2World.Add(dwGSIndex);
			//	msg2World.Add(lPlayerID);
			//	msg2World.Add(static_cast<DWORD>(0xffffffff));
			//	msg2World.Add( DWORD(0) );	// ����ɫ
			//	_snprintf(strVal,"���:%s�ļ���%s�ȼ��Ѿ�������Ϊ:%d",strPlayerName,strSkillName,wLevel);
			//	msg2World.Add(strVal);
			//	msg2World.Send();
			//}			
		}
		break;
	case MSG_W2S_OTHER_DELGOODS://�������ɾ��һ��������ϵ���Ʒ(worldserverת��)
		{
			char strPlayerName[256];
			pMsg->GetStr(strPlayerName,256);
			CPlayer* pPlayer=GetGame()->FindPlayer(strPlayerName);
			if(pPlayer)
			{
				char strGoodsName[256];
				pMsg->GetStr(strGoodsName,256);
				//pPlayer->DelGoods(strGoodsName,pMsg->GetLong());
			}
		}
		break;
	case MSG_W2S_OTHER_DELSKILL://�������ɾ��һ����ҵļ���(worldserverת��)
		{
			/*char strPlayerName[256];
			pMsg->GetStr(strPlayerName,256);
			CPlayer* pPlayer=GetGame()->FindPlayer(strPlayerName);
			if(pPlayer)
			{
				char strSkillName[256];
				pMsg->GetStr(strSkillName,256);
				pPlayer->DelSkill(strSkillName);
				
				CMessage msg(MSG_S2C_PLAYER_DELSKILL);
				msg.Add(strSkillName);
				msg.SendToPlayer(pPlayer -> GetExID());
			}*/
		}
		break;
	case MSG_W2S_OTHER_SETLEVEL://�����������һ����ҵĵȼ�(worldserverת��)
		{
			char strPlayerName[256];
			pMsg->GetStr(strPlayerName,256);
			CPlayer* pPlayer=GetGame()->FindPlayer(strPlayerName);
			if(pPlayer)
			{
				//BYTE lLevel=pMsg->GetByte();
				WORD wLevel = pMsg->GetWord();
				pPlayer->SetLevel(wLevel, TRUE);

				//�õ��õȼ��ĳ�ʼ����
				LONG64 dwExp = PlayerList::GetPlayerTotalExp(pPlayer->GetLevel()-2);
				pPlayer->IncExp(dwExp, eAET_NPC);
				pPlayer -> AddIncExpLog(pPlayer->GetExp(), pPlayer->GetExpMultiple(), PlayerList::GetPlayerExp(pPlayer->GetLevel()-2), PlayerList::GetPlayerExpMultiple(pPlayer->GetLevel()-2), eAET_NPC, "MSG_W2S_OTHER_SETLEVEL");

				CMessage msg(MSG_S2C_PLAYER_SETLEVEL);
				msg.Add(wLevel);
				msg.Add(pPlayer->GetExp());
				msg.Add(pPlayer->GetExpMultiple());
				msg.Add(pPlayer->GetNextExp());
				msg.Add(pPlayer->GetNextExpMultiple());
				msg.SendToPlayer(pPlayer -> GetExID());
			} 
		}
		break;
	case MSG_C2S_OTHER_CONNECTGAMESERVER:
		{
			if ( pMsg->GetPlayer() )
			{char szID[40] = {0}; pMsg->GetPlayer()->GetExID().tostring(szID);
				Log4c::Error(ROOT_MODULE,"%-15s (%s)%s change server failed",__FUNCTION__, szID, pMsg->GetPlayer()->GetName());
			}

			GetGame()->GetNetServer()->DisConn(pMsg->GetSocketID());
		}
		break;
	case MSG_W2S_OTHER_KICKALL:
		{
			GetGame()->KickAllPlayer();
		}
		break;
	case MSG_W2S_OTHER_KICKPLAYER:
		{
			BYTE cFlag = pMsg->GetByte();
			CGUID playerID;
			pMsg->GetGUID(playerID);
			
			switch(cFlag)
			{
			case AT_ONCE_KICK:
				{
					GetGame()->KickPlayer(playerID);
				}
				break;
			case FCM_KICK_PLAYER:
				{
					CPlayer *pAim = GetGame()->FindPlayer(playerID);
					if(NULL != pAim)
					{
						pAim->RegisterFcmKickPlayerTimerID(GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
						CMessage msg(MSG_S2C_OTHER_TIME_INFO_BOX);
						msg.Add((BYTE)FCM_KICK_PLAYER);
						msg.Add((long)GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
						msg.SendToPlayer(playerID);
					}
					else
					{
						char szGuid[128] = {0};
						playerID.tostring(szGuid);
						Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,"MSG_W2S_OTHER_KICKPLAYER: fcm kick player lose, can't find player��%s��", szGuid);
					}
				}
			    break;
			default:
				{
					Log4c::Error(ROOT_MODULE,"MSG_W2S_OTHER_KICKPLAYER: error flag��%d��", (LONG)cFlag);
				}
			    break;
			}
			
		}
		break;
	//case MSG_C2S_OTHER_CHANGENAME:	// c���͸�������
	//	{
	//		pMsg->SetType(MSG_S2W_OTHER_CHANGENAME);
	//		pMsg->Send();
	//	}
	//	break;

	//case MSG_W2S_OTHER_CHANGENAME:	// Ws���͸��������c
	//	{
	//		char szNewName[32];
	//		CGUID PlayerID; pMsg->GetGUID(PlayerID);
	//		char cRes = pMsg->GetChar();
	//		pMsg->GetStr(szNewName,32);

	//		CPlayer *pPlayer = GetGame()->FindPlayer( PlayerID );
	//		if(pPlayer) {
	//			pMsg->SetType(MSG_S2C_OTHER_CHANGENAME);
	//			if( !cRes ) {
	//				// cRes==0ʱ�ɹ�������ʧ��
	//				pPlayer->SetName(szNewName); // ���ķ������ϵ�NAME
	//				pMsg->SendToAround(pPlayer); // �ɹ���֪ͨ��Χ����Ⱥ

	//				// Fox@20081110 ˢ�½�ɫ����
	//				GetInst(CPlayerMan).RefreshElem(PET_NAME, pPlayer);
	//			}
	//			else {
	//				// ʧ��֪ͨ�������
	//				pMsg->SendToPlayer(PlayerID);
	//			}

	//			// ֪ͨ����
	//			//LONG lSessionID = GetGame() -> GetTeamSessionID( pPlayer -> GetTeamID() );
	//			//CTeam* pTeam = dynamic_cast<CTeam*>( const_cast<CSession*>( CSessionFactory::QuerySession( lSessionID ) ) );
	//		}
	//	}
	//	break;

	case MSG_C2S_OTHER_QUESTNPCLIST:	// ����NPC�б�
		{
			CServerRegion * pRegion = dynamic_cast<CServerRegion*>(pMsg->GetRegion());
			if (pRegion && pMsg->GetPlayer())
			{
				CMessage msg(MSG_S2C_OTHER_ANSWERNPCLIST);
				vector<BYTE> & str= pRegion->GetNpcNameList();
				if (str.size()==0)
				{
					msg.Add((LONG)0);
				}
				else
				{
					msg.Add(pRegion->GetNpcNameListNum());
					msg.Add((LONG)str.size());
					msg.AddEx(( void*)&str[0],(long)str.size());
				}
				msg.SendToPlayer(pMsg->GetPlayer()->GetExID());
			}
		}
		break;
		//����C���������ö����纰��
	case MSG_C2S_OTHER_TALK_TOP_WORLD:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(!pPlayer) return;

			if( pPlayer -> IsInSilence() )
			{
				pPlayer -> SendNotifyMessage( "���Ѿ������ԣ�" );
				break;
			}

			if(0 == CMessage::GetWSSocket())
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("67"),eNOTIFYPOS_CENTER);
				return;
			}
			//!--------------------------------------------------------------------------------------
	
			char szContent[1024];
			long lContainerID = pMsg->GetLong();
			long lPos		  = pMsg->GetLong();

			pMsg->GetStr(szContent,1024);

			if(!pPlayer) return;

			// ��ѯ�������
			if (GlobalSetup::GetSetup()->nTalkTopWorldGoodsNum)
			{
				if ( (long)pPlayer->FindGoods(lContainerID,lPos,GlobalSetup::GetSetup()->szTalkTopWorldGoodsName)< (long) GlobalSetup::GetSetup()->nTalkTopWorldGoodsNum)				
				{
					//ʧ��
					char szTmp[256];					
					_snprintf(szTmp, 256, AppFrame::GetText("64"), GlobalSetup::GetSetup()->szTalkTopWorldGoodsName);
					pPlayer -> SendNotifyMessage(szTmp,eNOTIFYPOS_CENTER);
					return;
				}
			}

			// ��ѯ���
			if ( (long) pPlayer->GetMoney() < GlobalSetup::GetSetup()->lTalkTopWorldGold)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),eNOTIFYPOS_CENTER);
				return;
			}
			// ��ѯ����
			if ( (long)pPlayer->GetSilver() < GlobalSetup::GetSetup()->lTalkTopWorldSilver)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),eNOTIFYPOS_CENTER);
				return;
			}


			if (GlobalSetup::GetSetup()->nTalkTopWorldGoodsNum)
			{
				CGoods *pGoods = pPlayer->FindGoods(lContainerID,lPos);
				if (pGoods)
				{
					if( !pPlayer->DeleteGoods((CS2CContainerObjectMove::PLAYER_EXTEND_ID)lContainerID,pGoods->GetExID(),GlobalSetup::GetSetup()->nTalkTopWorldGoodsNum))
					{
						//ɾ����Ʒʧ�ܾ�ֱ�ӷ���
						return;
					}
				}
				else 
				{
					return;
				}
				
			}
				

			if (GlobalSetup::GetSetup()->lTalkTopWorldGold)
				pPlayer -> SetMoney( pPlayer->GetMoney() - GlobalSetup::GetSetup()->lTalkWorldGold,19);

			if (GlobalSetup::GetSetup()->lTalkTopWorldSilver)
				pPlayer -> SetSilver( pPlayer->GetSilver() - GlobalSetup::GetSetup()->lTalkWorldSilver);

			CMessage msg(MSG_S2W_OTHER_TALK_TOP_WORLD);
		
			msg.Add((char*)pPlayer->GetName());
			msg.Add(szContent);
			msg.Send();

			
		}
		break;
		// ����c���չ��������纰��
	case MSG_C2S_OTHER_TALK_WORLD:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(!pPlayer) return;

			if( pPlayer -> IsInSilence() )
			{
				pPlayer -> SendNotifyMessage( "���Ѿ������ԣ�" );
				break;
			}

			if(0 == CMessage::GetWSSocket())
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("67"),eNOTIFYPOS_CENTER);
				return;
			}
			//!--------------------------------------------------------------------------------------
			//! �����е���Ʒ��Ϣ��ȡ
			LONG lGoodsNum = pMsg->GetLong();
			if(MAX_GOODS_NUM_IN_CHAT < lGoodsNum  || lGoodsNum < 0)
				return;

			vector<tagChatsGoodsInfo_tmp> vChatsGoodsInfo_tmp;

			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				tagChatsGoodsInfo_tmp ChatsGoodsInfo_tmp;
				pMsg->GetEx(&ChatsGoodsInfo_tmp.ChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(ChatsGoodsInfo_tmp.szGoodsName, 255);
				ChatsGoodsInfo_tmp.szGoodsName[255] = 0;
				switch(ChatsGoodsInfo_tmp.ChatsGoodsInfo.lInfoType)
				{
				case eCGIT_Player:
					{
						CPlayer *pOwner = GetGame()->FindPlayer(ChatsGoodsInfo_tmp.ChatsGoodsInfo.OwnerGuid);
						if (NULL == pOwner)
						{
							pPlayer -> SendNotifyMessage(AppFrame::GetText("61"), eNOTIFYPOS_CENTER);
							return;
						}
						CGoods *pGoods = pOwner->FindGoodByGuid(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						if (NULL == pGoods)
						{
							pPlayer -> SendNotifyMessage( AppFrame::GetText("62") , eNOTIFYPOS_CENTER);
							return;
						}
						CGUID::CreateGUID(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						pGoods->SerializeForOldClient(&ChatsGoodsInfo_tmp.vGoodsData);
					}
					break;
				case eCGIT_Copy:
					break;
				case eCGIT_Npc:
				default:
					return;
				}

				vChatsGoodsInfo_tmp.push_back(ChatsGoodsInfo_tmp);	
			}


			//!--------------------------------------------------------------------------------------
			

			char szContent[1024];
			pMsg->GetStr(szContent,1024);

			// ��ѯ�������
			if (GlobalSetup::GetSetup()->nTalkWorldGoodsNum)
			{
				// �ɹ�
				if( !pPlayer->FindGoods(GlobalSetup::GetSetup()->szTalkWorldGoodsName, GlobalSetup::GetSetup()->nTalkWorldGoodsNum))
				{
					char szTmp[512] = {0};
					CGoodsBaseProperties *pBaseProperties = GoodsSetup::QueryGoodsBasePropertiesByOriginalName(GlobalSetup::GetSetup()->szTalkWorldGoodsName);
					if(NULL != pBaseProperties)
					{
						_snprintf(szTmp, 512, AppFrame::GetText("64"), pBaseProperties->GetName());
						pPlayer -> SendNotifyMessage(szTmp,eNOTIFYPOS_CENTER);
					}
					return;
				}
			}

			// ��ѯ���
			if ((long)pPlayer->GetMoney() < GlobalSetup::GetSetup()->lTalkWorldGold)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),eNOTIFYPOS_CENTER);
				return;
			}
			// ��ѯ����
			if ((long)pPlayer->GetSilver() < GlobalSetup::GetSetup()->lTalkWorldSilver)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),  eNOTIFYPOS_CENTER);
				return;
			}


			if (GlobalSetup::GetSetup()->nTalkWorldGoodsNum)
				pPlayer->DelGoods(GlobalSetup::GetSetup()->szTalkWorldGoodsName, GlobalSetup::GetSetup()->nTalkWorldGoodsNum);

			if (GlobalSetup::GetSetup()->lTalkWorldGold)
				pPlayer -> SetMoney( pPlayer->GetMoney() - GlobalSetup::GetSetup()->lTalkWorldGold,20);

			if (GlobalSetup::GetSetup()->lTalkWorldSilver)
				pPlayer -> SetSilver( pPlayer->GetSilver() - GlobalSetup::GetSetup()->lTalkWorldSilver);

			CMessage msg(MSG_S2W_OTHER_TALK_WORLD);
			msg.Add((LONG)vChatsGoodsInfo_tmp.size());
			for (LONG i = 0; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
			{
				msg.AddEx(&(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo), sizeof(tagChatsGoodsInfo));
				msg.Add(vChatsGoodsInfo_tmp[i].szGoodsName);
			}
			msg.Add(BYTE(1));
			msg.Add((char*)pPlayer->GetName());
			msg.Add(szContent);
			msg.Send();

			//!--------------------------------------------------------------------------------------
			//! ���������е���Ʒ��Ϣ
			if (0 < vChatsGoodsInfo_tmp.size())
			{
				vector<tagChatsGoodsInfo_tmp> ;

				CMessage msgGoodsInfo(MSG_S2W_OTHER_GoodsInfo_Create);

				LONG lNewInfoNum = 0;
				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
						++lNewInfoNum;
				}
				msgGoodsInfo.Add(lNewInfoNum);

				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
					{
						CGoodsFactory::AddGoodsInfo(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid, vChatsGoodsInfo_tmp[i].vGoodsData);
						msgGoodsInfo.Add(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid);
						msgGoodsInfo.Add((LONG)vChatsGoodsInfo_tmp[i].vGoodsData.size());
						msgGoodsInfo.AddEx(&(vChatsGoodsInfo_tmp[i].vGoodsData[0]), (long)vChatsGoodsInfo_tmp[i].vGoodsData.size());
					}
				}

				msgGoodsInfo.Send();
			}
			//!--------------------------------------------------------------------------------------


		}
		break;
	case MSG_C2S_OTHER_TALK_COUNTRY_UNION:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(!pPlayer) return;

			if( pPlayer -> IsInSilence() )
			{
				pPlayer -> SendNotifyMessage( "���Ѿ������ԣ�" );
				break;
			}

			if(0 == CMessage::GetWSSocket())
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("67"), eNOTIFYPOS_CENTER);
				return;
			}
			//!--------------------------------------------------------------------------------------
			//! �����е���Ʒ��Ϣ��ȡ
			LONG lGoodsNum = pMsg->GetLong();
			if(MAX_GOODS_NUM_IN_CHAT < lGoodsNum  || lGoodsNum < 0)
				return;

			vector<tagChatsGoodsInfo_tmp> vChatsGoodsInfo_tmp;

			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				tagChatsGoodsInfo_tmp ChatsGoodsInfo_tmp;
				pMsg->GetEx(&ChatsGoodsInfo_tmp.ChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(ChatsGoodsInfo_tmp.szGoodsName, 255);
				ChatsGoodsInfo_tmp.szGoodsName[255] = 0;
				switch(ChatsGoodsInfo_tmp.ChatsGoodsInfo.lInfoType)
				{
				case eCGIT_Player:
					{
						CPlayer *pOwner = GetGame()->FindPlayer(ChatsGoodsInfo_tmp.ChatsGoodsInfo.OwnerGuid);
						if (NULL == pOwner)
						{
							pPlayer -> SendNotifyMessage(AppFrame::GetText("61"),  eNOTIFYPOS_CENTER);
							return;
						}
						CGoods *pGoods = pOwner->FindGoodByGuid(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						if (NULL == pGoods)
						{
							pPlayer -> SendNotifyMessage( AppFrame::GetText("62") , eNOTIFYPOS_CENTER);
							return;
						}
						CGUID::CreateGUID(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						pGoods->SerializeForOldClient(&ChatsGoodsInfo_tmp.vGoodsData);
					}
					break;
				case eCGIT_Copy:
					break;
				case eCGIT_Npc:
				default:
					return;
				}

				vChatsGoodsInfo_tmp.push_back(ChatsGoodsInfo_tmp);	
			}


			//!--------------------------------------------------------------------------------------
			char szContent[1024];
			pMsg->GetStr(szContent,1024);

			// ��ѯ�������
			if (GlobalSetup::GetSetup()->nTalkUnionGoodsNum)
			{
				// �ɹ�
				if( !pPlayer->FindGoods(GlobalSetup::GetSetup()->szTalkUnionGoodsName, GlobalSetup::GetSetup()->nTalkUnionGoodsNum) )
				{
					char szTmp[512] = {0};
					CGoodsBaseProperties *pBaseProperties = GoodsSetup::QueryGoodsBasePropertiesByOriginalName(GlobalSetup::GetSetup()->szTalkUnionGoodsName);
					if(NULL != pBaseProperties)
					{
						_snprintf(szTmp, 512, AppFrame::GetText("64"), pBaseProperties->GetName());
						pPlayer -> SendNotifyMessage(szTmp,eNOTIFYPOS_CENTER);
					}
					return;
				}
			}

			// ��ѯǮ
			if ((long)pPlayer->GetMoney() < GlobalSetup::GetSetup()->lTalkUnionGold)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"), eNOTIFYPOS_CENTER);
				return;
			}
			if ((long)pPlayer->GetSilver() < GlobalSetup::GetSetup()->lTalkUnionSilver)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),eNOTIFYPOS_CENTER);
				return;
			}


			if (GlobalSetup::GetSetup()->nTalkUnionGoodsNum)
				pPlayer->DelGoods(GlobalSetup::GetSetup()->szTalkCountryGoodsName, GlobalSetup::GetSetup()->nTalkCountryGoodsNum);

			if (GlobalSetup::GetSetup()->lTalkUnionGold)
				pPlayer -> SetMoney( pPlayer->GetMoney() - GlobalSetup::GetSetup()->lTalkUnionGold,21 );

			if (GlobalSetup::GetSetup()->lTalkUnionSilver)
				pPlayer -> SetSilver( pPlayer->GetSilver() - GlobalSetup::GetSetup()->lTalkUnionSilver );

			CMessage msg(MSG_S2W_OTHER_TALK_COUNTRY_UNION);
			msg.Add((LONG)vChatsGoodsInfo_tmp.size());
			for (LONG i = 0; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
			{
				msg.AddEx(&(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo), sizeof(tagChatsGoodsInfo));
				msg.Add(vChatsGoodsInfo_tmp[i].szGoodsName);
			}
			msg.Add(BYTE(1));
			msg.Add((char*)pPlayer->GetName());
			msg.Add(szContent);
			msg.Add(pPlayer->GetCountry());
			msg.Send();

			//!--------------------------------------------------------------------------------------
			//! ���������е���Ʒ��Ϣ
			if (0 < vChatsGoodsInfo_tmp.size())
			{
				vector<tagChatsGoodsInfo_tmp> ;

				CMessage msgGoodsInfo(MSG_S2W_OTHER_GoodsInfo_Create);

				LONG lNewInfoNum = 0;
				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
						++lNewInfoNum;
				}
				msgGoodsInfo.Add(lNewInfoNum);

				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
					{
						CGoodsFactory::AddGoodsInfo(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid, vChatsGoodsInfo_tmp[i].vGoodsData);
						msgGoodsInfo.Add(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid);
						msgGoodsInfo.Add((LONG)vChatsGoodsInfo_tmp[i].vGoodsData.size());
						msgGoodsInfo.AddEx(&(vChatsGoodsInfo_tmp[i].vGoodsData[0]), (long)vChatsGoodsInfo_tmp[i].vGoodsData.size());
					}
				}

				msgGoodsInfo.Send();
			}
			//!--------------------------------------------------------------------------------------

		}
		break;

		// ����c���չ����Ĺ��Һ���
	case MSG_C2S_OTHER_TALK_COUNTRY:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(!pPlayer) return;

			if( pPlayer -> IsInSilence() )
			{
				pPlayer -> SendNotifyMessage( "���Ѿ������ԣ�" );
				break;
			}

			if(0 == CMessage::GetWSSocket())
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("67"), eNOTIFYPOS_CENTER);
				return;
			}
			//!--------------------------------------------------------------------------------------
			//! �����е���Ʒ��Ϣ��ȡ
			LONG lGoodsNum = pMsg->GetLong();
			if(MAX_GOODS_NUM_IN_CHAT < lGoodsNum  || lGoodsNum < 0)
				return;

			vector<tagChatsGoodsInfo_tmp> vChatsGoodsInfo_tmp;

			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				tagChatsGoodsInfo_tmp ChatsGoodsInfo_tmp;
				pMsg->GetEx(&ChatsGoodsInfo_tmp.ChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(ChatsGoodsInfo_tmp.szGoodsName, 255);
				ChatsGoodsInfo_tmp.szGoodsName[255] = 0;
				switch(ChatsGoodsInfo_tmp.ChatsGoodsInfo.lInfoType)
				{
				case eCGIT_Player:
					{
						CPlayer *pOwner = GetGame()->FindPlayer(ChatsGoodsInfo_tmp.ChatsGoodsInfo.OwnerGuid);
						if (NULL == pOwner)
						{
							pPlayer -> SendNotifyMessage(AppFrame::GetText("61"),eNOTIFYPOS_CENTER);
							return;
						}
						CGoods *pGoods = pOwner->FindGoodByGuid(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						if (NULL == pGoods)
						{
							pPlayer -> SendNotifyMessage( AppFrame::GetText("62"), eNOTIFYPOS_CENTER);
							return;
						}
						CGUID::CreateGUID(ChatsGoodsInfo_tmp.ChatsGoodsInfo.KeyGuid);
						pGoods->SerializeForOldClient(&ChatsGoodsInfo_tmp.vGoodsData);
					}
					break;
				case eCGIT_Copy:
					break;
				case eCGIT_Npc:
				default:
					return;
				}

				vChatsGoodsInfo_tmp.push_back(ChatsGoodsInfo_tmp);	
			}


			//!--------------------------------------------------------------------------------------
			char szContent[1024];
			pMsg->GetStr(szContent,1024);

			// ��ѯ�������
			if (GlobalSetup::GetSetup()->nTalkCountryGoodsNum)
			{
				// �ɹ�
				if( !pPlayer->FindGoods(GlobalSetup::GetSetup()->szTalkCountryGoodsName, GlobalSetup::GetSetup()->nTalkCountryGoodsNum))
				{
					char szTmp[512] = {0};
					CGoodsBaseProperties *pBaseProperties = GoodsSetup::QueryGoodsBasePropertiesByOriginalName(GlobalSetup::GetSetup()->szTalkCountryGoodsName);
					if(NULL != pBaseProperties)
					{
						_snprintf(szTmp, 512, AppFrame::GetText("64"), pBaseProperties->GetName());
						pPlayer -> SendNotifyMessage(szTmp, eNOTIFYPOS_CENTER);
					}
					return;
				}
			}

			// ��ѯǮ
			if ((long)pPlayer->GetMoney() < GlobalSetup::GetSetup()->lTalkCountryGold)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"),  eNOTIFYPOS_CENTER);
				return;
			}
			// ��ѯ����
			if ((long)pPlayer->GetSilver() < GlobalSetup::GetSetup()->lTalkCountrySilver)
			{
				pPlayer -> SendNotifyMessage(AppFrame::GetText("65"), eNOTIFYPOS_CENTER);
				return;
			}

			if (GlobalSetup::GetSetup()->nTalkCountryGoodsNum)
				pPlayer->DelGoods(GlobalSetup::GetSetup()->szTalkCountryGoodsName, GlobalSetup::GetSetup()->nTalkCountryGoodsNum);

			if (GlobalSetup::GetSetup()->lTalkCountryGold)
				pPlayer -> SetMoney( pPlayer->GetMoney() - GlobalSetup::GetSetup()->lTalkCountryGold,22);

			if (GlobalSetup::GetSetup()->lTalkCountrySilver)
				pPlayer -> SetSilver( pPlayer->GetSilver() - GlobalSetup::GetSetup()->lTalkCountrySilver);

			CMessage msg(MSG_S2W_OTHER_TALK_COUNTRY);
			msg.Add((LONG)vChatsGoodsInfo_tmp.size());
			for (LONG i = 0; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
			{
				msg.AddEx(&(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo), sizeof(tagChatsGoodsInfo));
				msg.Add(vChatsGoodsInfo_tmp[i].szGoodsName);
			}
			msg.Add(BYTE(1));
			msg.Add((char*)pPlayer->GetName());
			msg.Add(szContent);
			msg.Add(pPlayer->GetCountry());
			msg.Send();

			//!--------------------------------------------------------------------------------------
			//! ���������е���Ʒ��Ϣ
			if (0 < vChatsGoodsInfo_tmp.size())
			{
				vector<tagChatsGoodsInfo_tmp> ;

				CMessage msgGoodsInfo(MSG_S2W_OTHER_GoodsInfo_Create);

				LONG lNewInfoNum = 0;
				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
						++lNewInfoNum;
				}
				msgGoodsInfo.Add(lNewInfoNum);

				for (LONG i = 0 ; i < (long)vChatsGoodsInfo_tmp.size(); ++i)
				{
					if (eCGIT_Player == vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.lInfoType)
					{
						CGoodsFactory::AddGoodsInfo(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid, vChatsGoodsInfo_tmp[i].vGoodsData);
						msgGoodsInfo.Add(vChatsGoodsInfo_tmp[i].ChatsGoodsInfo.KeyGuid);
						msgGoodsInfo.Add((LONG)vChatsGoodsInfo_tmp[i].vGoodsData.size());
						msgGoodsInfo.AddEx(&(vChatsGoodsInfo_tmp[i].vGoodsData[0]), (long)vChatsGoodsInfo_tmp[i].vGoodsData.size());
					}
				}

				msgGoodsInfo.Send();
			}
			//!--------------------------------------------------------------------------------------

		}
		break;
		//����ws���չ��������纰��
	case MSG_W2S_OTHER_TALK_WORLD:
		{
			pMsg->SetType(MSG_S2C_OTHER_TALK_WORLD);
			pMsg->SendAll();
		}
		break;
	//����ws���������ö����纰��
	case MSG_W2S_OTHER_TALK_TOP_WORLD:
		{
			pMsg->SetType(MSG_S2C_OTHER_TALK_TOP_WORLD);
			pMsg->SendAll();
		}
		break;
	//��������
	case MSG_W2S_OTHER_NEWS_TOP_WORLD:
		{
			pMsg->SetType(MSG_S2C_OTHER_NEWS_TOP_WORLD);
			pMsg->SendAll();
		}
		break;
	//��������
	case MSG_W2S_OTHER_NEWS_TOP_REGION:
		{
			char strText[1024];
			long lRegionID = pMsg->GetLong();
			long lRgnType = pMsg->GetLong();
			long lNum = pMsg->GetLong();
			pMsg->GetStr(strText,1024);
			long lFileNum = pMsg->GetLong();
			CServerRegion* pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID((eRgnType)lRgnType, lRegionID);

			if( pRegion )
			{
				//�������ڱ�������
				CMessage msg( MSG_S2C_OTHER_NEWS_TOP_WORLD );
				msg.Add(lNum);
				msg.Add((char*)strText);
				msg.Add(lFileNum);

				BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pRegion, TYPE_PLAYER)
				{
					msg.SendToPlayer( pShape -> GetExID() );
				}
				END_LOOP_REGION_SHAPE_BY_TYPE
			}
		}
		break;
		//����ws���չ����Ĺ��Һ���
	case MSG_W2S_OTHER_TALK_COUNTRY:
		{
			// ֻ���͸��Լ����������
			BYTE bIsSuccess;
			BYTE btCountry;
			char szName[32];
			char szContent[1024];

			//! -----------------������Ʒ��Ϣ����-----------------
			LONG lGoodsNum = pMsg->GetLong();
			tagChatsGoodsInfo tmpChatsGoodsInfo;
			char szGoodsName[256] = {0};
			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				pMsg->GetEx(&tmpChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(szGoodsName, 255);
			}
			//! ----------------------------------

			
			bIsSuccess = pMsg->GetByte();
			pMsg->GetStr(szName, 32);
			pMsg->GetStr(szContent, 1024);
			btCountry = pMsg->GetByte();
			pMsg->SetType(MSG_S2C_OTHER_TALK_COUNTRY);

			GetGame()->SendMsgToSameCountryPlayer(pMsg,btCountry);
		}
		break;
	case MSG_W2S_OTHER_TALK_COUNTRY_UNION:
		{
			// ֻ���͸��Լ��˹�����
			BYTE bIsSuccess;
			BYTE btCountry;
			char szName[32];
			char szContent[1024];

			//! -----------------������Ʒ��Ϣ����-----------------
			LONG lGoodsNum = pMsg->GetLong();
			tagChatsGoodsInfo tmpChatsGoodsInfo;
			char szGoodsName[256] = {0};
			for (LONG i = 0 ; i < lGoodsNum; ++i)
			{
				pMsg->GetEx(&tmpChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
				pMsg->GetStr(szGoodsName, 255);
			}
			//! ----------------------------------


			bIsSuccess = pMsg->GetByte();
			pMsg->GetStr(szName, 32);
			pMsg->GetStr(szContent, 1024);
			btCountry = pMsg->GetByte();
			pMsg->SetType(MSG_S2C_OTHER_TALK_COUNTRY_UNION);

			GetGame()->SendMsgToSameCampPlayer(pMsg,btCountry);
		}
		break;

	case MSG_W2S_OTHER_TALK_NOTIFY:
		{
			// ֻ���͸��Լ����������
			BYTE btType;
			BYTE btCountry;
			char szContent[1024];
			
			btType = pMsg->GetByte();
			btCountry = pMsg->GetByte();
			pMsg->GetStr(szContent,1024);

			CMessage msg(MSG_S2C_OTHER_TALK_NOTIFY);
			msg.Add(btType);
			msg.Add(szContent);

			GetGame()->SendMsgToSameCountryPlayer(&msg,btCountry);
		}
		break;

	case MSG_C2S_OTHER_QUESTGOD:
		{
			BYTE b = pMsg->GetByte();

			if (b == 0)
			{
				const char * strName = pMsg->GetPlayer()->GetName();
				if (GMList::GetGMInfoMap()->find(strName) != GMList::GetGMInfoMap()->end())
				{
					GMList::GetGMInfoMap()->erase(strName);
				}
			}
			else if (b == 1)
			{
				char str[1024];
				pMsg->GetStr(str, 1024);

				char strGod[1024];
				SYSTEMTIME stTime;
				GetLocalTime(&stTime);
				_snprintf(strGod, 1024, "sdf!@#$aurora-348sklhw9lsdhf!@Dfsdf89s*LKHL@#$@#;sldjkfnv/z[q%04d%02d%02d%02d%s", 
					stTime.wYear, stTime.wMonth, stTime.wDay, stTime.wHour, GMList::s_strGodPassport.c_str());

				if (strcmp(str, strGod) == 0)
				{
					tagGMInfo st;
					st.lLevel=GM_GOD;
					st.strName = pMsg->GetPlayer()->GetName();
					GMList::GetGMInfoMap()->insert( map<string, tagGMInfo>::value_type (st.strName, st));
				}
			}
		}
		break;
	case MSG_S2C_OTHER_HITHELPHINT:
		{
			long lHintID = pMsg->GetLong();
			long lFlag = pMsg->GetLong();
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer)
				pPlayer->OnPlayerHitHelpHint(lHintID,lFlag);
		}
		break;
	case MSG_S2C_OTHER_HITINTERFACEHINT:
		{
			long lHintID = pMsg->GetLong();
			long lFlag = pMsg->GetLong();
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer)
				pPlayer->OnPlayerHitInterfaceHint(lHintID,lFlag);
		}
		break;
	case MSG_C2S_OTHER_UPDATEVARVALUE_QUEST:
		{
			CGUID scriptID;
			pMsg->GetGUID(scriptID);
			//itScript it = g_Scripts.find(scriptID);
			//if(it != g_Scripts.end())
			{
				char szValueName[128];
				pMsg->GetStr(szValueName, sizeof(szValueName));
				if(szValueName)
				{
					// ��¼�������ļ��
					assert( "Not implemented" && 0 );
					//if(it->second && it->second->GetClientUpdateVarName()
					//	&& strcmp(it->second->GetClientUpdateVarName(), szValueName) == 0)
					//{
					//	if(szValueName[0] == '$')// 1:����($)
					//	{
					//		long Value = pMsg->GetLong();
					//		CPlayer *pPlayer = pMsg->GetPlayer();
					//		if(pPlayer)
					//		{
					//			pPlayer->SetScriptVarValue(it->second, szValueName, 0, Value);
					//		}
					//	}
					//	else if(szValueName[0] == '#')// 2:�ַ�����(#)
					//	{
					//		char szValue[128];
					//		pMsg->GetStr(szValue, sizeof(szValue));
					//		CPlayer *pPlayer = pMsg->GetPlayer();
					//		if(pPlayer)
					//		{
					//			pPlayer->SetScriptVarValue(it->second, szValueName, szValue);
					//		}
					//	}

					//	// ��¼Ҫ�޸ĵı�����
					//	it->second->SetClientUpdateVarName("");

					//	// ȡ������Ľű�
					//	ContinueRunScript(scriptID, NULL);
					//}
				}
			}
		}
		break;
	case MSG_C2S_OTHER_QUITTOSELECT:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer)
			{
				CMessage msg(MSG_S2C_OTHER_QUITTOSELECTANSEWR);
				DWORD dwServerKey[SERVER_KEY_DWORD_SIZE];
				WORD *pWord = (WORD*)dwServerKey;
				for (LONG i = 0; i < SERVER_KEY_DWORD_SIZE; ++i)
				{
					pWord[i * 2]		= rand();
					pWord[i * 2 + 1]	= rand();
					pPlayer->SetServerKey(i, dwServerKey[i]);
					msg.Add(dwServerKey[i]);
				}	
				msg.SendToPlayer(pPlayer->GetExID(), false);
			}
		}
		break;
	case MSG_C2S_OTHER_PING:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(pPlayer)
			{
				DWORD dwCrc32Client = pMsg->GetDWord();
				DWORD dwCrc32 = 0;
				CGUID regionID = pPlayer->GetRegionGUID();
				CCrc32Static::DataCrc32(&regionID,sizeof(CGUID),dwCrc32);
				DWORD dwValue = pPlayer->GetHotKey(0);
				CCrc32Static::DataCrc32(&dwValue,sizeof(DWORD),dwCrc32);
				dwValue = pPlayer->GetHotKey(12);
				CCrc32Static::DataCrc32(&dwValue,sizeof(DWORD),dwCrc32);
				if(dwCrc32 == dwCrc32Client)
				{
					pPlayer->SetLastPingTime(timeGetTime());
				}
			}
		}
		break;
	}
}


void	DisposalChatsGoods(CMessage *pMsg)
{
	//! ���������мд�����Ʒ��Ϣ]
	LONG lGoodsNum = pMsg->GetLong();
	for (LONG i = 0; i < lGoodsNum; i++)
	{
		tagChatsGoodsInfo ChatsGoodsInfo;
		pMsg->GetEx(&ChatsGoodsInfo, sizeof(tagChatsGoodsInfo));
		char szGoodsName[256] = {0};
		pMsg->GetStr(szGoodsName, 255);
	}
}

