#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\frostengine\render.h"
#include "..\..\gameengine\gamemodelmanager.h"

#include "AppClient\RoleModel.h"
#include "gamesetting.h"
#include "wndviewer.h"
#include "animmodifydlg.h"
#include "WndModel.h"
#include "WndLocator.h"
#include "WndAction.h"
#include "WndModelShaderTree.h"
#include "WndModelTree.h"
WndModel::WndModel(void)
{
}

WndModel::~WndModel(void)
{	
}

BOOL WndModel::Create(const char *pszName,const RECT *pRect,DWORD dwStyle,ui::Wnd *pParent)
{
	ui::Wnd::Create(pszName,pRect,dwStyle,pParent);

	// ������ɫģ���б�ؼ�
	RECT rcRoleModels = { 4, 16, 100 - 4, 16 + 126 };
	m_pRoleModels = new ui::ListBox;
	m_pRoleModels->Create("RoleModelsListBox", &rcRoleModels, FWS_VISIBLE | FWS_LBS_CHECK, this);

	// ������ģ���б�ؼ�
	RECT rcBindModels = { 4, 172, 100 - 4, 172 + 126 };
	m_pBindModels = new ui::ListBox;
	m_pBindModels->Create("BindModelsListBox", &rcBindModels, FWS_VISIBLE | FWS_LBS_CHECK, this);

	return TRUE;
}

void WndModel::Destroy()
{
	SAFEDESTROY(m_pRoleModels);
	SAFEDESTROY(m_pBindModels);
	ui::Wnd::Destroy();
}

void WndModel::Update()
{
	// UPDATE DEFAULT
	ui::Wnd::UpdateDefault();

	DWORD & dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE && !(dwStyle & FWS_DISABLE))
	{
		int	 iSelItem	  = m_pRoleModels->GetSelItem();
		int	 iModelAction = 0;
		BOOL bChecked	  = false;

		m_pRoleModels->SetItemChecked(iSelItem, true);
		if (m_pRoleModels->IsSelChanged())
		{
			WndViewer  * pWndViewer	= (WndViewer *)GetParent();
			CRoleModel * pRoleModel	= NULL;
			bChecked = m_pRoleModels->GetItemChecked(iSelItem);

			// ��յ�ǰ��ɫģ���б�ѡ��
			for (int iItem = 0; iItem < m_pRoleModels->GetItemCount(); ++iItem)
				m_pRoleModels->SetItemChecked(iItem, false);

			m_pRoleModels->SetItemChecked(iSelItem, true);

			pRoleModel = pWndViewer->m_vRoleModels[iSelItem];

			// �ؽ���ز˵�
			pWndViewer->DestroyMenuBar();				//�������˵�
			pWndViewer->CreateMenuBar();				//�������˵��̶���Ŀ
			pWndViewer->CreateMainModelMenu((pWndViewer->m_vRoleModels[GetSelRoleModel()]->GetFilePathName()).c_str());			//������ɫģ����ز˵�ѡ��
			pWndViewer->CreateActionMenu(pRoleModel->GetFilePathName().c_str());			//���������Ի�������Ŀ

			if (!pWndViewer->m_vBindModels.empty())		//������ģ����ز˵�ѡ��
				pWndViewer->CreateBindModelMenu(pWndViewer->m_vBindModels[GetSelBindModel()]);

			pWndViewer->m_pWndAction->SetCurrentModelName(m_pRoleModels->GetItemText(iSelItem));
			pWndViewer->m_pWndAction->SetCurrentModelIndex(iSelItem);
			pWndViewer->m_pWndAction->SetCurrentActionType(AT_ROLE);
			pWndViewer->m_pWndAction->SetActionItemCheck();

			// ���ݵ�ǰģ�͵Ķ������ı䵱ǰ���Ŷ���
			iModelAction = pRoleModel->GetCurrentSelActionIndex();
			if (iModelAction == -1)
			{
				AnimInfo::tagActionInfo	* pActionInfo = pWndViewer->m_cAnimInfo.GetActionInfo();
				pActionInfo->dwCurAction	= 0;
				pActionInfo->bInterpolation = FALSE;
			}else
			{
				AnimInfo::tagActionInfo	* pActionInfo = pWndViewer->m_cAnimInfo.GetActionInfo();
				pActionInfo->dwCurAction	= pWndViewer->m_pWndAction->GetActionCode(iModelAction);
				pActionInfo->bInterpolation = FALSE;
			}
			WndModelTexture *pModelTexture =  pWndViewer->GetWndModelShaderTree();
			string filePathName =  pRoleModel->GetFilePathName();
			string modelName;
			pWndViewer->GetModelName(modelName,filePathName);
			WndModelTree *pModeTree = pWndViewer->GetWndModelTree();
			pModeTree->CreateModelTree(filePathName.c_str());
			pModeTree->SetCurrentModelName(modelName.c_str());
		}

		if (m_pBindModels->GetItemCount() != 0)
		{
			WndViewer  * pWndViewer = (WndViewer *)GetParent();
			CBindModel * pBindModel	= NULL;
			iSelItem = m_pBindModels->GetSelItem();
			bChecked = m_pBindModels->GetItemChecked(iSelItem);

			pWndViewer->m_pWndLocator->ShowWnd(bChecked);
			if (m_pBindModels->IsSelChanged())
			{
				pBindModel			 = pWndViewer->m_vBindModels[iSelItem];
				int	iBindModeSelItem = pBindModel->GetCurrentSelLocatorIndex();

				// ��յ�ǰ��ģ���б�ѡ��
				for (int iItem = 0; iItem < m_pBindModels->GetItemCount(); ++iItem)
					m_pBindModels->SetItemChecked(iItem, false);

				// ��հ󶨵�Ի����ѡ��
				for (int iItem = 0; iItem < (int)pWndViewer->m_pWndLocator->GetItemCount(); ++iItem)
					pWndViewer->m_pWndLocator->SetItemChecked(iItem, false);

				m_pBindModels->SetItemChecked(iSelItem, bChecked);

				// �趨��ǰѡ��ģ����ѡ��İ󶨵�ѡ��
				if (iBindModeSelItem != -1)
					pWndViewer->m_pWndLocator->SetItemChecked(iBindModeSelItem, true);

				pWndViewer->m_pWndLocator->SetCurrentSelBindModelIndex(iSelItem);
				pWndViewer->m_pWndLocator->SetCurrentSelBindModelName(m_pBindModels->GetItemText(iSelItem));

				// �ؽ���ز˵�
				pWndViewer->DestroyMenuBar();				//�������˵�
				pWndViewer->CreateMenuBar();				//�������˵��̶���Ŀ
				pWndViewer->CreateMainModelMenu((pWndViewer->m_vRoleModels[GetSelRoleModel()]->GetFilePathName()).c_str());			//������ɫģ����ز˵�ѡ��
				pWndViewer->CreateActionMenu(pBindModel->GetFilePathName().c_str());			//���������Ի�������Ŀ

				if (!pWndViewer->m_vBindModels.empty())		//������ģ����ز˵�ѡ��
					pWndViewer->CreateBindModelMenu(pWndViewer->m_vBindModels[GetSelBindModel()]);

				pWndViewer->m_pWndAction->SetCurrentModelIndex(iSelItem);
				pWndViewer->m_pWndAction->SetCurrentModelName(m_pBindModels->GetItemText(iSelItem));
				pWndViewer->m_pWndAction->SetCurrentActionType(AT_BIND);
				pWndViewer->m_pWndAction->SetActionItemCheck();
				pWndViewer->m_pWndAnimModify->ShowWnd(false);

				//pBindModel	 = pWndViewer->m_vBindModels[iSelItem];
				//iModelAction = pBindModel->GetCurrentSelActionIndex();
				//if (iModelAction == -1)
				//{
				//	
				//}else
				//{
				//}
			}
		}

		ui::Wnd::UpdateChild();
	}
}

void WndModel::Render()
{
	//NO DRAW DEFAULT
	render::Interface *pInterface = render::Interface::GetInstance();
	ASSERT(pInterface);

	render::Interface::Layer2D *pLayer2D = pInterface->GetLayer2D();
	ASSERT(pLayer2D);

	//SELF
	DWORD &dwStyle = m_tWindowInfo.dwStyle;
	if (dwStyle & FWS_VISIBLE)
	{
		ui::Wnd::RenderDefault();

		render::Interface::Layer2D * pLayer2D = render::Interface::GetInstance()->GetLayer2D();

		RECT rcLB;
		m_pRoleModels->GetWndRect(&rcLB);
		pLayer2D->DrawWireQuadRect(rcLB.left ,rcLB.top,rcLB.right,rcLB.bottom,FWC_HIGH);
		int iX = rcLB.left, iY = rcLB.top - 12;
		ui::Wnd::SetUIRendState();
		pLayer2D->TextOut(iX + 1 + 4,iY + 1 + 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY - 1+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX + 1+ 4,iY - 1+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY + 1+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX + 1+ 4,iY+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY + 1+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY - 1+ 2,"��ɫģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY+ 2,"��ɫģ���б�",12,FWC_WHITE);

		m_pBindModels->GetWndRect(&rcLB);
		pLayer2D->DrawWireQuadRect(rcLB.left ,rcLB.top,rcLB.right,rcLB.bottom,FWC_HIGH);
		iX = rcLB.left;
		iY = rcLB.top - 12;
		ui::Wnd::SetUIRendState();
		pLayer2D->TextOut(iX + 1 + 4,iY + 1 + 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY - 1+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX + 1+ 4,iY - 1+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY + 1+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX + 1+ 4,iY+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX - 1+ 4,iY+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY + 1+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY - 1+ 2,"��ģ���б�",12,FWC_BLACK);
		pLayer2D->TextOut(iX+ 4,iY+ 2,"��ģ���б�",12,FWC_WHITE);

		//NO CHILD
		ui::Wnd::RenderChild();	
	}
}

void WndModel::AddRoleModel(const char * szModelName)
{
	m_pRoleModels->InsertItem(szModelName);
	m_pRoleModels->SetSelItem(m_pRoleModels->GetItemCount() - 1);

	// ��յ�ǰ��ɫģ���б�ѡ��
	for (int iItem = 0; iItem < m_pRoleModels->GetItemCount(); ++iItem)
		m_pRoleModels->SetItemChecked(iItem, false);

	m_pRoleModels->SetItemChecked(m_pRoleModels->GetSelItem(), true);
}

void WndModel::AddBindModel(const char * szModelName)
{
	m_pBindModels->InsertItem(szModelName);
	m_pBindModels->SetSelItem(m_pBindModels->GetItemCount() - 1);

	// ��յ�ǰ��ɫģ���б�ѡ��
	for (int iItem = 0; iItem < m_pBindModels->GetItemCount(); ++iItem)
		m_pBindModels->SetItemChecked(iItem, false);

	m_pBindModels->SetItemChecked(m_pBindModels->GetSelItem(), true);
}