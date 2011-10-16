#include "stdafx.h"
#include "LogManage.h"
#include "../LogServer/Game.h"



////////////////////////////////////////////////////////////////////////////////////////////////////
const char* szServerCfgFile = "setup/ServerCfg.ini";
const char* szTableCfgFile	= "setup/TableCfg.ini";
const DWORD SQL_SAVE_CACHE_SIZE	= 1024U * 512U;
const DWORD SQL_SEND_NUM		= 64U;
const DWORD WAIT_OUT_TIME		= 1000U * 30; //! ����

//public
////////////////////////////////////////////////////////////////////////////////////////////////////
LogManage::LogManage(HWND hwnd)
:m_hWnd(hwnd)
{
	LogItem tmpLogItem;
	tmpLogItem.m_conditionId	= -1;
	tmpLogItem.m_eType			= eDTC_INT32;

	char *szTmp = "client_no";
	memmove(tmpLogItem.m_szItemName, szTmp, strlen(szTmp) + 1);
	m_TableHead.push_back(tmpLogItem);
}

LogManage::~LogManage(VOID)
{
	Release();
}

BOOL LogManage::Init(VOID)
{
	if(!LoadServerConfig())
		return FALSE;
	if(!LoadTableConfig())
		return FALSE;
	if(!LoadControlConfig())
		return FALSE;
	if(!CreateCache())
		return FALSE;

	return TRUE;
}

VOID LogManage::Release(VOID)
{
	DbOperator::Release();
}

//! ��֤���������
BOOL LogManage::AffirmClientNo(LONG dClientNo, LONG lClientIp, LONG lSocket)
{
	for (DbInfoIte ite = m_vecDbInfo.begin(); ite != m_vecDbInfo.end(); ++ite)
	{
		if(ite->m_worldNo == dClientNo)
		{
			ite->m_lClientIp = lClientIp;
			m_mapClientInfo[lSocket] = pair<LONG, LONG>(dClientNo, lClientIp);

			char szClientInfo[128] = {0};
			UCHAR arrIp[4] = {0};
			memmove(arrIp, &lClientIp, sizeof(LONG));
			sprintf(szClientInfo, "Socket:%d, ID:%d, IP:%u.%u.%u.%u", lSocket, dClientNo, (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);
			AddClientList(szClientInfo);

			return TRUE;
		}
	}
	return FALSE;
}

pair<LONG, LONG> LogManage::GetNoAndIpBySockte(LONG lSocket)
{
	return m_mapClientInfo[lSocket];
}

VOID LogManage::ClientLost(LONG lSocket)
{
	map<LONG, pair<LONG, LONG>>::iterator ite = m_mapClientInfo.find(lSocket);
	if(m_mapClientInfo.end() != ite)
	{
		char szClientInfo[128] = {0};
		UCHAR arrIp[4] = {0};
		memmove(arrIp, &(ite->second.second), sizeof(LONG));
		sprintf(szClientInfo, "Socket:%d, ID:%d, IP:%u.%u.%u.%u", ite->first, ite->second.first, (ULONG)arrIp[0], (ULONG)arrIp[1], (ULONG)arrIp[2], (ULONG)arrIp[3]);
		DelClientList(szClientInfo);

		m_mapClientInfo.erase(lSocket);
	}
	
}

BOOL LogManage::AddLog(LONG lDbID, LPCSTR pSql)
{
	map<string, LONG> &TableWriteCount = m_WriteCount[lDbID];
	LONG &DbTableWriteCount = TableWriteCount["Not table type"];
	DbTableWriteCount += 1;

	return AddLog(lDbID, -1, pSql);
}

//! ���һ����־д�����
BOOL LogManage::AddLog(LONG lDbID, LONG lTableID, LPCSTR pSql)
{
	map<LONG, map<LONG, DbOperator*>>::iterator iteDB =	m_mapmapSqlStr.find(lDbID);
	if(m_mapmapSqlStr.end() == iteDB)
		return FALSE;

	map<LONG, DbOperator*>::iterator iteTable = iteDB->second.find(lTableID);
	if(iteDB->second.end() == iteTable)
		return FALSE;

	if(NULL == iteTable->second)
	{
		iteTable->second = DbOperator::CreateDbOperator(lDbID);
		if(NULL == iteTable->second)
			return FALSE;
	}

	DbOperator *pDbOperator = iteTable->second;
	pDbOperator->AddSql(pSql);
	if(SQL_SAVE_CACHE_SIZE < pDbOperator->GetSqlSize())
	{
		iteTable->second = DbOperator::CreateDbOperator(lDbID);
		pDbOperator->ExecuteSql();
	}
	return TRUE;
}

//! �ر�
VOID LogManage::Exit(VOID)
{
	map<LONG, map<LONG, DbOperator*>>::iterator iteDB = m_mapmapSqlStr.begin();
	for ( ; m_mapmapSqlStr.end() != iteDB; ++iteDB)
	{
		map<LONG, DbOperator*>::iterator iteTable = iteDB->second.begin();
		for ( ; iteDB->second.end() != iteTable; ++iteTable)
		{
			if(NULL != iteTable->second)
			{
				if(0 != iteTable->second->GetSqlSize())
				{
					iteTable->second->ExecuteSql();
				}
				else
				{
					DbOperator::ReleaseDbOperator(&(iteTable->second));
				}
				iteTable->second = NULL;
			}
		}
	}

	m_mapmapSqlStr.clear();
}
//! ��Ӧ��ʱ��д����־����С�ڻ�����ʱ���ɴ˴���д�룩
VOID LogManage::OnOutTime(VOID)
{
	static DWORD dwLastTime = timeGetTime();
	DWORD dwCurrTime = timeGetTime();
	if(WAIT_OUT_TIME < dwCurrTime - dwLastTime)
	{
		map<LONG, map<LONG, DbOperator*>>::iterator iteDB = m_mapmapSqlStr.begin();
		for ( ; m_mapmapSqlStr.end() != iteDB; ++iteDB)
		{
			map<LONG, DbOperator*>::iterator iteTable = iteDB->second.begin();
			for ( ; iteDB->second.end() != iteTable; ++iteTable)
			{
				if(NULL != iteTable->second)
				{
					if(0 != iteTable->second->GetSqlSize())
					{
						iteTable->second->ExecuteSql();
						iteTable->second = NULL;
					}
				}
			}
		}
	}
}


const	LogDbInfo*	LogManage::GetLogDbInfo(LONG lDbNo)
{
	for (LONG i = 0; i < m_vecDbInfo.size(); ++i)
	{
		if (m_vecDbInfo[i].m_worldNo == lDbNo)
		{
			return &m_vecDbInfo[i];
		}
	}

	return NULL;
}

BOOL LogManage::AddData(LONG lDbID, LONG lTableType, BYTE* pData)
{
	if (m_TableMap.end() == m_TableMap.find(lTableType))
	{
		return FALSE;
	}

	LogTable &logTable = m_TableMap[lTableType];

	map<string, LONG> &TableWriteCount = m_WriteCount[lDbID];
	LONG &DbTableWriteCount = TableWriteCount[string(logTable.m_szTableName)];
	DbTableWriteCount += 1;

	string strValueList;
	string strItemList;

	//! ����˱�ͷ
	strItemList = string(m_TableHead[0].m_szItemName)/* + string(",") + string(m_TableHead[1].m_szItemName)*/;

	const	LogDbInfo* pLogDbInfo = GetLogDbInfo(lDbID);
	assert(NULL != pLogDbInfo);
	char szDbID[32] = {0};
	sprintf(szDbID, "%d", lDbID);
	strValueList = string(szDbID);

	//! ����ͻ��˴�����������
	LONG lPos = 0;
	for (LONG i = 0; i < logTable.m_vecItem.size(); ++i)
	{
		strItemList += "," + string(logTable.m_vecItem[i].m_szItemName);
		string strItem;

		if(!GetValueByType_WithDB(pData, lPos, logTable.m_vecItem[i], strItem))
			return FALSE;
		strValueList += "," + strItem;
	}

	string strSql = "INSERT INTO " + string(logTable.m_szTableName) + " (" + strItemList + ") VALUES (" + strValueList + ");";

	AddLog(lDbID, lTableType, strSql.c_str());
	return TRUE;
}


//private
////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL LogManage::LoadServerConfig(VOID)
{
	// ��ȡȫ������
	ifstream stream;
	stream.open(szServerCfgFile);
	if( !stream.is_open() )
		return OutErrMsgBox("Setup/ServerCfg.ini�ļ���ʧ�ܣ�");

	string strTemp;
	stream >> strTemp; //�ļ�ע��

	LONG loop_count = 0;
	LogDbInfo tmpLogDB;
	do 
	{
		tmpLogDB.m_worldNo = 0;
		stream 
			>> tmpLogDB.m_worldNo
			>> tmpLogDB.m_szProvider
			>> tmpLogDB.m_szDbName 
			>> tmpLogDB.m_szDbIP 
			>> tmpLogDB.m_szUsername
			>> tmpLogDB.m_szPassword;
		
		if (0 == tmpLogDB.m_worldNo) 
			break;

		m_vecDbInfo.push_back(tmpLogDB);
		++loop_count;
	} 
	while(!stream.eof());

	AddDbInfoToOperator();
	return (0 < loop_count) ? TRUE : OutErrMsgBox("Setup/ServerCfg.ini�ļ���û��һ����ȷ�����ã�");
}


//------------------------------------------------------------------------------------------------
const char * arrSzTypeStr[] = 
{
	"INT32",
	"FLOAT",
	"DATE",
	"GUID",
	"STR128",
	"STR512"
};

eDataTypeCode GetDTC_byStr(const char* str)
{
	for (LONG i = 0; i < eDTC_END; ++i)
	{
		if (0 == _stricmp(str, arrSzTypeStr[i]))
		{
			return (eDataTypeCode)i;
		}
	}

	return eDTC_ErrorType;
}

BOOL LogManage::LoadTableConfig(VOID)
{
	////////////////////////////////////////
	TiXmlNode *pNode = NULL; 		

	TiXmlDocument* m_Tdoc = new TiXmlDocument("Setup/TableCfg.xml");
	if(!m_Tdoc->LoadFile())
	{
		return OutErrMsgBox("������ļ���");
	}

	pNode=m_Tdoc->FirstChild("DB_Table_list");

	if (pNode==NULL)
	{
		return OutErrMsgBox("�޽ڵ㣬�������");
	}

	TiXmlElement* pDB_Table_list=pNode->ToElement();//��ȡnode ��ָ��

	if (pDB_Table_list==NULL)
	{
		return OutErrMsgBox("��ȡnode ��ָ��ʧ��");
	}

	//! ��ȡXML��װ������
	TiXmlElement* obj_type = pDB_Table_list->FirstChildElement("obj_type");

	map<string, vector<LogItem>> tmpLogItemClass;//! ��ʱ�����ͱ�

	for (TiXmlElement* pType=obj_type->FirstChildElement("type"); pType!=NULL; pType=pType->NextSiblingElement())
	{
		string name = /*atoi*/(pType->Attribute("name"));
		vector<LogItem>	vecLogItem;
		
		for (TiXmlElement* pDB_Item=pType->FirstChildElement("DB_Item"); pDB_Item!=NULL; pDB_Item=pDB_Item->NextSiblingElement())
		{
			LogItem tmpLogItem;

			const char *pItem_name = pDB_Item->Attribute("Item_name");
			assert(128 > strlen(pItem_name));
			memmove(tmpLogItem.m_szItemName, pItem_name, strlen(pItem_name));

			if(eDTC_ErrorType == (tmpLogItem.m_eType = GetDTC_byStr(pDB_Item->Attribute("data_type"))))
			{
				return OutErrMsgBox("");
			}

			tmpLogItem.m_conditionId = -1;

			vecLogItem.push_back(tmpLogItem);
		}
		tmpLogItemClass[name] = vecLogItem;
	}

	//! ��ʼ��ȡ��־����Ϣ
	
	for (TiXmlElement* pTable = pDB_Table_list->FirstChildElement("Table"); pTable!=NULL; pTable=pTable->NextSiblingElement())
	{
		LogTable tmpLogTable;
		tmpLogTable.m_isLog = 1; //������ΪҪ��¼
		const char *pTableName = pTable->Attribute("name");
		assert(128 > strlen(pTableName));
		tmpLogTable.m_tableType = atoi(pTable->Attribute("log_no"));
		tmpLogTable.m_SendControl = atoi(pTable->Attribute("log_pos"));
		assert(0 < tmpLogTable.m_SendControl && eSC_INVALID > tmpLogTable.m_SendControl);
		memmove(tmpLogTable.m_szTableName, pTableName, strlen(pTableName));
		for (TiXmlElement* pItem=pTable->FirstChildElement("item"); pItem!=NULL; pItem=pItem->NextSiblingElement())
		{
			string strType = pItem->Attribute("type");
			if (0 == stricmp(strType.c_str(), "alone"))//û��Ԥ������ֶ�
			{
				LogItem logItem;
				const char *pItemName = pItem->Attribute("Item_name");
				assert(128 > strlen(pItemName));
				memmove(logItem.m_szItemName, pItemName, strlen(pItemName));
				if(eDTC_ErrorType == (logItem.m_eType = GetDTC_byStr(pItem->Attribute("data_type"))))
				{
					return OutErrMsgBox("�ֶ���������˵���Ƿ�");
				}
				tmpLogTable.m_vecItem.push_back(logItem);
			}
			else//��װ�õ��ֶμ�
			{
				vector<LogItem> vecObjItems = tmpLogItemClass[strType];
				string strHeadName = pItem->Attribute("head_name");
				assert(vecObjItems.size() > 0);
				for (LONG i = 0; i < vecObjItems.size(); ++i)
				{
					LogItem logItem;
					string strItemName = strHeadName + string(vecObjItems[i].m_szItemName);
					assert(128 > strItemName.size());
					memmove(logItem.m_szItemName, strItemName.c_str(), strItemName.size());
					
					logItem.m_eType = vecObjItems[i].m_eType;
					logItem.m_conditionId = -1;
					tmpLogTable.m_vecItem.push_back(logItem);
				}
			}
		}
		if(0 >= tmpLogTable.m_vecItem.size())
		{
			return OutErrMsgBox("������һ���յ���־��");
		}
		if (m_TableMap.end() != m_TableMap.find(tmpLogTable.m_tableType))
		{
			return OutErrMsgBox("��־�����ظ���");
		}
		m_TableMap[tmpLogTable.m_tableType] = tmpLogTable;
	}

	if (0 >= m_TableMap.size())
	{
		return OutErrMsgBox("û�ж�ȡ��һ����־��Ķ��壡");
	}
	////////////////////////////////////////

	delete m_Tdoc;
	m_Tdoc = NULL;
	return TRUE;
}


BOOL	LogManage::LoadControlConfig(VOID)
{
	TiXmlNode *pNode = NULL; 		

	TiXmlDocument* m_Tdoc = new TiXmlDocument("Setup/ControlCfg.xml");
	if(!m_Tdoc->LoadFile())
	{
		return OutErrMsgBox("������ļ���");
	}

	pNode=m_Tdoc->FirstChild("Control_Condition");

	if (pNode==NULL)
	{
		return OutErrMsgBox("�޽ڵ㣬�������");
	}

	TiXmlElement* pControl_Condition=pNode->ToElement();//��ȡnode ��ָ��

	if (pControl_Condition==NULL)
	{
		return OutErrMsgBox("��ȡnode ��ָ��ʧ��");
	}

	//! ��������
	m_vSendControl.clear();
	for (TableMap_ite ite = m_TableMap.begin(); ite != m_TableMap.end(); ++ite)
	{
		ite->second.m_isLog = 1;
	}

	//! ����û�����������
	TiXmlElement* pUse = pControl_Condition->FirstChildElement("Use");
	set<LONG> tmpSet_UseTemplate_index;
	{
		//! Ҫ�󲻼�¼������
		TiXmlElement* pNolog_TableType = pUse->FirstChildElement("Nolog_TableType");

		for(TiXmlElement* pTable = pNolog_TableType->FirstChildElement("Table"); 
			pTable != NULL; 
			pTable = pTable->NextSiblingElement())
		{
			LONG table_type = atoi(pTable->Attribute("table_type"));
			if (m_TableMap.end() == m_TableMap.find(table_type))
			{
				return OutErrMsgBox("�������������޴����ͱ��");
			}
			m_TableMap[table_type].m_isLog = 0;
			m_vSendControl.push_back(pair<LONG, LONG>(table_type, 0));
		}

		//! �ڲ���¼�������У�ɸѡ����Ҫ�ļ�¼������
		TiXmlElement* pException_Condition = pUse->FirstChildElement("Exception_Condition");
		
		for(TiXmlElement* pUseTemplate = pException_Condition->FirstChildElement("UseTemplate"); 
			pUseTemplate != NULL; 
			pUseTemplate = pUseTemplate->NextSiblingElement())
		{
			LONG template_index = atoi(pUseTemplate->Attribute("template_index"));
			tmpSet_UseTemplate_index.insert(template_index);
		}
	}

	//! ������õ�����ʱ���ϣ�tmpSet_UseTemplate_index������ȡ����ģ��
	TiXmlElement* pDefine_Template = pControl_Condition->FirstChildElement("Define_Template");

	for(TiXmlElement* pCondition_Template = pDefine_Template->FirstChildElement("Condition_Template"); 
		pCondition_Template != NULL; 
		pCondition_Template = pCondition_Template->NextSiblingElement("Condition_Template"))
	{
		LONG index = atoi(pCondition_Template->Attribute("index"));
		if (tmpSet_UseTemplate_index.end() == tmpSet_UseTemplate_index.find(index))
		{
			continue;
		}
		tmpSet_UseTemplate_index.erase(index);

		ItemCondition tmpItemCondition;
		map<LogTable*, LONG> tmpVec_holdTrueTable;
		//! ʹ�ø������ı���ֶ�
		for(TiXmlElement* pHold_true = pCondition_Template->FirstChildElement("hold_true"); 
			pHold_true != NULL; 
			pHold_true = pHold_true->NextSiblingElement("hold_true"))
		{
			LONG table_type=atoi(pHold_true->Attribute("table_type"));
			if (m_TableMap.end() == m_TableMap.find(table_type))
			{
				return OutErrMsgBox("�������������޴����ͱ��");
			}
			LogTable *pLogTable = &m_TableMap[table_type];

			LONG item_index=atoi(pHold_true->Attribute("item_index"));
			if (item_index >= pLogTable->m_vecItem.size())
			{
				return OutErrMsgBox("�������������ֶ����������ֶ�����");
			}
			tmpVec_holdTrueTable[pLogTable] = item_index;

			tmpItemCondition.m_vPosList.push_back(pair<LONG, LONG>(table_type, item_index));
			tmpItemCondition.m_valueType = pLogTable->m_vecItem[item_index].m_eType;
			pLogTable->m_vecItem[item_index].m_conditionId = m_vItemCondition.size();

		}

		//! ��ȡ������ϸ
		for(TiXmlElement* pCondition = pCondition_Template->FirstChildElement("Condition"); 
			pCondition != NULL;
			pCondition = pCondition->NextSiblingElement("Condition"))
		{
			ConditionTile tmpConditionTile;
			const char *pNexus		= pCondition->Attribute("nexus");
			const char *pECondition	= pCondition->Attribute("eCondition");
			const char *pValue		= pCondition->Attribute("value");
			
			//! ���������ϵ
			if (0 == strcmp(pNexus, "����"))
			{
				tmpConditionTile.m_eConditionUse = eCU_INTERSECTION;
			}
			else if (0 == strcmp(pNexus, "����"))
			{
				tmpConditionTile.m_eConditionUse = eCU_UNION;
			}
			else
			{
				return OutErrMsgBox("nexus�������󣬷Ƿ�����");
			}

			//! ��ñȽϰ취
			if (0 == strcmp(pECondition, "����"))
			{
				tmpConditionTile.m_eCondition = eC_Even;
			}
			else if (0 == strcmp(pECondition, "����"))
			{
				tmpConditionTile.m_eCondition = eC_More_Than;
			}
			else if (0 == strcmp(pECondition, "С��"))
			{
				tmpConditionTile.m_eCondition = eC_Less_Than;
			}
			else
			{
				return OutErrMsgBox("eCondition�������󣬷Ƿ�����");
			}

			//! ��ñȽ�ֵ
			switch(tmpItemCondition.m_valueType)
			{
			case eDTC_INT32:
				{
					LONG lValue = atoi(pValue);
					_AddToByteArray(&(tmpConditionTile.m_vData), lValue);
				}
				break;
			case eDTC_FLOAT:
				{
					float fValue = atof(pValue);
					_AddToByteArray(&(tmpConditionTile.m_vData), fValue);
				}
				break;
			case eDTC_GUID:
				{
					CGUID guid(pValue);
					_AddToByteArray(&(tmpConditionTile.m_vData), guid);
				}
				break;
			case eDTC_DATE:
			case eDTC_STR128:
			case eDTC_STR512:
				_AddToByteArray(&(tmpConditionTile.m_vData), pValue);
				break;
			default:
				break;
			}

			tmpItemCondition.m_vConditionTile.push_back(tmpConditionTile);
		}

		m_vItemCondition.push_back(tmpItemCondition);
	}

	if (0 != tmpSet_UseTemplate_index.size())
	{
		return OutErrMsgBox("<Exception_Condition>�ڵ���ʹ�����Ҳ�����������");
	}


	delete m_Tdoc;
	m_Tdoc = NULL;
	return TRUE;
}

//!		��������
BOOL	LogManage::CreateCache(VOID)
{
	for (LONG i = 0; i < m_vecDbInfo.size(); ++i)
	{
		m_mapmapSqlStr.insert(pair<LONG, map<LONG, DbOperator*>>(m_vecDbInfo[i].m_worldNo, map<LONG, DbOperator*>()));
		DbOperator *pDbOperator = DbOperator::CreateDbOperator(m_vecDbInfo[i].m_worldNo);
		if(NULL == pDbOperator)
			return FALSE;
		m_mapmapSqlStr[m_vecDbInfo[i].m_worldNo].insert(pair<LONG, DbOperator*>(-1, pDbOperator));
	}

	TableMap_ite ite = m_TableMap.begin();
	for (; m_TableMap.end() != ite; ++ite)
	{
		for (LONG i = 0; i < m_vecDbInfo.size(); ++i)
		{
			DbOperator *pDbOperator = DbOperator::CreateDbOperator(m_vecDbInfo[i].m_worldNo);
			if(NULL == pDbOperator)
				return FALSE;
			m_mapmapSqlStr[m_vecDbInfo[i].m_worldNo].insert(pair<LONG, DbOperator*>(ite->first, pDbOperator));
		}
	}

	return TRUE;
}

VOID	LogManage::TableAddToByteArray(vector<BYTE>& outVector)
{
	_AddToByteArray(&outVector, (LONG)m_TableMap.size());
	for (TableMap_ite ite = m_TableMap.begin(); ite != m_TableMap.end(); ++ite)
	{
		LogTable &logTable = ite->second;
		_AddToByteArray(&outVector, (VOID*)&logTable, LogTableBaseDataSize);
		_AddToByteArray(&outVector, (LONG)logTable.m_vecItem.size());
		for (ItemIte ite2 = logTable.m_vecItem.begin(); ite2 != logTable.m_vecItem.end(); ++ite2)
		{
			LogItem &logItem = *ite2;
			_AddToByteArray(&outVector, (VOID*)&logItem, sizeof(LogItem));
		}
	}
}

VOID	LogManage::ConditionAddToByteArray(vector<byte>& outVector)
{
	//! ���Ϳ���
	_AddToByteArray(&outVector, (LONG)m_vSendControl.size());
	for (LONG i = 0; i < m_vSendControl.size(); ++i)
	{
		_AddToByteArray(&outVector, m_vSendControl[i].first);
		_AddToByteArray(&outVector, m_vSendControl[i].second);
	}

	//! ɸѡ����
	_AddToByteArray(&outVector, (LONG)m_vItemCondition.size());
	for (LONG i = 0; i < m_vItemCondition.size(); ++i)
	{
		_AddToByteArray(&outVector, m_vItemCondition[i].m_valueType);
		_AddToByteArray(&outVector, (LONG)m_vItemCondition[i].m_vPosList.size());
		for (LONG n = 0; n < m_vItemCondition[i].m_vPosList.size(); ++n)
		{
			_AddToByteArray(&outVector, m_vItemCondition[i].m_vPosList[n].first);
			_AddToByteArray(&outVector, m_vItemCondition[i].m_vPosList[n].second);
		}
		_AddToByteArray(&outVector, (LONG)m_vItemCondition[i].m_vConditionTile.size());
		for (LONG j = 0; j < m_vItemCondition[i].m_vConditionTile.size(); ++j)
		{
			_AddToByteArray(&outVector, m_vItemCondition[i].m_vConditionTile[j].m_eCondition);
			_AddToByteArray(&outVector, m_vItemCondition[i].m_vConditionTile[j].m_eConditionUse);
			_AddToByteArray(&outVector, (LONG)m_vItemCondition[i].m_vConditionTile[j].m_vData.size());
			_AddToByteArray(&outVector,	(VOID*)(&(m_vItemCondition[i].m_vConditionTile[j].m_vData[0])), m_vItemCondition[i].m_vConditionTile[j].m_vData.size());
		}
	}
}

LONG	LogManage::AddDbInfoToOperator(VOID)
{
	LONG lSuccessfulNum = 0;
	for (DbInfoIte ite = m_vecDbInfo.begin(); ite != m_vecDbInfo.end(); ++ite)
	{
		LogDbInfo& info = *ite;

		string strErrLogFileName = string(info.m_szDbName) + string("Err.log");
		string strBusyLogFileName = string(info.m_szDbName) + string("Busy.log");
		string strDbConn =	"Provider=" + string(info.m_szProvider) +
							"; Server=" + string(info.m_szDbIP) + 
							"; Database=" + string(info.m_szDbName) + 
							"; UID=" + string(info.m_szUsername) + 
							"; PWD=" + string(info.m_szPassword) + 
							"; OLE DB Services=-1; Driver={SQL Server}";
		if(DbOperator::AddExecuteData(info.m_worldNo, strDbConn, strErrLogFileName, strBusyLogFileName))
			++lSuccessfulNum;
	}
	return lSuccessfulNum;
}


BOOL	LogManage::OutErrMsgBox(char*	pMsg)
{
	if (NULL != m_hWnd)
	{
		MessageBox(m_hWnd, pMsg, "error message", MB_OK);
	}
	return FALSE;
}

BOOL LogManage::GetValueByType_WithDB(BYTE *pData, LONG &lPos, const LogItem& logItem, string &strValue)
{
	char szTmp[1024] = {0};
	switch(logItem.m_eType)
	{
	case eDTC_INT32:
		{
			LONG lValue = _GetLongFromByteArray(pData, lPos);
			sprintf_s(szTmp, "%d", lValue);
		}
		break;
	case eDTC_FLOAT:
		{
			float fValue = _GetFloatFromByteArray(pData, lPos);
			sprintf_s(szTmp, "%f", fValue);
		}
		break;
	case eDTC_GUID:
		{
			CGUID guid;
			_GetBufferFromByteArray(pData, lPos, guid);
			char szGuid[128] = {0};
			guid.tostring(szGuid);
			sprintf_s(szTmp, "'%s'", szGuid);
		}
		break;
	case eDTC_DATE:
	case eDTC_STR128:
	case eDTC_STR512:
		{
			LONG lStrSize = _GetLongFromByteArray(pData, lPos);
#ifdef _DEBUG
			if(eDTC_STR128 == logItem.m_eType)
			{
				assert(128 > lStrSize);
			}
			else
			{
				assert(512 > lStrSize);
			}
#endif
			if(512 < lStrSize) return FALSE;
			char szStr[513] = {0};
			_GetBufferFromByteArray(pData, lPos, szStr, lStrSize);

			szTmp[0] = 'N';
			szTmp[1] = '\'';
			LONG lIndex = 2;

			LONG lSize = strlen(szStr);
			for (LONG i = 0; i < lSize; ++i)
			{
				if(0 == szStr[i])
					break;

				szTmp[lIndex] = szStr[i];
				++lIndex;

				if('\'' == szStr[i])
				{
					BOOL bInHz = FALSE;
					//! �ж��Ƿ��ٺ����м�
					if(127 < (UCHAR)szStr[i - 1])
					{
						LONG lNum = 0;
						for(LONG lTmpIdx = i - 1; 0 <= lTmpIdx; --lTmpIdx)
						{
							if(127 > (UCHAR)szStr[lTmpIdx])
								break;
							lNum++;
						}

						if(1 == (lNum % 2))
							bInHz = TRUE;
					}
					//! �ٵ����ź��������һ�������ű�ʾת��
					if(!bInHz)
					{
						szTmp[lIndex] = '\'';
						++lIndex;
					}
				}
			}
			szTmp[lIndex] = '\'';
		}
		break;
	default:
		return FALSE;
		break;
	}
	strValue.clear();
	strValue = string(szTmp);
	return TRUE;
}

bool IsCursorInHz(long cursorPos, string strText)
{
	if( cursorPos>0 && cursorPos<=(long)strText.length() && (unsigned char)strText[cursorPos-1]>127 )
	{
		int num=0;
		int i;
		for(i=cursorPos-1;i>=0;i--)
		{
			if( (unsigned char)strText[i]<127)
				break;
			num++;
		}
		// �����м�,���Ϊ���������ں����м�
		if( num % 2 == 1 )
			return true;
	}
	return false;
}

int FindSubString(string& str, string& strSub)
{
	int iPos = 0;
	int iSize = (int)str.size() - (int)strSub.size();
	while (iPos <= iSize)
	{
		if(!IsCursorInHz(iPos, str))
			if (strnicmp(str.c_str()+ iPos, strSub.c_str(), strSub.size()) == 0)
				return iPos;
		iPos ++;
	}

	return -1;
}

//! �ض���־������Ϣ
BOOL LogManage::ReLoadControlConfig(VOID)
{
	return LoadControlConfig();
}

VOID LogManage::CreateTableInDb(BOOL bOutToFile, string file_path)
{
	//! �����ж����
	string strExists_1		= "IF  NOT EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[";
	string strExists_3		= "]') AND type in (N'U'))\r\n";
	//! SQL����ͷ			= 1 + ���� + 3
	string strHead_1		= "BEGIN\r\n\r\nCREATE TABLE [";
	string strHead_3		= "] (\r\n";
	//! 32λ���������ֶ�	= 1 + �ֶ��� + 3
	string strInt32_1		= "\t[";
	string strInt32_3		= "] [int] NOT NULL ,\r\n";
	//! ���������ֶ�		= 1 + �ֶ��� + 3
	string strFloat_1		= "\t[";
	string strFloat_3		= "] [float] NOT NULL ,\r\n";
	//! ���������ֶ�		= 1 + �ֶ��� + 3
	string strDate_1		= "\t[";
	string strDate_3		= "] [datetime] NOT NULL ,\r\n";
	//!	GUID�����ֶ�		= 1 + �ֶ��� + 3
	string strGuid_1		= "\t[";
	string strGuid_3		= "] [uniqueidentifier] NOT NULL ,\r\n";
	//!	128λ�ַ��������ֶ� = 1 + �ֶ��� + 3
	string strStr128_1		= "\t[";
	string strStr128_3		= "] [nvarchar] (128) COLLATE Chinese_PRC_CI_AS NOT NULL,\r\n";
	//!	512λ�ַ��������ֶ� = 1 + �ֶ��� + 3
	string strStr512_1		= "\t[";
	string strStr512_3		= "] [nvarchar] (512) COLLATE Chinese_PRC_CI_AS NOT NULL,\r\n";
	//!	SQL����β
	string strFinality		= ") ON [PRIMARY];";

	//! �����ۼ�����					= 1 + tablename + 3 + tablename + 5 + itemname + 7;
	string strCreateCongregateIndex_1	= "\r\n\r\nCREATE  CLUSTERED  INDEX [IX_";
	string strCreateCongregateIndex_3	= "] ON [dbo].[";
	string strCreateCongregateIndex_5	= "]([";
	string strCreateCongregateIndex_7	= "]) ON [PRIMARY];\r\n\r\nEND\r\n\r\n";

	//! ����˳��ο�eDataTypeCode˳��
	string arrItemName[] = 
	{
		strInt32_1,		strInt32_3,
		strFloat_1,		strFloat_3,
		strDate_1,		strDate_3,
		strGuid_1,		strGuid_3,
		strStr128_1,	strStr128_3,
		strStr512_1,	strStr512_3
	};

	string strAllCrtSql;
	for (TableMap_ite ite = m_TableMap.begin(); ite != m_TableMap.end(); ++ite)
	{
		string strCreateTableSql;
		//! �����ж����
		strCreateTableSql += strExists_1 + string(ite->second.m_szTableName) + strExists_3;
		//! �������ͷ
		strCreateTableSql += strHead_1 + string(ite->second.m_szTableName) + strHead_3;

		//! ���������
		for (LONG i = 0; i < m_TableHead.size(); ++i)
		{
			strCreateTableSql += 
				arrItemName[2 * m_TableHead[i].m_eType] + 
				m_TableHead[i].m_szItemName +
				arrItemName[2 * m_TableHead[i].m_eType + 1];
		}
		for (LONG i = 0; i < ite->second.m_vecItem.size(); ++i)
		{
			strCreateTableSql += 
				arrItemName[2 * ite->second.m_vecItem[i].m_eType] + 
				ite->second.m_vecItem[i].m_szItemName +
				arrItemName[2 * ite->second.m_vecItem[i].m_eType + 1];
		}

		//! �������β
		strCreateTableSql += strFinality;

		//! ����ÿ�����е�ʱ���ֶεľۼ�����
		string strCreateCongregateIndex = 
			strCreateCongregateIndex_1 + 
			string(ite->second.m_szTableName) + 
			strCreateCongregateIndex_3 + 
			string(ite->second.m_szTableName) +
			strCreateCongregateIndex_5 +
			"_date" + 
			strCreateCongregateIndex_7;

		strCreateTableSql += strCreateCongregateIndex;

		if(bOutToFile)
			strAllCrtSql += strCreateTableSql;
		else
		{
			//! �����õ����ݿ���ִ��
			for (LONG i = 0; i < m_vecDbInfo.size(); ++i)
			{
				AddLog(m_vecDbInfo[i].m_worldNo, strCreateTableSql.c_str());
			}
		}
	}

	//! ִ�����
	if (bOutToFile)
	{
		//! ��ȡ��ǰʱ��
		SYSTEMTIME	stTime;
		char		szTime[128] = {0};
		GetLocalTime(&stTime);
		sprintf(szTime,"[%d-%d-%d %d-%d-%d]" ,
			stTime.wYear,
			stTime.wMonth,
			stTime.wDay,
			stTime.wHour,
			stTime.wMinute,
			stTime.wSecond);

		//! д���ļ�
		ofstream outfile;
		string strPathFile = file_path + string(szTime) + string("create_table.sql");
		outfile.open(strPathFile.c_str(), ios::trunc);

		if (!outfile.is_open())
			return;

		outfile << strAllCrtSql << endl;
	}
}

//! ������ݱ���Ϣ���ļ�
VOID LogManage::OutTableInfoToFile(VOID)
{
	//! ����˳��ο�eDataTypeCode˳��
	string arrItemType[] = 
	{
		"32λ����", //strInt32,
		"32λ������", //strFloat,
		"����ʱ��", //strDate,
		"GUID", //strGuid,
		"128λ�ַ���", //strStr128,
		"512λ�ַ���", //strStr512
	};

	//! �����ļ�����
	string LogInfoBody;

	for (TableMap_ite ite = m_TableMap.begin(); ite != m_TableMap.end(); ++ite)
	{
		//! ������
		LogInfoBody += string("������,") + string(ite->second.m_szTableName) + "\r";

		//! ���������
		for (LONG i = 0; i < m_TableHead.size(); ++i)
		{
			LogInfoBody += "," + string(m_TableHead[i].m_szItemName) + "," + arrItemType[m_TableHead[i].m_eType] + "\r";
		}
		for (LONG i = 0; i < ite->second.m_vecItem.size(); ++i)
		{
			LogInfoBody += "," + string(ite->second.m_vecItem[i].m_szItemName) + "," + arrItemType[ite->second.m_vecItem[i].m_eType] + "\r";
		}

		//! �������β
		LogInfoBody += "\r";
	}

	//! ������ļ�
	//! ��ȡ��ǰʱ��
	SYSTEMTIME	stTime;
	char		szTime[128] = {0};
	GetLocalTime(&stTime);
	sprintf(szTime,"[%d-%d-%d %d-%d-%d]" ,
		stTime.wYear,
		stTime.wMonth,
		stTime.wDay,
		stTime.wHour,
		stTime.wMinute,
		stTime.wSecond);

	//! д���ļ�
	ofstream outfile;
	string strPathFile = string(szTime) + string("LogInfo.csv");
	outfile.open(strPathFile.c_str(), ios::trunc);

	if (!outfile.is_open())
		return;

	outfile << LogInfoBody << endl;
}

