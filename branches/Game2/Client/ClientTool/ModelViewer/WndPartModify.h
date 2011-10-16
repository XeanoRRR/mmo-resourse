#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndPartModify: public ui::Wnd
{
	char				m_pszPartFilePathName[128];	//ģ��part·���ļ���
	ui::TreeView::Node	*m_pNode;

	ui::CheckBox	*m_Check[2];
	ui::Button		*m_pBtnConfirm;		//ȷ������
	ui::Button		*m_pBtnCancel;		//ȡ��
	ui::Combo		*m_pCombo;
	ui::Button		*m_pBtnSetMeshFile;

	ui::CheckBox		*m_pAnimcolorCheck;		
	ui::CheckBox		*m_pAnimuvCheck;
	ui::CheckBox		*m_pAnimvisibilityCheck;
	ui::CheckBox		*m_pAnimtextureCheck;
	ui::CheckBox		*m_pEnviromentmapsCheck;

	ui::Button			*m_pBtnSetAnimcolor;			//����AC�ļ�
	ui::Button			*m_pBtnSetAnimuv;				//����UV�ļ�
	ui::Button			*m_pBtnSetAnimvisibility;		//����VIS�ļ�
	ui::Button			*m_pBtnSetAnimtexture;			//����AT�ļ�

	ui::Button			*m_pBtnAddRenderMesh;			//����һ��RenderMesh
	ui::Button			*m_pBtnDelRenderMesh;			//ɾ��һ��RenderMesh

	ui::Button			*m_pBtnAddLevel;				//����һ��levelarray
	ui::Button			*m_pBtnDelLevel;				//ɾ��һ��levelarray

	ui::Button			*m_pBtnAddMeshIndex;			//����һ��MeshIndex
	ui::Button			*m_pBtnDelMeshIndex;			//ɾ��һ��MeshIndex
	ui::Combo			*m_pComboMeshList;				//�༭MeshIndexʱ ѡ��ǰ��MESH��

	ui::Button			*m_pBtnAddRenderMeshItem;		//����RenderMesh�������Ŀ
	ui::Button			*m_pBtnDelRenderMeshItem;		//ɾ��RenderMesh��������һ����Ŀ

public:
	WndPartModify(void);
	~WndPartModify(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	virtual void Update();
	virtual void Render();

	void SetPartFilePathName (const char * pathname) {strcpy(m_pszPartFilePathName,pathname);}
	const char *GetPartFilePathName() {return m_pszPartFilePathName;}
	void SetCurPartNode(ui::TreeView::Node *pNode);
	void CaculateToDisplay();
};