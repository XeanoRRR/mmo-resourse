/////////////////////////////////////////////////////////////////////////////////
//FilesInfo.h
//Fun:����ͳ�ƿͻ�����Դ��Ϣ
//Create Time:2004.10.29
//Author:Joe
/////////////////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include ".\filesinfo.h"
#include "ClientResource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void PutDebugString(const char *msg);

CFilesInfo::CFilesInfo(HWND pWnd)
:m_pDisplayWnd(pWnd)
{
	m_FileName ="";
	m_PackageInfos.clear();
	m_pRootFile=new tagFileInfo;
	m_strVersion = "00.00.0000";

	m_dwFileNum = 0;

	//��ʼ��
	OnIntial();
}

CFilesInfo::~CFilesInfo(void)
{
	//ɾ�����нڵ�
	DelFileInfo(m_pRootFile);

	//ɾ��
	OnDeleteAll();
}

//���ļ���װ����Դ��Ϣ�б�
BOOL	CFilesInfo::Load(string& strName)
{
	if(strName.compare("") ==0)	return false;
	m_FileName = strName;

	FILE *pf = fopen(m_FileName.c_str(),"rb");
	if(pf == NULL)
	{
		char strText[MAX_PATH]="";
		_snprintf(strText, MAX_PATH, "װ���ļ� %s ����!",m_FileName.c_str());
		PutDebugString(strText);
		return false;
	}

	char strPath[MAX_PATH]="";
	DWORD bFolder;
	try
	{
		//�����ڵ�
		if(!feof(pf))
		{
			fscanf(pf,"%s\n",m_strVersion.c_str());

			//װ�ذ���Ϣ
			LoadPackInfo(pf);

			fscanf(pf,"%u ",&m_pRootFile->dwSize);
			fscanf(pf,"%u ",&m_pRootFile->dwOrginSize);
			fscanf(pf,"%u ",&m_pRootFile->dwValidSize);
			fscanf(pf,"%u ",&m_pRootFile->dwCrc32);
			fscanf(pf,"%d ",&m_pRootFile->dwPackageType);
			fscanf(pf,"%d ",&m_pRootFile->dwCompressType);	
			fscanf(pf,"%d\n",&bFolder);
			m_pRootFile->bFolder=true;
			OnUpdate(m_pRootFile);
			//װ����Դ
			LoadFolderInfo(pf,m_pRootFile);
		}
		fclose(pf);
	}
	catch (...)
	{
		Clear();
		PutDebugString("��ȡ�ļ���Ϣ����.");
		return false;
	}
	PutDebugString("CFilesInfo::Load() end!");
	return true;
}

BOOL	CFilesInfo::LoadPackInfo(FILE* pf)
{
	if(pf==NULL)	return FALSE;
	//��ȡ����
	char strName[100]="";
	DWORD dwSize = 0;
	fscanf(pf,"%u\n",&dwSize);
	for(DWORD i = 0;i<dwSize;i++)
	{
		tagPackFileInfo PackInfo;
		fscanf(pf,"%u ",&PackInfo.dwID);
		fscanf(pf,"%s ",strName);
		PackInfo.strFileName = strName;
		fscanf(pf,"%u ",&PackInfo.dwIndexNum);
		fscanf(pf,"%u\n",&PackInfo.dwEmptyIndexNum);
		m_PackageInfos.push_back(PackInfo);
	}

	return TRUE;
}

BOOL	CFilesInfo::LoadFolderInfo(FILE* pf,tagFileInfo* pParentagFileInfo)
{
	if(pf==NULL	|| pParentagFileInfo==NULL)	return false;

	char strName[MAX_PATH];
	memset(strName, 0, sizeof(strName));
	DWORD dwSize,dwOrginSize,dwValidSize,dwCrc32,dwPackageType,dwCompressType;
	DWORD bFolder;
	while(!feof(pf))
	{
		//fscanf(pf,"%s ",strName);
		long szPos = 0;
		char szFlag = 0;
		while(szFlag != ' ' && szFlag !='<')
			fread(&szFlag, 1, 1, pf); // ��ѭ�������ֿ�ʼ��'['
		while (szFlag != '>') // ��¼�ļ�����
		{
			fread(&szFlag, 1, 1, pf); // �����ո�' '
			if(szFlag != '>')
			{
				strName[szPos] = szFlag;
				szPos++;
			}
		}
		fread(&szFlag, 1, 1, pf); // �����ո�' '
		strName[szPos] = '\0';

		fscanf(pf,"%u ",&dwSize);
		fscanf(pf,"%u ",&dwOrginSize);
		fscanf(pf,"%u ",&dwValidSize);
		fscanf(pf,"%u ",&dwCrc32);
		fscanf(pf,"%d ",&dwPackageType);
		fscanf(pf,"%d ",&dwCompressType);	
		fscanf(pf,"%d\n",&bFolder);
		string strFileName(strName);

		//����
		if(bFolder==20)
			return true;

		//���ļ���
		if(bFolder != false)
		{
			if(strFileName == "*") // �Ǹ�Ŀ¼,������
			{
				LoadFolderInfo(pf,pParentagFileInfo);
			}
			else
			{
				tagFileInfo* pTemptParentagInfo = AddFolderInfo(pParentagFileInfo,strFileName,
				dwSize,dwOrginSize,dwValidSize,dwPackageType,dwCompressType);
				LoadFolderInfo(pf,pTemptParentagInfo);
			}
		}
		else
		{
			AddFileInfo(pParentagFileInfo,strFileName,dwCrc32,dwSize,dwOrginSize,dwValidSize,
				dwPackageType,dwCompressType);
		}
	}
	return true;
}
//������Դ��Ϣ�б�
BOOL	CFilesInfo::Save(string& strName)
{
	string strTemptName = strName;
	if(strTemptName.compare("")==0)
		strTemptName=m_FileName;
	if(strTemptName.compare("")==0)
		return false;

	FILE *pf = fopen(strTemptName.c_str(),"wb");
	if(pf)
	{
		//��汾��
		fprintf(pf,"%s\n",m_strVersion.c_str());

		//�������Ϣ
		SavePackInfo(pf);

		Save(pf,GetRootFile());
		fclose(pf);
	}
	else
	{
		char strText[MAX_PATH]="";
		_snprintf(strText, MAX_PATH, "�����ļ� %s ����!",strName.c_str());
		PutDebugString(strText);
		return false;
	}
	return true;
}

//�������Ϣ
BOOL	CFilesInfo::SavePackInfo(FILE* pf)
{
	if(pf==NULL)	return FALSE;
	//������ĸ���
	fprintf(pf,"%d\n",m_PackageInfos.size());
	itTypeInfo it = m_PackageInfos.begin();
	for(;it != m_PackageInfos.end();it++)
	{
		fprintf(pf,"%d ",(*it).dwID);
		fprintf(pf,"%s ",(*it).strFileName.c_str());
		fprintf(pf,"%d ",(*it).dwIndexNum);
		fprintf(pf,"%d\n",(*it).dwEmptyIndexNum);
	}
	return TRUE;
}

//������Դ��Ϣ�б�
bool	CFilesInfo::Save(FILE* pf,tagFileInfo* pFileInfo)
{
	if(pFileInfo==NULL || pf == NULL)	return false;

	//��汾��
	if(pFileInfo->ParentFileInfo == NULL) // ��Ŀ¼
		fprintf(pf,"<*> ");
	else
		fprintf(pf,"<%s> ",pFileInfo->strFileName.c_str());
	fprintf(pf,"%u ",pFileInfo->dwSize);
	fprintf(pf,"%u ",pFileInfo->dwOrginSize);
	fprintf(pf,"%u ",pFileInfo->dwValidSize);
	fprintf(pf,"%u ",pFileInfo->dwCrc32);
	fprintf(pf,"%d ",pFileInfo->dwPackageType);
	fprintf(pf,"%d ",pFileInfo->dwCompressType);
	fprintf(pf,"%d\n",pFileInfo->bFolder);

	if( pFileInfo->bFolder )
	{
		if(HaveChild(pFileInfo))
		{
			for(itFileInfo it = pFileInfo->ChildFilesInfo.begin();
				it != pFileInfo->ChildFilesInfo.end();it++)
			{
				Save(pf,(*it).second);
			}
		}

		//���һ���ļ��д洢��ϣ���һ�����ر�ʾ
		fprintf(pf,"<%s> ","Return");
		fprintf(pf,"%u ",0);
		fprintf(pf,"%u ",0);
		fprintf(pf,"%u ",0);
		fprintf(pf,"%u ",0);
		fprintf(pf,"%d ",0);
		fprintf(pf,"%d ",0);
		fprintf(pf,"%d\n",20);
	}
	return true;
}

//Ѱ��һ���ļ��Ƿ����
bool	CFilesInfo::FindFileInfo(string& strFileName)
{
	if(GetFileInfoByText(strFileName)==NULL)	return true;
	return false;
}

tagFileInfo* 
CFilesInfo::AddFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
						  DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
						  DWORD dwPackageType,DWORD dwCompressType)
{
	if(pParentFileInfo==NULL)	return NULL;

	char *newstring = _strlwr( _strdup( strName.c_str() ));
	strName = newstring;
	free(newstring);
	//�����µĽڵ�
	tagFileInfo* pTemptFileInfo =
		new tagFileInfo(strName,dwSize,dwOrginSize,dwValidSize,0,dwPackageType,dwCompressType,true,pParentFileInfo);
	pParentFileInfo->ChildFilesInfo[strName] = pTemptFileInfo;

	//�������
	OnAdd(pTemptFileInfo);
	return pTemptFileInfo;
}

tagFileInfo* 
CFilesInfo::AddFileInfo(tagFileInfo* pParentFileInfo,string& strName,DWORD dwCrc32,
						DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
						DWORD dwPackageType,DWORD dwCompressType)
{
	if(pParentFileInfo==NULL)	return NULL;
	char *newstring = _strlwr( _strdup( strName.c_str() ));
	strName = newstring;
	free(newstring);
	//�����µĽڵ�
	tagFileInfo* pTemptFileInfo =
		new tagFileInfo(strName,dwSize,dwOrginSize,dwValidSize,
						dwCrc32,dwPackageType,dwCompressType,false,pParentFileInfo);

	pParentFileInfo->ChildFilesInfo[strName] = pTemptFileInfo;
	//�������
	OnAdd(pTemptFileInfo);
	return pTemptFileInfo;
}

tagFileInfo* 
CFilesInfo::AddFolderInfo(string& strParentName,string& strName,
						  DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
						  DWORD dwPackageType,DWORD dwCompressType)
{
	tagFileInfo* pParentFileInfo = GetFileInfoByText(strParentName);
	if(pParentFileInfo==NULL)
		return NULL;
	return AddFolderInfo(pParentFileInfo,strName,dwSize,
						dwOrginSize,dwValidSize,dwPackageType,dwCompressType);
}

tagFileInfo* 
CFilesInfo::InsertFolderInfo(tagFileInfo* pParentFileInfo,string& strName,
							 DWORD dwSize,DWORD dwOrginSize,DWORD dwValidSize,
							 DWORD dwPackageType,DWORD dwCompressType)
{	
	return AddFolderInfo(pParentFileInfo,strName,dwSize,dwOrginSize,dwValidSize,
		dwPackageType,dwCompressType);
	
}
tagFileInfo* 
CFilesInfo::InsertFileInfo(tagFileInfo* pParentFileInfo,string& strName,
						   DWORD dwCrc32,DWORD dwSize,DWORD dwOrginSize,
						   DWORD dwValidSize,DWORD dwPackageType,DWORD dwCompressType)
{
	//�����µĽڵ�
	tagFileInfo* pTemptFileInfo =AddFileInfo(pParentFileInfo,strName,
											dwCrc32,dwSize,dwOrginSize,dwValidSize,
											dwPackageType,dwCompressType);
	
	if(pTemptFileInfo)
	{
		AddFileSize(pParentFileInfo,pTemptFileInfo->dwSize);
	}
	return pTemptFileInfo;
}
//����һ���ļ���Ϣ
tagFileInfo* 
CFilesInfo::UpdateFileInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,
							DWORD dwOrginSize,DWORD dwValidSize,
							DWORD dwPackageType,DWORD dwCompressType)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pParentFileInfo = pParentFileInfo = GetRootFile();

	string str = strFileName;
	tagFileInfo* pInFileInfo=NULL;
	int nPos = str.find("\\");

	string strTempt("");
	while(str.find("\\") ==0 && pParentFileInfo != NULL)
	{
		str = str.erase(0,str.find("\\")+1);
		nPos = str.find("\\");
		strTempt="";
		if(nPos < 0)
		{
			strTempt=str;
			str = "";
		}
		else
		{
			strTempt.insert(strTempt.begin(),str.begin(),str.begin()+str.find("\\"));
			str = str.erase(0,str.find("\\"));
		}
		pInFileInfo=pParentFileInfo;
		pParentFileInfo = FindChildFileInfoByText(pParentFileInfo,strTempt);
	}

	if(pParentFileInfo==NULL)
	{
		do
		{
			bool bFolder = false;
			nPos = str.find("\\");
			if(nPos < 0)
			{
				bFolder = false;
				pParentFileInfo = new tagFileInfo(	strTempt,dwSize,dwOrginSize,
													dwValidSize,dwCrc32,dwPackageType,
													dwCompressType,bFolder,pInFileInfo);
			}
			else
			{
				bFolder = true;
				pParentFileInfo = new tagFileInfo(strTempt,0,0,0,0,0,0,bFolder,pInFileInfo);
			}

			//�����µĽڵ�
			pInFileInfo->ChildFilesInfo[strTempt] = pParentFileInfo;
			pInFileInfo = pParentFileInfo;

			if(bFolder==false)
			{
				AddFileSize(pInFileInfo->ParentFileInfo,pParentFileInfo->dwSize);
				UpdateParentFolderInfo(pInFileInfo->ParentFileInfo);
			}

			//�������
			OnAdd(pParentFileInfo);


			str = str.erase(0,str.find("\\")+1);
			nPos = str.find("\\");
			strTempt="";
			if(nPos < 0)
			{
				strTempt=str;
				str = "";
			}
			else
			{
				strTempt.insert(strTempt.begin(),str.begin(),str.begin()+str.find("\\"));
				str = str.erase(0,str.find("\\"));
			}
		}while(strTempt.compare("") != 0);
	}
	else
	{
		pParentFileInfo->dwPackageType=dwPackageType;
		pParentFileInfo->dwCompressType=dwCompressType;
		
		if(pParentFileInfo->bFolder==false)
		{
			pParentFileInfo->dwCrc32=dwCrc32;
			int nDirSize = dwSize-pParentFileInfo->dwSize;
			pParentFileInfo->dwSize = dwSize;
			pParentFileInfo->dwOrginSize = dwOrginSize;
			pParentFileInfo->dwValidSize = dwValidSize;
			AddFileSize(pParentFileInfo->ParentFileInfo,nDirSize);
			UpdateParentFolderInfo(pParentFileInfo->ParentFileInfo);
		}

		//��������
		OnUpdate(pParentFileInfo);
	}
	return pInFileInfo;
}

//����һ���ļ���Ϣ
tagFileInfo* 
CFilesInfo::UpdateFileInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,
						   DWORD dwOrginSize,DWORD dwValidSize,bool bFolder,
						   DWORD dwPackageType,DWORD dwCompressType)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
	if(pFileInfo)
	{		
		pFileInfo->dwPackageType=dwPackageType;
		pFileInfo->dwCompressType=dwCompressType;		
		//��������
		if(pFileInfo->bFolder==false)
		{
			pFileInfo->dwCrc32=dwCrc32;
			int nDirSize = dwSize-pFileInfo->dwSize;
			pFileInfo->dwSize = dwSize;
			pFileInfo->dwOrginSize=dwOrginSize;
			pFileInfo->dwValidSize=dwValidSize;
			AddFileSize(pFileInfo->ParentFileInfo,nDirSize);
			UpdateParentFolderInfo(pFileInfo->ParentFileInfo);
		}

		OnUpdate(pFileInfo);
	}
	else
	{
		string str(""),strTempt("");
		str.insert(str.begin(),strFileName.begin(),
					strFileName.begin()+strFileName.find_last_of("\\"));
		strTempt.insert(strTempt.begin(),
						strFileName.begin()+strFileName.find_last_of("\\")+1,
						strFileName.end());

		pFileInfo = GetFileInfoByText(str);
		if(pFileInfo==NULL)	return NULL;
		//�����µĽڵ�
		tagFileInfo* pTemptFileInfo =
			new tagFileInfo(strTempt,dwSize,dwOrginSize,dwValidSize,dwCrc32,
							dwPackageType,dwCompressType,bFolder,pFileInfo);
		pFileInfo->ChildFilesInfo[strTempt] = pTemptFileInfo;
		pFileInfo=pTemptFileInfo;

		if(bFolder==false)
		{
			AddFileSize(pTemptFileInfo->ParentFileInfo,pTemptFileInfo->dwSize);
			UpdateParentFolderInfo(pTemptFileInfo->ParentFileInfo);
		}

		//�������
		OnAdd(pFileInfo);
	}
	return pFileInfo;

}

//��Ҫ���¸����Ĵ�С��������Ϣ
tagFileInfo* 
CFilesInfo::UpdateFileInfo(tagFileInfo* ptagInfo,DWORD dwCrc32,DWORD dwSize,
							DWORD dwOrginSize,DWORD dwValidSize,bool bFolder,
							DWORD dwPackageType,DWORD dwCompressType)
{
	if(ptagInfo)
	{
		ptagInfo->dwPackageType=dwPackageType;
		ptagInfo->dwCompressType=dwCompressType;		
		//��������
		if(ptagInfo->bFolder==false)
		{
			ptagInfo->dwCrc32=dwCrc32;
			int nDirSize = dwSize-ptagInfo->dwSize;
			ptagInfo->dwSize = dwSize;
			ptagInfo->dwOrginSize=dwOrginSize;
			ptagInfo->dwValidSize=dwValidSize;
			AddFileSize(ptagInfo->ParentFileInfo,nDirSize);

			UpdateParentFolderInfo(ptagInfo->ParentFileInfo);
		}
		OnUpdate(ptagInfo);
	}
	return ptagInfo;
}

//���´����Ϣ
void	CFilesInfo::UpdateFileInfoWithChild(string& strFileName,DWORD dwPackageType,
									DWORD dwCompressType,
									bool bForcePack,
									bool bForceCompress)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
	if(pFileInfo)
	{		
		//�������ļ�
		UpdateFileInfoWithChild(pFileInfo,dwPackageType,dwCompressType,bForcePack,bForceCompress);
		//if(pFileInfo->bFolder==false)
		//{
		//	pFileInfo->dwCrc32=dwCrc32;
		//	int nDirSize = dwSize-pFileInfo->dwSize;
		//	pFileInfo->dwSize = dwSize;
		//	pFileInfo->dwOrginSize=dwOrginSize;
		//	pFileInfo->dwValidSize=dwValidSize;
		//	AddFileSize(pFileInfo->ParentFileInfo,nDirSize);
		//	UpdateParentFolderInfo(pFileInfo->ParentFileInfo);
		//}

		UpdateParentFolderInfo(pFileInfo->ParentFileInfo);
	}
}

//���´����Ϣ
void	CFilesInfo::UpdateFileInfoWithChild(tagFileInfo* ptagInfo,DWORD dwPackageType,
									DWORD dwCompressType,
									bool bForcePack,
									bool bForceCompress)
{
	if(ptagInfo == NULL)	return;
	if(HaveChild(ptagInfo))
	{
		for(itFileInfo it = ptagInfo->ChildFilesInfo.begin();
			it != ptagInfo->ChildFilesInfo.end();it++)
		{
			UpdateFileInfoWithChild((*it).second,dwPackageType,dwCompressType,bForcePack,bForceCompress);
		}
	}
	if( ENABLEBIT(ptagInfo->dwPackageType,NOPACK) ||  bForcePack)
		ptagInfo->dwPackageType=dwPackageType;
	if( ENABLEBIT(ptagInfo->dwCompressType,NOCOMPRESS) ||  bForceCompress)
		ptagInfo->dwCompressType=dwCompressType;
	//���������Ϣ
	OnUpdate(ptagInfo);
	return;
}

void	CFilesInfo::UpdateFilePackInfoWithChild(string& strFileName,DWORD dwPackageType)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
	if(pFileInfo)
	{		
		//�������ļ�
		UpdateFilePackInfoWithChild(pFileInfo,dwPackageType);
		UpdateParentFolderInfo(pFileInfo->ParentFileInfo);
	}
}

void	CFilesInfo::UpdateFilePackInfoWithChild(tagFileInfo* ptagInfo,DWORD dwPackageType)
{
	if(ptagInfo == NULL)	return;
	if(HaveChild(ptagInfo))
	{
		for(itFileInfo it = ptagInfo->ChildFilesInfo.begin();
			it != ptagInfo->ChildFilesInfo.end();it++)
		{
			UpdateFilePackInfoWithChild((*it).second,dwPackageType);
		}
	}
	if( !ENABLEBIT(ptagInfo->dwPackageType,NOPACK) )
		ptagInfo->dwPackageType=dwPackageType;
	//���������Ϣ
	OnUpdate(ptagInfo);
	return;
}


//����һ���ļ���Ϣ������������ļ���Ϣ���������Լ�
void	CFilesInfo::UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo)
{
	if(pOtherFilesInfo == NULL)	return;
	UpdateFilePackInfoWithChild(pOtherFilesInfo,pOtherFilesInfo->GetRootFile());
}
//���°���Ϣ
void	CFilesInfo::UpdateFilePackInfoWithChild(CFilesInfo* pOtherFilesInfo,
									tagFileInfo* pOthertagInfo)
{
	if(pOtherFilesInfo->HaveChild(pOthertagInfo))
	{
		for(itFileInfo it = pOthertagInfo->ChildFilesInfo.begin();
			it != pOthertagInfo->ChildFilesInfo.end();it++)
		{
			UpdateFilePackInfoWithChild( pOtherFilesInfo ,(*it).second);
		}
	}
	if(pOthertagInfo->bFolder == false)
	{
		string strFileName = pOtherFilesInfo->GetFileTextWithParent(pOthertagInfo);
		tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
		if(pFileInfo)
		{
			pFileInfo->dwPackageType = pOthertagInfo->dwPackageType;
			OnUpdate(pFileInfo);
			UpdateParentFolderInfo(pFileInfo->ParentFileInfo);
		}
	}
}

 //ɾ��һ���ļ���Ϣ
VOID	CFilesInfo::DelFileInfo(string& strFileName)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* ptagInfo = GetFileInfoByText(strFileName);
	string strTemptName = ptagInfo->strFileName;
	tagFileInfo* pParentInfo = ptagInfo->ParentFileInfo;
	if(pParentInfo)
	{
		DelFileInfo(ptagInfo);
		pParentInfo->ChildFilesInfo.erase(strTemptName);
	}
}

//ɾ��һ���ļ���Ϣ
VOID	CFilesInfo::DelFileInfo(tagFileInfo* pFileInfo)
{
	if(pFileInfo==NULL)	return;
	if(HaveChild(pFileInfo))
	{
		for(itFileInfo it = pFileInfo->ChildFilesInfo.begin();
			it != pFileInfo->ChildFilesInfo.end();)
		{
			DelFileInfo((*it).second);
			pFileInfo->ChildFilesInfo.erase(it++);
		}

		//����ɾ��
		OnDelete(pFileInfo);
		delete pFileInfo;pFileInfo=NULL;
	}
	else
	{
		//��ȥ��ش�С
		int nSize = pFileInfo->dwSize;
		AddFileSize(pFileInfo->ParentFileInfo,-nSize);
		//����ɾ��
		OnDelete(pFileInfo);
		delete pFileInfo;pFileInfo=NULL;
	}
}

//����0����ʾ���ʧ��
DWORD	CFilesInfo::AddPackFileInfo(string& strPackFileName)
{
	//������ӿյİ���
	if( strPackFileName.length() == 0)	return 0;

	DWORD dwID = 0;
	//��������򷵻�ID�������ҳ�����ID���Ա���ӡ�
	itTypeInfo it = m_PackageInfos.begin();
	for(;it != m_PackageInfos.end();it++)
	{
		if( strPackFileName == (*it).strFileName )
			return (*it).dwID;
		else if( (*it).dwID > dwID )
			dwID = (*it).dwID;
	}
	if(dwID == 0xFFFFFFFF)
		return 0;

	dwID++;
	tagPackFileInfo Info = {dwID,strPackFileName};
	m_PackageInfos.push_back(Info);
	return dwID;
}

string	CFilesInfo::GetPackFileName(DWORD dwID)
{
	itTypeInfo it = m_PackageInfos.begin();
	for(;it != m_PackageInfos.end();it++)
	{
		if( (*it).dwID == dwID )
			return (*it).strFileName;
	}
	return "";
}


DWORD	CFilesInfo::GetPackID(string strFileName)
{
	itTypeInfo it = m_PackageInfos.begin();
	for(;it != m_PackageInfos.end();it++)
	{
		if( (*it).strFileName == strFileName )
			return (*it).dwID;
	}
	return 0;
}

//�õ��ļ�����
DWORD	CFilesInfo::GetFileNum()
{
	return m_dwFileNum;
}

VOID	CFilesInfo::Clear()
{
	for(itFileInfo it = m_pRootFile->ChildFilesInfo.begin();
		it != m_pRootFile->ChildFilesInfo.end();it++)
	{
		DelFileInfo((*it).second);
	}
	m_pRootFile->ChildFilesInfo.clear();
	m_PackageInfos.clear();
}

//�õ��ļ���CRC��Ϣ��
//����0 ��ʾû�д��ļ�
DWORD	CFilesInfo::GetFileCrc32(string& strFileName)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
	if(pFileInfo)
		return pFileInfo->dwCrc32;
	return 0;
}

//�Ƿ����ļ���
bool	CFilesInfo::IsFolder(string& strFileName)
{
	char *newstring = _strlwr( _strdup( strFileName.c_str() ));
	strFileName = newstring;
	free(newstring);

	tagFileInfo* pFileInfo = GetFileInfoByText(strFileName);
	if(pFileInfo)
		return pFileInfo->bFolder;
	return false;
}

//�õ�һ���ļ�����Ϣ
BOOL
CFilesInfo::GetFileInfo(const string& strFileName,DWORD& dwSize,
						DWORD& dwOrginSize,DWORD &dwValidSize,DWORD& dwCrc32,
						DWORD& dwPackageType,DWORD& dwCompressType,bool& bFolder)
{
	string strTemptFileName(strFileName);
	char *newstring = _strlwr( _strdup( strTemptFileName.c_str() ));
	strTemptFileName = newstring;
	free(newstring);

	dwCrc32=-1;dwPackageType=-1;dwCompressType=-1;
	tagFileInfo* pFileInfo = GetFileInfoByText(strTemptFileName);
	if(pFileInfo)
	{
		dwSize = pFileInfo->dwSize;
		dwOrginSize=pFileInfo->dwOrginSize;;
		dwValidSize=pFileInfo->dwValidSize;;
		dwCrc32= pFileInfo->dwCrc32;
		dwPackageType= pFileInfo->dwPackageType;
		dwCompressType= pFileInfo->dwCompressType;
		bFolder = pFileInfo->bFolder;
		return true;
	}
	return false;
}

//�����Ƿ����õ������ļ���Ϣ�б�Ĳ���
bool
CFilesInfo::GetDifferenceByPackage(CFilesInfo& filesInfo,CFilesInfo& temptFilesInfo)
{
	return GetDifferenceByPackage(GetRootFile(),filesInfo,temptFilesInfo,true);
}

bool
CFilesInfo::GetDifferenceByPackage(tagFileInfo* ptagFileInfo,CFilesInfo& filesInfo,
								   CFilesInfo& temptFilesInfo,bool bPackage)
{
	string strTempt = GetFileTextWithParent(ptagFileInfo);
	if(ptagFileInfo==NULL)	return false;
	if(HaveChild(ptagFileInfo))
	{
		temptFilesInfo.UpdateFileInfo(strTempt,0,0,0,0,true,ptagFileInfo->dwPackageType,
			ptagFileInfo->dwCompressType);

		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			GetDifferenceByPackage((*it).second,filesInfo,temptFilesInfo,bPackage);
		}
		
		//û�����ļ�����ɾ�����ļ���
		tagFileInfo* pTemptTagInfo = temptFilesInfo.GetFileInfoByText(strTempt);
		if(pTemptTagInfo->ChildFilesInfo.size()<=0)
		{
			temptFilesInfo.DelFileInfo(strTempt);
		}
	}
	else
	{
		if(ptagFileInfo->bFolder==false)
		{
			DWORD dwSize=0,dwOrginSize=0,dwValidSize=0;
			DWORD dwCrc32=2,dwPackageType=1,dwCompressType=1;
			bool bFolder=false;
			if(filesInfo.GetFileInfo(strTempt,dwSize,dwOrginSize,dwValidSize,
									dwCrc32,dwPackageType,dwCompressType,bFolder))
			{
				if(bPackage == true)
				{
					if(ptagFileInfo->dwCrc32 != dwCrc32  ||
						ptagFileInfo->dwPackageType != dwPackageType ||
						ptagFileInfo->dwCompressType != dwCompressType)
					{
						if( !ENABLEBIT(dwCompressType,NOCOMPRESS) ||
							ptagFileInfo->dwOrginSize != dwOrginSize)
						{
							temptFilesInfo.UpdateFileInfo(strTempt,
														ptagFileInfo->dwCrc32,
														ptagFileInfo->dwSize,
														ptagFileInfo->dwOrginSize,
														ptagFileInfo->dwValidSize,
														ptagFileInfo->dwPackageType,
														ptagFileInfo->dwCompressType);
						}
					}
				}
				else
				{
					if(ptagFileInfo->dwCrc32 != dwCrc32 ||
						(ptagFileInfo->dwCrc32 == dwCrc32 && dwValidSize != dwSize) )
					{
						temptFilesInfo.UpdateFileInfo(	strTempt,ptagFileInfo->dwCrc32,
														ptagFileInfo->dwSize,
														ptagFileInfo->dwOrginSize,
														ptagFileInfo->dwValidSize,
														ptagFileInfo->dwPackageType,
														ptagFileInfo->dwCompressType);
					}
				}
			}
			else
			{
				temptFilesInfo.UpdateFileInfo(strTempt,ptagFileInfo->dwCrc32,
												ptagFileInfo->dwSize,
												ptagFileInfo->dwOrginSize,
												ptagFileInfo->dwValidSize,
												ptagFileInfo->dwPackageType,
												ptagFileInfo->dwCompressType);
			}
		}
	}

	return true;
}

//�����Ƿ�仯�õ������ļ���Ϣ�б�Ĳ���
bool CFilesInfo::GetDifferenceByCrc32(CFilesInfo& filesInfo,CFilesInfo& temptFilesInfo)
{	
	return GetDifferenceByPackage(GetRootFile(),filesInfo,temptFilesInfo,false);
}


bool CFilesInfo::CopyData(CFilesInfo* pFileInfo)
{
	CopyData(GetRootFile(),pFileInfo);
	//pFileInfo->m_pDisplayWnd = m_pDisplayWnd;
	return true;
}

bool CFilesInfo::CopyData(tagFileInfo* ptagFileInfo,CFilesInfo* pFileInfo)
{
	if(pFileInfo==NULL || ptagFileInfo==NULL)	return false;

	if(ptagFileInfo != GetRootFile())
	{
		string strName(GetFileTextWithParent(ptagFileInfo));
		pFileInfo->UpdateFileInfo(	strName,ptagFileInfo->dwCrc32,ptagFileInfo->dwSize,
									ptagFileInfo->dwOrginSize,ptagFileInfo->dwValidSize,
									ptagFileInfo->bFolder,ptagFileInfo->dwPackageType,
									ptagFileInfo->dwCompressType);
	}

	if(HaveChild(ptagFileInfo))
	{
		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			CopyData((*it).second,pFileInfo);
		}
	}
	return true;
}

//�Ƿ����ӽڵ�
bool	CFilesInfo::HaveChild(tagFileInfo* pFieInfo)
{
	if(pFieInfo==NULL)	return false;
	if(pFieInfo->ChildFilesInfo.size() > 0)	return true;
	return false;
}

//�Ƿ���δ����ļ�
bool	CFilesInfo::ChildHaveUnPackFile(tagFileInfo* ptagFieInfo)
{
	for(itFileInfo it = ptagFieInfo->ChildFilesInfo.begin();
		it != ptagFieInfo->ChildFilesInfo.end();it++)
	{
		if((*it).second->bFolder==true || (*it).second->dwPackageType==0)
			return true;
	}
	return false;
}

//�ҵ�һ��ƥ���ַ��ӽڵ�
tagFileInfo* CFilesInfo::FindChildFileInfoByText(tagFileInfo* pParent,string& str)
{
	char *newstring = _strlwr( _strdup( str.c_str() ));
	str = newstring;
	free(newstring);

	if(pParent == NULL)	return NULL;
	if(str.compare("")==0)
		return m_pRootFile;

	if(HaveChild(pParent) == false)	return NULL;

	itFileInfo it = pParent->ChildFilesInfo.find(str);
	if(it != pParent->ChildFilesInfo.end())
		return (*it).second;
	return NULL;
}

//�����ַ����õ�һ���ڵ�
tagFileInfo* CFilesInfo::GetFileInfoByText(string strFileName)
{
	string str(strFileName);

	if(str.compare("") ==0 )
		return m_pRootFile;

	tagFileInfo* pFileInfo = GetRootFile();
	int nPos = str.find("\\");

	while(str.find("\\") ==0 && pFileInfo != NULL)
	{
		str = str.erase(0,str.find("\\")+1);
		string strTempt("");
		nPos = str.find("\\");
		if(nPos < 0)
		{
			strTempt=str;
			str = "";
		}
		else
		{
			strTempt.insert(strTempt.begin(),str.begin(),str.begin()+str.find("\\"));

			str = str.erase(0,str.find("\\"));
		}
		pFileInfo = FindChildFileInfoByText(pFileInfo,strTempt);
	}
	return pFileInfo;
}

//�õ�һ���ڵ�ĺ͸���������ַ���
string CFilesInfo::GetFileTextWithParent(tagFileInfo* pFileInfo)
{
	
	string str("");
	while(pFileInfo && pFileInfo != GetRootFile())
	{
		str= pFileInfo->strFileName+str;
		pFileInfo = pFileInfo->ParentFileInfo;
		str="\\"+str;
	}
	return str;
}

//����ļ���С��
void CFilesInfo::AddFileSize(tagFileInfo* pFileInfo,int iSize)
{
	if(pFileInfo==NULL)	return;
	pFileInfo->dwSize += iSize;

	//��������
	OnUpdate(pFileInfo);

	if(pFileInfo->ParentFileInfo)
		AddFileSize(pFileInfo->ParentFileInfo,iSize);
}


//ͳ�Ƹ��ļ�����Ϣ
void CFilesInfo::UpdateParentFolderInfo(tagFileInfo* ptagFileInfo)
{
	if(ptagFileInfo==NULL)	return;

	if(HaveChild(ptagFileInfo))
	{
		DWORD dwTemptPackage = 0,dwTemptCompress = 0;

		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			dwTemptPackage |= (*it).second->dwPackageType;
			dwTemptCompress |= (*it).second->dwCompressType;
		}
		if( ptagFileInfo->dwPackageType!=dwTemptPackage || ptagFileInfo->dwCompressType!=dwTemptCompress )
		{
			ptagFileInfo->dwPackageType=dwTemptPackage;
			ptagFileInfo->dwCompressType=dwTemptCompress;
			//��������
			OnUpdate(ptagFileInfo);
		}
	}

	UpdateParentFolderInfo(ptagFileInfo->ParentFileInfo);
}
//ͳ���ļ�����Ϣ
void CFilesInfo::UpdateFolderInfo(tagFileInfo* ptagFileInfo,DWORD& dwPackage,DWORD& dwCompress)
{
	if(ptagFileInfo==NULL)	return;

	if(ptagFileInfo->bFolder==true)
	{
		DWORD dwTemptPackage = 0,dwTemptCompress=0;

		if(HaveChild(ptagFileInfo))
		{
			for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
				it != ptagFileInfo->ChildFilesInfo.end();it++)
			{
				UpdateFolderInfo((*it).second,dwTemptPackage,dwTemptCompress);
			}

			if( ptagFileInfo->dwPackageType!=dwTemptPackage ||
				ptagFileInfo->dwCompressType!=dwTemptCompress )
			{
				ptagFileInfo->dwPackageType=dwTemptPackage;ptagFileInfo->dwCompressType=dwTemptCompress;
				//��������
				OnUpdate(ptagFileInfo);
			}
		}
		else
		{
			ptagFileInfo->dwPackageType=1;ptagFileInfo->dwCompressType=1;
		}

		dwPackage = dwPackage|dwTemptPackage;dwCompress = dwCompress|dwTemptCompress;
	}
	else
	{
		//if( ptagFileInfo->dwPackageType != 1 ||
		//	ptagFileInfo->dwCompressType != 1 )
		//{
		//	ptagFileInfo->dwPackageType=1;
		//	ptagFileInfo->dwCompressType=1;

		//	//��������
		//	OnUpdate(ptagFileInfo);
		//}

		dwPackage = (ptagFileInfo->dwPackageType|dwPackage);
		dwCompress= (ptagFileInfo->dwCompressType|dwCompress);
	}
}

void CFilesInfo::FindFileList(string& strFile, string& strExt, list<string>& listFileName)
{
	//ת����Сд
	char *newstring = _strlwr( _strdup( strFile.c_str() ));
	strFile = newstring;
	free(newstring);

	newstring = _strlwr( _strdup( strExt.c_str() ));
	strExt = newstring;
	free(newstring);
	
	string strTempt(strFile);
	tagFileInfo* ptagFileInfo = GetFileInfoByText(strTempt);
	if(ptagFileInfo)
	{
		FindFileList(ptagFileInfo,strExt,listFileName);
	}
}

void CFilesInfo::FindFileList(tagFileInfo* ptagFileInfo, string& strExt,
							  list<string>& listFileName)
{
	if(ptagFileInfo==NULL)	return;
	if(HaveChild(ptagFileInfo))
	{
		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			FindFileList((*it).second,strExt,listFileName);
		}
	}
	else
	{	
		if(ptagFileInfo->bFolder==false)
		{
			string strTempt = ptagFileInfo->strFileName;
			strTempt.erase(0,strTempt.find_last_of("."));
			if(strTempt==strExt)
			{
				strTempt = GetFileTextWithParent(ptagFileInfo);
				listFileName.push_back(strTempt);
			}
		}
	}
}

void CFilesInfo::OnAdd(tagFileInfo* ptagFileInfo)
{
	if(ptagFileInfo->bFolder == false)
		m_dwFileNum++;
	if(ptagFileInfo==NULL)	return;
	if(m_pDisplayWnd)
	{
		::SendMessage(m_pDisplayWnd,TREELIST_ADD,(WPARAM)this,(LPARAM)ptagFileInfo);
	}
}

void CFilesInfo::OnUpdate(tagFileInfo* ptagFileInfo)
{
	if(ptagFileInfo==NULL)	return; 
	if(m_pDisplayWnd)
	{
		::SendMessage(m_pDisplayWnd,TREELIST_UPDATE,(WPARAM)this,(LPARAM)ptagFileInfo);
	}
}

void CFilesInfo::OnDelete(tagFileInfo* ptagFileInfo)
{
	if(ptagFileInfo->bFolder == false)
		m_dwFileNum++;

	if(ptagFileInfo==NULL)	return;
	if(m_pDisplayWnd)
	{
		::SendMessage(m_pDisplayWnd,TREELIST_DELETE,(WPARAM)this,(LPARAM)ptagFileInfo);
	}
}

void CFilesInfo::OnDeleteAll()
{
	m_dwFileNum = 0;

	if(m_pDisplayWnd)
	{
		::SendMessage(m_pDisplayWnd,TREELIST_DELALL,0,0);
	}
}

void CFilesInfo::OnIntial()
{
	if(m_pDisplayWnd)
	{
		::SendMessage(m_pDisplayWnd,TREELIST_INITIAL,0,0);
	}
}

void CFilesInfo::UpdateAllInfoToView(HWND hWnd)
{
	if(hWnd==NULL)
		hWnd=m_pDisplayWnd;

	::SendMessage(hWnd,TREELIST_INITIAL,0,0);
	UpdateAllInfoToView(GetRootFile(),hWnd);
}

void CFilesInfo::UpdateAllInfoToViewEx(HWND hWnd)
{
	if(hWnd==NULL)
		hWnd=m_pDisplayWnd;

	::SendMessage(hWnd,TREELIST_INITIAL,0,0);
	::SendMessage(hWnd,TREELIST_UPDATEWITHCHILD,(WPARAM)this,(LPARAM)GetRootFile());
}

void CFilesInfo::UpdateAllInfoToView(tagFileInfo* ptagFileInfo,HWND hWnd)
{
	if(ptagFileInfo==NULL)	return;
	::SendMessage(hWnd,TREELIST_ADD,(WPARAM)this,(LPARAM)ptagFileInfo);

	if(HaveChild(ptagFileInfo))
	{
		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			UpdateAllInfoToView((*it).second,hWnd);
		}
	}
}

DWORD	GetFirstEnableBit(DWORD dwNum)
{
	int i=0;
	for(;i< sizeof(DWORD);i++)
	{
		if( ENABLEBIT(dwNum,1<<i) )
			break;
	}
	return 1<<i;
}