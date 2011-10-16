#include "stdafx.h"
#include "MoveShape.h"
#include "player.h"
#include "monster.h"
#include "Effect.h"
#include "ClientRegion.h"
#include "Other/StateListXml.h"
#include "Other\DeputyOccuSystemClient.h"
#include "Skills/SkillListXml.h"
#include "Skills/SkillListIni.h"
#include "../../EngineExtend\gamemodelmanager.h"
#include "../../EngineExtend\textureprojective.h"
#include "../../EngineExtend\gamemap.h"
#include "../../EngineExtend\DisplayModel.h"
#include "other/Chat.h"
#include "Other\FaceActionList.h"

#include "Other/AudioList.h"
#include "PetSystem/Pet.h"
#include "Player.h"
#include "../../GameClient/GameControl.h"
#include "../../GameClient/ClientApp/Skills/SkillXml.h"
#include "../GameClient/Game.h"
#include "../../Public/Common/Public.h"
#include "Other/GlobeSetup.h"
#include "../../Public/Common/SkillRelated.h"
//Vector3 d;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define  NOTANGPENG

long CMoveShape::s_lIdleMinCount = 1000;		// �������idle�����ļ���
long CMoveShape::s_lIdleMaxCount = 50000;	// ��ٽ���ilde�����ļ���
DWORD CMoveShape::m_dwMoveTimeCounter = 0;
DWORD CMoveShape::m_dwLastMoveTimeCounter = 0;

float	CMoveShape::highhurt_monster = 0.0f;
float	CMoveShape::highhurt_boss = 0.0f;
float	CMoveShape::highhurt_knight = 0.0f;
float	CMoveShape::highhurt_bow = 0.0f;
float	CMoveShape::highhurt_magic = 0.0f;
float	CMoveShape::mildhurt_monster  = 0.0f;
float	CMoveShape::mildhurt_boss  = 0.0f;
float	CMoveShape::mildhurt_knight  = 0.0f;
float	CMoveShape::mildhurt_bow  = 0.0f;
float	CMoveShape::mildhurt_magic  = 0.0f;

CMoveShape::CMoveShape(void)
{
	m_fDestX = .5f;
	m_fDestY = .5f;
	m_lStopCount = 0;
	m_lIdleCount = 0;
    m_moveOffsetX = 0;
    m_moveOffsetY = 0;
	m_bIdleStart = false;

	m_strTalkText = "";
	m_lTalkLiveTimer = 1000;

	m_pForceMove=NULL;
	m_bInForceMoving=false;

	m_lMoveSoundCount = 13;
	m_dwEndMoveTime = 0;
	m_dwLastUpdatePosTime = 0;

	m_dwCurSkillActTime = 0;
	m_dwCurSkillStartTime = 0;
	// ƽ����λ���
	m_bIsSlipState = false;
	m_fSlipSpeed = 0.f;
	m_fSlipDestPosX = 0.f;
	m_fSlipDestPosY = 0.f;
	m_fSlipDir = 0.f;
	m_dwSlipEndMoveTime = 0;
	m_dwSlipLastUpdatePosTime = 0;

	m_bIsArrive = true;

	//��Ӧ�ɼ�������
	m_strCollectionName = "";
	//�ɼ���id
	m_dwCollectID = 0;
	//�ܱ��ɼ��Ĵ���
	m_dwCollectTimes = 0;
	//�Ƿ����ڱ��ɼ�
	m_bIsBeenCollect = false;
	//�Ƿ��ܱ��ɼ�
	m_bCanBeenCollect = false;

	m_bRun					= true;			// ��
	m_bAntiInvisibleable    = true;			// ���ܷ�����
	m_bInvisible			= true;			// ��������	
	m_bPretendDead			= true;			// ����װ��	

    m_sMoveable					= 1;			//���ƶ���־��
    m_sSelfControl				= 1;			//�ɿ��Ʊ�־��
    m_sUseSkill					= 1;			//��ʹ�ü��ܱ�־��
    m_sMoveDamage				= 1;			//���Ƴ��������ñ�־��
    m_sBeenAttack				= 1;			//�ɱ�������־��
    m_sRandomMove				= 1;		    //������ܶ�

	m_bUseRabbSkill			= true;			// ����ʹ�÷�ʦ����
	m_bUseFighterSkill		= true;			// ����ʹ��սʿ����
	m_bUseItem				= true;			// ����ʹ����Ʒ
	m_bInstruction			= true;			// ����ʹ��ָ��
	m_bUnAttack				= true;			// ���Թ�����־��
	m_bAddHp				= true;			// ����hp���ӱ�־��
	m_bHide					= true;			// ��������
	m_bForceAttact			= true;			// ����ǿ�ƹ���
	
	m_lChangeGraphicstype	= 0;
	m_lChangeGraphicsID		= 0;
	m_bNotRide					= true;			// û�г���
	m_bStateColorPlay = FALSE;
	m_bColorFade	  = true;
	m_bColorFadeFlag = true;
	m_fColorFadePer = 1.0f;
	ZeroMemory(&m_CurrentStateExForStateColor, sizeof(tagStateEx));

	// ����Ч����ض���
	m_fXIncrement  = 0.0f;
	m_fYIncrement  = 0.0f;
	m_fShakeMax    = 0.0f;
	m_fShakeMin	   = 0.0f;
	m_bRestoration = false;
	ZeroMemory(m_listAddEffect, sizeof(tagAddEffect) * 2);

	for (int i = 0; i < 12; ++i)
		m_fActionCustomSpeedRate[i] = 1.0f;

	ZeroMemory(&m_stCurrentFaceAnim, sizeof(tagFaceAnim));
	ZeroMemory(&m_stCurrentActionAnim, sizeof(tagActionAnim));
	m_stCurrentActionAnim.iActionIndex = -1;
	m_iActionChannel = 0;
	m_bPlayActionAudio = false;
	m_strActionAudioFile = "";

	m_dwIntonateTime = 0;
	m_dwIntonateStartTime = 0;
	m_wIntonateSkillID = 0;
	m_lIntonateSkillKey = 0;
	m_bIntonateAlpha = 0;
	m_bIsLToR = true;
	m_busesimplemodel = false;
	m_iLastColorLevel = 0;
    m_PhaseID = NULL_GUID;
}



/*
* ����: ��������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2009.02.09 - lpf
*		��������Ѫ�б�����
*/
CMoveShape::~CMoveShape(void)
{
	SAFE_DELETE(m_pForceMove);
	DeleteCurSkillList();		// ɾ�������б�

	m_listBloodText.clear();
}

// ��ʼ����������ֵ��Ӱ��
void CMoveShape::InitNameValueMap()
{
	tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	st.lType=3; st.pfValue=&m_fSpeed;				m_PropertyEnumToName[S_ATTR_SPEED]="Speed";	   					m_mapNameValue["Speed"] = st;
	st.lType=1; st.pwValue=&m_wState;				m_PropertyEnumToName[S_ATTR_SHAPESTATE]="ShapeState";			m_mapNameValue["ShapeState"] = st;	
	st.lType=6; st.pbValue=&m_bInvisible;			m_PropertyEnumToName[S_ATTR_INVISIBLETIME]="Invisible";			m_mapNameValue["Invisible"] = st;		//����
	st.lType=6; st.pbValue=&m_bHide;				m_PropertyEnumToName[S_ATTR_HIDE]="Hide";						m_mapNameValue["Hide"] = st;			//����
	st.lType=6; st.pbValue=&m_bPretendDead;			m_PropertyEnumToName[S_ATTR_PRETENDDEADTIME]="PretendDead";		m_mapNameValue["PretendDead"]=st;		//����
	st.lType=4; st.pValue=&m_lChangeGraphicstype;	m_PropertyEnumToName[S_ATTR_CHANGEMODELTYPE]="ChangeModelType";	m_mapNameValue["ChangeModelType"]=st;	//�ı�����
	st.lType=4; st.pValue=&m_lChangeGraphicsID;		m_PropertyEnumToName[S_ATTR_CHANGEMODELID]="ChangeModelID";		m_mapNameValue["ChangeModelID"]=st;	//�ı�ģ��id
	st.lType=6; st.pbValue=&m_bInstruction;			m_PropertyEnumToName[S_ATTR_USEINSTRUCTION]="UnUseInstruction";	m_mapNameValue["UnUseInstruction"]=st;	//ʹ��ָ��
	
    st.lType=5; st.psValue=&m_sMoveable;			m_PropertyEnumToName[S_ATTR_MOVEABLE]="sMoveable";		   		m_mapNameValue["sMoveable"]=st;			//�ܷ��ƶ�
    st.lType=5; st.psValue=&m_sSelfControl;			m_PropertyEnumToName[S_ATTR_SELFCONTROL]="sSelfControl";		m_mapNameValue["sSelfControl"]=st;	    //�ܷ����ҿ���
    st.lType=5; st.psValue=&m_sUseSkill;			m_PropertyEnumToName[S_ATTR_USEALLSKILL]="sUseAllSKill";		m_mapNameValue["sUseAllSKill"]=st;		//�Ƿ����ʹ�ü���
    st.lType=5; st.psValue=&m_sMoveDamage;			m_PropertyEnumToName[S_ATTR_MOVEDAMAGE]="sMoveDamage";		   	m_mapNameValue["sMoveDamage"]=st;		//�Ƿ��ƶ�����Ч��
    st.lType=5; st.psValue=&m_sBeenAttack;			m_PropertyEnumToName[S_ATTR_CANBEENATTACK]="sCanBeenAttack";	m_mapNameValue["sCanBeenAttack"]=st;	//�ܷ񱻹���
    st.lType=5; st.psValue=&m_sRandomMove;			m_PropertyEnumToName[S_ATTR_RAMDOMMOVE]="sRandomMove";			m_mapNameValue["sRandomMove"]=st;		//����ܶ�

	st.lType=6; st.pbValue=&m_bUseFighterSkill;		m_PropertyEnumToName[S_ATTR_USEFIGHTSKILL]="UseFighterSkill";	m_mapNameValue["UseFighterSkill"]=st;	//ʹ��սʿ����
	st.lType=6; st.pbValue=&m_bUseRabbSkill;		m_PropertyEnumToName[S_ATTR_USERABBSKILL]="UseRabbSkill";		m_mapNameValue["UseRabbSkill"]=st;		//ʹ�÷�ʦ����
	st.lType=6; st.pbValue=&m_bUseItem;				m_PropertyEnumToName[S_ATTR_USEITEM]="UseItem";					m_mapNameValue["UseItem"]=st;			//ʹ�õ���
	st.lType=6; st.pbValue=&m_bAntiInvisibleable;	m_PropertyEnumToName[S_ATTR_ANTIINVISIBLEABLE]="AntiInvisibleable";	m_mapNameValue["AntiInvisibleable"]=st;//������
	st.lType=6; st.pbValue=&m_bAddHp;				m_PropertyEnumToName[S_ATTR_ADDHP]="AddHp";						m_mapNameValue["AddHp"]=st;		    //��������HP
	st.lType=6; st.pbValue=&m_bUnAttack;			m_PropertyEnumToName[S_ATTR_UNATTACK]="UnAttack";				m_mapNameValue["UnAttack"]=st;		    //���ܱ�����
	st.lType=6; st.pbValue=&m_bForceAttact;			m_PropertyEnumToName[S_ATTR_FORCEATTACK]="ForceAttack";			m_mapNameValue["ForceAttack"]=st;		//ǿ�ƹ���

	st.lType=2; st.pdwValue=&m_dwCollectID;			m_PropertyEnumToName[S_ATTR_COLLECTIONID]="CollectionID";		m_mapNameValue["CollectionID"]=st;		 //�ɼ���id
	st.lType=2; st.pdwValue=&m_dwCollectTimes;		m_PropertyEnumToName[S_ATTR_COLLECTIONTIMES]="CollectionTimes";	m_mapNameValue["CollectionTimes"]=st;	 //�ܲɼ��Ĵ���
	st.lType=6; st.pbValue=&m_bIsBeenCollect;		m_PropertyEnumToName[S_ATTR_ISBEENCOLLECT]="IsBeenCollect";		m_mapNameValue["IsBeenCollect"]=st;	 //�Ƿ����ڱ��ɼ�
	st.lType=6; st.pbValue=&m_bCanBeenCollect;		m_PropertyEnumToName[S_ATTR_CANBEENCOLLECT]="CanBeenCollect";	m_mapNameValue["CanBeenCollect"]=st;	 //�ܷ񱻲ɼ�

	st.lType=6; st.pbValue=&m_bNotRide;		m_PropertyEnumToName[S_ATTR_NOTRIDE]="Ride";	m_mapNameValue["Ride"]=st;	 //�Ƿ�����
}

void CMoveShape::SetState(WORD l)
{
	if(l < STATE_PEACE || l>STATE_OPEN)
	{
		Log4c::Warn(ROOT_MODULE,"�Զ���(type:%d,name:%s)�����˴����״̬(%d)",GetType(),GetName(),l);
		return;
	}
	CShape::SetState(l);
}

bool g_bForceUpdate = false;



/*
* ����: �趨����
* ժҪ: -
* ����: l - ��������
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.02.15 - lpf
*		������"����ǰ���Ͷ���ʱ�䳤��"
*	2008.02.18 - lpf
*		��"����ǰ���Ͷ���ʱ�䳤��"�޸�Ϊ"����ǰ���Ͷ�������"
*	2008.03.27 - lpf
*		�����˶���һ���������ʽ��б���Ĵ���
*	2008.04.22 - lpf
*		����Һ͹���Ķ����趨�������˹�����̬����������
*	2008.12.26 - lpf
*		�����������̬����֮��Ĳ�ֵ����
*	2009.04.08 - lpf
*		���������趨�ɿ���ʱ,"��ǰ�������ŵĴ�������"�趨Ϊ-1,������ѭ��
*/
void CMoveShape::SetAction(WORD l)
{
	if(l < ACT_STAND || (l>ACT_SEL_X5 && l<ACT_PERFORM) || l>ACT_MAX_PERFORM)
	{
		Log4c::Warn(ROOT_MODULE,"�Զ���(type:%d,name:%s)�����˴���Ķ���(%d)",GetType(),GetName(),l);
		return;
	}

	// ����û�б仯��ֱ�ӷ���
	if (m_wAction==l && g_bForceUpdate==false)
		return;

 	if (m_lType == TYPE_PET && l == ACT_RUN)
	{
		CPet *pPet = (CPet*)this;
		pPet->ConfusePetStandInfo();
	}
	// ����ʹ�õļ���
	if(this == GetGame()->GetMainPlayer() && m_wAction < ACT_PERFORM)
	{
		SetCurSkillActTime(0);
		SetCurSkillStartTime(0);
	}
	g_bForceUpdate = false;

	//�õ�����
	DWORD dwOldAction = m_wAction;
	DWORD dwCurrentTime = GetCurTickCount();
	//---------------------------------------------
	m_wAction = l;

	// �����վ�����������Idle����
	if (m_wAction!=ACT_STAND)
	{
		m_lIdleCount = 0;
		m_bIdleStart = false;
	}

	AnimInfo::tagActionInfo *pActionInfo = m_pAnimInfo->GetActionInfo();
	pActionInfo->bStayToLastFrame = false;
	pActionInfo->dwCurActionLoopCount = 0;

	if (m_wAction == CShape::ACT_STAND || m_wAction == CShape::ACT_INTONATE
		|| m_wAction == CShape::ACT_WALK || m_wAction == CShape::ACT_RUN 
		|| m_wAction == CShape::ACT_MEGRIM || m_wAction == CShape::ACT_ALREADYDIED ||
		GetType() == TYPE_EFFECT )
	{
		pActionInfo->bCurActionLooped = TRUE;
	}
	else
	{
		pActionInfo->bCurActionLooped = FALSE;
	}

	//���ö������ڲ�
	if ( GetType() == TYPE_EFFECT || GetType() == TYPE_GOODS || GetType() == TYPE_BUILD || GetType() == TYPE_SUMMON || GetType() == TYPE_CITYGATE )
	{
	}
	else
	{
		//���κ����ƻص�վ��
		if (m_wAction == CShape::ACT_STAND)
		{
			pActionInfo->bInterpolation = TRUE;
			pActionInfo->dwOldAction = pActionInfo->dwCurAction;
			pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
			pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
			pActionInfo->dwInterpolationElapseTime = 0;
			pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
			pActionInfo->dwInterpolationTimeLength = 240;
			if (m_pDisplayModel)
			{
				m_pDisplayModel->SetAllTrailEmitter(FALSE);
			}

		}
	}

	pActionInfo->dwCurActionStartTime = dwCurrentTime;

	// ����ǰ���Ͷ�������
	if (l > -1 && l < 12)
		m_pAnimInfo->SetCurActionSpeedRate(m_fActionCustomSpeedRate[l]);

	//---------------------------------------------
	static char str[32];
	long lType = GetType();
	long lGraphicsID = GetGraphicsID();
	float fDir = GetNowDir();
	long lState = 0;
	long lAct = m_wAction;
	//�Ƿ�������
	DWORD dwWeaponIndex=0;
	//������������
	DWORD dwWeaponActType = 0;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(this);
	switch(lType)
	{
		// ���
	case TYPE_PLAYER:
		{
			//״̬							                      
			static char *strState[] =  
			{
				"B",	//��ƽ
				"E",	//����
				"S",	//���ֽ�
				"H",	//���ִ�
				"K",	//��
				"X",	//��
				"A",	//˫�ָ�
				"W",	//˫����
				"B",	//��
				"M",	//ȭ��
				"T",	//�h
                "G"     //��
			};
			//���ﶯ������							                                      
			static char *strActionHorse[] = 
			{
				"S",	//վ��  
				"I",	//վ��idle    
				"W",	//���� 
				"R",	//�ܲ�
				"H",	//������  
				"903",	//���� 
				"B",	//��
				"Z",	//�м� 
				"K",	//���� 
				"Y",	//ѣ�� 
				"D00",	//����
				"SIQU", //ʰȡ
				"D01",  //�Ѿ�����
				"D01",  //�Ѿ�����
				"OPEN", //����
				"AROP", //�Ѿ�����
				"DOWN"  //ˤ��
			};
			//��������							                                      
			static char *strAction[] = 
			{
				"B00",	//վ��  
				"I00",	//վ��idle    
				"W00",	//���� 
				"R00",	//�ܲ�
				"H00",	//������  
				"903",	//���� 
				"LK0",	//��
				"P00",	//�м� 
				"T00",	//���� 
				"Y00",	//ѣ�� 
				"D00",	//����
				"PIK", //ʰȡ
				"D01",  //�Ѿ�����
				"OPEN", //����
				"AROP", //�Ѿ�����
				"DOWN"  //ˤ��
			};
			//��������
			static char *strAttackAction[] = 
			{
				"K00",	//����1
				"K01",	//����2
				"K02"	//����3
			};
			static char* strsingleperson[]=
			{
				"0",
				"1"
			};
			static char* strhorsetype[]=
			{
				"1",
				"2",
				"3"
			};
			// �õ������Լ���������
			if (pPlayer==GetGame()->GetMainPlayer())
			{
				CGoods* pEquip = pPlayer->GetEquip(CPlayer::EQUIP_WEAPON);
				//�Ƿ�������
				dwWeaponIndex = pEquip?pEquip->GetProperty()->dwIndex:0;
				//����ɶ�Ӷ�������
				dwWeaponActType = pEquip?pEquip->GetProperty()->dwWeaponActType:0;
			}
			// ���������������
			else	
			{
				dwWeaponIndex = pPlayer->GetEquipIndex(CPlayer::EQUIP_WEAPON);
				CGoodsList::tagGoods2 *pG = CGoodsList::GetProperty(dwWeaponIndex);
				if (pG)
					dwWeaponActType = pG->BaseProperty.dwWeaponActType;
				else
					dwWeaponActType = 0;
			}

			//ʩ������
			if (lAct >= CShape::ACT_PERFORM && lAct < ACT_MAX)
			{
				//���û������(����)
				if (dwWeaponIndex == 0)
				{
					lState = 1;
				}
				else
				{
					lState = dwWeaponActType + 2;
				}

				if(lAct >= ACT_MINE && lAct <= ACT_MINEMAX)
				{
					sprintf_s(str,"C%03d",lAct - CShape::ACT_PERFORM);
					m_iActionCount = -1;
				}else if (lAct >= ACT_FACEACTION && lAct <= ACT_FACEACTIONMAX)
				{
					sprintf_s(str, "V%03d", lAct);
					pActionInfo->bInterpolation = TRUE;
					pActionInfo->dwOldAction = pActionInfo->dwCurAction;
					pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
					pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
					pActionInfo->dwInterpolationElapseTime = 0;
					pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
					pActionInfo->dwInterpolationTimeLength = 250;
				}else
				{
					sprintf_s(str,"%s%03s",strState[lState],strAttackAction[GetAttackAction()]);
				}
			}
			else
			{

				//��ƽ״̬
				if (GetState() == CShape::STATE_PEACE)
				{
					lState = 0;
				}
				else
				{
					//���û������(����)
					if (dwWeaponIndex == 0)
					{
						lState = 1;
					}
					else
					{
						lState = dwWeaponActType + 2;
					}
				}
				if (lAct == ACT_PICK||lAct == ACT_FALLDOWN||lAct == ACT_MEGRIM)
				{

					if(GetGame()->GetRegion()!=NULL&& GetGame()->GetRegion()->GetSpaceType() == RST_Warter)
					{
						if (lAct == ACT_MEGRIM)
						{
							sprintf_s(str,"%04s","YY00");
						}
						else
						{
							sprintf_s(str,"%04s",strAction[lAct]);
						}
					}
					else if (pPlayer->IsRide())
					{
						sprintf_s(str,"R%s%s%s",strActionHorse[lAct],strsingleperson[0],strhorsetype[0]);
					}
					else
					{
						sprintf_s(str,"%04s",strAction[lAct]);
					}

				}
				else
				{
					//��Ӿ�����˴���
					if(GetGame()->GetRegion()!=NULL&& (GetGame()->GetRegion()->GetSpaceType() == RST_Warter) )
					{

						sprintf_s(str,"Y%03s",strAction[lAct]);
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;


					}
					//���ζ���
					else if(!GetIsConceal())
					{

						if (lAct == ACT_PICK||lAct == ACT_FALLDOWN||lAct == ACT_MEGRIM)
						{
							sprintf_s(str,"%04s",strAction[lAct]);
						}
						else
						{
							sprintf_s(str,"N%03s",strAction[lAct]);
						}
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;
					}
					else
					{
						if (lAct == ACT_IDLE)
						{
							pActionInfo->bInterpolation = TRUE;
							pActionInfo->dwOldAction = pActionInfo->dwCurAction;
							pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
							pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
							pActionInfo->dwInterpolationElapseTime = 0;
							pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
							pActionInfo->dwInterpolationTimeLength = 240;
						}
						if (pPlayer->IsRide())
						{
							sprintf_s(str,"R%s%s%s",strActionHorse[lAct],strsingleperson[0],strhorsetype[0]);
						}
						else
						{
							sprintf_s(str,"%s%03s",strState[lState],strAction[lAct]);

						}

					}

				}
			}
			if (m_pDisplayModel&&m_pDisplayModel->GetLoadState() == Load_Did)
			{
				std::map<DWORD,GameModel::Action*>* mapaction = CGameModelManager::GetInstance()->GetGameModel(m_pDisplayModel->GetGameModelID())->GetActionArray();
				if (mapaction->find(MAKEFOURCC(str[0],str[1],str[2],str[3])) != mapaction->end())
				{
					pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);
				}
				else
				{
					pActionInfo->dwCurAction = MAKEFOURCC('S','S','0','0');
				}

			}
			else
			{
				pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);
			}

		}
		break;
		//����
	case	TYPE_PET:
		{	
			static DWORD dwSTD0 = MAKEFOURCC('S','T','D','0');		// ��ƽվ��
			static DWORD dwIDLE = MAKEFOURCC('I','D','L','E');		// ��ƽ����
			static DWORD dwSTD1 = MAKEFOURCC('S','T','D','1');		// ս��վ��
			static DWORD dwWLK0 = MAKEFOURCC('W','L','K','0');		// ��ƽ����
			static DWORD dwRUN  = MAKEFOURCC('W','L','K','1');		// �ܲ�
			static DWORD dwWLK1 = MAKEFOURCC('W','L','K','1');		// ս������
			static DWORD dwDIED = MAKEFOURCC('D','I','E','D');		// ����
			static DWORD dwDIE1 = MAKEFOURCC('D','I','E','1');		// ����
			static DWORD dwATK0 = MAKEFOURCC('A','T','K','0');		// ����1
			static DWORD dwATK1 = MAKEFOURCC('A','T','K','1');		// ����2
			static DWORD dwHURT = MAKEFOURCC('H','U','R','T');		// ����
			static DWORD dwMEGR = MAKEFOURCC('M','E','G','R');		// ѣ��
			static DWORD dwDOWN = MAKEFOURCC('D','O','W','N');		// ����
			static DWORD dwINTO = MAKEFOURCC('I','N','T','O');		// ����

			lState = GetState();
			switch (lAct)
			{
			case CShape::ACT_STAND:
				{
					if(lState == CShape::STATE_PEACE)
					{
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;
						pActionInfo->dwCurAction = dwSTD0;
					}
					else if(lState == CShape::STATE_FIGHT)
					{
						pActionInfo->bInterpolation = FALSE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 50;
						pActionInfo->dwCurAction = dwSTD1;
					}
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ��վ��������");
				}
				break;
			case CShape::ACT_IDLE:
				{
					if(lState == CShape::STATE_PEACE)
					{
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;
						pActionInfo->dwCurAction = dwIDLE;
					}
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ��IDLE������");
				}
				break;
			case CShape::ACT_WALK:
				{
					pActionInfo->bInterpolation = FALSE;
					pActionInfo->dwOldAction = pActionInfo->dwCurAction;
					pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
					pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
					pActionInfo->dwInterpolationElapseTime = 0;
					pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
					pActionInfo->dwInterpolationTimeLength = 240;
					if(lState == CShape::STATE_PEACE)
						pActionInfo->dwCurAction = dwWLK0;
					else if(lState == CShape::STATE_FIGHT)
						pActionInfo->dwCurAction = dwWLK1;
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ�����߶�����");
				}
				break;
			case CShape::ACT_RUN:
				{
					pActionInfo->bInterpolation = FALSE;
					pActionInfo->dwOldAction = pActionInfo->dwCurAction;
					pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
					pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
					pActionInfo->dwInterpolationElapseTime = 0;
					pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
					pActionInfo->dwInterpolationTimeLength = 240;
					pActionInfo->dwCurAction = dwRUN;
					break;
				}
			case CShape::ACT_DIED:
				pActionInfo->bInterpolation = TRUE;
				pActionInfo->dwOldAction = pActionInfo->dwCurAction;
				pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
				pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
				pActionInfo->dwInterpolationElapseTime = 0;
				pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
				pActionInfo->dwInterpolationTimeLength = 240;
				pActionInfo->dwCurAction = dwDIED;
				break;
			case CShape::ACT_ALREADYDIED:
				pActionInfo->dwCurAction = dwDIE1;
				break;
			case CShape::ACT_ATTACK:
				if (GetAttackAction()==0)
					pActionInfo->dwCurAction = dwATK0;
				else
					pActionInfo->dwCurAction = dwATK1;
				break;
			case CShape::ACT_BEATTACK:
				pActionInfo->dwCurAction = dwHURT;
				break;
			case CShape::ACT_MEGRIM:
				pActionInfo->dwCurAction = dwMEGR;	
				break;
			case CShape::ACT_FALLDOWN:
				pActionInfo->dwCurAction = dwDOWN;	
				break;
			default:	
				{	// ���ܶ���
					if(lAct >= ACT_PERFORM && lAct <= ACT_MAX_PERFORM)
					{
						// �������̬����
						if (lAct >= ACT_FACEACTION && lAct <= ACT_FACEACTIONMAX)
						{
							sprintf_s(str, "V%03d", lAct);
							pActionInfo->bInterpolation = TRUE;
							pActionInfo->dwOldAction = pActionInfo->dwCurAction;
							pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
							pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
							pActionInfo->dwInterpolationElapseTime = 0;
							pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
							pActionInfo->dwInterpolationTimeLength = 250;
							pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);	
						}
						else
						{
							sprintf(str, "S%03d", lAct - CShape::ACT_PERFORM);
							pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);	
						}

					}
					else
					{
						Log4c::Warn(ROOT_MODULE,"����Ķ�������:%d",lAct);
					}
				}
				break;
			}
			break;
		}
		// ���NPC
	case TYPE_MONSTER:
		//case TYPE_NPC:
		{
			static DWORD dwSTD0 = MAKEFOURCC('S','T','D','0');		// ��ƽվ��
			static DWORD dwIDLE = MAKEFOURCC('I','D','L','E');		// ��ƽ����
			static DWORD dwSTD1 = MAKEFOURCC('S','T','D','1');		// ս��վ��
			static DWORD dwWLK0 = MAKEFOURCC('W','L','K','0');		// ��ƽ����
			static DWORD dwWLK1 = MAKEFOURCC('W','L','K','1');		// ս������
			static DWORD dwDIED = MAKEFOURCC('D','I','E','D');		// ����
			static DWORD dwDIE1 = MAKEFOURCC('D','I','E','1');		// ����
			static DWORD dwATK0 = MAKEFOURCC('A','T','K','0');		// ����1
			static DWORD dwATK1 = MAKEFOURCC('A','T','K','1');		// ����2
			static DWORD dwHURT = MAKEFOURCC('H','U','R','T');		// ����
			static DWORD dwMEGR = MAKEFOURCC('M','E','G','R');		// ѣ��
			static DWORD dwDOWN = MAKEFOURCC('D','O','W','N');		// ����
			//static DWORD dwINTO = MAKEFOURCC('I','N','T','O');		// ����

			lState = GetState();
			switch (lAct)
			{
			case CShape::ACT_STAND:
				{
					if(lState == CShape::STATE_PEACE)
					{
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;
						pActionInfo->dwCurAction = dwSTD0;
					}
					else if(lState == CShape::STATE_FIGHT)
					{
						pActionInfo->bInterpolation = FALSE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 50;
						pActionInfo->dwCurAction = dwSTD1;
					}
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ��վ��������");
				}
				break;
			case CShape::ACT_IDLE:
				{
					if(lState == CShape::STATE_PEACE)
					{
						pActionInfo->bInterpolation = TRUE;
						pActionInfo->dwOldAction = pActionInfo->dwCurAction;
						pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
						pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
						pActionInfo->dwInterpolationElapseTime = 0;
						pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
						pActionInfo->dwInterpolationTimeLength = 240;
						pActionInfo->dwCurAction = dwIDLE;
					}
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ��IDLE������");
				}
				break;
			case CShape::ACT_WALK:
				{
					pActionInfo->bInterpolation = FALSE;
					pActionInfo->dwOldAction = pActionInfo->dwCurAction;
					pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
					pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
					pActionInfo->dwInterpolationElapseTime = 0;
					pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
					pActionInfo->dwInterpolationTimeLength = 240;
					if(lState == CShape::STATE_PEACE)
						pActionInfo->dwCurAction = dwWLK0;
					else if(lState == CShape::STATE_FIGHT)
						pActionInfo->dwCurAction = dwWLK1;
					else
						Log4c::Warn(ROOT_MODULE,"�����ڴ����״̬��ִ�����߶�����");
				}
				break;
			case CShape::ACT_DIED:
				pActionInfo->bInterpolation = TRUE;
				pActionInfo->dwOldAction = pActionInfo->dwCurAction;
				pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
				pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
				pActionInfo->dwInterpolationElapseTime = 0;
				pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
				pActionInfo->dwInterpolationTimeLength = 240;
				pActionInfo->dwCurAction = dwDIED;
				break;
			case CShape::ACT_ALREADYDIED:
				pActionInfo->dwCurAction = dwDIE1;
				break;
			case CShape::ACT_ATTACK:
				if (GetAttackAction()==0)
					pActionInfo->dwCurAction = dwATK0;
				else
					pActionInfo->dwCurAction = dwATK1;
				break;
			case CShape::ACT_BEATTACK:
				pActionInfo->dwCurAction = dwHURT;
				break;
			case CShape::ACT_MEGRIM:
				pActionInfo->dwCurAction = dwMEGR;	
				break;
			case CShape::ACT_FALLDOWN:
				pActionInfo->dwCurAction = dwDOWN;	
				break;
			default:	
				{	// ���ܶ���
					if(lAct >= ACT_PERFORM && lAct <= ACT_MAX_PERFORM)
					{
						// �������̬����
						if (lAct >= ACT_FACEACTION && lAct <= ACT_FACEACTIONMAX)
						{
							sprintf_s(str, "V%03d", lAct);
							pActionInfo->bInterpolation = TRUE;
							pActionInfo->dwOldAction = pActionInfo->dwCurAction;
							pActionInfo->dwOldActionTimeElapse = pActionInfo->dwCurActionElapseTime;
							pActionInfo->fOldActionSpeedRate = pActionInfo->fCurActionSpeedRate;
							pActionInfo->dwInterpolationElapseTime = 0;
							pActionInfo->dwInterpolationStartTime  = dwCurrentTime;
							pActionInfo->dwInterpolationTimeLength = 250;
							pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);	
						}
						else
						{
							sprintf(str, "S%03d", lAct - CShape::ACT_PERFORM);
							pActionInfo->dwCurAction = MAKEFOURCC(str[0],str[1],str[2],str[3]);	
						}

					}
					else
					{
						Log4c::Warn(ROOT_MODULE,"����Ķ�������:%d",lAct);
					}
				}
				break;
			}

			// ����ȱʡ����
		}
		break;
	case TYPE_BUILD:
	case TYPE_CITYGATE:
		{
			static DWORD dwSTND = MAKEFOURCC('S','T','N','D');
			static DWORD dwDIED = MAKEFOURCC('D','I','E','D');
			static DWORD dwOPEN = MAKEFOURCC('O','P','E','N');
			static DWORD dwHURT = MAKEFOURCC('H','U','R','T');

			switch (lAct)
			{
			case CShape::ACT_STAND:
				pActionInfo->dwCurAction = dwSTND;
				break;

			case CShape::ACT_DIED:
				pActionInfo->dwCurAction = dwDIED;
				break;

			case CShape::ACT_BEATTACK:
				pActionInfo->dwCurAction = dwHURT;
				break;

			case CShape::ACT_OPEN:
				pActionInfo->dwCurAction = dwOPEN;
				break;
			}

			// ����ȱʡ����

		}
		break;
		// ��Ʒ
	case TYPE_EFFECT:
	case TYPE_SKILL:
		{
			pActionInfo->dwCurAction = MAKEFOURCC('S', 'T', 'N', 'D');
		}
		break;
		// ���壺�ɼ���
	case TYPE_COLLECTION:
		{
			pActionInfo->dwCurAction = CShape::ACT_ALREADYDIED == lAct ? MAKEFOURCC('D','I','E','1') : MAKEFOURCC('D','I','E','D');
		}
		break;
	}
}



/*
* ����: MoveShape���˹�����
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.12.01 - lpf
* 		������"��Ѫ���ֵ���ɫ�͸߶ȿ���"������еĹ�����Ѫ�ƶ��Ĳ���,ʹ֮����ȥЧ���ĸ���г;
*	2008.12.03 - lpf
* 		������"��Ѫ���ֵ���ɫ�͸߶ȿ���",Ŀǰ�ַ��Ŵ�����Ժ�,���ٽ�����С��ʾ,��ֱ�ӽ���,������������ӵ����Ų���;
*	2008.12.12 - lpf
* 		������"��Ѫ���ֵ��ٶ�"������0.01;
*/
void CMoveShape::AI()
{
	CShape::AI();

	// ˵����������ɫ����
	if (m_strTalkText.size())
	{
		m_lTalkLiveTimer -= 1;
		if (m_lTalkLiveTimer <= 0)
		{
			m_strTalkText = "";
		}
	}

	// ��Ѫ���ֵ���ɫ�͸߶ȿ���
	DWORD dwCurTime = timeGetTime();	
	static const DWORD dwBloodTextTimeLength = 5000;
	for(list<tagBloodText>::iterator it=m_listBloodText.begin(); it!=m_listBloodText.end();)
	{
		tagBloodText *pst = &(*it);
		if (pst)
		{
			DWORD dwTimeElapse = dwCurTime - pst->dwStartTime;
			DWORD dwSliceIndex = 3;
			if ( dwTimeElapse < 50)
			{
				dwSliceIndex = 0;
			}else
			{
				if (dwTimeElapse < 120)
				{
					dwSliceIndex = 1;
				}else
				{
					if (dwTimeElapse < 300)
					{
						dwSliceIndex = 2;
					}
				}
			}

			switch(dwSliceIndex)
			{
			case 0: 
				{
					float fRatio = (float)dwTimeElapse / 50.0f;
					pst->lAlpha = 128 + (int)(128.0f * fRatio);
					pst->fCurScale = pst->fScale * (0.5f + 0.5f * fRatio);
				}
				break;
			case 1: 
				pst->fCurScale = pst->fScale;
				pst->fCurScale2 = pst->fScale;
				pst->lAlpha = 255;
				break;
			case 2: 
				{
					float fRatio = (float)(dwTimeElapse - 120) / 120.0f;
					//pst->fCurScale = (pst->fScale + 1.0f) * (1.0f - (fRatio) * 0.5f);
					//pst->lAlpha = 192 - (int)(64.0f * fRatio);

					pst->lAlpha -= 5;
					if (pst->lAlpha <0 )
						pst->lAlpha = 0;

					pst->fPos		+= 0.05f;
					pst->fCurScale2 += 0.008f;
				}
				break;
			default: // 400 - ?
				/*pst->fCurScale = pst->fScale * 0.5f*/;

				pst->lAlpha -= 5;
				if (pst->lAlpha <0 )
					pst->lAlpha = 0;

				pst->fPos += 0.06f;
				pst->fCurScale2 += 0.008f;
				break;
			}

			// ����ɾ��
			if (dwTimeElapse > dwBloodTextTimeLength)
			{
				m_listBloodText.erase(it++);
			}
			else
			{
				++it;
			}
		}
		else
			m_listBloodText.erase(it++);
	}

	//// Ӳֱ
	//if(m_dwStiffStartTime)
	//{
	//	if(GetCurTickCount()-m_dwStiffStartTime<500)
	//	{
	//		return;
	//	}
	//	m_dwStiffStartTime=0;
	//}

	if (GetType()!=TYPE_MONSTER && 
		GetType()!=TYPE_PLAYER &&
		/*GetType()!=TYPE_NPC &&*/ GetType() != TYPE_PET &&
		GetType()!=TYPE_SUMMON)
	{
		return;
	}

	AI_StateEx();

	switch( GetAction() )
	{
	case ACT_STAND:
	case ACT_IDLE:
		AI_Stand();
		break;

	case ACT_WALK:
		AI_Walk();
		break;

	case ACT_RUN:
		AI_Run();
		break;

	case ACT_ATTACK:
		AI_Attack();
		break;

	case ACT_BEATTACK:
		AI_BeAttack();
		break;

	case ACT_DIED:
		AI_Died();
		break;

	case ACT_BLOCK:				// ��
	case ACT_PARRY:				// �м�
	case ACT_MISS:				// ����
	case ACT_MEGRIM:			// ѣ��
		AI_Perform();
		break;

	default:
		if (GetAction()>=ACT_PERFORM)
			AI_Perform();
		else
			OnAttackEvent();
		break;
	}
	AI_SlipMove();
	AI_Skill();
	AIEffectByOrder();
}

void CMoveShape::DecordChangeProperty(BYTE* pByte,long& lPos)
{
	WORD lNum = _GetWordFromByteArray(pByte,lPos);
	WORD pro_type = 0;
	BYTE data_type = 0;
	WORD data_length = 0;
	long data = 0;
	BYTE* byte_Array = NULL;
	WORD* word_Array = NULL;
	DWORD* dword_Array = NULL;
	float* flaot_Array = NULL;
	long*  long_Array = NULL;
	short* short_Array = NULL;
	char*  char_Array = NULL;
	bool*  bool_Array = NULL;

	for (int i = 0;i<lNum;i++)
	{
		pro_type = _GetWordFromByteArray(pByte,lPos);
		data_type = _GetByteFromByteArray(pByte, lPos);
		data_length = _GetWordFromByteArray(pByte,lPos);		
		if (data_length<=0) continue;

		//�����������ͻ�ȡ���������ֵ
		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = _GetByteFromByteArray(pByte, lPos);
			break;
		case ATTR_DATA_WORD:
			data = _GetWordFromByteArray(pByte,lPos);
			break;
		case ATTR_DATA_DWORD:
			data = _GetDwordFromByteArray(pByte,lPos);
			break;
		case ATTR_DATA_FLOAT:
			data = (long)_GetFloatFromByteArray(pByte,lPos);
			break;
		case ATTR_DATA_LONG:
			data = _GetLongFromByteArray(pByte,lPos);
			break;
		case ATTR_DATA_SHORT:
			data = _GetShortFromByteArray(pByte,lPos);
			break;
		case ATTR_DATA_BOOL:
			data = _GetByteFromByteArray(pByte, lPos);
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				byte_Array[i] = _GetByteFromByteArray(pByte, lPos);
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				word_Array[i] = _GetWordFromByteArray(pByte, lPos);
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				dword_Array[i] = _GetDwordFromByteArray(pByte,lPos);
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			flaot_Array = new float[data_length];
			if (!flaot_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				flaot_Array[i] = _GetFloatFromByteArray(pByte,lPos);
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				long_Array[i] = _GetLongFromByteArray(pByte,lPos);
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				short_Array[i] = _GetShortFromByteArray(pByte,lPos);
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			_GetStringFromByteArray(pByte, lPos, char_Array);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				data = _GetByteFromByteArray(pByte, lPos);
				if (data!=0)
				{
					bool_Array[i] = true;
				}else
					bool_Array[i] = false;
			}
			break;
		default:
			break;
		}						

		//������Ӧ����ֵ
		itProETN it = m_PropertyEnumToName.find(pro_type);
		if(it != m_PropertyEnumToName.end())
		{
			SetProperty( (*it).second.c_str(),data);
		}

		SAFEDELETE(byte_Array);
		SAFEDELETE(word_Array);
		SAFEDELETE(dword_Array);
		SAFEDELETE(flaot_Array);
		SAFEDELETE(long_Array);
		SAFEDELETE(short_Array);
		SAFEDELETE(char_Array);
		SAFEDELETE(bool_Array);
	}
}


void CMoveShape::DecodeChangePropertyFromDataBlock(DBReadSet& setReadDB)
{
	WORD lNum = setReadDB.GetWordFromByteArray();
	WORD pro_type = 0;
	BYTE data_type = 0;
	WORD data_length = 0;
	long data = 0;
	BYTE* byte_Array = NULL;
	WORD* word_Array = NULL;
	DWORD* dword_Array = NULL;
	float* flaot_Array = NULL;
	long*  long_Array = NULL;
	short* short_Array = NULL;
	char*  char_Array = NULL;
	bool*  bool_Array = NULL;

	for (int i = 0;i<lNum;i++)
	{
		pro_type = setReadDB.GetWordFromByteArray();
		data_type = setReadDB.GetByteFromByteArray();
		data_length = setReadDB.GetWordFromByteArray();		
		if (data_length<=0) continue;

		//�����������ͻ�ȡ���������ֵ
		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = setReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_WORD:
			data = setReadDB.GetWordFromByteArray();
			break;
		case ATTR_DATA_DWORD:
			data = setReadDB.GetDwordFromByteArray();
			break;
		case ATTR_DATA_FLOAT:
			data = (long)setReadDB.GetFloatFromByteArray();
			break;
		case ATTR_DATA_LONG:
			data = setReadDB.GetLongFromByteArray();
			break;
		case ATTR_DATA_SHORT:
			data = setReadDB.GetShortFromByteArray();
			break;
		case ATTR_DATA_BOOL:
			data = setReadDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				byte_Array[i] = setReadDB.GetByteFromByteArray();
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				word_Array[i] = setReadDB.GetWordFromByteArray();
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				dword_Array[i] = setReadDB.GetDwordFromByteArray();
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			flaot_Array = new float[data_length];
			if (!flaot_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				flaot_Array[i] = setReadDB.GetFloatFromByteArray();
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				long_Array[i] = setReadDB.GetLongFromByteArray();
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				short_Array[i] = setReadDB.GetShortFromByteArray();
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			setReadDB.GetStringFromByteArray(char_Array,data_length);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				data = setReadDB.GetByteFromByteArray();
				if (data!=0)
				{
					bool_Array[i] = true;
				}else
					bool_Array[i] = false;
			}
			break;
		default:
			break;
		}						

		//������Ӧ����ֵ
		itProETN it = m_PropertyEnumToName.find(pro_type);
		if(it != m_PropertyEnumToName.end())
		{
			SetProperty( (*it).second.c_str(),data);
		}

		SAFEDELETE(byte_Array);
		SAFEDELETE(word_Array);
		SAFEDELETE(dword_Array);
		SAFEDELETE(flaot_Array);
		SAFEDELETE(long_Array);
		SAFEDELETE(short_Array);
		SAFEDELETE(char_Array);
		SAFEDELETE(bool_Array);
	}
}

//����MoveShape������
void CMoveShape::UpdateProperty(CMessage* pMsg)
{
	DBReadSet setReadDB;
	pMsg->GetDBReadSet(setReadDB);
	CMoveShape::DecodeChangePropertyFromDataBlock(setReadDB);
}

// վAI
long CMoveShape::AI_Stand()
{
	if( OnAttackEvent() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}

	// ��ƽ״̬��IDLE(�����NPCֻ����һ��״̬)
	if (GetState()==STATE_PEACE)
	{
		if (m_bIdleStart)
		{
			//�жϲ��Ž���
			AnimInfo::tagActionInfo *pActionInfo = m_pAnimInfo->GetActionInfo();
			m_bDisplayFinish =  (pActionInfo->dwCurActionLoopCount > 0);
			if (m_bDisplayFinish)
			{
				SetAction(ACT_IDLE);
				m_lIdleCount = 0;
				m_bIdleStart = false;
			}
		}
		else
		{
			m_lIdleCount++;
			if (m_lIdleCount>s_lIdleMinCount && random(s_lIdleMaxCount) < m_lIdleCount-s_lIdleMinCount)
			{
				m_bIdleStart = true;
				m_lIdleCount = 0;
			}

		}
	}

	return RET_AI_CONTINUE;
}

// ��AI
long CMoveShape::AI_Walk()
{
	if( OnAttackEvent() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}

	if( MoveAI() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}
	return RET_AI_CONTINUE;
}

// ��AI
long CMoveShape::AI_Run()
{
	if( OnAttackEvent() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}

	if( MoveAI() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}
	return RET_AI_CONTINUE;
}

// ����AI
long CMoveShape::AI_Attack()
{
	//	GetGame()->SetOnAttacted(false);

	if (OnAttackEvent()==RET_AI_BREAK)
	{
		return RET_AI_BREAK;
	}

	return RET_AI_CONTINUE;
}

// ʩ��AI
long CMoveShape::AI_Perform()
{
	if (OnAttackEvent()==RET_AI_BREAK)
	{
		return RET_AI_BREAK;
	}

	return RET_AI_CONTINUE;
}

// ������AI
long CMoveShape::AI_BeAttack()
{
	if( OnAttackEvent() == RET_AI_BREAK )
	{
		return RET_AI_BREAK;
	}

	//	GetGame()->SetOnAttacted(false);
	return RET_AI_CONTINUE;
}

// ����AI
long CMoveShape::AI_Died()
{
	//�жϲ��Ž���
	AnimInfo::tagActionInfo *pActionInfo = m_pAnimInfo->GetActionInfo();
	if (pActionInfo->dwCurActionLoopCount > 0)
	{
		SetAction(ACT_ALREADYDIED);
	}
	return RET_AI_CONTINUE;
}



/*
* ����: �쳣״̬��AI
* ժҪ: -
* ����: -
* ����ֵ: AI����ִ��״̬
* ����: -
* ��������: -
* �޸���־:
*	2008.01.14 - lpf
*		�ڸú����������˶�ģ�ͱ�ɫʱ�Ľ�������Ч��
*	2008.04.04 - lpf
*		�������쳣״̬�о���������ʾ����ɫ(Ŀǰֻ������)���ᱻ�������쳣״̬��ɫ���滻��.
*	2008.07.07 - lpf
*		�ж������쳣״̬����ʱ,��Ҫǿ���趨ģ����ɫǰ��ģ���Ƿ�Ϊ�ս������ж�.
*	2008.10.20 - lpf
*		ʹ�����µĶ����쳣״̬����,����ʾ�쳣״̬�µĶ�����ɫ.
*	2008.10.22 - lpf
*		��"�Ƿ�����ɫ����Ч��"ʱ,�Ե�ǰ��ʾ�Ľ���ģ��ID�������ж�;
*	2008.11.17 - ����
*		�޸�״̬����Ч����ʽ��ÿ���׶�һ����Ч�б�
*	2009.05.15 - lpf
*		��"��������������״̬,ȡ����ѡ��״̬"������������Ƿ���з����������ж�;
*	2009.07.17 - lpf
*		"�Ƿ�����ɫ����Ч��"�о�̬����iLastColorLevel�ᵼ�²�ͬʵ����MOVESHOP������ɫ��ʾȴ��ͬ���Ĵ�������;
*/
long CMoveShape::AI_StateEx()
{	
	list<tagStateEx>::iterator it;

	for (it = m_listStateEx.begin(); it != m_listStateEx.end();)
	{
		tagStateEx * pStateEx = &(*it);

		// �쳣״̬ʱ�����
		if (pStateEx->dwStandingTime != 0)
		{
			DWORD dwCurTickCount = GetCurTickCount();

			if (pStateEx->dwStartTime + pStateEx->dwStandingTime < dwCurTickCount)
				pStateEx->bEndSelf = true;

			// ���ڷ�����״̬
			if (GetIsConceal())
			{
				if (m_bStateColorPlay &&
					pStateEx->dwStartTime + pStateEx->dwStandingTime - 2000 < dwCurTickCount)
					m_bColorFadeFlag = false;
			}
		}

		CStateListXml::tagStateList * stStateList = CStateListXml::GetState(pStateEx->dwID);
		if(stStateList)
		{
			if (pStateEx->bEndSelf)			//�����쳣״̬����
			{
				DWORD dwEffect = 0;
				DWORD dwEffectIndex = 0;
				DWORD dwLocator = 0;
				bool  bIsOrderPlay = false;
				vector<CStateListXml::tagStateEffect> vecStateEffect = stStateList->vecStateLevel[(pStateEx->dwLevel) - 1].vecMidEffect;
				// ɾ���м�׶���Ч
				for (size_t i=0; i<vecStateEffect.size();++i)
				{
					// ������״̬��ָ���ȼ�����Ч��Ϣ
					dwEffect		= vecStateEffect[i].dwEffectID;
					dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
					bIsOrderPlay	= vecStateEffect[i].bIsOrderPlay;
					// �ӱ���������ɾ�����е���Ч
					if(!bIsOrderPlay)
						DelEffect(dwEffect,dwEffectIndex);
				}
				DelAllEffectByOrder();
				// ��ӽ����׶���Ч
				vecStateEffect = stStateList->vecStateLevel[(pStateEx->dwLevel) - 1].vecEndEffect;
				for (size_t i=0; i<vecStateEffect.size();++i)
				{
					// ������״̬��ָ���ȼ�����Ч��Ϣ
					dwEffect		= vecStateEffect[i].dwEffectID;
					dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
					dwLocator		= vecStateEffect[i].dwEffectPosition;		//ʩ��λ��
					eLOCATOR Loc;
					switch(dwLocator)
					{
					case 0: Loc = LOCATOR_WIND; break;
					case 1: Loc = LOCATOR_HEAD; break;
					case 2: Loc = LOCATOR_FOOTCENTER; break;
					case 3: Loc = LOCATOR_LEFTHAND; break;
					case 4: Loc = LOCATOR_RIGHTHAND; break;
					case 5: Loc = LOCATOR_LEFTFOOT; break;
					case 6: Loc = LOCATOR_RIGHTFOOT; break;
					default: Loc = LOCATOR_FOOTCENTER; break;
					}
					// ������״̬�Ž��������Ž���ʱ����Ч
					if (GetIsConceal())
					{
						if(dwEffect)
							AddEffectLocator(dwEffect, this, Loc, false, 0.0f, dwEffectIndex);
					}
				}
			}
			else
			{
				DWORD dwEffect = 0;
				DWORD dwEffectIndex = 0;
				DWORD dwLocator = 0;
				DWORD dwCycle = 0;
				bool  bIsOrderPlay = false;			// �Ƿ���˳�򲥷ŵ���Ч
				bool  bBeginEffectFinish = true;	// ��ʼ�׶ε�������Ч�Ƿ񲥷����
				bool  bMidEffectFinish = true;		// �м�׶ε�������Ч�Ƿ񲥷����
				vector<CStateListXml::tagStateEffect> vecStateEffect = stStateList->vecStateLevel[(pStateEx->dwLevel) - 1].vecBeginEffect;
				// ��ʼ�׶���Ч
				for (size_t i=0; i<vecStateEffect.size();++i)
				{
					// ������״̬��ָ���ȼ�����Ч��Ϣ
					dwEffect		= vecStateEffect[i].dwEffectID;
					dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
					// ����״̬
					if(!GetIsConceal())
						DelEffect(dwEffect, dwEffectIndex);
					CEffect * pEffect = FindEffect(dwEffect, dwEffectIndex);
					if(pEffect)
					{
						bBeginEffectFinish = false;
						break;
					}
				}
				// ����״̬
				if (!GetIsConceal())
					m_bStateColorPlay = TRUE;
				// �м�׶���Ч
				vecStateEffect = stStateList->vecStateLevel[(pStateEx->dwLevel) - 1].vecMidEffect;
				for (size_t i=0; i<vecStateEffect.size();++i)
				{
					// ������״̬��ָ���ȼ�����Ч��Ϣ
					dwEffect		= vecStateEffect[i].dwEffectID;
					dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
					dwLocator		= vecStateEffect[i].dwEffectPosition;		//ʩ��λ��
					dwCycle			= vecStateEffect[i].dwEffectCycle;			//�Ƿ�ѭ��
					bIsOrderPlay	= vecStateEffect[i].bIsOrderPlay;			// �Ƿ�˳�򲥷�
					bool bCycle		= ((dwCycle == 0) ? false : true);
					eLOCATOR Loc;
					switch(dwLocator)
					{
					case 0: Loc = LOCATOR_WIND; break;
					case 1: Loc = LOCATOR_HEAD; break;
					case 2: Loc = LOCATOR_FOOTCENTER; break;
					case 3: Loc = LOCATOR_LEFTHAND; break;
					case 4: Loc = LOCATOR_RIGHTHAND; break;
					case 5: Loc = LOCATOR_LEFTFOOT; break;
					case 6: Loc = LOCATOR_RIGHTFOOT; break;
					default: Loc = LOCATOR_FOOTCENTER; break;
					}
					// ��ʼ�׶ε���Чȫ���ͷ���ϣ�������û����ӹ��м�׶ε���Ч
					CEffect * pEffect = NULL;
					// ����˳�򲥷���Ч�����������
					if(!bIsOrderPlay)
						pEffect = FindEffect(dwEffect, dwEffectIndex);
					else
						pEffect = FindEffectByOrder(dwEffect, dwEffectIndex);
					if(bBeginEffectFinish && pEffect == NULL)
					{
						// �����ǰ����û������״̬�����״̬����Ч
						if(GetIsConceal())
						{
							if(!bIsOrderPlay)// ���ǰ�˳�򲥷ŵ���Ч
								AddEffectLocator(dwEffect, this, Loc, bCycle, 0.0f, dwEffectIndex);
							else
								AddEffectToList(dwEffect, this, Loc, bCycle, 0.0f, dwEffectIndex);
						}
						else	// ������ʾ״̬��Ч
						{
							if(!bIsOrderPlay)// ���ǰ�˳�򲥷ŵ���Ч
								DelEffect(dwEffect, dwEffectIndex);
							else
								DelEffectByOrder(dwEffect, dwEffectIndex);
						}
					}
					else
					{
						long lType, lID;
						GetCurGraphicTypeID(lType, lID);
						if (pEffect && pEffect->GetHostGraphicsID() != lID)		// �жϽ�ɫģ���Ƿ�ı�
						{
							if(!bIsOrderPlay) // ���ǰ�˳�򲥷ŵ���Ч
							{
								DelEffect(dwEffect, dwEffectIndex);									// ɾ����ɫģ�͸ı�ǰ����Ч
								AddEffectLocator(dwEffect, this, Loc, bCycle, 0.0f, dwEffectIndex);	// ������ģ�͵���Ч
							}
							else
							{
								DelEffectByOrder(dwEffect, dwEffectIndex);
								AddEffectToList(dwEffect, this, Loc, bCycle, 0.0f, dwEffectIndex);
							}
						}
					}
				}
			}
		}

		if (pStateEx->bEndSelf)
		{
			if (m_bStateColorPlay && m_CurrentStateExForStateColor.dwID == pStateEx->dwID)
			{
				bool bIsEnd = true;
				for (list<tagStateEx>::iterator it2 = m_listStateEx.begin(); it2 != m_listStateEx.end(); ++it2)
				{
					tagStateEx & pStateEx2 = (*it2);
					if (pStateEx2.dwID == m_CurrentStateExForStateColor.dwID && pStateEx2.dwLevel == m_CurrentStateExForStateColor.dwLevel &&
						!pStateEx2.bEndSelf)
					{
						bIsEnd = false;
					}
				}

				if (bIsEnd)
				{
					ZeroMemory(&m_CurrentStateExForStateColor, sizeof(tagStateEx));
					m_bStateColorPlay = FALSE;
					m_bColorFade = false;
					m_bColorFadeFlag	= true;
					m_fColorFadePer		= 1.0f;
					m_colorModel		= s_colorModelDefault;

					if (m_pDisplayModel != NULL)
						m_pDisplayModel->SetModelColorCompel(m_colorModel);
				}
			}

			// ��ǰ�Ķ������ڸ�״̬��Ӧ�Ķ�������״̬����ʱ�����ö���Ϊվ��
			CStateListXml::tagStateList *pSL = CStateListXml::GetState(pStateEx->dwID);
			if(pSL)
			{
				if(pSL->wAction == GetAction())
					SetAction(ACT_STAND);
			}
			it = m_listStateEx.erase(it++);	// ɾ������쳣״̬

			if (m_CurrentStateExForStateColor.dwID == 0)
			{
				for (list<tagStateEx>::iterator it3 = m_listStateEx.begin(); it3 != m_listStateEx.end(); ++it3)
				{
					tagStateEx & pStateEx3 = (*it3);

					CStateListXml::tagStateList * pSL = CStateListXml::GetState(pStateEx3.dwID);
					if (pSL != NULL)
					{
						BOOL bStateColorPlay = (BOOL)pSL->vecStateLevel[(pStateEx3.dwLevel) - 1].vecBuffColor.size();
						if (bStateColorPlay)
						{
							m_CurrentStateExForStateColor.dwID = pStateEx3.dwID;
							m_CurrentStateExForStateColor.dwLevel = pStateEx3.dwLevel;
							m_CurrentStateExForStateColor.dwStartTime = GetCurTickCount();
							m_CurrentStateExForStateColor.dwStandingTime = pStateEx3.dwStandingTime;
							m_CurrentStateExForStateColor.bEndSelf = false;
							m_bStateColorPlay = TRUE;
							m_bColorFade	  = pSL->vecStateLevel[(pStateEx3.dwLevel) - 1].bColorFade;					//�Ƿ��쳣״̬��ɫ��������
						}
					}
				}
			}
		}else
			++it;
	}

	// �Ƿ�����ɫ����Ч��
	if (m_bStateColorPlay && m_CurrentStateExForStateColor.dwID != 0)
	{
		// ��¼���һ�θ���ʱ��
		static DWORD dwLastTime = timeGetTime();
		CStateListXml::tagStateList * pStateListForColor = CStateListXml::GetState(m_CurrentStateExForStateColor.dwID);
		if (pStateListForColor != NULL)
		{
			DWORD dwInterval = pStateListForColor->vecStateLevel[(m_CurrentStateExForStateColor.dwLevel) - 1].dwInterval;					//ģ����ɫ�仯���ʱ��

			// ÿ��һ�μ��ʱ��仯һ����ɫ
			if (timeGetTime() - dwLastTime > dwInterval)
			{
				if (m_iLastColorLevel < (int)pStateListForColor->vecStateLevel[(m_CurrentStateExForStateColor.dwLevel) - 1].vecBuffColor.size())
				{
					m_colorModel = pStateListForColor->vecStateLevel[(m_CurrentStateExForStateColor.dwLevel) - 1].vecBuffColor[m_iLastColorLevel];
					++m_iLastColorLevel;
					dwLastTime = timeGetTime();
				}else
					m_iLastColorLevel = 0;
			}
		}
	}

	//����״̬
	if (IsInDieAwayState())
		SetAction(ACT_DIED);
	// ��������������״̬,ȡ����ѡ��״̬
	if(!GetIsConceal() && this != GetGame()->GetMainPlayer() &&
		GetGame()->GetMainPlayer()->GetIsSpy())
	{
		// �Ƕ���
		if(!GetGame()->GetMainPlayer()->IsTeammate(GetExID()))
		{
			if(this == GetGame()->GetGameControl()->GetSelectedShape())
				GetGame()->GetGameControl()->SetSelectedShape(NULL);
			if(this == GetGame()->GetGameControl()->GetCurAttractShape())
				GetGame()->GetGameControl()->SetCurAttractShape(NULL);
            if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
            {
                FireUIEvent("ObjectPage", "Closed");
            }
		}
	}

	// //����״̬
	//if(IsHaveStateEx(82)&& GetState() == STATE_DIED )
	//	SetAction(ACT_STAND);

	return RET_AI_CONTINUE;
}

// ƽ����λAI
void CMoveShape::AI_SlipMove()
{
	// �ﵽ��ƽ��Ŀ�ĵ�
	if(GetIsSlipState() && GetCurTickCount() >= GetSlipEndMoveTime())
	{
		//�������
		if(GetTileX() != (long)GetSlipDestPosX() || GetTileY() != (long)GetSlipDestPosY() )
			SetPosXY(GetSlipDestPosX(), GetSlipDestPosY());
		SetIsSlipState(false);
	}
}

// ����AI
void CMoveShape::AI_Skill()
{
	if(m_mapCurSkill.size()>0)
	{
		vector<long> vecDelSkill;
		map<long, CSkillXml*>::iterator it;
		for (it = m_mapCurSkill.begin(); it != m_mapCurSkill.end(); it++)
		{
			if(it->second->GetIsDeleted())
				vecDelSkill.push_back(it->first);
			else
			{
				it->second->AI();
				if(it->second->GetIsDeleted())
					vecDelSkill.push_back(it->first);
			}
		}
		if(vecDelSkill.size()>0)
		{
			for (vector<long>::iterator vit = vecDelSkill.begin(); vit != vecDelSkill.end(); vit++)
			{
				DeleteCurSkill((*vit));
			}
			vecDelSkill.clear();
		}
	}
}
// �Ƿ���ĳ��״̬
bool CMoveShape::IsHaveStateEx(DWORD dwID)
{
	list<tagStateEx>::iterator it;
	for(it = m_listStateEx.begin(); it != m_listStateEx.end();it++)
	{
		tagStateEx* pStateEx = &(*it);
		if (pStateEx && pStateEx->dwID == dwID)
			return true;
	}

	return false;
}
// �Ƿ�������״̬
//bool CMoveShape::IsInHideState()
//{
//	if(IsHaveStateEx(55))
//		return true;
//	else 
//		return false; 
//}
// �Ƿ��ڼ���״̬
bool CMoveShape::IsInDieAwayState()
{
	if (IsHaveStateEx(81))
		return true;
	else 
		return false;
}



/*
* ����: MoveShape����ʾ
* ժҪ: �ú���ִ��ģ���������ʾ,�������˹���Ч��
* ����: -
* ����ֵ: ִ�гɹ�����ture,���򷵻�false
* ����: -
* ��������: -
* �޸���־:
*	2008.01.10 - lpf
*		�ڸú����������˶�ģ�͸�����Ч����Ⱦ(������,��)
*	2008.01.16 - lpf
*		�����˴����쳣״̬����ɫ��������
*	2008.03.10 - lpf
*		�����˴����쳣״̬����ɫ�����͸��ģ�͵Ĺ���
*	2008.03.11 - lpf
*		�ڴ����쳣״̬����ɫ,������m_dwCurrentModelCol��Alphaͨ��ֵ���޸�,��Ϊ��ֵ�������ֳ�������Ⱦʱ���õ�
*	2008.09.10 - lpf
*		�ڴ����쳣״̬����ɫʱ,�����˶��Ƿ���ɫ����Ŀ���
*	2008.09.18 - lpf
*		�ָ���9.10���޸�
*	2008.10.17 - lpf
*		�����˶���Ч�����������Ĵ���
*	2009.05.22 - lpf
*		����ȡ�������趨֮��,û�д����쳣״̬��ɫ��BUG,ʹ�������޷���ɽ�������
*	2009.07.17 - lpf
*		�������ڴ����쳣״̬��ɫ�Ľ���ʱ,����ֲ�����������ݵ����,֮ǰ��ɵ�˲�����Ӧ�þ����������
*/
bool CMoveShape::Display()
{
	// ��������
	static D3DXVECTOR3 vLightVector(-0.613793f, -0.789465f, 0.001831f);

	CShape::Display();

	// �жϺ���ִ������
	if (!IsNeedDisplay() ||
		m_pDisplayModel == NULL ||
		m_pDisplayModel->GetLoadState() != Load_Did)
		return false;

	// �õ����
	CClientRegion * pRegion = GetGame()->GetRegion();
	if(pRegion == NULL)
		return false;

	render::Camera * pCamera = pRegion->GetCamera();

	// ����ģ����Ч��
	if (m_listAddEffect[ADDEFFECT_SHAKE].bPlayState)
	{
		if (timeGetTime() - m_listAddEffect[ADDEFFECT_SHAKE].dwStartTime >= m_listAddEffect[ADDEFFECT_SHAKE].dwTimeLength)
		{
			m_fXIncrement								= 0.0f;
			m_fYIncrement								= 0.0f;
			m_bRestoration								= false;
			m_listAddEffect[ADDEFFECT_SHAKE].bPlayState = false;
		}

		if (m_bRestoration)
		{
			float fExtent  = (float)m_listAddEffect[ADDEFFECT_SHAKE].dwValue * 0.01f;
			m_fXIncrement  = (float)rand() / RAND_MAX * fExtent + m_fShakeMin;
			m_fYIncrement  = (float)rand() / RAND_MAX * fExtent + m_fShakeMin;
			m_bRestoration = false;
		}
		else
		{
			m_fXIncrement  *= -1.0f;
			m_fYIncrement  *= -1.0f;
			m_bRestoration = true;
		}
	}
	/*D3DXMATRIX mats;
	D3DXMatrixScaling(&mats,9,9,9);
	m_pAnimInfo->SetScalMatrix(&mats);*/

	// ����ģ���������
	m_pAnimInfo->SetupAnimInfo(GetPosX() + m_fXIncrement, GetPosY() + m_fYIncrement, GetHeight(), GetNowDir(), pCamera);

	//m_pDisplayModel->SetRegionAmbient(GetGame()->GetRegion()->GetGameMap()->GetSunLightAmbientColorforBuild());

	if (m_bStateColorPlay)			//�����쳣״̬����ɫ��������
	{
		// AddBy--- ��ǫ----����ɫ��RGB��ΪFF��ʱ��,����͸��Ч��,����Ϊ��ɫ�任Ч�� ,��ɫʱ�رչ���
		DWORD dwColor;
		bool  bIsColorFade = false;

		/*if (GetGame()->GetSetup()->lLight)
		{*/
		//m_pDisplayModel->SetModelColor(0xFFcfcfcf);
		//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(),(D3DXVECTOR3*)&vLightVector,0xFF808080);
		GameMap *pGameMap = pRegion->GetGameMap();
		dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());

		int iMA, iMR, iMG, iMB;
		DWORD dw(m_colorModel);
		if (m_colorModel != s_colorModelDefault)
		{
			bIsColorFade = true;

			//��ɫ
			if ((dw & 0x00ffffff) != 0x00ffffff)
			{
				m_pDisplayModel->SetDirLightEnable(false);
				m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);

				// �����Ƿ񽥱�
				if (m_bColorFade)
				{
					int iR = (dw & 0x00ff0000) >> 16;
					int iG = (dw & 0x0000ff00) >> 8;
					int iB = (dw & 0x000000ff);

					int SR = (dwColor & 0x00ff0000) >> 16;
					int SG = (dwColor & 0x0000ff00) >> 8;
					int SB = (dwColor & 0x000000ff);

					// ������ɫ����������
					if (m_bColorFadeFlag)
					{
						iMR = (int)((float)SR * m_fColorFadePer);
						iMG = (int)((float)SG * m_fColorFadePer);
						iMB = (int)((float)SB * m_fColorFadePer);
						m_fColorFadePer -= 0.01f;

						if (iMR < iR) iMR = iR;
						if (iMG < iG) iMG = iG;
						if (iMB < iB) iMB = iB;
						if (m_fColorFadePer < 0.0f) m_fColorFadePer = 0.0f;
					}
					else
					{
						iMR = iR + (int)((float)SR * m_fColorFadePer);
						iMG = iG + (int)((float)SR * m_fColorFadePer);
						iMB = iB + (int)((float)SR * m_fColorFadePer);
						m_fColorFadePer += 0.01f;

						if (iMR > 0xff) iMR = 0x000000ff;
						if (iMG > 0xff)	iMG = 0x000000ff;
						if (iMB > 0xff)	iMB = 0x000000ff;
						if (m_fColorFadePer > 1.0f) m_fColorFadePer = 1.0f;
					}
					m_dwCurrentModelCol = (0xff << 24 | (DWORD)iMR << 16 | (DWORD)iMG << 8 | (DWORD)iMB);
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
				}else
				{
					m_dwCurrentModelCol = dw;
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
				}
			}
			//����
			else
			{
				bIsColorFade = false;
				m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0);
				m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);

				// �����Ƿ񽥱�
				if (m_bColorFade)
				{
					int iA = (dw & 0xff000000) >> 24;
					int iR = (dwColor & 0x00ff0000) >> 16;
					int iG = (dwColor & 0x0000ff00) >> 8;
					int iB = (dwColor & 0x000000ff);

					// ������ɫ����������
					if (m_bColorFadeFlag)
					{
						iMA = (int)(255.0f * m_fColorFadePer);
						m_fColorFadePer -= 0.01f;
						if (iMA < iA) iMA = iA;
						if (m_fColorFadePer < 0.0f) m_fColorFadePer = 0.0f;
					}
					else
					{
						iMA = iA + (int)(255.0f * m_fColorFadePer);
						m_fColorFadePer += 0.01f;
						if (iMA > 0xff) iMA = 0x000000ff;
						if (m_fColorFadePer > 1.0f) m_fColorFadePer = 1.0f;
					}
					m_dwCurrentModelCol = ((DWORD)iMA << 24 | (DWORD)iR << 16 | (DWORD)iG << 8 | (DWORD)iB);
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
					m_dwCurrentModelCol = ((DWORD)iMA << 24 | 0xff << 16 | 0xff << 8 | 0xff);

					//if (m_bIsNeedAlphaFade)
					//m_iAlphaFade = (DWORD)iMA;
				}else
				{
					m_dwCurrentModelCol = dw;
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);

					//if (m_bIsNeedAlphaFade)
					//m_iAlphaFade = (int)((m_dwCurrentModelCol & 0xff000000) >> 24);
				}
			}
		}else
		{
			m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0);
			m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
			m_dwCurrentModelCol = dwColor;
			m_pDisplayModel->SetModelColor(m_dwCurrentModelCol);
			m_dwCurrentModelCol = ((DWORD)0xff << 24 | 0xff << 16 | 0xff << 8 | 0xff);
		}

		//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(), &vLightVector, m_dwCurrentModelCol);
		/*}
		else
		{
		GameMap *pGameMap = pRegion->GetGameMap();
		dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());
		m_pDisplayModel->SetDirLightEnable(false);
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		m_pDisplayModel->SetModelColor(dwColor);
		}*/

		// ����ģ�ͽ���Ч��
		if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_IN)
		{
			long lElapseTime = timeGetTime() - m_lAlphaFadeInStartTime;
			if (lElapseTime > m_lAlphaFadeInTimeLength)
			{
				lElapseTime		 = m_lAlphaFadeInTimeLength;
				m_eAlphaFadeMode = CAlphaFadeObjectList::ALPHA_FADE_NONE;
			}

			float fAlphaValue = (float)lElapseTime / (float)m_lAlphaFadeInTimeLength;

			if (!bIsColorFade)
			{
				m_dwCurrentModelCol &= 0x00ffffff;
				m_dwCurrentModelCol |= ((DWORD)(fAlphaValue * 255.0f)) << 24;
			}

			m_pDisplayModel->MultiplyModelAlpha(fAlphaValue);
		}

		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);

		/*m_pDisplayModel->MultiplyModelColor(GetModelColor());
		m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );*/
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->ProcessVisibility(m_pAnimInfo);

		if (((m_dwCurrentModelCol >> 24)) == 0xff && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			m_pDisplayModel->RenderModel();
		else
			m_pDisplayModel->RenderModelTransparent();
	}
	// ��������Լ�ģ����ɫЧ��
	else if (m_listAddEffect[ADDEFFECT_FLASH].bPlayState)			//����Ч��
	{
		if (timeGetTime() - m_listAddEffect[ADDEFFECT_FLASH].dwStartTime >= m_listAddEffect[ADDEFFECT_FLASH].dwTimeLength)
		{
			m_pDisplayModel->SetModelColorCompel(0xffffffff);
			m_listAddEffect[ADDEFFECT_FLASH].bPlayState = false;
		}

		DWORD dwColor = 0xff000000 |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 16) |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 8) |
			m_listAddEffect[ADDEFFECT_FLASH].dwValue;
		m_pDisplayModel->SetDirLightEnable(false);
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);
		m_pDisplayModel->RenderModelFlash(dwColor);
	}
	else			//����ģʽ�µ���Ⱦ
	{
		DWORD dwColor;
		REGION_LIGHT  edn = GetGame()->GetRegion()->GetClientReionLightType();
		if (edn == RL_CAVE && this != GetGame()->GetMainPlayer())
			m_pDisplayModel->ForceToEnableLight();
		else
			m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0?true:false);
		//m_pDisplayModel->MultiplyModelColor(dwColor);
		//m_pDisplayModel->MultiplyModelColor(dwColor);
		//m_pDisplayModel->SetAmbient(dwColor);

		//m_pDisplayModel->SetDirLightEnable(false);


		//m_pDisplayModel->MultiplyModelColor(GetModelColor());
		//m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		if (GetGame()->GetSetup()->lLight)
		{
			//m_pDisplayModel->SetModelColor(0xFFcfcfcf);
			//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(),(D3DXVECTOR3*)&vLightVector,0xFF808080);
			GameMap *pGameMap = pRegion->GetGameMap();
			dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());
			/***********************************************************************/
			/*  zhaohang 2010-11-16 fix ��float��ΪDWORD*/
			DWORD coefficientR = pGameMap->GetGridColorcoefficientforMoveShapR();
			DWORD coefficientG = pGameMap->GetGridColorcoefficientforMoveShapG();
			DWORD coefficientB = pGameMap->GetGridColorcoefficientforMoveShapB();
			/***********************************************************************/
			
			//��ɫ����2�� * �����и�����ɫ��Ӱ��ϵ��
			DWORD r = ( (dwColor & 0x00FF0000) >> 16) * 2 + (DWORD)coefficientR;
			if (r > 255 )
				r = 255;
			DWORD g = ( (dwColor & 0x0000FF00) >>  8) * 2 + (DWORD)coefficientG;
			if (g > 255) 
				g = 255;
			DWORD b = (  dwColor & 0x000000FF) * 2 + (DWORD)coefficientB;
			if (b > 255)
				b = 255;
			dwColor = 0xff000000 | r << 16 | g << 8 | b;
			m_pDisplayModel->SetModelColor(pGameMap->GetSunLightAmbientColorforMoveShap());
			m_pDisplayModel->MultiplyModelColor(dwColor);

			//if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			//	m_iAlphaFade = (int)((dwColor & 0xff000000) >> 24);
		}
		else
		{
			dwColor = 0xffffffff;
			m_pDisplayModel->SetModelColor(dwColor);

			//if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			//	m_iAlphaFade = (int)((dwColor & 0xff000000) >> 24);
		}

		// ����ģ�ͽ���Ч��
		if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_IN)
		{
			long lElapseTime = timeGetTime() - m_lAlphaFadeInStartTime;
			if (lElapseTime > m_lAlphaFadeInTimeLength)
			{
				lElapseTime		 = m_lAlphaFadeInTimeLength;
				m_eAlphaFadeMode = CAlphaFadeObjectList::ALPHA_FADE_NONE;
			}

			m_pDisplayModel->MultiplyModelAlpha((float)lElapseTime / (float)m_lAlphaFadeInTimeLength);
		}
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);

		/*m_pDisplayModel->MultiplyModelColor(GetModelColor());
		m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );*/
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->ProcessVisibility(m_pAnimInfo);

		if (m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
		{
			m_pDisplayModel->RenderModel();
		}
		else
			m_pDisplayModel->RenderModelTransparent();
	}
	SkillDisplay();
	DisPlayEffectByOrder();
	return true;
}



/*
* ����: MoveShape���ݰ󶨵����ʾ
* ժҪ: �ú���ִ��ģ���������ʾ,�������˹���Ч��
* ����: -
* ����ֵ: ִ�гɹ�����ture,���򷵻�false
* ����: -
* ��������: -
* �޸���־:
*	2009.07.17 - lpf
*		�������ڴ����쳣״̬��ɫ�Ľ���ʱ,����ֲ�����������ݵ����,֮ǰ��ɵ�˲�����Ӧ�þ����������
*/
bool CMoveShape::DisplayByLocator(CDisplayModel *pParentModel,AnimInfo *pParentAnimInfo,DWORD locatorname)
{
	// ��������
	static D3DXVECTOR3 vLightVector(-0.613793f, -0.789465f, 0.001831f);

	CShape::Display();

	// �жϺ���ִ������
	if (!IsNeedDisplay() ||
		m_pDisplayModel == NULL ||
		m_pDisplayModel->GetLoadState() != Load_Did)
		return false;

	// �õ����
	CClientRegion * pRegion = GetGame()->GetRegion();
	if(pRegion == NULL)
		return false;

	render::Camera * pCamera = pRegion->GetCamera();


	// ����ģ����Ч��
	if (m_listAddEffect[ADDEFFECT_SHAKE].bPlayState)
	{
		if (timeGetTime() - m_listAddEffect[ADDEFFECT_SHAKE].dwStartTime >= m_listAddEffect[ADDEFFECT_SHAKE].dwTimeLength)
		{
			m_fXIncrement								= 0.0f;
			m_fYIncrement								= 0.0f;
			m_bRestoration								= false;
			m_listAddEffect[ADDEFFECT_SHAKE].bPlayState = false;
		}

		if (m_bRestoration)
		{
			float fExtent  = (float)m_listAddEffect[ADDEFFECT_SHAKE].dwValue * 0.01f;
			m_fXIncrement  = (float)rand() / RAND_MAX * fExtent + m_fShakeMin;
			m_fYIncrement  = (float)rand() / RAND_MAX * fExtent + m_fShakeMin;
			m_bRestoration = false;
		}
		else
		{
			m_fXIncrement  *= -1.0f;
			m_fYIncrement  *= -1.0f;
			m_bRestoration = true;
		}
	}

	// ����ģ���������
	m_pDisplayModel->GetAnimInfobyLocator(m_pAnimInfo,pParentModel,pParentAnimInfo,locatorname);
	m_pAnimInfo->SetViewMatrix(pRegion->GetCamera()->GetViewMatrix());
	m_pAnimInfo->SetProjectedMatrix(pRegion->GetCamera()->GetProjectionMatrix());
	//m_pDisplayModel->SetRegionAmbient(GetGame()->GetRegion()->GetGameMap()->GetSunLightAmbientColorforBuild());

	if (m_bStateColorPlay)			//�����쳣״̬����ɫ��������
	{
		// AddBy--- ��ǫ----����ɫ��RGB��ΪFF��ʱ��,����͸��Ч��,����Ϊ��ɫ�任Ч�� ,��ɫʱ�رչ���
		DWORD dwColor;
		bool bIsColorFade = false;

		/*if (GetGame()->GetSetup()->lLight)
		{*/
		//m_pDisplayModel->SetModelColor(0xFFcfcfcf);
		//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(),(D3DXVECTOR3*)&vLightVector,0xFF808080);
		GameMap *pGameMap = pRegion->GetGameMap();
		dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());

		int iMA, iMR, iMG, iMB;
		DWORD dw(m_colorModel);
		if (m_colorModel != s_colorModelDefault)
		{
			bIsColorFade = true;

			//��ɫ
			if ((dw & 0x00ffffff) != 0x00ffffff)
			{
				m_pDisplayModel->SetDirLightEnable(false);
				m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);

				// �����Ƿ񽥱�
				if (m_bColorFade)
				{
					int iR = (dw & 0x00ff0000) >> 16;
					int iG = (dw & 0x0000ff00) >> 8;
					int iB = (dw & 0x000000ff);

					int SR = (dwColor & 0x00ff0000) >> 16;
					int SG = (dwColor & 0x0000ff00) >> 8;
					int SB = (dwColor & 0x000000ff);

					// ������ɫ����������
					if (m_bColorFadeFlag)
					{
						iMR = (int)((float)SR * m_fColorFadePer);
						iMG = (int)((float)SG * m_fColorFadePer);
						iMB = (int)((float)SB * m_fColorFadePer);
						m_fColorFadePer -= 0.01f;

						if (iMR < iR) iMR = iR;
						if (iMG < iG) iMG = iG;
						if (iMB < iB) iMB = iB;
						if (m_fColorFadePer < 0.0f) m_fColorFadePer = 0.0f;
					}
					else
					{
						iMR = iR + (int)((float)SR * m_fColorFadePer);
						iMG = iG + (int)((float)SR * m_fColorFadePer);
						iMB = iB + (int)((float)SR * m_fColorFadePer);
						m_fColorFadePer += 0.01f;

						if (iMR > 0xff) iMR = 0x000000ff;
						if (iMG > 0xff)	iMG = 0x000000ff;
						if (iMB > 0xff)	iMB = 0x000000ff;
						if (m_fColorFadePer > 1.0f) m_fColorFadePer = 1.0f;
					}
					m_dwCurrentModelCol = (0xff << 24 | (DWORD)iMR << 16 | (DWORD)iMG << 8 | (DWORD)iMB);
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
				}else
				{
					m_dwCurrentModelCol = dw;
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
				}
			}
			//����
			else
			{
				bIsColorFade = false;
				m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0);
				m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);

				// �����Ƿ񽥱�
				if (m_bColorFade)
				{
					int iA = (dw & 0xff000000) >> 24;
					int iR = (dwColor & 0x00ff0000) >> 16;
					int iG = (dwColor & 0x0000ff00) >> 8;
					int iB = (dwColor & 0x000000ff);

					// ������ɫ����������
					if (m_bColorFadeFlag)
					{
						iMA = (int)(255.0f * m_fColorFadePer);
						m_fColorFadePer -= 0.01f;
						if (iMA < iA) iMA = iA;
						if (m_fColorFadePer < 0.0f) m_fColorFadePer = 0.0f;
					}
					else
					{
						iMA = iA + (int)(255.0f * m_fColorFadePer);
						m_fColorFadePer += 0.01f;
						if (iMA > 0xff) iMA = 0x000000ff;
						if (m_fColorFadePer > 1.0f) m_fColorFadePer = 1.0f;
					}
					m_dwCurrentModelCol = ((DWORD)iMA << 24 | (DWORD)iR << 16 | (DWORD)iG << 8 | (DWORD)iB);
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);
					m_dwCurrentModelCol = ((DWORD)iMA << 24 | 0xff << 16 | 0xff << 8 | 0xff);

					//if (m_bIsNeedAlphaFade)
					//m_iAlphaFade = (DWORD)iMA;
				}else
				{
					m_dwCurrentModelCol = dw;
					m_pDisplayModel->SetModelColorCompel(m_dwCurrentModelCol);

					//if (m_bIsNeedAlphaFade)
					//m_iAlphaFade = (int)((m_dwCurrentModelCol & 0xff000000) >> 24);
				}
			}
		}else
		{
			m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0);
			m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
			m_dwCurrentModelCol = dwColor;
			m_pDisplayModel->SetModelColor(m_dwCurrentModelCol);
			m_dwCurrentModelCol = ((DWORD)0xff << 24 | 0xff << 16 | 0xff << 8 | 0xff);
		}

		//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(), &vLightVector, m_dwCurrentModelCol);
		//}
		/*else
		{
		GameMap *pGameMap = pRegion->GetGameMap();
		dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());
		m_pDisplayModel->SetDirLightEnable(false);
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		m_pDisplayModel->SetModelColor(dwColor);
		}*/

		// ����ģ�ͽ���Ч��
		if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_IN)
		{
			long lElapseTime = timeGetTime() - m_lAlphaFadeInStartTime;
			if (lElapseTime > m_lAlphaFadeInTimeLength)
			{
				lElapseTime		 = m_lAlphaFadeInTimeLength;
				m_eAlphaFadeMode = CAlphaFadeObjectList::ALPHA_FADE_NONE;
			}

			float fAlphaValue = (float)lElapseTime / (float)m_lAlphaFadeInTimeLength;

			if (!bIsColorFade)
			{
				m_dwCurrentModelCol &= 0x00ffffff;
				m_dwCurrentModelCol |= ((DWORD)(fAlphaValue * 255.0f)) << 24;
			}

			m_pDisplayModel->MultiplyModelAlpha(fAlphaValue);
		}

		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);

		/*m_pDisplayModel->MultiplyModelColor(GetModelColor());
		m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );*/
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->ProcessVisibility(m_pAnimInfo);

		if (((m_dwCurrentModelCol >> 24)) == 0xff && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			m_pDisplayModel->RenderModel();
		else
			m_pDisplayModel->RenderModelTransparent();
	}
	// ��������Լ�ģ����ɫЧ��
	else if (m_listAddEffect[ADDEFFECT_FLASH].bPlayState)			//����Ч��
	{
		if (timeGetTime() - m_listAddEffect[ADDEFFECT_FLASH].dwStartTime >= m_listAddEffect[ADDEFFECT_FLASH].dwTimeLength)
		{
			m_pDisplayModel->SetModelColorCompel(0xffffffff);
			m_listAddEffect[ADDEFFECT_FLASH].bPlayState = false;
		}

		DWORD dwColor = 0xff000000 |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 16) |
			(m_listAddEffect[ADDEFFECT_FLASH].dwValue << 8) |
			m_listAddEffect[ADDEFFECT_FLASH].dwValue;
		m_pDisplayModel->SetDirLightEnable(false);
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);
		m_pDisplayModel->RenderModelFlash(dwColor);
	}
	else			//����ģʽ�µ���Ⱦ
	{
		DWORD dwColor;
		REGION_LIGHT  edn = GetGame()->GetRegion()->GetClientReionLightType();
		if (edn == RL_CAVE && this != GetGame()->GetMainPlayer())
			m_pDisplayModel->ForceToEnableLight();
		else
			m_pDisplayModel->SetDirLightEnable(GetGame()->GetSetup()->lLight>0?true:false);
		//m_pDisplayModel->MultiplyModelColor(dwColor);
		//m_pDisplayModel->MultiplyModelColor(dwColor);
		//m_pDisplayModel->SetAmbient(dwColor);

		//m_pDisplayModel->SetDirLightEnable(false);


		//m_pDisplayModel->MultiplyModelColor(GetModelColor());
		//m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );
		m_pDisplayModel->ProcessAnimJoint(m_pAnimInfo);
		if (GetGame()->GetSetup()->lLight)
		{
			//m_pDisplayModel->SetModelColor(0xFFcfcfcf);
			//m_pDisplayModel->ProcessDirectionalLight(pCamera->GetPosition(),(D3DXVECTOR3*)&vLightVector,0xFF808080);
			GameMap *pGameMap = pRegion->GetGameMap();
			dwColor = pGameMap->GetGridColor(GetTileY(),GetTileX());
			DWORD coefficientR = pGameMap->GetGridColorcoefficientforMoveShapR();
			DWORD coefficientG = pGameMap->GetGridColorcoefficientforMoveShapG();
			DWORD coefficientB = pGameMap->GetGridColorcoefficientforMoveShapB();
			//��ɫ����2�� * �����и�����ɫ��Ӱ��ϵ��
			DWORD r = ( (dwColor & 0x00FF0000) >> 16) * 2 + coefficientR;
			if (r > 255 )
				r = 255;
			DWORD g = ( (dwColor & 0x0000FF00) >>  8) * 2 + coefficientG;
			if (g > 255) 
				g = 255;
			DWORD b = (  dwColor & 0x000000FF) * 2 + coefficientB;
			if (b > 255)
				b = 255;
			dwColor = 0xff000000 | r << 16 | g << 8 | b;
			m_pDisplayModel->SetModelColor(pGameMap->GetSunLightAmbientColorforMoveShap());
			m_pDisplayModel->MultiplyModelColor(dwColor);

			//if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			//	m_iAlphaFade = (int)((dwColor & 0xff000000) >> 24);
		}
		else
		{
			dwColor = 0xffffffff;
			m_pDisplayModel->SetModelColor(dwColor);

			//if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			//	m_iAlphaFade = (int)((dwColor & 0xff000000) >> 24);
		}

		// ����ģ�ͽ���Ч��
		if (m_bIsNeedAlphaFade && m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_IN)
		{
			long lElapseTime = timeGetTime() - m_lAlphaFadeInStartTime;
			if (lElapseTime > m_lAlphaFadeInTimeLength)
				m_eAlphaFadeMode = CAlphaFadeObjectList::ALPHA_FADE_NONE;

			m_pDisplayModel->MultiplyModelAlpha((float)lElapseTime / (float)m_lAlphaFadeInTimeLength);
		}

		m_pDisplayModel->ProcessAnimTexture(m_pAnimInfo);
		m_pDisplayModel->ProcessAnimUV(m_pAnimInfo);
		m_pDisplayModel->ProcessEnvironmentMaps(pCamera->GetViewMatrix());
		m_pDisplayModel->ProcessParticleEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessTextureProjective(m_pAnimInfo);
		m_pDisplayModel->ProcessTrailEmitter(m_pAnimInfo);
		m_pDisplayModel->ProcessLocator(m_pAnimInfo);

		/*m_pDisplayModel->MultiplyModelColor(GetModelColor());
		m_pDisplayModel->MultiplyModelAlpha( (float)(GetModelColor()>>24)/255.0f );*/
		m_pDisplayModel->ProcessAnimColor(m_pAnimInfo);
		m_pDisplayModel->ProcessVisibility(m_pAnimInfo);

		if (m_eAlphaFadeMode == CAlphaFadeObjectList::ALPHA_FADE_NONE)
			m_pDisplayModel->RenderModel();
		else
			m_pDisplayModel->RenderModelTransparent();
	}
	SkillDisplay();
	return true;
}



void CMoveShape::RendShadow(bool bsimpleshadow)
{
	D3DXVECTOR3 pos;
	pos.x = m_pAnimInfo->GetWorldMatrix()->_41;
	pos.y = GetHeight();
	pos.z = m_pAnimInfo->GetWorldMatrix()->_43;
	render::BoundingBox * pboundbox = m_pDisplayModel->GetSizeBox();
	float wid = (max(pboundbox->GetWidth(),pboundbox->GetLength()));
	float hei = pboundbox->GetHeight() * 2;
	wid *= m_pAnimInfo->GetScalMatrix()->_11;
	hei *= m_pAnimInfo->GetScalMatrix()->_22;
	float size = max(wid,hei) + 0.5f;
	if (size < 2.0f)
	{
		size = 2.0f;
	}
	/*if (size - (int)size < 0.5f)
	{
	size = (int)size == 0?1.0f:(int)size;
	}
	else
	{
	size = (int)(size + 1.5f);
	}*/
	if (m_pDisplayModel->GetRendShadow())
	{
		CTextureProjective::SetSimpleShadow(bsimpleshadow);
		CTextureProjective::BeginRendShadow(pos,(int)size);
		CTextureProjective::AddRendShadow(m_pDisplayModel,m_pAnimInfo);
		CTextureProjective::EndRendShadow();
	}

}

void CMoveShape::DisplayLayer2D(float pos)
{
	CGlobeSetup::tagSetup* pSetup = CGlobeSetup::GetSetup();

	// �Ի�
	if (m_strTalkText.size())
	{
		D3DCOLOR colorText = D3DCOLOR_ARGB(255,0,0,0);
		D3DCOLOR colorBack = pSetup->colorChatTextBack[CHAT_NORMAL];
		D3DCOLOR colorBackBar = pSetup->colorChatBackBar;

		BYTE rt = LOBYTE(HIWORD(colorText));
		BYTE gt = HIBYTE(LOWORD(colorText));
		BYTE bt = LOBYTE(LOWORD(colorText));
		BYTE rb = LOBYTE(HIWORD(colorBack));
		BYTE gb = HIBYTE(LOWORD(colorBack));
		BYTE bb = LOBYTE(LOWORD(colorBack));
		BYTE rbb = LOBYTE(HIWORD(colorBackBar));
		BYTE gbb = HIBYTE(LOWORD(colorBackBar));
		BYTE bbb = LOBYTE(LOWORD(colorBackBar));

		long lTalkLiveTimer;
		if (m_lTalkLiveTimer > 255)
		{
			lTalkLiveTimer = 255;
		}
		else
		{
			if(m_lTalkLiveTimer>100)
			{lTalkLiveTimer = (m_lTalkLiveTimer -= 5);}
			else
			{lTalkLiveTimer = 0;}
		}

		long lDistance = 0;
		if(this != GetGame()->GetMainPlayer())
			lDistance = GetGame()->GetMainPlayer()->Distance(this);
		// ����10��˵�Ļ�����ʾ
		if(lDistance <= 10)
		{
			DisplayChat(GetTalkText(),
				D3DCOLOR_ARGB(lTalkLiveTimer,rt,gt,bt),
				D3DCOLOR_ARGB(lTalkLiveTimer,rb,gb,bb),
				pos,
				D3DCOLOR_ARGB(lTalkLiveTimer/2,rbb,gbb,bbb));
		}
	}

	// ðѪ
	for(list<tagBloodText>::iterator it=m_listBloodText.begin(); it!=m_listBloodText.end();)
	{
		tagBloodText *pst = &(*it);

		DWORD dwAlpha = pst->lAlpha << 24;
		D3DCOLOR color;
		D3DCOLOR colorBack;

		if(GetType() == TYPE_PLAYER)
		{
			color = (pSetup->colorBloodTextPlayer & 0x00ffffff) | dwAlpha;
			colorBack = (pSetup->colorBloodTextPlayerBack & 0x00ffffff) | dwAlpha;
		}
		else
		{
			color= (pSetup->colorBloodTextMonster & 0x00ffffff) | dwAlpha;
			colorBack= (pSetup->colorBloodTextMonsterBack & 0x00ffffff) | dwAlpha;
		}
		if(pst->bBlastAttack)
		{
			color= (pSetup->colorCriticalBloodText & 0x00ffffff) | dwAlpha;
			colorBack= (pSetup->colorCriticalBloodTextBack & 0x00ffffff) | dwAlpha;
		}
		// ����
		if(pst->cType == AE_CURE)
		{
			color= (pSetup->colorAddBloodTextPlayer & 0x00ffffff) | dwAlpha;
			colorBack= (pSetup->colorAddBloodTextPlayerBack & 0x00ffffff) | dwAlpha;
		}
		switch(pst->cType)
		{
			// ��Ѫ�˺�
		case AE_DAMAGE:
			DisplayDamage(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale);
			break;
			// ����
		case AE_SUCK:
			DisplaySuck(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// ����
		case AE_BREAK:
			DisplayNumber(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->fCurScale2, pst->bBlastAttack);
			break;
			// ��
		case AE_BLOCK:
			DisplayBlock(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// ����
		case AE_REBOUND:
			DisplayRebound(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// ����
		case AE_CURE:
			DisplayNumber(pst->strText.c_str(), color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->fCurScale2, pst->bBlastAttack);
			break;
			// ����
		case AE_FULLMISS:
			DisplayMiss(color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// �м�
		case AE_PARRY:
			DisplayParry(color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// �ֿ�
		case AE_RESIST:
			DisplayResist(color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
			// ��ʧ
		case AE_LOST:
			DisplayLost(color, colorBack, pst->fPos, pst->fPosX, pst->fPosY, pst->fCurScale, pst->bBlastAttack);
			break;
		default:

			break;
		}
		++it;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//	Shape���ߴ���
//////////////////////////////////////////////////////////////////////////////////////////////////

// ����AI
long CMoveShape::MoveAI()
{
	// ����������ߵ�����
	if (GetType()==TYPE_PLAYER)
	{
		//int nPlayRunCount = (float)1/(GetSpeed()*15.0f);  
		//int nPlayWalkCount = (float)1/(GetSpeed()*25.0f); 
		DWORD nPlayRunCount = timeGetTime();  
		DWORD nPlayWalkCount = timeGetTime(); 
		//�����ܲ���Ч
		if ((nPlayRunCount-m_lMoveSoundCount) >= 400&&GetAction()==ACT_RUN)
		{
			if(GetGame()->GetMainPlayer()==this||GetGame()->GetSetup()->bOthersFootstep)
			{
				GetGame()->GetAudioList()->Play(this,TYPE_REGION,GetAction(),Vector3(GetPosY(), GetHeight(), GetPosX()) );
			}
			m_lMoveSoundCount = timeGetTime();
		}
		//����������Ч
		else if ((nPlayWalkCount - m_lMoveSoundCount) >= 450&&GetAction()==ACT_WALK)
		{
			if(GetGame()->GetMainPlayer()==this||GetGame()->GetSetup()->bOthersFootstep)
			{
				GetGame()->GetAudioList()->Play(this,TYPE_REGION,GetAction(),Vector3(GetPosY(), GetHeight(), GetPosX()) );
			}
			m_lMoveSoundCount = timeGetTime();
		}
		m_lMoveSoundCount++;
	}

	//// �����ƶ��ľ���
	//float fDistance = GetSpeed()*static_cast<float>(m_dwMoveTimeCounter - m_dwLastMoveTimeCounter);

	////���������ƫ��
	//float fx = GetPosX() + fDistance * cosf(m_fDir);
	//float fy = GetPosY() + fDistance * sinf(m_fDir);	
	//
	//SetPosXY(fx, fy);
	//�ﵽ��Ŀ�ĵ�

	if( GetCurTickCount() >= GetEndMoveTime())
	{
		//�������
		OnArrive();
		if (m_lType == TYPE_PET && ((CPet*)this)->IsMainPlayerPet())
		{
			return	RET_AI_CONTINUE;
		}
		//�������
		if(GetTileX() != (long)GetDestX() || GetTileY() != (long)GetDestY() )
			SetPosXY(GetDestX(), GetDestY());
	}

	return RET_AI_CONTINUE;
}

void CMoveShape::SetLastUpdatePosTime(DWORD dwTime)
{
	m_dwLastUpdatePosTime = dwTime;
}

// ����Ŀ��㴦��
long CMoveShape::OnArrive()
{
	SetIsArrive(true);

	if ( !IsInForceMoving() &&
		( this == GetGame()->GetMainPlayer() ||
		( m_lType == TYPE_PET && ((CPet*)this)->IsMainPlayerPet() ) ) )
		return RET_AI_CONTINUE;

	if(IsInForceMoving())
		SetInForceMove(false);

	SetAction( ACT_STAND );	
	return RET_AI_BREAK;
}

// �ﵽ�������Ĵ���
long CMoveShape::OnArriveCell(long x, long y)
{
	return OnAttackEvent();
}



/*
* ����: �������¼�
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	22009.02.04 - lpf
*		����AE_BREAK�¼��п�����ɶ�������,�����������������(����Ϣѭ���︴��Ĳ��ı��˺�ƽ״̬),�Ϳ�����ɶ�������Ϣѭ���ﱻ�趨��
*		ƽ״̬��,�ٴ���AI���ﱻ�趨Ϊս��״̬,���������ĸö����Ѿ��Ǻ�ƽ״̬��,���Ըö����ڿͻ��˵�ս��״̬�Ͳ�����ʧ;����ע�͵�ս��
*		״̬��AI����趨,����ս��״̬���趨����������ͨ��MSG_S2C_SHAPE_CHANGESTATE������;
*	22009.03.13 - lpf
*		��AE_BLOCK�е������Ѫ�ı�ʱ,������'-',��������ʱ������������ֻ���һλ;
*	22009.03.27 - lpf
*		��AE_LOST�е������Ѫ�ı�ʱ,�ж�����ǹ���Ҳ�������Ѫ�ı�;
*/
long CMoveShape::OnAttackEvent()
{
	bool bAsyn = true;
	if(this == GetGame()->GetMainPlayer())
		bAsyn = false;
	while( !m_queueAttackEvent.empty() )
	{
		tagAttackEvent* p = &m_queueAttackEvent.front();
		switch(	p->cType )
		{
			// ��Ѫ�˺�
		case AE_DAMAGE:
			{
				SetState(STATE_FIGHT);
				SetHp(p->lFinalHP);
				char str[32];
				// ͷ����ʾ��
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					sprintf(str, "%d", p->lTotalLostPoint);
					AddBloodText(str,AE_DAMAGE, p->bBlastAttack,1.0f);	// �����Ǳ������
				}
				// ��������ǵ��������ٵ�������
				if(this == pPlayer)
				{
					for (int i = 0; i<(int)p->vtLost.size(); i++)
					{
						switch (p->vtLost[i].cLostType)
						{
						case DAMAGE_TYPE_HP:
							break;
						case DAMAGE_TYPE_MP:
							if ((long)(GetGame()->GetMainPlayer()->GetMp()) >= p->vtLost[i].lLostPoint)
								GetGame()->GetMainPlayer()->SetMp((WORD)((GetGame()->GetMainPlayer()->GetMp()) - p->vtLost[i].lLostPoint));
							break;
						}
					}
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();

					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				// ������������
				else
				{	
					//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
					//if(pObjPage)
					//{
					//	// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
					//	if(this==pObjPage->GetObjShape())
					//	{
					//		pObjPage->SetObjectHpInfo();
					//	}
					//}
                    if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
                    {
                        FireUIEvent("ObjectPage", "ObjectAttr");
                    }
				}
				m_queueAttackEvent.pop();
			}
			break;
		case AE_SUCK:
			{
				SetState(STATE_FIGHT);
				if(GetAction()==ACT_STAND)
					SetAction(ACT_BLOCK);
				SetHp(p->lFinalHP);
				// ���ðѪ����
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					char str[32];
					if(p->lTotalLostPoint == 0)
						sprintf(str,"");
					else
						sprintf(str, "%d", p->lTotalLostPoint);
					AddBloodText(str,AE_BLOCK, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				// ��������ǵ��������ٵ�������
				if (this == pPlayer)
				{
					for(int i=0; i<(int)p->vtLost.size(); i++)
					{
						switch (p->vtLost[i].cLostType)
						{
						case DAMAGE_TYPE_HP:
							break;
						case DAMAGE_TYPE_MP:
							if ((long)(GetGame()->GetMainPlayer()->GetMp()) >= p->vtLost[i].lLostPoint)
								GetGame()->GetMainPlayer()->SetMp((WORD)((GetGame()->GetMainPlayer()->GetMp())-p->vtLost[i].lLostPoint));
							//else
							;//GetGame()->GetMainPlayer()->SetMP(0);
							break;
						}
					}
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				// ������������
				else
				{	
					//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
					//if(pObjPage)
					//{
					//	// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
					//	if(this==pObjPage->GetObjShape())
					//	{
					//		pObjPage->SetObjectHpInfo();
					//	}
					//}
                    if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
                    {
                        FireUIEvent("ObjectPage", "ObjectAttr");
                    }
				}
				m_queueAttackEvent.pop();
				// ������Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
			}
			break;
			// ��ȫ����
		case AE_FULLMISS:
			{
				//SetState(STATE_FIGHT);
				if(GetAction()==ACT_STAND)
					SetAction(ACT_MISS);
				// ͷ����ʾ��
				// �����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					AddBloodText("����", AE_FULLMISS, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				if(this == pPlayer)
				{
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				m_queueAttackEvent.pop();
				long lGraphicID(0);
				float x, y, height;
				// ������Ч
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height,-1.f,bAsyn);
				// ��������
				GetGame()->GetAudioList()->Play(this,GetType(),ACT_MISS, Vector3(GetPosY(), GetHeight(), GetPosX()));
			}
			break;
			// �м�
		case AE_PARRY:
			{
				//SetState(STATE_FIGHT);
				if(GetAction()==ACT_STAND)
					SetAction(ACT_PARRY);
				// ͷ����ʾ��
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					AddBloodText("�м�", AE_PARRY, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				if(this == pPlayer)
				{
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				m_queueAttackEvent.pop();
				// �м���Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
				// �м�����
				GetGame()->GetAudioList()->Play(this,GetType(),ACT_PARRY, Vector3(GetPosY(), GetHeight(), GetPosX()));
			}
			break;
			// ��
		case AE_BLOCK:
			{
				SetState(STATE_FIGHT);
				if(GetAction()==ACT_STAND)
					SetAction(ACT_BLOCK);
				DWORD dwOldHP = GetHp();
				SetHp(p->lFinalHP);
				// ���ðѪ����
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					char str[32];
					sprintf(str, "-%d", p->lTotalLostPoint);
					AddBloodText(str,AE_BLOCK, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				// ��������ǵ��������ٵ�������
				if (this == pPlayer)
				{
					for(int i=0; i<(int)p->vtLost.size(); i++)
					{
						switch (p->vtLost[i].cLostType)
						{
						case DAMAGE_TYPE_HP:
							break;
						case DAMAGE_TYPE_MP:
							if ((long)(GetGame()->GetMainPlayer()->GetMp()) >= p->vtLost[i].lLostPoint)
								GetGame()->GetMainPlayer()->SetMp((WORD)((GetGame()->GetMainPlayer()->GetMp())-p->vtLost[i].lLostPoint));
							//else
							;//GetGame()->GetMainPlayer()->SetMP(0);
							break;
						}
					}
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				m_queueAttackEvent.pop();
				// ����Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
				// ������
				GetGame()->GetAudioList()->Play(this,GetType(),ACT_BLOCK, Vector3(GetPosY(), GetHeight(), GetPosX()));
			}
			break;
			// �ֿ�
		case AE_RESIST:
			{
				//SetState(STATE_FIGHT);
				// ͷ����ʾ��
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					AddBloodText("�ֿ�",AE_RESIST, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				if(this == pPlayer)
				{
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				m_queueAttackEvent.pop();
				// �ֿ���Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
			}
			break;
			// ����
		case AE_REBOUND:
			{
				SetState(STATE_FIGHT);
				DWORD dwOldHP = GetHp();
				SetHp(p->lFinalHP);
				// ���ðѪ����
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					char str[32];
					sprintf(str, "%d", p->lTotalLostPoint);
					AddBloodText(str,AE_REBOUND, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				// ��������ǵ��������ٵ�������
				if (this == pPlayer)
				{
					for(int i=0; i<(int)p->vtLost.size(); i++)
					{
						switch (p->vtLost[i].cLostType)
						{
						case DAMAGE_TYPE_HP:
							break;
						case DAMAGE_TYPE_MP:
							if ((long)(GetGame()->GetMainPlayer()->GetMp()) >= p->vtLost[i].lLostPoint)
								GetGame()->GetMainPlayer()->SetMp((WORD)((GetGame()->GetMainPlayer()->GetMp())-p->vtLost[i].lLostPoint));
							//else
							;//GetGame()->GetMainPlayer()->SetMP(0);
							break;
						}
					}
                    //GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				m_queueAttackEvent.pop();
				// ������Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
			}
			break;

			// ����
		case AE_CURE:
			{
				// ���ﲻ�������˶���
				//if(GetAction()==ACT_STAND && GetType() != TYPE_MONSTER)
				//	SetAction("���˶���");
				//�����ǰ�Ķ����ǺͶ�����صģ�Ϊ�����ðѪ����
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					char str[32];
					sprintf(str,"+%d",p->lTotalLostPoint);
					AddBloodText(str, AE_CURE, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
				/*CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
				if(pObjPage && this==pObjPage->GetObjShape())
				{
					pObjPage->SetObjectHpInfo();
				}*/
                if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
                {
                    FireUIEvent("ObjectPage", "ObjectAttr");
                }
				m_queueAttackEvent.pop();
				// ��Ѫ��Ч
				long lGraphicID(0);
				float x, y, height;
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir,bAsyn);
			}
			break;

			// ����
		case AE_BREAK:
			{
				//SetState(STATE_FIGHT);

				// ���ﲻ�������˶���
				if(GetAction()==ACT_STAND && GetType() != TYPE_MONSTER)
					SetAction(ACT_BEATTACK);

				DWORD dwOldHP = GetHp();
				SetHp(p->lFinalHP);

				//�����ǰ�Ķ����ǺͶ�����صģ�Ϊ�����ðѪ����
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID())
				{
					char str[32];
					sprintf(str,"-%d",p->lTotalLostPoint);
					AddBloodText(str, AE_BREAK, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}			

				// ��������ǵ��������ٵ�������
				if (this == pPlayer)
				{
					for(int i=0; i<(int)p->vtLost.size(); i++)
					{
						switch (p->vtLost[i].cLostType)
						{
						case DAMAGE_TYPE_HP:
							break;
						case DAMAGE_TYPE_MP:
							if ((long)(GetGame()->GetMainPlayer()->GetMp()) >= p->vtLost[i].lLostPoint)
								GetGame()->GetMainPlayer()->SetMp((WORD)((GetGame()->GetMainPlayer()->GetMp())-p->vtLost[i].lLostPoint));
							else
								;//GetGame()->GetMainPlayer()->SetMP(0);
							break;
						}
					}
					//GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
					// �����߲����Լ�
					if(p->Guidattracker != this->GetExID())
					{
						// ��֤��ǰѡ��Ŀ���Ƿ����
						CGUID guid = CGUID::GUID_INVALID;
						long type = 0;
						//��ǰѡ��Ŀ�겻Ϊ��
						if(GetGame()->GetGameControl()->GetSelectedShape())
						{
							guid = GetGame()->GetGameControl()->GetSelectedShape()->GetExID();
							type = GetGame()->GetGameControl()->GetSelectedShape()->GetType();
						}
						CMoveShape* pattracker = dynamic_cast<CMoveShape*>(GetGame()->GetRegion()->FindChildObject(type,guid));
						// �Ҳ�����ǰѡ�е�Ŀ�꣬���ù�����Ϊѡ��Ŀ��
						if(!pattracker)
						{
							// ������Ϊ���
							pattracker = dynamic_cast<CPlayer*>(GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker));
							// ������Ϊmonster
							if (!pattracker)
								pattracker = dynamic_cast<CMonster*>(GetGame()->GetRegion()->FindChildObject(TYPE_MONSTER,p->Guidattracker));
							// ������Ϊ����
							//if (!pattracker)
							//	pattracker = dynamic_cast<CPet*>(GetGame()->GetRegion()->FindChildObject(TYPE_PET,p->Guidattracker));
							if(pattracker)
								GetGame()->GetGameControl()->SetSelectedShape(pattracker);
						}
					}
				}
				// ������������
				else
				{	
					//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
					//if(pObjPage)
					//{
					//	// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
					//	if(this==pObjPage->GetObjShape())
					//	{
					//		pObjPage->SetObjectHpInfo();
					//	}
					//}
                    if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
                    {
                        FireUIEvent("ObjectPage", "ObjectAttr");
                    }
				}

				m_queueAttackEvent.pop();

				long lGraphiscID(1);
				long lSoundID(1);
				/***********************************************************************/
				/* zhaohang fix */
				/***********************************************************************/
				CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(p->dwSkillID);
				if(pSkill)
				{
					bool  bFlashEffect		  = false;
					DWORD dwFlashEffectLum    = 0;
					DWORD dwFlashEffectTime   = 0;
					bool  bShakeEffect		  = false;
					DWORD dwShakeEffectExtent = 0;
					DWORD dwShakeEffectTime   = 0;
					bool  bHaveEnergy         = false;

					DWORD dwlev = pSkill->vectorLevels.size() >= p->bySkillLevel? p->bySkillLevel-1:((DWORD)pSkill->vectorLevels.size() - 1);
					CSkillListXml::tagLevel pLevel = pSkill->vectorLevels[dwlev];
					vector<CSkillListXml::tagLevelValue>::iterator pos;
					for(pos = pLevel.vecStepEnd.begin(); pos!= pLevel.vecStepEnd.end(); pos++)
					{
						// ��������������Ч
						if(!strcmp((*pos).strKeyWord.c_str(),"HitEffect"))
						{
							CPlayer* pattracker = (CPlayer*)GetGame()->GetRegion()->FindChildObject(TYPE_PLAYER,p->Guidattracker);
							if (pattracker)
							{
								DWORD energyeffectid = pattracker->GetEnergyHitEffectID();
								if (energyeffectid && pSkill->dwSkillUseType == 1)
								{
									lGraphiscID = energyeffectid;
									bHaveEnergy = true;
								}
								else
								{
									lGraphiscID = (*pos).dwValue;
								}
							}
							else
							{
								lGraphiscID = (*pos).dwValue;
							}
							//float x, y, height;
							AddEffectLocator(lGraphiscID,this,LOCATOR_WIND,false,p->fHurtedDir,0,bAsyn);
							//if (GetLocatorPos(LOCATOR_WIND, x, y, height))
							//	GetGame()->GetRegion()->PlayEffect(lGraphiscID, x, y, height, p->fHurtedDir);
						}
						// �Ƿ��������Ч��
						else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffect"))
						{
							bFlashEffect = true;
						}
						// ��������
						else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffectLum"))
						{
							dwFlashEffectLum = (*pos).dwValue;
						}
						// �������ʱ��
						else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffectTime"))
						{
							dwFlashEffectTime = (*pos).dwValue;
						}
						// �Ƿ������Ч��
						else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffect"))
						{
							bShakeEffect = true;
						}
						// ���Ž�ɫ��Ч��
						else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffectExtent"))
						{
							dwShakeEffectExtent = (*pos).dwValue;
						}
						// ���Ž�ɫ��Ч��
						else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffectTime"))
						{
							dwShakeEffectTime = (*pos).dwValue;
						}
						// ���ų��������������������������������Ч�����������
						else if(!strcmp((*pos).strKeyWord.c_str(),"HitSound"))
						{
							if(bHaveEnergy==false)
							{
								lSoundID = (*pos).dwValue;
								char strSound[32];
								sprintf(strSound,"sounds/skills/%d.wav",lSoundID);
								GetGame()->GetAudioList()->Play(strSound, GetPosX(),GetPosY(),GetHeight());
							}else{
								char strSound[32];
								lSoundID = 33;
								sprintf(strSound,"sounds/interfaces/i-%d.wav",lSoundID);
								GetGame()->GetAudioList()->Play(strSound, GetPosX(),GetPosY(),GetHeight());
							}
						}
					}

					// �趨����������Ч
					if (GetGame()->GetSetup()->bRoleFlash && bFlashEffect)
						SetupFlashEffect(dwFlashEffectLum, dwFlashEffectTime);

					// �趨��������Ч
					if (GetGame()->GetSetup()->bRoleShake && bShakeEffect)
						SetupShakeEffect(dwShakeEffectExtent, dwShakeEffectTime);
				}

				// �������˵ĳ̶Ȳ�����Ӧ����Ч
				float fMildDamage=0;
				float fHighDamage=0;
				// ��Ҳ���ְҵ���Ա�ͳһ�ı��������˺��̶�
				if ( GetType() == TYPE_PLAYER )
				{
					fMildDamage = CMoveShape::mildhurt_knight;
					fHighDamage = CMoveShape::highhurt_knight;
				}

				//���������С�˺����Ǵ��˺�
				if ( GetType() == TYPE_MONSTER )
				{
					int iMonsterType = dynamic_cast<CMonster*>(this)->GetClass();
					switch ( iMonsterType )
					{
					case CMonster::MK_BOSS:
						{
							fMildDamage = CMoveShape::mildhurt_boss;
							fHighDamage = CMoveShape::highhurt_boss;
						}
						break;

					default :
						{
							fMildDamage = CMoveShape::mildhurt_monster;
							fHighDamage = CMoveShape::highhurt_monster;
						}
						break;
					}
				}

				// ���׷���
				if ( GetType() == TYPE_PLAYER && dynamic_cast<CPlayer*>(this)->GetEquip(CPlayer::EQUIP_BODY) )
				{
					CPlayer * pPlayer = dynamic_cast<CPlayer*>(this);					
					if ( p->lTotalLostPoint < (float)pPlayer->GetMaxHp()*fMildDamage )
					{
						// �����п��׷���
						GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_NORMAL, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
					}
					else if ( p->lTotalLostPoint > (float)pPlayer->GetMaxHp()*fHighDamage )
					{
						// �ػ��п��׷���
						GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_EXCEPT, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
						// �ҽ�
						GetGame()->GetAudioList()->Play(this,GetType(),ACT_BEATTACK, Vector3(GetPosY(), GetHeight(), GetPosX()));
					}
					else
					{
						// �ػ��п��׷���
						GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_EXCEPT, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
					}
				}
				else // ���� �� �ˣ���װ����
				{
					enum
					{
						ACT_HURT01 = ACT_BEATTACK,			// ��ͨ
						ACT_HURT02  = 12,					// ���ػ�		
					};

					CMoveShape * pMoveShape = dynamic_cast<CMoveShape*>(this);

					if ( p->lTotalLostPoint <= (float)pMoveShape->GetMaxHp()*fMildDamage )
					{
						// ���˺�
						GetGame()->GetAudioList()->Play(this,GetType(),ACT_HURT01, Vector3(GetPosY(), GetHeight(), GetPosX()));
					}
					else if ( p->lTotalLostPoint > (float)pMoveShape->GetMaxHp()*fHighDamage )	// ǿ�˺�
					{
						// �ػ��˺�
						GetGame()->GetAudioList()->Play(this,GetType(),ACT_HURT02, Vector3(GetPosY(), GetHeight(), GetPosX()));
						// �ҽ�
						GetGame()->GetAudioList()->Play(this,GetType(),ACT_BEATTACK, Vector3(GetPosY(), GetHeight(), GetPosX()));
					}
					else	// �ػ��˺�
					{
						GetGame()->GetAudioList()->Play(this,GetType(),ACT_HURT01, Vector3(GetPosY(), GetHeight(), GetPosX()));
					}
				}

				return RET_AI_BREAK;
			}
			break;

			// ����
		case AE_DIED:
			{
				SetState(STATE_DIED);
				SetAction( ACT_DIED );

				//// ðѪ
				//if(p->eDeathType==1)
				//{
				//	char str[32];
				//	sprintf(str,"%d",p->lTotalLostPoint);
				//	AddBloodText(str, AE_BREAK, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				//}

				SetHp(0);
				// ����ѡ��Ŀ����Ϣ
				//CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
				//// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
				//if(pObjPage && this==pObjPage->GetObjShape())
				//{
				//	pObjPage->SetObjectHpInfo();
				//}
                if ( this == GetGame()->GetGameControl()->GetSelectedShape() )
                {
                    FireUIEvent("ObjectPage", "ObjectAttr");
                }

				// �Լ�������
				if( this == GetGame()->GetMainPlayer() )
				{
					/*CGoods* pHandGoods = GetGame()->GetCGuiEx()->GetPMainPlayerHand()->GetPGoodsOfMainPlayerHand();
					if (pHandGoods!=NULL)
					{
						pHandGoods->SetHaveShadowState(false);
					}
					GetGame()->GetCGuiEx()->GetPMainPlayerHand()->ClearMainPlayerHand();
					GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();*/
                    FireUIEvent("MainRole", "RoleAttr");
					// ��������������
					GetGame()->GetMainPlayer()->m_listGrowHP.clear();
					GetGame()->GetMainPlayer()->m_listGrowMP.clear();
					// ֹͣ�ƶ��͹���
					GetGame()->GetGameControl()->AutoAttactStop();
					GetGame()->GetGameControl()->AutoMoveStop();
				}

				// ��ʾ������
				if (p->lHitNum!=-1)
				{
					GetGame()->GetMainPlayer()->SetHitNum(p->lHitNum);
					GetGame()->GetMainPlayer()->SetLastHitTimer(GetCurTickCount());
				}

				// ����쳣״̬
				//ClearStateEx();

				while(!m_queueAttackEvent.empty())
					m_queueAttackEvent.pop();

				GetGame()->GetAudioList()->Play(this,GetType(),ACT_DIED, Vector3(GetPosY(), GetHeight(), GetPosX()));
				return RET_AI_BREAK;
			}
			break;
			// ��ʧ
		case AE_LOST:
			{
				// ��Ӷ�ʧ
				// ͷ����ʾ��
				//�����ǰ�Ķ����ǺͶ�����ص�
				CPlayer *pPlayer = GetGame()->GetMainPlayer();
				if(pPlayer->IsTeammate(p->Guidattracker) || 
					pPlayer->IsTeammate(this->GetExID()) ||
					this == pPlayer || p->Guidattracker == pPlayer->GetExID() ||
					this->GetType() == TYPE_MONSTER)
				{
					AddBloodText("��ʧ", AE_LOST, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
				}
				m_queueAttackEvent.pop();
				long lGraphicID(0);
				float x, y, height;
				// ������Ч
				if(GetLocatorPos(LOCATOR_WIND, x, y, height) && lGraphicID > 0)
					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height,-1.f,bAsyn);
				// ��������
				char* strSound="sounds/xxx.wav";
				GetGame()->GetAudioList()->Play(strSound,GetPosX(),GetPosY(),GetHeight());
			}
			break;
			// �ı䷽��
		case AE_CHANGEDIR:
			{
				if(GetAction() == ACT_STAND || GetAction() == ACT_IDLE)
				{
					long lDir = (long)p->fHurtedDir;
					SetDir((CShape::eDIR)lDir);
					m_queueAttackEvent.pop();
				}
				else if(m_queueAttackEvent.size() == 1)
				{
					return RET_AI_CONTINUE;
				}
				else
				{
					m_queueAttackEvent.push(m_queueAttackEvent.front());
					m_queueAttackEvent.pop();
				}
			}
			break;
		}
		//if(!m_queueAttackEvent.empty())
		//	m_queueAttackEvent.pop();
	}
	return RET_AI_CONTINUE;
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



/*
* ����: �����쳣״̬
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.10.20 - lpf
*		�������쳣״̬����ɫ�����ڴ˳�ʼ��.
*/
void CMoveShape::AddStateEx(DWORD dwID, DWORD dwLevel, DWORD dwStandingTime, DWORD dwAdditionalData,BYTE bFlag)
{
	// ������µ�Shape�ǵ�ǰѡ��Ķ������״̬��Ϣ
	/*CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	if(pObjPage && (pObjPage->GetObjShape()==this))
	{
		pObjPage->AddObjState(dwID,dwLevel);
	}*/
	CStateListXml::tagStateList *pSL = CStateListXml::GetState(dwID);
	if(pSL)
	{
		DWORD dwEffect = 0;
		DWORD dwEffectIndex = 0;
		DWORD dwLocator = 0;
		bool bIsOrderPlay = false;
		if(dwLevel >= (DWORD)pSL->vecStateLevel.size())
			dwLevel = (DWORD)pSL->vecStateLevel.size();
		vector<CStateListXml::tagStateEffect> vecStateEffect = pSL->vecStateLevel[dwLevel-1].vecMidEffect;
		// ɾ���м�׶���Ч
		for (size_t i=0; i<vecStateEffect.size();++i)
		{
			// ������״̬��ָ���ȼ�����Ч��Ϣ
			dwEffect		= vecStateEffect[i].dwEffectID;
			dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
			bIsOrderPlay	= vecStateEffect[i].bIsOrderPlay;
			// �ӱ���������ɾ�����е���Ч
			if(!bIsOrderPlay)
				DelEffect(dwEffect,dwEffectIndex);
		}
		DelAllEffectByOrder();
		// ɾ�������׶���Ч
		vecStateEffect = pSL->vecStateLevel[dwLevel-1].vecEndEffect;
		for (size_t i=0; i<vecStateEffect.size();++i)
		{
			// ������״̬��ָ���ȼ�����Ч��Ϣ
			dwEffect		= vecStateEffect[i].dwEffectID;
			dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
			// �ӱ���������ɾ�����е���Ч
			DelEffect(dwEffect,dwEffectIndex);
		}
		// ��ӿ�ʼ�׶���Ч
		vecStateEffect = pSL->vecStateLevel[dwLevel-1].vecBeginEffect;
		for (size_t i=0; i<vecStateEffect.size();++i)
		{
			// ������״̬��ָ���ȼ�����Ч��Ϣ
			dwEffect		= vecStateEffect[i].dwEffectID;
			dwEffectIndex	= vecStateEffect[i].dwEffectIndex;
			dwLocator		= vecStateEffect[i].dwEffectPosition;		//ʩ��λ��
			// �ӱ���������ɾ�����е���Ч
			DelEffect(dwEffect,dwEffectIndex);
			eLOCATOR Loc;
			switch(dwLocator)
			{
			case 0: Loc = LOCATOR_WIND; break;
			case 1: Loc = LOCATOR_HEAD; break;
			case 2: Loc = LOCATOR_FOOTCENTER; break;
			case 3: Loc = LOCATOR_LEFTHAND; break;
			case 4: Loc = LOCATOR_RIGHTHAND; break;
			case 5: Loc = LOCATOR_LEFTFOOT; break;
			case 6: Loc = LOCATOR_RIGHTFOOT; break;
			default: Loc = LOCATOR_FOOTCENTER; break;
			}
			// Ϊ��������Ӹ�״̬��ʼ�׶ε���Ч
			if(dwEffect)
				AddEffectLocator(dwEffect, this, Loc, false, 0.0f, dwEffectIndex);
		}

		// ��״̬�ж�Ӧ�Ķ���
		if(pSL->wAction > 0)
			SetAction(pSL->wAction);

		BOOL bStateColorPlay = (BOOL)pSL->vecStateLevel[(dwLevel) - 1].vecBuffColor.size();
		if (bStateColorPlay)
		{
			m_CurrentStateExForStateColor.dwID = dwID;
			m_CurrentStateExForStateColor.dwLevel = dwLevel;
			m_CurrentStateExForStateColor.dwStartTime = GetCurTickCount();
			m_CurrentStateExForStateColor.dwStandingTime = dwStandingTime;
			m_CurrentStateExForStateColor.bEndSelf = false;
			m_bStateColorPlay = TRUE;
			m_bColorFade	  = pSL->vecStateLevel[(dwLevel) - 1].bColorFade;					//�Ƿ��쳣״̬��ɫ��������
		}
	}

	// �������״̬
	if(this == GetGame()->GetMainPlayer())
	{
		// �����״̬�������ֹͣ�ƶ�
		if(pSL && pSL->bStopMove)
		{
			GetGame()->GetGameControl()->AutoAttactStop();
			GetGame()->GetGameControl()->AutoMoveStop();
		}
	}

	list<tagStateEx>::iterator it;
	for(it = m_listStateEx.begin(); it != m_listStateEx.end(); it++)
	{
		tagStateEx* pStateEx = &(*it);
		if (pStateEx && pStateEx->dwID==dwID && pStateEx->bEndSelf==false)
		{
			pStateEx->dwStartTime = GetCurTickCount();
			pStateEx->dwStandingTime = dwStandingTime;
			pStateEx->dwAdditionalData = dwAdditionalData;
			pStateEx->dwLevel = dwLevel;
			if(bFlag == 0 && pStateEx->lCount >= 0)
				pStateEx->lCount++;
			return;
		}
	}

	tagStateEx st;
	st.dwID = dwID;
	st.dwLevel = dwLevel;
	st.dwAdditionalData = dwAdditionalData;
	st.dwStartTime = GetCurTickCount();
	st.dwStandingTime =dwStandingTime;
	st.bEndSelf = false;
	st.lCount = 1;
	m_listStateEx.push_back(st);
}

void CMoveShape::RemoveStateEx(DWORD dwID,DWORD dwLevel)
{
	if(dwID==STATE_RIDE_HORSE)
	{
		CPlayer *pPlayer = (CPlayer *)this;
		pPlayer->GetHorse()->SetVisible(FALSE);
		g_bForceUpdate=true;
		SetAction(GetAction());
		return;
	}
	list<tagStateEx>::iterator it;
	for(it = m_listStateEx.begin(); it != m_listStateEx.end(); it++)
	{
		tagStateEx* pStateEx = &(*it);
		if (pStateEx && pStateEx->dwID==dwID && pStateEx->dwLevel == dwLevel)
		{
			pStateEx->lCount--;
			if (pStateEx->lCount<=0)
			{
				pStateEx->bEndSelf = true;
				AI_StateEx();
				// ��ǰ�Ķ������ڸ�״̬��Ӧ�Ķ�������״̬����ʱ�����ö���Ϊվ��
				CStateListXml::tagStateList *pSL = CStateListXml::GetState(dwID);
				if(pSL)
				{
					if(pSL->wAction == GetAction())
						SetAction(ACT_STAND);
				}
				// ������µ�Shape�ǵ�ǰѡ��Ķ���ɾ��״̬��Ϣ
				/*CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
				if(pObjPage && (pObjPage->GetObjShape()==this))
					pObjPage->RemoveObjState(dwID);*/
			}
			return;
		}
	}
}

// ��������쳣״̬
void CMoveShape::ClearStateEx()
{
	int x = GetGame()->GetMainPlayer()->GetTileX();
	int y = GetGame()->GetMainPlayer()->GetTileY();
	CRegion::tagCell *pCell = GetGame()->GetRegion()->GetCell(x, y);
	// ������µ�Shape�ǵ�ǰѡ��Ķ���ɾ��״̬��Ϣ
	/*CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	if(pObjPage && (pObjPage->GetObjShape()==this))
	{
		pObjPage->ClearState();
	}*/
	list<tagStateEx>::iterator it;
	for(it = m_listStateEx.begin(); it != m_listStateEx.end(); it++)
	{
		tagStateEx* pStateEx = &(*it);
		if (pStateEx)
		{
			CStateListXml::tagStateList *pSL = CStateListXml::GetState(pStateEx->dwID);
			if (pSL && pSL->vecStateLevel[pStateEx->dwLevel-1].AfettleDeathIsClear && pCell->bSecurity == CRegion::SECURITY_FREE)
			{
				pStateEx->lCount = 0;
				pStateEx->bEndSelf = true;
			}
		}
	}
}

void CMoveShape::GetCurGraphicTypeID(long &lType,long &lID)
{
	if(m_lChangeGraphicsID==0)
		return CBaseObject::GetCurGraphicTypeID(lType,lID);
	else
	{
		lType = m_lChangeGraphicstype;
		lID = m_lChangeGraphicsID;
	}

}
// ����
bool CMoveShape::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CShape::DecordFromByteArray(pByte, pos, bExData);
	float fSpeedChangedRatio = _GetFloatFromByteArray(pByte, pos);
	SetActionSpeedRate(ACT_WALK,fSpeedChangedRatio);
	SetActionSpeedRate(ACT_RUN,fSpeedChangedRatio);

	m_bRun			        = (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bAntiInvisibleable	= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bInvisible			= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bPretendDead			= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;

	m_bUseRabbSkill			= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bUseFighterSkill		= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bUseItem				= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bInstruction			= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bUnAttack				= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bAddHp				= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bHide					= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bForceAttact			= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;

	m_dwCollectID		= _GetDwordFromByteArray(pByte, pos);
	if (m_dwCollectID!=0)
	{
		const CCollectionLayout* CollectLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
		if (CollectLayout!=NULL)
		{
			m_strCollectionName = CollectLayout->GetCollectionName();
		}
	}
	m_dwCollectTimes        = _GetDwordFromByteArray(pByte, pos);
	m_bIsBeenCollect        = (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bCanBeenCollect		= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_bNotRide					= (_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	m_lChangeGraphicstype	= _GetLongFromByteArray(pByte, pos);
	m_lChangeGraphicsID		= _GetLongFromByteArray(pByte, pos);
	BYTE bDied				= _GetByteFromByteArray(pByte, pos);
	if (bDied)
	{
		// ��������״̬
		CShape::SetState(STATE_DIED);
		CShape::SetAction(ACT_ALREADYDIED);
	}
	if(!bExData)
	{
		//��ѹbuff
		DecordBuffFromByteArray(pByte, pos);
		//��ѹdebuff
		DecordDeBuffFromByteArray(pByte, pos);
	}
	//��ѹ��������
	DecordActSkillFromByteArray(pByte, pos);
	//��ѹ���ؼ���
	if(bExData)
		DecordOnOffSkillFromByteArray(pByte, pos);
	//�����ƶ�Ŀ���
	SetDestXY(GetPosX(),GetPosY());
	return true;
}

//��ѹbuff
bool CMoveShape::DecordBuffFromByteArray(BYTE* pByte, long& pos)
{
	DWORD dwSize = _GetDwordFromByteArray(pByte, pos);
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  _GetLongFromByteArray(pByte, pos);
		long lLvl = _GetLongFromByteArray(pByte, pos);
		DWORD dwRemianTime = _GetDwordFromByteArray(pByte, pos);
		AddStateEx(lID, lLvl, dwRemianTime);
	}
	return true;
}

//��ѹdebuff
bool CMoveShape::DecordDeBuffFromByteArray(BYTE* pByte, long& pos)
{
	DWORD dwSize = _GetDwordFromByteArray(pByte, pos);
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  _GetLongFromByteArray(pByte, pos);
		long lLvl = _GetLongFromByteArray(pByte, pos);
		DWORD dwRemianTime = _GetDwordFromByteArray(pByte, pos);
		AddStateEx(lID, lLvl, dwRemianTime);
	}
	return true;
}
//��ѹonoffskill
bool CMoveShape::DecordOnOffSkillFromByteArray(BYTE* pByte, long& pos)
{
	DWORD dwSize = _GetDwordFromByteArray(pByte, pos);
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  _GetLongFromByteArray(pByte, pos);
		long lLvl = _GetLongFromByteArray(pByte, pos);
	}
	return true;
}
//��ѹ����ʹ�ü���
bool CMoveShape::DecordActSkillFromByteArray(BYTE* pByte, long& pos)
{
	bool bFlag = _GetByteFromByteArray(pByte, pos)==0?false:true;
	if(!bFlag)	return false;
	int num = _GetLongFromByteArray(pByte, pos);	// ����ʹ�õļ�����
	for (int i = 0 ; i< num; i++)
	{
		tagDecordSkillList skillList;
		skillList.lSkillKey = _GetLongFromByteArray(pByte, pos);
		skillList.lSkillID =  _GetLongFromByteArray(pByte, pos);
		skillList.lSkillLvl = _GetLongFromByteArray(pByte, pos);
		//��ǰִ�еĽ׶����
		skillList.lProceNum = _GetLongFromByteArray(pByte, pos);

		skillList.lMouseX = _GetLongFromByteArray(pByte, pos);
		skillList.lMouseY = _GetLongFromByteArray(pByte, pos);

		DWORD dwDestObjNum = _GetDwordFromByteArray(pByte, pos);
		CGUID guid;
		for(DWORD i=0;i<dwDestObjNum;i++)
		{
			skillList.vecType.push_back( _GetLongFromByteArray(pByte, pos));
			_GetBufferFromByteArray(pByte, pos,guid);
			skillList.vecID.push_back(guid);
		}
		m_vecDecordSkill.push_back(skillList);
	}

	return true;
}

bool CMoveShape::DecodeFromDataBlock(DBReadSet& setReadDB, bool bExData)
{
	CShape::DecodeFromDataBlock(setReadDB, bExData);
	float fSpeedChangedRatio = setReadDB.GetFloatFromByteArray();
	SetActionSpeedRate(ACT_WALK,fSpeedChangedRatio);
	SetActionSpeedRate(ACT_RUN,fSpeedChangedRatio);
	m_bRun			= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	m_bHide					= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	m_bInvisible			= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	m_bPretendDead			= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	m_bNotRide				= (setReadDB.GetByteFromByteArray() == 0)?false : true;
	short m_sBeenAttack     = setReadDB.GetShortFromByteArray();
	setReadDB.GetBufferFromByteArray(m_PhaseID);                /// ������λ�ı���

	if(bExData)
	{
		m_bAntiInvisibleable	= (setReadDB.GetByteFromByteArray() == 0)?false : true;	
		m_bUseRabbSkill			= (setReadDB.GetByteFromByteArray() == 0)?false : true;
		m_bUseFighterSkill		= (setReadDB.GetByteFromByteArray() == 0)?false : true;
		m_bInstruction			= (setReadDB.GetByteFromByteArray() == 0)?false : true;
		m_bForceAttact			= (setReadDB.GetByteFromByteArray() == 0)?false : true;

		short m_sMoveable             = setReadDB.GetShortFromByteArray();
		short m_sRandomMove           = setReadDB.GetShortFromByteArray();
		short m_sSelfControl          = setReadDB.GetShortFromByteArray();
		short m_sMoveDamage           = setReadDB.GetShortFromByteArray();
		short m_sUseSkill             = setReadDB.GetShortFromByteArray();
	}

	m_lChangeGraphicstype	= setReadDB.GetLongFromByteArray();
	m_lChangeGraphicsID		= setReadDB.GetLongFromByteArray();
	BYTE bDied				= setReadDB.GetByteFromByteArray();
	if (bDied)
	{
		// ��������״̬
		CShape::SetState(STATE_DIED);
		CShape::SetAction(ACT_ALREADYDIED);
	}
	if(!bExData)
	{
		//��ѹbuff
		DecodeBuffFromDataBlock(setReadDB);
		//��ѹdebuff
		DecodeDeBuffFromDataBlock(setReadDB);
	}
	//��ѹ��������
	//DecodeActSkillFromDataBlock(setReadDB);
	//��ѹ���ؼ���
	if(bExData)
		DecodeOnOffSkillFromDataBlock(setReadDB);
	//�����ƶ�Ŀ���
	SetDestXY(GetPosX(),GetPosY());
	return true;
}
//��ѹbuff
bool CMoveShape::DecodeBuffFromDataBlock(DBReadSet& setReadDB)
{
	DWORD dwSize = setReadDB.GetDwordFromByteArray();
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  setReadDB.GetLongFromByteArray();
		long lLvl = setReadDB.GetLongFromByteArray();
		DWORD dwRemianTime = setReadDB.GetDwordFromByteArray();
		AddStateEx(lID, lLvl, dwRemianTime);
	}
	return true;
}
//��ѹdebuff
bool CMoveShape::DecodeDeBuffFromDataBlock(DBReadSet& setReadDB)
{
	DWORD dwSize = setReadDB.GetDwordFromByteArray();
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  setReadDB.GetLongFromByteArray();
		long lLvl = setReadDB.GetLongFromByteArray();
		DWORD dwRemianTime = setReadDB.GetDwordFromByteArray();
		AddStateEx(lID, lLvl, dwRemianTime);
	}
	return true;
}
//��ѹonoffskill
bool CMoveShape::DecodeOnOffSkillFromDataBlock(DBReadSet& setReadDB)
{
	DWORD dwSize = setReadDB.GetDwordFromByteArray();
	for(DWORD i =0;i<dwSize;i++)
	{
		long lID =  setReadDB.GetLongFromByteArray();
		long lLvl = setReadDB.GetLongFromByteArray();
	}
	return true;
}
//��ѹ����ʹ�ü���
bool CMoveShape::DecodeActSkillFromDataBlock(DBReadSet& setReadDB)
{
	bool bFlag = setReadDB.GetByteFromByteArray()==0?false:true;
	if(!bFlag)	return false;
	int num = setReadDB.GetLongFromByteArray();	// ����ʹ�õļ�����
	for (int i = 0 ; i< num; i++)
	{
		tagDecordSkillList skillList;
		skillList.lSkillKey = setReadDB.GetLongFromByteArray();
		skillList.lSkillID =  setReadDB.GetLongFromByteArray();
		skillList.lSkillLvl = setReadDB.GetLongFromByteArray();
		//��ǰִ�еĽ׶����
		skillList.lProceNum = setReadDB.GetLongFromByteArray();

		skillList.lMouseX = setReadDB.GetLongFromByteArray();
		skillList.lMouseY = setReadDB.GetLongFromByteArray();

		DWORD dwDestObjNum = setReadDB.GetDwordFromByteArray();
		CGUID guid;
		for(DWORD i=0;i<dwDestObjNum;i++)
		{
			skillList.vecType.push_back( setReadDB.GetLongFromByteArray());
			setReadDB.GetBufferFromByteArray(guid);
			skillList.vecID.push_back(guid);
		}
		m_vecDecordSkill.push_back(skillList);
	}

	return true;
}

/*
* ����: �趨��ɫ���⸽��Ч��
* ժҪ: �ó�Ա������������һ����ɫ������Ч��,���趨����Ҫ��״ֵ̬
* ����: dwLum		  - ���������ֵ0~100
*		 dwTimeLength - �������ʱ��(��λ:����)
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.10
*/
void CMoveShape::SetupFlashEffect(DWORD dwLum, DWORD dwTimeLength)
{
	m_listAddEffect[ADDEFFECT_FLASH].bPlayState   = true;
	m_listAddEffect[ADDEFFECT_FLASH].dwStartTime  = timeGetTime();
	m_listAddEffect[ADDEFFECT_FLASH].dwTimeLength = dwTimeLength;

	if (dwLum > 100)
		m_listAddEffect[ADDEFFECT_FLASH].dwValue = 100;
	else
		m_listAddEffect[ADDEFFECT_FLASH].dwValue = dwLum;
}



/*
* ����: �趨��ɫ�𶯸���Ч��
* ժҪ: �ó�Ա������������һ����ɫ����Ч��,���趨����Ҫ��״ֵ̬
* ����: dwExtent	  - �𶯷���ֵ(���������ֵ���ջᱻ��С100��,��ɸ���ֵ����ʵ��Ӧ��)
*		 dwTimeLength - �𶯳���ʱ��(��λ:����)
* ����ֵ: -
* ����: lpf
* ��������: 2008.01.10
*/
void CMoveShape::SetupShakeEffect(DWORD dwExtent, DWORD dwTimeLength)
{
	m_listAddEffect[ADDEFFECT_SHAKE].bPlayState   = true;
	m_listAddEffect[ADDEFFECT_SHAKE].dwStartTime  = timeGetTime();
	m_listAddEffect[ADDEFFECT_SHAKE].dwTimeLength = dwTimeLength;
	m_listAddEffect[ADDEFFECT_SHAKE].dwValue	  = dwExtent;

	m_fShakeMax = (float)dwExtent * 0.005f;			//(0.005f = 0.01f * 0.5f)
	m_fShakeMin = (float)dwExtent * 0.005f;

	srand((unsigned int)timeGetTime());
}



// ʹ�õ�ǰ����
void CMoveShape::UseCurSkill()
{
	if(m_vecDecordSkill.size()>0)
	{
		vector<tagDecordSkillList>::iterator it;
		for (it = m_vecDecordSkill.begin(); it != m_vecDecordSkill.end(); it ++)
		{
			if ((*it).lProceNum == 1)   // �����׶�
			{
				/***********************************************************************/
				/* �Ժ� fix */
				//CSkillXml *m_pSkill = new CSkillXml();
				/***********************************************************************/
				ConcreteSkillComp* SkillCom = new ConcreteSkillComp();
				ConcreteDecSkillXml* pSkill = new ConcreteDecSkillXml(SkillCom);
				/***********************************************************************/
				/* zhaohang fix */
				/***********************************************************************/
				CSkillListXml::tagSkill *pTagSkill = CSkillListXml::GetProperty((*it).lSkillID);
				if (pTagSkill && pTagSkill->dwSkillType == CSkillListXml::SKILLTYPE_PLAY)	// ֱ��ʩ��
				{
					// ֱ�Ӵӿ�ʼ�׶�ִ��
					if(pSkill->StepBegin((*it).lSkillID, (*it).lSkillLvl, GetType(), GetExID(), (*it).lMouseX, (*it).lMouseY,GetDirEx(),(*it).vecType, (*it).vecID))
					{
						pSkill->SetSkillKey((*it).lSkillKey);
						AddCurSkillList((*it).lSkillKey, pSkill);
					}
					else
						SAFE_DELETE(pSkill);
					// �����ʼ�׶γɹ���ֱ��ִ��ʩ�Ž׶�
					if(pSkill&&pSkill->StepRun((*it).lSkillID, (*it).lSkillLvl, GetType(), GetExID(), (*it).lMouseX, (*it).lMouseY,GetDirEx(),(*it).vecType, (*it).vecID) == false)
					{
						//pSkill->SetIsDeleted(true);
						DeleteCurSkill((*it).lSkillKey);
					}
				}
				/***********************************************************************/
				/* zhaohang fix */
				/***********************************************************************/
				else if(pTagSkill && pTagSkill->dwSkillType == CSkillListXml::SKILLTYPE_INTONATE) // ����ʩ��
				{
					if(pSkill->StepBegin((*it).lSkillID, (*it).lSkillLvl, GetType(), GetExID(), (*it).lMouseX, (*it).lMouseY,GetDirEx(),(*it).vecType, (*it).vecID))
					{
						pSkill->SetSkillKey((*it).lSkillKey);
						AddCurSkillList((*it).lSkillKey, pSkill);
					}
					else
						SAFE_DELETE(pSkill);
					if(GetType() == TYPE_PLAYER && GetGame()->GetMainPlayer()->GetExID() == GetExID())
					{
						// �����׶���������ƶ�ȡ������
						GetGame()->GetGameControl()->SetControlAble(true);
						GetGame()->GetGameControl()->SetMoveAble(true);
					}
				}
			}
			else					// ʩ�Ž׶�
			{
				/***********************************************************************/
				/* �Ժ� fix */
				//CSkillXml *m_pSkill = new CSkillXml();
				/***********************************************************************/
				ConcreteSkillComp* SkillCom = new ConcreteSkillComp();
				ConcreteDecSkillXml* pSkill = new ConcreteDecSkillXml(SkillCom);
				if(pSkill&&pSkill->StepRun((*it).lSkillID, (*it).lSkillLvl, GetType(), GetExID(), (*it).lMouseX, (*it).lMouseY,GetDirEx(),(*it).vecType, (*it).vecID))
				{
					pSkill->SetSkillKey((*it).lSkillKey);
					AddCurSkillList((*it).lSkillKey, pSkill);
				}
				else
				{
					SAFE_DELETE(pSkill);
				}
				if(GetType() == TYPE_PLAYER && GetGame()->GetMainPlayer()->GetExID() == GetExID())
				{
					GetGame()->GetGameControl()->SetControlAble(true);
				}
			}
		}
	}
}
// ������Shape��ľ��루��λ�����ӣ�
long CMoveShape::Distance(CShape* pShape)
{
	float x1, y1, x2, y2;
	if (GetAction()==ACT_WALK || GetAction()==ACT_RUN)
	{
		x1 = GetDestX();
		y1 = GetDestY();
	}
	else
	{
		x1 = GetPosX();
		y1 = GetPosY();
	}

	CMoveShape* pMoveShape = dynamic_cast<CMoveShape*>(pShape);
	if (pMoveShape)
	{
		if (pMoveShape->GetAction()==ACT_WALK || pMoveShape->GetAction()==ACT_RUN)
		{
			x2 = pMoveShape->GetDestX();
			y2 = pMoveShape->GetDestY();
		}
		else
		{
			x2 = pMoveShape->GetPosX();
			y2 = pMoveShape->GetPosY();
		}
	}

	return (long)max( abs(x1 - x2) - GetFigure(DIR_RIGHT) - pShape->GetFigure(DIR_RIGHT),
		abs(y1 - y2) - GetFigure(DIR_UP) - pShape->GetFigure(DIR_UP) );
}

// �Ƿ�����
bool CMoveShape::IsDied()
{
	if (GetState()==STATE_DIED)
	{
		return true;
	}
	return false;
}



/*
* ����: ������Ѫ�ı�
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: -
* ��������: -
* �޸���־:
*	2008.12.04 - lpf
*		������ַ�������ʱ,�ж������������Ҿ���ʾ����,������ʾ����.
*/
void CMoveShape::AddBloodText(const char* str,char cType, bool bBlastAttack, float fScale)
{
	tagBloodText st;
	st.strText = str;

	if ((unsigned char)str[0] <= 128)
	{
		if (GetGame()->GetMainPlayer() != (CPlayer *)this)
			st.strText.erase(0, 1);
	}

	st.fPosX = GetPosX(); 
	st.fPosY = GetPosY(); 
	st.fPos=2.0f; 
	st.lAlpha = 128;
	st.cType = cType;
	st.bBlastAttack = bBlastAttack;
	st.fScale = fScale;
	st.fCurScale = 0.5f;
	st.dwStartTime = timeGetTime();

	m_listBloodText.push_back(st);
}



// ȡ�ü����б���ĳһ������ʹ�õļ���
CSkillXml* CMoveShape::GetCurSkill(long lSkillKey)
{
	return m_mapCurSkill.find(lSkillKey)!=m_mapCurSkill.end()?m_mapCurSkill[lSkillKey]:NULL;
}
// ɾ�������б���ĳһ������
void CMoveShape::DeleteCurSkill(long lSkillKey)
{
	CSkillXml* pSkill = m_mapCurSkill.find(lSkillKey)!= m_mapCurSkill.end()? m_mapCurSkill[lSkillKey]:NULL;
	if(pSkill)
	{
		m_mapCurSkill.erase(lSkillKey);
		SAFE_DELETE(pSkill);
	}
}
// ɾ�������б�
void CMoveShape::DeleteCurSkillList()
{
	if (m_mapCurSkill.size()>0)
	{
		for (map<long,CSkillXml*>::iterator it= m_mapCurSkill.begin(); it!= m_mapCurSkill.end(); it++)
		{
			CSkillXml* pSkill = (CSkillXml*)it->second;
			if(pSkill)
				SAFE_DELETE(pSkill);
		}
		m_mapCurSkill.clear();
	}
	if(m_vecDecordSkill.size()>0)
		m_vecDecordSkill.clear();
}

// �쿴�Ƿ����ָ��ID�ļ���
bool CMoveShape::IsHaveSkillByIndex(DWORD dwID)
{
	for (map<long,CSkillXml*>::iterator it= m_mapCurSkill.begin(); it!= m_mapCurSkill.end(); it++)
	{
		CSkillXml* pSkill = (CSkillXml*)it->second;
		if(pSkill && pSkill->GetSkillID() == dwID)
		{
			return true;
		}
	}
	return false;
}

// ��ʾ����
void CMoveShape::SkillDisplay()
{
	if(m_mapCurSkill.size()>0)
	{
		map<long, CSkillXml*>::iterator it;
		for (it = m_mapCurSkill.begin(); it != m_mapCurSkill.end(); it++)
		{
			it->second->Display();
		}
	}
}


// ��ʱ���°󶨵�λ��
void CMoveShape::UpdataGetLocator()
{
	if(GetDisplayModel() && GetAnimInfo())
	{
		GetDisplayModel()->ProcessAnimJoint(GetAnimInfo());
		GetDisplayModel()->ProcessLocator(GetAnimInfo());
	}
}

//�õ�����ֵ�ӿ�
bool  CMoveShape::SetProperty(const char* attrName, long value)
{
	if(NULL == attrName)	return false;
	mapNameValueItr itr = m_mapNameValue.find(attrName);
	if(itr == m_mapNameValue.end())	return false;

	switch (itr->second.lType)
	{
	case 0:	// byte
		*(itr->second.plValue) = (BYTE)value;
		break;
	case 1:	// word
		*(itr->second.pwValue) = (WORD)value;
		break;
	case 2:	// dword
		*(itr->second.pdwValue) = (DWORD)value;
		break;
	case 3:	// float
		*(itr->second.pfValue) = (float)value;
		break;
	case 4:	// long
		*(itr->second.pValue) = (long)value;
		break;
	case 5:	// short
		*(itr->second.psValue) = (short)value;
		break;
	case 6:	// bool
		*(itr->second.pbValue) = value==0?false:true;
		break;
	}

	OnPropertyChanged(attrName);
	return true;
}

//������ֵ�ı��Ժ����
bool  CMoveShape::OnPropertyChanged(const char* attrName)
{
	if(NULL == attrName)	return false;

	if( strncmp(attrName,"CollectionID",12) == 0)
	{
		const CCollectionLayout* CollectLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
		if (CollectLayout!=NULL)
		{
			m_strCollectionName = CollectLayout->GetCollectionName();
		}
	}
	return true;
}

//��������ֵ�ӿ�
long  CMoveShape::GetProperty(const char* attrName)
{	
	if(NULL == attrName)	return 0;
	mapNameValueItr itr = m_mapNameValue.find(attrName);
	if(itr == m_mapNameValue.end())	return 0;

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
	return 0;
}

//�õ�����ֵ���ַ�����ʾ
const char* CMoveShape::GetPropertyStringValue(const char* pszPropertyName)
{
	static char pszString[1024]="";
	strncpy(pszString,"",1024);	
	if( NULL != pszPropertyName)
	{
		//����
		if( strncmp(pszPropertyName,"Name",4) == 0 )
			return GetName();
		else
		{
			mapNameValueItr itr = m_mapNameValue.find(pszPropertyName);
			if(itr != m_mapNameValue.end())
			{
				switch (itr->second.lType)
				{
				case 0:	// byte
					_snprintf(pszString,1024,"%d", *(itr->second.plValue));
					break;
				case 1:	// word
					_snprintf(pszString,1024,"%d", *(itr->second.pwValue));
					break;
				case 2:	// dword
					_snprintf(pszString,1024,"%u", *(itr->second.pdwValue));
					break;
				case 3:	// float
					_snprintf(pszString,1024,"%f", *(itr->second.pfValue));
					break;
				case 4:	// long
					_snprintf(pszString,1024,"%f", *(itr->second.pValue));
					break;
				case 5:	// short
					_snprintf(pszString,1024,"%f", *(itr->second.psValue));
					break;
				case 6:	// bool
					if( *(itr->second.pbValue) == false)
						_snprintf(pszString,1024,"��");
					else
						_snprintf(pszString,1024,"��");
					break;
				}
			}
		}
	}
	return pszString;
}


void CMoveShape::MoveTo( float xDest, float yDest )
{
	// Ѱ·
	m_FindPath.SetRegion( GetGame()->GetRegion() );
	m_FindPath.Find( (int)this->GetPosX(), (int)this->GetPosY(), (int)xDest, (int)yDest );
}


//���ݶ������Ը�ʽ���ַ���
//��ͨ���"{}"����ʽ���ַ��� ,�������ְ�����{}��
//����:"���{Name}�˳���Ϸ."
void CMoveShape::FormatStringWithProperty(const char* pszIn,char *pszOut,int nOutSize)
{
	if(NULL == pszIn || NULL == pszOut || nOutSize <= 0)	return;
	strncpy(pszOut,"",nOutSize);
	int InPos = 0;
	int OutPos = 0;
	char pszName[256] = "";
	int	nNamePos = 0;
	bool bGetName = false;
	//�ܳ���
	int len = (int)strnlen(pszIn,102400);
	while(InPos < len &&  OutPos < nOutSize)
	{
		char c = pszIn[InPos++];
		if(c == '{')
		{
			nNamePos = 0;
			bGetName = true;
		}
		else if(c == '}')
		{
			bGetName = false;
			pszName[nNamePos++] ='\0';
			//�õ�ֵ���ַ�
			const char* pszValueString = GetPropertyStringValue(pszName);
			if(pszValueString)
			{
				int nValueLen = (int)strnlen(pszValueString,1024);
				if(OutPos+nValueLen>=nOutSize)
					break;
				strcat(pszOut,pszValueString);
				OutPos += nValueLen;
			}
		}
		else
		{
			if(bGetName)
				pszName[nNamePos++] =c;
			else
				pszOut[OutPos++] = c;
		}
	}
	if(OutPos == nOutSize)	OutPos--;
	//�жϿ��ַ�
	if(OutPos >= 2 && (BYTE)pszOut[OutPos-1] >= 128 && (BYTE)pszOut[OutPos-2] < 128)
		OutPos--;
	pszOut[OutPos] = '\0';
}



/*
* ����: �趨���ű��鶯������Ҫ����Ϣ
* ժҪ: ���øú�������ʼ�����ű�������Ҫ��״̬��Ϣ
*		 ���û�б����ļ�����򲻴���,����Ҫ����
*		 ���鶯���Ľ��������������
* ����: pFaceAction - ��̬����ָ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.04.10
* �޸���־:
*	2008.06.05 - lpf
*		��������ʼ֡�ĸ�ֵ
*	2008.12.24 - lpf
*		�����˶�֡���ʱ�����õķ�����,��ֹ����ȷ�����õ��µĵ���
*/
void CMoveShape::SetupFaceAnim(tagFaceAction * pFaceAction)
{
	if (pFaceAction->dwFaceFileHandle != 0 && pFaceAction->dwFaceInterval != 0)
	{
		m_stCurrentFaceAnim.dwFaceCount		  = pFaceAction->dwFaceCount;
		m_stCurrentFaceAnim.dwFaceStartIndex  = pFaceAction->dwFaceStartIndex;
		m_stCurrentFaceAnim.dwFaceEndIndex    = pFaceAction->dwFaceEndIndex;
		m_stCurrentFaceAnim.dwFaceInterval    = pFaceAction->dwFaceInterval;
		m_stCurrentFaceAnim.dwFaceFileHandle  = pFaceAction->dwFaceFileHandle;

		m_stCurrentFaceAnim.bPlayState		   = true;
		m_stCurrentFaceAnim.dwFaceLoopCount	   = 0;
		m_stCurrentFaceAnim.dwFaceCurrentIndex = 0;
		m_stCurrentFaceAnim.dwStartTime		   = timeGetTime();
	}
}



/*
* ����: �趨���Ŷ�����������Ҫ����Ϣ
* ժҪ: ���øú�������ʼ�����Ŷ�������Ҫ��״̬��Ϣ
*		 ����һ����ִ��һ���С��������ı��鶯��ʱ�������Ҳ��Ǻ�ƽ״̬���ܲ��Ŷ�����
*		 �����������һ�����鶯���еĶ���ʱ������һ����վ������������ƽ��ս�����򡰣ɣģ̣š�����Ķ������������ڵı��鶯����
* ����: pFaceAction - ��̬����ָ��
* ����ֵ: -
* ����: lpf
* ��������: 2008.04.10
* �޸���־:
*	2008.11.11 - lpf
*		�������µĶԶ����б�Ĵ���
*	2008.12.26 - lpf
*		�����˶�����Ϣ��Ϊ��ǰ�����Ƿ�ѭ�����ŵ��ж�,Ŀ�����ڽ��Ŀǰ�ͻ���һֱ���ڵĶ������ֵ���Ե�����
*/
void CMoveShape::SetupActionAnim(tagFaceAction * pFaceAction)
{
	if (GetState() != CShape::STATE_PEACE ||
		pFaceAction->iActionIndex == -1 ||
		pFaceAction->sActionFileName == "0"/* ||
										   GetType() != TYPE_PLAYER*/)
										   return;

	tagActionList * pActionList = GetGame()->GetFaceActionList()->GetActionList(pFaceAction->iActionIndex);

	SetAction((WORD)atoi(pActionList->m_vecActionDecList[0].sAction.c_str()));
	//GetAnimInfo()->m_tActionInfo.bCurActionLooped = true;
	m_iActionCount = (int)pActionList->m_vecActionDecList[0].iLoopCount;
	if (m_iActionCount == -1 || m_iActionCount > 1)
		GetAnimInfo()->m_tActionInfo.bCurActionLooped = TRUE;
	else
		GetAnimInfo()->m_tActionInfo.bCurActionLooped = FALSE;

	m_stCurrentActionAnim.iActionIndex = pFaceAction->iActionIndex;
	m_stCurrentActionAnim.bActionPlayState = true;
	m_stCurrentActionAnim.bEffectPlayState = false;
	m_stCurrentActionAnim.dwActionCurrentIndex = 0;
	m_stCurrentActionAnim.dwStartTime = timeGetTime();
}



/*
* ����: ���ű��鶯��
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.04.10
* �޸���־:
*	2008.05.21 - lpf
*		�޸��˱��鶯��ͼƬ�Ĵ����߳ߴ���ԭ����64���ر�Ϊ32����
*	2008.06.05 - lpf
*		�޸���Ŀǰ��ͼƬ��Դ���Ź���,��������ʼ֡����
*		�������ڵı����ļ�����ߴ�Ϊ512x512(��֡�����سߴ粻��),�������������������Ϊ255
*/
void CMoveShape::PlayFaceAnim()
{
	if (!m_stCurrentFaceAnim.bPlayState)
		return;

	// ͷ������
	D3DXMATRIX  matFinal;
	D3DXVECTOR3 pos(GetPosY(), GetPickBoxHeight() + GetHeight(), GetPosX());
	D3DXVECTOR3 pos2;

	GetGame()->GetRegion()->CalculateFinalTransformMatrix(matFinal);
	D3DXVec3TransformCoord(&pos2, &pos, &matFinal);

	if (m_stCurrentFaceAnim.dwFaceLoopCount < m_stCurrentFaceAnim.dwFaceCount)
	{
		RECT  rc;
		DWORD dwCurrentIndex;
		DWORD dwCurrentTime = timeGetTime();
		DWORD dwElapseTime	= dwCurrentTime - m_stCurrentFaceAnim.dwStartTime;

		dwCurrentIndex = dwElapseTime / m_stCurrentFaceAnim.dwFaceInterval + m_stCurrentFaceAnim.dwFaceStartIndex;
		if (dwCurrentIndex > m_stCurrentFaceAnim.dwFaceEndIndex ||
			dwCurrentIndex > 255)
		{
			m_stCurrentFaceAnim.dwStartTime = dwCurrentTime;
			++m_stCurrentFaceAnim.dwFaceLoopCount;
		}else
		{
			rc.left	  = (dwCurrentIndex % 16) * 32;
			rc.top	  = (dwCurrentIndex / 16) * 32;
			rc.right  = rc.left + 32;
			rc.bottom = rc.top + 32;
			GameGraphics2D::GetInstance()->DrawImageSubRect((int)pos2.x - 16, (int)pos2.y - 70, &rc, m_stCurrentFaceAnim.dwFaceFileHandle);
		}
	}else
		m_stCurrentFaceAnim.bPlayState = false;
}



/*
* ����: ���Ŷ�������
* ժҪ: -
* ����: -
* ����ֵ: -
* ����: lpf
* ��������: 2008.11.05
* �޸���־:
*	2008.11.17 - lpf
*		�����õ���ЧIDΪ0����������˴���
*	2008.12.26 - lpf
*		�����˶�����Ϣ��Ϊ��ǰ�����Ƿ�ѭ�����ŵ��ж�,Ŀ�����ڽ��Ŀǰ�ͻ���һֱ���ڵĶ������ֵ���Ե�����
*/
void CMoveShape::PlayActionAnim()
{
	if (!m_stCurrentActionAnim.bActionPlayState)
	{
		if (m_stCurrentActionAnim.bEffectPlayState)
		{
			// ������Чȡ��ѭ����־,���´��߼��л��Զ�ɾ���ö���
			CEffect * pEffect = dynamic_cast<CEffect *>(FindEffectByIndex(m_stCurrentActionAnim.guidEffect));
			if (pEffect)
			{
				m_stCurrentActionAnim.guidEffect = CGUID::GUID_INVALID;
				pEffect->SetIsDeleted(true);
			}

			m_stCurrentActionAnim.bEffectPlayState = false;
		}
		return;
	}

	if (!m_stCurrentActionAnim.bEffectPlayState)
	{
		tagActionList * pActionList = GetGame()->GetFaceActionList()->GetActionList(m_stCurrentActionAnim.iActionIndex);
		if (timeGetTime() - m_stCurrentActionAnim.dwStartTime > pActionList->dwStartTime)
		{
			if (pActionList->dwIdEffect != 0)
			{
				m_stCurrentActionAnim.guidEffect = AddEffectLocator(pActionList->dwIdEffect, this, (eLOCATOR)pActionList->eLocator, pActionList->bIsLoop);
				m_stCurrentActionAnim.bEffectPlayState = true;
			}
		}
	}

	if (m_bDisplayFinish)
	{
		tagActionList * pActionList = GetGame()->GetFaceActionList()->GetActionList(m_stCurrentActionAnim.iActionIndex);
		m_stCurrentActionAnim.dwActionCurrentIndex++;

		if (m_stCurrentActionAnim.dwActionCurrentIndex < pActionList->m_vecActionDecList.size())
		{
			const char * pszAction = pActionList->m_vecActionDecList[m_stCurrentActionAnim.dwActionCurrentIndex].sAction.c_str();

			SetAction((WORD)atoi(pszAction));
			//GetAnimInfo()->m_tActionInfo.bCurActionLooped = true;
			m_iActionCount = (int)pActionList->m_vecActionDecList[m_stCurrentActionAnim.dwActionCurrentIndex].iLoopCount;
			if (m_iActionCount == -1 || m_iActionCount > 1)
				GetAnimInfo()->m_tActionInfo.bCurActionLooped = TRUE;
			else
				GetAnimInfo()->m_tActionInfo.bCurActionLooped = FALSE;
		}else
			m_stCurrentActionAnim.bActionPlayState = false;
	}else
	{
		if (GetAction() < CShape::ACT_FACEACTION || GetAction() > CShape::ACT_FACEACTIONMAX)
			m_stCurrentActionAnim.bActionPlayState = false;
	}
}

//���ű��鶯����Ӧ��Ч
void CMoveShape::PlayActionSound()
{
	if(GetType()!=TYPE_PLAYER) return;    //ֻ����ҵı��鶯��������Ч

	if(m_stCurrentActionAnim.bActionPlayState&&m_bPlayActionAudio)
	{
		if(!GetGame()->GetAudioList()->IsPlaying(GetActionAudio().c_str(),GetActionChannel()))
		{
			int iChannel = GetGame()->GetAudioList()->Play3DSound(GetActionAudio().c_str(),-1,GetPosY(), GetHeight(), GetPosX());
			SetActionChannel(iChannel);
		}
	}else{
		if(m_bPlayActionAudio==true)
		{
			GetGame()->GetAudioList()->Stop3DSound(GetActionAudio().c_str(),GetActionChannel());
			m_bPlayActionAudio = false;
		}
	}
}

//��ʼ�����鶯������Ч
void CMoveShape::InitActionAudio(DWORD dwFaceActionID)
{
	//ֹ֮ͣǰ���ڲ��ŵ���Ч
	GetGame()->GetAudioList()->Stop3DSound(GetActionAudio().c_str(),GetActionChannel());
	if(GetType()!=TYPE_PLAYER) return;    //ֻ����ҵı��鶯��������Ч
	tagFaceAction * stpFaceAction = NULL;
	stpFaceAction = GetGame()->GetFaceActionList()->GetPlayerFaceAction(dwFaceActionID);
	if(stpFaceAction->sActionFileName != "0")
	{
		char ActionAudio[256]="";
		sprintf(ActionAudio,"SOUNDS/Action/%d.wav",dwFaceActionID);
		SetActionAudio(ActionAudio);
		m_bPlayActionAudio = true;
	}
}


// �Ƿ�ﵽ��ǰʹ�ü��ܵĹ涨ʱ��
bool CMoveShape::IsCurSkillActTimeFinished()
{
	if(m_dwCurSkillStartTime > 0 && m_dwCurSkillActTime > 0)
	{
		DWORD dwTime = timeGetTime() - m_dwCurSkillStartTime;
		if(dwTime >= m_dwCurSkillActTime)
		{
			m_dwCurSkillStartTime = 0;
			m_dwCurSkillActTime = 0;
			return true;
		}
	}
	return false;
}


// ���˳�򲥷ŵļ���
CEffect* CMoveShape::AddEffectToList(long lGraphicsID, CShape* pHoster, eLOCATOR eLocator, 
									 bool bLoop, float fDir, long lID,bool bAsyn)
{
	CEffect* pEffect = NULL;
	CGUID guid = AddEffectLocator(lGraphicsID,pHoster,eLocator,false,fDir,lID,bAsyn);
	if(guid != CGUID::GUID_INVALID)
	{
		pEffect = FindEffectByIndex(guid);
		if(pEffect)
		{
			RemoveEffectByIndex(guid);
			//����Ч�Ѿ����ڣ���ɾ����Ȼ������ӵ����
			if(FindEffectByOrder(pEffect->GetGraphicsID(),pEffect->GetEffectID()))
				RemoveEffectByOrder(pEffect->GetGraphicsID(),pEffect->GetEffectID());
			m_lsEffectByOrder.push_back(pEffect);
		}
	}
	return pEffect;
}

// ����ָ��˳�򲥷ŵ���Ч
CEffect * CMoveShape::FindEffectByOrder(long lGraphicsID, long lID)
{
	list<CEffect*>::iterator it = m_lsEffectByOrder.begin();
	for (; it!=m_lsEffectByOrder.end(); it++)
	{
		if ((*it)->GetGraphicsID() == lGraphicsID && (*it)->GetEffectID() == lID)
		{
			CEffect* pEffect = (*it);
			return pEffect;
		}
	}
	return NULL;
}

// ɾ��ָ��˳�򲥷ŵ���Ч
void CMoveShape::DelEffectByOrder(long lGraphicsID, long lID)
{
	list<CEffect*>::iterator it = m_lsEffectByOrder.begin();
	for (; it!=m_lsEffectByOrder.end(); it++)
	{
		if ((*it)->GetGraphicsID() == lGraphicsID && (*it)->GetEffectID() == lID)
		{
			CEffect* pEffect = (*it);
			SAFE_DELETE(pEffect);
			m_listEffect.erase(it);
			return;
		}
	}
}

// �Ƴ�ָ��˳�򲥷ŵ���Ч
void CMoveShape::RemoveEffectByOrder(long lGraphicsID, long lID)
{
	list<CEffect*>::iterator it = m_lsEffectByOrder.begin();
	for (; it!=m_lsEffectByOrder.end(); it++)
	{
		if ((*it)->GetGraphicsID() == lGraphicsID && (*it)->GetEffectID() == lID)
		{
			CEffect* pEffect = (*it);
			m_listEffect.erase(it);
			return;
		}
	}
}

// ɾ��ȫ��˳�򲥷ŵ���Ч
void CMoveShape::DelAllEffectByOrder()
{
	list<CEffect*>::iterator it = m_lsEffectByOrder.begin();
	for (; it!=m_lsEffectByOrder.end();++it)
	{
		SAFE_DELETE((*it));
	}
	m_lsEffectByOrder.clear();
}

// ˳�򲥷ŵ���Ч
void CMoveShape::AIEffectByOrder()
{
	if(m_lsEffectByOrder.empty())
		return;
	CEffect* pEffect = m_lsEffectByOrder.front();
	AnimInfo::tagActionInfo *pActionInfo = pEffect->GetAnimInfo()->GetActionInfo();
	if(pActionInfo)
	{
		if (pEffect->GetIsDeleted())
		{
			m_lsEffectByOrder.erase(m_lsEffectByOrder.begin());
			AddEffectToList(pEffect->GetGraphicsID(),this,pEffect->GetHostLocator(),false,pEffect->GetDirEx(),pEffect->GetEffectID());
			pEffect = m_lsEffectByOrder.front();
			if(pEffect)
			{
				pActionInfo = pEffect->GetAnimInfo()->GetActionInfo();
				if(pActionInfo)
					pActionInfo->dwCurActionStartTime = timeGetTime();
			}
		}
		else
		{
			if(m_lsEffectByOrder.size() == 1)
				pEffect->SetLoop(true);
			else
				pEffect->SetLoop(false);
			pEffect->AI();
			//�жϽ���
			pEffect->m_bDisplayFinish = (pActionInfo->dwCurActionLoopCount > 0);
		}
	}
}

// ����˳�򲥷ŵ���Ч
void CMoveShape::DisPlayEffectByOrder()
{
	if(m_lsEffectByOrder.empty())
		return;
	static DWORD dwStartTime = timeGetTime();
	DWORD dwLostTime = timeGetTime() - dwStartTime;
	CEffect* pEffect = NULL;
	// �����Ч��ʱ��Ž���ʱ��ε�˳����ʾ
	//if(m_lsEffectByOrder.size() > 1)
	//{
	//	// ��1500�����ڲ��ŵ�һ����Ч
	//	if(dwLostTime > 1500)
	//	{
	//		dwStartTime = timeGetTime();
	//		pEffect = m_lsEffectByOrder.front();
	//		m_lsEffectByOrder.erase(m_lsEffectByOrder.begin());
	//		m_lsEffectByOrder.push_back(pEffect);
	//	}
	//}
	pEffect = m_lsEffectByOrder.front();
	if(pEffect)
	{
		if (!pEffect->GetIsDeleted())
			pEffect->Display();
	}
}

// ���������������ܵĿ�ʼʱ�������ʱ��
void CMoveShape::SetCurIntonateTime(DWORD dwTime,DWORD dwStartTime)
{
	m_dwIntonateTime = dwTime;
	m_dwIntonateStartTime = dwStartTime;
	if(m_dwIntonateTime == 0)
		m_bIntonateAlpha = 240;
	else
		m_bIntonateAlpha = 40;
}

// ��������������
void CMoveShape::UpdataIntonatebar()
{
	//CPlayer::tagSkill *pSkill = GetSkill(m_wIntonateSkillID);
	if(m_dwIntonateTime)
	{
		//ȡ������������ʱ��
		DWORD dwTimeElapse = timeGetTime() - m_dwIntonateStartTime;
		// ������
		if (m_dwIntonateStartTime > 0 && m_dwIntonateStartTime < 0xffffffff)
		{
			if (dwTimeElapse >= m_dwIntonateTime)
			{
				m_dwIntonateStartTime = 0;
				// ������ϣ����õݼ���alpha��ʼֵ
				m_bIntonateAlpha = 240;
			}
			// ���������У�������������alphaֵ
			else
			{
				if(m_bIntonateAlpha > 200)
					m_bIntonateAlpha = 255;
				else
					m_bIntonateAlpha += 40;
			}
		}
		else
		{
			m_dwIntonateTime = 0;
			m_dwIntonateStartTime = 0;
		}
	}
	else
	{
		m_dwIntonateStartTime = 0;
		if(m_bIntonateAlpha > 40)
			m_bIntonateAlpha -= 40;
		else
		{
			m_bIntonateAlpha = 0;
			m_wIntonateSkillID = 0;
		}
	}
}
// ����������
void CMoveShape::DisplayIntonate()
{
	if(m_wIntonateSkillID == 0)
		return;
	//CPlayer::tagSkill *pSkill = GetSkill(m_wIntonateSkillID);
	if(m_dwIntonateTime || m_bIntonateAlpha > 0)
	{
		if ((m_dwIntonateStartTime > 0 && m_dwIntonateStartTime < 0xffffffff) || m_bIntonateAlpha > 0)
		{
			//ȡ����������ʱ��
			DWORD dwTimeElapse = timeGetTime() - m_dwIntonateStartTime;
			float iIndex = (float)dwTimeElapse / m_dwIntonateTime;
			// ��������ʱ�䣬��������
			if(iIndex > 1.f && m_bIntonateAlpha == 0)
			{
				m_dwIntonateStartTime = 0;
				m_wIntonateSkillID = 0;
				return;
			}
			// ����������Դ
			if(GetGame()->GetIntanateBarBackBitmap() 
				&& GetGame()->GetIntanateBarBitmap()
				&& GetGame()->GetIntanateTimePotBitmap())
			{
				int w = GetGame()->GetIntanateBarBitmap()->GetWidth();
				int h = GetGame()->GetIntanateBarBitmap()->GetHeight();
				int iLen = 0;
				CSkillListXml::tagSkill *pS = CSkillListXml::GetProperty(m_wIntonateSkillID);
				if (pS != NULL)
					iLen = GetGame()->GetFontWidth(pS->strSkillName.c_str());
				static RECT rc;
				rc.left   =  0;
				rc.right  =  (int)(iIndex * w);
				rc.top    =  0;
				rc.bottom = h;
				// �����������
				if(!m_bIsLToR)
					rc.right = w - rc.right;
				DWORD dwColor = 0xffffffff;
				if(m_bIntonateAlpha < 255)
				{
					dwColor = m_bIntonateAlpha;
					dwColor = dwColor<<24 | 0xffffff;
				}
				int x = (SCREEN_WIDTH - GetGame()->GetIntanateBarBackBitmap()->GetWidth())/2;
				int y = SCREEN_HEIGHT*4 / 5;
				GetGame()->GetIntanateBarBackBitmap()->Draw(x,y,dwColor);
				GetGame()->GetIntanateBarBitmap()->SetColor(dwColor);
				GetGame()->GetIntanateBarBitmap()->Draw(x+8,y+7,rc);
				GetGame()->GetIntanateTimePotBitmap()->Draw(x+2+rc.right,y,dwColor);
				if(iLen!=0)
				{
					GetGame()->OutPutText((SCREEN_WIDTH-iLen)/2+1,y+5,pS->strSkillName.c_str(),iLen,dwColor & 0xff000000);
					GetGame()->OutPutText((SCREEN_WIDTH-iLen)/2,y+6,pS->strSkillName.c_str(),iLen,dwColor & 0xff000000);
					GetGame()->OutPutText((SCREEN_WIDTH-iLen)/2,y+5,pS->strSkillName.c_str(),iLen,dwColor & 0xfffabe32);
				}
			}
		}
	}
}