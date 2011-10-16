#pragma once

//#include "../../../public/UseMemManageSCC.h"
#include "ServiceNet.h"


class ServiceSCC
{
public:
	ServiceSCC(VOID);
	~ServiceSCC(VOID);
public:
	static		ServiceSCC&			GetInstance(VOID);

public:
	//!								��ʼ������
	BOOL							Init(VOID);
	//!								ִ�г���
	VOID							Begin(VOID);
	//!								�ⲿ�������򣨲��ܸ�RUN��ͬһ���̵߳��ã�
	VOID							CloseExec(VOID);
	//!								�ͷų���
	VOID							Relese(VOID);

#ifndef OUT_SERVICE
	//!								ִ������ָ��
	VOID							RunCommand(VOID);
	//!								�ⲿ�����ָ����
	VOID							InputCommand(DWORD dwCommand);
#endif

public:
	//!								��ӦSUC����
	VOID							OnConnSUC(LONG lSocket);
	//!								��Ӧ�Ͽ�SUC
	VOID							OnLostSUC(LONG lSocket);
	//!								��ӦSM����
	DWORD							OnConnSM(LONG lSocket, LONG lIP);
	//!								��Ӧ�Ͽ�SM
	VOID							OnLostSM(DWORD dwSMID);

private:
	//!								ִ��һ֡�����ú�������FALSEʱ���������ڲ�������
	BOOL							RunOneFrame(VOID);
	//!
	static	unsigned	__stdcall	MainLoopFunc( void* pThis );

private:
	static	ULONG					c_InstanceNum;
	static	ServiceSCC				c_ServiceSCCInstance;
	

private:
	//!								�ⲿ֪ͨ�����ı�־
	BOOL							m_bExotericExit;
	//!								������
	HANDLE							m_hMutex;
	//!								���߳̾��
	HANDLE							m_hMainThrd;

#ifndef OUT_SERVICE
	DWORD							m_dwCommand;
#endif
};