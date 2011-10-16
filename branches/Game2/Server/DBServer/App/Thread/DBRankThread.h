/**
*File:  DBRankThread.h
*brief: �������а����ݼ��ؼ������߳�
*Author:��η
*Date:  2010-03-03
*/
#pragma once
#include "../../../../Public/Common/RankDef.h"

class DBAdoBase;

class DBRankThread
{
public:
    DBRankThread(string strProvider,
        string strDataSource,
        string strInitialCatalog,
        string strUserID,
        string strPSW);
    virtual ~DBRankThread(void);

    bool Begin(void);
    void Exit(void);

    DBAdoBase* GetAdoBasePtr() { return m_pConnPtr; }

    // ���ݼ��ز���
    bool LoadRanks(tagRankInfo *pRankInfo);

    // �������ͻ�ְ�����
    void SendRanksToWS(bool bSendAll = true);
    // ��������
    void SendRanksToWS(long lRankID);

    // ��������
    bool UpdateRanks(long lRankID);

    HANDLE GetHandle(void)  { return m_hThread; }
    void    SetHandle(HANDLE hHandle)   { m_hThread = hHandle; }

    bool   GetExit(void)    { return m_bExit; }
    void    SetExit(bool bExit) { m_bExit = bExit; }

    bool    IsLoadNow(void)     { return m_bLoadNow; }
    void    SetReLoad(bool bLoad)   { m_bLoadNow = bLoad; }

    DWORD GetLoadTime(void) { return m_dwLoadTime; }
    void SetLoadTime(DWORD dwTime)  { m_dwLoadTime = dwTime; }

private:
    DBAdoBase*      m_pConnPtr;
    unsigned        m_nThreadID;
    HANDLE          m_hThread;
    bool            m_bExit;
    bool            m_bLoadNow;

    DWORD           m_dwLoadTime;   // ����ʱ��

    // �����߳�
    static unsigned __stdcall RunThread(void* pArgument);

};