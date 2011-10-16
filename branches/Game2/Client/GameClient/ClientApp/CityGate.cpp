////////////////////////////////////////////////////////////////////////////
//CityGate.cpp/////////////////////////////////////////////////////////////
//������
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include ".\citygate.h"
#include "ClientRegion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCityGate::CCityGate(void)
{
}

CCityGate::~CCityGate(void)
{
}


void CCityGate::AI(void)
{
	CBuild::AI();

	switch( GetAction() )
	{
	case ACT_OPEN:
		if( AI_Open() == RET_AI_BREAK ) return;
		break;
	}
}
// ����AI
long CCityGate::AI_Open()
{
	return RET_AI_CONTINUE;
}


bool CCityGate::Display(void)
{
	CBuild::Display();
	return true;
}

void CCityGate::SetAction(WORD l)
{
	CBuild::SetAction(l);
}

// ����
bool CCityGate::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CBuild::DecordFromByteArray(pByte, pos, bExData);

	//char strVal[256];
	//_GetStringFromByteArray(pByte, pos, strVal);//������Ч
	//_GetStringFromByteArray(pByte, pos, strVal);//������Ч

	return true;
}

bool CCityGate::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData)
{
	CBuild::DecodeFromDataBlock(setReadDB, bExData);
	return true;
}