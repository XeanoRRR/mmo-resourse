//BaseEntity.h/////////////////////////////////////////////////////////////////////
//����������:���ж������̳�
//Author:������
//Create Time:2008.11.03
#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "../Session/WorldServerSession.h"
#include "../../../../Public/ServerPublic/Server/Common/EntityProperty.h"

/*
��Ϣ���(ÿ����Ϣֻ����һ���ڵ��Լ��ýڵ������Ҷ�ӽڵ�����)��
|---- ����-------------------------------------
|needBackFlag:	1byte, �Ƿ���Ҫ��Ӧ
|SessionID:		17byte, ��Ҫ���ص�SessionID
|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|rootComFlag:	4byte, ���ڵ�flag
|rootGuid:		17byte,���ڵ�guid
|rootName:		32byte,���ڵ�����
--�ýڵ������ֵ----�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����-----------
|	|QueryAttrNum:	2byte, ��ѯ�ֶθ���(N)
|	|	 |----
|	|	 |nameLen:  4byte,  ��ѯ�ֶ����ַ�������
|	|N * |szName;   nbyte,  ��ѯ�ֶ����ַ���
|	|	 |varType:  4byte,  ��ѯ�ֶ�����
|	|	 |varLen:	4byte,  ��ѯ�ֶ����ݳ���
|	|	 |varBuf:   nbyte,  ��ѯ�ֶ�����
|	|	 |relate:   1byte,  ����һ����ѯ�ֶεĹ�ϵ(AND OR)
|	|	 |----
|	|Attr Num:		2byte, �����������(N)
|X*	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|childNum:		4byte, �ӽڵ����(X)
|	|----
|   |compositeFlag: 4byte, ���ӽڵ�flag
|   |EntityID:		17byte,���ӽڵ�guid
|	|entityName:	32byte,���ӽڵ�����
|	|findKey:		1byte, ���ӽڵ���ҷ�ʽ 0:guid,1:string
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|X*	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|---- ����-------------------------------------

|---- ����-------------------------------------
|SessionID:		17byte, ��Ҫ���ص�SessionID
|retSeType:		4byte, ��Ҫ���ص�SessionID
|retFlag:		1byte, ��Ҫ���صı�־λ
|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|rootComFlag:	4byte, ���ڵ�flag
|rootGuid:		17byte,���ڵ�guid
|rootName:		32byte,���ڵ�����
|findKey:		1byte, �������ҷ�ʽ 0:guid,1:Set String,2:Find String
|fatherComFlag:	4byte, ���ڵ�flag
|fatherGuid:	17byte,���ڵ�guid
|bufSize:			4byte, �������ݳ���ֵ
--�ýڵ������ֵ----�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����-----------
|	|RetAttrNum:	2byte, ���ز�ѯ�õ����ֶθ���(N)
|	|	 |----
|	|	 |nameLen:  4byte,  ���ز�ѯ�õ����ֶ����ַ�������
|	|N * |szName;   nbyte,  ���ز�ѯ�õ����ֶ����ַ���
|	|	 |varType:  4byte,  ���ز�ѯ�õ����ֶ�����
|	|	 |varLen:	4byte,  ���ز�ѯ�õ����ֶ����ݳ���
|	|	 |varBuf:   nbyte,  ���ز�ѯ�õ����ֶ�����
|	|	 |----
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|X*	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|childNum:		4byte, �ӽڵ����(X)
|	|----
|   |compositeFlag: 4byte, ���ӽڵ�flag
|   |EntityID:		17byte,���ӽڵ�guid
|	|entityName:	32byte,���ӽڵ�����
|	|findKey:		1byte, ���ӽڵ���ҷ�ʽ 0:guid,1:string
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |nameLen:  4byte, �������ַ�������
|X*	|N * |szName;   nbyte, �������ַ���
|	|	 |varType:  4byte, ��������
|	|	 |varLen:	4byte, �������ݳ���
|	|	 |varBuf:   nbyte, ��������
|	|	 |----
|---- ����-------------------------------------
*/


// ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
class CBaseEntity
{
public:
	CBaseEntity();
	CBaseEntity(const CGUID& guid);
	virtual ~CBaseEntity();

public: 
	const string& GetCompositeFlag(void) { return m_CompositeFlag; }

	virtual void AddChild(CBaseEntity* entity) {}
	virtual void RemoveChild(const CGUID& guid)  {}
	virtual CBaseEntity* GetChild(const CGUID& guid) { return NULL; }

	virtual void ProcessMsg(bool isLoadOperFlag, const CGUID& SessionID, long retFlag, BYTE* msgBuf, long& msgBufPos, long bufSize) = 0;
	virtual void GetFromByteArray(bool isLoadOperFlag, BYTE* msgBuf, long& msgBufPos, long bufSize=-1) = 0;
	virtual void AddToByteArray(std::vector<BYTE>& pBA) = 0;


	// GetID
	const CGUID& GetGUID(void) { return m_GUID; }
	// SetID
	void SetGUID(const CGUID& id) { m_GUID = id; }

	// ��ʼ����������
	virtual void InitProperty() {};

	CDataEntityManager& GetDataEntityManager(void) { return m_DataEntityManager; }

	// �������ݶ���ֵ
	void			SetLongAttr(const std::string& name, long value);
	void			SetStringAttr(const std::string& name, const char* value);
	void			SetBufAttr(const std::string& name, void* buf, long bufSize);
	void			SetTimeAttr(const std::string& name, long* buf, long bufSize);
	void			SetGuidAttr(const std::string& name, const CGUID& guid);

	// �������ݶ���ֵ
	long			GetLongAttr(const std::string& name);
	const char*		GetStringAttr(const std::string& name);
	long			GetBufAttr(const std::string& name, void* buf, long bufSize);
	void			GetTimeAttr(const std::string& name, long* buf, long bufSize);
	void			GetGuidAttr(const std::string& name, CGUID& guid);

	// �������ݶ���ֵ
	void			AddLongAttr		(const std::string& name, DATA_OBJECT_TYPE type, long value);
	void			AddStringAttr	(const std::string& name, const char* value);
	void			AddBufAttr		(const std::string& name, void* buf, long bufSize);
	void			AddTimeAttr		(const std::string& name, long* buf, long bufSize);
	void			AddGuidAttr		(const std::string& name, const CGUID& guid);

	// ɾ�����ݶ���ֵ
	void			DelLongAttr		(const std::string& name);
	void			DelStringAttr	(const std::string& name);
	void			DelBufAttr		(const std::string& name);
	void			DelTimeAttr		(const std::string& name);
	void			DelGuidAttr		(const std::string& name);
	//
	long			GetBufSize		(const std::string& name);

	virtual CBaseEntity& operator=(CBaseEntity& right);
	void SetCompositeFlag(const string& type) { m_CompositeFlag = type; }
	void SetCompositeType(COMPOSITE_TYPE type) { m_CompositeType = type; }
	COMPOSITE_TYPE GetCompositeType(void) { return m_CompositeType; }

	BYTE GetCurDbOperFlag(void) { return m_bCurDbOperFlag; }
	void SetCurDbOperFlag(BYTE flag) { m_bCurDbOperFlag = flag; }

	BYTE GetCurDbOperType(void) { return m_bCurDbOperType; }
	void SetCurDbOperType(BYTE flag) { m_bCurDbOperType = flag; }

protected:
	BYTE m_bCurDbOperFlag; // �Ƿ�ִ�е�ǰ���ݿ������־
	BYTE m_bCurDbOperType; // ��ǰ���ݿ��������	
	CGUID m_GUID;
	COMPOSITE_TYPE m_CompositeType;
	string m_CompositeFlag;
	CDataEntityManager m_DataEntityManager;	
};

#endif//DB_BASEENTITY_H
