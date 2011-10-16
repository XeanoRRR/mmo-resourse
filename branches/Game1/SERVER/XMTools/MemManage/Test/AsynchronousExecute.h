


/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-13
*/



#pragma once



//! �첽ִ����
class AsynchronousExecute
{
public:
	//!		bCallCom:				�߳��Ƿ�����COM���
	AsynchronousExecute(BOOL bCallCom = FALSE);
	//! ��ͬ��AsynchronousExecute() + SetWorkParam()
	//!		dwMinThreadNum��		��С���ֵ��߳�������С��Чֵ0��
	//!		dwMinThreadNum��		��󱣳ֵ��߳����������Чֵ128��
	//!		dwThreadNumAdjustSpace���Զ������߳������ļ��ʱ�䣨��Сֵ3000����λ�����룩
	//!		bCallCom:				�߳��Ƿ�����COM���
	AsynchronousExecute(DWORD dwMinThreadNum, DWORD dwMaxThreadNum, DWORD dwThreadNumAdjustSpace, BOOL bCallCom = FALSE);
	~AsynchronousExecute(VOID);

	/*----------------------------------------------------------------
	/*
	----------------------------------------------------------------*/
#pragma region ���ܽӿ�
public:
	//!										���ù����������ڶ��������󣬽ӿ���Ч��
	//!											dwMinThreadNum��				��С���ֵ��߳�������С��Чֵ0��
	//!											dwMinThreadNum��				��󱣳ֵ��߳����������Чֵ128��
	//!											dwThreadNumAdjustSpace��		�Զ������߳������ļ��ʱ�䣨��Сֵ3000����λ�����룩
	BOOL									SetWorkParam(DWORD dwMinThreadNum, DWORD dwMaxThreadNum, DWORD dwThreadNumAdjustSpace);
	//!										��ʼ����
	//!											dwEmbryonicThreadNum��			��ʼ�߳���
	//!											dwNumberOfConcurrentThreads��	��󲢷�����0 = CUP����
	VOID									BeginWork(DWORD dwEmbryonicThreadNum, DWORD dwNumberOfConcurrentThreads = 0);
	//!										��������
	VOID									Exit(VOID);

	//!										�õ�����״̬
	BOOL									IsRunning(VOID);

	//!										��������ָ��
	typedef		LPVOID						(*WorkFunction)(LPVOID);

	//!										�첽ִ��һ���޷��غ���
	//!											pfWork��						��������ָ��
	//!											pParm��							������������
	VOID									Execute(WorkFunction pfWork, LPVOID pParm);
	//!										�첽ִ��һ�����غ�������������ֵ��Ϊ�������ݵ���ɺ��ִ�н����������
	//!											pfWork��						��������ָ��
	//!											pParm��							������������
	//!											pParm��							�ص�����ָ��
	//!											��ǰ��ʵ�ֲ�û�ж�pfWork��pfCallBack���̴߳������б�Ҫ�ڽ������
	VOID									Execute(WorkFunction pfWork, LPVOID pParm, WorkFunction pfCallBack);

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
	VOID									AddThreads(DWORD dwAddNum = 1);
	VOID									RemoveThreads(VOID);


private:
	HANDLE									m_hManageIoPort;
	HANDLE									m_hMgrThread;
	unsigned								m_dwThreadId;
	//!										����״̬
	BOOL									m_bIsRunning;
	//!										��С�����߳���
	DWORD									m_dwMinThreadNum;
	//!										��󱣳��߳���
	DWORD									m_dwMaxThreadNum;
	//!										��ʱʱ��
	DWORD									m_dwWaitTime;
	//!										�������߳��Ƿ�Ҫ����COM���
	const		BOOL						m_bCallCom;


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
		ThreadInfo(VOID)										{m_dwThredId=0; m_hThread=NULL; m_bBusyWorking=FALSE; }
		ThreadInfo(DWORD dwThredId, HANDLE handle, BOOL bBusy)	{m_dwThredId=dwThredId; m_hThread=handle; m_bBusyWorking=bBusy; }
		ThreadInfo(const ThreadInfo& info)						{m_dwThredId=info.m_dwThredId; m_hThread=info.m_hThread; m_bBusyWorking=info.m_bBusyWorking; }

		DWORD		m_dwThredId;
		HANDLE		m_hThread;
		BOOL		m_bBusyWorking;
	};

	typedef		map<DWORD, ThreadInfo>		ThreadInfoMap;

private:
	static		unsigned		WINAPI		WorkerProc(void* p);

private:
	VOID									AddThreadInfo(DWORD dwThredId, HANDLE hThread, BOOL bBusy);
	VOID									DelThreadInfo(DWORD dwThredId);
	VOID									SetThreadInfo(DWORD dwThredId, BOOL bBusy);
	ThreadInfo*								GetThreadInfo(DWORD dwThredId);
	DWORD									GetThreadNum(VOID);
	VOID									GetAllThreadHandle(vector<HANDLE> &vThreadHandle);
	eThreadPoolStatus						GetThreadPoolStatus(VOID);

	MyOverlapped*							CreateMyOverlapped(VOID);
	VOID									RestoreMyOverlapped(MyOverlapped** ppMyOverlapped);

private:
	HANDLE									m_hWorkIoPort;
	CRITICAL_SECTION						m_CriticalSection;
	ThreadInfoMap							m_mapThreadInfo;

#pragma endregion


};