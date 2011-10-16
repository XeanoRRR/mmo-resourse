#pragma once
#include <fstream>
#include <string>

using namespace std;

class CRegionList
{
public:
	CRegionList(void);
public:
	~CRegionList(void);

// ���ݶ�д
public:
	const inline ULONG GetID()const							{return m_uResourceID;}
	const inline string& GetName()const						{return m_strName;}

public:
	// �������б��ļ� "regionlist.ini"
	friend ifstream &operator>>(ifstream &rstream, CRegionList &regionlist);

protected:
	ULONG		m_uID;
	unsigned	m_uResourceID;
	unsigned	m_uExpScale;
	unsigned	m_uType;
	unsigned	m_uNoPK;
	unsigned	m_uContribution;
	string		m_strName;
	unsigned	m_uGS_ID;
	unsigned	m_uCountry;
	unsigned	m_uNotify;
};

// �������б��ļ� "regionlist.ini"
ifstream &operator>>(ifstream &rstream, CRegionList &regionlist);
