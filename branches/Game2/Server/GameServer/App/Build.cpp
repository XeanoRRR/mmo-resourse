////////////////////////////////////////////////////////////////////////////
//build.cpp///////////////////////////////////////////////////////////////////
//�����࣬�������������ͷ�������AI
//Author:Joe
//Create Time:2004.721
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "Build.h"
#include "ServerRegion.h"
//#include "Script/Script.h"
#include "Player.h"
#include "Monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
class CPlayer;
CBuild::CBuild(void)
{
	InitNameValueMap();
	m_lIndex=0;	
	m_fPosX=0.0;
	m_fPosY=0.0;					
	m_lMaxHP=0;				
	m_lHP=0;			
	m_lDef=0;			
	m_lMagicDef=0;			
	//lElementResistant=0;	
	//lWidthInc=0;			
	//lHeightInc=0;		
	m_lNameColor=0;			
	m_lLenth=0;			
	m_lWidth=0;		
	m_lLevel=0;		
	m_strAttack="0";		
	m_strDestory="0";		
	m_strRefresh="0";		
	//##��ӻ�����ս������
	CMoveShape::InitSkills();
}

CBuild::~CBuild(void)
{
}

// ��ӵ�CByteArray
//bool CBuild::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
//{
//
//	CMoveShape::AddToByteArray_ForClient( pByteArray, bExData );
//	_AddToByteArray(pByteArray,m_lIndex);
//	_AddToByteArray(pByteArray,m_lMaxHP);
//	_AddToByteArray(pByteArray,m_lHP);
//	_AddToByteArray(pByteArray,m_lDef);
//	_AddToByteArray(pByteArray,m_lMagicDef);
//	_AddToByteArray(pByteArray,m_lNameColor);
//	_AddToByteArray(pByteArray,m_lLenth);
//	_AddToByteArray(pByteArray,m_lWidth);
//	_AddToByteArray(pByteArray,m_lLevel);
//	_AddToByteArray(pByteArray,m_strHitSound.c_str());
//	_AddToByteArray(pByteArray,m_strDestorySound.c_str());
//	return true;
//}
//// ����
//bool CBuild::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
//{
//	CMoveShape::DecordFromByteArray(pByte, pos, bExData);
//	m_lIndex=_GetLongFromByteArray(pByte, pos);
//	m_lMaxHP=_GetLongFromByteArray(pByte, pos);
//	m_lHP=_GetLongFromByteArray(pByte, pos);
//	m_lDef=_GetLongFromByteArray(pByte, pos);
//	m_lMagicDef=_GetLongFromByteArray(pByte, pos);
//	m_lNameColor=_GetLongFromByteArray(pByte, pos);
//	m_lLenth=_GetLongFromByteArray(pByte, pos);
//	m_lWidth=_GetLongFromByteArray(pByte, pos);
//	m_lLevel=_GetLongFromByteArray(pByte, pos);
//	char str[1024];
//	_GetStringFromByteArray(pByte, pos, str);
//	m_strHitSound=str;
//	_GetStringFromByteArray(pByte, pos, str);
//	m_strDestorySound=str;
//	return true;
//}

// ��ӵ�CByteArray
bool CBuild::CodeToDataBlock(DBWriteSet& setWriteDB, bool bExData)
{
	CMoveShape::CodeToDataBlock_ForClient( setWriteDB, bExData );
	setWriteDB.AddToByteArray(m_lIndex);
	setWriteDB.AddToByteArray(m_lMaxHP);
	setWriteDB.AddToByteArray(m_lHP);
	setWriteDB.AddToByteArray(m_lDef);
	setWriteDB.AddToByteArray(m_lMagicDef);
	setWriteDB.AddToByteArray(m_lNameColor);
	setWriteDB.AddToByteArray(m_lLenth);
	setWriteDB.AddToByteArray(m_lWidth);
	setWriteDB.AddToByteArray(m_lLevel);
	setWriteDB.AddToByteArray(m_strHitSound.c_str());
	setWriteDB.AddToByteArray(m_strDestorySound.c_str());
	return true;
}

// ����
bool CBuild::DecodeFromDataBlock(DBReadSet& setReadDB,bool bExData )
{
	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);
	m_lIndex=setReadDB.GetLongFromByteArray();
	m_lMaxHP=setReadDB.GetLongFromByteArray();
	m_lHP=setReadDB.GetLongFromByteArray();
	m_lDef=setReadDB.GetLongFromByteArray();
	m_lMagicDef=setReadDB.GetLongFromByteArray();
	m_lNameColor=setReadDB.GetLongFromByteArray();
	m_lLenth=setReadDB.GetLongFromByteArray();
	m_lWidth=setReadDB.GetLongFromByteArray();
	m_lLevel=setReadDB.GetLongFromByteArray();
	char str[1024];
	setReadDB.GetStringFromByteArray(str,1024);
	m_strHitSound=str;
	setReadDB.GetStringFromByteArray(str,1024);
	m_strDestorySound=str;
	return true;
}

void CBuild::SetAction(WORD l)
{
	//if( m_wAction != l )
	m_lPos = 0;
	m_wAction = l;

	// ������������赲
	switch(m_wAction)
	{
	case ACT_IDLE:
	case ACT_STAND:
		{
			SetBlock(GetTileX(), GetTileY(), CRegion::BLOCK_NO);
			SetBlock(GetTileX(), GetTileY(), GetBlockType());
		}
		break;
	case ACT_OPEN:
	case ACT_DIED:
		{
			SetBlock(GetTileX(), GetTileY(), GetBlockType());
			SetBlock(GetTileX(), GetTileY(), CRegion::BLOCK_NO);
		}
		break;
	default:
		break;
	}
}


void CBuild::OnBeenAttacked( tagAttackInformation& ai )
{
	CServerRegion* pRegion = (CServerRegion*)GetFather();
	if(pRegion==NULL) return;

	//##����ǲ��ɹ��������ء�
	CMoveShape* pAttacker = dynamic_cast<CMoveShape*>( pRegion -> FindChildObject(ai.lAttackerType, ai.AttackerID) );
	if( IsAttackAble( pAttacker ) == FALSE ) return;

	vector<tagDamage*> vLose;
	ApplyFinalDamage( ai, vLose );

	//��ɱ������Ҫ��¼һЩ����
	if( IsDied() )
	{
		//����һ���������¼�
		//tagAttackEvent ae;
		//ae.lTime = GetGame() -> GetAITick();
		//ae.cType = AE_DIED;
		//GetAttackEventQueue() -> push( ae );

		//��ɱ�Ĵ���
		OnBeenMurdered( ai.lAttackerType, ai.AttackerID );

		//������ָ��
		CMessage msg( MSG_S2C_SHAPE_ATK_DIED );
		msg.Add( ai.lAttackerType );
		msg.Add( ai.AttackerID );
		msg.Add( GetType() );
		msg.Add( GetExID() );
		for( size_t i = 0; i < vLose.size(); i ++ )
		{
			if( vLose[i] -> dtType == DAMAGE_TYPE_HP )
			{
				msg.Add( vLose[i] -> wValue );
				break;
			}
		}
		// �����ķ�ʽ����ɱ��
		msg.Add( static_cast<CHAR>(DT_BEEN_KILLED) );

		//##����Ƿ�������һ���ı�־��
		msg.Add( static_cast<char>(ai.bCritical) );

		//##����Ƿ��Ǳ���
		msg.Add( static_cast<char>(ai.bBlastAttack) );

		//##��ӹ����ļ���
		msg.Add( static_cast<DWORD>(ai.tSkillID) );
		msg.Add(ai.bySkillLevel);

		msg.SendToAround( this );
		//SetKilledMeAttackInfo(ai);
		SetState(STATE_DIED);
		SetAction( ACT_ALREADYDIED );
#ifdef _DEBUG
		char szError[256];       
		_snprintf(szError, 256, "Build����״̬%d:161",STATE_DIED);   //ֵ%s����,���� , name    
		OutputDebugString(szError);		
#endif
	}
	//##���˺�
	else
	{
		if( vLose.size() > 0 )
		{
			//���������¼�
			//tagAttackEvent ae;
			//ae.lTime = GetGame() -> GetAITick();
			//ae.cType = AE_BREAK;
			//GetAttackEventQueue() -> push( ae );

			//##������ָ��
			CMessage msg( MSG_S2C_SHAPE_ATK_BREAK );
			msg.Add( ai.lAttackerType );
			msg.Add( ai.AttackerID );
			msg.Add( GetType() );
			msg.Add( GetExID() );
			msg.Add( static_cast<CHAR>( vLose.size() ) );
			for( size_t i = 0; i < vLose.size(); i ++ )
			{
				msg.Add( static_cast<CHAR>( vLose[i] -> dtType ) );
				msg.Add( vLose[i] -> wValue );
			}
			msg.Add( GetHP() );

			//##��ֱ,��0.
			//msg.Add( static_cast<char>(0) );

			//##����Ƿ�������һ���ı�־��
			msg.Add( static_cast<char>(ai.bCritical) );

			//##����Ƿ��Ǳ���
			msg.Add( static_cast<char>(ai.bBlastAttack) );

			//##��ӹ����ļ���
			msg.Add( static_cast<DWORD>(ai.tSkillID) );
			msg.Add(ai.bySkillLevel);

			msg.SendToNearestPlayer( this );

			// ���˺��AI������Ҫ�ǹ���ʹ�á�
			OnBeenHurted( ai.lAttackerType, ai.AttackerID );
		}
	}
	for( size_t i = 0; i < vLose.size(); i ++ )
	{
		OBJ_RELEASE( tagDamage, vLose[i] );
	}
}
void CBuild::ApplyFinalDamage( tagAttackInformation& ai, vector<tagDamage*>& vRet )
{
	//return ;
}

// ������ʱ����
void CBuild::OnDied()
{
	CServerRegion* pRegion = (CServerRegion*)GetFather();
	if(pRegion==NULL) return;

	// ���ɱ���Լ��Ķ���
	if(GetKilledMeType()==TYPE_PLAYER)
	{
		CPlayer* pKilledMe = (CPlayer*)pRegion->FindChildObject(TYPE_PLAYER, GetKilledMeID());
		pRegion->OnSymbolDestroy(GetExID(),GetKilledMeID(),GetKilledMeAttackInfo().AttackerGuildID,
			GetKilledMeAttackInfo().AttackerUnionID);

		if(pKilledMe)
		{
			CMessage msg( MSG_S2C_SHAPE_ATK_DIED );
			msg.Add( GetType());
			msg.Add(GetExID());
			msg.Add( GetType() );
			msg.Add( GetExID() );
			msg.SendToAround(this);
			// ִ�нű�
			
		}
	}
	else
	{
		CMessage msg( MSG_S2C_SHAPE_ATK_DIED );
		msg.Add( GetType());
		msg.Add(GetExID());
		msg.Add( GetType() );
		msg.Add( GetExID() );
		msg.SendToAround(this);
	}
}


void CBuild::OnBeenHurted( LONG lAttackerType, const CGUID& AttackerGuid )
{
}

void CBuild::OnBeenMurdered( LONG lMurdererType, const CGUID& MurdererGuid )
{
	CServerRegion* pRegion = (CServerRegion*)GetFather();
	if(pRegion==NULL) return;
}

// �ɷ񹥻�
bool CBuild::IsAttackAble(CMoveShape* pAttackShape)
{
	if(GetState() == STATE_DIED || IsDied()) return false;

	CServerRegion* pRegion = (CServerRegion*)GetFather();
	if(pRegion && !pRegion->SymbolIsAttackAble(pAttackShape)) return false;

	return true;
}

// ���ݷ���õ�����
WORD CBuild::GetFigure(eDIR dir)
{
	if(GetDir()==DIR_DOWN||GetDir()==DIR_UP)
	{
		if(dir==DIR_UP || dir== DIR_DOWN)
		{
			return (WORD)m_lWidth/2;
		}
		else if(dir==DIR_LEFT || dir==DIR_RIGHT)
		{
			return (WORD)m_lLenth/2;
		}
	}
	else if(GetDir()==DIR_RIGHT||GetDir()==DIR_LEFT)
	{
		if(dir==DIR_UP || dir== DIR_DOWN)
		{
			return (WORD)m_lLenth/2;
		}
		else if(dir==DIR_LEFT || dir==DIR_RIGHT)
		{
			return (WORD)m_lWidth/2;
		}
	}
	return 0;
}

long CBuild::GetAttackerDir(long lAttackerX,long lAttackerY,long ldx,long ldy)
{
	long lDistance = 10000000;
	long lDir = 0;
	long lTileX = GetTileX();
	long lTileY = GetTileY();
	GetBeAttackedPoint(lAttackerY,lAttackerY,lTileX,lTileY);

	lDir = GetDestDir(lAttackerY,lAttackerY,lTileX,lTileY);
	return lDir;
}

void	CBuild::GetBeAttackedPoint(long lAttackerX,long lAttackerY,long& lTileX,long& lTileY)
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

void CBuild::SetHP(long lhp)
{
	if (0==lhp)
	{
		m_lHP=lhp;
		SetAttribute("Action",ACT_DIED);
		SetAttribute("State",STATE_DIED);
		SetAttribute("Hp",0);
		OnDied();
	}
	else
	{
		SetAttribute("Hp",lhp<m_lMaxHP?lhp:m_lMaxHP);
	}
}

void CBuild::SetPosXY(float fX, float fY)
{
	long lOldX = GetTileX();
	long lOldY = GetTileY();
	long lNewTileX = static_cast<long>(fX);
	long lNewTileY = static_cast<long>(fY);
	CMoveShape::SetPosXY(fX,fY);
	//ȡ�����赲
	SetBlock(lOldX,lOldY,CRegion::BLOCK_NO);
	//���ù����赲
	if( !IsDied())
		SetBlock(lNewTileX,lNewTileY,CRegion::BLOCK_SHAPE);
}

void CBuild::SetBlock(int x, int y, unsigned short block)
{
	CShape::SetBlock(x,y,block);
}

long CBuild::OnRefresh()
{
	/*stRunScript st;
	st.pszFileName=(char*)GetRefresh().c_str();
	st.desShape=this;
	st.srcShape=this;
	st.pRegion=dynamic_cast<CRegion*>(this->GetFather());
	RunScript(&st,(char*)GetGame()->GetScriptFileData(st.pszFileName));*/
	return 0;
}

void CBuild::InitNameValueMap()
{
	CMoveShape::InitNameValueMap();
	m_mapNameValue.clear();

	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool

	st.SetType(4); st.pValue=&m_lType;         			st.attrEnumValue= B_ATTR_TYPE;			m_mapNameValue["Type"] = st;		
	st.SetType(4); st.pValue=&m_lGraphicsID;			st.attrEnumValue=B_ATTR_GRAPHICSID;		m_mapNameValue["GraphicsID"]=st;	
	st.SetType(4); st.pValue=&m_lRegionID;				st.attrEnumValue=B_ATTR_REGIONID;		m_mapNameValue["RegionID"]=st;
	st.SetType(3);	st.pfValue=&m_fPosX;				st.attrEnumValue=B_ATTR_TILEX;			m_mapNameValue["TileX"]=st;			
	st.SetType(3);	st.pfValue=&m_fPosY;				st.attrEnumValue=B_ATTR_TILEY;			m_mapNameValue["TileY"]=st;	
	st.SetType(3);	st.pfValue=&m_fDir;			        st.attrEnumValue=B_ATTR_DIR;			m_mapNameValue["Dir"]=st;			
	st.SetType(4);	st.pValue=&m_lPos;			        st.attrEnumValue=B_ATTR_POS;			m_mapNameValue["Pos"]=st;			
	st.SetType(1);	st.pwValue=&m_wState;				st.attrEnumValue=B_ATTR_STATE;			m_mapNameValue["State"]=st;			
	st.SetType(1);	st.pwValue=&m_wAction;				st.attrEnumValue=B_ATTR_ACTION;		m_mapNameValue["Action"]=st;		

	//��������
	st.SetType(4);	st.pValue = &m_lIndex;				st.attrEnumValue=B_ATTR_INDEX;			m_mapNameValue["Index"]=st;	
	st.SetType(4);	st.pValue = &m_lHP;			   		st.attrEnumValue=B_ATTR_HP;		    	m_mapNameValue["Hp"]=st;	
	st.SetType(4);	st.pValue = &m_lNameColor;	        st.attrEnumValue=B_ATTR_NAMECOLOR;		m_mapNameValue["NameColor"]=st;	
	st.SetType(4);	st.pValue = &m_lLevel;				st.attrEnumValue=B_ATTR_LEVEL;			m_mapNameValue["Level"]=st;	
	st.SetType(4);	st.pValue = &m_lMaxHP;				st.attrEnumValue=B_ATTR_MAXHP;			m_mapNameValue["MaxHp"]=st;	
	st.SetType(4);	st.pValue = &m_lDef;				st.attrEnumValue=B_ATTR_DEF;			m_mapNameValue["E_Def"]=st;
	st.SetType(4);	st.pValue = &m_lMagicDef;			st.attrEnumValue=B_ATTR_MDEF;			m_mapNameValue["E_MDef"]=st;
	
	//��������
	st.SetType(4);	st.pValue = &m_lMaxHP;				m_mapNameValue["B_MaxHp"]=st;				//��������
	st.SetType(4);	st.pValue = &m_lDef;				m_mapNameValue["B_Def"]=st;
	st.SetType(4);	st.pValue = &m_lMagicDef;			m_mapNameValue["B_MDef"]=st;	//ħ������
}

long CBuild::GetValue(const char* strName)
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
			return  (long)*(itr->second.pfValue);

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

long CBuild::SetValue(const char* strName, long dwValue)
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

//��ӱ�������������
void  CBuild::AddAttributeToDirty(string attrName)
{
	if(attrName == "")	return;
	if( (attrName[0] == 'E' && attrName[1] == '_')
		|| (attrName[0] == 'B' && attrName[1] == '_') )
		attrName[0] = 'C';
	m_mapDirtyAttrData.insert(attrName);
}
bool  CBuild::SetAttribute(std::string& attrName, long value)
{
	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if(!bFlag)
	{
		// ���������ݱ��
		if(SetValue(attrName.c_str(), value) != INVILID_VALUE)
		{
			bFlag = true;
		}
	}
	if(bFlag)
		AddAttributeToDirty(attrName);
	return false;
}
bool  CBuild::SetAttribute(char* attrName, long value)
{
	if(NULL == attrName)
		return false;

	bool bFlag = CMoveShape::SetAttribute(attrName, value);
	if(!bFlag)
	{
		// ���������ݱ��
		if(SetValue(attrName, value) != INVILID_VALUE)
		{			
			bFlag = true;
		}
	}
	if(bFlag)
		AddAttributeToDirty(string(attrName));
	return false;
}
long CBuild::GetAttribute(std::string attrName)
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
long CBuild::GetCurrentValue(const tagPty& tagValue, long evalue/*�ı�ֵ*/, long bvalue/*����ֵ*/)
{
	long retValue = evalue+bvalue;
	if( retValue > tagValue.maxValue )
		retValue = tagValue.maxValue;

	if( retValue < tagValue.minValue )
		retValue = tagValue.minValue;

	return retValue;
}

bool  CBuild::ChangeAttribute(std::string& attrName, long value)
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
long  CBuild::UpdateAttributeToClient()
{
	if( m_mapDirtyAttrData.size() == 0)
		return false;

	CMessage msg( MSG_S2C_SHAPE_CHANGE_PROPERTIES );
	msg.Add(GetType());
	msg.Add(GetExID());
	DBWriteSet setWriteDB;
	msg.GetDBWriteSet(setWriteDB);
	CodeChangeAttrToDataBlock(setWriteDB);
	// ����msg
	msg.SendToAround(this);
	return true;
}

long  CBuild::UpdateAttribute(BOOL bImmediately)
{
	if(m_mapDirtyAttrData.size() > 0)
	{
		RegisterUpdateAttrTimer();
	}
	return true;
}
long  CBuild::OnUpdateAttributeTimeOut()
{
	UpdateAttributeToClient();
	return true;
}

//void CBuild::AddChangeAttrToMsgArray(std::vector<BYTE>* bArray)
//{
//	long lShapeNum = 0;
//	long lBuildNum = 0;
//	vector<BYTE>	byShapeDirtyData;byShapeDirtyData.clear();
//	vector<BYTE>	byBuildDirtyData;byBuildDirtyData.clear();
//	//��m_mapShapeDirtyAttrData�в���,�Ƿ��޸�CMoveShape�е�����
//	set<string>::iterator itShapeDirty = m_mapShapeDirtyAttrData.begin();
//	for (;itShapeDirty!=m_mapShapeDirtyAttrData.end();itShapeDirty++)
//	{
//		lShapeNum++;
//		string& name = *itShapeDirty;
//		mapShapeNameValueItr sitr = m_mapShapeNameValue.find(name);
//		if (sitr != m_mapShapeNameValue.end())
//		{
//			//msg.Add((BYTE)saitr->second); // ��������
//			::_AddToByteArray(&byShapeDirtyData, (WORD)sitr->second.attrEnumValue);
//			switch (sitr->second.lType)
//			{
//			case 0:	// byte
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BYTE);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(BYTE));
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)*(BYTE*)(sitr->second.plValue));
//				break;
//			case 1:	// word
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_WORD);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(WORD));
//				::_AddToByteArray(&byShapeDirtyData, (WORD)*(WORD*)(sitr->second.pwValue));
//				break;
//			case 2:	// dword
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_DWORD);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(DWORD));
//				::_AddToByteArray(&byShapeDirtyData, (DWORD)*(DWORD*)(sitr->second.pdwValue));
//				break;
//			case 3:	// float
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_FLOAT);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(float));
//				::_AddToByteArray(&byShapeDirtyData, (float)*(float*)(sitr->second.pfValue));
//				break;
//			case 4:	// long
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_LONG);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(long));
//				::_AddToByteArray(&byShapeDirtyData, (long)*(long*)(sitr->second.pValue));
//				break;
//			case 5:	// short
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_SHORT);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(short));
//				::_AddToByteArray(&byShapeDirtyData, (short)*(short*)(sitr->second.psValue));
//				break;
//			case 6:	// bool
//				::_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BOOL);
//				::_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(bool));
//				::_AddToByteArray(&byShapeDirtyData, (char)*(bool*)(sitr->second.pbValue));
//				break;
//			default:
//				break;
//			}
//		}
//		else // δ�ҵ�����������
//		{
//			AddErrLog("�����һ��δ֪���ԣ�%s", name.c_str());
//			// ��������
//			_AddToByteArray(&byShapeDirtyData, (WORD)S_ATTR_UNKOWN);
//			_AddToByteArray(&byShapeDirtyData, (BYTE)ATTR_DATA_BYTE);
//			_AddToByteArray(&byShapeDirtyData, (WORD)sizeof(BYTE));
//			_AddToByteArray(&byShapeDirtyData, (BYTE)0);
//		}
//	}
//	//�����޸ĵ�Build����
//	set<string>::iterator itDirty = m_mapDirtyAttrData.begin();
//	for (;itBuildDirty!=m_mapDirtyAttrData.end();itBuildDirty++)
//	{
//		lBuildNum++;
//		string name = *itBuildDirty;
//		long baseValue = 0;
//		if(name[0] == 'C' && name[1] == '_')
//		{	
//			// ���ҳ�����ֵ
//			name[0] = 'B';
//			baseValue = GetValue(name.c_str());
//			// ���ҳ��ı�ֵ
//			name[0] = 'E';
//		}
//		mapNameValueItr itr = m_mapNameValue.find(name);
//		if (itr!=m_mapNameValue.end())
//		{
//			// ��������
//			::_AddToByteArray(&byBuildDirtyData, (WORD)itr->second.attrEnumValue);
//			long sendValue = INVILID_VALUE;
//
//			switch (itr->second.lType)
//			{
//			case 0:	// byte
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_BYTE);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(BYTE));
//				if((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_') // ����ֵ 
//				{
//					BYTE EValue = *(BYTE*)(itr->second.plValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(BYTE*)(itr->second.plValue);
//
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)sendValue);
//
//				break;
//			case 1:	// word 
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_WORD);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(WORD));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					WORD EValue = *(WORD*)(itr->second.pwValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(WORD*)(itr->second.pwValue);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sendValue);
//				break;
//			case 2:	// dword
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_DWORD);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(DWORD));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					DWORD EValue = *(DWORD*)(itr->second.pdwValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(DWORD*)(itr->second.pdwValue);
//				::_AddToByteArray(&byBuildDirtyData, (DWORD)sendValue);
//				break;
//			case 3:	// float
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_FLOAT);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(float));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					float EValue = *(float*)(itr->second.pfValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(float*)(itr->second.pfValue);
//				::_AddToByteArray(&byBuildDirtyData, (float)sendValue);
//				break;
//			case 4:	// long
//				//msg.Add((BYTE)ATTR_DATA_LONG);
//				//msg.Add((WORD)sizeof(long));
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_LONG);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(long));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					long EValue = (long)*(long*)(itr->second.pValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(long*)(itr->second.pValue);
//				::_AddToByteArray(&byBuildDirtyData, (long)sendValue);
//				break;
//			case 5:	// short
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_SHORT);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(short));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					short EValue = *(short*)(itr->second.psValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(short*)(itr->second.psValue);
//				::_AddToByteArray(&byBuildDirtyData, (short)sendValue);
//				break;
//			case 6:	// bool
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_BOOL);
//				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(bool));
//				if(((*itBuildDirty)[0] == 'C' && (*itBuildDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
//				{
//					bool EValue = *(bool*)(itr->second.pbValue);
//					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
//				}
//				else
//					sendValue = (long)*(bool*)(itr->second.pbValue);
//				::_AddToByteArray(&byBuildDirtyData, (BYTE)sendValue);
//				break;
//			default:
//				break;
//			}
//		}
//		else // δ�ҵ�����������
//		{
//			AddErrLog("�����һ��δ֪���ԣ�%s", name.c_str());
//			// ��������
//			_AddToByteArray(&byBuildDirtyData, (WORD)S_ATTR_UNKOWN);
//			_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_BYTE);
//			_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(BYTE));
//			_AddToByteArray(&byBuildDirtyData, (BYTE)0);
//		}
//		
//	}
//	_AddToByteArray(bArray, (WORD)lShapeNum);
//	if(byShapeDirtyData.size() > 0)
//		_AddToByteArray(bArray,&byShapeDirtyData[0],byShapeDirtyData.size());
//	_AddToByteArray(bArray, (WORD)lBuildNum);
//	if(byBuildDirtyData.size() > 0)
//		_AddToByteArray(bArray,&byBuildDirtyData[0],byBuildDirtyData.size());
//	m_mapDirtyAttrData.clear();
//	m_mapShapeDirtyAttrData.clear();
//}


void CBuild::CodeChangeAttrToDataBlock(DBWriteSet& setWriteDB)
{
	long lShapeNum = 0;
	long lBuildNum = 0;
	vector<BYTE>	byShapeDirtyData;byShapeDirtyData.clear();
	vector<BYTE>	byBuildDirtyData;byBuildDirtyData.clear();
	//��m_mapShapeDirtyAttrData�в���,�Ƿ��޸�CMoveShape�е�����
	set<string>::iterator itDirty = m_mapDirtyAttrData.begin();
	for (;itDirty!=m_mapDirtyAttrData.end();itDirty++)
	{
		string name = *itDirty;
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

		//�����޸ĵ�Build����
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
			lBuildNum++;
			// ��������
			::_AddToByteArray(&byBuildDirtyData, (WORD)itr->second.attrEnumValue);
			long sendValue = INVILID_VALUE;

			switch (itr->second.lType)
			{
			case 0:	// byte
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_BYTE);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(BYTE));
				if((*itDirty)[0] == 'C' && (*itDirty)[1] == '_') // ����ֵ 
				{
					BYTE EValue = *(BYTE*)(itr->second.plValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(BYTE*)(itr->second.plValue);

				::_AddToByteArray(&byBuildDirtyData, (BYTE)sendValue);

				break;
			case 1:	// word 
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_WORD);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(WORD));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					WORD EValue = *(WORD*)(itr->second.pwValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(WORD*)(itr->second.pwValue);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sendValue);
				break;
			case 2:	// dword
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_DWORD);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(DWORD));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					DWORD EValue = *(DWORD*)(itr->second.pdwValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(DWORD*)(itr->second.pdwValue);
				::_AddToByteArray(&byBuildDirtyData, (DWORD)sendValue);
				break;
			case 3:	// float
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_FLOAT);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(float));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					float EValue = *(float*)(itr->second.pfValue);
					sendValue = GetCurrentValue(itr->second, (long)EValue, baseValue);
				}
				else
					sendValue = (long)*(float*)(itr->second.pfValue);
				::_AddToByteArray(&byBuildDirtyData, (float)sendValue);
				break;
			case 4:	// long
				//msg.Add((BYTE)ATTR_DATA_LONG);
				//msg.Add((WORD)sizeof(long));
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_LONG);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(long));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					long EValue = (long)*(long*)(itr->second.pValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(long*)(itr->second.pValue);
				::_AddToByteArray(&byBuildDirtyData, (long)sendValue);
				break;
			case 5:	// short
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_SHORT);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(short));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					short EValue = *(short*)(itr->second.psValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(short*)(itr->second.psValue);
				::_AddToByteArray(&byBuildDirtyData, (short)sendValue);
				break;
			case 6:	// bool
				::_AddToByteArray(&byBuildDirtyData, (BYTE)ATTR_DATA_BOOL);
				::_AddToByteArray(&byBuildDirtyData, (WORD)sizeof(bool));
				if(((*itDirty)[0] == 'C' && (*itDirty)[1] == '_')) // �ı�ֵ) // �ı�ֵ
				{
					bool EValue = *(bool*)(itr->second.pbValue);
					sendValue = GetCurrentValue(itr->second, EValue, baseValue);
				}
				else
					sendValue = (long)*(bool*)(itr->second.pbValue);
				::_AddToByteArray(&byBuildDirtyData, (BYTE)sendValue);
				break;
			default:
				break;
			}
		}	

	}
	setWriteDB.AddToByteArray((WORD)lShapeNum);
	if(byShapeDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byShapeDirtyData[0],(long)byShapeDirtyData.size());
	setWriteDB.AddToByteArray((WORD)lBuildNum);
	if(byBuildDirtyData.size() > 0)
		setWriteDB.AddToByteArray(&byBuildDirtyData[0],(long)byBuildDirtyData.size());
	m_mapDirtyAttrData.clear();
	m_mapShapeDirtyAttrData.clear();
}

void CBuild::SetBuild(WORD l)
{
	switch (l)
	{
	case STATE_DESTORY:			//�������״̬
		{
			if (m_wState!=CShape::STATE_DIED)
			{
				SetAttribute("Action",ACT_DIED);
				SetAttribute("State",STATE_DIED);
				SetAttribute("Hp",0);
				SetAction(ACT_DIED);
				OnDied();
			}
		}
		break;
	case STATE_REFRESH:			//����Ϊ��ˢ״̬
		{

			SetAttribute("Hp",GetMaxHP());
			MoveAroundPlayer();
			SetAttribute("Action",ACT_STAND);
			SetAttribute("State",STATE_FIGHT);
			SetAction(ACT_STAND);
			OnRefresh();
		}
		break;
	default:
		break;
	}
}

void CBuild::MoveAroundPlayer()
{
	CShape *pShape=dynamic_cast<CShape*>(this);
	vector<CPlayer*> vOut;
	dynamic_cast<CServerRegion*>(this->GetFather())->FindAroundPlayer(pShape,vOut);
	long lx=GetTileX();
	long ly=GetTileY();
	//�ƿ����������е�����shape,�����赲

	for (int i=lx-GetFigure(DIR_RIGHT); i<=lx+GetFigure(DIR_RIGHT); i++)
	{
		for (int j=ly-GetFigure(DIR_UP); j<=ly+GetFigure(DIR_UP); j++)
		{
			vector<CPlayer*>::iterator iter=vOut.begin();
			for (;iter!=vOut.end();iter++)
			{
				if ((*iter)->GetTileX()==i && (*iter)->GetTileY()==j)
				{
					(*iter)->SetPosition(lx+GetWidth(),ly+GetWidth());
				}
			}
		}
	}	
}