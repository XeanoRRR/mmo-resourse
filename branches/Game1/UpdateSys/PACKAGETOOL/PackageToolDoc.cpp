// PackageToolDoc.cpp :  CPackageToolDoc ���ʵ��
//

#include "stdafx.h"
#include "PackageTool.h"
#include "MainFrm.h"
#include "PackageToolDoc.h"
#include ".\packagetooldoc.h"
#include "VersionDialog.h"
#include "makeautoupdate.h"
#include "MakePatch.h"
#include "../../PUBLIC/Crc32Static.h"
#include "../../PUBLIC/readwrite.h"
#include "../../public/MD5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//void PutDebugString(const char *msg)
//{
//	AfxMessageBox(msg);
//}
// CPackageToolDoc
IMPLEMENT_DYNCREATE(CPackageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CPackageToolDoc, CDocument)
	ON_COMMAND(ID_32784, OnOpenLastProject)
	ON_COMMAND(ID_32785, OnSaveCurrProject)
	ON_COMMAND(ID_FILE_SAVERESINFOTOUPDATE, OnFileSaveresinfotoupdate)
//	ON_COMMAND(ID_MAKEAUTOUPDATEBYDEFAULT, OnMakeautoupdatebydefault)
//	ON_COMMAND(ID_MAKEAUTOUPDATEBYSELECT, OnMakeautoupdatebyselect)
	//ON_COMMAND(ID_FILE_OPENCLIENTINFO, OnFileOpenclientinfo)
	ON_COMMAND(ID_FILE_OPENORIGININFO, OnFileOpenorigininfo)
	ON_COMMAND(ID_MAKEDEFAULTPATCH, OnMakedefaultpatch)
	ON_COMMAND(ID_MAKESELECTPATCH, OnMakeselectpatch)
	ON_COMMAND(ID_MAKEAUTOUPDATEBYDEFAULTANDPATCH, OnMakeautoupdatebydefaultandpatch)
	ON_COMMAND(ID_MAKEAUTOUPDATEBYSELECTANDPATCH, OnMakeautoupdatebyselectandpatch)
	ON_COMMAND(ID_FILE_OPENRESINFOTABLE, OnFileOpenOtherFilesinfo)
	ON_COMMAND(ID_GENERATEMD5CODE, &CPackageToolDoc::OnGeneratemd5code)
	ON_COMMAND(ID_SETUPCLIENTMODE, &CPackageToolDoc::OnSetupclientmode)
	ON_COMMAND(ID_SETUPSERVERMODE, &CPackageToolDoc::OnSetupservermode)
	ON_COMMAND(ID_MAKESERVERPATCH, &CPackageToolDoc::OnMakeserverpatch)
	ON_COMMAND(ID_MAKESELECTSERVERPATCH, &CPackageToolDoc::OnMakeselectserverpatch)
END_MESSAGE_MAP()


// CPackageToolDoc ����/����

CPackageToolDoc::CPackageToolDoc()
:m_strCurFolder("")
,m_strExWorkPath("")
,m_pOriginResource(NULL)
,m_pClientResource(NULL)
,m_pUpdateInfo(NULL)
,m_pPatchPackage(NULL)
{
	// TODO: �ڴ����һ���Թ������

	m_strPackageName = "Game.dp";
	m_strFileInfoName = "PackRes.cfg";
	m_strUpdateFileName = "LastUpdate.cfg";
	m_strDefaultProName = "Default.ptc";

	m_strGameVersion="00.00.0000";
	m_pUpdateInfo=NULL;

	m_nDisplayOriInfo=0;
	m_pOpenOtherFilesInfo=NULL;
	m_strOtherFileInfoName="";
	m_lCurPatchMode = 1;
}

CPackageToolDoc::~CPackageToolDoc()
{
	if(m_pOriginResource)		delete m_pOriginResource;
	if(m_pUpdateInfo)			delete m_pUpdateInfo;
	if(m_pClientResource)		delete m_pClientResource;
	if(m_pOpenOtherFilesInfo)	delete m_pOpenOtherFilesInfo;
}

BOOL CPackageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	CHAR* strPath = new CHAR[MAX_PATH];
	if(::GetCurrentDirectory(MAX_PATH,strPath))
	{
		m_strExWorkPath=string(strPath);
	}

	m_strDefaultProName = m_strExWorkPath+"\\"+m_strDefaultProName;
	m_strUpdateFileName = m_strExWorkPath+"\\"+m_strUpdateFileName;

	FILE* pf = fopen(m_strDefaultProName.c_str(),"rb");
	if(pf)
	{
		fscanf(pf,"%s ",strPath);
		m_strPackageName=strPath;
		fscanf(pf,"%s ",strPath);
		m_strFileInfoName=strPath;
		fscanf(pf,"%s\n",strPath);
		m_strCurFolder=strPath;
		fclose(pf);
	}

	delete []strPath;
	//װ�ذ汾��
	LoadVersion();

	//װ�ذ���������Ϣ
	LoadPackageInfos();

	//װ��ѹ����Ϣ
	LoadCompressInfos();
	//���ϴ���Ŀ
	//OpenDefaultProject();
	return TRUE;
}


void	CPackageToolDoc::LoadVersion()
{
	//�����汾��
	string strTempt = m_strExWorkPath+"\\Game.vs";
	FILE* pf = fopen(strTempt.c_str(),"rb");
	if(pf)
	{
		fscanf(pf,"%s",m_strGameVersion.c_str());
		fclose(pf);
	}
}

//����汾��
void	CPackageToolDoc::SaveVersion()
{

	//���浱ǰ�汾��
	string strTempt = m_strExWorkPath+"\\Game.vs";
	FILE* pf = fopen(strTempt.c_str(),"wb");
	if(pf)
	{
		fprintf(pf,"%s\r\n",m_strGameVersion.c_str());
		fclose(pf);
	}
}


string CPackageToolDoc::GetCurFolder()
{
	if(m_nDisplayOriInfo==1)
		return m_strCurFolder;
	else if(m_nDisplayOriInfo==2)
		return m_strExWorkPath+"\\"+"Client";
	else if(m_nDisplayOriInfo==3)
		return m_strOtherFileInfoName;
	else
		return "";
}


void	CPackageToolDoc::LoadCurFileInfoUpdateInf(string strUpdateFileName)
{
	if(strUpdateFileName=="")
		strUpdateFileName= m_strUpdateFileName;
	FILE* pf = fopen(strUpdateFileName.c_str(),"rb");
	if(pf==NULL)
		return;
	else
		 fclose(pf);
	if(m_pUpdateInfo==NULL)
	{
		m_pUpdateInfo=new CFilesInfo;
	}
	//װ���ļ���Ϣ
	m_pUpdateInfo->Clear();
	m_pUpdateInfo->Load(strUpdateFileName);
}

//װ�ذ���Ϣ
void	CPackageToolDoc::LoadPackageInfos()
{
	string filename = m_strExWorkPath+"\\PackageConfig.ini";
	ifstream stream;
	stream.open(filename.c_str());
	if (!stream.is_open())
	{
		return;
	}

	set<DWORD>	CheckIDData;
	typedef set<DWORD>::iterator itData;
	for(int i=0;i< 32;i++)
	{
		DWORD dwNum = 1<<i;
		CheckIDData.insert(1<<i);
	}

	set<string> CheckNameData;CheckNameData.clear();
	typedef set<string>::iterator itCheckName;
	
	bool bFlag = true;
	m_PackageInfos.clear();
	m_PackageConfigInfos.clear();
	while (ReadTo(stream, "#"))
	{
		tagPackageInfo	PackConfInfo;
		tagPackFileInfo Info;
		stream >> PackConfInfo.dwID
			>> PackConfInfo.strFileName
			>> PackConfInfo.dwIndexNum
			>> PackConfInfo.dwEmptyIndexNum;

		itData it = CheckIDData.find(PackConfInfo.dwID);
		if(it == CheckIDData.end())
		{
			AfxMessageBox("���õİ�ID�����Ϲ���(ID=2^n,n<32)��");
			bFlag = false;
			break;
		}
		itPackage itPack = m_PackageConfigInfos.find(PackConfInfo.dwID);
		if(itPack != m_PackageConfigInfos.end())
		{
			AfxMessageBox("�����ظ��İ�ID�����������ļ�(PackageConfig.ini)��");
			bFlag = false;
			break;
		}

		itCheckName itName = CheckNameData.find(PackConfInfo.strFileName);
		if(itName != CheckNameData.end())
		{
			AfxMessageBox("�����ظ��İ����֣����������ļ�(PackageConfig.ini)��");
			bFlag = false;
			break;
		}
		else
		{
			CheckNameData.insert(PackConfInfo.strFileName);
		}

		PackConfInfo.pPackage = NULL;
		Info.dwID= PackConfInfo.dwID;
		Info.strFileName = PackConfInfo.strFileName;
		Info.dwIndexNum = PackConfInfo.dwIndexNum;
		Info.dwEmptyIndexNum = PackConfInfo.dwEmptyIndexNum;
		m_PackageConfigInfos[PackConfInfo.dwID] = PackConfInfo;
		m_PackageInfos.push_back(Info);
	}

	if(bFlag == false)
	{
		m_PackageConfigInfos.clear();
		m_PackageInfos.clear();
	}
}

//װ��ѹ����Ϣ
void	CPackageToolDoc::LoadCompressInfos()
{
	string filename = m_strExWorkPath+"\\CompressConfig.ini";
	ifstream stream;
	stream.open(filename.c_str());
	if (!stream.is_open())
	{
		return;
	}
	m_CompressInfos.clear();
	while (ReadTo(stream, "#"))
	{
		tagPackFileInfo Info;
		stream >> Info.dwID
			>> Info.strFileName;
		m_CompressInfos.push_back(Info);
	}
}
//���浱ǰ��Ϣ��Ϊ������Ϣ����
void	CPackageToolDoc::SaveCurFileInfoUpdateInfo(CString& strFileName)	
{
	if(m_pOriginResource==NULL || strFileName =="")	return;

	string strTemptFile(strFileName.GetBuffer());	
	
	if(m_pOriginResource)
		m_pOriginResource->GetFilesInfo()->Save(strTemptFile);
	AfxMessageBox("�洢��ϣ�");
}

// CPackageToolDoc ���л�

void CPackageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CPackageToolDoc ���

#ifdef _DEBUG
void CPackageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPackageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPackageToolDoc ����

//����һ���ļ�ʱ����
void CPackageToolDoc::OnOpenfolder(CString strFolder)
{
	if(strFolder == "")	return;
	if(m_nDisplayOriInfo == 1)
	{
		::AfxMessageBox("�Ѵ�ԭʼ��Դ��Ϣ��");
		return;
	}
	m_nDisplayOriInfo=1;

	m_strCurFolder = strFolder;

	if(m_pOriginResource)
	{
		delete m_pOriginResource;
	}

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if(pView)
	{
		Packages TemptPackages;
		m_pOriginResource = new CClientResource(ORIGIN_RES,m_strCurFolder,TemptPackages,m_strFileInfoName,
			pView->m_hWnd);
		m_pOriginResource->LoadClientResource();
	}
}

//�ӵ�ǰ�ļ���Ϣ���и���һ���ļ���Ϣ
VOID	CPackageToolDoc::UpdateFileInfoToOpFilZesInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,bool bFolder, DWORD dwPackageType,DWORD dwCompressType)
{
	if(m_pOriginResource)
		m_pOriginResource->GetFilesInfo()->UpdateFileInfo(strFileName,dwCrc32,dwSize,dwSize,dwSize,bFolder,dwPackageType,dwCompressType);
}

//�����ļ���Ϣ���������ļ�
VOID	CPackageToolDoc::UpdateFileInfoToOpFilesWithChild(string& strFileName,DWORD dwPackageType,DWORD dwCompressType,
														  bool bForcePack,bool bForceCompress)
{
	if(m_pOriginResource)
		m_pOriginResource->GetFilesInfo()->UpdateFileInfoWithChild(strFileName,dwPackageType,dwCompressType,
																	bForcePack,bForceCompress);
}

//�ӵ�ǰ�ļ���Ϣ���еõ�һ���ļ�����Ϣ
BOOL	CPackageToolDoc::GetFileInfoFromOpFilesInfo(string& strFileName,DWORD& dwSize,DWORD& dwCrc32,DWORD& dwPackageType,DWORD& dwCompressType,bool& bFolder)
{
	if(m_pOriginResource)
	{
		DWORD dwOrginSize, dwValidSize;
		return m_pOriginResource->GetFilesInfo()->GetFileInfo(strFileName,dwSize,dwOrginSize,dwValidSize,dwCrc32,dwPackageType,dwCompressType,bFolder);
	}
	return false;
}

//���ݵ�ǰ�������ļ���Ϣ���д��
void	CPackageToolDoc::MakePakcage()
{
	if(m_pOriginResource==NULL)	return;


	PackTypeInfos TemptPacksInfo;
	CVersionDialog VerDialog(TemptPacksInfo,false);
	VerDialog.m_strOldVersion=m_pOriginResource->GetFilesInfo()->GetVersion().c_str();
	VerDialog.m_strNewVersion=VerDialog.m_strOldVersion;
	//���°汾��Ϣ
	if(VerDialog.DoModal()==IDOK)
	{
		m_strGameVersion=VerDialog.m_strNewVersion;
		//����仯�İ汾��
		SaveVersion();

		//������Ϣ�İ汾
		m_pOriginResource->GetFilesInfo()->SetVersion(m_strGameVersion);

		//����һ���ͻ���Ŀ¼
		string strTempt = m_strExWorkPath+"\\"+"Client";
		::CreateDirectory(strTempt.c_str(),NULL);
		//�����Ű���Ŀ¼
		string strTemptPack = strTempt+"\\"+"Data";
		::CreateDirectory(strTemptPack.c_str(),NULL);
		if(m_pClientResource==NULL)
		{
			m_pClientResource = new CClientResource(GAME_RES,strTempt,m_PackageConfigInfos,
				m_strFileInfoName);
			//װ�ؿͻ�����Դ�ļ�
			m_pClientResource->Create();
		}

		m_pClientResource->GetFilesInfo()->SetVersion(m_strGameVersion);
		m_pClientResource->GetFilesInfo()->SetPackageTypeInfo(m_PackageInfos);
		m_pOriginResource->MakePakcage(m_pClientResource);
	}
}

void	CPackageToolDoc::MakePakcageEnd()
{
	m_pOriginResource->GetFilesInfo()->Save(m_strUpdateFileName);

	//�����Զ������ļ���Ϣ
	string strTempt = m_strExWorkPath+"\\"+"AutoUpdate-";
	strTempt += m_pOriginResource->GetVersion();

	//����һ��Ŀ¼
	::CreateDirectory(strTempt.c_str(),NULL);

	string strFileInfo = strTempt +"\\PackRes.cfg";
	m_pOriginResource->GetFilesInfo()->Save(strFileInfo);
	DWORD dwCrc32;
	CCrc32Static::FileCrc32Filemap(strFileInfo.c_str(),dwCrc32);
	//����汾��
	string strVersion = strTempt +"\\AutoUpdateGame.vs";
	FILE* pfAuto = fopen(strVersion.c_str(),"wb");
	if(pfAuto)
	{
		fprintf(pfAuto,"%s\n",m_strGameVersion.c_str());
		fprintf(pfAuto,"%u",dwCrc32);
		fclose(pfAuto);
	}
	//����汾���ļ���Patch�ļ���
	string strAutoVersion = m_strExWorkPath +"\\AutoUpdateGame.vs";
	FILE* pf = fopen(strAutoVersion.c_str(),"wb");
	if(pf)
	{
		fprintf(pf,"%s\n",strVersion.c_str());
		fprintf(pf,"%u",dwCrc32);
		fclose(pf);
	}
}


//��������� 
void	CPackageToolDoc::ReMakePakcage()
{
	if(m_pClientResource)
	{
		m_pClientResource->ReMakePackage();
	}
}



void	CPackageToolDoc::OnAddFolder(string& strPos,string& strFolder)
{
	/*if(m_nDisplayOriInfo)
	{
		if(m_pOriginResource)
		{
			if(m_pOriginResource->OnAddFolder(strPos,strFolder))
			{
				POSITION pos = GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = GetNextView(pos);
					if(pView) pView->SendMessage(OPEN_FOLDER);
				}
			}
		}
	}
	else
	{
		AfxMessageBox("�ͻ�����Դ��ֹ������");
	}*/
}
void	CPackageToolDoc::OnAddFile(string& strPos,string& strFileName)
{
	/*if(m_nDisplayOriInfo)
	{
		if(m_pOriginResource)
		{
			if(m_pOriginResource->OnAddFile(strPos,strFileName))
			{
				POSITION pos = GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = GetNextView(pos);
					if(pView) pView->SendMessage(OPEN_FOLDER);
				}
			}
		}
	}
	else
	{
		AfxMessageBox("�ͻ�����Դ��ֹ������");
	}*/
}
void	CPackageToolDoc::OnDelFileFolder(string& strPos,string& strFolderFileName)
{
	/*if(m_nDisplayOriInfo)
	{
		if(m_pOriginResource)
		{
			if(m_pOriginResource->OnDelFileFolder(strPos,strFolderFileName))
			{
				POSITION pos = GetFirstViewPosition();
				while (pos != NULL)
				{
					CView* pView = GetNextView(pos);
					if(pView) pView->SendMessage(OPEN_FOLDER);
				}
			}
		}
	}
	else
	{
		AfxMessageBox("�ͻ�����Դ��ֹ������");
	}*/
}
void	CPackageToolDoc::OnExtrFileFolder(string& strPos, string& strFolderFileName)
{
	if(m_nDisplayOriInfo == 1)
	{
		if(m_pOriginResource)
			m_pOriginResource->OnExtrFileFolder(strPos,strFolderFileName);
	}
	else if(m_nDisplayOriInfo == 2)
	{
		if(m_pClientResource)
			m_pClientResource->OnExtrFileFolder(strPos,strFolderFileName);
	}
	else if(m_nDisplayOriInfo == 4)
	{
		if(m_pClientResource)
			m_pClientResource->OnExtrFileFolderFromPatch(strPos,strFolderFileName, m_pPatchPackage);
	}
}
void CPackageToolDoc::OnOpenLastProject()
{
	// TODO: �ڴ���������������
	OpenDefaultProject();
}

void CPackageToolDoc::OnSaveCurrProject()
{
	// TODO: �ڴ���������������
	SaveProject(string(""));
}

void	CPackageToolDoc::OpenDefaultProject()
{

	if(m_nDisplayOriInfo == 1)
	{
		AfxMessageBox("�Ѵ�ԭʼ��Դ��Ϣ��");
		return;
	}
	m_nDisplayOriInfo=1;

	if(m_pOriginResource)
	{
		delete m_pOriginResource;
	}

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if(pView)
	{
		Packages TemptPackages;
		m_pOriginResource = new CClientResource(ORIGIN_RES,m_strCurFolder,TemptPackages,m_strFileInfoName,pView->m_hWnd);
		CString strTempt2("");
		if(m_pOriginResource->Load()==false)
		{
			CString str;
			str.Format("û����Ϣ�ļ�,�����´򿪸��ļ��н�����Ϣͳ��.");
			AfxMessageBox((LPCTSTR)str);
		}

	}
}

void	CPackageToolDoc::SaveProject(string& strName)
{
	string strTemptName = strName;
	if(strName=="")
	{
		strTemptName=m_strDefaultProName;
	}

	FILE* pf = fopen(strTemptName.c_str(),"wb");
	if(pf)
	{
		fprintf(pf,"%s ",m_strPackageName.c_str());
		fprintf(pf,"%s ",m_strFileInfoName.c_str());
		fprintf(pf,"%s\n",m_strCurFolder.c_str());

		fclose(pf);
	}
	else
	{
		::AfxMessageBox("�������Ŀ���ó���");
		return;
	}

	//������Ϣ�б�
	if(m_pOriginResource)
		m_pOriginResource->Save();
}

void CPackageToolDoc::OnCloseDocument()
{
	// TODO: �ڴ����ר�ô����/����û���

	//����ȱʡ��Ŀ
	SaveProject(string(""));

	CDocument::OnCloseDocument();
}

void	 CPackageToolDoc::ReLoadFolder(string& strFolder,bool bWithChild)
{
	if(m_pOriginResource)
		m_pOriginResource->ReLoadClientResourece(strFolder,bWithChild);

}

void	CPackageToolDoc::UpdateClientResourToView()
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		if(pView) pView->SendMessage(OPEN_FOLDER);
	}
}


const string&	CPackageToolDoc::GetPackFormatSring(DWORD dwPackID)
{
	static string strFormat = "";
	strFormat.clear();
	if( ( dwPackID & 0x1 ) == 1)
		strFormat += "��,";

	itTypeInfo it = m_PackageInfos.begin();
	for(;it != m_PackageInfos.end();it++)
	{
		if( ( dwPackID & (*it).dwID ) != 0)
		{
			strFormat += (*it).strFileName;
			strFormat += ",";
		}
	}
	strFormat = strFormat.substr(0,strFormat.length() -1);
	return strFormat;
}


const string&	CPackageToolDoc::GetCompFormatSring(DWORD dwCompID)
{
	static string strFormat = "";
	strFormat.clear();
	if( ( dwCompID & 0x1 ) == 1)
		strFormat += "��,";

	itTypeInfo it = m_CompressInfos.begin();
	for(;it != m_CompressInfos.end();it++)
	{
		if( ( dwCompID & (*it).dwID ) != 0)
		{
			strFormat += (*it).strFileName;
			strFormat += ",";
		}
	}
	strFormat = strFormat.substr(0,strFormat.length() -1);
	return strFormat;
}


void	CPackageToolDoc::SetStatStatuOp(string& strOp)
{
	m_StaDisplay.strOp= strOp;

	theApp.GetMainWnd()->SendMessage(STATUS_UPDATE,(WPARAM)&m_StaDisplay);
}

void CPackageToolDoc::SetStatStatuFileName(string& strFileName)
{
	m_StaDisplay.strFileName= strFileName;
	
	theApp.GetMainWnd()->SendMessage(STATUS_UPDATE,(WPARAM)&m_StaDisplay);
}

bool	CPackageToolDoc::SetGameVersion(string& strGameVersion)
{
	string strVersion(strGameVersion);

	strupr((char*)strVersion.c_str());
	bool bFlag = true;
	if(strVersion.length()==10)
	{
		for(int i=0;i < 10; i++)
		{
			if(i==2 || i == 5)
			{
				if(strVersion[i] != '.')
				{
					bFlag=false;
					break;
				}
			}
			else
			{
				if( !(strVersion[i] >= '0' && strVersion[i] <= '9' ) && 
					!(strVersion[i] >= 'A' && strVersion[i] <= 'Z' ) )
				{
					bFlag=false;
					break;
				}
			}
		}
	}
	else
	{
		bFlag=false;
	}

	if(bFlag==false)
	{
		string strTempt("�˰汾�Ŵ���:\n");
		strTempt += strVersion;
		::AfxMessageBox(strTempt.c_str());
		return false;
	}
	else
	{
		m_strGameVersion=strGameVersion;
		return true;
	}
	return false;
}

void CPackageToolDoc::OnFileSaveresinfotoupdate()
{
	// TODO: �ڴ���������������
	//�������ԭʼֵ

	CFileDialog	OpenDlg(FALSE,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();
		SaveCurFileInfoUpdateInfo(strFile);
	}
	
}

//make autoupdate package
void CPackageToolDoc::OnMakeautoupdatebydefault()
{
	// TODO: �ڴ���������������1
	if(m_pOriginResource==NULL)	return;

	LoadCurFileInfoUpdateInf();

	if(m_pUpdateInfo==NULL)
	{
		AfxMessageBox("No Original Update Info of Files!");
		return;
	}

	PackTypeInfos& TemptPacksInfo = GetPackageInfos();
	CVersionDialog VerDialog(TemptPacksInfo,true);
	VerDialog.m_strOldVersion=m_strGameVersion.c_str();
	VerDialog.m_strNewVersion=m_strGameVersion.c_str();
	//���°汾��Ϣ
	if(VerDialog.DoModal()==IDOK)
	{
		m_strGameVersion=VerDialog.m_strNewVersion;
		//����仯�İ汾��
		SaveVersion();

		m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
		CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
										m_pUpdateInfo,m_pOriginResource);
		MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), true, false);
	}
}

void CPackageToolDoc::OnMakeautoupdatebyselect()
{
	// TODO: �ڴ���������������

	if(m_pOriginResource==NULL)	return;

	CFileDialog	OpenDlg(true,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();

		LoadCurFileInfoUpdateInf(strFile.GetBuffer());

		if(m_pUpdateInfo==NULL)
		{
			AfxMessageBox("No Original Update Info of Files!");
			return;
		}

		PackTypeInfos& TemptPacksInfo = GetPackageInfos();
		CVersionDialog VerDialog(TemptPacksInfo,true);

		VerDialog.m_strOldVersion=m_strGameVersion.c_str();
		VerDialog.m_strNewVersion=m_strGameVersion.c_str();
		//���°汾��Ϣ
		if(VerDialog.DoModal()==IDOK)
		{
			m_strGameVersion=VerDialog.m_strNewVersion;
			//����仯�İ汾��
			SaveVersion();

			
			m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
			CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
											m_pUpdateInfo,m_pOriginResource);

			MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), false, true);
		}
	}
}

//void CPackageToolDoc::OnFileOpenclientinfo()
//{
//	// TODO: �ڴ���������������
//	if(m_nDisplayOriInfo)
//	{
//		m_nDisplayOriInfo=false;
//		if(m_pOriginResource)
//		{
//			m_pOriginResource->GetFilesInfo()->SetDisplayWnd(NULL);
//			m_pOriginResource->Save();
//		}
//
//		if(m_pClientResource)
//		{
//			delete m_pClientResource;m_pClientResource=NULL;
//		}
//		
//		string strTempt = m_strExWorkPath+"\\"+"Client";
//
//		Packages TemptPackages;
//		m_pClientResource = new CClientResource(strTempt,TemptPackages,m_strFileInfoName);
//		//װ�ؿͻ�����Դ�ļ�
//		m_pClientResource->LoadEx();
//
//		POSITION pos = GetFirstViewPosition();
//		CView* pView = GetNextView(pos);
//		if(pView)
//		{
//			m_pClientResource->GetFilesInfo()->SetDisplayWnd(pView->m_hWnd);
//			m_pClientResource->GetFilesInfo()->UpdateAllInfoToView(NULL);
//		}
//	}
//}
void	 CPackageToolDoc::OnOpenPatchResouce(CString strFolder) // ��patch��Դ�ļ�
{
	m_nDisplayOriInfo = 4;
	if(m_pOriginResource)
	{
		m_pOriginResource->GetFilesInfo()->SetDisplayWnd(NULL);
		m_pOriginResource->Save();
	}

	if(m_pClientResource)
	{
		delete m_pClientResource;m_pClientResource=NULL;
	}

	string strTempt = strFolder;

	Packages TemptPackages;
	m_pClientResource = new CClientResource(GAME_RES,strTempt,TemptPackages,m_strFileInfoName);
	//װ�ؿͻ�����Դ�ļ�
	m_pClientResource->LoadEx();

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if(pView)
	{
		m_pClientResource->GetFilesInfo()->UpdateAllInfoToViewEx(pView->m_hWnd);
	}

	m_pPatchPackage = new CPackage(NULL, strTempt);
	m_pPatchPackage->Open( "\\Patch", false);
}
void	 CPackageToolDoc::OnOpenGameResouce(CString strFolder)
{	
	m_nDisplayOriInfo = 2;
	if(m_pOriginResource)
	{
		m_pOriginResource->GetFilesInfo()->SetDisplayWnd(NULL);
		m_pOriginResource->Save();
	}

	if(m_pClientResource)
	{
		delete m_pClientResource;m_pClientResource=NULL;
	}
	
	string strTempt = strFolder;

	Packages TemptPackages;
	m_pClientResource = new CClientResource(GAME_RES,strTempt,TemptPackages,m_strFileInfoName);
	//װ�ؿͻ�����Դ�ļ�
	m_pClientResource->LoadEx();

	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	if(pView)
	{
		//m_pClientResource->GetFilesInfo()->SetDisplayWnd(pView->m_hWnd);
		//m_pClientResource->GetFilesInfo()->UpdateAllInfoToView(NULL);
		m_pClientResource->GetFilesInfo()->UpdateAllInfoToViewEx(pView->m_hWnd);
	}
}

void CPackageToolDoc::OnFileOpenorigininfo()
{
	// TODO: �ڴ���������������
	/*if(m_nDisplayOriInfo==false)
	{
		m_nDisplayOriInfo=true;
		if(m_pClientResource)
			m_pClientResource->GetFilesInfo()->SetDisplayWnd(NULL);

		if(m_pOriginResource==NULL)
		{
			OpenDefaultProject();
		}
		else
		{
			POSITION pos = GetFirstViewPosition();
			CView* pView = GetNextView(pos);
			if(pView)
			{
				m_pOriginResource->GetFilesInfo()->SetDisplayWnd(pView->m_hWnd);
				m_pOriginResource->GetFilesInfo()->UpdateAllInfoToView(NULL);
			}
		}
	}*/
}

void CPackageToolDoc::OnMakedefaultpatch()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������1
	if(m_pOriginResource==NULL)	return;

	LoadCurFileInfoUpdateInf();

	if(m_pUpdateInfo==NULL)
	{
		AfxMessageBox("No Original Update Info of Files!");
		return;
	}

	PackTypeInfos& TemptPacksInfo = GetPackageInfos();
	CVersionDialog VerDialog(TemptPacksInfo,true);
	VerDialog.m_strOldVersion=m_strGameVersion.c_str();
	VerDialog.m_strNewVersion=m_strGameVersion.c_str();
	//���°汾��Ϣ
	if(VerDialog.DoModal()==IDOK)
	{
		m_strGameVersion=VerDialog.m_strNewVersion;
		//����仯�İ汾��
		SaveVersion();

		m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
		CMakePatch PatchUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
			m_pUpdateInfo,m_pOriginResource);
		PatchUpdate.MakePatch(false);
	}
}

void CPackageToolDoc::OnMakeselectpatch()
{
	// TODO: �ڴ���������������
	if(m_pOriginResource==NULL)	return;

	CFileDialog	OpenDlg(true,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();

		LoadCurFileInfoUpdateInf(strFile.GetBuffer());

		if(m_pUpdateInfo==NULL)
		{
			AfxMessageBox("No Original Update Info of Files!");
			return;
		}

		PackTypeInfos& TemptPacksInfo = GetPackageInfos();
		CVersionDialog VerDialog(TemptPacksInfo,true);
		VerDialog.m_strOldVersion=m_strGameVersion.c_str();
		VerDialog.m_strNewVersion=m_strGameVersion.c_str();
		//���°汾��Ϣ
		if(VerDialog.DoModal()==IDOK)
		{
			m_strGameVersion=VerDialog.m_strNewVersion;
			//����仯�İ汾��
			SaveVersion();

			m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
			CMakePatch PatchUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
				m_pUpdateInfo,m_pOriginResource);
			PatchUpdate.MakePatch(true);
		}
	}
}

void CPackageToolDoc::OnMakeautoupdatebydefaultandpatch()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 1;
	if(m_pOriginResource==NULL)	return;

	LoadCurFileInfoUpdateInf();

	if(m_pUpdateInfo==NULL)
	{
		AfxMessageBox("No Original Update Info of Files!");
		return;
	}

	PackTypeInfos& TemptPacksInfo = GetPackageInfos();
	CVersionDialog VerDialog(TemptPacksInfo,true);
	VerDialog.m_strOldVersion=m_strGameVersion.c_str();
	VerDialog.m_strNewVersion=m_strGameVersion.c_str();
	//���°汾��Ϣ
	if(VerDialog.DoModal()==IDOK)
	{
		m_strGameVersion=VerDialog.m_strNewVersion;
		//����仯�İ汾��
		SaveVersion();

		m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
		CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
										m_pUpdateInfo,m_pOriginResource);
		MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), false, true);
	}
}

void CPackageToolDoc::OnMakeautoupdatebyselectandpatch()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 1;
	if(m_pOriginResource==NULL)	return;

	CFileDialog	OpenDlg(true,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();

		LoadCurFileInfoUpdateInf(strFile.GetBuffer());

		if(m_pUpdateInfo==NULL)
		{
			AfxMessageBox("No Original Update Info of Files!");
			return;
		}

		PackTypeInfos& TemptPacksInfo = GetPackageInfos();
		CVersionDialog VerDialog(TemptPacksInfo,true);
		VerDialog.m_strOldVersion=m_pUpdateInfo->GetVersion().c_str();
		VerDialog.m_strNewVersion=m_strGameVersion.c_str();
		//���°汾��Ϣ
		if(VerDialog.DoModal()==IDOK)
		{
			m_strGameVersion=VerDialog.m_strNewVersion;
			//����仯�İ汾��
			SaveVersion();

			m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
			CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
											m_pUpdateInfo,m_pOriginResource);
			MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), true, true);
		}
	}
}

void CPackageToolDoc::OnFileOpenOtherFilesinfo()
{
	// TODO: �ڴ���������������

	CFileDialog	OpenDlg(true,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();

		if(m_nDisplayOriInfo == 1)
		{
			m_pOriginResource->GetFilesInfo()->SetDisplayWnd(NULL);
			m_pOriginResource->Save();
		}
		else if(m_nDisplayOriInfo == 2)
		{
			m_pClientResource->GetFilesInfo()->SetDisplayWnd(NULL);
		}
		else if(m_nDisplayOriInfo == 3)
		{
			m_pOpenOtherFilesInfo->SetDisplayWnd(NULL);
		}
		if(m_pOpenOtherFilesInfo )
		{
			delete m_pOpenOtherFilesInfo;
			m_pOpenOtherFilesInfo = NULL;
		}
		m_nDisplayOriInfo = 3;

		POSITION pos = GetFirstViewPosition();
		CView* pView = GetNextView(pos);
		if(pView)
		{
			m_pOpenOtherFilesInfo=new CFilesInfo;
			m_strOtherFileInfoName = string(strFile.GetBuffer());
			m_pOpenOtherFilesInfo->Load(m_strOtherFileInfoName);
			m_pOpenOtherFilesInfo->UpdateAllInfoToViewEx(pView->m_hWnd);
		}
	}
}

void CPackageToolDoc::OnGeneratemd5code()
{
	if(!m_pOriginResource)
	{
		MessageBox(NULL, "���ȴ�һ����Դ��Ŀ��", "����", MB_OK);
		return;
	}

	// TODO: �ڴ���������������
	m_strCurFolder;
	m_strFileInfoName;

	// �������ļ���crc32ֵ
	string exeName = m_strCurFolder + "\\setup\\serverlist.ini";
	DWORD dwCrc32 = 0;
	CCrc32Static::FileCrc32Filemap(exeName.c_str(), dwCrc32);

	if(dwCrc32 == 0)
	{
		MessageBox(NULL, "δ�ҵ�����ĿsetupĿ¼�µ�ServerList.ini�ļ���", "����", MB_OK);
		return;
	}

	//�����Զ������ļ���Ϣ
	string strTempt = m_strExWorkPath+"\\"+"AutoUpdate-";
	strTempt += m_pOriginResource->GetVersion();
	string vsName = strTempt + "\\AutoUpdateGame.vs";
	
	string strCrc32;
	string strVer;

	ifstream myfile;
	myfile.open(vsName.c_str());
	if (!myfile.is_open())
	{
		char szOutSStr[1024];
		sprintf(szOutSStr, "δ�ҵ��ò�����%sĿ¼�µ�AutoUpdateGame.vs�ļ����汾��%s��",
			strTempt.c_str(),m_pOriginResource->GetVersion().c_str());
		MessageBox(NULL, szOutSStr, "����", MB_OK);
		return;
	}
	myfile >> strVer;
	myfile >> strCrc32;
	myfile.close();

	ofstream myfile1;
	myfile1.open(vsName.c_str());
	if (!myfile1.is_open())
	{
		char szOutSStr[1024];
		sprintf(szOutSStr, "δ�ҵ��ò�����%sĿ¼�µ�AutoUpdateGame.vs�ļ����汾��%s��",
			strTempt.c_str(),m_pOriginResource->GetVersion().c_str());
		MessageBox(NULL, szOutSStr, "����", MB_OK);
		return;
	}
	myfile1 << strVer << "\r\n";
	myfile1 << dwCrc32 << "\r\n";
	myfile1.close();

	char szOutSStr[1024];
	sprintf(szOutSStr, "�ɹ��޸ĸò�����%sĿ¼�µ�AutoUpdateGame.vs�ļ����汾��%s��",
		strTempt.c_str(),m_pOriginResource->GetVersion().c_str());
	MessageBox(NULL, szOutSStr, "��ʾ", MB_OK);
}


void CPackageToolDoc::OnSetupclientmode()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 1;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame)
	{
		CMenu* pMenu = pMainFrame->GetMenu();
		if(pMenu)
		{
			pMenu->CheckMenuItem(ID_SETUPSERVERMODE, MF_UNCHECKED);
			pMenu->CheckMenuItem(ID_SETUPCLIENTMODE, MF_CHECKED);
		}
	}
}

void CPackageToolDoc::OnSetupservermode()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 0;
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame)
	{
		CMenu* pMenu = pMainFrame->GetMenu();
		if(pMenu)
		{
			pMenu->CheckMenuItem(ID_SETUPSERVERMODE, MF_CHECKED);
			pMenu->CheckMenuItem(ID_SETUPCLIENTMODE, MF_UNCHECKED);
		}
	}
}

void CPackageToolDoc::OnMakeserverpatch()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 0;

	if(m_pOriginResource==NULL)	return;

	LoadCurFileInfoUpdateInf();

	if(m_pUpdateInfo==NULL)
	{
		AfxMessageBox("No Original Update Info of Files!");
		return;
	}

	PackTypeInfos& TemptPacksInfo = GetPackageInfos();
	CVersionDialog VerDialog(TemptPacksInfo,true);
	VerDialog.m_strOldVersion=m_strGameVersion.c_str();
	VerDialog.m_strNewVersion=m_strGameVersion.c_str();
	//���°汾��Ϣ
	if(VerDialog.DoModal()==IDOK)
	{
		m_strGameVersion=VerDialog.m_strNewVersion;
		//����仯�İ汾��
		SaveVersion();

		m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
		CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
			m_pUpdateInfo,m_pOriginResource);
		MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), false, true);
	}
}

void CPackageToolDoc::OnMakeselectserverpatch()
{
	// TODO: �ڴ���������������
	m_lCurPatchMode = 0;

	CFileDialog	OpenDlg(true,"cfg","*.cfg",OFN_EXTENSIONDIFFERENT,"cfg");
	if (OpenDlg.DoModal() == IDOK && OpenDlg.GetFileExt() == "cfg")
	{
		CString strFile = OpenDlg.GetPathName();

		LoadCurFileInfoUpdateInf(strFile.GetBuffer());

		if(m_pUpdateInfo==NULL)
		{
			AfxMessageBox("No Original Update Info of Files!");
			return;
		}

		PackTypeInfos& TemptPacksInfo = GetPackageInfos();
		CVersionDialog VerDialog(TemptPacksInfo,true);
		VerDialog.m_strOldVersion=m_pUpdateInfo->GetVersion().c_str();
		VerDialog.m_strNewVersion=m_strGameVersion.c_str();
		//���°汾��Ϣ
		if(VerDialog.DoModal()==IDOK)
		{
			m_strGameVersion=VerDialog.m_strNewVersion;
			//����仯�İ汾��
			SaveVersion();

			m_pOriginResource->GetFilesInfo()->SetPackageTypeInfo(GetPackageInfos());
			CMakeAutoUpdate MakeAutoUpdate(m_strGameVersion,m_strExWorkPath,VerDialog.m_nPackType,
				m_pUpdateInfo,m_pOriginResource);
			MakeAutoUpdate.MakeAutoUpdatePackage(GetCurPatchMode(), true, true);
		}
	}
}
