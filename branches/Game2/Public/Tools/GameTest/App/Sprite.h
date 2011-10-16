


#ifndef _Sprite_H_
#define _Sprite_H_
#pragma once
#include "../Utility/gtDef.h"



class CSprite
{
public:
	CSprite(void);
	~CSprite(void);

	void Release();

	bool DecodeFromDataBlock(DBReadSet& dbRead);

private:
	tagAutoPixie	m_Pixie;
	tagAutoSupplyData	m_AutoSupplySetup[AutoSupplyTypeNum];
	tagAutoSupplyData	m_AutoSupplySetupEx[AutoSupplyTypeNum];
	tagSetupOnServer				m_SetupOnServer;		//�Զ���ҩ����������
};
#endif//_Sprite_H_