// Copyright (C) 1991 - 1999 Rational Software Corporation

#include "stdafx.h"
#include "CGoods.h"
#include "CGoodsFactory.h"




//##ModelId=41A59BA702EE
map<DWORD,CGoodsBaseProperties*> CGoodsFactory::s_mGoodsBaseProperties;

map<string, DWORD> CGoodsFactory::s_mOriginalNameIndex;

map<long,set<CGoods*> >CGoodsFactory::s_UsingGoods;

map<string, DWORD> CGoodsFactory::s_mNameIndex;
BYTE CGoodsFactory::s_GoodsAttrDBSetup[256][2];
DWORD CGoodsFactory::s_dwAddonPropertyNum;

//##ModelId=41A5A2A0004E
CGoodsFactory::CGoodsFactory()
{
	// TODO: Add your specialized code here.
}

//##ModelId=41A5A2A802EE
CGoodsFactory::~CGoodsFactory()
{
	// TODO: Add your specialized code here.
}

//##ModelId=41A5A2AC00FA
CGoodsBaseProperties* CGoodsFactory::QueryGoodsBaseProperties(DWORD dwID)
{
	// TODO: Add your specialized code here.
	map<DWORD,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.find( dwID );
	if( it != s_mGoodsBaseProperties.end() )
	{
		return it -> second;
	}
	return NULL;
}

//��Ʒ�������Ե����ɼ���
VOID CGoodsFactory::CreateAddonProperties(CGoods* pGoods)
{
	if(pGoods == NULL)	return;
	DWORD dwID = pGoods->GetBasePropertiesIndex();
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		//##��ȡ������Ч�ĸ�������
		vector<GOODS_ADDON_PROPERTIES> vOut;
		pProperties -> GetValidAddonProperties( vOut );
		//##�ж���Щ���������Ƿ���Ч(���ϳ��ֵĸ���).			
		for( size_t i = 0; i < vOut.size(); i ++ )
		{	

			//##׼���ø����ԵĽṹ.
			CGoods::tagAddonProperty apGoodsAddonProperty;
			apGoodsAddonProperty.gapType				= vOut[i];	
			apGoodsAddonProperty.lValues[0]=0;
			apGoodsAddonProperty.lValues[1]=0;

			//##��ȡ�����Ե�ֵ.
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties -> GetAddonPropertyValues( vOut[i], vValues );
			if(vValues.size()==2)
			{
				apGoodsAddonProperty.lValues[0]=vValues[0].lBaseValue;
				apGoodsAddonProperty.lValues[1]=vValues[1].lBaseValue;
			}
			//##���ø���������ӵ���Ʒ�ṹ��.
			pGoods -> m_vAddonProperties.push_back( apGoodsAddonProperty );
		}	
	}
}

//##ModelId=41A5A3BE0399
CGoodsBaseProperties* CGoodsFactory::QueryGoodsBasePropertiesByOriginalName(const CHAR* pName)
{
	// TODO: Add your specialized code here.
	return QueryGoodsBaseProperties( QueryGoodsIDByOriginalName(pName) );
}

//##ModelId=41A5A3D800FA
CGoodsBaseProperties* CGoodsFactory::QueryGoodsBasePropertiesByName(const CHAR* pName)
{
	// TODO: Add your specialized code here.
	return QueryGoodsBaseProperties( QueryGoodsIDByName(pName) );
}

//##ModelId=41A5A319004E
const CHAR* CGoodsFactory::QueryGoodsOriginalName(DWORD dwID)
{
	// TODO: Add your specialized code here.
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		return pProperties -> GetOriginalName();
	}
	return NULL;
}

//##ModelId=41A5A3400128
const CHAR* CGoodsFactory::QueryGoodsName(DWORD dwID)
{
	// TODO: Add your specialized code here.
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		return pProperties -> GetName();
	}
	return NULL;
}

//##ModelId=41A5A356008C
DWORD CGoodsFactory::QueryGoodsIDByOriginalName(const CHAR* pName)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	if( pName == NULL ) return 0;

	string strKey;
	strKey = pName;
	map<string, DWORD>::iterator it = s_mOriginalNameIndex.find( pName );
	if( it != s_mOriginalNameIndex.end() )
	{
		return it -> second;
	}
	return 0;
}

//##ModelId=41A5A37D0119
DWORD CGoodsFactory::QueryGoodsIDByName(const CHAR* pName)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	if( pName == NULL ) return 0;

	string strKey;
	strKey = pName;
	map<string, DWORD>::iterator it = s_mNameIndex.find( pName );
	if( it != s_mNameIndex.end() )
	{
		return it -> second;
	}
	return 0;
}

//##ModelId=41A5A45B0280
BOOL CGoodsFactory::Serialize(vector<BYTE>* pStream, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	_AddToByteArray( pStream, static_cast<DWORD>( s_mGoodsBaseProperties.size() ) );
	map<DWORD,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.begin();
	for( ; it != s_mGoodsBaseProperties.end(); it ++ )
	{
		_AddToByteArray( pStream, it -> first );
		it -> second -> Serialize( pStream );
	}

	_AddToByteArray(pStream,s_dwAddonPropertyNum);
	for(DWORD i=1;i<=s_dwAddonPropertyNum;i++)
	{
		_AddToByteArray(pStream,s_GoodsAttrDBSetup[i][0]);
		_AddToByteArray(pStream,s_GoodsAttrDBSetup[i][1]);
	}
	return TRUE;
}

//##ModelId=41A5A473033C
BOOL CGoodsFactory::Unserialize(BYTE* pStream, LONG& lOffset, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	Release();

	DWORD dwNumGoods = _GetDwordFromByteArray( pStream, lOffset );
	for( DWORD i = 0; i < dwNumGoods; i ++ )
	{
		CGoodsBaseProperties* pGoodsBaseProperties = OBJ_CREATE(CGoodsBaseProperties);
		if( pGoodsBaseProperties == NULL )
		{
			OutputDebugString( "Cann't alloc memory for goods base properties decode!\n" );
			return FALSE;
		}

		DWORD dwIndex = _GetDwordFromByteArray( pStream, lOffset );
		pGoodsBaseProperties -> Unserialize( pStream, lOffset );

		s_mGoodsBaseProperties[dwIndex]									= pGoodsBaseProperties;
		s_mOriginalNameIndex[pGoodsBaseProperties -> m_strOriginalName] = dwIndex;
		s_mNameIndex[pGoodsBaseProperties -> m_strName]					= dwIndex;
	}

	for(DWORD i=0;i<256;i++)
	{
		s_GoodsAttrDBSetup[i][0]=0;
		s_GoodsAttrDBSetup[i][1]=0;
	}

	s_dwAddonPropertyNum=_GetDwordFromByteArray(pStream,lOffset);
	for(DWORD i=1;i<=s_dwAddonPropertyNum;i++)
	{
		s_GoodsAttrDBSetup[i][0]=_GetByteFromByteArray(pStream,lOffset);
		s_GoodsAttrDBSetup[i][1]=_GetByteFromByteArray(pStream,lOffset);
	}

	return TRUE;
}


//���л���DataBlock
BOOL CGoodsFactory::Serialize(DBWriteSet& setWriteDB, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	setWriteDB.AddToByteArray( static_cast<DWORD>( s_mGoodsBaseProperties.size() ) );
	map<DWORD,CGoodsBaseProperties*>::iterator it = s_mGoodsBaseProperties.begin();
	for( ; it != s_mGoodsBaseProperties.end(); it ++ )
	{
		setWriteDB.AddToByteArray( it -> first );
		it -> second -> Serialize( setWriteDB );
	}

	setWriteDB.AddToByteArray(s_dwAddonPropertyNum);
	for(DWORD i=1;i<=s_dwAddonPropertyNum;i++)
	{
		setWriteDB.AddToByteArray(s_GoodsAttrDBSetup[i][0]);
		setWriteDB.AddToByteArray(s_GoodsAttrDBSetup[i][1]);
	}
	return TRUE;
}

//ͨ��DataBlock��ԭ
BOOL CGoodsFactory::Unserialize(DBReadSet& setReadDB, BOOL b)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	Release();
	DWORD dwNumGoods = setReadDB.GetDwordFromByteArray(  );
	for( DWORD i = 0; i < dwNumGoods; i ++ )
	{
		CGoodsBaseProperties* pGoodsBaseProperties = OBJ_CREATE(CGoodsBaseProperties);
		if( pGoodsBaseProperties == NULL )
		{
			OutputDebugString( "Cann't alloc memory for goods base properties decode!\n" );
			return FALSE;
		}

		DWORD dwIndex = setReadDB.GetDwordFromByteArray(  );
		pGoodsBaseProperties -> Unserialize( setReadDB );

		s_mGoodsBaseProperties[dwIndex]									= pGoodsBaseProperties;
		s_mOriginalNameIndex[pGoodsBaseProperties -> m_strOriginalName] = dwIndex;
		s_mNameIndex[pGoodsBaseProperties -> m_strName]					= dwIndex;
	}

	for(DWORD i=0;i<256;i++)
	{
		s_GoodsAttrDBSetup[i][0]=0;
		s_GoodsAttrDBSetup[i][1]=0;
	}

	s_dwAddonPropertyNum=setReadDB.GetDwordFromByteArray();
	for(DWORD i=1;i<=s_dwAddonPropertyNum;i++)
	{
		s_GoodsAttrDBSetup[i][0]=setReadDB.GetByteFromByteArray( );
		s_GoodsAttrDBSetup[i][1]=setReadDB.GetByteFromByteArray( );
	}

	return TRUE;
}

void CGoodsFactory::AddCreateGoodsLog(CGoods* pGoods,long lType)
{
	if(pGoods)
	{
		s_UsingGoods[lType].insert(pGoods);
	}
}

//##ModelId=41A5A7660251
CGoods* CGoodsFactory::CreateGoods( DWORD dwID,long lType )
{
	// TODO: Add your specialized code here.
	CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( dwID );
	CGoods* pGoods = NULL;
	if( pProperties )
	{
		//##��������Ʒ,�������úó�ʼ������.
		pGoods = MP_NEW CGoods;
		pGoods -> SetBasePropertiesIndex( dwID );
		pGoods -> SetName( pProperties -> GetName() );
		pGoods -> SetGoodsName( pProperties -> GetName() );
		
		pGoods -> SetPrice( pProperties -> GetPrice() );
		pGoods->SetSilverPrice(pProperties->GetSilverPrice());
		pGoods -> SetGraphicsID( pProperties -> GetIconID(CGoodsBaseProperties::IT_GROUND) );

		/*
		//##������
		CGUID	guid;
		CGUID::CreateGUID(guid);
		pGoods->SetExID(guid);*/

		//##��ȡ������Ч�ĸ�������
		vector<GOODS_ADDON_PROPERTIES> vOut;
		pProperties -> GetValidAddonProperties( vOut );

		//##�ж���Щ���������Ƿ���Ч(���ϳ��ֵĸ���).
		for( size_t i = 0; i < vOut.size(); i ++ )
		{			
			//DWORD dwProbability = static_cast<DWORD>( random(10000) );
			//if( dwProbability >= pProperties -> GetOccurProbability(vOut[i]) ) continue;

			//##׼���ø����ԵĽṹ.
			CGoods::tagAddonProperty apGoodsAddonProperty;
			apGoodsAddonProperty.gapType				= vOut[i];
			apGoodsAddonProperty.lValues[0]=0;
			apGoodsAddonProperty.lValues[1]=0;				

			//##��ȡ�����Ե�ֵ.
			vector<CGoodsBaseProperties::tagAddonPropertyValue> vValues;
			pProperties -> GetAddonPropertyValues( vOut[i], vValues );
			if(vValues.size()==2)
			{
				apGoodsAddonProperty.lValues[0]=vValues[0].lBaseValue;
				apGoodsAddonProperty.lValues[1]=vValues[1].lBaseValue;
			}
			//##���ø���������ӵ���Ʒ�ṹ��.
			pGoods -> m_vAddonProperties.push_back( apGoodsAddonProperty );
		}
		CGUID	guid;
		CGUID::CreateGUID(guid);
		pGoods->SetExID(guid);
		s_UsingGoods[lType].insert(pGoods);
	}
	return pGoods;
}



//##ModelId=41A688EC0109
VOID CGoodsFactory::Release()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	map<DWORD,CGoodsBaseProperties*>::iterator it = 
		s_mGoodsBaseProperties.begin();

	for( ; it != s_mGoodsBaseProperties.end(); it ++ )
	{
		OBJ_RELEASE(CGoodsBaseProperties, it -> second );
	}

	s_mGoodsBaseProperties.clear();
	s_mOriginalNameIndex.clear();
	s_mNameIndex.clear();
}


//##ModelId=41B55322019C
VOID CGoodsFactory::CreateGoods(DWORD dwID, DWORD dwAmount, vector<CGoods*>& vOut,long lType)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	CGoodsBaseProperties* pProperties = QueryGoodsBaseProperties( dwID );
	if( pProperties )
	{
		if( pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE )
		{
			CGoods* pGoods = CreateGoods( dwID,lType );
			if( pGoods )
			{
				pGoods -> SetAmount( dwAmount );
				vOut.push_back( pGoods );
			}
		}
		else
		{
			for( DWORD i = 0; i < dwAmount; i ++ )
			{
				CGoods* pGoods = CreateGoods( dwID,lType );
				if( pGoods )
				{
					vOut.push_back( pGoods);
				}
			}
		}
	}
}

//ȡ�á���Ǯ���ı��
DWORD CGoodsFactory::GetGoldCoinIndex()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return CGlobeSetup::GetSetup()->dwGoldGoodsId;	
}

//ȡ�á���ʯ���ı��
DWORD CGoodsFactory::GetGemIndex()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.	
	return QueryGoodsIDByName("��ʯ");
}

const char* CGoodsFactory::GetGoldOrginName()
{
	DWORD dwGoldId=GetGoldCoinIndex();
	return QueryGoodsOriginalName(dwGoldId);
}

const char* CGoodsFactory::GetGoldName()
{
	DWORD dwGoldId=GetGoldCoinIndex();
	return QueryGoodsName(dwGoldId);
}

DWORD CGoodsFactory::GetSilverIndex()
{
	return CGlobeSetup::GetSetup()->dwSilverGoodsId;
}

const char* CGoodsFactory::GetSilverOrginName()
{
	DWORD dwId=GetSilverIndex();
	return QueryGoodsOriginalName(dwId);
}

const char* CGoodsFactory::GetSilverName()
{
	DWORD dwId=GetSilverIndex();
	return QueryGoodsName(dwId);
}


//ȡ�á�Ԫ�����ı��
DWORD CGoodsFactory::GetYuanBaoIndex()
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.	
	return QueryGoodsIDByName("Ԫ��");
}

struct tagFix
{
	BYTE	id;
	LONG	lMin;
	LONG	lMax;
	WORD	wOdds;
};

//##ModelId=41AD288C003E
BOOL CGoodsFactory::Load(const CHAR* strPath)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	Release();

	CRFile *rfile = rfOpen(strPath);
	if (rfile == NULL)
	{
		OutputDebugString( "Can't open goods config file.\n" );
		return FALSE;
	}

	CHAR szHeader[6];
	ZeroMemory( szHeader, 6 );
	//�ļ�ͷУ��
	rfile->ReadData(szHeader, 5);
	if( strcmp( szHeader, "GOODS" ) != 0 )
	{
		OutputDebugString( "Not a correct goods config file.\n" );
		return FALSE;
	}
	//�汾��
	LONG lVersion = 0;
	rfile->ReadData( &lVersion, sizeof(LONG) );
	//��Ʒ����
	LONG lNumGoodsProperties = 0;
	rfile->ReadData( &lNumGoodsProperties, sizeof(LONG) );
	for( LONG i = 0; i < lNumGoodsProperties; i ++ )
	{
		DWORD dwIndex = 0;
		//���
		rfile->ReadData( &dwIndex, sizeof(DWORD) );

		CGoodsBaseProperties* pProperties = OBJ_CREATE(CGoodsBaseProperties);
		if( pProperties == NULL )
		{
			OutputDebugString( "Load goods config file failed.\n" );
			Release();
			return FALSE;
		}

		CHAR* pString	= NULL;
		LONG lLength	= 0;
		//ԭʼ��
		rfile->ReadData( &lLength, sizeof(LONG) );
		pString = (CHAR*)M_ALLOC(lLength + 1);
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties -> m_strOriginalName = pString;
		M_FREE(pString, lLength + 1);
	    
		//����
		rfile->ReadData( &lLength, sizeof(LONG) );
		pString = (CHAR*)M_ALLOC(lLength + 1);
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties -> m_strName = pString;
		M_FREE(pString, lLength + 1);

		//��������
		bool bTemp;
		//�Ƿ��DB
		rfile->ReadData( &bTemp, sizeof(bool) );
		//�۸�
		rfile->ReadData( &(pProperties -> m_dwPrice), sizeof(DWORD) );
		rfile->ReadData(&(pProperties -> m_dwSilverPrice), sizeof(DWORD));
		

		//��Ʒ������Ҫ��һ�α任��
		DWORD dwType = 0;
		rfile->ReadData( &dwType, sizeof(DWORD) );
		
		pProperties->SetGoodsBaseType(static_cast<eGoodsBaseType>(dwType));
		
		switch( dwType )
		{
		case 0://��ͨ
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_USELESS;
			break;
		case 1://������
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_CONSUMABLE;
			break;
		case 2://��Ƭ			
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_CARD;
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_CARD;
			break;
		case 3:			
			break;
		case 4://ͷ��
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_HEAD;
			break;
		case 5://�·�
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_BODY;
			break;
		case 6://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_GLOVE;
			break;
		case 7://Ь��
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_BOOT;
			break;
		case 8:
			//����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_WEAPON;
			break;
		case 9://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_WEAPON2;
			break;
		case 10://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_NECKLACE;
			break;			
		case 11://��ָ
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_LRING;
			break;
		case 12://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_BACK;
			break;
		case 13://���
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_WING;
			break;
		case 14://ͷ��
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_HEADGEAR;
			break;
		case 15://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_FROCK;
			break;
		case 16://����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_FAIRY;
			break;
		case 17://ѫ��
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties -> m_epEquipPlace	= CGoodsBaseProperties::EP_MEDAL1;
			break;
		case 18: //����
			pProperties -> m_gtGoodsType	= CGoodsBaseProperties::GT_PACK;			
			break;
		case 19://��ʸ
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_WEAPON2;
			break;
		case 21://����
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_HORSE;
			break;
		case 24://����
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_CLOAK;
			break;
		case 25://��Ʒ
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_DECORATION1;
			break;
		case 26://����
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_ADORN;
			break;
		case 27://�����ػ�ʯ
			pProperties->m_gtGoodsType=CGoodsBaseProperties::GT_EQUIPMENT;
			pProperties->m_epEquipPlace=CGoodsBaseProperties::EP_GUARDIAN;
			break;

		}
		

		DWORD dwIcon;
		CGoodsBaseProperties::tagIcon iIcon;

		rfile->ReadData( &dwIcon, sizeof(DWORD) );
		iIcon.itIconType	= CGoodsBaseProperties::IT_CONTAINER;
		iIcon.dwIconID		= dwIcon;
		pProperties -> m_vIcons.push_back( iIcon );

		rfile->ReadData( &dwIcon, sizeof(DWORD) );
		iIcon.itIconType	= CGoodsBaseProperties::IT_GROUND;
		iIcon.dwIconID		= dwIcon;
		pProperties -> m_vIcons.push_back( iIcon );

		rfile->ReadData( &dwIcon, sizeof(DWORD) );
		iIcon.itIconType	= CGoodsBaseProperties::IT_EQUIPED;
		iIcon.dwIconID		= dwIcon;
		pProperties -> m_vIcons.push_back( iIcon );

		DWORD dwTemp;
		//ʰȡ����
		rfile->ReadData( &dwTemp, sizeof(DWORD) );
		//�Ӷ�����
		rfile->ReadData( &dwTemp, sizeof(DWORD) );
		//��Ч����
		rfile->ReadData( &dwTemp, sizeof(DWORD) );
		//�Ƿ����
		rfile->ReadData( &bTemp, sizeof(bool) );
		//��Ӧ�������
		rfile->ReadData( &dwTemp, sizeof(DWORD) );

		//����
		rfile->ReadData( &lLength, sizeof(LONG) );
		pString = (CHAR*)M_ALLOC(lLength + 1);
		ZeroMemory( pString, lLength + 1 );
		rfile->ReadData( pString, lLength );
		pProperties -> m_strDescribe = pString;
		M_FREE(pString, lLength + 1);

		LONG lNumAddonProperties = 0;
		rfile->ReadData( &lNumAddonProperties, sizeof(LONG) );

		for( LONG j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagAddonProperty apProperty;

			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );
			apProperty.gapType		= static_cast<GOODS_ADDON_PROPERTIES>( wType+1 );

			//�Ƿ���Ч
			rfile->ReadData( &bTemp, sizeof(bool) );
			if( bTemp )
			{
				apProperty.bIsEnabled = TRUE;
			}
			//�Ƿ�����ʽ����
			rfile->ReadData( &bTemp, sizeof(bool) );
			if( bTemp )
			{
				apProperty.bIsImplicitAttribute = TRUE;
			}
			CGoodsBaseProperties::tagAddonPropertyValue apvValue;

			//ֵ1
			apvValue.dwId = 1;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(LONG) );
			apProperty.vValues.push_back( apvValue );
			
			//ֵ2
			apvValue.dwId = 2;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(LONG) );
			apProperty.vValues.push_back( apvValue );
					
			pProperties -> m_vAddonProperties.push_back( apProperty );
		}

		lNumAddonProperties = 0;
		rfile->ReadData( &lNumAddonProperties, sizeof(LONG) );		
		for( LONG j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagSuitProperty suitProperty;

			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );
			
			
			suitProperty.gapType		= static_cast<GOODS_ADDON_PROPERTIES>( wType+1 );
			
			CGoodsBaseProperties::tagAddonPropertyValue apvValue;

			//ֵ1
			apvValue.dwId = 1;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(LONG) );
			suitProperty.vValues.push_back( apvValue );
			
			//ֵ2
			apvValue.dwId = 2;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(LONG) );
			suitProperty.vValues.push_back( apvValue );

			//��װ����
			apvValue.dwId = 3;
			rfile->ReadData( &apvValue.lBaseValue, sizeof(WORD) );
			suitProperty.vValues.push_back( apvValue );	
					
			pProperties -> m_vSuitProperties.push_back( suitProperty );
		}

		lNumAddonProperties=0;
		rfile->ReadData(&lNumAddonProperties,sizeof(LONG));
		for( LONG j = 0; j < lNumAddonProperties; j ++ )
		{
			CGoodsBaseProperties::tagTriggerProperty stTriggerProperty;
			//�������Ե�����
			WORD wType = 0;
			rfile->ReadData( &wType, sizeof(WORD) );
			stTriggerProperty.gapType		= static_cast<GOODS_ADDON_PROPERTIES>( wType+1 );
			rfile->ReadData(&stTriggerProperty.lVal1,sizeof(LONG));
			rfile->ReadData(&stTriggerProperty.lVal2,sizeof(LONG));
			rfile->ReadData(&stTriggerProperty.wTriggerLevel,sizeof(WORD));
			pProperties->m_vTriggerProperties.push_back(stTriggerProperty);
		}

		s_mGoodsBaseProperties[dwIndex]							= pProperties;
		s_mOriginalNameIndex[pProperties -> m_strOriginalName]	= dwIndex;
		s_mNameIndex[pProperties -> m_strName]					= dwIndex;
	}

	rfClose( rfile );
	return TRUE;
}

BOOL CGoodsFactory::LoadAttrDBSetup(const char* strpath)
{
	for(int i=0;i<256;i++)
	{
		s_GoodsAttrDBSetup[i][0]=0;
		s_GoodsAttrDBSetup[i][1]=0;
	}	

	CRFile* prfile = rfOpen(strpath);
	if(prfile == NULL)
	{
		char str[256];
		sprintf(str, "file '%s' can't found!", strpath);
		MessageBox(NULL, str, "error", MB_OK);
		return FALSE;
	}

	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);
	s_dwAddonPropertyNum=0;
	stream>>s_dwAddonPropertyNum;
	ReadTo(stream,"#");
	string strVal="";	
	for(int i=1;i<=s_dwAddonPropertyNum;i++)
	{
		stream>>strVal>>strVal>>strVal;
		s_GoodsAttrDBSetup[i][0]=(BYTE)atoi(strVal.c_str());
		stream>>strVal;
		s_GoodsAttrDBSetup[i][1]=(BYTE)atoi(strVal.c_str());
	}		
	return TRUE;
}

//##ModelId=41C632EA0387
BOOL CGoodsFactory::GarbageCollect(CGoods** ppGoods)
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	BOOL bResult = FALSE;
	if( ppGoods )
	{
		map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
		for(;it != s_UsingGoods.end();it++)
		{
			(*it).second.erase(*ppGoods);
		}

		SAFE_DELETE( *ppGoods );
		bResult = TRUE;
	}
	return bResult;
}


//##ModelId=41C632940182
CGoods* CGoodsFactory::UnserializeGoods(BYTE* pStream, LONG& lOffset,long lType)
{
	// TODO: Add your specialized code here.
	CGoods* pResult = NULL;
	if( pStream )
	{
		pResult = MP_NEW CGoods;
		if( pResult == NULL )
		{
			assert( 0 );
			OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
		}
		else 
		{
			pResult -> Unserialize( pStream, lOffset );

			CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( 
				pResult -> GetBasePropertiesIndex() );
			if( pProperties )
			{
				if( pProperties -> GetGoodsType() == CGoodsBaseProperties::GT_CONSUMABLE )
				{
					//pResult -> SetID( 0 );
				}
				s_UsingGoods[lType].insert(pResult);
			}
			else
			{
				GarbageCollect( &pResult );
			}
		}
	}
	return pResult;
}


//ͨ��ByteArray��ԭ��Ʒ
CGoods* CGoodsFactory::UnserializeGoods(DBReadSet& setReadDB,long lType)
{
	// TODO: Add your specialized code here.
	CGoods* pResult = NULL;	
	pResult = MP_NEW CGoods;
	if( pResult == NULL )
	{
		assert( 0 );
		OutputDebugString( "Can't alloc memory for unserialize goods.\n" );
	}
	else 
	{
		pResult -> Unserialize( setReadDB );
		
		CGoodsBaseProperties* pProperties = CGoodsFactory::QueryGoodsBaseProperties( 
			pResult -> GetBasePropertiesIndex() );
		if(! pProperties )
		{
			GarbageCollect( &pResult );
		}
		else
		{
			s_UsingGoods[lType].insert(pResult);
		}
	}
	return pResult;
}


BOOL CGoodsFactory::Upgrade(CGoods* pGoods, GOODS_ADDON_PROPERTIES gapSource,
							GOODS_ADDON_PROPERTIES gapDest, BOOL bUpgrade)
{
	return TRUE;
}

map<CGUID, vector<BYTE>>	CGoodsFactory::m_mapChatGoodsInfo;
list<pair<CGUID, LONG>>		CGoodsFactory::m_listSaveTimeOrder;

//! ���һ����Ʒ��Ϣ
void CGoodsFactory::AddGoodsInfo(const CGUID &Key, const vector<BYTE> &vGoodsInfo)
{
	assert(m_mapChatGoodsInfo.end() == m_mapChatGoodsInfo.find(Key));
	if (CGlobeSetup::GetSetup()->lMaxGoodInfoNum <= m_mapChatGoodsInfo.size())
	{
		ClearInfo(CGlobeSetup::GetSetup()->lMaxGoodInfoNum / 10);
	}
	
	m_mapChatGoodsInfo[Key] = vGoodsInfo;
	m_listSaveTimeOrder.push_back(pair<CGUID, LONG>(Key, time(NULL)));
}

//! ȡ��һ����Ʒ��Ϣ
BOOL CGoodsFactory::GetGoodsInfo(const CGUID &Key, vector<BYTE> &vGoodsInfo)
{
	map<CGUID, vector<BYTE>>::iterator ite = m_mapChatGoodsInfo.find(Key);
	if (m_mapChatGoodsInfo.end() != ite)
	{
		vGoodsInfo.resize(ite->second.size());
		memmove(&vGoodsInfo[0], &(ite->second[0]), ite->second.size());
		return TRUE;
	}
	return FALSE;
}

//! ����
void CGoodsFactory::ClearInfo(LONG lClearNum)
{
	if (0 >= lClearNum)
		lClearNum = 1;

	list<pair<CGUID, LONG>>::iterator ite = m_listSaveTimeOrder.begin();
	LONG i = 0;
	for ( ; ite != m_listSaveTimeOrder.end(); ++ite)
	{
		if(i < lClearNum)
			break;
		++i;
	}

	m_listSaveTimeOrder.erase(m_listSaveTimeOrder.begin(), ite);
}

long CGoodsFactory::OutputUseGoodsInfo(const char *pszFileName)
{
	char str[1024];
	PutStringToFile(pszFileName,"UseGoodsInfo:");
	map<long,set<CGoods*> >::iterator it = s_UsingGoods.begin();
	for(;it != s_UsingGoods.end();it++)
	{
		if((*it).second.size() > 0)
		{
			sprintf(str,"Goods Flag:%d,Count:%d",(*it).first,(*it).second.size() );
			PutStringToFile(pszFileName,str);
		}
	}
	return 0;
}
