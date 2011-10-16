#ifndef DB_MAIL_GOOD_H
#define DB_MAIL_GOOD_H
#include "BaseDBEntity.h"
#include <list>
#include "..\..\..\public\GUID.h"
#include "../public/DBDefine.h"

using namespace std;


class CDBMailGoods : public CBaseDBEntity
{
public:


private:
	CDBMailGoods();
public:
	CDBMailGoods(const CGUID& guid);
	virtual ~CDBMailGoods();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	
		
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType,vector<BYTE>& pBA);

	CDBMailGoods& operator=(CDBMailGoods& right)
	{
		CBaseDBEntity::operator=(right);
		if(this != &right)
		{

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

private:
	bool b_IsFirstCreated;
};

#endif//DB_GOOD_H