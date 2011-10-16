#include "StdAfx.h"
#include "organizingparam.h"
#include "../public/tinystr.h"
#include "../public/tinyxml.h"
#include "../goods/CGoodsFactory.h"

//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Create Time:2005.3.15

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern HWND g_hWnd;

COrganizingParam	g_OrganizingParam;

COrganizingParam::COrganizingParam(void)
:m_LoadSucceed(FALSE)
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
void COrganizingParam::DecodeByWS(BYTE* pData, LONG &lPos)
{
	m_LoadSucceed = FALSE;

	//! ����ȫ������
	_GetBufferFromByteArray(pData, lPos, &m_tagGlobeSetup, sizeof(tagGlobeSetup));

	//! �����صĵȼ���������
	m_vFacLevelSetup.clear();
	LONG lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacLevelSetup FacLevelSetup;
		FacLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacLevelSetup.lMaxMemNum	= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacLevelSetup.push_back(FacLevelSetup);
	}

	//! ���˴�������
	m_vUnionUpgradeCondition.clear();
	LONG lConditionSize = _GetLongFromByteArray(pData, lPos);
	for (LONG j = 0; j < lConditionSize; ++j)
	{
		tagUpgradeCondition UpgradeCondition;
		_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
		m_vUnionUpgradeCondition.push_back(UpgradeCondition);
	}

	//! ���Ṧ�ܿ��ŵĵȼ�����
	_GetBufferFromByteArray(pData, lPos, &m_FunctionWithLevel, sizeof(LONG) * MAXNUM_FactionPurview);

	m_mapFacSetWithLevel.clear();
	LONG lSetWithLevelSize = _GetLongFromByteArray(pData, lPos);
	for (LONG j = 0; j < lSetWithLevelSize; ++j)
	{
		tagFacSetWithLevel tmpFacSetWithLevel;
		_GetBufferFromByteArray(pData, lPos, &tmpFacSetWithLevel, sizeof(tagFacSetWithLevel));
		m_mapFacSetWithLevel[tmpFacSetWithLevel.lLevel] = tmpFacSetWithLevel;
	}

	//! ��ᾭ�齱���ȼ�������
	m_vFacExpLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacExpLevelSetup FacExpLevelSetup;
		FacExpLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacExpLevelSetup.lBaseExp	= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
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
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacDepotLevelSetup FacDepotLevelSetup;
		FacDepotLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacDepotLevelSetup.lCellNum		= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize				= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
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
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
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
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
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
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		LONG lConditionSize			= _GetLongFromByteArray(pData, lPos);
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			_GetBufferFromByteArray(pData, lPos, &UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SewingLevelSetup.push_back(FacSubLevelSetup);
	}


	m_LoadSucceed = TRUE;
}

// ��ӵ�CByteArray
BOOL COrganizingParam::AddToByteArray(vector<BYTE> &vByteArray)
{
	//! ����ȫ������
	_AddToByteArray(&vByteArray, &m_tagGlobeSetup, sizeof(tagGlobeSetup));

	//! �����صĵȼ���������
	_AddToByteArray(&vByteArray, (LONG)m_vFacLevelSetup.size());
	for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
	{
		_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lLevel);
		_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lMaxMemNum);
		_AddToByteArray(&vByteArray, (LONG)m_vFacLevelSetup[i].vCondition.size());
		for (LONG j = 0; j < m_vFacLevelSetup[i].vCondition.size(); ++j)
		{
			_AddToByteArray(&vByteArray, &m_vFacLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
		}
	}
	//! ���Ṧ�ܿ��ŵĵȼ�����
	_AddToByteArray(&vByteArray, &m_FunctionWithLevel, sizeof(LONG) * MAXNUM_FactionPurview);
	//! ���Ḩ����������
	_AddToByteArray(&vByteArray, (LONG)m_mapFacSetWithLevel.size());
	map<LONG, tagFacSetWithLevel>::iterator ite = m_mapFacSetWithLevel.begin();
	for (; ite != m_mapFacSetWithLevel.end(); ++ite)
	{
		_AddToByteArray(&vByteArray, &(ite->second), sizeof(tagFacSetWithLevel));
	}
	return TRUE;
}

//! ��õȼ�����
const tagFacLevelSetup* COrganizingParam::GetLevelSetup(LONG lLevel)
{
	if (m_LoadSucceed)
	{
		for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
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
LONG COrganizingParam::GetFacPurviewNeedLevel(eFactionPurviewIndex PurviewIndex)
{
	if (MAXNUM_FactionPurview > PurviewIndex && 0 <= PurviewIndex)
	{
		return m_FunctionWithLevel[PurviewIndex];
	}
	return  0x7FFFFFFF;
}

//!											�õ��������˵�����
const VecCondition& COrganizingParam::GetUnionCreateCondition(VOID)
{
	assert(m_LoadSucceed);
	return m_vUnionUpgradeCondition;
}


//!											�õ�һ���������͵�����
const VecCondition* COrganizingParam::GetUpgradeVecCondition(DWORD dwUpgradeType, LONG lAimLevel)
{
	if (m_LoadSucceed)
	{
		switch(dwUpgradeType)
		{
		case eUT_FactionLevel:		//! ����ȼ�
			{
				for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
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
				for (LONG i = 0; i < m_vFacExpLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacExpLevelSetup[i].lLevel)
					{
						return &(m_vFacExpLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_DepotLevel:		//! �ֿ�ȼ�
			{
				for (LONG i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_vFacDepotLevelSetup[i].lLevel)
					{
						return &(m_vFacDepotLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_SmithingLevel:		//! ����ȼ�
			{
				for (LONG i = 0; i < m_SmithingLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_SmithingLevelSetup[i].lLevel)
					{
						return &(m_SmithingLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_MetallurgyLevel:	//! ����ȼ�
			{
				for (LONG i = 0; i < m_MetallurgyLevelSetup.size(); ++i)
				{
					if (lAimLevel == m_MetallurgyLevelSetup[i].lLevel)
					{
						return &(m_MetallurgyLevelSetup[i].vCondition);
					}
				}
			}
			break;
		case eUT_SewingLevel:		//! ���ҵȼ�
			{
				for (LONG i = 0; i < m_SewingLevelSetup.size(); ++i)
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