#pragma once

#include "Business/BusinessPackage.h"
#include "Container/AmountLimitGoodsContainer.h"
#include "Container/Bank.h"
#include "Container/Depot.h"
#include "Container/EquipmentContainer.h"
#include "Container/SubpackContainer.h"
#include "Container/VolumeLimitGoodsContainer.h"
#include "Container/SpecialEquipContainer.h"
#include "Container/Wallet.h"
#include "Mail/Mail.h"
#include "MedalContainer.h"
#include "MoveShape.h"
#include "OrganizingSystem/OrganizingCtrl.h"
#include "PlayerDepot.h"
#include "Session/WorldServerSession.h"
#include "SpriteSystem.h"
#include "../../../Public/Common/QuestRecord.h"
#include "../../../Public/Common/PlayerSetupOnServer.h"
#include "../../../Public/Common/PlayerDef.h"
#include "../../../Public/ServerPublic/Server/Common/DeputyOccuRecord.h"
#include "../../../Public/ServerPublic/Server/BaseDef/PhaseSeverDef.h"
#include "../../../Public/Setup/IncrementShopList.h"

class VariableList;
class CGoods;
class CPet;

typedef map<CGUID, CPet*> PETS;
typedef const PETS& C_PETS;
typedef PETS::iterator ITR_PET;
typedef PETS::const_iterator C_ITR_PET;

class CPlayer :
	public CMoveShape
{
public:
	CPlayer(void);

	virtual ~CPlayer(void);

public:
	// ���＼��
	struct tagSkill
	{
		DWORD	wID;					//	���
		WORD	wLevel;					//	�ȼ�
		DWORD	wCooldown;				//  ��ȴʱ��
	};

	struct tagShopSelledGoods 
	{
		DWORD dwSelledPrice;
		CGoods* pGoods;
	};

	// ����������������Ҫ�浽���ݿ��е����� BAIYUN@20090602
	struct dbAntiCheatData
	{
		int   m_nActFactor;				// ��Ծ��
		int   m_nAccWrongTimes;			// �ۼƴ�����
		int   m_nCurWrongTimes;			// ��ǰ������
		int   m_nCurRightTimes;			// ������Դ���
		int   m_nLastGameTime;		    // �ϴ���Ϸʱ��
		unsigned char m_bBanFlag;		// ��ű�־
		unsigned char m_bScriptExecuted; // Ϊ1ʱ��ʾ֮ǰ�Ĵ���ű�δ����
		int m_nBonusTimes;				// ��ǰ��������
		unsigned char m_bRHAnswered;	/// �Ƿ��ڻƽ�ʱ�������ȷ��
	};

	// ������������
	struct AntiCheatData 
	{
		AntiCheatData()
		{
			memset(this, 0, sizeof(*this));
		}
		dbAntiCheatData m_dbData;
		int	            m_nAntiCheatPK;		    // PKֵ ע:��PKValue��ͬ	
		DWORD           m_dwLastAnswerTime;     // �ϴδ���ʱ��
		long			m_nRefuseTimes;			// �ܾ��ش���Ŀ����
		long			m_nAtkAct;				// ս����Ծ��
	};
	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
public:
	tagPlayerProperty& GetPlayerProperty(void) { return m_Property; }


	list<tagSkill>& GetSkillList()				{return m_listSkillID;}

    map<CGUID, tagBrotherInfo>& GetBrotherMap() { return m_mapBrotherInfo; }

	void SetSaveDepotFlag(bool bFlag){m_bSaveDepot=bFlag;}
	bool IsSaveDepot(){return m_bSaveDepot;}

	CSubpackContainer* GetSubpackContainer(void) { return m_pSubpackContainer; }

private:

	//��ʱ�����������NPC�̵����Ʒ��������Ʒ�ع�����
	list<tagShopSelledGoods> m_listNpcShopSellGoods;

	map<DWORD,DWORD> m_mapCredit;
	map<DWORD,DWORD> m_mapInvestRecord;
	long m_lInvestLimitTimeMark;
	CVolumeLimitGoodsContainer* m_cOriginPack;
	CSubpackContainer* m_pSubpackContainer;
	VOID gcPacks();


public:
	VOID SetCredit(DWORD dwAreaId,DWORD dwCredit);
	VOID AddCredit(DWORD dwAreaId,long lVal);
	DWORD GetCredit(DWORD dwAreaId);
	VOID ClearCredit(){m_mapCredit.clear();}
	CVolumeLimitGoodsContainer* getOriginPack(){return m_cOriginPack;}
	CSubpackContainer* getSubpackContainer(){return m_pSubpackContainer;}

	BOOL UnserializeSubDepot(BYTE* pStream, LONG& lOffset);
	BOOL UnserializeSubDepot(DBReadSet& setReadDB);

	VOID setSubPackGoods(BYTE pos,CGoods** pGoods);

	BOOL SerializeSubpack(vector<BYTE>* pStream,PLAYER_EXTEND_ID extendId);
	BOOL UnserializeSubpack(BYTE* pStream, LONG& lOffset);
	BOOL UnserializeSubpack(DBReadSet& setReadDB);

	BOOL SerializeSubDepot(vector<BYTE>* pStream, LONG eContainerID);

	hash_map<CGUID,CGoods*,hash_guid_compare>* GetOriginPackGoodsMap(){return m_cOriginPack->GetGoodsMap();}
	hash_map<CGUID,CGoods*,hash_guid_compare>* GetSubPackGoodsMap(int i);
	map<eEquipType,CGoods*>* GetEquipmentGoodsMap(){return m_cEquipment.GetGoodsMap();}
    hash_map<CGUID,CGoods*, hash_guid_compare>* GetEquipGodGoodsMap() { return m_cEquipmentGod.GetGoodsMap(); }
    hash_map<CGUID,CGoods*, hash_guid_compare>* GetEquipLandGoodsMap()  { return m_cEquipmentLand.GetGoodsMap(); }
    hash_map<CGUID,CGoods*, hash_guid_compare>* GetEquipHumanGoodsMap() { return m_cEquipmentHuman.GetGoodsMap(); }

	map<DWORD,DWORD>* GetCreditMap(){return &m_mapCredit;}

	VOID PushbakNpcShopSellGoods(CGoods* pGoods,DWORD dwPrice);
	CGoods* FetchNpcShopSellGoods(const CGUID& guid);
	CGoods* FindNpcShopSellGoods(CGUID& guid,DWORD* dwPrice);
	VOID ReleaseNpcShopSellGoods();


private:
	long		m_lTicket;
	map<DWORD,DWORD>  m_mapGoodsIdCoolDown;
	map<DWORD,DWORD>  m_mapClassIdCoolDown;
public:
	long GetTicket()				{ return m_lTicket; }
	void SetTicket(long	lTicket)	{ m_lTicket = lTicket; }

	map<DWORD,DWORD>& GetClassIdCooldownMap(){return m_mapClassIdCoolDown;}
	map<DWORD,DWORD>& GetGoodsIdCooldownMap(){return m_mapGoodsIdCoolDown;}

public:

	CEquipmentContainer			m_cEquipment;
    CSpecialEquipContainer      m_cEquipmentGod;
    CSpecialEquipContainer      m_cEquipmentLand;
    CSpecialEquipContainer      m_cEquipmentHuman;
	CWallet						m_cWallet;
	CWallet				        m_cSilverWallet;
	CWallet				        m_cGem;
		

	//!				�ֿ�
	PlayerDepot		m_PlayerDepot;
private:
	//changed by chenaly .....07.4.5.................
	//tagBaseProperty m_BaseProperty;		//	�������� 
	//tagProperty		m_Property;			//	ʵ������
	tagPPro   m_Property;                   //  ��������
	tagPFPro  m_BaseFightProperty;          //  ս����������
	tagPFPro  m_FactFightProperty;           // ս��ʵ������
	tagPlayerAppendProperty  m_AppendProperty;//���渽������
	tagPlayerElementProperty	m_arrElementProperty[MT_Max];//Ԫ������
	// end change......................................

    bool m_bSaveDepot;

	list<tagSkill>	m_listSkillID;		//	�����б�
    map<CGUID, tagBrotherInfo>    m_mapBrotherInfo;      //�������

    ///@by:expter �洢����츳�б�
private:
    std::map<long,long>         m_GeniusList;//�츳���ã�iD��Ӧ�ȼ�

public:
    /// �����츳����
    void  AddGenius(long Geniusid,long Lvl)     {  m_GeniusList[Geniusid]=Lvl;}
    std::map<long,long>&  GetGeniusList()       {  return m_GeniusList;       }

public:

	//changed by chenaly.....07.4.5..............
	//�������� �ӿ�  m_Property
	eOccupation	GetOccupation()				{return m_Property.byOccu;}			//ְҵ
	void	SetOccupation(BYTE l)	{m_Property.byOccu=(eOccupation)l;}
	//��ְҵ
	BYTE	GetDOccupation()				{return m_Property.byAssOccu;}
	void	SetDOccupation(eDeputyOccu l)	{m_Property.byAssOccu=l;}


	BYTE	GetSex()					{return m_Property.bySex;}				//�Ա�
	void	SetSex(BYTE l)				{m_Property.bySex=l;}
	BYTE	GetHeadPic()				{return m_Property.byHairPic;}			// ͷ��ͼƬ
	void	SetHeadPic(BYTE l)			{m_Property.byHairPic=l;} 
	BYTE	GetFacePic()				{return m_Property.byFacePic;}			//�沿ͼƬ
	void	SetFacePic(BYTE l)			{m_Property.byFacePic=l;}
	DWORD	GetHP()						{return m_Property.dwHp;}				//��ǰhp
	void	SetHP(DWORD l)				{m_Property.dwHp=l;}
	WORD	GetMP()						{return m_Property.wMp;}				//��ǰmp
	void	SetMP(WORD l)				{m_Property.wMp=l;}
	WORD    GetEnergy()                 {return m_Property.wEnergy;}			//��ǰ����
	void    SetEnergy(WORD l)           {m_Property.wEnergy=l;}
	WORD    GetStamina()                {return m_Property.wStamina;}			//��ǰ����
	void    SetStamina(WORD l)          {m_Property.wStamina=l;}
	WORD	GetLevel()					{return m_Property.wLevel;}				//�ȼ�
	void	SetLevel(WORD l)			{m_Property.wLevel =l;}
	DWORD	GetExp()					{return m_Property.dwExp;}			    //����ֵ
	void	SetExp(DWORD l)				{m_Property.dwExp=l;}
	DWORD	GetExpMultiple()			{return m_Property.dwExpMultiple;}		//���鱶��
	void	SetExpMultiple(DWORD l)		{m_Property.dwExpMultiple=l;}
	DWORD	GetPresentExp()				{return m_Property.dwPresentExp;}		//���;���ֵ
	void	SetPresentExp(DWORD l)		{m_Property.dwPresentExp=l;}
	DWORD   GetUpgradeExp()             { return m_Property.dwUpgradeExp ;}
	void    SetUpGradeExp(DWORD l)      { m_Property.dwUpgradeExp=l;}
    ushort  GetTaoismLvl()              { return m_Property.wTaoismLvl;}
    void    SetTaoismLvl(ushort l)      { m_Property.wTaoismLvl = l; }
    ulong   GetTaoismExp()              { return m_Property.dwTaoismExp; }
    void    SetTaoismExp(ulong l)       { m_Property.dwTaoismExp = l; }
    ulong   GetRelationVal()            { return m_Property.dwRelationVal; }
    void    SetRelationVal(ulong l)     { m_Property.dwRelationVal = l; }
    ulong   GetHiddenVirtue()           { return m_Property.dwHiddenVirtue; }
    void    SetHiddenVirtur(ulong l)    { m_Property.dwHiddenVirtue = l; }
    ushort  GetVigour()                 { return m_Property.wVigour; }
    void    SetVigour(ushort l)         { m_Property.wVigour = l; }
    ushort  GetThew()                   { return m_Property.wThew; }
    void    SetThew(ushort l)           { m_Property.wThew = l; }
    short   GetPoint2Str()              { return m_Property.wPoint2Str; }
    void    SetPoint2Str(short s)       { m_Property.wPoint2Str = s; }
    short   GetPoint2Dex()              { return m_Property.wPoint2Dex; }
    void    SetPoint2Dex(short s)       { m_Property.wPoint2Dex = s; }
    short   GetPoint2Con()              { return m_Property.wPoint2Con; }
    void    SetPoint2Con(short s)       { m_Property.wPoint2Con = s; }
    short   GetPoint2Intellect()              { return m_Property.wPoint2Intellect; }
    void    SetPoint2Intellect(short s)       { m_Property.wPoint2Intellect = s; }
    short   GetPoint2Spiritualism()              { return m_Property.wPoint2Spiritualism; }
    void    SetPoint2Spiritualism(short s)       { m_Property.wPoint2Spiritualism = s; }

	BYTE	GetCountry()				{ return m_Property.byCountry; }		  //����
	void	SetCountry(BYTE l)			{ m_Property.byCountry = l; }
	long	GetCountryContribute()		{ return m_Property.lCountryContribute; } //���ҹ���
	void	SetCountryContribute(long l){ m_Property.lCountryContribute  = l; }   
    BYTE*   GetCountryCredit()          { return (BYTE*)&m_Property.dwCountryCredit[0]; }
    void    SetCountryCredit(BYTE* pBuf, long lSize)    { memcpy(&m_Property.dwCountryCredit[0], pBuf, lSize); }
    BYTE    GetCountryStatus()          { return m_Property.byCountryStatus; }  //��Ӫ��λ
    void    SetCountryStatus(BYTE l)    { m_Property.byCountryStatus = l; }
	DWORD	GetSpouseId()				{return m_Property.dwSpouseID;}			//��ż���
	void	SetSpouseId(DWORD l)		{m_Property.dwSpouseID=l;}
	DWORD   GetSpouseParam()            { return m_Property.dwSpouseParam;}		//��ż��ϵֵ 
	void    SetSpouseParam(DWORD l)     { m_Property.dwSpouseParam=l;}
	BYTE    GetBusinessLevel()          { return m_Property.byBusinessLevel;}	//��ҵ�ȼ�  
	void    SetBusinessLevel(BYTE l)    { m_Property.byBusinessLevel=l;}
	DWORD   GetBusinessExp()            { return m_Property.dwBusinessExp;}		//��ҵ����  
	void    SetBusinessExp(DWORD l)     { m_Property.dwBusinessExp=l;} 
	DWORD   GetPkCount()                { return m_Property.dwPkCount;}			//pkɱ��������� 
	void    SetPkCount(DWORD l)         { m_Property.dwPkCount=l;}
	DWORD   GetPkValue()                { return m_Property.dwPkValue;}			//����ֵ 
	void    SetPkValue( DWORD l)        { m_Property.dwPkValue=l;}
	DWORD   GetPVPCount()               { return m_Property.dwPVPCount;	}		//���PVPɱ��������� 
	void    SetPVPCount(DWORD l)		{ m_Property.dwPVPCount=l;}
	DWORD   GetPVPValue()               {return m_Property.dwPVPValue;}			//���PVPֵ(����ֵ)
	void    SetPVPValue(DWORD l)        { m_Property.dwPVPValue=l;}
	DWORD	GetHotKey(long lPos)		{return m_Property.dwHotKey[lPos];}		//�ȼ�
	DWORD*	GetHotKeyArray(void)		{return &m_Property.dwHotKey[0];}		//�ȼ�
	void	SetHotKey(long lPos,DWORD l){m_Property.dwHotKey[lPos]=l;}
	BYTE    GetPKOnOff()                {return m_Property.byPKOnOff;}			//pk״̬ 
	void    SetPKOnOff(BYTE L)          {m_Property.byPKOnOff=L;}
	bool	GetShowCountry(void)		{return m_Property.bShowCountry;}
	void	SetShowCountry(bool bShow)	{m_Property.bShowCountry = bShow;}
	void	SetOccuLvl(BYTE* pBuf,long lSize) {memcpy(&m_Property.byOccuLvl[0],pBuf,lSize);}
	BYTE*	GetOccuLvl()				{return &m_Property.byOccuLvl[0];}
	void	SetOccuExp(BYTE* pBuf,long lSize) {memcpy(&m_Property.dwOccuExp[0],pBuf,lSize);}
	BYTE*	GetOccuExp()				{return (BYTE*)&m_Property.dwOccuExp[0];}
	void	SetOccuSP(BYTE* pBuf,long lSize) {memcpy(&m_Property.dwOccuSP[0],pBuf,lSize);}
	BYTE*	GetOccuSP()					{return (BYTE*)&m_Property.dwOccuSP[0];}

	long GetCurPKCP() const { return m_Property.lCurPKCP; }
	void SetCurPKCP( long v ) 	{ m_Property.lCurPKCP = v; }

	//pk����״̬���趨���ýӿ�
	bool	IsPk_Normal()				{return true;}							//PK����-��ͨ
	void	SetPk_Normal(bool b)		{return ;}
	bool	IsPk_Team()					{return true;}							//PK����-����
	void	SetPk_Team(bool b)			{return ;}
	bool	IsPk_Union()				{return true;}							//PK����-�л�
	void	SetPk_Union(bool b)			{return ;}
	bool	IsPk_Badman()				{return true;}							//PK����-�ﷸ
	void	SetPk_Badman(bool b)		{return ;}
	bool	IsPk_Country()				{return true;}							//pk�������
	void	SetPk_Country(bool b)		{return ;}

	DWORD	GetLastExitGameTime()		{return m_Property.dwLastExitGameTime;}
	void	SetLastExitGameTime(DWORD l){m_Property.dwLastExitGameTime=l;}

	/////////����ս������ �ӿ� m_BaseFightProperty
	DWORD	GetBaseMaxHP()					{return m_BaseFightProperty.dwMaxHp;}		//����
	void	SetBaseMaxHP(DWORD l)			{m_BaseFightProperty.dwMaxHp=l;}
	WORD	GetBaseMaxMP()					{return m_BaseFightProperty.wMaxMp;}		//����
	void	SetBaseMaxMP(WORD l)			{m_BaseFightProperty.wMaxMp=l;}
	WORD    GetBaseMaxStamina()             {return m_BaseFightProperty.wMaxStamina;}	//����
	void    SetBaseMaxStamina(WORD l)       {m_BaseFightProperty.wMaxStamina=l;}
	WORD	GetBaseMaxEnergy()				{return m_BaseFightProperty.wMaxEnergy;}	//����
	void	SetBaseMaxEnergy(WORD l)		{m_BaseFightProperty.wMaxEnergy = l;}
	WORD	GetBaseHpRecoverSpeed()			{return m_BaseFightProperty.wHpRecoverSpeed;}//��hp
	void	SetBaseHpRecoverSpeed(WORD l)	{m_BaseFightProperty.wHpRecoverSpeed=l;}
	WORD    GetBaseFightHpRecoverSpeed()          {return m_BaseFightProperty.wFightHpRecoverSpeed ;} //ս����hp
	void    SetBaseFightHpRecoverSpeed(WORD l)    {m_BaseFightProperty.wFightHpRecoverSpeed=l;}
	WORD	GetBaseMpRecoverSpeed()			{return m_BaseFightProperty.wMpRecoverSpeed;}//��mp
	void	SetBaseMpRecoverSpeed(WORD l)	{m_BaseFightProperty.wMpRecoverSpeed=l;}   
	WORD    GetBaseFightMpRecoverSpeed()          {return m_BaseFightProperty.wFightMpRecoverSpeed;} //ս����mp
	void    SetBaseFightMpRecoverSpeed(WORD l)    {m_BaseFightProperty.wFightMpRecoverSpeed=l;}
	short   GetBaseFightEnerRecoverSpeed()        {return m_BaseFightProperty.wFightEnerRecoverSpeed;}	//ս�������ָ��ٶ� 
	void    SetBaseFightEnerRecoverSpeed(short l) {m_BaseFightProperty.wFightEnerRecoverSpeed=l;}
	WORD    GetBaseStamRecoverSpeed()       {return m_BaseFightProperty.wStamRecoverSpeed;}//������ 
	void    SetBaseStamRecoverSpeed(WORD l) {return;}									 //.....
    short   GetBaseFightStamRecoverSpeed()  { return m_BaseFightProperty.wFightStamRecoverSpeed; }
    void    SetBaseFightStamRecoverSpeed(short l)   { m_BaseFightProperty.wFightStamRecoverSpeed = l; }
    
    short   GetBaseManly()                  { return m_BaseFightProperty.wManly; }
    void    SetBaseManly(short l)           { m_BaseFightProperty.wManly = l; }
    short   GetBasePerverseTrend()          { return m_BaseFightProperty.wPerverseTrend; }
    void    SetBasePerverseTrend(short l)   { m_BaseFightProperty.wPerverseTrend = l; }
    short   GetBaseComplaint()              { return m_BaseFightProperty.wComplaint; }
    void    SetBaseComplaint(short l)       { m_BaseFightProperty.wComplaint = l; }
    short   GetBaseMurderousLook()          { return m_BaseFightProperty.wMurderousLook; }
    void    SetBaseMurderousLook(short l)   { m_BaseFightProperty.wMurderousLook; }

    short   GetBaseStr()                    { return m_BaseFightProperty.wStr; }
    void    SetBaseStr(short s)             { m_BaseFightProperty.wStr = s; }
    short   GetBaseDex()                    { return m_BaseFightProperty.wDex; }
    void    SetBaseDex(short s)             { m_BaseFightProperty.wDex = s; }
    short   GetBaseCon()                    { return m_BaseFightProperty.wCon; }
    void    SetBaseCon(short s)             { m_BaseFightProperty.wCon = s; }
    short   GetBaseIntellect()              { return m_BaseFightProperty.wIntellect; }
    void    SetBaseIntellect(short s)       { m_BaseFightProperty.wIntellect = s; }
    short   GetBaseSpiritualism()           { return m_BaseFightProperty.wSpiritualism; }
    void    SetBaseSpiritualism(short s)    { m_BaseFightProperty.wSpiritualism = s; }
    short   GetBasePoint()                  { return m_BaseFightProperty.wPoint; }
    void    SetBasePoint(short s)           { m_BaseFightProperty.wPoint = s; }
    short   GetBaseParry()                  { return m_BaseFightProperty.wParry; }		 //�м�
    void    SetBaseParry(short l)           { m_BaseFightProperty.wParry=l; }
	
	WORD    GetBaseBlock()                  {return m_BaseFightProperty.wFightBlock ;}	 //��
	void    SetBaseBlock(WORD l)            {m_BaseFightProperty.wFightBlock =l;}
	
	WORD    GetBaseDodge()                  {return m_BaseFightProperty.wDodge;}		 //����
	void    SetBaseDodge(WORD l)            {m_BaseFightProperty.wDodge=l;}
	//	WORD    GetBaseShot()                   {return m_BaseFightProperty.wShot;}			 //���
	//	void    SetBaseShot(WORD l)             {m_BaseFightProperty.wShot=l;}
	WORD    GetBaseCharm()                  {return m_BaseFightProperty.wCharm;}		 //����
	void    SetBaseCharm(WORD l)            {m_BaseFightProperty.wCharm=l;}
	WORD    GetBaseHit()                    {return m_BaseFightProperty.wHit;}			 //����
	void    SetBaseHit(WORD l)              {m_BaseFightProperty.wHit=l;}
	WORD    GetBaseAbsorb()                 {return m_BaseFightProperty.wAbsorb;}		 //רע
	void    SetBaseAbsorb(WORD l)           {m_BaseFightProperty.wAbsorb=l;}
	WORD    GetBaseDef()                    {return m_BaseFightProperty.wDef;}			 //�������
	void    SetBaseDef(WORD l)              {m_BaseFightProperty.wDef=l;}
	WORD    GetBaseMdef()                   {return m_BaseFightProperty.wMdef;}			 //ħ������
	void    SetBaseMdef(WORD l)             {m_BaseFightProperty.wMdef=l;}
	WORD    GetBaseCri()                    {return m_BaseFightProperty.wCri;}			 //������
	void    SetBaseCri(WORD l)              {m_BaseFightProperty.wCri=l;}
	WORD    GetBaseMAtkSpeed()              {return m_BaseFightProperty.wMAtkSpeed;}	 //ʩ���ٶ�
	void    SetBaseMAtkSpeed(WORD l)        {m_BaseFightProperty.wMAtkSpeed=l;}
	WORD    GetBaseBuffPower()              {return m_BaseFightProperty.wBuffPower;}	 //buffer����ǿ��
	void    SetBaseBuffPower(WORD l)        {m_BaseFightProperty.wBuffPower=l;}
	WORD    GetBaseMinAtk()                 {return m_BaseFightProperty.wMinAtk;}		 //��С������
	void    SetBaseMinAtk(WORD l)           {m_BaseFightProperty.wMinAtk=l;}
	WORD    GetBaseMaxAtk()                 {return m_BaseFightProperty.wMaxAtk;}		 //��󹥻���
	void    SetBaseMaxAtk(WORD l)           {m_BaseFightProperty.wMaxAtk=l;}
	WORD    GetBaseMAtk()                   {return (WORD)m_BaseFightProperty.wMAtk;}			 //ħ��������
	void    SetBaseMAtk(WORD l)             {m_BaseFightProperty.wMAtk=l;}
	WORD    GetBaseMCri()                   {return (WORD)m_BaseFightProperty.wMCri;}			 //ħ��������
	void    SetBaseMCri(WORD l)             {m_BaseFightProperty.wMCri=l;}
	WORD    GetBaseBloodSuk()               {return m_BaseFightProperty.wBloodSuck;}		 //����������
	void    SetBaseBloodSuk(WORD l)         {m_BaseFightProperty.wBloodSuck=l;}
	WORD    GetBaseManaSuck()               {return m_BaseFightProperty.wManaSuck;}		 //����������
	void    SetBaseManaSuck(WORD l)         {m_BaseFightProperty.wManaSuck=l;}


	WORD    GetBaseCriDef()                     {return m_BaseFightProperty.wCriDef;}//����������
	void    SetBaseCriDef(WORD l)               {m_BaseFightProperty.wCriDef=l;}
	WORD    GetBaseMCriDef()                    {return m_BaseFightProperty.wMCriDef;}//ħ����������
	void    SetBaseMCriDef(WORD l)              {m_BaseFightProperty.wMCriDef=l;}
	WORD    GetBaseWillDef()                {return m_BaseFightProperty.wWillDef;}		 //��־����
	void    SetBaseWillDef(WORD l)          {m_BaseFightProperty.wWillDef=l;}
	WORD    GetBaseConstiDef()              {return m_BaseFightProperty.wConstiDef;}	 //���ʿ���
	void    SetBaseConstiDef(WORD l)        {m_BaseFightProperty.wConstiDef=l;}

	//buffer ����
	WORD    GetBaseDeBuffDef(long lPos)              {return m_BaseFightProperty.wDeBuffDef[lPos];}
	void    SetBaseDeBuffDef(long lPos ,WORD l)      {m_BaseFightProperty.wDeBuffDef[lPos]=l;}
	//����Ԫ�صĿ���
	WORD    GetBaseElemDef(long lPos)                {return m_BaseFightProperty.wElemDef[lPos] ;}
	void    SetBaseElemDef(long lPos ,WORD l)        {m_BaseFightProperty.wElemDef[lPos]=l;}
	//����������˺�
	WORD    GetBaseClassDam(long lPos )              {return m_BaseFightProperty.wClassDam[lPos];}
	void    SetBaseClassDam(long lPos ,WORD l)       {m_BaseFightProperty.wClassDam[lPos]=l;}
	//����Ԫ�ص��˺�
	WORD    GetBaseElemDam(long lPos )                {return m_BaseFightProperty.wElemDam[lPos];}
	void    SetBaseElemDam(long lPos ,WORD l)         {m_BaseFightProperty.wElemDam[lPos]=l;}
	//////ʵ��ս������  �ӿ� m_FactFightProperty
	DWORD	GetMaxHP()					{return m_FactFightProperty.dwMaxHp;}		//����
	void	SetMaxHP(DWORD l)			{m_FactFightProperty.dwMaxHp=l; 
	if(GetHP()>GetMaxHP()) SetHP(GetMaxHP());}
	WORD	GetMaxMP()					{return m_FactFightProperty.wMaxMp;}		//����
	void	SetMaxMP(WORD l)			{m_FactFightProperty.wMaxMp=l; 
	if(GetMP()>GetMaxMP()) SetMP(GetMaxMP());}
	WORD    GetMaxStamina()             {return m_FactFightProperty.wMaxStamina;}	//����
	void    SetMaxStamina(WORD l)       {m_FactFightProperty.wMaxStamina=l;
	if( GetStamina()>GetMaxStamina()) SetStamina(GetMaxStamina());}
	WORD	GetMaxEnergy()				{return m_FactFightProperty.wMaxEnergy;}	//����
	void	SetMaxEnergy(WORD l)		{m_FactFightProperty.wMaxEnergy = l; 
	if(GetEnergy()>GetMaxEnergy()) m_Property.wEnergy = GetMaxEnergy();}
	WORD	GetHpRecoverSpeed()			{return m_FactFightProperty.wHpRecoverSpeed;}//��hp
	void	SetHpRecoverSpeed(WORD l)	{m_FactFightProperty.wHpRecoverSpeed=l;}
	WORD    GetFightHpRecoverSpeed()          {return m_FactFightProperty.wFightHpRecoverSpeed ;} //ս����hp
	void    SetFightHpRecoverSpeed(WORD l)    {m_FactFightProperty.wFightHpRecoverSpeed=l;}
	WORD	GetMpRecoverSpeed()			{return m_FactFightProperty.wMpRecoverSpeed;}//��mp
	void	SetMpRecoverSpeed(WORD l)	{m_FactFightProperty.wMpRecoverSpeed=l;}
	WORD    GetFightMpRecoverSpeed()          {return m_FactFightProperty.wFightMpRecoverSpeed;} //ս����mp
	void    SetFightMpRecoverSpeed(WORD l)    {m_FactFightProperty.wFightMpRecoverSpeed=l;}
	short   GetEnerRecoverSpeed()             {return m_FactFightProperty.wEnerRecoverSpeed ;}//�����ָ��ٶ�
	short   GetFightEnerRecoverSpeed()        {return m_FactFightProperty.wFightEnerRecoverSpeed;}	//ս�������ָ��ٶ� 
	void    SetFightEnerRecoverSpeed(short l) {m_FactFightProperty.wFightEnerRecoverSpeed=l;}
	WORD    GetStamRecoverSpeed()       {return m_FactFightProperty.wStamRecoverSpeed;}//������ &
	void    SetStamRecoverSpeed(WORD l) {} //.....

    short   GetStr()                    { return m_FactFightProperty.wStr; }
    void    SetStr(short s)             { m_FactFightProperty.wStr = s; }
    short   GetDex()                    { return m_FactFightProperty.wDex; }
    void    SetDex(short s)             { m_FactFightProperty.wDex = s; }
    short   GetCon()                    { return m_FactFightProperty.wCon; }
    void    SetCon(short s)             { m_FactFightProperty.wCon = s; }
    short   GetIntellect()              { return m_FactFightProperty.wIntellect; }
    void    SetIntellect(short s)       { m_FactFightProperty.wIntellect = s; }    
    short   GetSpiritualism()           { return m_FactFightProperty.wSpiritualism; }
    void    SetSpiritualism(short s)    { m_FactFightProperty.wSpiritualism = s; }
    short   GetPoint()                  { return m_FactFightProperty.wPoint; }
    void    SetPoint(short s)           { m_FactFightProperty.wPoint = s; }
	WORD    GetBlock()                  {return m_FactFightProperty.wFightBlock ;}		//��
	void    SetBlock(WORD l)            {m_FactFightProperty.wFightBlock =l;}
	WORD    GetParry()                  {return m_FactFightProperty.wParry;}		//�м�
	void    SetParry(WORD l)            {m_FactFightProperty.wParry=l;}
	WORD    GetDodge()                 {return m_FactFightProperty.wDodge;}			//����
	void    SetDodge(WORD l)           {m_FactFightProperty.wDodge=l;}
	//	WORD    GetShot()                   {return m_BaseFightProperty.wShot;}			//���
	//	void    SetShot(WORD l)             {m_BaseFightProperty.wShot=l;}
	WORD    GetCharm()                  {return m_FactFightProperty.wCharm;}		//����
	void    SetCharm(WORD l)            {m_FactFightProperty.wCharm=l;}
	WORD    GetHit()                    {return m_FactFightProperty.wHit;}			//����
	void    SetHit(WORD l)              {m_FactFightProperty.wHit=l;}
	WORD    GetAbsorb()                 {return m_FactFightProperty.wAbsorb;}		//רע
	void    SetAbsorb(WORD l)           {m_FactFightProperty.wAbsorb=l;}
	WORD    GetDef()                    {return m_FactFightProperty.wDef;}			//�������
	void    SetDef(WORD l)              {m_FactFightProperty.wDef=l;}
	WORD    GetMdef()                   {return m_FactFightProperty.wMdef;}			//ħ������
	void    SetMdef(WORD l)             {m_FactFightProperty.wMdef=l;}
	WORD    GetCri()                    {return m_FactFightProperty.wCri;}			//������
	void    SetCri(WORD l)              {m_FactFightProperty.wCri=l;}
	WORD    GetMAtkSpeed()              {return m_FactFightProperty.wMAtkSpeed;}	//ʩ���ٶ�
	void    SetMAtkSpeed(WORD l)        {m_FactFightProperty.wMAtkSpeed=l;}
	WORD    GetBuffPower()              {return m_FactFightProperty.wBuffPower;}	//buffer����ǿ��
	void    SetBuffPower(WORD l)        {m_FactFightProperty.wBuffPower=l;}
	WORD    GetMinAtk()                 {return m_FactFightProperty.wMinAtk;}		//��С������
	void    SetMinAtk(WORD l)           {m_FactFightProperty.wMinAtk=l;}
	WORD    GetMaxAtk()                 {return m_FactFightProperty.wMaxAtk;}		//��󹥻���
	void    SetMaxAtk(WORD l)           {m_FactFightProperty.wMaxAtk=l;}
	WORD    GetMAtk()                   {return (WORD)m_FactFightProperty.wMAtk;}			//ħ��������
	void    SetMAtk(WORD l)             {m_FactFightProperty.wMAtk=l;}
	WORD    GetMCri()                   {return (WORD)m_FactFightProperty.wMCri;}			//ħ��������
	void    SetMCri(WORD l)             {m_FactFightProperty.wMCri=l;}
	WORD    GetBloodSuk()               {return m_FactFightProperty.wBloodSuck;}		//����������
	WORD    GetCriDef()                 {return m_FactFightProperty.wCriDef;}//����������
	void    SetCriDef(WORD l)           {m_FactFightProperty.wCriDef=l;}
	WORD    GetMCriDef()                {return m_FactFightProperty.wMCriDef;}//ħ����������
	void    SetMCriDef(WORD l)          {m_FactFightProperty.wMCriDef=l;}
	WORD    GetManaSuck()               {return m_FactFightProperty.wManaSuck;}		//����������
	void    SetManaSuck(WORD l)         {m_FactFightProperty.wManaSuck=l;}
	WORD    GetWillDef()                {return m_FactFightProperty.wWillDef;}		//��־����
	void    SetWillDef(WORD l)          {m_FactFightProperty.wWillDef=l;}
	WORD    GetConstiDef()              {return m_FactFightProperty.wConstiDef;}		//���ʿ���
	void    SetConstiDef(WORD l)        {m_FactFightProperty.wConstiDef=l;}

	//buffer ����
	WORD    GetDeBuffDef(long lPos)              {return m_FactFightProperty.wDeBuffDef[lPos];}
	void    SetDeBuffDef(long lPos ,WORD l)      {m_FactFightProperty.wDeBuffDef[lPos]=l;}
	//����Ԫ�صĿ���
	WORD    GetElemDef(long lPos)                {return m_FactFightProperty.wElemDef[lPos] ;}
	void    SetElemDef(long lPos ,WORD l)        {m_FactFightProperty.wElemDef[lPos]=l;}
	//����������˺�
	WORD    GetClassDam(long lPos )              {return m_FactFightProperty.wClassDam[lPos];}
	void    SetClassDam(long lPos ,WORD l)       {m_FactFightProperty.wClassDam[lPos]=l;}
	//����Ԫ�ص��˺�
	WORD    GetElemDam(long lPos )                {return m_FactFightProperty.wElemDam[lPos];}
	void    SetElemDam(long lPos ,WORD l)         {m_FactFightProperty.wElemDam[lPos]=l;}
	/////////�������Խӿ�  m_AppendProperty
	bool	IsCharged()					{return m_AppendProperty.bIsCharged;}	//�Ƿ��ֵ
	void	SetCharged(bool b=true)		{m_AppendProperty.bIsCharged=b;}

    const   char *GetAccount()			{return m_AppendProperty.strAccount;}    //
	void	SetAccount(const char* l)	{strncpy(m_AppendProperty.strAccount ,l,ACCOUNT_SIZE);TrimRight(m_AppendProperty.strAccount);}		
	const   char *GetTitle()			{return m_AppendProperty.strTitle;}
	void	SetTitle(const char* l)		{strncpy(m_AppendProperty.strTitle ,l,TITLE_SIZE);}

	//δ��
	DWORD	GetMoney();              
	void	SetMoney(DWORD l)			{return ;}

	DWORD   GetSilver();
	
	void	ResetActive(void);
	long	GetCurrBaseActive(void)				{return m_Property.lCurrBaseActive;}
	long	GetCurrExActive(void)				{return m_Property.lCurrExActive;}
	ULONG	GetActiveTime(void)					{return m_Property.lLastResetActiveTime;}
	void	SetCurrBaseActive(long lBaseActive)	{m_Property.lCurrBaseActive = lBaseActive;}
	void	SetCurrExActive(long lExActive)		{m_Property.lCurrExActive = lExActive;}
	void	SetActiveTime(ULONG uTime)			{m_Property.lLastResetActiveTime = uTime;}

	//!				�õ���ҽ�鲽��
	long			GetMarriageStep(void)					{return m_Property.lMarriageStep;}
	//!				������ҽ�鲽��
	void			SetMarriageStep(long lStep)				{m_Property.lMarriageStep = lStep;}

	DWORD   GetGem(void);
	void	SetGem(DWORD l)				{return ;}
	//ʣ��Ľӿ�

	map<CGUID,CMail*>	m_mapMail;
	map<long,long>		m_mapSysMail;									//ϵͳ�ʼ������б�
	list<CGUID>			m_lNewMail;										//���ʼ��б�
	list<CGUID>			m_lReceiveMail;									//�����ʼ��б�

public:


	
	CMail				*GetMail(CGUID &guid);				
	void				DeleteMail(list<CGUID> &lguid);
	void				EraseMail(const CGUID &guid);
	void				ClearMailMap();
	
	long	CheckGoodsInPacket(char* strGoodName);						//����ڱ������Ƿ����ĳ�����ֵ���Ʒ
	void				AddSysMail(long lID,long lEndTime);				//
	void				AddMailList(CGUID &MailGuid);					//������ʼ��б�

	list<CGUID>			&GetNewMailList();
	void				AddReceiveMailList(CGUID MailGuid);			//��������ʼ��б�
	list<CGUID>			&GetReceiveMailList();							
	map<long,long>      &GetSysMail();									//��ȡ�ʼ��б�
	

	// �й�����
	int	ChangeCountry(BYTE c);


public:
	bool  LoadData();		// ��ȡ
	// �洢
	//bool SaveData(_ConnectionPtr cn = NULL);
	//bool SaveCdkeyData(_ConnectionPtr cn = NULL);

	virtual bool AddToByteArray(vector<BYTE>* pByteArray, bool bExData = true);		// ���� ��ӵ�CByteArray
	virtual bool AddOrgSysToByteArray(vector<BYTE>* pByteArray);
	virtual bool DecordFromByteArray(BYTE* pByte, long& pos, bool bExData = true);	// ����

	virtual bool CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData = true);		// ��ӵ�CByteArray
	virtual bool DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData = true);		// ����
	bool CodeOrgSysToDataBlock(DBWriteSet& setWriteDB);

	void LoadDefaultProperty(BYTE lOccupation,BYTE lSex, BYTE byCountry,BYTE byConstellation);	//LOAD��ҵĳ�ʼĬ������
	void InitSkill();
	void AddSkill(DWORD dwID,WORD Lv,WORD wCool);
	void UpdateProperty();		// ��������
	//----------------------------------------------------------------
	// ����ʱ��
	//----------------------------------------------------------------
private:
	long	m_lSilienceTime;

public:
	long	GetSilenceTime()			{ return m_lSilienceTime;  }
	void	SetSilenceTime( long lTime)	{ m_lSilienceTime = lTime; }

	//----------------------------------------------------------------
	// ɱ����ʱ��
	//----------------------------------------------------------------
private:
	DWORD	m_dwMurdererTime;

public:
	DWORD	GetMurdererTime()				{ return m_dwMurdererTime;  }
	VOID	SetMurdererTime( DWORD dwTime )	{ m_dwMurdererTime = dwTime; }

	//----------------------------------------------------------------
	// ս��״̬ʱ��
	//----------------------------------------------------------------
private:
	LONG	m_lFightStateCount;

public:
	LONG	GetFightStateCount()			{ return m_lFightStateCount; }
	VOID	SetFightStateCount( LONG l )	{ m_lFightStateCount = 2; }
public:
	struct tagOwnedReg
	{
		long lRegionID;						//������ID
		WORD wType;							//����������
	};
	//���ϵͳ
private:
	CGUID	m_FactionID;							//�Լ����ID
	CGUID	m_UnionID;								//�Լ����ID
	char	m_szFactionName[MAXSIZE_FactionName];	//! �������
	char	m_szUnionName[MAXSIZE_FactionName];		//! ��������
	LONG	m_lFacDemiseWarning;					//! ��ᴫλ���ѵ���ʱ
	LONG	m_lFacDisbandWarning;					//! ����ɢ���ѵ���ʱ

public:
	const CGUID&	GetFactionID		(void)						{return m_FactionID;}
	void			SetFactionID		(const CGUID& guid);
	void			SetFactionInfo		(CFaction *pFaction);
	const CGUID&	GetUnionID			(void)						{return m_UnionID;}
	void			SetUnionID			(const CGUID& UnionID);
	void			SetUnionInfo		(CUnion *pUnion);
	const char*		GetFactionName		(void)						{return m_szFactionName; }
	const char*		GetUnionName		(void)						{return m_szUnionName; }
	void			SetFacDemiseWarning	(LONG lTime)				{m_lFacDemiseWarning = lTime;}
	void			SetFacDisbandWarning(LONG lTime)				{m_lFacDisbandWarning = lTime;}

	long	GetNetExID()
	{
		static long lID = 0;
		return ++lID;
	}

	//����ϵͳ
	/*
	public:
	struct tagPlayerQuest
	{
	WORD	wQuestID;				//�����ID
	BYTE	byComplete;				//�Ƿ����
	};
	private:
	map<WORD,tagPlayerQuest>	m_PlayerQuests;	//����ѻ�õ������б�
	bool	m_bLogin;					//��ʾ����Ƿ��ѵ�½
	*/
private:
	CQuestQueue m_RoleQueue;
	//ٸ������
	CMercenaryQuestSys	m_MerQuestSys;
	bool		m_bLogin;
public:
	BOOL AddQuestDataToByteArray(vector<BYTE>* pByteArray);		//����������ݵ�ByteArray
	BOOL DecordQuestDataFromByteArray(BYTE* pByte, long& pos);

	BOOL CodeQuestDataToDataBlock(DBWriteSet& setWriteDB);
	BOOL DecodeQuestDataFromDataBlock(DBReadSet& setReadDB);

	//�õ�һ�������״̬(0:���ڵ�û���,1:���������,2:û��  [δʹ�� ҲΪʵ�ֺ���])
	long GetQuestState(WORD wQuestID);							
	void AddFinishedQuestFromDB(WORD wQuestID);				//�����ݿ����һ�������¼
	BOOL AddQuestFromDB(CQuestRecord* rolerecord);
	//map<WORD,tagPlayerQuest>& GetPlayerQuests()	{return m_PlayerQuests;}
	const list<CQuestRecord*>* GetPlayerQuests()	{return m_RoleQueue.GetQuestRecordList();}

	bool AddMerQuestDataByteArray(vector<BYTE>* pByteArray);	//���ٸ���������ݵ�ByteArray
	bool DecordMerQuestDataFromByteArray(BYTE* pByte, long& pos);//��ٸ����������

	bool CodeMerQuestDataToDataBlock(DBWriteSet& setWriteDB);
	bool DecodeMerQuestDataFromDataBlock(DBReadSet& setReadDB);

	bool	GetPlayerLogin()		{return m_bLogin;}
	void	SetPlayerLogin(bool b)	{m_bLogin = b;}

	bool	GetFirstLogin()		{return m_bFirstLogin;}
	void	SetFirstLogin(bool b)	{m_bFirstLogin = b;}
	//����ϵͳend

	//��ְҵϵͳ

	//ͷ�� ���� ����
	BYTE	GetShowFashion()		{return m_AppendProperty.bShowFashion;}
	void	SetShowFashion(BYTE b)	{m_AppendProperty.bShowFashion = b;}

private:
	CDeputyOccuRecord m_DOccuRecord;
//��ְҵϵͳend
public:
	BOOL AddDOccuDataToByteArray(vector<BYTE>* pByteArray){return m_DOccuRecord.AddDeputyOccuDataByteArray(pByteArray);}
	BOOL DecordDOccuDataFromByteArray(BYTE* pByte, long& pos){return m_DOccuRecord.DecordDeputyOccuDataFromByteArray(pByte,pos);}
	BOOL DecodeDOccuFromDataBlock(DBReadSet& setReadDB){return m_DOccuRecord.DecodeFromDataBlock(setReadDB);}

protected:
	friend void OnLogMessage(CMessage* pMsg);

	//////////////////////////////////////////////////////////////////////////
	// ���
	//////////////////////////////////////////////////////////////////////////
private:
	CGUID m_TeamGuid;

public:
	const CGUID& GetTeamID() { return m_TeamGuid; }
	void SetTeamID(const CGUID& guid) { m_TeamGuid = guid;	}

    ///Expter ��λ�Ĺ���ģ��
private:
    CGUID           m_PhaseId;
    ePhaseRest      m_PhaseRest;

public:
    const CGUID& GetPhaseID()          { return m_PhaseId; }
    void  SetPhaeID(const CGUID& guid) { m_PhaseId = guid; }
    ePhaseRest&  GetPhaseRest()        { return m_PhaseRest;}
    void  SetPhaseRest(ePhaseRest e )  { m_PhaseRest = e ; }

	// ��Ҹ���
public:
	int ChangeName(const char *szNewName);

	CGoods* GetGoodsById(const CGUID& guid);			//����ID�õ���Ʒ

public:
	const CGUID& GetPersonalRgnGUID(void);
	const CGUID& GetPersonalHouseRgnGUID(void);
	void SetPersonalRgnGUID(const CGUID& guid);
	void SetPersonalHouseRgnGUID(const CGUID& guid);

	////!					���һ�����鸱��GUID
	//void				AddTeamRgnGUID(LONG lTemplateRgnID, const CGUID& guid, BOOL bCover);
	////!					ɾ��һ�����鸱��GUID
	//void				DelTeamRgnGUID(LONG lTemplateRgnID);
	//void				DelTeamRgnGUID(const CGUID& guid);
	////!					�õ�һ�����鸱����GUID
	//const	CGUID&		GetTeamRgnGUID(LONG lTemplateRgnID);
	////!					�õ����鸱��GUID�б�
	//map<LONG, CGUID>&	GetTeamRgnGuidMap(VOID){return m_mapTeamRgnGUID;}
	////!					���鸱��GUID����
	//VOID				RgnGuidCode(vector<BYTE> &vData);
	////!					���鸱��GUID����
	//VOID				RgnGuidDecode(BYTE *pData, LONG &lPos);
	////!					���鸱��GUID����
	//VOID				RgnGuidCode(DBWriteSet& setWriteDB);
	////!					���鸱��GUID����
	//VOID				RgnGuidDecode(DBReadSet& setReadDB);

	long GetCurPRgnNum(void) { return m_CurPRgnNum; }
	long GetCurTRgnNum(void) { return m_CurTRgnNum; }

	void SetCurPRgnNum(long num) { m_CurPRgnNum = num; }
	void SetCurTRgnNum(long num) { m_CurTRgnNum = num; }

	// ȡ����ҵ�¼ʱ��
	DWORD			GetLoginTime(void) { return m_dwLoginTime; }
	// ������ҵ�¼ʱ��
	void			SetLoginTime(DWORD time) { m_dwLoginTime = time; }
private:
	CGUID m_PersonalRgnGUID;
	CGUID m_PersonalHouseRgnGUID;
	//map<LONG, CGUID> m_mapTeamRgnGUID;

	long m_CurPRgnNum;
	long m_CurTRgnNum;
	bool m_bFirstLogin;

	// ��ҵ�¼ʱ��
	DWORD			m_dwLoginTime;

	//-------------------------------------------------------------------------------
	// ��������10����Ʒ�б�. by Fox.		2008-02-14
	// func : ��GS��¼��WSֻ��������
	//-------------------------------------------------------------------------------
public:
	typedef map<DWORD,list<DWORD>> IncShopCur10Map;
	typedef IncShopCur10Map::iterator IncShopCur10Iter;
public:
	IncShopCur10Map &GetIncShopCur10()			{ return m_mapIncShopCur10; }
	void ClearIncShopCur10();

	CIncrementShopList::VEC_BUYLIST GetIncTradeList() { return m_vecTradeList; }
	void ClearIncTradeList() { m_vecTradeList.clear(); }
	void AddIncTradeRecord( CIncrementShopList::IncShopTradeLog stIncTradeElem );

	void AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum);
	map<DWORD,DWORD>& GetLimitGoodsMap(){return m_mapLimitGoodsRecord;}
	void ClearLimitGoodsRecord(){m_mapLimitGoodsRecord.clear();}
private:
	//��ֵ�̵���������Top10��¼
	IncShopCur10Map m_mapIncShopCur10;
	//��ֵ�̵깺����ϸ��¼�б�
	CIncrementShopList::VEC_BUYLIST m_vecTradeList;
	//��ֵ�̵������޹���Ʒ�����¼
	map<DWORD,DWORD> m_mapLimitGoodsRecord;


	////////////////////////////////////
public:
	//!	�õ��������GS��socket
	LONG	GetGsSocket(void)				{return m_GsSocket;}
	//!		�����������GS��socket
	void	SetGsSocket(LONG lGsSocket)		{m_GsSocket = lGsSocket;}
	LONG	GetIP(VOID)						{return m_lIP;}
	VOID	SetIP(LONG lIP)					{m_lIP = lIP;}
private:
	LONG m_GsSocket;
	LONG m_lIP;
	//////////////////////////////////////////////////////////////////////////
	// С���鸨��
	//////////////////////////////////////////////////////////////////////////
public:
	SpriteSystem		m_SpriteSystem;

	//////////////////////////////////////////////////////////////////////////
	// �����Ҫ�����ڷ�����������
	//////////////////////////////////////////////////////////////////////////
public:
	PlayerSetupOnServer	m_SetupOnServer;

	//////////////////////////////////////////////////////////////////////////
	// ѫ����������ǰ�ƺ�
	//////////////////////////////////////////////////////////////////////////
public:
	MedalContainer			m_MedalContainer;

	//!						���óƺŵ���ʾ����
	void					SetTitleType(LONG lType)	{m_lCurrTitleType = lType;};
	//!						�õ��ƺŵ���ʾ����
	LONG					GetTitleType(void)			{return m_lCurrTitleType;};

private:
	//!						��ǰ�ƺ�����
	LONG					m_lCurrTitleType;

	// Fox@20081223 ����
public:
	/// ȡ�ó����б�
	C_PETS GetPets(void) { return m_Pets; }
	/// ��������
	CPet* CreatePet(const CGUID& guid);
	/// ��ӳ���
	void AddPet(CPet* pet);
	/// ɾ��ָ��GUID����
	void DelPet(const CGUID& guid);
	/// ɾ�����г���
	void ReleaseAllPets(void);

private:
	/// ɾ������
	void DelPet(ITR_PET& itr);

private:
	PETS m_Pets;			///< �����б�

public:
	Business::CPackage &GetBusinessPackage()
	{
		return m_BusinessContainer;
	}
	/// �жϸ�����Ƿ�������
	bool IsBusinessMan() const { return m_Property.byIsBusinessMan == 1; }
	/// �ı��������
	void SetBusinessMan( bool bBusinessMan ) { m_Property.byIsBusinessMan = ( bBusinessMan ? 1 : 0 ); }
private:
	Business::CPackage m_BusinessContainer;

	// ������������ BAIYUN@20090602
private:
	AntiCheatData    m_ACProperty;
public:
	AntiCheatData*   GetACProperty()   {return &m_ACProperty;}
	void             SetACProperty(AntiCheatData* ACData)
	                                   {memcpy(&m_ACProperty, ACData, sizeof(AntiCheatData));}
	void             SetDBACProperty(dbAntiCheatData *DBACData)
	                                   {memcpy(&m_ACProperty.m_dbData, DBACData, sizeof(dbAntiCheatData));}
private:
	VariableList *m_VarList;
public:
	VariableList *GetVariableList() { return m_VarList; }
};
