#pragma once
#include "..\myadobase.h"


class CRsCDKey :
	public CMyAdoBase
{
public:
	CRsCDKey(void);
	virtual ~CRsCDKey(void);
    
	int CDKeyIsValidate(DWORD dwIP, const char *cdkey,vector<BYTE>& password, char *szOutBanTime);//1,CDKey���� 2,CDKey or password error 3,cdkey in ban(pOutBantime not null)
	bool CDKeyBan(const char* cdkey,long lminute);
	bool GetPassWord(_RecordsetPtr& rs,BYTE* pByte);
	bool GetPassWord(_RecordsetPtr& rs,string& strMD5);
	
	// �����������IP�󶨣������IP�󣬼�Ȼ��ҵ��˺�δ��������ͬ�������ڸ������ڽ��е�¼��Ϸ��
	// IP�ֶ�Ϊ����ͨ���˼��
	bool IPIsAllowed(DWORD dwIP);
	bool IPIsForbidded(DWORD dwIP);

	// ����Ҫ�����GM��¼���п��ƣ�ֻ����������ָ��IP�б�������ķ�Χ�ڽ��е�¼��Ϸ��
	// IP�б�Ϊ����ͨ���˼��
	bool IsBetweenIP(const char *szCdkey, DWORD dwIP);
	
};
