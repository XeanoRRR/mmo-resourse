//////////////////////////////////////////////////////////////////////////
//worldvillageregion.h
//��սս��ͼ�࣬�����ս�����ݴ���
//Author��Joe
//Create Date:2005.7.5
//////////////////////////////////////////////////////////////////////////


#pragma once
#include "worldwarregion.h"

class CWorldVillageRegion :
	public CWorldWarRegion
{
public:
	CWorldVillageRegion(void);
	~CWorldVillageRegion(void);

public:
	BOOL Load();	// ��ȡ
	void SetEnterPosXY(CPlayer* pPlayer);
};
