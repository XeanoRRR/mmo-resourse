#pragma once

#include "../../../../SCMPublic/def/PurviewDef.h"

class PurviewSetup
{
public:
	PurviewSetup(VOID);
	~PurviewSetup(VOID);

public:
	//!
	static		PurviewSetup&			GetInstance(VOID);

public:
	//!									��ȡ����
	BOOL								LoadSetup(const char *pXmlFilename);
	//!									ͨ���û����͵õ�Ȩ��
	const		tagGMTPurviewSetup*		GetPurviewByUserType(DWORD dwUserType);



private:
	//!									�����Ƿ�׼����
	BOOL								m_bIsReady;
	//!									һ���û�������ӵ�е�Ȩ�޵��б�
	map<DWORD, tagGMTPurviewSetup>		m_mapGMTPurviewSetup;

private:
	//!
	static		PurviewSetup			c_PurviewSetup;
};