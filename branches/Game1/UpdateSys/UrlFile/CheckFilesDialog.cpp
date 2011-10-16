// CheckFilesDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "urlfile.h"
#include "CheckFilesDialog.h"
#include "../../PUBLIC/Package.h"
#include <vector>
#include <map>
#include ".\checkfilesdialog.h"
using namespace std;

#define GET_BUFFER_MAX 1024 * 16

#define DISPLAY_UPDATE_CHECK	WM_USER+7000
const char* AcceptTypes = "*/*";



tagDisplayInfo	gCheckDisplayInfo;
HANDLE hCheckThread=0;
bool bCheckExit = false;


// CCheckFilesDialog �Ի���

IMPLEMENT_DYNAMIC(CCheckFilesDialog, CDialog)
CCheckFilesDialog::CCheckFilesDialog(string& strWorkPath,string& strPackageName,string& strFileInfoName,CWnd* pParent /*=NULL*/)
	: CDialog(CCheckFilesDialog::IDD, pParent)
	,m_strWorkPath(strWorkPath)
	,m_strPackageName(strPackageName)
	,m_strFilesInfoName(strFileInfoName)
	,m_strDisplayInfo(_T(""))
	,m_strServerCurDir("")
	,m_strnewVersion("")
	,m_pClientResource(NULL)
	,m_hUpdateSession(NULL)
	,m_hUpdateServerConnect(NULL)
{
}

CCheckFilesDialog::~CCheckFilesDialog()
{
	if(m_pClientResource)
	{
		delete m_pClientResource;
		m_pClientResource=NULL;
	}
	if(m_hUpdateServerConnect)
		::InternetCloseHandle(m_hUpdateServerConnect);
	if(m_hUpdateSession)
		::InternetCloseHandle(m_hUpdateSession);
}

void CCheckFilesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDCANCEL, m_bnCancel);
}


BEGIN_MESSAGE_MAP(CCheckFilesDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CCheckFilesDialog ��Ϣ�������

BOOL CCheckFilesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	bCheckExit = false;

	int cy = ::GetSystemMetrics(SM_CYSCREEN) / 2; 
	int cx = ::GetSystemMetrics(SM_CXSCREEN) / 2; 
	cx = cx - 294/2;
	cy = cy - 100/2;
	MoveWindow(cx,cy,296,102);
	m_Progress.MoveWindow(32,44,240,10);
	m_bnCancel.MoveWindow(125,68,50,20);

	//m_bnCancel.LoadBitmaps(IDB_BITMAP_CANCEL_UP,IDB_BITMAP_CANCEL_DOWN);

	UpdateData(false);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	gCheckDisplayInfo.nCur=0;
	gCheckDisplayInfo.nMax=0;
	gCheckDisplayInfo.strText="";
	m_Progress.SetRange(0,1);
	m_pClientResource = new CClientResource(GAME_RES,m_strWorkPath,m_strFilesInfoName,m_hWnd);

	unsigned threadID;
	hCheckThread = (HANDLE)_beginthreadex(NULL,0,LoadInfoProc,this,0,&threadID);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CCheckFilesDialog::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case DISPLAY_UPDATE_CHECK:
		{
			OnUpdateDisplay();
		}
		break;
	}

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}


void	CCheckFilesDialog::OnUpdateDisplay()
{
	if( m_strDisplayInfo !=gCheckDisplayInfo.strText )
	{
		m_strDisplayInfo=gCheckDisplayInfo.strText;
		CRect myRect;
		myRect.left=6;
		myRect.top=18;
		myRect.right=288;
		myRect.bottom=35;
		//�ػ��ı���ʾ��
		InvalidateRect(&myRect);
	}	

	m_Progress.SetRange(0,200);
	if(gCheckDisplayInfo.nMax==0)
	{
		m_Progress.SetPos(0);
	}
	else
	{
		float fNum =(double)gCheckDisplayInfo.nCur/(double)gCheckDisplayInfo.nMax;
		short nCurpos = (float)200*fNum;
		m_Progress.SetPos(nCurpos);
	}
    
	UpdateData(false);
}

void CCheckFilesDialog::SetDisplayInfo(HWND hwnd,CString strText,int nPos,int nMax)
{
	gCheckDisplayInfo.strText=strText;
	gCheckDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gCheckDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE_CHECK,NULL,NULL);
}
void CCheckFilesDialog::SetDisplayInfo(HWND hwnd,int nPos,int nMax)
{
	gCheckDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gCheckDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE_CHECK,NULL,NULL);
}

unsigned __stdcall CCheckFilesDialog::LoadInfoProc(void* pParam)
{

	CCheckFilesDialog* pCheckDialog = (CCheckFilesDialog*)pParam;

	//װ�ؿͻ�����Դ
	if(LoadClientResource(pCheckDialog)==false)
	{
		::PostMessage(pCheckDialog->m_hWnd,WM_COMMAND,IDOK,NULL);
		_endthreadex( 0 );
		return 0;
	}
	if(LoadCheckServerResource(pCheckDialog)==false)
	{
		::PostMessage(pCheckDialog->m_hWnd,WM_COMMAND,IDOK,NULL);
		_endthreadex( 0 );
		return 0;
	}

	//������Ϣ
	pCheckDialog->GetClientResource()->Save();

	::PostMessage(pCheckDialog->m_hWnd,WM_COMMAND,IDOK,NULL);
	_endthreadex( 0 );
	return 0;
}

bool	CCheckFilesDialog::LoadClientResource(CCheckFilesDialog* pCheckDialog)
{
	if(pCheckDialog==NULL || pCheckDialog->GetClientResource() == NULL)	return false;

	CClientResource* pClientResour = pCheckDialog->GetClientResource();

	int nMax = 10000;
	int nPos = 0;
	CString strTempt("����ͳ�ƿͻ�����Դ��Ϣ�ù��̿�����Ҫ������...");
	SetDisplayInfo(pCheckDialog->m_hWnd,strTempt,nPos,nMax);

	//�����Ϣ
	//pClientResour->Clear();
	//����װ��

	
	pClientResour->LoadPackage(true);

	string strFolder("");
	HANDLE hThread = pClientResour->ReLoadClientResourece(strFolder);

	DWORD dwRes;
	//�̻߳�û�����
	dwRes =::WaitForSingleObject(hThread,500);
	while(dwRes != WAIT_OBJECT_0)
	{
		nPos++;
		if(nMax-10 < nPos)
			nMax += 20;
		SetDisplayInfo(pCheckDialog->m_hWnd,strTempt,nPos,nMax);

		if(bCheckExit==true)
		{
			//�Ⱥ�װ���߳����
			//::WaitForSingleObject(hCheckThread,INFINITE);
			TerminateThread(hThread,0);
			//��ǰ�˳�ɾ���ļ���Ϣ
			//::DeleteFile(pCheckDialog->GetFilesInfoName().c_str());
			return false;
		}
		dwRes =::WaitForSingleObject(hThread,500);
	}
	
	//�������ˣ���������
	SetDisplayInfo(pCheckDialog->m_hWnd,strTempt,nMax,nMax);

	SetDisplayInfo(pCheckDialog->m_hWnd,"����ͳ�ƿͻ�����Դ��Ϣ���.",nMax,nMax);
	return true;;
}

bool	CCheckFilesDialog::LoadCheckServerResource(CCheckFilesDialog* pCheckDialog)
{
	if(pCheckDialog==NULL || pCheckDialog->GetClientResource() == NULL)	return false;

	CClientResource* pClientResour = pCheckDialog->GetClientResource();

	if(bCheckExit==true)	return false;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDisplayInfo(pCheckDialog->m_hWnd,"���Ӹ��·�����...",0,1);
	if(::InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
		SetDisplayInfo(pCheckDialog->m_hWnd,"�����������",1,1);
		return false;
	}

	string strTempt = pClientResour->GetCurFolder();
	strTempt += "\\setup\\CheckFilehostlist.dat";
	FILE* pf = fopen(strTempt.c_str(),"rb");
	if(!pf)
	{
		CString str("��ȡ���·������б��ļ�����,������ļ�����ȡ��ȷ���ļ���");
		//str.Format("��ȡ���·������б��ļ�(%s)����,������ļ�����ȡ��ȷ���ļ���",strTempt);
		SetDisplayInfo(pCheckDialog->m_hWnd,str,1,1);
		return false;
	}
	map<string,string> UpdateServerList;
	while(!feof(pf))
	{
		string strTempt,strServer,strPath;
		CHAR str[100];
		fscanf(pf,"%s\n",str);
		strTempt=str;
		int nPos=strTempt.find_first_of("/");
		if(nPos > 0)
		{
			strServer = strTempt;
			strServer.erase(nPos,strServer.length()-nPos);
			strPath = strTempt;
			strPath.erase(0,nPos);
			UpdateServerList[strServer]=strPath;
		}
	}
	fclose(pf);
	if(UpdateServerList.size()<=0)
	{
		CString str;
		str.Format("��ȡ���·������б��ļ�(%s)����,������ļ�����ȡ��ȷ���ļ���",strTempt);
		SetDisplayInfo(pCheckDialog->m_hWnd,str,1,1);
		return false;
	}
	//Ѱ�������·�����
	HINTERNET hSession = ::InternetOpen(_T("Tnt"),PRE_CONFIG_INTERNET_ACCESS,_T(""),
		INTERNET_INVALID_PORT_NUMBER,0);
	if(hSession == NULL)
	{
		SetDisplayInfo(pCheckDialog->m_hWnd,"�����������",1,1);
		return false;
	}

	HINTERNET hUpdateServerConnect=NULL;
	string strServer("");
	for(map<string,string>::iterator it=UpdateServerList.begin();it != UpdateServerList.end();it++)
	{

		HINTERNET hConnect = ::InternetConnect(hSession,(*it).first.c_str(),
			INTERNET_INVALID_PORT_NUMBER,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
		if(hConnect)
		{
			strServer = (*it).first;
			hUpdateServerConnect = hConnect;
			break;
		}
	}
	if(hUpdateServerConnect==NULL)
	{
		SetDisplayInfo(pCheckDialog->m_hWnd,"���Ӹ��·���������",1,1);
		return false;
	}
	pCheckDialog->SetUpdateServerSessionAndConnect(hSession,hUpdateServerConnect);

	SetDisplayInfo(pCheckDialog->m_hWnd,"���Ӹ��·������ɹ�",1,1);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(bCheckExit==true)	return false;

	SetDisplayInfo(pCheckDialog->m_hWnd,"�ӷ���������ļ��б�...",1,2);

	CFilesInfo ServerFilesInfo,TemptFilesInfo;;

	string strFileName = UpdateServerList[strServer];
	//strFileName +="/FilesInfo.ril"; 
	strFileName +="/PackRes.cfg";
	string strLocalFileName = pCheckDialog->GetWorkPath();
	strLocalFileName += "\\UpdateFilesInfo.ril";
	if(!GetUpdateFileInfos(hUpdateServerConnect,strFileName,strLocalFileName,&ServerFilesInfo))
	{
		SetDisplayInfo(pCheckDialog->m_hWnd,"�ø����ļ��б����.",2,2);
		return false;
	}

	ServerFilesInfo.GetDifferenceByCrc32(*(pClientResour->GetFilesInfo()),TemptFilesInfo);
	SetDisplayInfo(pCheckDialog->m_hWnd,"�ӷ������ø����б�...",2,2);
	//�����ļ�
	int nCount = TemptFilesInfo.GetRootFile()->dwSize;
	int nTotalSize = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(bCheckExit==true)	return false;

	SetDisplayInfo(pCheckDialog->m_hWnd,"�����ļ�...",0,nCount);

	if(DownLoadFiles(hUpdateServerConnect,TemptFilesInfo.GetRootFile(),&TemptFilesInfo,UpdateServerList[strServer],pClientResour,pCheckDialog->m_hWnd,
		nTotalSize)==false)
	{
		SetDisplayInfo(pCheckDialog->m_hWnd,"����ʧ�ܡ�",nCount,nCount);
		return false;
	}


	WIN32_FIND_DATA myWFD;
	CString strUpdateFileName = "update.new";
	HANDLE hl = FindFirstFile(strUpdateFileName.GetBuffer(),&myWFD);

	tagFileInfo* pFileInfo = pClientResour->GetFilesInfo()->GetFileInfoByText("\\update.new");
	if ( hl != INVALID_HANDLE_VALUE && pFileInfo)
	{
		//SetDisplayInfo(pCheckDialog->m_hWnd,"������������...",0,6);
		//remove("update.old");

		//SetDisplayInfo(pCheckDialog->m_hWnd,"������������...",1,6);

		//if ( rename("update.exe","update.old"))
		//{
		//	SetDisplayInfo(pCheckDialog->m_hWnd,"������������ʧ�ܡ�",6,6);
		//	return false;
		//}
		//SetDisplayInfo(pCheckDialog->m_hWnd,"������������...",2,6);

		//if ( rename("update.new","update.exe") )
		//{
		//	SetDisplayInfo(pCheckDialog->m_hWnd,"������������ʧ�ܡ�",6,6);
		//	return false;
		//}


		//SetDisplayInfo(pCheckDialog->m_hWnd,"������������ɹ���",3,6);

		////����updater.exe���ļ���Ϣ		
		//string strUpdater("\\update.exe");
		//pClientResour->GetFilesInfo()->UpdateFileInfo(strUpdater,pFileInfo->dwCrc32,
		//	pFileInfo->dwSize,pFileInfo->dwSize,false,0,0);

		//���������ĸĶ�
		SetDisplayInfo(pCheckDialog->m_hWnd,"������Ϣ...",4,6);
		pClientResour->GetFilesInfo()->SetVersion(ServerFilesInfo.GetVersion());
		pClientResour->UpdateSave();
		SetDisplayInfo(pCheckDialog->m_hWnd,"������Ϣ�ɹ�",5,6);

		SetDisplayInfo(pCheckDialog->m_hWnd,"������ϣ�������������������",6,6);
		FindClose(hl);

		WinExec("upself.dat",SW_SHOWNORMAL);
		pCheckDialog->OnCancel();
		return false;
	}


	//���������ĸĶ�
	SetDisplayInfo(pCheckDialog->m_hWnd,"������Ϣ...",0,2);
	pClientResour->GetFilesInfo()->SetVersion(ServerFilesInfo.GetVersion());
	pClientResour->UpdateSave();
	SetDisplayInfo(pCheckDialog->m_hWnd,"������Ϣ�ɹ�",2,2);


	SetDisplayInfo(pCheckDialog->m_hWnd,"�������.",nCount,nCount);
	return true;
}


//�õ������������ļ���Ϣ
bool	CCheckFilesDialog::GetUpdateFileInfos(HINTERNET hConnect,string& strPathName,string& strLocalFileName,CFilesInfo* pFilesInfo)
{
	if(pFilesInfo==NULL || hConnect==NULL)	return false;



	//if(!::FtpGetFile(hConnect,strPathName.c_str(),strLocalFileName.c_str(),FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY |
	//	INTERNET_FLAG_NO_CACHE_WRITE,0))
	//{
	//	//InternetCloseHandle(hFtpFile);
	//	return false;
	//}
	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
		strPathName.c_str(),HTTP_VERSION,NULL,0,
		INTERNET_FLAG_RELOAD,0);
	if(hHttpFile == NULL)
	{
		return false;
	}
	bool bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(bSendRequest)
	{
		char	achQueryBuf[16];
		DWORD	dwFileSize;
		DWORD	dwQueryBufLen = sizeof(achQueryBuf);

		bool	bQuery = ::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH,achQueryBuf,&dwQueryBufLen,NULL);

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
		bool	bRead;

		DWORD dwCount = 0;
		DWORD dwRequest = ( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
		while (true)
		{
			bRead = ::InternetReadFile(hHttpFile,strBuffer,dwRequest,&dwBytesRead);
			if(!bRead)
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

		pFilesInfo->Load(strLocalFileName);
		::DeleteFile(strLocalFileName.c_str());	
		return	true;
	}
	return false;
}


//�����ļ��б�
bool	 CCheckFilesDialog::DownLoadFiles(HINTERNET hConnect,tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,string& strFtpPath,
										CClientResource* pResource,HWND hwnd,int& nTotalSize)
{
	if(ptagFileInfo==NULL || pFilesInfo==NULL || pResource==NULL )	return false;

	if(pFilesInfo->HaveChild(ptagFileInfo))
	{
		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			if(!DownLoadFiles(hConnect,(*it).second,pFilesInfo,strFtpPath,pResource,hwnd,nTotalSize))
				return false;
		}
	}
	else if(ptagFileInfo->bFolder == false)
	{
		string strTemptFileName = pFilesInfo->GetFileTextWithParent(ptagFileInfo);
		
		string strServerFilename = strFtpPath+strTemptFileName;

		//����Ǹ����ļ��Լ�����ı�����
		if( strTemptFileName == "\\update.exe" )
		{
			strTemptFileName="\\update.new";
		}

		HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
			strServerFilename.c_str(),HTTP_VERSION,NULL,0,
			INTERNET_FLAG_RELOAD,0);
		if(hHttpFile == NULL)
		{
			return false;
		}
		bool bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
		if(bSendRequest)
		{
			char	achQueryBuf[16];
			DWORD	dwQueryBufLen = sizeof(achQueryBuf);

			//bool	bQuery = ::HttpQueryInfo(hHttpFile,
			//	HTTP_QUERY_CONTENT_LOCATION,achQueryBuf,&dwQueryBufLen,NULL);
			///�������ϲ����ڴ��ļ�
			//if(bQuery==false)
			//	return false;

			bool bQuery = ::HttpQueryInfo(hHttpFile,
				HTTP_QUERY_CONTENT_LENGTH,achQueryBuf,&dwQueryBufLen,NULL);

			if(!bQuery)
			{
				::InternetCloseHandle(hHttpFile);
				return false;
			}

			DWORD   dwCount=0;
			DWORD   dwFileSize;

			tagFileInfo* pCurFileInfo = pResource->GetFilesInfo()->GetFileInfoByText(strTemptFileName);

			if(pCurFileInfo)
			{
				if(ptagFileInfo->dwCrc32==pCurFileInfo->dwCrc32)
				{
					dwCount = pCurFileInfo->dwValidSize;
					int nt = InternetSetFilePointer(hHttpFile,dwCount,NULL,FILE_BEGIN,0);
					if(nt==-1)
					{
						dwCount=0;
						dwFileSize = (DWORD) atol(achQueryBuf);
						pResource->OnDelFileFolder(strTemptFileName,strTemptFileName);
						pResource->AddEmptyFile(strTemptFileName.c_str(),dwFileSize,ptagFileInfo->dwCrc32,ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType, ptagFileInfo->dwOrginSize);
					}
					else
					{
						dwCount=pCurFileInfo->dwValidSize;
						dwFileSize = pCurFileInfo->dwSize;
					}
				}
				else
				{	//CRC��ͬ��������
					dwCount=0;
					dwFileSize = (DWORD) atol(achQueryBuf);
					pResource->OnDelFileFolder(strTemptFileName,strTemptFileName);
					pResource->AddEmptyFile(strTemptFileName.c_str(),dwFileSize,ptagFileInfo->dwCrc32,ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType, ptagFileInfo->dwOrginSize);
				}
			}
			else
			{
				dwCount=0;
				dwFileSize = (DWORD) atol(achQueryBuf);
				pResource->AddEmptyFile(strTemptFileName.c_str(),dwFileSize,ptagFileInfo->dwCrc32,ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType, ptagFileInfo->dwOrginSize);
			}


			BYTE lpszBuf[GET_BUFFER_MAX];

			DWORD	dwBytesRead;
			BOOL	bRead;
			DWORD   dwRequest = ( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
			while(dwRequest > 0)
			{
				bRead = ::InternetReadFile(hHttpFile,lpszBuf,dwRequest,&dwBytesRead);
				if(!bRead)
				{
					return false;
				}

				pResource->AddFileData(strTemptFileName.c_str(),lpszBuf,dwBytesRead);

				dwCount += dwBytesRead;
				if ( dwFileSize - dwCount < GET_BUFFER_MAX )
					dwRequest = dwFileSize - dwCount;
			}
			pResource->AdjustFile(strTemptFileName.c_str(),ptagFileInfo->dwPackageType,ptagFileInfo->dwCompressType);

			nTotalSize += ptagFileInfo->dwSize;
			SetDisplayInfo(hwnd,nTotalSize);

			InternetCloseHandle(hHttpFile);
		}
	}
	return true;
}

void CCheckFilesDialog::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	bCheckExit = true;

	::WaitForSingleObject(hCheckThread,INFINITE);

   // string strFileInfoName = m_pClientResource->GetCurFolder()+"\\FilesInfo.ril";

	//::DeleteFile(strFileInfoName.c_str());
	CDialog::OnCancel();
}

void CCheckFilesDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rect;
	GetClientRect(&rect);

	/*CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bpt;
	bpt.LoadBitmap(IDB_BITMAP_GENGXIN);
	BITMAP bm;
	bpt.GetBitmap(&bm);
	memDC.SelectObject(&bpt);
	dc.BitBlt(0,0, bm.bmWidth , bm.bmHeight , &memDC, 0, 0, SRCCOPY);*/

	////����������
	CFont myFont;
	myFont.CreatePointFont(90,"����");

	dc.SelectObject(&myFont);
	dc.SetTextColor(RGB(255,255,255));

	//����CHECT˵���ľ��δ���
	CRect myRect;
	myRect.left=6;
	myRect.top=18;
	myRect.right=288;
	myRect.bottom=35;

	//����͸���ķ�ʽ����Ļ��д��
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strDisplayInfo,&myRect,DT_LEFT);
}

void CCheckFilesDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
