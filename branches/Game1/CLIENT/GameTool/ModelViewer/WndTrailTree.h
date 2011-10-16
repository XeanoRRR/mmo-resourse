#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndTrailTree: public ui::Wnd
{
	ui::TreeView  * m_pTrailTreeView;			//��ʾģ�͵����οؼ�

	char		  m_pszTrailFilePathName[128];	//Trail�ļ�·���ļ���
	char		  m_pszTrailName[128];			//��ǰ������trail�ļ�����
public:
	WndTrailTree(void);
	~WndTrailTree(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	ui::TreeView * GetModelTreeView() {return m_pTrailTreeView;}
	void CreateTrailTree(const char * trailFilePathName);
	virtual void Update();
	virtual void Render();
	void SetTrailFilePathName (const char * pathname) {strcpy(m_pszTrailFilePathName,pathname);}
	void SetCurrentTrailName(const char *name){strcpy(m_pszTrailName,name);}
	const char * GetTrailFilePathName() {return m_pszTrailFilePathName;}
	void SaveTrailFile();
};