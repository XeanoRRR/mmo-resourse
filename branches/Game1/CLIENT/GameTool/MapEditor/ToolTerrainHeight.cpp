//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��8�� 11:19:57
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#include "StdAfx.h"
#include "..\..\frostengine\ui.h"

#include "toolterrainheight.h"

#include "TerrainMesh.h"
#include "EditWnd.h"
#include "CommUI.h"
#include "CommDB.h"
#include "CmdMapTerrainRaiseHeight.h"
#include "CmdMgr.h"
#include "WndTerrainHeightEdit.h"

ToolTerrainHeight::ToolTerrainHeight(void)
{
}

ToolTerrainHeight::~ToolTerrainHeight(void)
{
}

//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::MessageProc
// Description     : 
// Return type     : void 
// Argument        : BOOL bActive
//---------------------------------------------------------------------------------
void ToolTerrainHeight::MessageProc(BOOL bActive)
{
	//�л�
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();

	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	if (pTerrainMesh)
	{
		render::Interface *pInterface = render::Interface::GetInstance();
		render::Interface::Layer3D *pLayer3D = pInterface->GetLayer3D();
		
		EditWnd* pEditWnd = CommUI::GetInstance()->GetEditWnd();
		WndTerrainHeightEdit *pWndHeightEdit = pEditWnd->GetWndTerrainHeightEdit();

		//render::Interface::Layer3D::ShaderGroup *pShaderGroup = pLayer3D->GetShaderGroup(pWndHeightEdit->GetShaderGroupHandle());
		//DWORD dwTextureCount = pShaderGroup->GetTextureCount(0);

		static BOOL bOnFocus = FALSE;
		if(pInputInfo->eType == UIMT_KB_KEYDOWN)
		{
			static BOOL bChange = TRUE;
			switch(pInputInfo->dwData)
			{
			case DIK_LBRACKET:                 // [
				if(m_dwBrushSize > 0)      // ��С��ˢ
				{
					m_dwBrushSize--;
					bOnFocus = FALSE;
				}
				break;
			case DIK_RBRACKET:                 // ]
				if(m_dwBrushSize < 4)      // ����ˢ
				{
					m_dwBrushSize++;
					bOnFocus = FALSE;
				}
				break;
			case DIK_B:                 // T
				{
					if(bChange)
						pTerrainMesh->SetBrushType(BT_QUAD);    // ���û�ˢΪ����
					else
						pTerrainMesh->SetBrushType(BT_CIRCLE);  // ���û�ˢΪԲ��
					bChange = !bChange;
				}
				break;
			default:
				break;
			}
		}

		if (bActive)
		{
			if(!bOnFocus)
			{
				bOnFocus = TRUE;
				switch(m_dwBrushSize)
				{
				case 0:
					pTerrainMesh->SetBrushSize(BS_TINY);
					break;
				case 1:
					pTerrainMesh->SetBrushSize(BS_SMALL);
					break;
				case 2:
					pTerrainMesh->SetBrushSize(BS_MEDIUM);
					break;
				case 3:
					pTerrainMesh->SetBrushSize(BS_LARGE);
					break;
				case 4:
					pTerrainMesh->SetBrushSize(BS_HUGE);
					break;
				default:
					break;
				}
			}

			if ((pInputInfo->eType == UIMT_MS_MOVE || pInputInfo->eType == UIMT_MS_BTNDOWN) && pInputInfo->dwData & MK_LBUTTON)
			{
				if(pTerrainMesh->ProcessLBDown(pEditWnd->GetCamera(), FALSE))  // ����������Ч
				{
					CmdMapTerrainRaiseHeight* pCmd = new CmdMapTerrainRaiseHeight;
					TerrainMesh::_tagProcessData* pData = new TerrainMesh::_tagProcessData;
					
					pTerrainMesh->GetProcessData(pData);
					pCmd->SetProcessData(pData);
					pCmd->SetEditStatusType(m_eEditStatus);

					CmdMgr::GetInstance()->Do(pCmd);

					SAFEDELETE(pData);
				}
			}
		}
		else
			bOnFocus = FALSE;
	}
}

//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::Render
// Description     : 
// Return type     : void 
//---------------------------------------------------------------------------------
void ToolTerrainHeight::Render()
{
	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	pTerrainMesh->DrawBrush();

	ShowStatus();
}

//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::Create
// Description     : 
// Return type     : void 
//---------------------------------------------------------------------------------
void ToolTerrainHeight::Create()
{
	m_eEditStatus = HEST_RAISE;
	m_dwBrushSize = 0;
}

//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::Destroy
// Description     : 
// Return type     : void 
// Argument        : void
//---------------------------------------------------------------------------------
void ToolTerrainHeight::Destroy(void)
{
}

//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::SetEditStatue
// Description     : ���ñ༭״̬
// Return type     : VOID 
// Argument        : HEIGHTEDITSTATUETYPE flag
//---------------------------------------------------------------------------------
VOID ToolTerrainHeight::SetEditStatue(HEIGHTEDITSTATUETYPE flag)
{
	m_eEditStatus = flag;
}



//---------------------------------------------------------------------------------
// Function name   : ToolTerrainHeight::ShowStatus
// Description     : ��ʾ�༭״̬
// Return type     : VOID 
// Argument        : void
//---------------------------------------------------------------------------------
VOID ToolTerrainHeight::ShowStatus(void)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D   *pLayer2D   = pInterface->GetLayer2D();

	TerrainMesh *pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	TerrainMesh::_tagGrid *pGridList = pTerrainMesh->GetGridList();
	TerrainMesh::_tagVertex *pVerList = pTerrainMesh->GetVertexList();
	TerrainMesh::_tagProcessData* pData = new TerrainMesh::_tagProcessData;
	pTerrainMesh->GetProcessData(pData);

	DWORD vindex = pGridList[pData->dwSelectedGridIndex].dwVertexIndex[pData->dwSelectedVertexIndex];

	if(vindex > pTerrainMesh->GetVertexNum())
		vindex = 0;

	SAFEDELETE(pData);

	//DRAW �༭״̬
	static char szTemp[256];
	static TCHAR szStatus[16];
	switch(m_eEditStatus)
	{
	case HEST_RAISE:
		sprintf(szStatus, "���");
		break;
	case HEST_LOWER:
		sprintf(szStatus, "ѹ��");
		break;
	case HEST_PLATEAU:
		sprintf(szStatus, "ƽ̨");
		break;
	case HEST_NOISE:
		sprintf(szStatus, "���");
		break;
	case HEST_SMOOTH:
		sprintf(szStatus, "ƽ��");
		break;
	default:
		break;
	}

	sprintf(szTemp,"[Q,W]:%s [[,]]:��С [B]:��״ ��ǰ�߶�:%f", szStatus, pVerList[vindex].vPosition.y);
	int iTexLen = lstrlen(szTemp);

	int iTextWidth = pLayer2D->CalcTextWidth(szTemp,iTexLen);
	int iTextHeight= pLayer2D->GetFontSize();

	int iWidth = iTextWidth + 16;	

	int iStartX = 300;
	int iStartY = pInterface->GetWndHeight() - iTextHeight - 4;

	pLayer2D->DrawSolidQuad(iStartX - 2,iStartY ,iTextWidth + 8,iTextHeight + 2 ,FWC_BACK);
	ui::Wnd::SetUIRendState();
	pLayer2D->TextOut(iStartX + 1,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX - 1,iStartY - 1,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX + 1,iStartY - 1,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX - 1,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX + 1,iStartY,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX - 1,iStartY,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX,iStartY + 1,szTemp,iTexLen,FWC_BLACK);
	pLayer2D->TextOut(iStartX,iStartY - 1,szTemp,iTexLen,FWC_BLACK);  
	sprintf(szTemp,"[#ffff4444Q#FFFFFFFF,#ffff4444W#FFFFFFFF]:#ff88ffff%s#FFFFFFFF [#ffff4444E#FFFFFFFF,#ffff4444R#FFFFFFFF]:��С [#ffff4444T#FFFFFFFF]:��״ #ffff4444��ǰ�߶�#FFFFFFFF:#ff88ffff%f#FFFFFFFF",szStatus, pVerList[vindex].vPosition.y);
	pLayer2D->TextOutEx(iStartX,iStartY,szTemp,lstrlen(szTemp));
}