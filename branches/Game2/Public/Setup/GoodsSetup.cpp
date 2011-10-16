#include "StdAfx.h"
#include "GoodsSetup.h"
#include "GlobalSetup.h"
#include "../Common/Public.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/// ��̬��Ա��ʼ��
map<ulong,CGoodsBaseProperties*> GoodsSetup::s_mGoodsBaseProperties;

map<string, ulong> GoodsSetup::s_mOriginalNameIndex;

map<string, ulong> GoodsSetup::s_mNameIndex;

uchar GoodsSetup::s_GoodsAttrDBSetup[GAP_TATAL_NUM][2];

ulong GoodsSetup::s_dwAddonPropertyNum;

map<string,ulong> GoodsSetup::s_mAddonNameMap;


/// ����
GoodsSetup::GoodsSetup()
{
	
}

/// ����
GoodsSetup::~GoodsSetup()
{
	Release();
}

/// ȡ����ƷID����Ʒ��������ӳ�伯��
map<ulong,CGoodsBaseProperties*>* GoodsSetup::GetGoodsBasePropertiesList()
{
	return &s_mGoodsBaseProperties;
}

/// ������Ʒ����
bool GoodsSetup::Load(const char* strPath)
{
	Release();

	CRFile *rfile = rfOpen(strPath);
	if (rfile == NULL)
	{
		OutputDebugString( "Can't open goods config file.\n" );
		return false;
	}

	char szHeader[6];
	ZeroMemory( szHeader, 6 );
	//�ļ�ͷУ��
	rfile->ReadData(szHeader, 5);
	if( strcmp( szHeader, "GOODS" ) != 0 )
	{
		OutputDebugString( "Not a correct goods config file.\n" );
		return false;
	}
	//�汾��
	long lVersion = 0;
	rfile->ReadData( &lVersion, sizeof(long) );
	//��Ʒ����
	long lNumGoodsProperties = 0;
	rfile->ReadData( &lNumGoodsProperties, sizeof(long) );
	for( long i = 0; i < lNumGoodsProperties; i ++ )
	{		
		CGoodsBaseProperties* pProperties = new CGoodsBaseProperties;
		if( pProperties == NULL )
		{
			OutputDebugString( "Load goods config file failed.\n" );
			Release();
			return false;
		}

		ulong dwIndex = 0;
		//���
		rfile->ReadData( &dwIndex, sizeof(ulong) );
		pProperties->m_index = dwIndex;

		char* pString	= NULL;
		long lLength	= 0;
		//ԭʼ��
		rfile->ReadData( &lLength, sizeof(long) );
		pString = new char[lLength + 1];
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties->m_strOriginalName = pString;
		SAFE_DELETE( pString );

		//����
		rfile->ReadData( &lLength, sizeof(long) );
		pString = new char[lLength + 1];
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties->m_strName = pString;
		SAFE_DELETE( pString );

		//��������
		bool bTemp;
		//�Ƿ��DB
		rfile->ReadData( &bTemp, sizeof(bool) );
		//�۸�
		rfile->ReadData( &(pProperties->m_dwPrice), sizeof(ulong) );
		rfile->ReadData(&(pProperties->m_dwSilverPrice), sizeof(ulong));

		//��Ʒ������Ҫ��һ�α任��
		ulong dwType = 0;
		rfile->ReadData( &dwType, sizeof(ulong) );

		pProperties->SetGoodsBaseType(static_cast<eGoodsBaseType>(dwType));

		ulong dwIcon;
		CGoodsBaseProperties::tagIcon iIcon;

		rfile->ReadData( &dwIcon, sizeof(ulong) );
		iIcon.itIconType	= IT_CONTAINER;
		iIcon.dwIconID		= dwIcon;
		pProperties->m_vIcons.push_back( iIcon );

		rfile->ReadData( &dwIcon, sizeof(ulong) );
		iIcon.itIconType	= IT_GROUND;
		iIcon.dwIconID		= dwIcon;
		pProperties->m_vIcons.push_back( iIcon );

		rfile->ReadData( &dwIcon, sizeof(ulong) );
		iIcon.itIconType	= IT_EQUIPED;
		iIcon.dwIconID		= dwIcon;
		pProperties->m_vIcons.push_back( iIcon );

		ulong dwTemp;
		//ʰȡ����
		rfile->ReadData( &dwTemp, sizeof(ulong) );
		//�Ӷ�����
		rfile->ReadData( &dwTemp, sizeof(ulong) );
		//��Ч����
		rfile->ReadData( &dwTemp, sizeof(ulong) );
		//�Ƿ����
		rfile->ReadData( &bTemp, sizeof(bool) );
		//��Ӧ�������
		rfile->ReadData( &dwTemp, sizeof(ulong) );

		//����
		rfile->ReadData( &lLength, sizeof(long) );
		pString = new char[lLength + 1];
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties->m_strDescribe = pString;
		SAFE_DELETE( pString );

		long lNumAddonProperties = 0;
		rfile->ReadData( &lNumAddonProperties, sizeof(long) );

		for( long j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagAddonProperty apProperty;

			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );
			apProperty.gapType		= static_cast<eGoodsAddonProperties>( wType+1 );

			//�Ƿ���Ч
			rfile->ReadData( &bTemp, sizeof(bool) );
			if( bTemp )
			{
				apProperty.bIsEnabled = true;
			}
			//�Ƿ�����ʽ����
			rfile->ReadData( &bTemp, sizeof(bool) );
			if( bTemp )
			{
				apProperty.bIsImplicitAttribute = true;
			}
			CGoodsBaseProperties::tagAddonPropertyValue apvValue;

			//ֵ1
			apvValue.dwId = 1;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(long) );
			apProperty.vValues.push_back( apvValue );

			//ֵ2
			apvValue.dwId = 2;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(long) );
			apProperty.vValues.push_back( apvValue );

			pProperties->m_vAddonProperties.push_back( apProperty );
		}

		lNumAddonProperties = 0;
		rfile->ReadData( &lNumAddonProperties, sizeof(long) );		
		for( long j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagSuitProperty suitProperty;

			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );


			suitProperty.gapType		= static_cast<eGoodsAddonProperties>( wType+1 );

			CGoodsBaseProperties::tagAddonPropertyValue apvValue;

			//ֵ1
			apvValue.dwId = 1;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(long) );
			suitProperty.vValues.push_back( apvValue );

			//ֵ2
			apvValue.dwId = 2;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(long) );
			suitProperty.vValues.push_back( apvValue );

			//��װ����
			apvValue.dwId = 3;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(WORD) );
			suitProperty.vValues.push_back( apvValue );	

			pProperties->m_vSuitProperties.push_back( suitProperty );
		}

		lNumAddonProperties=0;
		rfile->ReadData(&lNumAddonProperties,sizeof(long));
		for( long j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagTriggerProperty stTriggerProperty;
			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );
			stTriggerProperty.gapType		= static_cast<eGoodsAddonProperties>( wType+1 );
			rfile->ReadData(&stTriggerProperty.lVal1,sizeof(long));
			rfile->ReadData(&stTriggerProperty.lVal2,sizeof(long));
			rfile->ReadData(&stTriggerProperty.wTriggerLevel,sizeof(WORD));
			pProperties->m_vTriggerProperties.push_back(stTriggerProperty);
		}

		s_mGoodsBaseProperties[dwIndex]							= pProperties;
		s_mOriginalNameIndex[pProperties->m_strOriginalName]	= dwIndex;
		s_mNameIndex[pProperties->m_strName]					= dwIndex;
	}

	rfClose( rfile );
	return true;
}

/// ������Ʒ����DB�洢����
bool GoodsSetup::LoadAttrDBSetup(const char* strpath)
{
	for(int i=0; i<GAP_TATAL_NUM; i++)
	{
		s_GoodsAttrDBSetup[i][0] = 0;
		s_GoodsAttrDBSetup[i][1] = 0;
	}	

	CRFile* prfile = rfOpen(strpath);
	if(prfile == NULL)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("WS_SYS_31", strpath));
		return false;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);
	s_dwAddonPropertyNum = 0;
	stream >> s_dwAddonPropertyNum;
	ReadTo(stream, "#");
	string strVal = "";	
	for(ulong i=1; i<=s_dwAddonPropertyNum; i++)
	{
		stream >> strVal >> strVal >> strVal;
		s_GoodsAttrDBSetup[i][0] = (uchar)atoi(strVal.c_str());
		stream >> strVal;
		s_GoodsAttrDBSetup[i][1] = (uchar)atoi(strVal.c_str());
	}		
	return true;
}

/// ����
bool GoodsSetup::Serialize(vector<uchar>* pStream, bool b)
{
	_AddToByteArray(pStream, static_cast<ulong>(s_mGoodsBaseProperties.size()));
	map<ulong,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.begin();
	for(; it != s_mGoodsBaseProperties.end(); ++it)
	{
		_AddToByteArray(pStream, it->first);
		it->second->Serialize(pStream);
	}

	_AddToByteArray(pStream, s_dwAddonPropertyNum);
	for(ulong i=1; i<=s_dwAddonPropertyNum; i++)
	{
		_AddToByteArray(pStream, s_GoodsAttrDBSetup[i][0]);
		_AddToByteArray(pStream, s_GoodsAttrDBSetup[i][1]);
	}
	return true;
}

/// ����
bool GoodsSetup::Unserialize(uchar* pStream, long& lOffset, bool b)
{
	Release();

	ulong dwNumGoods = _GetDwordFromByteArray(pStream, lOffset);
	for(ulong i = 0; i < dwNumGoods; i++)
	{
		CGoodsBaseProperties* pGoodsBaseProperties = new CGoodsBaseProperties();
		if( pGoodsBaseProperties == NULL )
		{
			OutputDebugString( "Cann't alloc memory for goods base properties decode!\n" );
			return false;
		}

		ulong dwIndex = _GetDwordFromByteArray(pStream, lOffset);
		pGoodsBaseProperties->Unserialize(pStream, lOffset);

		s_mGoodsBaseProperties[dwIndex]	= pGoodsBaseProperties;
		s_mOriginalNameIndex[pGoodsBaseProperties->m_strOriginalName] = dwIndex;
		s_mNameIndex[pGoodsBaseProperties->m_strName] = dwIndex;
	}

	for(ulong i = 0; i < GAP_TATAL_NUM; i++)
	{
		s_GoodsAttrDBSetup[i][0] = 0;
		s_GoodsAttrDBSetup[i][1] = 0;
	}

	s_dwAddonPropertyNum = _GetDwordFromByteArray(pStream, lOffset);
	for( ulong i = 1; i <= s_dwAddonPropertyNum; i++ )
	{
		s_GoodsAttrDBSetup[i][0] = _GetByteFromByteArray(pStream, lOffset);
		s_GoodsAttrDBSetup[i][1] = _GetByteFromByteArray(pStream, lOffset);
	}

	return true;
}

/// ���л���DataBlock
bool GoodsSetup::Serialize(DBWriteSet& setWriteDB, bool b)
{
	setWriteDB.AddToByteArray(static_cast<ulong>(s_mGoodsBaseProperties.size()));
	map<ulong,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.begin();
	for(; it != s_mGoodsBaseProperties.end(); ++it)
	{
		setWriteDB.AddToByteArray(it->first);
		it->second->Serialize(setWriteDB);
	}

	setWriteDB.AddToByteArray(s_dwAddonPropertyNum);
	for(ulong i = 1; i <= s_dwAddonPropertyNum; i++)
	{
		setWriteDB.AddToByteArray(s_GoodsAttrDBSetup[i][0]);
		setWriteDB.AddToByteArray(s_GoodsAttrDBSetup[i][1]);
	}
	return true;
}

/// ͨ��DataBlock��ԭ
bool GoodsSetup::Unserialize(DBReadSet& setReadDB, bool b)
{
	Release();
	ulong dwNumGoods = setReadDB.GetDwordFromByteArray();
	for(ulong i = 0; i < dwNumGoods; i++)
	{
		CGoodsBaseProperties* pGoodsBaseProperties = new CGoodsBaseProperties();
		if( pGoodsBaseProperties == NULL )
		{
			OutputDebugString( "Cann't alloc memory for goods base properties decode!\n" );
			return false;
		}

		ulong dwIndex = setReadDB.GetDwordFromByteArray();
		pGoodsBaseProperties->Unserialize(setReadDB);

		s_mGoodsBaseProperties[dwIndex]	= pGoodsBaseProperties;
		s_mOriginalNameIndex[pGoodsBaseProperties->m_strOriginalName] = dwIndex;
		s_mNameIndex[pGoodsBaseProperties->m_strName] = dwIndex;
	}

	for(ulong i = 0; i < GAP_TATAL_NUM; i++)
	{
		s_GoodsAttrDBSetup[i][0] = 0;
		s_GoodsAttrDBSetup[i][1] = 0;
	}

	s_dwAddonPropertyNum = setReadDB.GetDwordFromByteArray();
	for(ulong i = 1; i <= s_dwAddonPropertyNum; i++)
	{
		s_GoodsAttrDBSetup[i][0] = setReadDB.GetByteFromByteArray();
		s_GoodsAttrDBSetup[i][1] = setReadDB.GetByteFromByteArray();
	}

	return true;
}

/// ���
void GoodsSetup::Release()
{
	map<ulong,CGoodsBaseProperties*>::iterator it = 
		s_mGoodsBaseProperties.begin();

	for(; it != s_mGoodsBaseProperties.end(); ++it)
	{
		SAFE_DELETE(it->second);
	}

	s_mGoodsBaseProperties.clear();
	s_mOriginalNameIndex.clear();
	s_mNameIndex.clear();
	s_mAddonNameMap.clear();
}

/// ����ID��ȡ��Ʒ��������
CGoodsBaseProperties* GoodsSetup::QueryGoodsBaseProperties(ulong dwID)
{
	map<ulong,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.find(dwID);
	if(it != s_mGoodsBaseProperties.end())
	{
		return it->second;
	}
	return NULL;
}

/// ����ԭʼ����ȡ��Ʒ��������
CGoodsBaseProperties* GoodsSetup::QueryGoodsBasePropertiesByOriginalName(const char* pName)
{
	return QueryGoodsBaseProperties( QueryGoodsIDByOriginalName(pName) );
}

/// �������ƻ�ȡ��Ʒ��������
CGoodsBaseProperties* GoodsSetup::QueryGoodsBasePropertiesByName(const char* pName)
{
	return QueryGoodsBaseProperties( QueryGoodsIDByName(pName) );
}

/// ����ID��ȡ��Ʒԭʼ��
const char* GoodsSetup::QueryGoodsOriginalName(ulong dwID)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties(dwID);
	if( pProperties )
	{
		return pProperties->GetOriginalName();
	}
	return NULL;
}

/// ����ID��ȡ��Ʒ���� 
const char* GoodsSetup::QueryGoodsName(ulong dwID)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties(dwID);
	if( pProperties )
	{
		return pProperties->GetName();
	}
	return NULL;
}

/// ������Ʒԭʼ����ȡ��Ʒ����
const char* GoodsSetup::QueryGoodsName(const char* pOriginName)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBasePropertiesByOriginalName(pOriginName);
	if( pProperties )
	{
		return pProperties->GetName();
	}
	return NULL;
}

/// ������Ʒԭʼ����ȡ��ƷID
ulong GoodsSetup::QueryGoodsIDByOriginalName(const char* pName)
{
	if( pName == NULL ) 
		return 0;

	string strKey;
	strKey = pName;
	map<string, ulong>::iterator it = s_mOriginalNameIndex.find(pName);
	if( it != s_mOriginalNameIndex.end() )
	{
		return it->second;
	}
	return 0;
}

/// ������Ʒ����ȡ��ƷID 
ulong GoodsSetup::QueryGoodsIDByName(const char* pName)
{
	if( pName == NULL )
		return 0;

	string strKey;
	strKey = pName;
	map<string, ulong>::iterator it = s_mNameIndex.find(pName);
	if( it != s_mNameIndex.end() )
	{
		return it->second;
	}
	return 0;
}

/// ȡ��"��Ǯ"�ı��
ulong GoodsSetup::GetGoldCoinIndex()
{
	return GlobalSetup::GetSetup()->dwGoldGoodsId;	
}

/// ȡ��"��ʯ"�ı��
ulong GoodsSetup::GetGemIndex()
{
	return QueryGoodsIDByName("��ʯ");
}

/// ȡ��"��Ǯ"����Ʒԭʼ��
const char* GoodsSetup::GetGoldOrginName()
{
	ulong dwGoldId = GetGoldCoinIndex();
	return QueryGoodsOriginalName(dwGoldId);
}

/// ȡ��"��Ǯ"����Ʒ��
const char* GoodsSetup::GetGoldName()
{
	ulong dwGoldId = GetGoldCoinIndex();
	return QueryGoodsName(dwGoldId);
}

/// ȡ��"����"�ı��
ulong GoodsSetup::GetSilverIndex()
{
	return GlobalSetup::GetSetup()->dwSilverGoodsId;
}

/// ȡ��"����"����Ʒԭʼ��
const char* GoodsSetup::GetSilverOrginName()
{
	ulong dwId = GetSilverIndex();
	return QueryGoodsOriginalName(dwId);
}

/// ȡ��"����"����Ʒ��
const char* GoodsSetup::GetSilverName()
{
	ulong dwId = GetSilverIndex();
	return QueryGoodsName(dwId);
}


/// ȡ��"Ԫ��"�ı��
ulong GoodsSetup::GetYuanBaoIndex()
{
	// ������������GlobalSetup���������
	return QueryGoodsIDByName("Ԫ��");
}

/// ����ID��ȡ��Ʒ��������
eGoodsBaseType GoodsSetup::QueryGoodsType(ulong dwID)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties(dwID);
	if( pProperties )
	{
		return pProperties->GetGoodsBaseType();
	}
	return GBT_USELESS;
}

/// ����ID��ȡ��Ʒ�۸�
ulong GoodsSetup::QueryGoodsPrice(ulong dwID)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties(dwID);
	if( pProperties )
	{
		return pProperties->GetPrice();
	}

	return 0;
}

/// ����ID��ȡ��Ʒ����
const char* GoodsSetup::QueryGoodsDescribe(ulong dwID)
{
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties(dwID);
	if( pProperties )
	{
		return pProperties->GetDescribe();
	}

	return NULL;
}

//��ȡ��Ʒ��������
ulong GoodsSetup::QueryGoodsMaxStackNumber(ulong dwID)
{
	ulong dwResult = 1;
	CGoodsBaseProperties* pProperties = GoodsSetup::QueryGoodsBaseProperties(dwID);

	if(dwID==GoodsSetup::GetGoldCoinIndex())
	{			
		return GlobalSetup::GetSetup()->dwGoldCoinLimit;
	}
	else if(dwID==GoodsSetup::GetSilverIndex())
	{
		return GlobalSetup::GetSetup()->dwSilverLimit;
	}
	if(pProperties && (pProperties->GetGoodsBaseType() == GBT_CONSUMABLE ||
		pProperties->GetGoodsBaseType() == GBT_NORMAL ||
		pProperties->GetGoodsBaseType() == GBT_USELESS))
	{
		vector<CGoodsBaseProperties::tagAddonPropertyValue> vOut;
		pProperties->GetAddonPropertyValues(GAP_MAXOVERLAP, vOut);
		for(size_t i = 0; i < vOut.size(); i++)
		{
			if(vOut[i].dwId == 1)
			{
				dwResult = static_cast<ulong>(vOut[i].lBaseValue);
				break;
			}
		}
		vOut.clear();
	}
	return dwResult;
}

/// ��ʼ���������Ա��
VOID GoodsSetup::InitAddonNameMap()
{
	s_mAddonNameMap.clear();	
	s_mAddonNameMap["GAP_GOODS_CATEGORY"] = GAP_GOODS_CATEGORY;
	s_mAddonNameMap["GAP_COLOR"] = GAP_COLOR;
	s_mAddonNameMap["GAP_COSPLAYID"] = GAP_COSPLAYID;
	s_mAddonNameMap["GAP_EQUIP_EFFECT1"] = GAP_EQUIP_EFFECT1;
	s_mAddonNameMap["GAP_EQUIP_EFFECT2"] = GAP_EQUIP_EFFECT2;
	s_mAddonNameMap["GAP_EQUIP_EFFECT3"] = GAP_EQUIP_EFFECT3;
	s_mAddonNameMap["GAP_WEAPON_CATEGORY"] = GAP_WEAPON_CATEGORY;
	s_mAddonNameMap["GAP_ROLE_MINIMUM_LEVEL_LIMIT"] = GAP_ROLE_LEVEL_LIMIT;
	s_mAddonNameMap["GAP_SKILL_LIMIT"] = GAP_SKILL_LIMIT;
	s_mAddonNameMap["GAP_REQUIRE_OCCUPATION"] = GAP_REQUIRE_OCCUPATION;
	s_mAddonNameMap["GAP_REQUIRE_GENDER"] = GAP_REQUIRE_GENDER;
	s_mAddonNameMap["GAP_PARTICULAR_ATTRIBUTE"] = GAP_PARTICULAR_ATTRIBUTE;
	s_mAddonNameMap["GAP_WEAPON_DAMAGE_LEVEL"] = GAP_WEAPON_DAMAGE_LEVEL;
	s_mAddonNameMap["GAP_MIN_ATTACK"] = GAP_MIN_ATTACK;
	s_mAddonNameMap["GAP_MAX_ATTACK"] = GAP_MAX_ATTACK;
	s_mAddonNameMap["GAP_NOR_DEF"] = GAP_NOR_DEF;
	s_mAddonNameMap["GAP_HURT_SUCK_PER"] = GAP_HURT_SUCK_PER;
	s_mAddonNameMap["GAP_HIT_RATE_CORRECT"] = GAP_HIT_RATE_CORRECT;
	s_mAddonNameMap["GAP_NORATTACK_KILLRATE"] = GAP_NORATTACK_KILLRATE;
	s_mAddonNameMap["GAP_FLEE_CORRECT"] = GAP_FLEE_CORRECT;
	s_mAddonNameMap["GAP_PARRY_CORRECT"] = GAP_PARRY_CORRECT;
	s_mAddonNameMap["GAP_BLOCK_CORRECT"] = GAP_BLOCK_CORRECT;
	s_mAddonNameMap["GAP_FUMO_PROPERTY"] = GAP_FUMO_PROPERTY;
	s_mAddonNameMap["GAP_MAGIC_KILLRATE"] = GAP_MAGIC_KILLRATE;
	s_mAddonNameMap["GAP_DEDICANT_CORRECT"] = GAP_DEDICANT_CORRECT;
	s_mAddonNameMap["GAP_MAGIC_SPEED"] = GAP_MAGIC_SPEED;
	s_mAddonNameMap["GAP_MAGIC_DEF"] = GAP_MAGIC_DEF;
	s_mAddonNameMap["GAP_FIREDEF_CORRECT"] = GAP_FIREDEF_CORRECT;
	s_mAddonNameMap["GAP_WATERDEF_CORRECT"] = GAP_WATERDEF_CORRECT;
	s_mAddonNameMap["GAP_WINDDEF_CORRECT"] = GAP_WINDDEF_CORRECT;
	s_mAddonNameMap["GAP_EARTHDEF_CORRECT"] = GAP_EARTHDEF_CORRECT;
	s_mAddonNameMap["GAP_HOLYDEF_CORRECT"] = GAP_HOLYDEF_CORRECT;
	s_mAddonNameMap["GAP_DARKDEF_CORRECT"] = GAP_DARKDEF_CORRECT;
	s_mAddonNameMap["GAP_WILL_DEF"] = GAP_WILL_DEF;
	s_mAddonNameMap["GAP_HYPNOSIS_DEF"] = GAP_HYPNOSIS_DEF;
	s_mAddonNameMap["GAP_POISON_DEF"] = GAP_POISON_DEF;
	s_mAddonNameMap["GAP_FIX_DEF"] = GAP_FIX_DEF;
	s_mAddonNameMap["GAP_CON_DEF"] = GAP_CON_DEF;
	s_mAddonNameMap["GAP_BLED_DEF"] = GAP_BLED_DEF;
	s_mAddonNameMap["GAP_DIZZY_DEF"] = GAP_DIZZY_DEF;
	s_mAddonNameMap["GAP_LULL_DEF"] = GAP_LULL_DEF;
	s_mAddonNameMap["GAP_ADD_STR"] = GAP_ADD_STR;
	s_mAddonNameMap["GAP_ADD_DEX"] = GAP_ADD_DEX;
	s_mAddonNameMap["GAP_ADD_CON"] = GAP_ADD_CON;
	s_mAddonNameMap["GAP_ADD_INT"] = GAP_ADD_INT;
	s_mAddonNameMap["GAP_ADD_WIS"] = GAP_ADD_WIS;
	s_mAddonNameMap["GAP_ADD_SPI"] = GAP_ADD_SPI;
	s_mAddonNameMap["GAP_MAXHP_CORRECT"] = GAP_MAXHP_CORRECT;
	s_mAddonNameMap["GAP_HP_RESUME_CORRECT"] = GAP_HP_RESUME_CORRECT;
	s_mAddonNameMap["GAP_MAXMP_CORRECT"] = GAP_MAXMP_CORRECT;
	s_mAddonNameMap["GAP_MP_RESUME_CORRECT"] = GAP_MP_RESUME_CORRECT;
	s_mAddonNameMap["GAP_ADD_HP"] = GAP_ADD_HP;
	s_mAddonNameMap["GAP_ADD_PERCENT_HP"] = GAP_ADD_PERCENT_HP;
	s_mAddonNameMap["GAP_ADD_MP"] = GAP_ADD_MP;
	s_mAddonNameMap["GAP_ADD_PERCENT_MP"] = GAP_ADD_PERCENT_MP;
	s_mAddonNameMap["GAP_GOODS_MAX_DURABILITY"] = GAP_GOODS_MAX_DURABILITY;
	s_mAddonNameMap["GAP_MAXOVERLAP"] = GAP_MAXOVERLAP;
	s_mAddonNameMap["GAP_LEARN_SKILL"] = GAP_LEARN_SKILL;
	s_mAddonNameMap["GAP_ADD_SKILL"] = GAP_ADD_SKILL;
	s_mAddonNameMap["GAP_LEECH_HP"] = GAP_LEECH_HP;
	s_mAddonNameMap["GAP_LEECH_MP"] = GAP_LEECH_MP;
	s_mAddonNameMap["GAP_HURT2MP"] = GAP_HURT2MP;
	s_mAddonNameMap["GAP_KNOCK"] = GAP_KNOCK;
	s_mAddonNameMap["GAP_HOME"] = GAP_HOME;
	s_mAddonNameMap["GAP_RAN_TRANS"] = GAP_RAN_TRANS;
	s_mAddonNameMap["GAP_SCRIPT		"] = GAP_SCRIPT		;
	s_mAddonNameMap["GAP_GEM_TYPE"] = GAP_GEM_TYPE;
	s_mAddonNameMap["GAP_GEM_LEVEL"] = GAP_GEM_LEVEL;
	s_mAddonNameMap["GAP_GEM_PROBABILITY"] = GAP_GEM_PROBABILITY;
	s_mAddonNameMap["GAP_GEM_UPGRADE_SUCCEED_RESULT"] = GAP_GEM_UPGRADE_SUCCEED_RESULT;
	s_mAddonNameMap["GAP_GEM_UPGRADE_FAILED_RESULT"] = GAP_GEM_UPGRADE_FAILED_RESULT;
	s_mAddonNameMap["GAP_MOUNT_TYPE	"] = GAP_MOUNT_TYPE	;
	s_mAddonNameMap["GAP_MOUNT_LEVEL"] = GAP_MOUNT_LEVEL;
	s_mAddonNameMap["GAP_MOUNT_PLAYER_ROLE_LIMIT"] = GAP_MOUNT_ROLE_LIMIT;
	s_mAddonNameMap["GAP_UNLIMITED_ACCESS"] = GAP_UNLIMITED_ACCESS;
	s_mAddonNameMap["GAP_EXCEPTION_STATE"] = GAP_EXCEPTION_STATE;
	s_mAddonNameMap["GAP_EXP_BONUS"] = GAP_EXP_BONUS;
	s_mAddonNameMap["GAP_EXPBALL_USERID"] = GAP_EXPBALL_USERID;
	s_mAddonNameMap["GAP_EXPBALL_EXP"] = GAP_EXPBALL_EXP;
	s_mAddonNameMap["GAP_YUANBAO_QUITS"] = GAP_YUANBAO_QUITS;
	s_mAddonNameMap["GAP_RECORD_REGIONID"] = GAP_RECORD_REGIONID;
	s_mAddonNameMap["GAP_RECORD_COOR"] = GAP_RECORD_COOR;
	s_mAddonNameMap["GAP_EQUIP_MAXEXP"] = GAP_EQUIP_MAXEXP;
	s_mAddonNameMap["GAP_SUITID"] = GAP_SUITID;
	s_mAddonNameMap["GAP_SUIT_NUMBER"] = GAP_SUIT_NUMBER;
	s_mAddonNameMap["GAP_CREATOR_NAME"] = GAP_CREATOR_NAME;
	s_mAddonNameMap["GAP_DEL_TIME"] = GAP_DEL_TIME;
	s_mAddonNameMap["GAP_LIFE"] = GAP_LIFE;
	s_mAddonNameMap["GAP_GET_TRIGGER"] = GAP_GET_TRIGGER;
	s_mAddonNameMap["GAP_LOST_TRIGGER"] = GAP_LOST_TRIGGER;
	s_mAddonNameMap["GAP_HOLE_NUM"] = GAP_HOLE_NUM;
	s_mAddonNameMap["GAP_ENCHASE_TARGET"] = GAP_ENCHASE_TARGET;
	s_mAddonNameMap["GAP_PACK_TYPE"] = GAP_PACK_TYPE;
	s_mAddonNameMap["GAP_PACK_CAPACITY"] = GAP_PACK_CAPACITY;
	s_mAddonNameMap["GAP_MIN_ATK_UP"] = GAP_MIN_ATK_UP;	
	s_mAddonNameMap["GAP_MAX_ATK_UP"] = GAP_MAX_ATK_UP;	
	s_mAddonNameMap["GAP_MAGIC_UP"] = GAP_MAGIC_UP;
	s_mAddonNameMap["GAP_DEF_UP"] = GAP_DEF_UP;	
	s_mAddonNameMap["GAP_MAGICDEF_UP"] = GAP_MAGICDEF_UP;	
	s_mAddonNameMap["GAP_HIT_RATE_UP"] = GAP_HIT_RATE_UP;
	s_mAddonNameMap["GAP_KILL_RATE_UP"] = GAP_KILL_RATE_UP;
	s_mAddonNameMap["GAP_PARRY_UP"] = GAP_PARRY_UP;
	s_mAddonNameMap["GAP_BLOCK_UP"] = GAP_BLOCK_UP;
	s_mAddonNameMap["GAP_FLEE_UP"] = GAP_FLEE_UP;
	s_mAddonNameMap["GAP_MAXDUR_UP"] = GAP_MAXDUR_UP;
	s_mAddonNameMap["GAP_WEAPON_LEVEL"] =GAP_WEAPON_LEVEL;
	s_mAddonNameMap["GAP_MINATK_MODIFY_PER"] = GAP_MINATK_MODIFY_PER;
	s_mAddonNameMap["GAP_MAXATK_MODIFY_PER"] = GAP_MAXATK_MODIFY_PER;
	s_mAddonNameMap["GAP_DEF_MODIFY_PER"] = GAP_DEF_MODIFY_PER;
	s_mAddonNameMap["GAP_MAGICATK_MODIFY_PER"] = GAP_MAGICATK_MODIFY_PER;
	s_mAddonNameMap["GAP_MAGICDEF_MODIFY_PER"] = GAP_MAGICDEF_MODIFY_PER;
	s_mAddonNameMap["GAP_GUARD_EQUIP_CATEGORY"] = GAP_GUARD_EQUIP_CATEGORY;
	s_mAddonNameMap["GAP_EQUIP_TRIGGER"] = GAP_EQUIP_TRIGGER;
	s_mAddonNameMap["GAP_UNEQUIP_TRIGGER"] = GAP_UNEQUIP_TRIGGER;
	s_mAddonNameMap["GAP_ADD_STATE	"] = GAP_ADD_STATE	;
	s_mAddonNameMap["GAP_COOLDOWN"] = GAP_COOLDOWN;
	s_mAddonNameMap["GAP_BOTHHAND_GRAPHIC"] = GAP_BOTHHAND_GRAPHIC;
	s_mAddonNameMap["GAP_CARD_CATEGORY"] = GAP_CARD_CATEGORY;
	s_mAddonNameMap["GAP_CARD_ENCHASE_NUM_LIMIT"] = GAP_CARD_ENCHASE_NUM_LIMIT;
}

/// ȡ����Ʒ�������Ա��
ulong GoodsSetup::GetAddonByName(string str)
{
	map<string,ulong>::iterator iter = s_mAddonNameMap.find(str);
	if(iter != s_mAddonNameMap.end())
	{
		return iter->second;
	}
	return 0;
}
