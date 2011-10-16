#pragma once
#include "StdAfx.h"
#include "../../../SCMPublic/def/InfoDef.h"
#include "../../../SCMPublic/class/UpdatePack.h"
#include "patch.h"
class CGapp : public tagGappBaseInfo
{
public:
	CGapp(void);
	~CGapp(void);
	BOOL	Init(TiXmlElement* p);
	
	BOOL	Start();
	BOOL	ShutDown();
	void	SetInfo(const char* strTitle);
	void	SetState(DWORD sdwstate);
	inline  DWORD	GetState()				{return _dwGappState;}
	DWORD	GetID()					{return _dwGappID;}
	DWORD	GetProcessID()			{return m_dwProcessID;}
	bool	SetProcessID();
	const	char* GetPhysName()			{return m_szPhysName;}
	const	char* GetDisplayName()		{return _szGappName;}
	void	DelayTest();
	long	SetDelayTime(DWORD dwtime);
	long	GetDelayTime();
	void	AddToByteArray(DBWriteSet& dbWriteSet);
	BOOL	InitMemServer();
	BOOL	UpdateGapp(const char* szPatchName);
	DWORD	GetType()				{return _dwGappType;}
	BOOL	SetVersion();
	void	SetMainTick(DWORD tick);
	const char* GetVersion()		{return _szGappVersion;}
	BOOL	CmpVersion(stringstream& stream);//��SCC�ϵİ汾�Ƚ�
	BOOL	GetNextVersionPatch(stringstream& stream);//��ȡ��һ���汾�ĸ��°�
	BOOL	GetFile(DBReadSet& dbreadset);//��ȡSCC���������ļ�
	char*	GetTitle()				{return m_szTitle;}
	BOOL    CheckVersion(const char* szVersion = NULL);	
	BOOL	GetPackRes();//��ȡԭʼ��
	BOOL	WritePackRes(DBReadSet& dbreadset);//��ԭʼ���ļ�д������
	BOOL	Backup();//����ԭ�еİ��ļ�
	bool	GetHardWareLoad();
	void	OnTimeOut(DWORD dwTimerID,DWORD dwcurTime,const void* pargs);
	void	OnTimerCancel(DWORD dwTdwTimeID,const void* parg);
	void	CancelLoadResTimer();
	void	CancelPingTimer();
	void	RequestAITick();//���͵�GAPP����AITick
	void	RequestFile(char* filename);
	void	CmpAITick(DWORD tick);
	bool	CheckStart();
	bool	FindGappWindow();
	bool	IsShutDownOk();
	void	LSSendRefreshInfo(CMessage* pMsg){};
	void	DBSSendRefreshInfo(CMessage* pMsg){};
	void	WSSendRefreshInfo(CMessage* pMsg){};
	void	GSSendRefreshInfo(CMessage* pMsg){};
	void	SetIsLatest(BYTE b)	{m_bLatest=b;}
	bool	SetLSPort(DWORD dwPort);
	bool	Reset();//����״̬
	bool	CheckProcessNum();
	bool	FindProcByProcID();//���������رյ�ʱ�������
	bool	EnumGappWindow(DWORD dwProcID);
private:
	DWORD			m_dwProcessID;//���̺�
	char			m_szPath[BASE_TEXT_SIZE];//·��
	char			m_szTitle[BASE_TEXT_SIZE];//��������
	char			m_szPhysName[BASE_TEXT_SIZE];//�����ļ���
	list<long>		m_listDelayTime;
	//�ڴ�ӳ���ļ�
	char			m_szMapFileName[BASE_NAME_SIZE];//Ӱ���ļ���
	char			m_szMapObjName[BASE_NAME_SIZE];//Ӱ�������
	char			m_szOutPutDir[BASE_NAME_SIZE];//�����־�ļ���
	
	//���°�
	CPatch*			m_pPatch;//���°�
	char			m_szNextVersion[BASE_NAME_SIZE];//��һ��Ҫ���µİ汾
	DWORD			m_MainAITick;
	DWORD			m_dwLoadResTimerID;
	DWORD			m_dwMainLoopTimerID;
	DWORD			m_dwShutDownTimerId;
	DWORD			m_dwPingTimerID;
	BYTE			m_bLatest;//�Ƿ���µ����°汾
	HWND			m_hHandle;//GAPP���
};

typedef struct tagWndInfo
{
	DWORD _dwProcID;
	HWND _hWnd;
}WNDINFO,*PWNDINFO;
