#include "StdAfx.h"
#include "DBLoginPlayer.h"
#include "DBPlayer.h"
#include "DBGood.h"
#include "DBEntityManager.h"
#include "../WorldServer/game.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBLoginPlayer::CDBLoginPlayer()
: CBaseDBEntity(),m_lMailInitLoadFlag(0)
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
	// ��ʼ���ֶ�����
	InitProperty();
}

CDBLoginPlayer::CDBLoginPlayer(const CGUID& id)
: CBaseDBEntity(id),m_lMailInitLoadFlag(0)
{
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
	case DB_OPER_LOAD:
		Load(msgid, msgBuf, msgBufPos, bufSize);
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

void CDBLoginPlayer::Load(long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
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

	// ����ɾ��ʱ��
	long delTime[6];
	GetTimeAttr(string("DelDate"), 0, delTime, sizeof(long)*6);

	if(delTime[0] == 0 && delTime[1] == 0 && delTime[2] == 0
		&& delTime[3] == 0 && delTime[4] == 0 && delTime[5] == 0)
	{
		m_DelTime = -1;
		return;
	}

	time_t tCurTime = time(0);
	tm *pCurTime = localtime(&tCurTime);

	tm tt;
	memset(&tt,0, sizeof(tm));
	tt.tm_year= delTime[0]-1900; tt.tm_mon = delTime[1]-1; tt.tm_mday = delTime[2];
	DWORD res = mktime(&tt);
	if(res == -1)
	{
		m_DelTime = -1;
		return;
	}

	double dDiffSecond = difftime(tCurTime, res);
	short days = -short(dDiffSecond/86400.0);
	m_DelTime = days+1;
	if(m_DelTime < 0) m_DelTime = 0; 
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
			CEntityProperty* pe = m_DataEntityManager.CreateEntityProperty(attrItr->first, attrItr->second->GetDBTableName(), attrItr->second->GetDataType());
			if(pe)
			{
				if(pe->GetDataType() == DATA_OBJECT_TIME)
				{
					long timeBuf[6] = {0};
					timeBuf[0] = 0;
					timeBuf[1] = 0;
					timeBuf[2] = 0;
					pe->SetBufAttr(0, (void*)&timeBuf[0], sizeof(DWORD)*6);
				}
			}
		}
	}
	else
	{
		AddLogText("CDBLoginPlayer::InitProperty() Err,δ�ҵ����������Ϣ!");
	}
}