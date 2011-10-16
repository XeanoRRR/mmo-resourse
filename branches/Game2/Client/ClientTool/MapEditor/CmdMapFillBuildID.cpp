
#include "StdAfx.h"

#include "..\..\engine\utility.h"
#include "..\..\engine\console.h"
#include "commdb.h"
#include "map.h"
#include "cmdmapfillBuildID.h"

CmdMapFillBuild::CmdMapFillBuild(void)
{
	m_pOldGridBuildArray = NULL;
}

CmdMapFillBuild::~CmdMapFillBuild(void)
{
	SAFEDELETEARRAY(m_pOldGridBuildArray);
}

void CmdMapFillBuild::SetGridIndexCount(DWORD dwCount)
{
	m_dwGridIndexCount = dwCount;
	m_pOldGridIndexArray  =  new DWORD[dwCount];
	m_pOldGridBuildArray = new Grid::tagBuild[dwCount];
}

void CmdMapFillBuild::SetGridIndex(DWORD dwIndex,DWORD dwGridIndex)
{
	m_pOldGridIndexArray[dwIndex] = dwGridIndex;
}

void CmdMapFillBuild::SetBuild(const Grid::tagBuild &tBuild)
{
	m_tBuild = tBuild;
}

bool CmdMapFillBuild::Execute()
{
	Cmd::Execute();

	//�������
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(DWORD n = 0; n < m_dwGridIndexCount; n++)
	{
		DWORD dwGridIndex = m_pOldGridIndexArray[n];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		
		//����ɵ�
		m_pOldGridBuildArray[n] = * pGrid->GetBuild();

		if (m_tBuild.wID != 0xFFFF)
		{
			//set to build
			pMap->AddPalaceBuildCount();
			pMap->AddRefenceBuildCount(m_tBuild.wID);
			
		}
		else
		{
			//ɾ����ص�grid�ݶ���־
			Grid::tagBuild *pGridBuild = pGrid->GetBuild();
			for(DWORD n = 0; n < (DWORD)pGridBuild->m_vHideFlagGridIndexArray.size(); n++)
			{
				DWORD dwGridIndex = pGridBuild->m_vHideFlagGridIndexArray[n];
				Grid *pTempGrid = pMap->GetGrid(dwGridIndex);
				pTempGrid->SetTargetHideGridIndex(0xFFFFFFFF);
				//������ӱ�����룬�ṩ�ָ�����
			}
			pMap->SubPalaceBuildCount();
			pMap->SubRefenceBuildCount(pGrid->GetBuild()->wID);
		}

        pGrid->SetBuild(m_tBuild);		
	}

	return true;
}

void CmdMapFillBuild::UnExecute()
{
	Cmd::UnExecute();
	//�ָ�����
	Grid::tagBuild tBuild;
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(DWORD n = 0; n < m_dwGridIndexCount; n++)
	{
		DWORD dwGridIndex = m_pOldGridIndexArray[n];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		
		tBuild = m_pOldGridBuildArray[n];

		if (tBuild.wID != 0xFFFF)
		{
			//set to build
			pMap->AddPalaceBuildCount();
			pMap->AddRefenceBuildCount(tBuild.wID);
		}
		else
		{
			pMap->SubPalaceBuildCount();
			pMap->SubRefenceBuildCount(pGrid->GetBuild()->wID);
		}

		pGrid->SetBuild(tBuild);
	}
}

void CmdMapFillBuild::Dump()
{
	OutputConsole("Map_Fill_Build:\n");
}

