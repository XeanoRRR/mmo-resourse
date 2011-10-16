#include "stdafx.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"
#include "..\..\gameengine\gamemodelmanager.h"

#include "gamesetting.h"
#include "wndviewer.h"
#include "animmodifydlg.h"
#include "WndModel.h"
#include "WndModelTree.h"
#include "WndModelFileModify.h"
#include <fstream>

WndModelModify::WndModelModify()
{
	m_pBtnAddGroup = NULL;					//���GROUP
	m_pBtnDelGroup = NULL;					//ɾ�����һ��Group
	m_pBtnAddAction = NULL;					//��Ӷ���
	m_pBtnDelAction = NULL;					//ɾ������
	m_pBtnSetActionFile = NULL;				//ָ������anim�ļ�
	m_pBtnAddPart = NULL;					//���part��
	m_pBtnDelPart = NULL;					//ɾ��part��
	m_pBtnSetPartFile = NULL;				//����part�ļ�

	m_pBtnAddTrail_emitter = NULL;			//���β����������
	m_pBtnDelTrail_emitter = NULL;			//ɾ��β����������
	m_pBtnSetTrail_MeshFile = NULL;			//����β��MESH�ļ�
	m_pBtnSetTrail_TrailFile = NULL;		//����β��Trail�ļ�

	m_pBtnAddParticle_emitter = NULL;		//������ӷ�������
	m_pBtnDelParticle_emitter = NULL;		//ɾ�����ӷ�������
	m_pBtnSetParticle_MeshFile = NULL;		//��������MESH�ļ�
	m_pBtnSetParticle_ParticleFile = NULL;	//��������Trail�ļ�
	m_pEditNumber = NULL;					//������������

	m_pConfirm = NULL;
	m_pCancel = NULL;
	m_pNode = NULL;							//��ǰ�༭��Node
}

void WndModelModify::Destroy()
{
	SAFEDESTROY(m_pBtnAddGroup);
	SAFEDESTROY(m_pBtnDelGroup);
	
	SAFEDESTROY(m_pBtnAddAction);
	SAFEDESTROY(m_pBtnDelAction);
	SAFEDESTROY(m_pBtnSetActionFile);

	SAFEDESTROY(m_pBtnAddPart);
	SAFEDESTROY(m_pBtnDelPart);
	SAFEDESTROY(m_pBtnSetPartFile);

	SAFEDESTROY(m_pBtnAddTrail_emitter);
	SAFEDESTROY(m_pBtnDelTrail_emitter);
	SAFEDESTROY(m_pBtnSetTrail_MeshFile);
	SAFEDESTROY(m_pBtnSetTrail_TrailFile);

	SAFEDESTROY(m_pBtnAddParticle_emitter);
	SAFEDESTROY(m_pBtnDelParticle_emitter);
	SAFEDESTROY(m_pBtnSetParticle_MeshFile);
	SAFEDESTROY(m_pBtnSetParticle_ParticleFile);
	SAFEDESTROY(m_pEditNumber);
	SAFEDESTROY(m_pConfirm);
	SAFEDESTROY(m_pCancel);
	m_pNode = NULL;
	ui::Wnd::Destroy();
}

WndModelModify::~WndModelModify()
{

}
void WndModelModify::CaculateWhichToDisplay()
{
	if (!m_pNode)
		return;
	//////SetAllVisibleFlase/////////////
	m_pBtnAddGroup->ShowWnd(false);					//���GROUP
	m_pBtnDelGroup->ShowWnd(false);					//ɾ�����һ��Group
	m_pBtnAddAction->ShowWnd(false);					//��Ӷ���
	m_pBtnDelAction->ShowWnd(false);					//ɾ������
	m_pBtnSetActionFile->ShowWnd(false);				//ָ������anim�ļ�
	m_pBtnAddPart->ShowWnd(false);					//���part��
	m_pBtnDelPart->ShowWnd(false);					//ɾ��part��
	m_pBtnSetPartFile->ShowWnd(false);				//����part�ļ�

	m_pBtnAddTrail_emitter->ShowWnd(false);			//���β����������
	m_pBtnDelTrail_emitter->ShowWnd(false);			//ɾ��β����������
	m_pBtnSetTrail_MeshFile->ShowWnd(false);			//����β��MESH�ļ�
	m_pBtnSetTrail_TrailFile->ShowWnd(false);		//����β��Trail�ļ�

	m_pBtnAddParticle_emitter->ShowWnd(false);		//������ӷ�������
	m_pBtnDelParticle_emitter->ShowWnd(false);		//ɾ�����ӷ�������
	m_pBtnSetParticle_MeshFile->ShowWnd(false);		//��������MESH�ļ�
	m_pBtnSetParticle_ParticleFile->ShowWnd(false);	//��������Trail�ļ�
	m_pEditNumber->ShowWnd(false);					//������������

	m_pConfirm->ShowWnd(false);
	m_pCancel->ShowWnd(false);
	ShowWnd(false);
	/////////////////////////////////////
	RECT rcContol2;
	rcContol2.left = 480;
	rcContol2.top = 120;
	rcContol2.right = rcContol2.left + 162;
	rcContol2.bottom = rcContol2.top + 25;

	const char *name = m_pNode->GetName();
	const char *value = m_pNode->GetValue();
	const char *parentName = NULL;				
	const char *GrandFatherName = NULL;
	const char *GreatGrandFatherName = NULL;
	ui::TreeView::Node *pCurNodeParent = m_pNode->GetParent();
	ui::TreeView::Node *pNodeGrandfather = NULL;
	ui::TreeView::Node *pNodeGreatGrandfather = NULL;
	if(pCurNodeParent)
	{
		parentName = pCurNodeParent->GetName();
		pNodeGrandfather = pCurNodeParent->GetParent();
		if(pNodeGrandfather)
		{
			GrandFatherName = pNodeGrandfather->GetName();
			pNodeGreatGrandfather = pNodeGrandfather->GetParent();
			if (pNodeGreatGrandfather)
				GreatGrandFatherName = pNodeGreatGrandfather->GetName();
		}
	}

	if (strcmp(name,"animuvcircle") == 0 || strcmp(name,"animcolorcircle") == 0 ||
		strcmp(name,"animvisibilitycircle") == 0 || strcmp(name,"animtexturecircle") == 0 )
	{
		m_pEditNumber->SetEditText(m_pNode->GetValue());
		m_pEditNumber->ShowWnd(true);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if (strcmp(name,"group") == 0 && strcmp(parentName,"main") == 0)
	{
		m_pBtnDelGroup->SetWndRect(&rcContol2);
		m_pBtnAddGroup->ShowWnd(true);
		m_pBtnDelGroup->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if (strcmp(parentName,"group") == 0 && strcmp(GrandFatherName,"main") == 0)
	{
		rcContol2.top = 42;
		rcContol2.bottom = 42 + 25;
		m_pEditNumber->SetEditText(name);
		m_pEditNumber->ShowWnd(true);
		m_pEditNumber->SetWndRect(&rcContol2);
		rcContol2.top += 30;
		rcContol2.bottom = rcContol2.top + 25;
		m_pBtnAddPart->SetWndRect(&rcContol2);
		rcContol2.top += 30;
		rcContol2.bottom = rcContol2.top + 25;
		m_pBtnDelPart->SetWndRect(&rcContol2);
		m_pBtnAddPart->ShowWnd(true);
		m_pBtnDelPart->ShowWnd(true);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if (GrandFatherName && strcmp(GrandFatherName,"group") == 0 && GreatGrandFatherName && strcmp(GreatGrandFatherName,"main") == 0)
	{
		m_pEditNumber->SetEditText(name);
		m_pEditNumber->ShowWnd(true);
		rcContol2.top -= 35;
		rcContol2.bottom -= 35;
		m_pBtnSetPartFile->SetWndRect(&rcContol2);				//����part
		m_pBtnSetPartFile->ShowWnd(true);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if (strcmp(name,"action") == 0 && strcmp(parentName,"main") == 0)
	{
		m_pBtnAddAction->ShowWnd(true);								//��Ӷ���
		m_pBtnDelAction->ShowWnd(true);
		m_pBtnDelAction->SetWndRect(&rcContol2);					//ɾ������
		ShowWnd(true);
		return;
	}
	else if (strcmp(parentName,"action") == 0 && GrandFatherName &&strcmp(GrandFatherName,"main") == 0)
	{
		m_pEditNumber->SetEditText(name);
		m_pEditNumber->ShowWnd(true);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if(GrandFatherName && strcmp(GrandFatherName,"action") == 0 && GreatGrandFatherName && strcmp(GreatGrandFatherName,"main") == 0)
	{
		rcContol2.top -= 30;
		rcContol2.bottom -= 30;
		m_pBtnSetActionFile->SetWndRect(&rcContol2);
		m_pBtnSetActionFile->ShowWnd(true);
		ShowWnd(true);
		return;
	}
	else if (strcmp(name,"particle_emitter") == 0 && strcmp(parentName,"main") == 0)
	{
		m_pBtnAddParticle_emitter->ShowWnd(true);
		m_pBtnDelParticle_emitter->ShowWnd(true);
		m_pBtnDelParticle_emitter->SetWndRect(&rcContol2);
		ShowWnd(true);
		return;
	}
	else if (strcmp(parentName,"particle_emitter") == 0 && GrandFatherName && strcmp(GrandFatherName,"main") == 0)
	{
		m_pEditNumber->SetEditText(value);
		m_pEditNumber->ShowWnd(true);
		rcContol2.top -= 30;
		rcContol2.bottom -= 30;
		m_pBtnSetParticle_ParticleFile->ShowWnd(true);
		m_pBtnSetParticle_ParticleFile->SetWndRect(&rcContol2);
		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
	}
	else if (GrandFatherName && strcmp(GrandFatherName,"particle_emitter") == 0 && GreatGrandFatherName && strcmp(GreatGrandFatherName,"main") == 0)
	{
		if (strcmp(name,"shape") == 0)				//���ӷ���������ɾ����
		{		
			rcContol2.top -= 30;								
			rcContol2.bottom -= 30;
			m_pBtnSetParticle_MeshFile->ShowWnd(true);
			m_pBtnSetParticle_MeshFile->SetWndRect(&rcContol2);
			ShowWnd(true);
		}
		else if (strcmp(name,"number") == 0)
		{
			m_pEditNumber->SetEditText(value);
			m_pEditNumber->ShowWnd(true);
			m_pConfirm->ShowWnd(true);
			m_pCancel->ShowWnd(true);
			ShowWnd(true);
		}
		return;
	}
	else if (strcmp(name,"trail_emitter") == 0 && strcmp(parentName,"main") == 0)
	{
		m_pBtnAddTrail_emitter->ShowWnd(true);
		m_pBtnDelTrail_emitter->ShowWnd(true);
		m_pBtnDelTrail_emitter->SetWndRect(&rcContol2);
		ShowWnd(true);
		return;
	}
	else if (strcmp(parentName,"trail_emitter") == 0 && GrandFatherName &&strcmp(GrandFatherName,"main") == 0)
	{
		rcContol2.top = 42;
		rcContol2.bottom = 42 + 25;
		m_pEditNumber->ShowWnd(true);
		m_pEditNumber->SetWndRect(&rcContol2);
		m_pEditNumber->SetEditText(name);
		rcContol2.top += 30;								//β������������ɾ����
		rcContol2.bottom = rcContol2.top + 25;

		m_pBtnSetTrail_MeshFile->ShowWnd(true);
		m_pBtnSetTrail_MeshFile->SetWndRect(&rcContol2);
		rcContol2.top += 30;
		rcContol2.bottom = rcContol2.top + 25;
		m_pBtnSetTrail_TrailFile->ShowWnd(true);
		m_pBtnSetTrail_TrailFile->SetWndRect(&rcContol2);

		m_pConfirm->ShowWnd(true);
		m_pCancel->ShowWnd(true);
		ShowWnd(true);
		return;
	}
}
void WndModelModify::Update()
{

	//UPDATE DEFAULT
	ui::Wnd::UpdateDefault();

	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();
	if (dwStyle & FWS_VISIBLE && !(dwStyle & FWS_DISABLE))
	{
		ui::Wnd::UpdateChild();		
		if(pInputInfo->eType == UIMT_MS_BTNDOWN && pInputInfo->dwData & MK_LBUTTON)
		{
			if(!m_pNode)
				return;
			const char *name = m_pNode->GetName();
			const char *value = m_pNode->GetValue();
			const char *parentName = NULL;				
			const char *GrandFatherName = NULL;
			const char *GreatGrandFatherName = NULL;
			ui::TreeView::Node *pCurNodeParent = m_pNode->GetParent();
			ui::TreeView::Node *pNodeGrandfather = NULL;
			ui::TreeView::Node *pNodeGreatGrandfather = NULL;
			if(pCurNodeParent)
			{
				parentName = pCurNodeParent->GetName();
				pNodeGrandfather = pCurNodeParent->GetParent();
				if(pNodeGrandfather)
				{
					GrandFatherName = pNodeGrandfather->GetName();
					pNodeGreatGrandfather = pNodeGrandfather->GetParent();
					if (pNodeGreatGrandfather)
						GreatGrandFatherName = pNodeGreatGrandfather->GetName();
				}
			}
			WndViewer  * pWndViewer	= (WndViewer *)GetParent();
			WndModelTree *pWndModelTree = pWndViewer->GetWndModelTree();
			ui::TreeView *pTreeView = pWndModelTree->GetModelTreeView();
			
			RECT rcControl;
			//���ȡ��
			m_pCancel->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pCancel-IsVisible())		
			{
				ShowWnd(false);
			}
			//ȷ��������
			m_pConfirm->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pConfirm-IsVisible())		
			{

				char strText[128] = "";
				m_pEditNumber->GetEditText(strText,127);
				if (strlen(strText) == 0 && m_pEditNumber->IsVisible())
				{
					MessageBox(NULL,"�������óɿ��ַ�","����",MB_OK);
					return;
				}
				if (strcmp(parentName,"timer")==0 &&(strcmp(name,"animuvcircle") == 0 || strcmp(name,"animcolorcircle") == 0 ||
					strcmp(name,"animvisibilitycircle") == 0 || strcmp(name,"animtexturecircle") == 0) )
				{
					m_pNode->SetValue(strText);
				} 
				else if(GrandFatherName && (strcmp(parentName,"group")==0 || strcmp(GrandFatherName,"group")==0))
				{
					m_pNode->SetName(strText);
				}
				else if (strcmp(parentName,"trail_emitter") == 0 && GrandFatherName &&strcmp(GrandFatherName,"main") == 0)
				{
					m_pNode->SetName(strText);
				}
				else if (GrandFatherName && strcmp(GrandFatherName,"particle_emitter") == 0 
					&& GreatGrandFatherName && strcmp(GreatGrandFatherName,"main") == 0 && strcmp(name,"number") == 0)
				{
					m_pNode->SetValue(strText);
				}
				else if (strcmp(parentName,"particle_emitter") == 0 && GrandFatherName && strcmp(GrandFatherName,"main") == 0)
				{
					m_pNode->SetName(strText);
				}
				else if (strcmp(name,"action") == 0 && strcmp(parentName,"main") == 0)
				{
					m_pNode->SetName(strText);
				}
				else if (strcmp(parentName,"action") == 0 && GrandFatherName &&strcmp(GrandFatherName,"main") == 0)
				{
					m_pNode->SetName(strText);
				}
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			//���Group
			m_pBtnAddGroup->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddGroup->IsVisible())
			{
				ui::TreeView::Node *pNewGroupNode = new ui::TreeView::Node;
				pNewGroupNode->SetName("�����ӵ�Group");
				pNewGroupNode->SetParent(m_pNode);
				pTreeView->AppendChildNode(m_pNode,pNewGroupNode);

				//Ĭ������һ��part
				ui::TreeView::Node *pNewPartNode = new ui::TreeView::Node;
				pNewPartNode->SetName("Ĭ��part");
				pTreeView->AppendChildNode(pNewGroupNode,pNewPartNode);

				//����һ��Ĭ��part�ļ�
				ui::TreeView::Node *pNewPartFileNode = new ui::TreeView::Node;
				pNewPartFileNode->SetName("file");
				pNewPartFileNode->SetValue("armo0001.part");
				pTreeView->AppendChildNode(pNewPartNode,pNewPartFileNode);
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			m_pBtnDelGroup->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddGroup->IsVisible())
			{
				pTreeView->DeleteLastNode(m_pNode);			//ɾ�����һ��Group
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}

			m_pBtnAddPart->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddPart->IsVisible())
			{
				ui::TreeView::Node *pNewPartNode = new ui::TreeView::Node;
				pNewPartNode->SetName("�����ӵ�Part");
				pTreeView->AppendChildNode(m_pNode, pNewPartNode);
				
				//����һ��Ĭ��part�ļ�
				ui::TreeView::Node *pNewPartFileNode = new ui::TreeView::Node;
				pNewPartFileNode->SetName("file");
				pNewPartFileNode->SetValue("armo0001.part");
				pTreeView->AppendChildNode(pNewPartNode, pNewPartFileNode);
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}

			m_pBtnDelPart->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnDelPart->IsVisible())
			{
				pTreeView->DeleteLastNode(m_pNode);			//ɾ�����һ��Group
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}

			//����part�ļ�
			m_pBtnSetPartFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetPartFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"part");
				if (name.length())
				{
					std::vector<ui::TreeView::Node*> *pChildList = m_pNode->GetChildList();
					ui::TreeView::Node *pChildNode = (*pChildList)[0];
					pChildNode->SetValue(name.c_str());
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}
			
			//���β��������
			m_pBtnAddTrail_emitter->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddTrail_emitter->IsVisible())
			{
				ui::TreeView::Node *pNewTrailNode = new ui::TreeView::Node;
				pNewTrailNode->SetName("�����ӵ�Trail");
				pTreeView->AppendChildNode(m_pNode, pNewTrailNode);
				
				//���һ��Ĭ�ϵ�trail
				ui::TreeView::Node *pNewTrail_ShapeNode = new ui::TreeView::Node;
				pNewTrail_ShapeNode->SetName("shape");
				pNewTrail_ShapeNode->SetValue("mesh\\trailX1.mesh");
				pTreeView->AppendChildNode(pNewTrailNode, pNewTrail_ShapeNode);

				ui::TreeView::Node *pNewTrail_TrailNode = new ui::TreeView::Node;
				pNewTrail_TrailNode->SetName("trail");
				pNewTrail_TrailNode->SetValue("blade.trail");
				pTreeView->AppendChildNode(pNewTrailNode, pNewTrail_TrailNode);
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}

			//ɾ��β��������
			m_pBtnDelTrail_emitter->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnDelTrail_emitter->IsVisible())
			{
				pTreeView->DeleteLastNode(m_pNode);			//ɾ�����һ��Group
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			
			//����TrailMesh�ļ�
			m_pBtnSetTrail_MeshFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetTrail_MeshFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"mesh");
				char strTile[128] = "mesh\\";
				if (name.length())
				{
					strcat(strTile,name.c_str());
					std::vector<ui::TreeView::Node*> *pChildList = m_pNode->GetChildList();
					ui::TreeView::Node *pChildNode = (*pChildList)[0];
					pChildNode->SetValue(strTile);
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}
			//����Trail trail�ļ�
			m_pBtnSetTrail_TrailFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetTrail_TrailFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"trail");
				if (name.length())
				{
					std::vector<ui::TreeView::Node*> *pChildList = m_pNode->GetChildList();
					ui::TreeView::Node *pChildNode = (*pChildList)[1];
					pChildNode->SetValue(name.c_str());
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}

			//���particle_emitter
			m_pBtnAddParticle_emitter->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddParticle_emitter->IsVisible())
			{
				ui::TreeView::Node *pNewParticleNode = new ui::TreeView::Node;
				pNewParticleNode->SetName("�����ӵ�Particle");
				pTreeView->AppendChildNode(m_pNode,pNewParticleNode);

				//����Ĭ�ϵĸ��ڵ���Ϣ
				ui::TreeView::Node *pNewParticleNode_Shap = new ui::TreeView::Node;
				pNewParticleNode_Shap->SetName("shape");
				pNewParticleNode_Shap->SetValue("mesh\\fire.mesh");
				pTreeView->AppendChildNode(pNewParticleNode,pNewParticleNode_Shap);

				ui::TreeView::Node *pNewParticleNode_TYPE = new ui::TreeView::Node;
				pNewParticleNode_TYPE->SetName("type");
				pNewParticleNode_TYPE->SetValue("LINE");
				pTreeView->AppendChildNode(pNewParticleNode,pNewParticleNode_TYPE);

				ui::TreeView::Node *pNewParticleNode_NUMBER = new ui::TreeView::Node;
				pNewParticleNode_NUMBER->SetName("number");
				pNewParticleNode_NUMBER->SetValue("1");
				pTreeView->AppendChildNode(pNewParticleNode,pNewParticleNode_NUMBER);

				ui::TreeView::Node *pNewParticleNode_PARTICLE = new ui::TreeView::Node;
				pNewParticleNode_PARTICLE->SetName("particle");
				pNewParticleNode_PARTICLE->SetValue("blink4.particle");
				pTreeView->AppendChildNode(pNewParticleNode,pNewParticleNode_PARTICLE);

				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			//ɾ��particle_emitter
			m_pBtnDelParticle_emitter->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnDelParticle_emitter->IsVisible())
			{
				pTreeView->DeleteLastNode(m_pNode);			
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			//����Particle_MeshFile
			m_pBtnSetParticle_MeshFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetParticle_MeshFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"mesh");
				char strTile[128] = "mesh\\";
				if (name.length())
				{
					strcat(strTile,name.c_str());
					
					m_pNode->SetValue(strTile);
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}
			//����Particle_particleFile
			m_pBtnSetParticle_ParticleFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetParticle_ParticleFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"particle");
				if (name.length())
				{
					std::vector<ui::TreeView::Node*> *pChildList = m_pNode->GetChildList();
					ui::TreeView::Node *pChildNode = (*pChildList)[3];
					pChildNode->SetValue(name.c_str());
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}
			//���Action
			m_pBtnAddAction->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnAddAction->IsVisible())
			{
				ui::TreeView::Node *pNewActionNode = new ui::TreeView::Node;
				pNewActionNode->SetName("�����ӵ�ACTION");
				pTreeView->AppendChildNode(m_pNode,pNewActionNode);

				//����һ��Ĭ�ϵ�action
				ui::TreeView::Node *pNewActionFile = new ui::TreeView::Node;
				pNewActionFile->SetName("file");
				pNewActionFile->SetValue("stnd.anim");
				pTreeView->AppendChildNode(pNewActionNode,pNewActionFile);
				
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			//ɾ��Action
			m_pBtnDelAction->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnDelAction->IsVisible())
			{
				pTreeView->DeleteLastNode(m_pNode);
				pWndModelTree->SaveModelFile();
				ShowWnd(false);
			}
			//���ö����ļ�
			m_pBtnSetActionFile->GetWndRect(&rcControl);
			if (::PtInRect(&rcControl,pInputInfo->ptMouse) && m_pBtnSetActionFile->IsVisible())
			{
				string name = "";
				OpenFileNamePath(&name,"anim");
				if (name.length())
				{
					m_pNode->SetValue(name.c_str());
					pWndModelTree->SaveModelFile();
					ShowWnd(false);
				}
			}
		}
	}
}
void WndModelModify::Render()
{
	string sTitle;
	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE)
	{
		ui::Wnd::RenderDefault();
		if(m_pNode)
		{
			string sTitle = " �༭:";
			sTitle	 += m_pNode->GetName();
			if(strcmp(m_pNode->GetValue(),"")!=0)
			{
				sTitle += " = ";
				sTitle += m_pNode->GetValue();
			}
			render::Interface *pInterface = render::Interface::GetInstance();
			render::Interface::Layer2D *pLayer2D = pInterface->GetLayer2D();

			RECT &rc = m_tWindowInfo.rcRect;

			int iX = rc.left, iY = rc.top;//,iW = rc.right - rc.left + 1,;

			int iTextLen = (int)sTitle.size();
			ui::Wnd::SetUIRendState();
			pLayer2D->TextOut(iX + 1 + 4,iY + 1 + 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX + 1+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY + 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX + 1+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX - 1+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY + 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY - 1+ 2,sTitle.c_str(),iTextLen,FWC_BLACK);
			pLayer2D->TextOut(iX+ 4,iY+ 2,sTitle.c_str(),iTextLen,FWC_WHITE);	
		}
		ui::Wnd::RenderChild();	
	} 
}
BOOL WndModelModify::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent)
{
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);
	m_pBtnAddGroup = new ui::Button;					//���GROUP
	m_pBtnDelGroup = new ui::Button;					//ɾ�����һ��Group
	m_pBtnAddAction = new ui::Button;					//��Ӷ���
	m_pBtnDelAction = new ui::Button;					//ɾ������
	m_pBtnSetActionFile = new ui::Button;				//ָ������anim�ļ�
	m_pBtnAddPart = new ui::Button;					//���part��
	m_pBtnDelPart = new ui::Button;					//ɾ��part��
	m_pBtnSetPartFile = new ui::Button;				//����part�ļ�

	m_pBtnAddTrail_emitter = new ui::Button;			//���β����������
	m_pBtnDelTrail_emitter = new ui::Button;			//ɾ��β����������
	m_pBtnSetTrail_MeshFile = new ui::Button;			//����β��MESH�ļ�
	m_pBtnSetTrail_TrailFile = new ui::Button;		//����β��Trail�ļ�

	m_pBtnAddParticle_emitter = new ui::Button;		//������ӷ�������
	m_pBtnDelParticle_emitter = new ui::Button;		//ɾ�����ӷ�������
	m_pBtnSetParticle_MeshFile = new ui::Button;		//��������MESH�ļ�
	m_pBtnSetParticle_ParticleFile = new ui::Button;	//��������Trail�ļ�
	m_pEditNumber = new ui::EditBox;					//������������

	m_pConfirm = new ui::Button;
	m_pCancel = new ui::Button;

	RECT rcContol = {30, 26, 35+157 , 26+25 };
	
	RECT rcConfirm = {15, 110, 15 + 90, 110+25};
	m_pConfirm->Create("ȷ��/����",&rcConfirm, FWS_VISIBLE ,this);
	
	RECT rcCancel = {123, 110, 123 + 90, 110+25};
	m_pCancel->Create("ȡ��", &rcCancel, FWS_VISIBLE,this);

	m_pBtnAddGroup->Create("���Group", &rcContol, FWS_VISIBLE,this);
	m_pBtnDelGroup->Create("ɾ��Group", &rcContol, FWS_VISIBLE,this);

	m_pBtnAddAction->Create("���Action", &rcContol, FWS_VISIBLE,this);
	m_pBtnDelAction->Create("ɾ��Action", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetActionFile->Create("���ö����ļ�", &rcContol, FWS_VISIBLE,this);

	m_pBtnAddPart->Create("���Part", &rcContol, FWS_VISIBLE,this);
	m_pBtnDelPart->Create("ɾ��Part", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetPartFile->Create("����part�ļ�", &rcContol, FWS_VISIBLE,this);

	m_pBtnAddTrail_emitter->Create("���Trail", &rcContol, FWS_VISIBLE,this);
	m_pBtnDelTrail_emitter->Create("ɾ��Trail", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetTrail_MeshFile->Create("����TrailMesh�ļ�", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetTrail_TrailFile->Create("����Trail�ļ�", &rcContol, FWS_VISIBLE,this);

	m_pBtnAddParticle_emitter->Create("���Particle", &rcContol, FWS_VISIBLE,this);
	m_pBtnDelParticle_emitter->Create("ɾ��Particle", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetParticle_MeshFile->Create("����ParticleMesh�ļ�", &rcContol, FWS_VISIBLE,this);
	m_pBtnSetParticle_ParticleFile->Create("����Particle�ļ�", &rcContol, FWS_VISIBLE,this);
	m_pEditNumber->Create("���ֱ༭", &rcContol, FWS_VISIBLE | FWS_FILLBKG | FWS_OUTLINE,this);
	return TRUE;
}

const char * WndModelModify::OpenFileNamePath(string *filestring,const char * postfix)
{
	render::Interface		   * pInterface	= render::Interface::GetInstance();
	OPENFILENAME ofn;								// common dialog box structure
	static char szFile[_MAX_PATH] = "";		// buffer for file name
	char strFilter[_MAX_PATH]	= "����";//"����part�ļ�  \0*.part";
	
	strcat(strFilter,postfix);
	strcat(strFilter,"�ļ� (*.");
	strcat(strFilter,postfix);
	strcat(strFilter,")");
	char  cha[4] = "*.";
	
	char * szTemp = &(strFilter[strlen(strFilter) + 1]);
	
	szTemp[0] = cha[0];
	szTemp[1] = cha[1];
	szTemp[2] = postfix[0];
	szTemp[3] = postfix[1];
	szTemp[4] = postfix[2];
	szTemp[5] = postfix[3];
	szTemp[6] = postfix[4];
	szTemp[7] = postfix[5];
	szTemp[8] = postfix[6];
	szTemp[9] = postfix[7];
	szTemp[10] = postfix[8];
	szTemp[11] = postfix[9];
	//  ����part�ļ�\0*.part\0
	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = pInterface->GetWndHandle();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = strFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = "";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	static char szOldDirectory[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH ,szOldDirectory);
	char strName[128] = "";
	if(GetOpenFileName(&ofn))
	{
		*filestring = utility::File::GetFileName(szFile);
	}
	return (*filestring).c_str();
}

