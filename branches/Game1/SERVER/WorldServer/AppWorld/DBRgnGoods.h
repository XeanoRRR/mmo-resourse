#ifndef DB_RGN_GOOD_H
#define DB_RGN_GOOD_H
#include "BaseDBEntity.h"
#include <list>
#include "..\..\..\public\GUID.h"
#include "../public/DBDefine.h"

using namespace std;


class CDBRgnGoods : public CBaseDBEntity
{
public:


private:
	CDBRgnGoods();
public:
	CDBRgnGoods(const CGUID& guid);
	virtual ~CDBRgnGoods();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);


	//void Load(long msgid, BYTE* msgBuf, long& msgBufPos, vector<BYTE>& pBA);
	void Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	
		
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType,vector<BYTE>& pBA);

	CDBRgnGoods& operator=(CDBRgnGoods& right)
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