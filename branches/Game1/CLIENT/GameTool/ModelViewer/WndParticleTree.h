#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndParticleTree: public ui::Wnd
{
	ui::TreeView  * m_pParticleTreeView;			//��ʾģ�͵����οؼ�
	char		  m_pszParticleFilePathName[128];	//ģ��·���ļ���
	
public:
	WndParticleTree(void);
	~WndParticleTree(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	ui::TreeView * GetParticleTreeView() {return m_pParticleTreeView;}
	void CreateParticleTree(const char * strParticleFilePathName);
	virtual void Update();
	virtual void Render();
	const char *GetParticleFilePathName() {return m_pszParticleFilePathName;}
	void SaveParticleFile();
};