#pragma once
#include "afxwin.h"


// CShapePositionToDlg �Ի���

class CShapePositionToDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapePositionToDlg)

public:
	CShapePositionToDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShapePositionToDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ShapePosition };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	long lPosX;
public:
	long lPosY;
public:
	afx_msg void OnSetpositionto();
public:
	CEdit m_PosX;
public:
	CEdit m_PosY;
};
