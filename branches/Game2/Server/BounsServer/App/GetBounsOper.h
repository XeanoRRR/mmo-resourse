#pragma once
#include "BounsOper.h"

#include "../DB/RsBouns.h"

// ��ȡ����
class CGetBounsOper :
	public CBounsOper
{
private:
	CGUID					m_guidBouns;			// ������Ϣguid
	string					m_strPlayerName;		// �콱���������
	string					m_strPlayerIP;			// �콱������ڵ�IP
	DWORD					m_dwWSID;				// WorldServer ID

public:
	CGetBounsOper(eOperType eOt);
	~CGetBounsOper(void);

	void SetBounsGUID(CGUID& guid)			{ m_guidBouns = guid; }
	void SetPlayerName(const char* strName)	{ m_strPlayerName = strName; }
	void SetPlayerIP(const char* strIP)		{ m_strPlayerIP = strIP; }
	void SetWSID(DWORD dwID)				{ m_dwWSID = dwID; }

	virtual BOOL DoOperator();
};
