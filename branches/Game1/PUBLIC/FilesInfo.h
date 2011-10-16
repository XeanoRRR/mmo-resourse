#ifndef	FILESINFO_H
#define FILESINFO_H

#pragma once
#include <map>
#include <string>
#include <list>

using namespace std;

#define TREELIST_ADD				WM_USER+1000				//����ͼ�����
#define TREELIST_UPDATE				WM_USER+1001				//����ͼ��ˢ����
#define TREELIST_DELETE				WM_USER+1002				//����ͼ��ɾ����
#define TREELIST_DELALL				WM_USER+1003				//����ͼ��ɾ��������
#define TREELIST_INITIAL			WM_USER+1004				//��ʼ��
#define TREELIST_UPDATEWITHCHILD	WM_USER+1005				//����ͼ��ˢ����,������������

/////////////////////////////////////////////////////////////////////////////////
//FilesInfo.h
//Fun:����ͳ�ƿͻ�����Դ��Ϣ
//Create Time:2004.10.29
//Author:Joe
//
//
//Modified by Joe,2006.10,23.Ϊ��Դ����˶�����Ĺ��ܡ�
/////////////////////////////////////////////////////////////////////////////////
class CClientResource;

#define ENABLEBIT(NUM,BIT)	(NUM&BIT)
#define NOPACK		0x1
#define NOCOMPRESS	0x1


typedef struct tagFileInfo
{
	 string		strFileName;					//�ļ����·����
	 DWORD		dwSize;							//��Ӳ���ϵĴ�С��δѹ������ԭʼ��С��ѹ���˾���ѹ�����С
	 DWORD		dwOrginSize;					//�ļ�ԭʼ��С��δѹ��ǰsize
	 DWORD		dwValidSize;					//��ǰ��Ч���ļ���С����Ҫ���ڶϵ�������¼�Ѿ����ص��ļ���С
	 DWORD		dwCrc32;						//CRCУ����
	 DWORD		dwPackageType;					//�Ƿ���: 1, δ���; ����1, ��ʾ�������������ֵ���Ӧ�İ���
	 DWORD		dwCompressType;					//1 δѹ�� 2 zipѹ��

	 bool		bFolder;						//�Ƿ����ļ���
	 tagFileInfo* ParentFileInfo;				//���ļ�����Ϣ
	 map<string,tagFileInfo*> ChildFilesInfo;	//���ļ���Ϣ
	 
	 tagFileInfo()
	 {
		 strFileName="";dwSize=0;dwOrginSize=0;dwValidSize=0;
		 dwCrc32=0;dwPackageType=1;dwCompressType=1;
		 bFolder=true;ParentFileInfo=NULL;ChildFilesInfo.clear();
	 }

	 tagFileInfo(string& FileName,DWORD Size,DWORD OrginSize,
				DWORD ValidSize,DWORD Crc32,DWORD PackageType,
				DWORD CompressType,bool Folder,tagFileInfo* pParentFileInfo)
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
	DWORD		dwID;				//���ݰ�ID����
	string		strFileName;		//����Ӧ���ļ���
	DWORD		dwIndexNum;		
	DWORD		dwEmptyIndexNum;
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
	DWORD						m_dwFileNum;											//�ļ��ܸ���
	
public:
	DWORD	GetFileNum();			//�õ��ļ�����
	VOID	SetName(string& strName)		{m_FileName=strName;}
	string& GetName()						{return m_FileName;	}
	void	SetVersion(string strVersion)	{m_strVersion=strVersion;}
	string&	GetVersion()					{return m_strVersion;}

	PackTypeInfos& GetPackageTypeInfo()						{return m_PackageInfos;}
	void	SetPackageTypeInfo(PackTypeInfos& PackageInfos)	{m_PackageInfos = PackageInfos;}

	tagFileInfo* GetRootFile()				{return m_pRootFile;}

	DWORD	AddPackFileInfo(string& strPackFileName);
	string	GetPackFileName(DWORD dwID);
	DWORD	GetPackID(string strFileName);
		
	VOID	Clear();

	BOOL	Load(string& strName);														//���ļ���װ����ԴϢ�б�
	BOOL	LoadPackInfo(FILE* pf);
	BOOL	LoadFolderInfo(FILE* pf,tagFileInfo* pParentagFileInfo);					//װ��һ���ļ��е���Ϣ
	BOOL	Save(string& strName);														//������Դ��Ϣ�б�
	bool	Save(FILE* pf,tagFileInfo* pFileInfo);										//������Դ��Ϣ�б�
	BOOL	SavePackInfo(FILE* pf);

	bool	FindFileInfo(string& strFileName);											//Ѱ��һ���ļ��Ƿ����
	//����Ҫ���¸��ڵ���Ϣ
	tagFileInfo* AddFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
								DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
								DWORD dwPackageType,DWORD dwCompressType);

	tagFileInfo* AddFileInfo(tagFileInfo* pParentFileInfo,string& strName,
								DWORD dwCrc32,DWORD dwSize,DWORD dwOrginSize,
								DWORD dwValidSize,DWORD dwPackageType,DWORD dwCompressType);

	tagFileInfo* AddFolderInfo(string& strParentName,string& strName,
								DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
								DWORD dwPackageType,DWORD dwCompressType);

	//��Ҫ���¸��ڵ�Ĵ�С
	tagFileInfo* InsertFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
									DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
									DWORD dwPackageType,DWORD dwCompressType);

	tagFileInfo* InsertFileInfo(tagFileInfo* pParentFileInfo,string& strName,
								DWORD dwCrc32,DWORD dwSize,DWORD dwOrginSize,
								DWORD dwValidSize,DWORD dwPackageType,DWORD dwCompressType);

	//��Ҫ���¸����Ĵ�С��������Ϣ
	tagFileInfo* UpdateFileInfo(tagFileInfo* ptagInfo,DWORD dwCrc32,DWORD dwSize,
								DWORD dwOrginSize,DWORD dwValidSize,bool bFolder,
								DWORD dwPackageType,DWORD dwCompressType);				//����һ���ļ���Ϣ

	tagFileInfo* UpdateFileInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,
								DWORD dwOrginSize,DWORD dwValidSize,
								DWORD dwPackageType,DWORD dwCompressType);				//����һ���ļ���Ϣ

	tagFileInfo* UpdateFileInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,
								DWORD dwOrginSize,DWORD dwValidSize,bool bFolder,
								DWORD dwPackageType,DWORD dwCompressType);				//����һ���ļ���Ϣ


	//����һ���ļ��м�����������ļ�,������ļ��Ѿ����ð���Ϣ���ⲻ��
	void	UpdateFileInfoWithChild(string& strFileName,DWORD dwPackageType,
									DWORD dwCompressType,
									bool bForcePack,
									bool bForceCompress);								//���´����ѹ����Ϣ
	void	UpdateFileInfoWithChild(tagFileInfo* ptagInfo,DWORD dwPackageType,
									DWORD dwCompressType,
									bool bForcePack,
									bool bForceCompress);								//���´����ѹ����Ϣ

	void	UpdateFilePackInfoWithChild(string& strFileName,DWORD dwPackageType);
	void	UpdateFilePackInfoWithChild(tagFileInfo* ptagInfo,DWORD dwPackageType);		//���°���Ϣ

	//����һ���ļ���Ϣ������������ļ���Ϣ���������Լ�
	void	UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo);
	void	UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo,
										tagFileInfo* pOthertagInfo);					//���°���Ϣ

	VOID	DelFileInfo(string& strFileName);											//ɾ��һ���ļ���Ϣ
	VOID	DelFileInfo(tagFileInfo* pFileInfo);										//ɾ��һ���ļ���Ϣ
	
	DWORD	GetFileCrc32(string& strFileName);											//�õ��ļ���CRC��Ϣ��
	BOOL	GetFileInfo(const string& strFileName,DWORD& dwSize,DWORD& dwOrginSize,
						DWORD &dwValidSize,DWORD& dwCrc32,DWORD& dwPackageType,
						DWORD& dwCompressType,bool& bFolder);							//�õ�һ���ļ�����Ϣ

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
	void UpdateFolderInfo(tagFileInfo* pFileInfo,DWORD& dwPackage,DWORD& dwCompress);	//ͳ���ļ�����Ϣ

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

DWORD	GetFirstEnableBit(DWORD dwNum);

#endif