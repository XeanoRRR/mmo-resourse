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

#pragma once

/////////////////////////////////////////////////////////////////////////////
// CCalendarBar ����

class CCalendarBar : public CWnd
{
// ����
public:
    CCalendarBar();

// ����
protected:
    CMonthCalCtrl m_wndCalendar;
    INT m_nMyCalendarsY;
    CImageList m_Images;

// ��д
public:
    virtual BOOL Create(CONST RECT& rect, CWnd* pParentWnd, UINT nID = (UINT)-1);

// ʵ��
public:
    virtual ~CCalendarBar();

protected:
    afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg VOID OnSize(UINT nType, INT cx, INT cy);
    afx_msg VOID OnPaint();

    DECLARE_MESSAGE_MAP()
};
