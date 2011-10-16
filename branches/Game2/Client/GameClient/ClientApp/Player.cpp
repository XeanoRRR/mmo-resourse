#include "stdafx.h"
#include "Player.h"
#include "../../Input/KeyBoard.h"
#include "../../Net/Message.h"
#include "../../EngineExtend/TextureProjective.h"
#include "../../EngineExtend/DisplayModel.h"
#include "../../GameClient/ClientApp/Skills/SkillXml.h"
#include "../../GameClient/ClientApp/ClientRegion.h"
#include "../../GameClient/ClientApp/Other/CountryList.h"
#include "../../GameClient/ClientApp/Other/PicList.h"
#include "../../GameClient/ClientApp/MsgBoxSys/MsgEventManager.h"
#include "../../GameClient/GameControl.h"
#include "../../Public/Common/Public.h"
#include "../../Public/Common/SkillRelated.h"
#include "../../Public/Common/StudySkillCondition.h"
#include "../../Public/Common/DeputyOccu.h"
#include "Other/GlobeSetup.h"
#include "Other/AudioList.h"
#include "QuestSystem/QuestSystem.h"
#include "organizingsystem/Union.h"
#include "MainPlayerHand.h"
#include "Monster.h"
#include "PetSystem/Pet.h"
#include "Depot.h"
#include "../Game.h"
#include "../../UI/GamePage/Package/PackagePage.h"
#include "../../UI/Windows/ShopCityDef.h"
#include "Other/AutoSupply.h"

extern void UpdateEquipmentIcon(CPlayer::eEquipIndex index);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DECODE_GENIUS_
typedef list<CPlayer::tagGoods>::iterator tagGoodsIter;

class Skill_Compare
{
public:
	bool operator ()(CPlayer::tagSkill& left, CPlayer::tagSkill& right) const
	{
		CPlayer *pPlayer = GetGame()->GetMainPlayer();
		if(pPlayer == NULL)
			return false;
		DWORD leftID = left.dwID;
		DWORD rightID = right.dwID;
		if(CSkillListXml::OccuJudgeStudy(leftID,pPlayer->GetOccupation()) && 
			!CSkillListXml::OccuJudgeStudy(rightID,pPlayer->GetOccupation()))
		{
			return true;
		}
		return false;
	}
};

string CPlayer::s_strOccupation[OCC_Max] = {""};			// ְҵ
string CPlayer::s_strConstellation[CONST_Pisces+1] = {""};	// ����

BYTE CPlayer::BuyBackGoodsNum = 10;

// ��ʼ��ְҵ����
void CPlayer::InitPlayerOccuType()
{
	for (int i = 0;i<OCC_Max;i++)
	{
        char strIndex[128];
        sprintf_s<128>(strIndex, "Player_%d", i+1);
        s_strOccupation[i] = AppFrame::GetText(strIndex);
	}
}
// ���ְҵ����
const char* CPlayer::GetPlayerOccuType(long lOccuTypeIndex)
{
	if (lOccuTypeIndex>=0&&lOccuTypeIndex<OCC_Max)
	{
		return s_strOccupation[lOccuTypeIndex].c_str();
	}
	return "";
}

// ��ʼ������
void CPlayer::InitPlayerConstType()
{
	for (int i = 0;i <= CONST_Pisces; i++)
    {
        char strIndex[128];
        sprintf_s<128>(strIndex,"Player_%d",i+200);
        s_strConstellation[i] = AppFrame::GetText(strIndex);
    }
}

// ���ָ������������
const char* CPlayer::GetPlayerConstType(long lConstTypeIndex)
{
	if (lConstTypeIndex > CONST_No && lConstTypeIndex <= CONST_Pisces)
	{
		return s_strConstellation[lConstTypeIndex].c_str();
	}
	return "";
}

long	CPlayer::m_iEnemyPlayerLevelFirstQuotiety = 0;		//�������й���ҵȼ�ϵ��1
long	CPlayer::m_iEnemyPlayerLevelSecondQuotiety = 0;	//�������й���ҵȼ�ϵ��1
WORD	CPlayer::m_wPublicCoolTime = 0;			// ������ȴʱ�䣨ʹ�ü��ܻ����ĵ���֮��Ļ�����ȴʱ�䣩

vector<CEmotionShower::tagEmotion> vecEmotions;

void LoadEmotionsConfig(char *filename)
{
	CRFile* prfile = rfOpen(filename);
	if( prfile )
	{	
		stringstream stream;
		prfile->ReadToStream(stream);
		rfClose(prfile);
		string s,action;
		int count=0,type,num;
		stream>>s>>count;
		vecEmotions.clear();
		for(int i=0;i<count;i++)
		{
			CEmotionShower::tagEmotion stEmotion;
			stream>>s;//����"*"��
			stream>>stEmotion.dwIndex;
			stream>>s;//�����Ƿ�ѭ����־
			stream>>stEmotion.strName>>num;
			for(int j=0;j<num;j++)
			{
				stream>>action>>type;
				stEmotion.vecActions.push_back(MAKEFOURCC(action[0],action[1],action[2],action[3]));
				stEmotion.vecTypes.push_back(type);
			}
			stream>>stEmotion.dwType;
			stream>>stEmotion.strText>>stEmotion.strSound>>stEmotion.nSoundLoops;
			vecEmotions.push_back(stEmotion);
		}
	}
}

CPlayer::CPlayer(void):m_dwMoney(0),m_dwYuanBao(0),m_dwJiFen(0)
{
	m_bSelf = false;

	m_lPlayerShopPlugID = 0;
	m_dwEnergyHitEffectID = 0;
	//m_setPetID.clear();
	m_vecFactureEntrys.clear();

	m_pEmotionShower=new CEmotionShower(this);
	SetType(TYPE_PLAYER);

	SetUpgradeExp(0);
	m_lGrowTimer = 0;
	int i=0;
	for(;i<24;i++)
	{
		SetHotKey(i,0);
	}

	memset(&m_Property, 0, sizeof(tagPPro));
	memset(&m_FightProperty, 0, sizeof(tagPFPro));
	memset(&m_AppendProperty, 0, sizeof(tagPAPro));
	memset(&m_FriendshipCounty, 0, sizeof(tagFriendshipCounty));
	SetShowFashion(FASHION_TYPE_GREATCOAT | FASHION_TYPE_MANTEAU/* | FASHION_TYPE_HELM*/);

	//SetReAnk(0);
	for(i=0;i<EQUIP_SUM;i++)
	{
		m_Equip[i] = NULL;
		m_dwEquipIndex[i] = 0;
		m_btEquipLevel[i] = 0;
	}
	for (int i = 0;i<COTE_NUM;i++)
	{
		m_Cote[i] = new tagCote;
	}

    for(i=0; i<EX_EQUIP_NUM; i++)
    {
        m_eEquipGod[i] = NULL;
        m_eEquipLand[i] = NULL;
        m_eEquipHuman[i] = NULL;
    }
	//m_stHand.bChange=false;
	//m_stHand.lNum=0;
	//m_stHand.lPos=0;
	//m_stHand.pGoods=NULL;

	m_CoteBussinesPack = new tagCote;

	m_lFactionJob = 0;

    m_strShopName = AppFrame::GetText("Shop_26");

	m_strTraderName = "";
	//m_bTradeModeSign = false;
	m_mapPlayerCreditForZone.clear();
	m_mapPlayerInvestmentForPoint.clear();
	m_dwCollectExp = 0;
	m_dwCollectLevel = 0;
	m_dwFactureExp = 0;
	m_dwFactureLevel = 0;
	//m_wTradeTaskID = 0;
	//m_fTradeTaskCompleteDegree = 0.0f;

	m_lTeammateAmount = 1;
	m_bSinState = false;
	m_lHitNum = 0;
	m_dwLastHitTimer = 0;
	m_lGMLevel = 0;

	m_bTradeDisAble=true;
	m_bFriendDisAble=true;
	m_bCanSetHotKey = true;

	m_lAllotMade = 0;
	m_lLimtSkillID = 0;
	m_lLimtSkillLev = 0;
	m_bIsRecruitingTeam=false;
	m_strFactionName="";
	m_strFactionTitle="";
	m_bPlayerShopOpened = false;
	m_bContendState = false;
	m_lCityWarDiedState = false;
	m_dwToolIndex = 0;

	m_cRemainDays = -1;
	m_EnemyFactions.clear();
	m_CityWarEnemyFactions.clear();

	m_queBuyBackGoodsItem.clear();

	m_VecMedalOnPlayer.clear();

	InitNameValueMap();
	m_LEnergy.SetLocator(LOCATOR_LEFTHAND);
	m_REnergy.SetLocator(LOCATOR_RIGHTHAND);
	m_LeftHandWeapon.SetLocator(LOCATOR_LEFTHAND);
	m_RightHandWeapon.SetLocator(LOCATOR_RIGHTHAND);
	m_CollectTool.SetLocator(LOCATOR_RIGHTHAND);
	m_CollectTool.SetVisible(false);
	m_Wing.SetLocator(LOCATOR_BACK);
	m_HeadWear.SetLocator(LOCATOR_HEAD);
	m_bPlayEnergyEffect = FALSE;

	m_guMoneyID = CGUID::GUID_INVALID;
	m_guSilverMoneyID = CGUID::GUID_INVALID;
	m_guBaoShiID = CGUID::GUID_INVALID;
	m_guYuanBaoID = CGUID::GUID_INVALID;
	m_guJiFenID = CGUID::GUID_INVALID;

	m_dwLevelCurrentDisplayNum = 0;
	m_dwLevelMax = (CGlobeSetup::GetSetup()->dwExpTipTextLineMax == 0 ? 3 : CGlobeSetup::GetSetup()->dwExpTipTextLineMax);
	m_vecTipText.resize(m_dwLevelMax);

	for (size_t st = 0; st < m_vecTipText.size(); ++st)
		m_vecTipText[st].fAlpha = 0.0f;

	m_lOldContribute = 0;
	m_lCreditNum=0;
	m_bIsShowLoverName = false;
	//m_bIsHideInfo = false;
	//m_strHideInfoName = "������";

	m_PetList.clear();
	m_ShowPets.clear();
	m_ActivePetGUID = CGUID::GUID_INVALID;
}

CPlayer::~CPlayer(void)
{
	m_mapPlayerCreditForZone.clear();
	m_mapPlayerInvestmentForPoint.clear();
	m_VecMedalOnPlayer.clear();
	//m_setPetID.clear();
	deque<CGoods*>::iterator GoodsIter = m_queBuyBackGoodsItem.begin();
	for (;GoodsIter!=m_queBuyBackGoodsItem.end();GoodsIter++)
	{
		SAFE_DELETE(*GoodsIter);
	}
	m_queBuyBackGoodsItem.clear();
	for(int i=0;i<EQUIP_SUM;i++)
	{
		if(i!=EQUIP_MEDAL1&&i!=EQUIP_MEDAL2&&i!=EQUIP_MEDAL3)  //ѫ�µ����ͷ�
		{
			SAFE_DELETE(m_Equip[i]);
		}
		else
			m_Equip[i] = NULL;
	}

	for(tagGoodsIter it=m_listGoods.begin(); it!=m_listGoods.end(); it++)
	{
		SAFE_DELETE(it->pGoods);
	}
	m_listGoods.clear();
	for (int i = 0;i<COTE_NUM;i++)
	{
		if (m_Cote[i]!=NULL)
		{
			if(m_Cote[i]->pGoods!=NULL)
			{
				if (!m_Cote[i]->m_listCoteGoods.empty())
				{
					tagGoodsIter it = m_Cote[i]->m_listCoteGoods.begin();
					for (;it!=m_Cote[i]->m_listCoteGoods.end();it++)
					{
						SAFE_DELETE(it->pGoods);
					}
					m_Cote[i]->m_listCoteGoods.clear();
				}
				SAFE_DELETE(m_Cote[i]->pGoods);
			}
			SAFE_DELETE(m_Cote[i]);
		}
	}

	if (m_CoteBussinesPack!=NULL)
	{
		if(m_CoteBussinesPack->pGoods!=NULL)
		{
			if (!m_CoteBussinesPack->m_listCoteGoods.empty())
			{
				tagGoodsIter it = m_CoteBussinesPack->m_listCoteGoods.begin();
				for (;it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
				{
					SAFE_DELETE(it->pGoods);
				}
				m_CoteBussinesPack->m_listCoteGoods.clear();
			}
			SAFE_DELETE(m_CoteBussinesPack->pGoods);
		}
		SAFE_DELETE(m_CoteBussinesPack);
	}



	//if(GetHand()->pGoods!=NULL)
	//{
	//	SAFE_DELETE(GetHand()->pGoods);
	//	ClearHand();
	//}
	SAFE_DELETE(m_pEmotionShower);
	RemoveAllPet();

	//m_vecTipText.clear();

	//vector<CPet*>::iterator petIter = m_PetList.begin();
	//for (; petIter!=m_PetList.end(); ++petIter)
	//{
	//	SAFE_DELETE(*petIter);
	//}
	//m_PetList.clear();
	//m_ShowPets.clear();
}



/*
* ����: ��ʼ����������ֵ��Ӱ��
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.09.16 - lpf
*		������"ShowCountry"�Ľ���
*	2009.03.13 - lpf
*		������"bShowFashion"�Ľ���,�滻��ԭ�е�ͷ������
*/
void CPlayer::InitNameValueMap()
{
	CMoveShape::InitNameValueMap();
	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool

	st.lType=6; st.pbValue=&m_bContendState;			m_PropertyEnumToName[P_ATTR_CONTENDSTATE]="ContendState";					m_mapNameValue["ContendState"]=st;			
	st.lType=6; st.pbValue=&m_lCityWarDiedState;		m_PropertyEnumToName[P_ATTR_CITYWARDIEDSTATE]="CityWarDiedState"; 			m_mapNameValue["CityWarDiedState"]=st;		


	st.lType=4; st.pValue=&m_lType;;			m_PropertyEnumToName[P_ATTR_TYPE]="Type";				m_mapNameValue["Type"]=st;			
	st.lType=4; st.pValue=&m_lGraphicsID;;		m_PropertyEnumToName[P_ATTR_GRAPHICSID]="GraphicsID";	m_mapNameValue["GraphicsID"]=st;	
	st.lType=4; st.pValue=&m_lRegionID;			m_PropertyEnumToName[P_ATTR_REGIONID]="RegionID";		m_mapNameValue["RegionID"]=st;
	st.lType=3;	st.pfValue=&m_fPosX;			m_PropertyEnumToName[P_ATTR_TILEX]="TileX";				m_mapNameValue["TileX"]=st;		
	st.lType=3;	st.pfValue=&m_fPosY;			m_PropertyEnumToName[P_ATTR_TILEY]="TileY";				m_mapNameValue["TileY"]=st;
	st.lType=3;	st.pfValue=&m_fDir;				m_PropertyEnumToName[P_ATTR_DIR]="Dir";					m_mapNameValue["Dir"]=st;					
	st.lType=1;	st.pwValue=&m_wState;			m_PropertyEnumToName[P_ATTR_STATE]="State";				m_mapNameValue["State"]=st;			
	st.lType=1;	st.pwValue=&m_wAction;			m_PropertyEnumToName[P_ATTR_ACTION]="Action";			m_mapNameValue["Action"]=st;		


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*��������*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byOccu);			m_PropertyEnumToName[P_ATTR_OCCU]="Occu";					m_mapNameValue["Occu"]=st;		
	st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byAssOccu);		m_PropertyEnumToName[P_ATTR_DOCCU]="DOccu";					m_mapNameValue["DOccu"]=st;
//	st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byConst);			m_PropertyEnumToName[P_ATTR_CONST]="Const";					m_mapNameValue["Const"]=st;
	st.lType=0;	st.plValue	= &m_Property.bySex;					m_PropertyEnumToName[P_ATTR_SEX]="Sex";						m_mapNameValue["Sex"]=st;
//	st.lType=0;	st.plValue	= &m_Property.byClass;					m_PropertyEnumToName[P_ATTR_CLASS]="Class";					m_mapNameValue["Class"]=st;
	st.lType=0;	st.plValue	= &m_Property.byHairPic;				m_PropertyEnumToName[P_ATTR_HEADPIC]="HeadPic";				m_mapNameValue["HeadPic"]=st;				
	st.lType=0;	st.plValue	= &m_Property.byFacePic;;				m_PropertyEnumToName[P_ATTR_FACEPIC]="FacePic";				m_mapNameValue["FacePic"]=st;				
	st.lType=1;	st.pdwValue	= &m_Property.dwHp;						m_PropertyEnumToName[P_ATTR_HP]="Hp";						m_mapNameValue["Hp"]=st;				
	st.lType=1;	st.pwValue	= &m_Property.wMp;;						m_PropertyEnumToName[P_ATTR_MP]="Mp";						m_mapNameValue["Mp"]=st;				
	st.lType=1;	st.pwValue  = &m_Property.wEnergy;					m_PropertyEnumToName[P_ATTR_ENERGY]="Energy";				m_mapNameValue["Energy"]=st;			
	st.lType=1; st.pwValue  = &m_Property.wStamina;					m_PropertyEnumToName[P_ATTR_STAMINA]="Stamina";	;			m_mapNameValue["Stamina"]=st;				
	st.lType=0;	st.pwValue	= &m_Property.wLevel;					m_PropertyEnumToName[P_ATTR_LEVEL]="Level";					m_mapNameValue["Level"]=st;				
	st.lType=2;	st.pdwValue	= &m_Property.dwExp;					m_PropertyEnumToName[P_ATTR_EXP]="Exp";						m_mapNameValue["Exp"]=st;
	st.lType=2;	st.pdwValue	= &m_Property.dwExpMultiple;			m_PropertyEnumToName[P_ATTR_EXPMULTIPLE]="ExpMultiple";		m_mapNameValue["ExpMultiple"]=st;
	st.lType=2;	st.pdwValue	= &m_Property.dwPresentExp;				m_PropertyEnumToName[P_ATTR_PRESENTEXP]="PresentExp";		m_mapNameValue["PresentExp"]=st;
	/*st.lType=0; st.plValue	= &m_Property.byRankOfNobility;			m_PropertyEnumToName[P_ATTR_RANKOFNOBILITY]="RankOfNobility";m_mapNameValue["RankOfNobility"]=st;	
	st.lType=2;	st.pdwValue = &m_Property.dwRankOfNobCredit;		m_PropertyEnumToName[P_ATTR_RANKOFNOBCREDIT]="RankOfNobCredit";	m_mapNameValue["RankOfNobCredit"]=st;		
	st.lType=0; st.plValue	= &m_Property.byRankOfMercenary;		m_PropertyEnumToName[P_ATTR_RANKOFMERCENARY]="RankOfMercenary";	m_mapNameValue["RankOfMercenary"]=st;		
	st.lType=2;	st.pdwValue = &m_Property.dwMercenaryCredit;		m_PropertyEnumToName[P_ATTR_MERCENARYCREDIT]="MercenaryCredit";	m_mapNameValue["MercenaryCredit"]=st;		
	st.lType=2;	st.pdwValue = &m_Property.dwBatakCredit;			m_PropertyEnumToName[P_ATTR_BATAKCREDIT]="BatakCredit";		m_mapNameValue["BatakCredit"]=st;
	st.lType=2;	st.pdwValue = &m_Property.dwZanDoHunterCredit;		m_PropertyEnumToName[P_ATTR_ZANDOCREDIT]="ZanDoCredit";		m_mapNameValue["ZanDoCredit"]=st;
	st.lType=2;	st.pdwValue = &m_Property.dwMedalScores;			m_PropertyEnumToName[P_ATTR_MEDALSCORES]="MedalScores";		m_mapNameValue["MedalScores"]=st;*/
	st.lType=0;	st.plValue  = &m_Property.byCountry;				m_PropertyEnumToName[P_ATTR_COUNTRY]="Country";					m_mapNameValue["Country"]=st;				
	st.lType=4;	st.pValue = &m_Property.lCountryContribute;			m_PropertyEnumToName[P_ATTR_COUNTRYCONTRIBUTE]="CountryContribute";	m_mapNameValue["CountryContribute"]=st;	
	st.lType=2;	st.pdwValue = &m_Property.dwSpouseID;				m_PropertyEnumToName[P_ATTR_SPOUSEID]="SpouseId";			m_mapNameValue["SpouseId"]=st;			
	st.lType=2;	st.pdwValue = &m_Property.dwSpouseParam;			m_PropertyEnumToName[P_ATTR_SPOUSEPARAM]="SpouseParam";		m_mapNameValue["SpouseParam;"]=st;		
	st.lType=0; st.plValue  = &m_Property.byBusinessLevel;			m_PropertyEnumToName[P_ATTR_BUSINESSLEVEL]="BusinessLevel";	m_mapNameValue["BusinessLevel"]=st;		
	st.lType=2; st.pdwValue = &m_Property.dwBusinessExp;			m_PropertyEnumToName[P_ATTR_BUSINESSEXP]="BusinessExp";		m_mapNameValue["BusinessExp"]=st;
	/*st.lType=2; st.pdwValue = &m_Property.dwArtisanCredit;			m_PropertyEnumToName[P_ATTR_ARTISANCREDIT]="ArtisanCredit";	m_mapNameValue["ArtisanCredit"]=st;
	st.lType=0; st.plValue  = &m_Property.byArtisanLevel;			m_PropertyEnumToName[P_ATTR_ARTISANLEVEL]="ArtisanLevel";	m_mapNameValue["ArtisanLevel"]=st;		
	st.lType=2; st.pdwValue = &m_Property.dwArtisanExp;				m_PropertyEnumToName[P_ATTR_ARTISANEXP]="ArtisanExp";	;	m_mapNameValue["ArtisanExp"]=st;		*/
	st.lType=2;	st.pdwValue	= &m_Property.dwPkValue;				m_PropertyEnumToName[P_ATTR_PKVALUE]="PkValue";				m_mapNameValue["PkValue"]=st;				
	st.lType=2;	st.pdwValue	= &m_Property.dwPkCount;				m_PropertyEnumToName[P_ATTR_PKCOUNT]="PkCount";				m_mapNameValue["PkCount"]=st;			
	st.lType=2;	st.pdwValue	= &m_Property.dwPVPCount;				m_PropertyEnumToName[P_ATTR_PVPCOUNT]="PVPCount";			m_mapNameValue["PVPCount"]=st;			
	st.lType=2;	st.pdwValue	= &m_Property.dwPVPValue;;				m_PropertyEnumToName[P_ATTR_PVPVALUE]="PVPValue";			m_mapNameValue["PVPValue"]=st;			
	st.lType=0;	st.plValue  = &m_Property.byPKOnOff;  				m_PropertyEnumToName[P_ATTR_PKONOFF]="PkOnOff";				m_mapNameValue["PkOnOff"]=st;			
	st.lType=2; st.pdwValue = &m_Property.dwUpgradeExp;				m_PropertyEnumToName[P_ATTR_UPGRADEEXP]="UpgradeExp";		m_mapNameValue["UpgradeExp"]=st;
	st.lType=2; st.pdwValue = &m_Property.dwCurOccuUpgradeExp;		m_PropertyEnumToName[P_ATTR_CUROCCUUPGRADEEXP]="OccuUpgradeExp";m_mapNameValue["OccuUpgradeExp"]=st;
	st.lType=4; st.pValue = &m_Property.lCurPKCP;		            m_PropertyEnumToName[P_ATTR_CURPKCP]="CurPKCP";              m_mapNameValue["CurPKCP"]=st;
//	st.lType=2; st.pdwValue = &m_Property.dwChurchCredit;		    m_PropertyEnumToName[P_ATTR_CHURCHCREDIT]="ChurchCredit";   m_mapNameValue["ChurchCredit"]=st;

	st.lType=6; st.pbValue  = &m_Property.bShowCountry;				m_PropertyEnumToName[P_ATTR_SHOWCOUNTRY]="ShowCountry";		m_mapNameValue["ShowCountry"]=st;
	char tmp[1024];
	for(int i=0; i<24; ++i)
	{
		sprintf(tmp, "dwHotKey%d", i);
		st.lType=2;	st.pdwValue	= &m_Property.dwHotKey[0];	m_PropertyEnumToName[P_ATTR_HOTKEY0+i]=tmp;		m_mapNameValue[tmp]=st;			
	}
	//ְҵ�ȼ�����
	for(int i=0;i<OCC_Max;i++)
	{
		sprintf(tmp, "OccuLvl%d", i);
		st.lType=0;	st.plValue	= (BYTE*)(&m_Property.byOccuLvl[i]);	m_PropertyEnumToName[P_ATTR_OCCULVL0+i]=tmp;		m_mapNameValue[tmp]=st;
	}
	//ְҵ��������
	for(int i=0;i<OCC_Max;i++)
	{
		sprintf(tmp, "OccuExp%d", i);
		st.lType=2;	st.pdwValue	= &m_Property.dwOccuExp[i];	m_PropertyEnumToName[P_ATTR_OCCUEXP0+i]=tmp;		m_mapNameValue[tmp]=st;
	}
	//ְҵSP����
	for(int i=0;i<OCC_Max;i++)
	{
		sprintf(tmp, "OccuSP%d", i);
		st.lType=2;	st.pdwValue	= &m_Property.dwOccuSP[i];	m_PropertyEnumToName[P_ATTR_OCCUSP0+i]=tmp;		m_mapNameValue[tmp]=st;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*ս������*/ 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.lType=5;	st.psValue	= &m_FightProperty.wShieldDef;		m_PropertyEnumToName[P_ATTR_SHIELD_DEF]="C_ShieldDef";		m_mapNameValue["C_ShieldDef"]=st;							
	st.lType=5;	st.psValue	= &m_FightProperty.wImmunity;		m_PropertyEnumToName[P_ATTR_IMMUNITY]="C_Immunity";			 m_mapNameValue["C_Immunity"]=st;
	st.lType=5;	st.psValue	= &m_FightProperty.wPierce;			m_PropertyEnumToName[P_ATTR_PIERCE]="C_Pierce";				 m_mapNameValue["C_Pierce"]=st;
	st.lType=5;	st.psValue	= &m_FightProperty.wLuck;			m_PropertyEnumToName[P_ATTR_LUCK]="C_Luck";					 m_mapNameValue["C_Luck"]=st;

	//HP�ָ�����
	st.lType=5;	st.psValue	= &m_FightProperty.wHpRecoverRatio;	m_PropertyEnumToName[P_ATTR_HPRECOVERRATIO]="C_HpRecoverRatio"; m_mapNameValue["C_HpRecoverRatio"]=st;							

	st.lType=5;	st.psValue	= &m_FightProperty.wEnerRecoverSpeed;	m_PropertyEnumToName[P_ATTR_ENERRECOVERSPEED]="C_EnerRecoverSpeed";	 m_mapNameValue["C_EnerRecoverSpeed"]=st;							
	st.lType=5;	st.psValue	= &m_FightProperty.wFightEnerRecoverSpeed;m_PropertyEnumToName[P_ATTR_FIGHTENERRECOVERSPEED]="C_FightEnerRecoverSpeed";m_mapNameValue["C_FightEnerRecoverSpeed"]=st;				

	st.lType=4;	st.pValue	= &m_FightProperty.dwMaxHp;				m_PropertyEnumToName[P_ATTR_MAXHP]="C_MaxHp";			   	m_mapNameValue["C_MaxHp"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wMaxMp;				m_PropertyEnumToName[P_ATTR_MAXMP]="C_MaxMp";		m_mapNameValue["C_MaxMp"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wMaxStamina;			m_PropertyEnumToName[P_ATTR_MAXSTAMINA]="C_MaxStamina";		m_mapNameValue["C_MaxStamina"]=st;		
	st.lType=5;	st.psValue	= &m_FightProperty.wMaxEnergy;			m_PropertyEnumToName[P_ATTR_MAXENERGY]="C_MaxEnergy";		m_mapNameValue["C_MaxEnergy"]=st;		
	st.lType=5;	st.psValue	= &m_FightProperty.wHpRecoverSpeed;		m_PropertyEnumToName[P_ATTR_HPRECOVERSPEED]="C_HpRecoverSpeed";	m_mapNameValue["C_HpRecoverSpeed"]=st;	
	st.lType=5;	st.psValue	= &m_FightProperty.wMpRecoverSpeed;		m_PropertyEnumToName[P_ATTR_MPRECOVERSPEED]="C_MpRecoverSpeed";	m_mapNameValue["C_MpRecoverSpeed"]=st;	
	st.lType=5;	st.psValue	= &m_FightProperty.wStamRecoverSpeed;	m_PropertyEnumToName[P_ATTR_STAMRECOVERSPEED]="C_StamRecoverSpeed";	m_mapNameValue["C_StamRecoverSpeed"]=st;	
	/*st.lType=5;	st.psValue	= &m_FightProperty.wStrenth;			m_PropertyEnumToName[P_ATTR_STRENTH]="C_Strenth";			m_mapNameValue["C_Strenth"]=st;				
	st.lType=5;	st.psValue	= &m_FightProperty.wAgility;			m_PropertyEnumToName[P_ATTR_AGILITY]="C_Agility";			m_mapNameValue["C_Agility"]=st;				
	st.lType=5;	st.psValue	= &m_FightProperty.wConsti;				m_PropertyEnumToName[P_ATTR_CONSTI]="C_Consti";			m_mapNameValue["C_Consti"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wIntell;				m_PropertyEnumToName[P_ATTR_INTELL]="C_Intell";			m_mapNameValue["C_Intell"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wWisdom;				m_PropertyEnumToName[P_ATTR_WISDOM]="C_Wisdom";			m_mapNameValue["C_Wisdom"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wSpirit;				m_PropertyEnumToName[P_ATTR_SPIRIT]="C_Spirit";			m_mapNameValue["C_Spirit"]=st;		*/	
	st.lType=5;	st.psValue	= &m_FightProperty.wFightBlock;			m_PropertyEnumToName[P_ATTR_BLOCK]="C_Block";				m_mapNameValue["C_Block"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wParry;				m_PropertyEnumToName[P_ATTR_PARRY]="C_Parry";				m_mapNameValue["C_Parry"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wDodge;				m_PropertyEnumToName[P_ATTR_DODGE]="C_Dodge";				m_mapNameValue["C_Dodge"]=st;			

	st.lType=5;	st.psValue	= &m_FightProperty.wCharm;				m_PropertyEnumToName[P_ATTR_CHARM]="C_Charm";			m_mapNameValue["C_Charm"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wHit;				m_PropertyEnumToName[P_ATTR_HIT]="C_Hit";			m_mapNameValue["C_Hit"]=st;					
	st.lType=5;	st.psValue	= &m_FightProperty.wAbsorb;				m_PropertyEnumToName[P_ATTR_ABSORB]="C_Absorb";		m_mapNameValue["C_Absorb"]=st;			
	st.lType=4;	st.psValue	= &m_FightProperty.wDef;				m_PropertyEnumToName[P_ATTR_DEF]="C_Def";			m_mapNameValue["C_Def"]=st;					
	st.lType=4;	st.psValue	= &m_FightProperty.wMdef;				m_PropertyEnumToName[P_ATTR_MDEF]="C_Mdef";			m_mapNameValue["C_Mdef"]=st;				
	st.lType=4;	st.psValue	= &m_FightProperty.wCri;				m_PropertyEnumToName[P_ATTR_CRI]="C_Cri";			m_mapNameValue["C_Cri"]=st;					
	st.lType=5;	st.psValue	= &m_FightProperty.wMAtkSpeed;			m_PropertyEnumToName[P_ATTR_MATKSPEED]="C_MAtkSpeed";		m_mapNameValue["C_MAtkSpeed"]=st;		
	st.lType=5;	st.psValue	= &m_FightProperty.wBuffPower;			m_PropertyEnumToName[P_ATTR_BUFFPOWER]="SpeedC_BuffPower";m_mapNameValue["C_BuffPower"]=st;		
	st.lType=4;	st.psValue	= &m_FightProperty.wMinAtk;;			m_PropertyEnumToName[P_ATTR_MINATK]="C_MinAtk";		m_mapNameValue["C_MinAtk"]=st;			
	st.lType=4;	st.psValue	= &m_FightProperty.wMaxAtk;				m_PropertyEnumToName[P_ATTR_MAXATK]="C_MaxAtk";		m_mapNameValue["C_MaxAtk"]=st;			
	st.lType=4;	st.pValue	= &m_FightProperty.wMAtk;				m_PropertyEnumToName[P_ATTR_MATK]="C_MAtk";			m_mapNameValue["C_MAtk"]=st;				
	st.lType=4;	st.pValue	= &m_FightProperty.wMCri;				m_PropertyEnumToName[P_ATTR_MCRI]="C_MCri";			m_mapNameValue["C_MCri"]=st;				
	st.lType=5;	st.psValue	= &m_FightProperty.wBloodSuck;			m_PropertyEnumToName[P_ATTR_BLOODSUCK]="C_BloodSuck";		m_mapNameValue["C_BloodSuck"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wManaSuck;			m_PropertyEnumToName[P_ATTR_MANASUCK]="C_ManaSuck";		m_mapNameValue["C_ManaSuck"]=st;			
	st.lType=5;	st.psValue	= &m_FightProperty.wWillDef;			m_PropertyEnumToName[P_ATTR_WILLDEF]="C_WillDef";		m_mapNameValue["C_WillDef"]=st;				
	st.lType=5;	st.psValue	= &m_FightProperty.wConstiDef;			m_PropertyEnumToName[P_ATTR_CONSTIDEF]="C_ConstiDef";	m_mapNameValue["C_ConstiDef"]=st;		

	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DFT_Bled];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_BLED]="C_DeBuffDef0";	 m_mapNameValue["C_DeBuffDef0"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Dizzy];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_DIZZY]="C_DeBuffDef1";	 m_mapNameValue["C_DeBuffDef1"]=st;			
	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Lull];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_LULL]="C_DeBuffDef2";	 m_mapNameValue["C_DeBuffDef2"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Hypnosis];m_PropertyEnumToName[P_ATTR_DEBUFFDEF_HYPNOSIS]="C_DeBuffDef3";	m_mapNameValue["C_DeBuffDef3"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Poison];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_POSION]="C_DeBuffDef4"; m_mapNameValue["C_DeBuffDef4"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Fix];		m_PropertyEnumToName[P_ATTR_DEBUFFDEF_FIX]="C_DeBuffDef5";	 m_mapNameValue["C_DeBuffDef5"]=st;		

	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Fire];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_FIRE]="C_ElemDef0";  m_mapNameValue["C_ElemDef0"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Water];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_WATER]="C_ElemDef1";  m_mapNameValue["C_ElemDef1"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Earth];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_EARTH]="C_ElemDef2";  m_mapNameValue["C_ElemDef2"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Wind];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_WIND]="C_ElemDef3";  m_mapNameValue["C_ElemDef3"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Dark];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_DARK]="C_ElemDef4";	  m_mapNameValue["C_ElemDef4"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Holy];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_HOLY]="C_ElemDef5";	  m_mapNameValue["C_ElemDef5"]=st;	


	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Human];			m_PropertyEnumToName[P_ATTR_CLASSDAM_HUMAN]="C_ClassDam0";   m_mapNameValue["C_ClassDam0"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Element];	;		m_PropertyEnumToName[P_ATTR_CLASSDAM_ELEMENT]="C_ClassDam1";   m_mapNameValue["C_ClassDam1"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Alien];			m_PropertyEnumToName[P_ATTR_CLASSDAM_ALIEN]="C_ClassDam2";   m_mapNameValue["C_ClassDam2"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Animal];			m_PropertyEnumToName[P_ATTR_CLASSDAM_ANIMAL]="C_ClassDam3";  m_mapNameValue["C_ClassDam3"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Undead];			m_PropertyEnumToName[P_ATTR_CLASSDAM_UNDEAD]="C_ClassDam4";  m_mapNameValue["C_ClassDam4"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Evil];			    m_PropertyEnumToName[P_ATTR_CLASSDAM_EVIL]="C_ClassDam5";   m_mapNameValue["C_ClassDam5"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Hexapods];		m_PropertyEnumToName[P_ATTR_CLASSDAM_HEXAPODS]="C_ClassDam6";  m_mapNameValue["C_ClassDam6"]=st;	

	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Fire];			    m_PropertyEnumToName[P_ATTR_ELEMDAM_FIRE]="C_ElemDam0";		m_mapNameValue["C_ElemDam0"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Water];			m_PropertyEnumToName[P_ATTR_ELEMDAM_WATER]="C_ElemDam1";		m_mapNameValue["C_ElemDam1"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Earth];			m_PropertyEnumToName[P_ATTR_ELEMDAM_EARTH]="C_ElemDam2";		m_mapNameValue["C_ElemDam2"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Wind];	;		    m_PropertyEnumToName[P_ATTR_ELEMDAM_WIND]="C_ElemDam3";		m_mapNameValue["C_ElemDam3"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Dark];		    	m_PropertyEnumToName[P_ATTR_ELEMDAM_DARK]="C_ElemDam4";		m_mapNameValue["C_ElemDam4"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Holy];			    m_PropertyEnumToName[P_ATTR_ELEMDAM_HOLY]="C_ElemDam5";		m_mapNameValue["C_ElemDam5"]=st;	

	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Fire];			    m_PropertyEnumToName[P_ATTR_ELEMATK_FIRE]="C_ElemAtk0";		m_mapNameValue["C_ElemAtk0"]=st;		
	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Water];			m_PropertyEnumToName[P_ATTR_ELEMATK_WATER]="C_ElemAtk1";		m_mapNameValue["C_ElemAtk1"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Earth];			m_PropertyEnumToName[P_ATTR_ELEMATK_EARTH]="C_ElemAtk2";		m_mapNameValue["C_ElemAtk2"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Wind];			    m_PropertyEnumToName[P_ATTR_ELEMATK_WIND]="C_ElemAtk3";		m_mapNameValue["C_ElemAtk3"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Dark];			    m_PropertyEnumToName[P_ATTR_ELEMATK_DARK]="C_ElemAtk4";		m_mapNameValue["C_ElemAtk4"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Holy];			    m_PropertyEnumToName[P_ATTR_ELEMATK_HOLY]="C_ElemAtk5";		m_mapNameValue["C_ElemAtk5"]=st;	

	st.lType=5;st.psValue = &m_FightProperty.wCriDef;		m_PropertyEnumToName[P_ATTR_CRIDEF]="C_CriDef";								m_mapNameValue["C_CriDef"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wMCriDef;		m_PropertyEnumToName[P_ATTR_MCRIDEF]="C_MCriDef";							m_mapNameValue["C_MCriDef"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wCriDamage;	m_PropertyEnumToName[P_ATTR_CRIDAMAGE]="C_CriDamage";						m_mapNameValue["C_CriDamage"]=st;	
	st.lType=5;st.psValue = &m_FightProperty.wMCriDamage;	m_PropertyEnumToName[P_ATTR_MCRIDAMAGE]="C_MCriDamage";						m_mapNameValue["C_MCriDamage"]=st;	

	st.lType=4; st.pValue = &m_FightProperty.lMaxPKCP;		            m_PropertyEnumToName[P_ATTR_MAXPKCP]="C_MaxPKCP";              m_mapNameValue["C_MaxPKCP"]=st;


	st.lType=2;	st.pdwValue=&m_dwFactureExp;				m_PropertyEnumToName[P_ATTR_FACTUREEXP]="FactureExp";						m_mapNameValue["FactureExp"]=st;						//	��������
	st.lType=2;	st.pdwValue=&m_dwCollectExp;				m_PropertyEnumToName[P_ATTR_COLLECTIONEXP]="CollectionExp";					m_mapNameValue["CollectionExp"]=st;					//	�ɼ�����
	st.lType=2;	st.pdwValue=&m_dwFactureLevel;				m_PropertyEnumToName[P_ATTR_FACTUREGRADE]="FactureGrade";					m_mapNameValue["FactureGrade"]=st;					//	�����ȼ�
	st.lType=2;	st.pdwValue=&m_dwCollectLevel;				m_PropertyEnumToName[P_ATTR_COLLECTIONGRADE]="CollectionGrade";				m_mapNameValue["CollectionGrade"]=st;				//	�ɼ��ȼ�

	st.lType=0;	st.plValue  = &m_AppendProperty.bShowFashion;		m_PropertyEnumToName[P_ATTR_DISPLAYHEADPIECE]="ShowFashion";m_mapNameValue["ShowFashion"]=st;
	//GM����
	st.lType=6;	st.pbValue  = &m_bGMConcealment;		m_PropertyEnumToName[P_ATTR_CONCEALMENT]="GMConcealment";m_mapNameValue["GMConcealment"]=st;

	// ���壺ħ������
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Attack];	m_PropertyEnumToName[P_EARTH_ATTACK] = "Earth_Attack";	m_mapNameValue["Earth_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Defend];	m_PropertyEnumToName[P_EARTH_DEFEND] = "Earth_Defend";	m_mapNameValue["Earth_Defend"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Attack];	m_PropertyEnumToName[P_WATER_ATTACK] = "Water_Attack";	m_mapNameValue["Water_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Defend];	m_PropertyEnumToName[P_WATER_DEFEND] = "Water_Defend";	m_mapNameValue["Water_Defend"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Attack];	m_PropertyEnumToName[P_FIRE_ATTACK] = "Fire_Attack";		m_mapNameValue["Fire_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Defend];	m_PropertyEnumToName[P_FIRE_DEFEND] = "Fire_Defend";		m_mapNameValue["Fire_Defend"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Attack];	m_PropertyEnumToName[P_WIND_ATTACK] = "Wind_Attack";		m_mapNameValue["Wind_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Defend];	m_PropertyEnumToName[P_WIND_DEFEND] = "Wind_Defend";		m_mapNameValue["Wind_Defend"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Attack];	m_PropertyEnumToName[P_BRIGHT_ATTACK] = "Bright_Attack";	m_mapNameValue["Bright_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Defend];	m_PropertyEnumToName[P_BRIGHT_DEFEND] = "Bright_Defend";	m_mapNameValue["Bright_Defend"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Attack];	m_PropertyEnumToName[P_DARK_ATTACK] = "Dark_Attack";		m_mapNameValue["Dark_Attack"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Defend];	m_PropertyEnumToName[P_DARK_DEFEND] = "Dark_Defend";		m_mapNameValue["Dark_Defend"]=st;

	st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_AttackXiang];		m_PropertyEnumToName[P_EARTH_ATTACKXIANG] = "Earth_AttackXiang";	m_mapNameValue["Earth_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_DefendXiang];		m_PropertyEnumToName[P_EARTH_DEFENDXIANG] = "Earth_DefendXiang";	m_mapNameValue["Earth_DefendXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_AttackXiang];		m_PropertyEnumToName[P_WATER_ATTACKXIANG] = "Water_AttackXiang";	m_mapNameValue["Water_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_DefendXiang];		m_PropertyEnumToName[P_WATER_DEFENDXIANG] = "Water_DefendXiang";	m_mapNameValue["Water_DefendXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_AttackXiang];		m_PropertyEnumToName[P_FIRE_ATTACKXIANG] = "Fire_AttackXiang";	m_mapNameValue["Fire_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_DefendXiang];		m_PropertyEnumToName[P_FIRE_DEFENDXIANG] = "Fire_DefendXiang";	m_mapNameValue["Fire_DefendXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_AttackXiang];		m_PropertyEnumToName[P_WIND_ATTACKXIANG] = "Wind_AttackXiang";	m_mapNameValue["Wind_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_DefendXiang];		m_PropertyEnumToName[P_WIND_DEFENDXIANG] = "Wind_DefendXiang";	m_mapNameValue["Wind_DefendXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_AttackXiang];	m_PropertyEnumToName[P_BRIGHT_ATTACKXIANG] = "Bright_AttackXiang";	m_mapNameValue["Bright_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_DefendXiang];	m_PropertyEnumToName[P_BRIGHT_DEFENDXIANG] = "Bright_DefendXiang";	m_mapNameValue["Bright_DefendXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_AttackXiang];		m_PropertyEnumToName[P_DARK_ATTACKXIANG] = "Dark_AttackXiang";	m_mapNameValue["Dark_AttackXiang"]=st;
	st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_DefendXiang];		m_PropertyEnumToName[P_DARK_DEFENDXIANG] = "Dark_DefendXiang";	m_mapNameValue["Dark_DefendXiang"]=st;
}

CGoods* CPlayer::GetEquip(int index)
{
	if(index<0 || index>=EQUIP_SUM )
		return NULL;
	return m_Equip[index];
}

void CPlayer::SetEquip(int index,CGoods *pGood)
{
	if(index<0 || index>=EQUIP_SUM)
		return ;
	m_Equip[index]=pGood;
}

// �õ�ĳ��Player�ܹ����ķ�������
eEquipLimitType CPlayer::QueryPlayerEquipLimitType(CPlayer* pPlayer)
{
	eEquipLimitType eEquipType = ELT_UNKNOWN;
	if (pPlayer!=NULL)
	{
		eOccupation eOccu = pPlayer->GetOccupation();
		switch(eOccu)
		{
		case OCC_Mystic:		// ��ʦ
		case OCC_Warlock:		// ��ʿ
		case OCC_Cleric:		// ��ʦ
		case OCC_Sorcerer:		// ��ʦ
		case OCC_Necromancer:	// ���鷨ʦ
		case OCC_Bishop:		// ����
		case OCC_Prophet:		// ����
			eEquipType = ELT_LIGHT;
			break;
		case OCC_Rogue:			// ����
		case OCC_Scout:			// ���
		case OCC_Craftsman:		// ����
		case OCC_Ranger:		// Ѳ��
		case OCC_Assassin:		// �̿�
		case OCC_BladeDancer:	// ������
		case OCC_Bard:			// ����ʫ��
			eEquipType = ELT_MID;
			break;
		case OCC_Fighter:		// սʿ
		case OCC_Knight:		// ��ʿ
		case OCC_Warrior:		// ��ʿ
		case OCC_Paladin:		// ʥ��ʿ
		case OCC_DarkKnight:	// ����ʿ
		case OCC_Gladiator:		// ��ʿ
		case OCC_Berserker:		// ��սʿ
			eEquipType = ELT_HEAVY;
			break;
		}
	}
	return eEquipType;
}

//��ȡ��ǰ�����Ŀո�������
long CPlayer::GetEmptyGridNum()
{
	// �󱳰��ܵ�����
	DWORD dwTotlePackageDimen = 0;		
	// �󱳰�����Ʒ������
	DWORD dwTotleGoodsNum = 0;
	for (int i = 0;i<PACK_NUM;i++)
	{
		if (i==0)
		{
			dwTotlePackageDimen+=GetPacketDimen(PEI_PACKET);
		}
        else
			dwTotlePackageDimen+=GetPacketDimen(PEI_SUBPACK+i);

		list<CPlayer::tagGoods>*pGoodsList = GetGoodsList(i);
		if (pGoodsList!=NULL)
		{
			dwTotleGoodsNum += (DWORD)pGoodsList->size();
		}
	}
	return dwTotlePackageDimen-dwTotleGoodsNum;
}

//bool CPlayer::CanEquip(DWORD dwGoodsIndex)
//{
//	if (dwGoodsIndex!=0)
//	{
//		DWORD dwGoodsType = CGoodsList::GetProperty(dwGoodsIndex)->BaseProperty.dwType;
//		if (dwGoodsType==GBT_EQUIPMENT)
//		{
//			//װ�����������ж�
//			DWORD dwVal=0;
//			//�ȼ�����
//			dwVal = CGoodsList::GetEffectVal( dwGoodsIndex,GAP_ROLE_LEVEL_LIMIT, 1 );
//			if(dwVal!=0 && dwVal>GetLevel())
//				return false;
//
//			//��������
//			dwVal = CGoodsList::GetEffectVal( dwGoodsIndex,GAP_SKILL_LIMIT,1);
//			if(dwVal!=0)
//			{
//				DWORD dwLev=CheckSkill(dwVal);
//				if(dwLev<=0)
//					return false;
//			}
//
//			//������������
//			dwVal = CGoodsList::GetEffectVal( dwGoodsIndex, GAP_GUARD_EQUIP_CATEGORY,1);
//			if(dwVal!=0)
//			{
//				eEquipLimitType elt = QueryPlayerEquipLimitType(this);
//				if(elt!=dwVal)
//					return false;
//			}
//
//			//ְҵ����
//			if (CGoodsList::HaveEffect(dwGoodsIndex, GAP_REQUIRE_OCCUPATION))
//			{
//				dwVal = CGoodsList::GetEffectVal( dwGoodsIndex, GAP_REQUIRE_OCCUPATION, 1 );
//				long lVal = 0;
//				int i = 0;
//				for (;i<OCC_Max;i++)
//				{
//					lVal = (dwVal>>i)&0x1; 
//					if (lVal!=0)
//					{
//						if (GetOccupation()==(eOccupation)i)
//						{
//							break;
//						}
//					}
//				}
//				if(i==OCC_Max)
//					return false;
//			}
//
//
//			//�Ա�����
//			dwVal = CGoodsList::GetEffectVal( dwGoodsIndex, GAP_REQUIRE_GENDER, 1 );
//			if(dwVal!=0 && dwVal!=GetSex()+1)
//				return false;
//
//			// ������������������������˵�һЩ�����ж�����������
//			switch(dwGoodsType)
//			{
//			case GBT_E_WEAPON:
//				{
//					long eWT = CGoodsList::GetEffectVal( dwGoodsIndex, GAP_WEAPON_CATEGORY,1);
//				//	CGoods* pWeapon2 = GetEquip(EQUIP_WEAPON2);
//					CGoods* pWeapon1 = GetEquip(EQUIP_WEAPON);
//					if (pWeapon1!=NULL)
//					{
//						long eWP = pWeapon1->GetEffectVal( GAP_WEAPON_CATEGORY,1);
//					//	long eWP2 = pWeapon2->GetEffectVal(CGoods::GAP_WEAPON_CATEGORY,1);
//						if ((eWT!=WT_SINGLE_SWORD&&eWT!=WT_SINGLE_HAMMER&&eWT!=WT_SINGLE_KNIFE)
//							&&GetEmptyGridNum()<2)
//						{
//							return false;
//						}
//					}
//					//switch(eWT)
//					//{
//					//case WT_SINGLE_SWORD:	//���ֽ�
//					//case WT_SINGLE_HAMMER:	//���ִ�
//					//case WT_SINGLE_KNIFE:	//ذ��
//					//case WT_BOW:
//					//	{
//
//					//	}
//					//    break;
//					//case WT_WAND:			//����
//					//case WT_HEAVY_SWORD:	//˫�ֽ�
//					//case WT_HEAVY_AX:		//˫�ָ�
//					//case WT_DOUBLE_SWORD:	//˫��
//					//case WT_FIST:			//ȭ��
//					//	{
//					//		if (pWeapon2!=NULL&&
//					//		   (pWeapon2->GetProperty()->dwType==GT_WEAPON2||pWeapon2->GetProperty()->dwType==GT_ARROW))
//					//		{
//					//			return false;
//					//		}
//					//	}
//					//    break;
//					//}
//				}
//				break;
//			/*case GT_WEAPON2:
//				{
//					CGoods* pWeapon = GetEquip(EQUIP_WEAPON);
//					if (pWeapon!=NULL)
//					{
//						long eWT = pWeapon->GetEffectVal(CGoods::GAP_WEAPON_CATEGORY,1);
//						if (eWT!=WT_SINGLE_SWORD&&eWT!=WT_SINGLE_HAMMER&&eWT!=WT_SINGLE_KNIFE)
//						{
//							return false;
//						}
//					}
//				}
//				break;
//			case GT_ARROW:
//				{
//					CGoods* pWeapon = GetEquip(EQUIP_WEAPON);
//					if (pWeapon!=NULL)
//					{
//						long eWT = pWeapon->GetEffectVal(CGoods::GAP_WEAPON_CATEGORY,1);
//						if (eWT!=WT_BOW)
//						{
//							return false;
//						}
//					}
//				}
//				break;*/
//				//case GT_HORSE:
//				//	{
//				//		// Ԥ��������ʱ��û������
//				//	}
//				//	break;
//			}
//		}
//		return true;
//	}
//	return false;
//}

// ������Ʒ��ǰ�Ƿ���װ��
bool CPlayer::CanEquip(CGoods* pGoods)
{
	if (pGoods!=NULL)
	{
		DWORD dwGoodsType = pGoods->GetProperty()->dwType;
		if (dwGoodsType==GBT_EQUIPMENT)
		{
			//װ�����������ж�
			DWORD dwVal=0;
			//�ȼ�����
			dwVal = pGoods ->GetEffectVal( GAP_ROLE_LEVEL_LIMIT, 1 );
			if(dwVal!=0 && dwVal>GetLevel())
				return false;

			//��������
			dwVal=pGoods->GetEffectVal(GAP_SKILL_LIMIT,1);
			if(dwVal!=0)
			{
				DWORD dwLev=CheckSkill(dwVal);
				if(dwLev<=0)
					return false;
			}

			//������������
			dwVal = pGoods->GetEffectVal(GAP_GUARD_EQUIP_CATEGORY,1);
			if(dwVal!=0)
			{
				eEquipLimitType elt = QueryPlayerEquipLimitType(this);
				if(elt!=dwVal)
					return false;
			}

			//ְҵ����
			if (pGoods->HasEffect(GAP_REQUIRE_OCCUPATION))
			{
				dwVal=pGoods -> GetEffectVal( GAP_REQUIRE_OCCUPATION, 1 );
				long lVal = 0;
				int i = 0;
				for (;i<OCC_Max;i++)
				{
					lVal = (dwVal>>i)&0x1; 
					if (lVal!=0)
					{
						if (GetOccupation()==(eOccupation)i)
						{
							break;
						}
					}
				}
				if(i==OCC_Max)
					return false;
			}

			//�Ա�����
			dwVal=pGoods -> GetEffectVal( GAP_REQUIRE_GENDER, 1 );
			if(dwVal!=0 && dwVal!=GetSex()+1)
				return false;

			// ������������������������˵�һЩ�����ж�����������
			ulong equipType = pGoods->GetEquipType();
			switch(equipType)
			{
			case EC_E_WEAPON:
				{
					long eWT = pGoods->GetEffectVal(GAP_WEAPON_CATEGORY,1);
					CGoods* pWeapon1 = GetEquip(EQUIP_WEAPON);
					if (pWeapon1!=NULL)
					{
						long eWP = pWeapon1->GetEffectVal(GAP_WEAPON_CATEGORY,1);
						if ((eWT!=WT_SINGLE_SWORD&&eWT!=WT_SINGLE_HAMMER&&eWT!=WT_SINGLE_KNIFE)
							&&GetEmptyGridNum()<2)
						{
							return false;
						}
					}
					//switch(eWT)
					//{
					//case WT_SINGLE_SWORD:	//���ֽ�
					//case WT_SINGLE_HAMMER:	//���ִ�
					//case WT_SINGLE_KNIFE:	//ذ��
					//case WT_BOW:			//��
					//	{

					//	}
					//    break;
					//case WT_WAND:			//����
					//case WT_HEAVY_SWORD:	//˫�ֽ�
					//case WT_HEAVY_AX:		//˫�ָ�
					//case WT_DOUBLE_SWORD:	//˫��
					//case WT_FIST:			//ȭ��
					//	{
					//		if (pWeapon2!=NULL&&
					//		   (pWeapon2->GetProperty()->dwType==GT_WEAPON2||pWeapon2->GetProperty()->dwType==GT_ARROW))
					//		{
					//			return false;
					//		}
					//	}
					//    break;
					//}
				}
				break;
			/*case GT_WEAPON2:
				{
					CGoods* pWeapon = GetEquip(EQUIP_WEAPON);
					if (pWeapon!=NULL)
					{
						long eWT = pWeapon->GetEffectVal(CGoods::GAP_WEAPON_CATEGORY,1);
						if (eWT!=WT_SINGLE_SWORD&&eWT!=WT_SINGLE_HAMMER&&eWT!=WT_SINGLE_KNIFE)
						{
							return false;
						}
					}
				}
				break;
			case GT_ARROW:
				{
					CGoods* pWeapon = GetEquip(EQUIP_WEAPON);
					if (pWeapon!=NULL)
					{
						long eWT = pWeapon->GetEffectVal(CGoods::GAP_WEAPON_CATEGORY,1);
						if (eWT!=WT_BOW)
						{
							return false;
						}
					}
				}
				break;*/
				//case GT_HORSE:
				//	{
				//		// Ԥ��������ʱ��û������
				//	}
				//	break;
			}
		}
		return true;
	}
	return false;
}

//void CPlayer::SetHand(bool bChanged,WORD lNum,DWORD lPos,CGoods *pGood,DWORD lType)
//{
//	m_stHand.bChange=bChanged;
//	m_stHand.lNum=lNum;
//	m_stHand.lPos=lPos;
//	m_stHand.pGoods=pGood;
//	m_stHand.lType=lType;
//}

CGoods* CPlayer::GetGoodsByGUID(const CGUID& guID,int *pPos,long* lExtendID)
{
	CGoods *pGoods=NULL;

	//if(m_stHand.pGoods && m_stHand.pGoods->GetExID() == guID)
	//	return m_stHand.pGoods;


	//��װ���в���
	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	{
		pGoods = GetEquip(i);
		if(pGoods)
		{
			if(pGoods->GetExID() == guID)
			{
				if(pPos!=NULL) pPos[0]=i;
				if (lExtendID!=NULL) (*lExtendID) = PEI_EQUIPMENT;
				return pGoods;
			}
		}
	}
	//���������в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			pGoods = GoodsIter->pGoods;
			if(pGoods && pGoods->GetExID() == guID)
			{
				if(pPos!=NULL) pPos[0]=GoodsIter->lPos;
				if (lExtendID!=NULL) (*lExtendID) = PEI_PACKET;
				return pGoods;
			}
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ�����λ�ϲ���
		pGoods = GetSubPGoods(i);
		if (pGoods&&pGoods->GetExID()==guID)
		{
			if(pPos!=NULL) pPos[0] = PEI_PACK1+i;
			if (lExtendID!=NULL) (*lExtendID) = PEI_SUBPACK;
			return pGoods;
		}
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList!=NULL)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				pGoods = GoodsIter->pGoods;
				if(pGoods && pGoods->GetExID() == guID)
				{
					if(pPos!=NULL) pPos[0]=GoodsIter->lPos;
					if (lExtendID!=NULL) (*lExtendID) = (PEI_SUBPACK+i+1);;
					return pGoods;
				}
			}
		}
	}

	// ���壺�ڲֿ��в���
	CPlayer::tagGoods	tagGoodsInDepot = CGCMana::Inst()->GetGoods( guID );
	if( tagGoodsInDepot.pGoods )
	{
		if( lExtendID )
			*lExtendID = tagGoodsInDepot.lType;

		if( pPos )
			*pPos = tagGoodsInDepot.lPos;

		return tagGoodsInDepot.pGoods;
	}

	/// ���ʱ�����λ�ϲ���
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		if (m_CoteBussinesPack->pGoods->GetExID()==guID)
		{
			return m_CoteBussinesPack->pGoods;
		}
	}

	///�����ʱ����в���
	pGoodList = NULL;
	if (m_CoteBussinesPack!=NULL)
	{
		pGoodList = &m_CoteBussinesPack->m_listCoteGoods;
	}
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			pGoods = GoodsIter->pGoods;
			if(pGoods && pGoods->GetExID() == guID)
			{
				if(pPos!=NULL) pPos[0]=GoodsIter->lPos;
				if (lExtendID!=NULL) (*lExtendID) = PEI_BUSINESSPACK;
				return pGoods;
			}
		}
	}

	return NULL;
}



long CPlayer::CheckSpace(CGoods* pGoods)
{
	/*int w=pGoods->GetBaseProperty()->lX_Size,h=pGoods->GetBaseProperty()->lY_Size;
	int w=1,h=1;
	for(long i=0;i<48;i++)
	{
	if(GetGame()->GetInterface()->GetItemsPage()->CheckDataIsEmpty(i,w,h))
	{
	return i;
	}
	}*/
	return -1;
}

DWORD CPlayer::TotalRepairPrice()
{
	DWORD dwTotalPrice=0;
	list<CPlayer::tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		list<CPlayer::tagGoods>::iterator GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			if(GoodsIter->pGoods->CanRepair())
			{
				//vector<CGoodsList::tagEffect>::iterator EffectIter=GoodsIter->pGoods->GetEffect()->begin();
				//for(;EffectIter!=GoodsIter->pGoods->GetEffect()->end();EffectIter++)
				//{
				//	if(EffectIter->wType==37 && EffectIter->dwValue2<EffectIter->dwValue1)
				//	{
				//		DWORD dwPrice=DWORD(GoodsIter->pGoods->GetBaseProperty()->dwValue * 
				//			(EffectIter->dwValue1 - EffectIter->dwValue2)/EffectIter->dwValue1 * 1.3 * (1+0.1));
				//		dwTotalPrice+=dwPrice;
				//	}
				//}
				dwTotalPrice+=GoodsIter->pGoods->RepairPrice();
			}
		}
	}
	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	{
		CGoods* pGoods=GetEquip(i);
		if(pGoods)
		{
			//vector<CGoodsList::tagEffect>::iterator EffectIter=pGoods->GetEffect()->begin();
			//for(;EffectIter!=pGoods->GetEffect()->end();EffectIter++)
			//{
			//	if(EffectIter->wType==37 && EffectIter->dwValue2<EffectIter->dwValue1)
			//	{
			//		DWORD dwPrice=DWORD(pGoods->GetBaseProperty()->dwValue * 
			//			(EffectIter->dwValue1 - EffectIter->dwValue2)/EffectIter->dwValue1 * 1.3 * (1+0.1));
			//		dwTotalPrice+=dwPrice;
			//	}
			//}
			dwTotalPrice+=pGoods->RepairPrice();
		}
	}
	return dwTotalPrice;
}



/*
* ����: ��ʼ����ʾģ��
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.04.02 - lpf
*		����������,����,��ɫģ�Ͳ������趨;
*	2008.10.17 - lpf
*		�����˾ֲ����������ʼֵ;
*	2009.03.05 - lpf
*		�����˸����Լ�������ʱ,�����翪���Ƿ��������ж�;
*	2009.03.13 - lpf
*		������������ʾ�¹���:���������ͼ,�������Ƿ���ʾ����;
*/
void CPlayer::SetDisplayModelGroup(bool bAsyn)
{
	if (m_pDisplayModel == NULL)
	{
		return;
	}
	//��ʼ����ʾģ������
	DWORD dwHelm=0, dwBody=0,dwGlove=0, dwShoes=0, dwWaistBand=0, dwWeaponActType=0,dwPifeng = 0;
	int   lHelmLevel=0, lBodyLevel=0,lGloveLevel=0, lShoesLevel=0, lBackLevel=0,dwPifengLevel = 0;

	// �����Լ�
	if (m_bSelf)
	{
		CGoods* pEquip = GetEquip(CPlayer::EQUIP_HEAD);
		dwHelm = pEquip?pEquip->GetProperty()->dwEquipID:0;
		lHelmLevel = pEquip?pEquip->GetEffectVal(GAP_WEAPON_LEVEL,1):0;

		pEquip = GetEquip(CPlayer::EQUIP_BODY);
		dwBody = pEquip?pEquip->GetProperty()->dwEquipID:1;
		lBodyLevel = pEquip?pEquip->GetEffectVal(GAP_WEAPON_LEVEL,1):0;

		if (GetGame()->GetRegion() == NULL ||
			(GetGame()->GetRegion() != NULL && GetGame()->GetRegion()->IsChangeEquip()))
		{
			if (GetShowFashion() & FASHION_TYPE_MANTEAU/*!GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_NoManteau)*/)
			{
				/*pEquip = GetEquip(CPlayer::EQUIP_PIFENG);
				dwPifeng = pEquip?pEquip->GetProperty()->dwEquipID:0;
				dwPifengLevel = pEquip?pEquip->GetEffectVal(CGoods::GAP_WEAPON_LEVEL,1):0;*/
			}
		}

		//dwWeaponActType = pEquip?pEquip->GetProperty()->dwWeaponActType:0;
		pEquip = GetEquip(CPlayer::EQUIP_HAND);
		dwGlove = pEquip?pEquip->GetProperty()->dwEquipID:1;
		lGloveLevel = pEquip?pEquip->GetEffectVal(GAP_WEAPON_LEVEL,1):0;

		pEquip = GetEquip(CPlayer::EQUIP_BOOT);
		dwShoes = pEquip?pEquip->GetProperty()->dwEquipID:1;
		lShoesLevel = pEquip?pEquip->GetEffectVal(GAP_WEAPON_LEVEL,1):0;

		pEquip = GetEquip(CPlayer::EQUIP_BACK);
		dwWaistBand = pEquip?pEquip->GetProperty()->dwEquipID:0;
		lBackLevel = pEquip?pEquip->GetEffectVal(GAP_WEAPON_LEVEL,1):0;
	}
	else	// �������
	{
		CGoodsList::tagGoods2 * pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_HEAD));
		dwHelm = pst?pst->BaseProperty.dwEquipID:0;
		lHelmLevel = GetEquipLevel(CPlayer::EQUIP_HEAD);

		pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_BODY));
		dwBody = pst?pst->BaseProperty.dwEquipID:1;
		lBodyLevel = GetEquipLevel(CPlayer::EQUIP_BODY);

		if (GetGame()->GetRegion() == NULL ||
			(GetGame()->GetRegion() != NULL && GetGame()->GetRegion()->IsChangeEquip()))
		{
			if (GetShowFashion() & FASHION_TYPE_MANTEAU)
			{
				/*pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_PIFENG));
				dwPifeng = pst?pst->BaseProperty.dwEquipID:0;
				dwPifengLevel = GetEquipLevel(CPlayer::EQUIP_PIFENG);*/
			}
		}
		//dwWeaponActType = pst?pst->BaseProperty.dwWeaponActType:0;

		pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_HAND));
		dwGlove = pst?pst->BaseProperty.dwEquipID:1;
		lGloveLevel = GetEquipLevel(CPlayer::EQUIP_HAND);

		pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_BOOT));
		dwShoes = pst?pst->BaseProperty.dwEquipID:1;
		lShoesLevel = GetEquipLevel(CPlayer::EQUIP_BOOT);

		pst = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_BACK));
		dwWaistBand = pst?pst->BaseProperty.dwEquipID:0;
		lBackLevel = GetEquipLevel(CPlayer::EQUIP_BACK);
	}

	static char str[32];
	static DWORD dwFACE=MAKEFOURCC('F','A','C','E');
	static DWORD dwHAIR=MAKEFOURCC('H','A','I','R');
	static DWORD dwHELM=MAKEFOURCC('H','E','L','M');
	static DWORD dwARMO=MAKEFOURCC('A','R','M','O');
	static DWORD dwPIFE=MAKEFOURCC('P','I','F','E');//����
	static DWORD dwHAND=MAKEFOURCC('H','A','N','D');
	static DWORD dwFOOT=MAKEFOURCC('F','O','O','T');
	static DWORD dwBACK=MAKEFOURCC('B','A','C','K');
	static DWORD dwSCAB=MAKEFOURCC('S','C','A','B');
	static DWORD dwSPMS=MAKEFOURCC('S','P','M','S');

	// ��ʾ�򵥲���
	m_pDisplayModel->ShowGroup(dwSPMS,0,0);

	// ��ʾ��������
	DWORD dwName;
	sprintf(str, "%04d", GetFacePic());
	dwName = MAKEFOURCC(str[0], str[1], str[2], str[3]);
	m_pDisplayModel->ShowGroup(dwFACE, dwName, 0);

	// ��ʾ���Ͳ���
	sprintf(str, "%04d", GetHairStyle());
	dwName = MAKEFOURCC(str[0], str[1], str[2], str[3]);
	m_pDisplayModel->ShowGroup(dwHAIR, dwName, 0);

	// ��ʾ��ɫ(�ȼ�)
	m_pDisplayModel->ShowGroup(dwHAIR, dwName, GetHairColor());

	// ͷ��
	//sprintf(str, "%04d", dwHelm);
	//if(GetShowFashion())
	//{
	//	m_pDisplayModel->ShowGroup(dwHELM,MAKEFOURCC(str[0],str[1],str[2],str[3]),lHelmLevel);
	//}
	//else
	//	m_pDisplayModel->HideGroup(dwHELM);

	// ����
	sprintf(str, "%04d", dwBody);
	m_pDisplayModel->ShowGroup(dwARMO,MAKEFOURCC(str[0],str[1],str[2],str[3]),lBodyLevel);

	// ����
	if(dwPifeng != 0)
	{
		sprintf(str, "%04d", dwPifeng);
		m_pDisplayModel->ShowGroup(dwPIFE,MAKEFOURCC(str[0],str[1],str[2],str[3]),dwPifengLevel);
	}else
		m_pDisplayModel->HideGroup(dwPIFE);

	// ����
	sprintf(str, "%04d", dwGlove);
	m_pDisplayModel->ShowGroup(dwHAND,MAKEFOURCC(str[0],str[1],str[2],str[3]),lGloveLevel);

	// ѥ��
	sprintf(str, "%04d", dwShoes);
	m_pDisplayModel->ShowGroup(dwFOOT,MAKEFOURCC(str[0],str[1],str[2],str[3]),lShoesLevel);
	//��������

	//��������
	//���
	UpdateSpecialEquipment();

	// ������Ʒ
	UpdateAccouterment();
}

// ��Player�Ƿ���Ҫ��ʾ
bool CPlayer::IsNeedDisplay()
{
	if( GetCityWarDiedState() == true )	return false;
	return CMoveShape::IsNeedDisplay();
}
void CPlayer::UpdateSpecialEquipment()
{
	DWORD dwLocator = 0;

	//CClientRegion *pClientRegion = GetGame()->GetRegion();

	/*if (pClientRegion)
	{*/
	//GameMap *pGameMap = pClientRegion->GetGameMap();
	//���
	if (m_Wing.IsVisible())
	{
		dwLocator= CShape::LOCATOR_BACK;
		m_Wing.SetLocator(dwLocator);
		//m_Wing->Render(pCamera,pGameMap,!pPlayer->GetIsConceal());
		//D3DXMatrixTranslation(&matWorld,vLocator.x,vLocator.y,vLocator.z);


	}
	dwLocator = 0;
	//ͷ��
	if (m_HeadWear.IsVisible())
	{
		dwLocator= CShape::LOCATOR_HEAD;
		m_HeadWear.SetLocator(dwLocator);
		//m_Wing->Render(pCamera,pGameMap,!pPlayer->GetIsConceal());
		//D3DXMatrixTranslation(&matWorld,vLocator.x,vLocator.y,vLocator.z);
	}
	dwLocator = 0;
	//��������
	if (m_RightHandWeapon.IsVisible())
	{
		if (this->GetState() == CPlayer::STATE_PEACE
			||this->IsRide()
			||(this->GetAction() >= ACT_FACEACTION && this->GetAction() <= ACT_FACEACTIONMAX)
			||(this->GetAction() >= ACT_MINE && this->GetAction() <= ACT_MINEMAX)
			)

		{
			switch(m_RightHandWeapon.GetWeaponType())
			{
			case CWeapon::WEAPONSHOW_SINGLESWORD:
			case CWeapon::WEAPONSHOW_SINGLEHAMMER:
			case CWeapon::WEAPONSHOW_SINGLEKNIFE:
				dwLocator= CShape::LOCATOR_LEFTWAIST;
				break;
			case CWeapon::WEAPONSHOW_BIGWANG:
			case CWeapon::WEAPONSHOW_BIGAX:
				dwLocator= CShape::LOCATOR_BACKSTAFF;
				break;
			case CWeapon::WEAPONSHOW_BIGSWORD:
				dwLocator= CShape::LOCATOR_BIGSWORD;
				break;
			case CWeapon::WEAPONSHOW_BOXINGRIGHT:
				dwLocator= CShape::LOCATOR_RIGHTBOXING;
				break;
			case CWeapon::WEAPONSHOW_DOUBLEWEAPON:
				dwLocator= CShape::LOCATOR_RIGHTBACK;
				break;
			case CWeapon::WEAPONSHOW_BOW:
				dwLocator= CShape::LOCATOR_BACKBOW;
				break;

			}


		}
		else if (GetState() == CPlayer::STATE_FIGHT)
		{
			switch(m_RightHandWeapon.GetWeaponType())
			{
			case CWeapon::WEAPONSHOW_SINGLESWORD:
			case CWeapon::WEAPONSHOW_SINGLEHAMMER:
			case CWeapon::WEAPONSHOW_SINGLEKNIFE:
				dwLocator= CShape::LOCATOR_RIGHTHAND;
				break;
			case CWeapon::WEAPONSHOW_BIGWANG:
			case CWeapon::WEAPONSHOW_BIGAX:
				dwLocator= CShape::LOCATOR_RIGHTHAND;
				break;
			case CWeapon::WEAPONSHOW_BIGSWORD:
				dwLocator= CShape::LOCATOR_RIGHTHAND;
				break;

			case CWeapon::WEAPONSHOW_DOUBLEWEAPON:
				dwLocator= CShape::LOCATOR_RIGHTHAND;
				break;
			case CWeapon::WEAPONSHOW_BOXINGRIGHT:
				dwLocator= CShape::LOCATOR_RIGHTBOXING;
				break;

			case CWeapon::WEAPONSHOW_BOW:
				dwLocator= CShape::LOCATOR_LEFTHAND;
				break;
			}

		}
		if (dwLocator != 0)
		{
			m_RightHandWeapon.SetLocator(dwLocator);
			m_REnergy.SetLocator(dwLocator);
		}

		//m_RightHandWeapon->Render(pCamera,pGameMap,!pPlayer->GetIsConceal());
		//D3DXMatrixTranslation(&matWorld,vLocator.x,vLocator.y,vLocator.z);


	}
	dwLocator = 0;
	//��������
	if (m_LeftHandWeapon.IsVisible())
	{


		if (GetState() == CPlayer::STATE_PEACE
			||this->IsRide()
			||(this->GetAction() >= ACT_FACEACTION && this->GetAction() <= ACT_FACEACTIONMAX)
			||(this->GetAction() >= ACT_MINE && this->GetAction() <= ACT_MINEMAX)
			)

		{
			switch(m_LeftHandWeapon.GetWeaponType())
			{
			case CWeapon::WEAPONSHOW_SINGLESWORD:
			case CWeapon::WEAPONSHOW_SINGLEHAMMER:
			case CWeapon::WEAPONSHOW_SINGLEKNIFE:
				dwLocator= CShape::LOCATOR_RIGHTWAIST;
				break;
			case CWeapon::WEAPONSHOW_BOXINGLEFT:
				dwLocator= CShape::LOCATOR_LEFTBOXING;
				break;
			case CWeapon::WEAPONSHOW_DOUBLEWEAPON:
				dwLocator= CShape::LOCATOR_LEFTBACK;
				break;
			case CWeapon::WEAPONSHOW_SHIELD:
				dwLocator= CShape::LOCATOR_LEFTARM;
				break;


			}

		}
		else if (GetState() == CPlayer::STATE_FIGHT)
		{
			switch(m_LeftHandWeapon.GetWeaponType())
			{
			case CWeapon::WEAPONSHOW_SINGLESWORD:
			case CWeapon::WEAPONSHOW_SINGLEHAMMER:
			case CWeapon::WEAPONSHOW_SINGLEKNIFE:
				dwLocator= CShape::LOCATOR_LEFTHAND;
				break;
			case CWeapon::WEAPONSHOW_BOXINGLEFT:
				dwLocator= CShape::LOCATOR_LEFTBOXING;
				break;
			case CWeapon::WEAPONSHOW_DOUBLEWEAPON:
				dwLocator= CShape::LOCATOR_LEFTHAND;
				break;
			case CWeapon::WEAPONSHOW_SHIELD:
				dwLocator= CShape::LOCATOR_LEFTARM;
				break;


			}

		}
		if (dwLocator != 0)
		{
			m_LeftHandWeapon.SetLocator(dwLocator);
			m_LEnergy.SetLocator(dwLocator);
		}

		//m_LeftHandWeapon->Render(pCamera,pGameMap,!pPlayer->GetIsConceal());
		//D3DXMatrixTranslation(&matWorld,vLocator.x,vLocator.y,vLocator.z);
	}
	//	}
}
void CPlayer::RendShadow(bool bsimpleshadow)
{
	if (m_pDisplayModel->GetLoadState() != Load_Did)
	{
		return;
	}
	CTextureProjective::SetSimpleShadow(bsimpleshadow);
	D3DXVECTOR3 pos;
	pos.x = m_pAnimInfo->GetWorldMatrix()->_41;
	pos.y = GetHeight();
	pos.z = m_pAnimInfo->GetWorldMatrix()->_43;
	render::BoundingBox * pboundbox = m_pDisplayModel->GetSizeBox();
	float wid = (max(pboundbox->GetWidth(),pboundbox->GetLength()))/2.0f;
	float hei = pboundbox->GetHeight();
	wid *= m_pAnimInfo->GetScalMatrix()->_11;
	hei *= m_pAnimInfo->GetScalMatrix()->_22;
	CTextureProjective::BeginRendShadow(pos,(int)max(wid,hei * 2));
	CTextureProjective::AddRendShadow(m_pDisplayModel,m_pAnimInfo);
	if (m_busesimplemodel)
	{
		if (m_RightHandWeapon.GetSimpleModel()&&m_RightHandWeapon.IsVisible())
		{
			if (m_RightHandWeapon.GetSimpleModel()->GetLoadState() == Load_Did)
			{
				//m_RightHandWeapon.GetSimpleModel()->ProcessAnimJoint(m_RightHandWeapon.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_RightHandWeapon.GetSimpleModel(),m_RightHandWeapon.GetAnimInfo());
			}

		}

		if (m_LeftHandWeapon.GetSimpleModel()&&m_LeftHandWeapon.IsVisible())
		{
			if (m_LeftHandWeapon.GetSimpleModel()->GetLoadState() == Load_Did)
			{
				//m_LeftHandWeapon.GetSimpleModel()->ProcessAnimJoint(m_LeftHandWeapon.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_LeftHandWeapon.GetSimpleModel(),m_LeftHandWeapon.GetAnimInfo());
			}

		}

		if (m_Wing.GetDisplayModel()&&m_Wing.IsVisible())
		{
			if (m_Wing.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_Wing.GetDisplayModel()->ProcessAnimJoint(m_Wing.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_Wing.GetDisplayModel(),m_Wing.GetAnimInfo());
			}

		}
		if (m_Horse.GetDisplayModel()&&m_Horse.IsVisible())
		{
			if (m_Horse.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_Horse.GetDisplayModel()->ProcessAnimJoint(m_Horse.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_Horse.GetDisplayModel(),m_Horse.GetAnimInfo());
			}

		}
	}
	else
	{
		if (m_RightHandWeapon.GetDisplayModel()&&m_RightHandWeapon.IsVisible())
		{
			if (m_RightHandWeapon.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_RightHandWeapon.GetDisplayModel()->ProcessAnimJoint(m_RightHandWeapon.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_RightHandWeapon.GetDisplayModel(),m_RightHandWeapon.GetAnimInfo());
			}

		}

		if (m_LeftHandWeapon.GetDisplayModel()&&m_LeftHandWeapon.IsVisible())
		{
			if (m_LeftHandWeapon.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_LeftHandWeapon.GetDisplayModel()->ProcessAnimJoint(m_LeftHandWeapon.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_LeftHandWeapon.GetDisplayModel(),m_LeftHandWeapon.GetAnimInfo());
			}

		}

		if (m_Wing.GetDisplayModel()&&m_Wing.IsVisible())
		{
			if (m_Wing.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_Wing.GetDisplayModel()->ProcessAnimJoint(m_Wing.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_Wing.GetDisplayModel(),m_Wing.GetAnimInfo());
			}

		}
		if (m_HeadWear.GetDisplayModel()&&m_HeadWear.IsVisible())
		{
			if (m_HeadWear.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_HeadWear.GetDisplayModel()->ProcessAnimJoint(m_HeadWear.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_HeadWear.GetDisplayModel(),m_HeadWear.GetAnimInfo());
			}

		}
		if (m_Horse.GetDisplayModel()&&m_Horse.IsVisible())
		{
			if (m_Horse.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_Horse.GetDisplayModel()->ProcessAnimJoint(m_Horse.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_Horse.GetDisplayModel(),m_Horse.GetAnimInfo());
			}

		}
		if (m_CollectTool.GetDisplayModel()&&m_CollectTool.IsVisible())
		{
			if (m_CollectTool.GetDisplayModel()->GetLoadState() == Load_Did)
			{
				//m_CollectTool.GetDisplayModel()->ProcessAnimJoint(m_CollectTool.GetAnimInfo());
				CTextureProjective::AddRendShadow(m_CollectTool.GetDisplayModel(),m_CollectTool.GetAnimInfo());
			}

		}
	}



	CTextureProjective::EndRendShadow();
}
void CPlayer::PlayEnergyEffect(DWORD energylevel)
{
	m_bPlayEnergyEffect = TRUE;
	m_bEnergyTimeStarted = FALSE;
	m_LEnergy.GetAnimInfo()->m_tActionInfo.bCurActionLooped = 1;
	m_LEnergy.GetAnimInfo()->m_tActionInfo.dwCurActionElapseTime = 0;
	m_LEnergy.GetAnimInfo()->m_tActionInfo.dwCurActionLoopCount = 0;
	m_REnergy.GetAnimInfo()->m_tActionInfo.bCurActionLooped = 1;
	m_REnergy.GetAnimInfo()->m_tActionInfo.dwCurActionElapseTime = 0;
	m_REnergy.GetAnimInfo()->m_tActionInfo.dwCurActionLoopCount = 0;
	UpdateEnergyEffect(energylevel);
}
void CPlayer::EndEnergyEffect()
{
	if (m_REnergy.GetAnimInfo()->m_tActionInfo.dwCurActionLoopCount >= 1 
		|| m_LEnergy.GetAnimInfo()->m_tActionInfo.dwCurActionLoopCount >= 1)
	{
		m_bPlayEnergyEffect = FALSE;
		m_bEnergyTimeStarted = FALSE;
		SetEnergyHitEffectID(0);
	}

}
void CPlayer::UpdateEnergyEffect(DWORD energylevle)
{
	DWORD type = 0;
	DWORD index = 0;
	DWORD level = 0;
	DWORD modelid = 0;
	if (energylevle%2 == 0)
	{
		modelid = 20;
	}
	else
	{
		modelid = 10;
	} 
	DWORD hiteffectID = 0; 
	if (m_bSelf)
	{
		index = GetEquip(CPlayer::EQUIP_WEAPON)->GetIndex();
	}
	else
	{
		index = GetEquipIndex(CPlayer::EQUIP_WEAPON);
	}
	level = CGoodsList::GetEffectVal(index,GAP_ROLE_LEVEL_LIMIT,1);
	if (level >= 0 && level <= 39)
	{
		level = 0;
	}
	else if(level >= 40 && level <= 79)
	{
		level = 1;
	}
	else if(level >= 80 && level <= 119)
	{
		level = 2;
	}
	else if(level >= 120 && level <= 159)
	{
		level = 3;
	}
	else if(level >= 160 && level <= 200)
	{
		level = 4;
	}
	CGoodsList::tagEffect* ptegeffect = CGoodsList::GetEffectTag(index,GAP_WEAPON_CATEGORY);
	type = ptegeffect->dwValue1;

	switch(type)
	{
	case WT_SINGLE_SWORD:
	case WT_SINGLE_HAMMER:
	case WT_SINGLE_KNIFE:
		m_REnergy.SetModelID(60001 + modelid);
		m_REnergy.SetVisible(true);
		m_LEnergy.SetVisible(false);
		break;
	case WT_WAND://����
	case WT_HEAVY_SWORD://˫�ֽ�
	case WT_HEAVY_AX://˫�ָ�:
		m_REnergy.SetModelID(60002 + modelid);
		m_REnergy.SetVisible(true);
		m_LEnergy.SetVisible(false);
		break;
	case WT_BOW:
		m_LEnergy.SetModelID(60004 + modelid);
		m_LEnergy.SetVisible(true);
		m_REnergy.SetVisible(false);
		break;
	case WT_DOUBLE_SWORD:
		m_LEnergy.SetModelID(60003 + modelid);
		m_REnergy.SetModelID(60003 + modelid);
		m_LEnergy.SetVisible(true);
		m_REnergy.SetVisible(true);
		break;
	case WT_FIST:
		m_LEnergy.SetModelID(60005 + modelid);
		m_REnergy.SetModelID(61005 + modelid);
		m_LEnergy.SetVisible(true);
		m_REnergy.SetVisible(true);
		break;
	}
	hiteffectID = 61000 + modelid + level + 1;
	SetEnergyHitEffectID(hiteffectID);
	m_LEnergy.SetDisplayModel();
	m_REnergy.SetDisplayModel();
	m_LEnergy.SetLevel(level);
	m_REnergy.SetLevel(level);

}



/*
* ����: �趨���׵Ŀɼ���
* ժҪ: -
* ����: bIsVisible - �Ƿ�ɼ�
* ����ֵ: -
* ����: lpf
* ��������: 2009.03.05
* �޸���־:
*	2009.04.10- lpf
* 		������������ʱ,����Ҳ�ᱻ����.�������˲���ʾ����ʱ����������ʾ;
*/
void CPlayer::SetGreatcoatVisible(BOOL bIsVisible)
{
	static DWORD dwGreatcoatGroup = MAKEFOURCC('B','O','D','Y');
	static DWORD dwHairGroup	  = MAKEFOURCC('H','A','I','R');
	static DWORD dwArmoGroup	  = MAKEFOURCC('A','R','M','O');
	static DWORD dwHandGroup	  = MAKEFOURCC('H','A','N','D');
	static DWORD dwFootGroup	  = MAKEFOURCC('F','O','O','T');
	static DWORD dwFaceGroup	  = MAKEFOURCC('F','A','C','E');
	static DWORD dwPIFEGroup	  = MAKEFOURCC('P','I','F','E');		//����
	static char str[32];

	if (m_pDisplayModel == NULL)
		return;

	if (bIsVisible)
	{
		if (m_bSelf)
		{
			CGoods * pGoods = GetEquip(CPlayer::EQUIP_P_FROCK);
			if (pGoods != NULL)
			{
				DWORD dwGreatcoatPart  = pGoods->GetProperty()->dwEquipID;
				DWORD dwGreatcoatLevel = pGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1);

				if (pGoods->GetEffectVal(GAP_HIDE_HEAR, 1) != 0)
					m_pDisplayModel->HideGroup(dwHairGroup);
				if (pGoods->GetEffectVal(GAP_HIDE_FACE, 1) != 0)
					m_pDisplayModel->HideGroup(dwFaceGroup);
				if (pGoods->GetEffectVal(GAP_HIDE_BODY, 1) != 0)
				{
					m_pDisplayModel->HideGroup(dwArmoGroup);
					m_pDisplayModel->HideGroup(dwHandGroup);
					m_pDisplayModel->HideGroup(dwFootGroup);
					m_pDisplayModel->HideGroup(dwPIFEGroup);
				}

				if (dwGreatcoatPart != 0)
				{
					sprintf(str, "%04d", dwGreatcoatPart);
					m_pDisplayModel->ShowGroup(dwGreatcoatGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwGreatcoatLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwGreatcoatGroup);
		}else
		{
			DWORD dwEquipIndex = GetEquipIndex(CPlayer::EQUIP_P_FROCK);
			CGoodsList::tagGoods2 * pGoods = CGoodsList::GetProperty(dwEquipIndex);
			if (pGoods != NULL)
			{				
				DWORD dwGreatcoatPart  = pGoods->BaseProperty.dwEquipID;
				DWORD dwGreatcoatLevel = GetEquipLevel(CPlayer::EQUIP_P_FROCK);

				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_HEAR, 1) != 0)
					m_pDisplayModel->HideGroup(dwHairGroup);
				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_FACE, 1) != 0)
					m_pDisplayModel->HideGroup(dwFaceGroup);
				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_BODY, 1) != 0)
				{
					m_pDisplayModel->HideGroup(dwArmoGroup);
					m_pDisplayModel->HideGroup(dwHandGroup);
					m_pDisplayModel->HideGroup(dwFootGroup);
					m_pDisplayModel->HideGroup(dwPIFEGroup);
				}

				if (dwGreatcoatPart != 0)
				{
					sprintf(str, "%04d", dwGreatcoatPart);
					m_pDisplayModel->ShowGroup(dwGreatcoatGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwGreatcoatLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwGreatcoatGroup);
		}
	}else
	{
		DWORD dwArmo, dwGlove, dwShoes, dwPifeng; 
		DWORD dwArmoLevel, lGloveLevel, lShoesLevel, dwPifengLevel;
		if (m_bSelf)
		{
			// ����
			CGoods * pGoods = GetEquip(CPlayer::EQUIP_BODY);
			dwArmo = pGoods ? pGoods->GetProperty()->dwEquipID : 1;
			dwArmoLevel = pGoods ? pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1) : 0;

			// ����
			pGoods = GetEquip(CPlayer::EQUIP_HAND);
			dwGlove = pGoods ? pGoods->GetProperty()->dwEquipID : 1;
			lGloveLevel = pGoods ? pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1) : 0;

			// ѥ��
			pGoods = GetEquip(CPlayer::EQUIP_BOOT);
			dwShoes = pGoods ? pGoods->GetProperty()->dwEquipID : 1;
			lShoesLevel = pGoods ? pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1) : 0;

			// ����
			pGoods = GetEquip(CPlayer::EQUIP_P_FROCK);
			dwPifeng = pGoods ? pGoods->GetProperty()->dwEquipID : 1;
			dwPifengLevel = pGoods ? pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1) : 0;
		}else
		{
			// ����
			CGoodsList::tagGoods2 * pGoods = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_BODY));
			dwArmo = pGoods ? pGoods->BaseProperty.dwEquipID : 1;
			dwArmoLevel = GetEquipLevel(CPlayer::EQUIP_BODY);

			// ����
			pGoods = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_HAND));
			dwGlove = pGoods ? pGoods->BaseProperty.dwEquipID : 1;
			lGloveLevel = GetEquipLevel(CPlayer::EQUIP_HAND);

			// ѥ��
			pGoods = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_BOOT));
			dwShoes = pGoods ? pGoods->BaseProperty.dwEquipID : 1;
			lShoesLevel = GetEquipLevel(CPlayer::EQUIP_BOOT);

			// ����
			pGoods = CGoodsList::GetProperty(GetEquipIndex(CPlayer::EQUIP_P_FROCK));
			dwPifeng = pGoods ? pGoods->BaseProperty.dwEquipID : 1;
			dwPifengLevel = GetEquipLevel(CPlayer::EQUIP_P_FROCK);
		}

		// ��ʾ��������
		sprintf(str, "%04d", GetFacePic());
		DWORD dwPart = MAKEFOURCC(str[0], str[1], str[2], str[3]);
		m_pDisplayModel->ShowGroup(dwFaceGroup, dwPart, 0);

		// ��ʾ���Ͳ���
		sprintf(str, "%04d", GetHairStyle());
		dwPart = MAKEFOURCC(str[0], str[1], str[2], str[3]);

		// ��ʾ��ɫ(�ȼ�)
		m_pDisplayModel->ShowGroup(dwHairGroup, dwPart, GetHairColor());

		sprintf(str, "%04d", dwArmo);
		m_pDisplayModel->ShowGroup(dwArmoGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwArmoLevel);
		sprintf(str, "%04d", dwGlove);
		m_pDisplayModel->ShowGroup(dwHandGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), lGloveLevel);
		sprintf(str, "%04d", dwShoes);
		m_pDisplayModel->ShowGroup(dwFootGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), lShoesLevel);

		if (GetShowFashion() & FASHION_TYPE_MANTEAU)
		{
			sprintf(str, "%04d", dwPifeng);
			m_pDisplayModel->ShowGroup(dwPIFEGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwPifengLevel);
		}

		m_pDisplayModel->HideGroup(dwGreatcoatGroup);
	}
}



/*
* ����: �趨����Ŀɼ���
* ժҪ: -
* ����: bIsVisible - �Ƿ�ɼ�
* ����ֵ: -
* ����: lpf
* ��������: 2009.03.03
* �޸���־:
*/
void CPlayer::SetManteauVisible(BOOL bIsVisible)
{
	DWORD dwManteauGroup = MAKEFOURCC('P','I','F','E');

	if (bIsVisible)
	{
		static char str[32];
		if (m_bSelf)
		{
			CGoods * pGoods = GetEquip(CPlayer::EQUIP_P_FROCK);
			if (pGoods != NULL)
			{
				DWORD dwManteauPart  = pGoods->GetProperty()->dwEquipID;
				DWORD dwManteauLevel = pGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1);
				if (dwManteauPart != 0)
				{
					sprintf(str, "%04d", dwManteauPart);
					m_pDisplayModel->ShowGroup(dwManteauGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwManteauLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwManteauGroup);
		}else
		{
			DWORD dwEquipIndex = GetEquipIndex(CPlayer::EQUIP_P_FROCK);
			CGoodsList::tagGoods2 * pGoods = CGoodsList::GetProperty(dwEquipIndex);
			if (pGoods != NULL)
			{
				DWORD dwManteauPart  = pGoods->BaseProperty.dwEquipID;
				DWORD dwManteauLevel = GetEquipLevel(CPlayer::EQUIP_P_FROCK);
				if (dwManteauPart != 0)
				{
					sprintf(str, "%04d", dwManteauPart);
					m_pDisplayModel->ShowGroup(dwManteauGroup, MAKEFOURCC(str[0],str[1],str[2],str[3]), dwManteauLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwManteauGroup);
		}
	}else
		m_pDisplayModel->HideGroup(dwManteauGroup);
}



/*
* ����: ��ҵ���ʾ
* ժҪ: �ú���ִ�������ʾ,�������˹���Ч��
* ����: -
* ����ֵ: ִ�гɹ�����ture,���򷵻�false
* ����: -
* ��������: -
* �޸���־:
*	2008.01.12 - lpf
*		�ڸú����������˶�����ģ�͸���������Ч����Ⱦ
*	2008.01.16 - lpf
*		�ڸú����������˶�����ģ���쳣״̬��Ч����Ⱦ
*	2008.07.28 - lpf
*		�޸�����������״̬ʱ,Locatorλ��,�����ǴӺ󱳿�ʼ
*	2008.10.17 - lpf
*		�޸���������ʾ�ĵ���
*	2008.10.23 - lpf
*		�޸���������ʾ�ĵ���
*	2009.04.24 - lpf
*		������ʾʱ,�������ָ�������dynamic_cast��ʽ,�������ת��;
*/
bool CPlayer::Display(void)
{
	if (m_pDisplayModel)
	{
		m_pDisplayModel->SetRendSimpleMesh(m_busesimplemodel);
		m_Horse.SetSimpleMesh(m_busesimplemodel);
		m_Wing.SetSimpleMesh(m_busesimplemodel);
		m_HeadWear.SetSimpleMesh(m_busesimplemodel);
		m_LeftHandWeapon.SetSimpleMesh(m_busesimplemodel);
		m_RightHandWeapon.SetSimpleMesh(m_busesimplemodel);
	}
	if (IsRide())
	{
		if (m_Horse.GetDisplayModel() && GetDisplayModel() &&
			m_Horse.GetDisplayModel()->GetLoadState() == Load_Did && GetDisplayModel()->GetLoadState() == Load_Did)
		{
			//m_Horse.Display(GetPosX() + m_fXIncrement, GetPosY() + m_fYIncrement, GetHeight(), GetNowDir());
			m_Horse.Display(dynamic_cast<CMoveShape *>(this));
			if( !CMoveShape::DisplayByLocator(m_Horse.GetDisplayModel(),m_Horse.GetAnimInfo(),CShape::LOCATOR_BACK))
			{
				return false;
			}
		}
	}
	else
	{
		m_Horse.SetVisible(false);
		if( !CMoveShape::Display() )
			return false;
	}

	// ����ģ�͸���Ч������ʱ����������Ч��
	if (GetGame()->GetSetup()->bRoleFlash && m_listAddEffect[ADDEFFECT_FLASH].bPlayState)			//����Ч��
	{
		if (timeGetTime() - m_listAddEffect[ADDEFFECT_FLASH].dwStartTime >= m_listAddEffect[ADDEFFECT_FLASH].dwTimeLength)
			m_listAddEffect[ADDEFFECT_FLASH].bPlayState = false;

		DWORD dwColor = 0xff000000 |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 16) |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 8) |
			m_listAddEffect[ADDEFFECT_FLASH].dwValue;

		// ������ģ����ɫ
		m_RightHandWeapon.DisplayForFlashColor(m_pDisplayModel, m_pAnimInfo, dwColor);
		m_LeftHandWeapon.DisplayForFlashColor(m_pDisplayModel, m_pAnimInfo, dwColor);
		m_Wing.DisplayForFlashColor(m_pDisplayModel, m_pAnimInfo, dwColor);
		m_HeadWear.DisplayForFlashColor(m_pDisplayModel, m_pAnimInfo, dwColor);
		m_CollectTool.DisplayForFlashColor(m_pDisplayModel, m_pAnimInfo, dwColor);
	}else if (m_bStateColorPlay)				//�����쳣״̬��������ɫЧ��
	{
		//��ʾ������ģ��
		m_RightHandWeapon.DisplayForExStateColor(m_pDisplayModel, m_pAnimInfo, m_dwCurrentModelCol);
		m_LeftHandWeapon.DisplayForExStateColor(m_pDisplayModel, m_pAnimInfo, m_dwCurrentModelCol);	
		m_Wing.DisplayForExStateColor(m_pDisplayModel, m_pAnimInfo, m_dwCurrentModelCol);
		m_HeadWear.DisplayForExStateColor(m_pDisplayModel, m_pAnimInfo, m_dwCurrentModelCol);
		m_CollectTool.DisplayForExStateColor(m_pDisplayModel, m_pAnimInfo, m_dwCurrentModelCol);
	}else
	{
		//��ʾ������ģ��
		m_RightHandWeapon.Display(dynamic_cast<CShape *>(this));
		m_LeftHandWeapon.Display(dynamic_cast<CShape *>(this));	
		m_Wing.Display(dynamic_cast<CShape *>(this));
		m_HeadWear.Display(dynamic_cast<CShape *>(this));
		m_CollectTool.Display(dynamic_cast<CShape *>(this));
		if (m_bPlayEnergyEffect == TRUE)
		{
			if (!m_bEnergyTimeStarted)
			{
				m_dwEnergyStartTime = timeGetTime();
				m_bEnergyTimeStarted = true;
			}
			if (timeGetTime() - m_dwEnergyStartTime > 500)
			{
				m_LEnergy.Display(dynamic_cast<CShape *>(this));
				m_REnergy.Display(dynamic_cast<CShape *>(this));
			}

			/*	m_LEnergy.Display(m_pDisplayModel, m_pAnimInfo);
			m_REnergy.Display(m_pDisplayModel, m_pAnimInfo);*/
			if (timeGetTime() - m_dwEnergyStartTime > 1300)
			{
				EndEnergyEffect();
			}



		}
	}
	return true;
}



//��ʾս��״̬�����ͼ��
void  CPlayer::DisplayWarBitmap()
{
	D3DXVECTOR3 pos(GetPosY(),GetHeight()+2,GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	long x = (long)pos2.x;
	long y = (long)pos2.y-2;

	//����ǵж��л���ң���ʾ�ж�״̬
	if(GetFactionID() != CGUID::GUID_INVALID)
	{
		if( GetGame()->GetRegion()->GetWarType() == RWT_VillageWar )	//����Ǵ�ս��ͼ
		{
			if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				x = x - GetGame()->GetFactionWarUnionBitmap()->GetWidth()/2;
				GetGame()->GetFactionWarUnionBitmap()->Draw(x,y);
			}
			else
			{
				x = x - GetGame()->GetFactionWarEnemyBitmap()->GetWidth()/2;
				GetGame()->GetFactionWarEnemyBitmap()->Draw(x,y);
			}
		}
		else if( GetGame()->GetMainPlayer()->IsHaveCityWarEnemyFactions() )
		{
			if( GetGame()->GetMainPlayer()->IsCityWarEnemyFaction(GetFactionID()) == true )
			{
				GetGame()->GetCityWarEnemyBitmap()->Draw(x-GetGame()->GetCityWarEnemyBitmap()->GetWidth()-2,y);
			}
			else if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				GetGame()->GetCityWarUnionBitmap()->Draw(x-GetGame()->GetCityWarUnionBitmap()->GetWidth()-2,y);
			}
		}
		else if( GetGame()->GetMainPlayer()->IsHaveEnemyFactions() )
		{
			if( GetGame()->GetMainPlayer()->IsEnemyFaction(GetFactionID()) == true)
			{
				x = x - GetGame()->GetFactionWarEnemyBitmap()->GetWidth()/2;
				GetGame()->GetFactionWarEnemyBitmap()->Draw(x,y);
			}
			else if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				x = x - GetGame()->GetFactionWarUnionBitmap()->GetWidth()/2;
				GetGame()->GetFactionWarUnionBitmap()->Draw(x,y);
			}
		}
	}
}



/*
* ����: ��ʾ���ͷ����Ϣ
* ժҪ: -
* ����: str		- ��Ҫ��ʾ��������Ϣ
*		 color		- ������ɫ
*		 bkcolor	- ���屳����ɫ
*		 fPos		- ������ʼ�߶�
*		 bkBarColor - ���Ʊ�������ɫ
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.18 - lpf
*		���Ƹ߶ȵ�long y(��ʼ����)�Դ����fPosΪ��׼
*	2008.03.01 - lpf
*		�����µ�������������ͷ����Ϣ����ʾ����,������ѫ�º;�λ��ʾ,���⻹��״̬�������ʾ��Ϣ�Ŀ���
*	2008.06.18 - lpf
*		��ʾ���������ʽ��ԭ�������,��Ϊ��Ӱ
*	2008.07.07 - lpf
*		���ͷ�������и�Ϊ:ͬ��������ʾ��Ϣ������ʾ,����Ӧ�µ�ѡ�������ʾ��ʽ
*   2008.07.10 - lpf
*		�����˶���ͷ�����ֻ��ƵĿ���,��Ҫ��Ϊ�������Ϸ����ҳ��(�Ƿ������������)
*   2008.07.17 - lpf
*		ȡ�����л���Ƶĳ�����Ļ�ж�;
*		�޸���һЩͷ����Ϣ�Ļ���λ��;
*   2008.07.22 - lpf
*		ȡ����������ֻ��Ƹ߶ȳ�����Ļ�·��ж�;
*		�޸��˹�������ѡ����Ҽ�ͷ��ͬ����ʾ����;
*   2008.07.30 - lpf
*		������Ϣ��ɫ�̶�Ϊ��ɫ����Ӱ;
*   2008.08.28 - lpf
*		�������л��Զ����־��ʾ;
*		�޸����л�����ɫ(����ҵ�ǰ������ɫһ��);
*		ȡ�����л�ƺ���ʾ;
*		ͷ����ʾ����Ϣ����������һ����ҵ�ǰѡ��ĳƺž�λ��ʾ;
*   2008.09.16 - lpf
*		�����˹��ұ�־������Ҷ�Ӧ�������������Ƿ���ʾ;
*   2008.10.09 - lpf
*		�����˵��гƺ�,����,����ʱ,ѡ����ʾ����,���ճ�һ��;
*   2008.10.20 - lpf
*		����������ڹ���ر�״̬��,��������ɫ�������κ���ҿ�������ʾΪ��ɫ,������ͨ������°���Ӫ��ʾ;
*   2008.10.22 - lpf
*		���������������ʾ����Ӱ��ɫ��������ɫ����;
*   2008.12.05 - lpf
*		���������ͷ����λ��Ϣ,ͨ����Ϸ���ý���ѡ�����;
*   2008.12.11 - lpf
*		������ͬ�˹��ͷ�ͬ�˹����������ʾ�ж�;
*   2008.12.25 - lpf
*		��"�����м�ͷ��ʾ"λ��,���������ͷ����2������,����ͷ�����ֺͼ�ͷ��Ϣ������;
*   2008.01.04 - lpf
*		�������Լ��������������ʱ,���ֲ��Ǻ���,������������ʾʱ��PKֵ�ж�;
*   2008.01.07 - lpf
*		�ع���2008.12.25���޸�;
*   2009.03.14 - lpf
*		��Ҫ�޸��˹��ڳƺŵ���ɫ��ʾ:
*		������Ӫ�������ͷ����Ϣ��ɫ��������ɫ��ʾ;
*		�����Զ���ƺ���ɫ�����������ɫ��ʾ(ԭ���Ǹ��������ļ���ʾ);
*   2009.11.13 - lpf
*		�����˺�������bIsDisplayHPBar,���ڼ�¼Ѫ���Ƿ���ʾ:
*		��������ʾ����,��PK������,��ͬ��Ӫ�������ǰ����˫������ͼƬ��ʾ(Ŀǰ��˫������);
*/
void CPlayer::DisplayText(const char * str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor, bool bIsDisplayHPBar)
{
	//	static bool bPlayerTitleCheck = true;			//���ͷ��ѫ�º;�λ��Ϣ����
	bool bPlayerGuildInfoCheck = false; //���ͷ���лἰ�л�ƺ���Ϣ����

	D3DXVECTOR3 pos(GetPosY(), GetPickBoxHeight() + 0.2f + GetHeight(), GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	long x			 = (long)pos2.x;
	long y			 = (long)fPos;
	long lFontHeight = GetGame()->GetFontHeight(-1);
//	long lMaxLeft	 = x - 50;
	int	 len = 0;
	long lLineCount		  = 0;
	long lDrawNameLineLen = 0;
	long lDrawNamePos	  = 0;
	bool bPlayerNameCheck = true;			//���ͷ�����ֿ���
	bool bIsMatchingNameColor = false;		//�Ƿ�ƥ��������ɫ(���������Ϣ������ɫ)
	bool bIsPkFlag = false;					//�Ƿ�Ϊpk������־

	//if (bIsDisplayHPBar)
	//	lMaxLeft = x - 50;
	//else
	//	lMaxLeft = x;

	// ��������ʾ���ж�
	if (this == GetGame()->GetMainPlayer())
		bPlayerNameCheck = true;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowMyName);
	else
	{
		CCountryList::tagCountryList * countrySelf	  = CCountryList::GetCountry(GetGame()->GetMainPlayer()->GetCountry());
		CCountryList::tagCountryList * countryAnother = CCountryList::GetCountry(this->GetCountry());

		if (countrySelf && countryAnother)
		{
			// �жϹ����Ƿ����
			if (countrySelf->nLeague == countryAnother->nLeague)
			{
				if (this->GetIsConceal())
					bPlayerNameCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowAlliedPlayerName);
			}else
			{
				bIsMatchingNameColor = true;

				if (this->GetIsConceal())
					bPlayerNameCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowNotAlliedPlayerName);

				// �ж�pk���
				if (GetGame()->GetRegion()->GetPKRegionType() == RPKT_PK && bPlayerNameCheck)
				{
					// �ж��Ƿ����
					if (GetGame()->GetMainPlayer()->GetExID() != GetExID() &&
						GetGame()->GetMainPlayer()->IsTeammate(GetExID()))
						bIsPkFlag = false;
					else
						bIsPkFlag = true;
				}
			}
		}
	}
	if (m_bIsSelected)
		bPlayerNameCheck = true;

	// ����
	bool bIsKing = false;

	CCountryList::tagCountryList * pst = CCountryList::GetCountry(GetCountry());
	CBitmapX * pbmPirate = NULL;
	CBitmapX * pSelectShape = NULL;
	static char strCountry[256];

	// ������ʾ�Ƿ��ǹ���
	// ͨ���л��Ƿ�ӵ������
	// ��ͨ��������Ƿ��ǰ������ж� bugs
	for( list<tagownedReg>::iterator it = m_OwnedRegions.begin(); it!=m_OwnedRegions.end(); ++it)
	{
		if(it->wType == 2) // ���� ��Ȼ��4�����ң���ÿ������ֻ��һ�����ǣ�����������ֻ�ܱ��������л���¡�
		{
			if(GetExID() == m_FacMasterID)
			{
				bIsKing = true;
				break;
			}
		}
	}

	// ������������ʾ��Ϣ���س���
	if (bPlayerNameCheck)
		lDrawNameLineLen += (GetGame()->GetFontWidth(str) + 4);

	if (GetShowCountry())
	{
		if (pst)
			lDrawNameLineLen += 30;
		else
		{
            sprintf(strCountry, AppFrame::GetText("Base_100"));
			lDrawNameLineLen += GetGame()->GetFontWidth(strCountry);
		}
	}

	if (GetPVPValue())
	{
		if( GetPVPValue() < PVP_PIRATE1 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate1.tga");
		}
		else if( GetPVPValue() < PVP_PIRATE2 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate2.tga");
		}
		else if( GetPVPValue() < PVP_PIRATE3 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate3.tga");
		}
		else
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate4.tga");
		}
		if (pbmPirate)
			lDrawNameLineLen += pbmPirate->GetWidth();
	}

	// �����м�ͷ��ʾ
	if (m_bIsSelected)
	{
		long lTmpY;
		pSelectShape = GetGame()->m_pSelectShape;
		DWORD dwW2	 = pSelectShape->GetWidth() / 2;
		DWORD dwH	 = pSelectShape->GetHeight();
		RECT  rc	 = { 0, 0, dwW2, dwH };
		lDrawNamePos = (long)pos2.x - dwW2 - lDrawNameLineLen / 2;		//���������л���X��ʼ����

		if (GetFactionID() != CGUID::GUID_INVALID && bPlayerGuildInfoCheck)
			lTmpY = y - lFontHeight - 3;
		else
			lTmpY = y;

		GetGame()->m_pSelectShape->Draw(lDrawNamePos - 8, lTmpY, rc);

		rc.left  = dwW2;
		rc.right = dwW2 * 2;

		GetGame()->m_pSelectShape->Draw(lDrawNamePos + 8 + dwW2 + lDrawNameLineLen, lTmpY, rc);
		lDrawNamePos += dwW2;
	}else
		lDrawNamePos = (long)pos2.x - lDrawNameLineLen / 2;


	/*	// ��ʾ���ҹٽ�
	D3DCOLOR textColor = D3DCOLOR_ARGB(255,255,255,255);
	D3DCOLOR backTextColor = D3DCOLOR_ARGB(255,255,255,255);
	//int iCountryCredit  = GetBaseProperty()->dwCredit;
	string strOfficialRank = "";
	SetOfficialRank("");
	vector<COffciailRankConfig::tagOfficialRankConfig>::iterator ConfigIter= COffciailRankConfig::GetOfficialRankVec().begin();
	for(;ConfigIter!= COffciailRankConfig::GetOfficialRankVec().end();ConfigIter++)
	{
	if(iCountryCredit >= (*ConfigIter).CreditMinValue && iCountryCredit <= (*ConfigIter).CreditMaxValue)
	{
	if(GetOccupation()==CPlayer::OC_FIGHTER)
	{
	SetOfficialRank((*ConfigIter).strFighterOfficialRank);
	}
	else if(GetOccupation()==CPlayer::OC_HUNTER)
	{
	SetOfficialRank((*ConfigIter).strHunterOfficialRank);
	}
	else if(GetOccupation()==CPlayer::OC_TAOIST)
	{
	SetOfficialRank((*ConfigIter).strDaoshiOfficialRank);
	}
	textColor = D3DCOLOR_ARGB((*ConfigIter).textColorA,(*ConfigIter).textColorR,(*ConfigIter).textColorG,(*ConfigIter).textColorB);
	backTextColor =  D3DCOLOR_ARGB((*ConfigIter).textBkColorA,(*ConfigIter).textBkColorR,(*ConfigIter).textBkColorG,(*ConfigIter).textBkColorB);
	SetOffcialRankTextColor(textColor);
	SetOffcialRankBkTextColor(backTextColor);
	break;
	}
	}

	if(GetOfficialRank()!="")
	{
	strOfficialRank += GetOfficialRank();
	}

	if(bIsKing)
	{
	string strCountry = CCountryList::GetCountry(GetCountry())->strName;
	strCountry = strCountry.substr(2,4);
	strOfficialRank = strCountry + "�� ����";
	}



	y = y - GetGame()->GetFontHeight()-2;

	if(GetFactionID() > 0)
	{
	y = y - GetGame()->GetFontHeight()-2;
	}

	if (x<0)
	x = 0;
	if (y<0)
	y = 0;
	if (x > SCREEN_WIDTH-len * GetGame()->GetFontWidth(-1))
	x = SCREEN_WIDTH-len * GetGame()->GetFontWidth(-1);
	if (y > SCREEN_HEIGHT-63-GetGame()->GetFontHeight())
	y = SCREEN_HEIGHT-63-GetGame()->GetFontHeight();

	if (bIsKing)
	{
	SetOffcialRankTextColor( CGlobeSetup::GetSetup()->colorGoodsDarkGold);
	}

	GetGame()->OutPutText(x, y-1, strOfficialRank.c_str(), len, GetOffcialRankBkTextColor(), 1.f, false);
	GetGame()->OutPutText(x-1, y, strOfficialRank.c_str(), len, GetOffcialRankBkTextColor(), 1.f, false);
	GetGame()->OutPutText(x, y+1, strOfficialRank.c_str(), len, GetOffcialRankBkTextColor(), 1.f, false);
	GetGame()->OutPutText(x+1, y, strOfficialRank.c_str(), len, GetOffcialRankBkTextColor(), 1.f, false);
	GetGame()->OutPutText(x, y, strOfficialRank.c_str(), len, GetOffcialRankTextColor(), 1.f, false);
	*/

	// Խ���ж�
	//////////////////��ʱʹ��/////////////////
	//string strOfficialRank = "";
	//int len = strOfficialRank.length();
	//long x = (long)pos2.x - len * GetGame()->GetFontWidth(-1) / 2;
	//long y = (long)fPos - GetGame()->GetFontWidth(-1);
	//////////////////////////////////////////	

	// ��ʾ�л�
	if(GetFactionID() != CGUID::GUID_INVALID && bPlayerGuildInfoCheck)
	{
		CBitmapX * pPic		  = NULL;
		string strFactionName =  "<";
		strFactionName		  += GetFactionName();
		strFactionName		  += ">";
		//if(GetFactionTitle() != "")
		//{
		//	strFactionName += " ";
		//	strFactionName += GetFactionTitle();
		//}
		len = (int)strFactionName.length();
		x = (long)pos2.x - GetGame()->GetFontWidth(strFactionName.c_str()) / 2;

		if( NULL_GUID != m_FactionID )
		{
			string strFacIconFile = GetGame()->GetFactionManager()->GetFacIconPath( m_FactionID );

			fstream file;
			file.open(strFacIconFile.c_str(), ios::in);
			if (file.is_open())
			{
				file.close();
				x -= (12 + 5);
				pPic = CBitmapX::GetBitmapX(strFacIconFile);
				if( pPic )
				{
					pPic->Draw(x, y, 24, 16);
	/*				if (lMaxLeft > x)
						lMaxLeft = x;*/

					x += (24 + 5);
				}
			}
		}

		//if (x<0)
		//	x = 0;
		//if (y<0)
		//	y = 0;
		//if (x > SCREEN_WIDTH-len * lFontWidth)
		//	x = SCREEN_WIDTH-len * lFontWidth;
		//if (y > SCREEN_HEIGHT-63-lFontHeight)
		//	y = SCREEN_HEIGHT-63-lFontHeight;

		if (bkBarColor)
			GetGame()->DrawSolidQuadRect(x-2, y-2, x + GetGame()->GetFontWidth(strFactionName.c_str()) + 2, y+lFontHeight+2, bkBarColor);

		if( GetGame()->GetRegion()->GetWarType() == RWT_VillageWar )	//����Ǵ�ս��ͼ
		{
			if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				GetGame()->GetCityWarUnionBitmap()->Draw(x-GetGame()->GetCityWarUnionBitmap()->GetWidth()-2,y);
			}
			else
			{
				GetGame()->GetCityWarEnemyBitmap()->Draw(x-GetGame()->GetCityWarEnemyBitmap()->GetWidth()-2,y);
			}
		}
		else if( GetGame()->GetMainPlayer()->IsHaveCityWarEnemyFactions() )
		{
			if( GetGame()->GetMainPlayer()->IsCityWarEnemyFaction(GetFactionID()) == true )
			{
				GetGame()->GetCityWarEnemyBitmap()->Draw(x-GetGame()->GetCityWarEnemyBitmap()->GetWidth()-2,y);
			}
			else if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				GetGame()->GetCityWarUnionBitmap()->Draw(x-GetGame()->GetCityWarUnionBitmap()->GetWidth()-2,y);
			}
		}
		else if( GetGame()->GetMainPlayer()->IsHaveEnemyFactions() )
		{
			if( GetGame()->GetMainPlayer()->IsEnemyFaction(GetFactionID()) == true )
			{
				GetGame()->GetFactionWarEnemyBitmap()->Draw(x-GetGame()->GetFactionWarEnemyBitmap()->GetWidth()-2,y);
			}
			else if(GetGame()->GetMainPlayer()->IsFriendFaction(GetFactionID(),GetUnionID()) == true)
			{
				GetGame()->GetFactionWarUnionBitmap()->Draw(x-GetGame()->GetFactionWarUnionBitmap()->GetWidth()-2,y);
			}
		}

		// ��ʾ�л�����
		if (bIsPkFlag)
		{
			GetGame()->OutPutText(x+1, y+1, strFactionName.c_str(), len, bkcolor);//0xff808080);
			GetGame()->OutPutText(x, y, strFactionName.c_str(), len, 0xffff0000);//0xff00ff00);

			//if (lMaxLeft > x)
			//	lMaxLeft = x;
		}else if (IsSelf() || GetShowCountry() || GetPkValue())
		{
			//GetGame()->OutPutText(x, y-1, strFactionName.c_str(), len, bkcolor);
			//GetGame()->OutPutText(x-1, y, strFactionName.c_str(), len, bkcolor);
			//GetGame()->OutPutText(x, y+1, strFactionName.c_str(), len, bkcolor);
			//GetGame()->OutPutText(x+1, y, strFactionName.c_str(), len, bkcolor);
			//GetGame()->OutPutText(x, y, strFactionName.c_str(), len, color);
			//GetGame()->OutPutText(x, y-1, strFactionName.c_str(), len, 0xff00ff00);
			//GetGame()->OutPutText(x-1, y, strFactionName.c_str(), len, 0xff00ff00);
			//GetGame()->OutPutText(x, y+1, strFactionName.c_str(), len, 0xff00ff00);
			//GetGame()->OutPutText(x+1, y, strFactionName.c_str(), len, 0xff00ff00);
			GetGame()->OutPutText(x+1, y+1, strFactionName.c_str(), len, bkcolor);//0xff808080);
			GetGame()->OutPutText(x, y, strFactionName.c_str(), len, color);//0xff00ff00);
		}else
		{
			GetGame()->OutPutText(x+1, y+1, strFactionName.c_str(), len, CGlobeSetup::GetSetup()->colorNoShowCountryNameBack);
			GetGame()->OutPutText(x, y, strFactionName.c_str(), len, CGlobeSetup::GetSetup()->colorNoShowCountryName);
		}

		WORD wIconType = GetDisplayOwneRegionIcon();
		if(wIconType == 2)
		{
			GetGame()->GetOwnedCityBitmap()->Draw(x-GetGame()->GetOwnedCityBitmap()->GetWidth()-2,y);
		}
		else if(wIconType == 1)
		{
			GetGame()->GetOwnedVillageBitmap()->Draw(x-GetGame()->GetOwnedVillageBitmap()->GetWidth()-2,y);
		}

		y -= (lFontHeight + 3);
		lLineCount++;
	}

	// ��ʾ����
	if (GetShowCountry())
	{
		if (pst)
		{
			GetGame()->GetPicList()->Display(CPicList::PT_COUNTRY_ICON, pst->lIconID, lDrawNamePos, y);
			lDrawNamePos += 30;
		}else
		{
			static D3DCOLOR colorCountry = 0xffffffff, colorCountryBack = 0xff000000;
            sprintf(strCountry, AppFrame::GetText("Base_100"));
			colorCountry	 = 0xffffffff;
			colorCountryBack = 0xff000000;

			len = lstrlen(strCountry);
			x   = x - GetGame()->GetFontWidth(strCountry);
			lDrawNamePos += GetGame()->GetFontWidth(strCountry);

			GetGame()->OutPutText(x + 1, y + 1, strCountry, len, colorCountryBack);
			GetGame()->OutPutText(x, y, strCountry, len, colorCountry);
		}
	}

	// ����������ʾ��Ϣ
	if (bPlayerNameCheck)
	{
		len = lstrlen(str);

		//if(GetFactionID() == CGUID::GUID_INVALID)
		//{
		//	if (y > SCREEN_HEIGHT-63-lFontHeight)
		//		y = SCREEN_HEIGHT-63-lFontHeight;
		//}else
		//{
		//	if (y > SCREEN_HEIGHT-63-lFontHeight*2-2)
		//		y = SCREEN_HEIGHT-63-lFontHeight*2-2;
		//}

		if (bkBarColor)
			GetGame()->DrawSolidQuadRect(lDrawNamePos - 2, y - 2, lDrawNamePos + GetGame()->GetFontWidth(str) + 2, y + lFontHeight + 2, bkBarColor);

		// ��ʾ����
		if (bIsPkFlag)
		{
			GetGame()->OutPutText(lDrawNamePos + 1, y + 1, str, len, bkcolor);
			GetGame()->OutPutText(lDrawNamePos, y, str, len, 0xffff0000);
		}else if (IsSelf() || GetShowCountry() || GetPkValue())
		{
			GetGame()->OutPutText(lDrawNamePos + 1, y + 1, str, len, bkcolor);
			GetGame()->OutPutText(lDrawNamePos, y, str, len, color);
		}else
		{
			color = CGlobeSetup::GetSetup()->colorNoShowCountryName;
			bkcolor = CGlobeSetup::GetSetup()->colorNoShowCountryNameBack;
			GetGame()->OutPutText(lDrawNamePos + 1, y + 1, str, len, bkcolor);
			GetGame()->OutPutText(lDrawNamePos, y, str, len, color);
		}

		//if (lMaxLeft > lDrawNamePos)
		//	lMaxLeft = lDrawNamePos;

		lDrawNamePos += GetGame()->GetFontWidth(str);
		lLineCount++;
	}

	// ���壺���ƺ�����־��ֻҪ����ֵ��Ϊ0
	if (GetPVPValue())
	{
		// ����ͼƬ
		if (pbmPirate)
			pbmPirate->Draw(lDrawNamePos + 3, y);
	}

	if (GetShowCountry() || bPlayerNameCheck || GetPVPValue())
		y -= (lFontHeight + 3);

	// �ƺ���ʾ
	D3DCOLOR dwColor = 0;
	//if (GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowOfficialTitle))
	//{
	//	CGlobeSetup::tagSetup * pSetup = CGlobeSetup::GetSetup();
	//	switch (GetTitleType()/*GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetTitleType()*/)
	//	{
	//	case ePTT_Hide:
	//		break;
	//	case ePTT_Knight:
	//		dwColor = pSetup->colorKnight;
	//		break;
	//	case ePTT_Faction:
	//		dwColor = color/*pSetup->colorFaction*/;
	//		break;
	//	case ePTT_Medal1:
	//	case ePTT_Medal2:
	//	case ePTT_Medal3:
	//		if (bIsMatchingNameColor)
	//			dwColor = color;
	//		else
	//			dwColor = pSetup->colorMedal;
	//		break;
	//	case ePTT_LOVER:
	//		dwColor = 0xffff63c2;
	//		break;
	//	}
	//}

	if (dwColor)
	{
		if (bIsPkFlag)
			dwColor = 0xffff0000;

		string sPlayerTitle = GetPlayerTitle()/*GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetPlayerTitle()*/;
		x = (long)(pos2.x - GetGame()->GetFontWidth(sPlayerTitle.c_str()) / 2);
		GetGame()->OutPutText(x + 1, y + 1, sPlayerTitle.c_str(), (int)sPlayerTitle.length(), bkcolor);
		GetGame()->OutPutText(x, y, sPlayerTitle.c_str(), (int)sPlayerTitle.length(), dwColor);
		lLineCount++;

		//if (lMaxLeft > x)
		//	lMaxLeft = x;
	}

	// Ů�Ա�־
	//if (GetFacePic())
	//{
	//	GetGame()->GetFemaleIcon()->Draw(x - 24, y - 4);
	//}

	// ��ʾ����ͼ��
	//tx = x + 10;
	//ty = y - 16;
	//if( bIsKing )
	//{
	//	GetGame()->GetKingIcon()->Draw(x+10,y-16-GetGame()->GetFontHeight()-2);
	//}

	// ��ʾPK������־
	if (bIsPkFlag)
	{
	//	y = (long)fPos - (lLineCount * lFontHeight) / 2 - GetGame()->m_pPKIcon->GetHeight() / 2 + 16;
	//	GetGame()->m_pPKIcon->Draw(lMaxLeft - GetGame()->m_pPKIcon->GetWidth(), y);
		y = (long)fPos - (lLineCount - 1) * lFontHeight - (GetGame()->m_pPKIcon->GetHeight());
		GetGame()->m_pPKIcon->Draw((long)pos2.x - GetGame()->m_pPKIcon->GetWidth() / 2, y);
	}
}

// ��shape������ʾ������Ϣ�������
void CPlayer::DisplayHideInfo(const char* str, D3DCOLOR color, D3DCOLOR bkcolor, float fPos, D3DCOLOR bkBarColor)
{
	// ���ͷ���лἰ�л�ƺ���Ϣ����
	bool bPlayerGuildInfoCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowFactionName);
	D3DXVECTOR3 pos(GetPosY(), GetPickBoxHeight() + 0.2f + GetHeight(), GetPosX());
	D3DXMATRIX matFinal;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVECTOR3 pos2;
	D3DXVec3TransformCoord(&pos2,&pos,&matFinal);

	long x			 = (long)pos2.x;
	long y			 = (long)fPos;
	long lFontHeight = GetGame()->GetFontHeight(-1);
	int	 len = 0;
	long lDrawNameLineLen = 0;
	long lDrawNamePos	  = 0;
	bool bPlayerNameCheck = true;			//���ͷ�����ֿ���
	bool bIsMatchingNameColor = false;		//�Ƿ�ƥ��������ɫ(���������Ϣ������ɫ)

	// ��������ʾ���ж�
	if (this == GetGame()->GetMainPlayer())
		bPlayerNameCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowMyName);
	else
	{
		CCountryList::tagCountryList * countrySelf	  = CCountryList::GetCountry(GetGame()->GetMainPlayer()->GetCountry());
		CCountryList::tagCountryList * countryAnother = CCountryList::GetCountry(this->GetCountry());

		if (countrySelf && countryAnother)
		{
			if (countrySelf->nLeague == countryAnother->nLeague)
			{
				if (this->GetIsConceal())
					bPlayerNameCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowAlliedPlayerName);
			}
			else
			{
				bIsMatchingNameColor = true;
				if (this->GetIsConceal())
					bPlayerNameCheck = false;//GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_ShowNotAlliedPlayerName);
			}
		}
	}
	if (m_bIsSelected)
		bPlayerNameCheck = true;

	// ����
	bool bIsKing = false;

	CCountryList::tagCountryList * pst = CCountryList::GetCountry(GetCountry());
	CBitmapX * pbmPirate = NULL;
	CBitmapX * pSelectShape = NULL;
	static char strCountry[256];

	// ������ʾ�Ƿ��ǹ���
	// ͨ���л��Ƿ�ӵ������
	// ��ͨ��������Ƿ��ǰ������ж� bugs
	for( list<tagownedReg>::iterator it = m_OwnedRegions.begin(); it!=m_OwnedRegions.end(); ++it)
	{
		if(it->wType == 2) // ���� ��Ȼ��4�����ң���ÿ������ֻ��һ�����ǣ�����������ֻ�ܱ��������л���¡�
		{
			if(GetExID() == m_FacMasterID)
			{
				bIsKing = true;
				break;
			}
		}
	}

	// ������������ʾ��Ϣ���س���
	if (bPlayerNameCheck)
		lDrawNameLineLen += (GetGame()->GetFontWidth(str) + 4);

	if (GetShowCountry())
	{
		if (pst)
			lDrawNameLineLen += 30;
		else
		{
			sprintf(strCountry, AppFrame::GetText("Base_100"));
			lDrawNameLineLen +=  GetGame()->GetFontWidth(strCountry);
		}
	}

	if (GetPVPValue())
	{
		if( GetPVPValue() < PVP_PIRATE1 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate1.tga");
		}
		else if( GetPVPValue() < PVP_PIRATE2 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate2.tga");
		}
		else if( GetPVPValue() < PVP_PIRATE3 )
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate3.tga");
		}
		else
		{
			pbmPirate = CBitmapX::GetBitmapX("pictures/state/pirate4.tga");
		}
		if (pbmPirate)
			lDrawNameLineLen += pbmPirate->GetWidth();
	}

	// �����м�ͷ��ʾ
	if (m_bIsSelected)
	{
		long lTmpY;
		pSelectShape = GetGame()->m_pSelectShape;
		DWORD dwW2	 = pSelectShape->GetWidth() / 2;
		DWORD dwH	 = pSelectShape->GetHeight();
		RECT  rc	 = { 0, 0, dwW2, dwH };
		lDrawNamePos = (long)pos2.x - dwW2 - lDrawNameLineLen / 2;		//���������л���X��ʼ����

		if (GetFactionID() != CGUID::GUID_INVALID  && GetFactionID() == GetGame()->GetMainPlayer()->GetFactionID() && bPlayerGuildInfoCheck)
			lTmpY = y - lFontHeight - 3;
		else
			lTmpY = y;

		GetGame()->m_pSelectShape->Draw(lDrawNamePos - 8, lTmpY, rc);

		rc.left  = dwW2;
		rc.right = dwW2 * 2;

		GetGame()->m_pSelectShape->Draw(lDrawNamePos + 8 + dwW2 + lDrawNameLineLen, lTmpY, rc);
		lDrawNamePos += dwW2;
	}else
		lDrawNamePos = (long)pos2.x - lDrawNameLineLen / 2;

	// ���л��Ա����ʾ�л�
	if(GetFactionID() != CGUID::GUID_INVALID && GetFactionID() == GetGame()->GetMainPlayer()->GetFactionID() && bPlayerGuildInfoCheck)
	{
		CBitmapX * pPic		  = NULL;
		string strFactionName =  "<";
		strFactionName		  += GetFactionName();
		strFactionName		  += ">";
		len = (int)strFactionName.length();
		x = (long)pos2.x - GetGame()->GetFontWidth(strFactionName.c_str()) / 2;

		if( NULL_GUID != m_FactionID )
		{
			string strFacIconFile = GetGame()->GetFactionManager()->GetFacIconPath( m_FactionID );

			fstream file;
			file.open(strFacIconFile.c_str(), ios::in);
			if (file.is_open())
			{
				file.close();
				x -= (12 + 5);
				pPic = CBitmapX::GetBitmapX(strFacIconFile);
				if( pPic )
				{
					pPic->Draw(x, y, 24, 16);
					x += (24 + 5);
				}
			}
		}
		// ��ʾ�л�����
		if (IsSelf() || GetShowCountry() || GetPkValue())
		{
			GetGame()->OutPutText(x+1, y+1, strFactionName.c_str(), len, bkcolor);
			GetGame()->OutPutText(x, y, strFactionName.c_str(), len, color);
		}else
		{
			GetGame()->OutPutText(x+1, y+1, strFactionName.c_str(), len, CGlobeSetup::GetSetup()->colorNoShowCountryNameBack);
			GetGame()->OutPutText(x, y, strFactionName.c_str(), len, CGlobeSetup::GetSetup()->colorNoShowCountryName);
		}
		y -= (lFontHeight + 3);
	}

	// ��ʾ����
	if (GetShowCountry())
	{
		if (pst)
		{
			GetGame()->GetPicList()->Display(CPicList::PT_COUNTRY_ICON, pst->lIconID, lDrawNamePos, y);
			lDrawNamePos += 30;
		}else
		{
			static D3DCOLOR colorCountry = 0xffffffff, colorCountryBack = 0xff000000;
			sprintf(strCountry, AppFrame::GetText("Base_100"));
			colorCountry	 = 0xffffffff;
			colorCountryBack = 0xff000000;

			len = lstrlen(strCountry);
			x   = x - GetGame()->GetFontWidth(strCountry);
			lDrawNamePos += GetGame()->GetFontWidth(strCountry) * lstrlen(strCountry);

			GetGame()->OutPutText(x + 1, y + 1, strCountry, len, colorCountryBack);
			GetGame()->OutPutText(x, y, strCountry, len, colorCountry);
		}
	}

	// ����������ʾ��Ϣ
	if (bPlayerNameCheck)
	{
		len = lstrlen(str);
		// ��ʾ����
		if (IsSelf() || GetShowCountry() || GetPkValue())
		{
			GetGame()->OutPutText(lDrawNamePos + 1, y + 1, str, len, bkcolor);
			GetGame()->OutPutText(lDrawNamePos, y, str, len, color);
		}else
		{
			color = CGlobeSetup::GetSetup()->colorNoShowCountryName;
			bkcolor = CGlobeSetup::GetSetup()->colorNoShowCountryNameBack;
			GetGame()->OutPutText(lDrawNamePos + 1, y + 1, str, len, bkcolor);
			GetGame()->OutPutText(lDrawNamePos, y, str, len, color);
		}

		lDrawNamePos += GetGame()->GetFontWidth(str);
	}

	// ���壺���ƺ�����־��ֻҪ����ֵ��Ϊ0
	if (GetPVPValue())
	{
		// ����ͼƬ
		if (pbmPirate)
			pbmPirate->Draw(lDrawNamePos + 3, y);
	}
}

/////////////////////////��װ�ӿ�/////////////////////////////////////////////////////////
//�ж������Ƿ��д�ID����װ
bool CPlayer::IsSuitBeHold(WORD SuitID)
{
	map<WORD,tagSuitAttribute_P>::iterator iter = m_SuitList.find(SuitID);
	if (iter != m_SuitList.end())
	{
		return true;
	}
	return false;
}

//�ж��Ƿ��д�ID��װ����������,true��ʾ������һ��װ���������ˣ�false��ʾһ����û��
bool CPlayer::IsSuitBeSetuped(WORD SuitID)
{
	if (IsSuitBeHold(SuitID))
	{
		map<WORD,tagSuitAttribute_P>::iterator iter = m_SuitList.find(SuitID);
		if (iter != m_SuitList.end())
		{
			tagSuitAttribute_P temp = iter->second;
			map<string,bool>::iterator iiter = temp.Suit_Equit.begin();
			for (;iiter!=temp.Suit_Equit.end();iiter++)
			{
				if (iiter->second)
				{
					return true;
				}			
			}
		}
		m_SuitList[SuitID].Suit_SetupedNum = 0;
	}
	return false;
}

//����SuitID����װ����m_SuitList�У����ѵ�һ��װ����ԭʼ������Suit_OriginName��
void CPlayer::AddSuit(WORD SuitID,string SuitOriginName)			
{
	if (!IsSuitBeHold(SuitID))
	{
		tagSuitAttribute_P temp;
		temp.Suit_Num = 1;
		temp.Suit_SetupedNum = 0;
		temp.Suit_Equit[SuitOriginName] = false;
		m_SuitList.insert(pair<WORD,tagSuitAttribute_P>(SuitID,temp));
	}else if(!CompareSuitOriginName(SuitID,SuitOriginName))
	{
		m_SuitList[SuitID].Suit_Equit.insert(pair<string,bool>(SuitOriginName,false));
		m_SuitList[SuitID].Suit_Num++;
		m_SuitList[SuitID].Suit_Equit[SuitOriginName] = false;
	}
}

//�Ƚ��ж���ӵ�е���װ���Ƿ��д�ԭʼ����װ��
bool CPlayer::CompareSuitOriginName(WORD SuitID,string SuitOriginName)
{
	if (IsSuitBeHold(SuitID))
	{	
		map<string,bool>::iterator it = m_SuitList[SuitID].Suit_Equit.find(SuitOriginName);
		if (it!=m_SuitList[SuitID].Suit_Equit.end())
		{
			return true;
		}
	}
	return false;
}

//��ȡ��ID����װ��ӵ�еļ���
WORD CPlayer::GetNumOfPossessionSuit(WORD SuitID)									
{
	if (SuitID)
	{
		if (IsSuitBeHold(SuitID))
		{	
			return m_SuitList[SuitID].Suit_Num;
		}
	}
	return 0;
}

//��ȡ��ID����װ��װ���ϵļ���
WORD CPlayer::GetNumOfSetupEquip(WORD SuitID)	
{
	if (SuitID)
	{
		if (IsSuitBeSetuped(SuitID))
		{
			return m_SuitList[SuitID].Suit_SetupedNum;
		}
	}
	return 0;
}

//void CPlayer::AddEquipToSuit(WORD SuitID,string SuitOriginName)													//����ԭʼ����װ�������ӦID����װ�б���
//{
//	if (IsSuitBeHold(SuitID))
//	{
//		if (!CompareSuitOriginName(SuitID,SuitOriginName))
//		{
//			m_SuitList[SuitID].Suit_Equit.insert(pair<string,bool>(SuitOriginName,false));
//			m_SuitList[SuitID].Suit_Num++;
//		}
//	}
//}


//����ԭʼ����װ���Ƿ��ѱ�װ����
bool CPlayer::CompareSetEquipOriName(WORD SuitID,string SuitOriginName)
{
	if (IsSuitBeHold(SuitID))
	{
		if (CompareSuitOriginName(SuitID,SuitOriginName))
		{
			if (m_SuitList[SuitID].Suit_Equit[SuitOriginName])
			{
				return true;
			}
		}
	}
	return false;
}

//����ԭʼ����װ����������
void CPlayer::SetupEquip(WORD SuitID,string SuitOriginName)
{
	if (IsSuitBeSetuped(SuitID))
	{
		if (!CompareSetEquipOriName(SuitID,SuitOriginName))
		{
			m_SuitList[SuitID].Suit_SetupedNum++;
		}
	}else
	{
		m_SuitList[SuitID].Suit_SetupedNum = 1;
	}
	AddSuit(SuitID,SuitOriginName);
	m_SuitList[SuitID].Suit_Equit[SuitOriginName] = true;
}

//���װ�����д�IDԭʼ��ΪSuitOriginName����Ʒ����
WORD CPlayer::FindSetupEquipForOriName(WORD SuitID,string SuitOriginName)
{
	WORD num = 0;
	//��װ���в���
	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	{
		CGoods* pEquip=GetEquip(i);
		if(pEquip)
		{
			if(pEquip->GetEffectVal(GAP_SUITID,1) == SuitID&&
				pEquip->GetProperty()->strOrginName == SuitOriginName)
			{
				num++;
			}
		}
	}
	return num;
}

//����ԭʼ����װ������������
void CPlayer::UnfixEquip(WORD SuitID,string SuitOriginName)
{
	if (m_SuitList[SuitID].Suit_Equit[SuitOriginName]&&
		FindSetupEquipForOriName(SuitID,SuitOriginName)==0)
	{
		m_SuitList[SuitID].Suit_Equit[SuitOriginName] = false;
		m_SuitList[SuitID].Suit_SetupedNum--;
	}
}

//�жϴ�ID����װ�Ƿ��Ѵ���
bool CPlayer::IsSuitComplete(WORD SuitID)
{
	WORD Num = (WORD)CGoodsList::GetSuitCount(SuitID);
	if (m_SuitList[SuitID].Suit_Num == Num)
	{
		return true;
	}
	return false;
}

//�������������ж��ٴ�IDԭʼ��ΪSuitOriginName����Ʒ
WORD CPlayer::FindEquipByOrName(WORD SuitID,string SuitOriginName)
{
	WORD num = 0;
	//if(m_stHand.pGoods && m_stHand.pGoods->GetEffectVal(CGoods::GAP_SUITID,1) == SuitID&&
	//	m_stHand.pGoods->GetProperty()->strOrginName==SuitOriginName)
	//	num++;

	//��װ���в���
	for(int i=0;i<CPlayer::EQUIP_SUM;i++)
	{
		CGoods* pEquip=GetEquip(i);
		if(pEquip)
		{
			if(pEquip->GetEffectVal(GAP_SUITID,1) == SuitID&&
				pEquip->GetProperty()->strOrginName == SuitOriginName)
			{
				num++;
			}
		}
	}
	//�ڱ����в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			CGoods* pGoods = GoodsIter->pGoods;
			if(pGoods->GetEffectVal(GAP_SUITID,1) == SuitID&&
				pGoods->GetProperty()->strOrginName == SuitOriginName)
			{
				num++;
			}
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				CGoods* pGoods = GoodsIter->pGoods;
				if(pGoods)
				{
					if(pGoods->GetEffectVal(GAP_SUITID,1) == SuitID&&
						pGoods->GetProperty()->strOrginName == SuitOriginName)
					{
						num++;
					}
				}
			}
		}
	}

	//�ڲֿ��в���
	/*list<tagGoods> *pDepotGoodsList=GetGame()->GetInterface()->GetDepotPage()->GetDepotList();
	GoodsIter=pDepotGoodsList->begin();
	for(;GoodsIter!=pDepotGoodsList->end();GoodsIter++)
	{
	CGoods* pGoods = GoodsIter->pGoods;
	if(pGoods && pGoods->GetExID() == guID)
	{	
	if(pPos)pPos[0]=GoodsIter->lPos;
	return pGoods;
	}
	}*/
	//�ڸ����̵������

	return num;
}

//�Ѵ�ID����װ��ԭʼ��ΪSuitOriginName��װ����tagSuitAttribute_P��ɾ��
bool CPlayer::RemoveEquipFromSuit(WORD SuitID,string SuitOriginName)
{
	if (FindEquipByOrName(SuitID,SuitOriginName)==0)
	{
		map<string,bool>::iterator iter = m_SuitList[SuitID].Suit_Equit.find(SuitOriginName);
		if (iter!=m_SuitList[SuitID].Suit_Equit.end())
		{
			m_SuitList[SuitID].Suit_Equit.erase(iter);
			m_SuitList[SuitID].Suit_Num--;
			return true;
		}
	}
	return false;
}

//�Ѵ�ID����װ��m_SuitList��ɾ��
bool CPlayer::RemoveSuit(WORD SuitID)
{
	if(m_SuitList[SuitID].Suit_Num==0)
	{
		map<WORD,tagSuitAttribute_P>::iterator iter = m_SuitList.find(SuitID);
		m_SuitList.erase(iter);
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�����������ʾ
void CPlayer::UpdateRideShow()
{
	extern bool g_bForceUpdate;
	g_bForceUpdate = true;
	if (IsRide())
	{
		DWORD index = 0;
		SetAction(ACT_STAND);
		if (m_bSelf)
		{
			CGoods* pgoods = GetEquip(CPlayer::EQUIP_RIDER);
			if (pgoods)
			{
				index = GetEquip(CPlayer::EQUIP_RIDER)->GetIndex();
			}

		}
		else
		{
			index = GetEquipIndex(CPlayer::EQUIP_RIDER);
		}
		CGoodsList::tagGoods2* pGoods2 = CGoodsList::GetProperty(index);
		if (pGoods2)
		{
			m_Horse.SetHorseType(pGoods2->BaseProperty.dwWeaponActType);
			m_Horse.SetModelID(pGoods2->BaseProperty.dwEquipID);
			m_Horse.ChangeStatus(GetAction(),true);
			m_Horse.SetVisible(true);
			m_Horse.SetDisplayModel();
		}
		else
		{
			m_Horse.SetHorseType(0);
			m_Horse.SetModelID(0);
			m_Horse.SetVisible(false);

		}

	}
	else
	{
		m_Horse.SetHorseType(0);
		m_Horse.SetModelID(0);
		m_Horse.SetVisible(false);
		SetAction(GetAction());
	}
	SetState(GetState());
	GetAnimInfo()->GetActionInfo()->bInterpolation = FALSE;
}
//////////////////////////////////////////////////////////////////////////
//����װ��ģ�͵���ʾ
void CPlayer::UpdateAllWeaponShow()
{
	//***********
	//������ʾ����ADD by ��ǫ
	//***********
	long mod1ID = 0;
	long mod2ID = 0;

	CGoods* prequip = GetEquip(CPlayer::EQUIP_WEAPON);
//	CGoods* psequip = GetEquip(CPlayer::EQUIP_WEAPON2);
	CGoods* pwequip = GetEquip(CPlayer::EQUIP_WING);
	CGoods* phequip = GetEquip(CPlayer::EQUIP_P_HEADGEAR);
	DWORD RLevel = 0;
	DWORD LLevel = 0;
	DWORD WLevel = 0;
	DWORD HLevel = 0;
	CGoodsList::tagEffect* ptegeffect;
	CGoodsList::tagGoods2* taggoods2;
	if (prequip != NULL)
	{
		RLevel = prequip->GetEffectVal(GAP_WEAPON_LEVEL,1);
		taggoods2 = CGoodsList::GetProperty(prequip->GetIndex());
		if(prequip->GetEquipType() == EC_E_WEAPON)
		{

			long ltype = prequip->GetEffectVal(GAP_WEAPON_CATEGORY,1);

			switch(ltype)
			{
				//���ֽ�
				//���ִ�
				//ذ��
			case WT_SINGLE_SWORD:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLESWORD);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
			case WT_SINGLE_HAMMER:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEHAMMER);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
			case WT_SINGLE_KNIFE:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEKNIFE);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
				//�޽�
			case WT_HEAVY_SWORD:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGSWORD);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
				//����
				//��
			case WT_WAND:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGWANG);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
			case WT_HEAVY_AX:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGAX);
				mod1ID = taggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
				//��
			case WT_BOW:
				//����
				ptegeffect = CGoodsList::GetEffectTag(prequip->GetIndex(),GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOW);
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				//����,��δ�ӹ���
				/*m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOW);
				mod2ID = pGoods->GetEffectVal(CGoods::GAP_COSPLAYID,2);
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);*/
				break;
				//˫�ֳֽ�
			case WT_DOUBLE_SWORD:
				//����
				ptegeffect = CGoodsList::GetEffectTag(prequip->GetIndex(),GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
				m_LeftHandWeapon.SetModelID(mod1ID);
				m_LeftHandWeapon.SetVisible(true);
				m_LeftHandWeapon.SetDisplayModel();
				m_LeftHandWeapon.SetLevel(RLevel);
				//����
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
				mod2ID = ptegeffect->dwValue2;
				m_RightHandWeapon.SetModelID(mod2ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;
				//ȭ��
			case WT_FIST:
				//����
				ptegeffect = CGoodsList::GetEffectTag(prequip->GetIndex(),GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGLEFT);
				m_LeftHandWeapon.SetModelID(mod1ID);
				m_LeftHandWeapon.SetVisible(true);
				m_LeftHandWeapon.SetDisplayModel();
				m_LeftHandWeapon.SetLevel(RLevel);
				//����
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGRIGHT);
				mod2ID = ptegeffect->dwValue2;
				m_RightHandWeapon.SetModelID(mod2ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(RLevel);
				break;


			}



		}
	}
	/*if (psequip != NULL)
	{
		LLevel = psequip->GetEffectVal(CGoods::GAP_WEAPON_LEVEL,1);
		if(CGoodsList::GetProperty(psequip->GetIndex())->BaseProperty.dwType == CGoodsList::GT_WEAPON2)
		{
			m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SHIELD);
			mod1ID = CGoodsList::GetProperty(psequip->GetIndex())->BaseProperty.dwEquipID;
			m_LeftHandWeapon.SetModelID(mod1ID);
			m_LeftHandWeapon.SetVisible(true);
			m_LeftHandWeapon.SetDisplayModel();
			m_LeftHandWeapon.SetLevel(LLevel);
		}
	}*/
	if (pwequip != NULL)
	{
		WLevel = pwequip->GetEffectVal(GAP_WEAPON_LEVEL,1);
		if(pwequip->GetEquipType() == EC_E_WING)
		{
			mod1ID = CGoodsList::GetProperty(pwequip->GetIndex())->BaseProperty.dwEquipID;
			m_Wing.SetModelID(mod1ID);
			m_Wing.SetVisible(true);
			m_Wing.SetDisplayModel();
			m_Wing.SetLevel(WLevel);
		}
	}
	if (phequip != NULL)
	{
		HLevel = phequip->GetEffectVal(GAP_WEAPON_LEVEL,1);
		if(phequip->GetEquipType() == EC_E_HEAD)
		{
			mod1ID = CGoodsList::GetProperty(phequip->GetIndex())->BaseProperty.dwEquipID;
			m_HeadWear.SetModelID(mod1ID);
			m_HeadWear.SetVisible(true);
			m_HeadWear.SetDisplayModel();
			m_HeadWear.SetLevel(HLevel);
		}
	}
	extern bool g_bForceUpdate;
	g_bForceUpdate = true;
	SetAction(GetAction());
	g_bForceUpdate = false;
	//***********
	//������ʾ����ADD by ��ǫ
	//***********

}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����װ��ģ�͵���ʾ
void CPlayer::UpdateWeaponShow(CGoods* pGoods)
{
	if (pGoods == NULL)
	{
		return;
	}
	//***********
	//������ʾ����ADD by ��ǫ
	//***********
	long mod1ID = 0;
	long mod2ID = 0;
	DWORD index = pGoods->GetIndex();
	long level = 0;
	CGoodsList::tagEffect* ptegeffect;
	CGoodsList::tagGoods2* ptaggoods2 = CGoodsList::GetProperty(index);
	if (ptaggoods2)
	{
		eEquipType equipType = pGoods->GetEquipType();
		if(equipType == EC_E_WEAPON)
		{

			long ltype = pGoods->GetEffectVal(GAP_WEAPON_CATEGORY,1);
			level = pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1);

			switch(ltype)
			{
				//���ֽ�
				//���ִ�
				//ذ��
			case WT_SINGLE_SWORD:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLESWORD);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
			case WT_SINGLE_HAMMER:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEHAMMER);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
			case WT_SINGLE_KNIFE:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEKNIFE);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
				//�޽�
			case WT_HEAVY_SWORD:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGSWORD);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
				//����
				//��
			case WT_WAND:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGWANG);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
			case WT_HEAVY_AX:
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGAX);
				mod1ID = ptaggoods2->BaseProperty.dwEquipID;
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
				//��
			case WT_BOW:
				//����
				ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOW);
				m_RightHandWeapon.SetModelID(mod1ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				//����,��δ�ӹ���
				m_LeftHandWeapon.SetWeaponType(0);
				//mod2ID = pGoods->GetEffectVal(0);
				m_LeftHandWeapon.SetModelID(0);
				m_LeftHandWeapon.SetVisible(false);
				m_LeftHandWeapon.SetDisplayModel();
				m_LeftHandWeapon.SetLevel(0);
				break;

				//˫�ֳֽ�
			case WT_DOUBLE_SWORD:
				//����
				ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
				m_LeftHandWeapon.SetModelID(mod1ID);
				m_LeftHandWeapon.SetVisible(true);
				m_LeftHandWeapon.SetDisplayModel();
				m_LeftHandWeapon.SetLevel(level);
				//����
				mod2ID = ptegeffect->dwValue2;
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
				m_RightHandWeapon.SetModelID(mod2ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);
				break;
				//ȭ��
			case WT_FIST:
				//����
				ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
				mod1ID = ptegeffect->dwValue1;
				m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGLEFT);
				m_LeftHandWeapon.SetModelID(mod1ID);
				m_LeftHandWeapon.SetVisible(true);
				m_LeftHandWeapon.SetDisplayModel();
				m_LeftHandWeapon.SetLevel(level);
				//����
				mod2ID = ptegeffect->dwValue2;
				m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGRIGHT);
				mod2ID = pGoods->GetEffectVal(GAP_BOTHHAND_GRAPHIC,2);
				m_RightHandWeapon.SetModelID(mod2ID);
				m_RightHandWeapon.SetVisible(true);
				m_RightHandWeapon.SetDisplayModel();
				m_RightHandWeapon.SetLevel(level);

				break;

			}




		}
		// ���������ƣ�
		else if(equipType == EC_E_CARD)
		{

			level = pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1);
			mod1ID = ptaggoods2->BaseProperty.dwEquipID;
			m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SHIELD);
			m_LeftHandWeapon.SetModelID(mod1ID);
			m_LeftHandWeapon.SetVisible(true);
			m_LeftHandWeapon.SetDisplayModel();
			m_LeftHandWeapon.SetLevel(level);
		}
		else if(equipType == EC_E_WING)
		{

			level = pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1);
			mod1ID = ptaggoods2->BaseProperty.dwEquipID;
			m_Wing.SetModelID(mod1ID);
			m_Wing.SetVisible(true);
			m_Wing.SetDisplayModel();
			m_Wing.SetLevel(level);
		}
		else if(equipType == EC_E_HEAD)
		{

			level = pGoods->GetEffectVal(GAP_WEAPON_LEVEL,1);
			mod1ID = ptaggoods2->BaseProperty.dwEquipID;
			m_HeadWear.SetModelID(mod1ID);
			m_HeadWear.SetVisible(true);
			m_HeadWear.SetDisplayModel();
			m_HeadWear.SetLevel(level);
		}


		extern bool g_bForceUpdate;
		g_bForceUpdate = true;
		if (GetAction() != CShape::ACT_PICK)
		{
			SetAction(GetAction());
		}
		g_bForceUpdate = false;
		//***********
		//������ʾ����ADD by ��ǫ
		//***********
	}


}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//����װ��ģ�͵���ʾ
void CPlayer::UpdateWeaponShow(DWORD dwPos)
{

	//***********
	//������ʾ����ADD by ��ǫ
	//***********
	DWORD index = m_dwEquipIndex[dwPos];
	DWORD level = m_btEquipLevel[dwPos];
	CGoodsList::tagGoods2* pGoods = CGoodsList::GetProperty(index);
	if (pGoods == NULL)
	{
		return;
	}
	long mod1ID = 0;
	long mod2ID = 0;

	long equipType = CGoodsList::GetEffectTag(index,GAP_EQUIP_CATEGORY)->dwValue1;
	if(equipType == EC_E_WEAPON)
	{

		CGoodsList::tagEffect* ptegeffect = CGoodsList::GetEffectTag(index,GAP_WEAPON_CATEGORY);
		long ltype = ptegeffect->dwValue1;
		switch(ltype)
		{
			//���ֽ�
			//���ִ�
			//ذ��

		case WT_SINGLE_SWORD:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLESWORD);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
		case WT_SINGLE_HAMMER:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEHAMMER);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
		case WT_SINGLE_KNIFE:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SINGLEKNIFE);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
			//�޽�
		case WT_HEAVY_SWORD:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGSWORD);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
			//����
			//��
		case WT_WAND:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGWANG);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
		case WT_HEAVY_AX:
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BIGAX);
			mod1ID = pGoods->BaseProperty.dwEquipID;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
			//��
		case WT_BOW:
			ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
			ASSERT(ptegeffect);
			//����
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOW);
			mod1ID = ptegeffect->dwValue1;
			m_RightHandWeapon.SetModelID(mod1ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			//����,��δ�ӹ���
			m_LeftHandWeapon.SetWeaponType(0);
			m_LeftHandWeapon.SetModelID(0);
			m_LeftHandWeapon.SetVisible(false);
			m_LeftHandWeapon.SetDisplayModel();
			m_LeftHandWeapon.SetLevel(0);
			break;

			//˫�ֳֽ�
		case WT_DOUBLE_SWORD:
			ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
			ASSERT(ptegeffect);
			//����
			m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
			mod1ID = ptegeffect->dwValue1;
			m_LeftHandWeapon.SetModelID(mod1ID);
			m_LeftHandWeapon.SetVisible(true);
			m_LeftHandWeapon.SetDisplayModel();
			m_LeftHandWeapon.SetLevel(level);
			//����
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_DOUBLEWEAPON);
			mod2ID = ptegeffect->dwValue2;
			m_RightHandWeapon.SetModelID(mod2ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;
			//ȭ��
		case WT_FIST:
			ptegeffect = CGoodsList::GetEffectTag(index,GAP_BOTHHAND_GRAPHIC);
			ASSERT(ptegeffect);
			//����
			m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGLEFT);
			mod1ID = ptegeffect->dwValue1;
			m_LeftHandWeapon.SetModelID(mod1ID);
			m_LeftHandWeapon.SetVisible(true);
			m_LeftHandWeapon.SetDisplayModel();
			m_LeftHandWeapon.SetLevel(level);
			//����
			m_RightHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_BOXINGRIGHT);
			mod2ID = ptegeffect->dwValue2;
			m_RightHandWeapon.SetModelID(mod2ID);
			m_RightHandWeapon.SetVisible(true);
			m_RightHandWeapon.SetDisplayModel();
			m_RightHandWeapon.SetLevel(level);
			break;

		}



	}
	else if(equipType == EC_E_CARD)
	{
		m_LeftHandWeapon.SetWeaponType(CWeapon::WEAPONSHOW_SHIELD);
		mod1ID = pGoods->BaseProperty.dwEquipID;
		m_LeftHandWeapon.SetModelID(mod1ID);
		m_LeftHandWeapon.SetVisible(true);
		m_LeftHandWeapon.SetDisplayModel();
		m_LeftHandWeapon.SetLevel(level);
	}
	else if(equipType == EC_E_WING)
	{
		mod1ID = pGoods->BaseProperty.dwEquipID;
		m_Wing.SetModelID(mod1ID);
		m_Wing.SetVisible(true);
		m_Wing.SetDisplayModel();
		m_Wing.SetLevel(level);
	}
	else if(equipType == EC_E_HEAD)
	{
		mod1ID = pGoods->BaseProperty.dwEquipID;
		m_HeadWear.SetModelID(mod1ID);
		m_HeadWear.SetVisible(true);
		m_HeadWear.SetDisplayModel();
		m_HeadWear.SetLevel(level);
	}
	extern bool g_bForceUpdate;
	g_bForceUpdate = true;
	SetAction(GetAction());
	g_bForceUpdate = false;
	//***********
	//������ʾ����ADD by ��ǫ
	//***********

}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//ȡ����Ʒ������ʾ
void CPlayer::RemoveWeaponShow(CGoods* pGoods)
{
	//*******
	//ȡ��������ʾADD BY ��ǫ
	//*******
	if (!pGoods)
	{
		return;
	}
	DWORD index = pGoods->GetIndex();
	CGoodsList::tagGoods2* ptaggoods2 = CGoodsList::GetProperty(index);
	if (ptaggoods2)
	{
		eEquipType equipType = pGoods->GetEquipType();
		if (equipType == EC_E_WEAPON)
		{
			CGoodsList::tagEffect* ptegeffect = CGoodsList::GetEffectTag(index,GAP_WEAPON_CATEGORY);
			long ltype = 0;
			if (ptegeffect!=NULL) 
				ltype = ptegeffect->dwValue1;
			if (ltype >=WT_BOW&&ltype <=WT_FIST)
			{
				m_LeftHandWeapon.Hide();
				m_LeftHandWeapon.SetVisible(false);
				m_LeftHandWeapon.SetWeaponType(0);
				m_LeftHandWeapon.SetModelID(0);
				m_LeftHandWeapon.SetLevel(0);
				m_RightHandWeapon.Hide();
				m_RightHandWeapon.SetVisible(false);
				m_RightHandWeapon.SetWeaponType(0);
				m_RightHandWeapon.SetModelID(0);
				m_RightHandWeapon.SetLevel(0);
			}
			else
			{
				m_RightHandWeapon.Hide();
				m_RightHandWeapon.SetVisible(false);
				m_RightHandWeapon.SetWeaponType(0);
				m_RightHandWeapon.SetModelID(0);
				m_RightHandWeapon.SetLevel(0);
			}

		}
		else if(equipType == EC_E_CARD)
		{
			if (m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_DOUBLEWEAPON
				||m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_BOW
				||m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_BOXINGRIGHT)
			{

			}
			else
			{
				m_LeftHandWeapon.Hide();
				m_LeftHandWeapon.SetVisible(false);
				m_LeftHandWeapon.SetWeaponType(0);
				m_LeftHandWeapon.SetModelID(0);
				m_LeftHandWeapon.SetLevel(0);
			}

		}
		else if(equipType == EC_E_WING)
		{
			m_Wing.Hide();
			m_Wing.SetVisible(false);
			m_Wing.SetModelID(0);
			m_Wing.SetLevel(0);
		}
		else if(equipType == EC_E_HEAD)
		{
			m_HeadWear.Hide();
			m_HeadWear.SetVisible(false);
			m_HeadWear.SetModelID(0);
			m_HeadWear.SetLevel(0);
		}
		extern bool g_bForceUpdate;
		g_bForceUpdate = true;
		SetAction(GetAction());
		g_bForceUpdate = false;
		//*******
		//ȡ��������ʾADD BY ��ǫ
		//*******
	}

}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//ȡ����Ʒ������ʾ
void CPlayer::RemoveWeaponShow(DWORD dwPos)
{
	//*******
	//ȡ��������ʾADD BY ��ǫ
	//*******
	DWORD index = m_dwEquipIndex[dwPos];
	CGoodsList::tagGoods2* ptaggoods2 = CGoodsList::GetProperty(index);
	if (ptaggoods2)
	{
		long equipType = CGoodsList::GetEffectTag(index,GAP_EQUIP_CATEGORY)->dwValue1;
		if (equipType == EC_E_WEAPON)
		{
			CGoodsList::tagEffect* ptegeffect = CGoodsList::GetEffectTag(index,GAP_WEAPON_CATEGORY);
			long ltype = ptegeffect->dwValue1;
			if (ltype >=WT_BOW&&ltype <=WT_FIST)
			{
				m_LeftHandWeapon.Hide();
				m_LeftHandWeapon.SetVisible(false);
				m_LeftHandWeapon.SetWeaponType(0);
				m_LeftHandWeapon.SetModelID(0);
				m_LeftHandWeapon.SetLevel(0);
				m_RightHandWeapon.Hide();
				m_RightHandWeapon.SetVisible(false);
				m_RightHandWeapon.SetWeaponType(0);
				m_RightHandWeapon.SetModelID(0);
				m_RightHandWeapon.SetLevel(0);
			}
			else
			{
				m_RightHandWeapon.Hide();
				m_RightHandWeapon.SetVisible(false);
				m_RightHandWeapon.SetWeaponType(0);
				m_RightHandWeapon.SetModelID(0);
				m_RightHandWeapon.SetLevel(0);
			}
		}
		else if(equipType == EC_E_CARD)
		{
			if (m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_DOUBLEWEAPON
				||m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_BOW
				||m_RightHandWeapon.GetWeaponType() == CWeapon::WEAPONSHOW_BOXINGRIGHT)
			{

			}
			else
			{
				m_LeftHandWeapon.Hide();
				m_LeftHandWeapon.SetVisible(false);
				m_LeftHandWeapon.SetWeaponType(0);
				m_LeftHandWeapon.SetModelID(0);
				m_LeftHandWeapon.SetLevel(0);
			}
		}
		else if(equipType == EC_E_WING)
		{
			m_Wing.Hide();
			m_Wing.SetVisible(false);
			m_Wing.SetModelID(0);
			m_Wing.SetLevel(0);
		}
		else if(equipType == EC_E_HEAD)
		{
			m_HeadWear.Hide();
			m_HeadWear.SetVisible(false);
			m_HeadWear.SetModelID(0);
			m_HeadWear.SetLevel(0);
		}
		extern bool g_bForceUpdate;
		g_bForceUpdate = true;
		SetAction(GetAction());
		g_bForceUpdate = false;
		//*******
		//ȡ��������ʾADD BY ��ǫ
		//*******
	}

}



//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////

// ��������
void CPlayer::UpdateProperty(CMessage* pMsg/*,char** &strName,DWORD* &dwValue,char** &str,WORD& wNum*/)
{
	//BYTE pByte[102400];
	//memset(pByte,0,102400);
	//pMsg->GetEx(pByte,102400);
	//long lpos = 0;
	//DecordChangeProperty(pByte,lpos/*,strName,dwValue,str,wNum*/);
	DBReadSet setReadDB;
	pMsg->GetDBReadSet(setReadDB);
	CPlayer::DecodeChangePropertyFromDataBlock(setReadDB);
}

// �����仯�������
void CPlayer::DecordChangeProperty(BYTE* pByte, long& pos/*,char** &strName,DWORD* &dwValue,char** &str,WORD& wNum*/)
{
	CMoveShape::DecordChangeProperty(pByte,pos);
	WORD wNum = _GetWordFromByteArray(pByte,pos);
	WORD pro_type = 0;
	BYTE data_type = 0;
	WORD data_length = 0;
	long data = 0;
	BYTE* byte_Array = NULL;
	WORD* word_Array = NULL;
	DWORD* dword_Array = NULL;
	float* flaot_Array = NULL;
	long*  long_Array = NULL;
	short* short_Array = NULL;
	char*  char_Array = NULL;
	bool*  bool_Array = NULL;

	//if (wNum>0)
	//{
	//	strName = new char*[wNum];
	//	dwValue = new DWORD[wNum];
	//	str = new char*[wNum];
	//}
	for (int i = 0;i<wNum;i++)
	{	
		//strName[i] = new char[30];
		//str[i] = new char[30];
		//strcpy(strName[i],"");
		//strcpy(str[i],"");
		//dwValue[i] = -1;
		pro_type = _GetWordFromByteArray(pByte,pos);
		data_type = _GetByteFromByteArray(pByte, pos);
		data_length = _GetWordFromByteArray(pByte,pos);		
		if (data_length<=0) continue;

		//�����������ͻ�ȡ���������ֵ
		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = _GetByteFromByteArray(pByte, pos);
			break;
		case ATTR_DATA_WORD:
			data = _GetWordFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_DWORD:
			data = _GetDwordFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_FLOAT:
			data = (long)_GetFloatFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_LONG:
			data = _GetLongFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_SHORT:
			data = _GetShortFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_BOOL:
			data = _GetByteFromByteArray(pByte,pos);
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				byte_Array[j] = _GetByteFromByteArray(pByte, pos);
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				word_Array[j] = _GetWordFromByteArray(pByte,pos);
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				dword_Array[j] = _GetDwordFromByteArray(pByte,pos);
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			flaot_Array = new float[data_length];
			if (!flaot_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				flaot_Array[j] = _GetFloatFromByteArray(pByte,pos);
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				long_Array[j] = _GetLongFromByteArray(pByte,pos);
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				short_Array[j] = _GetShortFromByteArray(pByte,pos);
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			_GetStringFromByteArray(pByte, pos, char_Array);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				data = _GetByteFromByteArray(pByte, pos);
				if (data!=0)
				{
					bool_Array[j] = true;
				}else
					bool_Array[j] = false;
			}
			break;
		default:
			break;
		}						
		//������Ӧ����ֵ
		itProETN it = m_PropertyEnumToName.find(pro_type);
		if(it != m_PropertyEnumToName.end())
		{
			SetProperty( (*it).second.c_str(),data);
		}

		SAFEDELETE(byte_Array);
		SAFEDELETE(word_Array);
		SAFEDELETE(dword_Array);
		SAFEDELETE(flaot_Array);
		SAFEDELETE(long_Array);
		SAFEDELETE(short_Array);
		SAFEDELETE(char_Array);
		SAFEDELETE(bool_Array);
	}	
}


void CPlayer::DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB)
{
	CMoveShape::DecodeChangePropertyFromDataBlock(setReadDB);
	WORD wNum = setReadDB.GetWordFromByteArray();
	WORD pro_type = 0;
	BYTE data_type = 0;
	WORD data_length = 0;
	long data = 0;
	BYTE* byte_Array = NULL;
	WORD* word_Array = NULL;
	DWORD* dword_Array = NULL;
	float* flaot_Array = NULL;
	long*  long_Array = NULL;
	short* short_Array = NULL;
	char*  char_Array = NULL;
	bool*  bool_Array = NULL;

	for (int i = 0;i<wNum;i++)
	{	
		pro_type = setReadDB.GetWordFromByteArray();
		data_type = setReadDB.GetByteFromByteArray();
		data_length = setReadDB.GetWordFromByteArray();		
		if (data_length<=0) continue;

		//�����������ͻ�ȡ���������ֵ
		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = setReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_WORD:
			data = setReadDB.GetWordFromByteArray();
			break;
		case ATTR_DATA_DWORD:
			data = setReadDB.GetDwordFromByteArray();
			break;
		case ATTR_DATA_FLOAT:
			data = (long)setReadDB.GetFloatFromByteArray();
			break;
		case ATTR_DATA_LONG:
			data = setReadDB.GetLongFromByteArray();
			break;
		case ATTR_DATA_SHORT:
			data = setReadDB.GetShortFromByteArray();
			break;
		case ATTR_DATA_BOOL:
			data = setReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				byte_Array[j] = setReadDB.GetByteFromByteArray();
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				word_Array[j] = setReadDB.GetWordFromByteArray();
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				dword_Array[j] = setReadDB.GetDwordFromByteArray();
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			flaot_Array = new float[data_length];
			if (!flaot_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				flaot_Array[j] = setReadDB.GetFloatFromByteArray();
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				long_Array[j] = setReadDB.GetLongFromByteArray();
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				short_Array[j] = setReadDB.GetShortFromByteArray();
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			setReadDB.GetStringFromByteArray(char_Array,data_length);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				data = setReadDB.GetByteFromByteArray();
				if (data!=0)
				{
					bool_Array[j] = true;
				}else
					bool_Array[j] = false;
			}
			break;
		default:
			break;
		}						
		//������Ӧ����ֵ
		itProETN it = m_PropertyEnumToName.find(pro_type);
		if(it != m_PropertyEnumToName.end())
		{
			SetProperty( (*it).second.c_str(),data);
		}

		SAFEDELETE(byte_Array);
		SAFEDELETE(word_Array);
		SAFEDELETE(dword_Array);
		SAFEDELETE(flaot_Array);
		SAFEDELETE(long_Array);
		SAFEDELETE(short_Array);
		SAFEDELETE(char_Array);
		SAFEDELETE(bool_Array);
	}	
}

//��յ��߱���
void CPlayer::ClearCoteGoods(WORD index)
{
	if (index<0||index>=COTE_NUM)
		return ;
	if (m_Cote[index]!=NULL&&!m_Cote[index]->m_listCoteGoods.empty())
	{
		list<CPlayer::tagGoods>::iterator it = m_Cote[index]->m_listCoteGoods.begin();
		for (;it!=m_Cote[index]->m_listCoteGoods.end();it++)
		{
			SAFE_DELETE(it->pGoods);						//���ÿ�����������Ʒ����Ϣ
		}
		m_Cote[index]->m_listCoteGoods.clear();	
	}
}

//��ȡָ����ŵ��ӱ���������Ϊ��Ʒ������
CGoods* CPlayer::GetSubPGoods(WORD index)
{
	if (index>=0&&index<5&&m_Cote[index]!=NULL)
	{
		return m_Cote[index]->pGoods;
	}
	return NULL;
}	

//����ָ����ŵ��ӱ�����pGoods
void CPlayer::SetSubPGoods(WORD index,CGoods* pGoods)
{
	if (index>=0&&index<5&&m_Cote[index]!=NULL)
	{
		m_Cote[index]->pGoods = pGoods;
	}
}

//��ȡָ����ŵ��ӱ���ָ��λ�õ���Ʒ
CPlayer::tagGoods* CPlayer::GetSubGoods(WORD ExtendID,WORD lPos)
{
	if (ExtendID>=0&&ExtendID<=4)
	{
		list<tagGoods>* pLGoods = GetSubGoodslist(ExtendID);
		if (pLGoods==NULL)
		{
			return NULL;
		}
		tagGoodsIter it  = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==lPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

//��������ID��ָ����λ�û�ȡ�����ϵ���Ʒ������ԭʼ�������ӱ�����
CPlayer::tagGoods* CPlayer::GetGoodsOnPlayer(WORD ExtendID,WORD lPos)
{
	if (ExtendID==PEI_PACKET)
	{
		return GetGoods(lPos);
	}
	else if (ExtendID<=PEI_PACK5&&ExtendID>=PEI_PACK1)
	{
		return GetSubGoods(ExtendID - PEI_PACK1,lPos);
	}
	else if (ExtendID==PEI_BUSINESSPACK)
	{
		return GetBusinessGoods(lPos);
	}
	return NULL;
}

//��ȡԭʼ����ָ��λ�õ���Ʒ
CPlayer::tagGoods* CPlayer::GetGoods(WORD wPos)
{
	if (wPos >= 0 && wPos <= 20)
	{
		list<tagGoods>* pLGoods = GetGoodsList();
		if (pLGoods==NULL)
		{
			return NULL;
		}
		tagGoodsIter it = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==wPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

///��ȡ���ʱ���ָ��λ�õ���Ʒ
CPlayer::tagGoods* CPlayer::GetBusinessGoods(WORD wPos)
{
	if (wPos>=0&&wPos<=20 && m_CoteBussinesPack!=NULL)
	{
		list<tagGoods>* pLGoods = &m_CoteBussinesPack->m_listCoteGoods;
		if (pLGoods==NULL)
		{
			return NULL;
		}
		tagGoodsIter it = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==wPos)
			{
				return &(*it);
			}
		}
	}
	return NULL;
}

///��ȡ���ʱ�����λ�ϵı���
CGoods* CPlayer::GetBusinessPackGoods()
{
	if (m_CoteBussinesPack!=NULL)
	{
		return m_CoteBussinesPack->pGoods;
	}
	return NULL;
}

//���˱����Ƿ�����Ʒ��������
bool CPlayer::IsHaveLockedGoodsOnPackage(DWORD dwPacketIndex)
{
	if (dwPacketIndex!=PEI_PACKET&&dwPacketIndex!=PEI_PACK1
		&&dwPacketIndex!=PEI_PACK2&&dwPacketIndex!=PEI_PACK3
		&&dwPacketIndex!=PEI_PACK4&&dwPacketIndex!=PEI_PACK5)
		return false;
	list<CPlayer::tagGoods>* pGoodsList = NULL;
	DWORD dwIndex = 0;
	if (dwPacketIndex==PEI_PACKET)
	{
		dwIndex = 0;
	}else if (dwPacketIndex>=PEI_PACK1&&dwPacketIndex<=PEI_PACK5)
	{
		dwIndex = dwPacketIndex-PEI_PACK1+1;
	}
	pGoodsList = GetGoodsList((WORD)dwIndex);
	if (pGoodsList!=NULL)
	{
		list<CPlayer::tagGoods>::iterator GoodsIter = pGoodsList->begin();
		for (;GoodsIter!=pGoodsList->end();GoodsIter++)
		{
			if (GoodsIter->pGoods!=NULL&&GoodsIter->pGoods->IsLocked())
			{
				return true;
			}
		}
	}
	return false;
}

///�Ƿ�װ���˿��õ����ʱ���
bool CPlayer::IsHaveBusinessPack()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		return true;
	}
	return false;
}

///�������е����ʱ��������ʱ�����CGoods���󣬱����������Ʒ����û�б仯��
bool CPlayer::ClearBussinesPackGoods()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		m_CoteBussinesPack->pGoods = NULL;
		return true;
	}
	return false;
}

///�������ʱ���
void CPlayer::SetBussinesPack(CGoods*pPackGoods)
{
	if (m_CoteBussinesPack!=NULL&&pPackGoods!=NULL)
	{
		ClearBussinesPackGoods();
		m_CoteBussinesPack->pGoods = pPackGoods;
	}
}

///��ȡ���ʱ����ĸ�����
long CPlayer::GetBussinesPackDimen()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		return m_CoteBussinesPack->pGoods->GetEffectVal(GAP_PACK_CAPACITY,1);
	}
	return 0;
}

///��ȡ���ʱ������������˴��ġ�������ָ�ܴ�ŵ�������Ʒ�ܵ����������������;�ֵ�йأ�
long CPlayer::GetBussinesPackCapacity()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		return m_CoteBussinesPack->pGoods->GetEffectVal(GAP_PACK_CAPACITY,2);
	}
	return 0;
}

///��ȡ���ʱ����ĵ�ǰ����
long CPlayer::GetBussinesPackCurCapacity()
{
	long lGoodsTotalNum = 0;
	if (m_CoteBussinesPack!=NULL)
	{
		for (tagGoodsIter it=m_CoteBussinesPack->m_listCoteGoods.begin();
			it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
		{
			lGoodsTotalNum+=it->lNum;
		}
	}
	return lGoodsTotalNum;
}

///���������Ƿ�����
bool CPlayer::IsGrimFull()
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		if (GetBussinesPackDimen()>(long)m_CoteBussinesPack->m_listCoteGoods.size())
		{
			return false;
		}
	}
	return true;
}

///���������Ƿ�����
bool CPlayer::IsCapacityFull()
{
	if (m_CoteBussinesPack!=NULL)
	{
		if (GetBussinesPackCurCapacity()<GetBussinesPackCapacity())
		{
			return false;
		}
	}
	return true;
}

///��ȡ���ʱ����ĸ�������ֵ
long CPlayer::GetEffectValueOfPackGoods(WORD wEffectType,int iWhich)
{
	if (m_CoteBussinesPack!=NULL&&m_CoteBussinesPack->pGoods!=NULL)
	{
		return m_CoteBussinesPack->pGoods->GetEffectVal(wEffectType,iWhich);
	}
	return 0;
}

//��ȡָ����ŵı������ݻ����������������ӱ�����
WORD CPlayer::GetPacketDimen(DWORD dwPacketIndex)
{
	if (dwPacketIndex!=PEI_PACKET&&dwPacketIndex!=PEI_PACK1
		&&dwPacketIndex!=PEI_PACK2&&dwPacketIndex!=PEI_PACK3
		&&dwPacketIndex!=PEI_PACK4&&dwPacketIndex!=PEI_PACK5)
		return 0;
	if (dwPacketIndex==PEI_PACKET)
	{
		return GetHostPackgeDimen();
	}else if (dwPacketIndex<=PEI_PACK5&&dwPacketIndex>=PEI_PACK1)
	{
		return GetPackageDimen((WORD)dwPacketIndex-PEI_PACK1);
	}
	return 0;
}

//��ȡָ����ŵ��ӱ������ݻ�
WORD CPlayer::GetPackageDimen(WORD index)	
{
	if (index<0||index>=COTE_NUM||m_Cote[index]==NULL||m_Cote[index]->pGoods==NULL)
		return 0;
	else if(m_Cote[index]->pGoods!=NULL)
	{
		return (WORD)m_Cote[index]->pGoods->GetEffectVal(GAP_PACK_CAPACITY,1);//��ȡ���߱����ĸ�����
	}
	return 0;
}

//�ж��Ƿ����еı�����������
bool CPlayer::IsAllPackageFull()
{
	bool bResult = true;
	for (int i = 0;i<PACK_NUM;i++)
	{
		if (!IsFullForPackage(i))
		{
			bResult = false;
			break;
		}
	}
	return bResult;
}

//�ж�ĳ�������Ƿ�����(�������������ӱ�������Ŵ�0~5)
bool CPlayer::IsFullForPackage(long PackageIndex)
{
	long GoodsNum = (long)GetGoodsList((WORD)PackageIndex)->size();
	long dimension = 0;
	if (PackageIndex==0)
	{
		dimension = HOST_GRIDITEM;
	}
    else if (GetSubPGoods(WORD(PackageIndex-1)))
	{
		dimension = GetPackageDimen(WORD(PackageIndex-1));
	}
	if (GoodsNum == dimension)
	{
		return true;
	}
	return false;
}

//��ȡδ����������ţ���ԭʼ������ʼ�ƣ�
int CPlayer::GetNotFullPackage()
{
	for (WORD i= 0;i<PACK_NUM;i++)
	{
		long num = (long)GetGoodsList(i)->size();
		long dimension = 0;
		if (i == 0)
		{
			dimension = HOST_GRIDITEM;
		}
		else if (GetSubPGoods(i-1))
		{
			dimension = GetPackageDimen(i-1);
		}
		if (num != dimension)
		{
			return	i;
		}
	}
	return -1;
}

//��ȡδ���ı����ĵ�һ���ո�λ��(dwExtendID>=0&&dwExtendID<=5)
int CPlayer::GetFirstEmptyGrid(DWORD dwExtendID)									
{
	list<tagGoods>* pLGoods = GetGoodsList((WORD)dwExtendID);
	long dimension = 0;
	if (dwExtendID == 0)
	{
		dimension = HOST_GRIDITEM;
	}
	else if (GetSubPGoods((WORD)dwExtendID-1))
	{
		dimension = GetPackageDimen((WORD)dwExtendID-1);
	}
	for (int i = 0;i<dimension;i++)
	{
		tagGoodsIter it = pLGoods->begin();
		for (;it!=pLGoods->end();it++)
		{
			if (it->lPos==i)
			{
				break;
			}
		}
		if (it==pLGoods->end())
		{
			return i;
		}
	}
	return -1;
	//long* GoodsPos = NULL;
	//GoodsPos = new long[num];
	//long MinPos = 0;
	//	/*���������е���Ʒ,��ÿ����Ʒ�ڱ����е�λ�ô���һ��������*/
	//	for (int i = 0;i<num;i++)
	//	{ 
	//		GoodsPos[i] = it->lPos;
	//		it++;
	//	}
	//	/*������������ð������*/
	//	for (int j = 0;j<num-1;j++)
	//	{	
	//		for (int i = 0;i<num-1-j;i++)
	//		{
	//			if (GoodsPos[i+1]<GoodsPos[i])
	//			{
	//				MinPos = GoodsPos[i];
	//				GoodsPos[i] = GoodsPos[i+1];
	//				GoodsPos[i+1] = MinPos;
	//			}
	//		}
	//	}
	//	/*�Ƚ�������ǰ������ֵ�Ĳ�ֵ���������1����ҵ�����֮��ĵ�һ���ո�*/
	//	if (GoodsPos[0]==0)
	//	{	
	//		int i = 0;
	//		for(;i<num-1;i++)
	//		{
	//			BYTE temp = GoodsPos[i+1]-GoodsPos[i];
	//			if (temp>1)
	//			{
	//				return GoodsPos[i]+1;
	//			}
	//		}
	//		return GoodsPos[num-1]+1;
	//	}else
	//		return GoodsPos[0]-1;
}

//��ȡ�������ϵ���Ʒ���ڵ�����ID
WORD CPlayer::GetGoodsContainerID(CGoods* pGoods)
{
	if (pGoods!=NULL)
	{
		//if(m_stHand.pGoods && m_stHand.pGoods->GetExID() == pGoods->GetExID())
		//	return (WORD)PEI_HAND;

		//��װ���в���
		for(int i=0;i<CPlayer::EQUIP_SUM;i++)
		{
			CGoods* pEquip=GetEquip(i);
			if(pEquip)
			{
				if(pEquip->GetExID() == pGoods->GetExID())
				{
					return (WORD)PEI_EQUIPMENT;
				}
			}
		}
		//�ڱ����в���
		list<tagGoods>* pGoodList = GetGoodsList();
		if (pGoodList)
		{
			CGoods* ptGoods = NULL;
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				ptGoods = GoodsIter->pGoods;
				if(ptGoods!=NULL && ptGoods->GetExID() == pGoods->GetExID())
				{
					return (WORD)PEI_PACKET;
				}
			}
		}
		//��ѯ�ӱ���
		for (int i = 0;i<COTE_NUM;i++)
		{	
			//���ӱ����в���
			pGoodList = GetSubGoodslist(i);
			if (pGoodList)
			{
				CGoods* ptGoods = NULL;
				tagGoodsIter GoodsIter=pGoodList->begin();
				for(;GoodsIter!=pGoodList->end();GoodsIter++)
				{
					ptGoods = GoodsIter->pGoods;
					if(ptGoods!=NULL && ptGoods->GetExID() == pGoods->GetExID())
					{
						return (WORD)(PEI_SUBPACK+i+1);
					}
				}
			}
		}

		//�����ʱ����в���
		pGoodList = NULL;
		if (m_CoteBussinesPack!=NULL)
		{
			pGoodList = &m_CoteBussinesPack->m_listCoteGoods;
		}
		if (pGoodList)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				if(GoodsIter->pGoods!=NULL && pGoods->GetExID() == GoodsIter->pGoods->GetExID())
				{
					return PEI_BUSINESSPACK;
				}
			}
		}
	}
	return -1;
}

//������������(����ָ���������ӱ�����Χ��)�Ƿ���ĳ�����͵���Ʒ������з������ڵ�����ID
WORD CPlayer::FindContainerIDOfGoods(CGoods* pGoods)
{
	if (pGoods==NULL) return -1;

	//��ԭʼ�����в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			if(GoodsIter->pGoods!=NULL && GoodsIter->pGoods->GetProperty()->dwType == pGoods->GetProperty()->dwType)
			{
				return (WORD)PEI_PACKET;
			}
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				if(GoodsIter->pGoods!=NULL && GoodsIter->pGoods->GetProperty()->dwType == pGoods->GetProperty()->dwType)
				{
					return (WORD)(PEI_SUBPACK+i+1);
				}
			}
		}
	}
	return -1;
}

//������Ʒ����λ�ò�����Ʒ��lGoodType:1Ϊװ��2Ϊ��Ʒ lGoodId��Ʒ�����ͻ��߱�� lContainerID ��Ʒ����λ��
bool CPlayer::FindAppointGoods(long lGoodType,long lGoodId, long lGoodNum, long lContainerID)
{
	bool bIsHave=false;
	long lNum = 0;
	int iCount = 0;
	switch(lGoodType)
	{
	case 1:
		//��װ���в���
		if (lContainerID == 1)
		{
			for(int i=0;i<CPlayer::EQUIP_SUM;i++)
			{
				CGoods* pEquip = m_Equip[i];
				if (pEquip&&pEquip->GetEffectVal(GAP_WEAPON_CATEGORY,1) == lGoodId)
				{
					iCount++;
				}
			}
		}
		else if (lContainerID == 2)				//�鱳��
		{
			//��ԭʼ�����в���װ��
			list<tagGoods>* pGoodList = GetGoodsList();
			if (pGoodList)
			{
				tagGoodsIter GoodsIter=pGoodList->begin();
				for(;GoodsIter!=pGoodList->end();GoodsIter++)
				{
					CGoods* pGoods = GoodsIter->pGoods;
					if(pGoods && pGoods->GetEffectVal(GAP_WEAPON_CATEGORY,1)==lGoodId)
					{
						iCount++;
					}
				}
			}		

			//��ѯ�ӱ���װ��
			for (int i = 0;i<COTE_NUM;i++)
			{	
				//���ӱ����в���
				pGoodList = GetSubGoodslist(i);
				if (pGoodList)
				{
					tagGoodsIter GoodsIter=pGoodList->begin();
					for(;GoodsIter!=pGoodList->end();GoodsIter++)
					{
						CGoods* pGoods = GoodsIter->pGoods;
						if(pGoods && pGoods->GetEffectVal(GAP_WEAPON_CATEGORY,1)==lGoodId)
						{
							iCount++;
						}
					}
				}
			}
		}
		else			//��ֿ⣨������
			bIsHave = false;
		if(iCount>0)
			bIsHave = true;
		else
			bIsHave = false;
		break;
	case 2:
		//��װ���в�����Ʒ
		if (lContainerID == 1)
		{
			for(int i=0;i<CPlayer::EQUIP_SUM;i++)
			{
				CGoods* pEquip = m_Equip[i];
				if (pEquip&&pEquip->GetProperty()->dwIndex == lGoodId)
				{
					lNum += pEquip->GetNum();
					bIsHave = true;
				}
			}
		}
		else if (lContainerID == 2)				//�鱳��
		{
			//��ԭʼ�����в�����Ʒ
			list<tagGoods>* pGoodList = GetGoodsList();
			if (pGoodList)
			{
				tagGoodsIter GoodsIter=pGoodList->begin();
				for(;GoodsIter!=pGoodList->end();GoodsIter++)
				{
					CGoods* pGoods = GoodsIter->pGoods;
					if(pGoods && pGoods->GetProperty()->dwIndex == lGoodId)
					{
						lNum += pGoods->GetNum();
						iCount++;
					}
				}
			}		

			//��ѯ�ӱ�����Ʒ
			for (int i = 0;i<COTE_NUM;i++)
			{	
				//���ӱ����в���
				pGoodList = GetSubGoodslist(i);
				if (pGoodList)
				{
					tagGoodsIter GoodsIter=pGoodList->begin();
					for(;GoodsIter!=pGoodList->end();GoodsIter++)
					{
						CGoods* pGoods = GoodsIter->pGoods;
						if(pGoods && pGoods->GetProperty()->dwIndex == lGoodId)
						{
							lNum += pGoods->GetNum();
							iCount++;
						}
					}
				}
			}
		}
		else 			//��ֿ⣨������
			bIsHave = false;
		if(iCount>0)
			bIsHave = true;
		else
			bIsHave = false;
		break;
	}
	if(bIsHave && lNum >= lGoodNum)		// �ҵ���������Ҫ�����Ʒ
		return true;
	else if(bIsHave && lNum == 0)		// �ҵ�����
		return true;
	else
		return false;
}
//ͨ��λ�ò�����Ʒ
CGoods* CPlayer::GetGoodsByPosition(long lExtendID,DWORD dwPos)
{
	CGoods* pGoods = NULL;
	if (lExtendID==PEI_PACKET&&dwPos>=0&&dwPos<HOST_GRIDITEM)
	{
		tagGoods* pstGoods = GetGoods((WORD)dwPos);
		if (pstGoods)
		{
			pGoods = pstGoods->pGoods;
		}
	}
	else if (lExtendID==PEI_BUSINESSPACK&&dwPos>=0&&dwPos<HOST_GRIDITEM)
	{
		tagGoods* pstGoods = GetBusinessGoods((WORD)dwPos);
		if (pstGoods)
		{
			pGoods = pstGoods->pGoods;
		}
	}
	else if (lExtendID<=PEI_PACK5&&lExtendID>=PEI_PACK1)
	{
		WORD wGridNum = GetPackageDimen((WORD)lExtendID-PEI_PACK1);
		if (dwPos>=0&&dwPos<wGridNum)
		{
			tagGoods* pstGoods = GetSubGoods((WORD)(lExtendID-PEI_PACK1),(WORD)dwPos);
			if (pstGoods)
			{
				pGoods = pstGoods->pGoods;
			}
		}
	}else if (lExtendID==PEI_EQUIPMENT&&dwPos>=0&&dwPos<EQUIP_SUM)
	{
		pGoods = m_Equip[dwPos];
	}
	// ���壺�ڲֿ��в���
	else if( CGoods* pGoodsInDeopt = CGCMana::Inst()->GetGoods( lExtendID, dwPos ) )
	{
		pGoods = pGoodsInDeopt;
	}

	return pGoods;
}

//����ID��������Ŀ���������ѧ��������Ŀ�б���
bool CPlayer::AddFactureSkill(DWORD dwFactureID)
{
	for(int i = 0; i < (int)m_vecFactureEntrys.size(); i++)
	{
		if (m_vecFactureEntrys[i]==dwFactureID)
		{
			return false;
		}
	}
	m_vecFactureEntrys.push_back(dwFactureID);
	return true;
}

// ɾ��ĳ����ѧ���������Ŀ
void CPlayer::DeleteFactureSkill(DWORD dwFactureID)
{
	vector<DWORD>::iterator FactureSkillIter = m_vecFactureEntrys.begin();
	for (;FactureSkillIter!=m_vecFactureEntrys.end();FactureSkillIter++)
	{
		if ((*FactureSkillIter)==dwFactureID)
		{
			m_vecFactureEntrys.erase(FactureSkillIter);
			return;
		}
	}
}

// ɾ����ѧ���ȫ��������Ŀ
void CPlayer::DeleteAllFactureSkill()
{
	m_vecFactureEntrys.clear();
}

//��ȡĳ����������Ʒ��������ԭʼ������
list<CPlayer::tagGoods>* CPlayer::GetGoodsList(WORD index)
{
	if (index<0||index>=PACK_NUM)
	{
		return NULL;
	}else if(index==0)
	{
		return &m_listGoods;
	}else
	{
		return GetSubGoodslist(index-1);
	}
}

//��ȡĳ����������Ʒ����������ԭʼ������
list<CPlayer::tagGoods>* CPlayer::GetSubGoodslist(WORD index) 
{
	if (index<0||index>=COTE_NUM||m_Cote[index]==NULL)
	{
		return	NULL;
	}
	return &m_Cote[index]->m_listCoteGoods;
}

//��ȡ����ָ����ŵ��ӱ���
CPlayer::tagCote* CPlayer::GetCotePackage(WORD index) 
{
	if (index<0||index>=COTE_NUM)
	{
		return	NULL;
	}
	return m_Cote[index];
}										

// ��ӵ�CByteArray
bool CPlayer::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	CMoveShape::AddToByteArray(pByteArray, bExData);

	if( bExData )
	{
		_AddToByteArray(pByteArray, (BYTE*)&m_Property, (long)sizeof(tagPPro));
		_AddToByteArray(pByteArray, (BYTE*)&m_FightProperty, (long)sizeof(tagPFPro));
		_AddToByteArray(pByteArray, GetAccount());
		_AddToByteArray(pByteArray, GetTitle());
		//_AddToByteArray(pByteArray, &m_Property, (long)sizeof(tagPPro));
		//_AddToByteArray(pByteArray, m_dwNextExp);

		//m_listSkillID
		_AddToByteArray(pByteArray, (long)m_listSkillID.size());
		list<tagSkill>::iterator SkillIter;
		for(SkillIter=m_listSkillID.begin();SkillIter!=m_listSkillID.end();SkillIter++)
		{
			_AddToByteArray(pByteArray, (BYTE*)&(*SkillIter), (long)sizeof(tagSkill));
		}

		//m_listGoods
		_AddToByteArray(pByteArray,(long)m_listGoods.size());//��Ʒ����
		tagGoodsIter GoodIter;
		for(GoodIter=m_listGoods.begin();GoodIter!=m_listGoods.end();GoodIter++)
		{
			//_AddToByteArray(pByteArray,(BYTE*)&(*GoodIter),long(sizeof(bool)+sizeof(BYTE)*2));//��ѹǰ3���ֽ�

			_AddToByteArray(pByteArray, (BYTE)GoodIter->bChange);
			_AddToByteArray(pByteArray, (WORD)GoodIter->lNum);
			_AddToByteArray(pByteArray, (BYTE)GoodIter->lPos);
			if(GoodIter->bChange)
			{
				_AddToByteArray(pByteArray,GoodIter->pGoods->GetIndex());//ѹ����ƷINDEX
				_AddToByteArray(pByteArray,(long)(GoodIter->pGoods->GetEffect()->size()));//��ѹ����Ʒ��EFFECT����
				vector<CGoodsList::tagEffect>::iterator EffectIter;
				for(EffectIter=GoodIter->pGoods->GetEffect()->begin();EffectIter!=GoodIter->pGoods->GetEffect()->end();EffectIter++)
				{
					_AddToByteArray(pByteArray,(BYTE*)&(*EffectIter),(long)sizeof(CGoodsList::tagEffect));
				}
			}			
		}

		//// ����
		//_AddToByteArray(pByteArray, GetCountry() );
		//_AddToByteArray(pByteArray, GetCountryContribute() );
	}

	return true;
}



/*
* ����: ����
* ժҪ: -
* ����: pByte   - 
*		 pos	 - 
*		 bExData - 
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.09.16 - lpf
*		�����˶��Ƿ���ʾ���ұ�־�Ľ���
*	2009.03.13 - lpf
*		�����˶�ʱװ��Ľ���,����ʼ���˽��水ť״̬
*	2009.03.20 - lpf
*		ȡ���˽���"ʱװ��"ʱ,����Ϸ���ý��水ť״̬�ĳ�ʼ������
*	2009.04.10- lpf
* 		����"�趨��ҹ��ҹ��׳�ʼֵ";
*/
// �˽ӿ�û��ʹ���ˣ��ο�DecodeFromDataBlock
bool CPlayer::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	m_listSkillID.clear();
	m_SuitList.clear();
	//����ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{
		if (m_Cote[i]!=NULL&&!m_Cote[i]->m_listCoteGoods.empty())
		{
			tagGoodsIter it = m_Cote[i]->m_listCoteGoods.begin();
			for (;it!=m_Cote[i]->m_listCoteGoods.end();it++)
			{
				SAFE_DELETE(it->pGoods);						//��ձ������ÿ����Ʒ
			}
			m_Cote[i]->m_listCoteGoods.clear();				
		}
	}
	//���ԭʼ����
	if (!m_listGoods.empty())
	{
		tagGoodsIter it = m_listGoods.begin();
		for (;it != m_listGoods.end();it++)
		{
			SAFE_DELETE(it->pGoods);					
		}
		m_listGoods.clear();									//���ԭʼ����
	}

	//������ʱ���
	if (m_CoteBussinesPack!=NULL&&!m_CoteBussinesPack->m_listCoteGoods.empty())
	{
		tagGoodsIter it = m_CoteBussinesPack->m_listCoteGoods.begin();
		for (;it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
		{
			SAFE_DELETE(it->pGoods);						//��ձ������ÿ����Ʒ
		}
		m_CoteBussinesPack->m_listCoteGoods.clear();				
	}

	m_listStateEx.clear();

	CMoveShape::DecordFromByteArray(pByte, pos, bExData);

	if( bExData )
	{
		//int n  = (long)sizeof(m_Property);

		_GetBufferFromByteArray(pByte, pos, &m_Property, (long)sizeof(tagPPro));

		DecordChangeProperty(pByte,pos);

		// ʱװ��
		SetShowFashion(_GetByteFromByteArray(pByte, pos));

		// �趨��ҹ��ҹ��׳�ʼֵ
		SetOldContribute(GetCountryContribute());

		// skill
		long lNum = _GetLongFromByteArray(pByte, pos);
		m_wPublicCoolTime = _GetWordFromByteArray(pByte, pos);
		tagSkill stSkill;
		for(int i=0; i<lNum; i++)
		{
			stSkill.dwID = _GetDwordFromByteArray(pByte, pos);
			stSkill.wLevel = _GetWordFromByteArray(pByte, pos);
			stSkill.dwRestoreTime = _GetDwordFromByteArray(pByte, pos);
			stSkill.fAffectRangeMin = _GetFloatFromByteArray(pByte, pos);
			stSkill.fAffectRangeMax = _GetFloatFromByteArray(pByte, pos);
			stSkill.wCostMP = _GetWordFromByteArray(pByte, pos);
			DWORD	dwTime = _GetDwordFromByteArray(pByte, pos);
			if(stSkill.dwRestoreTime > 0)
			{
				stSkill.dwRestoreTime += dwTime;
				stSkill.dwCoolDownStartTime = timeGetTime() - dwTime;
			}
			// ս�����ܲ�ѹ��
			if(stSkill.dwID < SKILL_DEPUTY_OCCU)
			{
				m_listSkillID.push_back(stSkill);
				//IsAddOccupedList(stSkill.dwID);
			}
		}
		SortSkillList();

		//�������ҵϵͳ�е�����
		lNum = _GetDwordFromByteArray(pByte,pos);
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwIndexOfZone = _GetDwordFromByteArray(pByte,pos);
			DWORD dwCreditForZoneValue = _GetDwordFromByteArray(pByte,pos);
			m_mapPlayerCreditForZone[dwIndexOfZone] = dwCreditForZoneValue;
		}

		map<DWORD,DWORD> tempGoodsCoolTime;
		/// ������Ʒ��������ȴ���͵���ȴʱ��
		lNum = _GetDwordFromByteArray(pByte,pos);
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwCoolDowmID = _GetDwordFromByteArray(pByte,pos);
			DWORD dwCoolDownReTime = _GetDwordFromByteArray(pByte,pos);
			tempGoodsCoolTime.insert(pair<DWORD,DWORD>(dwCoolDowmID,dwCoolDownReTime));
		}

		/// ���뵥����Ʒ��ȴʱ��
		lNum = _GetDwordFromByteArray(pByte,pos);
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwGoodsID = _GetDwordFromByteArray(pByte,pos);
			DWORD dwCoolDownTime = _GetDwordFromByteArray(pByte,pos);
			AddShortCutGoods((WORD)dwGoodsID);
			SetGoodsRemainTime((WORD)dwGoodsID,dwCoolDownTime);
		}


		//equipments
		bool bVal=false;
		lNum=_GetWordFromByteArray(pByte,pos);
		for(int i=0;i<lNum;i++)
		{
			CGoods *pGood=new CGoods;
			bVal = pGood->DecordFromByteArray(pByte, pos);
			WORD lIndex = _GetWordFromByteArray(pByte,pos);
			if(bVal&&lIndex>=0&&lIndex<EQUIP_SUM)
			{
				m_Equip[lIndex] = pGood;
				WORD SuitID = (WORD)pGood->GetEffectVal(GAP_SUITID,1);			
				if (SuitID!=0)
				{
					string stOrName = pGood->GetProperty()->strOrginName;					
					AddSuit(SuitID,stOrName);	
					SetupEquip(SuitID,stOrName);
				}
				pGood = NULL;
			}
			else
				SAFE_DELETE(pGood);
		}

		//ѫ���б�
		//GetGame()->GetCGuiEx()->GetCharacterPageEx()->GetFormByteArrayAndInit(pByte, pos);

		//Goods
		//��ʼ��ԭʼ����
		lNum = _GetLongFromByteArray(pByte,pos);
		if (lNum>0)
		{
			for (int i = 0;i<lNum;i++)
			{
				tagGoods stGood;
				stGood.bChange=_GetByteFromByteArray(pByte,pos) != 0;
				stGood.lNum = _GetWordFromByteArray(pByte,pos);
				stGood.lPos = _GetByteFromByteArray(pByte,pos);
				stGood.lType = PEI_PACKET;
				stGood.pGoods = new CGoods;
				bVal=stGood.pGoods->DecordFromByteArray(pByte, pos);
				if (CGoodsList::GetProperty(stGood.pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE)
				{
					AddShortCutGoods((WORD)stGood.pGoods->GetIndex());
					/// ��ȡ������Ʒ����ȴ����
					DWORD dwCoolType = CGoodsList::GetEffectVal(stGood.pGoods->GetIndex(),GAP_COOLDOWN,2);
					map<DWORD,DWORD>::const_iterator CoolTimeIter = tempGoodsCoolTime.find(dwCoolType);
					if (CoolTimeIter!=tempGoodsCoolTime.end())
					{
						SetGoodsRemainTime((WORD)stGood.pGoods->GetIndex(),CoolTimeIter->second);
					}
					DWORD dwRemainTime = GetGoodsRemainTime((WORD)stGood.pGoods->GetIndex());
					if (dwRemainTime!=0)
					{
						DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)stGood.pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
						SetGoodsCoolDownStartTime((WORD)stGood.pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
						SetGoodsUseState((WORD)stGood.pGoods->GetIndex(),false);
					}
				}
				if(bVal)
				{
					m_listGoods.push_back(stGood);
					WORD SuitID = (WORD)stGood.pGoods->GetEffectVal(GAP_SUITID,1);			
					if (SuitID!=0)
					{
						string stOrName = stGood.pGoods->GetProperty()->strOrginName;					
						AddSuit(SuitID,stOrName);	
						//SetupEquip(SuitID,stOrName);
					}
					stGood.pGoods = NULL;
				}
				else
				{
					SAFE_DELETE(stGood.pGoods);
				}
			}
		}

		//##��ʼ��ÿ��������λ������ֵ
		BYTE lnum = _GetByteFromByteArray(pByte,pos);					    //���߱�����
		for (int i = 0;i<lnum;i++)
		{
			CGoods *pGood=new CGoods;
			bVal=pGood->DecordFromByteArray(pByte, pos);
			BYTE index=_GetByteFromByteArray(pByte,pos);
			if(bVal&&m_Cote[index]!=NULL)
			{
				m_Cote[index]->pGoods = pGood;							//pGoods�а����˵��߱����Ĵ�С������
				pGood = NULL;
			}
			else
				SAFE_DELETE(pGood);	
		}
		//##��ʼ��ÿ�����������Ʒ����Ϣ������ÿ��������λ��
		for (int i = 0;i<COTE_NUM;i++)
		{	
			lNum=_GetLongFromByteArray(pByte,pos);
			if (lNum>0)
			{
				for(int j=0;j<lNum;j++)
				{
					tagGoods stGood;
					stGood.bChange=_GetByteFromByteArray(pByte,pos) != 0;
					stGood.lNum = _GetWordFromByteArray(pByte,pos);
					stGood.lPos = _GetByteFromByteArray(pByte,pos);
					stGood.lType = PEI_PACK1+i;
					stGood.pGoods = new CGoods;
					bVal = stGood.pGoods->DecordFromByteArray(pByte, pos);
					if (CGoodsList::GetProperty(stGood.pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE)
					{
						AddShortCutGoods((WORD)stGood.pGoods->GetIndex());
						/// ��ȡ������Ʒ����ȴ����
						DWORD dwCoolType = CGoodsList::GetEffectVal(stGood.pGoods->GetIndex(),GAP_COOLDOWN,2);
						map<DWORD,DWORD>::const_iterator CoolTimeIter = tempGoodsCoolTime.find(dwCoolType);
						if (CoolTimeIter!=tempGoodsCoolTime.end())
						{
							SetGoodsRemainTime((WORD)stGood.pGoods->GetIndex(),CoolTimeIter->second);
						}
						DWORD dwRemainTime = GetGoodsRemainTime((WORD)stGood.pGoods->GetIndex());
						if (dwRemainTime!=0)
						{
							DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)stGood.pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
							SetGoodsCoolDownStartTime((WORD)stGood.pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
							SetGoodsUseState((WORD)stGood.pGoods->GetIndex(),false);
						}
					}
					if(bVal)
					{
						m_Cote[i]->m_listCoteGoods.push_back(stGood);
						WORD SuitID = (WORD)stGood.pGoods->GetEffectVal(GAP_SUITID,1);			
						if (SuitID!=0)
						{
							string stOrName = stGood.pGoods->GetProperty()->strOrginName;					
							AddSuit(SuitID,stOrName);	
							//SetupEquip(SuitID,stOrName);
						}
						stGood.pGoods = NULL;
					}
					else
					{
						SAFE_DELETE(stGood.pGoods);
					}
				}
			}
		}


		// #���
		lNum = _GetLongFromByteArray(pByte, pos);
		CGUID guID;
		_GetBufferFromByteArray(pByte, pos, guID);
		SetMoney(lNum);
		SetguMoneyID(guID);

		// #����
		lNum = _GetLongFromByteArray(pByte, pos);
		_GetBufferFromByteArray(pByte, pos, guID);
		SetSilverMoney(lNum);
		SetguSilverMoneyID(guID);

		// ��ʯ
		lNum = _GetLongFromByteArray(pByte, pos);
		_GetBufferFromByteArray(pByte, pos, guID);
		SetBaoShi(lNum);
		SetguBaoShiID(guID);

		// #����
		lNum = _GetLongFromByteArray(pByte, pos);
		//_GetBufferFromByteArray(pByte, pos, guID);
		SetJiFen(lNum);
		//SetguJiFenID(guID);

		// �ֿ���Ϣͷ ����
		//GetGame()->GetCGuiEx()->GetDepotPageEx()->DecordDepotHeadInfo( pByte, pos );

		// ��֯ϵͳ
		DecordOrgSysFromByteArray(pByte,pos);

		//��ǰʹ�õĳƺ�
		long lTitleType = _GetLongFromByteArray(pByte,pos);
		SetTitleType(lTitleType);
		//GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetTitleType(lTitleType);
		char strTitleName[1024];
		string strTitlle = _GetStringFromByteArray(pByte,pos,strTitleName);
		SetPlayerTitle(strTitlle);
		//GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetPlayerTitle(strTitlle);


		//С����ϵͳ
		//GetGame()->GetCGuiEx()->GetAutoPageEx()->GetFormByteArrayAndInit(pByte,pos);

		// 
		m_PlayerSetupOnServer.GetFormByteArray( pByte, pos );

		//�õ���������
		//DecordQuestDataFromByteArray(pByte,pos);
		//-----07.9.6�����޸�------
		m_RoleQueue.DecordQuestDataFromByteArray(pByte,pos);
		//////////��ְҵϵͳ������//////////
		m_dwCollectExp = _GetLongFromByteArray(pByte,pos);
		m_dwCollectLevel = _GetLongFromByteArray(pByte,pos);
		m_dwFactureExp=_GetLongFromByteArray(pByte,pos);
		m_dwFactureLevel=_GetLongFromByteArray(pByte,pos);
		lNum =_GetLongFromByteArray(pByte,pos);
		for (int i=0; i<lNum; i++)
		{
			DWORD dwFactureID =_GetLongFromByteArray(pByte,pos);
			m_vecFactureEntrys.push_back(dwFactureID);
		}

		long lLev = _GetLongFromByteArray(pByte,pos);
		long lID = _GetLongFromByteArray(pByte,pos);

		GetGame()->GetMainPlayer()->SetLimtSkill(lID,lLev);

		// ��ļ��Ϣ
		DecordRecruitDataFromByteArray(pByte, pos);
		////////////////////////////////////
		/*CRoleRecord *pQuest = new CRoleRecord(77,1);
		m_RoleQueue.AddOneRecord(pQuest);*/
		//-------------------------


		//GetGame()->GetInterface()->GetContainerPage()->SetGoods(NULL);
	}
	else
	{
		// װ�����
		SetHairPic(_GetByteFromByteArray(pByte, pos));
		SetShowFashion(_GetByteFromByteArray(pByte, pos));

		// װ�����
        for(long l=EQUIP_HEAD; l<EQUIP_SUM; l++)
        {
            m_dwEquipIndex[l] = _GetDwordFromByteArray(pByte, pos);
            m_btEquipLevel[l] = _GetLongFromByteArray(pByte, pos);
        }

		// �Ƿ���ʾ���ұ�־
		SetShowCountry(_GetDwordFromByteArray(pByte, pos) != 0 ? true : false);

		SetHp(_GetDwordFromByteArray(pByte, pos));
		SetMaxHp(_GetDwordFromByteArray(pByte, pos));
		//--------07.8.3ȡ��ע��(����)---------
		SetPkCount(_GetDwordFromByteArray(pByte, pos));
		SetPkValue(_GetByteFromByteArray(pByte, pos) != 0);

		//��֯ϵͳ
		DecordOrgSysFromByteArray(pByte,pos);

		//�ƺ�ϵͳ
		long lTitleType = _GetLongFromByteArray(pByte,pos);
		SetTitleType(lTitleType);
		char strTitleName[1024];
		string strTitlle = _GetStringFromByteArray(pByte,pos,strTitleName);
		SetPlayerTitle(strTitlle); 

		//ְҵ+�Ա�
		SetOccupation(_GetByteFromByteArray(pByte, pos));
		SetSex(_GetByteFromByteArray(pByte, pos));
		SetAssOccu(_GetByteFromByteArray(pByte,pos));

		// PKֵ �� ����ֵ
		this->SetPkValue( _GetDwordFromByteArray( pByte, pos ) );
		this->SetPVPValue( _GetDwordFromByteArray( pByte, pos ) );

		//
		_GetBufferFromByteArray(pByte, pos,m_PlayerShopSessionID);
		m_lPlayerShopPlugID=_GetLongFromByteArray(pByte,pos);
		char strPlayerShopName[17];
		memset(strPlayerShopName,0,sizeof(strPlayerShopName));
		_GetStringFromByteArray(pByte,pos,strPlayerShopName);
		if(m_PlayerShopSessionID != CGUID::GUID_INVALID && m_lPlayerShopPlugID)
		{
			this->SetPlayerShopName(strPlayerShopName);
			this->SetPlayerShopOpen(true);
		}
		else
		{
			this->SetPlayerShopOpen(false);
		}
		DWORD dwEmotionIndex=_GetDwordFromByteArray(pByte, pos);
		DWORD dwTime=_GetDwordFromByteArray(pByte, pos);
		if(dwEmotionIndex)
		{
			SetEmotion(dwEmotionIndex-1,dwTime);
		}
		// ����ID
		SetCountry( _GetByteFromByteArray(pByte,pos));
		SetFacePic(_GetByteFromByteArray(pByte, pos));
		SetLevel(_GetWordFromByteArray(pByte, pos));
		//SetRanOfNobCredit(_GetLongFromByteArray(pByte, pos));
		// ��ļ��Ϣ
		DecordRecruitDataFromByteArray(pByte, pos);
	}
	return true;
}

// ��ļ��Ϣ
void CPlayer::DecordRecruitDataFromByteArray(BYTE* pByte, long &pos)
{
	// ��ļ״̬
	long lFlag = _GetLongFromByteArray(pByte, pos);
	// ֹͣ��
	if(lFlag==0)
	{
		SetRecruitingTeam(false);
		return;
	}
	long lMemNum = _GetLongFromByteArray(pByte, pos);
	SetTeammateAmount(lMemNum);
	char strTeamName[20];
	memset(strTeamName,0,sizeof(strTeamName));
	_GetStringFromByteArray(pByte,pos,strTeamName);
	SetRecruitingTeam(true);
	SetTeamname(strTeamName);
	long lHasPass = _GetLongFromByteArray(pByte, pos);
	SetTeamHasPassword((lHasPass)==0?false:true);
}

bool CPlayer::DecordOrgSysFromByteArray(BYTE* pByte, long& pos)
{
	// �л������ GUID
	_GetBufferFromByteArray(pByte, pos,m_FactionID);
	_GetBufferFromByteArray(pByte, pos,m_UnionID);

	if((BOOL)_GetLongFromByteArray(pByte, pos))
	{
		LONG lFacDemiseWarning = 0;
		LONG lFacDisbandWarning = 0;

		if( this->IsSelf() )
		{
			lFacDemiseWarning = _GetLongFromByteArray( pByte, pos ); //! ��ᴫλ���ѵ���ʱ
			lFacDisbandWarning = _GetLongFromByteArray( pByte, pos ); //! ����ɢ���ѵ���ʱ
		}

		// �ƺ�
		TCHAR szFactionTitle[ MAX_MEMBER_NAME_SIZE ];
		_GetStringFromByteArray( pByte, pos, szFactionTitle );

		// ��ȡPUBLIC��Ϣ
		tagFacPublicData stFacPublicData;
		_GetBufferFromByteArray( pByte, pos, &stFacPublicData, sizeof(tagFacPublicData) );

		// ��ȡ�ٽ�
		m_lFactionJob = _GetLongFromByteArray( pByte, pos );

		// ��ȡȨ������
		long lMyJobPurview = _GetLongFromByteArray( pByte, pos );

		// ����
		TCHAR	szPronounce[ MAX_PronounceCharNum ];
		_GetStringFromByteArray( pByte, pos, szPronounce );

		// ������ƺͳƺ�
		m_strFactionName = stFacPublicData.szName;
		m_strFactionTitle = szFactionTitle;

		// ����һ���л�
		CFactionManager*	pFactionManager = GetGame()->GetFactionManager();

        CFaction::SFacInfo	stFacInfo;
		ZeroMemory( &stFacInfo, sizeof(stFacInfo) );
		stFacInfo.guidFac = stFacPublicData.guid;
		stFacInfo.strFacName = stFacPublicData.szName;
		stFacInfo.strPlacard = szPronounce;
		stFacInfo.arrLevel[ CFaction::FUN_FACTION ] = stFacPublicData.lLevel;

		stFacInfo.tMasterLastOnlineTime = (time_t)lFacDemiseWarning;
		stFacInfo.tLessMemNumTime = (time_t)lFacDisbandWarning;

		if( pFactionManager->AddFaction( &stFacInfo ) )
			pFactionManager->GetFaction( stFacPublicData.guid )->SetJobPurview( m_lFactionJob, lMyJobPurview );
	}

	return true;
}



/*
* ����: �µĽ���
* ժҪ: -
* ����: setReadDB   - 
*		 bExData - 
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2009.03.13 - lpf
*		�����˶�ʱװ��Ľ���,����ʼ���˽��水ť״̬
*	2009.03.20 - lpf
*		ȡ���˽���"ʱװ��"ʱ,����Ϸ���ý��水ť״̬�ĳ�ʼ������
*	2009.04.10- lpf
* 		����"�趨��ҹ��ҹ��׳�ʼֵ";
*/
bool CPlayer::DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData )
{
	m_listSkillID.clear();
    m_mapBrotherInfo.clear();
	m_SuitList.clear();
	//��յ��߱���
	for (int i = 0;i<COTE_NUM;i++)
	{
		if (m_Cote[i]!=NULL&&!m_Cote[i]->m_listCoteGoods.empty())
		{
			tagGoodsIter it = m_Cote[i]->m_listCoteGoods.begin();
			for (;it!=m_Cote[i]->m_listCoteGoods.end();it++)
			{
				SAFE_DELETE(it->pGoods);						//���ÿ�����������Ʒ����Ϣ
			}
			m_Cote[i]->m_listCoteGoods.clear();				
		}
	}
	//���ԭʼ����
	if (!m_listGoods.empty())
	{
		tagGoodsIter it = m_listGoods.begin();
		for (;it != m_listGoods.end();it++)
		{
			SAFE_DELETE(it->pGoods);					
		}
		m_listGoods.clear();									//���ԭʼ����
	}

	//������ʱ���
	if (m_CoteBussinesPack!=NULL&&!m_CoteBussinesPack->m_listCoteGoods.empty())
	{
		tagGoodsIter it = m_CoteBussinesPack->m_listCoteGoods.begin();
		for (;it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
		{
			SAFE_DELETE(it->pGoods);						//��ձ������ÿ����Ʒ
		}
		m_CoteBussinesPack->m_listCoteGoods.clear();				
	}

	// �ͷ�װ��
	for(int i=0;i<EQUIP_SUM;i++)
	{
		if(i!=EQUIP_MEDAL1&&i!=EQUIP_MEDAL2&&i!=EQUIP_MEDAL3)  //ѫ�µ����ͷ�
		{
			SAFE_DELETE(m_Equip[i]);
		}
		else
			m_Equip[i] = NULL;
	}

	m_listStateEx.clear();

	m_vecFactureEntrys.clear();

	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);

	if( bExData )
	{
		setReadDB.GetBufferFromByteArray(&m_Property, (long)sizeof(tagPPro));

		DecodeChangePropertyFromDataBlock(setReadDB);

		// ���壺ħ������
		setReadDB.GetBufferFromByteArray( m_arrElementProperty, sizeof(m_arrElementProperty) );

		// ʱװ��
		SetShowFashion(setReadDB.GetByteFromByteArray());

		// �趨��ҹ��ҹ��׳�ʼֵ
		SetOldContribute(GetCountryContribute());

		// skill
		long lNum = setReadDB.GetLongFromByteArray();
		m_wPublicCoolTime = setReadDB.GetWordFromByteArray();
		tagSkill stSkill;
		for(int i=0; i<lNum; i++)
		{
			stSkill.dwID = setReadDB.GetDwordFromByteArray();
			stSkill.wLevel = setReadDB.GetWordFromByteArray();
			stSkill.dwRestoreTime = setReadDB.GetDwordFromByteArray();
            // ���ܷ�Χ��Ϊfloat TODO:�޸Ŀͻ��˼��ܷ�Χ����߼�
			stSkill.fAffectRangeMin = setReadDB.GetFloatFromByteArray();
			stSkill.fAffectRangeMax = setReadDB.GetFloatFromByteArray();
			stSkill.wCostMP = setReadDB.GetWordFromByteArray();
			DWORD	dwTime = setReadDB.GetDwordFromByteArray();
			if(stSkill.dwRestoreTime > 0)
			{
				stSkill.dwRestoreTime += dwTime;
				stSkill.dwCoolDownStartTime = timeGetTime() - dwTime;
			}
			// ս�����ܲ�ѹ��
			if(stSkill.dwID < SKILL_DEPUTY_OCCU)
			{
				m_listSkillID.push_back(stSkill);
				//IsAddOccupedList(stSkill.dwID);
			}
		}
		SortSkillList();

#define _DECODE_GENIUS_

#ifdef  _DECODE_GENIUS_
        /// �����츳�������
        DecodeGeniusFromDataBlock(setReadDB);
#endif
        // �������
        long lBrotherNum = setReadDB.GetLongFromByteArray();
        tagBrotherInfo stBrother;
        for(long l=0; l<lBrotherNum; l++)
        {
            setReadDB.GetBufferFromByteArray(stBrother.guidBrother);
            setReadDB.GetLongFromByteArray();
            m_mapBrotherInfo[stBrother.guidBrother] = stBrother;
        }

		//�������ҵϵͳ�е�����
		lNum = setReadDB.GetDwordFromByteArray();
		SetCreditNum(lNum);
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwIndexOfZone = setReadDB.GetDwordFromByteArray();
			DWORD dwCreditForZoneValue = setReadDB.GetDwordFromByteArray();
			m_mapPlayerCreditForZone[dwIndexOfZone] = dwCreditForZoneValue;
		}

		/// ������Ʒ��������ȴ���͵���ȴʱ��
		lNum = setReadDB.GetDwordFromByteArray();
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwCoolDowmID = setReadDB.GetDwordFromByteArray();
			DWORD dwCoolDownReTime = setReadDB.GetDwordFromByteArray();
			AddPublicCooldownType((WORD)dwCoolDowmID,dwCoolDownReTime);
		}

		/// ���뵥����Ʒ��ȴʱ��
		lNum = setReadDB.GetDwordFromByteArray();
		for (int i = 0;i<lNum;i++)
		{
			DWORD dwGoodsID = setReadDB.GetDwordFromByteArray();
			DWORD dwCoolDownTime = setReadDB.GetDwordFromByteArray();
			AddShortCutGoods((WORD)dwGoodsID);
			SetGoodsRemainTime((WORD)dwGoodsID,dwCoolDownTime);
		}

		//equipments
		bool bVal=false;
		lNum=setReadDB.GetWordFromByteArray();
		for(int i=0;i<lNum;i++)
		{
			CGoods *pGood=new CGoods;
			bVal = pGood->DecodeFromDataBlock(setReadDB,bExData);
			WORD lIndex = setReadDB.GetWordFromByteArray();
			if(bVal&&lIndex>=0&&lIndex<EQUIP_SUM)
			{
				m_Equip[lIndex] = pGood;
				WORD SuitID = (WORD)pGood->GetEffectVal(GAP_SUITID,1);			
				if (SuitID!=0)
				{
					string stOrName = pGood->GetProperty()->strOrginName;					
					AddSuit(SuitID,stOrName);	
					SetupEquip(SuitID,stOrName);
				}
				pGood = NULL;
			}
			else
				SAFE_DELETE(pGood);
		}

        DWORD dwPos = 0;
        lNum = setReadDB.GetDwordFromByteArray();        
        for(int i=0; i<lNum; i++)
        {
            dwPos = setReadDB.GetByteFromByteArray();
            CGoods* pGoods = new CGoods;
            pGoods->DecodeFromDataBlock(setReadDB, bExData);
            m_eEquipGod[dwPos] = pGoods;
        }

        lNum = setReadDB.GetDwordFromByteArray();        
        for(int i=0; i<lNum; i++)
        {
            dwPos = setReadDB.GetByteFromByteArray();
            CGoods* pGoods = new CGoods;
            pGoods->DecodeFromDataBlock(setReadDB, bExData);
            m_eEquipLand[dwPos] = pGoods;
        }

        lNum = setReadDB.GetDwordFromByteArray();        
        for(int i=0; i<lNum; i++)
        {
            dwPos = setReadDB.GetByteFromByteArray();
            CGoods* pGoods = new CGoods;
            pGoods->DecodeFromDataBlock(setReadDB, bExData);
            m_eEquipHuman[dwPos] = pGoods;
        }

		//ѫ���б�
		//GetGame()->GetCGuiEx()->GetMadalPageEx()->DecodeFromDataBlockAndInit(setReadDB);
		DecodeMedalFromDataBlockAndInit(setReadDB);

		long lTest = setReadDB.GetLongFromByteArray();
		//Goods
		//��ʼ��ԭʼ����
		lNum = setReadDB.GetLongFromByteArray();
		if (lNum>0)
		{
			for (int i = 0;i<lNum;i++)
			{
				tagGoods stGood;
				stGood.bChange=setReadDB.GetByteFromByteArray() != 0;
				stGood.lNum = setReadDB.GetWordFromByteArray();
				stGood.lPos = setReadDB.GetByteFromByteArray();
				stGood.lType = PEI_PACKET;
				stGood.pGoods = new CGoods;
				bVal=stGood.pGoods->DecodeFromDataBlock(setReadDB,bExData);
				if (CGoodsList::GetProperty(stGood.pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE)
				{
					AddShortCutGoods((WORD)stGood.pGoods->GetIndex());
					/// ��ȡ������Ʒ����ȴ����
					DWORD dwRemainTime = GetGoodsRemainTime((WORD)stGood.pGoods->GetIndex());
					if (dwRemainTime!=0)
					{
						DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)stGood.pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
						SetGoodsCoolDownStartTime((WORD)stGood.pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
						SetGoodsUseState((WORD)stGood.pGoods->GetIndex(),false);
					}
				}
				if(bVal)
				{
					m_listGoods.push_back(stGood);
					WORD SuitID = (WORD)stGood.pGoods->GetEffectVal(GAP_SUITID,1);			
					if (SuitID!=0)
					{
						string stOrName = stGood.pGoods->GetProperty()->strOrginName;					
						AddSuit(SuitID,stOrName);	
						//SetupEquip(SuitID,stOrName);
					}
					stGood.pGoods = NULL;
				}
				else
				{
					SAFE_DELETE(stGood.pGoods);
				}
			}
		}

		//##��ʼ��ÿ��������λ������ֵ
		BYTE lnum = setReadDB.GetByteFromByteArray();					    //���߱�����
		for (int i = 0;i<lnum;i++)
		{
			CGoods *pGood=new CGoods;
			bVal=pGood->DecodeFromDataBlock(setReadDB,bExData);
			BYTE index=setReadDB.GetByteFromByteArray();
			if(bVal&&m_Cote[index]!=NULL)
			{
				m_Cote[index]->pGoods = pGood;							//pGoods�а����˵��߱����Ĵ�С������
				pGood = NULL;
			}
			else
				SAFE_DELETE(pGood);	
		}
		//##��ʼ��ÿ�����������Ʒ����Ϣ������ÿ��������λ��
		for (int i = 0;i<COTE_NUM;i++)
		{	
			lNum=setReadDB.GetLongFromByteArray();
			if (lNum>0)
			{
				for(int j=0;j<lNum;j++)
				{
					tagGoods stGood;
					stGood.bChange=setReadDB.GetByteFromByteArray() != 0;
					stGood.lNum = setReadDB.GetWordFromByteArray();
					stGood.lPos = setReadDB.GetByteFromByteArray();
					stGood.lType = PEI_PACK1+i;
					stGood.pGoods = new CGoods;
					bVal = stGood.pGoods->DecodeFromDataBlock(setReadDB,bExData);
					if (CGoodsList::GetProperty(stGood.pGoods->GetIndex())->BaseProperty.dwType==GBT_CONSUMABLE)
					{
						AddShortCutGoods((WORD)stGood.pGoods->GetIndex());
						/// ��ȡ������Ʒ����ȴ����
						DWORD dwRemainTime = GetGoodsRemainTime((WORD)stGood.pGoods->GetIndex());
						if (dwRemainTime!=0)
						{
							DWORD dwTimeElapse = CGoodsList::GetEffectVal((WORD)stGood.pGoods->GetIndex(),GAP_COOLDOWN,1)-dwRemainTime;
							SetGoodsCoolDownStartTime((WORD)stGood.pGoods->GetIndex(),timeGetTime()-dwTimeElapse);
							SetGoodsUseState((WORD)stGood.pGoods->GetIndex(),false);
						}
					}
					if(bVal)
					{
						m_Cote[i]->m_listCoteGoods.push_back(stGood);
						WORD SuitID = (WORD)stGood.pGoods->GetEffectVal(GAP_SUITID,1);			
						if (SuitID!=0)
						{
							string stOrName = stGood.pGoods->GetProperty()->strOrginName;					
							AddSuit(SuitID,stOrName);	
							//SetupEquip(SuitID,stOrName);
						}
						stGood.pGoods = NULL;
					}
					else
					{
						SAFE_DELETE(stGood.pGoods);
					}
				}
			}
		}


		lTest = setReadDB.GetLongFromByteArray();
		// #���
		lNum = setReadDB.GetLongFromByteArray();
		CGUID guID;
		setReadDB.GetBufferFromByteArray(guID);
		SetMoney(lNum);
		SetguMoneyID(guID);

		// #����
		lNum = setReadDB.GetLongFromByteArray();
		setReadDB.GetBufferFromByteArray(guID);
		SetSilverMoney(lNum);
		SetguSilverMoneyID(guID);

		// ��ʯ
		lNum = setReadDB.GetLongFromByteArray();
		setReadDB.GetBufferFromByteArray(guID);
		SetBaoShi(lNum);
		SetguBaoShiID(guID);

		// #ticket
		lNum = setReadDB.GetLongFromByteArray();
		SetJiFen(lNum);

		// ���壺�ֿ���Ϣͷ ����
		CDepot::Inst()->DecodeDepotHeadInfo( setReadDB );
		// ���壺��֯ϵͳ
		DecodeOrgSysFromDataBlock(setReadDB);

		//��ǰʹ�õĳƺ�
		long lTitleType = setReadDB.GetLongFromByteArray();
		SetTitleType(lTitleType);
		//GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetTitleType(lTitleType);
		char strTitleName[1024];
		string strTitlle = setReadDB.GetStringFromByteArray(strTitleName,1024);
		SetPlayerTitle(strTitlle);
		//GetGame()->GetCGuiEx()->GetCharacterPageEx()->SetPlayerTitle(strTitlle);

		//GM����
		BYTE bMConcealment = setReadDB.GetByteFromByteArray();
		SetGMConcealment(bMConcealment==0?false:true);

		//�Զ�����
        GetGame()->GetAutoSupply()->DecodeFromDataBlockAndInit(setReadDB);
		//GetGame()->GetCGuiEx()->GetAutoPageEx()->DecodeFromDataBlockAndInit(setReadDB);

		// 
		m_PlayerSetupOnServer.DecodeFromDataBlock( setReadDB );

		//�õ���������
		//DecordQuestDataFromByteArray();
		//-----07.9.6�����޸�------
		m_RoleQueue.DecodeFromDataBlock(setReadDB);
		//////////��ְҵϵͳ������//////////
		m_dwCollectExp = setReadDB.GetLongFromByteArray();
		m_dwCollectLevel = setReadDB.GetLongFromByteArray();
		m_dwFactureExp=setReadDB.GetLongFromByteArray();
		m_dwFactureLevel=setReadDB.GetLongFromByteArray();
		lNum =setReadDB.GetLongFromByteArray();
		for (int i=0; i<lNum; i++)
		{
			DWORD dwFactureID =setReadDB.GetLongFromByteArray();
			m_vecFactureEntrys.push_back(dwFactureID);
		}

		long lLev = setReadDB.GetLongFromByteArray();
		long lID = setReadDB.GetLongFromByteArray();

		GetGame()->GetMainPlayer()->SetLimtSkill(lID,lLev);

		// ��ļ��Ϣ
		DecodeRecruitDataFromDataBlock(setReadDB);
		////////////////////////////////////
		/*CRoleRecord *pQuest = new CRoleRecord(77,1);
		m_RoleQueue.AddOneRecord(pQuest);*/
		//-------------------------

		//�������� 2009/1/06 nikai
		DecodePetFromDataBlock(setReadDB,bExData);

		BYTE bIsHaveBusinessPack = setReadDB.GetByteFromByteArray();
		if (bIsHaveBusinessPack>0)
		{
			//##��ʼ�����ʱ�����λ������ֵ
			CGoods *pBusinessPackGoods = new CGoods;
			bVal=pBusinessPackGoods->DecodeFromDataBlock(setReadDB,bExData);
			if(bVal&&m_CoteBussinesPack!=NULL)
			{
				m_CoteBussinesPack->pGoods = pBusinessPackGoods;							//pGoods�а����˵��߱����Ĵ�С������
				pBusinessPackGoods = NULL;		
				//##��ʼ�����ʱ������ÿ����Ʒ����Ϣ
				lNum=setReadDB.GetLongFromByteArray();	// ��Ʒ����
				if (lNum>0)
				{
					for(int j=0;j<lNum;j++)
					{
						tagGoods stGood;
						stGood.bChange=setReadDB.GetByteFromByteArray() != 0;
						stGood.lNum = setReadDB.GetWordFromByteArray();
						stGood.lPos = setReadDB.GetByteFromByteArray();
						stGood.lType = PEI_BUSINESSPACK;
						stGood.pGoods = new CGoods;
						bVal = stGood.pGoods->DecodeFromDataBlock(setReadDB,bExData);
						if(bVal)
						{
							m_CoteBussinesPack->m_listCoteGoods.push_back(stGood);
							stGood.pGoods = NULL;
						}
						else
						{
							SAFE_DELETE(stGood.pGoods);
						}
					}
				}
			}
			else
				SAFE_DELETE(pBusinessPackGoods);	
		}
		SetState(GetState());
		//
		m_bIsShowLoverName = (setReadDB.GetByteFromByteArray()>0);
	}
	else
	{
		// װ�����
		SetHairPic(setReadDB.GetByteFromByteArray());
		SetFacePic(setReadDB.GetByteFromByteArray());
		SetShowFashion(setReadDB.GetByteFromByteArray());

		// װ�����
        for(long l=EQUIP_HEAD; l<EQUIP_SUM; l++)
        {
            m_dwEquipIndex[l] = setReadDB.GetDwordFromByteArray();
            m_btEquipLevel[l] = setReadDB.GetByteFromByteArray();
        }

		// �Ƿ���ʾ���ұ�־
		SetShowCountry(setReadDB.GetByteFromByteArray() != 0 ? true : false);

		SetHp(setReadDB.GetDwordFromByteArray());
		SetMaxHp(setReadDB.GetDwordFromByteArray());
		//--------07.8.3ȡ��ע��(����)---------
		SetPkValue( setReadDB.GetDwordFromByteArray() );
		SetPVPValue( setReadDB.GetDwordFromByteArray() );

		//��֯ϵͳ
		DecodeOrgSysFromDataBlock(setReadDB);

		//�ƺ�ϵͳ
		long lTitleType = setReadDB.GetLongFromByteArray();
		SetTitleType(lTitleType);
		char strTitleName[1024];
		string strTitlle = setReadDB.GetStringFromByteArray(strTitleName,1024);
		SetPlayerTitle(strTitlle); 

		//GM����
		BYTE bMConcealment = setReadDB.GetByteFromByteArray();
		SetGMConcealment(bMConcealment==0?false:true);

		//ְҵ+�Ա�
		SetOccupation(setReadDB.GetByteFromByteArray());
		SetSex(setReadDB.GetByteFromByteArray());
		SetAssOccu(setReadDB.GetByteFromByteArray());

		//
		setReadDB.GetBufferFromByteArray(m_PlayerShopSessionID);
		m_lPlayerShopPlugID = 0;
		SetPlayerShopName("");
		SetPlayerShopOpen(false);
		if(m_PlayerShopSessionID != NULL_GUID)
		{
			m_lPlayerShopPlugID=setReadDB.GetLongFromByteArray();
			char strPlayerShopName[17];
			memset(strPlayerShopName,0,sizeof(strPlayerShopName));
			setReadDB.GetStringFromByteArray(strPlayerShopName,17);
			SetPlayerShopName(strPlayerShopName);
			SetPlayerShopOpen(true);			
		}

		DWORD dwEmotionIndex=setReadDB.GetDwordFromByteArray();
		DWORD dwTime=setReadDB.GetDwordFromByteArray();
		if(dwEmotionIndex)
		{
			SetEmotion(dwEmotionIndex-1,dwTime);
		}
		// ����ID
		SetCountry( setReadDB.GetByteFromByteArray());
		SetLevel(setReadDB.GetWordFromByteArray());
		// ��ļ��Ϣ
		DecodeRecruitDataFromDataBlock(setReadDB);
		//���CPֵ
		long lCurPKCP = setReadDB.GetLongFromByteArray();
		long lMaxPKCP = setReadDB.GetLongFromByteArray();
		SetCurPKCP(lCurPKCP);
		SetMaxPKCP(lMaxPKCP);

        //����Ŀ���
        SetDestXY(setReadDB.GetFloatFromByteArray(),
                  setReadDB.GetFloatFromByteArray());
        
        float fRadianDir = GetLineDirF(m_fPosX,m_fPosY,m_fDestX,m_fDestY);
        //�����µķ���
        SetDirEx(fRadianDir);
        
        //�ó��ƶ�ʱ��
        float fTotalDistance = RealDistance(m_fDestX,m_fDestY);
        float dwMoveTime = fTotalDistance/m_fSpeed;
        SetEndMoveTime(timeGetTime()+(DWORD)dwMoveTime);
        SetMoveOffset((m_fDestX-m_fPosX)/dwMoveTime, (m_fDestY-m_fPosY)/dwMoveTime);
	}

	return true;
}

void CPlayer::DecodeGeniusFromDataBlock(DBReadSet& setReadDB)
{

    /// ������츳����
    long   GisCount = setReadDB.GetLongFromByteArray();
    for ( int l = 0 ; l < GisCount ; l++ )
    {
        long  GisID = setReadDB.GetLongFromByteArray();
        long  GisLvl= setReadDB.GetLongFromByteArray();
        m_GeniusTable [ GisID ] = GisLvl ;
    }

    /// Ӱ�켼���б�
    long   ListCount = setReadDB.GetLongFromByteArray();
    for ( int l = 0 ; l < ListCount ; l++ )
    {
        /// ���� id �� lvl
        long  skillid = setReadDB.GetLongFromByteArray();
        long  skillLvl= setReadDB.GetLongFromByteArray();
        /// ��������Ӱ�����        
        long  ProNum  = setReadDB.GetLongFromByteArray();
		std::vector<string>vecStrKey;
		std::vector<long>vecValue;
        for ( int j = 0 ; j < ProNum ; j++ )
        {
            char pStr[64] ;

            /// ��������ֵ
            setReadDB.GetStringFromByteArray(pStr,64);
			vecStrKey.push_back(pStr);
            /// ����ֵ
            long pValue = setReadDB.GetLongFromByteArray();
			vecValue.push_back(pValue);

			//����������ϼ�������
			UpdateSkillAttr(skillid,skillLvl,pStr,pValue);
        }
		//���¼���ģ��
		UpdateSkillXmlList(skillid,skillLvl,vecStrKey,vecValue);
    }
}

bool CPlayer::DecodeOrgSysFromDataBlock(DBReadSet& setReadDB)
{
	// �л������ GUID
	setReadDB.GetBufferFromByteArray(m_FactionID);
	setReadDB.GetBufferFromByteArray(m_UnionID);

	if((BOOL)setReadDB.GetByteFromByteArray())
	{
		LONG lFacDemiseWarning = 0;
		LONG lFacDisbandWarning = 0;

		if( this->IsSelf() )
		{
			lFacDemiseWarning = setReadDB.GetLongFromByteArray(); //! ��ᴫλ���ѵ���ʱ
			lFacDisbandWarning = setReadDB.GetLongFromByteArray(); //! ����ɢ���ѵ���ʱ
		}

		// �ƺ�
		TCHAR szFactionTitle[ MAX_MEMBER_NAME_SIZE ];
		setReadDB.GetStringFromByteArray( szFactionTitle, MAX_MEMBER_NAME_SIZE );

		// �л�ͼ��CRC
		m_dwFacIconCRC = setReadDB.GetDwordFromByteArray();

		// PUBLIC��Ϣ��Ȩ�ޡ��ٽ�
		tagFacPublicData stFacPublicData;
		ZeroMemory( &stFacPublicData, sizeof(stFacPublicData) );

		TCHAR	szPronounce[ MAX_PronounceCharNum ];
		ZeroMemory( szPronounce, sizeof(szPronounce) );

		long lMyJobPurview = 0;

		if( this->IsSelf() )
		{
			// ��ȡPUBLIC��Ϣ
			setReadDB.GetBufferFromByteArray( &stFacPublicData, sizeof(tagFacPublicData) );

			// ��ȡ�ٽ�
			m_lFactionJob = setReadDB.GetLongFromByteArray();

			// ��ȡȨ������
			lMyJobPurview = setReadDB.GetLongFromByteArray();

			// ����
			setReadDB.GetStringFromByteArray( szPronounce, MAX_PronounceCharNum);
		}
		else
		{
			setReadDB.GetStringFromByteArray( stFacPublicData.szName, MAX_MEMBER_NAME_SIZE );
		}

		// ������ƺͳƺ�
		m_strFactionName = stFacPublicData.szName;
		m_strFactionTitle = szFactionTitle;

		// ����һ���л�
		CFactionManager*	pFactionManager = GetGame()->GetFactionManager();

		CFaction::SFacInfo	stFacInfo;
		ZeroMemory( &stFacInfo, sizeof(stFacInfo) );
		stFacInfo.guidFac = m_FactionID;
		stFacInfo.guidUnion = m_UnionID;
		stFacInfo.strFacName = stFacPublicData.szName;
		stFacInfo.strPlacard = szPronounce;
		stFacInfo.arrLevel[ CFaction::FUN_FACTION ] = stFacPublicData.lLevel;

		stFacInfo.tMasterLastOnlineTime = (time_t)lFacDemiseWarning;
		stFacInfo.tLessMemNumTime = (time_t)lFacDisbandWarning;

		if( CFaction* pFaction = pFactionManager->GetFaction( m_FactionID ) )
		{
			pFaction->SetUnionGUID( m_UnionID );
			pFaction->SetName( stFacPublicData.szName );
			pFaction->SetPlacard( szPronounce );
			pFaction->SetLevel( stFacPublicData.lLevel );
			pFaction->GetFacInfo()->tMasterLastOnlineTime = stFacInfo.tMasterLastOnlineTime;
			pFaction->GetFacInfo()->tLessMemNumTime = stFacInfo.tLessMemNumTime;
			pFaction->SetJobPurview( m_lFactionJob, lMyJobPurview );
		}
		else
		{
			if( pFactionManager->AddFaction( &stFacInfo ) )
				pFactionManager->GetFaction( m_FactionID )->SetJobPurview( m_lFactionJob, lMyJobPurview );
		}

		// У��ձ�
		if( pFactionManager->CheckFacIconCRC( m_FactionID, m_dwFacIconCRC ) )
		{
			pFactionManager->SendUpdateFacIconMsg( m_FactionID );
		}
	}

	return true;
}

void CPlayer::DecodeRecruitDataFromDataBlock(DBReadSet& setReadDB)
{
	// ��ļ״̬
	long lFlag = setReadDB.GetByteFromByteArray();
	// ֹͣ��
	if(lFlag==0)
	{
		SetRecruitingTeam(false);
		return;
	}
	long lMemNum = setReadDB.GetByteFromByteArray();
	SetTeammateAmount(lMemNum);
	char strTeamName[20];
	memset(strTeamName,0,sizeof(strTeamName));
	setReadDB.GetStringFromByteArray(strTeamName,20);
	SetRecruitingTeam(true);
	SetTeamname(strTeamName);
	long lHasPass = setReadDB.GetByteFromByteArray();
	SetTeamHasPassword((lHasPass)==0?false:true);
	return;
}

void CPlayer::RandomMoveGoods()
{
	if (!m_listGoods.empty())
	{
		CMainPlayerHand* pHand = 0;//GetGame()->GetCGuiEx()->GetPMainPlayerHand();
		if (pHand!=NULL)
		{  
			int iPackDem = 0;
			long pack[PACK_NUM] = { PEI_PACKET,PEI_PACK1,PEI_PACK2,PEI_PACK3,PEI_PACK4,PEI_PACK5 };	
			//for (int i = 0;i<PACK_NUM;i++)
			//{
			for(tagGoodsIter GoodsIter = GetGoodsList(0)->begin();
				GoodsIter!=GetGoodsList(0)->end(); ++GoodsIter)
			{
				//long lExtendID = pack[random((int)PACK_NUM)];
				//if (lExtendID==PEI_PACKET)
				//{
				iPackDem = (int)GetHostPackgeDimen();
				//}
				//else if (lExtendID<=PEI_PACK5&&lExtendID>=PEI_PACK1)
				//{
				//	iPackDem = (int)GetPackageDimen(lExtendID-PEI_PACK1);
				//}
				if (iPackDem>0)
				{
					pHand->SendPutDownMes(TYPE_PLAYER,this->GetExID(),PEI_PACKET,GoodsIter->lPos,TYPE_PLAYER,this->GetExID(),PEI_PACKET,random((int)iPackDem),TYPE_GOODS,(CGUID&)GoodsIter->pGoods->GetExID(),GoodsIter->pGoods->GetNum());
					//pHand->SendPutDownMes(TYPE_PLAYER,this->GetExID(),GoodsIter->lType,GoodsIter->lPos,TYPE_PLAYER,this->GetExID(),lExtendID,random((int)iPackDem),TYPE_GOODS,(CGUID&)GoodsIter->pGoods->GetExID(),GoodsIter->pGoods->GetNum());
				}
			}
			//}
		}
	}
}



/*
* ����: ����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2009.02.09 - lpf
*		������"��һ�þ�����ʾ"�Ĵ���
*	2009.04.08 - lpf
*		������"��ҹ�ѫֵ�ı�"�Ĵ���
*/
void CPlayer::AI()
{
	CMoveShape::AI();

	// ��������HP/MP/MA
	m_lGrowTimer++;
	if( GetCurTickCount() - m_lGrowTimer > 400 )
	{
		m_lGrowTimer = GetCurTickCount();

		if( m_listGrowHP.size() )
		{
			SetHp( GetHp() + *(m_listGrowHP.begin()) );
			m_listGrowHP.pop_front();
		}
		if( m_listGrowMP.size() )
		{
			SetMp( GetMp() + *(m_listGrowMP.begin()) );
			m_listGrowMP.pop_front();
		}
	}
	//ˢ�±��鶯��
	m_pEmotionShower->Update();

	//ͷ��������ʧ
	vector<tagPlayerInfo>::iterator it;
	for(it=m_vecPlayerInfo.begin();it!=m_vecPlayerInfo.end();it++)
	{
		if(GetCurTickCount()-it->dwStartTime>5000)
		{
			if(it->backAlpha>=3)
			{
				it->backAlpha-=3;
				it->dwBackColor=it->backAlpha<<24|(it->dwBackColor&0xffffff);
			}
			if(it->foreAlpha>=3)
			{
				it->foreAlpha-=3;
				it->dwForeColor=it->foreAlpha<<24|(it->dwForeColor&0xffffff);
			}
			else
			{
				m_vecPlayerInfo.erase(it);
				break;
			}
		}
	}

	// ��һ�þ�����ʾ
	static DWORD dwExpTipTextTimeLength = CGlobeSetup::GetSetup()->dwExpTipTextTimeLength;
	DWORD dwCurrentTime = timeGetTime();
	m_dwLevelCurrentDisplayNum = 0;
	for (DWORD dw = 0; dw < m_dwLevelMax; ++dw)
	{
		if (m_vecTipText[dw].fAlpha <= 0.0f)
			continue;

		m_vecTipText[dw].fAlpha = (float)((long)dwExpTipTextTimeLength - (long)(dwCurrentTime - m_vecTipText[dw].dwStartTime)) / (float)dwExpTipTextTimeLength;
		++m_dwLevelCurrentDisplayNum;
	}

	// ��ҹ�ѫֵ�ı�
	long lIncrement = (long)GetCountryContribute() - m_lOldContribute;
	if (lIncrement == 0)
	{
		// do nothing
	}else if (lIncrement > 0)
	{
		AddTipText(TT_CON, lIncrement);
		m_lOldContribute = GetCountryContribute();
	}else if (lIncrement < 0)
		m_lOldContribute = GetCountryContribute();
}

void CPlayer::SetEquipIndex(long lIndex,DWORD dwIndex)
{
	if(lIndex>=0 && lIndex<EQUIP_SUM)
	{
		m_dwEquipIndex[lIndex]=dwIndex;
	}
}

void CPlayer::SetEquipLevel(long lIndex,DWORD level)
{
	if(lIndex>=0 && lIndex<EQUIP_SUM)
	{
		m_btEquipLevel[lIndex]=level;
	}
}

void CPlayer::MountEquip(CGoods *pGood,bool bPositive)
{
	//if(pGood)
	//{
	//	vector<CGoodsList::tagEffect>::iterator iter=pGood->GetEffect()->begin();
	//	for(;iter!=pGood->GetEffect()->end();iter++)
	//	{
	//		switch(iter->wType) 
	//		{			
	//		case 14:	//��С������
	//			{
	//				long lret=GetMinAtk()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>=0)
	//					{
	//						SetMinAtk(lret);
	//					}
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetMinAtk(lret);
	//						else
	//							SetMinAtk(0);
	//					}
	//				}	
	//			}
	//			break;
	//		case 15://��󹥻���
	//			{
	//				long lret=GetMaxAtk()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetMaxAtk(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetMaxAtk(lret);
	//						else
	//							SetMaxAtk(0);
	//					}
	//				}	
	//			}
	//			break;
	//		case 16:	//��������ϵ��
	//			{	
	//				
	//			}
	//			break;
	//		case 17://���������
	//			{
	//				long lret=GetDef()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetDef(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetDef(lret);
	//						else
	//							SetDef(0);
	//					}
	//				}	
	//			}
	//			break;
	//		case 18:	//�����ٶ�����
	//			{
	//				
	//			}
	//			break;
	//		case 19:	//��������
	//			{
	//				long lret=GetHit()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetHit(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetHit(lret);
	//						else
	//							SetHit(0);
	//					}
	//				}
	//			}
	//			break;
	//		case 20:	//����һ��
	//			{
	//
	//			}
	//			break;
	//		case 21:	//��������
	//			{
	//				long lret=GetDodge()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetDodge(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetDodge(lret);
	//						else
	//							SetDodge(0);
	//					}
	//				}	
	//			}
	//			break;
	//		case 22:	
	//			{					
	//			}
	//			break;
	//		case 23://������������
	//			{
	//
	//			}
	//			break;
	//		case 24:
	//			{
	//			}
	//			break;
	//		case 25://�����ָ��ٶ�
	//			{
	//				long lret=GetHpRecoverSpeed()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetHpRecoverSpeed(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetHpRecoverSpeed(lret);
	//						else
	//							SetHpRecoverSpeed(0);							
	//					}
	//				}	
	//			}
	//			break;
	//		case 26:	//�����ָ��ٶ�	
	//			{
	//				long lret=GetMpRecoverSpeed()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetMpRecoverSpeed(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{							
	//						if(lret>=0)
	//							SetMpRecoverSpeed(lret);
	//						else
	//							SetMpRecoverSpeed(0);							
	//					}
	//				}	
	//			}
	//			break;
	//		case 27://����׷��
	//			{
	//				long lret=GetStrenth()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>=0)
	//					{
	//						SetStrenth(lret);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sStrChange = short(iter->dwValue1);
	//						SetMinAtk(WORD(GetMinAtk() + sStrChange * pSetup->fStr2MinAtk[GetOccupation()]));
	//						SetMaxAtk(WORD(GetMaxAtk()+ sStrChange * pSetup->fStr2MaxAtk[GetOccupation()]));
	//						SetHit(WORD(GetHit() + sStrChange * pSetup->fStr2Hit[GetOccupation()]));				
	//					}
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)							
	//							SetStrenth(lret);							
	//						else							
	//							SetStrenth(0);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sStrChange = short(iter->dwValue1);
	//						float fVal=GetMinAtk() + sStrChange * pSetup->fStr2MinAtk[GetOccupation()];
	//						if(fVal>=0)
	//							SetMinAtk(fVal);
	//						else
	//							SetMinAtk(0);
	//						fVal=GetMaxAtk() + sStrChange * pSetup->fStr2MaxAtk[GetOccupation()];
	//						if(fVal>=0)
	//							SetMaxAtk(fVal);
	//						else
	//							SetMaxAtk(0);
	//						fVal=GetHit() + sStrChange * pSetup->fStr2Hit[GetOccupation()];
	//						if(fVal>=0)
	//							SetHit(fVal);
	//						else
	//							SetHit(0);
	//					}
	//				}				
	//			}
	//			break;
	//		case 28://����׷��
	//			{
	//				long lret=GetAgility()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>=0)
	//					{
	//						SetAgility(lret);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sDexChange = short(iter->dwValue1);
	//						SetDodge(WORD(GetDodge() + sDexChange * pSetup->fDex2Flee[GetOccupation()]));
	//						//SetAtcSpeed(WORD(m_Property.wAtcSpeed + sDexChange * pSetup->fDex2Speed[GetOccupation()]));
	//						SetHit(WORD(GetHit() + sDexChange * pSetup->fDex2Hit[GetOccupation()]));
	//					}
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>0)
	//							SetAgility(lret);
	//						else
	//							SetAgility(0);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sDexChange = short(iter->dwValue1);
	//						float fVal=GetDodge() + sDexChange * pSetup->fDex2Flee[GetOccupation()];
	//						if(fVal>=0)
	//							SetDodge(fVal);
	//						else
	//							SetDodge(0);
	//						fVal=GetHit() + sDexChange * pSetup->fDex2Hit[GetOccupation()];
	//						if(fVal>=0)
	//							SetHit(fVal);
	//						else
	//							SetHit(0);
	//					}
	//				}				
	//			}
	//			break;
	//		case 29://����׷��
	//			{
	//				long lret=GetConsti()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>=0)
	//					{
	//						SetConsti(lret);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sConChange = short(iter->dwValue1);
	//						SetMaxHP(WORD(GetMaxHp() + sConChange * pSetup->fCon2MaxHp[GetOccupation()]));
	//						SetDef(WORD(GetDef() + sConChange * pSetup->fCon2Def[GetOccupation()]));
	//					}
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetConsti(lret);
	//						else
	//							SetConsti(0);
	//						CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//						short sConChange = short(iter->dwValue1);
	//						float fVal=GetMaxHp() + sConChange * pSetup->fCon2MaxHp[GetOccupation()];
	//						if(fVal>=0)
	//							SetMaxHP(fVal);
	//						else
	//							SetMaxHP(0);
	//						fVal=GetDef() + sConChange * pSetup->fCon2Def[GetOccupation()];
	//						if(fVal>=0)
	//							SetDef(fVal);
	//						else
	//							SetDef(0);
	//					}
	//				}
	//			}
	//			break;
	//		case 30://����׷��
	//			{
	//				
	//			}
	//			break;
	//		case 31:	//������������		
	//			{
	//				long lret=GetMaxHp()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetMaxHP(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetMaxHP(lret);
	//						else
	//							SetMaxHP(0);
	//					}
	//				}
	//			}
	//			break;
	//		case 32://������������
	//			{
	//				long lret=GetMaxMP()+iter->dwValue1;
	//				if(bPositive)
	//				{
	//					if(iter->dwValue1>0)
	//						SetMaxMP(lret);
	//				}
	//				else
	//				{
	//					if(iter->dwValue1<0)
	//					{
	//						if(lret>=0)
	//							SetMaxMP(lret);
	//						else
	//							SetMaxMP(0);
	//					}
	//				}
	//			}
	//			break;
	//		case 33:
	//			{
	//			}
	//			break;
	//		case 34:
	//			{
	//			}
	//			break;
	//		case 35:
	//			{
	//			}
	//			break;
	//		case 36:
	//			{
	//			}
	//			break;
	//		case 37:
	//			{
	//			}
	//			break;
	//		case 38:
	//			{
	//			}
	//			break;
	//		case 39:
	//			{
	//			}
	//			break;
	//		case 40:
	//			{
	//			}
	//			break;
	//		case 41:
	//			{
	//			}
	//			break;
	//		case 42:
	//			{
	//			}
	//			break;
	//		case 43:
	//			{
	//			}
	//			break;
	//		case 44:
	//			{
	//			}
	//			break;
	//		case 45:
	//			{
	//			}
	//			break;
	//		case 46:
	//			{
	//			}
	//			break;
	//		case 47:
	//			{
	//			}
	//			break;
	//		case 51:
	//			{
	//				
	//			}
	//			break;

	//		case 91:
	//			{
	//					
	//			}
	//			break;
	//		case 92:
	//			{	
	//			}
	//			break;
	//		case 93:
	//			{
	//
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
}

void CPlayer::UnMountEquip(CGoods *pGood)
{
	//if(pGood)
	//{
	//	vector<CGoodsList::tagEffect>::iterator iter=pGood->GetEffect()->begin();
	//	for(;iter!=pGood->GetEffect()->end();iter++)
	//	{
	//		switch(iter->wType) 
	//		{			
	//		case 14:	//��С������
	//			{
	//				SetMinAtk(GetMinAtk()-iter->dwValue1);
	//			}
	//			break;
	//		case 15://��󹥻���
	//			{
	//				SetMaxAtk(GetMaxAtk()-iter->dwValue1);
	//			}
	//			break;
	//		case 16:	//��������ϵ��	
	//			{
	//				
	//			}
	//			break;
	//		case 17://���������
	//			{
	//				SetDef(GetDef()-iter->dwValue1);
	//			}
	//			break;
	//		case 18:	//�����ٶ�����
	//			{
	//			
	//			}
	//			break;
	//		case 19:	//��������
	//			{
	//				SetHit(GetHit()-iter->dwValue1);
	//			}
	//			break;
	//		case 20:	//����һ��
	//			{
	//				
	//			}
	//			break;
	//		case 21:	//��������
	//			{
	//				SetDodge(GetDodge()-iter->dwValue1);
	//			}
	//			break;
	//		case 22:	
	//			{					
	//			}
	//			break;
	//		case 23://������������
	//			{
	//				
	//			}
	//			break;
	//		case 24:
	//			{
	//			}
	//			break;
	//		case 25:	//�����ָ��ٶ�
	//			{
	//				SetHpRecoverSpeed(GetHpRecoverSpeed()-iter->dwValue1);
	//			}
	//			break;
	//		case 26:	//�����ָ��ٶ�	
	//			{
	//                   SetMpRecoverSpeed(GetMpRecoverSpeed()-iter->dwValue1);
	//			}
	//			break;
	//		case 27://����׷��
	//			{
	//				SetStrenth(GetStrenth()-iter->dwValue1);

	//				CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//				short sStrChange = -short(iter->dwValue1);
	//				SetMinAtk(WORD(GetMinAtk() + sStrChange * pSetup->fStr2MinAtk[GetOccupation()]));
	//				SetMaxAtk(WORD(GetMaxAtk() + sStrChange * pSetup->fStr2MaxAtk[GetOccupation()]));
	//				SetHit(WORD(GetHit() + sStrChange * pSetup->fStr2Hit[GetOccupation()]));
	//				
	//			}
	//			break;
	//		case 28://����׷��
	//			{
	//				SetAgility(GetDex()-iter->dwValue1);

	//				CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//				short sDexChange = -short(iter->dwValue1);
	//				SetDodge(WORD(m_Property.wDodge + sDexChange * pSetup->fDex2Flee[GetOccupation()]));
	//				SetAtcSpeed(WORD(m_Property.wAtcSpeed + sDexChange * pSetup->fDex2Speed[GetOccupation()]));
	//				SetHit(WORD(m_Property.wHit + sDexChange * pSetup->fDex2Hit[GetOccupation()]));
	//			}
	//			break;
	//		case 29://����׷��
	//			{
	//				SetCon(GetCon()-iter->dwValue1);

	//				CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//				short sConChange = -short(iter->dwValue1);
	//				SetMaxHP(WORD(m_Property.wMaxHp + sConChange * pSetup->fCon2MaxHp[GetOccupation()]));
	//				SetDef(WORD(m_Property.wDef + sConChange * pSetup->fCon2Def[GetOccupation()]));
	//			}
	//			break;
	//		case 30://����׷��
	//			{
	//				SetInt(GetInt()-iter->dwValue1);

	//				CGame::tagSetup *pSetup = GetGame()->GetSetup();
	//				short sIntChange = -short(iter->dwValue1);
	//				SetMaxMP(WORD(m_Property.wMaxMp + sIntChange * pSetup->fInt2MaxMp[GetOccupation()]));
	//				SetElementResistant(WORD(m_Property.wElementResistant + sIntChange * pSetup->fInt2Resistant[GetOccupation()]));
	//			}
	//			break;
	//		case 31:	//������������	
	//			{
	//				SetMaxHP(GetMaxHp()-iter->dwValue1);
	//			}
	//			break;
	//		case 32://������������
	//			{
	//				SetMaxMP(GetMaxMP()-iter->dwValue1);
	//			}
	//			break;
	//		case 33:
	//			{
	//			}
	//			break;
	//		case 34:
	//			{
	//			}
	//			break;
	//		case 35:
	//			{
	//			}
	//			break;
	//		case 36:
	//			{
	//			}
	//			break;
	//		case 37:
	//			{
	//			}
	//			break;
	//		case 38:
	//			{
	//			}
	//			break;
	//		case 39:
	//			{
	//			}
	//			break;
	//		case 40:
	//			{
	//			}
	//			break;
	//		case 41:
	//			{
	//			}
	//			break;
	//		case 42:
	//			{
	//			}
	//			break;
	//		case 43:
	//			{
	//			}
	//			break;
	//		case 44:
	//			{
	//			}
	//			break;
	//		case 45:
	//			{
	//			}
	//			break;
	//		case 46:
	//			{
	//			}
	//			break;
	//		case 47:
	//			{
	//			}
	//			break;
	//		case 51:
	//			{
	//				SetReAnk(GetReAnk()-iter->dwValue1);
	//			}
	//			break;

	//		case 91:
	//			{
	//				SetAttackAvoid(GetAttackAvoid() - iter->dwValue1);
	//			}
	//			break;
	//		case 92:
	//			{
	//				SetElementAvoid(GetElementAvoid() - iter->dwValue1);
	//			}
	//			break;
	//		case 93:
	//			{
	//				SetFullMiss(GetFullMiss() - iter->dwValue1);
	//			}
	//			break;
	//		default:
	//			break;
	//		}
	//	}
	//}
}

long CPlayer::CanMountEquip(CGoods *pGoods)
{
	//װ�����������ж�
	DWORD dwVal=0;
	//�ȼ�����
	dwVal=pGoods->GetEffectVal(6,1);
	if(dwVal!=0 && dwVal>GetLevel())
		return LEVEL_NOTFIT;

	//��������
	dwVal=pGoods->GetEffectVal(7,1);
	if(dwVal!=0 && dwVal>GetStr())
		return STR_NOTFIT;

	//��������
	dwVal=pGoods->GetEffectVal(8,1);
	if(dwVal!=0 && dwVal>GetDex())
		return DEX_NOTFIT;

	//��������
	dwVal=pGoods->GetEffectVal(9,1);
	if(dwVal!=0 && dwVal>GetCon())
		return CON_NOTFIT;

	//ְҵ����
	dwVal=CGoodsList::GetEffectVal(pGoods->GetIndex(),11,1);//pGoods->GetEffectVal(11,1);
	if(dwVal!=0 && dwVal!=GetOccupation()+1)
		return OCCUPATION_NOTFIT;

	//�Ա�����
	//dwVal=pGoods->GetEffectVal(12,1);
	//if(dwVal!=0 && dwVal!=GetSex()+1)
	//	return SEX_NOTFIT;
	return EQUIPFIT;
}

// ���ݴ������ƷID��Ҫ���ӵ���������һ���ѵ������ᳬ���ѵ����޲���δ����������Ʒ
CGoods* CPlayer::FindOverLapGoodsByIndex(DWORD dwIndex,DWORD dwGoodsNum,int& lExtendID,int& pPos)
{
	CGoods *pGoods=NULL;


	//�ڱ����в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			pGoods = GoodsIter->pGoods;
			if(pGoods!=NULL && pGoods->GetIndex()==dwIndex&&
				(pGoods->GetNum()+dwGoodsNum)<=(WORD)pGoods->GetEffectVal(GAP_MAXOVERLAP,1)&&!pGoods->IsLocked())
			{
				lExtendID = GetGoodsContainerID(pGoods);
				pPos = GoodsIter->lPos;
				return pGoods;
			}
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				pGoods = GoodsIter->pGoods;
				if(pGoods!=NULL && pGoods->GetIndex()==dwIndex&&
					(pGoods->GetNum()+dwGoodsNum)<=(WORD)pGoods->GetEffectVal(GAP_MAXOVERLAP,1)&&!pGoods->IsLocked())
				{
					lExtendID = GetGoodsContainerID(pGoods);
					pPos = GoodsIter->lPos;
					return pGoods;
				}
			}
		}
	}

	return NULL;
}

//���ҵ�һ�����ID���ڴ����ID��δ����������Ʒ
CGoods* CPlayer::FindFirstGoodsByIndex(DWORD dwIndex,int& lExtendID,int& pPos)
{
	CGoods *pGoods=NULL;

	//if(m_stHand.pGoods && m_stHand.pGoods->GetIndex()==dwIndex)
	//	return m_stHand.pGoods;

	//�ڱ����в���
	list<tagGoods>* pGoodList = GetGoodsList();
	if (pGoodList)
	{
		tagGoodsIter GoodsIter=pGoodList->begin();
		for(;GoodsIter!=pGoodList->end();GoodsIter++)
		{
			pGoods = GoodsIter->pGoods;
			if(pGoods!=NULL && pGoods->GetIndex()==dwIndex&&!pGoods->IsLocked())
			{
				lExtendID = GetGoodsContainerID(pGoods);
				pPos = GoodsIter->lPos;
				return pGoods;
			}
		}
	}
	//��ѯ�ӱ���
	for (int i = 0;i<COTE_NUM;i++)
	{	
		//���ӱ����в���
		pGoodList = GetSubGoodslist(i);
		if (pGoodList)
		{
			tagGoodsIter GoodsIter=pGoodList->begin();
			for(;GoodsIter!=pGoodList->end();GoodsIter++)
			{
				pGoods = GoodsIter->pGoods;
				if(pGoods!=NULL && pGoods->GetIndex()==dwIndex&&!pGoods->IsLocked())
				{
					lExtendID = GetGoodsContainerID(pGoods);
					pPos = GoodsIter->lPos;
					return pGoods;
				}
			}
		}
	}

	//�ڲֿ��в���
	/*list<tagGoods> *pDepotGoodsList=GetGame()->GetInterface()->GetDepotPage()->GetDepotList();
	GoodsIter=pDepotGoodsList->begin();
	for(;GoodsIter!=pDepotGoodsList->end();GoodsIter++)
	{
	CGoods* pGoods = GoodsIter->pGoods;
	if(pGoods && pGoods->GetExID() == guID)
	{	
	if(pPos)pPos[0]=GoodsIter->lPos;
	return pGoods;
	}
	}*/
	//�ڸ����̵������
	return NULL;
}


//����һ��ʹ�������Ե��ߵ���Ϣ��gameserver
void CPlayer::UseItem(DWORD dwIndex,int nExtendId,int iPos)
{
	long lEffectVal=CGoodsList::GetEffectVal(dwIndex,84,1);
	if( !GetGame()->GetMainPlayer()->GetHorse()->IsVisible() && lEffectVal )//����
	{
		if( GetState() == STATE_FIGHT || IsPlayerShopOpened())
		{
            GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Package_12")); //�����ڲ���ʹ�ø���Ʒ��
			return;
		}
	}
	tagGoods* pstGoods = NULL;
	CGoods* pGoods = NULL;
	if (nExtendId==PEI_PACKET)
	{
		pstGoods = GetGoods(iPos);
		if (pstGoods!=NULL)
		{
			pGoods = pstGoods->pGoods;
		}else
			return;
	}else if (nExtendId<=PEI_PACK5&&nExtendId>=PEI_PACK1)
	{
		pstGoods = GetSubGoods(nExtendId-PEI_PACK1,iPos);
		if (pstGoods!=NULL)
		{
			pGoods = pstGoods->pGoods;
		}else
			return;
	}else if (nExtendId == -1&&iPos == -1)
	{
		pGoods = FindFirstGoodsByIndex(dwIndex,nExtendId,iPos);
	}

	if (pGoods&&nExtendId!=-1&&iPos!=-1)
	{
		CPlayer::tagGoods TagGoods;
		TagGoods.lNum = pGoods->GetNum();
		TagGoods.pGoods = pGoods;
		TagGoods.lType = nExtendId;
		TagGoods.lPos = iPos;

		if(pGoods->GetProperty()->dwIndex==dwIndex && pGoods->GetNum()>=1)
		{
			//for(int i = 0;i<PACK_NUM;i++)
			//{
			//	list<CPlayer::tagGoods>* GoodsList = GetGoodsList(i);
			//	list<CPlayer::tagGoods>::iterator GoodsIter = GoodsList->begin();
			//	for(;GoodsIter!=GoodsList->end();GoodsIter++)
			//	{
			//		if(GoodsIter->pGoods->GetEffectVal(CGoods::GAP_COOLDOWN,2)==pGoods->GetEffectVal(CGoods::GAP_COOLDOWN,2))
			//		{
			//			AddShortCutGoods(GoodsIter->pGoods->GetIndex());
			//		}
			//	}
			//}

			// ���壺����ǳ��ܣ���Ҫ��ȷ��
			if( pGoods->HasEffect( GAP_POWER_INTENSITY ) )
			{
				//CItemPageEx* pItemUI = GetGame()->GetCGuiEx()->GetItemPageEx();
				// �������û��װ�����ܳ���
				if( GetEquip( EQUIP_WEAPON ) )
				{

					//pItemUI->m_powerExtendId = nExtendId;
					//pItemUI->m_powerPos	= iPos;

					// ���� ����Э��
					CMessage msg(MSG_C2S_PLAYER_USEITEM);
					msg.Add((WORD)nExtendId);
					msg.Add((BYTE)iPos);
					msg.Send();
					//GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = 1;
					//GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open( pItemUI, CStringReading::LoadText( 208, 210 ), CMessageBoxPageEx::MSG_OK_NO, true );
				}
				else
				{
					//GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = -1;
					//GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open( pItemUI, CStringReading::LoadText( 208, 211 ), CMessageBoxPageEx::MSG_OK, true );
				}
			}
			
			else if (GetGoodsUseState((WORD)dwIndex))
			{
				CGUID shapGuid = CGUID::GUID_INVALID;
				long shapType = 0;	
				long lShapX = 0;
				long lShapY = 0;

				DWORD dwGoodsSkillID = 0;

				//if (pGoods->GetEffectVal(CGoods::GAP_SCRIPT,1)!=0)
				//{
				//	//dwGoodsSkillID = 0;
				//}
				/// ����Ʒ���˴����ű�ʱ�ͷ��͹̶��ļ���40050
				/*else */
				if (pGoods->GetEffectVal(GAP_SKILL_CALL_SCRIPT,1)!=0)
				{
					dwGoodsSkillID = 40050;
				}
				else
					dwGoodsSkillID = pGoods->GetEffectVal(GAP_ADD_SKILL,1);
				CShape* pShape = dynamic_cast<CShape*>(GetGame()->GetGameControl()->GetSelectedShape());
				if (pShape!=NULL)
				{
					shapType = pShape->GetType();
					shapGuid = pShape->GetExID();
					lShapX = pShape->GetTileX();
					lShapY = pShape->GetTileY();
				}
				// ������Ʒ��ʹ��ʱ���ܴ���ĳ�����ܣ��ͷ���ʹ��ĳ�����ܵ���Ϣ
				if (dwGoodsSkillID!=0)
				{
					CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(dwGoodsSkillID);
					if (pSkill!=NULL&&pSkill->bHasAct)
					{
						if(GetGame()->GetGameControl()->GetMPActionMode() == CGameControl::MPAM_AutoMove)
							GetGame()->GetGameControl()->AutoMoveStop();
					}
					// ����Ŀ�꼼�ܣ��������������ܣ�ͬʱ���¡�~���������ͷ�Ŀ�����ó��Լ�����ֹͣĿǰ���Զ�����
					if(pSkill!=NULL&&!pSkill->bLockSkill && pSkill->bLockTarget && GetKeyboard()->IsKeyDown(DIK_GRAVE))
					{
						GetGame()->GetGameControl()->AutoAttactStop();
						pShape = GetGame()->GetMainPlayer();
						shapType = pShape->GetType();
						shapGuid = pShape->GetExID();
						lShapX = pShape->GetTileX();
						lShapY = pShape->GetTileY();
					}
					// �����Ҫ����Ŀ���ҵ�ǰ������Ŀ�꣬������Ŀ����Ϊ�Լ�
					else if(pSkill != NULL && pShape == NULL && pSkill->bLockTarget)
					{
						pShape = GetGame()->GetMainPlayer();
						shapType = pShape->GetType();
						shapGuid = pShape->GetExID();
						lShapX = pShape->GetTileX();
						lShapY = pShape->GetTileY();
					}
					/***********************************************************************/
					/* �Ժ� fix */
					// CSkillXml skill;
					/***********************************************************************/
					ConcreteSkillComp SkillCom;
					//SkillCom���ڴ潻����CSkillXml������������
					CSkillXml skill(&SkillCom); 
					if(skill.StepSend(dwGoodsSkillID,1,GetGame()->GetMainPlayer(),(CMoveShape*)pShape,lShapX,lShapY))
					{
						//���弼��
						GetGame()->GetGameControl()->SetBufferSkillID(dwGoodsSkillID);
						// ���ô������ܵ���Ʒ
						GetGame()->GetGameControl()->SetSkillGoods((long)nExtendId,(long)iPos);
						// ��Ŀ��ʹ�ü���
						GetGame()->GetGameControl()->StartAttack(pShape);
					}
					//pGoods->Lock();
				}
				// ������Ʒ�ܱ�ʹ�õ��ֱ����������ϴ�����Ӱ״̬ʱ��ȡ����Ʒ����Ӱ״̬��
				if (pGoods->IsHaveShadow())
				{
					pGoods->SetHaveShadowState(false);
				}
				/// �������ļ��ܲ���40050����û�д�������ʱ�ŷ���ʹ����Ʒ����Ϣ
				if (dwGoodsSkillID!=40050)
				{
					// ����ʹ����Ʒ����Ϣ
					CMessage msg(MSG_C2S_PLAYER_USEITEM);
					msg.Add((WORD)nExtendId);
					msg.Add((BYTE)iPos);
					msg.Add(shapType);
					msg.Add(shapGuid);
					msg.Send();
				}
			}
			else
			{
				//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,CStringReading::LoadText(208,218),D3DCOLOR_ARGB(255,255,0,0));
			}
		}
	}
}

// �ⲿʹ�õ�һ����̬���ýӿ�
void CPlayer::CallUseItem(DWORD dwIndex,int nExtendId,int iPos,CPlayer* pPlayer)
{
	if (pPlayer!=NULL)
	{
		pPlayer->UseItem(dwIndex,nExtendId,iPos);
	}else
		GetGame()->GetMainPlayer()->UseItem(dwIndex,nExtendId,iPos);
}

//��������ĳ�����������ֵ
DWORD CPlayer::GetCreditForZone(DWORD dwZoneID)
{
	if (m_mapPlayerCreditForZone.find(dwZoneID)!=m_mapPlayerCreditForZone.end())
	{
		return m_mapPlayerCreditForZone[dwZoneID];
	}
	return 0;
}

long CPlayer::CanUseItem(DWORD dwIndex)
{
	DWORD dwVal=0;
	//�ȼ����Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,6,1);	
	if(dwVal!=0 && dwVal>GetLevel())
		return LEVEL_NOTFIT;

	//�������Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,7,1);
	if(dwVal!=0 && dwVal>GetStr())
		return STR_NOTFIT;

	//�������Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,8,1);
	if(dwVal!=0 && dwVal>GetDex())
		return DEX_NOTFIT;

	//�������Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,9,1);
	if(dwVal!=0 && dwVal>GetCon())
		return CON_NOTFIT;

	//ְҵ���Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,11,1);
	if(dwVal!=0 && dwVal!=GetOccupation()+1)
		return OCCUPATION_NOTFIT;

	//�Ա����Ƽ��
	dwVal=CGoodsList::GetEffectVal(dwIndex,12,1);
	if(dwVal!=0 && dwVal!=GetSex()+1)
		return SEX_NOTFIT;

	return EQUIPFIT;
}

void  CPlayer::RefreshProperty()
{
	for(int i=0;i<EQUIP_SUM;i++)
	{
		if(m_Equip[i])
		{
			MountEquip(m_Equip[i]);
		}
	}
	for(int i=0;i<EQUIP_SUM;i++)
	{
		if(m_Equip[i])
		{
			MountEquip(m_Equip[i],false);
		}
	}
}


//������ֵ�ı��Ժ����
bool CPlayer::OnPropertyChanged(const char* attrName)
{
	if(NULL == attrName)
		return false;

	if(strlen(attrName) == 0)
		return false;

	CMoveShape::OnPropertyChanged(attrName);
	if(this == GetGame()->GetMainPlayer())
	{
		if(strncmp(attrName,"PresentExp",10) == 0)
		{
			/*CMainBarPageEx *pMainPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			if(pMainPage)
				pMainPage->UpdataPlayerExp();*/
		}
		// תְ
		else if(strcmp(attrName,"Occu") == 0)
		{
			SortSkillList();
			/*CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
			if (pMainBarPage)
			{
				for (int i = 0;i<24;i++)
					pMainBarPage->UpdataHotComp(i);
			}
			CSkillPageEx * pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
			if(pSkillPage && pSkillPage->IsOpen())
			{
				pSkillPage->Close();
				pSkillPage->Open();
			}*/
		}
		// ���ܵ�����SP��
		else if(strncmp(attrName,"OccuSP",6) == 0)
		{
			//CSkillPageEx* pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
			//if(pSkillPage && pSkillPage->IsOpen())
			//{
			//	pSkillPage->UpdataOccupExp();
			//	// ���µ�ǰְҵ��Ӧ�ļ����б�
			//	if(pSkillPage->GetCurActPage() == 0)
			//		pSkillPage->UpdataOccupSkill(pSkillPage->GetCurSelectedOccup(),pSkillPage->GetCurOccupSkillPage());
			//}
		}
		// ������רְ��ѧϰ��ְҵ����������������б�
		if(strcmp(attrName,"Level") == 0 || 
			strcmp(attrName,"Occu") == 0 ||
			strcmp(attrName,"DOccu") == 0)
		{
			TidySearchQuestList();
		}
	}
	if(strncmp(attrName,"Ride",4) == 0)
	{
		UpdateRideShow();
	}
	else if(strncmp(attrName,"Invisible",9) == 0)
	{
		if(GetAction() < ACT_PERFORM)
		{
			extern bool g_bForceUpdate;
			g_bForceUpdate = true;
			SetAction(GetAction());
		}
	}

	// ħ�����Ը��£��ܶࡣ����
	else
	{
		/*CCharacterPageEx* pCharacterPageEx = ::GetGame()->GetCGuiEx()->GetCharacterPageEx();
		if( pCharacterPageEx )
		{
			pCharacterPageEx->UpdateMagicInfo();
		}*/
	}
	return true;
}

//�õ�����ֵ���ַ�����ʾ
const char* CPlayer::GetPropertyStringValue(const char* pszPropertyName)
{
	return CMoveShape::GetPropertyStringValue(pszPropertyName);
}

// �쿴ָ��������Ƿ��Ǳ������(guid)
bool CPlayer::IsTeammate(const CGUID& id)
{
	vector<tagWSMemberInfo>::iterator iter = m_vecTeamMateInfo.begin();
	for (;iter != m_vecTeamMateInfo.end();++iter)
	{
		tagWSMemberInfo t = *iter;
		if ( t.guid == id ) return true;
	}
	return false;
}

// �쿴ָ��������Ƿ��Ǳ������(����)
bool CPlayer::IsTeammate(const char *szName)
{
	string strName = szName;
	vector<tagWSMemberInfo>::iterator iter = m_vecTeamMateInfo.begin();
	for (;iter != m_vecTeamMateInfo.end();++iter)
	{
		tagWSMemberInfo t = *iter;
		if ( t.szName == strName ) 
			return true;
	}
	return false;
}

// ���ָ��ID��Ա����Ϣ
tagWSMemberInfo * CPlayer::GetWsMemberInfo(const CGUID& id)
{
	vector<tagWSMemberInfo>::iterator iter = m_vecTeamMateInfo.begin();
	for (;iter != m_vecTeamMateInfo.end();++iter)
	{
		tagWSMemberInfo t = *iter;
		if ( t.guid == id )
			return &(*iter);
	}
	return NULL;
}

void CPlayer::SetCaptainID(CGUID& id)
{
	m_CaptainID = id;
	if(this != GetGame()->GetMainPlayer())
		return;
	// ���˵��ӳ�
	/*if(m_CaptainID == GetGame()->GetMainPlayer()->GetExID())
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetTeamCapPic(true);
		GetGame()->GetCGuiEx()->GetTeamPageEx()->SetTeamCapPic(false);
	}
	else
	{
		GetGame()->GetCGuiEx()->GetMainBarPageEx()->SetTeamCapPic(false);
		GetGame()->GetCGuiEx()->GetTeamPageEx()->SetTeamCapPic(true);
	}*/
}

// �Ƿ���Բ���������
bool CPlayer::IsOperforTeam()
{
	return true;//CheckSkill(m_lLimtSkillID) >= m_lLimtSkillLev?true:false;
}
//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////

// ��Ӽ���
bool CPlayer::AddSkill(long lSkillId, WORD wLevel/* =1 */,
					   DWORD dwRestoreTime/* =0 */, 
					   float RangeMin/* =1 */, 
					   float RangMax/* =1 */, 
					   WORD wCostMP/* =0 */)
{
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		if ( it->dwID == lSkillId )
		{
			it->wLevel = wLevel;
			it->dwRestoreTime = dwRestoreTime;
			it->fAffectRangeMin = RangeMin;
			it->fAffectRangeMax = RangMax;
			/*it->wCostMP = wCostMP;*/
			return false;
		}
	}

	tagSkill st;
	st.dwID = lSkillId;
	st.wLevel = wLevel;
	st.dwRestoreTime = dwRestoreTime;
	st.fAffectRangeMin = RangeMin;
	st.fAffectRangeMax = RangMax;
	/*st.wCostMP = wCostMP;*/
	m_listSkillID.push_back(st);
	//CSkillPageEx *pPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
	//if(IsAddOccupedList(st.dwID) && pPage && pPage->IsOpen())
	//	pPage->UpdataOccupList();
	SortSkillList();
	return true;
}

// ɾ������
bool CPlayer::DelSkill(DWORD lSkillId)
{
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		if ( it->dwID == lSkillId )
		{
			m_listSkillID.erase(it);
			return true;
		}
	}
	return false;
}

// ɾ�����ܣ�λ��)
bool CPlayer::DelSkill(long lPos)
{
	long n = 0;
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		if (n++ == lPos)
		{
			m_listSkillID.erase(it);
			return true;
		}
	}
	return false;
}

// Ϊ�����б���������
void CPlayer::SortSkillList()
{
	if(!m_listSkillID.empty())
		m_listSkillID.sort(Skill_Compare());
}
// ������ҵļ�������
void CPlayer::UpdateSkillAttr(long skillid,long level, const char* strKey,long value)
{
	tagSkill* pSkill = GetSkill(skillid);
	if(pSkill)
	{
		//����ʱ��
		if(strcmp("IntonateTime",strKey) == 0)
			pSkill->dwIntonateTime = value;
		//��ȴʱ��
		else if( strcmp("CoolDown",strKey) == 0)
			pSkill->dwRestoreTime = value;
		//����mP
		else if( strcmp("SMp", strKey ) == 0)
			pSkill->wCostMP = (WORD)value;
	}
}

// ��ȡ����
CPlayer::tagSkill*	CPlayer::GetSkill(DWORD dwID)
{
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		tagSkill* pSkill = &(*it);
		if (pSkill && pSkill->dwID == dwID)
		{
			return pSkill;
		}
	}
	return NULL;
}


// ���ü��ܵȼ�
bool CPlayer::SetSkillLevel(const char* strName, WORD wLevel, DWORD dwRestoreTime, 
							float RangeMin, 
							float RangeMax,
							WORD wCostMP)
{
	CSkillListXml::tagSkill *pt = CSkillListXml::GetPropertyByName(strName);
	if (pt==NULL)
	{
		return false;
	}
	DWORD dwID = pt->dwSkillID;

	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		if ( it->dwID == dwID )
		{
			it->wLevel = wLevel;
			it->dwRestoreTime = dwRestoreTime;
			/*it->fAffectRangeMin = RangeMin;
			it->fAffectRangeMax = RangeMax;
			it->wCostMP = wCostMP;*/
			return true;
		}
	}

	return false;
}

// ����Ƿ���ָ�����ܣ����صȼ� (0����û��)
WORD CPlayer::CheckSkill(DWORD dwIndex)
{
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		if ( it->dwID == dwIndex )
		{
			return it->wLevel;
		}
	}

	return 0;
}

// ˢ�¼�����ȴʱ��(������ʾ�Ƿ�ˢ�¹�����ȴʱ��)
void CPlayer::UpdateSkillCoolDownTime(bool bIsPublicTime)
{
	for (list<tagSkill>::iterator it=m_listSkillID.begin(); it!=m_listSkillID.end(); it++)
	{
		tagSkill *pSkill = &(*it);//GetSkill(it->wID);
		if (pSkill && pSkill->dwRestoreTime>=0)
		{
			//ȡ������ʱ��
			DWORD dwTimeElapse = timeGetTime() - pSkill->dwCoolDownStartTime;

			// ��Ҫ���¼��ܵĹ�����ȴʱ��
			if(bIsPublicTime)
			{
				// ���û����ȴ��ʼʱ�䣬ֱ������������ȴ��ʼʱ��
				// �������ȴ��ʼʱ�䣬ͬʱ��ʣ�µ���ȴʱ��С�ڹ�����ȴʱ�䣬�����趨��ȴ�Ŀ�ʼʱ��
				if( pSkill->dwCoolDownStartTime == 0 ||
					( (pSkill->dwCoolDownStartTime > 0 && pSkill->dwCoolDownStartTime < 0xffffffff) &&
					(pSkill->dwRestoreTime - dwTimeElapse < m_wPublicCoolTime)
					)
					)
				{
					pSkill->dwCoolDownStartTime = timeGetTime();
					pSkill->bIsPublicCoolEffect = bIsPublicTime;
					continue;
				}
			}
			// cool down
			if (pSkill->dwCoolDownStartTime > 0 && pSkill->dwCoolDownStartTime < 0xffffffff)
			{
				// ���ڹ�����ȴʱ��
				if(pSkill->bIsPublicCoolEffect)
				{
					// ������ȴʱ�䵽
					if(dwTimeElapse >= m_wPublicCoolTime)
					{
						pSkill->dwCoolDownStartTime = 0;
						pSkill->bIsPublicCoolEffect = false;
					}
				}
				// ���ܱ������ȴʱ�䵽
				else if (dwTimeElapse >= pSkill->dwRestoreTime)
				{
					pSkill->dwCoolDownStartTime = 0;
				}
			}
			else	// ��ɫ3���Ӽ��ʱ��
			{
				if ( GetCurTickCount() - pSkill->dwCoolDownGrayStartTime > 1500)
				{
					pSkill->dwCoolDownStartTime = 0;
					pSkill->dwCoolDownGrayStartTime = 0;
				}
			}
		}
	}
}

///���(������)��Ʒ������ȴ���͵�ʣ����ȴʱ��
bool CPlayer::AddPublicCooldownType(WORD wType,DWORD dwTime)
{
	map<WORD,PCooldownRTime>::iterator Iter = m_mapPublicCooldown.find(wType);
	if (Iter!=m_mapPublicCooldown.end())
	{
		Iter->second = dwTime;
		return FALSE;
	}else
	{
		m_mapPublicCooldown.insert(pair<WORD,PCooldownRTime>(wType,dwTime));
		return TRUE;
	}
}

///��ȡĳ�����͵���Ʒ������ȴʣ��ʱ��
DWORD CPlayer::GetReTimeOfPCooldownType(WORD wType)
{
	DWORD dwReTime = 0;
	map<WORD,PCooldownRTime>::iterator Iter = m_mapPublicCooldown.find(wType);
	if (Iter!=m_mapPublicCooldown.end())
	{
		dwReTime = Iter->second;
	}
	return dwReTime;
}

//�������ΪwGoodsIndex����Ʒ���б���
bool CPlayer::AddShortCutGoods(ulong wGoodsIndex)
{
	if (CGoodsList::GetEffectVal(wGoodsIndex,GAP_COOLDOWN,1)!=0)
	{
		tagShortCutGoods temp;
		temp.wID = wGoodsIndex;
		temp.wPublicCooldownType = (WORD)CGoodsList::GetEffectVal(wGoodsIndex,GAP_COOLDOWN,2);
		temp.dwRestoreTime = CGoodsList::GetEffectVal(wGoodsIndex,GAP_COOLDOWN,1);
		list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
		for (;it!=m_listShortCutGoods.end();it++)
		{
			/// ������е���Ʒ��ȴ������ͬ�����ȴID��GAP_COOLDOWN��ֵ2��ͬ���������¼������Ʒ����ȴ��ʼʱ������ͬ��
			if (temp.wPublicCooldownType==CGoodsList::GetEffectVal(it->wID,GAP_COOLDOWN,2)
				&&it->bCanBeUsed==false)
			{
				temp.dwCoolDownStartTime = it->dwCoolDownStartTime;
			}
			if (it->wID==wGoodsIndex)
			{
				//if (GetGoodsAmount(wGoodsIndex)==0)
				//{
				//	it->bCanBeUsed = true;
				//	return true;
				//}else
				return false;
			}
		}
		m_listShortCutGoods.push_back(temp);
		return true;
	}
	return false;
}

//���б���ɾ������ΪwGoodsIndex����Ʒ
bool CPlayer::DelShortCutGoods(ulong wGoodsIndex)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			m_listShortCutGoods.erase(it);
			return true;
		}
	}
	return false;
}

//��������ΪwGoodsIndex����Ʒ����ȴʱ��
void CPlayer::SetGoodsRestoreTime(ulong wGoodsIndex,DWORD dwTime)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			it->dwRestoreTime = dwTime;
			break;
		}
	}
}

//��������ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
void CPlayer::SetGoodsCoolDownStartTime(ulong wGoodsIndex,ulong dwTime)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			it->dwCoolDownStartTime = dwTime;
			if (GetGoodsRemainTime(wGoodsIndex)!=0&&dwTime==0)
			{
				SetGoodsRemainTime(wGoodsIndex,0);
			}
			break;
		}
	}
}

///��������ΪwGoodsIndex����Ʒ��ʣ����ȴʱ��
void CPlayer::SetGoodsRemainTime(ulong wGoodsIndex,DWORD dwTime)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			it->dwRemainTime = dwTime;
			if (GetReTimeOfPCooldownType(it->wPublicCooldownType)!=0&&dwTime==0)
			{
				AddPublicCooldownType(it->wPublicCooldownType,dwTime);
			}
			break;
		}
	}
}

///�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
ulong CPlayer::GetGoodsRemainTime(ulong wGoodsIndex)
{
	DWORD dwReTime = 0;
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			DWORD dwPReTime = GetReTimeOfPCooldownType(it->wPublicCooldownType);
			if (dwPReTime!=0)
			{
				it->dwRemainTime = dwPReTime;
				dwReTime = dwPReTime;
			}
			else
				dwReTime = it->dwRemainTime;
		}
	}
	return dwReTime;
}

//�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
ulong CPlayer::GetGoodsRestoreTime(ulong wGoodsIndex)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			return it->dwRestoreTime;
		}
	}
	return 0;
}

//�õ�����ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
ulong CPlayer::GetGoodsCoolDownStartTime(ulong wGoodsIndex)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			return it->dwCoolDownStartTime;
		}
	}
	return 0;
}

//��������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
void CPlayer::SetGoodsUseState(ulong wGoodsIndex,bool bState)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			//if (bState&&GetGoodsAmount(wGoodsIndex)==0)
			//{
			//	DelShortCutGoods(wGoodsIndex);
			//}else
			it->bCanBeUsed = bState;
			break;
		}
	}
}

//ȡ������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
bool CPlayer::GetGoodsUseState(ulong wGoodsIndex)
{
	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
	for (;it!=m_listShortCutGoods.end();it++)
	{
		if (it->wID==wGoodsIndex)
		{
			return it->bCanBeUsed;
		}
	}
	if (CGoodsList::GetEffectVal(wGoodsIndex,GAP_COOLDOWN,1)==0)
	{
		return true;
	}
	return false;
}

//------------------07.11.22liuke����ѧϰ�����ж�--------------
// ����ָ�����������ļ��ܺ͵ȼ����ж��Ƿ���ѧϰ
bool CPlayer::IsCanStudySkill(DWORD dwSkillID, WORD wLevel)
{
	CStudySkillCondition::tagCondition * pCondition = NULL;
	pCondition = CStudySkillCondition::GetCondition(dwSkillID,wLevel);
	if(pCondition)
	{
		DWORD dwPlayerOccuLev = GetOccuLvl(GetOccupation());
		long lNeedOccuLev = (long)pCondition->wPlayerLev - (long)GetOccuLvlCoe((eOccupation)GetOccupation(),(WORD)dwPlayerOccuLev);
		if(lNeedOccuLev < 1)
			lNeedOccuLev = 1;
		// �������ѧϰ����
		if(dwPlayerOccuLev >= (WORD)lNeedOccuLev && (WORD)GetSkillPoint() >= pCondition->dwSp)
		{
			vector<CStudySkillCondition::tagConditionEx*>::iterator it = pCondition->vec_ConditionEx.begin();
			for(; it != pCondition->vec_ConditionEx.end(); ++it)
			{
				// ���߸�������
				if((*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_GOODS)
				{
					CGoodsList::tagGoods2* ptgGoods = CGoodsList::GetProperty((*it)->strValue.c_str());
					if(ptgGoods)
					{
						// ��������еĸõ����������㣬����false
						if((*it)->lValue > GetGoodsAmount(ptgGoods->BaseProperty.dwIndex))
							return false;
					}
				}
				// ǰ�ü��ܸ���������ֻҪ��һ��ǰ�ü��ܲ���Ҫ�󣬷���false
				else if((*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_PRESKILL)
				{
					WORD wlev = CheckSkill((DWORD)atoi((*it)->strValue.c_str()));
					// ��ǰ�ü��ܵȼ�����Ҫ��
					if( (*it)->lValue > wlev )
						return false;
				}
				// ��ͻ���Ƽ��ܸ���������ֻҪ�������Ƽ��ܣ�����false
				else if((*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_CONFLICTSKILL)
				{
					WORD wlev = CheckSkill((DWORD)atoi((*it)->strValue.c_str()));
					// ѧ��������Ƽ���
					if(wlev != 0)
						return false;
				}
			}
			return true;
		}
	}
	return false;
}
//-------------------------------------------------------------


void	CPlayer::SetguID(LONG lExtendID,const CGUID& guid)
{
	if( lExtendID==8 )
	{
		//GetGame()->GetInterface()->GetDepotPage()->SetguDepotMoneyID(guid);
	}
	else if( lExtendID==PEI_WALLET )
	{
		SetguMoneyID(guid);
	}
	else if( lExtendID==PEI_SILVERWALLET )
	{
		SetguSilverMoneyID(guid);
	}
	else if( lExtendID==PEI_YUANBAO )
	{
		SetguYuanBaoID(guid);
	}
	else if( lExtendID==6 )
	{
		SetguJiFenID(guid);
	}
}

// ����Ҫ���ص���Ʒ
bool CPlayer::AddBuyBackGoods(CGoods* pBuyBackGoods)
{
	if (pBuyBackGoods!=NULL)
	{
		if (m_queBuyBackGoodsItem.size()>=BuyBackGoodsNum)
		{
			DelBuyBackGoods();
		}
		if (pBuyBackGoods!=NULL)
		{
			m_queBuyBackGoodsItem.push_back(pBuyBackGoods);
		}
		return true;
	}
	return false;
}

// ɾ��һ���ع������е���Ʒ(Ĭ�ϰ����е��Ƚ��ȳ�ɾ������ͷ������) 
bool CPlayer::DelBuyBackGoods(int index)
{
	if (!m_queBuyBackGoodsItem.empty())
	{
		if (index == -1)
		{
			CGoods* pGoods = m_queBuyBackGoodsItem.front();
			SAFE_DELETE(pGoods);
			m_queBuyBackGoodsItem.pop_front();
		}
        else if (index >= 0 && index < (WORD)m_queBuyBackGoodsItem.size())
		{
			deque<CGoods*>::iterator GoodsIter = m_queBuyBackGoodsItem.begin();
			for (int i = 0; i < (WORD)m_queBuyBackGoodsItem.size(); i++,GoodsIter++)
			{
				if (i==index)
				{
					break;
				}
			}
			if (GoodsIter!=m_queBuyBackGoodsItem.end())
			{
				SAFE_DELETE(*GoodsIter);
				if (m_queBuyBackGoodsItem.size()==1)
				{
					m_queBuyBackGoodsItem.clear();
				}else
				{
					m_queBuyBackGoodsItem.erase(GoodsIter);
				}
			}

		}
		return true;
	}
	return false;
}

/*
*	���ܣ��Զ�װ����Ʒ��װ����
*	ϸ���������ڱ�������һ������Ʒ��ʱ����������Ʒ����װ����
*				��ԭװ������װ�����������������װ����Ϣ��
*	������	pGoods - ��Ʒ
*			lSExtend - ��Դ������ֻ���ձ������ӱ����������е���Դ��
*			lDExtend - Ŀ��������һ���Ǳ������ӱ�����Ч��
*			dwDPos	 - Ŀ�ĵ�λ�ã��������ӱ����е�λ�ã�
*	����ֵ�� true - ����װ���������������װ����Ϣ��false - ����װ��
*	����: ����
*	��������: 09.10.23
*	�޸���־: -
*/
bool CPlayer::AutoEquip(CGoods * pGoods,long lSExtend,long lDExtend,DWORD dwDPos)
{
	if ( pGoods && IS_PACKETCONTAINER(lDExtend) &&	// ��ƷҪ���뱳�����ӱ���
		! IS_PACKETCONTAINER(lSExtend)  &&			// ��Ʒ��Դ���Ǳ������ӱ���
		! CDepot::Inst()->IsDepotGC(lSExtend) &&	// ��Ʒ��Դ���ǲֿ�
		PEI_EQUIPMENT != lSExtend &&				// ��Ʒ��Դ����װ����
		CanEquip(pGoods) )							// ����Ʒ�ɱ�װ��
	{
		// �õ�����Ʒ�Ļ������ͣ���ӦeGoodsBaseType��
		//DWORD dwType = pGoods->GetProperty()->dwType;
		DWORD equipType = (DWORD)pGoods->GetEquipType();
		WORD wAction = GetAction();
		DWORD dwBind = pGoods->GetEffectVal(GAP_BIND, 1);
		// ѣ�λ�ս��ʱ���Զ�������
		// װ����󶨣����Զ���
		if( (equipType == EC_E_WEAPON && wAction > CShape::ACT_MEGRIM && wAction != CShape::ACT_PICK) || 
			eBT_DressAndCantUntie == dwBind  ||  eBT_DressAndBind == dwBind)
			return false;
		//CItemPageEx * pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
		// ����Ʒ���Ǽ�ʸ������
		if (/*dwType != GT_ARROW && */equipType != EC_E_RIDER )
		{
			// ����Ŀ������Ϊװ����
			//long lDExtendId = PEI_EQUIPMENT;
			long nIndex = -2;
			if (equipType>EC_MEDAL3)
			{
				pGoods->SetHaveShadowState(false);
				//if ( lDExtend == PEI_PACKET )
				//	pPage->UpdataItem(dwDPos);
				//else if ( lDExtend >= PEI_PACK1 && lDExtend <= PEI_PACK5 )
				//	pPage->UpdataSubPackData(lDExtend-PEI_PACK1,dwDPos);
				return false;
			}

			// ԭװ����λû����Ʒ���Զ�װ��
			if ( !GetEquip(equipType) )
			{
				//pPage->ChangeEquip(nIndex,dwDPos,PEI_EQUIPMENT,lDExtend);
				return true;
			}
		}
	}
	return false;
}

/*
* ����: ������Ʒ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.12.02 - lpf
*		�����˲�����̬������ͬʱ,���л�װ,����ֵĶ�������������;
*/
bool CPlayer::AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID)
{
	bool results = false;
	bool bDeleteGoods = false;
	if(pGoods==NULL) return results;

	// ˢ�¸����̵��е���ʾ
	//CPlayerShopPageEx* pPlayerShopPage = GetGame()->GetCGuiEx()->GetPlayerShopPageEx();
	//if (pPlayerShopPage!=NULL&&pPlayerShopPage->IsOpen()&&pPlayerShopPage->GetCurShopState()==CPlayerShopPageEx::SET_SHOP)
	//{
	//	//CGoods* pUpdateGoods = GetGoodsByGUID(SourceGUID);
	//	int iPos = -1;
	//	if (pPlayerShopPage->IsItemReadyToTrade(pGoods->GetIndex()))
	//	{
	//		pPlayerShopPage->UpdateMyShopList(pGoods->GetExID(),pGoods->GetIndex(),lExtendID,dwPos,pGoods->GetNum());
	//		pPlayerShopPage->Open();
	//	}
	//}

	if( lExtendID==10)
	{
		//GetGame()->GetInterface()->GetContainerPage()->SetGoods(pGoods);
	}
	else if( lExtendID==9 )
	{
		//GetGame()->GetInterface()->GetDepotPage()->AddGoods(pGoods,dwPos);
	}
	else if( lExtendID==8 )
	{
		//GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-09.wav");
		//GetGame()->GetInterface()->GetDepotPage()->SetMoney(pGoods->GetNum());
		//delete pGoods;
	}
	else if( lExtendID==7 )
	{
		//GetGame()->GetInterface()->GetEquipUpgradePage()->SetGoods(pGoods);
	}
	else if( lExtendID==PEI_WALLET )
	{
		//CItemPageEx* pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
		GetGame()->GetAudioList()->Play2DSound("SOUNDS\\equipments\\00018.wav"); 
		//SetguMoneyID(pGoods->GetExID());
		if (m_dwMoney == 0)
			SetguMoneyID(pGoods->GetExID());
		SetMoney(m_dwMoney+pGoods->GetNum());
		/*if (pPage)
		{
			char temp[32];
			sprintf(temp,"%d",m_dwMoney);
			pPage->UpdataYBMoney("Gold",temp);
		}*/
		//SAFEDELETE(pGoods);
		return true;
	}
    else if (lExtendID==PEI_SILVERWALLET)
	{
		//CItemPageEx* pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
		GetGame()->GetAudioList()->Play2DSound("SOUNDS\\equipments\\00018.wav");
		//SetguSilverMoneyID(pGoods->GetExID());
		if (m_dwSilverMoney == 0)
			SetguSilverMoneyID(pGoods->GetExID());
		SetSilverMoney(m_dwSilverMoney+pGoods->GetNum());
		/*if (pPage)
		{
			char temp[32];
			sprintf(temp,"%d",m_dwSilverMoney);
			pPage->UpdataYBMoney("Silver",temp);
		}*/
		//SAFEDELETE(pGoods);
		return true;
	}
	else if( lExtendID==PEI_YUANBAO )
	{
		//CItemPageEx* pPage = GetGame()->GetCGuiEx()->GetItemPageEx();
		SetYuanBao(m_dwYuanBao+pGoods->GetNum());
		//SetguYuanBaoID(pGoods->GetExID());
		if (m_dwYuanBao == 0)
			SetguYuanBaoID(pGoods->GetExID());
		/*if (pPage)
		{
			char temp[32];
			sprintf(temp,"%d",m_dwYuanBao);
			pPage->UpdataYBMoney("Gem",temp);
		}*/
		//SAFEDELETE(pGoods);
		return true;
	}
	else if( lExtendID==PEI_JIFEN )
	{
		SetJiFen(pGoods->GetNum());
		//SAFEDELETE(pGoods);
		return true;
	}
	else if( lExtendID==3 )		// ����
	{
		//GetGame()->GetInterface()->GetItemsPage()->SetDragState(true);
		//tagGoods stGoods;
		//stGoods.pGoods=pGoods;
		//stGoods.bChange=false;
		//stGoods.lNum=pGoods->GetNum();
		//stGoods.lPos=dwPos;
		//SetHand(&stGoods);
		//results = true;
	}
	else if( lExtendID==PEI_EQUIPMENT )		// װ��
	{
		//CCharacterPageEx *pCharacterPage = GetGame()->GetCGuiEx()->GetCharacterPageEx();
		//CSkillPageEx * pSkillPage = GetGame()->GetCGuiEx()->GetSkillPageEx();
		//if (pCharacterPage==NULL) return false;
		SetEquip(dwPos,pGoods);
		pGoods->SetHaveShadowState(false);
		//DWORD index = pGoods->GetIndex();
		//WORD suitID = CGoodsList::GetEffectVal(index,CGoods::GAP_SUITID,1);
		//if (suitID)
		//{
		//	string strName = pGoods->GetProperty()->strOrginName;
		//	SetupEquip(suitID,strName);
		//}
		//pCharacterPage->UpdataEquip(dwPos);
        
		char str[256]="";
        sprintf(str,AppFrame::GetText("Equip_16"),pGoods->GetName());
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText(eNOTIFYPOS_CENTER,str,D3DCOLOR_ARGB(255,255,0,0));
		// ��װ��������������������������ϰ�ü���ҳ��Ļ�����������ͼ��
		//if(pSkillPage && (dwPos == EQUIP_WEAPON || dwPos == EQUIP_WEAPON2||dwPos == EQUIP_WING||dwPos == EQUIP_HEADGEAR))
		//{
		//	pSkillPage->UpdataPlayerBaseSkill();

        if(dwPos == EQUIP_WEAPON ||dwPos == EQUIP_WING||dwPos == EQUIP_P_HEADGEAR)
        {	// �ж�û�в�����̬����ʱ�Ÿ��� - Add by Lpf 2008-12-02
			if (!GetPlayStateOfActionAnim())
				UpdateWeaponShow(pGoods);
        }
		GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex()); 
        //�����������Խ���װ��ICON
        UpdateEquipmentIcon((eEquipIndex)dwPos);
		SetDisplayModelGroup();

		// ˢ�¶���
		//GetGame()->GetCGuiEx()->GetEquipEwearPageEx()->CheckEquip();
		extern bool g_bForceUpdate;
		g_bForceUpdate=true;
		SetAction(ACT_STAND);
		results = true;
	}
	else if (lExtendID==PEI_SUBPACK)					//�ӱ�����
	{
		//GetGame()->GetAudioList()->PlayEquipmentSound(pGoods->GetIndex());
		//CItemPageEx *pItems=GetGame()->GetCGuiEx()->GetItemPageEx();
		//CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		pGoods->SetHaveShadowState(false);
		if (dwPos>=PEI_PACK1&&dwPos<=PEI_PACK5)
		{				
			/*if (pItems!=NULL)
			{
				pItems->ClearPackData(dwPos-PEI_PACK1);
				SetSubPGoods(dwPos-PEI_PACK1,pGoods);
				pGoods = NULL;
				pItems->UpdataPackData(dwPos-PEI_PACK1);
				pItems->UpdataPack(dwPos-PEI_PACK1);
				if(pItems->GetSwitchForPack()&&pItems->IsOpen())
				{
					pItems->Close();
					pItems->Open();
				}else
					pItems->UpdataSubPackSize(dwPos-PEI_PACK1);
			}
			if (pMainBarPage!=NULL)
			{
				pMainBarPage->UpdataPacketInfor();
			}*/
			results = true;
		}
	}
	// �����ʱ�������λ
	else if (lExtendID==PEI_BUSINESPACKFIELD)
    

	{
		///ˢ�����ʱ�����λ�Ľ�����ʾ
		//CItemPageEx *pItems=GetGame()->GetCGuiEx()->GetItemPageEx();
		SetBussinesPack(pGoods);

		/*if (pItems!=NULL)
		{
			pItems->CloseBusinessPack();
			pItems->UpdateBusinessPackField();
		}*/
		///////////////////////////////////////////
		//SAFEDELETE(pGoods);
	}
	// ���ʱ�������
	else if (lExtendID==PEI_BUSINESSPACK)
	{
		//CItemPageEx *pItems=GetGame()->GetCGuiEx()->GetItemPageEx();
		tagGoods *pstGoods= GetBusinessGoods((WORD)dwPos);
		list<tagGoods>* pLGoods = NULL;
		pGoods->SetHaveShadowState(false);
		/*�����жϵ�����Ƿ�����Ʒ����������ж�Ҫ����ȥ����Ʒ��m_dwIndex�Ƿ�͵��������Ʒ��m_dwIndex��ͬ��
		�����ͬ����еݼӴ������������������µ���Ʒ*/
		if(pstGoods&& pstGoods->pGoods&&pstGoods->pGoods->GetIndex()==pGoods->GetIndex() )
		{
			long num = 0;
			num = pstGoods->pGoods->GetNum()+pGoods->GetNum();
			if (num<=pGoods->GetEffectVal(GAP_MAXOVERLAP,1)&&num>0)
			{				
				pstGoods->pGoods->SetNum(num);
				pstGoods->lNum=num;
				pstGoods->bChange=true;
				bDeleteGoods = true;
				//SAFEDELETE(pGoods);
			}
            else if(num>pstGoods->pGoods->GetEffectVal(GAP_MAXOVERLAP,1))
			{
				long lGoodsNum = pGoods->GetNum();
				pGoods->SetNum(pstGoods->pGoods->GetNum());
				pstGoods->pGoods->SetNum(lGoodsNum);
				pstGoods->lNum = lGoodsNum;
			}			
		}
		else 
		{
			tagGoods stGoods;
			stGoods.bChange=true;
			stGoods.pGoods=pGoods;
			stGoods.lPos=dwPos;
			stGoods.lType = PEI_BUSINESSPACK;
			stGoods.lNum=pGoods->GetNum();
			if (m_CoteBussinesPack!=NULL)
			{
				m_CoteBussinesPack->m_listCoteGoods.push_back(stGoods);
			}
			//pGoods = NULL;
		}
		/*�Խ��漰�����ݽ��и���*/
		/*if (pItems!=NULL&&pItems->IsBusinessPackOpen())
		{
			pItems->UpdateBusinessPack(dwPos);
			/// ˢ���;õ���ʾ
			pItems->UpdateBusinessPackDurability();
			/// ˢ���ݻ�����ʾ
			pItems->UpdateBusinessPackCapacity();
		}*/
		results = true;
	}
	else if( lExtendID==PEI_PACKET )				//ԭʼ����
	{
		//CItemPageEx *pItems=GetGame()->GetCGuiEx()->GetItemPageEx();
		//CMainBarPageEx* pMainBarPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		tagGoods *pstGoods= GetGoods((WORD)dwPos);
		list<tagGoods>* pLGoods = NULL;
		pGoods->SetHaveShadowState(false);
		/*�����жϵ�����Ƿ�����Ʒ����������ж�Ҫ����ȥ����Ʒ��m_dwIndex�Ƿ�͵��������Ʒ��m_dwIndex��ͬ��
		�����ͬ����еݼӴ������������������µ���Ʒ*/
		if(pstGoods&& pstGoods->pGoods&&pstGoods->pGoods->GetIndex()==pGoods->GetIndex() )
		{
			long num = 0;
			num = pstGoods->pGoods->GetNum()+pGoods->GetNum();
			if (num<=pGoods->GetEffectVal(GAP_MAXOVERLAP,1)&&num>0)
			{				
				pstGoods->pGoods->SetNum(num);
				pstGoods->lNum=num;
				pstGoods->bChange=true;
				bDeleteGoods = true;
				//SAFEDELETE(pGoods);
			}else if(num>pstGoods->pGoods->GetEffectVal(GAP_MAXOVERLAP,1))
			{
				long lGoodsNum = pGoods->GetNum();
				pGoods->SetNum(pstGoods->pGoods->GetNum());
				pstGoods->pGoods->SetNum(lGoodsNum);
				pstGoods->lNum = lGoodsNum;
			}			
		}
		else 
		{
			tagGoods stGoods;
			stGoods.bChange=true;
			stGoods.pGoods=pGoods;
			stGoods.lPos=dwPos;
			stGoods.lType = PEI_PACKET;
			stGoods.lNum=pGoods->GetNum();
			pLGoods = GetGoodsList();
			if (pLGoods)
			{
				pLGoods->push_back(stGoods);
			}
		}
		/*�Խ��漰�����ݽ��и���*/
        
        GetInst(PackagePage).UpdatePage(dwPos);
		results = true;
	}
	else if (lExtendID == PEI_PACK1||lExtendID == PEI_PACK2||lExtendID == PEI_PACK3||lExtendID == PEI_PACK4||lExtendID == PEI_PACK5) //�ӱ���
	{	
		tagGoods* pCoteGood = GetSubGoods((WORD)lExtendID-PEI_PACK1,(WORD)dwPos);
		list<tagGoods>* pLGoods = GetSubGoodslist((WORD)lExtendID-PEI_PACK1);
		pGoods->SetHaveShadowState(false);
		/*�����жϵ�����Ƿ�����Ʒ����������ж�Ҫ����ȥ����Ʒ��m_dwIndex�Ƿ�͵��������Ʒ��m_dwIndex��ͬ��
		�����ͬ����еݼӴ������������������µ���Ʒ*/
		if (pCoteGood&&pCoteGood->pGoods&&pCoteGood->pGoods->GetIndex()==pGoods->GetIndex())
		{
			long num = 0;
			num = pCoteGood->pGoods->GetNum()+pGoods->GetNum();
			if (num<=pGoods->GetEffectVal(GAP_MAXOVERLAP,1)&&num>0)
			{	
				pCoteGood->pGoods->SetNum(num);
				pCoteGood->lNum = num;
				pCoteGood->bChange = true;
				bDeleteGoods = true;
				//SAFEDELETE(pGoods);
			}
            else if (num>pCoteGood->pGoods->GetEffectVal(GAP_MAXOVERLAP,1))
			{
				long lGoodsNum = pGoods->GetNum();
				pGoods->SetNum(pCoteGood->pGoods->GetNum());
				pCoteGood->pGoods->SetNum(lGoodsNum);
				pCoteGood->lNum = lGoodsNum;
			}
		}
        else 
		{
			tagGoods stGoods;
			stGoods.bChange=true;
			stGoods.pGoods=pGoods;
			stGoods.lType = lExtendID;
			stGoods.lPos=dwPos;
			stGoods.lNum=pGoods->GetNum();
			if (pLGoods)
			{
				pLGoods->push_back(stGoods);
			}
		}		
		results = true;
	}
	// ���壺���� �ֿ����Ʒ�����Ϣ
	else if( CGCMana::Inst()->AddGoods( pGoods, lExtendID, dwPos ) )
	{
		results = true;

		// ���²ֿ�
	}

	if (results&&lExtendID!=3&&lExtendID!=PEI_SUBPACK)
	{	
		// �����Ʒ����װ����
		WORD SuitID = (WORD)pGoods->GetEffectVal(GAP_SUITID,1);			
		if (SuitID!=0)
		{
			string stOrName = pGoods->GetProperty()->strOrginName;	
			if (lExtendID==2)
			{
				SetupEquip(SuitID,stOrName);
			}
			AddSuit(SuitID,stOrName);
		}
	}
	if (bDeleteGoods)
	{
		SAFEDELETE(pGoods);
	}
    else
	{
		pGoods = NULL;
	}

	// ������ֵ�̵������ʾ
	if (results)
		FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);

	return results;
}



/*
* ����: �Ƴ���Ʒ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.12.02 - lpf
*		�����˲�����̬������ͬʱ,���л�װ,����ֵĶ�������������;
*/
CGoods *CPlayer::RemoveGoods(const CGUID& SourceGUID,const CGUID& DesGUID,DWORD dwPos,LONG lExtendID,DWORD dwAmount)
{
	// ���壺ɾ����Ʒʱˢ�½���
	
	bool bIsRemoveBack = false;
	CGoods* preGoods = NULL;
	CMainPlayerHand* pHand = 0;
	if (pHand!=NULL)
	{
		tagGoods *pstGoods = pHand->GetTGoodsOfMainPlayerHand();
		if( pstGoods!=NULL&&pstGoods->pGoods!=NULL&&pstGoods->pGoods->GetExID()==SourceGUID )
		{
			pHand->ClearMainPlayerHand();
		}
	}

	// ˢ�¸����̵��е���ʾ


	if( lExtendID==10 )	// ����
	{
		//CGoods *pGoods=GetGame()->GetInterface()->GetContainerPage()->GetGoods();
		//GetGame()->GetInterface()->GetContainerPage()->SetGoods(NULL);
	}
	else if( lExtendID==9 )	// �ֿ�
	{
		//CGoods *pGoods=NULL;//GetGame()->GetInterface()->GetDepotPage()->RemoveGoods(dwPos,dwAmount);
		//return preGoods;
	}
	else if( lExtendID==8 )	// ����
	{
		//GetGame()->GetInterface()->GetDepotPage()->SetMoney(GetGame()->GetInterface()->GetDepotPage()->GetMoney()-dwAmount);
		//preGoods=new CGoods();
		//preGoods->SetExID(SourceGUID);
		//preGoods->GetProperty()->dwType = eGoodsBaseType::GT_MONEY;
		//preGoods->SetNum(dwAmount);
		//return preGoods;
	}
	else if( lExtendID==7 )	// ����
	{
		//CGoods* pGoods=GetGame()->GetInterface()->GetEquipUpgradePage()->GetGoods();
		//GetGame()->GetInterface()->GetEquipUpgradePage()->SetGoods(NULL);
		//return NULL;//pGoods;
	}
	else if( lExtendID==PEI_WALLET )	// ���Ǯ��
	{
		SetMoney(GetMoney()-dwAmount);
		preGoods=new CGoods();
		preGoods->SetExID(SourceGUID);
		preGoods->SetIndex(CGlobeSetup::GetSetup()->dwMoneyIndex);
		preGoods->GetProperty()->dwType = GBT_MONEY;
		preGoods->SetNum(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",GetMoney());
		//pItems->UpdataYBMoney("Gold",strValue);
		//return preGoods;
	}
	else if( lExtendID==PEI_SILVERWALLET )	// ����Ǯ��
	{
		SetSilverMoney(GetSilverMoney()-dwAmount);
		preGoods=new CGoods();
		preGoods->SetExID(SourceGUID);
		preGoods->SetIndex(CGlobeSetup::GetSetup()->dwSilverMoneyIndex);
		preGoods->GetProperty()->dwType = GBT_SILVERMONEY;
		preGoods->SetNum(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",GetSilverMoney());
		//pItems->UpdataYBMoney("Silver",strValue);
		//return preGoods;
	}
	else if( lExtendID==PEI_YUANBAO )	// Ԫ��
	{
		SetYuanBao(GetYuanBao() - dwAmount);
		preGoods=new CGoods();
		preGoods->SetExID(SourceGUID);
		preGoods->SetType(TYPE_GOODS);
		preGoods->SetIndex(YUANBAO_INDEX);
		preGoods->SetNum(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",GetYuanBao());
		//pItems->UpdataYBMoney("Gem",strValue);
		//return pGoods;
	}
	else if( lExtendID==PEI_JIFEN )	// ����
	{
		SetJiFen(GetJiFen() - dwAmount);
		preGoods=new CGoods();
		preGoods->SetExID(SourceGUID);
		preGoods->SetType(TYPE_GOODS);
		preGoods->SetIndex(JIFEN_INDEX);
		preGoods->SetNum(dwAmount);
		//return pGoods;
	}

	else if( lExtendID==3 )	//��
	{
		//RemoveGoods(SourceGUID,DesGUID,pstGoods->lPos,pstGoods->lType,pstGoods->pGoods->GetNum());
		//GetGame()->GetInterface()->GetItemsPage()->SetDragState(false);
		//return pGoods;
	}
	else if( lExtendID==PEI_EQUIPMENT )	//װ��
	{
		//int nIndex=(int)dwPos;
		//CGoods *pGoods = NULL;

		preGoods=GetEquip(dwPos);
		SetEquip(dwPos,NULL);
		
		// ��װ��������������������������ϰ�ü���ҳ��Ļ�����������ͼ��
		if((dwPos == EQUIP_WEAPON ||dwPos == EQUIP_WING||dwPos == EQUIP_P_HEADGEAR))
		{
			// �ж�û�в�����̬����ʱ�Ÿ��� - Add by Lpf 2008-12-02
			if (!GetPlayStateOfActionAnim())
				RemoveWeaponShow(preGoods);
		}

		SetDisplayModelGroup();
        UpdateEquipmentIcon(eEquipIndex(dwPos));

	}
	// �����ʱ�������λ
	else if (lExtendID == PEI_BUSINESPACKFIELD)
	{
		
	}
	// ���ʱ�������
	else if (lExtendID==PEI_BUSINESSPACK)
	{
		int nIndex=(int)dwPos;
		if (m_CoteBussinesPack==NULL) return NULL;
		list<tagGoods> *pGoodList = &m_CoteBussinesPack->m_listCoteGoods;
		if (pGoodList==NULL)
		{
			return NULL;
		}
		CGoods *pTmp = NULL;
		list<CPlayer::tagGoods>::iterator it = pGoodList->begin();

		for(;it!=pGoodList->end();)
		{
			/*���ݴ����GUID������Ʒ�����ж�Ӧ����Ʒ*/
			if( it->pGoods->GetExID()==SourceGUID )
			{
				/*�������Ĳ�������С����Ʒ�����������ʾҪ���в�ֲ���������ֻ��������������Ʒ�Ĳ���*/
				if( it->pGoods->GetNum() > (WORD)dwAmount && dwAmount )
				{
					DWORD Id = it->pGoods->GetIndex();
					preGoods = it->pGoods;
					pTmp = new CGoods();
					CGoodsList::FillGoodsInfo(Id,pTmp);
					preGoods->SetNum(preGoods->GetNum()-dwAmount);
					pTmp->SetExID(DesGUID);
					pTmp->SetType(preGoods->GetType());
					pTmp->SetIndex(preGoods->GetIndex());
					pTmp->SetNum(dwAmount);
					pTmp->SetBaseProperty(preGoods->GetBaseProperty());
					pTmp->SetName(preGoods->GetName());
					pTmp->SetGraphicsID(preGoods->GetGraphicsID());
					it->lNum=preGoods->GetNum();
					preGoods = pTmp;
					it->pGoods->SetHaveShadowState(false);
				}
				else
				{
					preGoods = it->pGoods;
					it = pGoodList->erase(it);
				}					
				break;
			}
			else
			{
				it++;
			}
		}	
		
		bIsRemoveBack = true;
	}
	else if( lExtendID==PEI_PACKET )	//����
	{
		int nIndex=(int)dwPos;
		list<tagGoods> *pGoodList= GetGoodsList();
		if (pGoodList==NULL)
		{
			return NULL;
		}
		CGoods *pTmp = NULL;
		list<CPlayer::tagGoods>::iterator it = pGoodList->begin();

		for(;it!=pGoodList->end();)
		{
			/*���ݴ����GUID������Ʒ�����ж�Ӧ����Ʒ*/
			if( it->pGoods->GetExID()==SourceGUID )
			{
				/*�������Ĳ�������С����Ʒ�����������ʾҪ���в�ֲ���������ֻ��������������Ʒ�Ĳ���*/
				if( it->pGoods->GetNum() > (WORD)dwAmount && dwAmount )
				{
					DWORD Id = it->pGoods->GetIndex();
					preGoods = it->pGoods;
					pTmp = new CGoods();
					CGoodsList::FillGoodsInfo(Id,pTmp);
					preGoods->SetNum(preGoods->GetNum()-dwAmount);
					pTmp->SetExID(DesGUID);
					pTmp->SetType(preGoods->GetType());
					pTmp->SetIndex(preGoods->GetIndex());
					pTmp->SetNum(dwAmount);
					pTmp->SetBaseProperty(preGoods->GetBaseProperty());
					pTmp->SetName(preGoods->GetName());
					pTmp->SetGraphicsID(preGoods->GetGraphicsID());
					it->lNum=preGoods->GetNum();
					preGoods = pTmp;
					it->pGoods->SetHaveShadowState(false);
				}
				else
				{
					preGoods = it->pGoods;
					it = pGoodList->erase(it);
				}					
				break;
			}
			else
			{
				it++;
			}
		}	
		bIsRemoveBack = true;
        GetInst(PackagePage).UpdatePage(dwPos);
	}
	else if (lExtendID==PEI_SUBPACK)						//�ӱ�����
	{
		int nIndex=(int)dwPos;
		if (nIndex>=PEI_PACK1&&nIndex<=PEI_PACK5)
		{				
			preGoods = GetCotePackage(nIndex-PEI_PACK1)->pGoods;
			if (preGoods==NULL)
			{
				return NULL;
			}
			
		}
	}
	else if (lExtendID == PEI_PACK1||lExtendID == PEI_PACK2||lExtendID == PEI_PACK3||lExtendID == PEI_PACK4||lExtendID == PEI_PACK5)
	{
		int nIndex=(int)dwPos;
		CGoods *pTmp = NULL;
		list<tagGoods> *pGoodList = GetSubGoodslist((WORD)lExtendID-PEI_PACK1);
		if (pGoodList==NULL)
		{
			return NULL;
		}
		list<CPlayer::tagGoods>::iterator it = pGoodList->begin();
		for(;it!=pGoodList->end();)
		{
			/*���ݴ����GUID������Ʒ�����ж�Ӧ����Ʒ*/
			if( it->pGoods->GetExID()== SourceGUID)
			{
				/*�������Ĳ�������С����Ʒ�����������ʾҪ���в�ֲ���������ֻ��������������Ʒ�Ĳ���*/
				if( it->pGoods->GetNum() > (WORD)dwAmount && dwAmount )
				{
					DWORD Id = it->pGoods->GetIndex();
					preGoods=it->pGoods;
					pTmp=new CGoods();
					CGoodsList::FillGoodsInfo(Id,pTmp);
					preGoods->SetNum(preGoods->GetNum()-dwAmount);
					pTmp->SetExID(DesGUID);
					pTmp->SetType(preGoods->GetType());
					pTmp->SetIndex(preGoods->GetIndex());
					pTmp->SetNum(dwAmount);
					pTmp->SetBaseProperty(preGoods->GetBaseProperty());
					pTmp->SetName(preGoods->GetName());
					pTmp->SetGraphicsID(preGoods->GetGraphicsID());
					it->lNum=preGoods->GetNum();
					preGoods = pTmp;
					it->pGoods->SetHaveShadowState(false);
				}
				else
				{
					preGoods = it->pGoods;
					it = pGoodList->erase(it);
				}					
				break;
			}else
			{
				it++;
			}
		}	
		
		bIsRemoveBack = true;
	}
	// ���壺���� �ֿ����Ʒɾ����Ϣ
	else if( CGoods* pGoodsInDepot = CGCMana::Inst()->DelGoods( SourceGUID, DesGUID, dwAmount ) )
	{
		preGoods = pGoodsInDepot;
	}

	//����������ϵ���װ����������������ɾ����װ��Ϣ�Ĳ���
	if (preGoods)
	{
		WORD SuitID = (WORD)preGoods->GetEffectVal(GAP_SUITID,1);
		if (SuitID!=0)
		{
			string stOrName = preGoods->GetProperty()->strOrginName;
			if (lExtendID==2)
			{
				UnfixEquip(SuitID,stOrName);
			}
			RemoveEquipFromSuit(SuitID,stOrName);
			if (GetNumOfPossessionSuit(SuitID)==0)
			{
				RemoveSuit(SuitID);
			}
		}
		//------------�쿴�����б��Ƿ��и���Ʒ�йص�����׷����Ҫ������ʾ(07.9.28�������)----------- 
		if(bIsRemoveBack)
		{
			// ������ֵ�̵������ʾ
			FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
		}
	}

	// ˢ���ʼ���Ʒ��ʾ

	return preGoods;
}

//-----------07.9.14liuke�޸ģ����б����в��ң�-------------
long CPlayer::GetGoodsAmount(long nIdx)
{
	long ret = 0;
	WORD index = 0;
	while(GetGoodsList(index))
	{
		for(tagGoodsIter iter = GetGoodsList(index)->begin();
			iter!=GetGoodsList(index)->end(); ++iter)
		{
			if( iter->pGoods->GetIndex() == nIdx ) ret += iter->pGoods->GetNum();
		}
		++index;
	}
	return ret;
}

//������Ʒ���Ʋ��������������еĴ�����Ʒ������
long CPlayer::GetGoodsAmount(const char* cGoodsName)
{
	long ret = 0;
	WORD index = 0;
	while(GetGoodsList(index))
	{
		for(tagGoodsIter iter = GetGoodsList(index)->begin();
			iter!=GetGoodsList(index)->end(); ++iter)
		{
			if( strcmp(iter->pGoods->GetOrginName(),cGoodsName) == 0 ) ret += iter->pGoods->GetNum();
		}
		++index;
	}
	return ret;
}

//bool CPlayer::HasPetArount()
//{
//	return m_setPetID.size()>0;
//}
//
//long CPlayer::GetPetAmount()
//{
//	return m_setPetID.size();
//}

void CPlayer::SetEmotion(long lIndex,DWORD dwTime)
{
	//if(lIndex<0)
	//{
	//	m_pEmotionShower->Stop();
	//	return;
	//}
	//if(GetAction()!=ACT_STAND && GetAction()!=ACT_IDLE && this==GetGame()->GetMainPlayer() )
	//{
	//	//GetGame()->GetInterface()->AddText("�����ڲ���ʹ�ñ��飡",0xFFFF0000);
	//	return;
	//}
	//if(GetHorse()->IsVisible() && this==GetGame()->GetMainPlayer() )
	//{
	//	//GetGame()->GetInterface()->AddText("�����ڲ���ʹ�ñ��飡",0xFFFF0000);
	//	return;
	//}
	//if(IsPlayerShopOpened() && this==GetGame()->GetMainPlayer() )
	//{
	//	//GetGame()->GetInterface()->AddText("�����ڲ���ʹ�ñ��飡",0xFFFF0000);
	//	return;
	//}
	//if(IsDied() && this==GetGame()->GetMainPlayer() )
	//{
	//	//GetGame()->GetInterface()->AddText("�����ڲ���ʹ�ñ��飡",0xFFFF0000);
	//	return;
	//}
	//static DWORD dwLastEmotionTime=0;
	//if(GetCurTickCount()-dwLastEmotionTime<5000 && this==GetGame()->GetMainPlayer() )
	//{
	//	//GetGame()->GetInterface()->AddText("������Ƶ����ʹ�ñ��飡",0xFFFF0000);
	//	return;
	//}
	//dwLastEmotionTime=GetCurTickCount();
	//if(g_bDebug)
	//	LoadEmotionsConfig("data/emotions.ini");
	//if(vecEmotions.empty())
	//	LoadEmotionsConfig("data/emotions.ini");
	//m_pEmotionShower->Stop();
	//m_pEmotionShower->SetActionInfo(m_cAnimInfo.GetActionInfo());
	//m_pEmotionShower->Init(lIndex);
	//m_pEmotionShower->Start(dwTime);
	//if(this==GetGame()->GetMainPlayer())
	//{
	//	CMessage msg(MSG_C2S_SHAPE_EMOTION);
	//	msg.Add(GetType());
	//	msg.Add(GetExID());
	//	msg.Add(lIndex+1);
	//	msg.Send();
	//}
}

bool CPlayer::IsEmotionShowing()
{
	return !m_pEmotionShower->IsStopped();
}

//���õ�ǰ���õĹ���
void CPlayer::SetToolsID(DWORD dwBeavioralID,DWORD dwActionID)
{
	if (dwBeavioralID==CDeputyOccuSystem::OB_TaskCollection)
	{
		switch(dwActionID)
		{
		case 2:
			m_dwToolIndex = 318033;	//�󹤳�
			break;
		case 3:
			m_dwToolIndex = 318035;	//��ҩ����
			break;
		case 4:
			m_dwToolIndex = 318031;	//��ƤС��
			break;
		default:
			break;
		}
	}else if (dwBeavioralID==CDeputyOccuSystem::OB_Collection)
	{
		switch(this->GetAssOccu())
		{
		case DOccu_Smithing:
			m_dwToolIndex = 318033;	//�󹤳�
			break;
		case DOccu_Metallurgy:
			m_dwToolIndex = 318035;	//��ҩ����
			break;
		case DOccu_Sewing:
			m_dwToolIndex = 318031;	//��ƤС��
			break;
		default:
			break;
		}
	}else if (dwBeavioralID==CDeputyOccuSystem::OB_Facture)
	{
		switch(this->GetAssOccu())
		{
		case DOccu_Smithing:
			m_dwToolIndex = 318034;	//��������
			break;
		case DOccu_Metallurgy:
			m_dwToolIndex = 318036;	//�����ձ�
			break;
			//case DOccu_Sewing:
			//	m_dwToolIndex = 1180;	//���¼���
			//	break;
		default:
			break;
		}
	}else
	{
		m_dwToolIndex = 0;
	}
}

void CPlayer::SetAction(WORD l)
{
	DWORD dwCurTime = GetCurTickCount();
	//char strInfo[512]="";
	//_snprintf(strInfo,512,CStringReading::LoadText(eIDS_CONTROL,eIDS_MOVETEST_15),
	//	dwCurTime,l,GetTileX(),GetTileY());
	//PutStringToFile("MoveTest",strInfo);

	if(IsEmotionShowing())
	{
		if(l==ACT_IDLE||l==ACT_STAND)
			return;
	}
	if(!m_pEmotionShower->IsStopped())
		m_pEmotionShower->Stop();
	if( m_Horse.IsVisible() )
	{
		int eState = CHorse::HS_STAND ;
		switch( l )
		{
		case ACT_BEATTACK:
		case ACT_WALK:
		case ACT_RUN:
		case ACT_IDLE:	
		case ACT_STAND:
		case ACT_MISS:
		case ACT_MEGRIM:
			eState = l;
			break;
		}
		m_Horse.ChangeStatus( eState );

	}
	if( m_Wing.IsVisible() )
	{
		int eState = CWing::WING_STAND ;
		switch( l )
		{
		case ACT_WALK:
		case ACT_RUN:
		case ACT_IDLE:
		case ACT_STAND:
			eState = l;
			break;
		}
		m_Wing.ChangeStatus( eState );
	}
	if( m_HeadWear.IsVisible() )
	{
		int eState = CHeadWear::HeadWear_STAND ;
		switch( l )
		{
		case ACT_WALK:
		case ACT_RUN:
		case ACT_IDLE:
		case ACT_STAND:
			eState = l;
			break;
		}
		m_HeadWear.ChangeStatus( eState );
	}
	if (l >= ACT_MINE && l <= ACT_MINEMAX)
	{
		/*if (GetState() == STATE_FIGHT)
		{
		m_LeftHandWeapon.SetVisible(false);
		m_RightHandWeapon.SetVisible(false);
		}
		else
		{
		m_LeftHandWeapon.SetVisible(true);
		m_RightHandWeapon.SetVisible(true);
		}*/
		m_CollectTool.SetVisible(true);
		m_CollectTool.SetLocator(LOCATOR_RIGHTHAND);
		if (!GetToolsID())
		{
			m_CollectTool.SetVisible(false);
		}
		else
		{
			m_CollectTool.SetModelID(GetToolsID());
			m_CollectTool.SetDisplayModel();
		}

	}
	else
	{
		m_LeftHandWeapon.SetVisible(true);
		m_RightHandWeapon.SetVisible(true);
		m_CollectTool.SetModelID(0);
		m_CollectTool.SetVisible(false);
	}
	CMoveShape::SetAction(l);
	UpdateSpecialEquipment();

}
void CPlayer::SetState(WORD l)
{
	WORD state = GetState();
	CMoveShape::SetState(l);
	UpdateSpecialEquipment();
	// ����Լ�������״̬�ı仯��������ͼ���ϵ�����Ч��
	if(this == GetGame()->GetMainPlayer())
	{
		//CMainBarPageEx * pPage = GetGame()->GetCGuiEx()->GetMainBarPageEx();
		//if(pPage)
		//{
		//	// ս��״̬
		//	if(l == STATE_FIGHT && state != l)
		//		pPage->UpdataStateIcon(true);
		//	else if(l != STATE_FIGHT)
		//		pPage->UpdataStateIcon(false);
		//}
	}
}

/// �ı�ָ���������ĳ����Ʒ������������������ID�������������͡����������GUID����������ı���������
bool CPlayer::ChangeObjectAmount(LONG lContainerExtendID,LONG lObjectType,CGUID& guObjectID,DWORD dwAmount)
{
	bool ret = false;
	CGoods * pGoods = NULL;
	if(lContainerExtendID==PEI_WALLET)
	{
		
		SetMoney(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",dwAmount);
		
		ret = true;
        FireUIEvent("PackagePage","UpdateMoney");
	}
	else if(lContainerExtendID==PEI_SILVERWALLET)
	{
		
		SetSilverMoney(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",dwAmount);
		FireUIEvent("PackagePage","UpdateMoney");
		ret = true;
	}
	else if(lContainerExtendID==PEI_BUSINESSPACK)
	{
		
		tagGoodsIter it;
		if (m_CoteBussinesPack!=NULL)
		{
			it = m_CoteBussinesPack->m_listCoteGoods.begin();
			for(;it!=m_CoteBussinesPack->m_listCoteGoods.end();it++)
			{
				if( it->pGoods->GetExID()==guObjectID && it->pGoods->GetType()==lObjectType )
				{
					int nIndex = it->lPos;
					it->pGoods->SetNum(dwAmount);
					it->lNum=dwAmount;
					
					/// ˢ���ݻ�����ʾ
					ret = true;
					pGoods = it->pGoods;
					break;
				}
			}
		}
	}
	else if(lContainerExtendID==PEI_PACKET)
	{
		tagGoodsIter it;
		for(it=GetGoodsList()->begin();it!=GetGoodsList()->end();it++)
		{
			if( it->pGoods->GetExID()==guObjectID && it->pGoods->GetType()==lObjectType )
			{
				int nIndex = it->lPos;
				it->pGoods->SetNum(dwAmount);
				it->lNum=dwAmount;
				/*���½������ݼ���ʾ*/
				//pItems->ClearItemGoodData(nIndex);
				//pItems->UpdataItemData(nIndex);
				//pItems->UpdataItem(nIndex);
                GetInst(PackagePage).UpdatePage(nIndex);
				ret = true;
				pGoods = it->pGoods;
				break;
			}
		}
	}
	else if (lContainerExtendID<=PEI_PACK5&&lContainerExtendID>=PEI_PACK1)
	{
		
		tagGoodsIter it;
		list<tagGoods>* pLGoods = GetSubGoodslist((WORD)lContainerExtendID - PEI_PACK1);
		if (!pLGoods) return false;
		for(it=pLGoods->begin();it!=pLGoods->end();it++)
		{
			if( it->pGoods->GetExID()==guObjectID && it->pGoods->GetType()==lObjectType )
			{
				int nIndex = it->lPos;
				it->pGoods->SetNum(dwAmount);
				it->lNum=dwAmount;
				
				ret = true;
				pGoods = it->pGoods;
				break;
			}
		}
	}
	else if(lContainerExtendID==PEI_YUANBAO)
	{
		
		SetYuanBao(dwAmount);
		char strValue[64];
		sprintf(strValue,"%d",dwAmount);
		FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
		ret = true;
	}
	else if(lContainerExtendID==PEI_JIFEN)
	{
		SetJiFen(dwAmount);
		FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
		ret = true;
	}
	//------------�쿴�����б��Ƿ��и���Ʒ�йص�����׷����Ҫ������ʾ(07.9.28�������)----------- 
	if(pGoods)
	{
		// ˢ����Ʒ��ص�����
		UpdataQuestByGoods(pGoods);
		//CPlayerShopPageEx* pPlayerShopPage = GetGame()->GetCGuiEx()->GetPlayerShopPageEx();
		//if (pPlayerShopPage!=NULL&&pPlayerShopPage->IsOpen()&&pPlayerShopPage->GetCurShopState()==CPlayerShopPageEx::SET_SHOP)
		//{
		//	int lPos = -1;
		//	int iPos = -1;
		//	CGoods* pChangeGoods = GetGoodsByGUID(guObjectID,&lPos);
		//	if (pChangeGoods!=NULL&&pPlayerShopPage->IsItemOnShopPage(GetGoodsOnPlayer(lContainerExtendID,lPos),iPos))
		//	{
		//		pPlayerShopPage->UpdateMyShopList(guObjectID,pChangeGoods->GetIndex(),lContainerExtendID,lPos,dwAmount);
		//		pPlayerShopPage->SetbSetTrue(false);
		//		pPlayerShopPage->Open();
		//	}
		//}
		FireUIEvent(SHOPCITY_PAGE_NAME,SHOPCITY_PAGE_EVENT_UPDATEMONEYEDBOXES);
	}
	return ret;
}

bool CPlayer::ClearGoods()
{
	return false;
}


CEmotionShower::CEmotionShower(CPlayer *pPlayer)
{
	//m_pActionInfo=NULL;
	m_nCurIndex=-1;
	m_pSource=pPlayer;
}

CEmotionShower::~CEmotionShower()
{
	Stop();
}

//void CEmotionShower::SetActionInfo(GameModel::AnimInfo::tagActionInfo *pActionInfo)
//{
//	//m_pActionInfo=pActionInfo;
//}
void CEmotionShower::Init(int eType)
{
	if(eType<0||eType >= (int)vecEmotions.size()) return;
	m_stEmotion=vecEmotions[eType];
	m_stEmotion.nSoundChannel=-1;
}
void CEmotionShower::Start(DWORD dwTime)
{
	//��������ǰ�Ѿ��ڳ��������˱��飬ֻ��������������ʾ��ʾ����
	if( m_stEmotion.strText!="null" && dwTime==0 )
	{
		char msg[128];
		sprintf(msg,m_stEmotion.strText.c_str(),m_pSource->GetName());
		//GetGame()->GetInterface()->AddText(msg);
	}
	if( m_stEmotion.strSound!="null" )
	{
		//��������ǰ�Ѿ��ڳ������˱��飬ֻ��������������������
		if( dwTime==0 || m_stEmotion.nSoundLoops==0 )
		{
			sprintf(m_stEmotion.sounds,m_stEmotion.strSound.c_str(),m_pSource->GetOccupation());
			m_stEmotion.nSoundChannel=GetGame()->GetAudioList()->Play3DSound(m_stEmotion.sounds,m_stEmotion.nSoundLoops,m_pSource->GetPosY(),m_pSource->GetHeight(),m_pSource->GetPosX());
		}
	}
	m_nCurIndex=0;
	//��������������ʱ�䣬�����ƶ������ŵ�����������ÿ���������ŵ�ʱ����1��
	while(dwTime >= 1000 && m_nCurIndex < (WORD)m_stEmotion.vecActions.size())
	{
		if(m_stEmotion.vecTypes[m_nCurIndex]>0)
		{
			dwTime=0;
			break;
		}
		m_nCurIndex++;
		dwTime-=1000;
	}
	if(!IsStopped())
		ShowNextAction();
}
void CEmotionShower::ShowNextAction()
{
	/*if(m_pActionInfo==NULL)return;
	DWORD dwCurrentTime=GetCurTickCount();

	m_pActionInfo->bInterpolation = FALSE;
	m_pActionInfo->bCurActionLooped=(m_stEmotion.vecTypes[m_nCurIndex]!=1);
	m_pActionInfo->dwCurActionLoopCount = 0;
	m_pActionInfo->dwOldAction = m_pActionInfo->dwCurAction;
	m_pActionInfo->dwOldActionTimeElapse = m_pActionInfo->dwCurActionElapseTime;
	m_pActionInfo->dwInterpolationElapseTime = 0;
	m_pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
	m_pActionInfo->dwInterpolationTimeLength = 240;	
	m_pActionInfo->dwCurActionElapseTime = 0;
	m_pActionInfo->dwCurActionStartTime = dwCurrentTime;
	m_pActionInfo->dwCurAction = m_stEmotion.vecActions[m_nCurIndex];*/
}
bool CEmotionShower::IsStopped()
{
	//return m_nCurIndex<0||m_nCurIndex>=m_stEmotion.vecActions.size()||m_pActionInfo==NULL;
	return true;
}
void CEmotionShower::Stop()
{
	if(m_stEmotion.nSoundChannel>0)
	{
		GetGame()->GetAudioList()->Stop3DSound(m_stEmotion.sounds,m_stEmotion.nSoundChannel);
		//char msg[64];
		//sprintf(msg,"�ͷ�����:%s,%d",m_stEmotion.strSound.c_str(),m_stEmotion.nSoundChannel);
		//GetGame()->GetInterface()->AddText(msg);
		m_stEmotion.nSoundChannel=-1;
	}
	if(IsStopped())
		return;
	m_nCurIndex=-1;
	RestoreToNormal();
}
void CEmotionShower::RestoreToNormal()
{
	extern bool g_bForceUpdate;
	g_bForceUpdate=true;
	WORD wOriginAction=CShape::ACT_STAND;
	//if(m_pSource->GetState()==CShape::STATE_FIGHT)
	//	wOriginAction=m_pSource->GetAction();
	m_pSource->SetAction(wOriginAction);
}
void CEmotionShower::Update()
{
	
}

//�õ���ʾӵ�г�����IcongID
WORD CPlayer::GetDisplayOwneRegionIcon()
{
	if(m_OwnedRegions.size() > 0)
	{
		list<tagownedReg>::iterator it = m_OwnedRegions.begin();
		for(;it != m_OwnedRegions.end();it++)
		{
			if( (*it).wType == 2 )
				return 2;
		}
		return 1;
	}
	return 0;
}

//======================������ط�����07.9.6����ע�ͣ�=======================//
// ���ָ�������һ������
bool CPlayer::AddQuest(long lQuestID, long lStepID)
{
	const CQuest *pQuest = CQuestSystem::GetQuest(lQuestID);
	// �������ô��ڣ�������������б��м�����������¼
	if(pQuest && m_RoleQueue.AddOneRecord(lQuestID,lStepID))
	{
        OutputDebugStr(AppFrame::GetText("Quest_4"));
		CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(lQuestID);
		if(pRecord)
		{
			// �������
			OutputDebugStr(AppFrame::GetText("Quest_5"));
            CQuestSystem::SetTempQuestRecord(pRecord);
            FireUIEvent("Quest/Tab/Normal/Tree", "AddNormalQuest");
			OutputDebugStr(AppFrame::GetText("Quest_6"));
			// ��ʾ��Ϣ
			OutputDebugStr(AppFrame::GetText("Quest_7"));
			// ����������󣬽�����������Ӧ�ķ���NPC��������ʾ״̬�б����
			CMonster *pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pQuest->GetAttribute().GetNPCName().c_str()));
			if(pNpc)
			{
				pNpc->RemoveStateByQuest(CMonster::STATE_QUEST,lQuestID);
				OutputDebugStr(AppFrame::GetText("Quest_8"));
			}
			// ������������б��в��Ҳ����¸�NPC��ص�������ʾ��Ϣ
			UpdataQuestInfoAboutNpcByQuestID(lQuestID);
			// ��������ʱ������������������б�
			TidySearchQuestList();
			OutputDebugStr(AppFrame::GetText("Quest_9"));
			return true;
		}
	}
	return false;
}


// ɾ��һ������
const char * CPlayer::RemoveQuest(long lQuestID)
{
	// ��ո������µ����е�ͼ��Ч
	DelAllQuestInfoByRgn(lQuestID);
	// �����������б������������
	CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(lQuestID);
	if(pRecord)
	{
		const CQuest *pQuest = CQuestSystem::GetQuest(lQuestID);
		if(pQuest)
		{
            //�ӽ���ɾ��
            CQuestSystem::SetTempQuestRecord(pRecord);
            FireUIEvent("Quest/Tab/Normal/Tree", "DeleteNormalQuest");

			CMonster *pNpc = NULL;
			// ɾ������󣬽�������ĵ�ǰ����͵�ǰ�������������׷��NPC��������ʾ״̬�б����
			long lStep = pRecord->GetQuestStep();
			const CQuestStep* pStep = pQuest->GetStep(lStep);
			if(pStep)
			{
				// �ҵ��������ĵ�ǰ��������Ӧ��Ŀ��NPC�������¸�NPC��������ʾ״̬
				pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pStep->GetNPCName().c_str()));
				if(pNpc)
					pNpc->RemoveStateByQuest(CMonster::STATE_STEP,lQuestID,lStep);
				// ����Ҫ���µ�ǰ�����£�����׷��Ŀ��NPC��������ʾ״̬
				const CQuestStep* pStep = pQuest->GetStep(lStep);
				if(pStep)
				{
					long lNum = 0;
					const CQuestTrace* pTrace = pStep->GetQuestTraceHead();
					while(pTrace)
					{
						pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pTrace->GetNPCName().c_str()));
						if(pNpc)
							pNpc->RemoveStateByQuest(CMonster::STATE_TRACE,lQuestID,lStep,lNum);
						pTrace = pTrace->next;
						lNum++;
					}
				}
			}
			// ����ҵ������б���ɾ��������
            m_RoleQueue.DelOneRecord(lQuestID);
			
			// ɾ�������¼�󣬽���Χ����NPC������״̬ˢ��һ�Σ���������Ϊ���������ǰ������ʱ��ˢ�£�
			GetGame()->GetRegion()->UpdataAllNpcQuestState();
			//string strName=pQuest->GetAttribute().GetNPCName();
			//pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)strName.c_str()));
			//UpdataSpecialQuestInfoByNpc(pNpc);
			// ����������������б�
			TidySearchQuestList();
		}
	}
	return NULL;
}


// ����һ��������
bool CPlayer::UpdataQuestStep(long lQuestID, long lStepIndex)
{
	CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(lQuestID);
	if(pRecord)
	{
		const CQuest *pQuest = CQuestSystem::GetQuest(lQuestID);
		if(pQuest)
		{
			long lStep = pRecord->GetQuestStep();
			const CQuestStep* pStep = pQuest->GetStep(lStep);
			if(pStep)
			{
				CMonster *pNpc = NULL;
				// ��������֮ǰ������Ϣ���Ӷ�ӦNPC��������Ϣ�б���ɾ��
				pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pStep->GetNPCName().c_str()));
				if(pNpc)
					pNpc->RemoveStateByQuest(CMonster::STATE_STEP,lQuestID,lStep);
				// ����Ҫ���µ�ǰ�����£�����׷��Ŀ��NPC��������ʾ״̬
				const CQuestStep* pStep = pQuest->GetStep(lStep);
				if(pStep)
				{
					long lNum = 0;
					const CQuestTrace* pTrace = pStep->GetQuestTraceHead();
					while(pTrace)
					{
						pNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pTrace->GetNPCName().c_str()));
						if(pNpc)
							pNpc->RemoveStateByQuest(CMonster::STATE_TRACE,lQuestID,lStep,lNum);
						pTrace = pTrace->next;
						lNum++;
					}
				}
			}
		}
		// ���²���
		pRecord->JumpStepTo((uchar)lStepIndex);
		// ��ո������µ����е�ͼ��Ч
		DelAllQuestInfoByRgn(lQuestID);
		// ������������б��в��Ҳ����¸�NPC��ص�������ʾ��Ϣ
		UpdataQuestInfoAboutNpcByQuestID(lQuestID);
		return true;
	}
	return false;
}

// ����һ�����������׷��
bool CPlayer::UpdataQuestTarget(long lQuestID, long lIndex, long lValue,long lRegionID, long x, long y)
{
	CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(lQuestID);
	if(pRecord)
	{
		pRecord->SetParam(lIndex,lValue);
		long lEffectOk = 0;
		long lEffectNO = 0;
		// �ҵ���Ӧ������׷��
		const CQuestTrace* pTrace = CQuestSystem::GetQuestTrace(lQuestID,pRecord->GetQuestStep(),lIndex);
		if(pTrace)
		{
			lEffectOk = pTrace->GetNPCEffect_1();
			lEffectNO = pTrace->GetNPCEffect_2();
		}

		// ��Ӹ�׷����ص�һ����ͼ��Ч��Ϣ
		if(lRegionID > 0)
		{
			m_RoleQueue.SetRegionID(lQuestID,pRecord->GetQuestStep(),lIndex,lRegionID);
			m_RoleQueue.SetRegionX(lQuestID,pRecord->GetQuestStep(),lIndex,x);
			m_RoleQueue.SetRegionY(lQuestID,pRecord->GetQuestStep(),lIndex,y);
			stQuestInfoByRgn tagInfo;
			tagInfo.lQuestID = lQuestID;
			tagInfo.lIndex = lIndex;
			tagInfo.lRgnID = lRegionID;
			tagInfo.lRgnX = x;
			tagInfo.lRgnY = y;
			tagInfo.lEffectIDOK = lEffectOk;
			tagInfo.lEffectIDNO = lEffectNO;
			// ���ø���׷�ٵĵ�ͼ��Ч��Ϣ
			SetQuestInfoByRgn(lQuestID,lIndex,tagInfo);
		}
		// �����׷��û����ص�ͼ��Ч��ɾ�������Ϣ
		else
			DelQuestInfoByRgn(lQuestID,lIndex);
		
		UpdataQuestInfoAboutNpcByQuestID(lQuestID);
		return true;
	}
	return false;
}


// ����ָ�������NPC�����������ʾ��Ϣ
void CPlayer::UpdataQuestInfoAboutNpcByQuestID(long lQuestID)
{
	CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(lQuestID);
	if(pRecord)
	{
		const CQuest *pQuest = CQuestSystem::GetQuest(lQuestID);
		if(pQuest)
		{
			const CQuestStep* pStep = pQuest->GetStep(pRecord->GetQuestStep());
			if(pStep)
			{
				CMonster *pTNpc = NULL;
				// �Ƚϸò���ľ���׷��Ŀ��NPC
				long lParam = 0;
				long lNum = 0;
				// ��ʱ���������ڱ�ʾ�ò����Ƿ�ﵽ���������ÿ��׷�ٴﵽ���������
				bool bIsTraceFinish = false;
				bool bIsStepFinish = true;
				const CQuestTrace* pTrace = pStep->GetQuestTraceHead();
				while(pTrace)
				{
					string strID = pTrace->GetGoodsStrID();
					// �����׷�ٵ�Ŀ����ԭʼ������ʾ��Ҫ����ұ������Ҹ���Ʒ������Ȼ�����ø���׷����������
					if(!strID.empty())
					{
						CGoodsList::tagGoods2* ptgGoods = CGoodsList::GetProperty(strID.c_str());
						if(ptgGoods)
							lParam = GetGoodsAmount(ptgGoods->BaseProperty.dwIndex);
					}
					// �������
					else
						lParam = pRecord->GetParam(pTrace->GetParam());
					// ����׷�ٴﵽ�������
					if(lParam >= pTrace->GetSum())
						bIsTraceFinish = true;
					// û�дﵽ�������
					else
					{
						bIsTraceFinish = false;
						bIsStepFinish = false;
					}
					// ����ҿɼ���Χ�ڲ��Ҷ�Ӧ��NPC��������ڣ���������״̬
					pTNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pTrace->GetNPCName().c_str()));
					if(pTNpc)
						pTNpc->SetStateByQuest(CMonster::STATE_TRACE,pTrace->GetNPCEffect_1(),pTrace->GetNPCEffect_2(),
						bIsTraceFinish,lQuestID,pRecord->GetQuestStep(),lNum);
					pTrace = pTrace->next;
					lNum++;
				}
				// �����Ŀ��NPC������
				pTNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pStep->GetNPCName().c_str()));
				if(pTNpc)
				{
					pTNpc->SetStateByQuest(CMonster::STATE_STEP,pStep->GetNPCEffect_1(),pStep->GetNPCEffect_2(),
						bIsStepFinish,lQuestID,pRecord->GetQuestStep());
				}
			}			
		}
	}
    FireUIEvent("Quest/Tab","UQuestText");
}

// ����ָ��NPC������������б��е����������ʾ��Ϣ
void CPlayer::UpdataQuestInfoAboutNpc(CMonster *pNpc)
{
	CMonster * pTNpc = pNpc;
	// �ڱ���ҵ������б��в������NPC��ص�����Ŀ��NPC����������ЩNPC�����񣨻�����Ĳ����׷�٣���״̬
	list<CQuestRecord*>::iterator it = m_RoleQueue.GetQuestRecordList()->begin();
	for(; it!=m_RoleQueue.GetQuestRecordList()->end(); ++it)
	{
		const CQuest *pQuest = CQuestSystem::GetQuest((*it)->GetQuestID());
		if(pQuest)
		{
			const CQuestStep* pStep = pQuest->GetStep((*it)->GetQuestStep());
			if(pStep)
			{
				// �Ƚϸò���ľ���׷��Ŀ��NPC
				long lParam = 0;
				long lNum = 0;
				// ��ʱ���������ڱ�ʾ�ò����Ƿ�ﵽ���������ÿ��׷�ٴﵽ���������
				bool bIsTraceFinish = false;
				bool bIsStepFinish = true;
				const CQuestTrace* pTrace = pStep->GetQuestTraceHead();
				while(pTrace)
				{
					string strID = pTrace->GetGoodsStrID();
					// �����׷�ٵ�Ŀ����ԭʼ������ʾ��Ҫ����ұ������Ҹ���Ʒ������Ȼ�����ø���׷����������
					if(!strID.empty())
					{
						CGoodsList::tagGoods2* ptgGoods = CGoodsList::GetProperty(strID.c_str());
						if(ptgGoods)
							lParam = GetGoodsAmount(ptgGoods->BaseProperty.dwIndex);
					}
					// �������
					else
						lParam = (*it)->GetParam(pTrace->GetParam());
					// ����׷�ٴﵽ�������
					if(lParam >= pTrace->GetSum())
						bIsTraceFinish = true;
					// û�дﵽ�������
					else
					{
						bIsTraceFinish = false;
						bIsStepFinish = false;
					}
					// ���ָ��NPCΪ�գ���ô���¸�׷������ʾ��NPC��������ʾ״̬
					if(pTNpc==NULL)
					{
						// ����ҿɼ���Χ�ڲ��Ҹ�NPC��������ڣ���������״̬
						pTNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pTrace->GetNPCName().c_str()));
						// ��NPC�Ǳ����Ļ���������
						if(pTNpc && (pTNpc->GetCountry() == GetCountry() || pTNpc->GetCountry() == 7))
							pTNpc->SetStateByQuest(CMonster::STATE_TRACE,pTrace->GetNPCEffect_1(),pTrace->GetNPCEffect_2(),
							bIsTraceFinish,(*it)->GetQuestID(),(*it)->GetQuestStep(),lNum);
					}
					// �ҵ�����׷�ٵ����NPC
					else if(strcmp(pTNpc->GetOrigName(), pTrace->GetNPCName().c_str()) == 0)
					{
						if(pTNpc->GetCountry() == GetCountry() || pTNpc->GetCountry() == 7)
						{
							pTNpc->SetStateByQuest(CMonster::STATE_TRACE,pTrace->GetNPCEffect_1(),pTrace->GetNPCEffect_2(),
								bIsTraceFinish,(*it)->GetQuestID(),(*it)->GetQuestStep(),lNum);
						}
					}
					pTrace = pTrace->next;
					lNum++;
				}
				// �Ƚϲ����Ŀ��NPC������
				pTNpc = dynamic_cast<CMonster *>(GetGame()->GetRegion()->RecursiveFindObject(TYPE_MONSTER,(char *)pStep->GetNPCName().c_str()));
				if(pTNpc)
				{
					pTNpc->SetStateByQuest(CMonster::STATE_STEP,pStep->GetNPCEffect_1(),pStep->GetNPCEffect_2(),
						bIsStepFinish,(*it)->GetQuestID(),(*it)->GetQuestStep());
				}
			}			
		}
	}
}


// ���³����ϵ������������NPC����ʾ��Ϣ
void CPlayer::UpdataSpecialQuestInfoByNpc(CMonster* pNpc)
{
	if (!pNpc) return;
	// ��NPC���Ǳ�����ͬʱҲ���������ģ�����
	if (pNpc->GetCountry() != GetCountry() && pNpc->GetCountry() != 7)
		return;
	// ��ø�NPC��Ϊ���������ܹ�����������б�
	vector<LONG> vec_quest = CQuestSystem::GetSpecialQuestByNPCID(pNpc->GetOrigName());
	for (int i=0; i < (int)vec_quest.size(); ++i)
	{
		const CQuest* pQuest = CQuestSystem::GetQuest(vec_quest[i]);
		// ���������
		if (pQuest)
		{
			// �����������
			const CQuestAttribute attrib = pQuest->GetAttribute();

            // ǰ�������б�
			bool bNoPreQuest = true;
			for (uint size = 0; size < attrib.GetPreQuestList().size(); ++size)
			{
				long id = attrib.GetPreQuestList()[size];
				// ������û�������������¼��
				if(!m_RoleQueue.GetQuestFinishedRecord(id))
				{
					bNoPreQuest = false;
					break;
				}
			}
			// ǰ������û����
			if(!bNoPreQuest)
				continue;
			// �����ߵȼ�����
			int iMin = attrib.GetMinGrade();
			int iMax = attrib.GetMaxGrade();
			WORD dwlev = GetLevel();
			// ��������ϲ鿴�Ƿ���ڸ�����,
			CQuestRecord* pRecord = m_RoleQueue.GetRecordByID(vec_quest[i]);
			// ���û�У�ͬʱ��������û�������������¼��
			if(!pRecord && !m_RoleQueue.GetQuestFinishedRecord(vec_quest[i]))
			{
				// ְҵ�����͸�ְҵ�������ﵽҪ����ʾ������ʾ
				if(attrib.GetOccupationRestrict(GetOccupation()) &&
					attrib.GetDOccuRestrict((BYTE)GetAssOccu()))
				{
					// �ȼ������ﵽ
					if(dwlev >= iMin && dwlev <= iMax)
						pNpc->SetStateByQuest(CMonster::STATE_QUEST,attrib.GetNPCShowType(),attrib.GetNPCClewType(),true,vec_quest[i]);
					// �ȼ����������ﵽ
					else if(dwlev >= iMin - 3 && dwlev < iMin)
						pNpc->SetStateByQuest(CMonster::STATE_QUEST,attrib.GetNPCShowType(),attrib.GetNPCClewType(),false,vec_quest[i]);
				}
			}
		}
	}
}
// �ɼ�������NPC��Ӧ��������ʾ��Ϣ
void CPlayer::CollectionQuestClewInfo(CMonster *pNpc)
{
	if(!pNpc) return;
	UpdataSpecialQuestInfoByNpc(pNpc);
	// ������������б��в��Ҳ����¸�NPC��ص�������ʾ��Ϣ
	UpdataQuestInfoAboutNpc(pNpc);
}

// ��Ʒ�仯������������
void CPlayer::UpdataQuestByGoods(CGoods *pGoods)
{
	if(pGoods)
	{
		list<CQuestRecord*>::iterator itrec = m_RoleQueue.GetQuestRecordList()->begin();
		for(; itrec != m_RoleQueue.GetQuestRecordList()->end(); ++itrec)
		{
			CQuestRecord* pRecord = (*itrec);
			if(pRecord)
			{
				const CQuest *pQuest = CQuestSystem::GetQuest(pRecord->GetQuestID());
				if(pQuest)
				{
					// ��ø�����ǰ���ڵĲ���
					bool bIsFind = false;
					const CQuestStep* pStep = pQuest->GetStep(pRecord->GetQuestStep());
					if(pStep)
					{
						const CQuestTrace* pTrace = pStep->GetQuestTraceHead();
						while(pTrace)
						{
							// ����׷����ص���Ʒ�����¸������׷��˵����ʾ
							if(pTrace->GetGoodsStrID() == pGoods->GetOrginName())
							{
								//pQuestPage->UpdataQuestData(pRecord,false);
								//pQuestPage->CheckQuestIsFinish(pRecord);
								bIsFind = true;
								break;
							}
							pTrace = pTrace->next;
						}
						if(bIsFind)
							break;
					}
				}
			}
		}
		// �����ұߵ�����׷����ʾ��Ϣ
		//pQuestPage->UpdataRightTrace();
		// ���³����ɼ�NPC��������ʾ��Ϣ
		UpdataQuestInfoAboutNpc();
	}
}


// ����ָ�������ָ�����еĵ�ͼ��Ч
void CPlayer::SetQuestInfoByRgn(long lQuestID, long lIndex, CPlayer::stQuestInfoByRgn tagInfo)
{
	list<stQuestInfoByRgn>::iterator it = m_lsQuestInfoByRgn.begin();
	for(; it!=m_lsQuestInfoByRgn.end(); ++it)
	{
		if((*it).lQuestID == lQuestID && (*it).lIndex == lIndex)
		{
			(*it).lRgnID = tagInfo.lRgnID;
			(*it).lRgnX = tagInfo.lRgnX;
			(*it).lRgnY = tagInfo.lRgnY;
			(*it).lEffectIDOK = tagInfo.lEffectIDOK;
			(*it).lEffectIDNO = tagInfo.lEffectIDNO;
			// ��ͼ��Ч�л���������ԭ�����������Чɾ��
			if(GetGame()->GetRegion())
			{
				GetGame()->GetRegion()->DeleteChildObject(TYPE_EFFECT,(*it).guidEffect);
				(*it).guidEffect = CGUID::GUID_INVALID;
			}
			//if((*it).pEffect)
			//{
			//(*it).pEffect->SetIsDeleted(true);
			//(*it).pEffect = NULL;
			//}
			return;
		}
	}
	m_lsQuestInfoByRgn.push_back(tagInfo);
}

// ɾ��ָ�������ָ�����еĵ�ͼ��Ч
void CPlayer::DelQuestInfoByRgn(long lQuestID, long lIndex)
{
	list<stQuestInfoByRgn>::iterator it = m_lsQuestInfoByRgn.begin();
	for(; it!=m_lsQuestInfoByRgn.end(); ++it)
	{
		if((*it).lQuestID == lQuestID && (*it).lIndex == lIndex)
		{
			if((*it).guidEffect != CGUID::GUID_INVALID && GetGame()->GetRegion())
			{
				GetGame()->GetRegion()->DeleteChildObject(TYPE_EFFECT,(*it).guidEffect);
				(*it).guidEffect = CGUID::GUID_INVALID;
			}
			//if((*it).pEffect)
			//{
			//GetGame()->GetRegion()->DeleteChildObject(TYPE_EFFECT,(*it).pEffect->GetExID());
			//(*it).pEffect->SetIsDeleted(true);
			//(*it).pEffect = NULL;
			//}
			m_lsQuestInfoByRgn.erase(it);
			return;
		}
	}
}

// ɾ��ָ����������е�ͼ��Ч
void CPlayer::DelAllQuestInfoByRgn(long lQuestID)
{
	list<stQuestInfoByRgn> lsQuestInfoByRgn = m_lsQuestInfoByRgn;
	list<stQuestInfoByRgn>::iterator it = lsQuestInfoByRgn.begin();
	for(; it!=lsQuestInfoByRgn.end(); ++it)
	{
		if((*it).lQuestID == lQuestID)
			DelQuestInfoByRgn(lQuestID,(*it).lIndex);
	}
}

// ����������������б��ﵽ�ɽ���������������
void CPlayer::TidySearchQuestList()
{
	m_vecCanSearchQuest.clear();
	//vector<CQuest*> vecQuest = CQuestSystem::GetBeSearchQuestList();
	//vector<CQuest *>::iterator it = vecQuest.begin();
	//for (; it != vecQuest.end();++it)
	//{
	//	CQuest *pQuest = (*it);
	//	if(pQuest)
	//	{
	//		const CQuestAttribute attrib = pQuest->GetAttribute();
	//		// �ȼ���������
	//		if(attrib.GetSpDescTextIDBySearch() > 0 && GetLevel() >= attrib.GetMinGrade() && GetLevel() <= attrib.GetMinGrade()+3)
	//		{
	//			// ǰ�������б�
	//			bool bNoPreQuest = true;
	//			for (int i=0;i<attrib.GetPreRoleList().size();++i)
	//			{
	//				long id = attrib.GetPreRoleList()[i];
	//				// ������û�������������¼��
	//				if(!m_RoleQueue.GetRoleFinishedRecord(id))
	//				{
	//					bNoPreQuest = false;
	//					break;
	//				}
	//			}
	//			// ǰ������û����,��ǰ��������Ѿ������ˣ����ߵ�ǰ��������Ѿ����յ��ˣ���������һ��
	//			if(!bNoPreQuest || m_RoleQueue.GetRoleFinishedRecord(pQuest->GetID())
	//				|| m_RoleQueue.GetRecordByID(pQuest->GetID()))
	//				continue;
	//			// ְҵ�����͸�ְҵ�������ﵽҪ����ʾ������ʾ
	//			if(attrib.GetOccupationRestrict(GetOccupation()) &&
	//				attrib.GetDOccuRestrict(GetAssOccu()))
	//			{
	//				m_vecCanSearchQuest.push_back(pQuest);
	//			}
	//		}
	//	}
	//}
}

// ������һ��������������������
CQuest * CPlayer::RedomCanSearchQuest(bool bIsReSearch)
{
	if(bIsReSearch || m_vecCanSearchQuest.empty())
		TidySearchQuestList();
	else if(bIsReSearch)
		TidySearchQuestList();
	long size = (long)m_vecCanSearchQuest.size();
	if(size > 0)
	{
		size = random(size);
		return m_vecCanSearchQuest[size];
	}
	return NULL;
}

//======================================================================

//bool CPlayer::DecordQuestDataFromByteArray(BYTE* pByte, long& pos)
//{
//	/*char strTempt[2002];
//	m_lMaxQuestNum = _GetLongFromByteArray(pByte,pos);
//	long lNum = _GetLongFromByteArray(pByte,pos);
//	for(int i = 0; i < lNum; i++)
//	{
//		tagPlayerQuest PlayerQuest;
//		PlayerQuest.wQuestID = _GetWordFromByteArray(pByte,pos);
//		PlayerQuest.strName = _GetStringFromByteArray(pByte,pos,strTempt);
//		PlayerQuest.strDesc = _GetStringFromByteArray(pByte,pos,strTempt);
//		PlayerQuest.bDisplay=_GetCharFromByteArray(pByte,pos)==0?false:true;
//		PlayerQuest.lRegionID = _GetLongFromByteArray(pByte,pos);
//		PlayerQuest.lTileX = _GetLongFromByteArray(pByte,pos);
//		PlayerQuest.lTileY= _GetLongFromByteArray(pByte,pos);
//		PlayerQuest.lEffectID = _GetLongFromByteArray(pByte,pos);
//		PlayerQuest.pEffect = NULL;
//		m_PlayerQuests[PlayerQuest.wQuestID] = PlayerQuest;
//	}
//	GetGame()->GetInterface()->GetTaskPage()->UpdateList();*/
//	return true;
//}
//
////����������һ������
//void CPlayer::AddQuest(WORD wQuestID,char* strName,char* strDesc,bool bDisplay,long lEffectID,long lRegionID,long lTileX,long lTileY)
//{
//	tagPlayerQuest PlayerQuest = {wQuestID,bDisplay,lEffectID,lRegionID,lTileX,lTileY,NULL,strName,strDesc};
//
//	if(GetGame()->GetRegion() && (lRegionID==GetGame()->GetRegion()->GetExID() ||
//		GetGame()->GetDupliRegionSetup()->IsDupliRegion(lRegionID,GetGame()->GetRegion()->GetExID()) ) )
//	{
//		PlayerQuest.pEffect=((CClientRegion *)GetGame()->GetRegion())->PlayEffect(lEffectID,lTileX+0.5f,lTileY+0.5f);
//		PlayerQuest.pEffect->SetLoop(true);
//		PlayerQuest.pEffect->SetTask(true);
//	}
//	m_PlayerQuests[PlayerQuest.wQuestID] = PlayerQuest;
//}
////�Ƴ�һ������
//void CPlayer::RemoveQuest(WORD wQuestID)
//{
//	if(((CClientRegion *)GetGame()->GetRegion())->FindChildObject(m_PlayerQuests[wQuestID].pEffect))
//	{
//		m_PlayerQuests[wQuestID].pEffect->SetLoop(false);
//		m_PlayerQuests[wQuestID].pEffect->SetTask(false);
//		GetGame()->GetRegion()->RemoveObject(m_PlayerQuests[wQuestID].pEffect);
//	}
//	m_PlayerQuests.erase(wQuestID);
//}
//
//void CPlayer::UpdateQuestPosXY(WORD wQuestID,long lReginID,long lTileX,long lTileY)
//{
//	map<WORD,tagPlayerQuest>::iterator it = m_PlayerQuests.find(wQuestID);
//	if(it != m_PlayerQuests.end())
//	{
//		tagPlayerQuest& quest = (*it).second;
//		if( GetGame()->GetRegion()->FindChildObject(quest.pEffect) )
//		{
//			quest.pEffect->SetLoop(false);
//			quest.pEffect->SetTask(false);
//			GetGame()->GetRegion()->RemoveObject(quest.pEffect);
//		}
//		quest.lRegionID = lReginID;
//		quest.lTileX = lTileX;
//		quest.lTileY = lTileY;
//
//		if(GetGame()->GetRegion() && (lReginID==GetGame()->GetRegion()->GetExID() ||
//				GetGame()->GetDupliRegionSetup()->IsDupliRegion(lReginID,GetGame()->GetRegion()->GetExID()) ) )
//		{
//			quest.pEffect=((CClientRegion *)GetGame()->GetRegion())->PlayEffect(quest.lEffectID,lTileX+0.5f,lTileY+0.5f);
//			quest.pEffect->SetLoop(true);
//			quest.pEffect->SetTask(true);
//		}
//	};
//}

//==========================================================================//

//���ܵ�ѡ���߼�
DWORD CPlayer::GetDefaultAttackSkillID()
{
	//##0������;1�����ֽ�;2�����ִ�;3��ذ��;4������(˫��);5��˫�ֽ�(˫��);6��˫�ָ�(˫��);7����;8��˫��(˫��);9��ȭ��(˫��)
	//���ְҵ��ħ������װ���˹�
	CGoods* pWeapon = GetEquip( EQUIP_WEAPON );
	LONG lWeaponCategory = 0;
	if( pWeapon )
	{
		lWeaponCategory = pWeapon->GetEffectVal( GAP_WEAPON_CATEGORY, 1);
	}
	// ������������⣬���տ��ֱ�׼����
	if(lWeaponCategory < 0 || lWeaponCategory > 9)
		lWeaponCategory = 0;
	//if( lWeaponCategory == 7 || lWeaponCategory == 2 || lWeaponCategory == 3 || lWeaponCategory == 5 || lWeaponCategory == 6
	//	|| lWeaponCategory == 4 || lWeaponCategory == 8 || lWeaponCategory == 9)
	//{
	//	//��ǰ���������л�������
	//	return SKILL_BASE_ATTACK;
	//}
	//else if(lWeaponCategory == 1)
	//{
	//	//��������ͨ����
	//	return SKILL_BASE_ARCHERY;
	//}
	return SKILL_BASE_ATTACK+lWeaponCategory;
}


/*
*���ܣ�����ħ������
*ϸ�������� 
*������ eAD		����ö��
*		eMT		ħ��Ԫ��ö��
*����ֵ�� ��
*�޸�ʱ�䣺2000-11-4
*��д�ˣ�����
*/
void CPlayer::SetMagicElem( eAttackDefend eAD, eMagicElement eMT, long lVal )
{
	if( eAD < AD_Max && eMT < MT_Max )
	{
		m_arrElementProperty[ eMT ].uArrPro[ eAD ] = lVal;
	}
}

/*
*���ܣ���ȡħ������
*ϸ�������� 
*������ eAD		����ö��
*		eMT		ħ��Ԫ��ö��
*����ֵ�� ��Ӧ��ħ������ֵ
*�޸�ʱ�䣺2000-11-4
*��д�ˣ�����
*/
long CPlayer::GetMagicElem( eAttackDefend eAD, eMagicElement eMT )
{
	if( eAD < AD_Max && eMT < MT_Max )
	{
		return m_arrElementProperty[ eMT ].uArrPro[ eAD ];
	}

	return -1;
}


/*
*���ܣ���ȡ������
*ϸ�������� 
*������ ��
*����ֵ�� �ܵ����Ժ�
*�޸�ʱ�䣺2000-11-4
*��д�ˣ�����
*/
long CPlayer::GetNumOfXiangXing()
{
	long lNumOfXiangXing = 0;

	for( long c = 0; c < MT_Max; c++ )
	{
		lNumOfXiangXing += this->GetMagicElem( AD_AttackXiang, (eMagicElement)c );
		lNumOfXiangXing += this->GetMagicElem( AD_DefendXiang, (eMagicElement)c );
	}

	return lNumOfXiangXing;
}


/*
* ����: ������Ʒ
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.08.27
* �޸���־:
*	2008.09.22 - lpf
*		�����˶���ʾͷ�λ�����ʱ,û�л�װID��ShowGroup()���ж�,������û�л�װID��ͷ�����߻ص�ѡ�˻���ʱ,ģ����ʾ������������
*	2008.10.17 - lpf
*		�����˾ֲ����������ʼֵ;
*	2009.03.05 - lpf
*		�����˸����Լ�������ʱ,�����׿����Ƿ��������ж�;
*	2009.03.13 - lpf
*		������������ʾ�¹���:���������ͼ,�������Ƿ���ʾ����;
*	2009.03.27 - lpf
*		�����˵����׸�������Ϊ����ȫ��ʱ,Ҳ����������Ĺ���;
*/
void CPlayer::UpdateAccouterment()
{
	static char str[32];
	static DWORD dwFACE = MAKEFOURCC('F','A','C','E');
	static DWORD dwHAIR = MAKEFOURCC('H','A','I','R');
	static DWORD dwARMO = MAKEFOURCC('A','R','M','O');
	static DWORD dwHAND = MAKEFOURCC('H','A','N','D');
	static DWORD dwFOOT = MAKEFOURCC('F','O','O','T');
	static DWORD dwBODY = MAKEFOURCC('B','O','D','Y');
	static DWORD dwPIFE = MAKEFOURCC('P','I','F','E');		//����

	DWORD dwHead	 = 0;
	DWORD dwBody	 = 0;
	long  lHeadLevel = 0;
	long  lBodyLevel = 0;

	if (m_bSelf)
	{
		// ͷ�δ���
		CGoods * pGoods = GetEquip(CPlayer::EQUIP_P_HEADGEAR);
		if (pGoods != NULL)
		{
			dwHead	   = pGoods->GetProperty()->dwEquipID;
			lHeadLevel = pGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1);

			if (pGoods->GetEffectVal(GAP_HIDE_HEAR, 1) != 0)
				m_pDisplayModel->HideGroup(dwHAIR);
			if (pGoods->GetEffectVal(GAP_HIDE_FACE, 1) != 0)
				m_pDisplayModel->HideGroup(dwFACE);
			if (pGoods->GetEffectVal(GAP_HIDE_BODY, 1) != 0)
			{
				m_pDisplayModel->HideGroup(dwARMO);
				m_pDisplayModel->HideGroup(dwHAND);
				m_pDisplayModel->HideGroup(dwFOOT);
			}

			if (dwHead != 0)
			{
				/*sprintf(str, "%04d", dwHead);
				m_pDisplayModel->ShowGroup(dwHEAD, MAKEFOURCC(str[0],str[1],str[2],str[3]), lHeadLevel);*/
				m_HeadWear.SetVisible(true);
			}
		}else
			m_HeadWear.SetVisible(false);

		// ���״���
		if (GetGame()->GetRegion() == NULL ||
			(GetGame()->GetRegion() != NULL && GetGame()->GetRegion()->IsChangeEquip()))
		{
			if (GetShowFashion() & FASHION_TYPE_GREATCOAT/*!GetGame()->GetCGuiEx()->GetGameConfigPageEx()->GetOption(CGameConfigPageEx::OPTION_NoGreatcoat)*/)
			{
				pGoods = GetEquip(CPlayer::EQUIP_P_FROCK);
				if (pGoods != NULL)
				{
					dwBody	   = pGoods->GetProperty()->dwEquipID;
					lBodyLevel = pGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1);

					if (pGoods->GetEffectVal(GAP_HIDE_HEAR, 1) != 0)
						m_pDisplayModel->HideGroup(dwHAIR);
					if (pGoods->GetEffectVal(GAP_HIDE_FACE, 1) != 0)
						m_pDisplayModel->HideGroup(dwFACE);
					if (pGoods->GetEffectVal(GAP_HIDE_BODY, 1) != 0)
					{
						m_pDisplayModel->HideGroup(dwARMO);
						m_pDisplayModel->HideGroup(dwHAND);
						m_pDisplayModel->HideGroup(dwFOOT);
						m_pDisplayModel->HideGroup(dwPIFE);
					}

					if (dwBody != 0)
					{
						sprintf(str, "%04d", dwBody);
						m_pDisplayModel->ShowGroup(dwBODY, MAKEFOURCC(str[0],str[1],str[2],str[3]), lBodyLevel);
					}
				}else
					m_pDisplayModel->HideGroup(dwBODY);
			}else
				m_pDisplayModel->HideGroup(dwBODY);
		}else if (GetGame()->GetRegion() != NULL && !GetGame()->GetRegion()->IsChangeEquip())
		{
			pGoods = GetEquip(CPlayer::EQUIP_P_FROCK);
			if (pGoods != NULL)
			{
				dwBody	   = pGoods->GetProperty()->dwEquipID;
				lBodyLevel = pGoods->GetEffectVal(GAP_WEAPON_LEVEL, 1);

				if (pGoods->GetEffectVal(GAP_HIDE_HEAR, 1) != 0)
					m_pDisplayModel->HideGroup(dwHAIR);
				if (pGoods->GetEffectVal(GAP_HIDE_FACE, 1) != 0)
					m_pDisplayModel->HideGroup(dwFACE);
				if (pGoods->GetEffectVal(GAP_HIDE_BODY, 1) != 0)
				{
					m_pDisplayModel->HideGroup(dwARMO);
					m_pDisplayModel->HideGroup(dwHAND);
					m_pDisplayModel->HideGroup(dwFOOT);
					m_pDisplayModel->HideGroup(dwPIFE);
				}

				if (dwBody != 0)
				{
					sprintf(str, "%04d", dwBody);
					m_pDisplayModel->ShowGroup(dwBODY, MAKEFOURCC(str[0],str[1],str[2],str[3]), lBodyLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwBODY);
		}
	}else
	{
		// ͷ�δ���
		DWORD					dwEquipIndex = GetEquipIndex(CPlayer::EQUIP_P_HEADGEAR);
		CGoodsList::tagGoods2 * pGoods2		 = CGoodsList::GetProperty(dwEquipIndex);
		if (pGoods2 != NULL)
		{
			dwHead	   = pGoods2->BaseProperty.dwEquipID;
			lHeadLevel = GetEquipLevel(CPlayer::EQUIP_P_HEADGEAR);

			if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_HEAR, 1) != 0)
				m_pDisplayModel->HideGroup(dwHAIR);
			if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_FACE, 1) != 0)
				m_pDisplayModel->HideGroup(dwFACE);
			if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_BODY, 1) != 0)
			{
				m_pDisplayModel->HideGroup(dwARMO);
				m_pDisplayModel->HideGroup(dwHAND);
				m_pDisplayModel->HideGroup(dwFOOT);
			}

			if (dwHead != 0)
			{
				/*sprintf(str, "%04d", dwHead);
				m_pDisplayModel->ShowGroup(dwHEAD, MAKEFOURCC(str[0],str[1],str[2],str[3]), lHeadLevel);*/
				m_HeadWear.SetVisible(true);
			}
		}else
			m_HeadWear.SetVisible(false);

		// ���״���
		if (GetGame()->GetRegion() == NULL ||
			(GetGame()->GetRegion() != NULL && GetGame()->GetRegion()->IsChangeEquip()))
		{
			if (GetShowFashion() & FASHION_TYPE_GREATCOAT)
			{
				dwEquipIndex = GetEquipIndex(CPlayer::EQUIP_P_FROCK);
				pGoods2		 = CGoodsList::GetProperty(dwEquipIndex);
				if (pGoods2 != NULL)
				{
					dwBody	   = pGoods2->BaseProperty.dwEquipID;
					lBodyLevel = GetEquipLevel(CPlayer::EQUIP_P_FROCK);

					if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_HEAR, 1) != 0)
						m_pDisplayModel->HideGroup(dwHAIR);
					if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_FACE, 1) != 0)
						m_pDisplayModel->HideGroup(dwFACE);
					if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_BODY, 1) != 0)
					{
						m_pDisplayModel->HideGroup(dwARMO);
						m_pDisplayModel->HideGroup(dwHAND);
						m_pDisplayModel->HideGroup(dwFOOT);
						m_pDisplayModel->HideGroup(dwPIFE);
					}

					if (dwBody != 0)
					{
						sprintf(str, "%04d", dwBody);
						m_pDisplayModel->ShowGroup(dwBODY, MAKEFOURCC(str[0],str[1],str[2],str[3]), lBodyLevel);
					}
				}else
					m_pDisplayModel->HideGroup(dwBODY);
			}else
				m_pDisplayModel->HideGroup(dwBODY);
		}else if (GetGame()->GetRegion() != NULL && !GetGame()->GetRegion()->IsChangeEquip())
		{
			dwEquipIndex = GetEquipIndex(CPlayer::EQUIP_P_FROCK);
			pGoods2		 = CGoodsList::GetProperty(dwEquipIndex);
			if (pGoods2 != NULL)
			{
				dwBody	   = pGoods2->BaseProperty.dwEquipID;
				lBodyLevel = GetEquipLevel(CPlayer::EQUIP_P_FROCK);

				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_HEAR, 1) != 0)
					m_pDisplayModel->HideGroup(dwHAIR);
				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_FACE, 1) != 0)
					m_pDisplayModel->HideGroup(dwFACE);
				if (CGoodsList::GetEffectVal(dwEquipIndex, GAP_HIDE_BODY, 1) != 0)
				{
					m_pDisplayModel->HideGroup(dwARMO);
					m_pDisplayModel->HideGroup(dwHAND);
					m_pDisplayModel->HideGroup(dwFOOT);
					m_pDisplayModel->HideGroup(dwPIFE);
				}

				if (dwBody != 0)
				{
					sprintf(str, "%04d", dwBody);
					m_pDisplayModel->ShowGroup(dwBODY, MAKEFOURCC(str[0],str[1],str[2],str[3]), lBodyLevel);
				}
			}else
				m_pDisplayModel->HideGroup(dwBODY);
		}
	}
}



/*
*	���ܣ�����ָ�����������װ����Ϣ
*	ϸ�������� 
*	������  pShape - ���ƵĲ��ն���
*	����ֵ�� ��
*	����: ����
*	��������: 08.8.28
*	�޸���־: 
*			���� - 09.11.10	�����ӿڷ���CShape�£�������д�ӿڣ����������ı��CShape
*/
void CPlayer::CopyAllEquip(CShape *pShape)
{
	CPlayer * pPlayer = dynamic_cast<CPlayer *>(pShape);
	if(pPlayer)
	{

		SetShowFashion(pPlayer->GetShowFashion());
		// ����װ��
		SetHairPic(pPlayer->GetHairPic());
		SetFacePic(pPlayer->GetFacePic());
		DWORD dwIndex = 0;
		/***********************************************************************/
		/*  zhaohang 2010-11-16 fix bylev ��byte��ΪDWORD ���
		pPlayer->GetEquipǰ��byteȫ��ȥ��*/
		/***********************************************************************/
		DWORD byLev = 0;

		bool bIsMainPlayer = (pPlayer == GetGame()->GetMainPlayer());
		//	0��ͷ��
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_HEAD))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_HEAD)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_HEAD)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_HEAD);
			byLev = pPlayer->GetEquipLevel(EQUIP_HEAD);
		}
		SetEquipIndex(CPlayer::EQUIP_HEAD, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_HEAD, byLev);

		//	1������
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_NECKLACE))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_NECKLACE)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_NECKLACE)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_NECKLACE);
			byLev = pPlayer->GetEquipLevel(EQUIP_NECKLACE);
		}
		SetEquipIndex(CPlayer::EQUIP_NECKLACE, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_NECKLACE, byLev);

		//	2�����
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_WING))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_WING)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_WING)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_WING);
			byLev = pPlayer->GetEquipLevel(EQUIP_WING);
		}
		if (dwIndex != GetEquipIndex(CPlayer::EQUIP_WING) || byLev != GetEquipLevel(EQUIP_WING))
		{
			RemoveWeaponShow(EQUIP_WING);
			SetEquipIndex(CPlayer::EQUIP_WING, dwIndex);
			SetEquipLevel(CPlayer::EQUIP_WING, byLev);
			UpdateWeaponShow(EQUIP_WING);
		}

        //	3������
        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_BODY))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_BODY);
            byLev = pPlayer->GetEquipLevel(EQUIP_BODY);
        }
        SetEquipIndex(CPlayer::EQUIP_BODY, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_BODY, byLev);
		

		//	4������
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_BACK))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_BACK)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_BACK)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_BACK);
			byLev = pPlayer->GetEquipLevel(EQUIP_BACK);
		}
		SetEquipIndex(CPlayer::EQUIP_BACK, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_BACK, byLev);

		//	5������	
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_HAND))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_HAND)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_HAND)->GetEffectVal(GAP_WEAPON_LEVEL,1);

			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_HAND);
			byLev = pPlayer->GetEquipLevel(EQUIP_HAND);
		}
		SetEquipIndex(CPlayer::EQUIP_HAND, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_HAND, byLev);

		//	6��Ь��
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_BOOT))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_BOOT)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_BOOT)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_BOOT);
			byLev = pPlayer->GetEquipLevel(EQUIP_BOOT);
		}
		SetEquipIndex(CPlayer::EQUIP_BOOT, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_BOOT, byLev);

		//	7��ͷ��
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_SHOULDER))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_SHOULDER)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_SHOULDER)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_SHOULDER);
			byLev = pPlayer->GetEquipLevel(EQUIP_SHOULDER);
		}
		if (dwIndex != GetEquipIndex(CPlayer::EQUIP_SHOULDER) || byLev != GetEquipLevel(EQUIP_SHOULDER))
		{
			RemoveWeaponShow(EQUIP_SHOULDER);
			SetEquipIndex(CPlayer::EQUIP_SHOULDER, dwIndex);
			SetEquipLevel(CPlayer::EQUIP_SHOULDER, byLev);
			UpdateWeaponShow(EQUIP_SHOULDER);
		}

        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_WRIST))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_WRIST)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_WRIST)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_WRIST);
            byLev = pPlayer->GetEquipLevel(EQUIP_WRIST);
        }
        SetEquipIndex(CPlayer::EQUIP_WRIST, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_WRIST, byLev);

        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_LEG))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_LEG)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_LEG)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_LEG);
            byLev = pPlayer->GetEquipLevel(EQUIP_LEG);
        }
        SetEquipIndex(CPlayer::EQUIP_LEG, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_LEG, byLev);

        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_BOOT))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_BOOT)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_BOOT)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_BOOT);
            byLev = pPlayer->GetEquipLevel(EQUIP_BOOT);
        }
        SetEquipIndex(CPlayer::EQUIP_BOOT, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_BOOT, byLev);

        //	9�����ָ
        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_LRING))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_LRING)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_LRING)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_LRING);
            byLev = pPlayer->GetEquipLevel(EQUIP_LRING);
        }
        SetEquipIndex(CPlayer::EQUIP_LRING, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_LRING, byLev);

        //	10���ҽ�ָ
        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_RRING))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_RRING)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_RRING)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_RRING);
            byLev = pPlayer->GetEquipLevel(EQUIP_RRING);
        }
        SetEquipIndex(CPlayer::EQUIP_RRING, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_RRING, byLev);

        //	15������
        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_WEAPON))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_WEAPON);
            byLev = pPlayer->GetEquipLevel(EQUIP_WEAPON);
        }
        if (dwIndex != GetEquipIndex(CPlayer::EQUIP_WEAPON) || byLev != GetEquipLevel(EQUIP_WEAPON))
        {
            RemoveWeaponShow(EQUIP_WEAPON);
            SetEquipIndex(CPlayer::EQUIP_WEAPON, dwIndex);
            SetEquipLevel(CPlayer::EQUIP_WEAPON, byLev);
            UpdateWeaponShow(EQUIP_WEAPON);
        }

        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_CARD))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_CARD)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_CARD)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_CARD);
            byLev = pPlayer->GetEquipLevel(EQUIP_CARD);
        }
        if (dwIndex != GetEquipIndex(CPlayer::EQUIP_CARD) || byLev != GetEquipLevel(EQUIP_CARD))
        {
            RemoveWeaponShow(EQUIP_CARD);
            SetEquipIndex(CPlayer::EQUIP_CARD, dwIndex);
            SetEquipLevel(CPlayer::EQUIP_CARD, byLev);
            UpdateWeaponShow(EQUIP_CARD);
        }

        //	17������
        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_GODGOODS))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_GODGOODS)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_GODGOODS)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_GODGOODS);
            byLev = pPlayer->GetEquipLevel(EQUIP_GODGOODS);
        }
        SetEquipIndex(CPlayer::EQUIP_GODGOODS, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_GODGOODS, byLev);


		//	8������
		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_EFFIGY))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_EFFIGY)->GetIndex();
				byLev = pPlayer->GetEquip(CPlayer::EQUIP_EFFIGY)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_EFFIGY);
			byLev = pPlayer->GetEquipLevel(EQUIP_EFFIGY);
		}
		SetEquipIndex(CPlayer::EQUIP_EFFIGY, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_EFFIGY, byLev);

        dwIndex = 0;
        byLev = 0;
        if ( bIsMainPlayer )
        {
            if(pPlayer->GetEquip(CPlayer::EQUIP_LIFECOWRY))
            {
                dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_LIFECOWRY)->GetIndex();
                byLev = pPlayer->GetEquip(CPlayer::EQUIP_LIFECOWRY)->GetEffectVal(GAP_WEAPON_LEVEL,1);
            }
        }
        else
        {
            dwIndex = pPlayer->GetEquipIndex(EQUIP_LIFECOWRY);
            byLev = pPlayer->GetEquipLevel(EQUIP_LIFECOWRY);
        }
        SetEquipIndex(CPlayer::EQUIP_LIFECOWRY, dwIndex);
        SetEquipLevel(CPlayer::EQUIP_LIFECOWRY, byLev);

		dwIndex = 0;
		byLev = 0;
		if ( bIsMainPlayer )
		{
			if(pPlayer->GetEquip(CPlayer::EQUIP_SOULGUARD))
			{
				dwIndex = pPlayer->GetEquip(CPlayer::EQUIP_SOULGUARD)->GetIndex();
				byLev = (BYTE)pPlayer->GetEquip(CPlayer::EQUIP_SOULGUARD)->GetEffectVal(GAP_WEAPON_LEVEL,1);
			}
		}
		else
		{
			dwIndex = pPlayer->GetEquipIndex(EQUIP_SOULGUARD);
			byLev = pPlayer->GetEquipLevel(EQUIP_SOULGUARD);
		}
		SetEquipIndex(CPlayer::EQUIP_SOULGUARD, dwIndex);
		SetEquipLevel(CPlayer::EQUIP_SOULGUARD, byLev);


		SetDisplayModelGroup(false);
	}
}

// �鿴ָ����ְҵ�Ƿ��������ҵ�ְҵ��֮��
bool CPlayer::IsInOccupedList(DWORD dwOccupID)
{
	for (int i = 0; i < (int)m_listOccuped.size(); ++i)
	{
		if(dwOccupID == m_listOccuped[i])
			return true;
	}
	return false;
}

// ָ���ļ����Ƿ��ܴ�ʹְҵ���ı仯
bool CPlayer::IsAddOccupedList(DWORD dwSkillID)
{
	bool bRt = false;
	DWORD dwOcc = OCC_Max;
	switch(dwSkillID)
	{
	case SKILL_BASE_NOVICE_OCC:				// ������
		dwOcc = OCC_Novice;
		bRt = true;
		break;
	case SKILL_BASE_FIGHTER_OCC:				// սʿ
		dwOcc = OCC_Fighter;
		bRt = true;
		break;
	case SKILL_BASE_RABBI_OCC:					// ��ʦ
		dwOcc = OCC_Mystic;
		bRt = true;
		break;
	case SKILL_BASE_FAQUIR_OCC:					// ����
		dwOcc = OCC_Rogue;
		bRt = true;
		break;
	case SKILL_BASE_KNIGHT_OCC:					// ��ʿ
		dwOcc = OCC_Knight;
		bRt = true;
		break;
	case SKILL_BASE_WARRIOR_OCC:				// ��ʿ
		dwOcc = OCC_Warrior;
		bRt = true;
		break;
	case SKILL_BASE_SCOUT_OCC:					// ���
		dwOcc = OCC_Scout;
		bRt = true;
		break;
	case SKILL_BASE_CRAFTSMAN_OCC:				// ����
		dwOcc = OCC_Craftsman;
		bRt = true;
		break;
	case SKILL_BASE_WARLOCK_OCC:				//��ʿ
		dwOcc = OCC_Warlock;
		bRt = true;
		break;
	case SKILL_BASE_CLERIC_OCC:					//��ʦ
		dwOcc = OCC_Cleric;
		bRt = true;
		break;
	case SKILL_BASE_PALADIN_OCC:				//ʥ��ʿ
		dwOcc = OCC_Paladin;
		bRt = true;
		break;
	case SKILL_BASE_DARKKINGHT_OCC:				//����ʿ
		dwOcc = OCC_DarkKnight;
		bRt = true;
		break;
	case SKILL_BASE_GLADIATOR_OCC:				//�Ƕ�ʿ
		dwOcc = OCC_Gladiator;
		bRt = true;
		break;
	case SKILL_BASE_BERSERKER_OCC:				//��սʿ
		dwOcc = OCC_Berserker;
		bRt = true;
		break;
	case SKILL_BASE_RANGER_OCC:					//Ѳ��
		dwOcc = OCC_Ranger;
		bRt = true;
		break;
	case SKILL_BASE_ASSASSIN_OCC:				//�̿�
		dwOcc = OCC_Assassin;
		bRt = true;
		break;
	case SKILL_BASE_BLADEDANCER_OCC:			//������
		dwOcc = OCC_BladeDancer;
		bRt = true;
		break;
	case SKILL_BASE_BARD_OCC:					//����ʫ��
		dwOcc = OCC_Bard;
		bRt = true;
		break;
	case SKILL_BASE_SORCERER_OCC:				//��ʦ
		dwOcc = OCC_Sorcerer;
		bRt = true;
		break;
	case SKILL_BASE_NECROMANCER_OCC:			//������ʦ
		dwOcc = OCC_Necromancer;
		bRt = true;
		break;
	case SKILL_BASE_BISHOP_OCC:					//����
		dwOcc = OCC_Bishop;
		bRt = true;
		break;
	case SKILL_BASE_PROPHET_OCC:				//����(��֪)
		dwOcc = OCC_Prophet;
		bRt = true;
		break;
	}
	// ְҵ�Ϸ�
	if(dwOcc >= OCC_Novice && dwOcc < OCC_Max)
		m_listOccuped.push_back(dwOcc);
	return bRt;
}

//���ݵ�ǰ�ĳƺ����ͷ��سƺ�
string CPlayer::GetPlayerTitle()
{
	if(GetTitleType()==ePTT_Faction)
	{
		if(GetFactionID() == NULL_GUID)
		{
			string strFacTitle("");
			return strFacTitle;
		}else{
			return GetFactionTitle();
		}
	}else{
		return m_pPlayerTitle;
	}
}

CPet* CPlayer::GetActivePet() const
{
    map<CGUID, CPet*>::const_iterator it = m_PetList.find(m_ActivePetGUID);
    if (it == m_PetList.end())
        return NULL;
    return it->second;
}

void CPlayer::SetCurOperPetGUID(const CGUID& guid)
{
	m_CurOperPetGUID = guid;
	CMessage msg(MSG_C2S_PET_SET_FOCUS);
	msg.Add(m_CurOperPetGUID);
	msg.Send();
}

/// ��������
CPet* CPlayer::CreatePet(const CGUID& guid)
{
	if( NULL_GUID == guid )
		return NULL;

	CPet* pet = new CPet;
	pet->SetHost(this);
	pet->SetExID(guid);
	m_PetList[guid] = pet;
	return pet;
}

/// ���������Ϣ
void CPlayer::DecodePetFromDataBlock(DBReadSet& setReadDB, bool bExData)
{
	if (bExData)
	{
		RemoveAllPet();
		long lCount = setReadDB.GetLongFromByteArray();	//������г��������
		if (lCount == 0)
			return ;

		for(int i=0;i<lCount;i++)
		{
			long lType = setReadDB.GetLongFromByteArray();
			if (lType == 0)
			{
				continue;
			}

			CGUID guid;
			setReadDB.GetBufferFromByteArray(guid);
			long lGrahpicsID = setReadDB.GetLongFromByteArray();
			char strName[32];
			setReadDB.GetStringFromByteArray(strName, 32);
			if (strcmp(strName, "") == 0)
				ASSERT(0);

			CPet* pet = CreatePet(guid);
			if (pet!=NULL)
			{
				pet->DecodeAttrFromDataBlock(setReadDB);

				if (PET_STATE_ACTIVE == pet->GetPetState())
				{
					SetActivePetGUID(pet->GetExID());
				}
			}
		}
	}
}

/// ȡ�ó�ս״̬������Ŀ
long CPlayer::GetFightPetNum()
{
	long lNum = 0;
	map<CGUID, CPet*>::iterator PetIter = m_PetList.begin();
	for (; PetIter != m_PetList.end(); ++PetIter)
	{
		CPet* pPet = PetIter->second;
		if (pPet->IsActive())
		{
			lNum++;
		}
	}
	return lNum;
}
/// ��ӳ���
void CPlayer::AddPet(CPet *pPet)
{
	if(pPet)
	{
		m_PetList[pPet->GetExID()] = pPet;
	}
	else
		ASSERT(0);
}

void CPlayer::SetPetFighting(CPet *pPet)
{
	if(!pPet)
	{
		ASSERT(0);
		return;
	}

	long lPetType = pPet->GetPetType();
	map<CGUID, CPet*>::iterator PetIter = m_PetList.begin();
	for (; PetIter!=m_PetList.end(); ++PetIter)
	{
		CPet* pOrigin = PetIter->second;
		if(pOrigin->GetPetType() == lPetType && pOrigin->GetExID() != pPet->GetExID() && pOrigin->IsActive())
		{
			pOrigin->SetPetIsActive(false);
			break;
		}
	}
	//���ó��Ｄ��
	pPet->SetPetIsActive(true);
}

void CPlayer::PetEnterRegion()
{
	if(m_PetList.empty())
		return;

	map<CGUID, CPet*>::iterator PetIter = m_PetList.begin();
	for (; PetIter!=m_PetList.end(); ++PetIter)
	{
		CPet* pet = PetIter->second;
		if (pet && pet->IsActive())
		{
			pet->EnterRegion(this == GetGame()->GetMainPlayer());
		}
	}
}
/// ���ݳ���GUIDȡ�ó���
CPet* CPlayer::GetPet(const CGUID& guid)
{
	CPet* pet = NULL;

	map<CGUID, CPet*>::iterator PetIter = m_PetList.find(guid);
	if (PetIter!=m_PetList.end())
	{
		pet = PetIter->second;
		if (guid == pet->GetExID())
		{
			return pet;
		}
	}
	return NULL;
}

/// ɾ������
void CPlayer::DelPet(const CGUID& guid)
{
	if (m_PetList.empty())
		return;

	map<CGUID, CPet*>::iterator PetIter = m_PetList.find(guid);
	if (PetIter!=m_PetList.end())
	{
		CPet* pPet = PetIter->second;
		if (guid == pPet->GetExID())
		{
			m_PetList.erase(PetIter);
			SAFE_DELETE(pPet);
		}
	}
}

/// չʾ����
void CPlayer::ShowPet(const CGUID& guid)
{
	m_ShowPets.insert(guid);
}
/// �ջس���
void CPlayer::UnShowPet(const CGUID& guid)
{
	m_ShowPets.erase(m_ShowPets.find(guid));
}

void CPlayer::RemoveAllPet()
{
	//�ͷų���
	map<CGUID, CPet*>::iterator PetIter = m_PetList.begin();
	for (; PetIter!=m_PetList.end(); ++PetIter)
	{
        CPet *pPet = PetIter->second;
        if(pPet->IsActive())    //����ĳ����ѽ�������������Ͳ��ͷ���
            continue;
		SAFE_DELETE(pPet);
	}
	m_PetList.clear();
	//MessageBox(g_hWnd,"�����еĳ��ﱻ�����","Info",MB_OK);
}

void CPlayer::ValidatePetInterface()
{

}

void CPlayer::SavePetSkillSetting()
{
	if (this == GetGame()->GetMainPlayer())
	{
		struct tagSkillAlter 
		{
			long lpos;
			long lActPos;
			bool bAuto;
		};

		long lPetActivedSize = GetFightPetNum();
		if (lPetActivedSize == 0)
			return;
		tagSkillAlter	petSkill;
		CMessage	msg(MSG_C2S_SKILL_SAVE);
		msg.Add(lPetActivedSize);						//������������ĳ�����Ŀ

		map<CGUID,CPet*>::iterator PetIter = m_PetList.begin();
		for (; PetIter!=m_PetList.end(); ++PetIter)
		{
			CPet* pPet = PetIter->second;
			//������γ� ������
			//if (pPet->GetPetType() == PET_TYPE_TRAP || pPet->GetPetType() == PET_TYPE_ORNA)
			//	continue;
			if (pPet->IsPetSkillChanged())
			{
				msg.Add((long)pPet->GetPetType());		//��������
				msg.Add(pPet->GetExID());				//����guid
				long lSkillCount = pPet->GetPetSkillCount();

				vector<tagSkillAlter>	vecPetSkillAlter;
				for (long lPos = 0;lPos < lSkillCount;++lPos)
				{
					tagPetSkillInfo *pSkill = pPet->GetPetSkillInfoByPos(lPos);

					if(pSkill && pSkill->bSkillIsChanged)
					{
						petSkill.lpos	 = lPos;				
						petSkill.lActPos = pSkill->lActPos;		
						petSkill.bAuto	 = pSkill->bAuto;		
						vecPetSkillAlter.push_back(petSkill);
					}
				}
				long lChangeSize = (long)vecPetSkillAlter.size();		//���ı似�ܵĸ���
				msg.Add(lChangeSize);
				for (long i = 0; i<lChangeSize; ++i)
				{
					msg.Add(vecPetSkillAlter[i].lpos);			//����λ��
					msg.Add(vecPetSkillAlter[i].lActPos);		//���ܶ�����λ��
					msg.Add((BYTE)vecPetSkillAlter[i].bAuto);	//�Ƿ��Զ��ͷ�
				}
			}
			else
			{
				msg.Add((long)0);		//��������
				continue;
			}
		}
		msg.Send();
	}
}


/*
* ����: ��Ӿ�����ʾ�ı�
* ժҪ: -
* ����: eType	 - ��ʾ�ı�����
*		 lNumber - ��Ҫ��ʾ�ľ�����ֵ
* ����ֵ: -
* ����: lpf
* ��������: 2009.02.05
* �޸���־:
*	2008.02.24 - lpf
*		�ں�����ʼλ�������˶Բ㼶���޵Ĵ����ж�
*	2009.04.08 - lpf
*		��������ʾ�ı����Ͳ���
*/
void CPlayer::AddTipText(TIPTEXT_TYPE eType, long lNumber)
{
	if (lNumber <= 0 || m_dwLevelMax == 0)
		return;

	char szTmp[50];
	sprintf(szTmp, "%d", lNumber);

	DWORD dwLevel = 0;
	while (dwLevel < m_dwLevelMax - 1)
	{
		if (m_vecTipText[dwLevel].fAlpha <= 0.0f)
			break;

		dwLevel++;
	}

	tagTipText stTipText;
	stTipText.eType = eType;
	stTipText.sNumber = szTmp;
	stTipText.fAlpha = 1.0f;					//͸����
	stTipText.dwStartTime = timeGetTime();		//��ʼʱ��
	stTipText.vPos = D3DXVECTOR3(GetPosY(), GetPickBoxHeight() + 0.5f + GetHeight() + (float)dwLevel * CGlobeSetup::GetSetup()->fExpTipTextLineSpace, GetPosX());

	m_vecTipText[dwLevel] = stTipText;
}



/*
* ����: ��ʾ��ҵľ�����ʾ�ı�
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2009.02.05
* �޸���־:
*	2009.04.08 - lpf
*		��������ʾ�ı����ʹ���
*/
void CPlayer::DisplayTipText()
{
	if (m_dwLevelCurrentDisplayNum == 0)
		return;

	DWORD dwColor = 0;
	DWORD dwColorBack;
	DWORD dwColorAlpha;
	DWORD dwDrawX = 0, dwDrawY = 0;
	float fNumberScale = CGlobeSetup::GetSetup()->fExpTipTextNumberScale;
	D3DXMATRIX mFinal;
	D3DXVECTOR3 vScreenPos;
	GetGame()->GetRegion()->CalculateFinalTransformMatrix(mFinal);

	const char * pszNumber = NULL;
	DWORD		 dwNumberLen = 0;
	CBitmapX   * pBitmap = NULL;

	for (DWORD dw = 0; dw < (DWORD)m_vecTipText.size(); ++dw)
	{
		if (m_vecTipText[dw].fAlpha <= 0.0f)
			continue;

		D3DXVec3TransformCoord(&vScreenPos, &m_vecTipText[dw].vPos, &mFinal);
		dwColorAlpha = (DWORD)(m_vecTipText[dw].fAlpha * 255.0f) << 24;

		// �ж���ʾ�ı�����
		if (m_vecTipText[dw].eType == TT_EXP)
		{
			dwColor		= dwColorAlpha | CGlobeSetup::GetSetup()->colorExpTipText;
			dwColorBack = dwColorAlpha | CGlobeSetup::GetSetup()->colorExpTipTextBack;
			pBitmap		= GetGame()->GetExpTip();
		}else
		{
			dwColor		= dwColorAlpha | CGlobeSetup::GetSetup()->colorConTipText;
			dwColorBack = dwColorAlpha | CGlobeSetup::GetSetup()->colorConTipTextBack;
			pBitmap		= GetGame()->GetConTip();
		}

		// ����"����ֵ"ͼƬ
		dwDrawX = (DWORD)vScreenPos.x - pBitmap->GetWidth();
		dwDrawY = (DWORD)vScreenPos.y - pBitmap->GetHeight() / 2;
		pBitmap->Draw(dwDrawX - 1, dwDrawY, dwColorBack);
		pBitmap->Draw(dwDrawX, dwDrawY - 1, dwColorBack);
		pBitmap->Draw(dwDrawX + 1, dwDrawY, dwColorBack);
		pBitmap->Draw(dwDrawX, dwDrawY + 1, dwColorBack);
		pBitmap->Draw(dwDrawX, dwDrawY, dwColor);

		// ��������
		dwDrawX = (DWORD)vScreenPos.x;
		dwDrawY = (DWORD)vScreenPos.y - (DWORD)(((float)GetGame()->GetNumberHeight() * fNumberScale) * 0.5f);
		pszNumber = m_vecTipText[dw].sNumber.c_str();
		dwNumberLen = (DWORD)m_vecTipText[dw].sNumber.length();
		GetGame()->NumberOut(dwDrawX - 1, dwDrawY, pszNumber, dwNumberLen, dwColorBack, fNumberScale);
		GetGame()->NumberOut(dwDrawX, dwDrawY - 1, pszNumber, dwNumberLen, dwColorBack, fNumberScale);
		GetGame()->NumberOut(dwDrawX + 1, dwDrawY, pszNumber, dwNumberLen, dwColorBack, fNumberScale);
		GetGame()->NumberOut(dwDrawX, dwDrawY + 1, pszNumber, dwNumberLen, dwColorBack, fNumberScale);
		GetGame()->NumberOut(dwDrawX, dwDrawY, pszNumber, dwNumberLen, dwColor, fNumberScale);
	}
}

//������ɫ��ѫ���б�
void CPlayer::DecodeMedalFromDataBlockAndInit(DBReadSet& setReadDB)
{
	//���ԭ�е�����
	if(m_VecMedalOnPlayer.size()>0)
	{
		m_VecMedalOnPlayer.clear();
	}
	//m_iDisplayMode = 0;
	LONG lGoodsNum = setReadDB.GetLongFromByteArray();
	for(int i=0;i<lGoodsNum;i++)
	{
		DWORD MedalIndex = setReadDB.GetLongFromByteArray();
		m_VecMedalOnPlayer.push_back(MedalIndex);
	}
	return;
}

//������������һ��ѫ��
void CPlayer::AddMedal(DWORD Index)
{
	m_VecMedalOnPlayer.push_back(Index);
}
//���������ɾ��һ��ѫ��
void CPlayer::DeleteMedal(DWORD Index)
{
	if(m_VecMedalOnPlayer.size()>0&&FindMedalOnPlayer(Index))   //�жϵ�ǰѫ���б�Ϊ0�������и�ѫ��
	{
		int i=0;
		vector<DWORD>  m_VecMedalOnPlayerTemp; //����һ����ʱ�������ڴ洢�仯�������
		for(vector<DWORD>::iterator it=m_VecMedalOnPlayer.begin();it!=m_VecMedalOnPlayer.end();it++)
		{
			if(m_VecMedalOnPlayer[i]!=Index)
			{
				m_VecMedalOnPlayerTemp.push_back(m_VecMedalOnPlayer[i]);
			}
			i++;
		}
		m_VecMedalOnPlayer.clear();    //���ԭ����ѫ������
		int j=0;
		for(vector<DWORD>::iterator it=m_VecMedalOnPlayerTemp.begin();it!=m_VecMedalOnPlayerTemp.end();it++)
		{
			m_VecMedalOnPlayer.push_back(m_VecMedalOnPlayerTemp[j]);
			j++;
		}
	}
}

//��������Ƿ��иñ�ŵ���Ʒ
bool CPlayer::FindMedalOnPlayer(DWORD Index)
{
	int i=0;
	for(vector<DWORD>::iterator it=m_VecMedalOnPlayer.begin();it!=m_VecMedalOnPlayer.end();it++)
	{
		if(m_VecMedalOnPlayer[i]==Index)
		{
			return true;
		}
		i++;
	}
	return false;
}


WORD CPlayer::GetOccuLvlCoe(eOccupation byOccu,WORD wOccuLevel)
{
	WORD wRet = 0;
	switch(byOccu)
	{
		//������
	case OCC_Novice:
		wRet = 0;
		break;
		//սʿ����ʦ������
	case OCC_Fighter:
	case OCC_Mystic:
	case OCC_Rogue:
		wRet = 9;
		break;
		//��ʿ����ʿ�������ˡ���ʿ����ʦ
	case OCC_Knight:
	case OCC_Warrior:
	case OCC_Scout:
	case OCC_Craftsman:
	case OCC_Warlock:
	case OCC_Cleric:
		wRet = 39;
		break;
		//ʥ��ʿ������ʿ����ʿ����սʿ��Ѳ�ء��̿͡������ߡ�����ʫ�ˡ���ʦ�����鷨ʦ�����̡�����
	case OCC_Paladin:
	case OCC_DarkKnight:
	case OCC_Gladiator:
	case OCC_Berserker:
	case OCC_Ranger:
	case OCC_Assassin:
	case OCC_BladeDancer:
	case OCC_Bard:
	case OCC_Sorcerer:
	case OCC_Necromancer:
	case OCC_Bishop:
	case OCC_Prophet:
		wRet = 79;
		break;
	};
	return wRet;
}

/// ��������Χָ����Χ���Ƿ��б������ڿ���
bool CPlayer::CheckIsAnyBodyShopOpenedOnRange(DWORD dwRangeSize)
{
	/// �������飨���ڻ����ĸ����ޣ������������ķ�Χ��
	int iStepArray[4][2] = {1,1,1,-1,-1,1,-1,-1};

	for(int iStepNum = 0;iStepNum<4/*sizeof(iStepArray)*/;iStepNum++)
	{
		for (int iIncreaseX = 0;abs(iIncreaseX) <= (int)dwRangeSize; iIncreaseX+=iStepArray[iStepNum][0])
		{
			for (int iIncreaseY = 0;abs(iIncreaseY)<= (int)dwRangeSize;iIncreaseY+=iStepArray[iStepNum][1])
			{
				list<CShape *> lsFindShape;
				((CClientRegion*)GetFather())->GetAllShapeByType(lsFindShape,GetTileX()+iIncreaseX,GetTileY()+iIncreaseY,TYPE_PLAYER);
				if(!lsFindShape.empty())
				{
					list<CShape *>::iterator ShapeIter = lsFindShape.begin();
					for(;ShapeIter!=lsFindShape.end();ShapeIter++)
					{
						if ((*ShapeIter)->GetType()==TYPE_PLAYER&&((CPlayer*)(*ShapeIter))->IsPlayerShopOpened())
						{      
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
