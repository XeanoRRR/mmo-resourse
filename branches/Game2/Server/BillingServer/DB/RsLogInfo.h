#ifndef  _INC_RSLOGINFO
#define  _INC_RSLOGINFO

#pragma once
#include "../../../Public/ServerPublic/db/CnPool.h"
class CRsLogInfo :
	public CMyAdoBase
{
public:
	CRsLogInfo(void);
	virtual ~CRsLogInfo(void);

	// �û��ڵ�¼ʱ������һ����־��¼�����л����û���Ϣ����Ϸ��ɫ��Ϣ������ʱ��
	bool AccountEnterLog(LPCSTR szAccount, DWORD dwIP);

	// �û��ڽ����ɫʱ�����¸ü�¼�����ɫʱ��
	bool RoleEnterLog(LPCSTR szAccount, LPCSTR szRoleName, BYTE btRoleLevel, BYTE btWorldNumber);

	// �û�����ʱ�����¸ü�¼�Ľ�ɫ����ʱ�䣬���ʻ��뿪ʱ�䣨2��ʱ����ͬ��
	bool LeaveLog(LPCSTR szAccount);

	bool AccountLeaveLog(LPCSTR szAccount);

protected:
	// ��õ�ǰʱ��Ĵ�
	void GetSystemTimeAsString(OUT LPSTR szCurrentTime);

	// ͨ����Account��Role�ȹ�ϵȷ��LogInfoID�����ڸ���(update)
	DWORD GetLogInfoIDForUpdate(LPCSTR szAccount);
};

#endif //_INC_RSLOGINFO