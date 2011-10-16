#include "stdafx.h"
#include "ServiceControl.h"
#include "sm.h"

char*						ServiceControl::c_pServiceName		= "SM";
LONG						ServiceControl::c_lServiceRunError	= 0;
SERVICE_STATUS				ServiceControl::c_ServiceStatus;
SERVICE_STATUS_HANDLE		ServiceControl::c_hStatus			= NULL;
DWORD						ServiceControl::c_dwThreadID			= 0;
SC_HANDLE					g_ServiceHandle=NULL;

//! ��װ����
BOOL ServiceControl::InstallService()
{
	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		LogTools::MsgboxErrInfo(AppFrame::GetText("IDS_SM_TIP3"));
		return FALSE;
	}

	//! ����Ѿ���װ����ֱ�Ӵ򿪷���
	g_ServiceHandle = ::OpenService(hSCM, ServiceControl::c_pServiceName, SERVICE_QUERY_CONFIG);
	if (g_ServiceHandle != NULL)
	{
		::CloseServiceHandle(g_ServiceHandle);
		return TRUE;
	}
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);
	//��������
	g_ServiceHandle = ::CreateService(
		hSCM, 
		ServiceControl::c_pServiceName, 
		ServiceControl::c_pServiceName,
		SERVICE_ALL_ACCESS, 
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START, 
		SERVICE_ERROR_NORMAL,
		szFilePath, 
		NULL, 
		NULL, 
		"", 
		NULL, 
		NULL);
	
	if (g_ServiceHandle == NULL)
	{
		::CloseServiceHandle(hSCM);
		LogTools::MsgboxErrInfo(AppFrame::GetText("IDS_SM_TIP4"),GetLastError());
		return FALSE;
	}

	::CloseServiceHandle(g_ServiceHandle);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

//! ��������
BOOL ServiceControl::StartService(VOID)
{
	c_dwThreadID = ::GetCurrentThreadId();
	if(!InstallService())
	{
		return FALSE;
	}
	c_ServiceStatus.dwServiceType				= SERVICE_WIN32;
	c_ServiceStatus.dwCurrentState				= SERVICE_START_PENDING;
	c_ServiceStatus.dwControlsAccepted			= SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	c_ServiceStatus.dwWin32ExitCode				= 0;
	c_ServiceStatus.dwServiceSpecificExitCode	= 0;
	c_ServiceStatus.dwCheckPoint				= 0;
	c_ServiceStatus.dwWaitHint					= 0;

	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = c_pServiceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceControl::ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	// ��������Ŀ��Ʒ��ɻ��߳� 
	StartServiceCtrlDispatcher(ServiceTable); 
	//::StartService(g_ServiceHandle,0,NULL);
	return TRUE;
}

//! ���������������
VOID WINAPI ServiceControl::ServiceMain(int argc, char** argv)
{
	c_hStatus = RegisterServiceCtrlHandler(c_pServiceName, (LPHANDLER_FUNCTION(ServiceControl::ControlHandler)));
	if (NULL == c_hStatus)
	{
		c_lServiceRunError = -1;
		return;
	}

	SetServiceStatus(c_hStatus, &c_ServiceStatus);

	c_ServiceStatus.dwWin32ExitCode = S_OK;
	c_ServiceStatus.dwCheckPoint	= 0;
	c_ServiceStatus.dwWaitHint		= 0;
	c_ServiceStatus.dwCurrentState	= SERVICE_RUNNING;

	SetServiceStatus(c_hStatus, &c_ServiceStatus);

	unsigned threadID;
	_beginthreadex(NULL,0,SMThreadFunc,NULL,0,&threadID);

	g_hSMThreadExitEvent = CreateEvent(NULL,0,0,0);
	//�ȴ�SM���߳�
	while(1)
	{
		if(g_bSMThreadExit)
			break;
		Sleep(10);
	}
	

	c_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(c_hStatus, &c_ServiceStatus);
	
}

//! ���������¼���Ӧ���
VOID WINAPI ServiceControl::ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
	case SERVICE_CONTROL_SHUTDOWN:
		{
			g_bSMThreadExit = true;
			//�ȴ����߳��˳�
			WaitForSingleObject(g_hSMThreadExitEvent, INFINITE);
			CloseHandle(g_hSMThreadExitEvent);
			c_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(c_hStatus, &c_ServiceStatus);
		}
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:

		break;
	}
}

