#pragma once
#include "..\public\OrganizingDef.h"

//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Create Time:2005.3.15


//! ��������
enum eUpgradeType
{
	eUT_FactionLevel,	//! ����ȼ�
	eUT_ExpLevel,		//! ���鸣���ȼ�
	eUT_DepotLevel,		//! �ֿ�ȼ�

	eUT_SmithingLevel,	//! ����ȼ�
	eUT_MetallurgyLevel,//! ����ȼ�
	eUT_SewingLevel,	//! ���ҵȼ�
};

class COrganizingParam
{
public:
	COrganizingParam(void);
	~COrganizingParam(void);

public:
	//!											�õ�Ψһʵ�����
	static		COrganizingParam&				getInstance(void);
	//!											����
	void										DecodeByWS(BYTE* pData, LONG &lPos);
	//!											��ӵ�CByteArray
	BOOL										AddToByteArray(vector<BYTE> &vByteArray);

	//!											��õȼ�����
	const		tagFacLevelSetup*				GetLevelSetup(LONG lLevel);
	//!											�õ�ʹ��һ��Ȩ�޵Ĺ���ȼ�
	LONG										GetFacPurviewNeedLevel(eFactionPurviewIndex PurviewIndex);

	//!											�õ��������˵�����
	const		VecCondition&					GetUnionCreateCondition(VOID);

	//!											�õ�һ���������͵�����
	const		VecCondition*					GetUpgradeVecCondition(DWORD dwUpgradeType, LONG lAimLevel);



private:
	//!											���û�ȡ�ɹ����
	BOOL										m_LoadSucceed;

	//!											����ȫ������
	tagGlobeSetup								m_tagGlobeSetup;
	//!											�����صĵȼ���������
	vector<tagFacLevelSetup>					m_vFacLevelSetup;
	//!											���Ṧ�ܿ��ŵĵȼ�����
	LONG										m_FunctionWithLevel[MAXNUM_FactionPurview];

	//!											���Ḩ����������
	map<LONG, tagFacSetWithLevel>				m_mapFacSetWithLevel;

	//!											���˴�������
	VecCondition								m_vUnionUpgradeCondition;

	//!											��ᾭ�齱���ȼ�������
	vector<tagFacExpLevelSetup>					m_vFacExpLevelSetup;
	//!											���ֿ�ȼ�������
	vector<tagFacDepotLevelSetup>				m_vFacDepotLevelSetup;

	//!											����ȼ�����
	vector<tagFacSubLevelSetup>					m_SmithingLevelSetup;
	//!											����ȼ�����
	vector<tagFacSubLevelSetup>					m_MetallurgyLevelSetup;
	//!											���ҵȼ�����
	vector<tagFacSubLevelSetup>					m_SewingLevelSetup;	
};