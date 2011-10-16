#ifndef DB_LINKMAN_H
#define DB_LINKMAN_H
#include "BaseDBEntity.h"
#include <list>
#include "..\..\..\public\GUID.h"
#include "../public/DBDefine.h"

using namespace std;

class CDBLinkMan : public CBaseDBEntity
{
public:
private:
	CDBLinkMan();
public:
	CDBLinkMan(const CGUID& guid);
	virtual ~CDBLinkMan();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	
	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType,vector<BYTE>& pBA);

	CDBLinkMan& operator=(CDBLinkMan& right)
	{
		CBaseDBEntity::operator=(right);
		if(this != &right)
		{
			b_IsFirstCreated = right.b_IsFirstCreated;
		}

		return *this;
	}

	// �Ƿ��Ǹմ�����δ��Load/Save�̴߳�����Ķ���
	bool	GetIsFirstCreated(void) { return b_IsFirstCreated; }
	void	SetIsFirstCreated(bool flag) { b_IsFirstCreated = flag; }

	// ��ʼ����������
	void				InitProperty();

	// �������������Ƿ���ָ��������������
	bool				FindTableUpdateFlag(const string& tableName);


	const CGUID&	GetPlayerID(void)					{return m_PlayerGuid;}
	void			SetPlayerID(const CGUID& guid)		{m_PlayerGuid = guid;}

private:
	bool b_IsFirstCreated;
	CGUID	m_PlayerGuid;
};

#endif//DB_GOOD_H