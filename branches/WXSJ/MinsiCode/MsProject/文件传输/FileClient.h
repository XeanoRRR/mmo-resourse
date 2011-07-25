#pragma once

class CFileClient
    : public CMsMsgThread
{
public:
    CFileClient(TCHAR* szServerIP, WORD wProt, TCHAR* szPassword);
    ~CFileClient(VOID);

    // ֹ֮ͣǰ
    virtual BOOL ForeStop(VOID);

    // �߳��¼�֪ͨ
    virtual VOID OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent);

    // ���д���
    virtual VOID OnThreadFreeWork(VOID);

    // ��Ϣ����
    virtual VOID OnThreadMsgExecute(LPMSG lpMsg);

    // �����ļ�
    VOID CreateFile(TCHAR* szFileName);

    // ׷���ļ�
    VOID AppendFile(LPVOID lpData, DWORD dwSize);

private:
    CMsClientNetManager*    m_lpCMsClientNetManager;
    CMsPacketFactory*       m_lpCMsPacketFactory;
    TCHAR                   m_szPassword[MAX_PATH];
    HANDLE                  m_hFile;
};

extern CFileClient* g_pCFileClient;