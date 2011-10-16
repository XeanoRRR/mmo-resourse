



#pragma once



//! --------------------------------------------------------------------------------------
//! �������ʹ���
enum eDataTypeCode
{
	eDTC_ErrorType	= -1,		//! ����
	eDTC_INT32		= 0,		//! 32λ����
	eDTC_FLOAT,					//! ����
	eDTC_DATE,					//! ʱ��
	eDTC_GUID,					//! 128λGUID
	eDTC_STR128,				//!	128λ�ַ���
	eDTC_STR512,				//!	512λ�ַ���

	eDTC_END,
};


//! ��־�ı���ģʽ
enum eSendControl
{
	eSC_LOGSERVER		= 1,			//! ���͵�LogServer
	eSC_HERE			= 2,			//! ��¼�ڱ���
	sSC_SERVER_AND_HERE = 3,			//! ���غ�serverͬʱ��¼

	eSC_INVALID,
};

enum eCondition
{
	eC_Even,					//! ����
	eC_More_Than,				//! ����
	eC_Less_Than,				//! С��
};

enum eConditionUse
{
	eCU_INTERSECTION,			//! ����
	eCU_UNION,					//! ����
};

//! --------------------------------------------------------------------------------------
//! ����
struct ConditionTile
{
	long			m_eCondition;		//!	eCondition
	long			m_eConditionUse;	//! eConditionUse
	vector<BYTE>	m_vData;			//!	ֵ����
};

//! �ֶ�����
struct  ItemCondition
{
	long						m_valueType;	//!	eSendControl
	vector<pair<long, long>>	m_vPosList;	//! first:table_type, second:item_index
	vector<ConditionTile>		m_vConditionTile;
};


//! --------------------------------------------------------------------------------------
//! ��־�ֶ�
struct LogItem
{
	char			m_szItemName[128];
	long			m_eType;
	long			m_conditionId;

	LogItem(void)		{ memset(this, 0, sizeof(LogItem)); }
};

//! �ֶ�����
typedef	vector<LogItem>				ItemVector;
typedef	vector<LogItem>::iterator	ItemIte;



//! --------------------------------------------------------------------------------------
//! ��־��
struct LogTable
{
	char			m_szTableName[128];
	long			m_tableType;
	long			m_SendControl;
	long			m_isLog;
	ItemVector		m_vecItem;

	LogTable(void)		{ Clear(); }
	void Clear(void)	{ memset(this, 0, sizeof(LogTable) - sizeof(ItemVector)); m_vecItem.clear();}
};

//! ��־������������ݳ���
const long LogTableBaseDataSize = sizeof(LogTable) - sizeof(ItemVector);

//! ������
typedef	map<long, LogTable>					TableMap_byType;
typedef	map<long, LogTable>::iterator		TableMap_ite;



//! --------------------------------------------------------------------------------------
//! ��־���ݿ�
struct LogDbInfo
{
	long			m_worldNo;			//! ��Ӧ������
	long			m_lClientIp;
	char			m_szProvider[128];	//! �ṩ��
	char			m_szDbName[256];	//! ���ݿ�����
	char			m_szDbIP[256];		//! ���ݿ������IP
	char			m_szUsername[128];	//! �û���
	char			m_szPassword[128];	//! ����

	LogDbInfo(void)		{ memset(this, 0, sizeof(LogDbInfo)); }
};

typedef	vector<LogDbInfo>				DbInfoVector;
typedef	vector<LogDbInfo>::iterator		DbInfoIte;