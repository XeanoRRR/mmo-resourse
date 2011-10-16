#include "StdAfx.h"
#include "../GlobalRgnManager.h"
#include "../WorldRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ӦGM��Ϣ
void OnGMMessage(CMessage* pMsg)
{
    CGUID PlayerGuid_GM;
    pMsg->GetGUID(PlayerGuid_GM);

    switch(pMsg->GetType())
    {
    case MSG_S2W_GM_QUERY_PLAYERCOUNT://��GameServer���������û�����
        {
			CGUID scriptID;
			pMsg->GetGUID( scriptID );
            long nCounts = GetGame().GetMapPlayerCount();
            CMessage msg(MSG_W2S_GM_ANSWER_PLAYERCOUNT);
            msg.Add(PlayerGuid_GM);
            msg.Add(nCounts);
            msg.Add( scriptID );
            msg.SendToSocket(pMsg->GetSocketID());
        }
        break;
    case MSG_S2W_GM_KICKPLAYER:
        /**## ��ĳ��������� */
        /**## MSG_S2W_GM_KICKPLAYER ��Ϣ��ʽ�� */
        /**## long SenderID, char* strPlayer */
        /**## Last updated by Jar@2004-4-9 */
        {
            char strPlayer[256];
            ZeroMemory ( strPlayer, 256 );
            /**## ��ȡ���ߵ��û���ɫ�� */
            pMsg->GetStr( strPlayer, 256);

            /**## ��ȡ���ߵ��û���ɫ�������׽��� */
            const CGUID& PlayerGuid = GetGame().GetOnlinePlayerIDByName( strPlayer );
            long lNumber = GetGame().GetGameServerNumber_ByPlayerID( PlayerGuid );
            if( lNumber )
            {
                /**## �򵥵���һ��ת�� */
                pMsg->SetType( MSG_W2S_GM_KICKPLAYER );
                pMsg->SendToGS( lNumber );
            }
            else 
            {
                long lGameServer = GetGame().GetGameServerNumber_ByPlayerID( PlayerGuid_GM );
                if( lGameServer )
                {
                    CMessage msg( MSG_W2S_GM_ANSWER_KICKPLAYER );
                    msg.Add( PlayerGuid_GM );
                    msg.Add( static_cast<char>(0) );
                    msg.Add( strPlayer );
                    msg.SendToGS( lGameServer );
                }
            }
        }
        break;
    case MSG_S2W_GM_ANSWER_KICKPLAYER:
        /**## ��Ϸ��������Ӧ�������� */
        /**## MSG_W2S_GM_ANSWER_KICKPLAYER ��ʽ */
        /**## long lGMPlayer, char bSucceed=[1|0], char* pKickedPlayerName */
        /**## MSG_S2W_GM_ANSWER_KICKPLAYER ��Ϣ��ʽ��*/
        /**## long lGMPlayer, char bSucceed=[1|0], char* pKickedPlayerName */
        /**## Last updated by Jar@2004-4-9 */
        {
            long lGameServer = GetGame().GetGameServerNumber_ByPlayerID( PlayerGuid_GM );
            if( lGameServer )
            {
                /**## �򵥵���һ��ת�� */
                pMsg->SetType( MSG_W2S_GM_ANSWER_KICKPLAYER );
                pMsg->SendToGS( lGameServer );
            }
        }
        break;
    case MSG_S2W_GM_KICK_ALLPLAYER:
        /**## ��ϷGM���������������ߵ�ָ�� */
        /**## MSG_S2W_GM_KICK_ALLPLAYER ��ʽ */
        /**## long lGMPlayer */
        /**## Last updated by Jar@2004-4-12 */
        {
            CMessage msg(MSG_W2S_GM_KICK_ALLPLAYER);
            msg.Add(PlayerGuid_GM);
            msg.SendToAllGS();
        }
        break;

    case MSG_S2W_GM_KICK_REGIONPLAYER:
        /**## ��ϷGM������һ�����������������ߵ�ָ�� */
        /**## MSG_S2W_GM_KICK_REGIONPLAYER ��ʽ      */
        /**## long lGMPlayer, char* strRegionName    */
        /**## Last updated by Jar@2004-4-12          */
        {
            long lRegionID = pMsg->GetLong();
            long rgnType = pMsg->GetLong();

            //	map<long, CGame::tagRegion>* pRegionList = GetGame().GetRegionListMap();
            //	if( pRegionList )
            {
                //		for( map<long, CGame::tagRegion>::iterator itBegin = pRegionList->begin();
                //			itBegin != pRegionList->end(); itBegin ++ )
                {
                    //			if( itBegin->second.pRegion->GetID() == lRegionID )
                    {
                        long lGameServerNumber = GetGame().
                            GetGameServerNumber_ByRegionID( lRegionID );
                        if( lGameServerNumber )
                        {
                            pMsg->SetType( MSG_W2S_GM_KICK_REGIONPLAYER );
                            pMsg->SendToGS( lGameServerNumber );
                        }
                    }
                }
            }
        }
        break;
    case MSG_S2W_GM_ANSWER_SILENCE_PLAYER:
        {
            long lGameServer = GetGame().GetGameServerNumber_ByPlayerID( PlayerGuid_GM );
            if( lGameServer )
            {
                pMsg->SetType( MSG_W2S_GM_ANSWER_SILENCE_PLAYER );
                pMsg->SendToGS( lGameServer );
            }
        }
        break;
    case MSG_S2W_GM_SILENCE_PLAYER:
        {
            CGUID ScriptID;
            pMsg->GetGUID(ScriptID);
            char strName[256];
            ZeroMemory( strName, 256 );			
            pMsg->GetStr( strName,256 );
            long lTime = pMsg->GetLong();
            CPlayer* pPlayer = GetGame().GetMapPlayer(strName);

            CMessage msg( MSG_W2S_GM_ANSWER_SILENCE_PLAYER );
            msg.Add( PlayerGuid_GM );
            msg.Add(ScriptID);
            if( pPlayer )
            {
                msg.Add((BYTE)1);
                pPlayer->SetSilenceTime( lTime );
                pMsg->SetType( MSG_W2S_GM_SILENCE_PLAYER );
                pMsg->SendToGS( GetGame().GetGameServerNumber_ByPlayerID( pPlayer->GetExID() ) );
            }
            else 
            {		
                msg.Add((BYTE)2);
            }
            msg.SendToSocket(pMsg->GetSocketID());
        }
        break;
    case MSG_S2W_GM_WORLD_MESSAGE:
        /**## �����������������Ϸ����������ȫ�ֹ㲥 */
        /**## MSG_S2W_GM_WORLD_MESSAGE �� MSG_W2S_GM_WORLD_MESSAGE ��ʽ */
        /**## long lGMId, char* pMessage, DWORD dwColor */
        {
            pMsg->SetType( MSG_W2S_GM_WORLD_MESSAGE );
            pMsg->SendToAllGS();
        }
        break;

    case MSG_S2W_GM_COUNTRY_MESSAGE:
        {
            pMsg->SetType( MSG_W2S_GM_COUNTRY_MESSAGE );
            pMsg->SendToAllGS();
        }
        break;
    case MSG_S2W_GM_QUERY_SILENCE_PLAYERS:
        /**## �����ѯ��ҽ����б� */
        /**## ��Ϣ��ʽ: long lGMId */
        {
            pMsg->SetType( MSG_W2S_GM_QUERY_SILENCE_PLAYERS );
            pMsg->SendToAllGS();
        }
        break;
    case MSG_S2W_GM_ANSWER_QUERY_SILENCE_PLAYERS:
        /**## ���������ز�ѯ��ҽ����б� */
        /**## ��Ϣ��ʽ: long lGMId, long lSize, char* pList */
        {
            /**## ת����GM���ڵ�GameServer */
            long lGameServer= GetGame().GetGameServerNumber_ByPlayerID( PlayerGuid_GM );
            if( lGameServer )
            {
                /**## �򵥵���һ��ת�� */
                pMsg->SetType( MSG_W2S_GM_ANSWER_QUERY_SILENCE_PLAYERS );
                pMsg->SendToGS( lGameServer );
            }
        }
        break;
        //case MSG_S2W_GM_QUERY_PLAYERDATA:	//GameServer��ѯ�û�����
        //	{
        //	char chPlayerName[32];
        //	char chPropName[32];
        //	long lPlayerID;
        //	//long lRegionID;
        //	//CString ServerIP;
        //	pMsg->GetStr(chPlayerName,32);
        //	pMsg->GetStr(chPropName,32);

        //	lPlayerID=theApp.GetPlayerID(chPlayerName);


        //	if (0==lPlayerID){	//��Ҳ�����,ֱ�ӷ�����GameServer,��GameServer�Լ������ݿ����
        //			CMessage msg(MSG_W2S_GM_QUERY_PLAYERDATA);
        //			msg.Add(PlayerGuid_GM);
        //			msg.Add(chPlayerName);
        //			msg.Add(chPropName);
        //			msg.Add((BYTE)false);
        //			msg.Add((long)0);
        //			//CMySocket* pSock=theApp.GetPlayerSocket(lPlayerID);
        //			//if(pSock)
        //			msg.SendToSocket(pMsg->GetSocketID());
        //	}
        //	else{
        //		CMessage msg(MSG_GM_QUERY_PLAYERDATA);
        //		msg.Add(PlayerGuid_GM);
        //		msg.Add(lPlayerID);
        //		msg.Add(chPropName);
        //		CMySocket* pSock=theApp.GetPlayerSocket(lPlayerID);
        //		if(pSock)
        //			msg.SendToGS(pSock);
        //	}//else
        //	}//case
        //	break;

        //	case MSG_GM_ANSWER_PLAYERDATA://GameServer�򱾷��������������û�����
        //		//ֻ��Ҫ��ת����GM���ڵ�GameServer
        //		pMsg->SendToGS(theApp.GetPlayerSocket(PlayerGuid_GM));

        //	break;



    case MSG_S2W_GM_QUERY_PLAYERDATA:	//GameServer��ѯ�û�����
        {
            char chPlayerName[256];
            char chPropName[256];

            pMsg->GetStr(chPlayerName,256);
            pMsg->GetStr(chPropName,256);
            long lScriptID = pMsg->GetLong();
            const CGUID& PlayerGuid = GetGame().GetOnlinePlayerIDByName(chPlayerName);           
            CMessage msg(MSG_W2S_GM_QUERY_PLAYERDATA);
            msg.Add(PlayerGuid_GM);
            msg.Add(PlayerGuid);
            msg.Add(chPropName);
            msg.Add((long)pMsg->GetGSID());
            msg.Add(lScriptID);
            msg.SendToAllGS();	
        }//case
        break;
    case MSG_S2W_GM_QUERY_PLAYER:	//GameServer��ѯ�û�����(GUID)
        {
            char szgoodsID[128]; // GUID ��������
            char chPlayerName[256];
            pMsg->GetStr(chPlayerName,256);// player����
            long lScriptID = pMsg->GetLong();// script id
            pMsg->GetStr(szgoodsID, 128);// GUID ��������

            const CGUID& PlayerGuid = GetGame().GetOnlinePlayerIDByName(chPlayerName);           
            CMessage msg(MSG_W2S_GM_QUERY_PLAYER);
            msg.Add(PlayerGuid_GM);
            msg.Add(PlayerGuid);
            msg.Add((long)pMsg->GetGSID());
            msg.Add(lScriptID);
            msg.Add(szgoodsID);
            msg.SendToAllGS();	
        }//case
        break;
    case MSG_S2W_GM_QUERY_PLAYER_P:	//GameServer��ѯ�û�����(GUID)
        {

            char szgoodsID[128]; // GUID ��������
            char chPlayerName[256];
            pMsg->GetStr(chPlayerName,256);// player����
            long lScriptID = pMsg->GetLong();// script id
            pMsg->GetStr(szgoodsID, 128);// GUID ��������

            CGUID PGuid;
            CPlayer* player = GetGame().GetMapPlayer(chPlayerName); 
            if(player)
                PGuid = player->GetPersonalRgnGUID();

            CMessage msg(MSG_W2S_GM_QUERY_PLAYER_P);
            msg.Add(PlayerGuid_GM);
            msg.Add(PGuid);
            msg.Add((long)pMsg->GetGSID());
            msg.Add(lScriptID);
            msg.Add(szgoodsID);
            msg.SendToAllGS();	
        }//case
        break;
    case MSG_S2W_GM_QUERY_PLAYER_PH:	//GameServer��ѯ�û�����(GUID)
        {
            char szgoodsID[128]; // GUID ��������
            char chPlayerName[256];
            pMsg->GetStr(chPlayerName,256);// player����
            long lScriptID = pMsg->GetLong();// script id
            pMsg->GetStr(szgoodsID, 128);// GUID ��������

            CGUID PGuid;
            CPlayer* player = GetGame().GetMapPlayer(chPlayerName); 
            if(player)
                PGuid = player->GetPersonalHouseRgnGUID();

            CMessage msg(MSG_W2S_GM_QUERY_PLAYER_PH);
            msg.Add(PlayerGuid_GM);
            msg.Add(PGuid);
            msg.Add((long)pMsg->GetGSID());
            msg.Add(lScriptID);
            msg.Add(szgoodsID);
            msg.SendToAllGS();	
        }//case
        break;
    case MSG_S2W_GM_QUERY_PLAYER_T:	//GameServer��ѯ�û�����(GUID)
        {
            char szgoodsID[128]; // GUID ��������
            char chPlayerName[256];
            pMsg->GetStr(chPlayerName,256);// player����
            long lScriptID = pMsg->GetLong();// script id
            pMsg->GetStr(szgoodsID, 128);// GUID ��������

            CGUID PGuid;
            CPlayer* player = GetGame().GetMapPlayer(chPlayerName); 
            if(player)
                PGuid = player->GetTeamID();

            CMessage msg(MSG_W2S_GM_QUERY_PLAYER_T);
            msg.Add(PlayerGuid_GM);
            msg.Add(PGuid);
            msg.Add((long)pMsg->GetGSID());
            msg.Add(lScriptID);
            msg.Add(szgoodsID);
            msg.SendToAllGS();	
        }//case
        break;
    case MSG_S2W_GM_ANSWER_PLAYERDATA:	//gameserver��worldserver��ѯ������ݵķ���
        {			
            long lVal=0;
            DWORD dwGameServerIndex=0;				
            lVal=pMsg->GetLong();
            dwGameServerIndex=pMsg->GetDWord();
            long lScriptID = pMsg->GetLong();
            CMessage msg(MSG_W2S_GM_ANSWER_PLAYERDATA);
            msg.Add(PlayerGuid_GM);						
            msg.Add(lVal);
            msg.Add(lScriptID);
            msg.SendToGS(dwGameServerIndex);
        }
        break;

        //gm�������ָ��ĳ����л�����,������Ϣת�������������gameserver	
    case MSG_S2W_GM_CHANGEREGION:
        {
            pMsg->SetType(MSG_W2S_GM_CHANGEREGION);
            pMsg->SendToAllGS();
        }
        break;
    case MSG_S2W_GM_KICKPLAYEREX: //gm��������,��Ϣת�������������gameserver
        {
            pMsg->SetType(MSG_W2S_GM_KICKPLAYEREX);
            pMsg->Add((long)pMsg->GetGSID());
            pMsg->SendToAllGS();
        }
        break;
    case MSG_S2W_GM_QUERY_FINDREGION://GM����һ������,���ݳ���������һ������ID
        {
            char strRegionName[256];
            pMsg->GetStr(strRegionName,256);	//get the region name
            CGUID lScriptID ;
            pMsg->GetGUID(lScriptID);
            CGlobalRgnManager::tagRegion* pstRegion = GetGame().GetGlobalRgnManager()->GetTemplateNormalRegion(strRegionName);
            CGUID lRegionID;
            if(pstRegion)
            {
                DWORD dwGSIndex=pstRegion->dwGameServerIndex;
                //����INDEX��GS�Ƿ�����
                if (CMessage::IsGSReady(dwGSIndex))
                {
                    //�ó�����Ч,����REGIONID
                    lRegionID=pstRegion->pRegion->GetExID();					
                }
            }
            CMessage msg(MSG_W2S_GM_ANSWER_FINDREGION);
            msg.Add(PlayerGuid_GM);
            msg.Add(lRegionID);
            msg.Add(lScriptID);
            msg.SendToAllGS();
        }
        break;
    case MSG_S2W_GM_QUERY_PLAYERONLINE:	//GameServer��ѯ�û��Ƿ�����
        {
            char chPlayerName[256];

            pMsg->GetStr(chPlayerName, 256);
            long lScriptID = pMsg->GetLong();
            const CGUID& PlayerGuid=GetGame().GetOnlinePlayerIDByName(chPlayerName);

            long reFlag = (NULL_GUID != PlayerGuid) ? 1 : -1;
            CMessage msg(MSG_W2S_GM_ANSWER_PLAYERONLINE);
            msg.Add(PlayerGuid_GM);
            msg.Add(reFlag);
            msg.Add(lScriptID);
            msg.SendToSocket(pMsg->GetSocketID());
        }
        break;

    case MSG_S2W_GM_RELOAD:		// �ض������ļ�
        {
            //char strName[256];
            //pMsg->GetStr(strName,256);
            //GetGame().ReLoad(strName, true,true);
        }
        break;
    case MSG_S2W_GM_BANPLAYER: //gm BANһ����ң��ҵ�����ҵ�CDKEY�������͸�loginserver
        {
            //get playername
            char strPlayerName[256];
            pMsg->GetStr(strPlayerName,256);
            long lTime=pMsg->GetLong();

            //[CDKEY�޸�]
            //get cdkey
            /*			string strCDKey=GetGame().GetRsPlayer()->GetCDKey(strPlayerName);
            if(!strCDKey.empty())
            {
            CMessage msg(MSG_W2L_GM_BANPLAYER);				
            msg.Add(const_cast<char*>(strCDKey.c_str()));
            msg.Add(lTime);
            msg.SendToServer(NF_LOGIN_SERVER);
            }*/
            GetGame().SendGMBanPlayerToDBSMsg(strPlayerName, lTime);
            Log4c::Trace(ROOT_MODULE,FormatText("WS_GMMSG_10", strPlayerName));
        }
        break;
    case MSG_S2W_GM_SAVEPLAYER:	//	gmҪ�����
        {
        }
        break;
    case MSG_S2W_GM_QUERYGM: //gm�鿴��һgm�Ƿ����ߣ����ｫ��Ϣת��������gameserver
        {
            CMessage msg(MSG_W2S_GM_QUERYGM);
            msg.Add(PlayerGuid_GM);
            msg.Add((long)pMsg->GetGSID());

            msg.SendToAllGS();
        }
        break;
        //gameserver���ض�һ��GM�Ĳ���,worldserver�������Ϣ���������ѯ��gm���ڵ�gameserver
    case MSG_S2W_GM_ANSWERGM:
        {
            pMsg->GetLong();
            DWORD dwGSIndex=pMsg->GetDWord();
            pMsg->SetType(MSG_W2S_GM_ANSWERGM);
            pMsg->SendToGS(dwGSIndex);
        }
        break;

    case MSG_S2W_GM_GETPLAYER_REGION:
        {
            CGUID ScriptID;
            pMsg->GetGUID(ScriptID);
            char pszPlayerName[256]="";
            pMsg->GetStr(pszPlayerName,256);
            CPlayer* player = GetGame().GetMapPlayer(pszPlayerName);

            if(!player)
            {
                CMessage msg(MSG_W2S_GM_ANSWER_GETPLAYER_REGION);
                msg.Add(PlayerGuid_GM);
                msg.Add((long)0);
                msg.Add(ScriptID);
                //��Ҳ�����
                msg.Add(BYTE(2));
                msg.SendToSocket(pMsg->GetSocketID());
            }
            else
            {
                long lGameServerID = GetGame().GetGameServerNumber_ByPlayerID(player->GetExID());
                if(!lGameServerID)
                {
                    CMessage msg(MSG_W2S_GM_ANSWER_GETPLAYER_REGION);
                    msg.Add(PlayerGuid_GM);
                    msg.Add((long)0);
                    msg.Add(ScriptID);
                    //GS������					
                    msg.Add(BYTE(3));
                    msg.SendToSocket(pMsg->GetSocketID());
                }
                else
                {
                    CMessage msg(MSG_W2S_GM_GETPLAYER_REGION);
                    msg.Add(PlayerGuid_GM);
                    msg.Add(ScriptID);
                    msg.Add(player->GetExID());
                    msg.Add(pMsg->GetSocketID());
                    msg.SendToGS(lGameServerID);
                }
            }
        }
        break;
    case MSG_S2W_GM_ANSWERGETPLAYER_REGION:
        {
            long lGMGSID = pMsg->GetLong();
            pMsg->SetType(MSG_W2S_GM_ANSWER_GETPLAYER_REGION);
            pMsg->SendToSocket(lGMGSID);
        }
        break;
    case MSG_S2W_GM_CHANGETPLAYER_REGION:
        {
            CGUID ScriptID;
            pMsg->GetGUID(ScriptID);
            char pszPlayerName[256]="";
            pMsg->GetStr(pszPlayerName,256);
            CGUID RegionID;
            pMsg->GetGUID(RegionID);
            long lX = pMsg->GetLong();
            long lY = pMsg->GetLong();
            CWorldRegion* pWorldRegion = GetGame().GetGlobalRgnManager()->FindRgnByGUID(RegionID);
            if(!pWorldRegion)
            {
                CMessage msg(MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION);
                msg.Add(PlayerGuid_GM);
                msg.Add((long)0);
                msg.Add(ScriptID);
                //����������
                msg.Add(BYTE(4));
                msg.SendToSocket(pMsg->GetSocketID());
                break;
            }

            CPlayer* player = GetGame().GetMapPlayer(pszPlayerName);
            if(!player)
            {
                CMessage msg(MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION);
                msg.Add(PlayerGuid_GM);
                msg.Add((long)0);
                msg.Add(ScriptID);
                //��Ҳ�����
                msg.Add(BYTE(2));
                msg.SendToSocket(pMsg->GetSocketID());
            }
            else
            {
                long lGameServerID = GetGame().GetGameServerNumber_ByPlayerID(player->GetExID());
                if(!lGameServerID)
                {
                    CMessage msg(MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION);
                    msg.Add(PlayerGuid_GM);
                    msg.Add((long)0);
                    msg.Add(ScriptID);
                    //GS������					
                    msg.Add(BYTE(3));
                    msg.SendToSocket(pMsg->GetSocketID());
                }
                else
                {
                    CMessage msg(MSG_W2S_GM_CHANGEPLAYER_REGION);
                    msg.Add(PlayerGuid_GM);
                    msg.Add(ScriptID);
                    msg.Add(player->GetExID());
                    msg.Add(RegionID);
                    msg.Add(lX);
                    msg.Add(lY);
                    msg.Add(pMsg->GetSocketID());
                    msg.SendToGS(lGameServerID);
                }
            }
        }
        break;
    case MSG_S2W_GM_ANSWERCHANGETPLAYER_REGION:
        {
            long lGMGSID = pMsg->GetLong();
            pMsg->SetType(MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION);
            pMsg->SendToSocket(lGMGSID);
        }
        break;
    case MSG_S2W_GM_QUERY_REGIONISEXIT:
        {
            CGUID RegionID;
            pMsg->GetGUID(RegionID);
            CGUID ScriptID;
            pMsg->GetGUID(ScriptID);
            CMessage msg(MSG_W2S_GM_ANSWER_REGIONISEXIT);
            msg.Add(PlayerGuid_GM);
            msg.Add(ScriptID);
            if(GetGame().GetGlobalRgnManager()->FindRgnByGUID(RegionID))
                msg.Add((BYTE)1);
            else
                msg.Add((BYTE)0);
            msg.SendToSocket(pMsg->GetSocketID());
        }
        break;
        //	case MSG_S2W_GM_TEAMCHANGEREGION:
        //		{
        //			CPlayer *pPlayer;
        //			long lGMPlayerID,lPlayerID,lRegionID,lTileX,lTileY;
        //			char szPlayerName[32];
        //
        //			lGMPlayerID = pMsg->GetLong();
        //			pMsg->GetStr(szPlayerName, 32);
        //
        //			lRegionID=pMsg->GetLong();
        //			lTileX=pMsg->GetLong();
        //			lTileY=pMsg->GetLong();
        //
        //			lPlayerID = GetGame().GetOnlinePlayerIDByName(szPlayerName);
        //
        //			// �����ID��Ϊ0
        //			if(lPlayerID)
        //			{
        //				// ��ȡ����ҵĶ����������
        //				CGame::tagTeam *pTeam;
        //
        //				// ����������ڶ�����ÿ����Ա�����͵�GS�л�����
        //				for ( vector<long>::iterator iter = pTeam->vecTeammates.begin(); iter != pTeam->vecTeammates.end(); ++iter)
        //				{
        //					CPlayer * pTeamPlayer = GetGame().GetOnlinePlayerByID(*iter);
        //					if ( pTeamPlayer )
        //					{
        //						CMessage msg(MSG_W2S_GM_CHANGEREGION);
        //						msg.Add( lGMPlayerID );
        //						msg.Add( (char*)pTeamPlayer->GetName() );
        //						msg.Add( lRegionID );
        //						msg.Add( lTileX );
        //						msg.Add( lTileY );
        //						msg.SendToServer(NF_LOGIN_SERVER);
        //					}
        //				}
        //			}
        //		}
        //		break;
    }
}
