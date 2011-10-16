#include "StdAfx.h"
#include "Monster.h"
#include "ServerRegion.h"
#include "Player.h"
#include "Colony.h"
#include "Session/CSessionFactory.h"
#include "Goods/CGoods.h"
#include "Goods/CGoodsFactory.h"
#include "PhaseSystem/PhaseManager.h"
#include "MessagePackage/CS2CContainerObjectMove.h"
#include "BuffSkill/NewSkillFactory.h"
#include "BuffSkill/NewSkillBaseProperties.h"
#include "OrganizingSystem/GSTeam.h"
#include "OrganizingSystem/GameOrganizingCtrl.h"
#include "StateAI/MonsterAI.h"
#include "StateAI/StateImpl/StateImplDef.h"
#include "StateAI/AIEventSender.h"
#include "../Net/Message.h"
#include "LuaScript/Script.h"
#include "LuaScript/ScriptSys.h"
#include "MonsterTaunt/TauntHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

set<string>  CMonster::m_mapOtherUpdateDirtyAttrData;

//===========================================================================================================================
///
/// These codes below are only used for the Goods Drop function.
///
//===========================================================================================================================
struct Range
{
	int Min;
	int Max;
	size_t Index;

	Range( int _min, int _max, size_t _i ) : Min( _min ), Max( _max ), Index( _i ) 
	{
	}
};

/// range list
typedef std::vector<Range> RangeListType;

/// goods list
typedef std::vector<CMonsterList::tagDropGoods::tagOneGoods> GoodListType;

/// build the range list for goods list
static size_t BuildRangeTable( const GoodListType &goods, RangeListType &ranges )
{
	std::size_t goods_size = goods.size();
	std::size_t size = ranges.size();
	int prev = 0;
	for( std::size_t i = 0; i < goods_size; ++ i )
	{
		ranges.push_back( Range( prev + 1, goods[i].lProbability + prev, i ) );
		prev += goods[i].lProbability;
	}
	return ranges.size() - size;
}

/// query the goods index by a random number
static size_t QueryGoodsIndex( const RangeListType &ranges, int value )
{
	for( RangeListType::const_iterator it = ranges.begin(); it != ranges.end(); ++ it )
	{
		if( value >= (*it).Min && value <= (*it).Max )
		{
			return (*it).Index;
		}
	}
	// fatal error
	return (size_t)(-1);
}

///
/// ��Ʒ����ȼ�˥�����㺯��
///
#define COMP_RANGE 10e-5

static long CalLevelOdds( long level_sub, long odds_max, long odds, float level_atte, float level_atte_min )
{
	if( level_sub <= 0 || ( level_atte > -COMP_RANGE && level_atte < COMP_RANGE ) )
	{
		return odds;
	}
	// ����Ϊ�����odds_max������ֵ�������е�level_atte�������odds��
	long level_atte_l = (long)level_atte * level_sub * odds;
	long level_min_l  = (long)level_atte_min * odds_max;

	if( odds <= level_min_l )
	{
		// ���ô��󣬲���ԭʼ����
		return odds;
	}

	long ret_odds = odds - level_atte_l;
	// ˥����СΪlevel_min_l
	ret_odds = ret_odds < level_min_l ? level_min_l : ret_odds;

	return ret_odds;
}

//===========================================================================================================================

CMonster::CMonster(void)
{
	SetType(TYPE_MONSTER);
	SetIsRun(false);
	//##��ӻ�����ս������
	CMoveShape::InitSkills();

	InitNameValueMap();

	//	m_pBaseProperty = NULL;	
	m_dwAICount = 0;	
	//m_iBaseSkillNum	= 0;
	m_bLockAttackImmediately = false;	
	//m_FirstAttackPlayerID=0;

	//////////////////////////////////////////////////////////////////////////
	//�����������
	//////////////////////////////////////////////////////////////////////////
	//�������
	m_tgMonsterProperty.dwIndex	= 0;
	//ͼƬID
	m_tgMonsterProperty.dwPicID = 0;
	//ͼƬ�ȼ�
	m_tgMonsterProperty.dwPicLevel = 0;
	//������ɫ���,��Ӧ���ͻ�������
	m_tgMonsterProperty.dwNameColor = 0;
	//Ѫ����ɫ��0����ʾ����ʾѪ�� 1����ɫ 2����ɫ
	m_tgMonsterProperty.dwHpBarColor = 1;
	//����ID
	m_tgMonsterProperty.dwSoundID = 0;
	//�ȼ�
	m_tgMonsterProperty.wLevel = 0;
	//����ֵ
	m_tgMonsterProperty.dwExp = 0;
	//ְҵ����
	m_tgMonsterProperty.dwOccuPoint = 0;
	//����ID
	m_tgMonsterProperty.byCountry = 0;
	//�Ƿ����ѱ��
	m_tgMonsterProperty.byCatchable = 0;
	//�����ѱ������
	m_tgMonsterProperty.dwMaxCatchFastness = 0;			
	//���ҹ���ֵ
	m_tgMonsterProperty.wCountryContriValue = 0;
	//SP����ֵ
	m_tgMonsterProperty.wSPContriValue = 0;
	//Ԫ�����
	m_tgMonsterProperty.eEType = (eEleType)0;
	//Npc��� (NNPC,Monster,Guard)
	m_tgMonsterProperty.eNType = (eNpcType)0;
	//����
	m_tgMonsterProperty.eClass_ = (eClass)0;
	//��Ⱥ(AIʹ��)
	m_tgMonsterProperty.wRace = 0;
	//�����ƶ��ĸ���(����)
	m_tgMonsterProperty.wMoveRandomValue = 0;
	//ͣ����ʱ��(��һ�������м�ͣ��֡��)
	m_tgMonsterProperty.wStopTime = 0;
	//AI��϶
	m_tgMonsterProperty.wAIHangupType = 0;
	//����(0:1x1 1:3*3 2:5*5)*
	m_tgMonsterProperty.wFigure = 0;
	//��ǰ����ֵ
	m_tgMonsterProperty.dwHp = 0;
	//
	m_tgMonsterProperty.lRefeashIndex=-1;

	//////////////////////////////////////////////////////////////////////////
	//m_dwTameAttemptCount		= 0;
	//m_pVariableList = OBJ_CREATE(CVariableList);//new CVariableList;
	m_dwColonyID				= 0;
	m_wMemberType				= 2;
	m_bColonyState				=FALSE;
	m_bCatchFlag=false;

    m_tauntHandler = new TauntHandler( this ); 
}

CMonster::~CMonster(void)
{
	ClearPassiveSkill();	
	ClearAllBuff();
	//CAIFactory::DeleteAI(m_pAI);
	delete m_pAI;
	//SAFE_DELETE(m_pPetAI);

	//ע�����Ը��¶�ʱ��
	CancelUpdateAttrTimer();
	//OBJ_RELEASE(CVariableList, m_pVariableList);

	//DelectAllScripByShape(GetExID());
    delete m_tauntHandler;
}


void CMonster::CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB)
{
	long lShapeNum = 0;
	long lMonsterNum = 0;
	vector<BYTE>	byShapeDirtyData;byShapeDirtyData.clear();
	vector<BYTE>	byMonsterDirtyData;byMonsterDirtyData.clear();

	set<string>::iterator itUpdate = m_mapDirtyAttrData.begin();
	for(; itUpdate != m_mapDirtyAttrData.end(); itUpdate++)
	{
		string name = *itUpdate;			
		mapShapeNameValueItr sitr = m_mapShapeNameValue.find(name);
		if (sitr != m_mapShapeNameValue.end())
		{
			lShapeNum++;
			//msg.Add((BYTE)saitr->second); // ��������
			::_AddToByteArray(&byShapeDirtyData, (WORD)sitr->second.attrEnumValue);
			switch (sitr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(BYTE));
				::_AddToByteArray(&byShapeDirtyData, (BYTE)*(BYTE*)(sitr->second.plValue));
				break;
			case 1:	// word
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(WORD));
				::_AddToByteArray(&byShapeDirtyData, (WORD)*(WORD*)(sitr->second.pwValue));
				break;
			case 2:	// dword
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(DWORD));
				::_AddToByteArray(&byShapeDirtyData, (DWORD)*(DWORD*)(sitr->second.pdwValue));
				break;
			case 3:	// float
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(float));
				::_AddToByteArray(&byShapeDirtyData, (float)*(float*)(sitr->second.pfValue));
				break;
			case 4:	// long
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(long));
				::_AddToByteArray(&byShapeDirtyData, (long)*(long*)(sitr->second.pValue));
				break;
			case 5:	// short
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(short));
				::_AddToByteArray(&byShapeDirtyData, (short)*(short*)(sitr->second.psValue));
				break;
			case 6:	// bool
				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(bool));
				::_AddToByteArray(&byShapeDirtyData, (char)*(bool*)(sitr->second.pbValue));
				break;
			default:
				break;
			}
		}
		long baseValue = 0;
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
			lMonsterNum++;
			// ��������
			::_AddToByteArray(&byMonsterDirtyData, (WORD)itr->second.attrEnumValue);

			long sendValue = INVILID_VALUE;

			switch (itr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(BYTE));
				if((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_') // ����ֵ
				{
					BYTE EValue = *(BYTE*)(itr->second.plValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(itr->second.plValue);

				::_AddToByteArray(&byMonsterDirtyData, (BYTE)sendValue);

				break;
			case 1:	// word
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(WORD));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(itr->second.pwValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(itr->second.pwValue);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(DWORD));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(itr->second.pdwValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(itr->second.pdwValue);
				::_AddToByteArray(&byMonsterDirtyData, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(float));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(itr->second.pfValue);
					sendValue = GetCurrentValue(itr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(itr->second.pfValue);
				::_AddToByteArray(&byMonsterDirtyData, (float)sendValue);
				break;
			case 4:	// long
				//msg.Add((BYTE)ATTR_DATA_LONG);
				//msg.Add((WORD)sizeof(long));
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(long));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(itr->second.pValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(itr->second.pValue);
				::_AddToByteArray(&byMonsterDirtyData, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(short));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(itr->second.psValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(itr->second.psValue);
				::_AddToByteArray(&byMonsterDirtyData, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(&byMonsterDirtyData, (WORD)sizeof(bool));
				if(((*itUpdate)[0] == 'C' && (*itUpdate)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(itr->second.pbValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(itr->second.pbValue);
				::_AddToByteArray(&byMonsterDirtyData, (BYTE)sendValue);
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
	setWriteDB.AddToByteArray( (WORD)lMonsterNum);
	if(byMonsterDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byMonsterDirtyData[0],(long)byMonsterDirtyData.size());
	m_mapDirtyAttrData.clear();
}

//��ӱ�������������
void  CMonster::AddAttributeToDirty(string attrName)
{
	if(attrName == "")	return;
	if( (attrName[0] == 'E' && attrName[1] == '_')
		|| (attrName[0] == 'B' && attrName[1] == '_') )
		attrName[0] = 'C';

	if(m_mapOtherUpdateDirtyAttrData.find(attrName) !=  m_mapOtherUpdateDirtyAttrData.end())
		m_mapDirtyAttrData.insert(attrName);
}

bool  CMonster::SetAttribute(std::string& attrName, long value)
{
	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if(!bFlag)
	{
		if(SetValue(attrName.c_str(), value) != INVILID_VALUE)
		{
			bFlag = true;
		}
	}
	// ���������ݱ��	
	if(bFlag)
		AddAttributeToDirty(attrName);
	return bFlag;
}
bool  CMonster::SetAttribute(char* attrName, long value)
{
	if(NULL == attrName)
		return false;

	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if( !bFlag)
	{
		// ���������ݱ��
		if( SetValue(attrName, value) != INVILID_VALUE )
		{		
			bFlag =  true;
		}
	}
	if(bFlag)
		AddAttributeToDirty(string(attrName));
	return bFlag;
}
long CMonster::GetAttribute(std::string attrName)
{
	long value = INVILID_VALUE;

	value = CMoveShape::GetAttribute(attrName);
	if(value != INVILID_VALUE)
		return value;

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
			map<string, tagPty>::iterator vMonsterItr = m_mapNameValue.find(attrName);
			if(vMonsterItr != m_mapNameValue.end())
			{
				tagPty& tagValue = vMonsterItr->second;
				value = GetCurrentValue(tagValue, evalue, bvalue);
			}
		}
		else
		{
			tagPty& tagValue = vMoveShapeItr->second;
			value = GetCurrentValue(tagValue, evalue, bvalue);
		}
	}
	else
		value = GetValue(attrName.c_str());

	return value;
}
// ȡ�õ�ǰֵ�ĺϷ�ֵ
long CMonster::GetCurrentValue(const tagPty& tagValue, long evalue/*�ı�ֵ*/, long bvalue/*����ֵ*/)
{
	long retValue = evalue+bvalue;
	if( retValue > tagValue.maxValue )
		retValue = tagValue.maxValue;

	if( retValue < tagValue.minValue )
		retValue = tagValue.minValue;

	return retValue;
}

bool  CMonster::ChangeAttribute(std::string& attrName, long value)
{

	bool bFlag = CMoveShape::ChangeAttribute(attrName, value);
	if(!bFlag)
	{
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
long  CMonster::UpdateAttributeToClient()
{
	if( m_mapDirtyAttrData.size() == 0 )
		return false;

	CMessage msg( MSG_S2C_SHAPE_CHANGE_PROPERTIES );
	msg.Add(GetType());
	msg.Add(GetExID());
	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);
	CodeChangeAttrToDataBlock(setWriteDB);
	// ����msg
	msg.SendToNearestPlayer(this);
	return true;
}

long  CMonster::UpdateAttribute(BOOL bImmediately)
{
	RegisterUpdateAttrTimer();
	return true;
}

long  CMonster::OnUpdateAttributeTimeOut()
{
	UpdateAttributeToClient();
	return true;
}

// ��ʼ��
void CMonster::InitNameValueMap()
{
	CMoveShape::InitNameValueMap();

	m_mapNameValue.clear();
	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.SetType(4); st.pValue=&m_lType;               st.attrEnumValue= M_ATTR_TYPE;   m_mapNameValue["Type"] = st;		
	st.SetType(4); st.pValue=&m_lGraphicsID;	  st.attrEnumValue=M_ATTR_GRAPHICSID;   m_mapNameValue["GraphicsID"]=st;	
	st.SetType(4); st.pValue=&m_lRegionID;		    st.attrEnumValue=M_ATTR_REGIONID;   m_mapNameValue["RegionID"]=st;
	st.SetType(3);	st.pfValue=&m_fPosX;		       st.attrEnumValue=M_ATTR_TILEX;   m_mapNameValue["TileX"]=st;			
	st.SetType(3);	st.pfValue=&m_fPosY;		       st.attrEnumValue=M_ATTR_TILEY;   m_mapNameValue["TileY"]=st;	
	st.SetType(3);	st.pfValue=&m_fDir;			         st.attrEnumValue=M_ATTR_DIR;   m_mapNameValue["Dir"]=st;			
	st.SetType(4);	st.pValue=&m_lPos;			         st.attrEnumValue=M_ATTR_POS;   m_mapNameValue["Pos"]=st;			
	st.SetType(1);	st.pwValue=&m_wState;		      st.attrEnumValue=M_ATTR_STATE;	m_mapNameValue["State"]=st;			
	st.SetType(1);	st.pwValue=&m_wAction;		      st.attrEnumValue=M_ATTR_ACTION;	m_mapNameValue["Action"]=st;		


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*��������*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.SetType(2);	st.pdwValue = &m_tgMonsterProperty.dwIndex;                 	    st.attrEnumValue=M_ATTR_INDEX;			m_mapNameValue["Index"]=st;		
	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwPicLevel;              		st.attrEnumValue=M_ATTR_PICLEVEL;		m_mapNameValue["PicLevel"]=st;			   
	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwNameColor;             		st.attrEnumValue=M_ATTR_NAMECOLOR;		m_mapNameValue["NameColor"]=st;			

	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwHpBarColor;            		st.attrEnumValue=M_ATTR_HPBARCOLOR;		m_mapNameValue["HpBarColor"]=st;		
	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwSoundID;               		st.attrEnumValue=M_ATTR_SOUNDID;		m_mapNameValue["SoundID"]=st;			
	st.SetType(1);	st.pwValue  = &m_tgMonsterProperty.wLevel; 										st.attrEnumValue=M_ATTR_LEVEL;			m_mapNameValue["Level"]=st;							
	st.SetType(2); st.pdwValue  = &m_tgMonsterProperty.dwExp;                      st.attrEnumValue=M_ATTR_EXP;				 m_mapNameValue["Exp"]=st;				
	st.SetType(2); st.pdwValue  = &m_tgMonsterProperty.dwOccuPoint;                      st.attrEnumValue=M_ATTR_OCCUPOINT;	m_mapNameValue["OccuPoint"]=st;				
	st.SetType(0);	st.plValue	= &m_tgMonsterProperty.byCountry;                   st.attrEnumValue=M_ATTR_COUNTRY;			 m_mapNameValue["byCountry"]=st;			
	st.SetType(0);	st.plValue	= &m_tgMonsterProperty.byCatchable;                   st.attrEnumValue=M_ATTR_TAMABLE;			 m_mapNameValue["byCatchable"]=st;			
	st.SetType(2); st.pdwValue	= &m_tgMonsterProperty.dwMaxCatchFastness;          	st.attrEnumValue=M_ATTR_MAXTAMECOUNT;		 m_mapNameValue["MaxTameCount"]=st;		

	st.SetType(1); st.pwValue	= &m_tgMonsterProperty.wCountryContriValue;						st.attrEnumValue=M_ATTR_COUNTRYCONTRIVALUE;	 m_mapNameValue["CountryContriValue"]=st;
	st.SetType(1);	st.pwValue = &m_tgMonsterProperty.wSPContriValue;							st.attrEnumValue=M_ATTR_SPCONTRIVALUE;		 m_mapNameValue["SPContriValue"]=st;		
	st.SetType(2);	st.pdwValue  = (DWORD*)&m_tgMonsterProperty.eEType;          st.attrEnumValue=M_ATTR_ELETYPE_;			 m_mapNameValue["EleType"]=st;	        
	st.SetType(2);	st.pdwValue = (DWORD*)&m_tgMonsterProperty.eNType;           st.attrEnumValue=M_ATTR_NPCTYPE_;			 m_mapNameValue["NpcType"]=st;         

	st.SetType(1);	st.pwValue = &m_tgMonsterProperty.wBossLevel;	 m_mapNameValue["BossLevel"]=st;		
	

	st.SetType(2);	st.pdwValue = (DWORD*)&m_tgMonsterProperty.eClass_;           st.attrEnumValue=M_ATTR_CLASS;			 m_mapNameValue["Class"]=st;         
	//st.SetType(2);	st.pwValue = &m_tgMonsterProperty.eClass_;                   	st.attrEnumValue=M_ATTR_CLASS;				 m_mapNameValue["Class"]=st;				

	st.SetType(1);	st.pwValue = &m_tgMonsterProperty.wRace;                        st.attrEnumValue=M_ATTR_RACE;				 m_mapNameValue["Race"]=st;	            
	st.SetType(1); st.pwValue  = &m_tgMonsterProperty.wMoveRandomValue;        	st.attrEnumValue=M_ATTR_MOVERANDOMVALUE;	 m_mapNameValue["MoveRandomValue"]=st;	
	st.SetType(1); st.pwValue = &m_tgMonsterProperty.wStopTime;                    st.attrEnumValue=M_ATTR_STOPTIME;			 m_mapNameValue["StopTime"]=st;	        
	st.SetType(1); st.pwValue  = &m_tgMonsterProperty.wAIHangupType; 	        	st.attrEnumValue=M_ATTR_AIHANGUPTYPE;			 m_mapNameValue["AIHangupType"]=st;		
	st.SetType(1); st.pwValue = &m_tgMonsterProperty.wFigure;                      st.attrEnumValue=M_ATTR_FIGURE;				 m_mapNameValue["Figure"]=st;	        

	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwHp;                    	st.attrEnumValue=M_ATTR_HP;					 m_mapNameValue["Hp"]=st;				
	st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIType;                	st.attrEnumValue=M_ATTR_AITYPE;				 m_mapNameValue["AIType"]=st;			

	st.SetType(3);	st.pfValue = &m_tgMonsterProperty.fHurtProportion;          	st.attrEnumValue=M_ATTR_HURTPROPORTION;		 m_mapNameValue["fHurtProportion"]=st;	
	st.SetType(2); st.pdwValue = &m_tgMonsterProperty.dwGuardTime;             	st.attrEnumValue=M_ATTR_GUARDTIME;			 m_mapNameValue["GuardTime"]=st;			
	st.SetType(3); st.pfValue = &m_tgMonsterProperty.fEscapePoint;             	st.attrEnumValue=M_ATTR_ESCAPEPOINT;		 m_mapNameValue["fEscapePoint"]=st;		
	st.SetType(1); st.pwValue = &m_tgMonsterProperty.wSkillProportion;         	st.attrEnumValue=M_ATTR_SKILLPROPORTION;	 m_mapNameValue["SkillProportion"]=st;		
	st.SetType(2); st.pdwValue = &m_tgMonsterProperty.dwSkillSpace;            	st.attrEnumValue=M_ATTR_SKILLSPACE;			 m_mapNameValue["SkillSpace"]=st;		
	st.SetType(3);	st.pfValue	= &m_tgMonsterProperty.fMoveSpeed;				st.attrEnumValue=M_ATTR_MOVESPEED;			m_mapNameValue["MoveSpeed"]=st;		
	st.SetType(3);	st.pfValue	= &m_tgMonsterProperty.fFightMoveSpeed;				st.attrEnumValue=M_ATTR_FIGHTMOVESPEED;			m_mapNameValue["FightMoveSpeed"]=st;		

	//for(i=0; i<MONSTER_AI_REFERRENCE_NUM; ++i)
	{
		st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[0]; st.attrEnumValue=M_ATTR_AIREFERRENCE0; m_mapNameValue["AIReferrence0"]=st; 
		st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[1]; st.attrEnumValue=M_ATTR_AIREFERRENCE1; m_mapNameValue["AIReferrence1"]=st; 
		st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE2; m_mapNameValue["AIReferrence2"]=st; 
		st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[3]; st.attrEnumValue=M_ATTR_AIREFERRENCE3; m_mapNameValue["AIReferrence3"]=st; 
		st.SetType(2);	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[4]; st.attrEnumValue=M_ATTR_AIREFERRENCE4; m_mapNameValue["AIReferrence4"]=st; 
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*ս������*/ // B_:����ֵ, C_:��ǰֵ, E_:�仯ֵ
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.SetType(4);	st.pValue	= &m_tgMonsterFightProperty.dwMaxHp;				st.attrEnumValue=M_ATTR_MAXHP;				m_mapNameValue["E_MaxHp"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wHpRecoverSpeed;		st.attrEnumValue=M_ATTR_HPRECOVERSPEED;		m_mapNameValue["E_HpRecoverSpeed"]=st;	
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMinAtk;				st.attrEnumValue=M_ATTR_MINATK;				m_mapNameValue["E_MinAtk"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMaxAtk;				st.attrEnumValue=M_ATTR_MAXATK;				m_mapNameValue["E_MaxAtk"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMinMAtk;				st.attrEnumValue=M_ATTR_MINMATK;			m_mapNameValue["E_MinMAtk"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMaxMAtk;				st.attrEnumValue=M_ATTR_MAXMATK;			m_mapNameValue["E_MaxMAtk"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wHit;					st.attrEnumValue=M_ATTR_HIT;				m_mapNameValue["E_Hit"]=st;				
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wDef;					st.attrEnumValue=M_ATTR_DEF;				m_mapNameValue["E_Def"]=st;		
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMDef;					st.attrEnumValue=M_ATTR_MDEF;				m_mapNameValue["E_MDef"]=st;		
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wDodge;					st.attrEnumValue=M_ATTR_DODGE;				m_mapNameValue["E_Dodge"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wBlock;					st.attrEnumValue=M_ATTR_BLOCK;				m_mapNameValue["E_Block"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wParry;					st.attrEnumValue=M_ATTR_PARRY;				m_mapNameValue["E_Parry"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wImmunity;					st.attrEnumValue=M_ATTR_IMMUNITY;				m_mapNameValue["E_Immunity"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wPierce;					st.attrEnumValue=M_ATTR_PIERCE;				m_mapNameValue["E_Pierce"]=st;			
	//	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wAtcSpeed;				st.attrEnumValue=M_ATTR_ATCSPEED;			m_mapNameValue["E_AtcSpeed"]=st;		

	st.SetType(DisTypeID);	st.MemP	= &m_tgMonsterFightProperty.FarAtkDis;				st.attrEnumValue=M_ATTR_FARATKDIS;			m_mapNameValue["E_FarAtkDis"]=st;		
	st.SetType(DisTypeID);	st.MemP	= &m_tgMonsterFightProperty.NeaAtkDis;				st.attrEnumValue=M_ATTR_NEAATKDIS;			m_mapNameValue["E_NeaAtkDis"]=st;		
	//	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wAtkRange;				st.attrEnumValue=M_ATTR_ATKRANGE;			m_mapNameValue["E_AtkRange"]=st;		
	st.SetType(DisTypeID);	st.MemP	= &m_tgMonsterFightProperty.GuardRange;			st.attrEnumValue=M_ATTR_GUARDRANGE;			m_mapNameValue["E_GuardRange"]=st;		
	st.SetType(DisTypeID);	st.MemP	= &m_tgMonsterFightProperty.ChaseRange;			st.attrEnumValue=M_ATTR_CHASERANGE;			m_mapNameValue["E_ChaseRange"]=st;		
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wWillDef;				st.attrEnumValue=M_ATTR_WILLDEF;			m_mapNameValue["E_WillDef"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wConstiDef;				st.attrEnumValue=M_ATTR_CONSTIDEF;			m_mapNameValue["E_ConstiDef"]=st;		
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wCri;					st.attrEnumValue=M_ATTR_CRI;				m_mapNameValue["E_Cri"]=st;				
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wCriDef;				st.attrEnumValue=M_ATTR_CRIDEF;				m_mapNameValue["E_CriDef"]=st;			
	st.SetType(5);	st.psValue	= &m_tgMonsterFightProperty.wMCriDef;				st.attrEnumValue=M_ATTR_MCRIDEF;			m_mapNameValue["E_MCriDef"]=st;			
	//st.SetType(1);	st.psValue	= &m_tgMonsterProperty.wCanTalk;				st.attrEnumValue=M_ATTR_CANTALK;			m_mapNameValue["E_CanTalk"]=st;			
	st.SetType(1); st.pwValue = &m_tgMonsterProperty.wCanTalk;                      st.attrEnumValue=M_ATTR_CANTALK;				 m_mapNameValue["CanTalk"]=st;	        


	{
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DFT_Bled];		st.attrEnumValue=M_ATTR_DEBUFFDEF_BLED;     m_mapNameValue["E_DeBuffDef0"]=st;      
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Dizzy];	st.attrEnumValue=M_ATTR_DEBUFFDEF_DIZZY;	m_mapNameValue["E_DeBuffDef1"]=st;	    
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Lull];		st.attrEnumValue=M_ATTR_DEBUFFDEF_LULL;		m_mapNameValue["E_DeBuffDef2"]=st;	    
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Hypnosis];	st.attrEnumValue=M_ATTR_DEBUFFDEF_HYPNOSIS;	m_mapNameValue["E_DeBuffDef3"]=st;  
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Poison];	st.attrEnumValue=M_ATTR_DEBUFFDEF_POSION;	m_mapNameValue["E_DeBuffDef4"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Fix];		st.attrEnumValue=M_ATTR_DEBUFFDEF_FIX;		m_mapNameValue["E_DeBuffDef5"]=st;	    
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wDeBuffDef[DET_Silence];		st.attrEnumValue=M_ATTR_DEBUFFDEF_SILENCE;		m_mapNameValue["E_DeBuffDef6"]=st;	    
	}																														   

	//for(i=0; i<ET_Max; ++i)
	{
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Fire];		st.attrEnumValue=M_ATTR_ELEMDEF_FIRE;   m_mapNameValue["E_ElemDef0"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Water];		st.attrEnumValue=M_ATTR_ELEMDEF_WATER;	m_mapNameValue["E_ElemDef1"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Earth];		st.attrEnumValue=M_ATTR_ELEMDEF_EARTH;	m_mapNameValue["E_ElemDef2"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Wind];		st.attrEnumValue=M_ATTR_ELEMDEF_WIND;	m_mapNameValue["E_ElemDef3"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Dark];		st.attrEnumValue=M_ATTR_ELEMDEF_DARK;	m_mapNameValue["E_ElemDef4"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDef[ET_Holy];		st.attrEnumValue=M_ATTR_ELEMDEF_HOLY;	m_mapNameValue["E_ElemDef5"]=st;	
	}

	//for(i=0; i<C_Max; ++i)
	{
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Human];		st.attrEnumValue=M_ATTR_CLASSDAM_HUMAN;    m_mapNameValue["E_ClassDam0"]=st;		
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Element];		st.attrEnumValue=M_ATTR_CLASSDAM_ELEMENT;  m_mapNameValue["E_ClassDam1"]=st;	
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Alien];		st.attrEnumValue=M_ATTR_CLASSDAM_ALIEN;	   m_mapNameValue["E_ClassDam2"]=st;		
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Animal];		st.attrEnumValue=M_ATTR_CLASSDAM_ANIMAL;   m_mapNameValue["E_ClassDam3"]=st;		
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Undead];		st.attrEnumValue=M_ATTR_CLASSDAM_UNDEAD;   m_mapNameValue["E_ClassDam4"]=st;		
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Evil];		st.attrEnumValue=M_ATTR_CLASSDAM_EVIL;	   m_mapNameValue["E_ClassDam5"]=st;		
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Hexapods];	st.attrEnumValue=M_ATTR_CLASSDAM_HEXAPODS; m_mapNameValue["E_ClassDam6"]=st;	
	}																														  

	//for(i=0; i<ET_Max; ++i)
	{
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Fire];		st.attrEnumValue=M_ATTR_ELEMDAM_FIRE;   m_mapNameValue["E_ElemDam0"]=st;  
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Water];		st.attrEnumValue=M_ATTR_ELEMDAM_WATER;  m_mapNameValue["E_ElemDam1"]=st; 
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Earth];		st.attrEnumValue=M_ATTR_ELEMDAM_EARTH;  m_mapNameValue["E_ElemDam2"]=st; 
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Wind];		st.attrEnumValue=M_ATTR_ELEMDAM_WIND;   m_mapNameValue["E_ElemDam3"]=st;  
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Dark];		st.attrEnumValue=M_ATTR_ELEMDAM_DARK;   m_mapNameValue["E_ElemDam4"]=st;  
		st.SetType(5);st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Holy];		st.attrEnumValue=M_ATTR_ELEMDAM_HOLY;   m_mapNameValue["E_ElemDam5"]=st;  

	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*����ս������*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.SetType(4);	st.pValue	= &m_tgBaseMonsterFightProperty.dwMaxHp;				m_mapNameValue["B_MaxHp"]=st;				//��������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wHpRecoverSpeed;				m_mapNameValue["B_HpRecoverSpeed"]=st;				//��������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMinAtk;			m_mapNameValue["B_MinAtk"]=st;			//��������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMaxAtk;			m_mapNameValue["B_MaxAtk"]=st;			//��������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMinMAtk;		m_mapNameValue["B_MinMAtk"]=st;		//�����ָ��ٶ�			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMaxMAtk;		m_mapNameValue["B_MaxMAtk"]=st;		//�����ָ��ٶ�			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wHit;		m_mapNameValue["B_Hit"]=st;		//�����ָ��ٶ�			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wDef;				m_mapNameValue["B_Def"]=st;
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMDef;				m_mapNameValue["B_MDef"]=st;	//ħ������
	//����			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wDodge;				m_mapNameValue["B_Dodge"]=st;				//����			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wBlock;				m_mapNameValue["B_Block"]=st;				//����		
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wParry;				m_mapNameValue["B_Parry"]=st;				//����
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wImmunity;			m_mapNameValue["B_Immunity"]=st;			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wPierce;			m_mapNameValue["B_Pierce"]=st;			

	st.SetType(DisTypeID);	st.MemP	= &m_tgBaseMonsterFightProperty.FarAtkDis;				m_mapNameValue["B_FarAtkDis"]=st;				//�м�			
	st.SetType(DisTypeID);	st.MemP	= &m_tgBaseMonsterFightProperty.NeaAtkDis;				m_mapNameValue["B_NeaAtkDis"]=st;				//����		
	//st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wAtkRange;				m_mapNameValue["B_AtkRange"]=st;				//����
	st.SetType(DisTypeID);	st.MemP	= &m_tgBaseMonsterFightProperty.GuardRange;					m_mapNameValue["B_GuardRange"]=st;					//����
	st.SetType(DisTypeID);	st.MemP	= &m_tgBaseMonsterFightProperty.ChaseRange;				m_mapNameValue["B_ChaseRange"]=st;				//רע			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wWillDef;					m_mapNameValue["B_WillDef"]=st;					//�������	
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wConstiDef;					m_mapNameValue["B_ConstiDef"]=st;					//ħ������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wCri;					m_mapNameValue["B_Cri"]=st;					//������			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wCriDef;			m_mapNameValue["B_CriDef"]=st;			//ʩ���ٶ�			
	st.SetType(5);	st.psValue	= &m_tgBaseMonsterFightProperty.wMCriDef;			m_mapNameValue["B_MCriDef"]=st;			//Buff����ǿ��		

	{
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DFT_Bled];	m_mapNameValue["B_DeBuffDef0"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Dizzy];	m_mapNameValue["B_DeBuffDef1"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Lull];	m_mapNameValue["B_DeBuffDef2"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Hypnosis];	m_mapNameValue["B_DeBuffDef3"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Poison];	m_mapNameValue["B_DeBuffDef4"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Fix];	m_mapNameValue["B_DeBuffDef5"]=st;			//Debuff����
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wDeBuffDef[DET_Silence];	m_mapNameValue["B_DeBuffDef6"]=st;			//Debuff����
	}

	{
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Fire];		m_mapNameValue["B_ElemDef0"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Water];		m_mapNameValue["B_ElemDef1"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Earth];		m_mapNameValue["B_ElemDef2"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Wind];		m_mapNameValue["B_ElemDef3"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Dark];		m_mapNameValue["B_ElemDef4"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDef[ET_Holy];		m_mapNameValue["B_ElemDef5"]=st; //����Ԫ�ؿ���
	}

	{
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Human];		m_mapNameValue["B_ClassDam0"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Element];		m_mapNameValue["B_ClassDam1"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Alien];		m_mapNameValue["B_ClassDam2"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Animal];		m_mapNameValue["B_ClassDam3"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Undead];		m_mapNameValue["B_ClassDam4"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Evil];		m_mapNameValue["B_ClassDam5"]=st;
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Hexapods];	m_mapNameValue["B_ClassDam6"]=st;;//�Ը��������˺�
	}

	{
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Fire];		m_mapNameValue["B_ElemDam0"]=st;			//�Ը���Ԫ���˺�
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Water];		m_mapNameValue["B_ElemDam1"]=st;			//�Ը���Ԫ���˺�
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Earth];		m_mapNameValue["B_ElemDam2"]=st;			//�Ը���Ԫ���˺�
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Wind];		m_mapNameValue["B_ElemDam3"]=st;			//�Ը���Ԫ���˺�
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Dark];		m_mapNameValue["B_ElemDam4"]=st;			//�Ը���Ԫ���˺�
		st.SetType(5);st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Holy];		m_mapNameValue["B_ElemDam5"]=st;			//�Ը���Ԫ���˺�

	}
	{
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Attack];		m_mapNameValue["Earth_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Defend];		m_mapNameValue["Earth_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Attack];		m_mapNameValue["Water_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Defend];		m_mapNameValue["Water_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Attack];		m_mapNameValue["Fire_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Defend];		m_mapNameValue["Fire_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Attack];		m_mapNameValue["Wind_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Defend];		m_mapNameValue["Wind_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Attack];		m_mapNameValue["Bright_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Defend];		m_mapNameValue["Bright_Defend"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Attack];		m_mapNameValue["Dark_Attack"]=st;
		st.SetType(4);st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Defend];		m_mapNameValue["Dark_Defend"]=st;
	}
}


void CMonster::InitUpdateDirtyAttrData()
{
	// ��ʼ��֪ͨ��simple����
	m_mapOtherUpdateDirtyAttrData.insert("Hp");
	m_mapOtherUpdateDirtyAttrData.insert("C_MaxHp");

	m_mapOtherUpdateDirtyAttrData.insert("dwCollectionID");
	m_mapOtherUpdateDirtyAttrData.insert("dwCollectionTimes");
	m_mapOtherUpdateDirtyAttrData.insert("bIsBeenCollect");
	m_mapOtherUpdateDirtyAttrData.insert("bCanBeenCollect");
}

long CMonster::GetValue(const char* strName)
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

long CMonster::SetValue(const char* strName, long dwValue)
{
	long value = INVILID_VALUE;
	value = CMoveShape::SetValue(strName, dwValue);
	if(INVILID_VALUE != value)
		return value;

	mapNameValueItr itr = m_mapNameValue.begin();
	itr = m_mapNameValue.find(strName);
	if (itr != m_mapNameValue.end())
	{
		if(dwValue < itr->second.minValue)
			dwValue = itr->second.minValue;
		if(dwValue > itr->second.maxValue)
			dwValue = itr->second.maxValue;

		switch (itr->second.lType)
		{
		case 0:	// byte
			*(itr->second.plValue) = (BYTE)dwValue;
			return dwValue;

		case 1:	// word
			*(itr->second.pwValue) = (WORD)dwValue;
			return dwValue;

		case 2:	// dword
			*(itr->second.pdwValue) = (DWORD)dwValue;
			return dwValue;

		case 3:	// float
			*(itr->second.pfValue) = (float)dwValue;
			return dwValue;

		case 4:	// long
			*(itr->second.pValue) = (long)dwValue;
			return dwValue;

		case 5:	// short
			*(itr->second.psValue) = (short)dwValue;
			return dwValue;

		case 6:	// bool
			*(itr->second.pbValue) = dwValue==0?false:true;
			return dwValue;
		}
	}
	return value;
}

void CMonster::SetPosXY(float fX,float fY )
{
	long lOldX = GetTileX();
	long lOldY = GetTileY();
	long lNewTileX = static_cast<long>(fX);
	long lNewTileY = static_cast<long>(fY);
	CMoveShape::SetPosXY(fX,fY );
	//ȡ�����赲
	SetBlock(lOldX,lOldY,CRegion::BLOCK_NO);
	//���ù����赲
	if( !IsDied())
		SetBlock(lNewTileX,lNewTileY,CRegion::BLOCK_AIM);
}

// 3.10.2009 �������ƶ������н�����������
void CMonster::SetBlock(int x, int y, unsigned short block)
{
	assert(GetFather());
	BYTE byShapeBlockType=GetBlockType();
	if (byShapeBlockType==CRegion::BLOCK_NO ) return;//|| byShapeBlockType==CRegion::BLOCK_SHAPE)	return;
	if (byShapeBlockType==CRegion::BLOCK_CANFLY || byShapeBlockType== CRegion::BLOCK_UNFLY)	return;

	switch(block) 
	{
	case CRegion::BLOCK_AIM:
		{
			CRegion::tagCell* pCell = ((CRegion*)GetFather())->GetCell(x,y);
			if( pCell && pCell->bBlock == CRegion::BLOCK_NO )
			{
				pCell->bBlock = byShapeBlockType;//CRegion::BLOCK_AIM;
			}
		}
		break;

	case CRegion::BLOCK_NO:
		{
			CRegion::tagCell* pCell = ((CRegion*)GetFather())->GetCell(x,y);
			if( pCell && pCell->bBlock == byShapeBlockType)
			{
				pCell->bBlock = CRegion::BLOCK_NO;
			}
		}
		break;
	}
}

//��������ս����Ϣ
void CMonster::SendLeaveFightMessage(long lAimX,long lAimY,long lTime)
{
	CMessage msg(MSG_S2C_SHAPE_LEAVEFIGHT);
	msg.Add(GetType());
	msg.Add(GetExID());
	msg.Add(lAimX);
	msg.Add(lAimY);
	msg.Add(lTime);
	msg.SendToAround(this);
}
void CMonster::SendLeaveFightEndMessage()
{
	CMessage msg(MSG_S2C_SHAPE_LEAVEFIGHTEND);
	msg.SendToAround(this);
}

long CMonster::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	DWORD dwStartTime = timeGetTime();
	long lTimerType = 600;
	const tagTimerVar* pTimerVar = (const tagTimerVar*)var;
	if(pTimerVar)
		lTimerType =pTimerVar ->lvar1<<16 | pTimerVar->TimerType;
#ifdef _RUNSTACKINFO1_
	char pszStatckInfo[256]="";
	_snprintf(pszStatckInfo,256,"CMonster::OnTimeOut(TimerType:%d,Name:%s) Start",pTimerVar?pTimerVar->TimerType:0,GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

	CMoveShape::OnTimeOut(timerid,curtime,var);

	GetGame()->AddTimerExpendTime(lTimerType,timeGetTime()-dwStartTime);
#ifdef _RUNSTACKINFO1_
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),"CMonster::OnTimeOut() End");
#endif
	return 0;
}

//##��Ӷ�Ѫ��
void CMonster::AddHpBar(int HpRange, const char* ScripntName)
{
	m_MultiHpBar.SetBaseMaxHp(GetMaxHP());
	m_MultiHpBar.AddHpBar(HpRange,ScripntName);
	SetBaseMaxHp(GetMaxHP()+HpRange);
	SetHP(GetMaxHP());
}

int  CMonster::GetHpLevel(int Hp)
{
	return m_MultiHpBar.GetCurrentInfo(Hp).Level;
}

bool CMonster::OnHpChanged(int Hp, int ChangeHp)
{
	const char* ScriptName=m_MultiHpBar.GetCurrentInfo(Hp,ChangeHp).ScriptName;

	if (ScriptName!= NULL)
	{
		//stRunScript st;
		//st.pszFileName   = (char*)(ScriptName);
		//st.srcShape  = this;
		//st.desShape	 = this;
		//st.pRegion   = dynamic_cast<CRegion*>(GetFather());
		//RunScript(&st,(char*)GetGame()->GetScriptFileData(st.pszFileName));	
		Script::RunEnv env( this, this, static_cast<CRegion*>( GetFather() ) );
		GetInst( ScriptSys ).RunScript( env, ScriptName );
		return 0;
	}
	return false;
}

void CMonster::Init()
{
	InitSkills();//���ܳ�ʼ
	InitAI();	//AI��ʼ
	//m_pAI->Resume( 0 );
	//if (m_pAI!=NULL)
	//{
	//	m_pAI->OnBoring();
	//}
}

// ��ӵ�CByteArray
bool CMonster::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
	//��������
	CMoveShape::CodeToDataBlock_ForClient( setWriteDB, bExData );
	setWriteDB.AddToByteArray(GetHP());
	setWriteDB.AddToByteArray(GetMaxHP());
	setWriteDB.AddToByteArray((WORD)GetClass());
	setWriteDB.AddToByteArray((WORD)GetFigure());
	setWriteDB.AddToByteArray((DWORD)GetSounID());
	setWriteDB.AddToByteArray((DWORD)GetNameColor());
	setWriteDB.AddToByteArray((DWORD)GetHpBarColor());
	setWriteDB.AddToByteArray((WORD)GetLevel());
	setWriteDB.AddToByteArray(m_dwCollectionID);							//�ɼ���id
	setWriteDB.AddToByteArray(static_cast<BYTE>(m_bIsBeenCollect));			//�Ƿ����ڱ��ɼ�
	setWriteDB.AddToByteArray(static_cast<BYTE>(m_bCanBeenCollect));		//�ܷ񱻲ɼ�

	setWriteDB.AddToByteArray(GetOriginName());
	setWriteDB.AddToByteArray((WORD)GetNpcType());
	setWriteDB.AddToByteArray(GetCanTalk());
	setWriteDB.AddToByteArray(GetTitlePicID());

	setWriteDB.AddToByteArray(GetTitleColor());
	setWriteDB.AddToByteArray(GetTitleName());
	setWriteDB.AddToByteArray(GetBlockType());
	//���汣��
	setWriteDB.AddToByteArray((long)GetBeneficiaryType());//long
	setWriteDB.AddToByteArray(GetBeneficiaryID());//CGUID
	//����id
	setWriteDB.AddToByteArray(GetCountry());//byte
	//ת���ٶ�
	setWriteDB.AddToByteArray(GetTurningSpeed());
	//boss ����
	setWriteDB.AddToByteArray(GetBossLevel());
	if(GetBossLevel() != 0 && GetBossLevel() != 1)
	{
	m_MultiHpBar.CodeToDataBlock(setWriteDB);
	}

	//CodeChangeAttrToDataBlock(setWriteDB);
	return true;
}

// ����
bool CMonster::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);
	SetHP(setReadDB.GetDwordFromByteArray());
	return true;
}

void CMonster::InitCycleSkill()
{
	for ( long a=0 ; a< (long)m_vCycleSkills.size(); a++)
	{
		m_vCycleSkills[a].dwCoolDownTime=timeGetTime();
	}
}

// //�Ƿ����ѱ�� �����ӿ�
bool CMonster::IsCatchable()
{
	bool bResult = false;

	tagMonsterProperty *pProperties = &m_tgMonsterProperty;

	if (pProperties && pProperties->byCatchable == 1 
		&& !GetCatchFlag()
		/*&& m_dwTameAttemptCount < pProperties -> dwMaxCatchFastness*/
		)
	{
		bResult = true;
	}
	return bResult;
}

//void CMonster::IncreaseTameAttemptCount()
//{
//	++ m_dwTameAttemptCount;
//}

// ���¹���Ĳ�����
void CMonster::DecreaseCatchFastness(int catchFastness)
{
	if ((int)m_dwCatchFastness <= catchFastness)
	{
		m_dwCatchFastness = 0;
	}
	else
		m_dwCatchFastness -= catchFastness;
}


void CMonster::AddSkill(CMonsterList::tagSkillInfo& info,SKILLTYPE skilltype )
{
    if( !GetInst(SkillAttribute).IsExist(info.dwSkillID,info.dwSkillLevel) )
        return ;

	tagSkillRecord tgSklll;

    tgSklll.wID = (WORD)info.dwSkillID;
	tgSklll.wLevel = (WORD)info.dwSkillLevel;
	tgSklll.iActModify=info.iActModifyTime;
	tgSklll.dwCoolDownTime=0;

	if (skilltype== SKILLTYPE_CYCLE)
	{
		tgSklll.dwCool=info.dwCoolDownTime;

	}
	else
	{
        
		if( skilltype == SKILLTYPE_BASE )
		{
			tgSklll.dwCool = ( info.dwCoolDownTime == 0 ? GetSkillValue(info.dwSkillID,info.dwSkillLevel,"CoolDown") :
				info.dwCoolDownTime );
		}
		else
		{
			tgSklll.dwCool = GetSkillValue(info.dwSkillID,info.dwSkillLevel,"CoolDown");
		}
        
	}

	if (skilltype== SKILLTYPE_BASE) 
		m_vBaseSkills.push_back(tgSklll);
	else if (skilltype== SKILLTYPE_NORMAL) 
		m_vSkills.push_back(tgSklll);
	else if (skilltype== SKILLTYPE_CYCLE) 
		m_vCycleSkills.push_back(tgSklll);

#ifdef MONSTER_DIS_FLOAT
    CountAtkRange((float)GetSkillValue(info.dwSkillID,info.dwSkillLevel,"MaxAtkDistance"),
                  (float)GetSkillValue(info.dwSkillID,info.dwSkillLevel,"MinAtkDistance"));
#else
	CountAtkRange((short)GetSkillValue(info.dwSkillID,info.dwSkillLevel,"MaxAtkDistance"),
				  ((short))GetSkillValue(info.dwSkillID,info.dwSkillLevel,"MinAtkDistance"));
#endif
}

bool CMonster::DelSkill(long id)
{
	if (DelSkillOnList(m_vBaseSkills,id) || DelSkillOnList(m_vSkills,id) || DelSkillOnList(m_vCycleSkills ,id))
	{
		SetNearAtkDis(0);
		SetFarAtkDis(0);
		CountAtkRange(m_vBaseSkills);
		CountAtkRange(m_vSkills);
		CountAtkRange(m_vCycleSkills);
		return true;
	}
	return false;
}

void CMonster::CountAtkRange(vector<tagSkillRecord>& vSkills)
{
	for (vector<tagSkillRecord>::iterator  it= vSkills.begin();
		it!= vSkills.end(); it++)
	{
		long id= it->wID;
		long lv= it->wLevel;

#ifdef MONSTER_DIS_FLOAT
        CountAtkRange((float)GetSkillValue(id,lv,"MaxAtkDistance"),
                      (float)GetSkillValue(id,lv,"MinAtkDistance"));
#else
		CountAtkRange((short)GetSkillValue(id,lv,"MaxAtkDistance"),
			          (short)GetSkillValue(id,lv,"MinAtkDistance"));
#endif
	}
}

void CMonster::CountAtkRange(DisType ragMax,DisType ragMin)
{
	if (ragMin>=0 && ragMax>=ragMin)
	{
		DisType max=GetAtkRangeMax();
		DisType min=GetAtkRangeMin();
        if( ragMin < min ) SetNearAtkDis( ragMin );
        if( ragMax > max ) SetFarAtkDis( ragMax );
	}
}

bool CMonster::DelSkillOnList(vector<tagSkillRecord>& vSkills, long SkillID)
{
	for (vector<tagSkillRecord>::iterator it= vSkills.begin();
		it!= vSkills.end(); it++)
	{
		if (it->wID ==SkillID)
		{
			vSkills.erase(it);
			return true;
		}
	}
	return false;
}

long CMonster::GetActModify(long SkillID)
{
	for (vector<tagSkillRecord>::iterator it= m_vBaseSkills.begin();
		it!= m_vBaseSkills.end(); it++)
	{
		if (it->wID== SkillID) return it->iActModify;
	}

	for (vector<tagSkillRecord>::iterator it= m_vSkills.begin();
		it!= m_vSkills.end(); it++)
	{
		if (it->wID== SkillID) return it->iActModify;
	}

	for (vector<tagSkillRecord>::iterator it= m_vCycleSkills.begin();
		it!= m_vCycleSkills.end(); it++)
	{
		if (it->wID== SkillID) return it->iActModify;
	}

	return 0;
}

void CMonster::InitAI()
{
	//OBJ_RELEASE(CBaseAI, m_pAI);
	//m_pAI=CAIFactory::CreateAI(this);
	m_pAI = new StateAI::MonsterAI( this );

	const StateAI::ImplConfig *impl = GetInst( StateAI::MonsterAISetup ).GetImpl( (long)GetAIType() );
	if( impl != NULL )
	{
		m_pAI->Init( *impl, StateAI::ST_PEACE );
        AI_EVENT_SENDER( m_pAI ).OnBorn();
	}
}

void CMonster::InitProperty(CMonsterList::tagMonster *p)  //��ʼ����������
{
	m_strOrginName = p->cOriginalName;
	m_strTitleName = p->cTitleName;
	this->SetCollectionName(p->cCollectionName);
	this ->SetTitleColor(p->dwTitleColor);
	this ->SetTitlePicID(p->dwTitlePicID);
	this ->SetIndex(p->dwIndex );
	this ->SetPicID (p->dwPicID );
	this ->SetPicLevel(p->dwPicLevel);
	this ->SetBlockType(p->byBlockType);
	this ->SetNameColor(p->dwNameColor);
	this ->SetHpBarColor(p->dwHpBarColor);
	this ->SetSounID(p->dwSoundID);
	this ->SetLevel(p->wLevel);
	this ->SetExp(p->dwExp);
	this ->SetDExp(p->dwDExp);
	this ->SetOccuPoint(p->dwOccuPoint);
	this ->SetCountry(p->byCountry );
	this ->SetCatchable(p->byCatchable);
	this ->SetMaxCatchFastness(p->dwMaxCatchFastness);
	this ->SetServantID(p->dwServantID);
	this ->SetCountryContriValue(0);///���ҹ���ֵ  ��0
	this ->SetSPContriValue (0);///SP����ֵ   �� 0
	this ->SetEleType(p->eEType);
	this ->SetNpcType(p->eNType);
	this ->SetBossLevel(p->wBossLevel);

	this ->SetClass(p->eClass_);
	this ->SetRace(0);////��Ⱥ(AIʹ��)�� 0
	this ->SetMoveRandomValue(p->wMoveRandomValue);
	this ->SetStopTime(p->wStopTime);
	this ->SetAIHangupType(p->wAIHangupType);
	this ->SetFigure(p->wFigure);//ȡ���������� 11.7.2008
	this ->SetAIType((DWORD)p->wAIModel);

	this ->SetHurtProportion( p->fHurtProportion);
	this ->SetGuardTime(p->dwGuardTime);
	this ->SetEscapePoint(p->fEscapePoint );
	this ->SetSkillProportion(p->wSkillProportion );
	this ->SetSkillSpace(p->dwSkillSpace);
	this ->SetIsFlyToPeace(p->byIsFlyToPeace);
	this ->SetIsResumeAll(p->byIsResumeAll);
	this ->SetChaseTime(p->dwChaseTime);
	this ->SetReturnRange(p->ReturnRange);
	this->SetPeaceMoveRange(p->PeaceMoveRange);

	this ->SetMinStopTimes(p->wMinStopTimes);
	this ->SetMaxStopTimes(p->wMaxStopTimes);
	this ->SetMinRunTimes(p->wMinRunTimes);
	this ->SetMaxRunTimes(p->wMaxRunTimes);
	this ->SetCanTalk(p->wCanTalk);

	int a=0;
	for(a=0; a<MONSTER_AI_REFERRENCE_NUM; a++)
	{
		this ->SetAIReferrence (a, p->dwAIReferrence [a]);
	}

	//ս����������
	this ->SetBaseMaxHp(p->dwMaxHP);
	this ->SetBaseHpRecoverSpeed(p->wHpRecoverSpeed);
	this ->SetBaseMinAtk(p->wMinAtk);
	this ->SetBaseMaxAtk(p->wMaxAtk);
	this ->SetBaseMinMAtk(p->wMinMAtk);
	this ->SetBaseMaxMAtk(p->wMaxMAtk);
	this ->SetBaseHit(p->wHit);
	this ->SetBaseDef(p->wDef);
	this ->SetBaseMDef(p->wMDef);
	this ->SetBaseDodge(p->wDodge);
	this ->SetBaseBlock(p->wBlock);
	this ->SetBaseParry(p->wParry);
	this ->SetMoveSpeed(p->fMoveSpeed);
	this ->SetFightMoveSpeed(p->fFightMoveSpeed);
	this ->SetTurningSpeed(p->fTurningSpeed);
	this ->SetBaseGuardRange(p->GuardRange);
	this ->SetBaseChaseRange(p->ChaseRange);
	this ->SetBaseWillDef(p->wWillDef);
	this ->SetBaseConstiDef(p->wBaseConstiDef);
	this->SetBaseImmunity( p->wImmunity );
	this->SetBasePierce( p->wPierce );

	this ->SetBaseCri(p->wCri);
	this ->SetBaseCriDamg(p->wCriDamg);
	this ->SetBaseCriDef(p->wCriDef);
	this ->SetBaseMCri(p->wMCri);
	this ->SetBaseMCriDamg(p->wMCriDamg);
	this ->SetBaseMCriDef(p->wMCriDef);

	for(a=0; a<DET_Max; a++)
	{
		this->SetBaseDeBuffDef (a,p->wDeBuffDef [a]);
	}
	for( a=0; a<ET_Max; a++)
	{
		this->SetBaseElemDef(a,p->wElemDef[a]);
	}
	for( a=0; a<C_Max; a++)
	{
		this->SetBaseClassDam(a,p->wClassDam[a]);
	}
	for( a=0; a<ET_Max; a++)
	{
		this->SetBaseElemDam(a,p->wElemDam[a]);
	}
	//ս������
	memset(&m_tgMonsterFightProperty,0,sizeof(tagMonsterFightProperty));

	this ->SetHP(p->dwMaxHP );     //����hp����ս�����Եĳ�ʼ��֮�� 
	this->SetSpeed(p->fMoveSpeed);

	//���ܳ�ʼ��
	SetFarAtkDis(0);
	SetNearAtkDis(0);
	m_vBaseSkills.clear();
	m_vSkills.clear();
	m_vCycleSkills.clear();
	for(a=0; a<MONSTER_SKILL_SUM && p->BaseSkillList[a].dwSkillID!=0; a++)
	{
		this->AddSkill(p->BaseSkillList[a],SKILLTYPE_BASE);
	}
	for(a=0; a<MONSTER_SKILL_SUM && p->CommonSkillList[a].dwSkillID!=0; a++)
	{
		this->AddSkill(p->CommonSkillList[a],SKILLTYPE_NORMAL);
	}
	for(a=0; a<MONSTER_SKILL_SUM && p->CycleSkillList[a].dwSkillID!=0; a++)
	{
		this->AddSkill(p->CycleSkillList[a],SKILLTYPE_CYCLE);
	}

	this->m_strBornScript=p->cBornScriptName;
	this->m_strFightScript=p->cFightScriptName;
	this->m_strGBHScript=p->cGBHScriptName;
	this->m_strDeadScript=p->cDeadScriptName;
	this->m_strCycleScript=p->cCycleScriptName;
	this->m_strSearchEnemyScript=p->cSearchEnemyScriptName;
	this->m_strFindTargetScript= p->cFindTargetScriptName;
	this->m_strKillTargetScript= p->cKillTargetScriptName;
	this->m_strCatchScript = p->cCatchScriptName;
	memcpy(this->m_arrElementProperty, p->m_arrElementProperty, sizeof(tagPlayerElementProperty) * MT_Max);
	if (p->byIsunbeatable)
	{
		God();
	}
	m_MultiHpBar.SetBaseMaxHp( p->dwMaxHP );
	int hp=m_MultiHpBar.GetMaxHp();
	if (hp!=0)
	{
		this->SetHP(hp);
	}

	// �Թ��ҽ���У��
	if( GetFather() != NULL && ( GetNpcType() == NT_Guard || GetNpcType() == NT_Normal ) && 
		GetCountry() == 8 )
	{
		CServerRegion *pRegion = static_cast<CServerRegion*>( GetFather() );
		SetCountry( pRegion->GetCountry() );
	}

	//����Ĳ�����
	ResetMaxCatchFastness();

    m_tauntHandler->Clear();
    m_tauntHandler->SetMaxSize( p->tauntSize );
}
//����˵��
void CMonster::Talk(const char* strContent)
{
	BEGIN_LOOP_AROUND_SHAPE((CServerRegion*)GetFather(),GetArea()->GetX(),GetArea()->GetY())
	{
		if( pShape->GetType() == TYPE_PLAYER )
		{
			CPlayer* p = (CPlayer*)pShape;

			// ͬ�����
			if( abs( (long)(p->GetTileX() - GetTileX()) ) < AREA_WIDTH && abs( (long)(p->GetTileY() - GetTileY()) ) < AREA_HEIGHT )
			{
				CMessage msg(MSG_S2C_OTHER_TALK);
				msg.Add((BYTE)0);
				msg.Add(long(CHAT_NPC));
				msg.Add((long)TYPE_MONSTER);
				msg.Add(GetExID());
				msg.Add((long)0);
				msg.Add(const_cast<char*>(GetName()));
				msg.Add(const_cast<char*>(strContent) );
				msg.SendToPlayer(  p->GetExID() ,false);
			}
		}
	}
	END_LOOP_AROUND_SHAPE
}

void CMonster::Talk(LONG TextType,LONG TextIndex)
{
	BEGIN_LOOP_AROUND_SHAPE((CServerRegion*)GetFather(),GetArea()->GetX(),GetArea()->GetY())
	{
		if( pShape->GetType() == TYPE_PLAYER )
		{
			CPlayer* p = (CPlayer*)pShape;

			// ͬ�����
			if( abs( (long)(p->GetTileX() - GetTileX()) ) < AREA_WIDTH && abs( (long)(p->GetTileY() - GetTileY()) ) < AREA_HEIGHT )
			{
				CMessage msg(MSG_S2C_OTHER_TALK);
				msg.Add((BYTE)1);
				msg.Add(long(CHAT_NPC));
				msg.Add((long)TYPE_MONSTER);
				msg.Add(GetExID());
				msg.Add((long)0);
				msg.Add(const_cast<char*>(GetName()));
				msg.Add(TextType);
				msg.Add(TextIndex);
				msg.SendToPlayer(  p->GetExID(),false );
			}
		}
	}
	END_LOOP_AROUND_SHAPE
}

LONG CMonster::CalculateExperienceQuota( CPlayer* pPlayer, LONG lPosX, LONG lPosY )
{
	//##��ȡ������ָ��
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return 0;

	if( pPlayer )
	{
		DWORD dwGetExp = GetExpDead();
		//##�ж��Ƿ������
		if( NULL_GUID != pPlayer -> GetTeamID() )
		{
			//##��ȡ�������
			//CGame::tagTeam* pTeam = GetGame() -> GetTeam( pPlayer -> GetTeamID() );
			//LONG lSessionID = GetGame() -> GetTeamSessionID( pPlayer -> GetTeamID() );
			//CTeam* pTeam = dynamic_cast<CTeam*>( const_cast<CSession*>( CSessionFactory::QuerySession( lSessionID ) ) );

			GSTeam* pGSTeam = NULL;
			if( NULL_GUID != pPlayer -> GetTeamID() )
			{
				pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
			}

			if( pGSTeam )
			{
				return pGSTeam->CalculateExperience(pPlayer, GetLevel(), dwGetExp, lPosX, lPosY);
			}
			else
			{
				//##��ȡ����������ϣ��д���ֱ�ӷ���0
				return 0;
			}
		}
		else
		{
			//##���û�����,��ֱ�ӷ��ع���ľ���ֵ.
			if( pPlayer -> IsDied() )	return 0;
			else 
			{
				float parm1 = (float)(pPlayer->GetLevel() - GetLevel());
				if(0 > parm1)
					parm1 = (GlobalSetup::GetSetup()->new_fExpMinLvlDiff < -parm1) ? (parm1 + GlobalSetup::GetSetup()->new_fExpMinLvlDiff) : 0;

				float fExp = (float)dwGetExp;
				float fCoefficient = (0 > parm1) ? GlobalSetup::GetSetup()->new_fExpHortation : GlobalSetup::GetSetup()->new_fExpAmerce;
				fExp *= (float)(1.0f - parm1 * fCoefficient / 100.0f);
				fExp += 0.999999f;
				return (1 >= (LONG)fExp) ? 1 : (LONG)fExp;//! ��������0
			}
		}
	}
	//##��Ҳ�����
	else return 0;
}

//����ְҵ������亯��
LONG CMonster::CalculateOccuExpQuota( CPlayer* pPlayer, LONG lPosX, LONG lPosY )
{
	//##��ȡ������ָ��
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return 0;

	if( pPlayer )
	{
		DWORD dwGetExp = GetOccuPoint();
		//##�ж��Ƿ������
		if( NULL_GUID != pPlayer -> GetTeamID() )
		{
			GSTeam* pGSTeam = NULL;
			if( NULL_GUID != pPlayer -> GetTeamID() )
			{
				pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
			}

			if( pGSTeam )
			{
				return pGSTeam->CalculateOccuExperience(pPlayer, GetLevel(), dwGetExp, lPosX, lPosY);
			}
			else
			{
				//##��ȡ����������ϣ��д���ֱ�ӷ���0
				return 0;
			}
		}
		else
		{
			//##���û�����,��ֱ�ӷ��ع���ľ���ֵ.
			if( pPlayer -> IsDied() )	return 0;
			else 
			{
				eOccupation byOccu = (eOccupation)pPlayer->GetOccupation();
				float parm1 = PlayerList::GetOccuLvlCoe(byOccu,pPlayer->GetOccuLvl(byOccu)) - GetLevel();
				if(0 > parm1)
					parm1 = (GlobalSetup::GetSetup()->vigour_fExpMinLvlDiff < -parm1) ? (parm1 + GlobalSetup::GetSetup()->vigour_fExpMinLvlDiff) : 0;	
				float fExp = (float)dwGetExp;
				float fCoefficient = (0 > parm1) ? GlobalSetup::GetSetup()->vigour_fExpHortation : GlobalSetup::GetSetup()->vigour_fExpAmerce;
				fExp *= (float)(1.0f - parm1 * fCoefficient / 100.0f);
				fExp = fExp * GlobalSetup::GetSetup()->vigour_fOccExpSpRatio;
				fExp += 0.5000f;
				return (0 >= (LONG)fExp) ? 0 : (LONG)fExp;
			}
		}
	}
	//##��Ҳ�����
	else return 0;
}


LONG CMonster::CalculateOccuPointQuota( CPlayer* pPlayer, LONG lPosX, LONG lPosY )
{
	//##��ȡ������ָ��
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return 0;

	if( pPlayer )
	{
		DWORD dwGetExp = GetOccuPoint();
		//##�ж��Ƿ������
		if( NULL_GUID != pPlayer -> GetTeamID() )
		{
			//##��ȡ�������
			//CGame::tagTeam* pTeam = GetGame() -> GetTeam( pPlayer -> GetTeamID() );
			//LONG lSessionID = GetGame() -> GetTeamSessionID( pPlayer -> GetTeamID() );
			//CTeam* pTeam = dynamic_cast<CTeam*>( const_cast<CSession*>( CSessionFactory::QuerySession( lSessionID ) ) );

			GSTeam* pGSTeam = NULL;
			if( NULL_GUID != pPlayer -> GetTeamID() )
			{
				pGSTeam = (GSTeam*)GetOrganiCtrl()->GetGameTeamByID(pPlayer -> GetTeamID());
			}

			if( pGSTeam )
			{
				return pGSTeam->CalculateOccuPoint(pPlayer, GetLevel(), dwGetExp, lPosX, lPosY);
			}
			else
			{
				//##��ȡ����������ϣ��д���ֱ�ӷ���0
				return 0;
			}
		}
		else
		{
			//##���û�����,��ֱ�ӷ��ع���ľ���ֵ.
			if( pPlayer -> IsDied() )	return 0;
			else
			{
				eOccupation byOccu = (eOccupation)pPlayer->GetOccupation();
				float parm1 = PlayerList::GetOccuLvlCoe(byOccu,pPlayer->GetOccuLvl(byOccu)) - GetLevel();
				if(0 > parm1)
					parm1 = (GlobalSetup::GetSetup()->vigour_fExpMinLvlDiff < -parm1) ? (parm1 + GlobalSetup::GetSetup()->vigour_fExpMinLvlDiff) : 0;
				float fExp = (float)dwGetExp;
				float fCoefficient = (0 > parm1) ? GlobalSetup::GetSetup()->vigour_fExpHortation : GlobalSetup::GetSetup()->vigour_fExpAmerce;
				fExp *= (float)(1.0f - parm1 * fCoefficient / 100.0f);
				fExp += 0.5000f;
				return (0 >= (LONG)fExp) ? 0 : (LONG)fExp;
			}
		}
	}
	//##��Ҳ�����
	else return 0;
}


CPlayer* CMonster::GetBeneficiary()
{
	//##��ȡ������ָ��
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return NULL;

	CPlayer* pBeneficiary=NULL;

	//##����ȷ��������һ�ε���
	if(GetBeneficiaryType()== TYPE_TEAM)
	{
		GSTeam *pGSTeam = (GSTeam *)GetOrganiCtrl()->GetGameTeamByID(GetBeneficiaryID());
		if(pGSTeam==NULL) return NULL;

		map<CGUID, CPlayer*> RegionTeamates;
		pGSTeam->GetLucrativeArea_Alive(RegionTeamates, pRegion->GetExID(), (long)GetPosX(), (long)GetPosY(), FALSE);
		//! �������ֻ��1�������ˣ����ͬ��û�ж������������ظ�������
		if(1 == RegionTeamates.size())
			return RegionTeamates.begin()->second;
		return NULL;
	}
	else if (GetBeneficiaryType()== TYPE_PLAYER)
	{
		pBeneficiary=  GetGame() -> FindPlayer( GetBeneficiaryID() );
	}


	//##���û���ҵ�������,����ȥ�������ɱ���������
	if( pBeneficiary == NULL )
	{
		long killer_type = GetKilledMeType();
		const CGUID &killer_id = GetKilledMeID();
		if( killer_type == TYPE_PLAYER )
		{
			pBeneficiary =  GetGame() -> FindPlayer( killer_id );
		}
		// ����ɱ��û��������
		//else if( killer_type == TYPE_MONSTER )
		//{
		//CMonster* pMonster = dynamic_cast<CMonster*>( pRegion -> FindChildObject( TYPE_MONSTER, killer_id ) );
		//if( pMonster && pMonster -> DoesCreatureBeenTamed() && pMonster -> GetMasterInfo().lMasterType == TYPE_PLAYER )
		//{
		//	pBeneficiary = GetGame() -> FindPlayer( pMonster -> GetMasterInfo().MasterID );
		//}
		//}
		//##�ҵ����ɱ���ù������?
		if( pBeneficiary )
		{
			//##����ҵ����ɱ���ù������,û������,��9��Area�ķ�Χ��
			if( pBeneficiary -> IsDied() == FALSE &&
				Distance( pBeneficiary ) <= GlobalSetup::GetSetup() -> new_lExperienceRadii )
			{
				//##�ɹ��ҵ������
			}
			//##�������Ҳ��������������������ж���
			else if( NULL_GUID != pBeneficiary -> GetTeamID() )
			{
				//LONG lSessionID = GetGame() -> GetTeamSessionID( pBeneficiary -> GetTeamID() );
				//CTeam* pTeam = dynamic_cast<CTeam*>( const_cast<CSession*>( CSessionFactory::QuerySession( lSessionID ) ) );
				GSTeam *pGSTeam = (GSTeam *)GetOrganiCtrl()->GetGameTeamByID(pBeneficiary -> GetTeamID());

				//##�ڲ��Ҷ�Ա�Ĺ���֮ǰ���Ƚ���������ΪNULL��
				LONG lRegionID = pBeneficiary -> GetRegionID();
				//pBeneficiary = NULL;
				if( pGSTeam )
				{
					pBeneficiary = pGSTeam -> FindTeamatesInCurrentRegion( pRegion->GetExID() );
				}
			}
			else
			{
				//##����Ҳ���������,��û�����,ֱ�ӷ���null
				pBeneficiary = NULL;
			}

		}
	}

	return pBeneficiary;
}

//��������
void CMonster::OnDied()
{
	//�����������
	//GetAI() -> Clear();

	//##��ȡ������ָ��
    if( GetPhaseID()!=NULL_GUID)
        PHASEMAN.OnPhaseOper( GetPhaseID(),this, eMonsterDie );

	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return;

	//##��ȡȫ�ֵ�����
	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
	if( pSetup == NULL ) return;

	//ȡ�����赲
	SetBlock(GetTileX(),GetTileY(),CRegion::BLOCK_NO);

	BOOL bKilledByGuard = FALSE;	// �Ƿ�������ɱ����

	// ���ɱ���Լ��Ķ���
	CMoveShape* pKiller = dynamic_cast<CMoveShape*>( pRegion -> 
		FindChildObject( GetKilledMeType(), GetKilledMeID() ) );

	CMonster* pMonster = 0;
	if (pKiller != NULL)
	{
		if (pKiller->GetType() == TYPE_MONSTER)
		{
			pMonster = (CMonster*)( pKiller );
			if( pMonster && pMonster->GetNpcType() == NT_Guard)
			{
				bKilledByGuard = TRUE;
			}
		}
		else if( TYPE_PET == pKiller->GetType() )
		{
			pKiller = pKiller->GetHost();
		}
	}

	// �ж��Ƿ�����������3.24.2009
	if( bKilledByGuard == FALSE )
	{
		//CMonsterAI *pAI = static_cast<CMonsterAI*>( GetAI() );
		//if( pAI != NULL )
		//{
		//	bKilledByGuard = pAI->IsAttackedByGuard() ? TRUE : FALSE;
		//}
	}

	map<CGUID,CPlayer*> RegionTeamates;

	// ��ͨ���� �� �ڳ��� ���ᱩ��Ʒ�;���
	if( bKilledByGuard == FALSE)
	{
		//##���ȸ�ɱ���Լ����˷�����������
		CPlayer* pPlayer = 0;
		if (pKiller!= NULL && pKiller->GetType () == TYPE_PLAYER)
		{
			pPlayer = (CPlayer*)( pKiller );
		}

		if( pPlayer && pPlayer -> GetLevel() - pSetup -> lHitBaseLv <= GetLevel() )
		{
			pPlayer -> IncreaseContinuousKill();
		}

		CPlayer* pBeneficiary=NULL;

		//##����Ǳ�Guardɱ������û���κ����ܻ�ȡ����
		if( bKilledByGuard == FALSE )
		{
			//! ���������ˣ����������£���������ֻ��1����Ҳ�᷵�أ���ʱ���Ƕ���������㣩
			pBeneficiary= GetBeneficiary();

			//! ���û�ҵ������ˣ�˵���������п��ܲ�ֹһ����Ҫ�鿴������ص���Ϣ
			if( NULL == pBeneficiary )
			{
				GSTeam *pGSTeam = (GSTeam *)GetOrganiCtrl()->GetGameTeamByID(GetBeneficiaryID());
				//! �ҵ��������
				if( NULL != pGSTeam)
				{
					RegionTeamates.clear();
					pGSTeam->GetLucrativeArea_Alive(RegionTeamates, pRegion->GetExID(), (long)GetPosX(), (long)GetPosY(), FALSE);
					assert(1 != RegionTeamates.size());

					for (map<CGUID,CPlayer*>::iterator ite = RegionTeamates.begin(); ite != RegionTeamates.end(); ++ite)
					{						
						//CPlayer* pMember = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
						//##��������ڵĳ�Ա��Ȼ����9��Area��Χ�ڣ���û�������Ļ�
						CPlayer* pMember = ite->second;
						if( pMember && pMember -> IsDied() == FALSE )
						{
							//���侭��
							DWORD dwExperience = CalculateExperienceQuota( pMember, (long)GetPosX(), (long)GetPosY() );
							dwExperience = static_cast<DWORD>( dwExperience * pSetup -> fExpScale );
							//���ϳ����������ϵ��
							dwExperience = static_cast<DWORD>( dwExperience * pRegion->GetExpScale());												
							// ����ɫ�ϻ�þ��鱶��
							dwExperience = (DWORD)( (float)dwExperience * pMember->GetExpScale() );
							//��ȡ���;���
							dwExperience += pMember->AcquirePresentExp(dwExperience);
							// ���վ����ȡ
							pMember -> IncExp( dwExperience , eAET_Monster);
							pMember -> AddIncExpLog(pMember->GetExp(), pMember->GetExpMultiple(), dwExperience, 0, eAET_Monster, GetName());
							pMember->AllEquipmentRefineUpdate(dwExperience);
							pMember -> CheckLevel();

							//����ְ������
							eOccupation eOccu = (eOccupation)pMember->GetOccupation();
							DWORD dwOccuExp = CalculateOccuExpQuota(pMember, (long)GetPosX(), (long)GetPosY());
							dwOccuExp = static_cast<DWORD>( dwOccuExp * pSetup -> fOccuExpScale );
							//���ϳ���ְ���������ϵ��
							dwOccuExp = static_cast<DWORD>( dwOccuExp * pRegion->GetOccuExpScale());												
							// ����ɫ�ϻ��ְ�����鱶��
							dwOccuExp = (DWORD)( (float)dwOccuExp * pMember->GetOccuExpScale() );
							DWORD dwOldOccuExp = pMember->GetOccuExp(eOccu);
							//���������ְҵ����
							pMember->IncOccuExp(eOccu,dwOccuExp,eAET_Monster);
							pMember->AddIncOccuExpLog(eOccu,dwOldOccuExp, dwOccuExp , eAET_Monster, GetName());
							pMember->CheckOccuLevel(eOccu);


							//����Ԫ��
							DWORD dwSkillPoint = CalculateOccuPointQuota( pMember, (long)GetPosX(), (long)GetPosY() );
							//����ȫ�ַ���ϵ��
							dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pSetup -> fSpScale );
							//���ϳ���Ԫ������ϵ��
							dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pRegion->GetSpScale());
							// ����ɫ�ϻ�ü��ܵ�������
							dwSkillPoint = (DWORD)( (float)dwSkillPoint * pMember->GetExpScale() );
							// ���ռ��ܵ�����ȡ
							pMember -> IncOccuSp( eOccu,dwSkillPoint );

						}
					}
				}
			}
			else
			{
				//##û�����
				DWORD dwExperience = CalculateExperienceQuota( pBeneficiary, (long)GetPosX(), (long)GetPosY() );
				dwExperience = static_cast<DWORD>( dwExperience * pSetup -> fExpScale );
				//���ϳ����������ϵ��
				dwExperience = static_cast<DWORD>( dwExperience * pRegion->GetExpScale());	
				// ����ɫ����������״̬ �������Ը�״̬��ϵ��
				float fExExp = 1; // ���鱶��
				dwExperience = (DWORD)( (float)dwExperience * fExExp );
				//��ȡ���;���
				dwExperience += pBeneficiary->AcquirePresentExp(dwExperience);

				pBeneficiary -> IncExp( dwExperience, eAET_Monster);
				pBeneficiary -> AddIncExpLog(pBeneficiary->GetExp(), pBeneficiary->GetExpMultiple(), dwExperience, 0, eAET_Monster, GetName());
				pBeneficiary->AllEquipmentRefineUpdate(dwExperience);
				pBeneficiary -> CheckLevel();

				//����ְ������
				eOccupation eOccu = (eOccupation)pBeneficiary->GetOccupation();
				DWORD dwOccuExp =CalculateOccuExpQuota(pBeneficiary, (long)GetPosX(), (long)GetPosY());
				dwOccuExp = static_cast<DWORD>( dwOccuExp * pSetup -> fOccuExpScale );
				//���ϳ���ְ���������ϵ��
				dwOccuExp = static_cast<DWORD>( dwOccuExp * pRegion->GetOccuExpScale());												
				// ����ɫ�ϻ��ְ�����鱶��
				dwOccuExp = (DWORD)( (float)dwOccuExp * pBeneficiary->GetOccuExpScale() );
				DWORD dwOldOccuExp = pBeneficiary->GetOccuExp(eOccu);
				//���������ְҵ����
				pBeneficiary->IncOccuExp(eOccu,dwOccuExp,eAET_Monster);
				pBeneficiary -> AddIncOccuExpLog(eOccu,dwOldOccuExp, dwOccuExp , eAET_Monster, GetName());
				pBeneficiary->CheckOccuLevel(eOccu);

				//����Ԫ��
				DWORD dwSkillPoint = CalculateOccuPointQuota( pBeneficiary, (long)GetPosX(), (long)GetPosY() );
				//����ȫ�ַ���ϵ��
				dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pSetup -> fSpScale );
				//���ϳ���Ԫ������ϵ��
				dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pRegion->GetSpScale());
				// ����ɫ�ϻ�ü��ܵ�������
				dwSkillPoint = (DWORD)( (float)dwSkillPoint * pBeneficiary->GetExpScale() );
				// ���ռ��ܵ�����ȡ
				pBeneficiary -> IncOccuSp( eOccu,dwSkillPoint );
			}
		}
		vector<CGoods *> vOut;
		long  lDropGoodsStartPos=0;

		CMonsterList::tagMonster* p = CMonsterList::GetPropertyByOrginName(m_strOrginName.c_str());
		BYTE drop_flag = 0;
		if( p != NULL )
		{
			drop_flag = p->DropFlag;
		}
		if (pKiller!=NULL)
		{  
			CheckDropGoods(GetOriginName(),GetRegionID(),GetLevel(),pKiller->GetLevel()-GetLevel(),vOut, drop_flag);
		}
		else if (pMonster!= NULL && pMonster->GetNpcType()==NT_Normal)
		{
			CheckDropGoods(GetOriginName(),GetRegionID(),GetLevel(),0,vOut, drop_flag);
		}

		for( long sGoods = 0; sGoods < (long)vOut.size(); sGoods ++ )
		{
			if( vOut[sGoods] )
			{
				long x, y;

				lDropGoodsStartPos= pRegion->GetDropGoodsPos(GetTileX(), GetTileY(), x, y, lDropGoodsStartPos);
				if( lDropGoodsStartPos != -1 )
				{
					vOut[sGoods]->SetPosXY(x+0.5f, y+0.5f);

					pRegion->AddObject( vOut[sGoods] );
					if(vOut[sGoods])
					{
						// ֪ͨÿ�����
						CMessage MsgSend( MSG_S2C_RGN_ADDSHAPE,vOut[sGoods]->GetExID());
						MsgSend.Add( vOut[sGoods]->GetType() );
						MsgSend.Add( vOut[sGoods]->GetExID() );
						DBWriteSet setWriteDB;
						MsgSend.GetDBWriteSet(setWriteDB);
						vOut[sGoods]->SerializeForRgnDisplay(setWriteDB);

						MsgSend.SendToAround(vOut[sGoods]);				
#ifdef _GOODSLOG1_
						char pszGoodsLog[1024]="";					
						char goodsGUID[50]="";
						vOut[sGoods]->GetExID().tostring(goodsGUID);
						_snprintf(pszGoodsLog,1024,"[����:������������][�����:%s][����ID:%d][��ƷID:%d][��ƷGUID:%s][��Ʒ����:%d][X����:%d][Y����:%d]",
							pPlayer->GetName(),pRegion->GetID(),vOut[sGoods]->GetBasePropertiesIndex(),
							goodsGUID,vOut[sGoods]->GetAmount(),GetTileX(),GetTileY());
						CMessage::AsyWriteFile(GetGame()->GetGoodsLogFile(),pszGoodsLog);	
#endif	
					}

					// ��Ʒ����
					if( pBeneficiary  || RegionTeamates.size()!=0)
					{
						if(pBeneficiary==NULL)
						{
							pBeneficiary=RegionTeamates.begin()->second;
						}
						if (pBeneficiary)
						{
							pRegion -> SetGoodsProtection( pBeneficiary, vOut[sGoods] );
						}
					}
					
				}
			}
		}

	}

	// ִ�нű�
	if( bKilledByGuard == FALSE )
	{
		//##����ִ�нű�
		for (map<CGUID,CPlayer*>::iterator it= RegionTeamates.begin(); it!=RegionTeamates.end() ; it++)
		{
			//�����ӳ�Ա
			CPlayer* pBeneficiary=it->second;

			if( !m_strScriptFile.empty()&& pBeneficiary )
			{
				//stRunScript rsScript;
				//rsScript.pszFileName	= static_cast<CHAR*>( const_cast<CHAR*>( GetScriptFile() ) );
				//rsScript.srcShape	= pBeneficiary;
				//rsScript.pRegion	= pRegion;
				//rsScript.desShape		= this;
				//rsScript.dwDiedMonsterIndex =  GetIndex();//GetBaseProperty()->dwIndex;
				//RunScript( &rsScript, static_cast<CHAR*>( const_cast<CHAR*>( GetGame() -> 
				//	GetScriptFileData( GetScriptFile() ) ) ) );
				Script::RunEnv env( pBeneficiary, this, pRegion );
				GetInst( ScriptSys ).RunScript( env, GetScriptFile() );
			}
		}
		if (RegionTeamates.size()==0)
		{
			//##����������
			CPlayer* pBeneficiary = GetBeneficiary();

			if( !m_strScriptFile.empty() && pBeneficiary )
			{
				//stRunScript rsScript;
				//rsScript.pszFileName	= static_cast<CHAR*>( const_cast<CHAR*>( GetScriptFile() ) );
				//rsScript.srcShape	= pBeneficiary;
				//rsScript.pRegion	= pRegion;
				//rsScript.desShape		= this;
				//rsScript.dwDiedMonsterIndex =  GetIndex();//GetBaseProperty()->dwIndex;
				//RunScript( &rsScript, static_cast<CHAR*>( const_cast<CHAR*>( GetGame() -> 
				//	GetScriptFileData( GetScriptFile() ) ) ) );
				Script::RunEnv env( pBeneficiary, this, pRegion );
				GetInst( ScriptSys ).RunScript( env, GetScriptFile() );
			}
		}
	}

	//##ɾ��
	//##����ˢ���б��е��������
	CServerRegion::tagMonster* pRefresh =NULL;
	//pRegion -> GetMonsterRefeash( GetProperty() -> lRefeashIndex );
	if( pRefresh && pRefresh -> lLivingCount > 0)
	{
		pRefresh -> lLivingCount --;
	}

	//CleanBeneficiary();
	LogoutDelEvent();
	ClearActiveSkills();
	EndBuffAndDebuff();

	//�ɼ����ʼ��
	const CCollectionLayout* p=CDeputyOccuSystem::GetCollectionLayout(GetCollectionName().c_str());
	if (p!=NULL)
	{
		SetCollectionID(p->GetCollectionID());
		//SetCollectionTimes(random(p->GetMaxTime()-p->GetMinTimes())+p->GetMinTimes());
		int mintime=p->GetMinTimes();
		int maxtime=p->GetMaxTime();
		if (mintime>=maxtime)
		{
			SetCollectionTimes(maxtime);
		}
		else 
		{
			SetCollectionTimes(random(maxtime-mintime+1)+mintime);
		}

		if (GetCollectionTimes()<=0)
		{
			SetCollectionTimes(1);
		}
		SetIsBeenCollect(FALSE);
		SetCanBeenCollect(TRUE);
		UpdateAttribute();
	}
}

void CMonster::OnBeenHurted( LONG lType, const CGUID& AttackerGuid ) 
{
	//##��¼��һ�������Լ�����
	//##FindChildObject()������ƿ�����ᵼ�·��������س���
	CGUID AttackPlayerID=AttackerGuid;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if(pRegion== NULL) return;


	CMoveShape* pMoveShape= dynamic_cast<CMoveShape*>(pRegion->FindChildObject(lType,AttackPlayerID));
	if (pMoveShape== NULL) return ;

	CPlayer* pPlayer = 0;

	if (lType==TYPE_MONSTER)
	{
		//lType=((CMonster*)pMoveShape)->GetMasterInfo().lMasterType;
		//AttackPlayerID=((CMonster*)pMoveShape)->GetMasterInfo().MasterGuildID;
		//pPlayer=  dynamic_cast<CPlayer*>(pRegion->FindChildObject(TYPE_PLAYER,AttackPlayerID));
	}
	else if (lType== TYPE_PLAYER)
	{
		pPlayer = (CPlayer*)pMoveShape;
	}
	else if( TYPE_PET == lType )
	{
		pPlayer = dynamic_cast<CPlayer*>(pMoveShape->GetHost());
		lType = TYPE_PLAYER;
		AttackPlayerID = pPlayer->GetExID();
	}
	else
		return;


	if( pPlayer != NULL )
	{
		const CGUID &gTeamID= pPlayer->GetTeamID();
		if( gTeamID!= NULL_GUID )
		{
			AttackPlayerID=gTeamID;
			lType= TYPE_TEAM;
		}
	}

	// ���������������������������������Ϊ�������������ˣ�
	bool bGuardAttacker = false;
	if( lType == TYPE_MONSTER )
	{
		CMonster *pGuardAttacker = static_cast<CMonster*>( pMoveShape );
		bGuardAttacker = pGuardAttacker->GetNpcType() == NT_Guard;

		// �����߲���������û��Ҫ���������˴���
		if( !bGuardAttacker )
		{
			return;
		}
	}

	//���ı�������ʱ��
	if ( GetBeneficiaryID()==NULL_GUID || timeGetTime()- GetBeneficiaryTime() > GlobalSetup::GetSetup()->dwAttackMonsterProtectedTimer
		|| bGuardAttacker )
	{
		// ��������������������������������������
		//if( lType == TYPE_PLAYER || lType == TYPE_TEAM )
		//{
		//	CMonsterAI *pAI = static_cast<CMonsterAI*>( GetAI() );
		//	if( pAI != NULL && pAI->IsAttackedByGuard() )
		//	{
		//		return;
		//	}
		//}

		if (CheckDelete())
		{
		}
		else
			StartRecordBeneficiary(AttackPlayerID,(OBJECT_TYPE)lType,0);//
	}
	else if (GetBeneficiaryID()==AttackPlayerID)
	{
		RecordBeneficiaryTime();
	}
}


//void CMonster::AdjustPetProperties()
//{
//	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
//	if( pSetup && m_dwCurrentPetLevel < PET_MAX_LEVEL )
//	{
//		m_fMaxAttackFactor			= pSetup -> m_fMaxAttackFactor[m_dwCurrentPetLevel];
//		m_fMinAttackFactor			= pSetup -> m_fMinAttackFactor[m_dwCurrentPetLevel];
//		m_fElementAttackFactor		= pSetup -> m_fElementAttackFactor[m_dwCurrentPetLevel];
//		m_fElementResistantFactor	= pSetup -> m_fElementResistantFactor[m_dwCurrentPetLevel];
//		m_fDodgeFactor				= pSetup -> m_fDodgeFactor[m_dwCurrentPetLevel];
//		m_fDefenseFactor			= pSetup -> m_fDefenseFactor[m_dwCurrentPetLevel];
//		m_fMaxHpFactor				= pSetup -> m_fMaxHpFactor[m_dwCurrentPetLevel];
//		m_fAttackIntervalFactor		= pSetup -> m_fAttackIntervalFactor[m_dwCurrentPetLevel];
//		m_fSpeedFactor				= pSetup -> m_fSpeedFactor[m_dwCurrentPetLevel];
//		m_fStopFrameFactor			= pSetup -> m_fStopFrameFactor[m_dwCurrentPetLevel];
//	}
//}

//void CMonster::UpgradePetLevel()
//{
//	//##�����жϵ�ǰ�����Ƿ񵽴Ȼ�����ú���ô������
//	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
//
//	if( pSetup )
//	{
//		if( m_dwCurrentPetLevel < PET_MAX_LEVEL )
//		{
//			if( m_dwCurrentExperience > GetExpDead() * pSetup -> m_fExprienceFactor[m_dwCurrentPetLevel] )
//			{
//				if( m_dwCurrentPetLevel + 1 < PET_MAX_LEVEL )
//				{
//					++ m_dwCurrentPetLevel;
//					m_dwCurrentExperience = 0;
//					AdjustPetProperties();
//
//					//##���õ�ǰHpΪ���Hp
//					SetHP( GetMaxHP() );
//				}
//			}
//			else 
//			{
//				if( m_dwCurrentPetLevel == 0 && m_dwCurrentExperience == 0 )
//				{
//					m_dwCurrentExperience = 0;
//					AdjustPetProperties();
//				}
//			}
//		}
//	}
//}

//void CMonster::IncreasePetExperience( DWORD dwExperience )
//{
//	m_dwCurrentExperience += dwExperience;
//	UpgradePetLevel();
//	if( dwExperience )
//	{
//		CMessage msg( MSG_S2C_PET_CHANGE_EXPERIENCE );
//		msg.Add( GetType() );
//		msg.Add( GetExID() );
//		msg.Add( GetPetLevel() );
//		msg.Add( GetPetExperience() );
//		msg.Add( GetMaxHP() );
//		msg.Add( GetHP() );
//		msg.SendToAround( this );
//	}
//}

// �ɷ񹥻�
bool CMonster::IsAttackAble(CMoveShape* pAttacker)
{
	bool bResult = false;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );

	if( pRegion )
	{
		switch( pAttacker -> GetType() )
		{
		case TYPE_PLAYER:
            {
                if( PHASEMAN.PhasePlayerFilter( pAttacker, this ) || IsCanBeenAttack()==0 )
                    bResult = false;
                else 
                    bResult = true;
            }
			break;

		case TYPE_PET:
			bResult = true;
			break;

		case TYPE_MONSTER:
			bResult = true;
			break;
		}
	}
	return bResult;
}


long CMonster::GetStopFrame()
{
	//LONG lResult = GetStopTime();
	//if( DoesCreatureBeenTamed() )
	//{
	//	lResult = static_cast<LONG>( lResult * m_fStopFrameFactor );
	//}
	return GetStopTime ();
}

void CMonster::GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY)
{
	if( GetFigure() == 0 )
	{
		lTileX = GetTileX();
		lTileY = GetTileY();
		return;
	}
	else 
	{
		long lDistance = 10000000;
		long lDir = 0;
		lTileX = GetTileX();
		lTileY = GetTileY();
		for (long i=GetTileX()-GetFigure(DIR_RIGHT); i<=GetTileX()+GetFigure(DIR_RIGHT); i++)
		{
			for (long j=GetTileY()-GetFigure(DIR_UP); j<=GetTileY()+GetFigure(DIR_UP); j++)
			{
				if(lDistance>Distance(i,j,lAttackerX,lAttackerY))
				{
					lDistance=Distance(i,j,lAttackerX,lAttackerY);
					lDir = GetDestDir(lAttackerX,lAttackerY,i,j);
					lTileX = i;lTileY = j;
				}
				else if( lDistance==Distance(i,j,lAttackerX,lAttackerY) &&
					(lDir%2==1) && (GetDestDir(lAttackerX,lAttackerY,i,j)%2==0) )
				{
					lDistance=Distance(i,j,lAttackerX,lAttackerY);
					lDir = GetDestDir(lAttackerX,lAttackerY,i,j);
					lTileX = i;lTileY = j;
				}
			}
		}
	}
}


//##��ȡ��ȭ��ʱ��
DWORD CMonster::GetStrikeOutTime()
{
	/*if( GetBaseProperty() == NULL ) */
	return 600;
	//else 
	//	return GetBaseProperty() -> dwStrikeOutTime;
}

void CMonster::Evanish()
{
	//##����֪ͨ����
	//NotifyMasterWhenPetDied();

	CMoveShape::Evanish();
}

void CMonster::ExpAction(long ActID,long DesType,CGUID DesGUID)
{
	//CMoveShape *pMoveShape = GetGame() -> FindShape(OBJECT_TYPE(dwSrcType), guidSrc );
	CMessage msg(MSG_S2C_OTHER_EXPATC);
	msg.Add(ActID);
	msg.Add(GetType());
	msg.Add(GetExID());
	msg.Add(DesType);
	msg.Add(DesGUID);
	msg.SendToAround(this);
}

void CMonster::OnLoseBeneficiary()			//��ʧ�������¼�
{
	if (timeGetTime() > GetBeneficiaryTime()+ GlobalSetup::GetSetup()->dwBeneficiaryTime)
	{
		CleanBeneficiary();
		CheckDelete();
	}
	else
	{
		AddBeneficiaryEvent(GetBeneficiaryTime()+ GlobalSetup::GetSetup()->dwBeneficiaryTime);
	}
}

bool CMonster::CheckDelete()
{
	if (GetRoleProtect())
	{
		CServerRegion* pServerRegion = dynamic_cast<CServerRegion*>(GetFather() );//Ѱ�����ڳ���

		if(pServerRegion)
		{
			pServerRegion->DelGurdMonster(GetExID() );

			int iFreshIndex=GetRefeashIndex();
			CServerRegion::tagMonster* pRefresh=pServerRegion->GetMonsterRefeash( iFreshIndex);
			if (pRefresh!= NULL)
			{
				pServerRegion->OnRemoveObject(this,pRefresh->lResetTime);
			}
			else
			{
				pServerRegion->OnDeleteObject(this);
			}
		}
		return true;
	}
	return false;
}

void CMonster::StartRecordBeneficiary(CGUID guidFirst,OBJECT_TYPE typeFirst,DWORD time)				//��ʼ��¼������
{
	SetBeneficiary(typeFirst,guidFirst);
	RecordBeneficiaryTime();
	if (time==0)
		AddBeneficiaryEvent(GlobalSetup::GetSetup()->dwBeneficiaryTime);
	else
		AddBeneficiaryEvent(time);
}

//DWORD CMonster::GetPetLevel()
//{
//	return m_dwCurrentPetLevel;
//}
//
//DWORD CMonster::GetPetExperience()
//{
//	return m_dwCurrentExperience;
//}
//
//void CMonster::SetPetLevel( DWORD dwLevel )
//{
//	if( dwLevel < PET_MAX_LEVEL )
//	{
//		m_dwCurrentPetLevel = dwLevel;
//	}
//}
//
//void CMonster::SetPetExperience( DWORD dwExperience )
//{
//	m_dwCurrentExperience = dwExperience;
//}


BOOL CMonster::ChangeAI()
{
	AI_ASSERT( "ChangeAI not implemented" && 0 );
	//if (GetAI()->GetAIType()==GetAIType())
	//{
	//	return TRUE;
	//}
	//int AIType=GetAIType();
	//CBaseAI* pAI=CAIFactory::CreateAI(AIType);

	//if (pAI==0)
	//{
	//	SetAIType(GetAI()->GetAIType());
	//	return FALSE;
	//}
	//*pAI=*m_pAI;

	//CBaseAI* point=m_pAI;

	//m_pAI=pAI;

	//CAIFactory::DeleteAI(point);

	return TRUE;
}

void CMonster::ResumeAll()
{
	CMonsterList::tagMonster* p = CMonsterList::GetPropertyByOrginName(GetOriginName());
	if (p)
	{
		SetName( p ->cName );// strName.c_str() );
		SetGraphicsID( p -> dwPicID );
		//..........monster ���Գ�ʼ��  
		//  [8/27/2009 chenxianj]
		//�޸Ĺ����λ���������Ժ�ɾ��״̬���ֵ�bug
		EndBuffAndDebuff();
		ClearActiveSkills();

		InitProperty(p);
		m_mapDirtyAttrData.insert("Hp");
		UpdateAttribute();
		SetCollectionID(0);
		//ClearAllBuff();
	
		CleanBeneficiary();
        m_tauntHandler->Clear();
	}
	SetState(CShape::STATE_PEACE);
	//if (GetAIType()!= m_pAI->GetAIType())
	//{
	//	CBaseAI* ai=CAIFactory::CreateAI(GetAIType());
	//	*ai=*m_pAI;
	//	CBaseAI* point=m_pAI;
	//	m_pAI=ai;
	//	CAIFactory::DeleteAI(point);
	//}
}

BOOL CMonster::Relive()
{
	return dynamic_cast<CServerRegion*>(this->GetFather())->AddOneMonster(this);
}
//////////////////////////////////////////////////////////////////////////
//���ù���ս������
//////////////////////////////////////////////////////////////////////////

//�����������
void CMonster::SetNearAtkDis(DisType l)	
{

	m_tgMonsterFightProperty.NeaAtkDis = l;
}
//��Զ��������
void CMonster::SetFarAtkDis(DisType l)		
{
	m_tgMonsterFightProperty.FarAtkDis = l;
}	
//��������
void CMonster::SetMaxHp(DWORD l)	
{
	m_tgMonsterFightProperty.dwMaxHp = l; 
}		
//��С������
void CMonster::SetMinAtk(WORD l)	
{
	m_tgMonsterFightProperty.wMinAtk = l; 
}
//��󹥻���
void CMonster::SetMaxAtk(WORD l)		
{
	m_tgMonsterFightProperty.wMaxAtk = l;
}
//��С����������
void CMonster::SetMinMAtk(WORD l)		
{
	m_tgMonsterFightProperty.wMinMAtk = l;
}
//�����������
void CMonster::SetMaxMAtk(WORD l)		
{
	m_tgMonsterFightProperty.wMaxMAtk = l; 
}
//�����ָ��ٶ�
void CMonster::SetHpRecoverSpeed(WORD l)	
{
	m_tgMonsterFightProperty.wHpRecoverSpeed = l;
}

//���䷶Χ
void CMonster::SetGuardRange(DisType l)	
{
	m_tgMonsterFightProperty.GuardRange = l;
}
//׷����Χ
void CMonster::SetChaseRange(DisType l)	
{
	m_tgMonsterFightProperty.ChaseRange = l;
}	
//��������
void CMonster::SetHit(WORD l)			
{
	m_tgMonsterFightProperty.wHit = l;
}
//������
void CMonster::SetDef(WORD l)			
{
	m_tgMonsterFightProperty.wDef = l;
}
//ħ��������
void CMonster::SetMDef(WORD l)
{

	m_tgMonsterFightProperty.wMDef = l;
}
//��������
void CMonster::SetDodge(WORD l)			
{

	m_tgMonsterFightProperty.wDodge = l;
}
//��
void CMonster::SetFightBlock(WORD l)			
{

	m_tgMonsterFightProperty.wBlock = l;
}
//�м�
void CMonster::SetParry(WORD l)			
{
	m_tgMonsterFightProperty.wParry = l;
}	

//�ƶ��ٶ�
void CMonster::SetMoveSpeed(float f)			
{
	m_tgMonsterProperty.fMoveSpeed = f;
}
//ս���ƶ��ٶ�
void CMonster::SetFightMoveSpeed(float f)		
{
	m_tgMonsterProperty.fFightMoveSpeed = f;
}
//��־����
void CMonster::SetWillDef(WORD l)				
{
	m_tgMonsterFightProperty.wWillDef = l;
}
//���ʿ���
void CMonster::SetConstiDef(WORD l)			
{
	m_tgMonsterFightProperty.wConstiDef = l;
}
//Debuff����
void CMonster::SetDeBuffDef(WORD lpos,WORD l)			
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1;
	m_tgMonsterFightProperty.wDeBuffDef[lpos] = l;
}
//����Ԫ�ؿ���
void CMonster::SetElemDef(WORD lpos,WORD l)			
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
	m_tgMonsterFightProperty.wElemDef[lpos] = l;
}
//D�Ը��������˺�
void CMonster::SetClassDam(WORD lpos,WORD l)	
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>C_Max) lpos = C_Max -1;
	m_tgMonsterFightProperty.wClassDam[lpos] = l;
}
//�Ը���Ԫ���˺�
void CMonster::SetElemDam(WORD lpos,WORD l)	
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
	m_tgMonsterFightProperty.wElemDam[lpos] = l;
}
//��������
void CMonster::SetCri(WORD l)							{m_tgMonsterFightProperty.wCri = l;}
//�������˺�
void CMonster::SetCriDamg(WORD l)						{m_tgMonsterFightProperty.wCriDamg = l;}
//����������
void CMonster::SetCriDef(WORD l)						{m_tgMonsterFightProperty.wCriDef = l;}
//ħ��������
void CMonster::SetMCri(WORD l)							{m_tgMonsterFightProperty.wMCri = l;}
//ħ�������˺�
void CMonster::SetMCriDamg(WORD l)						{m_tgMonsterFightProperty.wMCriDamg = l;}
//ħ����������
void CMonster::SetMCriDef(WORD l)						{m_tgMonsterFightProperty.wMCriDef = l;}
void CMonster::SetCanTalk(WORD l)						
{
	m_mapDirtyAttrData.insert("CanTalk");
	m_tgMonsterProperty.wCanTalk=l;
}

void CMonster::AddToColony(DWORD dwColonyID, WORD wMemberType)
{
	if (dwColonyID== 0 || GetColonyID()!=0 ) return;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return ;
	CColony* pColony= pRegion->GetOneColony(dwColonyID);
	if (pColony== NULL) return;

	BOOL bMemID= pColony->AddMember(wMemberType== 1,this);
	if (bMemID == TRUE)
	{
		if (wMemberType==1)
		{
			SetColonyID(dwColonyID);
			SetMemberType(wMemberType);
		}
		else
		{
			SetColonyID(dwColonyID);
			SetMemberType(wMemberType);
		}
	}
}

//�ı�����״̬
void CMonster::ChangeWeaknessState(bool l)
{
	SetWeakNessState(l);
	/* if (l== GetWeaknessState()) 
	return ;*/
	if (GetWeaknessState() == false)
	{
		if (GetState()== CShape::STATE_FIGHT)
		{
			SetSpeed(GetMoveSpeed());
		}
	}
	else
	{
		if (GetState()==CShape::STATE_FIGHT)
		{
			SetSpeed(GetFightMoveSpeed());
		}
	}
	SendChangedSpeedToClients();
}

// ��Ӧɾ���¼�
//void CMonster::OnDelEvent()
//{
//	CServerRegion* pServerRegion = dynamic_cast<CServerRegion*>(  GetFather() );//Ѱ�����ڳ���
//	CleanDelEvent();
//	if(pServerRegion)
//	{
//		pServerRegion->DelGurdMonster( GetExID() );
//
//		int iFreshIndex=GetRefeashIndex();
//		CServerRegion::tagMonster* pRefresh=pServerRegion->GetMonsterRefeash( iFreshIndex);
//		//m_pOwner->OnLoseBeneficiary();
//		CleanBeneficiary();
//
//		if (pRefresh!= NULL)
//		{
//			pServerRegion->OnRemoveObject(this,pRefresh->lResetTime);
//		}
//		else
//		{
//			pServerRegion->OnDeleteObject(this);
//		}
//	}
//	
//
//	return ;
//}

//CVariableList* CMonster::GetVariableList()
//{
//	return m_pVariableList;
//}
//
//int CMonster::SetScriptVarValue(CScript* scriptPtr, const char* varName, const char* value)
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
//int CMonster::SetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray, double value)
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
//const char* CMonster::GetScriptVarValue(CScript* scriptPtr, const char* varName)
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
//double CMonster::GetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray)
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
//int CMonster::SetScriptGuidValue(CScript* scriptPtr, const char* varName, const CGUID& guid)
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
//const CGUID& CMonster::GetScriptGuidValue(CScript* scriptPtr, const char* varName)
//{
//	return scriptPtr->GetScriptGuidValue( this, varName );
//}

BOOL CMonster::BeenCollectedAll()
{
	AI_ASSERT( "BeenCollectedAll not implemented" && 0 );
	//CMonsterAI* pMonsterAI= dynamic_cast<CMonsterAI*>(m_pAI);
	//if (GetState()==CShape::STATE_DIED && pMonsterAI!= NULL)
	//{
	//	pMonsterAI->LogoutCurrentEvent();
	//	pMonsterAI->OnCheckDeadState();
	//	return TRUE;
	//}
	return FALSE;
}

/// 
BOOL CMonster::CheckDropGoods(string MonsterName, INT RegionIndex,
							  INT Level,INT LevelBalances, vector<CGoods*> &vOut, BYTE drop_flag )
{
	//tgMonsterDrop* drop= m_mapMonsterDrop[MonsterName];
	map<string,CMonsterList::tgMonsterDrop>* pMonsterDropList = CMonsterList::GetMonsterDropList();
	vector<CMonsterList::tagDropGoods*>* pDropGoodsList = CMonsterList::GetDropGoodsList();
	map<int, int>* pRegionDropList = CMonsterList::GetRegionDropList();
	vector<int>* pLevelDropList = CMonsterList::GetLevelDropList();
	int WorldDropIdx = CMonsterList::GetWorldDropIdx();

	map<string,CMonsterList::tgMonsterDrop>::iterator it=pMonsterDropList->find(MonsterName);
	if (it!=pMonsterDropList->end())
	{
		CMonsterList::tgMonsterDrop *md = &(*it).second;

		if (md->ShareDropIndex!=DROPINDEX_NULL && md->ShareDropIndex< (int)pDropGoodsList->size())
		{
			CMonsterList::tagDropGoods *drop = (*pDropGoodsList)[md->ShareDropIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (md->OwnDropIndex !=DROPINDEX_NULL && md->OwnDropIndex < (int)pDropGoodsList->size())
		{
			CMonsterList::tagDropGoods *drop = (*pDropGoodsList)[md->OwnDropIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (md->ActivityIndex !=DROPINDEX_NULL && md->ActivityIndex < (int)pDropGoodsList->size())
		{
			CMonsterList::tagDropGoods *drop = (*pDropGoodsList)[md->ActivityIndex];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

	}

	if( drop_flag != 1 )
	{
		map<int,int>::iterator ite=pRegionDropList->find(Level);
		if (ite!=pRegionDropList->end())
		{
			int index=(*ite).second;
			if (index>= 0 && index< (int)pDropGoodsList->size())
			{
				CMonsterList::tagDropGoods *drop = (*pDropGoodsList)[index];
				CheckOneDrop(drop,LevelBalances,vOut);
			}
		}

		if (Level>=0 && Level< (int)pLevelDropList->size())
		{
			int index=(*pLevelDropList)[Level];
			CMonsterList::tagDropGoods *drop = (*pDropGoodsList)[index];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

		if (pDropGoodsList->size()>0 && WorldDropIdx >=0 && WorldDropIdx < (int) pDropGoodsList->size())
		{
			CMonsterList::tagDropGoods *drop=(*pDropGoodsList)[WorldDropIdx];
			CheckOneDrop(drop,LevelBalances,vOut);
		}

	}
	return TRUE;
}
BOOL CMonster::CheckOneDrop(CMonsterList::tagDropGoods* pDrop,int LevelBalance, vector<CGoods*> &vOut)
{
	//1 ���Ƿ�������ɱ  2 �Ƿ��������� 3 �ȼ���
	//	CMonsterList::tagDropGoods* pDrop;//=CMonsterList::GetDropGoods(this->GetName());//GetProperty() -> strOrginName.c_str() );
	//##������Ʒ����ʼλ��
	LONG lDropGoodsStartPos = 0;
	if(pDrop )// bKilledByGuard == FALSE && 
	{
		for( size_t i = 0; i < pDrop -> vectorDrop.size(); i++ )
		{
			// ������Ʒ
			long lMaxOdds = pDrop->vectorDrop[i].lMaxOdds;
			long lOdds = pDrop->vectorDrop[i].lOdds;

			// ����ȼ�˥�� Kevin Lynx 4.7.2009 ����
			lOdds = CalLevelOdds( LevelBalance, lMaxOdds, lOdds, pDrop->vectorDrop[i].fLevelAttenuation, pDrop->vectorDrop[i].fLevelAttenuationLimit );

			if( lMaxOdds && random(lMaxOdds) < static_cast<LONG>( lOdds * GlobalSetup::GetSetup() -> fMonsterDropGoodsScale ) )
			{
				DWORD dwAmount = 1;
				// Ǯ
				if( pDrop->vectorDrop[i].lGoodsIndex == GoodsSetup::GetGoldCoinIndex() ||pDrop->vectorDrop[i].lGoodsIndex == GoodsSetup::GetSilverIndex())
				{
					dwAmount = pDrop->vectorDrop[i].lMinMoney + random(abs(pDrop->vectorDrop[i].lMaxMoney - pDrop->vectorDrop[i].lMinMoney));
				}
				//				vector<CGoods*> vOut;
#ifdef __MEMORY_LEAK_CHECK__
				CBaseObject::SetConstructFlag(59);
#endif
				CGoods *pEquipment = NULL;
				if( dwAmount == 1 )
				{
					pEquipment = CGoodsFactory::CreateGoods( pDrop->vectorDrop[i].lGoodsIndex ,2);
					if( pEquipment != NULL )
					{
						vOut.push_back( pEquipment );
					}
				}
				else
				{
					CGoodsFactory::CreateGoods( pDrop -> vectorDrop[i].lGoodsIndex, dwAmount, vOut,3 );
				}
				// װ�����伴���(+1, +2, etc)
				long lMinLevel = pDrop->vectorDrop[i].lMinLevel;
				long lMaxLevel = pDrop->vectorDrop[i].lMaxLevel;
				if( pEquipment != NULL && lMaxLevel - lMinLevel > 0 )
				{
					// [lMinLevel, lMaxLevel]
					long level = random( lMaxLevel - lMinLevel + 1 ) + lMinLevel;
					CGoodsFactory::SetEquipmentLevel( pEquipment, level );
				}

			}
		}

		// �̶���Ʒ����
		for( size_t i = 0; i < pDrop->vecFixedDrop.size(); ++ i )
		{
			CMonsterList::tagDropGoods::tagFixedDrop *pGoodsGroup = &pDrop->vecFixedDrop[i];
			// ����ȼ�˥������
			long lOdds = CalLevelOdds( LevelBalance, pGoodsGroup->lMaxOdds, pGoodsGroup->lOdds, pGoodsGroup->fLevelAttenuation, pGoodsGroup->fLevelAttenuationLimit );

			if( random( pGoodsGroup->lMaxOdds ) >= lOdds ) 
			{
				continue;
			}

			int BaseDropNum = pGoodsGroup->iMaxDropNum - pGoodsGroup->iMinDropNum ;
			int DropNum = random( BaseDropNum ) + pGoodsGroup->iMinDropNum;
			int MaxProbability = 0;
			for( size_t j = 0; j < pGoodsGroup->vecOneDropGroup.size(); ++ j )
			{
				MaxProbability += pGoodsGroup->vecOneDropGroup[j].lProbability ;
			}

			// build the range list
			RangeListType ranges;
			BuildRangeTable( pGoodsGroup->vecOneDropGroup, ranges );
			assert( ranges.size() == pGoodsGroup->vecOneDropGroup.size() );

			// create goods.
			for( int num = 0; num < DropNum; ++ num )
			{
				int probability = random( MaxProbability ) + 1;
				// query the goods index by this probability
				size_t index = QueryGoodsIndex( ranges, probability );
				assert( index < ranges.size() );	
				if( index < pGoodsGroup->vecOneDropGroup.size() )
				{
					CGoods *pGoods = CGoodsFactory::CreateGoods( pGoodsGroup->vecOneDropGroup[index].lGoodsIndex,4 );
					vOut.push_back( pGoods );
					
					// װ�����伴���(+1, +2, etc)
					long lMinLevel = pGoodsGroup->vecOneDropGroup[index].lMinLevel;
					long lMaxLevel = pGoodsGroup->vecOneDropGroup[index].lMaxLevel;
					if ( pGoods != NULL && lMaxLevel - lMinLevel > 0)
					{
						// [lMinLevel, lMaxLevel]
						long level = random( lMaxLevel - lMinLevel + 1 ) + lMinLevel;
						CGoodsFactory::SetEquipmentLevel( pGoods, level );
					}
				}
			}
		}
	}
	return FALSE;
}
