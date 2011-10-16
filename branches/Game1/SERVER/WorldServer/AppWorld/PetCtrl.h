// Fox@20081118----------------------------------------------
// File:    PetCtrl.h
// Brief:   CPetCtrl���ڿ��Ƴ���AI.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.18, 2008
// Fox@20081118----------------------------------------------

#include "../public/BasePetServerDef.h"

#pragma once

class CEntityGroup;
class CPlayer;
class CMonster;
struct tagDataBlockWriteSet;

class CPetCtrl
{
public:
	CPetCtrl(void);
	virtual ~CPetCtrl(void);

public:
	long GetFuncSwitch(void) { return m_ConfPet.nFuncSwitch; }
	bool LoadConfig(void);									// ��������
	bool CodeToDataBlock(DBWriteSet& setWriteDB);			// ���ñ���

	bool CodeToDataBlock(CPlayer* pPlayer,
		CEntityGroup* pDBPetList);							// WS==>DB����
	bool DecodeFromDataBlock(CEntityGroup* pDBPetList,
		CPlayer* pPlayer);									// DB==>WS����

	bool CodeToDataBlock(DBWriteSet& setWriteDB,
		CPlayer* pPlayer);									// WS==>GS����
	bool DecodeFromDataBlock(DBReadSet& setReadDB,
		CPlayer* pPlayer);									// GS==>GS����

private:
	tagPetConf		m_ConfPet;								// ����ϵͳͳһ����
	GROW_CONF		m_GrowConfPets;							// ����ɳ�
	PET_LIST		m_ConfPets;								// �����б�����
};