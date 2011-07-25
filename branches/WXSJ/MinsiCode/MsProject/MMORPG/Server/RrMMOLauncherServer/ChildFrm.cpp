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

// ChildFrm.cpp : CChildFrame ���ʵ��
//

#include "Precompiled.h"
#include "RrMMOLauncherServer.h"

#include "ChildFrm.h"

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
    ON_COMMAND(ID_FILE_PRINT, &CChildFrame::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_DIRECT, &CChildFrame::OnFilePrint)
    ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnFilePrintPreview)
    ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, &CChildFrame::OnUpdateFilePrintPreview)
END_MESSAGE_MAP()

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
    // TODO: �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
    if( !CMDIChildWndEx::PreCreateWindow(cs) )
        return FALSE;

    return TRUE;
}

// CChildFrame ���

#ifdef _DEBUG
VOID CChildFrame::AssertValid() CONST
{
    CMDIChildWndEx::AssertValid();
}

VOID CChildFrame::Dump(CDumpContext& dc) CONST
{
    CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame ��Ϣ�������

VOID CChildFrame::OnFilePrint()
{
    if (m_dockManager.IsPrintPreviewValid())
    {
        PostMessage(WM_COMMAND, AFX_ID_PREVIEW_PRINT);
    }
}

VOID CChildFrame::OnFilePrintPreview()
{
    if (m_dockManager.IsPrintPreviewValid())
    {
        PostMessage(WM_COMMAND, AFX_ID_PREVIEW_CLOSE);  // ǿ�ƹرա���ӡԤ����ģʽ
    }
}

VOID CChildFrame::OnUpdateFilePrintPreview(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_dockManager.IsPrintPreviewValid());
}
