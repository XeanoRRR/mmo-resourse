#include "StdAfx.h"
#include "organizingparam.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "public.h"

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
void COrganizingParam::DecodeFromMsgSet(DBReadSet &DbReadSet)
{
	m_LoadSucceed = FALSE;

	//! ����ȫ������
	DbReadSet.GetBufferFromByteArray(&m_tagGlobeSetup, sizeof(tagGlobeSetup));

	//! �����صĵȼ���������
	m_vFacLevelSetup.clear();
	LONG lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacLevelSetup FacLevelSetup;
		FacLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacLevelSetup.lMaxMemNum	= DbReadSet.GetLongFromByteArray();
		FacLevelSetup.lDisbandMemNum = DbReadSet.GetLongFromByteArray();
		LONG lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacLevelSetup.push_back(FacLevelSetup);
	}

	//! ���˴�������
	m_vUnionUpgradeCondition.clear();
	LONG lConditionSize = DbReadSet.GetLongFromByteArray();
	for (LONG j = 0; j < lConditionSize; ++j)
	{
		tagUpgradeCondition UpgradeCondition;
		DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
		m_vUnionUpgradeCondition.push_back(UpgradeCondition);
	}

	//! ���Ṧ�ܿ��ŵĵȼ�����
	DbReadSet.GetBufferFromByteArray(&m_FunctionWithLevel, sizeof(LONG) * MAXNUM_FactionPurview);

	//! ��ᾭ�齱���ȼ�������
	m_vFacExpLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacExpLevelSetup FacExpLevelSetup;
		FacExpLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacExpLevelSetup.fBaseExp	= DbReadSet.GetFloatFromByteArray();
		LONG lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacExpLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacExpLevelSetup.push_back(FacExpLevelSetup);
	}
	//! ���רҵ�ȼ�������
	m_vFacDepotLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacDepotLevelSetup FacDepotLevelSetup;
		FacDepotLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		FacDepotLevelSetup.lCellNum		= DbReadSet.GetLongFromByteArray();
		LONG lConditionSize				= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacDepotLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_vFacDepotLevelSetup.push_back(FacDepotLevelSetup);
	}
	//! ��ҵ�ȼ�����
	m_MetallurgyLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		LONG lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_MetallurgyLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ս���ȼ�����
	m_SmithingLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		LONG lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SmithingLevelSetup.push_back(FacSubLevelSetup);
	}
	//! ����ȼ�����
	m_SewingLevelSetup.clear();
	lLevelSize = DbReadSet.GetLongFromByteArray();
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacSubLevelSetup FacSubLevelSetup;
		FacSubLevelSetup.lLevel		= DbReadSet.GetLongFromByteArray();
		LONG lConditionSize			= DbReadSet.GetLongFromByteArray();
		for (LONG j = 0; j < lConditionSize; ++j)
		{
			tagUpgradeCondition UpgradeCondition;
			DbReadSet.GetBufferFromByteArray(&UpgradeCondition, sizeof(tagUpgradeCondition));
			FacSubLevelSetup.vCondition.push_back(UpgradeCondition);
		}
		m_SewingLevelSetup.push_back(FacSubLevelSetup);
	}
	//�����ճ�����
	m_BaseConsume.clear();
	lLevelSize=DbReadSet.GetLongFromByteArray();
	for (LONG i=0;i<lLevelSize;i++)
	{
		tagBaseConsume tgBase;
		LONG lLevel=DbReadSet.GetLongFromByteArray();
		DbReadSet.GetBufferFromByteArray(&tgBase,sizeof(tagBaseConsume));
		m_BaseConsume[lLevel]=tgBase;
	}
	//��ҵ�ȼ���Ӧ�Ļ���
	m_BussinessBase.clear();
	lLevelSize=DbReadSet.GetLongFromByteArray();
	for (LONG i=0;i<lLevelSize;i++)
	{
		LONG lLevel=DbReadSet.GetLongFromByteArray();
		LONG lBaseVal=DbReadSet.GetLongFromByteArray();
		m_BussinessBase[lLevel]=lBaseVal;
	}
	m_LoadSucceed = TRUE;
}

void COrganizingParam::DecodeFromByteArray(BYTE* pData, LONG &lPos)
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
		FacLevelSetup.lDisbandMemNum = _GetLongFromByteArray(pData, lPos);
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

	//! ��ᾭ�齱���ȼ�������
	m_vFacExpLevelSetup.clear();
	lLevelSize = _GetLongFromByteArray(pData, lPos);
	for (LONG i = 0; i < lLevelSize; ++i)
	{
		tagFacExpLevelSetup FacExpLevelSetup;
		FacExpLevelSetup.lLevel		= _GetLongFromByteArray(pData, lPos);
		FacExpLevelSetup.fBaseExp	= _GetFloatFromByteArray(pData, lPos);
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
	//�����ճ�����
	m_BaseConsume.clear();
	lLevelSize=_GetLongFromByteArray(pData, lPos);
	for (LONG i=0;i<lLevelSize;i++)
	{
		tagBaseConsume tgBase;
		LONG lLevel=_GetLongFromByteArray(pData,lPos);
		_GetBufferFromByteArray(pData,lPos,&tgBase,sizeof(tagBaseConsume));
		m_BaseConsume[lLevel]=tgBase;
	}
	//��ҵ�ȼ���Ӧ�Ļ���
	m_BussinessBase.clear();
	lLevelSize=_GetLongFromByteArray(pData, lPos);
	for (LONG i=0;i<lLevelSize;i++)
	{
		LONG lLevel=_GetLongFromByteArray(pData,lPos);
		LONG lBaseVal=_GetLongFromByteArray(pData,lPos);
		m_BussinessBase[lLevel]=lBaseVal;
	}

	m_LoadSucceed = TRUE;
}

// ��ӵ�CByteArray
BOOL COrganizingParam::AddToByteArray(vector<BYTE> &vByteArray)
{
	if (m_LoadSucceed)
	{
		//! ����ȫ������
		_AddToByteArray(&vByteArray, &m_tagGlobeSetup, sizeof(tagGlobeSetup));

		//! �����صĵȼ���������
		_AddToByteArray(&vByteArray, (LONG)m_vFacLevelSetup.size());
		for (LONG i = 0; i < m_vFacLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lMaxMemNum);
			_AddToByteArray(&vByteArray, m_vFacLevelSetup[i].lDisbandMemNum);
			_AddToByteArray(&vByteArray, (LONG)m_vFacLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! ���˴�������
		_AddToByteArray(&vByteArray, (LONG)m_vUnionUpgradeCondition.size());
		for (LONG i = 0; i < m_vUnionUpgradeCondition.size(); ++i)
		{
			_AddToByteArray(&vByteArray, &m_vUnionUpgradeCondition[i], sizeof(tagUpgradeCondition));
		}

		//! ���Ṧ�ܿ��ŵĵȼ�����;
		_AddToByteArray(&vByteArray, &m_FunctionWithLevel, sizeof(LONG) * MAXNUM_FactionPurview);

		//! ��ᾭ�齱���ȼ�������
		_AddToByteArray(&vByteArray, (LONG)m_vFacExpLevelSetup.size());
		for (LONG i = 0; i < m_vFacExpLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacExpLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacExpLevelSetup[i].fBaseExp);
			_AddToByteArray(&vByteArray, (LONG)m_vFacExpLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacExpLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacExpLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! ���רҵ�ȼ�������
		_AddToByteArray(&vByteArray, (LONG)m_vFacDepotLevelSetup.size());
		for (LONG i = 0; i < m_vFacDepotLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_vFacDepotLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, m_vFacDepotLevelSetup[i].lCellNum);
			_AddToByteArray(&vByteArray, (LONG)m_vFacDepotLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_vFacDepotLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_vFacDepotLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ս���ȼ�����
		_AddToByteArray(&vByteArray, (LONG)m_SmithingLevelSetup.size());
		for (LONG i = 0; i < m_SmithingLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_SmithingLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (LONG)m_SmithingLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_SmithingLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_SmithingLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ��ҵ�ȼ�����
		_AddToByteArray(&vByteArray, (LONG)m_MetallurgyLevelSetup.size());
		for (LONG i = 0; i < m_MetallurgyLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_MetallurgyLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (LONG)m_MetallurgyLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_MetallurgyLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_MetallurgyLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}

		//! ����ȼ�����
		_AddToByteArray(&vByteArray, (LONG)m_SewingLevelSetup.size());
		for (LONG i = 0; i < m_SewingLevelSetup.size(); ++i)
		{
			_AddToByteArray(&vByteArray, m_SewingLevelSetup[i].lLevel);
			_AddToByteArray(&vByteArray, (LONG)m_SewingLevelSetup[i].vCondition.size());
			for (LONG j = 0; j < m_SewingLevelSetup[i].vCondition.size(); ++j)
			{
				_AddToByteArray(&vByteArray, &m_SewingLevelSetup[i].vCondition[j], sizeof(tagUpgradeCondition));
			}
		}
		//! �����ճ�����
		_AddToByteArray(&vByteArray,(LONG)m_BaseConsume.size());
		map<LONG,tagBaseConsume>::iterator itBase=m_BaseConsume.begin();
		for (;itBase!=m_BaseConsume.end();itBase++)
		{
			_AddToByteArray(&vByteArray,itBase->first);
			_AddToByteArray(&vByteArray,&(itBase->second),sizeof(tagBaseConsume));
		}
		//��ҵ�ȼ���Ӧ�Ļ���
		_AddToByteArray(&vByteArray,(LONG)m_BussinessBase.size());
		map<LONG,LONG>::iterator itBussBase=m_BussinessBase.begin();
		for (;itBussBase!=m_BussinessBase.end();itBussBase++)
		{
			_AddToByteArray(&vByteArray,itBussBase->first);
			_AddToByteArray(&vByteArray,itBussBase->second);
		}
	}
	return m_LoadSucceed;
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

//! �õ��������˵�����
const VecCondition& COrganizingParam::GetUnionCreateCondition(VOID)
{
	assert(m_LoadSucceed);
	return m_vUnionUpgradeCondition;
}


//! �õ�һ�� �лṦ�ܵ���ߵȼ�
LONG COrganizingParam::GetFacFunMaxLevel(DWORD dwUpgradeType)
{
	if (m_LoadSucceed)
	{
		switch(dwUpgradeType)
		{
		case eUT_FactionLevel:		//! ����ȼ�
			{
				return (LONG)m_vFacLevelSetup.size();
			}
			break;
		case eUT_ExpLevel:			//! ���鸣���ȼ�
			{
				return (LONG)m_vFacExpLevelSetup.size();
			}
			break;
		case eUT_SpecialityLevel:		//! רҵ�ȼ�
			{
				return (LONG)m_vFacDepotLevelSetup.size();
			}
			break;
		case eUT_BattleLevel:		//! ս���ȼ�
			{
				return (LONG)m_SmithingLevelSetup.size();
			}
			break;
		case eUT_BussinessLevel:	//! ��ҵ�ȼ�
			{
				return (LONG)m_MetallurgyLevelSetup.size();
			}
			break;
		case eUT_NobleLevel:		//! ����ȼ�
			{
				return (LONG)m_SewingLevelSetup.size();
			}
			break;
		}
	}

	return NULL;
}


//! �õ�һ���������͵�����
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
		case eUT_SpecialityLevel:		//! רҵ�ȼ�
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
		case eUT_BattleLevel:		//! ս���ȼ�
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
		case eUT_BussinessLevel:	//! ��ҵ�ȼ�
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
		case eUT_NobleLevel:		//! ����ȼ�
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


//! �õ�һ���ȼ��ĸ�����������
const tagFacExpLevelSetup* COrganizingParam::GetFacExpLevelSetup(LONG lExpLevel)
{
	for (LONG i = 0; i < m_vFacExpLevelSetup.size(); ++i)
	{
		if (lExpLevel == m_vFacExpLevelSetup[i].lLevel)
		{
			return &(m_vFacExpLevelSetup[i]);
		}
	}
	return NULL;
}


//! ���洫λ��ʱ��
LONG COrganizingParam::GetDemiseWarningTime()
{
	return 3;
}

//! ���洫λ������
LONG COrganizingParam::GetDemiseTerm()
{
	return 7;
}


//! ����̫�ٽ�ɢ������
LONG COrganizingParam::GetDisbandTerm()
{
	return 3;
}


//! �������ٴ����
float COrganizingParam::GetMemberLeastRate()
{
	return 0.25f;
}

const tagBaseConsume* COrganizingParam::GetConsume(DWORD dwLv)
{
	tagBaseConsume* pC=NULL;
	map<LONG,tagBaseConsume>::iterator it=m_BaseConsume.find(dwLv);
	if (it!=m_BaseConsume.end())
	{
		pC=&(it->second);
	}
	return pC;
}

float COrganizingParam::GetBussBase(DWORD dwLv)
{
	LONG lBase=0;
	if(m_BussinessBase.find(dwLv)!=m_BussinessBase.end())
		lBase=m_BussinessBase[dwLv];
	return lBase;
}