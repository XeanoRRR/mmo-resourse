/**
*File:  BaseEntity.cpp
*brief: ����������:���ж������̳�
*Author:������
*Date:  2008.11.03
*/
#include "StdAfx.h"
#include "BaseEntity.h"

#include "EntityManager.h"


//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBaseEntity::CBaseEntity()
: m_GUID(NULL_GUID),m_CompositeType(COM_COMPONENT),
m_bHasAttrs(0),m_bCurDbOperFlag(0),m_bCurDbOperType(DB_OPER_INIT),
m_CompositeFlag(""),m_bIsTempEntityFlag(0)
{
};
CBaseEntity::CBaseEntity(const CGUID& guid)
: m_GUID(guid),m_CompositeType(COM_COMPONENT),
m_bHasAttrs(0),m_bCurDbOperFlag(0),m_bCurDbOperType(DB_OPER_INIT),
m_CompositeFlag(""),m_bIsTempEntityFlag(0)
{

};
CBaseEntity::~CBaseEntity()
{
	m_DataEntityManager.Release();

	
};
CBaseEntity& CBaseEntity::operator=(CBaseEntity& right)
{
	if(this != &right)
	{
		m_DataEntityManager.Release();

		m_GUID = right.m_GUID;
		m_CompositeType = right.m_CompositeType;	
		m_DataEntityManager = right.m_DataEntityManager;
		m_bIsTempEntityFlag = right.m_bIsTempEntityFlag;
		m_bHasAttrs = right.m_bHasAttrs;
		m_bCurDbOperFlag = right.m_bCurDbOperFlag;
		m_bCurDbOperType = right.m_bCurDbOperType;
	}
	return *this;
}

// �������ݶ���ֵ
long CBaseEntity::GetLongAttr(const std::string& name)
{
	return m_DataEntityManager.GetLongAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
const char* CBaseEntity::GetStringAttr(const std::string& name)
{
	return m_DataEntityManager.GetStringAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
long CBaseEntity::GetBufAttr(const std::string& name, void* buf, long bufSize)
{
	return m_DataEntityManager.GetBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), buf, bufSize);
}
void CBaseEntity::GetTimeAttr(const std::string& name, long* buf, long bufSize)
{
	m_DataEntityManager.GetBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), (void*)buf, bufSize);
}
void CBaseEntity::GetGuidAttr(const std::string& name, CGUID& guid)
{
	m_DataEntityManager.GetGuidAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), guid);
}


// �������ݶ���ֵ
void CBaseEntity::SetLongAttr(const std::string& name, long value)
{
	m_DataEntityManager.SetLongAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), value);
}
void CBaseEntity::SetStringAttr(const std::string& name, const char* value)
{
	m_DataEntityManager.SetStringAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), value);
}
void CBaseEntity::SetBufAttr(const std::string& name, void* buf, long bufSize)
{
	m_DataEntityManager.SetBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), buf, bufSize);
}
void CBaseEntity::SetTimeAttr(const std::string& name, long* buf, long bufSize)
{
	m_DataEntityManager.SetBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), (void*)&buf[0], bufSize);
}
void CBaseEntity::SetGuidAttr(const std::string& name, const CGUID& guid)
{
	m_DataEntityManager.SetGuidAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), guid);
}
// �������ݶ���ֵ
void CBaseEntity::AddLongAttr(const std::string& name, DATA_OBJECT_TYPE type, long value)
{
	m_DataEntityManager.AddLongAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), value);
}
void CBaseEntity::AddStringAttr(const std::string& name, const char* value)
{
	m_DataEntityManager.AddStringAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), value);
}
void CBaseEntity::AddBufAttr (const std::string& name, void* buf, long bufSize)
{
	m_DataEntityManager.AddBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), buf, bufSize);
}
void CBaseEntity::AddTimeAttr (const std::string& name, long* buf, long bufSize)
{
	m_DataEntityManager.AddTimeAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), buf, bufSize);
}
void CBaseEntity::AddGuidAttr(const std::string& name, const CGUID& guid)
{
	m_DataEntityManager.AddGuidAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name), guid);
}

// ɾ�����ݶ���ֵ
void CBaseEntity::DelLongAttr(const std::string& name)
{
	m_DataEntityManager.DelLongAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
void CBaseEntity::DelStringAttr(const std::string& name)
{
	m_DataEntityManager.DelStringAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
void CBaseEntity::DelBufAttr(const std::string& name)
{
	m_DataEntityManager.DelBufAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
void CBaseEntity::DelTimeAttr(const std::string& name)
{
	m_DataEntityManager.DelTimeAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
void CBaseEntity::DelGuidAttr(const std::string& name)
{
	m_DataEntityManager.DelGuidAttr(GetGame()->GetEntityManager()->GetAttrEnumByStr(GetCompositeFlag(), name));
}
