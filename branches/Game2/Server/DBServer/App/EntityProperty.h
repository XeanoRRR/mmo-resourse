#ifndef ENTITY_PROPERTY_H
#define ENTITY_PROPERTY_H
#include <map>
#include <string>
#include <list>
#include "DBEntityDef.h"

#define MAX_DEFINED_ID		1024
#define INVILID_ENTITY_ID	-99999999
#define OPER_ALL_DATA 0xFFFF
typedef unsigned long ulong;
typedef unsigned short ushort;
/*
|----
|OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|MSG ID:        4byte, ����Ϣ��MSGID
|Entity Num:	2byte, ��ʵ�����(X)
---------------�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����----------------
|	|----
|   |EntityID:		4byte, ʵ��ID
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |�����Ը���:		2byte, �������ݸ���(M) ȫ���� 0xFFFF OPER_ALL_DATA
|X*	|N * |����������:		4byte, ��������ö��ֵ
|	|	 |������ID:			4byte, ����ID, 
|	|	 |����������:		nbyte, ��������,�ַ���
|	|	 |		|----
|	|	 |		|������POS:		2byte, ����POS, 
|	|	 |		|������size:	2byte, ����SIZE, 
|	|	 |	M *	|���������ݿ�:	nbtye, �����ݿ鳤�����������ݳ��Ⱦ��� (����ϢΪLOAD������ݿ�Ϊ1byte��ֵΪ0)
|	|	 |		|----
|	|	 |----
|	|----
*/
enum DATA_OBJECT_TYPE
{
	DATA_OBJECT_UNKNOWN = 0,
	DATA_OBJECT_FLOAT,
	DATA_OBJECT_LONG,
	DATA_OBJECT_SHORT,
	DATA_OBJECT_ULONG,
	DATA_OBJECT_USHORT,
	DATA_OBJECT_BOOL,
	DATA_OBJECT_CHAR,
	DATA_OBJECT_BYTE,
	DATA_OBJECT_WORD,
	DATA_OBJECT_DWORD,
	DATA_OBJECT_STRING,

	DATA_OBJECT_FLOAT_ARRAY,
	DATA_OBJECT_LONG_ARRAY,
	DATA_OBJECT_SHORT_ARRAY,
	DATA_OBJECT_ULONG_ARRAY,
	DATA_OBJECT_USHORT_ARRAY,
	DATA_OBJECT_BOOL_ARRAY,
	DATA_OBJECT_CHAR_ARRAY,
	DATA_OBJECT_BYTE_ARRAY,
	DATA_OBJECT_WORD_ARRAY,
	DATA_OBJECT_DWORD_ARRAY,

	DATA_OBJECT_STRING_ARRAY, // ��:  {"124", "2235",...}
	DATA_OBJECT_BUFFER_ARRAY, // ��:  {tagTime[6]}
};

// ��װC++�������ݶ���
class CWrapDataObject
{
public:
	// �������ݵ������Ϣ
	typedef struct tagValueInfo
	{
		long				pos;		// ������λ��,��list,vector���ݽṹ�е�pos
		DATA_OBJECT_TYPE	dataType;	// ��������
		void*				buf;		// ������ָ��
		long				size;		// ����������(BYTE)

		tagValueInfo()
		{

		}
		~tagValueInfo()
		{

		}

		bool operator ==(const tagValueInfo& right)
		{
			if( (&right == this)
				|| (right.pos == pos && right.dataType == dataType
				&& right.buf == buf && right.size == size) )
				return true;

			return false;
		}
	}ValueInfo;

	typedef std::map<long, ValueInfo*> ValueInfoMap;
	typedef ValueInfoMap::iterator ValueInfoMapItr;

	CWrapDataObject();
	CWrapDataObject(DATA_OBJECT_TYPE type, long id=INVILID_ENTITY_ID, std::string& name=string("object"));
	~CWrapDataObject();

	

	// ��ӿ�
	void SetID(long id) { m_ID = id; }
	long GetID(void)	{ return m_ID; }

	void SetDataType(DATA_OBJECT_TYPE type) { m_DataType = type; }
	DATA_OBJECT_TYPE GetDataType(void)		{ return m_DataType; }

	void SetName(const std::string name)	{ m_Name = name; }
	const std::string& GetName(void)		{ return m_Name; }

	//	--------------------- ���ݲ����ӿ� ---------------------	//
	//	ADD------------------------------------------------------------
	bool AddValueArray(long pos, DATA_OBJECT_TYPE dataType, void* buf, long size);

	//	DEL------------------------------------------------------------
	bool DelValueArray(long pos);

	//	GET------------------------------------------------------------
	ValueInfo* GetValueArray(long pos);

	// FIND------------------------------------------------------------
	long GetValueArray(ValueInfo& valueInfo);

	// ��־����
	// �Ӷ��к����
	void PushBackOperValuePosFlag(DB_OPERATION_TYPE op, WORD pos);
	// �Ӷ���ǰ����
	WORD PopFrontOperValuePosFlag(DB_OPERATION_TYPE op);
	// ��ն���
	void ClearOperValuePosFlag(DB_OPERATION_TYPE op);
	// num
	long GetOperValuePosFlagNum(DB_OPERATION_TYPE op);

private:
	std::map<long, tagValueInfo*> m_mapDataValue;

	long				m_ID;		// �ڸ�ObjectValue���ݽṹ�е�λ��
	std::string			m_Name;		// ����
	DATA_OBJECT_TYPE	m_DataType; // ��������

	// ��¼ĳλ�����Ա�������״̬
	std::list<WORD> m_LoadValuePosFlag;
	std::list<WORD> m_SaveValuePosFlag;
};

// ���ݶ���(���ݱ�)
class CEntityProperty
{
public:
	typedef std::map<std::string, CWrapDataObject*> WrapDataObjectMap;
	typedef WrapDataObjectMap::iterator WrapDataObjectMapItr;

	CEntityProperty(long id=INVILID_ENTITY_ID, std::string& name=string("EP"));
	~CEntityProperty();

	// ��ӿ�
	void SetID(long id) { m_ID = id; }
	long GetID(void)	{ return m_ID; }

	void GetName(const std::string& name) { m_Name = name; }
	const std::string& GetName(void) { return m_Name; }

	// Add DataObject
	bool AddDataObject(CWrapDataObject* dataObj);
	// Add DataObject
	bool AddDataObject(DATA_OBJECT_TYPE type, std::string& objName);

	// Get DataObject
	CWrapDataObject* GetDataObject(std::string& name);

	// Del DataObject
	bool DelDataObject(std::string& name);

	// ����һ��ID
	long AllocDataObjectID();
	// �ͷ�һ��ID
	void FreeDataObjectID(long id);

	// ��־����
	// �Ӷ��к����
	void PushBackOperDataObjFlag(DB_OPERATION_TYPE op, string& name);
	// �Ӷ���ǰ����
	string PopFrontOperDataObjFlag(DB_OPERATION_TYPE op);
	// num
	long GetOperDataObjFlagNum(DB_OPERATION_TYPE op);

private:
	std::map<std::string, CWrapDataObject*> m_mapDataObject;

	// ID
	long		m_ID;
	// ���ݿ�������ݴ�name:  [name]_Table 
	std::string m_Name;

	// ��ǰDataObjectID
	long m_CurDOID;
	// ����DataObjectID
	std::list<long> m_FreeDOID;

	// ��¼ĳλ�����Ա�������״̬
	std::list<string> m_LoadDataObjFlag;
	std::list<string> m_SaveDataObjFlag;
};


// ���ݶ��������
class CDataEntityManager
{
public:
	typedef std::map<std::string, CEntityProperty*> EntityPropertyMap;
	typedef EntityPropertyMap::iterator EntityPropertyMapItr;

	CDataEntityManager();
	~CDataEntityManager();

	// �������ݶ���
	CEntityProperty* CreateEntityProperty(std::string& name);

	// ɾ�����ݶ���
	bool DelEntityProperty(std::string& name);

	// ȡ�����ݶ���
	CEntityProperty* GetEntityProperty(std::string& name);

	// ������Ϣ
	void ProcessMsg(CMessage* msg);
	// ������Ϣ
	void ProcessMsg(CEntityProperty* ep, DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long msgBufPos, vector<BYTE>& pBA);

	// �����Ϣ
	void AddMsg(CMessage* msg);
	// �����Ϣ
	void AddMsg(CEntityProperty* ep, DB_OPERATION_TYPE op, long msgid, CWrapDataObject* wdo, vector<BYTE>& pBA);

	// ����һ��ID
	long AllocEntityPropertyID();
	// �ͷ�һ��ID
	void FreeEntityPropertyID(long id);

private:
	std::map<std::string, CEntityProperty*> m_mapEntityProperty;

	// ��ǰEntityPropertyID
	long m_CurEPID;
	// ����EntityPropertyID
	std::list<long> m_FreeEPID;
};
#endif//ENTITY_PROPERTY_H