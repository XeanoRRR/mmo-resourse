#pragma once
#include "..\..\frostengine\ui.h"

class WndViewer;

class WndModelModify: public ui::Wnd
{
	ui::Button	* m_pBtnAddGroup;					//���GROUP
	ui::Button	* m_pBtnDelGroup;					//ɾ�����һ��Group
	ui::Button	* m_pBtnAddAction;					//��Ӷ���
	ui::Button	* m_pBtnDelAction;					//ɾ������
	ui::Button  * m_pBtnSetActionFile;				//ָ������anim�ļ�
	ui::Button  * m_pBtnAddPart;					//���part��
	ui::Button	* m_pBtnDelPart;					//ɾ��part��
	ui::Button	* m_pBtnSetPartFile;				//����part�ļ�

	ui::Button	* m_pBtnAddTrail_emitter;			//���β����������
	ui::Button  * m_pBtnDelTrail_emitter;			//ɾ��β����������
	ui::Button	* m_pBtnSetTrail_MeshFile;			//����β��MESH�ļ�
	ui::Button	* m_pBtnSetTrail_TrailFile;			//����β��Trail�ļ�

	ui::Button	* m_pBtnAddParticle_emitter;		//������ӷ�������
	ui::Button  * m_pBtnDelParticle_emitter;		//ɾ�����ӷ�������
	ui::Button	* m_pBtnSetParticle_MeshFile;		//��������MESH�ļ�
	ui::Button	* m_pBtnSetParticle_ParticleFile;	//��������Trail�ļ�
	ui::EditBox * m_pEditNumber;					//������������

	ui::Button	* m_pConfirm;						//ȷ��������
	ui::Button  * m_pCancel;						//ȡ��
	ui::TreeView::Node *m_pNode;					//��ǰ�༭��Node
public:
	WndModelModify(void);
	~WndModelModify(void);

	BOOL Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent);
	void Destroy();

	virtual void Update();
	virtual void Render();
	void SetNode(ui::TreeView::Node *pNode) {if (pNode == m_pNode) return; m_pNode = pNode;}
	void CaculateWhichToDisplay();

	const char * OpenFileNamePath(string *filestring,const char * postfix);

};