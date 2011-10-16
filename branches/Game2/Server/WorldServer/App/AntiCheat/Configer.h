#pragma once

class TiXmlElement ;

namespace AntiCheat
{
	typedef std::pair<int, int> Range;
	class PlayerProperty;

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
			Range AnswerAlphaRange; /* [0-255] */
			int BanHours;
			int LevelFilter;
			int Enable;
		}; 

		typedef std::map<int, ScriptCfg> ScriptsTable;
		typedef ScriptsTable::iterator     SCon_LIST;

	public:
		/// װ�������ļ� 
		bool Load();

		/// ����������Ϣ����Ϣ  
		void EncodeToDataBlock( DBWriteSet &db );

		/// ��ȡȫ������(System��)  
		const System &GetSystem() const   {    return m_sys;   }

		/// �Ƿ�������ҹ���
		bool IsEnable() const { return m_sys.Enable != 0; }

	private:
		/// �������д��б�
		void EncodeSensWords( DBWriteSet &db );

		/// װ�����д��б�
		bool LoadSensWords( const char *file );

		/// װ�ؽű����ÿ�
		bool LoadScriptTable( TiXmlElement *pElem, ScriptsTable &st );
	private:
		System m_sys;
		ScriptsTable m_RightScripts;
		ScriptsTable m_WrongScripts;

	private:
		typedef std::list<std::string> SensWordsList;
		SensWordsList m_SensWords;
	};

}