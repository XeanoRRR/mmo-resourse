#pragma once 

#include "..\..\..\..\public\GameEvent.h"
#include "..\..\..\..\public\MsgType.h"
#include "..\..\GameServer\GameManager.h"
#include "..\public\Singleton.h"

class CPlayer;
class CRegion;

namespace AntiCheat
{

	// ����ű���ѯ�������
#define SCRIPT_QUERY
#ifdef SCRIPT_QUERY
	class CScriptQuery
	{
	private:
		typedef void* VarPtr;
		typedef std::map<std::string, VarPtr> VarTable;
	public:
		template <typename _Tp>
		void AddVar( const std::string &name, _Tp &var )
		{
			m_Vars[name] = &var;
		}

		VarPtr GetVar( const std::string &name ) const
		{
			VarTable::const_iterator it = m_Vars.find( name );
			if( it == m_Vars.end() )
			{
				return VarPtr();
			}
			return it->second;
		}

		template <typename _Tp>
		_Tp GetVarValue( const std::string &name ) const
		{
			VarPtr v = GetVar( name );
			return v == VarPtr() ? _Tp() : *( (_Tp*)v );
		}

	private:
		VarTable m_Vars;
	};
#endif

	class CQusetionManager;
	class CPlayerProperty : public BaseMemObj, public CGameEvent
	{
	private:
		/// ��ҵĴ浵����
		struct SavedProperty 
		{
			SavedProperty() 
			{
				memset(this,0,sizeof(SavedProperty));
			};
			int nActFactor;							/// ��Ծ��
			int nAccWrongTimes;						/// �ۼƴ�����
			int nCurWrongTimes;						/// ��ǰ������
			int nCurRightTimes;						/// ������Դ���
			int nLastGameTime;						/// �ϴ���Ϸʱ��
			unsigned char bBanFlag;					/// ��ű�־��Ϊ1ʱ��ʾ�����
			unsigned char m_bScriptExecuted;		/// Ϊ1ʱ��ʾ֮ǰ�Ĵ���ű�δ����
			int nBonusTimes;						/// ��ǰ�����������ڶ�������
			unsigned char m_bRHAnswered;			/// �Ƿ��ڻƽ�ʱ�������ȷ��
		};

		/// ��Ҫ���뵽WS������
		struct CachedProperty
		{
			SavedProperty saved;
			long nAntiCheatPK;
			unsigned long LastAnswerTime;	
			long nRefuseTimes;
			long nAtkAct;
			CachedProperty()
			{
				memset( this, 0, sizeof( *this ) );
			}
		};

		/// ��ʱ������
		enum Anti_Type
		{
			ANTICHEAT=1,							/// ���ⶨʱ��
			PKTIMER,								/// PK��ʱ��
			ANSTIMER,								/// �ش����ⳬʱ��ʱ��
			ATKACT_TIMER
		};

		/// ���ⷴ������
		enum AnswerRet
		{
			AR_RIGHT = 1,
			AR_WRONG = 2,
			AR_TIMEOUT = 3,
		};

		/// ��ʱ��״̬
		enum TimerStatus
		{
			TS_VALID = 0,
			TS_INVALID = -1,
			TS_STOP = -2
		};
	public:
		CPlayerProperty( const CGUID &OwnerID ) : m_OwnerID( OwnerID )
		{
			m_lAnswerTimer= m_lCheckTimer = 
				m_lPKTimer = m_lAtkActTimer = TS_INVALID;
			m_CheckFinish= false;
			m_pOwner = NULL;
#ifdef SCRIPT_QUERY
			InitVarTable();
#endif
		}
 
	public:

		///  ����������Ϣ����Ϣ  
		void EncodeToDataBlock( DBWriteSet &db );

		///  ����Ϣ��������  
		void DecodeFromDataBlock( DBReadSet &db );
		
		///  ��Ϣ
		///  ע�����ж�ʱ����Դ
		void CancelTimers( bool bStop = false );

		///  ��Ҹս���GSʱ����Ҫ��һЩ�߼��жϣ������������ֵ������
		///  ���ߴ���ȣ�������Ҫ�����ű�������
		bool OnEnter();

		/// ����˳����������˳���Ϸ���з���
		/// ��δ����״̬���˳���Ϸ���
		void OnExit( bool bChangeServer );

		///  �ͻ��˻�Ӧ��Ϣ�����������ͻ��˷��ش�����Ϣ�󣬵��ô˺���
		void OnAntiCheatAnswer( int sel );

		///  ���PK����
		void OnFight();

		/// ���������������
		void OnAttack();

		/// ��ҷ��Դ���
		void OnTalk( const std::string &content, const std::string &listener, long talk_type );

		///  �ı��Ծ��ֵ�����ظı��Ļ�Ծ��
		int  ChangeActivity( int changeValue, bool bSend = true );

		///  ������������ߣ�
		void KickPlayer();  

		/// �������Ƿ��оܾ��ش���Ŀ����Ϊ���з��ܾ������������Ӿܾ��ش����
		/// ���ܾ��ش�������ӵ�һ��ֵ��ֱ�ӹ�Ϊ����
		/// ������˳���Ϸ�ܾ�����ֱ�Ӽ������һ��
		void CheckRefuseAnswer( bool bChangeServer );

		/// ������ҵķ���Ҽ��
		bool StartCheck();
		/// �ر�������з���Ҷ�ʱ��
		bool StopCheck();
	private:
		///  ע����ⶨʱ��
		bool ReginsterAntiCheatTimer( bool bFirst = false );			
		///  ע����ⳬʱ��ʱ������������Ŀ���ͻ��˺�ע��ö�ʱ��
		bool RegisterAnswerTimeOutTimer();
		///  ע��Pk��ʱ��
		bool RegisterPkTimer();					
		/// ע��ս����Ծ�ȶ�ʱ��
		bool RegisterAtkActTimer();

		///  ��ʱ��ע�����ص�����
		long OnTimeOut( DWORD timerid, DWORD curtime, const void *var );
		///  ��ʱ��ע��ʱ���õĺ�����������ɾ��һЩ���Ӳ���
		void OnTimerCancel( DWORD timerid, const void *var );
		///  GameEvent�в��ִ��麯����Ҫ�ڴ���д��ֻ��Ҫд���պ�������
		void OnMessage( CMessage* ) {}
		long Proc( eEventType type, const tagProcVar *pVar ) { return 0; }
		///  ע����ʱ��ID
		void CancelTimerId(long &TimerId, TimerStatus s );
		
		///  �����ж϶�ʱ�������ص��������ú�����Ҫ���Ƿ�����Ŀ���߼����
		bool OnAntiCheatTimeOut();
		///  ���ⳬʱ��ʱ����������
		void OnAnswerTimeOut();
		///  ��Ծ���жϺ������ڴ����ж��߼��У����Ƿ���ͻ��˷�����Ŀ���жϣ����ô˺���
		bool OnChangeActivity();
		///  PKֵ�ı��жϺ���
		void OnPKDropTimeOut();
		/// ս����Ծ��˥��������ͬPKֵһ��һֱ˥��
		void OnAtkActDropTimeOut();

		///  �����
		///  ������Ŀ���ͻ���
		void SendQuestionToClient();
		///  �����û�������Ϣ
		void SendResultToClient();

		///  �Ƿ���
		bool IsKickPlayer();

		/// �������Ĵ���
		void OnAnswerWrong();

		/// ��鵱ǰ��������������һ��ֵ��ò���������
		/// �����ǰʱ���Ѿ��ǵڶ��죬�Զ������ֵ
		bool CheckBonusTimes();

		/// ����Ƿ��������ҷ�����Ŀ
		bool CheckSendQuestion();

		/// ����Ƿ���Ըı��Ծ��
		bool CheckChangeActivity();

		/// �������ID��ȡ��ң���Ҫ�Է���ֵ��NULL�ж�
		CPlayer *GetPlayer() const;

		/// �������ID��ȡ������ڳ�������Ҫ�Է���ֵ��NULL�ж�
		CRegion *GetPlayerRegion() const;

		/// ���ؿͻ��˴�����
		void SendAnswerRetToClient( AnswerRet ret );

		/// ��Ӵ��������־
		void AppendAnswerLog( const char *log_type );
	private:
		///  �����ж϶�ʱ��ID
		long m_lCheckTimer;
		///  ���ⳬʱ��ʱ��ID
		long m_lAnswerTimer;
		///  PK��ʱ��ID
		long m_lPKTimer;
		/// ս����Ծ�ȶ�ʱ��
		long m_lAtkActTimer;
		///  ��PlayerProperty��Ӧ�����
		CGUID m_OwnerID;	
		/// ���ָ�룬��ÿ�ζ�ʱ������ʱ/�ͻ��˷�����Ϣ/�ű���������ʱ���²��Ҳ����´�ֵ
		CPlayer *m_pOwner;

		/// ��һ������ԣ������浵����
		CachedProperty m_cachedProper;

		///  Ϊ�������Ӹ�������,��Ҫ�Ƿ�ֹ�ظ����⣬��ʱ��Ҳ���ܴ���
		bool  m_CheckFinish;
		///  ��Ŀ��
		int   m_Answer;

		// �ṩ���ű���ѯ������Ե�����
#ifdef SCRIPT_QUERY
	public:
		long GetAttr( const std::string &name )
		{
			return m_scriptQueryer.GetVarValue<long>( name );
		}
	private:
		void InitVarTable();
	private:
		CScriptQuery m_scriptQueryer;
#endif
	};
}
