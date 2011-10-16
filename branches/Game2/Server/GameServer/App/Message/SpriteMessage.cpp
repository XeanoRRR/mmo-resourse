#include "StdAfx.h"

#include "../Player.h"
#include "../Region.h"
#include "../SpriteSystem.h"
#include "../LuaScript/Script.h"
#include "../LuaScript/ScriptSys.h"
#include "../../../../Public/Setup/GlobalSetup.h"


//! ��Ӧ�ͻ��˵�С������Ϣ
void OnSpriteMessage_ForClient(CMessage *pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_C2S_SPRITE_AutoSupply_OnOff_Request://! ���ݲ�ʹ�ã��Զ�����ϵͳ�����ܿ�������
		{

		}
		break;
	case MSG_C2S_SPRITE_AutoSupply_ChangeSetup_Request://! �Զ�����ϵͳ���޸�һ��������������
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				pPlayer->m_SpriteSystem.OnChangeAutoSupply(pMsg);
				
			}	
		}
		break;

	//!-------------------------------------------------------------------------------------------
	//! ��С����ϵͳ��Ϣ
	case MSG_C2S_SaveNewSetupData_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				tagSetupOnServer SetupOnServer;
				pMsg->GetEx(&SetupOnServer, sizeof(tagSetupOnServer));
				BOOL bRevert = pPlayer->m_SetupOnServer.SetSetup(SetupOnServer);
				CMessage msg(MSG_S2C_SaveNewSetupData_Re);
				msg.Add((LONG)bRevert);
				msg.SendToPlayer(pPlayer->GetExID());
			}
		}
		break;
		  //! ������ճ̱�
	case	MSG_C2S_Calendar_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = GlobalSetup::GetSetup()->pszCalendar;
				if(strScript)
				{
					//stRunScript sScript;
					//sScript.pszFileName = (char*)strScript;
					//sScript.desShape = pPlayer;
					//sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					//sScript.srcShape	= pPlayer;
					//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
					Script::RunEnv env( pPlayer, pPlayer, static_cast<CRegion*>( pPlayer->GetFather() ) );
					GetInst( ScriptSys ).RunScript( env, strScript );
				}
			}
		}
		break;
			//! ���������άħ��
	case	MSG_C2S_JanaweBox_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = GlobalSetup::GetSetup()->pszMagicBox;
				if(strScript)
				{
					//stRunScript sScript;
					//sScript.pszFileName = (char*)strScript;
					//sScript.desShape = pPlayer;
					//sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					//sScript.srcShape	= pPlayer;
					//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
					Script::RunEnv env( pPlayer, pPlayer, static_cast<CRegion*>( pPlayer->GetFather() ) );
					GetInst( ScriptSys ).RunScript( env, strScript );
				}
			}
		}
		break;
			//! �ʴ��뽱��
	case	MSG_C2S_Answer_Request:
		{
			CPlayer *pPlayer = pMsg->GetPlayer();
			if(NULL != pPlayer)
			{
				const char* strScript = GlobalSetup::GetSetup()->pszAnswerRequest;
				if(strScript)
				{
					//stRunScript sScript;
					//sScript.pszFileName = (char*)strScript;
					//sScript.desShape = pPlayer;
					//sScript.pRegion =  dynamic_cast<CRegion*>(pPlayer->GetFather());
					//sScript.srcShape	= pPlayer;
					//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
					Script::RunEnv env( pPlayer, pPlayer, static_cast<CRegion*>( pPlayer->GetFather() ) );
					GetInst( ScriptSys ).RunScript( env, strScript );

				}
			}
		}
	break;

	}
}