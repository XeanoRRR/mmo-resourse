

#include "../../../../SCMPublic/class/UpdatePack.h"


#pragma once


class PackManage
{
public:
	PackManage(VOID);
	~PackManage(VOID);
public:
	//!
	static		PackManage&					GetInstance(VOID) {return c_PackManage;}

public:
	//!										��ʼ������
	BOOL									Init(VOID);
	//!										�ͷŶ���
	VOID									Release(VOID);

public:
	//!										��Ӧ�û��ϴ����°�
	VOID									OnUserUploadData(DBReadSet &setReadDB, LONG lUserSocket);
	//!										��Ӧ�û��ϴ��ļ�
	VOID									OnUserUploadFile(DBReadSet &setReadDB, LONG lUserSocket);

	//!										�������и��°���Ϣ���ͻ���
	VOID									SendPackInfoListToClient(LONG lUserSocket);
	//!										����һ������GAPPP�ĸ����ļ���Ϣ���ͻ���
	VOID									SendFileInfoListToClient(DWORD dwGappID, LONG lUserSocket);

	//!										��һ��GAPP��������ѹ�����ݼ�
	BOOL									GetUpdateGapp(DWORD dwGappType, LPCSTR pVersion, DBWriteSet &setWriteDB);
	//!										ȷ��һ��GAPP�İ汾�ǲ�����ߵ�
	BOOL									IsHigher(DWORD dwGappType, LPCSTR pVersion);
	//!										�ҵ�һ��GAPP�����¸��°���ѹ�����ݼ�
	BOOL									FindAndGetHighestPack(DWORD dwGappType, DBWriteSet &setWriteDB);

public:
	//!										�õ�һ��ָ��·�����ļ�
	const		CUpdateFile*				GetFile(DWORD dwGappType, LPCSTR pPathAndName);
	//!										ˢ�»����ļ�
	VOID									RefreshFileCache(VOID);

private:
	//!										��ȡ���и��°�����Ϣ
	VOID									LoadAllPack(VOID);
	//!										��ȡ���и����ļ�����Ϣ
	VOID									LoadAllFileInfo(VOID);
	//!										��һ������Ӧ����Ŀ¼��ӵ������ָ����
	BOOL									GetPackSubDir(DWORD dwServerType, LPCSTR pServerVersion, char *pOutPath);

private:
	//!										��ǰ��������������з����������и��°�
	map<DWORD, map<string, string>>			m_mapAllPack;
	//!										��ǰ��������������и����ļ�
	map<DWORD, set<string>>					m_mapAllFileInfo;
	//!										ö��ֵ�ͷ��������ֶ�Ӧ��
	map<DWORD, string>						m_mapServerName;
	//!										��ÿ��GAPP����߰汾�������ڴ���
	map<DWORD, pair<string, UpdatePack*>>	m_mapCachePack;
	//!										�ṩ��SM�������ļ��Ķ�ȡ����
	map<string, CUpdateFile*>				m_mapSmDiyFile;

private:
	static		PackManage					c_PackManage;
	static		LPSTR						c_pPackPath;
	static		LPSTR						c_pFilePath;
	static		LPSTR						c_pDiyPath;
	static		LPSTR						c_pReadMeFileName;
};