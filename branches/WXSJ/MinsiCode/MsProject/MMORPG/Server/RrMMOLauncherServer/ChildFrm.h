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

// ChildFrm.h : CChildFrame ��Ľӿ�
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
    DECLARE_DYNCREATE(CChildFrame)
public:
    CChildFrame();

// ����
public:

// ����
public:

// ��д
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
    virtual ~CChildFrame();
#ifdef _DEBUG
    virtual VOID AssertValid() CONST;
    virtual VOID Dump(CDumpContext& dc) CONST;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
    afx_msg VOID OnFilePrint();
    afx_msg VOID OnFilePrintPreview();
    afx_msg VOID OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
    DECLARE_MESSAGE_MAP()
};
