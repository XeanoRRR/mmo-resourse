



#ifndef _EquipMgr_H_
#define _EquipMgr_H_
#pragma once
#include "../Utility/gtDef.h"
class CGoods;

class CEquipMgr
{
public:
	CEquipMgr(void);
	~CEquipMgr(void);

	void DecodeFromDataBlock(DBReadSet& dbRead,bool bExData);

	void DecodeFromDataBlockEx(DBReadSet& dbRead);

	CGoods* GetEquip(int index);

	bool GetWeapon(const char* strOrgName);

	void SetEquip(int index,CGoods *pGood);

	CGoods* GetGoodsByGUID(const CGUID& guID,int *pPos,long* lExtendID);

	WORD GetGoodsContainerID(CGoods* pGoods);

	DWORD GetEquipIndex(long l)	{return m_dwEquipIndex[l];}
	void SetEquipIndex(long lIndex, DWORD dwIndex);
	BYTE GetEquipLevel(long l)	{return m_btEquipLevel[l];}
	void SetEquipLevel(long lIndex,BYTE level);

	long GetEquipAmount();

	CGoods* GetEquipInfo()	{ return m_Equip[0];}

private:
	CGoods*	m_Equip[EQUIP_SUM];								//װ������,���浱ǰװ�������ݽṹ
	DWORD	m_dwEquipIndex[EQUIP_SUM];		// װ����ţ��������ʹ�ã�
	BYTE	m_btEquipLevel[EQUIP_SUM];		// װ���ȼ����������ʹ�ã�
};
#endif//_EquipMgr_H_