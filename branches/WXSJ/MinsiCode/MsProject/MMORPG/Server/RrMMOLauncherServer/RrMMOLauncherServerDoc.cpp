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

// RrMMOLauncherServerDoc.cpp : CRrMMOLauncherServerDoc ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOLauncherServer.h"

#include "RrMMOLauncherServerDoc.h"

// CRrMMOLauncherServerDoc

IMPLEMENT_DYNCREATE(CRrMMOLauncherServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CRrMMOLauncherServerDoc, CDocument)
    ON_COMMAND(ID_FILE_SEND_MAIL, &CRrMMOLauncherServerDoc::OnFileSendMail)
    ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CRrMMOLauncherServerDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRrMMOLauncherServerDoc, CDocument)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IRrMMOLauncherServer ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {16F8DBF5-6632-43DD-9834-F96AD0BDC911}
static CONST IID IID_IRrMMOLauncherServer =
{ 0x16F8DBF5, 0x6632, 0x43DD, { 0x98, 0x34, 0xF9, 0x6A, 0xD0, 0xBD, 0xC9, 0x11 } };

BEGIN_INTERFACE_MAP(CRrMMOLauncherServerDoc, CDocument)
    INTERFACE_PART(CRrMMOLauncherServerDoc, IID_IRrMMOLauncherServer, Dispatch)
END_INTERFACE_MAP()


// CRrMMOLauncherServerDoc ����/����

CRrMMOLauncherServerDoc::CRrMMOLauncherServerDoc()
{
    // TODO: �ڴ����һ���Թ������

    EnableAutomation();

    AfxOleLockApp();
}

CRrMMOLauncherServerDoc::~CRrMMOLauncherServerDoc()
{
    AfxOleUnlockApp();
}

BOOL CRrMMOLauncherServerDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: �ڴ�������³�ʼ������
    // (SDI �ĵ������ø��ĵ�)

    return TRUE;
}




// CRrMMOLauncherServerDoc ���л�

VOID CRrMMOLauncherServerDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // TODO: �ڴ���Ӵ洢����
    }
    else
    {
        // TODO: �ڴ���Ӽ��ش���
    }
}


// CRrMMOLauncherServerDoc ���

#ifdef _DEBUG
VOID CRrMMOLauncherServerDoc::AssertValid() CONST
{
    CDocument::AssertValid();
}

VOID CRrMMOLauncherServerDoc::Dump(CDumpContext& dc) CONST
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CRrMMOLauncherServerDoc ����
