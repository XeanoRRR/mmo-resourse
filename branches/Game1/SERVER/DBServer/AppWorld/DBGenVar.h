#ifndef DB_GEN_VAR_H
#define DB_GEN_VAR_H
#include "BaseDBEntity.h"
#include "../public/DBDefine.h"

class CDBGenVar : public CBaseDBEntity
{
public:
	CDBGenVar();
	CDBGenVar(const CGUID& id);
	~CDBGenVar();

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void AddToByteArray(vector<BYTE>& pBA);

	virtual CDBGenVar& operator=(CDBGenVar& right);

	// �������������Ƿ���ָ��������������
	bool				FindTableUpdateFlag(const string& tableName);
	void				InitProperty(void);

};

#endif//DB_GEN_VAR_H