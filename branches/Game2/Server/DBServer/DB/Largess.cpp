#include "stdafx.h"

#include <map>
#include "Largess.h"
//#include "..\..\server\dbserver\appworld\goods\cgoodsfactory.h"
#include "..\..\server\dbserver\dbserver\game.h"
//#include "..\..\server\dbserver\appworld\goods\CGoodsBaseProperties.h"
//#include "../../server/setup/Logsystem.h"
#include "..\..\server\dbserver\AppWorld\DBentityManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

multimap<string,CLargess::tagLargess> CLargess::m_mapLargess;
CRITICAL_SECTION CLargess::CriticalSectionmapLargess;
CRITICAL_SECTION CLargess::csWorkerThread;
//CRITICAL_SECTION CLargess::CriticalSectionTransferLargessThread;
CRITICAL_SECTION CLargess::csConnectSatus;

HANDLE CLargess::m_hWorkThread = NULL;

extern void AddLogText(char* msg, ...);
extern CGame* GetGame();

// ��ʼ��
bool CLargess::Init()
{	
	//if (!GetGame()->GetSetup()->dwLoadLargessTime) return false;

	InitializeCriticalSection(&CriticalSectionmapLargess);
	InitializeCriticalSection(&csWorkerThread);

	InitializeCriticalSection(&csConnectSatus);
	return true;
}

// �ͷ�
bool CLargess::UnInit()
{	
	// �ȴ������߳̽���
	WaitForSingleObject(m_hWorkThread, INFINITE);
	CloseHandle(m_hWorkThread);

	DeleteCriticalSection(&CriticalSectionmapLargess);
	DeleteCriticalSection(&csWorkerThread);
	//DeleteCriticalSection(&CriticalSectionTransferLargessThread);
	DeleteCriticalSection(&csConnectSatus);
	return true;
}

//�����ݿ��������ͼ�¼���ڴ�
void CLargess::AppendLargessToMap(long lSendID,string strCDKey,DWORD dwGoodsIndex,long lSendNum,long lObtainedNum,long lGoodsLevel)
{	
	tagLargess stLargess={lSendID,dwGoodsIndex,lSendNum,lObtainedNum,lGoodsLevel};
	if (m_mapLargess.empty())
		m_mapLargess.insert(pair<string,tagLargess> (strCDKey,stLargess));
	else
	{
		multimap<string,tagLargess>::iterator it=m_mapLargess.begin();
		for (;it!=m_mapLargess.end();it++)
		{
			if (it->second.lSendID==lSendID)
				return;
		}
		if (it==m_mapLargess.end())
			m_mapLargess.insert(pair<string,tagLargess> (strCDKey,stLargess));	
	}
	return;
}
/*
//��ӽ�Ǯ������
bool CLargess::AddGoldCoin(CDBPlayer* pPlayer,CDBGoods* pGoods)
{
if (pPlayer->m_cBank.AddGoldCoinOfLargess(pGoods))
{
return true;
}
else
{
return false;
}
}

//���һ����Ʒ���ֿ�
bool CLargess::AddOneLargess(CDBPlayer* pPlayer,CDBGoods* pGoods)
{
bool res = false;
DWORD dwPosition=0;
DWORD dwTmpAmount=0;	
for (;dwPosition<96;dwPosition++)
{
if(pPlayer->m_cDepot.Add(dwPosition,pGoods))  
{
res = true;
break;
}
}

// ������Ϣ
if( !res )
{
char str[512];
sprintf(str, "Acc:%s Id:%d Pos:%d ����%s(%d)ʧ��!",pPlayer->GetAccount(), pPlayer->GetID(),
dwPosition,	pGoods->GetName(), pGoods->GetAmount());
PutStringToFile("Largess", str);
}

return res;
}
*/
//��ĳ�ʺ��µ���Ʒװ����ϸ�����ݿ�
bool CLargess::SaveLoadDetails(string strCDKey)
{
	return false;
}

// ����Ʒ���ӵ���Ҳֿ���
void CLargess::LoadLargess(CDBPlayer* pParam)
{
}

//////////////////////////////////////////////////////////////////////////
//	�߳����
//////////////////////////////////////////////////////////////////////////

// ���������߳�
void CLargess::StartWorkerThread()
{
}

unsigned __stdcall CLargess::WorkerThread(void* pParam)
{
	return 0;
}

bool CLargess::TransferLargessThread()
{
	return false;
}

bool CLargess::CycleLoadLargessThread()
{
	return false;
}


void CLargess::GetTime(char* strTime)
{		
	SYSTEMTIME Time;
	GetLocalTime(&Time);
	sprintf(strTime,"%d-%d-%d %d:%d:%d",Time.wYear,Time.wMonth,Time.wDay,Time.wHour,Time.wMinute,Time.wSecond);
	return;
}
