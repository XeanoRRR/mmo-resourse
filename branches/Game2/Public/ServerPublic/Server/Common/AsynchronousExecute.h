/*
*	file:		
*	Brief:		
*	detail:					
*	Authtor:	�ſ���
*	Date:		2008-10-13
*/
  

#include "../Module/MultiObjPool.h"

#pragma once

//! �첽ִ����
class AsynchronousExecute
{
public:
	//!		bCallCom:				�߳��Ƿ�����COM���
	AsynchronousExecute(bool bCallCom = false);
	//! ��ͬ��AsynchronousExecute() + SetWorkParam()
	//!		dwMinThreadNum��		��С���ֵ��߳�������С��Чֵ0��
	//!		dwMinThreadNum��		��󱣳ֵ��߳����������Чֵ128��
	//!		dwThreadNumAdjustSpace���Զ������߳������ļ��ʱ�䣨��Сֵ3000����λ�����룩
	//!		bCallCom:				�߳��Ƿ�����COM���
	AsynchronousExecute(ulong dwMinThreadNum, ulong dwMaxThreadNum, ulong dwThreadNumAdjustSpace, bool bCallCom = false);
	~AsynchronousExecute(void);

	/*----------------------------------------------------------------
	/*
	----------------------------------------------------------------*/
#pragma region ���ܽӿ�
public:
	//!										���ù����������ڶ��������󣬽ӿ���Ч��
	//!											dwMinThreadNum��				��С���ֵ��߳�������С��Чֵ0��
	//!											dwMinThreadNum��				��󱣳ֵ��߳����������Чֵ128��
	//!											dwThreadNumAdjustSpace��		�Զ������߳������ļ��ʱ�䣨��Сֵ3000����λ�����룩
	bool									SetWorkParam(ulong dwMinThreadNum, ulong dwMaxThreadNum, ulong dwThreadNumAdjustSpace);
	//!										��ʼ����
	//!											dwEmbryonicThreadNum��			��ʼ�߳���
	//!											dwNumberOfConcurrentThreads��	��󲢷�����0 = CUP����
	void									BeginWork(ulong dwEmbryonicThreadNum, ulong dwNumberOfConcurrentThreads = 0);
	//!										��������
	void									Exit(void);

	//!										�õ�����״̬
	bool									IsRunning(void);

	//!										��������ָ��
	typedef		LPVOID						(*WorkFunction)(LPVOID);

	//!										�첽ִ��һ���޷��غ���
	//!											pfWork��						��������ָ��
	//!											pParm��							������������
	void									Execute(WorkFunction pfWork, LPVOID pParm);
	//!										�첽ִ��һ�����غ�������������ֵ��Ϊ�������ݵ���ɺ��ִ�н����������
	//!											pfWork��						��������ָ��
	//!											pParm��							������������
	//!											pParm��							�ص�����ָ��
	//!											��ǰ��ʵ�ֲ�û�ж�pfWork��pfCallBack���̴߳������б�Ҫ�ڽ������
	void									Execute(WorkFunction pfWork, LPVOID pParm, WorkFunction pfCallBack);

#pragma endregion
	/*----------------------------------------------------------------
	/*
	----------------------------------------------------------------*/
#pragma region ���й���
private:
	enum eWorkControl
	{
		eCC_ManagerThreadExit,
		eCC_WaitWorkThreadExit,
	};
	enum eThreadPoolStatus
	{
		eTPS_BUSY,
		eTPS_IDLE,
		eTPS_NORMAL 
	};

private:

	static		unsigned		WINAPI		ManagerProc(LPVOID p);

private:
	void									AddThreads(ulong dwAddNum = 1);
	void									RemoveThreads(void);


private:

	HANDLE									m_hManageIoPort;
	HANDLE									m_hMgrThread;
	unsigned								m_dwThreadId;
	//!										����״̬
	bool									m_bIsRunning;
	//!										��С�����߳���
	ulong									m_dwMinThreadNum;
	//!										��󱣳��߳���
	ulong									m_dwMaxThreadNum;
	//!										��ʱʱ��
	ulong									m_dwWaitTime;
	//!										�������߳��Ƿ�Ҫ����COM���
	const		bool						m_bCallCom;


#pragma endregion
	/*----------------------------------------------------------------
	/*
	----------------------------------------------------------------*/
#pragma region �����߳�

private:
	enum eExecuteMode
	{
		eEM_MidwayExit,
		eEM_Exit,
		eEM_Normal,
		eEM_CallBack,
	};
	struct MyOverlapped : public OVERLAPPED
	{
		WorkFunction	pfWork;
		LPVOID			pParm;
		WorkFunction	pfCallBack;
	};

	class ThreadInfo
	{
	public:  
		ThreadInfo(void)										{m_dwThredId=0; m_hThread=NULL; m_bBusyWorking=false; }
		ThreadInfo(ulong dwThredId, HANDLE handle, bool bBusy)	{m_dwThredId=dwThredId; m_hThread=handle; m_bBusyWorking=bBusy; }
		ThreadInfo(const ThreadInfo& info)						{m_dwThredId=info.m_dwThredId; m_hThread=info.m_hThread; m_bBusyWorking=info.m_bBusyWorking; }

		ulong		m_dwThredId;
		HANDLE		m_hThread;
		bool		m_bBusyWorking;
	};

	typedef		map<ulong, ThreadInfo>		ThreadInfoMap;

private:
	static		unsigned		WINAPI		WorkerProc(void* p);

private:
	void									AddThreadInfo(ulong dwThredId, HANDLE hThread, bool bBusy);
	void									DelThreadInfo(ulong dwThredId);
	void									SetThreadInfo(ulong dwThredId, bool bBusy);
	ThreadInfo*								GetThreadInfo(ulong dwThredId);
	ulong									GetThreadNum(void);
	void									GetAllThreadHandle(vector<HANDLE> &vThreadHandle);
	eThreadPoolStatus						GetThreadPoolStatus(void);

	MyOverlapped*							CreateMyOverlapped(void);
	void									RestoreMyOverlapped(MyOverlapped** ppMyOverlapped);

private:

	HANDLE									m_hWorkIoPort;
	CRITICAL_SECTION						m_CriticalSection;
	ThreadInfoMap							m_mapThreadInfo;

	MultiObjPool<MyOverlapped, LockOfWin32>	m_MultiObjPool;

#pragma endregion


};