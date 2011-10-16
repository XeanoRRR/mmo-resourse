#pragma once

#ifndef _CountryParam_h_
#define _CountryParam_h_

class CCountryParam
{
private:
	CCountryParam(void);
	~CCountryParam(void);

private:
	static CCountryParam* instance;											// ����Ψһʵ�����

	static long	m_lMaxCountyrs;													// ���������
	static long	m_lMaxCountryPower;												// �������ֵ
	

	// �������
	static map<BYTE,long>	m_mpMainRegions;// ���ǳ���
	static map<BYTE,RECT>	m_mpMainRect;// ��������
	static map<BYTE,long>	m_mpMainDir;// ���Ƿ���
	static map<BYTE,long>  m_mpStartCamp;//��Ӫ���

	static map<BYTE,long>	m_mpJailVillageRegions;// ���Ŵ峡��
	static map<BYTE,RECT>	m_mpJailVillageRect;// ���Ŵ�����
	static map<BYTE,long>	m_mpJailVillageDir;// ���Ŵ巽��

	static map<BYTE,long>	m_mpJailIslandRegions;// ���ŵ�����
	static map<BYTE,RECT>	m_mpJailIslandRect;// ���ŵ�����
	static map<BYTE,long>	m_mpJailIslandDir;// ���ŵ�����

public:
	static bool	Initialize();														//��ʼ��
	static void	Release();															//�ͷ���Դ

	static void	GetCountryList(long lCampID,list<BYTE> &lCountryID);				//����ͬ��ID��ȡ�����б�

	static bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	static bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����

	long GetMaxCountrys(void) { return m_lMaxCountyrs; }
	long GetMaxCountryPower(void) { return m_lMaxCountryPower; }
	// ����������Խӿ�
	static long GetCountryMainRegionID	(BYTE byCountryID);
	static long GetCountryMainRectLeft	(BYTE byCountryID);
	static long GetCountryMainRectRight	(BYTE byCountryID);
	static long GetCountryMainRectBottom(BYTE byCountryID);
	static long GetCountryMainRectTop	(BYTE byCountryID);
	static long GetCountryMainDir		(BYTE byCountryID);
	static long GetCountryStartCamp		(BYTE byCountryID);

	// ���Ŵ�������Խӿ�
	static long GetCountryJailVillageRegionID	(BYTE byCountryID);
	static long GetCountryJailVillageRectLeft	(BYTE byCountryID);
	static long GetCountryJailVillageRectRight	(BYTE byCountryID);
	static long GetCountryJailVillageRectBottom	(BYTE byCountryID);
	static long GetCountryJailVillageRectTop	(BYTE byCountryID);
	static long GetCountryJailVillageDir		(BYTE byCountryID);

	// ���ŵ�������Խӿ�
	static long GetCountryJailIsLandRegionID	(BYTE byCountryID);
	static long GetCountryJailIsLandRectLeft	(BYTE byCountryID);
	static long GetCountryJailIsLandRectRight	(BYTE byCountryID);
	static long GetCountryJailIsLandRectBottom	(BYTE byCountryID);
	static long GetCountryJailIsLandRectTop		(BYTE byCountryID);
	static long GetCountryJailIsLandDir			(BYTE byCountryID);

	static inline	CCountryParam* GetInstance()
	{
		if(instance == NULL)
		{
			instance = new CCountryParam;
		}
		return instance;
	}
};

//�õ���֯��������
inline CCountryParam* GetCountryParam()
{
	return CCountryParam::GetInstance();
}


#endif