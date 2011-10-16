
#include "../StdAfx.h"

#pragma warning(disable:4996)

#include "LogicLogInterface.h"
#include "LogClient.h"


LogicLogInterface::LogicLogInterface(LogClient *pLogClient)
{
	m_pLogClient = pLogClient;
}


//!							�õ���ǰʱ���ַ�����00-00-00 00:00:00��ʽ��
void LogicLogInterface::GetCurrTimeString(string &strTime)
{
	SYSTEMTIME	stTime;
	char		szTime[128] = {0};
	GetLocalTime(&stTime);
	sprintf(szTime,"%d-%d-%d %d:%d:%d" ,
		stTime.wYear,
		stTime.wMonth,
		stTime.wDay,
		stTime.wHour,
		stTime.wMinute,
		stTime.wSecond);

	strTime = string(szTime);
}


//! ע�⣡
//! ����Ľӿڶ������߼�ģ���Լ���ɣ���Ҫ������LogicLogInterface.cpp��





