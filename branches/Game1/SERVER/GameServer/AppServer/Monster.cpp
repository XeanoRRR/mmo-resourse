#include "StdAfx.h"
#include ".\monster.h"
#include "ServerRegion.h"
#include "Player.h"
#include "Colony.h"
#include "Session\CSessionFactory.h"
#include "Goods\CGoods.h"
#include "Goods\CGoodsFactory.h"
#include "..\..\nets\netserver\message.h"
#include "..\setup\playerlist.h"
#include "..\setup\monsterlist.h"
#include "script/Script.h"
#include "script\VariableList.h"
#include "..\..\setup\globesetup.h"
#include "ai\aifactory.h"
#include "Message Packaging\CS2CContainerObjectMove.h"
#include <MMSystem.h>
#include <limits>
#include <math.h>
#include "BuffSkill/NewSkillFactory.h"
#include "BuffSkill/NewSkillBaseProperties.h"

#include "OrganizingSystem/GSTeam.h"
#include "OrganizingSystem/GameOrganizingCtrl.h"
#include "../../public/DeputyOccu.h"


set<string>  CMonster::m_mapOtherUpdateDirtyAttrData;

CMonster::CMonster(void)
{
	SetType(TYPE_MONSTER);
	SetIsRun(false);
	//##��ӻ�����ս������
	CMoveShape::InitSkills();

	InitNameValueMap();

	m_dwAICount = 0;	
	m_bLockAttackImmediately = false;

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
	m_tgMonsterProperty.byTamable = 0;
	//�����ѱ������
	m_tgMonsterProperty.dwMaxTameCount = 0;			
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
	m_dwTameAttemptCount		= 0;
	m_pVariableList = MP_NEW CVariableList;
	m_dwColonyID				= 0;
	m_wMemberType				= 2;
	m_bColonyState				=FALSE;
}

CMonster::~CMonster(void)
{
	ClearPassiveSkill();	
	ClearAllBuff();
	CAIFactory::DeleteAI(m_pAI);
	//ע�����Ը��¶�ʱ��
	CancelUpdateAttrTimer();
	MP_DELETE(m_pVariableList);

	DelectAllScripByShape(GetExID());
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
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(itr->second.pfValue);
				::_AddToByteArray(&byMonsterDirtyData, (float)sendValue);
				break;
			case 4:	// long
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
		setWriteDB.AddToByteArray(&byShapeDirtyData[0],byShapeDirtyData.size());
	setWriteDB.AddToByteArray( (WORD)lMonsterNum);
	if(byMonsterDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byMonsterDirtyData[0],byMonsterDirtyData.size());
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

	long minValue[7] = { 0x0, 0, 0, (long)-99999999.999f, -0x7FFFFFFF, -0x7FFF, 0};
	long maxValue[7] = { 0xFF, 0xFFFF, 0x7FFFFFFF, (long)99999999.999f, 0x7FFFFFFF, 0x7FFF, 1};

	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.lType=4; st.pValue=&m_lType;         st.minValue = minValue[4];st.maxValue = maxValue[4];      st.attrEnumValue= M_ATTR_TYPE;   m_mapNameValue["Type"] = st;		
	st.lType=4; st.pValue=&m_lGraphicsID;	st.minValue = minValue[4];st.maxValue = maxValue[4];  st.attrEnumValue=M_ATTR_GRAPHICSID;   m_mapNameValue["GraphicsID"]=st;	
	st.lType=4; st.pValue=&m_lRegionID;		st.minValue = minValue[4];st.maxValue = maxValue[4];    st.attrEnumValue=M_ATTR_REGIONID;   m_mapNameValue["RegionID"]=st;
	st.lType=3;	st.pfValue=&m_fPosX;		st.minValue = minValue[3];st.maxValue = maxValue[3];       st.attrEnumValue=M_ATTR_TILEX;   m_mapNameValue["TileX"]=st;			
	st.lType=3;	st.pfValue=&m_fPosY;		st.minValue = minValue[3];st.maxValue = maxValue[3];       st.attrEnumValue=M_ATTR_TILEY;   m_mapNameValue["TileY"]=st;	
	st.lType=3;	st.pfValue=&m_fDir;			st.minValue = minValue[3];st.maxValue = maxValue[3];         st.attrEnumValue=M_ATTR_DIR;   m_mapNameValue["Dir"]=st;			
	st.lType=4;	st.pValue=&m_lPos;			st.minValue = minValue[4];st.maxValue = maxValue[4];         st.attrEnumValue=M_ATTR_POS;   m_mapNameValue["Pos"]=st;			
	st.lType=1;	st.pwValue=&m_wState;		st.minValue = minValue[1];st.maxValue = maxValue[1];      st.attrEnumValue=M_ATTR_STATE;	m_mapNameValue["State"]=st;			
	st.lType=1;	st.pwValue=&m_wAction;		st.minValue = minValue[1];st.maxValue = maxValue[1];      st.attrEnumValue=M_ATTR_ACTION;	m_mapNameValue["Action"]=st;	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*��������*/
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.lType=2;	st.pdwValue = &m_tgMonsterProperty.dwIndex; st.minValue = minValue[2];st.maxValue = maxValue[2];                	    st.attrEnumValue=M_ATTR_INDEX;			m_mapNameValue["Index"]=st;		
	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwPicLevel;st.minValue = minValue[2];st.maxValue = maxValue[2];              		st.attrEnumValue=M_ATTR_PICLEVEL;		m_mapNameValue["PicLevel"]=st;			   
	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwNameColor;st.minValue = minValue[2];st.maxValue = maxValue[2];             		st.attrEnumValue=M_ATTR_NAMECOLOR;		m_mapNameValue["NameColor"]=st;			

	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwHpBarColor;st.minValue = minValue[2];st.maxValue = maxValue[2];            		st.attrEnumValue=M_ATTR_HPBARCOLOR;		m_mapNameValue["HpBarColor"]=st;		
	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwSoundID; st.minValue = minValue[2];st.maxValue = maxValue[2];              		st.attrEnumValue=M_ATTR_SOUNDID;		m_mapNameValue["SoundID"]=st;			
	st.lType=1;	st.pwValue  = &m_tgMonsterProperty.wLevel; st.minValue = minValue[1];st.maxValue = maxValue[1];										st.attrEnumValue=M_ATTR_LEVEL;			m_mapNameValue["Level"]=st;							
	st.lType=2; st.pdwValue  = &m_tgMonsterProperty.dwExp; st.minValue = minValue[2];st.maxValue = maxValue[2];                     st.attrEnumValue=M_ATTR_EXP;				 m_mapNameValue["Exp"]=st;				
	st.lType=2; st.pdwValue  = &m_tgMonsterProperty.dwOccuPoint; st.minValue = minValue[2];st.maxValue = maxValue[2];                     st.attrEnumValue=M_ATTR_OCCUPOINT;	m_mapNameValue["OccuPoint"]=st;				
	st.lType=0;	st.plValue	= &m_tgMonsterProperty.byCountry; st.minValue = minValue[0];st.maxValue = maxValue[0];                  st.attrEnumValue=M_ATTR_COUNTRY;			 m_mapNameValue["byCountry"]=st;			
	st.lType=0;	st.plValue	= &m_tgMonsterProperty.byTamable;  st.minValue = minValue[0];st.maxValue = maxValue[0];                 st.attrEnumValue=M_ATTR_TAMABLE;			 m_mapNameValue["byTamable"]=st;			
	st.lType=2; st.pdwValue	= &m_tgMonsterProperty.dwMaxTameCount;st.minValue = minValue[2];st.maxValue = maxValue[2];          	st.attrEnumValue=M_ATTR_MAXTAMECOUNT;		 m_mapNameValue["MaxTameCount"]=st;		

	st.lType=1; st.pwValue	= &m_tgMonsterProperty.wCountryContriValue;st.minValue = minValue[1];st.maxValue = maxValue[1];						st.attrEnumValue=M_ATTR_COUNTRYCONTRIVALUE;	 m_mapNameValue["CountryContriValue"]=st;
	st.lType=1;	st.pwValue = &m_tgMonsterProperty.wSPContriValue;st.minValue = minValue[1];st.maxValue = maxValue[1];							st.attrEnumValue=M_ATTR_SPCONTRIVALUE;		 m_mapNameValue["SPContriValue"]=st;		
	st.lType=2;	st.pdwValue  = (DWORD*)&m_tgMonsterProperty.eEType;st.minValue = minValue[2];st.maxValue = maxValue[2];          st.attrEnumValue=M_ATTR_ELETYPE_;			 m_mapNameValue["EleType"]=st;	        
	st.lType=2;	st.pdwValue = (DWORD*)&m_tgMonsterProperty.eNType; st.minValue = minValue[2];st.maxValue = maxValue[2];          st.attrEnumValue=M_ATTR_NPCTYPE_;			 m_mapNameValue["NpcType"]=st;         

	st.lType=1;	st.pwValue = &m_tgMonsterProperty.wBossLevel;st.minValue = minValue[1];st.maxValue = maxValue[1];	 m_mapNameValue["BossLevel"]=st;		
	st.lType=2;	st.pdwValue = (DWORD*)&m_tgMonsterProperty.eClass_; st.minValue = minValue[2];st.maxValue = maxValue[2];          st.attrEnumValue=M_ATTR_CLASS;			 m_mapNameValue["Class"]=st;         
	st.lType=1;	st.pwValue = &m_tgMonsterProperty.wRace; st.minValue = minValue[1];st.maxValue = maxValue[1];                       st.attrEnumValue=M_ATTR_RACE;				 m_mapNameValue["Race"]=st;	            
	st.lType=1; st.pwValue  = &m_tgMonsterProperty.wMoveRandomValue;st.minValue = minValue[1];st.maxValue = maxValue[1];        	st.attrEnumValue=M_ATTR_MOVERANDOMVALUE;	 m_mapNameValue["MoveRandomValue"]=st;	
	st.lType=1; st.pwValue = &m_tgMonsterProperty.wStopTime; st.minValue = minValue[1];st.maxValue = maxValue[1];                   st.attrEnumValue=M_ATTR_STOPTIME;			 m_mapNameValue["StopTime"]=st;	        
	st.lType=1; st.pwValue  = &m_tgMonsterProperty.wAIHangupType;st.minValue = minValue[1];st.maxValue = maxValue[1]; 	        	st.attrEnumValue=M_ATTR_AIHANGUPTYPE;			 m_mapNameValue["AIHangupType"]=st;		
	st.lType=1; st.pwValue = &m_tgMonsterProperty.wFigure; st.minValue = minValue[1];st.maxValue = maxValue[1];                     st.attrEnumValue=M_ATTR_FIGURE;				 m_mapNameValue["Figure"]=st;	        

	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwHp;  st.minValue = minValue[2];st.maxValue = maxValue[2];                  	st.attrEnumValue=M_ATTR_HP;					 m_mapNameValue["Hp"]=st;				
	st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIType; st.minValue = minValue[2];st.maxValue = maxValue[2];               	st.attrEnumValue=M_ATTR_AITYPE;				 m_mapNameValue["AIType"]=st;			

	st.lType=3;	st.pfValue = &m_tgMonsterProperty.fHurtProportion;st.minValue = minValue[3];st.maxValue = maxValue[3];          	st.attrEnumValue=M_ATTR_HURTPROPORTION;		 m_mapNameValue["fHurtProportion"]=st;	
	st.lType=2; st.pdwValue = &m_tgMonsterProperty.dwGuardTime;  st.minValue = minValue[2];st.maxValue = maxValue[2];           	st.attrEnumValue=M_ATTR_GUARDTIME;			 m_mapNameValue["GuardTime"]=st;			
	st.lType=3; st.pfValue = &m_tgMonsterProperty.fEscapePoint;st.minValue = minValue[3];st.maxValue = maxValue[3];             	st.attrEnumValue=M_ATTR_ESCAPEPOINT;		 m_mapNameValue["fEscapePoint"]=st;		
	st.lType=1; st.pwValue = &m_tgMonsterProperty.wSkillProportion;st.minValue = minValue[1];st.maxValue = maxValue[1];         	st.attrEnumValue=M_ATTR_SKILLPROPORTION;	 m_mapNameValue["SkillProportion"]=st;		
	st.lType=2; st.pdwValue = &m_tgMonsterProperty.dwSkillSpace;st.minValue = minValue[2];st.maxValue = maxValue[2];            	st.attrEnumValue=M_ATTR_SKILLSPACE;			 m_mapNameValue["SkillSpace"]=st;		
	st.lType=3;	st.pfValue	= &m_tgMonsterProperty.fMoveSpeed;	st.minValue = minValue[3];st.maxValue = maxValue[3];			st.attrEnumValue=M_ATTR_MOVESPEED;			m_mapNameValue["MoveSpeed"]=st;		
	st.lType=3;	st.pfValue	= &m_tgMonsterProperty.fFightMoveSpeed;	st.minValue = minValue[3];st.maxValue = maxValue[3];			st.attrEnumValue=M_ATTR_FIGHTMOVESPEED;			m_mapNameValue["FightMoveSpeed"]=st;		

		st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[0];st.minValue = minValue[2];st.maxValue = maxValue[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE0; m_mapNameValue["AIReferrence0"]=st; 
		st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[1];st.minValue = minValue[2];st.maxValue = maxValue[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE1; m_mapNameValue["AIReferrence1"]=st; 
		st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[2];st.minValue = minValue[2];st.maxValue = maxValue[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE2; m_mapNameValue["AIReferrence2"]=st; 
		st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[3];st.minValue = minValue[2];st.maxValue = maxValue[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE3; m_mapNameValue["AIReferrence3"]=st; 
		st.lType=2;	st.pdwValue	= &m_tgMonsterProperty.dwAIReferrence[4];st.minValue = minValue[2];st.maxValue = maxValue[2]; st.attrEnumValue=M_ATTR_AIREFERRENCE4; m_mapNameValue["AIReferrence4"]=st; 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*ս������*/ // B_:����ֵ, C_:��ǰֵ, E_:�仯ֵ
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.dwMaxHp;	st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_MAXHP;				m_mapNameValue["E_MaxHp"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wHpRecoverSpeed;st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_HPRECOVERSPEED;		m_mapNameValue["E_HpRecoverSpeed"]=st;	
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wMinAtk;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_MINATK;				m_mapNameValue["E_MinAtk"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wMaxAtk;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_MAXATK;				m_mapNameValue["E_MaxAtk"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wMinMAtk;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_MINMATK;			m_mapNameValue["E_MinMAtk"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wMaxMAtk;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_MAXMATK;			m_mapNameValue["E_MaxMAtk"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wHit;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_HIT;				m_mapNameValue["E_Hit"]=st;				
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lDef;		st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_DEF;				m_mapNameValue["E_Def"]=st;		
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lMDef;		st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_MDEF;				m_mapNameValue["E_MDef"]=st;		
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wDodge;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_DODGE;				m_mapNameValue["E_Dodge"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wBlock;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_BLOCK;				m_mapNameValue["E_Block"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wParry;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_PARRY;				m_mapNameValue["E_Parry"]=st;			
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lImmunity;		st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_IMMUNITY;				m_mapNameValue["E_Immunity"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wPierce;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_PIERCE;				m_mapNameValue["E_Pierce"]=st;			
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wFarAtkDis;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_FARATKDIS;			m_mapNameValue["E_FarAtkDis"]=st;		
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wNeaAtkDis;	st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_NEAATKDIS;			m_mapNameValue["E_NeaAtkDis"]=st;		
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wGuardRange;st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_GUARDRANGE;			m_mapNameValue["E_GuardRange"]=st;		
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wChaseRange;st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_CHASERANGE;			m_mapNameValue["E_ChaseRange"]=st;		
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lWillDef;	st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_WILLDEF;			m_mapNameValue["E_WillDef"]=st;			
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lConstiDef;	st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_CONSTIDEF;			m_mapNameValue["E_ConstiDef"]=st;		
	st.lType=5;	st.psValue	= &m_tgMonsterFightProperty.wCri;		st.minValue = minValue[5];st.maxValue = maxValue[5];			st.attrEnumValue=M_ATTR_CRI;				m_mapNameValue["E_Cri"]=st;				
	st.lType=5;	st.pValue	= &m_tgMonsterFightProperty.lCriDef;	st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_CRIDEF;				m_mapNameValue["E_CriDef"]=st;			
	st.lType=4;	st.pValue	= &m_tgMonsterFightProperty.lMCriDef;	st.minValue = minValue[4];st.maxValue = maxValue[4];			st.attrEnumValue=M_ATTR_MCRIDEF;			m_mapNameValue["E_MCriDef"]=st;			
	st.lType=1; st.pwValue = &m_tgMonsterProperty.wCanTalk; st.minValue = minValue[1];st.maxValue = maxValue[1];                     st.attrEnumValue=M_ATTR_CANTALK;				 m_mapNameValue["CanTalk"]=st;	        
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DFT_Bled];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_DEBUFFDEF_BLED;     m_mapNameValue["E_DeBuffDef0"]=st;      
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Dizzy];st.minValue = minValue[4];st.maxValue = maxValue[4];	st.attrEnumValue=M_ATTR_DEBUFFDEF_DIZZY;	m_mapNameValue["E_DeBuffDef1"]=st;	    
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Lull];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_DEBUFFDEF_LULL;		m_mapNameValue["E_DeBuffDef2"]=st;	    
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Hypnosis];st.minValue = minValue[4];st.maxValue = maxValue[4];	st.attrEnumValue=M_ATTR_DEBUFFDEF_HYPNOSIS;	m_mapNameValue["E_DeBuffDef3"]=st;  
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Poison];st.minValue = minValue[4];st.maxValue = maxValue[4];	st.attrEnumValue=M_ATTR_DEBUFFDEF_POSION;	m_mapNameValue["E_DeBuffDef4"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Fix];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_DEBUFFDEF_FIX;		m_mapNameValue["E_DeBuffDef5"]=st;	    
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lDeBuffDef[DET_Silence];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_DEBUFFDEF_SILENCE;		m_mapNameValue["E_DeBuffDef6"]=st;	    
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Fire];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_FIRE;   m_mapNameValue["E_ElemDef0"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Water];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_WATER;	m_mapNameValue["E_ElemDef1"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Earth];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_EARTH;	m_mapNameValue["E_ElemDef2"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Wind];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_WIND;	m_mapNameValue["E_ElemDef3"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Dark];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_DARK;	m_mapNameValue["E_ElemDef4"]=st;	
		st.lType=4;st.pValue = &m_tgMonsterFightProperty.lElemDef[ET_Holy];st.minValue = minValue[4];st.maxValue = maxValue[4];		st.attrEnumValue=M_ATTR_ELEMDEF_HOLY;	m_mapNameValue["E_ElemDef5"]=st;	
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Human];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_HUMAN;    m_mapNameValue["E_ClassDam0"]=st;		
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Element];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_ELEMENT;  m_mapNameValue["E_ClassDam1"]=st;	
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Alien];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_ALIEN;	   m_mapNameValue["E_ClassDam2"]=st;		
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Animal];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_ANIMAL;   m_mapNameValue["E_ClassDam3"]=st;		
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Undead];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_UNDEAD;   m_mapNameValue["E_ClassDam4"]=st;		
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Evil];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_CLASSDAM_EVIL;	   m_mapNameValue["E_ClassDam5"]=st;		
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wClassDam[C_Hexapods];st.minValue = minValue[5];st.maxValue = maxValue[5];	st.attrEnumValue=M_ATTR_CLASSDAM_HEXAPODS; m_mapNameValue["E_ClassDam6"]=st;	
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Fire];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_FIRE;   m_mapNameValue["E_ElemDam0"]=st;  
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Water];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_WATER;  m_mapNameValue["E_ElemDam1"]=st; 
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Earth];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_EARTH;  m_mapNameValue["E_ElemDam2"]=st; 
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Wind];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_WIND;   m_mapNameValue["E_ElemDam3"]=st;  
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Dark];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_DARK;   m_mapNameValue["E_ElemDam4"]=st;  
		st.lType=5;st.psValue = &m_tgMonsterFightProperty.wElemDam[ET_Holy];st.minValue = minValue[5];st.maxValue = maxValue[5];		st.attrEnumValue=M_ATTR_ELEMDAM_HOLY;   m_mapNameValue["E_ElemDam5"]=st;  
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*����ս������*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.dwMaxHp;st.minValue = minValue[4];st.maxValue = maxValue[4];				m_mapNameValue["B_MaxHp"]=st;				//��������			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wHpRecoverSpeed;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_HpRecoverSpeed"]=st;				//��������			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wMinAtk;st.minValue = minValue[5];st.maxValue = maxValue[5];			m_mapNameValue["B_MinAtk"]=st;			//��������			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wMaxAtk;st.minValue = minValue[5];st.maxValue = maxValue[5];			m_mapNameValue["B_MaxAtk"]=st;			//��������			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wMinMAtk;st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_MinMAtk"]=st;		//�����ָ��ٶ�			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wMaxMAtk;st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_MaxMAtk"]=st;		//�����ָ��ٶ�			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wHit;st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_Hit"]=st;		//�����ָ��ٶ�			
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lDef;st.minValue = minValue[4];st.maxValue = maxValue[4];				m_mapNameValue["B_Def"]=st;
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lMDef;st.minValue = minValue[4];st.maxValue = maxValue[4];				m_mapNameValue["B_MDef"]=st;	//ħ������
	//����			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wDodge;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_Dodge"]=st;				//����			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wBlock;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_Block"]=st;				//����		
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wParry;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_Parry"]=st;				//����
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lImmunity;		st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_Immunity"]=st;			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wPierce;		st.minValue = minValue[5];st.maxValue = maxValue[5];	m_mapNameValue["B_Pierce"]=st;			

	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wFarAtkDis;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_FarAtkDis"]=st;				//�м�			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wNeaAtkDis;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_NeaAtkDis"]=st;				//����		
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wGuardRange;st.minValue = minValue[5];st.maxValue = maxValue[5];					m_mapNameValue["B_GuardRange"]=st;					//����
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wChaseRange;st.minValue = minValue[5];st.maxValue = maxValue[5];				m_mapNameValue["B_ChaseRange"]=st;				//רע			
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lWillDef;st.minValue = minValue[4];st.maxValue = maxValue[4];					m_mapNameValue["B_WillDef"]=st;					//�������	
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lConstiDef;st.minValue = minValue[4];st.maxValue = maxValue[4];					m_mapNameValue["B_ConstiDef"]=st;					//ħ������			
	st.lType=5;	st.psValue	= &m_tgBaseMonsterFightProperty.wCri;st.minValue = minValue[5];st.maxValue = maxValue[5];					m_mapNameValue["B_Cri"]=st;					//������			
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lCriDef;st.minValue = minValue[4];st.maxValue = maxValue[4];			m_mapNameValue["B_CriDef"]=st;			//ʩ���ٶ�			
	st.lType=4;	st.pValue	= &m_tgBaseMonsterFightProperty.lMCriDef;st.minValue = minValue[4];st.maxValue = maxValue[4];			m_mapNameValue["B_MCriDef"]=st;			//Buff����ǿ��		

	{
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DFT_Bled];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef0"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Dizzy];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef1"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Lull];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef2"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Hypnosis];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef3"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Poison];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef4"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Fix];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef5"]=st;			//Debuff����
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lDeBuffDef[DET_Silence];st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["B_DeBuffDef6"]=st;			//Debuff����
	}

	{
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Fire];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef0"]=st;
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Water];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef1"]=st;
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Earth];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef2"]=st;
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Wind];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef3"]=st;
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Dark];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef4"]=st;
		st.lType=4;st.pValue = &m_tgBaseMonsterFightProperty.lElemDef[ET_Holy];st.minValue = minValue[4];st.maxValue = maxValue[4];		m_mapNameValue["B_ElemDef5"]=st; //����Ԫ�ؿ���
	}

	{
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Human];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam0"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Element];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam1"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Alien];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam2"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Animal];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam3"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Undead];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam4"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Evil];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ClassDam5"]=st;
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wClassDam[C_Hexapods];st.minValue = minValue[5];st.maxValue = maxValue[5];	m_mapNameValue["B_ClassDam6"]=st;;//�Ը��������˺�
	}

	{
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Fire];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam0"]=st;			//�Ը���Ԫ���˺�
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Water];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam1"]=st;			//�Ը���Ԫ���˺�
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Earth];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam2"]=st;			//�Ը���Ԫ���˺�
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Wind];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam3"]=st;			//�Ը���Ԫ���˺�
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Dark];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam4"]=st;			//�Ը���Ԫ���˺�
		st.lType=5;st.psValue = &m_tgBaseMonsterFightProperty.wElemDam[ET_Holy];st.minValue = minValue[5];st.maxValue = maxValue[5];		m_mapNameValue["B_ElemDam5"]=st;			//�Ը���Ԫ���˺�

	}
	{
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Earth_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Earth].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Earth_Defend"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Water_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Water].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Water_Defend"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Fire_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Fire].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Fire_Defend"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Wind_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Wind].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Wind_Defend"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Bright_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Bright].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Bright_Defend"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Attack];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Dark_Attack"]=st;
		st.lType=4;st.pValue = &m_arrElementProperty[ME_Dark].uArrPro[AD_Defend];	st.minValue = minValue[4];st.maxValue = maxValue[4];	m_mapNameValue["Dark_Defend"]=st;
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
			return *(itr->second.pfValue);

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
	if (byShapeBlockType==CRegion::BLOCK_NO ) return;
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
		stRunScript st;
		st.pszFileName   = (char*)(ScriptName);
		st.srcShape  = this;
		st.desShape	 = this;
		st.pRegion   = dynamic_cast<CRegion*>(GetFather());
		RunScript(&st,(char*)GetGame()->GetScriptFileData(st.pszFileName));	
		return 0;
	}
	return false;
}

void CMonster::Init()
{
	InitSkills();//���ܳ�ʼ
	InitAI();	//AI��ʼ
	if (m_pAI!=NULL)
	{
		m_pAI->OnBoring();
	}
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
	return true;
}

// ����
bool CMonster::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);
	SetHP(setReadDB.GetDwordFromByteArray());
	return true;
}

VOID CMonster::InitCycleSkill()
{
	for ( int a=0 ; a< m_vCycleSkills.size(); a++)
	{
		m_vCycleSkills[a].dwCoolDownTime=timeGetTime();
	}
}

// //�Ƿ����ѱ�� �����ӿ�
BOOL CMonster::IsTamable()
{
	BOOL bResult = FALSE;

	tagMonsterProperty *pProperties = &m_tgMonsterProperty;

	if (pProperties && pProperties -> byTamable == 1 && 
		m_dwTameAttemptCount < pProperties -> dwMaxTameCount )
	{
		bResult = TRUE;
	}
	return bResult;
}

VOID CMonster::IncreaseTameAttemptCount()
{
	++ m_dwTameAttemptCount;
}

void CMonster::AddSkill(CMonsterList::tagSkillInfo& info,SKILLTYPE skilltype )
{
	tagSkillRecord tgSklll;
	CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(info.dwSkillID);
	if (pSkillProperty== NULL)	return;
	CNewSkillProperties::tagSkill *ptgSkill = pSkillProperty->GetNewSkill(info.dwSkillLevel);
	if (ptgSkill== NULL)	return;
	if (ptgSkill->pStaticParam== NULL) return;

	tgSklll.wID = info.dwSkillID;
	tgSklll.wLevel = info.dwSkillLevel;
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
			tgSklll.dwCool = ( info.dwCoolDownTime == 0 ? ptgSkill->pStaticParam->lCoolDownTime :
				info.dwCoolDownTime );
		}
		else
		{
			tgSklll.dwCool = ptgSkill->pStaticParam->lCoolDownTime;
		}
	}

	if (skilltype== SKILLTYPE_BASE) 
		m_vBaseSkills.push_back(tgSklll);
	else if (skilltype== SKILLTYPE_NORMAL) 
		m_vSkills.push_back(tgSklll);
	else if (skilltype== SKILLTYPE_CYCLE) 
		m_vCycleSkills.push_back(tgSklll);

	CountAtkRange(ptgSkill->pStaticParam->lMaxAtkDistance,ptgSkill->pStaticParam->lMinAtkDistance);
}

bool CMonster::DelSkill(long id)
{
	if (DelSkillOnList(m_vBaseSkills,id) || DelSkillOnList(m_vSkills,id) || DelSkillOnList(m_vCycleSkills ,id))
	{
		SetNeaAtkDis(0);
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
		CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(id);
		if (pSkillProperty== NULL)	return;
		CNewSkillProperties::tagSkill *ptgSkill = pSkillProperty->GetNewSkill(lv);
		if (ptgSkill== NULL)	return;
		if (ptgSkill->pStaticParam== NULL) return;
		CountAtkRange(ptgSkill->pStaticParam->lMaxAtkDistance, ptgSkill->pStaticParam->lMinAtkDistance);
	}
}

void CMonster::CountAtkRange(int iRagMax,int iRagMin)
{
	if (iRagMin>=0 && iRagMax>=iRagMin)
	{
		int max=GetAckRangeMax();
		int min=GetAckRangeMin();
		if (max==0 && min==0)
		{
			SetNeaAtkDis(iRagMin);
			SetFarAtkDis(iRagMax);
		}
		else
		{
			if (min>iRagMin)
			{
				SetNeaAtkDis(iRagMin);
			}
			if (max< iRagMax)
			{
				SetFarAtkDis(iRagMax);
			}
		}
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
	MP_DELETE(m_pAI);
	m_pAI=CAIFactory::CreateAI(this);
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
	this ->SetOccuPoint(p->dwOccuPoint);
	this ->SetCountry(p->byCountry );
	this ->SetTamable(p->byTamable);
	this ->SetMaxTameCount(p->dwMaxTameCount);
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
	this ->SetReturnRange(p->wReturnRange);
	this->SetPeaceMoveRange(p->wPeaceMoveRange);

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
	this ->SetBaseDef(p->lDef);
	this ->SetBaseMDef(p->lMDef);
	this ->SetBaseDodge(p->wDodge);
	this ->SetBaseBlock(p->wBlock);
	this ->SetBaseParry(p->wParry);
	this ->SetMoveSpeed(p->fMoveSpeed);
	this ->SetFightMoveSpeed(p->fFightMoveSpeed);
	this ->SetTurningSpeed(p->fTurningSpeed);
	this ->SetBaseGuardRange(p->wGuardRange);
	this ->SetBaseChaseRange(p->wChaseRange);
	this ->SetBaseWillDef(p->lWillDef);
	this ->SetBaseConstiDef(p->lBaseConstiDef);
	this->SetBaseImmunity( p->lImmunity );
	this->SetBasePierce( p->wPierce );

	this ->SetBaseCri(p->wCri);
	this ->SetBaseCriDamg(p->wCriDamg);
	this ->SetBaseCriDef(p->lCriDef);
	this ->SetBaseMCri(p->wMCri);
	this ->SetBaseMCriDamg(p->wMCriDamg);
	this ->SetBaseMCriDef(p->lMCriDef);

	for(a=0; a<DET_Max; a++)
	{
		this->SetBaseDeBuffDef (a,p->lDeBuffDef [a]);
	}
	for( a=0; a<ET_Max; a++)
	{
		this->SetBaseElemDef(a,p->lElemDef[a]);
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
	SetNeaAtkDis(0);
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
		//##���Ҹù�������Խṹ
		CMonsterList::tagMonster* pMonster = NULL;
		DWORD dwGetExp = GetExpDead();
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
				float parm1 = pPlayer->GetLevel() - GetLevel();
				if(0 > parm1)
					parm1 = (CGlobeSetup::GetSetup()->new_fExpMinLvlDiff < -parm1) ? (parm1 + CGlobeSetup::GetSetup()->new_fExpMinLvlDiff) : 0;

				float fExp = dwGetExp;
				float fCoefficient = (0 > parm1) ? CGlobeSetup::GetSetup()->new_fExpHortation : CGlobeSetup::GetSetup()->new_fExpAmerce;
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
		//##���Ҹù�������Խṹ
		CMonsterList::tagMonster* pMonster = NULL;
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
				float parm1 = CPlayerList::GetOccuLvlCoe(byOccu,pPlayer->GetOccuLvl(byOccu)) - GetLevel();
				if(0 > parm1)
					parm1 = (CGlobeSetup::GetSetup()->vigour_fExpMinLvlDiff < -parm1) ? (parm1 + CGlobeSetup::GetSetup()->vigour_fExpMinLvlDiff) : 0;	
				float fExp = dwGetExp;
				float fCoefficient = (0 > parm1) ? CGlobeSetup::GetSetup()->vigour_fExpHortation : CGlobeSetup::GetSetup()->vigour_fExpAmerce;
				fExp *= (float)(1.0f - parm1 * fCoefficient / 100.0f);
				fExp = fExp * CGlobeSetup::GetSetup()->vigour_fOccExpSpRatio;
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
		//##���Ҹù�������Խṹ
		CMonsterList::tagMonster* pMonster = NULL;
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
				float parm1 = CPlayerList::GetOccuLvlCoe(byOccu,pPlayer->GetOccuLvl(byOccu)) - GetLevel();
				if(0 > parm1)
					parm1 = (CGlobeSetup::GetSetup()->vigour_fExpMinLvlDiff < -parm1) ? (parm1 + CGlobeSetup::GetSetup()->vigour_fExpMinLvlDiff) : 0;
				float fExp = dwGetExp;
				float fCoefficient = (0 > parm1) ? CGlobeSetup::GetSetup()->vigour_fExpHortation : CGlobeSetup::GetSetup()->vigour_fExpAmerce;
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
		pGSTeam->GetLucrativeArea_Alive(RegionTeamates, pRegion->GetExID(), GetPosX(), GetPosY(), FALSE);
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

		//##�ҵ����ɱ���ù������?
		if( pBeneficiary )
		{
			//##����ҵ����ɱ���ù������,û������,��9��Area�ķ�Χ��
			if( pBeneficiary -> IsDied() == FALSE &&
				Distance( pBeneficiary ) <= CGlobeSetup::GetSetup() -> new_lExperienceRadii )
			{
				//##�ɹ��ҵ������
			}
			//##�������Ҳ��������������������ж���
			else if( NULL_GUID != pBeneficiary -> GetTeamID() )
			{			
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
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion == NULL ) return;

	//##��ȡȫ�ֵ�����
	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();
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
		CMonsterAI *pAI = static_cast<CMonsterAI*>( GetAI() );
		if( pAI != NULL )
		{
			bKilledByGuard = pAI->IsAttackedByGuard() ? TRUE : FALSE;
		}
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
					pGSTeam->GetLucrativeArea_Alive(RegionTeamates, pRegion->GetExID(), GetPosX(), GetPosY(), FALSE);
					assert(1 != RegionTeamates.size());

					for (map<CGUID,CPlayer*>::iterator ite = RegionTeamates.begin(); ite != RegionTeamates.end(); ++ite)
					{						
						//CPlayer* pMember = dynamic_cast<CPlayer*>( pPlug -> GetOwner() );
						//##��������ڵĳ�Ա��Ȼ����9��Area��Χ�ڣ���û�������Ļ�
						CPlayer* pMember = ite->second;
						if( pMember && pMember -> IsDied() == FALSE )
						{
							//���侭��
							DWORD dwExperience = CalculateExperienceQuota( pMember, GetPosX(), GetPosY() );
							dwExperience = static_cast<DWORD>( dwExperience * pSetup -> fExpScale );
							//���ϳ����������ϵ��
							dwExperience = static_cast<DWORD>( dwExperience * pRegion->GetExpScale());												
							// ����ɫ�ϻ�þ��鱶��
							dwExperience = (DWORD)( (float)dwExperience * pMember->GetExpScale()/100 );
							//��ȡ���;���
							dwExperience += pMember->AcquirePresentExp(dwExperience);
							//��Ʒ���Ӿ���
							pMember->SetGoodsExtraExp(dwExperience);
							// ���վ����ȡ
							pMember -> IncExp( dwExperience , eAET_Monster);
							//pMember -> AddIncExpLog(pMember->GetExp(), pMember->GetExpMultiple(), dwExperience, 0, eAET_Monster, GetName());
							pMember->AllEquipmentRefineUpdate(dwExperience);
							pMember -> CheckLevel();

							//����ְ������
							eOccupation eOccu = (eOccupation)pMember->GetOccupation();
							DWORD dwOccuExp = CalculateOccuExpQuota(pMember, GetPosX(), GetPosY());
							dwOccuExp = static_cast<DWORD>( dwOccuExp * pSetup -> fOccuExpScale );
							//���ϳ���ְ���������ϵ��
							dwOccuExp = static_cast<DWORD>( dwOccuExp * pRegion->GetOccuExpScale());												
							// ����ɫ�ϻ��ְ�����鱶��
							dwOccuExp = (DWORD)( (float)dwOccuExp * pMember->GetOccuExpScale()/100 );
							DWORD dwOldOccuExp = pMember->GetOccuExp(eOccu);
							//���������ְҵ����
							pMember->IncOccuExp(eOccu,dwOccuExp,eAET_Monster);
							//pMember->AddIncOccuExpLog(eOccu,dwOldOccuExp, dwOccuExp , eAET_Monster, GetName());
							pMember->CheckOccuLevel(eOccu);
							//����Ԫ��
							DWORD dwSkillPoint = CalculateOccuPointQuota( pMember, GetPosX(), GetPosY() );
							//����ȫ�ַ���ϵ��
							dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pSetup -> fSpScale );
							//���ϳ���Ԫ������ϵ��
							dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pRegion->GetSpScale());
							// ����ɫ�ϻ�ü��ܵ�������
							dwSkillPoint = (DWORD)( (float)dwSkillPoint * pMember->GetSpScale()/100 );
							// ���ռ��ܵ�����ȡ
							pMember -> IncOccuSp( eOccu,dwSkillPoint,4);

						}
					}
				}
			}
			else
			{
				//##û�����
				DWORD dwExperience = CalculateExperienceQuota( pBeneficiary, GetPosX(), GetPosY() );
				dwExperience = static_cast<DWORD>( dwExperience * pSetup -> fExpScale );
				//���ϳ����������ϵ��
				dwExperience = static_cast<DWORD>( dwExperience * pRegion->GetExpScale());	
				// ����ɫ�ϻ�þ��鱶��
				dwExperience = (DWORD)( (float)dwExperience * pBeneficiary->GetExpScale()/100);						
				//��ȡ���;���
				dwExperience += pBeneficiary->AcquirePresentExp(dwExperience);
				//��Ʒ���Ӿ���
				pBeneficiary->SetGoodsExtraExp(dwExperience);
				pBeneficiary -> IncExp( dwExperience, eAET_Monster);
				//pBeneficiary -> AddIncExpLog(pBeneficiary->GetExp(), pBeneficiary->GetExpMultiple(), dwExperience, 0, eAET_Monster, GetName());
				pBeneficiary->AllEquipmentRefineUpdate(dwExperience);
				pBeneficiary -> CheckLevel();
				//����ְ������
				eOccupation eOccu = (eOccupation)pBeneficiary->GetOccupation();
				DWORD dwOccuExp =CalculateOccuExpQuota(pBeneficiary, GetPosX(), GetPosY());
				dwOccuExp = static_cast<DWORD>( dwOccuExp * pSetup -> fOccuExpScale );
				//���ϳ���ְ���������ϵ��
				dwOccuExp = static_cast<DWORD>( dwOccuExp * pRegion->GetOccuExpScale());												
				// ����ɫ�ϻ��ְ�����鱶��
				dwOccuExp = (DWORD)( (float)dwOccuExp * pBeneficiary->GetOccuExpScale()/100 );
				DWORD dwOldOccuExp = pBeneficiary->GetOccuExp(eOccu);
				//���������ְҵ����
				pBeneficiary->IncOccuExp(eOccu,dwOccuExp,eAET_Monster);
				//pBeneficiary -> AddIncOccuExpLog(eOccu,dwOldOccuExp, dwOccuExp , eAET_Monster, GetName());
				pBeneficiary->CheckOccuLevel(eOccu);
				//����Ԫ��
				DWORD dwSkillPoint = CalculateOccuPointQuota( pBeneficiary, GetPosX(), GetPosY() );
				//����ȫ�ַ���ϵ��
				dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pSetup -> fSpScale );
				//���ϳ���Ԫ������ϵ��
				dwSkillPoint = static_cast<DWORD>( dwSkillPoint * pRegion->GetSpScale());
				// ����ɫ�ϻ�ü��ܵ�������
				dwSkillPoint = (DWORD)( (float)dwSkillPoint * pBeneficiary->GetSpScale()/100 );
				// ���ռ��ܵ�����ȡ
				pBeneficiary -> IncOccuSp( eOccu,dwSkillPoint,5);
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
			CMonsterList::CheckDropGoods(GetOriginName(),GetRegionID(),GetLevel(),pKiller->GetLevel()-GetLevel(),vOut, drop_flag);
		}
		else if (pMonster!= NULL && pMonster->GetNpcType()==NT_Normal)
		{
			CMonsterList::CheckDropGoods(GetOriginName(),GetRegionID(),GetLevel(),0,vOut, drop_flag);
		}

		for( int sGoods = 0; sGoods < vOut.size(); sGoods ++ )
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
				stRunScript rsScript;
				rsScript.pszFileName	= static_cast<CHAR*>( const_cast<CHAR*>( GetScriptFile() ) );
				rsScript.srcShape	= pBeneficiary;
				rsScript.pRegion	= pRegion;
				rsScript.desShape		= this;
				rsScript.dwDiedMonsterIndex =  GetIndex();
				RunScript( &rsScript, static_cast<CHAR*>( const_cast<CHAR*>( GetGame()->GetScriptFileData( GetScriptFile() ) ) ) );
			}
		}
		if (RegionTeamates.size()==0)
		{
			//##����������
			CPlayer* pBeneficiary = GetBeneficiary();
			if( !m_strScriptFile.empty() && pBeneficiary )
			{
				stRunScript rsScript;
				rsScript.pszFileName	= static_cast<CHAR*>( const_cast<CHAR*>( GetScriptFile() ) );
				rsScript.srcShape	= pBeneficiary;
				rsScript.pRegion	= pRegion;
				rsScript.desShape		= this;
				rsScript.dwDiedMonsterIndex =  GetIndex();
				RunScript( &rsScript, static_cast<CHAR*>( const_cast<CHAR*>( GetGame()->GetScriptFileData( GetScriptFile() ) ) ) );
			}
		}
	}

	//##ɾ��
	//##����ˢ���б��е��������
	CServerRegion::tagMonster* pRefresh =NULL;	
	if( pRefresh && pRefresh -> lLivingCount > 0)
	{
		pRefresh -> lLivingCount --;
	}

	LogoutDelEvent();
	ClearActiveSkills();
	EndBuffAndDebuff();

	//�ɼ����ʼ��
	const CCollectionLayout* p=CDeputyOccuSystem::GetCollectionLayout(GetCollectionName().c_str());
	if (p!=NULL)
	{
		SetCollectionID(p->GetCollectionID());
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

VOID CMonster::OnBeenHurted( LONG lType, const CGUID& AttackerGuid ) 
{
	//##��¼��һ�������Լ�����
	//##FindChildObject()������ƿ�����ᵼ�·��������س���
	CGUID AttackPlayerID=AttackerGuid;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if(pRegion== NULL) 
		return;
	
	CMoveShape* pMoveShape= dynamic_cast<CMoveShape*>(pRegion->FindChildObject(lType,AttackPlayerID));
	if (pMoveShape== NULL) 
		return ;

	CPlayer* pPlayer = 0;

	if (lType== TYPE_PLAYER)
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
	if ( GetBeneficiaryID()==NULL_GUID || timeGetTime()- GetBeneficiaryTime() > CGlobeSetup::GetSetup()->dwAttackMonsterProtectedTimer
		|| bGuardAttacker )
	{
		// ��������������������������������������
		if( lType == TYPE_PLAYER || lType == TYPE_TEAM )
		{
			CMonsterAI *pAI = static_cast<CMonsterAI*>( GetAI() );
			if( pAI != NULL && pAI->IsAttackedByGuard() )
			{
				return;
			}
		}

		if (!CheckDelete())
			StartRecordBeneficiary(AttackPlayerID,(OBJECT_TYPE)lType,0);//
	}
	else if (GetBeneficiaryID()==AttackPlayerID)
	{
		RecordBeneficiaryTime();
	}
}

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
			bResult = true;
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





void CMonster::ExpAction(long ActID,long DesType,CGUID DesGUID)
{
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
	if (timeGetTime() > GetBeneficiaryTime()+ CGlobeSetup::GetSetup()->dwBeneficiaryTime)
	{
		CleanBeneficiary();
		CheckDelete();
	}
	else
	{
		AddBeneficiaryEvent(GetBeneficiaryTime()+ CGlobeSetup::GetSetup()->dwBeneficiaryTime);
	}
}

bool CMonster::CheckDelete()
{
	if (GetRoleProtect())
	{
		CServerRegion* pServerRegion = dynamic_cast<CServerRegion*>(GetFather() );//Ѱ�����ڳ���

		if(pServerRegion)
		{
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
		AddBeneficiaryEvent(CGlobeSetup::GetSetup()->dwBeneficiaryTime);
	else
		AddBeneficiaryEvent(time);
}

BOOL CMonster::ChangeAI()
{
	if (GetAI()->GetAIType()==GetAIType())
	{
		return TRUE;
	}
	int AIType=GetAIType();
	CBaseAI* pAI=CAIFactory::CreateAI(AIType);

	if (pAI==0)
	{
		SetAIType(GetAI()->GetAIType());
		return FALSE;
	}
	*pAI=*m_pAI;

	CBaseAI* point=m_pAI;

	m_pAI=pAI;

	CAIFactory::DeleteAI(point);

	return TRUE;
}

VOID CMonster::ResumeAll()
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
	}
	SetState(CShape::STATE_PEACE);
	if (GetAIType()!= m_pAI->GetAIType())
	{
		CBaseAI* ai=CAIFactory::CreateAI(GetAIType());
		*ai=*m_pAI;
		CBaseAI* point=m_pAI;
		m_pAI=ai;
		CAIFactory::DeleteAI(point);
	}
}

BOOL CMonster::Relive()
{
	return dynamic_cast<CServerRegion*>(this->GetFather())->AddOneMonster(this);
}
//////////////////////////////////////////////////////////////////////////
//���ù���ս������
//////////////////////////////////////////////////////////////////////////

//�����������
void CMonster::SetNeaAtkDis(WORD l)	
{

	m_tgMonsterFightProperty.wNeaAtkDis = l;
}
//��Զ��������
void CMonster::SetFarAtkDis(WORD l)		
{
	m_tgMonsterFightProperty.wFarAtkDis = l;
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
void CMonster::SetGuardRange(WORD l)	
{
	m_tgMonsterFightProperty.wGuardRange = l;
}
//׷����Χ
void CMonster::SetChaseRange(WORD l)	
{
	m_tgMonsterFightProperty.wChaseRange = l;
}	
//��������
void CMonster::SetHit(WORD l)			
{
	m_tgMonsterFightProperty.wHit = l;
}
//������
void CMonster::SetDef(long l)			
{
	m_tgMonsterFightProperty.lDef = l;
}
//ħ��������
void CMonster::SetMDef(long l)
{

	m_tgMonsterFightProperty.lMDef = l;
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
void CMonster::SetWillDef(long l)				
{
	m_tgMonsterFightProperty.lWillDef = l;
}
//���ʿ���
void CMonster::SetConstiDef(long l)			
{
	m_tgMonsterFightProperty.lConstiDef = l;
}
//Debuff����
void CMonster::SetDeBuffDef(WORD lpos,long l)			
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>DET_Max) lpos = DET_Max -1;
	m_tgMonsterFightProperty.lDeBuffDef[lpos] = l;
}
//����Ԫ�ؿ���
void CMonster::SetElemDef(WORD lpos,long l)			
{
	if(lpos<0) {lpos = 0;}else if((lpos+1)>ET_Max) lpos = ET_Max -1;
	m_tgMonsterFightProperty.lElemDef[lpos] = l;
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
void CMonster::SetCriDef(long l)						{m_tgMonsterFightProperty.lCriDef = l;}
//ħ��������
void CMonster::SetMCri(WORD l)							{m_tgMonsterFightProperty.wMCri = l;}
//ħ�������˺�
void CMonster::SetMCriDamg(WORD l)						{m_tgMonsterFightProperty.wMCriDamg = l;}
//ħ����������
void CMonster::SetMCriDef(long l)						{m_tgMonsterFightProperty.lMCriDef = l;}
void CMonster::SetCanTalk(WORD l)						
{
	m_mapDirtyAttrData.insert("CanTalk");
	m_tgMonsterProperty.wCanTalk=l;
}

VOID CMonster::AddToColony(DWORD dwColonyID, WORD wMemberType)
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


CVariableList* CMonster::GetVariableList()
{
	return m_pVariableList;
}

int CMonster::SetScriptVarValue(CScript* scriptPtr, const char* varName, const char* value)
{
	int retFlag = GetVariableList()->SetVarValue(varName, value);
	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
		retFlag = scriptPtr->GetVariableList()->SetVarValue(varName, value);
	if(retFlag == CVariableList::VAR_NOT_FOUND)
	{
		if(CScript::GetGeneralVariableList())
			retFlag = CScript::GetGeneralVariableList()->SetVarValue(varName, value);
		if(1 == retFlag)
			CScript::UpdateToWorldServer(varName, value);
	}

	return retFlag;
}
int CMonster::SetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray, double value)
{
	int retFlag = GetVariableList()->SetVarValue(varName, lArray, value);
	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
		retFlag = scriptPtr->GetVariableList()->SetVarValue(varName, lArray, value);
	if(retFlag == CVariableList::VAR_NOT_FOUND)
	{
		if(CScript::GetGeneralVariableList())
			retFlag = CScript::GetGeneralVariableList()->SetVarValue(varName, lArray, value);
		if(1 == retFlag)
			CScript::UpdateToWorldServer(varName, lArray, value);
	}

	return retFlag;
}

const char* CMonster::GetScriptVarValue(CScript* scriptPtr, const char* varName)
{
	const char* retStr = GetVariableList()->GetVarValue(varName);
	if(retStr == NULL && scriptPtr)
		retStr = scriptPtr->GetVariableList()->GetVarValue(varName);
	if(retStr == NULL)
	{
		if(CScript::GetGeneralVariableList())
			retStr = CScript::GetGeneralVariableList()->GetVarValue(varName);
	}

	return retStr;
}
double CMonster::GetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray)
{
	double retFlag = GetVariableList()->GetVarValue(varName, lArray);
	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
		retFlag = scriptPtr->GetVariableList()->GetVarValue(varName, lArray);
	if(retFlag == CVariableList::VAR_NOT_FOUND)
	{
		if(CScript::GetGeneralVariableList())
			retFlag = CScript::GetGeneralVariableList()->GetVarValue(varName, lArray);
	}

	return retFlag;
}
int CMonster::SetScriptGuidValue(CScript* scriptPtr, const char* varName, const CGUID& guid)
{
	int retFlag = GetVariableList()->SetGuidValue(varName, &guid);
	if(retFlag == CVariableList::VAR_NOT_FOUND && scriptPtr)
		retFlag = scriptPtr->GetVariableList()->SetGuidValue(varName, &guid);
	if(retFlag == CVariableList::VAR_NOT_FOUND)
	{
		if(CScript::GetGeneralVariableList())
			retFlag = CScript::GetGeneralVariableList()->SetGuidValue(varName, &guid);
	}

	return retFlag;
}
const CGUID& CMonster::GetScriptGuidValue(CScript* scriptPtr, const char* varName)
{
	return scriptPtr->GetScriptGuidValue( this, varName );
}

BOOL CMonster::BeenCollectedAll()
{
	CMonsterAI* pMonsterAI= dynamic_cast<CMonsterAI*>(m_pAI);
	if (GetState()==CShape::STATE_DIED && pMonsterAI!= NULL)
	{
		pMonsterAI->LogoutCurrentEvent();
		pMonsterAI->OnCheckDeadState();
		return TRUE;
	}
	return FALSE;
}