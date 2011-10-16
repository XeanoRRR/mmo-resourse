/////////////////////////////////////////////////////////////////////////////////
//FilesInfo.h
//Fun:����ͳ�ƿͻ�����Դ��Ϣ
//Create Time:2004.10.29
//Author:Joe
//
//
//Modified by Joe,2006.10,23.Ϊ��Դ����˶�����Ĺ��ܡ�
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../DefType.h"

#define TREELIST_ADD				WM_USER+1000				//����ͼ�����
#define TREELIST_UPDATE				WM_USER+1001				//����ͼ��ˢ����
#define TREELIST_DELETE				WM_USER+1002				//����ͼ��ɾ����
#define TREELIST_DELALL				WM_USER+1003				//����ͼ��ɾ��������
#define TREELIST_INITIAL			WM_USER+1004				//��ʼ��
#define TREELIST_UPDATEWITHCHILD	WM_USER+1005				//����ͼ��ˢ����,������������

class CClientResource;

#define ENABLEBIT(NUM,BIT)	(NUM&BIT)
#define NOPACK		0x1
#define NOCOMPRESS	0x1

typedef struct tagFileInfo
{
	 string		strFileName;					//�ļ����·����
	 ulong		dwSize;							//��Ӳ���ϵĴ�С��δѹ������ԭʼ��С��ѹ���˾���ѹ�����С
	 ulong		dwOrginSize;					//�ļ�ԭʼ��С��δѹ��ǰsize
	 ulong		dwValidSize;					//��ǰ��Ч���ļ���С����Ҫ���ڶϵ�������¼�Ѿ����ص��ļ���С
	 ulong		dwCrc32;						//CRCУ����
	 ulong		dwPackageType;					//�Ƿ���: 1, δ���; ����1, ��ʾ�������������ֵ���Ӧ�İ���
	 ulong		dwCompressType;					//1 δѹ�� 2 zipѹ��

	 bool		bFolder;						//�Ƿ����ļ���
	 tagFileInfo* ParentFileInfo;				//���ļ�����Ϣ
	 map<string,tagFileInfo*> ChildFilesInfo;	//���ļ���Ϣ
	 
	 tagFileInfo()
	 {
		 strFileName="";dwSize=0;dwOrginSize=0;dwValidSize=0;
		 dwCrc32=0;dwPackageType=1;dwCompressType=1;
		 bFolder=true;ParentFileInfo=NULL;ChildFilesInfo.clear();
	 }

	 tagFileInfo(string& FileName,ulong Size,ulong OrginSize,
				ulong ValidSize,ulong Crc32,ulong PackageType,
				ulong CompressType,bool Folder,tagFileInfo* pParentFileInfo)
	 {
		 strFileName=FileName;dwSize=Size;dwOrginSize=OrginSize;
		 dwValidSize=ValidSize;dwCrc32=Crc32;
		 dwPackageType=PackageType;dwCompressType=CompressType;
		 bFolder=Folder;ParentFileInfo=pParentFileInfo;ChildFilesInfo.clear();
	 }

}*PTFILEINFO;

typedef map<string,tagFileInfo*>::iterator itFileInfo;

//���ļ���Ϣ
struct tagPackFileInfo
{
	ulong		dwID;				//���ݰ�ID����
	string		strFileName;		//����Ӧ���ļ���
	ulong		dwIndexNum;		
	ulong		dwEmptyIndexNum;
};

typedef list<tagPackFileInfo>	PackTypeInfos;
typedef list<tagPackFileInfo>	CompTypeInfos;


typedef list<tagPackFileInfo>::iterator itTypeInfo;

class CFilesInfo
{
public:
	CFilesInfo(HWND pWnd=0);
	virtual ~CFilesInfo(void);

private:
	string						m_FileName;												//��Դ��Ϣ�洢�ļ���
	tagFileInfo					*m_pRootFile;											//���ļ�

	PackTypeInfos				m_PackageInfos;											//���ļ�����Ϣ
	string						m_strVersion;											//�ļ��汾��
	ulong						m_dwFileNum;											//�ļ��ܸ���
	
public:
	ulong	GetFileNum();			//�õ��ļ�����
	void	SetName(string& strName)		{m_FileName=strName;}
	string& GetName()						{return m_FileName;	}
	void	SetVersion(string strVersion)	{m_strVersion=strVersion;}
	string&	GetVersion()					{return m_strVersion;}

	PackTypeInfos& GetPackageTypeInfo()						{return m_PackageInfos;}
	void	SetPackageTypeInfo(PackTypeInfos& PackageInfos)	{m_PackageInfos = PackageInfos;}

	tagFileInfo* GetRootFile()				{return m_pRootFile;}

	ulong	AddPackFileInfo(string& strPackFileName);
	string	GetPackFileName(ulong dwID);
	ulong	GetPackID(string strFileName);
		
	void	Clear();

	bool	Load(string& strName);														//���ļ���װ����ԴϢ�б�
	bool	LoadPackInfo(FILE* pf);
	bool	LoadFolderInfo(FILE* pf,tagFileInfo* pParentagFileInfo);					//װ��һ���ļ��е���Ϣ
	bool	Save(string& strName);														//������Դ��Ϣ�б�
	bool	Save(FILE* pf,tagFileInfo* pFileInfo);										//������Դ��Ϣ�б�
	bool	SavePackInfo(FILE* pf);

	bool	FindFileInfo(string& strFileName);											//Ѱ��һ���ļ��Ƿ����
	//����Ҫ���¸��ڵ���Ϣ
	tagFileInfo* AddFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
								ulong dwSize,ulong dwOrginSize,ulong dwValidSize,
								ulong dwPackageType,ulong dwCompressType);

	tagFileInfo* AddFileInfo(tagFileInfo* pParentFileInfo,string& strName,
								ulong dwCrc32,ulong dwSize,ulong dwOrginSize,
								ulong dwValidSize,ulong dwPackageType,ulong dwCompressType);

	tagFileInfo* AddFolderInfo(string& strParentName,string& strName,
								ulong dwSize,ulong dwOrginSize,ulong dwValidSize,
								ulong dwPackageType,ulong dwCompressType);

	//��Ҫ���¸��ڵ�Ĵ�С
	tagFileInfo* InsertFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
									ulong dwSize,ulong dwOrginSize,ulong dwValidSize,
									ulong dwPackageType,ulong dwCompressType);

	tagFileInfo* InsertFileInfo(tagFileInfo* pParentFileInfo,string& strName,
								ulong dwCrc32,ulong dwSize,ulong dwOrginSize,
								ulong dwValidSize,ulong dwPackageType,ulong dwCompressType);

	//��Ҫ���¸����Ĵ�С��������Ϣ
	tagFileInfo* UpdateFileInfo(tagFileInfo* ptagInfo,ulong dwCrc32,ulong dwSize,
								ulong dwOrginSize,ulong dwValidSize,bool bFolder,
								ulong dwPackageType,ulong dwCompressType);				//����һ���ļ���Ϣ

	tagFileInfo* UpdateFileInfo(string& strFileName,ulong dwCrc32,ulong dwSize,
								ulong dwOrginSize,ulong dwValidSize,
								ulong dwPackageType,ulong dwCompressType);				//����һ���ļ���Ϣ

	tagFileInfo* UpdateFileInfo(string& strFileName,ulong dwCrc32,ulong dwSize,
								ulong dwOrginSize,ulong dwValidSize,bool bFolder,
								ulong dwPackageType,ulong dwCompressType);				//����һ���ļ���Ϣ


	//����һ���ļ��м�����������ļ�,������ļ��Ѿ����ð���Ϣ���ⲻ��
	void	UpdateFileInfoWithChild(string& strFileName,ulong dwPackageType,
									ulong dwCompressType,
									bool bForcePack,
									bool bForceCompress);								//���´����ѹ����Ϣ
	void	UpdateFileInfoWithChild(tagFileInfo* ptagInfo,ulong dwPackageType,
									ulong dwCompressType,
									bool bForcePack,
									bool bForceCompress);								//���´����ѹ����Ϣ

	void	UpdateFilePackInfoWithChild(string& strFileName,ulong dwPackageType);
	void	UpdateFilePackInfoWithChild(tagFileInfo* ptagInfo,ulong dwPackageType);		//���°���Ϣ

	//����һ���ļ���Ϣ������������ļ���Ϣ���������Լ�
	void	UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo);
	void	UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo,
										tagFileInfo* pOthertagInfo);					//���°���Ϣ

	void	DelFileInfo(string& strFileName);											//ɾ��һ���ļ���Ϣ
	void	DelFileInfo(tagFileInfo* pFileInfo);										//ɾ��һ���ļ���Ϣ
	
	ulong	GetFileCrc32(string& strFileName);											//�õ��ļ���CRC��Ϣ��
	bool	GetFileInfo(const string& strFileName,ulong& dwSize,ulong& dwOrginSize,
						ulong &dwValidSize,ulong& dwCrc32,ulong& dwPackageType,
						ulong& dwCompressType,bool& bFolder);							//�õ�һ���ļ�����Ϣ

	bool	GetDifferenceByPackage(CFilesInfo& filesInfo,CFilesInfo& temptFilesInfo);	//�����Ƿ����õ������ļ���Ϣ�б�Ĳ���

	bool	GetDifferenceByPackage(tagFileInfo* pFileInfo,CFilesInfo& filesInfo,
									CFilesInfo& temptFilesInfo,bool bPackage);

	bool	GetDifferenceByCrc32(CFilesInfo& filesInfo,CFilesInfo& temptFilesInfo);		//�õ������ļ���Ϣ�б�Ĳ���

	bool	GetDifferenceByCrc32(tagFileInfo* pFileInfo,CFilesInfo& filesInfo,
									CFilesInfo& temptFilesInfo);

	bool	CopyData(CFilesInfo* pFileInfo);
	bool	CopyData(tagFileInfo* ptagFileInfo,CFilesInfo* pFileInfo);


	bool	IsFolder(string& strFileName);												//�Ƿ����ļ���
	bool	HaveChild(tagFileInfo* pFieInfo);											//�Ƿ����ӽڵ�
	bool	ChildHaveUnPackFile(tagFileInfo* ptagFieInfo);								//�Ƿ���δ����ļ�
	tagFileInfo* GetFileInfoByText(string strFileName);									//�����ַ����õ�һ���ڵ�
	tagFileInfo* FindChildFileInfoByText(tagFileInfo* pParent,string& str);				//�ҵ�һ��ƥ���ַ��ӽڵ�
	string GetFileTextWithParent(tagFileInfo* pFileInfo);								//�õ�һ���ڵ�ĺ͸���������ַ���


	void AddFileSize(tagFileInfo* pFileInfo,int iSize);									//����ļ���С��
	void UpdateParentFolderInfo(tagFileInfo* ptagFileInfo);								//ͳ�Ƹ��ļ�����Ϣ
	void UpdateFolderInfo(tagFileInfo* pFileInfo,ulong& dwPackage,ulong& dwCompress);	//ͳ���ļ�����Ϣ

	void FindFileList(string& strFile, string& strExt, list<string>& listFileName);
	void FindFileList(tagFileInfo* ptagFileInfo, string& strExt,
						list<string>& listFileName);
private:
	HWND		m_pDisplayWnd;															//��ʾ���ݵĴ���
public:
	HWND SetDisplayWnd(HWND hWnd)
	{ HWND hOldWnd = m_pDisplayWnd;m_pDisplayWnd=hWnd;return hOldWnd;}
	HWND GetDisplayWnd(void) { return m_pDisplayWnd; }
	void OnAdd(tagFileInfo* ptagFileInfo);
	void OnUpdate(tagFileInfo* ptagFileInfo);
	void OnDelete(tagFileInfo* ptagFileInfo);
	void OnDeleteAll();
	void OnIntial();

	void UpdateAllInfoToView(HWND hWnd);
	void UpdateAllInfoToViewEx(HWND hWnd);
	void UpdateAllInfoToView(tagFileInfo* ptagFileInfo,HWND hWnd);
};

ulong	GetFirstEnableBit(ulong dwNum);
