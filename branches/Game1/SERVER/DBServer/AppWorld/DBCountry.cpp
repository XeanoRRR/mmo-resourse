#include "StdAfx.h"
#include "DBCountry.h"
#include "DBentityManager.h"
#include "..\..\..\dbaccess\worlddb\RSCountry.h"
#include "../DBServer/game.h"
#include "DBAccessThread.h"
#include "DBLoadThread.h"
#include "dbsavethread.h"
#include "ThreadDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDBCountry::CDBCountry()
: CBaseDBEntity(), m_lPower(0),m_lTechLevel(0),m_lKing(NULL_GUID)
{
	GetGame()->GetDBEntityManager()->GenerateGUID(m_GUID);
}

CDBCountry::CDBCountry(const CGUID& id)
: m_lPower(0),m_lTechLevel(0),m_lKing(NULL)
{						
	m_GUID = id;
}
CDBCountry::~CDBCountry()
{
}
void CDBCountry::AddEntityDataToMsg(vector<BYTE>& pBA)
{
	_AddToByteArray(&pBA, m_GUID); // ʵ��ID
	_AddToByteArray(&pBA, (WORD)4/*CDBCountry�����������������*/); // ���͵�CDBCountry�����������������

	// add one card data
	_AddToByteArray(&pBA, (WORD)1);
	_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_ID);
	_AddToByteArray(&pBA, GetID());

	_AddToByteArray(&pBA, (WORD)1);
	_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_POWER);
	_AddToByteArray(&pBA, GetPower());

	_AddToByteArray(&pBA, (WORD)1);
	_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_TECH_LEVEL);
	_AddToByteArray(&pBA, GetTechLevel());

	_AddToByteArray(&pBA, (WORD)1);
	_AddToByteArray(&pBA, (long)DB_COUNTRY_ATTR_LONG_KING);
	_AddToByteArray(&pBA, GetKing());	
}
// virtual interfaces
void CDBCountry::ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize)
{
	switch(op)
	{
	case DB_OPER_SAVE:
		{
			SaveThreadData* saveData = new SaveThreadData(GetExID(), DB_COUNTRY);
			Save(msgid, msgBuf, msgBufPos);
			saveData->saveObj->objGUID = GetExID();
			saveData->saveObj->objType = DB_COUNTRY;
			saveData->saveObj->objNum  = 1;
			*(CDBCountry*)(saveData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushSaveThreadData(saveData);
		}
		break;
	case DB_OPER_LOAD:
		{
			LoadThreadData* loadData = new LoadThreadData(GetExID(), DB_COUNTRY);
			loadData->loadObj->objGUID = GetExID();
			loadData->loadObj->objType = DB_COUNTRY;
			loadData->loadObj->objNum  = 1;
			*(CDBCountry*)(loadData->dbObj) = *this;
			GetGame()->GetDBEntityManager()->PushLoadThreadData(loadData);
		}
		break;
	}
}
void CDBCountry::Save(long msgid, BYTE* msgBuf, long& msgBufPos)
{
	// ������Ϣ
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �����������
	for(WORD i = 0; i < attrNum; i++)
	{
		WORD attrDataNum = _GetWordFromByteArray(msgBuf, msgBufPos); // �������ݸ���
		DB_COUNTRY_ATTR_TYPE type = (DB_COUNTRY_ATTR_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos);//��������ö��ֵ
		
		switch(type)
		{
		case DB_COUNTRY_ATTR_LONG_ID:
			{
				SetID(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_POWER:
			{
				SetPower(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_TECH_LEVEL:
			{
				SetTechLevel(_GetLongFromByteArray(msgBuf, msgBufPos));
			}
			break;
		case DB_COUNTRY_ATTR_LONG_KING:
			{
				CGUID tGuid;
				_GetBufferFromByteArray(msgBuf, msgBufPos, tGuid);
				SetKing(tGuid);
			}
			break;
		}
	}
}
