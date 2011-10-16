/**
*File:  Entity.cpp
*brief: ������:�ö���ֻ�����Լ�����������,�ڽṹ���г�ΪҶ�ӽڵ�
        ��Ϸ������࣬���ж������Ϸ��ص����ɸ�������
*Author:������
*Date:  2008.11.03
*/
#include "StdAfx.h"
#include "Entity.h"

#include "EntityGroup.h"
#include "EntityManager.h"

#include "../Thread/DBThread.h"
#include "../Thread/Threaddatadef.h"

#pragma warning(push)
#pragma warning(disable :  4244)

//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CEntity::CEntity(const string& flag, const CGUID& id)
	:	CBaseEntity(id)
{
	CBaseEntity::m_CompositeType = COM_LEAF;
	CBaseEntity::m_CompositeFlag = flag;
	InitProperty();
}

CEntity::~CEntity(void)
{
}

CEntity& CEntity::operator=(CEntity& right)
{
	CBaseEntity::operator=(right);
	return *this;
}

// ��ʼ����������
void CEntity::InitProperty()
{
	CEntityManager::AttrDefMap& mapAttrDef = GetGame()->GetEntityManager()->GetObjAttrDef();
	CEntityManager::AttrDefMapItr itr = mapAttrDef.find(GetCompositeFlag());
	if(itr != mapAttrDef.end())
	{
		// ��ʼ�����Ա���
		tagEntityBuildInfo* pInfo = itr->second;
		if(!pInfo) return;
		//m_DataEntityManager = *itr->second->pDataMgr;
		map<string, tagEntityAttrInfo>::iterator daEpItr = itr->second->pDataMgr.begin();
		for(; daEpItr != itr->second->pDataMgr.end(); daEpItr++)
		{
			m_DataEntityManager.CreateEntityProperty(daEpItr->second.lAttrTypeEnum);
		}

		// ��ʼ������
		m_CompositeType = (COMPOSITE_TYPE)pInfo->lComType;
		m_bHasAttrs = pInfo->lHhasAttrFlag;
	}
}


void CEntity::GetFromByteArray(BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	// ������Ϣ
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // �����������
	if(attrNum)
	{
		for(int i=0; i<(int)attrNum; i++)
		{
			long lAttrEnum = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//��������ö��ֵ��long��
			long varBufLen = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ݳ��ȣ�long��

			CEntityProperty* ep = NULL;
			long bufType = GetGame()->GetEntityManager()->GetDataObjectType(GetCompositeFlag(), lAttrEnum);
			CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().find(lAttrEnum);
			if(itr != GetDataEntityManager().GetEntityPropertyMap().end())
				ep = itr->second;

			assert(ep != NULL);

			switch((DATA_OBJECT_TYPE)bufType)
			{
			case DATA_OBJECT_TIME:
				{
					// 6��longֵ : �� �� �� ʱ �� ��
					BYTE timeValue[6*sizeof(DWORD)];
					memset(timeValue, 0, 6*sizeof(DWORD));
					_GetBufferFromByteArray(msgBuf, msgBufPos, timeValue, 6*sizeof(DWORD), bufSize);
					ep->SetBufAttr(timeValue, 6*sizeof(DWORD));
				}
				break;
			case DATA_OBJECT_STRING:
				{
					char varValue[1024];
					_GetStringFromByteArray(msgBuf, msgBufPos, varValue, bufSize);//��������
					ep->SetStringAttr(string((char*)varValue));
				}
				break;
			case DATA_OBJECT_FLOAT:
			case DATA_OBJECT_LONG:
			case DATA_OBJECT_SHORT:
			case DATA_OBJECT_ULONG:
			case DATA_OBJECT_USHORT:
			case DATA_OBJECT_BOOL:
			case DATA_OBJECT_CHAR:
			case DATA_OBJECT_BYTE:
			case DATA_OBJECT_WORD:
			case DATA_OBJECT_DWORD:
				{
					long value = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//��������
					ep->SetLongAttr((long)value);
				}
				break;
			case DATA_OBJECT_GUID:
				{
					CGUID tGUID;
					_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize);//��������
					ep->SetGuidAttr(tGUID);
				}
				break;
			case DATA_OBJECT_BUFFER:
				{
					BYTE* buf = new BYTE[varBufLen];
					_GetBufferFromByteArray(msgBuf, msgBufPos, buf, varBufLen, bufSize);//��������
					ep->SetBufAttr(buf, varBufLen);
					SAFE_DELETE_ARRAY(buf);
				}
				break;
			}
		}
	}
}

void CEntity::AddToByteArray(std::vector<BYTE>& pBA, bool isLoadOperFlag)
{
	_AddToByteArray(&pBA, (BYTE)GetCompositeType());
	_AddToByteArray(&pBA, GetCompositeFlag().c_str());
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID

	_AddToByteArray(&pBA, (BYTE)GetCurDbOperFlag());
	_AddToByteArray(&pBA, (BYTE)GetCurDbOperType());

	if(   (GetCurDbOperFlag() == false && !isLoadOperFlag )  // ���Ƕ��������Ҳ�����־Ϊ0
		|| (isLoadOperFlag && GetCurDbOperType() == DB_OPER_DELETE)   ) return; // �Ƕ��������Ҹýڵ������־ΪDELETE
	
	// ����ֵ
	_AddToByteArray(&pBA, (WORD)GetDataEntityManager().GetEntityPropertyMap().size()); // �����������
	CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().begin();
	for(; itr != GetDataEntityManager().GetEntityPropertyMap().end(); itr++)
	{
		long lDataType = GetGame()->GetEntityManager()->GetDataObjectType(GetCompositeFlag(), itr->first);
		_AddToByteArray(&pBA, (long)itr->first);//��������ö��ֵ��long��
		_AddToByteArray(&pBA, (long)itr->second->GetBufSize());//�������ݳ��ȣ�long��

		switch(lDataType)
		{
		case DATA_OBJECT_TIME:
			{
				// 6��longֵ : �� �� �� ʱ �� ��
				BYTE timeValue[6*sizeof(DWORD)];
				memset(timeValue, 0, 6*sizeof(DWORD));
				itr->second->GetBufAttr( timeValue, 6*sizeof(DWORD));
				_AddToByteArray(&pBA, timeValue, 6*sizeof(DWORD));
			}
			break;
		case DATA_OBJECT_STRING:
			{
				_AddToByteArray(&pBA, itr->second->GetStringAttr());//��������
			}
			break;
		case DATA_OBJECT_FLOAT:
		case DATA_OBJECT_LONG:
		case DATA_OBJECT_SHORT:
		case DATA_OBJECT_ULONG:
		case DATA_OBJECT_USHORT:
		case DATA_OBJECT_BOOL:
		case DATA_OBJECT_CHAR:
		case DATA_OBJECT_BYTE:
		case DATA_OBJECT_WORD:
		case DATA_OBJECT_DWORD:
			{
				_AddToByteArray(&pBA, (long)itr->second->GetLongAttr());//��������
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGUID;
				itr->second->GetGuidAttr( tGUID);
				_AddToByteArray(&pBA, tGUID);//��������
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				long bufSize = itr->second->GetBufSize();
				if(bufSize)
				{
					BYTE* buf = new BYTE[bufSize];
					itr->second->GetBufAttr(buf, bufSize);
					_AddToByteArray(&pBA, buf, bufSize);//��������
					SAFE_DELETE_ARRAY(buf);
				}
			}
			break;
		}

	}
}

#pragma warning(pop)
