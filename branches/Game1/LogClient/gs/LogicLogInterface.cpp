
#include "../StdAfx.h"


#include "LogicLogInterface.h"
#include "LogClient.h"

#pragma warning(disable:4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
	_snprintf(szTime, 128, "%d-%d-%d %d:%d:%d" ,
		stTime.wYear,
		stTime.wMonth,
		stTime.wDay,
		stTime.wHour,
		stTime.wMinute,
		stTime.wSecond);

	strTime = string(szTime);
}

/*
inline bool LogicLogInterface::LogT350_team_join(GSTeam *pTeam, CPlayer *pPlayer)
{
if(0 == GetGame()->GetSetup()->lUseLogServer) return true;

assert(NULL != m_pLogClient);

CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(pPlayer->GetRegionGUID()));

string strTime;
GetCurrTimeString(strTime);

float posX = pPlayer->GetTileX();
float posY = pPlayer->GetTileY();

return m_pLogClient->SendLogToServer( 
350, strTime.c_str(),
pRegion->GetID(),		pRegion->GetName(), 
&posX,					&posY, 
&(pTeam->GetExID()),	pTeam->GetName().c_str(), 
&(pPlayer->GetExID()),	pPlayer->GetName()
);
}
*/



//! ע�⣡
//! ����Ľӿڶ������߼�ģ���Լ���ɣ���Ҫ������LogicLogInterface.cpp��





