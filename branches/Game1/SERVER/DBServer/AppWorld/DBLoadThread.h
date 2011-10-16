#pragma once
#include <list>
#include "..\..\..\dbaccess\myadobase.h"
#include "ThreadDataDef.h"

class DBAdoBase;
class CDBPlayer;
class CDBLoginPlayer;
//! ---------------------------------------���ݿ��߳���---------------------------------------
//!	�����߳��ɵ���Begin������ʼ������Exit����������
//! һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
//! ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
//!---------------------------------------------------------------------------------------------------

class DBLoadThread
{
public:
	DBLoadThread(string strProvider, 
				string strDataSource, 
				string strInitialCatalog,
				string strUserID, 
				string strPassword
				);
	~DBLoadThread(void);

public:
	//!								��ʼ�����̹߳���
	bool							Begin(void);
	//!								���������߳�
	void							Exit(void);
	//!								�Ƿ�������
	bool							GetRunningFlag(void) { return m_LoadRunning; }
	//!								�����Ƿ�������
	void							SetRunningFlag(bool flag) { m_LoadRunning = flag; }
	//!								�����˳��¼�
	void							SetLoadWaitThread(void);
	//!								�õ��߳�ID
	long							GetThreadID(void) { return m_lThreadID; }
	//!								�߳��˳��ȴ����
	HANDLE&							GetQuitWaitHandle(void) { return m_hQuitWait; }
private:
	//!		
	bool							ProcessData(void);
	//!								�߳�
	static	unsigned	__stdcall	RunThread(void *pThis);
	//!								ConnctionPtr
	_ConnectionPtr					GetConnPtr(void);
	bool							OpenCn(void);
	DBAdoBase*						GetAdoBasePtr(void);
private:
	//!
	DBAdoBase*			m_pConnPtr;
	//!					���ݿ�����
	string				m_strInitialCatalog;
	//!					�߳̾��
	HANDLE				m_hLoadWaitThread;
	//!					�߳����б��
	bool				m_LoadRunning;
	//!					����ID
	unsigned int		m_lThreadID;
	//!					�߳��˳��ȴ����
	HANDLE				m_hQuitWait;
};