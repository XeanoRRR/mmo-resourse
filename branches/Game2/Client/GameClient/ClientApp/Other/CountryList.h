#pragma once
#ifndef _CountryList_h_
#define _CountryList_h_
// ״̬����
class CCountryList
{
public:
	CCountryList(void);
	virtual ~CCountryList(void);
public:

	struct tagCountryList
	{
		BYTE		nID;				// ���
		long		lIconID;			// ͼ����
		string		strName;			// ����
		string		strDesc;			// ����
		string		strAllianceDesc;	// ��������
		D3DCOLOR	color;				// ��ɫ
		D3DCOLOR	colorBack;			// ����ɫ
		BYTE		nLeague;			// ��Ӫ
	};
private:
	static map<BYTE, tagCountryList>		m_mpCountryList;

	static D3DCOLOR	m_dwMyCountryColor;			// �Լ�����������ɫ
	static D3DCOLOR	m_dwOtherCountryColor;		// ��������������ɫ
	static D3DCOLOR	m_dwNoCountryColor;			// û�й��ҵ�������ɫ
public:
	static D3DCOLOR	GetMyCountryColor()			{return m_dwMyCountryColor;}
	static D3DCOLOR	GetOtherCountryColor()		{return m_dwOtherCountryColor;}
	static D3DCOLOR	GetNoCountryColor()			{return m_dwNoCountryColor;}
	static BOOL Load(const char* filename);
	typedef map<BYTE,tagCountryList>MapCountryList;
	static MapCountryList* GetCountryList(){return &m_mpCountryList;}
	static tagCountryList* GetCountry(BYTE nID);
	static const char* GetCountryName(BYTE nID);
};

#endif