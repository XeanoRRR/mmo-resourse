//////////////////////////////////////////////////////////////////////////
//MakePatch.h
//Fun:������������������
//Create Time:2004.12.09
//Author:Joe
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\makepatch.h"
#include "../../PUBLIC/Crc32Static.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define  IDR_PATCH1 146
#define  IDR_PATCH2 147
//#define  IDR_PATCH3 148
#define  IDS_UPDATEDVERSION 145

CMakePatch::CMakePatch(string& strVersion,string& strExeFolder,DWORD dwPackType,
					   CFilesInfo* LastFilesInfo,CClientResource* pClientResource)
:m_strVersion(strVersion)
,m_strExeFoler(strExeFolder)
,m_pLastFilesInfo(LastFilesInfo)
,m_pOriginResource(pClientResource)
,m_dwPackType(dwPackType)
{
}

CMakePatch::~CMakePatch(void)
{
}


//��������������
void	CMakePatch::MakePatch(bool bSelect)
{
	CFilesInfo* pSouFilesInfo = m_pOriginResource->GetFilesInfo();
	CFilesInfo DifFilesInfo;
	pSouFilesInfo->GetDifferenceByCrc32(*m_pLastFilesInfo,DifFilesInfo);

	//������µĴ���ļ�Ҫд�뵥���Ĳ�����
	if( !ENABLEBIT(m_dwPackType,NOPACK) )
	{
		DifFilesInfo.UpdateFilePackInfoWithChild(DifFilesInfo.GetRootFile(),m_dwPackType);
		pSouFilesInfo->UpdateFilePackInfoWithChild(&DifFilesInfo);
	}

	string strTemptDirec = m_strExeFoler+"\\Patch";
	string strTemptPackageName("Patch");
	string strTemptFilesInfoName("\\Patch.cfg");
	strTemptFilesInfoName = strTemptDirec + strTemptFilesInfoName;
	//û�б仯
	if(DifFilesInfo.GetRootFile()->ChildFilesInfo.size() <= 0)
	{
		AfxMessageBox("��Դ�ļ�û�з����仯!");
	}
	else
	{
		::CreateDirectory(strTemptDirec.c_str(),NULL);

		DWORD dwFileSize = DifFilesInfo.GetFileNum();
		CPackage* pPackage = new CPackage(NULL,strTemptDirec,dwFileSize,0);
		pPackage->Create(strTemptPackageName.c_str());

		//DifFilesInfo.
		if(MakePatchPackage(DifFilesInfo.GetRootFile(),&DifFilesInfo,pPackage))
		{
			pPackage->WriteIndexEx();
			delete pPackage;

			DifFilesInfo.SetVersion(m_strVersion);
			DifFilesInfo.SetPackageTypeInfo(pSouFilesInfo->GetPackageTypeInfo());
			DifFilesInfo.Save(strTemptFilesInfoName);

			strTemptPackageName = strTemptDirec + "\\" + strTemptPackageName;

			if(UpdatePatch(strTemptDirec,strTemptPackageName,strTemptFilesInfoName))
			{
				pSouFilesInfo->SetVersion(m_strVersion);
				
				string strFileName1 = m_pOriginResource->GetCurFolder()+"\\PackRes.cfg";
				pSouFilesInfo->Save(strFileName1);

				if(bSelect==false)
				{
					//���ݾɵ��ļ���Ϣ
					char strSaveName[256]="";
					sprintf(strSaveName,"\\%sFilesInfo.cfg",m_pLastFilesInfo->GetVersion().c_str());
					strFileName1 = m_strExeFoler+strSaveName;
					m_pLastFilesInfo->SetPackageTypeInfo(pSouFilesInfo->GetPackageTypeInfo());
					m_pLastFilesInfo->Save(strFileName1);
				}

				//�����ļ���Ϣ
				strFileName1 = m_strExeFoler+"\\LastUpdate.cfg";
				pSouFilesInfo->Save(strFileName1);

				//�����Զ������ļ���
				UpdateAutoUpdateFolder(DifFilesInfo,pSouFilesInfo);
			}
			else
			{
				AfxMessageBox("��������ʧ��!");
				return;
			}
		}
	}

	//DeleteFile(strTemptPackageName.c_str());
	//DeleteFile(strTemptFilesInfoName.c_str());

	AfxMessageBox("�����������!");
}

bool	CMakePatch::MakePatchPackage(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,
									 CPackage* pPackage)
{
	if(ptagFileInfo==NULL || pFilesInfo == NULL || pPackage == NULL)	return false;

	if(pFilesInfo->HaveChild(ptagFileInfo))
	{
		for(itFileInfo it = ptagFileInfo->ChildFilesInfo.begin();
			it != ptagFileInfo->ChildFilesInfo.end();it++)
		{
			if( !MakePatchPackage((*it).second,pFilesInfo,pPackage) )
				return false;
		}
	}
	else if(ptagFileInfo->bFolder == false)
	{
		string strFileName = pFilesInfo->GetFileTextWithParent(ptagFileInfo);
		string strTemptFileName = m_pOriginResource->GetCurFolder()+strFileName;

		FILE* pf= fopen(strTemptFileName.c_str(),"rb");
		if(pf)
		{
			DWORD dwOrginSize = 0;
			DWORD dwSize = pPackage->AddFile(pf,strFileName.c_str(),ptagFileInfo->dwCrc32,
											ptagFileInfo->dwCompressType, dwOrginSize);

			ptagFileInfo->dwSize = dwSize;
			ptagFileInfo->dwValidSize = dwSize;
			fclose(pf);

		}
	}
	return true;
}

//�����Զ����µ��ļ�������ļ�
bool	CMakePatch::UpdateAutoUpdateFolder(CFilesInfo& DifFilesInfo,CFilesInfo* pSouFilesInfo)
{
	if(pSouFilesInfo==NULL)	return false;

	string strTemptDirec = m_strExeFoler+"\\AutoUpdate-";
	strTemptDirec += pSouFilesInfo->GetVersion();

	//û�б仯
	if(DifFilesInfo.GetRootFile()->ChildFilesInfo.size() <= 0)
	{
		AfxMessageBox("��Դ�ļ�û�з����仯!");
	}
	else
	{
		::CreateDirectory(strTemptDirec.c_str(),NULL);

		//��ȡ�����ļ�
		m_pOriginResource->ExtrAutoUpdateFileFolder(DifFilesInfo.GetRootFile(),strTemptDirec,&DifFilesInfo);
	}

	//��������б�
	string strUFIFileName = strTemptDirec +"\\PackRes.cfg";
	pSouFilesInfo->SetVersion(m_strVersion);
	pSouFilesInfo->Save(strUFIFileName);
	DWORD dwCrc32;
	CCrc32Static::FileCrc32Filemap(strUFIFileName.c_str(),dwCrc32);

	//����汾��
	string strVersion = strTemptDirec +"\\AutoUpdateGame.vs";
	FILE* pf = fopen(strVersion.c_str(),"wb");
	if(pf)
	{
		fprintf(pf,"%s\n",m_strVersion.c_str());
		fprintf(pf,"%u",dwCrc32);
		fclose(pf);
	}
	//����汾���ļ���Patch�ļ���
	string strAutoVersion = m_strExeFoler + string("\\patch") +"\\AutoUpdateGame.vs";
	FILE* pfAuto = fopen(strAutoVersion.c_str(),"wb");
	if(pfAuto)
	{
		fprintf(pfAuto,"%s\n",m_strVersion.c_str());
		fprintf(pfAuto,"%u",dwCrc32);
		fclose(pfAuto);
	}

	//���ɸ����б�
	string strVsName = strTemptDirec +"\\Patch\\Update-"+m_strVersion+".vs";
	FILE* vsFile = fopen(strVsName.c_str(), "wb");
	if(vsFile)
	{
		fwrite(m_strVersion.c_str(), m_strVersion.size(), 1, vsFile);
		fwrite("\r\n", 2, 1, vsFile);
		fprintf(pf,"%u",dwCrc32);
	}
	fclose(vsFile);

	return true;
}

//����ִ���ļ������Դ
bool CMakePatch::UpdatePatch(string& strDir,string& strPackageName,string& strFilesInfoName)
{
	string strPatchExe("\\Patch.exe");
	strPatchExe = m_strExeFoler + strPatchExe;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	strPackageName += ".dp";
	hFind = ::FindFirstFile(strPackageName.c_str(), &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		return false;
	FindClose(hFind);

	char strNewPatchExe[255];
	sprintf(strNewPatchExe,"\\Game%sPatch.exe",m_strVersion.c_str());
	string strNewExe = strDir+strNewPatchExe;
	//string strNewExe = strDir+"\\Patch.exe";

	CopyFile(strPatchExe.c_str(),strNewExe.c_str(),FALSE);

	BOOL result;
	HANDLE hUpdateRes;  // update resource handle 


	hUpdateRes = BeginUpdateResource(strNewExe.c_str(), FALSE); 
	if (hUpdateRes == NULL) 
	{ 
		return false; 
	} 

	//���ļ���Ϣ����д��ʱ�ļ�
	HANDLE hFile = CreateFile(strFilesInfoName.c_str(),
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
		return false;
		delete []pBuf;
	}
	pBuf[dwSize]='\0';

	result = UpdateResource(hUpdateRes,"PATCH",
		MAKEINTRESOURCE(IDR_PATCH1), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		pBuf,
		dwSize);

	delete []pBuf;
	CloseHandle(hFile);
	if (result == FALSE) 
	{ 
		return false;
	} 


	//�Ѱ�����д��ʱ�ļ�
	hFile = CreateFile(strPackageName.c_str(),
		GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

	if (hFile == INVALID_HANDLE_VALUE) { 
		return FALSE; 
	} 

	dwSize = ::GetFileSize(hFile,NULL);
	pBuf = new BYTE[dwSize+1];
	result = ReadFile(hFile,pBuf,dwSize,&dwReadSize,NULL);
	if( !result )
	{
		return false;
		delete []pBuf;
	}
	pBuf[dwSize]='\0';

	result = UpdateResource(hUpdateRes,"PATCH",
		MAKEINTRESOURCE(IDR_PATCH2), 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		pBuf,
		dwSize);

	delete []pBuf;
	CloseHandle(hFile);
	if (result == FALSE) 
	{ 
		return false;
	} 

	//���ñ������İ汾
	/*result = UpdateResource(hUpdateRes,RT_STRING,
		MAKEINTRESOURCE(IDS_UPDATEDVERSION),  
		MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
		(VOID*)(m_pOriginResource->GetVersion().c_str()),
		m_pOriginResource->GetVersion().length());*/

	//result = UpdateResource(hUpdateRes,"PATCH",
	//	MAKEINTRESOURCE(IDR_PATCH3), 
	//	MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
	//	(VOID*)(m_pLastFilesInfo->GetVersion().c_str()),
	//	m_pLastFilesInfo->GetVersion().length());


	if (result == FALSE) 
	{ 
		return false;
	} 

	if (!EndUpdateResource(hUpdateRes, FALSE)) 
	{ 
		return false; 
	} 

	return true;
}