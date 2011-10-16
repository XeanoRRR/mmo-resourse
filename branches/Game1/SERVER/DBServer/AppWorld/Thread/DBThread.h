//DBThread.h/////////////////////////////////////////////////////////////////////
//������:�ö���ά��һ�����ݿ����ʹ�õ��߳�,���в��������߳������
//Author:������
//Create Time:2008.11.03
#pragma once
#include "..\..\dbaccess\myadobase.h"
#include <list>
#include "ThreadDataDef.h"


class DBAdoBase;
class CBaseEntity;

//! ---------------------------------------���ݿ��߳���---------------------------------------
//!	�����߳��ɵ���Begin������ʼ������Exit����������
//! һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
//! ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
//!---------------------------------------------------------------------------------------------------
class DBThread
{
public:
	DBThread(std::string strProvider, 
				std::string strDataSource, 
				std::string strInitialCatalog,
				std::string strUserID, 
				std::string strPassword,
				eThreadType type);
	~DBThread(void);

public:
	//!								��ʼ�����̹߳���
	bool							Begin(void);
	//!								���������߳�
	void							Exit(void);
	//!								ȡ���߳�ID
	long							GetThreadID(void) { return m_lThreadID; }
	//!								�߳��˳��ȴ����
	HANDLE&							GetQuitWaitHandle(void) { return m_hQuitWait; }
protected:
	DBThread(void);

	//!								�ݹ�ִ�и���������ݿ����
	HRESULT							TravelRootDbOper(CEntityGroup* pRoot, _ConnectionPtr& cn);
	//!								ִ��һ�����ݿ����
	HRESULT							DbOperEntityAttrs(CEntityGroup* pRoot, _ConnectionPtr& cn);
	HRESULT							DbOperAllEntityGroup(CEntityGroup* pRoot, _ConnectionPtr& cn);
	HRESULT							DbOperLeaves(DB_OPERATION_TYPE dbOperType, const char* szSql, 
										const char* szTblName, const string& strComFlag, 
										map<CGUID, CBaseEntity*>& entityMap, CEntityGroup* pRoot, _ConnectionPtr& cn);
	//!		
	virtual bool					ProcessData(void);
	//!								�߳�
	static	unsigned	__stdcall	RunThread(void *pThis);

	bool							OpenCn(void);
	DBAdoBase*						GetAdoBasePtr(void);

	//!
	DBAdoBase*			m_pConnPtr;
	//!					���ݿ�����
	std::string			m_strInitialCatalog;
	//!					�߳�ID
	unsigned			m_lThreadID;
	//!					�߳��˳��ȴ����
	HANDLE				m_hQuitWait;
	map<CGUID,ThreadData*>   m_listData;
	eThreadType			m_ThreadType;
};

