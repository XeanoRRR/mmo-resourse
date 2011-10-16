#include "StdAfx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"

#include "cmdmapterrainsettile.h"
#include "CommDB.h"
#include "EditWnd.h"
#include "CommUI.h"
#include "TerrainTileSetWnd.h"
#include "WndMiniMap.h"

CmdMapTerrainSetTile::CmdMapTerrainSetTile(void)
{
	m_ptProcessData   = NULL;
}

CmdMapTerrainSetTile::~CmdMapTerrainSetTile(void)
{
	SAFEDELETE(m_ptProcessData);
	m_vOldVertexArray.clear();
}

void CmdMapTerrainSetTile::Execute(void)
{
	Cmd::Execute();

	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();

	static TerrainMesh::_tagProcessData pCurrentData;
	// ���浱ǰ�Ĳ�������
	pTerrainMesh->GetProcessData(&pCurrentData);

	pTerrainMesh->SetProcessData(m_ptProcessData);
	pTerrainMesh->RecalculateGridTexture(m_ptProcessData->dwSelectedGridIndex, m_ptProcessData->dwSelectedVertexIndex, m_vOldVertexArray);

	// ��ص�ǰ�Ĳ�������
	pTerrainMesh->SetProcessData(&pCurrentData);

	EditWnd* pEditWnd = CommUI::GetInstance()->GetEditWnd();
	WndTerrainTileSet *pWndTerrainTileSet = pEditWnd->GetWndTerrainTileSet();
	WndMiniMap *pWndMiniMap = pEditWnd->GetWndMiniMap();
	TerrainMesh::_tagVertex *pVertex;
	DWORD dwColor = pWndTerrainTileSet->GetBrushColor(m_ptProcessData->nCurrentTextureID);
	DWORD dwCount = (DWORD)m_vOldVertexArray.size();
	for(DWORD i = 0; i < dwCount; i++)
	{
		pVertex = &m_vOldVertexArray[i];
		DWORD dwNumGrid = pVertex->dwBelongToNum;
		for(DWORD j = 0; j < dwNumGrid; j++)
		{
			DWORD dwGridIndex = pVertex->dwBelongToGrid[j];
			pWndMiniMap->SetMapPixel(dwGridIndex, dwColor);
		}
	}
	pWndMiniMap->UpdateMiniPixel();
}

void CmdMapTerrainSetTile::UnExecute(void)
{
	Cmd::UnExecute();

	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	static TerrainMesh::_tagProcessData pCurrentData;

	// ���浱ǰ�Ĳ�������
	pTerrainMesh->GetProcessData(&pCurrentData);
	// ������һ�β���ʱ�Ĳ�������
	pTerrainMesh->SetProcessData(m_ptProcessData);
	// �ָ���һ�β���ǰ����������
	pTerrainMesh->FillVertex(m_vOldVertexArray);
	// ��ص�ǰ�Ĳ�������
	pTerrainMesh->SetProcessData(&pCurrentData);

	EditWnd* pEditWnd = CommUI::GetInstance()->GetEditWnd();
	WndTerrainTileSet *pWndTerrainTileSet = pEditWnd->GetWndTerrainTileSet();
	WndMiniMap *pWndMiniMap = pEditWnd->GetWndMiniMap();
	TerrainMesh::_tagVertex *pVertex;
	DWORD dwCount = (DWORD)m_vOldVertexArray.size();
	for(DWORD i = 0; i < dwCount; i++)
	{
		pVertex = &m_vOldVertexArray[i];
		DWORD dwNumGrid = pVertex->dwBelongToNum;
		DWORD dwColor = pWndTerrainTileSet->GetBrushColor(pVertex->byTexID);
		for(DWORD j = 0; j < dwNumGrid; j++)
		{
			DWORD dwGridIndex = pVertex->dwBelongToGrid[j];
			pWndMiniMap->SetMapPixel(dwGridIndex, dwColor);
		}
	}
	pWndMiniMap->UpdateMiniPixel();
}

void CmdMapTerrainSetTile::Dump(void)
{
	// �����������
	OutputConsole("Map_Set_Tile error: %d, %d", m_ptProcessData->dwSelectedGridIndex, m_ptProcessData->dwSelectedVertexIndex);
}




//---------------------------------------------------------------------------------
// Function name   : CmdMapTerrainSetTile::GetProcessData
// Description     : ��ȡ��������
// Return type     : VOID 
// Argument        : TerrainMesh::_tagProcessData* pData
//---------------------------------------------------------------------------------
VOID CmdMapTerrainSetTile::GetProcessData(TerrainMesh::_tagProcessData* pData)
{
	ASSERT(pData);

	if(m_ptProcessData)
		memcpy(pData, m_ptProcessData, sizeof(TerrainMesh::_tagProcessData));
}



//---------------------------------------------------------------------------------
// Function name   : CmdMapTerrainSetTile::SetProcessData
// Description     : ���ò�������
// Return type     : VOID 
// Argument        : TerrainMesh::_tagProcessData* pData
//---------------------------------------------------------------------------------
VOID CmdMapTerrainSetTile::SetProcessData(TerrainMesh::_tagProcessData* pData)
{
	ASSERT(pData);

	if(!m_ptProcessData)
	{
		m_ptProcessData = new TerrainMesh::_tagProcessData;
		memcpy(m_ptProcessData, pData, sizeof(TerrainMesh::_tagProcessData));
	}
}

VOID CmdMapTerrainSetTile::Destroy()
{
	SAFEDELETE(m_ptProcessData);
	m_vOldVertexArray.clear();
}
