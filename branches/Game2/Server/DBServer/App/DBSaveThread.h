#pragma once

#include "..\..\..\dbaccess\myadobase.h"
struct SaveObj;
class CBaseDBEntity;
struct SaveThreadData;
class DBAdoBase;
//! ---------------------------------------���ݿ��߳���---------------------------------------
//!	�����߳��ɵ���Begin������ʼ������Exit����������
//! һ��������Exit�����ӿڣ������ٽ����µ����񣬵��Ѿ�ѹ����������ִ����ɡ�
//! ����ʱ���Զ����������̣߳����Ƽ��ֶ�����Exit������
//!---------------------------------------------------------------------------------------------------
class DBSaveThread
{
public:
	DBSaveThread(string strProvider, 
				string strDataSource, 
				string strInitialCatalog,
				string strUserID, 
				string strPassword
				);
	~DBSaveThread(void);

public:
	//!								��ʼ�����̹߳���
	bool							Begin(void);
	//!								���������߳�
	void							Exit(void);
	//!								�Ƿ�������
	bool							GetRunningFlag(void) { return m_SaveRunning; }
	//!								�����Ƿ�������
	void							SetRunningFlag(bool flag) { m_SaveRunning = flag; }
	//!								�����˳��¼����
	void							SetSaveWaitThread(void);
	//!								ȡ���߳�ID
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
	HANDLE				m_hSaveWaitThread;
	//!					�߳����б��
	bool				m_SaveRunning;
	//!					�߳�ID
	unsigned			m_lThreadID;
	//!					�߳��˳��ȴ����
	HANDLE				m_hQuitWait;
};