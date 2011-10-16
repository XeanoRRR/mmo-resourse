#include "StdAfx.h"
#include "..\..\frostengine\utility.h"
#include "..\..\frostengine\console.h"
#include "commdb.h"
#include "map.h"
#include "CmdMapFillWeatherEffectFlag.h"

CmdMapFillWeatherEffectFlag::CmdMapFillWeatherEffectFlag(void)
{
}

CmdMapFillWeatherEffectFlag::~CmdMapFillWeatherEffectFlag(void)
{
}

void CmdMapFillWeatherEffectFlag::AddGridIndex(DWORD dwGridIndex)
{
	m_vGridIndexArray.push_back(dwGridIndex);

}

void CmdMapFillWeatherEffectFlag::Execute()
{
	Cmd::Execute();
	//�������
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(int i = 0; i < (int)m_vGridIndexArray.size(); i++)
	{
		DWORD dwGridIndex = m_vGridIndexArray[i];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		m_vGridOldWeatherEffectFlagArray.push_back(pGrid->GetWeatherEffectFlag());
		pGrid->SetWeatherEffectFlag(m_bWeatherEffectFlag);
	}
}																

void CmdMapFillWeatherEffectFlag::UnExecute()
{
	Cmd::UnExecute();
	//�ָ�����
	EditMap *pMap = CommDB::GetInstance()->GetMap();
	for(int i = 0; i < (int)m_vGridIndexArray.size(); i++)
	{
		DWORD dwGridIndex = m_vGridIndexArray[i];
		Grid * pGrid = pMap->GetGrid(dwGridIndex);
		DWORD dwOldWeatherEffectFlag = m_vGridOldWeatherEffectFlagArray[i];
		pGrid->SetWeatherEffectFlag(dwOldWeatherEffectFlag);
	}
	m_vGridOldWeatherEffectFlagArray.clear();
}

void CmdMapFillWeatherEffectFlag::Dump()
{
	OutputConsole("Map_Fill_WeatherEffectFlag: %d %d",m_vGridIndexArray.size(),m_bWeatherEffectFlag);
}