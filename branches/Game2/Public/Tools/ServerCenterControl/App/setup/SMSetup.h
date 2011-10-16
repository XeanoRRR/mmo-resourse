#pragma once

#include "../../../../SCMPublic/def/InfoDef.h"

typedef tagSmInfo tagSmBaseSetup;

struct  tagSMSetup : public tagSmBaseSetup
{
	char	szIP[MAX_HOST_ADDRESS_SZIE];
	LONG	lIP;
	char	szMD5LoginPwd[MD5_16_PASSWORD_SIZE];

	tagSMSetup(VOID){memset(this, 0, sizeof(tagSMSetup));}
};

struct tagTempConnSetup 
{
	string	strIP;
	DWORD	dwID;

	tagTempConnSetup(const char *pIP, DWORD dwInID)
		:strIP(pIP),dwID(dwInID)
	{}
};

////////////////////////////////////
class SMSetup
{
public:
	SMSetup(VOID);
	~SMSetup(VOID);

public:
	static			SMSetup&			GetInstance(VOID);

public:
	//!									��ȡ����
	BOOL								LoadSetup(const char *pXmlFilename);
	//!									�õ�һ��SM������
	const			tagSMSetup*			GetSMSetup(DWORD dwSMID) const;
	//!									�õ�һ��SM������
	const			tagSMSetup*			GetSMSetupByIP(LONG lIP) const;
	//!									�õ����õ�SM��
	long								GetSmNum(void) const {return (long)m_mapSMSetup.size();}
	//!
	VOID								GetSMNetSetupList(vector<tagTempConnSetup> &vSMSetup);
	//!
	const			tagScc2SmSetup&		GetScc2SmSetup(void) {return m_Scc2SmSetup;}


private:
	//!
	tagScc2SmSetup						m_Scc2SmSetup;
	//!
	map<LONG, tagSMSetup>				m_mapSMSetup;

private:
	static	SMSetup	c_SMSetup;
};