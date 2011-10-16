#pragma once
#include "../public/OrganizingDef.h"

//Function:	��ἰ�����֯�Ĳ�����
//Author:	Joe
//Editer:	Fred
//Create Time:2005.3.15
//Edit Time:2008.04.08





class WSOrganizingParam
{
public:
	WSOrganizingParam(void);
	~WSOrganizingParam(void);

public:
	//!									�õ�Ψһʵ�����
	static		WSOrganizingParam&		getInstance(void);
	//!									��ʼ��
	BOOL								Initialize(void);
	//!									�ͷŶ���
	void								Release(void);
	//!									��ӵ���Ϣ����
	BOOL								CodeToMsgSet(DBWriteSet &DbWriteSet);

	//!									��õȼ�����
	const		tagFacLevelSetup*		GetLevelSetup(LONG lLevel);
	//!									�õ�һ������ȼ����ŵ����л�ԱȨ��
	LONG								GetCurrLvlAllPurview(LONG lCurrLvl);
	//!									�õ�һ���������͵�����
	const		VecCondition*			GetUpgradeVecCondition(DWORD dwUpgradeType, LONG lAimLevel);
	//!									���ÿ������ȼ��εĿƼ��ȼ���߼������ڽ��ͼ���ȼ�ʱ���ͿƼ��ȼ�
	void								AddLevel(DWORD dwFacLv,eUpgradeType eType,DWORD dwLv);
	//!									��ȡ������Դ��Ϣ
	const		tagBaseConsume*			GetConsume(DWORD dwLv);
	//!									��ȡ��ҵ�ȼ�����
	float								GetBussBase(DWORD dwLv); 
	//!									ȫ������
	const		tagGlobeSetup&			GetGlobe(){return m_tagGlobeSetup;}
	//!									����ȼ���Ӧ�ĿƼ��ȼ�
	const		tagTechInfo*			GetTechInfo(LONG lv){return m_TechInfo.find(lv)!=m_TechInfo.end()?&m_TechInfo[lv]:NULL;}
private:						
	//!									�ض�����
	BOOL								ReLoad(void);


private:
	//!									���ö�ȡ�ɹ����
	BOOL								m_LoadSucceed;

	//!									����ȫ������
	tagGlobeSetup						m_tagGlobeSetup;
	//!									�����صĵȼ���������
	vector<tagFacLevelSetup>			m_vFacLevelSetup;
	//!									���Ṧ�ܿ��ŵĵȼ�����
	LONG								m_FunctionWithLevel[MAXNUM_FactionPurview];

	//!									���˴�������
	VecCondition						m_vUnionUpgradeCondition;

	//!									��ᾭ�齱���ȼ�������
	vector<tagFacExpLevelSetup>			m_vFacExpLevelSetup;
	//!									רҵ�ȼ�������
	vector<tagFacDepotLevelSetup>		m_vFacDepotLevelSetup;

	//!									ս���ȼ�����
	vector<tagFacSubLevelSetup>			m_SpecialLevelSetup;
	//!									��ҵ�ȼ�����
	vector<tagFacSubLevelSetup>			m_BussinessLevelSetup;
	//!									����ȼ�����
	vector<tagFacSubLevelSetup>			m_NobleLevelSetup;		
	//!									����ȼ���Ӧ�Ƽ��ȼ���߼�����
	map<LONG,tagTechInfo>				m_TechInfo;
	//!									�����Ӧ���Ļ������ٽ��
	map<LONG,tagBaseConsume>			m_BaseConsume;
	//!									��ҵ�ȼ���Ӧ����Դ���Ļ���
	map<LONG,LONG>						m_BussinessBase;
	
};

