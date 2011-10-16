#pragma once

#include "ServiceSCC.h"


class ServiceControl
{
public:
	//!									��װ����
	static		BOOL					InstallService(VOID);
	//!									��������
	static		BOOL					StartService(VOID);

private:
	//!									���������������
	static		VOID	WINAPI			ServiceMain(int argc, char** argv);
	//!									���������¼���Ӧ���
	static		VOID	WINAPI			ControlHandler(DWORD request);
	

private:
	//!									��������
	static		char					*c_pServiceName;
	//!									���д���ֵ����
	static		LONG					c_lServiceRunError;

	static		SERVICE_STATUS			c_ServiceStatus;
	static		SERVICE_STATUS_HANDLE	c_hStatus;
	static		DWORD					c_dwThreadID;
};