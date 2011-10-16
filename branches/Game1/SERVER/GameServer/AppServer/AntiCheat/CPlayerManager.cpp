#include "StdAfx.h"
#include "CPlayerManager.h"
#include "PlayerProperty.h"
#include "CConfiger.h"
#include "CGUID.h"
#include "../Player.h"

namespace AntiCheat
{
	
#define CHECK_ENABLE if( !GetInst( CConfiger ).IsEnable() ) return

	void CPlayerManager::Release()
	{
		for( PlayersTable::iterator it = m_playerTable.begin();
			it != m_playerTable.end(); ++ it )
		{
			it->second->CancelTimers();
			MP_DELETE( it->second );
		}
		m_playerTable.clear();
	}

	/// ��ҽ�����Ϸ
	bool CPlayerManager::PlayerEnter(CPlayer *player )
	{		
		if( !GetInst( CConfiger ).IsEnable() )
		{
			return true;
		}

		const CGUID &playerid = player->GetExID();
		CPlayerProperty * playerpro = FindPlayerProPerty(playerid);
		
		if (playerpro !=NULL)
		{
			/// ��ҽ�����Ϸ��������ʱ��
			return playerpro->OnEnter();
		}
		return false;
	}

	/// ������ҷ���Ҷ�ʱ��
	void CPlayerManager::StartCheck( const CGUID& PlayerId)
	{
		CHECK_ENABLE;
		CPlayerProperty * playerpro = FindPlayerProPerty(PlayerId);
	
		if ( playerpro != NULL)
		{
			 playerpro->StartCheck();
		}
		return ;
	}

	/// ����˳�,����б�
	void CPlayerManager::PlayerExit( const CGUID &playerID )
	{	
		CHECK_ENABLE;

		CPlayerProperty * playerpro = FindPlayerProPerty(playerID);
		if (playerpro != NULL)
		{
			//playerpro->OnExit( bChangeServer );
			/// ע��������еĶ�ʱ��
			playerpro->CancelTimers();
			/// �ͷŶ���ռ�
			MP_DELETE(playerpro);
			/// ɾ��map��Դ
			m_playerTable.erase(playerID);
		}
		
	}

	/// ������EncodeToDataBlock�������ݡ�
	void CPlayerManager::PlayerEncode( CPlayer *player , DBWriteSet &db )
	{
		CHECK_ENABLE;

		CPlayerProperty * playerpro = FindPlayerProPerty(player->GetExID());
		if (playerpro != NULL)
		{
			db.AddToByteArray((BYTE)1);
			playerpro->EncodeToDataBlock(db);
		}
		else
		{
			db.AddToByteArray((BYTE)0);
		}
	}

	void CPlayerManager::PlayerDecode( CPlayer *player, DBReadSet &db )
	{
		CHECK_ENABLE;
		
		const CGUID &playerid = player->GetExID();
		CPlayerProperty *playerpro = MP_NEW CPlayerProperty(player->GetExID());
		playerpro->DecodeFromDataBlock(db);
	
		//! ԭ�����ݿ��Ը�����--------------
		PlayersTable::iterator itePlayer = m_playerTable.find(playerid);
		if (m_playerTable.end() != itePlayer)
		{
			MP_DELETE(itePlayer->second);
			m_playerTable.erase(itePlayer);
		}
		//! --------------
		m_playerTable[playerid] = playerpro;

	}
	/// ��Ӧ�ͻ��˵Ĵ�����Ϣ
	void CPlayerManager::PlayerAnswer(CPlayer *player, int sel )
	{
		const CGUID &playerid = player->GetExID();
		CPlayerProperty * playerpro = FindPlayerProPerty(playerid);
		if (playerpro != NULL)
		{
			/// �û�ѡ��Ĵ�
			playerpro->OnAntiCheatAnswer(sel);
		}
	}

	/// Pk�����¼�
	void CPlayerManager::PlayerFight(CPlayer *player)
	{
		const CGUID &playerid = player->GetExID();
		CPlayerProperty * playerpro = FindPlayerProPerty(playerid);
		if (playerpro != NULL)
		{
			/// �ı���ҵ�Pkֵ
			playerpro->OnFight();
		}
	}

	void CPlayerManager::PlayerAttack( CPlayer *player )
	{
		const CGUID &playerid = player->GetExID();
		CPlayerProperty *playerpro = FindPlayerProPerty( playerid );
		if( playerpro != NULL )
		{
			playerpro->OnAttack();
		}
	}

	void CPlayerManager::PlayerTalk( CPlayer *talker, const std::string &content, 
			const std::string &listener, long talk_type )
	{
		CPlayerProperty *playerpro = FindPlayerProPerty( talker->GetExID() );
		if( playerpro != NULL )
		{
			playerpro->OnTalk( content, listener, talk_type );
		}
	}

	/// ��Ծ�Ƚű��ӿں���
	int CPlayerManager::PlayerChangeActivity(const CGUID &playerID, int changeValue, bool bSend )
	{
		CPlayerProperty * playerpro = FindPlayerProPerty(playerID);
		if (playerpro != NULL)
		{
			return playerpro->ChangeActivity(changeValue, bSend);
		}
		return 0;
	}

	/// ����Ԫ��
	CPlayerProperty *CPlayerManager::FindPlayerProPerty(const CGUID & guid )
	{
		map<CGUID, CPlayerProperty*>::iterator it = m_playerTable.find(guid);
		if (it != m_playerTable.end())
		{
			return it->second;
		}
		return NULL;
	}

	/// ���������
	void CPlayerManager::KickOnlinePlayer(const CGUID &PlayerId  )
	{
		CPlayerProperty * playerpro = FindPlayerProPerty(PlayerId);
		
		if (playerpro != NULL)
		{
			playerpro->KickPlayer();
		}
	}

	void CPlayerManager::StopCheck(const CGUID & PlayerId)
	{
		CPlayerProperty * playerpro = FindPlayerProPerty(PlayerId);
		if (playerpro != NULL)
		{
			playerpro->StopCheck();
		}
	}

	void CPlayerManager::CheckRefuseAnswer( const CGUID &playerID, bool bChangeServer )
	{
		CPlayerProperty * playerpro = FindPlayerProPerty(playerID);
		if (playerpro != NULL)
		{
			playerpro->CheckRefuseAnswer( bChangeServer );
		}
	}

	long CPlayerManager::ScriptQueryProperty( const CGUID &playerID, const std::string &var_name )
	{
#ifdef SCRIPT_QUERY
		CPlayerProperty * playerpro = FindPlayerProPerty(playerID);
		if (playerpro != NULL)
		{
			return playerpro->GetAttr( var_name );
		}
#endif
		return 0;
	}
}