/**
* @file    PetCtrl.h
* @author  Fox (yulefox@gmail.com)
* @date    Nov.18, 2008
* @brief   CPetCtrl���ڿ��Ƴ���AI.
*/

#pragma once

#include "../Net/MsgRegister.h"
#include "../../../Public/Common/BasePetDef.h"
#include "../../../Public/Common/PetSetup.h"
#include "../../../Public/ServerPublic/Server/BaseDef/BasePetServerDef.h"

class CPlayer;

class CPetCtrl
{
public:
	CPetCtrl(void);
	~CPetCtrl(void);

	// ���á������ӿ�
public:
	/// ȡ�ó��﹦�ܿ���
	long GetFuncSwitch(void) { return m_ConfPet.nFuncSwitch; }

	/// ���ݳ���Index��ȡ�ɳ�ϵ��
	void GetGrowCoefs(long lIndex, GROW_LIST& rList);

	/// ȡ�ó���ͳһ����
	const tagPetConf& GetPetConfig(void) { return m_ConfPet; }
	
	/// ȡ�ó����б�����
	const PET_LIST& GetPetsConfig(void)	{ return m_ConfPets; }

	/// ���ý���
	bool DecodeFromDataBlock(DBReadSet& setReadDB);

	/// ����GS==>Client����
	bool CodeToDataBlockForClient(DBWriteSet& setWriteDB,CPlayer* pPlayer);
	
	/// ����GS==>WS����
	bool CodeToDataBlock(DBWriteSet& setWriteDB,CPlayer* pPlayer);

	/// ����WS==>GS����
	bool DecodeFromDataBlock(DBReadSet& setReadDB, CPlayer* pPlayer);

public:

	void InformClientAI(CPet* pPet, long lOpType);				// ֪ͨ�ͻ���AI�¼�

	/// �������
	void ActivePet(CPet* pPet);

	//void DelPet(CPet* pPet, long lOpType);						// ɾ��(δ)����

	// ����ű��ӿ�

	/// ���ݳ�������ID��������
	/// @param  pPlayer		���
	/// @param  type		��������
	/// @param  id			��������ID
	/// @return	��������ɹ����
	bool CreatePet(CPlayer* pPlayer,long lType, long lIndexID, bool IsWild);

	/// ɾ������
	/// @param  pPlayer		���
	/// @param  guid		����GUID
	/// @return	�ɹ����
	bool DelPet(CPlayer* pPlayer, const CGUID& guid);

	/// �������
	/// @param player	���
	/// @param  guid		����GUID
	/// @return	�ɹ����
	bool ActivePet(CPlayer* player, const CGUID& guid);

	/// ɾ���������
	/// @param player	���
	/// @param  guid		����GUID
	/// @return	�ɹ����
	bool UnActivePet(CPlayer* player, const CGUID& guid);

	/// ���ó�������
	/// @param player	���
	/// @param attrName ������
	/// @param attrVal ����ֵ
	/// @param guid ����GUID
	/// @return	�ɹ����
	bool SetPetAttr(CPlayer* player, const char *attrName, double attrVal, const CGUID& guid);

	/// ��ȡ��������ֵ
	/// @param player	���
	/// @param attrName ������
	/// @param guid ����GUID
	/// @return ����ֵ
	long GetPetAttr(CPlayer* player, const char* attrName, const CGUID& guid);


	/// ��ӳ��＼��
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param  id			����ID
	/// @param  level		���ܵȼ�
	/// @param	pos			λ��
	/// @return	��ӳ��＼�ܳɹ����
	bool AddPetSkill(CPlayer* player,
		const CGUID& guid,
		long id,
		long level,
		long pos);												// ��ӳ��＼��

	/// ���ó�������
	/// @param  player		���
	/// @param  guid		����GUID
	/// @return	�������ʳɹ����
	bool ResetPetTalent(CPlayer* player,const CGUID& guid);

	/// ��������ǿ��
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param	val			ǿ��ϵ��(����ǿ��Ч��ֵ)
	/// @return	��������ǿ���ɹ����
	bool UpgradePetDAfterVal(CPlayer* player,const CGUID& guid,long val);

	/// ���ú�������ǿ������
	/// @param  player		���
	/// @param  guid		����GUID
	/// @return	���ú�������ǿ�������ɹ����
	bool ResetPetDAfterCnt(CPlayer* player,const CGUID& guid);

	/// ���ó������Ե�
	/// @param  player		���
	/// @param  guid		����GUID
	/// @return	���ó������Ե�ɹ����
	bool ResetPetSavvy(CPlayer* player,const CGUID& guid);

	/// ȡ���̴�Ʒ��
	/// @param  player		���
	/// @param  guid		����GUID
	/// @return �̴�Ʒ��
	long GetPetGrade(CPlayer* player,const CGUID& guid);

	/// ȡ���̴Ӻ���������ǿ������
	/// @param  player		���
	/// @param  guid		����GUID
	/// @return �̴Ӻ�����ǿ������
	long GetPetDAfterCnt(CPlayer* player, const CGUID& guid);

	/// ȡ���������ӵ���̴���Ŀ
	/// @return �������ӵ���̴���Ŀ
	long GetMaxServantNum(void);

	/// ����ǿ������ȡ��ǿ�������б�
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param  type		ǿ������
	/// @param  goods		�����б�
	void GetMaterialList(CPlayer* player, const CGUID& guid, long type, vector<tagMaterial>& goods);

	/// �̴Ӿ����ȡ
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param  val			����ֵ
	void IncPetExp(CPlayer* player, const CGUID& guid, long val);

	/// �̴���Ϊ�����ȡ
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param  val			��Ϊ����ֵ
	void IncPetDExp(CPlayer* player, const CGUID& guid, long val);

	/// ���½ű�������Ϊ����ʹ��
	/// ��������
	/// @param  player		���
	/// @param  guid		����GUID
	/// @param  newLev		�����µȼ�
	/// @return ���������Ƿ�ɹ�
	bool PetLevUp(CPlayer* player, const CGUID& guid, long newLev);

	/// �����������
	/// ������������
	/// �����������ǿ��
	/// ����Ʒ��ǿ��
	/// ����

private:
	/// ����WS���̵�
	void InformWS(CPlayer* pPlayer);

	/// ֪ͨ�ͻ����ڳ�������ӳ���
	void InformClient(CPlayer* pPlayer, CPet* pPet, bool bAdd);

	/// ���ó���
	bool ReSetPet(CPet* pPet, long lInitType, bool IsWild);

	/// ��ӳ���
	void OnCreatePet(CPet* pPet);

	/// ����GUID��ȡ����
	CPet* GetPet(CPlayer* player, const CGUID& guid);

private:
	tagPetConf		m_ConfPet;									///< ����ϵͳͳһ����
	GROW_CONF		m_GrowConfPets;								///< ����ɳ�����
	PET_LIST		m_ConfPets;									///< �����б�����
};