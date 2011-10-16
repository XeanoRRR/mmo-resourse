#include "StdAfx.h"
#include "organizingparam.h"
//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Create Time:2005.3.15

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COrganizingParam	g_OrganizingParam;

COrganizingParam::COrganizingParam(void)
:m_LoadSucceed(false)
{
}

COrganizingParam::~COrganizingParam(void)
{
}

//�õ�Ψһʵ�����
//�ú�����֧�ֶ��̲߳���
COrganizingParam& COrganizingParam:: getInstance()
{
	return g_OrganizingParam;
}

//!									����
void COrganizingParam::DecodeFromMsgSet(DBReadSet &DbReadSet)
{
	m_LoadSucceed = false;

	//! ����ȫ������
	DbReadSet.GetBufferFromByteArray(&m_tagGlobalSetup, sizeof(tagGlobalSetup));

	//! �����صĵȼ���������
	m_vFacLevelSetup.clear();
	long lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacLevelSetup FacLevelSetup;
		FacLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacLevelSetup.lMaxMemNum	= DbReadSet.GetLongFromByteArray();
		FacLevelSetup.lDisbandMemNum = DbReadSet.GetLongFromByteArray();
		long lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacLevelSetup.push_back(FacLevelSetup);
	}

	//! ���˴�������
	m_vUnionUpgradeCondition.clear();
	long lConditionSize = DbReadSet.GetLongFromByteArray();
	for (long j = 0; j < lConditionSize; ++j)
	{
		tagUpgradeCondition UpgradeCondition;
		DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
		m_vUnionUpgradeCondition.push_back(UpgradeCondition);
	}

	//! ���Ṧ�ܿ��ŵĵȼ�����
	DbReadSet.GetBufferFromByteArray(&m_FunctionWithLevel, sizeof(long) * MAXNUM_FactionPurview);

	//! ��ᾭ�齱���ȼ�������
	m_vFacExpLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacExpLevelSetup FacExpLevelSetup;
		FacExpLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacExpLevelSetup.fBaseExp	= DbReadSet.GetFloatFromByteArray();
		long lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacExpLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacExpLevelSetup.push_back(FacExpLevelSetup);
	}
	//! ���ֿ�ȼ�������
	m_vFacDepotLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacDepotLevelSetup FacDepotLevelSetup;
		FacDepotLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacDepotLevelSetup.lCellNum		= DbReadSet.GetLongFromByteArray();
		long lConditionSize				= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacDepotLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacDepotLevelSetup.push_back(FacDepotLevelSetup);
	}
	//! ����ȼ�����
	m_SmithingLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		long lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SmithingLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ����ȼ�����
	m_MetallurgyLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		long lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_MetallurgyLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ���ҵȼ�����
	m_SewingLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		long lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SewingLevelSetup.push_back(FacSubLevelSetup);
	}


	m_LoadSucceed = true;
}

void COrganizingParam::DecodeFromByteArray(uchar* pData, long &lPos)
{
	m_LoadSucceed = false;

	//! ����ȫ������
	_GetBufferFromByteArray(pData, lPos, &m_tagGlobalSetup, sizeof(tagGlobalSetup));

	//! �����صĵȼ���������
	m_vFacLevelSetup.clear();
	long lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacLevelSetup FacLevelSetup;
		FacLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacLevelSetup.lMaxMemNum	= _GetLongFromByteArray(pData, lPos);
		FacLevelSetup.lDisbandMemNum = _GetLongFromByteArray(pData, lPos);
		long lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacLevelSetup.push_back(FacLevelSetup);
	}

	//! ���˴�������
	m_vUnionUpgradeCondition.clear();
	long lConditionSize = _GetLongFromByteArray(pData, lPos);
	for (long j = 0; j < lConditionSize; ++j)
	{
		tagUpgradeCondition UpgradeCondition;
		_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
		m_vUnionUpgradeCondition.push_back(UpgradeCondition);
	}

	//! ���Ṧ�ܿ��ŵĵȼ�����
	_GetBufferFromByteArray(pData, lPos, &m_FunctionWithLevel, sizeof(long) * MAXNUM_FactionPurview);

	//! ��ᾭ�齱���ȼ�������
	m_vFacExpLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacExpLevelSetup FacExpLevelSetup;
		FacExpLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacExpLevelSetup.fBaseExp	= _GetFloatFromByteArray(pData, lPos);
		long lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacExpLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacExpLevelSetup.push_back(FacExpLevelSetup);
	}
	//! ���ֿ�ȼ�������
	m_vFacDepotLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacDepotLevelSetup FacDepotLevelSetup;
		FacDepotLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacDepotLevelSetup.lCellNum		= _GetLongFromByteArray(pData, lPos);
		long lConditionSize				= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacDepotLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacDepotLevelSetup.push_back(FacDepotLevelSetup);
	}
	//! ����ȼ�����
	m_SmithingLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		long lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SmithingLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ����ȼ�����
	m_MetallurgyLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		long lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_MetallurgyLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ���ҵȼ�����
	m_SewingLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (long i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		long lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (long j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SewingLevelSetup.push_back(FacSubLevelSetup);
	}


	m_LoadSucceed = true;
}

// ��ӵ�CByteArray
bool COrganizingParam::AddToByteArray(vector<uchar> &vByteArray)
{
	if (m_LoadSucceed)
	{
		//! ����ȫ������
		_AddToByteArray(&vByteArray, &m_tagGlobalSetup, sizeof(tagGlobalSetup));

		//! �����صĵȼ���������
		_AddToByteArray(&vByteArray, (long)m_vFacLevelSetup.size());
		for (size_t i = 0; i < m_vFacLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lMaxMemNum);
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lDisbandMemNum);
			_AddToByteArray(&vByteArray, (long)m_vFacLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_vFacLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! ���˴�������
		_AddToByteArray(&vByteArray, (long)m_vUnionUpgradeCondition.size());
		for (size_t i = 0; i < m_vUnionUpgradeCondition.size(); ++i)
		{
			_AddToByteArray(&vByteArray, &m_vUnionUpgradeCondition[i], sizeof(tagUpgradeCondition));
		}

		//! ���Ṧ�ܿ��ŵĵȼ�����;
		_AddToByteArray(&vByteArray, &m_FunctionWithLevel, sizeof(long) * MAXNUM_FactionPurview);

		//! ��ᾭ�齱���ȼ�������
		_AddToByteArray(&vByteArray, (long)m_vFacExpLevelSetup.size());
		for (size_t i = 0; i < m_vFacExpLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacExpLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacExpLevelSetup[i].fBaseExp);
			_AddToByteArray(&vByteArray, (long)m_vFacExpLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_vFacExpLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacExpLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! ���ֿ�ȼ�������
		_AddToByteArray(&vByteArray, (long)m_vFacDepotLevelSetup.size());
		for (size_t i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacDepotLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacDepotLevelSetup[i].lCellNum);
			_AddToByteArray(&vByteArray, (long)m_vFacDepotLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_vFacDepotLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacDepotLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ����ȼ�����
		_AddToByteArray(&vByteArray, (long)m_SmithingLevelSetup.size());
		for (size_t i = 0; i < m_SmithingLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_SmithingLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (long)m_SmithingLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_SmithingLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_SmithingLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ����ȼ�����
		_AddToByteArray(&vByteArray, (long)m_MetallurgyLevelSetup.size());
		for (size_t i = 0; i < m_MetallurgyLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_MetallurgyLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (long)m_MetallurgyLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_MetallurgyLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_MetallurgyLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ���ҵȼ�����
		_AddToByteArray(&vByteArray, (long)m_SewingLevelSetup.size());
		for (size_t i = 0; i < m_SewingLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_SewingLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (long)m_SewingLevelSetup[i].vCondition.size());
			for (size_t j = 0; j < m_SewingLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_SewingLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
	}
	return m_LoadSucceed;
}

//! ��õȼ�����
const tagFacLevelSetup* COrganizingParam::GetLevelSetup(long lLevel)
{
	if (m_LoadSucceed)
	{
		for (size_t i = 0; i < m_vFacLevelSetup.size(); ++i)
		{
			if (lLevel == m_vFacLevelSetup[i].lLevel)
			{
				return &m_vFacLevelSetup[i];
			}
		}
	}

	return NULL;
}

//! �õ�ʹ��һ��Ȩ�޵Ĺ���ȼ�
long COrganizingParam::GetFacPurviewNeedLevel(eFactionPurviewIndex PurviewIndex)
{
	if (MAXNUM_FactionPurview > PurviewIndex && 0 <= PurviewIndex)
	{
		return m_FunctionWithLevel[PurviewIndex];
	}
	return  0x7FFFFFFF;
}

//! �õ��������˵�����
const VecCondition& COrganizingParam::GetUnionCreateCondition(void)
{
	assert(m_LoadSucceed);
	return m_vUnionUpgradeCondition;
}


//! �õ�һ�� �лṦ�ܵ���ߵȼ�
long COrganizingParam::GetFacFunMaxLevel(ulong dwUpgradeType)
{
	if (m_LoadSucceed)
	{
		switch(dwUpgradeType)
		{
		case eUT_FactionLevel:		//! ����ȼ�
			{
				return (long)m_vFacLevelSetup.size();
			}
			break;
		case eUT_ExpLevel:			//! ���鸣���ȼ�
			{
				return (long)m_vFacExpLevelSetup.size();
			}
			break;
		case eUT_SpecialityLevel:		//! רҵ�ȼ�
			{
				return (long)m_vFacDepotLevelSetup.size();
			}
			break;
		case eUT_BattleLevel:		//! ս���ȼ�
			{
				return (long)m_SmithingLevelSetup.size();
			}
			break;
		case eUT_BussinessLevel:	//! ��ҵ�ȼ�
			{
				return (long)m_MetallurgyLevelSetup.size();
			}
			break;
		case eUT_NobleLevel:		//! ����ȼ�
			{
				return (long)m_SewingLevelSetup.size();
			}
			break;
		}
	}

	return NULL;
}


//! �õ�һ���������͵�����
const VecCondition* COrganizingParam::GetUpgradeVecCondition(ulong dwUpgradeType, long lAimLevel)
{
	if (m_LoadSucceed)
	{
		switch(dwUpgradeType)
		{
		case eUT_FactionLevel:		//! ����ȼ�
			{
				for (size_t i = 0; i < m_vFacLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacLevelSetup[i].lLevel)
					{
						return &(m_vFacLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_ExpLevel:			//! ���鸣���ȼ�
			{
				for (size_t i = 0; i < m_vFacExpLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacExpLevelSetup[i].lLevel)
					{
						return &(m_vFacExpLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_SpecialityLevel:		//! רҵ�ȼ�
			{
				for (size_t i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacDepotLevelSetup[i].lLevel)
					{
						return &(m_vFacDepotLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_BattleLevel:		//! ս���ȼ�
			{
				for (size_t i = 0; i < m_SmithingLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_SmithingLevelSetup[i].lLevel)
					{
						return &(m_SmithingLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_BussinessLevel:	//! ��ҵ�ȼ�
			{
				for (size_t i = 0; i < m_MetallurgyLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_MetallurgyLevelSetup[i].lLevel)
					{
						return &(m_MetallurgyLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_NobleLevel:		//! ����ȼ�
			{
				for (size_t i = 0; i < m_SewingLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_SewingLevelSetup[i].lLevel)
					{
						return &(m_SewingLevelSetup[i].vCondition);
					}
				}
			}
			break;
		}
	}

	return NULL;
}


//! �õ�һ���ȼ��ĸ�����������
const tagFacExpLevelSetup* COrganizingParam::GetFacExpLevelSetup(long lExpLevel)
{
	for (size_t i = 0; i < m_vFacExpLevelSetup.size(); ++i)
	{
		if (lExpLevel == m_vFacExpLevelSetup[i].lLevel)
		{
			return &(m_vFacExpLevelSetup[i]);
		}
	}
	return NULL;
}


//! ���洫λ��ʱ��
long COrganizingParam::GetDemiseWarningTime()
{
	return 3;
}

//! ���洫λ������
long COrganizingParam::GetDemiseTerm()
{
	return 7;
}


//! ����̫�ٽ�ɢ������
long COrganizingParam::GetDisbandTerm()
{
	return 3;
}


//! �������ٴ����
float COrganizingParam::GetMemberLeastRate()
{
	return 0.25f;
}

