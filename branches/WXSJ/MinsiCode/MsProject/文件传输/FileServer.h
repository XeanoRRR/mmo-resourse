#pragma once
#include "S2C_FileInfo.h"
#include "S2C_FlyFile.h"

class CFileServer
    : public CMsMsgThread
{
public:
    CFileServer(WORD wProt, DWORD dwMaxClient, TCHAR* szPassword);
    ~CFileServer(VOID);

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent);

    // ���д���
    virtual VOID OnThreadFreeWork(VOID);

    // ��Ϣ����
    virtual VOID OnThreadMsgExecute(LPMSG lpMsg);

    // ��ȡ����
    TCHAR*  GetPassword(VOID);

    // ���ļ�
    BOOL    FlyFile(DWORD dwIndex, TCHAR* szFileName);

private:
    CMsServerNetManager*    m_lpCMsServerNetManager;
    CMsPacketFactory*       m_lpCMsPacketFactory;
    CMsFindFileManager*     m_lpCMsFindFileManager;
    TCHAR                   m_szPassword[MAX_PATH];
    S2C_FlyFile::MS_PACKET_DATA m_S2C_FlyFileData;
};

extern CFileServer* g_pCFileServer;
