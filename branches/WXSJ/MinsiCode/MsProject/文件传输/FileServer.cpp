#include "Precompiled.h"
#include "FileServer.h"

CFileServer* g_pCFileServer = NULL;
CFileServer::CFileServer(WORD wProt, DWORD dwMaxClient, TCHAR* szPassword)
: m_lpCMsServerNetManager   (NULL)
, m_lpCMsFindFileManager    (NULL)
{
    m_lpCMsPacketFactory = NEW CMsPacketFactory(RR_PACKET_MAX_COUNT);
    AssertEx(m_lpCMsPacketFactory, _T("�ڴ治��"));
    m_lpCMsPacketFactory->Register(g_lpCMsBasePacket, PACKET_MAX_COUNT);
    
    m_lpCMsServerNetManager = NEW CMsServerNetManager(
        _T("0.0.0.0"),
        wProt,
        m_dwThreadId,
        dwMaxClient,
        m_lpCMsPacketFactory
        );
    AssertEx(m_lpCMsServerNetManager, _T("�ڴ治��"));

    if (szPassword)
    {
        _tcscpy_s(m_szPassword, MAX_PATH, szPassword);
    }
}

CFileServer::~CFileServer(VOID)
{
    this->Stop(TRUE);
    SAFE_DELETE(m_lpCMsServerNetManager);
    SAFE_DELETE(m_lpCMsPacketFactory);
}

// ֹ֮ͣǰ
BOOL CFileServer::ForeStop(VOID)
{
    return m_lpCMsServerNetManager->Stop(TRUE);
}

// �߳��¼�֪ͨ
VOID CFileServer::OnEventNotify(E_THREAD_EVENT_NOTIFY eThreadEvent)
{
    switch (eThreadEvent)
    {
    case E_ON_THREAD_RUN:
        {
            m_lpCMsServerNetManager->Run();
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
VOID CFileServer::OnThreadFreeWork(VOID)
{
    return;
}

// ��Ϣ����
VOID CFileServer::OnThreadMsgExecute(LPMSG lpMsg)
{
    switch (lpMsg->message)
    {
    case MSM_SERVER_SOCKET_ON_ENTER:
        {
            CMsClientSocket* lpSocket = (CMsClientSocket*)lpMsg->wParam;
            _tcout << _T("�пͻ������ӷ�����\n");
            _tcout << _T("��ʼ�����ļ�\n");
            BOOL bRet = this->FlyFile(lpSocket->GetPoolIndex(), _T("Repositories.rar"));
            if (bRet)
            {
                _tcout << _T("�����ļ��ɹ�\n");
            }
            else
            {
                _tcout << _T("�����ļ�ʧ��\n");
            }
        }break;

    case MSM_SERVER_SOCKET_ON_EXIT:
        {
            _tcout << _T("�пͻ��˶Ͽ�������\n");
        }break;

    case MSM_MSG_SOCKET_EXECUTE:
        {
            ;
        }break;

    default:
        {
            ;
        }break;
    }
}

// ��ȡ����
TCHAR* CFileServer::GetPassword(VOID)
{
    return m_szPassword;
}

// ���ļ�
BOOL CFileServer::FlyFile(DWORD dwIndex, TCHAR* szFileName)
{
    S2C_FileInfo::MS_PACKET_DATA PacketData;
    HANDLE hFile = ::CreateFile(szFileName, FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING, 0, NULL);
    PacketData.dwFileSize = ::GetFileSize(hFile, NULL);
    _tcscpy_s(PacketData.szFileName, MAX_PATH, szFileName);
    DWORD dwFlySize = sizeof(m_S2C_FlyFileData.m_Data);
    BOOL bRet = FALSE;
    bRet = m_lpCMsServerNetManager->GetClientSocket(dwIndex)->WritePacket(BPT(S2C_FileInfo), &PacketData);

    while (PacketData.dwFileSize)
    {
        if (PacketData.dwFileSize > dwFlySize)
        {
            ::ReadFile(hFile, m_S2C_FlyFileData.m_Data, dwFlySize, &dwFlySize, NULL);
            PacketData.dwFileSize -= dwFlySize;
            _tcout << PacketData.dwFileSize << std::endl;
            while (!bRet)
            {
                bRet = m_lpCMsServerNetManager->GetClientSocket(dwIndex)->WritePacket(BPT(S2C_FlyFile), &m_S2C_FlyFileData);
            }
            bRet = FALSE;
        }
    }
    return TRUE;
}