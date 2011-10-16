#pragma once
 
class CPlayer;

namespace AntiCheat
{
	class CPlayerProperty;
	class CPlayerManager
	{
	public:
		typedef std::map<CGUID, CPlayerProperty*> PlayersTable;

	public:

		void Release();

		///  ��ҽ�����Ϸ��������ҵĶ�ʱ��
		bool PlayerEnter( CPlayer *player );

		///  ����˳���ֻ��Ҫ�������б�������ݼ��ɣ����ݴ浵��Ҫ
		///  ʹ����һ���ӿڡ�
		void PlayerExit( const CGUID &playerID );

		/// ����ĳ����ҵ����ݣ��ú������б��в��ҵ�PlayerProperty����
		/// ������EncodeToDataBlock�������ݡ����øú�����ʾ����ҿ����з���
		/// �����˳���Ϸ׼�����档(CPlayer::CodeToDataBlock)
		void PlayerEncode( CPlayer *player , DBWriteSet &db );

		///  ��ҽ����µ�GS����һ�ν������л�GS��ʱ���ô˺���
		///  �ú������������ҵĴ浵����(CPlayerProperty::DecodeFromDataBlock)
		void PlayerDecode( CPlayer *player, DBReadSet &db );

		///  �ͻ��˻�Ӧĳ�����⣬�����б�������PlayerProperty::OnAntiCheatAnswer
		void PlayerAnswer( CPlayer *player, int sel );

		///  ��ҽ���Pk
		void PlayerFight(CPlayer *player);

		/// �����������
		void PlayerAttack( CPlayer *player );

		/// ��ҷ��ԣ����ܻ�������Ծ��
		void PlayerTalk( CPlayer *talker, const std::string &content, 
			const std::string &listener, long talk_type );

		///  �ı���һ�Ծ��
		int  PlayerChangeActivity( const CGUID &playerID, int changeValue, bool bSend = true );

		///  �������������߽ű�
		void KickOnlinePlayer( const CGUID &PlayerId );

		///  ֹͣ��ҷ����ģ��
		void StopCheck(const CGUID & PlayerId);

		/// �ṩ���ű���ѯ���Ե�����
		long ScriptQueryProperty( const CGUID &playerID, const std::string &var_name );

		/// �������Ƿ��оܾ��ش���Ŀ����Ϊ���з��ܾ���
		void CheckRefuseAnswer( const CGUID &playerID, bool bChangeServer );

		/// ������ҷ���Ҷ�ʱ��
		void StartCheck(const CGUID & PlayerId);
	private:
		///  ���ز��ҽ��
		inline CPlayerProperty *FindPlayerProPerty(const CGUID & guid );

	private:
		PlayersTable m_playerTable;
	};
}

