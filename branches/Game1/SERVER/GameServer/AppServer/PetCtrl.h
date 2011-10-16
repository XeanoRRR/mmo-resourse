// Fox@20081118----------------------------------------------
// File:    PetCtrl.h
// Brief:   CPetCtrl���ڿ��Ƴ���AI.
//
// Author:  Fox (yulefox@gmail.com)
// Date:    Nov.18, 2008
// Fox@20081118----------------------------------------------

#include "../public/GUID.h"
#include "../public/BasePetServerDef.h"
#include "../../NETS/NetServer/MsgRegister.h"

#pragma once

class CEntityGroup;
class CDataEntityManager;
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
	bool DecodeFromDataBlock(DBReadSet& setReadDB);				// ���ý���

	bool CodeToDataBlockForClient(DBWriteSet& setWriteDB,
		CPlayer* pPlayer);										// GS==>Client����
	bool CodeToDataBlock(DBWriteSet& setWriteDB,
		CPlayer* pPlayer);										// GS==>WS����
	bool DecodeFromDataBlock(DBReadSet& setReadDB,
		CPlayer* pPlayer);										// WS==>GS����

	CPet* GetPet(const CGUID& rID);
	void DelPet(CPet* pPet, long lOpType);						// ɾ��(δ)����
	void ActivatePet(CPet* pPet);								// �������
	bool RelivePet(CPet* pPet, long lInitType);					// �������
	void OnDeletePet(void);										// ����ɾ������

	void GetGrowCoefs(long lIndex, GROW_LIST& rList)			// ��ȡ����ɳ�ϵ��
	{
		ITR_GROW_CONF itr = m_GrowConfPets.find(lIndex);
		if( itr != m_GrowConfPets.end() )
		{
			rList = itr->second;
		}
	}

	const tagPetConf& GetPetConfig(void)						// ����ͳһ����
	{
		return m_ConfPet;
	}
	const PET_LIST& GetPetsConfig(void)							// �����б�����
	{
		return m_ConfPets;
	}
	void InformClientAI(CPet* pPet, long lOpType);				// ֪ͨ�ͻ���AI�¼�

	// ����ű��ӿ�
	CPet* CreatePet(CPlayer* pPlayer,
		long lType,
		long lIndexID);											// ��������
	bool DelPet(const CGUID& rID);								// ɾ������(ΪHOUSE)
	bool RelivePet(CPlayer* pPlayer, long lType, long lIndex);	// �������
	bool AddPetSkill(CPlayer* pPlayer,
		long lType,
		long lID,
		long lLevel,
		long lPos);												// ��ӳ��＼��

private:
	void InformWS(CPlayer* pPlayer);
	void InformClient(CPlayer* pPlayer, CPet* pPet, bool bAdd);

private:
	tagPetConf		m_ConfPet;
	GROW_CONF		m_GrowConfPets;								// ����ɳ�
	PET_LIST		m_ConfPets;									// �����б�����
	vector<CPet*>	m_DetectPets;								// ��������б�
	PETS			m_Pets;										// ���������б�

	DECLARE_MSG_MAP();
};