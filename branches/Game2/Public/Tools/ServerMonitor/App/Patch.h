#pragma once
//#ifndef ZLIB_WINAPI
//#define ZLIB_WINAPI
//#endif
#include "../../../../Public/ziplib/zlib.h"
#include "../../../../Public/ziplib/unzip.h"

class CPatch
{
public:
	CPatch(void);
	~CPatch(void);
public:
	BOOL				Init(const char* filename);
private:
	CFilesInfo*			m_pPatchFilesInfo;		//�������ļ���Ϣ
	CPackage*			m_pPatchPackage;		//�����İ�����
	CClientResource*	m_pClientResource;		//�ͻ�����Դ

	string				m_strWorkPath;
	string				m_strExePath;			//ִ���ļ�Ŀ¼
	string				m_strFilesInfoName;		//�ļ���Ϣ����

	string				m_strTemptPackageName;	//��ʱ����
	string				m_strTemptFilesInfoName;//��ʱ�ļ�����

	string				m_listStrUpdatedVersion;	//�µİ汾��

	string				m_strPatchName;			//����
	DWORD				m_dwCrc32;				//��ǰ���ļ���ϢCRC32��
public:
	BOOL	GetPatchData();						//�õ�����������
	BOOL	CheckGameRunFolder();				//����Ƿ�����Ϸ����Ŀ¼
	
	void	SetCrc32(DWORD crc32)				{m_dwCrc32 = crc32;}
	string	Update(const char* filename,const char* patchname);

	bool	Release();
	static BOOL	 LoadClientResource(CPatch* pPatch);
	static BOOL  UpdateClientResource(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,BYTE*& pBuf,DWORD& dwBuflen,
		CPackage* pPackage,CClientResource* pClientResource,HWND hwnd,int& nTotalSize);

public:
};
