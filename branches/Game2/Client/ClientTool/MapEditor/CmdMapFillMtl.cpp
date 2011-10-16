#include "StdAfx.h"

#include "..\..\engine\utility.h"
#include "..\..\engine\console.h"

#include "commdb.h"
#include "map.h"
#include "cmdmapfillmtl.h"

CmdMapFillMtl::CmdMapFillMtl(void)
{
	m_pGridOldMtlArray = NULL;
}

CmdMapFillMtl::~CmdMapFillMtl(void)
{
	SAFEDELETEARRAY(m_pGridOldMtlArray);
}

void CmdMapFillMtl::AddGridIndex(DWORD dwGridIndex)
{
	m_vGridIndexArray.push_back(dwGridIndex);
	m_pGridOldMtlArray = new DWORD[m_vGridIndexArray.size()];
}

bool CmdMapFillMtl::Execute()
{
	Cmd::Execute();

	//�������
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(int i = 0; i < (int)m_vGridIndexArray.size(); i++)
	{
		DWORD dwGridIndex = m_vGridIndexArray[i];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		m_pGridOldMtlArray[i] = pGrid->GetMtl();
        pGrid->SetMtl(m_dwMtl);
	}

	return true;
}

void CmdMapFillMtl::UnExecute()
{
	Cmd::UnExecute();
	//�ָ�����
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(int i = 0; i < (int)m_vGridIndexArray.size(); i++)
	{
		DWORD dwGridIndex = m_vGridIndexArray[i];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		DWORD dwOldMtl = m_pGridOldMtlArray[i];
		pGrid->SetMtl(dwOldMtl);
	}
	
}

void CmdMapFillMtl::Dump()
{
	OutputConsole("Map_Fill_Mtl: %d %d",m_vGridIndexArray.size(),m_dwMtl);
}