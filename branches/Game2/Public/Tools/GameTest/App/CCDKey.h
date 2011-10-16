
//--------------------------------------------------
//brief:���������ĳһ����������д�뱾�ش��̺�
//��������һ�κͷ����������������ݽ��бȽ�,
//���������ӳ�,�����˳�ʱδ����Ϣ������Ϣ�����������Ĵ���
//������Ҫ���ڼ򵥵������ж�,������Ʒ�ܵ�����---��Ʒ�Ƿ���ʧ
//  [7/5/2008 Square]
//modify :09/1/5

#pragma once

#include <string.h>
#include <vector>
#include "Player.h"
#include <stdarg.h>
#include <stdio.h>
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
	enum eSaveDataType
	{
		eSAVE_GOODS,
		eSAVE_MAIL,
		eSAVE_SKILL,
		eSAVE_QUEST,
		eSAVE_LINKMAN,
		eSAVE_FACTION,
		eSAVE_LEVEL,
		eSAVE_EQUIP,
	};

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
		tagCDKeyGoodsInfo(void)
		{
			containerID = (eContainerID)-1;
			dwPos = -1;
			wNum = -1;
			pGoods = NULL;
		}
	};

	hash_map<CGUID,tagCDKeyGoodsInfo,hash_guid_compare>	m_mapContainerID[CONTAINER_NUM];
	typedef hash_map<CGUID,tagCDKeyGoodsInfo,hash_guid_compare>::iterator hashGoodsIter;

	struct tagCDKeyEquipInfo 
	{
		DWORD dwItemPos;
		DWORD dwGoodsIndex;
		char szGuid[128];
		tagCDKeyEquipInfo(void)
		{
			dwItemPos = -1;
			dwGoodsIndex = -1;
			ZeroMemory(szGuid,128);
		}
	};


	//	void AddPlayerDataExAmount(CPlayer* pPlayer);
	//	void AddDepotData(CPlayer* pPlayer);


	// ��������Ľӿ� [1/5/2009 Square]
	static void InitCDKeyList(const char* filename);			//��CDKeyList.ini�ļ�
	static void InitCDKeyData(CCDKey* pCDKey);					//��ȡ�����ϵ�cdkey����	cdkey��Ӧ��Ψһ��һ��player

	static CCDKey* GetCDKey(const char* strCDKey);				//�õ�һ��strCDKey��CDKey����
	static BOOL FindCDKey(const char* strCDKey);				//strCDKey�Ƿ����
	static void AddCDKey(CCDKey* pCDKey);						//���һ��cdkey����

	void AddPlayerData(CPlayer* pPlayer,eSaveDataType eType);	//�����eType��������д������
	void CheckPlayerData(CPlayer* pPlayer,eSaveDataType eType);	//������eType��������

	void SaveGoodsData(CPlayer* pPlayer);						//����player���ϵ���Ʒ����
	void SaveGoodsCounts(CGoods* pGoods);						//��Ʒ����
	void CheckGoods(CPlayer* pPlayer);

	void SaveSkill(CPlayer* pPlayer);
	void SaveQuest(CPlayer* pPlayer);
	void CheckSkill(CPlayer* pPlayer);
	void CheckQuest(CPlayer* pPlayer);

	void SaveLevel(CPlayer * pPlayer);
	void CheckLevel(CPlayer * pPlayer);

	void SaveEquip(CPlayer* pPlyaer);
	void CheckEquip(CPlayer* pPlayer);

	//private:
	map<DWORD,DWORD>	m_mapGoodsAmountByIndex;

	static void Release();

	//
	void WriteOutInfo(std::ofstream & out_f,const char* strOutInfo);
private:
	hash_map<CGUID,CGoods*,hash_guid_compare> m_mapGoodsSaveInfo;
	map<DWORD,BOOL>	m_mapQuestInfo;
	map<DWORD,BOOL>	m_mapSkillInfo;

	map<DWORD,tagCDKeyEquipInfo*>	m_mapCDKeyEquipInfo;

	//-----------------------------------------------------------------------------------------
	// ��������ϵ��ʼ��������ݱ���ͺ˶�
	// by MartySa 2009.1.5
	//-----------------------------------------------------------------------------------------
private:
	std::vector<stMailData> m_vecSaveMailData;							//�����յ���mail����

public:
	void	AddSaveMailDataByTxt(CPlayer *pPlayer);						//��ӱ����ʼ����ݴ��ı���
	void	ClearAllSaveMailData();										//��ձ����ʼ���������

	void	SavaMailData(CPlayer *pPlayer);								//�����ʼ�����
	void	CheckMailData(CPlayer *pPlayer);							//����ʼ�����
	void    PutCheckMailErr(const char *msg);	

	//�ַ�ת�����ܺ���
	string GetWriteCString(int cstrSize, ... );						//ȡ�����CString
	string  GetWirteString(int strSize, ... );							//ȡ�����string
	//end save&check mail
	//-----------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// ��������ϵĺ��ѽ������ݱ���ͺ˶�
	// by MartySa 2009.1.12
	//-----------------------------------------------------------------------------------------
private:
	SPanel		m_SaveLinkManPanelGroup[MAXNUM_PANELS];

public:
	void	AddSaveLinkManPanleGroupByTxt(CPlayer *pPlayer);					//��ӱ���������ݴ��ı���
	void	ClearLinkManAllSavePanleGroup(CPlayer *pPlayer);					//��ձ���������ݺͱ����������
	void	SaveLinkManPanleGroup(CPlayer *pPlayer);							//����������ݵ��ı�
	void	CheckSaveLinkManPanleGroup(CPlayer *pPlayer);						//����������
	void	PutCheckLinkManErr(const char *msg);								//�����⵽������Ϣ
	//����õ� mail���ַ���ת�����ܺ���

	//end save&check linkman
	//-----------------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------------
	// ��������ϵ��л�������ݱ���ͺ˶�
	// by MartySa 2009.1.13
	//-----------------------------------------------------------------------------------------
public:
	//�Զ���һ���ṹ����Ϊ�����л����ݵ�tag
	struct tagSaveFaction 
	{
		CGUID						FamilyGuid;								//����guid
		string						strFamilyName;							//��������
		string						strFamilyTitle;							//����ƺ�
		CGUID						FamilyMasterGuid;						//�����峤guid
		long						lFamilyJobLevel;
		CGUID						UnionGuid;								//�Լ�����ͬ��GUID
		set<CGUID, guid_compare>	EnemyFamily;							//�жԼ���
		set<CGUID, guid_compare>	CityWarEnemyFamily;						//��ս�жԼ���
		list<tagownedReg>	listOwnRgn;								//����ӵ�еĳ����б�
		bool						bInContendState;						//�Ƿ�����״̬
		bool						bCityWarDied;							//��ս����״̬

		tagSaveFaction() : FamilyGuid(CGUID::GUID_INVALID), FamilyMasterGuid(CGUID::GUID_INVALID),
			lFamilyJobLevel(0), UnionGuid(CGUID::GUID_INVALID), 
			bInContendState(false), bCityWarDied(false)
		{}
	};
private:
	tagSaveFaction		m_SaveFaction;

public:
	void	AddSaveFactionByTxt(CPlayer *pPlayer);					//��ӱ����л����ݴ��ı���
	void	ClearSaveFaction(CPlayer *pPlayer);						//��ձ����л����ݺͱ����л�����
	void	SaveFaction(CPlayer *pPlayer);							//�����л����ݵ��ı�
	void	CheckFaction(CPlayer *pPlayer);							//����л�����
	void	PutCheckFactionErr(const char *msg);					//�����⵽������Ϣ
	//
	//����õ� mail���ַ���ת�����ܺ���

	//end save&check Faction 
	//-----------------------------------------------------------------------------------------

public:
	struct tagCDKeyDepotPack 
	{
		hash_map<CGUID,tagGoods,hash_guid_compare> mapGoods;
		long	lContainerID;				//ID-----���ֿ�/Ĭ���ӱ���/�ӱ���/(���/������λ)�ȵ�
		long	lGoodsAmount;				//����
		long	lCubage;					
		tagCDKeyDepotPack() : lContainerID(-1), lGoodsAmount(0), lCubage(0)
		{}
	};
private:
	string m_strCDKey;
	char szPlayerGuid[256];

private:
	//��̬ ��������cdkey
	static hash_map<string,CCDKey*> m_mapCDKeyList;
	//Player��������
	tagPlayerProperty m_PlayerBaseProperty;
	//��ҵ����
	hash_map<DWORD,DWORD> m_mapTradeCredit;
	//player����
	hash_map<DWORD,tagPlayerSkill>	m_mapSkillEx;
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

	long	m_lLevel;

	void SetLevel(long l)	{ m_lLevel = l; }

};