#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndPartTree: public ui::Wnd
{
	ui::TreeView  * m_pPartTreeView;			//��ʾģ�͵����οؼ�

	char		  m_pszPartFilePathName[128];	//ģ��·���ļ���
	ui::TreeView::Node *m_pNode;
public:
	WndPartTree(void);
	~WndPartTree(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	ui::TreeView * GetPartTreeView() {return m_pPartTreeView;}
	void CreatePartTree(const char * strPartFilePathName);
	virtual void Update();
	virtual void Render();

	void SetPartFilePathName (const char * pathname) {strcpy(m_pszPartFilePathName,pathname);}
	const char *GetPartFilePathName() {return m_pszPartFilePathName;}
	void SetCurPartNode(ui::TreeView::Node *pNode);
	void SavePartFile();
};