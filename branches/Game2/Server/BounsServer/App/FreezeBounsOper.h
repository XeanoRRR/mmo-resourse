#pragma once
#include "BounsOper.h"

// ���ά��
class CFreezeBounsOper :
	public CBounsOper
{
private:
	string					m_strPlayerIP;			// ���IP
	tagBounsInfo			*m_pBInfo;				// ������Ϣ

public:
	CFreezeBounsOper(eOperType eOt);
	~CFreezeBounsOper(void);

	void SetPlayerIP(const char* strIP)	{ m_strPlayerIP = strIP; }
	tagBounsInfo * GetBounsInfo()		{ return m_pBInfo; }

	virtual BOOL DoOperator();

};
