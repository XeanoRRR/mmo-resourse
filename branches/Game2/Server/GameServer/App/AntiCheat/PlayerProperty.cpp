#include "StdAfx.h"

#include "CConfiger.h"
#include "PlayerProperty.h"
#include "CQuestionManager.h"
#include "AntiCheatCommon.h"
#include "../Player.h"

#define NOWHOUR(nHours) \
	{\
	time_t timep;\
	tm     *ptm; \
	time(&timep); \
	ptm = localtime(&timep); \
	nHours = ptm->tm_hour;	\
	}

#define NOWDATE(nYear,nMonth,nDay) \
	{\
	time_t timep;\
	tm     *ptm; \
	time(&timep); \
	ptm = localtime(&timep); \
	nYear  = ptm->tm_year + 1900 ;   \
	nMonth = ptm->tm_mon  + 1;    \
	nDay   = ptm->tm_mday;	 \
	}

#define BUILD_DATE( var ) \
	{ \
	long year, month, day; \
	NOWDATE( year, month, day ); \
	var = ( ( (long)(year) << 16 ) | ( (long)(month) << 8 ) | (long)(day) ) ; \
	}

/// GM hates AntiCheat.
#define NULL_RET ;
#define FILTER_GM( player, ret ) \
	if( player->IsGM() ) \
	return ret; 

#define FILTER_LEVEL( lev , ret )\
	if( lev < (long) GetInst( CConfiger ).GetSystem().LevelFilter )	\
	return ret;\

#define LOG_LOGIN_TYPE "LOGIN"
#define LOG_KICK_TYPE "KICK"

/// answer operation type
#define LOG_OPER_ANSWER "ANSWER"
#define LOG_OPER_TIMEOUT "TIMEOUT"
#define LOG_OPER_REFUSED "REFUSED"
#define LOG_OPER_EXIT "EXIT"

///
/// calculate the rest seconds in this day.
///
static unsigned long GetRestSeconds()
{
	time_t timep;
	tm *ptm;
	time( &timep );
	ptm = localtime( &timep );
	return ( 23 - ptm->tm_hour ) * 60 * 60 + ( 60 - ptm->tm_min ) * 60 + ( 60 - ptm->tm_sec ) ;
}

static const char *GuidToString( const CGUID &guid )
{
	static char s_str[128];
	guid.tostring( s_str );
	return s_str;
}

namespace AntiCheat
{
	//	���뵽��Ϣ
	void CPlayerProperty::EncodeToDataBlock(DBWriteSet &db )
	{
		db.AddToByteArray( &m_cachedProper, sizeof( m_cachedProper ) );
	}

	// ����Ϣ��������
	void CPlayerProperty::DecodeFromDataBlock(DBReadSet &db )
	{
		db.GetBufferFromByteArray( &m_cachedProper, sizeof( m_cachedProper ) );
	}

	// ��������
	void CPlayerProperty::SendQuestionToClient()
	{
		// �����µ���Ŀ
		size_t  QLen  =0;
		void    *Qbuf =NULL;

		// ���ܵõ���Ŀ
		if (!GetInst(CQuestionManager).GetRandQuestion(&Qbuf,&QLen,&m_Answer))
		{
			// ���������ж϶�ʱ��
			ReginsterAntiCheatTimer();
			return ;
		}

		m_CheckFinish = true;

		CMessage msg( MSG_S2C_AC_QUESTION );
		msg.Add((long)QLen);
		msg.AddEx(Qbuf,(long)QLen);    
		msg.SendToPlayer( m_OwnerID, false );

		// ע��ش�𰸳�ʱ��ʱ��
		RegisterAnswerTimeOutTimer();

		AddAccQues();
	}

	// �������߼��ж�
	// ��Ҹս�����Ϸ�Ĳ���
	bool CPlayerProperty::OnEnter()
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
            Log4c::Error(ROOT_MODULE,"%-15s Invalid player [%s], startup failed.",__FUNCTION__,GuidToString( m_OwnerID ));
			return true;
		}

		FILTER_GM( m_pOwner, true );
		FILTER_LEVEL( m_pOwner->GetLevel() , true );

		if ( IsKickPlayer() || true ) /* ���÷�Ż��ƺ󣬽�����Ϸ����Ҫ���з�ż�� */
		{
			// �����Ҿܾ��������
			if( m_cachedProper.nRefuseTimes >= GetInst( CConfiger ).GetSystem().RefuseMax )
			{
				// ��һ�δ������ͷ�
				OnAnswerWrong();
				m_cachedProper.nRefuseTimes = 0;
				SendAnswerRetToClient( AR_WRONG );
				AppendAnswerLog( LOG_OPER_REFUSED );
			}

			// ����ϴ�����ʱδ�����Ľű�
			if( m_cachedProper.saved.m_bScriptExecuted == 1 )
			{
				// ����һ�νű�
				GetInst(CConfiger).OnAnswerWrong( m_pOwner, m_cachedProper.saved.nCurWrongTimes);
				m_cachedProper.saved.m_bScriptExecuted = 0;
			}

			// ����Ƿ���������������
			CheckBonusTimes();

			// ��ʼ������Ϸʱ��
			BUILD_DATE( m_cachedProper.saved.nLastGameTime  );
			//
			m_pOwner->AddNewState( m_pOwner, STATE_BASE_AC_ENTER, 1, 23);
			return true;
		}
		CMessage msg(MSG_S2C_AC_LOGINFAILED);
		msg.SendToPlayer( m_OwnerID , false );
		return false;
	}

	bool CPlayerProperty::StartCheck()
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
            Log4c::Error(ROOT_MODULE,"%-15sInvalid player [%s].",__FUNCTION__, GuidToString( m_OwnerID ));

			return 0;
		}
		// ������ʱ��
		FILTER_GM( m_pOwner, true );
		FILTER_LEVEL( m_pOwner->GetLevel() , true );
		if( m_lCheckTimer != TS_INVALID )
		{
			return false;
		}
		ReginsterAntiCheatTimer(true);
		RegisterAtkActTimer();
		RegisterPkTimer();
		return true;
	}

	bool CPlayerProperty::StopCheck()
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
            Log4c::Error(ROOT_MODULE,"%-15s Invalid player [%s].",__FUNCTION__,GuidToString( m_OwnerID ));
			return 0;
		}
		CancelTimers(true);
		return true;
	}

	void CPlayerProperty::OnExit( bool bChangeServer )
	{

	}

	// �����ж϶�ʱ�������ص��������ú�����Ҫ���Ƿ�����Ŀ���߼����
	bool CPlayerProperty::OnAntiCheatTimeOut()
	{
		// ��Ծ������,����ֵΪ�٣���ʾ��������Ŀ
		if (!OnChangeActivity())
		{
			// ����ע����ⶨʱ��
			ReginsterAntiCheatTimer();
			return false;
		}

		if( !CheckSendQuestion() )
		{
			// ��������Ŀ��������ע����ⶨʱ��
			ReginsterAntiCheatTimer();
			return false; 
		}
		else
		{
			SendQuestionToClient();
		}
		return true;
	}

	bool CPlayerProperty::CheckSendQuestion()
	{
		if( m_CheckFinish )
		{
			// �Ѿ���������Ŀ���ͻ��ˣ����ڵȴ�Ӧ���״̬�����ظ�����
			return false;
		}

		// ����Ծ�ȴ����ActivityMaxʱ��������
		if (m_cachedProper.saved.nActFactor >= GetInst(CConfiger).GetSystem().ActivityMax)
		{
			// ��ҵ�ͼ����,Ϊ0���ͼ��ʱ��ֹͣ����
			CRegion *pRegion = GetPlayerRegion();
			if( pRegion == NULL )
			{
				Log4c::Error(ROOT_MODULE,"%-15s The region for [%s] does not exist.",__FUNCTION__, GuidToString( m_OwnerID ) );
				return false;
			}
			if( pRegion->GetACRgnType() == CRegion::ACRT_FIELD )
			{
				// �Ƿ񳬹�������,����ʱ���ȥ�ϴδ���ʱ��
				if ( (long)(time(NULL) - m_cachedProper.LastAnswerTime ) > GetInst(CConfiger).GetSystem().AnswerInterval)
				{
					// �Ƿ���Pk״̬
					if( m_cachedProper.nAntiCheatPK > 0 )
					{
						if ( (long)(time(NULL) - m_cachedProper.LastAnswerTime ) > GetInst(CConfiger).GetSystem().PkTimeInterval)
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}

		return false;
	}

	bool CPlayerProperty::CheckChangeActivity()
	{
		int nHours;
		NOWHOUR(nHours);
		AntiCheat::Range RangeTime = GetInst(CConfiger).GetSystem().RushHourRange;
		// �ж��Ƿ��ڻƽ�ʱ��
		if( nHours >= RangeTime.first && nHours <= RangeTime.second )   
		{
			// �ڻƽ�ʱ�������ȷ��������Ҫ���ӻ�Ծ��
			if( m_cachedProper.saved.m_bRHAnswered == 1)
			{
				return false;
			}
		}
		else
		{
			m_cachedProper.saved.m_bRHAnswered = 0;
		}
		// ��ҵ�ͼ����,Ϊ0���ͼ��ʱ��ֹͣ����
		CRegion *pRegion = GetPlayerRegion(); 
		if( pRegion == NULL )
		{
			Log4c::Warn(ROOT_MODULE,"The region for [%s] does not exist.",GuidToString( m_OwnerID ) );
			return false;
		}
		if( pRegion->GetACRgnType() == CRegion::ACRT_ACTION )
		{
			return false;
		}

		return true;
	}

	// �ı���ҵ�Pkֵ 
	void CPlayerProperty::OnPKDropTimeOut()
	{
		m_cachedProper.nAntiCheatPK -= GetInst( CConfiger ).GetSystem().PkDropValue;
		if( m_cachedProper.nAntiCheatPK < 0 )
		{
			m_cachedProper.nAntiCheatPK = 0;
			// ��������Ƿ�����Ŀ
			if( CheckSendQuestion() )
			{
				CancelTimerId( m_lCheckTimer, TS_INVALID );
				SendQuestionToClient();
			}
		}
	}

	// ս����Ծ��˥��
	void CPlayerProperty::OnAtkActDropTimeOut()
	{
		m_cachedProper.nAtkAct -= GetInst( CConfiger ).GetSystem().AtkActivityDropValue;
		if( m_cachedProper.nAtkAct < 0 )
		{
			m_cachedProper.nAtkAct = 0;
		}
	}

	// ����ʱ����δ�ش��,��ʱ��
	void CPlayerProperty::OnAnswerTimeOut()
	{
		// ����Ѿ�����⣬�������ز�����
		if (!m_CheckFinish)
		{
			return ;
		}
		m_CheckFinish = false;
		m_Answer = -1;

		// ��ʱ��Ծ����0
		m_cachedProper.saved.nActFactor = 0;

		// �������ֵ+1
		m_cachedProper.saved.nAccWrongTimes ++; 
		// �������ֵ+1
		m_cachedProper.saved.nCurWrongTimes ++; 
		// ���������Ŀ�ű�
		GetInst(CConfiger).OnAnswerWrong( m_pOwner,m_cachedProper.saved.nCurWrongTimes);

		// ע����ʱ��
		CancelTimerId(m_lAnswerTimer,TS_INVALID);
		CancelTimerId(m_lCheckTimer,TS_INVALID);
		// ����ע����ⶨʱ��
		ReginsterAntiCheatTimer();

		AddAccTimeOut();
		SendAnswerRetToClient( AR_TIMEOUT );
		AppendAnswerLog( LOG_OPER_TIMEOUT );
	}

	// ��Ծ�ȵ��ж�,���ݷ��ص�ֵ���Ƿ�Ϊ�������Ϣ
	bool CPlayerProperty::OnChangeActivity()
	{
		//  ��Ծ���Ѿ����,������Ծ��,��ʱ���Է�����Ŀ
		if (m_cachedProper.saved.nActFactor >= GetInst(CConfiger).GetSystem().ActivityMax )
		{
			m_cachedProper.saved.nActFactor = GetInst(CConfiger).GetSystem().ActivityMax;
			return true;
		}

		if( CheckChangeActivity() )
		{
			// �����ս��״̬����ս����Ծ��
			if( m_cachedProper.nAtkAct > 0 && m_pOwner->GetState() == CShape::STATE_FIGHT )
			{
				const AntiCheat::Range &range = GetInst(CConfiger).GetSystem().FightActivityRange;
				m_cachedProper.saved.nActFactor +=range.first + random( range.second - range.first + 1 );
			}
		}
		return true;
	}
	//  �ı��Ծ�ȵ�ֵ,���ᳬ�����ֵ
	int CPlayerProperty::ChangeActivity(int changeValue, bool bSend )
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
			Log4c::Error(ROOT_MODULE,"%-15s Invalid player [%s].",__FUNCTION__, GuidToString( m_OwnerID ) );
			return 0;
		}
		FILTER_GM( m_pOwner, 0 );
		FILTER_LEVEL( m_pOwner->GetLevel() , 0 );
		if( CheckChangeActivity() )
		{
			m_cachedProper.saved.nActFactor += changeValue;
			if( m_cachedProper.saved.nActFactor > GetInst( CConfiger ).GetSystem().ActivityMax )
			{
				m_cachedProper.saved.nActFactor = GetInst( CConfiger ).GetSystem().ActivityMax;
			}

			if( bSend && CheckSendQuestion() )
			{
				SendQuestionToClient();
			}
		}
		return m_cachedProper.saved.nActFactor; 
	}

	//  ���PK����
	void CPlayerProperty::OnFight()
	{
		FILTER_GM( m_pOwner, NULL_RET );
		FILTER_LEVEL( m_pOwner->GetLevel() , NULL_RET );
		m_cachedProper.nAntiCheatPK = GetInst(CConfiger).GetSystem().PkAddValue;
	}

	// ���������������
	void CPlayerProperty::OnAttack()
	{
		FILTER_GM( m_pOwner, NULL_RET );
		FILTER_LEVEL( m_pOwner->GetLevel() , NULL_RET );
		m_cachedProper.nAtkAct = GetInst( CConfiger ).GetSystem().AtkActivityAddValue;
	}

	// ��ҷ��Դ���
	void CPlayerProperty::OnTalk( const std::string &content, const std::string &listener, long talk_type )
	{
		bool bAddAct = false;
		CConfiger &configer = GetInst( CConfiger );
		switch( talk_type )
		{
		case CHAT_NORMAL:
			{
				bAddAct = configer.IsCheckTalk( CConfiger::eTC_NORMAL );
			}
			break;

		case CHAT_REGION:
			{
				bAddAct = configer.IsCheckTalk( CConfiger::eTC_REGION );
			}
			break;

		case CHAT_PRIVATE:
			{
				if( listener.size() == 0 ||
					!configer.IsCheckTalk( CConfiger::eTC_PRIVATE ) || 
					CheckRelation( m_pOwner, listener, PR_ALL ) )
				{
					return;
				}
				bAddAct = true;
			}
			break;

		case CHAT_TEAM:
			{
				bAddAct = configer.IsCheckTalk( CConfiger::eTC_TEAM );
			}
			break;

		case CHAT_FACTION:
			{
				bAddAct = configer.IsCheckTalk( CConfiger::eTC_FACTION );
			}
			break;
		}

		if( bAddAct )
		{
			const Range &r = configer.GetSystem().TalkActivityRange;
			int AddAct = ::random( r.second - r.first + 1 ) + r.first;
			ChangeActivity( AddAct );
			// extra add
			if( configer.GetSystem().CheckSensWords != 0 && configer.HasSensWord( content ) )
			{
				const Range &r2 = configer.GetSystem().TalkSensActivityRange;
				AddAct = ::random( r2.second - r2.first + 1 ) + r2.first;
				ChangeActivity( AddAct );
			}
		}
	}

	//  ������ҵĴ���Դ�����ж�
	void CPlayerProperty::OnAntiCheatAnswer(int sel )
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
			Log4c::Error(ROOT_MODULE,"%-15s Invalid player when received message from client [%s].",__FUNCTION__,GuidToString( m_OwnerID ) );
			return ;
		}
		FILTER_GM( m_pOwner, NULL_RET );
		FILTER_LEVEL( m_pOwner->GetLevel() , NULL_RET );
		// ����Ѿ�����⣬�������ز�����
		if (!m_CheckFinish)
		{
			Log4c::Warn(ROOT_MODULE, "Multiple messages responsed from client [%s], ignore it.", m_pOwner->GetName() );
			return ;
		}
		m_CheckFinish = false;

		// �޸��ϴδ���
		m_cachedProper.LastAnswerTime = (long)time(NULL);

		// ���ж�,�Ȼ�Ծ����0
		m_cachedProper.saved.nActFactor = 0;

		// ����ȷ
		if (sel == m_Answer)
		{
			// �����ش����+1
			m_cachedProper.saved.nCurRightTimes ++;
			// �����������ֵ
			if (m_cachedProper.saved.nCurRightTimes>0 && (GetInst(CConfiger).GetSystem().DropCurWrongValue) >0 && (m_cachedProper.saved.nCurRightTimes % (GetInst(CConfiger).GetSystem().DropCurWrongValue) ) ==0 )
			{
				m_cachedProper.saved.nCurWrongTimes = (m_cachedProper.saved.nCurWrongTimes-1)>=0 ? (m_cachedProper.saved.nCurWrongTimes-1):0;
			}
			// ���ܽ���
			bool bCanBonus = CheckBonusTimes();
			// ������ȷ��Ŀ�ű�
			GetInst(CConfiger).OnAnswerRight(m_pOwner,m_cachedProper.saved.nCurRightTimes, !bCanBonus );
			if( bCanBonus )
			{
				// ���ӽ�������
				m_cachedProper.saved.nBonusTimes ++;
			}
			/// ���ûƽ�ʱ�������ȷ��־ 
			int nHours;
			NOWHOUR(nHours);
			AntiCheat::Range RangeTime = GetInst(CConfiger).GetSystem().RushHourRange;
			if (nHours >= RangeTime.first && nHours <= RangeTime.second )   
			{
				m_cachedProper.saved.m_bRHAnswered = 1;
			}
			else
			{
				m_cachedProper.saved.m_bRHAnswered = 0;
			}

			AddAccRight();
			SendAnswerRetToClient( AR_RIGHT );
		}
		else 
		{
			OnAnswerWrong();	
			SendAnswerRetToClient( AR_WRONG ); 
		}

		m_Answer = -1;

		// ע�����ⳬʱ��ʱ��
		CancelTimerId(m_lAnswerTimer,TS_INVALID);
		CancelTimerId(m_lCheckTimer,TS_INVALID);
		// ����ע����ⶨʱ��
		ReginsterAntiCheatTimer();
		AppendAnswerLog( LOG_OPER_ANSWER );
	}


	//  ע������ж϶�ʱ��,ֻ����һ�εĶ�ʱ��
	bool CPlayerProperty::ReginsterAntiCheatTimer( bool bFirst)
	{
		if (m_lCheckTimer == TS_STOP)
		{
			return false;
		}
		if (!bFirst)
		{
			const Range &r = GetInst( CConfiger ).GetSystem().CheckIntervalRange;
			long lTime = ( random( r.second - r.first + 1 ) + r.first ) * 1000;
			tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
			pTimvar->TimerType = (eChildObjType)ANTICHEAT;
			m_lCheckTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime() + lTime,0);
			return true;
		}
		else
		{
			// ����,�з����ߵ�һ��¼
			int lTime = 0; //60 * 1000;
			tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
			pTimvar->TimerType = (eChildObjType)ANTICHEAT;
			m_lCheckTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime() + lTime,0);
			return true;
		}

	}

	// ע��Pk��ʱ����һֱ����
	bool CPlayerProperty::RegisterPkTimer()
	{
		if (m_lPKTimer !=-1 || m_lPKTimer == TS_STOP )
			return false;
		int lTime = GetInst(CConfiger).GetSystem().PkDropInterval * 1000;
		tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
		pTimvar->TimerType = (eChildObjType)PKTIMER;
		m_lPKTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime() + lTime,lTime);
		return true;
	}

	// ע��ս����Ծ�ȶ�ʱ��
	bool CPlayerProperty::RegisterAtkActTimer()
	{
		if (m_lAtkActTimer >= TS_VALID || m_lAtkActTimer == TS_STOP )
			return false;
		long lTime = GetInst(CConfiger).GetSystem().AtkActivityDropInterval * 1000;
		tagTimerVar *pTimvar =  OBJ_CREATE(tagTimerVar);
		pTimvar->TimerType = (eChildObjType)ATKACT_TIMER;
		m_lAtkActTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime() + lTime,lTime);
		return true;
	}

	// ע����ⳬʱ��ʱ����ֻ����һ�εĶ�ʱ��
	bool CPlayerProperty::RegisterAnswerTimeOutTimer()
	{
		if (m_lCheckTimer == TS_STOP)
		{
			return false;
		}
		int lTime = GetInst(CConfiger).GetSystem().AnswerTimeOutInterval * 1000;
		tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
		pTimvar->TimerType = (eChildObjType)ANSTIMER;
		m_lAnswerTimer= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime() + lTime,0);
		return true;
	}

	// ��ʱ
	long CPlayerProperty::OnTimeOut(DWORD timerid, DWORD curtime, const void *var )
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
			CancelTimers();
			Log4c::Error(ROOT_MODULE,"%-15sThe player [%s] is invalid.",__FUNCTION__, GuidToString( m_OwnerID ) );
			return 0;
		}

		const tagTimerVar *pTimervar = (const tagTimerVar *)var	;

		assert(pTimervar!=NULL);
		switch(pTimervar->TimerType)
		{
		case ANTICHEAT:
			OnAntiCheatTimeOut();
			break;

		case PKTIMER:
			OnPKDropTimeOut();
			break;

		case ANSTIMER:
			OnAnswerTimeOut();
			break;

		case ATKACT_TIMER:
			OnAtkActDropTimeOut();
			break;

		default:
			Log4c::Warn(ROOT_MODULE, "Unknown timer [%d].", pTimervar->TimerType );
			break;	
		}	
		return 0;
	}


	/// ע�����ж�ʱ��
	void CPlayerProperty::CancelTimers(bool bStop )
	{
		TimerStatus timer_status = ( bStop ? TS_STOP : TS_INVALID );

		CancelTimerId( m_lAnswerTimer, timer_status );
		CancelTimerId( m_lCheckTimer, timer_status );
		CancelTimerId( m_lPKTimer, timer_status );
		CancelTimerId( m_lAtkActTimer, timer_status );
	}

	/// ע����ʱ��Id
	void CPlayerProperty::CancelTimerId(long &TimerId ,TimerStatus s)
	{
		if (TimerId >= TS_VALID )
		{
			GameManager::GetInstance()->Cancel(TimerId,0);
			TimerId = s;
		}
	}

	/// ɾ����ʱ��var
	void CPlayerProperty::OnTimerCancel(DWORD timerid, const void *var )
	{
		if (var != NULL)
		{
			tagTimerVar *pTimervar = (tagTimerVar*)var;
			OBJ_RELEASE(tagTimerVar, pTimervar );
		}
	}

	bool CPlayerProperty::IsKickPlayer()
	{
		if (m_cachedProper.saved.bBanFlag == 0)
		{
			return true;
		}
		else
		{
			long now ;
			BUILD_DATE( now );
			// ����GM��Ż��ƺ�ֻҪ����GS�����ʾ�Ѿ���� 
			m_cachedProper.saved.bBanFlag = 0;
			GetLogicLogInterface()->logT900_anticheat_player_log(m_pOwner, LOG_LOGIN_TYPE,
				m_cachedProper.saved.nAccWrongTimes, m_cachedProper.saved.nCurWrongTimes, m_cachedProper.saved.nBonusTimes );
			return false;
		}
	}

	void CPlayerProperty::OnAnswerWrong()
	{
		m_cachedProper.saved.nActFactor = 0;
		m_cachedProper.saved.nCurRightTimes =0;
		m_cachedProper.saved.nAccWrongTimes ++;
		m_cachedProper.saved.nCurWrongTimes ++;
		/// ���������Ŀ�ű�
		GetInst(CConfiger).OnAnswerWrong(m_pOwner,m_cachedProper.saved.nCurWrongTimes);

		AddAccWrong();
	}

	bool CPlayerProperty::CheckBonusTimes()
	{
		long now;
		BUILD_DATE( now );
		if( now > m_cachedProper.saved.nLastGameTime )
		{
			m_cachedProper.saved.nBonusTimes = 0;
			// ���µ�¼ʱ��Ϊ�ڶ���
			BUILD_DATE( m_cachedProper.saved.nLastGameTime );
			return true;
		}
		else if( m_cachedProper.saved.nBonusTimes >= GetInst( CConfiger ).GetSystem().BonusMax )
		{
			return false;
		}
		return true;
	}

	void CPlayerProperty::KickPlayer()
	{
		m_pOwner = GetPlayer();
		if( m_pOwner == NULL )
		{
			Log4c::Error(ROOT_MODULE,"%-15s Invalid player [%s].",__FUNCTION__, GuidToString( m_OwnerID ) );
			return ;
		}
		FILTER_GM( m_pOwner, NULL_RET );
		FILTER_LEVEL( m_pOwner->GetLevel() , NULL_RET );
		GetLogicLogInterface()->logT900_anticheat_player_log(m_pOwner,LOG_KICK_TYPE, 
			m_cachedProper.saved.nAccWrongTimes, m_cachedProper.saved.nCurWrongTimes, m_cachedProper.saved.nBonusTimes );

		m_cachedProper.saved.bBanFlag = 1 ;
		BUILD_DATE( m_cachedProper.saved.nLastGameTime );
		m_cachedProper.saved.nCurWrongTimes=0;
		m_CheckFinish = false;
		std::string name( m_pOwner->GetName() );
		GetGame()->KickPlayer(m_pOwner->GetExID());		

		// ���
		long lBanSecs = GetInst( CConfiger ).GetSystem().BanHours * 60 * 60;
		CMessage msg( MSG_S2W_GM_BANPLAYER );
		msg.Add( NULL_GUID );
		msg.Add( name.c_str() );
		msg.Add( (long) lBanSecs );
		msg.Send();
	}

	void CPlayerProperty::CheckRefuseAnswer( bool bChangeServer )
	{
		if( m_CheckFinish )
		{
			if( bChangeServer )
			{
				m_cachedProper.nRefuseTimes ++;
			}
			else
			{
				m_cachedProper.saved.nActFactor = 0;
				m_cachedProper.saved.nCurRightTimes =0;
				m_cachedProper.saved.nAccWrongTimes ++;
				m_cachedProper.saved.nCurWrongTimes ++;
				// ��־�´�������Ҫ��������ű�
				m_cachedProper.saved.m_bScriptExecuted = 1; 

				AddAccWrong();
				AppendAnswerLog( LOG_OPER_EXIT );
			}
		}
	}

	CPlayer *CPlayerProperty::GetPlayer() const
	{
		return GetGame()->FindPlayer( m_OwnerID );
	}

	CRegion *CPlayerProperty::GetPlayerRegion() const
	{
		CPlayer *pPlayer = GetPlayer();
		if( pPlayer == NULL )
		{
			return NULL;
		}
		return static_cast<CRegion*>( pPlayer->GetFather() );
	}

	void CPlayerProperty::SendAnswerRetToClient( AnswerRet ret )
	{
		CMessage msg( MSG_S2C_AC_ANSWER_RET );
		msg.Add( (BYTE) ret );
		msg.SendToPlayer( m_OwnerID, false );
	}

	void CPlayerProperty::AppendAnswerLog( const char *log_type )
	{
		GetLogicLogInterface()->logT901_anticheat_answer_log( m_pOwner, 
			log_type, m_cachedProper.saved.nAccWrongTimes, m_cachedProper.saved.nCurWrongTimes, 
			m_cachedProper.saved.nCurRightTimes, m_cachedProper.saved.nBonusTimes, 
			m_cachedProper.nAntiCheatPK, m_cachedProper.saved.m_bRHAnswered != 0 );
	}

#ifdef SCRIPT_QUERY

	void CPlayerProperty::InitVarTable()
	{
		m_scriptQueryer.AddVar( "Activity", m_cachedProper.saved.nActFactor );
		m_scriptQueryer.AddVar( "AccWrong", m_cachedProper.saved.nAccWrongTimes );
		m_scriptQueryer.AddVar( "CurWrong", m_cachedProper.saved.nCurWrongTimes );
		m_scriptQueryer.AddVar( "CurRight", m_cachedProper.saved.nCurRightTimes );
		m_scriptQueryer.AddVar( "PKValue", m_cachedProper.nAntiCheatPK );
		m_scriptQueryer.AddVar( "LastAnswerTime", m_cachedProper.LastAnswerTime );
		m_scriptQueryer.AddVar( "Refused", m_cachedProper.nRefuseTimes );
		m_scriptQueryer.AddVar( "Bonus", m_cachedProper.saved.nBonusTimes );
		m_scriptQueryer.AddVar( "RHAnswered", m_cachedProper.saved.m_bRHAnswered );
		m_scriptQueryer.AddVar( "AtkAct", m_cachedProper.nAtkAct );
	}

#endif
}