// InitInfoDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <Mmsystem.h>
#include "urlfile.h"
#include "InitInfoDialog.h"
#include ".\initinfodialog.h"
#include "Ping.h"

#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/Crc32Static.h"
#include <vector>
#include <map>
#include <time.h>
#include <process.h>
using namespace std;

#define GET_BUFFER_MAX 1024 * 16

#define DISPLAY_UPDATE	WM_USER+6000
const char* FAcceptTypes = "*/*";

tagDisplayInfo	gDisplayInfo;
HANDLE hInfoThread=0;
bool bExit = false;
// CInitInfoDialog �Ի���

struct tagServer
{
	string	strIP;
	string	strPath;
};

IMPLEMENT_DYNAMIC(CInitInfoDialog, CDialog)
CInitInfoDialog::CInitInfoDialog(string& strWorkPath,string& strPackageName,string& strFileInfoName,CWnd* pParent /*=NULL*/)
	: CDialog(CInitInfoDialog::IDD, pParent)
	,m_strWorkPath(strWorkPath)
	,m_strPackageName(strPackageName)
	,m_strFilesInfoName(strFileInfoName)
	,m_strDisplayInfo(_T(""))
	,m_strServerCurDir("")
	,m_strnewVersion("")
	,m_pClientResource(NULL)
	,m_hUpdateSession(NULL)
	,m_hUpdateServerConnect(NULL)
	,m_bReadError(false)
{
	//��ʼ����Դ��ľ�̬����
}

CInitInfoDialog::~CInitInfoDialog()
{
	Release();
}

void	  CInitInfoDialog::Release()
{
	if(m_pClientResource )
	{
		delete m_pClientResource;
		m_pClientResource=NULL;
	}

	if(m_hUpdateServerConnect)
		::InternetCloseHandle(m_hUpdateServerConnect);
	if(m_hUpdateSession)
		::InternetCloseHandle(m_hUpdateSession);
}

void CInitInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDCANCEL, m_bnCancel);
	DDX_Control(pDX, IDC_DOWNLOADHINTINFO, m_DownloadHintInfo);
}


BEGIN_MESSAGE_MAP(CInitInfoDialog, CDialog)
//	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CInitInfoDialog ��Ϣ�������

BOOL CInitInfoDialog::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: �ڴ����ר�ô����/����û���
	switch(message)
	{
	case DISPLAY_UPDATE:
		{
			OnUpdateDisplay();
		}
		break;
	case TREELIST_ADD:
		{
			//if(gDisplayInfo.nCur<gDisplayInfo.nMax)
			//{
			//	gDisplayInfo.nCur++;
			//	OnUpdateDisplay();
			//}
		}
		break;
	}

	return CDialog::OnWndMsg(message, wParam, lParam, pResult);
}

BOOL CInitInfoDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	int cy = ::GetSystemMetrics(SM_CYSCREEN) / 2; 
	int cx = ::GetSystemMetrics(SM_CXSCREEN) / 2; 
	cx = cx - 560/2;
	cy = cy - 430/2;
	MoveWindow(cx,cy,560,430);
	m_DownloadHintInfo.MoveWindow(5,4,546,296);
	m_Progress.MoveWindow(5,330,546,20);
	m_bnCancel.MoveWindow(225,360,112,30);

	SetWindowText("������");//���öԻ���ı���

	UpdateData(false);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CWnd* pMainWnd = FindWindow("OnlineCLIENT",NULL);
	if (pMainWnd != NULL)
	{
		AfxMessageBox(_T("����������Ѿ��򿪣����ȹر������������и��³���"));
		return TRUE;
	}

	m_bReadError=false;
	m_bIDOK		=false;
	gDisplayInfo.nCur=0;
	gDisplayInfo.nMax=0;
	gDisplayInfo.strText="";
	m_Progress.SetRange(0,1);
	m_pClientResource = new CClientResource(GAME_RES,m_strWorkPath,m_strFilesInfoName);

	unsigned threadID;
	hInfoThread = (HANDLE)_beginthreadex(NULL,0,LoadInfoProc,this,0,&threadID);	

	IniDownloadHintWeb();
	m_DownloadHintInfo.Navigate(m_strDownloadHitAddress.c_str(),FALSE,FALSE,FALSE,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

//��ʼ����Ϣ��ʾweb,���internet webpage�����ã��ñ���webpage;
void CInitInfoDialog::IniDownloadHintWeb(void)
{
	m_strDownloadHitAddress = "";
	vector<string>		HintList;
	CHAR* chBuffer = new CHAR[250];
	ifstream stream;
	stream.open("setup/downloadhintserver.ini");

	if(!stream.is_open())
	{
		delete []chBuffer;
		return;
	}
	while( !stream.eof() )
	{
		stream >> chBuffer;
		if (lstrlen(chBuffer) > 0)
			HintList.push_back(chBuffer);
	}
	if(HintList.size() ==0)
	{
		delete []chBuffer;
		stream.close();
		return;
	}

	srand((unsigned)time(NULL));
	int nRandom  = rand();
	nRandom = nRandom%(HintList.size());
	m_strDownloadHitAddress = HintList[nRandom];
	delete []chBuffer;
	stream.close();
}

void CInitInfoDialog::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	bExit = true;

	DWORD dwResult = ::WaitForSingleObject(hInfoThread,6000);
	if(dwResult == WAIT_TIMEOUT)
	{
		TerminateThread(hInfoThread,0);
	}
	//m_pClientResource->UpdateSave();

	if(m_bReadError==true)
		Release();
	CDialog::OnCancel();
}

void	CInitInfoDialog::OnUpdateDisplay()
{
	if(m_strDisplayInfo != gDisplayInfo.strText)
	{
		m_strDisplayInfo=gDisplayInfo.strText;
		//bFlag = true;
		CRect myRect;
		myRect.left=5;
		myRect.top=310;
		myRect.right=500;
		myRect.bottom=325;
		//�ػ��ı���ʾ��
		InvalidateRect(&myRect);
	}
	

	m_Progress.SetRange(0,10000);
	if(gDisplayInfo.nMax==0)
	{
		m_Progress.SetPos(0);
	}
	else
	{
		float fNum = (double)gDisplayInfo.nCur/(double)gDisplayInfo.nMax;
		short nCurpos = (float)10000*fNum;
		m_Progress.SetPos(nCurpos);
	}

	UpdateData(false);
}

void CInitInfoDialog::SetDisplayInfo(HWND hwnd,CString strText,int nPos,int nMax)
{
	gDisplayInfo.strText=strText;
	gDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE,NULL,NULL);
	Sleep(2);
}
void CInitInfoDialog::SetDisplayInfo(HWND hwnd,int nPos,int nMax)
{
	gDisplayInfo.nCur=nPos;
	if(nMax!=-1)
		gDisplayInfo.nMax=nMax;
	::PostMessage(hwnd,DISPLAY_UPDATE,NULL,NULL);
	Sleep(2);
}

unsigned __stdcall CInitInfoDialog::LoadInfoProc(void* pParam)
{
	
	CInitInfoDialog* pInfoDialog = (CInitInfoDialog*)pParam;

	//װ�ؿͻ�����Դ
	if(LoadClientResource(pInfoDialog)==false)
	{
		//if(pInfoDialog->m_bReadError==true)
		//{
		//	//ҪУ���ļ�
		//	::PostMessage(pInfoDialog->m_hWnd,WM_COMMAND,IDOK,NULL);
		//}
		_endthreadex( 0 );
		return 0;
	}

	if(bExit==true)
		return false;

	if(LoadUpdateServerResource(pInfoDialog)==false)
	{
		_endthreadex( 0 );
		return 0;
	}

	pInfoDialog->m_bIDOK = true;
	::PostMessage(pInfoDialog->m_hWnd,WM_COMMAND,IDOK,NULL);
	_endthreadex( 0 );
	return 0;
}

//װ�ؿͻ�����Դ
bool	CInitInfoDialog::LoadClientResource(CInitInfoDialog* pInfoDialog)
{
	if(pInfoDialog==NULL || pInfoDialog->GetClientResource() == NULL)	return false;

	CClientResource* pClientResour = pInfoDialog->GetClientResource();

	int nMax = 1;
	int nPos = 0;
	SetDisplayInfo(pInfoDialog->m_hWnd,"��ȡ�ͻ����ļ���Ϣ...",nPos,nMax);

	if(pClientResour->LoadExForAutoUpdate()==false)
	{
		SetDisplayInfo(pInfoDialog->m_hWnd,"��ȡ�ͻ�����Ҫ�ļ���Ϣ���������³��ԡ�[�ٷ���ҳ www.saga3d.com]",nMax,nMax);

		AfxMessageBox("�Զ�����ʧ�ܣ������Ե� www.saga3d.com ȥ�������µĲ������ļ���");
		ShellExecute( 0, 0, "http://www.saga3d.com", 0, 0, SW_SHOWNORMAL );

		pInfoDialog->m_bReadError=true;
		return false;
	}

	SetDisplayInfo(pInfoDialog->m_hWnd,"��ȡ�ͻ����ļ���Ϣ...",nMax,nMax);

	char strVersion[250];
	sprintf(strVersion,"��ǰ��Ϸ�汾��Ϊ%s",pClientResour->GetFilesInfo()->GetVersion().c_str());
	pInfoDialog->SetWindowText(strVersion);
	return true;;
}

bool CInitInfoDialog::LoadUpdateServerResource(CInitInfoDialog* pInfoDialog)
{
	if(pInfoDialog==NULL || pInfoDialog->GetClientResource() == NULL)	return false;

	CClientResource* pClientResour = pInfoDialog->GetClientResource();

	if(bExit==true)	return false;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SetDisplayInfo(pInfoDialog->m_hWnd,"��ȡ�������б��ļ�...",0,2);
	
	if(::InternetAttemptConnect(0) != ERROR_SUCCESS)
	{
		SetDisplayInfo(pInfoDialog->m_hWnd,"���Ӹ��·�����ʧ�ܣ������������������Ƿ�������[�ٷ���ҳ www.saga3d.com]",1,1);
		return false;
	}

	string strTempt = pClientResour->GetCurFolder();
	strTempt += "\\setup\\Updatehostlist.ini"; 

	FILE* pf = fopen(strTempt.c_str(),"rb");
	if(!pf)
	{
		CString str("��ȡ���·������б��ļ������뵽 www.saga3d.com �����������������������Ϸ��");
		SetDisplayInfo(pInfoDialog->m_hWnd,str,1,1);
		return false;
	}

	CPing ping;
	string strUpdateServer("");
	long lTimeOut = 2000;
	multimap<long,tagServer> UpdateServerList;

	string str; //�洢�ļ��ж�ȡ���ַ���
	//���ļ���ȡ���·������б�
	char strbuf[100];
	string strch,strtmp;
	while(!feof(pf))
	{
	   fscanf(pf,"%s\n",strbuf);
	   strtmp = strbuf;
	   strch+=strtmp;
	}
	fclose(pf);
	str = strch;
	int Sever = 0;   //0��ʾѡ����ŷ�������1��ʾ��ͨ������
	int Pos = str.find_first_of("@");    //@Ϊ���ź���ͨ�������ķֽ��
	if(Sever == 0)
	{
		//cout<<"���ŷ�������"<<endl;
		str.erase(Pos,str.length()-Pos);  //�������ŷ������Ķ�
	}
	else if(Sever == 1)
	{
		//cout<<"��ͨ��������"<<endl;
		str.erase(0,Pos+1);   //������ͨ�������Ķ�

	}
	int nPos=-1;
    string strTemp(""); 
	string	strServer("");
	string	strPath("");
	string strVersion("");
	bool isSelect = false;
	DWORD dwFileInfoCRC;
	//ѭ������ѡ�����ÿһ����������IP��Path
	SetDisplayInfo(pInfoDialog->m_hWnd,"���Ӹ��·�����...",1,2);
	while(str.find_first_of(",")!=-1)
	{
		nPos=str.find_first_of(",");

		strTemp = str;
		strTemp.erase(0,nPos+1);
		str.erase(nPos,str.length()-nPos);
		string strPathTemp,strIpTemp;

		int npos=str.find_first_of("#");
		strPathTemp = str;
		strPathTemp.erase(npos,str.length()-npos);
		strIpTemp = str;
		strIpTemp.erase(0,npos+1);

		tagServer Server = {strIpTemp,strPathTemp};
		//�õ�pingֵ ,Ѱ�������·�����
		long lRet = ping.Ping((char*)strIpTemp.c_str(),1,2000);
		if(lRet != -1 && lRet < 100)
		{
			strServer = strIpTemp;
			strPath = strPathTemp;
			isSelect = true;
			break;
		}else if(lRet!=-1&&lRet<lTimeOut)
		{
			UpdateServerList.insert(make_pair(lRet,Server));
		}else{
			srand((unsigned)time(NULL));
			int nRandom  = rand()+2000;
			UpdateServerList.insert(make_pair(nRandom,Server));
		}
		str = strTemp;
	}

	if(UpdateServerList.size()<=0&&isSelect==false)
	{
		CString str("��ȡ���·������б��ļ�����");
		SetDisplayInfo(pInfoDialog->m_hWnd,str,1,1);
		return false;
	}
	//�ҳ�pingֵ��С�ĸ��·�����
	if(!isSelect)
	{
			multimap<long,tagServer>::iterator it = UpdateServerList.begin();
			long minRel = (*it).first;
			strServer = (*it).second.strIP;
			strPath = (*it).second.strPath;
			for(;it != UpdateServerList.end();it++)
			{
				if((*it).first<minRel)
				{
					minRel = (*it).first;
					strServer = (*it).second.strIP;
					strPath = (*it).second.strPath;
				}
			}
	}

	HINTERNET hSession = ::InternetOpen(_T("Tnt"),INTERNET_OPEN_TYPE_PRECONFIG,_T(""),
		INTERNET_INVALID_PORT_NUMBER,0);
	if(hSession == NULL)
	{		
		SetDisplayInfo(pInfoDialog->m_hWnd,"���Ӹ��·�����ʧ�ܣ������������������Ƿ�������[�ٷ���ҳ www.saga3d.com]",1,1);
		return false;
	}
	SetDisplayInfo(pInfoDialog->m_hWnd,"������...",2,2);
	if(bExit==true)	return false;
	HINTERNET hUpdateServerConnect=NULL;

	int nAllPro = UpdateServerList.size()+2;
	SetDisplayInfo(pInfoDialog->m_hWnd,"������...",1,nAllPro);
	if(GetServerVersionFromOneGameServer(pInfoDialog,hSession,strServer,strPath,
			hUpdateServerConnect,dwFileInfoCRC,strVersion) == false)
	{
		char strTempt[256] = "���Ӹ��·�����ʧ�ܣ������������������Ƿ�������[�ٷ���ҳ www.saga3d.com]";
		SetDisplayInfo(pInfoDialog->m_hWnd,strTempt,1,1);
		return false;
	}

	SetDisplayInfo(pInfoDialog->m_hWnd,"������...",nAllPro-1,nAllPro);
	string strOldVersion = pClientResour->GetFilesInfo()->GetVersion();
	//�汾һ���˳�,�������ý���
	if(strVersion==strOldVersion)
	{
		SetDisplayInfo(pInfoDialog->m_hWnd,"��ǰ�����°汾��",nAllPro,nAllPro);
		return	true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(bExit==true)	return false;
	//Ϊ��ǰ�ͻ��˰汾���÷������ĸ���Ŀ¼,�����ͻ��˰汾����Ϣ
	strPath = strPath + "/Update-" + strOldVersion;

    SetDisplayInfo(pInfoDialog->m_hWnd,"�ӷ������ø����б�...",1,2);

	//CFilesInfo ServerFilesInfo,TemptFilesInfo;;
	string ServerFilesName("");
	string ServerFilesCRC("");

	string strFileName1 = strPath;
	strFileName1 +="/FilesInfo.txt";
	string strLocalFileName = pInfoDialog->GetWorkPath();
	strLocalFileName += "\\UpdateFilesInfo.ril";
	if(!GetUpdateFileInfosEx(hUpdateServerConnect,strFileName1,strLocalFileName,ServerFilesCRC,ServerFilesName))
	{
		SetDisplayInfo(pInfoDialog->m_hWnd,"���ظ����б�������ٴγ������ӣ����ߵ� www.saga3d.com ������������",2,2);
	 	return false;
	}

	int nTotalSize = 0;

	//����һ����ʱ�ļ��������ݴ����ص��ļ���Ҳͬʱ��ɶ˵�����
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(bExit==true)	return false;

	BYTE *pBuf = new BYTE[GET_BUFFER_MAX];
	DWORD dwBufLen = GET_BUFFER_MAX;
	int nCount=2;
	//�ȳ��������ļ�
	SetDisplayInfo(pInfoDialog->m_hWnd,"�����ļ�...",0,nCount);
	string ServerFile = strPath+"/"+ServerFilesName;  //��������ļ��ڷ������ϵ�����·��
	if(ContinueDownLoadEx(hUpdateServerConnect,strOldVersion,pBuf,dwBufLen,ServerFile,nTotalSize,pInfoDialog->m_hWnd))
	{
		string ServerTemp = "update\\Game"+strOldVersion+"Patch.exe";
		SetDisplayInfo(pInfoDialog->m_hWnd,"�������.",2,2);
		WinExec(ServerTemp.c_str(),SW_SHOWNORMAL);
		bExit = true;
		::PostMessage(pInfoDialog->m_hWnd,WM_CLOSE,NULL,NULL);  //�����������ɹ��رմ���
		_endthreadex( 0 );
		return true;
	}
	else{
		if(DownLoadEXEFile(hUpdateServerConnect,strOldVersion,pBuf,dwBufLen,ServerFile,nTotalSize,pInfoDialog->m_hWnd))
		{
		   string ServerTemp = "update\\Game"+strOldVersion+"Patch.exe";
		   SetDisplayInfo(pInfoDialog->m_hWnd,"�������.",2,2);
		   WinExec(ServerTemp.c_str(),SW_SHOWNORMAL);
		   bExit = true;
		   ::PostMessage(pInfoDialog->m_hWnd,WM_CLOSE,NULL,NULL);  //�����������ɹ��رմ���
		   _endthreadex( 0 );
		   return true;
		}
		else{
			SetDisplayInfo(pInfoDialog->m_hWnd,"�ļ����·����������ٴγ������ӣ����ߵ� www.saga3d.com ������������",nCount,nCount);
			return false;
		}
	}
	//return true;
}

bool CInitInfoDialog::GetServerVersionFromOneGameServer(CInitInfoDialog* pInfoDialog,HINTERNET hSession,
														string& strUrl1,string& strServerName,
														HINTERNET& hUpdateServerConnect,DWORD& dwFileInfoCRC,string& strVersion)
{
	//�����Ƿ��ܴ򿪷�����
	string strUrl = "http://"+strUrl1;
	strUrl +=strServerName;
	HINTERNET hOpen = ::InternetOpenUrl(hSession,strUrl.c_str(),NULL,0,INTERNET_FLAG_EXISTING_CONNECT,NULL);
	if(hOpen)
	{
		InternetCloseHandle(hOpen);
		HINTERNET hConnect = ::InternetConnect(hSession,strUrl1.c_str(),
			INTERNET_INVALID_PORT_NUMBER,_T(""),_T(""),INTERNET_SERVICE_HTTP,0,0);
		if(hConnect)
		{
			hUpdateServerConnect = hConnect;
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

	string strFileName = strServerName;
	//strFileName +="/AutoUpdateGame.vs";
	strFileName +="/AutoUpdateGame.txt";
	string strLocalVersionFileName = pInfoDialog->GetWorkPath();
	strLocalVersionFileName += "\\AutoUpdateGame.vs";
	if(GetServerVersion(hUpdateServerConnect,strFileName,strLocalVersionFileName,dwFileInfoCRC,strVersion)==false)
	{
		InternetCloseHandle(hUpdateServerConnect);
		return false;
	}
	return true;
}
//�õ���������Ϸ�汾
bool CInitInfoDialog::GetServerVersion(HINTERNET hConnect,string& strPathName,string& strLocalVersionName,
									   DWORD& dwFileInfoCRC,string& strVersion)
{

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
		return	true;
	}
	else
	{
		DWORD dwError = GetLastError();
		return false;
	}
	//return	false;
}

//�õ������������ļ���Ϣ
bool	CInitInfoDialog::GetUpdateFileInfosEx(HINTERNET hConnect,string& strPathName,string& strLocalFileName,
											string &dwFileInfoCRC,string &ServerFilesName)
{
	if(hConnect==NULL)	return false;

	

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
	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(bSendRequest)
	{
		DWORD dwContentLen;
        DWORD dwBufLen = sizeof(dwContentLen);

		BOOL	bQuery = ::HttpQueryInfo(hHttpFile,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwContentLen,&dwBufLen,NULL);

		if(!bQuery)
		{
			::InternetCloseHandle(hHttpFile);
			return false;
		}
		
		DWORD	dwFileSize = dwContentLen;
		FILE* pf = fopen(strLocalFileName.c_str(),"wb");
		if(!pf)
		{
			::InternetCloseHandle(hHttpFile);
			return	false;
		}
		//dwFileSize = (DWORD) atol(achQueryBuf);				
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
		//����ʱ�����ļ��ж�ȡ������Դ�����ֺ�CRC��
		FILE* pf_read = fopen(strLocalFileName.c_str(),"rb");
		char strbuf[100];
		fscanf(pf_read,"%s\n",strbuf);
		ServerFilesName = strbuf;
		fscanf(pf_read,"%s\n",strbuf);
		dwFileInfoCRC = strbuf;
		fclose(pf_read);
		string strfile = "UpdateFilesInfo.ril"; 
		::DeleteFile(strfile.c_str());	
		return	true;
	}
	return false;
}

bool    CInitInfoDialog::ContinueDownLoadEx(HINTERNET hConnect,string strOldVersion,BYTE* pBuf,DWORD& dwBufLen,
		                        string& strFtpPath,int& nTotalSize,HWND hwnd)
{
	if(hConnect==NULL)
		return false;
	//��updateĿ¼�µ���ʱ�ļ������ʧ�ܣ��˳��ϵ�����
	FILE *fp_date = fopen("update//updateflie.fc","rb");
	if(fp_date==NULL)
	{
		return false;
	}
	//����򿪳ɹ���˵�����ϴ�δ������ɵ��ļ�
	DWORD dwCount=0;
	fread(&dwCount,1,sizeof(DWORD),fp_date);
	fclose(fp_date);
	if(dwCount==0)
	{
		//��ȡ����ɾ�����ļ�
		//string strFullFileName = pInfoDialog->GetWorkPath();
	    string strFullFileName = "update\\updateflie.fc";
	    ::DeleteFile(strFullFileName.c_str());
	}
	//string strOldVersion = pClientResour->GetFilesInfo()->GetVersion();
	//����ļ������Ƿ����δ�������.exe�������ļ�����������ڣ������ϵ�����
	string UpdateFile = "update\\Game"+strOldVersion+"Patch.exe";
	FILE *fp_Temp = fopen(UpdateFile.c_str(),"rb");
	if(fp_Temp==NULL)
	{
		return false;
	}
	fclose(fp_Temp);
	FILE *fp_f = fopen(UpdateFile.c_str(),"wb");
	if(fp_f==NULL)
	{
		return false;
	}
	//���ļ�ָ��ָ���������ļ���ĩβ
	fseek(fp_f,dwCount,0);
	nTotalSize = dwCount;
	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
			strFtpPath.c_str(),HTTP_VERSION,NULL,0,
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_DONT_CACHE |
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION,0);
	if(hHttpFile == NULL)	return false;

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(!bSendRequest)	return false;

	DWORD dwContentLen;
	DWORD dwBufLenth = sizeof(dwContentLen);
	bool bQuery =::HttpQueryInfo(hHttpFile, 
			  HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
			  (LPVOID)&dwContentLen,
			  &dwBufLenth,
			  0);

	if(!bQuery)
	{
		::InternetCloseHandle(hHttpFile);
		return false;
	}
	DWORD	dwServerFileSize = dwContentLen;//(DWORD) atol(achQueryBuf);
	//���ô������ʼ��
	DWORD nt = InternetSetFilePointer(hHttpFile,dwCount,NULL,FILE_BEGIN,0);
	if(nt==-1||nt!=dwCount)
	{
		//��֧�ֶϵ�����
		::InternetCloseHandle(hHttpFile);
		string strFullFileName = "update\\updateflie.fc";
	    ::DeleteFile(strFullFileName.c_str());
		//::DeleteFile(UpdateFile.c_str());
		fclose(fp_f);
		//pPckage->ClearData();
		return false;
	}
	nTotalSize = dwCount;
	DWORD	dwBytesRead = 0;
	BOOL	bRead;
	DWORD   dwFileSize = dwServerFileSize;
	DWORD   dwRequest =
		( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
	while(dwRequest > 0)
	{
		bRead = ::InternetReadFile(hHttpFile,pBuf,dwRequest,&dwBytesRead);

		if(!bRead || bExit == true)
		{
			return false;
		}
		if(dwBytesRead == 0)
			break;
		fwrite(pBuf,dwBytesRead,1,fp_f);   //�����ص����������뵽�ļ���
		dwCount += dwBytesRead;
		FILE *fp_d = fopen("update//updateflie.fc","wb");
		if(fp_d==NULL) return false;
		fwrite(&dwCount,1,sizeof(DWORD),fp_d);
		fclose(fp_d);
		if ( dwFileSize - dwCount < GET_BUFFER_MAX )
			dwRequest = dwFileSize - dwCount;

		//��ʾ����
		nTotalSize += dwBytesRead;
		CString strFileSize("");
		strFileSize.Format("�����ļ�...                                                        %d/%d",nTotalSize,dwFileSize);
		SetDisplayInfo(hwnd,strFileSize,nTotalSize,dwFileSize);
		//SetDisplayInfo(hwnd,nTotalSize,dwFileSize);
	}
	fclose(fp_f);
	return true;

}

bool    CInitInfoDialog::DownLoadEXEFile(HINTERNET hConnect,string strOldVersion,BYTE* pBuf,DWORD& dwBufLen,
		                        string& strFtpPath,int& nTotalSize,HWND hwnd)
{
	if(hConnect==NULL)
       return false;
	//string strOldVersion = pClientResour->GetFilesInfo()->GetVersion();
	string UpdateFile = "update\\Game"+strOldVersion+"Patch.exe";
	FILE *fp_f = fopen(UpdateFile.c_str(),"wb");
	if(fp_f==NULL)
	{
		return false;
	}
	nTotalSize = 0;
	DWORD dwCount=0;
	HINTERNET hHttpFile = ::HttpOpenRequest(hConnect,_T("GET"),
			strFtpPath.c_str(),HTTP_VERSION,NULL,0,
			INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_DONT_CACHE |
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_KEEP_CONNECTION,0);
	if(hHttpFile == NULL)	return false;

	BOOL bSendRequest = ::HttpSendRequest(hHttpFile,NULL,0,0,0);
	if(!bSendRequest)	return false;

	DWORD dwContentLen;
    DWORD dwBufLenth = sizeof(dwContentLen);
	bool bQuery =::HttpQueryInfo(hHttpFile, 
              HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
              (LPVOID)&dwContentLen,
              &dwBufLenth,
              0);

	if(!bQuery)
	{
		::InternetCloseHandle(hHttpFile);
		return false;
	}
	DWORD	dwServerFileSize = dwContentLen;//(DWORD) atol(achQueryBuf);
	DWORD   dwFileSize = dwServerFileSize;
	DWORD	dwBytesRead = 0;
	BOOL	bRead;
	DWORD   dwRequest =
		( GET_BUFFER_MAX > dwFileSize - dwCount ) ? dwFileSize - dwCount : GET_BUFFER_MAX;
	while(dwRequest > 0)
	{
		bRead = ::InternetReadFile(hHttpFile,pBuf,dwRequest,&dwBytesRead);

		if(!bRead || bExit == true)
		{
			return false;
		}
		if(dwBytesRead == 0)
			break;
		fwrite(pBuf,dwBytesRead,1,fp_f);   //�����ص����������뵽�ļ���
		dwCount += dwBytesRead;
		//�������صĴ�С����.fc�ļ����Ա�ϵ�����
		FILE *fp_d = fopen("update//updateflie.fc","wb");
		if(fp_d==NULL) return false;
		fwrite(&dwCount,1,sizeof(DWORD),fp_d);
		fclose(fp_d);
		if ( dwFileSize - dwCount < GET_BUFFER_MAX )
			dwRequest = dwFileSize - dwCount;

		//��ʾ����
		nTotalSize += dwBytesRead;
		CString strFileSize("");
		strFileSize.Format("�����ļ�...                                                       %d/%d",nTotalSize,dwFileSize);
		SetDisplayInfo(hwnd,strFileSize,nTotalSize,dwFileSize);
		//SetDisplayInfo(hwnd,nTotalSize,dwFileSize);
	}
	string strFullFileName = "update\\updateflie.fc";
	::DeleteFile(strFullFileName.c_str());
	fclose(fp_f);
	return true;
}

void CInitInfoDialog::OnOK()
{
	if(m_bIDOK == false)
		return;
	// TODO: �ڴ����::GetMouse
	if(m_bReadError==true)
		Release();
	CDialog::OnOK();
}

//BOOL CInitInfoDialog::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	
//
//	return CDialog::OnEraseBkgnd(pDC);
//}

void CInitInfoDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()

	CRect rect;
	GetClientRect(&rect);

	////����������
	CFont myFont;
	myFont.CreatePointFont(90,"����");

	dc.SelectObject(&myFont);
	dc.SetTextColor(RGB(0,0,0));

	//����CHECT˵���ľ��δ���
	CRect myRect;
	myRect.left=5;
	myRect.top=310;
	myRect.right=500;
	myRect.bottom=325;

	//����͸���ķ�ʽ����Ļ��д��
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strDisplayInfo,&myRect,DT_LEFT);
}

void CInitInfoDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));

	CDialog::OnLButtonDown(nFlags, point);
}
