// ServerUpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServerUpdate.h"
#include "ServerUpdateDlg.h"
#include ".\serverupdatedlg.h"

#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/Crc32Static.h"

#include <AFXINET.H>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerUpdateDlg �Ի���

#define GET_BUFFER_MAX 1024 * 16
#define UPDATE_DISPLAY_INFO	WM_USER+6000
#define UPDATE_PROGRESS_MAX_LEN	WM_USER+6001
#define UPDATE_PROGRESS_CUR_LEN	WM_USER+6002

#define START_BUTTON_ENABLE	WM_USER+6003
#define EXIT_BUTTON_ENABLE	WM_USER+6004

#define UPDATE_TEXT	WM_USER+6000
HANDLE hUpdateThread=0;
static string strExceFilePath = "";
string g_strExceFileName="";

HINTERNET hUpdateServerConnect = NULL;							//�͸��·�����������
HINTERNET hUpdateSession = NULL;

static string g_strOldVersion="";
static string g_strCurVersion="";
static string g_strServerTile="";

struct tagThreadParam
{
	HWND hWnd;
	CClientResource *pClientResouce;
	string strCurDirecotry;
	string strServerAddress;
	string strServerFolder;
};

CServerUpdateDlg::CServerUpdateDlg(string& strCurDirectory,string& strServerAdress,
								  string& strServerFolder,string &strFileName,
								  string& strServerTitle,CWnd* pParent /*=NULL*/)
	: CDialog(CServerUpdateDlg::IDD, pParent)
	,m_strWorkPath(strCurDirectory)
	,m_strServerAddress(strServerAdress)
	,m_strServerFolder(strServerFolder)
	,m_strFilesInfoName("FilesInfo.ril")
	,m_pClientResouce(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	g_strExceFileName = strFileName;
	strExceFilePath = "\\"+strFileName;
	g_strServerTile=strServerTitle;
}

void CServerUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_UPDATE, m_UpdateProgress);
	DDX_Control(pDX, IDC_STATIC_UPDATE_INFO, m_UpdateInfo);
	DDX_Control(pDX, IDC_BUTTON_STARTSERVER, m_StartServer);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_Exit);
}

BEGIN_MESSAGE_MAP(CServerUpdateDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
//	ON_WM_CHAR()
ON_BN_CLICKED(IDC_BUTTON_STARTSERVER, OnBnClickedButtonStartserver)
ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CServerUpdateDlg ��Ϣ�������

BOOL CServerUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_StartServer.EnableWindow(false);
	m_Exit.EnableWindow(false);
	//��ʼ����Դʵ��
	m_pClientResouce = new CClientResource(GAME_RES,m_strWorkPath,m_strFilesInfoName);
	if(m_pClientResouce->LoadExForAutoUpdate()==false)
	{
		delete m_pClientResouce;m_pClientResouce= NULL;
		m_Exit.EnableWindow(true);
		return TRUE;
	}
	g_strCurVersion = m_pClientResouce->GetFilesInfo()->GetVersion();
	g_strOldVersion = g_strCurVersion;

	string strTitle = g_strServerTile;
	strTitle += "(";
	strTitle += g_strCurVersion;
	strTitle += ")";
	SetWindowText(strTitle.c_str());
	//��ʼ�����߳�
	unsigned threadID;
	tagThreadParam* pParam = new tagThreadParam;
	pParam->hWnd = m_hWnd;
	pParam->pClientResouce = m_pClientResouce;
	pParam->strCurDirecotry= m_strWorkPath;
	pParam->strServerAddress= m_strServerAddress;
	pParam->strServerFolder = "/"+m_strServerFolder;
	hUpdateThread = (HANDLE)_beginthreadex(NULL,0,UpdateProc,(void*)pParam,0,&threadID);	
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CServerUpdateDlg::Release()
{
	::WaitForSingleObject(hUpdateThread,INFINITE);

	if(m_pClientResouce )
	{
		delete m_pClientResouce;
		m_pClientResouce = NULL;
	}

	if(hUpdateServerConnect)
		::InternetCloseHandle(hUpdateServerConnect);
	if(hUpdateSession)
		::InternetCloseHandle(hUpdateSession);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerUpdateDlg::OnPaint() 
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
HCURSOR CServerUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerUpdateDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
}

void CServerUpdateDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
}

BOOL CServerUpdateDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case UPDATE_DISPLAY_INFO:
		{
			char *strInfo = (char*)wParam;
			m_UpdateInfo.SetWindowText(strInfo);
			delete []strInfo;
		}
		break;
	case UPDATE_PROGRESS_MAX_LEN:
		{
			int nNum = (int)wParam;
			m_UpdateProgress.SetRange(0,nNum);
		}
		break;
	case UPDATE_PROGRESS_CUR_LEN:
		{
			int nPos = (int)wParam;
			m_UpdateProgress.SetPos(nPos);
		}
		break;
	case START_BUTTON_ENABLE:
		{
			m_StartServer.EnableWindow(true);
			m_Exit.EnableWindow(true);

			string strTitle = g_strServerTile;
			strTitle += "(";
			strTitle += g_strCurVersion;
			strTitle += ")";
			SetWindowText(strTitle.c_str());
			Release();
		}
		break;
	case EXIT_BUTTON_ENABLE:
		{
			m_Exit.EnableWindow(true);
			Release();
		}
		break;
	}
	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

void CServerUpdateDlg::SetProgressMaxLen(HWND hwnd,int nNum)
{
	::PostMessage(hwnd,UPDATE_PROGRESS_MAX_LEN,(WPARAM)nNum,NULL);
}
void CServerUpdateDlg::SetProgressCunPos(HWND hwnd,int nPos)
{
	::PostMessage(hwnd,UPDATE_PROGRESS_CUR_LEN,(WPARAM)nPos,NULL);
}
void CServerUpdateDlg::SetDisplayText(HWND hwnd,CString strInfo)
{
	int len = strInfo.GetLength();
	char *szInfo = new char[len+1];
	strcpy(szInfo,strInfo.GetBuffer());
	szInfo[len] = '\0';
	::PostMessage(hwnd,UPDATE_DISPLAY_INFO,(WPARAM)szInfo,NULL);
}

//�����߳�
unsigned __stdcall CServerUpdateDlg::UpdateProc(void* pParam)
{
	tagThreadParam* pThreadParam = (tagThreadParam*)pParam;
	SetProgressMaxLen(pThreadParam->hWnd,2);
	SetProgressCunPos(pThreadParam->hWnd,0);
	SetDisplayText(pThreadParam->hWnd,"�õ����������°汾��Ϣ...");

	if(::InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
		SetDisplayText(pThreadParam->hWnd,"�����ʼ��ʧ��(InternetAttemptConnect)!");
		::PostMessage(pThreadParam->hWnd,EXIT_BUTTON_ENABLE,NULL,NULL);
		delete pThreadParam;
		_endthreadex( 0 );
		return 0;
	}

	HINTERNET hSession = ::InternetOpen(_T("Tnt"),INTERNET_OPEN_TYPE_PRECONFIG,_T(""),
		INTERNET_INVALID_PORT_NUMBER,0);
	if(hSession == NULL)
	{		
		SetDisplayText(pThreadParam->hWnd,"�����ʼ��ʧ��(InternetOpen)!");
		::PostMessage(pThreadParam->hWnd,EXIT_BUTTON_ENABLE,NULL,NULL);
		delete pThreadParam;
		_endthreadex( 0 );
		return 0;
	}
	hUpdateSession = hSession;

	HINTERNET hUpdateConnect=NULL;
	string strServerVersion("");
	DWORD dwServerFileInfoCRC;
	if(GetLastFileInfo(hSession,pThreadParam->strCurDirecotry,
					pThreadParam->strServerAddress,pThreadParam->strServerFolder,
					hUpdateConnect,dwServerFileInfoCRC,strServerVersion) == false)
	{
		SetDisplayText(pThreadParam->hWnd,"�����°汾��Ϣʧ��!");
		::PostMessage(pThreadParam->hWnd,EXIT_BUTTON_ENABLE,NULL,NULL);
		delete pThreadParam;
		_endthreadex( 0 );
		return 0;
	}
	hUpdateServerConnect = hUpdateConnect;

	string strCurVersion = pThreadParam->pClientResouce->GetFilesInfo()->GetVersion();
	//�汾һ���˳�,�������ý���
	if(strServerVersion==strCurVersion)
	{
		SetProgressCunPos(pThreadParam->hWnd,2);
		SetDisplayText(pThreadParam->hWnd,"��ǰ�����°汾��");

		::PostMessage(pThreadParam->hWnd,START_BUTTON_ENABLE,NULL,NULL);
		delete pThreadParam;
		_endthreadex( 0 );
		return	0;
	}
	SetProgressCunPos(pThreadParam->hWnd,1);
	SetDisplayText(pThreadParam->hWnd,"�汾�ѹ���,�������ļ���Ϣ...");

	//��һ���������÷�Χ,��ʹ�߳��˳�ǰ���ͷ�������Դ
	{
		CFilesInfo ServerFilesInfo,TemptFilesInfo;
		string strServerFileInfoName = pThreadParam->strServerFolder;
		strServerFileInfoName +="\\FilesInfo.ril";
		string strLocalFileName = pThreadParam->strCurDirecotry;
		strLocalFileName += "\\UpdateFilesInfo.ril";
		if( GetLastFileInfos(hUpdateConnect,strServerFileInfoName,
							strLocalFileName,dwServerFileInfoCRC,&ServerFilesInfo) == false)
		{
			SetDisplayText(pThreadParam->hWnd,"�������ļ���Ϣʧ�ܡ�");
			::PostMessage(pThreadParam->hWnd,EXIT_BUTTON_ENABLE,NULL,NULL);
			delete pThreadParam;
			_endthreadex( 0 );
			return	0;
		}

		ServerFilesInfo.GetDifferenceByCrc32(*(pThreadParam->pClientResouce->GetFilesInfo()),TemptFilesInfo);
		pThreadParam->pClientResouce->ResetPackInfosForAutoUpdate(ServerFilesInfo.GetPackageTypeInfo());
		SetProgressCunPos(pThreadParam->hWnd,2);
		SetDisplayText(pThreadParam->hWnd,"�������ļ��б���ϡ�");

		//�����ļ�
		int nFileTotalSize = TemptFilesInfo.GetRootFile()->dwSize;
		int nUpdateSize = 0;
		SetProgressCunPos(pThreadParam->hWnd,nFileTotalSize);
		SetDisplayText(pThreadParam->hWnd,"�����ļ�...");

		//����ʱ���ذ�
		CPackage DownloadPack(NULL, pThreadParam->strCurDirecotry,1);
		DownloadPack.OpenForUpdate("TemptDownloadPack");
		string strDownloadPackFileName = pThreadParam->strCurDirecotry;
		strDownloadPackFileName = strDownloadPackFileName+"\\"+"TemptDownloadPack"+".pak";

		BYTE *pBuf = new BYTE[GET_BUFFER_MAX];
		DWORD dwBufLen = GET_BUFFER_MAX;

		if(DownLoadFilesEx(hUpdateConnect,&DownloadPack,pBuf,dwBufLen,
			TemptFilesInfo.GetRootFile(),&TemptFilesInfo,
			pThreadParam->strServerFolder,pThreadParam->pClientResouce,
			nUpdateSize,pThreadParam->hWnd)==false)
		{
			SetDisplayText(pThreadParam->hWnd,"�ļ����·�������");
			pThreadParam->pClientResouce->UpdateSave();
			delete[] pBuf;
			DownloadPack.CloseFileHandle();
			::DeleteFile(strDownloadPackFileName.c_str());
			::PostMessage(pThreadParam->hWnd,EXIT_BUTTON_ENABLE,NULL,NULL);
			delete pThreadParam;
			_endthreadex( 0 );
			return 0;
		}
		delete[] pBuf;
		DownloadPack.CloseFileHandle();
		::DeleteFile(strDownloadPackFileName.c_str());

		string strNewExceFileName = strExceFilePath+".new";
		tagFileInfo* pFileInfo =
			pThreadParam->pClientResouce->GetFilesInfo()->GetFileInfoByText(strNewExceFileName);

		//�쿴������ִ���ļ��Ƿ����
		if(pFileInfo && pFileInfo->dwValidSize == pFileInfo->dwSize)
		{
			pThreadParam->pClientResouce->GetFilesInfo()->UpdateFileInfo(strExceFilePath,pFileInfo->dwCrc32,
						pFileInfo->dwSize,pFileInfo->dwOrginSize,pFileInfo->dwValidSize,false,
						pFileInfo->dwPackageType,pFileInfo->dwCompressType);

			pThreadParam->pClientResouce->GetFilesInfo()->DelFileInfo(strNewExceFileName);

			
			WIN32_FIND_DATA myWFD;
			CString strNewExceName(g_strExceFileName.c_str());
			strNewExceName = strNewExceName+".new";
			HANDLE hl = FindFirstFile(strNewExceName.GetBuffer(),&myWFD);
			if ( hl != INVALID_HANDLE_VALUE)
			{
				CString strOldExceName(g_strExceFileName.c_str());
				
				SYSTEMTIME sysCurrTime;
				::GetLocalTime(&sysCurrTime);
				char strCurTime[100];
				sprintf(strCurTime,"(%d_%d_%d[%d_%d_%d])(%s)",
									sysCurrTime.wYear,sysCurrTime.wMonth,sysCurrTime.wDay,
									sysCurrTime.wHour,sysCurrTime.wMinute,sysCurrTime.wSecond,
									g_strOldVersion.c_str());
				strOldExceName = strOldExceName+strCurTime;

				rename(g_strExceFileName.c_str(),strOldExceName.GetBuffer());
				rename(strNewExceName.GetBuffer(),g_strExceFileName.c_str());
			}
		}

		SetDisplayText(pThreadParam->hWnd,"���������Ϣ...");
		SetProgressCunPos(pThreadParam->hWnd,nFileTotalSize);
		pThreadParam->pClientResouce->GetFilesInfo()->SetVersion(strServerVersion);
		pThreadParam->pClientResouce->UpdateSave();

		//��¼�汾��
		g_strCurVersion = strServerVersion;
	}

	//�����汾���ļ�
	if( _access(g_strOldVersion.c_str(),0) != -1 )
	{
		//�ļ�����
		rename(g_strOldVersion.c_str(),g_strCurVersion.c_str());
	}
	else
	{
		//������
		FILE *pf = fopen( g_strCurVersion.c_str(),"w" );
		if(pf)
			fclose(pf);
	}


	SetDisplayText(pThreadParam->hWnd,"������ϡ�");
	::PostMessage(pThreadParam->hWnd,START_BUTTON_ENABLE,NULL,NULL);
	delete pThreadParam;
	_endthreadex( 0 );
	return 0;
}

bool CServerUpdateDlg::GetLastFileInfo(HINTERNET hSession,string& strCurDirectory,
									string& strUrl,string& strServerFolder,
									HINTERNET& hUpdateConnect,DWORD& dwFileInfoCRC,string& strVersion)
{
	//�����Ƿ��ܴ򿪷�����
	string strUpdateUrl = "http://"+strUrl;
	strUpdateUrl += strServerFolder;
	HINTERNET hOpen = ::InternetOpenUrl(hSession,strUpdateUrl.c_str(),NULL,0,INTERNET_FLAG_EXISTING_CONNECT,NULL);
	if(hOpen)
	{
		InternetCloseHandle(hOpen);
		HINTERNET hConnect = ::InternetConnect(hSession,strUrl.c_str(),
			INTERNET_INVALID_PORT_NUMBER,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
		if(hConnect)
		{
			hUpdateConnect = hConnect;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	string strFileName = strServerFolder;
	strFileName +="/AutoUpdateGame.vs";
	string strLocalVersionFileName = strCurDirectory;
	strLocalVersionFileName += "\\AutoUpdateGame.vs";
	if(GetLastVersion(hUpdateConnect,strFileName,strLocalVersionFileName,dwFileInfoCRC,strVersion)==false)
	{
		InternetCloseHandle(hUpdateConnect);
		hUpdateConnect = NULL;
		return false;
	}

	return true;
}
bool CServerUpdateDlg::GetLastVersion(HINTERNET hConnect,string& strPathName,string& strLocalVersionName,
								DWORD& dwFileInfoCRC,string& strVersion)
{
	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
		strPathName.c_str(),HTTP_VERSION,NULL,0,
		INTERNET_FLAG_RELOAD,0);
	if(hHttpFile == NULL)
	{
		return false;
	}
	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(bSendRequest)
	{
		char	achQueryBuf[16];
		DWORD	dwFileSize;
		DWORD	dwQueryBufLen = sizeof(achQueryBuf);

		BOOL	bQuery = ::HttpQueryInfo(hHttpFile,
			HTTP_QUERY_CONTENT_LENGTH,achQueryBuf,&dwQueryBufLen,NULL);

		if(!bQuery)
		{
			::InternetCloseHandle(hHttpFile);
			return false;
		}


		FILE* pf = fopen(strLocalVersionName.c_str(),"w+b");
		if(!pf)
		{
			::InternetCloseHandle(hHttpFile);
			return	false;
		}

		dwFileSize = (DWORD) atol(achQueryBuf);	
		char * lpszBuf = new char[dwFileSize + 1 ];
		DWORD	dwBytesRead;
		BOOL	bRead;
		
		bRead = ::InternetReadFile(hHttpFile,lpszBuf,dwFileSize+1,&dwBytesRead);
		::InternetCloseHandle(hHttpFile);

		fwrite(lpszBuf,1,dwBytesRead,pf);
		fflush(pf);
		fseek(pf,0,SEEK_SET);
		char chVersion[50]="";
		fscanf(pf,"%s\n",chVersion);
		fscanf(pf,"%u ",&dwFileInfoCRC);
		fclose(pf);
		chVersion[10]='\0';
		strVersion = chVersion;

		::DeleteFile(strLocalVersionName.c_str());	

		if( lpszBuf[0] < '0' || lpszBuf[0] > '9' || lpszBuf[1] < '0' || lpszBuf[1] > '9' ||
			 lpszBuf[3] < '0' || lpszBuf[3] > '9' || lpszBuf[4] < '0' || lpszBuf[4] > '9' ||
			  lpszBuf[6] < '0' || lpszBuf[6] > '9' || lpszBuf[7] < '0' || lpszBuf[7] > '9' ||
			   lpszBuf[8] < '0' || lpszBuf[8] > '9' || lpszBuf[9] < '0' || lpszBuf[9] > '9')
		{
			return false;
		}
		delete []lpszBuf;
		return	true;
	}
	else
	{
		DWORD dwError = GetLastError();
		return false;
	}
	return	false;
}

bool CServerUpdateDlg::GetLastFileInfos(HINTERNET hConnect,string& strPathName,string& strLocalFileName,
									DWORD dwFileInfoCRC,CFilesInfo* pFilesInfo)
{
	if(pFilesInfo==NULL || hConnect==NULL)	return false;

	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
		strPathName.c_str(),HTTP_VERSION,NULL,0,
		INTERNET_FLAG_RELOAD,0);
	if(hHttpFile == NULL)
	{
		return false;
	}
	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(bSendRequest)
	{
		char	achQueryBuf[16];
		DWORD	dwFileSize;
		DWORD	dwQueryBufLen = sizeof(achQueryBuf);

		BOOL	bQuery = ::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH,achQueryBuf,&dwQueryBufLen,NULL);

		if(!bQuery)
		{
			::InternetCloseHandle(hHttpFile);
			return false;
		}
		
		FILE* pf = fopen(strLocalFileName.c_str(),"wb");
		if(!pf)
		{
			::InternetCloseHandle(hHttpFile);
			return	false;
		}
		dwFileSize = (DWORD) atol(achQueryBuf);				
		BYTE	strBuffer[GET_BUFFER_MAX];
		DWORD	dwBytesRead;
		BOOL	bRead;

		DWORD dwCount = 0;
		DWORD dwRequest = ( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
		while (true)
		{
			bRead = ::InternetReadFile(hHttpFile,strBuffer,dwRequest,&dwBytesRead);
			if(!bRead || dwBytesRead <= 0)
			{
				fclose(pf);
				::DeleteFile(strLocalFileName.c_str());	
				::InternetCloseHandle(hHttpFile);
				return	false;
			}

			fwrite(strBuffer,1,dwBytesRead,pf);
			dwCount += dwBytesRead;
			if ( dwFileSize - dwCount < GET_BUFFER_MAX )
				dwRequest = dwFileSize - dwCount;

			if ( dwRequest <= 0 )
			{
				break;
			}
		}
		::InternetCloseHandle(hHttpFile);
		fclose(pf);

		DWORD dwCRC;
		CCrc32Static::FileCrc32Filemap(strLocalFileName.c_str(),dwCRC);
		if(dwCRC != dwFileInfoCRC)
		{
			::DeleteFile(strLocalFileName.c_str());	
			return	false;
		}
		pFilesInfo->Load(strLocalFileName);
		::DeleteFile(strLocalFileName.c_str());	
		return	true;
	}
	return false;
}

bool	 CServerUpdateDlg::DownLoadFilesEx(HINTERNET hConnect,CPackage* pPckage,BYTE*& pBuf,DWORD& dwBufLen,
								tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,
								string& strFtpPath,CClientResource* pResource,
								int& nTotalSize,HWND hwnd)
{
	if(ptagFileInfo==NULL)	return false;

	string strTemptFileName = pFilesInfo->GetFileTextWithParent(ptagFileInfo);

	if(pFilesInfo->HaveChild(ptagFileInfo))
	{
		//���û�д��ļ��У������
		tagFileInfo* pCurFolderInfo = pResource->GetFilesInfo()->GetFileInfoByText(strTemptFileName);
		if(pCurFolderInfo==NULL)
		{
			string strParentName = pFilesInfo->GetFileTextWithParent(ptagFileInfo->ParentFileInfo);
			pResource->GetFilesInfo()->AddFolderInfo(strParentName,ptagFileInfo->strFileName,
										ptagFileInfo->dwSize,ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize, 
										ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType);
		}

		//���δ����ģ���Ҫ�����ļ���
		if( ENABLEBIT(ptagFileInfo->dwPackageType,NOPACK) )
		{
			//���Դ������ļ���
			string strTemptFileName1 = pResource->GetCurFolder()+strTemptFileName;
			::CreateDirectory(strTemptFileName1.c_str(),NULL);
		}

		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			if( !DownLoadFilesEx(hConnect,pPckage,pBuf,dwBufLen,
								(*it).second,pFilesInfo,
								strFtpPath,pResource,
								nTotalSize,hwnd) )
				return false;
		}
	}
	else if(ptagFileInfo->bFolder == false)
	{
		string strServerFilename = strFtpPath+strTemptFileName;
		//����Ǹ�������Ϸִ���ļ�����ı�����
		if( strTemptFileName == strExceFilePath )
		{
			strTemptFileName=strExceFilePath+".new";
		}
		else if(strTemptFileName == "ServerUpdate.exe")
		{
			//�������Լ�
			return true;
		}

		SetDisplayText(hwnd,strTemptFileName.c_str());
			
		if( DownLoadOneFile(hConnect,pPckage,pBuf,
						strTemptFileName,strServerFilename,
						ptagFileInfo,nTotalSize,hwnd) == true)
		{
			bool bRet = false;
			//����ڰ���
			if( !ENABLEBIT(ptagFileInfo->dwPackageType,NOPACK) )
			{
				//���سɹ�������и�������
				tagFileIndex FileIndex;
				DWORD dwSize = ptagFileInfo->dwSize;
				if(dwSize > dwBufLen)
				{
					delete[] pBuf;
					pBuf = new BYTE[dwSize];
					dwBufLen = dwSize;
				}
				if(pPckage->ExtractToBuf(strTemptFileName.c_str(),pBuf,dwSize,FileIndex))
				{
					FileIndex.dwValidSize = FileIndex.dwSize;
					CPackage *pUpdatePack = pResource->GetPackageForUpdate(ptagFileInfo->dwPackageType);
					if(pUpdatePack)
					{
						pUpdatePack->WriteData(FileIndex,pBuf);

						pResource->GetFilesInfo()->UpdateFileInfo(strTemptFileName,ptagFileInfo->dwCrc32,
							ptagFileInfo->dwSize,ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize,false,
							ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType);
						
						bRet = true;
					}
					
				}
			}
			else
			{
				string strFullFileName = pResource->GetCurFolder()+strTemptFileName;
				//��ȡ���ļ�
				if( pPckage->ExtractToFile(strTemptFileName.c_str(),strFullFileName.c_str()) )
				{
					pResource->GetFilesInfo()->UpdateFileInfo(strTemptFileName,ptagFileInfo->dwCrc32,
							ptagFileInfo->dwSize,ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize,false,
							ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType);
					bRet = true;
				}
			}
			//������ݣ�������һ���ļ�
			pPckage->ClearData();
			return bRet;
			
		}
		else
		{
			//�����򱣴����ص����ݣ��˳���
			pPckage->WriteIndexEx();
			pPckage->CloseFileHandle();
			return false;
		}
	}
	return true;
}

bool	CServerUpdateDlg::DownLoadOneFile(HINTERNET hConnect,CPackage* pPckage,BYTE* pBuf,
							string& strTemptFileName,string& strServerFilename,
							tagFileInfo* ptagFileInfo,
							int& nTotalSize,HWND hwnd)
{
	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
			strServerFilename.c_str(),HTTP_VERSION,NULL,0,
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_DONT_CACHE |
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION,0);
	if(hHttpFile == NULL)	return false;

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(!bSendRequest)	return false;

	char	achQueryBuf[16];
	DWORD	dwQueryBufLen = sizeof(achQueryBuf);
	BOOL bQuery = ::HttpQueryInfo(hHttpFile,
		HTTP_QUERY_CONTENT_LENGTH,achQueryBuf,&dwQueryBufLen,NULL);

	if(!bQuery)
	{
		::InternetCloseHandle(hHttpFile);
		return false;
	}
	DWORD	dwServerFileSize = (DWORD) atol(achQueryBuf);
	if(dwServerFileSize != ptagFileInfo->dwSize)	return false;

	pPckage->AddFile(strTemptFileName.c_str(),dwServerFileSize,ptagFileInfo->dwCrc32,
		ptagFileInfo->dwCompressType,ptagFileInfo->dwOrginSize);

	bool bRet = DownloadFile(hHttpFile,pPckage,pBuf,
							strTemptFileName,dwServerFileSize,0,
							nTotalSize,hwnd);
	
	::InternetCloseHandle(hHttpFile);
	return bRet;
}

bool CServerUpdateDlg::DownloadFile(HINTERNET hHttpFile,CPackage* pPckage,BYTE* pBuf,
							string& strTemptFileName,
							DWORD dwFileSize,DWORD dwCount,
							int& nTotalSize,HWND hwnd)
{
	DWORD   dwInvaSize = 0;
	int		nRet = true;
	DWORD	dwBytesRead = 0;
	BOOL	bRead;
	DWORD   dwRequest =
		( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
	while(dwRequest > 0)
	{
		bRead = ::InternetReadFile(hHttpFile,pBuf,dwRequest,&dwBytesRead);

		if(!bRead )
		{
			return false;
		}
		if(dwBytesRead == 0)
			break;
		nRet = pPckage->AddFileData(strTemptFileName.c_str(),pBuf,dwInvaSize,dwBytesRead);
		if(nRet == false)
		{
			return false;
		}
		dwCount += dwBytesRead;

		if ( dwFileSize - dwCount < GET_BUFFER_MAX )
			dwRequest = dwFileSize - dwCount;

		//��ʾ����
		nTotalSize += dwBytesRead;
		SetProgressCunPos(hwnd,nTotalSize);
	}
	return true;
}
void CServerUpdateDlg::OnBnClickedButtonStartserver()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}

void CServerUpdateDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
