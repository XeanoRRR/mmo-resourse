#include "StdAfx.h"
#include "Player.h"
#include "SkillMgr.h"
#include "MailMgr.h"
#include "PetMgr.h"
#include "DepotMgr.h"
#include "EquipMgr.h"
#include "PacketMgr.h"
#include "GoodsMgr.h"
#include "MedalMgr.h"
#include "OrganiseSys.h"
#include "Sprite.h"
#include "Task.h"
#include "Collect.h"
#include "TeamMgr.h"
#include "PlayerShop.h"
#include "FamilyMgr.h"
#include "LinkMan.h"
#include "TradeMgr.h"

#include "Goods.h"
#include "Monster.h"
#include "ClientRegion.h"
#include ".\player.h"

#include "..\Utility\FindPath.h"
//#include "..\Net\Message.h"
#include "..\Script\ScriptSys.h"
#include "..\GameTest\GameTestDlg.h"
//#include "..\App\PetSystem\CPet.h"
//#include "..\App\PetSystem\CFairyPet.h"
//#include "..\App\PetSystem\CFighterPet.h"
//#include "..\App\PetSystem\CSummonPet.h"
#include "../Utility/gtDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef list<tagGoods>::iterator tagGoodsIter;

CPlayer::CPlayer(void)
{
	m_pSkillMgr = new CSkillMgr;
	m_pMailMgr = new CMailMgr;
	m_pPetMgr = new CPetMgr;
	m_pGoodsMgr = new CGoodsMgr(this);
	m_pTeamMgr = new CTeamMgr;
	m_pShop = new CPlayerShop;
	m_pCollect = new CPlayerCollect;
	m_pFamily = new CFamily;
	m_pLinkMan = new CLinkMan;
	m_pMedalMgr = new CMedalMgr;
	m_pOrganiseSys = new COrganiseSys;
	m_pSpriteMgr = new CSprite;
	m_pTaskMgr = new CTask;
	m_pTradeMgr = new CTradeMgr;
	m_bSelf = false;

	memset(&m_Property,0,sizeof(m_Property));
	memset(&m_FightProperty,0,sizeof(m_FightProperty));

	int nSize = sizeof(tagPlayerElementProperty);;
	memset(&m_arrElementProperty,0,nSize*MT_Max);

	m_PlayerMoneyGuid = CGUID::GUID_INVALID;			//���
	m_PlayerBaoShiGuid = CGUID::GUID_INVALID;			//��ʯ
	m_PlayerYuanBaoGuid = CGUID::GUID_INVALID;		//Ԫ��
	m_PlayerJiFenGuid = CGUID::GUID_INVALID;			//����
	m_PlayerSilverMoneyGuid = CGUID::GUID_INVALID;	//����

	m_dwPlayerMoneyNum = 0;				//	���
	m_dwPlayerBaoShiNum = 0;			//��ʯ
	m_dwPlayerYuanBaoNum = 0;			//Ԫ��
	m_dwPlayerJiFenNum = 0;				//����
	m_dwPlayerSilverMoneyNum = 0;		//	����
}	

CPlayer::~CPlayer(void)
{
	Release();

	SAFE_DELETE(m_pSkillMgr);
	SAFE_DELETE(m_pMailMgr);
	SAFE_DELETE(m_pPetMgr);
	SAFE_DELETE(m_pGoodsMgr);
	SAFE_DELETE(m_pTeamMgr);
	SAFE_DELETE(m_pShop);
	SAFE_DELETE(m_pCollect);
	SAFE_DELETE(m_pFamily);
	SAFE_DELETE(m_pLinkMan);
	SAFE_DELETE(m_pMedalMgr);
	SAFE_DELETE(m_pOrganiseSys);
	SAFE_DELETE(m_pSpriteMgr);
	SAFE_DELETE(m_pTaskMgr);
	SAFE_DELETE(m_pTradeMgr);
}

void CPlayer::Release()
{
	m_pSkillMgr->Release();
	m_pMailMgr->Release();
	//m_pPetMgr->Release();
	m_pGoodsMgr->Release();
	m_pTeamMgr->Release();
	m_pShop->Release();
	m_pCollect->Release();
	m_pFamily->Release();
	m_pLinkMan->Release();
	m_pMedalMgr->Release();
	m_pOrganiseSys->Release();
	m_pSpriteMgr->Release();
	m_pTaskMgr->Release();
}

void CPlayer::AddBounsInfo(tagBounsInfo* info)
{
	tagGtBounsInfo * gtInfo = new tagGtBounsInfo;
	gtInfo->bQuestGet = false;
	gtInfo->pInfo = info;
	m_listGtBounsInfo.push_back(gtInfo);
}



// long CPlayer::GetWeapPosFromPacket(const char* strOrgName)
// {
// 	list<tagGoods> listGoods = m_pPackMgr->GetMainGoodsList();
// 
// 	list<tagGoods>::iterator it = listGoods.begin();
// 	for( ;it != listGoods.end(); ++it)
// 	{
// 		if( !strcmp( it->pGoods->GetOrginName(),strOrgName))
// 			return it->lPos;
// 	}
// 	return -1;
// }



// CGoods* CPlayer::GetGoodsByGUID(const CGUID& guID,int *pPos,long* lExtendID)   //??? Ϊʲô�������� 
// {
// 	CGoods *pGoods=NULL;
// 	//��װ���в���
// 	pGoods = m_pEquipMgr->GetGoodsByGUID(guID,pPos,lExtendID);
// 	if( pGoods )
// 		return pGoods;
// 	//�ڱ����в���
// 	pGoods = m_pPackMgr->GetGoodsByGUID(guID,pPos,lExtendID);
// 	//	�ֿ�
// 	pGoods = m_pDepotMgr->GetGoodsByGuid(guID,pPos,lExtendID);
// 	if( pGoods != NULL )
// 		return pGoods;
// 	return NULL;
// }


// void CPlayer::MountEquip(CGoods *pGood,bool bPositive)
// {
// }

// long CPlayer::CanMountEquip(CGoods *pGoods)
// {
// 	//װ�����������ж�
// 	DWORD dwVal=0;
// 	//�ȼ�����
// 	dwVal=pGoods->GetEffectVal(6,1);
// 	if(dwVal!=0 && dwVal>GetLevel())
// 		return LEVEL_NOTFIT;
// 
// 	//��������
// 	dwVal=pGoods->GetEffectVal(7,1);
// 	if(dwVal!=0 && dwVal>GetStrenth())
// 		return STR_NOTFIT;
// 
// 	//��������
// 	dwVal=pGoods->GetEffectVal(8,1);
// 	if(dwVal!=0 && dwVal>GetAgility())
// 		return DEX_NOTFIT;
// 
// 	//��������
// 	dwVal=pGoods->GetEffectVal(9,1);
// 	if(dwVal!=0 && dwVal>GetConsti())
// 		return CON_NOTFIT;
// 
// 	//ְҵ����
// 	dwVal=CGoodsList::GetEffectVal(pGoods->GetIndex(),11,1);//pGoods->GetEffectVal(11,1);
// 	if(dwVal!=0 && dwVal!=GetOccupation()+1)
// 		return OCCUPATION_NOTFIT;
// 
// 	//�Ա�����
// 	//dwVal=pGoods->GetEffectVal(12,1);
// 	//if(dwVal!=0 && dwVal!=GetSex()+1)
// 	//	return SEX_NOTFIT;
// 	return EQUIPFIT;
// }

//����һ��ʹ�������Ե��ߵ���Ϣ��gameserver
// void CPlayer::UseItem(DWORD dwIndex,WORD nExtendId,int iPos)
// {
// }

// long CPlayer::CanUseItem(DWORD dwIndex)
// {
// 	DWORD dwVal=0;
// 	//�ȼ����Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,6,1);	
// 	if(dwVal!=0 && dwVal>GetLevel())
// 		return LEVEL_NOTFIT;
// 
// 	//�������Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,7,1);
// 	if(dwVal!=0 && dwVal>GetStrenth())
// 		return STR_NOTFIT;
// 
// 	//�������Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,8,1);
// 	if(dwVal!=0 && dwVal>GetAgility())
// 		return DEX_NOTFIT;
// 
// 	//�������Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,9,1);
// 	if(dwVal!=0 && dwVal>GetConsti())
// 		return CON_NOTFIT;
// 
// 	//ְҵ���Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,11,1);
// 	if(dwVal!=0 && dwVal!=GetOccupation()+1)
// 		return OCCUPATION_NOTFIT;
// 
// 	//�Ա����Ƽ��
// 	dwVal=CGoodsList::GetEffectVal(dwIndex,12,1);
// 	if(dwVal!=0 && dwVal!=GetSex())
// 		return SEX_NOTFIT;
// 
// 	return EQUIPFIT;
// }
// 

// bool CPlayer::SetProperty(const char* strName,long value)
// {
// 	if(NULL == strName)	return false;
// 	//	map<string,tagPty>::iterator itr = m_mapNameValue.find(strName);
// 	//	if(itr == m_mapNameValue.end())	return false;
// 
// 	//switch (itr->second.lType)
// 	//{
// 	//case 0:	// byte
// 	//	*(itr->second.plValue) = (BYTE)value;
// 	//	break;
// 	//case 1:	// word
// 	//	*(itr->second.pwValue) = (WORD)value;
// 	//	break;
// 	//case 2:	// dword
// 	//	*(itr->second.pdwValue) = (DWORD)value;
// 	//	break;
// 	//case 3:	// float
// 	//	*(itr->second.pfValue) = (float)value;
// 	//	break;
// 	//case 4:	// long
// 	//	*(itr->second.pValue) = (long)value;
// 	//	break;
// 	//case 5:	// short
// 	//	*(itr->second.psValue) = (short)value;
// 	//	break;
// 	//case 6:	// bool
// 	//	*(itr->second.pbValue) = value==0?false:true;
// 	//	break;
// 	//}
// 
// 	//OnPropertyChanged(strName);
// 	return true;
// }
// bool CPlayer::OnPropertyChanged(const char* attrName)
// {
// 	if( attrName == NULL ) return false;
// 
// 	return CMoveShape::OnPropertyChanged(attrName);
// 	//return true;
// }
// const char* CPlayer::GetPropertyStringValue(const char* pszPropertyName)
// {
// 	return CMoveShape::GetPropertyStringValue(pszPropertyName);
// }

void CPlayer::UpdateProperty(CMessage* pMsg)
{
	DBReadSet db_r;
	pMsg->GetDBReadSet(db_r);
	DecordChangeProperyFromDataBlock(db_r);
}

void CPlayer::DecordChangeProperyFromDataBlock(DBReadSet& setReadDB)
{
	CMoveShape::DecordChangePropertyFromDataBlock(setReadDB);
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
			data = setReadDB.GetFloatFromByteArray();
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
		// 		itProETN it = m_PropertyEnumToName.find(pro_type);
		// 		if(it != m_PropertyEnumToName.end())
		// 		{
		// 			SetProperty( (*it).second.c_str(),data);
		// 		}

		SAFE_DELETE_ARRAY(byte_Array);
		SAFE_DELETE_ARRAY(word_Array);
		SAFE_DELETE_ARRAY(dword_Array);
		SAFE_DELETE_ARRAY(flaot_Array);
		SAFE_DELETE_ARRAY(long_Array);
		SAFE_DELETE_ARRAY(short_Array);
		SAFE_DELETE_ARRAY(char_Array);
		SAFE_DELETE_ARRAY(bool_Array);
	}	
}

//////////////////////////////////////////////////////////////////////////
//	�������͵�ַ��ӳ���
//////////////////////////////////////////////////////////////////////////

// ��ʼ����������ֵ��Ӱ��
// void CPlayer::InitNameValueMap()
// {
// 	CMoveShape::InitNameValueMap();
// 	//tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
// 
// 	//st.lType=6; st.pbValue=&m_bContendState;			m_PropertyEnumToName[P_ATTR_CONTENDSTATE]="ContendState";					m_mapNameValue["ContendState"]=st;			
// 	//st.lType=6; st.pbValue=&m_lCityWarDiedState;		m_PropertyEnumToName[P_ATTR_CITYWARDIEDSTATE]="CityWarDiedState"; 			m_mapNameValue["CityWarDiedState"]=st;		
// 
// 
// 	//st.lType=4; st.pValue=&m_lType;;			m_PropertyEnumToName[P_ATTR_TYPE]="Type";				m_mapNameValue["Type"]=st;			
// 	//st.lType=4; st.pValue=0/*&m_lGraphicsID;*/;		m_PropertyEnumToName[P_ATTR_GRAPHICSID]="GraphicsID";	m_mapNameValue["GraphicsID"]=st;	
// 	//st.lType=4; st.pValue=&m_lRegionID;			m_PropertyEnumToName[P_ATTR_REGIONID]="RegionID";		m_mapNameValue["RegionID"]=st;
// 	//st.lType=3;	st.pfValue=&m_fPosX;			m_PropertyEnumToName[P_ATTR_TILEX]="TileX";				m_mapNameValue["TileX"]=st;		
// 	//st.lType=3;	st.pfValue=&m_fPosY;			m_PropertyEnumToName[P_ATTR_TILEY]="TileY";				m_mapNameValue["TileY"]=st;
// 	//st.lType=3;	st.pfValue=&m_fDir;				m_PropertyEnumToName[P_ATTR_DIR]="Dir";					m_mapNameValue["Dir"]=st;					
// 	//st.lType=1;	st.pwValue=&m_wState;			m_PropertyEnumToName[P_ATTR_STATE]="State";				m_mapNameValue["State"]=st;			
// 	//st.lType=1;	st.pwValue=&m_wAction;			m_PropertyEnumToName[P_ATTR_ACTION]="Action";			m_mapNameValue["Action"]=st;		
// 
// 
// 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	///*��������*/
// 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// 	//st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byOccu);			m_PropertyEnumToName[P_ATTR_OCCU]="Occu";					m_mapNameValue["Occu"]=st;		
// 	//st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byAssOccu);		m_PropertyEnumToName[P_ATTR_DOCCU]="DOccu";					m_mapNameValue["DOccu"]=st;
// 	//st.lType=0;	st.plValue  = (BYTE*)(&m_Property.byConst);			m_PropertyEnumToName[P_ATTR_CONST]="Const";					m_mapNameValue["Const"]=st;
// 	//st.lType=0;	st.plValue	= &m_Property.bySex;					m_PropertyEnumToName[P_ATTR_SEX]="Sex";						m_mapNameValue["Sex"]=st;
// 	//st.lType=0;	st.plValue	= &m_Property.byClass;					m_PropertyEnumToName[P_ATTR_CLASS]="Class";					m_mapNameValue["Class"]=st;
// 	//st.lType=0;	st.plValue	= &m_Property.byHairPic;				m_PropertyEnumToName[P_ATTR_HEADPIC]="HeadPic";				m_mapNameValue["HeadPic"]=st;				
// 	//st.lType=0;	st.plValue	= &m_Property.byFacePic;;				m_PropertyEnumToName[P_ATTR_FACEPIC]="FacePic";				m_mapNameValue["FacePic"]=st;				
// 	//st.lType=1;	st.pdwValue	= &m_Property.dwHp;						m_PropertyEnumToName[P_ATTR_HP]="Hp";						m_mapNameValue["Hp"]=st;				
// 	//st.lType=1;	st.pwValue	= &m_Property.wMp;;						m_PropertyEnumToName[P_ATTR_MP]="Mp";						m_mapNameValue["Mp"]=st;				
// 	//st.lType=1;	st.pwValue  = &m_Property.wEnergy;					m_PropertyEnumToName[P_ATTR_ENERGY]="Energy";				m_mapNameValue["Energy"]=st;			
// 	//st.lType=1; st.pwValue  = &m_Property.wStamina;					m_PropertyEnumToName[P_ATTR_STAMINA]="Stamina";	;			m_mapNameValue["Stamina"]=st;				
// 	//st.lType=0;	st.pwValue	= &m_Property.wLevel;					m_PropertyEnumToName[P_ATTR_LEVEL]="Level";					m_mapNameValue["Level"]=st;				
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwExp;					m_PropertyEnumToName[P_ATTR_EXP]="Exp";						m_mapNameValue["Exp"]=st;
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwPresentExp;				m_PropertyEnumToName[P_ATTR_PRESENTEXP]="PresentExp";		m_mapNameValue["PresentExp"]=st;
// 	//st.lType=0; st.plValue	= &m_Property.byRankOfNobility;			m_PropertyEnumToName[P_ATTR_RANKOFNOBILITY]="RankOfNobility";m_mapNameValue["RankOfNobility"]=st;	
// 	//st.lType=2;	st.pdwValue = &m_Property.dwRankOfNobCredit;		m_PropertyEnumToName[P_ATTR_RANKOFNOBCREDIT]="RankOfNobCredit";	m_mapNameValue["RankOfNobCredit"]=st;		
// 	//st.lType=0; st.plValue	= &m_Property.byRankOfMercenary;		m_PropertyEnumToName[P_ATTR_RANKOFMERCENARY]="RankOfMercenary";	m_mapNameValue["RankOfMercenary"]=st;		
// 	//st.lType=2;	st.pdwValue = &m_Property.dwMercenaryCredit;		m_PropertyEnumToName[P_ATTR_MERCENARYCREDIT]="MercenaryCredit";	m_mapNameValue["MercenaryCredit"]=st;		
// 	//st.lType=2;	st.pdwValue = &m_Property.dwBatakCredit;			m_PropertyEnumToName[P_ATTR_BATAKCREDIT]="BatakCredit";		m_mapNameValue["BatakCredit"]=st;
// 	//st.lType=2;	st.pdwValue = &m_Property.dwMedalScores;			m_PropertyEnumToName[P_ATTR_MEDALSCORES]="MedalScores";		m_mapNameValue["MedalScores"]=st;
// 	//st.lType=0;	st.plValue  = &m_Property.byCountry;				m_PropertyEnumToName[P_ATTR_COUNTRY]="Country";					m_mapNameValue["Country"]=st;				
// 	//st.lType=2;	st.pdwValue = &m_Property.dwCountryContribute;		m_PropertyEnumToName[P_ATTR_COUNTRYCONTRIBUTE]="CountryContribute";	m_mapNameValue["CountryContribute"]=st;	
// 	//st.lType=2;	st.pdwValue = &m_Property.dwSpouseID;				m_PropertyEnumToName[P_ATTR_SPOUSEID]="SpouseId";			m_mapNameValue["SpouseId"]=st;			
// 	//st.lType=2;	st.pdwValue = &m_Property.dwSpouseParam;			m_PropertyEnumToName[P_ATTR_SPOUSEPARAM]="SpouseParam";		m_mapNameValue["SpouseParam;"]=st;		
// 	//st.lType=0; st.plValue  = &m_Property.byBusinessLevel;			m_PropertyEnumToName[P_ATTR_BUSINESSLEVEL]="BusinessLevel";	m_mapNameValue["BusinessLevel"]=st;		
// 	//st.lType=2; st.pdwValue = &m_Property.dwBusinessExp;			m_PropertyEnumToName[P_ATTR_BUSINESSEXP]="BusinessExp";		m_mapNameValue["BusinessExp"]=st;
// 	//st.lType=2; st.pdwValue = &m_Property.dwArtisanCredit;			m_PropertyEnumToName[P_ATTR_ARTISANCREDIT]="ArtisanCredit";	m_mapNameValue["ArtisanCredit"]=st;
// 	//st.lType=0; st.plValue  = &m_Property.byArtisanLevel;			m_PropertyEnumToName[P_ATTR_ARTISANLEVEL]="ArtisanLevel";	m_mapNameValue["ArtisanLevel"]=st;		
// 	//st.lType=2; st.pdwValue = &m_Property.dwArtisanExp;				m_PropertyEnumToName[P_ATTR_ARTISANEXP]="ArtisanExp";	;	m_mapNameValue["ArtisanExp"]=st;		
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwPkValue;				m_PropertyEnumToName[P_ATTR_PKVALUE]="PkValue";				m_mapNameValue["PkValue"]=st;				
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwPkCount;				m_PropertyEnumToName[P_ATTR_PKCOUNT]="PkCount";				m_mapNameValue["PkCount"]=st;			
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwPVPCount;				m_PropertyEnumToName[P_ATTR_PVPCOUNT]="PVPCount";			m_mapNameValue["PVPCount"]=st;			
// 	//st.lType=2;	st.pdwValue	= &m_Property.dwPVPValue;;				m_PropertyEnumToName[P_ATTR_PVPVALUE]="PVPValue";			m_mapNameValue["PVPValue"]=st;			
// 	//st.lType=0;	st.plValue  = &m_Property.byPKOnOff;  				m_PropertyEnumToName[P_ATTR_PKONOFF]="PkOnOff";				m_mapNameValue["PkOnOff"]=st;			
// 	//st.lType=2; st.pdwValue = &m_Property.dwUpgradeExp;				m_PropertyEnumToName[P_ATTR_UPGRADEEXP]="UpgradeExp";		m_mapNameValue["UpgradeExp"]=st;
// 	//st.lType=6; st.pbValue  = &m_Property.bShowCountry;				m_PropertyEnumToName[P_ATTR_SHOWCOUNTRY]="ShowCountry";		m_mapNameValue["ShowCountry"]=st;
// 	//char tmp[1024];
// 	//for(int i=0; i<24; ++i)
// 	//{
// 	//	sprintf(tmp, "dwHotKey%d", i);
// 	//	st.lType=2;	st.pdwValue	= &m_Property.dwHotKey[0];	m_PropertyEnumToName[P_ATTR_HOTKEY0+i]=tmp;		m_mapNameValue[tmp]=st;			
// 	//}
// 	////ְҵ�ȼ�����
// 	//for(int i=0;i<OCC_Max;i++)
// 	//{
// 	//	sprintf(tmp, "OccuLvl%d", i);
// 	//	st.lType=0;	st.plValue	= (BYTE*)(&m_Property.byOccuLvl[i]);	m_PropertyEnumToName[P_ATTR_OCCULVL0+i]=tmp;		m_mapNameValue[tmp]=st;
// 	//}
// 	////ְҵ��������
// 	//for(int i=0;i<OCC_Max;i++)
// 	//{
// 	//	sprintf(tmp, "OccuExp%d", i);
// 	//	st.lType=2;	st.pdwValue	= &m_Property.dwOccuExp[i];	m_PropertyEnumToName[P_ATTR_OCCUEXP0+i]=tmp;		m_mapNameValue[tmp]=st;
// 	//}
// 	////ְҵSP����
// 	//for(int i=0;i<OCC_Max;i++)
// 	//{
// 	//	sprintf(tmp, "OccuSP%d", i);
// 	//	st.lType=2;	st.pdwValue	= &m_Property.dwOccuSP[i];	m_PropertyEnumToName[P_ATTR_OCCUSP0+i]=tmp;		m_mapNameValue[tmp]=st;
// 	//}
// 
// 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	///*ս������*/ 
// 	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wShieldDef;		m_PropertyEnumToName[P_ATTR_SHIELD_DEF]="C_ShieldDef";		m_mapNameValue["C_ShieldDef"]=st;							
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wImmunity;		m_PropertyEnumToName[P_ATTR_IMMUNITY]="C_Immunity";			 m_mapNameValue["C_Immunity"]=st;
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wPierce;			m_PropertyEnumToName[P_ATTR_PIERCE]="C_Pierce";				 m_mapNameValue["C_Pierce"]=st;
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wLuck;			m_PropertyEnumToName[P_ATTR_LUCK]="C_Luck";					 m_mapNameValue["C_Luck"]=st;
// 
// 	////HP�ָ�����
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wHpRecoverRatio;	m_PropertyEnumToName[P_ATTR_HPRECOVERRATIO]="C_HpRecoverRatio"; m_mapNameValue["C_HpRecoverRatio"]=st;							
// 
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wEnerRecoverSpeed;	m_PropertyEnumToName[P_ATTR_ENERRECOVERSPEED]="C_EnerRecoverSpeed";	 m_mapNameValue["C_EnerRecoverSpeed"]=st;							
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wFightEnerRecoverSpeed;m_PropertyEnumToName[P_ATTR_FIGHTENERRECOVERSPEED]="C_FightEnerRecoverSpeed";m_mapNameValue["C_FightEnerRecoverSpeed"]=st;				
// 
// 	//st.lType=4;	st.pValue	= &m_FightProperty.dwMaxHp;				m_PropertyEnumToName[P_ATTR_MAXHP]="C_MaxHp";			   	m_mapNameValue["C_MaxHp"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMaxMp;				m_PropertyEnumToName[P_ATTR_MAXMP]="C_MaxMp";		m_mapNameValue["C_MaxMp"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMaxStamina;			m_PropertyEnumToName[P_ATTR_MAXSTAMINA]="C_MaxStamina";		m_mapNameValue["C_MaxStamina"]=st;		
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMaxEnergy;			m_PropertyEnumToName[P_ATTR_MAXENERGY]="C_MaxEnergy";		m_mapNameValue["C_MaxEnergy"]=st;		
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wHpRecoverSpeed;		m_PropertyEnumToName[P_ATTR_HPRECOVERSPEED]="C_HpRecoverSpeed";	m_mapNameValue["C_HpRecoverSpeed"]=st;	
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMpRecoverSpeed;		m_PropertyEnumToName[P_ATTR_MPRECOVERSPEED]="C_MpRecoverSpeed";	m_mapNameValue["C_MpRecoverSpeed"]=st;	
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wStamRecoverSpeed;	m_PropertyEnumToName[P_ATTR_STAMRECOVERSPEED]="C_StamRecoverSpeed";	m_mapNameValue["C_StamRecoverSpeed"]=st;	
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wStrenth;			m_PropertyEnumToName[P_ATTR_STRENTH]="C_Strenth";			m_mapNameValue["C_Strenth"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wAgility;			m_PropertyEnumToName[P_ATTR_AGILITY]="C_Agility";			m_mapNameValue["C_Agility"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wConsti;				m_PropertyEnumToName[P_ATTR_CONSTI]="C_Consti";			m_mapNameValue["C_Consti"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wIntell;				m_PropertyEnumToName[P_ATTR_INTELL]="C_Intell";			m_mapNameValue["C_Intell"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wWisdom;				m_PropertyEnumToName[P_ATTR_WISDOM]="C_Wisdom";			m_mapNameValue["C_Wisdom"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wSpirit;				m_PropertyEnumToName[P_ATTR_SPIRIT]="C_Spirit";			m_mapNameValue["C_Spirit"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wFightBlock;			m_PropertyEnumToName[P_ATTR_BLOCK]="C_Block";				m_mapNameValue["C_Block"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wParry;				m_PropertyEnumToName[P_ATTR_PARRY]="C_Parry";				m_mapNameValue["C_Parry"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wDodge;				m_PropertyEnumToName[P_ATTR_DODGE]="C_Dodge";				m_mapNameValue["C_Dodge"]=st;			
// 
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wCharm;				m_PropertyEnumToName[P_ATTR_CHARM]="C_Charm";			m_mapNameValue["C_Charm"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wHit;				m_PropertyEnumToName[P_ATTR_HIT]="C_Hit";			m_mapNameValue["C_Hit"]=st;					
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wAbsorb;				m_PropertyEnumToName[P_ATTR_ABSORB]="C_Absorb";		m_mapNameValue["C_Absorb"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wDef;				m_PropertyEnumToName[P_ATTR_DEF]="C_Def";			m_mapNameValue["C_Def"]=st;					
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMdef;				m_PropertyEnumToName[P_ATTR_MDEF]="C_Mdef";			m_mapNameValue["C_Mdef"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wCri;				m_PropertyEnumToName[P_ATTR_CRI]="C_Cri";			m_mapNameValue["C_Cri"]=st;					
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMAtkSpeed;			m_PropertyEnumToName[P_ATTR_MATKSPEED]="C_MAtkSpeed";		m_mapNameValue["C_MAtkSpeed"]=st;		
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wBuffPower;			m_PropertyEnumToName[P_ATTR_BUFFPOWER]="SpeedC_BuffPower";m_mapNameValue["C_BuffPower"]=st;		
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMinAtk;;			m_PropertyEnumToName[P_ATTR_MINATK]="C_MinAtk";		m_mapNameValue["C_MinAtk"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMaxAtk;				m_PropertyEnumToName[P_ATTR_MAXATK]="C_MaxAtk";		m_mapNameValue["C_MaxAtk"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMAtk;				m_PropertyEnumToName[P_ATTR_MATK]="C_MAtk";			m_mapNameValue["C_MAtk"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wMCri;				m_PropertyEnumToName[P_ATTR_MCRI]="C_MCri";			m_mapNameValue["C_MCri"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wBloodSuk;			m_PropertyEnumToName[P_ATTR_BLOODSUCK]="C_BloodSuck";		m_mapNameValue["C_BloodSuck"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wManaSuck;			m_PropertyEnumToName[P_ATTR_MANASUCK]="C_ManaSuck";		m_mapNameValue["C_ManaSuck"]=st;			
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wWillDef;			m_PropertyEnumToName[P_ATTR_WILLDEF]="C_WillDef";		m_mapNameValue["C_WillDef"]=st;				
// 	//st.lType=5;	st.psValue	= &m_FightProperty.wConstiDef;			m_PropertyEnumToName[P_ATTR_CONSTIDEF]="C_ConstiDef";	m_mapNameValue["C_ConstiDef"]=st;		
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DFT_Bled];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_BLED]="C_DeBuffDef0";	 m_mapNameValue["C_DeBuffDef0"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Dizzy];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_DIZZY]="C_DeBuffDef1";	 m_mapNameValue["C_DeBuffDef1"]=st;			
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Lull];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_LULL]="C_DeBuffDef2";	 m_mapNameValue["C_DeBuffDef2"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Hypnosis];m_PropertyEnumToName[P_ATTR_DEBUFFDEF_HYPNOSIS]="C_DeBuffDef3";	m_mapNameValue["C_DeBuffDef3"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Poison];	m_PropertyEnumToName[P_ATTR_DEBUFFDEF_POSION]="C_DeBuffDef4"; m_mapNameValue["C_DeBuffDef4"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wDeBuffDef[DET_Fix];		m_PropertyEnumToName[P_ATTR_DEBUFFDEF_FIX]="C_DeBuffDef5";	 m_mapNameValue["C_DeBuffDef5"]=st;		
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Fire];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_FIRE]="C_ElemDef0";  m_mapNameValue["C_ElemDef0"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Water];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_WATER]="C_ElemDef1";  m_mapNameValue["C_ElemDef1"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Earth];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_EARTH]="C_ElemDef2";  m_mapNameValue["C_ElemDef2"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Wind];		 m_PropertyEnumToName[P_ATTR_ELEMDEF_WIND]="C_ElemDef3";  m_mapNameValue["C_ElemDef3"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Dark];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_DARK]="C_ElemDef4";	  m_mapNameValue["C_ElemDef4"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDef[ET_Holy];			 m_PropertyEnumToName[P_ATTR_ELEMDEF_HOLY]="C_ElemDef5";	  m_mapNameValue["C_ElemDef5"]=st;	
// 
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Human];			m_PropertyEnumToName[P_ATTR_CLASSDAM_HUMAN]="C_ClassDam0";   m_mapNameValue["C_ClassDam0"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Element];	;		m_PropertyEnumToName[P_ATTR_CLASSDAM_ELEMENT]="C_ClassDam1";   m_mapNameValue["C_ClassDam1"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Alien];			m_PropertyEnumToName[P_ATTR_CLASSDAM_ALIEN]="C_ClassDam2";   m_mapNameValue["C_ClassDam2"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Animal];			m_PropertyEnumToName[P_ATTR_CLASSDAM_ANIMAL]="C_ClassDam3";  m_mapNameValue["C_ClassDam3"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Undead];			m_PropertyEnumToName[P_ATTR_CLASSDAM_UNDEAD]="C_ClassDam4";  m_mapNameValue["C_ClassDam4"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Evil];			    m_PropertyEnumToName[P_ATTR_CLASSDAM_EVIL]="C_ClassDam5";   m_mapNameValue["C_ClassDam5"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wClassDam[C_Hexapods];		m_PropertyEnumToName[P_ATTR_CLASSDAM_HEXAPODS]="C_ClassDam6";  m_mapNameValue["C_ClassDam6"]=st;	
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Fire];			    m_PropertyEnumToName[P_ATTR_ELEMDAM_FIRE]="C_ElemDam0";		m_mapNameValue["C_ElemDam0"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Water];			m_PropertyEnumToName[P_ATTR_ELEMDAM_WATER]="C_ElemDam1";		m_mapNameValue["C_ElemDam1"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Earth];			m_PropertyEnumToName[P_ATTR_ELEMDAM_EARTH]="C_ElemDam2";		m_mapNameValue["C_ElemDam2"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Wind];	;		    m_PropertyEnumToName[P_ATTR_ELEMDAM_WIND]="C_ElemDam3";		m_mapNameValue["C_ElemDam3"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Dark];		    	m_PropertyEnumToName[P_ATTR_ELEMDAM_DARK]="C_ElemDam4";		m_mapNameValue["C_ElemDam4"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemDam[ET_Holy];			    m_PropertyEnumToName[P_ATTR_ELEMDAM_HOLY]="C_ElemDam5";		m_mapNameValue["C_ElemDam5"]=st;	
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Fire];			    m_PropertyEnumToName[P_ATTR_ELEMATK_FIRE]="C_ElemAtk0";		m_mapNameValue["C_ElemAtk0"]=st;		
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Water];			m_PropertyEnumToName[P_ATTR_ELEMATK_WATER]="C_ElemAtk1";		m_mapNameValue["C_ElemAtk1"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Earth];			m_PropertyEnumToName[P_ATTR_ELEMATK_EARTH]="C_ElemAtk2";		m_mapNameValue["C_ElemAtk2"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Wind];			    m_PropertyEnumToName[P_ATTR_ELEMATK_WIND]="C_ElemAtk3";		m_mapNameValue["C_ElemAtk3"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Dark];			    m_PropertyEnumToName[P_ATTR_ELEMATK_DARK]="C_ElemAtk4";		m_mapNameValue["C_ElemAtk4"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wElemAtk[ET_Holy];			    m_PropertyEnumToName[P_ATTR_ELEMATK_HOLY]="C_ElemAtk5";		m_mapNameValue["C_ElemAtk5"]=st;	
// 
// 	//st.lType=5;st.psValue = &m_FightProperty.wCriDef;		m_PropertyEnumToName[P_ATTR_CRIDEF]="C_CriDef";								m_mapNameValue["C_CriDef"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wMCriDef;		m_PropertyEnumToName[P_ATTR_MCRIDEF]="C_MCriDef";							m_mapNameValue["C_MCriDef"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wCriDamage;	m_PropertyEnumToName[P_ATTR_CRIDAMAGE]="C_CriDamage";						m_mapNameValue["C_CriDamage"]=st;	
// 	//st.lType=5;st.psValue = &m_FightProperty.wMCriDamage;	m_PropertyEnumToName[P_ATTR_MCRIDAMAGE]="C_MCriDamage";						m_mapNameValue["C_MCriDamage"]=st;	
// 
// 
// 	//st.lType=2;	st.pdwValue=&m_dwFactureExp;				m_PropertyEnumToName[P_ATTR_FACTUREEXP]="FactureExp";						m_mapNameValue["FactureExp"]=st;						//	��������
// 	//st.lType=2;	st.pdwValue=&m_dwCollectExp;				m_PropertyEnumToName[P_ATTR_COLLECTIONEXP]="CollectionExp";					m_mapNameValue["CollectionExp"]=st;					//	�ɼ�����
// 	//st.lType=2;	st.pdwValue=&m_dwFactureLevel;				m_PropertyEnumToName[P_ATTR_FACTUREGRADE]="FactureGrade";					m_mapNameValue["FactureGrade"]=st;					//	�����ȼ�
// 	//st.lType=2;	st.pdwValue=&m_dwCollectLevel;				m_PropertyEnumToName[P_ATTR_COLLECTIONGRADE]="CollectionGrade";				m_mapNameValue["CollectionGrade"]=st;				//	�ɼ��ȼ�
// 
// 	//st.lType=0;	st.plValue  = &m_AppendProperty.bDisplayHeadPiece;		m_PropertyEnumToName[P_ATTR_DISPLAYHEADPIECE]="DisplayHeadPiece";m_mapNameValue["DisplayHeadPiece"]=st;
// }

// DWORD CPlayer::GetValue(const char* strName)
// {
// 	//if (m_mapNameValue.find(strName)!=m_mapNameValue.end())
// 	//{
// 	//	switch (m_mapNameValue[strName].lType)
// 	//	{
// 	//	case 0:	// byte
// 	//		return *m_mapNameValue[strName].plValue;
// 
// 	//	case 1:	// word
// 	//		return *m_mapNameValue[strName].pwValue;
// 
// 	//	case 2:	// dword
// 	//		return *m_mapNameValue[strName].pdwValue;
// 
// 	//	case 3:	// float
// 	//		return (DWORD)*m_mapNameValue[strName].pfValue;
// 
// 	//	case 4:	// long
// 	//		return *m_mapNameValue[strName].pValue;
// 
// 	//	case 5:	// short
// 	//		return *m_mapNameValue[strName].psValue;
// 
// 	//	case 6:	// bool
// 	//		return *m_mapNameValue[strName].pbValue;
// 	//	}
// 	//}
// 	return 0;
// }

// DWORD CPlayer::SetValue(const char* strName, DWORD dwValue)
// {
// 	//if (m_mapNameValue.find(strName)!=m_mapNameValue.end())
// 	//{
// 	//	switch (m_mapNameValue[strName].lType)
// 	//	{
// 	//	case 0:	// byte
// 	//		*m_mapNameValue[strName].plValue = (BYTE)dwValue;
// 	//		return dwValue;
// 
// 	//	case 1:	// word
// 	//		*m_mapNameValue[strName].pwValue = (WORD)dwValue;
// 	//		return dwValue;
// 
// 	//	case 2:	// dword
// 	//		*m_mapNameValue[strName].pdwValue = (DWORD)dwValue;
// 	//		return dwValue;
// 
// 	//	case 3:	// float
// 	//		*m_mapNameValue[strName].pfValue = (float)dwValue;
// 	//		return dwValue;
// 
// 	//	case 4:	// long
// 	//		*m_mapNameValue[strName].pValue = (long)dwValue;
// 	//		return dwValue;
// 
// 	//	case 5:	// short
// 	//		*m_mapNameValue[strName].psValue = (short)dwValue;
// 	//		return dwValue;
// 
// 	//	case 6:	// bool
// 	//		*m_mapNameValue[strName].pbValue = dwValue != 0;
// 	//		return dwValue;
// 	//	}
// 	//}
// 	return 0;
// }

// DWORD CPlayer::ChangeValue(const char* strName, long lValue)
// {
// 	//if (m_mapNameValue.find(strName)!=m_mapNameValue.end())
// 	//{
// 	//	switch (m_mapNameValue[strName].lType)
// 	//	{
// 	//	case 0:	// byte
// 	//		*m_mapNameValue[strName].plValue += (BYTE)lValue;
// 	//		return *m_mapNameValue[strName].plValue;
// 
// 	//	case 1:	// word
// 	//		*m_mapNameValue[strName].pwValue += (WORD)lValue;
// 	//		return *m_mapNameValue[strName].pwValue;
// 
// 	//	case 2:	// dword
// 	//		*m_mapNameValue[strName].pdwValue += lValue;
// 	//		return *m_mapNameValue[strName].pdwValue;
// 
// 	//	case 3:	// float
// 	//		*m_mapNameValue[strName].pfValue += lValue;
// 	//		return (DWORD)*m_mapNameValue[strName].pfValue;
// 
// 	//	case 4:	// long
// 	//		*m_mapNameValue[strName].pValue += lValue;
// 	//		return *m_mapNameValue[strName].pValue;
// 
// 	//	case 5:	// short
// 	//		*m_mapNameValue[strName].psValue += (short)lValue;
// 	//		return *m_mapNameValue[strName].psValue;
// 
// 	//	case 6:	// bool
// 	//		*m_mapNameValue[strName].pbValue = (lValue!=0);
// 	//		return *m_mapNameValue[strName].pbValue;
// 	//	}
// 	//}
// 	return 0;
// }

// bool CPlayer::IsTeammate(const CGUID& id)
// {
// 	vector<tagWSMemberInfo>::iterator iter = m_vecTeamMateInfo.begin();
// 	for (;iter != m_vecTeamMateInfo.end();++iter)
// 	{
// 		tagWSMemberInfo t = *iter;
// 		if ( t.guid == id ) return true;
// 	}
// 	return false;
// }

// ���ָ��ID��Ա����Ϣ
// tagWSMemberInfo * CPlayer::GetWsMemberInfo(const CGUID& id)
// {
// 	vector<tagWSMemberInfo>::iterator iter = m_vecTeamMateInfo.begin();
// 	for (;iter != m_vecTeamMateInfo.end();++iter)
// 	{
// 		tagWSMemberInfo t = *iter;
// 		if ( t.guid == id )
// 			return &(*iter);
// 	}
// 	return NULL;
// }


// void	CPlayer::SetguID(LONG lExtendID,const CGUID& guid)
// {
// 	if( lExtendID==8 )
// 	{
// 		//GetGame()->GetInterface()->GetDepotPage()->SetguDepotMoneyID(guid);
// 	}
// 	else if( lExtendID==4 )
// 	{
// 		SetPlayerMoneyGuid(guid);
// 	}
// 	else if( lExtendID==5 )
// 	{
// 		SetPlayerYuanBaoGuid(guid);
// 	}
// 	else if( lExtendID==6 )
// 	{
// 		SetPlayerJiFenGuid(guid);
// 	}
// }

// bool CPlayer::AddGoods(CGoods *pGoods,DWORD dwPos,LONG lExtendID)
// {
// 	if (pGoods==NULL)
// 		return false;
// 
// 	bool bRet = false;			//	����ֵ
// 	bool bDelGoods = false;
// 
// 	CGUID tempTestGoodsGuid = pGoods->GetExID();
// 	DWORD dwTempGoodsIndex = pGoods->GetIndex();
// 
// 	DWORD dwInsertNum = pGoods->GetNum();
// 
// 	if( lExtendID==PEI_WALLET )						//	���
// 	{
// 		if( m_dwPlayerMoneyNum == 0 )
// 			SetPlayerMoneyGuid(pGoods->GetExID());
// 		SetPlayerMoneyNum(m_dwPlayerMoneyNum + pGoods->GetNum());
// 		bRet = true;
// 	}
// 	else if( lExtendID == PEI_SILVERWALLET )		//	����
// 	{
// 		if( m_dwPlayerSilverMoneyNum == 0 )
// 			SetPlayerSilverMoneyGuid(pGoods->GetExID());
// 		SetPlayerSilverMoneyNum(m_dwPlayerSilverMoneyNum + pGoods->GetNum());
// 		bRet = true;
// 	}
// 	else if( lExtendID == PEI_YUANBAO )				//	Ԫ��
// 	{
// 		if( m_dwPlayerSilverMoneyNum == 0)
// 			SetPlayerYuanBaoGuid(pGoods->GetExID());
// 		SetPlayerYuanBaoNum(m_dwPlayerYuanBaoNum + pGoods->GetNum());
// 		bRet = true;
// 	}
// 	else if( lExtendID == PEI_JIFEN )
// 	{
// 		SetPlayerJiFenNum(pGoods->GetNum());
// 		bRet = true;
// 	}
// 	//else if( lExtendID == 3 )
// 	//{
// 
// 	//}
// 	else if( lExtendID==PEI_EQUIPMENT )		// װ��
// 	{
// 		m_pEquipMgr->SetEquip(dwPos,pGoods);
// 		bRet = true;
// 	}
// 	else if (lExtendID==PEI_SUBPACK)					//�ӱ�����
// 	{
// 		if (dwPos>=PEI_PACK1&&dwPos<=PEI_PACK5)
// 		{
// 			m_pPackMgr->SetSubPGoods(dwPos-PEI_PACK1,pGoods);
// 			bRet = true;
// 		}
// 	}
// 	else if(lExtendID==PEI_BUSSINESPACKFIELD)
// 	{
// 		//		SetBussinesPack(pGoods);
// 		bRet = true;
// 	}
// 	else if(lExtendID==PEI_BUSSINESPACK)
// 	{
// 		tagGoods* ptGoods = GetBusinessGoods(dwPos);
// 		list<tagGoods>* pLGoods = NULL;
// 		if(ptGoods&&ptGoods->pGoods&&ptGoods->pGoods->GetIndex() == pGoods->GetIndex() )
// 		{
// 			long num = ptGoods->pGoods->GetNum() + pGoods->GetNum();
// 			if(num <= pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1) && num >0)
// 			{
// 				ptGoods->pGoods->SetNum(num);
// 				ptGoods->lNum = num;
// 				ptGoods->bChange = true;
// 				bDelGoods = true;
// 			}
// 			else if( num > ptGoods->pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1))
// 			{
// 				CGoods tGoods;
// 				tGoods.SetNum( pGoods->GetNum() );
// 				pGoods->SetNum(ptGoods->pGoods->GetNum());
// 				ptGoods->pGoods->SetNum( tGoods.GetNum() );
// 				ptGoods->lNum = tGoods.GetNum();
// 			}
// 		}
// 		else
// 		{
// 			tagGoods stGoods;
// 			stGoods.bChange = true;
// 			stGoods.pGoods = pGoods;
// 			stGoods.lPos = dwPos;
// 			stGoods.lNum = pGoods->GetNum();
// 			stGoods.lType = PEI_BUSSINESPACK;
// 			if(m_CoteBussinesPack != NULL )
// 			{
// 				m_CoteBussinesPack->m_listCoteGoods.push_back(stGoods);
// 				m_CoteBussinesPack->m_mapCoteHaveGoods[stGoods.lPos] = TRUE;
// 			}
// 		}
// 		bRet = true;
// 	}
// 	else if( lExtendID == PEI_PACKET )				//ԭʼ����
// 	{
// 		tagGoods* pstGoods = GetGoods(dwPos);
// 		list<tagGoods>* pLGoods = NULL;
// 
// 		//	��� Ŀ�����(dwPos) ����Ʒ
// 		if(pstGoods&& pstGoods->pGoods&& pstGoods->pGoods->GetIndex() == pGoods->GetIndex())
// 		{
// 			long num =0;
// 			num = pstGoods->pGoods->GetNum() + pGoods->GetNum();
// 
// 			//	����
// 			if(num<=pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1)&&num>0)
// 			{
// 				pstGoods->pGoods->SetNum(num);
// 				pstGoods->lNum=num;
// 				pstGoods->bChange=true;
// 
// 				bDelGoods = true;
// 			}
// 			//	����
// 			else if (num>pstGoods->pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1))
// 			{
// 				CGoods stGoods;
// 				stGoods.SetNum(pGoods->GetNum());
// 				pGoods->SetNum(pstGoods->pGoods->GetNum());
// 				pstGoods->pGoods->SetNum(stGoods.GetNum());
// 				pstGoods->lNum = stGoods.GetNum();
// 				bRet = true;
// 			}
// 			m_setPacketEmptyPos.insert( pstGoods->lPos );
// 		}
// 		else
// 		{
// 			tagGoods tGoods;
// 			tGoods.bChange =  true;
// 			tGoods.pGoods = pGoods;
// 			tGoods.lPos = dwPos;
// 			tGoods.lType = PEI_PACKET;
// 			tGoods.lNum = pGoods->GetNum();
// 			pLGoods = GetGoodsList();
// 			if(pLGoods)
// 			{
// 				pLGoods->push_back(tGoods);
// 				m_setPacketEmptyPos.insert(dwPos);
// 			}
// 		}
// 
// 		bRet = true;
// 	}
// 	else if ( lExtendID == PEI_PACK1 || lExtendID == PEI_PACK2 || lExtendID == PEI_PACK3 || lExtendID == PEI_PACK4 || lExtendID <= PEI_PACK5) //�ӱ���
// 	{
// 		tagGoods* pCoteGoods = GetSubGoods(lExtendID - PEI_PACK1,dwPos);
// 		list<tagGoods>* PLGoods = GetSubGoodslist(lExtendID-PEI_PACK1);
// 		//	Ŀ��λ���Ƿ�����Ʒ
// 		if(pCoteGoods&&pCoteGoods->pGoods&&pCoteGoods->pGoods->GetIndex()==pGoods->GetIndex())
// 		{
// 			long num;
// 			num = pCoteGoods->pGoods->GetNum() + pGoods->GetNum();
// 			//	����
// 			if(num<=pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1)&&num>0)
// 			{
// 				pCoteGoods->pGoods->SetNum(num);
// 				pCoteGoods->lNum = num;
// 				pCoteGoods->bChange = true;
// 				// �ڴ�й© [12/27/2008 Square]
// 				bDelGoods = true;
// 			}
// 			//	����
// 			else if(num > pCoteGoods->pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1))
// 			{
// 				CGoods stGoods;
// 				stGoods.SetNum(pGoods->GetNum());
// 				pGoods->SetNum(pCoteGoods->pGoods->GetNum());
// 				pCoteGoods->pGoods->SetNum(stGoods.GetNum());
// 				pCoteGoods->lNum = stGoods.GetNum();
// 			}
// 			m_Cote[lExtendID-PEI_PACK1]->m_mapCoteHaveGoods[dwPos] = TRUE;
// 		}
// 		else
// 		{
// 			tagGoods tGoods;
// 			tGoods.bChange = true;
// 			tGoods.pGoods = pGoods;
// 			tGoods.lType = lExtendID;
// 			tGoods.lPos = dwPos;
// 			tGoods.lNum = pGoods->GetNum();
// 			if(PLGoods)
// 			{	
// 				m_Cote[lExtendID-PEI_PACK1]->m_mapCoteHaveGoods[dwPos] = TRUE;
// 				PLGoods->push_back(tGoods);
// 			}
// 		}
// 
// 		bRet = true;
// 	}
// 	else if( lExtendID == eDCT_Primary || lExtendID == eDCT_Secondary || lExtendID == eDCT_Secondary1 ||
// 		lExtendID == eDCT_Secondary2 || lExtendID == eDCT_Secondary3 || lExtendID == eDCT_Secondary4 )
// 	{
// 		bRet = m_pDepotMgr->AddDepotGoods(lExtendID,dwPos, pGoods);
// 	}
// 
// 	if(bRet == false)
// 	{
// 		char outinfo[128],strGuid[128];
// 		tempTestGoodsGuid.tostring(strGuid);
// 		sprintf(outinfo,"%s AddGoods Index(%d) Number(%d) Guid %s To Con(%d) Pos(%d) Failed!",GetName(),dwTempGoodsIndex,dwInsertNum,strGuid,lExtendID,dwPos);
// 		//		OutputContainerInfo(outinfo);
// 	}
// 	else
// 	{
// 		char outinfo[128],strGuid[128];
// 		tempTestGoodsGuid.tostring(strGuid);
// 		if( pGoods )
// 		{
// 			sprintf(outinfo,"%s AddGoods Index(%d) Number(%d) Guid %s To Con(%d) Pos(%d) Succeed!",GetName(),dwTempGoodsIndex,dwInsertNum,strGuid,lExtendID,dwPos);
// 		}
// 		else
// 		{
// 			sprintf(outinfo,"%s InsertGoods Index(%d) Number(%d) Guid %s To Con(%d) Pos(%d) Succeed!",GetName(),dwTempGoodsIndex,dwInsertNum,strGuid,lExtendID,dwPos);
// 		}
// 		// 		OutputContainerInfo(outinfo);
// 	}
// 
// 	if( bDelGoods )
// 	{
// 		SAFE_DELETE(pGoods);
// 	}
// 	pGoods = NULL;
// 	return bRet;
// }

//�������ΪwGoodsIndex����Ʒ���б���
// bool CPlayer::AddShortCutGoods(WORD wGoodsIndex)
// {
// 	//if (CGoodsList::GetEffectVal(wGoodsIndex,CGoods::GAP_COOLDOWN,1)!=0)
// 	//{
// 	//	list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//	for (;it!=m_listShortCutGoods.end();it++)
// 	//	{
// 	//		if (it->wID==wGoodsIndex)
// 	//		{
// 	//			it->dwRestoreTime = CGoodsList::GetEffectVal(wGoodsIndex,CGoods::GAP_COOLDOWN,1);
// 	//			return false;
// 	//		}
// 	//	}
// 	//	tagShortCutGoods temp;
// 	//	temp.wID = wGoodsIndex;
// 	//	temp.dwRestoreTime = CGoodsList::GetEffectVal(wGoodsIndex,CGoods::GAP_COOLDOWN,1);
// 	//	m_listShortCutGoods.push_back(temp);
// 	//	return true;
// 	//}
// 	return false;
// }
//���б���ɾ������ΪwGoodsIndex����Ʒ
// bool CPlayer::DelShortCutGoods(WORD wGoodsIndex)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		m_listShortCutGoods.erase(it);
// 	//		return true;
// 	//	}
// 	//}
// 	return false;
// }
//��������ΪwGoodsIndex����Ʒ����ȴʱ��
// void CPlayer::SetGoodsRestoreTime(WORD wGoodsIndex,DWORD dwTime)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		it->dwRestoreTime = dwTime;
// 	//	}
// 	//}
// }

//��������ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
// void CPlayer::SetGoodsCoolDownStartTime(WORD wGoodsIndex,DWORD dwTime)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		it->dwCoolDownStartTime = dwTime;
// 	//	}
// 	//}
// }

//�õ�����ΪwGoodsIndex����Ʒ����ȴʱ��
// DWORD CPlayer::GetGoodsRestoreTime(WORD wGoodsIndex)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		return it->dwRestoreTime;
// 	//	}
// 	//}
// 	return 0;
// }

// //�õ�����ΪwGoodsIndex����Ʒ����ȴ��ʼʱ��
// DWORD CPlayer::GetGoodsCoolDownStartTime(WORD wGoodsIndex)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		return it->dwCoolDownStartTime;
// 	//	}
// 	//}
// 	return 0;
// }
// 
// //��������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
// void CPlayer::SetGoodsUseState(WORD wGoodsIndex,bool bState)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		it->bCanBeUsed = bState;
// 	//	}
// 	//}
// }

//ȡ������ΪwGoodsIndex����Ʒ��ǰ�Ƿ�ɱ�ʹ�õ�״̬
// bool CPlayer::GetGoodsUseState(WORD wGoodsIndex)
// {
// 	//list<tagShortCutGoods>::iterator it = m_listShortCutGoods.begin();
// 	//for (;it!=m_listShortCutGoods.end();it++)
// 	//{
// 	//	if (it->wID==wGoodsIndex)
// 	//	{
// 	//		return it->bCanBeUsed;
// 	//	}
// 	//}
// 	return false;
// }

// CGoods *CPlayer::RemoveGoods(const CGUID& SourceGUID, const CGUID& DesGUID, DWORD dwPos, LONG lExtendID, DWORD dwAmount)
// {
// 	CGoods* pRetGoods = NULL;
// 
// 	//if( lExtendID==10 )	// ����
// 	//{
// 	//}
// 	//else if( lExtendID==9 )	// �ֿ�
// 	//{
// 	//}
// 	//else if( lExtendID==8 )	// ����
// 	//{
// 	//}
// 	//else if( lExtendID==7 )	// ����
// 	//{
// 	//}	//���Ǯ��
// 	/*else */if(lExtendID == PEI_WALLET)
// 	{
// 		SetPlayerMoneyNum(GetPlayerMoneyNum() - dwAmount);
// 
// 		pRetGoods = new CGoods();
// 		pRetGoods->SetExID(SourceGUID);
// 		pRetGoods->SetIndex(MONEY_INDEX);
// 		pRetGoods->GetProperty()->dwType = GT_MONEY;
// 		pRetGoods->SetNum(dwAmount);
// 		pRetGoods->SetType(TYPE_GOODS);
// 	}
// 	//����Ǯ��
// 	else if( lExtendID == PEI_SILVERWALLET )	// ����Ǯ��
// 	{
// 		SetPlayerSilverMoneyNum(GetPlayerSilverMoneyNum()-dwAmount);
// 		pRetGoods = new CGoods();
// 		pRetGoods->SetExID(SourceGUID);
// 		pRetGoods->SetIndex(YUANBAO_INDEX);
// 		pRetGoods->GetProperty()->dwType = GT_SILVERMONEY;
// 		pRetGoods->SetNum(dwAmount);
// 	}
// 	//Ԫ��
// 	else if(lExtendID == PEI_YUANBAO)
// 	{
// 		SetPlayerYuanBaoNum(GetPlayerYuanBaoNum() - dwAmount);
// 		pRetGoods = new CGoods();
// 		pRetGoods->SetExID(SourceGUID);
// 		pRetGoods->SetType(TYPE_GOODS);
// 		pRetGoods->SetIndex(YUANBAO_INDEX);
// 		pRetGoods->SetNum(dwAmount);
// 	}
// 	//����
// 	else if(lExtendID == PEI_JIFEN)
// 	{
// 		SetPlayerJiFenNum(GetPlayerJiFenNum() - dwAmount);
// 		pRetGoods = new CGoods;
// 		pRetGoods->SetExID(SourceGUID);
// 		pRetGoods->SetType(TYPE_GOODS);
// 		pRetGoods->SetIndex(JIFEN_INDEX);
// 		pRetGoods->SetNum(dwAmount);
// 	}
// 	//װ��
// 	else if(lExtendID == PEI_EQUIPMENT)
// 	{
// 		pRetGoods = m_pEquipMgr->GetEquip(dwPos);
// 		m_pEquipMgr->SetEquip(dwPos,pRetGoods);
// 	}
// 	//����
// 	else if(lExtendID == PEI_PACKET)
// 	{
// 		int nIndex = (int)dwPos;
// 		list<tagGoods>* pGoodsList = GetGoodsList();
// 		if(pGoodsList == NULL)
// 		{
// 			return NULL;
// 		}
// 
// 		CGoods* pTemp = NULL;
// 		list<tagGoods>::iterator it = pGoodsList->begin();
// 
// 		for(; it!= pGoodsList->end(); )
// 		{
// 			//���ݴ����GUID������Ʒ�����ж�Ӧ����Ʒ
// 			if(it->pGoods->GetExID() == SourceGUID )
// 			{
// 				//�������Ĳ�������С����Ʒ�����������ʾҪ���в�ֲ���������ֻ��������������Ʒ�Ĳ���
// 				if( it->pGoods->GetNum() > dwAmount && dwAmount!=0 )
// 				{
// 					DWORD index = it->pGoods->GetIndex();
// 
// 					pRetGoods = new CGoods;
// 					pRetGoods->SetNum(dwAmount);
// 					pRetGoods->SetExID(DesGUID);
// 					pRetGoods->SetIndex( index );
// 					pRetGoods->SetName(it->pGoods->GetName());
// 
// 					it->pGoods->SetNum( it->pGoods->GetNum() - dwAmount );
// 					it->lNum = it->pGoods->GetNum();
// 					m_setPacketEmptyPos.insert(it->lPos);
// 				}
// 				else 
// 				{
// 					set<DWORD>::iterator itPos = m_setPacketEmptyPos.find( it->lPos );
// 					if( itPos != m_setPacketEmptyPos.end() )
// 						m_setPacketEmptyPos.erase(itPos);
// 
// 					pRetGoods = it->pGoods;
// 					pGoodsList->erase(it);
// 				}
// 				break;
// 			}
// 			else
// 			{
// 				it++;
// 			}
// 		}
// 	}
// 	else if(lExtendID == PEI_SUBPACK)						
// 	{
// 		int nIndex = (int)dwPos;
// 		if ( nIndex >= PEI_PACK1 && nIndex <=PEI_PACK5 )
// 		{
// 			pRetGoods = GetCotePackage(nIndex - PEI_PACK1)->pGoods;
// 			if(pRetGoods ==NULL)	
// 			{
// 				return NULL;
// 			}
// 			SetSubPGoods(nIndex - PEI_PACK1,NULL);
// 		}
// 	}
// 	else if( lExtendID == PEI_PACK1 || lExtendID == PEI_PACK2 || lExtendID == PEI_PACK3 || lExtendID == PEI_PACK4 || lExtendID == PEI_PACK5 )
// 	{
// 		int nIndex = (int)dwPos;
// 		CGoods* pTemp = NULL;
// 		list<tagGoods> *pGoodsList = GetSubGoodslist( lExtendID - PEI_PACK1);
// 		if ( pGoodsList ==NULL)
// 		{
// 			return NULL;
// 		}
// 		list<tagGoods>::iterator it = pGoodsList->begin();
// 		for(; it!= pGoodsList->end(); )
// 		{
// 			/*���ݴ����GUID������Ʒ�����ж�Ӧ����Ʒ*/
// 			if( it->pGoods->GetExID()== SourceGUID)
// 			{
// 				/*�������Ĳ�������С����Ʒ�����������ʾҪ���в�ֲ���������ֻ��������������Ʒ�Ĳ���*/
// 				if( it->pGoods->GetNum()>dwAmount && dwAmount )
// 				{
// 					pRetGoods = new CGoods;
// 					pRetGoods->SetNum(dwAmount);
// 					pRetGoods->SetExID(DesGUID);
// 					pRetGoods->SetIndex( it->pGoods->GetIndex() );
// 					//					pRetGoods->SetBaseProperty(it->pGoods->GetBaseProperty());
// 					pRetGoods->SetName(it->pGoods->GetName());
// 					//					pRetGoods->SetGraphicsID(it->pGoods->GetGraphicsID());
// 
// 					it->pGoods->SetNum( it->pGoods->GetNum() - dwAmount );
// 					it->lNum = it->pGoods->GetNum();
// 					m_Cote[lExtendID - PEI_PACK1]->m_mapCoteHaveGoods[it->lPos] = TRUE;
// 				}
// 				else
// 				{
// 					pRetGoods = it->pGoods;
// 					m_Cote[lExtendID - PEI_PACK1]->m_mapCoteHaveGoods[it->lPos] = FALSE;
// 					pGoodsList->erase(it);
// 				}			
// 				break;
// 			}else
// 			{
// 				it++;
// 			}
// 		}
// 	}
// 	else if(lExtendID>= eDCT_Gold && lExtendID <DEPOT_CONTAINER_TYPE_END_VALUE)
// 	{
// 		pRetGoods = m_pDepotMgr->DelDepotGoods(lExtendID,dwPos,dwAmount,SourceGUID,DesGUID);
// 	}
// 
// 	if(pRetGoods == NULL)
// 	{
// 		char outinfo[128], strGuid[128];
// 		SourceGUID.tostring(strGuid);
// 		sprintf(outinfo,"%s RemoveGoods Guid%s From Con(%d) Pos(%d) Failed!",GetName(),strGuid,lExtendID,dwPos);
// 		//		OutputContainerInfo(outinfo);
// 	}
// 	else
// 	{
// 		char outinfo[128], strGuid[128];
// 		SourceGUID.tostring(strGuid);
// 		sprintf(outinfo,"%s RemoveGoods Index(%d) Number(%d) Guid%s From Con(%d) Pos(%d) Succeed!",GetName(),pRetGoods->GetIndex(),pRetGoods->GetNum(),strGuid,lExtendID,dwPos);
// 		//		OutputContainerInfo(outinfo);
// 	}
// 	return pRetGoods;
// }

// long CPlayer::GetGoodsAmount(long nIdx)
// {
// 	long ret = 0;
// 	WORD index = 0;
// 	while(GetGoodsList(index))
// 	{
// 		for(list<tagGoods>::iterator iter = GetGoodsList(index)->begin();
// 			iter!=GetGoodsList(index)->end(); ++iter)
// 		{
// 			if( iter->pGoods->GetIndex() == nIdx )
// 				ret += iter->pGoods->GetNum();
// 		}
// 		++index;
// 	}
// 	return ret;
// }

// bool CPlayer::HasPetArount()
// {
// 	return /*m_setPetID.size()>0*/!m_setPetID.empty();
// }

// long CPlayer::GetPetAmount()
// {
// 	return m_setPetID.size();
// }

void CPlayer::SetAction(WORD l)
{
	CMoveShape::SetAction(l);
}
void CPlayer::SetState(WORD l)
{
	CMoveShape::SetState(l);
}

// bool CPlayer::ChangeObjectAmount(LONG lContainerExtendID,LONG lObjectType,CGUID& guObjectID,DWORD dwAmount)
// {
// 	bool bRet = false;
// 	CGoods* pGoods = NULL;
// 
// 	if(lContainerExtendID == PEI_WALLET)
// 	{
// 		SetPlayerMoneyNum(dwAmount);
// 		bRet = true;
// 	}
// 	else if( lContainerExtendID == PEI_SILVERWALLET )
// 	{
// 		SetPlayerSilverMoneyNum(dwAmount);
// 		bRet = true;
// 	}
// 	else if(lContainerExtendID == PEI_PACKET)
// 	{
// 		list<tagGoods>::iterator it = m_listMainPackGoods.begin();
// 		for( ;it != m_listMainPackGoods.end();it++)
// 		{
// 			if( it->pGoods->GetExID()==guObjectID && it->pGoods->GetType()==lObjectType )
// 			{
// 				int nIndex = it->lPos;
// 				it->pGoods->SetNum(dwAmount);
// 				it->lNum = dwAmount;
// 				bRet = true;
// 			}
// 		}
// 	}
// 	else if (lContainerExtendID <= PEI_PACK5&&lContainerExtendID >= PEI_PACK1)
// 	{
// 		list<tagGoods> pGoodsList = m_Cote[lContainerExtendID-PEI_PACK1]->m_listCoteGoods;
// 		list<tagGoods>::iterator it = pGoodsList.begin();
// 
// 		for( ;it != pGoodsList.end();it++)
// 		{
// 			if( it->pGoods->GetExID()==guObjectID && it->pGoods->GetType()==lObjectType )
// 			{
// 				int nIndex = it->lPos;
// 				it->pGoods->SetNum(dwAmount);
// 				it->lNum=dwAmount;
// 				bRet = true;
// 			}
// 		}
// 	}
// 	else if(lContainerExtendID==PEI_YUANBAO)
// 	{
// 		SetPlayerYuanBaoNum(dwAmount);
// 		bRet = true;
// 	}
// 	else if(lContainerExtendID==PEI_JIFEN)
// 	{
// 		SetPlayerJiFenNum(dwAmount);
// 		bRet = true;
// 	}
// 
// 	return false;
// }

//bool CPlayer::ClearGoods()
//{
//	return false;
//}



//ͨ��ԭʼ���õ�һ��Goods
// tagGoods* CPlayer::GetGoodsByOrgName(const char* strOrgName)
// {
// 	//CGoods* pGoods = NULL;
// 	//ԭʼ����
// 	list<tagGoods>::iterator it = m_listMainPackGoods.begin();
// 	for( ; it != m_listMainPackGoods.end(); it++)
// 	{
// 		if( !strcmp(it->pGoods->GetOrginName(), strOrgName) )
// 		{
// 			return &(*it);
// 		}
// 	}
// 	//��ѯ�ӱ���
// 	for (int i = 0;i<COTE_NUM;i++)
// 	{	
// 		//���ӱ����в���
// 		list<tagGoods>* pGoodList = GetSubGoodslist(i);
// 		if (pGoodList)
// 		{
// 			list<tagGoods>::iterator GoodsIter=pGoodList->begin();
// 			for(;GoodsIter!=pGoodList->end();GoodsIter++)
// 			{
// 				if( !strcmp(GoodsIter->pGoods->GetOrginName(), strOrgName) )
// 					return &(*GoodsIter);
// 			}
// 		}
// 	}
// 	return NULL;
// }
// 
// //ͨ��λ�ò�����Ʒ
// CGoods* CPlayer::GetGoodsByPosition(long lExtendID,WORD dwPos)
// {
// 	//CGoods* pGoods = NULL;
// 	if (lExtendID==PEI_PACKET&&dwPos>=0&&dwPos<HOST_GRIDITEM)
// 	{
// 		tagGoods* pstGoods = GetGoods((WORD)dwPos);
// 		if (pstGoods)
// 		{
// 			return pstGoods->pGoods;
// 		}
// 	}else if (lExtendID<=PEI_PACK5&&lExtendID>=PEI_PACK1)
// 	{
// 		WORD wGridNum = GetPackageDimen(lExtendID-PEI_PACK1);
// 		if (dwPos>=0&&dwPos<wGridNum)
// 		{
// 			tagGoods* pstGoods = GetSubGoods((WORD)(lExtendID-PEI_PACK1),(WORD)dwPos);
// 			if (pstGoods)
// 			{
// 				return pstGoods->pGoods;
// 			}
// 		}
// 	}else if (lExtendID==PEI_EQUIPMENT)//&&dwPos>=0&&dwPos<EQUIP_SUM)
// 	{
// 		return m_pEquipMgr->GetEquip(dwPos);
// 	}
// 	return NULL;
// }
// 
// BOOL CPlayer::GetPacketHaveGoods(DWORD dwIndex)
// {
// 	if( dwIndex>=0 && dwIndex < HOST_GRIDITEM)
// 		return m_setPacketEmptyPos.find(dwIndex) == m_setPacketEmptyPos.end() ? false : true;
// 	return FALSE;
// }
// 
// //��ȡָ����ŵ��ӱ���������Ϊ��Ʒ������
// CGoods* CPlayer::GetSubPGoods(WORD index)
// {
// 	if (index>=0&&index<5&&m_Cote[index]!=NULL)
// 	{
// 		return m_Cote[index]->pGoods;
// 	}
// 	return NULL;
// }
// //����ָ����ŵ��ӱ�����pGoods
// void CPlayer::SetSubPGoods(WORD index,CGoods* pGoods)
// {
// 	if (index>=0&&index<5&&m_Cote[index]!=NULL)
// 	{
// 		m_Cote[index]->pGoods = pGoods;
// 		if(pGoods)
// 		{
// 			SetContainSubPack(true);
// 			return;
// 		}
// 		else
// 		{
// 			SetContainSubPack(false);
// 			/*
// 			selfע:�����Ƚϱ�,���Ľ�
// 			*/
// 			for(int i=0;i<5;i++)
// 			{
// 				if(m_Cote[i] != NULL && m_Cote[i]->pGoods != NULL )
// 					return;
// 			}
// 		}
// 	}
// }
// 
// //���ҵ�һ�����ID���ڴ����ID����Ʒ
// CGoods* CPlayer::FindFirstGoodsByIndex(DWORD dwIndex,int& lExtendID,int& pPos)
// {
// 	//CGoods *pGoods=NULL;
// 
// 	//	if(m_stHand.pGoods && m_stHand.pGoods->GetIndex()==dwIndex)
// 	//		return m_stHand.pGoods;
// 
// 	//�ڱ����в���
// 	list<tagGoods>* pGoodList = GetGoodsList();
// 	if (pGoodList)
// 	{
// 		list<tagGoods>::iterator GoodsIter=pGoodList->begin();
// 		for(;GoodsIter!=pGoodList->end();GoodsIter++)
// 		{
// 			CGoods* pGoods = GoodsIter->pGoods;
// 			if(pGoods && pGoods->GetIndex()==dwIndex)
// 			{
// 				lExtendID = GetGoodsContainerID(pGoods);
// 				pPos = GoodsIter->lPos;
// 				return pGoods;
// 			}
// 		}
// 	}
// 	//��ѯ�ӱ���
// 	for (int i = 0;i<COTE_NUM;i++)
// 	{	
// 		//���ӱ����в���
// 		pGoodList = GetSubGoodslist(i);
// 		if (pGoodList)
// 		{
// 			list<tagGoods>::iterator GoodsIter=pGoodList->begin();
// 			for(;GoodsIter!=pGoodList->end();GoodsIter++)
// 			{
// 				CGoods* pGoods = GoodsIter->pGoods;
// 				if(pGoods && pGoods->GetIndex()==dwIndex)
// 				{
// 					lExtendID = GetGoodsContainerID(pGoods);
// 					pPos = GoodsIter->lPos;
// 					return pGoods;
// 				}
// 			}
// 		}
// 	}
// 	return NULL;
// }
// //===========================================================================================================
// //2.22 Square
// //�ֿ����*******************************************************************************************
// 

//�ӱ���

// 
// bool CPlayer::DecordPixieFromDataBlock(DBReadSet& dbr)
// {
// 	long lGoodsNum = dbr.GetLongFromByteArray();
// 	m_Pixie.lItemGoodsNum = lGoodsNum;
// 	long * pItemList = new long[lGoodsNum];
// 	DWORD* pItemUse = new DWORD[lGoodsNum];
// 	int j = 0;
// 	for(int i=0;i<lGoodsNum;i++)
// 	{
// 		pItemList[i] = dbr.GetLongFromByteArray();
// 		pItemUse[i] = dbr.GetLongFromByteArray();
// 	}
// 	tagAutoSupplyData mAutoData[AutoSupplyTypeNum];
// 	dbr.GetBufferFromByteArray(mAutoData,sizeof(tagAutoSupplyData)*AutoSupplyTypeNum);
// 	for(int i=0;i<AutoSupplyTypeNum;i++)
// 	{
// 		m_AutoSupplySetup[i].lGoodsIndex = mAutoData[i].lGoodsIndex;
// 		m_AutoSupplySetup[i].lSpringScale = mAutoData[i].lSpringScale;
// 		m_AutoSupplySetup[i].bActive = mAutoData[i].bActive;
// 		//
// 		m_AutoSupplySetupEx[i].lGoodsIndex = m_AutoSupplySetup[i].lGoodsIndex;
// 		m_AutoSupplySetupEx[i].lSpringScale= m_AutoSupplySetup[i].lSpringScale;
// 		m_AutoSupplySetupEx[i].bActive = m_AutoSupplySetup[i].bActive;
// 	}
// 	SAFE_DELETE_ARRAY(pItemList);
// 	SAFE_DELETE_ARRAY(pItemUse);
// 	return true;
// }

// void CPlayer::ClearAllGoods(long lSocketID)
// {
// 	CGameClient* pClient = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(lSocketID);
// 	if( !pClient) return ;
// 	CPlayer* pPlayer = pClient->GetMainPlayer();
// 	if( pPlayer == NULL || this != pPlayer )
// 		return;
// 	//ԭʼ����
// 
// 	//���������ﵽ���ֵʱ���˳�ѭ������ΪҪ����1���ڷ�����Ϣ�Ĵ��� by MartySa 2009.7.8]
// 	int nBreakCount = 0;
// 	//
// 	if( !m_listMainPackGoods.empty())
// 	{
// 		list<tagGoods>::iterator it =  m_listMainPackGoods.begin();
// 		for( ;it != m_listMainPackGoods.end(); ++it)
// 		{
// 			++nBreakCount;
// 			static DWORD delgoods_times = 0;
// #ifdef _DEBUG
// 			char szOut[128];
// 			sprintf(szOut,"delgoods_times:%d\n",++delgoods_times);
// 			OutputDebugString(szOut);
// #endif // _DEBUG
// 			CGoods* pGoods = it->pGoods;
// 			char str[64];
// 			sprintf(str,"DelGoods(\"%s\",%d);",pGoods->GetOrginName(),it->lNum);
// 
// 			//CBaseMessage* msg = CBaseMessage::AllocMsg();
// 			//msg->Reset();
// 			//msg->Init(MSG_C2S_OTHER_TALK);
// 			//CMessage* pMsg = (CMessage*)msg;
// 
// 			//pMsg->Add((BYTE)0);
// 			//pMsg->Add((long)RUN_SCRIPT);
// 			//pMsg->Add((long)TYPE_PLAYER);
// 			//pMsg->Add(pPlayer->GetExID());
// 			//pMsg->Add((long)0);
// 			//pMsg->Add(str);
// 			//pMsg->SendBySocket(lSocketID);
// 			//CBaseMessage::FreeMsg(pMsg);
// 			//pMsg->Reset();
// 
// 			//ע��&�޸�By MartySa  2009,7,8
// 			CMessage *pMsg = (CMessage *)CBaseMessage::AllocMsg();
// 			pMsg->Reset();
// 			pMsg->Init(MSG_C2S_OTHER_TALK);
// 			pMsg->Add((BYTE)0);
// 			pMsg->Add((long)RUN_SCRIPT);
// 			pMsg->Add((long)TYPE_PLAYER);
// 			pMsg->Add(pPlayer->GetExID());
// 			pMsg->Add((long)0);
// 			pMsg->Add(str);
// 			pMsg->SendBySocket(lSocketID);
// 			CBaseMessage::FreeMsg(pMsg);
// 
// 			if ( nBreakCount >= 3 )
// 			{
// 				return ;
// 			}
// 		}
// 	}
// 	//�ӱ���
// 	for (int i=0;i<COTE_NUM;i++)
// 	{
// 		if( m_Cote[i] != NULL )
// 		{
// 			list<tagGoods>::iterator it = m_Cote[i]->m_listCoteGoods.begin();
// 			for(; it!= m_Cote[i]->m_listCoteGoods.end();++it)
// 			{
// 				//
// 				++nBreakCount;
// 				//
// 
// 				CGoods* pGoods = it->pGoods;
// 				char str[64];
// 				sprintf(str,"DelGoods(\"%s\",%d);",pGoods->GetOrginName(),it->lNum);
// 				//CBaseMessage* msg = CBaseMessage::AllocMsg();
// 				//msg->Reset();
// 				//msg->Init(MSG_C2S_OTHER_TALK);
// 				//CMessage* pMsg = (CMessage*)msg;
// 
// 				//pMsg->Add((BYTE)0);
// 				//pMsg->Add((long)RUN_SCRIPT);
// 				//pMsg->Add((long)TYPE_PLAYER);
// 				//pMsg->Add(pPlayer->GetExID());
// 				//pMsg->Add((long)0);
// 				//pMsg->Add(str);
// 				//pMsg->SendBySocket(lSocketID);
// 				//CBaseMessage::FreeMsg(pMsg);
// 				//pMsg->Reset();
// 
// 				//ע��&�޸�By MartySa  2009,7,8
// 				CMessage *pMsg = (CMessage *)CBaseMessage::AllocMsg();
// 				pMsg->Reset();
// 				pMsg->Init(MSG_C2S_OTHER_TALK);
// 				pMsg->Add((BYTE)0);
// 				pMsg->Add((long)RUN_SCRIPT);
// 				pMsg->Add((long)TYPE_PLAYER);
// 				pMsg->Add(pPlayer->GetExID());
// 				pMsg->Add((long)0);
// 				pMsg->Add(str);
// 				pMsg->SendBySocket(lSocketID);
// 				CBaseMessage::FreeMsg(pMsg);
// 
// 				if ( nBreakCount >= 3 )
// 				{
// 					break;
// 				}
// 			}
// 		}
// 	}
// }


// ���ݴ������ƷID��Ҫ���ӵ���������һ���ѵ������ᳬ���ѵ����޲���δ����������Ʒ
// CGoods* CPlayer::FindOverLapGoodsByIndex(DWORD dwIndex,DWORD dwGoodsNum,int& lExtendID,int& pPos)
// {
// 	CGoods* pGoods = NULL;
// 	//�����в���
// 	list<tagGoods>* pGoodsList = GetGoodsList();
// 	if(pGoodsList)
// 	{
// 		list<tagGoods>::iterator goodsIter = pGoodsList->begin();
// 		for(;goodsIter != pGoodsList->end();goodsIter++)
// 		{
// 			pGoods = goodsIter->pGoods;
// 			if( pGoods!=NULL && pGoods->GetIndex()==dwIndex &&
// 				(pGoods->GetNum()+dwGoodsNum)<=pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1)&&!pGoods->IsLocked())
// 			{
// 				lExtendID = GetGoodsContainerID(pGoods);
// 				pPos = goodsIter->lPos;
// 				return pGoods;
// 			}
// 		}
// 	}
// 	//�ֱ���
// 	for(int i=0;i<COTE_NUM;i++)
// 	{
// 		pGoodsList = GetSubGoodslist(i);
// 		if(pGoodsList)
// 		{
// 			list<tagGoods>::iterator goodsIter = pGoodsList->begin();
// 			for(;goodsIter!=pGoodsList->end();goodsIter++)
// 			{
// 				pGoods = goodsIter->pGoods;
// 				if(pGoods!=NULL&&pGoods->GetIndex()==dwIndex&&
// 					(pGoods->GetNum()+dwGoodsNum)<=pGoods->GetEffectVal(CGoods::GAP_MAXOVERLAP,1)&&
// 					!pGoods->IsLocked())
// 				{
// 					lExtendID = GetGoodsContainerID(pGoods);
// 					pPos = goodsIter->lPos;
// 					return pGoods;
// 				}
// 			}
// 		}
// 	}
// 
// 	return NULL;
// }




// DWORD CPlayer::GetBuyGoodsID()
// {
// 	srand(GetTickCount());
// 	//m_RandomNum = ();
// 	return m_ShopGoodsID.at(/*m_RandomNum*/rand()%m_dwGoodsNum);
// }

//����
//����panledata
// 
// //------------------------------------------------------------------------------------------------
// //------------------------------------------------------------------------------------------------
// 
// void CPlayer::SetLinkManPanleData(tagLinkman *data, int spanelid)
// {	
// 	if ( (NULL == data) || (spanelid < 0) || (spanelid >= MAXNUM_PANELS))
// 		return ;
// 	m_PanelGroup[spanelid].m_PanleMemberData.push_back(*data);
// 	//��������
// 	m_PanelGroup[spanelid].number++;				
// }
// 
// void CPlayer::DelteLinkManPanleData(int spanelid, const string &strName)
// {	
// 	if ( (strName.empty()) || (spanelid < 0) || (spanelid >= MAXNUM_PANELS))
// 		return ;
// 	list<tagLinkman>::iterator it = m_PanelGroup[spanelid].m_PanleMemberData.begin();
// 	for ( ; it != m_PanelGroup[spanelid].m_PanleMemberData.end(); ++it)
// 	{
// 		if (strName.compare(it->szName) == 0)
// 		{
// 			m_PanelGroup[spanelid].m_PanleMemberData.erase(it);
// 			m_PanelGroup[spanelid].number--;
// 			return ;
// 		}
// 	}
// }
// 
// tagLinkman *CPlayer::GetLinkManPanleDataForName(int spanelid, const string &strName)
// {	
// 	if ((strName.empty()) || (spanelid < 0) || (spanelid >= MAXNUM_PANELS))
// 		return NULL;
// 
// 	list<tagLinkman>::iterator it = m_PanelGroup[spanelid].m_PanleMemberData.begin();
// 	for ( ; it != m_PanelGroup[spanelid].m_PanleMemberData.end(); ++it)
// 	{
// 		if (strName.compare(it->szName) == 0)
// 		{	
// 			return &(*it);
// 		}
// 	}
// 	return NULL;
// }
// 
// bool CPlayer::isLinkManRepeatForName(const string &strName)
// {	
// 	if(strName.empty())
// 		return false;
// 
// 	for (int i=0; i!=MAXNUM_PANELS; ++i)
// 	{
// 		list<tagLinkman>::iterator it = m_PanelGroup[i].m_PanleMemberData.begin();
// 		for ( ; it != m_PanelGroup[i].m_PanleMemberData.end(); ++it)
// 		{
// 			if (strName.compare(it->szName) == 0)
// 			{	
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }
// 
// void CPlayer::ClearLinkmanData()
// {
// 	for (int i=0; i!=MAXNUM_PANELS; ++i)
// 	{
// 		m_PanelGroup[i].m_PanleMemberData.clear();
// 		m_PanelGroup[i].number = 0;	
// 	}
// }
// 
// 
// 
// 
// 
// 
// 
// 
// //----------------------------------------------------------------------------------------------------
// //	�¼ӽ���ӿ�
// //----------------------------------------------------------------------------------------------------
void CPlayer::DecordChangePropertyFromDataBlock(DBReadSet& ReadDB)
{
	CMoveShape::DecordChangePropertyFromDataBlock(ReadDB);
	WORD wNum = ReadDB.GetWordFromByteArray();
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
		pro_type = ReadDB.GetWordFromByteArray();
		data_type = ReadDB.GetByteFromByteArray();
		data_length = ReadDB.GetWordFromByteArray();		
		if (data_length<=0) continue;

		//�����������ͻ�ȡ���������ֵ
		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = ReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_WORD:
			data = ReadDB.GetWordFromByteArray();
			break;
		case ATTR_DATA_DWORD:
			data = ReadDB.GetDwordFromByteArray();
			break;
		case ATTR_DATA_FLOAT:
			data = ReadDB.GetFloatFromByteArray();
			break;
		case ATTR_DATA_LONG:
			data = ReadDB.GetLongFromByteArray();
			break;
		case ATTR_DATA_SHORT:
			data = ReadDB.GetShortFromByteArray();
			break;
		case ATTR_DATA_BOOL:
			data = ReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				byte_Array[j] = ReadDB.GetByteFromByteArray();
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				word_Array[j] = ReadDB.GetWordFromByteArray();
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				dword_Array[j] = ReadDB.GetDwordFromByteArray();
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			flaot_Array = new float[data_length];
			if (!flaot_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				flaot_Array[j] = ReadDB.GetFloatFromByteArray();
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				long_Array[j] = ReadDB.GetLongFromByteArray();
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				short_Array[j] = ReadDB.GetShortFromByteArray();
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			ReadDB.GetStringFromByteArray(char_Array,data_length);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int j = 0;j<data_length;j++)
			{
				data = ReadDB.GetByteFromByteArray();
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
		// 		itProETN it = m_PropertyEnumToName.find(pro_type);
		// 		if(it != m_PropertyEnumToName.end())
		// 		{
		// 			SetProperty( (*it).second.c_str(),data);
		// 		}

		SAFE_DELETE_ARRAY(byte_Array);
		SAFE_DELETE_ARRAY(word_Array);
		SAFE_DELETE_ARRAY(dword_Array);
		SAFE_DELETE_ARRAY(flaot_Array);
		SAFE_DELETE_ARRAY(long_Array);
		SAFE_DELETE_ARRAY(short_Array);
		SAFE_DELETE_ARRAY(char_Array);
		SAFE_DELETE_ARRAY(bool_Array);
	}	
}
// 
bool CPlayer::DecordFromDataBlock(DBReadSet& readDB,bool bExData /* = true */)
{
	if (CGameTestDlg::m_iProjName == CGameTestDlg::PROJ_XINGCHEN)
	{
		//��ʼ������Mgr
		m_pSkillMgr->ClearSkillList();

		//��ʼ����ƷMgr
		m_pGoodsMgr->Release();


		CMoveShape::DecordFromDataBlock(readDB,bExData);

		if(bExData)
		{
			m_bSelf = true;

			readDB.GetBufferFromByteArray(&m_Property,(long)sizeof(tagPPro));

			DecordChangePropertyFromDataBlock(readDB);

			// ħ������ [11/3/2009 Square]
			readDB.GetBufferFromByteArray(m_arrElementProperty,sizeof(tagPlayerElementProperty)*MT_Max);

			//	ʱװ��
			readDB.GetByteFromByteArray();

			//	����
			m_pSkillMgr->DecodeFromDataBlock(readDB);

			//�������ҵϵͳ�е�����
			long lNum = readDB.GetDwordFromByteArray();
			for(int i=0;i<lNum;++i)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			// ������Ʒ��������ȴ���͵���ȴʱ��
			lNum = readDB.GetDwordFromByteArray();
			for (int i=0;i<lNum;i++)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			// ���뵥����Ʒ��ȴʱ��
			lNum = readDB.GetDwordFromByteArray();
			for (int i=0;i<lNum;i++)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			bool bVal = false;
			//װ��
			m_pGoodsMgr->GetEquipMgr()->DecodeFromDataBlock(readDB,bExData);

			//ѫ��
			m_pMedalMgr->DecodeFromDataBlock(readDB);

			long lTest = readDB.GetLongFromByteArray();

			//ԭʼ����
			GetGoodsMgr()->GetPacketMgr()->DecodeFromDataBlock(readDB,bExData);

			lTest = readDB.GetLongFromByteArray();
			// ���
			lNum = readDB.GetLongFromByteArray();
			CGUID guid;
			readDB.GetBufferFromByteArray(guid);
			SetPlayerMoneyNum(lNum);
			SetPlayerMoneyGuid(guid);
			//����
			lNum = readDB.GetLongFromByteArray();
			readDB.GetBufferFromByteArray(guid);
			SetPlayerSilverMoneyNum(lNum);
			SetPlayerSilverMoneyGuid(guid);
			//��ʯ
			lNum = readDB.GetLongFromByteArray();
			readDB.GetBufferFromByteArray(guid);
			SetPlayerBaoShiNum(lNum);
			SetPlayerBaoShiGuid(guid);

			//ticket
			lNum = readDB.GetLongFromByteArray();

			//�ֿ���Ϣͷ
			m_pGoodsMgr->GetDepotMgr()->DecordDepotHeadInfo(readDB);
			//��֯ϵͳ
			m_pOrganiseSys->DecodeFromDataBlock(readDB,true);
			//��ǰʹ�õĳƺ�
			long lCurTitleType = readDB.GetLongFromByteArray();
			char strTitleName[1024];
			string strTitle = readDB.GetStringFromByteArray(strTitleName,1024);

			//GM����
			BYTE bMConcealment = readDB.GetByteFromByteArray();
			//С����
			m_pSpriteMgr->DecodeFromDataBlock(readDB);


			//����
			m_pTaskMgr->DecodeFromDataBlock(readDB);

			//��ְҵϵͳ
			GetCollectMgr()->DecodeFromDataBlock(readDB);

			long lLev = readDB.GetLongFromByteArray();
			long lID = readDB.GetLongFromByteArray();

			//	��ļ��Ϣ
			m_pTeamMgr->DecodeFromDataBlock(readDB);

			//�������� 2009/1/06 nikai
			m_pPetMgr->DecodeFromDataBlock(readDB,bExData);

			//	����
			m_pGoodsMgr->GetPacketMgr()->DecodeBusinessFromDataBlock(readDB,bExData);

			bool b = readDB.GetByteFromByteArray() == 0 ? false : true;

			DecodeHotKeyData(readDB);
		}
		else
		{
			m_bSelf = false;
			// װ�����
			readDB.GetByteFromByteArray();
			readDB.GetByteFromByteArray();
			readDB.GetByteFromByteArray();

			m_pGoodsMgr->GetEquipMgr()->DecodeFromDataBlockEx(readDB);


			// �Ƿ���ʾ���ұ�־
			readDB.GetByteFromByteArray();

			SetHp(readDB.GetDwordFromByteArray());
			SetMaxHp(readDB.GetDwordFromByteArray());
			//--------07.8.3ȡ��ע��(����)---------
			SetPkValue( readDB.GetDwordFromByteArray() );
			SetPVPValue( readDB.GetDwordFromByteArray() );

			//��֯ϵͳ
			m_pOrganiseSys->DecodeFromDataBlock(readDB,false);
			//		DecordOrgSysFromDataBlock(readDB);

			//�ƺ�ϵͳ
			long lTitleType = readDB.GetLongFromByteArray();
			//		SetTitleType(lTitleType);
			char strTitleName[1024];
			string strTitlle = readDB.GetStringFromByteArray(strTitleName,1024);
			//		SetPlayerTitle(strTitlle); 

			//GM����
			BYTE bMConcealment = readDB.GetByteFromByteArray();

			//ְҵ+�Ա�
			SetOccupation(readDB.GetByteFromByteArray());
			SetSex(readDB.GetByteFromByteArray());
			SetAssOccu(readDB.GetByteFromByteArray());

			//
			m_pShop->DecodeFromDataBlock(readDB);

			DWORD dwEmotionIndex=readDB.GetDwordFromByteArray();
			DWORD dwTime=readDB.GetDwordFromByteArray();

			// ����ID
			SetCountry( readDB.GetByteFromByteArray());
			SetLevel(readDB.GetWordFromByteArray());
			// ��ļ��Ϣ
			m_pTeamMgr->DecodeFromDataBlock(readDB);

			//���CPֵ
			long lCurPKCP = readDB.GetLongFromByteArray();
			long lMaxPKCP = readDB.GetLongFromByteArray();

			m_pPetMgr->DecodeFromDataBlock(readDB,bExData);
		}
	}
	else if (CGameTestDlg::m_iProjName == CGameTestDlg::PROJ_PANLONG)
	{
		//��ʼ������Mgr
		m_pSkillMgr->ClearSkillList();

		//��ʼ����ƷMgr
		m_pGoodsMgr->Release();

		CMoveShape::DecordFromDataBlock(readDB,bExData);

		if(bExData)
		{
			m_bSelf = true;

			readDB.GetBufferFromByteArray(&m_Property,(long)sizeof(tagPPro));

			DecordChangePropertyFromDataBlock(readDB);

			// ħ������ [11/3/2009 Square]
			readDB.GetBufferFromByteArray(m_arrElementProperty,sizeof(tagPlayerElementProperty)*MT_Max);

			//	ʱװ��
			readDB.GetByteFromByteArray();

			//	����
			m_pSkillMgr->DecodeFromDataBlock(readDB);


#define _DECODE_GENIUS_

#ifdef  _DECODE_GENIUS_
			/// �����츳�������
			DecodeGeniusFromDataBlock(readDB);
#endif
			// KL �������
			long size = readDB.GetLongFromByteArray();
			for( long i = 0; i < size; ++ i )
			{
				CGUID guid;
				readDB.GetBufferFromByteArray( guid );
				readDB.GetLongFromByteArray();
			}

			//�������ҵϵͳ�е�����
			long lNum = readDB.GetDwordFromByteArray();
			for(int i=0;i<lNum;++i)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			// ������Ʒ��������ȴ���͵���ȴʱ��
			lNum = readDB.GetDwordFromByteArray();
			for (int i=0;i<lNum;i++)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			// ���뵥����Ʒ��ȴʱ��
			lNum = readDB.GetDwordFromByteArray();
			for (int i=0;i<lNum;i++)
			{
				readDB.GetDwordFromByteArray();
				readDB.GetDwordFromByteArray();
			}

			bool bVal = false;
			//װ��
			m_pGoodsMgr->GetEquipMgr()->DecodeFromDataBlock(readDB,bExData);

			// TODO: ����װ�����ڽ���
			lNum = readDB.GetDwordFromByteArray();        
			assert(!lNum);
			lNum = readDB.GetDwordFromByteArray();        
			assert(!lNum);
			lNum = readDB.GetDwordFromByteArray();        
			assert(!lNum);

			//ѫ��
			m_pMedalMgr->DecodeFromDataBlock(readDB);

			long lTest = readDB.GetLongFromByteArray();

			//ԭʼ����
			GetGoodsMgr()->GetPacketMgr()->DecodeFromDataBlock(readDB,bExData);

			lTest = readDB.GetLongFromByteArray();
			// ���
			lNum = readDB.GetLongFromByteArray();
			CGUID guid;
			readDB.GetBufferFromByteArray(guid);
			SetPlayerMoneyNum(lNum);
			SetPlayerMoneyGuid(guid);
			//����
			lNum = readDB.GetLongFromByteArray();
			readDB.GetBufferFromByteArray(guid);
			SetPlayerSilverMoneyNum(lNum);
			SetPlayerSilverMoneyGuid(guid);
			//��ʯ
			lNum = readDB.GetLongFromByteArray();
			readDB.GetBufferFromByteArray(guid);
			SetPlayerBaoShiNum(lNum);
			SetPlayerBaoShiGuid(guid);

			//ticket
			lNum = readDB.GetLongFromByteArray();

			//�ֿ���Ϣͷ
			m_pGoodsMgr->GetDepotMgr()->DecordDepotHeadInfo(readDB);
			//��֯ϵͳ
			m_pOrganiseSys->DecodeFromDataBlock(readDB,true);
			//��ǰʹ�õĳƺ�
			long lCurTitleType = readDB.GetLongFromByteArray();
			char strTitleName[1024];
			string strTitle = readDB.GetStringFromByteArray(strTitleName,1024);

			//GM����
			BYTE bMConcealment = readDB.GetByteFromByteArray();
			//С����
			m_pSpriteMgr->DecodeFromDataBlock(readDB);

			//����
			m_pTaskMgr->DecodeFromDataBlock(readDB);

			//��ְҵϵͳ
			GetCollectMgr()->DecodeFromDataBlock(readDB);

			long lLev = readDB.GetLongFromByteArray();
			long lID = readDB.GetLongFromByteArray();

			//��ļ��Ϣ
			m_pTeamMgr->DecodeFromDataBlock(readDB);

			lNum = readDB.GetLongFromByteArray();
			assert( lNum == 0 && "Not implemented" );

			//����
			m_pGoodsMgr->GetPacketMgr()->DecodeBusinessFromDataBlock(readDB,bExData);

			bool b = readDB.GetByteFromByteArray() == 0 ? false : true;
		}
		else
		{
			m_bSelf = false;
			// װ�����
			readDB.GetByteFromByteArray();
			readDB.GetByteFromByteArray();
			readDB.GetByteFromByteArray();

			m_pGoodsMgr->GetEquipMgr()->DecodeFromDataBlockEx(readDB);


			// �Ƿ���ʾ���ұ�־
			readDB.GetByteFromByteArray();

			SetHp(readDB.GetDwordFromByteArray());
			SetMaxHp(readDB.GetDwordFromByteArray());
			//--------07.8.3ȡ��ע��(����)---------
			SetPkValue( readDB.GetDwordFromByteArray() );
			SetPVPValue( readDB.GetDwordFromByteArray() );

			//��֯ϵͳ
			m_pOrganiseSys->DecodeFromDataBlock(readDB,false);
			//		DecordOrgSysFromDataBlock(readDB);

			//�ƺ�ϵͳ
			long lTitleType = readDB.GetLongFromByteArray();
			//		SetTitleType(lTitleType);
			char strTitleName[1024];
			string strTitlle = readDB.GetStringFromByteArray(strTitleName,1024);
			//		SetPlayerTitle(strTitlle); 

			//GM����
			BYTE bMConcealment = readDB.GetByteFromByteArray();

			//ְҵ+�Ա�
			SetOccupation(readDB.GetByteFromByteArray());
			SetSex(readDB.GetByteFromByteArray());
			SetAssOccu(readDB.GetByteFromByteArray());

			//
			m_pShop->DecodeFromDataBlock(readDB);

			DWORD dwEmotionIndex=readDB.GetDwordFromByteArray();
			DWORD dwTime=readDB.GetDwordFromByteArray();

			// ����ID
			SetCountry( readDB.GetByteFromByteArray());
			SetLevel(readDB.GetWordFromByteArray());
			// ��ļ��Ϣ
			m_pTeamMgr->DecodeFromDataBlock(readDB);

			//���CPֵ
			long lCurPKCP = readDB.GetLongFromByteArray();
			long lMaxPKCP = readDB.GetLongFromByteArray();
		}
	}
	return true;
}
// 
// 
// void CPlayer::DecordRecruitFromDataBlock(DBReadSet& dbr)
// {
// 	long flag = dbr.GetByteFromByteArray();
// 	if(flag==0)
// 	{
// 		SetRecruitingTeam(false);
// 		return;
// 	}
// 	long lMemNum = dbr.GetByteFromByteArray();
// 	SetTeammateAmount(lMemNum);
// 	char strTeamName[20];
// 	memset(strTeamName,0,sizeof(strTeamName));
// 	dbr.GetStringFromByteArray(strTeamName,20);
// 	SetRecruitingTeam(true);
// 	SetTeamname(strTeamName);
// 	long lHasPass = dbr.GetByteFromByteArray();
// 	SetTeamHasPassword((lHasPass)==0?false:true);
// 	return;
// }
// 
// void CPlayer::DecordOrgSysFromDataBlock(DBReadSet& setReadDB)
// {
// 	// �л������ GUID
// 	setReadDB.GetBufferFromByteArray(m_FamilyGuid);
// 	setReadDB.GetBufferFromByteArray(m_UnionGuid);
// 
// 	if((BOOL)setReadDB.GetByteFromByteArray())
// 	{
// 		LONG lFacDemiseWarning = 0;
// 		LONG lFacDisbandWarning = 0;
// 
// 		if( this->IsSelf() )
// 		{
// 			lFacDemiseWarning = setReadDB.GetLongFromByteArray(); //! ��ᴫλ���ѵ���ʱ
// 			lFacDisbandWarning = setReadDB.GetLongFromByteArray(); //! ����ɢ���ѵ���ʱ
// 		}
// 
// 		// �ƺ�
// 		TCHAR szFactionTitle[ MAX_MEMBER_NAME_SIZE ];
// 		setReadDB.GetStringFromByteArray( szFactionTitle, MAX_MEMBER_NAME_SIZE );
// 
// 		// �л�ͼ��CRC
// 		m_dwFacIconCRC = setReadDB.GetDwordFromByteArray();
// 
// 		// PUBLIC��Ϣ��Ȩ�ޡ��ٽ�
// 		tagFacPublicData stFacPublicData;
// 		ZeroMemory( &stFacPublicData, sizeof(stFacPublicData) );
// 
// 		TCHAR	szPronounce[ MAX_PronounceCharNum ];
// 		ZeroMemory( szPronounce, sizeof(szPronounce) );
// 
// 		long lMyJobPurview = 0;
// 
// 		if( this->IsSelf() )
// 		{
// 			// ��ȡPUBLIC��Ϣ
// 			setReadDB.GetBufferFromByteArray( &stFacPublicData, sizeof(tagFacPublicData) );
// 
// 			// ��ȡ�ٽ�
// 			m_lFamilyJobLevel = setReadDB.GetLongFromByteArray();
// 
// 			// ��ȡȨ������
// 			lMyJobPurview = setReadDB.GetLongFromByteArray();
// 
// 			// ����
// 			setReadDB.GetStringFromByteArray( szPronounce, MAX_PronounceCharNum);
// 		}
// 		else
// 		{
// 			setReadDB.GetStringFromByteArray( stFacPublicData.szName, MAX_MEMBER_NAME_SIZE );
// 		}
// 
// 		// ������ƺͳƺ�
// 		strcpy(m_strFamilyName,stFacPublicData.szName);
// 		strcpy(m_strFamilyTitle,szFactionTitle);
// 
// 		// ����һ���л�
// 		//CFactionManager*	pFactionManager = GetGame()->GetFactionManager();
// 
// 		FamilyInfo	stFacInfo;
// 		ZeroMemory( &stFacInfo, sizeof(stFacInfo) );
// 		stFacInfo.FamilyGuid				 = m_FamilyGuid;
// 		strcpy(stFacInfo.strName,stFacPublicData.szName);
// 		strcpy(stFacInfo.strPlacard,szPronounce);
// 		stFacInfo.arrFunLevel[ FUN_FACTION ] = stFacPublicData.lLevel;
// 		stFacInfo.tMasterLastOnlineTime		 = (time_t)lFacDemiseWarning;
// 		stFacInfo.tLessMemNumTime			 = (time_t)lFacDisbandWarning;
// 
// // 		if( ! GetFamilyInfoByGuid( m_FamilyGuid ) )
// // 			if( AddFamily( &stFacInfo ) )
// // 				if( m_lFamilyJobLevel>0 && m_lFamilyJobLevel <= MAXNUM_JobLvl)
// // 					GetFamilyInfoByGuid( m_FamilyGuid )->arrJobPurview[m_lFamilyJobLevel-1] = lMyJobPurview;		
// 
// 		//// У��ձ�
// 		//if( pFactionManager->CheckFacIconCRC( m_FactionID, m_dwFacIconCRC ) )
// 		//{
// 		//	pFactionManager->SendUpdateFacIconMsg( m_FactionID );
// 		//}
// 	}
// 
// }
// 
// bool CPlayer::AddFamily(FamilyInfo* info)
// {
// 	return true;
// // 	if(info)
// // 	{
// // 		if( (m_hmapFamily.find(info->FamilyGuid) == m_hmapFamily.end()) )
// // 		{
// // 			m_hmapFamily[info->FamilyGuid] = *info;
// // 		}
// // 		return true;
// // 	}
// // 	return false;
// }
// void CPlayer::DelFamilyByGuid(const CGUID& guid)
// {
// 	if ( guid == CGUID::GUID_INVALID ) return ;
// 
// // 	for(hash_map<CGUID,FamilyInfo,hash_guid_compare>::iterator it = m_hmapFamily.begin();it != m_hmapFamily.end();it++)
// // 	{
// // 		if(it->first == guid)
// // 		{
// // 			m_hmapFamily.erase(it);
// // 			return;
// // 		}
// // 	}
// }
// void CPlayer::DecordMedalFromDataBlock(DBReadSet& dbr)
// {
// 	if( /*m_vecMedalIndex.size() > 0*/ !m_vecMedalIndex.empty() )
// 	{
// 		m_vecMedalIndex.clear();
// 	}
// 	long lNum = dbr.GetLongFromByteArray();
// 	for(int i=0;i<lNum;i++)
// 	{
// 		DWORD dwMedalIndex = dbr.GetLongFromByteArray();
// 		m_vecMedalIndex.push_back(dwMedalIndex);
// 	}
// }
// 
// 
// //---------------------------------------------------------
// //	��ҽ��׵Ĳ��ֽӿ�
// //----------------------------------------------------------
// void CPlayer::AddMyTradeGoods(CGoods* pGoods,long lPos,long lSocketID)
// {
// 	if ( pGoods == NULL ) return ;
// 
// 	tagTradePageGoods myGoods;
// 	myGoods.lNum = pGoods->GetNum();
// 	myGoods.lPos = lPos;
// 	myGoods.dwIconID = pGoods->GetProperty()->dwIconId;
// 	myGoods.guid = pGoods->GetExID();
// 
// 	map<long,tagTradePageGoods>::iterator oldGoodsIter = m_mapMyTradeGoodsInfo.find(lPos);
// 	if( oldGoodsIter != m_mapMyTradeGoodsInfo.end() )
// 	{
// 		int pos = -1;
// 		long lExtendID = 0;
// 		CGoods* pOldGoods = GetGoodsByGUID(oldGoodsIter->second.guid,&pos,&lExtendID);
// 		if(pOldGoods!=NULL)
// 		{
// 			if(pOldGoods->IsLocked())
// 			{
// 				pOldGoods->UnLock();
// 			}
// 			m_mapMyTradeGoodsInfo.erase(oldGoodsIter);
// 		}
// 	}
// 	m_mapMyTradeGoodsInfo[lPos] = myGoods;
// 	CGoods* pRealGoods = GetGoodsByGUID(myGoods.guid);
// 	if(pRealGoods != NULL && !pRealGoods->IsLocked())
// 	{
// 		pRealGoods->UnLock();
// 	}
// 	SAFE_DELETE(pGoods);
// 	SetCanTrade(true);
// }
// void CPlayer::AddYouTradeGoods(CGoods* pGoods,long lPos)
// {
// 	if ( pGoods == NULL )  return ;
// 
// 	tagTradePageGoods youGoods;
// 	youGoods.dwIconID = pGoods->GetProperty()->dwIconId;
// 	youGoods.lNum = pGoods->GetNum();
// 	youGoods.lPos = lPos;
// 	youGoods.guid = pGoods->GetExID();
// 	m_mapYouTradeGoodsInfo.insert(pair<long,tagTradePageGoods>(lPos,youGoods));
// 
// 	SAFE_DELETE(pGoods);
// }
// 
// // С���� [12/30/2008 Square]
// void CPlayer::SetAutoSupplySetup(long lIndex)
// {
// 	m_AutoSupplySetup[lIndex].lGoodsIndex = m_AutoSupplySetupEx[lIndex].lGoodsIndex;
// 	m_AutoSupplySetup[lIndex].lSpringScale= m_AutoSupplySetupEx[lIndex].lSpringScale;
// 	m_AutoSupplySetup[lIndex].bActive	  = m_AutoSupplySetupEx[lIndex].bActive;
// }
// 
// long CPlayer::GetItemAmount(long lIndex)
// {
// 	if( m_AutoSupplySetup[lIndex].lGoodsIndex != 0 )
// 	{
// 		//long lAmount = GetGoodsAmount(m_AutoSupplySetup[lIndex].lGoodsIndex);
// 		//return lAmount;
// 		return GetGoodsAmount(m_AutoSupplySetup[lIndex].lGoodsIndex);
// 	}
// 	return 0;
// }
// 
// //����(���)
// // 2009.1.20 by MartySa
// 
// 
// 
// 
// ///��ȡ���ʱ���ָ��λ�õ���Ʒ
// tagGoods* CPlayer::GetBusinessGoods(WORD wPos)
// {
// 	if (wPos>=0&&wPos<=GRIDITEM&&m_CoteBussinesPack!=NULL)
// 	{
// 		list<tagGoods>* pLGoods = &m_CoteBussinesPack->m_listCoteGoods;
// 		if (pLGoods==NULL)
// 		{
// 			return NULL;
// 		}
// 		tagGoodsIter it = pLGoods->begin();
// 		for (;it!=pLGoods->end();it++)
// 		{
// 			if (it->lPos==wPos)
// 			{
// 				return &(*it);
// 			}
// 		}
// 	}
// 	return NULL;
// }
// 
// //ȡ���������goods����
// long CPlayer::GetPlayerPackGoodsSum()
// {	
// 	long lGoodsSum = 0;
// 	//������
// 	if ( !m_listMainPackGoods.empty() )
// 	{	
// 		list<tagGoods>::iterator it = m_listMainPackGoods.begin();
// 		for ( ; it!=m_listMainPackGoods.end(); ++it)
// 		{
// 			if ( (*it).pGoods && it->lNum == 1 )
// 			{
// 				lGoodsSum++;
// 				continue;
// 			}
// 			else if( (*it).pGoods && (it->lNum)>1 )
// 			{
// 				lGoodsSum += it->lNum;
// 			}
// 		}
// 	}
// 
// 	//�ӱ���
// 	for(int i=0; i<COTE_NUM; i++)
// 	{	
// 		if ( m_Cote[i]->pGoods )
// 		{		
// 			list<tagGoods>::iterator iter = m_Cote[i]->m_listCoteGoods.begin();
// 			for ( ; iter!=m_Cote[i]->m_listCoteGoods.end(); ++iter)
// 			{
// 				if ( (*iter).pGoods && iter->lNum == 1 )
// 				{
// 					lGoodsSum++;
// 				}
// 				else if( (*iter).pGoods && (iter->lNum)>1 )
// 				{
// 					lGoodsSum += iter->lNum;
// 				}	
// 			}
// 		}
// 	}
// 
// 	return lGoodsSum;
// }
// 
void CPlayer::QuestGetBouns(long lSocket)
{
 	list<tagGtBounsInfo*>::iterator it = m_listGtBounsInfo.begin();
 	for(;it != m_listGtBounsInfo.end(); ++it )
 	{
 		if(!(*it)->bQuestGet)
 		{
 			(*it)->dwLastTime = timeGetTime();
 
 			CMessage msg(593410);
 
 			DBWriteSet dbWr;
 			msg.GetDBWriteSet(dbWr);
 
 			(*it)->pInfo->EncodeToDataBlock(dbWr);
 
 			msg.SendBySocket(lSocket);
 
 			(*it)->bQuestGet = true;
 			return;
 		}
 		else
 		{
 			if( timeGetTime() - (*it)->dwLastTime > 2000 )
 			{
 				(*it)->bQuestGet = false;
 			}
 		}
 	}
}

void CPlayer::OnGetBounsResult(const CGUID & guid,bool bSucced)
{
	list<tagGtBounsInfo*>::iterator it = m_listGtBounsInfo.begin();
	for(;it != m_listGtBounsInfo.end(); ++it )
	{
		if((*it)->pInfo->guid == guid )
		{
			if( bSucced )
			{
				tagGtBounsInfo* p = *it;
				SAFE_DELETE(p->pInfo);
				SAFE_DELETE(p);
				m_listGtBounsInfo.erase(it);
				return;
			}
			else
			{
				OutputDebugString("Error\n");
			}
		}
	}
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
			//UpdateSkillAttr(skillid,skillLvl,pStr,pValue);
		}
		//���¼���ģ��
		//UpdateSkillXmlList(skillid,skillLvl,vecStrKey,vecValue);
	}
}


bool CPlayer::DecodeHotKeyData(DBReadSet& setReadDB,bool bExData)
{
	//DefaultToCurrent();
	DWORD dwSize = setReadDB.GetDwordFromByteArray();
	if (dwSize!=0)
	{
		for (DWORD i = 0;i<dwSize;i++)
		{
			setReadDB.GetDwordFromByteArray();
			BYTE mapkey;
			setReadDB.GetBufferFromByteArray(&mapkey,(long)20);
		}
	}
	return true;
}
