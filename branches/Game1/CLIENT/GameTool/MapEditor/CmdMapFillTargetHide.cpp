#include "StdAfx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"

#include "commdb.h"
#include "map.h"
#include "CmdMapFillTargetHide.h"

CmdMapFillTargetHide::CmdMapFillTargetHide(void)
{
	m_pOldTargetHideGridIndexArray = NULL;
}

CmdMapFillTargetHide::~CmdMapFillTargetHide(void)
{
	SAFEDELETEARRAY(m_pOldTargetHideGridIndexArray);
}

void CmdMapFillTargetHide::AddGridIndex(DWORD dwGridIndex)
{
	m_vGridIndexArray.push_back(dwGridIndex);
	m_pOldTargetHideGridIndexArray = new DWORD[m_vGridIndexArray.size()];
}

void CmdMapFillTargetHide::Execute()
{
	Cmd::Execute();
	//�������
	EditMap *pMap = CommDB::GetInstance()->GetMap();

	Grid * pMapGrid(NULL);
	if (m_dwTargetHideGridIndex != 0xFFFFFFFF)
	{
		
		pMapGrid = pMap->GetGrid(m_dwTargetHideGridIndex);

		Grid::tagBuild *pBuild = pMapGrid->GetBuild();
		for(DWORD n = 0; n < (DWORD)m_vGridIndexArray.size();n++)
		{
			pMapGrid = pMap->GetGrid(m_vGridIndexArray[n]);
			
			m_pOldTargetHideGridIndexArray[n] = pMapGrid->GetTargetHideGridIndex();

			//���������������Ӱ�죬ͬʱҪ��ԭ���Ľ�����ȥ���������
			if (m_pOldTargetHideGridIndexArray[n] != 0xFFFFFFFF)
			{
				Grid *pOldGrid = pMap->GetGrid(m_pOldTargetHideGridIndexArray[n]);
				Grid::tagBuild *pOldBuild = pOldGrid->GetBuild();
				pOldBuild->RemoveHideFlagGridIndex(m_vGridIndexArray[n]);
			}
			
			pMapGrid->SetTargetHideGridIndex(m_dwTargetHideGridIndex);
			//���
			pBuild->AddHideFlagGridIndex(m_vGridIndexArray[n]);
		}
	}
	else
	{
		//ɾ��
		for(DWORD n = 0; n < (DWORD)m_vGridIndexArray.size();n++)
		{
			pMapGrid = pMap->GetGrid(m_vGridIndexArray[n]);
			m_pOldTargetHideGridIndexArray[n] = pMapGrid->GetTargetHideGridIndex();
			pMapGrid->SetTargetHideGridIndex(m_dwTargetHideGridIndex);

			//���������������Ӱ�죬ͬʱҪ��ԭ���Ľ�����ȥ���������
			if (m_pOldTargetHideGridIndexArray[n] != 0xFFFFFFFF)
			{
				Grid *pOldGrid = pMap->GetGrid(m_pOldTargetHideGridIndexArray[n]);
				Grid::tagBuild *pOldBuild = pOldGrid->GetBuild();
				pOldBuild->RemoveHideFlagGridIndex(m_vGridIndexArray[n]);
			}
		}
	}
}

void CmdMapFillTargetHide::UnExecute()
{
	Cmd::UnExecute();
	//�ָ�����
	EditMap *pMap = CommDB::GetInstance()->GetMap();

	Grid * pMapGrid(NULL);
	if (m_dwTargetHideGridIndex != 0xFFFFFFFF)
	{
		
		pMapGrid = pMap->GetGrid(m_dwTargetHideGridIndex);
		Grid::tagBuild *pBuild = pMapGrid->GetBuild();

		for(DWORD n = 0; n < (DWORD)m_vGridIndexArray.size();n++)
		{
			pMapGrid = pMap->GetGrid(m_vGridIndexArray[n]);
			pMapGrid->SetTargetHideGridIndex(m_pOldTargetHideGridIndexArray[n]);
			pBuild->RemoveHideFlagGridIndex(m_vGridIndexArray[n]);

			//���������������Ӱ�죬ͬʱҪ��ԭ���Ľ�������ӻ��������
			if (m_pOldTargetHideGridIndexArray[n] != 0xFFFFFFFF)
			{
				Grid *pOldGrid = pMap->GetGrid(m_pOldTargetHideGridIndexArray[n]);
				Grid::tagBuild *pOldBuild = pOldGrid->GetBuild();
				pOldBuild->AddHideFlagGridIndex(m_vGridIndexArray[n]);
			}
		}
	}
	else
	{
		for(DWORD n = 0; n < (DWORD)m_vGridIndexArray.size();n++)
		{
			pMapGrid = pMap->GetGrid(m_vGridIndexArray[n]);
			pMapGrid->SetTargetHideGridIndex(m_pOldTargetHideGridIndexArray[n]);

			//���������������Ӱ�죬ͬʱҪ��ԭ���Ľ�������ӻ��������
			if (m_pOldTargetHideGridIndexArray[n] != 0xFFFFFFFF)
			{
				Grid *pOldGrid = pMap->GetGrid(m_pOldTargetHideGridIndexArray[n]);
				Grid::tagBuild *pOldBuild = pOldGrid->GetBuild();
				pOldBuild->AddHideFlagGridIndex(m_vGridIndexArray[n]);
			}
		}
	}
}

void CmdMapFillTargetHide::Dump()
{
	OutputConsole("Map_Set_Target_Visible:\n");
}