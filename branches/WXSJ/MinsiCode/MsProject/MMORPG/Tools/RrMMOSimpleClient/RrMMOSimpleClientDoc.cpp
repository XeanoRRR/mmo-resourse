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

// RrMMOSimpleClientDoc.cpp : CRrMMOSimpleClientDoc ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOSimpleClient.h"

#include "RrMMOSimpleClientDoc.h"

// CRrMMOSimpleClientDoc

IMPLEMENT_DYNCREATE(CRrMMOSimpleClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CRrMMOSimpleClientDoc, CDocument)
    ON_COMMAND(ID_FILE_SEND_MAIL, &CRrMMOSimpleClientDoc::OnFileSendMail)
    ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, &CRrMMOSimpleClientDoc::OnUpdateFileSendMail)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CRrMMOSimpleClientDoc, CDocument)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IRrMMOSimpleClient ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {16F8DBF5-6632-43DD-9834-F96AD0BDC911}
static CONST IID IID_IRrMMOSimpleClient =
{ 0x16F8DBF5, 0x6632, 0x43DD, { 0x98, 0x34, 0xF9, 0x6A, 0xD0, 0xBD, 0xC9, 0x11 } };

BEGIN_INTERFACE_MAP(CRrMMOSimpleClientDoc, CDocument)
    INTERFACE_PART(CRrMMOSimpleClientDoc, IID_IRrMMOSimpleClient, Dispatch)
END_INTERFACE_MAP()


// CRrMMOSimpleClientDoc ����/����

CRrMMOSimpleClientDoc::CRrMMOSimpleClientDoc()
{
    // TODO: �ڴ����һ���Թ������

    EnableAutomation();

    AfxOleLockApp();
}

CRrMMOSimpleClientDoc::~CRrMMOSimpleClientDoc()
{
    AfxOleUnlockApp();
}

BOOL CRrMMOSimpleClientDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // TODO: �ڴ�������³�ʼ������
    // (SDI �ĵ������ø��ĵ�)

    return TRUE;
}




// CRrMMOSimpleClientDoc ���л�

void CRrMMOSimpleClientDoc::Serialize(CArchive& ar)
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


// CRrMMOSimpleClientDoc ���

#ifdef _DEBUG
void CRrMMOSimpleClientDoc::AssertValid() CONST
{
    CDocument::AssertValid();
}

void CRrMMOSimpleClientDoc::Dump(CDumpContext& dc) CONST
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CRrMMOSimpleClientDoc ����
