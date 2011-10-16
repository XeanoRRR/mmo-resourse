#include <string.h>
#include "Player.h"
#include "../Utility/GUID.h"
using namespace std;

enum eCompareGoodsError{
	ONE_INVALID,
	TWO_INVALID,
	RESULT_NO_ERROR,
	RESULT_ERROR,
};
//////////////////////////////////////////////////////////////////////////
#define DEPOT	1
#define PACKET	0
#define GOLD	1
#define SILVER	0
//////////////////////////////////////////////////////////////////////////
class CCDKey{
public:
	CCDKey(const char* strCDKey);
	~CCDKey(void);
public:
	enum eContainerID
	{
		//װ��
		EQUIP_ITEM,
		//����
		MAIN_PACK,
		MAIN_PACK_ITEM,
		MAIN_PACK_ITEM_PACK_1,
		MAIN_PACK_ITEM_PACK_2,
		MAIN_PACK_ITEM_PACK_3,
		MAIN_PACK_ITEM_PACK_4,
		MAIN_PACK_ITEM_PACK_5,
		//�ֿ�
		DEPOT_MAIN_PACK,
		DEPOT_Def_SubPACK,
		DEPOT_ITEM,
		DEPOT_ITEM_PACK_1,
		DEPOT_ITEM_PACK_2,
		DEPOT_ITEM_PACK_3,
		DEPOT_ITEM_PACK_4,
		//
		CONTAINER_NUM,
	};

	struct tagCDKeyGoodsInfo 
	{
		//		CGUID guid;				//��Ʒguid
		eContainerID containerID;	//��Ʒ��������ID
		DWORD dwPos;			//������������
		WORD  wNum;				//ͬһ�����ϵ�����
		//
		CGoods* pGoods;
	};

	hash_map<CGUID,tagCDKeyGoodsInfo,hash_guid_compare>	m_mapContainerID[CONTAINER_NUM];
	typedef hash_map<CGUID,tagCDKeyGoodsInfo,hash_guid_compare>::iterator hashGoodsIter;

	static void InitCDKeyList(const char* filename);
	static void InitCDKeyData(CCDKey* pCDKey);
	static CCDKey* GetCDKey(const char* strCDKey);
	static BOOL FindCDKey(const char* strCDKey);//	{ return m_mapCDKeyList.find(strCDKey)->second; }
	static void AddCDKey(CCDKey* pCDKey)	;//	{ m_mapCDKeyList.insert(make_pair(strCDKey,TRUE));}
	
	//BOOL IsAddData()		{ return m_bHasAddData = m_bPlayerDataSaved & m_bDepotDataSaved;}
	void AddPlayerData(CPlayer* pPlayer);
	void AddDepotData(CPlayer* pPlayer);
	void CheckPlayerData(CPlayer* pPlayer);
	void CheckGoods(CPlayer* pPlayer);

	/*
			Goods������֤
	*/
	eCompareGoodsError CompareGoodsData(CGoods* pPlayerGoods,CGoods* pCDKeyGoods);

	static void Release();

public:
	//
	void WriteOutInfo(std::ofstream & out_f,const char* strOutInfo);
	void SaveAllContainerData();
	void SaveAllMoney();
	void SavePlayerBaseProperty();
	void SaveSkill();

	void GetAllContainerData();
	void GetAllMoney();
	void GetPlayerBaseProperty();
	void GetSkill();
private:
	hash_map<CGUID,CGoods*,hash_guid_compare> m_mapGoodsSaveInfo;

public:
	struct tagCDKeyDepotPack 
	{
		hash_map<CGUID,CPlayer::tagGoods,hash_guid_compare> mapGoods;
		long	lContainerID;				//ID-----���ֿ�/Ĭ���ӱ���/�ӱ���/(���/������λ)�ȵ�
		long	lGoodsAmount;				//����
		long	lCubage;					
		tagCDKeyDepotPack()
		{
			lContainerID = -1;
			lGoodsAmount = 0;
			lCubage = 0;
		}
	};
private:
	string m_strCDKey;

	//��̬ ��������cdkey
	static hash_map<string,CCDKey*> m_mapCDKeyList;
	//Player��������
	tagPlayerProperty m_PlayerBaseProperty;
	//��ҵ����
	hash_map<DWORD,DWORD> m_mapTradeCredit;
	//player����
	hash_map<DWORD,CPlayer::tagSkill>	m_mapSkillEx;
	//�ֿ�
	CGUID	m_DepotMoneyGuid;
	CGUID	m_DepotSilverGuid;
	long	m_DepotMoneyAmount;
	long	m_DepotSilverAmount;
	//����
	CGUID	m_MoneyGuid;
	CGUID	m_SilverGuid;
	CGUID	m_BaoShiGuid;
	CGUID	m_JiFenGuid;
	long	m_MoneyAmount;
	long	m_SilverAmount;
	long	m_BaoShiAmount;
	long	m_JiFenAmount;

public:

};