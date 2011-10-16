





/*
���ļ��������ڴ�������Ϣ��¼

*/

#ifndef _MEM_INFO_H_
#define _MEM_INFO_H_

#include "mmdef.h"

namespace XM_Tools
{

	//!
	class CMemInfo
	{
	public:
		CMemInfo(void);
		~CMemInfo(void);

	public:
		//!				�õ���ǰ��ʱ������
		static void		GetTimeFrequencyCount(LARGE_INTEGER *pOutCount){QueryPerformanceCounter(pOutCount);}

		//!				��¼һ���ڴ��������Ϣ
		void			LogMemOptInfo(bool IsAlloc, unsigned long MemSize, LARGE_INTEGER &OptCount);
		//!				�����Ϣ
		void			OutInfo(std::string &strOutInfo, bool Detailed);

	private:

		//!		ʱ����Ϣ
		struct tagTimeInfo 
		{
			//! ����
			unsigned long	Degree;
			//! �ܺ�ʱ
			LARGE_INTEGER	AllTime;
			//! ����ʱ
			LARGE_INTEGER	MaxTime;

			tagTimeInfo(void){memset(this, 0, sizeof(tagTimeInfo));}
		};

		//!		������Ϣ
		struct	tagOptInfo 
		{
			//! ��ǰ����
			long			CurrCount;
			//! ���ʹ������
			long			MaxUseCount;
			//! ����ʱ����Ϣ
			tagTimeInfo		AllocTimeInfo;
			//! �ͷ�ʱ����Ϣ
			tagTimeInfo		FreeTimeInfo;

			tagOptInfo(void):CurrCount(0),MaxUseCount(0){}
		};


		typedef stdext::hash_map<unsigned long, tagOptInfo>			InfoGroup;
		typedef InfoGroup::iterator							InfoItr;


	private:
		//!					
		InfoGroup			m_InfoGroup;
		//!					ʼ��Ƶ��
		LARGE_INTEGER		m_FrequencyTime;
	};
};

#endif