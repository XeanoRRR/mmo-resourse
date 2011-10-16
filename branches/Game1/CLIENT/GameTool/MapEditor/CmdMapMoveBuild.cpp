#include "stdafx.h"

#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "..\..\gameengine\gamebuildlist.h"

#include "cmdmapmovebuild.h"
#include "commdb.h"
#include "map.h"
#include "editWnd.h"

#include "toolbuildedit.h"
#include "commui.h"


CmdMapMoveBuild::CmdMapMoveBuild(void)
{
}

CmdMapMoveBuild::~CmdMapMoveBuild(void)
{
}

void CmdMapMoveBuild::SetBuildData(DWORD dwDstIndex,DWORD dwSrcIndex)
{
	m_dwSrcGridIndex = dwSrcIndex;
	m_dwDstGridIndex = dwDstIndex; 
}

void CmdMapMoveBuild::Execute(void)
{
	Cmd::Execute();
	//��������
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	Grid * pGrid = pMap->GetGrid(m_dwSrcGridIndex);

	int iMapWidth = pMap->GetWidth();
	int iMapDepth = pMap->GetDepth();
	EditWnd *pEditWnd = CommUI::GetInstance()->GetEditWnd();
	ToolBuildEdit *pToolBuildEdit = pEditWnd->GetToolBuildEdit();
	GameBuildList *pGameBuildList = GameBuildList::GetInstance();
	GameBuildList::Build *pBuild = pGameBuildList->AcquireBuild(pGrid->GetBuild()->wID,false);
//	GameBuildList::Build::tagBlockData *pBlockData = pBuild->GetBlockData();

	Grid::tagBuild tSaveBuild;
	
	tSaveBuild = *pGrid->GetBuild();

	//���ԭ��
	Grid::tagBuild tBuild;
	tBuild.wID		= 0xFFFF;
	tBuild.fHeight  = 0.0f;
	tBuild.fRotationY = 0.0f;
	pGrid->SetBuild(tBuild);
	//���ԭ���ϰ�
	//int iCX = (int)m_dwSrcGridIndex % iMapWidth;
	//int iCZ = (int)m_dwSrcGridIndex / iMapWidth;
	//if (pBlockData)
	//{
	//	for(int i = pBlockData->iXMin; i <= pBlockData->iXMax; i++)
	//	{
	//		for(int j = pBlockData->iZMin; j <= pBlockData->iZMax; j++)
	//		{
	//			//�����Χ�ĸ���
	//			int iX = i + iCX;
	//			int iZ = j + iCZ;
	//			if (!(iX < 0 || iZ < 0 || iX >= iMapWidth || iZ >= iMapDepth))
	//			{
	//				Grid *pTempGrid = pMap->GetGrid(iX,iZ);
	//				pTempGrid->SetBlock(0);	//0 none 1 normal 2 high
	//			}
	//		}
	//	}
	//}

	//�����µ��ϰ�
	//iCX = (int)m_dwDstGridIndex % iMapWidth;
	//iCZ = (int)m_dwDstGridIndex / iMapWidth;
	//pBuild = pGameBuildList->GetBuild(tSaveBuild.wID);
	//pBlockData = pBuild->GetBlockData();
	//if (pBlockData)
	//{
	//	for(int i = pBlockData->iXMin; i <= pBlockData->iXMax; i++)
	//	{
	//		for(int j = pBlockData->iZMin; j <= pBlockData->iZMax; j++)
	//		{
	//			//�����Χ�ĸ���
	//			int iX = i + iCX;
	//			int iZ = j + iCZ;
	//			if (!(iX < 0 || iZ < 0 || iX >= iMapWidth || iZ >= iMapDepth))
	//			{
	//				Grid *pTempGrid = pMap->GetGrid(iX,iZ);
	//				pTempGrid->SetBlock(pBlockData->byType + 1);
	//			}
	//		}
	//	}
	//}  

	//�����µ�
	pGrid = pMap->GetGrid(m_dwDstGridIndex);
	pGrid->SetBuild(tSaveBuild);

	//�������ýҿ����ı�־���Ӷ�Ӧ�Ľ������ڸ���
	Grid::tagBuild *pTempBuild = pGrid->GetBuild();
	for(DWORD n = 0; n < (DWORD)pTempBuild->m_vHideFlagGridIndexArray.size();n++)
	{
		DWORD dwGridIndex = pTempBuild->m_vHideFlagGridIndexArray[n];
		Grid *pMapGrid = pMap->GetGrid(dwGridIndex);
		pMapGrid->SetTargetHideGridIndex(m_dwDstGridIndex);
	}
}

void CmdMapMoveBuild::UnExecute(void)
{
	Cmd::UnExecute();
	//��������
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	Grid * pGrid = pMap->GetGrid(m_dwDstGridIndex);

	int iMapWidth = pMap->GetWidth();
	int iMapDepth = pMap->GetDepth();
	EditWnd *pEditWnd = CommUI::GetInstance()->GetEditWnd();
	ToolBuildEdit *pToolBuildEdit = pEditWnd->GetToolBuildEdit();
	GameBuildList *pGameBuildList = GameBuildList::GetInstance();
	GameBuildList::Build *pBuild = pGameBuildList->AcquireBuild(pGrid->GetBuild()->wID,false);
//	GameBuildList::Build::tagBlockData *pBlockData = pBuild->GetBlockData();


	Grid::tagBuild tSaveBuild;

	tSaveBuild = * pGrid->GetBuild();

	//���ԭ��
	Grid::tagBuild tBuild;
	tBuild.wID		= 0xFFFF;
	tBuild.fHeight  = 0.0f;
	tBuild.fRotationY = 0.0f;
	pGrid->SetBuild(tBuild);
	//���ԭ���ϰ�
	//int iCX = (int)m_dwDstGridIndex % iMapWidth;
	//int iCZ = (int)m_dwDstGridIndex / iMapWidth;
	//if (pBlockData)
	//{
	//	for(int i = pBlockData->iXMin; i <= pBlockData->iXMax; i++)
	//	{
	//		for(int j = pBlockData->iZMin; j <= pBlockData->iZMax; j++)
	//		{
	//			//�����Χ�ĸ���
	//			int iX = i + iCX;
	//			int iZ = j + iCZ;
	//			if (!(iX < 0 || iZ < 0 || iX >= iMapWidth || iZ >= iMapDepth))
	//			{
	//				Grid *pTempGrid = pMap->GetGrid(iX,iZ);
	//				pTempGrid->SetBlock(0);	//0 none 1 normal 2 high
	//			}
	//		}
	//	}
	//}

	//�����µ��ϰ�
	//iCX = (int)m_dwSrcGridIndex % iMapWidth;
	//iCZ = (int)m_dwSrcGridIndex / iMapWidth;
	//pBuild = pGameBuildList->GetBuild(tSaveBuild.wID);
	//pBlockData = pBuild->GetBlockData();
	//if (pBlockData)
	//{
	//	for(int i = pBlockData->iXMin; i <= pBlockData->iXMax; i++)
	//	{
	//		for(int j = pBlockData->iZMin; j <= pBlockData->iZMax; j++)
	//		{
	//			//�����Χ�ĸ���
	//			int iX = i + iCX;
	//			int iZ = j + iCZ;
	//			if (!(iX < 0 || iZ < 0 || iX >= iMapWidth || iZ >= iMapDepth))
	//			{
	//				Grid *pTempGrid = pMap->GetGrid(iX,iZ);
	//				pTempGrid->SetBlock(pBlockData->byType + 1);
	//			}
	//		}
	//	}
	//}  

	//����ԭ����
	pGrid = pMap->GetGrid(m_dwSrcGridIndex);
	pGrid->SetBuild(tSaveBuild);
	//�������ýҿ����ı�־���Ӷ�Ӧ�Ľ������ڸ���
	Grid::tagBuild *pTempBuild = pGrid->GetBuild();
	for(DWORD n = 0; n < (DWORD)pTempBuild->m_vHideFlagGridIndexArray.size();n++)
	{
		DWORD dwGridIndex = pTempBuild->m_vHideFlagGridIndexArray[n];
		Grid *pMapGrid = pMap->GetGrid(dwGridIndex);
		pMapGrid->SetTargetHideGridIndex(m_dwSrcGridIndex);
	}
}

void CmdMapMoveBuild::Dump(void)
{
	OutputConsole("Map_Move_Build: %d %d",m_dwDstGridIndex,m_dwSrcGridIndex);
}		