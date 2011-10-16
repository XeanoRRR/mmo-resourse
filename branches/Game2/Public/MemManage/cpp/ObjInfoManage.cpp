

#ifndef _OBJ_INFO_MANAGE_CPP_
#define _OBJ_INFO_MANAGE_CPP_



#include "../include/ObjInfoManage.h"

namespace XM_Tools
{
	//////////////////////////////////////////////////////////////////////////////////////////
	//! ������Ϣ����
	//////////////////////////////////////////////////////////////////////////////////////////



	CObjInfoManage::CObjInfoManage(void)
		:m_uRTIDSeed(0)
		,m_OutInfoFun(NULL)
		,m_pOutFile(NULL)
	{
	}

	CObjInfoManage::~CObjInfoManage(void)
	{
		if(NULL == m_OutInfoFun || NULL == m_pOutFile)
			return;

		std::string strTempInfo("---------------------------�����������Ϣ---------------------------\r\n");
		if(0 != m_mapObjInfo.size())
		{
			for (std::map<unsigned long, tagObjRunInfo>::iterator ite = m_mapObjInfo.begin(); m_mapObjInfo.end() != ite; ++ite)
			{
				if (0 != ite->second.uInstNum)
				{
					char szObjInfo[512] = {0};
					sprintf(szObjInfo, "%-64s��%u��û�ͷţ�����\r\n", ite->second.szName, ite->second.uInstNum);
					strTempInfo += szObjInfo;
				}
			}
		}

		strTempInfo += "-------------------------------------------------------------------\r\n";
		m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
	}

	//! ������Ϣ������������·��
	void CObjInfoManage::SetOutInfoSetup(OutInfoFun pOutInfoFun, const char *pFileName)
	{
		m_OutInfoFun = pOutInfoFun;
		m_pOutFile = pFileName;
	}


	//! ����ʱ������Ϣ���
	void CObjInfoManage::OutRunTimeObjInfo(void)
	{
		if(NULL == m_OutInfoFun || NULL == m_pOutFile)
			return;

		std::string strTempInfo("---------------------------���������ʱ��Ϣ---------------------------\r\n");
		if(0 != m_mapObjInfo.size())
		{
			for (std::map<unsigned long, tagObjRunInfo>::iterator ite = m_mapObjInfo.begin(); m_mapObjInfo.end() != ite; ++ite)
			{
				if (0 < ite->second.uInstNum)
				{
					char szObjInfo[512] = {0};
					sprintf(szObjInfo, "%-64s��%8u(max:%8u)����\r\n", ite->second.szName, ite->second.uInstNum, ite->second.uMaxInstNum);
					strTempInfo += szObjInfo;
				}
			}
		}

		strTempInfo += "-------------------------------------------------------------------\r\n";
		m_OutInfoFun(m_pOutFile, strTempInfo.c_str());
	}

	//! �õ�һ���µ�����ʱID
	unsigned long CObjInfoManage::GetNewRTID(void)
	{
		return ++m_uRTIDSeed;
	}

	//! ע��һ��������Ϣ
	bool CObjInfoManage::RegisterObj(const char *pRegName, unsigned long uRTID)
	{
		if(m_mapObjInfo.end() == m_mapObjInfo.find(uRTID))
		{
			tagObjRunInfo ObjRunInfo;
			ObjRunInfo.uRTID	= uRTID;
			strcpy_s(ObjRunInfo.szName, CLASS_NAME_SIZE, pRegName);
			m_mapObjInfo.insert(std::make_pair(uRTID, ObjRunInfo));

			return true;
		}
		
		RETURN_THROW(false);
	}

	//! �����һ������
	bool CObjInfoManage::AddObj(unsigned long uRTID)
	{
		std::map<unsigned long, tagObjRunInfo>::iterator ite = m_mapObjInfo.find(uRTID);
		if(m_mapObjInfo.end() != ite)
		{
			ite->second.uInstNum += 1;
			if(ite->second.uMaxInstNum < ite->second.uInstNum)
				ite->second.uMaxInstNum = ite->second.uInstNum;
			return true;
		}
		return false;
	}

	//! ɾ����һ������
	bool CObjInfoManage::DelObj(unsigned long uRTID)
	{
		std::map<unsigned long, tagObjRunInfo>::iterator ite = m_mapObjInfo.find(uRTID);
		if(m_mapObjInfo.end() != ite)
		{
			if(0 < ite->second.uInstNum)
			{
				ite->second.uInstNum -= 1;
				return true;
			}
		}
		return false;
	}

	CObjInfoManage& GetObjInfoManage(void)
	{
		static CObjInfoManage c_ObjInfoManage;
		return c_ObjInfoManage;
	}
};

#endif //! _OBJ_INFO_MANAGE_CPP_