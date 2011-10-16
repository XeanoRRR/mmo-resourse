
#include "stdafx.h"

#include "..\..\engine\utility.h"
#include "..\..\engine\console.h"
#include "..\..\engine\render.h"
#include "..\..\engine\ui.h"

#include "..\..\EngineExtend\gameselectray.h"

#include "editwnd.h"
#include "commdb.h"
#include "visibleset.h"
#include "selectfrustum.h"
#include "toolsecurityedit.h"
#include "commui.h"
#include "map.h"
#include "mtllistbox.h"
#include "cmdmgr.h"
#include "cmdmapfillsecurity.h"
#include "listboxsecurity.h"
#include "cmdmapfillsecurity.h"
#include "cmdmapfillsecurity.h"
#include "terrainmesh.h"

const char *pszSecurityEditStatusName[] = {"ѡ��","����"};

ToolSecurityEdit::ToolSecurityEdit(void)
{
	m_dwCurSelGrid = 0xFFFFFFFF;
	m_iBrushSize   = 0;
}

ToolSecurityEdit::~ToolSecurityEdit(void)
{
}

void ToolSecurityEdit::Create()
{
}

void ToolSecurityEdit::Destroy(void)
{
}

void ToolSecurityEdit::PaintBrush(DWORD dwSecurity)
{
	if (m_dwCurSelGrid == 0xFFFFFFFF)
		return;       
	
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	DWORD dwMapWidth = pMap->GetWidth();
	DWORD dwMapDepth = pMap->GetDepth();
	int iCenterX = m_dwCurSelGrid % dwMapWidth;
	int iCenterZ = m_dwCurSelGrid / dwMapWidth;

	TerrainMesh *pMesh = CommDB::GetInstance()->GetTerrainMesh();
	TerrainMesh::_tagGrid *pGrid(NULL);
	TerrainMesh::_tagVertex *pVertexArray = pMesh->GetVertexList();
	D3DXVECTOR3 pos[4];

	BOOL bAllLike = TRUE;

	for(int j = - m_iBrushSize; j < m_iBrushSize + 1; j++)
	{
		for(int i = - m_iBrushSize; i < m_iBrushSize + 1;i++)
		{
			int iX = iCenterX + i;
			int iZ = iCenterZ + j;

			if (!(iX < 0 || iX >= (int)dwMapWidth || iZ < 0 || iZ >= (int)dwMapDepth))
			{
				//�ڵ�ͼ��
				//��ʾ����
				DWORD dwGridIndex = iX + iZ * dwMapWidth;
				//����Ƿ�ȫ����ͬ
				if (pMap->GetGrid(dwGridIndex)->GetSecurity() != dwSecurity)
				{
					bAllLike = FALSE;
					break;
				}
			}
		}
	}
	if (!bAllLike)//�в�ͬ����Ϳ
	{
		CmdMapFillSecurity *pCmd = new CmdMapFillSecurity;
		pCmd->SetSecurity(dwSecurity);
		for(int j = - m_iBrushSize; j < m_iBrushSize + 1; j++)
		{
			for(int i = - m_iBrushSize; i < m_iBrushSize + 1;i++)
			{
				int iX = iCenterX + i;
				int iZ = iCenterZ + j;

				if (!(iX < 0 || iX >= (int)dwMapWidth || iZ < 0 || iZ >= (int)dwMapDepth))
				{
					//�ڵ�ͼ��
					//��ʾ����
					DWORD dwGridIndex = iX + iZ * dwMapWidth;
					pCmd->AddGridIndex(dwGridIndex);					
				}
			}
		}

		CmdMgr::GetInstance()->Do(pCmd);
	}
}

void ToolSecurityEdit::MessageProc(BOOL bActive)
{
	//�л�
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();
	EditWnd *pEditWnd = CommUI::GetInstance()->GetEditWnd();
	ListBoxSecurity *pListBox = pEditWnd->GetListBoxSecurity();
	DWORD dwSelIndex = (DWORD)((pListBox->GetSelItem() > 0 )? pListBox->GetSelItem(): 0);

	if (pInputInfo->eType == UIMT_KB_KEYDOWN)
	{
		switch(pInputInfo->dwData)
		{
		case DIK_LBRACKET:
			m_iBrushSize--;
			if (m_iBrushSize < 0)
			{
				m_iBrushSize = 0;
			}
			break;
		case DIK_RBRACKET:
			m_iBrushSize++;
			if (m_iBrushSize > 5)
			{
				m_iBrushSize = 5;
			}
			break;
		}
	}

	//ѡ��
	m_dwCurSelGrid = GetSelectGrid();
	if (bActive)
		{
			if (pInputInfo->dwMouseButtonState & MK_LBUTTON)
			{
				PaintBrush((DWORD)dwSelIndex);
			}
		}
}

void ToolSecurityEdit::DrawBrush(void)
{
	if (m_dwCurSelGrid == 0xFFFFFFFF) 
		return;

	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer3D *pLayer3D = render::Interface::GetInstance()->GetLayer3D();

	EditMap *pMap = CommDB::GetInstance()->GetMap();
	DWORD dwMapWidth = pMap->GetWidth();
	DWORD dwMapDepth = pMap->GetDepth();
	int iCenterX = m_dwCurSelGrid % dwMapWidth;
	int iCenterZ = m_dwCurSelGrid / dwMapWidth;

	TerrainMesh *pMesh = CommDB::GetInstance()->GetTerrainMesh();
	TerrainMesh::_tagGrid *pGrid(NULL);
	TerrainMesh::_tagVertex *pVertexArray = pMesh->GetVertexList();
	D3DXVECTOR3 pos[4];


	for(int j = - m_iBrushSize; j < m_iBrushSize + 1; j++)
	{
		for(int i = - m_iBrushSize; i < m_iBrushSize + 1;i++)
		{
			int iX = iCenterX + i;
			int iZ = iCenterZ + j;

			if (!(iX < 0 || iX >= (int)dwMapWidth || iZ < 0 || iZ >= (int)dwMapDepth))
			{
				//�ڵ�ͼ��
				//��ʾ����
				DWORD dwGridIndex = iX + iZ * dwMapWidth;
				pGrid = pMesh->GetGrid(dwGridIndex);

				for(DWORD v = 0; v < 4; v++)
				{
					pos[v] = pVertexArray[pGrid->dwVertexIndex[v]].vPosition;
				}
				pLayer3D->_DEBUG_DrawTriangle(&pos[0],&pos[1],&pos[2],0x8080ff80);
				pLayer3D->_DEBUG_DrawTriangle(&pos[0],&pos[2],&pos[3],0x8080ff80);
			}
		}
	}
}

void ToolSecurityEdit::Render()
{
	DrawBrush();
	_ShowStatus();
}

DWORD ToolSecurityEdit::GetSelectGrid(void)
{
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();
	//����ѡ�еĸ���
	GameSelectRay cSelRay;
	RECT rc;
	EditWnd *pEditWnd = CommUI::GetInstance()->GetEditWnd();
	pEditWnd->GetWndRect(&rc);
	cSelRay.CalculateRay(&rc,pEditWnd->GetCamera(),pInputInfo->ptMouse);
	//��ʾ����
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	VisibleSet *pSet = pMap->GetVisibleSet();
	DWORD *pVIArray = pSet->GetVisibleIndexArray();
	TerrainMesh *pMesh = CommDB::GetInstance()->GetTerrainMesh();
	TerrainMesh::_tagGrid *pGrid(NULL);
	TerrainMesh::_tagVertex *pVertexArray = pMesh->GetVertexList();
	D3DXVECTOR3 pos[4];
	DWORD dwSelGrid = 0xFFFFFFFF;
	float fDistance = 100000000.0f;
	float fTempDist;

	for(DWORD n = 0; n< pSet->GetVisibleIndexCount(); n++)
	{
		DWORD dwGridIndex = pVIArray[n];
		pGrid = pMesh->GetGrid(dwGridIndex);

		for(DWORD v = 0; v < 4; v++)
		{
			pos[v] = pVertexArray[pGrid->dwVertexIndex[v]].vPosition;
		}

	    if (cSelRay.IntersectTri(&pos[0],&pos[1],&pos[2],NULL,&fTempDist))
		{
			if (fTempDist < fDistance)
			{
				fDistance = fTempDist;
				dwSelGrid = dwGridIndex;
			}
		}
		else
			if (cSelRay.IntersectTri(&pos[0],&pos[2],&pos[3],NULL,&fTempDist))
			{	 
				if (fTempDist < fDistance)
				{
					fDistance = fTempDist;
					dwSelGrid = dwGridIndex;
				}

				
			}
	}
	return dwSelGrid;
}

void ToolSecurityEdit::_ShowStatus(void)
{
	//render::Interface *pInterface = render::Interface::GetInstance();
	//render::Interface::Layer2D   *pLayer2D   = pInterface->GetLayer2D();

	////DRAW �༭״̬
	//static char szTemp[256];

	//sprintf(szTemp,"[Q]:%s\n",pszSecurityEditStatusName[m_eEditStatus]);
	//int iTexLen = lstrlen(szTemp);

	//int iTextWidth = pLayer2D->CalcTextWidth(szTemp,iTexLen);
	//int iTextHeight= pLayer2D->GetFontHeight();

	//int iWidth = iTextWidth + 16;	

	//int iStartX = 200;
	//int iStartY = pInterface->GetWndHeight() - iTextHeight - 4;

 //   pLayer2D->DrawSolidQuad(iStartX - 2,iStartY ,iTextWidth + 8,iTextHeight + 2 ,0x80bbc5ff);

	//pLayer2D->OutPutText(iStartX + 1,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX - 1,iStartY - 1,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX + 1,iStartY - 1,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX - 1,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX + 1,iStartY,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX - 1,iStartY,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	//pLayer2D->OutPutText(iStartX,iStartY - 1,szTemp,iTexLen,FWC_BLACK);  
	//sprintf(szTemp,"[#a080FF80Q#FFFFFFFF]:#FFFFFF44%s",pszSecurityEditStatusName[m_eEditStatus]);
	//pLayer2D->TextOutEx(iStartX,iStartY,szTemp,lstrlen(szTemp));
}

