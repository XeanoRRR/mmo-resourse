//EntityProperty.h/////////////////////////////////////////////////////////////////////
//��̬������:
//Author:������
//Create Time:2008.11.03
#ifndef ENTITY_PROPERTY_H
#define ENTITY_PROPERTY_H

#include "../../DB/DBDefine.h"

#define MAX_DEFINED_ID		1024
#define INVILID_ENTITY_ID	-99999999
#define OPER_ALL_DATA 0xFFFF

// ��װC++�������ݶ���
class CWrapDataObject
{
public:
	bool operator ==(const CWrapDataObject& right)
	{
		if(&right != this)
		{
			if( (right.m_Buf == m_Buf) && (right.m_Size == m_Size) )
				return true;
			else
				return false;
		}

		return false;
	}

	CWrapDataObject();
	~CWrapDataObject();

	// ������ݽӿ�
	void Release(void);

	// ��ӿ�
	// �������ݵ������Ϣ
	void*				GetBuf		(void)	{ return m_Buf;			}		// ������ָ�� / ����4�ֽ�����
	long				GetSize		(void)	{ return m_Size;		}		// ����������(BYTE)

	void	SetBuf		(void*				value)	{ m_Buf			= value;}		// ������ָ�� / ����4�ֽ�����
	void	SetSize		(long				value)	{ m_Size		= value;}		// ����������(BYTE)

	//	--------------------- ���ݲ����ӿ� ---------------------	//
	//	ADD------------------------------------------------------------
	bool AddLongValue(long value);
	bool AddBufValue(void* buf, long size);
	bool AddStringValue(const char* value, long size);
	bool AddGUIDValue(const CGUID& value);

	//	DEL------------------------------------------------------------
	bool DelValue();

	//	GET------------------------------------------------------------
	long GetLongValue();
	long GetBufValue(void* buf, long bufSize);
	const char* GetStringValue();
	const CGUID& GetGUIDValue();

	//	SET------------------------------------------------------------
	bool SetLongValue	(long value);
	bool SetBufValue	(void* value, long size);
	bool SetStringValue	(const char* value, long size);
	bool SetGUIDValue	(const CGUID& value);

	CWrapDataObject& operator=(CWrapDataObject& right);

private:
	bool SetValueArray	(void* tbuf, long bufSize);
	// char����
	bool AddValueArray(void* buf, long size);
	// time����
	bool AddValueArray(long* buf, long size);
	long GetValueArray(void** buf, long bufSize);

	// �������ݵ������Ϣ
	//DATA_OBJECT_TYPE	m_DataType;	// ��������
	void*				m_Buf;		// ������ָ�� / ����4�ֽ�����
	long				m_Size;		// ����������(BYTE), -1��ʾ����
};

// ���ݶ���(���ݱ�)
class CEntityProperty
{
public:

	//CEntityProperty(const std::string& name=std::string("EP"), DATA_OBJECT_TYPE type=DATA_OBJECT_UNKNOWN);
	CEntityProperty(long lAttrEnum);
	~CEntityProperty();

	// ������ݽӿ�
	void Release(void);

	// ��ӿ�
	//void SetEPName(const std::string& name) { m_EPName = name; }
	//const std::string& GetEPName(void) { return m_EPName; }

	long	GetAttrNumEnum(void) { return m_lAttrNumEnum; }
	void	SetAttrNumEnum(long value) { m_lAttrNumEnum = value; }

	// push DataObject
	CWrapDataObject* AddDataObject();

	// Get DataObject
	CWrapDataObject* GetDataObject();

	// Del DataObject
	bool DelDataObject();

	// 
	void CopyWrapDataObject(CWrapDataObject* desVec)
	{
		if(desVec)
			*desVec = *m_pDataObject;
	}

	CWrapDataObject* GetWrapDataObject(void)
	{
		return m_pDataObject;
	}

	//void SetAttrType( , DATA_OBJECT_TYPE type);

	int  GetLongAttr(  );
	void SetLongAttr( int p_val );
	bool HasLongAttr(  );
	void DelLongAttr(  );

	const char*  GetStringAttr	(  );
	void SetStringAttr			( const std::string& p_name );
	bool HasStringAttr			(  );
	void DelStringAttr			(  );

	long GetBufSize(  );
	long GetBufAttr( void* buf, long bufSize );
	void SetBufAttr( void* buf, long bufSize );
	bool HasBufAttr(  );
	void DelBufAttr(  );

	bool GetGuidAttr( CGUID& guid );
	void SetGuidAttr( const CGUID& guid );
	bool HasGuidAttr( );
	void DelGuidAttr( );

	CEntityProperty& operator=(CEntityProperty& right);

	BYTE	GetDbRelateType(void)					{ return m_DbRelateType; }
	void	SetDbRelateType(DB_RELATE_TYPE type)	{ m_DbRelateType = type; }

protected:
	BYTE	m_DbRelateType;
	long    m_lAttrNumEnum;// ������ö��ֵ
	CWrapDataObject* m_pDataObject;
};


// ���ݶ��������
class CDataEntityManager
{
public:
	typedef std::map<long, CEntityProperty*> EntityPropertyMap;
	typedef EntityPropertyMap::iterator EntityPropertyMapItr;

	CDataEntityManager();
	~CDataEntityManager();

	CDataEntityManager& operator=(CDataEntityManager& right);

	// �������
	void Release(void);

	// �������ݶ���
	CEntityProperty* CreateEntityProperty(long lAttrEnum);

	// ɾ�����ݶ���
	bool DelEntityProperty(long lAttrEnum);

	// ȡ�����ݶ���
	CEntityProperty* GetEntityProperty(long lAttrEnum);

	// ȡ�����ݶ���ֵ
	long			GetLongAttr(long lAttrEnum);
	const char*		GetStringAttr(long lAttrEnum);
	long			GetBufAttr(long lAttrEnum, void* buf, long bufSize);
	long			GetGuidAttr(long lAttrEnum,  CGUID& guid);

	// �������ݶ���ֵ
	bool			SetLongAttr(long lAttrEnum, long value);
	bool			SetStringAttr(long lAttrEnum, const char* value);
	bool			SetBufAttr(long lAttrEnum, void* buf, long bufSize);
	bool			SetGuidAttr(long lAttrEnum, const CGUID& guid);

	// �������ݶ���ֵ
	void			AddLongAttr		(long lAttrEnum, long value);
	void			AddStringAttr	(long lAttrEnum, const char* value);
	void			AddBufAttr		(long lAttrEnum, void* buf, long bufSize);
	void			AddTimeAttr		(long lAttrEnum, long* buf, long bufSize);
	void			AddGuidAttr		(long lAttrEnum, const CGUID& guid);

	// ɾ�����ݶ���ֵ
	void			DelLongAttr		(long lAttrEnum);
	void			DelStringAttr	(long lAttrEnum);
	void			DelBufAttr		(long lAttrEnum);
	void			DelTimeAttr		(long lAttrEnum);
	void			DelGuidAttr		(long lAttrEnum);

	EntityPropertyMap& GetEntityPropertyMap(void) { return m_mapEntityProperty; }

private:
	EntityPropertyMap m_mapEntityProperty;
};


// ������ʱ����
class CTempEntityProperty : public CEntityProperty
{
public:
	CTempEntityProperty(const string& strName, DATA_OBJECT_TYPE type);
	~CTempEntityProperty();

	const string& GetVarName(void) { return m_strVarName; }
	DATA_OBJECT_TYPE GetDataType(void) { return m_eDataType; }

	CTempEntityProperty& operator=(CTempEntityProperty& right);
private:
	string m_strVarName;
	DATA_OBJECT_TYPE m_eDataType;
};

#endif//ENTITY_PROPERTY_H