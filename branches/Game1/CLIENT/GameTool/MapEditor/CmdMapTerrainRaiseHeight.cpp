//---------------------------------------------------------------------------------
// ����     : ����
// ����ʱ�� : 2004��1��9�� 10:29:31
// ����ʱ�� : 
// ժҪ     :
//---------------------------------------------------------------------------------
#include "StdAfx.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"

#include ".\Cmdmapterrainraiseheight.h"

#include "CommDB.h"
#include "EditWnd.h"
#include "CommUI.h"

CmdMapTerrainRaiseHeight::CmdMapTerrainRaiseHeight(void)
{
	m_ptProcessData   = NULL;
}

CmdMapTerrainRaiseHeight::~CmdMapTerrainRaiseHeight(void)
{
	SAFEDELETE(m_ptProcessData);
	m_vOldVertexArray.clear();
}

void CmdMapTerrainRaiseHeight::Execute(void)
{
	Cmd::Execute();

	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();

	static TerrainMesh::_tagProcessData pCurrentData;
	// ���浱ǰ�Ĳ�������
	pTerrainMesh->GetProcessData(&pCurrentData);

	pTerrainMesh->SetProcessData(m_ptProcessData);

	static FLOAT fOffset = 0.05f;
	switch(m_eEditStatus)
	{
	case HEST_IDLE:
		break;
	case HEST_RAISE:     // ���
		{
			pTerrainMesh->ModifyTerrainHeight(m_vOldVertexArray, m_ptProcessData->dwSelectedGridIndex, 
				                              m_ptProcessData->dwSelectedVertexIndex, fOffset, ET_ADD);
		}
		break;
	case HEST_LOWER:     // ѹ��
		{
			pTerrainMesh->ModifyTerrainHeight(m_vOldVertexArray, m_ptProcessData->dwSelectedGridIndex, 
				                              m_ptProcessData->dwSelectedVertexIndex, -fOffset, ET_ADD);
		}
		break;
	case HEST_PLATEAU:   // ƽ̨
		{
			pTerrainMesh->ModifyTerrainHeight(m_vOldVertexArray, m_ptProcessData->dwSelectedGridIndex, 
				                              m_ptProcessData->dwSelectedVertexIndex, 0.0f, ET_REPLACE);
		}
		break;
	case HEST_NOISE:     // ���
		{
			pTerrainMesh->ModifyTerrainHeight(m_vOldVertexArray, m_ptProcessData->dwSelectedGridIndex, 
				                              m_ptProcessData->dwSelectedVertexIndex, 0.0f, ET_NOISE);
		}
		break;
	case HEST_SMOOTH:    // ƽ��
		{
			pTerrainMesh->ModifyTerrainHeight(m_vOldVertexArray, m_ptProcessData->dwSelectedGridIndex, 
				                              m_ptProcessData->dwSelectedVertexIndex, 0.0f, ET_SMOOTH);
		}
		break;
	default:
		break;				
	}

	// ��ص�ǰ�Ĳ�������
	pTerrainMesh->SetProcessData(&pCurrentData);
}

void CmdMapTerrainRaiseHeight::UnExecute(void)
{
	Cmd::UnExecute();

	TerrainMesh* pTerrainMesh = CommDB::GetInstance()->GetTerrainMesh();
	static TerrainMesh::_tagProcessData pCurrentData;

	// ���浱ǰ�Ĳ�������
	pTerrainMesh->GetProcessData(&pCurrentData);
	// ������һ�β���ʱ�Ĳ�������
	pTerrainMesh->SetProcessData(m_ptProcessData);
	// �ָ���һ�β���ǰ����������
	pTerrainMesh->FillVertex(m_vOldVertexArray, FALSE);
	// ��ص�ǰ�Ĳ�������
	pTerrainMesh->SetProcessData(&pCurrentData);
}

void CmdMapTerrainRaiseHeight::Dump(void)
{
	// �����������
	OutputConsole("Map_Raise_Height error: %d, %d", m_ptProcessData->dwSelectedGridIndex, m_ptProcessData->dwSelectedVertexIndex);
}




//---------------------------------------------------------------------------------
// Function name   : CmdMapTerrainRaiseHeight::GetProcessData
// Description     : ��ȡ��������
// Return type     : VOID 
// Argument        : TerrainMesh::_tagProcessData* pData
//---------------------------------------------------------------------------------
VOID CmdMapTerrainRaiseHeight::GetProcessData(TerrainMesh::_tagProcessData* pData)
{
	ASSERT(pData);

	if(m_ptProcessData)
		memcpy(pData, m_ptProcessData, sizeof(TerrainMesh::_tagProcessData));
}



//---------------------------------------------------------------------------------
// Function name   : CmdMapTerrainRaiseHeight::SetProcessData
// Description     : ���ò�������
// Return type     : VOID 
// Argument        : TerrainMesh::_tagProcessData* pData
//---------------------------------------------------------------------------------
VOID CmdMapTerrainRaiseHeight::SetProcessData(TerrainMesh::_tagProcessData* pData)
{
	ASSERT(pData);

	if(!m_ptProcessData)
	{
		m_ptProcessData = new TerrainMesh::_tagProcessData;
		memcpy(m_ptProcessData, pData, sizeof(TerrainMesh::_tagProcessData));
	}
}