#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndShaderModify: public ui::Wnd
{
private:
	ui::Button			*m_pAddTextureBtn;		//�������ť
	ui::Button			*m_pDelTextureBtn;		//ɾ������ť
	ui::Button			*m_pAddShaderBtn;		//������λ�����һ��shader
	ui::Button			*m_pDelShaderBtn;		//������λ��ɾ��Shader
	ui::Button			*m_pReplaceTextureBtn;	//�滻����
	ui::CheckBox		*m_pTwoSideCheckEnable[2];		//��ѡ��	
	ui::Combo			*m_pComboAlphaTest;
	ui::Combo			*m_pComboalphablend;
	ui::Combo			*m_pComboColorOP;
	ui::Combo			*m_pComboTexIndex;
	ui::TreeView::Node	*m_pNode;			//��ǰ�༭��Nodeָ��;
	ui::EditBox			*m_pTextureNameEdit;	//�������ֱ༭��

	ui::Button			*m_pConfirm;		//ȷ���޸�
	ui::Button			*m_pCancel;			//ȡ��

public:
	WndShaderModify(void);
	~WndShaderModify(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	void CaculateWhichToDisplay();	//������Щ��Ҫ��ʾ�Ա༭
	virtual void Update();
	virtual void Render();
	void	SetCurNode(ui::TreeView::Node *pNode);
};