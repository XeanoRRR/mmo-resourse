#include "Precompiled.h"
#include "FileClient.h"

CFileClient* g_pCFileClient = NULL;
CFileClient::CFileClient(TCHAR* szServerIP, WORD wProt, TCHAR* szPassword)
: m_lpCMsClientNetManager   (NULL)
, m_lpCMsPacketFactory      (NULL)
, m_hFile                   (NULL)
{
    m_lpCMsPacketFactory = NEW CMsPacketFactory(RR_PACKET_MAX_COUNT);
    AssertEx(m_lpCMsPacketFactory, _T("�ڴ治��"));
    m_lpCMsPacketFactory->Register(g_lpCMsBasePacket, PACKET_MAX_COUNT);

    m_lpCMsClientNetManager = NEW CMsClientNetManager(
        szServerIP,
        wProt,
        m_dwThreadId,
        m_lpCMsPacketFactory
        );
    AssertEx(m_lpCMsClientNetManager, _T("�ڴ治��"));

    if (szPassword)
    {
        _tcscpy_s(m_szPassword, MAX_PATH, szPassword);
    }
}

CFileClient::~CFileClient(VOID)
{
    this->Stop(TRUE);
    SAFE_DELETE(m_lpCMsClientNetManager);
    SAFE_DELETE(m_lpCMsPacketFactory);
}

// ֹ֮ͣǰ
BOOL CFileClient::ForeStop(VOID)
{
    return m_lpCMsClientNetManager->Stop(TRUE);
}

// �߳��¼�֪ͨ
VOID CFileClient::OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent)
{
    switch (eThreadEvent)
    {
    case E_ON_THREAD_RUN:
        {
            m_lpCMsClientNetManager->Run();
        }break;
    case E_ON_THREAD_SUSPEND:
        {

        }break;
    default:
        {
            AssertEx(FALSE, _T("����"));
        }break;
    }
}

// ���д���
VOID CFileClient::OnThreadFreeWork(VOID)
{
    return;
}

// ��Ϣ����
VOID CFileClient::OnThreadMsgExecute(LPMSG lpMsg)
{
    switch (lpMsg->message)
    {
    case MSM_CLIENT_SOCKET_ON_ENTER:
        {
            CMsServerSocket* lpSocket = (CMsServerSocket*)lpMsg->wParam;
            C2S_Connect::MS_PACKET_DATA PacketData;
            _tcscpy_s(PacketData.sz_Password, MAX_PATH, m_szPassword);
            lpSocket->WritePacket(BPT(C2S_Connect), &PacketData);
            _tcout << _T("���ӷ�����\n");
        }break;

    case MSM_CLIENT_SOCKET_ON_EXIT:
        {
            _tcout << _T("�Ͽ�������\n");
        }break;

    case MSM_MSG_SOCKET_EXECUTE:
        {
        }break;

    default:
        {
            ;
        }break;
    }
}

// �����ļ�
VOID CFileClient::CreateFile(TCHAR* szFileName)
{
    m_hFile = ::CreateFile(szFileName, FILE_ALL_ACCESS, 0, NULL, CREATE_NEW, 0, NULL);
}

// ׷���ļ�
VOID CFileClient::AppendFile(LPVOID lpData, DWORD dwSize)
{
    ::WriteFile(m_hFile, lpData, dwSize, &dwSize, NULL);
}