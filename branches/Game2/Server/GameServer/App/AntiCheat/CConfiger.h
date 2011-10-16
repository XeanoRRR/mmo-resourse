#pragma once

namespace AntiCheat
{
	typedef std::pair<int, int> Range;
	class CPlayerProperty;

	class CConfiger
	{
	public:
		enum TalkCheck
		{
			eTC_NONE = 0x0000,
			eTC_NORMAL = 0x0001,
			eTC_REGION = 0x0002,
			eTC_PRIVATE = 0x0004,
			eTC_TEAM = 0x0008,
			eTC_FACTION = 0x0010,
		};
		struct ScriptCfg
		{
			std::string file;
			int limit;
		};
	public:
		struct System
		{
			System()
			{
				memset(this,0,sizeof(System));
			}
			Range CheckIntervalRange;
			Range QuitActivityRange;
			int RefuseMax;
			int BonusMax;
			int AnswerInterval;
			int PkTimeInterval;
			int ActivityMax;
			int PkAddValue;
			int PkDropValue;
			int PkDropInterval;
			int AtkActivityAddValue;
			int AtkActivityDropValue;
			int AtkActivityDropInterval;
			Range FightActivityRange;
			Range TalkActivityRange;
			Range TalkSensActivityRange;
			Range RushHourRange;
			int AnswerTimeOutInterval;
			int DropCurWrongValue;
			int CheckSensWords;
			int TalkCheckFlag;

			int UpdateQuestionInterval;
			int GenQuestionCount;
			Range AnswerAlpha;
			int BanHours;
			int LevelFilter;
			int Enable;
		}; 

		typedef std::map<int, ScriptCfg> ScriptsTable;
		typedef ScriptsTable::iterator     SCon_LIST;

	public:

		/// ����Ϣ��������  
		void DecodeFromDataBlock( DBReadSet &db );

		/// �ش���ȷ�ű�����  
		void OnAnswerRight( const CPlayer *player, int rightTimes, bool bLimit );

		/// �ش����ű�����  
		void OnAnswerWrong( const CPlayer *player, int wrongTimes );

		/// ��ȡȫ������(System��)  
		const System &GetSystem() const   {    return m_sys;   }

		/// �Ƿ�������ҹ���
		bool IsEnable() const { return m_sys.Enable != 0; }

		/// ���ָ���ַ����Ƿ�������д�
		bool HasSensWord( const std::string &content );

		/// �Ƿ���ָ��Ƶ������ 
		bool IsCheckTalk( TalkCheck flag )
		{
			return ( m_sys.TalkCheckFlag & flag ) != 0;
		}

	private:
		/// �������д��б�
		void DecodeSensWords( DBReadSet &db );
	private:
		System m_sys;
		ScriptsTable m_RightScripts;
		ScriptsTable m_WrongScripts;
	private:
		typedef std::list<std::string> SensWordsList;
		SensWordsList m_SensWords;
	};

}