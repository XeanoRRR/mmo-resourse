// urlfile.h : main header file for the URLFILE application
//

#if !defined(AFX_URLFILE_H__BB9CC6D1_27C9_11D3_ACCC_00104B247756__INCLUDED_)
#define AFX_URLFILE_H__BB9CC6D1_27C9_11D3_ACCC_00104B247756__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../PUBLIC/ClientResource.h"

#include <vector>
#include <set>
#include <d3d9.h>
#include "Winreg.h"
using namespace std;

struct tagDisplayInfo
{
	CString	strText;		//�ı���ʾֵ
	int		nMax;			//���������ֵ
	int		nCur;			//��������ǰֵ
};

/////////////////////////////////////////////////////////////////////////////
// CUrlFileApp:
// See urlfile.cpp for the implementation of this class
//
struct tagSetup
{
	bool	bWindow;
	long	lWidth;
	long	lHeight;
	long	lBitsPerPel;
	long	lFrequency;
	long	lTexturemode;
	long	lLight;
	long	lModel;
	long	lShadow;
	long	lTexture;
	long	lWeather;
	long	lControl;
	long	lViewRange;
	bool	bSound;
	bool	bMusic;
	float	fSoundVolume;
	float	fMusicVolume;
};
struct tagSetupEx
{

	BOOL	bFullscreen;
	long	lWidth;
	long	lHeight;
	long	lBitsPerPel;
	long	lFrequency;
	long	lTexturemode;
	bool	b2DLinear;		//�Ƿ���2D�����������ģʽ
	bool	b3DLinear;		//�Ƿ���3D�����������ģʽ
	long	lLight;
	long	lModel;
	long	lShadow;
	long	lTexture;
	bool	bRoleFlash;			//�Ƿ�����ɫ��������Ч��
	bool	bRoleShake;			//�Ƿ�����ɫ������Ч��
	bool	bShock;				//�Ƿ���ĳЩ�����ͷ�ʱ,��Ļ��Ч��
	long	lWeather;
	long	lControl;
	long	lViewRange;
	bool	bSound;
	bool	bMusic;
	bool    bSpeech;
	float	fSoundVolume;
	float	fMusicVolume;
	float   fSpeechVolume;
	bool    bOthersFootstep;
	long    lAntiAliasing;
	bool    bUseVSync;
	long    lFrameControl;
	long    lsimplemodelnum;
	bool    bupdateonlostfoucs;
};

struct DisplayMode
{
	int		nWidth;
	int		nHeight;

	set<int>	vColors;
	set<int>	vRefresh;
};

typedef	vector<DisplayMode>	VECTORDISPLAYMODE;

class CUrlFileApp : public CWinApp
{
public:
	CUrlFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUrlFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	string					m_strWorkPath;									//����·��
	string					m_strPackageName;								//����
	string					m_strFilesInfoName;								//�ļ���Ϣ����
	//CClientResource*		m_pClientResource;								//�ͻ�����Դ��Ϣ
	//HINTERNET				m_hUpdateServerConnect;							//�͸��·�����������

private:
	VECTORDISPLAYMODE	g_mDisplayMode;

	void LoadSetup();
	void SaveSetup();

	bool	IsNeeded( D3DDISPLAYMODE & mode );
	void	InsertMode( D3DDISPLAYMODE & mode, int n );
	void	CheckDisplayMode();
	void    SetDisplayMode();

	tagSetupEx m_tagSetup;

public:
	bool	InitResource();													//��ʼ���ͻ�����Դ��Ϣ
	bool    WriteReg(string strPath);                                                      //ÿ������ʱ��ע���д��·��

	VECTORDISPLAYMODE& GetDisplayMode()	{return g_mDisplayMode;	}
};


void PutDebugString(const char*);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLFILE_H__BB9CC6D1_27C9_11D3_ACCC_00104B247756__INCLUDED_)
