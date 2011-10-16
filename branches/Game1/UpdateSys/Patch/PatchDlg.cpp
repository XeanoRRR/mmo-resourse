// PatchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Patch.h"
#include "PatchDlg.h"

#include "../../PUBLIC/ClientResource.h"
#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/FilesInfo.h"
#include "../../PUBLIC/ziplib/zlib.h"
#include "../../PUBLIC/ziplib/unzip.h"
#include ".\patchdlg.h"
#include "../../public/tools.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DISPLAY_UPDATE	WM_USER+6000

void PutDebugStringMsg(const char *msg)
{
	AfxMessageBox(msg);
}

struct tagDisplayInfo
{
	CString	strText;		//�ı���ʾֵ
	int		nMax;			//���������ֵ
	int		nCur;			//��������ǰֵ
};
tagDisplayInfo	gDisplayInfo;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
HANDLE hThread=0;
bool bExit = false;

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


// CPatchDlg �Ի���



CPatchDlg::CPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchDlg::IDD, pParent)
	,m_pPatchFilesInfo(NULL)
	,m_pPatchPackage(NULL)
	,m_pClientResource(NULL)
	, m_strWorkPath("")
	, m_strGameRunFolder(_T(""))
	, m_strDisplayInfo(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CPatchDlg::~CPatchDlg()
{
	if(m_pPatchFilesInfo)	delete m_pPatchFilesInfo;
	if(m_pPatchPackage) delete m_pPatchPackage;
	if(m_pClientResource) delete m_pClientResource;

#ifndef	_DEBUG
	DeleteFile(m_strTemptPackageName.c_str());
	DeleteFile(m_strTemptFilesInfoName.c_str());
	::RemoveDirectory(m_strExePath.c_str());
#endif
}

void CPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//	DDX_Text(pDX, IDC_EDIT_GAMERUNFOLDER, m_strGameRunFolder);
	DDX_Text(pDX, IDC_STATIC_TEXTINFO, m_strDisplayInfo);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_OUTPUTINFO, m_OutPutInfoEdit);
}

BEGIN_MESSAGE_MAP(CPatchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_BN_CLICKED(IDC_BUTTON_SELECTFOLDER, OnBnClickedButtonSelectfolder)
//	ON_EN_CHANGE(IDC_EDIT_GAMERUNFOLDER, OnEnChangeEditGamerunfolder)
	ON_BN_CLICKED(ID_UPDATE, OnBnClickedUpdate)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_STN_CLICKED(IDC_STATIC_TEXTINFO, &CPatchDlg::OnStnClickedStaticTextinfo)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPatchDlg ��Ϣ�������

BOOL CPatchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ʼ�����Ϣ
	if(FALSE == InitialInfo())
		return FALSE;

	if(CheckGameRunFolder())
	{
		GetDlgItem(ID_UPDATE)->EnableWindow(TRUE);
		m_strGameRunFolder= CString(m_strWorkPath.c_str());
	}
	else
	{
		GetDlgItem(ID_UPDATE)->EnableWindow(FALSE);
		m_strWorkPath="";
	}

	char strTitle[200];
	sprintf(strTitle,"������Ϸ");

	SetWindowText(strTitle);

	UpdateData(false);


	HGLOBAL  hResLoad;     // handle to loaded resource 
	HRSRC hRes;         // handle/ptr. to res. info. in hExe 
	VOID *lpResLock;    // pointer to resource data 

	//#ifndef _DEBUG
	//�õ����ļ�����
	hRes = ::FindResource(NULL,MAKEINTRESOURCE(IDR_PATCH1),"PATCH");
	if( hRes == NULL ) 
	{ 
		DWORD dwError = GetLastError();
		return FALSE; 
	} 

	hResLoad = ::LoadResource(NULL, hRes); 
	if (hResLoad == NULL) 
	{ 
		return FALSE;
	} 

	// Lock the dialog box into global memory. 
	lpResLock = ::LockResource(hResLoad); 
	if (lpResLock == NULL) 
	{ 
		return FALSE;
	} 

	string tmpZipFileNmae = m_strWorkPath + "\\updatefiles\\tmp.zip";
	FILE* pf = fopen(tmpZipFileNmae.c_str(),"wb");
	if(!pf)
	{	
		char str[100]="";
		sprintf(str,"��ʼ���Ի��򴴽���ʱZIP�ļ� %s ����",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}	

	DWORD dwSize = SizeofResource(NULL,hRes);
	DWORD dwCount = fwrite(lpResLock,dwSize,1, pf);
	if(dwCount != 1)
	{
		char str[100]="";
		sprintf(str,"��ʼ���Ի���д��ʱZIP�ļ� %s ���ݳ��������ԡ�",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}

	int nRet = fflush(pf);
	if(nRet != 0)
	{
		char str[100]="";
		sprintf(str,"��ʼ���Ի��򴴽���ʱZIP�ļ� %s ���������ԡ�",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}
	fclose(pf);

	//��ѹ������
	unzFile dataZipFile = unzOpen(tmpZipFileNmae.c_str());
	if(dataZipFile)
	{
		unz_file_info zipFileInfo;
		// �ҵ�Update.html
		if( UNZ_OK == unzLocateFile(dataZipFile, "Update.txt", 1) )
		{
			unzGetCurrentFileInfo(dataZipFile, &zipFileInfo, NULL, 0,
				NULL, 0, NULL, 0);

			string tmpUpdateTxtPath = m_strWorkPath + "\\updatefiles\\Update.txt";

			char* buf = new char[zipFileInfo.uncompressed_size+1];
			unzOpenCurrentFile(dataZipFile);
			unzReadCurrentFile(dataZipFile, buf, zipFileInfo.uncompressed_size);

			FILE* file = fopen(tmpUpdateTxtPath.c_str(), "wb");
			if(file)
			{
				fwrite(buf, zipFileInfo.uncompressed_size, 1, file);
				buf[zipFileInfo.uncompressed_size] = '\0';
				fclose(file);
			}
			unzCloseCurrentFile(dataZipFile);
			m_OutPutInfoEdit.SetWindowText(buf);
			SAFE_DELETE_ARRAY(buf);
		}
	}
	unzClose(dataZipFile);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CPatchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam); 
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPatchDlg::OnPaint() 
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
		CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

		CRect rect;
		GetClientRect(&rect);

		//CDC memDC;
		//memDC.CreateCompatibleDC(&dc);
		//CBitmap bpt;
		//bpt.LoadBitmap(IDB_BITMAP1);
		//BITMAP bm;
		//bpt.GetBitmap(&bm);
		//memDC.SelectObject(&bpt);
		//dc.BitBlt(10,10, bm.bmWidth , bm.bmHeight , &memDC, 0, 0, SRCCOPY);
		//����������

		//CFont myFont;
		//myFont.CreatePointFont(350,"����");

		//CFont *OldFont = dc.SelectObject(&myFont);
		//dc.SetTextColor(RGB(255,0,0));

		////����CHECT˵���ľ��δ���
		//CRect myRect;
		//myRect.left=rect.left+80;
		//myRect.top=rect.top+10;
		//myRect.right=rect.right-10;
		//myRect.bottom=rect.bottom/3;

		////����͸���ķ�ʽ����Ļ��д��
		//dc.SetBkMode(TRANSPARENT);
		//dc.DrawText("��  ��  ��  ��",&myRect,DT_LEFT);

		//dc.SelectObject(OldFont);
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CPatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ʼ���ͻ�����Դ��Ϣ
BOOL	CPatchDlg::InitialInfo()
{
	// ���ҿͻ���·��,ע���
	//ע�����Ϣ
	string strReg("HKEY_LOCAL_MACHINE/SOFTWARE/xingman/Luvinia");

	HKEY hKey;
	DWORD dwBufLen =2*MAX_PATH+10;
	char szKeyValue[2*MAX_PATH+10];
	memset(szKeyValue, 0, 2*MAX_PATH+10);

	LONG lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		"SOFTWARE\\XINGMAN\\LUVINIA",
		0, KEY_QUERY_VALUE, &hKey );


	lRet = RegQueryValueEx( hKey, "Client_Path", NULL, NULL,
		(LPBYTE)szKeyValue, &dwBufLen);

	RegCloseKey( hKey );

	if(szKeyValue[0] != '\0')
	{
		m_strExePath = string(szKeyValue);
		m_strWorkPath = m_strExePath;
	}

	bExit = false;

	string strGameExeName = m_strWorkPath + "\\PackRes.cfg";
	
	// ����GameClient.exe
	WIN32_FIND_DATA stFindData;
	HANDLE hFind = FindFirstFile(strGameExeName.c_str(), &stFindData);
	if (hFind==INVALID_HANDLE_VALUE)
	{
		CHAR* strPath = new CHAR[MAX_PATH];
		::GetSystemDirectory(strPath,MAX_PATH);
		string strTemptr(strPath);
		SAFE_DELETE_ARRAY(strPath);

		string strTemptFoler;strTemptFoler.clear();
		strTemptFoler.insert(0,strTemptr,0,2);

		////#ifdef	_DEBUG
		CHAR* strWorkPath = new CHAR[MAX_PATH];
		if(::GetCurrentDirectory(MAX_PATH,strWorkPath))
		{
			m_strExePath = string(strWorkPath);
			m_strWorkPath = m_strExePath;
		}
		strGameExeName = m_strWorkPath + "\\PackRes.cfg";
		SAFE_DELETE_ARRAY(strWorkPath);

		HANDLE hFind = FindFirstFile(strGameExeName.c_str(), &stFindData);
		if (hFind==INVALID_HANDLE_VALUE)
		{
			MessageBox("�뽫���°���������Ϸ��Ŀ¼��!", "����!", MB_OK);
			return false;
		}
	}

	string tmpUpdatePath = m_strWorkPath + "\\updatefiles";
	//"gameclient"
	::CreateDirectory(tmpUpdatePath.c_str(), NULL);
	
	m_strFilesInfoName="PackRes.cfg";

	m_strTemptFilesInfoName=m_strExePath+"\\Patch.cfg";	//��ʱ��
	m_strTemptPackageName=m_strExePath+"\\Patch.dp";//��ʱ�ļ�����

	return true;
}

BOOL	CPatchDlg::GetPatchData()
{	
	HGLOBAL  hResLoad;     // handle to loaded resource 
	HRSRC hRes;         // handle/ptr. to res. info. in hExe 
	VOID *lpResLock;    // pointer to resource data 

	//#ifndef _DEBUG
	//�õ����ļ�����
	hRes = ::FindResource(NULL,MAKEINTRESOURCE(IDR_PATCH1),"PATCH");
	if( hRes == NULL ) 
	{ 
		DWORD dwError = GetLastError();
		return FALSE; 
	} 

	hResLoad = ::LoadResource(NULL, hRes); 
	if (hResLoad == NULL) 
	{ 
		return FALSE;
	} 

	// Lock the dialog box into global memory. 
	lpResLock = ::LockResource(hResLoad); 
	if (lpResLock == NULL) 
	{ 
		return FALSE;
	} 

	string tmpZipFileNmae = m_strWorkPath + "\\updatefiles\\tmp.zip";
	FILE* pf = fopen(tmpZipFileNmae.c_str(),"wb");
	if(!pf)
	{	
		char str[100]="";
		sprintf(str,"������ʱZIP�ļ� %s ����",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}	

	DWORD dwSize = SizeofResource(NULL,hRes);
	DWORD dwCount = fwrite(lpResLock,dwSize,1, pf);
	if(dwCount != 1)
	{
		char str[100]="";
		sprintf(str,"д��ʱZIP�ļ� %s ���ݳ��������ԡ�",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}

	int nRet = fflush(pf);
	if(nRet != 0)
	{
		char str[100]="";
		sprintf(str,"������ʱZIP�ļ� %s ���������ԡ�",tmpZipFileNmae.c_str());
		AfxMessageBox(str);
		return FALSE;
	}
	fclose(pf);
	SetDisplayInfo(this->m_hWnd,"��ѹ�õ�����...",16,20);
	PutDebugString("��ѹ�õ�����...");

	//��ѹ������
	unzFile dataZipFile = unzOpen(tmpZipFileNmae.c_str());
	if(dataZipFile)
	{
		unz_file_info zipFileInfo;
		//// �ҵ�Update.html
		//if( UNZ_OK == unzLocateFile(dataZipFile, "Update.txt", 1) )
		//{
		//	unzGetCurrentFileInfo(dataZipFile, &zipFileInfo, NULL, 0,
		//		NULL, 0, NULL, 0);

		//	string tmpUpdateTxtPath = m_strWorkPath + "\\updatefiles\\Update.txt";
		//	
		//	char* buf = new char[zipFileInfo.uncompressed_size+1];
		//	unzOpenCurrentFile(dataZipFile);
		//	unzReadCurrentFile(dataZipFile, buf, zipFileInfo.uncompressed_size);
		//	
		//	FILE* file = fopen(tmpUpdateTxtPath.c_str(), "wb");
		//	if(file)
		//	{
		//		fwrite(buf, zipFileInfo.uncompressed_size, 1, file);
		//		buf[zipFileInfo.uncompressed_size] = '\0';
		//		fclose(file);
		//	}
		//	unzCloseCurrentFile(dataZipFile);

		//	FILE* outInfoFile = fopen(tmpUpdateTxtPath.c_str(), "rb");
		//	if(outInfoFile)
		//	{
		//		char fileBuf[5120];
		//		memset(fileBuf, 0, 5120);
		//		fread(fileBuf, 5120, 1, outInfoFile);
		//		SetDlgItemText(IDC_OUTPUTINFO, &fileBuf[0]);
		//	}

		//	SAFE_DELETE_ARRAY(buf);
		//}
		// �ҵ�Patch.cfg
		if( UNZ_OK == unzLocateFile(dataZipFile, "Patch.cfg", 1) )
		{
			unzGetCurrentFileInfo(dataZipFile, &zipFileInfo, NULL, 0,
				NULL, 0, NULL, 0);

			char* buf = new char[zipFileInfo.uncompressed_size];
			unzOpenCurrentFile(dataZipFile);
			unzReadCurrentFile(dataZipFile, buf, zipFileInfo.uncompressed_size);
			string tmpUpdatePath = m_strWorkPath + "\\updatefiles\\Patch.cfg";
			FILE* file = fopen(tmpUpdatePath.c_str(), "wb");
			if(file)
			{
				fwrite(buf, zipFileInfo.uncompressed_size, 1, file);
				fclose(file);
			}
			unzCloseCurrentFile(dataZipFile);
			SAFE_DELETE_ARRAY(buf);
		}

		// �ҵ�Patch.pak
		if( UNZ_OK == unzLocateFile(dataZipFile, "Patch.dp", 1) )
		{
			unzGetCurrentFileInfo(dataZipFile, &zipFileInfo, NULL, 0,
				NULL, 0, NULL, 0);

			char* buf = new char[zipFileInfo.uncompressed_size];
			unzOpenCurrentFile(dataZipFile);
			unzReadCurrentFile(dataZipFile, buf, zipFileInfo.uncompressed_size);
			string tmpUpdatePath = m_strWorkPath + "\\updatefiles\\Patch.dp";
			FILE* file = fopen(tmpUpdatePath.c_str(), "wb");
			if(file)
			{
				fwrite(buf, zipFileInfo.uncompressed_size, 1, file);
				fclose(file);
			}
			unzCloseCurrentFile(dataZipFile);
			SAFE_DELETE_ARRAY(buf);
		}

		if(!m_pPatchFilesInfo)
			m_pPatchFilesInfo = new CFilesInfo;
		m_pPatchFilesInfo->Clear();
		string tmpPatchRilPath = m_strWorkPath + "\\updatefiles\\Patch.cfg";
		m_pPatchFilesInfo->Load(tmpPatchRilPath);

		SetDisplayInfo(this->m_hWnd,"��ѹ�õ�����...",19,20);
		PutDebugString("��ѹ�õ�����...");

		if(!m_pPatchPackage)
			m_pPatchPackage = new CPackage(NULL,m_strExePath);
		m_pPatchPackage->Open("\\updatefiles\\Patch");
	}

	unzClose(dataZipFile);

	//�õ��������İ汾����
	hRes = ::FindResource(NULL,MAKEINTRESOURCE(IDR_PATCH2),"PATCH");
	if( hRes == NULL ) 
	{ 
		DWORD dwError = GetLastError();
		return FALSE; 
	} 

	hResLoad = ::LoadResource(NULL, hRes); 
	if (hResLoad == NULL) 
	{ 
		return FALSE;
	} 

	lpResLock = ::LockResource(hResLoad); 
	if (lpResLock == NULL) 
	{ 
		return FALSE;
	}
	m_listStrUpdatedVersion = (char*)lpResLock;	
	return true;
}

//��������������
BOOL	CPatchDlg::SetPatchData()
{

	BOOL result;
	HANDLE hUpdateRes;  // update resource handle 

	hUpdateRes = BeginUpdateResource(NULL, TRUE); 
	if (hUpdateRes == NULL) 
	{ 
		return FALSE; 
	} 

	//�Ѱ�����д��ʱ�ļ�
	HANDLE hFile = CreateFile("test.txt",
		GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

	if (hFile == INVALID_HANDLE_VALUE) { 
		return FALSE; 
	} 

	DWORD dwSize = ::GetFileSize(hFile,NULL);
	BYTE* pBuf = new BYTE[dwSize+1];
	DWORD dwReadSize;
	result = ReadFile(hFile,pBuf,dwSize,&dwReadSize,NULL);
	if( !result )
	{
		return FALSE;
		delete []pBuf;
	}
	pBuf[dwSize]='\0';

	result = UpdateResource(hUpdateRes,"PATCH",
		MAKEINTRESOURCE(IDR_PATCH1), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		pBuf,
		dwSize);

	delete []pBuf;
	if (result == FALSE) 
	{ 
		return FALSE;
	} 

	// Write changes to FOOT.EXE and then close it. 
	if (!EndUpdateResource(hUpdateRes, FALSE)) 
	{ 
		return FALSE; 
	} 
	return TRUE;
}
void CPatchDlg::OnBnClickedButtonSelectfolder()
{
	//// TODO: �ڴ���ӿؼ�֪ͨ����������
	//BROWSEINFO bi;
	//ZeroMemory(&bi,sizeof(BROWSEINFO));
	//bi.hwndOwner=GetSafeHwnd();
	//bi.lpszTitle="ѡ����Ϸ����Ŀ¼:";
	//LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	//if(idl==NULL)
	//	return;
	//SHGetPathFromIDList(idl,m_strGameRunFolder.GetBuffer(MAX_PATH));

	//UpdateData(false);
	//m_strWorkPath = m_strGameRunFolder.GetBuffer(MAX_PATH);

	//if(CheckGameRunFolder())
	//{
	//	GetDlgItem(ID_UPDATE)->EnableWindow(TRUE);
	//}
	//else
	//{
	//	GetDlgItem(ID_UPDATE)->EnableWindow(FALSE);
	//	AfxMessageBox("��Ŀ¼������Ϸ����Ŀ¼��");
	//}
}


void CPatchDlg::OnEnChangeEditGamerunfolder()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	UpdateData(true);
	m_strWorkPath = m_strGameRunFolder.GetBuffer(MAX_PATH);

	if(CheckGameRunFolder())
	{
		GetDlgItem(ID_UPDATE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(ID_UPDATE)->EnableWindow(FALSE);
		AfxMessageBox("��Ŀ¼������Ϸ����Ŀ¼��");
	}
}

void CPatchDlg::OnBnClickedUpdate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������ť
	GetDlgItem(ID_UPDATE)->EnableWindow(FALSE);

	if(m_pClientResource)
	{
		delete m_pClientResource;
		m_pClientResource=NULL;
	}

	m_pClientResource = new CClientResource(GAME_RES,m_strWorkPath,m_strFilesInfoName);

	unsigned threadID;
	hThread = (HANDLE)_beginthreadex(NULL,0,UpdateProc,this,0,&threadID);
}


BOOL	CPatchDlg::CheckGameRunFolder()
{
	string strFilesInfoName = m_strWorkPath+"\\PackRes.cfg";
	WIN32_FIND_DATA fdt;

	HANDLE hFind = FindFirstFile(strFilesInfoName.c_str(), &fdt);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;
	FindClose(hFind);

	/*strFilesInfoName = m_strWorkPath+"\\Game.dp";
	hFind = FindFirstFile(strFilesInfoName.c_str(), &fdt);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;
	FindClose(hFind);*/

	return TRUE;
}

void	CPatchDlg::OnUpdateDisplay()
{
	m_strDisplayInfo=gDisplayInfo.strText;
	

	m_Progress.SetRange(0,200);
	if(gDisplayInfo.nMax==0)
	{
		m_Progress.SetPos(0);
	}
	else
	{
		float fNum = (double)gDisplayInfo.nCur/(double)gDisplayInfo.nMax;
		short nCurpos = (float)200*fNum;
		m_Progress.SetPos(nCurpos);
	}

	UpdateData(false);
}

void CPatchDlg::SetDisplayInfo(HWND hwnd,CString strText,int nPos,int nMax)
{
	gDisplayInfo.strText=strText;
	gDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE,NULL,NULL);
	Sleep(2);
}
void CPatchDlg::SetDisplayInfo(HWND hwnd,int nPos,int nMax)
{
	gDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE,NULL,NULL);
	Sleep(2);
}


BOOL CPatchDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case DISPLAY_UPDATE:
		{
			OnUpdateDisplay();
		}
		break;
	}

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

//�����߳�
unsigned __stdcall CPatchDlg::UpdateProc(void* pParam)
{
	CPatchDlg* pPatchDlg = (CPatchDlg*)pParam;
	if(pPatchDlg == NULL || pPatchDlg->m_pClientResource == NULL)	return 0;

	/*if(pPatchDlg->GetPatchInfo()==FALSE)
	{
		AfxMessageBox("UpdateProc::�õ�����Ϣ���ݴ���");
		_endthreadex( 0 );
		return 0;
	}*/

	if(pPatchDlg->GetPatchData()==FALSE)
	{
		AfxMessageBox("UpdateProc::�õ������ݴ���");
		_endthreadex( 0 );
		return 0;
	}

	//���ð���Ϣ
	CClientResource* pClientResour = pPatchDlg->m_pClientResource;
	if(!LoadClientResource(pPatchDlg))
	{
		AfxMessageBox("UpdateProc::��ȡ��Դ���ݴ���");
		_endthreadex( 0 );
		return FALSE;
	}

	if(bExit == true)
	{
		_endthreadex( 0 );
		return FALSE;
	}

	char strTitle[200];
	string tmpFolder = pPatchDlg->m_strExePath + "\\PackRes.cfg";
	FILE* file = fopen(tmpFolder.c_str(), "rb");
	char strVersion[16] = {0};
	if(file)
	{
		fread(strVersion, 10, 1, file);
		fclose(file);
	}
	
	
	if(strcmp(strVersion, pPatchDlg->m_listStrUpdatedVersion.c_str()) != 0) // �������ڵı������汾�Ÿ��ͻ���FileInfo��¼�Ĳ���.
	{
		char szOutPut[1024];
		sprintf(szOutPut, "�������ڵı������ͻ��˰汾��[%s]��ͻ��˵İ汾��[%s]����,\r\n����ͻ��˵ĵ�ǰ�汾�š�", pPatchDlg->m_listStrUpdatedVersion.c_str(), strVersion);
		AfxMessageBox(szOutPut);
		return FALSE;
	}

	sprintf(strTitle,"������Ϸ�Ӱ汾%s ���汾%s�� ", strVersion,
		pPatchDlg->m_pPatchFilesInfo->GetVersion().c_str());
	pPatchDlg->SetWindowText(strTitle);

	

	string strTemptOld;		
	string strTemptNew;
	strTemptOld.insert(0,pClientResour->GetVersion(),0,5);
	strTemptNew.insert(0,strVersion,0,5);

#ifndef	_DEBUG
	if(strTemptOld != strTemptNew)
	{
		//����������Ϸ�汾����
		SetDisplayInfo(pPatchDlg->m_hWnd,"����������Ϸ�汾������",1,1);
		PutDebugString("����������Ϸ�汾������");
		return FALSE;
	}
#endif

	//�õ���ͬ����
	CFilesInfo DirfFilesInfo;
	pPatchDlg->m_pPatchFilesInfo->GetDifferenceByCrc32( *(pClientResour->GetFilesInfo()),DirfFilesInfo);


	SetDisplayInfo(pPatchDlg->m_hWnd,"�����ļ�...",0,
				DirfFilesInfo.GetRootFile()->dwSize);
	PutDebugString("�����ļ�...");

	int nTotalSize = 0;

	DWORD	dwBufSize = 1024*1024*2;
	BYTE *pBuf = new BYTE[dwBufSize];
	BOOL bFlag = UpdateClientResource(DirfFilesInfo.GetRootFile(),&DirfFilesInfo,pBuf,dwBufSize,
		pPatchDlg->m_pPatchPackage,pClientResour,pPatchDlg->m_hWnd,nTotalSize);
	delete[] pBuf;

	if( bFlag )
	{
		pClientResour->GetFilesInfo()->SetVersion(pPatchDlg->m_pPatchFilesInfo->GetVersion());
		pClientResour->UpdateSave();
		SetDisplayInfo(pPatchDlg->m_hWnd,"�����ɹ���",1,1);
		AfxMessageBox("�����ɹ�");
	}

	else
	{
		pClientResour->UpdateSave();
		SetDisplayInfo(pPatchDlg->m_hWnd,"����ʧ�ܡ�",1,1);
		AfxMessageBox("����ʧ��");
	}

	// �رհ���Դ
	pPatchDlg->m_pPatchPackage->CloseFileHandle();

	string delPath = pPatchDlg->m_strWorkPath + "\\updatefiles";
	DeleteDirectory(delPath.c_str());

	::SendMessage(pPatchDlg->m_hWnd, WM_CLOSE, 0, 0);
	// ���Զ����³���
	string updateExe = pPatchDlg->m_strWorkPath + "\\update.exe";
	WinExec(updateExe.c_str(), SW_SHOWNORMAL);
	_endthreadex( 0 );
	return 0;
}

//װ�ؿͻ�����Դ��Ϣ
BOOL	CPatchDlg::LoadClientResource(CPatchDlg* pPatchDlg)
{
	
	CClientResource* pClientResour = pPatchDlg->m_pClientResource;

	int nMax = 1;
	int nPos = 0;
	SetDisplayInfo(pPatchDlg->m_hWnd,"��ȡ�ͻ����ļ���Ϣ...",nPos,nMax);
	PutDebugString("��ȡ�ͻ����ļ���Ϣ...");
	if(pClientResour->LoadEx( 
					pPatchDlg->m_pPatchFilesInfo->GetPackageTypeInfo() )==false)
	{
		SetDisplayInfo(pPatchDlg->m_hWnd,"��ȡ�ͻ�����Ҫ�ļ���Ϣ����",nMax,nMax);
		PutDebugString("��ȡ�ͻ�����Ҫ�ļ���Ϣ����.");
		return FALSE;
	}

	SetDisplayInfo(pPatchDlg->m_hWnd,"��ȡ�ͻ����ļ���Ϣ��ϡ�",nMax,nMax);
	PutDebugString("��ȡ�ͻ����ļ���Ϣ���.");
	return TRUE;;
}

//������Ӧ���ļ�
BOOL  CPatchDlg::UpdateClientResource(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,BYTE*& pBuf,DWORD& dwBuflen,
					 CPackage* pPackage, CClientResource* pClientResource,HWND hwnd,int& nTotalSize)
{
	if(ptagFileInfo==NULL || pFilesInfo==NULL || pPackage==NULL 
		|| pClientResource==NULL || bExit==true )
		return false;

	string strFileName = pFilesInfo->GetFileTextWithParent(ptagFileInfo);
	if( pFilesInfo->HaveChild(ptagFileInfo) )
	{
		//���û�д��ļ��У������
		tagFileInfo* pCurFolderInfo = pClientResource->GetFilesInfo()->GetFileInfoByText(strFileName);
		if(pCurFolderInfo==NULL)
		{
			string strParentName = pFilesInfo->GetFileTextWithParent(ptagFileInfo->ParentFileInfo);
			pClientResource->GetFilesInfo()->AddFolderInfo(strParentName,ptagFileInfo->strFileName,
				ptagFileInfo->dwSize,ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize,ptagFileInfo->dwPackageType,
				ptagFileInfo->dwCompressType);
		}

		if( ENABLEBIT(ptagFileInfo->dwPackageType,NOPACK) )
		{
			//���Դ������ļ���
			string strTemptFileName = pClientResource->GetCurFolder()+strFileName;
			::CreateDirectory(strTemptFileName.c_str(),NULL);
		}

		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			if(!UpdateClientResource((*it).second,pFilesInfo,pBuf,dwBuflen,
										pPackage,pClientResource,hwnd,nTotalSize))
			{
				return FALSE;
			}
		}
	}
	else if(ptagFileInfo->bFolder == false)
	{
		//nTotalSize += ptagFileInfo->dwSize;
		//SetDisplayInfo(hwnd,nTotalSize);

		if( !ENABLEBIT(ptagFileInfo->dwPackageType,NOPACK) )
		{
			DWORD dwSize = pPackage->GetFileSize(strFileName.c_str());
			// 0�ֽ��ļ���д
			if(dwSize == 0) return true;

			tagFileIndex FileIndex;
			if(dwSize > dwBuflen)
			{
				delete[] pBuf;
				dwBuflen = dwSize;
				pBuf = new BYTE[dwSize];
			}

			//�Ӱ�����ȡ����
			if(pPackage->ExtractToBuf(strFileName.c_str(),pBuf,dwSize,FileIndex))
			{
				try
				{
					CPackage *pGamePackage = pClientResource->GetPackageForUpdate(ptagFileInfo->dwPackageType);
					if(pGamePackage)
					{
						//	FileIndex.dwRealSize=FileIndex.dwSize;
						int nRet = pGamePackage->WriteData(FileIndex,pBuf);
						if(nRet == false)
						{
							char outPtr[1024];
							sprintf(outPtr, "д�ļ� %s ����!", strFileName.c_str());
							PutDebugString(outPtr);
							return false;
						}
					}
				}
				catch (...)
				{
					char strTempt[256]="";
					sprintf(strTempt,"д���ݳ������ش���:FileName:%s,dwSize:%d",
						strFileName.c_str(),dwSize);
					AfxMessageBox(strTempt);
					return false;
				}
			}
			else
			{
				char outPtr[1024];
				sprintf(outPtr, "�Ӱ�����ȡ�����ļ� %s ����!", strFileName.c_str());
				PutDebugString(outPtr);
				return false;
			}
		}
		else
		{
			try
			{
				string strTemptFileName = pClientResource->GetCurFolder()+strFileName;
				//��ȡ���ļ�
				if( pPackage->ExtractToFile(strFileName.c_str(),strTemptFileName.c_str()) == false)
				{
					char outPtr[1024];
					sprintf(outPtr, "�Ӱ�����ȡ���ļ� %s ����!", strFileName.c_str());
					PutDebugString(outPtr);
					return false;
				}
			}
			catch (...)
			{
				AfxMessageBox("��ȡ�ļ��������ش���");
				return false;
			}
		}

		pClientResource->GetFilesInfo()->UpdateFileInfo(strFileName,ptagFileInfo->dwCrc32,
						ptagFileInfo->dwSize,ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize,false,
						ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType);

		nTotalSize += ptagFileInfo->dwSize;
		SetDisplayInfo(hwnd,strFileName.c_str(),nTotalSize);
	}
	//Sleep(2);
	// ������������ʱ�ļ�Update.txt,tmp.zip,

	return TRUE;
}
void CPatchDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	::WaitForSingleObject(hThread,INFINITE);
	CDialog::OnCancel();
}

void CPatchDlg::OnBnClickedCancel()
{
	bExit = true;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CPatchDlg::OnStnClickedStaticTextinfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CPatchDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	// ���ҿͻ���·��,ע���
	//ע�����Ϣ
	string strReg("HKEY_LOCAL_MACHINE/SOFTWARE/xingman/Luvinia");

	HKEY hKey;
	DWORD dwBufLen =2*MAX_PATH+10;
	char szKeyValue[2*MAX_PATH+10];
	memset(szKeyValue, 0, 2*MAX_PATH+10);

	LONG lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		"SOFTWARE\\XINGMAN\\LUVINIA",
		0, KEY_QUERY_VALUE, &hKey );


	lRet = RegQueryValueEx( hKey, "Client_Path", NULL, NULL,
		(LPBYTE)szKeyValue, &dwBufLen);

	RegCloseKey( hKey );

	if(szKeyValue[0] != '\0')
	{
		m_strExePath = string(szKeyValue);
		m_strWorkPath = m_strExePath;
	}

	bExit = false;

	string strFileName = m_strWorkPath + "\\launcher.exe";
	WinExec(strFileName.c_str(), SW_SHOW);
}
