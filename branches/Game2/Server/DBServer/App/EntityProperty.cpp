#include "stdafx.h"
#include "EntityProperty.h"
#include "..\..\..\nets\netdb\message.h"
#include "..\..\..\dbaccess\worlddb\DataEntityOperator.h"

CWrapDataObject::CWrapDataObject()
: m_ID(0),m_Name("WDO"),m_DataType(DATA_OBJECT_UNKNOWN)
{

}
CWrapDataObject::CWrapDataObject(DATA_OBJECT_TYPE type, long id, std::string& name)
: m_ID(id),m_Name(name),m_DataType(type)
{

}
CWrapDataObject::~CWrapDataObject()
{
	ValueInfoMapItr itr = m_mapDataValue.begin();
	for(; itr != m_mapDataValue.end(); itr++)
	{
		if(itr->second->buf)
		{
			delete[] itr->second->buf;
			itr->second->buf = NULL;
		}
	}
}

// ��־����
void CWrapDataObject::PushBackOperValuePosFlag(DB_OPERATION_TYPE op, WORD pos)
{
	switch(op)
	{
	case DB_OPER_LOAD:
		m_LoadValuePosFlag.insert(m_LoadValuePosFlag.end(), pos);
		break;
	case DB_OPER_SAVE:
		m_SaveValuePosFlag.insert(m_SaveValuePosFlag.end(), pos);
		break;
	}
}
WORD CWrapDataObject::PopFrontOperValuePosFlag(DB_OPERATION_TYPE op)
{
	WORD num = 0;
	switch(op)
	{
	case DB_OPER_LOAD:
		{
			num = *m_LoadValuePosFlag.begin();
			m_LoadValuePosFlag.erase(m_LoadValuePosFlag.begin());
		}
			break;
	case DB_OPER_SAVE:
		{
			num = *m_SaveValuePosFlag.begin();
			m_SaveValuePosFlag.erase(m_SaveValuePosFlag.begin());
		}
		break;
	}
	return num;
}
// ��ն���
void CWrapDataObject::ClearOperValuePosFlag(DB_OPERATION_TYPE op)
{
	switch(op)
	{
	case DB_OPER_LOAD:
		{
			m_LoadValuePosFlag.clear();
		}
		break;
	case DB_OPER_SAVE:
		{
			m_SaveValuePosFlag.clear();
		}
		break;
	}
}
// num
long CWrapDataObject::GetOperValuePosFlagNum(DB_OPERATION_TYPE op)
{
	long num = 0;
	switch(op)
	{
	case DB_OPER_LOAD:
		num = m_LoadValuePosFlag.size();
			break;
	case DB_OPER_SAVE:
		num = m_SaveValuePosFlag.size();
		break;
	}
	return num;
}
//	ADD------------------------------------------------------------
bool CWrapDataObject::AddValueArray(long pos, DATA_OBJECT_TYPE dataType, void* buf, long size)
{
	ValueInfo* vi = NULL;
	ValueInfoMapItr itr = m_mapDataValue.find(pos);
	if(itr != m_mapDataValue.end())
	{
		vi = itr->second;
	}
	else
	{
		vi = new ValueInfo;
		m_mapDataValue[pos] = vi;
	}

	vi->dataType = dataType;
	vi->pos = pos;
	vi->size = size;
	BYTE* tBuf = new BYTE[size];
	memcpy_s(tBuf, size, vi->buf, size);

	return true;
	/*switch(dataType)
	{
	case DATA_OBJECT_LONG:
		
		break;
	case DATA_OBJECT_SHORT:
		break;
	case DATA_OBJECT_ULONG:
		break;
	case DATA_OBJECT_USHORT:
		break;
	case DATA_OBJECT_BOOL:
		break;
	case DATA_OBJECT_CHAR:
		break;
	case DATA_OBJECT_BYTE:
		break;
	case DATA_OBJECT_WORD:
		break;
	case DATA_OBJECT_DWORD:
		break;
	case DATA_OBJECT_STRING:
		break;

	case DATA_OBJECT_LONG_ARRAY:
		break;
	case DATA_OBJECT_SHORT_ARRAY:
		break;
	case DATA_OBJECT_ULONG_ARRAY:
		break;
	case DATA_OBJECT_USHORT_ARRAY:
		break;
	case DATA_OBJECT_BOOL_ARRAY:
		break;
	case DATA_OBJECT_CHAR_ARRAY:
		break;
	case DATA_OBJECT_BYTE_ARRAY:
		break;
	case DATA_OBJECT_WORD_ARRAY:
		break;
	case DATA_OBJECT_DWORD_ARRAY:
		break;

	case DATA_OBJECT_STRING_ARRAY:
		break;
	case DATA_OBJECT_BUFFER_ARRAY:
		break;
	}*/
}

//	DEL------------------------------------------------------------
bool CWrapDataObject::DelValueArray(long pos)
{
	ValueInfo* vi = NULL;
	ValueInfoMapItr itr = m_mapDataValue.find(pos);
	if(itr != m_mapDataValue.end())
	{
		if(itr->second->buf)
		{
			delete[] itr->second->buf;
			itr->second->buf = NULL;
		}

		m_mapDataValue.erase(itr);

		return true;
	}

	return false;
}
//	GET------------------------------------------------------------
CWrapDataObject::ValueInfo* CWrapDataObject::GetValueArray(long pos)
{
	ValueInfo* vi = NULL;
	ValueInfoMapItr itr = m_mapDataValue.find(pos);
	if(itr != m_mapDataValue.end())
	{
		if(itr->second)
		{
			vi = itr->second;
		}
	}

	return vi;
}

// FIND------------------------------------------------------------
long CWrapDataObject::GetValueArray(CWrapDataObject::ValueInfo& valueInfo)
{
	long ret = INVILID_ENTITY_ID;
	ValueInfoMapItr itr = m_mapDataValue.find(valueInfo.pos);
	if(itr != m_mapDataValue.end())
	{
		if(*itr->second == valueInfo)
			ret = (*itr).second->pos;
	}

	return ret;
}
//////////////////////////////////////////////////////////////////////////
CEntityProperty::CEntityProperty(long id, std::string& name)
:m_ID(id), m_Name(name),m_CurDOID(0)
{

}
CEntityProperty::~CEntityProperty()
{
	WrapDataObjectMapItr itr = m_mapDataObject.begin();
	for(; itr != m_mapDataObject.end(); itr++)
	{
		if(itr->second)
			SAFE_DELETE(itr->second);
	}

	m_mapDataObject.clear();
}

// Add DataObject
bool CEntityProperty::AddDataObject(CWrapDataObject* dataObj)
{
	if(dataObj)
	{
		WrapDataObjectMapItr itr = m_mapDataObject.find(dataObj->GetName());
		if(itr == m_mapDataObject.end()) // δ�ҵ�
		{
			m_mapDataObject[dataObj->GetName()] = dataObj;
		}
		else
		{
			delete itr->second;
			itr->second = dataObj;
		}
		return true;
	}
	return false;
}
// Add DataObject
bool CEntityProperty::AddDataObject(DATA_OBJECT_TYPE type, std::string& objName)
{
	CWrapDataObject* dataObj = NULL;
	
	WrapDataObjectMapItr itr = m_mapDataObject.find(objName);
	if(itr == m_mapDataObject.end()) // δ�ҵ�
	{
		dataObj = new CWrapDataObject(type, AllocDataObjectID(), objName);
		m_mapDataObject[objName] = dataObj;

		return true;
	}

	return false;
}

// Get DataObject
CWrapDataObject* CEntityProperty::GetDataObject(std::string& objName)
{
	CWrapDataObject* dataObj = NULL;

	WrapDataObjectMapItr itr = m_mapDataObject.find(objName);
	if(itr != m_mapDataObject.end()) // δ�ҵ�
	{
		dataObj = itr->second;
	}

	return dataObj;
}

// Del DataObject
bool CEntityProperty::DelDataObject(std::string& objName)
{
	WrapDataObjectMapItr itr = m_mapDataObject.find(objName);
	if(itr != m_mapDataObject.end()) // δ�ҵ�
	{
		delete itr->second;
		m_mapDataObject.erase(itr);
	}

	return false;
}
// ����һ��ID
long CEntityProperty::AllocDataObjectID()
{
	if(m_FreeDOID.size())
	{
		long id = *(m_FreeDOID.begin());
		m_FreeDOID.erase(m_FreeDOID.begin());
		return id;
	}
	return ++m_CurDOID;
}
// �ͷ�һ��ID
void CEntityProperty::FreeDataObjectID(long id)
{
	std::list<long>::iterator itr = std::find(m_FreeDOID.begin(), m_FreeDOID.end(), id);
	// δ�ҵ���ID����IDС�����ֵ
	if( (itr == m_FreeDOID.end()) && (id < MAX_DEFINED_ID) ) 
		m_FreeDOID.push_back(id);
}
// ��־����
// �Ӷ��к����
void CEntityProperty::PushBackOperDataObjFlag(DB_OPERATION_TYPE op, string& name)
{
	switch(op)
	{
	case DB_OPER_LOAD:
		m_LoadDataObjFlag.insert(m_LoadDataObjFlag.end(), name);
		break;
	case DB_OPER_SAVE:
		m_SaveDataObjFlag.insert(m_SaveDataObjFlag.end(), name);
		break;
	}
}
// �Ӷ���ǰ����
string CEntityProperty::PopFrontOperDataObjFlag(DB_OPERATION_TYPE op)
{
	string name = "";

	switch(op)
	{
	case DB_OPER_LOAD:
		{
			name = *(m_LoadDataObjFlag.begin());
			m_LoadDataObjFlag.erase(m_LoadDataObjFlag.begin());
		}
		break;
	case DB_OPER_SAVE:
		{
			name = *(m_SaveDataObjFlag.begin());
			m_SaveDataObjFlag.erase(m_SaveDataObjFlag.begin());
		}
		break;
	}
	return name;
}
// num
long CEntityProperty::GetOperDataObjFlagNum(DB_OPERATION_TYPE op)
{
	long num = 0;
	switch(op)
	{
	case DB_OPER_LOAD:
		num = m_LoadDataObjFlag.size();
		break;
	case DB_OPER_SAVE:
		num = m_SaveDataObjFlag.size();
		break;
	}
	return num;
}
//////////////////////////////////////////////////////////////////////////
CDataEntityManager::CDataEntityManager()
: m_CurEPID(0)
{
	m_mapEntityProperty.clear();
}
CDataEntityManager::~CDataEntityManager()
{
	EntityPropertyMapItr itr = m_mapEntityProperty.begin();
	for(; itr != m_mapEntityProperty.end(); itr++)
	{
		if(itr->second)
			SAFE_DELETE(itr->second);
	}

	m_mapEntityProperty.clear();
}

// �������ݶ���
CEntityProperty* CDataEntityManager::CreateEntityProperty(std::string& name)
{
	CEntityProperty* ep = NULL;
	try
	{
		ep = new CEntityProperty(AllocEntityPropertyID(), name);
		return ep;
	}
	catch(...)
	{
		SAFE_DELETE(ep);
		AddErrorLogText("create EntityProperty Error!");
	}
}

// ɾ�����ݶ���
bool CDataEntityManager::DelEntityProperty(std::string& name)
{
	EntityPropertyMapItr itr = m_mapEntityProperty.find(name);
	if(itr != m_mapEntityProperty.end())
	{
		SAFE_DELETE(itr->second);
		m_mapEntityProperty.erase(itr);
		return true;
	}

	return false;
}

// ȡ�����ݶ���
CEntityProperty* CDataEntityManager::GetEntityProperty(std::string& name)
{
	CEntityProperty* ep = NULL;
	EntityPropertyMapItr itr = m_mapEntityProperty.find(name);
	if(itr != m_mapEntityProperty.end())
	{
		ep = itr->second;
	}

	return ep;
}

// ������Ϣ
void CDataEntityManager::ProcessMsg(CMessage* pMsg)
{
	// ȡ��Oper Entity Flag 0:1��, 1:���
	BYTE opEntityFlag = pMsg->GetByte();

	// DB��������
	DB_OPERATION_TYPE op = (DB_OPERATION_TYPE)pMsg->GetByte();

	// MSGID
	long msgid = pMsg->GetLong();

	// Entity Num
	long entityNum = pMsg->GetWord();

	// ��Ϣ���ݿ�
	BYTE msgBuf[102400];
	// pos
	long msgBufPos = 0;

	// ��ȡMsgBuf
	pMsg->GetEx(msgBuf, 102400);

	CMessage msg(MSG_DB2W_OPER_CARD);
	string tstr = "";

	switch(pMsg->GetType())
	{
		//////////////////////////////////////////////////////////////////////////
	case MSG_W2DB_OPER_CARD:
		{
			msg.SetType(MSG_DB2W_OPER_CARD);
			tstr = "Card";
		}
		break;
	case MSG_W2DB_OPER_CITY_WAR:
		{
			msg.SetType(MSG_W2DB_OPER_CITY_WAR);
			tstr = "CityWar";
		}
		break;
	case MSG_W2DB_OPER_COUNTRY:
		{
			msg.SetType(MSG_W2DB_OPER_COUNTRY);
			tstr = "Country";
		}
		break;

	case MSG_W2DB_OPER_ENEMY_FACTION:
		{
			msg.SetType(MSG_DB2W_OPER_ENEMY_FACTION);
			tstr = "EnemyFaction";
		}
		break;
	case MSG_W2DB_OPER_FACTION:
		{
			msg.SetType(MSG_DB2W_OPER_FACTION);
			tstr = "Faction";
		}
		break;
	case MSG_W2DB_OPER_GOOD:
		{
			msg.SetType(MSG_DB2W_OPER_GOOD);
			tstr = "Good";
		}
		break;
	case MSG_W2DB_OPER_PLAYER:
		{
			msg.SetType(MSG_DB2W_OPER_PLAYER);
			tstr = "Player";
		}
		break;
	case MSG_W2DB_OPER_REGION:
		{
			msg.SetType(MSG_DB2W_OPER_REGION);
			tstr = "Region";
		}
		break;
	case MSG_W2DB_OPER_SETUP:
		{
			msg.SetType(MSG_DB2W_OPER_SETUP);
			tstr = "Setup";
		}
		break;
	case MSG_W2DB_OPER_UNION:
		{
			msg.SetType(MSG_DB2W_OPER_UNION);
			tstr = "Union";
		}
		break;
	case MSG_W2DB_OPER_VILLAGEWAR:
		{
			msg.SetType(MSG_DB2W_OPER_VILLAGEWAR);
			tstr = "VillageWar";
		}
		break;
	}

	msg.Add((BYTE)opEntityFlag);// OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
	msg.Add((BYTE)op);		  // DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
	msg.Add(msgid);		  // MSG ID:        4byte, ����Ϣ��MSGID
	msg.Add((WORD)entityNum);	  // Entity Num:	2byte, ��ʵ�����(X)

	vector<BYTE> pBA;

	// ������
	CEntityProperty* ep = NULL;
	for(int i=0; i<entityNum; i++)
	{
		// EntityID
		long entityid = _GetLongFromByteArray(msgBuf, msgBufPos);

		_AddToByteArray(&pBA, entityid); // EntityID:		4byte, ʵ��ID
		EntityPropertyMapItr itr = m_mapEntityProperty.find(tstr);
		if(itr != m_mapEntityProperty.end())
		{
			ep = itr->second;
		}
		else
		{
			ep = new CEntityProperty(entityid);
			m_mapEntityProperty[tstr] = ep;	
		}

		ProcessMsg(ep, op, msgid, msgBuf, msgBufPos, pBA);
	}

	msg.AddEx(&pBA[0], pBA.size());
	msg.SendToSocket(pMsg->GetSocketID());
}
// ������Ϣ
void CDataEntityManager::ProcessMsg(CEntityProperty* ep, DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long msgBufPos, vector<BYTE>& pBA)
{
/*
|OPEntityFlag:  1byte, ����ʵ�������ı�־ 0: ����, 1:���
|DBOP Type:     1byte, ���ݿ�������ͣ�ö��ֵ��
|MSG ID:        4byte, ����Ϣ��MSGID
|Entity Num:	2byte, ��ʵ�����(X)
---------------�Ӵ˴���ʼʹ�� _AddToByteArray��_GetXXXXByArray �ӿڱ����----------------
|	|----
|   |EntityID:		4byte, ʵ��ID
|	|Attr Num:		2byte, �����������(N)
|	|	 |----
|	|	 |�����Ը���:		2byte, �������ݸ���(M)
|X*	|N * |����������:		4byte, ��������ö��ֵ
|	|	 |������ID:			4byte, ����ID
|	|	 |����������:		nbyte, ��������,�ַ���
|	|	 |		|----
|	|	 |		|������POS:		2byte, ����POS, 
|	|	 |	M *	|���������ݿ�:	nbtye, �����ݿ鳤�����������ݳ��Ⱦ��� (����ϢΪLOAD������ݿ�Ϊ1byte��ֵΪ0)
|	|	 |		|----
|	|	 |----
|	|----
*/
	WORD attrNum = _GetWordFromByteArray(msgBuf, msgBufPos); //Attr Num
	_AddToByteArray(&pBA, attrNum); // Attr Num:		2byte, �����������(N)

	char tableName[128];
	sprintf(tableName, "%s_Table", ep->GetName());

	char epDataName[128]; // ����������
	for(WORD j=0; j<attrNum; j++)
	{
		CWrapDataObject* obj = NULL;
		WORD epDataNum = _GetWordFromByteArray(msgBuf, msgBufPos); //�����Ը���

		DATA_OBJECT_TYPE type = (DATA_OBJECT_TYPE)_GetLongFromByteArray(msgBuf, msgBufPos); // ����������
		
		long epDataID = _GetLongFromByteArray(msgBuf, msgBufPos); // ������ID
	
		_GetStringFromByteArray(msgBuf, msgBufPos, epDataName); // ����������
		
		// ���Load����
		_AddToByteArray(&pBA, epDataNum);	// �����Ը���:		2byte, �������ݸ���(M)
		_AddToByteArray(&pBA, (long)type);		// ����������:		4byte, ��������ö��ֵ
		_AddToByteArray(&pBA, epDataID);	// ������ID:		4byte, ����ID
		_AddToByteArray(&pBA, epDataName);	// ����������:		nbyte, ��������,�ַ���

		string tname = epDataName;
		obj = ep->GetDataObject(tname);

		if(NULL == obj)
		{
			obj = new CWrapDataObject(type, ep->AllocDataObjectID(), tname);
			ep->AddDataObject(type, tname);
		}

		if(obj)
		{
			for(int j=0; j<epDataNum; j++)
			{
				WORD dataPos = _GetWordFromByteArray(msgBuf, msgBufPos); // ������POS:	2byte, ����POS
				WORD dataSize =  _GetWordFromByteArray(msgBuf, msgBufPos); // ������size:	2byte, ����SIZE, 

				if(dataPos != OPER_ALL_DATA)
					obj->PushBackOperValuePosFlag(op, dataPos);

				switch(op)
				{
				case DB_OPER_LOAD:
					{
						g_DataEntityOperator.LoadDataObject(string(tableName), ep->GetID(), obj, epDataNum, dataPos, dataSize);
					}
					break;
				case DB_OPER_SAVE:
					g_DataEntityOperator.SaveDataObject(string(tableName), ep->GetID(), obj, epDataNum, dataPos, dataSize);
					break;
				}

				// Add Msg
				AddMsg(ep, op, msgid, obj, pBA);
			}
		}
	}
}

// �����Ϣ
void CDataEntityManager::AddMsg(CMessage* msg)
{

}
// �����Ϣ
void CDataEntityManager::AddMsg(CEntityProperty* ep, DB_OPERATION_TYPE op, long msgid, CWrapDataObject* wdo, vector<BYTE>& pBA)
{
	/*
	|	|	 |		|----
	|	|	 |		|������POS:		2byte, ����POS, 
	|	|	 |		|������size:	2byte, ����SIZE, 
	|	|	 |	M *	|���������ݿ�:	nbtye, �����ݿ鳤�����������ݳ��Ⱦ��� (����ϢΪLOAD������ݿ�Ϊ1byte��ֵΪ0)
	|	|	 |		|----
	|	|	 |----
	|	|----
	*/
	WORD operValuePos = 0;
	switch(op)
	{
	case DB_OPER_LOAD:
		{
			if(wdo->GetOperValuePosFlagNum(DB_OPER_LOAD) != 0) // ����������ǵ�����
			{
				operValuePos = wdo->PopFrontOperValuePosFlag(DB_OPER_LOAD);
			}
			_AddToByteArray(&pBA, operValuePos); // ����ֵPOS
			CWrapDataObject::ValueInfo* vi = wdo->GetValueArray(operValuePos);
			if(vi && vi->buf)
			{	
				switch(wdo->GetDataType())
				{
				case DATA_OBJECT_LONG:
					_AddToByteArray(&pBA, (WORD)sizeof(long));
					_AddToByteArray(&pBA, *(long*)(vi->buf)); // ���������ݿ�:nbtye
					break;
				case DATA_OBJECT_BUFFER_ARRAY:
					_AddToByteArray(&pBA, (WORD)vi->size);
					_AddToByteArray(&pBA, vi->buf, vi->size); // ���������ݿ�:nbtye
					break;
				case DATA_OBJECT_CHAR_ARRAY:
					_AddToByteArray(&pBA, (WORD)vi->size);
					_AddToByteArray(&pBA, (char*)vi->buf); // ���������ݿ�:nbtye
					break;
				}
			}
		}
		break;
	case DB_OPER_SAVE:
		{
			if(wdo->GetOperValuePosFlagNum(DB_OPER_SAVE) != 0) // ����������ǵ�����
			{
				operValuePos = wdo->PopFrontOperValuePosFlag(DB_OPER_SAVE);
			}

			_AddToByteArray(&pBA, operValuePos); // ����ֵPOS
		}
		break;
	}
}
// ����һ��ID
long CDataEntityManager::AllocEntityPropertyID()
{
	if(m_FreeEPID.size())
	{
		long id = *(m_FreeEPID.begin());
		m_FreeEPID.erase(m_FreeEPID.begin());
		return id;
	}
	return ++m_CurEPID;
}
// �ͷ�һ��ID
void CDataEntityManager::FreeEntityPropertyID(long id)
{
	std::list<long>::iterator itr = std::find(m_FreeEPID.begin(), m_FreeEPID.end(), id);
	// δ�ҵ���ID����IDС�����ֵ
	if( (itr == m_FreeEPID.end()) && (id < MAX_DEFINED_ID) ) 
		m_FreeEPID.push_back(id);
}