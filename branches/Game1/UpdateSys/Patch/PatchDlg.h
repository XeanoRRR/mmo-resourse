// PatchDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "outputexplorer.h"
struct tagFileInfo;
class CFilesInfo;
class CPackage;
class CClientResource;

// CPatchDlg �Ի���
class CPatchDlg : public CDialog
{
// ����
public:
	CPatchDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CPatchDlg();

// �Ի�������
	enum { IDD = IDD_PATCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CFilesInfo*			m_pPatchFilesInfo;		//�������ļ���Ϣ
	CPackage*			m_pPatchPackage;		//�����İ�����
	CClientResource*	m_pClientResource;		//�ͻ�����Դ

	string				m_strWorkPath;
	string				m_strExePath;			//ִ���ļ�Ŀ¼
	string				m_strFilesInfoName;		//�ļ���Ϣ����

	string				m_strTemptPackageName;	//��ʱ����
	string				m_strTemptFilesInfoName;//��ʱ�ļ�����

	string				m_listStrUpdatedVersion;	//�������İ汾��

	BOOL	InitialInfo();						//��ʼ���ͻ�����Դ��Ϣ
	void	OnUpdateDisplay();					//������ʾ
	BOOL	SetPatchData();						//��������������
	BOOL	GetPatchData();						//�õ�����������
	//BOOL	GetPatchInfo();						//�õ�����������
	BOOL	CheckGameRunFolder();				//����Ƿ�����Ϸ����Ŀ¼
	afx_msg void OnBnClickedButtonSelectfolder();



	static unsigned __stdcall UpdateProc(void* pParam);
	static BOOL	 LoadClientResource(CPatchDlg* pPatchDlg);
	static BOOL  UpdateClientResource(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,BYTE*& pBuf,DWORD& dwBuflen,
							CPackage* pPackage,CClientResource* pClientResource,HWND hwnd,int& nTotalSize);

	static void SetDisplayInfo(HWND hwnd,CString strText,int nPos,int nMax=-1);
	static void	SetDisplayInfo(HWND hwnd,int nPos,int nMax=-1);

	
private:
	CString m_strGameRunFolder;
public:
	afx_msg void OnEnChangeEditGamerunfolder();
	afx_msg void OnBnClickedUpdate();
	CString m_strDisplayInfo;
	CProgressCtrl m_Progress;
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedCancel();

	afx_msg void OnStnClickedStaticTextinfo();
	CEdit m_OutPutInfoEdit;
	afx_msg void OnDestroy();
};

void PutDebugStringMsg(const char *msg);
