#include "StdAfx.h"
#include "ftime.h"
#include "../../../public/tools.h"

//CLASS__(time::Clock)___
void time::Clock::Update()
{
	m_dwCurTime = GetCurTickCount();
}


//CLASS__(time::Manager)___
IMPLEMENT_SINGLETON(time::Manager);
void time::Manager::Create()
{
	//�����:
}

void time::Manager::Destroy()
{
	//�����:
}
void time::Manager::Update()
{
	m_cClock.Update();

	//�����:
}

