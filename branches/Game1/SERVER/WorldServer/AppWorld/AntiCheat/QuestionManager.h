///
/// @file QuestionManager.h
/// @author Kevin Lynx
/// @brief manage the questions generation and cache details.
///
#ifndef ___QUESTION_MANAGER_H_
#define ___QUESTION_MANAGER_H_

namespace AntiCheat
{
	class CQuestionManager : public CGameEvent
	{
	private:
		struct BufferInfo
		{
			void *buf;
			size_t size;
		};

		struct Question
		{
			BufferInfo data;
			int answer;
		};
		typedef std::list<Question> QuestionList;

		/// event types
		enum
		{
			ET_QUIT = 0,
			ET_GEN,
			ET_COUNT
		};

		typedef CTimerQueue<CGameEvent*> TimerType;

	public:
		CQuestionManager() : m_Timer( 1024, 1000 )
		{
			m_pQuestions = NULL;
			m_UpdateTimer = -1;
		}

		/// �������ܣ������ɵ�һ����Ŀ�������̣߳�ע�ᶨʱ���ȡ�
		bool Startup();

		/// �رղ����������Ŀ���棬�ر��̣߳�ע����ʱ����
		void Shutdown();

		/// ÿ֡����
		void Run();

		/// ������Ŀ���ݸ�ָ��GS��GS������WSʱ���ô˺�����
		void SendQuestionsToGS( long lGSSocketID );

	private:
		long OnTimeOut( DWORD timerid, DWORD curtime, const void *var ) ;
		void OnTimerCancel( DWORD timerid, const void *var ) {}
		void OnMessage( CMessage* ) {}
		long Proc( eEventType type, const tagProcVar *pVar ) { return 0; }
	private:

		/// �̻߳ص���׼��������Ŀ��������Ŀ����
		void OnThreadCall();

		/// ����һ����Ŀ�����ص��ڴ���Ҫ�Լ��ͷ� : free
		Question GenOneQuestion();

		/// ������������һ����Ŀ�����ص�����������Ҫ�Լ��ͷ� : delete
		QuestionList *GenQuestions();

		/// ������Ŀ���棬�˺����������Դ����
		void UpdateQuestionCache( QuestionList *pQuestions );

		/// ������Ŀ���棬�˺����������Դ����
		void EncodeQuestionCache( DBWriteSet &db );

		/// ������Ŀ���浽����GS
		void UpdateQuestionsToAll();

		/// �����Ŀ����
		void ReleaseQuestionCache( QuestionList *pQuestions );

		/// ������Ŀ�߳�
		static  unsigned int __stdcall threadFunc( void *p );
	private:
		QuestionList *m_pQuestions;
		long m_UpdateTimer;
		CRITICAL_SECTION m_QuesMutex;
		HANDLE m_Events[ET_COUNT];
		HANDLE m_Thread;
		TimerType m_Timer;
	};
}

#endif