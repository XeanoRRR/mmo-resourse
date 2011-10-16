

#include "stdafx.h"
#include "UpdateFolder.h"



UpdateFolder::UpdateFolder(VOID)
{

}
UpdateFolder::~UpdateFolder(VOID)
{

}

//! ��һ��Ŀ¼�µ��������ݶ��뵽�����У�������ǰ�����ݣ�
BOOL UpdateFolder::LoadDataFromDir(LPCSTR pDirPath)
{
	if(NULL == pDirPath)
		return FALSE;

	strcpy(m_Root.szName, pDirPath);
	return  (0 < LoadDataFromDir(pDirPath, m_Root));
}
//! �Ѱ��е�����д�뵽ָ����Ŀ¼·��
BOOL UpdateFolder::WriteDataToDir(LPCSTR pDirPath)
{
	return (0 < WriteDataToDir(pDirPath, m_Root));
}

//! �ݹ麯������ȡһ���ļ��е��������ݵ�����Ľڵ���
DWORD UpdateFolder::LoadDataFromDir(LPCSTR pDirPath, tagDirectory &Directory)
{
	DWORD dwLoadFileNum = 0;
	string strFind(pDirPath);

	WIN32_FIND_DATA FindData;
	HANDLE hFind = ::FindFirstFile((strFind + "\\*").c_str(), &FindData);
	if(INVALID_HANDLE_VALUE != hFind)
	{
		do 
		{
			if((FILE_ATTRIBUTE_DIRECTORY == FindData.dwFileAttributes))
			{
				if(strcmp(FindData.cFileName, ".") && strcmp(FindData.cFileName, ".."))
				{			
					//! �����½ڵ�
					Directory.listDir.push_back(tagDirectory());
					tagDirectory &NewDir = *(-- Directory.listDir.end());
					strcpy(NewDir.szName, FindData.cFileName);
					//! �ݹ����
					dwLoadFileNum += LoadDataFromDir((strFind + "\\" + FindData.cFileName).c_str(), NewDir);
				}
			}
			else
			{
				Directory.listFile.push_back(CUpdateFile(FindData.cFileName));
				CUpdateFile &NewFile = *(-- Directory.listFile.end());
				if(NewFile.LoadDataFromDir((strFind + "\\" + FindData.cFileName).c_str()))
					++ dwLoadFileNum;
			}
		} 
		//! ��ȡ�������˳�
		while(FindNextFile(hFind, &FindData));

		FindClose(hFind);
	}

	return dwLoadFileNum;
}

//! �ݹ麯������һ���ڵ����������д��ָ���Ĵ���λ��
DWORD UpdateFolder::WriteDataToDir(LPCSTR pDirPath, tagDirectory &Directory)
{
	DWORD dwLoadFileNum = 0;
	//! ������ǰĿ¼
	string strCurrDir(pDirPath);
	strCurrDir.append("\\");
	MakeSureDirectoryPathExists(strCurrDir.c_str());

	//! ������ǰ�ļ�
	list<CUpdateFile>::iterator iteFile = Directory.listFile.begin();
	for (; Directory.listFile.end() != iteFile; ++iteFile)
	{
		if(iteFile->WriteDataToPath(strCurrDir.c_str()))
			++ dwLoadFileNum;
	}

	//! �ݹ���ô�����Ŀ¼
	list<tagDirectory>::iterator iteDir = Directory.listDir.begin();
	for (; Directory.listDir.end() != iteDir; ++iteDir)
	{
		dwLoadFileNum += WriteDataToDir((strCurrDir + iteDir->szName).c_str(), *iteDir);
	}

	return dwLoadFileNum;
}


//! ͨ����������·�����ļ������ҵ�һ���ļ�
const CUpdateFile* UpdateFolder::FindFile(LPCSTR pFilename)
{
	if(NULL != pFilename)
	{
		list<string> listPath;
		if(ParsePath(pFilename, listPath))
		{
			tagDirectory *pDirectory = &m_Root;
			list<string>::const_iterator iteLast = listPath.end();
			--iteLast;
			for (list<string>::iterator ite = listPath.begin(); listPath.end() != ite; ++ite)
			{
				if(iteLast != ite)//! ���ļ���
				{
					for (list<tagDirectory>::iterator iteDir = pDirectory->listDir.begin(); pDirectory->listDir.end() != iteDir; ++iteDir)
					{
						if(0 == stricmp(ite->c_str(), iteDir->szName))
						{
							pDirectory = &(*iteDir);
							break;
						}
					}
					return NULL;
				}
				else //! ���ļ�
				{
					for (list<CUpdateFile>::iterator iteFile = pDirectory->listFile.begin(); pDirectory->listFile.end() != iteFile; ++iteFile)
					{
						if(0 == stricmp(ite->c_str(), iteFile->GetName()))
						{
							return &(*iteFile);
						}
					}
				}
			}
		}
	}
	
	return NULL;
}


//! ����·��
BOOL UpdateFolder::ParsePath(LPCSTR pFilename, list<string> &listPath)
{
	LONG lBeginPos = 0, lEndPos = 0, lSize = strlen(pFilename);

	char szTmp[MAX_PATH] = {0};
	do 
	{
		++lEndPos;
		if('\\' == pFilename[lEndPos] || '/' == pFilename[lEndPos] || lSize == lEndPos)
		{
			memcpy(szTmp, &pFilename[lBeginPos], lEndPos - lBeginPos);
			szTmp[lEndPos - lBeginPos] = 0;
			listPath.push_back(string(szTmp));
			lBeginPos = lEndPos;
		}
	} while(lEndPos <= lSize);

	if(0 != listPath.size())
		return TRUE;
	return FALSE;
}