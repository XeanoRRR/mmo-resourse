


#ifndef __GoodsMGr_H_
#define __GoodsMGr_H_
#pragma once
#include "../Utility/gtDef.h"

class CGoodsMgr
{
public:
	CGoodsMgr(class CPlayer* pPlayer);

	~CGoodsMgr(void);

	void Release();

	class CPlayer*		GetOwnerPlayer(void)	{ return m_pOwnerPlayer; }

	class CEquipMgr*	GetEquipMgr(void)		{ return m_pEquipMgr; }

	class CPacketMgr*	GetPacketMgr(void)	{ return m_pPacketMgr; }

	class CDepotMgr*	GetDepotMgr(void)		{ return m_pDepotMgr; }

	class CGoods* QueryGoodsByGuid(const CGUID& guid);

	class CGoods* QueryGoodsByGuid(const CGUID& guid,int* lPos,long* lType);

	bool AddGoods(CGoods* pGoods,DWORD dwPos,DWORD eExtendID);
	
	CGoods* GetGoodsByPos(long lExtendID,DWORD dwPos);

	CGoods* RemoveGoods(const CGUID& SourceGUID,const CGUID& DesGUID,DWORD dwPos,LONG lExtendID,DWORD dwAmount);
	
private:
	CPlayer*			m_pOwnerPlayer;
	//	������Ʒ����
	list<tagGoods>		m_listAllGoods;

	//	������λ
	class CPacketMgr*		m_pPacketMgr;
	//	װ��
	class CEquipMgr*		m_pEquipMgr;
	//	�ֿ�
	class CDepotMgr*		m_pDepotMgr;

};
#endif//__GoodsMGr_H_