#include "StdAfx.h"
#include "DBLoginPlayer.h"
#include "DBPlayer.h"
#include "DBGood.h"
#include "..\..\..\dbaccess\worlddb\RSPlayer.h"
#include "DBEntityManager.h"
#include "../DBServer/game.h"
#include "DBAccessThread.h"
#include "DBSaveThread.h"
#include "DBLoadThread.h"
#include "ThreadDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBLoginPlayer::CDBLoginPlayer()
: CBaseDBEntity()
{
	m_LogoutTime = -1;
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
	// ��ʼ���ֶ�����
	InitProperty();
}

CDBLoginPlayer::CDBLoginPlayer(const CGUID& id)
: CBaseDBEntity(id)
{
	m_LogoutTime = -1;
	// ��ʼ���ֶ�����
	InitProperty();
}
CDBLoginPlayer::~CDBLoginPlayer()
{
}

// virtual interfaces
void CDBLoginPlayer::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_SAVE:
		{
			SaveThreadData* saveData = new SaveThreadData(GetExID(), DB_LOGINPLAYER);
			// ��Ҫ��¼һ��ʱ���,�ڴ����߳���
			Save(msgid, msgBuf, msgBufPos, bufSize);
			saveData->saveObj->objGUID = GetExID();
			saveData->saveObj->objType = DB_LOGINPLAYER;
			saveData->saveObj->objNum  = 1;
			*(CDBLoginPlayer*)(saveData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
		}
		break;
	case DB_OPER_LOAD:
		{
			if(b_IsFirstCreated)
			{
				LoadThreadData* loadData = new LoadThreadData(GetExID(), DB_LOGINPLAYER);
				loadData->loadObj->objGUID = GetExID();
				loadData->loadObj->objType = DB_LOGINPLAYER;
				loadData->loadObj->objNum  = 1;
				strcpy(loadData->loadObj->szCDKEY, GetAccount());
				*(CDBLoginPlayer*)(loadData->dbObj) = *this;
				GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
			}
			else //���Ǹմ����Ķ���,ֱ�ӷ����ڴ�����
			{
				// ��װ��Ϣ
				CMessage msg(MSG_DB2W_OPER_ENTITY);
				msg.Add((long)DB_LOGINPLAYER);
				msg.Add((BYTE)0);
				msg.Add((BYTE)DB_OPER_LOAD);
				msg.Add(msgid);
				msg.Add((WORD)1);
				vector<BYTE> pBA;
				AddToByteArray(pBA);
				msg.Add((long)pBA.size());
				msg.AddEx(&pBA[0], pBA.size());
				msg.SendToSocket(GetGame()->GetWSSocket());
			}
		}
		break;
	}
}

void CDBLoginPlayer::AddToByteArray(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, (WORD)GetDataEntityManager().GetEntityPropertyMap().size()); // �����������

	//--�¸�ʽ
	//�������ַ������ȣ�long��
	//�������ַ���
	//�������ͣ�long��
	//�������ݳ��ȣ�long��
	//��������
	CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().begin();
	for(; itr != GetDataEntityManager().GetEntityPropertyMap().end(); itr++)
	{
		_AddToByteArray(&pBA, (long)itr->first.size());//�������ַ������ȣ�long��
		_AddToByteArray(&pBA, itr->first.c_str());//�������ַ���
		_AddToByteArray(&pBA, (long)itr->second->GetDataType());//�������ͣ�long��
		_AddToByteArray(&pBA, (long)itr->second->GetBufSize(0));//�������ݳ��ȣ�long��
		switch(itr->second->GetDataType())
		{
		case DATA_OBJECT_TIME:
			{
				// 6��longֵ : �� �� �� ʱ �� ��
				BYTE timeValue[6*sizeof(DWORD)];
				memset(timeValue, 0, 6*sizeof(DWORD));
				itr->second->GetBufAttr(0, (void*)timeValue, 6*sizeof(DWORD));
				_AddToByteArray(&pBA, timeValue, 6*sizeof(DWORD));
			}
			break;
		case DATA_OBJECT_STRING:
			{
				_AddToByteArray(&pBA, itr->second->GetStringAttr(0));//��������
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
				_AddToByteArray(&pBA, (long)itr->second->GetLongAttr(0));//��������
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGUID;
				itr->second->GetGuidAttr(0, tGUID);
				_AddToByteArray(&pBA, tGUID);//��������
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				long bufSize = itr->second->GetBufSize(0);
				if(bufSize)
				{
					BYTE* buf = new BYTE[bufSize];
					itr->second->GetBufAttr(0, (void*)buf, bufSize);
					_AddToByteArray(&pBA, buf, bufSize);//��������
					SAFE_DELETE_ARRAY(buf);
				}
			}
			break;
		}

	}
}


void CDBLoginPlayer::Save(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	// ������Ϣ
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos, bufSize); // �����������
	char varName[1024];
	for(int i=0; i<(int)attrNum; i++)
	{
		//--�¸�ʽ
		//�������ַ������ȣ�long��
		//�������ַ���
		//�������ͣ�long��
		//�������ݳ��ȣ�long��
		//��������
		long varNameLen = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ַ������ȣ�long��
		_GetStringFromByteArray(msgBuf, msgBufPos, varName, bufSize);//�������ַ���
		m_mapDirtyAttrData.insert(string(varName));
		long bufType = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ͣ�long��
		long varBufLen = _GetLongFromByteArray(msgBuf, msgBufPos, bufSize);//�������ݳ��ȣ�long��

		CEntityProperty* ep = NULL;
		CDataEntityManager::EntityPropertyMapItr itr = GetDataEntityManager().GetEntityPropertyMap().find(varName);
		if(itr != GetDataEntityManager().GetEntityPropertyMap().end())
			ep = itr->second;
		else
		{
			CDataEntityManager* pdeManager = NULL;
			std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(string("loginplayer"));
			if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
				pdeManager = itr->second;

			if(pdeManager)
			{
				CEntityProperty* templateEP = pdeManager->GetEntityProperty(varName);
				if(templateEP)
					ep = GetDataEntityManager().CreateEntityProperty(varName, templateEP->GetDBTableName(), templateEP->GetDataType());
				else
					AddLogText("����������[%s]��������ָ��Ϊ��!", varName);
			}
		}

		assert(ep != NULL);

		switch((DATA_OBJECT_TYPE)bufType)
		{
		case DATA_OBJECT_TIME:
			{
				// 6��longֵ : �� �� �� ʱ �� ��
				BYTE timeValue[6*sizeof(DWORD)];
				memset(timeValue, 0, 6*sizeof(DWORD));
				_GetBufferFromByteArray(msgBuf, msgBufPos, timeValue, 6*sizeof(DWORD), bufSize);
				ep->SetBufAttr(0, timeValue, 6*sizeof(DWORD));
			}
			break;
		case DATA_OBJECT_STRING:
			{
				char varValue[1024];
				_GetStringFromByteArray(msgBuf, msgBufPos, varValue, bufSize);//��������
				ep->SetStringAttr(0, string((char*)varValue));
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
				ep->SetLongAttr(0, (long)value);
			}
			break;
		case DATA_OBJECT_GUID:
			{
				CGUID tGUID;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGUID, bufSize);//��������
				CGUID oldGuid;
				ep->GetGuidAttr(0, oldGuid);
				if(oldGuid != NULL_GUID && oldGuid != tGUID)
				{
					char szOldGuid[128];
					char szTGuid[128];
					oldGuid.tostring(szOldGuid);
					tGUID.tostring(szTGuid);
					AddLogText("Save LoginPlayer tGuid[%s],OldGuid[%s]!", szTGuid, szOldGuid);
				}
				ep->SetGuidAttr(0, tGUID);
			}
			break;
		case DATA_OBJECT_BUFFER:
			{
				BYTE* buf = new BYTE[varBufLen];
				_GetBufferFromByteArray(msgBuf, msgBufPos, buf, varBufLen, bufSize);//��������
				ep->SetBufAttr(0, buf, varBufLen);
				SAFE_DELETE_ARRAY(buf);
			}
			break;
		}
	}

	char* szCdkey = (char*)GetStringAttr(string("Account"), 0);
	if(szCdkey)
		m_strCdkey = szCdkey;
	else
	{
		char szGuid[128];
		GetExID().tostring(szGuid);
		AddLogText("LoginPlayer:[%s] CDKEYΪ��!", szGuid);
	}
}


// ��ʼ����������
void CDBLoginPlayer::InitProperty()
{
	CDataEntityManager* pdeManager = NULL;
	std::map<string, CDataEntityManager*>::iterator itr = GetGame()->GetDBEntityManager()->GetObjAttrDef().find(string("loginplayer"));
	if(itr != GetGame()->GetDBEntityManager()->GetObjAttrDef().end())
		pdeManager = itr->second;

	if(pdeManager)
	{
		CDataEntityManager::EntityPropertyMapItr attrItr = pdeManager->GetEntityPropertyMap().begin();

		for(; attrItr != pdeManager->GetEntityPropertyMap().end(); attrItr++)
		{
			CEntityProperty* ep = m_DataEntityManager.CreateEntityProperty(attrItr->first, attrItr->second->GetDBTableName(), attrItr->second->GetDataType());
			if(ep)
			{
				ep->SetDbFlag(attrItr->second->GetDbFlag());
				if(ep->GetDataType() == DATA_OBJECT_TIME)
				{
					long timeBuf[6] = {0};
					ep->SetBufAttr(0, (void*)&timeBuf[0], sizeof(DWORD)*6);
				}
			}
		}
	}
}

// �������������Ƿ���ָ��������������
bool CDBLoginPlayer::FindTableUpdateFlag(const string& tableName)
{
	set<string>::iterator itr = m_mapDirtyAttrData.begin();
	for (; itr != m_mapDirtyAttrData.end(); itr++)
	{
		CEntityProperty* ep = GetDataEntityManager().GetEntityProperty(*itr);
		if( ep )
		{
			if(ep->GetDBTableName() == tableName)
				return true;
		}
	}
	return false;
}