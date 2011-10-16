#include "StdAfx.h"
#include "ContributeSetup.h"
#include "../Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ��̬��Ա��ʼ��
ContributeSetup::tagContributePara ContributeSetup::m_ContributePara;
vector<ContributeSetup::tagContributeItem> ContributeSetup::m_vContributeItems;

/// ����
ContributeSetup::ContributeSetup(void)
{
}

/// ����
ContributeSetup::~ContributeSetup(void)
{
	Release();
}

/// ���ع��ҹ�������
bool ContributeSetup::LoadContributeSetup(const char* filename)
{
	Release();

	CRFile* prfile = rfOpen(filename);
	if(prfile == NULL)
	{
        Log4c::Error(ROOT_MODULE,"%-15s file '%s' can't found!", __FUNCTION__,filename);
		return false;
	}
	
	stringstream stream;
	string strTemp;
	prfile->ReadToStream(stream);
	rfClose(prfile);

	// ��ȡ���ҹ��ײ����б�
	stream	
		>> strTemp >> m_ContributePara.lCombatLevel1
		>> strTemp >> m_ContributePara.lCombatLevel2
		>> strTemp >> m_ContributePara.lContribute1
		>> strTemp >> m_ContributePara.lContribute2

		>> strTemp >> m_ContributePara.lContributeLevel
		>> strTemp >> m_ContributePara.lContributeMultiple

		>> strTemp >> m_ContributePara.lContributeBaseValue
		>> strTemp >> m_ContributePara.lContributeMax

		>> strTemp >> m_ContributePara.lCountryCityContributeModifierLose
		>> strTemp >> m_ContributePara.lCountryCityContributeModifierGain
		>> strTemp >> m_ContributePara.lCountryCityContributeItemModifier;


	// ��ȡ���ҹ�����Ʒ�б�
	while(ReadTo(stream, "#"))
	{
		tagContributeItem stItem;

		stream >> stItem.dwLoValue;
		stream >> stItem.dwHiValue;
		stream >> stItem.strName;
		stream >> stItem.dwNum;

		m_vContributeItems.push_back(stItem);
	}
	return true;
}

/// ����
bool ContributeSetup::AddToByteArray(vector<uchar>* pByteArray)
{
	_AddToByteArray(pByteArray, m_ContributePara.lCombatLevel1);
	_AddToByteArray(pByteArray, m_ContributePara.lCombatLevel2);

	_AddToByteArray(pByteArray, m_ContributePara.lContribute1);
	_AddToByteArray(pByteArray, m_ContributePara.lContribute2);

	_AddToByteArray(pByteArray, m_ContributePara.lContributeLevel);
	_AddToByteArray(pByteArray, m_ContributePara.lContributeMultiple);

	_AddToByteArray(pByteArray, m_ContributePara.lContributeBaseValue);
	_AddToByteArray(pByteArray, m_ContributePara.lContributeMax);

	_AddToByteArray(pByteArray, m_ContributePara.lCountryCityContributeModifierLose);
	_AddToByteArray(pByteArray, m_ContributePara.lCountryCityContributeModifierGain);
	_AddToByteArray(pByteArray, m_ContributePara.lCountryCityContributeItemModifier);

	long lNum = (long)m_vContributeItems.size();
	_AddToByteArray(pByteArray, lNum);

	vector<tagContributeItem>::iterator it=m_vContributeItems.begin();
	for(; it!=m_vContributeItems.end(); ++it)
	{
		tagContributeItem *pItem = &(*it);
		if( pItem )
		{
			_AddToByteArray(pByteArray, pItem->dwLoValue);
			_AddToByteArray(pByteArray, pItem->dwHiValue);
			_AddToByteArray(pByteArray, pItem->dwNum);
			_AddToByteArray(pByteArray, (char*)pItem->strName.data());
		}
	}
	return true;
}

/// ����
bool ContributeSetup::DecordFromByteArray(uchar* pByte, long& pos)
{
	char szTemp[256];
	int i;
	long lNum;
	tagContributeItem stItem;
	
	Release();

	m_ContributePara.lCombatLevel1 = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lCombatLevel2 = _GetLongFromByteArray(pByte, pos);

	m_ContributePara.lContribute1 = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lContribute2 = _GetLongFromByteArray(pByte, pos);

	m_ContributePara.lContributeLevel = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lContributeMultiple = _GetLongFromByteArray(pByte, pos);

	m_ContributePara.lContributeBaseValue = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lContributeMax = _GetLongFromByteArray(pByte, pos);

	m_ContributePara.lCountryCityContributeModifierLose = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lCountryCityContributeModifierGain = _GetLongFromByteArray(pByte, pos);
	m_ContributePara.lCountryCityContributeItemModifier = _GetLongFromByteArray(pByte, pos);
	

	lNum = _GetLongFromByteArray(pByte, pos);
	for(i=0; i<lNum; i++)
	{
		stItem.dwLoValue = _GetLongFromByteArray(pByte, pos);
		stItem.dwHiValue = _GetLongFromByteArray(pByte, pos);
		stItem.dwNum = _GetLongFromByteArray(pByte, pos);
		stItem.strName = _GetStringFromByteArray(pByte, pos, szTemp);

		m_vContributeItems.push_back(stItem);
	}
	return true;
}

/// ����
void ContributeSetup::Release()
{
	memset(&m_ContributePara, 0, sizeof(tagContributePara));
	m_vContributeItems.clear();
}


/// ȡ�ù��ҹ��ײ����б�
ContributeSetup::tagContributePara* ContributeSetup::GetContributeParaList()
{
	return &m_ContributePara;
}

/// ȡ�ù�����Ʒ�б�
vector<ContributeSetup::tagContributeItem>* ContributeSetup::GetContributeItemList()
{
	return &m_vContributeItems;
}