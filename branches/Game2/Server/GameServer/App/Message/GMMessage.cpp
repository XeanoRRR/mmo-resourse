#include "StdAfx.h"

#include "../Player.h"
#include "../ServerRegion.h"
//#include "../Script/Script.h"
//#include "../Script/VariableList.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h"

#include "../../../../Public/Setup/GMList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ӦGM��Ϣ
void OnGMMessage(CMessage* pMsg)
{
	CGUID PlayerID_GM;
	pMsg->GetGUID(PlayerID_GM);

	switch(pMsg->GetType())
	{
	/**## WorldServer�򱾷�����Ҫ���߳�ĳλ��� */
	/**## MSG_W2S_GM_KICKPLAYER ��Ϣ��ʽ�� */
	/**## long SenderID, char* strPlayer */
	case MSG_W2S_GM_KICKPLAYER:
		{
			char strPlayer[24];
			ZeroMemory( strPlayer, 24 );
			pMsg -> GetStr( strPlayer,24 );
			CPlayer* pPlayer = GetGame() -> FindPlayer( strPlayer );
			if( pPlayer )
			/**## ���������������� */
			/**## MSG_S2W_GM_ANSWER_KICKPLAYER ��Ϣ��ʽ��*/
			/**## long lGMPlayer, char bSucceed=[1|0], char* pKickedPlayerName */
			{
				GetGame()->KickPlayer(pPlayer -> GetExID());
				CMessage msg( MSG_S2W_GM_ANSWER_KICKPLAYER );
				msg.Add( PlayerID_GM );
				msg.Add( static_cast<char>( 1 ) );
				msg.Add( strPlayer );
				msg.Send();
			}
			else
			/**## ��Ҳ�������������� */
			{
				CMessage msg( MSG_S2W_GM_ANSWER_KICKPLAYER );
				msg.Add( PlayerID_GM );
				msg.Add( static_cast<char>( 0 ) );
				msg.Add( strPlayer );
				msg.Send();
			}
		}
		break;

	/**## WorldServer�򱾷�������������ҵĽ�� */
	case MSG_W2S_GM_ANSWER_KICKPLAYER:
		{
			
			char strText[64];
			char strName[24];
			ZeroMemory( strName, 24 );
			char bSucceed = pMsg -> GetChar();
			pMsg -> GetStr( strName,24 );
			if( bSucceed > 0 )
			/**## MSG_W2S_GM_ANSWER_KICKPLAYER ��ʽ */
			/**## long lGMPlayer, char bSucceed=[1|0], char* pKickedPlayerName */
			{
				_snprintf( strText, 64, "�ɹ������%s���ߡ�", strName );
				/***********************************************************************/
				/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
				/***********************************************************************/
				//GetGame()->SendNotifyMessage(strText,PlayerID_GM,0xffffffff);
				GetGame()->SendNotifyMessage(strText,PlayerID_GM);

			}
			else 
			{
				_snprintf( strText, 64, "�����%s����ʧ�ܣ������ܲ����ߡ�", strName );
				/***********************************************************************/
				/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
				/***********************************************************************/
				//GetGame()->SendNotifyMessage(strText,PlayerID_GM,0xffffffff);
				GetGame()->SendNotifyMessage(strText,PlayerID_GM);
			}
		}
		break;

	/**## WorldServer�򱾷��������������û����� */
	case MSG_W2S_GM_ANSWER_PLAYERCOUNT:
		{
			//wq:2008-7-9,��Ҫ�޸�
			long lPlayers = pMsg -> GetLong();
			CGUID lScriptID;
			pMsg->GetGUID(lScriptID);
			CPlayer* pPlayer = dynamic_cast<CPlayer*>( GetGame()->FindPlayer(PlayerID_GM) );
			if( pPlayer )
			{
				//ContinueRunScript(lScriptID, NULL);
				Script *script = GetInst( ScriptSys ).GetScript( lScriptID );
				if( script != NULL )
				{
					script->PushRetValue( (double) lPlayers );
					GetInst( ScriptSys ).ResumeScript( script, 1 );
				}
			}
			else 
			{
			//	/**## ȡ���ű�������? */
			}
		}
		break;
	/**## WorldServer�򱾷���������������������� */
	/**## MSG_S2W_GM_KICK_ALLPLAYER �� MSG_W2S_GM_KICK_ALLPLAYER ��ʽ */
	/**## long lGMPlayerID */
	case MSG_W2S_GM_KICK_ALLPLAYER:
		{
			GetGame()->KickAllPlayer(PlayerID_GM);
		}
		break;

	/**## WorldServer�򱾷�����������һ�������е�������� */
	/**## MSG_S2W_GM_KICK_REGIONPLAYER �� MSG_W2S_GM_KICK_REGIONPLAYER ��ʽ */
	/**## long lGMPlayerID, char* strRegionName */
	case MSG_W2S_GM_KICK_REGIONPLAYER:
		{
			CGUID lRegionID;
			pMsg -> GetGUID(lRegionID);

			CServerRegion* pRegion = GetGame() -> FindRegion( lRegionID );
			/**## ��֤�����Ƿ��ڱ������� */
			if( pRegion )
			{
				BEGIN_LOOP_REGION_SHAPE_BY_TYPE(pRegion, TYPE_PLAYER)
				{
					if(pShape->GetExID() != PlayerID_GM)
					{
						GetGame()->KickPlayer( pShape->GetExID() );
					}
				}
				END_LOOP_REGION_SHAPE_BY_TYPE
			}
			else 
			{
				/**## �������ڱ���������������ʱ�����κδ��� */
			}
		}
		break;
	case MSG_W2S_GM_QUERY_SILENCE_PLAYERS:
		/**## �����ѯ�����Ե���� */
		/**## MSG_W2S_GM_QUERY_SILENCE_PLAYERS ��ʽ: */
		/**## long lGMId */
		{
			GetGame()->QuerySilencePlayers(PlayerID_GM);
		}
		break;
	case MSG_W2S_GM_ANSWER_QUERY_SILENCE_PLAYERS:
		/**## ���������ر����Ե�����б� */
		/**## MSG_W2S_GM_ANSWER_QUERY_SILENCE_PLAYERS ��ʽ: */
		/**## long lGMId, long lSize, char* pList */
		{
		
			long lSize = pMsg -> GetLong();
			if( lSize != 0 /*&& lSize != ERROR_CODE */ )
			{
				char* pText = (char*)M_ALLOC(sizeof(char)*(lSize + 64));
				if( pText )
				{
					ZeroMemory( pText, lSize + 64 );
					pMsg -> GetStr( pText,lSize+64 );
					strcat( pText, " By Game Server " );
					strcat( pText, GetGame() -> GetNetServer() -> GetPeerIP() );
					/***********************************************************************/
					/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
					/***********************************************************************/
					//GetGame()->SendNotifyMessage(pText,PlayerID_GM,0xffffffff);
					GetGame()->SendNotifyMessage(pText,PlayerID_GM);

					M_FREE( pText, sizeof(char)*(lSize + 64) );
				}
				else
				{
					/***********************************************************************/
					/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
					/***********************************************************************/
					//GetGame()->SendNotifyMessage("��ǰ��Ϸ��������Դ���㣬�޷���ɴ�����" ,PlayerID_GM,0xffffffff);
					GetGame()->SendNotifyMessage("��ǰ��Ϸ��������Դ���㣬�޷���ɴ�����" ,PlayerID_GM);
				}
			}
		}
		break;
	case MSG_W2S_GM_QUERY_PLAYERDATA://worldserverת����gm��ѯһ�����������Ϣ
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			char chPropName[32];			
			DWORD dwGameServerIndex;			
			pMsg->GetStr(chPropName,32);
			dwGameServerIndex=pMsg->GetDWord();
			long lScriptID = pMsg->GetLong();
			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				DWORD dwVal=pPlayer->GetValue(chPropName);
				CMessage msg(MSG_S2W_GM_ANSWER_PLAYERDATA);
				msg.Add(PlayerID_GM);	//playerid
				msg.Add((long)dwVal);	//value
				msg.Add(dwGameServerIndex);	//gameserver index
				msg.Add(lScriptID);
				msg.Send();
			}
		}
		break;
	case MSG_W2S_GM_ANSWER_PLAYERDATA://worldserver���ص�gm��ѯһ�����������Ϣ
		{
			//wq:2008-7-9,��Ҫ�޸�
			long lVal=pMsg->GetLong();
			CGUID lScriptID;
			pMsg->GetGUID(lScriptID);
			//ContinueRunScript(lScriptID, NULL);
			GetInst( ScriptSys ).ResumeScript( lScriptID, 0 );
		}
		break;
	case MSG_W2S_GM_QUERY_PLAYER://worldserver���ص�gm��ѯһ�����������Ϣ(GUID)
	case MSG_W2S_GM_QUERY_PLAYER_P://worldserver���ص�gm��ѯһ�����������Ϣ(GUID)
	case MSG_W2S_GM_QUERY_PLAYER_PH://worldserver���ص�gm��ѯһ�����������Ϣ(GUID)
	case MSG_W2S_GM_QUERY_PLAYER_T://worldserver���ص�gm��ѯһ�����������Ϣ(GUID)
		{
			CGUID lVal;
			pMsg->GetGUID(lVal);
			DWORD dwGameServerIndex=pMsg->GetDWord();
			CGUID lScriptID;
			pMsg->GetGUID(lScriptID);
			char varName[128];
			pMsg->GetStr(varName, 128);// ��������

			//ScriptContinue(lScriptID, GetGame()->FindPlayer(PlayerID_GM), 1.0f);
			CPlayer* pPlayer = GetGame()->FindPlayer(PlayerID_GM);
			if(pPlayer==NULL)
				return;

			//wq:2008-7-9,��Ҫ�޸�
			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddGuid(varName, lVal);
			//}
			//ContinueRunScript(lScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( lScriptID );
			if( script != NULL )
			{
				script->PushRetValue( lVal );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}

		}
		break;
	case MSG_W2S_GM_CHANGEREGION://worldserverת����gm��һ������л���������Ϣ
		{
			char strPlayerName[256];
			pMsg->GetStr(strPlayerName,256);
			CPlayer* pPlayer=GetGame()->FindPlayer(strPlayerName);
			if(pPlayer)
			{
				long lRegionID=pMsg->GetLong();
				long lTileX=pMsg->GetLong();
				long lTileY=pMsg->GetLong();

					// [071127 AHC REGION]
//				pPlayer->ChangeRegion(lRegionID,lTileX,lTileY,pPlayer->GetDir());

			}
		}
		break;
	case MSG_W2S_GM_ANSWER_FINDREGION://worldserver���ص�gm��ѯregionid�Ľ��
		{
			//wq:2008-7-9,��Ҫ�޸�
			CGUID RegionID;
			pMsg->GetGUID(RegionID);
			CGUID lScriptID;
			pMsg->GetGUID(lScriptID);

			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddGuid("@RegionID", RegionID);
			//}
			//ContinueRunScript(lScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( lScriptID );
			if( script != NULL )
			{
				script->PushRetValue( RegionID );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}

		}
		break;
	case MSG_W2S_GM_KICKPLAYEREX://worldserverת����gm��������
		{
			char strName[256];
			pMsg->GetStr(strName,256);
			DWORD dwGSIndex=pMsg->GetDWord();
			CPlayer* pPlayer=GetGame()->FindPlayer(strName);
			if( pPlayer )				
			{
				/*
				*��7��7���ӵ����
				*/
				int nStartX=0,nStartY=0;
				CServerRegion* pRegion=dynamic_cast<CServerRegion*>(pPlayer->GetFather());
				nStartX=pPlayer->GetTileX() - 3;
				nStartY=pPlayer->GetTileY() - 3;
				if(pPlayer->GetTileX()+3 >= pRegion->GetWidth())
				{
					nStartX=pRegion->GetWidth() - 7;
				}
				else if(pPlayer->GetTileX()-3 <= 0)
				{
					nStartX=0;
				}
				if(pPlayer->GetTileY()+3 >= pRegion->GetHeight())
				{
					nStartY=pRegion->GetHeight() - 7;
				}
				else if(pPlayer->GetTileY()-3 <= 0)
				{
					nStartY=0;
				}
				list<CPlayer*> playersList;
				CShape* pShape=NULL;
				for(int i=nStartX;i<nStartX+7;i++)
				{
					for(int j=nStartY;j<nStartY+7;j++)
					{
						pShape=pRegion->GetShape(i,j);
						if(pShape && pShape->GetType()==TYPE_PLAYER)
						{
							playersList.push_back(dynamic_cast<CPlayer*>(pShape));
						}
					}
				}
				playersList.unique();
				list<CPlayer*>::iterator PlayerIter=playersList.begin();
				for(;PlayerIter!=playersList.end();PlayerIter++)
				{
					GetGame()->KickPlayer((*PlayerIter)->GetExID());
				}
				char szText[256];				
				_snprintf(szText, 256, "%d������Ѿ����߳���Ϸ",playersList.size());
				CMessage msg( MSG_S2W_OTHER_ADDINFO );
				msg.Add(dwGSIndex);
				msg.Add(PlayerID_GM);
				msg.Add( static_cast<DWORD>(0xffffffff) );
				msg.Add( DWORD(0) );	// ����ɫ
				msg.Add( szText );
				msg.Send();					
									
			}
		}
		break;
	case MSG_W2S_GM_ANSWER_PLAYERONLINE://WorldServer�򱾷����������û�������Ϣ
		{
			//wq:2008-7-9,��Ҫ�޸�
			long reVal=pMsg->GetLong();
			CGUID lScriptID;
			pMsg->GetGUID(lScriptID);
			//ContinueRunScript(lScriptID, NULL);
			Script *script = GetInst( ScriptSys ).GetScript( lScriptID );
			if( script != NULL )
			{
				script->PushRetValue( reVal );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}
		}
		break;
	case MSG_W2S_GM_SILENCE_PLAYER:
		/**## MSG_S2W_GM_SILENCE_PLAYER, MSG_W2S_GM_SILENCE_PLAYER ��Ϣ��ʽ */
		/**## long lGMId, char* pSilencePlayerName, long lTime */		
		{
			CGUID ScriptID;
			pMsg->GetGUID(ScriptID);
			char strPlayerName[256];
			ZeroMemory( strPlayerName, 256 );
			pMsg -> GetStr( strPlayerName,256 );
			long lTime = pMsg -> GetLong();
			if( lTime < 1 ) lTime = 1;

			/*CMessage msg( MSG_S2W_GM_ANSWER_SILENCE_PLAYER );
			msg.Add( PlayerID_GM );
			msg.Add( strPlayerName );
			msg.Add( lTime );*/

			CPlayer* pPlayer = GetGame() -> FindPlayer( strPlayerName );
			if( pPlayer )
			{
				pPlayer -> SetSilence( lTime );
				//msg.Add( static_cast<char>(1) );
				//msg.Add("�����ɹ���");
				//msg.Send();
			}
			/*else 
			{
				msg.Add( static_cast<char>(0) );
				msg.Add("��ҿ��ܲ����ڣ����߲����ߡ�");
				msg.Send();
			}*/
		}
		break;

	case MSG_W2S_GM_ANSWER_SILENCE_PLAYER:
		/**## �������������Ϸ������������ҽ��Ե���� */
		/**## MSG_W2S_GM_ANSWER_SILENCE_PLAYER �� MSG_S2W_GM_ANSWER_SILENCE_PLAYER ��ʽ */
		/**## long lGMId, char* pPlayerName, long lTime, byte bSucceed, char* pMsg */
		{
			CGUID ScriptID;
			pMsg->GetGUID(ScriptID);
			long lFlag = pMsg->GetByte();

			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddVar("$m_TalkRet", lFlag);
			//}
			//ContinueRunScript(ScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( ScriptID );
			if( script != NULL )
			{
				script->PushRetValue( lFlag );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}

		}
		break;
	case MSG_W2S_GM_WORLD_MESSAGE:
		/**## �������������Ϸ����������ȫ�ֹ㲥 */
		/**## MSG_S2W_GM_WORLD_MESSAGE �� MSG_W2S_GM_WORLD_MESSAGE ��ʽ */
		/**## long lGMId, char* pMessage, DWORD dwColor */
		{
			char strMessage[256];
			ZeroMemory( strMessage, 256 );
			pMsg -> GetStr( strMessage,256 );
			//DWORD dwColor = pMsg -> GetDWord();
			//DWORD dwBkColor = pMsg->GetDWord();

			
				CMessage msg( MSG_S2C_OTHER_ADDINFO );
				//msg.Add( dwColor );
				//msg.Add( dwBkColor );	// ����ɫ
				msg.Add((BYTE)eNOTIFYPOS_LEFT_BOTTOM);
				msg.Add( strMessage );
				msg.SendAll();
		}
		break;

	case MSG_W2S_GM_COUNTRY_MESSAGE:
		{
			// ֻ���͸��Լ����������
			char strMessage[256];
			ZeroMemory( strMessage, 256 );
			pMsg -> GetStr( strMessage,256 );
			long cid = pMsg->GetLong();
			//DWORD dwColor = pMsg -> GetDWord();
			//DWORD dwBkColor = pMsg->GetDWord();

			CMessage msg( MSG_S2C_OTHER_ADDINFO );
			//msg.Add( dwColor );
			//msg.Add( dwBkColor );	// ����ɫ
			msg.Add((BYTE)eNOTIFYPOS_CENTER);
			msg.Add( strMessage );
			GetGame()->SendMsgToSameCountryPlayer(&msg,(BYTE)cid);

		}
		break;

	case MSG_W2S_GM_QUERYGM:
		{
			pMsg->SetType(MSG_S2W_GM_ANSWERGM);
			list<string> GMNameList;
			CPlayer* pPlayer;
			tagGMInfo*	pGM;			
			char buf[64];
			for (map<string, tagGMInfo>::iterator it=GMList::GetGMInfoMap()->begin(); it!=GMList::GetGMInfoMap()->end(); it++)
			{
				pGM = &it->second;				
				pPlayer= GetGame()->FindPlayer(pGM->strName.c_str());	//�õ�����ָ��
				if( pPlayer )
				{
					switch(pGM->lLevel)
					{
						case GM_ADMIN:
							_snprintf(buf, 64, "%s (ϵͳ����Ա)",pGM->strName.c_str());
							break;
						case GM_ARCH:
							_snprintf(buf, 64, "%s (��Ϸ����Ա)",pGM->strName.c_str());
							break;
						case GM_WIZARD:
							_snprintf(buf, 64, "%s (��ʦ)",pGM->strName.c_str());
							break;
					}

					GMNameList.push_back(buf);
					
				}
			}
			pMsg->Add(long(GMNameList.size()));
			list<string>::iterator NameIter=GMNameList.begin();
			for(;NameIter!=GMNameList.end();NameIter++)
			{
				pMsg->Add(const_cast<char*>(NameIter->c_str()));
			}
			pMsg->Send();
		}
		break;
	case MSG_S2W_GM_ANSWERGM:
		{
			CGUID PlayerID;
			pMsg->GetGUID(PlayerID);
			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);
			if(pPlayer)
			{
				pMsg->GetDWord();
				long lSize=pMsg->GetLong();
				char buf[256];
				for(long i=0;i<lSize;i++)
				{
					pMsg->GetStr(buf,256);
					pPlayer->SendNotifyMessage(buf);
				}
			}
		}
		break;
	case MSG_W2S_GM_GETPLAYER_REGION:
		{
			CGUID ScriptID;
			CGUID PlayerID;
			pMsg->GetGUID(ScriptID);
			pMsg->GetGUID(PlayerID);
			long lGMGSID = pMsg->GetLong();
			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);

			CMessage msg(MSG_S2W_GM_ANSWERGETPLAYER_REGION);
			msg.Add(PlayerID_GM);
			msg.Add(lGMGSID);
			msg.Add(ScriptID);
			if(!pPlayer)
			{
				//������
				msg.Add(BYTE(2));
			}
			else
			{
				msg.Add(BYTE(1));
				msg.Add(pPlayer->GetRegionGUID());
				msg.Add(pPlayer->GetTileX());
				msg.Add(pPlayer->GetTileY());
			}
			msg.Send();
		}
		break;
	case MSG_W2S_GM_ANSWER_GETPLAYER_REGION:
		{

			long lGSID = pMsg->GetLong();
			CGUID ScriptID;
			pMsg->GetGUID(ScriptID);
			long lFlag = pMsg->GetByte();
			CGUID RegionGUID;
			long lX = 0;
			long lY = 0;
			if(1==lFlag)
			{				
				pMsg->GetGUID(RegionGUID);
				lX = pMsg->GetLong();
				lY = pMsg->GetLong();
			}

			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddVar("$m_TalkRet", lFlag);
			//	stackVarList->AddGuid("@PlayerRegionID",RegionGUID);
			//	stackVarList->AddVar("$PlayerRegionX", lX);
			//	stackVarList->AddVar("$PlayerRegionY", lY);
			//}
			//ContinueRunScript(ScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( ScriptID );
			if( script != NULL )
			{
				script->PushRetValue( lFlag );
				script->PushRetValue( RegionGUID );
				script->PushRetValue( lX );
				script->PushRetValue( lY );
				GetInst( ScriptSys ).ResumeScript( script, 4 );
			}

		}
		break;
	case MSG_W2S_GM_CHANGEPLAYER_REGION:
		{
			CGUID ScriptID;
			CGUID PlayerID;
			CGUID RegionID;
			pMsg->GetGUID(ScriptID);
			pMsg->GetGUID(PlayerID);
			pMsg->GetGUID(RegionID);
			long lX = pMsg->GetLong();
			long lY = pMsg->GetLong();
			long lGMGSID = pMsg->GetLong();

			CPlayer* pPlayer=GetGame()->FindPlayer(PlayerID);

			CMessage msg(MSG_S2W_GM_ANSWERCHANGETPLAYER_REGION);
			msg.Add(PlayerID_GM);
			msg.Add(lGMGSID);
			msg.Add(ScriptID);
			if(!pPlayer)
			{
				//������
				msg.Add(BYTE(2));
			}
			else
			{
				msg.Add(BYTE(1));
				pPlayer->ChangeRegion(RGN_ALL,RegionID,lX,lY,0);
			}
			msg.Send();
		}
		break;
	case MSG_W2S_GM_ANSWER_CHANGEPLAYER_REGION:
		{
			long lGSID = pMsg->GetLong();
			CGUID ScriptID;
			pMsg->GetGUID(ScriptID);
			long lFlag = pMsg->GetByte();
			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddVar("$m_TalkRet", lFlag);
			//}
			//ContinueRunScript(ScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( ScriptID );
			if( script != NULL )
			{
				script->PushRetValue( lFlag );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}

		}
		break;
	case MSG_W2S_GM_ANSWER_REGIONISEXIT:
		{
			CGUID ScriptID;
			pMsg->GetGUID(ScriptID);
			long lFlag = pMsg->GetByte();
			//CVariableList* stackVarList = CScript::GetStackVarListInst();
			//if(stackVarList)
			//{
			//	stackVarList->Release();
			//	stackVarList->AddVar("$m_TalkRet", lFlag);
			//}
			//ContinueRunScript(ScriptID, stackVarList);
			Script *script = GetInst( ScriptSys ).GetScript( ScriptID );
			if( script != NULL )
			{
				script->PushRetValue( lFlag );
				GetInst( ScriptSys ).ResumeScript( script, 1 );
			}

		}
		break;
	}
}

// �����Ƿ�ɹ�ִ��
bool ParseGMCommand(CMessage* pMsg, char *CmdLine)
{
	tagGMInfo *pCurrentGM;

	pCurrentGM=GMList::GetInfoByName(pMsg->GetPlayer()->GetName());

	if (!pCurrentGM)
		return false;

	char Cmd[40];
	char Param[256];
	char Param1[128];
	char Param2[128];
	char Param3[128];
	char Param4[128];
	char Param5[128];

	int nParam1=0;
	int nParam2=0;
	int nParam3=0;
	int nParam4=0;
	int nParam5=0;

	char* pParam;
	int nSpaceCount=0;
	int nCharCount=0;
	int nCharCount2=0;

	memset(Cmd,0,sizeof(Cmd));
	memset(Param,0,sizeof(Param));
	memset(Param1,0,sizeof(Param1));
	memset(Param2,0,sizeof(Param2));
	memset(Param3,0,sizeof(Param3));
	memset(Param4,0,sizeof(Param4));
	memset(Param5,0,sizeof(Param5));

	pParam=CmdLine;

	while(*pParam!=0)
	{
		if (*pParam==' ')
		{
			nSpaceCount++;
			nCharCount=0;
			if (nSpaceCount>0)
			{
				Param[nCharCount2]=' ';
				nCharCount2++;
			}
			pParam++;
			continue;
		}

		switch(nSpaceCount)
		{
			case 0:
				if (nCharCount<sizeof(Cmd))//��ֹ���������
					Cmd[nCharCount]=*pParam;
				nCharCount++;
				break;
			case 1:
				if (nCharCount<sizeof(Param1))
					Param1[nCharCount]=*pParam;
				if (nCharCount2<sizeof(Param))
					Param[nCharCount2]=*pParam;
				nCharCount++;
				nCharCount2++;
				break;
			case 2:
				if (nCharCount<sizeof(Param2))
					Param2[nCharCount]=*pParam;
				if (nCharCount2<sizeof(Param))
					Param[nCharCount2]=*pParam;
				nCharCount++;
				nCharCount2++;
				break;
			case 3:
				if (nCharCount<sizeof(Param3))
					Param3[nCharCount]=*pParam;
				if (nCharCount<sizeof(Param))
					Param[nCharCount2]=*pParam;
				nCharCount++;
				nCharCount2++;
				break;
			case 4:
				if (nCharCount<sizeof(Param4))
					Param4[nCharCount]=*pParam;
				if (nCharCount<sizeof(Param))
					Param[nCharCount2]=*pParam;
				nCharCount++;
				nCharCount2++;
				break;
			case 5:
				if (nCharCount<sizeof(Param5))
					Param5[nCharCount]=*pParam;
				if (nCharCount<sizeof(Param))
					Param[nCharCount2]=*pParam;
				nCharCount++;
				nCharCount2++;
				break;
		}
		pParam++;
	}

	nParam1=atoi(Param1);
	nParam2=atoi(Param2);
	nParam3=atoi(Param3);
	nParam4=atoi(Param4);
	nParam5=atoi(Param5);


	//sscanf(CmdLine, "%s %s", Cmd,Param);
	//sscanf(CmdLine, "%s %s", Cmd,Param1);
	//sscanf(CmdLine, "%*s %d",&nParam1);
	//sscanf(CmdLine, "%*s %*s %d",&nParam2);

	assert( "Not implemented" && 0 );
	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMLevel",pCurrentGM->lLevel);

	//pMsg->GetPlayer()->GetVariableList()->AddVar("#GMParam1",Param1);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("#GMParam2",Param2);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("#GMParam3",Param3);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("#GMParam4",Param4);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("#GMParam5",Param5);

	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMParam1",nParam1);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMParam2",nParam2);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMParam3",nParam3);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMParam4",nParam4);
	//pMsg->GetPlayer()->GetVariableList()->AddVar("$GMParam5",nParam5);


	////Ϊ�˱�֤��ȫ��,����������ϲ�·��,����".."��ֱ���˳�
	//if (NULL!=strstr(Cmd,".."))
	//	return false;

	//char filename[MAX_PATH];
	//_snprintf(filename, MAX_PATH, "scripts/gm/%s.script", Cmd);

	//stRunScript st;
	//st.pRegion = (CRegion*)pMsg->GetRegion();
	//st.desShape = (CPlayer*)pMsg->GetPlayer();
	//st.srcShape = (CPlayer*)pMsg->GetPlayer();
	//st.pszFileName = filename;

	//RunScript(&st, (char*)GetGame()->GetScriptFileData(filename));

	return true;
}
