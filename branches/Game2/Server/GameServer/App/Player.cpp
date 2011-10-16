#include "StdAfx.h"
#include "Player.h"

#include "AntiCheat/CPlayerManager.h"
#include "AntiCheat/CConfiger.h"
#include "BuffSkill/NewSkillFactory.h"
#include "BuffSkill/NewSkillBaseProperties.h"
#include "Country/CountryParam.h"
#include "Container/CTestContainer.h"
#include "Country/Country.h"
#include "Country/CountryHandler.h"
#include "FBusiness/ShopManager.h"
#include "GoodsExManage.h"
#include "Goods/CGoods.h"
#include "Goods/CGoodsFactory.h"
#include "Goods/CSuitruleFactory.h"
#include "Linkman/LinkmanSystem.h"
#include "Listener/CGoodsListListener.h"
#include "Listener/CSeekGoodsListener.h"
#include "Listener/CGoodsParticularPropertyListener.h"
#include "LuaScript/Script.h"
#include "LuaScript/ScriptSys.h"
#include "MessagePackage/CS2CContainerObjectAmountChange.h"
#include "Monster.h"
#include "OrganizingSystem/GameOrganizingCtrl.h"
#include "OrganizingSystem/GSTeam.h"
#include "Pet.h"
#include "PetCtrl.h"
#include "PhaseSystem/PhaseManager.h"
#include "PKSys.h"
#include "PlayerMan/PlayerMan.h"
#include "RgnManager.h"
#include "ServerRegion.h"
#include "Session/CSessionFactory.h"
#include "Session/CSession.h"
#include "Session/CPlug.h"
#include "Session/CTrader.h"
#include "Session/CPersonalShopSeller.h"
#include "StateAI/PlayerAI.h"
#include "StateAI/PetAI.h"
#include "MonsterTaunt/TauntedList.h"
#include "StateAI/AIEventSender.h"
#include "../../../Public/Setup/GoodsSetup.h"
#include "../../../Public/Setup/GlobalSetup.h"
#include "../../../Public/Setup/PlayerList.h"
#include "../../../Public/Setup/GMList.h"
#include "../../../Public/Setup/RegionSetup.h"
#include "../../../Public/Setup/ContributeSetup.h"
#include "../../../Public/Setup/QuestSystem.h"
#include "../../../Public/Setup/ReliveGoodsList.h"
#include "../../../Public/Setup/Emotion.h"
#include "../../../Public/Common/QuestDef.h"
#include "../../../Public/ServerPublic/Server/Common/VarListUtils.h"
#include "../../../Public/ServerPublic/Server/Common/VariableList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///
/// ����ChildObjType_GoodsExTimeOut��ʱ���������͡�
/// ϣ��ͨ��GoodsTimerHandler��δ�����ܳ��ֵĸ�����Ʒ���ϵĶ�ʱ��ͳһ��
/// GoodsTimerHandler��������ͬʱ�䴥�������Ʒ��ʱ��ͳһΪһ����ʱ����
///
struct GoodsExTimerType
{
	enum
	{
		GETT_GUARDIAN_ENERGY,
		// add more...
	};
};

static long lFightPetsCount = 0;
static long lPetsCount = 0;

#pragma warning(disable:4355)

map<eGoodsAddonProperties,string> CPlayer::m_mapFightEnum;
set<string>  CPlayer::m_mapOtherUpdateDirtyAttrData;

CPlayer::CPlayer(void)
: m_lPRgnTimerID(-1)
, m_lTRgnTimerID(-1)
, m_lQueueTimerID(-1)
, m_lRgnKickPlayerTimerID(-1)
, m_CurPRgnNum(0)
, m_CurTRgnNum(0)
, m_bLogin(false)
, m_bIncShopFlag(true)
, m_PlayerSavePartFlag(0)
, m_pFacMemInfo(NULL)
, m_MedalContainer(this)
, m_lCurrTitleType(ePTT_Hide)
, m_pPlayerDepot(this)
, m_pPackage(this)
//, m_pTameTarget(NULL)
, m_bReceiveMail(false)
, m_FocusPet(NULL_GUID)
, m_ActivePet(NULL_GUID)
{	
    m_PhaseRest = ePRest;
    m_GeniusAction = new GeniusAction(this);
    m_GeniusTable.clear();
	b_FirstEnterGame = true;
	////-----------------------------------
	SetType(TYPE_PLAYER);
	SetIsRun(true);

	m_dwMoveValiStartTime=timeGetTime();//�ƶ���֤��ʼ�ļ�ʱ
	m_dwLastRecvMsgTime = m_dwMoveValiStartTime;
	m_fMaxPeriodMoveSpeed = 0.0f;
	m_guMoveValiStartRgnID=NULL_GUID;//��ʼ��֤�ĳ���ID
	m_lMoveValiStartPosX=0;//��ʼ�ƶ���Xλ��
	m_lMoveValiStartPosY=0;//��ʼ�ƶ���Yλ��
	m_fMoveValiStartSpeed = 0.0f;
	m_fMoveValiTotalDistance=0.0f;//�ƶ����ܾ���
	m_fMoveValiRecvMsgCount=0.0f;//�յ����ƶ���Ϣ����

	m_lAICount					= 0;
	m_dwLastRestoreHpTimeStamp	= 0;
	m_dwLastRestoreMpTimeStamp	= 0;
	m_dwLastPingTime			= timeGetTime();
	m_fExpScale = 1.0f;
	m_fOccuExpScale = 1.0f;
	m_fSpScale = 1.0f;
	m_fPresentExpScale = 1.0f;

	m_lTicket=0;
	m_lTradePlugId=0;
	m_TradeSessionId=CGUID::GUID_INVALID;

	m_lLastPickGoodsTime=0;
	for(int i=0;i<24;i++)
	{
		SetHotKey(i,0);
	}
	m_mapGoodsIdCoolDown.clear();
	m_mapClassIdCoolDown.clear();
	ClearPersonalShopGoodsData();

	ReleaseNpcShopSellGoods();	

	m_dwYuanbao=0;    

	memset(&m_AppendProperty, 	0,	(long)sizeof(tagPlayerAppendProperty));
	memset(&m_FightProperty,  	0,	(long)sizeof(tagPlayerFightProperty));
	memset(&m_Property,	    	0,	(long)sizeof(tagPlayerProperty));
	memset(&m_BaseFightProperty,0,	(long)sizeof(tagPlayerFightProperty));
	memset(m_dwServerKey,		0,	sizeof(DWORD) * SERVER_KEY_DWORD_SIZE);
	//��ʼ����������
	//InitProperty();


	/*m_pVariableList = NULL;*/
	//m_TeamID = 0;
	m_bCaptain = false;

	//	m_strDepotPassword="";

	InitNameValueMap();
	//m_bInChangingServer			= false;

	m_lFightStateCount			= 0;
	m_dwSinStateTimeStamp		= 0;
	m_dwContinuousKillAmount	= 0;
	m_dwContinuousKillTimeStamp	= 0;
	m_lSilenceTime				= 0;
	m_lSilenceTimeStamp			= 0;
	m_bGMInvincible				= false;
	m_bGMConcealment			= false;
	m_bAlreadyEnterGame			= false;
	m_bNetTransferStatus		= false;
	m_bReliveSkill				= false;

	m_dwLostTimeStamp = 0;
	m_dwMurdererTimeStamp = 0;
	m_lInvestLimitTimeMark=0;

	m_pAI = new StateAI::PlayerAI( this );

	m_dwDisconnectTimeStamp		= 0;
	m_dwDisconnectDelayTime		= 0;

	m_eProgress					= PROGRESS_NONE;
	m_PersonalShopPlugID		= 0;

	m_lEmotionIndex				= 0;
	m_dwEmotonTimeStamp			= 0;

	m_lFightStateTimerID = -1;
	m_dwIntonateStartTime = 0;
	m_dwIntonateTime	  = 0;
	m_dwActStartTime	  = 0;
	m_dwActTime			  = 0;

	//##��Ӽ�����
	m_cEquipment.AddListener( this );	
    m_cEquipmentGod.AddListener(this);
    m_cEquipmentLand.AddListener(this);
    m_cEquipmentHuman.AddListener(this);

	m_cEquipmentGod.SetTrumpType(TT_GOD);
	m_cEquipmentLand.SetTrumpType(TT_GLAND);
	m_cEquipmentHuman.SetTrumpType(TT_HUMAN);

	m_EffectIdSeed = 0;
	strcpy_s(m_szSessionID, 33, "");
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_YUANBAO,list<DWORD>()));
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_WEIMIAN,list<DWORD>()));
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_TICKET,list<DWORD>()));

	m_mapLimitGoodsRecord.clear();
	m_vecTradeList.clear();
	m_lPKTimerID = -1;
	m_lPvPTimerID = -1;
	m_lHpMpTimerID = -1;
	//m_lEnergyTimerID = -1;
	m_lReliveTimerID = -1;

	//��ʾ�ļ�����
	m_nHintCounter = 0;
	//������ʾ
	m_HelpHints.clear();
	//������ʾ
	m_InterfaceHints.clear();
	m_lReceiveMail.clear();

	//�Ѿ�ѡ��������id
	m_dwElectFactureID=0;

	m_lTotalSkillNum = 0;
	m_dwSkillStartTime = timeGetTime();

	m_lTotalNpcScriptsNum = 0;
	m_dwNpcScriptsStartTIme = timeGetTime();

	m_SpriteSystem.SetOwner(this);

	memset(m_szFactionName, 0, MAXSIZE_FactionName);
	memset(m_szUnionName, 0, MAXSIZE_FactionName);

	m_dwBounsTime = timeGetTime();

    m_mover.Init( this );

    m_tauntList = new TauntedList( this );
}

CPlayer::~CPlayer(void)
{
	ReleaseAllPets();
	m_pPlayerDepot.Release();
	b_FirstEnterGame = false;
	m_GoodsAllPurposeOpt.Release();
	m_lReceiveMail.clear();

    SAFE_DELETE(m_GeniusAction);
    m_GeniusTable.clear();
	delete m_pAI;
	m_pPackage.Release();
	map<long,vector<tagSuitGoods>*>::iterator vecIter=  m_mSuitState.begin();
	for(;vecIter!=m_mSuitState.end();vecIter++)
	{		
		SAFE_DELETE(vecIter->second);
	}
	m_mSuitState.clear();
	ClearPersonalShopGoodsData();
	ReleaseNpcShopSellGoods();

	// ɾ����ʱ��
	map<CGUID, long>::iterator ite = m_GoodsTimerID.begin();
	for (;ite != m_GoodsTimerID.end(); ++ite)
	{
		void* pVar = NULL;
		GameManager::GetInstance()->Cancel(ite->second, (const void**)&pVar);
	}
	m_GoodsTimerID.clear();


	CancelRgnKickPlayerTimer();
	CancelCreatePRgnTimer();
	CancelCreateTRgnTimer();
	CancelQueueTimer();
	//StopAllSkills();
	ClearActiveSkills();
	ClearPassiveSkill();
	ClearAllBuff();

    delete m_tauntList;
}

// [070608 AHC]
bool CPlayer::SetAttribute(std::string& attrName, long value)
{
#ifdef _DEBUG_SKILL
	/*char strDebug[256];
	_snprintf(strDebug, 256, "���״̬��������%sֵ%d\n",attrName.c_str(),value);
	OutputDebugStr(strDebug);*/
	//return true;
#endif
	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if(!bFlag)
	{
		if("Money" == attrName)
		{
			return SetMoney(value,3);
		}
        
        if("Silver" == attrName)
        {
            return SetSilver(value);
        }
		// ���������ݱ��
		if( SetValue(attrName.c_str(), value) != INVILID_VALUE )
		{

			bFlag = true;
		}
	}
	if(bFlag)
		AddAttributeToDirty(attrName);
	return bFlag;
}
// [070608 AHC]
bool CPlayer::SetAttribute(char* attrName, long value)
{
	if(NULL == attrName)
		return false;

#ifdef _DEBUG_SKILL
	/*char strDebug[256];
	_snprintf(strDebug, 256, "���״̬��������%sֵ%d\n",attrName.c_str(),value);
	OutputDebugStr(strDebug);*/
	//return true;
#endif
	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if(!bFlag)
	{
		if( strcmp("Money", attrName) == 0 )
		{
			return SetMoney(value,4);
		}
		// ���������ݱ��
		if( SetValue(attrName, value) != INVILID_VALUE )
		{			
			bFlag =  true;
		}
	}
	if(bFlag)
		AddAttributeToDirty(attrName);
	return bFlag;
}

long  CPlayer::SetStateAttribute(std::string& attrName,long value)
{
    long ret = CMoveShape::SetStateAttribute( attrName , value );
    
    if( ret < 0 )
        SetAttribute(attrName,value);
    return 0;
}

long  CPlayer::ChangeStateAttribute(std::string& attrName,long value)
{
    long ret = CMoveShape::ChangeStateAttribute( attrName,value );

    if ( ret < 0 )
    {
        ChangeAttribute( attrName , value );
    }
    return 0;
}

//��pkֵ/pvpֵ��һ����������һ���ű�
void CPlayer::OnPKOrPVPValueChange(long lOldValue,long lNewValue)
{
	string strPkVaule="PkValue";
	string strPvPValue="PVPValue";
	long lCurrentPkValue = GetAttribute(strPkVaule);
	long lCurrentPvPValue = GetAttribute(strPvPValue);
	if (lCurrentPkValue!=0 && lCurrentPvPValue/lCurrentPkValue==GlobalSetup::GetSetup()->fPkToPvpValue)
	{
		const char* strScript = GlobalSetup::GetSetup()->pszPKToPvPScript;
		if(strScript)
		{
			Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ) );
			GetInst( ScriptSys ).RunScript( env, strScript );
		}
	}
	if( (!(lOldValue == 0 && lNewValue == 0) && 
		!(lOldValue>0 && lOldValue <= 20 && lNewValue > 0 && lNewValue <= 20) &&
		!(lOldValue > 20 && lNewValue > 20 )) 
		|| (lOldValue=0 && (lNewValue>0 && lNewValue<ePvp_First_Level))
		|| (lOldValue<ePvp_First_Level && (lNewValue>=ePvp_First_Level && lNewValue<ePvp_Second_Level))
		|| (lOldValue<ePvp_Second_Level && (lNewValue>=ePvp_Second_Level && lNewValue<ePvp_Third_Level))
		|| (lOldValue<ePvp_Third_Level && (lNewValue>=ePvp_Third_Level)))
	{
		CMessage msg(MSG_S2C_PLAYER_SYNC_PKCOUNT);
		msg.Add(GetExID());
		msg.Add(lCurrentPkValue);
		msg.Add(lCurrentPvPValue);
		msg.SendToAround(this,this);
	}
}

long CPlayer::GetAttribute(std::string attrName)
{
	long value = INVILID_VALUE;

	if(attrName.size() < 2)	return value;

	value = CMoveShape::GetAttribute(attrName);
	if(value != INVILID_VALUE)
		return value;

	if("Money" == attrName)
	{
		return GetMoney();
	}

	if(attrName.size() > 2 && attrName[0] == 'C' && attrName[1] == '_') // ȡ�õ�ǰֵ
	{
		// ȡ�øı�ֵ
		attrName[0] = 'E';
		long evalue = GetValue(attrName.c_str());
		// ȡ�û���ֵ
		attrName[0] = 'B';
		long bvalue = GetValue(attrName.c_str());

		// ȡ�õ�ǰֵ�ĺϷ�ֵ
		map<string, tagPty>::iterator vMoveShapeItr = m_mapShapeNameValue.find(attrName);
		if(vMoveShapeItr == m_mapShapeNameValue.end())
		{
			map<string, tagPty>::iterator vPlayerItr = m_mapNameValue.find(attrName);
			if(vPlayerItr != m_mapNameValue.end())
			{
				tagPty& tagValue = vPlayerItr->second;
				value = GetCurrentValue((char*)vPlayerItr->first.c_str(), tagValue, evalue, bvalue);
			}
		}
		else
		{
			tagPty& tagValue = vMoveShapeItr->second;
			value = GetCurrentValue((char*)vMoveShapeItr->first.c_str(), tagValue, evalue, bvalue);
		}
	}
	else
		value = GetValue(attrName.c_str());



	return value;
}
// ȡ�õ�ǰֵ�ĺϷ�ֵ
long CPlayer::GetCurrentValue(char* valueName, const tagPty& tagValue, long evalue/*�ı�ֵ*/, long tmpValue)
{
	long retValue = evalue+tmpValue;

	//// 9.21.2009 ��CPֵ���ԣ����㹫ʽ��ͬ
	//if( stricmp( valueName, "E_MaxPKCP" ) == 0 ||
	//	stricmp( valueName, "B_MaxPKCP" ) == 0 )
	//{
	//	// (����CPֵ+�仯CPֵ)*(1+װ��CP�����ܺ�/100)
	//	retValue = (long)( retValue * ( 1 + GetAllEquipTimesCP() / 100.0f ) );
	//}

	if( retValue > tagValue.maxValue )
		retValue = tagValue.maxValue;

	if( retValue < tagValue.minValue )
		retValue = tagValue.minValue;

	if( retValue < 0 )
		retValue = 0;

	/*if (!stricmp(valueName,"E_MaxHp"))
	{
		retValue =  GetMaxHP();
	}
	if (!stricmp(valueName,"E_MaxMp"))
	{
		retValue =  GetMaxMP();
	}*/

	if (!stricmp(valueName,"Hp"))
	{
		string str="C_MaxHp";
		long lMaxHp = GetMaxHP();
		if (retValue>=lMaxHp)
		{
			retValue = lMaxHp;
		}
	} 
	else if (!stricmp(valueName,"Mp"))
	{
		string str="C_MaxMp";
		long lMaxMp = GetMaxMP();
		if (retValue>=lMaxMp)
		{
			retValue = lMaxMp;
		}
	}	

	return retValue;
}
//�����������ֵ
void	CPlayer::SetBaseMaxEnergy(WORD l)					
{
	m_BaseFightProperty.wMaxEnergy = l; 
	if(GetEnergy()>GetMaxEnergy()) 
		SetEnergy(GetMaxEnergy());
}
bool  CPlayer::ChangeAttribute(std::string& attrName, long value)
{
	bool bFlag = CMoveShape::ChangeAttribute(attrName, value);
	//if(!bFlag)
	//{
	//	// ���������ݱ��
	//	if("Money" == attrName)
	//	{
	//		SetMoney(GetMoney()+value);
	//		return true;
	//	}
	//	else if (("Hp"==attrName && value>0) && !GetAddHp())
	//	{
	//		return true;
	//	}
	//	long curValue = 0;
	//	long baseValue = 0;
	//	mapNameValueItr itr = m_mapNameValue.begin();
	//	itr = m_mapNameValue.find(attrName);
	//	if (itr != m_mapNameValue.end())
	//	{
	//		baseValue = GetValue((char*)itr->first.c_str());
	//		// ����ֵ�Ϸ�����
	//		//curValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, value, baseValue);
	//		curValue = baseValue + value;
	//		if(SetValue(attrName.c_str(), curValue) != INVILID_VALUE)	
	//		{
	//			bFlag = true;
	//		}
	//	}
	//}
	if(!bFlag)
	{
		// ���������ݱ��
		if("Money" == attrName)
		{
			SetMoney(GetMoney()+value,5);
			return true;
		}
		else if (("Hp"==attrName && value>0) && !GetAddHp())
		{
			return true;
		}

		long curValue = 0;
		long rightValue = GetValue(attrName.c_str());
		curValue = rightValue + value;
		mapNameValueItr itr = m_mapNameValue.begin();
		itr = m_mapNameValue.find(attrName);
		if (itr != m_mapNameValue.end())
		{
			if(attrName[0] != 'E') // �ı�ֵ����Ϊ����
			{
				if( rightValue + value <  itr->second.minValue ) 
					curValue = itr->second.minValue;
				if( rightValue + value > itr->second.maxValue ) 
					curValue = itr->second.maxValue;
			}

			if (attrName == "Hp")
			{
				string str="C_MaxHp";
				long lMaxHp = GetMaxHP();
				if (curValue>=lMaxHp)
				{
					curValue = lMaxHp;
				}
			} 
			else if (attrName == "Mp")
			{
				string str="C_MaxMp";
				long lMaxMp = GetMaxMP();
				if (curValue>=lMaxMp)
				{
					curValue = lMaxMp;
				}
			}
			else if (attrName == "Energy")
			{
				string str="C_MaxEnergy";
				long lMaxEnergy = GetAttribute(str);
				if (curValue<0)
				{
					curValue = 0;
				}
				else if (curValue>lMaxEnergy)
				{
					curValue = lMaxEnergy;
				}
			}
			// PK CPֵ��Χ��֤
			else if( attrName == "CurPKCP" )
			{
				long lMaxPKCP = GetAttribute( "C_MaxPKCP" );
				if( curValue > lMaxPKCP )
				{
					curValue = lMaxPKCP;
				}
			}

			if(SetValue(attrName.c_str(), curValue) != INVILID_VALUE)
			{
				bFlag = true;
			}
		}
	}
	if(bFlag)
		AddAttributeToDirty(attrName);
	return bFlag;
}

void CPlayer::AddFightPropertyTODirty()
{
	m_mapDirtyAttrData.insert("C_MaxHp");
	m_mapDirtyAttrData.insert("C_MaxMp");
	m_mapDirtyAttrData.insert("C_MaxStamina");
	m_mapDirtyAttrData.insert("C_MaxEnergy");
	m_mapDirtyAttrData.insert("C_HpRecoverSpeed");
	m_mapDirtyAttrData.insert("C_MpRecoverSpeed");
	m_mapDirtyAttrData.insert("C_StamRecoverSpeed");
    m_mapDirtyAttrData.insert("C_EnerRecoverSpeed");
    m_mapDirtyAttrData.insert("C_FightHpRecoverSpeed");
    m_mapDirtyAttrData.insert("C_FightMpRecoverSpeed");
    m_mapDirtyAttrData.insert("C_FightEnerRecoverSpeed");
    m_mapDirtyAttrData.insert("C_FightStamRecoverSpeed");

    m_mapDirtyAttrData.insert("C_Manly");
    m_mapDirtyAttrData.insert("C_PerverseTrend");
    m_mapDirtyAttrData.insert("C_Complaint");
    m_mapDirtyAttrData.insert("C_MurderousLook");

	m_mapDirtyAttrData.insert("C_Str");
	m_mapDirtyAttrData.insert("C_Dex");
	m_mapDirtyAttrData.insert("C_Con");
	m_mapDirtyAttrData.insert("C_Intellect");
	m_mapDirtyAttrData.insert("C_Spiritualism");
    m_mapDirtyAttrData.insert("C_Point");

    m_mapDirtyAttrData.insert("C_Parry");
    m_mapDirtyAttrData.insert("C_Dodge");
    m_mapDirtyAttrData.insert("C_Def");
    m_mapDirtyAttrData.insert("C_DefSuck");
    m_mapDirtyAttrData.insert("C_Mdef");
    m_mapDirtyAttrData.insert("C_MdefSuck");
    m_mapDirtyAttrData.insert("C_BloodSuck");
    m_mapDirtyAttrData.insert("C_ManaSuck");
    m_mapDirtyAttrData.insert("C_StaminaSuck");

    m_mapDirtyAttrData.insert("C_Hit");
    m_mapDirtyAttrData.insert("C_AtkSpeed");
    m_mapDirtyAttrData.insert("C_MAtkSpeed");
    m_mapDirtyAttrData.insert("C_MinAtk");
    m_mapDirtyAttrData.insert("C_MaxAtk");
    m_mapDirtyAttrData.insert("C_MinMAtk");
    m_mapDirtyAttrData.insert("C_MaxMAtk");
    m_mapDirtyAttrData.insert("C_PhysicImmunity");
    m_mapDirtyAttrData.insert("C_MagicImmunity");
    m_mapDirtyAttrData.insert("C_MoveSpeed");
    m_mapDirtyAttrData.insert("C_MoveAcceleration");
    m_mapDirtyAttrData.insert("C_MoveRadius");
    m_mapDirtyAttrData.insert("C_TurnSpeed");

    m_mapDirtyAttrData.insert("C_ElemDef0");
    m_mapDirtyAttrData.insert("C_ElemDef1");
    m_mapDirtyAttrData.insert("C_ElemDef2");
    m_mapDirtyAttrData.insert("C_ElemDef3");
    m_mapDirtyAttrData.insert("C_ElemDef4");
    m_mapDirtyAttrData.insert("C_ElemDef5");

    m_mapDirtyAttrData.insert("C_DeBuffDef0");
    m_mapDirtyAttrData.insert("C_DeBuffDef1");
    m_mapDirtyAttrData.insert("C_DeBuffDef2");
    m_mapDirtyAttrData.insert("C_DeBuffDef3");
    m_mapDirtyAttrData.insert("C_DeBuffDef4");
    m_mapDirtyAttrData.insert("C_DeBuffDef5");

    m_mapDirtyAttrData.insert("C_ClassDam0");
    m_mapDirtyAttrData.insert("C_ClassDam1");
    m_mapDirtyAttrData.insert("C_ClassDam2");
    m_mapDirtyAttrData.insert("C_ClassDam3");
    m_mapDirtyAttrData.insert("C_ClassDam4");
    m_mapDirtyAttrData.insert("C_ClassDam5");
    m_mapDirtyAttrData.insert("C_ClassDam6");

	m_mapDirtyAttrData.insert("C_Block");
	m_mapDirtyAttrData.insert("C_Charm");
	m_mapDirtyAttrData.insert("C_Absorb");
	m_mapDirtyAttrData.insert("C_Cri");
	m_mapDirtyAttrData.insert("C_BuffPower");
	m_mapDirtyAttrData.insert("C_MCri");
	m_mapDirtyAttrData.insert("C_CriDef");
	m_mapDirtyAttrData.insert("C_MCriDef");
	m_mapDirtyAttrData.insert("C_WillDef");
	m_mapDirtyAttrData.insert("C_ConstiDef");	
	m_mapDirtyAttrData.insert("C_ElemDam0");
	m_mapDirtyAttrData.insert("C_ElemDam1");
	m_mapDirtyAttrData.insert("C_ElemDam2");
	m_mapDirtyAttrData.insert("C_ElemDam3");
	m_mapDirtyAttrData.insert("C_ElemDam4");
	m_mapDirtyAttrData.insert("C_ElemDam5");
	m_mapDirtyAttrData.insert("C_ElemAtk0");
	m_mapDirtyAttrData.insert("C_ElemAtk1");
	m_mapDirtyAttrData.insert("C_ElemAtk2");
	m_mapDirtyAttrData.insert("C_ElemAtk3");
	m_mapDirtyAttrData.insert("C_ElemAtk4");
	m_mapDirtyAttrData.insert("C_ElemAtk5");
	m_mapDirtyAttrData.insert( "C_MaxPKCP" );
}

//��ӱ�������������
void  CPlayer::AddAttributeToDirty(string attrName)
{
	if(attrName == "")	return;
	if( (attrName[0] == 'E' && attrName[1] == '_')
		|| (attrName[0] == 'B' && attrName[1] == '_') )
		attrName[0] = 'C';
	m_mapDirtyAttrData.insert(attrName);
	InsertOtherDirtyName(attrName);

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		long lChangeType = 0;
		LPCSTR strName = attrName.c_str();

		if (0 == strcmp(strName, "B_MaxHp") || 0 == strcmp(strName, "E_MaxHp") || 0 == strcmp(strName, "C_MaxHp"))
		{
			lChangeType = eMIS_MaxHp;
		}
		else if (0 == strcmp(strName, "B_MaxMp") || 0 == strcmp(strName, "E_MaxMp") || 0 == strcmp(strName, "C_MaxMp"))
		{
			lChangeType = eMIS_MaxMp;
		}
		else
		{
			return;
		}
		
		pTeam->OnPlayerIdioinfoChange(GetExID(), (eUpdateIdioinfo)lChangeType);
	}
}

void CPlayer::CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB)
{
	long lShapeNum = 0;
	long lPlayerNum = 0;
	vector<BYTE>	byShapeDirtyData;byShapeDirtyData.clear();
	vector<BYTE>	byDirtyData;byDirtyData.clear();

#ifdef _DEBUG_SKILL
	/*char str[256];
	_snprintf(str, 256, "\n�޸����Ը�����%d\n",ShapeAttrNum+PlayerAttrNum);
	if((ShapeAttrNum+PlayerAttrNum)>200)
	{
	long x = 0;
	}
	OutputDebugString(str);*/
#endif
	set<string>::iterator sStrItr = m_mapDirtyAttrData.begin();
	for(; sStrItr != m_mapDirtyAttrData.end(); sStrItr++)
	{
		mapShapeNameValueItr sitr = m_mapShapeNameValue.find(*sStrItr);
		if (sitr != m_mapShapeNameValue.end())
		{
			lShapeNum++;
			// ��������
			_AddToByteArray(&byShapeDirtyData, (WORD)sitr->second.attrEnumValue);
			switch (sitr->second.lType)
			{
			case 0:	// byte
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BYTE);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(BYTE));
				_AddToByteArray(&byShapeDirtyData, (BYTE)*(BYTE*)(sitr->second.plValue));
				break;
			case 1:	// word
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_WORD);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(WORD));
				_AddToByteArray(&byShapeDirtyData, (WORD)*(WORD*)(sitr->second.pwValue));
				break;
			case 2:	// dword
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_DWORD);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(DWORD));
				_AddToByteArray(&byShapeDirtyData, (DWORD)*(DWORD*)(sitr->second.pdwValue));
				break;
			case 3:	// float
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_FLOAT);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(float));
				_AddToByteArray(&byShapeDirtyData, (float)*(float*)(sitr->second.pfValue));
				break;
			case 4:	// long
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_LONG);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(long));
				_AddToByteArray(&byShapeDirtyData, (long)*(long*)(sitr->second.pValue));
				break;
			case 5:	// short
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_SHORT);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(short));
				_AddToByteArray(&byShapeDirtyData, (short)*(short*)(sitr->second.psValue));
				break;
			case 6:	// bool
				_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BOOL);
				_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(bool));
				_AddToByteArray(&byShapeDirtyData, (char)*(bool*)(sitr->second.pbValue));
				break;
			default:
				break;
			}
		}


		long baseValue = 0;
		string  name = *sStrItr;
		if(name[0] == 'C' && name[1] == '_')
		{	
			// ���ҳ�����ֵ
			name[0] = 'B';
			baseValue = GetValue(name.c_str());
			// ���ҳ��ı�ֵ
			name[0] = 'E';
		}

		mapNameValueItr itr = m_mapNameValue.find(name);
		if (itr!=m_mapNameValue.end())
		{
			lPlayerNum++;
			// ��������
			_AddToByteArray(&byDirtyData, (WORD)itr->second.attrEnumValue);
			long sendValue = INVILID_VALUE;

			switch (itr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(BYTE));
				if((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(itr->second.plValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(itr->second.plValue);

				::_AddToByteArray(&byDirtyData, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(WORD));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(itr->second.pwValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(itr->second.pwValue);
				::_AddToByteArray(&byDirtyData, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(DWORD));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(itr->second.pdwValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(itr->second.pdwValue);
				::_AddToByteArray(&byDirtyData, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(float));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(itr->second.pfValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(itr->second.pfValue);
				::_AddToByteArray(&byDirtyData, (float)sendValue);
				break;
			case 4:	// long
				//msg.Add((BYTE)ATTR_DATA_LONG);
				//msg.Add((WORD)sizeof(long));
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(long));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(itr->second.pValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(itr->second.pValue);
				::_AddToByteArray(&byDirtyData, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(short));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(itr->second.psValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(itr->second.psValue);
				::_AddToByteArray(&byDirtyData, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(&byDirtyData, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(&byDirtyData, (WORD)sizeof(bool));
				if(((*sStrItr)[0] == 'C' && (*sStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(itr->second.pbValue);
					sendValue = GetCurrentValue((char*)itr->first.c_str(), itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(itr->second.pbValue);
				::_AddToByteArray(&byDirtyData, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}
	}

	// Monster���Ը���
	setWriteDB.AddToByteArray( (WORD)lShapeNum);
	if(byShapeDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byShapeDirtyData[0],(long)byShapeDirtyData.size());
	setWriteDB.AddToByteArray( (WORD)lPlayerNum);
	if(byDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byDirtyData[0],(long)byDirtyData.size());
	m_mapDirtyAttrData.clear();
}
bool CPlayer::CodeOtherUpdateAttrToDataBlock(DBWriteSet& setWriteDB)
{
	vector<BYTE> vecData;vecData.clear();
	vector<BYTE> *bArray = &vecData;
	vector<BYTE> vecPlayerData;vecPlayerData.clear();
	vector<BYTE> *bPlayerArray = &vecPlayerData;
	long lShapeNum =0;
	long lNum =0;	
	mapNameValueItr shapeitr = m_mapShapeNameValue.begin();
	mapNameValueItr playeritr = m_mapNameValue.begin();
	long baseValue = 0;
	string name = "";
	set<string>::iterator pStrItr = m_mapOtherDirtyAttrData.begin();
	for(; pStrItr!= m_mapOtherDirtyAttrData.end(); pStrItr++)
	{
		name = *pStrItr;

		shapeitr = m_mapShapeNameValue.find(name);
		if (shapeitr!=m_mapShapeNameValue.end())
		{
			lShapeNum++;
			// ��������
			_AddToByteArray(bArray, (WORD)shapeitr->second.attrEnumValue);

			long sendValue = INVILID_VALUE;
			switch (shapeitr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(bArray, (WORD)sizeof(BYTE));
				if((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(shapeitr->second.plValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(shapeitr->second.plValue);

				::_AddToByteArray(bArray, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(bArray, (WORD)sizeof(WORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(shapeitr->second.pwValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(shapeitr->second.pwValue);
				::_AddToByteArray(bArray, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bArray, (WORD)sizeof(DWORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(shapeitr->second.pdwValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(shapeitr->second.pdwValue);
				::_AddToByteArray(bArray, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(bArray, (WORD)sizeof(float));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(shapeitr->second.pfValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second,(long) EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(shapeitr->second.pfValue);
				::_AddToByteArray(bArray, (float)sendValue);
				break;
			case 4:	// long
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(bArray, (WORD)sizeof(long));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(shapeitr->second.pValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(shapeitr->second.pValue);
				::_AddToByteArray(bArray, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(bArray, (WORD)sizeof(short));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(shapeitr->second.psValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(shapeitr->second.psValue);
				::_AddToByteArray(bArray, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(bArray, (WORD)sizeof(bool));
				if((name[0] == 'E' && name[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(shapeitr->second.pbValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(shapeitr->second.pbValue);
				::_AddToByteArray(bArray, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}

		if(name[0] == 'C' && name[1] == '_')
		{	
			// ���ҳ�����ֵ
			name[0] = 'B';
			baseValue = GetValue(name.c_str());
			// ���ҳ��ı�ֵ
			name[0] = 'E';
		}
		playeritr = m_mapNameValue.find(name);
		if (playeritr!=m_mapNameValue.end())
		{
			lNum++;
			// ��������
			::_AddToByteArray(bPlayerArray, (WORD)playeritr->second.attrEnumValue);

			if("Money" == name)
			{
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(DWORD));
				::_AddToByteArray(bPlayerArray, GetMoney());
				continue;
			}

			long sendValue = INVILID_VALUE;
			switch (playeritr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(BYTE));
				if((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(playeritr->second.plValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(playeritr->second.plValue);

				::_AddToByteArray(bPlayerArray, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(WORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(playeritr->second.pwValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(playeritr->second.pwValue);
				::_AddToByteArray(bPlayerArray, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(DWORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(playeritr->second.pdwValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(playeritr->second.pdwValue);
				::_AddToByteArray(bPlayerArray, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(float));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(playeritr->second.pfValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(playeritr->second.pfValue);
				::_AddToByteArray(bPlayerArray, (float)sendValue);
				break;
			case 4:	// long
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(long));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(playeritr->second.pValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(playeritr->second.pValue);
				::_AddToByteArray(bPlayerArray, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(short));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(playeritr->second.psValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(playeritr->second.psValue);
				::_AddToByteArray(bPlayerArray, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(bool));
				if((name[0] == 'E' && name[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(playeritr->second.pbValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(playeritr->second.pbValue);
				::_AddToByteArray(bPlayerArray, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}
	}
	if(lShapeNum == 0 && lNum==0)
		return false;

	// shape���Ը���
	setWriteDB.AddToByteArray( (WORD)lShapeNum);
	if(vecData.size() > 0)
		setWriteDB.AddToByteArray(&vecData[0],(long)vecData.size());
	// Player���Ը���
	setWriteDB.AddToByteArray( (WORD)lNum);
	if(vecPlayerData.size() > 0)
		setWriteDB.AddToByteArray(&vecPlayerData[0],(long)vecPlayerData.size());
	return true;
}

bool CPlayer::CodeDirtyAttrToDataBlock(set<string>& dirtyAttr, DBWriteSet& setWriteDB)
{
	vector<BYTE> vecData;vecData.clear();
	vector<BYTE> *bArray = &vecData;
	vector<BYTE> vecPlayerData;vecPlayerData.clear();
	vector<BYTE> *bPlayerArray = &vecPlayerData;
	long lShapeNum =0;
	long lNum =0;	
	mapNameValueItr shapeitr;
	mapNameValueItr playeritr;
	long baseValue = 0;
	string name = "";
	set<string>::iterator pStrItr = dirtyAttr.begin();
	for(; pStrItr!= dirtyAttr.end(); pStrItr++)
	{
		name = *pStrItr;

		shapeitr = m_mapShapeNameValue.find(name);
		if (shapeitr!=m_mapShapeNameValue.end())
		{
			lShapeNum++;
			// ��������
			_AddToByteArray(bArray, (WORD)shapeitr->second.attrEnumValue);

			long sendValue = INVILID_VALUE;
			switch (shapeitr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(bArray, (WORD)sizeof(BYTE));
				if((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(shapeitr->second.plValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(shapeitr->second.plValue);

				::_AddToByteArray(bArray, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(bArray, (WORD)sizeof(WORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(shapeitr->second.pwValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(shapeitr->second.pwValue);
				::_AddToByteArray(bArray, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bArray, (WORD)sizeof(DWORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(shapeitr->second.pdwValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(shapeitr->second.pdwValue);
				::_AddToByteArray(bArray, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(bArray, (WORD)sizeof(float));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(shapeitr->second.pfValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(shapeitr->second.pfValue);
				::_AddToByteArray(bArray, (float)sendValue);
				break;
			case 4:	// long
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(bArray, (WORD)sizeof(long));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(shapeitr->second.pValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(shapeitr->second.pValue);
				::_AddToByteArray(bArray, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(bArray, (WORD)sizeof(short));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(shapeitr->second.psValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(shapeitr->second.psValue);
				::_AddToByteArray(bArray, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(bArray, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(bArray, (WORD)sizeof(bool));
				if((name[0] == 'E' && name[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(shapeitr->second.pbValue);
					sendValue = GetCurrentValue((char*)shapeitr->first.c_str(), shapeitr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(shapeitr->second.pbValue);
				::_AddToByteArray(bArray, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}

		if(name[0] == 'C' && name[1] == '_')
		{	
			// ���ҳ�����ֵ
			name[0] = 'B';
			baseValue = GetValue(name.c_str());
			// ���ҳ��ı�ֵ
			name[0] = 'E';
		}
		playeritr = m_mapNameValue.find(name);
		if (playeritr!=m_mapNameValue.end())
		{
			lNum++;
			// ��������
			::_AddToByteArray(bPlayerArray, (WORD)playeritr->second.attrEnumValue);

			if("Money" == name)
			{
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(DWORD));
				::_AddToByteArray(bPlayerArray, GetMoney());
				continue;
			}

			long sendValue = INVILID_VALUE;
			switch (playeritr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(BYTE));
				if((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(playeritr->second.plValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(playeritr->second.plValue);

				::_AddToByteArray(bPlayerArray, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(WORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(playeritr->second.pwValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(playeritr->second.pwValue);
				::_AddToByteArray(bPlayerArray, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(DWORD));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(playeritr->second.pdwValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(playeritr->second.pdwValue);
				::_AddToByteArray(bPlayerArray, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(float));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(playeritr->second.pfValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(playeritr->second.pfValue);
				::_AddToByteArray(bPlayerArray, (float)sendValue);
				break;
			case 4:	// long
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(long));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(playeritr->second.pValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(playeritr->second.pValue);
				::_AddToByteArray(bPlayerArray, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(short));
				if(((*pStrItr)[0] == 'C' && (*pStrItr)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(playeritr->second.psValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(playeritr->second.psValue);
				::_AddToByteArray(bPlayerArray, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(bPlayerArray, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(bPlayerArray, (WORD)sizeof(bool));
				if((name[0] == 'E' && name[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(playeritr->second.pbValue);
					sendValue = GetCurrentValue((char*)playeritr->first.c_str(), playeritr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(playeritr->second.pbValue);
				::_AddToByteArray(bPlayerArray, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}
	}
	if(lShapeNum == 0 && lNum==0)
		return false;

	// shape���Ը���
	setWriteDB.AddToByteArray( (WORD)lShapeNum);
	if(vecData.size() > 0)
		setWriteDB.AddToByteArray(&vecData[0],(long)vecData.size());
	// Player���Ը���
	setWriteDB.AddToByteArray( (WORD)lNum);
	if(vecPlayerData.size() > 0)
		setWriteDB.AddToByteArray(&vecPlayerData[0],(long)vecPlayerData.size());
	return true;
}

long CPlayer::UpdateAttributeToClient()
{
	if(m_mapDirtyAttrData.size() == 0)
		return false;

	CMessage msg( MSG_S2C_SHAPE_CHANGE_PROPERTIES );
	msg.Add(GetType());
	msg.Add(GetExID());
	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);
	CodeChangeAttrToDataBlock(setWriteDB);
	msg.SendToPlayer(GetExID(),false);

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("Hp"))
		{
			pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrHp);
		}
		if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("Mp"))
		{
			pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrMp);
		}
		if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("C_MaxHp"))
		{
			pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxHp);
		}
		if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("C_MaxMp"))
		{
			pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxMp);
		}
	}
	m_mapDirtyAttrData.clear();
	return true;
}

long  CPlayer::UpdateAttribute(BOOL bImmediately)
{
	if(!IsEnterGame()) return false;

	if(!bImmediately)
	{
		RegisterUpdateAttrTimer();
		return true;
	}

	UpdateAttributeToClient();	
	UpdateOtherAttributeToClient();

	return true;
}

long CPlayer::UpdateOtherAttributeToClient()
{
	//����Χ��Ҹ�������
	if(m_mapOtherDirtyAttrData.size() > 0)
	{
		set<string>	setDirtyAttr;
		set<string>::iterator it = m_mapOtherDirtyAttrData.find("C_MaxHp");
		if( it != m_mapOtherDirtyAttrData.end() )
		{
			m_mapOtherDirtyAttrData.erase(it);
			setDirtyAttr.insert("C_MaxHp");
		}
		it = m_mapOtherDirtyAttrData.find("Hp");
		if( it != m_mapOtherDirtyAttrData.end() )
		{
			m_mapOtherDirtyAttrData.erase(it);
			setDirtyAttr.insert("Hp");
		}
		// ��Ӹ���PK CPֵ
		it = m_mapOtherDirtyAttrData.find( "CurPKCP" );
		if( it != m_mapOtherDirtyAttrData.end() )
		{
			m_mapOtherDirtyAttrData.erase( it );
			setDirtyAttr.insert( "CurPKCP" );
		}
		it = m_mapOtherDirtyAttrData.find( "MaxPKCP" );
		if( it != m_mapOtherDirtyAttrData.end() )
		{
			m_mapOtherDirtyAttrData.erase( it );
			setDirtyAttr.insert( "MaxPKCP" );
		}

		if(setDirtyAttr.size() > 0 && GetSelectSelfPlayer().size() > 0)
		{
			UpdateHPMPToClient(setDirtyAttr);
		}

		if(m_mapOtherDirtyAttrData.size() > 0)
		{
			CMessage msgOther( MSG_S2C_SHAPE_CHANGE_PROPERTIES );
			msgOther.Add(GetType());
			msgOther.Add(GetExID());
			DBWriteSet setWriteDBOther;
			msgOther.GetDBWriteSet(setWriteDBOther);
			if( CodeOtherUpdateAttrToDataBlock(setWriteDBOther) )
				msgOther.SendToAround(this,this);
		}

		GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
		if(NULL != pTeam)
		{
			if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("Hp"))
			{
				pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrHp);
			}
			if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("Mp"))
			{
				pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrMp);
			}
			if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("C_MaxHp"))
			{
				pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxHp);
			}
			if(m_mapDirtyAttrData.end() != m_mapDirtyAttrData.find("C_MaxMp"))
			{
				pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxMp);
			}
		}

		m_mapOtherDirtyAttrData.clear();
	}
	return true;
}

long  CPlayer::UpdateHPMPToClient(set<string>& dirtyAttr)
{
	CMessage msgOther( MSG_S2C_SHAPE_CHANGE_PROPERTIES );
	msgOther.Add(GetType());
	msgOther.Add(GetExID());
	DBWriteSet setWriteDBOther;
	msgOther.GetDBWriteSet(setWriteDBOther);
	if( CodeDirtyAttrToDataBlock(dirtyAttr,setWriteDBOther) )
		msgOther.SendToPlayers(GetSelectSelfPlayer());
	return true;
}

long  CPlayer::OnUpdateAttributeTimeOut()
{
	UpdateAttributeToClient();
	UpdateOtherAttributeToClient();
	return true;
}


void CPlayer::OnSuitNumChanged(long suit_id,int suit_num)
{	
	map<long,vector<tagSuitGoods>*>::iterator vecIter=m_mSuitState.find(suit_id);
	if(vecIter!=m_mSuitState.end())
	{
		for(long i=0;i< (long) vecIter->second->size();i++)
		{
			CGoods* pGoods=(*(vecIter->second))[i].pGoods;
			pGoods->OnSuitNumChanged(this,suit_num);
		}
	}	
}

void CPlayer::AllEquipmentRefineUpdate(long lVal)
{
	//������������װ��������
	CGoods* pEquip=NULL;
	for(DWORD dwPos=EC_E_HEAD;dwPos<=EC_E_RIDER;dwPos++)
	{
		pEquip=m_cEquipment.GetGoods(dwPos);
		if(pEquip)
		{
			//���뺬��������������
			if(pEquip->HasAddonProperty(GAP_REFINE))
			{
				DWORD dwOldVal=pEquip->GetAddonPropertyValues(GAP_REFINE,1);
				pEquip->AddRefine(lVal);
				DWORD dwNewVal=pEquip->GetAddonPropertyValues(GAP_REFINE,1);
				DWORD dwMaxVal=pEquip->GetAddonPropertyValues(GAP_REFINE,2);
				if( (dwOldVal / 100 != dwNewVal / 100) || (dwOldVal!=dwMaxVal && dwNewVal==dwMaxVal) )
				{
					CMessage msg(MSG_S2C_EQUIP_REFINE_UPDATE);
					msg.Add((BYTE)dwPos);
					msg.Add(dwNewVal);
					msg.SendToPlayer(GetExID(),false);
				}
			}
		}
	}
}

BOOL CPlayer::DropGoods( CS2CContainerObjectMove::PLAYER_EXTEND_ID pei, const CGUID& guid )
{
	BOOL bResult = FALSE;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		CGoodsContainer* pContainer = NULL;
		switch( pei )
		{
		case CS2CContainerObjectMove::PEI_PACKET:
			pContainer = getOriginPack();
			break;
		case CS2CContainerObjectMove::PEI_EQUIPMENT:
			pContainer = &m_cEquipment;
			break;
		case CS2CContainerObjectMove::PEI_WALLET:
			pContainer = &m_pPackage.GetWallet();
			break;
		case CS2CContainerObjectMove::PEI_GEM:
			pContainer = &m_pPackage.GetGem();
			break;
        case CS2CContainerObjectMove::PEI_EQUIP_GOD:
            pContainer = &m_cEquipmentGod;
            break;
        case CS2CContainerObjectMove::PEI_EQUIP_LAND:
            pContainer = &m_cEquipmentLand;
            break;
        case CS2CContainerObjectMove::PEI_EQUIP_HUMAN:
            pContainer = &m_cEquipmentHuman;
            break;
		case CS2CContainerObjectMove::PEI_PACK1:
			pContainer = m_pPackage.GetSubpack(0)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK2:
			pContainer = m_pPackage.GetSubpack(1)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK3:
			pContainer = m_pPackage.GetSubpack(2)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK4:
			pContainer = m_pPackage.GetSubpack(3)->pSubpackContainer;
			break;
		case CS2CContainerObjectMove::PEI_PACK5:
			pContainer = m_pPackage.GetSubpack(4)->pSubpackContainer;
			break;
		}
		if( pContainer )
		{
			CS2CContainerObjectMove comMsg;
			CGoods* pGoods = dynamic_cast<CGoods*>( pContainer -> Remove(TYPE_GOODS, guid, &comMsg) );
			if( pGoods )
			{
				LONG lX = GetTileX();
				LONG lY = GetTileY();
				LONG lDropGoodsStartPos = 0;
				lDropGoodsStartPos = pRegion -> GetDropGoodsPos( lX, lY, lX, lY, lDropGoodsStartPos );
				if( lDropGoodsStartPos != -1 )
				{
					DWORD dwDestinationContainerPosition = lX + pRegion -> GetWidth() * lY;
					pGoods -> SetPosXY( lX+0.5f, lY+0.5f );

					pRegion -> AddObject( pGoods );
					CArea* pArea = pRegion->GetArea(lX/AREA_WIDTH, lY/AREA_HEIGHT);
					if(pArea)
					{
						pArea->AddGoodsDelTime(pGoods);
					}
					comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
					comMsg.SetSourceContainerExtendID( pei );
					comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), dwDestinationContainerPosition );
					comMsg.Send( GetExID() );

					CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
					MsgSend.Add( pGoods->GetType() );
					MsgSend.Add( pGoods->GetExID() );
					DBWriteSet setWriteDB;
					MsgSend.GetDBWriteSet(setWriteDB);
					pGoods->SerializeForRgnDisplay(setWriteDB);

					MsgSend.SendToAround(pGoods);

					bResult = TRUE;
				}
			}
		}
	}
	return bResult;
}

BOOL CPlayer::AddGoodsWithProtect(CGoods* pGoods,BOOL bProtect, LONG *pOptRerult)
{	
	//! �޸�����ӿڵ�ʱ��ͬʱ�޸��������AddAndReturnGoodsWithProtect�ӿ�

	if(!pGoods)
		return FALSE;
	DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
	BOOL bAdded=FALSE;	
	DWORD dwCell = 0;
	if(getOriginPack()->FindPositionForGoods(pGoods, dwCell))
	{
		CGoods* pExistedGoods =getOriginPack()->GetGoods( dwCell );
		if( pExistedGoods == NULL )
		{
			CS2CContainerObjectMove comMsg;
			if(getOriginPack()->Add(dwCell,pGoods, NULL, &comMsg) )
			{
				GetGame()->UpdatePlayerPack(this,CS2CContainerObjectMove::PEI_PACKET,0);
				OnGetGoods(dwGoodsId,pGoods);
				comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
				comMsg.SetDestinationContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
				pGoods-> SerializeForOldClient( &comMsg.GetObjectStream() );								
				comMsg.Send(GetExID() );
				bAdded=TRUE;				
				if(NULL != pOptRerult) *pOptRerult = 1;
			}
		}
		else
		{
			if(getOriginPack()->Add(dwCell,pGoods, NULL) )
			{
				GetGame()->UpdatePlayerPack(this,CS2CContainerObjectMove::PEI_PACKET,0);
				OnGetGoods(dwGoodsId);
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( TYPE_PLAYER, GetExID(), dwCell );
				coacMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
				coacMsg.SetObject( pExistedGoods -> GetType(), pExistedGoods -> GetExID() );
				coacMsg.SetObjectAmount( pExistedGoods -> GetAmount() );
				coacMsg.Send(GetExID() );
				bAdded=TRUE;				
				if(NULL != pOptRerult) *pOptRerult = 2;
			}
		}
		GetGame()->UpdatePlayerPack(this,CS2CContainerObjectMove::PEI_PACKET,dwCell);
	}
    else
    {
        for(long l=CS2CContainerObjectMove::PEI_PACK1; l<=CS2CContainerObjectMove::PEI_PACK5; l++)
        {
            int nSub = l - CS2CContainerObjectMove::PEI_PACK1;
            CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(nSub)->pSubpackContainer;
            CGoods* pExistedGoods = pContainer->GetGoods(dwCell);
            if( pExistedGoods == NULL )
            {
                CS2CContainerObjectMove comMsg;
                if(pContainer->Add(dwCell, pGoods, NULL, &comMsg))
                {
                    GetGame()->UpdatePlayerPack(this, l, 0);
                    OnGetGoods(dwGoodsId, pGoods);
                    comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
                    comMsg.SetDestinationContainerExtendID(l);
                    pGoods->SerializeForOldClient(&comMsg.GetObjectStream());
                    comMsg.Send(GetExID());
                    bAdded = TRUE;
                    if(NULL != pOptRerult)
                        *pOptRerult = 1;
                }
            }
            else
            {
                if(pContainer->Add(dwCell,pGoods, NULL) )
                {
                    GetGame()->UpdatePlayerPack(this, l, 0);
                    OnGetGoods(dwGoodsId);
                    CS2CContainerObjectAmountChange coacMsg;
                    coacMsg.SetSourceContainer( TYPE_PLAYER, GetExID(), dwCell );
                    coacMsg.SetSourceContainerExtendID( l );
                    coacMsg.SetObject( pExistedGoods -> GetType(), pExistedGoods -> GetExID() );
                    coacMsg.SetObjectAmount( pExistedGoods -> GetAmount() );
                    coacMsg.Send(GetExID() );
                    bAdded=TRUE;
                    if(NULL != pOptRerult) 
                        *pOptRerult = 2;
                }
            }
            GetGame()->UpdatePlayerPack(this, nSub, dwCell);
            if(bAdded)
                break;
        }
    }

	if(!bAdded && bProtect)
	{
		//��ӵ����������ñ���ʱ��
		CServerRegion* pRegion=dynamic_cast<CServerRegion*>(GetFather());
		if(pRegion)
		{
			long x=0, y=0;
			long lDropGoodsStartPos=0;

			lDropGoodsStartPos= pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), x, y, lDropGoodsStartPos);
			if( lDropGoodsStartPos != -1 )
			{
				pGoods->SetPosXY(x+0.5f, y+0.5f);
				pRegion->AddObject( pGoods);				
				pRegion -> SetGoodsProtection( this, pGoods);				
				// ֪ͨÿ�����
				/*
				CS2CContainerObjectMove comMsg;
				comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
				comMsg.SetDestinationContainer( pRegion -> GetType(), 
				pRegion -> GetExID(), y * pRegion -> GetWidth() + x );
				comMsg.SetDestinationObject(pGoods -> GetType(),pGoods-> GetExID() );
				comMsg.SetDestinationObjectAmount(pGoods-> GetAmount() );
				pGoods-> SerializeForOldClient( &( comMsg.GetObjectStream() ) );
				comMsg.SendToAround( this );
				comMsg.Send(GetExID());*/
				CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
				MsgSend.Add( pGoods->GetType() );
				MsgSend.Add( pGoods->GetExID() );
				DBWriteSet setWriteDB;
				MsgSend.GetDBWriteSet(setWriteDB);
				pGoods->SerializeForRgnDisplay(setWriteDB);

				MsgSend.SendToAround(pGoods);
				if(NULL != pOptRerult) *pOptRerult = 1;
			}
		}
	}
	return bAdded;	
}

BOOL CPlayer::AddAndReturnGoodsWithProtect(CGoods **ppGoods,BOOL bProtect, LONG *pOptRerult)
{	
	if(NULL == ppGoods)
		return FALSE;
	CGoods *pGoods = *ppGoods;
	if(!pGoods)
		return FALSE;
	DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
	BOOL bAdded=FALSE;	
	DWORD dwCell = 0;
	if(getOriginPack()->FindPositionForGoods(pGoods, dwCell))
	{
		CGoods* pExistedGoods =getOriginPack()->GetGoods( dwCell );
		if( pExistedGoods == NULL )
		{
			CS2CContainerObjectMove comMsg;
			if(getOriginPack()->Add(dwCell,pGoods, NULL, &comMsg) )
			{
				OnGetGoods(dwGoodsId,pGoods);
				comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
				comMsg.SetDestinationContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
				pGoods-> SerializeForOldClient( &comMsg.GetObjectStream() );								
				comMsg.Send(GetExID() );
				bAdded=TRUE;				
				if(NULL != pOptRerult) *pOptRerult = 1;
			}
		}
		else
		{
			if(getOriginPack()->Add(dwCell,pGoods, NULL) )
			{
				OnGetGoods(dwGoodsId);
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( TYPE_PLAYER, GetExID(), dwCell );
				coacMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
				coacMsg.SetObject( pExistedGoods -> GetType(), pExistedGoods -> GetExID() );
				coacMsg.SetObjectAmount( pExistedGoods -> GetAmount() );
				coacMsg.Send(GetExID() );
				bAdded=TRUE;				
				if(NULL != pOptRerult) *pOptRerult = 2;
				CGoods *pTmpGoods = getOriginPack()->GetGoods(dwCell);
				if(NULL != pTmpGoods)*ppGoods = pTmpGoods;
			}
		}
    }
    else
    {
        for(long l=CS2CContainerObjectMove::PEI_PACK1; l<=CS2CContainerObjectMove::PEI_PACK5; l++)
        {
            int nSub = l - CS2CContainerObjectMove::PEI_PACK1;
            CVolumeLimitGoodsContainer* pContainer=GetPackage()->GetSubpack(nSub)->pSubpackContainer;
            if(pContainer && pContainer->FindPositionForGoods(pGoods, dwCell))
            {
                CGoods* pExistedGoods =pContainer->GetGoods( dwCell );
                if( pExistedGoods == NULL )
                {
                    CS2CContainerObjectMove comMsg;
                    if(pContainer->Add(dwCell,pGoods, NULL, &comMsg) )
                    {
                        OnGetGoods(dwGoodsId,pGoods);
                        comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
                        comMsg.SetDestinationContainerExtendID( l );
                        pGoods-> SerializeForOldClient( &comMsg.GetObjectStream() );								
                        comMsg.Send(GetExID() );
                        bAdded=TRUE;
                        //vOut[i] = NULL;
                        if(NULL != pOptRerult) *pOptRerult = 1;
                    }
                }
                else
                {
                    if(pContainer->Add(dwCell,pGoods, NULL) )
                    {
                        OnGetGoods(dwGoodsId);
                        CS2CContainerObjectAmountChange coacMsg;
                        coacMsg.SetSourceContainer( TYPE_PLAYER, GetExID(), dwCell );
                        coacMsg.SetSourceContainerExtendID( l );
                        coacMsg.SetObject( pExistedGoods -> GetType(), pExistedGoods->GetExID() );
                        coacMsg.SetObjectAmount( pExistedGoods -> GetAmount() );
                        coacMsg.Send(GetExID() );
                        bAdded=TRUE;
                        //vOut[i] = NULL;
                        if(NULL != pOptRerult) *pOptRerult = 2;
                        CGoods *pTmpGoods = pContainer->GetGoods(dwCell);
                        if(NULL != pTmpGoods)*ppGoods = pTmpGoods;
                    }
                }
                if(bAdded)
                    break;
            }            
        }
    }

	if(!bAdded && bProtect)
	{
		//��ӵ����������ñ���ʱ��
		CServerRegion* pRegion=dynamic_cast<CServerRegion*>(GetFather());
		if(pRegion)
		{
			long x=0, y=0;
			long lDropGoodsStartPos=0;

			lDropGoodsStartPos= pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), x, y, lDropGoodsStartPos);
			if( lDropGoodsStartPos != -1 )
			{
				pGoods->SetPosXY(x+0.5f, y+0.5f);
				pRegion->AddObject( pGoods);				
				pRegion -> SetGoodsProtection( this, pGoods);				
				// ֪ͨÿ�����
				CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
				MsgSend.Add( pGoods->GetType() );
				MsgSend.Add( pGoods->GetExID() );
				DBWriteSet setWriteDB;
				MsgSend.GetDBWriteSet(setWriteDB);
				pGoods->SerializeForRgnDisplay(setWriteDB);

				MsgSend.SendToAround(pGoods);
				if(NULL != pOptRerult) *pOptRerult = 1;
			}
		}
	}
	return bAdded;	
}

BOOL    CPlayer::EquipBackToPack()
{
	CGoods* pGoods=m_cEquipment.GetBackToPackGoods();
	if(!pGoods)
		return FALSE;
	CS2CContainerObjectMove cResult;
	DWORD dwSourcePos=0;
	m_cEquipment.QueryGoodsPosition(pGoods,dwSourcePos);
	m_cEquipment.Remove(pGoods);

	cResult.SetOperation(CS2CContainerObjectMove::OT_MOVE_OBJECT);
	cResult.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );	

	DWORD dwCell = 0;
	if(getOriginPack()->FindPositionForGoods(pGoods, dwCell))
	{		
		if(getOriginPack()->Add(dwCell,pGoods, NULL) )
		{

			cResult.SetSourceContainer( TYPE_PLAYER, GetExID(), dwSourcePos );					
			cResult.SetSourceObject(pGoods->GetType(),pGoods->GetExID());
			cResult.SetDestinationContainer(TYPE_PLAYER,GetExID(),dwCell);
			cResult.SetDestinationContainerExtendID(CS2CContainerObjectMove::PEI_PACKET);
			cResult.SetDestinationObject(pGoods->GetType(),pGoods->GetExID());
			cResult.SetSourceObjectAmount(pGoods->GetAmount());
			cResult.SetDestinationObjectAmount(pGoods->GetAmount());
			cResult.Send(GetExID());
			m_cEquipment.ClearBackToPackGoods();

			return TRUE;		
		}		
	}
    else
    {
        BOOL bAddSucc = FALSE;
        for(long l=CS2CContainerObjectMove::PEI_PACK1; l<=CS2CContainerObjectMove::PEI_PACK5; l++)
        {
            int nSub = l - CS2CContainerObjectMove::PEI_PACK1;
            CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(nSub)->pSubpackContainer;
            if(pContainer && pContainer->FindPositionForGoods(pGoods, dwCell))
            {
                if(pContainer->Add(dwCell, pGoods, NULL))
                {
                    cResult.SetSourceContainer( TYPE_PLAYER, GetExID(), dwSourcePos );					
                    cResult.SetSourceObject(pGoods->GetType(),pGoods->GetExID());
                    cResult.SetDestinationContainer(TYPE_PLAYER,GetExID(),dwCell);
                    cResult.SetDestinationContainerExtendID(l);
                    cResult.SetDestinationObject(pGoods->GetType(),pGoods->GetExID());
                    cResult.SetSourceObjectAmount(pGoods->GetAmount());
                    cResult.SetDestinationObjectAmount(pGoods->GetAmount());
                    cResult.Send(GetExID());
                    m_cEquipment.ClearBackToPackGoods();
                    bAddSucc = TRUE;
                    break;
                }
            }
        }
        if(bAddSucc)
        {
            return TRUE;
        }
    }
   
	m_cEquipment.ClearBackToPackGoods();
	return FALSE;
}

BOOL CPlayer::IsInPersonalShop(CGoods* pGoods)
{
	vector<CGoods*>::iterator it=m_vecInPersonalShopGoods.begin();
	for(;it!=m_vecInPersonalShopGoods.end();it++)
	{
		if(pGoods==*it)
			return TRUE;
	}
	return FALSE;
}

void CPlayer::RemovePersonalShopGoodsData(CGUID& guid)
{
	vector<CGoods*>::iterator it=m_vecInPersonalShopGoods.begin();
	for(;it!=m_vecInPersonalShopGoods.end();it++)
	{
		if((*it)->GetExID()==guid)
		{
			m_vecInPersonalShopGoods.erase(it);
			return;
		}
	}	
}

void CPlayer::SetTicket(long	lTicket)
{
	long lOldTicket=m_lTicket;
	m_lTicket=lTicket;
	if(m_lTicket<0)
		m_lTicket=0;
	else if(m_lTicket>(long)GlobalSetup::GetSetup()->dwTicketLimit)
	{
		m_lTicket=GlobalSetup::GetSetup()->dwTicketLimit;
	}
	long lChangeVal=m_lTicket-lOldTicket;
	if(lChangeVal>0)
	{
		//�õ�
		CHAR szMsg[256];
		const char* strRes=AppFrame::GetText("40");
		if(strRes)
		{
			_snprintf( szMsg, 256, strRes, lChangeVal );
			//SendNotifyMessage( szMsg,0xffffffff,0,eNOTIFYPOS_LEFT_BOTTOM);
			SendNotifyMessage(szMsg,eNOTIFYPOS_LEFT_BOTTOM);
		}

	}
	else if(lChangeVal<0)
	{
		//ʧȥ
		CHAR szMsg[256];
		const char* strRes=AppFrame::GetText("39");
		if(strRes)
		{
			_snprintf( szMsg, 256, strRes, -lChangeVal );
			//SendNotifyMessage( szMsg,0xffffffff,0,eNOTIFYPOS_LEFT_BOTTOM);
			SendNotifyMessage(szMsg,eNOTIFYPOS_LEFT_BOTTOM);
		}
	}

	CMessage msg(MSG_S2C_SET_TICKET);
	msg.Add(m_lTicket);
	msg.SendToPlayer(GetExID(),false);
	// �����������
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
}

void CPlayer::AddPersonalShopGoodsData(CGoods* pGoods)
{
	if(!IsInPersonalShop(pGoods))
		m_vecInPersonalShopGoods.push_back(pGoods);
}

BOOL CPlayer::CheckChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut)
{	
	///////////////////////////
	vector<CVolumeLimitGoodsContainer*> vecCloneContainer;//������п�¡����������
	CVolumeLimitGoodsContainer* pOriginPackClone=OBJ_CREATE(CVolumeLimitGoodsContainer);
	vector<CGoods*> cloneGoodsVec;
	pOriginPackClone->SetContainerMode( CGoodsContainer::GCM_TEST );
	getOriginPack()->CloneEx(pOriginPackClone,&cloneGoodsVec);
	vecCloneContainer.push_back(pOriginPackClone);
	for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{
		CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
		if(pTagSubpack && pTagSubpack->pGoods)
		{
			CVolumeLimitGoodsContainer* pSubPackClone=OBJ_CREATE(CVolumeLimitGoodsContainer);
			pSubPackClone->SetContainerMode( CGoodsContainer::GCM_TEST );
			pTagSubpack->pSubpackContainer->CloneEx(pSubPackClone,&cloneGoodsVec);
			vecCloneContainer.push_back(pSubPackClone);
		}
	}
#ifdef _GOODSLOG1_
	char pszLog1[1024]="";							

	_snprintf(pszLog1,1024,"[MSG_C2S_SHOP_BUY][������¡���]");
	CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog1);							
#endif
	if(vecCloneContainer.size()<1)
	{
		//û�п��Կ�¡������
		return FALSE;
	}
	//�����ڿ�¡�����п۳�������Ҫ�۳�����Ʒ,���
	//�������ʧ�ܣ�˵��������Ʒ��������������
	for(long i=0 ; i<(long)vecGoodsOut.size();i++)
	{
		DWORD dwGoodsId=vecGoodsOut[i].dwGoodsId;		
		DWORD dwRemoveTotal=vecGoodsOut[i].dwAmount;		
		for(long j=0;j<(long)vecCloneContainer.size();j++)
		{
			//ɾ����Ʒֱ��dwRemoveAmount==dwAmount ��˵������Ʒ������������
			DWORD dwRemoveAmount=vecCloneContainer[j]->DelGoods(dwGoodsId,dwRemoveTotal);
			if(dwRemoveTotal==dwRemoveAmount)
			{
				//˵��Ҫ��ɾ��������ȫ���ɹ�ɾ����
				dwRemoveTotal=0;
				break;
			}
			else
			{
				dwRemoveTotal-=dwRemoveAmount;
			}
		}
		if(dwRemoveTotal>0)
		{
			//��������������Ҳ���ܰ���Ʒ�۳���
			for(int n=0;n<(int)vecCloneContainer.size();n++)
			{
				OBJ_RELEASE(CVolumeLimitGoodsContainer, vecCloneContainer[n]);
			}

			for(int n=0;n<(int)cloneGoodsVec.size();n++)
			{
				CGoodsFactory::GarbageCollect(&cloneGoodsVec[n],18);
			}
			return FALSE;
		}
	}
#ifdef _GOODSLOG1_
	char pszLog2[1024]="";							

	_snprintf(pszLog2,1024,"[MSG_C2S_SHOP_BUY][���Կ۳�������Ҫ�۳�����Ʒ]");
	CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog2);							
#endif

	//���е�����˵���۳���Ʒ�������������
	//��ʼ�������Ҫ��������Ʒ
	BOOL bSucceed=TRUE;
	vector<CGoods*> vecTempGoods;
	for(long i=0 ; i<(long)vecGoodsIn.size();i++)
	{		
		CGoodsFactory::CreateGoods(vecGoodsIn[i].dwGoodsId,vecGoodsIn[i].dwAmount,vecTempGoods,5);
	}
	for(long i=0 ; i<(long)vecTempGoods.size();i++)
	{
		BOOL bAdded=FALSE;
		for(long j=0;j<(long)vecCloneContainer.size();j++)
		{
			bAdded=vecCloneContainer[j]->Add(vecTempGoods[i],NULL);
			if(bAdded)
				break;
		}
		if(!bAdded)
		{
			//������ȫ�����������ܷ��¸���Ʒ
			bSucceed=FALSE;
			break;
		}
	}
#ifdef _GOODSLOG1_
	char pszLog3[1024]="";							

	_snprintf(pszLog3,1024,"[MSG_C2S_SHOP_BUY][�������Ҫ��������Ʒ���]");
	CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszLog3);							
#endif

	for(long i=0 ; i<(long)vecTempGoods.size();i++)
	{
#ifdef _GOODSLOG1_
		if(vecTempGoods[i])
		{
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			vecTempGoods[i]->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:���ﻻ�������ʱ��Ʒ][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				vecTempGoods[i]->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(vecTempGoods[i]->GetBasePropertiesIndex()),srcGoodsGUID,vecTempGoods[i]->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
		}								
#endif
		CGoodsFactory::GarbageCollect(&vecTempGoods[i],20);
	}
	for(long n=0 ; n<(long)vecCloneContainer.size();n++)
	{
		OBJ_RELEASE(CVolumeLimitGoodsContainer, vecCloneContainer[n]);
	}
	for(long n=0 ; n<(long)cloneGoodsVec.size(); n++)
	{
		CGoodsFactory::GarbageCollect(&cloneGoodsVec[n],18);
	}
	return bSucceed;
}

void CPlayer::GoodsChangeGoods(vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsIn,vector<CGoodsBaseProperties::tagGoodsInfo>& vecGoodsOut)
{
	///////////////////////////
	struct tagContainers
	{
		DWORD dwContainerId;
		CVolumeLimitGoodsContainer* pContainer;
	};
	vector<tagContainers*> vecContainer;	
	tagContainers* stOrigin=OBJ_CREATE(tagContainers);
	stOrigin->dwContainerId=CS2CContainerObjectMove::PEI_PACKET;
	stOrigin->pContainer=getOriginPack();
	vecContainer.push_back(stOrigin);
	for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{
		CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
		if(pTagSubpack && pTagSubpack->pGoods)
		{	
			tagContainers* stSubpack=OBJ_CREATE(tagContainers);
			stSubpack->dwContainerId=CS2CContainerObjectMove::PEI_PACK1+i;
			stSubpack->pContainer=pTagSubpack->pSubpackContainer;
			vecContainer.push_back(stSubpack);
		}
	}	
	if(vecContainer.size()<1)
	{
		//û�п��Կ�¡������
		return;
	}
	//�����ڿ�¡�����п۳�������Ҫ�۳�����Ʒ,���
	//�������ʧ�ܣ�˵��������Ʒ��������������
	CMessage msg(MSG_S2C_SHOP_GOODSCHANGEGOODS);
	//Ҫ�۳���Ʒ�ĸ���
	msg.Add((DWORD)vecGoodsOut.size());	
	struct tagGoodsOutData
	{
		DWORD dwContainerId;
		vector<CGoodsBaseProperties::tagGoodsDelInfo> vecPosAndNum;
	};

	for(int i=0;i<(int)vecGoodsOut.size();i++)
	{
		DWORD dwGoodsId=vecGoodsOut[i].dwGoodsId;		
		DWORD dwRemoveTotal=vecGoodsOut[i].dwAmount;
		vector<tagGoodsOutData*> vecGoodsOutData;
		vecGoodsOutData.clear();
		for(long j=0; j<(long)vecContainer.size();j++)
		{
			//ɾ����Ʒֱ��dwRemoveAmount==dwAmount ��˵������Ʒ������������
			tagGoodsOutData* stOutData=OBJ_CREATE(tagGoodsOutData);
			stOutData->dwContainerId=vecContainer[j]->dwContainerId;
			DWORD dwRemoveAmount=vecContainer[j]->pContainer->DelGoods(dwGoodsId,dwRemoveTotal,&(stOutData->vecPosAndNum),TRUE);
			vecGoodsOutData.push_back(stOutData);
			if(dwRemoveTotal==dwRemoveAmount)
			{
				//˵��Ҫ��ɾ��������ȫ���ɹ�ɾ����
				dwRemoveTotal=0;
				break;
			}                 
			else
			{
				dwRemoveTotal-=dwRemoveAmount;
			}			
		}
		if(dwRemoveTotal>0)
		{
			//��������������Ҳ���ܰ���Ʒ�۳���			
		}
		msg.Add((DWORD)vecGoodsOutData.size());
		for(long j=0;j<(long)vecGoodsOutData.size();j++)
		{
			msg.Add(vecGoodsOutData[j]->dwContainerId);
			msg.Add((DWORD)vecGoodsOutData[j]->vecPosAndNum.size());
			for(long k=0 ; k<(long)vecGoodsOutData[j]->vecPosAndNum.size();k++)
			{
				msg.Add(vecGoodsOutData[j]->vecPosAndNum[k].dwPos);
				msg.Add(vecGoodsOutData[j]->vecPosAndNum[k].dwNum);
			}
		}
		for(long j=0;j<(long)vecGoodsOutData.size();j++)
		{
			OBJ_RELEASE(tagGoodsOutData, vecGoodsOutData[j]);
		}
	}

	//���е�����˵���۳���Ʒ�������������
	//��ʼ�������Ҫ��������Ʒ

	vector<CGoods*> vecTempGoods;
	for(long i=0 ; i<(long)vecGoodsIn.size();i++)
	{		
		CGoodsFactory::CreateGoods(vecGoodsIn[i].dwGoodsId,vecGoodsIn[i].dwAmount,vecTempGoods,6);
	}
	msg.Add((DWORD)vecTempGoods.size());
	vector<DWORD> vecGoodsId;
	vector<CGoods*> vecGoodsPtr;
	vector<BYTE> btGoodsInData;
	for(long i=0 ; i<(long)vecTempGoods.size();i++)
	{
		BOOL bAdded=FALSE;
		for(int j=0;j<(int)vecContainer.size();j++)
		{
			DWORD dwTempGoodsId=vecTempGoods[i]->GetBasePropertiesIndex();
			DWORD dwTempGoodsNum=vecTempGoods[i]->GetAmount();
			DWORD dwMaxStackNum=vecTempGoods[i]->GetMaxStackNumber();
			bAdded=vecContainer[j]->pContainer->Add(vecTempGoods[i],&btGoodsInData,vecContainer[j]->dwContainerId);
			//���NPC�̵�����Ʒ����־��¼
			if(bAdded)
				GetLogicLogInterface()->logT030_goods_log_npc(this,dwTempGoodsId,vecTempGoods[i]->GetExID(),
					GoodsSetup::QueryGoodsName(dwTempGoodsId),2,0,dwTempGoodsNum);
#ifdef _GOODSLOG1_
			char pszGoodsLog[1024]="";
			char playerGUID[50]="";
			GetExID().tostring(playerGUID);

			_snprintf(pszGoodsLog,1024,"[����:���ﻻ�����Ʒ][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��Ʒ����:%d]",
				GetName(),playerGUID,vecContainer[j]->dwContainerId,dwTempGoodsId,GoodsSetup::QueryGoodsName(dwTempGoodsId),dwTempGoodsNum);
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif	
			if(bAdded)
			{
				if(dwMaxStackNum>1)
				{
					//OnGetGoods(dwTempGoodsId);
					vecGoodsId.push_back(dwTempGoodsId);
				}	
				else
				{
					//OnGetGoods(dwTempGoodsId,vecTempGoods[i]);
					vecGoodsPtr.push_back(vecTempGoods[i]);
				}
				break;
			}
		}
		if(!bAdded)
		{
			//������ȫ�����������ܷ��¸���Ʒ			
			break;
		}
	}
	if(btGoodsInData.size()==0)
		return;
	msg.Add((long)btGoodsInData.size());
	msg.AddEx(&btGoodsInData[0],(long)btGoodsInData.size());
	msg.SendToPlayer(GetExID(),false);

	vector<DWORD>::iterator idIter=vecGoodsId.begin();
	for(;idIter!=vecGoodsId.end();idIter++)
	{
		OnGetGoods(*idIter);
	}
	vector<CGoods*>::iterator ptrIter=vecGoodsPtr.begin();
	for(;ptrIter!=vecGoodsPtr.end();ptrIter++)
	{
		OnGetGoods((*ptrIter)->GetBasePropertiesIndex(),*ptrIter);
	}

	for(long i=0 ; i<(long)vecContainer.size();i++)
	{
		OBJ_RELEASE(tagContainers, vecContainer[i]);
	}
	vecContainer.clear();
}

//�����Ʒ�Ƿ��и���Ʒ��������Ʒ�ĸ���
int	CPlayer::CheckGoods(const char *goodoriname)
{
	long lRet = 0;
	if( goodoriname )
	{
		CSeekGoodsListener lSeeker;
		lSeeker.SetTarget( (char*)goodoriname );
		getOriginPack()->TraversingContainer( &lSeeker );
		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
			if( pGoods )
			{
				lRet += pGoods -> GetAmount();
			}
		}

		lSeeker.m_vGoodsID.clear();
		m_cEquipment.TraversingContainer( &lSeeker );
		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
			if( pGoods )
			{
				lRet += pGoods -> GetAmount();
			}
		}
	}
	return lRet;
}

DWORD CPlayer::GetTotalEmptySpaceNum()
{
	DWORD dwEmptyGrid=0;
	dwEmptyGrid+=getOriginPack()->GetEmptySpaceNum();
	for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{
		CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
		if(pTagSubpack && pTagSubpack->pSubpackContainer)
		{
			dwEmptyGrid+=pTagSubpack->pSubpackContainer->GetEmptySpaceNum();
		}
	}	
	return dwEmptyGrid;
}

DWORD CPlayer::GetTotalSpaceNum()
{
	DWORD allNum=0;
	allNum+=getOriginPack()->GetVolume();
	//�ӱ���
	for (int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{
		CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
		if(pTagSubpack && pTagSubpack->pSubpackContainer)
		{
			allNum+=pTagSubpack->pSubpackContainer->GetVolume();
		}
	}
	return allNum;
}


DWORD CPlayer::GetGoodsAmountByOriginName(const char* strOriginName)
{
	DWORD dwAmount=0;
	dwAmount+=getOriginPack()->GetGoodsAmountByOriginName(strOriginName);
	for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	{
		CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
		if(pTagSubpack && pTagSubpack->pSubpackContainer)
		{
			dwAmount+=pTagSubpack->pSubpackContainer->GetGoodsAmountByOriginName(strOriginName);
		}
	}	
	return dwAmount;
}


int	CPlayer::check_item_in_packet(const int item_idx)
{
	long lRet = 0;
	if( item_idx )
	{
		CSeekGoodsListener lSeeker;
		lSeeker.SetTarget( item_idx );
		getOriginPack()->TraversingContainer( &lSeeker );
		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
			if( pGoods )
			{
				lRet += pGoods -> GetAmount();
			}
		}
	}
	return lRet;
}

int	CPlayer::remove_item_in_packet(const int item_idx, const int item_num)
{
	long ret = 0;
	if( item_idx )
	{
		long lRemainedGoods;
		CSeekGoodsListener lSeeker;
		lSeeker.SetTarget( item_idx );
		getOriginPack()->TraversingContainer( &lSeeker );

		for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
		{
			lRemainedGoods = 0;
			if( ret < item_num )
				lRemainedGoods = item_num - ret;
			if( lRemainedGoods == 0 )
				break;
			ret += DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], lRemainedGoods );
		}
	}
	return ret;
}

BOOL CPlayer::DelGoodsAmountByOriginName( const char* goodsname, int goodsnum)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget( (char*)goodsname );
	getOriginPack()->TraversingContainer( &lSeeker );
	int  lRet=0;

	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		LONG lRemainedGoods = 0;
		if( lRet < goodsnum) lRemainedGoods = goodsnum - lRet;
		if( lRemainedGoods == 0 ) break;
		lRet += DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], lRemainedGoods );
	}

	//lSeeker.m_vGoodsID.clear();
	//m_cEquipment.TraversingContainer( &lSeeker );
	//for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	//{
	//	LONG lRemainedGoods = 0;
	//	if( lRet < goodsnum ) lRemainedGoods = goodsnum - lRet;
	//	if( lRemainedGoods == 0 ) break;
	//	lRet += DeleteGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lSeeker.m_vGoodsID[i], lRemainedGoods );
	//}

	BOOL bEnough=FALSE;
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				LONG lRemainedGoods=0;
				if(lRet<goodsnum)
					lRemainedGoods=goodsnum-lRet;
				if(lRemainedGoods==0)
				{
					bEnough=TRUE;
					break;
				}
				lRet+= DeleteGoods(static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i),lSeeker.m_vGoodsID[j],lRemainedGoods);
			}
			if(bEnough)
				break;			
		}
	}	
	//SAFE_DELETE_ARRAY(name);
	return lRet;
}

void CPlayer::SetPersonalShopFlag( const CGUID& SessionID, long PlugID )
{
	if( SessionID == NULL_GUID && PlugID == 0 )
	{
		m_PersonalShopSessionID	= NULL_GUID;
		m_PersonalShopPlugID		= 0;
	}
	else
	{
		CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(SessionID) );
		if( pSession )
		{
			CPlug* pPlug = pSession -> QueryPlugByOwner( GetType(), GetExID() );
			if( pPlug && pPlug -> Get32ID() == PlugID )
			{
				m_PersonalShopSessionID	= SessionID;
				m_PersonalShopPlugID		= PlugID;
			}
		}
	}
}

void CPlayer::CodePersonalShopForClient(DBWriteSet& setWriteDB)
{
    CPersonalShopSeller* pSeller = dynamic_cast<CPersonalShopSeller*>( 
        const_cast<CPlug*>( CSessionFactory::QueryPlug(m_PersonalShopPlugID) ) );
    if( m_PersonalShopSessionID != NULL_GUID &&  pSeller )
    {
        setWriteDB.AddToByteArray( m_PersonalShopSessionID );
        setWriteDB.AddToByteArray( m_PersonalShopPlugID );
        setWriteDB.AddToByteArray( pSeller -> GetShopName() );
    }
    else
    {
        setWriteDB.AddToByteArray(NULL_GUID );
    }
}

//������Ʒԭʼ�������б����в��ҵ�һ����Ʒ
CGoods* CPlayer::FindGoodInPack(const char* strGoodsName)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget((char*)strGoodsName );
	getOriginPack()->TraversingContainer(&lSeeker);
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(
			TYPE_GOODS, lSeeker.m_vGoodsID[i]));
		if( pGoods )
		{
			return pGoods;
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		//�ȼ���ӱ�����λ�еı�����Ʒ�����Ƿ�Ψһ
		CGoods* pSubContainer=GetPackage()->GetSubpack(
			i-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
		if(pSubContainer)
		{	
			const char* cName=GoodsSetup::QueryGoodsOriginalName(
				pSubContainer->GetBasePropertiesIndex());
			if (cName && 0==strcmp(strGoodsName,cName))
			{
				return pSubContainer;
			}
			//�˱����е���Ʒ�Ƿ�Ψһ
			lSeeker.m_vGoodsID.clear();
			GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->
				pSubpackContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(
					i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Find(
					TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					return pGoods;
				}
			}	
		}
	}	

	return NULL;
}

CGoods* CPlayer::FindGoodInDepot(const char* strGoodsName)
{
	static DWORD arrContainerType[] = 
	{
		eDCT_Primary,
		eDCT_Secondary1,
		eDCT_Secondary2,
		eDCT_Secondary3,
		eDCT_Secondary4
	};

	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget((char*)strGoodsName );

	for (LONG i = 0; i < sizeof(arrContainerType) / sizeof(DWORD); ++i)
	{
		CAmountLimitGoodsContainer *pContainer = (CAmountLimitGoodsContainer*)m_pPlayerDepot.FindContainer(arrContainerType[i],true);
		if(NULL != pContainer)
		{
			lSeeker.m_vGoodsID.clear();
			pContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					return pGoods;
				}
			}
		}
	}

	return NULL;
}

BOOL CPlayer::IsGoodsUnique(const char* strGoodsName)
{
	DWORD dwGoodsId=GoodsSetup::QueryGoodsIDByOriginalName(strGoodsName);
	if(!dwGoodsId)
		return FALSE;

	CGoods* pExistGoods=FindGoodInPack(strGoodsName);
	if (pExistGoods)	
		return FALSE;
	pExistGoods=FindGoodInDepot(strGoodsName);
	if(pExistGoods)
		return FALSE;
	pExistGoods=m_cEquipment.GetTheFirstGoods(dwGoodsId);
	if(pExistGoods)
		return FALSE;
	return TRUE;
}

//! ��������ϣ����б�����װ����λ���ֿ⣩�ҵ�һ����Ʒ��������������Ʒ��
CGoods* CPlayer::FindGoodByGuid(const CGUID &GoodsGUID)
{

	CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->FindIgnoreLock(GoodsGUID));
	if( NULL != pGoods )
	{
		return pGoods;
	}
	pGoods = dynamic_cast<CGoods*>(m_cEquipment.Find(GoodsGUID));
	if( NULL != pGoods )
	{
		return pGoods;
	}	
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentGod.Find(GoodsGUID));
    if(NULL != pGoods)
    {
        return pGoods;
    }
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentLand.Find(GoodsGUID));
    if(NULL != pGoods)
    {
        return pGoods;
    }
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentHuman.Find(GoodsGUID));
    if(NULL != pGoods)
    {
        return pGoods;
    }
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->FindIgnoreLock(GoodsGUID));
			if( NULL != pGoods )
			{
				return pGoods;
			}
		}
	}

	static DWORD arrContainerType[] = 
	{
		eDCT_Primary,
		eDCT_Secondary1,
		eDCT_Secondary2,
		eDCT_Secondary3,
		eDCT_Secondary4
	};

	for (LONG i = 0; i < sizeof(arrContainerType) / sizeof(DWORD); ++i)
	{
		CAmountLimitGoodsContainer *pContainer = (CAmountLimitGoodsContainer*)m_pPlayerDepot.FindContainer(arrContainerType[i]);
		if(NULL != pContainer)
		{
			pGoods = (CGoods*)pContainer->FindIgnoreLock(GoodsGUID);
			if(NULL != pGoods)
				return pGoods;
		}
	}


	return NULL;
}


CGoods* CPlayer::FindGoodsAllPlaceRemove(const CGUID& GoodsGUID)
{
	CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->FindIgnoreLock(GoodsGUID));
	if( NULL != pGoods )
	{
		getOriginPack()->Remove(pGoods);
		return pGoods;
	}
	pGoods = dynamic_cast<CGoods*>(m_cEquipment.Find(GoodsGUID));
	if( NULL != pGoods )
	{
		m_cEquipment.Remove(pGoods);
		return pGoods;
	}	
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentGod.Find(GoodsGUID));
    if( NULL != pGoods )
    {
        m_cEquipmentGod.Remove(pGoods);
        return pGoods;
    }	
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentLand.Find(GoodsGUID));
    if( NULL != pGoods )
    {
        m_cEquipmentLand.Remove(pGoods);
        return pGoods;
    }	
    pGoods = dynamic_cast<CGoods*>(m_cEquipmentHuman.Find(GoodsGUID));
    if( NULL != pGoods )
    {
        m_cEquipmentHuman.Remove(pGoods);
        return pGoods;
    }	
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		CGoods* pPackGoods=GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
		if(pPackGoods)
		{
			if(pPackGoods->GetExID()==GoodsGUID)
			{
				GetPackage()->GetSubpackContainer()->Remove(pGoods);
				return pPackGoods;
			}
			pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->FindIgnoreLock(GoodsGUID));
			if( NULL != pGoods )
			{
				GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Remove(pGoods);
				return pGoods;
			}
		}
	}

	static DWORD arrContainerType[] = 
	{
		eDCT_Primary,
		eDCT_Secondary1,
		eDCT_Secondary2,
		eDCT_Secondary3,
		eDCT_Secondary4
	};

	for (LONG i = 0; i < sizeof(arrContainerType) / sizeof(DWORD); ++i)
	{
		CAmountLimitGoodsContainer *pContainer = (CAmountLimitGoodsContainer*)m_pPlayerDepot.FindContainer(arrContainerType[i]);
		if(NULL != pContainer)
		{
			pGoods = (CGoods*)pContainer->FindIgnoreLock(GoodsGUID);
			if(NULL != pGoods)
			{
				pContainer->Remove(pGoods);
				return pGoods;
			}
		}
	}


	return NULL;
}

//! ��������б����������ӱ��������ҵ�һ����Ʒ��������������Ʒ��
CGoods* CPlayer::FindGoodsByGuidInPack(const CGUID &GoodsGUID)
{
	CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->FindIgnoreLock(GoodsGUID));
	if( NULL != pGoods )
	{
		return pGoods;
	}
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->FindIgnoreLock(GoodsGUID));
			if( NULL != pGoods )
			{
				return pGoods;
			}
		}
	}

	return NULL;
}

BOOL CPlayer::FindGoodsByGuid(const CGUID& guid,DWORD& dwContainer,DWORD& dwPos)
{
	BOOL bFound=FALSE;
	DWORD dwPosition=0;
	bFound = getOriginPack()->QueryGoodsPosition(guid,dwPosition);
	if(bFound)
	{
		dwContainer=CS2CContainerObjectMove::PEI_PACKET;
		dwPos=dwPosition;
		return TRUE;
	}

	/*
	pGoods = dynamic_cast<CGoods*>(m_cEquipment.Find(GoodsGUID));
	if( NULL != pGoods )
	{
	return pGoods;
	}	
	*/

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			bFound =GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->QueryGoodsPosition(guid,dwPosition);
			if(bFound)
			{
				dwContainer=i;
				dwPos=dwPosition;
				return TRUE;
			}
		}
	}	

	return FALSE;
}

CGoods* CPlayer::RemoveGoods(long lContainerId,long lPos)
{
	//! �ҵ�����Ʒ������
	CGoodsContainer* pCGoodsContainer = FindContainer(lContainerId);
	if( NULL != pCGoodsContainer )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( pCGoodsContainer -> GetGoods(lPos) );
		assert(NULL != pGoods);

		//##ɾ��
		CS2CContainerObjectMove comMsg;
		if( pCGoodsContainer -> Remove(pGoods, &comMsg) )
		{
			//OnLoseGoods(pGoods);
			comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
			comMsg.SetSourceContainerExtendID( lContainerId );
			comMsg.Send( GetExID() );

			//##ɾ������Ʒ?
			//GoodsSetup::GarbageCollect( &pGoods );
			return pGoods;
		}
	}
	return NULL;

	/*CGoods* pGoods=NULL;
	pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(GoodsGUID));
	if( NULL != pGoods )
	{
	DWORD dwPos=0;
	getOriginPack()->QueryGoodsPosition(pGoods,dwPos);
	getOriginPack()->Remove(dwPos,dwNum);
	return pGoods;
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
	if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
	{			
	pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Find(GoodsGUID));
	if( NULL != pGoods )
	{
	DWORD dwPos=0;
	GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->QueryGoodsPosition(pGoods,dwPos);

	GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Remove(dwPos,dwNum);
	return pGoods;
	}
	}
	}	*/

}


CGoods* CPlayer::FindGoods(LONG lContainerId,LONG lPos)
{
	CGoods* pGoods=NULL;
	CGoodsContainer* pContainer = FindContainer(lContainerId);
	if(pContainer)
	{
		pGoods=pContainer->GetGoods(lPos);
	}
	return pGoods;
}


BOOL CPlayer::DelGoods(LONG lContainerId,LONG lPos)
{
	CGoodsContainer* pContainer = FindContainer(lContainerId);
	if(pContainer)
	{
		if(pContainer->Remove(lPos,1))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CPlayer::ClearGoods(LONG lContainerId,LONG lPos)
{
	if(lContainerId==CS2CContainerObjectMove::PEI_PACKET || (lContainerId>=CS2CContainerObjectMove::PEI_PACK1 && 
		lContainerId<=CS2CContainerObjectMove::PEI_PACK5))
	{
		CGoodsContainer* pContainer = FindContainer(lContainerId);
		CVolumeLimitGoodsContainer* pVolumeContainer=dynamic_cast<CVolumeLimitGoodsContainer*>(pContainer);
		if(pVolumeContainer)
		{
			pVolumeContainer->ClearGoods(lPos);
		}
	}
}

//! �ҵ�ָ��ԭʼ����������[δ����]��Ʒ
BOOL CPlayer::FindGoods(char* strOriginName,int num)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget((char*)strOriginName );
	getOriginPack()->TraversingContainer(&lSeeker);
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]));
		if( pGoods )
		{
			if(!getOriginPack()->IsLocked(pGoods) && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
			{
				return TRUE;
			}
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			pContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					if(!pContainer->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
					{
						return TRUE;
					}
				}
			}	
		}
	}

	return FALSE;
}
//! ɾ��ָ��ԭʼ����������[δ����]��Ʒ
BOOL CPlayer::DelGoods(char* strOriginName,int num)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget((char*)strOriginName );
	getOriginPack()->TraversingContainer(&lSeeker);
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]));
		if( pGoods )
		{
			if(!getOriginPack()->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
			{
				DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], num);
				return TRUE;
			}
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			pContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					if(!pContainer->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
					{
						DeleteGoods( (CS2CContainerObjectMove::PLAYER_EXTEND_ID)i, lSeeker.m_vGoodsID[j], num);
						return TRUE;
					}
				}
			}	
		}
	}

	return FALSE;
}

//! ɾ��ָ��GUID��������[δ����]��Ʒ
BOOL CPlayer::DelGoods(const CGUID &GoodsGuid,int num)
{
	CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(GoodsGuid));
	if( pGoods )
	{
		if(!getOriginPack()->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
		{	
			return 0 != DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, GoodsGuid, num);
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;

			CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(GoodsGuid));
			if( pGoods )
			{
				if(!pContainer->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (int)pGoods->GetAmount() >= num)
				{
					return 0 != DeleteGoods( (CS2CContainerObjectMove::PLAYER_EXTEND_ID)i, GoodsGuid, num);
				}
			}	
		}
	}

	return FALSE;
}

//! �ҵ�ָ��ԭʼ����������[δ����]��Ʒ
BOOL CPlayer::FindGoodsByIndex(LONG lGoodsIdx,  LONG num)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget(lGoodsIdx);
	getOriginPack()->TraversingContainer(&lSeeker);
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]));
		if( pGoods )
		{
			if(!getOriginPack()->IsLocked(pGoods) && !IsInPersonalShop(pGoods) && (long)pGoods->GetAmount() >= num)
			{
				return TRUE;
			}
			else
			{
				num -= pGoods->GetAmount();
			}
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			pContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					if(!pContainer->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (long)pGoods->GetAmount() >= num)
					{
						return TRUE;
					}
					else
					{
						num -= pGoods->GetAmount();
					}
				}
			}	
		}
	}

	return FALSE;
}
//! ɾ��ָ��ԭʼ����������[δ����]��Ʒ
BOOL CPlayer::DelGoodsByIndex(LONG lGoodsIdx,  LONG num)
{
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget(lGoodsIdx);
	getOriginPack()->TraversingContainer(&lSeeker);
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]));
		if( pGoods )
		{
			if(!getOriginPack()->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (long)pGoods->GetAmount() >= num)
			{
				DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[i], num);
				return TRUE;
			}
		}
	}

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lSeeker.m_vGoodsID.clear();
			CVolumeLimitGoodsContainer* pContainer = GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			pContainer->TraversingContainer(&lSeeker);
			for(size_t j=0;j<lSeeker.m_vGoodsID.size();j++)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>(pContainer->Find(TYPE_GOODS, lSeeker.m_vGoodsID[j]));
				if( pGoods )
				{
					if(!pContainer->IsLocked(pGoods)  && !IsInPersonalShop(pGoods) && (long)pGoods->GetAmount() >= num)
					{
						DeleteGoods( (CS2CContainerObjectMove::PLAYER_EXTEND_ID)i, lSeeker.m_vGoodsID[j], num);
						return TRUE;
					}
				}
			}	
		}
	}

	return FALSE;
}

//! �����Խ��ɾ����Ʒ
void CPlayer::DelGoodsByTestResult(vector<tagDelTestResult> &vecDelResult)
{
	for (long i=0; i<(long)vecDelResult.size(); i++)
	{
		DelGoodsByTest(&vecDelResult[i]);
	}
}

DWORD CPlayer::FindGoods(LONG lContainerId,LONG lPos,const char* strGoodsName)
{
	if(NULL == strGoodsName)	return 0;
	DWORD dwNum=0;
	if(lContainerId==0) 
	{
		return CheckGoods(strGoodsName);
	}
	else
	{
		CGoods* pGoods=FindGoods(lContainerId,lPos);
		if(pGoods)
		{
			char *strName =(char*)(GoodsSetup::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex()));
			if(strName && strcmp(strGoodsName,strName) == 0)
				dwNum=pGoods->GetAmount();
		}
	}
	return dwNum;
}

DWORD CPlayer::FindGoodsByName(LONG lContainerId,const char* strGoodsName)
{
	CGoodsContainer* pContainer = FindContainer(lContainerId);
	if(pContainer)
	{
		return pContainer->GetGoodsAmountByName(strGoodsName);		
	}
	return 0;
}

DWORD CPlayer::FindGoodsByType(LONG lContainerId,int nType)
{
	CGoodsContainer* pContainer = FindContainer(lContainerId);
	if(pContainer)
	{
		return pContainer->GetGoodsAmountByType(nType);
	}
	return 0;
}

//������Գ�ʼ��
void CPlayer::InitProperty(void)
{
	PlayerList::SetBaseFightProperty(&m_Property,&m_BaseFightProperty);
	PlayerList::SetFightProperty(&m_Property,&m_BaseFightProperty,&m_FightProperty);
	if (GetState()==STATE_DIED)
	{
		SetHP(0);
	}
	UpdateHpMpRecoverSpeed();

	if(!GetIsRun())
		SetSpeed(GlobalSetup::GetSetup()->fPlayerWalkSpeed);
	else
		SetSpeed(GlobalSetup::GetSetup()->fPlayerRunSpeed);
}

//��ʼ�����;���ֵ
void CPlayer::InitPresentExp(void)
{
	CServerRegion* pServerRegion = dynamic_cast<CServerRegion*>(GetFather());
	if(NULL == pServerRegion)	return;

	time_t curTime;
	time(&curTime);
	//ʱ������
	long lDiffMinute = (long)(curTime-m_Property.dwLastExitGameTime)/60;
	long lCooldown = GlobalSetup::GetSetup()->dwPresentExpCooldown;
	if(lDiffMinute > 0 && lDiffMinute > lCooldown)
	{
		//��ȥ��ȴʱ��
		lDiffMinute = lDiffMinute - lCooldown;
		DWORD dwPresentExpPerMinute = PlayerList::GetPresentExpPerMinute((uchar)GetLevel());
		DWORD dwMaxPresentExp = PlayerList::GetMaxPresentExp((uchar)GetLevel());
		DWORD dwCurPresentExp = lDiffMinute*dwPresentExpPerMinute;

		float fPresentExpScaleWithPlace = 1.0f;
		//���˸���
		if( GetCurRgnType() == ::RGN_PERSONAL_HOUSE )
			fPresentExpScaleWithPlace = GlobalSetup::GetSetup()->fPresentExpScaleInPersonalHouse;
		//��ȫ��ϵ��
		else if(pServerRegion->GetSecurity(GetTileX(),GetTileY() ) == CRegion::SECURTIY_SAFE)
			fPresentExpScaleWithPlace = GlobalSetup::GetSetup()->fPresentExpScaleInSafeArea;
		else
			fPresentExpScaleWithPlace = GlobalSetup::GetSetup()->fPresentExpScale;

		dwCurPresentExp = (DWORD)(dwCurPresentExp*fPresentExpScaleWithPlace*GetPresentExpScale());

		if(m_Property.dwPresentExp+dwCurPresentExp > dwMaxPresentExp)
			m_Property.dwPresentExp = dwMaxPresentExp;
		else
			m_Property.dwPresentExp += dwCurPresentExp;

		//���;���
		m_mapDirtyAttrData.insert("PresentExp");
		UpdateAttribute();		
	}
	//�����ǰ�����;���ֵ,���һ��buff��ʾ
	if(m_Property.dwPresentExp > 0)
	{
		AddNewState(this,GlobalSetup::GetSetup()->lPresentExpBuffId,GlobalSetup::GetSetup()->lPresentExpBuffLvl,14);
	}
	//��������ʱ��
	SetLastExitGameTime((DWORD)curTime);
}

//����ս�������е����������ݡ����ʡ��������ǻۡ���������ֵ������ֵ�Ȼ�������
void CPlayer::ResetBaseFightProperty(void)
{
	PlayerList::SetBaseFightProperty(&m_Property,&m_BaseFightProperty);

	//����
	AddAttributeToDirty("C_Strenth");
	//����
	AddAttributeToDirty("C_Agility");
	//����
	AddAttributeToDirty("C_Consti");
	//����
	AddAttributeToDirty("C_Intell");
	//�ǻ�
	AddAttributeToDirty("C_Wisdom");
	//����
	AddAttributeToDirty("C_Spirit");
	//�������ֵ
	AddAttributeToDirty("C_MaxHp");
	//��ǰ����ֵ
	AddAttributeToDirty("Hp");
	//�����ֵ
	AddAttributeToDirty("C_MaxMp");
	//��ǰ����ֵ
	AddAttributeToDirty("Mp");
	//�������ֵ
	AddAttributeToDirty("C_MaxEnergy");
}


//������,����,����,����,�ǻ�,����Ļ������Ժͱ仯���Է����ı��ʱ��
//��Ҫ���¼����,�м�,���ܵȻ�������
void CPlayer::ResetFightProperty(void)
{
	//���¼���
	PlayerList::SetFightProperty(&m_Property,&m_BaseFightProperty,&m_FightProperty);
	if (GetState()==STATE_DIED)
	{
		SetHP(0);
	}
	UpdateHpMpRecoverSpeed();
	ProduceEvent(ET_ResetBaseFightProperty);
	//�������������
	//��
	m_mapDirtyAttrData.insert("C_Block");
	//�м�
	m_mapDirtyAttrData.insert("C_Parry");
	//����
	m_mapDirtyAttrData.insert("C_Dodge");
	//����������С����
	m_mapDirtyAttrData.insert("C_MinAtk");
	//����������󹥻�
	m_mapDirtyAttrData.insert("C_MaxAtk");

	m_mapDirtyAttrData.insert("Hp");
	m_mapDirtyAttrData.insert("Mp");

	AddAttributeToDirty("C_MaxHp");
	AddAttributeToDirty("C_MaxMp");

	m_mapDirtyAttrData.insert( "CurPKCP" );
	AddAttributeToDirty( "C_MaxPKCP" );

	//�������
	m_mapDirtyAttrData.insert("C_Def");
	//������ֵ
	m_mapDirtyAttrData.insert("C_Cri");
	//����ֵ
	m_mapDirtyAttrData.insert("C_Hit");
	//�ﱬ�˺�ֵ
	m_mapDirtyAttrData.insert("C_CriDamage");;
	//�ﱬ����ֵ
	m_mapDirtyAttrData.insert("C_CriDef");
	//���ʿ���ֵ
	m_mapDirtyAttrData.insert("C_ConstiDef");
	//ħ������
	m_mapDirtyAttrData.insert("C_MAtk");
	//ħ�������˺�
	m_mapDirtyAttrData.insert("C_MCriDamage");
	//��־����ֵ
	m_mapDirtyAttrData.insert("C_WillDef");
	//ħ����������ֵ
	m_mapDirtyAttrData.insert("C_MCriDef");
	//ħ������ֵ
	m_mapDirtyAttrData.insert("C_Mdef");
	//ħ������ֵ
	m_mapDirtyAttrData.insert("C_MCri");
	//��������
	m_mapDirtyAttrData.insert("UpgradeExp");
	//ְҵ��������
	m_mapDirtyAttrData.insert("OccuUpgradeExp");
	//���������ָ�ֵ
	m_mapDirtyAttrData.insert("B_EnerRecoverSpeed");
	//ս�������ָ�ֵ
	m_mapDirtyAttrData.insert("B_FightEnerRecoverSpeed");
}

void CPlayer::StudySkill(DWORD dwID,DWORD dwLv)
{
	AddSkill(dwID,dwLv);
	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwID);

	if (NULL!=pSkillProperty)
	{
		if(pSkillProperty->GetSkillType()==ModuType_Skill_Passive)
		{
			OnBeginSkill(dwID,dwLv);				
		}
	}
	StudyKillScript(dwID,dwLv);	
}
bool CPlayer::CheckStudySkill(DWORD dwID,DWORD &dwSkillLv)
{
	//DWORD dwLv=0;	
	if (this->IsDied())
	{
		//����״̬�²�����������
		return false;
	}
	if (dwSkillLv==0)
	{
		map<long,tagSkill>::iterator it = m_Skills.find(dwID);
		if (it!=m_Skills.end())
		{
			dwSkillLv = it->second.wLevel + 1;
		}
		else
		{
			dwSkillLv = 1;
		} 
	}


	//ְҵ�ȼ�����������
	if(!OccuLvJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}

	//ְҵ������ѧϰ����
	if (!OccuJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}

	//�ȼ�������ѧϰ����	
	if (!LevelJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}



	//ǰ�ü��ܲ�����ѧϰ����	
	if (!SkillJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}
	//ǰ�����Ƽ��ܲ���������
	if (!LimitSkillJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}

	//SP������ѧϰ����		
	if (!SPJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}

	//��������ѧϰ��������
	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwID);
	if (NULL!=pSkillProperty)
	{
		if (!GetInst(SkillAttribute).IsExist(dwID , dwSkillLv))
		{			
			return false;			
		}
	}
	else
	{
		return false;
	}
	//��Ʒ����
	if (!GoodJudgeStudy(dwID,dwSkillLv))
	{
		return false;
	}
	return true;//����ѧϰ
}


//ѧϰ���ܴ����ű�
void CPlayer::StudyKillScript(DWORD dwID,DWORD dwLv)
{
	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);	
	if (ptgCondition!=NULL)
	{
		for (vector<CStudySkillCondition::tagConditionEx*>::iterator it = ptgCondition->vec_ConditionEx.begin();
			it!=ptgCondition->vec_ConditionEx.end();++it)
		{
			if ((*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_SCRIPT)
			{
				PlayerRunScript((char*)(*it)->strValue.c_str());
			}
		}
	}	
}
//ְҵ�ȼ��ж�
bool CPlayer::OccuLvJudgeStudy(DWORD dwID,DWORD dwLv)
{
	char strTemp[256];
	string str="Occu";
	_snprintf(strTemp, 256, "OccuLvl%d", GetAttribute(str));
	string strOccuLv = strTemp;

	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition!=NULL)
	{
		if((long)ptgCondition->dwOccuLv >	GetAttribute(strOccuLv))
		{
			StudySkillFailed(SKILL_STUDY_FAILED_OCCULV);
			return false;
		}
		return true;
	}	
	return false;
}

//ְҵѧϰ����
bool CPlayer::OccuJudgeStudy(DWORD dwID,DWORD dwLv)
{	
	string str="Occu";
	COccupSkill* pOccupSkill = COccupSkillList::GetOccupSkill(GetAttribute(str));
	if(pOccupSkill!=NULL)
	{
		vector<COccupSkill::tagOccupSkill*> vcOccupSkill = pOccupSkill->GetOccupSkill();
		vector<COccupSkill::tagOccupSkill*>::iterator it=vcOccupSkill.begin();
		for (;it!=vcOccupSkill.end();++it)
		{
			if (((*it)->lSkillID==(long)dwID)&&((*it)->lLeve>=(long)dwLv))
			{
				return true;
			}
		}
	}

	StudySkillFailed(SKILL_STUDY_FAILED_OCCUPATION);
	return false;
}
//�ȼ�ѧϰ����	
bool CPlayer::LevelJudgeStudy(DWORD dwID,DWORD dwLv)
{
	string str="Level";
	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition!=NULL)
	{
		eOccupation Occu = (eOccupation)GetOccupation();
		long lOccuLvlCoe = (long)PlayerList::GetOccuLvlCoe(Occu,GetOccuLvl(Occu));
		if(ptgCondition->wPlayerLev >	lOccuLvlCoe )
		{
			StudySkillFailed(SKILL_STUDY_FAILED_RANK);
			return false;
		}
		return true;
	}	

	return false;
}
//SPѧϰ����		
bool CPlayer::SPJudgeStudy(DWORD dwID,DWORD dwLv)
{

	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition!=NULL)
	{
		if (ptgCondition->dwSp > GetOccuSp((eOccupation)GetOccupation()) )
		{
			StudySkillFailed(SKILL_STUDY_FAILED_SP);
			return false;
		}
		DecOccuSp((eOccupation)GetOccupation(),ptgCondition->dwSp);
		return true;
	}
	return false;


}
//ǰ�ü���ѧϰ����	
bool CPlayer::SkillJudgeStudy(DWORD dwID,DWORD dwLv)
{
	//CONDITIONEX_PRESKILL,			// ����ǰ�ü���
	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition)
	{
		vector<CStudySkillCondition::tagConditionEx*>::iterator	it=ptgCondition->vec_ConditionEx.begin();
		for (;it!=ptgCondition->vec_ConditionEx.end();++it)
		{
			if ((CStudySkillCondition::eConditionEx)(*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_PRESKILL)
			{
				DWORD dwSkillID = atol((*it)->strValue.c_str());//)
				DWORD dwSkillLv = (*it)->lValue;
				itSkill sit=m_Skills.find(dwSkillID);
				if (sit!=m_Skills.end())
				{
					if (sit->second.wLevel < dwSkillLv)
					{
						//��ǰ���ܵĵȼ�С�����Ƶȼ�
						StudySkillFailed(SKILL_STUDY_FAILED_SKILL);
						return false;
					}					
				}
				else
				{
					//û���ҵ�ǰ�ü���
					StudySkillFailed(SKILL_STUDY_FAILED_SKILL);
					return false;
				}

			}
		}
		return true;
	}

	return false;
}
//ǰ�����Ƽ���ѧϰ����
bool CPlayer::LimitSkillJudgeStudy(DWORD dwID,DWORD dwLv)
{
	//CONDITIONEX_CONFLICTSKILL		// �������Ƽ���
	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition)
	{
		vector<CStudySkillCondition::tagConditionEx*>::iterator	it=ptgCondition->vec_ConditionEx.begin();
		for (;it!=ptgCondition->vec_ConditionEx.end();++it)
		{
			if ((CStudySkillCondition::eConditionEx)(*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_CONFLICTSKILL)
			{
				DWORD dwSkillID = atol((*it)->strValue.c_str()); 			
				itSkill sit=m_Skills.find(dwSkillID);
				if (sit!=m_Skills.end())
				{
					//����ǰ�����Ƽ���
					StudySkillFailed(SKILL_STUDY_FAILED_LIMIT_SKILL);
					return false;			
				}

			}
		}
		return true;
	}

	return false;
}

//��Ʒ�����ж�
bool CPlayer::GoodJudgeStudy(DWORD dwID, DWORD dwLv)
{
	CStudySkillCondition::tagCondition *ptgCondition = CStudySkillCondition::GetCondition(dwID,(WORD)dwLv);
	if (ptgCondition)
	{
		vector<CStudySkillCondition::tagConditionEx*>::iterator	it=ptgCondition->vec_ConditionEx.begin();
		for (;it!=ptgCondition->vec_ConditionEx.end();++it)
		{
			if ((CStudySkillCondition::eConditionEx)(*it)->wConditionExType==CStudySkillCondition::CONDITIONEX_GOODS)
			{
				string strName = (*it)->strValue; 		

				if (strName == AppFrame::GetText("GS_GOODS_GOLD"))
				{
					//���������
					if((long)GetMoney()<(*it)->lValue)
					{						
						StudySkillFailed(SKILL_STUDY_FAILED_GOLD);
						return false;	
					}
					else
					{
						SetMoney(GetMoney()-(*it)->lValue,6);
					}
				} 
				else if(strName == AppFrame::GetText("GS_GOODS_SILVER"))
				{
					//�����������
					if ((long)GetSilver()<(*it)->lValue)
					{
						StudySkillFailed(SKILL_STUDY_FAILED_SILVER);
						return false;
					} 
					else
					{
						SetSilver(GetSilver()-(*it)->lValue);
					}
				}
				else
				{
					//��ͨ��Ʒ
					if((long)GetGoodsAmountByOriginName(strName.c_str()) < (*it)->lValue)
					{
						//��Ʒ
						StudySkillFailed(SKILL_STUDY_FAILED_GOOD);
						return false;			
					}
					else
					{
						DelGoods((char*)strName.c_str(),(*it)->lValue);
					}
				}

			}
		}
		return true;
	}	
	return false;

}
//ѧϰ����ʧ��
void CPlayer::StudySkillFailed(long lValue)
{

	CMessage msg(MSG_S2C_SKILL_STUDY_FAILED);
	msg.Add(lValue);
	msg.SendToPlayer(this -> GetExID(),false);
}
//���ؼ���ѧϰ�б�
void CPlayer::SendBackSkill(string strName,CGUID guidNpc)
{
	map<string,CStudySkillCondition::tagTeachSkill*> mapSkillList = CStudySkillCondition::GetTeacherSkill(); 
	map<string,CStudySkillCondition::tagTeachSkill*>::iterator it=mapSkillList.find(strName);
	if (it!=mapSkillList.end())
	{
		CMessage msg(MSG_S2C_SKILL_STUDY_BEGIN);
		msg.Add(guidNpc);
		if (CareerJudge(strName))
		{
			msg.Add((long)(it->second->lSkillList.size()));
			for(list<DWORD>::iterator sit=it->second->lSkillList.begin();sit!=it->second->lSkillList.end();++sit)
			{
				CNewSkillProperties *pNewSkillProperties = CNewSkillFactory::QueryNewSkillProperty(*sit);
				if (pNewSkillProperties)
				{
					msg.Add(pNewSkillProperties->GetUseType());
				}
				else
				{
					msg.Add((BYTE)0);	
				}				
				msg.Add(*sit);
			}
		}
		else
		{
			msg.Add((long)0);
		}

		msg.SendToPlayer(this -> GetExID(),false);
	}


}
//��ʾѧϰ���ܵ�ְҵ�ж�
bool CPlayer::CareerJudge(string strName)
{
	string str="Occu";
	long lOccuID = GetAttribute(str);
	map<string,CStudySkillCondition::tagTeachSkill*> mapSkillList = CStudySkillCondition::GetTeacherSkill(); 
	map<string,CStudySkillCondition::tagTeachSkill*>::iterator it=mapSkillList.find(strName);
	if (it!=mapSkillList.end())
	{

		for(list<DWORD>::iterator sit=it->second->lOccList.begin();sit!=it->second->lOccList.end();++sit)
		{
			if (lOccuID==(*sit))
			{
				return true;
			}
		}
	}
	return false;
}
//��Ӽ����ж�
bool CPlayer::CanAddSkill(DWORD dwID, DWORD dwLv)
{
	map<long,tagSkill>::iterator it = m_Skills.find(dwID);
	if (it!=m_Skills.end())
	{
		if (it->second.wLevel<dwLv)
		{
			//�¼Ӽ��ܵĵȼ�����ԭ�м���
			m_Skills.erase(it);
			return true;
		} 
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}
//��Ӽ���
void CPlayer::AddSkill(long id,long lv,long lCoolDown,time_t t_EndTime)
{	
	if (!CanAddSkill(id,lv))
	{
		return;
	}
	tagSkill tgSklll;

    if ( GetInst(SkillAttribute).IsExist(id,lv) )
    {
        tgSklll.dwID = id;
        tgSklll.wLevel = (WORD)lv;

        if (lCoolDown>0)
        {
            tgSklll.wCooldown = lCoolDown;
            tgSklll.dwCooldownStartTime = timeGetTime();
        } 
        else
        {
            tgSklll.wCooldown = 0;
            tgSklll.dwCooldownStartTime = 0;
        }

        tgSklll.tCooldownEndtime = t_EndTime;
        tgSklll.dwOldCooldown = GetSkillValue( id , lv, "CoolDown" );


        m_Skills[id] = tgSklll;	

        CMessage msg(MSG_S2C_PLAYER_ADDSKILL);
        msg.Add(this->GetExID());
        msg.Add((DWORD)id);
        msg.Add((WORD)lv);
        msg.Add((DWORD)tgSklll.dwOldCooldown);
        msg.Add((float)GetSkillValue( id , lv, "MinAtkDistance" ));
        msg.Add((float)GetSkillValue( id , lv, "MaxAtkDistance" ));
        msg.SendToAround(this);

        GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRSKILLLIST);

       Log4c::Trace(ROOT_MODULE,"Add Skllid %d lvl %d successful!", id , lv  );

        /// @Expter ��������
        if( m_GeniusTable.size() > 0) 
            m_GeniusAction->AddSkill(id,lv);
    }
}



LONG CPlayer::CheckSkill(long id)
{
	itSkill  it = m_Skills.find(id);
	if (it != m_Skills.end())
	{
		return it->second.wLevel;
	}
	return 0;
}

//��ҵļ��ܳ�ʼ��
void CPlayer::InitSkills()
{

	BYTE eOcc = GetOccupation();
	switch(eOcc)
	{

	case OCC_Novice://������
		break;
	case OCC_Fighter://սʿ
		break;
	case OCC_Mystic://��ʦ(���෨ʦ)
		break;
	case OCC_Rogue:	//����
		break;
	case OCC_Knight://��ʿ 
		break;
	case OCC_Warrior://��ʿ 
		break;
	case OCC_Scout://��� 
		break;
	case OCC_Craftsman://����
		break;
	case OCC_Warlock://��ʿ
		break;
	case OCC_Cleric://��ʦ
		break;
	case OCC_Paladin://ʥ��ʿ
		break;
	case OCC_DarkKnight://����ʿ
		break;
	case OCC_Gladiator://�Ƕ�ʿ 
		break;
	case OCC_Berserker://��սʿ 
		break;
	case OCC_Ranger://Ѳ�� 
		break;
	case OCC_Assassin://�̿� 
		break;
	case OCC_BladeDancer://������ 
		break;
	case OCC_Bard://����ʫ�� 
		break;
	case OCC_Sorcerer://��ʦ 
		break;
	case OCC_Necromancer://������ʦ 
		break;
	case OCC_Bishop://���� 
		break;
	case OCC_Prophet://����(��֪)
		break;
	}
}

////���ܵ�ѡ���߼�
tagSkillID CPlayer::GetDefaultAttackSkillID()
{
	//PlayerList::eOccupation eOcc = (PlayerList::eOccupation)GetOccupation();
	BYTE eOcc = GetOccupation();

	switch(eOcc)
	{
	case OCC_Novice://������
		return SKILL_BASE_ATTACK;
		break;
	case OCC_Fighter://սʿ
		break;
	case OCC_Mystic://��ʦ(���෨ʦ)
		break;
	case OCC_Rogue:	//����
		break;
	case OCC_Knight://��ʿ 
		break;
	case OCC_Warrior://��ʿ 
		break;
	case OCC_Scout://��� 
		break;
	case OCC_Craftsman://����
		break;
	case OCC_Warlock://��ʿ
		break;
	case OCC_Cleric://��ʦ
		break;
	case OCC_Paladin://ʥ��ʿ
		break;
	case OCC_DarkKnight://����ʿ
		break;
	case OCC_Gladiator://�Ƕ�ʿ 
		break;
	case OCC_Berserker://��սʿ 
		break;
	case OCC_Ranger://Ѳ�� 
		break;
	case OCC_Assassin://�̿� 
		break;
	case OCC_BladeDancer://������ 
		break;
	case OCC_Bard://����ʫ�� 
		break;
	case OCC_Sorcerer://��ʦ 
		break;
	case OCC_Necromancer://������ʦ 
		break;
	case OCC_Bishop://���� 
		break;
	case OCC_Prophet://����(��֪)
		break;

	}

	return SKILL_BASE_ATTACK;
}

//int CPlayer::GetSubpackCanPlace()
//{
//	for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
//	{
//		if(!m_pSubpackContainer->GetSubpack(i)->pSubpackContainer->IsFull())
//		{
//			return i;
//		}
//	}
//	return -1;
//}

CVolumeLimitGoodsContainer* CPlayer::GetPackContainerById(DWORD dwId)
{
	if(dwId==CS2CContainerObjectMove::PEI_PACKET)
	{
		return getOriginPack();
	}
	else if(dwId>=CS2CContainerObjectMove::PEI_PACK1 && dwId<=CS2CContainerObjectMove::PEI_PACK5)
	{
		return GetPackage()->GetSubpack(dwId-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
	}
	return NULL;
}

//���ٶȷ����ı��Ժ󴥷�
void CPlayer::OnSpeedChanged()
{
	//���ƶ�������֤
	ResetMoveValiData();
}
//��Ӧλ�øı�
void CPlayer::OnPosChanged()
{
	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		//pTeam->OnPlayerIdioinfoChange(GetExID(), GSTeam::eMIS_MaxEnergy);
		pTeam->MarkPlayerPosChange(GetExID());
	}
}

void CPlayer::OnGetGoods(DWORD dwGoodsId,CGoods* pGoods)
{

	CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(dwGoodsId);
	if(!pProperty)
		return;	

	CGoodsFactory::RunScriptOnGet(dwGoodsId,this);

	//�ǵ�����Ʒִ��
	if(GoodsSetup::QueryGoodsMaxStackNumber(dwGoodsId)==1 && pGoods)
	{
		//! �õ����󶨵���Ʒ����
		LONG lBindType = pGoods->GetAddonPropertyValues(GAP_BIND, 1);

		if(eBT_GetAndCantUntie == lBindType)//! �õ����󶨣��Ҳ��ܽ��
		{
			pGoods->ChangeAttribute(GAP_BIND, eBT_CantUntie, 1, true);
			pGoods->UpdateAttribute(GetExID());
		}
		else if (eBT_GetAndBind == lBindType)//! �õ����󶨣����Խ��
		{
			pGoods->ChangeAttribute(GAP_BIND, eBT_Bind, 1, true);
			pGoods->UpdateAttribute(GetExID());
		}

		//! ��������ʱ��
		if(TestGoodsTimeout(pGoods))
		{
			//! ɾ����Ʒ
			RegisterLimitGoodsTimerID(pGoods->GetExID(), 1);
		}
	}	
}

void CPlayer::OnLoseGoods(CGoods* pGoods)
{
	if(NULL == pGoods) return;

	pGoods->RunScriptOnLose(this);

	//! ɾ�������Ʒ�йص���ʱ��ƷTimer
	CancelLimitGoodsTimer(pGoods->GetExID());

	m_GoodsTimerHandler.UnRegister( this, pGoods->GetExID() );
}

//! ��Ӧ�ֿ���Ʒ���
void CPlayer::OnDepotGetGoods(CGoods* pGoods)
{

}
//! ��Ӧ�ֿ���Ʒʧȥ
void CPlayer::OnDepotLoseGoods(CGoods* pGoods)
{

}

BOOL CPlayer::IsEquipBusinessPack()
{
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{

		CGoods* pGoods=GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods;
		if(pGoods)
		{
			LONG lVal=pGoods->GetAddonPropertyValues(GAP_PACK_TYPE,1);
			if(lVal==SPT_BUSINESS)
				return TRUE;
		}
	}
	return FALSE;
}

//! ɾ����Ʒ��֪ͨ���
void CPlayer::DelGoodsAndSendMsg(long lContainerId, long lPos)
{
	//! �ҵ�����Ʒ������
	CGoodsContainer* pCGoodsContainer = FindContainer(lContainerId);
	if( NULL != pCGoodsContainer )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( pCGoodsContainer -> GetGoods(lPos) );
		assert(NULL != pGoods);

		//##ɾ��
		CS2CContainerObjectMove comMsg;
		if( pCGoodsContainer -> Remove(pGoods, &comMsg) )
		{
			OnLoseGoods(pGoods);
			comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
			comMsg.SetSourceContainerExtendID( lContainerId );
			comMsg.Send( GetExID() );

			//##ɾ������Ʒ?
#ifdef _GOODSLOG1_
			if(pGoods)
			{
				char pszGoodsLog[1024]="";			
				char srcGoodsGUID[50]="";
				pGoods->GetExID().tostring(srcGoodsGUID);					
				_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::DelGoodsAndSendMsg][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
					pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
			}			
#endif
			CGoodsFactory::GarbageCollect( &pGoods ,21);
		}
	}
}

long	CPlayer::RegisterInvestTimer()
{
	tagTimerVar* pVar= OBJ_CREATE(tagTimerVar);
	pVar->TimerType=ChildObjType_InvestLimit;
	time_t t;
	tm* pTm=NULL;
	t=time(NULL);
	pTm=localtime(&t);
	pTm->tm_hour=0;
	pTm->tm_min=0;
	pTm->tm_sec=0;
	time_t t0=mktime(pTm);
	long lFuture=(24*60*60)-(long)(t-t0);
	m_lInvestLimitTimeMark= (long)t+lFuture;
	long lTimerId=GameManager::GetInstance()->Schedule(this,pVar,timeGetTime()+lFuture*1000);
	return lTimerId;
}

//! Ϊ��Ʒע��һ����ʱ���¼�ID�������ظ�ID
long	CPlayer::RegisterLimitGoodsTimerID(const CGUID &GoodsGUID, ULONG dwFutureTime)
{
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar =  OBJ_CREATE(tagTimerVar);
	/*assert(NULL != pVar);*/
	((tagTimerVar*)pVar)->TimerType = ChildObjType_GoodsTimeout;

	long lTimerID = GameManager::GetInstance()->Schedule(this, pVar, dwFutureTime + timeGetTime());
	m_GoodsTimerID[GoodsGUID] = lTimerID;
	return lTimerID;
}

// ע�ᡮ�����������ݼ���ʱ��
long CPlayer::RegisterGuardianEnergyDecTimer( const CGUID &goodsID )
{
	unsigned long lRestSec = GetRestSeconds();
	// ����������Ϊ�������ӣ���Ҫ��Ϊ������ұ����ڵ����и�����Ʒ����һ����ʱ��
	lRestSec = 60 - lRestSec % 60 + lRestSec;
 
	tagTimerVar *pVar = OBJ_CREATE(tagTimerVar);
	pVar->TimerType = ChildObjType_GoodsExTimeOut;
	pVar->lvar = GoodsExTimerType::GETT_GUARDIAN_ENERGY;
	pVar->lvar1 = lRestSec * 1000;

	long lTimerID = m_GoodsTimerHandler.Register( this, goodsID, pVar->lvar1, 24 * 60 * 60 * 1000, pVar ) ;
	if( lTimerID == 0 )
	{
		OBJ_RELEASE(tagTimerVar, pVar );
	}
	return lTimerID;
}

//! ȡ��һ����Ʒ��Timer
void CPlayer::CancelLimitGoodsTimer(const CGUID &GoodsGUID)
{
	map<CGUID, long>::iterator ite = m_GoodsTimerID.find(GoodsGUID);
	if (ite != m_GoodsTimerID.end())
	{
		void* pVar = NULL;
		GameManager::GetInstance()->Cancel(ite->second, (const void**)&pVar);
		/*assert(NULL != pVar);
		GameManager::GetInstance()->FreeBlock(pVar);*/
		m_GoodsTimerID.erase(ite);
	}
}

//! ��Ӧ��ʱ����������Ʒɾ���¼�
void CPlayer::OnLimitGoodsTimeout(long lTimerID)
{
	//! �ҵ���Ӧ����ƷID
	map<CGUID, long>::iterator ite = m_GoodsTimerID.begin();
	for (; ite != m_GoodsTimerID.end(); ++ite)
	{
		if (ite->second == lTimerID) break;
	}

	//! ����ҵ�
	if (ite != m_GoodsTimerID.end())
	{
		pair<long, long> Pos;
		if(FindGoodsByGuid(ite->first, Pos))
		{
			CGoods *pGoods = FindGoodByGuid(ite->first);
			if(NULL != pGoods)
			{
				char szInfo[128] = {0};
				_snprintf(szInfo, 128, "%s[%s]%s", AppFrame::GetText("52"), pGoods->GetName(), AppFrame::GetText("53"));
				//SendNotifyMessage( szInfo,0xffff0000,0,eNOTIFYPOS_LEFT_BOTTOM);	
				SendNotifyMessage(szInfo,eNOTIFYPOS_LEFT_BOTTOM);
			}

			//! ɾ����Ʒ
			pair<long, long> GoodsPosInfo;
			if(FindGoodsByGuid(ite->first, GoodsPosInfo))
			{
				DelGoodsAndSendMsg(GoodsPosInfo.first, GoodsPosInfo.second);
			}
		}
	}
}

//void CPlayer::OnGuardianEnergyTimeOut( const CGUID &goodsID )
//{ 
//	CGoods *pGoods = FindGoodByGuid( goodsID );
//	if( pGoods != NULL )
//	{
//		long lOldValue = pGoods->GetGuardianEnergy();
//		// ���͡�����������ֵ
//		long lNewValue = pGoods->DecGuardianEnergy();
//		pGoods->UpdateAttribute( GetExID() );
//		// ������Ʒ�Ƿ����װ������Ҫ�ı�����
//		const CGoods *pEquip = m_cEquipment.GetGoods( EC_GUARDIAN );
//		if( pGoods == pEquip &&
//			lOldValue > 0 && lNewValue == 0 )
//		{
//			MountEquipEx( pGoods, false, true );	
//			UpdateAttribute( TRUE );
//		}
//	}
//}

PlayerDepot* CPlayer::GetPlayerDepot(void)
{
	return &m_pPlayerDepot;
}

//! ��Ʒ����ʱ�䴦��
//!	��δ��ʱ���ö�ʱ��������ʱ������true��Ӧ���ٽӿ��ⲿɾ����Ʒ
bool CPlayer::TestGoodsTimeout(CGoods* pGoods)
{
	//! �õ���Ʒ������ʱ������
	vector<long> vOutProperty;
	pGoods->GetAddonPropertyValues(GAP_TIME_LIMIT, vOutProperty);

	//! ���
	if (2 == vOutProperty.size())
	{
		DWORD dwValue1 = vOutProperty[0];
		DWORD dwValue2 = vOutProperty[1];

		switch(dwValue1)
		{
		case eGTL_Minute:
			{
				if (0 < dwValue2)
				{
					//! �޸���Ʒ����
					pGoods->ChangeAttribute(GAP_TIME_LIMIT, (long)eGTL_Date, 1, true);
					pGoods->ChangeAttribute(GAP_TIME_LIMIT, dwValue2 * 60 + time(NULL), 2, true);
					

					//! ����15�죬��ע��ɾ����ʱ��
					if(15 > (dwValue2 / 24 / 60))
					{
						RegisterLimitGoodsTimerID(pGoods->GetExID(), dwValue2 * 60 * 1000);
					}
				}
				else
				{
					return true;
				}
			}
			break;
		case eGTL_Date:
			{
				DWORD dwCurrTime = (DWORD)time(NULL);
				if (dwCurrTime < dwValue2)
				{
					//! ����15�죬��ע��ɾ����ʱ��
					if(15 > ((dwValue2 - dwCurrTime) / 24 / 60))
					{
						RegisterLimitGoodsTimerID(pGoods->GetExID(), (dwValue2 - dwCurrTime) * 1000);
					}
				}
				else
				{
					return true;
				}
			}
			break;
		}
	}

	return false;
}

void CPlayer::CheckGoodsTimeOut(CGoods* pGoods)
{
    if (NULL != pGoods)
    {
        //! ��������ʱ��
        if(TestGoodsTimeout(pGoods))
        {
            //! ɾ����Ʒ
            pair<long, long> GoodsPosInfo;
            if(FindGoodsByGuid(pGoods->GetExID(), GoodsPosInfo))
            {
                DelGoodsAndSendMsg(GoodsPosInfo.first, GoodsPosInfo.second);
            }
        }

        // ������������
    //    CheckGuardianEnergy( pGoods, true );
    }
}

//bool CPlayer::CheckGuardianEnergy( CGoods *pGoods, bool bLogin )
//{
//	if( pGoods->GetGoodsBaseType() != GT_GUARDIAN || 
//		!pGoods->HasAddonProperty( CGoodsBaseProperties::GAP_GUARDIAN_ENERGY ) )
//	{
//		return false;
//	} 
//
//	if( bLogin )
//	{
//		// �ս�����Ϸ��Ҫ�������ֵ�Ƿ�˥��
//		unsigned long uElapsedDays = GetElapsedDay( time( NULL ), (time_t) m_Property.dwLastExitGameTime );
//		if( uElapsedDays > 0 )
//		{
//			pGoods->DecGuardianEnergy( uElapsedDays );
//		} 
//	}
//
//	// ע��˥����ʱ��
//	RegisterGuardianEnergyDecTimer( pGoods->GetExID() );
//
//	return true;
//}

// ����
void CPlayer::SetArea(CArea* p)
{
	m_pArea=p;

	// Fox@20081112 ˢ�½�ɫ����������
	GetInst(CPlayerMan).RefreshElem(PET_AREA, this);
}

void CPlayer::SetCredit(DWORD dwAreaId,DWORD dwCredit)
{
	m_mapCredit[dwAreaId]=dwCredit;
}

void CPlayer::AddCredit(DWORD dwAreaId,DWORD dwCredit)
{	
	map<DWORD,DWORD>::iterator it=m_mapCredit.find(dwAreaId);
	//09-9-14 ������������������ж� by tanglei
	long lMaxCredit=GetInst(CCreditSystem).MaxAreaCredit();
	if(it!=m_mapCredit.end())
	{
		if ((long)(it->second+dwCredit)>lMaxCredit)
		{
			it->second=lMaxCredit;
		}
		else
			it->second+=dwCredit;
	}
	else
	{		
		m_mapCredit[dwAreaId]=dwCredit;
	}
}

void CPlayer::ChangeCredit(DWORD dwAreaId,long lCredit)
{
	map<DWORD,DWORD>::iterator it=m_mapCredit.find(dwAreaId);
	//09-9-14 ������������������ж�,�����Ӷ����޸�����ֵ by tanglei
	long lMaxCredit=GetInst(CCreditSystem).MaxAreaCredit();
	if(it!=m_mapCredit.end())
	{
		if (lCredit>lMaxCredit)
		{
			it->second=lMaxCredit;
		}
		else
			it->second=lCredit;
		if(it->second<0)
			it->second=0;		
	}
	else
	{
		if(lCredit<0)
			lCredit=0;
		m_mapCredit[dwAreaId]=lCredit;
	}
}

DWORD CPlayer::GetCredit(DWORD dwAreaId)
{
	map<DWORD,DWORD>::iterator it=m_mapCredit.find(dwAreaId);
	if(it!=m_mapCredit.end())
		return it->second;
	return 0;
}

long CPlayer::GetCreditLevel(DWORD dwAreaID)
{
	return GetInst(CCreditSystem).CreditLevel( GetCredit(dwAreaID) );
}

long CPlayer::GetOrganLevel(const char* strName)
{
	long lValue=INVILID_VALUE;
	lValue=GetAttribute(string(strName));
	if (lValue==INVILID_VALUE)
		return 0;
	return GetInst(CCreditSystem).CreditOrganLeveyByDirtyName(strName,lValue);
}

void CPlayer::AddOrganCredit(const char* strName,DWORD dwCredit)
{	
	long lValue=INVILID_VALUE;
	lValue=GetAttribute(string(strName));
	if (lValue==INVILID_VALUE)
		return;
	//�����ж�
	long MaxValue=GetInst(CCreditSystem).MaxOrganValByDirtyName(strName);
	if ((long)(lValue+dwCredit)>MaxValue)
	{
		SetAttribute(string(strName),MaxValue);
	}
	else
		SetAttribute(string(strName),lValue+dwCredit);
}

void CPlayer::ChangeOrganCredit(const char* strName,DWORD dwCredit)
{
	//�����ж�
	long MaxValue=GetInst(CCreditSystem).MaxOrganValByDirtyName(strName);
	if ( (long)dwCredit>MaxValue)
	{
		SetAttribute(string(strName),MaxValue);
	}
	else
		SetAttribute(string(strName),dwCredit);
}

long CPlayer::GetOrganCredit(DWORD dwOrganID)
{
	//�������ֶ���
	const char* strDirtyName=GetInst(CCreditSystem).GetDirtyName(dwOrganID);
	long lValue=INVILID_VALUE;
	if (strDirtyName!=NULL)
	{
		lValue=GetAttribute(strDirtyName); 
	}
	if (lValue==INVILID_VALUE)
		return 0;
	else
		return lValue;
}

//����
void CPlayer::CodeCreditToDataBlock(DBWriteSet& setWriteDB)
{
    setWriteDB.AddToByteArray((DWORD)m_mapCredit.size());
    map<DWORD,DWORD>::iterator creditIter=m_mapCredit.begin();
    for(;creditIter!=m_mapCredit.end();creditIter++)
    {
        setWriteDB.AddToByteArray(creditIter->first);
        setWriteDB.AddToByteArray(creditIter->second);
    }
}

void CPlayer::DecodeCreditFromDataBlock(DBReadSet& setReadDB)
{
    m_mapCredit.clear();
    DWORD dwCreditSize=setReadDB.GetDwordFromByteArray();
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"Decode Credit(size:%d) start.",dwCreditSize);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
    for(DWORD i=0;i<dwCreditSize;i++)
    {
        DWORD dwAreaId=setReadDB.GetDwordFromByteArray();
        DWORD dwCredit=setReadDB.GetDwordFromByteArray();
        m_mapCredit[dwAreaId]=dwCredit;
    }
}

//��Ӽ�¼
void CPlayer::AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum)
{
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsRecord.find(dwGoodsId);
	if(recordIt==m_mapLimitGoodsRecord.end())
		m_mapLimitGoodsRecord[dwGoodsId]=dwGoodsNum;
	else	
		recordIt->second+=dwGoodsNum;

	// �����������
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
}

//��ȡ��������
DWORD CPlayer::GetLimitGoodsBuyNum(DWORD dwGoodsId)
{
	DWORD dwNum=0;
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsRecord.find(dwGoodsId);
	if(recordIt!=m_mapLimitGoodsRecord.end())
		dwNum=recordIt->second;
	return dwNum;
}
BOOL CPlayer::SetInvestRecord(DWORD dwSpotId,DWORD dwInvestVal)
{	
	map<DWORD,DWORD>::iterator it=m_mapInvestRecord.find(dwSpotId);
	if(it!=m_mapInvestRecord.end())
		return FALSE;
	if(m_mapInvestRecord.size()==0)
	{
		RegisterInvestTimer();
	}
	m_mapInvestRecord[dwSpotId]=dwInvestVal;
	return TRUE;
}

DWORD CPlayer::GetInvestRecord(DWORD dwSpotId)
{
	map<DWORD,DWORD>::iterator it=m_mapInvestRecord.find(dwSpotId);
	if(it!=m_mapInvestRecord.end())
		return it->second;
	return 0;
}

void CPlayer::PushbakNpcShopSellGoods(CGoods* pGoods,DWORD dwPrice)
{
	if(!pGoods)
		return;
	CPlayer::tagShopSelledGoods stSelledGoods;
	stSelledGoods.pGoods=pGoods;
	stSelledGoods.dwSelledPrice=dwPrice;
	if(m_listNpcShopSellGoods.size()>=10)
	{
		list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();		
#ifdef _GOODSLOG1_
		if((*it).pGoods)
		{
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			(*it).pGoods->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::PushbakNpcShopSellGoods][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				(*it).pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName((*it).pGoods->GetBasePropertiesIndex()),srcGoodsGUID,(*it).pGoods->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
		}

#endif
		CGoodsFactory::GarbageCollect(&((*it).pGoods),22);
		m_listNpcShopSellGoods.erase(it);

		m_listNpcShopSellGoods.push_back(stSelledGoods);
	}
	else
	{
		m_listNpcShopSellGoods.push_back(stSelledGoods);
	}
}

CGoods* CPlayer::FetchNpcShopSellGoods(const CGUID& guid)
{
	CGoods* pGoods=NULL;
	list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();
	for(;it!=m_listNpcShopSellGoods.end();it++)
	{
		if(guid==((*it).pGoods)->GetExID())
		{
			pGoods=(*it).pGoods;
			m_listNpcShopSellGoods.erase(it);
			break;
		}
	}
	return pGoods;
}

CGoods* CPlayer::FindNpcShopSellGoods(CGUID& guid,DWORD* dwPrice)
{
	CGoods* pGoods=NULL;
	*dwPrice=0;
	list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();
	for(;it!=m_listNpcShopSellGoods.end();it++)
	{
		if(guid==((*it).pGoods)->GetExID())
		{
			pGoods=(*it).pGoods;
			*dwPrice=(*it).dwSelledPrice;			
			break;
		}
	}
	return pGoods;
}

void CPlayer::ReleaseNpcShopSellGoods()
{
	list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();
	for(;it!=m_listNpcShopSellGoods.end();it++)
	{
		CGoods* pGoods=(*it).pGoods;
		if(pGoods)
		{
#ifdef _GOODSLOG1_			
			char pszGoodsLog[1024]="";			
			char srcGoodsGUID[50]="";
			pGoods->GetExID().tostring(srcGoodsGUID);					
			_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::ReleaseNpcShopSellGoods][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
				pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
			CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
			CGoodsFactory::GarbageCollect(&((*it).pGoods),23);
		}
	}
	m_listNpcShopSellGoods.clear();
}

BOOL CPlayer::UpdateCooldown(DWORD dwGoodsId)
{
	CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(dwGoodsId);
	if(pProperty)
	{
		//��ȴʱ��		
		DWORD dwCoolDownTime=pProperty->GetAddonPropertyValue(GAP_COOLDOWN,1);
		DWORD dwCoolDownId=pProperty->GetAddonPropertyValue(GAP_COOLDOWN,2);
		if(dwCoolDownId!=0)
		{						
			tagGoodsCommonCooldown stCooldown;
			ZeroMemory(&stCooldown,sizeof(stCooldown));
			stCooldown.dwCooldownTime=dwCoolDownTime;
			stCooldown.dwCooldownStartTime=timeGetTime();
			m_mapClassIdCoolDown[dwCoolDownId]=stCooldown;
		}
		else if(dwCoolDownTime!=0)
		{
			tagGoodsCommonCooldown stCooldown;
			ZeroMemory(&stCooldown,sizeof(stCooldown));
			stCooldown.dwCooldownTime=dwCoolDownTime;
			stCooldown.dwCooldownStartTime=timeGetTime();
			m_mapGoodsIdCoolDown[dwGoodsId]=stCooldown;
		}
		return TRUE;
	}
	return FALSE;
}

//�ع�
void CPlayer::CodeNpcShopSellGoodsToDataBlock(DBWriteSet& setWriteDB)
{
#ifdef _RUNSTACKINFO1_
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code NpcShopSell Start");
#endif

    setWriteDB.AddToByteArray((DWORD)m_listNpcShopSellGoods.size());
    list<tagShopSelledGoods>::iterator sellGoodsIter= m_listNpcShopSellGoods.begin();
    for(;sellGoodsIter!=m_listNpcShopSellGoods.end();sellGoodsIter++)
    {
        CGoods* pGoods=(*sellGoodsIter).pGoods;
        if(pGoods)			
        {
            pGoods->Serialize(setWriteDB);
            setWriteDB.AddToByteArray((*sellGoodsIter).dwSelledPrice);
        }
    }
}

void CPlayer::DecodeNpcShopSellGoodsFromDataBlock(DBReadSet& setReadDB)
{
    DWORD dwSellGoodsSize=setReadDB.GetDwordFromByteArray();
#ifdef _RUNSTACKINFO1_
    char pszStatckInfo[1024];
    _snprintf(pszStatckInfo,1024,"Decode dwSellGoodsSize(Size:%d) start.",dwSellGoodsSize);
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
    for(DWORD i=0;i<dwSellGoodsSize;i++)
    {
        CGoods* pGoods = CGoodsFactory::UnserializeGoods( setReadDB,7 );
        DWORD dwSellPrice=setReadDB.GetDwordFromByteArray();
        if( pGoods )
        {
            PushbakNpcShopSellGoods(pGoods,dwSellPrice);
        }
    }
}

//! �ҵ���������
CGoodsContainer*	CPlayer::FindContainer(long lContainerId)
{
	switch( lContainerId )
	{
	case CS2CContainerObjectMove::PEI_PACKET:
		return getOriginPack();
	case CS2CContainerObjectMove::PEI_EQUIPMENT:
		return &m_cEquipment;	
    case CS2CContainerObjectMove::PEI_EQUIP_GOD:
        return &m_cEquipmentGod;
    case CS2CContainerObjectMove::PEI_EQUIP_LAND:
        return &m_cEquipmentLand;
    case CS2CContainerObjectMove::PEI_EQUIP_HUMAN:
        return &m_cEquipmentHuman;
	case CS2CContainerObjectMove::PEI_PACK:
		return GetPackage()->GetSubpackContainer();
	case CS2CContainerObjectMove::PEI_PACK1:
		return GetPackage()->GetSubpack(0)->pSubpackContainer;
	case CS2CContainerObjectMove::PEI_PACK2:
		return GetPackage()->GetSubpack(1)->pSubpackContainer;
	case CS2CContainerObjectMove::PEI_PACK3:
		return GetPackage()->GetSubpack(2)->pSubpackContainer;
	case CS2CContainerObjectMove::PEI_PACK4:
		return GetPackage()->GetSubpack(3)->pSubpackContainer;
	case CS2CContainerObjectMove::PEI_PACK5:
		return GetPackage()->GetSubpack(4)->pSubpackContainer;
	}

	return NULL;
}

//! ͨ����Ʒ�������ڱ������ӱ������ҵ���һ��һ����Ʒ����Ϣ
BOOL CPlayer::FindFirstGoodsByIndex(LONG lGoodsIndex, tagGoodsParam &GoodsParam)
{
	//! Ҫ�����������б�
	static LONG arrContainerID[] = 
	{
		CS2CContainerObjectMove::PEI_PACKET,
		CS2CContainerObjectMove::PEI_PACK1,
		CS2CContainerObjectMove::PEI_PACK2,
		CS2CContainerObjectMove::PEI_PACK3,
		CS2CContainerObjectMove::PEI_PACK4,
		CS2CContainerObjectMove::PEI_PACK5,
	};

	//! Ҫ��������������
	static long lContainerNum = sizeof(arrContainerID) / sizeof(LONG);

	CGoods						*pGoods		= NULL;
	DWORD						dwPos		= 0;
	CVolumeLimitGoodsContainer	*pContainer = NULL;

	for (LONG i = 0; i < lContainerNum; ++i)
	{
		CVolumeLimitGoodsContainer *pContainer = (CVolumeLimitGoodsContainer*)FindContainer(arrContainerID[i]);
		if (NULL != pContainer)
		{
			pGoods = pContainer->GetTheFirstGoods(lGoodsIndex, m_vecInPersonalShopGoods);
			if(NULL != pGoods)
			{
				pContainer->QueryGoodsPosition(pGoods, dwPos);
				GoodsParam.wPlace		= (WORD)arrContainerID[i];
				GoodsParam.lPos			= (BYTE)dwPos;
				GoodsParam.TargetGuid	= pGoods->GetExID();
				GoodsParam.lTargetType	= pGoods->GetGoodsBaseType();

				GoodsParam.lCoolingTime	= pGoods->GetAddonPropertyValues(GAP_COOLDOWN, 1);
				LONG lCoolingType = pGoods->GetAddonPropertyValues(GAP_COOLDOWN, 2);
				if(GoodsParam.lCoolingType != lCoolingType)
				{
					GoodsParam.lLastTime	= 0;
					GoodsParam.lCoolingType = lCoolingType;
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

//! ͨ����Ʒguid�ҵ���Ʒ����λ��
//! ����ֵ��pair<����ID����Ʒλ��>
bool CPlayer::FindGoodsByGuid(const CGUID &GoodsGUID,  pair<long, long> &ReValue)
{
	//! Ҫ�����������б�
	static long arr_lContainerID[] = {
		CS2CContainerObjectMove::PEI_PACKET,	//! ��ͨ����
		CS2CContainerObjectMove::PEI_EQUIPMENT,	//! װ����
		CS2CContainerObjectMove::PEI_PACK,		//! �ӱ�������
		CS2CContainerObjectMove::PEI_PACK1,		//! �ӱ�����λ1
		CS2CContainerObjectMove::PEI_PACK2,		//! �ӱ�����λ2
		CS2CContainerObjectMove::PEI_PACK3,		//! �ӱ�����λ3
		CS2CContainerObjectMove::PEI_PACK4,		//! �ӱ�����λ4
		CS2CContainerObjectMove::PEI_PACK5,		//! �ӱ�����λ5
	};
	//! Ҫ��������������
	static long lContainerNum = sizeof(arr_lContainerID) / sizeof(long);

	//! ��ʼ����
	for (long i = 0; i < lContainerNum; ++i)
	{
		CGoodsContainer *pContainer = FindContainer(arr_lContainerID[i]);
		if (NULL != pContainer)
		{
			DWORD dPos = 0;
			if(pContainer->QueryGoodsPosition(GoodsGUID, dPos))
			{
				ReValue.first	= arr_lContainerID[i];
				ReValue.second	= dPos;
				return true;
			}
		}
	}

	return false;
}


bool CPlayer::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::CodeToDataBlock() Start");
#endif
	//! ��ʼ����
	CMoveShape::CodeToDataBlock(setWriteDB, bExData);

	if( bExData )
	{
		setWriteDB.AddToByteArray((BYTE*)&m_Property, (long)sizeof(tagPlayerProperty));
		//setWriteDB.AddToByteArray(&m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
		setWriteDB.AddToByteArray( (char*)GetAccount());
		setWriteDB.AddToByteArray( GetTeamID());

        CodeSkillsToDataBlock(setWriteDB);
        //�츳����Ws
        CodeGeniusToDataBlock(setWriteDB);
        //�������
        CodeBrotherToDataBlock(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Buff Start");
#endif
		//##����쳣״̬
		CodeBuffToDataBlock( setWriteDB );


		setWriteDB.AddToByteArray(m_lInvestLimitTimeMark);
		setWriteDB.AddToByteArray((DWORD)m_mapInvestRecord.size());
		map<DWORD,DWORD>::iterator recordIter=m_mapInvestRecord.begin();
		for(;recordIter!=m_mapInvestRecord.end();recordIter++)
		{
			setWriteDB.AddToByteArray(recordIter->first);
			setWriteDB.AddToByteArray(recordIter->second);
		}

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Equipment Start");
#endif
		m_cEquipment.Serialize( setWriteDB, bExData );
        m_cEquipmentGod.Serialize(setWriteDB, bExData);
        m_cEquipmentLand.Serialize(setWriteDB, bExData);
        m_cEquipmentHuman.Serialize(setWriteDB, bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Medal Start");
#endif
		m_MedalContainer.CodeToDataBlock(setWriteDB);

        // ����
        m_pPackage.CodeToDataBlockForServer(setWriteDB);
        // ����
        CodeCreditToDataBlock(setWriteDB);
		//�ع�
        CodeNpcShopSellGoodsToDataBlock(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code GoodsCooldown Start");
#endif
		CodeGoodsCooldown(setWriteDB);

		setWriteDB.AddToByteArray(m_lTicket);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Depot Start");
#endif
		//! �ֿ�
		m_pPlayerDepot.CodeToDataBlock_ForServer(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code SpriteSystem Start");
#endif
		//! С����ϵͳ
		m_SpriteSystem.CodeToDataBlock(setWriteDB);
		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.CodeToDataBlock(setWriteDB);


		//VarialbeList
		//if(m_pVariableList)
		//	m_pVariableList->CodeToDataBlock(setWriteDB);
		VarList::Encode( m_VarList, setWriteDB );

		//SilenceTime
		setWriteDB.AddToByteArray(GetSilence());

		//GodMode
        setWriteDB.AddToByteArray((static_cast<char>(IsGod() ? 1:0)));

		//##ɱ��ʱ��
		if( m_dwMurdererTimeStamp )
		{
			DWORD dwMurdererTimeLeft = GlobalSetup::GetSetup()->dwOnePkCountTime - ( timeGetTime() - m_dwMurdererTimeStamp );
			if (dwMurdererTimeLeft > GlobalSetup::GetSetup()->dwOnePkCountTime)
				dwMurdererTimeLeft = GlobalSetup::GetSetup()->dwOnePkCountTime;

			setWriteDB.AddToByteArray( dwMurdererTimeLeft );
		}
		else 
		{
			setWriteDB.AddToByteArray( static_cast<DWORD>(0) );
		}

		//##ս��״̬��ʣ��ʱ��
		setWriteDB.AddToByteArray( m_lFightStateCount );

		//����Ƿ��½
		setWriteDB.AddToByteArray( (char)m_bLogin);

		//��ӳ�ս����״̬ʱ��
		//setWriteDB.AddToByteArray( m_lCityWarDiedStateTime );

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Quest Start");
#endif
		//�����������
		CodeQuestDataToDataBlock(setWriteDB);
		//���ٸ����������
		CodeMerQuestDataToDataBlock(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code OccuRecord Start");
#endif
		//��Ӹ�ְҵ����
		m_DOccuRecord.CodeToDataBlock(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code PersonalRgn Start");
#endif
		// ���˸���
		setWriteDB.AddToByteArray( GetPersonalRgnGUID());
		// ���˷���GUID
		setWriteDB.AddToByteArray( GetPersonalHouseRgnGUID());
		// ��Ӹ���
		//GameManager::GetInstance()->GetRgnManager()->RgnGuidCode(GetExID(), setWriteDB);
		// ��ǰ���˸�����
		setWriteDB.AddToByteArray( this->GetCurPRgnNum());
		// ��ǰ���鸱����
		setWriteDB.AddToByteArray( this->GetCurTRgnNum());

		//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
        CodeIncShopToDataBlock(setWriteDB);

		//! ��ǰ�ƺ�����
		setWriteDB.AddToByteArray( GetTitleType());
		// �Ƿ��һ�ε�¼
		setWriteDB.AddToByteArray( (BYTE)GetFirstLogin());

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code SysMail Start");
#endif
		//ϵͳ�ʼ��б�
		setWriteDB.AddToByteArray((long)m_mapSysMail.size());
		map<long,long>::iterator mit=m_mapSysMail.begin();
		for (; mit!=m_mapSysMail.end(); ++mit)
		{
			setWriteDB.AddToByteArray((*mit).first);
			setWriteDB.AddToByteArray((*mit).second);			
		}

		// �����б�
		GetInst(CPetCtrl).CodeToDataBlock(setWriteDB, this);

		// ��ҵ����
		m_BusinessContainer.EncodeToSave( setWriteDB );

		// ����״̬
		setWriteDB.AddToByteArray((long)m_wState);

		setWriteDB.AddToByteArray((BYTE)GetShowFashion());
		
		/// ��ӷ����ģ��,�˳�������ұ���  @Expter20090602 
		GetInst(AntiCheat::CPlayerManager).PlayerEncode(this,setWriteDB);
	}

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::CodeToDataBlock() End");
#endif
	return true;
}

bool CPlayer::CodeToDataBlock_ForClient(DBWriteSet& setWriteDB, bool bExData)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPlayer::CodeToDataBlock_ForClient(name:%s) Start",GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	CMoveShape::CodeToDataBlock_ForClient( setWriteDB, bExData );

	if( bExData )
	{
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Property Start");
#endif
		setWriteDB.AddToByteArray( (BYTE*)&m_Property, (long)sizeof(tagPlayerProperty));//-sizeof(string)*2
		//����ս������
		CodeChangeAttrToDataBlock(setWriteDB);
		setWriteDB.AddToByteArray(&m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
		m_mapDirtyAttrData.clear();
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStatckInfo,1024,"Code Skill(Num:%d) Start",GetNumSkills());
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

		SetShowFashion(GetShowFashion());
		setWriteDB.AddToByteArray(GetShowFashion());

		//##���,���ܸ���		
		CodeSkillsToDataBlock_ForClient(setWriteDB, TRUE);
        //����츳
        CodeGeniusToDataBlock_ForClient(setWriteDB,TRUE);
        // �������
        // __FOX__: �����б�ɻ���, ��Ϊ���򿪺��ѽ���ʱ��ˢ��״̬��
        CodeBrotherToDataBlock(setWriteDB);
        // ����
        CodeCreditToDataBlock(setWriteDB);
		//������ȴ
        CodeGoodsColldownForClient(setWriteDB);

#ifdef _RUNSTACKINFO1_
		_snprintf(pszStatckInfo,1024,"Code Equipment(Num:%d) Start",m_cEquipment.GetEquipAmount());
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
		////add equipments
		setWriteDB.AddToByteArray((WORD)(m_cEquipment.GetEquipAmount()));
		for(long i=0;i< (long)CEquipmentContainer::s_dwEquipmentLimit;i++)
		{
			if(m_cEquipment.GetGoods(DWORD(i)))
			{				
				m_cEquipment.GetGoods(DWORD(i))->SerializeForOldClient(setWriteDB);
				setWriteDB.AddToByteArray((WORD)i);
			}
		}

        m_cEquipmentGod.SerializeForClient(setWriteDB);
        m_cEquipmentLand.SerializeForClient(setWriteDB);
        m_cEquipmentHuman.SerializeForClient(setWriteDB);

		m_MedalContainer.CodeToDataBlock(setWriteDB);

		//����
        m_pPackage.CodeToDataBlockForClient(setWriteDB);

		setWriteDB.AddToByteArray(GetTicket());	
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code Depot Start");
#endif
		//! �ֿ�����
        // __FOX__: �����ɻ�
		m_pPlayerDepot.CodeHeadToDataBlock_ForClient(setWriteDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code OrgSys Start");
#endif
		//! ��֯ϵͳ
		CodeOrgSysToDataBlock(setWriteDB, bExData);
		//! ��ǰʹ�õĳƺ�����
		setWriteDB.AddToByteArray( m_lCurrTitleType);
		//! ��ǰ�ƺ�
		setWriteDB.AddToByteArray( m_AppendProperty.strTitle);
		//GM����
		setWriteDB.AddToByteArray((BYTE)m_bGMConcealment);
		//! С����ϵͳ
		/////////////////////////////////////////////////////////
		////! -------------ϵͳ����-------------
		SpriteSystem::CodeSetupToDataBlock(setWriteDB);

		////! -------------�������-------------
		m_SpriteSystem.CodeToDataBlock(setWriteDB);
		/////////////////////////////////////////////////////////

		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.CodeToDataBlock(setWriteDB);

		CodeQuestDataToDataBlock_ForClient(setWriteDB);
		//��Ӹ�ְҵ����
		m_DOccuRecord.CodeToDataBlockForClient(setWriteDB);
		//////////////////////////////////////////////////////////////////////////

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Code GSTeam Start");
#endif
		//! ����������õ��ͻ���
		GSTeam::SendAboutSkillToC(setWriteDB);
		//! ��ļ��Ϣ
		CodeRecruitToDataBlock_ForClient(setWriteDB);

		// Fox@20081226 �����б�
		GetInst(CPetCtrl).CodeToDataBlockForClient(setWriteDB, this);
		// ��ҵ��������
        m_BusinessContainer.CodeForClient(setWriteDB);

		if(m_Property.lMarriageStep >= GlobalSetup::GetSetup()->lShowSpouseNameStep)
			setWriteDB.AddToByteArray( (BYTE) true );
		else
			setWriteDB.AddToByteArray( (BYTE) false );
	}
	else
	{
        // __FOX__: �����ұ��˼����˵ı������Ժϲ�
		/*
		EC_HEAD,				//	1��ͷ��
		EC_NECKLACE,			//	2������
		EC_WING,				//	3�����
		EC_BODY,				//	4������
		EC_BACK,				//	5������
		EC_GLOVE,			//6������	
		EC_BOOT,			//7��Ь��
		EC_HEADGEAR,			//	8��ͷ��
		EC_FROCK,			//	9������		
		EC_LRING,			//	10�����ָ
		EC_RRING,			//	11���ҽ�ָ		
		EC_MEDAL1,			//	12��ѫ��1
		EC_MEDAL2,			//	13	ѫ��2
		EC_MEDAL3,			//	14	ѫ��3
		EC_FAIRY,			//	15��С����
		EC_WEAPON,			//	16������
		EC_WEAPON2,			//	17����������
		*/
		setWriteDB.AddToByteArray( GetHeadPic());
		setWriteDB.AddToByteArray( GetFacePic() );
		setWriteDB.AddToByteArray( GetShowFashion());
        for(long l=EC_E_HEAD; l<=EC_P_LEG; l++)
        {
            CGoods* tGoods = m_cEquipment.GetGoods(l);
            setWriteDB.AddToByteArray(tGoods?tGoods->GetBasePropertiesIndex():0 );
            setWriteDB.AddToByteArray(tGoods?tGoods->GetAddonPropertyValues(GAP_WEAPON_LEVEL, 1):0 );
        }
		

		setWriteDB.AddToByteArray( (BYTE)m_Property.bShowCountry);
		//////////////////////////////////////////////////////////////////////////
		//  [4/18/2007 chenxianjin]
		setWriteDB.AddToByteArray( GetHP());
		setWriteDB.AddToByteArray( GetMaxHP());
		setWriteDB.AddToByteArray( GetPkValue());//pkֵ
		setWriteDB.AddToByteArray( GetPVPValue());//����ֵ
		//////////////////////////////////////////////////////////////////////////

		//! ��֯ϵͳ
		CodeOrgSysToDataBlock(setWriteDB, bExData);

		//! ��ǰʹ�õĳƺ�����
		setWriteDB.AddToByteArray( m_lCurrTitleType);
		//! ��ǰ�ƺ�
		setWriteDB.AddToByteArray( m_AppendProperty.strTitle);
		//GM����
		setWriteDB.AddToByteArray((BYTE)m_bGMConcealment);

		// ְҵ+�Ա�
		//  [4/18/2007 chenxianjin]
		setWriteDB.AddToByteArray( GetOccupation());
		setWriteDB.AddToByteArray( GetSex());
		setWriteDB.AddToByteArray( GetDOccupation());

		//�����̵����Ϣ
        CodePersonalShopForClient(setWriteDB);
		//##������Ϣ
		setWriteDB.AddToByteArray( m_lEmotionIndex );
		setWriteDB.AddToByteArray( timeGetTime() - m_dwEmotonTimeStamp );
		// ����
		setWriteDB.AddToByteArray( m_Property.byCountry );
		// �ȼ�
		setWriteDB.AddToByteArray( GetLevel() );
		//! ��ļ��Ϣ
		CodeRecruitToDataBlock_ForClient(setWriteDB);
		// PK CPֵ
		setWriteDB.AddToByteArray( GetPKCP() );
		setWriteDB.AddToByteArray( GetMaxPKCP() );
        // �ƶ�Ŀ���
        setWriteDB.AddToByteArray( m_mover.GetDestX() );
        setWriteDB.AddToByteArray( m_mover.GetDestY() );
	}

#ifdef _RUNSTACKINFO1_
	_snprintf(pszStatckInfo,1024,"CPlayer::CodeToDataBlock_ForClient(name:%s) End",GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	return true;
}

void CPlayer::CodeRecruitToDataBlock_ForClient(DBWriteSet& setWriteDB)
{
	//! �����ļ״̬
	setWriteDB.AddToByteArray( (BYTE)((m_RecruitState.bIsRecruiting) ? 1 : 0) );
	//! �����ļ����
	if (m_RecruitState.bIsRecruiting)
	{
		if (NULL_GUID == GetTeamID())
		{
			setWriteDB.AddToByteArray( (BYTE)1);
		}
		else
		{
			GSTeam *pTeam = (GSTeam*)(GetOrganiCtrl()->GetGameTeamByID(GetTeamID()));
			if (NULL == pTeam)
			{
				setWriteDB.AddToByteArray( (BYTE)1);
			}
			else
			{
				setWriteDB.AddToByteArray( (BYTE)pTeam->GetMemberNum());
			}
		}
		setWriteDB.AddToByteArray( m_RecruitState.szRecruitName);
		if (0 != m_RecruitState.szPassword[0])
		{
			setWriteDB.AddToByteArray( (BYTE)1);
		}
		else
		{
			setWriteDB.AddToByteArray( (BYTE)0);
		}
	}
}

// ����
bool CPlayer::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::DecodeFromDataBlock() Start");
#endif
	ClearSkills();
	ClearAllStates();

	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);

	//##����������ӵ����
	m_cEquipment.SetOwner( GetType(), GetExID() );
    m_cEquipmentGod.SetOwner(GetType(), GetExID());
    m_cEquipmentLand.SetOwner(GetType(), GetExID());
    m_cEquipmentHuman.SetOwner(GetType(), GetExID());
	m_pPackage.SetOwner(this);	

	//! ���òֿ��ӵ����
	m_pPlayerDepot.SetOwner(this);

	//m_cEquipmentUpgrading.SetOwner( GetType(), GetExID() );

	if( bExData )
	{
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::DecodeFromDataBlock(),Decode property start.");
#endif
		char strTemp[256];
		setReadDB.GetBufferFromByteArray( &m_Property, (long)sizeof(tagPlayerProperty));
		//setReadDB.GetBufferFromByteArray( &m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
		setReadDB.GetStringFromByteArray( strTemp,256);
		SetAccount(strTemp);		
		setReadDB.GetBufferFromByteArray(m_TeamID);
        setReadDB.GetBufferFromByteArray(m_PhaseID);
        m_PhaseRest = (ePhaseRest)setReadDB.GetLongFromByteArray();

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode Skill start.");
#endif
		DecodeSkillsFromDataBlock( setReadDB );

        //�����츳
        DecodeGeniusFromDataBlock( setReadDB );

        // �������
        DecodeBrotherFromDataBlock(setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode Buff start.");
#endif
		//##��ȡ�쳣״̬����
		DecodeBuffFromDataBlock( setReadDB );

		m_lInvestLimitTimeMark=setReadDB.GetLongFromByteArray();
		DWORD dwRecordSize=setReadDB.GetDwordFromByteArray();
		m_mapInvestRecord.clear();
		for(DWORD i=0;i<dwRecordSize;i++)
		{
			DWORD dwSpotId=setReadDB.GetDwordFromByteArray();
			DWORD dwInvestValue=setReadDB.GetDwordFromByteArray();
			m_mapInvestRecord[dwSpotId]=dwInvestValue;
		}
		time_t t;
		t=time(NULL);
		if(t>m_lInvestLimitTimeMark)
		{
			m_lInvestLimitTimeMark=0;
			ClearInvestRecord();
		}		

		m_cEquipment.Unserialize( setReadDB );
        m_cEquipmentGod.Unserialize( setReadDB );
        m_cEquipmentLand.Unserialize( setReadDB );
        m_cEquipmentHuman.Unserialize( setReadDB );

		m_MedalContainer.DecodeFromDataBlock(setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode Equipment start.");
#endif
		for (long i = EC_E_HEAD; i <= EC_MEDAL3; ++i)
		{
			CGoods *pTmpGoods = m_cEquipment.GetGoods( i );
			CheckGoodsTimeOut(pTmpGoods);
		}

        m_pPackage.DecordeDataBlockFromServer(setReadDB);

        // ����
		DecodeCreditFromDataBlock(setReadDB);

        DecodeNpcShopSellGoodsFromDataBlock(setReadDB);		

        DecodeGoodsCooldown(setReadDB);

		m_lTicket=setReadDB.GetDwordFromByteArray();

#ifdef _RUNSTACKINFO1_
		_snprintf(pszStatckInfo,1024,"Decode Depot(Ticket:%d) start.",m_lTicket);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
		//! �ֿ�
		m_pPlayerDepot.DecodeFromDataBlock_FromServer(setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode SpriteSystem start.");
#endif
		//! С����ϵͳ
		m_SpriteSystem.DecodeFromDataBlock(setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode SetupOnServer start.");
#endif

		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.DecodeFromDataBlock(setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode VariableList start.");
#endif
		//VariableList
		/*OBJ_RELEASE(CVariableList, m_pVariableList);*/
		/*m_pVariableList = OBJ_CREATE(CVariableList);
		m_pVariableList->DecodeFromDataBlock( setReadDB,true);*/
		VarList::Decode( m_VarList, setReadDB );

		// Silence
		SetSilence( setReadDB.GetLongFromByteArray() );

		// GodMode
		if( setReadDB.GetCharFromByteArray() )
			God();
		else 
			God( false );

		OnUpdateMurdererSign();

		//##ʱ��
		DWORD dwMurdererTimeLeft	= setReadDB.GetDwordFromByteArray(  );
		if( dwMurdererTimeLeft && GetPkValue())
		{
			if (dwMurdererTimeLeft > GlobalSetup::GetSetup()->dwOnePkCountTime)
				dwMurdererTimeLeft = GlobalSetup::GetSetup()->dwOnePkCountTime;
			m_dwMurdererTimeStamp	= timeGetTime() - (GlobalSetup::GetSetup()->dwOnePkCountTime - dwMurdererTimeLeft);
		}

		//##ս��״̬
		m_lFightStateCount			= setReadDB.GetLongFromByteArray(  );

		////##�������
		//char strName[256] = {0};
		//DWORD dwPetsAmount = setReadDB.GetDwordFromByteArray(  );
		//for( DWORD i = 0; i < dwPetsAmount; i ++ )
		//{
		//	tagPetInformation tPet;
		//	tPet.strOriginalName	= setReadDB.GetStringFromByteArray(  strName );
		//	tPet.dwHp				= setReadDB.GetDwordFromByteArray(  );
		//	tPet.dwLevel			= setReadDB.GetDwordFromByteArray(  );
		//	tPet.dwExperience		= setReadDB.GetDwordFromByteArray(  );
		//	m_vUncreatedPets.push_back( tPet );
		//}


		//�õ���ս����״̬ʱ��
		m_bLogin = setReadDB.GetCharFromByteArray(  )==0?false:true;

		//�õ���������
		DecodeQuestDataFromDataBlock(setReadDB);
		//�õ�ٸ����������
		DecodeMerQuestDataFromDataBlock(setReadDB);

		//�õ���ְҵ����
		m_DOccuRecord.DecodeFromDataBlock(setReadDB);

		// ����
		//m_Property.byCountry = setReadDB.GetByteFromByteArray(  );
		//m_lContribute = setReadDB.GetLongFromByteArray(  );
		//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
        DecodeIncShopFromDataBlock(setReadDB);
		
		//ϵͳ�ʼ��б�
		long lNum = setReadDB.GetLongFromByteArray();
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStatckInfo,1024,"Decode SysMai(Size:%d) start.",lNum);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
		for (int i=0;i<lNum;++i)
		{
			long lID = setReadDB.GetLongFromByteArray();
			long lEndTime = setReadDB.GetLongFromByteArray();
			InsertSysMail(lID,lEndTime);
		}

		// �Ƿ��һ�ε�¼
		SetFirstLogin( (0 != setReadDB.GetByteFromByteArray()) );

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode OrgSys start.");
#endif
		//��֯ϵͳ
		DecodeOrgSysFromDataBlock(setReadDB,bExData);

		//! ��ǰ�ƺ�����
		m_lCurrTitleType = setReadDB.GetLongFromByteArray();
		UpdateTitle(FALSE);

		CGUID tGUID;
		CGUID PHGUID;
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Decode PersonalRgn start.");
#endif
		// ���˸���GUID
		setReadDB.GetBufferFromByteArray( tGUID);
		SetPersonalRgnGUID(tGUID);
		// ���˷���GUID
		setReadDB.GetBufferFromByteArray( PHGUID);
		SetPersonalHouseRgnGUID(PHGUID);
		// ��Ӹ���GUID
		//GameManager::GetInstance()->GetRgnManager()->RgnGuidDecode(GetExID(), setReadDB);

		// ��ǰ���˸�����
		SetCurPRgnNum(setReadDB.GetLongFromByteArray());
		// ��ǰ���鸱����
		SetCurTRgnNum(setReadDB.GetLongFromByteArray());

		GetInst(CPetCtrl).DecodeFromDataBlock(setReadDB, this);

		// ������ҵ����
		m_BusinessContainer.DecodeFromDB( setReadDB );

		// ����״̬
		m_wState = (WORD)setReadDB.GetLongFromByteArray();

		SetShowFashion(setReadDB.GetByteFromByteArray());

		/// ����ҽ����������ģ��,Expter 20090602
		GetInst(AntiCheat::CPlayerManager).PlayerDecode(this,setReadDB);

	}
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::DecodeFromDataBlock() End");
#endif
	return true;
}

bool CPlayer::DecodeOrgSysFromDataBlock(DBReadSet& setReadDB,bool bExData)
{
	setReadDB.GetBufferFromByteArray( m_TeamID);
	setReadDB.GetBufferFromByteArray( m_FactionID);
	GetOrganiCtrl()->UpdateFacInfo(this);
	setReadDB.GetBufferFromByteArray( m_UnionID);
	m_lFacDemiseWarning = setReadDB.GetLongFromByteArray();
	m_lFacDisbandWarning = setReadDB.GetLongFromByteArray();

	return true;
}

bool CPlayer::CodeOrgSysToDataBlock(DBWriteSet& setWriteDB,bool bExData)
{
	if(bExData)
		GetOrganiCtrl()->SendInitFacDataToPlayer(GetExID());
	setWriteDB.AddToByteArray(GetFactionID());
	setWriteDB.AddToByteArray(GetUnionID());
	if (NULL != m_pFacMemInfo)
	{
		setWriteDB.AddToByteArray( (BYTE)TRUE);
		if(bExData)
		{
			setWriteDB.AddToByteArray(m_lFacDemiseWarning);
			setWriteDB.AddToByteArray(m_lFacDisbandWarning);
		}
		setWriteDB.AddToByteArray( m_pFacMemInfo->szTitle);
		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(GetFactionID());
		assert(NULL != pFaction);
		if (NULL != pFaction)
		{
			setWriteDB.AddToByteArray(pFaction->GetIconCRC32());
			if(bExData)
			{
				pFaction->AddPublicData_ToDataBlock(setWriteDB);
				setWriteDB.AddToByteArray( pFaction->GetMemberJobLvl(GetExID()));
				setWriteDB.AddToByteArray( pFaction->GetPurview(GetExID()));
				const tagOrgPronounceWord& PronounceWord = pFaction->GetPronounceWord();
				setWriteDB.AddToByteArray( PronounceWord.szBody);
			}	
			else
			{
				setWriteDB.AddToByteArray(pFaction->GetName());
			}
		}	
	}
	else
	{
		setWriteDB.AddToByteArray( (BYTE)FALSE);
	}

	if(bExData)
		GetOrganiCtrl()->SendPlayerUnionData(this, GetUnionID());

	return true;
}

void CPlayer::CodeGoodsCooldown(DBWriteSet& setWriteDB, bool bExData/* =true */)
{
    map<DWORD,DWORD> mapCooldownData;
    mapCooldownData.clear();

    map<DWORD,tagGoodsCommonCooldown>::iterator classIdIter=m_mapClassIdCoolDown.begin();
    //������ȴ
    DWORD dwNowTime=timeGetTime();
    for(;classIdIter!=m_mapClassIdCoolDown.end();classIdIter++)
    {
        DWORD dwElapse=dwNowTime-classIdIter->second.dwCooldownStartTime;
        if(dwElapse < classIdIter->second.dwCooldownTime)
        {
            mapCooldownData[classIdIter->first]=classIdIter->second.dwCooldownTime - dwElapse;
        }
    }

    setWriteDB.AddToByteArray((DWORD)mapCooldownData.size());
    map<DWORD,DWORD>::iterator dataIter=mapCooldownData.begin();
    for(;dataIter!=mapCooldownData.end();dataIter++)
    {
        setWriteDB.AddToByteArray(dataIter->first);
        setWriteDB.AddToByteArray(dataIter->second);
    }
    

	//GOODS ID
	mapCooldownData.clear();
	map<DWORD,tagGoodsCommonCooldown>::iterator goodsIdIter=m_mapGoodsIdCoolDown.begin();
	for(;goodsIdIter!=m_mapGoodsIdCoolDown.end();goodsIdIter++)
	{
		DWORD dwElapse=dwNowTime-goodsIdIter->second.dwCooldownStartTime;
		if(dwElapse < goodsIdIter->second.dwCooldownTime)
		{
			mapCooldownData[goodsIdIter->first]=goodsIdIter->second.dwCooldownTime - dwElapse;
		}
	}
	setWriteDB.AddToByteArray((DWORD)mapCooldownData.size());
	dataIter=mapCooldownData.begin();
	for(;dataIter!=mapCooldownData.end();dataIter++)
	{
		setWriteDB.AddToByteArray(dataIter->first);
		setWriteDB.AddToByteArray(dataIter->second);
	}
}

void CPlayer::DecodeGoodsCooldown(DBReadSet& setReadDB)
{
    m_mapClassIdCoolDown.clear();
    DWORD dwCooldownSize=setReadDB.GetDwordFromByteArray();
    DWORD dwCooldownId=0,dwCooldownTime=0;
    tagGoodsCommonCooldown stCooldown;
    DWORD dwNowTime=timeGetTime();
#ifdef _RUNSTACKINFO1_
    char pszStatckInfo[1024];
    _snprintf(pszStatckInfo,1024,"Decode dwCooldownSize(Size:%d) start.",dwCooldownSize);
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
    for(DWORD i=0;i<dwCooldownSize;i++)
    {
        dwCooldownId=setReadDB.GetDwordFromByteArray();
        dwCooldownTime=setReadDB.GetDwordFromByteArray();
        ZeroMemory(&stCooldown,sizeof(tagGoodsCommonCooldown));
        stCooldown.dwCooldownStartTime=dwNowTime;
        stCooldown.dwCooldownTime=dwCooldownTime;
        m_mapClassIdCoolDown[dwCooldownId]=stCooldown;
    }

    m_mapGoodsIdCoolDown.clear();
    dwCooldownSize=setReadDB.GetDwordFromByteArray();
#ifdef _RUNSTACKINFO1_
    _snprintf(pszStatckInfo,1024,"Decode CooldownSize(Size:%d) start.",dwCooldownSize);
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
    for(DWORD i=0;i<dwCooldownSize;i++)
    {
        dwCooldownId=setReadDB.GetDwordFromByteArray();
        dwCooldownTime=setReadDB.GetDwordFromByteArray();
        ZeroMemory(&stCooldown,sizeof(tagGoodsCommonCooldown));
        stCooldown.dwCooldownStartTime=dwNowTime;
        stCooldown.dwCooldownTime=dwCooldownTime;
        m_mapGoodsIdCoolDown[dwCooldownId]=stCooldown;
    }
}
//������ȴ
void CPlayer::CodeGoodsColldownForClient(DBWriteSet& setWriteDB)
{
    map<DWORD,tagGoodsCommonCooldown>::iterator cooldownClassIt=m_mapClassIdCoolDown.begin();
    setWriteDB.AddToByteArray((DWORD)m_mapClassIdCoolDown.size());
    for(;cooldownClassIt!=m_mapClassIdCoolDown.end();cooldownClassIt++)
    {
        //����ID
        setWriteDB.AddToByteArray((DWORD)cooldownClassIt->first);
        //ʣ�������
        setWriteDB.AddToByteArray((DWORD)cooldownClassIt->second.dwCooldownTime);
    }		
    //GOODS ID ��ȴ
    map<DWORD,tagGoodsCommonCooldown>::iterator cooldownGoodsIt=m_mapGoodsIdCoolDown.begin();
    setWriteDB.AddToByteArray((DWORD)m_mapGoodsIdCoolDown.size());
    for(;cooldownGoodsIt!=m_mapGoodsIdCoolDown.end();cooldownGoodsIt++)
    {
        //GOODS ID
        setWriteDB.AddToByteArray((DWORD)cooldownGoodsIt->first);
        //ʣ�������
        setWriteDB.AddToByteArray((DWORD)cooldownGoodsIt->second.dwCooldownTime);
    }
}

//�ƶ�ʱ����
void CPlayer::OnMove(float fNewX, float fNewY)
{
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnMove() Start.");
#endif
#ifdef _DEBUG

	//char str1[128]="";
	//_snprintf(str1, 128, "----------------------��ǰ�ٶ�ֵ:%f----------------x����:%f,y����:%f-----------\n",GetSpeed(),fNewX,fNewY);
	//OutputDebugString(str1);
#endif
	CMoveShape::OnMove(fNewX,fNewY);
	//if( !m_FightPets.empty() )
	//{
	//	CheckPetsMove();	// �������ƶ�
	//}

	//����л���
	CheckSwitchPoint();
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnMove() End.");
#endif
}
// ���󸴻�(0:��ȫ�س� 1:ԭ�ظ���)
void CPlayer::OnRelive(long lType,long lRecoverRatio)
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(GetFather());
	if(NULL == pRegion || lRecoverRatio<=0)
		return;				
	//�����ɫ����
	if( IsDied() )
	{
		long lHp = GetMaxHP()*lRecoverRatio/100;
		long lMp = GetMaxMP()*lRecoverRatio/100;
		if (lHp<1)
		{
			lHp = 1;
		}
		if (lMp<1)
		{
			lMp = 1;
		}
		
        /// ת��Ϊ��ƽ״̬
        AI_EVENT_SENDER( m_pAI ).Relive();      

		ChangeStateTo(STATE_PEACE);
		SetAttribute("Hp",lHp);
		SetAttribute("Mp",lMp);
		UpdateAttribute();
		SetAction(CShape::ACT_STAND);
		
		//StartUpPassiveSkill();
		SetPos(0);

		UnRegisterReliveTimerID();
		FreeReliveScript();

		// ����ʱ�����ű�
		PlayerRunScript(GlobalSetup::GetSetup()->szReliveScriptName);

		long region, l, t, r, b, d;
		CServerRegion* pRegion = (CServerRegion*)GetFather();
		CServerRegion* pReturnRegion = NULL;

		//���ŵ�����������ŵ� && 
		//  [9/7/2009 chenxianj]
		//�������������pkֵ����18000ʱ�ص����ŵ�
		long lJailLandID = GetCountryParam()->GetCountryJailIsLandRegionID(GetCountry());
		if ( ( (long)GetPkValue()>=GlobalSetup::GetSetup()->lPrisonIslandPkValue && lType!=1) 
			|| (lJailLandID==pRegion->GetID() && 0 != lJailLandID) )
		{
			//�ȸ���			
			CMessage msg(MSG_S2C_PLAYER_ANSWER_RELIVE);
			msg.Add(GetAction());
			msg.Add(GetState());
			msg.Add(GetHP());
			msg.Add(GetMP());
			msg.SendToPlayer(GetExID());

			// ֪ͨ��Χ���
			CMessage msg2(MSG_S2C_SHAPE_RELIVE);
			msg2.Add(GetType());
			msg2.Add(GetExID());
			msg2.Add(GetAction());
			msg2.Add(GetState());
			msg2.SendToAround(this, this);		

			ChangeRgnToIsland();
			return;
		}


		if (lType==1) // ԭ�ظ���
		{
			region = pRegion->GetID();
			l = r = GetTileX();
			t = b = GetTileY();
			d = GetDir();
			//�ȸ���			
			CMessage msg(MSG_S2C_PLAYER_ANSWER_RELIVE);
			msg.Add(GetAction());
			msg.Add(GetState());
			msg.Add(GetHP());
			msg.Add(GetMP());
			msg.SendToPlayer(GetExID());

			// ֪ͨ��Χ���
			CMessage msg2(MSG_S2C_SHAPE_RELIVE);
			msg2.Add(GetType());
			msg2.Add(GetExID());
			msg2.Add(GetAction());
			msg2.Add(GetState());
			msg2.SendToAround(this, this);					
		}
		else //����س�
		{
			if(pRegion)
			{
				if(ChangeRgnToVillage())
				{
					//�ȸ���			
					CMessage msg(MSG_S2C_PLAYER_ANSWER_RELIVE);
					msg.Add(GetAction());
					msg.Add(GetState());
					msg.Add(GetHP());
					msg.Add(GetMP());
					msg.SendToPlayer(GetExID());

					// ֪ͨ��Χ���
					CMessage msg2(MSG_S2C_SHAPE_RELIVE);
					msg2.Add(GetType());
					msg2.Add(GetExID());
					msg2.Add(GetAction());
					msg2.Add(GetState());
					msg2.SendToAround(this, this);	
					return;
				}
				// �س��¹���
				region = l = t = r = b = d = -1;
				// ָ����ͼ����
				if(pRegion->GetRgnType() == RGN_NORMAL)
				{
					if(pRegion->GetSetup().bReLiveIsUsed)
						pRegion->GetReLiveReturnPoint(this, region, 1, l, t, r, b, d);

					// �жϸ��������Ƿ�Ϊ��ͨ����
					if(pRegion->GetSecurity((int)GetPosX(), (int)GetPosY()) == CRegion::SECURITY_FREE) // ��ͨ�������һ�����õ�Debuff
					{
						AddNewState(this, GlobalSetup::GetSetup()->lReliveDebuffId, GlobalSetup::GetSetup()->lReliveDebuffLevel,15);
					}
				}
				else // ��������
				{
					if(pRegion->GetSetup().bDupRgnIsUsed)
						pRegion->GetDupRgnReturnPoint(this, region, l, t, r, b, d);
				}

				if(region == 0) // ԭ��������
				{
					SetRegionID(pRegion->GetID());
					SetRegionGUID(pRegion->GetExID());
					SetCurRgnType((eRgnType)RGN_NORMAL);
				}
				else if(region == -1) // �ع�������
				{
					BYTE byCountry = GetCountry();
					long lCityRegion = GetCountryParam()->GetCountryMainRegionID(byCountry);
					l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
					t = GetCountryParam()->GetCountryMainRectTop(byCountry);
					r = GetCountryParam()->GetCountryMainRectRight(byCountry);
					b = GetCountryParam()->GetCountryMainRectBottom(byCountry);

					pReturnRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, lCityRegion);
					if(!pReturnRegion)
					{
						ChangeRegion(RGN_NORMAL, NULL_GUID, l, t, GetDir(), lCityRegion);
						return;
					}

					SetRegionID(lCityRegion);
					SetRegionGUID(pReturnRegion->GetExID());
					SetCurRgnType((eRgnType)RGN_NORMAL);
				}
				else if(region > 0) // �ƶ���������
				{
					pReturnRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, region);
					if(!pReturnRegion)
					{
						ChangeRegion(RGN_NORMAL, NULL_GUID, l, t, GetDir(), region);
						return;
					}
					SetRegionID(region);
					SetRegionGUID(pReturnRegion->GetExID());
					SetCurRgnType((eRgnType)RGN_NORMAL);
				}
			}
		}

		if (pReturnRegion==NULL) // ��ԭ�ظ��� �� δ�ҵ����õĸ����
		{
			return;
		}

		//##�ָ�״̬
		EnterResidentState();

		//�л�����
		if( ChangeRegion((eRgnType)pReturnRegion->GetRgnType(),pReturnRegion->GetExID(), l, t, GetDir()) == true)
		{
			CMessage msg(MSG_S2C_PLAYER_ANSWER_RELIVE);
			msg.Add(GetAction());
			msg.Add(GetState());
			msg.Add(GetHP());
			msg.Add(GetMP());
			msg.SendToPlayer(GetExID());
		}
	}
	else
	{
		//����
	}
}

// �����л�����
void CPlayer::PetsChangeRegionLocal(INIT_TYPE lInitType)
{
	//if( !m_FightPets.empty() )
	//{
	//	ITR_ACT_PET itrActPet = m_FightPets.begin();
	//	CPet* pPet = NULL;
	//	while( itrActPet != m_FightPets.end() )
	//	{
	//		pPet = GetActivePet(itrActPet->first);
	//		if( pPet != NULL )
	//		{
	//			++itrActPet;
	//			pPet->SetTarget(NULL);
	//			pPet->ModifyPos(lInitType);
	//		}
	//		else
	//		{
	//			m_FightPets.erase(itrActPet++);
	//		}
	//	}
	//}
}

// �л�����
bool CPlayer::ChangeRegion(eRgnType type, const CGUID& lRegionID, long x, long y, long dir, long templateID, long lUseGoods, long lRange, long lCarDistance)
{
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::ChangeRegion() Start.");
#endif
    // �л�����ʱֹͣ�ƶ��߼�
    m_mover.OnSetPos( x, y );

	/*
	if (GetCurrentProgress()!=PROGRESS_NONE)
		return false;*/
	if(GetCurrentProgress()==PROGRESS_INCREMENT)
	{
		SetCurrentProgress( CPlayer::PROGRESS_NONE );
		SetMoveable();
		CMessage msg( MSG_S2C_INCREMENTSHOP_CLOSE );
		msg.SendToPlayer( GetExID() );
	}
	else if(GetCurrentProgress()==PROGRESS_TRADING)
	{		
		CSession* pSession = const_cast<CSession*>( CSessionFactory::QuerySession(m_TradeSessionId) );
		if(pSession)
		{
			CTrader* pPlug = dynamic_cast<CTrader*>( pSession -> QueryPlugByOwner( 
				GetType(), GetExID() ) );
			if( pPlug && pPlug -> Get32ID() == m_lTradePlugId )
			{
				if( pSession -> IsSessionAvailable() )
				{
					pSession -> Abort();
				}
			}
		}
	}
	else if( GetCurrentProgress() == PROGRESS_FBUSINESS )
	{
		SetCurrentProgress( PROGRESS_NONE );
		SetMoveable();
		GetInst( FBusiness::CShopManager ).PlayerCloseShop( this );
	}
    /// ������λ�����л�����
    else if ( GetPhaseID() != NULL_GUID )
    {
        return false;
    }

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL || !this->CanMove())
	{
		return false;
	}

	//�л�����ʱ��,����һ���ٶ���֤
	ResetMoveValiData();

	LONG lDir	= dir;
	LONG lX		= x;
	LONG lY		= y;
	LONG lOldX,lOldY;
	lOldX = GetTileX();
	lOldY = GetTileY();

	if( lDir < DIR_UP || lDir > DIR_LEFTUP )
	{
		lDir = random( DIR_LEFTUP - DIR_UP + 1 );
	}

	// ����������ʱ��
	CancelRgnKickPlayerTimer();

	//����������������
	EndAllActiveSkill();

	//##�������ͬһ��������ֻ�ı����λ��
	if( lRegionID == pRegion -> GetExID() )
	{
		//##���������
		if( lX == -1 && lY == -1 )
		{
			pRegion -> GetRandomPos( lX, lY );
		}
		else if( lRange > 0 )
		{
			pRegion -> GetRandomPosInRange( lX, lY, lX - lRange, lY - lRange, 
				lRange * 2 + 1, lRange * 2 + 1 );
		}

		if( lX != GetTileX() || lY != GetTileY() )
		{
			CMessage Msg( MSG_S2C_SHAPE_SETPOS );
			Msg.Add( GetType() );
			Msg.Add( GetExID() );
			Msg.Add( (float)lX );
			Msg.Add( (float)lY );
			Msg.Add( lUseGoods );
			Msg.SendToAround( this );

			SetPosXY( lX+0.5f, lY+0.5f );
		}

		if( lDir != GetDir() )
		{
			SetDir( lDir );

			CMessage Msg( MSG_S2C_SHAPE_CHANGEDIR );
			Msg.Add( static_cast<BYTE>(lDir) );
			Msg.Add( GetDirEx());
			Msg.Add( GetType() );
			Msg.Add( GetExID() );

			Msg.SendToAround( this );
		}
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::ChangeRegion() End1.");
#endif
		GetLogicLogInterface()->logT440_change_map_log(this, pRegion->GetID(), templateID, 1);
		PetsChangeRegionLocal(IT_SAME_RGN);
		return true;
	}

	//##�л����� �˳����ڴ˷�������
	// test [071127 AHC]
	CServerRegion * pNewRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(type, lRegionID);
	if( pNewRegion != NULL )
	{
		// ���������
		if( lX == -1 && lY == -1 )
		{
			pNewRegion -> GetRandomPos( lX, lY );
		}
		else if( lRange > 0 )
		{
			pNewRegion -> GetRandomPosInRange( lX, lY, lX - lRange, lY - lRange, lRange * 2, lRange * 2 );
		}

		// ������Ҫ��ͻ��˽����л�
		CMessage Msg( MSG_S2C_RGN_CHANGE );
		Msg.Add( GetType());
		Msg.Add( GetExID() );
		Msg.Add( lUseGoods );
		Msg.Add( pNewRegion->GetExID() ); // ��ͨ��������RgnID
		Msg.Add( pNewRegion->GetID() ); // ������������OwnerID
		Msg.Add( lX );
		Msg.Add( lY );
		Msg.Add( lDir );
		Msg.Add( (char*)pNewRegion->GetName() );
		Msg.Add(pNewRegion->GetTaxRate());
		Msg.Add((BYTE)pNewRegion->GetWarType());
		Msg.Add((BYTE)pNewRegion->GetSpaceType());
		Msg.Add((BYTE)pNewRegion->IsChangeEquip());
		Msg.Add(pNewRegion->GetResourceID());
		Msg.Add(pNewRegion->GetExpScale());
		Msg.Add((BYTE)((CServerRegion*)pNewRegion)->IsNoPk());
		DBWriteSet setWriteDB;
		Msg.GetDBWriteSet(setWriteDB);
		GameManager::GetInstance()->GetRgnManager()->AddLinedRgnInfoToByteArray(pNewRegion->GetLinedIdFlag(), setWriteDB);
		// ���͵�ǰ�����ľݵ���Ϣ
		Msg.Add((BYTE)pNewRegion->GetStrongPointFlag()); // �ݵ�ս�Ƿ���
		if(pNewRegion->GetStrongPointFlag() == false) // δ�����ݵ�ս
		{
			Msg.Add((long)0); // �ݵ����
		}
		else
		{
			Msg.Add((long)pNewRegion->GetStronPointNum()); // �ݵ����
			std::vector<CServerRegion::tagStrongPoint>::iterator spItr = pNewRegion->GetStrongPointVec().begin();
			for(; spItr != pNewRegion->GetStrongPointVec().end(); spItr++)
			{
				Msg.Add((*spItr).szName);//�ݵ�����
				Msg.Add((long)(*spItr).lPosX); //�ݵ�X����
				Msg.Add((long)(*spItr).lPosY); //�ݵ�Y����
				Msg.Add((long)(*spItr).lState); //�ݵ�״̬
			}
		}
		Msg.SendToAround( this );
		//�������¼�
		OnExitRegion();
		//Player�˳���������
		pRegion->PlayerExit(this);

		// ���ø��׳���
		SetFather(pNewRegion);
		SetRegionGUID(pNewRegion->GetExID());
		SetRegionID(pNewRegion->GetID());
		SetNextRegionID(pNewRegion->GetID());
		SetCurRgnType((eRgnType)pNewRegion->GetRgnType());
		//��������
		CShape::SetPosXY(lX+0.5f,lY+0.5f);

		//Player���볡������
		pNewRegion->PlayerEnter(this,lUseGoods);

		// Fox@20081110 ˢ�½�ɫ����������
		GetInst(CPlayerMan).RefreshElem(PET_REGION, this);
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::ChangeRegion() End2.");
#endif
		GetLogicLogInterface()->logT440_change_map_log(this, pRegion->GetID(), templateID, 2);
		PetsChangeRegionLocal(IT_SAME_GS);
		return true;
	}

	// ����ң�������з�����������Ƿ�ܾ��˴���
	GetInst( AntiCheat::CPlayerManager ).CheckRefuseAnswer( GetExID(), true );

	////�������ڴ˷������ϣ�����������
	//SetAction(ACT_STAND);
	if(GetState() != STATE_DIED)
	SetState(STATE_PEACE);

	tagPCR ChangePlayer = {type,templateID,lRegionID,lX,lY,lDir,lUseGoods,lRange};
	GetGame()->PushChangeRgnPlayer( GetExID(),ChangePlayer);
	GetLogicLogInterface()->logT440_change_map_log(this, pRegion->GetID(), templateID, 3);
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::ChangeRegion() End3.");
#endif
	return false;
}

void CPlayer::SetPosition( LONG lX, LONG lY )
{
	CMoveShape::SetPosition(lX,lY);

	ResetMoveValiData();
}

void CPlayer::SetSlipPos( LONG lX, LONG lY,float fSpeed ,float fDir)
{
	CMoveShape::SetSlipPos(lX,lY,fSpeed,fDir);

	ResetMoveValiData();
}


BOOL CPlayer::CheckSwitchPoint()
{
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::CheckSwitchPoint() Start.");
#endif
	CRegion* pRegion = dynamic_cast<CRegion*>( GetFather() );
	if( pRegion )
	{
		CRegion::tagSwitch* pSwitchPoint = pRegion -> GetSwitch( GetTileX() , GetTileY() );
		if( pSwitchPoint )
		{
			switch( pSwitchPoint -> lState )
			{
				//##ֱ���л�
			case CRegion::CPT_NORMAL:
				{

					// �жϵȼ��Ƿ��㹻 soft 2006.9.19
					long lEenterLvl = CRegionSetup::GetEnterRegionLvl(pSwitchPoint->lRegionID);
					if (GetLevel() < lEenterLvl)
					{
						// ��������
						//long x = GetTileX();
						//long y = GetTileY();
						//long dir = GetRearDir();
						//for (int i=0; i<3; i++)
						//{
						//	x += _dir[dir][0];
						//	y += _dir[dir][1];
						//}
						//// [071127 AHC REGION]
						//CGUID returnRgn = NULL_GUID;
						//CServerRegion* pRetRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, GetRegionID());
						//if(pRetRgn)
						//	returnRgn = pRetRgn->GetExID();
						//ChangeRegion(RGN_NORMAL, returnRgn, x, y, GetDir(), 0, 1);
						// ����ʾ
						CHAR szMsg[256];
						_snprintf( szMsg, 256, "[Colour 'ffff0000']�㻹���� %d ������ʱ�޷����������", lEenterLvl);
						//SendNotifyMessage( szMsg, 0xffff0000);
						SendNotifyMessage(szMsg);
						return FALSE;
					}
					// [071127 AHC REGION]
					CGUID returnRgn = NULL_GUID;
					CServerRegion* pRetRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, pSwitchPoint->lRegionID);
					if(pRetRgn)
						returnRgn = pRetRgn->GetExID();
					ChangeRegion(RGN_NORMAL, returnRgn, pSwitchPoint -> lCoordX, 
						pSwitchPoint -> lCoordY, pSwitchPoint -> lDir,pSwitchPoint->lRegionID);
				}
				break;

				//##����л�
			case CRegion::CPT_RANDOM:
				break;

				//##�ű�����
			case CRegion::CPT_SCRIPT:
				{
					//stRunScript sScript;
					//sScript.desShape = NULL;
					//sScript.pRegion = pRegion;
					//sScript.srcShape	= this;

					char strScript[256];
					_snprintf( strScript, 256, "scripts/regions/%d.lua", pSwitchPoint -> lRegionID );
					//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
					Script::RunEnv env( this, NULL, pRegion );
					GetInst( ScriptSys ).RunScript( env, strScript );
				}
				break;
			}
			return TRUE;
		}
	}
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::CheckSwitchPoint() End.");
#endif
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//	ս��
//////////////////////////////////////////////////////////////////////////

void CPlayer::OnEquipmentWaste( eEquipType ec, CGoods* pEquipment )
{
	if( pEquipment )
	{
		if( m_cEquipment.GetGoods(ec) == pEquipment )
		{
			LONG lPreviousDurability = pEquipment -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
			//##����������ģ���ʱ������
			if( lPreviousDurability > 0 && CGoodsFactory::EquipmentWaste(pEquipment) )
			{
				LONG lCurrentDurability = pEquipment -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
				if( lCurrentDurability > 0 )
				{
					if( lPreviousDurability / 100 != lCurrentDurability / 100 )
					{
						CMessage msg( MSG_S2C_EQUIPDURUPDATE );
						msg.Add( static_cast<BYTE>(ec) );
						msg.Add( static_cast<DWORD>(lCurrentDurability) );
						msg.SendToPlayer( GetExID() );
					}
				}
				else
				{
                    /** 
                     *  �µ�װ���;õ�������
                     *  ���ڼ���͵���װ��������
                     */
                    pEquipment->RunScriptOnRemoveEquip(this);
                    MountEquipEx(pEquipment,false);
                    return ;


					//##�����Ƴ�װ��
					CS2CContainerObjectMove comMsg;
					if( m_cEquipment.Remove(pEquipment, &comMsg) )
					{
						CHAR szMsg[256];
						const char* strRes=AppFrame::GetText("7");
						_snprintf( szMsg, 256, strRes, pEquipment -> GetName() );
						//SendNotifyMessage( szMsg,NOTIFY_COLOR_RED);
						SendNotifyMessage(szMsg);

						//##����ɹ�����֪ͨ��ҡ�
						comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
						comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
						comMsg.Send( GetExID() );

						//##Ȼ��֪ͨ��Χ��ҡ�
						CMessage msg( MSG_S2C_PLAYER_CHANGE_EQUIPMENT );
						msg.Add( GetExID() );
						msg.Add( static_cast<DWORD>(ec) );
						msg.Add( BYTE(0) /*��ʾж��װ��*/ );
						msg.Add( pEquipment -> GetBasePropertiesIndex() );
						msg.Add( pEquipment -> GetAddonPropertyValues(GAP_WEAPON_LEVEL, 1) );
						msg.SendToAround( this );
						//װ����ʧ��־
						GetLogicLogInterface()->logT040_goods_log_disappear(this,pEquipment,0);
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pEquipment->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::OnEquipmentWaste][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pEquipment->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pEquipment->GetBasePropertiesIndex()),srcGoodsGUID,pEquipment->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif

						//##ɾ������Ʒ
						CGoodsFactory::GarbageCollect( &pEquipment,27 );
						


					}
				}
			}
		}
	}
}

//##���ߵ����
void CPlayer::OnArmorDamaged()
{
	LONG lHeadWastingProbability	= random( 100 );
	LONG lBodyWastingProbability	= random( 100 );
	LONG lGloveWastingProbability	= random( 100 );
	LONG lBootWastingProbability	= random( 100 );

	LONG	lBackWaste = random( 100 );
	LONG	lHeadgearWaste = random( 100 );
	LONG	lFrockWaste = random( 100 );
	LONG	lWingWaste = random( 100 );
	LONG	lManteauWaste = random( 100 );
	LONG	lFairyWaste = random( 100 );
	LONG	lNeckWaste=random(100);
	LONG	lRingWaste=random(100);
	LONG	lShieldWaste=random(100);
	/*
	enum eEquipType 
	{	
	EC_HEAD,				//	0��ͷ��
	EC_NECKLACE,			//	1������
	EC_WING,				//	2�����
	EC_BODY,				//	3������
	EC_BACK,				//	4������
	EC_GLOVE,			//5������	
	EC_BOOT,			//6��Ь��
	EC_HEADGEAR,			//	7��ͷ��
	EC_FROCK,			//	8������		
	EC_LRING,			//	9�����ָ
	EC_RRING,			//	10���ҽ�ָ		
	EC_MEDAL1,			//	11��ѫ��1
	EC_MEDAL2,			//	12	ѫ��2
	EC_MEDAL3,			//	13	ѫ��3
	EC_FAIRY,			//	14��С����
	EC_WEAPON,			//	15������
	EC_WEAPON2,			//	16����������
	EC_CLOAK,			//17����
	EC_HORSE,			//18����
	EC_TOTAL
	};
	*/

	//����
	if( lNeckWaste < GlobalSetup::GetSetup() ->lNeckWaste )
	{
		CGoods* pNeck = m_cEquipment.GetGoods( EC_E_NECKLACE );
		if( pNeck )
		{
			OnEquipmentWaste( EC_E_NECKLACE, pNeck );
		}
	}

	//ͷ��
	if( lHeadWastingProbability < GlobalSetup::GetSetup() -> lHeadWaste )
	{
		CGoods* pHelmet = m_cEquipment.GetGoods( EC_E_HEAD );
		if( pHelmet )
		{
			OnEquipmentWaste( EC_E_HEAD, pHelmet );
		}
	}

	//���ָ
	//�ҽ�ָ
	if(lRingWaste<GlobalSetup::GetSetup() ->lRingWaste)
	{
		CGoods* pLRing = m_cEquipment.GetGoods( EC_E_LRING);
		if( pLRing )
		{
			OnEquipmentWaste( EC_E_LRING, pLRing );
		}
		CGoods* pRRING = m_cEquipment.GetGoods( EC_E_RRING);
		if( pRRING )
		{
			OnEquipmentWaste( EC_E_RRING, pRRING );
		}
	}


	if( lBodyWastingProbability < GlobalSetup::GetSetup() -> lBodyWaste )
	{
		CGoods* pMail = m_cEquipment.GetGoods( EC_E_BODY );
		if( pMail )
		{
			OnEquipmentWaste( EC_E_BODY, pMail );
		}
	}

	if( lGloveWastingProbability < GlobalSetup::GetSetup() -> lGloveWaste )
	{
		CGoods* pGlove = m_cEquipment.GetGoods( EC_E_HAND );
		if( pGlove )
		{
			OnEquipmentWaste( EC_E_HAND, pGlove );
		}
	}

	if( lBootWastingProbability < GlobalSetup::GetSetup() -> lBootWaste )
	{
		CGoods* pBoot = m_cEquipment.GetGoods( EC_E_BOOT );
		if( pBoot )
		{
			OnEquipmentWaste( EC_E_BOOT, pBoot );
		}
	}

	if( lWingWaste < GlobalSetup::GetSetup() -> lWingWaste )
	{
		CGoods* pGoods = m_cEquipment.GetGoods( EC_E_WING );
		if( pGoods )
		{
			OnEquipmentWaste( EC_E_WING, pGoods );
		}
	}
}

//##��������
void CPlayer::OnWeaponDamaged()
{
	CGoods* pWeapon = m_cEquipment.GetGoods( EC_E_WEAPON );
	if( pWeapon )
	{
		OnEquipmentWaste( EC_E_WEAPON, pWeapon );
	}
}
//��pkֵ
void	CPlayer::OnDecreasePKValue()
{
	if( IsDied() ) return;
	DWORD wMurdererCounts = GetPkValue();
	// [9/7/2009 chenxianj]
	//pkֵС�ڵ���20�������ǰ״̬��ս��״̬��pkֵ���ټ���
	if (wMurdererCounts<=eUnDecPKPVP_Value && GetState()==STATE_FIGHT)
	{
		return;
	}

	if (wMurdererCounts>0)
	{
		SetPkValue(GetPkValue() - 1);
	} 

}
//����PKֵ
void	CPlayer::SetPkValue(DWORD l)
{
	SetAttribute("PkValue",l);
	UpdateAttribute(TRUE);
}

//����������
void    CPlayer::ChangeRgnToIsland()
{
	BYTE	byCountry = GetCountry();
	long	lRgnID = GetCountryParam()->GetCountryJailIsLandRegionID(byCountry);
	long 	l = GetCountryParam()->GetCountryJailIsLandRectLeft(byCountry);
	long	t = GetCountryParam()->GetCountryJailIsLandRectTop(byCountry);
	long	lDir = GetCountryParam()->GetCountryJailIsLandDir(byCountry);


	ChangeRegion(RGN_NORMAL, NULL_GUID, l, t,lDir, lRgnID);
}

//���͹��ҹ���
void	CPlayer::PostCountryInfo(const char *strName)
{
	DWORD dwColor = 0xFFFF0000;
	DWORD dwBkColor = 0;
	char  strInfo[256];
	_snprintf(strInfo, 256, AppFrame::GetText("STR_GS_RGNFUNC_COUNTRYINFO"),strName);

	CMessage msg( MSG_S2W_GM_COUNTRY_MESSAGE );
	msg.Add(GetExID());
	msg.Add((char*)strInfo);	
	msg.Add( (long)GetCountry());
	msg.Add( dwColor );
	msg.Add( dwBkColor );
	msg.Send();
}

//����������
bool	CPlayer::ChangeRgnToVillage()
{
	long lPkValue = GetPkValue(); 
	if (lPkValue<= (long)GlobalSetup::GetSetup()->dwPkCountPerAttack || lPkValue>=GlobalSetup::GetSetup()->lPrisonIslandPkValue)
	{
		return false;
	}

	BYTE	byCountry = GetCountry();
	long	lRgnID = GetCountryParam()->GetCountryJailVillageRegionID(byCountry);
	long 	l = GetCountryParam()->GetCountryJailVillageRectLeft(byCountry);
	long	t = GetCountryParam()->GetCountryJailVillageRectTop(byCountry);
	long	lDir = GetCountryParam()->GetCountryJailVillageDir(byCountry);

	/*CServerRegion* pReliveRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, lRgnID);
	CGUID retRgnGuid;
	if(pReliveRgn)
	{
	retRgnGuid = pReliveRgn->GetExID();*/
	ChangeRegion(RGN_NORMAL, NULL_GUID, l, t,lDir, lRgnID);
	return true;
	/*}
	return false;*/
}

//������ֵ
void	CPlayer::OnDecreasePvPValue()
{
	if( IsDied() ) return;

	DWORD dwPvPValue = GetPVPValue();

	if (dwPvPValue<=eUnDecPKPVP_Value && GetState()==STATE_FIGHT)
	{
		return;
	}
	if (dwPvPValue>0)
	{
		SetAttribute("PVPValue",GetPVPValue()-1);
		UpdateAttribute(TRUE);
	} 

}
//�Զ��ָ�hp,mp
void	CPlayer::OnIncreaseHpMpValue()
{
	string strHp = "Hp";
	string strMp = "Mp";
	string strBHpRecover = "B_HpRecoverSpeed";
	string strBMpRecover = "B_MpRecoverSpeed";
	string strEHpRecover = "E_HpRecoverSpeed";
	string strEMpRecover = "E_MpRecoverSpeed";
	long lCurretnHpRecoverSpeed = GetAttribute(strBHpRecover) + GetAttribute(strEHpRecover);
	long lCurretnMpRecoverSpeed = GetAttribute(strBMpRecover) + GetAttribute(strEMpRecover);

	if (!IsDied())
	{
		if (GetAttribute(strHp)< (long)GetMaxHP() && lCurretnHpRecoverSpeed>0)
		{
			long lCurretnHp = GetAttribute(strHp) + lCurretnHpRecoverSpeed;
			if (lCurretnHp> (long)GetMaxHP())
			{
				lCurretnHp = (long) GetMaxHP();
			}
			SetAttribute("Hp",lCurretnHp);
			UpdateAttribute(1);			
		}
		if (GetAttribute(strMp)< (long)GetMaxMP() && lCurretnMpRecoverSpeed>0)
		{
			long lCurretnMp = GetAttribute(strMp) + lCurretnMpRecoverSpeed;
			if (lCurretnMp>GetMaxMP())
			{
				lCurretnMp = GetMaxMP();
			}
			SetAttribute("Mp",lCurretnMp);
			UpdateAttribute(1);			
		}			
	}

	//���ͻ����Ƿ񷢹���ʱPing��Ϣ
	DWORD dwCurTime = timeGetTime();
	if(  (long)(dwCurTime - GetLastPingTime()) > 60*1000*GlobalSetup::GetSetup()->lPingChekTime )
	{
		//����ͻ��˳�ʱδ��ping��Ϣ,��ֱ���ߵ���
		GetGame()->KickPlayer(GetExID());
		SetLastPingTime(dwCurTime);

		GetLogicLogInterface()->logT900_anticheat_player_log(this,"PINGERROR", 0, 0, 0 );
		//���
		long lBanSecs = GlobalSetup::GetSetup()->lPingErrorBanTime * 60;
		CMessage msg( MSG_S2W_GM_BANPLAYER );
		msg.Add( NULL_GUID );
		msg.Add( GetName() );
		msg.Add( (long) lBanSecs );
		msg.Send();
	}
}
//��������ֵ
//void	CPlayer::OnUpdateEnergy()
//{
//	long lNum = 0;
//	string strEnergy	= "Energy";
//	string strMaxEnergy = "B_MaxEnergy";
//	string strEMaxEnergy = "E_MaxEnergy";
//
//	string strBBaseRecover = "B_EnerRecoverSpeed";
//	string strBFightRecover = "B_FightEnerRecoverSpeed";
//	string strEBaseRecover = "E_EnerRecoverSpeed";
//
//	string strEFightRecover = "E_FightEnerRecoverSpeed";
//	long lMaxEnergy = GetAttribute(strMaxEnergy) + GetAttribute(strEMaxEnergy);
//
//
//	if (this->GetState()==STATE_FIGHT)
//	{
//		lNum = GetAttribute(strBFightRecover) + GetAttribute(strEFightRecover);
//	} 
//	else if(this->GetState()==STATE_PEACE)
//	{
//		lNum = GetAttribute(strBBaseRecover) + GetAttribute(strEBaseRecover);
//	}
//
//	long lCurrentValue = lNum + GetAttribute(strEnergy);
//
//	if (lCurrentValue<0)
//	{	
//		lCurrentValue = 0;		
//	}
//	else if (lCurrentValue>=lMaxEnergy)
//	{
//		lCurrentValue = lMaxEnergy;
//	}			
//	if (lCurrentValue==GetAttribute(strEnergy))
//	{
//		return;
//	}
//	SetAttribute(strEnergy,lCurrentValue);
//	UpdateAttribute(1);
//}
//pkֵע�ᶨʱ��
void	CPlayer::RegisterPKValue()
{
	if(m_lPKTimerID!=-1)// || GetPkCount()==0
		return;
	tagTimerVar* pTimvar = OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_PKValue;
	pTimvar->lvar = 0;
	m_lPKTimerID = TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime(),GlobalSetup::GetSetup()->dwOnePkCountTime);
}
//pvpֵע�ᶨʱ��
void	CPlayer::RegisterPvPValue()
{
	if(m_lPvPTimerID!=-1)// || GetPVPValue()==0
		return;
	tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_PvPValue;
	pTimvar->lvar = 0;
	m_lPvPTimerID = TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime(),GlobalSetup::GetSetup()->dwOnePkCountTime);
}
//pkֵע����ʱ��
void	CPlayer::UnRegisterPKValue()
{
	if(m_lPKTimerID != -1)
		TIMER_CANCEL(m_lPKTimerID);
	m_lPKTimerID = -1;
}
//pvpֵע����ʱ��
void	CPlayer::UnRegisterPvPValue()
{
	if(m_lPvPTimerID != -1)
		TIMER_CANCEL(m_lPvPTimerID);
	m_lPvPTimerID = -1;
}
//ע���Զ��ָ�HP,MP��ʱ��ID
void CPlayer::RegisterAutoReNewHpMp()
{
	if (m_lHpMpTimerID!=-1 )
		return;
	tagTimerVar *pTimType = OBJ_CREATE(tagTimerVar);
	pTimType->TimerType = ChildObjType_AutoRecoverHpMp;
	pTimType->lvar = 0;
	m_lHpMpTimerID = TIMER_SCHEDULE(this,(void*)pTimType,timeGetTime(),GlobalSetup::GetSetup()->dwHpMpRestoreTime);

}
//ע���Զ��ָ�HP,MP��ʱ��
void CPlayer::UnRegisterAutoReNewHpMp()
{
	if (m_lHpMpTimerID != -1)
	{
		TIMER_CANCEL(m_lHpMpTimerID);
	}
	m_lHpMpTimerID = -1;
}
//ע������ֵ�ָ�
//void	CPlayer::RegisterEnergyUpdate(long lUpdateTime)
//{
//	if (m_lEnergyTimerID!=-1 || lUpdateTime==0)
//		return;
//	tagTimerVar *pTimType = OBJ_CREATE(tagTimerVar);
//	pTimType->TimerType = ChildObjType_AutoRecoverEnergy;
//	pTimType->lvar = 0;
//	m_lEnergyTimerID = TIMER_SCHEDULE(this,(void*)pTimType,timeGetTime(),lUpdateTime);
//}

//ע������ֵ�ָ�
//void	CPlayer::UnRegisterEnergyUpdate		()
//{
//	if (m_lEnergyTimerID != -1)
//	{
//		TIMER_CANCEL(m_lEnergyTimerID);
//	}
//	m_lEnergyTimerID = -1;
//}

//����ע��سǸ��ʱ��
void	CPlayer::RegisterReliveTimerID()
{
	if (m_lReliveTimerID != -1)
		return;
	
	tagTimerVar *pTimType = OBJ_CREATE(tagTimerVar);
	pTimType->TimerType = ChildObjType_ReliveTimer;
	pTimType->lvar = 0;
	m_lReliveTimerID = TIMER_SCHEDULE(this,(void*)pTimType,timeGetTime()+GlobalSetup::GetSetup()->lReLiveTime,0);
	
}

//����ע���سǸ��ʱ��
void	CPlayer::UnRegisterReliveTimerID()
{
	if (m_lReliveTimerID != -1)
	{
		TIMER_CANCEL(m_lReliveTimerID);
	}
	m_lReliveTimerID = -1;
}


void CPlayer::OnDecreaseMurdererSign()
{
	//##����״̬�£����ܼ���ɱ������
	if( IsDied() ) return;

	WORD wMurdererCounts = (WORD)GetPkValue();
	DWORD dwPvPValue = GetPVPValue();
	if (wMurdererCounts>0)
	{
		SetPkValue(GetPkValue() - 1);

		if (GetPkCount()==0)
			m_dwMurdererTimeStamp = 0;
		else
			m_dwMurdererTimeStamp = timeGetTime();
	}
	if (dwPvPValue>0)
	{
		SetAttribute("PVPValue",GetPVPValue()-1);
		UpdateAttribute(TRUE);
		if (GetPVPValue()==0)
		{
			m_dwMurdererTimeStamp = 0;
		}
		else
		{
			m_dwMurdererTimeStamp = timeGetTime();
		}
	}


}

//##���˺���Ĵ���
void CPlayer::OnBeenHurted( LONG lAttackerType, const CGUID& AttackerID )
{
	//EnterCombatState();
	OnArmorDamaged();

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		CPlayer* pAttacker = NULL;
		if( TYPE_PET == lAttackerType )
		{
			if( lAttackerType == TYPE_PET )
			{
				CMoveShape* pPet = GetGame()->FindShape(TYPE_PET, AttackerID);
				if( pPet != NULL )
				{
					pAttacker = (CPlayer*)(pPet->GetHost());
				}
			}
		}
		else if( TYPE_PLAYER == lAttackerType )
		{
			CPlayer* pAttacker = GetGame() -> FindPlayer( AttackerID );
		}

		if( pAttacker != NULL )
		{
			// ֪ͨ
			// ����֪ͨ ��ͬ���� �ǹ�սʱ�� �ڷǾ�����
			if( pAttacker && pRegion->GetNotify() &&
				GetCountry() != pAttacker->GetCountry() &&
				//!CPKSys::getInstance()->IsCountryWarState(this, pMudder) && 
				pRegion->GetSecurity(GetTileX(), GetTileY()) != CRegion::SECURTIY_FIGHT )
			{

				if(  (long)(timeGetTime() - pRegion->GetLastNotifyHurtTime()) > pRegion->GetNotify() )
				{
					char szInfo[1024];
					pRegion->SetLastNotifyHurtTime( timeGetTime() );

					// ֪ͨ��������
					// ������������<����>������x��yϮ��������������ȥ֧Ԯ��
					_snprintf(szInfo, 1024, "������<%s>[%u,%u]Ϯ��������������ȥ֧Ԯ��",
						pRegion->GetName(), GetTileX(), GetTileY() );
					CMessage msg(MSG_S2W_OTHER_TALK_NOTIFY);
					msg.Add(BYTE(0));
					msg.Add(GetCountry());	// ����ID
					msg.Add(szInfo);
					msg.Send();

					//// �������� ������������
					//if( GetCountry() != pRegion->GetCountry() || !pRegion->GetCountry() ) 
					//{
					//	// ֪ͨ��������
					//	// [�����]��[����]�ɹ���ɱ�˵��ˣ�
					//	_snprintf(szInfo, "[%s]����<%s>�ɹ���ɱ�˵��ˡ�",
					//		pAttacker->GetName(), pRegion->GetName());

					//	CMessage msg(MSG_S2W_OTHER_TALK_NOTIFY);
					//	msg.Add(pMudder->GetCountry());	// ����ID
					//	msg.Add(szInfo);
					//	msg.Send();
					//}
				}
			}
		}
	}
}

void CPlayer::OnUpdateMurdererSign()
{
	if (GetPkValue()==0)
	{
		m_dwMurdererTimeStamp = 0;
	}
	else if( m_dwMurdererTimeStamp == 0 )
	{
		m_dwMurdererTimeStamp = timeGetTime();
	}
}

void CPlayer::OnClearMurdererSign()
{
	if( GetPkValue() == 0 )
	{
		m_dwMurdererTimeStamp	= 0;
	}
}

//##��������
void CPlayer::ReportMurderer( const CGUID& MurdererID )
{
	CPlayer* pMurderer = GetGame() -> FindPlayer( MurdererID );
	if( pMurderer )
	{
		long lNum = pMurderer -> GetPkValue() + GlobalSetup::GetSetup()->dwPkCountPerKill;
		if (lNum > 65535)
			lNum = 65535;

		//pMurderer->SetAttribute("PkValue",lNum);
		//pMurderer->SetPkValue(lNum);
		pMurderer->SetPkValue(lNum);

		pMurderer -> OnUpdateMurdererSign();
	}
	//##����WorldServer����ɱ����
	else
	{
		CMessage msg( MSG_S2W_SERVER_REPORT_MURDERER );
		msg.Add( GetType() );
		msg.Add( GetExID() );
		msg.Add( GetKilledMeType() );
		msg.Add( GetKilledMeID() );
		//##���͸��������������
		msg.Send();
	}
}

bool	CPlayer::IsBadman()
{
	return GetPkValue() > GlobalSetup::GetSetup()->dwPkCountPerKill || m_dwSinStateTimeStamp > 0;
}


//##��ɱ����Ĵ���
void CPlayer::OnBeenMurdered( LONG lMurdererType, const CGUID& MurdererGuid )
{

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( !pRegion ) return;

	//##��������
	CGUID MurdererID	= MurdererGuid;
	if( lMurdererType == TYPE_PLAYER )
	{
		CPlayer* pMurderer = GetGame() -> FindPlayer( MurdererID );
		if( pMurderer )
		{
			GetLogicLogInterface()->logT140_player_pkdie_log(this, pMurderer);
			//! �Զ���¼PK����
			LinkmanSystem::GetInstance().NotePkPlayer(this, pMurderer);
			//##����ɱ������
			pMurderer -> SetPkCount( pMurderer -> GetPkCount() + 1 );
		}
		//����Pkϵͳ�ж�
		GetPKSys()->OnKill(pMurderer,this,pRegion);
	}
}

// ��������
void CPlayer::OnDied()
{
	// ����������ջ��̴�
	CPet* activePet = GetActivePet();
	if (activePet!=NULL)
	{
		activePet->ChangeStateType(PET_STATE_FIGHT);
	}

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(GetFather());
	if (!pRegion)
		return;

	//##����赲
	CServerRegion::tagCell *pCell = pRegion->GetCell(GetTileX(),GetTileY());
	if (!pCell)
		return;

    if( GetPhaseID()!=NULL_GUID)
        PHASEMAN.OnPhaseEvent( GetPhaseID(),this, eDie );

	CRegion::eSecurity Security = pRegion->GetSecurity(GetTileX(), GetTileY());

	// ����Ʒ --------------------------------------------------------------------------------------------
	DropParticularGoodsWhenDead();

	// ����Ʒ��ʼ
	// ֻ���ڴ���14���������ǻ��˲ŵ�����
	//if( GetLevel() > GlobalSetup::GetSetup()->m_btNewbieLevelLimit || IsBadman() )
	//{
	//	LONG64 lCurLevelExp = PlayerList::GetPlayerTotalExp(GetLevel()-1) - PlayerList::GetPlayerTotalExp(GetLevel()-2);

	//	LONG64 lLossExp = GetPKSys()->GetDiedLostExp(this,pRegion,Security);

	//	LONG64 lTotalExp = GetTotalExp();
	//	LONG64 lLvlExp = PlayerList::GetPlayerTotalExp(GetLevel()-2);
	//	if (lTotalExp-lLossExp<lLvlExp)
	//	{
	//		lLossExp = lTotalExp - lLvlExp;
	//	}

	//	if (lLossExp>0)
	//	{
	//		SetExp(lTotalExp-lLossExp);

	//		CMessage msg(MSG_S2C_PLAYER_ADDEXP);

	//		msg.Add(GetExp());
	//		msg.Add(GetExpMultiple());
	//		//			msg.Add(GetVigour());
	//		msg.SendToPlayer(GetExID());
	//	}

	//	long lNameColor = 0;
	//	if(m_dwSinStateTimeStamp>0)
	//		lNameColor = 3;
	//	else if (GetPkValue() > 0 && GetPkValue() <= GlobalSetup::GetSetup()->dwPkCountPerAttack)
	//		lNameColor = 1;
	//	else if(GetPkValue() > GlobalSetup::GetSetup()->dwPkCountPerAttack)
	//		lNameColor = 2;

	//	long lDropGoodsStartPos = 0;		// ������Ʒ����ʼλ��
	//	LONG lX, lY;

		//�õ��Ƿ����Ĳ���
//		if(GetPKSys()->GetDiedLostGoods(this,GetKilledMeType(),GetKilledMeID(),pRegion,Security,lNameColor))
//		{
//			// ֻ���ڴ���40���������ǻ��˲ŵ�����
//			if( GetLevel() > GlobalSetup::GetSetup()->m_btNewSoldierLevel || IsBadman() )
//			{
//				//------------------
//				// װ������
//				//------------------
//				/*
//				enum eEquipType 
//				{	
//				EC_HEAD,				//	0��ͷ��
//				EC_NECKLACE,			//	1������
//				EC_WING,				//	2�����
//				EC_BODY,				//	3������
//				EC_BACK,				//	4������
//				EC_GLOVE,			//5������	
//				EC_BOOT,			//6��Ь��
//				EC_HEADGEAR,			//	7��ͷ��
//				EC_FROCK,			//	8������		
//				EC_LRING,			//	9�����ָ
//				EC_RRING,			//	10���ҽ�ָ		
//				EC_MEDAL1,			//	11��ѫ��1
//				EC_MEDAL2,			//	12	ѫ��2
//				EC_MEDAL3,			//	13	ѫ��3
//				EC_FAIRY,			//	14��С����
//				EC_WEAPON,			//	15������
//				EC_WEAPON2,			//	16����������
//				EC_CLOAK,			//17����
//				EC_HORSE,			//18����
//				EC_TOTAL
//				};
//				*/
//				// ͷ
//				if(m_cEquipment.GetGoods(EC_HEAD) && random(10000) < DiedLost.fDropEquip_Head*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_HEAD) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{								
//
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_HEAD), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_NECKLACE) && random(10000) < DiedLost.fDropEquip_Necklace*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_NECKLACE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_NECKLACE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//���
//				if(m_cEquipment.GetGoods(EC_WING) && random(10000) < DiedLost.fDropEquip_Wing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_WING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_WING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				// ����
//				if (m_cEquipment.GetGoods(EC_BODY) && random(10000) < DiedLost.fDropEquip_Body*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_BODY) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_BODY), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_BACK) && random(10000) < DiedLost.fDropEquip_Back*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_BACK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_BACK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_GLOVE) && random(10000) < DiedLost.fDropEquip_Glove*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_GLOVE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_GLOVE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//Ь��
//				if(m_cEquipment.GetGoods(EC_BOOT) && random(10000) < DiedLost.fDropEquip_Boot*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_BOOT) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_BOOT), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//ͷ��
//				if(m_cEquipment.GetGoods(EC_HEADGEAR) && random(10000) < DiedLost.fDropEquip_Headgear*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_HEADGEAR) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_HEADGEAR), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//					L:			comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_FROCK) && random(10000) < DiedLost.fDropEquip_Frock*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_FROCK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_FROCK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//LRING
//				if(m_cEquipment.GetGoods(EC_LRING) && random(10000) < DiedLost.fDropEquip_LRing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_LRING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_LRING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//RRING
//				if(m_cEquipment.GetGoods(EC_RRING) && random(10000) < DiedLost.fDropEquip_RRing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_RRING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_RRING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_FAIRY) && random(10000) < DiedLost.fDropEquip_Fairy*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_FAIRY) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_FAIRY), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				// ����
//				if (m_cEquipment.GetGoods(EC_WEAPON) && random(10000) < DiedLost.fDropEquip_Weapon*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_WEAPON) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_WEAPON), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//��������
//				if(m_cEquipment.GetGoods(EC_WEAPON2) && random(10000) < DiedLost.fDropEquip_Weapon2*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_WEAPON2) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_WEAPON2), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(EC_CLOAK) && random(10000) < DiedLost.fDropEquip_Cloak*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_CLOAK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_CLOAK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}	
//
//
//				//����
//				if(m_cEquipment.GetGoods(EC_HORSE) && random(10000) < DiedLost.fDropEquip_Horse*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(EC_HORSE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(EC_HORSE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}	
//
//
//			}
//			else
//			{
//				// �±��������װ��
//			}
//			//------------------
//			// ��������
//			//------------------
//			//##����
//			CGoodsListListener lGoodsList;
//			m_cOriginPack->TraversingContainer( &lGoodsList );
//			for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
//			{
//				if( random(10000) < DiedLost.fDropEquip_Bag * 10000 )
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cOriginPack->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
//
//						if(pGoods)
//						{
//							if (GoodsExManage::GetInstance().IsFrost(pGoods) || GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								continue;
//							}
//						}
//
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) == CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							continue;
//						}
//
//						CS2CContainerObjectMove comMsg;
//						pGoods = dynamic_cast<CGoods*>( m_cOriginPack->Remove(TYPE_GOODS, lGoodsList.m_vGoodsID[i], &comMsg) );
//						if( pGoods )
//						{
//							pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//							pRegion -> AddObject( pGoods );
//
//							//##���Լ�����Ϣ
//							comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
//							comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//							comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//								lY * pRegion -> GetWidth() + lX );
//							comMsg.SetDestinationContainerExtendID( 0 );
//							comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//							comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//							comMsg.Send( GetExID() );
//
//							//##����Χ��ҷ�����Ϣ
//							CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//							MsgSend.Add( pGoods->GetType() );
//							MsgSend.Add( pGoods->GetExID() );
//							DBWriteSet setWriteDB;
//							MsgSend.GetDBWriteSet(setWriteDB);
//							pGoods->SerializeForRgnDisplay(setWriteDB);
//
//							MsgSend.SendToAround(pGoods);
//
//							lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//							char pszGoodsLog[1024]="";
//							char playerGUID[50]="";
//							GetExID().tostring(playerGUID);
//							char goodsGUID[50]="";
//							pGoods->GetExID().tostring(goodsGUID);
//							_snprintf(pszGoodsLog,1024,"[����:���������������Ʒ][�����:%s][���GUID:%s][����ID:%d][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//								GetName(),playerGUID,1,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//								goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//							GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//						}
//					}
//				}//if end
//			}
//
//
//			for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
//			{
//				if(getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
//				{		
//					CVolumeLimitGoodsContainer* pSubContainer=getSubpackContainer()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
//					lGoodsList.m_vGoodsID.clear();
//					pSubContainer->TraversingContainer(&lGoodsList);
//					for(size_t j=0;j<lGoodsList.m_vGoodsID.size();j++)
//					{
//						if( random(10000) < DiedLost.fDropEquip_Bag * 10000 )
//						{
//							pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//							if( lDropGoodsStartPos != -1 )
//							{
//								CGoods* pGoods = dynamic_cast<CGoods*>( pSubContainer->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[j]) );
//
//								if(pGoods)
//								{
//									if (GoodsExManage::GetInstance().IsFrost(pGoods) || GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//									{
//										continue;
//									}
//								}
//
//								if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//									CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) == CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//								{
//									continue;
//								}
//
//								CS2CContainerObjectMove comMsg;
//								pGoods = dynamic_cast<CGoods*>( pSubContainer->Remove(TYPE_GOODS, lGoodsList.m_vGoodsID[j], &comMsg) );
//								if( pGoods )
//								{
//									pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//									pRegion -> AddObject( pGoods );
//
//									//##���Լ�����Ϣ
//									comMsg.SetSourceContainerExtendID( i );
//									comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//									comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//										lY * pRegion -> GetWidth() + lX );
//									comMsg.SetDestinationContainerExtendID( 0 );
//									comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//									comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//									comMsg.Send( GetExID() );
//
//									//##����Χ��ҷ�����Ϣ
//									CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//									MsgSend.Add( pGoods->GetType() );
//									MsgSend.Add( pGoods->GetExID() );
//									DBWriteSet setWriteDB;
//									MsgSend.GetDBWriteSet(setWriteDB);
//									pGoods->SerializeForRgnDisplay(setWriteDB);
//
//									MsgSend.SendToAround(pGoods);
//
//									lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//									char pszGoodsLog[1024]="";
//									char playerGUID[50]="";
//									GetExID().tostring(playerGUID);
//									char goodsGUID[50]="";
//									pGoods->GetExID().tostring(goodsGUID);
//									_snprintf(pszGoodsLog,1024,"[����:���������������Ʒ][�����:%s][���GUID:%s][����ID:%d][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//										GetName(),playerGUID,i,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//										goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//									CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//									GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//								}
//							}
//						}//if end
//					}	
//				}
//			}	
//
//
//			//------------------
//			// ��������
//			//------------------
//			// ��Ǯ
//			if (GetMoney() && random(10000) < DiedLost.fDropEquip_Money*10000)
//			{
//				DWORD dwDropMoney = static_cast<DWORD>( GetMoney() * DiedLost.fDropEquip_MoneyPercent );
//				if( dwDropMoney>0 && GetMoney() - dwDropMoney >= 0 )
//				{
//					lDropGoodsStartPos = pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos);
//					if( lDropGoodsStartPos != -1 )
//					{
//						//extern bool _AddMoneyToRegion(CRegion* p_region, long dx, long dy, DWORD dw_money, long lProtectPlayerID=0, long lProtectTeamID=0);
//						//_AddMoneyToRegion(pRegion, lX, lY, dwDropMoney);
//						SetMoney(GetMoney()-dwDropMoney,7);
//						CGoods* pGoldCoins = CGoodsFactory::CreateGoods( GoodsSetup::GetGoldCoinIndex(),8 );
//						if( pGoldCoins )
//						{
//							pGoldCoins -> SetAmount( dwDropMoney );
//							pGoldCoins -> SetPosXY( lX+0.5f, lY+0.5f );
//							pRegion -> AddObject( pGoldCoins );
//
//							CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoldCoins->GetExID());
//							MsgSend.Add( pGoldCoins->GetType() );
//							MsgSend.Add( pGoldCoins->GetExID() );
//							DBWriteSet setWriteDB;
//							MsgSend.GetDBWriteSet(setWriteDB);
//							pGoldCoins->SerializeForRgnDisplay(setWriteDB);
//
//							MsgSend.SendToAround(pGoldCoins);
//#ifdef _GOODSLOG1_
//							char pszGoodsLog[1024]="";
//							char playerGUID[50]="";
//							GetExID().tostring(playerGUID);
//							char goodsGUID[50]="";
//							pGoldCoins->GetExID().tostring(goodsGUID);
//							_snprintf(pszGoodsLog,1024,"[����:������������][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//								GetName(),playerGUID,pRegion->GetID(),pGoldCoins->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoldCoins->GetBasePropertiesIndex()),
//								goodsGUID,pGoldCoins->GetAmount(),GetTileX(),GetTileY());
//							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//							GetLogicLogInterface()->logT050_goods_log_fall(this,pGoldCoins);
//						}
//					}			
//				}
//			}
//		}
		//if(GetPKSys()->GetDiedLostGoods(this,GetKilledMeType(),GetKilledMeID(),pRegion,Security,lNameColor,DiedLost))
		//{
			// ֻ���ڴ���40���������ǻ��˲ŵ�����
			//if( GetLevel() > GlobalSetup::GetSetup()->m_btNewSoldierLevel || IsBadman() )
			//{
				//------------------
				// װ������
				//------------------
				/*
				enum EQUIPMENT_COLUMN 
				{	
				EC_HEAD,				//	0��ͷ��
				EC_NECKLACE,			//	1������
				EC_WING,				//	2�����
				EC_BODY,				//	3������
				EC_BACK,				//	4������
				EC_GLOVE,			//5������	
				EC_BOOT,			//6��Ь��
				EC_HEADGEAR,			//	7��ͷ��
				EC_FROCK,			//	8������		
				EC_LRING,			//	9�����ָ
				EC_RRING,			//	10���ҽ�ָ		
				EC_MEDAL1,			//	11��ѫ��1
				EC_MEDAL2,			//	12	ѫ��2
				EC_MEDAL3,			//	13	ѫ��3
				EC_FAIRY,			//	14��С����
				EC_WEAPON,			//	15������
				EC_WEAPON2,			//	16����������
				EC_CLOAK,			//17����
				EC_HORSE,			//18����
				EC_TOTAL
				};
				*/
				// ͷ
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_HEAD) && random(10000) < DiedLost.fDropEquip_Head*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_HEAD) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{								
//
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_HEAD), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_NECKLACE) && random(10000) < DiedLost.fDropEquip_Necklace*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_NECKLACE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_NECKLACE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//���
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_WING) && random(10000) < DiedLost.fDropEquip_Wing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_WING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_WING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				// ����
//				if (m_cEquipment.GetGoods(CEquipmentContainer::EC_BODY) && random(10000) < DiedLost.fDropEquip_Body*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_BODY) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_BODY), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_BACK) && random(10000) < DiedLost.fDropEquip_Back*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_BACK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_BACK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_GLOVE) && random(10000) < DiedLost.fDropEquip_Glove*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_GLOVE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_GLOVE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//Ь��
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_BOOT) && random(10000) < DiedLost.fDropEquip_Boot*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_BOOT) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_BOOT), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//ͷ��
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_HEADGEAR) && random(10000) < DiedLost.fDropEquip_Headgear*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_HEADGEAR) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_HEADGEAR), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_FROCK) && random(10000) < DiedLost.fDropEquip_Frock*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_FROCK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_FROCK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//LRING
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_LRING) && random(10000) < DiedLost.fDropEquip_LRing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_LRING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_LRING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//RRING
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_RRING) && random(10000) < DiedLost.fDropEquip_RRing*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_RRING) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_RRING), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_FAIRY) && random(10000) < DiedLost.fDropEquip_Fairy*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_FAIRY) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_FAIRY), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				// ����
//				if (m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON) && random(10000) < DiedLost.fDropEquip_Weapon*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_WEAPON), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//��������
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON2) && random(10000) < DiedLost.fDropEquip_Weapon2*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_WEAPON2) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_WEAPON2), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_CLOAK) && random(10000) < DiedLost.fDropEquip_Cloak*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_CLOAK) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_CLOAK), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}	
//
//
//				//����
//				if(m_cEquipment.GetGoods(CEquipmentContainer::EC_HORSE) && random(10000) < DiedLost.fDropEquip_Horse*10000)
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.GetGoods(CEquipmentContainer::EC_HORSE) );
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) != CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							if (!GoodsExManage::GetInstance().IsFrost(pGoods) && !GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								
//							CS2CContainerObjectMove comMsg;
//							pGoods = dynamic_cast<CGoods*>( m_cEquipment.Remove( static_cast<DWORD>(CEquipmentContainer::EC_HORSE), static_cast<DWORD>(1), &comMsg) );
//							if( pGoods )
//							{
//								pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//								pRegion -> AddObject( pGoods );
//
//								//##����Ϣ���Լ�
//								comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_EQUIPMENT );
//								comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//								comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//									lY * pRegion -> GetWidth() + lX );
//								comMsg.SetDestinationContainerExtendID( 0 );
//								comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//								comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//								comMsg.Send( GetExID() );
//
//								//##����Χ��ҷ�����Ϣ
//								CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//								MsgSend.Add( pGoods->GetType() );
//								MsgSend.Add( pGoods->GetExID() );
//								DBWriteSet setWriteDB;
//								MsgSend.GetDBWriteSet(setWriteDB);
//								pGoods->SerializeForRgnDisplay(setWriteDB);
//
//								MsgSend.SendToAround(pGoods);
//
//								lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//								char pszGoodsLog[1024]="";
//								char playerGUID[50]="";
//								GetExID().tostring(playerGUID);
//								char goodsGUID[50]="";
//								pGoods->GetExID().tostring(goodsGUID);
//								_snprintf(pszGoodsLog,1024,"[����:���������װ��][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//									GetName(),playerGUID,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//									goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//								CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//								GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//							}
//							}
//						}
//					}					
//				}	
//
//
//			}
//			else
//			{
//				// �±��������װ��
//			}
//			//------------------
//			// ��������
//			//------------------
//			//##����
//			CGoodsListListener lGoodsList;
//			getOriginPack()->TraversingContainer( &lGoodsList );
//			for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
//			{
//				if( random(10000) < DiedLost.fDropEquip_Bag * 10000 )
//				{
//					pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//					if( lDropGoodsStartPos != -1 )
//					{
//						CGoods* pGoods = dynamic_cast<CGoods*>( getOriginPack()->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
//
//						if(pGoods)
//						{
//							if (GoodsExManage::GetInstance().IsFrost(pGoods) || GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//							{
//								continue;
//							}
//						}
//
//						if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//							CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) == CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//						{
//							continue;
//						}
//
//						CS2CContainerObjectMove comMsg;
//						pGoods = dynamic_cast<CGoods*>( getOriginPack()->Remove(TYPE_GOODS, lGoodsList.m_vGoodsID[i], &comMsg) );
//						if( pGoods )
//						{
//							pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//							pRegion -> AddObject( pGoods );
//
//							//##���Լ�����Ϣ
//							comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_PACKET );
//							comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//							comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//								lY * pRegion -> GetWidth() + lX );
//							comMsg.SetDestinationContainerExtendID( 0 );
//							comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//							comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//							comMsg.Send( GetExID() );
//
//							//##����Χ��ҷ�����Ϣ
//							CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//							MsgSend.Add( pGoods->GetType() );
//							MsgSend.Add( pGoods->GetExID() );
//							DBWriteSet setWriteDB;
//							MsgSend.GetDBWriteSet(setWriteDB);
//							pGoods->SerializeForRgnDisplay(setWriteDB);
//
//							MsgSend.SendToAround(pGoods);
//
//							lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//							char pszGoodsLog[1024]="";
//							char playerGUID[50]="";
//							GetExID().tostring(playerGUID);
//							char goodsGUID[50]="";
//							pGoods->GetExID().tostring(goodsGUID);
//							_snprintf(pszGoodsLog,1024,"[����:���������������Ʒ][�����:%s][���GUID:%s][����ID:%d][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//								GetName(),playerGUID,1,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//								goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//							GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//						}
//					}
//				}//if end
//			}
//
//
//			for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
//			{
//				if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
//				{		
//					CVolumeLimitGoodsContainer* pSubContainer=GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
//					lGoodsList.m_vGoodsID.clear();
//					pSubContainer->TraversingContainer(&lGoodsList);
//					for(size_t j=0;j<lGoodsList.m_vGoodsID.size();j++)
//					{
//						if( random(10000) < DiedLost.fDropEquip_Bag * 10000 )
//						{
//							pRegion -> GetDropGoodsPos( GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos );
//							if( lDropGoodsStartPos != -1 )
//							{
//								CGoods* pGoods = dynamic_cast<CGoods*>( pSubContainer->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[j]) );
//
//								if(pGoods)
//								{
//									if (GoodsExManage::GetInstance().IsFrost(pGoods) || GoodsExManage::GetInstance().IsBind(pGoods)) //! �������
//									{
//										continue;
//									}
//								}
//
//								if( pGoods && ( pGoods -> GetAddonPropertyValues(CGoodsBaseProperties::GAP_PARTICULAR_ATTRIBUTE, 1) &
//									CGoodsBaseProperties::PA_NEVER_DROP_DOWN ) == CGoodsBaseProperties::PA_NEVER_DROP_DOWN )
//								{
//									continue;
//								}
//
//								CS2CContainerObjectMove comMsg;
//								pGoods = dynamic_cast<CGoods*>( pSubContainer->Remove(TYPE_GOODS, lGoodsList.m_vGoodsID[j], &comMsg) );
//								if( pGoods )
//								{
//									pGoods -> SetPosXY( lX+0.5f, lY+0.5f );
//									pRegion -> AddObject( pGoods );
//
//									//##���Լ�����Ϣ
//									comMsg.SetSourceContainerExtendID( i );
//									comMsg.SetOperation( CS2CContainerObjectMove::OT_MOVE_OBJECT );
//									comMsg.SetDestinationContainer( pRegion -> GetType(), pRegion -> GetExID(), 
//										lY * pRegion -> GetWidth() + lX );
//									comMsg.SetDestinationContainerExtendID( 0 );
//									comMsg.SetDestinationObject( pGoods -> GetType(), pGoods -> GetExID() );
//									comMsg.SetDestinationObjectAmount( pGoods -> GetAmount() );
//									comMsg.Send( GetExID() );
//
//									//##����Χ��ҷ�����Ϣ
//									CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoods->GetExID());
//									MsgSend.Add( pGoods->GetType() );
//									MsgSend.Add( pGoods->GetExID() );
//									DBWriteSet setWriteDB;
//									MsgSend.GetDBWriteSet(setWriteDB);
//									pGoods->SerializeForRgnDisplay(setWriteDB);
//
//									MsgSend.SendToAround(pGoods);
//
//									lDropGoodsStartPos ++;
//#ifdef _GOODSLOG1_
//									char pszGoodsLog[1024]="";
//									char playerGUID[50]="";
//									GetExID().tostring(playerGUID);
//									char goodsGUID[50]="";
//									pGoods->GetExID().tostring(goodsGUID);
//									_snprintf(pszGoodsLog,1024,"[����:���������������Ʒ][�����:%s][���GUID:%s][����ID:%d][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//										GetName(),playerGUID,i,pRegion->GetID(),pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),
//										goodsGUID,pGoods->GetAmount(),GetTileX(),GetTileY());
//									CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//									GetLogicLogInterface()->logT050_goods_log_fall(this,pGoods);
//								}
//							}
//						}//if end
//					}	
//				}
//			}	
//
//
//			//------------------
//			// ��������
//			//------------------
//			// ��Ǯ
//			if (GetMoney() && random(10000) < DiedLost.fDropEquip_Money*10000)
//			{
//				DWORD dwDropMoney = static_cast<DWORD>( GetMoney() * DiedLost.fDropEquip_MoneyPercent );
//				if( dwDropMoney>0 && GetMoney() - dwDropMoney >= 0 )
//				{
//					lDropGoodsStartPos = pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), lX, lY, lDropGoodsStartPos);
//					if( lDropGoodsStartPos != -1 )
//					{
//						//extern bool _AddMoneyToRegion(CRegion* p_region, long dx, long dy, DWORD dw_money, long lProtectPlayerID=0, long lProtectTeamID=0);
//						//_AddMoneyToRegion(pRegion, lX, lY, dwDropMoney);
//						SetMoney(GetMoney()-dwDropMoney,7);
//						CGoods* pGoldCoins = CGoodsFactory::CreateGoods( GoodsSetup::GetGoldCoinIndex(),8 );
//						if( pGoldCoins )
//						{
//							pGoldCoins -> SetAmount( dwDropMoney );
//							pGoldCoins -> SetPosXY( lX+0.5f, lY+0.5f );
//							pRegion -> AddObject( pGoldCoins );
//
//							CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,pGoldCoins->GetExID());
//							MsgSend.Add( pGoldCoins->GetType() );
//							MsgSend.Add( pGoldCoins->GetExID() );
//							DBWriteSet setWriteDB;
//							MsgSend.GetDBWriteSet(setWriteDB);
//							pGoldCoins->SerializeForRgnDisplay(setWriteDB);
//
//							MsgSend.SendToAround(pGoldCoins);
//#ifdef _GOODSLOG1_
//							char pszGoodsLog[1024]="";
//							char playerGUID[50]="";
//							GetExID().tostring(playerGUID);
//							char goodsGUID[50]="";
//							pGoldCoins->GetExID().tostring(goodsGUID);
//							_snprintf(pszGoodsLog,1024,"[����:������������][�����:%s][���GUID:%s][����ID:%d][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
//								GetName(),playerGUID,pRegion->GetID(),pGoldCoins->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoldCoins->GetBasePropertiesIndex()),
//								goodsGUID,pGoldCoins->GetAmount(),GetTileX(),GetTileY());
//							CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
//#endif
//
//							GetLogicLogInterface()->logT050_goods_log_fall(this,pGoldCoins);
//						}
//					}			
//				}
//			}
//		}
//	}
//	else { //
//		const char* strRes=AppFrame::GetText("8");
//		SendNotifyMessage( strRes );
//
//	}

	//����ע��سǸ��ʱ��
	RegisterReliveTimerID();

	//����ʱ�����ű�
	const char* strScript = GlobalSetup::GetSetup()->pszPlayerDiedScript;
	if(strScript)
	{
		//stRunScript sScript;
		//sScript.pszFileName = (char*)strScript;
		//sScript.desShape = NULL;
		//sScript.pRegion =  dynamic_cast<CRegion*>(GetFather());
		//sScript.srcShape	= this;
		//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
		Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ) );
		GetInst( ScriptSys ).RunScript( env, strScript );
	}

	char szMsg[1024]="";
	CRegion* pLocalRgn = dynamic_cast<CRegion*>(GetFather());
	// ��������
	switch(GetKilledMeType())
	{
	case TYPE_PLAYER:
		{
			CPlayer *pMudder = GetGame()->FindPlayer(GetKilledMeID());
			if (pMudder)
			{
				if( pMudder->GetHideFlag() == 1 && pLocalRgn && pLocalRgn->GetRgnHideFlag() != 2
					&& (GetTeamID()!=pMudder->GetTeamID() || GetTeamID()==NULL_GUID || pMudder->GetTeamID()==NULL_GUID) )
					_snprintf(szMsg, 1024, "[Colour 'ffff0000']�㱻���<%s>ɱ���ˡ�", GlobalSetup::GetSetup()->szHideName);
				else
					_snprintf(szMsg, 1024, "�㱻���<%s>ɱ���ˡ�", pMudder->GetName());
				//SendNotifyMessage(szMsg, 0xffff0000);
				SendNotifyMessage(szMsg);


				// ֪ͨ
				// ����֪ͨ ��ͬ���� �ǹ�սʱ�� �ڷǾ�����
				if( pRegion->GetNotify() &&
					GetCountry() != pMudder->GetCountry() &&
					pRegion->GetSecurity(GetTileX(), GetTileY()) != CRegion::SECURTIY_FIGHT )
				{
					char szInfo[1024];
					if( pRegion->GetLastNotifyKillTime()+pRegion->GetNotify() < timeGetTime() )
					{
						pRegion->SetLastNotifyKillTime( timeGetTime() );

						// ֪ͨ��������
						// [�����]��[����]�ɹ���ɱ�˵��ˣ�
						if( pMudder->GetFactionID() != NULL_GUID)
						{
							//							_snprintf(szInfo, "<%s>��� %s�����<%s>�ɹ���ɱ�й��ˡ�", pMudder->GetFactionName().data(), pMudder->GetName(), pRegion->GetName());
						}
						else
						{
							if(pMudder->GetHideFlag() == 1 && pRegion && pRegion->GetRgnHideFlag() != 2)
								_snprintf(szInfo, 1024, "%s�����<%s>�ɹ���ɱ�й��ˡ�",	 GlobalSetup::GetSetup()->szHideName, pRegion->GetName());
							else
								_snprintf(szInfo, 1024, "%s�����<%s>�ɹ���ɱ�й��ˡ�",	 pMudder->GetName(), pRegion->GetName());
						}
						CMessage msg2(MSG_S2W_OTHER_TALK_NOTIFY);
						msg2.Add(BYTE(2));
						msg2.Add(pMudder->GetCountry());	// ����ID
						msg2.Add(szInfo);
						msg2.Send();
					}

				}
			}
		}
		break;

	case TYPE_MONSTER:
		{
			CMonster *pMudder = pRegion->FindMonsterByID(GetKilledMeID());
			if (pMudder)
			{
				_snprintf(szMsg, 1024, "[Colour 'ffff0000']�㱻����<%s>ɱ���ˡ�", pMudder->GetName());
				SendNotifyMessage(szMsg);
			}
		}
		break;

	case TYPE_PET:
		{
			CPet *pMudder = GetPet(GetKilledMeID());
			if( pMudder != NULL )
			{
				CPlayer* pPlayer = (CPlayer*)pMudder->GetHost();
				_snprintf(szMsg, 1024, "[Colour 'ffff0000']�㱻���%s�ĳ���<%s>ɱ���ˡ�",
					pPlayer->GetName(),
					pMudder->GetName());
				SendNotifyMessage(szMsg);

				// ֪ͨ
				// ����֪ͨ ��ͬ���� �ǹ�սʱ�� �ڷǾ�����
				if( pRegion->GetNotify() &&
					GetCountry() != pMudder->GetCountry() &&
					pRegion->GetSecurity(GetTileX(), GetTileY()) != CRegion::SECURTIY_FIGHT )
				{
					char szInfo[1024];
					if( pRegion->GetLastNotifyKillTime()+pRegion->GetNotify() < timeGetTime() )
					{
						pRegion->SetLastNotifyKillTime( timeGetTime() );



						// ֪ͨ��������
						// [�����]��[����]�ɹ���ɱ�˵��ˣ�
						if( pPlayer->GetFactionID() != NULL_GUID)
						{
							//_snprintf(szInfo, "<%s>��� %s�����<%s>�ɹ���ɱ�й��ˡ�",
							//	pMudder->GetFactionName().data(),
							//	pMudder->GetName(),
							//	pRegion->GetName());
						}
						else
						{
							if(pPlayer->GetHideFlag() == 1 && pRegion && pRegion->GetRgnHideFlag() != 2)
								_snprintf(szInfo, 1024, "%s�����<%s>�ɹ���ɱ�й��ˡ�",
								GlobalSetup::GetSetup()->szHideName,
								pRegion->GetName());
							else
								_snprintf(szInfo, 1024, "%s�����<%s>�ɹ���ɱ�й��ˡ�",
									pPlayer->GetName(),
									pRegion->GetName());
						}
						CMessage msg2(MSG_S2W_OTHER_TALK_NOTIFY);
						msg2.Add(BYTE(2));
						msg2.Add(pMudder->GetCountry());	// ����ID
						msg2.Add(szInfo);
						msg2.Send();
					}
				}
			}
		}
		break;
	}
}

// ����
long CPlayer::CheckLevel()
{
	WORD lOldLevel = GetLevel();
	string strHp = "Hp";
	string strMp = "Mp";
	string strExp = "Exp";
	string strExpMultiple = "ExpMultiple";

	LONG64 llExp = GetTotalExp();
	LONG64 llNextExp = GetNextTotalExp();
	while( llExp >= llNextExp )
	{
		if (GetLevel() >= PlayerList::GetLevelNum())	// �Ѿ������ȼ�
		{
			SetExp(GetNextTotalExp());
			break;
		}
		else	
		{
			// ����
			SetLevel(GetLevel()+1, FALSE);		
			//����Hp
			SetAttribute(strHp,GetMaxHP());
			//����Mp
			SetAttribute(strMp,GetMaxMP());
			LONG64 llRet = llExp-llNextExp;
			//����Exp
			SetAttribute(strExp, (long)llRet%dwExpMutipleValue);
			//����ExpMultiple 
			SetAttribute(strExpMultiple, (long)llRet/dwExpMutipleValue);
			//�����ȼ��仯�ӿ�
			OnLevelChanged();
			UpdateAttribute();

			llExp = llRet;
			llNextExp = GetNextTotalExp();
		}
	}

	WORD lChangeLevel = GetLevel() - lOldLevel;
	//##����Ƿ�����
	if( lChangeLevel > 0 )
	{
		GetLogicLogInterface()->logT100_player_level_log(this, lOldLevel, "��ͨ����");
		// ������������
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
		//##����������Ϣ����Χ���
		CMessage msg( MSG_S2C_PLAYER_LEVELUP );
		msg.Add( GetExID() );
		msg.Add(GetMaxHP());
		msg.Add(GetMaxMP());
		msg.Add(GetLevel());
		msg.Add( GetExp() );
		msg.Add(GetExpMultiple());
		msg.Add(GetNextExp());
		msg.Add(GetNextExpMultiple());
		//�õ���ǰ���;���
		msg.Add(GetPresentExp());
		msg.SendToAround(this);

		// Fox@20081110 ˢ�½�ɫ�ȼ�
		GetInst(CPlayerMan).RefreshElem(PET_LEVEL, this);
	}

	return lChangeLevel;
}

// ���ְҵ�ȼ��仯
long CPlayer::CheckOccuLevel(eOccupation byOccu)
{
	WORD lOldOccuLevel = GetOccuLvl(byOccu);
	string strHp = "Hp";
	string strMp = "Mp";
	string strExp = "Exp";

	while( GetOccuExp(byOccu) >= GetNextOccuExp() )
	{
		if (GetOccuLvl(byOccu) >= PlayerList::GetOccuLevelNum(byOccu))	// �Ѿ������ȼ�
		{
			SetOccuExp(byOccu,GetNextExp());
			break;
		}
		else	
		{
			// ����
			SetOccuLvl(byOccu,GetOccuLvl(byOccu)+1, FALSE);		
			//����
			SetOccuExp(byOccu,GetOccuExp(byOccu)-GetNextOccuExp());
			//�����ȼ��仯�ӿ�
			OnOccuLevelChanged();
			UpdateAttribute();
		}
	}

	WORD lChangeLevel = GetOccuLvl(byOccu) - lOldOccuLevel;
	//##����Ƿ�����
	if( lChangeLevel > 0 )
	{
		GetLogicLogInterface()->logT101_player_Occulevel_log(this, lOldOccuLevel, "��ͨ����");
		// ������������
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
		//##����������Ϣ����Χ���
		CMessage msg( MSG_S2C_PLAYER_OCCULEVELUP );
		msg.Add( GetExID() );
		msg.Add(GetMaxHP());
		msg.Add(GetMaxMP());
		msg.Add(BYTE(byOccu));
		msg.Add(GetOccuLvl(byOccu));
		msg.Add(GetOccuExp(byOccu) );
		msg.Add(GetNextOccuExp());
		msg.SendToAround(this);
	}

	return lChangeLevel;
}

/**## Added by Jar @ 2004-4-6 */
bool CPlayer::IsGM()
{
	if( NULL == GMList::GetInfoByName( GetName() ) ) 
		return false;
	return true;
}

long CPlayer::GetGMLevel()
{
	tagGMInfo* pInfo = GMList::GetInfoByName( GetName() );
	if( pInfo )
	{
		return pInfo -> lLevel;
	}
	else return GM_PLAYER;
}

// �����ҵ�TileX,Y�Ƿ���ָ����Χ�� (һ����������)
// x,y�����������꣬range��ȣ�
//
//	��������������
//	��p1����������	r
//	��������������	a
//	������xy������	n = 3
//	��������������	g
//	��������������	e
//	��������������
//		p2
//
// p1 �������ڣ� p2 ��������
bool CPlayer::IsInArea(/*long regionid,*/ long x, long y, long range)
{
	long lX,lY;

	// ��������ȥ���С��0����� �� ����WH�����
	CServerRegion* pRegion=dynamic_cast<CServerRegion*>(GetFather());

	lX=lY=0;


	if( x - range < 0 )
	{
		lX = 0;
	}
	else if(x - range > pRegion->GetWidth() )
	{
		lX = pRegion->GetWidth();
	}
	else
	{
		lX = x;
	}

	if( y - range < 0 ) 
	{
		lY = 0;
	}
	else if( y-range > pRegion->GetHeight() )
	{
		lY = pRegion->GetHeight();
	}
	else
	{
		lY = y;
	}

	if( GetTileX() >= lX - range && 
		GetTileX() <= lX + range &&
		GetTileY() >= lY - range &&
		GetTileY() <= lY + range )
		return true;
	return false;
}

// ��ⳡ��
bool CPlayer::IsInRegion(long regionid)
{
	// ����Ƿ��ڸó���
	if( GetRegionID() != regionid ) return false;

	return true;
}
//�ı�ְҵ,����ר�Ű�װתְ�Ľӿ�
//ְҵ�任�Ժ�Ҫ���������������
void CPlayer::ChangeOccu(eOccupation eNewOccu)
{
	//����������ڵ�ְҵ����ֱ�ӷ���
	if(GetOccupation() == eNewOccu)	return;
	ProduceEvent(ET_ChangeOccu);
	int nOldOccu = GetOccupation();
	int nNewOccu = eNewOccu;
	SetOccupation(eNewOccu);
	//�������ûת����ְҵ,���ʼ����ְҵ�ȼ�Ϊ1
	if(m_Property.byOccuLvl[m_Property.byOccu] == 0)
		m_Property.byOccuLvl[m_Property.byOccu] = 1;
	AddAttributeToDirty(string("Occu"));
	char pszTempt[256]="";
	_snprintf(pszTempt,256,"OccuLvl%d",m_Property.byOccu);
	AddAttributeToDirty(string(pszTempt));

	list<long> lSkill = CNewSkillFactory::GetInitOccuSkill(GetOccupation());
	for (list<long>::iterator it=lSkill.begin(); it!=lSkill.end(); ++it)
	{

		DWORD dwID = (*it);
		DWORD dwLevel = 1;		
		AddSkill(dwID,dwLevel);		
		OnBeginSkill(dwID,dwLevel);		
	}	

	//����ս�����ԵĻ������Ե�����
	ResetBaseFightProperty();
	//��������ս������
	ResetFightProperty();
	////����������ʱ��
	//UpdateEnergyTimer();//

	UpdateAttribute(1);

	// Fox@20081111 ˢ�½�ɫ�ȼ�
	GetInst(CPlayerMan).RefreshElem(PET_OCCUPATION, this);

	//! ֪ͨWS�ı�
	if(nOldOccu != nNewOccu)
	{
		CMessage msg(MSG_S2W_PLAYER_OCCU_CHANGE);
		msg.Add(GetExID());
		msg.Add((LONG)eNewOccu);
		msg.Send(false);
	}
}

void CPlayer::ChangePropertyByGoods(CGoods* pGoods,eGoodsAddonProperties addon,int whichVal,string strAttrName,bool bMount)
{
	if(pGoods==NULL)
		return;
	if(whichVal!=1 && whichVal!=2)
		return;
	long lval=pGoods -> GetAddonPropertyValues( addon, whichVal );	
	if(bMount)
	{					
		ChangeAttribute(strAttrName,lval);
	}
	else
	{
		ChangeAttribute(strAttrName,-lval);
	}	
}

void CPlayer::UpdatePropertyByGoodsTrigger(CGoods* pGoods,bool bMount)
{
	if(!pGoods)
		return;
	//��Ʒ�Ƿ��С����Լ������ĸ�������
	if(!pGoods->HasAddonProperty(GAP_PROPERTY_TRIGGER))
		return;
	LONG lCurTriggerLevel=pGoods->GetAddonPropertyValues(GAP_PROPERTY_TRIGGER,1);
	if(lCurTriggerLevel<=0)
		return;
	CGoodsBaseProperties* pGoodsBaseProperty=GoodsSetup::QueryGoodsBaseProperties(pGoods->GetBasePropertiesIndex());
	if(!pGoodsBaseProperty)
		return;
	for(size_t i=0;i<pGoodsBaseProperty->GetTriggerProperties().size();i++)
	{			
		if(lCurTriggerLevel>=pGoodsBaseProperty->GetTriggerProperties()[i].wTriggerLevel)
		{
			string strPropertyName=CPlayer::GetFightPropertyNameByEnum(pGoodsBaseProperty->GetTriggerProperties()[i].gapType);
			if(strPropertyName!="")
			{
				if(bMount)
					ChangeAttribute(strPropertyName,pGoodsBaseProperty->GetTriggerProperties()[i].lVal1);
				else
					ChangeAttribute(strPropertyName,-(pGoodsBaseProperty->GetTriggerProperties()[i].lVal1));
			}
		}		
	}	
}

void CPlayer::MountAllEquip()
{
	CGoods* pGoods=NULL;
	for(size_t i = 0; i<EC_TOTAL; i++)
	{		
		pGoods = m_cEquipment.GetGoods((DWORD)i);
		if(pGoods!=NULL)
		{   
            if( !pGoods->HasAddonProperty( GAP_GOODS_MAX_DURABILITY) || 
                pGoods->GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 ) > 0)
			    MountEquipEx(pGoods,true);
		}
	}
}

void CPlayer::MountEquipEx(CGoods *pGoods, bool bMount, bool bGuardianFlag )
{
	if(pGoods==NULL)
		return;
	vector<eGoodsAddonProperties> vOut;
	pGoods -> GetEnabledAddonProperties( vOut );
	BOOL bHasAddSkillProperty=pGoods->HasAddonProperty(GAP_ADD_SKILL);
	if(bHasAddSkillProperty)
	{
		long lSkillId=pGoods->GetAddonPropertyValues(GAP_ADD_SKILL,1);
		DWORD lSkillLev=pGoods->GetAddonPropertyValues(GAP_ADD_SKILL,2);
		if(bMount)
		{
			//��Ӽ���
			if(CheckStudySkill(lSkillId,lSkillLev))
			{
				StudySkill(lSkillId,lSkillLev);
			}
		}
		else
		{
			//ɾ������
			DelSkill(lSkillId,lSkillLev);
		}
	}

	// ����ǡ������ػ�ʯ������Ʒ������䡮�����������Ƿ���Ч����Ч�����θ�������
	//if( !bGuardianFlag )
	//{
	//	long lCurEnergy = pGoods->GetGuardianEnergy();
	//	if( lCurEnergy == 0 )
	//	{
	//		// ��������
	//		return ;
	//	}
	//}

	//for( size_t i = 0; i < vOut.size(); i++ )
	//{
	//	switch( vOut[i] ) 
	//	{			
	//	case CGoodsBaseProperties::GAP_ADD_CON://����׷��
	//		{
	//			long lret=pGoods -> GetAddonPropertyValues( vOut[i], 1 );
	//			string strAttrName="";
	//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();					
	//			long lVal=lret * (long)pSetup->fCon2MaxHp[GetOccupation()];
	//			if(bMount)
	//			{	
	//				strAttrName="E_Consti";
	//				ChangeAttribute(strAttrName,lret);
	//			}
	//			else
	//			{					
	//				strAttrName="E_Consti";
	//				DWORD dwCurHp=GetHP();
	//				ChangeAttribute(strAttrName,-lret);		
	//				if(dwCurHp>GetMaxHP())
	//				{
	//					strAttrName="Hp";
	//					ChangeAttribute(strAttrName,GetMaxHP());
	//				}
	//				//strAttrName="Hp";
	//				//ChangeAttribute(strAttrName,GetMaxHP());
	//			}						
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_ADD_MAXHP_LEVEL:	//HP�����������
	//		{
	//			long lVal=pGoods->GetAddonPropertyValues(vOut[i],1);
	//			string strAttrName="E_MaxHp";
	//			if(bMount)
	//			{
	//				ChangeAttribute(strAttrName,lVal);
	//			}
	//			else
	//			{
	//				ChangeAttribute(strAttrName,-lVal);
	//				/*
	//				if(GetHP()>GetMaxHP())
	//				{
	//				strAttrName="Hp";
	//				ChangeAttribute(strAttrName,GetMaxHP());
	//				}*/
	//			}
	//		}
	//		break;		
	//	case CGoodsBaseProperties::GAP_MAXHP_CORRECT:	//������������	
	//		{
	//			ChangePropertyByGoods(pGoods,CGoodsBaseProperties::GAP_MAXHP_CORRECT,1,"E_MaxHp",bMount);	
	//			if(!bMount)
	//			{
	//				// жװ
	//				if(GetHP() > GetMaxHP())
	//				{
	//					string strAttrName="Hp";
	//					ChangeAttribute(strAttrName,GetMaxHP());						
	//				}
	//			}
	//			else
	//			{

	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_RANDOM_ADDON_MAXHP:	//���������������	
	//		{
	//			ChangePropertyByGoods(pGoods,CGoodsBaseProperties::GAP_RANDOM_ADDON_MAXHP,1,"E_MaxHp",bMount);	
	//			if(!bMount)
	//			{
	//				// жװ
	//				if(GetHP() > GetMaxHP())
	//				{
	//					string strAttrName="Hp";
	//					ChangeAttribute(strAttrName,GetMaxHP());						
	//				}
	//			}
	//			else
	//			{

	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_ADD_MAX_PKCP: // PK CPֵ��������(���)
	//		{
	//			ChangePropertyByGoods( pGoods, CGoodsBaseProperties::GAP_ADD_MAX_PKCP, 1, "E_MaxPKCP", bMount );
	//			if( !bMount )
	//			{
	//				// жװ��Ҫ�ʵ�������ǰֵ
	//				long maxcp = GetMaxPKCP();
	//				if( GetPKCP() > maxcp )
	//				{
	//					string strAttrName = "CurPKCP";
	//					SetAttribute( strAttrName, maxcp );
	//				}
	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_TIMES_MAX_PKCP: // PK CPֵ���ޱ��ʱ仯
	//		{
	//			// ���ʷ����仯����Ҫ��������
	//			AddAttributeToDirty( "C_MaxPKCP" );
	//			if( !bMount )
	//			{
	//				// жװ��Ҫ�ʵ�������ǰֵ
	//				long maxcp = GetMaxPKCP();
	//				if( GetPKCP() > maxcp )
	//				{
	//					string strAttrName = "CurPKCP";
	//					SetAttribute( strAttrName, maxcp );
	//				}
	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_MAXMP_CORRECT://������������
	//		{
	//			ChangePropertyByGoods(pGoods,CGoodsBaseProperties::GAP_MAXMP_CORRECT,1,"E_MaxMp",bMount);	
	//			if(!bMount)
	//			{
	//				// жװ
	//				if(GetMP() > GetMaxMP())
	//				{
	//					string strAttrName="Mp";
	//					ChangeAttribute(strAttrName,GetMaxMP());						
	//				}
	//			}
	//			else
	//			{

	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_MOUNT_MAXHP://������������׷��
	//		{
	//			ChangePropertyByGoods(pGoods,CGoodsBaseProperties::GAP_MOUNT_MAXHP,1,"E_MaxHp",bMount);	
	//			if(!bMount)
	//			{
	//				// жװ
	//				if(GetHP() > GetMaxHP())
	//				{
	//					string strAttrName="Hp";
	//					ChangeAttribute(strAttrName,GetMaxHP());						
	//				}
	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_ELEMENT_1://! ��Ʒħ������1��ֵ1����ħ�����ԣ�ֵ2��ʾħ���ȼ�
	//	case CGoodsBaseProperties::GAP_ELEMENT_2://! ��Ʒħ������2��ֵ1����ħ�����ԣ�ֵ2��ʾħ���ȼ�
	//	case CGoodsBaseProperties::GAP_ELEMENT_3://! ��Ʒħ������3��ֵ1����ħ�����ԣ�ֵ2��ʾħ���ȼ�
	//		{
	//			vector<long> vPro;
	//			pGoods->GetAddonPropertyValues(vOut[i], vPro);
	//			if (2 > vPro.size() || eGET_Disabled == vPro[0] || eGET_Enabled == vPro[0])
	//				break;

	//			GoodsExSetup::tagElementValue RetagElementValue;
	//			if(!GoodsExManage::GetInstance().GetGoodsExSetup().GetElementValue(pGoods->GetAddonPropertyValues(CGoodsBaseProperties::GAP_ROLE_MINIMUM_LEVEL_LIMIT, 1), vPro[1], RetagElementValue))
	//				break;

	//			DWORD dwElementType = vPro[0] % 10;
	//			if(CGoodsFactory::IsAttEquipment(pGoods))
	//			{
	//				AddElementProperty(dwElementType, AD_Attack, RetagElementValue.lAtkValue * ((bMount)? 1 : -1));
	//			}
	//			else
	//			{
	//				AddElementProperty(dwElementType, AD_Defend, RetagElementValue.lDefValue * ((bMount)? 1 : -1));
	//				for (long i = 0; i < ET_Max; ++i)
	//				{
	//					if(i != dwElementType)
	//						AddElementProperty(i, AD_Defend, RetagElementValue.lAffixValue * ((bMount)? 1 : -1));
	//				}
	//			}
	//		}
	//		break;
	//	case CGoodsBaseProperties::GAP_ELEMENT_XIANG_XING_1://! ħ������1
	//	case CGoodsBaseProperties::GAP_ELEMENT_XIANG_XING_2://! ħ������2
	//	case CGoodsBaseProperties::GAP_ELEMENT_XIANG_XING_3://! ħ������3
	//		{
	//			vector<long> vPro;
	//			pGoods->GetAddonPropertyValues(vOut[i], vPro);
	//			if (2 > vPro.size() || eGET_Disabled == vPro[0] || eGET_Enabled == vPro[0])
	//				break;

	//			DWORD dwElementType = vPro[0] % 10;
	//			if(CGoodsFactory::IsAttEquipment(pGoods))
	//				AddElementProperty(dwElementType, AD_AttackXiang, vPro[1] * ((bMount)? 1 : -1));
	//			else
	//				AddElementProperty(dwElementType, AD_DefendXiang, vPro[1] * ((bMount)? 1 : -1));
	//		}
	//		break;
	//	default:
 //           {
 //               string strAttrName = CPlayer::GetFightPropertyNameByEnum(vOut[i]);
 //               if(strAttrName != "")
 //               {
 //                   ChangePropertyByGoods(pGoods, vOut[i], 1, strAttrName, bMount);
 //               }
 //           }
	//		break;
	//	}
	//}

	UpdatePropertyByGoodsTrigger(pGoods,bMount);


	if(pGoods->IsSuitGoods())
	{
		LONG lSuitId=pGoods->GetAddonPropertyValues(GAP_SUITID,1);		

		map<long,vector<tagSuitGoods>*>::iterator suitIter=m_mSuitState.find(lSuitId);
		if(suitIter!=m_mSuitState.end())//�Ѿ��ж�ӦID����װ״̬
		{				
			if(bMount)
			{
				//vector���Ƿ������ͬԭʼ������Ʒ
				BOOL bSameOriginName=FALSE;
				vector<tagSuitGoods>::iterator goodsIter=suitIter->second->begin();
				const CHAR* pOriginName=GoodsSetup::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
				for(;goodsIter!=suitIter->second->end();goodsIter++)
				{
					const CHAR* pName=GoodsSetup::QueryGoodsOriginalName(goodsIter->pGoods->GetBasePropertiesIndex());
					if(strcmp(pName,pOriginName)==0)
					{
						bSameOriginName=TRUE;
						break;
					}
				}

				int nSize=(int)suitIter->second->size();

				if(!bSameOriginName)
				{
					tagSuitGoods stSuitGoods;
					stSuitGoods.bActived=FALSE;
					stSuitGoods.pGoods=pGoods;
					suitIter->second->push_back(stSuitGoods);	

					if(CSuitRuleFactory::SuitActive(this,nSize,pGoods))
					{							
						SendNotifyMessage("��װЧ��������");
					}
					OnSuitNumChanged(lSuitId,(int)suitIter->second->size());
				}																
			}
			else
			{	
				//Ŀǰװ�����Ƿ�����ͬԭʼ����Ʒ
				vector<CGoods*> goodsVec;
				m_cEquipment.GetGoodsByID(pGoods->GetBasePropertiesIndex(),goodsVec);
				CGoods* pNextGoods=NULL;
				if(goodsVec.size()>=1)
					pNextGoods=goodsVec[0];
				//��SUITMAP���Ƿ����
				BOOL bInMap=FALSE;
				vector<tagSuitGoods>::iterator goodsIter=suitIter->second->begin();				
				for(;goodsIter!=suitIter->second->end();goodsIter++)
				{					
					if(pGoods==goodsIter->pGoods)
					{
						bInMap=TRUE;
						break;
					}
				}

				if(bInMap)
				{				
					//goodsVec��Ϊ��˵����װ����ͬ������װ��Ʒ	
					if(!pNextGoods)
					{
						if(CSuitRuleFactory::SuitInvalidate(this,(int)suitIter->second->size(),pGoods))
						{
						}
						pGoods->InvalidSuitProperties(this);					
						suitIter->second->erase(goodsIter);					
						OnSuitNumChanged(lSuitId,(int)suitIter->second->size());
					}
					else
					{
						pGoods->InvalidSuitProperties(this);
						suitIter->second->erase(goodsIter);
						tagSuitGoods stVal;
						stVal.bActived=FALSE;
						stVal.pGoods=pNextGoods;
						suitIter->second->push_back(stVal);
						OnSuitNumChanged(lSuitId,(int)suitIter->second->size());
					}					
				}				
			}
		}
		else
		{
			vector<tagSuitGoods>* pVec=new vector<tagSuitGoods>;
			tagSuitGoods stSuitGoods;
			stSuitGoods.bActived=FALSE;
			stSuitGoods.pGoods=pGoods;
			pVec->push_back(stSuitGoods);
			m_mSuitState[lSuitId]=pVec;
		}
	}
}

int CPlayer::MountFuMoProperty( eGoodsAddonProperties type, int value )
{
	// �����ħֵΪ0��ֱ�ӷ��ء����Բ����ı䡣
	if(!value) return 0;

	switch( type ) 
	{			
	case 14:	//��С������
		{
			long lret = GetMinAtk() + value;
			if(value > 0)
			{
				SetMinAtk((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetMinAtk((WORD)lret);
				else
					SetMinAtk(0);
			}					
		}
		break;
	case 15:	//��󹥻���
		{
			long lret = (long)GetMaxAtk() + value;
			if(value > 0)
			{
				SetMaxAtk((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetMaxAtk((WORD)lret);
				else
					SetMaxAtk(0);
			}					
		}
		break;
		//case 16:	//��������ϵ��
		//	{	
		//		long lret = GetElementModify() + value;
		//		if(value > 0)
		//		{
		//			SetElementModify(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetElementModify(lret);
		//			else
		//				SetElementModify(0);
		//		}					
		//	}
		//	break;
	case 17:	//���������
		{
			long lret = GetDef() + value;
			if(value > 0)
			{
				SetDef((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetDef((WORD)lret);
				else
					SetDef(0);
			}
		}
		break;
		//case 18:	//�����ٶ�����
		//	{
		//		long lret  =GetAtcSpeed() + value;
		//		if(value > 0)
		//		{
		//			SetAtcSpeed(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetAtcSpeed(lret);
		//			else
		//				SetAtcSpeed(0);
		//		}					
		//	}
		//	break;
	case 19:	//��������
		{
			long lret=GetHit() + value;
			if(value > 0)
			{
				SetHit((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetHit((WORD)lret);
				else
					SetHit(0);
			}
		}
		break;
		//case 20:	//����һ��
		//	{
		//		long lret=GetCCH() + value;
		//		if(value > 0)
		//		{
		//			SetCCH(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetCCH(lret);
		//			else
		//				SetCCH(0);
		//		}					
		//	}
		//	break;
	case 21:	//��������
		{
			long lret=GetDodge() + value;
			if(value > 0)
			{
				SetDodge((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetDodge((WORD)lret);
				else
					SetDodge(0);
			}					
		}
		break;
	case 22:	
		{					
		}
		break;
		//case 23:	//������������
		//	{
		//		long lret=GetElementResistant() + value;
		//		if(value > 0)
		//		{
		//			SetElementResistant(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetElementResistant(lret);
		//			else
		//				SetElementResistant(0);
		//		}					
		//	}
		//	break;
	case 24:	
		{
		}
		break;
	case 25:	//�����ָ��ٶ�
		{
			long lret=GetHpRecoverSpeed() + value;
			if(value > 0)
			{
				SetHpRecoverSpeed((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetHpRecoverSpeed((WORD)lret);
				else
					SetHpRecoverSpeed(0);							
			}					
		}
		break;
	case 26:	//�����ָ��ٶ�
		{
			long lret=GetMpRecoverSpeed() + value;
			if(value > 0)
			{
				SetMpRecoverSpeed((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetMpRecoverSpeed((WORD)lret);
				else
					SetMpRecoverSpeed(0);							
			}					
		}
		break;
		//case 27:	//����׷��
		//	{
		//		long lret=GetStr() + value;
		//		if(value > 0)
		//		{
		//			SetStr(lret);
		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sStrChange = short( value );
		//			SetMaxAtk(WORD(m_Property.wMaxAtk + sStrChange * pSetup->fStr2MaxAtk[GetOccupation()]));
		//			SetBurden(WORD(m_Property.wBurden + sStrChange * pSetup->fStr2Burden[GetOccupation()]));
		//		}
		//		else
		//		{
		//			if(lret>=0)							
		//				SetStr(lret);							
		//			else							
		//				SetStr(0);
		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sStrChange = short( value );

		//			float fVal=m_Property.wMaxAtk + sStrChange * pSetup->fStr2MaxAtk[GetOccupation()];
		//			if(fVal>=0)
		//				SetMaxAtk(fVal);
		//			else
		//				SetMaxAtk(0);

		//			fVal=m_Property.wBurden + sStrChange * pSetup->fStr2Burden[GetOccupation()];
		//			if(fVal>=0)
		//				SetBurden(fVal);
		//			else
		//				SetBurden(0);
		//		}					
		//	}
		//	break;
		//case 28://����׷��
		//	{
		//		long lret=GetDex() + value;
		//		if(value > 0)
		//		{
		//			SetDex(lret);
		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sDexChange = short( value );
		//			SetMinAtk(WORD(m_Property.wMinAtk + sDexChange * pSetup->fDex2MinAtk[GetOccupation()]));
		//			SetReAnk(WORD(m_Property.wReAnk + sDexChange * pSetup->fDex2Stiff[GetOccupation()]));

		//		}
		//		else
		//		{
		//			if(lret>0)
		//				SetDex(lret);
		//			else
		//				SetDex(0);

		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sDexChange = short( value );

		//			float fVal=m_Property.wMinAtk + sDexChange * pSetup->fDex2MinAtk[GetOccupation()];
		//			if(fVal>=0)
		//				SetMinAtk(fVal);
		//			else
		//				SetMinAtk(0);

		//			fVal=m_Property.wReAnk + sDexChange * pSetup->fDex2Stiff[GetOccupation()];
		//			if(fVal>=0)
		//				SetReAnk(fVal);
		//			else
		//				SetReAnk(0);
		//		}				
		//	}
		//	break;
	case 29://����׷��
		{
			long lret=GetCon() + value;
			if(value > 0)
			{
				SetCon((WORD)lret);
				GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
				short sConChange = short( value );

				/*SetMaxHP(WORD(m_Property.wMaxHp + sConChange * pSetup->fCon2MaxHp[GetOccupation()]));
				SetDef(WORD(m_Property.wDef + sConChange * pSetup->fCon2Def[GetOccupation()]));*/
				SetMaxHP(DWORD(m_FightProperty.dwMaxHp + sConChange * pSetup->fCon2MaxHp[GetOccupation()]));
				SetDef(WORD(m_FightProperty.wDef + sConChange * pSetup->fCon2Def[GetOccupation()]));
			}
			else
			{
				if(lret>=0)
					SetCon((WORD)lret);
				else
					SetCon(0);

				GlobalSetup::tagSetup *pSetup = NULL;// GlobalSetup::SetConsti();
				short sConChange = short( value );
				float fVal=m_FightProperty.dwMaxHp + sConChange * pSetup->fCon2MaxHp[GetOccupation()];
				if(fVal>=0)
					SetMaxHP((DWORD)fVal);
				else
					SetMaxHP(0);
				fVal=m_FightProperty.wDef + sConChange * pSetup->fCon2Def[GetOccupation()];
				if(fVal>=0)
					SetDef((WORD)fVal);
				else
					SetDef(0);
			}									
		}
		break;
		//case 30://����׷��
		//	{
		//		long lret=GetInt() + value;
		//		if(value > 0)
		//		{
		//			SetInt(lret);
		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sIntChange =short( value );

		//			SetElementModify(WORD(m_Property.sElementModify + sIntChange * pSetup->fInt2Element[GetOccupation()]));
		//			SetMaxMP(WORD(m_Property.wMaxMp + sIntChange * pSetup->fInt2MaxMp[GetOccupation()]));
		//			SetElementResistant(WORD(m_Property.wElementResistant + sIntChange * pSetup->fInt2Resistant[GetOccupation()]));
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetInt(lret);
		//			else
		//				SetInt(0);
		//			GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
		//			short sIntChange =short( value );

		//			float fVal=m_Property.sElementModify + sIntChange * pSetup->fInt2Element[GetOccupation()];
		//			if(fVal>=0)
		//				SetElementModify(fVal);
		//			else
		//				SetElementModify(0);

		//			fVal=m_Property.wMaxMp + sIntChange * pSetup->fInt2MaxMp[GetOccupation()];
		//			if(fVal>=0)
		//				SetMaxMP(fVal);
		//			else
		//				SetMaxMP(0);

		//			fVal=m_Property.wElementResistant + sIntChange * pSetup->fInt2Resistant[GetOccupation()];
		//			if(fVal>=0)
		//				SetElementResistant(fVal);
		//			else
		//				SetElementResistant(0);
		//		}					
		//	}
		//	break;
	case 31:	//������������	
		{
			long lret=GetMaxHP() + value;
			if(value > 0)
			{
				SetMaxHP((DWORD)lret);
			}
			else
			{
				if(lret>=0)
					SetMaxHP((DWORD)lret);
				else
					SetMaxHP(0);
			}				
		}
		break;
	case 32://������������
		{
			long lret=GetMaxMP() + value;
			if(value > 0)
			{
				SetMaxMP((WORD)lret);
			}
			else
			{
				if(lret>=0)
					SetMaxMP((WORD)lret);
				else
					SetMaxMP(0);
			}					
		}
		break;
	case 33:
		{
		}
		break;
	case 34:
		{
		}
		break;
	case 35:
		{
		}
		break;
	case 36:
		{
		}
		break;
	case 37:
		{
		}
		break;
	case 38:
		{
		}
		break;
	case 39:
		{
		}
		break;
	case 40:
		{
		}
		break;
	case 41:
		{
		}
		break;
	case 42:
		{
		}
		break;
	case 43:
		{
		}
		break;
	case 44:
		{
		}
		break;
	case 45:
		{
		}
		break;
	case 46:
		{
		}
		break;
	case 47:
		{
		}
		break;
		//case 51:	//Ӳֱ����
		//	{
		//		long lret=GetReAnk() + value;
		//		if(value > 0)
		//		{
		//			SetReAnk(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetReAnk(lret);
		//			else
		//				SetReAnk(0);
		//		}
		//	}
		//	break;
		//case 52:	//��������
		//	{
		//		long lret=GetBurden() + value;
		//		if(value > 0)
		//		{
		//			SetBurden(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetBurden(lret);
		//			else
		//				SetBurden(0);
		//		}
		//	}
		//	break;
		//case 91:	// �������
		//	{
		//		long lret=GetAttackAvoid() + value;
		//		if(value > 0)
		//		{
		//			SetAttackAvoid(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetAttackAvoid(lret);
		//			else
		//				SetAttackAvoid(0);
		//		}
		//	}
		//	break;
		//case 92:	// ħ�����
		//	{
		//		long lret=GetElementAvoid() + value;
		//		if(value > 0)
		//		{
		//			SetElementAvoid(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetElementAvoid(lret);
		//			else
		//				SetElementAvoid(0);
		//		}
		//	}
		//	break;
		//case 93:	// ���ܼ���
		//	{
		//		long lret=GetFullMiss() + value;
		//		if(value > 0)
		//		{
		//			SetFullMiss(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetFullMiss(lret);
		//			else
		//				SetFullMiss(0);
		//		}
		//	}
		//	break;
		//case 95:	// ����������
		//	{
		//		long lret=GetBlastAttack() + value;
		//		if(value > 0)
		//		{
		//			SetBlastAttack(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetBlastAttack(lret);
		//			else
		//				SetBlastAttack(0);
		//		}
		//	}
		//	break;
		//case 96:	// ������������
		//	{
		//		long lret=GetBlastAttack() + value;
		//		if(value > 0)
		//		{
		//			SetBlastElementAttack(lret);
		//		}
		//		else
		//		{
		//			if(lret>=0)
		//				SetBlastElementAttack(lret);
		//			else
		//				SetBlastElementAttack(0);
		//		}
		//	}
		//	break;

	case 97:	// ��ħ
		{
			break;
		}
		break;
	default:
		break;
	}
	return value;
}

void CPlayer::InitFightEnumMap()
{
	//m_mapFightEnum[GAP_MAXHP_CORRECT]="E_MaxHp";//��������			
	//m_mapFightEnum[GAP_MAXMP_CORRECT]="E_MaxMp";//��������			
	//m_mapFightEnum[GAP_HP_RESUME_CORRECT]="E_HpRecoverSpeed";//�����ָ��ٶ�			
	//m_mapFightEnum[GAP_MP_RESUME_CORRECT]="E_MpRecoverSpeed";//�����ָ��ٶ�			
	//m_mapFightEnum[GAP_ADD_STR]="E_Str";//����			
	//m_mapFightEnum[GAP_ADD_DEX]="E_Dex";//����			
	//m_mapFightEnum[GAP_ADD_CON]="E_Con";//����		
	//m_mapFightEnum[GAP_ADD_INT]="E_Intellect";//����
	//m_mapFightEnum[GAP_ADD_SPI]="E_Spiritualism";//����			
	//m_mapFightEnum[GAP_PARRY_CORRECT]="E_Block";//��			
	//m_mapFightEnum[GAP_BLOCK_CORRECT]="E_Parry";//�м�			
	//m_mapFightEnum[GAP_FLEE_CORRECT]="E_Dodge";//����			
	//m_mapFightEnum[GAP_HIT_RATE_CORRECT]="E_Hit";//����
	//m_mapFightEnum[GAP_DEDICANT_CORRECT]="E_Absorb";//רע			
	//m_mapFightEnum[GAP_NOR_DEF]="E_Def";//�������	
	//m_mapFightEnum[GAP_MAGIC_DEF]="E_Mdef";//ħ������			
	//m_mapFightEnum[GAP_NORATTACK_KILLRATE]="E_Cri";//������			
	//m_mapFightEnum[GAP_MAGIC_SPEED]="E_MAtkSpeed";//ʩ���ٶ�			
	//m_mapFightEnum[GAP_MIN_ATTACK]="E_MinAtk";//��С������
	//m_mapFightEnum[GAP_MAX_ATTACK]="E_MaxAtk";//��󹥻���
	//m_mapFightEnum[GAP_FUMO_PROPERTY]="E_MAtk";//ħ��������			
	//m_mapFightEnum[GAP_MAGIC_KILLRATE]="E_MCri";//ħ��������			
	//m_mapFightEnum[GAP_LEECH_HP]="E_BloodSuk";//����������			
	//m_mapFightEnum[GAP_LEECH_MP]="E_ManaSuck";//����������		
	//m_mapFightEnum[GAP_WILL_DEF]="E_WillDef";//��־����		
	//m_mapFightEnum[GAP_CON_DEF]="E_ConstiDef";//���ʿ���
	//m_mapFightEnum[GAP_BLED_DEF]="E_DeBuffDef0";//��Ѫ����
	//m_mapFightEnum[GAP_DIZZY_DEF]="E_DeBuffDef1";//���迹��
	//m_mapFightEnum[GAP_LULL_DEF]="E_DeBuffDef2";//��Կ���
	//m_mapFightEnum[GAP_HYPNOSIS_DEF]="E_DeBuffDef3";//���߿���
	//m_mapFightEnum[GAP_POISON_DEF]="E_DeBuffDef4";//�ж�����
	//m_mapFightEnum[GAP_FIX_DEF]="E_DeBuffDef5";//������
	//m_mapFightEnum[GAP_SILENCE_DEF]="E_DeBuffDef6";//��Ĭ����
	//m_mapFightEnum[GAP_FIREDEF_CORRECT]="E_ElemDef0";//��		
	//m_mapFightEnum[GAP_WATERDEF_CORRECT]="E_ElemDef1";//ˮ��
	//m_mapFightEnum[GAP_EARTHDEF_CORRECT]="E_ElemDef2";//�ؿ�	
	//m_mapFightEnum[GAP_WINDDEF_CORRECT]="E_ElemDef3";//�翹
	//m_mapFightEnum[GAP_DARKDEF_CORRECT]="E_ElemDef4";//�ڰ���
	//m_mapFightEnum[GAP_HOLYDEF_CORRECT]="E_ElemDef5";//��ʥ��	
	//m_mapFightEnum[GAP_PENETRATE]="E_Pierce";//��͸
	//m_mapFightEnum[GAP_EXCUSE_HURT]="E_Immunity";//����
	//m_mapFightEnum[GAP_LUCKY]="E_Luck";//����
}

string CPlayer::GetFightPropertyNameByEnum(eGoodsAddonProperties enumVal)
{
	map<eGoodsAddonProperties,string>::iterator nameIter=m_mapFightEnum.find(enumVal);
	if(nameIter!=m_mapFightEnum.end())
	{
		return nameIter->second;
	}
	return "";
}



BOOL CPlayer::RestoreHp( DWORD dwHp, DWORD dwPersistTime, DWORD  dwFrequency)
{
	//##�����ǰʱ�仹û�е�����Сʹ�ü��.
	if( timeGetTime() < m_dwLastRestoreHpTimeStamp + GlobalSetup::GetSetup() -> dwHealTimeInterval )
	{
		return FALSE;
	}

	m_dwLastRestoreHpTimeStamp = timeGetTime();

	return FALSE;
}

BOOL CPlayer::RestoreMp( DWORD dwMp, DWORD dwPersistTime, DWORD dwFrequency )
{
	//##�����ǰʱ�仹û�е�����Сʹ�ü��.
	if( timeGetTime() < m_dwLastRestoreMpTimeStamp + GlobalSetup::GetSetup() -> 
		dwMpTimeInterval )
	{
		return FALSE;
	}

	m_dwLastRestoreMpTimeStamp = timeGetTime();
	return FALSE;
}

void CPlayer::UseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid)
{	
	//  [7/19/2007 chenxianjin]
	//�в���ʹ��װ����״̬
	if (!GetUseItem() || IsLockPlayerState() )
	{
		return;
	}
	CGoods* pGoods=NULL;
	CS2CContainerObjectMove::PLAYER_EXTEND_ID place=static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(wPlace);

	CVolumeLimitGoodsContainer* pVolumeContainer=NULL;

	if(place==CS2CContainerObjectMove::PEI_PACKET)
	{
		pVolumeContainer=getOriginPack();
		pGoods=getOriginPack()->GetGoods(lPos);
	}
	else if(place>=CS2CContainerObjectMove::PEI_PACK1 && place<=CS2CContainerObjectMove::PEI_PACK5)
	{
		pVolumeContainer=m_pPackage.GetSubpack(place-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
		pGoods=pVolumeContainer->GetGoods(lPos);
	}	
	CGoodsBaseProperties* pGoodsProperty=GoodsSetup::QueryGoodsBaseProperties(pGoods->GetBasePropertiesIndex());
	if(!pGoodsProperty)
		return;
	if(GetPkValue()>20)
	{
		if(strcmp(pGoodsProperty->GetOriginalName(),"GZ0203")==0 || 
			strcmp(pGoodsProperty->GetOriginalName(),"GZ0204")==0 || 
			strcmp(pGoodsProperty->GetOriginalName(),"GZ0205")==0 || 
			strcmp(pGoodsProperty->GetOriginalName(),"GZ0206")==0)
		{
			const char* strMsg=AppFrame::GetText("49");
			if(strMsg)
				SendSystemInfo(strMsg,0xFFFFFFFF);	
			return;
		}
	}


	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pGoods && pRegion )
	{
		if(IsInPersonalShop(pGoods))
		{
			const char* strMsg=AppFrame::GetText("46");
			if(strMsg)
				SendSystemInfo(strMsg,0xFFFFFFFF);	
			return;
		}
		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(
			pGoods -> GetBasePropertiesIndex() );
		if( pProperties && pProperties -> GetGoodsBaseType() == GBT_CONSUMABLE )
		{
			BOOL bCanUse = TRUE;

			//�жϸó����Ƿ�֧��ʹ�ø���Ʒ
			EquipRestrict eRet = EQUIPFIT;
			if( pRegion->FindForbidGood(pProperties->GetOriginalName()) == true)
			{
				eRet = REGION_FORBID;
			}

			if( eRet==EQUIPFIT )
			{
				eRet = (EquipRestrict)CanUseItem( pGoods );
			}

			if(eRet!=EQUIPFIT)
			{
				bCanUse = FALSE;
			}

			if( bCanUse && pGoods -> GetAmount() > 0 )
			{
				if(strcmp(pGoodsProperty->GetOriginalName(),"GZ0201")==0 || 
					strcmp(pGoodsProperty->GetOriginalName(),"GZ0201P")==0)
				{
					if(!pRegion->GetSetup().bRuneStoneIsUsed)
					{
						const char* strMsg=AppFrame::GetText("42");
						if(strMsg)
							SendSystemInfo(strMsg,0xFFFFFFFF);
						return;
					}
				}
				
				BOOL bItemUsed		= TRUE;
				BOOL bBackToCity	= FALSE;
				vector<eGoodsAddonProperties> vOut;
				pGoods -> GetEnabledAddonProperties( vOut );

				//�����ʹ���������Ʒ,�򲻽���ѭ��
				if( pGoods -> GetAddonPropertyValues(GAP_MOUNT_TYPE, 1) )
				{			
						//##��̯��ս��״̬�²���������
						if( GetCurrentProgress() != PROGRESS_OPEN_STALL && m_lFightStateCount == 0 )
						{
							const char *strGoodOriName = GoodsSetup::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
							if( Mount( pGoods -> GetAddonPropertyValues(GAP_MOUNT_TYPE, 1), 
								pGoods -> GetAddonPropertyValues(GAP_MOUNT_LEVEL, 1), 
								pGoods -> GetAddonPropertyValues(GAP_MOUNT_ROLE_LIMIT, 1),
								strGoodOriName) )
							{
								//##�����������ʹ��,�򲻼�������
								if( pGoods -> GetAddonPropertyValues(GAP_UNLIMITED_ACCESS, 1) == 1 )
								{
									bItemUsed = FALSE;
								}
							}
							else
							{
								bItemUsed = FALSE;
							}
						}
						else
						{
							bItemUsed = FALSE;
						}
					
				}
				else
				{
					//! pGoods -> GetEnabledAddonProperties( vOut );
				}

				for( size_t i = 0; i < vOut.size(); i++ )
				{
					switch( vOut[i] )
					{						
					case GAP_ADD_HP:
						{
							if( pGoods -> GetAddonPropertyValues(vOut[i], 2) == 0 )
							{
								bItemUsed = RestoreHp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 0, 400 );

							}
							else
							{
								bItemUsed = RestoreHp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 
									pGoods -> GetAddonPropertyValues(vOut[i], 2) , 400 );
							}
						}
						break;
					case GAP_ADD_PERCENT_HP:	//�����ٷֱ�����
						{
							bItemUsed = RestoreHp( static_cast<DWORD>( GetMaxHP() * ( pGoods -> GetAddonPropertyValues(vOut[i], 1) / 100.0f) ),
								0, 400 );
						}
						break;
					case GAP_ADD_MP:	//��������
						{
							if(pGoods -> GetAddonPropertyValues( vOut[i], 2 )==0)
							{
								bItemUsed = RestoreMp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 0, 400 );
							}
							else
							{
								bItemUsed = RestoreMp( pGoods -> GetAddonPropertyValues(vOut[i], 1), (pGoods -> GetAddonPropertyValues(vOut[i], 2) - 1) * 400, 400 );
							}
						}
						break;
					case GAP_ADD_PERCENT_MP:	//�����ٷֱ�����
						{
							bItemUsed = RestoreMp( static_cast<DWORD>( GetMaxMP() * (pGoods -> GetAddonPropertyValues(vOut[i], 1) / 100.0f) ),
								0, 400 );
						}
						break;
					case GAP_HOME:	//�س�
						{
							// ��������������ʹ�ûس�
							if( bCanUse )
							{
								if(GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
									|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING)
								{
									bItemUsed=FALSE;
								}
								else if( pRegion->GetCountry() && pRegion->GetCountry() != GetCountry() )
								{
									bItemUsed = FALSE;
									const char* strMsg=AppFrame::GetText("41");
									if(strMsg)
										SendSystemInfo(strMsg,0xFFFFFFFF);									
								}
								else if(!pRegion->GetSetup().bRuneStoneIsUsed)
								{
									if(pRegion->GetSetup().lRuneStoneRegionID == 0) // ԭ��
									{
									}
									else if(pRegion->GetSetup().lRuneStoneRegionID == -1) // �ع���									{
									{
									}
									else if(pRegion->GetSetup().lRuneStoneRegionID>0) // > 0 �л�����ID����
									{

									}
									bItemUsed=FALSE;
									const char* strMsg=AppFrame::GetText("42");
									if(strMsg)
										SendSystemInfo(strMsg,0xFFFFFFFF);
								}
								else
								{
									bBackToCity = TRUE;
								}
							}
							else
								bItemUsed = FALSE;

						}
						break;
					case GAP_RAN_TRANS:	//�������
						{
							// ֻ���ڱ�����ʹ���������
							if(bCanUse)
							{
								if(GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
									|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING)
								{
									bItemUsed=FALSE;
								}
								else if( pRegion->GetCountry() != GetCountry() )
								{
									bItemUsed = FALSE;
									const char* strMsg=AppFrame::GetText("43");
									if(strMsg)
										SendSystemInfo(strMsg,0xFFFFFFFF);
								}
								else
								{
									CServerRegion* pRegion=((CServerRegion*)GetFather());
									if(pRegion)
									{
										//##��ʹ�ûس̾������npc���͵�ʱ�������Ʒ
										DropParticularGoodsWhenRecall();

										long lX = -1, lY = -1;
										pRegion->GetRandomPos(lX, lY);

										// [071127 AHC REGION]
										ChangeRegion(GetCurRgnType(), GetRegionGUID(),lX, lY,GetDir());
									}
								}
							}
							else 
								bItemUsed = FALSE;
						}
						break;
					case GAP_SCRIPT:	//ִ�нű�
						{
							char strFile[50];
							_snprintf(strFile, 50, "scripts/Goods/%d.lua",pGoods -> GetAddonPropertyValues(vOut[i], 1));
							if( !m_VarList->HasVar( "#m_strUseItem" ) )
							{
								m_VarList->AddVar( "#m_strUseItem", tString("") );
							}
							m_VarList->SetVarValue( "#m_strUseItem", tString( pProperties->GetOriginalName() ) );
							
							Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ), m_VarList );
							GetInst( ScriptSys ).RunScript( env, strFile );
						}
						break;
					case GAP_ADD_STATE:
						{
							DWORD dwStateId=pGoods -> GetAddonPropertyValues(vOut[i],1);
							DWORD dwLevel=pGoods -> GetAddonPropertyValues(vOut[i],2);	
							AddNewState(this,dwStateId,dwLevel,16);
						}
						break;
					case GAP_POWER_INTENSITY:
						{
							CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON);
							bItemUsed = GoodsExManage::GetInstance().ReinforcePower(pArm, pGoods, this);				
						}
						break;						
					}
				}

				BOOL bHasRecordRegionAttr=pGoods->HasAddonProperty(GAP_RECORD_REGIONID);
				BOOL bHasRecordCoorAttr=pGoods->HasAddonProperty(GAP_RECORD_COOR);
				BOOL bReduce=TRUE;
				if(bHasRecordCoorAttr && bHasRecordRegionAttr)
				{
					//��Ʒ�����ط�
					LONG lRegionId=pGoods->GetAddonPropertyValues(GAP_RECORD_REGIONID,1);
					LONG lXPos=pGoods->GetAddonPropertyValues(GAP_RECORD_COOR,1);
					LONG lYPos=pGoods->GetAddonPropertyValues(GAP_RECORD_COOR,2);
					if(lRegionId==0)
					{
						//δ��¼
						CServerRegion* pRegion=((CServerRegion*)GetFather());
						if(pRegion)
						{
							if(pRegion->GetSetup().bRuneStoneCanRecord)
							{	
								//�ж���Ӫ
								CCountry *pCountry = GetCountryHandler()->GetCountry(GetCountry());
								CCountry* pRgnCountry=GetCountryHandler()->GetCountry(pRegion->GetCountry());
								if(!pCountry || !pRgnCountry)
									return;
								if(pCountry->GetCampID(GetCountry())==pRgnCountry->GetCampID(pRegion->GetCountry()))
								{
									//ͬһ��Ӫ
									LONG lRgnId=pRegion->GetID();
									LONG lPlayerPosX=(LONG)GetPosX();
									LONG lPlayerPosY=(LONG)GetPosY();
									pGoods->ChangeAttribute(GAP_RECORD_REGIONID,lRgnId,1);
									pGoods->ChangeAttribute(GAP_RECORD_COOR,lPlayerPosX,1);
									pGoods->ChangeAttribute(GAP_RECORD_COOR,lPlayerPosY,2);
									pGoods->UpdateAttribute(GetExID());
								}
								else
								{
									const char* strMsg=AppFrame::GetText("47");
									if(strMsg)
									{
										//SendNotifyMessage(strMsg,0xffffffff,0,eNOTIFYPOS_CENTER);
										//SendNotifyMessage(strMsg,0xffffffff,0,eNOTIFYPOS_SYSTEM);
										SendNotifyMessage(strMsg,eNOTIFYPOS_CENTER);
										SendNotifyMessage(strMsg,eNOTIFYPOS_SYSTEM);
									}
								}
							}
							else
							{
								const char* strMsg=AppFrame::GetText("44");
								if(strMsg)
								{
									SendNotifyMessage(strMsg,eNOTIFYPOS_CENTER);
									SendNotifyMessage(strMsg,eNOTIFYPOS_SYSTEM);
								}
							}

							bReduce=FALSE;
						}
					}
					else if(lRegionId && lXPos>=0 && lYPos>=0)
					{
						//����ServerRegion  GUID
						CGUID rgnGUID = NULL_GUID;
						CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, lRegionId);
						if(pRgn)
						{
							if(pRegion->GetSetup().bRuneStoneIsUsed)
							{
								//�ж���Ӫ
								CCountry *pCountry = GetCountryHandler()->GetCountry(GetCountry());
								CCountry* pRgnCountry=GetCountryHandler()->GetCountry(pRegion->GetCountry());
								if(!pCountry || !pRgnCountry)
									return;
								if(pCountry->GetCampID(GetCountry())==pRgnCountry->GetCampID(pRegion->GetCountry()))
								{
									//ͬһ��Ӫ
									rgnGUID = pRgn->GetExID();						
									ChangeRegion(RGN_NORMAL, rgnGUID, lXPos, lYPos, GetDir());
								}
								else
								{
									const char* strMsg=AppFrame::GetText("47");
									if(strMsg)
									{
										SendNotifyMessage(strMsg,eNOTIFYPOS_CENTER);
										SendNotifyMessage(strMsg,eNOTIFYPOS_SYSTEM);
									}
									bReduce=FALSE;
								}
							}
							else
							{
								const char* strMsg=AppFrame::GetText("45");
								if(strMsg)
								{
									SendNotifyMessage(strMsg,eNOTIFYPOS_CENTER);
									SendNotifyMessage(strMsg,eNOTIFYPOS_SYSTEM);
								}
								bReduce=FALSE;
							}
						}
					}
				}

				if( bItemUsed )
				{
					//��ȴʱ��
					BOOL bAddSkill=pGoods->HasAddonProperty(GAP_ADD_SKILL);
					if(!bAddSkill)
					{
						DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
						UpdateCooldown(dwGoodsId);

						CMessage msg(MSG_S2C_PLAYER_USEITEM);
						msg.Add((char)PLAYER_USEITEM_SUCCEED);
						msg.Add(GetExID());
						msg.Add(pGoods->GetBasePropertiesIndex());
						msg.SendToAround( this );
					}


					if(pGoods -> GetAddonPropertyValues(GAP_UNLIMITED_ACCESS, 1) ==0 && !bAddSkill && bReduce)
					{


						pGoods -> SetAmount( pGoods -> GetAmount() - 1 );

						//Goods Consume Log
						GetLogicLogInterface()->logT022_goods_log_consume(this,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),
							GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),pGoods->GetAmount(),1,wPlace,lPos);

						if( pGoods -> GetAmount() == 0 )
						{
							//���ĵ����������� ɾ��							
							if(pVolumeContainer)
							{							
								CS2CContainerObjectMove comMsg;
								if(pVolumeContainer->Remove( pGoods, &comMsg ) )
								{
									comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
									comMsg.SetSourceContainerExtendID( wPlace);
									comMsg.Send( GetExID() );
#ifdef _GOODSLOG1_
									char pszGoodsLog[1024]="";			
									char srcGoodsGUID[50]="";
									pGoods->GetExID().tostring(srcGoodsGUID);					
									_snprintf(pszGoodsLog,1024,"[��Ʒ����:UseItem��Ʒ����][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
										pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
									CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
									CGoodsFactory::GarbageCollect( &pGoods,28 );
								}
								else
								{
									OutputDebugString("Some error occur, goods can't remove from m_cPacket.\n");
								}
							}
						}
						else
						{
							CS2CContainerObjectAmountChange coacMsg;
							coacMsg.SetSourceContainer( GetType(), GetExID(), lPos );
							coacMsg.SetSourceContainerExtendID(wPlace);
							coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
							coacMsg.SetObjectAmount( pGoods -> GetAmount() );
							coacMsg.Send( GetExID() );
						}
					}
				}
				else
				{
					//CMessage msg(MSG_S2C_PLAYER_USEITEM);
					//msg.Add((char)PLAYER_USEITEM_FAILED);
					//msg.Add((char)REGION_FORBID);
					//msg.SendToPlayer(GetExID());
				}

				if( bBackToCity )
				{
					//##��ʹ�ûس̾������npc���͵�ʱ�������Ʒ
					DropParticularGoodsWhenRecall();
					if (!ChangeRgnToVillage())
					{
						((CServerRegion*)GetFather())->BackToCity(this);
					}					
				}
			}
			else
			{
				if ( eRet == REGION_FORBID )
				{
					const char* strMsg=AppFrame::GetText("83");
					if(strMsg)
						SendNotifyMessage( strMsg, eNOTIFYPOS_CENTER );
				}

				CMessage msg(MSG_S2C_PLAYER_USEITEM);
				msg.Add((char)PLAYER_USEITEM_FAILED);
				msg.Add((char)eRet);
				msg.SendToPlayer(GetExID());
			}
		}
	}
}

void CPlayer::UseItem(DWORD dwContainerId,DWORD dwPos)
{
	CGoods* pGoods=FindGoods(dwContainerId,dwPos);
	if(!pGoods)
		return;
	if(pGoods -> GetAddonPropertyValues(GAP_UNLIMITED_ACCESS, 1) !=0)
		return;
	pGoods -> SetAmount( pGoods -> GetAmount() - 1 );

	//Goods Consume Log
	GetLogicLogInterface()->logT022_goods_log_consume(this,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),
		GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),pGoods->GetAmount(),1,dwContainerId,dwPos);

	if( pGoods -> GetAmount() == 0 )
	{
		//���ĵ����������� ɾ��
		CVolumeLimitGoodsContainer* pVolContainer=NULL;
		pVolContainer=this->GetPackContainerById(dwContainerId);
		if(pVolContainer)
		{							
			CS2CContainerObjectMove comMsg;
			if(pVolContainer->Remove( pGoods, &comMsg ) )
			{
				comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
				comMsg.SetSourceContainerExtendID( dwContainerId);
				comMsg.Send( GetExID() );
#ifdef _GOODSLOG1_
				char pszGoodsLog[1024]="";			
				char srcGoodsGUID[50]="";
				pGoods->GetExID().tostring(srcGoodsGUID);					
				_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::UseItem(DWORD dwContainerId,DWORD dwPos)][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
					pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
				CGoodsFactory::GarbageCollect( &pGoods ,29);
			}
			else
			{
				OutputDebugString("Some error occur, goods can't remove from m_cPacket.\n");
			}
		}
	}
	else
	{
		CS2CContainerObjectAmountChange coacMsg;
		coacMsg.SetSourceContainer( GetType(), GetExID(), dwPos );
		coacMsg.SetSourceContainerExtendID(dwContainerId);
		coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
		coacMsg.SetObjectAmount( pGoods -> GetAmount() );
		coacMsg.Send( GetExID() );
	}	
}

BOOL CPlayer::AutoUseItem(WORD wPlace,BYTE lPos,long lTargetType,CGUID& lTargetGuid)
{
	BOOL bReturnValue = FALSE;

	//�в���ʹ��װ����״̬
	if (!GetUseItem() || IsDied())
	{
		return bReturnValue;
	}
	CGoods* pGoods=NULL;
	CS2CContainerObjectMove::PLAYER_EXTEND_ID place=static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(wPlace);
	CVolumeLimitGoodsContainer* pVolumeContainer=NULL;
	if(place==CS2CContainerObjectMove::PEI_PACKET)
	{
		pGoods=getOriginPack()->GetGoods(lPos);
		pVolumeContainer=getOriginPack();
		if(NULL != pGoods)
			if(pVolumeContainer->IsLocked(pGoods))
				return FALSE;
	}
	else if(place>=CS2CContainerObjectMove::PEI_PACK1 && place<=CS2CContainerObjectMove::PEI_PACK5)
	{
		pVolumeContainer=m_pPackage.GetSubpack(place-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
		pGoods=pVolumeContainer->GetGoods(lPos);
		if(NULL != pGoods)
			if(pVolumeContainer->IsLocked(pGoods))
				return FALSE;
	}

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pGoods && pRegion )
	{
		if(lTargetGuid != pGoods->GetExID())
		{
			return FALSE;
		}

		if(IsInPersonalShop(pGoods))
		{
			return FALSE;
		}

		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(
			pGoods -> GetBasePropertiesIndex() );
		if( pProperties && pProperties -> GetGoodsBaseType() == GBT_CONSUMABLE )
		{
			BOOL bCanUse = TRUE;

			//�жϸó����Ƿ�֧��ʹ�ø���Ʒ
			EquipRestrict eRet = EQUIPFIT;
			if( pRegion->FindForbidGood(pProperties->GetOriginalName()) == true)
			{
				eRet = REGION_FORBID;
			}

			if( eRet==EQUIPFIT )
			{
				eRet = (EquipRestrict)CanUseItem( pGoods );
			}

			if(eRet!=EQUIPFIT)
			{
				bCanUse = FALSE;
			}

			if( bCanUse && pGoods -> GetAmount() > 0 )
			{
				BOOL bItemUsed		= TRUE;
				BOOL bBackToCity	= FALSE;
				vector<eGoodsAddonProperties> vOut;
				pGoods -> GetEnabledAddonProperties( vOut );

				//�����ʹ���������Ʒ,�򲻽���ѭ��
				if( pGoods -> GetAddonPropertyValues(GAP_MOUNT_TYPE, 1) )
				{				
						//##��̯��ս��״̬�²���������
						if( GetCurrentProgress() != PROGRESS_OPEN_STALL && m_lFightStateCount == 0 )
						{
							const char *strGoodOriName = GoodsSetup::QueryGoodsOriginalName(pGoods->GetBasePropertiesIndex());
							if( Mount( pGoods -> GetAddonPropertyValues(GAP_MOUNT_TYPE, 1), 
								pGoods -> GetAddonPropertyValues(GAP_MOUNT_LEVEL, 1), 
								pGoods -> GetAddonPropertyValues(GAP_MOUNT_ROLE_LIMIT, 1),
								strGoodOriName) )
							{
								//##�����������ʹ��,�򲻼�������
								if( pGoods -> GetAddonPropertyValues(GAP_UNLIMITED_ACCESS, 1) == 1 )
								{
									bItemUsed = FALSE;
								}
							}
							else
							{
								bItemUsed = FALSE;
							}
						}
						else
						{
							bItemUsed = FALSE;
						}
					
				}
				else
				{
					//! pGoods -> GetEnabledAddonProperties( vOut );
				}

				for( size_t i = 0; i < vOut.size(); i++ )
				{
					switch( vOut[i] )
					{
					case GAP_ADD_HP:
						{
							if( pGoods -> GetAddonPropertyValues(vOut[i], 2) == 0 )
							{
								bItemUsed = RestoreHp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 0, 400 );

							}
							else
							{
								bItemUsed = RestoreHp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 
									pGoods -> GetAddonPropertyValues(vOut[i], 2) , 400 );
							}
						}
						break;
					case GAP_ADD_PERCENT_HP:	//�����ٷֱ�����
						{
							bItemUsed = RestoreHp( static_cast<DWORD>( GetMaxHP() * ( pGoods -> GetAddonPropertyValues(vOut[i], 1) / 100.0f) ),
								0, 400 );
						}
						break;
					case GAP_ADD_MP:	//��������
						{
							if(pGoods -> GetAddonPropertyValues( vOut[i], 2 )==0)
							{
								bItemUsed = RestoreMp( pGoods -> GetAddonPropertyValues(vOut[i], 1), 0, 400 );
							}
							else
							{
								bItemUsed = RestoreMp( pGoods -> GetAddonPropertyValues(vOut[i], 1), (pGoods -> GetAddonPropertyValues(vOut[i], 2) - 1) * 400, 400 );
							}
						}
						break;
					case GAP_ADD_PERCENT_MP:	//�����ٷֱ�����
						{
							bItemUsed = RestoreMp( static_cast<DWORD>( GetMaxMP() * (pGoods -> GetAddonPropertyValues(vOut[i], 1) / 100.0f) ),
								0, 400 );
						}
						break;
					case GAP_HOME:	//�س�
						{
							// ��������������ʹ�ûس�
							if( bCanUse )
							{
								if( pRegion->GetCountry() && pRegion->GetCountry() != GetCountry() )
								{
									bItemUsed = FALSE;
									const char* strMsg=AppFrame::GetText("41");
									if(strMsg)
										SendSystemInfo(strMsg,0xFFFFFFFF);
								}
								else
								{
									bBackToCity = TRUE;
								}
							}
							else
								bItemUsed = FALSE;

						}
						break;
					case GAP_RAN_TRANS:	//�������
						{
							// ֻ���ڱ�����ʹ���������
							if( bCanUse )
							{
								if( pRegion->GetCountry() != GetCountry() )
								{
									bItemUsed = FALSE;
									const char* strMsg=AppFrame::GetText("43");
									if(strMsg)
										SendSystemInfo(strMsg,0xFFFFFFFF);
								}
								else
								{
									CServerRegion* pRegion=((CServerRegion*)GetFather());
									if(pRegion)
									{
										//##��ʹ�ûس̾������npc���͵�ʱ�������Ʒ
										DropParticularGoodsWhenRecall();

										long lX = -1, lY = -1;
										pRegion->GetRandomPos(lX, lY);

										// [071127 AHC REGION]
										ChangeRegion(GetCurRgnType(), GetRegionGUID(),lX, lY,GetDir());
									}
								}
							}
							else 
								bItemUsed = FALSE;
						}
						break;
					case GAP_SCRIPT:	//ִ�нű�
						{
							char strFile[50];
							_snprintf(strFile, 50, "scripts/Goods/%d.lua",pGoods -> GetAddonPropertyValues(vOut[i], 1));
							if( !m_VarList->HasVar( "#m_strUseItem" ) )
							{
								m_VarList->AddVar( "#m_strUseItem", tString("") );
							}
							m_VarList->SetVarValue( "#m_strUseItem", tString( pProperties->GetOriginalName() ) );

							Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ), m_VarList );
							GetInst( ScriptSys ).RunScript( env, strFile );
						}
						break;
					case GAP_ADD_STATE:
						{
							DWORD dwStateId=pGoods -> GetAddonPropertyValues(vOut[i],1);
							DWORD dwLevel=pGoods -> GetAddonPropertyValues(vOut[i],2);	
							AddNewState(this,dwStateId,dwLevel,17);
						}
						break;
					case GAP_POWER_INTENSITY:
						{
							CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON);
							bItemUsed = GoodsExManage::GetInstance().ReinforcePower(pArm, pGoods, this);				
						}
						break;						
					}
				}

				if( bItemUsed )
				{
					//��ȴʱ��
					DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
					UpdateCooldown(dwGoodsId);

					////////////////////////////////

					CMessage msg(MSG_S2C_PLAYER_USEITEM);
					msg.Add((char)PLAYER_USEITEM_SUCCEED);
					msg.Add(GetExID());
					msg.Add(pGoods->GetBasePropertiesIndex());
					msg.SendToAround( this );
					BOOL bAddSkill=pGoods->HasAddonProperty(GAP_ADD_SKILL);
					if(pGoods -> GetAddonPropertyValues(GAP_UNLIMITED_ACCESS, 1) ==0 && !bAddSkill)
					{


						pGoods -> SetAmount( pGoods -> GetAmount() - 1 );

						if( pGoods -> GetAmount() == 0 )
						{
							//���ĵ����������� ɾ��							
							if(pVolumeContainer)
							{							
								CS2CContainerObjectMove comMsg;
								if(pVolumeContainer->Remove( pGoods, &comMsg ) )
								{
									comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
									comMsg.SetSourceContainerExtendID( wPlace);
									comMsg.Send( GetExID() );
#ifdef _GOODSLOG1_
									char pszGoodsLog[1024]="";			
									char srcGoodsGUID[50]="";
									pGoods->GetExID().tostring(srcGoodsGUID);					
									_snprintf(pszGoodsLog,1024,"[��Ʒ����:AutoUseItem][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
										pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
									CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif
									CGoodsFactory::GarbageCollect( &pGoods,28 );
								}
								else
								{
									OutputDebugString("Some error occur, goods can't remove from m_cPacket.\n");
								}
							}
						}
						else
						{
							CS2CContainerObjectAmountChange coacMsg;
							coacMsg.SetSourceContainer( GetType(), GetExID(), lPos );
							coacMsg.SetSourceContainerExtendID(wPlace);
							coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
							coacMsg.SetObjectAmount( pGoods -> GetAmount() );
							coacMsg.Send( GetExID() );
						}
					}

					bReturnValue = TRUE;
				}
				else
				{
					//CMessage msg(MSG_S2C_PLAYER_USEITEM);
					//msg.Add((char)PLAYER_USEITEM_FAILED);
					//msg.Add((char)REGION_FORBID);
					//msg.SendToPlayer(GetExID());
				}

				if( bBackToCity )
				{
					//##��ʹ�ûس̾������npc���͵�ʱ�������Ʒ
					DropParticularGoodsWhenRecall();

					((CServerRegion*)GetFather())->BackToCity(this);
				}
			}
			else
			{
				if ( eRet == REGION_FORBID )
				{
					const char* strMsg=AppFrame::GetText("83");
					if(strMsg)
						SendNotifyMessage( strMsg, eNOTIFYPOS_CENTER );
				}

				CMessage msg(MSG_S2C_PLAYER_USEITEM);
				msg.Add((char)PLAYER_USEITEM_FAILED);
				msg.Add((char)eRet);
				msg.SendToPlayer(GetExID());
			}
		}
	}

	return bReturnValue;
}

BOOL CPlayer::Mount( DWORD dwMountType, DWORD dwMountLevel, DWORD dwRoleLimit,const char* strGoodName  )
{
	if(strGoodName == NULL)
		return FALSE;
	BOOL bResult = TRUE;

	if( m_lFightStateCount == 0 )
	{
		if( dwRoleLimit )
		{
			//##�ж���ҵĽ�ɫ
			if( dwRoleLimit | PR_GUILD_MASTER )
			{
				//bResult = FALSE;
			}

			if( dwRoleLimit | PR_UNION_MASTER )
			{
				//bResult = FALSE;
			}

			if( dwRoleLimit | PR_CITY_MASTER )
			{
				//bResult = FALSE;
			}
		}

		if( bResult )
		{
			//  [6/15/2007 chenxianjin]
			//CNewState *pState = new CNewState(STATE_RIDE_HORSE);
			/*CRideState* pState = new CRideState( dwMountType, dwMountLevel, dwRoleLimit );
			pState->SetGoodName(strGoodName);
			if( pState && pState -> Begin( this, this ) )
			{
			m_vStates.push_back( pState );
			}
			else
			{
			SAFE_DELETE( pState );
			bResult = FALSE;
			}*/
		}
	}
	else 
	{
		const char* strRes=AppFrame::GetText("10");
		SendNotifyMessage( strRes);
	}

	return bResult;
}

//long CPlayer::GetCurBurden()
//{
//	long lWeight=0;
//	//lWeight += m_cEquipment.GetContentsWeight();
//	//lWeight += m_cPacket.GetContentsWeight();
//	//lWeight += m_cHand.GetContentsWeight();
//	return lWeight;
//}
//
//bool CPlayer::CheckBurden(CGoods* pGoods)
//{
//	/*if( pGoods -> GetWeight() + GetCurBurden() > GetBurden() )
//	{
//	return false;
//	}*/
//	return true;
//}

bool CPlayer::CanMountSubpack(CGoods* pGoods)
{
	if(!pGoods)
		return false;
	DWORD dwLevLimit=pGoods->GetAddonPropertyValues(GAP_ROLE_LEVEL_LIMIT,1);
	if(dwLevLimit>0 && GetLevel()<dwLevLimit)
	{
		//�ȼ�����
		const char* pStrMsg = AppFrame::GetText("11");
		if(pStrMsg)
			SendNotifyMessage(pStrMsg);		
		return false;
	}
	return true;
}

long CPlayer::CanMountEquip(CGoods *pGoods)
{	
	if( pGoods )
	{
        if( pGoods->HasAddonProperty(GAP_GOODS_MAX_DURABILITY) )
        {
           LONG lCurrentDurability	= pGoods -> GetAddonPropertyValues( GAP_GOODS_MAX_DURABILITY, 2 );
           if ( lCurrentDurability <= 0 )
           {
                /// װ���;öȲ��㣬�޷�װ��
               return  UNKNOW_NOTFIT;
           }
        }
        
		CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(pGoods->GetBasePropertiesIndex());
		if(!pProperty)
			return UNKNOW_NOTFIT;
		if(pProperty->GetGoodsBaseType()!=GBT_EQUIPMENT)
			return UNKNOW_NOTFIT;
		//װ�����������ж�
		DWORD dwVal=0;
		//�ȼ�
		dwVal=pGoods -> GetAddonPropertyValues( GAP_ROLE_LEVEL_LIMIT, 1 );
		if(dwVal!=0 && dwVal>GetLevel())
			return LEVEL_NOTFIT;

		dwVal=pGoods->GetAddonPropertyValues(GAP_SKILL_LIMIT,1);
		if(dwVal!=0)
		{
			DWORD dwLev=GetSkillLv(dwVal);
			if(dwLev<=0)
				return SKILL_NOTFIT;
		}

		//������������
		dwVal=pGoods->GetAddonPropertyValues(GAP_GUARD_EQUIP_CATEGORY,1);
		if(dwVal!=0)
		{
			eEquipLimitType elt=CGoodsFactory::QueryPlayerEquipLimitType(this);
			if(elt!=dwVal)
				return EQUIP_TYPE_NOTFIT;
		}

		//ְҵ����
		if(pGoods->HasAddonProperty(GAP_REQUIRE_OCCUPATION))
		{
			dwVal=pGoods -> GetAddonPropertyValues( GAP_REQUIRE_OCCUPATION, 1 );
			//if(dwVal!=0 && dwVal!=GetOccupation())
			//return OCCUPATION_NOTFIT;
			BYTE nPlayerOccu=GetOccupation();
			DWORD dwOccuMask=1;
			dwOccuMask=dwOccuMask<<nPlayerOccu;
			if(!(dwOccuMask & dwVal))
			{
				return OCCUPATION_NOTFIT;
			}
		}


		if(pGoods->HasAddonProperty(GAP_REQUIRE_GENDER))
		{
			dwVal=pGoods -> GetAddonPropertyValues( GAP_REQUIRE_GENDER, 1 );
			dwVal--;
			if(dwVal!=GetSex())
				return SEX_NOTFIT;
		}
		return EQUIPFIT;
	}
	return UNKNOW_NOTFIT;
}


void    CPlayer::SendEquipNotifyMsg(long lRet)
{
	const char* pStrMsg=NULL;
	if(lRet==CPlayer::LEVEL_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("11");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//�ȼ�����
	}
	else if(lRet==CPlayer::STR_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("12");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//��������
	}
	else if(lRet==CPlayer::DEX_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("13");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//���ݲ���
	}
	else if(lRet==CPlayer::CON_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("14");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//���ʲ���
	}
	else if(lRet==CPlayer::INT_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("15");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//��������
	}
	else if(lRet==CPlayer::OCCUPATION_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("16");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//ְҵ����
	}
	else if(lRet==CPlayer::SEX_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("17");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//�Ա𲻶�
	}
	else if(lRet==CPlayer::SKILL_NOTFIT)
	{
		pStrMsg=AppFrame::GetText("18");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
		//���ܲ���
	}
	else if(lRet==CPlayer::REGION_FORBID)
	{
		//������ֹ
		pStrMsg=AppFrame::GetText("19");
		if(pStrMsg)
			/*SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);*/
			SendNotifyMessage(pStrMsg);
	}
	else if(lRet==CPlayer::COOLDOWN_NOTFIT)
	{
		//��ȴ����
		pStrMsg=AppFrame::GetText("21");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
	}
	else if(lRet==CPlayer::EQUIP_TYPE_NOTFIT)
	{
		//�������Ͳ���
		pStrMsg=AppFrame::GetText("22");
		if(pStrMsg)
			//SendNotifyMessage(pStrMsg,NOTIFY_COLOR_RED);
			SendNotifyMessage(pStrMsg);
	}
}

long CPlayer::CanMountEquip(CGoods *pGoods,DWORD dwPos,BOOL bMsgNotify)
{
	long lRet=CanMountEquip(pGoods);
	if(lRet==EQUIPFIT)
	{
		//����λ���м��
		eEquipType ecColumn = static_cast<eEquipType>( dwPos);
		DWORD dwEcVal = pGoods->GetEquipType();

		if(ecColumn==dwEcVal)
		{
			return EQUIPFIT;
		}
		else
		{
			//��λ����
			return UNKNOW_NOTFIT;
		}
	}
	else
	{			
		if(bMsgNotify)
		{
			SendEquipNotifyMsg(lRet);
		}
	}
	return lRet;
}

long CPlayer::CanUseItem(CGoods* pGoods)
{
	if( pGoods )
	{
		DWORD dwVal=0;
		dwVal= pGoods -> GetAddonPropertyValues( GAP_ROLE_LEVEL_LIMIT, 1 );
		if(dwVal!=0 && dwVal>GetLevel())
			return LEVEL_NOTFIT;

		dwVal= pGoods -> GetAddonPropertyValues( GAP_REQUIRE_OCCUPATION, 1 );
		if(dwVal!=0 && dwVal!=GetOccupation()+1)
			return OCCUPATION_NOTFIT;

		dwVal= pGoods -> GetAddonPropertyValues( GAP_REQUIRE_GENDER, 1 );
		if(dwVal!=0 && dwVal!=GetSex())
			return SEX_NOTFIT;

		//��ȴ���
		DWORD dwGoodsId=pGoods->GetBasePropertiesIndex();
		DWORD dwCoolDownTime=pGoods->GetAddonPropertyValues(GAP_COOLDOWN,1);
		DWORD dwCoolDownId=pGoods->GetAddonPropertyValues(GAP_COOLDOWN,2);
		if(dwCoolDownId!=0)
		{
			//�й�����ȴ����
			map<DWORD,tagGoodsCommonCooldown>::iterator iter=m_mapClassIdCoolDown.find(dwCoolDownId);
			if(iter!=m_mapClassIdCoolDown.end())
			{
				if(timeGetTime() - iter->second.dwCooldownStartTime<=iter->second.dwCooldownTime/*dwCoolDownTime*/)				
				{
					return COOLDOWN_NOTFIT;
				}
				else
				{
				}
			}
		}
		else if(dwCoolDownTime!=0)
		{
			//GOODSID��ȴ����
			map<DWORD,tagGoodsCommonCooldown>::iterator iter=m_mapGoodsIdCoolDown.find(dwGoodsId);
			if(iter!=m_mapGoodsIdCoolDown.end())
			{
				if(timeGetTime()-iter->second.dwCooldownStartTime<=iter->second.dwCooldownTime)
				{
					return COOLDOWN_NOTFIT;
				}
			}
		}


		return EQUIPFIT;
	}
	return UNKNOW_NOTFIT;
}

//����HPMP�ָ��ٶ�
void CPlayer::UpdateHpMpRecoverSpeed()
{
	string strBHpSpeed = "B_HpRecoverSpeed";
	string strBMpSpeed = "B_MpRecoverSpeed";
	if (GetState()==STATE_FIGHT)
	{
		SetAttribute(strBHpSpeed,0);
		SetAttribute(strBMpSpeed,0);
	}

}
//�ı�״̬
void CPlayer::ChangeStateTo(CShape::eSTATE state)
{
	if (state==STATE_FIGHT || state==STATE_PEACE)
	{
		string strConsti = "C_Consti";
		string strSpirit = "C_Spirit";
		string strOccu	 = "Occu";
		string strBHpSpeed = "B_HpRecoverSpeed";
		string strBMpSpeed = "B_MpRecoverSpeed";
		string strLv       = "Level";

		long lTotalConsti = GetAttribute(strConsti);
		long lTotalSpiri = GetAttribute(strSpirit);
		long lLv		 = GetAttribute(strLv);

		float fTemp = pow((float)(1.0/lLv),(float)0.1);
		//Hp�ָ��ٶ�ֵ
		double dbHpRecoverSpeed =  (double)((((lTotalConsti*(1-lLv/220) + lTotalConsti)*(3-lLv/200))/5)*2)*fTemp;//GetAttribute(strBHpSpeed);//
		//Mp�ָ��ٶ�ֵ
		double dbMpRecoverSpeed =  (double)((((lTotalSpiri*(1-lLv/220) + lTotalSpiri)*(3-lLv/200))/5)*2)*fTemp;//GetAttribute(strBMpSpeed); //

		if (state==STATE_FIGHT)
		{
			dbHpRecoverSpeed = 0;//dbHpRecoverSpeed/5;
			dbMpRecoverSpeed = 0;//dbMpRecoverSpeed/5;
			SetAttribute(strBHpSpeed, (long)dbHpRecoverSpeed);
			SetAttribute(strBMpSpeed, (long)dbMpRecoverSpeed);
		}
		else if (state==STATE_PEACE)
		{			
			SetAttribute(strBHpSpeed, (long)dbHpRecoverSpeed);
			SetAttribute(strBMpSpeed, (long)dbMpRecoverSpeed);

		}
		
		UpdateAttribute(1);
	}


	CMoveShape::ChangeStateTo(state);

	//if (state==STATE_FIGHT || state==STATE_PEACE)
	//{
	//	UpdateEnergyTimer();
	//}
	
	ChangeActivePetState(state);
	
}
//////////////////////////////////////////////////////////////////////////
//	�������͵�ַ��ӳ���
//////////////////////////////////////////////////////////////////////////

// ��ʼ��
void CPlayer::InitNameValueMap()
{
	CMoveShape::InitNameValueMap();

	m_mapNameValue.clear();

	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	char tmp[128];
	st.SetType(4);  st.pValue   = &m_lType;				st.attrEnumValue=P_ATTR_TYPE;       m_mapNameValue["Type"]=st;			
	st.SetType(4);  st.pValue   = &m_lGraphicsID;		st.attrEnumValue=P_ATTR_GRAPHICSID;	m_mapNameValue["GraphicsID"]=st;	
	st.SetType(4);  st.pValue   = &m_lRegionID;			st.attrEnumValue=P_ATTR_REGIONID;	m_mapNameValue["RegionID"]=st;
	st.SetType(3);	st.pfValue  = &m_fPosX;			    st.attrEnumValue=P_ATTR_TILEX;		m_mapNameValue["TileX"]=st;		
	st.SetType(3);	st.pfValue  = &m_fPosY;			    st.attrEnumValue=P_ATTR_TILEY;		m_mapNameValue["TileY"]=st;
	st.SetType(4);	st.pfValue  = &m_fDir;			    st.attrEnumValue=P_ATTR_DIR;		m_mapNameValue["Dir"]=st;			
	st.SetType(4);	st.pValue   = &m_lPos;				st.attrEnumValue=P_ATTR_POS;		m_mapNameValue["Pos"]=st;			
	st.SetType(1);	st.pwValue  = &m_wState;			st.attrEnumValue=P_ATTR_STATE;		m_mapNameValue["State"]=st;			
	st.SetType(1);	st.pwValue  = &m_wAction;			st.attrEnumValue=P_ATTR_ACTION;		m_mapNameValue["Action"]=st;		


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*��������*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	st.SetType(0);	st.plValue  = (BYTE*)(&m_Property.byOccu);				st.attrEnumValue=P_ATTR_OCCU;				m_mapNameValue["Occu"]=st;		
	st.SetType(0);	st.plValue  = (BYTE*)(&m_Property.byAssOccu);			st.attrEnumValue=P_ATTR_DOCCU;				m_mapNameValue["DOccu"]=st;
	st.SetType(0);	st.plValue	= &m_Property.bySex;						st.attrEnumValue=P_ATTR_SEX;				m_mapNameValue["Sex"]=st;					
	st.SetType(0);	st.plValue	= &m_Property.byHairPic;					st.attrEnumValue=P_ATTR_HEADPIC;			m_mapNameValue["HeadPic"]=st;				
	st.SetType(0);	st.plValue	= &m_Property.byFacePic;					st.attrEnumValue=P_ATTR_FACEPIC;			m_mapNameValue["FacePic"]=st;				
	st.SetType(1);	st.pdwValue	= &m_Property.dwHp;							st.attrEnumValue=P_ATTR_HP;					m_mapNameValue["Hp"]=st;				
	st.SetType(1);	st.pwValue	= &m_Property.wMp;							st.attrEnumValue=P_ATTR_MP;					m_mapNameValue["Mp"]=st;				
	st.SetType(1);	st.pwValue  = &m_Property.wEnergy;						st.attrEnumValue=P_ATTR_ENERGY;				m_mapNameValue["Energy"]=st;			
	st.SetType(1);  st.pwValue  = &m_Property.wStamina;						st.attrEnumValue=P_ATTR_STAMINA;			m_mapNameValue["Stamina"]=st;				
	st.SetType(1);	st.pwValue	= &m_Property.wLevel;						st.attrEnumValue=P_ATTR_LEVEL;				m_mapNameValue["Level"]=st;				
	st.SetType(2);	st.pdwValue	= &m_Property.dwExp;						st.attrEnumValue=P_ATTR_EXP;				m_mapNameValue["Exp"]=st;
	st.SetType(2);	st.pdwValue	= &m_Property.dwExpMultiple;				st.attrEnumValue=P_ATTR_EXPMULTIPLE;		m_mapNameValue["ExpMultiple"]=st;
	st.SetType(2);	st.pdwValue	= &m_Property.dwPresentExp;					st.attrEnumValue=P_ATTR_PRESENTEXP;			m_mapNameValue["PresentExp"]=st;
    st.SetType(4);  st.pValue   = &m_Buddhism.lDistract;                    st.attrEnumValue=P_ATTR_BUDDHISM;           m_mapNameValue["lDistract"]=st;
    st.SetType(4);  st.pValue   = &m_Buddhism.lDryNum;                      st.attrEnumValue=P_ATTR_DRYNUM;             m_mapNameValue["lDryNum"]=st;
    st.SetType(4);  st.pValue   = &m_Buddhism.lDryEndTime;                  st.attrEnumValue=P_ATTR_DRYENDTIME;         m_mapNameValue["lDryEndTime"]=st;

	//ְҵ�ȼ�����
	for(int i=0;i<OCC_Max;i++)
	{
		_snprintf(tmp, 128, "OccuLvl%d", i);
		st.SetType(0);	st.plValue	= (BYTE*)(&m_Property.byOccuLvl[i]);	st.attrEnumValue=P_ATTRIBUTE_TYPE(P_ATTR_OCCULVL0+i);	m_mapNameValue[tmp]=st;
	}
	//ְҵ��������
	for(int i=0;i<OCC_Max;i++)
	{
		_snprintf(tmp, 128, "OccuExp%d", i);
		st.SetType(2);	st.pdwValue	= &m_Property.dwOccuExp[i];				st.attrEnumValue=P_ATTRIBUTE_TYPE(P_ATTR_OCCUEXP0+i);	m_mapNameValue[tmp]=st;
	}
	st.SetType(2);	st.pdwValue	= &m_Property.dwCurOccuUpgradeExp;			st.attrEnumValue=P_ATTR_CUROCCUUPGRADEEXP;	m_mapNameValue["OccuUpgradeExp"]=st;

    st.SetType(1); st.pwValue  = &m_Property.wTaoismLvl;                     st.attrEnumValue=P_ATTR_TAOISMLVL;          m_mapNameValue["TaoismLvl"]=st;
    st.SetType(2); st.pdwValue = &m_Property.dwTaoismExp;                    st.attrEnumValue=P_ATTR_TAOISMEXP;          m_mapNameValue["TaoismExp"]=st;
    st.SetType(2); st.pdwValue = &m_Property.dwRelationVal;                  st.attrEnumValue=P_ATTR_RELATIONVAL;        m_mapNameValue["RelationVal"]=st;
    st.SetType(2); st.pdwValue = &m_Property.dwHiddenVirtue;                 st.attrEnumValue=P_ATTR_HIDDENVIRTUE;       m_mapNameValue["HiddenVirtue"]=st;
    st.SetType(1); st.pwValue  = &m_Property.wVigour;                        st.attrEnumValue=P_ATTR_VIGOUR;             m_mapNameValue["Vigour"]=st;
    st.SetType(1); st.pwValue  = &m_Property.wThew;                          st.attrEnumValue=P_ATTR_THEW;               m_mapNameValue["Thew"]=st;

	st.SetType(0);	st.plValue  = &m_Property.byCountry;					st.attrEnumValue=P_ATTR_COUNTRY;			m_mapNameValue["Country"]=st;				
	st.SetType(4);	st.pValue   = &m_Property.lCountryContribute;				st.attrEnumValue=P_ATTR_COUNTRYCONTRIBUTE;	m_mapNameValue["CountryContribute"]=st;	
	st.SetType(2);	st.pdwValue = &m_Property.dwSpouseID;					st.attrEnumValue=P_ATTR_SPOUSEID;			m_mapNameValue["SpouseId"]=st;			
	st.SetType(4);	st.pdwValue = &m_Property.dwSpouseParam;				st.attrEnumValue=P_ATTR_SPOUSEPARAM;		m_mapNameValue["SpouseParam"]=st;		
	st.SetType(0); st.plValue   = &m_Property.byBusinessLevel;				st.attrEnumValue=P_ATTR_BUSINESSLEVEL;		m_mapNameValue["BusinessLevel"]=st;		
	st.SetType(2); st.pdwValue  = &m_Property.dwBusinessExp;				st.attrEnumValue=P_ATTR_BUSINESSEXP;		m_mapNameValue["BusinessExp"]=st;
	st.SetType(2);	st.pdwValue	= &m_Property.dwPkValue;					st.attrEnumValue=P_ATTR_PKVALUE;			m_mapNameValue["PkValue"]=st;				
	st.SetType(2);	st.pdwValue	= &m_Property.dwPkCount;					st.attrEnumValue=P_ATTR_PKCOUNT;			m_mapNameValue["PkCount"]=st;			
	st.SetType(2);	st.pdwValue	= &m_Property.dwPVPCount;					st.attrEnumValue=P_ATTR_PVPCOUNT;			m_mapNameValue["PVPCount"]=st;			
	st.SetType(2);	st.pdwValue	= &m_Property.dwPVPValue;					st.attrEnumValue=P_ATTR_PVPVALUE;			m_mapNameValue["PVPValue"]=st;			
	st.SetType(0);	st.plValue  = &m_Property.byPKOnOff;  					st.attrEnumValue=P_ATTR_PKONOFF;			m_mapNameValue["PkOnOff"]=st;			
	st.SetType(4);	st.pValue   = &m_DOccuRecord.m_lFactureExp ;			st.attrEnumValue=P_ATTR_FACTUREEXP;			m_mapNameValue["dwFactureExp"]=st;						//	��������
	st.SetType(4);	st.pValue   = &m_DOccuRecord.m_lCollectionExp;			st.attrEnumValue=P_ATTR_COLLECTIONEXP;		m_mapNameValue["dwCollectionExp"]=st;					//	�ɼ�����
	st.SetType(4);	st.pValue   = &m_DOccuRecord.m_lFactureGrade ;			st.attrEnumValue=P_ATTR_FACTUREGRADE;		m_mapNameValue["dwFactureGrade"]=st;					//	�����ȼ�
	st.SetType(4);	st.pValue   = &m_DOccuRecord.m_lCollectionGrade;		st.attrEnumValue=P_ATTR_COLLECTIONGRADE;	m_mapNameValue["dwCollectionGrade"]=st;				//	�ɼ��ȼ�
	//ְҵSP����
	for(int i=0;i<OCC_Max;i++)
	{
		_snprintf(tmp, 128, "OccuSP%d", i);
		st.SetType(2);	st.pdwValue	= &m_Property.dwOccuSP[i];				st.attrEnumValue=P_ATTRIBUTE_TYPE(P_ATTR_OCCUSP0+i);	m_mapNameValue[tmp]=st;
	}
	st.SetType(2); st.pdwValue = &m_Property.dwUpgradeExp;					st.attrEnumValue=P_ATTR_UPGRADEEXP;			m_mapNameValue["UpgradeExp"]=st;		
	st.SetType(6); st.pdwValue = (DWORD*)&m_Property.bShowCountry;					st.attrEnumValue=P_ATTR_SHOWCOUNTRY;		m_mapNameValue["ShowCountry"]=st;
	//	st.SetType(0); st.plValue  = &m_Property.byVisibility;				m_mapNameValue["byVisibility"]=st;				m_mapAttrType["byVisibility"]=P_ATTR_VISIBILITY;			
	for(int i=0; i<24; ++i)
	{
		_snprintf(tmp, 128, "dwHotKey%d", i);
		st.SetType(2);	st.pdwValue	= &m_Property.dwHotKey[i];		st.attrEnumValue=P_ATTRIBUTE_TYPE(P_ATTR_HOTKEY0+i);	m_mapNameValue[tmp]=st;			

	}

	st.SetType(6);	st.pbValue  = &m_bGMInvincible;		m_mapNameValue["GMInvincible"]=st;;	//GM�޵�
	st.SetType(6);	st.pbValue  = &m_bGMConcealment;	st.attrEnumValue=P_ATTR_CONCEALMENT;	m_mapNameValue["GMConcealment"]=st;	//GM����

	st.lType = 4; st.pValue = &m_Property.lCurPKCP;     st.attrEnumValue = P_ATTR_CURPKCP; m_mapNameValue["CurPKCP"] = st;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*ս������*/ // B_:����ֵ, C_:��ǰֵ, E_:�仯ֵ
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////							

	st.SetType(4);	st.pValue	= &m_FightProperty.dwMaxHp;					st.attrEnumValue=P_ATTR_MAXHP;			   	m_mapNameValue["E_MaxHp"]=st;			
	st.SetType(5);	st.psValue	= &m_FightProperty.wMaxMp;					st.attrEnumValue=P_ATTR_MAXMP;				m_mapNameValue["E_MaxMp"]=st;			
	st.SetType(5);	st.psValue	= &m_FightProperty.wMaxStamina;				st.attrEnumValue=P_ATTR_MAXSTAMINA;			m_mapNameValue["E_MaxStamina"]=st;		
	st.SetType(5);	st.psValue	= &m_FightProperty.wMaxEnergy;				st.attrEnumValue=P_ATTR_MAXENERGY;			m_mapNameValue["E_MaxEnergy"]=st;		
	st.SetType(5);	st.psValue	= &m_FightProperty.wHpRecoverSpeed;			st.attrEnumValue=P_ATTR_HPRECOVERSPEED;		m_mapNameValue["E_HpRecoverSpeed"]=st;	
	st.SetType(5);	st.psValue	= &m_FightProperty.wMpRecoverSpeed;			st.attrEnumValue=P_ATTR_MPRECOVERSPEED;		m_mapNameValue["E_MpRecoverSpeed"]=st;	
    st.SetType(5);	st.psValue	= &m_FightProperty.wHpRecoverRatio;			st.attrEnumValue=P_ATTR_HPRECOVERRATIO;     m_mapNameValue["E_HpRecoverRatio"]=st;							
    st.SetType(5);	st.psValue	= &m_FightProperty.wEnerRecoverSpeed;		st.attrEnumValue=P_ATTR_ENERRECOVERSPEED;   m_mapNameValue["E_EnerRecoverSpeed"]=st;							
	st.SetType(5);	st.psValue	= &m_FightProperty.wStamRecoverSpeed;		st.attrEnumValue=P_ATTR_STAMRECOVERSPEED;	m_mapNameValue["E_StamRecoverSpeed"]=st;	
    st.SetType(5);  st.psValue  = &m_FightProperty.wFightHpRecoverSpeed;    st.attrEnumValue=P_ATTR_FIGHTHPRECOVERSPEED;m_mapNameValue["E_FightHpRecoverSpeed"]=st;
    st.SetType(5);  st.psValue  = &m_FightProperty.wFightMpRecoverSpeed;    st.attrEnumValue=P_ATTR_FIGHTMPRECOVERSPEED;m_mapNameValue["E_FightMpRecoverSpeed"]=st;
    st.SetType(5);	st.psValue	= &m_FightProperty.wFightEnerRecoverSpeed;	st.attrEnumValue=P_ATTR_FIGHTENERRECOVERSPEED;	m_mapNameValue["E_FightEnerRecoverSpeed"]=st;				
    st.SetType(5);  st.psValue  = &m_FightProperty.wFightStamRecoverSpeed;  st.attrEnumValue=P_ATTR_FIGHTSTAMRECOVERSPEED;m_mapNameValue["E_FightStamRecoverSpeed"]=st;
    
    st.SetType(5);  st.psValue  = &m_FightProperty.wManly;                  st.attrEnumValue=P_ATTR_MANLY;                  m_mapNameValue["E_Manly"]=st;
    st.SetType(5);  st.psValue  = &m_FightProperty.wPerverseTrend;          st.attrEnumValue=P_ATTR_PERVERSETREND;          m_mapNameValue["E_PerverseTrend"]=st;
    st.SetType(5);  st.psValue  = &m_FightProperty.wComplaint;              st.attrEnumValue=P_ATTR_COMPLAINT;              m_mapNameValue["E_Complaint"]=st;
    st.SetType(5);  st.psValue  = &m_FightProperty.wMurderousLook;          st.attrEnumValue=P_ATTR_MURDEROUSLOOK;          m_mapNameValue["E_MurderousLook"]=st;

    st.SetType(5);  st.psValue = &m_FightProperty.wStr;                     st.attrEnumValue=P_ATTR_STR;                    m_mapNameValue["E_Str"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wDex;                     st.attrEnumValue=P_ATTR_DEX;                    m_mapNameValue["E_DEX"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wCon;                     st.attrEnumValue=P_ATTR_CON;                    m_mapNameValue["E_Con"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wIntellect;               st.attrEnumValue=P_ATTR_INTELLECT;              m_mapNameValue["E_Intellect"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wSpiritualism;            st.attrEnumValue=P_ATTR_SPIRITUALISM;           m_mapNameValue["E_Spiritualism"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wPoint;                   st.attrEnumValue=P_ATTR_POINT;                  m_mapNameValue["E_Point"]=st;

    st.SetType(5);	st.psValue = &m_FightProperty.wParry;					st.attrEnumValue=P_ATTR_PARRY;				m_mapNameValue["E_Parry"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wDodge;					st.attrEnumValue=P_ATTR_DODGE;				m_mapNameValue["E_Dodge"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wDef;					    st.attrEnumValue=P_ATTR_DEF;			    m_mapNameValue["E_Def"]=st;	
    st.SetType(5);  st.psValue = &m_FightProperty.HurtFactor;               st.attrEnumValue=P_ATTR_HURTFACTOR;         m_mapNameValue["E_HurtFactor"]=st;
//    st.SetType(5);	st.psValue = &m_FightProperty.wDefSuck;                 st.attrEnumValue=P_ATTR_DEFSUCK;            m_mapNameValue["E_DefSuck"]=st;
    st.SetType(5);	st.psValue = &m_FightProperty.wMdef;					st.attrEnumValue=P_ATTR_MDEF;			    m_mapNameValue["E_Mdef"]=st;	
    st.SetType(5);  st.psValue = &m_FightProperty.wMdefSuck;                st.attrEnumValue=P_ATTR_MDEFSUCK;           m_mapNameValue["E_MdefSuck"]=st;
    st.SetType(5);	st.psValue = &m_FightProperty.wBloodSuck;				st.attrEnumValue=P_ATTR_BLOODSUCK;		    m_mapNameValue["E_BloodSuck"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wManaSuck;				st.attrEnumValue=P_ATTR_MANASUCK;		    m_mapNameValue["E_ManaSuck"]=st;			
 //   st.SetType(5);	st.psValue = &m_FightProperty.wEnergySuck;              st.attrEnumValue=P_ATTR_ENERGYSUCK;         m_mapNameValue["E_EnergySuck"]=st;
    st.SetType(5);	st.psValue = &m_FightProperty.wStaminaSuck;             st.attrEnumValue=P_ATTR_STAMINASUCK;        m_mapNameValue["E_StaminaSuck"]=st;

    st.SetType(5);	st.psValue = &m_FightProperty.wHit;						st.attrEnumValue=P_ATTR_HIT;			    m_mapNameValue["E_Hit"]=st;					
    st.SetType(5);  st.psValue = &m_FightProperty.wAtkSpeed;                st.attrEnumValue=P_ATTR_ATKSPEED;           m_mapNameValue["E_AtkSpeed"]=st;
    st.SetType(5);	st.psValue = &m_FightProperty.wMAtkSpeed;				st.attrEnumValue=P_ATTR_MATKSPEED;		    m_mapNameValue["E_MAtkSpeed"]=st;		
    st.SetType(5);	st.psValue = &m_FightProperty.wMinAtk;					st.attrEnumValue=P_ATTR_MINATK;			    m_mapNameValue["E_MinAtk"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wMaxAtk;					st.attrEnumValue=P_ATTR_MAXATK;			    m_mapNameValue["E_MaxAtk"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wMinMAtk;				st.attrEnumValue=P_ATTR_MINMATK;			m_mapNameValue["E_MinMAtk"]=st;			
    st.SetType(5);	st.psValue = &m_FightProperty.wMaxMAtk;				st.attrEnumValue=P_ATTR_MAXMATK;			m_mapNameValue["E_MaxMAtk"]=st;			
    st.SetType(5);  st.psValue = &m_FightProperty.wPhysicImmunity;      st.attrEnumValue=P_ATTR_PHYSICIMMUNITY;     m_mapNameValue["E_PhysicImmunity"]=st;
    st.SetType(5);  st.psValue = &m_FightProperty.wMagicImmunity;       st.attrEnumValue=P_ATTR_MAGICIMMUNITY;      m_mapNameValue["E_MagicImmunity"]=st;


	st.SetType(5);	st.psValue	= &m_FightProperty.wFightBlock;				st.attrEnumValue=P_ATTR_BLOCK;				m_mapNameValue["E_Block"]=st;			

	st.SetType(5);	st.psValue	= &m_FightProperty.wCharm;						st.attrEnumValue=P_ATTR_CHARM;			m_mapNameValue["E_Charm"]=st;			
	st.SetType(5);	st.psValue	= &m_FightProperty.wAbsorb;						st.attrEnumValue=P_ATTR_ABSORB;			m_mapNameValue["E_Absorb"]=st;			
	st.SetType(5);	st.psValue	= &m_FightProperty.wShieldDef;					st.attrEnumValue=P_ATTR_SHIELD_DEF;		m_mapNameValue["E_ShieldDef"]=st;
	st.SetType(4);	st.psValue	= &m_FightProperty.wCri;						st.attrEnumValue=P_ATTR_CRI;			m_mapNameValue["E_Cri"]=st;					
	st.SetType(5);	st.psValue	= &m_FightProperty.wBuffPower;					st.attrEnumValue=P_ATTR_BUFFPOWER;		m_mapNameValue["E_BuffPower"]=st;		
	st.SetType(4);	st.pValue	= &m_FightProperty.wMAtk;						st.attrEnumValue=P_ATTR_MATK;			m_mapNameValue["E_MAtk"]=st;				
	st.SetType(4);	st.pValue	= &m_FightProperty.wMCri;						st.attrEnumValue=P_ATTR_MCRI;			m_mapNameValue["E_MCri"]=st;				
	st.SetType(5);	st.psValue	= &m_FightProperty.wWillDef;					st.attrEnumValue=P_ATTR_WILLDEF;		m_mapNameValue["E_WillDef"]=st;				
	st.SetType(5);	st.psValue	= &m_FightProperty.wConstiDef;					st.attrEnumValue=P_ATTR_CONSTIDEF;		m_mapNameValue["E_ConstiDef"]=st;		

	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DFT_Bled];			st.attrEnumValue=P_ATTR_DEBUFFDEF_BLED;		 m_mapNameValue["E_DeBuffDef0"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Dizzy];			st.attrEnumValue=P_ATTR_DEBUFFDEF_DIZZY;	 m_mapNameValue["E_DeBuffDef1"]=st;			
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Lull];			st.attrEnumValue=P_ATTR_DEBUFFDEF_LULL;		 m_mapNameValue["E_DeBuffDef2"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Hypnosis];		st.attrEnumValue=P_ATTR_DEBUFFDEF_HYPNOSIS;	 m_mapNameValue["E_DeBuffDef3"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Poison];		st.attrEnumValue=P_ATTR_DEBUFFDEF_POSION;	 m_mapNameValue["E_DeBuffDef4"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Fix];			st.attrEnumValue=P_ATTR_DEBUFFDEF_FIX;		 m_mapNameValue["E_DeBuffDef5"]=st;
	st.SetType(5);st.psValue = &m_FightProperty.wDeBuffDef[DET_Silence];		st.attrEnumValue=P_ATTR_DEBUFFDEF_SILENCE;	 m_mapNameValue["E_DeBuffDef6"]=st;

	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Fire];			 st.attrEnumValue=P_ATTR_ELEMDEF_FIRE;    m_mapNameValue["E_ElemDef0"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Water];		 st.attrEnumValue=P_ATTR_ELEMDEF_WATER;	  m_mapNameValue["E_ElemDef1"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Earth];		 st.attrEnumValue=P_ATTR_ELEMDEF_EARTH;	  m_mapNameValue["E_ElemDef2"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Wind];			 st.attrEnumValue=P_ATTR_ELEMDEF_WIND;	  m_mapNameValue["E_ElemDef3"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Dark];			 st.attrEnumValue=P_ATTR_ELEMDEF_DARK;	  m_mapNameValue["E_ElemDef4"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDef[ET_Holy];			 st.attrEnumValue=P_ATTR_ELEMDEF_HOLY;	  m_mapNameValue["E_ElemDef5"]=st;	


	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Human];			st.attrEnumValue=P_ATTR_CLASSDAM_HUMAN;    m_mapNameValue["E_ClassDam0"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Element];			st.attrEnumValue=P_ATTR_CLASSDAM_ELEMENT;   m_mapNameValue["E_ClassDam1"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Alien];			st.attrEnumValue=P_ATTR_CLASSDAM_ALIEN;	   m_mapNameValue["E_ClassDam2"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Animal];			st.attrEnumValue=P_ATTR_CLASSDAM_ANIMAL;   m_mapNameValue["E_ClassDam3"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Undead];			st.attrEnumValue=P_ATTR_CLASSDAM_UNDEAD;   m_mapNameValue["E_ClassDam4"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Evil];			    st.attrEnumValue=P_ATTR_CLASSDAM_EVIL;	   m_mapNameValue["E_ClassDam5"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wClassDam[C_Hexapods];			st.attrEnumValue=P_ATTR_CLASSDAM_HEXAPODS;   m_mapNameValue["E_ClassDam6"]=st;	

	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Fire];			    		st.attrEnumValue=P_ATTR_ELEMDAM_FIRE;	m_mapNameValue["E_ElemDam0"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Water];					st.attrEnumValue=P_ATTR_ELEMDAM_WATER;	m_mapNameValue["E_ElemDam1"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Earth];					st.attrEnumValue=P_ATTR_ELEMDAM_EARTH;	m_mapNameValue["E_ElemDam2"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Wind];			    		st.attrEnumValue=P_ATTR_ELEMDAM_WIND;	m_mapNameValue["E_ElemDam3"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Dark];			    		st.attrEnumValue=P_ATTR_ELEMDAM_DARK;	m_mapNameValue["E_ElemDam4"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemDam[ET_Holy];			    		st.attrEnumValue=P_ATTR_ELEMDAM_HOLY;	m_mapNameValue["E_ElemDam5"]=st;	

	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Fire];			    		st.attrEnumValue=P_ATTR_ELEMATK_FIRE;	m_mapNameValue["E_ElemAtk0"]=st;		
	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Water];					st.attrEnumValue=P_ATTR_ELEMATK_WATER;	m_mapNameValue["E_ElemAtk1"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Earth];					st.attrEnumValue=P_ATTR_ELEMATK_EARTH;	m_mapNameValue["E_ElemAtk2"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Wind];			    		st.attrEnumValue=P_ATTR_ELEMATK_WIND;	m_mapNameValue["E_ElemAtk3"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Dark];			    		st.attrEnumValue=P_ATTR_ELEMATK_DARK;	m_mapNameValue["E_ElemAtk4"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wElemAtk[ET_Holy];			    		st.attrEnumValue=P_ATTR_ELEMATK_HOLY;	m_mapNameValue["E_ElemAtk5"]=st;	

	st.SetType(5);st.psValue = &m_FightProperty.wCriDef;		    st.attrEnumValue=P_ATTR_CRIDEF;	m_mapNameValue["E_CriDef"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wMCriDef;		    st.attrEnumValue=P_ATTR_MCRIDEF;	m_mapNameValue["E_MCriDef"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wCriDamage;		    st.attrEnumValue=P_ATTR_CRIDAMAGE;	m_mapNameValue["E_CriDamage"]=st;	
	st.SetType(5);st.psValue = &m_FightProperty.wMCriDamage;		st.attrEnumValue=P_ATTR_MCRIDAMAGE;	m_mapNameValue["E_MCriDamage"]=st;	

	st.SetType(5);	st.psValue	= &m_FightProperty.wImmunity;		st.attrEnumValue=P_ATTR_IMMUNITY;		m_mapNameValue["E_Immunity"]=st;				
	st.SetType(5);	st.psValue	= &m_FightProperty.wPierce;			st.attrEnumValue=P_ATTR_PIERCE;			m_mapNameValue["E_Pierce"]=st;
	st.SetType(5);	st.psValue	= &m_FightProperty.wLuck;			st.attrEnumValue=P_ATTR_LUCK;			m_mapNameValue["E_Luck"]=st;
	st.SetType(4);  st.pValue   = &m_FightProperty.lMaxPKCP;        st.attrEnumValue = P_ATTR_MAXPKCP;      m_mapNameValue["E_MaxPKCP"]=st;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*����ս������*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.attrEnumValue = 0;
    st.SetType(4);	st.pValue	= &m_BaseFightProperty.dwMaxHp;					m_mapNameValue["B_MaxHp"]=st;			
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wMaxMp;					m_mapNameValue["B_MaxMp"]=st;			
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wMaxStamina;				m_mapNameValue["B_MaxStamina"]=st;		
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wMaxEnergy;				m_mapNameValue["B_MaxEnergy"]=st;		
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wHpRecoverSpeed;			m_mapNameValue["B_HpRecoverSpeed"]=st;	
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wMpRecoverSpeed;			m_mapNameValue["B_MpRecoverSpeed"]=st;	
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wHpRecoverRatio;			m_mapNameValue["B_HpRecoverRatio"]=st;							
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wEnerRecoverSpeed;		m_mapNameValue["B_EnerRecoverSpeed"]=st;							
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wStamRecoverSpeed;		m_mapNameValue["B_StamRecoverSpeed"]=st;	
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wFightHpRecoverSpeed;    m_mapNameValue["B_FightHpRecoverSpeed"]=st;
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wFightMpRecoverSpeed;    m_mapNameValue["B_FightMpRecoverSpeed"]=st;
    st.SetType(5);	st.psValue	= &m_BaseFightProperty.wFightEnerRecoverSpeed;	m_mapNameValue["B_FightEnerRecoverSpeed"]=st;				
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wFightStamRecoverSpeed;  m_mapNameValue["B_FightStamRecoverSpeed"]=st;

    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wManly;                  m_mapNameValue["B_Manly"]=st;
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wPerverseTrend;          m_mapNameValue["B_PerverseTrend"]=st;
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wComplaint;              m_mapNameValue["B_Complaint"]=st;
    st.SetType(5);  st.psValue  = &m_BaseFightProperty.wMurderousLook;          m_mapNameValue["B_MurderousLook"]=st;

    st.SetType(5);  st.psValue = &m_BaseFightProperty.wStr;                     m_mapNameValue["B_Str"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wDex;                     m_mapNameValue["B_DEX"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wCon;                     m_mapNameValue["B_Con"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wIntellect;               m_mapNameValue["B_Intellect"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wSpiritualism;            m_mapNameValue["B_Spiritualism"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wPoint;                   m_mapNameValue["B_Point"]=st;

    st.SetType(5);	st.psValue = &m_BaseFightProperty.wParry;				m_mapNameValue["B_Parry"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wDodge;				m_mapNameValue["B_Dodge"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wDef;					m_mapNameValue["B_Def"]=st;	
 //   st.SetType(5);	st.psValue = &m_BaseFightProperty.wDefSuck;             m_mapNameValue["B_DefSuck"]=st;
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMdef;				m_mapNameValue["B_Mdef"]=st;	
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wMdefSuck;            m_mapNameValue["B_MdefSuck"]=st;
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wBloodSuck;			m_mapNameValue["B_BloodSuck"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wManaSuck;			m_mapNameValue["B_ManaSuck"]=st;			
 //   st.SetType(5);	st.psValue = &m_BaseFightProperty.wEnergySuck;          m_mapNameValue["B_EnergySuck"]=st;
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wStaminaSuck;         m_mapNameValue["B_StaminaSuck"]=st;

    st.SetType(5);	st.psValue = &m_BaseFightProperty.wHit;					m_mapNameValue["B_Hit"]=st;					
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wAtkSpeed;            m_mapNameValue["B_AtkSpeed"]=st;
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMAtkSpeed;			m_mapNameValue["B_MAtkSpeed"]=st;		
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMinAtk;				m_mapNameValue["B_MinAtk"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMaxAtk;				m_mapNameValue["B_MaxAtk"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMinMAtk;				m_mapNameValue["B_MinMAtk"]=st;			
    st.SetType(5);	st.psValue = &m_BaseFightProperty.wMaxMAtk;				m_mapNameValue["B_MaxMAtk"]=st;			
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wPhysicImmunity;      m_mapNameValue["B_PhysicImmunity"]=st;
    st.SetType(5);  st.psValue = &m_BaseFightProperty.wMagicImmunity;       m_mapNameValue["B_MagicImmunity"]=st;

	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wShieldDef;				m_mapNameValue["B_ShieldDef"]=st;							
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wImmunity;				m_mapNameValue["B_Immunity"]=st;				
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wPierce;					m_mapNameValue["B_Pierce"]=st;
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wLuck;					m_mapNameValue["B_Luck"]=st;

	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wFightBlock;					m_mapNameValue["B_Block"]=st;				//��			
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wAbsorb;					m_mapNameValue["B_Absorb"]=st;				//רע			
	st.SetType(4);	st.psValue	= &m_BaseFightProperty.wCri;						m_mapNameValue["B_Cri"]=st;					//������			
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wBuffPower;				m_mapNameValue["B_BuffPower"]=st;			//Buff����ǿ��		
	st.SetType(4);	st.pValue	= &m_BaseFightProperty.wMAtk;						m_mapNameValue["B_MAtk"]=st;					//ħ��������			
	st.SetType(4);	st.pValue	= &m_BaseFightProperty.wMCri;						m_mapNameValue["B_MCri"]=st;					//ħ��������			
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wWillDef;					m_mapNameValue["B_WillDef"]=st;				//��־����		
	st.SetType(5);	st.psValue	= &m_BaseFightProperty.wConstiDef;				m_mapNameValue["B_ConstiDef"]=st;			//���ʿ���

	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DFT_Bled];		m_mapNameValue["B_DeBuffDef0"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Dizzy];		m_mapNameValue["B_DeBuffDef1"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Lull];		m_mapNameValue["B_DeBuffDef2"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Hypnosis];		m_mapNameValue["B_DeBuffDef3"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Poison];		m_mapNameValue["B_DeBuffDef4"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Fix];		m_mapNameValue["B_DeBuffDef5"]=st;			//Debuff����
	st.SetType(5);st.psValue = &m_BaseFightProperty.wDeBuffDef[DET_Silence];	m_mapNameValue["B_DeBuffDef6"]=st;			

	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Fire];			m_mapNameValue["B_ElemDef0"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Water];			m_mapNameValue["B_ElemDef1"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Earth];			m_mapNameValue["B_ElemDef2"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Wind];			m_mapNameValue["B_ElemDef3"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Dark];			m_mapNameValue["B_ElemDef4"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDef[ET_Holy];			m_mapNameValue["B_ElemDef5"]=st; //����Ԫ�ؿ���

	{
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Attack];		st.attrEnumValue=P_EARTH_ATTACK;	m_mapNameValue["Earth_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Defend];		st.attrEnumValue=P_EARTH_DEFEND;	m_mapNameValue["Earth_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Attack];		st.attrEnumValue=P_WATER_ATTACK;	m_mapNameValue["Water_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Defend];		st.attrEnumValue=P_WATER_DEFEND;	m_mapNameValue["Water_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Attack];		st.attrEnumValue=P_FIRE_ATTACK;		m_mapNameValue["Fire_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Defend];		st.attrEnumValue=P_FIRE_DEFEND;		m_mapNameValue["Fire_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Attack];		st.attrEnumValue=P_WIND_ATTACK;		m_mapNameValue["Wind_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Defend];		st.attrEnumValue=P_WIND_DEFEND;		m_mapNameValue["Wind_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Attack];		st.attrEnumValue=P_BRIGHT_ATTACK;	m_mapNameValue["Bright_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Defend];		st.attrEnumValue=P_BRIGHT_DEFEND;	m_mapNameValue["Bright_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Attack];		st.attrEnumValue=P_DARK_ATTACK;		m_mapNameValue["Dark_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Defend];		st.attrEnumValue=P_DARK_DEFEND;		m_mapNameValue["Dark_Defend"]=st;

		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_AttackXiang];			st.attrEnumValue=P_EARTH_ATTACKXIANG;	m_mapNameValue["Earth_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_DefendXiang];			st.attrEnumValue=P_EARTH_DEFENDXIANG;	m_mapNameValue["Earth_DefendXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_AttackXiang];			st.attrEnumValue=P_WATER_ATTACKXIANG;	m_mapNameValue["Water_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_DefendXiang];			st.attrEnumValue=P_WATER_DEFENDXIANG;	m_mapNameValue["Water_DefendXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_AttackXiang];			st.attrEnumValue=P_FIRE_ATTACKXIANG;	m_mapNameValue["Fire_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_DefendXiang];			st.attrEnumValue=P_FIRE_DEFENDXIANG;	m_mapNameValue["Fire_DefendXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_AttackXiang];			st.attrEnumValue=P_WIND_ATTACKXIANG;	m_mapNameValue["Wind_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_DefendXiang];			st.attrEnumValue=P_WIND_DEFENDXIANG;	m_mapNameValue["Wind_DefendXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_AttackXiang];		st.attrEnumValue=P_BRIGHT_ATTACKXIANG;	m_mapNameValue["Bright_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_DefendXiang];		st.attrEnumValue=P_BRIGHT_DEFENDXIANG;	m_mapNameValue["Bright_DefendXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_AttackXiang];			st.attrEnumValue=P_DARK_ATTACKXIANG;	m_mapNameValue["Dark_AttackXiang"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_DefendXiang];			st.attrEnumValue=P_DARK_DEFENDXIANG;	m_mapNameValue["Dark_DefendXiang"]=st;
	}


	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Human];			m_mapNameValue["B_ClassDam0"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Element];	m_mapNameValue["B_ClassDam1"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Alien];			m_mapNameValue["B_ClassDam2"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Animal];			m_mapNameValue["B_ClassDam3"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Undead];			m_mapNameValue["B_ClassDam4"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Evil];			    m_mapNameValue["B_ClassDam5"]=st;
	st.SetType(5);st.psValue = &m_BaseFightProperty.wClassDam[C_Hexapods];			m_mapNameValue["B_ClassDam6"]=st;//�Ը��������˺�

	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Fire];			    m_mapNameValue["B_ElemDam0"]=st;			//�Ը���Ԫ���˺�
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Water];			m_mapNameValue["B_ElemDam1"]=st;			//�Ը���Ԫ���˺�
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Earth];			m_mapNameValue["B_ElemDam2"]=st;			//�Ը���Ԫ���˺�
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Wind];			    m_mapNameValue["B_ElemDam3"]=st;			//�Ը���Ԫ���˺�
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Dark];			    m_mapNameValue["B_ElemDam4"]=st;			//�Ը���Ԫ���˺�
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemDam[ET_Holy];			    m_mapNameValue["B_ElemDam5"]=st;			//�Ը���Ԫ���˺�

	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Fire];				m_mapNameValue["B_ElemAtk0"]=st;		
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Water];			m_mapNameValue["B_ElemAtk1"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Earth];			m_mapNameValue["B_ElemAtk2"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Wind];				m_mapNameValue["B_ElemAtk3"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Dark];				m_mapNameValue["B_ElemAtk4"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wElemAtk[ET_Holy];				m_mapNameValue["B_ElemAtk5"]=st;

	st.SetType(5);st.psValue = &m_BaseFightProperty.wCriDef;		m_mapNameValue["B_CriDef"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wMCriDef;		m_mapNameValue["B_MCriDef"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wCriDamage;		m_mapNameValue["B_CriDamage"]=st;	
	st.SetType(5);st.psValue = &m_BaseFightProperty.wMCriDamage;		m_mapNameValue["B_MCriDamage"]=st;	

	st.SetType(4); st.pValue = &m_BaseFightProperty.lMaxPKCP;   m_mapNameValue["B_MaxPKCP"] = st;


	st.SetType(0);	st.plValue  = &m_AppendProperty.bShowFashion;			m_mapNameValue["ShowFashion"]=st;	m_mapAttrType["ShowFashion"]=P_ATTR_DISPLAYHEADPIECE;	//	����ʾͷ��

	st.SetType(0);	st.plValue=&m_Property.byCountry;		m_mapNameValue["btCountry"]=st;		m_mapAttrType["btCountry"]=P_ATTR_COUNTRY;					//	���ұ��

	st.SetType(3);	st.pfValue=&m_fExpScale;                m_mapNameValue["ExpScale"]=st;
	st.SetType(3);	st.pfValue=&m_fOccuExpScale; 	        m_mapNameValue["OccuExpScale"]=st;
	st.SetType(3);	st.pfValue=&m_fSpScale;             	m_mapNameValue["SpScale"]=st;
	st.SetType(3);	st.pfValue=&m_fPresentExpScale;	        m_mapNameValue["PresentExpScale"]=st;

	// �������
	st.SetType(0); st.plValue = &m_Property.byIsBusinessMan; m_mapNameValue["BusinessMan"] = st;

	m_mapDirtyAttrData.clear();	
	AddFightPropertyTODirty();
}

//��ʼ�ͻ�����Ҫ���µ���������
void CPlayer::InitUpdateDirtyAttrData()
{
	/*m_mapSelfUpdateDirtyAttrData.insert("Occu");
	m_mapSelfUpdateDirtyAttrData.insert("DOccu");
	m_mapSelfUpdateDirtyAttrData.insert("Class");
	m_mapSelfUpdateDirtyAttrData.insert("Sex");
	m_mapSelfUpdateDirtyAttrData.insert("HeadPic");
	m_mapSelfUpdateDirtyAttrData.insert("Hp");
	m_mapSelfUpdateDirtyAttrData.insert("Mp");
	m_mapSelfUpdateDirtyAttrData.insert("Energy");
	m_mapSelfUpdateDirtyAttrData.insert("Stamina");
	m_mapSelfUpdateDirtyAttrData.insert("Level");
	m_mapSelfUpdateDirtyAttrData.insert("Exp");
	m_mapSelfUpdateDirtyAttrData.insert("PresentExp");
	m_mapSelfUpdateDirtyAttrData.insert("RankOfNobility");
	m_mapSelfUpdateDirtyAttrData.insert("RankOfNobCredit");
	m_mapSelfUpdateDirtyAttrData.insert("RankOfMercenary");
	m_mapSelfUpdateDirtyAttrData.insert("MercenaryCredit");
	m_mapSelfUpdateDirtyAttrData.insert("Country");
	m_mapSelfUpdateDirtyAttrData.insert("CountryContribute");
	m_mapSelfUpdateDirtyAttrData.insert("BusinessLevel");
	m_mapSelfUpdateDirtyAttrData.insert("BusinessExp");
	m_mapSelfUpdateDirtyAttrData.insert("ArtisanCredit");
	m_mapSelfUpdateDirtyAttrData.insert("ArtisanLevel");
	m_mapSelfUpdateDirtyAttrData.insert("ArtisanExp");
	m_mapSelfUpdateDirtyAttrData.insert("PkValue");
	m_mapSelfUpdateDirtyAttrData.insert("PkCount");
	m_mapSelfUpdateDirtyAttrData.insert("PVPCount");
	m_mapSelfUpdateDirtyAttrData.insert("PVPValue");
	m_mapSelfUpdateDirtyAttrData.insert("UpgradeExp");
	m_mapSelfUpdateDirtyAttrData.insert("ShowCountry");


	m_mapSelfUpdateDirtyAttrData.insert("C_MaxHp");
	m_mapSelfUpdateDirtyAttrData.insert("C_MaxMp");
	m_mapSelfUpdateDirtyAttrData("C_MaxStamina");
	m_mapSelfUpdateDirtyAttrData.insert("C_MaxEnergy");
	m_mapDirtyAttrData.insert("C_Strenth");
	m_mapDirtyAttrData.insert("C_Agility");
	m_mapDirtyAttrData.insert("C_Consti");
	m_mapDirtyAttrData.insert("C_Intell");
	m_mapDirtyAttrData.insert("C_Wisdom");
	m_mapDirtyAttrData.insert("C_Spirit");
	m_mapDirtyAttrData.insert("C_Block");
	m_mapDirtyAttrData.insert("C_Parry");
	m_mapDirtyAttrData.insert("C_Dodge");
	m_mapDirtyAttrData.insert("C_Charm");
	m_mapDirtyAttrData.insert("C_Hit");
	m_mapDirtyAttrData.insert("C_Absorb");
	m_mapDirtyAttrData.insert("C_Def");
	m_mapDirtyAttrData.insert("C_Mdef");
	m_mapDirtyAttrData.insert("C_Cri");
	m_mapDirtyAttrData.insert("C_MAtkSpeed");
	m_mapDirtyAttrData.insert("C_BuffPower");
	m_mapDirtyAttrData.insert("C_MinAtk");
	m_mapDirtyAttrData.insert("C_MaxAtk");
	m_mapDirtyAttrData.insert("C_MAtk");
	m_mapDirtyAttrData.insert("C_MCri");
	m_mapDirtyAttrData.insert("C_CriDef");
	m_mapDirtyAttrData.insert("C_MCriDef");
	m_mapDirtyAttrData.insert("C_BloodSuck");
	m_mapDirtyAttrData.insert("C_ManaSuck");
	m_mapDirtyAttrData.insert("C_WillDef");
	m_mapDirtyAttrData.insert("C_ConstiDef");
	m_mapDirtyAttrData.insert("C_DeBuffDef0");
	m_mapDirtyAttrData.insert("C_DeBuffDef1");
	m_mapDirtyAttrData.insert("C_DeBuffDef2");
	m_mapDirtyAttrData.insert("C_DeBuffDef3");
	m_mapDirtyAttrData.insert("C_DeBuffDef4");
	m_mapDirtyAttrData.insert("C_DeBuffDef5");
	m_mapDirtyAttrData.insert("C_ElemDef0");
	m_mapDirtyAttrData.insert("C_ElemDef1");
	m_mapDirtyAttrData.insert("C_ElemDef2");
	m_mapDirtyAttrData.insert("C_ElemDef3");
	m_mapDirtyAttrData.insert("C_ElemDef4");
	m_mapDirtyAttrData.insert("C_ElemDef5");
	m_mapDirtyAttrData.insert("C_ClassDam0");
	m_mapDirtyAttrData.insert("C_ClassDam1");
	m_mapDirtyAttrData.insert("C_ClassDam2");
	m_mapDirtyAttrData.insert("C_ClassDam3");
	m_mapDirtyAttrData.insert("C_ClassDam4");
	m_mapDirtyAttrData.insert("C_ClassDam5");
	m_mapDirtyAttrData.insert("C_ClassDam6");
	m_mapDirtyAttrData.insert("C_ElemDam0");
	m_mapDirtyAttrData.insert("C_ElemDam1");
	m_mapDirtyAttrData.insert("C_ElemDam2");
	m_mapDirtyAttrData.insert("C_ElemDam3");
	m_mapDirtyAttrData.insert("C_ElemDam4");
	m_mapDirtyAttrData.insert("C_ElemDam5");
	m_mapDirtyAttrData.insert("C_ElemAtk0");
	m_mapDirtyAttrData.insert("C_ElemAtk1");
	m_mapDirtyAttrData.insert("C_ElemAtk2");
	m_mapDirtyAttrData.insert("C_ElemAtk3");
	m_mapDirtyAttrData.insert("C_ElemAtk4");
	m_mapDirtyAttrData.insert("C_ElemAtk5");*/

	// ��ʼ��֪ͨ��simple����
	m_mapOtherUpdateDirtyAttrData.insert(string("Hp"));
	m_mapOtherUpdateDirtyAttrData.insert(string("C_MaxHp"));
	m_mapOtherUpdateDirtyAttrData.insert(string("ShowCountry"));
	m_mapOtherUpdateDirtyAttrData.insert(string("bInvisible"));
	m_mapOtherUpdateDirtyAttrData.insert(string("bRide"));
	m_mapOtherUpdateDirtyAttrData.insert(string("GMConcealment"));
	// PK CP ֵ
	m_mapOtherUpdateDirtyAttrData.insert( "CurPKCP" );
	m_mapOtherUpdateDirtyAttrData.insert( "MaxPKCP" );

	/*m_mapOtherUpdateDirtyAttrData.insert(string("PkValue"));
	m_mapOtherUpdateDirtyAttrData.insert(string("PVPValue"));*/
}

long CPlayer::GetValue(const char* strName)
{
	long value = INVILID_VALUE;
	value = CMoveShape::GetValue(strName);
	if(INVILID_VALUE != value)
		return value;

	mapNameValueItr itr = m_mapNameValue.begin();
	itr = m_mapNameValue.find(strName);
	if (itr != m_mapNameValue.end())
	{
		switch (itr->second.lType)
		{
		case 0:	// byte
			return *(itr->second.plValue);

		case 1:	// word
			return *(itr->second.pwValue);

		case 2:	// dword
			return *(itr->second.pdwValue);

		case 3:	// float
			return (long)*(itr->second.pfValue);

		case 4:	// long
			return *(itr->second.pValue);

		case 5:	// short
			return *(itr->second.psValue);

		case 6:	// bool
			return *(itr->second.pbValue);
		}
	}
	return value;
}

long CPlayer::SetValue(const char* strName, long dwValue)
{
#ifdef _DEBUG_SKILL
	/*char strDebug[256];
	_snprintf(strDebug, 256, "���״̬��������%sֵ%d\n",strName,dwValue);
	OutputDebugStr(strDebug);	*/
#endif


	long value = INVILID_VALUE;

	if (GetState()==STATE_DIED && strcmp(strName,"Hp")==0 && dwValue>0)
	{
		return 0;
	}
	value = CMoveShape::SetValue(strName, dwValue);
	if(INVILID_VALUE != value)
		return value;

	mapNameValueItr itr = m_mapNameValue.begin();
	itr = m_mapNameValue.find(strName);
	if (itr != m_mapNameValue.end())
	{
		long lOldValue = GetValue(strName);;
		/*	if(dwValue < itr->second.minValue)
		dwValue = itr->second.minValue;
		if(dwValue > itr->second.maxValue)
		dwValue = itr->second.maxValue;*/

		switch (itr->second.lType)
		{
		case 0:	// byte
			*(itr->second.plValue) = (BYTE)dwValue;
			break;
		case 1:	// word
			*(itr->second.pwValue) = (WORD)dwValue;
			break;
		case 2:	// dword
			*(itr->second.pdwValue) = (DWORD)dwValue;
			break;
		case 3:	// float
			*(itr->second.pfValue) = (float)dwValue;
			break;
		case 4:	// long
			*(itr->second.pValue) = (long)dwValue;
			break;
		case 5:	// short
			*(itr->second.psValue) = (short)dwValue;
			break;
		case 6:	// bool
			*(itr->second.pbValue) = dwValue==0?false:true;
			break;
		}

		OnPropertyValueChanged(strName, dwValue);

		if (strcmp("PkValue", strName) == 0 || strcmp("PVPValue", strName) == 0)
		{
			OnPKOrPVPValueChange(lOldValue,dwValue);
		}
		else if (strcmp("E_MaxHp",strName) == 0)
		{
			string strHp = "Hp";
			string strMaxHp = "C_MaxHp";
			if (GetAttribute(strHp)>(long)GetMaxHP())
			{
				SetAttribute(strHp,(long)GetMaxHP());
			}
		}
		else if (strcmp("E_MaxMp",strName) == 0)
		{
			string strMp = "Mp";
			string strMaxMp = "C_MaxMp";
			if (GetAttribute(strMp)>(long)GetMaxMP())
			{
				SetAttribute(strMp,(long)GetMaxMP());
			}
		}
		// ������ǰPK CPֵ
		else if( strcmp( "E_MaxPKCP", strName ) == 0 )
		{
			std::string strPKCP = "CurPKCP";
			long lMaxPKCP = GetMaxPKCP();
			if( GetAttribute( strPKCP ) > (long)lMaxPKCP )
			{
				SetAttribute( strPKCP, lMaxPKCP );
			}
		}

		return dwValue;
	}


	return value;
}

void  CPlayer::OnPropertyValueChanged(const char* strName, LONG lValue)
{
	//������������ݡ����ʡ��������ǻۡ�����֮һ�Ļ���ֵ��仯ֵ�����ı�
	//Ҫ���¼����������ս������
	if(NULL == strName)	return;
	if(0 == strncmp(strName,"B_Str",9) || 0 == strncmp(strName,"E_Str",9) ||
		0 == strncmp(strName,"B_Dex",9) || 0 == strncmp(strName,"E_Dex",9) ||
		0 == strncmp(strName,"B_Con",8) || 0 == strncmp(strName,"E_Con",8) ||
		0 == strncmp(strName,"B_Intellect",8) || 0 == strncmp(strName,"E_Intellect",8) ||
//		0 == strncmp(strName,"B_Wisdom",8) || 0 == strncmp(strName,"E_Wisdom",8) ||
		0 == strncmp(strName,"B_Spiritualism",8) || 0 == strncmp(strName,"E_Spiritualism",8))
	{
		ResetFightProperty();
	}
	//�����ְҵ�ȼ������仯,�򴥷���Ӧ�ӿ�
	else if( 0 == strncmp(strName,"OccuLvl",7) )
	{
		OnOccuLevelChanged();
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	}

	//�����ְҵ�����仯,�򴥷���Ӧ�ӿ�
	else if( 0 == strcmp(strName,"Occu") )
	{
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	}
	else if( 0 == strcmp(strName,"RankOfNobility") )
	{
		//���¾�λ�ƺ�
		if(GetTitleType() == ePTT_Knight)
			UpdateTitle(true);
	}

	//! [2007-8-7 add]
	if (NULL_GUID == GetTeamID())
	{
		return;
	}

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL == pTeam)
	{
		return;
	}

	long lChangeType = 0;

	if (0 == strcmp(strName, "lTileX") || 0 == strcmp(strName, "lTileY") )
	{
		lChangeType = eMIS_Pos;
	}
	else if (0 == strcmp(strName, "Hp"))
	{
		lChangeType = eMIS_CurrHp;
	}
	else if (0 == strcmp(strName, "Mp"))
	{
		lChangeType = eMIS_CurrMp;
	}
	else if (0 == strcmp(strName, "CurPKCP"))
	{
		lChangeType = eMIS_CurrEnergy;
	}
	else if (0 == strcmp(strName, "B_MaxHp") || 0 == strcmp(strName, "E_MaxHp") || 0 == strcmp(strName, "C_MaxHp"))
	{
		lChangeType = eMIS_MaxHp;
	}
	else if (0 == strcmp(strName, "B_MaxMp") || 0 == strcmp(strName, "E_MaxMp") || 0 == strcmp(strName, "C_MaxMp"))
	{
		lChangeType = eMIS_MaxMp;
	}
	else if (0 == strcmp(strName, "MaxPKCP"))
	{
		lChangeType = eMIS_MaxEnergy;
	}
	else if (0 == strcmp(strName, "Level"))
	{
		lChangeType = eMIS_Level;
	}
	else if (0 == strcmp(strName, "Occu"))
	{
		lChangeType = eMIS_Occupation;
	}
	else if (0 == strcmp(strName, "Sex"))
	{
		lChangeType = eMIS_Sex;
	}
	else
	{
		return;
	}

	pTeam->OnPlayerIdioinfoChange(GetExID(), (eUpdateIdioinfo)lChangeType, lValue);

	//! [add end]
}

//////////////////////////////////////////////////////////////////////////
//	����
//////////////////////////////////////////////////////////////////////////

//�ж�cooldown�Ƿ�ʱ
bool CPlayer::CooldownPast(long lSkillID)
{
	DWORD lEndTime = 0;
	DWORD dwCurTime = timeGetTime();
	itSkill it = m_Skills.find(lSkillID);
	if(it== m_Skills.end())	
		return true;
#ifdef _DEBUG
	/*char str[256];
	_snprintf(str, 256, "\n����ID:%d,��ȴ��ǰʱ��:%d,��ȴ����ʱ��%d,��ȴ��ʼʱ��:%d,��ȴʱ��%d\n",lSkillID,dwCurTime,(*it).second.dwCooldownStartTime + (*it).second.wCooldown,(*it).second.dwCooldownStartTime, (*it).second.wCooldown);
	OutputDebugString(str);*/
#endif
	lEndTime = (*it).second.dwCooldownStartTime+ (*it).second.wCooldown;
 	return 	dwCurTime >= lEndTime;//(*it).second.dwCooldownStartTime+ (*it).second.wCooldown;
}
//����ID����״̬
void CPlayer::EndBuff(DWORD dwStateID)
{
	CMoveShape::EndBuff(dwStateID);
}
//���ù�����ȴʱ��
void CPlayer::SetCommonCooldown(long lID, DWORD dwTime)
{
	//�������ܹ�����ȴʱ��Ӱ��ļ��ܼ���ȴʱ��
	DWORD dwCurTime = timeGetTime();
	itSkill it = m_Skills.begin();
	for(;it!=m_Skills.end();it++)
	{
		time_t t_StartTime;
		t_StartTime = time(NULL); 

		tagSkill &skill = (*it).second;
		if((*it).first == lID)
		{
			DWORD dwRemainTime = 0;
			if(dwCurTime < skill.dwCooldownStartTime+skill.wCooldown)
				dwRemainTime = skill.wCooldown-(dwCurTime-skill.dwCooldownStartTime);
			if (dwRemainTime<dwTime)
			{
				dwRemainTime = dwTime;
			}		
			skill.dwCooldownStartTime=dwCurTime;
			skill.wCooldown=dwRemainTime;
			skill.tCooldownEndtime = t_StartTime + time_t(dwRemainTime/1000);
		}
		else
		{
			DWORD dwRemainTime = 0;			
			if(dwCurTime < skill.dwCooldownStartTime+skill.wCooldown)
				dwRemainTime = skill.wCooldown-(dwCurTime-skill.dwCooldownStartTime);
			if (dwRemainTime<dwTime)
			{
				dwRemainTime = dwTime;
			}

				skill.dwCooldownStartTime = dwCurTime;
			skill.wCooldown = dwRemainTime;
		}
	}
	return;
}
void CPlayer::SendUseItemSucceed(long lGoodsIndex)
{
	CMessage msg(MSG_S2C_PLAYER_USEITEM);
	msg.Add((char)PLAYER_USEITEM_SUCCEED);
	msg.Add(GetExID());
	msg.Add(lGoodsIndex);
	msg.SendToAround( this );
}

bool CPlayer::GoodsCoolDown(long lGoodsIndex)
{
	CGoodsBaseProperties* pProperty=GoodsSetup::QueryGoodsBaseProperties(lGoodsIndex);
	if(pProperty)
	{
		//��ȴ���
		DWORD dwCoolDownTime = pProperty->GetAddonPropertyValue(GAP_COOLDOWN,1);
		DWORD dwCoolDownId = pProperty->GetAddonPropertyValue(GAP_COOLDOWN,2);

		if(dwCoolDownId!=0)
		{
			//�й�����ȴ����
			map<DWORD,tagGoodsCommonCooldown>::iterator iter=m_mapClassIdCoolDown.find(dwCoolDownId);
			if(iter!=m_mapClassIdCoolDown.end())
			{
				if(timeGetTime() - iter->second.dwCooldownStartTime<=dwCoolDownTime)				
				{
					return false;
				}			
			}
		}
		else if(dwCoolDownTime!=0)
		{
			//GOODSID��ȴ����
			map<DWORD,tagGoodsCommonCooldown>::iterator iter=m_mapGoodsIdCoolDown.find(lGoodsIndex);
			if(iter!=m_mapGoodsIdCoolDown.end())
			{
				if(timeGetTime()-iter->second.dwCooldownStartTime<=dwCoolDownTime)
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

int CPlayer::JudgeSkillBegining(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget,long lGoodsIndex)
{
    DWORD dwCurrTime = timeGetTime();

    if ( !IsCanUseAllSkill() )
    {
        return SKILL_USE_FAILED_INVALID_NOTUSESKILL;
    }

	if (lGoodsIndex==0 && !CooldownPast(dwSkillID))
	{	
		//��ȴʱ��
		return SKILL_USE_FAILED_INVALID_TIME;
	}

	if (lGoodsIndex!=0 && !GoodsCoolDown(lGoodsIndex))
	{
		//��Ʒ��ȴʱ��
		return SKILL_USE_FAILED_INVALID_TIME;
	}

	if (dwCurrTime<m_dwIntonateStartTime + m_dwIntonateTime)
	{
		//����ʱ��
		return SKILL_USE_FAILED_INVALID_INTONATE;		
	}

    if (dwCurrTime < (m_dwCDStartTime+m_dwCDControlTime) )
    {
        //����ȫ�ֵ�CDʱ��
        return SKILL_USE_FAILED_INVALID_CDTIME;
    }

    if ( m_Buddhism.IsFull() )
    {
#ifdef  _DEBUG
        char  szText[256]="[Colour 'ffffffff'] ����ֵ���߲���ʹ�ü���";
        SendNotifyMessage( szText, eNOTIFYPOS_LEFT_BOTTOM );
#endif
        /// ����ֵ���˲���ʹ�ü���
        return SKILL_USE_FAILED_INVALID_FULLDISTRACT;
    }

	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwSkillID);
	if (!pSkillProperty)
	{
		return SKILL_USE_FAILED_INVALID_UNKNOWN;
	}
	if (pSkillProperty)
	{	
		long lActTime = GetSkillValue( dwSkillID , dwLevel ,"ActTime" );
		if (lActTime>0)
		{
			if (IsInAct())
			{
				//����ʱ��
				return SKILL_USE_FAILED_INVALID_ACT;
			}
		}
	}
	if (pSkillProperty && pTarget)
	{
		if (pSkillProperty->GetTarget()==0 && pSkillProperty->GetDeadUse()==0 && pTarget->GetState()==STATE_DIED)
		{
			return SKILL_USE_FAILED_INVALID_STATE;
		}
        if( PHASEMAN.PhasePlayerFilter( this , pTarget) )
        {
            /// ��ͬ����λ
            return SKILL_USE_FAILED_INVALID_NOTSAMEPHASE;
        }
	}

	if (this->IsDied())
	{
		//����
		return SKILL_USE_FAILED_INVALID_STATE;
	}

	if (GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING 
        || IsLockPlayerState() )
	{
		/// ��̯/����/����/���� ״̬�²���ʹ�ü���
		return SKILL_USE_FAILED_INVALID_STATE;
	}

	if (GetUseFightSkill() == FALSE)
	{
		if (pSkillProperty->GetUseType()==1)
		{
			//����ʹ��սʿ�༼��
			return SKILL_USE_FAILED_INVALID_STATE;
		}
	}
	
	if (GetUseRabbSkill() == FALSE)
	{
		if (pSkillProperty->GetUseType()==2)
		{
			//����ʹ��ʦ�༼��
			return SKILL_USE_FAILED_INVALID_STATE;
		}
	}

	return -1;
}
long CPlayer::AddActiveSkill(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget,long lGoodsIndex,long lContainerID,long lPos,CGUID GoodsGuid)
{
	SetInstanceSkill(NULL);
	int nRet = JudgeSkillBegining(dwSkillID,dwLevel,pTarget,lGoodsIndex);

	if (nRet!=-1)
	{
#ifdef _SKILL_DEBUG
		Log4c::Trace(ROOT_MODULE,"���ͼ���ʧ����Ϣ:����ID:%6d\n",dwSkillID);
#endif
		CMessage msg(MSG_S2C_SKILL_USE);
		msg.Add((CHAR)SKILL_USE_RESULT_FAILED);
		msg.Add( this -> GetExID() );
		//id�͵ȼ�
		msg.Add((long)dwSkillID);
		msg.Add((BYTE)dwLevel );
		msg.Add((DWORD)0);
		//��������
		msg.Add((BYTE)nRet);
		msg.SendToPlayer(GetExID());
		return 0;
	}
	else
	{
		CMoveShape::InitSkillInstance(dwSkillID,dwLevel,lGoodsIndex,lContainerID,lPos,GoodsGuid,4);
		return 1;
	}	

}
//
void CPlayer::SetSkillCooldown(long lID,DWORD dwTime)
{
	itSkill it = m_Skills.find(lID);
	if(it== m_Skills.end())	return;

	(*it).second.dwCooldownStartTime=timeGetTime();
	(*it).second.wCooldown = dwTime;
}

LONG CPlayer::GetNumSkills()
{
	LONG lNumSkills = 0;
	lNumSkills = (LONG)m_Skills.size();

	return lNumSkills;
}
LONG CPlayer::GetSkillLv(long lSkillID)
{
	long lSkillLv=0;
	itSkill it = m_Skills.find(lSkillID);

	if(it!=m_Skills.end())
	{
		lSkillLv =(*it).second.wLevel; 
	}
	return lSkillLv;
}


void CPlayer::CodeSkillsToDataBlock_ForClient(DBWriteSet& setWriteDB, BOOL bAddRestoreTime )
{
    setWriteDB.AddToByteArray( GetNumSkills());
	//������ȴʱ��
	setWriteDB.AddToByteArray((WORD)GlobalSetup::GetSetup()->lCommoCoolDownTime);
	tagSkill tSkill;
	itSkill it=m_Skills.begin(); 
    for (;it!=m_Skills.end();++it)
    {
        tSkill.dwID = it->second.dwID;
        tSkill.wLevel = it->second.wLevel;
        tSkill.wCooldown = it->second.wCooldown;
        DWORD dwCooldown = 0;
        if (it->second.dwOldCooldown - tSkill.wCooldown>0 && tSkill.wCooldown>0)
        {
            dwCooldown = it->second.dwOldCooldown - tSkill.wCooldown;
            it->second.dwCooldownStartTime = timeGetTime();
#ifdef _DEBUG
            char str[256];
            _snprintf(str, 256, "\n---------------------------����ID:%d,��ȴ��ʼʱ��:%d,��ȴʱ��:%d\n",tSkill.dwID ,(*it).second.dwCooldownStartTime,tSkill.wCooldown);
            OutputDebugString(str);
#endif
        }

        setWriteDB.AddToByteArray(tSkill.dwID);
        setWriteDB.AddToByteArray(tSkill.wLevel);
        setWriteDB.AddToByteArray(tSkill.wCooldown);

        if ( bAddRestoreTime && GetInst(SkillAttribute).IsExist(it->second.dwID,it->second.wLevel) )
        {
            setWriteDB.AddToByteArray((float)GetSkillValue( it->second.dwID , it->second.wLevel , "MinAtkDistance" ));
            setWriteDB.AddToByteArray((float)GetSkillValue( it->second.dwID , it->second.wLevel , "MaxAtkDistance" ));
            setWriteDB.AddToByteArray((WORD)GetSkillValue( it->second.dwID , it->second.wLevel , "SMp" ));
        }
        setWriteDB.AddToByteArray(dwCooldown);
    }
}

void CPlayer::CodeSkillsToDataBlock(DBWriteSet& setWriteDB)
{
    setWriteDB.AddToByteArray( GetNumSkills());
    
#ifdef _RUNSTACKINFO1_
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::CodeToDataBlock() m_Skills Start");
#endif
    itSkill it=m_Skills.begin();
    for(;it!=m_Skills.end();++it)
    {
        setWriteDB.AddToByteArray( static_cast<DWORD>(it->second.dwID));
        setWriteDB.AddToByteArray( static_cast<DWORD>(it->second.wLevel));

        CNewSkillProperties *pSkillProperties = CNewSkillFactory::QueryNewSkillProperty(it->second.dwID);
        if (pSkillProperties && pSkillProperties->GetCoolDown()==1)
        {
            time_t t_CurrentTime;
            t_CurrentTime = time(NULL);
            if (t_CurrentTime-it->second.tCooldownEndtime < 0)
            {
                setWriteDB.AddToByteArray( (DWORD)(it->second.tCooldownEndtime));
            }
            else
            {
                setWriteDB.AddToByteArray( (DWORD)(0));
            }

        }
        else
        {
            setWriteDB.AddToByteArray( (DWORD)(0));
        }
    }
}

void CPlayer::DecodeSkillsFromDataBlock(DBReadSet& setReadDB)
{
	LONG lNumSkills = setReadDB.GetLongFromByteArray(  );

	tagSkill tSkill;
	time_t t_CurrentTime;
	for( LONG i = 0; i < lNumSkills; i++ )
	{
		t_CurrentTime = time(NULL);
		tSkill.dwID = setReadDB.GetDwordFromByteArray();
		tSkill.wLevel = (WORD)setReadDB.GetDwordFromByteArray();


		tSkill.tCooldownEndtime =time_t(setReadDB.GetDwordFromByteArray( ));
		long lDiffTime =  (long)(tSkill.tCooldownEndtime - t_CurrentTime);

		if (lDiffTime>0)
		{
			tSkill.wCooldown = lDiffTime*1000;
		}
		else
		{
			tSkill.wCooldown = 0;
		}
		//##��Ӽ���	
		AddSkill(tSkill.dwID,tSkill.wLevel,tSkill.wCooldown,tSkill.tCooldownEndtime);	
	}
}

void CPlayer::DecodeGeniusFromDataBlock(DBReadSet& setReadDB)
{
    long  lNumGenius = setReadDB.GetLongFromByteArray();

    for ( int i = 0 ; i < lNumGenius ; i++ )
    {
        long genid = setReadDB.GetLongFromByteArray();
        long lvl   = setReadDB.GetLongFromByteArray();

        m_GeniusTable[ genid ] = lvl ;
    }
    m_GeniusAction->Update();
}

void CPlayer::CodeGeniusToDataBlock(DBWriteSet& setWriteDB)
{
    setWriteDB.AddToByteArray( (long) m_GeniusTable.size() );

    for ( std::map<long,long>::iterator itr = m_GeniusTable.begin(); itr != m_GeniusTable.end() ; itr ++ )
    {
        setWriteDB.AddToByteArray( itr->first );
        setWriteDB.AddToByteArray( itr->second);
    }
}

void CPlayer::CodeGeniusToDataBlock_ForClient(DBWriteSet& setWriteDB, BOOL bAddRestoreTime)
{
      CodeGeniusToDataBlock(setWriteDB);

      m_GeniusAction->CodeGeniusToDataBlock( setWriteDB ,bAddRestoreTime );
}

// ������
void CPlayer::CodeBrotherToDataBlock(DBWriteSet& setWriteDB)
{
    setWriteDB.AddToByteArray((long)m_mapBrotherInfo.size());
    map<CGUID, tagBrotherInfo>::iterator itor = m_mapBrotherInfo.begin();
    for(; itor != m_mapBrotherInfo.end(); ++itor)
    {
        setWriteDB.AddToByteArray((*itor).second.guidBrother);
        setWriteDB.AddToByteArray((*itor).second.lFriendVal);
    }

}

void CPlayer::DecodeBrotherFromDataBlock(DBReadSet& setReadDB)
{
    m_mapBrotherInfo.clear();
    long lBrotherNum = setReadDB.GetLongFromByteArray();
    tagBrotherInfo stBrother;
    for(long l=0; l<lBrotherNum; l++)
    {
        setReadDB.GetBufferFromByteArray(stBrother.guidBrother);
        setReadDB.GetLongFromByteArray();
        m_mapBrotherInfo[stBrother.guidBrother] = stBrother;
    }

}

void CPlayer::OnChangeStates()
{
	//##�ڷ���HP/MP/RP/YP������󣬽�HP���͸�����

	//////////////////////////////////////////////////////////////////////////
	//  [4/18/2007 chenxianjin]
	CMessage msg( MSG_S2C_SKILL_STATE_CHANGE );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( GetHP() );
	msg.Add( GetMP() );
	msg.Add( GetEnergy() );
	//	msg.Add( GetYP() );
	msg.SendToPlayer(GetExID());
	//////////////////////////////////////////////////////////////////////////

	if( GetTeamID() != NULL_GUID )
	{
		//##���͸�����
		//assert(false);
		//LONG lSessionID = GetGame() -> GetTeamSessionID( GetTeamID() );
		/*	CTeam* pTeam = dynamic_cast<CTeam*>( const_cast<CSession*>( CSessionFactory::QuerySession(lSessionID) ) );
		if( pTeam )
		{
		CPlug* pPlug = pTeam -> QueryPlugByOwner( GetType(), GetExID() );
		if( pPlug )
		{
		FLOAT fHp = static_cast<FLOAT>( GetHP() ) / static_cast<FLOAT>( GetMaxHP() );
		pPlug -> ChangeState( CTeamate::TS_CHANGE_STATE, reinterpret_cast<UCHAR*>(&fHp) );
		}
		}*/
	}
}



void CPlayer::SetSilence( long lTime )
{ 
	m_lSilenceTimeStamp	= timeGetTime() ;
	m_lSilenceTime		= lTime; 
}

bool CPlayer::IsInSilence()
{ 
	if(m_lSilenceTime == 0)	return false;

	DWORD dwCurTime = timeGetTime();
	if( (m_lSilenceTimeStamp + m_lSilenceTime) >= (long)dwCurTime)
	{
		return true;
	}
	m_lSilenceTime = 0;
	return false;
}

void CPlayer::UpdateCurrentState()
{
	//##ս��״̬�ļ���
	if( m_lFightStateCount > 0 )
	{
		-- m_lFightStateCount;
		if( m_lFightStateCount <= 0 )
		{
			//EnterPeaceState();
		}
	}
	//##����״̬�ļ���
	if( m_dwSinStateTimeStamp > 0 )
	{
		//##�ָ�һ��״̬
		if( timeGetTime() >= m_dwSinStateTimeStamp + GlobalSetup::GetSetup() -> dwCriminalTime ||
			GetPkValue() > GlobalSetup::GetSetup()->dwPkCountPerKill )
		{
			EnterResidentState();
		}
	}
}

void CPlayer::EnterCriminalState()
{
	if( GetPkValue() <= GlobalSetup::GetSetup()->dwPkCountPerKill )
	{
		if( m_dwSinStateTimeStamp == 0 )
		{
			CMessage msg( MSG_S2C_SHAPE_SINSTATE );
			msg.Add( GetExID() );
			msg.Add( static_cast<BYTE>(true) );
			msg.SendToAround( this );
		}
		m_dwSinStateTimeStamp = timeGetTime();
	}
}

void CPlayer::EnterResidentState()
{
	//##�����Ƿ�ɱ�ˣ��������ֻ�����������״̬
	m_dwSinStateTimeStamp = 0;
	CMessage msg( MSG_S2C_SHAPE_SINSTATE );
	msg.Add( GetExID() );
	msg.Add( static_cast<BYTE>(false) );
	msg.SendToAround( this );
}

//void CPlayer::EnterPeaceState()
//{
//	SetState( STATE_PEACE );
//	m_lFightStateCount = 0;
//
//	//##������Ϣ���ͻ���
//	CMessage msg( MSG_S2C_SHAPE_CHANGESTATE );
//	msg.Add( GetExID() );
//	msg.Add( static_cast<LONG>(STATE_PEACE) );
//	msg.SendToAround( this );
//}

//void CPlayer::EnterCombatState()
//{
//	SetState( STATE_FIGHT );
//	m_lFightStateCount = GlobalSetup::GetSetup() -> lFightStateTimer;
//	if(m_lFightStateTimerID!=-1)
//	{
//		TIMER_CANCEL(m_lFightStateTimerID);
//	}
//	//ע�ᶨʱ��
//	m_lFightStateTimerID = TIMER_SCHEDULE(this,NULL,timeGetTime()+m_lFightStateCount,0);
//
//	//##������Ϣ���ͻ���
//	CMessage msg( MSG_S2C_SHAPE_CHANGESTATE );
//	msg.Add( GetExID() );
//	msg.Add( static_cast<LONG>(STATE_FIGHT) );
//	msg.SendToAround( this );
//}



//##ŭ�����ӣ���ֻ����ʿ���ã�
void CPlayer::IncreaseRp( BOOL bAttack/*##��־�Ƿ��ǹ��������Ϊfalse���Ǳ�����*/, WORD wLoseHP )
{
	//
	//	if( GetOccupation() != PlayerList::OC_FIGHTER ) return;

	//##��ȡȫ������
	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
	if( !pSetup ) return;

	//##����ȼ���������ֱ���˳�
	if( GetLevel() < pSetup -> wBaseRp_Lv1 ) return;

	LONG lMaxRp = 0;
	//##�����ȡŭֵ����
	if( GetLevel() < pSetup -> wBaseRp_Lv2 )
	{
		lMaxRp = pSetup -> wBaseMaxRp_Lv1;
	}
	else 
	{
		lMaxRp = pSetup -> wBaseMaxRp_Lv2;
	}

	//##������ʱ������ŭֵ
	if( bAttack )
	{
		LONG lRp = GetEnergy();
		lRp = lRp + pSetup -> wBaseAtcRp_Sword;
		if( lRp > lMaxRp ) lRp = lMaxRp;
		SetEnergy( static_cast<WORD>(lRp) );
	}
	else 
		//##���ˣ�����ŭֵ
	{
		FLOAT	fPercent	= static_cast<FLOAT>( wLoseHP ) / GetMaxHP();
		WORD	wRpAdded	= 0;
		for( LONG i = 5; i >= 0; i -- )
		{
			if( fPercent <= pSetup -> fRpAdd_Lv1[i] )
			{
				wRpAdded = pSetup -> wRpAdd_Lv1[i];
			}
			else
			{
				break;
			}
		}
		//##����ŭֵ��Ȼ�󷵻�
		LONG lRp	= GetEnergy();
		lRp			= lRp + wRpAdded;
		if( lRp > lMaxRp ) 
		{
			lRp = lMaxRp;
		}
		SetEnergy( static_cast<WORD>(lRp) );
	}
}


//����ID�õ���Ʒ
CGoods* CPlayer::GetGoodsById(const CGUID& guid)
{


	CGoods* pGoods=NULL;
	pGoods = (CGoods*)getOriginPack()->Find(TYPE_GOODS,guid);
	if(pGoods != NULL)
		return pGoods;
	pGoods = (CGoods*)m_cEquipment.Find(TYPE_GOODS,guid);

	return pGoods;
}

//����ID�õ�������Ʒ
CGoods* CPlayer::GetGoodsById_FromPackage(const CGUID& guid)
{
	CGoods* pGoods = (CGoods*)getOriginPack()->Find(TYPE_GOODS,guid);
	if(pGoods)
		return pGoods;
	CVolumeLimitGoodsContainer* pSubPack=NULL;
	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{
			pSubPack=GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
			if(pSubPack)
			{
				pGoods=(CGoods*)pSubPack->Find(TYPE_GOODS,guid);
				if(pGoods)
					return pGoods;
			}
		}		
	}	
	return NULL;

}


void CPlayer::Disconnect( DWORD dwDelayTime )
{
	// ���û�н��뵽�����У���ôֱ���ߵ�����
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(GetFather());

	if ( pRegion==NULL || pRegion->FindChildObject(TYPE_PLAYER, GetExID()) == NULL )
	{
		GetGame()->KickPlayer(GetExID());
		return;
	}

	//##���֮ǰ�Ѿ������˶Ͽ����ӵı��,�������öϿ����.
	if( m_dwDisconnectTimeStamp ) return;
	m_dwDisconnectTimeStamp = timeGetTime();
	m_dwDisconnectDelayTime = dwDelayTime;
}

void        CPlayer::UnCancelBuddhismTimer()
{
     if ( m_Buddhism.lBudhismTimer > 0  )
     {
         TIMER_CANCEL( m_Buddhism.lBudhismTimer );
     }
}

void        CPlayer::RegisterBuddhismTimer()
{
    /// ע�ᶨʱ��
    tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
    pTimvar->TimerType = (eChildObjType)ChildObjType_Budhism;
    m_Buddhism.lBudhismTimer= TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime() + GlobalSetup::GetSetup()->lDistractTime ,GlobalSetup::GetSetup()->lDistractTime);
}

void        CPlayer::RegisterDryTimer()
{
    /// ע�ᶨʱ��
    tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
    pTimvar->TimerType = (eChildObjType)ChildObjType_ClearDry;
    m_Buddhism.lDryEndTime = timeGetTime() + GlobalSetup::GetSetup()->lDryTime;
    m_Buddhism.lDryTimer= TIMER_SCHEDULE(this,(void*)pTimvar,m_Buddhism.lDryEndTime, 0 );

}

void        CPlayer::UseSkillUpdateBuddhismDistract()
{
    /// ��������ֵ
    m_Buddhism.UpdateDistract( GlobalSetup::GetSetup()->lAddDistractValue );
    
    /// ������Ӱ�������ֵ
    m_Buddhism.UpdateDistract( GlobalSetup::GetSetup()->lDryValue * m_Buddhism.RetDryNum() );
    
    /// ����һ����
    m_Buddhism.UpdateDryNum( 1 , GlobalSetup::GetSetup()->lDryCount );

    /// Cancel m_Buddhism.lDryTimer
    if( m_Buddhism.lDryTimer > 0 )
         TIMER_CANCEL( m_Buddhism.lDryTimer );

    /// ע�ᶨʱ��
    RegisterDryTimer();

#ifdef  _DEBUG
    char  szText[256];
    _snprintf(szText,256,"[Colour 'ffffffff'] ����ֵ=%d,�������=%d,����ʱ��=%d",m_Buddhism.lDistract,m_Buddhism.lDryNum,m_Buddhism.lDryEndTime);
    SendNotifyMessage( szText, eNOTIFYPOS_LEFT_BOTTOM );
#endif

    /// �������ֵ
    AddAttributeToDirty("lDistract");
    /// ���������� 
    AddAttributeToDirty("lDryNum");
    /// ����ʱ��
    AddAttributeToDirty("lDryEndTime");

    UpdateAttribute(1);
}

CPlayer::eProgress CPlayer::GetCurrentProgress()
{
	return m_eProgress;
}

void CPlayer::SetCurrentProgress( CPlayer::eProgress ep )
{
	int nOldProgress=m_eProgress;
	m_eProgress = ep;
	GetLogicLogInterface()->logT902_player_progress_chg_log(this,nOldProgress,ep);
}

bool CPlayer::IsLockPlayerState()  
{
    if ( GetPhaseID() == NULL_GUID && GetCurrentProgress() == CPlayer::PROGRESS_FOLLOW_LEADER )
        return true;
    if ( GetPhaseID() != NULL_GUID && GetState() == CShape::STATE_AGONAL )
        return true;
    return false;
}


void CPlayer::SendNotifyMessage(const char* szText,eNOTIFY_POS ePos)
{
	if( szText )
	{
		/*CMessage msg( MSG_S2C_OTHER_ADDINFO );
		msg.Add( dwColor );
		msg.Add( dwBkColor );
		msg.Add((BYTE)ePos);
		msg.Add( szText );
		msg.SendToPlayer(GetExID(),false);*/
		/***********************************************************************/
		/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
		CMessage msg( MSG_S2C_OTHER_ADDINFO );
		//msg.Add( dwColor );
		//msg.Add( dwBkColor );
		msg.Add((BYTE)ePos);
		msg.Add( szText );
		msg.SendToPlayer(GetExID(),false);
		/***********************************************************************/
	}
}
void CPlayer::SendPlayerInfo(const char* szText, DWORD dwColor /* = 0xffffffff  */,DWORD dwBkColor)
{
	if (szText)
	{
		CMessage msg(MSG_S2C_OTHER_ADDPLAYERINFO);
		msg.Add((LONG)0);
		msg.Add(dwColor);
		msg.Add(dwBkColor);
		msg.Add(szText);
		msg.SendToPlayer(GetExID(),false);

	}	
}

void CPlayer::SendSystemInfo(const char* szText, DWORD dwColor )
{
	if( szText )
	{
		CMessage msg( MSG_S2C_OTHER_ADDSYSTEMINFO );
		msg.Add( dwColor );
		msg.Add( szText );
		msg.SendToPlayer(GetExID());
	}
}


void CPlayer::SendOtherInfo(const char* szText )
{
	if( szText )
	{
		CMessage msg(MSG_S2C_OTHER_TALK_NOTIFY);
		msg.Add(BYTE(0));
		msg.Add(szText);
		msg.SendToPlayer(GetExID(),false);
	}
}
BOOL CPlayer::DelSkill( long lSkillID,long lSkillLv )
{	
	itSkill it = m_Skills.find(lSkillID);
	if (it!=m_Skills.end())
	{
		m_Skills.erase(it);		
	}
	else
	{
		return FALSE;
	}
	EndPassiveSkill(lSkillID,lSkillLv);

	char tmp[32];
	for(int i=0; i<24; ++i)
	{
		_snprintf(tmp, 32, "dwHotKey%d", i);
		DWORD dwHotKey = GetAttribute((string)tmp);
		if (dwHotKey!=0)
		{
			if ((dwHotKey&0x80000000)!=0)	// ��Ʒ
			{
				DWORD dwSkillID = dwHotKey & 0x7fffffff;
				if (dwSkillID==lSkillID)
				{
					SetAttribute((string)tmp,0);
					UpdateAttribute();
					//��ͻ��˷��������ݼ���Ϣ
					CMessage msg(MSG_S2C_SETHOTKEY);
					msg.Add((char)HOTKEY_CLEAR_SUCCEED);
					msg.Add((BYTE)i);
					msg.Add((DWORD)0);
					msg.SendToPlayer(GetExID());
				}
			}				
		}
	}

	CMessage msg(MSG_S2C_PLAYER_DELSKILL);
	msg.Add((DWORD)lSkillID);
	msg.SendToPlayer(GetExID());

    /// ɾ�����ܵ�ʱ������츳���ñ�
    m_GeniusAction->Remove( Skill_Remove , lSkillID , 1);
	return TRUE;
}

long  CPlayer::GetGeniusLvl(long Geniusid)
{
    std::map<long,long>::iterator itr = m_GeniusTable.find( Geniusid );
    if ( itr != m_GeniusTable.end() )
    {
        return itr->second;
    }
    return -1;
}

/// �Ƴ��츳
void  CPlayer::RemoveGenius(long Geniusid)
{
    std::map<long,long>::iterator itr = m_GeniusTable.find( Geniusid );
    if ( itr != m_GeniusTable.end() )
    {
        long   lvl = itr->second ;
        m_GeniusTable.erase( itr );
        m_GeniusAction->Remove( Genius_Remove , Geniusid , lvl );
    }
}

void   CPlayer::StudyGenius(long Geniusid,long lvl)
{
    std::map<long,long>::iterator itr = m_GeniusTable.find( Geniusid );
    if ( itr != m_GeniusTable.end() )
    {
        itr->second = lvl;
        m_GeniusAction->Update(Genius_Update,Geniusid,lvl);
        return;
    }
    m_GeniusAction->AddGenius(Geniusid,lvl);
}

void CPlayer::IncreaseContinuousKill()
{
	//##��ȡȫ�ֵ�����
	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
	if( pSetup == NULL ) return;

	//##������趨ʱ����ɱ������
	if(  (long)(timeGetTime() - m_dwContinuousKillTimeStamp) < pSetup -> lHitTime )
	{
		++ m_dwContinuousKillAmount;
		//##�����ȼ��ļ��
		//if( m_dwContinuousKillAmount > GetHitTopLog() )
		//{
		//	vector<HitLevelSetup::tagHitLevel>::iterator iter = 
		//		HitLevelSetup::GetHitLevelVec() -> begin();
		//	for( ; iter != HitLevelSetup::GetHitLevelVec() -> end(); iter ++ )
		//	{
		//		if( m_dwContinuousKillAmount == iter -> dwHit )
		//		{
		//			//##�����µ������ȼ����;���
		//			SetHitTopLog( static_cast<WORD>(iter -> dwHit) );
		//			//SetExp( GetExp() + iter -> dwExp );
		//			IncExp(iter -> dwExp );

		//			//##����Ƿ�����
		//			CheckLevel();

		//			//##��������������Ϣ
		//			CMessage msg( MSG_S2C_PLAYER_HITLEVELUPGRADE );
		//			msg.Add( GetHitTopLog() );
		//			msg.SendToPlayer(GetExID());
		//			break;
		//		}
		//	}
		//}
	}
	else
	{
		m_dwContinuousKillAmount = 0;
	}

	m_dwContinuousKillTimeStamp = timeGetTime();

	//##�������������ͻ���
	CMessage msg( MSG_S2C_PLAYER_SYNC_HITS );
	msg.Add( m_dwContinuousKillAmount );
	msg.SendToPlayer(GetExID());
}

//���һ���ƶ���Ϣ
void CPlayer::AddMoveMsg(float fNewX,float fNewY)
{
	float fXOffset = fNewX - GetPosX();
	float fYOffset = fNewY - GetPosY();
	float fDis = sqrt( fYOffset * fYOffset + fXOffset * fXOffset );

	m_fMoveValiTotalDistance += fDis;
	//����ܹ��յ�һ��������Ϣ�Ժ󣬽�����֤��
	++m_fMoveValiRecvMsgCount;

	DWORD dwCurTime = timeGetTime();
	if( (dwCurTime-m_dwLastRecvMsgTime) == 0)	return;

	float fMoveSpeed = fDis/(float)(dwCurTime-m_dwLastRecvMsgTime);
	if(fMoveSpeed > m_fMaxPeriodMoveSpeed)
		m_fMaxPeriodMoveSpeed = fMoveSpeed;
		
		

//#ifdef _DEBUG	
//	char pszStackInfo1[256]="";
//	_snprintf(pszStackInfo1,256,"�ܾ���:%f,����:%f,m_fMaxPeriodMoveSpeed:%f,fMoveSpeed:%f,ʱ��:%d,��־:%d \n",m_fMoveValiTotalDistance,fDis,m_fMaxPeriodMoveSpeed,fMoveSpeed,dwCurTime-m_dwLastRecvMsgTime,lFlag);
//	OutputDebugString(pszStackInfo1);
//#endif
	m_dwLastRecvMsgTime = dwCurTime;
}
//�ƶ���֤,�����ƶ��Ƿ�����
bool CPlayer::CheckMoveSpeedValidate()
{
	//Ϊ����֤����Ч��,ʱ����ڵ��������֤
	float fTimeDif = (float)(timeGetTime()-m_dwMoveValiStartTime);

	//float fTotalMoveDis = (float)dwTimeDif*m_fMoveValiStartSpeed;
	//����Ӧ�ý�����Ϣ�ĸ���
	float fTotalMsgCount = GlobalSetup::GetSetup()->fMaxMsgCountPerSecond*(fTimeDif/1000.00f+0.5f);
	if(fTotalMsgCount < m_fMoveValiRecvMsgCount )
	{
		char pszGuid[256]="";
		GetExID().tostring(pszGuid);
		Log4c::Trace(ROOT_MODULE,"(PlayerID:%s)Move Msg Count Error,fTimeDif:%f,MoveValiRecvMsgCount:%f,MaxRecvMsgCount:%f",pszGuid,fTimeDif,m_fMoveValiRecvMsgCount,fTotalMsgCount);
		//�����������
		GetGame()->KickPlayer(GetExID());
		return false;
	}
	//�������
	float fAverSpeed = m_fMoveValiStartSpeed;
	if(fTimeDif != 0.0f)
		fAverSpeed = m_fMoveValiTotalDistance/fTimeDif;
	if( (fAverSpeed-m_fMoveValiStartSpeed) >= 0.0005f &&
		(m_fMaxPeriodMoveSpeed-m_fMoveValiStartSpeed) > 0.0008)
	{
		if(m_guMoveValiStartRgnID != GetRegionGUID())
		{
			char pszGuid[256]="";
			GetExID().tostring(pszGuid);
			Log4c::Warn(ROOT_MODULE, FormatText("GS_PLAYER_0",pszGuid));//Move Region Error,Kick Player PlayerID[%s].
			GetGame()->KickPlayer(GetExID());	
		}
		else if( (fAverSpeed-m_fMoveValiStartSpeed) > GlobalSetup::GetSetup()->fMaxMistakeAverSpeed ||
			(m_fMaxPeriodMoveSpeed-m_fMoveValiStartSpeed) > GlobalSetup::GetSetup()->fMaxMistakeMaxSpeed )
		{
			char pszGuid[256]="";
			GetExID().tostring(pszGuid);
			Log4c::Trace(ROOT_MODULE,"PlayerID[%s] Move Speed Error,TimeDif:%f,RealMoveDis:%f,AverSpeed:%f,MaxPeriodMoveSpeed:%f,MoveValiStartSpeed:%f",
				pszGuid,fTimeDif,m_fMoveValiTotalDistance,fAverSpeed,m_fMaxPeriodMoveSpeed,m_fMoveValiStartSpeed);		
			GetGame()->KickPlayer(GetExID());	
		}
		else
			SetPosition(m_lMoveValiStartPosX,m_lMoveValiStartPosY);
			return false;
		}

	ResetMoveValiData();	
	return true;
}

//�����������
void CPlayer::ResetMoveValiData()
{
	m_dwMoveValiStartTime=timeGetTime();
	//m_dwLastRecvMsgTime = m_dwMoveValiStartTime;
	m_fMaxPeriodMoveSpeed = 0.0f;
	m_guMoveValiStartRgnID = GetRegionGUID();
	m_lMoveValiStartPosX=GetTileX();//��ʼ�ƶ���Xλ��
	m_lMoveValiStartPosY=GetTileY();//��ʼ�ƶ���Yλ��
	m_fMoveValiStartSpeed = GetSpeed();
	m_fMoveValiTotalDistance=0.0f;//�ƶ����ܾ���
	m_fMoveValiRecvMsgCount=0;//�յ����ƶ���Ϣ����
}

//��֤ÿһ�����ƶ�
bool CPlayer::CheckMoveStep(float fCurX,float fCurY)
{
	CServerRegion *pRegion = (CServerRegion*)GetFather();
	if(NULL == pRegion || pRegion->GetType() != TYPE_REGION)
		return false;
	long lSX = GetTileX();
	long lSY = GetTileY();
	int nMaxDis = max((abs(lSX-(long)fCurX)),(abs(lSY-(long)fCurY)));
	if(nMaxDis > 2)
		return false;
	//long lDir = GetLineDir(lSX,lSY,fCurX,fCurY);
	//�ж��赲
	BYTE byBlock = pRegion->GetBlock((int)fCurX,(int)fCurY);	
	if(byBlock != CRegion::BLOCK_NO && byBlock != CRegion::BLOCK_AIM)
	{
		return false;
	}
	/*nMaxDis--;
	while(nMaxDis>0)
	{
	long lDX = lSX + nMaxDis*_dir[lDir][0];
	long lDY = lSY + nMaxDis*_dir[lDir][1];
	byBlock = pRegion->GetBlock(lDX,lDY);	
	if(byBlock != CRegion::BLOCK_NO && byBlock != CRegion::BLOCK_AIM)
	{
	return false;
	}
	nMaxDis--;
	}*/


	/*CServerRegion *pRegion = (CServerRegion*)GetFather();
	if(NULL == pRegion || pRegion->GetType() != TYPE_REGION)
	return false;
	float fSX = GetPosX();
	float fSY = GetPosY();
	float fDistance = Distance(fSX,fSY,fCurX,fCurY);
	if(0.0f == fDistance)	return true;
	float fRadianDir = GetRadianDir(fSX,fSY,fCurX,fCurY);	
	float fcos= cosf(fRadianDir);
	float fsin= sinf(fRadianDir);

	float fCount = 0.0f;
	int nBlockNum = 0;
	while(fCount <= fDistance)
	{
	float fx = fSX + fCount * fcos;			
	float fy = fSY + fCount * fsin;
	BYTE byBlock = pRegion->GetBlock(fx,fy);
	if(byBlock != CRegion::BLOCK_NO && byBlock != CRegion::BLOCK_AIM)
	nBlockNum++;
	else
	nBlockNum = 0;
	if(nBlockNum >= 2)
	break;
	fCount += 0.5f;
	}
	if(nBlockNum >= 2)
	return false;*/
	return true;
}
// ������������
void CPlayer::OnQuestMoveStep(float fCurX,float fCurY,float fDestX,float fDestY)
{
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestMoveStep() Start.");
#endif

	if (!CanMove())
	{
		OnCannotMove();
		return;
	}

	if (!GetInstruction())
	{
		//����ʹ�����ʹ��ָ��ʱֱ����
		return;
	}

	if(!CheckMoveStep(fCurX,fCurY) )
	{
		OnCannotMove();
		return;
	}
	StateAI::BaseAI* pAI = GetAI();
	if( NULL == pAI )
	{
		OnCannotMove();
		return;
	}
	//���һ����֤��Ϣ����
	AddMoveMsg(fCurX,fCurY);

	long lDir = GetLineDir(GetTileX(),GetTileY(),(int)fCurX,(int)fCurY);
	SetDir(lDir);
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestMoveStep() SetPosXY.");
#endif
	SetPosXY(fCurX,fCurY);
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestMoveStep() MoveTo.");
#endif
	pAI->OnMove( fDestX,fDestY);		

	if(m_fMoveValiRecvMsgCount > MoveValiMaxMsgCount)
	{
		CheckMoveSpeedValidate();
	}
	//char strDebug[256];
	//_snprintf( strDebug, "Client move to: %d, %d \n", GetTileX(), GetTileY() );
	//OutputDebugString( strDebug );
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestMoveStep() End.");
#endif

}

void CPlayer::OnQuestStopMove(float fCurX,float fCurY,short lDir)
{
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestStopMove() CheckMoveStep.");
#endif
	if(!CheckMoveStep(fCurX,fCurY) )
	{
		OnCannotMove();
		return;
	}
	//���һ����֤��Ϣ����
	AddMoveMsg(fCurX,fCurY);

	SetDir(lDir);
	SetPosXY(fCurX,fCurY);	
	OnStopMove();

	CheckMoveSpeedValidate();

#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestStopMove() CheckSwitchPoint.");
#endif
	//����л���
	CheckSwitchPoint();

#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnQuestStopMove() end.");
#endif
}


//�Ƿ����ƶ�
bool  CPlayer::CanMove()
{
	/*if(m_lCityWarDiedState == true)
	return false;*/
	return CMoveShape::CanMove() > 0 ;
}

void CPlayer::OnCannotMove()
{
	CMessage msg( MSG_S2C_SHAPE_SETPOS );
	msg.Add( GetType() );
	msg.Add( GetExID());
	msg.Add( (GetPosX()) );
	msg.Add( (GetPosY()) );
	msg.Add( static_cast<LONG>(0) );
	msg.SendToPlayer(GetExID());
}

void CPlayer::OnExitRegion()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		// ����Ǹ������鸱����ǰ���� == 0,����ɾ��������ʱ��
		if((eRgnType)pRegion->GetRgnType() != RGN_NORMAL && (eRgnType)pRegion->GetRgnType() != RGN_PERSONAL_HOUSE)
		{
			// ����--
			//			pRegion->SetCurDupRgnPlayerNum(pRegion->GetCurDupRgnPlayerNum()-1);
			if(pRegion->GetPlayerAmout() == 1)
			{
				//	pRegion->SetRgnDelTimerID(GameManager::GetInstance()->GetRgnManager()->GetRegionTimer()->Schedule(pRegion, NULL, timeGetTime()+GetGame()->GetSetup()->lPDupRgnDelTime));
				pRegion->RegisterDeleteRgnTimerID(GlobalSetup::GetSetup()->lPDupRgnDelTime);
			}
		}
		//ִ���˳������ű�
		const char* pszExitRegionSctipt = CRegionSetup::GetExitRegionScript(pRegion->GetID());
		PlayerRunScript((char*)pszExitRegionSctipt);
	}

	DropParticularGoodsWhenRecall();
	ProduceEvent(ET_ExitRegion);
	ProduceEvent(ET_EventBreak);

	//���������ʾ
	ClearAllHint();
	//����ű���ʱ��
	ClearScriptTimer();

}


//����ҵȼ������仯��ʱ�򴥷�����Ҫָ��������
void CPlayer::OnLevelChanged()
{
	//����ս�����ԵĻ������Ե�����
	ResetBaseFightProperty();
	//��������ս������
	ResetFightProperty();
	//����ʱ�����ű�
	const char* strScript = GlobalSetup::GetSetup()->pszPlayerUpLevelScript;
	if(strScript)
	{
		//stRunScript sScript;
		//sScript.pszFileName = (char*)strScript;
		//sScript.desShape = this;
		//sScript.pRegion =  dynamic_cast<CRegion*>(GetFather());
		//sScript.srcShape	= this;
		//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
		Script::RunEnv env( this, this, static_cast<CRegion*>( GetFather() ) );
		GetInst( ScriptSys ).RunScript( env, strScript );
	}
	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxHp);
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxMp);
	}
	SetHP(GetMaxHP());
	SetMP(GetMaxMP());
	SetPKCP( GetMaxPKCP() ); // ����PK CPֵ

	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrHp);
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrMp);	
	}



	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	//AddPetsLev();

	if( GetLevel() == GlobalSetup::GetSetup()->m_btNewbieLevelLimit + 1 )
	{
		SendNotifyMessage( "�����ڿ�ʼ����ĵȼ��Ѿ��������ֱ�������涨�ĵȼ��������󽫲����ܵ�������" );
	}
	else if( GetLevel() == GlobalSetup::GetSetup()->m_btNewSoldierLevel + 1 )
	{
		SendNotifyMessage( "�����ڿ�ʼ����ĵȼ��Ѿ������±���������涨�ĵȼ���" );
	}
}

//�����ְҵ�ȼ��������仯��ʱ����Ҫְָҵ�ȼ����ӵ�ʱ��
void CPlayer::OnOccuLevelChanged()
{
	//����ս�����ԵĻ������Ե�����
	ResetBaseFightProperty();
	//UpdateAttribute(1);
	//��������ս������
	ResetFightProperty();
	//UpdateAttribute(1);
}


void CPlayer::OnEnterRegion(bool bIsRelive)
{
	CMoveShape::OnEnterRegion(bIsRelive);

	AddNewState(this,STATE_BASE_GOD,1,18);
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
        //֪ͨchat server������л�����
        CMessage csmsg(MSG_S2CS_PLAYER_CHANGE_REGION);
        csmsg.Add(GetExID());
        csmsg.Add(pRegion->GetID());
        csmsg.SendToChatServer();

		// ��������״̬
		if(pRegion->GetRgnHideFlag() == 2)
		{
			SetHideFlag(0);
		}
		else if(pRegion->GetRgnHideFlag() == 0 && GetCanHide() == 0)
		{
			SetHideFlag(0);
		}
		else if( pRegion->GetRgnHideFlag() != 2 && (pRegion->GetRgnHideFlag() == 1 || GetCanHide() == 1) )
		{
			SetHideFlag(1);
		}

		// ���þݵ�ս����PK����
		if(pRegion->GetStrongPointFlag() == true) // �����ݵ�ս����
		{
			SetPk_Camp(true);
			SetPk_Country(true);
			SetPk_Team(true);
			SetPk_Union(true);
			SetPk_Badman(false);
			SetPk_Pirate(false);
			SetPk_Normal(false);
		}

		ProduceEvent(ET_EnterRegion);
		SetCurRgnType((eRgnType)pRegion->GetRgnType());
		//ִ�н���ó����Ľű�
		const char* pszEnterRegionSctipt = CRegionSetup::GetEnterRegionScript(pRegion->GetID());
		PlayerRunScript((char*)pszEnterRegionSctipt);

		//����GM���ж�
		if(!IsGM())
		{
			//! �������
			GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
			if(NULL != pTeam) // ����ڶ�����
			{
				pTeam->MemberMapChange(GetExID());

				if(pRegion->GetRgnType() == RGN_TEAM)
				{
					// �ж϶ӳ��ĸ���RgnID,�Ƿ�����
					if(this->GetExID() != pTeam->GetMasterID())
					{
						if( pRegion->GetExID() != GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID(pTeam->GetMasterID(), pRegion->GetID()) )
						{
							RegisterRgnKickPlayerTimerID(GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
							CMessage msg(MSG_S2C_OTHER_TIME_INFO_BOX);
							msg.Add((BYTE)RGN_CLEARDUPRGNPLAYER);
							msg.Add((long)GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
							msg.SendToPlayer(GetExID());
							return;
						}
					}
				}
				if(pRegion->GetRgnType() == RGN_NORMAL && pRegion->GetStrongPointFlag() == true) // �ǿ����ݵ����ͨ����
				{
					// ȡ�����
					CMessage msg(MSG_S2W_TEAM_KICK_PLAYER);
					msg.Add(GetExID());
					msg.Add(GetExID());
					msg.Send(false);
				}
			}
			else
			{
				// �ж��Լ��ĸ���RgnID,�Ƿ�����
				if( pRegion->GetRgnType() == RGN_TEAM && (GameManager::GetInstance()->GetRgnManager()->GetTeamRgnGUID(GetExID(), pRegion->GetID()) != pRegion->GetExID()) )
				{
					RegisterRgnKickPlayerTimerID(GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
					CMessage msg(MSG_S2C_OTHER_TIME_INFO_BOX);
					msg.Add((BYTE)RGN_CLEARDUPRGNPLAYER);
					msg.Add((long)GlobalSetup::GetSetup()->lRgnKickPlayerDeltaTime);
					msg.SendToPlayer(GetExID());
				}
			}
		}
	}
}



//������GS����,������һ�ν���GS�������Ժ���л�������
bool CPlayer::OnEnter()
{
#ifdef _RUNSTACKINFO_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPlayer::OnEnter(name:%s,RegionID:%d,x:%d,y:%d) Start",GetName(),GetRegionID(),GetTileX(),GetTileY());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	string strRegionName = "δ֪����";
	string strOccu = "Occu";

	CGUID tGUID;
	CServerRegion* pRegion = NULL;
	char strRegionGUID[50]="";
	GetRegionGUID().tostring(strRegionGUID);

	pRegion = GameManager::GetInstance()->GetRgnManager()->FindRgnByRgnGUID(GetRegionGUID());
	long lTaxRate = 0;
	eRWT lWarRegionType = RWT_Normal;
	long lResourceID = 0;
	float fExpScale = 1.0;
	if( pRegion )
	{
		lTaxRate = pRegion->GetTaxRate();
		lWarRegionType = pRegion->GetWarType();
		lResourceID = pRegion->GetResourceID();
		fExpScale = pRegion->GetExpScale();
		long x, y;

		//�õ���ǰHP��MP,��װ����״̬��ʼ��֮��,��������������ֵ
		DWORD dwHp = GetHP();
		WORD wMp = GetMP();
#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SetPosXY Start");
#endif
		// ���������
		if( GetTileX() == 0 && GetTileY() == 0 )
		{
			pRegion->GetRandomPos(x, y);
			CShape::SetPosXY(x+0.5f,y+0.5f);
		}

		//��������ĵط����赲,��ô����Χ��Ѱ�ո���
		if (pRegion->GetBlock(GetTileX(), GetTileY()) != CRegion::BLOCK_NO)
		{
			pRegion->GetRandomPosInRange(x, y, GetTileX(), GetTileY(), 3,3);
			CShape::SetPosXY(x+0.5f,y+0.5f);
		}

		//���ó�ս���ߵ���ȷλ�� 
		pRegion->SetEnterPosXY(this);

		SetFather(pRegion);
		strRegionName = pRegion->GetName();	

#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"InitProperty Start");
#endif
		InitProperty();
		//InitSkills();
		MountAllEquip();

#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"ActivePets Start");
#endif

#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"Send MSG_S2C_LOG_ANSWER_PLAYERDATA Start");
#endif
		CMessage msg(MSG_S2C_LOG_ANSWER_PLAYERDATA);
		msg.Add(GetSignCode());
		msg.Add(1L);
		msg.Add(GetExID());
		// ȫ�ֲ���
		msg.AddEx(GlobalSetup::GetSetup(), sizeof(float)*33);
		msg.Add(GlobalSetup::GetSetup()->lHitTime);
		msg.Add(GlobalSetup::GetSetup()->wBaseRp_Lv1);
		msg.Add(GlobalSetup::GetSetup()->wBaseRp_Lv2);
		msg.Add((WORD)GlobalSetup::GetSetup()->dwPkCountPerKill);
		msg.Add((float)(GlobalSetup::GetSetup()->fBasePriceRate));
		msg.Add((float)(GlobalSetup::GetSetup()->fTradeInRate));
		msg.Add((float)(GlobalSetup::GetSetup()->fRepairFactor));

		msg.Add(GlobalSetup::GetSetup()->szTalkCountryGoodsName);
		msg.Add((long)GlobalSetup::GetSetup()->nTalkCountryGoodsNum);
		msg.Add(GlobalSetup::GetSetup()->lTalkCountryGold);
		//msg.Add(GlobalSetup::GetSetup()->lTalkCountrySilver);

		msg.Add(GlobalSetup::GetSetup()->szTalkWorldGoodsName);
		msg.Add((long)GlobalSetup::GetSetup()->nTalkWorldGoodsNum);
		msg.Add(GlobalSetup::GetSetup()->lTalkWorldGold);

		ContributeSetup::tagContributePara* ContrbutePara = ContributeSetup::GetContributeParaList();
		msg.Add((BYTE)ContrbutePara->lCombatLevel1);
		msg.Add((BYTE)ContrbutePara->lCombatLevel2);

		msg.Add(AREA_WIDTH);
		msg.Add(AREA_HEIGHT);
		AddFightPropertyTODirty();
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		CodeToDataBlock_ForClient(setWriteDB);
		msg.Add(const_cast<char*>(strRegionName.c_str()));
		msg.Add(lTaxRate);
		msg.Add((BYTE)lWarRegionType);
		msg.Add((BYTE)pRegion->GetSpaceType());
		msg.Add((BYTE)pRegion->IsChangeEquip());
		msg.Add(pRegion->GetExID());
		msg.Add(pRegion->GetID());
		msg.Add(lResourceID);
		msg.Add(fExpScale);
		msg.Add((BYTE)(pRegion)->IsNoPk());
		GameManager::GetInstance()->GetRgnManager()->AddLinedRgnInfoToByteArray(pRegion->GetLinedIdFlag(), setWriteDB);
		//���ͷ�����ʱ�� by tanglei
		time_t t = time(NULL);
		msg.Add((DWORD)t);//end
		// ���͵�ǰ�����ľݵ���Ϣ
		msg.Add((BYTE)pRegion->GetStrongPointFlag()); // �ݵ�ս�Ƿ���
		if(pRegion->GetStrongPointFlag() == false) // δ�����ݵ�ս
		{
			msg.Add((long)0); // �ݵ����
		}
		else
		{
			msg.Add((long)pRegion->GetStronPointNum()); // �ݵ����
			std::vector<CServerRegion::tagStrongPoint>::iterator spItr = pRegion->GetStrongPointVec().begin();
			for(; spItr != pRegion->GetStrongPointVec().end(); spItr++)
			{
				msg.Add((*spItr).szName);//�ݵ�����
				msg.Add((long)(*spItr).lPosX); //�ݵ�X����
				msg.Add((long)(*spItr).lPosY); //�ݵ�Y����
				msg.Add((long)(*spItr).lState); //�ݵ�״̬
			}
		}
#ifdef _DEBUG
        ulong lTotalSize = msg.GetSize();
        char szOut[128];
        sprintf(szOut, "the player %s msg totalsize %u.\n", GetName(),lTotalSize);
        OutputDebugString(szOut);
#endif
		msg.SendToPlayer(GetExID(),false);


        /// ��Ҫ������ϸָ����ͻ���������
        if ( GetPhaseID() != NULL_GUID )
        {
            PHASEMAN.OnPhaseOper( GetPhaseID() , this , eReJoin );
        }

#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"pRegion->PlayerEnter() Start");
#endif
		//������ҽ��볡��
		pRegion->PlayerEnter(this,0);
		// ������볡��
		if( GetActivePetGUID() != NULL_GUID && !IsDied() )
		{
			ActivatePet();
		}

#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"StartAllBuff() Start");
#endif
		StartAllBuff();

		StartUpPassiveSkill();

		ProduceEvent(ET_EnterRegion);

		RegisterPKValue();
		RegisterPvPValue();
		RegisterAutoReNewHpMp();
		//RegisterEnergyUpdate(PlayerList::GetOccuValueRatio("baseenergyrecovertime",GetAttribute(strOccu)));
		//��ʼ��ٸ����������
		IniMercQuest();
		//����ϵͳ�ʼ�
		//GameManager::GetInstance()->GetMailManager()->SendSysMailToReciver(this);
#ifdef _RUNSTACKINFO3_
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"SendSysMailMsgToReciver() Start");
#endif
		GameManager::GetInstance()->GetMailManager()->SendSysMailMsgToReciver(this);
		//����ϵͳ�ʼ�
		GameManager::GetInstance()->GetMailManager()->SendSysMailToReciver(this);

		//�ж��Ƿ��һ�ε�¼������ҳ�����½�ű�
		if(GetFirstLogin())
		{
			SetFirstLogin(false);
#ifdef _RUNSTACKINFO3_
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"OnEnterGame() Start");
#endif
			OnEnterGame();
		}

		// �ж�ʱ���и�������,��������������ʱ��
		if(GetCurPRgnNum() > 0)
			RegisterCreatePRgnTimerID(GlobalSetup::GetSetup()->lCreateDupRgnDeltaTime);
		if(GetCurTRgnNum() > 0)
			RegisterCreateTRgnTimerID(GlobalSetup::GetSetup()->lCreateDupRgnDeltaTime);

		//����HP��MP
		if(GetState() != STATE_DIED && dwHp == 0)
		{
			Log4c::Warn(ROOT_MODULE, FormatText("GS_PLAYER_1",GetName(),GetState(),dwHp));//���״̬����(Name:%s,State:%d,Hp:%d)
			SetState(STATE_PEACE);
			dwHp = GetMaxHP()/2;
		}
		SetHP(dwHp);AddAttributeToDirty("Hp");
		SetMP(wMp);AddAttributeToDirty("Mp");
		//��ʾ������Ϸ
		SetEnterGame(true);
		UpdateAttribute(1);

		/// ��ӷ����ģ��,��ҽ�����Ϸ������ʱ�� @Expter20090602
		if (!GetInst(AntiCheat::CPlayerManager).PlayerEnter(this))
		{
			return false;
		}
        RegisterBuddhismTimer();
	}
	else
	{
		CMessage retmsg(MSG_S2C_LOG_ANSWER_PLAYERDATA);
		retmsg.Add(GetSignCode());
		retmsg.Add(0L);
		retmsg.Add(GetExID());
		retmsg.SendToPlayer(GetExID(), false);
		Log4c::Trace(ROOT_MODULE,"��ҵ�½,���뵽GS,δ�ҵ�����Name:%s,RegionID:%d,regionguid:%s,PosX:%d,PosY:%d",
			GetName(),GetRegionID(),strRegionGUID,GetTileX(),GetTileY());
		return false;
	}
#ifdef _RUNSTACKINFO_
	_snprintf(pszStatckInfo,1024,"CPlayer::OnEnter(name:%s) End",GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	return true;
}
//������������
void CPlayer::StartUpPassiveSkill()
{
	map<long,tagSkill>::iterator it = m_Skills.begin();
	for(;it!=m_Skills.end();++it)
	{
		OnBeginSkill(it->second.dwID,it->second.wLevel);
	}
}

//ʹ����Ʒ��������
bool CPlayer::ItemBeginSkill(DWORD dwID, DWORD dwLevel, CMoveShape *pDestShape)
{

	if(!AddActiveSkill(dwID,dwLevel))
	{
		return false;
	}

	if (NULL == m_pInstanceSkill || !GetAI()->BeginSkill(m_pInstanceSkill,pDestShape->GetTileX(),pDestShape->GetTileY(),pDestShape))
	{
		return false;
	}
	else
	{
		return true;
	}	
}
//��������Ϸʱ����,��һ�ε�½��ʱ�򴥷����Ժ��л����������ᴥ��
void CPlayer::OnEnterGame(void)
{
	SetPlayerLogin(true);
	const char* strScript =GlobalSetup::GetSetup()->pszPlayerLoginScript;
	if(strScript)
	{
		PlayerRunScript((char*)strScript);
	}
	//��ʼ�����;���
	InitPresentExp();

	ProduceEvent(ET_EnterGame);

    // validate player for chat server
    SendValidateMsgForCs();
}

//���ѡ���˳���Ϸʱ�˳�GS�����������л������������˳�GSҲ����
void CPlayer::OnExit( BOOL bOnChangeServer )
{
#ifdef _RUNSTACKINFO_
	char pszStatckInfo[1024]="";
	char pszGUID[50]="";
	m_guid.tostring(pszGUID);
	_snprintf(pszStatckInfo,1024,"CPlayer::OnExit(PlayerID:%s) Start",pszGUID);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	// ���ʱ��Ч��
	GetGame()->RemoveValidateTime(GetExID());

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 1");
#endif
	//������еĽű�
	//DelectAllScripByShape(GetExID());
	GetInst( ScriptSys ).PushDelScript( this );

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 2");
#endif
	// �������߱ر�����Ʒ
	DropParticularGoodsWhenLost();

	//�ͷ�ٸ���������� 
	//ReleaseMercQuest();

	/**## ���ԵĴ��� */
	if( IsInSilence() )
	{
		m_lSilenceTime = ( m_lSilenceTimeStamp + m_lSilenceTime ) - timeGetTime();
	}

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 3");
#endif
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(GetFather());
	// ����˳���������Ϣ
	/*CMessage Msg( MSG_S2C_RGN_DELSHAPE );
	Msg.Add( GetType() );
	Msg.Add( GetExID() );
	Msg.Add( (long)0 );
	Msg.Add( GetTileX() );
	Msg.Add( GetTileY() );
	Msg.SendToAround( this, this );*/
	if(pRegion)
	{
		// �������ʱ��֪ͨ��Χ������Shape���󣨰�����Ҽ���������
		pRegion->DelShapeToAround(this);
		CPet* activePet = GetActivePet();
		if (activePet!=NULL)
		{
			pRegion->DelShapeToAround(activePet);
		}
	}

	////ȡ���ڹ���,�ݵ�ս�е�����״̬
	//if(GetIsContendState() == true && pRegion)
	//{
	//	pRegion->CancelContendByPlayerID(this);
	//}

	// �������л�����������,���뿪��Ϸ
	if( bOnChangeServer == FALSE )
	{
		//�����˳���Ϸ�¼��¼�
		ProduceEvent(ET_ExitGame);		
		ProduceEvent(ET_EventBreak);
	}

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 4");
#endif
	//���״̬
	//ClearAllBuff();	
	//EndBuffAndDebuff();
	//�������
	ClearSkills();

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 5");
#endif
	//�����ɫ����
	if( IsDied() || (pRegion && pRegion -> DoesRecallWhenLost(this)) )
	{
		if( IsDied() )
		{
			// 2009.01.07 ahc �޸� �����سǵ��ж�
			//SetHP(GetMaxHP()/2);	//������HPΪ��Ѫ��һ��
			SetAction(CShape::ACT_STAND);
			SetPos(0);
		}
	}

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 6");
#endif
	//�� ɾ����Ʒ��չ�����Ự
	GoodsExManage::GetInstance().CloseGoodsExWindow(this);

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 7");
#endif
	//! ��ϵ��ϵͳ��Ӧ
	LinkmanSystem::GetInstance().OnPlayerExit(GetExID());
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 8");
#endif
	//�ͷ��ʼ�
	GameManager::GetInstance()->GetMailManager()->ReleaseMail(GetReceiveMailList());

	// ����ң�������˳���Ϸ����ҪΪ�����������Ļ�Ծ�ȣ����Ҽ���Ƿ��оܾ�������Ϊ
	if( !bOnChangeServer )
	{
		const AntiCheat::Range &r = GetInst( AntiCheat::CConfiger ).GetSystem().QuitActivityRange;
		GetInst( AntiCheat::CPlayerManager ).PlayerChangeActivity( GetExID(), 
			random( r.second - r.first + 1 ) + r.first, false );
		GetInst( AntiCheat::CPlayerManager ).CheckRefuseAnswer( GetExID(), false );
	}

    /// �뿪��λ�����뿪����.
    if( GetPhaseID() != NULL_GUID )
        PHASEMAN.OnPhaseOper( GetPhaseID(),this,eLeave );

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 9");
#endif
	// �˳�����
	OnExitGame(bOnChangeServer);

	/// ��ӷ����ģ��,�����ҵ�ǰ��Ϣ @Expter20090602
	//GetInst(AntiCheat::CPlayerManager).PlayerExit(this, bOnChangeServer == TRUE );

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 10");
#endif


    UnCancelBuddhismTimer();
	UnRegisterAutoReNewHpMp();
	UnRegisterPKValue();
	UnRegisterPvPValue();
	//UnRegisterEnergyUpdate();
	UnRegisterReliveTimerID();

#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "OnExit() step 11");
#endif
	CancelCreatePRgnTimer();
	CancelCreateTRgnTimer();
	CancelQueueTimer();
	CancelUpdateAttrTimer();
	CancelRgnKickPlayerTimer();
	
	m_GoodsTimerHandler.UnRegisterAll( this );
#ifdef _RUNSTACKINFO_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnExit() End");
#endif
}



//PK����(b0:ȫ�屣��)
BOOL	CPlayer::IsPk_Normal()	
{
	return (BOOL)(m_Property.byPKOnOff & 0x01);
}
void	CPlayer::SetPk_Normal(bool b)		
{
	if(b==true)
	{
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x01;
	}
	else
	{
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfe;
	}
}
//b1:��ӱ���
BOOL	CPlayer::IsPk_Team()					
{
	return (BOOL)(m_Property.byPKOnOff & 0x02);
}
void	CPlayer::SetPk_Team(bool b)
{
	if(b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x02;
	else 
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfd; 
}
//b2:���ᱣ��,
BOOL	CPlayer::IsPk_Union()
{
	return (BOOL)(m_Property.byPKOnOff & 0x04);
}
void	CPlayer::SetPk_Union(bool b)
{
	if(b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x04;
	else 
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xfb; 
}
//b3:��������
BOOL	CPlayer::IsPk_Badman()				
{
	return (BOOL)(m_Property.byPKOnOff & 0x08);
}
void	CPlayer::SetPk_Badman(bool b)
{
	if(b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x08;
	else m_Property.byPKOnOff = m_Property.byPKOnOff & 0xf7;
}
//b4:��������
BOOL	CPlayer::IsPk_Country()
{
	return (BOOL)(m_Property.byPKOnOff & 0x10);
}
void	CPlayer::SetPk_Country(bool b)	
{
	if(b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x10;
	else
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xef; 
}
//b5:��Ӫ����
BOOL	CPlayer::IsPk_Camp()	
{
	return (BOOL)(m_Property.byPKOnOff & 0x20);
}
void	CPlayer::SetPk_Camp(bool b)			
{
	if (b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x20;
	else
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xdf;
}
//b6.��������
BOOL	CPlayer::IsPk_Pirate()				
{
	return (BOOL)(m_Property.byPKOnOff & 0x40);
}
void	CPlayer::SetPk_Pirate(bool b)
{
	if (b==true)
		m_Property.byPKOnOff = m_Property.byPKOnOff | 0x40;
	else
		m_Property.byPKOnOff = m_Property.byPKOnOff & 0xbf;
}
//���˳���Ϸʱ����,�л���������ʱ�򲻻ᴥ��
void CPlayer::OnExitGame(BOOL bOnChangeServer)
{
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnExitGame() Start");
#endif
	if(bOnChangeServer == FALSE)
	{
        //֪ͨchat server���������
        CMessage csmsg(MSG_S2CS_PLAYER_EXIT_GAME);
        csmsg.Add(GetExID());
        csmsg.SendToChatServer();

		// ���߻سǵ�����
		CServerRegion* pRegion = static_cast<CServerRegion*>(GetFather());
		if(pRegion)
		{
			long lRetRgnTemplateID = 0;
			long l = 0;
			long t = 0;
			long r = 0;
			long b = 0;
			long d = 0;

			if(pRegion->GetRgnType() == RGN_NORMAL)
			{	
				BYTE byCountry = GetCountry();

				if(GetState()==STATE_DIED  && (long)GetPkValue()<GlobalSetup::GetSetup()->lPrisonIslandPkValue
										   && GetPkValue()>GlobalSetup::GetSetup()->dwPkCountPerAttack)
				{
					//�������ߣ�pkֵ���20С��18000,���ŵ�������
					lRetRgnTemplateID = GetCountryParam()->GetCountryJailVillageRegionID(byCountry);
					l = GetCountryParam()->GetCountryJailVillageRectLeft(byCountry);
					t = GetCountryParam()->GetCountryJailVillageRectTop(byCountry);
					r = GetCountryParam()->GetCountryJailVillageRectRight(byCountry);
					b = GetCountryParam()->GetCountryJailVillageRectBottom(byCountry);
					d = GetCountryParam()->GetCountryJailVillageDir(byCountry);	
				}
				else if(GetState()==STATE_DIED  && (long)GetPkValue()>=GlobalSetup::GetSetup()->lPrisonIslandPkValue)
				{
					//�������ߣ�pkֵ���lPrisonIslandPkValue,���ŵ�������
					lRetRgnTemplateID = GetCountryParam()->GetCountryJailIsLandRegionID(byCountry);
					l = GetCountryParam()->GetCountryJailIsLandRectLeft(byCountry);
					t = GetCountryParam()->GetCountryJailIsLandRectTop(byCountry);
					r = GetCountryParam()->GetCountryJailIsLandRectRight(byCountry);
					b = GetCountryParam()->GetCountryJailIsLandRectBottom(byCountry);
					d = GetCountryParam()->GetCountryJailIsLandDir(byCountry);
				}
				else if((pRegion->GetSetup().bLogoutIsUsed && GetState() == STATE_DIED  ) 
					|| (pRegion->GetSetup().bLogoutIsUsed && GetState() != STATE_DIED  &&  pRegion->GetDeadReturn()))
				{					
					pRegion->GetLogoutReturnPoint(this, lRetRgnTemplateID, l, t, r, b, d);
				}
				else
				{
					//���ֵ�ǰλ��
					lRetRgnTemplateID = GetRegionID();
					l = GetTileX();
					t = GetTileY();
					r = GetTileX();
					b = GetTileY();
					d = GetDir();
				}
			}
			else // �Ǹ�������
			{
				if(pRegion->GetSetup().bDupRgnIsUsed)
					pRegion->GetDupRgnReturnPoint(this, lRetRgnTemplateID, l, t, r, b, d);
			}


			

			

			// ���������״̬
			if(GetState() == STATE_DIED)
			{
				SetState(STATE_PEACE);
				SetHP(GetMaxHP()/2);
				PlayerRunScript(GlobalSetup::GetSetup()->szReliveScriptName);
			}

			if(lRetRgnTemplateID > 0) // ���Ƿ��ص�ǰ��ͼ
			{
				SetRegionGUID(NULL_GUID);
				SetRegionID(lRetRgnTemplateID);
				CShape::SetPosXY((float)l, (float)t);
			}
			else if(lRetRgnTemplateID == -1) // �ع���
			{
				BYTE byCountry = GetCountry();
				long lCityRegion = GetCountryParam()->GetCountryMainRegionID(byCountry);
				l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
				t = GetCountryParam()->GetCountryMainRectTop(byCountry);
				r = GetCountryParam()->GetCountryMainRectRight(byCountry);
				b = GetCountryParam()->GetCountryMainRectBottom(byCountry);
	

				SetRegionGUID(NULL_GUID);
				SetRegionID(lCityRegion);
				CShape::SetPosXY((float)l, (float)t);
			}
		}

		//��������ʱ��
		time_t curTime;
		time(&curTime);
		SetLastExitGameTime((DWORD)curTime);
		
		StopAllSkills();
		StopPassiveSkills();
		

		// �޸�״̬
		if(GetState() != STATE_DIED)
			SetState(STATE_PEACE);

	
		//##������Ϣ
		CMessage msg( MSG_S2W_LOG_PLAYERQUIT );
		msg.Add( GetExID() );
		msg.Add((long)1); // �����˳�
		for (LONG i = 0; i < SERVER_KEY_DWORD_SIZE; ++i)
		{
			msg.Add(GetServerKey(i));
		}
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		CodeToDataBlock(setWriteDB);
		msg.Send();

		// ���ͱ���PHRgn��Ϣ
		if(GetPersonalHouseRgnGUID() != NULL_GUID)
		{
			GetGame()->SendSavePHDupRgn(GetExID());
		}
	}
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnExitGame() End");
#endif
}


DWORD CPlayer::GetMoney()
{
	return m_pPackage.GetWallet().GetGoldCoinsAmount();
}
DWORD	CPlayer::GetGem(void)
{
	return m_pPackage.GetGem().GetGoldCoinsAmount();
}

//!		�õ���װ�����Ǯ��
DWORD	CPlayer::GetMaxMoney(void)
{
	return m_pPackage.GetWallet().GetMaxCapacity();
}

DWORD   CPlayer::GetSilver()
{
	return m_pPackage.GetSilverWallet().GetGoldCoinsAmount();
}

DWORD   CPlayer::GetMaxSilver()
{
	return m_pPackage.GetSilverWallet().GetMaxCapacity();
}

bool CPlayer::SetMoney( DWORD dwCoins,int nFlag )
{
	long lVal=dwCoins;
	if(lVal<0)
	{
		Log4c::Warn(ROOT_MODULE,FormatText("GS_PLAYER_2",lVal));//SetMoney�����˸���%d
		return false;
	}
	bool bResult = false;
	CS2CContainerObjectMove comMsg;
	if( dwCoins > GetMoney() )
	{	
		GetLogicLogInterface()->logT503_Gold_gain_log(this,GetMoney(),dwCoins,nFlag);		
		bResult = m_pPackage.GetWallet().AddGoldCoins( dwCoins - GetMoney(), &comMsg )?true:false;
	}
	else if( dwCoins < GetMoney() )
	{				
		GetLogicLogInterface()->logT503_Gold_gain_log(this,GetMoney(),dwCoins,nFlag);		
		bResult = m_pPackage.GetWallet().DecreaseGoldCoins( GetMoney() - dwCoins, &comMsg )?true:false;
	}
	if( bResult )
	{
		if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_NEW_OBJECT )
		{
			//##����Ǯ�Ĳ�����
			comMsg.SetDestinationContainerExtendID( CS2CContainerObjectMove::PEI_WALLET );
			comMsg.Send( GetExID() );
		}
		else if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_DELETE_OBJECT )
		{
			//##ɾ��Ǯ�Ĳ���
			comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_WALLET );
			comMsg.Send( GetExID() );
		}
		else 
		{
			//##���Ӽ���
			CGoods* pGoldCoins = m_pPackage.GetWallet().GetGoods( 0 );
			if( pGoldCoins )
			{
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( GetType(), GetExID(), 0 );
				coacMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_WALLET );
				coacMsg.SetObject( pGoldCoins -> GetType(), pGoldCoins -> GetExID() );
				coacMsg.SetObjectAmount( pGoldCoins -> GetAmount() );
				coacMsg.Send( GetExID() );
			}
		}
		// ����Ǯ����Ʒ��־
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRWALLETGOODS);
	}
	return bResult;
}

bool CPlayer::SetSilver( DWORD dwCoins )
{
	long lVal=dwCoins;
	if(lVal<0)
	{
		Log4c::Warn(ROOT_MODULE,FormatText("GS_PLAYER_3",lVal));//SetSilver�����˸���%d
		return false;
	}
	bool bResult = false;
	CS2CContainerObjectMove comMsg;
	if( dwCoins > GetSilver() )
	{
		bResult = m_pPackage.GetSilverWallet().AddGoldCoins( dwCoins - GetSilver(), &comMsg )?true:false;
	}
	else if( dwCoins < GetSilver() )
	{
		bResult = m_pPackage.GetSilverWallet().DecreaseGoldCoins( GetSilver() - dwCoins, &comMsg )?true:false;
	}
	if( bResult )
	{
		if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_NEW_OBJECT )
		{
			//##����Ǯ�Ĳ�����
			comMsg.SetDestinationContainerExtendID( CS2CContainerObjectMove::PEI_SILVERWALLET );
			comMsg.Send( GetExID() );
		}
		else if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_DELETE_OBJECT )
		{
			//##ɾ��Ǯ�Ĳ���
			comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_SILVERWALLET );
			comMsg.Send( GetExID() );
		}
		else 
		{
			//##���Ӽ���
			CGoods* pSilverCoins = m_pPackage.GetSilverWallet().GetGoods( 0 );
			if( pSilverCoins )
			{
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( GetType(), GetExID(), 0 );
				coacMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_SILVERWALLET );
				coacMsg.SetObject( pSilverCoins -> GetType(), pSilverCoins -> GetExID() );
				coacMsg.SetObjectAmount( pSilverCoins -> GetAmount() );
				coacMsg.Send( GetExID() );
			}
		}
		// ����Ǯ����Ʒ��־
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRSILVERGOODS);
	}
	return bResult;
}

bool CPlayer::SetGem( DWORD lNum )
{
	bool bResult = false;
	CS2CContainerObjectMove comMsg;
	if( lNum > GetGem() )
	{
		bResult = m_pPackage.GetGem().AddGoldCoins( lNum - GetGem(), &comMsg )?true:false;
	}
	else if( lNum < GetGem() )
	{
		bResult = m_pPackage.GetGem().DecreaseGoldCoins( GetGem() - lNum, &comMsg )?true:false;
	}
	if( bResult )
	{
		if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_NEW_OBJECT )
		{
			//##����Ǯ�Ĳ�����
			comMsg.SetDestinationContainerExtendID( CS2CContainerObjectMove::PEI_GEM );
			comMsg.Send( GetExID() );
		}
		else if( comMsg.GetOperation() == CS2CContainerObjectMove::OT_DELETE_OBJECT )
		{
			//##ɾ��Ǯ�Ĳ���
			comMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_GEM );
			comMsg.Send( GetExID() );
		}
		else 
		{
			//##���Ӽ���
			CGoods* pGem = m_pPackage.GetGem().GetGoods( 0 );
			if( pGem )
			{
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( GetType(), GetExID(), 0 );
				coacMsg.SetSourceContainerExtendID( CS2CContainerObjectMove::PEI_GEM );
				coacMsg.SetObject( pGem -> GetType(), pGem -> GetExID() );
				coacMsg.SetObjectAmount( pGem -> GetAmount() );
				coacMsg.Send( GetExID() );
			}
		}
	}
	return bResult;
}

void CPlayer::SetYuanbao(DWORD dwVal)
{
	m_dwYuanbao=dwVal;
	CMessage msg(MSG_S2C_SET_YUANBAO);
	msg.Add(m_dwYuanbao);
	msg.SendToPlayer(GetExID());
}

CPacketListener::CPacketListener()
{
	m_pDBWriteData = NULL;
}

CPacketListener::~CPacketListener()
{
	m_pDBWriteData = NULL;
}

bool CPacketListener::OnTraversingContainer( CContainer* pContainer, CBaseObject* pObject )
{
	bool bResult	= false;
	CGoods* pGoods	= dynamic_cast<CGoods*>( pObject );

	CVolumeLimitGoodsContainer* pVolumeLimitGoodsContainer	= 
		dynamic_cast<CVolumeLimitGoodsContainer*>( pContainer );

	if( pVolumeLimitGoodsContainer && pGoods )
	{
		CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(
			pGoods -> GetBasePropertiesIndex() );
		if( pProperties )
		{
			DWORD dwPosition = 0;
			if( pVolumeLimitGoodsContainer -> QueryGoodsPosition(pGoods, dwPosition) )
			{

				if( pProperties -> GetGoodsBaseType() == GBT_EQUIPMENT ||
                    pProperties->GetGoodsBaseType() == GBT_TRUMP /*||
                    pProperties->GetGoodsType() == GT_EQUIP_LAND ||
                    pProperties->GetGoodsType() == GT_EQUIP_HUMAN*/)
				{
					m_pDBWriteData->AddToByteArray( static_cast<BYTE>(true) );
				}
				else
				{
					m_pDBWriteData->AddToByteArray( static_cast<BYTE>(false) );
				}
				m_pDBWriteData->AddToByteArray( static_cast<WORD>( pGoods -> GetAmount() ) );
				m_pDBWriteData->AddToByteArray( static_cast<BYTE>(dwPosition) );
				pGoods -> SerializeForOldClient( *m_pDBWriteData );
				bResult = true;
			}
		}
	}

	return bResult;
}

//CHAR* CPlayer::GetDepotPassword()
//{
//	return const_cast<CHAR*>( m_strDepotPassword.c_str() );
//}
//
//void CPlayer::SetDepotPassword( CHAR* strPassword )
//{
//	if( strPassword )
//	{
//		m_strDepotPassword = strPassword;
//	}
//}


// �������
WORD CPlayer::GetAtcInterval()
{
	/*
	switch(GetOccupation())
	{
	case PlayerList::OC_FIGHTER:
	return 700;

	case PlayerList::OC_HUNTER:
	break;

	case PlayerList::OC_TAOIST:
	break;
	}*/

	short sAI = 1500 - GetAtcSpeed() * 100;
	if( sAI<500 )
		sAI = 500;
	else if (sAI>2500)
		sAI = 2500;

	return sAI;
}

//##��ȡ��ȭ��ʱ��
DWORD CPlayer::GetStrikeOutTime()
{/*
 switch(GetOccupation()) 
 {
 case PlayerList::OC_FIGHTER:
 return 500;

 case PlayerList::OC_HUNTER:
 break;

 case PlayerList::OC_TAOIST:
 break;
 }*/

	return 600;
}

bool CPlayer::IsVisible(CMoveShape* pTarget)
{
	if(GetIsGMConcealment())
		return false;
	return CMoveShape::IsVisible(pTarget);
}

bool CPlayer::IsAttackAble(CMoveShape* pAttacker)
{
	bool bResult = false;
	//if( m_lCityWarDiedState == true )
	//	return bResult;
	//�����GM,���ҳ���������޵�״̬���ܱ�����
	if( GetIsGMInvincible() || GetIsGMConcealment() || 
        PHASEMAN.PhasePlayerFilter(this,pAttacker) || 
        IsCanBeenAttack()==0 )
		return bResult;

	//�쿴�Ƿ��ǳ�ս����ģʽ
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );

	if( pRegion )
	{
		switch( pAttacker -> GetType() )
		{
		case TYPE_PLAYER:
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>( pAttacker );
				if( pPlayer )
				{
					// �����nopk��������Ҳ���PK��
					if(0 == pRegion->IsNoPk())
						return false;

					// ��������߻����ܺ����ڰ�ȫ���ڣ��򹥻���Ч��
					if( pRegion->GetSecurity(GetTileX(),GetTileY()) ==  CRegion::SECURTIY_SAFE ||
						pRegion->GetSecurity(pPlayer->GetTileX(),pPlayer->GetTileY()) == CRegion::SECURTIY_SAFE )
						return false;

					bResult = true;
					//##���Լ�����ɱ��
					if( GetExID() == pPlayer -> GetExID() )
					{
						bResult = false;
					}

					//if( pPlayer->IsEnemyFactionMember(GetFactionID()) > 0|| pPlayer->IsCityWarEneymyFactionMemeber(GetFactionID()) > 0 )
					//{
					//	break;
					//}
					//##�������������ң���ô���Ӧ����ѭPK�������ơ�
					if((pPlayer->IsPk_Normal())
						&& IsBadman()==false)
					{
						if( GetCountry() == pPlayer->GetCountry())
						{
							bResult = false;
						}
					}

					if( (pPlayer->IsPk_Team()) && 
						NULL_GUID != pPlayer -> GetTeamID() &&
						pPlayer -> GetTeamID() == GetTeamID() )
					{
						bResult = false;
					}
					if( (pPlayer-> IsPk_Union()) &&
						(( NULL_GUID != pPlayer -> GetFactionID() && 
						pPlayer -> GetFactionID() == GetFactionID()) || 
						(NULL_GUID != pPlayer -> GetUnionID() &&
						pPlayer -> GetUnionID() == GetUnionID() )) )
					{
						bResult = false;
					}
					if( (pPlayer->IsPk_Badman()) &&	IsBadman() )
					{
						bResult = false;
					}

					if((pPlayer->IsPk_Country()) && 
						IsBadman() == false )
					{
						if( GetCountry() != pPlayer->GetCountry() )
						{
							bResult = false;
						}
					}
				}
			}
			break;

		case TYPE_PET:
			{
				CPet* pPet = dynamic_cast<CPet*>( pAttacker );
				if( pPet != NULL )
				{
					// �����nopk��������Ҳ���PK��
					if(0 == pRegion->IsNoPk())
						return false;

					CPlayer* pPlayer = (CPlayer*)pPet->GetHost();
					if( NULL == pPlayer || this == pPlayer )
					{
						return false;
					}

					// ���������(����)�����ܺ����ڰ�ȫ���ڣ��򹥻���Ч��
					if( pRegion->GetSecurity(GetTileX(),GetTileY()) ==  CRegion::SECURTIY_SAFE ||
						pRegion->GetSecurity(pPet->GetTileX(),pPet->GetTileY()) == CRegion::SECURTIY_SAFE ||
						pRegion->GetSecurity(pPlayer->GetTileX(),pPlayer->GetTileY()) == CRegion::SECURTIY_SAFE )
						return false;

					bResult = true;
					//##���Լ�����ɱ��
					if( GetExID() == pPet->GetExID() )
					{
						bResult = false;
					}

					//##�������������ң���ô���Ӧ����ѭPK�������ơ�
					if((pPlayer->IsPk_Normal())
						&& IsBadman()==false)
					{
						if( GetCountry() == pPlayer->GetCountry())
						{
							bResult = false;
						}
					}

					if( (pPlayer->IsPk_Team()) && 
						NULL_GUID != pPlayer -> GetTeamID() &&
						pPlayer -> GetTeamID() == GetTeamID() )
					{
						bResult = false;
					}
					if( (pPlayer-> IsPk_Union()) &&
						(( NULL_GUID != pPlayer -> GetFactionID() && 
						pPlayer -> GetFactionID() == GetFactionID()) || 
						(NULL_GUID != pPlayer -> GetUnionID() &&
						pPlayer -> GetUnionID() == GetUnionID() )) )
					{
						bResult = false;
					}
					if( (pPlayer->IsPk_Badman()) &&	IsBadman() )
					{
						bResult = false;
					}

					if((pPlayer->IsPk_Country()) && 
						IsBadman() == false )
					{
						if( GetCountry() != pPlayer->GetCountry() )
						{
							bResult = false;
						}
					}
				}
			}
			break;

		case TYPE_MONSTER:
			//##����������ǹ����ô��ͨ�Ĺ���Ӧ�ÿ��Թ���������������Ҫ�ж��Ƿ��ǻ���
			{
				CMonster* pMonster = dynamic_cast<CMonster*>( pAttacker );
				if( pMonster )
				{
					bResult = true;
					////##����ǳ���
					//if( pMonster -> DoesCreatureBeenTamed() && pMonster -> m_pPetAI )
					//{
					//	CMoveShape* pMaster = pMonster -> m_pPetAI -> GetPetMaster();
					//	if( pMaster )
					//	{
					//		if( pMaster == this || IsAttackAble(pMaster) == FALSE )
					//		{
					//			bResult = false;
					//		}
					//	}
					//	//##����Ҳ������ˣ����п��������Ѿ����ߣ����ʱ�򣬳��ﲻ�ٹ����κ���ң�
					//	//else 
					//	//{
					//	//	bResult = false;
					//	//}
					//}
					//
					//else

					//##���������
					if( pMonster -> GetNpcType()  == NT_Guard )
					{

						// commented by kevin lynx . ����Ҫ���������ж���
						//if( pAttacker -> GetAI() &&( pAttacker -> GetAI()->GetAIType() == CAIFactory::CITY_GUARD_WITH_SWORD ||pAttacker -> GetAI()->GetAIType() == CAIFactory::CITY_GUARD_WITH_BOW) )
						//{
						//	//##���س���
						//	if( NULL_GUID != GetFactionID() && GetFactionID() == pRegion->GetOwnedCityFaction() )
						//	{
						//		bResult = false;
						//	}
						//	else
						//	{
						//		if( NULL_GUID != GetUnionID() &&  GetUnionID() == pRegion->GetOwnedCityUnion() )
						//		{
						//			bResult = false;
						//		}
						//	}
						//}
						//else
						//{
						//	//##��һ�������
						//	if( IsBadman() == false )
						//	{
						//		bResult = false;
						//	}
						//}
					}
				}
			}

			break;
		}
	}
	return bResult;
}


long CPlayer::OnBeenAttacked( CMoveShape* pAttacker,long lHp,long lDamageResult)
{
	//�����GM,���ҳ���������޵�״̬���ܱ�����
	if(GetIsGMInvincible() || GetIsGMConcealment() )
		return 0;
	return CMoveShape::OnBeenAttacked(pAttacker,lHp,lDamageResult);
}

void CPlayer::OnBeenKilled(CMoveShape* pAttacker)
{
	CMoveShape::OnBeenKilled(pAttacker);
	//���ɱ����ҵ�ʱ�򣬴����ű�
	if(pAttacker && pAttacker->GetType() == TYPE_PLAYER)
	{
		//PVPɱ��ʱ�����ű�
		const char* strScript = GlobalSetup::GetSetup()->pszPVPKilled;
		if(strScript)
		{
			//stRunScript sScript;
			//sScript.pszFileName = (char*)strScript;
			//sScript.desShape = this;
			//sScript.pRegion =  dynamic_cast<CRegion*>(GetFather());
			//sScript.srcShape	= pAttacker;
			//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
			Script::RunEnv env( pAttacker, this, static_cast<CRegion*>( GetFather() ) );
			GetInst( ScriptSys ).RunScript( env, strScript );
		}

		//����ʱ�����ű�
		strScript = GlobalSetup::GetSetup()->pszPVPBeKilled;
		if(strScript)
		{
			//stRunScript sScript;
			//sScript.pszFileName = (char*)strScript;
			//sScript.desShape = pAttacker;
			//sScript.pRegion =  dynamic_cast<CRegion*>(GetFather());
			//sScript.srcShape	= this;
			//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ) );
			Script::RunEnv env( this, pAttacker, static_cast<CRegion*>( GetFather() ) );
			GetInst( ScriptSys ).RunScript( env, strScript );
		}
	}
}


DWORD  CPlayer::DeleteGoods(CS2CContainerObjectMove::PLAYER_EXTEND_ID pei,eGoodsBaseType baseType,DWORD dwWeaponType,DWORD dwNum)
{
	DWORD dwDelNum=0;
	CVolumeLimitGoodsContainer* pContainer = NULL;
	switch( pei )
	{
	case CS2CContainerObjectMove::PEI_PACKET:
		pContainer = getOriginPack();
		break;
		/*
		case CS2CContainerObjectMove::PEI_EQUIPMENT:
		pContainer = &m_cEquipment;
		break;*/	
	case CS2CContainerObjectMove::PEI_PACK1:
		pContainer=GetPackage()->GetSubpack(0)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK2:
		pContainer=GetPackage()->GetSubpack(1)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK3:
		pContainer=GetPackage()->GetSubpack(2)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK4:
		pContainer=GetPackage()->GetSubpack(3)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK5:
		pContainer=GetPackage()->GetSubpack(4)->pSubpackContainer;
		break;
	}
	if( pContainer )
	{
		dwDelNum=pContainer->DelGoods(baseType,dwWeaponType,dwNum,pei,GetExID());
	}
	return dwDelNum;
}


DWORD CPlayer::DeleteGoods( CS2CContainerObjectMove::PLAYER_EXTEND_ID pei, const CGUID& guid, DWORD dwAmount )
{
	DWORD dwResult = 0;
	CGoodsContainer* pContainer = NULL;
	switch( pei )
	{
	case CS2CContainerObjectMove::PEI_PACKET:
		pContainer = getOriginPack();
		break;
	case CS2CContainerObjectMove::PEI_EQUIPMENT:
		pContainer = &m_cEquipment;
		break;
	case CS2CContainerObjectMove::PEI_GEM:
		pContainer = &m_pPackage.GetGem();
		break;
	case CS2CContainerObjectMove::PEI_WALLET:
		pContainer = &m_pPackage.GetWallet();
		break;
    case CS2CContainerObjectMove::PEI_EQUIP_GOD:
        pContainer = &m_cEquipmentGod;
        break;
    case CS2CContainerObjectMove::PEI_EQUIP_LAND:
        pContainer = &m_cEquipmentLand;
        break;
    case CS2CContainerObjectMove::PEI_EQUIP_HUMAN:
        pContainer = &m_cEquipmentHuman;
        break;
	case CS2CContainerObjectMove::PEI_PACK1:
		pContainer=GetPackage()->GetSubpack(0)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK2:
		pContainer=GetPackage()->GetSubpack(1)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK3:
		pContainer=GetPackage()->GetSubpack(2)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK4:
		pContainer=GetPackage()->GetSubpack(3)->pSubpackContainer;
		break;
	case CS2CContainerObjectMove::PEI_PACK5:
		pContainer=GetPackage()->GetSubpack(4)->pSubpackContainer;
		break;		
	}
	if( pContainer )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( pContainer -> Find(TYPE_GOODS, guid) );
		if( pGoods )
		{
			DWORD dwPosition = 0;
			if( pContainer -> QueryGoodsPosition(pGoods, dwPosition) )
			{
				if( pGoods -> GetAmount() > dwAmount )
				{
					//##�ı�����
					dwResult = dwAmount;
					pGoods -> SetAmount( pGoods -> GetAmount() - dwAmount );

					CS2CContainerObjectAmountChange coacMsg;
					coacMsg.SetSourceContainer( GetType(), GetExID(), dwPosition );
					coacMsg.SetSourceContainerExtendID( pei );
					coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
					coacMsg.SetObjectAmount( pGoods -> GetAmount() );
					coacMsg.Send( GetExID() );

					OnLoseGoods(pGoods);

					//Goods Consume Log
					GetLogicLogInterface()->logT022_goods_log_consume(this,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),
						GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),pGoods->GetAmount(),dwAmount,pei,dwPosition);
				}
				else
				{
					//##ɾ��
					CS2CContainerObjectMove comMsg;
					if( pContainer -> Remove(pGoods, &comMsg) )
					{
						dwResult = pGoods -> GetAmount();

						comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
						comMsg.SetSourceContainerExtendID( pei );						
						comMsg.Send( GetExID() );

						OnLoseGoods(pGoods);

						//Goods Consume Log
						GetLogicLogInterface()->logT022_goods_log_consume(this,pGoods->GetBasePropertiesIndex(),pGoods->GetExID(),
							GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),pGoods->GetAmount(),dwAmount,pei,dwPosition);
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::DeleteGoods][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif

						//##ɾ������Ʒ?
						//GoodsSetup::GarbageCollect( &pGoods ,31);
						CGoodsFactory::AddGoodsToGCSet(pGoods);

					}
				}
			}
		}
	}
	return dwResult;
}

DWORD CPlayer::DeleteDepotGoods( DWORD dwContainerType, const CGUID& guid, DWORD dwAmount )
{
	DWORD dwResult = 0;
	CGoodsContainer* pContainer = m_pPlayerDepot.FindContainer(dwContainerType, true);

	if( pContainer )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( pContainer -> Find(TYPE_GOODS, guid) );
		if( pGoods )
		{
			DWORD dwPosition = 0;
			if( pContainer -> QueryGoodsPosition(pGoods, dwPosition) )
			{
				if( pGoods -> GetAmount() > dwAmount )
				{
					//##�ı�����
					dwResult = dwAmount;
					pGoods -> SetAmount( pGoods -> GetAmount() - dwAmount );

					CS2CContainerObjectAmountChange coacMsg;
					coacMsg.SetSourceContainer( GetType(), GetExID(), dwPosition );
					coacMsg.SetSourceContainerExtendID( dwContainerType );
					coacMsg.SetObject( pGoods -> GetType(), pGoods -> GetExID() );
					coacMsg.SetObjectAmount( pGoods -> GetAmount() );
					coacMsg.Send( GetExID() );
				}
				else
				{
					//##ɾ��
					CS2CContainerObjectMove comMsg;
					if( pContainer -> Remove(pGoods, &comMsg) )
					{
						dwResult = pGoods -> GetAmount();

						comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
						comMsg.SetSourceContainerExtendID( dwContainerType );						
						comMsg.Send( GetExID() );
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";			
						char srcGoodsGUID[50]="";
						pGoods->GetExID().tostring(srcGoodsGUID);					
						_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::DeleteGoods][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
							pGoods->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(pGoods->GetBasePropertiesIndex()),srcGoodsGUID,pGoods->GetAmount());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);							
#endif

						//##ɾ������Ʒ?
						//GoodsSetup::GarbageCollect( &pGoods ,31);
						CGoodsFactory::AddGoodsToGCSet(pGoods);

					}
				}
			}
		}
	}
	return dwResult;
}

void CPlayer::DropParticularGoodsWhenDead()
{
	if( IsDied() )
	{
		CGoodsListListener lGoodsList;
		getOriginPack()->TraversingContainer( &lGoodsList );
		for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
			if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
				PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
			{
				DropGoods( CS2CContainerObjectMove::PEI_PACKET, lGoodsList.m_vGoodsID[i] );
			}
		}

		lGoodsList.m_vGoodsID.clear();
		m_cEquipment.TraversingContainer( &lGoodsList );
		for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
			if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
				PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
			{
				DropGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lGoodsList.m_vGoodsID[i] );
			}
		}	

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentGod.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentGod.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_GOD, lGoodsList.m_vGoodsID[i] );
            }
        }	

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentLand.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentLand.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_LAND, lGoodsList.m_vGoodsID[i] );
            }
        }	

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentHuman.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentHuman.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_HUMAN, lGoodsList.m_vGoodsID[i] );
            }
        }	

		for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
		{
			if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
			{			
				lGoodsList.m_vGoodsID.clear();
				GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lGoodsList);
				for(size_t j=0;j<lGoodsList.m_vGoodsID.size();j++)
				{
					CGoods* pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[j]));
					if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
						PA_DROP_DOWN_WHEN_DEAD ) == PA_DROP_DOWN_WHEN_DEAD )
					{
						DropGoods( static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i), lGoodsList.m_vGoodsID[j] );
					}
				}	
			}
		}	

	}
}

void CPlayer::DropParticularGoodsWhenLost()
{
	//if( IsDied() )
	{
		CGoodsListListener lGoodsList;
		getOriginPack()->TraversingContainer( &lGoodsList );
		for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
			if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
				PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
			{
				DropGoods( CS2CContainerObjectMove::PEI_PACKET, lGoodsList.m_vGoodsID[i] );
			}
		}

		lGoodsList.m_vGoodsID.clear();
		m_cEquipment.TraversingContainer( &lGoodsList );
		for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
		{
			CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
			if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
				PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
			{
				DropGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lGoodsList.m_vGoodsID[i] );
			}
		}

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentGod.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentGod.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_GOD, lGoodsList.m_vGoodsID[i] );
            }
        }	

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentLand.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentLand.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_LAND, lGoodsList.m_vGoodsID[i] );
            }
        }	

        lGoodsList.m_vGoodsID.clear();
        m_cEquipmentHuman.TraversingContainer( &lGoodsList );
        for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
        {
            CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentHuman.Find(lGoodsList.m_vGoodsID[i]) );
            if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
                PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
            {
                DropGoods( CS2CContainerObjectMove::PEI_EQUIP_HUMAN, lGoodsList.m_vGoodsID[i] );
            }
        }	

		for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
		{
			if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
			{			
				lGoodsList.m_vGoodsID.clear();
				GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lGoodsList);
				for(size_t j=0;j<lGoodsList.m_vGoodsID.size();j++)
				{
					CGoods* pGoods = dynamic_cast<CGoods*>(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[j]));
					if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
						PA_DROP_DOWN_WHEN_LOGOUT ) == PA_DROP_DOWN_WHEN_LOGOUT )
					{
						DropGoods( static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i), lGoodsList.m_vGoodsID[j] );
					}
				}	
			}
		}	

	}
}

void CPlayer::DropParticularGoodsWhenRecall()
{
	CGoodsListListener lGoodsList;
	getOriginPack()->TraversingContainer( &lGoodsList );
	for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
		if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
			PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
		{
			DropGoods( CS2CContainerObjectMove::PEI_PACKET, lGoodsList.m_vGoodsID[i] );
		}
	}

	lGoodsList.m_vGoodsID.clear();
	m_cEquipment.TraversingContainer( &lGoodsList );
	for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipment.Find(TYPE_GOODS, lGoodsList.m_vGoodsID[i]) );
		if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
			PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
		{
			DropGoods( CS2CContainerObjectMove::PEI_EQUIPMENT, lGoodsList.m_vGoodsID[i] );
		}
	}

    lGoodsList.m_vGoodsID.clear();
    m_cEquipmentGod.TraversingContainer( &lGoodsList );
    for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
    {
        CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentGod.Find(lGoodsList.m_vGoodsID[i]) );
        if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
            PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
        {
            DropGoods( CS2CContainerObjectMove::PEI_EQUIP_GOD, lGoodsList.m_vGoodsID[i] );
        }
    }

    lGoodsList.m_vGoodsID.clear();
    m_cEquipmentLand.TraversingContainer( &lGoodsList );
    for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
    {
        CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentLand.Find(lGoodsList.m_vGoodsID[i]) );
        if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
           PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
        {
            DropGoods( CS2CContainerObjectMove::PEI_EQUIP_LAND, lGoodsList.m_vGoodsID[i] );
        }
    }

    lGoodsList.m_vGoodsID.clear();
    m_cEquipmentHuman.TraversingContainer( &lGoodsList );
    for( size_t i = 0; i < lGoodsList.m_vGoodsID.size(); i ++ )
    {
        CGoods* pGoods = dynamic_cast<CGoods*>( m_cEquipmentHuman.Find(lGoodsList.m_vGoodsID[i]) );
        if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
            PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
        {
            DropGoods( CS2CContainerObjectMove::PEI_EQUIP_HUMAN, lGoodsList.m_vGoodsID[i] );
        }
    }

	for(int i=CS2CContainerObjectMove::PEI_PACK1;i<=CS2CContainerObjectMove::PEI_PACK5;i++)
	{
		if(GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pGoods)
		{			
			lGoodsList.m_vGoodsID.clear();
			GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer->TraversingContainer(&lGoodsList);
			for(size_t j=0;j<lGoodsList.m_vGoodsID.size();j++)
			{
				CVolumeLimitGoodsContainer* pSubContainer=GetPackage()->GetSubpack(i-CS2CContainerObjectMove::PEI_PACK1)->pSubpackContainer;
				CGoods* pGoods = dynamic_cast<CGoods*>(pSubContainer->Find(TYPE_GOODS, lGoodsList.m_vGoodsID[j]));
				if( pGoods && ( pGoods -> GetAddonPropertyValues(GAP_PARTICULAR_ATTRIBUTE, 1) &
					PA_DROP_DOWN_WHEN_RECALL ) == PA_DROP_DOWN_WHEN_RECALL )
				{
					DropGoods( static_cast<CS2CContainerObjectMove::PLAYER_EXTEND_ID>(i), lGoodsList.m_vGoodsID[j] );
				}
			}	
		}
	}	

}

void CPlayer::DeleteGoodsInPacket(char* strGoodName)
{
	if(strGoodName==NULL)
		return;

	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget( strGoodName );
	getOriginPack()->TraversingContainer( &lSeeker );
	if(lSeeker.m_vGoodsID.size()<=0)
		return;

	LONG lRemainedGoods = 1;
	DeleteGoods( CS2CContainerObjectMove::PEI_PACKET, lSeeker.m_vGoodsID[0], lRemainedGoods );
}

//����ֵ
void CPlayer::SetExp(LONG64 l)
{
	DWORD lExpMultiple = (DWORD)( l / dwExpMutipleValue );
	DWORD lExp = (DWORD)(  l % dwExpMutipleValue );
	m_Property.dwExp = lExp;
	m_Property.dwExpMultiple = lExpMultiple;
}

//!����������Ӿ���
void CPlayer::IncExp(LONG64 llExp, DWORD dwAddType)
{
	LONG64 llResultExp = llExp+GetTotalExp();
	if(0 > llResultExp)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_11", GetName(), GetExp(), llExp, dwAddType));//��ҡ�%s���õ�һ�����ľ��顾��ǰ���� = %ld�����Ӿ��� = %ld���������� = %d����ִ�з��أ�
		return;
	}

	LONG64 llMaxExp = PlayerList::GetMaxExp();
	// ��ǰ�����Ѿ��������ֵ
	if(GetTotalExp() >= llMaxExp)
	{
		return;
	}

	if(llResultExp > llMaxExp)
		llResultExp = llMaxExp;

	SetExp(llResultExp);

	//if( !m_FightPets.empty() ) 
	//{
	//	AddPetsExp((long)llExp, (long)dwAddType);
	//	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	//}

	CMessage msg( MSG_S2C_PLAYER_ADDEXP );
	msg.Add(dwAddType);
	msg.Add( GetExp() );
	msg.Add( GetExpMultiple() );
	//�õ���ǰ���;���
	msg.Add(GetPresentExp());
	//		msg.Add( GetVigour() );
	msg.SendToPlayer( GetExID());
}

//!		��Ӿ�����־
void CPlayer::AddIncExpLog(DWORD dwOldExp, DWORD dwOldExpMultiple, DWORD dwAddExp, DWORD dwAddExpMultiple, DWORD dwAddType, const char *pSrcName)
{
	//���Լ��㾭��
	LONG64 llGetExp = GetLevel()*40;
	LONG64 llAddExp = (LONG64)dwExpMutipleValue*dwAddExpMultiple+dwAddExp;
	if(llAddExp > llGetExp)
		GetLogicLogInterface()->logT080_player_getexp_log(this, dwAddType, dwAddExp, dwAddExpMultiple, dwOldExp, dwOldExpMultiple, GetExp(), GetExpMultiple(), pSrcName);
}

//!		���ְҵ������־
void CPlayer::AddIncOccuExpLog(BYTE byOccu,DWORD dwOldExp, DWORD dwAddExp, DWORD dwAddType, const char *pSrcName)
{
	if(dwAddExp > 100)
		GetLogicLogInterface()->logT081_player_getoccuexp_log(this, dwAddType, byOccu, dwAddExp, dwOldExp, GetOccuExp((eOccupation)byOccu), pSrcName);
}

//�۳�SP
void CPlayer::DecOccuSp(eOccupation eOccu,long lSp)
{
	long lCurrentSp = GetOccuSp(eOccu) - lSp;
	if( 0 > lCurrentSp)
		lCurrentSp = 0;
	SetOccuSp(eOccu,lCurrentSp);	
	//##ͬ����ǰ���ܵ���ֵ
	CMessage msg( MSG_S2C_PLAYER_ADDSP );
	msg.Add((BYTE)eOccu);
	msg.Add( GetOccuSp(eOccu) );
	msg.SendToPlayer( GetExID());
}

//��������Ӽ��ܵ���
void CPlayer::IncOccuSp(eOccupation eOccu,long lSp)
{
	long lResultSp = GetOccuSp(eOccu)+lSp;
	if(0 > lResultSp)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_12", lSp));//�õ�һ�����ļ��ܵ�[%d]"/>
		return;
	}

	SetOccuSp(eOccu,lResultSp);
	//##ͬ����ǰ���ܵ���ֵ
	CMessage msg( MSG_S2C_PLAYER_ADDSP );
	msg.Add((BYTE)eOccu);
	msg.Add( GetOccuSp(eOccu) );
	msg.SendToPlayer( GetExID(),false);
}

//����ְҵ��ְҵ����
void CPlayer::IncOccuExp(eOccupation byOccu,long lExp, DWORD dwAddType)
{
	long lResultExp = GetOccuExp(byOccu)+lExp;
	if(0 > lResultExp)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PLAYER_13",byOccu,lExp,dwAddType));//�õ�һ������ְҵ����[Occu:%d,Exp:%d,addType:%d]
		return;
	}

	SetOccuExp(byOccu,lResultExp);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);

	CMessage msg( MSG_S2C_PLAYER_ADDOCCUEXP );
	msg.Add(dwAddType);
	msg.Add(BYTE(byOccu));
	msg.Add( GetOccuExp(byOccu) );
	msg.SendToPlayer( GetExID());
}

//��ȡ���;���
DWORD CPlayer::AcquirePresentExp(DWORD dwMaxAcqExp)
{
	DWORD dwOldPresentExp = GetPresentExp();
	DWORD dwResult = 0;
	if(GetPresentExp() >= dwMaxAcqExp)
		dwResult = dwMaxAcqExp;
	else
		dwResult = GetPresentExp();	
	SetPresentExp(GetPresentExp()-dwResult);
	if(dwOldPresentExp > 0 && GetPresentExp()== 0)
	{
		//ֹͣ��ʾ���;����buff
		DelNewState(GlobalSetup::GetSetup()->lPresentExpBuffId,GlobalSetup::GetSetup()->lPresentExpBuffLvl);
		//�������;���ֵ
		//���;���
		m_mapDirtyAttrData.insert("PresentExp");
		UpdateAttribute();		
	}
	return dwResult;
}

void CPlayer::PerformEmotion( LONG lID )
{
	//##�����������
	m_lEmotionIndex		= 0;
	m_dwEmotonTimeStamp	= 0;

	if( IsDied() ) return;
	if( GetAI() == FALSE ) return;
	if( GetAI() -> HasTarget() ) return;

	//##�ж��Ƿ���Ҫ�洢
	if( EmotionSetup::IsEmotionRepeated(lID) )
	{
		m_lEmotionIndex		= lID;
		m_dwEmotonTimeStamp	= timeGetTime();
	}

	CMessage msg( MSG_S2C_SHAPE_EMOTION );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( lID );
	msg.SendToAround( this, this );
}

void CPlayer::ClearEmotion()
{
	m_lEmotionIndex		= 0;
	m_dwEmotonTimeStamp	= 0;

	CMessage msg( MSG_S2C_SHAPE_EMOTION );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( static_cast<LONG>(0) );
	msg.SendToAround( this, this );
}

void CPlayer::PackGlint()
{
	CMessage msg( MSG_S2C_OTHER_PackGlint );
	msg.SendToPlayer( this->GetExID());
}

//����������ݵ�ByteArray
bool CPlayer::CodeQuestDataToDataBlock(DBWriteSet& setWriteDB)
{
	return m_RoleQueue.CodeToDataBlock(setWriteDB)?true:false;
}

//����������ݵ�ByteArray
bool CPlayer::CodeQuestDataToDataBlock_ForClient(DBWriteSet& setWriteDB)
{
	return m_RoleQueue.CodeToDataBlock(setWriteDB)?true:false;
}
bool CPlayer::DecodeQuestDataFromDataBlock(DBReadSet& setReadDB)
{
	return m_RoleQueue.DecodeFromDataBlock(setReadDB)?true:false;
}

//�õ�һ�������״̬(0:���ڵ�û���,1:���������,2:û��)
eQuestState CPlayer::GetQuestState(LONG lQuestID)
{
	if(m_RoleQueue.GetQuestFinishedRecord(lQuestID))
	{
		return eQS_Complete;
	}
	else if (m_RoleQueue.GetRecordByID(lQuestID)!=0)
	{
		return eQS_NotComplete;
	}
	return eQS_No;
}

//�õ���Ч����������
long CPlayer::GetValidQuestNum()
{
	return m_RoleQueue.GetQuestSum();
}
//����������һ������

BOOL CPlayer::AddQuest(CQuestRecord*& p)								//���һ������
{
	if(!m_RoleQueue.AddOneRecord(p))
		return FALSE;
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRQUESTLIST);
	return TRUE;
}
BOOL CPlayer::AddQuest(LONG lQuestID)
{
	if (!AddOneRecord(lQuestID))
	{
		return FALSE;
	}
	CQuestRecord* record =m_RoleQueue.GetRecordByID(lQuestID);
	if (record==NULL)
	{
		return FALSE;
	}
	GetLogicLogInterface()->logT460_quest_log(this,lQuestID,1);



	//���ͻ��˷�����Ϣ
	CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_Add);
    msg.Add((BYTE)QS::QS_ADD);
	msg.Add(record->GetQuestID());
	msg.Add(record->GetQuestStep());
	msg.SendToPlayer(GetExID());
	return TRUE ;
}
//��������һ������
bool CPlayer::CompleteQuest(DWORD dwQuestID)
{
	//��նӳ����������,�����ж϶�Ա�����ظ���
	if (GetTeamCurrQuest() == dwQuestID)
	{
		m_MerQuestSys.SetMasterQuestID(0);
	}
	m_MerQuestSys.OnDelQuest(dwQuestID);
	m_MerQuestSys.SetLastQuestID(dwQuestID);
	OnCompleteQuest( (long)dwQuestID );
	//ִ�����һ������
	if (!m_RoleQueue.DelOneRecord(dwQuestID))
	{
		return false;
	}
	long lListID=m_MerQuestSys.IsExistQuest(dwQuestID);
	if (0 == lListID)
	{
		lListID=CQuestSystem::GetQuestListID( (long) dwQuestID);
	}
	//���ˢ��ʱ��
	if (m_MerQuestSys.ClearTime(lListID))
	{
		FreshMercQuest(lListID);
	}
	if( GetAttributeBeRecord(dwQuestID) ) 
        AddFinishedRoleRecord(dwQuestID);
	GetLogicLogInterface()->logT460_quest_log(this,dwQuestID,2);
    CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_Complete);
    msg.Add((BYTE)QS::QS_COMPLETE);
	msg.Add(dwQuestID);
	msg.SendToPlayer(GetExID());
	return true;
}
//�Ƴ�һ������
void  CPlayer::RemoveQuest(LONG lQuestID)
{
	if(m_RoleQueue.DelOneRecord(lQuestID))
	{
		//string script=CQuestSystem::GetQuest(lQuestID)->GetAttribute().GetDeleteScript();
		////ִ��ɾ������ű�
		//char *str=new char[script.size()+1];
		//memcpy(str,script.c_str(),script.size()+1);
		//PlayerRunScript(str);
		//delete []str;
		GetLogicLogInterface()->logT460_quest_log(this,lQuestID,3);
        CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_Remove);
        msg.Add((BYTE)QS::QS_REMOVE);
		msg.Add(lQuestID);
		msg.SendToPlayer(GetExID());
	}	
	//����Ӷ����������
	if(m_MerQuestSys.OnDelQuest(lQuestID))
	{
		m_MerQuestSys.DisbandQuestRecord(lQuestID);
	}
	SendMercQuestDataToClient(0);
	SetSavePartFlag(GetSavePartFlag()|SAVE_DETAIL_ATTRQUESTLIST);

}

//����һ������Ĳ���
void CPlayer::UpdateQuestStep(DWORD dwQuestID,long lStep)
{
	CQuestRecord* record=m_RoleQueue.GetRecordByID(dwQuestID);
	if (record==NULL)
	{
		return ;
	}
	if (record->GetQuestStep()==lStep)
	{
		return ;
	}

	if(CQuestSystem::GetStep(dwQuestID,lStep)==0)
	{
		return;
	}
	m_RoleQueue.JumpStepTo(dwQuestID,(uchar)lStep);//GetRecordByID(dwQuestID)->JumpStepTo(lStep);
	CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_UpdateStep);
    msg.Add((BYTE)QS::QS_UPDATE_STEP);
	msg.Add((long)dwQuestID);
	msg.Add(lStep);
	msg.SendToPlayer(GetExID());	
}


void CPlayer::UpdataQuestParamData(DWORD dwQuestID,long lIndex,long lParam, long lRegionID, long lRegionX, long lRegionY)
{
	CQuestRecord* record=m_RoleQueue.GetRecordByID(dwQuestID);
	if (record==NULL)
	{
		return ;
	}

	//m_RoleQueue.GetRecordByID(dwQuestID)->SetParam(lIndex,lParam);
	record->SetParam(lIndex,lParam);
	m_RoleQueue.SetRegionID(record->GetQuestID(),record->GetQuestStep(),lIndex,lRegionID);
	m_RoleQueue.SetRegionX(record->GetQuestID(),record->GetQuestStep(),lIndex,lRegionX);
	m_RoleQueue.SetRegionY(record->GetQuestID(),record->GetQuestStep(),lIndex,lRegionY);
	CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_UpdateParamData);
    msg.Add((BYTE)QS::QS_UPDATE_PARAM);
	msg.Add((LONG)dwQuestID);
	msg.Add(lIndex);
	msg.Add(lParam);
	msg.Add(lRegionID);
	msg.Add(lRegionX);
	msg.Add(lRegionY);
	msg.SendToPlayer(GetExID());	
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRQUESTLIST);
}

void CPlayer::UpdataQuestData(DWORD dwQuestID,long lIndex,long lVal)
{
	CQuestRecord* record=m_RoleQueue.GetRecordByID(dwQuestID);
	if (record==NULL)
	{
		return ;
	}

	record->SetParam(lIndex,lVal);
	CMessage msg(MSG_S2C_QUEST);//(MSG_S2C_Quest_UpdateData);
    msg.Add((BYTE)QS::QS_UPDATE_DATA);
	msg.Add((LONG)dwQuestID);
	msg.Add(lIndex);
	msg.Add(lVal);
	msg.SendToPlayer(GetExID());	
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRQUESTLIST);
}

//���ٸ���������ݵ�ByteArray
bool CPlayer::CodeMerQuestDataToDataBlock(DBWriteSet& setWriteDB)
{
	return m_MerQuestSys.CodeToDataBlock(setWriteDB);
}

//��ٸ����������
bool CPlayer::DecodeMerQuestDataFromDataBlock(DBReadSet& setReadDB)
{
	return m_MerQuestSys.DecodeFromDataBlock(setReadDB);
}

//���ĳ���б�ID��ٸ������
void CPlayer::AcquireMercQuestList(long lMercListID,bool usegoods)
{
//	CMercenaryQuestSys::tagMerQuest MerQuests;
//	bool bResult = false;
//	//ɾ����Ӧ�б��е�����
//	set<long>	DisbandQuests;
//	m_MerQuestSys.GetDisbandQuest(DisbandQuests);
//	for (set<long>::iterator it=DisbandQuests.begin();it!=DisbandQuests.end();it++)
//	{
//		if(lMercListID == CQuestSystem::GetQuestListID( *it ))
//			m_MerQuestSys.DisbandQuestRecord( *it ,true);
//	}
//	//�ų������quest
//	set<long>	BesidesQuests;
//	m_MerQuestSys.GetMercListQuests(BesidesQuests);
//	BesidesQuests.insert(m_MerQuestSys.GetCurQuestID());
//	//����ˢ��
//	if (usegoods)
//	{
//		bResult = CQuestSystem::GetMercQuests(lMercListID,GetLevel(),
//			GetOccupation(),GetRankOfMercenary(),
//			BesidesQuests,MerQuests.Quests,MAX_MERC_QUEST_NUM,
//			usegoods);
//	}
//	//����ˢ��
//	else
//	{
//		//�ų����һ����ɵ�����
//		long lastquestID=m_MerQuestSys.GetLastQuestID();
//		if (lastquestID!=0)
//		{
//			BesidesQuests.insert(lastquestID);
//			m_MerQuestSys.SetLastQuestID(0);
//		}
//		bResult = CQuestSystem::GetMercQuests(lMercListID,GetLevel(),
//			GetOccupation(),GetRankOfMercenary(),
//			BesidesQuests,MerQuests.Quests,MAX_MERC_QUEST_NUM);
//	}
//	if(bResult)
//	{
//		if (MerQuests.Quests.size()==MAX_MERC_QUEST_NUM)
//		{
//			GetLogicLogInterface()->logT461_mercquest_log(this,usegoods?1:0,MerQuests.Quests[0],
//			MerQuests.Quests[1],
//			MerQuests.Quests[2],
//			MerQuests.Quests[3]);
//		}
//		
//		time_t curTime;
//		time(&curTime);
//		MerQuests.dwLastUpdateTime=DWORD(curTime);
//		m_MerQuestSys.SetMercQuests(lMercListID,MerQuests);
//		SetSavePartFlag(GetSavePartFlag()|SAVE_DETAIL_ATTRMERQUESTLIST);
//	}
//	//}
//
}

bool CPlayer::IsMerQuestInit(long lMercListID)
{
	return m_MerQuestSys.IsExistQuestList(lMercListID);
}

//����ٸ���������ݵ��ͻ���
void CPlayer::SendMercQuestDataToClient(long lMercListID)
{		
	//������Ϣ���ͻ���
	CMessage msg(MSG_S2C_MercQuest_UpdateData);
	msg.Add(m_MerQuestSys.GetCurQuestID());
	//��ȡ��ǰ�б����������
	set<long>quests;
	m_MerQuestSys.GetDisbandQuest(quests);
	long lsize=(long)quests.size();
	set<long>::iterator it=quests.begin();
	msg.Add(lsize);
	for (;it!=quests.end();it++)
	{
		msg.Add( (*it) );
	}
	//��ǰٸ������
	if (lMercListID==0)
	{
		msg.Add((long)0);
		msg.SendToPlayer(GetExID());
		return;
	}
	else
		msg.Add((long)1);

	msg.Add(lMercListID);
	if(lMercListID)
	{	
		CMercenaryQuestSys::tagMerQuest MerQuests;
		bool bResult = m_MerQuestSys.GetMercQuests(lMercListID,MerQuests);
		if(bResult)
		{
			time_t curTime;
			time(&curTime);
			long lUpdateTime = (long)(CQuestSystem::GetQuestListFreshTime(lMercListID)-(curTime-MerQuests.dwLastUpdateTime));
			msg.Add(lUpdateTime);
			msg.Add((WORD)MerQuests.Quests.size());
			vector<long>::iterator it = MerQuests.Quests.begin();
			for(;it !=  MerQuests.Quests.end();it++)
			{
				msg.Add((*it));
				if (m_RoleQueue.GetQuestFinishedRecord(*it) )
				{
					msg.Add((long)1);
				}
				else
				{
					msg.Add((long)0);
				}
			}
		}
		else
			msg.Add((long)0);
	}
	msg.SendToPlayer(GetExID());
}

//��ٸ���������
void CPlayer::OpenMercQuestInterface(long lMercListID)
{
	CMessage msg(MSG_S2C_MercQuest_OpenInterface);
	msg.Add(lMercListID);
	msg.Add(CQuestSystem::GetQuestListType(lMercListID));
	msg.SendToPlayer(GetExID());
}
void CPlayer::OpenShareQuestInterface()
{
	CMessage msg(MSG_S2C_SHARE_QUEST_OPEN);
	msg.SendToPlayer(GetExID());
}
//���͵�ǰ���ڽ��е�����ID
long CPlayer::GetTeamCurrQuest()
{
	CGUID guid=this->GetTeamID();
	GSTeam* pteam=GetOrganiCtrl()->GetGameTeamByID(guid);
	if (pteam)
	{
		if (NULL_GUID!=pteam->IsMember(m_guid))
		{
			return pteam->GetCurrShareQuest();
			/*CMessage msg(MSG_S2C_SHARE_CURRQUEST);
			msg.Add(pteam->GetCurrShareQuest());
			msg.SendToPlayer(GetExID());*/
		}	
	}	
	return 0;
}
void CPlayer::SetMasterQuest(long lQuestID)
{
	m_MerQuestSys.SetMasterQuestID(lQuestID);
	return;
}
//�ӳ����ö��鵱ǰ����ID
void CPlayer::SetTeamCurrQuest(long lQuestID)
{
	CGUID guid=this->GetTeamID();
	GSTeam* pteam=GetOrganiCtrl()->GetGameTeamByID(guid);
	if (pteam)
	{
		//��WS���͸��¶��鵱ǰ����ID��Ϣ
		if (NULL_GUID!=pteam->IsMaster(m_guid))
		{
			SetMercQuest(lQuestID,false);
			CMessage msg(MSG_S2W_SHARE_CURRQUEST);
			msg.Add(this->GetExID());
			msg.Add(guid);
			msg.Add(lQuestID);
			msg.Send(false);
		}
	}
}
//ɾ�չ����б��е�����
void CPlayer::DeleteTeamQuest(long lQuestID)
{
	CGUID guid=this->GetTeamID();
	GSTeam* pteam=GetOrganiCtrl()->GetGameTeamByID(guid);
	if (pteam)
	{
		if (NULL_GUID!=pteam->IsMember(m_guid))
		{
			if (pteam->FindShareQuest(lQuestID))
			{
				CMessage msg(MSG_S2W_SHARE_DELQUEST);
				long len = (long)strlen(GetName());
				msg.Add(len);
				msg.AddEx((void*)GetName(),len);
				msg.Add(m_guid);
				msg.Add(guid);
				msg.Send(false);
			}
		}
	}
}

bool CPlayer::UpdateTeamQuestList(long lQuestID)
{
	//�������ͽ��б��У����������ڷ������ͽ������б���
	if ( IsMercListQuest(lQuestID) && 
		m_MerQuestSys.FindDisbandQuest(lQuestID)==0 &&
		m_TeamID!=NULL_GUID)
	{
		//�Ƽ�����ܺ�Ϊһ������
		//SetMercQuest(lQuestID);
		//stRunScript sScript;
		//sScript.desShape = NULL;
		//CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(this->GetRegionGUID()));
		//if (pRegion==NULL)
		//	return false;
		//sScript.pRegion = pRegion;
		//sScript.srcShape=this;	
		//const char* strScript =CQuestSystem::GetAddQuestScript();
		//if (strScript == NULL)
		//	return false;
		//sScript.pszFileName = (char*)strScript;
		//RunScript( &sScript, const_cast<CHAR*>( GetGame() -> GetScriptFileData(strScript) ), lQuestID ,0);

		CRegion *pRegion = dynamic_cast<CRegion*>(GetGame()->FindRegion(this->GetRegionGUID()));
		if (pRegion==NULL)
			return false;
		const char* strScript =CQuestSystem::GetAddQuestScript();
		if (strScript == NULL)
			return false;

		Script::RunEnv env( this, NULL, pRegion );
		GetInst( ScriptSys ).RunScript( env, strScript );

		m_MerQuestSys.DisbandQuestRecord(lQuestID);
		return true;
	}
	return false;	
}
//ٸ������ˢ��ʱ�䳬ʱ
//void CPlayer::OnUpdateMercQuestTimeOut()
//{
//	ResetAllMercQuestData();
//	//ˢ���ϴν���򿪵�ٸ������
//	AcquireMercQuestList(GetLastAcquireMercListID());
//	//���͵��ͻ���
//	SendMercQuestDataToClient(GetLastAcquireMercListID());
//}

void CPlayer::OnCompleteQuest(long lQuestID)
{
	if (m_TeamID != NULL_GUID)
	{
		GSTeam* pTeam = GetOrganiCtrl()->GetGameTeamByID(m_TeamID);
		if (pTeam)
		{
			//�Ƕӳ�
			if (pTeam->IsMaster(m_guid)!=NULL_GUID)
			{
				//SetTeamCurrQuest(0);
				DeleteTeamQuest(lQuestID);
			}
			if (pTeam->GetCurrShareQuest() == lQuestID)
			{
				CMessage msg(MSG_S2W_SHARE_SETMEMBERQUEST);
				msg.Add(m_guid);
				msg.Add(m_TeamID);
				msg.Add(lQuestID);
				msg.Send(false);
			}
		}		
	}
}

bool CPlayer::IsCompleteQuest(long lQuestID)
{
	if (m_TeamID != NULL_GUID)
	{
		GSTeam* pTeam = GetOrganiCtrl()->GetGameTeamByID(m_TeamID);
		if(pTeam)
		{
			if (pTeam->IsCompleteQuest(m_guid,lQuestID))
			{
				return true;
			}
			else
				return false;
		}	
	}
	return false;
}

DWORD CPlayer::GetQuestOdds(long lQuestID)
{
	const CQuest* quest=CQuestSystem::GetQuest(lQuestID);
	if (quest)
	{
		return quest->GetAttribute().GetOdds();
	}
	return 0;
}

long CPlayer::CanAcceptMercQuest(long lQuestID)
{
	if (lQuestID == m_MerQuestSys.GetMasterQuestID())
	{
		return lQuestID;
	}
	return 0;
}
//����ٸ������
void CPlayer::SetMercQuest(long lQuestID,bool bcall)
{
	//���������,�����鹲���������0,�������ؽ�
	/*if (GetTeamCurrQuest() == lQuestID)
	{
		m_MerQuestSys.SetMasterQuestID(0);
	}*/
	long lQuestListID = m_MerQuestSys.OnAddQuest(lQuestID);
	if(lQuestListID==0)
	{	
		GSTeam* pTeam=GetOrganiCtrl()->GetGameTeamByID(this->GetTeamID());
		if (pTeam)
		{
			//�Ƕӳ������б��в����Ƿ��д�����
			if (pTeam->IsMaster(m_guid)!=NULL_GUID && !bcall)
			{
				if (pTeam->FindShareQuest(lQuestID))
				{
					m_MerQuestSys.SetCurQuestID(lQuestID);
					SetSavePartFlag(GetSavePartFlag()|SAVE_DETAIL_ATTRMERQUESTLIST);
				}
			}
			else
			{
				if (pTeam->GetCurrShareQuest()==lQuestID)
				{
					m_MerQuestSys.SetCurQuestID(lQuestID);
					SetSavePartFlag(GetSavePartFlag()|SAVE_DETAIL_ATTRMERQUESTLIST);
				}	
			}
		}	
	}
	else
	{
		//�������ڷ��������б���
		if (!IsDisbandQuest(lQuestID))
		{
			m_MerQuestSys.SetCurQuestID(lQuestID);
			SetSavePartFlag(GetSavePartFlag()|SAVE_DETAIL_ATTRMERQUESTLIST);
		}
		
	}
	SendMercQuestDataToClient(lQuestListID);
}

//��ҽ���GS�Ժ��ʼ�����ٸ����������
void CPlayer::IniMercQuest()
{
	//�жϵ�ǰӶ�������Ƿ��������б���
	if( GetQuestState(m_MerQuestSys.GetCurQuestID()) == eQS_No )
		m_MerQuestSys.SetCurQuestID(0);

}

//�Ƿ���ˢ��
long CPlayer::CanFreshMercQuest(long lMercListID)
{
	long type= CQuestSystem::GetQuestListType(lMercListID);
	DWORD Time= CQuestSystem::GetQuestListFreshTime(lMercListID);
	CMercenaryQuestSys::tagMerQuest mq;
	m_MerQuestSys.GetMercQuests(lMercListID,mq);

	time_t curTime;
	time(&curTime);

	//mq.Quests.size()==0 &&
	if (mq.Quests.size()==0 || mq.dwLastUpdateTime+ Time< DWORD(curTime))
	{
		return FT_YES;
	}

	if (type== 2)
	{
		return FT_NEEDGOODS;
	}

	return FT_NO;
}

//ˢ��
void CPlayer::FreshMercQuest(long lMercListID,bool usegoods)
{
	int FreshType= CanFreshMercQuest(lMercListID);
	if (FreshType==FT_YES)
	{
		AcquireMercQuestList(lMercListID);
		SendMercQuestDataToClient(lMercListID);
		//OpenMercQuestInterface(lMercListID);
		return;
	}
	else if (FreshType== FT_NEEDGOODS )
	{
		map<long,long>& mapItem=CQuestSystem::GetMerItem();
		map<long,long>::iterator it = mapItem.begin();
		for (;it!=mapItem.end();it++)
		{
			char* goodsname= (char*)GoodsSetup::QueryGoodsOriginalName((DWORD)it->second);
			if (goodsname!=NULL)
			{
		if (usegoods && DelGoods(goodsname,1))
		{
			AcquireMercQuestList(lMercListID,usegoods);
			SendMercQuestDataToClient(lMercListID);
			return;
		}
			}
		}

		//����ʧ����Ϣ
	}
	//����ʧ����Ϣ
}

//�ж��Ƿ���ٸ���б�����
bool CPlayer::IsMercListQuest(long lQuestID)
{
	//�����б��в����ڴ�����
	if (m_MerQuestSys.IsExistQuest(lQuestID)==0)
	{
		return false;
	}
	return true;
}

bool CPlayer::IsDisbandQuest(long lQuestID)
{
	//�����������б�Ҳ�����ڷ��������б���
	if (m_MerQuestSys.FindDisbandQuest(lQuestID)!=0)
	{
		return true;
	}
	else
		return false;
}

void CPlayer::ShowTalkBox(DWORD dwQuestID,long lTextStep)
{
	CMessage msg(MSG_S2C_QUEST);
    msg.Add((BYTE)QS::QS_TALK);
	msg.Add((LONG)dwQuestID);
	msg.Add(lTextStep);
	msg.SendToPlayer(GetExID());	
}
//�����������һ���ű�
void CPlayer::PlayerRunScript(const char *strScriptName)
{
	if(strScriptName == NULL)	return;

	Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ) );
	GetInst( ScriptSys ).RunScript( env, strScriptName );
}

long CPlayer::GetCurrentMasterRoleID()
{
	list<CQuestRecord*>* pRoleList= m_RoleQueue.GetQuestRecordList();
	if (pRoleList!= NULL)
	{
		for (list<CQuestRecord*>::iterator it= pRoleList->begin(); it!= pRoleList->end(); it++)
		{
			const CQuest* pQuest =CQuestSystem::GetQuest((*it)->GetQuestID());
			if (pQuest== NULL) break;
			const CQuestAttribute& pAttri= pQuest->GetAttribute();
			if (pAttri.GetQuestType()== 1) return (*it)->GetQuestID();
		}
	}
	return 0;
}

FLOAT CPlayer::GetWeaponModifier( LONG lTargetLevel )
{
	FLOAT fResult = 1.0f;
	GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
	if( pSetup )
	{
		DWORD dwWeaponModifier = 0;
		CGoods* pGoods = m_cEquipment.GetGoods( EC_E_WEAPON );
		if( pGoods )
		{
			dwWeaponModifier = pGoods -> GetAddonPropertyValues( GAP_WEAPON_DAMAGE_LEVEL, 1 );
		}

		long lTemp = ( dwWeaponModifier - lTargetLevel );
		if (lTemp<0)
			lTemp = 0;
		fResult = lTemp / pSetup -> m_fWeaponPowerModifier;

		if( fResult > 1.0f )
		{
			fResult = 1.0f;
		}
		if( fResult < pSetup -> m_fWeaponMinPowerModifier )
		{
			fResult = pSetup -> m_fWeaponMinPowerModifier;
		}
	}
	return fResult;
}

DWORD CPlayer::GetWeaponDamageLevel() {		// ��ȡ��ǰʹ�õ������Ĺ���ǿ��
	DWORD dwWeaponDamageLevel = 0;
	CGoods *pGoods = m_cEquipment.GetGoods(EC_E_WEAPON); 

	if(!pGoods) return 0;

	dwWeaponDamageLevel = pGoods->GetAddonPropertyValues( GAP_WEAPON_DAMAGE_LEVEL, 1 );	


	return dwWeaponDamageLevel;	
}

//
bool CPlayer::AddOneRecord(long roleid)
{
	const CQuest* quest=CQuestSystem::GetQuest(roleid);
	const CQuestStep* step=0;

	if (quest==0)
	{
		return false;
	}
	step=quest->GetStep();

	if (step==0)
	{
		return false;
	}

	CQuestRecord* record=new CQuestRecord(roleid, step->GetStepID());

	if(!AddQuest(record))
	{
		delete record;
		//���������Ϣ�� �������񲻳ɹ�
		return false;
	}
	return true;
}

bool CPlayer::SetChapter(long num)
{ 
	m_RoleQueue.SetChapter(num);
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRQUESTLIST);
	return true;
}

bool  CPlayer::AddFinishedRoleRecord(long index)
{
	if( m_RoleQueue.AddFinishedQuestRecord(index))
	{
		CMessage msg(MSG_S2C_QUEST);//MSG_S2C_Quest_UpdateRecordData
        msg.Add((BYTE)QS::QS_UPDATE_RECORD);
		msg.Add(index);
		msg.SendToPlayer(GetExID());
		GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRQUESTLIST);
		return true;
	}
	return false;
}

bool CPlayer::DelFinishedRoleRecord(long index)
{
	return m_RoleQueue.DelFinishedQuestRecord(index);
}

eQuestTextId CPlayer::GetQuestStateId(long QuestId)
{
    /// û����ȡ
    eQuestState st =  GetQuestState( QuestId ) ;
    if ( st == eQS_No )
    {
         return eNoQuest;
    }
    /// ��ȡ�Ѿ����
    else  if( st == eQS_Complete )
    {
         return eComQuest;
    }
    /// �ѽ�
    else 
    {
        const CQuestStep * step = GetStep( QuestId , 1 );

        if ( step  )
        {
            for ( int i = 0 ; i < step->GetStepAimNum() ; i++ )
            {
                const CQuestAim * aim = step->GetAimHead();
                if ( aim )
                {
                    if( aim->GetAimNum() > GetRoleParam(QuestId,1,aim->GetID()-1 ) )
                        return eNoComQuest_No;
                }
            }
            return eNoComQuest_Fin;
        }
    }
    return   eNoQuestId;
}

const char*  CPlayer::GetQuestTextId(long QuestId,long QState)
{
    return AppFrame::LoadQuestTextRes(QuestId,QState);
}

//////////////////////////////////////////////////////////.. ����ϵͳ���������ֵ�Ĳ�ѯ
const CQuest*	CPlayer::GetQuest(long RoleID)
{
	return CQuestSystem::GetQuest(RoleID);
}

const CQuestStep* CPlayer::GetStep(long RoleID, long StepID)				//Ѱ�Ҳ���
{
	const CQuest* pQuest=CQuestSystem::GetQuest(RoleID);
	if (pQuest==NULL)
	{
		return NULL;
	}
	return pQuest->GetStep(StepID);
}
const CQuestAim*  CPlayer::GetAim(long RoleID, long StepID,long AimID)	//Ѱ������Ŀ��
{
	const CQuest* pQuest=CQuestSystem::GetQuest(RoleID);
	if (pQuest==NULL)
	{
		return NULL;
	}
	const CQuestStep*pStep=pQuest->GetStep(StepID);
	if (pStep==NULL)
	{
		return NULL;
	}
	return pStep->GetAim(AimID);

}

const CQusetEncouragement *CPlayer::GetQuestEncouragement(long RoleID,long Index)
{
	CQusetEncouragement* encouragement=CQuestSystem::GetEncouragementHead(RoleID);//quest->GetEncouragementHead();
	int EncouragementSum=CQuestSystem::GetEncouragementSum(RoleID);
	if (Index > EncouragementSum)
	{
		return NULL;
	}
	for (int a=0; a<Index-1  ; a++)
	{
		encouragement=encouragement->next;
	}
	return encouragement;
}

const CQusetEncouragement *CPlayer::GetStepEncouragement(long RoleID,long  StepID,long Index)
{
// 	const CQuestStep* step=GetStep(RoleID,StepID);
// 
// 	if (step==NULL)
// 	{
// 		return NULL;
// 	}
// 
// 	const CQusetEncouragement* encouragement=step->GetRolePropHead();
// 	int EncouragementSum=step->GetRolePropSum();
// 
// 	if (Index> EncouragementSum)
// 	{
// 		return NULL;
// 	}
// 
// 	for (int a=0; a<Index-1; a++)
// 	{
// 		encouragement=encouragement->next;
// 	}
    return NULL;
}

//ȡ���ı�����
long CPlayer::GetTextType(long RoleID)
{
	const CQuest* quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetTextType();
	}
	return 0;
}
//attribute:
//LONG AttributeGetID(LONG RoleID)const{return m_lID;}
//ȡ����������
const char* CPlayer::GetAttributeRoleName(long RoleID)
{
    char   quest[32];
    sprintf_s(quest,"Q%6d",RoleID);
    return AppFrame::GetText(quest);
}
//�Ƿ񱻼�¼
long CPlayer::GetAttributeBeRecord(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetBeRecord();
	}
	return 0;
}
//��������
long CPlayer::GetAttributeQuestType(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetQuestType();
	}
	return 0;
}
//�����Ƿ���Թ���
long CPlayer::GetAttributeQuestIsShare(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetQuestIsShare();
	}
	return 0;
}
//�Ƿ���ʾ
long CPlayer::GetAttributeShowAble(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetShowAble();
	}
	return 0;
}
//�Ƿ��ܷ�����
long CPlayer::GetAttributeRepeated(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetRepeated();
	}
	return 0;
}
//�����Ƿ�ʧ��
long CPlayer::GetAttributeDeadLost(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetDeadLost();
	}
	return 0;
}
//����ʧ�ܺ��ܷ����½�
long CPlayer::GetAttributeLostRepeated(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetLostRepeated();
	}
	return 0;
}
//��С�ȼ�����
long CPlayer::GetAttributeMinGrade(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetMinGrade();
	}
	return 0;
}
//���ȼ�����
long CPlayer::GetAttributeMaxGrade(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetMaxGrade();
	}
	return 0;
}
//ְҵ����
bool CPlayer::GetAttributeOccupationRestrict(long RoleID,int index)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetOccupationRestrict(index);
	}
	return false;
}
//������
long CPlayer::GetAttributeOdds(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetOdds();
	}
	return 0;
}
// �Ա�����
bool CPlayer::GetAttributeQuestSex(long QuestID)
{
	const CQuest * quest=GetQuest(QuestID);
	if (quest)
	{
		BYTE lSex = quest->GetAttribute().GetQuestSex();
        if ( lSex == 2 )
            return true;
        return GetSex() == lSex ;
//		return quest->GetAttribute().GetEngageArmsRepute();
	}
	return false;
}
// ��������
bool CPlayer::GetAttributeQuestSkill(long QuestID)
{
    const CQuest * quest=GetQuest(QuestID);
    if (quest)
    {
        map<long,tagSkill>::iterator it = m_Skills.find(quest->GetAttribute().GetReqSkill());
        if (it!=m_Skills.end())
        {
            return true;
        }
    }
    return false;
}
// Я������
bool CPlayer::GetAttributeTotePet(long QuestID)
{
    const CQuest * quest =GetQuest(QuestID);
    if ( quest )
    {
        long  PetID = quest->GetAttribute().GetTotePet();
    }
    return false;
}

//��һ�����id
long CPlayer::GetAttributeNextRoleID(long RoleID)
{
	const CQuest* quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetNextRoleID();
	}

	return 0;
}
//�½���
long CPlayer::GetAttributeChapterNum(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetChapterNum();
	}
	return 0;
}

//
long CPlayer::GetAttributeFinishEffect(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetAttribute().GetFinishEffect();
	}
	return 0;
}

//ȡ��index��Ӧ��ǰ������
long CPlayer::GetLabelPrefix(long RoleID,long Index)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetLabelPrefix().GetLabelPrefix(Index);
	}
	return 0;
}
//ȡ��ǰ����������
long CPlayer::GetLabelSum(long RoleID)
{
	const CQuest * quest=GetQuest(RoleID);
	if (quest)
	{
		return quest->GetLabelPrefix().GetLabelSum();
	}
	return 0;
}

//Ŀ��ģ������
long	CPlayer::GetAimAMTType(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetAMTType();
	}
	return 0;
}
//Ŀ������
long CPlayer::GetAimType(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetAimType();
	}
	return 0;
}
//ȡ��Ŀ��id
long CPlayer::GetAimId(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetAimId();
	}
	return 0;
}
//ȡ��Ŀ������
long CPlayer::GetAimNum(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetAimNum();
	}
	return 0;
}
//ȡ�� ��������
long CPlayer::GetAimParameIndex(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetParameIndex();
	}
	return 0;
}
//����������Ʒ���������ͣ�����Ʒ��
long CPlayer::GetDropFrom(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetDropFrom();
	}
	return 0;
}
//����
long CPlayer::GetDropOdds(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetDropOdds();
	}
	return 0;
}

const char* CPlayer::GetAimName(long RoleID,long StepID,long AimID)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetName();
	}
	return 0;
}

//ȡ�ñ�������
long CPlayer::GetPrepareParam(long RoleID,long StepID,long AimID,long index)
{
	const CQuestAim* aim=GetAim(RoleID,StepID,AimID);
	if (aim)
	{
		return aim->GetPrepareParam(index);
	}
	return 0;
}


long CPlayer::GetQuestEncouragementSum(long RoleID)
{
	return CQuestSystem::GetEncouragementSum(RoleID);
}

//ȡ�ý���������
const char* CPlayer::GetSetpEncouragementName(long RoleID,long Step,long Index)
{
	const CQusetEncouragement* encouragement=GetStepEncouragement(RoleID,Step,Index);
	if (encouragement==NULL)
	{
		return NULL;
	}
	return encouragement->GetName();
}
const char* CPlayer::GetQuestEncouragementName(long RoleID,long Index)
{
	const CQusetEncouragement* encouragement=GetQuestEncouragement(RoleID,Index);
	if (encouragement==NULL)
	{
		return NULL;
	}
	return encouragement->GetName();
}
//ȡ�ý�������
long CPlayer::GetStepEncouragementType(long RoleID,long Step,long Index)
{
	const CQusetEncouragement* encouragement=GetStepEncouragement(RoleID,Step,Index);
	if (encouragement==NULL)
	{
		return 0;
	}
	return encouragement->GetType();
}
long CPlayer::GetQuestEncouragementType(long RoleID,long Index)
{
	const CQusetEncouragement* encouragement=GetQuestEncouragement(RoleID,Index);
	if (encouragement==NULL)
	{
		return 0;
	}
	return encouragement->GetType();
}
//�����ľ���id
long CPlayer::GetStepEncouragementID(long RoleID,long Step,long Index)
{
	const CQusetEncouragement* encouragement=GetStepEncouragement(RoleID,Step,Index);
	if (encouragement==NULL)
	{
		return 0;
	}
	return encouragement->GetID();
}
long CPlayer::GetQuestEncouragementID(long RoleID,long Index)
{
	const CQusetEncouragement* encouragement=GetQuestEncouragement(RoleID,Index);
	if (encouragement==NULL)
	{
		return 0;
	}
	return encouragement->GetID();
}
//��������
long CPlayer::GetStepEncouragementNum(long RoleID,long Step,long Index)
{
	const CQusetEncouragement* encouragement=GetStepEncouragement(RoleID,Step,Index);
	if (encouragement==NULL)
	{
		return 0;
	}
	return encouragement->GetNum();
}
long CPlayer::GetQuestEncouragementNum(long RoleID,long Index)
{
	const CQusetEncouragement* encouragement=GetQuestEncouragement(RoleID,Index);
	if (encouragement==NULL)
	{
		return NULL;
	}
	return encouragement->GetNum();
}
//�����ĸ�����
long CPlayer::GetStepEncouragementCell(long RoleID,long Step)
{
	const CQuestStep* step=GetStep(RoleID,Step);
	if (step==NULL)
	{
		return 0;
	}
	return step->GetRolePropCell();
}
long CPlayer::GetQuestEncouragementCell(long RoleID)
{
	 return CQuestSystem::GetEncouragementCell(RoleID);
}
//ִ�н���
bool CPlayer::PerformEncouragement(long RoleID)
{
	const CQuest* quest=GetQuest(RoleID);
	return false;
}
bool CPlayer::PerformEncouragement(long RoleID,long StepID)
{
	const CQuestStep* step=GetStep(RoleID,StepID);
	return false;
}

//�����������
long CPlayer::GetStepSum(long RoleID)
{
	const CQuest*  quest=GetQuest(RoleID);
	if (quest == NULL)
	{
		return 0;
	}
	return quest->GetStepSum();
}

long CPlayer::GetStepAimNum(long RoleID,long StepID)
{
	const CQuestStep* step=GetStep(RoleID,StepID);
	if (step==NULL)
	{
		return 0;
	}
	return step->GetStepAimNum();
}
//����ϵͳ���
long CPlayer::FindRoleIsInSystem(long RoleID)
{
	const CQuest* quest=GetQuest(RoleID);
	if (quest==NULL)
	{
		return 0;
	}
	return 1;
}

bool CPlayer::FindRoleStepIsInSystem(long RoleID,long StepID)
{
	const CQuest* quest=GetQuest(RoleID);
	if (quest==NULL)
	{
		return false;
	}
	const CQuestStep* step=quest->GetStep(StepID);
	if (step==NULL)
	{
		return false;
	}
	return true;
}
//////////////////////////////////////////////////////////..end



//���һ��������ť��ʾ
long CPlayer::AddHelpHint(const char* pszScript,long lFutureDelTime)
{
	if(NULL == pszScript)	return 0;
	//��������ƶ���Ŀ����ɾ����ǰ�ߵ���ʾ
	long lHintID = 0;
	if( (long)m_HelpHints.size() > GlobalSetup::GetSetup()->lMaxHelpHintNum)
	{
		lHintID = m_HelpHints.begin()->first;
		//�Ƴ�һ��������ʾ
		RemoveHelpHint(lHintID);
	}

	//��Ӱ�����ť��ʾ
	lHintID = ++m_nHintCounter;

	tagTimerVar* pTimvar = OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_PlayerHint;
	pTimvar->lvar = lHintID;
	long lTimerID = TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime()+lFutureDelTime,0);
	if(lTimerID != 0)
	{
		tagHint hint = {pszScript,lTimerID,random(1000)};
		m_HelpHints[lHintID] = hint;

		CMessage msg(MSG_S2C_OTHER_SETHELPHINT);
		//���
		msg.Add((BYTE)0);
		msg.Add(lHintID);
		msg.Add(hint.lFlag);
		msg.Add(lFutureDelTime);
		msg.SendToPlayer(GetExID());
	}
	else
	{
		Log4c::Warn(ROOT_MODULE, GetText("GS_PLAYER_4"));//��Ӱ�����ʾ��ťʱ,���ƶ�ʱ������
		lHintID = 0;
	}
	return lHintID;
}

//�Ƴ�һ��������ʾ
void CPlayer::RemoveHelpHint(long lHintID)
{
	itHint it = m_HelpHints.find(lHintID);
	if( it != m_HelpHints.end())
	{
		tagHint &hint = (*it).second;
		CMessage msg(MSG_S2C_OTHER_SETHELPHINT);
		//�Ƴ�
		msg.Add((BYTE)1);
		msg.Add((*it).first);
		msg.SendToPlayer(GetExID());
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
		m_HelpHints.erase(it);
	}
}

//����ҵ��������ʾ��ť
void CPlayer::OnPlayerHitHelpHint(long lHintID,long lFlag)
{
	string strScript("");
	itHint it = m_HelpHints.find(lHintID);
	if( it != m_HelpHints.end())
	{
		tagHint &hint = (*it).second;
		//��־λ��ͬ,�򷵻�
		if(hint.lFlag != lFlag)
			return;		
		strScript = hint.strScript;
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
		m_HelpHints.erase(it);
	}

	//���нű�
	PlayerRunScript((char*)strScript.c_str());
}

//���һ��������ʾ
long CPlayer::AddInterfaceHint(const char* pszScript,long lFutureDelTime,
							   const char* pszInterName,long lX,long lY,long lWidth,long lHeight)
{
	if(NULL == pszScript || NULL == pszInterName)	return 0;

	//��ӽ�����ʾ
	long lHintID = ++m_nHintCounter;

	tagTimerVar* pTimvar = OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_PlayerHint;
	pTimvar->lvar = lHintID;
	long lTimerID = TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime()+lFutureDelTime,0);
	if(lTimerID != 0)
	{
		tagHint hint = {pszScript,lTimerID,random(1000)};
		m_InterfaceHints[lHintID] = hint;

		CMessage msg(MSG_S2C_OTHER_SETINTERFACEHINT);
		//���
		msg.Add((BYTE)0);
		msg.Add(lHintID);
		msg.Add(hint.lFlag);
		msg.Add(lFutureDelTime);
		msg.Add(pszInterName);
		msg.Add(lX);
		msg.Add(lY);
		msg.Add(lWidth);
		msg.Add(lHeight);
		msg.SendToPlayer(GetExID());
	}
	else
	{
		Log4c::Warn(ROOT_MODULE, GetText("GS_PLAYER_5"));//��ӽ�����ʾʱ,���ƶ�ʱ������
		lHintID = 0;
	}
	return lHintID;
}

//�Ƴ�һ��������ʾ
void CPlayer::RemoveInterfaceHint(long lHintID)
{
	itHint it = m_InterfaceHints.find(lHintID);
	if( it != m_InterfaceHints.end())
	{
		tagHint &hint = (*it).second;
		CMessage msg(MSG_S2C_OTHER_SETINTERFACEHINT);
		//�Ƴ�
		msg.Add((BYTE)1);
		msg.Add((*it).first);
		msg.SendToPlayer(GetExID());
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
		m_InterfaceHints.erase(it);
	}
}
//����
//bool CPlayer::SetMapMail(CMail *pMail)
//{
//	time_t t_CurrentTime = time(NULL);
//	map<CGUID,CMail*>::iterator it=m_mapMail.find(pMail->GetExID());
//	if (it==m_mapMail.end())
//	{
//		m_mapMail[pMail->GetExID()] = pMail;		
//	}
//	else
//	{
//		it=m_mapMail.erase(it);
//		m_mapMail[pMail->GetExID()] = pMail;
//	}
//
//	if (pMail->GetRemainTime() - t_CurrentTime >0)
//		return true;
//	else		
//		return false;
//}

//����ҵ��������ʾ
void CPlayer::OnPlayerHitInterfaceHint(long lHintID,long lFlag)
{
	string strScript("");
	itHint it = m_InterfaceHints.find(lHintID);
	if( it != m_InterfaceHints.end())
	{
		tagHint &hint = (*it).second;
		//��־λ��ͬ,�򷵻�
		if(hint.lFlag != lFlag)
			return;		
		strScript = hint.strScript;
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
		m_InterfaceHints.erase(it);
	}
	//���нű�
	PlayerRunScript((char*)strScript.c_str());
}

//����ʾ��ʱʱ
void CPlayer::OnHintTimeOut(long lHintID)
{
	itHint it = m_HelpHints.find(lHintID);
	if( it != m_HelpHints.end())
	{
		m_HelpHints.erase(it);
		CMessage msg(MSG_S2C_OTHER_SETHELPHINT);
		//�Ƴ�
		msg.Add((BYTE)1);
		msg.Add(lHintID);
		msg.SendToPlayer(GetExID());
		return;
	}

	it = m_InterfaceHints.find(lHintID);
	if( it != m_InterfaceHints.end())
	{
		m_InterfaceHints.erase(it);
		CMessage msg(MSG_S2C_OTHER_SETINTERFACEHINT);
		//�Ƴ�
		msg.Add((BYTE)1);
		msg.Add(lHintID);
		msg.SendToPlayer(GetExID());
		return;
	}
	return;
}

//���������ʾ
void CPlayer::ClearAllHint()
{
	itHint it = m_HelpHints.begin();
	for(; it != m_HelpHints.end();it++)
	{
		tagHint &hint = (*it).second;
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
	}
	m_HelpHints.clear();

	it = m_InterfaceHints.begin();
	for(; it != m_InterfaceHints.end();it++)
	{
		tagHint &hint = (*it).second;
		//ɾ����ʱ��
		if(hint.lTimerID != 0)
		{
			TIMER_CANCEL(hint.lTimerID);
		}
	}
	m_InterfaceHints.clear();
}

//! [2007-7-24 move] ��Ӷ����Ա��Ϣͬ������
void CPlayer::SetLevel(WORD l, BOOL bScriptSet)
{ 
	if(l <= 0 || l > PlayerList::GetLevelNum() )
	{
		char strGUID[50]="";
		m_guid.tostring(strGUID);
		Log4c::Warn(ROOT_MODULE,FormatText("GS_PLAYER_6",l,GetName(),strGUID));//������ҵȼ�(%d)����(Name:%s,GUID:%s)��
		return;
	}
	LONG lOldLevel = m_Property.wLevel;
	m_Property.wLevel = l;

	if(bScriptSet)
		GetLogicLogInterface()->logT100_player_level_log(this, lOldLevel, "�ű�ֱ�����õȼ�");
}

void  CPlayer::SetOccuLvl(eOccupation byOccu,BYTE l, BOOL bScriptSet)
{
	if(l <= 0 || l > PlayerList::GetOccuLevelNum(byOccu) ||
		byOccu < OCC_Novice || byOccu >= OCC_Max)
	{
		char strGUID[50]="";
		m_guid.tostring(strGUID);
		Log4c::Warn(ROOT_MODULE,FormatText("GS_PLAYER_7",byOccu,l,GetName(),strGUID));//�������ְҵ(%d)�ȼ�(%d)����(Name:%s,GUID:%s)��
		return;
	}
	LONG lOldOccuLevel = m_Property.byOccuLvl[byOccu];
	m_Property.byOccuLvl[byOccu] = l;

	if(bScriptSet)
		GetLogicLogInterface()->logT101_player_Occulevel_log(this, lOldOccuLevel, "GS_PLAYER_8");//�ű�ֱ�����õȼ�
}

void CPlayer::SetHP(DWORD l)
{
	m_Property.dwHp = l;
	if(m_Property.dwHp > GetMaxHP())
	{
		m_Property.dwHp = GetMaxHP();
	}
	if (GetState()==STATE_DIED && l>0)
	{
		m_Property.dwHp = 0;
	}

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrHp);
	}
}

void CPlayer::SetMP(WORD l)
{
	m_Property.wMp = l;
	if(m_Property.wMp > GetMaxMP()) 
	{
		m_Property.wMp = GetMaxMP();
	}
	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrMp);
	}
}

void CPlayer::SetEnergy(WORD l)
{
	m_Property.wEnergy = l;
	if(m_Property.wEnergy > GetMaxEnergy())
	{
		m_Property.wEnergy = GetMaxEnergy();
	}
}

void CPlayer::SetPKCP( long v )
{
	m_Property.lCurPKCP = v;
	long lMaxPKCP = GetMaxPKCP();
	if( m_Property.lCurPKCP > lMaxPKCP )
	{
		m_Property.lCurPKCP = lMaxPKCP;
	}

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_CurrEnergy);
	}
}

long CPlayer::GetMaxPKCP()
{
	return GetAttribute( "C_MaxPKCP" );
}

void CPlayer::SetBaseMaxPKCP( long v ) 
{ 
	m_BaseFightProperty.lMaxPKCP = v; 
	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxEnergy);
	}
}

//long CPlayer::GetAllEquipTimesCP()
//{
//	typedef map<eEquipType, CGoods*> EquipTableT;
//	EquipTableT *pEquipList = GetEquipmentGoodsMap();
//	long cpvalue = 0;
//	for( EquipTableT::iterator it = pEquipList->begin(); it != pEquipList->end(); ++ it )
//	{
//		CGoods *pGoods = it->second;
//		if( pGoods != NULL )
//		{
//			if( pGoods->HasAddonProperty( CGoodsBaseProperties::GAP_TIMES_MAX_PKCP ) )
//			{
//				cpvalue += pGoods->GetAddonPropertyValues( CGoodsBaseProperties::GAP_TIMES_MAX_PKCP, 1 );
//			}
//		}
//	}
//	return cpvalue;
//}

void CPlayer::SetMaxHP(DWORD l)
{
	m_mapDirtyAttrData.insert("wMaxHP");
	m_FightProperty.dwMaxHp = l; 
	if(GetHP()>GetMaxHP()) 
	{
		SetHP(GetMaxHP());
	}

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxHp);
	}
}

void CPlayer::SetMaxMP(WORD l)
{
	m_mapDirtyAttrData.insert("wMaxMP");
	m_FightProperty.wMaxMp = l;
	if(GetMP()>GetMaxMP())
	{
		SetMP(GetMaxMP());
	}

	GSTeam *pTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->OnPlayerIdioinfoChange(GetExID(), eMIS_MaxMp);
	}
}

void CPlayer::SetMaxEnergy(WORD l)
{
	m_mapDirtyAttrData.insert("wMaxEnergy");
	m_FightProperty.wMaxEnergy = l;
	if(GetEnergy()>GetMaxEnergy()) 
	{
		SetEnergy(GetMaxEnergy());
	}
}


void CPlayer::OnAddBuff(DWORD dwStateID, DWORD dwBuffLevel)
{
	CMoveShape::OnAddBuff(dwStateID, dwBuffLevel);
	GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->SendMemberBuffToAll(this, dwStateID, dwBuffLevel, FALSE);
	}
}

void CPlayer::OnDelBuff(DWORD dwStateID, DWORD dwBuffLevel)
{
	CMoveShape::OnDelBuff(dwStateID, dwBuffLevel);
	GSTeam *pTeam = GetOrganiCtrl()->GetGameTeamByID(GetTeamID());
	if(NULL != pTeam)
	{
		pTeam->SendMemberBuffToAll(this, dwStateID, dwBuffLevel, TRUE);
	}
}

void CPlayer::SetRecruitState(bool bIsRecruiting, char* pRecruitName, char* pPassword)
{
	m_RecruitState.bIsRecruiting = bIsRecruiting;

	CMessage msg(MSG_S2C_TEAM_RECRUIT);


	do 
	{
		msg.Add(GetExID());
		msg.Add((long)((bIsRecruiting)? 1 : 0));
		if (!bIsRecruiting)
		{
			break;
		}

		if(NULL != pRecruitName)
		{
			long lStrSize = (long)strlen(pRecruitName);
			lStrSize = (lStrSize < MAX_RECRUIT_TEXT_SIZE ) ? lStrSize : MAX_RECRUIT_TEXT_SIZE - 1;
			memmove(m_RecruitState.szRecruitName, pRecruitName, MAX_RECRUIT_TEXT_SIZE - 1);

			msg.Add(m_RecruitState.szRecruitName);
		}
		else
		{
			m_RecruitState.szRecruitName[0] = 0;
		}

		long lUsePassword = 0;

		if (NULL != pPassword)
		{
			long lStrSize = (long)strlen(pPassword);
			lStrSize = (lStrSize < MAX_RECRUIT_PWD_SIZE) ? lStrSize : MAX_RECRUIT_PWD_SIZE - 1;
			memmove(m_RecruitState.szPassword, pPassword, MAX_RECRUIT_PWD_SIZE - 1);
			if(0 != strlen(m_RecruitState.szPassword))
			{
				lUsePassword = 1;
			}
		}
		else
		{
			m_RecruitState.szPassword[0] = 0;
		}

		msg.Add(lUsePassword);

	} while(false);


	msg.SendToRegion( GetGame()->FindRegion(GetRegionGUID()) );

}

tagRecruitState& CPlayer::GetRecruitState(void)
{
	return m_RecruitState;
}
// ------------------------------------------------------------
// ���ýӿ�
// ------------------------------------------------------------
//��������
void CPlayer::SetMaxStamina(WORD l)			
{
	m_mapDirtyAttrData.insert("C_MaxStamina"); 
	m_FightProperty.wMaxStamina = l;
	if(GetStamina()>GetMaxStamina()) SetStamian(GetMaxStamina());
}


//��С������
void CPlayer::SetMinAtk(WORD l)				
{
	m_mapDirtyAttrData.insert("C_MinAtk"); 
	m_FightProperty.wMinAtk = l;
}
//��󹥻���
void CPlayer::SetMaxAtk(WORD l)	
{
	m_mapDirtyAttrData.insert("C_MaxAtk"); 
	m_FightProperty.wMaxAtk = l;
}
//��������
void CPlayer::SetHit(WORD l)
{
	m_mapDirtyAttrData.insert("C_Hit"); 
	m_FightProperty.wHit = l;
}
//�������
void CPlayer::SetDef(WORD l)			
{
	m_mapDirtyAttrData.insert("C_Def"); 
	m_FightProperty.wDef = l;
}
//����
void CPlayer::SetDodge(WORD l)		
{
	m_mapDirtyAttrData.insert("C_Dodge");
	m_FightProperty.wDodge = l;
}
//Ԫ�ؿ���
void CPlayer::SetElementResistant(long lpos,WORD l)	
{
	string name;
	switch(lpos)
	{
	case ET_Fire:
		name = "E_ElemDef0";
		break;
	case ET_Water:
		name = "E_ElemDef1";
		break;
	case ET_Earth:
		name = "E_ElemDef2";
		break;
	case ET_Wind:
		name = "E_ElemDef3";
		break;
	case ET_Dark:
		name = "E_ElemDef4";
		break;
	case ET_Holy:
		name = "E_ElemDef5";
		break;
	}
	m_mapDirtyAttrData.insert(name);
	m_FightProperty.wElemDef[lpos] = l;
}
//�����ָ��ٶ�
void CPlayer::SetHpRecoverSpeed(WORD l)	
{
	m_mapDirtyAttrData.insert("C_HpRecoverSpeed"); 
	m_FightProperty.wHpRecoverSpeed = l;
}
//�����ָ��ٶ�
void CPlayer::SetMpRecoverSpeed(WORD l)
{
	m_mapDirtyAttrData.insert("C_MpRecoverSpeed");
	m_FightProperty.wMpRecoverSpeed = l;
}
//�����ָ��ٶ�
void CPlayer::SetStamRecoverSpeed(WORD l) 
{
	m_mapDirtyAttrData.insert("C_MaxHP"); 
	m_FightProperty.wStamRecoverSpeed = l;
}
//����
void CPlayer::SetStr(short s)
{
    m_mapDirtyAttrData.insert("C_Str");
    m_FightProperty.wStr = s;
}
//��
void CPlayer::SetDex(short s)
{
    m_mapDirtyAttrData.insert("C_Dex");
    m_FightProperty.wDex = s;
}
//����
void CPlayer::SetCon(short s)
{
    m_mapDirtyAttrData.insert("C_Con");
    m_FightProperty.wCon = s;
}
//��־
void CPlayer::SetIntellect(short s)
{
    m_mapDirtyAttrData.insert("C_Intellect");
    m_FightProperty.wIntellect = s;
}
//����
void CPlayer::SetSpiritualism(short s)
{
    m_mapDirtyAttrData.insert("C_Spiritualism");
    m_FightProperty.wSpiritualism = s;
}
//Ǳ�ܵ�
void CPlayer::SetPoint(short s)
{
    m_mapDirtyAttrData.insert("C_Point");
    m_FightProperty.wPoint = s; 
}

//��
void CPlayer::SetFightBlock(WORD l)	
{
	m_mapDirtyAttrData.insert("C_Block"); 
	m_FightProperty.wFightBlock = l;
}
//�м�
void CPlayer::SetParry(WORD l)	
{
	m_mapDirtyAttrData.insert("C_Parry");
	m_FightProperty.wParry = l;
}
//����
void CPlayer::SetCharm(WORD l)	
{
	m_mapDirtyAttrData.insert("C_Charm"); 
	m_FightProperty.wCharm = l;
}
//רע
void CPlayer::SetAbsorb(WORD l)	
{
	m_mapDirtyAttrData.insert("C_Absorb");
	m_FightProperty.wAbsorb = l;
}
//ħ������
void CPlayer::SetMdef(WORD l)		
{
	m_mapDirtyAttrData.insert("C_Mdef"); 
	m_FightProperty.wMdef = l;
}
//������
void CPlayer::SetCri(WORD l)	
{
	m_mapDirtyAttrData.insert("C_Cri"); 
	m_FightProperty.wCri = l;
}
//ʩ���ٶ�
void CPlayer::SetMAtSpeed(WORD l)	
{
	m_mapDirtyAttrData.insert("C_MAtkSpeed");
	m_FightProperty.wMAtkSpeed = l;
}
//Buff����ǿ��
void CPlayer::SetBuffPower(WORD l)	
{
	m_mapDirtyAttrData.insert("C_BuffPower"); 
	m_FightProperty.wBuffPower = l;
}
//ħ��������
void CPlayer::SetMAtk(WORD l)	
{
	m_mapDirtyAttrData.insert("C_MAtk"); 
	m_FightProperty.wMAtk = l;
}
//ħ��������
void CPlayer::SetMCri(WORD l)		
{
	m_mapDirtyAttrData.insert("C_MCri"); 
	m_FightProperty.wMCri = l;
}
//����������
void CPlayer::SetBloodSuk(WORD l)	
{
	m_mapDirtyAttrData.insert("C_BloodSuck"); 
	m_FightProperty.wBloodSuck = l;
}
//����������
void CPlayer::SetManaSuck(WORD l)	
{
	m_mapDirtyAttrData.insert("C_ManaSuck");
	m_FightProperty.wManaSuck = l;
}
//��־����
void CPlayer::SetWillDef(WORD l)	
{
	m_mapDirtyAttrData.insert("C_WillDef"); 
	m_FightProperty.wWillDef = l;
}
//���ʿ���
void CPlayer::SetConstiDef(WORD l)
{
	m_mapDirtyAttrData.insert("C_ConstiDef");
	m_FightProperty.wConstiDef = l;
}
//Debuff����
void CPlayer::SetDefBuffDef(long lpos,WORD l)			
{
	string name;
	switch(lpos)
	{
	case DFT_Bled:
		name = "E_DeBuffDef0";
		break;
	case DET_Dizzy:
		name = "E_DeBuffDef1";
		break;
	case DET_Lull:
		name = "E_DeBuffDef2";
		break;
	case DET_Hypnosis:
		name = "E_DeBuffDef3";
		break;
	case DET_Poison:
		name = "E_DeBuffDef4";
		break;
	case DET_Fix:
		name = "E_DeBuffDef5";
		break;
	case DET_Silence:
		name = "E_DeBuffDef6";
		break;
	}
	m_mapDirtyAttrData.insert(name);
	m_FightProperty.wDeBuffDef[lpos] = l;
}
//�Ը��������˺�
void CPlayer::SetClassDam(long lpos,WORD l)			
{
	string name;
	switch(lpos)
	{
	case C_Human:
		name = "E_ClassDam0";
		break;
	case C_Element:
		name = "E_ClassDam1";
		break;
	case C_Alien:
		name = "E_ClassDam2";
		break;
	case C_Animal:
		name = "E_ClassDam3";
		break;
	case C_Undead:
		name = "E_ClassDam4";
		break;
	case C_Evil:
		name = "E_ClassDam5";
		break;
	case C_Hexapods:
		name = "E_ClassDam6";
		break;
	}
	m_mapDirtyAttrData.insert(name); 
	m_FightProperty.wClassDam[lpos] = l;
}
void CPlayer::SetElemDam(long lpos,WORD l)	
{
	string name;
	switch(lpos)
	{
	case ET_Fire:
		name = "E_ElemDam0";
		break;
	case ET_Water:
		name = "E_ElemDam1";
		break;
	case ET_Earth:
		name = "E_ElemDam2";
		break;
	case ET_Wind:
		name = "E_ElemDam3";
		break;
	case ET_Dark:
		name = "E_ElemDam4";
		break;
	case ET_Holy:
		name = "E_ElemDam5";
		break;
	}
	m_mapDirtyAttrData.insert(name);
	m_FightProperty.wElemDam[lpos] = l;
}
//ս��ʱ�����ָ��ٶ�
void CPlayer::SetFightHPRecoverSpeed(WORD l)	
{
	m_mapDirtyAttrData.insert("C_FightHPRecoverSpeed"); 
	m_FightProperty.wFightHpRecoverSpeed = l;
}
//ս��ʱ�����ָ��ٶ�
void CPlayer::SetFightMPRecoverSpeed(WORD l)
{
	m_mapDirtyAttrData.insert("C_FightMPRecoverSpeed"); 
	m_FightProperty.wFightMpRecoverSpeed = l;
}
//�����ָ��ٶ�
void CPlayer::SetEnterRecoverSpeed(WORD l)		
{
	m_mapDirtyAttrData.insert("C_EnterRecoverSpeed"); 
	m_FightProperty.wEnerRecoverSpeed = (short)l;
}
//ս�������ָ��ٶ�
void CPlayer::SetFightEnterRecoverSpeed(WORD l)	
{
	m_mapDirtyAttrData.insert("C_FightEnterRecoverSpeed"); 
	m_FightProperty.wFightEnerRecoverSpeed = (short)l;
}
//����������
void CPlayer::SetEnergySuck(float f)					
{
	m_mapDirtyAttrData.insert("C_EnergySuck");
	m_FightProperty.fEnergySuck = f;
}
//����������
void CPlayer::SetCriDef(WORD l)	
{
	m_mapDirtyAttrData.insert("C_CriDef");
	m_FightProperty.wCriDef = l;
}
//ħ����������
void CPlayer::SetMCriDef(WORD l)	
{
	m_mapDirtyAttrData.insert("C_MCriDef");
	m_FightProperty.wMCriDef = l;
}

long CPlayer::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	DWORD dwStartTime = timeGetTime();
	const tagTimerVar* pTimerVar = (const tagTimerVar*)var;
	long lTimerType =pTimerVar ->lvar1<<16 | pTimerVar->TimerType;
#ifdef _RUNSTACKINFO3_
	char pszStatckInfo[256]="";
	_snprintf(pszStatckInfo,256,"CPlayer::OnTimeOut(TimerType:%d,Name:%s) Start",pTimerVar?pTimerVar->TimerType:0,GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

    // player move
    if( pTimerVar->TimerType == ChildObjType_ShapeMove )
    {
        m_mover.OnTimeOut();
    }
    else if (ChildObjType_GoodsTimeout == pTimerVar->TimerType)
	{    //! ��ʱ��Ʒ����
		OnLimitGoodsTimeout(timerid);
		/*GameManager::GetInstance()->FreeBlock((void*)var);*/
	}
    else if (ChildObjType_Budhism == pTimerVar->TimerType)
    {
        if( !static_cast<StateAI::PlayerAI*>(m_pAI)->IsInSkillActTime() && !m_Buddhism.IsEmpty() )
        {
            m_Buddhism.UpdateDistract( -1*GlobalSetup::GetSetup()->lDistractValue );
            /// �������ֵ
            AddAttributeToDirty("lDistract");
            UpdateAttribute(1);
#ifdef  _DEBUG
            char  szText[256];
            _snprintf(szText,256,"[Colour 'ffffffff'] ����ֵ=%d,�������=%d,����ʱ��=%d",m_Buddhism.lDistract,m_Buddhism.lDryNum,m_Buddhism.lDryEndTime);
            SendNotifyMessage( szText, eNOTIFYPOS_LEFT_BOTTOM );
#endif
        }
    }
    else if (ChildObjType_ClearDry == pTimerVar->TimerType )
    {
        m_Buddhism.ClearDryNum();
        /// �������ֵ
        AddAttributeToDirty("lDryNum");
        UpdateAttribute(1);
#ifdef  _DEBUG
        char  szText[256];
        _snprintf(szText,256,"[Colour 'ffffffff'] ����ֵ=%d,�������=%d,����ʱ��=%d",m_Buddhism.lDistract,m_Buddhism.lDryNum,m_Buddhism.lDryEndTime);
        SendNotifyMessage( szText,eNOTIFYPOS_LEFT_BOTTOM );
#endif
    }
	else if( pTimerVar->TimerType == ChildObjType_GoodsExTimeOut )
	{
		long lSubType = pTimerVar->lvar;
		long lTriger = pTimerVar->lvar1;
		if( lSubType == GoodsExTimerType::GETT_GUARDIAN_ENERGY )
		{
	//		m_GoodsTimerHandler.OnTimeOut( this, GoodsTimerHandler::TimerKey( lTriger ), 
	//			&CPlayer::OnGuardianEnergyTimeOut );
		}
		// add more goods timer handle...
	}
	//ս��״̬ʱ��
	else if(timerid == m_lFightStateTimerID)
	{
		//EnterPeaceState();
		m_lFightStateTimerID = -1;
	}
	// queue time
	else if (ChildObjType_CreateRgnQueue == pTimerVar->TimerType)
	{
		OnQueueTimeout(timerid);
	}

	// prgn del time
	else if (ChildObjType_CreatePRgn == pTimerVar->TimerType)
	{
		OnCreatePRgnTimeout(timerid);
	}
	// trgn del time
	else if (ChildObjType_CreateTRgn == pTimerVar->TimerType)
	{
		OnCreateTRgnTimeout(timerid);
	}

	else if(ChildObjType_InvestLimit==pTimerVar->TimerType)
	{
		//���ó�׵�Ͷ�ʼ�¼
		ClearInvestRecord();
	}

	else if(ChildObjType_PlayerHint == pTimerVar->TimerType)
	{
		OnHintTimeOut(pTimerVar->lvar);
	}

	else if (pTimerVar->TimerType == ChildObjType_PKValue)
	{
		OnDecreasePKValue();
	}
	else if (pTimerVar->TimerType == ChildObjType_PvPValue)
	{
		OnDecreasePvPValue();
	}
	//ˢ��ٸ������
	else if(pTimerVar->TimerType == ChildObjType_UpdateMercQuest)
	{
		//OnUpdateMercQuestTimeOut();
		return 0;
	}
	//�Զ��ָ�Hp,Mp
	else if (pTimerVar->TimerType == ChildObjType_AutoRecoverHpMp)
	{
		OnIncreaseHpMpValue();
	}
	//else if (pTimerVar->TimerType == ChildObjType_AutoRecoverEnergy)
	//{
	//	OnUpdateEnergy();
	//}
	else if (pTimerVar->TimerType == ChildObjType_RgnKickPlayerTime)
	{
		OnRgnKickPlayerTimeout(timerid);
	}
	else if(pTimerVar->TimerType == ChildObjType_Fcm_KickPlayerTime)
	{
		GetGame()->KickPlayer(GetExID());
	}
	else if(pTimerVar->TimerType == ChildObjType_ReliveTimer)
	{
		OnRelive(0);
	}
	//else if (pTimerVar->TimerType == ChildObjType_Mail)
	//{
	//	OnMailTimeOut(pTimerVar);
	//}
	else
	{
		CMoveShape::OnTimeOut(timerid,curtime,var);
	}

	GetGame()->AddTimerExpendTime(lTimerType,timeGetTime()-dwStartTime);
#ifdef _RUNSTACKINFO3_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CPlayer::OnTimeOut() End");
#endif
	return 0;
}
//�ʼ���ʱ����
//void 	CPlayer::OnMailTimeOut(const tagTimerVar* pTimerVar)
//{
//
//	CMail::tagMail *ptgMail  = (CMail::tagMail*)(pTimerVar->pvar);
//	CMail *pMail =  GetMail(ptgMail->MailGuid); //(CMail*)(pTimerVar->pvar)
//	if (pMail)
//	{
//		GameManager::GetInstance()->GetMailManager()->OnMailTimerOut(pMail,this);
//	}
//
//}
//long CMoveShape::Proc(eEventType type,const tagProcVar *pVar)
//{
//	switch(pVar->lType)
//	{
//	case ChildObjType_SysMail:
//		{
//
//		}
//		break;
//	}
//	CShape::Proc(type,pVar);
//	return 1;
//}

const CGUID& CPlayer::GetPersonalRgnGUID(void)
{
	return m_PersonalRgnGUID;
}
const CGUID& CPlayer::GetPersonalHouseRgnGUID(void)
{
	return m_PersonalHouseRgnGUID;
}
void CPlayer::SetPersonalRgnGUID(const CGUID& guid)
{
	m_PersonalRgnGUID = guid;
}
void CPlayer::SetPersonalHouseRgnGUID(const CGUID& guid)
{
	m_PersonalHouseRgnGUID = guid;
}

//! ���ܼӳɿ�ʼ
long CPlayer::SkillReinforceBegin(bool isMagic, bool isAttack)
{
	++m_EffectIdSeed;
	CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON); 
	if( NULL != pArm)
	{
		pair<float, float> ReturnReinforce;
		GoodsExManage::GetInstance().ArmReinforceBegin(this, pArm, isMagic, isAttack, ReturnReinforce);
		m_mapReturnReinforce[m_EffectIdSeed] = ReturnReinforce;
	}
	return m_EffectIdSeed;
}

//! ���ܼӳɽ���
void CPlayer::SkillReinforceEnd(long lKey)
{
	m_mapReturnReinforce.erase(lKey);
}

//! ��ü��ܼӳ�ֵ
const pair<float, float>& CPlayer::GetSkillReinforceAmend(long lKey)
{
	map<long, pair<float, float>>::iterator ite = m_mapReturnReinforce.find(lKey);
	if (m_mapReturnReinforce.end() != ite)
	{
		return ite->second;
	}

	return m_FixedReinforce;
}

//! ���������ǰ����
long CPlayer::GetArmCurrPower(void)
{
	CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON); 
	if(NULL != pArm && pArm->HasAddonProperty(GAP_CURR_POWER))
		return pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);
	return -1;
}

//! ������Ʒ��ǰ���������ֵ�ı������ðٷ������ӱ�ʾ��
long CPlayer::GetArmCurrPowerSacle(void)
{
	CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON); 
	if(NULL != pArm && pArm->HasAddonProperty(GAP_CURR_POWER))
		return pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1) * 100 / pArm->GetAddonPropertyValues(GAP_MAX_POWER, 1);
	return 100;
}

//! ��������������õ�һ�����õļӳ�ֵ
float CPlayer::GetReinforceByEnum(long eValuePos, BOOL isMagic)
{
	long lAimLevel = 0;
	long lPowerIntensity = 0; 
	CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON);
	if(NULL != pArm && pArm->HasAddonProperty(GAP_CURR_POWER))
	{
		if (0 != GoodsExManage::GetInstance().CanExpendPower(pArm, isMagic))
		{
			lAimLevel = pArm->GetAddonPropertyValues(GAP_WEAPON_DAMAGE_LEVEL, 1);
			lPowerIntensity = pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1);
			return GoodsExManage::GetInstance().GetReinforceByEnum(lAimLevel, lPowerIntensity, eValuePos);
		}
	}
	return 1.0f;
}
//����ϵͳ�ʼ��б�
void CPlayer::InsertSysMail(long lID,long lEndTime)
{

	map<long,long>::iterator it=m_mapSysMail.find(lID);
	if (it==m_mapSysMail.end() && lEndTime>0)
	{
		m_mapSysMail[lID] = lEndTime;
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	}
}
//���ϵͳ�ʼ��б�
void CPlayer::CheckSysMailTime()
{
	time_t t_CurrentTime = time(NULL);
	map<long,long>::iterator it=m_mapSysMail.begin();
	for (;it!=m_mapSysMail.end();)
	{
		if (t_CurrentTime>(*it).second && (*it).second!=1 )
		{
			it=m_mapSysMail.erase(it);
		}
		else
		{
			++it;
		}
	}

}
bool	CPlayer::AddReceiveMailList(CGUID &MailGuid)
{
	list<CGUID>::iterator it = find(m_lReceiveMail.begin(),m_lReceiveMail.end(),MailGuid);
	if (it == m_lReceiveMail.end())
	{
		m_lReceiveMail.push_back(MailGuid);
		return true;
	}
	return false;
}
void	CPlayer::DeleteReceiveMailList(CGUID &MailGuid)
{
	list<CGUID>::iterator it = find(m_lReceiveMail.begin(),m_lReceiveMail.end(),MailGuid);
	if (it != m_lReceiveMail.end())
	{
		m_lReceiveMail.erase(it);
	}
}

//�����û�н��չ�lSysIDϵͳ�ʼ�
bool	CPlayer::CheckSysMail(long lSysID)
{
	map<long,long>::iterator it=m_mapSysMail.find(lSysID);
	if(it!=m_mapSysMail.end())
	{
		return true;
	}
	return false;
}

//! �۳�һ�����ĵ�����������
bool CPlayer::ExpendPower(BOOL isMagic)
{
	CGoods *pArm = m_cEquipment.GetGoods(EC_E_WEAPON);
	if(NULL == pArm) return true;
	long lUsePower = GoodsExManage::GetInstance().GetExpendPower(pArm, isMagic);
	if(0 >= lUsePower) return false;
	long lCurrPower = pArm->GetAddonPropertyValues(GAP_CURR_POWER, 1);
	lCurrPower -= lUsePower;
	lCurrPower = (0 > lCurrPower) ? 0 : lCurrPower;
	pArm->ChangeAttribute(GAP_CURR_POWER, lCurrPower, 1, true);
	CMessage msg(MSG_S2C_GOODS_POWER_WARN);
	msg.Add(GetExID());
	msg.Add(pArm->GetAddonPropertyValues(GAP_POWER_INTENSITY, 1));
	msg.Add(lCurrPower);
	msg.SendToAround(this);
	return true;
}

//!	ע��һ���ŶӶ�ʱ���������ض�ʱ��ID
long CPlayer::RegisterQueueTimerID(long lFutureTime)
{
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar = OBJ_CREATE(tagTimerVar);
	//void* pVar = GameManager::GetInstance()->CreateBlock(sizeof(tagTimerVar));
	/*assert(NULL != pVar);*/
	((tagTimerVar*)pVar)->TimerType = ChildObjType_CreateRgnQueue;

	long lTimerID = GameManager::GetInstance()->GetRgnManager()->GetQueueTimer()->Schedule(this, pVar, lFutureTime + timeGetTime());
	m_lQueueTimerID = lTimerID;
	return lTimerID;
}
//!	ȡ��һ���Ŷӵ�Timer
void CPlayer::CancelQueueTimer(void)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPlayer::CancelQueueTimer(TimerID:%d)",m_lQueueTimerID);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	void* pVar = NULL;
	GameManager::GetInstance()->GetRgnManager()->GetQueueTimer()->Cancel(m_lQueueTimerID, (const void**)&pVar);
}
//!	��Ӧ�ŶӶ�ʱ���������¼�
void CPlayer::OnQueueTimeout(long lTimerID)
{
}

//!	ע��һ������P������ʱ���������ض�ʱ��ID
long CPlayer::RegisterCreatePRgnTimerID(long lFutureTime)
{
	// ��ʱ���Ѵ���
	if(-1 != m_lPRgnTimerID) return m_lPRgnTimerID;
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar = OBJ_CREATE(tagTimerVar);
	//void* pVar = GameManager::GetInstance()->CreateBlock(sizeof(tagTimerVar));
	/*assert(NULL != pVar);*/
	((tagTimerVar*)pVar)->TimerType = ChildObjType_CreatePRgn;
	m_lPRgnTimerID = GameManager::GetInstance()->GetRgnManager()->GetCreatePRgnTimer()->Schedule(this, pVar, lFutureTime + timeGetTime(), lFutureTime);
	return m_lPRgnTimerID;
}
//!	ȡ��һ������P������Timer
void CPlayer::CancelCreatePRgnTimer(void)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPlayer::CancelCreatePRgnTimer(TimerID:%d)",m_lPRgnTimerID);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	void* pVar = NULL;
	GameManager::GetInstance()->GetRgnManager()->GetCreatePRgnTimer()->Cancel(m_lPRgnTimerID, (const void**)&pVar);
	m_lPRgnTimerID = -1;
}
//!	��Ӧ����P������ʱ���������¼�
void CPlayer::OnCreatePRgnTimeout(long lTimerID)
{
	m_CurPRgnNum--;
	if(m_CurPRgnNum == 0)
		CancelCreatePRgnTimer();
}

void CPlayer::ClearIncShopCur10()
{
	IncShopCur10Iter it=m_mapIncShopCur10.begin();
	for(;it!=m_mapIncShopCur10.end();it++)
	{
		it->second.clear();
	}
}

// ��ֵ�̵��������
void CPlayer::CodeIncShopToDataBlock(DBWriteSet& setWriteDB)
{
    size_t nSize=m_mapIncShopCur10.size();
    setWriteDB.AddToByteArray(static_cast<DWORD>(nSize));
    IncShopCur10Iter IncShopCurIt=m_mapIncShopCur10.begin();
    for(;IncShopCurIt!=m_mapIncShopCur10.end();IncShopCurIt++)
    {
        setWriteDB.AddToByteArray(static_cast<DWORD>(IncShopCurIt->second.size()));
        list<DWORD>::iterator goodsIdIt=IncShopCurIt->second.begin();
        for(;goodsIdIt!=IncShopCurIt->second.end();goodsIdIt++)
        {
            setWriteDB.AddToByteArray(*goodsIdIt);
        }
    }

    //��ֵ�̵����������¼
    setWriteDB.AddToByteArray((DWORD)m_mapLimitGoodsRecord.size());
    map<DWORD,DWORD>::iterator buyRecordIt=m_mapLimitGoodsRecord.begin();
    for(;buyRecordIt!=m_mapLimitGoodsRecord.end();buyRecordIt++)
    {
        setWriteDB.AddToByteArray(buyRecordIt->first);
        setWriteDB.AddToByteArray(buyRecordIt->second);
    }

    //��ֵ�̵꽻��ȫ��¼
    setWriteDB.AddToByteArray( (DWORD)m_vecTradeList.size() );
    CIncrementShopList::ITR_BUYLIST buyListIt = m_vecTradeList.begin();
    for ( ; buyListIt != m_vecTradeList.end(); ++buyListIt )
    {
        setWriteDB.AddToByteArray( buyListIt->strBuyTime.c_str() );
        setWriteDB.AddToByteArray( buyListIt->dwPriceNum );
        setWriteDB.AddToByteArray( buyListIt->dwPriceType );
        setWriteDB.AddToByteArray( buyListIt->dwGoodsIdx );
        setWriteDB.AddToByteArray( buyListIt->dwBuyNum );  
    }
}

void CPlayer::DecodeIncShopFromDataBlock(DBReadSet& setReadDB)
{
    char strTemp[256];
    DWORD dwSize = setReadDB.GetDwordFromByteArray();
#ifdef _RUNSTACKINFO1_
    char pszStatckInfo[1024]="";
    _snprintf(pszStatckInfo,1024,"Decode ShopCur10(Size:%u) start.",dwSize);
    CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
    ClearIncShopCur10();
    IncShopCur10Iter IncShopIt=m_mapIncShopCur10.begin();
    for(;IncShopIt!=m_mapIncShopCur10.end();IncShopIt++)
    {
        DWORD dwRecordSize=setReadDB.GetDwordFromByteArray();
        DWORD dwId=0;
        for (DWORD i=0;i<dwRecordSize;i++)
        {
            dwId=setReadDB.GetDwordFromByteArray();
            IncShopIt->second.push_back(dwId);
        }
    }

    //��ֵ�̵����������¼
    m_mapLimitGoodsRecord.clear();
    DWORD dwBuyRecordSize=setReadDB.GetDwordFromByteArray();
    for(DWORD i=0;i<dwBuyRecordSize;i++)
    {
        DWORD dwBuyGoodsId=setReadDB.GetDwordFromByteArray();
        DWORD dwBuyGoodsNum=setReadDB.GetDwordFromByteArray();
        m_mapLimitGoodsRecord[dwBuyGoodsId]=dwBuyGoodsNum;
    }

    //��ֵ�̵꽻��ȫ��¼
    m_vecTradeList.clear();
    DWORD dwTradeListNum = setReadDB.GetDwordFromByteArray();
    for ( DWORD i = 0; i < dwTradeListNum; ++i )
    {
        setReadDB.GetStringFromByteArray( strTemp, 256 );
        DWORD dwPriceNum = setReadDB.GetDwordFromByteArray();
        DWORD dwPriceType = setReadDB.GetDwordFromByteArray();
        DWORD dwGoodsIdx = setReadDB.GetDwordFromByteArray();
        DWORD dwBuyNum = setReadDB.GetDwordFromByteArray();
        CIncrementShopList::IncShopTradeLog stIncTradeElem( strTemp, dwPriceNum, dwPriceType, dwGoodsIdx, dwBuyNum );
        m_vecTradeList.push_back( stIncTradeElem );
    }
}

// ��ӵ���������б���
void CPlayer::Add2IncShopCur10( DWORD dwMode,DWORD dwGoodsID )
{
	IncShopCur10Iter it=m_mapIncShopCur10.find(dwMode);
	if(it==m_mapIncShopCur10.end())
		return;
	list<DWORD>::iterator idIt=it->second.begin();
	for(;idIt!=it->second.end();idIt++)
	{
		if(*idIt==dwGoodsID)
			break;
	}
	if(idIt==it->second.end())
		it->second.push_front(dwGoodsID);
	while (it->second.size()>10)
	{
		it->second.pop_back();
	}
	// �����������
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	}

//��ӵ����˹�����ϸ��¼�б���
void CPlayer::Add2IncTradeList( DWORD dwMode, DWORD dwGoodsId, DWORD dwGoodsNum, long lTotalAmount )
	{
	char strBuyTime[64] = "";

	SYSTEMTIME	stTime;
	GetLocalTime(&stTime);
	_snprintf(strBuyTime,64, "%d.%d.%d  %d:%d:%d" ,
		stTime.wYear,stTime.wMonth, stTime.wDay,
		stTime.wHour,stTime.wMinute,stTime.wSecond);
	CIncrementShopList::IncShopTradeLog stIncTradeElem( strBuyTime, lTotalAmount, dwMode, dwGoodsId, dwGoodsNum );
	m_vecTradeList.push_back( stIncTradeElem );

	// �����������
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
}

//!	ע��һ���������˶�ʱ���������ض�ʱ��ID
long CPlayer::RegisterRgnKickPlayerTimerID(long lFutureTime)
{
	// ��ʱ���Ѵ���
	if(-1 != m_lRgnKickPlayerTimerID) return m_lRgnKickPlayerTimerID;
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar = OBJ_CREATE(tagTimerVar);
	((tagTimerVar*)pVar)->TimerType = ChildObjType_RgnKickPlayerTime;
	m_lRgnKickPlayerTimerID = GameManager::GetInstance()->GetRgnManager()->GetRgnKickPlayerTimer()->Schedule(this, pVar, lFutureTime + timeGetTime());
	return m_lRgnKickPlayerTimerID;
}

//!	ע��һ��������ϵͳ���˵Ķ�ʱ���������ض�ʱ��ID
long CPlayer::RegisterFcmKickPlayerTimerID(long lFutureTime)
{
	// ��ʱ���Ѵ���
	if(-1 != m_lRgnKickPlayerTimerID) return m_lRgnKickPlayerTimerID;
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar = OBJ_CREATE(tagTimerVar);
	((tagTimerVar*)pVar)->TimerType = ChildObjType_Fcm_KickPlayerTime;
	m_lRgnKickPlayerTimerID = GameManager::GetInstance()->GetRgnManager()->GetRgnKickPlayerTimer()->Schedule(this, pVar, lFutureTime + timeGetTime());
	return m_lRgnKickPlayerTimerID;
}

//!	ȡ��һ����������Timer
void CPlayer::CancelRgnKickPlayerTimer(void)
{
	void* pVar = NULL;
	GameManager::GetInstance()->GetRgnManager()->GetRgnKickPlayerTimer()->Cancel(m_lRgnKickPlayerTimerID, (const void**)&pVar);
	m_lRgnKickPlayerTimerID = -1;
}
//!	��Ӧ�������˶�ʱ���������¼�
void CPlayer::OnRgnKickPlayerTimeout(long lTimerID)
{
	// ȡ���ö�ʱ��
	CancelRgnKickPlayerTimer();

	// ����ҷ��õ��ø��������ĸ����سǵ�
	CServerRegion* pRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(GetCurRgnType(), GetRegionID());
	if(pRgn)
	{
		CGUID retRgnGuid(NULL_GUID);
		long lRetRgnID = 0;
		long l, t, r, b, d;
		if(pRgn->GetRegionSetup()->bDupRgnIsUsed)
			pRgn->GetDupRgnReturnPoint(this, lRetRgnID, l, t, r, b, d);
		CServerRegion* pRetRgn = NULL;
		if(lRetRgnID > 0) // ��ͨ��������
		{
			pRetRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, lRetRgnID);
		}
		else if(lRetRgnID == -1)// ���ع���
		{
			BYTE byCountry = GetCountry();
			lRetRgnID = GetCountryParam()->GetCountryMainRegionID(byCountry);
			pRetRgn = GameManager::GetInstance()->GetRgnManager()->FindRgnByTemplateID(RGN_NORMAL, lRetRgnID);
			long l = GetCountryParam()->GetCountryMainRectLeft(byCountry);
			long t = GetCountryParam()->GetCountryMainRectTop(byCountry);
			long r = GetCountryParam()->GetCountryMainRectRight(byCountry);
			long b = GetCountryParam()->GetCountryMainRectBottom(byCountry);

			if(pRetRgn)
			{				
				SetRegionID(lRetRgnID);
				SetRegionGUID(pRetRgn->GetExID());
				SetCurRgnType((eRgnType)RGN_NORMAL);
			}			
		}
		else if(lRetRgnID == 0) // ԭ��������
		{
			pRetRgn = pRgn;
		}

		if(pRetRgn) retRgnGuid = pRetRgn->GetExID();
		ChangeRegion(RGN_NORMAL, retRgnGuid, l, t, d, lRetRgnID);
	}
}
//!	ע��һ������T������ʱ���������ض�ʱ��ID
long CPlayer::RegisterCreateTRgnTimerID(long lFutureTime)
{
	// ��ʱ���Ѵ���
	if(-1 != m_lTRgnTimerID) return m_lTRgnTimerID;
	//! m_PlayerTimer������Զ��ͷ�pVar
	void* pVar = OBJ_CREATE(tagTimerVar);
	((tagTimerVar*)pVar)->TimerType = ChildObjType_CreateTRgn;
	m_lTRgnTimerID = GameManager::GetInstance()->GetRgnManager()->GetCreateTRgnTimer()->Schedule(this, pVar, lFutureTime + timeGetTime(), lFutureTime);
	return m_lTRgnTimerID;
}
//!	ȡ��һ������T������Timer
void CPlayer::CancelCreateTRgnTimer(void)
{
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPlayer::CancelCreateTRgnTimer(TimerID:%d)",m_lTRgnTimerID);
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
	void* pVar = NULL;
	GameManager::GetInstance()->GetRgnManager()->GetCreateTRgnTimer()->Cancel(m_lTRgnTimerID, (const void**)&pVar);
	m_lTRgnTimerID = -1;
}
//!	��Ӧ����T������ʱ���������¼�
void CPlayer::OnCreateTRgnTimeout(long lTimerID)
{
	m_CurTRgnNum--;
	if(m_CurTRgnNum == 0)
		CancelCreateTRgnTimer();
}

//�ܷ�ɼ�
tagUseFailed CPlayer::CanDOccuCollect(CMoveShape* moveshape)
{
	if (moveshape==NULL)
	{
		return SKILL_USE_FAILED_C_NOCOLLECTION;//�޲ɼ�����
	}
	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(moveshape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return SKILL_USE_FAILED_C_UNKNOW_COLLECTION;//δ֪�ɼ���
	}
	//ְҵ����
	if (GetDOccupation()!= pCollectionLayout->GetOccuID())
	{
		return SKILL_USE_FAILED_C_DOCCU_ERROR;//ְҵ����
	}
	//�ɼ��ȼ�����
	if (GetCollectionGrade()< (long)pCollectionLayout->GetCollectLevel())
	{
		return SKILL_USE_FAILED_C_LEVEL_ERROR;//�ɼ��ȼ�����
	}
	//������Ʒ�ж�
	const list<tgConditionGoods> list_conditiongoods= pCollectionLayout->GetConditionList();
	for (list<tgConditionGoods>::const_iterator it= list_conditiongoods.begin(); 
		it!= list_conditiongoods.end(); it++)
	{
		//Ѱ��������Ʒ
		int goodsnum=GetGoodsAmountByOriginName((*it).cGoodsName);
		if ((goodsnum< (*it).iNum && (*it).iNumType==0)
			|| (goodsnum!=(*it).iNum && (*it).iNumType==1)
			|| (goodsnum>= (*it).iNum && (*it).iNumType==2))
		{
			return SKILL_USE_FAILED_C_GOODSCONDITION_ERROR;//��Ʒ��������
		}
	}

	//�����ո��ж�
	//DWORD dwEmptyGrid=0;
	//dwEmptyGrid+=getOriginPack()->GetEmptySpaceNum();
	//for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	//{
	//	CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
	//	if(pTagSubpack && pTagSubpack->pSubpackContainer)
	//	{
	//		dwEmptyGrid+=pTagSubpack->pSubpackContainer->GetEmptySpaceNum();
	//	}
	//}
	//if (dwEmptyGrid<=0)
	//{
	//	return SKILL_USE_FAILED_C_NO_BAGSPACE;//�����ռ����
	//}

	return SKILL_USE_NO_FAILED;
}

//�ɼ�
BOOL CPlayer::DOccuCollect(CMoveShape* moveshape)
{
	if (moveshape==NULL)
	{
		return FALSE;
	}
	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(moveshape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return FALSE;
	}
	//���Ӿ���
	DWORD exp=CDeputyOccuSystem::GetCollectionExp(GetCollectionGrade()-pCollectionLayout->GetCollectLevel());
	AddCollectionExp(exp);
	moveshape->BeenCollected();

	vector<CGoods*> GainList;

	//�����Ʒ
	const list<tgOddsGoods>& list_goods=pCollectionLayout->GetGainedList();
	for (list<tgOddsGoods>::const_iterator it=list_goods.begin(); 
		it!= list_goods.end(); it++)
	{
		if (random(10000)< (*it).fOdds * 10000)
		{
			DWORD dwGoodsIndex=GoodsSetup::QueryGoodsIDByOriginalName((*it).cGoodsName);
			CGoodsFactory::CreateGoods( dwGoodsIndex, (*it).iGoodNum, GainList ,9);
		}
	}
	CTestContainer testContainer;
	testContainer.Init(this);
	vector<tagTestResult> vecResult;
	//ѭ�����Ʒ�б�,���������ӵ�����,����ӵ�����
	CGUID guid = NULL_GUID;
	CGUID::CreateGUID(guid);
	for (vector<CGoods*>::iterator it=GainList.begin();it!=GainList.end();it++)
	{
		CGoods* pGoods=*it;
		if (pGoods)
		{
			BOOL bSucced=testContainer.AddTest( pGoods,&vecResult);
			if (bSucced)
			{
				GetLogicLogInterface()->logT055_collect(this,pGoods,guid,pGoods->GetAmount());
				if (vecResult.size()==1)
				{
					AddGoodsByTest(&vecResult[0],pGoods);
				}
			}
			else
			{
				GetLogicLogInterface()->logT055_collect(this,pGoods,guid,pGoods->GetAmount());
				SendNotifyMessage(AppFrame::GetText("1"));
				//��ӵ����������ñ���ʱ��
				CServerRegion* pRegion=dynamic_cast<CServerRegion*>(GetFather());
				if(pRegion)
				{
					long x=0, y=0;
					long lDropGoodsStartPos=0;

					lDropGoodsStartPos= pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), x, y, lDropGoodsStartPos);
					if( lDropGoodsStartPos != -1 )
					{
						(pGoods)->SetPosXY(x+0.5f, y+0.5f);
						pRegion->AddObject( pGoods);				
						pRegion -> SetGoodsProtection( this, pGoods,FALSE);				
						// ֪ͨÿ�����
						CS2CContainerObjectMove comMsg;
						comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
						comMsg.SetDestinationContainer( pRegion -> GetType(), 
							pRegion -> GetExID(), y * pRegion -> GetWidth() + x );
						comMsg.SetDestinationObject(pGoods -> GetType(),pGoods-> GetExID() );
						comMsg.SetDestinationObjectAmount(pGoods-> GetAmount() );
						pGoods-> SerializeForOldClient( &( comMsg.GetObjectStream() ) );
						comMsg.SendToAround( this );
						comMsg.Send(GetExID());
					}
				}
			}
		}

	}
	return TRUE;
}
//�Ƿ�������ɼ�
tagUseFailed CPlayer::CanRoleCollect(CMoveShape* pMoveShape)
{
	if (pMoveShape==NULL)
	{
		return SKILL_USE_FAILED_C_NOCOLLECTION;//�޲ɼ�����
	}
	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(pMoveShape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return SKILL_USE_FAILED_C_UNKNOW_COLLECTION;//δ֪�ɼ���
	}
	//ְҵ����
	if ( DOccu_Other!= pCollectionLayout->GetOccuID())
	{
		return SKILL_USE_FAILED_C_NOT_ROLECOLLECTION;//��������ɼ���Ʒ 
	}

	if (pMoveShape->GetBeneficiaryID()!=NULL_GUID)
	{
		if(pMoveShape->GetBeneficiaryID()!=m_guid && pMoveShape->GetBeneficiaryID() != m_TeamID)
		{
			return SKILL_USE_FAILED_C_STATE_ERROR;//���ܲɼ�״̬
		}
	}

	//�ɼ�����
	//int RoleID= pCollectionLayout->GetRoleID();
	//	DWORD RoleSetp= pCollectionLayout->GetRoleStep();
	vector<tagQuestGoods>vecquestgoods=pCollectionLayout->GetRoleInfo();
	vector<tagQuestGoods>::iterator it=vecquestgoods.begin();
	BOOL bFind=false;
	for (;it!=vecquestgoods.end();it++)
	{
		//����������
		if (( (*it).nQuestId ==-1 ) )
		{
			bFind = true;
			break;
		}
		else
		{
			if(FindRoleStep((*it).nQuestId,(*it).dwRoleStep))
			{
				bFind = true;
				break;	
			}
		}	
	}
	if (!bFind)
	{
		return SKILL_USE_FAILED_C_QUESTSTEP_ERROR;//����id �Ͳ��費�ܶ�Ӧ
	}

	//������Ʒ�ж�
	const list<tgConditionGoods> list_conditiongoods= pCollectionLayout->GetConditionList();
	for (list<tgConditionGoods>::const_iterator it= list_conditiongoods.begin(); 
		it!= list_conditiongoods.end(); it++)
	{
		//Ѱ��������Ʒ
		int goodsnum=GetGoodsAmountByOriginName((*it).cGoodsName);
		if ((goodsnum< (*it).iNum && (*it).iNumType==0)
			|| (goodsnum!=(*it).iNum && (*it).iNumType==1)
			|| (goodsnum>=(*it).iNum && (*it).iNumType==2))
		{
			return SKILL_USE_FAILED_C_GOODSCONDITION_ERROR;//��Ʒ����
		}
	}

	//�����ո��ж�
	//DWORD dwEmptyGrid=0;
	//dwEmptyGrid+=getOriginPack()->GetEmptySpaceNum();
	//for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	//{
	//	CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
	//	if(pTagSubpack && pTagSubpack->pSubpackContainer)
	//	{
	//		dwEmptyGrid+=pTagSubpack->pSubpackContainer->GetEmptySpaceNum();
	//	}
	//}
	//if (dwEmptyGrid<=0)
	//{
	//	return SKILL_USE_FAILED_C_NO_BAGSPACE;//�����ռ䲻����
	//}

	return SKILL_USE_NO_FAILED;//��ʧ�ܼ�¼
}
//����ɼ�
BOOL CPlayer::RoleCollect(CMoveShape* pMoveShape)
{
	if (pMoveShape==NULL)
	{
		return FALSE;
	}
	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(pMoveShape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return FALSE;
	}
	vector<CGoods*> GainList;
	//�����Ʒ
	const list<tgOddsGoods>& list_goods=pCollectionLayout->GetGainedList();
	for (list<tgOddsGoods>::const_iterator it=list_goods.begin(); 
		it!= list_goods.end(); it++)
	{
		if (random(10000)< (*it).fOdds * 10000)
		{
			DWORD dwGoodsIndex=GoodsSetup::QueryGoodsIDByOriginalName((*it).cGoodsName);
			CGoodsFactory::CreateGoods( dwGoodsIndex, (*it).iGoodNum, GainList,10 );
		}
	}
	CTestContainer testContainer;
	testContainer.Init(this);
	vector<tagTestResult> vecResult;
	//ѭ�����Ʒ�б�,���������ӵ�����,����ӵ�����
	CGUID guid = NULL_GUID;
	CGUID::CreateGUID(guid);
	for (vector<CGoods*>::iterator it=GainList.begin();it!=GainList.end();it++)
	{
		CGoods* pGoods=*it;
		if (pGoods)
		{
			BOOL bSucced=testContainer.AddTest( pGoods,&vecResult);
			if (bSucced)
			{
				GetLogicLogInterface()->logT055_collect(this,pGoods,guid,pGoods->GetAmount());										
				if (vecResult.size()==1)
				{
					AddGoodsByTest(&vecResult[0],pGoods);
				}
			}
			else
			{
				GetLogicLogInterface()->logT055_collect(this,pGoods,guid,pGoods->GetAmount());
				SendNotifyMessage(AppFrame::GetText("1"));
				//��ӵ����������ñ���ʱ��
				CServerRegion* pRegion=dynamic_cast<CServerRegion*>(GetFather());
				if(pRegion)
				{
					long x=0, y=0;
					long lDropGoodsStartPos=0;

					lDropGoodsStartPos= pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), x, y, lDropGoodsStartPos);
					if( lDropGoodsStartPos != -1 )
					{
						(pGoods)->SetPosXY(x+0.5f, y+0.5f);
						pRegion->AddObject( pGoods);				
						pRegion -> SetGoodsProtection( this, pGoods,FALSE);				
						// ֪ͨÿ�����
						CS2CContainerObjectMove comMsg;
						comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
						comMsg.SetDestinationContainer( pRegion -> GetType(), 
							pRegion -> GetExID(), y * pRegion -> GetWidth() + x );
						comMsg.SetDestinationObject(pGoods -> GetType(),pGoods-> GetExID() );
						comMsg.SetDestinationObjectAmount(pGoods-> GetAmount() );
						pGoods-> SerializeForOldClient( &( comMsg.GetObjectStream() ) );
						comMsg.SendToAround( this );
						comMsg.Send(GetExID());
					}
				}
			}
		}
	}
	pMoveShape->BeenCollected();
	return TRUE;
}

tagUseFailed CPlayer::CanCollect(CMoveShape* pMoveShape)
{
	//��ǰ״̬���ܱ��ɼ�

	//pPlayer->SetCurrentProgress( CPlayer::PROGRESS_OPEN_STALL );//��̯
	//pPlayer -> SetCurrentProgress( CPlayer::PROGRESS_TRADING );//����
	if (GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING 
        ||  IsLockPlayerState() )
	{
		return SKILL_USE_FAILED_CANNOTCOLLECTION_STATE;
	}

	//�ɼ����ܷ񱻲ɼ�
	if (pMoveShape==NULL )
	{
		return SKILL_USE_FAILED_C_NOCOLLECTION;//�޲ɼ������
	}
	if ((!pMoveShape->GetCanBeenCollect()) || pMoveShape->GetIsBeenCollect()|| 
		pMoveShape->GetCollectionTimes() <=0)
	{
		return SKILL_USE_FAILED_C_STATE_ERROR;//���ܲɼ�״̬
	}
	if (pMoveShape->GetRoleProtect())//���񱣻�״̬
	{
		return SKILL_USE_FAILED_C_ROLE_PROTECT;
	}

	//��Ƥ����ʱ�ж�������
	if(pMoveShape->GetType() == TYPE_MONSTER)
	{
		if (pMoveShape->GetBeneficiaryID()!=NULL_GUID)
		{
			if(pMoveShape->GetBeneficiaryID()!=m_guid && pMoveShape->GetBeneficiaryID() != m_TeamID)
			{
				return SKILL_USE_FAILED_C_STATE_ERROR;//���ܲɼ�״̬
			}
		}
	}

	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(pMoveShape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return SKILL_USE_FAILED_C_UNKNOW_COLLECTION;//δ֪�ɼ���Ʒ
	}

	tagUseFailed tgUseSkillRes = SKILL_USE_FAILED_INVALID_UNKNOWN;

	if(pCollectionLayout->GetOccuID()< DOccu_Other)
	{
		tgUseSkillRes= CanDOccuCollect(pMoveShape);
	}
	else if (pCollectionLayout->GetOccuID()== DOccu_Other)
	{
		tgUseSkillRes= CanRoleCollect(pMoveShape);
	}

	if (tgUseSkillRes!= SKILL_USE_NO_FAILED)
	{
		return tgUseSkillRes;
	}

	return tgUseSkillRes;

}
tagUseFailed CPlayer::CheckCanCollect(CMoveShape* pMoveShape)
{
	//��ǰ״̬���ܱ��ɼ�
	if (GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING )
	{
		return SKILL_USE_FAILED_CANNOTCOLLECTION_STATE;
	}

	//�ɼ����ܷ񱻲ɼ�
	if (pMoveShape==NULL )
	{
		return SKILL_USE_FAILED_C_NOCOLLECTION;//�޲ɼ������
	}

	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(pMoveShape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return SKILL_USE_FAILED_C_UNKNOW_COLLECTION;//δ֪�ɼ���Ʒ
	}

	if(pCollectionLayout->GetOccuID()< DOccu_Other)
	{
		return CanDOccuCollect(pMoveShape);
	}
	else if (pCollectionLayout->GetOccuID()== DOccu_Other)
	{
		return CanRoleCollect(pMoveShape);
	}
	else
		return SKILL_USE_FAILED_INVALID_UNKNOWN;
}
BOOL CPlayer::CheckCollect(CMoveShape* pMoveShape)
{
	const CCollectionLayout* pCollectionLayout= CDeputyOccuSystem::GetCollectionLayout(pMoveShape->GetCollectionName().c_str());
	if (pCollectionLayout==NULL)
	{
		return FALSE;
	}

	BOOL rst=FALSE;

	if(pCollectionLayout->GetOccuID()< DOccu_Other)
	{
		rst= DOccuCollect(pMoveShape);
	}
	else if (pCollectionLayout->GetOccuID()== DOccu_Other)
	{
		rst= RoleCollect(pMoveShape);
	}
	else
		rst= FALSE;

	if (rst== TRUE)
	{
		const string& strScrpt = pCollectionLayout->GetSuccScriptName();
		if (strScrpt!="")
		{
			//stRunScript st;
			//st.pszFileName   = (char*)(strScrpt.c_str());
			//st.srcShape  = this;
			//st.desShape	 = pMoveShape;
			//st.pRegion   = dynamic_cast<CRegion*>(GetFather());
			//RunScript(&st,(char*)GetGame()->GetScriptFileData(st.pszFileName));	
			Script::RunEnv env( this, pMoveShape, static_cast<CRegion*>( GetFather() ) );
			GetInst( ScriptSys ).RunScript( env, strScrpt.c_str() );
		}
	}
	return rst;
}
//�����ɼ���
void CPlayer::LockCollection(CMoveShape* pMoveShape)
{
	pMoveShape->SetIsBeenCollect(TRUE);
	pMoveShape->SetCanBeenCollect(FALSE);
}
//����ɼ�������
void CPlayer::OutlockCollection(CMoveShape* pMoveShape)
{
	pMoveShape->SetIsBeenCollect(FALSE);
	if (pMoveShape->GetCollectionTimes()>0)
	{
		pMoveShape->SetCanBeenCollect(TRUE);
	}
	else
	{
		pMoveShape->SetCanBeenCollect(FALSE);
	}

}
//�Ƿ�������
tagUseFailed CPlayer::CanFacture()
{
	//��ǰ״̬��������
	if (GetCurrentProgress()==CPlayer::PROGRESS_OPEN_STALL 
		|| GetCurrentProgress()==CPlayer::PROGRESS_TRADING )
	{
		return SKILL_USE_FAILED_CANNOTFACTURE_STATE;
	}

	DWORD factureID=GetElectFactureID();
	const CFacture* pFacture= CDeputyOccuSystem::GetFacture(factureID);
	if (pFacture==NULL)
	{
		return SKILL_USE_FAILED_F_NO_FACTURE;// �����ڵ�������Ŀ
	}
	//ְҵ����
	if (GetDOccupation()!= pFacture->GetArtisanID())
	{
		return SKILL_USE_FAILED_F_DOCCU_ERROR;//��ְҵ����
	}
	//���޴�������Ŀ
	if (!FindFacture(factureID))
	{
		return SKILL_USE_FAILED_F_NOTHAS_FACTURE;//������������Ŀ
	}
	//������������
	const list<tgGoods>& list_Tools= pFacture->GetToolList();
	for (list<tgGoods>::const_iterator it= list_Tools.begin(); it!= list_Tools.end(); it++)
	{
		//���������ж�
		int goodsnum=GetGoodsAmountByOriginName((*it).cGoodsName);
		if (goodsnum< (*it).iGoodNum)
		{
			return SKILL_USE_FAILED_F_GOODSCONDITION_ERROR;//��������
		}
	}
	//
	//�����ж�
	const list<tgGoods>& list_Material=pFacture->GetMaterialList();
	for (list<tgGoods>::const_iterator it= list_Material.begin(); it!= list_Material.end(); it++)
	{
		//�����ж�
		int goodsnum=GetGoodsAmountByOriginName((*it).cGoodsName);
		if (goodsnum< (*it).iGoodNum)
		{
			return SKILL_USE_FAILED_F_MATERIALS_ERROR;//ԭ������
		}
	}
	//npc������������
	const list<tgNpcDistance>& list_NpcDis= pFacture->GetNpcList();
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );//Ѱ�����ڳ���
	if (pRegion==NULL)
	{
		return SKILL_USE_FAILED_INVALID_UNKNOWN;
	}
	vector<CMonster*> vecNPC;
	if (list_NpcDis.size()!=0)
	{

		vector<CShape*> vOut;
		pRegion -> FindAroundObject( this, TYPE_MONSTER ,vOut );

		for (long a=0; a< (long)vOut.size(); a++)
		{
			CMonster* monster=dynamic_cast<CMonster*>(vOut[a]);
			if (monster!= NULL && monster->GetNpcType()==NT_Normal)
			{
				vecNPC.push_back(monster);
			}
		}
		if (vecNPC.size()==0)
		{
			return SKILL_USE_FAILED_F_NPC_ERROR;//npc��������
		}

	}


	for (list<tgNpcDistance>::const_iterator it= list_NpcDis.begin(); it!= list_NpcDis.end(); it++)
	{
		int iNPCnum=0;
		for (long a=0; a< (long) vecNPC.size(); a++)
		{
			CMonster* pNPC= vecNPC[a];
			if (pNPC==NULL && 0==strcmp(pNPC->GetOriginName(),(*it).cNpcName) && Distance(pNPC)<(*it).iDistace )
			{
				iNPCnum++;
			}
		}
		if (iNPCnum==0)
		{
			return SKILL_USE_FAILED_F_NPC_ERROR;//npc��������
		}
	}
	//�����ո��ж�
	//DWORD dwEmptyGrid=0;
	//dwEmptyGrid+=getOriginPack()->GetEmptySpaceNum();
	//for(int i=0;i<CSubpackContainer::SUBPACK_NUM;i++)
	//{
	//	CSubpackContainer::tagSubpack* pTagSubpack=GetPackage()->GetSubpack(i);
	//	if(pTagSubpack && pTagSubpack->pSubpackContainer)
	//	{
	//		dwEmptyGrid+=pTagSubpack->pSubpackContainer->GetEmptySpaceNum();
	//	}
	//}
	//if (dwEmptyGrid<=0)
	//{
	//	return SKILL_USE_FAILED_F_NO_BAGSPACE;//�����ռ�����
	//}

	return SKILL_USE_NO_FAILED;//��ʧ��

}
//����
BOOL CPlayer::Facture()
{
	DWORD factureID=GetElectFactureID();

	const CFacture* pFacture= CDeputyOccuSystem::GetFacture(factureID);
	if (pFacture==NULL)
	{
		return FALSE;
	}



	//�����Ʒ
	//DWORD dwRandomNum=random(CDeputyOccuSystem::GetAdvancedGainedBacePro());

	vector<CGoods*> GainList;

	//const list<tgGoods>& list_goods;
	/*if (dwRandomNum< GetFactureSuccRatio())
	{
	const list<tgGoods>&list_goods= pFacture->GetAdvanceGainedList();
	ClearFactureSuccRatio();
	for (list<tgGoods>::const_iterator it = (list_goods).begin(); it!= (list_goods).end(); it++)
	{
	DWORD dwGoodsIndex= GoodsSetup::QueryGoodsIDByOriginalName((*it).cGoodsName);
	GoodsSetup::CreateGoods( dwGoodsIndex, (*it).iGoodNum, GainList );
	}
	}
	else
	{*/
	const list<tgGoods>&list_goods = pFacture->GetGainedList();
	AddFactureSuccRatio(1);
	for (list<tgGoods>::const_iterator it = (list_goods).begin(); it!= (list_goods).end(); it++)
	{
		DWORD dwGoodsIndex= GoodsSetup::QueryGoodsIDByOriginalName((*it).cGoodsName);
		CGoodsFactory::CreateGoods( dwGoodsIndex, (*it).iGoodNum, GainList ,11);
	}
	//	}
	//��ȡ�����б�,����ɾ��
	vector<CGoods*>vecMaterial;
	const list<tgGoods>& list_Material=pFacture->GetMaterialList();
	tagTestArg_ID_Num tagTestArg;
	vector<tagTestArg_ID_Num>vecArg;
	for (list<tgGoods>::const_iterator it= list_Material.begin(); it!= list_Material.end(); it++)
	{
		tagTestArg._dwGoodsID=GoodsSetup::QueryGoodsIDByOriginalName(it->cGoodsName);
		tagTestArg._dwNum=it->iGoodNum;
		vecArg.push_back(tagTestArg);
		/*DWORD dwGoodsIndex= GoodsSetup::QueryGoodsIDByOriginalName((*it).cGoodsName);
		GoodsSetup::CreateGoods( dwGoodsIndex, (*it).iGoodNum, vecMaterial );*/
	}


	CTestContainer testContainer;
	testContainer.Init(this);
	vector<tagDelTestResult> vecDelResult;
	BOOL bDelSucceed=testContainer.DelTest(vecArg,&vecDelResult);
	vector<tagTestResult>vecResult;
	BOOL bSucced=testContainer.AddTest(GainList,&vecResult);
	if (bDelSucceed && bSucced && vecResult.size() == GainList.size())
	{
		CGUID guid = NULL_GUID;
		CGUID::CreateGUID(guid);
		//���Ӿ���
		DWORD exp=CDeputyOccuSystem::GetFactureExp(GetFactureGrade()- pFacture->GetFactureLevel());//GetCollectionExp(GetCollectionGrade()-pCollectionLayout->GetCollectLevel());
		exp *= (DWORD)pFacture->GetQuotiety();
		AddFactureExp(exp);
		DWORD credit= CDeputyOccuSystem::GetFactureCredit(GetCollectionGrade() - pFacture->GetFactureLevel());
		if (credit>0)
		{
		//	SetArtisanCredit(GetArtisanCredit()+ credit);
		}

		//����ɾ��
		for (long i=0;i<(long)vecDelResult.size();i++)
		{
			CGoodsContainer *pContainer = FindContainer(vecDelResult[i].dwContainerId);
			if (pContainer)
			{
				CGoods* pGoods = dynamic_cast<CGoods*>( pContainer -> GetGoods(vecDelResult[i].dwPos) );
				if (pGoods)
				{
                    GetLogicLogInterface()->logT056_facture(this,pGoods,guid,0,vecDelResult[i].dwNum);
				}
				DelGoodsByTest(&vecDelResult[i]);
			}	
		}

		for(long i=0;i<(long)vecResult.size();i++)
		{
			if (GainList[i])
			{
				//��¼������
				const char* goodsname = GoodsSetup::QueryGoodsOriginalName(GainList[i]->GetBasePropertiesIndex());
				if (goodsname != NULL)
				{
					list<tgGoods>::const_iterator it = list_goods.begin();
					for (; it!= list_goods.end(); it++)
					{
						if (strcmp(goodsname,(*it).cGoodsName) == 0 &&
							(*it).bMaker == 1)
						{
							GainList[i]->SetMakerName(this->GetName());
							break;
						}	
					}
				}
				GetLogicLogInterface()->logT056_facture(this,GainList[i],guid,1,GainList[i]->GetAmount());
				AddGoodsByTest(&vecResult[i],GainList[i]);
			}
		}
	}
	else
	{
		//��Ӳ��ɹ�,�����ڴ�
		for(long i=0;i<(long)GainList.size();i++)
		{
#ifdef _GOODSLOG1_
			if(GainList[i])
			{
				char pszGoodsLog[1024]="";			
				char srcGoodsGUID[50]="";
				GainList[i]->GetExID().tostring(srcGoodsGUID);					
				_snprintf(pszGoodsLog,1024,"[��Ʒ����:CPlayer::Facture][��ƷID:%d][��Ʒ��:%s][��ƷGUID:%s][��Ʒ����:%d]",
					GainList[i]->GetBasePropertiesIndex(),GoodsSetup::QueryGoodsName(GainList[i]->GetBasePropertiesIndex()),srcGoodsGUID,GainList[i]->GetAmount());
				CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
			}									
#endif
			CGoodsFactory::GarbageCollect(&GainList[i],20);
		}
		SendNotifyMessage(AppFrame::GetText("1"));
		return FALSE;
	}
	return TRUE;
}

void CPlayer::FactureSkillStudyBegin(CMoveShape* moveshape)
{
	if (moveshape==NULL || moveshape->GetType()!=TYPE_MONSTER )	return;
	CMonster* pMonster=dynamic_cast<CMonster*>(moveshape);
	if (pMonster->GetNpcType()!=NT_Normal)	return;

	const CFactureStudy* pfacturestudy= CDeputyOccuSystem::GetFactureStudy(pMonster->GetOriginName());
	if (pfacturestudy==NULL)	return ;
	vector<DWORD> vecStudy;
	for (long i=0;i<(long)pfacturestudy->GetStudySum(); i++)
	{
		DWORD factureID= pfacturestudy->GetFactureID(i);
		const CFacture* pfacture= CDeputyOccuSystem::GetFacture(factureID);
		if (pfacture==NULL) continue;
		if (GetDOccupation()== pfacture->GetArtisanID()
			//&& GetFactureGrade()>= pfacture->GetFactureLevel()
			&& !FindFacture(factureID))
		{
			vecStudy.push_back(factureID);
		}
	}
	CMessage msg(MSG_S2C_OCCUSKILL_STUDY_BEGIN);
	msg.Add(pMonster->GetExID());
	msg.Add((DWORD)vecStudy.size());
	for (long a=0;a<(long)vecStudy.size(); a++)
	{
		msg.Add(vecStudy[a]);
		msg.Add(pfacturestudy->FindFactureCost(vecStudy[a]));
		////#ifdef _DUMP
		//char filename[256];
		//_snprintf(filename,"facture_studyinfo id= %d, cost= %d",vecStudy[a],pfacturestudy->FindFactureCost(vecStudy[a]));
		//PutStringToFile("facture_studyinfo",filename);
		//#endif
	}
	msg.SendToPlayer(GetExID());
}

void CPlayer::StudyFactureSkill(CMoveShape* moveshape, DWORD factureID)
{
	if (moveshape==NULL || moveshape->GetType()!=TYPE_MONSTER )	return;
	CMonster* pMonster=dynamic_cast<CMonster*>(moveshape);
	if (pMonster->GetNpcType()!=NT_Normal)	return;

	const CFactureStudy* pfacturestudy= CDeputyOccuSystem::GetFactureStudy(pMonster->GetOriginName());
	if (pfacturestudy==NULL)	return ;

	const CFacture* pfacture= CDeputyOccuSystem::GetFacture(factureID);
	if (pfacture== NULL) return;

	if (GetDOccupation()== pfacture->GetArtisanID()&& 
		GetFactureGrade()>= (LONG)pfacture->GetFactureLevel())
	{
		if (FindFacture(factureID)==FALSE && pfacturestudy->FindFacture(factureID)!=FALSE)
		{
			DWORD cost= pfacturestudy->FindFactureCost(factureID);
			if (GetMoney()>= cost)
			{
				SetMoney(GetMoney()-cost,8);
				AddFacture(factureID);
				return ;
			}
		}
	}
	CMessage msg(MSG_S2C_OCCUSKILL_STUDY);
	msg.Add(LONG(0));
	msg.Add(factureID);
	msg.SendToPlayer(GetExID());
}

void CPlayer::CleanFacture()
{
	CMessage msg(MSG_S2C_OCCUSKILL_DELETE);
	msg.SendToPlayer(GetExID());
	m_DOccuRecord.CleanFacture();
}

void CPlayer::AddCollectionExp(DWORD exp)
{
	if (exp==0)
	{
		return;
	}
	DWORD Level=GetCollectionGrade();
	DWORD CurrentExp=GetCollectionExp();
	long all = CurrentExp + exp;
	if (Level == CDeputyOccuSystem::GetMaxCollectionLevel() &&
		all > (long)CDeputyOccuSystem::GetCollectionNeededExp(Level) )
	{
		SetCollectionExp(CDeputyOccuSystem::GetCollectionNeededExp(Level));
		return;
	}
	bool bUpLevel = false;
	long lastexp =0;
	while (all > (long)CDeputyOccuSystem::GetCollectionNeededExp(Level)&& Level < CDeputyOccuSystem::GetMaxCollectionLevel())
	{
		all -=CDeputyOccuSystem::GetCollectionNeededExp(Level);
		if (all > 0)
		{
			lastexp =all;
		Level++;
			bUpLevel = true;
		}		
	}
	//����
	if (bUpLevel)
	{
		long temp = CDeputyOccuSystem::GetCollectionNeededExp(Level);
		SetCollectionGrade(Level);
		SetCollectionExp(lastexp>temp?temp:lastexp);	
	}
	else
	{
		SetCollectionExp(all);
	}
}

void CPlayer::AddFactureExp(DWORD exp)
{
	if (exp==0)
	{
		return;
	}

	long Level=GetFactureGrade();
	long CurrExp = GetFactureExp();
	long all = CurrExp + exp;
	//�ȼ�����Ҿ�������
	if (Level == CDeputyOccuSystem::GetMaxFactureLevel() &&
		all > (long)CDeputyOccuSystem::GetFactureNeededExp(Level) )
	{
		SetFactureExp(CDeputyOccuSystem::GetFactureNeededExp(Level));
		return;
	}
	bool bUpLevel = false;
	long lastexp =0;
	while (all > (long)CDeputyOccuSystem::GetFactureNeededExp(Level)&& Level < (long)CDeputyOccuSystem::GetMaxFactureLevel())
	{
		all -= (long)CDeputyOccuSystem::GetFactureNeededExp(Level);
		if (all > 0)
		{
			lastexp =all;
		Level++;
			bUpLevel = true;
	}
	}
	//����
	if (bUpLevel)
	{
		long temp = CDeputyOccuSystem::GetFactureNeededExp(Level);
		SetFactureGrade(Level);
		SetFactureExp(lastexp>temp?temp:lastexp);	
	}
	else
	{
		SetFactureExp(all);
	}	
	}

void CPlayer::SendDOccuSKillFailMsg(int nValue,int iSkillID, int iSkillLevel)//SendSKillFailMsg(int nValue)
{
	CMessage msg(MSG_S2C_SKILL_USE);
	msg.Add((CHAR)SKILL_USE_RESULT_FAILED);
	msg.Add( GetExID() );
	//id�͵ȼ�
	msg.Add((LONG)iSkillID);
	msg.Add((BYTE)iSkillLevel );
	msg.Add((DWORD)0);
	//��������
	msg.Add((BYTE)nValue);
	msg.SendToPlayer(GetExID());
#ifdef _SKILL_DEBUG
	Log4c::Trace(ROOT_MODULE,"���ͼ���ʧ����Ϣ:����ID:%6d\n",iSkillID);
#endif
}

//�Ը�ְҵϵͳ�Ĳ�ѯ
long CPlayer::GetFactureNeedsLevel(DWORD ID)//{return CDeputyOccuSystem::GetFacture(ID)->GetFactureLevel();}
{
	const CFacture* p= CDeputyOccuSystem::GetFacture(ID);
	if (p)
	{
		return p->GetFactureLevel();
	}
	else 
		return -1;
}
long CPlayer::GetFactureNeedsDOccu(DWORD ID)//{return CDeputyOccuSystem::GetFacture(ID)->GetArtisanID();}
{
	const CFacture* p= CDeputyOccuSystem::GetFacture(ID);
	if (p)
	{
		return p->GetArtisanID();
	}
	else 
		return -1;

}

BOOL CPlayer::AddFacture(DWORD l)
{
	if (m_DOccuRecord.AddFacture(l))
	{
		CMessage msg(MSG_S2C_OCCUSKILL_STUDY);
		msg.Add((LONG)1);
		msg.Add(l);
		msg.SendToPlayer(GetExID());
		GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
		return TRUE;
	}
	return FALSE;
	//m_DOccuRecord.AddFacture(l);
}
BOOL CPlayer::DeleteFacture(DWORD l)
{
	BOOL bflag = m_DOccuRecord.DeleteFacture(l);
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	return bflag;
}

void CPlayer::SetCollectionExp(LONG l)
{
	m_mapDirtyAttrData.insert("dwCollectionExp");
	m_DOccuRecord.m_lCollectionExp=  l;
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	UpdateAttribute(true);
}
void CPlayer::SetCollectionGrade(LONG l)
{
	m_DOccuRecord.m_lCollectionGrade= l;
	CMessage msg(MSG_S2C_PLAYER_COLLECTION_LEVELUP);
	msg.Add(l);
	msg.Add(GetExID());
	msg.SendToAround(this);
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
}

void CPlayer::SetFactureExp(LONG l)
{
	m_mapDirtyAttrData.insert("dwFactureExp");
	m_DOccuRecord.m_lFactureExp = l;
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	UpdateAttribute(true);
}

void CPlayer::SetFactureGrade(LONG l)
{
	m_DOccuRecord.m_lFactureGrade = l;
	GetGame()->UpdateSavePlayer(GetExID(), SAVE_DETAIL_ATTRPROFESSION);
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);
	CMessage msg(MSG_S2C_PLAYER_FACTURE_LEVELUP);
	msg.Add(l);
	msg.Add(GetExID());
	msg.SendToAround(this);
	//������ʾ��Ϣ
}
//int CPlayer::SetScriptVarValue(CScript* scriptPtr, const char* varName, const char* value)
//{
//	int retFlag = GetVariableList()->SetVarValue(varName, value);
//	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
//		retFlag = scriptPtr->GetVariableList()->SetVarValue(varName, value);
//	if(retFlag == CVariableList::VAR_NOT_FOUND)
//	{
//		if(CScript::GetGeneralVariableList())
//			retFlag = CScript::GetGeneralVariableList()->SetVarValue(varName, value);
//		if(1 == retFlag)
//			CScript::UpdateToWorldServer(varName, value);
//	}
//
//	return retFlag;
//}
//int CPlayer::SetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray, double value)
//{
//	int retFlag = GetVariableList()->SetVarValue(varName, lArray, value);
//	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
//		retFlag = scriptPtr->GetVariableList()->SetVarValue(varName, lArray, value);
//	if(retFlag == CVariableList::VAR_NOT_FOUND)
//	{
//		if(CScript::GetGeneralVariableList())
//			retFlag = CScript::GetGeneralVariableList()->SetVarValue(varName, lArray, value);
//		if(1 == retFlag)
//			CScript::UpdateToWorldServer(varName, lArray, value);
//	}
//
//	return retFlag;
//}
//
//const char* CPlayer::GetScriptVarValue(CScript* scriptPtr, const char* varName)
//{
//	const char* retStr = GetVariableList()->GetVarValue(varName);
//	if(retStr == NULL && scriptPtr)
//		retStr = scriptPtr->GetVariableList()->GetVarValue(varName);
//	if(retStr == NULL)
//	{
//		if(CScript::GetGeneralVariableList())
//			retStr = CScript::GetGeneralVariableList()->GetVarValue(varName);
//	}
//
//	return retStr;
//}
//double CPlayer::GetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray)
//{
//	double retFlag = GetVariableList()->GetVarValue(varName, lArray);
//	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
//		retFlag = scriptPtr->GetVariableList()->GetVarValue(varName, lArray);
//	if(retFlag == CVariableList::VAR_NOT_FOUND)
//	{
//		if(CScript::GetGeneralVariableList())
//			retFlag = CScript::GetGeneralVariableList()->GetVarValue(varName, lArray);
//	}
//
//	return retFlag;
//}
//int CPlayer::SetScriptGuidValue(CScript* scriptPtr, const char* varName, const CGUID& guid)
//{
//	int retFlag = GetVariableList()->SetGuidValue(varName, &guid);
//	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
//		retFlag = scriptPtr->GetVariableList()->SetGuidValue(varName, &guid);
//	if(retFlag == CVariableList::VAR_NOT_FOUND)
//	{
//		if(CScript::GetGeneralVariableList())
//			retFlag = CScript::GetGeneralVariableList()->SetGuidValue(varName, &guid);
//	}
//
//	return retFlag;
//}
//const CGUID& CPlayer::GetScriptGuidValue(CScript* scriptPtr, const char* varName)
//{
//	return scriptPtr->GetScriptGuidValue( this, varName );
//}

//ȡ��
//CMail *CPlayer::GetMail(CGUID &guid)
//{
//	map<CGUID,CMail*>::iterator it=m_mapMail.find(guid);
//	if (it!=m_mapMail.end())
//	{
//		return (*it).second;
//	}
//	return NULL;
//}
void CPlayer::UpdateEffect()
{
	CMessage msg(MSG_S2C_RGN_UPDATEEFFECT);
	msg.SendToPlayer(GetExID());
}

//! ���óƺŵ���ʾ����
BOOL CPlayer::SetTitleType(LONG lType)
{
	if(ePTT_Hide <= lType && ePTT_LOVER >= lType && lType != m_lCurrTitleType)
	{
		m_lCurrTitleType = lType;
		UpdateTitle(TRUE);

	}
	return FALSE;
}

//! ���³ƺ�
void CPlayer::UpdateTitle(BOOL bSendNotify)
{
	const char* pNewTitle = NULL;
	//! ����ֱ�ӻ�������ƺŵ�ʱ���õ��Ļ����ַ���
	char szTileBuf[128] = {0};

	switch(m_lCurrTitleType)
	{
	case ePTT_Knight:	//! ��λ�ƺ�
		{
	//		pNewTitle = AppFrame::GetText(IDS_GS_NOBILITYTITILE,GetRankofNobility());
		}
		break;
	case ePTT_Faction:	//! ���ƺ�
		{
			GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(GetFactionID());
			if(NULL != pFaction)
			{
				pNewTitle = pFaction->GetMemberTitle(GetExID());
			}
			else
				m_lCurrTitleType = ePTT_Hide;
		}
		break;
	case ePTT_Medal1:	//! ѫ��1�ƺ�
		{
			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL1);
			if(NULL != pMedal)
			{
				pNewTitle = pMedal->GetName();
			}
			else
				m_lCurrTitleType = ePTT_Hide;
		}
		break;
	case ePTT_Medal2:	//! ѫ��2�ƺ�
		{
			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL2);
			if(NULL != pMedal)
			{
				pNewTitle = pMedal->GetName();
			}
			else
				m_lCurrTitleType = ePTT_Hide;
		}
		break;
	case ePTT_Medal3:	//! ѫ��3�ƺ�
		{
			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL3);
			if(NULL != pMedal)
			{
				pNewTitle = pMedal->GetName();
			}
			else
				m_lCurrTitleType = ePTT_Hide;
		}
		break;
	case ePTT_LOVER:	//! ��ż����
		{
			if(GlobalSetup::GetSetup()->lShowSpouseNameStep <= GetMarriageStep())
			{
				const char * pSpouseName = GetSpouseName();
				if(NULL != pSpouseName)
				{
					//! 0���У�1��Ů
					if(0 == GetSex())
						_snprintf(szTileBuf, 128, "%s%s", pSpouseName, AppFrame::GetText("72"));
					else
						_snprintf(szTileBuf, 128, "%s%s", pSpouseName, AppFrame::GetText("71"));

					pNewTitle = szTileBuf;
				}
			}
			else
				m_lCurrTitleType = ePTT_Hide;
				
		}
		break;
	}

	if(NULL == pNewTitle)
		pNewTitle = "";

	if(0 != strcmp(pNewTitle, m_AppendProperty.strTitle))
	{
		LONG lCpyLen = (long)strlen(pNewTitle);
		lCpyLen = (lCpyLen < TITLE_SIZE) ? lCpyLen : TITLE_SIZE - 1;
		memset(m_AppendProperty.strTitle, 0, TITLE_SIZE);
		memmove(m_AppendProperty.strTitle, pNewTitle, lCpyLen);

		if(bSendNotify)
		{
			CMessage msg(MSG_S2C_PLAYER_UPDATE_TITLE_TYPE);
			msg.Add(GetExID());
			msg.Add(m_lCurrTitleType);
			msg.Add(pNewTitle);
			msg.SendToAround(this);
		}
	}
}

//! �õ���ǰ��ʾ��ѫ�µ�ԭʼ��
//const char* CPlayer::GetShowMedalBaseName(void)
//{
//	switch(m_lCurrTitleType)
//	{
//	case ePTT_Medal1:	//! ѫ��1�ƺ�
//		{
//			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL1);
//			if(NULL != pMedal)
//			{
//				return GoodsSetup::QueryGoodsOriginalName(pMedal->GetBasePropertiesIndex());
//			}
//		}
//		break;
//	case ePTT_Medal2:	//! ѫ��2�ƺ�
//		{
//			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL2);
//			if(NULL != pMedal)
//			{
//				return GoodsSetup::QueryGoodsOriginalName(pMedal->GetBasePropertiesIndex());
//			}
//		}
//		break;
//	case ePTT_Medal3:	//! ѫ��3�ƺ�
//		{
//			CGoods *pMedal = m_cEquipment.GetGoods(EC_MEDAL3);
//			if(NULL != pMedal)
//			{
//				return GoodsSetup::QueryGoodsOriginalName(pMedal->GetBasePropertiesIndex());
//			}
//		}
//		break;
//	}
//
//	return NULL;
//}


BOOL CPlayer::SerializeSubDepot(DBWriteSet& setWriteDB, LONG eContainerId)
{
	CSubpackContainer *pContainer = (CSubpackContainer*)m_pPlayerDepot.FindContainer(eDCT_Secondary, true);
	assert(NULL != pContainer);
	if(NULL == pContainer) return FALSE;

	LONG lIndex = eContainerId - eDCT_Secondary1;
	if(pContainer->GetSubpack(lIndex)->pGoods)
	{
		setWriteDB.AddToByteArray((BYTE)1);
		setWriteDB.AddToByteArray(static_cast<DWORD>(eContainerId));
		pContainer->GetSubpack(lIndex)->pGoods->Serialize(setWriteDB);
		pContainer->GetSubpack(lIndex)->pSubpackContainer->Serialize(setWriteDB);
	}
	else
	{
		setWriteDB.AddToByteArray((BYTE)0);
		setWriteDB.AddToByteArray(static_cast<DWORD>(eContainerId));

	}
	return TRUE;
}


void CPlayer::SetFactionID(const CGUID& FactionID)
{
	m_FactionID=FactionID;
	if(NULL_GUID == m_FactionID)
	{
		memset(m_szFactionName, 0, MAXSIZE_FactionName);
	}
	else
	{
		GameFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(m_FactionID);
		assert(NULL != pFaction);//! �������ʧ�ܣ�˵�����������SetFactionID()�ӿڣ�Ӧ��SetFactionInfo()����
		if(NULL != pFaction)
			memmove(m_szFactionName, pFaction->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_FACTION, this);
}

void CPlayer::SetFactionInfo(GameFaction *pFaction)
{
	if(NULL == pFaction)
	{
		m_FactionID = NULL_GUID;
		memset(m_szFactionName, 0, MAXSIZE_FactionName);
	}
	else
	{
		m_FactionID = pFaction->GetExID();
		memmove(m_szFactionName, pFaction->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_FACTION, this);
}

void CPlayer::SetUnionID(const CGUID& UnionID)
{
	m_UnionID=UnionID;
	if(NULL_GUID == m_UnionID)
	{
		memset(m_szUnionName, 0, MAXSIZE_FactionName);
	}

	else
	{
		GameUnion *pUnion = GetOrganiCtrl()->GetUnion(m_UnionID);
		assert(NULL != pUnion);
		if(NULL != pUnion)
			memmove(m_szUnionName, pUnion->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_UNION, this);
}

CPet* CPlayer::CreatePet(const CGUID& rID)
{
	if( NULL_GUID == rID )
		return NULL;

	CPet* pPet = OBJ_CREATE(CPet);
	pPet->SetHost(this);
	pPet->SetExID(rID);
	m_Pets[rID] = pPet;
	return pPet;
}

/// �����̴�
bool CPlayer::CatchPet(CMonster* pCatchTarget)
{
	bool bWlid = false;
	long lTargetLev = pCatchTarget->GetLevel();
	if (lTargetLev>1)
	{
		bWlid = true;
	}

	// �̴���λ�п���λ��
	if (GetPetsNum() >= UNI_CONF.maxTakeNum)
	{
		Log4c::Trace(PET_MODULE,"CatchPet Failure : Allready have %ld Servants. Allow have %ld Servants",
            GetPetsNum(),UNI_CONF.maxTakeNum);
		return false;
	}
	
	// Ŀ�겻�ܲ�����Monster�ļ����ã� && Ŀ�����ڱ�������Ҳ�����
	if (!pCatchTarget->IsCatchable())
	{
		Log4c::Trace(PET_MODULE,"CatchPet Failure : NPC Cann't be Catch or other's catching..");
		return false;
	}

	// Ŀ��ȼ��������5��
	if (GetLevel()+5 < pCatchTarget->GetLevel())
	{
		Log4c::Trace(PET_MODULE,"CatchPet Failure : NPC's level is too high");
		return false;
	}
	
	// Ŀ��Ѫ��̫�ߣ��޷�����
	if (pCatchTarget->GetMaxHP()*0.2 > pCatchTarget->GetAttribute("Hp"))
	{
		Log4c::Trace(PET_MODULE, "CatchPet Failure : NPC's HP is too high");
		return false;
	}
	
	// Ŀ�겻���ڵ�ǰ��ң����ɲ���

	pCatchTarget->SetCatchFlag(true);

	//������Ӱ�첶����
	
	//���ﲶ����Ϊ0
	if (pCatchTarget->GetCatchFastness()==0)
	{
		int rate = random(100);
		//50%����ɹ�
		if (rate<50)
		{
			long lType = PET_TYPE_SERVANT;
			long lIndex = pCatchTarget->GetServantID();
			//ɾ��Ŀ�����
			CServerRegion *pRegion = dynamic_cast<CServerRegion*>(GetFather());
			pRegion->DelShapeToAround(dynamic_cast<CMonster*>(pCatchTarget));

			//�����̴�
			GetInst(CPetCtrl).CreatePet(this,lType,lIndex,bWlid);
			return true;
		}
		//40%����
		else if (rate >=50 && rate<90)
		{
			pCatchTarget->ResetMaxCatchFastness();
			return false;
		}
		else if (rate>=90 && rate<100)

		{
			//10%�����ű�
			const char* strScript = pCatchTarget->GetCacthScript().c_str();
			if(strScript)
			{
				Script::RunEnv env(this, NULL, static_cast<CRegion*>(GetFather()));
				GetInst(ScriptSys).RunScript(env, strScript);
			}
			return false;
		}
	}
	return false;
}

CPet* CPlayer::GetPet(const CGUID& rID)
{
	CPet* pPet = NULL;
	C_ITR_PET itr = m_Pets.find(rID);
	if( itr != m_Pets.end() )
	{
		pPet = itr->second;
	}

	return pPet;
}

void CPlayer::AddPet(CPet* pPet)
{
	//PutoutLog("Pet", LT_DEFAULT, AppFrame::GetText("GS_PET_10"), ++lPetsCount);
	m_Pets[pPet->GetExID()] = pPet;
}

bool CPlayer::DelPet(const CGUID& rID)
{
	ITR_PET itr = m_Pets.find(rID);
	if( itr != m_Pets.end() )
	{
		char str[100];
		sprintf_s(str,AppFrame::GetText("GS_PET_9"), itr->second->GetPetName());
		/***********************************************************************/
		/*  zhaohang 2010-11-8 fix ֱ���޳���ɫ����ɫ�����ַ����У��ͻ������Զ�����*/
		/***********************************************************************/
		//GetGame()->SendNotifyMessage(str, GetExID(), NOTIFY_COLOR_RED);
		GetGame()->SendNotifyMessage(str,GetExID());
		DelPet(itr);
		return true;
	}
	return false;
}

void CPlayer::DelPet(ITR_PET& itr)
{
	Log4c::Trace(PET_MODULE, "DelPet : %ld.", --lPetsCount);
	m_Pets.erase(itr);

}

void CPlayer::ReleaseAllPets(void)
{
	ACT_PETS::iterator itrAP = m_FightPets.begin();
	while( itrAP != m_FightPets.end() )
	{
		CPet* pPet = GetFightPet(*itrAP);
		if( pPet != NULL )
		{
			++itrAP;
			//GetInst(CPetCtrl).DelPet(pPet, PET_OT_PLAYER_EXIT);
		}
		else
		{
			//PutoutLog("Pet", LT_ERROR, ".....");
			m_FightPets.erase(itrAP++);
		}
	}

	ITR_PET itr = m_Pets.begin();
	while( !m_Pets.empty() )
	{
		CPet* pPet = itr->second;
		if (pPet != NULL && pPet->IsDied())
		{
			pPet->DelFromRegion(PET_OT_PLAYER_EXIT);
		}

		OBJ_RELEASE(CPet,pPet);
		DelPet(itr++);
	}
	m_Pets.clear();
}

void CPlayer::ActivatePet(bool bActive)
{
	CPet* pPet = GetActivePet();

	if(NULL == pPet)
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_17",GetName()));//���%sû�м������.
		return;
	}

	if(bActive && pPet->IsActive())
	{
		GetInst(CPetCtrl).ActivePet(pPet);
	}
	//else
	//{
	//	GetInst(CPetCtrl).DelPet(pPet, PET_OT_DISAPPEAR);
	//}
}

void CPlayer::SetFocusPetGUID(CGUID guid)
{
	m_FocusPet = guid;
}

const CGUID& CPlayer::GetFocusPetGUID(void)
{
	return m_FocusPet;
}

void CPlayer::SetActivePetGUID(CGUID guid)
{
	m_ActivePet = guid;
}

const CGUID& CPlayer::GetActivePetGUID(void)
{
	return m_ActivePet;
}

CPet* CPlayer::GetFocusPet()
{
	CPet* pPet = NULL;
	if (NULL_GUID==m_FocusPet)
		return NULL;

	C_ITR_PET itrPet = m_Pets.find(m_FocusPet);
	if (itrPet!=m_Pets.end())
	{
		pPet = itrPet->second;
		if( NULL == pPet )
		{
            Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_18",GetName(),itrPet->first));//���%s����Ϊ%d�Ľ������ΪNULL.

		}
	}
	else
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_19",GetName(),itrPet->first));//���%s����Ϊ%d�Ľ�����ﲻ�ڳ����б���.
	}
	return pPet;
}

CPet* CPlayer::GetActivePet()
{
	CPet* pPet = NULL;
	if (NULL_GUID==m_ActivePet)
		return NULL;

	C_ITR_PET itrPet = m_Pets.find(m_ActivePet);
	if (itrPet!=m_Pets.end())
	{
		pPet = itrPet->second;
		if( NULL == pPet )
		{
            Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_20",GetName(),itrPet->first));//���%s����Ϊ%d�ļ������ΪNULL.
		}
	}
	else
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_21",GetName()));//���%s���͵ļ�����ﲻ�ڳ����б���.
	}
	return pPet;
}

CPet* CPlayer::GetFightPet(const CGUID& guid)
{
	CPet* pPet = NULL;
	//C_ITR_ACT_PET itr = m_FightPets.find(guid);
	C_ITR_ACT_PET itr = std::find(m_FightPets.begin(), m_FightPets.end(), guid);
	if( itr != m_FightPets.end() )
	{
		C_ITR_PET itrPet = m_Pets.find(*itr);
		if( itrPet != m_Pets.end() )
		{
			pPet = itrPet->second;
			if( NULL == pPet )
			{
                Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_22",GetName(),itrPet->first));//���%s����Ϊ%d�ĳ�ս����ΪNULL.
			}
		}
		else
		{
            Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_23",GetName(),itrPet->first));
		}
	}

	return pPet;
}

void CPlayer::AddFightPet(const CGUID& guid)
{
	Log4c::Trace(PET_MODULE,"AddActivePet : %ld.", ++lFightPetsCount);
	m_FightPets.push_back(guid);
}

bool CPlayer::AddFightPet(CPet* pPet)
{
	AddFightPet(pPet->GetExID());
	return true;
}

void CPlayer::DelFightPet(const CGUID& guid)
{
	Log4c::Trace(PET_MODULE, "DelFightPet : %ld.", --lFightPetsCount);
	ITR_ACT_PET itr = std::find(m_FightPets.begin(), m_FightPets.end(), guid);
	if( itr != m_FightPets.end() )
		m_FightPets.erase(itr);
}

// �������ƶ�
void CPlayer::CheckPetsMove(void)
{
	// ֻ�м��������Ҫ�����ƶ����
	CPet* pPet = GetActivePet();
	if( NULL == pPet )
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_24",GetName(),m_ActivePet.tostring()));//���%s����Ϊ%s�ļ������ΪNULL.
	}
	pPet->ModifyPos(IT_SAME_RGN);
}

//��ȡ���Ѻ��Լ���GUID
void CPlayer::GetPlayerIDAndTeammateID(hash_set<CGUID,hash_guid_compare>& hsPlayers)
{
	if (GetTeamID()==NULL_GUID)
	{
		hsPlayers.insert(GetExID());
	}
	else
	{
		GSTeam *pGSTeam = (GSTeam*)(GetOrganiCtrl()->GetGameTeamByID( GetTeamID()));
		CServerRegion *pRegion = dynamic_cast<CServerRegion*>(GetFather());
		if (pGSTeam && pRegion)
		{
			map<CGUID,CPlayer*> mpTeamates;
			pGSTeam->GetLucrativeArea_Alive(mpTeamates,pRegion->GetExID(),GetTileX(),GetTileY(), TRUE);
			map<CGUID,CPlayer*>::iterator it=mpTeamates.begin();
			for (;it!=mpTeamates.end();++it)
			{
				hsPlayers.insert((*it).second->GetExID());
			}
		}
	}
}


// �޸ĳ���״̬
void CPlayer::ChangeActivePetState(long lState)
{
	CPet* pPet = GetActivePet();
	if (pPet==NULL)
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_25",GetName()));//���%s�޼������
		return;
	}

	if( pPet->GetState() == lState ) return;

	StateAI::PetAI* pPetAI = (StateAI::PetAI*) pPet->GetAI();
	if( NULL == pPetAI )
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_26",GetName()));//���%s�ļ������AIΪNULL.
		return;
	}

	pPetAI->ChangeStateTo(lState);
}
//�жϼ����Ƿ�������
bool	CPlayer::JudgeSkilllegality()
{
	bool bSkill = false;

	++m_lTotalSkillNum;

	if (m_lTotalSkillNum>GlobalSetup::GetSetup()->lTotalMaxSkillNum)
	{
		//������������Խ������
		DWORD dwEndTime = timeGetTime();
		float fIntelTime = ((float)(dwEndTime - m_dwSkillStartTime))/1000.0f;
		float fOneSecSkillNum = m_lTotalSkillNum/fIntelTime;
		if (fOneSecSkillNum>GlobalSetup::GetSetup()->fOneSecMaxSkillNum)
		{
			bSkill = true;
			Log4c::Trace(ROOT_MODULE,FormatText("GS_PLAYER_9",GetName(),fOneSecSkillNum));

			GetGame()->KickPlayer(GetExID());			
		}
		m_lTotalSkillNum = 0;
		m_dwSkillStartTime = dwEndTime;
	}

	return bSkill;
}
//�ж�NPC�ű��Ƿ�������
bool CPlayer::JudgeNpcScriptslegality(void)
{
	bool bRet = false;

	++m_lTotalNpcScriptsNum;

	if (m_lTotalNpcScriptsNum>GlobalSetup::GetSetup()->lTotalMaxNpcScriptsNum)
	{
		//������������Խ������
		DWORD dwEndTime = timeGetTime();
		float fIntelTime = ((float)(dwEndTime - m_dwNpcScriptsStartTIme))/1000.0f;
		float fOneSecNum = m_lTotalNpcScriptsNum/fIntelTime;
		if (fOneSecNum>GlobalSetup::GetSetup()->fOneSecMaxNpcScriptsNum)
		{
			bRet = true;
			Log4c::Warn(ROOT_MODULE,FormatText("GS_PLAYER_10",GetName(),fOneSecNum));//���%s,1���ڴ���NPC�ű�����:%f ����������Χ
			GetGame()->KickPlayer(GetExID());			
		}
		m_lTotalNpcScriptsNum = 0;
		m_dwNpcScriptsStartTIme = dwEndTime;
	}

	return bRet;
}
//�жϵ�ǰ�����ܷ�ԭ�ظ���
bool	CPlayer::CheckReliveByOrigin()
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( this->GetFather() );
	if (NULL!=pRegion && pRegion->GetRelive())
	{
		//��ǰ��������ԭ�ظ���
		return true;
	}
	//����ԭ�ظ���
	return false;
}


//�ͷŹ������������ű�
void CPlayer::FreeReliveScript()
{
	//CScript::PushDelScript(m_guidRelivScript);
    GetInst( ScriptSys ).PushDelScript( m_guidRelivScript );
}

//�ж�ԭ�ظ�����Ʒ�Ƿ���������
bool	CPlayer::CheckReliveGoods(long &lIndex, long &lRatio, long &lCostNum,char* strOriginName)
{
		//��ȡ������Ʒ�б�
		list<ReLiveGoodsList::tagReLiveGoods*> lReliveGoods = ReLiveGoodsList::GetReliveGoodsList();

		if (lReliveGoods.size()==0)
			return false;
			
		list<ReLiveGoodsList::tagReLiveGoods*>::iterator it = lReliveGoods.begin();
		long lNum = 0;

		if(NULL != (*it))
		{
			lIndex = (*it)->dwIndex;
			lCostNum = (*it)->dwNum;
			lRatio = (*it)->dwRecoverRatio;
			strcpy_s(strOriginName,256, (*it)->strOrignalName.c_str());
			lNum =  GetGoodsAmountByOriginName((*it)->strOrignalName.c_str());
		}

		for (; it!=lReliveGoods.end(); ++it)
		{
			if(NULL!=(*it))
			{
				long lGoodsNum = GetGoodsAmountByOriginName((*it)->strOrignalName.c_str());
				 if(lGoodsNum>0)
				 {
					 //��������и���Ʒ
					 if (lGoodsNum >= (long)(*it)->dwNum)
					 {
						//������Ʒ����
						 lIndex = (*it)->dwIndex;
						 lCostNum = (*it)->dwNum;
						 lRatio = (*it)->dwRecoverRatio;
						 lNum =  lGoodsNum;
						 strcpy_s(strOriginName, 256, (*it)->strOrignalName.c_str());
						 return true;
					 }
					 else if(lGoodsNum< (long)(*it)->dwNum && lNum==0)
					 {
						 //�������ȼ���߲�����������Ʒ��Ϣ
						 lIndex = (*it)->dwIndex;
						 lCostNum = (*it)->dwNum;
						 lRatio = (*it)->dwRecoverRatio;
						 lNum =  lGoodsNum;
						 strcpy_s(strOriginName, 256, (*it)->strOrignalName.c_str());
					 }
				 }
			}			
		}
		return false;

}

// ͨ����ұ�������Ϣ
void CPlayer::SendNotifyWarningMsg(bool isKilled, CPlayer* pAttacker)
{
	if(!pAttacker) return;
	CServerRegion* pRgn = static_cast<CServerRegion*>(GetFather());
	if(pRgn)
	{
		if(pRgn->GetSetup().bWarningFlag == true)
		{
			if( (GetCountry() % 2) != (pAttacker->GetCountry() % 2) ) //������Ӫ��һ��
			{
				long lCurTime = timeGetTime();
				if(!isKilled) // �Ƿ�ɱ��
				{
					if( (long)(lCurTime- pRgn->GetLastWarningTime()) < (GlobalSetup::GetSetup()->lWarningDeltaTime*1000) )
					{
						return;
					}
					else
						pRgn->SetLastWarningTime(lCurTime);
				}

				CMessage msg(MSG_S2W_OTHER_WARNING);
				msg.Add((BYTE)GetCountry());
				msg.Add((BYTE)pAttacker->GetCountry());
				msg.Add((long)pRgn->GetID());
				msg.Add((long)GetTileX());
				msg.Add((long)GetTileY());
				msg.Add((BYTE)isKilled);
				msg.Add(GetName());
				msg.Add(pAttacker->GetName());
				msg.Send();
			}
		}
	}
}

//! �õ����CP
long CPlayer::GetActive(void)
{
	return m_Property.lCurrBaseActive + m_Property.lCurrExActive;
}

//! �������CP
void CPlayer::ResetActive(DWORD dwCurrTime)
{
	m_Property.lCurrBaseActive = (long)PlayerList::GetBaseCP(GetLevel());
//	if(GetGame()->GetStarDay() == m_Property.byConst)
//		m_Property.lCurrBaseActive += m_Property.lCurrBaseActive * 10 / 100;
	m_Property.lCurrBaseActive = min(99999, m_Property.lCurrBaseActive);
	m_Property.lLastResetActiveTime = dwCurrTime;

	// ������������
	GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);

	CMessage msg(MSG_S2C_PLAYER_ACTIVE_CHANGE);
	msg.Add(m_Property.lCurrBaseActive);
	msg.Add(m_Property.lCurrExActive);
	msg.SendToPlayer(GetExID(), false);
}

//! �����չCP
bool CPlayer::AddExActive(long lAddNum)
{
	if(0 < lAddNum)
	{
		m_Property.lCurrExActive += lAddNum;

		// ������������
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);

		CMessage msg(MSG_S2C_PLAYER_ACTIVE_CHANGE);
		msg.Add(m_Property.lCurrBaseActive);
		msg.Add(m_Property.lCurrExActive);
		msg.SendToPlayer(GetExID(), false);

		return true;
	}

	return false;
}

//! �۳�CP
bool CPlayer::TakeActive(long lTakeNum)
{
	if(0 < lTakeNum && GetActive() >= lTakeNum)
	{
		if(m_Property.lCurrBaseActive >= lTakeNum)
		{
			m_Property.lCurrBaseActive -= lTakeNum;
		}
		else
		{
			m_Property.lCurrExActive -= lTakeNum - m_Property.lCurrBaseActive;
			m_Property.lCurrBaseActive = 0;
		}

		// ������������
		GetGame()->UpdateSavePlayer(this, SAVE_DETAIL_ATTRPROPERTY);

		CMessage msg(MSG_S2C_PLAYER_ACTIVE_CHANGE);
		msg.Add(m_Property.lCurrBaseActive);
		msg.Add(m_Property.lCurrExActive);
		msg.SendToPlayer(GetExID(), false);
		return true;
	}
	
	return false;
}

void CPlayer::SetCountryContribute(long l)
{
	m_Property.lCountryContribute = l;
}


//! �õ���ҽ�鲽��
long CPlayer::GetMarriageStep(void)
{
	return m_Property.lMarriageStep;
}

//! ������ҽ�鲽��
long CPlayer::SetMarriageStep(long lStep)
{
	long lOldValue = m_Property.lMarriageStep; 
	m_Property.lMarriageStep = lStep;

	//! ��Խ��ʾֵ����ʱ��������Ϣ���ͻ���
	if(m_Property.lMarriageStep >= GlobalSetup::GetSetup()->lShowSpouseNameStep &&
		lOldValue < GlobalSetup::GetSetup()->lShowSpouseNameStep)
	{
		CMessage msg(MSG_S2C_PLAYER_SHOW_SPOUSENAME);
		msg.Add((BYTE)true);
		msg.SendToPlayer(GetExID(), false);
		UpdateTitle(TRUE);
	}
	else if (m_Property.lMarriageStep < GlobalSetup::GetSetup()->lShowSpouseNameStep &&
		lOldValue >= GlobalSetup::GetSetup()->lShowSpouseNameStep)
	{
		CMessage msg(MSG_S2C_PLAYER_SHOW_SPOUSENAME);
		msg.Add((BYTE)false);
		msg.SendToPlayer(GetExID(), false);
		UpdateTitle(TRUE);
	}

	return lStep;
}
//! ���ö���
bool CPlayer::SetSpouse(CPlayer *pPlayer)
{
	if (NULL != pPlayer)
		LinkmanSystem::GetInstance().AddSpouse(this, pPlayer);
	else
		LinkmanSystem::GetInstance().DelSpouse(this);

	return true;
}

//! �õ�����ID
const CGUID& CPlayer::GetSpouseGuid(void)
{
	const tagLinkman *pLinkman = LinkmanSystem::GetInstance().FindSpouseInfo(this);
	if(NULL != pLinkman)
		return pLinkman->guid;
	return NULL_GUID;
}
//! �õ���������
const char* CPlayer::GetSpouseName(void)
{
	const tagLinkman *pLinkman = LinkmanSystem::GetInstance().FindSpouseInfo(this);
	if(NULL != pLinkman)
		return pLinkman->szName;
	return NULL;
}
//ע��:ʹ�ô˺���,����Ʒ���ӵ�����»�ɾ��ԭ��Ʒָ��
BOOL CPlayer::AddGoodsByTest(tagTestResult* tgResult,CGoods* pGoods)
{
	if (tgResult==NULL || pGoods==NULL)
		return FALSE;
	CVolumeLimitGoodsContainer* pVolumeContainer=dynamic_cast<CVolumeLimitGoodsContainer*>(this->FindContainer(tgResult->dwContainerId));
	if(pVolumeContainer)
	{
		if(tgResult->eOperType == CS2CContainerObjectMove::OT_NEW_OBJECT)
		{
			//�������Ʒ							
			CS2CContainerObjectMove comMsg;
			if(pVolumeContainer->Add(tgResult->dwPos, pGoods, NULL, &comMsg) )
			{
				OnGetGoods(pGoods->GetBasePropertiesIndex(),pGoods);
				comMsg.SetOperation( CS2CContainerObjectMove::OT_NEW_OBJECT );
				comMsg.SetDestinationContainerExtendID( tgResult->dwContainerId );
				pGoods->SerializeForOldClient( &comMsg.GetObjectStream() );
				comMsg.Send( this->GetExID() );

			}
			else
				return FALSE;
		}
		else if(tgResult->eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT)						
		{
			//�ı���Ʒ����
			DWORD dwGoodsID=pGoods->GetBasePropertiesIndex();
			if(pVolumeContainer->Add(tgResult->dwPos, pGoods, NULL) )
			{
				OnGetGoods(dwGoodsID);
				CGoods* pExistedGoods=pVolumeContainer->GetGoods(tgResult->dwPos);
				if(pExistedGoods)
				{
					CS2CContainerObjectAmountChange coacMsg;
					coacMsg.SetSourceContainer( TYPE_PLAYER, this->GetExID(), 
						tgResult->dwPos );
					coacMsg.SetSourceContainerExtendID( tgResult->dwContainerId );
					coacMsg.SetObject( pExistedGoods->GetType(), pExistedGoods->GetExID() );
					coacMsg.SetObjectAmount( pExistedGoods->GetAmount() );
					coacMsg.Send( this->GetExID() );
				}	
				else
					return FALSE;
			}
			else
				return FALSE;
		}
		return TRUE;
	}
	else
		return FALSE;	
}

BOOL CPlayer::DelGoodsByTest(tagDelTestResult* tgResult)
{
	if (tgResult==NULL)
		return FALSE;
	CGoodsContainer *pContainer = this->FindContainer(tgResult->dwContainerId);
	if (pContainer)
	{
		CGoods* pGoods = dynamic_cast<CGoods*>( pContainer -> GetGoods(tgResult->dwPos) );
		if (pGoods==NULL)
			return FALSE;
		if(tgResult->eOperType == CS2CContainerObjectMove::OT_DELETE_OBJECT)
		{
			CS2CContainerObjectMove comMsg;
			if (pContainer->Remove(tgResult->dwPos,tgResult->dwNum,&comMsg))
			{
				OnLoseGoods(pGoods);
				comMsg.SetOperation( CS2CContainerObjectMove::OT_DELETE_OBJECT );
				comMsg.SetSourceContainerExtendID( tgResult->dwContainerId );
				comMsg.Send( this->GetExID() );
				CGoodsFactory::GarbageCollect(&pGoods,14);
			}
			else
				return FALSE;
		}
		else if(tgResult->eOperType == CS2CContainerObjectMove::OT_CHANGE_AMOUNT)						
		{
			CGoods* pExistedGoods=pContainer->GetGoods(tgResult->dwPos);
			if(pExistedGoods)
			{
				OnLoseGoods(pGoods);
				pExistedGoods->SetAmount(pExistedGoods->GetAmount() - tgResult->dwNum);
				CS2CContainerObjectAmountChange coacMsg;
				coacMsg.SetSourceContainer( TYPE_PLAYER, this->GetExID(), 
					tgResult->dwPos );
				coacMsg.SetSourceContainerExtendID( tgResult->dwContainerId );
				coacMsg.SetObject( pExistedGoods->GetType(), pExistedGoods->GetExID() );
				coacMsg.SetObjectAmount( pExistedGoods->GetAmount() );
				coacMsg.Send( this->GetExID() );
			}
			else
				return FALSE;
		}
		return TRUE;
	}
	else
		return FALSE;	
}


//! �õ�Ԫ������
DWORD CPlayer::GetElementProperty(DWORD dwElementType, DWORD dwAttackType)
{
	if(MT_Max > dwElementType && AD_Max > dwAttackType)
		return m_arrElementProperty[dwElementType].uArrPro[dwAttackType];
	assert(false);
	return 0;
}
//! ����Ԫ������
DWORD CPlayer::SetElementProperty(DWORD dwElementType, DWORD dwAttackType, long lNewValue)
{
	if(MT_Max > dwElementType && AD_Max > dwAttackType)
	{
		CMessage msg(MSG_S2C_PLAYER_ELEMENT_PROPERTY_CHANGE);
		msg.Add((BYTE)dwElementType);
		msg.Add((BYTE)dwAttackType);
		msg.Add(lNewValue);
		msg.SendToPlayer(GetExID(), false);
		return (m_arrElementProperty[dwElementType].uArrPro[dwAttackType] = lNewValue);
	}
	assert(false);
	return 0;
}
//! ���Ԫ������
DWORD CPlayer::AddElementProperty(DWORD dwElementType, DWORD dwAttackType, long lAddValue)
{
	if(MT_Max > dwElementType && AD_Max > dwAttackType)
	{
		m_arrElementProperty[dwElementType].uArrPro[dwAttackType] += lAddValue;
		m_arrElementProperty[dwElementType].uArrPro[dwAttackType] = max(0, m_arrElementProperty[dwElementType].uArrPro[dwAttackType]);
		CMessage msg(MSG_S2C_PLAYER_ELEMENT_PROPERTY_CHANGE);
		msg.Add((BYTE)dwElementType);
		msg.Add((BYTE)dwAttackType);
		msg.Add(m_arrElementProperty[dwElementType].uArrPro[dwAttackType]);
		msg.SendToPlayer(GetExID(), false);
		return m_arrElementProperty[dwElementType].uArrPro[dwAttackType];
	}
	assert(false);
	return 0;
}

bool CPlayer::OnIncreaseHp( long &validHp, CMoveShape *firer )
{
    if( CMoveShape::OnIncreaseHp( validHp, firer ) || !firer )
    {
        return false;
    }
    m_tauntList->OnRescued( firer->GetExID(), firer->GetType(), validHp );
    return true;
}

void CPlayer::SendValidateMsgForCs()
{
    // send role information to chat server
    CMessage csmsg(MSG_S2CS_PLAYER_ENTER_GAME);
    csmsg.Add(GetExID());       // role's id
    csmsg.Add(GetSignCode());   // code used for validate
    csmsg.Add(GetName());       // role's name
    csmsg.Add(GetTeamID());     // role's team id
    csmsg.Add(GetFactionID());  // role's faction id
    csmsg.Add(GetRegionID());   // role's region id
    csmsg.Add(GetCountry());    // role's country id
    csmsg.Add(GetGame()->GetWSNumber());   // role's world server id
    csmsg.SendToChatServer();

    // send chat server information and sign code to game client
    CMessage gcmsg(MSG_S2C_OTHER_VALIDATE);
    gcmsg.Add(GetGame()->GetSetup()->strChatServerIp.c_str());
    gcmsg.Add(GetGame()->GetSetup()->dwChatServerPort4Gc);
    gcmsg.Add(GetSignCode());
    gcmsg.SendToPlayer(GetExID());
}