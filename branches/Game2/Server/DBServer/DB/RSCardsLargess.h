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


 *
 * ���ڹ���ɹ��󣬰����ý��ŵ����ݿ��У�����´ε�½ʱ��ȡ������������ԭ�����ǣ�
 *  ֻ�н�Ǯ û������Ʒ
 *  ��dbserverֱ��д�����ݿ⣬���ô�logindbת������
 * copy by bugs 2006-1-21
 */


class CDBCardsLargess : public CMyAdoBase
{
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
	static bool AddOneLargess(/*long lSend,*/CDBPlayer* pPlayer,CDBGoods* pGoods);		// ���һ����Ʒ���ֿ�
	static bool AddGoldCoin(/*long lSendID,*/CDBPlayer* pPlayer,CDBGoods* pGoods);		// ��ӽ�Ǯ������
	static void AppendLargessToMap(long SendID,string strCDKey,long lSendNum,long lObtainedNum);     // �����ݿ��������ͼ�¼���ڴ�

	// ��Ʒ�ṹ
	struct tagLargess
	{
		long lSendID;											// ����ID
		long lSendNum;											// Ӧ��������
		long lObtainedNum;										// �ѻ�ȡ����
	};

	static multimap<string,tagLargess*> m_mapLargess;						// ���ͼ�¼,KEYΪCdkey

//////////////////////////////////////////////////////////////////////////
//	�߳����
//////////////////////////////////////////////////////////////////////////
private:
	static CRITICAL_SECTION csMapLargess;
	static CRITICAL_SECTION csCycleLoadThread;

	//static CRITICAL_SECTION csConnectSatus;

	static unsigned __stdcall TransferLargessThread(void* pParam);			// LoginDB�ϵ���Ʒ��¼���䵽CostDB�ϵ��߳�
	static unsigned __stdcall CycleLoadLargessThread(void* pParam);			// CostDB->dbserver �Ķ�ȡ�߳�

public:
	static void StartTransferLargessThread();								// ������������LoginDB�ϵ���Ʒ��¼���䵽CostDB�ϵ��߳�
	static void StartCycleLoadLargessThread();								// ���� CostDB->dbserver �Ķ�ȡ�߳�

	static HANDLE m_hWorkThread;	// �����߳̾��
};
