





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

		//!										ע��һ��������Ϣ
		bool									RegisterObj(const char *pRegName, unsigned long uRTID);	

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
			//!				���ʾ������
			unsigned long	uMaxInstNum;
			//!				������
			char			szName[CLASS_NAME_SIZE];

			tagObjRunInfo(void){memset(this, 0, sizeof(tagObjRunInfo));}
		};

	private:
		//!										��������ʱID����
		unsigned long							m_uRTIDSeed;
		//!
		stdext::hash_map<unsigned long, tagObjRunInfo>	m_mapObjInfo;
		//!										�������
		OutInfoFun								m_OutInfoFun;
		//!										����ļ�
		const char								*m_pOutFile;
	};


	//////////////////////////////////////////////////////////////////////////////////////////
	CObjInfoManage& GetObjInfoManage(void);
};
#endif //! _OBJ_INFO_MANAGE_H_