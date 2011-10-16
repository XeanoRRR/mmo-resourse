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

public:
	static inline	CCountryParam* GetInstance()
	{
		if(instance == NULL)
		{
			instance = new CCountryParam;
		}
		return instance;
	}
	static void ReleaseInstance()
	{
		if(instance)
			SAFE_DELETE(instance);
	};

	long	m_lMaxCountyrs;													// ���������
	long	m_lMaxCountryPower;												// �������ֵ

	map<BYTE,long>	m_mpStartRegions;// ��������
	map<BYTE,RECT>	m_mpStartRect;// ��������
	map<BYTE,long>	m_mpStartDir;// ��������
	map<BYTE,long>  m_mpStartCamp;//��Ӫ���

	// �������
	map<BYTE,long>	m_mpMainRegions;// ���ǳ���
	map<BYTE,RECT>	m_mpMainRect;// ��������
	map<BYTE,long>	m_mpMainDir;// ���Ƿ���

	map<BYTE,long>	m_mpJailVillageRegions;// ���Ŵ峡��
	map<BYTE,RECT>	m_mpJailVillageRect;// ���Ŵ�����
	map<BYTE,long>	m_mpJailVillageDir;// ���Ŵ巽��

	map<BYTE,long>	m_mpJailIslandRegions;// ���ŵ�����
	map<BYTE,RECT>	m_mpJailIslandRect;// ���ŵ�����
	map<BYTE,long>	m_mpJailIslandDir;// ���ŵ�����

	bool	Initialize();														//��ʼ��
	void	Release();															//�ͷ���Դ
	bool	Load();

	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};

//�õ���֯��������
inline CCountryParam* GetCountryParam()
{
	return CCountryParam::GetInstance();
}


#endif