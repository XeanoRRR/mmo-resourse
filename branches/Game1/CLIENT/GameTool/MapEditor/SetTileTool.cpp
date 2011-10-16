#include "StdAfx.h"
#include "..\..\frostengine\ui.h"
#include "settiletool.h"
#include "terrainmesh.h"
#include "editwnd.h"
#include "commui.h"
#include "commdb.h"
#include "cmdmapterrainsettile.h"
#include "cmdmgr.h"
#include "terraintilesetwnd.h"

ToolSetTile::ToolSetTile(void)
{
}

ToolSetTile::~ToolSetTile(void)
{
	ASSERT(!m_pData);
}

void ToolSetTile::MessageProc(BOOL bActive)
{
	//�л�
	ui::Manager::_tagINPUTINFO *pInputInfo = ui::Manager::GetInstance()->GetInputInfoBuffer();
	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	if (pTerrainMesh)
	{
		EditWnd* pEditWnd = CommUI::GetInstance()->GetEditWnd();
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
			case DIK_B:                 // B
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
				switch(m_eEditStatus)
				{
				case TEST_IDLE:
					break;
				case TEST_PLACE:   // ����
					{   
						//TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
						if(pTerrainMesh->ProcessLBDown(pEditWnd->GetCamera()))  // ����������Ч
						{
							CmdMapTerrainSetTile* pCmd = new CmdMapTerrainSetTile;

							TerrainMesh::_tagProcessData* pData = new TerrainMesh::_tagProcessData;
							pTerrainMesh->GetProcessData(pData);
							pCmd->SetProcessData(pData);
							if(CompareProcessData(pData))
							{
								pCmd->Execute();
								SAFEDESTROY(pCmd);
							}
							else
							{
								CmdMgr::GetInstance()->Do(pCmd);
							}
							SAFEDELETE(pData);
						}
					}
					break;
				}
			}
		}
		else
			bOnFocus = FALSE;
	}
}

void ToolSetTile::Render()
{
	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	pTerrainMesh->DrawBrush();

	ShowStatus();
}

void ToolSetTile::Create()
{
	m_dwBrushSize = 0;
	m_eEditStatus = TEST_PLACE;
	m_pData = new TerrainMesh::_tagProcessData[1];
}

void ToolSetTile::Destroy(void)
{
	SAFEDELETE(m_pData);
}


//---------------------------------------------------------------------------------
// Function name   : SetTileTool::SetEditStatue
// Description     : ���ñ༭״̬
// Return type     : VOID 
// Argument        : TILEEDITSTATUETYPE flag
//---------------------------------------------------------------------------------
VOID ToolSetTile::SetEditStatue(TILEEDITSTATUETYPE flag)
{
	m_eEditStatus = flag;
}



//---------------------------------------------------------------------------------
// Function name   : SetTileTool::CompareProcessData
// Description     : �Ƚϲ�����������ͬʱ����ΪpData��
// Return type     : BOOL    : ��ͬ����TRUE
// Argument        : TerrainMesh::_tagProcessData* pData
//---------------------------------------------------------------------------------
BOOL ToolSetTile::CompareProcessData(TerrainMesh::_tagProcessData* pData)
{
	if(memcmp(m_pData, pData, sizeof(TerrainMesh::_tagProcessData)))
	{
		memcpy(m_pData, pData, sizeof(TerrainMesh::_tagProcessData));
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



//---------------------------------------------------------------------------------
// Function name   : SetTileTool::ShowStatus
// Description     : ��ʾ�༭״̬
// Return type     : VOID 
// Argument        : void
//---------------------------------------------------------------------------------
void ToolSetTile::ShowStatus(void)
{
	render::Interface *pInterface = render::Interface::GetInstance();
	render::Interface::Layer2D   *pLayer2D   = pInterface->GetLayer2D();

	//DRAW �༭״̬
	static char szTemp[256];
	int iTextHeight= pLayer2D->GetFontSize();

	int iStartX = 300;
	int iStartY = pInterface->GetWndHeight() - iTextHeight - 4;

	sprintf(szTemp,"[#ff89C337ENTER#FFFFFFFF]:ͼ�� [#ff89C337[,]#FFFFFFFF]:��С [#ff89C337B#FFFFFFFF]:��״");
	ui::Wnd::SetUIRendState();
	pLayer2D->TextOutEx(iStartX,iStartY,szTemp,lstrlen(szTemp));
}