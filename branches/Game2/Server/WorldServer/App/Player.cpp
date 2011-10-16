#include "StdAfx.h"
#include "Player.h"

#include "AntiCheat/Configer.h"
#include "Container/Wallet.h"
#include "Country/Country.h"
#include "Country/CountryHandler.h"
#include "Country/CountryParam.h"
#include "DBEntity/EntityManager.h"
#include "GlobalRgnManager.h"
#include "Goods/Goods.h"
#include "Goods/GoodsFactory.h"
#include "Linkman/LinkmanSystem.h"
#include "Listener/SeekGoodsListener.h"
#include "NewSkill/NewSkillFactory.h"
#include "Pet/Pet.h"
#include "PlayerMan/PlayerMan.h"
#include "WorldRegion.h"

#include "../../../Public/Setup/GlobalSetup.h"
#include "../../../Public/Setup/PlayerList.h"
#include "../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../Public/ServerPublic/Server/Common/VarListUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CPlayer::CPlayer(void)
: m_lCurrTitleType(ePTT_Hide)
,m_lFacDemiseWarning(0)
,m_lFacDisbandWarning(0)
,m_cWallet(GoodsSetup::GetGoldCoinIndex())
,m_cSilverWallet(GoodsSetup::GetSilverIndex())
,m_cGem(GoodsSetup::GetGemIndex())
{
	m_CurPRgnNum = 0;
	m_CurTRgnNum = 0;

	SetType(TYPE_PLAYER);

	//m_lVariableNum = 0;		// ��������
	//m_lGuidNum = 0;            // GUIDNum

	//memset(&m_BaseProperty, 0, sizeof(tagBaseProperty)-sizeof(string)*2);
	//memset(&m_Property, 0, sizeof(tagProperty));
	memset(&m_Property,0,sizeof(m_Property));
	memset(&m_BaseFightProperty,0,sizeof(m_BaseFightProperty));
	memset(&m_FactFightProperty,0,sizeof(m_FactFightProperty));
	memset(&m_AppendProperty,0,sizeof(m_AppendProperty));
	//m_AppendProperty.strTitle="";
	//m_AppendProperty.strAccount ="";

	//m_lVariableNum = 0;			// ��������
	//m_pVariableData = NULL;		// ��������
	//m_lVariableDataLength = 0;	// �������ݳ���

	
	m_lInvestLimitTimeMark=0;
	m_cOriginPack=new CVolumeLimitGoodsContainer();
	m_cOriginPack->SetContainerVolume(5*4);
	m_pSubpackContainer=new CSubpackContainer();	

    // ����װ����λ
	m_cEquipmentGod.SetTrumpType(TT_GOD);
	m_cEquipmentLand.SetTrumpType(TT_GLAND);
	m_cEquipmentHuman.SetTrumpType(TT_HUMAN);

	m_mapGoodsIdCoolDown.clear();
	m_mapClassIdCoolDown.clear();

	m_bSaveDepot =false;
	m_lSilienceTime = 0;
	m_dwMurdererTime = 0;

	m_lFightStateCount = 0;

	m_TeamGuid = NULL_GUID;
    m_PhaseId  = NULL_GUID;
    m_PhaseRest= ePRest;

	m_PlayerDepot.BindPlayer(this);
	
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_YUANBAO,list<DWORD>()));
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_WEIMIAN,list<DWORD>()));
	m_mapIncShopCur10.insert(IncShopCur10Map::value_type(CIncrementShopList::TM_TICKET,list<DWORD>()));

	m_mapLimitGoodsRecord.clear();
	m_vecTradeList.clear();

	m_GsSocket = 0;

	m_lTicket = 0;
	m_lNewMail.clear();										//���ʼ��б�
	m_lReceiveMail.clear();									//�����ʼ��б�

	memset(m_szFactionName, 0, MAXSIZE_FactionName);
	memset(m_szUnionName, 0, MAXSIZE_FactionName);
    m_GeniusList.clear(); 
	m_VarList = new VariableList();
}


CPlayer::~CPlayer(void)
{
	ReleaseAllPets();
	m_cEquipment.Release();			////װ�����ͷ�
    m_cEquipmentGod.Release();
    m_cEquipmentLand.Release();
    m_cEquipmentHuman.Release();
	gcPacks();
	m_cWallet.Release();
	m_cGem.Release();
	ReleaseNpcShopSellGoods();
	ClearMailMap();
	//SAFE_DELETE(m_pVariableData);	////�ֿ�����Ʒ���ͷ�
	m_PlayerDepot.Release();		//! �ͷŲֿ����
	m_lNewMail.clear();										//���ʼ��б�
	m_lReceiveMail.clear();									//�����ʼ��б�
	LinkmanSystem::GetInstance().DelPlayerLinkman(GetExID());
	SAFE_DELETE( m_VarList );
}

VOID CPlayer::gcPacks()
{
	/*
	for(int i=0;i<SUBPACK_NUM;i++)
	{		
		if(m_SubPack[i].pSubPackContainer!=NULL)
		{
			m_SubPack[i].pSubPackContainer->Release();
		}
	}
	
	for(int i=0;i<SUBPACK_NUM;i++)
	{		
		delete m_SubPack[i].pSubPackContainer;
		m_SubPack[i].pSubPackContainer=NULL;
	}*/
	SAFE_DELETE(m_cOriginPack);
	SAFE_DELETE(m_pSubpackContainer);
}

VOID CPlayer::PushbakNpcShopSellGoods(CGoods* pGoods,DWORD dwPrice)
{
	if(!pGoods)
		return;
	CPlayer::tagShopSelledGoods stSelledGoods;
	stSelledGoods.pGoods=pGoods;
	stSelledGoods.dwSelledPrice=dwPrice;
	if(m_listNpcShopSellGoods.size()>=10)
	{
		list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();		
		CGoodsFactory::GarbageCollect(&((*it).pGoods));
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

VOID CPlayer::ReleaseNpcShopSellGoods()
{
	list<CPlayer::tagShopSelledGoods>::iterator it=m_listNpcShopSellGoods.begin();
	for(;it!=m_listNpcShopSellGoods.end();it++)
	{
		CGoods* pGoods=(*it).pGoods;

		if(pGoods)			
			CGoodsFactory::GarbageCollect(&((*it).pGoods));
	}
	m_listNpcShopSellGoods.clear();
}

hash_map<CGUID,CGoods*,hash_guid_compare>* CPlayer::GetSubPackGoodsMap(int i)
{
	CSubpackContainer::tagSubpack* pSubpack=m_pSubpackContainer->GetSubpack(i);
	if(pSubpack)
	{
		return pSubpack->pSubpackContainer->GetGoodsMap();
	}
	return NULL;
}

/*
CVolumeLimitGoodsContainer* CPlayer::getSubPack(BYTE pos)
{
	if(pos<0 || pos>=SUBPACK_NUM)
		return NULL;
	return m_SubPack[pos].pSubPackContainer;
}

CGoods* CPlayer::getSubPackGoods(BYTE pos)
{
	if(pos<0 || pos>=SUBPACK_NUM)
		return NULL;
	return m_SubPack[pos].pGoods;
}*/

VOID CPlayer::setSubPackGoods(BYTE pos,CGoods** pGoods)
{
	if(pos<0 || pos>=CSubpackContainer::SUBPACK_NUM)
		return;
	m_pSubpackContainer->GetSubpack(pos)->pGoods=*pGoods;	
}

BOOL CPlayer::SerializeSubpack(vector<BYTE>* pStream,PLAYER_EXTEND_ID extendId)
{
	
	int nIndex=extendId-PEI_PACK1;
	if(m_pSubpackContainer->GetSubpack(nIndex)->pGoods)
	{
		_AddToByteArray(pStream,static_cast<DWORD>(extendId));
		m_pSubpackContainer->GetSubpack(nIndex)->pGoods->Serialize(pStream);
		m_pSubpackContainer->GetSubpack(nIndex)->pSubpackContainer->Serialize(pStream);
	}
	else
	{
		_AddToByteArray(pStream,static_cast<DWORD>(0));
	}
	return TRUE;
}

BOOL CPlayer::UnserializeSubpack(BYTE* pStream, LONG& lOffset)
{	
	BYTE btVal=_GetByteFromByteArray(pStream,lOffset);
	DWORD dwExtendId=_GetDwordFromByteArray(pStream,lOffset);
	PLAYER_EXTEND_ID ecColumn = static_cast<PLAYER_EXTEND_ID>(dwExtendId); 			
	if(btVal)
	{		
		CGoods* pGoods = CGoodsFactory::UnserializeGoods( pStream, lOffset,30 );		
		if( pGoods )
		{
#ifdef __MEMORY_LEAK_CHECK__
			CBaseObject::SetConstructFlag(72);
#endif
			m_pSubpackContainer->Add( ecColumn, pGoods, NULL );			
			m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->SetContainerVolume(pGoods);
		}				
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->Unserialize(pStream,lOffset);
	}
	else
	{
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pGoods=NULL;
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->Clear();
	}
	return TRUE;
}


BOOL CPlayer::UnserializeSubpack(DBReadSet& setReadDB)
{
	BYTE btVal=setReadDB.GetByteFromByteArray();
	DWORD dwExtendId=setReadDB.GetDwordFromByteArray();
	PLAYER_EXTEND_ID ecColumn = static_cast<PLAYER_EXTEND_ID>(dwExtendId); 			
	if(btVal)
	{		
		CGoods* pGoods = CGoodsFactory::UnserializeGoods( setReadDB,31 );		
		if( pGoods )
		{
#ifdef __MEMORY_LEAK_CHECK__
			CBaseObject::SetConstructFlag(72);
#endif
			m_pSubpackContainer->Add( ecColumn, pGoods, NULL );			
			m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->SetContainerVolume(pGoods);
		}				
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->Unserialize(setReadDB);
	}
	else
	{
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pGoods=NULL;
		m_pSubpackContainer->GetSubpack(ecColumn-PEI_PACK1)->pSubpackContainer->Clear();
	}
	return TRUE;
}


// ��ȡ �����ݿ���
bool CPlayer::LoadData()
{
	// ��ɫ����
//	if( !GetGame().GetRsPlayer()->LoadPlayer(this) ) return false;
/*///
	//GetBaseProperty()->wBaseMaxRp=GlobalSetup::GetBaseMaxRp(GetOccupation(), GetLevel());
	this->SetEnergy( GlobalSetup::GetBaseMaxRp(GetOccupation(), GetLevel()) );

	tagPlayerProperty *pDefaultProperty=PlayerList::GetBaseProperty(GetOccupation());
	if (pDefaultProperty)
	{
		//GetBaseProperty()->wBaseHpRecoverSpeed= pDefaultProperty->wBaseHpRecoverSpeed;
		this->SetHpRecoverSpeed (pDefaultProperty->wBaseHpRecoverSpeed);
		//GetBaseProperty()->wBaseMpRecoverSpeed = pDefaultProperty->wBaseMpRecoverSpeed;
		this->SetMpRecoverSpeed(pDefaultProperty->wBaseMpRecoverSpeed);
	}

	PlayerList::tagPropertiesUpgrade t;
	if( PlayerList::GetPropertiesUpgrade((PlayerList::eOccupation)GetOccupation(), GetLevel(), t) )
	{
		//GetBaseProperty()->wBaseMaxHp = t.wBaseMaxHp;
		this->SetMaxHP(t.wBaseMaxHp);
		//GetBaseProperty()->wBaseMaxMp = t.wBaseMaxMp;
		this->SetMaxMP(t.wBaseMaxMp);
		//GetBaseProperty()->wBaseStr	= t.wBaseStr;
		this->SetStrenth (t.wBaseStr);
		//GetBaseProperty()->wBaseDex	= t.wBaseDex;
		this->SetAgility (t.wBaseDex);
		//GetBaseProperty()->wBaseCon	= t.wBaseCon;
		this->SetConsti (t.wBaseCon);
		//GetBaseProperty()->wBaseInt	= t.wBaseInt;//����
		//GetBaseProperty()->wBaseBurden= t.wBaseBurden;//��������
	}
//*///
	//UpdateProperty();

	//SetRP(GetRP());
	this->SetEnergy(this->GetEnergy ());
	SetGraphicsID( GetSex()+1);
	//���ø���ҵ���֯��ϵ
	//GetOrganiCtrl()->SetPlayerOrganizing(this);
	return true;
}

/*
// �洢
bool CPlayer::SaveData(_ConnectionPtr cn)
{
	if(GetGame().GetRsPlayer()->SavePlayer(this, cn))
		return true;
	return false;
}
*/
void CPlayer::LoadDefaultProperty(BYTE lOccupation,BYTE lSex, BYTE byCountry,BYTE byConstellation)
{
	long lX=-1, lY=-1;

	// ���ݹ��ң������ó����أ������ԭʼ�����͸����в�����
	BYTE temp = byCountry;
	long lRegionID = GetCountryParam()->m_mpStartRegions[byCountry];

	CGUID RegionID;
	CGlobalRgnManager::tagRegion* pst = GetGame().GetGlobalRgnManager()->GetTemplateNormalRegion(lRegionID);

	long lDir = 0;
	char szLogingInfo[512]="";
	if(pst)
	{
		// Ϊ�����ӷ��߳���ID
		if(pst->pRegion && pst->pRegion->GetLinedIdFlag() > 0) // �Ƿ��߳���
		{
			map<long, vector<long>>::iterator itr = GetGame().GetGlobalRgnManager()->GetLinedRgnIdByLinedIdMap().find(pst->pRegion->GetLinedIdFlag());
			if(itr != GetGame().GetGlobalRgnManager()->GetLinedRgnIdByLinedIdMap().end())
			{
				unsigned int randPos = random((int)itr->second.size());
				if(randPos >= 0 && randPos < (unsigned int)itr->second.size())
					lRegionID = itr->second[randPos];
			}
		}

		long sx = GetCountryParam()->m_mpStartRect[byCountry].left;
		long sy = GetCountryParam()->m_mpStartRect[byCountry].top;
		long w = GetCountryParam()->m_mpStartRect[byCountry].right - sx;
		long h = GetCountryParam()->m_mpStartRect[byCountry].bottom - sy;
		pst->pRegion->GetRandomPosInRange(lX, lY, sx, sy, w, h);
		lDir = GetCountryParam()->m_mpStartDir[byCountry];

		_snprintf(szLogingInfo,512,"������ɫName:%s,regionid:%d,sx:%d,sy:%d,w:%d,h:%d,posx:%d,posy:%d,dir:%d",
			GetName(),lRegionID,sx,sy,w,h,lX,lY,lDir);
		
	}
	else
	{
		_snprintf(szLogingInfo,512,"������ɫ%sʱ,δ�ҵ�����regionid:%d",GetName(),lRegionID);
	}
	//PutStringToFile("Login_WS_Info",szLogingInfo);

    Log4c::Trace(ROOT_MODULE,szLogingInfo);
	
	SetRegionID(lRegionID);
	SetPosXY(lX+0.5f, lY+0.5f);
	SetDir(lDir);
	SetGraphicsID(lSex+1);
	SetRegionExID(RegionID);

	//ս��������gs�ϳ�ʼ��
	memset(&m_Property,0,sizeof(tagPFPro));
		
	SetCharged(false);
	//����
	m_Property.byCountry = byCountry;
	m_Property.byOccu = (eOccupation)lOccupation;
	m_Property.bySex = lSex;
	m_Property.wLevel = 1;
//	m_Property.byConst = (eConstellation)byConstellation;
	//ְҵ�ȼ�
	m_Property.byOccuLvl[m_Property.byOccu]=1;
	m_Property.dwHp=0xFFFFFFFF;
	m_Property.wMp=0xFFFF;
	time_t tTime;
	time(&tTime);
	m_Property.dwLastExitGameTime = (DWORD)tTime;
	//InitSkill();
}

void CPlayer::AddSkill(DWORD dwID,WORD Lv,WORD wCool)
{
	tagSkill stSkill;
	stSkill.wID = dwID;// GlobalSetup::GetSetup()->dwInitSkillID[i];
	stSkill.wLevel = Lv;
	stSkill.wCooldown = wCool;
	m_listSkillID.push_back(stSkill);
}

void CPlayer::InitSkill()
{
	//vector<long> lInitSkill = CNewSkillFactory::GetInitSkillList();
	//long lSkillCount = lInitSkill.size();
	//for (int i=0;i<lSkillCount;++i)
	//{
	//	tagSkill stSkill;
	//	stSkill.wID = lInitSkill[i];// GlobalSetup::GetSetup()->dwInitSkillID[i];
	//	stSkill.wLevel = 1;
	//	stSkill.wCooldown = 0;
	//	m_listSkillID.push_back(stSkill);
	//}
}

VOID CPlayer::SetCredit(DWORD dwAreaId,DWORD dwCredit)
{
	m_mapCredit[dwAreaId]=dwCredit;
}

VOID CPlayer::AddCredit(DWORD dwAreaId,long lVal)
{
	map<DWORD,DWORD>::iterator it=m_mapCredit.find(dwAreaId);
	if(it!=m_mapCredit.end())
		it->second+=lVal;
	else
		m_mapCredit[dwAreaId]=lVal;
}

DWORD CPlayer::GetCredit(DWORD dwAreaId)
{
	map<DWORD,DWORD>::iterator it=m_mapCredit.find(dwAreaId);
	if(it!=m_mapCredit.end())
		return it->second;
	return 0;
}

// ��������
void CPlayer::UpdateProperty()
{
	GlobalSetup::tagSetup *pSetup = GlobalSetup::GetSetup();
	//m_Property.wStr = GetBaseProperty()->wBaseStr;
	this->SetStr(this->GetBaseStr());
	//m_Property.wDex = GetBaseProperty()->wBaseDex;
	this->SetDex(this->GetBaseDex());
	//m_Property.wCon = GetBaseProperty()->wBaseCon;
	this->SetCon(this->GetBaseCon());
	//m_Property.wInt = GetBaseProperty()->wBaseInt;
	this->SetIntellect(this->GetBaseIntellect());
	//m_Property.wMaxHp = GetBaseProperty()->wBaseMaxHp + (WORD)(GetCon()*pSetup->fCon2MaxHp[GetOccupation()]);
	this->SetMaxHP ( this->GetBaseMaxHP ()+ (WORD)(this->GetCon()*pSetup->fCon2MaxHp[GetOccupation()]));
	//m_Property.wMaxMp = GetBaseProperty()->wBaseMaxMp + (WORD)(GetInt()*pSetup->fInt2MaxMp[GetOccupation()]);
	this->SetMaxMP ( this->GetBaseMaxMP () +(WORD)(this->GetIntellect()*pSetup->fInt2MaxMp[GetOccupation()]) );
	//m_Property.wMaxYp = GetBaseProperty()->wBaseMaxYp;
	
	//m_Property.wMaxRp = GetBaseProperty()->wBaseMaxRp;
	this->SetMaxEnergy ( this->GetBaseMaxEnergy ());
	//m_Property.wMinAtk = GetBaseProperty()->wBaseMinAtk + (WORD)(GetDex()*pSetup->fDex2MinAtk[GetOccupation()]);
	this->SetMinAtk ( this->GetBaseMinAtk () + (WORD)(this->GetDex()*pSetup->fDex2MinAtk[GetOccupation()]));
	//m_Property.wMaxAtk = GetBaseProperty()->wBaseMaxAtk + (WORD)(GetStr()*pSetup->fStr2MaxAtk[GetOccupation()]);
	this->SetMaxAtk ( this->GetBaseMaxAtk () + (WORD)(this->GetStr()*pSetup->fStr2MaxAtk[GetOccupation()]) );
	//m_Property.wHit = GetBaseProperty()->wBaseHit;
	this->SetHit ( this->GetBaseHit ());
	//m_Property.wBurden = GetBaseProperty()->wBaseBurden + (WORD)(GetStr()*pSetup->fStr2Burden[GetOccupation()]);
	//m_Property.wCCH = GetBaseProperty()->wBaseCCH;
	//m_Property.wDef = GetBaseProperty()->wBaseDef + (WORD)(GetCon()*pSetup->fCon2Def[GetOccupation()]);
	this->SetDef ( this->GetBaseDef () +(WORD)(this->GetCon()*pSetup->fCon2Def[GetOccupation()]) );
	//m_Property.wDodge = GetBaseProperty()->wBaseDodge;
	this->SetDodge (this->GetBaseDodge ());
	//m_Property.wAtcSpeed = GetBaseProperty()->wBaseAtcSpeed;
	//m_Property.wElementResistant = GetBaseProperty()->wBaseElementResistant + (WORD)(GetInt()*pSetup->fInt2Resistant[GetOccupation()]);
	//m_Property.wHpRecoverSpeed = GetBaseProperty()->wBaseHpRecoverSpeed;
	this->SetHpRecoverSpeed ( this->GetBaseHpRecoverSpeed ());
	//m_Property.wMpRecoverSpeed = GetBaseProperty()->wBaseMpRecoverSpeed;
	this->SetMpRecoverSpeed ( this->GetBaseMpRecoverSpeed ());
	//m_Property.sElementModify = (GetInt()*pSetup->fInt2Element[GetOccupation()]);
	//m_Property.wReAnk = (GetDex()*pSetup->fDex2Stiff[GetOccupation()]);

	//	m_Property.wSoulResistant = 0;//�Ѿ�ɾ����
}


// ��ӵ�CByteArray
bool CPlayer::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	CMoveShape::AddToByteArray(pByteArray, bExData);

	if( bExData )
	{
		int n=(long)sizeof(tagPlayerProperty);
        _AddToByteArray(pByteArray, (BYTE*)&m_Property,n);
		_AddToByteArray(pByteArray, (char*)GetAccount());
		_AddToByteArray(pByteArray, GetTeamID());
        _AddToByteArray(pByteArray, GetPhaseID());
        _AddToByteArray(pByteArray, (long)GetPhaseRest());

		//m_listSkillID
		_AddToByteArray(pByteArray, (long)m_listSkillID.size());
		list<tagSkill>::iterator SkillIter;
		for(SkillIter=m_listSkillID.begin();SkillIter!=m_listSkillID.end();SkillIter++)
		{
			//_AddToByteArray(pByteArray, (BYTE*)&(*SkillIter), (long)sizeof(tagSkill));
			_AddToByteArray(pByteArray,static_cast<DWORD>((*SkillIter).wID));
			_AddToByteArray(pByteArray,static_cast<DWORD>((*SkillIter).wLevel));
			_AddToByteArray(pByteArray,static_cast<DWORD>((*SkillIter).wCooldown));
		}

        //�������
        _AddToByteArray(pByteArray, (long)m_mapBrotherInfo.size());
        map<CGUID, tagBrotherInfo>::iterator itor = m_mapBrotherInfo.begin();
        for(; itor != m_mapBrotherInfo.end(); ++itor)
        {
            _AddToByteArray(pByteArray, (*itor).second.guidBrother);
            _AddToByteArray(pByteArray, (*itor).second.lFriendVal);
        }

		//##�쳣״̬
		_AddToByteArray( pByteArray, static_cast<DWORD>( m_vExStates.size() ) );
		if( m_vExStates.size() > 0 )
		{
			_AddToByteArray( pByteArray, &m_vExStates[0], static_cast<LONG>( m_vExStates.size() ) );
		}

			

		_AddToByteArray(pByteArray,m_lInvestLimitTimeMark);
		_AddToByteArray(pByteArray,(DWORD)m_mapInvestRecord.size());
		map<DWORD,DWORD>::iterator recordIter=m_mapInvestRecord.begin();
		for(;recordIter!=m_mapInvestRecord.end();recordIter++)
		{
			_AddToByteArray(pByteArray,recordIter->first);
			_AddToByteArray(pByteArray,recordIter->second);
		}	

		//װ�������л�
		m_cEquipment.Serialize(pByteArray,bExData);
        m_cEquipmentGod.Serialize(pByteArray, bExData);
        m_cEquipmentLand.Serialize(pByteArray, bExData);
        m_cEquipmentHuman.Serialize(pByteArray, bExData);

		//! ѫ��
		m_MedalContainer.AddToByteArray(*pByteArray);

		//�������л�
		m_cOriginPack->Serialize(pByteArray,bExData);
        m_pSubpackContainer->Serialize(pByteArray,bExData);

		_AddToByteArray(pByteArray,(DWORD)m_mapCredit.size());
		map<DWORD,DWORD>::iterator creditIter=m_mapCredit.begin();
		for(;creditIter!=m_mapCredit.end();creditIter++)
		{
			_AddToByteArray(pByteArray,creditIter->first);
			_AddToByteArray(pByteArray,creditIter->second);
		}
		
		m_cWallet.Serialize( pByteArray,bExData );
		m_cSilverWallet.Serialize(pByteArray,bExData);
		m_cGem.Serialize( pByteArray,bExData );		

		_AddToByteArray(pByteArray,m_lTicket);

		//! �ֿ�
		m_PlayerDepot.AddToByteArray_ForServer(pByteArray);
		//! С����ϵͳ
		m_SpriteSystem.AddToByteArray(*pByteArray);
		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.AddToByteArray(*pByteArray);
// �ֿ�
//		_AddToByteArray(pByteArray, (char*)GetDepotPassWord() );
//		m_cBank.Serialize( pByteArray, bExData );
		//m_cDepot.Serialize( pByteArray, bExData );

		//VariableList
		//��ͨ��������
		//_AddToByteArray(pByteArray, GetVariableNum());	// ����
		////GUID��������
		//_AddToByteArray(pByteArray, GetGuidNum());	// ����
		//_AddToByteArray(pByteArray,GetVariableDataLength());
		//_AddToByteArray(pByteArray, GetVariableData(), GetVariableDataLength());
		VarList::Encode( m_VarList, pByteArray );

		//SilenceTime
		_AddToByteArray(pByteArray, GetSilenceTime());

		//Godmode
		if( IsGod() )
			_AddToByteArray(pByteArray, static_cast<char>(1) );
		else 
			_AddToByteArray(pByteArray, static_cast<char>(0) );

		_AddToByteArray( pByteArray, GetMurdererTime() );

		//##ս��״̬ʱ��
		_AddToByteArray( pByteArray, GetFightStateCount() );


		////##����
		//_AddToByteArray( pByteArray, static_cast<DWORD>( m_vUncreatedPets.size() ) );
		//for( size_t i = 0; i < m_vUncreatedPets.size(); i ++ )
		//{
		//	_AddToByteArray( pByteArray, const_cast<CHAR*>( m_vUncreatedPets[i].strOriginalName.c_str() ) );
		//	_AddToByteArray( pByteArray, m_vUncreatedPets[i].dwHp );
		//	_AddToByteArray( pByteArray, m_vUncreatedPets[i].dwLevel );
		//	_AddToByteArray( pByteArray, m_vUncreatedPets[i].dwExperience );
		//}


		//����Ƿ��½
		_AddToByteArray( pByteArray,(char)m_bLogin);

		//��ӳ�ս����״̬ʱ��
		//_AddToByteArray( pByteArray, m_lCityWarDiedStateTime );

		//�����������
		AddQuestDataToByteArray(pByteArray);
		//���ٸ������
		AddMerQuestDataByteArray(pByteArray);

		//��Ӹ�ְҵ����
		m_DOccuRecord.AddDeputyOccuDataByteArray(pByteArray);

		// ����
		//_AddToByteArray( pByteArray, m_btCountry );
		_AddToByteArray( pByteArray, this->GetCountry() );
		//_AddToByteArray( pByteArray, m_lContribute );
		_AddToByteArray( pByteArray, this->GetCountryContribute() );


		//��֯ϵͳ
		AddOrgSysToByteArray(pByteArray);

		// ���˸���
		_AddToByteArray(pByteArray, GetPersonalRgnGUID());
		// ���˷���GUID
		_AddToByteArray(pByteArray, this->GetPersonalHouseRgnGUID());
		// ��Ӹ���
		GetGame().GetGlobalRgnManager()->RgnGuidCode(GetExID(), *pByteArray);
		// ��ǰ���˸�����
		_AddToByteArray(pByteArray, this->GetCurPRgnNum());
		// ��ǰ���鸱����
		_AddToByteArray(pByteArray, this->GetCurTRgnNum());

		//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
		/*
		long lSize = m_lstIncShopCur10.size();
		_AddToByteArray(pByteArray, lSize);
		for( list<DWORD>::iterator it=m_lstIncShopCur10.begin(); it!=m_lstIncShopCur10.end(); ++it )
		{
			_AddToByteArray(pByteArray, *it);
		}*/

		//! ��ǰ�ƺ�����
		_AddToByteArray(pByteArray, GetTitleType());
		// �Ƿ��һ�ε�¼
		_AddToByteArray(pByteArray, (BYTE)GetFirstLogin());
	}

	return true;
}

bool CPlayer::AddOrgSysToByteArray(vector<BYTE>* pByteArray)
{
	_AddToByteArray(pByteArray,GetFactionID());
	_AddToByteArray(pByteArray,GetUnionID());
	_AddToByteArray(pByteArray, m_lFacDemiseWarning);
	_AddToByteArray(pByteArray, m_lFacDisbandWarning);

	return true;
}

CMail	*CPlayer::GetMail(CGUID &guid)	
{
	map<CGUID,CMail*>::iterator it=m_mapMail.find(guid);
	if (it!=m_mapMail.end())
	{
		return (*it).second;
	}	
	return NULL;
	
}
void	CPlayer::ClearMailMap()
{
	m_mapMail.clear();
	map<CGUID,CMail*>::iterator it=m_mapMail.begin();
	for (;it!=m_mapMail.end();)
	{		
		it = m_mapMail.erase(it);
	}
}
//�ͷ�������ϵ��ʼ�
//void	CPlayer::ReleasePlayerMail(void)
//{
//	map<CGUID,CMail*>::iterator it=m_mapMail.begin();
//	for (;it!=m_mapMail.end();)
//	{		
//		SAFE_DELETE((*it).second);		
//	}
//	m_mapMail.clear();
//}
void	CPlayer::EraseMail(const CGUID &guid)
{
	list<CGUID>::iterator it = find(m_lReceiveMail.begin(),m_lReceiveMail.end(),guid);
	if (it!=m_lReceiveMail.end())
	{
		it = m_lReceiveMail.erase(it);
	}
}
void	CPlayer::DeleteMail(list<CGUID> &lguid)
{
	
}
// ����
bool CPlayer::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
#ifdef _RUNSTACKINFO1_
	char pszStackInfo[1024]="";
	_snprintf(pszStackInfo,1024,"CPlayer::DecordFromByteArray(Name:%s) Start", GetName());
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
	m_listSkillID.clear();
    m_GeniusList.clear();
    m_mapBrotherInfo.clear();
	m_vExStates.clear();
	m_mapSysMail.clear();
	//m_vUncreatedPets.clear();

	CMoveShape::DecordFromByteArray(pByte, pos, bExData);

	if( bExData )
	{
		char strTemp[256];
		_GetBufferFromByteArray(pByte, pos, &m_Property, (long)sizeof(tagPPro));
		_GetStringFromByteArray(pByte, pos, strTemp);
		SetAccount(strTemp);
		//m_TeamGuid = _GetLongFromByteArray(pByte, pos);
		_GetBufferFromByteArray(pByte, pos, m_TeamGuid);

		// skill
		long lSkillNum = _GetLongFromByteArray(pByte, pos);
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord Skill(Num:%d)",lSkillNum);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		tagSkill stSkill;
		for(int i=0; i<lSkillNum; i++)
		{			
			stSkill.wID = _GetDwordFromByteArray(pByte,pos);
			stSkill.wLevel = (WORD)_GetDwordFromByteArray(pByte,pos);
			stSkill.wCooldown = _GetDwordFromByteArray(pByte,pos);
			m_listSkillID.push_back(stSkill);
		}

        long  lGeniusNum = _GetLongFromByteArray(pByte, pos);
        for ( int i = 0 ; i < lGeniusNum ; i++ )
        {
            long gid = _GetLongFromByteArray(pByte, pos);
            long lvl = _GetLongFromByteArray(pByte, pos);

            m_GeniusList[ gid ] = lvl ;
        }


		//##�쳣״̬
		DWORD dwBytes = _GetDwordFromByteArray( pByte, pos );
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord buff(Num:%d)",dwBytes);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		if( dwBytes > 0 )
		{
			SetExStates( &pByte[pos], dwBytes );
			pos += dwBytes;
		}

		// friendname
			

		/* NEW */

		m_lInvestLimitTimeMark=_GetLongFromByteArray(pByte,pos);
		DWORD dwRecordSize=_GetDwordFromByteArray(pByte,pos);
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord Invest(Num:%d)",dwRecordSize);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		m_mapInvestRecord.clear();
		for(DWORD i=0;i<dwRecordSize;i++)
		{
			DWORD dwSpotId=_GetDwordFromByteArray(pByte,pos);
			DWORD dwInvestValue=_GetDwordFromByteArray(pByte,pos);
			m_mapInvestRecord[dwSpotId]=dwInvestValue;
		}
		
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord SubpackContainer");
#endif
		//m_pSubpackContainer->Release();
		m_pSubpackContainer->Unserialize(pByte,pos);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_cEquipment");
#endif
		/* NEW */
		m_cEquipment.Release();
		m_cEquipment.Unserialize(pByte,pos,bExData);
        m_cEquipmentGod.SetContainerVolume(8);
        m_cEquipmentGod.Unserialize(pByte, pos, bExData);
        m_cEquipmentLand.SetContainerVolume(8);
        m_cEquipmentLand.Unserialize(pByte, pos, bExData);
        m_cEquipmentHuman.SetContainerVolume(8);
        m_cEquipmentHuman.Unserialize(pByte, pos, bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_MedalContainer");
#endif
		//! ѫ��
		m_MedalContainer.Decode(pByte, pos);

		
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord getOriginPack");
#endif
		/* NEW */
		getOriginPack()->Release();
		getOriginPack()->SetContainerVolume(5*4);
#ifdef __MEMORY_LEAK_CHECK__
		CBaseObject::SetConstructFlag(782);
#endif
		getOriginPack()->Unserialize(pByte,pos,bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_mapCredit");
#endif
		m_mapCredit.clear();

		DWORD dwCreditSize=_GetDwordFromByteArray(pByte,pos);
		for(DWORD i=0;i<dwCreditSize;i++)
		{
			DWORD dwAreaId=_GetDwordFromByteArray(pByte,pos);
			DWORD dwCredit=_GetDwordFromByteArray(pByte,pos);
			m_mapCredit[dwAreaId]=dwCredit;
		}
		
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_cWallet");
#endif
		m_cWallet.Release();
		m_cWallet.Unserialize(pByte,pos,bExData);

		m_cSilverWallet.Release();
		m_cSilverWallet.Unserialize(pByte,pos,bExData);

		m_cGem.Release();
		m_cGem.Unserialize(pByte,pos,bExData);

		m_lTicket=_GetDwordFromByteArray(pByte,pos);

		//! �ֿ�
		m_PlayerDepot.Release();
		m_PlayerDepot.GetFormByteArray_FromServer(pByte, pos);
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_SpriteSystem");
#endif
		//! С����ϵͳ
		m_SpriteSystem.GetFormByteArray(pByte, pos);
		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.GetFormByteArray(pByte, pos);

// �ֿ� 
		//_GetStringFromByteArray(pByte,pos,strTemp);
		//SetDepotPassWord( strTemp );

//		m_cBank.Release();
//		m_cBank.Unserialize(pByte,pos,bExData);

		//m_cDepot.Release();
		//m_cDepot.SetContainerVolume(8*12);
		//m_cDepot.Unserialize(pByte, pos, bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord VariableData");
#endif
		//VarialbeList
		//��ͨ��������
		//long tNum =_GetLongFromByteArray(pByte, pos);
		////GUID����
		//long guidNum = _GetLongFromByteArray(pByte, pos);
		//SetVariableNum(tNum);
		//SetGuidNum(guidNum);
		////�������鳤��
		//long varLength = _GetLongFromByteArray(pByte, pos);

		//SetVariableDataLength(varLength);
		//char* pData = new char[varLength];
		//SetVariableData((char*)_GetBufferFromByteArray(pByte, pos, pData, varLength));
		m_VarList->RemoveAll();
		VarList::Decode( m_VarList, pByte, (size_t)(-1), pos );

		// Silence
		SetSilenceTime(_GetLongFromByteArray(pByte, pos));

		// GodMode
		if( _GetCharFromByteArray(pByte, pos) )
			God();
		else 
			God( false );

		//##ɱ��������ʱ��
		DWORD dwMurdererTime		= _GetDwordFromByteArray( pByte, pos );
		SetMurdererTime( dwMurdererTime );

		//##ս��״̬ʱ��
		LONG lFightStateTime		= _GetLongFromByteArray( pByte, pos );
		SetFightStateCount( lFightStateTime );

		//##����
		//char strName[256] = {0};
		//DWORD dwPetsAmount = _GetDwordFromByteArray( pByte, pos );
		//for( DWORD i = 0; i < dwPetsAmount; i ++ )
		//{
		//	tagPetInformation tPet;
		//	tPet.strOriginalName	= _GetStringFromByteArray( pByte, pos, strName );
		//	tPet.dwHp				= _GetDwordFromByteArray( pByte, pos );
		//	tPet.dwLevel			= _GetDwordFromByteArray( pByte, pos );
		//	tPet.dwExperience		= _GetDwordFromByteArray( pByte, pos );
		//	m_vUncreatedPets.push_back( tPet );
		//}


		m_bLogin = _GetCharFromByteArray( pByte,pos )==0?false:true;
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord QuestData");
#endif
		//�õ���������
		DecordQuestDataFromByteArray(pByte,pos);
		//��ٸ������
		DecordMerQuestDataFromByteArray(pByte,pos);
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord OccuRecord");
#endif
		//�õ���ְҵ����
		m_DOccuRecord.DecordDeputyOccuDataFromByteArray(pByte,pos);

		// ����
		//m_btCountry = _GetByteFromByteArray( pByte, pos );
		this->SetCountry (_GetByteFromByteArray( pByte, pos ) );
		//m_lContribute = _GetLongFromByteArray( pByte, pos );
		this->SetCountryContribute( _GetLongFromByteArray( pByte, pos ) );
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord PersonalRgn");
#endif
		CGUID tGUID;
		// ���˸���GUID
		_GetBufferFromByteArray(pByte, pos, tGUID);
		SetPersonalRgnGUID(tGUID);
		// ���˷���GUID
		_GetBufferFromByteArray(pByte, pos, tGUID);
		SetPersonalHouseRgnGUID(tGUID);
		// ��Ӹ���GUID
		GetGame().GetGlobalRgnManager()->RgnGuidDecode(GetExID(), pByte, pos);

		// ��ǰ���˸�����
		SetCurPRgnNum(_GetLongFromByteArray(pByte, pos));
		// ��ǰ���鸱����
		SetCurTRgnNum(_GetLongFromByteArray(pByte, pos));

		//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
		/*
		long lSize = _GetLongFromByteArray(pByte, pos);
		m_lstIncShopCur10.clear();
		for( long l=0; l<lSize; ++l )
		{
			DWORD dwID = _GetDwordFromByteArray(pByte, pos);
			m_lstIncShopCur10.push_back(dwID);
		}*/

		//! ��ǰ�ƺ�����
		SetTitleType(_GetLongFromByteArray(pByte, pos));
		// �Ƿ��һ�ε�¼
		SetFirstLogin( (0 != _GetByteFromByteArray(pByte, pos)) );

		//�ʼ�
		long lNum = _GetLongFromByteArray(pByte,pos);
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord Invest(Num:%d)",lNum);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif		
	}

	//UpdateProperty();
#ifdef _RUNSTACKINFO1_
	_snprintf(pszStackInfo,1024,"CPlayer::DecordFromByteArray(Name:%s) End", GetName());
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
	return true;
}


bool CPlayer::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData )
{
	CMoveShape::CodeToDataBlock(setWriteDB, bExData);

	if( bExData )
	{
		int n=(long)sizeof(tagPlayerProperty);
        setWriteDB.AddToByteArray((BYTE*)&m_Property,n);
		//setWriteDB.AddToByteArray(&m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
		setWriteDB.AddToByteArray( (char*)GetAccount());
		setWriteDB.AddToByteArray( GetTeamID());
        setWriteDB.AddToByteArray( GetPhaseID());
        setWriteDB.AddToByteArray( (long)GetPhaseRest());

		//m_listSkillID
		setWriteDB.AddToByteArray( (long)m_listSkillID.size());
		list<tagSkill>::iterator SkillIter;
		for(SkillIter=m_listSkillID.begin();SkillIter!=m_listSkillID.end();SkillIter++)
		{
			//setWriteDB.AddToByteArray( (BYTE*)&(*SkillIter), (long)sizeof(tagSkill));
			setWriteDB.AddToByteArray(static_cast<DWORD>((*SkillIter).wID));
			setWriteDB.AddToByteArray(static_cast<DWORD>((*SkillIter).wLevel));
			setWriteDB.AddToByteArray(static_cast<DWORD>((*SkillIter).wCooldown));
		}

        /// �츳���뵽�ͻ���
        setWriteDB.AddToByteArray( (long) m_GeniusList.size() );
        for ( std::map<long,long>::iterator itr = m_GeniusList.begin(); itr != m_GeniusList.end() ; itr ++ )
        {
            setWriteDB.AddToByteArray( itr->first );
            setWriteDB.AddToByteArray( itr->second);
        }

        //�������
        setWriteDB.AddToByteArray((long)m_mapBrotherInfo.size());
        map<CGUID, tagBrotherInfo>::iterator itor = m_mapBrotherInfo.begin();
        for(; itor != m_mapBrotherInfo.end(); ++itor)
        {
            setWriteDB.AddToByteArray((*itor).second.guidBrother);
            setWriteDB.AddToByteArray((*itor).second.lFriendVal);
        }

		//##�쳣״̬
		setWriteDB.AddToByteArray( static_cast<DWORD>( m_vExStates.size() ) );
		if( m_vExStates.size() > 0 )
		{
			setWriteDB.AddToByteArray( &m_vExStates[0], static_cast<LONG>( m_vExStates.size() ) );
		}

			

		setWriteDB.AddToByteArray(m_lInvestLimitTimeMark);
		setWriteDB.AddToByteArray((DWORD)m_mapInvestRecord.size());
		map<DWORD,DWORD>::iterator recordIter=m_mapInvestRecord.begin();
		for(;recordIter!=m_mapInvestRecord.end();recordIter++)
		{
			setWriteDB.AddToByteArray(recordIter->first);
			setWriteDB.AddToByteArray(recordIter->second);
		}

		//װ�������л�
		m_cEquipment.Serialize(setWriteDB);
        m_cEquipmentGod.Serialize(setWriteDB);
        m_cEquipmentLand.Serialize(setWriteDB);
        m_cEquipmentHuman.Serialize(setWriteDB);

		//! ѫ��
		m_MedalContainer.CodeToDataBlock(setWriteDB);

		//ԭʼ�������л�
		m_cOriginPack->Serialize(setWriteDB);
        m_pSubpackContainer->Serialize(setWriteDB);
        m_cWallet.Serialize( setWriteDB,bExData );
        m_cSilverWallet.Serialize(setWriteDB,bExData);
        m_cGem.Serialize( setWriteDB,bExData );

		setWriteDB.AddToByteArray((DWORD)m_mapCredit.size());
		map<DWORD,DWORD>::iterator creditIter=m_mapCredit.begin();
		for(;creditIter!=m_mapCredit.end();creditIter++)
		{
			setWriteDB.AddToByteArray(creditIter->first);
			setWriteDB.AddToByteArray(creditIter->second);
		}

		//�ع�		
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

		//��Ʒ��ȴ
		setWriteDB.AddToByteArray((DWORD)m_mapClassIdCoolDown.size());
		map<DWORD,DWORD>::iterator classIdCooldownIter=m_mapClassIdCoolDown.begin();
		for(;classIdCooldownIter!=m_mapClassIdCoolDown.end();classIdCooldownIter++)
		{
			setWriteDB.AddToByteArray(classIdCooldownIter->first);
			setWriteDB.AddToByteArray(classIdCooldownIter->second);
		}

		setWriteDB.AddToByteArray((DWORD)m_mapGoodsIdCoolDown.size());
		map<DWORD,DWORD>::iterator goodsIdCooldownIter=m_mapGoodsIdCoolDown.begin();
		for(;goodsIdCooldownIter!=m_mapGoodsIdCoolDown.end();goodsIdCooldownIter++)
		{
			setWriteDB.AddToByteArray(goodsIdCooldownIter->first);
			setWriteDB.AddToByteArray(goodsIdCooldownIter->second);
		}


		setWriteDB.AddToByteArray(m_lTicket);

		//! �ֿ�
		m_PlayerDepot.CodeToDataBlock_ForServer(setWriteDB);
		//! С����ϵͳ
		m_SpriteSystem.CodeToDataBlock(setWriteDB);
		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.CodeToDataBlock(setWriteDB);

		//VariableList
		//��ͨ��������
		//setWriteDB.AddToByteArray( GetVariableNum());	// ����
		////GUID��������
		//setWriteDB.AddToByteArray( GetGuidNum());	// ����
		//setWriteDB.AddToByteArray(GetVariableDataLength());
		//setWriteDB.AddToByteArray( GetVariableData(), GetVariableDataLength());
		VarList::Encode( m_VarList, setWriteDB );

		//SilenceTime
		setWriteDB.AddToByteArray( GetSilenceTime());

		//Godmode
		if( IsGod() )
			setWriteDB.AddToByteArray( static_cast<char>(1) );
		else 
			setWriteDB.AddToByteArray( static_cast<char>(0) );

		setWriteDB.AddToByteArray( GetMurdererTime() );

		//##ս��״̬ʱ��
		setWriteDB.AddToByteArray( GetFightStateCount() );


		//����Ƿ��½
		setWriteDB.AddToByteArray((char)m_bLogin);

		//��ӳ�ս����״̬ʱ��
		//_AddToByteArray( pByteArray, m_lCityWarDiedStateTime );

		//�����������
		CodeQuestDataToDataBlock(setWriteDB);
		//���ٸ������
		CodeMerQuestDataToDataBlock(setWriteDB);

		//��Ӹ�ְҵ����
		m_DOccuRecord.CodeToDataBlock(setWriteDB);

		// ����
		//setWriteDB.AddToByteArray( this->GetCountry() );
		//setWriteDB.AddToByteArray( this->GetCountryContribute() );
//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
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

		//ϵͳ�ʼ��б�
		long lNum = (long)m_mapSysMail.size();
		setWriteDB.AddToByteArray(lNum);
		map<long,long>::iterator it=m_mapSysMail.begin();
		for (;it!=m_mapSysMail.end();++it)
		{
			setWriteDB.AddToByteArray((*it).first);
			setWriteDB.AddToByteArray((*it).second);
		}
		
		// �Ƿ��һ�ε�¼
		setWriteDB.AddToByteArray( (BYTE)GetFirstLogin());
		//��֯ϵͳ
		CodeOrgSysToDataBlock(setWriteDB);

		//! ��ǰ�ƺ�����
		setWriteDB.AddToByteArray( GetTitleType());

		// ���˸���
		setWriteDB.AddToByteArray( GetPersonalRgnGUID());
		// ���˷���GUID
		setWriteDB.AddToByteArray( this->GetPersonalHouseRgnGUID());
		// ��Ӹ���
		//GetGame().GetGlobalRgnManager()->RgnGuidCode(GetExID(), setWriteDB);
		// ��ǰ���˸�����
		setWriteDB.AddToByteArray( this->GetCurPRgnNum());
		// ��ǰ���鸱����
		setWriteDB.AddToByteArray( this->GetCurTRgnNum());

		// Fox@20081225 ����
		GetInst(CPetCtrl).CodeToDataBlock(setWriteDB, this);

		// ������ҵ����
		m_BusinessContainer.EncodeForGS( setWriteDB );

		// ����״̬
		setWriteDB.AddToByteArray((long)m_wState);

		setWriteDB.AddToByteArray((BYTE)GetShowFashion());

		// ���뷴���������� BAIYUN@20090603
		setWriteDB.AddToByteArray(&this->m_ACProperty, sizeof(AntiCheatData));
		ReleaseNpcShopSellGoods();
	}
	return true;
}
bool CPlayer::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
#ifdef _RUNSTACKINFO1_
	char pszStackInfo[1024]="";
	_snprintf(pszStackInfo,1024,"CPlayer::DecodeFromDataBlock(Name:%s) Start", GetName());
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
	m_listSkillID.clear();
    m_mapBrotherInfo.clear();
	m_vExStates.clear();
	m_mapSysMail.clear();
    m_GeniusList.clear();

	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);

	if( bExData )
	{
		ReleaseNpcShopSellGoods();
		char strTemp[256];
		setReadDB.GetBufferFromByteArray( &m_Property, (long)sizeof(tagPPro));
		//setReadDB.GetBufferFromByteArray( &m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
		setReadDB.GetStringFromByteArray( strTemp,256);
		SetAccount(strTemp);
		setReadDB.GetBufferFromByteArray( m_TeamGuid);

		// skill
		long lSkillNum = setReadDB.GetLongFromByteArray( );
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord Skill(Num:%d)",lSkillNum);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		tagSkill stSkill;
		for(int i=0; i<lSkillNum; i++)
		{			
			stSkill.wID = setReadDB.GetDwordFromByteArray( );
			stSkill.wLevel = (WORD)setReadDB.GetDwordFromByteArray( );
			stSkill.wCooldown = setReadDB.GetDwordFromByteArray( );
			m_listSkillID.push_back(stSkill);
		}

        long  lGeniusNum = setReadDB.GetLongFromByteArray();
        for ( int i = 0 ; i < lGeniusNum ; i++ )
        {
            long gid = setReadDB.GetLongFromByteArray();
            long lvl = setReadDB.GetLongFromByteArray();

            m_GeniusList[ gid ] = lvl ;
        }

        // �������
        long lBrotherNum = setReadDB.GetLongFromByteArray();
        tagBrotherInfo stBrother;
        for(long l=0; l<lBrotherNum; l++)
        {
            setReadDB.GetBufferFromByteArray(stBrother.guidBrother);
            setReadDB.GetLongFromByteArray();
            m_mapBrotherInfo[stBrother.guidBrother] = stBrother;
        }

		//##�쳣״̬
		DWORD dwBytes = setReadDB.GetDwordFromByteArray(   );
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord buff(Num:%d)",dwBytes);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		BYTE* pTemptByte = new BYTE[dwBytes];
		setReadDB.GetBufferFromByteArray(pTemptByte,dwBytes);
		if( dwBytes > 0 )
			SetExStates( pTemptByte, dwBytes );
		SAFE_DELETE_ARRAY(pTemptByte);

		m_lInvestLimitTimeMark=setReadDB.GetLongFromByteArray( );
		DWORD dwRecordSize=setReadDB.GetDwordFromByteArray( );
#ifdef _RUNSTACKINFO1_
		_snprintf(pszStackInfo,1024,"Decord Invest(Num:%d)",dwRecordSize);
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
		m_mapInvestRecord.clear();
		for(DWORD i=0;i<dwRecordSize;i++)
		{
			DWORD dwSpotId=setReadDB.GetDwordFromByteArray( );
			DWORD dwInvestValue=setReadDB.GetDwordFromByteArray( );
			m_mapInvestRecord[dwSpotId]=dwInvestValue;
		}


#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_cEquipment");
#endif
		/* NEW */
		m_cEquipment.Release();
		m_cEquipment.Unserialize(setReadDB ,bExData);
        m_cEquipmentGod.SetContainerVolume(8);
        m_cEquipmentGod.Unserialize(setReadDB, bExData);
        m_cEquipmentLand.SetContainerVolume(8);
        m_cEquipmentLand.Unserialize(setReadDB, bExData);
        m_cEquipmentHuman.SetContainerVolume(8);
        m_cEquipmentHuman.Unserialize(setReadDB, bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_MedalContainer");
#endif
		//! ѫ��
		m_MedalContainer.DecodeFromDataBlock( setReadDB);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord getOriginPack");
#endif
		/* NEW */
		m_cOriginPack->Release();
		m_cOriginPack->SetContainerVolume(5*4);

#ifdef __MEMORY_LEAK_CHECK__
        CBaseObject::SetConstructFlag(782);
#endif
        getOriginPack()->Unserialize(setReadDB ,bExData);

#ifdef _RUNSTACKINFO1_
        CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord SubpackContainer");
#endif
        m_pSubpackContainer->Unserialize( setReadDB );

        m_cWallet.Unserialize(setReadDB ,bExData);
        m_cSilverWallet.Unserialize(setReadDB ,bExData);
        m_cGem.Unserialize(setReadDB ,bExData);

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_mapCredit");
#endif
		m_mapCredit.clear();

		DWORD dwCreditSize=setReadDB.GetDwordFromByteArray( );
		for(DWORD i=0;i<dwCreditSize;i++)
		{
			DWORD dwAreaId=setReadDB.GetDwordFromByteArray( );
			DWORD dwCredit=setReadDB.GetDwordFromByteArray( );
			m_mapCredit[dwAreaId]=dwCredit;
		}


#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_cWallet");
#endif
		
		DWORD dwSellGoodsSize=setReadDB.GetDwordFromByteArray();
		for(DWORD i=0;i<dwSellGoodsSize;i++)
		{
			CGoods* pGoods = CGoodsFactory::UnserializeGoods( setReadDB,32 );
			DWORD dwSellPrice=setReadDB.GetDwordFromByteArray();
			if( pGoods )
			{
				PushbakNpcShopSellGoods(pGoods,dwSellPrice);
			}
		}

		//��Ʒ��ȴ
		m_mapClassIdCoolDown.clear();
		DWORD dwCooldownSize=setReadDB.GetDwordFromByteArray();
		DWORD dwCooldownId=0,dwCooldownTime=0;
		for(DWORD i=0;i<dwCooldownSize;i++)
		{
			dwCooldownId=setReadDB.GetDwordFromByteArray();
			dwCooldownTime=setReadDB.GetDwordFromByteArray();
			m_mapClassIdCoolDown[dwCooldownId]=dwCooldownTime;
		}

		m_mapGoodsIdCoolDown.clear();
		dwCooldownSize=setReadDB.GetDwordFromByteArray();
		for(DWORD i=0;i<dwCooldownSize;i++)
		{
			dwCooldownId=setReadDB.GetDwordFromByteArray();
			dwCooldownTime=setReadDB.GetDwordFromByteArray();
			m_mapGoodsIdCoolDown[dwCooldownId]=dwCooldownTime;
		}

		m_lTicket=setReadDB.GetDwordFromByteArray( );

		//! �ֿ�
		m_PlayerDepot.Release();
		m_PlayerDepot.DecodeFormDataBlock_FromServer( setReadDB);
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord m_SpriteSystem");
#endif
		//! С����ϵͳ
		m_SpriteSystem.DecodeFromDataBlock( setReadDB);
		//! �����Ҫ�����ڷ�����������
		m_SetupOnServer.DecodeFromDataBlock( setReadDB );

#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord VariableData");
#endif
		//VarialbeList
		//��ͨ��������
		//long tNum =setReadDB.GetLongFromByteArray( );
		////GUID����
		//long guidNum = setReadDB.GetLongFromByteArray( );
		//SetVariableNum(tNum);
		//SetGuidNum(guidNum);
		////�������鳤��
		//long varLength = setReadDB.GetLongFromByteArray( );

		//SetVariableDataLength(varLength);
		//char* pData = new char[varLength];
		//SetVariableData((char*)setReadDB.GetBufferFromByteArray( pData, varLength));
		m_VarList->RemoveAll();
		VarList::Decode( m_VarList, setReadDB );

		// Silence
		SetSilenceTime(setReadDB.GetLongFromByteArray( ));

		// GodMode
		if( setReadDB.GetCharFromByteArray( ) )
			God();
		else 
			God( false );

		//##ɱ��������ʱ��
		DWORD dwMurdererTime		= setReadDB.GetDwordFromByteArray(   );
		SetMurdererTime( dwMurdererTime );

		//##ս��״̬ʱ��
		LONG lFightStateTime		= setReadDB.GetLongFromByteArray(   );
		SetFightStateCount( lFightStateTime );


		m_bLogin = setReadDB.GetCharFromByteArray(   )==0?false:true;
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord QuestData");
#endif
		//�õ���������
		DecodeQuestDataFromDataBlock( setReadDB );
		//��ٸ������
		DecodeMerQuestDataFromDataBlock( setReadDB );
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord OccuRecord");
#endif
		//�õ���ְҵ����
		m_DOccuRecord.DecodeFromDataBlock( setReadDB );

		// ����
		//SetCountry (setReadDB.GetByteFromByteArray(   ) );
		//SetCountryContribute( setReadDB.GetLongFromByteArray(   ) );
#ifdef _RUNSTACKINFO1_
		CMessage::AsyWriteFile(GetGame().GetStatckFileName(),"Decord PersonalRgn");
#endif
		CGUID tGUID;
		// ���˸���GUID
		setReadDB.GetBufferFromByteArray( tGUID);
		SetPersonalRgnGUID(tGUID);
		// ���˷���GUID
		setReadDB.GetBufferFromByteArray( tGUID);
		SetPersonalHouseRgnGUID(tGUID);
		// ��Ӹ���GUID
		//GetGame().GetGlobalRgnManager()->RgnGuidDecode( GetExID(), setReadDB );

		// ��ǰ���˸�����
		SetCurPRgnNum(setReadDB.GetLongFromByteArray( ));
		// ��ǰ���鸱����
		SetCurTRgnNum(setReadDB.GetLongFromByteArray( ));

		//-------------------------------------------------------------------------------
		// ��������10����Ʒ�б�. by Fox.		2008-02-21
		DWORD dwSize = setReadDB.GetDwordFromByteArray( );
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

		//��ֵ�̵����������¼�б�
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

		//! ��ǰ�ƺ�����
		SetTitleType(setReadDB.GetLongFromByteArray( ));
		// �Ƿ��һ�ε�¼
		SetFirstLogin( (0 != setReadDB.GetByteFromByteArray( )) );

		//ϵͳ�ʼ��б�
		long lNum = setReadDB.GetLongFromByteArray();
		for (int i=0; i<lNum; ++i)
		{
			long lID = setReadDB.GetLongFromByteArray();
			long lTime = setReadDB.GetLongFromByteArray();
			AddSysMail(lID,lTime);
		}

		// �����б�
		GetInst(CPetCtrl).DecodeFromDataBlock(setReadDB, this);

		// ������ҵ����
		m_BusinessContainer.DecodeFromGS( this, setReadDB );

		// ����״̬
		m_wState = (WORD)setReadDB.GetLongFromByteArray();

		SetShowFashion(setReadDB.GetByteFromByteArray());

		// ���뷴���������� BAIYUN@20090603
		if( GetInst( AntiCheat::CConfiger ).IsEnable() )
		{
			BYTE byte = setReadDB.GetByteFromByteArray();
			if(byte==1)
			{
				AntiCheatData tmpACData;
				setReadDB.GetBufferFromByteArray(&tmpACData, sizeof(AntiCheatData));
				SetACProperty(&tmpACData);
			}
		}
	}

	//UpdateProperty();
#ifdef _RUNSTACKINFO1_
	_snprintf(pszStackInfo,1024,"CPlayer::DecodeFromDataBlock(Name:%s) End", GetName());
	CMessage::AsyWriteFile(GetGame().GetStatckFileName(),pszStackInfo);
#endif
	return true;
}
bool CPlayer::CodeOrgSysToDataBlock(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(GetTeamID());
	setWriteDB.AddToByteArray(GetFactionID());
	setWriteDB.AddToByteArray(GetUnionID());
	setWriteDB.AddToByteArray(m_lFacDemiseWarning);
	setWriteDB.AddToByteArray(m_lFacDisbandWarning);
	return true;
}


//����ڱ������Ƿ����ĳ�����ֵ���Ʒ
long CPlayer::CheckGoodsInPacket(char* strGoodName)
{
	long lRet = 0;
	if(strGoodName == NULL)
		return lRet;
	CSeekGoodsListener lSeeker;
	lSeeker.SetTarget( strGoodName );

	getOriginPack()->TraversingContainer( &lSeeker );
	for( size_t i = 0; i < lSeeker.m_vGoodsID.size(); i ++ )
	{
		CGoods* pGoods = dynamic_cast<CGoods*>(getOriginPack()->Find(TYPE_GOODS, lSeeker.m_vGoodsID[i]) );
		if( pGoods )
		{
			lRet += pGoods->GetAmount();
		}
	}
	return lRet;
}


//������ʼ��б�
void	CPlayer::AddMailList(CGUID &MailGuid)
{
	list<CGUID>::iterator it = find(m_lNewMail.begin(),m_lNewMail.end(),MailGuid);
	if (it == m_lNewMail.end())
	{
		m_lNewMail.push_back(MailGuid);
	}
}

//��ȡ�ʼ��б�
list<CGUID>      &CPlayer::GetNewMailList()
{
	return m_lNewMail;
}


//��������ʼ��б�
void	CPlayer::AddReceiveMailList(CGUID MailGuid)	
{
	list<CGUID>::iterator it = find(m_lReceiveMail.begin(),m_lReceiveMail.end(),MailGuid);
	if (it == m_lReceiveMail.end())
	{
		m_lReceiveMail.push_back(MailGuid);
	}
}

//��ȡ�����ʼ��б�
list<CGUID>		&CPlayer::GetReceiveMailList()
{
	return m_lReceiveMail;
}

DWORD CPlayer::GetMoney()
{
	return m_cWallet.GetGoldCoinsAmount();
}

DWORD CPlayer::GetGem()
{
	return m_cGem.GetGoldCoinsAmount();
}

DWORD   CPlayer::GetSilver()
{
	return m_cSilverWallet.GetGoldCoinsAmount();
}

void	CPlayer::AddSysMail(long lID,long lEndTime)
{
	map<long,long>::iterator it = m_mapSysMail.find(lID);
	if (it==m_mapSysMail.end())
	{
		m_mapSysMail[lID] = lEndTime;
	}
}

map<long,long>  &CPlayer::GetSysMail()
{
	return m_mapSysMail;
}
//���ͻ��˷���

//����������ݵ�ByteArray
BOOL CPlayer::AddQuestDataToByteArray(vector<BYTE>* pByteArray)
{
	/*
	long lNum = m_PlayerQuests.size();
	_AddToByteArray(pByteArray,lNum);
	map<WORD,tagPlayerQuest>::iterator it = m_PlayerQuests.begin();
	for(;it != m_PlayerQuests.end();it++)
	{
		_AddToByteArray(pByteArray,(*it).second.wQuestID);
		_AddToByteArray(pByteArray,(BYTE)(*it).second.byComplete);
	}
	*/
	return m_RoleQueue.AddQuestDataByteArray(pByteArray);
}

BOOL CPlayer::DecordQuestDataFromByteArray(BYTE* pByte, long& pos)
{
	/*
	m_PlayerQuests.clear();
	long lNum = _GetLongFromByteArray(pByte, pos);
	for(int i = 0;i < lNum;i++)
	{
		tagPlayerQuest PlayerQuest;
		PlayerQuest.wQuestID = _GetWordFromByteArray(pByte, pos);
		PlayerQuest.byComplete = _GetByteFromByteArray(pByte,pos);
		m_PlayerQuests[PlayerQuest.wQuestID] = PlayerQuest;
	}
	*/
	return m_RoleQueue.DecordQuestDataFromByteArray(pByte,pos);
}

BOOL CPlayer::CodeQuestDataToDataBlock(DBWriteSet& setWriteDB)
{
	return m_RoleQueue.CodeToDataBlock(setWriteDB);
}
BOOL CPlayer::DecodeQuestDataFromDataBlock(DBReadSet& setReadDB)
{
	return m_RoleQueue.DecodeFromDataBlock(setReadDB);
}
//�õ�һ�������״̬(0:���ڵ�û���,1:���������,2:û��)
long CPlayer::GetQuestState(WORD wQuestID)
{
	//map<WORD,tagPlayerQuest>::iterator it = m_PlayerQuests.find(wQuestID);
	//if(it == m_PlayerQuests.end())	return 2;
	//return (*it).second.byComplete;
	if(m_RoleQueue.GetQuestFinishedRecord(wQuestID))
	{
		return 1;
	}
	else if (m_RoleQueue.GetRecordByID(wQuestID)!=0)
	{
		return 0;
	}
	else 
	{
		return 0;
	}
}

//�����ݿ����һ�������¼
void CPlayer::AddFinishedQuestFromDB(WORD wQuestID)
{
	/*
	tagPlayerQuest PlayerQuest = {wQuestID,byState};
	m_PlayerQuests[PlayerQuest.wQuestID] = PlayerQuest;
	*/
	m_RoleQueue.AddFinishedQuestRecord(wQuestID);

}

BOOL CPlayer::AddQuestFromDB(CQuestRecord* rolerecord)
{
	return m_RoleQueue.AddOneRecord(rolerecord);
}

//���ٸ���������ݵ�ByteArray
bool CPlayer::AddMerQuestDataByteArray(vector<BYTE>* pByteArray)
{
	return m_MerQuestSys.AddDataByteArray(pByteArray);
}

//��ٸ����������
bool CPlayer::DecordMerQuestDataFromByteArray(BYTE* pByte, long& pos)
{
	return m_MerQuestSys.DecordDataFromByteArray(pByte,pos);
}

bool CPlayer::CodeMerQuestDataToDataBlock(DBWriteSet& setWriteDB)
{
	return m_MerQuestSys.CodeToDataBlock(setWriteDB);
}
bool CPlayer::DecodeMerQuestDataFromDataBlock(DBReadSet& setReadDB)
{
	return m_MerQuestSys.DecodeFromDataBlock(setReadDB);
}

// ��������
int CPlayer::ChangeName(const char *szNewName)
{
	if(!szNewName)
		return 1;	// �Ƿ������

	// ��������Ƿ�������� �������ļ���ȡ����������ţ�ֻҪ���и÷��ţ����������
	const char *res = strstr( GetName(), GlobalSetup::GetSetup()->strSpeStr );
	if( !res ) 
		return 2;	// ��Ҳ���ʹ�øù��ܣ���Ϊ���������û���иø����ַ�

	if( !GetGame().CheckInvalidString(string(szNewName)) ) // ��������ֻ��Ч�������Ƿ�Ϸ���
		return 3;	// ��������ֲ��Ϸ���ʧ�ܡ�

	if( GetGame().IsNameExistInMapPlayer(szNewName) )
		return 4;	// �������ʹ���˸�NAME������ʧ��

	//if( GetGame().IsNameExistInDBData(szNewName) )
	//	return 5;	// ������ң����ߺ�ģ�ʹ���˸�NAME������ʧ��

//	if( GetGame().IsNameExistInDBCreation(szNewName) )
//		return 6;	// ������ң��½�����������ʹ���˸�NAME������ʧ��

//	if( GetGame().GetRsPlayer()->IsNameExist( szNewName ) ) 
//		return 7;	// ���ݿ���ڣ�����ʧ��

	SetName(szNewName);

	return 0; // �����ɹ�
}

int CPlayer::ChangeCountry(BYTE c)
{
	// �������е�����
	//if(m_btCountry == c)
	if( this->GetCountry()==c )
	{
		return -1; // �Ƿ��й�
	}

	// ���û��ᣬ�ſ����й�
	if(NULL_GUID != GetFactionID())
	{
		return -3; // ���˳�����
	}
		
	if( GetCountryHandler()->GetCountry(GetCountry())->IsWar() )
	{
		return -4; // ��վ��
	}

	// ���Ĺ��Ƿ����
	if( !GetCountryHandler()->GetCountry(c) )
	{
		
		return -5; // Ŀ�Ĺ��Ƿ�
	}

	SetCountry(c);	// ���Ĺ���ID
	// ���µ�GameServer
	////////////
	return c;
}

//����ID�õ���Ʒ 
CGoods* CPlayer::GetGoodsById(const CGUID& guid)
{	

	CGoods* pGoods = (CGoods*)getOriginPack()->Find(TYPE_GOODS,guid);
	if(pGoods != NULL)
		return pGoods;

	pGoods = (CGoods*)m_cEquipment.Find(TYPE_GOODS,guid);
	if(pGoods != NULL)
		return pGoods;


	pGoods = (CGoods*)m_cWallet.Find(TYPE_GOODS,guid);
	if(pGoods != NULL)
		return pGoods;

	pGoods = (CGoods*)m_cGem.Find(TYPE_GOODS,guid);
	if(pGoods != NULL)
		return pGoods;
	return NULL;

}

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

//void CPlayer::AddTeamRgnGUID(LONG lTemplateRgnID, const CGUID& guid, BOOL bCover)
//{
//	map<LONG, CGUID>::iterator ite = m_mapTeamRgnGUID.find(lTemplateRgnID);
//	if(m_mapTeamRgnGUID.end() != ite && !bCover)
//		return;
//	m_mapTeamRgnGUID[lTemplateRgnID] = guid;
//}
//void CPlayer::DelTeamRgnGUID(LONG lTemplateRgnID)
//{
//	m_mapTeamRgnGUID.erase(lTemplateRgnID);
//}
//void CPlayer::DelTeamRgnGUID(const CGUID& guid)
//{
//	map<LONG, CGUID>::iterator ite = m_mapTeamRgnGUID.begin();
//	for (; ite != m_mapTeamRgnGUID.end(); ++ite)
//	{
//		if(ite->second == guid)
//		{
//			m_mapTeamRgnGUID.erase(ite);
//			return;
//		}
//	}
//}
//const CGUID& CPlayer::GetTeamRgnGUID(LONG lTemplateRgnID)
//{
//	map<LONG, CGUID>::iterator ite = m_mapTeamRgnGUID.find(lTemplateRgnID);
//	if(m_mapTeamRgnGUID.end() != ite)
//		return ite->second;
//	return NULL_GUID;
//}
//VOID CPlayer::RgnGuidCode(vector<BYTE> &vData)
//{
//	_AddToByteArray(&vData, (LONG)m_mapTeamRgnGUID.size());
//	map<LONG, CGUID>::iterator ite = m_mapTeamRgnGUID.begin();
//	for (; ite != m_mapTeamRgnGUID.end(); ++ite)
//	{
//		_AddToByteArray(&vData, ite->first);
//		_AddToByteArray(&vData, ite->second);
//	}
//}
//VOID CPlayer::RgnGuidDecode(BYTE *pData, LONG &lPos)
//{
//	m_mapTeamRgnGUID.clear();
//	LONG lNum = _GetLongFromByteArray(pData, lPos);
//	for (LONG i = 0; i < lNum; ++i)
//	{
//		LONG lTemplateRgnID = _GetLongFromByteArray(pData, lPos);
//		CGUID guid;
//		_GetBufferFromByteArray(pData, lPos, guid);
//		m_mapTeamRgnGUID.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
//	}
//}
//
////!					���鸱��GUID����
//VOID CPlayer::RgnGuidCode(DBWriteSet& setWriteDB)
//{
//	setWriteDB.AddToByteArray((LONG)m_mapTeamRgnGUID.size());
//	map<LONG, CGUID>::iterator ite = m_mapTeamRgnGUID.begin();
//	for (; ite != m_mapTeamRgnGUID.end(); ++ite)
//	{
//		setWriteDB.AddToByteArray(ite->first);
//		setWriteDB.AddToByteArray(ite->second);
//	}
//}
////!					���鸱��GUID����
//VOID CPlayer::RgnGuidDecode(DBReadSet& setReadDB)
//{
//	m_mapTeamRgnGUID.clear();
//	LONG lNum = setReadDB.GetLongFromByteArray();
//	for (LONG i = 0; i < lNum; ++i)
//	{
//		LONG lTemplateRgnID = setReadDB.GetLongFromByteArray();
//		CGUID guid;
//		setReadDB.GetBufferFromByteArray(guid);
//		m_mapTeamRgnGUID.insert(pair<LONG, CGUID>(lTemplateRgnID, guid));
//	}
//}
BOOL CPlayer::UnserializeSubDepot(BYTE* pStream, LONG& lOffset)
{	
	CSubpackContainer *pContainer = (CSubpackContainer*)m_PlayerDepot.FindContainer(eDCT_Secondary);
	assert(NULL != pContainer);
	if(NULL == pContainer) return FALSE;

	BYTE btVal=_GetByteFromByteArray(pStream,lOffset);
	LONG eContainerID=_GetDwordFromByteArray(pStream,lOffset);

	if(btVal)
	{		
		CGoods* pGoods = CGoodsFactory::UnserializeGoods( pStream, lOffset,33 );		
		if( pGoods )
		{
			pContainer->Add( eContainerID - eDCT_Secondary1 + 10001, pGoods, NULL );			
			pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->SetContainerVolume(pGoods);
		}				
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->Unserialize(pStream,lOffset);
	}
	else
	{
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pGoods=NULL;
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->Clear();
	}
	return TRUE;
}

BOOL CPlayer::UnserializeSubDepot(DBReadSet& setReadDB)
{
	CSubpackContainer *pContainer = (CSubpackContainer*)m_PlayerDepot.FindContainer(eDCT_Secondary);
	assert(NULL != pContainer);
	if(NULL == pContainer) return FALSE;

	BYTE btVal=setReadDB.GetByteFromByteArray();
	LONG eContainerID=setReadDB.GetDwordFromByteArray();

	if(btVal)
	{		
		pContainer->Release(eContainerID - eDCT_Secondary1);

		CGoods* pGoods = CGoodsFactory::UnserializeGoods( setReadDB,34);		
		if( pGoods )
		{
			pContainer->Add( eContainerID - eDCT_Secondary1 + 10001, pGoods, NULL );			
			pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->SetContainerVolume(pGoods);
		}				
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->Unserialize(setReadDB);
	}
	else
	{
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pGoods=NULL;
		pContainer->GetSubpack(eContainerID - eDCT_Secondary1)->pSubpackContainer->Clear();
	}
	return TRUE;
}

BOOL CPlayer::SerializeSubDepot(vector<BYTE>* pStream, LONG eContainerID)
{
	CSubpackContainer *pContainer = (CSubpackContainer*)m_PlayerDepot.FindContainer(eContainerID);
	assert(NULL != pContainer);
	if(NULL == pContainer) return FALSE;

	LONG lIndex = eContainerID - eDCT_Secondary1;
	if(pContainer->GetSubpack(lIndex)->pGoods)
	{
		_AddToByteArray(pStream,static_cast<DWORD>(eContainerID));
		pContainer->GetSubpack(lIndex)->pGoods->Serialize(pStream);
		pContainer->GetSubpack(lIndex)->pSubpackContainer->Serialize(pStream);
	}
	else
	{
		_AddToByteArray(pStream,static_cast<DWORD>(0));
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
		CFaction *pFaction = GetOrganiCtrl()->GetFactionOrganizing(m_FactionID);
		assert(NULL != pFaction);//! �������ʧ�ܣ�˵�����������SetFactionID()�ӿڣ�Ӧ��SetFactionInfo()����
		if(NULL != pFaction)
			memmove(m_szFactionName, pFaction->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_FACTION, this);
}

void CPlayer::SetFactionInfo(CFaction *pFaction)
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
		CUnion *pUnion = GetOrganiCtrl()->GetUnionOrganizing(m_UnionID);
		assert(NULL != pUnion);//! �������ʧ�ܣ�˵�����������SetUnionID()�ӿڣ�Ӧ��SetUnionInfo()����
		if(NULL != pUnion)
			memmove(m_szUnionName, pUnion->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_UNION, this);
}

void CPlayer::SetUnionInfo(CUnion *pUnion)
{
	if(NULL == pUnion)
	{
		m_UnionID = NULL_GUID;
		memset(m_szUnionName, 0, MAXSIZE_FactionName);
	}
	else
	{
		m_UnionID = pUnion->GetExID();
		memmove(m_szUnionName, pUnion->GetName(), MAXSIZE_FactionName - 1);
	}

	// Fox@20081113 ˢ�½�ɫ�������
	GetInst(CPlayerMan).RefreshElem(PET_UNION, this);
}

CPet* CPlayer::CreatePet(const CGUID& guid)
{
	CPet* pPet = NULL;
	if( NULL_GUID !=  guid )
	{
		pPet = new CPet;
		pPet->SetExID(guid);
		m_Pets[guid] = pPet;
	}

	return pPet;
}

void CPlayer::AddPet(CPet* pet)
{
	m_Pets[pet->GetExID()] = pet;
}

void CPlayer::DelPet(const CGUID& guid)
{
	ITR_PET itr = m_Pets.find(guid);
	if( itr != m_Pets.end() )
	{
		DelPet(itr);
	}
}

void CPlayer::DelPet(ITR_PET& itr)
{
	DELETE_SAFE(itr->second);
	m_Pets.erase(itr);
}

void CPlayer::ReleaseAllPets(void)
{
	ITR_PET itr = m_Pets.begin();
	while( itr != m_Pets.end() )
	{
		DelPet(itr++);
	}
	m_Pets.clear();
}

void CPlayer::ResetActive(void)
{
	DWORD dwCurrTime = (WORD)time(NULL);
	static tm CurrTm;
	tm *pTm = localtime((time_t*)&dwCurrTime);
	memcpy(&CurrTm, pTm, sizeof(tm));
	pTm = localtime((time_t*)&(m_Property.lLastResetActiveTime));

	if (pTm->tm_year == CurrTm.tm_year && pTm->tm_mon == CurrTm.tm_mon && pTm->tm_mday == CurrTm.tm_mday)
		return;

	m_Property.lCurrBaseActive = (long)PlayerList::GetBaseCP(GetLevel());
//	if(GetGame().GetStarDay() == m_Property.byConst)
//		m_Property.lCurrBaseActive += m_Property.lCurrBaseActive * 10 / 100;
	m_Property.lCurrBaseActive = min(99999, m_Property.lCurrBaseActive);
	m_Property.lLastResetActiveTime = dwCurrTime;
}

void CPlayer::ClearIncShopCur10()
{
	IncShopCur10Iter it=m_mapIncShopCur10.begin();
	for(;it!=m_mapIncShopCur10.end();it++)
	{
		it->second.clear();
	}
}

//(��ֵ�̵�����)��Ӽ�¼
void CPlayer::AddLimitGoodsRecord(DWORD dwGoodsId,DWORD dwGoodsNum)
{
	map<DWORD,DWORD>::iterator recordIt=m_mapLimitGoodsRecord.find(dwGoodsId);
	if(recordIt==m_mapLimitGoodsRecord.end())
		m_mapLimitGoodsRecord[dwGoodsId]=dwGoodsNum;
	else	
		recordIt->second+=dwGoodsNum;	
}

//��ֵ�̵���ӽ��׼�¼
void CPlayer::AddIncTradeRecord( CIncrementShopList::IncShopTradeLog stIncTradeElem )
{
	m_vecTradeList.push_back( stIncTradeElem );
}
