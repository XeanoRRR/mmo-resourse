// YaYaDiskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "YaYaDisk.h"
#include "YaYaDiskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYaYaDiskDlg �Ի���




CYaYaDiskDlg::CYaYaDiskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYaYaDiskDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYaYaDiskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CYaYaDiskDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    //ON_BN_CLICKED(IDOK, &CYaYaDiskDlg::OnBnClickedOk)
    ON_STN_CLICKED(IDC_QQ, &CYaYaDiskDlg::OnStnClickedQq)
    ON_STN_CLICKED(IDC_BEACH, &CYaYaDiskDlg::OnStnClickedBeach)
    ON_STN_CLICKED(IDC_WIND, &CYaYaDiskDlg::OnStnClickedWind)
    ON_STN_CLICKED(IDC_QQGAME, &CYaYaDiskDlg::OnStnClickedQqgame)
    ON_STN_CLICKED(IDC_QQ_DANCE, &CYaYaDiskDlg::OnStnClickedQqDance)
    ON_STN_CLICKED(IDC_EXIT, &CYaYaDiskDlg::OnStnClickedExit)
    ON_STN_CLICKED(IDC_SMALL, &CYaYaDiskDlg::OnStnClickedSmall)
END_MESSAGE_MAP()


// CYaYaDiskDlg ��Ϣ�������

BOOL CYaYaDiskDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYaYaDiskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CYaYaDiskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//void CYaYaDiskDlg::OnBnClickedOk()
//{
//    //SECURITY_DESCRIPTOR sd;
//    //PACL                pAcl;
//    //DWORD               cbSid = 96;
//    //SID                 Sid;
//    //TCHAR               RefDomain[DNLEN + 1];
//    //DWORD               cchDomain = DNLEN + 1;
//    //DWORD               dwAclSize = sizeof(ACL) + 1 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + sizeof(Sid);
//    //SID_NAME_USE        Use;
//    //::LookupAccountName(
//    //    NULL,           //   [in]       �������ָ�����ҵ��û��������ĸ�ϵͳ��,ΪNULL��ʾ����ϵͳ
//    //    _T("Everyone"), //   [in]       ���������Ȩ�޵��û�����
//    //    &Sid,           //   [out]      ��ŷ��ص�SIDֵ
//    //    &cbSid,         //   [in,out]   ��ȥ�������趨�Ļ���������,��������ʵ��SID�ĳ���
//    //    RefDomain,      //   [out]      ����
//    //    &cchDomain,     //   [in,out]   ����
//    //    &Use);          //   [out]      �ṹ,����ָʾ�û�������
//
//    //pAcl = (PACL)::HeapAlloc(::GetProcessHeap(), 0, dwAclSize);
//
//    //::InitializeAcl(pAcl, dwAclSize, ACL_REVISION);
//    //::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
//    //::AddAccessAllowedAce(pAcl, ACL_REVISION, GENERIC_ALL, &Sid);
//    ////::AddAccessDeniedAce(pAcl, ACL_REVISION, GENERIC_READ, &Sid);
//    //::SetSecurityDescriptorDacl(&sd, TRUE, pAcl, FALSE);
//    //::SetFileSecurity(_T("K:\\*"), DACL_SECURITY_INFORMATION, &sd);
//
//    HANDLE hPtoken;
//    if (::LogonUser(_T("Minsi"), _T("RrServer\\"), _T("XJBCEMLSPD"), LOGON32_LOGON_NETWORK_CLEARTEXT, LOGON32_PROVIDER_DEFAULT, &hPtoken))
//    {
//        AfxMessageBox(_T("��½�ɹ�"));
//    }
//    else
//    {
//        INT a = ::GetLastError();
//        AfxMessageBox(_T("��½ʧ��"));
//    }
//    //::CreateProcessAsUser(hPtoken, _T("H:\\����Ӱ��\\Storm.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
//
//    //PSID    pSid = NULL;                // ����һ��ָ��SID�ṹ��ָ��
//    //DWORD   cbSid;                      // ���SID�ĳ���
//    //TCHAR   RefDomain[DNLEN + 1];       // һ��������������������������
//    //DWORD   cchDomain = DNLEN + 1;      // ���Դ��뻺�������ַ��ĸ���
//    //SID_NAME_USE    peUse;              // һ��ö������,����ָ��SID������
//    //SECURITY_DESCRIPTOR sd;             // ����������ȫ���ԵĽṹ
//    //PACL    pDacl = NULL;               // ָ��Ȩ�޿����б��ָ��
//    //DWORD   dwAclSize;                  // �б�ĳ���
//
//    //cbSid   = 96;                       // �趨SID�ĳ���Ϊ96
//    //pSid    = (PSID)::HeapAlloc(::GetProcessHeap(), 0, cbSid);    // ΪSID����ռ�
//    //if(pSid == NULL)                    // ������
//    //{     
//    //    AfxMessageBox(_T("HeapAlloc Error!\n"));
//    //}                 
//
//    //// �õ����Է��ʴ���Դ���û������SID
//    //if(!::LookupAccountName(
//    //    NULL,           //   [in]       �������ָ�����ҵ��û��������ĸ�ϵͳ��,ΪNULL��ʾ����ϵͳ
//    //    _T("Everyone"), //   [in]       ���������Ȩ�޵��û�����
//    //    pSid,           //   [out]      ��ŷ��ص�SIDֵ
//    //    &cbSid,         //   [in,out]   ��ȥ�������趨�Ļ���������,��������ʵ��SID�ĳ���
//    //    RefDomain,      //   [out]      ����
//    //    &cchDomain,     //   [in,out]   ����
//    //    &peUse))        //   [out]      �ṹ,����ָʾ�û�������
//    //{
//    //    if(::GetLastError() == ERROR_INSUFFICIENT_BUFFER)   // �������������
//    //    {                                 
//    //        pSid = (PSID)::HeapReAlloc(::GetProcessHeap(), 0, pSid, cbSid);
//    //        if(pSid == NULL)
//    //        {
//    //            AfxMessageBox(_T("HeapAlloc Error!\n"));
//    //        }
//    //        cchDomain = DNLEN + 1;
//    //        if(!::LookupAccountName(
//    //            NULL,
//    //            _T("Everyone"),
//    //            pSid,
//    //            &cbSid,
//    //            RefDomain,
//    //            &cchDomain,
//    //            &peUse))
//    //        {
//    //            AfxMessageBox(_T("LookupAccountName Error! (rc=%lu)\n"), ::GetLastError());
//    //        }
//    //    }
//    //    else
//    //    {
//    //        AfxMessageBox(_T("LookupAccountName Error! (rc=%lu)\n"), ::GetLastError());
//    //    }
//    //}
//
//    //// ����Ȩ�޿����б����賤��
//    //dwAclSize = sizeof(ACL) + 1 * (sizeof(ACCESS_ALLOWED_ACE) - sizeof(DWORD)) + ::GetLengthSid(pSid);
//
//    //// ΪpDacl��������Ŀռ�
//    //pDacl = (PACL)::HeapAlloc(::GetProcessHeap(), 0, dwAclSize);
//    //if(pDacl == NULL) return;
//
//    //// ��pDacl��ʼ��
//    //::InitializeAcl(pDacl, dwAclSize, ACL_REVISION);
//
//    //// ����GENERIC_ALLȨ�����ڵõ���SID,�����뵽�б���
//    //::AddAccessAllowedAce(pDacl, ACL_REVISION, GENERIC_ALL, pSid);
//
//    //// ��ʼ�� SECURITY_DESCRIPTOR �ṹ��ʵ��sd
//    //::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
//
//    //// ��pDacl���뵽sd��     
//    //::SetSecurityDescriptorDacl(&sd, TRUE, pDacl, FALSE);       
//}

void CYaYaDiskDlg::OnStnClickedQq()
{
    //TCHAR szDirBuff[MAX_PATH] = {0};
    //::GetCurrentDirectory(MAX_PATH, szDirBuff);
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    PROCESS_INFORMATION ProcessInfo;  // ������Ϣ
    //_tcscat_s(szDirBuff, _T("��Ѷ\\QQ\\Bin\\QQ.exe"));
    ::CreateProcess(_T(".\\��Ѷ\\QQ\\Bin\\QQ.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
    this->ShowWindow(SW_MINIMIZE);
}

void CYaYaDiskDlg::OnStnClickedBeach()
{
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    PROCESS_INFORMATION ProcessInfo;
    ::CreateProcess(_T(".\\�Ըк�̲3\\SexyBeach3PlusCN.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
    this->ShowWindow(SW_MINIMIZE);
}

void CYaYaDiskDlg::OnStnClickedWind()
{
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    PROCESS_INFORMATION ProcessInfo;
    ::CreateProcess(_T(".\\����Ӱ��\\Storm.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
    this->ShowWindow(SW_MINIMIZE);
}

void CYaYaDiskDlg::OnStnClickedQqgame()
{
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    PROCESS_INFORMATION ProcessInfo;
    ::CreateProcess(_T(".\\��Ѷ\\QQGAME\\QQGame.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
    this->ShowWindow(SW_MINIMIZE);
}

void CYaYaDiskDlg::OnStnClickedQqDance()
{
    STARTUPINFOW StartInfo;
    ZeroMemory(&StartInfo, sizeof(StartInfo));
    StartInfo.cb = sizeof(StartInfo);
    PROCESS_INFORMATION ProcessInfo;
    ::CreateProcess(_T(".\\��Ѷ\\QQ����\\bin\\QQ����.exe"), _T(""), NULL, NULL, FALSE, 0, NULL, NULL, &StartInfo, &ProcessInfo);
    this->ShowWindow(SW_MINIMIZE);
}

void CYaYaDiskDlg::OnStnClickedExit()
{
    this->EndDialog(0);
}

void CYaYaDiskDlg::OnStnClickedSmall()
{
    this->ShowWindow(SW_MINIMIZE);
}
