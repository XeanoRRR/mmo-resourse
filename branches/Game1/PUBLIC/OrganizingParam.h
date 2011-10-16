#pragma once
#include "OrganizingDef.h"

//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Create Time:2005.3.15



class COrganizingParam
{
public:
	COrganizingParam(void);
	~COrganizingParam(void);

public:
	//!											�õ�Ψһʵ�����
	static		COrganizingParam&				getInstance(void);
	//!											����
	void										DecodeFromMsgSet(DBReadSet &DbReadSet);
	//!											��ӵ�CByteArray
	BOOL										AddToByteArray(vector<BYTE> &vByteArray);
	//!											����
	void										DecodeFromByteArray(BYTE* pData, LONG &lPos);

	//!											��õȼ�����
	const		tagFacLevelSetup*				GetLevelSetup(LONG lLevel);
	//!											�õ�ʹ��һ��Ȩ�޵Ĺ���ȼ�
	LONG										GetFacPurviewNeedLevel(eFactionPurviewIndex PurviewIndex);

	//!											�õ��������˵�����
	const		VecCondition&					GetUnionCreateCondition(VOID);

	//!											�õ�һ���������͵�����
	const		VecCondition*					GetUpgradeVecCondition(DWORD dwUpgradeType, LONG lAimLevel);

	//!											�õ�һ���ȼ��ĸ�����������
	const		tagFacExpLevelSetup*			GetFacExpLevelSetup(LONG lExpLevel);

	//!											�õ�һ�� �лṦ�ܵ���ߵȼ�
	LONG										GetFacFunMaxLevel(DWORD dwUpgradeType);

	//!											���洫λ��ʱ��
	LONG										GetDemiseWarningTime();

	//!											���洫λ������
	LONG										GetDemiseTerm();

	//!											����̫�ٽ�ɢ������
	LONG										GetDisbandTerm();

	//!											�������ٴ���ʣ��ٷֱ�
	float										GetMemberLeastRate();
	//!											��ȡ������Դ��Ϣ
	const		tagBaseConsume*					GetConsume(DWORD dwLv);
	//!											��ȡ��ҵ�ȼ�����
	float										GetBussBase(DWORD dwLv); 


private:
	//!											���û�ȡ�ɹ����
	BOOL										m_LoadSucceed;

	//!											����ȫ������
	tagGlobeSetup								m_tagGlobeSetup;
	//!											�����صĵȼ���������
	vector<tagFacLevelSetup>					m_vFacLevelSetup;
	//!											���Ṧ�ܿ��ŵĵȼ�����
	LONG										m_FunctionWithLevel[MAXNUM_FactionPurview];

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
	//!											�����Ӧ���Ļ������ٽ��
	map<LONG,tagBaseConsume>					m_BaseConsume;
	//!											��ҵ�ȼ���Ӧ����Դ���Ļ���
	map<LONG,LONG>								m_BussinessBase; 
};