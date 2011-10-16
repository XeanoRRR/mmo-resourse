#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndParticleModify: public ui::Wnd
{
	char				m_pszParticleFilePathName[128];	//particle·���ļ���
	ui::TreeView::Node	*m_pNode;

	ui::CheckBox	*m_Check[2];
	ui::Button		*m_pBtnConfirm;		//ȷ������
	ui::Button		*m_pBtnCancel;		//ȡ��
	
	ui::EditBox		*m_pEditBox;		//�༭��
public:
	WndParticleModify(void);
	~WndParticleModify(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	virtual void Update();
	virtual void Render();

	void SetCurPartNode(ui::TreeView::Node *pNode);
	void CaculateWhilchToDisplay();
};