#pragma once

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
	bool										AddToByteArray(vector<uchar> &vByteArray);
	//!											����
	void										DecodeFromByteArray(uchar* pData, long &lPos);

	//!											��õȼ�����
	const		tagFacLevelSetup*				GetLevelSetup(long lLevel);
	//!											�õ�ʹ��һ��Ȩ�޵Ĺ���ȼ�
	long										GetFacPurviewNeedLevel(eFactionPurviewIndex PurviewIndex);

	//!											�õ��������˵�����
	const		VecCondition&					GetUnionCreateCondition(void);

	//!											�õ�һ���������͵�����
	const		VecCondition*					GetUpgradeVecCondition(ulong dwUpgradeType, long lAimLevel);

	//!											�õ�һ���ȼ��ĸ�����������
	const		tagFacExpLevelSetup*			GetFacExpLevelSetup(long lExpLevel);

	//!											�õ�һ�� �лṦ�ܵ���ߵȼ�
	long										GetFacFunMaxLevel(ulong dwUpgradeType);

	//!											���洫λ��ʱ��
	long										GetDemiseWarningTime();

	//!											���洫λ������
	long										GetDemiseTerm();

	//!											����̫�ٽ�ɢ������
	long										GetDisbandTerm();

	//!											�������ٴ���ʣ��ٷֱ�
	float										GetMemberLeastRate();


private:
	//!											���û�ȡ�ɹ����
	bool										m_LoadSucceed;

	//!											����ȫ������
	tagGlobalSetup								m_tagGlobalSetup;
	//!											�����صĵȼ���������
	vector<tagFacLevelSetup>					m_vFacLevelSetup;
	//!											���Ṧ�ܿ��ŵĵȼ�����
	long										m_FunctionWithLevel[MAXNUM_FactionPurview];

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