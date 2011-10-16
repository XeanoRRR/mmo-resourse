// PackageToolDoc.h :  CPackageToolDoc ��Ľӿ�
//

#include "../../PUBLIC/FilesInfo.h"
#include "../../PUBLIC/Package.h"
#include "../../PUBLIC/ClientResource.h"
#pragma once

struct tagStaDisplay
{
	string strOp;
	string strFileName;
	tagStaDisplay()
	:strOp("")
	,strFileName("")
	{
	}
};

#define OPEN_FOLDER WM_USER+1					//���ļ���ʱ����
#define FILE_RERESH WM_USER+2					//�ļ�����ˢ����
#define INSERT_ITEM WM_USER+3					//�����ļ���


#define STATUS_UPDATE WM_USER+4					//����״̬����ʾ

class CPackageToolDoc : public CDocument
{
protected: // �������л�����
	CPackageToolDoc();
	DECLARE_DYNCREATE(CPackageToolDoc)

private:
	string		m_strCurFolder;						//��ǰ������Դ�ļ���
	string		m_strPackageName;					//����
	string		m_strFileInfoName;					//�ͻ�����Դ��Ϣ��
	string		m_strDefaultProName;				//ȱʡ��Ŀ����
	

	Packages		m_PackageConfigInfos;		//��������Ϣ
	PackTypeInfos	m_PackageInfos;					//���õİ���Ϣ
	CompTypeInfos	m_CompressInfos;				//���õ�ѹ����Ϣ

	string		m_strGameVersion;					//��Ϸ��ǰ�汾��

	string		m_strUpdateFileName;				//������Ϣ�ļ���
	CFilesInfo*	m_pUpdateInfo;						//���һ�β�������Դ��Ϣ�ļ�
	
	CClientResource*	m_pOriginResource;			//�ͻ���ԭʼ��Դ
	CClientResource*	m_pClientResource;			//�����Ŀͻ�����ԴĿ¼

	string m_strExWorkPath;							//��ǰִ���ļ��Ĺ���·��

	tagStaDisplay	m_StaDisplay;
	int				m_nDisplayOriInfo;				//0:�� 1:ԭʼ��Դ��Ϣ 2:��Ϸ��Դ��Ϣ 3:�����ļ���Ϣ
	CFilesInfo		*m_pOpenOtherFilesInfo;			//�򿪵������ļ���Ϣ
	string			m_strOtherFileInfoName;

	long			m_lCurPatchMode;				// 0:������ģʽ 1;�ͻ���ģʽ(Ĭ��)
	CPackage*		m_pPatchPackage;
	// ����
public:
	long			GetCurPatchMode(void) { return m_lCurPatchMode; }
	void			SetCurPatchMode(long lMode) { m_lCurPatchMode = lMode; }
	Packages&		GetPackConfigInfos()			{return m_PackageConfigInfos;}

	CPackage*		GetPatchPackage(void) { return m_pPatchPackage; }
	PackTypeInfos&	GetPackageInfos()				{return m_PackageInfos;}
	CompTypeInfos&	GetCompressInfos()				{return m_CompressInfos;}

	const string&	GetPackFormatSring(DWORD dwPackID);
	const string&	GetCompFormatSring(DWORD dwCompID);

	int	GetDisplayOriInfo()				{return m_nDisplayOriInfo;}

	void	SetStatStatuOp(string& strOp);
	void	SetStatStatuFileName(string& strFileName);

	bool	SetGameVersion(string& strGameVersion);

	void	LoadVersion();
	void	SaveVersion();																//����汾��
	void	LoadCurFileInfoUpdateInf(string strUpdateFileNam="");
	void	LoadPackageInfos();															//װ�ذ���Ϣ
	void	LoadCompressInfos();														//װ��ѹ����Ϣ
	void	SaveCurFileInfoUpdateInfo(CString& strFileName);							//���浱ǰ��Ϣ��Ϊ������Ϣ����

// ����
public:
	string GetCurFolder();

	CClientResource* GetClientResource()	{return m_pOriginResource;}


	VOID	UpdateFileInfoToOpFilZesInfo(string& strFileName,DWORD dwCrc32,DWORD dwSize,bool bFolder,
							DWORD dwPackageType=0,DWORD dwCompressType =0);				//����һ���ļ���Ϣ
	BOOL	GetFileInfoFromOpFilesInfo(string& strFileName,DWORD& dwSize,DWORD& dwCrc32,
						DWORD& dwPackageType,DWORD& dwCompressType,bool& bFolder);		//�õ�һ���ļ�����Ϣ

	//����������仯ʱ�򣬸����ļ���Ϣ
	VOID	UpdateFileInfoToOpFilesWithChild(string& strFileName,DWORD dwPackageType,DWORD dwCompressType,
											bool bForcePack = false,bool bForceCompress = false);

	void	 OnOpenfolder(CString strFolder);
	void	 ReLoadFolder(string& strFolder,bool bWithChild = true);
	void	 OnOpenGameResouce(CString strFolder);
	void	 OnOpenPatchResouce(CString strFolder); // ��patch��Դ�ļ�

	void	MakePakcage();																//���ݵ�ǰ�������ļ���Ϣ���д��
	void	ReMakePakcage();															//��������� 
	void	MakePakcageEnd();

	void	OnAddFolder(string& strPos,string& strFolder);
	void	OnAddFile(string& strPos,string& strFileName);
	void	OnDelFileFolder(string& strPos,string& strFolderFileName);
	void	OnExtrFileFolder(string& strPos,string& strFolderFileName);

	void	OpenDefaultProject();
	void	SaveProject(string& strName);
	void	UpdateClientResourToView();
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CPackageToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenLastProject();
	afx_msg void OnSaveCurrProject();
	virtual void OnCloseDocument();
	afx_msg void OnFileSaveresinfotoupdate();
	afx_msg void OnMakeautoupdatebydefault();
	afx_msg void OnMakeautoupdatebyselect();
	//afx_msg void OnFileOpenclientinfo();
	afx_msg void OnFileOpenorigininfo();
	afx_msg void OnMakedefaultpatch();
	afx_msg void OnMakeselectpatch();
	afx_msg void OnMakeautoupdatebydefaultandpatch();
	afx_msg void OnMakeautoupdatebyselectandpatch();
	afx_msg void OnFileOpenOtherFilesinfo();
	afx_msg void OnGeneratemd5code();
	afx_msg void OnSetupclientmode();
	afx_msg void OnSetupservermode();
	afx_msg void OnMakeserverpatch();
	afx_msg void OnMakeselectserverpatch();
};

//void PutDebugString(const char *msg);
