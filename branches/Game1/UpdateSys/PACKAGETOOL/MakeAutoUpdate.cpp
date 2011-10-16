//////////////////////////////////////////////////////////////////////////
//MakeAutoUpdate.cpp
//Fun:���������Զ����°�
//Create Time:2004.11.16
//Author:Joe
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include <sstream>
#include ".\makeautoupdate.h"
#include "../../PUBLIC/Crc32Static.h"
#include "../../PUBLIC/MD5.h"
#include "../../public/tools.h"
#include "../../public/ziplib/zip.h"
#include <algorithm>
#include "../../PUBLIC/readwrite.h"
#include "../../PUBLIC/ziplib/unzip.h"

#define  IDR_PATCH1 146
#define  IDR_PATCH2 147
//#define  IDR_PATCH3 148
#define  IDS_UPDATEDVERSION 145


CMakeAutoUpdate::CMakeAutoUpdate(string& strVersion,string& strExeFolder,DWORD dwPackType,
								 CFilesInfo* LastFilesInfo,CClientResource* pClientResource)
								 :m_strVersion(strVersion)
								 ,m_strExeFoler(strExeFolder)
								 ,m_pLastFilesInfo(LastFilesInfo)
								 ,m_pOriginResource(pClientResource)
								 ,m_dwPackType(dwPackType)
{
}

CMakeAutoUpdate::~CMakeAutoUpdate(void)
{
}


//�����ļ���ʱ������ǰ����ļ�����д����Եİ��������������õİ���
//ѹ�����ͼ̳и��ļ�������
void	CMakeAutoUpdate::MakeAutoUpdatePackage(long lCurMode/*����ģʽ 0����������1���ͻ���*/, bool bIsSelcted/*0:�ӵ�ǰ�汾���� 1:��ѡ��汾����*/, bool bPatch)
{
	CFilesInfo* pSouFilesInfo = m_pOriginResource->GetFilesInfo();
	string oldVersion = "";
	if(bIsSelcted)
		oldVersion = m_pLastFilesInfo->GetVersion();
	else
		oldVersion = pSouFilesInfo->GetVersion();

	CFilesInfo DifFilesInfo;
	pSouFilesInfo->GetDifferenceByCrc32(*m_pLastFilesInfo,DifFilesInfo);

	//������µĴ���ļ�Ҫд�뵥���Ĳ�����
	if( !ENABLEBIT(m_dwPackType,NOPACK) )
	{
		DifFilesInfo.UpdateFilePackInfoWithChild(DifFilesInfo.GetRootFile(),m_dwPackType);
		pSouFilesInfo->UpdateFilePackInfoWithChild(&DifFilesInfo);
	}

	string strTemptDirec = m_strExeFoler+"\\AutoUpdate-";
	strTemptDirec += m_strVersion;

	//û�б仯
	if(DifFilesInfo.GetRootFile()->ChildFilesInfo.size() <= 0)
	{
		AfxMessageBox("��Դ�ļ�û�з����仯!");
	}
	else
	{
		::CreateDirectory(strTemptDirec.c_str(),NULL);

		DifFilesInfo.SetPackageTypeInfo(pSouFilesInfo->GetPackageTypeInfo());

		//��ȡ�����ļ�
		m_pOriginResource->ExtrAutoUpdateFileFolder(DifFilesInfo.GetRootFile(),strTemptDirec,&DifFilesInfo);

		//�Ƿ�����������
		if( bPatch == true )
		{
			MakePacth(lCurMode, DifFilesInfo);
		}
	}

	//��������б�
	string strUFIFileName = strTemptDirec +"\\PackRes.cfg";
	DifFilesInfo.SetVersion(m_strVersion);
	DifFilesInfo.Save(strUFIFileName);
	DWORD dwCrc32 = 0;
	
	if(lCurMode == 1)
	{
		// �������ļ���crc32ֵ
		string crc32Name = m_pOriginResource->GetCurFolder() + "\\setup\\serverlist.ini";

		CCrc32Static::FileCrc32Filemap(crc32Name.c_str(), dwCrc32);

		if(dwCrc32 == 0)
		{
			MessageBox(NULL, "MakeAutoUpdatePackage()δ�ҵ�����ĿsetupĿ¼�µ�serverlist.ini�ļ���", "����", MB_OK);
		}
	}
	
	strUFIFileName = strTemptDirec +"\\AllFilesInfo.cfg";
	pSouFilesInfo->SetVersion(m_strVersion);
	pSouFilesInfo->Save(strUFIFileName);

	string strFileName1 = m_pOriginResource->GetCurFolder()+"\\PackRes.cfg";
	pSouFilesInfo->Save(strFileName1);
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

	// �������ļ���crc32ֵ
	long patchLen = 0;
	DWORD dwCalCrc32 = 0;

	if(lCurMode == 1)
	{
		string exeName = m_strExeFoler + "\\patch";
		char strNewPatchExe[255];
		sprintf(strNewPatchExe,"\\Game%sPatch.exe",m_strVersion.c_str());
		exeName.append(strNewPatchExe);
		
		HANDLE hFile0 = CreateFile(exeName.c_str(),
			GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

		if (hFile0 == INVALID_HANDLE_VALUE) { 
			return; 
		} 

		DWORD dwSize0 = ::GetFileSize(hFile0,NULL);
		BYTE* pBuf0 = new BYTE[dwSize0];
		DWORD dwReadSize;
		BOOL result = ReadFile(hFile0,pBuf0,dwSize0,&dwReadSize,NULL);
		if( !result )
		{
			delete []pBuf0;
			return;
		}
		CloseHandle(hFile0);
		if (result == FALSE) 
		{ 
			delete []pBuf0;
			return;
		} 
		if(dwSize0 != dwReadSize)
		{ 
			delete []pBuf0;
			return;
		} 

		CCrc32Static::DataCrc32(pBuf0, dwSize0, dwCalCrc32);
		delete []pBuf0;
	}
	else // ��������zip�ļ�Crc32ֵ
	{
		string exeName = m_strExeFoler + "\\patch\\ServerPatch\\";
		char strNewPatchExe[255];
		sprintf(strNewPatchExe,"\\%s.zip",m_strVersion.c_str());
		exeName.append(strNewPatchExe);

		HANDLE hFile0 = CreateFile(exeName.c_str(),
			GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

		if (hFile0 == INVALID_HANDLE_VALUE) { 
			return; 
		} 

		DWORD dwSize0 = ::GetFileSize(hFile0,NULL);
		BYTE* pBuf0 = new BYTE[dwSize0];
		DWORD dwReadSize;
		BOOL result = ReadFile(hFile0,pBuf0,dwSize0,&dwReadSize,NULL);
		if( !result )
		{
			delete []pBuf0;
			return;
		}
		CloseHandle(hFile0);
		if (result == FALSE) 
		{ 
			delete []pBuf0;
			return;
		} 
		if(dwSize0 != dwReadSize)
		{ 
			delete []pBuf0;
			return;
		} 

		CCrc32Static::DataCrc32(pBuf0, dwSize0, dwCalCrc32);
		delete []pBuf0;
	}

	// д�뵽��Ŀ¼��Game.vs�ļ�
	string vsName = m_strExeFoler + "\\Game.vs";
	//FILE* f1 = fopen(vsName.c_str(), "ab");
	//if(f1)
	//{
	//	fwrite(m_strVersion.c_str(), 1, m_strVersion.size() + 2, f1);
	//	fclose(f1);
	//}

	string pathVsName = m_strExeFoler + "\\Patch\\Game.vs";
	if(!::CopyFile(vsName.c_str(), pathVsName.c_str(), FALSE))
		AfxMessageBox("����Game.vs�ļ���Patch·������!");

	//���ɸ����б�
	string strVsName = m_strExeFoler + "\\Patch\\Update-"+ m_strVersion +".vs";
	FILE* verFile = fopen(strVsName.c_str(), "ab+");
	fclose(verFile);

	ofstream myfile;
	myfile.open(strVsName.c_str());
	if (!myfile.is_open())
	{
		return;
	}

	string tmp;
				
	myfile << dwCalCrc32 << "\r\n";
	myfile << m_strVersion << "\r\n";
	
	// ��д�ϴΰ汾�б�
	string newStrVsName = m_strExeFoler + "\\Patch\\Update-"+ oldVersion +".vs";
	ofstream omyfile;
	FILE* newOVerFile = fopen(newStrVsName.c_str(), "ab+");
	fclose(newOVerFile);
	omyfile.open(newStrVsName.c_str());
	if (!omyfile.is_open())
	{
		return;
	}
	omyfile << dwCalCrc32 << "\r\n";	
	omyfile << m_strVersion << "\r\n";
	AfxMessageBox("�Զ����°��������!");
}

void	CMakeAutoUpdate::MakePacth(long lCurMode/*����ģʽ 0����������1���ͻ���*/, CFilesInfo& DifFilesInfo)
{
	string strTemptDirec = m_strExeFoler+"\\Patch";
	string strTemptPackageName("Patch");
	string strTemptFilesInfoName("\\Patch.cfg");
	strTemptFilesInfoName = strTemptDirec + strTemptFilesInfoName;

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
		DifFilesInfo.Save(strTemptFilesInfoName);

		strTemptPackageName = strTemptDirec + "\\" + strTemptPackageName;

		if(!UpdatePatch(lCurMode, strTemptDirec,strTemptPackageName,strTemptFilesInfoName))
		{
			AfxMessageBox("����������ʧ�ܡ�");
		}
	}
	else
	{
		AfxMessageBox("����������ʧ�ܡ�");
	}
}
bool	CMakeAutoUpdate::MakePatchPackage(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,
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

//����ִ���ļ������Դ
bool CMakeAutoUpdate::UpdatePatch(long lCurMode/*����ģʽ 0����������1���ͻ���*/, string& strDir,string& strPackageName,string& strFilesInfoName)
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

	BOOL result;
	HANDLE hUpdateRes;  // update resource handle 
	
	if(lCurMode == 1)
	{
		CopyFile(strPatchExe.c_str(),strNewExe.c_str(),FALSE);
		hUpdateRes = BeginUpdateResource(strNewExe.c_str(), FALSE); 
		if (hUpdateRes == NULL) 
		{ 
			return false; 
		} 
	}

	//���ļ���Ϣ����д��ʱ�ļ�
	HANDLE hFile0 = CreateFile(strFilesInfoName.c_str(),
		GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

	if (hFile0 == INVALID_HANDLE_VALUE) 
	{ 
		return FALSE; 
	} 

	DWORD dwSize0 = ::GetFileSize(hFile0,NULL);
	BYTE* pBuf0 = new BYTE[dwSize0];
	DWORD dwReadSize;
	result = ReadFile(hFile0,pBuf0,dwSize0,&dwReadSize,NULL);
	if( !result )
	{
		delete []pBuf0;
		return false;
	}
	CloseHandle(hFile0);
	if (result == FALSE) 
	{ 
		delete []pBuf0;
		return false;
	} 

	//�Ѱ�����д��ʱ�ļ�
	HANDLE hFile1 = CreateFile(strPackageName.c_str(),
		GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

	if (hFile1 == INVALID_HANDLE_VALUE)
	{ 
		return FALSE; 
	} 

	DWORD dwSize1 = ::GetFileSize(hFile1,NULL);
	BYTE* pBuf1 = new BYTE[dwSize1];
	result = ReadFile(hFile1,pBuf1,dwSize1,&dwReadSize,NULL);
	if( !result )
	{
		delete []pBuf1;
		return false;
	}
	CloseHandle(hFile1);
	if (result == FALSE) 
	{ 
		delete []pBuf1;
		return false;
	} 

	//��Update.txtд��ʱ�ļ�
	string strUpdateTxtName = m_strExeFoler + "\\Patch\\Update.txt";
	HANDLE hFile2 = NULL;
	DWORD dwSize2 = 0;
	BYTE* pBuf2 = NULL;
	if(lCurMode == 1)
	{
		hFile2 = CreateFile(strUpdateTxtName.c_str(),
			GENERIC_READ,0,(LPSECURITY_ATTRIBUTES) NULL,OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);

		if (hFile2 == INVALID_HANDLE_VALUE) 
		{ 
			PutDebugString("���ʱδ�ҵ�\\Patch\\Update.txt�ļ�!");
			return FALSE; 
		} 

		dwSize2 = ::GetFileSize(hFile2,NULL);
		pBuf2 = new BYTE[dwSize2];
		result = ReadFile(hFile2,pBuf2,dwSize2,&dwReadSize,NULL);
		if( !result )
		{
			delete []pBuf2;
			return false;
		}
		CloseHandle(hFile2);
		if (result == FALSE) 
		{ 
			delete []pBuf2;
			return false;
		}
	}

	// crcֵ��֤
	DWORD resCrcValue = 0;

	// �����ݿ�ѹ��
	string zipPath = m_strExeFoler + "\\Patch\\tmp.zip";
	zipFile tZipFile = zipOpen(zipPath.c_str(), 0);
	if(tZipFile != NULL)
	{
		// �ļ���Ϣ
		zipOpenNewFileInZip(tZipFile, "Patch.cfg", NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
		zipWriteInFileInZip(tZipFile, pBuf0, dwSize0);
		zipCloseFileInZip(tZipFile);
		// ������
		zipOpenNewFileInZip(tZipFile, "Patch.dp", NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
		zipWriteInFileInZip(tZipFile, pBuf1, dwSize1);
		zipCloseFileInZip(tZipFile);
		if(lCurMode == 1)
		{
			// Update.txt
			zipOpenNewFileInZip(tZipFile, "Update.txt", NULL, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
			zipWriteInFileInZip(tZipFile, pBuf2, dwSize2);
			zipCloseFileInZip(tZipFile);
		}
		zipClose(tZipFile, NULL);
	}

	CCrc32Static::DataCrc32(pBuf0, dwSize0, resCrcValue);
	CCrc32Static::DataCrc32(pBuf1, dwSize1, resCrcValue);

	delete []pBuf0;
	delete []pBuf1;
	delete []pBuf2;

	
	//��zip�ļ�д����Դ
	string strZipFileName = m_strExeFoler + "\\Patch\\tmp.zip";
	FILE* hFile3 = fopen(strZipFileName.c_str(), "rb");
	if (hFile3 == NULL) 
	{ 
		return FALSE; 
	} 

	DWORD dwSize3 = 0;
	fseek(hFile3, 0, SEEK_END);
	dwSize3 = ftell(hFile3);
	fseek(hFile3, 0, SEEK_SET);

	BYTE* pBuf3 = new BYTE[dwSize3];

	size_t readSize = fread(pBuf3, dwSize3, 1, hFile3);

	if( readSize != 1)
	{
		char strErrorInfo[1024]="";
		_snprintf(strErrorInfo,1024,"��ȡzip�ļ�(size:%d)����.",readSize);
		AfxMessageBox(strErrorInfo);		
	}
	fclose(hFile3);

	if(lCurMode == 1) // �ͻ���
	{
		result = UpdateResource(hUpdateRes,"PATCH",
			MAKEINTRESOURCE(IDR_PATCH1), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			pBuf3,
			dwSize3);

		if(result==FALSE)
		{
			MessageBox(NULL, "�����Դzip����ʧ�ܣ�", "����", MB_OK);
			return false;
		}

		delete []pBuf3;
		
		//���������İ汾
		long szVerLen = m_pLastFilesInfo->GetVersion().size()+1;
		char *SzVer = new char[szVerLen];
		memset(SzVer, 0, szVerLen);
		memcpy(SzVer, &m_pLastFilesInfo->GetVersion()[0], m_pLastFilesInfo->GetVersion().size());
		result = UpdateResource(hUpdateRes,"PATCH",
			MAKEINTRESOURCE(IDR_PATCH2), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
			(VOID*)SzVer, szVerLen);
		SAFE_DELETE_ARRAY(SzVer);

		if (result == FALSE) 
		{ 
			MessageBox(NULL, "�����Դ�汾������ʧ�ܣ�", "����", MB_OK);
			return false;
		}

		if (!EndUpdateResource(hUpdateRes, FALSE)) 
		{ 
			MessageBox(NULL, "���������Դ����ʧ�ܣ�", "����", MB_OK);
			return false; 
		}

		// ��Դ���crc����
		HGLOBAL  hResLoad;     // handle to loaded resource 
		HRSRC hRes;         // handle/ptr. to res. info. in hExe 
		VOID *lpResLock;    // pointer to resource data 

		//#ifndef _DEBUG
		//�õ����ļ�����
	//	GetModuleFileName()
		HMODULE ip = LoadLibrary(strNewExe.c_str());
		//HMODULE ip = LoadLibrary("d:\\Game00.00.0001Patch.exe");
		hRes = ::FindResource((HMODULE)ip,MAKEINTRESOURCE(IDR_PATCH1),"PATCH");
		if( hRes == NULL ) 
		{ 
			MessageBox(NULL, "���Դ���Դ����ʧ�ܣ�", "����", MB_OK);
			return false; 
		} 

		hResLoad = ::LoadResource((HMODULE)ip, hRes); 
		if (hResLoad == NULL) 
		{ 
			MessageBox(NULL, "1���Զ�ȡ��Դ����ʧ�ܣ�", "����", MB_OK);
			return false;
		} 

		// Lock the dialog box into global memory. 
		lpResLock = ::LockResource(hResLoad); 
		if (lpResLock == NULL) 
		{ 
			MessageBox(NULL, "2���Զ�ȡ��Դ����ʧ�ܣ�", "����", MB_OK);
			return false;
		}

		string tmpZipFileNmae = m_strExeFoler + "\\testtmp.zip";
		FILE* pf = fopen(tmpZipFileNmae.c_str(),"wb");
		if(!pf)
		{	
			char str[100]="";
			sprintf(str,"������ʱZIP�ļ� %s ����",tmpZipFileNmae.c_str());
			AfxMessageBox(str);
			return false;
		}	

		DWORD dwSize = SizeofResource((HMODULE)ip,hRes);
		DWORD dwCount = fwrite(lpResLock,dwSize,1, pf);
		if(dwCount != 1)
		{
			char str[100]="";
			sprintf(str,"д��ʱZIP�ļ� %s ���ݳ��������ԡ�",tmpZipFileNmae.c_str());
			AfxMessageBox(str);
			return false;
		}

		fclose(pf);
	
		// ��crc����ֵ
		DWORD TestResCrcValue = 0;
		DWORD TestDpResCrcValue = 0;

		//��ѹ������
		unzFile dataZipFile = unzOpen(tmpZipFileNmae.c_str());
		if(dataZipFile)
		{
			unz_file_info zipFileInfo;
			
			// �ҵ�Patch.cfg
			if( UNZ_OK == unzLocateFile(dataZipFile, "Patch.cfg", 1) )
			{
				unzGetCurrentFileInfo(dataZipFile, &zipFileInfo, NULL, 0,
					NULL, 0, NULL, 0);

				char* buf = new char[zipFileInfo.uncompressed_size];
				unzOpenCurrentFile(dataZipFile);
				unzReadCurrentFile(dataZipFile, buf, zipFileInfo.uncompressed_size);
				CCrc32Static::DataCrc32(buf, zipFileInfo.uncompressed_size, TestResCrcValue);
				unzCloseCurrentFile(dataZipFile);
				FILE* f = fopen("TestPatch.cfg", "wb");
				fwrite(buf, zipFileInfo.uncompressed_size, 1, f);
				fclose(f);
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
				CCrc32Static::DataCrc32(buf, zipFileInfo.uncompressed_size, TestResCrcValue);
				CCrc32Static::DataCrc32(buf, zipFileInfo.uncompressed_size, TestDpResCrcValue);
				unzCloseCurrentFile(dataZipFile);
				SAFE_DELETE_ARRAY(buf);

				if(TestDpResCrcValue!=zipFileInfo.crc)
				{
					char str[100]="";
					sprintf(str,"�ļ�CRC��%u��zipCRC��%u��DP������ʧ�ܡ�", TestDpResCrcValue, zipFileInfo.crc);
					AfxMessageBox(str);
					return false;
				}
			}
		}

		if(TestResCrcValue!=resCrcValue)
		{
			char str[100]="";
			sprintf(str,"���CRC��%u������CRC��%u������ʧ�ܡ�", resCrcValue, TestResCrcValue);
			AfxMessageBox(str);
			return false;
		}
		else
		{
			char str[100]="";
			sprintf(str,"���CRC��%u������CRC��%u�����Գɹ���", resCrcValue, TestResCrcValue);
			AfxMessageBox(str);
		}
	}
	else
	{
		string strServerFilePath = m_strExeFoler + "\\Patch\\ServerPatch\\";
		CreateDirectory(strServerFilePath.c_str(), NULL);
		string strServerFileName = strServerFilePath + m_strVersion + ".zip";
		CopyFile(strZipFileName.c_str(), strServerFileName.c_str(), TRUE);
	}
	
	return true;
}
