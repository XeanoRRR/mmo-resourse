#pragma once

#include "../MyAdobase.h"
#include "RSGoods.h"
#include "RsPlayer.h"
#include <process.h>
using namespace std;

/*
*	���˼·

��Ʒ��Դ��LoginDB��Largess��
dbserver��ʱ��30�룩��LoginDB��Largess�е��µ���Ʒ��¼ͬ����CostDB��Largess���У�
dbserver��ʱ��30�룩��CostDB��Largess���еļ�¼��ȡ���ڴ��� m_mapLargess��
ÿ�����ʺŵ�½��ȡplayer����ʱ����m_mapLargess��Ѱ����û������ڸ��ʺŵ���Ʒ��
����о�ֱ�ӷŵ��ʺŵĲֿ��У��ֿ����˾Ͳ��ţ���
���ϲ��趼����dbserver�ڴ��н��У�

��ÿ��ϵͳ���̵��ʱ�򣬰������Ѿ����ͳ�ȥ����Ʒ��CostDB��Largess���б�ǳ�����
����CostDB��LoadDetail����д���Ӧ���Ѿ���ȡ��¼����������Ʒ�Ѿ����ŵ����������

*	soft 2005-11-25
*/


class CLargess : public CMyAdoBase
{
	//////////////////////////////////////////////////////////////////////////
	//	�������
	//////////////////////////////////////////////////////////////////////////
private:
	//static void ReconnectDB(/*_ConnectionPtr &pConn*/);
	static void GetTime(char* strTime);						// ��ȡ��ǰʱ���ִ�
	//static void HandleError(/*_ConnectionPtr &pConn*/);			// �쳣����


	//////////////////////////////////////////////////////////////////////////
	//	�������
	//////////////////////////////////////////////////////////////////////////
public:
	static bool Init();											// ��ʼ��
	static bool UnInit();										// �ͷ�

	static bool SaveLoadDetails(string strCDKey);				// ��ĳ�ʺ��µ���Ʒװ����ϸ�����ݿ� �����߳�SaveThread�е���
	static void LoadLargess(CDBPlayer* pParam);					// ����Ʒ���ӵ���Ҳֿ���
	static long GetLargessNum()		{return (long)m_mapLargess.size();}	// ��ȡ��û�з������Ʒ����

private:
	//static bool AddOneLargess(/*long lSend,*/CDBPlayer* pPlayer,CDBGoods* pGoods);		// ���һ����Ʒ���ֿ�
	//static bool AddGoldCoin(/*long lSendID,*/CDBPlayer* pPlayer,CDBGoods* pGoods);		// ��ӽ�Ǯ������
	static void AppendLargessToMap(long lSendID,string strCDKey,DWORD dwGoodsIndex,long lSendNum,long lObtainedNum,long lGoodsLevel);     // �����ݿ��������ͼ�¼���ڴ�

	// ��Ʒ�ṹ
	struct tagLargess
	{
		long lSendID;											// ����ID��Ψһ��ţ���LoginDB���ݿ��Զ�����
		DWORD dwGoodsIndex;										// ���͵���Ʒindex
		long lSendNum;											// Ӧ��������
		long lObtainedNum;										// �ѻ�ȡ����
		long lGoodsLevel;										// ��Ʒ�ȼ�
		string strSendedTime;									// ���͵���Ҳֿ��ʱ��
		string strFailedReason;									// ʧ�ܵ�ԭ��
	};

	static multimap<string,tagLargess> m_mapLargess;						// ���ͼ�¼,KEYΪCdkey

	//////////////////////////////////////////////////////////////////////////
	//	�߳����
	//////////////////////////////////////////////////////////////////////////
private:
	static CRITICAL_SECTION CriticalSectionmapLargess;
	//	static CRITICAL_SECTION CriticalSectionCycleLoadThread;
	//	static CRITICAL_SECTION CriticalSectionReconnectCostDBThread;
	//	static CRITICAL_SECTION CriticalSectionTransferLargessThread;

	static CRITICAL_SECTION csConnectSatus;

	static CRITICAL_SECTION csWorkerThread;		

	//static unsigned __stdcall ReconnectDBThread(void* pParam);				// �������ݿ��߳�
	//	static unsigned __stdcall TransferLargessThread(void* pParam);			
	//	static unsigned __stdcall CycleLoadLargessThread(void* pParam);			// CostDB->dbserver �Ķ�ȡ�߳�

	// �޸���ǰ2���߳���ɵĹ��ܣ����ϵ�һ���߳���ȥ��ɡ�
	// LoginDB�ϵ���Ʒ��¼���䵽CostDB�ϵ��߳�
	// CostDB->dbserver �Ķ�ȡ�߳�
	static bool TransferLargessThread();
	static bool CycleLoadLargessThread();
	static unsigned __stdcall WorkerThread(void* pParam);	

public:
	//	static void StartTransferLargessThread();								// ������������LoginDB�ϵ���Ʒ��¼���䵽CostDB�ϵ��߳�
	//	static void StartCycleLoadLargessThread();								// ���� CostDB->dbserver �Ķ�ȡ�߳�

	static void StartWorkerThread();

	static HANDLE m_hWorkThread;	// �����߳̾��
};
