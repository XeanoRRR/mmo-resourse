/**
*File:  EntityGroup.cpp
*brief: ������:�ö��������Լ�����������,�������ӽڵ����
*Author:������
*Date:  2008.11.03
*/
#include "stdafx.h"
#include "EntityGroup.h"

#include "Entity.h"
#include "EntityManager.h"
#include "../Thread/DBThread.h"

#pragma warning(push)
#pragma warning(disable :  4244)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

CEntityGroup::CEntityGroup(const string& flag)
: CBaseEntity(),m_lLeafComType(0),m_lLeafComFlag(0),
m_strDetailComPositeFlagName(""),m_lDetailComPositeFlagType(0),
m_bHasDetailLeavesFlag(0),m_strProcedureName(""),m_DeleteLeafGuid(NULL_GUID),
m_bIsFindByName(false),m_strDelLeafComFlag("")
{
	
	memset(m_szName, 0, sizeof(m_szName));

	CBaseEntity::m_CompositeFlag = flag;
	InitProperty();

	map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.begin();
	for(; itr != m_mapEntityGroup.end(); itr++)
	{
		itr->second->InitProperty();
	}
}

CEntityGroup::CEntityGroup(const string& flag, const CGUID& id)
: CBaseEntity(id),m_lLeafComType(0),m_lLeafComFlag(""),
m_strDetailComPositeFlagName(""),m_lDetailComPositeFlagType(0),
m_bHasDetailLeavesFlag(0),m_strProcedureName(""),m_DeleteLeafGuid(NULL_GUID),
m_bIsFindByName(false),m_strDelLeafComFlag("")
{
	memset(m_szName, 0, sizeof(m_szName));

	CBaseEntity::m_CompositeFlag = flag;
	InitProperty();

	map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.begin();
	for(; itr != m_mapEntityGroup.end(); itr++)
	{
		itr->second->InitProperty();
	}
}
CEntityGroup::~CEntityGroup()
{
	// 1 ������е�Ҷ�ڵ����
	std::map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.begin();
	for(; itr != m_mapEntityGroup.end(); itr++)
	{
		SAFE_DELETE(itr->second);
	}
	m_mapEntityGroup.clear();

	vector<CTempEntityProperty*>::iterator retitr = m_pQueryDataMgr.begin();
	for(; retitr != m_pQueryDataMgr.end(); retitr++)
		delete (*retitr);
	m_pQueryDataMgr.clear();

	retitr = m_pRetQueryDataMgr.begin();
	for(; retitr != m_pRetQueryDataMgr.end(); retitr++)
		delete (*retitr);
	m_pRetQueryDataMgr.clear();
}
CEntityGroup& CEntityGroup::operator=(CEntityGroup& right)
{
	CBaseEntity::operator=(right);
	if(this != &right)
	{
		m_GUID = right.m_GUID;

		map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.begin();
		for(; itr != m_mapEntityGroup.end(); itr++)
		{
			SAFE_DELETE(itr->second);
		}
		m_mapEntityGroup.clear();

		itr = right.m_mapEntityGroup.begin();
		for(; itr != right.m_mapEntityGroup.end(); itr++)
		{
			CBaseEntity* tGoods = GetGame()->GetEntityManager()->NewBaseEntity(itr->second->GetCompositeFlag(), itr->first);
			if(itr->second->GetCurDbOperFlag())
			{
				if(tGoods->GetCompositeType() == COM_COMPOSITE)
					*((CEntityGroup*)tGoods) = *((CEntityGroup*)(itr->second));
				else
					*((CEntity*)tGoods) = *((CEntity*)(itr->second));
			}
			m_mapEntityGroup[itr->first] = tGoods;
		}
		m_mapGuidByComFlag = right.m_mapGuidByComFlag;
		m_lLeafComType = right.m_lLeafComType;
		m_lLeafComFlag = right.m_lLeafComFlag;
		// �丸�ڵ�����������ֵ����ݿ������
		m_strDetailComPositeFlagName = right.m_strDetailComPositeFlagName;
		m_lDetailComPositeFlagType = right.m_lDetailComPositeFlagType;
		m_bHasDetailLeavesFlag = right.m_bHasDetailLeavesFlag;

		memcpy(m_szName, right.m_szName, sizeof(m_szName));
		m_SessionID = right.m_SessionID;
		m_DeleteLeafGuid = right.m_DeleteLeafGuid;
		m_strDelLeafComFlag = right.m_strDelLeafComFlag;
		QueryDataRelease();
		vector<CTempEntityProperty*>::iterator epItr = right.m_pQueryDataMgr.begin();
		for(; epItr != right.m_pQueryDataMgr.end(); epItr++)
		{
			CTempEntityProperty* pEP = new CTempEntityProperty((*epItr)->GetVarName(), (*epItr)->GetDataType());
			*pEP = *(*epItr);
			m_pQueryDataMgr.push_back(pEP);
		}
		RetQueryDataRelease();
		epItr = right.m_pRetQueryDataMgr.begin();
		for(; epItr != right.m_pRetQueryDataMgr.end(); epItr++)
		{
			CTempEntityProperty* pEP = new CTempEntityProperty((*epItr)->GetVarName(), (*epItr)->GetDataType());
			*pEP = *(*epItr);
			m_pRetQueryDataMgr.push_back(pEP);
		}
		m_bIsFindByName = right.m_bIsFindByName;
		m_strProcedureName = right.m_strProcedureName;
	}
	return *this;
}
void CEntityGroup::SetName(const char* pName) 
{
	if (NULL != pName)
	{
		size_t lBufSize = sizeof(m_szName);
		size_t lSize = strlen(pName);
		lSize = (lBufSize >= lSize ) ? lSize : lBufSize;
		memmove(m_szName, pName, lSize);
		if(lSize == lBufSize)
			m_szName[lSize-1] = '\0';
	}
	else
	{
		Log4c::Warn(ROOT_MODULE,"DB_ENTITYGROUP_0");//SetName:����Ϊ�գ�
	}
}
void CEntityGroup::AddToByteArray(vector<BYTE>& pBA, bool isLoadOperFlag)
{	
	_AddToByteArray(&pBA, (BYTE)GetCompositeType());
	_AddToByteArray(&pBA, GetCompositeFlag().c_str());
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID

	_AddToByteArray(&pBA, (BYTE)GetCurDbOperFlag());
	_AddToByteArray(&pBA, (BYTE)GetCurDbOperType());

	if(   (GetCurDbOperFlag() == false && !isLoadOperFlag)  // ���Ƕ��������Ҳ�����־Ϊ0
		|| (isLoadOperFlag && GetCurDbOperType() == DB_OPER_DELETE)   ) return; // �Ƕ��������Ҹýڵ������־ΪDELETE

	// ����proc����ֵ
	_AddToByteArray(&pBA, (WORD)GetRetQueryDataVec().size());
	vector<CTempEntityProperty*>::iterator epItr = GetRetQueryDataVec().begin();
	for(; epItr != GetRetQueryDataVec().end(); epItr++)
	{	
		_AddToByteArray(&pBA, (*epItr)->GetVarName().c_str());
		_AddToByteArray(&pBA, (long)(*epItr)->GetDataType());//�������ͣ�long��

		switch((*epItr)->GetDataType())
		{
		case  DATA_OBJECT_STRING:
			{
				_AddToByteArray(&pBA, (*epItr)->GetStringAttr());
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGuid;
				(*epItr)->GetGuidAttr( tGuid);
				_AddToByteArray(&pBA, tGuid);
			}
			break;
		case DATA_OBJECT_BOOL:
		case DATA_OBJECT_CHAR:
		case DATA_OBJECT_BYTE:
		case DATA_OBJECT_SHORT:
		case DATA_OBJECT_WORD:
		case DATA_OBJECT_USHORT:
		case DATA_OBJECT_FLOAT:
		case DATA_OBJECT_LONG:
		case DATA_OBJECT_ULONG:
		case DATA_OBJECT_DWORD:
			{
				_AddToByteArray(&pBA, (long)(*epItr)->GetLongAttr());
			}
			break;
		}

		_AddToByteArray(&pBA, (BYTE)(*epItr)->GetDbRelateType());
	}

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

	//---------------------------------------
	// childNum:		2byte, �ӽڵ����(X)
	_AddToByteArray(&pBA, (WORD)m_mapEntityGroup.size());

//#ifdef _TESTRUNSTACKINFO_
	// ���Դ���
	//{
	//	if(GetCompositeFlag() == "[player]")
	//	{
	//		char szOutPutStr[1024] = {0};
	//		char szOutGuid[128];

	//		bool isBreak = false;
	//		map<CGUID, CBaseEntity*>::iterator tmpItr = GetEntityGroupMap().begin();
	//		for(; tmpItr != GetEntityGroupMap().end(); tmpItr++)
	//		{
	//			if(isBreak)
	//				break;

	//			if(tmpItr->second->GetCompositeFlag() == "[goodsgroup]")
	//			{
	//				map<CGUID, CBaseEntity*>::iterator leafItr = ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().begin();
	//				for(; leafItr != ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().end(); leafItr++)
	//				{
	//					if(leafItr->second->GetCompositeFlag() == "[gp_packet]")
	//					{
	//						if(((CEntityGroup*)leafItr->second)->GetEntityGroupMap().size() == 0)
	//						{
	//							GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " {Add() %s[%s] leaf num %d,type %d,flag%d,isTemp %d.", 
	//								GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								m_mapEntityGroup.size(), 
	//								GetCurDbOperType(), 
	//								GetCurDbOperFlag(),
	//								GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);

	//							tmpItr->second->GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " Add() %s[%s] leaf num %d,type %d,flag %d,isTemp %d.", 
	//								tmpItr->second->GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().size(),
	//								tmpItr->second->GetCurDbOperType(),
	//								tmpItr->second->GetCurDbOperFlag(),
	//								tmpItr->second->GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);
	//	
	//							leafItr->second->GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " Add() %s[%s] leaf num %d,type %d,flag %d,isTemp %d.}", 
	//								leafItr->second->GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								((CEntityGroup*)leafItr->second)->GetEntityGroupMap().size(),
	//								leafItr->second->GetCurDbOperType(), 
	//								leafItr->second->GetCurDbOperFlag(),
	//								leafItr->second->GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);
	//						}
	//						isBreak = true;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
//#endif

	map<CGUID, CBaseEntity*>::iterator entityItr = m_mapEntityGroup.begin();
	for(; entityItr != m_mapEntityGroup.end(); entityItr++)
	{
		entityItr->second->AddToByteArray(pBA, isLoadOperFlag);
	}
}


void CEntityGroup::GetFromByteArray(BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	QueryDataRelease();
	RetQueryDataRelease();

	// ��ѯ����
	long lQueryAttrNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize);
	for(int j=0; j<lQueryAttrNum; j++)
	{	
		char szQueryStrName[128];
		_GetStringFromByteArray(msgBuf, msgBufPos, szQueryStrName, bufSize);
		DATA_OBJECT_TYPE objType = (DATA_OBJECT_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ͣ�long��
		long lQueryBufSize = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);
		CTempEntityProperty* pEP = new CTempEntityProperty(szQueryStrName, objType);
		if(pEP)
		{
			switch(objType)
			{
			case  DATA_OBJECT_BUFFER:
				{
					if(lQueryBufSize>0)
					{
						BYTE* queryBuf = new BYTE[lQueryBufSize];
						_GetBufferFromByteArray(msgBuf, msgBufPos, queryBuf, lQueryBufSize, bufSize);
						pEP->SetBufAttr(queryBuf, lQueryBufSize);
						SAFE_DELETE_ARRAY(queryBuf);
					}
				}
				break;
			case  DATA_OBJECT_STRING:
				{
					char szValue[128];
					memset(szValue, 0, sizeof(szValue));
					_GetStringFromByteArray(msgBuf, msgBufPos, szValue, bufSize);
					pEP->SetStringAttr(string(szValue));
				}
				break;
			case DATA_OBJECT_GUID:
				{
					CGUID tGuid;
					_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid, bufSize);
					pEP->SetGuidAttr(tGuid);
				}
				break;
			case DATA_OBJECT_BOOL:
			case DATA_OBJECT_CHAR:
			case DATA_OBJECT_BYTE:
			case DATA_OBJECT_SHORT:
			case DATA_OBJECT_WORD:
			case DATA_OBJECT_USHORT:
			case DATA_OBJECT_FLOAT:
			case DATA_OBJECT_LONG:
			case DATA_OBJECT_ULONG:
			case DATA_OBJECT_DWORD:
				{
					pEP->SetLongAttr(_GetLongFromByteArray(msgBuf, msgBufPos, bufSize));
				}
				break;
			}

			DB_RELATE_TYPE relateType = (DB_RELATE_TYPE)_GetByteFromByteArray(msgBuf, msgBufPos, bufSize);
			pEP->SetDbRelateType(relateType);
			m_pQueryDataMgr.push_back(pEP);
		}
	}

	// ��ѯ���صı���
	lQueryAttrNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize);
	for(int j=0; j<lQueryAttrNum; j++)
	{	
		char szQueryStrName[128];
		_GetStringFromByteArray(msgBuf, msgBufPos, szQueryStrName, bufSize);
		DATA_OBJECT_TYPE objType = (DATA_OBJECT_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ͣ�long��
		CTempEntityProperty* pEP = new CTempEntityProperty(szQueryStrName, objType);
		if(pEP)
		{
			switch(objType)
			{
			case  DATA_OBJECT_STRING:
				{
					char szValue[128];
					memset(szValue, 0, sizeof(szValue));
					_GetStringFromByteArray(msgBuf, msgBufPos, szValue, bufSize);
					pEP->SetStringAttr(string(szValue));
				}
				break;
			case DATA_OBJECT_GUID:
				{
					CGUID tGuid;
					_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid, bufSize);
					pEP->SetGuidAttr(tGuid);
				}
				break;
			case DATA_OBJECT_BOOL:
			case DATA_OBJECT_CHAR:
			case DATA_OBJECT_BYTE:
			case DATA_OBJECT_SHORT:
			case DATA_OBJECT_WORD:
			case DATA_OBJECT_USHORT:
			case DATA_OBJECT_FLOAT:
			case DATA_OBJECT_LONG:
			case DATA_OBJECT_ULONG:
			case DATA_OBJECT_DWORD:
				{
					pEP->SetLongAttr(_GetLongFromByteArray(msgBuf, msgBufPos, bufSize));
				}
				break;
			}

			DB_RELATE_TYPE relateType = (DB_RELATE_TYPE)_GetByteFromByteArray(msgBuf, msgBufPos, bufSize);
			pEP->SetDbRelateType(relateType);
			m_pRetQueryDataMgr.push_back(pEP);
		}
	}
	

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

	//////////////////////////////////////////////////////////////////////////
	// �Ƿ������ݱ�־λ
	bool isLoadFlag = false;
	WORD num = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize);
	if(num)
	{
		for(int j=0; j<num; j++)
		{
			BYTE entityType = _GetByteFromByteArray(msgBuf, msgBufPos, bufSize);

			char comFlag[64];
			memset(comFlag, 0, sizeof(comFlag));
			_GetStringFromByteArray(msgBuf, msgBufPos, comFlag, bufSize);
			CGUID guid;
			_GetBufferFromByteArray(msgBuf, msgBufPos, guid, bufSize);

			BYTE DbOperFlag = _GetByteFromByteArray(msgBuf, msgBufPos, bufSize);
			BYTE DBOperType = _GetByteFromByteArray(msgBuf, msgBufPos, bufSize);
			if(DBOperType == DB_OPER_LOAD)
				isLoadFlag = true;

			if(DbOperFlag != 1) continue;
			switch(DBOperType)
			{
			case DB_OPER_DELETE_INSERT:
			case DB_OPER_ONLY_MEMSAVE:
				{
					CBaseEntity* pEntity = GetChild(guid);
					if(pEntity) 
					{
						if(pEntity->GetCompositeType() == COM_COMPOSITE)
						{
							if(!((CEntityGroup*)pEntity)->GetHasDetailLeavesFlag())// �Ƿֲ�group(GoodsGroup)
							{
								ReleaseLeaf(guid);
								pEntity = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
								AddChild(pEntity);
							}
						}
					}
					else
					{
						pEntity = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
						AddChild(pEntity);
					}

					if(pEntity)
					{
						pEntity->SetCurDbOperType(DBOperType);
						pEntity->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
						{
							pEntity->GetFromByteArray(msgBuf, msgBufPos, bufSize);
						}
					}						
				}
				break;
			case DB_OPER_DELETE:
				{
					CBaseEntity* pEntity = GetChild(guid);
					if(!pEntity)
					{
						pEntity = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
						AddChild(pEntity);
					}
					if(pEntity)
					{
						pEntity->SetCurDbOperFlag(DbOperFlag);
						pEntity->SetCurDbOperType(DBOperType);
						if(DbOperFlag == 1)
							pEntity->GetFromByteArray(msgBuf, msgBufPos, bufSize);
					}
				}
				break;
			case DB_OPER_INSERT:
				{
					CBaseEntity* pEntity = GetChild(guid);
					if(!pEntity)
					{
						pEntity = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
						AddChild(pEntity);
					}
					if(pEntity)
					{
						pEntity->SetCurDbOperType(DBOperType);
						pEntity->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pEntity->GetFromByteArray(msgBuf, msgBufPos, bufSize);
					}						
				}
				break;
			case DB_OPER_SAVE:
				{
					CBaseEntity* pLeaf = GetChild(guid);
					if(!pLeaf)
					{
						pLeaf = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
						AddChild(pLeaf);
					}
					if(pLeaf)
					{
						pLeaf->SetCurDbOperType(DBOperType);
						pLeaf->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pLeaf->GetFromByteArray(msgBuf, msgBufPos, bufSize);
					}
				}
				break;
			case DB_OPER_LOAD:
				{
					CBaseEntity* pLeaf = GetChild(guid);
					if(!pLeaf)
					{
						pLeaf = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
						AddChild(pLeaf);
					}
					if(pLeaf)
					{
						pLeaf->SetCurDbOperType(DBOperType);
						pLeaf->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pLeaf->GetFromByteArray(msgBuf, msgBufPos, bufSize);
					}
				}
				break;
			default:
				{	
					CBaseEntity* pEntity = GetGame()->GetEntityManager()->NewBaseEntity(comFlag, guid);
					if(pEntity)
					{
						pEntity->SetCurDbOperType(DBOperType);
						pEntity->SetCurDbOperFlag(DbOperFlag);
						if(DbOperFlag == 1)
							pEntity->GetFromByteArray(msgBuf, msgBufPos, bufSize);
					}
					SAFE_DELETE(pEntity);
					Log4c::Warn(ROOT_MODULE,"DB_ENTITYGROUP_1", GetCompositeFlag().c_str(), comFlag, DBOperType);//GetFromByteArray(): ���ڵ�%s �ӽ��%s �Ĳ�������%d ��Ч!
				}
				break;
			}
		}
	}	

	// ���Դ���
//#ifdef _TESTRUNSTACKINFO_
	//{
	//	if(GetCompositeFlag() == "[player]")
	//	{
	//		char szOutPutStr[1024] = {0};
	//		char szOutGuid[128];

	//		bool isBreak = false;
	//		map<CGUID, CBaseEntity*>::iterator tmpItr = GetEntityGroupMap().begin();
	//		for(; tmpItr != GetEntityGroupMap().end(); tmpItr++)
	//		{
	//			if(isBreak)
	//				break;

	//			if(tmpItr->second->GetCompositeFlag() == "[goodsgroup]")
	//			{
	//				map<CGUID, CBaseEntity*>::iterator leafItr = ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().begin();
	//				for(; leafItr != ((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().end(); leafItr++)
	//				{
	//					if(leafItr->second->GetCompositeFlag() == "[gp_packet]")
	//					{
	//						if(((CEntityGroup*)leafItr->second)->GetEntityGroupMap().size() == 0 && !isLoadFlag )
	//						{
	//							GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " {Get() %s[%s] leaf num %d,type %d,flag%d,isTemp %d.", 
	//								GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								m_mapEntityGroup.size(), 
	//								GetCurDbOperType(), 
	//								GetCurDbOperFlag(),
	//								GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);

	//							tmpItr->second->GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " Get() %s[%s] leaf num %d,type %d,flag %d,isTemp %d.", 
	//								tmpItr->second->GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								((CEntityGroup*)tmpItr->second)->GetEntityGroupMap().size(),
	//								tmpItr->second->GetCurDbOperType(),
	//								tmpItr->second->GetCurDbOperFlag(),
	//								tmpItr->second->GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);

	//							leafItr->second->GetGUID().tostring(szOutGuid);
	//							sprintf(szOutPutStr, " Get() %s[%s] leaf num %d,type %d,flag %d,isTemp %d.}", 
	//								leafItr->second->GetCompositeFlag().c_str(), 
	//								szOutGuid, 
	//								((CEntityGroup*)leafItr->second)->GetEntityGroupMap().size(),
	//								leafItr->second->GetCurDbOperType(), 
	//								leafItr->second->GetCurDbOperFlag(),
	//								leafItr->second->GetIsTempEntityFlag());
	//							PutStringToFile("db-goods", szOutPutStr);
	//						}
	//						isBreak = true;
	//						break;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
//#endif
}

void CEntityGroup::ReleaseLeaf(const CGUID& guid)
{
	std::map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(guid);
	if( itr != m_mapEntityGroup.end() )
	{
		delete itr->second;
		itr = m_mapEntityGroup.erase(itr);
	}
}
// ת��ĳҶ�ӵ�ָ��
CBaseEntity* CEntityGroup::ReleaseChildPtr(const CGUID& guid)
{
	CBaseEntity* retPtr = NULL;
	std::map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(guid);
	if( itr != m_mapEntityGroup.end() )
	{
		retPtr = itr->second;
		itr = m_mapEntityGroup.erase(itr);
	}
	return retPtr;
}
// ���ĳ��Ҷ�ӵ�Ҷ��,������Ҷ��
void CEntityGroup::ClearLeafChilds(const CGUID& guid)
{
	std::map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(guid);
	if( itr != m_mapEntityGroup.end() )
	{
		if(itr->second->GetCompositeType() == COM_COMPOSITE)
			((CEntityGroup*)itr->second)->ReleaseChilds();
	}
}

void CEntityGroup::ReleaseChilds()
{
	std::map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.begin();
	while( itr != m_mapEntityGroup.end() )
	{
		delete itr->second;
		itr = m_mapEntityGroup.erase(itr);
	}
	m_mapEntityGroup.clear();
}


// ��ʼ����������
void CEntityGroup::InitProperty()
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
		m_lLeafComFlag = pInfo->strLeafComFlag;
		m_lLeafComType = pInfo->lLeafComType;
		m_bHasDetailLeavesFlag = pInfo->lHasDetailLeaves;

		m_CompositeType = (COMPOSITE_TYPE)pInfo->lComType;
		m_bHasAttrs = pInfo->lHhasAttrFlag;

		m_strDetailComPositeFlagName = pInfo->strDbQueryName;
		m_lDetailComPositeFlagType = pInfo->lDbQueryType;

		if(m_lDetailComPositeFlagType != -1) // �з�ɢ����
		{
			long lQueryAttrType = GetGame()->GetEntityManager()->GetAttrEnumByStr(pInfo->strComFlag, pInfo->strDbQueryName);

			CEntityProperty* pEP = m_DataEntityManager.CreateEntityProperty(lQueryAttrType);
			if(pEP)
			{
				switch((DATA_OBJECT_TYPE)pInfo->lDbQueryType)
				{
				case DATA_OBJECT_TIME:
					{
						pEP->SetBufAttr(((void*)&pInfo->pQueryVar[0]), 6*sizeof(DWORD));
					}
					break;
				case  DATA_OBJECT_STRING:
					{
						pEP->SetStringAttr(string((char*)&pInfo->pQueryVar[0]));
					}
					break;
				case DATA_OBJECT_BUFFER:
					{
						pEP->SetBufAttr(((void*)&pInfo->pQueryVar[0]), 6*sizeof(DWORD));
					}
					break;
				case DATA_OBJECT_GUID:
					{
						CGUID bguid;
						memcpy(&bguid, pInfo->pQueryVar, sizeof(CGUID));
						pEP->SetGuidAttr(bguid);
					}
					break;
				case DATA_OBJECT_BOOL:
				case DATA_OBJECT_CHAR:
				case DATA_OBJECT_BYTE:
				case DATA_OBJECT_SHORT:
				case DATA_OBJECT_WORD:
				case DATA_OBJECT_USHORT:
				case DATA_OBJECT_FLOAT:
				case DATA_OBJECT_LONG:
				case DATA_OBJECT_ULONG:
				case DATA_OBJECT_DWORD:
					{
						long lVar = *((long*)&pInfo->pQueryVar[0]);
						pEP->SetLongAttr(lVar);
					}
					break;
				}
			}
		}
		// ��ʼ���ṹ
		map<string, tagBuildFileInfo*>::iterator leafItr = pInfo->mapBuildFileInfo.begin();
		for(; leafItr != pInfo->mapBuildFileInfo.end(); leafItr++)
		{
			tagBuildFileInfo* leafInfo = leafItr->second;
			CEntityGroup* pLeaf = new CEntityGroup(leafInfo->strLeafComFlag, leafInfo->leafGuid);
			if(pLeaf)
			{
				AddChild(pLeaf);
				pLeaf->InitProperty();
			}
		}
	}
}

void CEntityGroup::AddChild(CBaseEntity* entity)
{
	if(entity)
	{
		map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(entity->GetGUID());
		if(itr != m_mapEntityGroup.end())
		{
			delete itr->second;
			m_mapEntityGroup.erase(itr);
		}
		m_mapEntityGroup[entity->GetGUID()] = entity;
		m_mapGuidByComFlag[entity->GetCompositeFlag()] = entity->GetGUID();
	}
}
void CEntityGroup::RemoveChild(const CGUID& guid)
{
	map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(guid);
	if(itr != m_mapEntityGroup.end())
	{
		delete itr->second;
		itr = m_mapEntityGroup.erase(itr);
	}
}
CBaseEntity* CEntityGroup::GetChild(const CGUID& guid)
{
	CBaseEntity* retEntity = NULL;
	map<CGUID, CBaseEntity*>::iterator itr = m_mapEntityGroup.find(guid);
	if(itr != m_mapEntityGroup.end())
	{
		retEntity = itr->second;
	}
	return retEntity;
}

// ��������������ָ���ڵ�
CBaseEntity* CEntityGroup::FindEntityBytravelTree(const CGUID& desGuid)
{
	std::map<CGUID, CBaseEntity*>::iterator itr = GetEntityGroupMap().find(desGuid);
	if(itr != GetEntityGroupMap().end())
	{
		return itr->second;
	}
	else
	{
		for(itr = GetEntityGroupMap().begin(); itr != GetEntityGroupMap().end(); itr++)
		{
			if(itr->second->GetCompositeType() == COM_COMPOSITE)
			{
				CBaseEntity* pRet = ((CEntityGroup*)itr->second)->FindEntityBytravelTree(desGuid);
				if(pRet)
					return pRet;
			}
		}
	}
	return NULL;
}

#pragma warning(pop)
