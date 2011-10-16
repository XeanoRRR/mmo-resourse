#pragma once

#include "../../../Public/Common/BounsDef.h"
#include "../../../Public/ServerPublic/db/CnPool.h"
class CRsBouns :
	public CMyAdoBase
{
private:
	CRITICAL_SECTION m_csBouns;
public:
	CRsBouns(void);
	virtual ~CRsBouns(void);

	// ���ؽ���������Ϣ
	HRESULT LoadBounsDes(map<DWORD,tagBounsDes> &mapBD);

	// ����ĳ����������Ϣ
	HRESULT LoadBounsDesByID(_ConnectionPtr& cn, DWORD dwBounsID, tagBounsDes& tgBD);

	// ��ȡ����콱��Ϣ
	HRESULT LoadBounsInfo(_ConnectionPtr& cn, const char* strAccount, DWORD dwAreaServerID, DWORD dwMaxBounsNum, list<tagBounsInfo*>& listBounsInfo);

    // ��ȡ��Ʒ bOper 1-�콱�� 2-����
    long GetBouns(_ConnectionPtr& cn, const CGUID& guid,const CGUID& playerID, const char* strPlayerName, DWORD dwWSID, const char* strIP, long &lRet);

    HRESULT	FreezeBouns(_ConnectionPtr& cn, const CGUID& guid, DWORD& dwBounsID, long &lRet);

	// �콱ʧ�ܻع�
	HRESULT BounsRollBack(_ConnectionPtr& cn, const CGUID& guid);

	// SYSTEMTIME to time_t
	time_t SystemtimeToTime(const SYSTEMTIME &st);

};
