

#include "stdafx.h"
#include "PackManage.h"
#include "UserManage.h"


LPSTR PackManage::c_pPackPath		= "ServerUpdatePack";
LPSTR PackManage::c_pFilePath		= "ServerUpdate";
LPSTR PackManage::c_pDiyPath		= "ServerUpdate";
LPSTR PackManage::c_pReadMeFileName = "_ReadMe.txt";

PackManage	PackManage::c_PackManage;


PackManage::PackManage(VOID)
{

}
PackManage::~PackManage(VOID)
{

}

//! ��ʼ������
BOOL PackManage::Init(VOID)
{
	m_mapServerName[eGT_AS]		= "AccountServer";
	m_mapServerName[eGT_LOGS]	= "LogServer";
	m_mapServerName[eGT_BS] 	= "BillingServer";
	m_mapServerName[eGT_LS] 	= "LoginServer";
	m_mapServerName[eGT_DB] 	= "DBServer";
	m_mapServerName[eGT_WS] 	= "WorldServer";
	m_mapServerName[eGT_GS] 	= "GameServer";

	//! ��������Ŀ¼
	char szDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szDir);

	map<DWORD, string>::iterator ite = m_mapServerName.begin();
	for (; m_mapServerName.end() != ite; ++ite)
	{
		char szTmpDir[MAX_PATH] = {0};
		sprintf(szTmpDir, "%s\\%s\\%s\\", szDir, c_pDiyPath, ite->second.c_str());
		MakeSureDirectoryPathExists(szTmpDir);
		sprintf(szTmpDir, "%s\\%s\\%s\\", szDir, c_pPackPath, ite->second.c_str());
		MakeSureDirectoryPathExists(szTmpDir);
	}

	//! ��ȡ���°���Ϣ
	LoadAllPack();
	LoadAllFileInfo();
	return TRUE;
}
//! �ͷŶ���
VOID PackManage::Release(VOID)
{
	map<DWORD, pair<string, UpdatePack*>>::iterator ite = m_mapCachePack.begin();
	for (; m_mapCachePack.end() != ite; ++ite)
	{
		if(NULL != ite->second.second)
		{
			ite->second.second->~UpdatePack();
			M_FREE(ite->second.second, sizeof(UpdatePack));
		}
	}
	m_mapCachePack.clear();

	map<string, CUpdateFile*>::iterator iteFile = m_mapSmDiyFile.begin();
	for (; m_mapSmDiyFile.end() != iteFile; ++iteFile)
	{
		SAFE_DELETE(iteFile->second);
	}
	m_mapSmDiyFile.clear();
}

//! ��Ӧ�û��ϴ����°�
VOID PackManage::OnUserUploadData(DBReadSet &setReadDB, LONG lUserSocket)
{
	UpdatePack *pPack = (UpdatePack*)M_ALLOC(sizeof(UpdatePack));
	if(NULL == pPack)
		return;
	new(pPack)UpdatePack();

	pPack->DeCode(setReadDB);

	char szDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szDir);
	strcat(szDir, "\\");

	char szSubDir[MAX_PATH] = {0};
	if(GetPackSubDir(pPack->GetGappType(), pPack->GetVersion(), szSubDir))
	{	
		CMessage msg(MSG_SCC2SUC_CTRL_Re_UpLoadPack);
        Json::Value var;

		//! �����°�˵����д��
		char szReadMePath[MAX_PATH] = {0};
		LPCSTR pServerName = m_mapServerName[pPack->GetGappType()].c_str();
		sprintf(szReadMePath, "%s%s\\%s\\", szDir, c_pPackPath, pServerName);
		MakeSureDirectoryPathExists(szReadMePath);
		sprintf(szReadMePath, "%s%s\\%s\\%s%s", szDir, c_pPackPath, pServerName, pPack->GetVersion(), c_pReadMeFileName);
		
		BOOL bSendNotify = FALSE;
		if(OverloadFile(szReadMePath, pPack->GetReadMe()))
		{
			//! �����°�д���ļ�
			strcat(szDir, szSubDir);
			if(pPack->WriteDataToDir(szDir))
			{
			//	msg.Add((DWORD)TRUE);
                var["result"] = true;
				bSendNotify = TRUE;
			}
			else
            {
			//	msg.Add((DWORD)FALSE);
                var["result"] = false;
            }
		}
		else
        {
		//	msg.Add((DWORD)FALSE);
            var["result"] = false;
        }
		
	//	msg.Add(pPack->GetGappType());
	//	msg.Add(pPack->GetVersion());
        var["gappType"] = (int)pPack->GetGappType();
        var["version"] = pPack->GetVersion();
        msg.AddJson(var);
		msg.SendToSuc(lUserSocket);

		if(bSendNotify)
		{
			LPCSTR pUserName = UserManage::GetInstance().FindUserName(lUserSocket).c_str();
			ActionLogManage::GetInstance().ActionLog(-1, -1, pUserName, "user[%s] upload pack[%s]��version[%d]", pUserName, pServerName);
			msg.SetType(MSG_SCC2SUC_INFO_Notify_PackList);
			UserManage::GetInstance().SendToAll(ePT_Update, msg);
		}
	}

	//! ���»���
	if(0 > strcmp(m_mapCachePack[pPack->GetGappType()].first.c_str(), pPack->GetVersion()))
	{
		if(NULL != m_mapCachePack[pPack->GetGappType()].second)
		{
			m_mapCachePack[pPack->GetGappType()].second->~UpdatePack();
			M_FREE(m_mapCachePack[pPack->GetGappType()].second, sizeof(UpdatePack));

			m_mapCachePack[pPack->GetGappType()].first = pPack->GetVersion();
			m_mapCachePack[pPack->GetGappType()].second = pPack;
		}
	}
	else
	{
		pPack->~UpdatePack();
		M_FREE(pPack, sizeof(UpdatePack));
	}
}

//! ��Ӧ�û��ϴ��ļ�
VOID PackManage::OnUserUploadFile(DBReadSet &setReadDB, LONG lUserSocket)
{
	DWORD dwGappType = setReadDB.GetDwordFromByteArray();
	CUpdateFile UpdateFile;
	UpdateFile.DeCode(setReadDB);

	char szDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szDir);
	strcat(szDir, "\\");
	strcat(szDir, c_pDiyPath);	

	CMessage msg(MSG_SCC2SUC_CTRL_Re_UploadFile);
    Json::Value var;

	map<DWORD, string>::iterator ite = m_mapServerName.find(dwGappType);
	if(m_mapServerName.end() != ite)
	{
		strcat(szDir, "\\");
		strcat(szDir, ite->second.c_str());
		UpdateFile.WriteDataToPath(szDir);

		strcat(szDir, "\\");
		strcat(szDir, UpdateFile.GetName());
		map<string, CUpdateFile*>::iterator iteFile = m_mapSmDiyFile.find(szDir);
		if(m_mapSmDiyFile.end() != iteFile)
		{
			SAFE_DELETE(iteFile->second);
			m_mapSmDiyFile.erase(iteFile);
		}

		map<DWORD, set<string>>::iterator ite = m_mapAllFileInfo.find(dwGappType);
		if(m_mapAllFileInfo.end() != ite)
		{
			ite->second.insert(UpdateFile.GetName());
		}
		else
			LogTools::OutErrLog("can't find GappType[%d]'s packfile in PackManage::OnUserUploadFile!!", dwGappType);
		
	//	msg.Add((DWORD)TRUE);
        var["result"] = true;
	}
	else
    {
	//	msg.Add((DWORD)FALSE);
        var["result"] = false;
    }

//	msg.Add(dwGappType);
//	msg.Add(UpdateFile.GetName());
    var["gappType"] = (int)dwGappType;
    var["updateFileName"] = UpdateFile.GetName();
    msg.AddJson(var);
	msg.SendToSuc(lUserSocket);
}

//! �������и��°���Ϣ���ͻ���
VOID PackManage::SendPackInfoListToClient(LONG lUserSocket)
{
	CMessage msg(MSG_SCC2SUC_INFO_Notify_PackList);
    Json::Value var;    
	//DBWriteSet setWriteDB;
	//msg.GetDBWriteSet(setWriteDB);

	////! ���°�����������
	//setWriteDB.AddToByteArray((LONG)m_mapAllPack.size());

	//for (map<DWORD, map<string, string>>::iterator iteType = m_mapAllPack.begin(); m_mapAllPack.end() != iteType; ++iteType)
	//{
	//	//! GAPP����
	//	setWriteDB.AddToByteArray(iteType->first);
	//	//! ��ǰ���͵ĸ��°�����
	//	setWriteDB.AddToByteArray((LONG)iteType->second.size());
	//	for (map<string, string>::iterator iteVersion = iteType->second.begin(); iteType->second.end() != iteVersion; ++iteVersion)
	//	{
	//		//! �汾��
	//		setWriteDB.AddToByteArray(iteVersion->first.c_str());
	//		//! ˵������
	//		setWriteDB.AddToByteArray(iteVersion->second.c_str());
	//	}
	//}

    map<DWORD, map<string, string>>::iterator iteType = m_mapAllPack.begin();
    for(; iteType != m_mapAllPack.end(); ++iteType)
    {
        Json::Value subVar;
        Json::Value tmpVar;
        tmpVar["gappType"] = (int)iteType->first;
        map<string, string>::iterator iteVersion = iteType->second.begin();
        for(; iteVersion != iteType->second.end(); ++iteVersion)
        {
            Json::Value subVarEx;
            subVarEx["version"] = iteVersion->first;
            subVarEx["remark"] = iteVersion->second;
            subVar.append(subVarEx);
        }
        subVar["arrayPack"] = tmpVar;
        var.append(subVar);
    }
    msg.AddJson(var);
	msg.SendToSuc(lUserSocket);
	//UserManage::GetInstance().SendToAll(ePT_Update, msg);
}

//! ����һ������GAPPP�ĸ����ļ���Ϣ���ͻ���
VOID PackManage::SendFileInfoListToClient(DWORD dwGappID, LONG lUserSocket)
{
	CMessage msg(MSG_SCC2SUC_INFO_Re_UpdateFile);
	/*DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);

	map<DWORD, set<string>>::iterator ite = m_mapAllFileInfo.find(dwGappID);
	if(m_mapAllFileInfo.end() != ite)
	{
		setWriteDB.AddToByteArray((BYTE)TRUE);
		setWriteDB.AddToByteArray((LONG)ite->second.size());
		for (set<string>::iterator iteFile = ite->second.begin(); ite->second.end() != iteFile ; ++iteFile)
		{
			setWriteDB.AddToByteArray((*iteFile).c_str());
		}
	}
	else
	{
		setWriteDB.AddToByteArray((BYTE)FALSE);
	}*/

    Json::Value var;
    map<DWORD, set<string>>::iterator ite = m_mapAllFileInfo.find(dwGappID);
    if(m_mapAllFileInfo.end() != ite)
    {
        var["result"] = true;
        Json::Value fileVar;
        for (set<string>::iterator iteFile = ite->second.begin(); ite->second.end() != iteFile ; ++iteFile)
        {
            Json::Value tmpVar;
            tmpVar["file"] = (*iteFile);
            fileVar.append(tmpVar);
        }
        var["fileInfo"] = fileVar;
    }
    else
    {
        var["result"] = false;
    }
    msg.AddJson(var);
	msg.SendToSuc(lUserSocket);
}

//! ����һ��GAPP������Ϣ
BOOL PackManage::GetUpdateGapp(DWORD dwGappType, LPCSTR pVersion, DBWriteSet &setWriteDB)
{
	//! ȷ���д˰汾���°�
	map<DWORD, map<string, string>>::iterator iteInfo = m_mapAllPack.find(dwGappType);
	if (m_mapAllPack.end() == iteInfo)
		return FALSE;
	
	map<string, string>::iterator iteVer = iteInfo->second.find(pVersion);
	if(iteInfo->second.end() == iteVer)
		return FALSE;
	

	//! ȡ�ø��°�����
	UpdatePack *pPack = NULL;
	BOOL bDelPack = FALSE;
	BOOL bRe = FALSE;

	map<DWORD, pair<string, UpdatePack*>>::iterator itePack = m_mapCachePack.find(dwGappType);
	if(m_mapCachePack.end() != itePack)
	{
		if(0 == strcmp(pVersion, itePack->second.first.c_str()))
			pPack = itePack->second.second;

		if(NULL == pPack)
		{
			char szPath[MAX_PATH] = {0};
			if(GetPackSubDir(dwGappType, pVersion, szPath))
			{
				pPack = (UpdatePack*)M_ALLOC(sizeof(UpdatePack));
				new(pPack)UpdatePack();
				if(NULL != pPack)
				{
					bDelPack = TRUE;
					if(pPack->LoadDataFromDir(szPath))
					{
						pPack->Code(setWriteDB);
						bRe = TRUE;
					}
				}
			}
		}
		else
		{
			pPack->Code(setWriteDB);
			bRe = TRUE;
		}

		if(bDelPack)
		{
			pPack->~UpdatePack();
			M_FREE(pPack, sizeof(UpdatePack));
		}
	}

	return bRe;
}

//! ȷ��һ��GAPP�İ汾�ǲ�����ߵ�
BOOL PackManage::IsHigher(DWORD dwGappType, LPCSTR pVersion)
{
	map<DWORD, map<string, string>>::iterator iteInfo = m_mapAllPack.find(dwGappType);
	if (m_mapAllPack.end() == iteInfo)
		return FALSE;

	map<string, string>::iterator iteVer = iteInfo->second.end();
	if(iteInfo->second.end() == iteInfo->second.begin())
		return TRUE;

	--iteVer;
	
	return (0 > strcmp(pVersion, iteVer->first.c_str()));
}

//! �ҵ�һ��GAPP�����¸��°���ѹ�����ݼ�
BOOL PackManage::FindAndGetHighestPack(DWORD dwGappType, DBWriteSet &setWriteDB)
{
	map<DWORD, map<string, string>>::iterator iteInfo = m_mapAllPack.find(dwGappType);
	if (m_mapAllPack.end() == iteInfo)
	{
		setWriteDB.AddToByteArray((BYTE)TRUE);
		return FALSE;
	}

	map<string, string>::iterator iteVer = iteInfo->second.end();
	if(iteInfo->second.end() == iteInfo->second.begin())
	{
		setWriteDB.AddToByteArray((BYTE)TRUE);
		return FALSE;
	}

	--iteVer;

	//! ȡ�ø��°�����
	UpdatePack *pPack = NULL;
	BOOL bDelPack = FALSE;
	BOOL bRe = FALSE;

	map<DWORD, pair<string, UpdatePack*>>::iterator itePack = m_mapCachePack.find(dwGappType);
	if(m_mapCachePack.end() != itePack)
	{
		if(iteVer->first == itePack->second.first)
			pPack = itePack->second.second;

		if(NULL == pPack)
		{
			char szPath[MAX_PATH] = {0};
			if(GetPackSubDir(dwGappType, iteVer->first.c_str(), szPath))
			{
				pPack = (UpdatePack*)M_ALLOC(sizeof(UpdatePack));
				new(pPack)UpdatePack();
				if(NULL != pPack)
				{
					bDelPack = TRUE;
					if(pPack->LoadDataFromDir(szPath))
					{
						setWriteDB.AddToByteArray((BYTE)TRUE);
						pPack->Code(setWriteDB);
						bRe = TRUE;
					}
				}
			}
		}
		else
		{
			setWriteDB.AddToByteArray((BYTE)TRUE);
			pPack->Code(setWriteDB);
			bRe = TRUE;
		}

		if(bDelPack)
		{
			pPack->~UpdatePack();
			M_FREE(pPack, sizeof(UpdatePack));
		}
	}

	if(!bRe)
		setWriteDB.AddToByteArray((BYTE)TRUE);
	return bRe;
}

//! ��һ������Ӧ��Ŀ¼��ӵ������Ŀ¼����
BOOL PackManage::GetPackSubDir(DWORD dwServerType, LPCSTR pServerVersion, char *pOutPath)
{
	if(NULL != pOutPath)
	{
		map<DWORD, string>::iterator ite = m_mapServerName.find(dwServerType);
		if(m_mapServerName.end() != ite)
		{
			sprintf(pOutPath, "%s\\%s\\%s", c_pPackPath, ite->second.c_str(), pServerVersion);
			return TRUE;
		}
	}

	return FALSE;
}

//! �õ�һ��ָ��·�����ļ�
const CUpdateFile* PackManage::GetFile(DWORD dwGappType, LPCSTR pPathAndName)
{
	if(NULL != pPathAndName)
	{
		char szDir[MAX_PATH] = {0};
		::GetCurrentDirectory(MAX_PATH, szDir);
		strcat(szDir, "\\");
		strcat(szDir, c_pDiyPath);	

		map<DWORD, string>::iterator ite = m_mapServerName.find(dwGappType);
		if(m_mapServerName.end() != ite)
		{
			strcat(szDir, "\\");
			strcat(szDir, ite->second.c_str());
			strcat(szDir, "\\");
			strcat(szDir, pPathAndName);

			map<string, CUpdateFile*>::const_iterator iteFile = m_mapSmDiyFile.find(szDir);
			if(m_mapSmDiyFile.end() != iteFile)
				return iteFile->second;

			//! �õ�������·�����ļ���
			LONG lBegin = 0, lSize = (LONG)strlen(pPathAndName);
			for (LONG i = 0; i < lSize; ++i)
			{
				if('\\' == pPathAndName[i] || '/' == pPathAndName[i])
					lBegin = i;
			}
			if(lSize - 1 <= lBegin)
				return NULL;
			const char *pFileName = &(pPathAndName[lBegin]);

			//! ��ȡ�ļ�
			CUpdateFile* pCUpdateFile = new CUpdateFile(pFileName);
			if(pCUpdateFile->LoadDataFromDir(szDir))
			{
				m_mapSmDiyFile[string(szDir)] = pCUpdateFile;
				return pCUpdateFile;
			}
			else
				SAFE_DELETE(pCUpdateFile);
		}
	}

	return NULL;
}

//! ˢ�»����ļ�
VOID PackManage::RefreshFileCache(VOID)
{
	Release();
	//! ��ȡ���°���Ϣ
	LoadAllPack();
	LoadAllFileInfo();
}

//! ��ȡ���и��°���Ϣ��
VOID PackManage::LoadAllPack(VOID)
{
	char szDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szDir);
	strcat(szDir, "\\");
	strcat(szDir, c_pPackPath);

	m_mapAllPack.clear();
	for (map<DWORD, string>::iterator ite = m_mapServerName.begin(); m_mapServerName.end() != ite; ++ite)
	{
		//! ������Ϣ����
		m_mapAllPack.insert(pair<DWORD, map<string, string>>(ite->first, map<string, string>()));
		map<string, string> &mapNode = m_mapAllPack[ite->first];
		//! �����������ݿռ�
		m_mapCachePack.insert(pair<DWORD, pair<string, UpdatePack*>>(ite->first, pair<string, UpdatePack*>(string(), NULL)));
		
		string strFind(szDir);
		WIN32_FIND_DATA FindData;
		HANDLE hFind = ::FindFirstFile((strFind + "\\" + ite->second + "\\*").c_str(), &FindData);
		if(INVALID_HANDLE_VALUE != hFind)
		{
			string strMaxVersion;
			do 
			{
				if((FILE_ATTRIBUTE_DIRECTORY == FindData.dwFileAttributes))
				{
					if(0 != strcmp(FindData.cFileName, ".") && 0 != strcmp(FindData.cFileName, ".."))
					{	
						try
						{
							if(0 == strMaxVersion.size() || (0 > strcmp(strMaxVersion.c_str(), FindData.cFileName)))
								strMaxVersion = FindData.cFileName;

							//! ��˵���ļ�
							fstream stream;
							string strFileName = strFind + "\\" + ite->second + "\\" + FindData.cFileName + c_pReadMeFileName;
							stream.open(strFileName.c_str());
							if( stream.is_open() )
							{
								string strReadMe;
								stream >> strReadMe;

								//! ������Ϣ
								mapNode[FindData.cFileName] = strReadMe;
							}
							else
								mapNode[FindData.cFileName] = string("");
							

						}
						catch (...)
						{
							//! ��������
							continue;
						}
					}
				}
			} 
			//! ��ȡ�������˳�
			while(FindNextFile(hFind, &FindData));

			FindClose(hFind);

			//! ȡ���汾��ߵİ���Ϊ��������
			if(0 != strMaxVersion.size())
			{
				UpdatePack *pPack = (UpdatePack*)M_ALLOC(sizeof(UpdatePack));
				if(NULL == pPack)
					continue;
				new(pPack)UpdatePack();

				if(pPack->LoadDataFromDir((strFind + "\\" + ite->second + "\\" + strMaxVersion).c_str()))
				{
					m_mapCachePack[ite->first].first = strMaxVersion;
					m_mapCachePack[ite->first].second = pPack;
				}
				else
				{
					pPack->~UpdatePack();
					M_FREE(pPack, sizeof(UpdatePack));
				}	
			}
		}
	}
}


//! ��ȡ���и����ļ�����Ϣ
VOID PackManage::LoadAllFileInfo(VOID)
{
	char szDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szDir);
	strcat(szDir, "\\");
	strcat(szDir, c_pFilePath);

	m_mapAllFileInfo.clear();
	for (map<DWORD, string>::iterator ite = m_mapServerName.begin(); m_mapServerName.end() != ite; ++ite)
	{
		//! ������Ϣ����
		m_mapAllFileInfo.insert(pair<DWORD, set<string>>(ite->first, set<string>()));
		set<string> &mapNode = m_mapAllFileInfo[ite->first];

		string strFind(szDir);
		WIN32_FIND_DATA FindData;
		HANDLE hFind = ::FindFirstFile((strFind + "\\" + ite->second + "\\*").c_str(), &FindData);
		if(INVALID_HANDLE_VALUE != hFind)
		{
			do 
			{
				if((FILE_ATTRIBUTE_DIRECTORY != FindData.dwFileAttributes))
				{
					if(0 != strcmp(FindData.cFileName, ".") && 0 != strcmp(FindData.cFileName, ".."))
					{	
						mapNode.insert(FindData.cFileName);
					}
				}
			} 
			//! ��ȡ�������˳�
			while(FindNextFile(hFind, &FindData));

			FindClose(hFind);
		}
	}
}