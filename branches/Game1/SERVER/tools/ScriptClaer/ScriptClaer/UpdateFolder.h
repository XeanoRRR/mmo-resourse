

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")

#include "UpdateFile.h"


#pragma once


class UpdateFolder
{
public:
	UpdateFolder(VOID);
	~UpdateFolder(VOID);

public:
	//!						��һ��Ŀ¼�µ��������ݶ��뵽�����У�������ǰ�����ݣ�
	BOOL					LoadDataFromDir(LPCSTR pDirPath);
	//!						�Ѱ��е�����д�뵽ָ����Ŀ¼·��
	BOOL					WriteDataToDir(LPCSTR pDirPath);

	//!						ͨ����������·�����ļ������ҵ�һ���ļ�
	const	CUpdateFile*	FindFile(LPCSTR pFilename);

protected:
	//! �����Ķ���
	/////////////////////////////////////////////////////////////////////////////////////////

	//! Ŀ¼
	struct tagDirectory
	{
		char					szName[MAX_PATH];	//�ļ�����
		list<tagDirectory>		listDir;
		list<CUpdateFile>		listFile;
		tagDirectory(VOID){memset(szName, 0, MAX_PATH);}
	};

protected:
	//! �����ĺ���
	/////////////////////////////////////////////////////////////////////////////////////////
	//!		�ݹ麯������ȡһ���ļ��е��������ݵ�����Ľڵ���
	DWORD	LoadDataFromDir(LPCSTR pDirPath, tagDirectory &Directory);
	//!		�ݹ麯������һ���ڵ����������д��ָ���Ĵ���λ��
	DWORD	WriteDataToDir(LPCSTR pDirPath, tagDirectory &Directory);

	//!		����·��
	BOOL	ParsePath(LPCSTR pFilename, list<string> &listPath);

protected:
	//!				��Ŀ¼
	tagDirectory	m_Root;

private:
	//! �����������ͷ����ݣ���˲�������󿽱�
	UpdateFolder(const UpdateFolder&);
	UpdateFolder& operator=(const UpdateFolder&);
};