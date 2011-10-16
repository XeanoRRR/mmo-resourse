//********************************************************************************
//	created:	2004/10/12 14:35:58
//	file name: 	d:\Project\DBAccess\Worlddb\DBGoods.h
//	file path:	d:\Project\DBAccess\Worlddb
//	file base:	DBGoods
//	file ext:	h
//	author:		Bugs
//
//	purpose:	�洢�����Ʒ�����ݿ�
//********************************************************************************

#pragma once
#ifndef _INC_DBGOODS
#define _INC_DBGOODS
#include "..\myadobase.h"
#include "..\..\server\dbserver\appworld\dbgood.h"
//#include "..\..\server\dbserver\appworld\goods\CGoodsFactory.h"
//#include "..\..\server\dbserver\appworld\goods\CGoodsBaseProperties.h"
#include "..\..\server\dbserver\appworld\dbplayer.h"

//class CGame;
class CDBGoodsGroup;

class CRSGoods :
	public CMyAdoBase
{
public:

	CRSGoods(void);
	~CRSGoods(void);

	// ɾ�������Ʒ
	bool static DeleteContainerGoods(CDBGoodsGroup* goodsGroup, const CGUID& ownerID, _ConnectionPtr& cn);

	bool static SaveGood(const CGUID& ownerID, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn);// save call
	bool static SaveGoodsFiled(const CGUID& ownerID, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn); // save call

	// װ�������Ʒ�б�
	bool LoadGoods(CDBPlayer* player, CDBGoodsGroup* goodsGroup, _ConnectionPtr& cn);

	bool static DeleteGoods(const CGUID& dwPlayerID, _ConnectionPtr& cn);

	CDBGoods* FindGoodsByPos(CDBPlayer* pPlayer ,long place, long pos);

	//��Buffer
	static bool SaveBufferField(CDBGoods *pGoods, const string& szFiledName, _RecordsetPtr &pRs);
	//��Buffer
	bool LoadBufferField(CDBGoods *pGoods, const string& szFiledName, _RecordsetPtr &pRs);

	bool UpdatePlayerGoldCoins(CDBGoods *pGoods, const CGUID& ownerID, _ConnectionPtr& cn); 
	enum GOODS_PLACE
	{		
		PLACE_PACKET					= 1,	//��ͨ����
		PLACE_EQUIP,//װ��
		//PLACE_HAND,//����
		PLACE_WALLET,//Ǯ��
		PLACE_SILVERWALLET,//����
		PLACE_GEM,//! ��ʯ
		PLACE_YUANBAO,//Ԫ��
		PLACE_JIFEN,//����
		PLACE_EQUIPMENT_UPGRADING,			//װ������
		//PLACE_BANK,//����
		//PLACE_DEPOT, //�ֿ�
		PEI_ENHANCEMENT,//

		PLACE_PACK =10000,
		PLACE_PACK1=10001,
		PLACE_PACK2=10002,
		PLACE_PACK3=10003,
		PLACE_PACK4=10004,
		PLACE_PACK5=10005,

		//! ����һ���ֿ��������ͱ�������ϵ������������ֵ��PEI_PACK5=10005���������ʼֵ�������ظ�
		DEPOT_CONTAINER_TYPE_BEGIN_VALUE = 0xFFFF00,
		//! ��Ҳֿ�
		eDCT_Gold,
		//! ���Ҳֿ⣨��ǰ�Ǳ�ʯ�ֿ⣬ö��ֵ���䣩
		eDCT_Gem,
		//! ���ֿ�
		eDCT_Primary,
		//! ���ֿ�2
		eDCT_Primary2,
		//! ��չ��������
		eDCT_Secondary,
		//! ��չ�ֿ�1
		eDCT_Secondary1,
		//! ��չ�ֿ�2
		eDCT_Secondary2,
		//! ��չ�ֿ�3
		eDCT_Secondary3,
		//! ��չ�ֿ�4
		eDCT_Secondary4,
		//! ��չ�ֿ�5
		//eDCT_Secondary5,
		//! ����ֵ�����ڷ�Χ�жϣ�
		DEPOT_CONTAINER_TYPE_END_VALUE,
	};

protected:
/*	// �洢��Ʒ��������
	bool static SaveGoodsProperties(vector<CDBGoods::tagAddonProperty>& vAddonProperties,
		CGoodsBaseProperties *pGoodsBaseProperties, 
		char* GoodsID, _ConnectionPtr cnSave);
*/
private:	
	// װ����Ʒ��������
	//bool static LoadGoodsProperties(CDBGoods *pGoods, const char *szID, _ConnectionPtr& cn);

};
#endif // _INC_DBGOODS