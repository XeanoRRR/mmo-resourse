#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndModelTree: public ui::Wnd
{
	ui::TreeView  * m_pModelTreeView;			//��ʾģ�͵����οؼ�

	char		  m_pszModelFilePathName[128];	//ģ��·���ļ���
	char		  m_pszModelName[128];		//��ǰ������model����
public:
	WndModelTree(void);
	~WndModelTree(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	ui::TreeView * GetModelTreeView() {return m_pModelTreeView;}
	void CreateModelTree(const char * ModelFilePathName);
	virtual void Update();
	virtual void Render();
	void SetModelFilePathName (const char * pathname) {strcpy(m_pszModelFilePathName,pathname);}
	void SetCurrentModelName(const char *name){strcpy(m_pszModelName,name);}
	const char * GetModelFilePathName() {return m_pszModelFilePathName;}
	const char * GetCurModelName()		{return m_pszModelName;}
	void SaveModelFile();
};