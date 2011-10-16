#include "StdAfx.h"
#include "MoveShape.h"

#include "AntiCheat/CPlayerManager.h"
#include "Area.h"
#include "BuffSkill/BaseModule.h"
#include "BuffSkill/NewSkillFactory.h"
#include "Goods/CGoods.h"
#include "Monster.h"
#include "Pet.h"
#include "PKSys.h"
#include "Player.h"
#include "ServerRegion.h"
#include "StateAI/AIEventSender.h"
#include "PhaseSystem/PhaseManager.h"
#include "../Net/Message.h"
#include "../../../Public/Setup/PhaseSetup.h"
#include "../../../Public/Setup/GlobalSetup.h"
#include "../../../Public/ServerPublic/Server/Common/VariableList.h"
#include "../../../Public/ServerPublic/Server/Common/VarListUtils.h"
#include "LuaScript/ScriptSys.h"
#include "LuaScript/Script.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define  _NEW_STATE_

CMoveShape::CMoveShape(void)
{		
	m_bIsGod = false;
	m_pAI=NULL;

	m_tCurrentSkill				= SKILL_UNKNOW;
	m_dwLastSendMoveSendTime = timeGetTime();

	m_lMoveableCount			= 0;
	m_lWeakCount				= 0;

	m_dwStiffenStartedTimeStamp	= 0;
	m_lStiffenCounts			= 0;
	

	m_ScriptTimers.clear();	
	ZeroMemory(&m_KilledMeAttackInfo,sizeof(tagAttackInformation));

	m_bRun						= true;
	m_bAntiInvisibleable        = true;			//���ɷ�����
	m_bInvisible				= true;			//�ɼ�״̬	
	m_bPretendDead				= true;			//��װ��	

	m_sMoveable					= 1;			//���ƶ���־��
    m_sSelfControl				= 1;			//�ɿ��Ʊ�־��
    m_sUseSkill					= 1;			//��ʹ�ü��ܱ�־��
    m_sMoveDamage				= 1;			//���Ƴ��������ñ�־��
    m_sBeenAttack				= 1;			//�ɱ�������־��
    m_sRandomMove				= 1;		    //������ܶ�

	m_bUseRabbSkill				= true;			//��ʹ�÷�ʦ����
	m_bUseFighterSkill			= true;			//��ʹ��սʿ����
	m_bUseItem					= true;			//��ʹ����Ʒ
	m_bInstruction				= true;			//��ʹ��ָ��
	m_bAddHp					= true;			//hp�����ӱ�־��
	m_bForceAttack				= true;			//��ǿ�ƹ���
	
	m_bHide						= true;			//������
	m_bRide						= true;			//������״̬
	m_bWeakness					= true;			//����״̬

	m_lChangeModelType			= 0;	
	m_lChangeModelID			= 0;

	//buff��̬����
	m_BuffParam.clear();
	//debuff��̬����
	m_DeBuffParam.clear();
	//�������ܶ�̬����
	m_ActiveSkillParam.clear();


	//�����ֵ
	m_ActiveID = 0;	
	m_FixedReinforce.first	= 1.0f;
	m_FixedReinforce.second = 1.0f;
	//���񱣻����
	m_BeneficiaryType=(OBJECT_TYPE)0;
	m_bRoleProtect				=false;
	//�ɼ����
	m_dwCollectionID=0;
	m_dwCollectTimes=0;
	m_bIsBeenCollect=FALSE;
	m_bCanBeenCollect=FALSE;
	m_dwActStartTime	  = 0;
	m_dwActTime			  = 0;
	m_lDelEventTimerID    =-1;
	m_lUpdateAttrTimerID = -1;
	m_lBeneficiaryTimerID = -1;
	//������
	m_lAttackerType = 0;
	m_guidAttacker = NULL_GUID;
	//ѡ��Ŀ��
	m_lTargetType  = 0;
	m_guidTarget   = NULL_GUID;	
	m_SelectSelfPlayers.clear();	
    m_dwCDStartTime=0;
    m_dwCDControlTime=0;
	m_VarList = new VariableList();
}



CMoveShape::~CMoveShape(void)
{
	//����ű���ʱ��
	ClearScriptTimer();	

	delete m_VarList;
}

//  [7/4/2007 chenxianjin]
//����ʹ��սʿ���ܱ�־��
void CMoveShape::SetUseFightSkill(bool b)
{
	m_bUseFighterSkill = b;
}

//����ʹ�÷�ʦ���ܱ�־��
void CMoveShape::SetUseRabbSkill(bool b)
{
	m_bUseRabbSkill = b;
}

//����ʹ�õ��߱�־��
void CMoveShape::SetUseItem(bool b)
{
	m_bUseItem = b;
}


bool CMoveShape::IsVisible(CMoveShape* pTarget)
{
	if(NULL == pTarget)
		return m_bInvisible;
	return ( !pTarget->GetAntiInvisibleable() || 	// �Ƿ���
		GetInvisibleState() );			// ����	
}

// [070611 AHC]
long CMoveShape::GetValue(const char* strName)
{
	long ret = INVILID_VALUE;

	mapShapeNameValueItr itr = m_mapShapeNameValue.begin();
	itr = m_mapShapeNameValue.find(strName);
	if (itr != m_mapShapeNameValue.end())
	{
		switch (itr->second.lType)
		{
		case 0:	// byte
			ret = (long)*(itr->second.plValue);
			break;

		case 1:	// word
			ret = (long)*(itr->second.pwValue);
			break;

		case 2:	// dword
			ret = (long)*(itr->second.pdwValue);
			break;

		case 3:	// float
			ret = (long)*(itr->second.pfValue);
			break;

		case 4:	// long
			ret = *(itr->second.pValue);
			break;

		case 5:	// short
			ret = (long)*(itr->second.psValue);

		case 6:	// bool
			ret = (long)*(itr->second.pbValue);
			break;
		}
	}
	return ret;
}

long CMoveShape::SetValue(const char* strName, long dwValue)
{
	long ret = INVILID_VALUE;

	mapShapeNameValueItr itr = m_mapShapeNameValue.begin();
	itr = m_mapShapeNameValue.find(strName);
	if (itr != m_mapShapeNameValue.end())
	{
		switch (itr->second.lType)
		{
		case 0:	// byte
			{
				*(itr->second.plValue) = (BYTE)dwValue;
				ret = dwValue;
			}
			break;
		case 1:	// word
			{
				*(itr->second.pwValue) = (WORD)dwValue;
				ret = dwValue;
			}
			break;
		case 2:	// dword
			{
				*(itr->second.pdwValue) = (DWORD)dwValue;
				ret = dwValue;
			}
			break;
		case 3:	// float
			{
				*(itr->second.pfValue) = (float)dwValue;
				ret = dwValue;
			}
			break;
		case 4:	// long
			{
				*(itr->second.pValue) = (long)dwValue;
				ret = dwValue;
			}
			break;
		case 5:	// short
			{
				*(itr->second.psValue) = (short)dwValue;
				ret = dwValue;
			}
			break;
		case 6:	// bool
			{
				*(itr->second.pbValue) = dwValue==0?false:true;
				ret = dwValue;
			}
			break;
		default:
			break;
		}
	}

	return ret;
}


//�����Է����ı�ʱ�����
void CMoveShape::OnPropertyValueChanged(const char* strName)
{
}

// ��ʼ��
void CMoveShape::InitNameValueMap()
{
	m_mapShapeNameValue.clear();

	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.lType=3; st.pfValue=&m_fSpeed;										st.attrEnumValue = S_ATTR_SPEED;	   		    m_mapShapeNameValue["fSpeed"] = st;	
	st.lType=1; st.pwValue=&m_wState;										st.attrEnumValue = S_ATTR_SHAPESTATE;	   		m_mapShapeNameValue["wShapeState"] = st;	
	st.lType=6; st.pbValue=&m_bInvisible;			st.lCount=0;			st.attrEnumValue = S_ATTR_INVISIBLETIME;   		m_mapShapeNameValue["bInvisible"] = st;				//����
	st.lType=6; st.pbValue=&m_bHide;				st.lCount=0;			st.attrEnumValue = S_ATTR_HIDE;   				m_mapShapeNameValue["bHide"] = st;			    	//����
	st.lType=6; st.pbValue=&m_bPretendDead;			st.lCount=0;			st.attrEnumValue = S_ATTR_PRETENDDEADTIME; 		m_mapShapeNameValue["bPretendDead"]=st;				//����
	st.lType=4; st.pValue=&m_lChangeModelType;		st.lCount=0;			st.attrEnumValue = S_ATTR_CHANGEMODELTYPE; 		m_mapShapeNameValue["lChangeModelType"]=st;			//�ı�����
	st.lType=4; st.pValue=&m_lChangeModelID;		st.lCount=0;			st.attrEnumValue = S_ATTR_CHANGEMODELID;   		m_mapShapeNameValue["lChangeModelID"]=st;			//�ı�ģ��id
	st.lType=6; st.pbValue=&m_bInstruction;	    	st.lCount=0;			st.attrEnumValue = S_ATTR_USEINSTRUCTION;  		m_mapShapeNameValue["bUnUseInstruction"]=st;		//ʹ��ָ��

	st.lType=5; st.psValue=&m_sMoveable;			st.lCount=0;			st.attrEnumValue = S_ATTR_MOVEABLE;		   		m_mapShapeNameValue["sMoveable"]=st;			    //�ܷ��ƶ�
    st.lType=5; st.psValue=&m_sSelfControl;			st.lCount=0;			st.attrEnumValue = S_ATTR_SELFCONTROL;		   	m_mapShapeNameValue["sSelfControl"]=st;			    //�ܷ����ҿ���
    st.lType=5; st.psValue=&m_sUseSkill;			st.lCount=0;			st.attrEnumValue = S_ATTR_USEALLSKILL;		   	m_mapShapeNameValue["sUseAllSKill"]=st;			    //�Ƿ����ʹ�ü���
    st.lType=5; st.psValue=&m_sMoveDamage;			st.lCount=0;			st.attrEnumValue = S_ATTR_MOVEDAMAGE;		   	m_mapShapeNameValue["sMoveDamage"]=st;			    //�Ƿ��ƶ�����Ч��
    st.lType=5; st.psValue=&m_sBeenAttack;			st.lCount=0;			st.attrEnumValue = S_ATTR_CANBEENATTACK;		m_mapShapeNameValue["sCanBeenAttack"]=st;			//�ܷ񱻹���
    st.lType=5; st.psValue=&m_sRandomMove;			st.lCount=0;			st.attrEnumValue = S_ATTR_RAMDOMMOVE;			m_mapShapeNameValue["sRandomMove"]=st;		        //����ܶ�
    st.lType=5; st.pfValue=&m_fMoveSpeed;			st.lCount=0;			st.attrEnumValue = S_ATTR_MOVECONTROL;			m_mapShapeNameValue["fMoveSpeed"]=st;		        //����Ӱ���ƶ��ٶ�

    
	st.lType=6; st.pbValue=&m_bUseFighterSkill;		st.lCount=0;			st.attrEnumValue = S_ATTR_USEFIGHTSKILL;   		m_mapShapeNameValue["bUseFighterSkill"]=st;			//ʹ��սʿ����
	st.lType=6; st.pbValue=&m_bUseRabbSkill;		st.lCount=0;			st.attrEnumValue = S_ATTR_USERABBSKILL;	   		m_mapShapeNameValue["bUseRabbSkill"]=st;			//ʹ�÷�ʦ����
	st.lType=6; st.pbValue=&m_bUseItem;	        	st.lCount=0;			st.attrEnumValue = S_ATTR_USEITEM;			    m_mapShapeNameValue["bUseItem"]=st;					//ʹ�õ���
	st.lType=6; st.pbValue=&m_bAntiInvisibleable;	st.lCount=0;			st.attrEnumValue = S_ATTR_ANTIINVISIBLEABLE;	m_mapShapeNameValue["bAntiInvisibleable"]=st;		//������
	st.lType=6; st.pbValue=&m_bAddHp;				st.lCount=0;			st.attrEnumValue = S_ATTR_ADDHP;				m_mapShapeNameValue["bAddHp"]=st;		            //��������HP
	st.lType=6; st.pbValue=&m_bForceAttack;			st.lCount=0;			st.attrEnumValue = S_ATTR_FORCEATTACK;			m_mapShapeNameValue["bForceAttack"]=st;				//ǿ�ƹ���
	st.lType=6; st.pbValue=&m_bRide;				st.lCount=0;			st.attrEnumValue = S_ATTR_NOTRIDE;				m_mapShapeNameValue["bRide"]=st;					//����״̬

	st.lType=2; st.pdwValue=&m_dwCollectionID;			st.lCount=0;			st.attrEnumValue = S_ATTR_COLLECTIONID;				m_mapShapeNameValue["dwCollectionID"]=st;		        //�ɼ���id
	st.lType=2; st.pdwValue=&m_dwCollectTimes;			st.lCount=0;			st.attrEnumValue = S_ATTR_COLLECTIONTIMES;			m_mapShapeNameValue["dwCollectionTimes"]=st;		    //�ܲɼ��Ĵ���
	st.lType=6; st.pbValue=&m_bIsBeenCollect;			st.lCount=0;			st.attrEnumValue = S_ATTR_ISBEENCOLLECT;			m_mapShapeNameValue["bIsBeenCollect"]=st;		        //�Ƿ����ڱ��ɼ�
	st.lType=6; st.pbValue=&m_bCanBeenCollect;			st.lCount=0;			st.attrEnumValue = S_ATTR_CANBEENCOLLECT;			m_mapShapeNameValue["bCanBeenCollect"]=st;		        //�ܷ񱻲ɼ�

}
bool  CMoveShape::SetAttribute(std::string& attrName, long value)
{
	if(INVILID_VALUE != SetValue(attrName.c_str(), value))
	{
		// ���������ݱ��
		return true;
	}

	return false;
}
bool  CMoveShape::SetAttribute(char* attrName, long value)
{
	if(NULL == attrName)
		return false;

	if(INVILID_VALUE != SetValue(attrName, value))
	{
		return true;
	}

	return false;
}
long CMoveShape::GetAttribute(std::string attrName)
{
	long value = INVILID_VALUE;

	value = GetValue(attrName.c_str());

	return value;
}

bool  CMoveShape::ChangeAttribute(std::string& attrName, long value)
{
	// ���������ݱ��
	long curValue = 0;
	long rightValue = GetValue(attrName.c_str());

	curValue = rightValue + value;
	mapShapeNameValueItr itr = m_mapShapeNameValue.begin();
	itr = m_mapShapeNameValue.find(attrName);
	if (itr != m_mapShapeNameValue.end())
	{	
#ifdef _DUMP
		Log4c::Trace(ROOT_MODULE,FormatText("GS_MOVESHAPE_0",attrName.c_str(),curValue));//�޸ĵ�������%s��ǰֵΪ%d.\n
#endif
		if(SetValue(attrName.c_str(), curValue+rightValue) != INVILID_VALUE)
			return true;
	}
	return false;
}



void CMoveShape::SetKilledMeAttackInfo( LONG lType, CGUID guid )
{
	m_KilledMeAttackInfo.lAttackerType			= lType;
	m_KilledMeAttackInfo.AttackerID				= guid;
}

void CMoveShape::SetWeakNessState(bool bWeak)
{

	if( bWeak )
	{
		-- m_lWeakCount;
	}
	else 
	{
		if( m_lWeakCount < 0 ) m_lWeakCount = 0;
		++ m_lWeakCount;

	}

	if( m_lWeakCount <= 0 )
	{
		m_bWeakness = TRUE;		
	}
	else 
		m_bWeakness = FALSE;


}

void CMoveShape::SetMoveable( BOOL b )
{

    if ( m_sMoveable != eImmunity )
    {
        m_sMoveable = b;
    }
}

//����bool����
BOOL  CMoveShape::SetBoolAttribute(const std::string& attrName, long value)
{
	return SetBoolAttribute(attrName, static_cast<BOOL>(value));	
}

BOOL CMoveShape::SetBoolAttribute(const string &attrName, BOOL b)
{
	mapShapeNameValueItr itr = m_mapShapeNameValue.find(attrName);

	if(itr != m_mapShapeNameValue.end()) // �ҵ��ñ���
	{
		if( b )
		{
			--itr->second.lCount;			
		}
		else 
		{		
			if(itr->second.lCount<0)
				itr->second.lCount = 0;
			++itr->second.lCount ;			
		}


		if( itr->second.lCount  <= 0 )
		{
			*(itr->second.pbValue) = true;			
		}
		else 
			*(itr->second.pbValue) = false;

		if (attrName=="bRandomMove" && *(itr->second.pbValue)==false)
		{
			if (GetType()==TYPE_PLAYER)
			{
				AI_ASSERT( "StartActiveAct not implemented" && 0 );
			}

		}
		if (attrName=="bMoveable" && *(itr->second.pbValue)==false)
		{
			SetPosition(GetTileX(),GetTileY());
		}

		AddAttributeToDirty(attrName);
	}

	//���Ը���
	return TRUE;
}


long CMoveShape::SetStateAttribute(std::string& attrName,long value)
{
    if ( attrName == "fSpeed")
    {
        /// ����
        if( m_sMoveable == 2 )  
        {
            SendSkillImmunityToClient(attrName);
            return 0;
        }

        SetSpeed( value );
        SendChangedSpeedToClients();

        return 0;
    }

    mapShapeNameValueItr itr = m_mapShapeNameValue.find(attrName);

    if(itr != m_mapShapeNameValue.end()) // �ҵ��ñ���
    {
        if ( value == 0 && *(itr->second.psValue) == 2)
        {
            SendSkillImmunityToClient(attrName);
            return 0;
        }

        if( value == 1 )
        {
            --itr->second.lCount;			
        }
        else 
        {		
            if(itr->second.lCount<0)
                itr->second.lCount = 0;
            ++itr->second.lCount ;			
        }

        if( itr->second.lCount  <= 0 )
        {
            *(itr->second.pbValue) = 1;			
        }
        else 
            *(itr->second.pbValue) = 0;

        AddAttributeToDirty(attrName);

        return 0;
    }

    return -1;
}

long  CMoveShape::ChangeStateAttribute(std::string& attrName,long value)
{
    if ( attrName == "fSpeed")
    {
        /// ����
        if( m_sMoveable == 2 )  
        {
            SendSkillImmunityToClient(attrName);
            return 0;
        }

        ChangeSpeed( value );
        SendChangedSpeedToClients();

        return 0;
    }

    mapShapeNameValueItr itr = m_mapShapeNameValue.find(attrName);

    if(itr != m_mapShapeNameValue.end()) // �ҵ��ñ���
    {
        if ( value == 0 && *(itr->second.psValue) == 2)
        {
             SendSkillImmunityToClient(attrName);
             return 0;
        }

        value = *(itr->second.psValue) + value;

        return SetStateAttribute(attrName,value);
    }

    return -1;
}


void CMoveShape::SendSkillImmunityToClient(const std::string& AttrName)
{

#ifdef _SKILL_INFO_D_   
    Log4c::Trace(SKILL_MODULE, "Change:����ֵ%s�����ߴ�������״̬",AttrName.c_str());    
#endif
    CMessage msg(MSG_S2C_SHAPE_ATK_IMMUNITY);
    msg.Add( AttrName.c_str() );
    msg.SendToAround(this);

    return ;
}

void CMoveShape::InitSkills()
{
	//##������еļ���
	ClearSkills();	
}

void CMoveShape::ClearSkills()
{
	ClearPassiveSkill();	 
	EndAllActiveSkill();	
}

// �쳣״̬��AI
void CMoveShape::UpdateAbnormality()
{	
	/**## ���������NULL��ָ�� */
	BOOL bNeedUpdateProperties = FALSE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
//	Shapeս������
//////////////////////////////////////////////////////////////////////////////////////////////////

// ��Ⲣ���𹥻�
long CMoveShape::BeginAttack( long lX, long lY )
{
	AI_ASSERT( "Attack not implemented" && 0 );
	//GetAI() ->Attack( GetDefaultAttackSkillID(), lX, lY );
	return RET_AI_BREAK;
}

// Ŀ�������ж�
BOOL CMoveShape::IsDied()
{	
	if(GetState() == STATE_DIED ||
		GetHP() ==0 )
	{
		return TRUE;
	}
	return FALSE;
}

// �������
WORD CMoveShape::GetAtcInterval()
{
	return static_cast<WORD>( GetAtcSpeed() );
}

//##��ȡ��ȭ��ʱ��
DWORD CMoveShape::GetStrikeOutTime()
{
	return 600;
}

//////////////////////////////////////////////////////////////////////////
// ����
//////////////////////////////////////////////////////////////////////////
// �ı�����λ��
// �ı��赲��λ��
void CMoveShape::SetPosXY(float fX,float fY )
{
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>(GetFather());
	if(NULL == pRegion)	return;

	if( (int)fX < 0 || (int)fX >= pRegion->GetWidth() || (int)fY < 0 || (int)fY >= pRegion->GetHeight() )
	{
		return; 
	}

	long lOldX = GetTileX();
	long lOldY = GetTileY();

	CShape::SetPosXY(fX, fY);

	if(GetType() == TYPE_PLAYER || GetType() == TYPE_MONSTER)
	{
		//���ٸ����ϵ�Shape����fY
		pRegion->DecCellObjNum(lOldX,lOldY);
		//���Ӹ����ϵ�Shape����fY
		pRegion->IncCellObjNum((long)fX,(long)fY);
	}

	long lNewTileX = static_cast<long>(fX);
	long lNewTileY = static_cast<long>(fY);
	// �����л�
	long nx = lNewTileX / AREA_WIDTH;
	long ny = lNewTileY / AREA_HEIGHT;
	if( GetArea() && ( GetArea()->GetX() != nx || GetArea()->GetY() != ny ))
	{
		pRegion->OnShapeChangeArea(this,nx,ny);
	}
	else
	{
		SetChangeState(0);
	}

	//����λ�øı�ӿ�
	if(lOldX != lNewTileX || lOldY != lNewTileX)
	{
		OnPosChanged();
	}

}

BOOL	CMoveShape::IsInAct()
{
	DWORD dwCurrTime=timeGetTime();

	if (dwCurrTime>(m_dwActStartTime+m_dwActTime))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

// ��ȡĿ�귽��
long CMoveShape::GetDestDir(long sx, long sy, long dx, long dy)
{

	long x = sx - dx;
	long y = sy - dy;

	// ����
	if( x > 0 )
	{
		if( y > 0 )
		{
			return DIR_LEFTUP;
		}
		else if( y < 0 )
		{
			return DIR_LEFTDOWN;
		}
		else
		{
			return DIR_LEFT;
		}
	}
	else if( x < 0 )	// ����
	{
		if( y > 0 )
		{
			return DIR_RIGHTUP;
		}
		else if( y < 0 )
		{
			return DIR_RIGHTDOWN;
		}
		else
		{
			return DIR_RIGHT;
		}
	}
	else	// ����
	{
		if( y > 0 )
		{
			return DIR_UP;
		}
		else
		{
			return DIR_DOWN;
		}
	}

	return 0;
}

void CMoveShape::ForceMove( LONG lDestX, LONG lDestY, DWORD dwTime )
{
	//##������֤X,Y�Ƿ����ڳ����ķ�Χ��
	LONG lVerifiedDestX = lDestX;
	LONG lVerifiedDestY = lDestY;
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		if( lDestX >= pRegion -> GetWidth() )
		{
			lVerifiedDestX = pRegion -> GetWidth() - 1;
		}
		if( lDestX < 0 )
		{
			lVerifiedDestX = 0;
		}
		if( lDestY >= pRegion -> GetHeight() )
		{
			lVerifiedDestY = pRegion -> GetWidth() - 1;
		}
		if( lDestY < 0 )
		{
			lVerifiedDestY = 0;
		}
	}
	else
	{
		return;
	}

	CMessage msg( MSG_S2C_SHAPE_FORCEMOVE );
	msg.Add( GetExID() );
	msg.Add( GetType() );
	msg.Add( lVerifiedDestX );
	msg.Add( lVerifiedDestY );
	msg.SendToAround( this );

	//##�Ƶ��յ�
	SetPosXY( lVerifiedDestX+0.5f, lVerifiedDestY+0.5f );

}
//������������
bool CMoveShape::EndPassiveSkill(DWORD dwID,DWORD dwLv)
{
	for (itModu it=m_PassiveSkillParam.begin(); it!=m_PassiveSkillParam.end();)
	{
		if (dwID==it->second->GetID())
		{
			if((long)dwLv > (long)it->second->GetLv())
			{
				CBaseModule *pBaseModule = CNewSkillFactory::GetBaseModule(it->second);
				if(pBaseModule)
				{
					pBaseModule->End();				
					CNewSkillFactory::RealeaseBaseMoudule(it->second);
					it = m_PassiveSkillParam.erase(it);	
				}
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			++it;
		}
	}
	return true;
}

//��ӱ�������
void CMoveShape::AddPassSkill(DWORD dwID, DWORD dwLv)
{
    CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwID);

    if (NULL!=pSkillProperty)
    {
        if(pSkillProperty->GetSkillType()==ModuType_Skill_Passive && GetInst(SkillAttribute).IsExist(dwID , dwLv) )
        {				
            //������������
            stModuParam *pPassiveSkill = AddPassiveSkill(dwID,dwLv);
            if (pPassiveSkill)
            {				
                m_PassiveSkillParam[pPassiveSkill->GetGlobleID()] = pPassiveSkill;					
            }
        }
    }   
}

//������̬����
stModuParam  *CMoveShape::InitModuParam(DWORD dwSkillID, DWORD dwLevel, DWORD dwGlobleID)
{
	stModuParam *pModuParam = OBJ_CREATE(stModuParam);
	pModuParam->SetGlobleID(dwGlobleID);

	CNewSkill *pSkillProperties = CNewSkillFactory::QueryNewSkillProperty(dwSkillID);
	CNewSkillFactory::InitModuParam(pSkillProperties,pModuParam,dwLevel);
    pModuParam->CalcCoolDown(this,dwSkillID,dwLevel);

	return pModuParam;

}

//�����������ܶ�̬����
stModuParam *CMoveShape::AddPassiveSkill(DWORD dwSkillID, DWORD dwLevel)
{
	++m_ActiveID;
	if (!EndPassiveSkill(dwSkillID,dwLevel))
	{
		return NULL;
	}

	stModuParam *pSkillParam = InitModuParam(dwSkillID, dwLevel, m_ActiveID);

	if(pSkillParam == NULL)	
	{						
		return NULL;
	}		
	pSkillParam->Reset(this,this);	

	return pSkillParam;
}

//������������
BOOL CMoveShape::OnBeginSkill( DWORD dwID,DWORD dwLv )
{
    CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwID);

    if (NULL!=pSkillProperty)
    {
        if(pSkillProperty->GetSkillType()==ModuType_Skill_Passive && GetInst(SkillAttribute).IsExist(dwID , dwLv) )
        {				
            //������������
            stModuParam *pSkillParam = AddPassiveSkill(dwID,dwLv);

            if (pSkillParam)
            {
                CBaseModule *pPassiveSkill = CNewSkillFactory::CreateModule(dwID,dwLv,pSkillParam,pSkillParam->GetGlobleID(),0,1);

                if(pPassiveSkill && pPassiveSkill->Begin(0,0,GetType(),GetExID())!=MR_Modu_End) 
                {
                    m_PassiveSkillParam[pSkillParam->GetGlobleID()] = pSkillParam;
                    return TRUE;
                }
                else
                {					
                    CNewSkillFactory::RealeaseBaseMoudule(pSkillParam);
                }
            }
        }
    }
	return FALSE;
}
//����������ܵĶ�̬����
void CMoveShape::AddActiveSkill(stModuParam *pSkillParam)
{	
	if (pSkillParam)
	{
		if( pSkillParam->GetModType()==ModuType_Skill_Active)
		{		
			m_ActiveSkillParam[pSkillParam->GetGlobleID()] = pSkillParam;
		}		
	}	
}

//��ʼ���������ܶ�̬����
void CMoveShape::InitSkillInstance(DWORD dwSkillID,DWORD dwLevel,long lGoodsIndex,long lContainerID,long lPos,CGUID GooodsGuid,long lType)
{
	++m_ActiveID;
	stModuParam *pModuParam = InitModuParam(dwSkillID, dwLevel, m_ActiveID);
	if (pModuParam)
	{
		CNewSkillFactory::CreateModule(dwSkillID,dwLevel,pModuParam,m_ActiveID,0,lType);
		pModuParam->Reset(this,this);
		pModuParam->SetGoodGuid(GooodsGuid);
		pModuParam->SetGoodsIndex(lGoodsIndex);
		pModuParam->SetContainerID(lContainerID);
		pModuParam->SetPos(lPos);	
		m_pInstanceSkill = pModuParam;
	}
}

//�����֧����
long CMoveShape::AddActiveSkill(DWORD dwSkillID, DWORD dwLevel,CMoveShape *pTarget,long lGoodsIndex,long lContainerID,long lPos,CGUID GoodsGuid)
{
	return m_ActiveID;
}

//��ȡ����״̬����
long CMoveShape::GetAllStateNum()
{
	long lStateNum=0;
	lStateNum = (long)m_BuffParam.size() + (long)m_DeBuffParam.size();
	return lStateNum;
}

//ͨ��״̬id�Ͳ������Ƶõ�״̬����ز���
long CMoveShape::GetStateParamByID(DWORD dwID,string strParam)	
{
	long lNum=0;
	mapModules mpTempModule;
	for (itModu it=m_BuffParam.begin(); it!=m_BuffParam.end(); ++it)
	{
		if (dwID==it->second->GetID())
		{
			mpTempModule[it->first] = it->second;
			++lNum;
		}
	}
	for (itModu it=m_DeBuffParam.begin(); it!=m_DeBuffParam.end(); ++it)
	{
		if (dwID==it->second->GetID())
		{
			mpTempModule[it->first] = it->second;
			++lNum;
		}
	}

	if (strParam=="Num")
	{
		return lNum;
	}
	else if (strParam=="Lv")
	{
		long lLv=0;
		for (itModu it=mpTempModule.begin(); it!=mpTempModule.end(); ++it)
		{
			if (lLv<it->second->GetLv())
			{
				lLv = it->second->GetLv();
			}			
		}
		return lLv;
	}
	else
	{
		return 0;
	}
}

long CMoveShape::GetStateParamByID(DWORD dwID,string strParam,CMoveShape* shape,long type)
{
    long lSelf=0,lDest=0;
    mapModules mpSelfModule,mpDestModule;
    for (itModu it=m_BuffParam.begin(); it!=m_BuffParam.end(); ++it)
    {
        if (dwID==it->second->GetID())
        {
            CMoveShape * pShape = it->second->GetFirer();
            if( pShape == shape )
            {
                mpSelfModule[it->first] = it->second;
                lSelf ++;
            }
            else 
            {
                mpDestModule[it->first] = it->second;
                lDest ++;
            }
        }
    }
    for (itModu it=m_DeBuffParam.begin(); it!=m_DeBuffParam.end(); ++it)
    {
        if (dwID==it->second->GetID())
        {
            CMoveShape * pShape = it->second->GetFirer();
            if( pShape == shape )
            {
                mpSelfModule[it->first] = it->second;
                lSelf ++;
            }
            else 
            {
                mpDestModule[it->first] = it->second;
                lDest ++;
            }
        }
    }

    if (strParam=="Num")
    {
        if( type == 1 )
            return lSelf;
        return lDest;
    }
    else if (strParam=="Lv")
    {
        long lLv=0;
        itModu it,end;
        if ( type == 1)
        {
            it = mpSelfModule.begin();
            end = mpSelfModule.end(); 
        }
        else
        {
            it = mpDestModule.begin();
            end = mpDestModule.end(); 
        }
        for (; it!=end; ++it)
        {
            if (lLv<it->second->GetLv())
            {
                lLv = it->second->GetLv();
            }			
        }
        return lLv;
    }
    else
    {
        return 0;
    }
}


//ͨ��״̬�����Ͳ������Ƶõ�״̬����ز���
long CMoveShape::GetStateParamByIndex(DWORD dwIndex,string strParam)
{
	long lNum=0;
	mapModules mpTempModule;
	for (itModu it=m_BuffParam.begin(); it!=m_BuffParam.end(); ++it)
	{
		++lNum;
		if (lNum==dwIndex)
		{
			if (strParam=="ID")
			{
				return it->second->GetID();
			}
			else if (strParam=="Lv")
			{
				return it->second->GetLv();
			}

		}		
	}
	for (itModu it=m_DeBuffParam.begin(); it!=m_DeBuffParam.end(); ++it)
	{
		++lNum;
		if (lNum==dwIndex)
		{
			if (strParam=="ID")
			{
				return it->second->GetID();
			}
			else if (strParam=="Lv")
			{
				return it->second->GetLv();
			}

		}		
	}
	return 0;
}

//ɾ��״̬
void CMoveShape::RemoveState(itModu &it)
{	
	CBaseModule *pState = CNewSkillFactory::GetBaseModule((*it).second);
	if (pState)
	{
		ModuleType eMType = (*it).second->GetModType();	
		EndState(pState);
		
		if(eMType==ModuType_Buff)
		{
			it = m_BuffParam.erase(it);		
		} 
		else
		{	
			it = m_DeBuffParam.erase(it);		
		}	
	}
}
//ɾ��״̬
void CMoveShape::RemoveState(mapModules &mpBaseModule, DWORD dwStateID, DWORD dwLevel,DWORD dwCount)	
{
	int nCount = 0;
	itModu it = mpBaseModule.begin();
	for(;it!=mpBaseModule.end();)
	{		
		if (it->second->GetID()==dwStateID && (it->second->GetLv()==dwLevel || dwLevel==0))
		{				
			if (it->second)
			{
 				RemoveState(it);
				++nCount;
				if (dwCount>0 && nCount>=(long)dwCount)
				{
					return;
				}
				else
				{
					continue;
				}
			}

		}	
		++it;
	}	  
}
//ɾ��״̬
void CMoveShape::DelNewState(DWORD dwStateID, DWORD dwLevel,DWORD dwCount)
{
	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwStateID);

	if (pSkillProperty && pSkillProperty->GetSkillType()==ModuType_Buff)
	{
		RemoveState(m_BuffParam,dwStateID,dwLevel,dwCount);
	}
	else if (pSkillProperty && pSkillProperty->GetSkillType()==ModuType_DeBuff)
	{
		RemoveState(m_DeBuffParam,dwStateID,dwLevel,dwCount);
	}	
}

//���ָ�����͵�״̬
void CMoveShape::ClearState(eSkillAttackType Type,DWORD dwCount)
{
    for ( itModu it = m_BuffParam.begin() ; it != m_BuffParam.end()  && dwCount > 0; it++ )
    {
         /// ͬ���͵�
         if ( it->second->GetAttackType() == Type )
         {
             RemoveState(it);
             dwCount --;
         }
    }
    
    for ( itModu it = m_DeBuffParam.begin();it != m_DeBuffParam.end() && dwCount > 0 ;it++ )
    {
        /// ͬ���͵�
        if ( it->second->GetAttackType() == Type )
        {
            RemoveState(it);
        }
    }
}

//�Ƴ�����
void CMoveShape::RemoveBaseModule(stModuParam* pModuParam)
{
	mapModules *mapBaseModule;
	CBaseModule *pBaseModule = CNewSkillFactory::GetBaseModule(pModuParam);

	if(pModuParam->GetModType()==ModuType_Skill_Active)
	{
		AI_EVENT_SENDER( GetAI() ).OnSkillEnd( pModuParam->GetAddSub() );
		//GetAI()->OnSkillEnd(pModuParam->GetAddSub());

		mapBaseModule = &m_ActiveSkillParam;

	}
	else if (pModuParam->GetModType()==ModuType_Buff)
	{
		SubStateCount(pModuParam);
		mapBaseModule = &m_BuffParam;	

		OnDelBuff(pModuParam->GetID(), pModuParam->GetLv());
	} 
	else if (pModuParam->GetModType()==ModuType_DeBuff)
	{
		SubStateCount(pModuParam);
		mapBaseModule = &m_DeBuffParam;

		OnDelBuff(pModuParam->GetID(), pModuParam->GetLv());
	}
	else if (pModuParam->GetModType()==ModuType_Skill_Passive)
	{	
		AI_EVENT_SENDER( GetAI() ).OnSkillEnd( pModuParam->GetAddSub() );
		//GetAI()->OnSkillEnd(pModuParam->GetAddSub());

		mapBaseModule = &m_PassiveSkillParam;
	}

	if ((*mapBaseModule).size()>0)
	{
		ReleaseBaseModule(pBaseModule->GetParam(),*mapBaseModule);
	}	
}
//�Ƴ�����
void CMoveShape::ReleaseBaseModule(stModuParam* pModuParam,mapModules &mpBaseModule)
{

	pModuParam->ProcNumSub();
	mpBaseModule.erase(pModuParam->GetGlobleID());
	CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
}

//�Ƴ�ActiveSkill
void CMoveShape::RemoveActive(CBaseModule* pActiveSkill)
{
	if (pActiveSkill==NULL)
	{
		return;
	}
	AI_EVENT_SENDER( GetAI() ).OnSkillEnd( pActiveSkill->GetParam()->GetAddSub() );
	//GetAI()->OnSkillEnd(pActiveSkill->GetParam()->GetAddSub());
	CNewSkillFactory::RealeaseBaseMoudule(pActiveSkill->GetParam());
}

//���״̬������
void	CMoveShape::AddStateCount(stModuParam *pState)
{
	tagModuleKey tgState;
	tgState.dwID = pState->GetID();
	tgState.dwLvl = pState->GetLv();

	map<tagModuleKey,long,tagKeycmp>::iterator it = m_mpStateCount.find(tgState);
	if (it!=m_mpStateCount.end())
	{
		++((*it).second);
	}
	else
	{
		m_mpStateCount[tgState] = 1;
	}
}
//��״̬������
void	CMoveShape::SubStateCount(stModuParam *pState)
{
	tagModuleKey tgState;
	tgState.dwID = pState->GetID();
	tgState.dwLvl = pState->GetLv();

	map<tagModuleKey,long,tagKeycmp>::iterator it = m_mpStateCount.find(tgState);
	if (it!=m_mpStateCount.end())
	{
		if ((*it).second >= 1)
		{
			--((*it).second);
			if ((*it).second<=0)
			{
				it = m_mpStateCount.erase(it);
			}
		}
	}	
}
//��ȡ״̬����
long	CMoveShape::GetStateCount(stModuParam *pState)
{
	tagModuleKey tgState;
	tgState.dwID = pState->GetID();
	tgState.dwLvl = pState->GetLv();
	map<tagModuleKey,long,tagKeycmp>::iterator it = m_mpStateCount.find(tgState);
	if (it!=m_mpStateCount.end())
	{
		return (*it).second;
	}
	return 0;
}

bool   CMoveShape::GetStateCount(eSkillAttackType Type)
{
    for ( itModu it = m_BuffParam.begin() ; it != m_BuffParam.end() ; it++ )
    {
        /// ͬ���͵�
        if ( it->second->GetAttackType() == Type )
        {
            return true;
        }
    }

    for ( itModu it = m_DeBuffParam.begin();it != m_DeBuffParam.end();it++ )
    {
        /// ͬ���͵�
        if ( it->second->GetAttackType() == Type )
        {
            return true;
        }
    }
    return false;
}

long  CMoveShape::GetSkillValue(long skill,long lvl,std::string name,long xPos,long yPos)
{
    if ( name == "FutureTime")
    {
        long   Dis = RealDistance( xPos,yPos );
        return GetInst(SkillAttribute).GetStrCondif(skill,lvl,name,Dis);
    }
    return GetInst(SkillAttribute).GetStrCondif(skill,lvl,name);
}

//����ID����״̬
void CMoveShape::EndBuff(DWORD dwStateID)
{
	//����Buff
	itModu it= m_BuffParam.begin();
	for(;it!=m_BuffParam.end();)
	{
		if (dwStateID==(*it).second->GetID())
		{
			CBaseModule *pState = CNewSkillFactory::GetBaseModule((*it).second);
			if (pState)
			{
				EndState(pState);
				it = m_BuffParam.erase(it);
			}			
		}
		else
		{
			++it;
		}
	}
}
//�����������
void CMoveShape::ClearPassiveSkill()
{	

	itModu it=m_PassiveSkillParam.begin();
	for(;it!=m_PassiveSkillParam.end();)
	{	
#ifdef _RUNSTACKINFO1_
		char pszStatckInfo[1024]="";
		long lID = (*it).second->GetID();	
		_snprintf(pszStatckInfo,1024,"CMoveShape::ClearPassiveSkill(ID:%d) Start",lID);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif
		CNewSkillFactory::RealeaseBaseMoudule((*it).second);		
		it = m_PassiveSkillParam.erase(it);

#ifdef _RUNSTACKINFO1_		
		_snprintf(pszStatckInfo,1024,"CMoveShape::ClearPassiveSkill(ID:%d) End",lID);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), pszStatckInfo);
#endif
	}
	m_PassiveSkillParam.clear();
}
//����������������
void CMoveShape::EndAllActiveSkill()
{
	itModu it=m_ActiveSkillParam.begin();
	for(;it!=m_ActiveSkillParam.end();)
	{	
#ifdef _RUNSTACKINFO1_
		char pszStatckInfo[1024]="";
		long lID = (*it).second->GetID();	
		_snprintf(pszStatckInfo,1024,"CMoveShape::EndAllActiveSkill(ID:%d) Start",lID);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

		CBaseModule *pActiveSkill = CNewSkillFactory::GetBaseModule((*it).second);
		if (pActiveSkill)
		{
#ifdef _RUNSTACKINFO1_			
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "this->GetAI()->OnSkillEnd Start");
#endif
			AI_EVENT_SENDER( GetAI() ).OnSkillEnd( (*it).second->GetAddSub() );
			//this->GetAI()->OnSkillEnd((*it).second->GetAddSub());	
#ifdef _RUNSTACKINFO1_			
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "this->GetAI()->OnSkillEnd End");
#endif
			pActiveSkill->End();	
#ifdef _RUNSTACKINFO1_			
			CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), "pActiveSkill->End() End");
#endif
		}
		CNewSkillFactory::RealeaseBaseMoudule((*it).second); 		
		it = m_ActiveSkillParam.erase(it);	

#ifdef _RUNSTACKINFO1_		
		_snprintf(pszStatckInfo,1024,"CMoveShape::EndAllActiveSkill(ID:%d) End",lID);
		CMessage::AsyWriteFile(GetGame()->GetStatckFileName(), pszStatckInfo);
#endif
	}
	m_ActiveSkillParam.clear();
}


//����״̬
void CMoveShape::EndState(CBaseModule *pBaseModule)
{

	if (pBaseModule)
	{
		pBaseModule->GetParam()->ProcNumSub();
		pBaseModule->End();				
		SubStateCount(pBaseModule->GetParam());
		OnDelBuff(pBaseModule->GetID(),pBaseModule->GetLv());
		CNewSkillFactory::RealeaseBaseMoudule(pBaseModule->GetParam());
	}
}

//����DEBUFF
void CMoveShape::EndDeBuff()
{
	itModu it= m_DeBuffParam.begin();

	for(;it!=m_DeBuffParam.end();)
	{
		CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty((*it).second->GetID());
		CBaseModule *pState = CNewSkillFactory::GetBaseModule((*it).second);

		if (pSkillProperty && pSkillProperty->GetSystem()==0 && pState) 
		{
			EndState(pState);
			it = m_DeBuffParam.erase(it);			
			continue;
		}
		++it;
	}
}

//����״̬
void CMoveShape::EndState(mapModules &mapState)													
{
	itModu it= mapState.begin();	
	for(;it!=mapState.end();)
	{
		CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty((*it).second->GetID());
		CBaseModule *pState = CNewSkillFactory::GetBaseModule((*it).second);
		if (pState && pSkillProperty && pSkillProperty->GetDeadEnd() == 0 && pSkillProperty->GetSystem()==0) 
		{
			EndState(pState);				
			it = mapState.erase(it);			
			continue;
		}
		++it;
	}	
}

//�������е�״̬
void CMoveShape::EndBuffAndDebuff()
{	
	//����buff
	EndState(m_BuffParam);
	//����debuff
	EndState(m_DeBuffParam);
}

//���״̬
void CMoveShape::ClearState(mapModules &mapState)
{
	itModu it= mapState.begin();	
	for(;it!=mapState.end();++it)
	{

		SubStateCount((*it).second);

		CNewSkillFactory::RealeaseBaseMoudule((*it).second);			
	}
	mapState.clear();
}



//�������״̬
void CMoveShape::ClearAllBuff()
{
	//���BUFF
	ClearState(m_BuffParam);
	//���DEBUFF
	ClearState(m_DeBuffParam);	
}

//����״̬
void CMoveShape::StartUpState(mapModules &mapState)
{
	itModu it= mapState.begin();
	for(;it!=mapState.end();)
	{
		CBaseModule* pState = CNewSkillFactory::GetBaseModule((*it).second);	
		if (pState && (*it).second->GetbRun()==false)
		{
			if(pState->Begin() == MR_Step_End)
			{
				(*it).second->SetbRun(true);				
				++it;
			}
			else
			{			
				SubStateCount(pState->GetParam());
				CNewSkillFactory::RealeaseBaseMoudule((*it).second);
				it = mapState.erase(it);
			}			
		}
		else
		{		
			++it;
		}
	}
}

//���¿�ʼ���е�״̬
void CMoveShape::StartAllBuff()
{
	//����Buff
	StartUpState(m_BuffParam);
	//����DeBuff
	StartUpState(m_DeBuffParam);
}

//���洦��
void CMoveShape::OnBeenAddAppendSkill(CMoveShape *pAttacker)
{
	if( IsDied() ) return;
	//PKֵ�趨
	if (pAttacker->GetExID()!=GetExID())
	{
		GetPKSys()->SetPKValue(pAttacker,this,1);
	}
}
bool	CMoveShape::OnIncreaseHp(long &lHp, CMoveShape *firer )
{
	string str("Hp");
	if (GetHP()==GetMaxHP())
	{
		return false;
	}

	long lCurrentHp = GetHP()+lHp;
	if (lCurrentHp > (long)GetMaxHP())
	{
		lCurrentHp = GetMaxHP();
		lHp = GetMaxHP() - GetHP();
	}
	SetAttribute(str,lCurrentHp);
	UpdateAttribute(1);
	return true;
}

//���洦��
long CMoveShape::OnBeenAttacked( CMoveShape* pAttacker,long lHp,long lDamageResult)
{
    if( CanReMoveDamage() == 0 )
        return 0;

	if(pAttacker==NULL)
		return 0;

	if(TYPE_PLAYER == GetType() && TYPE_PLAYER == pAttacker->GetType())
		GetLogicLogInterface()->logT130_player_attack_log((CPlayer*)pAttacker, (CPlayer*)this);

	//����Ѿ������ͷ���
	if(IsDied() || pAttacker->IsDied()) 
		return 0;

	SetAttacker(pAttacker->GetType(),pAttacker->GetExID());
	if (lHp>0 && lDamageResult!=DAMAGE_RESULT_DamageEx
		&& lDamageResult!=DAMAGE_RESULT_DotaDamage)
	{		
		ProduceEvent(ET_BeAttack,0,0,0,0,pAttacker);//
		ProduceEvent(ET_BeenDamage);
		pAttacker->ProduceEvent(ET_Damage);
	}

	DWORD dwHp = GetHP();
	if( (long)dwHp > lHp)
	{
		if( TYPE_PLAYER == this->GetType() && TYPE_PLAYER == pAttacker->GetType() )// ����ҹ���
			((CPlayer*)this)->SendNotifyWarningMsg(false, (CPlayer*)pAttacker);

		//pkֵ�趨
		if (pAttacker->GetExID()!=GetExID())
		{
			GetPKSys()->SetPKValue(pAttacker,this,2);
		}

		// ��ӷ����PK״̬�ж���@Expter 20090602
		if (TYPE_PLAYER == GetType())
		{
			/// ��Ҽ�PK
			if (TYPE_PLAYER == pAttacker->GetType())
			{
				/// ��Ҽ�PK,���������,һ��pk�¼�
				GetInst(AntiCheat::CPlayerManager).PlayerFight(dynamic_cast<CPlayer*>(this));	
				GetInst(AntiCheat::CPlayerManager).PlayerFight(dynamic_cast<CPlayer*>(pAttacker));
			}
			/// ������������ж��ǹ���
			else if (TYPE_MONSTER == pAttacker->GetType())
			{
				CMonster * pGuardAttacker = dynamic_cast<CMonster*>(pAttacker);
				/// �������
				if (pGuardAttacker->GetNpcType() == NT_Guard)
				{
					/// ���������PK,���������,һ��pk�¼�
					GetInst(AntiCheat::CPlayerManager).PlayerFight(dynamic_cast<CPlayer*>(this));	
				}
			}
		}
		// ����ң�������������ս����Ծ��
		if( pAttacker->GetType() == TYPE_PLAYER )
		{
			GetInst( AntiCheat::CPlayerManager ).PlayerAttack( static_cast<CPlayer*>( pAttacker ) );
		}

		string str("Hp");
		SetAttribute(str,dwHp-lHp);
		UpdateAttribute(1);
        
        //##���˺��AI������Ҫ�ǹ���ʹ�á�
		if (pAttacker!=NULL)
		{
            if (GetAI())
			{
                /// ��������λ��Player,���ʣ���Ѫ��С��lAgonal�����ٷֱȵ�ʱ�򣬽������״̬
                if( TYPE_PLAYER == GetType() && GetPhaseID()!=NULL_GUID && ((dwHp-lHp)< ( (GetMaxHP()*GetInst(PhaseSetup).GetConfig().lAgonal)/100 ) ) )
                {
                    SetState( STATE_AGONAL );
                    PHASEMAN.OnPhaseEvent(GetPhaseID(),static_cast<CPlayer*>(this),eAgonal);
                    /// �������״̬
                    AI_EVENT_SENDER( GetAI() ).WhenPlayerAgonal();
                }
                else
                {
                    AI_EVENT_SENDER( GetAI() ).WhenBeenHurted( pAttacker->GetType(), pAttacker->GetExID(), lHp );
                }
				//GetAI()->WhenBeenHurted(pAttacker->GetType(),pAttacker->GetExID(),lHp);
			}
			OnBeenHurted( pAttacker->GetType(),pAttacker->GetExID()) ;
		}
	}
	else
	{
		if( TYPE_PLAYER == this->GetType() && TYPE_PLAYER == pAttacker->GetType() )// ����ҹ���
			((CPlayer*)this)->SendNotifyWarningMsg(true, (CPlayer*)pAttacker);

        /// Expter���ӱ���ɱ��ֻ��Ծ�����λ,���Pk
        if( TYPE_PLAYER == this->GetType() && GetPhaseID()!=NULL_GUID 
            && (long)(GetInst(PhaseSetup).GetConfig().lHpNum*dwHp) <= lHp 
            && PHASEMAN.GetPhaseType(GetPhaseID()) == ePk )
        {
            PHASEMAN.OnPhaseEvent(GetPhaseID(),static_cast<CPlayer*>(this),eSecKill);
        }

		SetState(STATE_DIED);
		SetHP(0);	
		SetKillerInfo(pAttacker->GetExID(),pAttacker->GetType());
		pAttacker->ProduceEvent(ET_Kill);//ע��ɱ���¼�
		ProduceEvent(ET_BeenKilled);     //ע�ᱻɱ���¼�
		ProduceEvent(ET_EventBreak);
		return 1;
	}
	return 0;
}

//��ɱ����ش���
void	CMoveShape::OnBeenKilled(CMoveShape* pAttacker)
{
	//##��ɱ�Ĵ���, ��������
	if (pAttacker!=NULL)
	{
		OnBeenHurted( pAttacker->GetType(),pAttacker->GetExID()) ;
		OnBeenMurdered(  pAttacker->GetType(), pAttacker->GetExID() );
	}		
	//����
	ChangeStateTo((CShape::eSTATE)STATE_DIED);
	SetAction( ACT_ALREADYDIED );
	//ֹͣ���м���
	StopAllSkills();
	//��������״̬
	EndBuffAndDebuff();	
	SetKilledMeAttackInfo(pAttacker->GetType(),pAttacker->GetExID());

	if (GetAI() &&  pAttacker!=NULL)
	{
		AI_EVENT_SENDER( GetAI() ).WhenBeenKilled( pAttacker->GetType(), pAttacker->GetExID() );
		//GetAI()->WhenBeenKilled(pAttacker->GetType(),pAttacker->GetExID());		
	}
}

//���ù����ߵ����ͺ�guid
void CMoveShape::SetAttacker(long lType,const CGUID guid)
{
	m_lAttackerType = lType;
	m_guidAttacker = guid;
}

//���ݼ�ֵ��ȡ�������ܶ���
CBaseModule *CMoveShape::GetActiveSkill(long lActiveID)
{

	itModu it = m_ActiveSkillParam.find(lActiveID);
	if (it!=m_ActiveSkillParam.end())
	{
		return CNewSkillFactory::GetBaseModule((*it).second);
	}
	return NULL;
}

//����buff�����õ��ı����
bool CMoveShape::AddBuffToByteArray( vector<BYTE>* pByteArray)
{
	vector<BYTE> TemptByteArray;TemptByteArray.clear();
	//buff
	DWORD dwSize = (DWORD)m_BuffParam.size() + (DWORD)m_DeBuffParam.size();
	_AddToByteArray(&TemptByteArray,dwSize);

	itModu it= m_BuffParam.begin();
	for(; it!=m_BuffParam.end(); it++)
	{

		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetID());
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetLv());

		if (NULL==(*it).second->GetFirer())
		{
			_AddToByteArray(&TemptByteArray,(long)0);
			_AddToByteArray(&TemptByteArray,NULL_GUID);
		}
		else
		{
			_AddToByteArray(&TemptByteArray,(long)(*it).second->GetFirerType());
			_AddToByteArray(&TemptByteArray,(*it).second->GetFirerID());
		}
		(*it).second->AddToByteArray(&TemptByteArray);
	}
	it= m_DeBuffParam.begin();
	for(; it!=m_DeBuffParam.end(); it++)
	{
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetID());
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetLv());
		if (NULL==(*it).second->GetFirer())
		{
			_AddToByteArray(&TemptByteArray,(long)0);
			_AddToByteArray(&TemptByteArray,NULL_GUID);
		}
		else
		{
			_AddToByteArray(&TemptByteArray,(long)(*it).second->GetFirerType());
			_AddToByteArray(&TemptByteArray,(*it).second->GetFirerID());
		}
		(*it).second->AddToByteArray(&TemptByteArray);
	}


	//ѹ
	DWORD dwDataSize = (DWORD)TemptByteArray.size();
	_AddToByteArray(pByteArray,dwDataSize);
	_AddToByteArray(pByteArray,&TemptByteArray[0],dwDataSize);
	return true;
}


bool CMoveShape::CodeToDataBlock_ForClient( DBWriteSet& setWriteDB, bool bExData)
{
	CShape::CodeToDataBlock_ForClient( setWriteDB,bExData );	
	setWriteDB.AddToByteArray(GetSpeedChangedRatio());                      //�ٶȵı仯����
	setWriteDB.AddToByteArray(static_cast<BYTE>(GetIsRun()));	            //�ƶ���ʽ
	setWriteDB.AddToByteArray(static_cast<BYTE>(GetHide()));				//����
	setWriteDB.AddToByteArray(static_cast<BYTE>(GetInvisibleState()));		//����	
	setWriteDB.AddToByteArray(static_cast<BYTE>(GetPretendDeadState()));	//װ��
	setWriteDB.AddToByteArray(static_cast<BYTE>(GetIsRide()));				//������״̬
    setWriteDB.AddToByteArray(IsCanBeenAttack());			                //����������־��
    setWriteDB.AddToByteArray(GetPhaseID());                                //��λID����

	if(bExData)
	{
		setWriteDB.AddToByteArray(static_cast<BYTE>(GetAntiInvisibleable()));   //�Ƿ�����
		setWriteDB.AddToByteArray(static_cast<BYTE>(GetUseRabbSkill()));		//ʹ�÷�ʦ����
		setWriteDB.AddToByteArray(static_cast<BYTE>(GetUseFightSkill()));		//ʹ��սʿ����
		setWriteDB.AddToByteArray(static_cast<BYTE>(GetInstruction()));		    //ʹ��ָ��
		setWriteDB.AddToByteArray(static_cast<BYTE>(GetForceAttack()));		    //ǿ�ƹ���

        setWriteDB.AddToByteArray(CanMove());				        //�ƶ���־��
		setWriteDB.AddToByteArray(GetRandomMove());		            //����ܶ�
        setWriteDB.AddToByteArray(CanControlSelf());				//��������
        setWriteDB.AddToByteArray(CanReMoveDamage());		        //����˺�Ч��
        setWriteDB.AddToByteArray(IsCanUseAllSkill());				//�ܷ�ʹ�ü���
	}

	setWriteDB.AddToByteArray(GetChangeModelType());
	setWriteDB.AddToByteArray(GetChangeModelID());
	setWriteDB.AddToByteArray( (BYTE)IsDied());
	////���buff
	if(!bExData)
	{
		CodeBuffToDataBlock_ForClient(setWriteDB);
		////���debuff
		CodeDeBuffToDataBlocky_ForClient(setWriteDB);
	}
	if(bExData)
		CodeOnOffSkillToDataBlock_ForClient(setWriteDB);
	return true;
}

bool CMoveShape::CodeBuffToDataBlock_ForClient( DBWriteSet& setWriteDB)
{
	DWORD dwSize = (DWORD)m_BuffParam.size();
	setWriteDB.AddToByteArray(dwSize);

	itModu it= m_BuffParam.begin();
	for(;it!=m_BuffParam.end();it++)
	{		
		if ((*it).second!=NULL)
		{
			(*it).second->CodeToDataBlockForClient(setWriteDB,ModuType_Buff);
		}
	}
	return true;
}

bool CMoveShape::CodeDeBuffToDataBlocky_ForClient( DBWriteSet& setWriteDB)
{
	DWORD dwSize = (DWORD)m_DeBuffParam.size();
	setWriteDB.AddToByteArray(dwSize);

	itModu it= m_DeBuffParam.begin();
	for(;it!=m_DeBuffParam.end();it++)
	{		
		if ((*it).second!=NULL)
		{
			(*it).second->CodeToDataBlockForClient(setWriteDB,ModuType_DeBuff);
		}		
	}
	return true;
}

bool CMoveShape::CodeActSkillToDataBlock_ForClient( DBWriteSet& setWriteDB)
{
	if (m_ActiveSkillParam.size()>0)
	{
		setWriteDB.AddToByteArray((BYTE)1);
	} 
	else
	{
		setWriteDB.AddToByteArray((BYTE)0);
		return false;
	}

	setWriteDB.AddToByteArray((long)(m_ActiveSkillParam.size()));
	for (itModu it=m_ActiveSkillParam.begin(); 
		it!=m_ActiveSkillParam.end(); ++it)
	{
		setWriteDB.AddToByteArray((long)(it->first));//��ֵ
		it->second->CodeToDataBlockForClient(setWriteDB,ModuType_Skill_Active);
	}
	return true;
}

bool CMoveShape::CodeOnOffSkillToDataBlock_ForClient( DBWriteSet& setWriteDB)
{
	DWORD dwSize = 0;
	setWriteDB.AddToByteArray(dwSize);	
	return true;
}

bool CMoveShape::CodeBuffToDataBlock(DBWriteSet& setWriteDB)
{
	vector<BYTE> TemptByteArray;TemptByteArray.clear();
	//buff
	DWORD dwSize = (DWORD)m_BuffParam.size()+ (DWORD)m_DeBuffParam.size();
	_AddToByteArray(&TemptByteArray,dwSize);
	itModu it= m_BuffParam.begin();
	for(;it!=m_BuffParam.end();it++)
	{

		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetID());
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetLv());

		if (NULL==(*it).second->GetFirer())
		{
			_AddToByteArray(&TemptByteArray,(long)0);
			_AddToByteArray(&TemptByteArray,NULL_GUID);
		}
		else
		{
			_AddToByteArray(&TemptByteArray,(long)(*it).second->GetFirerType());
			_AddToByteArray(&TemptByteArray,(*it).second->GetFirerID());
		}
		(*it).second->AddToByteArray(&TemptByteArray);
	}
	it= m_DeBuffParam.begin();
	for(;it!=m_DeBuffParam.end();it++)
	{
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetID());
		_AddToByteArray(&TemptByteArray,(long)(*it).second->GetLv());
		if (NULL==(*it).second->GetFirer())
		{
			_AddToByteArray(&TemptByteArray,(long)0);
			_AddToByteArray(&TemptByteArray,NULL_GUID);
		}
		else
		{
			_AddToByteArray(&TemptByteArray,(long)(*it).second->GetFirerType());
			_AddToByteArray(&TemptByteArray,(*it).second->GetFirerID());
		}
		(*it).second->AddToByteArray(&TemptByteArray);
	}

	//ѹ
	DWORD dwDataSize = (DWORD)TemptByteArray.size();
	setWriteDB.AddToByteArray(dwDataSize);
	setWriteDB.AddToByteArray(&TemptByteArray[0],dwDataSize);
	return true;
}

bool CMoveShape::DecodeBuffFromDataBlock(DBReadSet& setReadDB)
{
	//�����ݳ���
	DWORD dwDataSize = setReadDB.GetDwordFromByteArray();
	if(dwDataSize==0)	return true;

	//��ѹbuff
	DWORD dwSize = setReadDB.GetDwordFromByteArray();
	for(DWORD i=0;i<dwSize;i++)
	{
		long lID =  setReadDB.GetLongFromByteArray();
		long lLvl = setReadDB.GetLongFromByteArray();
		int nType = setReadDB.GetLongFromByteArray();
		CGUID guid;
		setReadDB.GetBufferFromByteArray(guid);


		long nProceNum = setReadDB.GetLongFromByteArray();
		long lTimerNum = setReadDB.GetLongFromByteArray();

		list<stTimerParam*> Timers;
		for(int n=0;n<lTimerNum;n++)
		{
			stTimerParam *pTimerParam = OBJ_CREATE(stTimerParam);
			pTimerParam->nCalledCount = 0;
			pTimerParam->lDestType = GetType();
			pTimerParam->DestID = GetExID();
			pTimerParam->lTimerID = -1;
			pTimerParam->lFlag = setReadDB.GetLongFromByteArray();
			pTimerParam->lFuture = setReadDB.GetDwordFromByteArray();
			pTimerParam->nCount = setReadDB.GetDwordFromByteArray();
			Timers.push_back(pTimerParam);
		}

		CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(lID);


		if (pSkillProperty && (pSkillProperty->GetType()==ModuType_Buff || pSkillProperty->GetType()==ModuType_DeBuff))
		{
			++m_ActiveID;
			//�õ�һ����̬����
			stModuParam* pStateParam = InitModuParam(lID, lLvl, m_ActiveID);
			if (pStateParam)
			{
				list<stTimerParam*>::iterator it = Timers.begin();
				if (pStateParam->GetTime())
				{
					time_t t_Current = time(NULL);
					long lFuture = ((*it)->lFuture - t_Current)*1000;
					if (lFuture<0)
					{
						(*it)->lFuture = 0;
					}
					else
					{
						(*it)->lFuture = lFuture;
					}
				}

				pStateParam->Reset(this,NULL);
				pStateParam->DecodeFromDataBlock(nProceNum,Timers);
				

				if(pStateParam->GetModType() == ModuType_Buff)
				{
					AddStateCount(pStateParam);
					m_BuffParam[pStateParam->GetGlobleID()] = pStateParam;				
				}
				else if(pStateParam->GetModType() == ModuType_DeBuff)	
				{
					AddStateCount(pStateParam);
					m_DeBuffParam[pStateParam->GetGlobleID()] = pStateParam;			
				}
			}		
		}
	}
	return true;
}

void CMoveShape::ChangeStateTo(CShape::eSTATE state)
{
	SetState( state );

	CMessage msg( MSG_S2C_SHAPE_CHANGESTATE );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( static_cast<LONG>(state) );
	msg.SendToAround( this );

	if (state==STATE_FIGHT)
	{
		ProduceEvent(ET_Fighting);
	} 
	else if (state==STATE_PEACE)
	{
		ProduceEvent(ET_Peace);
	}
}

void CMoveShape::OnEnterRegion(bool bIsRelive)
{
	CRegion* pRegion = dynamic_cast<CRegion*>( GetFather() );

	if( pRegion == NULL )
	{

		assert( 0 );
		return;
	}

	//##���ü�����
	m_lMoveableCount	= 0;
	m_sMoveable			= 1;
    m_sMoveDamage       = 1;
    m_sUseSkill         = 1;
    m_sBeenAttack       = 1;
}

//������������
void	CMoveShape::StopPassiveSkills()
{	
	itModu it=m_PassiveSkillParam.begin();
	for (; it!=m_PassiveSkillParam.end(); ++it)
	{		
		CBaseModule *pSkill = CNewSkillFactory::GetBaseModule((*it).second);
		EndSkill(pSkill);
	}
	m_PassiveSkillParam.clear();
}

//��������
void	CMoveShape::StopSkills(long lID,long lLv,long lType)
{
	if (lType==ModuType_Skill_Active)
	{
		StopActiveSkill(lID,lLv);
	}
	else if (lType==ModuType_Skill_Passive)
	{
		StopPassiveSkill(lID,lLv);
	}
}

//��������
void CMoveShape::EndSkill(CBaseModule *pBaseModule)
{
	if (pBaseModule)
	{
		pBaseModule->End();
		CNewSkillFactory::RealeaseBaseMoudule(pBaseModule->GetParam());
	}
}

//������������
void CMoveShape::StopActiveSkill(long lID,long lLv)
{
	//tagModuleKey tgKey;
	itModu it=m_ActiveSkillParam.begin();
	for (; it!=m_ActiveSkillParam.end();)
	{	
		CBaseModule *pSkill = CNewSkillFactory::GetBaseModule((*it).second);
		if (pSkill)
		{
			if (lLv==0)
			{
				if (lID=it->second->GetID())
				{				
					EndSkill(pSkill);
					it = m_ActiveSkillParam.erase(it);
				}	
				else
				{
					++it;
				}
			}
			else
			{
				if (lID==it->second->GetID() && lLv==it->second->GetLv())
				{
					EndSkill(pSkill);
					it = m_ActiveSkillParam.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		else
		{
			++it;
		}
	}
}
//ֹͣ��������
void CMoveShape::StopPassiveSkill(long lID,long lLv)
{

	itModu it=m_PassiveSkillParam.begin();
	for (; it!=m_PassiveSkillParam.end();)
	{	

		CBaseModule *pSkill = CNewSkillFactory::GetBaseModule((*it).second);
		if (pSkill)
		{
			if (lLv==0)
			{
				if (lID=it->second->GetID())
				{				
					EndSkill(pSkill);
					it = m_PassiveSkillParam.erase(it);
				}
				else
				{
					++it;
				}
			}
			else
			{
				if (lID==it->second->GetID() && lLv==it->second->GetLv())
				{
					EndSkill(pSkill);
					it = m_PassiveSkillParam.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
		else
		{
			++it;
		}


	}
}


void CMoveShape::StopAllSkills()
{	
	itModu it=m_ActiveSkillParam.begin();
	for(;it!=m_ActiveSkillParam.end();)
	{	
		CBaseModule *pSkill = CNewSkillFactory::GetBaseModule((*it).second);
		if (pSkill)
		{
			EndSkill(pSkill);
			it = m_ActiveSkillParam.erase(it);
		}
		else
		{
			++it;
		}

	}	
}
//�ͷ��������ܲ���
void CMoveShape::ClearActiveSkills()
{
	if (m_ActiveSkillParam.size()>0)
	{
		itModu it=m_ActiveSkillParam.begin();
		for(;it!=m_ActiveSkillParam.end();++it)
		{		
			CNewSkillFactory::RealeaseBaseMoudule((*it).second); 				
		}
		m_ActiveSkillParam.clear();
	}
}

void CMoveShape::ClearAllStates(bool bIsDead)
{

}

void CMoveShape::StartAllStates(bool bIsRelive)
{
	CRegion* pRegion = dynamic_cast<CRegion*>( GetFather() );	
}

void CMoveShape::OnAction( tagAction a )
{

}

//##Ӳֱ
DWORD CMoveShape::Stiffen( WORD wLoseHP )
{
	//##��ȡȫ������
	GlobalSetup::tagSetup* pSetup = GlobalSetup::GetSetup();
	if( !pSetup ) return 0;

	//##���ʱ���Ƿ�ͷ�Ҫ��
	if( timeGetTime() > m_dwStiffenStartedTimeStamp + pSetup -> dwStiffBoundTime )
	{
		//##����Ѿ���Խ�˱߽磬�����¿�ʼ
		m_dwStiffenStartedTimeStamp = timeGetTime();
		m_lStiffenCounts			= 0;
	}
	else if( m_lStiffenCounts >= static_cast<LONG>(pSetup -> dwStiffLimit) )
	{
		//##�����λʱ���ڣ������Ѿ��ﵽ�����ƴ������򷵻�
		return 0;
	}

	//##Ӳֱ�ĸ��ʼ���
	LONG lProbability = 0;
	FLOAT fPercent = static_cast<FLOAT>( wLoseHP ) / GetMaxHP();
	for( DWORD i = 0; i < pSetup -> wNumStiffSetup; i ++ )
	{
		if( fPercent <= pSetup -> fStiff_Damage[i] )
		{
			lProbability = pSetup -> wStiff_Probability[i];
			break;
		}
	}

	//##��������ֵ�Ը��ʵ�Ӱ��
	lProbability -= GetReAnk();

	//##���������Χ����ֱ�ӷ���
	if( random( 100 ) > lProbability ) return 0;

	//##���Խ���Ӳֱ����
	++ m_lStiffenCounts;

	return pSetup -> dwStiffDelayTime;
}



void CMoveShape::OnChangeStates()
{
	CMessage msg( MSG_S2C_SKILL_STATE_CHANGE );
	msg.Add( GetType() );
	msg.Add( GetExID() );
	msg.Add( GetHP() );
	msg.Add( static_cast<WORD>(0) );
	msg.Add( static_cast<WORD>(0) );
	msg.Add( static_cast<WORD>(0) );
	msg.SendToAround( this );
}

BOOL CMoveShape::IsFaceTo( CMoveShape* pShape )
{
	if( pShape )
	{
		if( Distance( pShape ) == 1 )
		{
			LONG lX = GetTileX();
			LONG lY = GetTileY();
			GetDirPos( GetDir(), lX, lY );

			if( lX != pShape -> GetTileX() ||
				lY != pShape -> GetTileY() )
			{
				return FALSE;
			}

			lX = pShape -> GetTileX();
			lY = pShape -> GetTileY();
			pShape -> GetDirPos( pShape -> GetDir(), lX, lY );

			if( lX != GetTileX() ||
				lY != GetTileY() )
			{
				return FALSE;
			}

			//##���ȫ��ͨ����������
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

void CMoveShape::OnMove(float fNewX, float fNewY)
{
	ProduceEvent(ET_Move);
	ProduceEvent(ET_EventBreak);

	//��Ӳ��Դ���
	CRegion* p=dynamic_cast<CRegion*>(GetFather());
	if (fNewX<0 || fNewX> p->GetWidth() || fNewY<0 || fNewY> p->GetHeight())
	{
		Log4c::Warn(ROOT_MODULE, "Խ���¼ x=%f,y=%f\n",fNewX,fNewY);
	}

/*	SetDir(GetLineDir(GetTileX(),GetTileY(),(long)fNewX,(long)fNewY));

	CMessage msg(MSG_S2C_SHAPE_MOVE);
	msg.Add( GetExID() );
	msg.Add( (short)fNewX );
	msg.Add( (short)fNewY );
	if(GetType() == TYPE_MONSTER )
		msg.SendToAround(this);
	else
		msg.SendAroundLimitPlayer(this,NULL_GUID,true);*/		
    CMessage msg( MSG_S2C_SHAPE_MOVE );
    msg.Add( GetExID() );
    msg.Add( (uchar)0 );
    msg.Add( fNewX );
    msg.Add( fNewY );
    if( GetType() == TYPE_PLAYER )
    {
        msg.SendAroundLimitPlayer( this, NULL_GUID, true );
    }
    else
    {
        msg.SendToAround( this );
    }
}

void CMoveShape::OnStopMove()
{
	//CMessage msg(MSG_S2C_SHAPE_STOPMOVE);
	//msg.Add( GetExID() );
	//msg.Add( (short)GetTileX() );
	//msg.Add( (short)GetTileY() );
	//msg.Add( (BYTE)GetDir() );
	//msg.SendToAround(this);
    CMessage msg( MSG_S2C_SHAPE_MOVE );
    msg.Add( GetExID() );
    msg.Add( (uchar)1 );
    msg.Add( GetPosX() );
    msg.Add( GetPosY() );
    msg.SendToAround( this );
}	

void CMoveShape::OnCannotMove()
{
}

void CMoveShape::SetPosition( float x, float y )
{

	//##���ж��Ƿ���Թ�ȥ.Ȼ���ٹ�ȥ
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		if( x < pRegion -> GetWidth() && 
			x >= 0 &&
			y < pRegion -> GetHeight() &&
			y >= 0 && 
			pRegion -> GetBlock((long)x, (long)y) == CRegion::BLOCK_NO )
		{			
			CMessage msg( MSG_S2C_SHAPE_SETPOS );
			msg.Add( GetType() );
			msg.Add( GetExID() );
			msg.Add( (float)x );
			msg.Add( (float)y );
			msg.Add( static_cast<LONG>(0) );
			msg.SendToAround( this );

			SetPosXY( x, y );
		}
	}
}

void CMoveShape::SetPosition( long x, long y )
{
    SetPosition( x + 0.5f, y + 0.5f );
}

void CMoveShape::SetSlipPos(float x, float y,float fSpeed,float fDir)
{
	//##���ж��Ƿ���Թ�ȥ.Ȼ���ٹ�ȥ
	CServerRegion* pRegion = dynamic_cast<CServerRegion*>( GetFather() );
	if( pRegion )
	{
		if( x < pRegion -> GetWidth() && 
			x >= 0 &&
			y < pRegion -> GetHeight() &&
			y >= 0 && 
			pRegion -> GetBlock((long)x, (long)y) == CRegion::BLOCK_NO )
		{
			ProduceEvent(ET_Move);
			ProduceEvent(ET_EventBreak);
			CMessage msg( MSG_S2C_SHAPE_SLIPPOS );
			msg.Add( GetType() );
			msg.Add( GetExID() );
			msg.Add( x );
			msg.Add( y );
			msg.Add( fSpeed );
			msg.Add( fDir);
			msg.SendToAround( this );

			SetPosXY( x, y );
		}
	}
}

void CMoveShape::SetSlipPos(long x, long y,float fSpeed,float fDir)
{
    SetSlipPos( x + 0.5f, y + 0.5f, fSpeed, fDir );
}


tagSkillID CMoveShape::GetDefaultAttackSkillID()
{	
	tagSkillID tID = SKILL_BASE_ATTACK;	
	return tID;
}

BOOL CMoveShape::AddSkill( const char* strName, LONG lLevel )
{
	if( strName == NULL ) 
		return FALSE;	
	return TRUE;
}

BOOL CMoveShape::AddSkill( tagSkillID tID, LONG lLevel )
{	
	return true;
}



BOOL CMoveShape::DelSkill( const char* pSkillName )
{	
	return TRUE;
}

BOOL CMoveShape::DelSkill( long lSkillID ,long lSkillLv )
{	
	return TRUE;
}


LONG CMoveShape::CheckSkill( const char* strName )
{
	return 0;
}

LONG CMoveShape::CheckSkill( long lID )
{
	return 0;
}



BOOL CMoveShape::SetSkillLevel( const char* strName, LONG lLevel )
{
	return AddSkill( strName, lLevel );
}

//##��������ʧ���Ĵ���
void CMoveShape::Evanish()
{
}

//����һ���ű���ʱ��
long CMoveShape::RegisterScriptTimer(long lFutureTime,long lInterTime,
									 const char* pszScriptName,long lUserData)
{
	long lTimerID = -1;
	if(NULL == pszScriptName)	return lTimerID;
	tagScriptTimer scripttimer = {lUserData,pszScriptName,lInterTime>0?true:false};
	tagTimerVar* pTimvar = OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_RunScript;
	lTimerID = TIMER_SCHEDULE(this,(void*)pTimvar,timeGetTime()+lFutureTime,lInterTime);
	if(lTimerID != -1)
	{
		m_ScriptTimers[lTimerID]=scripttimer;
	}
	else
	{
		Log4c::Warn(ROOT_MODULE,GetText("GS_MOVESHAPE_1"));//���ƽű���ʱ���ǳ���!
	}
	return lTimerID;
}
//ע��һ���ű���ʱ��
long CMoveShape::UnRegisterScriptTimer(long lUserData,long lTimerID)
{
	long lTimerNum = 0;
	itST it = m_ScriptTimers.begin();
	for(;it != m_ScriptTimers.end();)
	{
		tagScriptTimer &scripttimer = (*it).second;
		if( scripttimer.lUserData == lUserData &&
			(lTimerID == -1 ||  (*it).first == lTimerID) )
		{
			TIMER_CANCEL((*it).first);
			it = m_ScriptTimers.erase(it);
			lTimerNum++;
		}
		else
		{
			it++;
		}
	}
	return lTimerNum;
}

//�ű���ʱ������
void CMoveShape::OnScripttTimeOut(long lTimerID)
{
	itST it = m_ScriptTimers.find(lTimerID);
	if(it != m_ScriptTimers.end())
	{
		//ִ�нű�
		tagScriptTimer &scripttimer = (*it).second;
		//stRunScript sScript;
		//sScript.pszFileName = const_cast<char*>(scripttimer.strScript.c_str());
		//sScript.pRegion = dynamic_cast<CRegion*>(GetFather());
		//sScript.srcShape	= this;
		//sScript.desShape = NULL;
		//RunScript( &sScript, const_cast<CHAR*>( GetGame()->GetScriptFileData(sScript.pszFileName) ) );
		Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ) );
		GetInst( ScriptSys ).RunScript( env, scripttimer.strScript.c_str() );
		if(!scripttimer.bLoop)
			m_ScriptTimers.erase(it);
	}
}
//������нű���ʱ��
void CMoveShape::ClearScriptTimer()
{
	itST it = m_ScriptTimers.begin();
	for(;it != m_ScriptTimers.end();it++)
	{
		TIMER_CANCEL((*it).first);
	}
	m_ScriptTimers.clear();
}

void CMoveShape::StartUpState(CMoveShape *pUser, stModuParam *pModuParam, mapModules &mapState)
{
	CNewSkillProperties *pNSkillProperty = CNewSkillFactory::QueryNewSkillProperty(pModuParam->GetID());
	if (NULL==pNSkillProperty || NULL==pModuParam)
	{
		return;
	}
	long dwID		= pModuParam->GetID();
	long dwLv		= pModuParam->GetLv();
	long dwGlobleID= pModuParam->GetGlobleID();


	itModu it =	mapState.begin();
	for(;it!=mapState.end();)
	{
		stModuParam *pOldState =it->second;

		if(pNSkillProperty->GetOnly()==1)
		{
			//����Ψһ
			CNewSkillProperties *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(pOldState->GetID());
			if (pSkillProperty)
			{
				if( pNSkillProperty->GetOnlyType()==pSkillProperty->GetOnlyType())
				{
					if (pOldState->GetID()==dwID && dwLv < pOldState->GetLv() )
					{	
						//�¼�״̬��ԭ״ID��ͬ�ҵȼ��;�ֱ�ӷ���
						CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
						return;
					}
					else
					{
						//ɾ��ԭ��ͬ���͵�״̬������Ψһ��
						RemoveState(it);
						continue;
					}
				}			
			}	
			++it;
		}
		else if (pNSkillProperty->GetNameOnly()==1)
		{
			if (pOldState->GetID()==dwID)
			{	
				if (dwLv>=pOldState->GetLv())							
				{	
					//ɾ��ԭ����ͬ��͵ȼ���ͬID״̬������Ψһ��
					RemoveState(it);
					continue;
				}			
			}			
			++it;

		}
		else if (pNSkillProperty->GetRoleOnly()==1)
		{
			//��ɫΨһ			
			if (pOldState->GetID()==dwID)
			{				
				if (pModuParam->GetFirerID()==pOldState->GetFirerID())							
				{
					if (dwLv>=pOldState->GetLv())
					{
						//ɾ��ԭ����ͬ��͵ȼ���ͬID״̬����ɫΨһ��
						RemoveState(it);
						continue;
					}								
				}				
			}					
			++it;			
		}
		else if (pNSkillProperty->GetTotalNum()>0)
		{
			//����
			if (pOldState->GetID()==dwID)
			{				
				if (pOldState->GetLv()<dwLv)
				{
					//ɾ��ԭ�е͵ȼ���ͬID״̬���ɵ��ӣ�
					RemoveState(it);
					continue;
				}
				else if(pOldState->GetLv()>dwLv)
				{	
					//��״̬�ȼ�С��ԭ��״̬��ͬID����ֱ�ӷ���
					CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
					return;
				}

				if (GetStateCount(pModuParam)>=(long)pNSkillProperty->GetTotalNum())
				{
					//״̬���������趨�ĵ���������ֱ�ӷ���
					ProduceEvent(ET_AddState,dwID);						
					CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
					return;
				}						
			}
			++it;
			continue;			
		}
		else
		{
			//�ߵȼ�����͵ȼ�
			if (pOldState->GetID()==dwID)
			{	
				if (dwLv>pOldState->GetLv())				
				{
					//ɾ��ԭ�е͵ȼ���ͬID״̬
					RemoveState(it);				
					continue;
				}
				else if (dwLv<pOldState->GetLv())				
				{	
					//��״̬�ȼ�С��ԭ��״̬��ͬID����ֱ�ӷ���
					CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
					return;
				}
			}
			++it;
		}		
	}




	CBaseModule *pBaseModule = CNewSkillFactory::GetBaseModule(pModuParam);
	if (pBaseModule->Begin()==MR_Step_End)
	{
		//״̬�����ɹ�

		AddStateCount(pBaseModule->GetParam());
		pBaseModule->GetParam()->SetbRun(true);

		ProduceEvent(ET_AddState,dwID);
		OnAddBuff(dwID, dwLv);
		if (1 == pNSkillProperty->GetBreak())
		{
			//״̬�ж�
			ProduceEvent(ET_StateBreak);
		}

		itModu it = mapState.find(dwGlobleID);
		if (it==mapState.end())
		{
			mapState[dwGlobleID] = pModuParam;
		}
	}
	else
	{	
		//״̬����ʧ���ͷ�
		CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
	}
}

//���Buff
void CMoveShape::AddNewState(CMoveShape *pUser,DWORD dwStateID, DWORD dwLevel,long lType)
{
	CNewSkill *pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(dwStateID);

	if (IsDied())
	{
		return;
	}

	if (pSkillProperty && (pSkillProperty->GetType()==ModuType_Buff || pSkillProperty->GetType()==ModuType_DeBuff))
	{
		++m_ActiveID;
		stModuParam *pModuParam = InitModuParam(dwStateID,dwLevel,m_ActiveID);

		if (pModuParam)
		{
			pModuParam->Reset(this,pUser);				
			CBaseModule* pState = CNewSkillFactory::CreateModule(dwStateID,dwLevel,pModuParam,m_ActiveID,0,lType);

			if (pState)
			{
				if(pModuParam->GetModType()==ModuType_Buff)
				{
					//Buff
					OnBeenAddAppendSkill(pUser);
					StartUpState(pUser,pModuParam,m_BuffParam);
					if (this->GetState()==STATE_FIGHT)
					{
						pUser->GetAI()->ChangeState( StateAI::ST_FIGHT );
						//pUser->GetAI()->ChangeStateTo(STATE_FIGHT);
					}

				}
				else if(pModuParam->GetModType()==ModuType_DeBuff)
				{
					//DeBuff
					if( TYPE_PET == pUser->GetType()
						//&&
						//PET_TYPE_ELF == ((CPet*)pUser)->GetPetType()
						)
					{
						AI_EVENT_SENDER( GetAI() ).WhenBeenHurted( pUser->GetType(), pUser->GetExID(), 0 );
						//GetAI()->WhenBeenHurted(pUser->GetType(),pUser->GetExID(),0);
					}
					else
					{
						AI_EVENT_SENDER( GetAI() ).WhenBeenHurted( pUser->GetType(), pUser->GetExID(), 0 );
						//GetAI()->WhenBeenHurted(pUser->GetType(),pUser->GetExID(),0);
					}
					StartUpState(pUser,pModuParam,m_DeBuffParam);
				}
			}
			else
			{
				//��������ʵ��ʧ��
				CNewSkillFactory::RealeaseBaseMoudule(pModuParam);
			}
		}
	}
}



long CMoveShape::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
	if(var)
	{
		stModuParam *pModuParam = NULL;
		eModuleRet ret = MR_No;

		tagTimerVar* timervar = (tagTimerVar*)(var);
		switch(timervar->TimerType)
		{
		case ChildObjType_Buff:
			{
				itModu itBuff = m_BuffParam.find(timervar->lvar);
				if (itBuff!=m_BuffParam.end())
				{
					pModuParam = itBuff->second;				
				}			
			}
			break;
		case ChildObjType_DeBuff:
			{
				itModu itBuff = m_DeBuffParam.find(timervar->lvar);
				if (itBuff!=m_DeBuffParam.end())
				{
					pModuParam = itBuff->second;					
				}			
			}
			break;	

		case ChildObjType_ActiveSkill:
			{
				itModu it = m_ActiveSkillParam.find(timervar->lvar);
				if (it!=m_ActiveSkillParam.end())
				{
					pModuParam = it->second;
					if (pModuParam && pModuParam->GetProcNum()==0 && pModuParam->FindTimerParam(timerid)->nCalledCount==0)
					{
						ProduceEvent(ET_SuccessSkill);
					}
				}			
			}
			break;

		case ChildObjType_AI:
			{
                m_pAI->Run( timerid, timervar->lvar );
				//m_pAI->Run(timerid,(int)timervar->lvar);
			}
			break;
		case ChildObjType_RunScript:
			{
				OnScripttTimeOut(timerid);
			}
			break;
		case ChildObjType_Beneficiary:
			{
				OnLoseBeneficiary();
			}
			break;
		case ChildObjType_CollectionDied:
			{
				this->OnDiedEvent();
			}
			break;
		case ChildObjType_UpdateAttr:
			{
				OnUpdateAttrTimeOut();
			}
			break;
		}
		if (pModuParam)
		{
			CBaseModule *pBaseModule = CNewSkillFactory::GetBaseModule(pModuParam);
			if(pBaseModule)
			{
				ret = pBaseModule->OnTimeOut(timerid,curtime,var);
                if (ret == MR_Modu_End)
				{
					RemoveBaseModule(pModuParam);
				}
			}

  		}

	}
	return 0;
}

//! �õ���BUFF��ID
void CMoveShape::GetAllBuffID(list<pair<DWORD, DWORD>>& listBuffID)	
{
	listBuffID.clear();

	mapModules::iterator ite = m_BuffParam.begin();
	for (; ite != m_BuffParam.end(); ++ite)
	{
		listBuffID.push_back(pair<DWORD, DWORD>(ite->second->GetID(), ite->second->GetLv()));
	}

	//debuff
	ite = m_DeBuffParam.begin();
	for (; ite != m_DeBuffParam.end(); ++ite)
	{
		listBuffID.push_back(pair<DWORD, DWORD>(ite->second->GetID(), ite->second->GetLv()));
	}
}

void CMoveShape::OnAddBuff(DWORD dwStateID, DWORD dwBuffLevel)
{

}

void CMoveShape::OnDelBuff(DWORD dwStateID, DWORD dwBuffLevel)
{

}

void CMoveShape::EndActiveSkill(long lID,long lKeyID)
{
	if (lKeyID!=-1)
	{
		itModu it = m_ActiveSkillParam.find(lKeyID);
		if (it!=m_ActiveSkillParam.end())
		{
			CBaseModule *pActiveSkill = CNewSkillFactory::GetBaseModule(it->second);
			if (pActiveSkill)
			{
				pActiveSkill->End();				
				RemoveActive(pActiveSkill);	
				it=m_ActiveSkillParam.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	else
	{
		if (m_ActiveSkillParam.size()>0)
		{
			itModu it = m_ActiveSkillParam.begin();
			for(;it!=m_ActiveSkillParam.end();)
			{
				CBaseModule *pActiveSkill =  CNewSkillFactory::GetBaseModule(it->second);
				if (pActiveSkill && pActiveSkill->GetParam()->GetID()==lID)
				{
					pActiveSkill->End();			
					RemoveActive(pActiveSkill);
					it = m_ActiveSkillParam.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

	}

}



long CMoveShape::Proc(eEventType type,const tagProcVar *pVar)
{
	stModuParam *pModuParam=NULL;	

	eModuleRet ret = MR_No;

	switch(pVar->lType)
	{
	case ChildObjType_Buff:
		{
			itModu itBuff = m_BuffParam.find(pVar->lID);
			if (itBuff!=m_BuffParam.end())
			{
				pModuParam = itBuff->second;
			}
		}
		break;
	case ChildObjType_DeBuff:
		{
			itModu itBuff = m_DeBuffParam.find(pVar->lID);
			if (itBuff!=m_DeBuffParam.end())
			{
				pModuParam = itBuff->second;
			}
		}
		break;
	case ChildObjType_ActiveSkill:
		{

			itModu it = m_ActiveSkillParam.find(pVar->lID);
			if (it!=m_ActiveSkillParam.end())
			{
				pModuParam = it->second;		
			}			
		}
		break;
	case ChildObjType_PassiveSkill:
		{
			itModu it = m_PassiveSkillParam.find(pVar->lID);
			if (it!=m_PassiveSkillParam.end())
			{
				pModuParam = it->second;			
			}
		}
		break;
	};
	if (pModuParam)
	{
		CBaseModule *pBaseModule = CNewSkillFactory::GetBaseModule(pModuParam);
		if (pBaseModule)
		{
			ret = pBaseModule->Proc(type,pVar);
			if (ret==MR_Modu_End)
			{	
				if (pVar->lType==ChildObjType_ActiveSkill)
				{
					AI_EVENT_SENDER( GetAI() ).OnSkillEnd( pBaseModule->GetParam()->GetAddSub() );
					//this->GetAI()->OnSkillEnd(pBaseModule->GetParam()->GetAddSub());
				}
				RemoveBaseModule(pModuParam);

			}
		}

	}

	CShape::Proc(type,pVar);
	return true;
}

long CMoveShape::SkillReinforceBegin(bool isMagic, bool isAttack)
{
	return 0;
}
//!									���ܼӳɽ��������Ч�����棩
void CMoveShape::SkillReinforceEnd(long lKey)
{
}
//! ��ü��ܼӳ�ֵ��first������ʱ��仯������second��Ч��ֵ�仯������
const pair<float, float>&	CMoveShape::GetSkillReinforceAmend(long lKey)
{
	return m_FixedReinforce;
}


void	CMoveShape::SetCollectionID(DWORD l)
{
	m_dwCollectionID=l;
	AddAttributeToDirty("dwCollectionID");
	UpdateAttribute(true);
}

void	CMoveShape::SetCollectionTimes(DWORD l)
{
	m_dwCollectTimes=l;
	AddAttributeToDirty("dwCollectionTimes");
	UpdateAttribute(true);
}

void	CMoveShape::SetIsBeenCollect(bool l)
{
	m_bIsBeenCollect=l;
	AddAttributeToDirty("bIsBeenCollect");
	UpdateAttribute(true);
}

void	CMoveShape::SetCanBeenCollect(bool l)
{
	m_bCanBeenCollect=l;
	AddAttributeToDirty("bCanBeenCollect");
	UpdateAttribute(true);
}

BOOL	CMoveShape::BeenCollected()
{
	SetCollectionTimes(GetCollectionTimes()-1);
	if (GetCollectionTimes()<=0)
	{
		BeenCollectedAll();
	}
	return TRUE;
}
// ���ؽӿ�
//int CMoveShape::SetScriptVarValue(CScript* scriptPtr, const char* varName, const char* value)
//{
//	return -1;
//}
//int CMoveShape::SetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray, double value)
//{
//	return -1;
//}
//const char* CMoveShape::GetScriptVarValue(CScript* scriptPtr, const char* varName)
//{
//	return NULL;
//}
//double CMoveShape::GetScriptVarValue(CScript* scriptPtr, const char* varName, long lArray)
//{
//	return -1.0f;
//}
//
//int CMoveShape::SetScriptGuidValue(CScript* scriptPtr, const char* varName, const CGUID& guid)
//{
//	return -1;
//}
//const CGUID& CMoveShape::GetScriptGuidValue(CScript* scriptPtr, const char* varName)
//{
//	return NULL_GUID;
//}
//
//CVariableList* CMoveShape::GetVariableList(void)
//{
//	return NULL;
//}
bool CMoveShape::SetScriptVarValue( Script *script, const char *varName, const char *val )
{
	bool ret = m_VarList->SetVarValue( varName, tString( val ) );
	if( !ret )
	{
		VariableList *varList = script->GetVarList();
		ret = varList == NULL ? false : varList->SetVarValue( varName, tString( val ) );
	}
	if( !ret )
	{
		ret = GetInst( ScriptSys ).GetGeneralVarList()->SetVarValue( varName, tString( val ) );
	}
	return ret;
}

bool CMoveShape::SetScriptVarValue( Script *script, const char *varName, double val, long index )
{
	bool ret = m_VarList->SetVarValue( varName, val, index );
	if( !ret )
	{
		VariableList *varList = script->GetVarList();
		ret = varList == NULL ? false : varList->SetVarValue( varName, val, index );
	}
	if( !ret )
	{
		ret = GetInst( ScriptSys ).GetGeneralVarList()->SetVarValue( varName, val, index );
	}
	return ret;
}
bool CMoveShape::SetScriptVarValue( Script *script, const char *varName, const CGUID &val )
{
	bool ret = m_VarList->SetVarValue( varName, val );
	if( !ret )
	{
		VariableList *varList = script->GetVarList();
		ret = varList == NULL ? false : varList->SetVarValue( varName, val );
	}
	if( !ret )
	{
		ret = GetInst( ScriptSys ).GetGeneralVarList()->SetVarValue( varName, val );
	}
	return ret;
}

const char *CMoveShape::GetScriptVarValueStr( Script *script, const char *varName )
{
	{
		const tString &val = m_VarList->GetVarValue<tString>( varName );
		if( VariableList::IsValueValid( val ) )
		{
			return val.Str();
		}
	}
	{
		VariableList *varList = script->GetVarList();
		if( varList != NULL )
		{
			const tString &val = varList->GetVarValue<tString>( varName );
			if( VariableList::IsValueValid( val ) )
			{
				return val.Str();
			}
		}
	}
	{
		const tString &val = GetInst( ScriptSys ).GetGeneralVarList()->GetVarValue<tString>( varName );
		if( VariableList::IsValueValid( val ) )
		{
			return val;
		}
	}
	return NULL;
}

double CMoveShape::GetScriptVarValueNum( Script *script, const char *varName, long index )
{
	{
		double val = m_VarList->GetVarValue<double>( varName );
		if( VariableList::IsValueValid( val ) )
		{
			return val;
		}
	}
	{
		VariableList *varList = script->GetVarList();
		if( varList != NULL )
		{
			double val = varList->GetVarValue<double>( varName );
			if( VariableList::IsValueValid( val ) )
			{
				return val;
			}
		}
	}
	{
		double val = GetInst( ScriptSys ).GetGeneralVarList()->GetVarValue<double>( varName );
		return val;
	}
}

const CGUID &CMoveShape::GetScriptVarValueGuid( Script *script, const char *varName )
{
	{
		const CGUID &val = m_VarList->GetVarValue<CGUID>( varName );
		if( VariableList::IsValueValid( val ) )
		{
			return val;
		}
	}
	{
		VariableList *varList = script->GetVarList();
		if( varList != NULL )
		{
			const CGUID &val = varList->GetVarValue<CGUID>( varName );
			if( VariableList::IsValueValid( val ) )
			{
				return val;
			}
		}
	}
	{
		const CGUID &val = GetInst( ScriptSys ).GetGeneralVarList()->GetVarValue<CGUID>( varName );
		return val;
	}
}

VariableList *CMoveShape::GetVariableList()
{
	return m_VarList;
}

//////////////////////////////////////////////////////////////////////////
//	������
//////////////////////////////////////////////////////////////////////////
void CMoveShape::SetBeneficiary(long lType,CGUID guidFirst)
{
	if (m_BeneficiaryID== guidFirst)
	{
		return;
	}
	m_BeneficiaryID= guidFirst;
	m_BeneficiaryType= (OBJECT_TYPE)lType;

	CMessage mes(MSG_S2C_SHAPE_CHANGE_FIRSTATTACKER);
	mes.Add(GetExID());					//������������guid
	mes.Add(GetType());					//������������type
	mes.Add(m_BeneficiaryID);			//������guid	
	mes.Add((long)m_BeneficiaryType);	//������type
	mes.SendToAround(this);
}

void CMoveShape::CleanBeneficiary()			//���������
{
	if (m_lBeneficiaryTimerID!= -1)
	{
		GameManager::GetInstance()->Cancel(m_lBeneficiaryTimerID,0);
	}
	SetBeneficiary(0,NULL_GUID);
	m_lBeneficiaryTimerID=-1;

}
long CMoveShape::AddBeneficiaryEvent(long lTime)		//����������¼�
{
	if (m_lBeneficiaryTimerID!= -1) return 0;
	if (lTime<=0) return 0;

	tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_Beneficiary;
	pTimvar->lvar = 0;
	m_lBeneficiaryTimerID= GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime()+lTime,0);
	return 0;
}

void CMoveShape::AddDelEvent(DWORD dwTime)					// ע��ɾ���¼�
{
	if (m_lDelEventTimerID!=-1)	return ;

	tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_DeleteObj;
	//pTimvar->lvar= (long)this;
	pTimvar->pvar = new stDestObj;
	((stDestObj*)pTimvar->pvar)->DestID= GetExID();
	((stDestObj*)pTimvar->pvar)->nDestType= GetType();
	m_lDelEventTimerID=GameManager::GetInstance()->Schedule(GetFather(),(void*)pTimvar,timeGetTime()+dwTime);
	return ;
}

void CMoveShape::LogoutDelEvent()				// ע��ɾ���¼�
{
	if (m_lDelEventTimerID==-1)
	{
		return ;
	}
	GameManager::GetInstance()->Cancel(m_lDelEventTimerID,0);
	m_lDelEventTimerID=-1;

	return ;
}
void CMoveShape::CleanDelEvent()				// ���ɾ���¼�
{
	m_lDelEventTimerID=-1;
}


//ע��������ԵĶ�ʱ��
void CMoveShape::RegisterUpdateAttrTimer()
{
	if(m_lUpdateAttrTimerID != -1 )
		return;
	tagTimerVar* pTimvar =  OBJ_CREATE(tagTimerVar);
	pTimvar->TimerType = ChildObjType_UpdateAttr;
	pTimvar->pvar = NULL;
	m_lUpdateAttrTimerID = GameManager::GetInstance()->Schedule(this,(void*)pTimvar,timeGetTime()+GlobalSetup::GetSetup()->lUpdateOtherPropertyDelayTime);
}

//ע���������Զ�ʱ��
void CMoveShape::CancelUpdateAttrTimer()
{
	GameManager::GetInstance()->Cancel(m_lUpdateAttrTimerID);
	m_lUpdateAttrTimerID=-1;
}

//�������Զ�ʱ��
void CMoveShape::OnUpdateAttrTimeOut()
{
	OnUpdateAttributeTimeOut();
	m_lUpdateAttrTimerID=-1;
}
//���ö������
void CMoveShape:: SetTarget(CGUID guid,long lType)
{
	m_guidTarget = guid;
	m_lTargetType = lType;
}
//��ȡ�������
void CMoveShape::GetTarget(CGUID &guidTarget,long &lTargetType)
{
	guidTarget =  m_guidTarget;
	lTargetType = m_lTargetType;
}

//���ô�ɱ�߲���
void	CMoveShape::SetKillerInfo(CGUID guid,long lType)
{
	m_guidKiller = guid;
	m_lKillerType = lType;
}

//��ȡ��ɱ�߲���		
void	CMoveShape::GetKillerInfo(CGUID &guidKiller,long &lKillerType)
{
	guidKiller = m_guidKiller;
	lKillerType = m_lKillerType;
}
//���ù���Ŀ�����
void	CMoveShape::SetAttackTarget(CGUID guid,long lType)
{
	m_guidAttackTarget  = guid;
	m_lAttackTargetType = lType;
}
//��ȡ����Ŀ�����
void	CMoveShape::GetAttacktarget(CGUID &guid,long &lType)
{
	guid = m_guidAttackTarget;
	lType = m_lAttackTargetType;
}




void CMoveShape::AddSelectSelfPlayer(const CGUID& PlayerID)
{
	if(PlayerID != NULL_GUID)
		m_SelectSelfPlayers.insert(PlayerID);
}

void CMoveShape::RemoveSelectSelfPlayer(const CGUID& PlayerID)
{
	m_SelectSelfPlayers.erase(PlayerID);
}


//���÷�����Ϣ��ʱ��,�����ʱ�򷵻�true
bool CMoveShape::ResetSendMoveMsgTime()
{
	DWORD dwCurTime = timeGetTime();
	if( (dwCurTime - m_dwLastSendMoveSendTime) > 4000)
	{
		m_dwLastSendMoveSendTime= dwCurTime;
		return true;
	}
	return false;
}
