// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û�����
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ��
// ���Բ��䡶Microsoft ������ο�����
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// RrMMOSimpleClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "Precompiled.h"
#include "afxwinappex.h"
#include "RrMMOSimpleClient.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "RrMMOSimpleClientDoc.h"
#include "RrMMOSimpleClientView.h"

// CRrMMOSimpleClientApp

BEGIN_MESSAGE_MAP(CRrMMOSimpleClientApp, CWinAppEx)
    ON_COMMAND(ID_APP_ABOUT, &CRrMMOSimpleClientApp::OnAppAbout)
    // �����ļ��ı�׼�ĵ�����
    ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
    // ��׼��ӡ��������
    ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CRrMMOSimpleClientApp ����

CRrMMOSimpleClientApp::CRrMMOSimpleClientApp()
{

    m_bHiColorIcons = TRUE;

    // TODO: �ڴ˴���ӹ�����룬
    // ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CRrMMOSimpleClientApp ����

CRrMMOSimpleClientApp theApp;
// ���ɵĴ˱�ʶ����ͳ���϶�������Ӧ�ó�����Ψһ��
// �������Ըѡ��һ���ض��ı�ʶ��������Ը�����

// {54913426-1B7F-4EFB-A4C9-0A00628AD316}
static CONST CLSID clsid =
{ 0x54913426, 0x1B7F, 0x4EFB, { 0xA4, 0xC9, 0xA, 0x0, 0x62, 0x8A, 0xD3, 0x16 } };

CONST GUID CDECL BASED_CODE _tlid =
        { 0x5B178151, 0xD3A0, 0x4F01, { 0xBB, 0x26, 0x12, 0x27, 0xAB, 0xE1, 0xA9, 0x79 } };
CONST WORD _wVerMajor = 1;
CONST WORD _wVerMinor = 0;


// CRrMMOSimpleClientApp ��ʼ��

BOOL CRrMMOSimpleClientApp::InitInstance()
{
    CWinAppEx::InitInstance();

    if (!AfxSocketInit())
    {
        AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
        return FALSE;
    }

    // ��ʼ�� OLE ��
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }
    AfxEnableControlContainer();
    // ��׼��ʼ��
    // ���δʹ����Щ���ܲ�ϣ����С
    // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
    // ����Ҫ���ض���ʼ������
    // �������ڴ洢���õ�ע�����
    // TODO: Ӧ�ʵ��޸ĸ��ַ�����
    // �����޸�Ϊ��˾����֯��
    SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
    LoadStdProfileSettings(0);  // ���ر�׼ INI �ļ�ѡ��

    InitContextMenuManager();
    InitShellManager();

    InitKeyboardManager();

    InitTooltipManager();
    CMFCToolTipInfo ttParams;
    ttParams.m_bVislManagerTheme = TRUE;
    theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
        RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

    // ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
    // �������ĵ�����ܴ��ں���ͼ֮�������
    CMultiDocTemplate* pDocTemplate;
    pDocTemplate = new CMultiDocTemplate(IDR_RRMMOSIMPLECLIETYPE,
        RUNTIME_CLASS(CRrMMOSimpleClientDoc),
        RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
        RUNTIME_CLASS(CRrMMOSimpleClientView));
    if (!pDocTemplate)
        return FALSE;
    AddDocTemplate(pDocTemplate);
    // �� COleTemplateServer ���ӵ��ĵ�ģ��
    // COleTemplateServer ͨ��ʹ��
    // �ĵ�ģ����ָ������Ϣ��Ϊ���� OLE ����
    // �������ĵ�
    m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
    // �����е� OLE ����������ע��Ϊ�������С��⽫����
    //  OLE ���Դ�����Ӧ�ó����д�������
    COleTemplateServer::RegisterAll();
        // ע��: MDI Ӧ�ó���ע�����еķ��������󣬶�����
        // �������ϵ� /Embedding �� /Automation

    // ������ MDI ��ܴ���
    CMainFrame* pMainFrame = new CMainFrame;
    if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
    {
        delete pMainFrame;
        return FALSE;
    }
    m_pMainWnd = pMainFrame;
    // �������к�׺ʱ�ŵ��� DragAcceptFiles
    //  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������


    // ������׼������DDE�����ļ�������������
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Ӧ�ó������� /Embedding �� /Automation ���������ġ�
    //ʹӦ�ó�����Ϊ�Զ������������С�
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        // ����ʾ������
        return TRUE;
    }
    // ʹ�� /Unregserver �� /Unregister ��������Ӧ�ó���ע��
    // ���Ϳ⡣����ע�������� ProcessShellCommand() �з�����
    else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
    {
        m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, NULL, NULL, FALSE);
        AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
    }
    // Ӧ�ó������Զ�����ʽ������������(�� /Register
    // �� /Regserver)�����ġ�����ע�����������Ϳ⡣
    else
    {
        m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
        COleObjectFactory::UpdateRegistryAll();
        AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
    }

    // ��������������ָ����������
    // �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;
    // �������ѳ�ʼ���������ʾ����������и���
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// �Ի�������
    enum { IDD = IDD_ABOUTBOX };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void CRrMMOSimpleClientApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

// CRrMMOSimpleClientApp �Զ������/���淽��

void CRrMMOSimpleClientApp::PreLoadState()
{
    BOOL bNameValid;
    CString strName;
    bNameValid = strName.LoadString(IDS_EDIT_MENU);
    ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
    bNameValid = strName.LoadString(IDS_EXPLORER);
    ASSERT(bNameValid);
    GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CRrMMOSimpleClientApp::LoadCustomState()
{
}

void CRrMMOSimpleClientApp::SaveCustomState()
{
}

// CRrMMOSimpleClientApp ��Ϣ�������



