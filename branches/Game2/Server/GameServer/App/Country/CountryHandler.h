// ���ҹ�����
// ��Ҫ�ǹ�����Ҷ���
// ��Singleton��
// bugs
// 06/6/27

#pragma once
#ifndef _CountryHandler_h_
#define _CountryHandler_h_

class CCountry;

class CCountryHandler
{
public:
	CCountryHandler(void);
	virtual ~CCountryHandler(void);

	map<BYTE,CCountry*>					m_pCountrys;

private:
	static CCountryHandler				*instance;

public:
	static CCountryHandler				*GetInstance(void);

	CCountry* GetCountry(BYTE id)
	{
		if(id <= 0) return NULL;
		if(m_pCountrys.find(id) != m_pCountrys.end())
		{
			return m_pCountrys[id];
		}
		return NULL;
	}

	bool			Initialize(void);
	void			Release(void);
	bool			Append( CCountry* c );
	
	bool AddToByteArray(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	bool DecordFromByteArray(BYTE* pByte, long& pos);	// ����
};


inline CCountryHandler* GetCountryHandler(void)
{
	return CCountryHandler::GetInstance();
}


#endif