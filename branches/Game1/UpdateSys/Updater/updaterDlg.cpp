// updaterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "updater.h"
#include "updaterDlg.h"
#include ".\updaterdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CupdaterDlg �Ի���

//�ж��ļ��Ƿ����
bool CheckFile(const char *name)
{
	WIN32_FIND_DATA fdt;
	if( FindFirstFile(name, &fdt)==INVALID_HANDLE_VALUE )
		return false;
	else 
		return true;
}

CupdaterDlg::CupdaterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CupdaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CupdaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_cText);
}

BEGIN_MESSAGE_MAP(CupdaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CupdaterDlg ��Ϣ�������

BOOL CupdaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(1, 1000, NULL);
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CupdaterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CupdaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CupdaterDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	static bool bRemoveOld = false, bRenameExe2Old = false;

	// 10���Ӳ��ɹ���򿪹رհ�ť
	static int num = 0;
	num ++;
	if (num>10)
	{
		KillTimer(1);
		MessageBox("����ʧ�ܣ����ֶ�����ϷĿ¼�е�update.new������Ϊupdate.exe��Ȼ�����ԡ�\n�������������������Ϸ���뵽 www.saga3d.com ���ز�������");
		CDialog::OnOK();
		ShellExecute( 0, 0, "http://www.saga3d.com", 0, 0, SW_SHOWNORMAL );
		return;
	}

	// ���update.new�ļ������ڣ���ִ��
	if (CheckFile("update.new")==false)
	{
		CDialog::OnTimer(nIDEvent);
		return;
	}

	if (bRemoveOld == false)
	{
		remove("update.old");
		bRemoveOld = true;
	}

	if (bRenameExe2Old == false)
	{
		if (rename("update.exe","update.old") == 0)
		{
			bRenameExe2Old = true;
		}
		else
		{
			if (CheckFile("update.exe") == false)
			{
				bRenameExe2Old = true;
			}
			else
			{
				CDialog::OnTimer(nIDEvent);
				return;
			}
		}
	}

	if (rename("update.new","update.exe") == 0)
	{
		KillTimer(1);

		m_cText.SetWindowText("���³ɹ���������������...");

		Sleep(1000);
		CDialog::OnOK();
		WinExec("update.exe",SW_SHOWNORMAL);
		return;
	}

	CDialog::OnTimer(nIDEvent);
}

void CupdaterDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
}

void CupdaterDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

//	CDialog::OnCancel();
}
