#ifndef BASE_DB_ENTITY_H
#define BASE_DB_ENTITY_H

#include <map>
#include <list>
#include <vector>
#include <string>

#include "..\..\..\nets\netdb\Message.h"
#include "../public/GUID.h"
#include "../../../public/entityproperty.h"

/*
��Ϣ��ƣ�
|----
|opType			4byte, ������������
|OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|MSG ID:        4byte, ����Ϣ��MSGID
|Entity Num:	2byte, ��ʵ�����(X)
---------------�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����----------------
|	|----
|   |EntityID:		4byte, ʵ��ID
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |�����Ը���:		2byte, �������ݸ���(M)
|X*	|N * |����������:		4byte, ��������ö��ֵ
|	|	 |		|----
|	|	 |	M *	|���������ݿ�:	nbtye, �����ݿ鳤�����������ݳ��Ⱦ��� (����ϢΪLOAD������ݿ�Ϊ1byte��ֵΪ0)
|	|	 |		|----
|	|	 |----
|	|----

*/

#include "dbentitydef.h"

class CBaseDBEntity
{
public:
	CBaseDBEntity()
		: m_GUID(0),b_IsFirstCreated(true)
	{
	};
	CBaseDBEntity(const CGUID& guid)
		: m_GUID(guid),b_IsFirstCreated(true)
	{
	};

	virtual ~CBaseDBEntity()
	{
	};

	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1) = 0;
	
	// �Ƿ��Ǹմ�����δ��Load/Save�̴߳�����Ķ���
	bool	GetIsFirstCreated(void) { return b_IsFirstCreated; }
	void	SetIsFirstCreated(bool flag) { b_IsFirstCreated = flag; }

	// GetID
	const CGUID& GetExID(void) { return m_GUID; }
	// SetID
	void SetExID(const CGUID& id) { m_GUID = id; }

	virtual CBaseDBEntity& operator=(CBaseDBEntity& right)
	{
		if(this != &right)
		{
			m_GUID = right.m_GUID;
			b_IsFirstCreated = right.b_IsFirstCreated;
			CDataEntityManager::EntityPropertyMapItr itr = right.m_DataEntityManager.GetEntityPropertyMap().begin();
			for(; itr != right.m_DataEntityManager.GetEntityPropertyMap().end(); itr++)
			{
				CEntityProperty* ep = m_DataEntityManager.CreateEntityProperty(itr->first, itr->second->GetDBTableName(), itr->second->GetDataType());
				*ep = *itr->second;
			}
			m_mapDirtyAttrData = right.m_mapDirtyAttrData;
		}
		return *this;
	}

	// ��ʼ����������
	virtual void InitProperty(const char* szFilePath) {};

	CDataEntityManager& GetDataEntityManager(void) { return m_DataEntityManager; }

	void    PushDirtyAttrData(string attrName) { m_mapDirtyAttrData.insert(attrName); }
	string  PopDirtyAttrData() 
	{ 
		string tmp = *m_mapDirtyAttrData.begin(); 
		m_mapDirtyAttrData.erase(m_mapDirtyAttrData.begin());
		return tmp; 
	}
	long    DirtyAttrDataSize(void) { return m_mapDirtyAttrData.size(); }
	set<string>& GetDirtyAttrDataMap(void) { return m_mapDirtyAttrData; }


	// �������ݶ���ֵ
	void			SetLongAttr(const std::string& name, const std::string& tableName, long pos, long value);
	void			SetStringAttr(const std::string& name, const std::string& tableName, long pos, const char* value);
	void			SetBufAttr(const std::string& name, const std::string& tableName, long pos, void* buf, long bufSize);
	void			SetTimeAttr(const std::string& name, const std::string& tableName, long pos, long* buf, long bufSize);
	void			SetGuidAttr(const std::string& name, const std::string& tableName, long pos, const CGUID& guid);

	// �������ݶ���ֵ
	long			GetLongAttr(const std::string& name, long pos);
	const char*		GetStringAttr(const std::string& name, long pos);
	void			GetBufAttr(const std::string& name, long pos, void** buf, long bufSize);
	void			GetTimeAttr(const std::string& name, long pos, long* buf, long bufSize);
	void			GetGuidAttr(const std::string& name, long pos, CGUID& guid);

	// �������ݶ���ֵ
	void			AddLongAttr		(const std::string& name, const std::string& tableName, long pos, DATA_OBJECT_TYPE type, long value);
	void			AddStringAttr	(const std::string& name, const std::string& tableName, long pos, const char* value);
	void			AddBufAttr		(const std::string& name, const std::string& tableName, long pos, void* buf, long bufSize);
	void			AddTimeAttr		(const std::string& name, const std::string& tableName, long pos, long* buf, long bufSize);
	void			AddGuidAttr		(const std::string& name, const std::string& tableName, long pos, const CGUID& guid);

	// ɾ�����ݶ���ֵ
	void			DelLongAttr		(const std::string& name, const std::string& tableName, long pos);
	void			DelStringAttr	(const std::string& name, const std::string& tableName, long pos);
	void			DelBufAttr		(const std::string& name, const std::string& tableName, long pos);
	void			DelTimeAttr		(const std::string& name, const std::string& tableName, long pos);
	void			DelGuidAttr		(const std::string& name, const std::string& tableName, long pos);

protected:
	CGUID m_GUID;
	// �Ƿ��Ǹմ�����δ��Load/Save�̴߳�����Ķ���
	bool	b_IsFirstCreated;
	CDataEntityManager m_DataEntityManager;	
	// ���������� ����ͻ��˷���ʱ���»�ȡ��������
	set<string>  m_mapDirtyAttrData;
};
#endif//BASE_DB_ENTITY_H