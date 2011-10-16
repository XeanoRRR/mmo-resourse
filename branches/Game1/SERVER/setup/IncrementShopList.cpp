// ��ֵ�̵ֻ꣬��������ֵ���ߣ���������������������������˰�ա�
// bugs
#include "StdAfx.h"
#include "IncrementShoplist.h"

#ifdef _GAME_SERVER
#include "..\GameServer\AppServer\goods\cgoodsfactory.h"
#endif

#ifdef _WORLD_SERVER_
#include "..\WorldServer\AppWorld\goods\cgoodsfactory.h"
#endif

#ifdef _MONSTER_SERVER
#include "..\MonsterServer\AppMonster\goods\cgoodsfactory.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HWND g_hWnd;

CIncrementShopList::MAP_TOP10 CIncrementShopList::m_mTop10List;
CIncrementShopList::MAP_NEWGOODS CIncrementShopList::m_mNewGoodsList;
CIncrementShopList::MAP_GUIDEGOODS CIncrementShopList::m_mGuideGoodsList;
CIncrementShopList::MAP_FILTERGOODS CIncrementShopList::m_mFilterListOfWShop;
CIncrementShopList::MAP_FILTERGOODS CIncrementShopList::m_mFilterListOfXShop;
CIncrementShopList::MAP_FILTERGOODS CIncrementShopList::m_mFilterListOfTShop;
CIncrementShopList::MAP_INCGOODS CIncrementShopList::m_mIncGoodsList;
CIncrementShopList::MAP_INCCLASSGOODS CIncrementShopList::m_mIncClassGoodsList;
CIncrementShopList::MAP_INCSHOP CIncrementShopList::m_mMapIncGoodsList;
CIncrementShopList::MAP_INCMAP CIncrementShopList::m_mMapList;
CIncrementShopList::tagReturnParam CIncrementShopList::m_tgReturnParm;

//string CIncrementShopList::m_sContactGM;
string CIncrementShopList::m_sAffiche;
//bool CIncrementShopList::m_bReload;




CIncrementShopList::CIncrementShopList(void)
{
}

CIncrementShopList::~CIncrementShopList(void)
{
	ClearIncShop();
	//m_sContactGM.clear();
}

// ���ļ���ȡ�б�
bool CIncrementShopList::LoadIncrementShopList(const char* filename)
{
	if( !filename ) return false;

	ClearIncShop();

	char szError[128] = "";
	TiXmlNode *pNode = NULL;
	TiXmlDocument xmldoc;
	// ���������Ϣ��������ļ���
	CRFile* prFile = rfOpen(filename);
	if (prFile == NULL)
	{
		return FALSE;
	}
	if( !xmldoc.LoadData(prFile->GetData(),prFile->GetDatalen()) )
	{
		_snprintf(szError, 128, "���������ļ�%sʧ��,����", filename);
		MessageBox(NULL,szError,"",MB_OK);
		return false;
	}
	rfClose(prFile);
	// �ҵ����ڵ㣬����޽ڵ㣬���������ʾ
	if ( !( pNode = xmldoc.FirstChild("IncShopList")) )
	{
		_snprintf(szError, 128, "û����IncShopList�ڵ�,����");
		MessageBox(NULL,szError,"",MB_OK); 
		return false;
	}

	// �õ����ֶ�
	string	strAffiche;						//������Ϣ
	string	strOrignalName;					//��Ʒԭʼ��
	string	strTOrignalName;				//��ȯԭʼ��
	DWORD	dwGoodsType;					//��Ʒ���ͣ��ӻ�&ҩƷ&����...��
	DWORD	dwShopType;						//�̵����ͣ�0 �����̵� 1  λ�澫���̵� 2 ��ȯ�̵�
	DWORD	dwPrice;						//�۸�����۸�&λ��۸� &��ȯ�۸�
	DWORD	dwDiscount;						//������Ϣ
	string	strTimeLimit;					//��ʱ��Ϣ
	long	lQuantityLimit;					//������Ϣ
	long	lBuyLimit;						//�޹���Ϣ
	string	strFilterType;					//ɸѡ����
	DWORD	dwGoodsIndex;					//��Ʒ���ID
	DWORD	dwGoodsNum;						//��Ʒ����

	double	dVal;

	// ����ת��
	TiXmlElement* pElem = pNode->ToElement();
	TiXmlElement *pChildElem=pElem->FirstChildElement();
	for ( ; pChildElem; pChildElem=pChildElem->NextSiblingElement() )
	{
		if( !strcmp(pChildElem->Value(), "ShopType" )
			&& TIXML_NO_ATTRIBUTE!=pChildElem->QueryDoubleAttribute("TypeID", &dVal) )
		{
			dwShopType = dVal;
			MAP_INCSHOP mapIncGoodsList;
			MAP_CLASSGOODS mapIncClassGoodsList;
			TiXmlElement* pGrandElem = pChildElem->FirstChildElement();
			for ( DWORD dwNo=0; pGrandElem; pGrandElem=pGrandElem->NextSiblingElement(), ++dwNo )
			{
				//������Ʒ�Ƽ��б�
				if ( !strcmp(pGrandElem->Value(), "NewGoods" ) )
				{
					VEC_GOODSIDX vecNewGoodIndex;
					ParseGoodsList( pGrandElem, vecNewGoodIndex );
					m_mNewGoodsList[dwShopType] = vecNewGoodIndex;
				}
				//����������Ʒ�б�
				else if( !strcmp(pGrandElem->Value(), "Top10Goods" ) )
				{
					VEC_GOODSIDX vecTop10GoodIndex;
					ParseGoodsList( pGrandElem, vecTop10GoodIndex );
					m_mTop10List[dwShopType] = vecTop10GoodIndex;
				}
				//����������ǩ
				else if ( !strcmp(pGrandElem->Value(), "ShopGuideTags" ) )
				{
					TiXmlElement* pShopGuideElem = pGrandElem->FirstChildElement();
					MAP_TYPEGOODS mapGuideGoods;
					for ( ; pShopGuideElem; pShopGuideElem=pShopGuideElem->NextSiblingElement() )
					{
						if( !strcmp(pShopGuideElem->Value(), "GuideTag" ) )
						{
							string strGuideType = pShopGuideElem->Attribute( "TypeID" );
							VEC_GOODSIDX vecGuideGoodIndex;
							ParseGoodsList( pShopGuideElem, vecGuideGoodIndex );
							mapGuideGoods[strGuideType] = vecGuideGoodIndex;
						}
					}
					m_mGuideGoodsList[dwShopType] = mapGuideGoods;
				}
				//������Ʒ�����ǩ
				else if ( !strcmp(pGrandElem->Value(), "GoodsType" ) 
					&& TIXML_NO_ATTRIBUTE!=pGrandElem->QueryDoubleAttribute("TypeID", &dVal) )
				{
					dwGoodsType = dVal;
					VEC_GOODSIDX vecGoodsIdx;
					MAP_TYPEGOODS mapFilterTypeList;
					TiXmlElement* pGoodTypeElem = pGrandElem->FirstChildElement();
					for ( ; pGoodTypeElem; pGoodTypeElem=pGoodTypeElem->NextSiblingElement() )
					{
						if ( !strcmp( pGoodTypeElem->Value(), "FilterTypeTag" ) )
						{
							string strFilterType = pGoodTypeElem->Attribute( "TypeID" );
							VEC_GOODSIDX vecFilterGoodIndex;
							ParseGoodsList( pGoodTypeElem, vecFilterGoodIndex );
							mapFilterTypeList[strFilterType] = vecFilterGoodIndex;
						}
						else if ( !strcmp( pGoodTypeElem->Value(), "Goods" ) )
						{
							//��Ʒԭʼ��
							strOrignalName	= pGoodTypeElem->Attribute("OrignalName");
							dwGoodsIndex	= CGoodsFactory::QueryGoodsIDByOriginalName(strOrignalName.c_str());
							if( !dwGoodsIndex )
							{
								char str[256];
								_snprintf(str, 256, "��ֵ�̵� GoodsType ���ô���ԭʼ��Ϊ %s ����Ʒ������(����&λ��)��",strOrignalName.c_str());
								MessageBox( NULL, str, "���ô���", MB_OK );
								continue;
							}

							//�۸�<�Բ�ͬ���̵����Ϊ��λ>
							pGoodTypeElem->Attribute("Price", &dVal);
							dwPrice	= dVal;

							//��Ʒ����
							pGoodTypeElem->Attribute("GoodsNum",&dVal);
							if (dVal==0)
							{
								dwGoodsNum = 1;
							}
							else
							{
								dwGoodsNum = dVal;
							}
							//������Ϣ
							pGoodTypeElem->Attribute( "discount", &dVal );
							dwDiscount = dVal;
							//��ʱ����
							strTimeLimit = pGoodTypeElem->Attribute( "timelimit" );
							//��������
							int ival;
							pGoodTypeElem->Attribute( "quantitylimit", &ival );
							lQuantityLimit = ival;
							//�޹�����
							pGoodTypeElem->Attribute( "buylimit", &ival );
							lBuyLimit = ival;

							IncrementShopElem	elemIncShop( dwGoodsType,dwGoodsIndex, dwPrice,dwGoodsNum,dwDiscount, 
								strTimeLimit,lQuantityLimit,lBuyLimit);

							mapIncGoodsList[dwGoodsIndex] = elemIncShop;
							vecGoodsIdx.push_back( dwGoodsIndex );
						}
					}
					if ( dwShopType == TM_YUANBAO )
					{
						m_mFilterListOfXShop[dwGoodsType] = mapFilterTypeList;
					}
					else if ( dwShopType == TM_WEIMIAN )
					{
						m_mFilterListOfWShop[dwGoodsType] = mapFilterTypeList;
					}
					else if ( dwShopType == TM_TICKET )
					{
						m_mFilterListOfTShop[dwGoodsType] = mapFilterTypeList;
					}
					mapIncClassGoodsList[dwGoodsType] = vecGoodsIdx;
				}
			}
			m_mIncGoodsList[dwShopType] = mapIncGoodsList;
			m_mIncClassGoodsList[dwShopType] = mapIncClassGoodsList;
		}
		//����������Ϣ
		else if( !strcmp(pChildElem->Value(), "Affiche" ) )
		{
			TiXmlElement* pGrandElem = pChildElem->FirstChildElement();
			for ( ; pGrandElem; pGrandElem=pGrandElem->NextSiblingElement() )
			{
				if( !strcmp(pGrandElem->Value(), "line" ) )
				{
					strAffiche	= pGrandElem->Attribute("String");
					m_sAffiche += strAffiche + '\n';
				}
			}
		}
		//������ص����ò���
		else if (!strcmp(pChildElem->Value(), "ReturnParam" ))
		{
			TiXmlElement* pReturnElem = pChildElem->FirstChildElement();
			m_tgReturnParm.lSGoodsName.clear();
			for ( ; pReturnElem; pReturnElem=pReturnElem->NextSiblingElement() )
			{
				if( !strcmp(pReturnElem->Value(), "Normal" ) )
				{
					double dbValue = 0.0;
					pReturnElem->QueryDoubleAttribute("Value",&dbValue);	
					m_tgReturnParm.fNoramlRatio	= dbValue;				
				}
				else if ( !strcmp(pReturnElem->Value(), "Special" ) )
				{
					double dbValue = 0.0;
					pReturnElem->QueryDoubleAttribute("Value",&dbValue);	
					m_tgReturnParm.fSpecialRatio = dbValue;			
					m_tgReturnParm.strStartTime = pReturnElem->Attribute("StartTime");	
					m_tgReturnParm.strEndTime =  pReturnElem->Attribute("EndTime");	
				}
				else if( !strcmp(pReturnElem->Value(), "Goods" ) )
				{
					string strGoodsName = pReturnElem->Attribute("Name");
					m_tgReturnParm.lSGoodsName.push_back(strGoodsName);
				}

			}
		}

		//��������ӳ������
		else if (!strcmp(pChildElem->Value(), "GoodMap"))
		{
			TiXmlElement* pGrandElem = pChildElem->FirstChildElement();
			for ( ; pGrandElem; pGrandElem=pGrandElem->NextSiblingElement())
			{
				if( !strcmp(pGrandElem->Value(), "GoodsType") &&
					TIXML_NO_ATTRIBUTE!=pGrandElem->QueryDoubleAttribute("TypeID", &dVal))
				{
					dwGoodsType = dVal;
					TiXmlElement* pGoodTypeElem = pGrandElem->FirstChildElement();
					for ( ; pGoodTypeElem; pGoodTypeElem=pGoodTypeElem->NextSiblingElement() )
					{
						if ( !strcmp( pGoodTypeElem->Value(), "Goods" ) )
						{
							//��Ʒԭʼ��
							strOrignalName = pGoodTypeElem->Attribute("OrignalName");
							dwGoodsIndex = CGoodsFactory::QueryGoodsIDByOriginalName(strOrignalName.c_str());

							//ӳ��ԭʼ��
							std::string strMapOrignalName = pGoodTypeElem->Attribute("MapName");
							DWORD dwMapGoodsIndex = CGoodsFactory::QueryGoodsIDByOriginalName(strMapOrignalName.c_str());

							if( !dwGoodsIndex || !dwMapGoodsIndex )
							{
								//���ô���
								continue;
							}

							//�۸�
							pGoodTypeElem->Attribute("Price", &dVal);
							dwPrice	= dVal;

							//��Ʒ����
							pGoodTypeElem->Attribute("GoodsNum",&dVal);
							if (dVal==0)
								dwGoodsNum = 1;
							else
								dwGoodsNum = dVal;

							//������Ϣ
							pGoodTypeElem->Attribute( "discount", &dVal );
							dwDiscount = dVal;
							//��ʱ����
							strTimeLimit = pGoodTypeElem->Attribute( "timelimit" );
							//��������
							int ival;
							pGoodTypeElem->Attribute( "quantitylimit", &ival );
							lQuantityLimit = ival;
							//�޹�����
							pGoodTypeElem->Attribute( "buylimit", &ival );
							lBuyLimit = ival;

							IncrementShopElem	elemIncShopMap( dwGoodsType,dwMapGoodsIndex, dwPrice,dwGoodsNum,dwDiscount, 
								strTimeLimit,lQuantityLimit,lBuyLimit);
							m_mMapIncGoodsList[dwMapGoodsIndex] = elemIncShopMap;
							m_mMapList[dwGoodsIndex] = dwMapGoodsIndex;
						}
					}
				}
			}
		}
	}
	return true;
}

// ��ӵ�CByteArray
bool CIncrementShopList::AddToByteArray(vector<BYTE>* pByteArray)
{
	if (!pByteArray)
		return false;

	//����������Ʒ�б�
	CodeShopGoodsList( pByteArray, M_TOP10 );

	//������Ʒ�Ƽ��б�
	CodeShopGoodsList( pByteArray, M_NEWGOODS );

	//���뵼����Ʒ�б�
	CodeTypeGoodsList( pByteArray,  M_GUIDETYPE );

	//���������̵�ɸѡ��Ʒ�б�
	CodeTypeGoodsList( pByteArray, M_FILTER_XINGZUAN );

	//����λ���̵�ɸѡ��Ʒ�б�
	CodeTypeGoodsList( pByteArray, M_FILTER_WEIMIAN );

	//�����ȯ�̵�ɸѡ��Ʒ�б�
	CodeTypeGoodsList( pByteArray, M_FILTER_TICKET );

	//�����̵���ϸ��Ʒ�б�
	CodeDetailedGoodsList( pByteArray );

	//������Ʒ����ӳ���
	CodeIncClassGoodsList( pByteArray );

	//�������ӳ�����ñ�
	CodeMapIncShopList( pByteArray );

	//���뷵����������
	CodeReturnParam(pByteArray);

	char buf[8192] = "";
	strcpy_s(buf, 8192, m_sAffiche.data());
	long lNum = m_sAffiche.size();
	_AddToByteArray(pByteArray, lNum);
	_AddToByteArray(pByteArray,buf, lNum);

	return true;
}

//����
bool CIncrementShopList::DecordFromByteArray(BYTE* pByte, long& pos)
{
	if (!pByte)
		return false;

	//�������
	ClearIncShop();

	//����������Ʒ�б�
	DecordShopGoodsList( pByte, pos, M_TOP10 );

	//������Ʒ�Ƽ��б�
	DecordShopGoodsList( pByte, pos, M_NEWGOODS );

	//���뵼����Ʒ�б�
	DecordTypeGoodsList( pByte, pos, M_GUIDETYPE );

	//���������̵�ɸѡ��Ʒ�б�
	DecordTypeGoodsList( pByte, pos, M_FILTER_XINGZUAN );

	//����λ���̵�ɸѡ��Ʒ�б�
	DecordTypeGoodsList( pByte, pos, M_FILTER_WEIMIAN );

	//�����ȯ�̵�ɸѡ��Ʒ�б�
	DecordTypeGoodsList( pByte, pos, M_FILTER_TICKET );

	//�����̵���ϸ��Ʒ�б�
	DecordDetailedGoodsList( pByte, pos );

	//������Ʒ����ӳ���
	DecordIncClassGoodsList( pByte, pos );

	//�������ӳ�����ñ�
	DecordMapIncShopList( pByte, pos );

	//���뷵����������
	DecordReturnParam(pByte,pos);

	//���빫����Ϣ
	char buf[8192] = "";
	long lNum = _GetLongFromByteArray(pByte, pos);
	_GetBufferFromByteArray(pByte, pos, buf, lNum);
	m_sAffiche = buf;

	return true;
}

//������Ʒ�б�
void CIncrementShopList::ParseGoodsList(TiXmlElement* pTiXmlElem, VEC_GOODSIDX& vecGoodsIndex )
{
	string	strOrignalName;
	DWORD	dwGoodsIndex;
	TiXmlElement* pGoodsElem = pTiXmlElem->FirstChildElement();
	for ( ; pGoodsElem; pGoodsElem=pGoodsElem->NextSiblingElement() )
	{
		if ( !strcmp( pGoodsElem->Value(), "Goods" ) )
		{
			strOrignalName = pGoodsElem->Attribute( "OrignalName" );
			dwGoodsIndex	= CGoodsFactory::QueryGoodsIDByOriginalName(strOrignalName.c_str());
			if( !dwGoodsIndex )
			{
				char str[256];
				_snprintf(str, 256, "��ֵ�̵� Goods ���ô���ԭʼ��Ϊ %s ����Ʒ�����ڣ�",strOrignalName.c_str());
				MessageBox( NULL, str, "���ô���", MB_OK );
				continue;
			}
			vecGoodsIndex.push_back(dwGoodsIndex);
		}
	}
}


//��������&��Ʒ�Ƽ��б�
void CIncrementShopList::CodeShopGoodsList( vector<BYTE>* pByteArray, eCodeMode eCodeMode )
{
	if ( !pByteArray ) return;
	if ( eCodeMode == M_TOP10 )
	{
		CodeShopGoodsListToByteArray( pByteArray, m_mTop10List );
	}
	else if ( eCodeMode == M_NEWGOODS )
	{
		CodeShopGoodsListToByteArray( pByteArray, m_mNewGoodsList );
	}
}

void CIncrementShopList::CodeShopGoodsListToByteArray( vector<BYTE>* pByteArray,MAP_TOP10&  mapGoodsList )
{
	if ( !pByteArray ) return;

	long lNum = (long)mapGoodsList.size();
	_AddToByteArray( pByteArray, lNum );
	ITR_TOP10 itTop10 = mapGoodsList.begin();
	for ( ; itTop10 != mapGoodsList.end(); ++itTop10 )
	{
		DWORD dwShopType = itTop10->first;
		_AddToByteArray( pByteArray, dwShopType );
		VEC_GOODSIDX& vecTop10GoodsList = itTop10->second;
		lNum = (long)vecTop10GoodsList.size();
		_AddToByteArray( pByteArray, lNum );
		ITR_GOODSIDX it = vecTop10GoodsList.begin(); 
		for ( ; it != vecTop10GoodsList.end(); ++it )
		{
			_AddToByteArray( pByteArray, *it );
		}
	}
}

//���뵼��&ɸѡ�б�
void CIncrementShopList::CodeTypeGoodsList( vector<BYTE>* pByteArray, eCodeMode eCodeMode )
{
	if ( !pByteArray ) return;
	
	switch( eCodeMode )
	{
	case M_GUIDETYPE:
		CodeTypeGoodsListToByteArray( pByteArray, m_mGuideGoodsList );
		break;
	case M_FILTER_XINGZUAN:
		CodeTypeGoodsListToByteArray( pByteArray, m_mFilterListOfXShop );
		break;
	case M_FILTER_WEIMIAN:
		CodeTypeGoodsListToByteArray( pByteArray, m_mFilterListOfWShop );
	    break;
	case M_FILTER_TICKET:
		CodeTypeGoodsListToByteArray( pByteArray, m_mFilterListOfTShop );
	    break;
	default:
	    break;
	}
}

void CIncrementShopList::CodeTypeGoodsListToByteArray( vector<BYTE>* pByteArray,MAP_GUIDEGOODS&  mapTypeGoodsList )
{
	if ( !pByteArray ) return;

	long lNum = (long)mapTypeGoodsList.size();
	_AddToByteArray( pByteArray, lNum );
	ITR_GUIDEGOODSLIST itTypeGoods = mapTypeGoodsList.begin();
	for ( ; itTypeGoods != mapTypeGoodsList.end(); ++itTypeGoods )
	{
		DWORD dwType = itTypeGoods->first;
		_AddToByteArray( pByteArray, dwType );
		MAP_TYPEGOODS& mapGoodsList = itTypeGoods->second;
		lNum = (long)mapGoodsList.size();
		_AddToByteArray( pByteArray, lNum );
		ITR_TYPEGOODS iter = mapGoodsList.begin();
		for ( ; iter != mapGoodsList.end(); ++iter )
		{
			string strType = iter->first;
			_AddToByteArray( pByteArray, strType.c_str() );
			VEC_GOODSIDX& vecGoodsList = iter->second;
			lNum = (long)vecGoodsList.size();
			_AddToByteArray( pByteArray, lNum );
			ITR_GOODSIDX it = vecGoodsList.begin();
			for ( ; it != vecGoodsList.end(); ++it )
			{
				_AddToByteArray( pByteArray, *it );
			}
		}
	}
}

//������Ʒ��ϸ��Ϣ�б�
bool CIncrementShopList::CodeDetailedGoodsList( vector<BYTE>* pByteArray )
{
	if (!pByteArray) return false;

	long lNum = (long)m_mIncGoodsList.size();
	_AddToByteArray( pByteArray, lNum );
	ITR_INCGOODS itIncGoodsList = m_mIncGoodsList.begin();
	for ( ; itIncGoodsList != m_mIncGoodsList.end(); ++itIncGoodsList )
	{
		DWORD dwShopType = itIncGoodsList->first;
		_AddToByteArray( pByteArray, dwShopType );
		MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
		lNum = mapIncGoodsList.size();
		_AddToByteArray(pByteArray, lNum);
		ITR_INCSHOP iter = mapIncGoodsList.begin();
		for ( ; iter != mapIncGoodsList.end(); ++iter )
		{
			IncrementShopElem *pElem = &(iter->second);
			if( pElem )
			{
				_AddToByteArray(pByteArray, pElem->dwGoodsType);
				_AddToByteArray(pByteArray, pElem->dwGoodsIndex);
				_AddToByteArray( pByteArray, pElem->dwPrice );
				_AddToByteArray(pByteArray, pElem->dwGoodsNum);
				_AddToByteArray( pByteArray, pElem->dwDiscount );
				_AddToByteArray( pByteArray, pElem->strTimeLimit.c_str() );
				_AddToByteArray( pByteArray, pElem->lQuantityLimit );
				_AddToByteArray( pByteArray, pElem->lBuyLimit );
			}
			else
				return false;
		}
	}
	return true;
}

bool CIncrementShopList::CodeMapIncShopList( vector<BYTE>* pByteArray )
{
	if (!pByteArray) return false;
	long lNum = (long)m_mMapIncGoodsList.size();
	_AddToByteArray( pByteArray, lNum );
	ITR_INCSHOP it = m_mMapIncGoodsList.begin();
	for (; it != m_mMapIncGoodsList.end(); ++it)
	{
		DWORD dwGoodsIndex = it->first;
		_AddToByteArray(pByteArray, dwGoodsIndex);
		IncrementShopElem *pElem = &(it->second);
		if( pElem )
		{
			_AddToByteArray(pByteArray, pElem->dwGoodsType);
			_AddToByteArray(pByteArray, pElem->dwGoodsIndex);
			_AddToByteArray( pByteArray, pElem->dwPrice );
			_AddToByteArray(pByteArray, pElem->dwGoodsNum);
			_AddToByteArray( pByteArray, pElem->dwDiscount );
			_AddToByteArray( pByteArray, pElem->strTimeLimit.c_str() );
			_AddToByteArray( pByteArray, pElem->lQuantityLimit );
			_AddToByteArray( pByteArray, pElem->lBuyLimit );
		}
		else
			return false;
	}
	lNum = (long)m_mMapList.size();
	_AddToByteArray(pByteArray, lNum);
	ITR_MAPLIST iter = m_mMapList.begin();
	for (; iter!=m_mMapList.end(); ++iter)
	{
		_AddToByteArray(pByteArray,iter->first);
		_AddToByteArray(pByteArray,iter->second);
	}
	return true;
}

//������Ʒ����ӳ���
bool CIncrementShopList::CodeIncClassGoodsList( vector<BYTE>* pByteArray )
{
	if (!pByteArray) return false;

	long lNum = (long)m_mIncClassGoodsList.size();
	_AddToByteArray( pByteArray, lNum );
	ITR_INCCLASSGOODS itIncClassGoodsList = m_mIncClassGoodsList.begin();
	for ( ; itIncClassGoodsList != m_mIncClassGoodsList.end(); ++itIncClassGoodsList )
	{
		DWORD dwShopType = itIncClassGoodsList->first;
		_AddToByteArray( pByteArray, dwShopType );
		MAP_CLASSGOODS& mapClassGoodsList = itIncClassGoodsList->second;
		lNum = mapClassGoodsList.size();
		_AddToByteArray(pByteArray, lNum);
		ITR_CLASSGOODS iter = mapClassGoodsList.begin();
		for ( ; iter != mapClassGoodsList.end(); ++iter )
		{
			DWORD dwClassType = iter->first;
			_AddToByteArray( pByteArray, dwClassType );
			VEC_GOODSIDX& vecGoodsIdx = iter->second;
			lNum = vecGoodsIdx.size();
			_AddToByteArray( pByteArray, lNum );
			ITR_GOODSIDX it = vecGoodsIdx.begin();
			for ( ; it != vecGoodsIdx.end(); ++it )
			{
				_AddToByteArray(pByteArray, *it );
			}
		}
	}
	return true;
}
//���뷵���������ñ�
bool CIncrementShopList::CodeReturnParam(vector<BYTE>* pByteArray)
{
	if (!pByteArray)
	{
		return false;
	}
	_AddToByteArray(pByteArray,m_tgReturnParm.fNoramlRatio);
	_AddToByteArray(pByteArray,m_tgReturnParm.fSpecialRatio);
	_AddToByteArray(pByteArray,m_tgReturnParm.strStartTime.c_str());
	_AddToByteArray(pByteArray,m_tgReturnParm.strEndTime.c_str());
	long lNum = m_tgReturnParm.lSGoodsName.size();
	_AddToByteArray(pByteArray,lNum);
	list<string>::iterator it = m_tgReturnParm.lSGoodsName.begin();
	for (; it!=m_tgReturnParm.lSGoodsName.end(); ++it)
	{
		_AddToByteArray(pByteArray,(*it).c_str());
	}
	return true;
}

//���뷵���������ñ�
void CIncrementShopList::DecordReturnParam(BYTE* pByte, long& pos)
{

	if (!pByte)
	{
		return;
	}
	char strTemp[256];
	m_tgReturnParm.lSGoodsName.clear();
	m_tgReturnParm.fNoramlRatio = _GetFloatFromByteArray(pByte,pos);
	m_tgReturnParm.fSpecialRatio = _GetFloatFromByteArray(pByte,pos);
	_GetStringFromByteArray(pByte,pos,strTemp);
	m_tgReturnParm.strStartTime = strTemp;
	_GetStringFromByteArray(pByte,pos,strTemp);
	m_tgReturnParm.strEndTime = strTemp;
	long lNum = _GetLongFromByteArray(pByte,pos);
	for (long l=0; l<lNum; ++l)
	{
		_GetStringFromByteArray(pByte,pos,strTemp);
		m_tgReturnParm.lSGoodsName.push_back(strTemp);
	}

}


//������Ʒ�б�
void CIncrementShopList::DecordGoodsList( BYTE* pByte, long& pos, VEC_GOODSIDX& vecGoodsList )
{
	if (!pByte) return;

	long lGoodsNum = _GetLongFromByteArray( pByte, pos );
	vecGoodsList.reserve( lGoodsNum );
	for ( int i = 0; i < lGoodsNum; ++i )
	{
		DWORD dwGoodIndex = _GetDwordFromByteArray( pByte, pos );
		vecGoodsList.push_back( dwGoodIndex );
	}
}

//����������Ʒ&��Ʒ�Ƽ��б�
void CIncrementShopList::DecordShopGoodsList( BYTE* pByte, long& pos, eMode eDecordMode )
{
	if (!pByte) return;

	long lNum = _GetLongFromByteArray( pByte, pos );
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwShopType = _GetDwordFromByteArray( pByte, pos );
		VEC_GOODSIDX vecGoodsList;
		DecordGoodsList( pByte, pos, vecGoodsList );
		if ( eDecordMode == M_TOP10 )
		{
			m_mTop10List.insert( make_pair( dwShopType, vecGoodsList ) );
		}
		else if ( eDecordMode == M_NEWGOODS )
		{	
			m_mNewGoodsList.insert( make_pair( dwShopType, vecGoodsList ) );
		}
	}
}

//���뵼��&ɸѡ��Ʒ�б�
void CIncrementShopList::DecordTypeGoodsList( BYTE* pByte, long& pos, eMode eDecordMode )
{
	if (!pByte) return;

	long lNum = _GetLongFromByteArray( pByte, pos );
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwType = _GetDwordFromByteArray( pByte, pos );
		MAP_TYPEGOODS mapGoods;
		long lTypeNum = _GetLongFromByteArray( pByte, pos );
		for ( int j = 0; j < lTypeNum; ++j )
		{
			char strTemp[256];
			_GetStringFromByteArray( pByte, pos, strTemp );
			string strType = strTemp;
			VEC_GOODSIDX vecGoodsList;
			DecordGoodsList( pByte, pos, vecGoodsList );
			mapGoods.insert( make_pair( strType, vecGoodsList ) );
		}

		switch( eDecordMode )
		{
		case M_GUIDETYPE:
			m_mGuideGoodsList.insert( make_pair( dwType, mapGoods ) );
			break;
		case M_FILTER_WEIMIAN:
			m_mFilterListOfWShop.insert( make_pair( dwType, mapGoods ) );
			break;
		case M_FILTER_XINGZUAN:
			m_mFilterListOfXShop.insert( make_pair( dwType, mapGoods ) );
		    break;
		case M_FILTER_TICKET:
			m_mFilterListOfTShop.insert( make_pair( dwType, mapGoods ) );
		    break;
		default:
		    break;
		}
	}
}

//������Ʒ��ϸ��Ϣ�б�
void CIncrementShopList::DecordDetailedGoodsList( BYTE* pByte, long& pos )
{
	if (!pByte) return;

	long lNum = _GetLongFromByteArray(pByte, pos);
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwShopType = _GetDwordFromByteArray( pByte, pos );
		MAP_INCSHOP mapIncElem;
		long lGoodsNum = _GetLongFromByteArray( pByte, pos );
		for ( int j = 0; j < lGoodsNum; ++j )
		{
			char strTemp[256];
			IncrementShopElem Elem;
			Elem.dwGoodsType		= _GetDwordFromByteArray(pByte, pos);
			Elem.dwGoodsIndex		= _GetDwordFromByteArray(pByte, pos);
			Elem.dwPrice = _GetDwordFromByteArray( pByte, pos );
			Elem.dwGoodsNum			= _GetDwordFromByteArray(pByte, pos);
			Elem.dwDiscount = _GetDwordFromByteArray( pByte, pos );
			_GetStringFromByteArray( pByte, pos, strTemp );
			Elem.strTimeLimit = strTemp;
			Elem.lQuantityLimit = _GetDwordFromByteArray( pByte, pos );
			Elem.lBuyLimit = _GetDwordFromByteArray( pByte, pos );

			mapIncElem[Elem.dwGoodsIndex] = Elem;
		}
		m_mIncGoodsList[dwShopType] = mapIncElem;
	}
}

//�������ӳ�����ñ�
void CIncrementShopList::DecordMapIncShopList( BYTE* pByte, long& pos )
{
	if (!pByte) return;

	long lNum = _GetLongFromByteArray(pByte, pos);
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwGoodsIndex = _GetDwordFromByteArray( pByte, pos );

		char strTemp[256];
		IncrementShopElem Elem;
		Elem.dwGoodsType		= _GetDwordFromByteArray(pByte, pos);
		Elem.dwGoodsIndex		= _GetDwordFromByteArray(pByte, pos);
		Elem.dwPrice = _GetDwordFromByteArray( pByte, pos );
		Elem.dwGoodsNum			= _GetDwordFromByteArray(pByte, pos);
		Elem.dwDiscount = _GetDwordFromByteArray( pByte, pos );
		_GetStringFromByteArray( pByte, pos, strTemp );
		Elem.strTimeLimit = strTemp;
		Elem.lQuantityLimit = _GetDwordFromByteArray( pByte, pos );
		Elem.lBuyLimit = _GetDwordFromByteArray( pByte, pos );

		m_mMapIncGoodsList[dwGoodsIndex] = Elem;
	}

	lNum = _GetLongFromByteArray(pByte, pos);
	for (int i = 0; i < lNum; ++i)
	{
		DWORD dwGoodsIndex = _GetDwordFromByteArray( pByte, pos );
		DWORD dwGoodsMapIndex = _GetDwordFromByteArray( pByte, pos );
		m_mMapList[dwGoodsIndex] = dwGoodsMapIndex;
	}
}

//������Ʒ����ӳ���
void CIncrementShopList::DecordIncClassGoodsList( BYTE* pByte, long& pos )
{
	if (!pByte) return;

	long lNum = _GetLongFromByteArray(pByte, pos);
	for ( int i = 0; i < lNum; ++i )
	{
		DWORD dwShopType = _GetDwordFromByteArray( pByte, pos );
		MAP_CLASSGOODS mapIncClass;
		long lGoodsNum = _GetLongFromByteArray( pByte, pos );
		for ( int j = 0; j < lGoodsNum; ++j )
		{
			DWORD dwClassType = _GetDwordFromByteArray( pByte, pos );
			long lIdxNum = _GetLongFromByteArray( pByte, pos );
			VEC_GOODSIDX vecGoodsIdx;
			for ( int k = 0; k < lIdxNum; ++k )
			{
				DWORD dwGoodsIdx = _GetDwordFromByteArray( pByte, pos );
				vecGoodsIdx.push_back( dwGoodsIdx );
			}
			mapIncClass[dwClassType] = vecGoodsIdx;
		}
		m_mIncClassGoodsList[dwShopType] = mapIncClass;
	}
}

//ʱ��Ƚ�
bool CIncrementShopList::TimeLimited(const string& strTime)
{
	if(strTime.length()!=8)
		return false;
	string strYear=strTime.substr(0,4);
	string strMonth=strTime.substr(4,2);
	string strDay=strTime.substr(6);
	WORD wYear=static_cast<WORD>(atoi(strYear.c_str()));
	WORD wMonth=static_cast<WORD>(atoi(strMonth.c_str()));
	WORD wDay=static_cast<WORD>(atoi(strDay.c_str()));
	SYSTEMTIME	stTime;	
	GetLocalTime(&stTime);
	
	if(stTime.wYear<wYear)
		return true;
	if(stTime.wYear>wYear)
		return false;
	if(stTime.wMonth<wMonth)
		return true;
	if(stTime.wMonth>wMonth)
		return false;
	if(stTime.wDay<wDay)
		return true;
	if(stTime.wDay>wDay)
		return false;
	return true;
}

//ȡ���޹���Ŀ
long CIncrementShopList::GetLimitNumByID( DWORD dwShopType, DWORD dwGoodsIndex )
{
	long lNum=0;
	ITR_INCGOODS itIncGoodsList = m_mIncGoodsList.find( dwShopType );
	if( itIncGoodsList != m_mIncGoodsList.end() )
	{
		MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
		ITR_INCSHOP it = mapIncGoodsList.find( dwGoodsIndex );
		if (  it != mapIncGoodsList.end() )
		{
			lNum = it->second.lBuyLimit;
		}
	}
	return lNum;
}

//ȡ��������Ŀ
long CIncrementShopList::GetWorldBuyLimitNum( DWORD dwShopType, DWORD dwGoodsIndex )
{
	long lNum=0;
	ITR_INCGOODS itIncGoodsList = m_mIncGoodsList.find( dwShopType );
	if( itIncGoodsList != m_mIncGoodsList.end() )
	{
		MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
		ITR_INCSHOP it = mapIncGoodsList.find( dwGoodsIndex );
		if (  it != mapIncGoodsList.end() )
		{
			lNum = it->second.lQuantityLimit;
		}
	}
	return lNum;
}

//ȡ����ʱ��Ϣ
string CIncrementShopList::GetLimitTimeByID( DWORD dwShopType, DWORD dwGoodsIndex )
{
	string strTimeLimit = "";
	ITR_INCGOODS itIncGoodsList = m_mIncGoodsList.find( dwShopType );
	if( itIncGoodsList != m_mIncGoodsList.end() )
	{
		MAP_INCSHOP& mapIncGoodsList = itIncGoodsList->second;
		ITR_INCSHOP it = mapIncGoodsList.find( dwGoodsIndex );
		if (  it != mapIncGoodsList.end() )
		{
			strTimeLimit = it->second.strTimeLimit;
		}
	}
	return strTimeLimit;
}

bool CIncrementShopList::IsProfitGoods(DWORD dwGoodsID)
{
	ITR_MAPLIST iter = m_mMapList.find(dwGoodsID);
	if (iter != m_mMapList.end())
	{
		return true;
	}
	return false;
}
//�ж��Ƿ񷵻ص��
//����true��ʾ������false��ʾ������
bool CIncrementShopList::IsReturnDianGuan(DWORD dwGoodsIdx)
{
	string strName =(char*)(CGoodsFactory::QueryGoodsOriginalName(dwGoodsIdx));
	list<string>::iterator it = find(m_tgReturnParm.lSGoodsName.begin(),m_tgReturnParm.lSGoodsName.end(),strName);
	if (it != m_tgReturnParm.lSGoodsName.end())
	{
		return false;
	}
	return true;
}

//�����������
long CIncrementShopList::GetReturnNum(long lTotalAmount,float &fReturnRatio)
{

	//float fReturnRatio = 0.0;
	if (IsStartTime() && IsEndTime() && IsHour())
	{
		//ִ�����ⷵ������
		fReturnRatio = m_tgReturnParm.fSpecialRatio;
	}
	else
	{
		//��ͨ��������
		fReturnRatio = m_tgReturnParm.fNoramlRatio;
	}

	float fTotalAmount = fReturnRatio*lTotalAmount;
	
	long lNum = (long)fTotalAmount;
	if (fTotalAmount-lNum>0.0)
	{
		lNum = lNum +1;
	}

	
	return lNum;
}

//�жϽ���ʱ��
bool CIncrementShopList::IsEndTime()	
{
	//����ʱ���ж�
	if(m_tgReturnParm.strEndTime.length()!=10)
		return false;

	string strYear = m_tgReturnParm.strEndTime.substr(0,4);
	string strMonth = m_tgReturnParm.strEndTime.substr(4,2);
	string strDay = m_tgReturnParm.strEndTime.substr(6,2);
	
	WORD wYear = static_cast<WORD>(atoi(strYear.c_str()));
	WORD wMonth = static_cast<WORD>(atoi(strMonth.c_str()));
	WORD wDay = static_cast<WORD>(atoi(strDay.c_str()));
	//	

	SYSTEMTIME	stTime;	
	GetLocalTime(&stTime);

	if(stTime.wYear>wYear)
		return false;

	if(stTime.wMonth>wMonth)			
		return false;

	if(stTime.wMonth==wMonth && stTime.wDay>wDay)
		return false;

	return true;
}
//�ж�Сʱ

bool CIncrementShopList::IsHour()													
{
	string strStartHour = m_tgReturnParm.strStartTime.substr(8,2);
	WORD wStartHour = static_cast<WORD>(atoi(strStartHour.c_str()));
	string strEndHour = m_tgReturnParm.strEndTime.substr(8,2);
	WORD wEndHour = static_cast<WORD>(atoi(strEndHour.c_str()));

	SYSTEMTIME	stTime;	
	GetLocalTime(&stTime);
	if (stTime.wHour>=wStartHour && stTime.wHour<=wEndHour)
		return true;
	else
		return false;
}
//��ʼʱ���ж�
bool CIncrementShopList::IsStartTime()
{
	//��ʼʱ���ж�
	if(m_tgReturnParm.strStartTime.length()!=10)
		return false;

	string strYear = m_tgReturnParm.strStartTime.substr(0,4);
	string strMonth = m_tgReturnParm.strStartTime.substr(4,2);
	string strDay = m_tgReturnParm.strStartTime.substr(6,2);

	WORD wYear = static_cast<WORD>(atoi(strYear.c_str()));
	WORD wMonth = static_cast<WORD>(atoi(strMonth.c_str()));
	WORD wDay = static_cast<WORD>(atoi(strDay.c_str()));
	

	SYSTEMTIME	stTime;	
	GetLocalTime(&stTime);

	if(stTime.wYear<wYear)
		return false;

	if(stTime.wMonth<wMonth)
		return false;


	if(stTime.wMonth==wMonth && stTime.wDay<wDay)
		return false;

	return true;	
}

//�������
void CIncrementShopList::ClearIncShop()
{
	m_mTop10List.clear();											//���������Ʒ�б�
	m_mNewGoodsList.clear();										//�����Ʒ�Ƽ��б�
	m_mGuideGoodsList.clear();										//���������Ʒ�б�
	m_mFilterListOfWShop.clear();									//���λ���̵�ɸѡ��Ʒ�б�
	m_mFilterListOfXShop.clear();									//��������̵�ɸѡ��Ʒ�б�
	m_mFilterListOfTShop.clear();									//�����ȯ�̵�ɸѡ��Ʒ�б�
	m_mIncGoodsList.clear();										//����̵���ϸ��Ʒ�б�
	m_mIncClassGoodsList.clear();									//�����Ʒ����ӳ���
	m_sAffiche.clear();												//�������
	m_mMapIncGoodsList.clear();
	m_mMapList.clear();
}
