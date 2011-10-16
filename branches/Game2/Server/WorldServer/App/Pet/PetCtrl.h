/**
* @file    PetCtrl.h
* @author  Fox (yulefox@gmail.com)
* @date    Nov.18, 2008
* @brief   CPetCtrl���ڿ��Ƴ���AI.
*/

#pragma once

#include "../../../../Public/ServerPublic/Server/BaseDef/BasePetServerDef.h"

class CEntityGroup;
class CPlayer;

class CPetCtrl
{
public:
	CPetCtrl(void);
	~CPetCtrl(void);

public:
	/// �������ÿ���
	long GetFuncSwitch(void) { return m_ConfPet.nFuncSwitch; }

	/// ���س�������
	bool LoadConfig(void);

	/// ���ñ���
	bool CodeToDataBlock(DBWriteSet& setWriteDB);

	/// WS==>DB����
	bool CodeToDataBlock(CPlayer* pPlayer, CEntityGroup* pDBPetList);

	/// DB==>WS����
	bool DecodeFromDataBlock(CEntityGroup* pDBPetList, CPlayer* pPlayer);

	/// WS==>GS����
	bool CodeToDataBlock(DBWriteSet& setWriteDB, CPlayer* pPlayer);

	/// GS==>WS����
	bool DecodeFromDataBlock(DBReadSet& setReadDB, CPlayer* pPlayer);
private:
	/// ��ȡ�����������
	bool LoadPetBaseConfig(void);

	/// ��ȡ���ﾭ������
	bool LoadPetExpConfig(void);

	/// ��ȡ�����б�
	bool LoadPetList(void);

	/// ��ȡ����ɳ�����
	bool LoadPetGrowList(void);

private:
	tagPetConf		m_ConfPet;								///< ����ϵͳͳһ����
	GROW_CONF		m_GrowConfPets;							///< ����ɳ�
	PET_LIST		m_ConfPets;								///< �����б�����
};