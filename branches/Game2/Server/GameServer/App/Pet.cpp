#include "StdAfx.h"
#include "Pet.h"
#include "StateAI/PetAI.h"
#include "Goods/CGoods.h"
#include "Monster.h"
#include "Player.h"
#include "LuaScript/Script.h"
#include "LuaScript/ScriptSys.h"
#include "../../../Public/Common/StudySkillCondition.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
long g_lTimer = 0;
#endif

#ifdef _DEBUG
#define PRINT_TO_CLIENT(__strLog) \
	GetGame()->SendNotifyMessage(__strLog,m_HostID,eNOTIFYPOS_CENTER);\
    Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,__strLog)

#else
#define PRINT_TO_CLIENT(__strLog) \
	Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,__strLog)
#endif

// ����long������ֵ
#define SET_LONG_PROP(PVAL, STRING, ENUM) \
	st.lType = 4; \
	st.pValue = (long*)(PVAL); \
	st.attrEnumValue = ENUM; \
	st.minValue = minValue[4]; \
	st.maxValue = maxValue[4]; \
	m_Attrs[STRING] = st

// ���ð������ֵ����Сֵ��long������ֵ
#define SET_LONG_PROP_EX(PVAL, STRING, ENUM, MIN, MAX) \
	st.lType = 4; \
	st.pValue = (long*)(PVAL); \
	st.attrEnumValue = ENUM; \
	st.minValue = (MIN); \
	st.maxValue = (MAX); \
	m_Attrs[STRING] = st

// ��������
#define SET_BASE_LONG_PROP(ATTR) \
	SET_LONG_PROP(&m_DiffAttrs.l##ATTR, #ATTR, PET_ATTR_##ATTR); \
	sprintf(strLog, #ATTR" --> m_DiffAttrs.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog);

// �̴ӵĻ�������
#define SET_SERVANTBASE_LONG_PROP(ATTR) \
	SET_LONG_PROP(&m_DiffAttrs.ServantAttr.l##ATTR,#ATTR,PET_ATTR_##ATTR); \
	sprintf(strLog, #ATTR" --> m_DiffAttrs.ServantAttr.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog);

// �̴ӵ�ǿ������
#define SET_SERVANT_EXTEND_PROP(TYPE, ATTR) \
	SET_LONG_PROP(&m_DiffAttrs.ServantAttr.##TYPE##Val.l##ATTR, #TYPE#ATTR, PET_ATTR_##TYPE##_##ATTR); \
	sprintf(strLog, #TYPE#ATTR" --> m_DiffAttrs.ServantAttr.l"#TYPE"Val.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog);

#define SET_BASE_LONG_PROP_EX(ATTR, MIN, MAX) \
	SET_LONG_PROP_EX(&m_DiffAttrs.l##ATTR, #ATTR, PET_ATTR_##ATTR, MIN, MAX)

// ��ʱ����
#define SET_TEMP_LONG_PROP(ATTR) \
	SET_LONG_PROP(&m_TempAttrs.l##ATTR, #ATTR, PET_ATTR_##ATTR); \
	sprintf(strLog, #ATTR" --> m_TempAttrs.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog);

// ����+��ʱ����
#define SET_BASE_TEMP_LONG_PROP(ATTR) \
	SET_LONG_PROP(&m_BaseAttrs.l##ATTR, "B_"#ATTR, PET_ATTR_##ATTR); \
	sprintf(strLog, "B_"#ATTR" --> m_BaseAttrs.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog); \
	SET_LONG_PROP(&m_TempAttrs.l##ATTR, "E_"#ATTR, PET_ATTR_##ATTR); \
	sprintf(strLog, #ATTR" --> m_TempAttrs.l"#ATTR);\
	Log4c::Trace(PET_MODULE, strLog);

// ���Ի��㹫ʽ
#define CALC_EXCH_SET_ATTR(ATTRS, ATTRD) \
	m_TempAttrs.l##ATTRD = (m_BaseAttrs.l##ATTRS + m_TempAttrs.l##ATTRS) * UNI_CONF.f##ATTRS##2##ATTRD + m_BaseAttrs.l##ATTRD
//SetAttr(#ATTRD,  (m_BaseAttrs.l##ATTRS + m_TempAttrs.l##ATTRS) * UNI_CONF.f##ATTRS##2##ATTRD + m_BaseAttrs.l##ATTRD);

// ���Ի��㹫ʽ
#define CALC_EXCH_CHG_ATTR(ATTRS, ATTRD) \
	m_TempAttrs.l##ATTRD += (m_BaseAttrs.l##ATTRS + m_TempAttrs.l##ATTRS) * UNI_CONF.f##ATTRS##2##ATTRD + m_BaseAttrs.l##ATTRD
//ChgAttr("E_"#ATTRD,  (m_BaseAttrs.l##ATTRS + m_TempAttrs.l##ATTRS) * UNI_CONF.f##ATTRS##2##ATTRD + m_BaseAttrs.l##ATTRD);

CPet::CPet(void)
: m_lAICount(0)
, m_dwMoveValiStartTime(timeGetTime())
, m_dwLastRecvMsgTime(m_dwMoveValiStartTime)
, m_fMaxPeriodMoveSpeed(0.0f)
, m_guMoveValiStartRgnID(NULL_GUID)
, m_lMoveValiStartPosX(0)
, m_lMoveValiStartPosY(0)
, m_fMoveValiStartSpeed(0.0f)
, m_fMoveValiTotalDistance(0.0f)
, m_fMoveValiRecvMsgCount(0.0f)
, m_pHost(NULL)
{
	//m_pVariableList = OBJ_CREATE(CVariableList);
	m_pAI = new StateAI::PetAI( this );
	//m_pAI->SetOwner(this);
	SetType(TYPE_PET);
    m_mover.Init( this );
	static long l = 0;
	Log4c::Trace(ROOT_MODULE, "%ld Constructor.", ++l);
}

CPet::~CPet(void)
{
	if( IsHangUp() )
	{
		DelFromRegion(PET_OT_DISAPPEAR);
	}

	//DelectAllScripByShape(GetExID());

	CancelAllTimer();
	ClearActiveSkills();
	ClearPassiveSkill();
	ClearAllBuff();
	delete m_pAI;
	//CAIFactory::DeleteAI(m_pAI);
	//OBJ_RELEASE(CVariableList, m_pVariableList);
	static long l = 0;
	Log4c::Trace(ROOT_MODULE, "%ld Destructor.", --l);
}

// ������Ϣ��Ӧ
void CPet::OnMessage(CMessage* pMsg)
{
}

// ��ʱ��Ӧ
long CPet::OnTimeOut(DWORD timerid,
					 DWORD curtime,
					 const void* var)
{
	const tagTimerVar* pTimerVar = (const tagTimerVar*)var;

#ifdef _RUNSTACKINFO_
	char szInfo[256]="";
	_snprintf(szInfo,256,"CPet::OnTimeOut(TimerType:%d,Name:%s) Start",
		pTimerVar?pTimerVar->TimerType:0,
		GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),szInfo);
#endif

	// 1) �̴ӵ�HP��MP���������ҳ϶ȡ�����

	if( IsDied() || !IsActive() )
	{
		CMoveShape::OnTimeOut(timerid, curtime, var);
		return 0;
	}

	switch( pTimerVar->TimerType )
	{
	case COT_PET_EVENT:
		{
			switch(pTimerVar->lvar)
			{
			case PET_HPRenew:
				{
					OnHPRenew();
				}
				break;

			case PET_MPRenew:
				{
					OnMPRenew();
				}
				break;

			case PET_EnergyRenew:
				{
					OnEnergyRenew();
				}
				break;

			case PET_LoyaltyRenew:
				{
					OnLoyaltyRenew();
				}
				break;

			case PET_LifeRenew:
				{
					OnLifeRenew();
				}
				break;
			case PET_SearchEnemy:
				{

					if( GetTarget() == NULL )
					{
						m_pAI->Resume( 0 );
						//m_pAI->AddAIEvent(CBaseAI::AET_MOVE);
					}
				}
				break;

			case PET_LifeTimeout:
				{
					OnLifeEnd();
				}
				break;
			}
		}
		break;

    case ChildObjType_ShapeMove:
        m_mover.OnTimeOut();
        break;
	default:
		{
			CMoveShape::OnTimeOut(timerid, curtime, var);
		}
		break;
	}

	return 0;
}

// ����ʵ�����Ϣ����
long CPet::Proc(eEventType type,
				const tagProcVar* pVar)
{
	return 0;
}

// ���һ���ƶ���Ϣ
void CPet::AddMoveMsg(float fNewX, float fNewY)
{
	float fXOffset = fNewX - GetPosX();
	float fYOffset = fNewY - GetPosY();
	float fDis = sqrt( fYOffset * fYOffset + fXOffset * fXOffset );

	m_fMoveValiTotalDistance += fDis;
	// ����ܹ��յ�һ��������Ϣ�Ժ󣬽�����֤��
	++m_fMoveValiRecvMsgCount;

	DWORD dwCurTime = timeGetTime();
	float fMoveSpeed = fDis/(float)(dwCurTime-m_dwLastRecvMsgTime);
	if( fMoveSpeed > m_fMaxPeriodMoveSpeed )
		m_fMaxPeriodMoveSpeed = fMoveSpeed;
	m_dwLastRecvMsgTime = dwCurTime;
}

// ��֤ÿһ�����ƶ�
bool CPet::CheckMoveStep(float fCurX, float fCurY)
{
	CServerRegion *pRegion = (CServerRegion*)GetFather();
	if( NULL == pRegion ||
		pRegion->GetType() != TYPE_REGION )
		return false;

	long lSX = GetTileX();
	long lSY = GetTileY();
	int nMaxDis = max((abs(lSX-(long)fCurX)), (abs(lSY-(long)fCurY)));
	if( nMaxDis > 2 )
		return false;

	// �ж��赲
	BYTE byBlock = pRegion->GetBlock(fCurX,fCurY);	
	if( byBlock != CRegion::BLOCK_NO &&
		byBlock != CRegion::BLOCK_AIM )
		return false;

	return true;
}

// �ƶ��ٶ���֤
bool CPet::CheckMoveSpeedValidate(void)
{
	// Ϊ����֤����Ч��, ʱ����ڵ���1�����֤
	DWORD dwTimeDif = timeGetTime() - m_dwMoveValiStartTime;
	float fTotalMoveDis = (float)dwTimeDif * m_fMoveValiStartSpeed;

	// ����Ӧ�ý�����Ϣ�ĸ���
	long nMsgCount = (long)(m_fMoveValiTotalDistance * 2.3f);
	float fAverSpeed = m_fMoveValiStartSpeed;
	if( dwTimeDif != 0 )
	{
		fAverSpeed = m_fMoveValiTotalDistance / dwTimeDif;
	}

	if( (fAverSpeed-m_fMoveValiStartSpeed) >= 0.0005f &&
		(m_fMaxPeriodMoveSpeed-m_fMoveValiStartSpeed) > 0.0008 )
	{
		/*char strInfo[256] = "";
		_snprintf(strInfo, 256,
		"PetMoveError,TimeDif:%d,TotalTimeDis:%f,RealMoveDis:%f",
		dwTimeDif, fTotalMoveDis, m_fMoveValiTotalDistance);
		PutStringToFile("MoveTest", strInfo);
		Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__, strInfo);*/

		// �����ͬһ������������Ϊ��֤�ĳ�ʼλ��
		//if( m_guMoveValiStartRgnID == GetRegionGUID() )
		//SetPosition(m_lMoveValiStartPosX, m_lMoveValiStartPosY);
		//else		// ���������ߴ���
		//{
		//	GetGame()->KickPlayer(m_HostID);
		//	return false;
		//}
	}

	ResetMoveValiData();
	return true;
}

// �����������
void CPet::ResetMoveValiData(bool bAll)
{
	m_dwMoveValiStartTime		= timeGetTime();
	m_dwLastRecvMsgTime			= m_dwMoveValiStartTime;
	m_fMaxPeriodMoveSpeed		= 0.0f;
	m_guMoveValiStartRgnID		= GetRegionGUID();
	m_lMoveValiStartPosX		= GetTileX();		// ��ʼ�ƶ���Xλ��
	m_lMoveValiStartPosY		= GetTileY();		// ��ʼ�ƶ���Yλ��
	m_fMoveValiStartSpeed		= GetSpeed();
	m_fMoveValiTotalDistance	= 0.0f;				// �ƶ����ܾ���
	m_fMoveValiRecvMsgCount		= 0;				// �յ����ƶ���Ϣ����
}

// �����ƶ�
void CPet::OnQuestMoveStep(float fCurX,
						   float fCurY,
						   float fDestX,
						   float fDestY)
{
	if ( !GetInstruction() )	// ����ʹ�����ʹ��ָ��ʱֱ����
		return;

	if( ModifyPos(IT_SAME_RGN) )
	{
		return;
	}

	StateAI::BaseAI* pAI = GetAI();
	if ( !CanMove() ||
		!CheckMoveStep(fCurX, fCurY) ||
		NULL == pAI )
	{
		OnCannotMove();
		return;
	}

	// ���һ����֤��Ϣ����
	AddMoveMsg(fCurX, fCurY);

	long lDir = GetLineDir(GetPosX(), GetPosY(), fCurX, fCurY);
	SetDir(lDir);
	SetPosXY(fCurX, fCurY);
	if( pAI->OnMove(fDestX, fDestY) )
	{
		if( m_fMoveValiRecvMsgCount > MoveValiMaxMsgCount )
		{
			CheckMoveSpeedValidate();
		}
	}
}

// ����ֹͣ�ƶ�
void CPet::OnQuestStopMove(float fCurX,
						   float fCurY,
						   short lDir)
{
	if( !CheckMoveStep(fCurX, fCurY) )
	{
		OnCannotMove();
		return;
	}

	// ���һ����֤��Ϣ����
	AddMoveMsg(fCurX, fCurY);
	SetDir(lDir);
	SetPosXY(fCurX, fCurY);	
	OnStopMove();
	ResetMoveValiData();
}

void CPet::OnCannotMove(void)
{
	CMessage msg( MSG_S2C_SHAPE_SETPOS );
	msg.Add( GetType() );
	msg.Add( GetExID());
	msg.Add( GetPosX() );
	msg.Add( GetPosY() );
	msg.Add( static_cast<LONG>(0) );
	msg.SendToPlayer(m_HostID);
}

// �����ܵ�����
long CPet::OnBeenAttacked(CMoveShape* pAttacker, long lHp)
{
	return CMoveShape::OnBeenAttacked(pAttacker, lHp);
}

/// ��������
void CPet::OnDied(void)
{
	Log4c::Trace(PET_MODULE, FormatText("GS_PET_4",m_pHost->GetName(),GetName(),GetExID().tostring()));//���%s�ĳ���(%s: %s) ����

	// ��������
	ChangeStateType(PET_STATE_AWAIT);
	OnChangeLoy(UNI_CONF.loyaltyMax*UNI_CONF.loyaDieRate/100);
	OnChangeLife(UNI_CONF.maxLife*UNI_CONF.lifeDieDownRate/100);
}

// ���ٶȷ����ı��Ժ󴥷�
void CPet::OnSpeedChanged(void)
{
	ResetMoveValiData();
}

void CPet::SetPosition(LONG lX, LONG lY)
{
	if( GetTileX() != lX || GetTileY() != lY )
	{
		CMoveShape::SetPosition(lX,lY);
		ResetMoveValiData(true);
	}
}

void CPet::SetSlipPos(LONG lX, LONG lY, float fSpeed, float fDir)
{
	CMoveShape::SetSlipPos(lX, lY, fSpeed, fDir);
	ResetMoveValiData();
}

// �ɷ񱻹���
bool CPet::IsAttackAble(CMoveShape* pAttacker)
{
	bool bRes = false;
	if( SAT_CAN_BEATK != CanBeAttacked() )
	{
		bRes = m_pHost->IsAttackAble(pAttacker);
	}

	return bRes;
}

// ����״̬
void CPet::UpdateCurrentState(void)
{
}

// �ı�״̬
void CPet::ChangeStateTo(CShape::eSTATE eState)
{
	if( !IsActive() || GetState() == eState ) return;

	switch( eState )
	{
	case STATE_FIGHT:
		{
			SetAttr("E_HpRecoverSpeed", -GetAttr("B_HpRecoverSpeed"));
			SetAttr("E_MpRecoverSpeed", -GetAttr("B_MpRecoverSpeed"));
			SetAttr("E_EnergyRecoverSpeed", -GetAttr("B_EnergyRecoverSpeed"));
		}
		break;

	case STATE_PEACE:
		{
			m_pAI->SetTarget( NULL_GUID, 0);
		}
		break;
	}

	CMoveShape::ChangeStateTo(eState);
	if( eState != m_pHost->GetState() &&
		(STATE_FIGHT == eState ||
		STATE_PEACE == eState) )
	{
		m_pHost->ChangeStateTo(eState);
		UpdateAttribute(TRUE);
	}
}

bool CPet::ChangeAttribute(std::string& attrName,
						   long value)
{
	long lVal = ChgAttr(attrName.c_str(), value);
	if( INVILID_VALUE == lVal )
		return false;
	return true;
}

bool CPet::SetAttribute(std::string& attrName,
						long value)
{
	long lVal = SetAttr(attrName.c_str(), value);
	if( INVILID_VALUE == lVal )
		return false;
	return true;
}

long CPet::GetAttribute(std::string attrName)
{
	return GetAttr(attrName.c_str());
}

// ��������
long CPet::UpdateAttributeToClient(bool bAll)
{
	WORD wAttrNum = 0;
	// �����˸�����ϸ����
	if( bAll && (!m_DirtyAttrs.empty() ) )
	{
		CMessage msg(MSG_S2C_SHAPE_CHANGE_PROPERTIES);
		msg.Add(GetType());
		msg.Add(GetExID());
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);
		wAttrNum = m_DirtyAttrs.size();
		setWriteDB.AddToByteArray(wAttrNum);
		if( wAttrNum > 0 )
		{
			CodeChgAttrs(setWriteDB, m_DirtyAttrs);
		}
		msg.SendToPlayer(m_pHost->GetExID(), false);
		m_DirtyAttrs.clear();
	}

	// ����Χ��Ҹ��»�������
	if( !bAll && !m_BaseDirtyAttrs.empty() )
	{
		CMessage msg(MSG_S2C_SHAPE_CHANGE_PROPERTIES);
		msg.Add(GetType());
		msg.Add(GetExID());
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);

		wAttrNum = m_BaseDirtyAttrs.size();
		setWriteDB.AddToByteArray(wAttrNum);
		if( wAttrNum > 0 )
		{
			CodeChgAttrs(setWriteDB, m_BaseDirtyAttrs);
		}
		msg.SendToPlayers(GetSelectSelfPlayer(), true);
		m_BaseDirtyAttrs.clear();
	}

	return true;
}

/// �������ͻ��˸�������
bool CPet::UpdateAttributeToOtherClient()
{
	if (!m_BaseDirtyAttrs.empty())
	{
		CMessage msg(MSG_S2C_SHAPE_CHANGE_PROPERTIES);
		msg.Add(GetType());
		msg.Add(GetExID());
		DBWriteSet setWriteDB;
		msg.GetDBWriteSet(setWriteDB);

		WORD wAttrNum = m_BaseDirtyAttrs.size();
		setWriteDB.AddToByteArray(wAttrNum);
		if( wAttrNum > 0 )
		{
			CodeChgAttrs(setWriteDB, m_BaseDirtyAttrs);
		}
		msg.SendToPlayers(GetSelectSelfPlayer(), true);
		m_BaseDirtyAttrs.clear();
		return true;
	}
	return false;
}

long  CPet::UpdateAttribute(BOOL bImmediately)
{
	if(!bImmediately)
	{
		RegisterUpdateAttrTimer();
		return true;
	}

	UpdateAttributeToClient(true);
	return true;
}

long CPet::OnUpdateAttributeTimeOut()
{
	UpdateAttributeToClient(false);
	return true;
}

void CPet::OnPropertyValueChanged(const char* strName, LONG lValue)
{
	//����������Է����ı�
	//Ҫ���¼������������������
	if(NULL == strName)	return;
	if(0 == strncmp(strName,"B_Strenth",9) || 0 == strncmp(strName,"Strenth",7) ||
		0 == strncmp(strName,"B_Dexterity",9) || 0 == strncmp(strName,"Dexterity",9) ||
		0 == strncmp(strName,"B_Con",8) || 0 == strncmp(strName,"Con",3) ||
		0 == strncmp(strName,"B_Intellect",8) || 0 == strncmp(strName,"Intellect",9) ||
		0 == strncmp(strName,"B_Spiritualism",8) || 0 == strncmp(strName,"Spiritualism",12))
	{
		InitialFightProperty();
	}
}

// ��ʼ����ʱ����
void CPet::InitialTempAttrs(void)
{
	m_TempAttrs = tagPetTemp();

	m_TempAttrs.lMaxExp = UNI_CONF.Exps[m_DiffAttrs.lLevel-1];
	m_TempAttrs.lMaxDExp = UNI_CONF.DExps[m_DiffAttrs.ServantAttr.lDLevel-1];
	m_TempAttrs.lRace = m_BaseAttrs.lRace;
	m_TempAttrs.lTakeLev = m_BaseAttrs.lTakeLevel;
	m_TempAttrs.lPetType = m_BaseAttrs.lPetType;
	m_TempAttrs.lNominateMode = m_BaseAttrs.lNominateMode;
	m_TempAttrs.lSkillNum = m_Skills.size();
	m_TempAttrs.lFightType = PET_FIGHETYPE_FIGHT;
	m_TempAttrs.lMoveType = PET_MOVETYPE_FOLLOW;
	m_TempAttrs.lLoyLev = CalcLoyLevel();

	InitialOrigAttrs(1, m_DiffAttrs.lLevel);
}

void CPet::InitialFightProperty(void)
{
	CALC_EXCH_SET_ATTR(Strenth, Hit);				// ����--����
	CALC_EXCH_SET_ATTR(Strenth, Dodge);				// ����--����
	CALC_EXCH_SET_ATTR(Strenth, MAtkSpeed);			// ����--�����ٶ�
	CALC_EXCH_SET_ATTR(Strenth, PhysicsAtk);		// ����--�����˺�
	CALC_EXCH_SET_ATTR(Strenth, PhysicsDef);		// ����--�������
	CALC_EXCH_SET_ATTR(Strenth, MAtk);				// ����--ħ���˺�
	CALC_EXCH_SET_ATTR(Strenth, MDef);				// ����--ħ������

	CALC_EXCH_CHG_ATTR(Dexterity, Hit);				// ��--����
	CALC_EXCH_CHG_ATTR(Dexterity, Dodge);			// ��--����
	CALC_EXCH_CHG_ATTR(Dexterity, MAtkSpeed);		// ��--�����ٶ�
	CALC_EXCH_CHG_ATTR(Dexterity, PhysicsAtk);		// ��--�����˺�
	CALC_EXCH_CHG_ATTR(Dexterity, PhysicsDef);		// ��--�������
	CALC_EXCH_CHG_ATTR(Dexterity, MAtk);			// ��--ħ���˺�
	CALC_EXCH_CHG_ATTR(Dexterity, MDef);			// ��--ħ������

	CALC_EXCH_CHG_ATTR(Con, Hit);					// ����--����
	CALC_EXCH_CHG_ATTR(Con, Dodge);					// ����--����
	CALC_EXCH_CHG_ATTR(Con, MAtkSpeed);				// ����--�����ٶ�
	CALC_EXCH_CHG_ATTR(Con, PhysicsAtk);			// ����--�����˺�
	CALC_EXCH_CHG_ATTR(Con, PhysicsDef);			// ����--�������
	CALC_EXCH_CHG_ATTR(Con, MAtk);					// ����--ħ���˺�
	CALC_EXCH_CHG_ATTR(Con, MDef);					// ����--ħ������

	CALC_EXCH_CHG_ATTR(Intellect, Hit);				// ��־--����
	CALC_EXCH_CHG_ATTR(Intellect, Dodge);			// ��־--����
	CALC_EXCH_CHG_ATTR(Intellect, MAtkSpeed);		// ��־--�����ٶ�
	CALC_EXCH_CHG_ATTR(Intellect, PhysicsAtk);		// ��־--�����˺�
	CALC_EXCH_CHG_ATTR(Intellect, PhysicsDef);		// ��־--�������
	CALC_EXCH_CHG_ATTR(Intellect, MAtk);			// ��־--ħ���˺�
	CALC_EXCH_CHG_ATTR(Intellect, MDef);			// ��־--ħ������

	CALC_EXCH_CHG_ATTR(Spiritualism, Hit);			// ����--����
	CALC_EXCH_CHG_ATTR(Spiritualism, Dodge);		// ����--����
	CALC_EXCH_CHG_ATTR(Spiritualism, MAtkSpeed);	// ����--�����ٶ�
	CALC_EXCH_CHG_ATTR(Spiritualism, PhysicsAtk);	// ����--�����˺�
	CALC_EXCH_CHG_ATTR(Spiritualism, PhysicsDef);	// ����--�������
	CALC_EXCH_CHG_ATTR(Spiritualism, MAtk);			// ����--ħ���˺�
	CALC_EXCH_CHG_ATTR(Spiritualism, MDef);			// ����--ħ������
}

// ��ʼ��ԭʼ���ԺͶ�Ӧ����
void CPet::InitialOrigAttrs(long lOldLevel, long lCurLevel)
{
	if(m_GrowCoefs.empty()) return;

	// ԭʼ���Լ��㹫ʽ
#define CALC_LEVEL_TEMP_ATTR(ATTR, LEV_START, LEV_END) \
	m_TempAttrs.l##ATTR += m_BaseAttrs.l##ATTR * (LEV_END - LEV_START) * pCoefs->f##ATTR

	long lID = 0;
	tagPetOrig* pCoefs = NULL;
	long lLevelStart = 0;
	long lLevelEnd = 0;

#define CALC_TEMP_ATTR(ATTR) \
	lID = 0; \
	pCoefs = &m_GrowCoefs[lID]; \
	lLevelStart = lOldLevel; \
	lLevelEnd = pCoefs->lLevel; \
	while( lCurLevel > lLevelEnd  && lID < m_GrowCoefs.size() ) \
	{ \
	if( lLevelStart < lLevelEnd ) \
	{ \
	CALC_LEVEL_TEMP_ATTR(ATTR, lLevelStart, lLevelEnd); \
	lLevelStart = lLevelEnd; \
	} \
	pCoefs = &m_GrowCoefs[++lID]; \
	lLevelEnd = pCoefs->lLevel; \
	} \
	if( lCurLevel > lLevelStart ) \
	{ \
	CALC_LEVEL_TEMP_ATTR(ATTR, lLevelStart, lCurLevel); \
	} \
	m_TempAttrs.l##ATTR += m_BaseAttrs.l##ATTR; \
	//SetAttr(#ATTR, m_TempAttrs.l##ATTR)

	CALC_TEMP_ATTR(MaxHp);							// Hp
	CALC_TEMP_ATTR(MaxMp);							// Mp
	CALC_TEMP_ATTR(MaxEnergy);						// ����

	CALC_TEMP_ATTR(Strenth);						// ����
	CALC_TEMP_ATTR(Dexterity);						// ��
	CALC_TEMP_ATTR(Con);							// ����
	CALC_TEMP_ATTR(Intellect);						// ��־
	CALC_TEMP_ATTR(Spiritualism);					// ����

	///  ȡ��Ʒ�ʶ�Ӧ������ֵ
	// Ʒ�ʶ��������� �� Ʒ�� * ��������
#define CALC_TALENT_VAL(ATTR) \
	m_BaseAttrs.lMax##ATTR * UNI_CONF.Quality[m_DiffAttrs.ServantAttr.lQuality] / 1000

	/// ȡ����������
	// �������� �� ��ǰ���� �� Ʒ�ʶ��������� �� �������� 
#define CALC_LAST_TALENT_VAL(ATTR) \
	m_DiffAttrs.ServantAttr.talentVal.l##ATTR \
	+ CALC_TALENT_VAL(ATTR) \
	+ m_DiffAttrs.ServantAttr.dAfterVal.l##ATTR

	/// ÿ��������������������
#define CALC_UP_LEV_ATTR(ATTR) \
	lCurLev  = GetAttr("Level"); \
	lastTalent = CALC_LAST_TALENT_VAL(ATTR); \
	baseAttr = m_BaseAttrs.l##ATTR; \
	m_TempAttrs.l##ATTR += lCurLev*baseAttr*lastTalent/1000

	// ����Ӱ��������Լ���
	int   lCurLev;
	double lastTalent;
	long baseAttr;
	CALC_UP_LEV_ATTR(Strenth);
	CALC_UP_LEV_ATTR(Dexterity);
	CALC_UP_LEV_ATTR(Con);
	CALC_UP_LEV_ATTR(Intellect);
	CALC_UP_LEV_ATTR(Spiritualism);

	// ս�����Լ���
	InitialFightProperty();

	if( 0 == GetAttr("Hp") || lOldLevel != lCurLevel )
	{
		SetAttr("Hp", GetMaxHP());
		SetAttr("Mp", GetMaxMP());
		SetAttr("Energy", GetMaxEnergy());
	}
	else
	{
		SetAttr("Hp", m_DiffAttrs.lHp);
		SetAttr("Mp", m_DiffAttrs.lMp);
		SetAttr("Energy", m_DiffAttrs.lEnergy);
	}

	if( STATE_FIGHT == GetState() )
	{
		SetAttr("E_HpRecoverSpeed", -GetAttr("B_HpRecoverSpeed"));
		SetAttr("E_MpRecoverSpeed", -GetAttr("B_MpRecoverSpeed"));
		SetAttr("E_EnergyRecoverSpeed", -GetAttr("B_EnergyRecoverSpeed"));
	}
}

// ��ʼ����������������
bool CPet::InitialBaseAttrs(const tagPetBase& rPet, long lInitType,long IsWild)
{
	m_BaseAttrs = rPet;
	SetGraphicsID(rPet.lPicID);

	// ȡ�ó���ɳ�ϵ������
	GetInst(CPetCtrl).GetGrowCoefs(m_BaseAttrs.lIndex, m_GrowCoefs);

	if( SAT_CAN_BEATK != CanBeAttacked() )
	{
		God();
	}

	if( NeedBeSaved() )
	{
		m_TempAttrs.cSaveFlag = 1;
	}

	if( rPet.lInvisible > 0 )
	{
		SetInvisibleState(false);
	}

	if( 0 == rPet.lMoveType )
	{
		SetMoveable(FALSE);
	}

	switch( lInitType )
	{
	case IT_CREATE:
		{
			InitialSkills();
			SetPetName(rPet.szOriName);
			//SetActiveAttr(1);
			SetHostName(m_pHost->GetName());

			m_DiffAttrs.lLevel	= 1;
			if (IsServant())
			{
				OnReSetTalent();
				m_DiffAttrs.lLife = m_BaseAttrs.lLife;
				m_DiffAttrs.lLoyalty = m_BaseAttrs.lLoyalty;
				m_DiffAttrs.ServantAttr.lWild = IsWild;
				// Ʒ��
				m_DiffAttrs.ServantAttr.lQuality = GetBaseQualityRadomVal();
			}		
		}
		break;

	case IT_RELIVE:
		{
			switch(GetPetType())
			{
			case PET_TYPE_SERVANT:
				{
					m_DiffAttrs.lLoyalty = m_BaseAttrs.lLoyalty;
				}
				break;
			}
		}
	}

	//SetActiveAttr(m_DiffAttrs.lStateType);
	//if( rPet.lExistTime != -1)
	//{
	//	return false;
	//}
	SetName(m_DiffAttrs.szName);

	return true;
}

// ��ʼ������Ĵ��̻��з��ı�������
bool CPet::InitialDiffAttrs(long lInitType, bool IsWild)
{
	SetWild(IsWild);

	C_PET_LIST rPets = GetInst(CPetCtrl).GetPetsConfig();
	C_ITR_PET_LIST itrPet = rPets.find(m_DiffAttrs.lIndex);
	bool bRes = false;
	if( itrPet != rPets.end() &&
		InitialBaseAttrs(itrPet->second, lInitType,IsWild) )
	{
		bRes = true;
	}

	return bRes;
}

// ��������
void CPet::SetHost(CPlayer* pPlayer)
{
	if( NULL == pPlayer )
	{
		m_HostID = NULL_GUID;
	}
	else
	{
		m_HostID = pPlayer->GetExID();
	}
	m_pHost = pPlayer;
}

// ��ӵ�������
bool CPet::AddToRegion(void)
{
	bool bRes = false;
	CServerRegion* pRgn = dynamic_cast<CServerRegion*>(m_pHost->GetFather());
	if( pRgn != NULL )
	{
		SetRegionGUID(pRgn->GetExID());
		SetFather(pRgn);
		SetPosition(m_pHost->GetTileX(), m_pHost->GetTileY());
		pRgn->AddObject(this);
		pRgn->AddShapeToAround(this);
	}

	return bRes;
}

// �ӳ����Єh��
bool CPet::DelFromRegion(long lOpType)
{
	//if( lOpType != PET_OT_CHANGE_REGION && !m_pAI->IsHibernated() )
	//{
	//	m_pAI->Hibernate();
	//	m_pAI->DeleteAIEvent(CBaseAI::AET_MOVE);
	//}
	if( lOpType != PET_OT_CHANGE_REGION && GetState() != CShape::STATE_HANGUP )
	{
		ChangeStateTo(CShape::STATE_HANGUP);
		m_pAI->Stop();
	}

	CServerRegion* pRgn = dynamic_cast<CServerRegion*>(GetFather());
	if (pRgn != NULL)
	{
		pRgn->DelShapeToAround(this);
		if (m_pHost != NULL &&
			pRgn == m_pHost->GetFather())
		{
			pRgn->DelShapeToPlayer(this, m_HostID);
		}
		pRgn->DeleteChildObject(this);
	}

	//SetState(STATE_DELET);
	return true;
}

// ͨ�����˳�ʼ������
bool CPet::InitialByHost(INIT_TYPE lInitType)
{
	CBaseObject* pRgn = NULL;
	switch(lInitType)
	{
	case IT_SAME_GS:		// ͬһGS�л�
		{
			DelFromRegion(PET_OT_CHANGE_REGION);
			AddToRegion();
		}
		break;

	case IT_LOGIN_GS:		// ��ͬGS�л�(��¼��Ϸ)
		{
			AddToRegion();
		}
		break;

	case IT_SAME_RGN:		// ͬһ�����л�
		{
			SetPosition(m_pHost->GetTileX(), m_pHost->GetTileY());
		}
		break;
	}

	m_TempAttrs.lCountry = m_pHost->GetCountry();

	if( m_BaseAttrs.lMoveType != 0 )
	{
		SetSpeed(m_pHost->GetSpeed());
	}

	return true;
}

// ��ʼ��AI
void CPet::InitAI(void)
{
	StateAI::PetAI *ai = (StateAI::PetAI*) m_pAI;
	ai->SetHost( m_pHost );
	ai->SetTarget( NULL_GUID, 0 );
	//CancelAllTimer();
	//((CPetAI*)m_pAI)->SetHost(m_pHost);
	//m_pAI->SetTarget(0, NULL_GUID);
}

// ��ʼ������
void CPet::InitSkills(void)
{
	ITR_SKILL_LIST itr = m_Skills.begin();
	for( ; itr != m_Skills.end(); ++itr )
	{
		OnBeginSkill(itr->second.lID, itr->second.lLevel);
	}
}

// �¼�ע��
inline void CPet::RegEvent(PET_EVENT_TYPE eType,
						   long lSpringTime,
						   long lIntervalTime)
{
	lSpringTime += timeGetTime();
	tagTimerVar* pTimer = OBJ_CREATE(tagTimerVar);
	pTimer->TimerType = COT_PET_EVENT;
	pTimer->lvar = eType;
	m_Timers[eType] = TIMER_SCHEDULE(this,
		(void*)pTimer,
		lSpringTime,
		lIntervalTime);
}

// ע�ὥ���¼�: ��ʱ��������������г�������, Ŀǰ��֪:
// 1) �̴ӵ�HP��MP���������ҳ϶ȡ�����
bool CPet::RegShadeEvent(void)
{
	RegEvent(PET_HPRenew, UNI_CONF.nHPMPInt, UNI_CONF.nHPMPInt);				// ����ˢ��HP
	RegEvent(PET_MPRenew, UNI_CONF.nHPMPInt, UNI_CONF.nHPMPInt);				// ����ˢ��MP
	RegEvent(PET_EnergyRenew, UNI_CONF.nHPMPInt, UNI_CONF.nHPMPInt);			// ����ˢ������
	RegEvent(PET_LoyaltyRenew, UNI_CONF.loyaDownUnit, UNI_CONF.loyaDownUnit);	// ����ˢ���ҳ϶�
	RegEvent( PET_LifeRenew , UNI_CONF.lifeSubUnit , UNI_CONF.lifeSubUnit);		// ����˥����ʱ��
	if( m_BaseAttrs.lExistTime != -1 )
	{
#ifdef _DEBUG
		g_lTimer = m_BaseAttrs.lExistTime % 1000;
#endif
		RegEvent(PET_LifeTimeout, m_BaseAttrs.lExistTime);							// ���������ս�
	}
	return true;
}

// ���ó��ﵱǰĿ��
void CPet::SetTarget(CMoveShape* pTarget)
{
	if( m_pAI != NULL )
	{
		if( pTarget != NULL )
		{
			m_pAI->SetTarget(pTarget->GetExID(), pTarget->GetType());
		}
		else
		{
			m_pAI->SetTarget( NULL_GUID, 0);
		}
	}
}

// ����ͻ��˼�������
void CPet::SaveSkillSetting(long lPos, long lActPos, bool bAuto)
{
	tagSkill* pSkill = GetSkill(lPos);
	if (pSkill != NULL)
	{
		pSkill->lActPos = lActPos;
		pSkill->bAuto = bAuto;
	}
}

// ���س��ﵱǰĿ��
const CMoveShape* CPet::GetTarget(void)
{
	if( m_pAI != NULL )
	{
		return m_pAI->GetTarget();
	}

	return NULL;
}

// �̴ӵ�HP
void CPet::OnHPRenew(void)
{
	long lCurHP = GetHP();
	long lMaxHP = GetMaxHP();
	long lHpRecoverSpeed = GetAttr("C_HpRecoverSpeed");
	if( lCurHP == lMaxHP && lHpRecoverSpeed > 0 ||
		0 == lHpRecoverSpeed )
		return;

	lCurHP += lHpRecoverSpeed;
	if( lCurHP > lMaxHP )
	{
		lCurHP = lMaxHP;
	}
	else if( lCurHP <= 0 )
	{
		lCurHP = 0;
		char szGUID[64];
		GetExID().tostring(szGUID);
		Log4c::Trace(ROOT_MODULE,"Player%s 's Pet(%s: %s) HP == 0.",m_pHost->GetName(),GetName(),szGUID);
		OnDied();	// ��������
	}

	SetAttr("Hp", lCurHP);
}

// �̴ӵ�MP
void CPet::OnMPRenew(void)
{
	long lCurMP = GetMP();
	long lMaxMP = GetMaxMP();
	long lMpRecoverSpeed = GetAttr("C_MpRecoverSpeed");
	if( lCurMP == lMaxMP && lMpRecoverSpeed >= 0 ||
		0 == lMpRecoverSpeed )
		return;

	lCurMP += lMpRecoverSpeed;
	if( lCurMP > lMaxMP )
	{
		lCurMP = lMaxMP;
	}
	else if( lCurMP < 0 )
	{
		lCurMP = 0;
	}

	SetAttr("Mp", lCurMP);
}

// �̴ӵ�����
void CPet::OnEnergyRenew(void)
{
	long lCurEnergy = GetEnergy();
	long lMaxEnergy = GetMaxEnergy();
	long lEnergyRecoverSpeed = GetAttr("C_EnergyRecoverSpeed");
	if (lCurEnergy == lMaxEnergy && lEnergyRecoverSpeed >= 0 
		|| 0 == lEnergyRecoverSpeed)
		return;

	lCurEnergy += lEnergyRecoverSpeed;
	if (lCurEnergy > lMaxEnergy)
	{
		lCurEnergy = lMaxEnergy;
	}
	else if (lCurEnergy < 0)
	{
		lCurEnergy = 0;
	}

	SetAttr("Energy", lCurEnergy);
}

/// �̴ӵ��ҳ϶�
void CPet::OnLoyaltyRenew(void)
{
	long lLoyaltyRenw = UNI_CONF.loyaDownVal;
	OnChangeLoy(lLoyaltyRenw);

	long loyaltyVal = GetAttr("Loyalty");
	if ( loyaltyVal <= UNI_CONF.loyaltyMin )
	{
        Log4c::Trace(PET_MODULE, FormatText("GS_PET_5",m_pHost->GetName(),GetName(),
            GetExID().tostring(), loyaltyVal, UNI_CONF.loyaltyMin));//Player%s 's Pet(%s: %s) �ҳ϶Ƚ���Ϊ%d������%d���ޣ��޷��ٻ� .
        char szLog[256];
		sprintf(szLog, "����%s�ҳ϶�̫�ͣ��޷��ٻ�", GetName());
		PRINT_TO_CLIENT(szLog);

		// ����״̬Ϊ����״̬
		ChangeStateType(PET_STATE_AWAIT);
	}
}

/// �̴ӵ�����
void CPet::OnLifeRenew(void)
{
	long lLifeSubRate = UNI_CONF.lifeSubRate;
	OnChangeLife(lLifeSubRate);

	long lifeVal = GetAttr("Life");
	if(lifeVal <= 0)
	{
		Log4c::Trace(PET_MODULE, FormatText("GS_PET_6",m_pHost->GetName(),GetPetName(),GetExID().tostring()));//Player%s 's Pet(%s: %s) ���� == 0.
        Log4c::Trace(PET_MODULE, FormatText("GS_PET_7",GetName()));//����%s����Ϊ�㣬�޷��ٻ�
		//PRINT_TO_CLIENT(szLog);

		// ����״̬Ϊ��˯״̬
		ChangeStateType(PET_STATE_SLEEP);
	}
}

// 5) ���������ս�
void CPet::OnLifeEnd(void)
{

}

// �������
void CPet::OnActive(void)
{
	SetStateType(PET_STATE_ACTIVE);
	InitAI();
	ChangeStateTo(STATE_PEACE);
	InitSkills();
	RegShadeEvent();
	InitialByHost(IT_LOGIN_GS);
}

// ��������
void CPet::OnCreate(void)
{
	InitNameValueMap();
	InitialTempAttrs();
}

// �ղس���
void CPet::OnHouse(long lOpType)
{
	// �������߻���ʧʱ��ȻΪ����״̬
	if( lOpType != PET_OT_PLAYER_EXIT )
	{
		SetActiveAttr(0);
		CancelAllTimer();
	}
	else
	{
		ChangeStateTo(STATE_HANGUP);
	}

	StopAllSkills();
	StopPassiveSkills();

	//ע�����Ը��¶�ʱ��
	CancelUpdateAttrTimer();

	if (lOpType != PET_OT_BEEN_KILLED)
	{
		DelFromRegion(lOpType);
	}
}

void CPet::SetActiveAttr(long lActiveType)
{
	if( 0 == lActiveType )
	{
		if( IsActive() )
		{
			m_pHost->DelFightPet(GetExID());
		}
		m_DiffAttrs.lStateType = lActiveType;
	}
	else if( lActiveType != 0 )
	{
		m_pHost->AddFightPet(this);
	}
}

void CPet::CancelAllTimer(void)
{
	map<long, long>::iterator itr = m_Timers.begin();
	for( ; itr != m_Timers.end(); ++itr )
	{
		TIMER_CANCEL(itr->second);
	}
	m_Timers.clear();
}

/// �����ҳ϶ȵȼ�
long CPet::CalcLoyLevel(void)
{
	long l = 0;
	long lMaxLv = UNI_CONF.loyaltyLvNum;
	const int* Loy = UNI_CONF.loyalties;
	while( Loy[l] < m_DiffAttrs.lLoyalty && l < lMaxLv ) ++l;

	return l;
}

// ��ʼ�������ֶ�
void CPet::InitNameValueMap(void)
{
	CMoveShape::InitNameValueMap();
	m_Attrs.clear();

	// �������Գ�ʼ��
	tagPty st;
	char strLog[256];

	static long minValue[7] = { 0x0, 0, 0, (long)-99999999.999f, -0x7FFFFFFF, -0x7FFF, 0};
	static long maxValue[7] = { 0xFF, 0xFFFF, 0x7FFFFFFF, (long)99999999.999f, 0x7FFFFFFF, 0x7FFF, 1};

	long* pVal = NULL;
	long lLevel = m_DiffAttrs.lLevel;

	SET_BASE_LONG_PROP(Hp);								// ��Ѫ
	SET_BASE_LONG_PROP(Mp);								// ����
	SET_BASE_LONG_PROP(Energy);							// ����

	SET_BASE_LONG_PROP(Loyalty);						// �ҳ϶�
	SET_BASE_LONG_PROP(Level);							// �ȼ�
	SET_BASE_LONG_PROP(Exp);							// ����
	SET_BASE_LONG_PROP(Life);							// ����
	SET_BASE_LONG_PROP(StateType);						// ״̬����

	SET_SERVANTBASE_LONG_PROP(Wild);					// Ұ����־
	SET_SERVANTBASE_LONG_PROP(Ages);					// ����
	SET_SERVANTBASE_LONG_PROP(DLevel);					// ���еȼ�
	SET_SERVANTBASE_LONG_PROP(DExp);					// ��Ϊ����
	SET_SERVANTBASE_LONG_PROP(Quality);					// Ʒ��
	SET_SERVANTBASE_LONG_PROP(Speciality);				// ����ƫ��
	SET_SERVANTBASE_LONG_PROP(DAfterValCount);			// ����ǿ������
	SET_SERVANTBASE_LONG_PROP(StarLev);					// �ǵ�
	SET_SERVANTBASE_LONG_PROP(StarLevCount);			// �ǵ�ǿ������

#define SET_SERVANT_EXTEND_PROPS(TYPE) \
	SET_SERVANT_EXTEND_PROP(TYPE,Strenth); \
	SET_SERVANT_EXTEND_PROP(TYPE,Dexterity); \
	SET_SERVANT_EXTEND_PROP(TYPE,Con); \
	SET_SERVANT_EXTEND_PROP(TYPE,Intellect); \
	SET_SERVANT_EXTEND_PROP(TYPE,Spiritualism)

	//�����̴�����
	SET_SERVANT_EXTEND_PROPS(talent);
	SET_SERVANT_EXTEND_PROPS(dAfter);
	SET_SERVANT_EXTEND_PROPS(syncretize);
	SET_SERVANT_EXTEND_PROPS(stoneUp);
	SET_SERVANT_EXTEND_PROPS(savvy);

	SET_TEMP_LONG_PROP(MaxExp);							// ��������
	SET_TEMP_LONG_PROP(MaxDExp);						// ��Ϊ��������
	SET_TEMP_LONG_PROP(Race);							// ����
	SET_TEMP_LONG_PROP(TakeLev);						// Я���ȼ�
	SET_TEMP_LONG_PROP(FightType);						// ս��ģʽ
	SET_TEMP_LONG_PROP(MoveType);						// �ƶ�ģʽ
	SET_TEMP_LONG_PROP(Country);						// ����
	SET_TEMP_LONG_PROP(LoyLev);							// �ҳ϶ȵȼ�

	SET_BASE_TEMP_LONG_PROP(MaxHp);						// ��Ѫ����
	SET_BASE_TEMP_LONG_PROP(MaxMp);						// ��������
	SET_BASE_TEMP_LONG_PROP(MaxEnergy);					// ��������

	SET_BASE_TEMP_LONG_PROP(HpRecoverSpeed);			// ����Hp�ظ��ٶ�
	SET_BASE_TEMP_LONG_PROP(MpRecoverSpeed);			// ����Mp�ظ��ٶ�
	SET_BASE_TEMP_LONG_PROP(EnergyRecoverSpeed);		// ���������ظ��ٶ�

	SET_BASE_TEMP_LONG_PROP(Strenth);					// ����
	SET_BASE_TEMP_LONG_PROP(Dexterity);					// ��
	SET_BASE_TEMP_LONG_PROP(Con);						// ����
	SET_BASE_TEMP_LONG_PROP(Intellect);					// ��־
	SET_BASE_TEMP_LONG_PROP(Spiritualism);				// ����

	SET_BASE_TEMP_LONG_PROP(Hit);						// ����
	SET_BASE_TEMP_LONG_PROP(Dodge);						// ����
	SET_BASE_TEMP_LONG_PROP(MAtkSpeed);					// �����ٶ�
	SET_BASE_TEMP_LONG_PROP(PhysicsAtk);				// �����˺�
	SET_BASE_TEMP_LONG_PROP(PhysicsDef);				// �������
	SET_BASE_TEMP_LONG_PROP(MAtk);						// ħ������
	SET_BASE_TEMP_LONG_PROP(MDef);						// ħ������

	SET_BASE_TEMP_LONG_PROP(ElemDef0);					// ˮ��
	SET_BASE_TEMP_LONG_PROP(ElemDef1);					// ����
	SET_BASE_TEMP_LONG_PROP(ElemDef2);					// ľ��
	SET_BASE_TEMP_LONG_PROP(ElemDef3);					// ��
	SET_BASE_TEMP_LONG_PROP(ElemDef4);					// ��
}

// ��ȡ��������, �������ֶβ�����ʱ, ����ֵΪ0.
long CPet::GetAttr(const char* szName)
{
	if( NULL == szName ||
		strlen(szName) <2 )
		return 0;

	long lVal = CMoveShape::GetAttribute(string(szName));
	if(lVal != INVILID_VALUE)
		return lVal;

	string strName = szName;
	if( '_' == szName[1] && 'C' == strName[0] ) // ȡ�õ�ǰֵ
	{
		strName[0] = 'E';
		long lEVal = GetAttr(strName.c_str());
		strName[0] = 'B';
		long lBVal = GetAttr(strName.c_str());
		lVal = lEVal + lBVal;

		return lVal;
	}

	lVal = 0;
	ITR_ATTR_MAP itr = m_Attrs.find(szName);
	if( m_Attrs.end() != itr )
	{
		const tagPty& rAttr = itr->second;
		switch( rAttr.lType )
		{
		case 0:	// byte
			return *(rAttr.plValue);

		case 1:	// word
			return *(rAttr.pwValue);

		case 2:	// dword
			return *(rAttr.pdwValue);

		case 3:	// float
			return *(rAttr.pfValue);

		case 4:	// long
			return *(rAttr.pValue);

		case 5:	// short
			return *(rAttr.psValue);

		case 6:	// bool
			return *(rAttr.pbValue);
		}
	}

	return lVal;
}

long CPet::SetAttr(const char* szName, long lVal)
{
	if( '_' == szName[1] && 'B' == szName[0] )
	{
		Log4c::Trace(PET_MODULE, FormatText("GS_PET_8",szName, lVal));//��ͼ�޸Ļ�������[%s]->%ld.
		return 0;
	}

	long lRet = CMoveShape::SetValue(szName, lVal);
	if( INVILID_VALUE != lRet )
	{
		AddDirtyAttr(szName);		// ���������ݱ��
		return lRet;
	}

	char szLog[512];
	ITR_ATTR_MAP itr = m_Attrs.find(szName);
	if( m_Attrs.end() != itr )
	{
		tagPty& rAttr = itr->second;
		if( strcmp("Hp", szName) == 0 )
		{
			long lMaxVal = GetMaxHP();
			if( lVal < 0 ) lVal = 0;
			if (lVal > lMaxVal ) lVal = lMaxVal;
		} 
		else if( strcmp("Mp", szName) == 0 )
		{
			long lMaxVal = GetMaxMP();
			if( lVal < 0 ) lVal = 0;
			if (lVal > lMaxVal ) lVal = lMaxVal;
		}
		else if (strcmp("Life", szName) == 0)
		{
			long lMaxVal = GetMaxLife();
			if (lVal < 0) lVal = 0;
			if (lVal > lMaxVal ) lVal = lMaxVal;
		}
		else if (strcmp("Loyalty", szName) == 0)
		{
			long lMaxVal = GetMaxLoyalty();
			if (lVal < 0) lVal = 0;
			if (lVal > lMaxVal ) lVal = lMaxVal;
		}
		if (strcmp("Energy", szName) == 0)
		{
			long lMaxVal = GetMaxEnergy();
			if( lVal < 0 ) lVal = 0;
			if (lVal > lMaxVal ) lVal = lMaxVal;
		}

		switch (rAttr.lType)
		{
		case 0:	// byte
			{
				*(rAttr.plValue) = (BYTE)lVal;
			}
			break;

		case 1:	// word
			{
				*(rAttr.pwValue) = (WORD)lVal;
			}
			break;
		case 2:	// dword
			{
				*(rAttr.pdwValue) = (DWORD)lVal;
			}
			break;

		case 3:	// float
			{
				*(rAttr.pfValue) = (float)lVal;
			}
			break;

		case 4:	// long
			{
				*(rAttr.pValue) = (long)lVal;
			}
			break;

		case 5:	// short
			{
				*(rAttr.psValue) = (short)lVal;
			}
			break;

		case 6:	// bool
			{
				*(rAttr.pbValue) = lVal==0?false:true;
			}
			break;
		default:
			{
				lVal = INVILID_VALUE;
			}
			break;
		}
		OnPropertyValueChanged(szName, lVal);
		AddDirtyAttr(szName);		// ���������ݱ��
	}
	else
	{
		_snprintf(szLog, 512, "��������ʧ��: [%-10s]-->%ld.",
			szName,
			lVal);
		PRINT_TO_CLIENT(szLog);
		lVal = INVILID_VALUE;
	}
	return lVal;
}

long CPet::ChgAttr(const char* szName, long lDelta)
{
	if( CMoveShape::ChangeAttribute(string(szName), lDelta) )
	{
		return CMoveShape::GetAttribute(szName);
	}

	long lVal = INVILID_VALUE;
	ITR_ATTR_MAP itr = m_Attrs.find(szName);
	if( m_Attrs.end() != itr )
	{
		lVal = GetAttr(szName);
		lVal += lDelta;
		lVal = SetAttr(szName, lVal);
	}

	return lVal;
}

// ���������ݱ��
void CPet::AddDirtyAttr(const char* szName)
{
	m_DirtyAttrs.insert(szName);

	if( '_' == szName[1] && ('C' == szName[0] || 'B' == szName[0]) )
	{
		char szLog[512];
		char szGUID[64];
		GetExID().tostring(szGUID);
		_snprintf(szLog, 512, "���%s��ͼ���³���%s������%s.",
			m_pHost->GetName(),
			szGUID,
			szName);
		PRINT_TO_CLIENT(szLog);
	}
	else
	{
		if( "Level" == szName ||
			"Hp" == szName ||
			"E_MaxHp" == szName )
		{
			m_BaseDirtyAttrs.insert(szName);
		}
		UpdateAttribute(TRUE);
	}
}

// ��������
void CPet::CodeChgAttr(DBWriteSet& setWriteDB,
					   short sEnum,
					   char cType,
					   long lValue)
{
	setWriteDB.AddToByteArray(sEnum);
	switch (cType)
	{
	case 0:		// byte
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((BYTE)lValue);
		break;
	case 1:		// word
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((WORD)lValue);
		break;
	case 2:		// dword
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((DWORD)lValue);
		break;
	case 3:		// float
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((float)lValue);
		break;
	case 4:		// long
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((long)lValue);
		break;
	case 5:		// short
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((short)lValue);
		break;
	case 6:		// bool
		setWriteDB.AddToByteArray(cType);
		setWriteDB.AddToByteArray((char)lValue);
		break;
	default:
		break;
	}
}

// ����������
void CPet::CodeChgAttrs(DBWriteSet& setWriteDB,
						set<string>& rAttrs)
{
	int i = 0;

	ITR_ATTR_MAP itrAttr;
	set<string>::iterator itr = rAttrs.begin();
	for( ; itr != rAttrs.end(); ++itr )
	{
		char* szAttr = const_cast<char*>(itr->c_str());
		if( (itrAttr = m_Attrs.find(szAttr)) != m_Attrs.end() ||
			(itrAttr = m_mapShapeNameValue.find(szAttr)) != m_mapShapeNameValue.end() )
		{
			if( '_' == szAttr[1] )
			{
				szAttr[0] = 'C';
			}
			long lValue = GetAttr(szAttr);
			CodeChgAttr(setWriteDB,
				itrAttr->second.attrEnumValue,
				itrAttr->second.lType,
				lValue);
			Log4c::Trace(PET_MODULE, FormatText("GS_PET_9", szAttr));//�������[%s].
		}
		else
		{
			CodeChgAttr(setWriteDB, 0, 0, 0);
			Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__, FormatText("GS_PET_10",szAttr));//%-15s �޷��������[%s].
		}
	}
}

// ���ó���ȼ�
void CPet::SetLevel(long lLevel)
{
	long lOldLevel = GetLevel();
	SetAttr("Level", lLevel);
	InitialOrigAttrs(lOldLevel, lLevel);
}

// ��������
void CPet::IncExp(long lDelta)
{
	/// ������̴ӣ������ҳ϶Ȼ�Ӱ������õľ���
	if ( IsServant() )
	{
		int loyLev = CalcLoyLevel();
		lDelta = lDelta * UNI_CONF.layaExp[loyLev] / 100 ;
	}
	long lExp = m_DiffAttrs.lExp +lDelta;
	if( 0 > lExp )
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_0", lExp));//�õ�һ�����ľ���[%d], ִ�з���!
		return;
	}

	// �������洢��ǰ���������3��
	long lMaxExp = GetAttr("MaxExp") * UNI_CONF.maxExpMultiple;
	if( lExp >= lMaxExp)
	{
		lExp = lMaxExp;
	}

	SetAttr("Exp", lExp);
	UpdateAttribute(TRUE);
	GetGame()->UpdateSavePlayer((CPlayer*)GetHost(), SAVE_DETAIL_PETS);
}

/// �����ȡ����
void CPet::AddExp(CMonster* pMonster)
{
	if (pMonster==NULL)
		return;

	ulong lDelta = pMonster->GetExpDead() * UNI_CONF.normalExp / 100;

	long levDiff = GetLevel() - pMonster->GetLevel();
	if (levDiff<0)
	{
		IncExp(lDelta);
	}
	else if (levDiff > UNI_CONF.expsMaxLevDiff)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_1", UNI_CONF.expsMaxLevDiff));//�̴ӵȼ����ڹ���ȼ�%d����, �޷���ȡ����!
		return;
	}
	else
	{
		lDelta = lDelta * UNI_CONF.levDiffExpRate[levDiff-UNI_CONF.expsBaseLevDiff] /100;
		IncExp(lDelta);
	}
}

/// �����ȡ��Ϊ����
void CPet::AddDExp(CMonster* pMonster)
{
	if (pMonster==NULL)
		return;

	ulong lDelta = pMonster->GetDExpDead() * UNI_CONF.normalDExp / 100;

	long levDiff = GetLevel() - pMonster->GetLevel();
	if (levDiff<0)
	{
		IncDExp(lDelta);
	}
	else if (levDiff > UNI_CONF.expsMaxLevDiff)
	{
		Log4c::Error(ROOT_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_2", UNI_CONF.expsMaxLevDiff));
		return;
	}
	else
	{
		lDelta = lDelta * UNI_CONF.levDiffDExpRate[levDiff-UNI_CONF.expsBaseLevDiff] /100;
		IncDExp(lDelta);
	}
}

/// ��������
bool CPet::OnUpgradeLev(void)
{
	long lLevel = GetLevel();
	long lExp = GetAttr("Exp");
	long lMaxExp = GetAttr("MaxExp");
	long lHostLev = m_pHost->GetLevel();

	if (lLevel>=lHostLev+UNI_CONF.moreHostLevel)
	{
		char str[100];
		sprintf_s(str,"���Ｖ��������5�����޷�������������!");
		PRINT_TO_CLIENT(str);
		return false;
	}

	if (lExp >= lMaxExp)
	{
		lExp -= lMaxExp;
		SetAttr("Exp", lExp);
		SetAttr("MaxExp", UNI_CONF.Exps[++lLevel]);
		SetLevel(lLevel);

		// �ҳ϶����̴���������
		OnChangeLoy(UNI_CONF.loyaUpGrageAdd);

		UpdateAttribute(TRUE);
		GetGame()->UpdateSavePlayer((CPlayer*)GetHost(), SAVE_DETAIL_PETS);
	}
	else
	{
		char str[100];
		sprintf_s(str,"���鲻�㣬�޷�����!");
		PRINT_TO_CLIENT(str);
		return false;
	}
	return true;
}

// ������Ϊ����
void CPet::IncDExp(long lDelta)
{
	/// ������̴ӣ������ҳ϶Ȼ�Ӱ������õľ���
	/*if ( IsServant() )
	{
	int   loyLev =  CalcLoyLevel();
	lDelta = lDelta * UNI_CONF.layaExp[loyLev] / 100 ;
	}*/
	long lDExp = m_DiffAttrs.ServantAttr.lDExp +lDelta;
	if( 0 > lDExp )
	{
		Log4c::Error(ROOT_MODULE,"%-18s %s",__FUNCTION__,FormatText("GS_PET_3", lDExp));//�õ�һ��������Ϊ����[%d], ִ�з���!
		return;
	}

	long lMaxDExp = GetAttr("MaxDExp") * UNI_CONF.maxDExpMultiple;
	if( lDExp >= lMaxDExp)
	{
		lDExp = lMaxDExp;
	}

	SetAttr("DExp", lDExp);
	UpdateAttribute(TRUE);
	GetGame()->UpdateSavePlayer((CPlayer*)GetHost(), SAVE_DETAIL_PETS);
}
/// �����������еȼ�
bool CPet::OnUpgradeDLev(void)
{
	long lLevel = GetLevel();
	long lDLevel = GetDLevel();
	long lDExp = GetAttr("DExp");
	long lMaxDExp = GetAttr("MaxDExp");

	if (lDLevel>=lLevel)
	{
		char str[100];
		sprintf_s(str,"���в��ܳ����̴ӵȼ�!");
		PRINT_TO_CLIENT(str);
		return false;
	}

	if (lDExp >= lMaxDExp)
	{
		lDExp -= lMaxDExp;
		SetAttr("DExp", lDExp);
		SetAttr("MaxDExp", UNI_CONF.DExps[++lDLevel]);
		//SetLevel(lLevel);
		UpdateAttribute(TRUE);
		GetGame()->UpdateSavePlayer((CPlayer*)GetHost(), SAVE_DETAIL_PETS);		
	}
	else
	{
		char str[100];
		sprintf_s(str,"��Ϊ���㣬�޷���������!");
		PRINT_TO_CLIENT(str);
		return false;
	}
	return true;
}

// ��������
bool CPet::ModifyPos(INIT_TYPE lInitType)
{
	long lNo = GetPetType() - 1;
	if( lNo < 0 ) lNo = S_MAX_CLASSES - 1;
	bool bRes = false;
	if( IT_SAME_GS == lInitType || Distance(m_pHost) >= UNI_CONF.MaxDistances[lNo] )
	{
		InitialByHost(lInitType);
		SetMoveType(PET_MOVETYPE_FOLLOW);
		bRes = true;
	}

	return bRes;
}

// ��������ģʽ
void CPet::SetMoveType(long lType)
{
	if(CanMove())
	{
		SetAttr("MoveType", lType);
		SetTarget(NULL);

		if( PET_FIGHETYPE_FIGHT == GetFightType() )
		{
			m_pAI->Resume( CONTINUE_TIME ); //AddAIEvent(CBaseAI::AET_MOVE);
		}
	}
}

// ���ù���ģʽ
void CPet::SetFightType(long lType)
{
	if(SAT_CAN_FIGHT == CanFight())
	{
		SetAttr("FightType", lType);
		if( lType != PET_FIGHETYPE_FIGHT )
		{
			SetTarget(NULL);
		}
		else if( NULL == GetTarget() )
		{
			m_pAI->Resume( CONTINUE_TIME ); //AddAIEvent(CBaseAI::AET_MOVE);
		}
	}
}

/// ����״̬ģʽ
void CPet::SetStateType(long lState)
{
	m_DiffAttrs.lStateType = lState;
}

/// ת��״̬ģʽ
void CPet::ChangeStateType(long lNewState)
{
	long lOldState = GetStateType();
	switch(lNewState)
	{
		// ��˯״̬, ��������Ϊ�㣬�Զ�ת��Ϊ��״̬
	case PET_STATE_SLEEP:
		if (lOldState != PET_STATE_ACTIVE)
		{
			//m_pHost->SendNotifyMessage("״̬���󣬲������ɷǼ���״̬ת��Ϊ��˯״̬��",NOTIFY_COLOR_RED);
			m_pHost->SendNotifyMessage("[Colour 'ffff0000']״̬���󣬲������ɷǼ���״̬ת��Ϊ��˯״̬��");
			return;
		}
		if (!GetInst(CPetCtrl).UnActivePet(m_pHost, GetExID()))
			return;
		m_pHost->SetActivePetGUID(NULL_GUID);
		break;

		// ����״̬ �������Ϊ��״̬
	case PET_STATE_AWAIT:
		if (lOldState == PET_STATE_ACTIVE)
		{
			if (!GetInst(CPetCtrl).UnActivePet(m_pHost, GetExID()))
				return;
			m_pHost->SetActivePetGUID(NULL_GUID);
		}
		else if (lOldState==PET_STATE_FIGHT)
		{
			m_pHost->DelFightPet(GetExID());
		}
		else
		{
			m_pHost->SendNotifyMessage("[Colour 'ffff0000']״̬����");
			return;
		}
		break;

		// ��ս״̬
	case PET_STATE_FIGHT:
		if (lOldState==PET_STATE_AWAIT)
		{
			if (GetLoyalty() <= UNI_CONF.loyaltyMin)
			{
				m_pHost->SendNotifyMessage("[Colour 'ffff0000']�ҳ϶�̫�ͣ��޷���ս��");
				return;
			}

			m_pHost->AddFightPet(GetExID());
		}
		else if (lOldState==PET_STATE_ACTIVE)
		{
			if (!GetInst(CPetCtrl).UnActivePet(m_pHost, GetExID()))
				return;
			m_pHost->SetActivePetGUID(NULL_GUID);
			m_pHost->AddFightPet(GetExID());
		}
		else
		{
			m_pHost->SendNotifyMessage("[Colour 'ffff0000']״̬���󣬲������ɷǼ���״̬��Ǵ���״̬ת��Ϊ��˯״̬��");
			return;
		}
		break;

		// ����״̬
	case PET_STATE_ACTIVE:
		if (lOldState==PET_STATE_FIGHT)
		{
			if (!GetInst(CPetCtrl).ActivePet(m_pHost, GetExID()))
				return;
			m_pHost->SetActivePetGUID(GetExID());
			m_pHost->DelFightPet(GetExID());
		}
		else
		{
			m_pHost->SendNotifyMessage("[Colour 'ffff0000']״̬����ֻ���ɳ�ս״̬ת��Ϊ����״̬��");
			return;
		}
		break;

	default:
		m_pHost->SendNotifyMessage("[Colour 'ffff0000']�����޴�״̬�����飡");
		break;
	}

	SetStateType(lNewState);

	CMessage msg(MSG_S2C_PET_CHANGE_STATE);
	msg.Add(GetExID());
	msg.Add((long)lNewState);
	msg.SendToPlayer(m_pHost->GetExID());
}

/// ��ȡ״̬ģʽ
long CPet::GetStateType(void)
{
	return m_DiffAttrs.lStateType;
}

// ��ȡ����ģʽ
long CPet::GetMoveType(void)
{
	return m_TempAttrs.lMoveType;
}

// ��ȡ����ģʽ
long CPet::GetFightType(void)
{
	return m_TempAttrs.lFightType;
}

/// �Ƿ�ӵ�м���
bool CPet::HasSkill(long lID)
{
	ITR_SKILL_LIST itr = m_Skills.begin();
	for( ; itr != m_Skills.end(); ++itr )
	{
		if( lID == itr->second.lID )
			return true;
	}
	return false;
}

/// ����ʱ��ʼ������
long CPet::InitialSkills(void)
{
	// �����ͨ�������ܣ�Ĭ�ϸü���Ϊ�̴��Զ��ͷţ�
	long lID = m_BaseAttrs.OrdinarilySkill.lID;
	long lLev = m_BaseAttrs.OrdinarilySkill.lLevel;
	if( lID != 0 )
	{
		tagSkill tSkill;
		tSkill.lPos = m_TempAttrs.lSkillNum++;
		tSkill.lType = PET_SKILL_ORDINARILY;
		tSkill.lID = lID;
		tSkill.lLevel = lLev;
		tSkill.bAuto = true;
		m_Skills[tSkill.lPos] = tSkill;
		ModifySkill(tSkill);
	}

	// ����츳���ܣ�Ĭ���̴ӵ��츳����ֻ��һ�֣�
	lID = m_BaseAttrs.GeniusSkill.lID;
	lLev = m_BaseAttrs.GeniusSkill.lLevel;
	if( lID != 0 )
	{		
		tagSkill tSkill;
		tSkill.lPos = m_TempAttrs.lSkillNum++;
		tSkill.lType = PET_SKILL_GENIUS;
		tSkill.lID = lID;
		tSkill.lLevel = lLev;
		m_Skills[tSkill.lPos] = tSkill;
		ModifySkill(tSkill);
	}

	return 0 ;
}

/// ��ȡ����
tagSkill* CPet::GetSkill(long lPos)
{
	ITR_SKILL_LIST itr = m_Skills.find(lPos);
	if( itr != m_Skills.end() )
		return &itr->second;
	return NULL;
}

/// ���ó��＼�ܿ�ݼ�
void CPet::SetSkillHotKey(long lPos, long lActPos)
{
	tagSkill* pSkill = GetSkill(lPos);
	if( pSkill != NULL )
	{
		pSkill->lActPos = lActPos;
	}
}

/// ��鼼����������
CNewSkill* CPet::CanModifySkill(tagSkill& rSkill)
{
	// ����������
	if( IsDied() )
	{
		return NULL;
	}

	// �����б��޴˼���
	CNewSkill* pSkill = CNewSkillFactory::QueryNewSkillProperty(rSkill.lID);
	if( NULL == pSkill )
	{
		char szGUID[128];
		GetExID().tostring(szGUID);
        Log4c::Error(PET_MODULE,"%-15s ����%s���޸ļ���%ld(Lv.%ld)ʱ����: ���ܲ�����",__FUNCTION__,szGUID, rSkill.lID, rSkill.lLevel);
		return NULL;
	}

	// ��������
	if( pSkill->GetSkillType() != rSkill.lType )
	{
		//OccuJudgeStudy();
	}

	// ��������
	if( pSkill->GetSkillType() != m_BaseAttrs.lPetType )
	{
	}

	return pSkill;
}

/// �������ܣ��̴Ӽ�����Ƕʱ����ԭ�м��ܣ�
bool CPet::ModifySkill(tagSkill& rSkill)
{
	CNewSkill* pSkill = CanModifySkill(rSkill);
	CNewSkillProperties::tagSkill *pTSkill = NULL;
	if( NULL == pSkill || NULL == (pTSkill = pSkill->GetNewSkill(rSkill.lLevel)) )
	{
		char szGUID[128];
		GetExID().tostring(szGUID);
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_11",szGUID, rSkill.lID, rSkill.lLevel));//����%s���޸ļ���%ld(Lv.%ld)ʱ����: �ȼ�������
		return false;
	}

	CMessage msg(MSG_S2C_SKILL_ADD_PET);
	msg.Add(GetExID());
	msg.AddEx(&rSkill, sizeof(rSkill));
    msg.Add(GetSkillValue( rSkill.lID , rSkill.lLevel, "CoolDown" ));
    msg.Add((float)GetSkillValue( rSkill.lID , rSkill.lLevel , "MaxAtkDistance" ));
    msg.Add((float)GetSkillValue( rSkill.lID , rSkill.lLevel , "MinAtkDistance" ));
    msg.Add(GetSkillValue( rSkill.lID , rSkill.lLevel , "SMp" ));
	msg.SendToPlayer(m_HostID);

	GetGame()->UpdateSavePlayer((CPlayer*)GetHost(), SAVE_DETAIL_PETS);
	if( ModuType_Skill_Passive == pSkill->GetSkillType() )
	{
		OnBeginSkill(rSkill.lID, rSkill.lLevel);
	}

	StudySkillScript(rSkill.lID, rSkill.lLevel);
	return true;
}

// ��������
bool CPet::ModifySkill(long lID, long lLevel, long lPos)
{
	// ����δ����
	tagSkill* pTSkill = GetSkill(lPos);
	if( NULL == pTSkill )
	{
		pTSkill = AddSkill(tagSkill());
	}

	pTSkill->lID = lID;
	pTSkill->lLevel = lLevel;
	return ModifySkill(*pTSkill);
}

/// ��Ӽ���
tagSkill* CPet::AddSkill(tagSkill& rSkill)
{
	if( 0 == rSkill.lPos ) rSkill.lPos = (long)m_Skills.size();
	m_Skills[rSkill.lPos] = rSkill;
	return &m_Skills[rSkill.lPos];
}

/// ͨ��������Ʒ��Ӽ��� @������Ƕ ��������Ʒ����˵��
bool CPet::AddSkill(CGoods* pGoods, long lPos, long lHoleType)
{
	tagSkill tSkill;
	tSkill.lID = pGoods->GetAddonPropertyValues(GAP_ADD_SKILL, 1);
	tSkill.lLevel = pGoods->GetAddonPropertyValues(GAP_ADD_SKILL, 2);
	tSkill.lType = pGoods->GetAddonPropertyValues(GAP_COLOR, 1);
	tSkill.lPos = lPos;

	if (lHoleType!=tSkill.lType)
	{
		// ��ʾ ���ܲ۲���Ӧ
		return false;
	}

	if(CanAddSkill(pGoods))
	{
		// ��Ƕ�ɹ���
		int addRate = pGoods->GetAddonPropertyValues(GAP_COLOR, 1);
		if (random(100)>addRate)
		{
			// ��ʾ ��Ƕʧ�� #��Ʒ��
			// ɾ����Ʒ��ɾ����ǰ��λ����
			m_pHost->DelGoods(pGoods->GetExID(), 1);
			m_Skills.erase(lPos);
			return false;
		}
        return AddSkill(tSkill)!=NULL;        
	}
	return false;
}

/// ������Ƕ�����ж�
bool CPet::CanAddSkill(CGoods* pGoods)
{
	//@ ��Ʒ������λֵδȷ������������@GAP_ADD_SKILL �ȴ���
	int addLev = pGoods->GetAddonPropertyValues(GAP_ADD_SKILL, 1);
	int needMoney = pGoods->GetAddonPropertyValues(GAP_ADD_SKILL, 1);
	long preSkillID = pGoods->GetAddonPropertyValues(GAP_ADD_SKILL, 1);
	if (GetLevel()<addLev)
	{
		// ��ʾ �ȼ�����
		return false;
	}
	if (m_pHost->GetMoney()<needMoney)
	{
		// ��ʾ ��ҽ�Ǯ����
		return false;
	}
	if (preSkillID!=0)
	{
		if (!HasSkill(preSkillID))
		{
			// ��ʾ δ��ǰ�ü���
			return false;
		}
	}
	return true;
}

/// ��鼼��ʹ������
long CPet::CanUseSkill(tagSkill* pSkill, CMoveShape* pTarget, long& lSkillType)
{
	long lCurTime = timeGetTime();

	if (this->IsDied() || pTarget->IsDied())
	{
		return SKILL_USE_FAILED_INVALID_STATE;
	}

	// �����ͷż��
	if( m_TempAttrs.lLastUseSkillTime - m_TempAttrs.lMAtkSpeed > lCurTime )
	{
		return SKILL_USE_FAILED_INVALID_TIME;
	}

	// ��ȴʱ��
	if( pSkill->lEndCooldown > lCurTime )
	{	
		return SKILL_USE_FAILED_INVALID_TIME;
	}

	//if (dwCurrTime<m_dwIntonateStartTime + m_dwIntonateTime)
	//{
	//	//����ʱ��
	//	return SKILL_USE_FAILED_INVALID_INTONATE;		
	//}


    CNewSkill* pSkillProperty = CNewSkillFactory::QueryNewSkillProperty(pSkill->lID);
    if ( NULL == pSkillProperty )
    {
        return SKILL_USE_FAILED_INVALID_UNKNOWN;
    }

	long lActTime = GetSkillValue( pSkill->lID , pSkill->lLevel, "ActTime" );
	if ( lActTime > 0 &&
		IsInAct() )
	{
		return SKILL_USE_FAILED_INVALID_ACT;
	}

	m_TempAttrs.lLastUseSkillTime = lCurTime;
    pSkill->lEndCooldown = lCurTime + GetSkillValue( pSkill->lID , pSkill->lLevel, "CoolDown" );

	lSkillType = pSkillProperty->GetSkillType();

	return -1;
}

// ʹ�ü���
bool CPet::UseSkill(tagSkill* pSkill, CMoveShape* pTarget)
{
	long lSkillType = -1;
	long lRet = CanUseSkill(pSkill, pTarget, lSkillType);
	if ( lRet != -1 )
	{
		CMessage msg(MSG_S2C_SKILL_USE);
		msg.Add((CHAR)SKILL_USE_RESULT_FAILED);
		msg.Add( GetExID() );
		msg.Add(pSkill->lID);
		msg.Add((BYTE)pSkill->lLevel);
		msg.Add((DWORD)0);
		msg.Add((BYTE)lRet);
		msg.SendToPlayer(m_HostID);
		return false;
	}

	switch( lSkillType )
	{
	case ModuType_Buff:
	case ModuType_DeBuff:
		{
			AddNewState(this, pSkill->lID, pSkill->lLevel,13);
		}
		break;

	case ModuType_Skill_Active:
		{
			AddActiveSkill(pSkill->lID, pSkill->lLevel, pTarget);
		}
		break;

	case ModuType_Skill_Passive:
		{
			AddPassSkill(pSkill->lID, pSkill->lLevel);
		}
		break;
	default:
		{
			AddActiveSkill(pSkill->lID, pSkill->lLevel, pTarget);
		}
		break;
	}

	if( pTarget != NULL )
	{
		StateAI::PetAI *ai = (StateAI::PetAI*) m_pAI;
		ai->Attack(pSkill->lID, pTarget->GetTileX(), pTarget->GetTileY(), pTarget);
	}

	return true;
}

// ��������ѧϰ�ű�
void CPet::StudySkillScript(long lID, long lLevel)
{
	CStudySkillCondition::tagCondition* ptgCon =
		CStudySkillCondition::GetCondition(lID, lLevel);
	if ( ptgCon != NULL )
	{
		vector<CStudySkillCondition::tagConditionEx*>::iterator itr
			= ptgCon->vec_ConditionEx.begin();
		for( ; itr != ptgCon->vec_ConditionEx.end(); ++itr )
		{
			if( CStudySkillCondition::CONDITIONEX_SCRIPT == (*itr)->wConditionExType )
			{
				PetRunScript((*itr)->strValue.c_str());
			}
		}
	}
}

//�����������һ���ű�
void CPet::PetRunScript(const char* szScriptName)
{
	if( NULL == szScriptName ) return;

	Script::RunEnv env( this, NULL, static_cast<CRegion*>( GetFather() ) );
	GetInst( ScriptSys ).RunScript( env, szScriptName );
}

// GS==>Client����
bool CPet::CodeToDataBlock_ForClient(DBWriteSet& setWriteDB,
									 bool bExData)
{
#ifdef _RUNSTACKINFO3_
	char pszStatckInfo[1024]="";
	_snprintf(pszStatckInfo,1024,"CPet::CodeToDataBlock_ForClient(name:%s) Start",GetName());
	CMessage::AsyWriteFile(GetGame()->GetStatckFileName(),pszStatckInfo);
#endif

	CMoveShape::CodeToDataBlock_ForClient(setWriteDB, bExData);
	setWriteDB.AddToByteArray(m_HostID);

	if(bExData)
	{
		long lNum = (long)m_Skills.size();
		setWriteDB.AddToByteArray(lNum);
		ITR_SKILL_LIST itr = m_Skills.begin();
		bool bFlag = false;
		CNewSkill* pSkillProperty = NULL;
		CNewSkillProperties::tagSkill* pSkill = NULL;
		long lCurTime = timeGetTime();
		long lRestTime = 0;
		for( ; itr != m_Skills.end(); ++itr )
		{
			bFlag = false;
			setWriteDB.AddToByteArray((long)itr->first);           
            setWriteDB.AddToByteArray((long)GetSkillValue( itr->second.lID , itr->second.lLevel, "CoolDown" ));
            setWriteDB.AddToByteArray((float)GetSkillValue( itr->second.lID , itr->second.lLevel,"MinAtkDistance" ));
            setWriteDB.AddToByteArray((float)GetSkillValue( itr->second.lID , itr->second.lLevel,"MaxAtkDistance" ));
            setWriteDB.AddToByteArray((DWORD)GetSkillValue( itr->second.lID , itr->second.lLevel,"SMp" ));
			lRestTime = lCurTime - itr->second.lEndCooldown;
			if( lRestTime < 0 ) lRestTime = 0;
			setWriteDB.AddToByteArray(lRestTime);
		}
	}
	else
	{
		setWriteDB.AddToByteArray(GetPetType());
		setWriteDB.AddToByteArray(m_DiffAttrs.lLevel);
		setWriteDB.AddToByteArray(m_DiffAttrs.lHp);
		setWriteDB.AddToByteArray(m_DiffAttrs.lMp);
		setWriteDB.AddToByteArray(m_DiffAttrs.lEnergy);
		setWriteDB.AddToByteArray(GetMaxHP());
		setWriteDB.AddToByteArray(GetMaxMP());
		setWriteDB.AddToByteArray(GetMaxEnergy());
	}
	return true;
}

bool CPet::CodeAttrToClient(DBWriteSet& setWriteDB)
{
	setWriteDB.AddToByteArray(&m_DiffAttrs, sizeof(tagPetDiff));
	setWriteDB.AddToByteArray(&m_TempAttrs, sizeof(tagPetTemp));
	long lNum = (long)m_Skills.size();
	setWriteDB.AddToByteArray(lNum);
	ITR_SKILL_LIST itr = m_Skills.begin();
	for( ; itr != m_Skills.end(); ++itr )
	{
		setWriteDB.AddToByteArray(&itr->second, sizeof(tagSkill));
	}

	return true;
}

// GS==>WS����
bool CPet::CodeToDataBlock(DBWriteSet& setWriteDB,
						   bool bExData)
{
	CMoveShape::CodeToDataBlock(setWriteDB, bExData);

	setWriteDB.AddToByteArray(&m_DiffAttrs, sizeof(m_DiffAttrs));
	setWriteDB.AddToByteArray(m_HostID);		// ����
	long lNum = (long)m_Skills.size();
	setWriteDB.AddToByteArray(lNum);
	ITR_SKILL_LIST itr = m_Skills.begin();
	for( ; itr != m_Skills.end(); ++itr )
	{
		setWriteDB.AddToByteArray(&itr->second, sizeof(tagSkill));
	}

	return true;
}

// WS==>GS����
bool CPet::DecodeFromDataBlock(DBReadSet& setReadDB,
							   bool bExData)
{
	CMoveShape::DecodeFromDataBlock(setReadDB, bExData);

	setReadDB.GetBufferFromByteArray(&m_DiffAttrs, sizeof(m_DiffAttrs));
	setReadDB.GetBufferFromByteArray(m_HostID);		// ����
	long lSkillNum = setReadDB.GetLongFromByteArray();
	tagSkill tSkill;
	for( long l=0; l<lSkillNum; ++l )
	{
		setReadDB.GetBufferFromByteArray(&tSkill, sizeof(tSkill));
		AddSkill(tSkill);							// ��Ӽ���
	}

	// ��ʼ���������ԺͲ�������
	bool isWild = m_DiffAttrs.ServantAttr.lWild?true:false;
	if (!InitialDiffAttrs(IT_LOGIN_GS,isWild))
		return false;

	/// ��ʼ����ʱ����
	OnCreate();
	return true;
}

CMoveShape* CPet::GetHost(void)
{
	if(NULL == m_pHost)
	{
        Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_12",GetExID().tostring()));
	}

	return (CMoveShape*)m_pHost;
}

void CPet::SetSkillCooldown(long lID, DWORD dwTime)
{
}

// �Ƿ�ɱ�GUIDΪrID��MoveShape����
bool CPet::IsVisible(OBJECT_TYPE eType, const CGUID& rID)
{
	bool bRet = true;
	CBaseObject* pRgn = GetFather();
	if( pRgn != NULL )
	{
		CBaseObject* pObj = pRgn->FindChildObject(eType, rID);
		CMoveShape* pShape = dynamic_cast<CMoveShape*>(pObj);
		if( pShape != NULL )
		{
			bRet = CMoveShape::IsVisible(pShape);
		}
	}
	return bRet;
}

long CPet::AddActiveSkill(DWORD dwSkillID,
						  DWORD dwLevel,
						  CMoveShape *pTarget,
						  long lGoodsIndex,
						  long lContainerID,
						  long lPos)
{
	SetInstanceSkill(NULL);
	CMoveShape::InitSkillInstance(dwSkillID, dwLevel,0,0,0,NULL_GUID,3);
	return 1;
}


/// ���������Ƿ����̴�
bool CPet::IsServant(void)
{
	return (GetPetType()==PET_TYPE_SERVANT)? true: false;
}

/// ��������㷨
long CPet::GetBaseTalentRadomVal(void)
{
	int	 randx = random(100);
	int  i = 0 , sum = 0 , nsum = 0;
	for (i = 0 ; i < PET_QUALITY_NUM;  ++i)
	{
		long lrand = UNI_CONF.talentAlogRate[i][2] - UNI_CONF.talentAlogRate[i][1] +1;
		sum += UNI_CONF.talentAlogRate[i][0];
		if ( randx < sum && nsum <= randx)
		{
			long talent = UNI_CONF.talentAlogRate[i][1] + random( lrand );
            Log4c::Trace(PET_MODULE,"%d.",talent);
			return talent;
		}
		nsum = sum;
	}  
    Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PET_13"));//��������㷨�������飡
	return 0;
}

/// Ʒ������㷨
long CPet::GetBaseQualityRadomVal(void)
{
	int randx = random(10000);
	for (int i = 0; i < PET_QUALITY_NUM; ++i)
	{
		int sum = m_BaseAttrs.lQualityRate[i];
		if (sum>randx)
		{
			return i;
		}
	}
    Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,GetText("GS_PET_14"));//Ʒ������㷨�������飡
	return -1;
}


/// �������ü��
//bool CPet::OnResetTalentCheck()
//{
//	if (!IsServant()) return false;
//
//	int  lev = m_DiffAttrs.lLevel;
//	int	 index = 1;
//	if ( lev <= UNI_CONF.nResetLev )
//		index = 0;
//	SetAttr("TalentLevel",index);
//	return true ;
//}

/// ���������㷨
bool CPet::OnReSetTalent()
{
	if (!IsServant()) return false;

#define CALC_TALENT(ATTR) \
	m_DiffAttrs.ServantAttr.talentVal.l##ATTR = (GetBaseTalentRadomVal() * \
	(m_BaseAttrs.lMax##ATTR-m_BaseAttrs.lMin##ATTR)) / 100 + m_BaseAttrs.lMin##ATTR

	// �������
	CALC_TALENT(Strenth);
	CALC_TALENT(Dexterity);
	CALC_TALENT(Con);
	CALC_TALENT(Intellect);
	CALC_TALENT(Spiritualism);

	// �ȼ���1	
	m_DiffAttrs.lLevel = 1; 
	// ����	
	m_DiffAttrs.lExp   = 0;

	// ���еȼ���1
	m_DiffAttrs.ServantAttr.lDLevel = 1;
	// ��Ϊ
	m_DiffAttrs.ServantAttr.lDExp = 0;

	// ����ƫ��
	m_DiffAttrs.ServantAttr.lSpeciality = random(PET_BASE_NUM);

	// ���Ե�
	memset(&m_DiffAttrs.ServantAttr.savvyVal,0,sizeof(tagPetBaseAttr));
	// ��ʯǿ����0
	memset(&m_DiffAttrs.ServantAttr.stoneUpVal,0,sizeof(tagPetBaseAttr));

	return true;
}

/// ��������ǿ�����
bool CPet::DAfterValCheck(void)
{
	int	DAfterCount = m_DiffAttrs.ServantAttr.lDAfterValCount;

	// ��ǰǿ������ С�� ����ǿ������<�˴��Գ����Ʒ��ֵ���ж�>
	if ( DAfterCount >= m_BaseAttrs.lGrade)
	{
		return false;
	}
	return true;
}

/// ��������ǿ��
bool CPet::OnDAfterValUpGrade(long DVal)
{
	if (!IsServant()) return false;

	int val;
	int	DAfterCount = m_DiffAttrs.ServantAttr.lDAfterValCount;
	int srval = UNI_CONF.dAfterStartRate - (DAfterCount-1)*UNI_CONF.dAfterDropRate;

#define DAFTER_UPGRADE(ATTR) \
	val   = DVal * (random(100-srval+1)+srval) /100; \
	m_DiffAttrs.ServantAttr.dAfterVal.l##ATTR += val;

	DAFTER_UPGRADE(Strenth);
	DAFTER_UPGRADE(Dexterity);
	DAFTER_UPGRADE(Con);
	DAFTER_UPGRADE(Intellect);
	DAFTER_UPGRADE(Spiritualism);

	// ���Ӻ�������ǿ������
	m_DiffAttrs.ServantAttr.lDAfterValCount++;

	return true;
}

/// ���ú�������ǿ������
bool CPet::OnResetDAfterCnt(void)
{
	if (!IsServant()) return false;

	if ( m_DiffAttrs.ServantAttr.lDAfterValCount < 1 )
	{
		// ��������ǿ������<1 ��ʾ��Ϣ����ǰ�̴Ӳ�û�б�ǿ�����ʣ�����������ܡ�
		return false;
	}
	m_DiffAttrs.ServantAttr.dAfterVal.ReSet();
	m_DiffAttrs.ServantAttr.lDAfterValCount = 0;
	return true;
}

/// �̴��ں�
bool CPet::OnServantSyncretism(void)
{
	// �����ж�
	return false;
}

/// �̴��ں��㷨
void CPet::CopyServantAttr(const CPet* pPet, const CPet* lPet, long lPLev)
{	
	// ���¼�������
	CALC_ELF_SYNCRETISM(pPet, lPet, lPLev);

	m_DiffAttrs.ServantAttr.lAges++;
	m_DiffAttrs.lLevel = 1;
}

/// �ںϼ��㹫ʽ<�Ժ�����>
void CPet::CALC_ELF_SYNCRETISM(const CPet* pPet, const CPet* lPet, long lPLev)
{
	m_DiffAttrs.ServantAttr.talentVal.lStrenth = 
		pPet->m_DiffAttrs.ServantAttr.talentVal.lStrenth 
		+ lPet->m_DiffAttrs.ServantAttr.talentVal.lStrenth / 3 * lPLev;

	m_DiffAttrs.ServantAttr.talentVal.lDexterity = 
		pPet->m_DiffAttrs.ServantAttr.talentVal.lDexterity 
		+ lPet->m_DiffAttrs.ServantAttr.talentVal.lDexterity / 3 * lPLev;

	m_DiffAttrs.ServantAttr.talentVal.lCon = 
		pPet->m_DiffAttrs.ServantAttr.talentVal.lCon 
		+ lPet->m_DiffAttrs.ServantAttr.talentVal.lCon / 3 * lPLev;

	m_DiffAttrs.ServantAttr.talentVal.lIntellect = 
		pPet->m_DiffAttrs.ServantAttr.talentVal.lIntellect 
		+ lPet->m_DiffAttrs.ServantAttr.talentVal.lIntellect / 3 * lPLev;

	m_DiffAttrs.ServantAttr.talentVal.lSpiritualism = 
		pPet->m_DiffAttrs.ServantAttr.talentVal.lSpiritualism 
		+ lPet->m_DiffAttrs.ServantAttr.talentVal.lSpiritualism / 3 * lPLev;
}

/// Ʒ��ǿ���㷨�����ж�
bool CPet::OnCElfPensValueUpGradeCheck()
{
	if (!IsServant())	return false;

	int quality = m_DiffAttrs.ServantAttr.lQuality;
	if (quality == PET_QUALITY_BRILLIANT)
	{
		/// �Ѿ������Ʒ��
		char str[100];
		sprintf_s(str,"Ʒ�ʲ�����ǿ��!");
		PRINT_TO_CLIENT(str);
		return false;
	}
	if ( m_DiffAttrs.lLevel < UNI_CONF.qualityMinLev)
	{
		/// �ȼ�����
		/// CMessage msg(MSG_S2C_PET_ELF_UPGRADE_MSGTYPE);
		/// msg.Add(PET_ELF_LEVELUPGR_MSGTYPE);
		char str[100];
		sprintf_s(str,"�ȼ�����!");
		PRINT_TO_CLIENT(str);
		return false;
	}

	int lExp = m_DiffAttrs.lExp;
	// rateΪ������Ϣ
	if ( lExp < UNI_CONF.Exps[m_DiffAttrs.lLevel] * UNI_CONF.qualityExpProp)	
	{
		/// ���鲻��,����ǿ��ʧ����Ϣ
		char str[100];
		sprintf_s(str,"���鲻��!");
		PRINT_TO_CLIENT(str);
		return false;
	}
	// ��վ�����Ŀ����
	//int  NNum = (lev +1)*UNI_ELFCONF.QDelta[lev] * m_BaseAttrs.lSNiceNum ;
	//SetAttr("NENice",NNum);
	return true;
}

/// Ʒ��ǿ���㷨�㷨
bool CPet::OnCElfPensValueUpGrade(long val)
{
	///  ���ӵĳɹ���
	int DropVal = GetDownRate(UNI_CONF.qualityLoyaProp);	
	/// ��ȡǿ���ɹ���
	int SuccessVal = (100 + DropVal) >= 100 ? 100 : (100 + DropVal) ;
	SuccessVal +=val;

	int srval = random(100)+1;
	if ( srval <= SuccessVal )
	{
#ifdef	_DEBUG
		/// ǿ���ɹ�
		/// CMessage msg(MSG_S2C_PET_ELF_UPGRADE_MSGTYPE);
		/// msg.Add(PET_ELF_PENVALUEUPGR_MSGTYPE);
		/// ����-5%
		char str[100];
		sprintf_s(str,"Ʒ��ǿ���ɹ�!");
		PRINT_TO_CLIENT(str);
#endif
		return true;
	}
	/// ǿ��ʧ��
	else
	{
#ifdef	_DEBUG
		char str[100];
		sprintf_s(str,"Ʒ��ǿ��ʧ��!");
		PRINT_TO_CLIENT(str);
#endif
		return false;
	}
}

/// ȡ��Ʒ��ǿ��������վ�����Ŀ
long CPet::GetLessNice()
{
	int quality = m_DiffAttrs.ServantAttr.lQuality;
	//return (quality +1)*UNI_CONF.QDelta[lev] * m_BaseAttrs.lSNiceNum ;
	return 1;
}

/// �������Ե�
bool  CPet::SetSawyPointValue(const tagPetBaseAttr& rtag) 
{
	long  lSawyV = m_DiffAttrs.ServantAttr.savvyVal.GetTotalVal();
	long  total  = rtag.GetTotalVal() + lSawyV;

	long  slev   = m_DiffAttrs.lLevel * UNI_CONF.savvyCnt;
	/// ���Ե��ܺ� ���� ���Եȼ�
	if ( total <= slev )
	{
		m_DiffAttrs.ServantAttr.savvyVal.SetElfBase(rtag);
		return true;
	}
	return false;
}

/// �������Ե�
bool CPet::OnResetSavvy(void)
{
	if (!IsServant())	return false;

	m_DiffAttrs.ServantAttr.savvyVal.ReSet();
	return true;
}


/// ��ʯǿ�������ж�
bool CPet::OnFStoneUpgradeCheck()
{
	if (!IsServant())	return false;

	/// �ȼ�
	long  lev   =  m_DiffAttrs.lLevel;
	long  levLimit = /*UNI_CONF.nFSUpGradeMulite*/10 * m_DiffAttrs.ServantAttr.lFStoneUpgradeFlag + UNI_CONF.fStoneMinLev;
	/// �жϵȼ�
	if ( lev >= UNI_CONF.fStoneMinLev && lev <= UNI_CONF.fStoneMaxLev  && lev >= levLimit ) 
	{
		int lExp = m_DiffAttrs.lExp;
		/// �жϾ���
		if ( lExp >= (UNI_CONF.Exps[lev] * UNI_CONF.fStoneExpProp / 100) ) 
		{
#ifdef	_DEBUG
			/// ���鲻��,����ǿ��ʧ����Ϣ
			char str[100];
			sprintf_s(str,"���鲻��,���ܽ��л�ʯǿ��");
			PRINT_TO_CLIENT(str);
#endif
			return true;
		}
	}	
	return  false;
}

/// ��ʯǿ��
bool  CPet::OnFStoneUpgrade(long fstone)
{
	// �ȼ�
	long  lev   =  m_DiffAttrs.lLevel;
	long  lsfup =  m_DiffAttrs.ServantAttr.lFStoneUpgradeFlag;
	long  rTemp =  UNI_CONF.fStoneSuccesRate[lsfup];	
	long  Addval=  fstone;			

	/// ���ڵĳɹ���
	rTemp *=(1.0 + Addval/100.0);			
	int lExp = m_DiffAttrs.lExp;
	if ( lExp < (UNI_CONF.Exps[lev] * UNI_CONF.fStoneExpProp / 100) )
	{
		// ���鲻��,����ǿ��ʧ����Ϣ
		return false;
	}

	/// ���ֵ��
	int srandx = random(1000)+1;
	/// ǿ���ɹ�
	if ( srandx <= rTemp )
	{
		/// ���� -5%   lsfup+1   
		/// ��������+++
		char str[100];
		sprintf_s(str,"��ʯǿ���ɹ�");
		PRINT_TO_CLIENT(str);
		return true;
	}
	/// û�гɹ�
	else
	{
		char str[100];
		sprintf_s(str,"��ʯǿ��ʧ��");
		PRINT_TO_CLIENT(str);
		return false;
	}
}


/// ������λǿ���������
bool CPet::OnlFUpgradeCheck()
{
	if (!IsServant())	return false;

	int lev = m_DiffAttrs.lLevel;
	if (lev < UNI_CONF.exclamaMinLev)
	{
		char str[100];
		sprintf_s(str,"������λǿ���ȼ�����");
		PRINT_TO_CLIENT(str);
		return false;
	}
	return true;
}

/// ���ݳ����ҳ϶�ǿ���ɹ��ʵİٷֱ�
int  CPet::GetDownRate(int drop)
{
	int DownVal  = 0;
	int lLoyalty = GetAttr("Loyalty");//m_DiffAttrs.lLoyalty;

	if (lLoyalty< UNI_CONF.exclamaLoyaProp)
	{
		DownVal -=drop;
	}

	return DownVal;
}

/// ������λǿ�� 
bool CPet::OnlFUpgrade( long rate )
{
	/// ���ӳɹ�����
	int fsval = m_DiffAttrs.ServantAttr.lFUpgradeCount;
	int rDrop = GetDownRate(UNI_CONF.exclamaDropProp);	
	int randx = random(10000)+1;


	/// ��ֱ�
	float rTemp = UNI_CONF.exclamaSuccesRate[fsval] * 10 ;
	rTemp *= (1.0 + rDrop/100.0+rate/100.0)*10;

	/// ǿ���ɹ�
	if ( randx <= rTemp )
	{
		fsval ++;
		m_DiffAttrs.ServantAttr.lFUpgradeCount ++;

		char str[100];
		sprintf_s(str,"������λǿ���ɹ�");
		PRINT_TO_CLIENT(str);
		/// Send to Client operator Successful
		return true;
	}
	else	/// ǿ��ʧ�ܣ�������������
	{
		/// ����-5% 
		int lExp = m_DiffAttrs.lExp;
		lExp = lExp * (100 - UNI_CONF.exclamaExpProp) /100.0;

		m_DiffAttrs.lExp = lExp;
		char str[100];
		sprintf_s(str,"������λǿ��ʧ��");
		PRINT_TO_CLIENT(str);
		return false;
	}
}

/// �ǵ�ǿ���������
bool CPet::OnStarLevUpGradeCheck()
{
	if (!IsServant())	return false;

	int Slev= GetAttr("StarLev");
	if ( Slev > UNI_CONF.starMaxLev)
	{
		return false;
	}
	return true;
}

/// �ǵ�ǿ��
bool CPet::OnStarLevUpGrade(long rates)
{
	/// ���ǿ���ɹ���
	int rate = rates;			
	//m_AddUpGradeRate = 0;
	int Slev = GetAttr("StarLev");
	int rRate= (100 + rate)/100 * UNI_CONF.starSuccessRate[Slev];
	int randx= random(100)+1;
	/// ǿ���ɹ� 
	if ( randx < rRate)
	{
		Slev ++;
		if ( Slev >= 5 )
		{
			/// ���͡�ϵͳ����ѧϰ��ǧ����࣬��������
			char str[100];
			sprintf_s(str,"�ǵ�ǿ���ɹ�,���͡�ϵͳ����ѧϰ��ǧ����࣬��������");
			PRINT_TO_CLIENT(str);
		}
		else 
		{
			/// ���� ��ϵͳ����ǿ������ɹ�
			char str[100];
			sprintf_s(str,"�ǵ�ǿ���ɹ�,���͡�ϵͳ��:��ǿ������ɹ�");
			PRINT_TO_CLIENT(str);
		}
		return true;
	}
	else
	{
		/// ���� ��ϵͳ����ǿ������ʧ��.
		char str[100];
		sprintf_s(str,"�ǵ�ǿ���ɹ�,���͡�ϵͳ��:��ǿ������ʧ��");
		PRINT_TO_CLIENT(str);
		return false;
	}
}

/// �ı��ҳ϶�
void CPet::OnChangeLoy(long lDelta)
{
	long tLoyalty = GetAttr("Loyalty");//m_DiffAttrs.lLoyalty ;
	long tMaxLoyalty = UNI_CONF.loyaltyMax;

	tLoyalty += lDelta;
	if ( tLoyalty > tMaxLoyalty )
	{
		tLoyalty = tMaxLoyalty;
	}
	else if (tLoyalty<=0)
	{
		tLoyalty = 0;
	}
	SetAttr("Loyalty",tLoyalty);

	// �����ҳ϶ȵȼ�״̬
	int oldLoyLev = GetAttr("LoyLev");
	int newLoylev = CalcLoyLevel();
	SetAttr("LoyLev", newLoylev);

	if ( newLoylev != oldLoyLev )
	{
        char szLog[256];
		sprintf(szLog, "�ҳ϶ȵȼ���%d����Ϊ%d.", oldLoyLev, newLoylev);
		PRINT_TO_CLIENT(szLog);
	}
    Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_15",tLoyalty));//�����ҳ϶�ֵ:: [Loyalty ]-->%ld.
}

/// �ı�����ֵ
void CPet::OnChangeLife(long lDelta)
{
	long tLife = GetAttr("Life");
	long tMaxLife = UNI_CONF.maxLife;

	tLife += lDelta;
	if ( tLife > tMaxLife)
	{
		tLife = tMaxLife;
	}
	else if (tLife<=0)
	{
		tLife = 0;
	}
	SetAttr("Life",tLife);
    Log4c::Error(PET_MODULE,"%-15s %s",__FUNCTION__,FormatText("GS_PET_16",tLife));//����ֵ����: [Life ]-->%ld.
}

/// �����Я����������
long CPet::AddServantTakeNum(long num)
{
	CPlayer * Phost = (CPlayer*)GetHost();
	//return Phost->ChangeMaxServantNum(num);
	return 1;
}

///// Ʒ��ǿ��ʱ�������Ҿ���
//const CGUID & CPet::IsTypeElfLessLev()
//{
//	return m_pHost->GetQueryElfTypeWithLev(GetLevel(),m_DiffAttrs.szName);
//}

/// ɾ������
//long    CPet::DelSameElf(CGUID & guid)
//{
//	CPet *pPet = GetInst(CPetCtrl).GetPet(guid);
//	GetInst(CPetCtrl).DelPet(pPet,PET_OT_DELETE);	
//	return 0;
//}


/// �õ�Lev��Ҫ����������
inline	long CPet::GetALevNeedExp(long  _lev)
{		
	if ( _lev < UNI_CONF.lMaxLevel )
	{
		return UNI_CONF.Exps[_lev];
	}
	return -1;
}

///  ���ÿ�������������Ե�������
inline long	CPet::GetUpGradeUpValue(long _talentval , long _BaseVal)
{
	//return (GetTalentValue(_talentval) - 1.0 ) * _BaseVal + _BaseVal;
	return 0;
}

/// �ٻ�����, �ж�������
bool   CPet::VacationPet( long lev )
{
	/// �ҳ϶�̫�Ͳ��ܽ����ٻ�
	int lLoyalty=m_DiffAttrs.lLoyalty;		// GetAttr("Loyalty");


	if ( lLoyalty < UNI_CONF.loyaltyMin)
	{
		char str[100];
		sprintf_s(str,"�ٻ����ڵȼ������ҳ϶�̫��!");
		PRINT_TO_CLIENT(str);
		return false;
	}
	if ( m_DiffAttrs.lLevel < lev )
	{
#ifdef _DEBUG
		char str[100];
		sprintf_s(str,"�ٻ��������ڵȼ����ƣ���ʧ��!");
		PRINT_TO_CLIENT(str);
#endif
		return false;
	}
	return true;
}

/// ȡ�ó���Ʒ��
long CPet::GetGrade(void)
{
	if (!IsServant())
	{
		return -1;
	}
	return m_BaseAttrs.lGrade;
}

/// ȡ�ó������������ǿ������
long CPet::GetDAfterCnt(void)
{
	if (!IsServant())
	{
		return -1;
	}
	return m_DiffAttrs.ServantAttr.lDAfterValCount;
}

bool CPet::OnServantUpGrade(long uId , long uRate)
{
	if ( uId == PET_QUALITYUP_ID )
	{
		return OnCElfPensValueUpGrade(uRate);
	}
	//else if ( uId == PET_FFUPGRADEID )
	//{
	//	return OnlFUpgrade( uRate );
	//}
	else if ( uId == PET_FSTONEUP_ID )
	{
		return OnFStoneUpgrade(uRate);
	}
	else if ( uId == PET_STARTUP_ID)
	{
		return OnStarLevUpGrade( uRate );
	}
	else if ( uId == PET_TALENT_RESET_ID)
	{
		return OnReSetTalent();
	}
	return false;
}