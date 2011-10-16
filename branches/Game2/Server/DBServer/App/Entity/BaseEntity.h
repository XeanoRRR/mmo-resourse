/**
*File:  BaseEntity.h
*brief: ����������:���ж������̳�
*Author:������
*Date:  2008.11.03
*/
#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "../../../../Public/ServerPublic/Server/Common/EntityProperty.h"

/*
��Ϣ���(ÿ����Ϣֻ����һ���ڵ��Լ��ýڵ������Ҷ�ӽڵ�����)��
|----������Ϣ--------------------------------------
|SessionID:		17byte, ��Ҫ���ص�SessionID
|rootComFlag:	4byte, ���ڵ�flag
|rootGuid:		17byte,���ڵ�guid
|rootName:		32byte,�ö�������������
|findKey:		1byte, �������ҷ�ʽ 0:guid,1:Set String,2:Find String
--�ýڵ������ֵ----�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����-----------
|	|QueryAttrNum:	2byte, ��ѯ�ֶθ���(N)
|	|	 |----
|	|N * |szName;   nbyte,  ��ѯ�ֶ����ַ���
|	|	 |varType:  4byte,  ��ѯ�ֶ�����
|	|	 |varLen:	4byte,  ��ѯ�ֶ����ݳ���
|	|	 |varBuf:   nbyte,  ��ѯ�ֶ�����
|	|	 |QueryType:1byte,  ��ѯ�ֶ���������(AND,OR��)
|X*	|	 |----
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|childNum:		4byte, �ӽڵ����(X)
|	|----
|   |compositeTyoe: 1byte, ���ӽڵ�Type
|   |compositeFlag: 4byte, ���ӽڵ�flag
|   |EntityID:		17byte,���ӽڵ�guid
|	|QueryAttrNum:	2byte, ��ѯ�ֶθ���(N)
|	|	 |----
|	|N * |szName;   nbyte,  ��ѯ�ֶ����ַ���
|	|	 |varType:  4byte,  ��ѯ�ֶ�����
|	|	 |relate:   1byte,  ����һ����ѯ�ֶεĹ�ϵ(AND OR)
|	|	 |----
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|X*	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|----������Ϣ--------------------------------------

|----������Ϣ--------------------------------------
|SessionID:		17byte, ��Ҫ���ص�SessionID
|retFlag:		1byte, ��Ҫ���صı�־λ
|rootComFlag:	4byte, ���ڵ�flag
|rootGuid:		17byte,���ڵ�guid
|rootName:		32byte,�ö�������������
|findKey:		1byte, �������ҷ�ʽ 0:guid,1:Set String,2:Find String
--�ýڵ������ֵ----�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����-----------
|	|comType:		1byte, comtype
|	|comFlag:		4byte, comFlag
|	|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|	|entityID:		17byte,guid
|	|Attr Num:		2byte, �����������(N)
|	|	 |----	
|	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|childNum:		2byte, �ӽڵ����(X)
|	|----
|   |compositeTyoe: 1byte, ���ӽڵ�Type
|   |compositeFlag: 4byte, ���ӽڵ�flag
|	|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|   |EntityID:		17byte,���ӽڵ�guid
|	|entityName:	32byte,�ö�������
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|----������Ϣ--------------------------------------
*/

// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CBaseEntity
{
public:
	CBaseEntity();
	CBaseEntity(const CGUID& guid);

	virtual ~CBaseEntity();

	const string& GetCompositeFlag(void) { return m_CompositeFlag; }

	virtual void GetFromByteArray(BYTE* msgBuf, long& msgBufPos, long bufSize=-1) = 0;
	virtual void AddToByteArray(std::vector<BYTE>& pBA, bool isLoadOperFlag) = 0;

	// GetID
	const CGUID& GetGUID(void) { return m_GUID; }
	// SetID
	void SetGUID(const CGUID& id) { m_GUID = id; }

	// ��ʼ����������
	virtual void InitProperty() {};

	CDataEntityManager& GetDataEntityManager(void) { return m_DataEntityManager; }

	// �������ݶ���ֵ
	void			SetLongAttr		(const std::string& name, long value);
	void			SetStringAttr	(const std::string& name, const char* value);
	void			SetBufAttr		(const std::string& name, void* buf, long bufSize);
	void			SetTimeAttr		(const std::string& name, long* buf, long bufSize);
	void			SetGuidAttr		(const std::string& name, const CGUID& guid);

	// �������ݶ���ֵ
	long			GetLongAttr		(const std::string& name);
	const char*		GetStringAttr	(const std::string& name);
	long			GetBufAttr		(const std::string& name,void* buf, long bufSize);
	void			GetTimeAttr		(const std::string& name,long* buf, long bufSize);
	void			GetGuidAttr		(const std::string& name,CGUID& guid);

	// �������ݶ���ֵ
	void			AddLongAttr		(const std::string& name,DATA_OBJECT_TYPE type, long value);
	void			AddStringAttr	(const std::string& name,const char* value);
	void			AddBufAttr		(const std::string& name,void* buf, long bufSize);
	void			AddTimeAttr		(const std::string& name,long* buf, long bufSize);
	void			AddGuidAttr		(const std::string& name,const CGUID& guid);

	// ɾ�����ݶ���ֵ
	void			DelLongAttr		(const std::string& name);
	void			DelStringAttr	(const std::string& name);
	void			DelBufAttr		(const std::string& name);
	void			DelTimeAttr		(const std::string& name);
	void			DelGuidAttr		(const std::string& name);

	virtual CBaseEntity& operator=(CBaseEntity& right);

	void SetCompositeFlag(const string& type)	{ m_CompositeFlag = type; }

	void SetCompositeType(COMPOSITE_TYPE type)	{ m_CompositeType = type; }
	COMPOSITE_TYPE GetCompositeType(void)		{ return (COMPOSITE_TYPE)m_CompositeType; }

	void SetHasAttrFlag(BYTE flag)				{ m_bHasAttrs = flag; }
	BYTE GetHasAttrFlag(void)					{ return m_bHasAttrs;}

	BYTE GetCurDbOperFlag(void)			{ return m_bCurDbOperFlag; }
	void SetCurDbOperFlag(BYTE flag)	{ m_bCurDbOperFlag = flag; }
	BYTE GetCurDbOperType(void)			{ return m_bCurDbOperType; }
	void SetCurDbOperType(BYTE flag)	{ m_bCurDbOperType = flag; }
	
	// �Ƿ���ʱ����Load����ʱ�ж�Ϊ1����ֱ���ڴ淵��WS��Ϣ
	BYTE		GetIsTempEntityFlag(void)		{ return m_bIsTempEntityFlag; }
	void		SetIsTempEntityFlag(BYTE flag)	{ m_bIsTempEntityFlag = flag; }

protected:
	BYTE m_bCurDbOperFlag; // �Ƿ�ִ�е�ǰ���ݿ������־
	BYTE m_bCurDbOperType; // ��ǰ���ݿ��������
	BYTE m_bHasAttrs;// �ö����Ƿ������ݱ��������ݿ�
	BYTE m_bIsTempEntityFlag;// �Ƿ���ʱ����Load����ʱ�ж�Ϊ1����ֱ���ڴ淵��WS��Ϣ
	BYTE m_CompositeType;
	CGUID m_GUID;
	string m_CompositeFlag;
	CDataEntityManager m_DataEntityManager;	
};



#endif//DB_BASEENTITY_H
