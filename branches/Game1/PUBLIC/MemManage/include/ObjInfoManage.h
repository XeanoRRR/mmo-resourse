





/*

*/


#include "mmdef.h"

#ifndef _OBJ_INFO_MANAGE_H_
#define _OBJ_INFO_MANAGE_H_



namespace XM_Tools
{
	//////////////////////////////////////////////////////////////////////////////////////////
	//! ������Ϣ����
	//! ����һ��ͬ���࣬���첽�ӿ���Ӧ����Ҫ����
	//////////////////////////////////////////////////////////////////////////////////////////
	class CObjInfoManage
	{
	public:
		CObjInfoManage(void);
		~CObjInfoManage(void);

	public:
		//!										�õ�һ���µ�����ʱID
		unsigned long							GetNewRTID(void);
#ifdef LOG_ALLOC_FREE_INFO
		//!										ע��һ��������Ϣ
		bool									RegisterObj(std::string &strRegName, unsigned long uRTID);	
#else
		//!										ע��һ��������Ϣ
		bool									RegisterObj(unsigned long uRTID);
#endif

		//!										������Ϣ������������·��
		void									SetOutInfoSetup(OutInfoFun pOutInfoFun, const char *pFileName);
		//!										����ʱ������Ϣ���
		void									OutRunTimeObjInfo(void);

	public:
		//!										�����һ������
		bool									AddObj(unsigned long uRTID);
		//!										ɾ����һ������
		bool									DelObj(unsigned long uRTID);
									

	private:
		//! ��������ʱ��Ϣ
		struct tagObjRunInfo
		{
			//!				����ʱID
			unsigned long	uRTID;
			//!				ʵ������
			unsigned long	uInstNum;
#ifdef LOG_ALLOC_FREE_INFO
			//!				������
			char			szName[CLASS_NAME_SIZE];
#endif
		};

	private:
		//!										��������ʱID����
		unsigned long							m_uRTIDSeed;
		//!
		std::map<unsigned long, tagObjRunInfo>	m_mapObjInfo;
		//!										�������
		OutInfoFun								m_OutInfoFun;
		//!										����ļ�
		const char								*m_pOutFile;

#ifdef LOG_ALLOC_FREE_INFO
		//!
		std::map<std::string, unsigned long>	m_mapNameToID;
#endif
	};


	//////////////////////////////////////////////////////////////////////////////////////////
	CObjInfoManage& GetObjInfoManage(void);
};
#endif //! _OBJ_INFO_MANAGE_H_