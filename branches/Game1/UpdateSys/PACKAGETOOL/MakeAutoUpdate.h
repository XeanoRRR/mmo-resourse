#pragma once

//////////////////////////////////////////////////////////////////////////
//MakeAutoUpdate.h
//Fun:���������Զ����°�
//Create Time:2004.11.16
//Author:Joe
//////////////////////////////////////////////////////////////////////////

#include <string>

#include "../../PUBLIC/FilesInfo.h"
#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/ClientResource.h"

class CMakeAutoUpdate
{
public:
	CMakeAutoUpdate(string& strVersion,string& strExeFolder,DWORD dwPackType,
					CFilesInfo* LastFilesInfo,CClientResource* pClientResource);
	~CMakeAutoUpdate(void);
private:
	DWORD			m_dwPackType;						//д������ļ��İ�
	string			m_strVersion;						//���µİ汾��
	string			m_strExeFoler;						//��ǰ�ļ���
	CFilesInfo*		m_pLastFilesInfo;					//ԭʼ�ļ���Ϣ
	CClientResource* m_pOriginResource;					//��ǰ��Դ

public: 
	void	MakeAutoUpdatePackage(long lCurMode/*����ģʽ 0����������1���ͻ���*/, bool bIsSelcted/*0:�ӵ�ǰ�汾���� 1:��ѡ��汾����*/, bool bPatch=false);	//�����Զ����±� 
	void	MakePacth(long lCurMode/*����ģʽ 0����������1���ͻ���*/, CFilesInfo& DifFilesInfo);

	bool	MakePatchPackage(tagFileInfo* ptagFileInfo,CFilesInfo* pFilesInfo,CPackage* pPackage);
	bool	UpdatePatch(long lCurMode/*����ģʽ 0����������1���ͻ���*/, string& strDir,string& strPackageName,string& strFilesInfoName);
};
