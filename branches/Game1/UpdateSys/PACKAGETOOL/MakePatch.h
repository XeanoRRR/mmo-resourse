#pragma once

//////////////////////////////////////////////////////////////////////////
//MakePatch.h
//Fun:������������������
//Create Time:2004.12.09
//Author:Joe
//////////////////////////////////////////////////////////////////////////

#include <string>

#include "../../PUBLIC/FilesInfo.h"
#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/ClientResource.h"

class CMakePatch
{
public:
	CMakePatch(string& strVersion,string& strExeFolder,DWORD dwPackType,
				CFilesInfo* LastFilesInfo,CClientResource* pClientResource);
	~CMakePatch(void);
private:
	DWORD			m_dwPackType;						//д������ļ��İ�
	string			m_strVersion;						//���µİ汾��
	string			m_strExeFoler;						//��ǰ�ļ���
	CFilesInfo*		m_pLastFilesInfo;					//ԭʼ�ļ���Ϣ
	CClientResource* m_pOriginResource;					//��ǰ��Դ

public: 
	void	MakePatch(bool bSelect);								//��������������
	bool	MakePatchPackage(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,CPackage* pPackage);
	bool	UpdatePatch(string& strDir,string& strPackageName,string& strFilesInfoName);

	bool	UpdateAutoUpdateFolder(CFilesInfo& DifFilesInfo,CFilesInfo* pSouFilesInfo);	//�����Զ����µ��ļ�������ļ�
};
