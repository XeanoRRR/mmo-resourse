/**
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-23
*/
#pragma once
#include "../../../Public/ServerPublic/db/CnPool.h"

const DWORD MAX_SQL_STRING_SIZE = 4096;

class DbOperator
{
public:
	static	VOID			Release(VOID);

public:
	//!						���һ��ִ��������Ϣ
	static	BOOL			AddExecuteData(LONG lDbID, string &strDbConn, string &strErrorLogFileName, string &strBusyLogFileName);
	//!						�ͷ�����ִ��������Ϣ
	static	VOID			ReleaseExecuteData(VOID);

	//!						����һ����������
	static	DbOperator*		CreateDbOperator(LONG lDbID);
	//!						�ͷŶ���
	static	VOID			ReleaseDbOperator(DbOperator **ppDbOperator);

public:
	//!						���һ��ִ�����
	VOID					AddSql(LPCSTR pSql){m_strSql.append(pSql);}
	//!						���һ��ִ�����
	size_t					GetSqlSize(VOID) {return m_strSql.size();}
	//!						ִ��
	VOID					ExecuteSql(VOID);

private:
	//!						ִ���߳�
	static	LPVOID			ExecuteThread(LPVOID pThis);
	//!						DBʧ�ܴ���
	static	LPVOID			OnExecuteFinish(LPVOID pThis);

private:
	struct tagExecuteData
	{
		//!					���ݿ������ַ���
		//string				_strDbConn;
		CnPool				*pCnPool;
		//!					�����¼�ļ�����
		string				_strErrorLogFileName;
		//!					��æ��¼�ļ�����
		string				_strBusyLogFileName;
		//!					д�ļ��ٽ���
		CRITICAL_SECTION	_FileCriticalSection;
	};

	tagExecuteData			*m_pExecuteData;
	string					m_strSql;

private:

	static	map<LONG, tagExecuteData>				c_mapExecuteData;

	static	MultiObjPool<DbOperator, LockOfWin32>	c_MultiObjPool;

private:
	DbOperator(VOID){}
	DbOperator(DbOperator&);
	~DbOperator(VOID){}
};