#pragma once
//#include "../GameTest/resource.h"	
//#include "..\GameTest\PlayerInfo.h"

#include "moveshape.h"

#include "../Utility/gtDef.h"
// #include "Goods.h"
// #include "../../public/BaseDef.h"
// 
// //#include "..\Utility\PlayerDef.h"
// 
// 
// #include "..\Utility\OrganizingParam.h"
//#include ".\PetSystem\CPet.h"
// #include "../../public/BounsDef.h"

class CGameClient;
class CMonster;
class CPet;
class CGoodsList;

struct tagWSMemberInfo;



// ���������� - ��Ӧ�������� CSCContainerObjectMove.h


class CRegion;
class CPlayer : public CMoveShape
{
public:

	CPlayer(void);
	
	~CPlayer(void);

	class CMailMgr*	GetMailMgr(void)	{ return m_pMailMgr; }

	class CSkillMgr*	GetSkillMgr(void)	{ return m_pSkillMgr; }

	class CPetMgr*		GetPetMgr(void)	{ return m_pPetMgr; }

	class CGoodsMgr*	GetGoodsMgr(void)	{ return m_pGoodsMgr; }

	class CTeamMgr*	GetTeamMgr(void)	{ return m_pTeamMgr; }

	class CPlayerShop*	GetShopMgr(void)	{ return m_pShop; }

	class CLinkMan*	GetLinkMan(void)	{ return m_pLinkMan; }

	class CMedalMgr*	GetMedalMgr(void)	{ return m_pMedalMgr; }

	class COrganiseSys* GetOrganiseSys(void)	{ return m_pOrganiseSys; }

	class CSprite*		GetSpriteMgr(void)	{ return m_pSpriteMgr; }

	class CTask*		GetTaskMgr(void)		{ return m_pTaskMgr; }

	class CPlayerCollect*	GetCollectMgr(void)	{ return m_pCollect; }

	class CTradeMgr*	GetTradeMgr(void)			{ return m_pTradeMgr; }


	bool IsSelf()	const				{ return m_bSelf;}

	void SetIsSelf(const bool b)		{ m_bSelf = b; }

private:

	void Release();

	//	���ܹ�����
	class CSkillMgr*	m_pSkillMgr;

	//	�ʼ�������
	class CMailMgr*	m_pMailMgr;

	//	���������
	class CPetMgr*		m_pPetMgr;

	class CGoodsMgr*	m_pGoodsMgr;

	class CTeamMgr*	m_pTeamMgr;

	class CPlayerShop*	m_pShop;

	class CPlayerCollect*	m_pCollect;

	class CFamily*			m_pFamily;

	class CLinkMan*		m_pLinkMan;

	class CMedalMgr*		m_pMedalMgr;

	class COrganiseSys*	m_pOrganiseSys;

	class CSprite*			m_pSpriteMgr;

	class CTask*			m_pTaskMgr;

	class CTradeMgr*		m_pTradeMgr;

	//	�Ƿ������Լ�	
  	bool	m_bSelf;		
	//	���� �������ݽṹ
  	tagPPro						m_Property;						//��������
  	tagPFPro					m_FightProperty;					//ս������
  	tagPlayerElementProperty	m_arrElementProperty[MT_Max];

	CGUID			m_PlayerMoneyGuid;				//	���
  	CGUID			m_PlayerBaoShiGuid;				//	��ʯ
  	CGUID			m_PlayerYuanBaoGuid;				//	Ԫ��
  	CGUID			m_PlayerJiFenGuid;				//	����
  	CGUID			m_PlayerSilverMoneyGuid;			//	����
  
  	DWORD			m_dwPlayerMoneyNum;				//	���
  	DWORD			m_dwPlayerBaoShiNum;				//	��ʯ
  	DWORD			m_dwPlayerYuanBaoNum;				//	Ԫ��
  	DWORD			m_dwPlayerJiFenNum;				//	����
  	DWORD			m_dwPlayerSilverMoneyNum;		//	����

	char			m_strPlayerOfficialRank[64];		//	���ҹٽ�

	std::map<long,long>   m_GeniusTable;    // �츳���ñ�,ID-level

 public:
 	virtual void SetAction(WORD l);
 	virtual void SetState(WORD l);

 	DWORD	GetPlayerMoneyNum()					{ return m_dwPlayerMoneyNum; }
 	DWORD	GetPlayerJiFenNum()					{ return m_dwPlayerJiFenNum; }
 	DWORD	GetPlayerBaoShiNum()					{ return m_dwPlayerBaoShiNum; }
 	DWORD	GetPlayerYuanBaoNum()					{ return m_dwPlayerYuanBaoNum; }
 	DWORD	GetPlayerSilverMoneyNum()				{ return m_dwPlayerSilverMoneyNum; }
 
 	void	SetPlayerJiFenNum(DWORD l)			{ m_dwPlayerJiFenNum = l; }
 	void	SetPlayerMoneyNum(DWORD l)			{ m_dwPlayerMoneyNum = l; }
 	void	SetPlayerBaoShiNum(DWORD l)			{ m_dwPlayerBaoShiNum = l; }
 	void	SetPlayerYuanBaoNum(DWORD l)			{ m_dwPlayerYuanBaoNum = l; }
 	void	SetPlayerSilverMoneyNum(DWORD l)		{ m_dwPlayerSilverMoneyNum = l; }
 
 	void	SetPlayerMoneyGuid(const CGUID& guid)			{m_PlayerMoneyGuid	= guid; }
 	void	SetPlayerBaoShiGuid(const CGUID& guid)			{m_PlayerBaoShiGuid	= guid; }
 	void	SetPlayerYuanBaoGuid(const CGUID& guid)			{m_PlayerYuanBaoGuid= guid;}
 	void	SetPlayerJiFenGuid(const CGUID& guid)			{m_PlayerJiFenGuid	= guid;}
 	void	SetPlayerSilverMoneyGuid(const CGUID& guid)	{m_PlayerSilverMoneyGuid	= guid;}
 
 	CGUID&	GetPlayerSilverGuid()								{return m_PlayerSilverMoneyGuid;}
 	CGUID&	GetPlayerMoneyGuid()								{return m_PlayerMoneyGuid;}
 	CGUID&	GetPlayerBaoShiGuid()								{return m_PlayerBaoShiGuid;}
 	CGUID&	GetPlayerYuanBaoGuid()							{return m_PlayerYuanBaoGuid;}
 	CGUID&	GetPlayerJiFenGuid()								{return m_PlayerJiFenGuid;}

	tagPPro &GetPlayerProperty()						{ return m_Property;}

	DWORD	GetOccu()					{return m_Property.byAssOccu;}		
	void	SetOccu(DWORD dwOccu)		{m_Property.byAssOccu = (eDeputyOccu)dwOccu;}	

	DWORD	GetAssOccu()				{return (DWORD)m_Property.byAssOccu;}			//��ȡ��ְҵ
	void	SetAssOccu(DWORD dwOccu)	{m_Property.byAssOccu = (eDeputyOccu)dwOccu;}

	DWORD	GetHp()						{return	m_Property.dwHp;}			//��ǰ����ֵ
	void	SetHp(DWORD l)				{m_Property.dwHp=l;}

	WORD	GetMp()						{return	m_Property.wMp;}			//��ǰħ��ֵ
	void	SetMp(WORD l)				{m_Property.wMp=l;}

	WORD	GetStamina()				{return	m_Property.wStamina;}		//��ǰ����
	void	SetStamina(WORD l)			{m_Property.wStamina=l;}

	WORD	GetEnergy()					{return	m_Property.wEnergy;}		//��ǰ����ֵ 
	void	SetEnergy(WORD l)			{m_Property.wEnergy=l;}

	//BYTE	GetRankOfNobility()			{return m_Property.byRankOfNobility;}		//��λ�ȼ�
	//void	SetRankOfNobility(BYTE l)	{m_Property.byRankOfNobility=l;}

	//DWORD	GetRanOfNobCredit()			{return m_Property.dwRankOfNobCredit;}	//��λ������ֵ
	//void	SetRanOfNobCredit(DWORD l)	{m_Property.dwRankOfNobCredit=l;}

	//BYTE	GetRankOfMercenary()		{return m_Property.byRankOfMercenary;}	//Ӷ���ȼ�
	//void	SetRankOfMercenary(BYTE l)	{m_Property.byRankOfMercenary=l;}

	//DWORD	GetMercenaryCredit()		{return m_Property.dwMercenaryCredit;}	//Ӷ��������ֵ
	//void	SetMercenaryCredit(DWORD l)	{m_Property.dwMercenaryCredit=l;}

	BYTE	GetCountry()				{return	m_Property.byCountry;}			//����ID
	void	SetCountry(BYTE l)			{m_Property.byCountry=l;}

	long	GetCountryContribute()		{return m_Property.lCountryContribute;}	//�Թ��ҵĹ���ֵ
	void	SetCountryContribute(long l)	{m_Property.lCountryContribute=l;}

	DWORD	GetUpgradeExp()				{return	m_Property.dwUpgradeExp;}			//��һ���������辭��
	void	SetUpgradeExp(DWORD l)		{m_Property.dwUpgradeExp=l;}				

	BYTE	GetBusinessLevel()			{return m_Property.byBusinessLevel;}		//��ҵ�ȼ�
	void	SetBusinessLevel(BYTE l)	{m_Property.byBusinessLevel=l;}

	DWORD	GetBusinessExp()			{return m_Property.dwBusinessExp;}		//��ҵ����
	void	SetBusinessExp(DWORD l)		{m_Property.dwBusinessExp = l;}

	WORD	GetLevel()					{return m_Property.wLevel;}				//��ҵ�ǰ�ȼ�
	void	SetLevel(WORD l)			{m_Property.wLevel=l;}	

	DWORD	GetExp()					{return m_Property.dwExp;}				//����ֵ
	void	SetExp(DWORD l)				{m_Property.dwExp=l;}

	DWORD	GetPresentExp()				{return m_Property.dwPresentExp;}				//����ֵ
	void	SetPresentExp(DWORD l)		{m_Property.dwPresentExp=l;}

	BYTE	GetHairPic()				{return m_Property.byHairPic;}			//�õ�ͷ����Ϣ
	void	SetHairPic(BYTE by)			{m_Property.byHairPic=by;}				//�趨ͷ����Ϣ

	BYTE	GetHairStyle()				{return (m_Property.byHairPic & 0xF0) >> 4;}					//�õ����ͱ��
	void	SetHairStyle(BYTE by)		{m_Property.byHairPic = (m_Property.byHairPic & 0x0F) | by;}	//�趨���ͱ��

	BYTE	GetHairColor()				{return (m_Property.byHairPic & 0x0F);}									//�õ���ɫ���
	void	SetHairColor(BYTE by)		{m_Property.byHairPic = (m_Property.byHairPic & 0xF0) | (by << 4);}		//�趨��ɫ���

	BYTE	GetFacePic()				{return m_Property.byFacePic;}			//�沿ͼƬID
	void	SetFacePic(BYTE l)			{m_Property.byFacePic=l;}

	eOccupation	GetOccupation()			{return m_Property.byOccu;}				//ְҵ
	void	SetOccupation(BYTE l)		{m_Property.byOccu = (eOccupation)l;}

	BYTE	GetSex()					{return m_Property.bySex;}				//�Ա�(0:�� 1:Ů) 
	void	SetSex(BYTE l)				{m_Property.bySex=l;}

	DWORD	GetSpouseID()				{return m_Property.dwSpouseID;}			//��ż���ID
	void	SetSpouseID(DWORD l)		{m_Property.dwSpouseID=l;}

	DWORD	GetSpouseParam()			{return m_Property.dwSpouseParam;}		//����ż�Ĺ�ϵֵ
	void	SetSpouseParam(DWORD l)		{m_Property.dwSpouseParam=l;}

	DWORD	GetPkCount()				{return m_Property.dwPkCount;}			//���PKɱ����������
	void	SetPkCount(DWORD l)			{m_Property.dwPkCount=l;}

	DWORD	GetPkValue()				{return m_Property.dwPkValue;}			//���PKֵ(����ֵ)
	void	SetPkValue(DWORD l)			{m_Property.dwPkValue=l;}

	DWORD	GetPVPCount()				{return m_Property.dwPVPCount;}			//���PVPɱ���������
	void	SetPVPCount(DWORD l)		{m_Property.dwPVPCount=l;}

	DWORD	GetPVPValue()				{return m_Property.dwPVPValue;}			//���PVPֵ(����ֵ)
	void	SetPVPValue(DWORD l)		{m_Property.dwPVPValue=l;}

	DWORD	GetHotKey(long lPos)		{return m_Property.dwHotKey[lPos];}		//�Ƚ�(F1~F12,1~+)
	void	SetHotKey(long lPos,DWORD l){m_Property.dwHotKey[lPos]=l;}

	//	//PK����(b0:��ͨ,b1:����,b2:�л�,b3:�ﷸ,b4:�������)
	bool IsPk_Normal()	{ return m_Property.byPKOnOff & 0x01 ? true : false ;}		
	bool IsPk_Team()		{ return m_Property.byPKOnOff & 0x02 ? true : false ;}
	bool IsPk_Union()		{ return m_Property.byPKOnOff & 0x03 ? true : false ;}
	bool IsPk_Badman()	{ return m_Property.byPKOnOff & 0x04 ? true : false ;}
	bool IsPk_Country()	{ return m_Property.byPKOnOff & 0x05 ? true : false ;}
	void	SetPKOnOff(BYTE b)			{ m_Property.byPKOnOff = b; }				//����PK����
	DWORD	GetSkillPoint()				{return	m_Property.dwOccuSP[m_Property.byOccu];}			//��ǰӵ�еļ��ܵ���(SP)
	//�õ�ְҵ�ȼ�
	inline BYTE	GetOccuLvl(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.byOccuLvl[byOccu];
		return 0;
	}

	inline void  SetOccuLvl(eOccupation byOccu,BYTE l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.byOccuLvl[byOccu] = l;
	}

	//�õ�ְҵ����
	inline DWORD	GetOccuExp(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.dwOccuExp[byOccu];
		return 0;
	}
	inline void SetOccuExp(eOccupation byOccu,DWORD l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.dwOccuExp[byOccu] = l;
	}

	inline DWORD GetOccuSp(eOccupation byOccu)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			return m_Property.dwOccuSP[byOccu];
		return 0;
	}

	inline void SetOccuSp(eOccupation byOccu,DWORD l)
	{
		if(byOccu >= 0 && byOccu < OCC_Max)
			m_Property.dwOccuSP[byOccu] = l;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	const char*	GetOfficialRank() const							{ return m_strPlayerOfficialRank;}
	void	SetOfficialRank(const char* str)							{ strcpy(m_strPlayerOfficialRank ,str); }

	//����ս�����Խӿ�
	tagPlayerFightProperty GetPlayerFightProperty()		{ return m_FightProperty; }


	DWORD	GetMaxHp()					{return m_FightProperty.dwMaxHp;}				//��������
	void	SetMaxHp(DWORD l)			{m_FightProperty.dwMaxHp=l;}

	WORD	GetMaxMp()					{return m_FightProperty.wMaxMp;}				//��������
	void	SetMaxMp(WORD l)			{m_FightProperty.wMaxMp=l;}

	WORD	GetMaxStamina()				{return m_FightProperty.wMaxStamina;}			//��������
	void	SetMaxStamina(WORD l)		{m_FightProperty.wMaxStamina=l;}

	WORD	GetMaxEnergy()				{return	m_FightProperty.wMaxEnergy;}			//�������� 
	void	SetMaxEnergy(WORD l)		{m_FightProperty.wMaxEnergy=l;}

	WORD	GetHpRecoverSpeed()			{return	m_FightProperty.wHpRecoverSpeed;}		//�����ָ��ٶ�
	void	SetHpRecoverSpeed(WORD l)	{m_FightProperty.wHpRecoverSpeed;}

	WORD	GetMpRecoverSpeed()			{return	m_FightProperty.wMpRecoverSpeed;}		//�����ָ��ٶ�
	void	SetMpRecoverSpeed(WORD l)	{m_FightProperty.wMpRecoverSpeed=l;}

	WORD	GetStamRecoverSpeed()		{return m_FightProperty.wStamRecoverSpeed;}		//�����ָ��ٶ�	
	void	SetStamRecoverSpeed(WORD l)	{m_FightProperty.wStamRecoverSpeed=l;}

	//WORD	GetStrenth()				{return m_FightProperty.wStrenth;}				//����
	//void	SetStrenth(WORD l)			{m_FightProperty.wStrenth=l;}

	//WORD	GetAgility()				{return	m_FightProperty.wAgility;}				//����
	//void	SetAgility(WORD l)			{m_FightProperty.wAgility=l;}

	//WORD	GetConsti()					{return	m_FightProperty.wConsti;}				//���� 
	//void	SetConsti(WORD l)			{m_FightProperty.wConsti=l;}

	//WORD	GetIntell()					{return	m_FightProperty.wIntell;}				//����
	//void	SetIntell(WORD l)			{m_FightProperty.wIntell=l;}

	//WORD	GetWisdom()					{return	m_FightProperty.wWisdom;}				//�ǻ�
	//void	SetWisdom(WORD l)			{m_FightProperty.wWisdom=l;}

	//WORD	GetSpirit()					{return	m_FightProperty.wSpirit;}				//����
	//void	SetSpirit(WORD l)			{m_FightProperty.wSpirit=l;}

	//WORD	GetFightBlock()				{return	m_FightProperty.wFightBlock;}			//��
	//void	SetFightBlock(WORD l)		{m_FightProperty.wFightBlock=l;}

	WORD	GetParry()					{return	m_FightProperty.wParry;}				//�м�
	void	SetParry(WORD l)			{m_FightProperty.wParry=l;}

	WORD	GetDodge()					{return	m_FightProperty.wDodge;}				//���� 
	void	SetDodge(WORD l)			{m_FightProperty.wDodge=l;}

	WORD	GetCharm()					{return	m_FightProperty.wCharm;}				//����
	void	SetCharm(WORD l)			{m_FightProperty.wCharm=l;}

	WORD	GetHit()					{return m_FightProperty.wHit;}					//����
	void	SetHit(WORD l)				{m_FightProperty.wHit=l;}

	WORD	GetAbsorb()					{return m_FightProperty.wAbsorb;}				//רע
	void	SetAbsorb(WORD l)			{m_FightProperty.wAbsorb=l;}

	WORD	GetDef()					{return m_FightProperty.wDef;}					//�������
	void	SetDef(WORD l)				{m_FightProperty.wDef=l;}

	WORD	GetMdef()					{return m_FightProperty.wMdef;}					//ħ������
	void	SetMdef(WORD l)				{m_FightProperty.wMdef=l;}

	WORD	GetCri()					{return m_FightProperty.wCri;}					//������
	void	SetCri(WORD l)				{m_FightProperty.wCri=l;}

	WORD	GetMAtkSpeed()				{return m_FightProperty.wMAtkSpeed;}			//ʩ���ٶ�
	void	SetMAtkSpeed(WORD l)		{m_FightProperty.wMAtkSpeed=l;}

	WORD	GetBuffPower()				{return m_FightProperty.wBuffPower;}			//Buff����ǿ��
	void	SetBuffPower(WORD l)		{m_FightProperty.wBuffPower=l;}

	WORD	GetMinAtk()					{return m_FightProperty.wMinAtk;}				//��С������
	void	SetMinAtk(WORD l)			{m_FightProperty.wMinAtk=l;}

	WORD	GetMaxAtk()					{return m_FightProperty.wMaxAtk;}				//��󹥻���
	void	SetMaxAtk(WORD l)			{m_FightProperty.wMaxAtk=l;}

	WORD	GetMAtk()					{return m_FightProperty.wMAtk;}					//ħ��������
	void	SetMAtk(WORD l)				{m_FightProperty.wMAtk=l;}

	WORD	GetMCri()					{return m_FightProperty.wMCri;}					//ħ��������
	void	SetMCri(WORD l)				{m_FightProperty.wMCri=l;}

	//WORD	GetBloodSuk()				{return m_FightProperty.wBloodSuk;}				//����������
	//void	SetBloodSuk(WORD l)			{m_FightProperty.wBloodSuk=l;}

	//WORD	GetManaSuck()				{return m_FightProperty.wManaSuck;}				//����������
	//void	SetManaSuck(WORD l)			{m_FightProperty.wManaSuck=l;}

	WORD	GetWillDef()				{return m_FightProperty.wWillDef;}				//��־����
	void	SetWillDef(WORD l)			{m_FightProperty.wWillDef=l;}

	WORD	GetConstiDef()				{return m_FightProperty.wConstiDef;}			//���ʿ���
	void	SetConstiDef(WORD l)		{m_FightProperty.wConstiDef=l;}

	WORD	GetDeBuffDef(long pos)				{return m_FightProperty.wDeBuffDef[pos];}		//Debuff����
	void	SetDeBuffDef(long pos,WORD l)		{m_FightProperty.wDeBuffDef[pos]=l;}

	WORD	GetElemDef(long pos)				{return m_FightProperty.wElemDef[pos];}		//����Ԫ�ؿ���
	void	SetElemDef(long pos,WORD l)			{m_FightProperty.wElemDef[pos]=l;}

	WORD	GetClassDam(long pos)				{return m_FightProperty.wClassDam[pos];}		//�Ը��������˺�
	void	SetClassDam(long pos,WORD l)		{m_FightProperty.wClassDam[pos]=l;}

	WORD	GetElemDam(long pos)				{return m_FightProperty.wElemDam[pos];}		//�Ը���Ԫ���˺�
	void	SetElemDam(long pos,WORD l)			{m_FightProperty.wElemDam[pos]=l;}

	WORD	GetFightHpRecoverSpeed()			{return	m_FightProperty.wFightHpRecoverSpeed;}	//ս��ʱ�����ָ��ٶ�
	void	SetFightHpRecoverSpeed(WORD l)		{m_FightProperty.wFightHpRecoverSpeed=l;}

	WORD	GetFightMpRecoverSpeed()			{return	m_FightProperty.wFightMpRecoverSpeed;}	//ս��ʱ�����ָ��ٶ�
	void	SetFightMpRecoverSpeed(WORD l)		{m_FightProperty.wFightMpRecoverSpeed=l;}

	short	GetEnerRecoverSpeed()				{return m_FightProperty.wEnerRecoverSpeed;}		//�����ָ��ٶ�
	void	SetEnerRecoverSpeed(short l)		{m_FightProperty.wEnerRecoverSpeed=l;}

	short	GetFightEnerRecoverSpeed()			{return m_FightProperty.wFightEnerRecoverSpeed;}	//ս�������ָ��ٶ�
	void	SetFightEnerRecoverSpeed(short l)	{m_FightProperty.wFightEnerRecoverSpeed=l;}

	//WORD	GetEnergySuck()						{return	m_FightProperty.wEnergySuck;}			//����������
	//void	SetEnergySuck(WORD l)				{m_FightProperty.wEnergySuck=l;}

	WORD	GetCriDef()							{return	m_FightProperty.wCriDef;}				//����������
	void	SetCriDef(WORD l)					{m_FightProperty.wCriDef=l;}

	WORD	GetMCriDef()						{return	m_FightProperty.wMCriDef;}				//ħ����������
	void	SetMCriDef(WORD l)					{m_FightProperty.wMCriDef=l;}
	//���µ���չ����ӿ�
	void UpdateProperty(CMessage* pMsg);
	void DecordChangePropertyFromDataBlock(DBReadSet& readDB);
 	virtual bool DecordFromDataBlock(DBReadSet& readDB,bool bExData = true);
// // 	virtual void DecordOrgSysFromDataBlock(DBReadSet& readDB);
// // 	void DecordRecruitDataFromDataBlock(DBReadSet& readDB);
  	void DecordChangeProperyFromDataBlock(DBReadSet& readDB);
	bool DecodeHotKeyData(DBReadSet& setReadDB,bool bExData = true);
// // 
// // 	void DecordPlayerDepotFromDataBlock(DBReadSet& dbr);
// // 	virtual bool DecordPixieFromDataBlock(DBReadSet& dbr);
// // 	virtual bool DecordQuestFromDataBlock(DBReadSet& dbr);
// // 	void DecordRecruitFromDataBlock(DBReadSet& dbr);
// // 
// // 
// // 	//	virtual void DecordDepotGoodsInfoFromDataBlock(DBReadSet& db_read);
// // 
// // private:
// // 
// // 
// // public:
// // 
// // 	tagAutoSupplyData* GetOneSupplySetup()	{ return & m_OneSupplySetup; }
// // 	void SetAutoSupplySetup(long lIndex);			//����������,������ȷ�Ĳ�������
// // 	tagAutoSupplyData* GetAutoSupplySetup(long lIndex)		{ return &m_AutoSupplySetup[lIndex]; }
// // 
// // 	long GetItemIndex(long lIndex)		{ return m_AutoSupplySetup[lIndex].lGoodsIndex; }
// // 	long GetItemAmount(long lIndex);
// // 	
// // private:
// // 
// // public:
// // 	HSM_OneTM_Info* GetOneTeammateInfo()	{ return &m_mapOneTeammateInfo; }
// // 
// // 
// // private:
// // //	hash_map<CGUID,tagWSMemberInfo,hash_guid_compare> m_hmapTeamMemberInfo;
// // //	hash_map<CGUID,tagTeammate,hash_guid_compare> m_hmapTeamInfo;
// // 
// // 
// // 	//-----------------------------------------------
// // 	// ��д����ӿ� [12/26/2008 Square]
// // public:
// // 	long GetTeamQuestID()	{ return m_lTeamQuestID; }
// // 	void SetTeamQuestID(long l)	{ m_lTeamQuestID = l; }
// // 	//--
// // //	hash_map<CGUID,tagWSMemberInfo,hash_guid_compare>& GetTeamMemberInfo()	{ return m_hmapTeamMemberInfo; }
// // //	hash_map<CGUID,tagTeammate,hash_guid_compare>& GetTeamInfo()	{ return m_hmapTeamInfo; }
// // 	vector<tagTeammate>& GetTeammateList()		{ return m_vecTeammates; }
// // 	//-----------------------------------------------
// // 	void SetTeamRequesterGuid(const CGUID& guid)	{ m_TeamRequesterGuid = guid; }
// // 	const CGUID& GetTeamRequesterGuid()		{ return m_TeamRequesterGuid; }
// // private:
// // public:
// // 	vector<tagPlayerInfo>	 &GetPlayerInfo()			{ return m_vecPlayerInfo; }
// // 
// // 	vector<tagWSMemberInfo>  &GetTeamMateInfo()			{ return m_vecTeamMateInfo; }
// // 
// // 	vector<tagTeammate>		 &GetTeam()					{ return m_vecTeammates; }
// // 	// �쿴ָ��������Ƿ��Ǳ������
// // 	bool IsTeammate(const CGUID& id);
// // 	// ���ָ��ID��Ա����Ϣ
// // 	tagWSMemberInfo * GetWsMemberInfo(const CGUID& id);
// // 	// ��ú����öӳ�
// // 	const CGUID& GetCaptainID() const					{ return m_CaptainID; }
// // 	void SetCaptainID(const CGUID& id)					{ m_CaptainID = id; }
// // 	// ��ú����ö���
// // 	const CGUID& GetTeamID()							{ return m_TeamID; }
// // 	void SetTeamID(const CGUID& ID)						{ m_TeamID=ID; }
// // 	// ��ú�������ļ״̬
// // 	bool IsRecruitingTeam()								{ return m_bIsRecruitingTeam; }
// // 	void SetRecruitingTeam(bool b)						{ m_bIsRecruitingTeam=b; }
// // 	// ��ú����ö����Ա������
// // 	long GetTeammateAmount()							{ return m_lTeammateAmount; }
// // 	void SetTeammateAmount(long l)						{ m_lTeammateAmount=l; }
// // 	// ��ú����ö��������
// // 	const char * GetTeamname()							{ return m_strTeamname; }
// // 	void SetTeamname(const char *strName)				{ strcpy(m_strTeamname,strName); }
// // 	// ��ú����ö����Ƿ��������
// // 	bool IsTeamHasPassword()							{ return m_bTeamHasPassword; }
// // 	void SetTeamHasPassword(bool b)						{ m_bTeamHasPassword=b; }
// // 	// �Ƿ���Բ���������
// // 	bool IsOperforTeam();
// // 	// ����������Ƽ���
// // 	void SetLimtSkill(long ID, long lLev)				{ m_lLimtSkillID = ID; m_lLimtSkillLev = lLev;}
// // 
// // 	//��Ҫ����Lua����ȷ������Ϣ���ṩ�Ľӿ��жϺ���
// // 	// �Ƿ�Ϊһ���ӵĶӳ�
// // 	void SetIsTeamLeader(bool b)						{ m_bIsTeamLeader = b; }
// // 	bool IsTeamLeader()									{ return m_bIsTeamLeader; }
// // 
// // 	//----------------------------------------------------------------------------------------------------------------/*//////////////////////////////////////////////////////////////////////////*/
// // 	//	����
// // 	//----------------------------------------------------------------------------------------------------------------
// // public:
// // 
// // 	////////////////////07.12.17 XY��Ӳ�����������Ʒ����������ȴЧ���Ľӿ�///////////////////
// // 
// // 	//	list<tagShortCutGoods>* GetShortCutGoodsList() {return &m_listShortCutGoods;}	
// // 	bool  AddShortCutGoods(WORD wGoodsIndex);							//�������ΪwGoodsIndex����Ʒ���б���
// // 	bool  DelShortCutGoods(WORD wGoodsIndex);							//ɾ������ΪwGoodsIndex����Ʒ
// // 	void  SetGoodsRestoreTime(WORD wGoodsIndex,DWORD dwTime);			//��������ΪwGoodsIndex����Ʒ����ȴʱ��
// // 	void  SetGoodsCoolDownStartTime(WORD wGoodsIndex,DWORD dwTime);		//��������ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
// // 	DWORD GetGoodsRestoreTime(WORD wGoodsIndex);						//�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
// // 	DWORD GetGoodsCoolDownStartTime(WORD wGoodsIndex);					//�õ�����ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
// // 	void  SetGoodsUseState(WORD wGoodsIndex,bool bState);				//��������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
// // 	bool  GetGoodsUseState(WORD wGoodsIndex);							//ȡ������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
// // public:
// // 
// // 
// // 	//--------------------------------------------------------------------------------------------------------------------------
// // 	//���ϵͳ(�����л�,����)
// // 	//  [12/20/2008 Square]
// // 	//--------------------------------------------------------------------------------------------------------------------------
// // private:
// // 
// // 
// // 	// [12/20/2008 Square]
// // 	//�Ƿ���Vector����һЩ�� hash_map�ڴ�ռ�ô��ˣ� MartySa 
// // 	//hash_map<CGUID,FamilyInfo,hash_guid_compare> m_hmapFamily;
// // public:
// // //	hash_map<CGUID,FamilyInfo,hash_guid_compare> *GethmapFamily()	{ return &m_hmapFamily; }
// // 	bool AddFamily(FamilyInfo* info);
// // 	void DelFamilyByGuid(const CGUID& guid);
// // //	FamilyInfo* GetFamilyInfoByGuid(const CGUID& guid)	
// // // 	{
// // // 		hash_map<CGUID,FamilyInfo,hash_guid_compare>::iterator it = m_hmapFamily.find(guid);
// // // 		if( /*m_hmapFamily.find(guid)*/it != m_hmapFamily.end() )		//Ч�ʸ���
// // // 			return &((*it).second);
// // // 		return NULL;
// // // 	}
// // 	//���ڱ���ͺ˶��л��������
// // 	// 2009.1.13 MartySa
// // //	void ClearhmapFamilyData()					{ if( !m_hmapFamily.empty() ) { m_hmapFamily.clear(); } }
// // public:
// // 	const CGUID& GetFamilyGuid()	{return m_FamilyGuid;}
// // 	void  SetFamilyGuid(const CGUID& guid)	{ m_FamilyGuid = guid; }
// // 
// // 	const CGUID& GetFamilyMasterGuid()	{ return m_FamilyMasterGuid;}
// // 	void SetFamilyMasterGuid(const CGUID& guid)	{m_FamilyMasterGuid = guid; }
// // 
// // 	const long GetFamilyJobLev()	{return m_lFamilyJobLevel;}
// // 	void SetFamilyJobLev(long l)	{ m_lFamilyJobLevel = l; }
// // 
// // 	const char * GetFamilyName() const	{ return m_strFamilyName;}
// // 	void SetFamilyName(const char* strName)	{ strcpy(m_strFamilyName,strName);}
// // 
// // 	const char* GetFamilyTitle() const	{ return m_strFamilyTitle; }
// // 	void SetFamilyTitle(const char* strTitle)	{ strcpy(m_strFamilyTitle,strTitle); }
// // 
// // 	bool GetIsContendState()		{return m_bInContendState;}
// // 	void SetContendState(bool b)	{m_bInContendState = b;}
// // 
// // 	bool GetCityWarDiedState()	{return m_bCityWarDied;}
// // 	void SetCityWarDiedState(bool b)	{m_bCityWarDied = b;}
// // 
// // 	DWORD	GetFacIconCRC()						{return m_dwFacIconCRC;}
// // 	void	SetFacIconCRC(DWORD dwFacIconCRC)	{m_dwFacIconCRC = dwFacIconCRC;}
// // 
// // 	const CGUID& GetUnionID()			{return m_UnionGuid;}
// // 	void SetUnionID(const CGUID& ID)	{m_UnionGuid = ID;}
// // 
// // 	void SetEnemyFamily(set<CGUID,guid_compare>& setFamily)	{ m_EnemyFamily = setFamily; }
// // 	bool IsHaveEnemyFamily()								{ return /*m_EnemyFamily.size() > 0*/!m_EnemyFamily.empty(); }
// // 	bool IsEnemyFamily(const CGUID& familyGuid)
// // 	{
// // 		return m_EnemyFamily.find(familyGuid) != m_EnemyFamily.end();
// // 	}
// // 
// // 	bool IsHaveCityWarEnemyFamily()	{ return /*m_CityWarEnemyFamily.size() > 0*/!m_CityWarEnemyFamily.empty(); }
// // 	bool IsCityWarEnemyFamily(const CGUID& guid)
// // 	{
// // 		return m_CityWarEnemyFamily.find(guid) != m_CityWarEnemyFamily.end();
// // 	}
// // 
// // 	bool IsFriendFamily(const CGUID& familyGuid,const CGUID& unionGuid)
// // 	{
// // 		//��������Լ��ļ���guid����Ч,�����Ѻ�
// // 		if( familyGuid == CGUID::GUID_INVALID || m_FamilyGuid == CGUID::GUID_INVALID)
// // 			return false;
// // 		//������Լ�
// // 		if( familyGuid == m_FamilyGuid )
// // 			return true;
// // 		//�����������Լ���ͬ��guid����Ч,����Ѻ�
// // 		if( unionGuid == CGUID::GUID_INVALID || m_UnionGuid == CGUID::GUID_INVALID)
// // 			return false;
// // 		//������Լ�
// // 		if( unionGuid == m_UnionGuid )
// // 			return true;
// // 		//����
// // 		return false;
// // 	}
// // 
// // 
// // 	/*******************************************************************************\
// // 	|*************************   ����ӿ�/����   ***********************************|
// // 	\*******************************************************************************/
// // private:
// // public:
// // 	//����
// // 	//	BOOL DecordRoleRecordFromByteArray(BYTE *pByte,long &pos, tagRoleRecord *record);
// // 	//����һ������
// // 	BOOL AddOneRoleRecord(tagRoleRecord*& pRole);
// // 	//
// // 	BOOL AddOneRoleRecord(LONG roleid,LONG step);
// // 	//ɾ��һ������
// // 	//const char* RemoveQuest(long lQuestID);
// // 	//����
// // 	list<tagRoleRecord*>*	GetQuestList()		{ return &m_listRoleRecord; }
// // 	//��������
// // 	list<tagRoleRecord*>*	GetMainRoleList() { return &m_listRoleRecord; }	
// // 	//�ж��Ƿ����һ������
// // 	BOOL IsRoleBeingByID(long id);
// // 	//�ڳ�GUID
// // 	void SetCarriageID(const CGUID& id)	{m_CarriageID=id;}
// // 	const CGUID& GetCarriageID()		{return m_CarriageID;}
// // 	//	����ID
// // 	set<CGUID> *GetPetIDSet()	{return &m_setPetID;}
// // 	bool HasPetArount();										//�ж���Χ��û���Լ��ĳ���
// // 	long GetPetAmount();										//�����Լ���Χ���������
// // 
// // private:
// // public:
// // 	//	void DecordMedalFromByteArray(BYTE* pByte,long &lPos);
// // 	void DecordMedalFromDataBlock(DBReadSet& dbr);
// // 
// // 	//--------------------------------------------------------------------------------------------------------
// // 	//��Ʒ�ϳɲ���
// // 	//---------------------------------------------------------------------------------------------------------
// // private:
// // 
// // public:
// // 	tagSynthesizeGoods &GetSyntsGoods() { return m_SyntsGoods; }	//��ȡ�ϳ���Ʒ�ṹ�����
// // 
// // 
// // 	//--------------------------------------------------------------------------------------
// // 	//���Ѳ���
// // 	//--------------------------------------------------------------------------------------
// // public:
// // 
// // private:
// // 	SPanel		m_PanelGroup[MAXNUM_PANELS];
// // public:
// // 	//��ȡ�����ڷ�������id
// // 	int			GroupIDtoPanelID(int groupid)	     { return groupid/10-1; }
// // 	SPanel      &GetSPanle(int spanelid)			 { return m_PanelGroup[spanelid]; }
// // 
// // 	void		SetLinkManPanleData(tagLinkman *data, int spanelid);
// // 	tagLinkman	*GetLinkManPanleDataForName(int spanelid, const string &strName);
// // 	void		DelteLinkManPanleData(int spanelid,  const string &strName);
// // 	//ͨ������������������Ƿ������Ѿ���ӹ��������Ӷ������ж�Lua���Ƿ�÷������AutoMsg
// // 	bool		isLinkManRepeatForName(const string &strName); 
// // 	void		ClearLinkmanData();
// // 
// // 	//------------------------------------------------------------------------------------------
// // 	//��ҽ��� �¼�
// // 	//-------------------------------------------------------------------------------------------
// // public:
// // 
// // 
// // private:
// // 
// // public:
// // 	bool IsRequestTrade()				{ return m_bRequestTrade; }
// // 	void SetRequestTrade(bool b)		{ m_bRequestTrade = b; }
// // 	bool IsCanTrade()					{ return m_bCanTread; }
// // 	void SetCanTrade(bool b)			{ m_bCanTread = b; }
// // 
// // 	void AddMyTradeGoods(CGoods* pGoods,long lPos,long lSocketID);
// // 	void AddYouTradeGoods(CGoods* pGoods,long lPos);
// // 	//void ClearTradeGoods();
// // 
// // 	const CGUID		&GetTradeRequestGUID()					{return m_TradeRequestGUID;}	//��ý��׷��𷽵�GUID(��ΪLUA�ӿ�)
// // 	void			SetTradeRequestGUID(CGUID guid)			{m_TradeRequestGUID = guid;}	//���ý��׷��𷽵�GUID(����S2C��Ϣʱ����)
// // 
// // 	const CGUID		&GetTradeSessionID()					{return m_TradeSessionGUID;}	//��ý���ʱ��SESSIONID(��ΪLUA�ӿ�)
// // 	void			SetTradeSessionID(CGUID &guid)			{m_TradeSessionGUID = guid;}	//���ý���ʱ��SESSIONID(����S2C��Ϣʱ����)
// // 
// // 	const CGUID		&GetMyID()								{return m_MyID;}	//����Լ���GUID(��ΪLUA�ӿ�)
// // 	void			SetMyID(CGUID &guid)					{m_MyID = guid;}	//�����Լ���GUID(����S2C��Ϣʱ����)
// // 	const CGUID		&GetYourID()							{return m_YourID;}	//��öԷ���GUID(��ΪLUA�ӿ�)
// // 	void			SetYourID(CGUID &guid)					{m_YourID = guid;}	//���öԷ���GUID(����S2C��Ϣʱ����)
// // 
// // 	long			GetlMPlugID()							{return m_lMPlugID;}	//����Լ��Ľ���ʱ�����ID(��ΪLUA�ӿ�)
// // 	void			SetlMPlugID(long l)						{m_lMPlugID = l;}		//�����Լ��Ľ���ʱ�����ID(����S2C��Ϣʱ����)
// // 	long			GetlYPlugID()							{return m_lYPlugID;}	//��öԷ�����ʱ�����ID(��ΪLUA�ӿ�)
// // 	void			SetlYPlugID(long l)						{m_lYPlugID = l;}		//���öԷ�����ʱ�����ID(����S2C��Ϣʱ����)
// // 	bool			GetbMeIsLocked()						{return m_bMeIsLocked;}	
// // 	void			SetbMeIsLocked(bool b)					{m_bMeIsLocked = b;}	
// // 	bool			GetbYouIsLocked()						{return m_bYouIsLocked;}//��öԷ��Ƿ��Ѿ���������(��ΪLUA�ӿ�)
// // 	void			SetbYouIsLocked(bool b)					{m_bYouIsLocked = b;}	
// // 	bool			GetTradeResult()						{return m_bResult;}		//��ý��׵Ľ��(��ΪLUA�ӿ�)
// // 	void			SetTradeResult(bool b)					{m_bResult = b;}		//���ý��׵Ľ��(����S2C��Ϣʱ����)
// // 
// // 	//--------------------------------------------------------------------------------------------------
// // 	//NPC�̵꽻��
// // 	//--------------------------------------------------------------------------------------------------
// // private:
// // public:
// // 	void SetShopNPCGuid(const CGUID &guid)	{m_ShopNPCGuid = guid; }	//���õ�ǰ���׵�NPC��GUID(����S2Cʱ����)
// // 	CGUID &GetShopNPCGuid()				{return m_ShopNPCGuid; }	//��ȡ��ǰ���׵�NPC��GUID(��ΪLUA�ӿ�)
// // 	void SetGoodsNum(DWORD num)			{m_dwGoodsNum = num;}		//���õ�ǰһ���ж�������Ʒ������(����S2Cʱ����)
// // 	void AddShopGoodsID(DWORD goodsid)  {m_ShopGoodsID.push_back(goodsid);}		//�����п��������Ʒ��id����(����S2Cʱ����)
// // 	void AddEveryGoodsNum(DWORD num)	{m_EveryGoodsNum.push_back(num);}		//��ÿ�ֿ��������Ʒ����������(����S2Cʱ����)
// // 	DWORD GetBuyGoodsID();		//�����ȡһ��׼���������Ʒ��ID(��ΪLUA�ӿ�)
// // 	DWORD GetEveryGoodsNum()			{return m_EveryGoodsNum.at(m_RandomNum);}	//���ÿ����Ʒ���Թ��������(��ΪLUA�ӿ�)
// // 
// // 
// // private:
// // 
// // 	//----------------------------------------------------------------------------------------------------------------
// // 	//�ʼ����
// // 	//----------------------------------------------------------------------------------------------------------------
// // 
// // public:	
// // 
// // 
// // 
// // private:
// // 
// 
	//////////////////////////////////////////////////////////////////////////
	// �콱 [11/3/2009 Square]
	//////////////////////////////////////////////////////////////////////////
public:
 	void AddBounsInfo(tagBounsInfo* info);  
 	void QuestGetBouns(long lSocket);
  	void OnGetBounsResult(const CGUID & guid,bool bSucced);
	void DecodeGeniusFromDataBlock(DBReadSet& setReadDB);
private:
	list<tagGtBounsInfo*>	m_listGtBounsInfo;
// // 
// // 	// For DriveClient
// // 	// ��Ҳ�������󣬽����״̬
// // 	// by MartySa 2009.2.11
// // 
// // 
// // protected:
// // public:
// // 	bool		GetCharacterPageState()					    { return m_bCharacterPageState; }
// // 	bool		GetSkillPageState()						    { return m_bSkillPageState; }
// // 	bool		GetDepotUIPageState()					    { return m_bDepotUIPageState; }
// // 	bool		GetMailPageState()						    { return m_bMailPageState; }
// // 	bool		GetFriendsUIPageState()					    { return m_bFriendsUIPageState; }
// // 	bool		GetTeamPageState()						    { return m_bTeamPageState; }
// // 	bool		GetItemPageState()						    { return m_bItemPageState; }
// // 	bool		GetSubBackUIPageState()					    { return m_bSubBackUIPageState; }
// // 
// // 	void		SetCharacterPageState(bool state)			{ m_bCharacterPageState = state; }
// // 	void		SetSkillPageState(bool state)				{ m_bSkillPageState = state; }
// // 	void		SetDepotUIPageState(bool state)				{ m_bDepotUIPageState = state; }
// // 	void		SetMailPageState(bool state)				{ m_bMailPageState = state; }
// // 	void		SetFriendsUIPageState(bool state)			{ m_bFriendsUIPageState = state; }
// // 	void		SetTeamPageState(bool state)				{ m_bTeamPageState = state; }
// // 	void		SetItemPageState(bool state)				{ m_bItemPageState = state; }
// // 	void		SetSubBackUIPageState(bool state)			{ m_bSubBackUIPageState = state; }
// // 
// // 	// ����
// // 	// By MartySa 2009.6.15
// // private:
// // 
//  public:
//  	bool		GetIsAnswerQuestion()	    { return bIsAnswerQuestion; }
//  	void		SetIsAnswerQuestion(bool b)	{ bIsAnswerQuestion = b;}
// 
// 
// 	//���ѡ��һ������󱣴�� GUID
// private:
// 
// public:
// 	CGUID		&GetSelectedShapeGuid()						{ return m_SelectedShapeGuid; }
// 	void		SetSelectedShapeGuid(CGUID guid)			{ m_SelectedShapeGuid = guid; }
// 
// 	//ȡ����ҵ�ǰ���ϱ�����Goods���� For DriveClient
// 	// by MartySa 2009.2.19
// public:
// 	long		GetPlayerPackGoodsSum();
};