#pragma once

//////////////////////////////////////////////////////////////////////////
//ClientResource.h
//Fun:�ͻ�����Դ�࣬����ͻ�����Դ
//Create Time:2004.11.7
//Author:Joe
//
//
//Modified by Joe,2006.10,23.Ϊ��Դ����˶�����Ĺ��ܡ�
//////////////////////////////////////////////////////////////////////////

#define LOADRESOURCE_START		WM_USER+2000				//װ����Դ��ʼ
#define LOADRESOURCE_END		WM_USER+2001				//װ����Դ����
#define MAKEPACKAGE_START		WM_USER+2002				//�����ʼ
#define MAKEPACKAGE_ING			WM_USER+2003				//�������
#define MAKEPACKAGE_END			WM_USER+2004				//�������
#define REMAKEPACKAGE_START		WM_USER+2005				//�������ʼ
#define REMAKEPACKAGE_END		WM_USER+2006				//���������
#define RELOADRESOURCE_START	WM_USER+2007				//����ͳ��װ����Դ��ʼ
#define RELOADRESOURCE_END		WM_USER+2008				//װ��ͳ����Դ����

#include "FilesInfo.h"

class CPackage;
class CRFile;

struct tagPackageInfo
{
	DWORD		dwID;				//���ݰ�ID����
	string		strFileName;		//����Ӧ���ļ���
	DWORD		dwIndexNum;		
	DWORD		dwEmptyIndexNum;
	CPackage	*pPackage;
};

typedef map<DWORD,tagPackageInfo>	Packages;
typedef map<DWORD,tagPackageInfo>::iterator itPackage;

//�첽���ļ��Ļص�����
typedef void (*AsynReadCallBack)(CRFile* prfFile,void* pData);

enum eResourceType
{
	NO_RES=0,
	ORIGIN_RES,
	GAME_RES,
};

class CClientResource
{
public:
	CClientResource(void);
	CClientResource(eResourceType eType,string& strWorkFolder,Packages& TemptPackages,
					string& strFileInfoName,HWND hwWnd=0);

	//������ʹ��
	CClientResource(eResourceType eType,string& strWorkFolder,string& strFileInfoName,HWND hwWnd=0);
	~CClientResource(void);

private:
	static CClientResource* instance;

public:
	static void InitialInstance();
	static CClientResource* GetInstance();
	static void ReleaseInstance();
private:	
	eResourceType	m_eType;
	//CPackage*		m_pPackage;				//�ͻ��˰�
	Packages		m_pPackages;			//�����Դ��
	CFilesInfo*		m_pFilesInfo;			//�ͻ����ۺ���Դ


	string			m_strCurFolder;			//��ǰ������Դ�ļ���
	string			m_strPackageName;		//����
	string			m_strFileInfoName;		//��Դ��Ϣ�б�

	bool			m_bOver;				//�Ƿ񸲸�
	bool			m_bAlwaysCheck;			//�Ƿ�ʼ�մ˲���

//#ifdef _CLENTTOTEST_
//	static map<string, DWORD> s_mapNotFoundFiles;			//û���ҵ����ļ�<·��, ID>
//#endif

public:
	
	string& GetVersion()			{return m_pFilesInfo->GetVersion();}
	bool	GetIsOver()				{return m_bOver;}
	void	SetIsOver(bool b)		{m_bOver=b;	}

	bool	IsFileExist(const char* strFileName);
	bool	IsFileBodyExist(DWORD dwPackage,const char* strFileName);

	bool	GetAlwaysCheck()		{return m_bAlwaysCheck;	}
	void	SetAlwaysCheck(bool b)	{m_bAlwaysCheck=b;	}

	//CPackage* GetPackage()			{return m_pPackage;	}
	//void	Setpackage(CPackage* pPackage)	{m_pPackage=pPackage;}
	CPackage* GetPackage(DWORD dwPackID);
	CPackage* GetPackageForUpdate(DWORD dwPackID);
	void	PackagesWriteIndexEx();

	CFilesInfo*	GetFilesInfo()	{return m_pFilesInfo;}

	string&  GetCurFolder()	{return m_strCurFolder;}
	void	SetCurFolder(string& strName){m_strCurFolder=strName;}

	string& GetPackageName(){return m_strPackageName;}
	void SetPackageName(string& strName){m_strPackageName=strName;}

	string& GetFilesInfoName(){return m_strFileInfoName;}
	DWORD   GetFileContentCRC(string& strFileName);

	void	ResetPackInfosForAutoUpdate(PackTypeInfos& TemptPackages);

	bool	Load();
	bool	LoadEx();								//�ṩ������Ϸ��Դʹ��
	bool	LoadEx(PackTypeInfos& PacksInfo);		//�ṩ������ʹ��
	bool	LoadExForAutoUpdate();					//�ṩ���Զ����´���Ϸ��Դʹ��

	bool	Create();
	bool	Clear();
	bool	CreatePackages();
	//bool	LoadORCreatePackages();

	bool	Save();	
	bool	UpdateSave();
	bool	SaveVersionToFile();

	bool	LoadPackage(bool bUpdate=true);
	void	OpenPackages(bool bUpdate=true);
	void	PackagesCloseHandle();
	HANDLE	LoadClientResource();
	HANDLE	ReLoadClientResourece(string& strSeachFold,bool bWithChild = true); //����ͳ����Դ����
	HANDLE	MakePakcage(CClientResource* pClientResource);						//�����µ���Ϣ�б�������
	HANDLE	ReMakePackage();													//��������
	
	bool	OnAddFolder(string& strPos,string& strFolder);
	bool	OnAddFile(string& strPos,string& strFileName);
	bool	OnDelFileFolder(string& strPos,string& strFolderFileName);
	bool	OnExtrFileFolder(string& strPos,string& strFolderFileName);
	bool	OnExtrFileFolderFromPatch(string& strPos,string& strFolderFileName, CPackage* package);
	bool	ExtrFileFolder(tagFileInfo* ptagFileInfo,string strFolder,
							CFilesInfo* pFilesInfo=NULL);		//��ȡһ���ļ��л��ļ�

	bool	ExtrFileFolderFromPatch(tagFileInfo* ptagFileInfo,string strFolder, CPackage* package,
		CFilesInfo* pFilesInfo=NULL);		//��ȡһ���ļ��л��ļ�

	bool	ExtrAutoUpdateFileFolder(tagFileInfo* ptagFileInfo,
									string strFolder,CFilesInfo* pFilesInfo);	//��ȡһ���ļ��л��ļ��������Զ�����Ŀ¼

	void	AddFolder(string& strFolder,DWORD dwPackage,DWORD dwCompress);
	void	AddFile(string& strSouFolder,string& strFileName,int nSize,
					DWORD dwCrc32,DWORD dwPackage,DWORD dwCompress);
	bool	DelFileFolder(tagFileInfo* ptagFileInfo,CFilesInfo* pFileInfo);		//ɾ��һ���ļ��л��ļ�

	FILE*	OpenFile(const char* strFileName);
	bool	CloseFile(FILE* pf);

	void FindFileList(string& strFile, string& strExt, list<string>& listFileName);

	void	Initial(eResourceType eType,string& strWorkFolder,string& strFileInfoName,HWND hwWnd=0);
	void	Release();

	bool	AddEmptyFile(	const char* strFileName,int nSize,DWORD dwCrc32,
							DWORD dwPackage,DWORD dwCompress, int dwOrginSize);
	bool	AddFileData(const char* strFileName,BYTE* pBuf,int nSize);
	bool	AddFile(string& strFileName,BYTE* pBuf,long lSize);							//�ӻ�����һ���ļ�	
	bool	AdjustFile(const char* strFileName,DWORD dwTemptPackage,DWORD dwTemptCompress);
	bool	ClearEmptyFilesInfo(tagFileInfo* pTagFileInfo,CFilesInfo* pFilesInfo);			//����յ��ļ���Ϣ

	static unsigned __stdcall LoadClientResourceProc(void* pArguments);
	static unsigned __stdcall ReLoadClientResoureceProc(void* pArguments);
	static unsigned __stdcall MakePakcageProc(void* pArguments);
	static unsigned __stdcall ReMakePakcageProc(void* pArguments);

private:
	
	bool	MakePackage(tagFileInfo* pTagFileInfo,CFilesInfo* pFilesInfo,
						CClientResource* pClientResource);	
	bool	ReMakePackage(tagFileInfo* pTagFileInfo,CPackage* pNewPackage);				//��������

	void	SearchChildFolder(CHAR* strFolder,tagFileInfo* pParenttagFileInfo);			//�������ļ���
	void	SearchOneFile(tagFileInfo* pParenttagInfo,CHAR* strPath,
							string& strFileName,DWORD dwSize);							//�������ļ�

	void	ReSearchChildFolder(CHAR* strFolder,
								tagFileInfo* pParenttagFileInfo,bool bWithChild = true);//�������������ļ���
	void	ReSearchOneFile(tagFileInfo* pParenttagInfo,CHAR* strPath,
		string& strFileName,DWORD dwSize);												//�������ļ�

	bool	AddFile(string& strPos,string& strFileName);								//���һ���ļ�

	bool	AddFolder(string strFolderName,string strWorkPos);							//���һ���ļ���

	bool	DelFileFolder(tagFileInfo* ptagFileInfo);									//ɾ��һ���ļ��л��ļ�

	void	CreateDirectory(tagFileInfo* pTagFileInfo,CFilesInfo* pFilesInfo);
	void	DeleteDirectory(tagFileInfo* pTagFileInfo,CFilesInfo* pFilesInfo);

private:
	HWND	m_hWnd;																		//��ʾ�����Ϣ�Ĵ���
	HANDLE  ahThread;																	//һ��ִ���߳�
public:

	void OnLoadResourceStart();															//װ����Դ��ʼ
	void OnLoadResourceEnd();															//װ����Դ����
	void OnReLoadResourceStart();														//����װ����Դ��ʼ
	void OnReLoadResourceEnd();															//����װ����Դ����

	void OnMakePackageStart();															//�����ʼ
	void OnMakePackageing(tagFileInfo* pTagFileInfo,CFilesInfo* pFilesInfo);			//�����
	void OnMakePackageEnd();															//�������
	
	void OnReMakePackageStart();														//�������ʼ
	void OnReMakePackageEnd();															//���������


//�첽��ȡ�ļ�����
private:
	struct tagFileAsynOper
	{
		//�ļ���
		char strFileName[MAX_PATH];
		//�ص�����
		AsynReadCallBack pCallBack;
		//�첽����
		void* pData;
		//�ļ����
		CRFile* prFile;
	};

	typedef deque<tagFileAsynOper*>	AsynOpFiles;
	typedef deque<tagFileAsynOper*>::iterator itAsynOpFile;

	//������첽����
	AsynOpFiles m_RequestedReadFiles;
	//�����������
	HANDLE	m_hRequestReadFile;
	CRITICAL_SECTION m_CSRequestReadFile;

	//��ɵ��첽����
	AsynOpFiles m_CompletedReadFiles;
	CRITICAL_SECTION m_CSCompletedReadFile;

	//һ��ִ���߳�
	HANDLE  m_hAsynReadThread;
	//ͬ�����ļ�ʱ���ͬ���ź�
	HANDLE  m_hSyncRead;

	//ͬ����ȡ�ļ�����Ϣ
	tagFileAsynOper* m_pSyncReadFile;

	void PushRequestReadFile(tagFileAsynOper* pAsynReadFile);
	tagFileAsynOper* PopRequestReadFile();

	void PushCompletedReadFile(tagFileAsynOper* pAsynReadFile);
	tagFileAsynOper* PopCompletedReadFile();

	CRFile* OpenRFile(const char* strFileName);
public:
	void StartAsynReadFileThread();
	void ExitAsynReadFileThread();
	//ͬ�����ļ�
	CRFile* SyncReadFile(const char* strFileName);
	//�첽���ļ�
	BOOL	AsynReadFile(const char* strFileName,AsynReadCallBack pCallBack,void* pData);
	//�첽��ȡ�ļ����̺߳���
	static unsigned __stdcall AsynReadFileProc(void* pArguments);
	//��ɶ�ȡ���ݵ���������
	void Run();
};


//����Դ�ļ���ȫ�ֺ���
//ͬ�����ļ�����,����rfClose�ر�
CRFile* rfOpen(const char* strFileName);
//�첽��һ���ļ�
BOOL rfAsynOpen(const char* strFileName,AsynReadCallBack pCallBack,void* pData);
//�ر�CRFile�ļ����
void rfClose(CRFile* rfile);
