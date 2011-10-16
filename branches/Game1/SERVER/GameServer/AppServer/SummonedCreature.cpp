#include "stdafx.h"
#include <mmsystem.h>
#include "SummonedCreature.h"
#include "ServerRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSummonedCreature::CSummonedCreature()
{
	m_dwSummonedTimeStamp	= 0;
	m_dwLifeTime			= 0;
}

CSummonedCreature::~CSummonedCreature()
{
}

VOID CSummonedCreature::AI()
{
	//##���������ֱ�ӷ���.
	if( IsDied() )
	{
		Evanish();
		return;
	}

	//##����һ�£�������ʧ���Ǻ��ϲ�AI�й�����
	if( m_dwSummonedTimeStamp )
	{
		if( timeGetTime() > m_dwSummonedTimeStamp + m_dwLifeTime )
		{
			//##��ʧ
			Evanish();
			return;
		}
	}
	//##����Monster��AI
	CMonster::AI();
}

//##��������ʱ��
VOID CSummonedCreature::SetLifeTime( DWORD dwLifeTime )
{
	m_dwSummonedTimeStamp	= timeGetTime();
	m_dwLifeTime			= dwLifeTime;
}

//##��ȡʣ�������ʱ��
DWORD CSummonedCreature::GetRemainedTime()
{
	if( m_dwSummonedTimeStamp )
	{
		if( timeGetTime() >= m_dwSummonedTimeStamp + m_dwLifeTime )
			return 0;
		else 
			return m_dwSummonedTimeStamp + m_dwLifeTime - timeGetTime();
	}
	else 
	{
		return 0xffffffff;
	}
}
