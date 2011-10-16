///
/// @file QuestionManager.h
/// @author Kevin Lynx
/// @brief manage the questions cache 
///
#ifndef ___QUESTION_MANAGER_H_
#define ___QUESTION_MANAGER_H_


namespace AntiCheat
{
	class CQuestionManager
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
		typedef std::vector<Question> QuestionList;

	public:
		CQuestionManager()
		{
		}

		~CQuestionManager()
		{
			ClearQuestions();
		}

		/// ������Ŀ����
		void UpdateQuestions( DBReadSet &db );

		/// �����Ŀ����
		void ClearQuestions();

		/// �����ȡһ����Ŀ������ɾ�����ص��ڴ�
		bool GetRandQuestion( void **pBuf, size_t *size, int *answer );

	private:
		QuestionList m_Questions;
	};
}

#endif