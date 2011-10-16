

#pragma once
#include "DbOperator.h"


//! --------------------------------------------------------------------------------------
//! ��־������
//!
class LogManage
{
public:
	LogManage(HWND hwnd = NULL);
	~LogManage(VOID);


public:
	//!					��ʼ��
	BOOL				Init(VOID);
	//!					�ر�
	VOID				Exit(VOID);
	//!					�ͷ�
	VOID				Release(VOID);

	//!					��֤���������
	BOOL				AffirmClientNo(LONG dClientNo, LONG lClientIp, LONG lSocket);

	//!					����Socket��ÿͻ��˱�ź�IP
	//!					����ֵ��pair<�ͻ��˱��, IP>
	pair<LONG, LONG>	GetNoAndIpBySockte(LONG lSocket);

	//!					�ͻ��˵���
	VOID				ClientLost(LONG lSocket);
	
	//!					���һ����־д�����
	BOOL				AddLog(LONG lDbID, LPCSTR pSql);

	//!					������ת��Ϊsql��䣬����ӵ�д�뻺��
	BOOL				AddData(LONG lDbID, LONG lTableType, BYTE* pData);

	//!					��Ӧ��ʱ��д����־����С�ڻ�����ʱ���ɴ˴���д�룩
	VOID				OnOutTime(VOID);

	//!					���л����б�vector
	VOID				TableAddToByteArray(vector<byte>& outVector);

	//!					���л�����������vector
	VOID				ConditionAddToByteArray(vector<byte>& outVector);

	//!					�ض���־������Ϣ
	BOOL				ReLoadControlConfig(VOID);

	//!					��DB�д�����񣨰��������ж�ȡ�ı����Ϣ������Ϊÿ����񴴽�һ��ʱ��ۼ�����
	//!					Ϊ��ֹ��־��¼����ɾ����������DROP���
	//!					��������bOutToFile����ֻ���һ����׺��Ϊsql���ļ�������DB��ִ�д������
	VOID				CreateTableInDb(BOOL bOutToFile = TRUE, string file_path = "setup/");

	//!					������ݱ���Ϣ���ļ�
	VOID				OutTableInfoToFile(VOID);

private:
	//!					��ȡ���ݿ�����
	BOOL				LoadServerConfig(VOID);
	//!					��ȡ���ݱ�ṹ����
	BOOL				LoadTableConfig(VOID);
	//!					��ȡ��־������Ϣ
	BOOL				LoadControlConfig(VOID);

	//!					��������
	BOOL				CreateCache(VOID);
	
	//!					���DB��Ϣ��ִ�ж���
	LONG				AddDbInfoToOperator(VOID);
	//!					����������Ϣ
	BOOL				OutErrMsgBox(char*	pMsg);

	//!					��ֵ����ת��Ϊ�ַ�����ʾ
	BOOL				GetValueByType_WithDB(BYTE *pData, LONG &lPos, const LogItem& logItem, string &strValue);

	//!					���һ����־д�����
	BOOL				AddLog(LONG lDbID, LONG lTableID, LPCSTR pSql);

	

private:
	//!					�õ�һ������Ϣ
	const	LogDbInfo*	GetLogDbInfo(LONG lDbNo);


private:
	//!									���п���Ϣ
	DbInfoVector						m_vecDbInfo;
	//!									���б�����
	TableMap_byType						m_TableMap;
	//!									ÿ�����еĸ��ӱ�ͷ���ֶμ���
	ItemVector							m_TableHead;
	//!									ɸѡ��������
	vector<ItemCondition>				m_vItemCondition;

	//!									�ͻ�����Ϣ��key=�ͻ���socket, value=<�ͻ��˱��, �ͻ���IP>��
	map<LONG, pair<LONG, LONG>>			m_mapClientInfo;

	//!									��Ϣ���Ϳ���
	vector<pair<LONG, LONG>>			m_vSendControl;
	//!									�ϼ�ģ����������������Ϣ�ã���ΪNULL��
	HWND								m_hWnd;

	map<LONG, map<LONG, DbOperator*>>	m_mapmapSqlStr;



public:
	//!									��־д�����map<���ݿ���, map<����, д������>>
	map<LONG, map<string, LONG>>		m_WriteCount;
};