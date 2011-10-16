#include "StdAfx.h"
#include ".\moveshape.h"
#include "player.h"
#include "monster.h"
#include "Region.h"
#include "ClientRegion.h"
#include "Mover.h"
#include "..\Script\ScriptSys.h"
#include "..\GameTest\GameTestDlg.h"
class CBaseObejct;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//long CMoveShape::s_lIdleMinCount = 100;		// �������idle�����ļ���
//long CMoveShape::s_lIdleMaxCount = 5000;	// ��ٽ���ilde�����ļ���

//float	CMoveShape::highhurt_monster = 0.0f;
//float	CMoveShape::highhurt_boss = 0.0f;
//float	CMoveShape::highhurt_knight = 0.0f;
//float	CMoveShape::highhurt_bow = 0.0f;
//float	CMoveShape::highhurt_magic = 0.0f;
//float	CMoveShape::mildhurt_monster  = 0.0f;
//float	CMoveShape::mildhurt_boss  = 0.0f;
//float	CMoveShape::mildhurt_knight  = 0.0f;
//float	CMoveShape::mildhurt_bow  = 0.0f;
//float	CMoveShape::mildhurt_magic  = 0.0f;

CMoveShape::CMoveShape(void):
m_dwUsedTimes(0),
m_fBeginX(0.5f),
m_fBeginY(0.5f),
m_fDestX(0.0f),
m_fDestY(0.0f),
m_bIsArrive(true)
{
    m_mover = new Mover( this );
	m_PhaseID = NULL_GUID;
}

CMoveShape::~CMoveShape(void)
{
    delete m_mover;
}


void CMoveShape::SetState(WORD l)
{
	//CShape::SetState(l);
}

bool g_bForceUpdate = false;

void CMoveShape::SetAction(WORD l)
{
	if(l < ACT_STAND || (l>ACT_SEL_X5 && l<ACT_PERFORM) || l>ACT_MAX_PERFORM)
		return;
	if (m_wAction==l && g_bForceUpdate==false)
		return;

	g_bForceUpdate = false;
	m_wAction = l;

	// ������������赲
	if(  m_wAction == ACT_DIED )
	{
		if (GetDestX()>1.f || GetDestY()>1.f)
			SetBlock(GetDestX(), GetDestY(), CRegion::BLOCK_NO);
		else
			SetBlock(GetTileX(), GetTileY(), CRegion::BLOCK_NO);
	}
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
			data = _GetFloatFromByteArray(pByte,lPos);
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
		// 		itProETN it = m_PropertyEnumToName.find(pro_type);
		// 		if(it != m_PropertyEnumToName.end())
		// 		{
		// 			SetProperty( (*it).second.c_str(),data);
		// 		}

		SAFE_DELETE_ARRAY(byte_Array);
		SAFE_DELETE_ARRAY(word_Array);
		SAFE_DELETE_ARRAY(dword_Array);
		SAFE_DELETE_ARRAY(flaot_Array);
		SAFE_DELETE_ARRAY(long_Array);
		SAFE_DELETE_ARRAY(short_Array);
		SAFE_DELETE_ARRAY(char_Array);
		SAFE_DELETE_ARRAY(bool_Array);
	}
}

// �Ƿ���ĳ��״̬
bool CMoveShape::IsHaveStateEx(DWORD dwID)
{
	//list<tagStateEx>::iterator it;
	//for(it = m_listStateEx.begin(); it != m_listStateEx.end();it++)
	//{
	//	tagStateEx* pStateEx = &(*it);
	//	if (pStateEx && pStateEx->dwID == dwID)
	//		return true;
	//}

	return false;
}



//////////////////////////////////////////////////////////////////////////////////////////////////
//	Shape���ߴ���
//////////////////////////////////////////////////////////////////////////////////////////////////
// �ﵽ�������Ĵ���
long CMoveShape::OnArriveCell(long x, long y)
{
	return OnAttackEvent();
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

void CMoveShape::AddStateEx(DWORD dwID, DWORD dwLevel, DWORD dwStandingTime, DWORD dwAdditionalData)
{
	//list<tagStateEx>::iterator it;
	//for(it = m_listStateEx.begin();it!=m_listStateEx.end();it++)
	//{
	//	tagStateEx* pStateEx = &(*it);
	//	if(pStateEx&&pStateEx->dwID ==dwID && pStateEx->bEndSelf==false && pStateEx->dwLevel == dwLevel)
	//	{
	//		pStateEx->dwStartTime = GetCurTickCount();
	//		pStateEx->dwStandingTime = dwStandingTime;
	//		pStateEx->dwAdditionalData = dwAdditionalData;
	//		pStateEx->lCount++;
	//		return;
	//	}
	//}
	//tagStateEx st;
	//st.dwID = dwID;
	//st.dwLevel = dwLevel;
	//st.dwAdditionalData = dwAdditionalData;
	//st.dwStartTime = GetCurTickCount();
	//st.dwStandingTime =dwStandingTime;
	//st.bEndSelf = false;
	//st.lCount = 1;
	//m_listStateEx.push_back(st);
}

void CMoveShape::RemoveStateEx(DWORD dwID)
{
	//if ( dwID == 100004)
	//{
	//	return;
	//}
	//list<tagStateEx>::iterator it;
	//for(it = m_listStateEx.begin(); it != m_listStateEx.end(); it++)
	//{
	//	tagStateEx* pStateEx = &(*it);
	//	if (pStateEx && pStateEx->dwID==dwID)
	//	{
	//		//	if (pStateEx->dwID == STATE_PARTICULAR_STATE)
	//		//	{
	//		pStateEx->bEndSelf = true;
	//		//	}
	//		//	else
	//		//	{
	//		//		pStateEx->lCount--;
	//		//		if (pStateEx->lCount<=0)
	//		//			pStateEx->bEndSelf = true;
	//		//	}
	//		return;
	//	}
	//}
}

// ��������쳣״̬
void CMoveShape::ClearStateEx()
{
}

bool CMoveShape::IsInDieAwayState()
{/*
 if (IsHaveStateEx(81))
 return true;
 else */
	return false;
}

// ����
bool CMoveShape::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CShape::DecordFromByteArray(pByte, pos, bExData);
	float fSpeedChangedRatio = _GetFloatFromByteArray(pByte, pos);
	//	SetActionSpeedRate(ACT_WALK,fSpeedChangedRatio);
	//	SetActionSpeedRate(ACT_RUN,fSpeedChangedRatio);
	/*m_bRun			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bAntiInvisibleable	= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bInvisible			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bPretendDead			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bCanFight				= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bMoveable				= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bUseRabbSkill			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bUseFighterSkill		= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bUseItem				= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bInstruction			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bUnAttack				= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bAddHp				= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bHide					= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bForceAttact			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bRandomMove			= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_dwCollectID		= */_GetDwordFromByteArray(pByte, pos);
	//if (m_dwCollectID!=0)
	//{
	//const CCollectionLayout* CollectLayout = CDeputyOccuSystemClient::GetCollectionLayout(m_dwCollectID);
	//if (CollectLayout!=NULL)
	//{
	//	m_strCollectionName = CollectLayout->GetCollectionName();
	//}
	//}
	/*m_dwCollectTimes        = */_GetDwordFromByteArray(pByte, pos);
	/*m_bIsBeenCollect        = */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bCanBeenCollect		= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_bNotRide					= */(_GetByteFromByteArray(pByte, pos) == 0)?false : true;
	/*m_lChangeGraphicstype	=*/ _GetLongFromByteArray(pByte, pos);
	/*m_lChangeGraphicsID		= */_GetLongFromByteArray(pByte, pos);
	BYTE bDied				= (_GetByteFromByteArray(pByte, pos) == 0 ) ? false:true;
	if (bDied)
	{
		// ��������״̬
		CShape::SetState(STATE_DIED);
		CShape::SetAction(ACT_ALREADYDIED);
	}

	if( !bExData)
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

// ������Shape��ľ��루��λ�����ӣ�
long CMoveShape::Distance(CShape* pShape)
{
	long x1, y1, x2, y2;
	if (GetAction()==ACT_WALK || GetAction()==ACT_RUN)
	{
		x1 = GetDestX();
		y1 = GetDestY();
	}
	else
	{
		x1 = GetTileX();
		y1 = GetTileY();
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
			x2 = pMoveShape->GetTileX();
			y2 = pMoveShape->GetTileY();
		}
	}

	return max( abs(x1 - x2) - GetFigure(DIR_RIGHT) - pShape->GetFigure(DIR_RIGHT),
		abs(y1 - y2) - GetFigure(DIR_UP) - pShape->GetFigure(DIR_UP) );
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
		//		tagDecordSkillList skillList;
		/*skillList.lSkillKey = */_GetLongFromByteArray(pByte, pos);
		/*skillList.lSkillID =  */_GetLongFromByteArray(pByte, pos);
		/*skillList.lSkillLvl = */_GetLongFromByteArray(pByte, pos);
		//��ǰִ�еĽ׶����
		/*skillList.lProceNum = */_GetLongFromByteArray(pByte, pos);

		/*skillList.lObjectX = */_GetLongFromByteArray(pByte, pos);
		/*skillList.lObjectY = */_GetLongFromByteArray(pByte, pos);

		DWORD dwDestObjNum = _GetDwordFromByteArray(pByte, pos);
		CGUID guid;
		for(DWORD i=0;i<dwDestObjNum;i++)
		{
			_GetLongFromByteArray(pByte,pos);
			//skillList.vecType.push_back( _GetLongFromByteArray(pByte, pos));
			_GetBufferFromByteArray(pByte, pos,guid);
			//skillList.vecID.push_back(guid);
		}
		//m_vecDecordSkill.push_back(skillList);
	}

	return true;
}
////////////////////////////////////////////////////////////////////
//long CMoveShape::MoveDir(CMoveShape* pShape, long dir, WORD action,long SocketID)
//{
//	CClientRegion* m_pRegion = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetClientRegion();
//
//	bool bWalkAble	= false;	//�ɷ���·
//	bool bRunAble	= false;	//�ɷ���
//	long lOrDir		= dir;
//	long x			= pShape->GetTileX();
//	long y			= pShape->GetTileY();
//	long newx1		= x;
//	long newy1		= y;
//	long newx2		= x;
//	long newy2		= y;
//
//	for(int i=0; i<8; i++)
//	{
//		newx1 = x + _dir[_slip_order[dir][i]][0];
//		newy1 = y + _dir[_slip_order[dir][i]][1];
//		char s = m_pRegion->GetBlock(newx1, newy1);
//		if( s == CRegion::BLOCK_NO )
//		{
//			dir = pShape->GetDestDir(x,y, newx1, newy1);
//			bWalkAble = true;
//
//			if( dir != lOrDir && action == CShape::ACT_RUN )
//			{
//				action = CShape::ACT_WALK;
//			}
//			break;
//		}
//	}
//
//	if( action == CShape::ACT_RUN && bWalkAble == true)
//	{
//		newx2 = newx1 + _dir[lOrDir][0];
//		newy2 = newy1 + _dir[lOrDir][1];
//
//		if( m_pRegion->GetBlock(newx2, newy2) == CRegion::BLOCK_NO )
//		{
//			dir = pShape->GetDestDir(x,y, newx2, newy2);
//			bRunAble = true;
//		}
//	}
//
//	if( bRunAble == true )
//	{
//		action = CShape::ACT_RUN;
//	}
//	else if( bWalkAble == true )
//	{
//		action = CShape::ACT_WALK;
//	}
//
//	if( bWalkAble == true || bRunAble == true )
//	{
//		char s[32] = "C2SShapeMoveStep(";
//		sprintf(s,"%s%d,%d)",s,dir,action);
//		CScriptSys::getInstance()->PerformString(s);
//
//		pShape->SetAction(action);
//		pShape->SetDir(dir);
//
//		if( action == CShape::ACT_WALK )
//		{
//			pShape->SetDestXY(newx1+0.5f, newy1+0.5f);
//		}
//		else if( action == CShape::ACT_RUN )
//		{
//			pShape->SetDestXY(newx2+0.5f, newy2+0.5f);
//		}
//
//		// ���ÿ�ʼ�ƶ���ʱ��
//		pShape->SetBeginMoveTime(GetCurTickCount());
//		pShape->SetBeginXY(pShape->GetPosX(), pShape->GetPosY());
//		pShape->SetIsArrive(false);
//		return 1;
//	}
//	return 0;
//}

//�Ƿ�����
bool CMoveShape::IsDied()
{
	//if (GetState()==STATE_DIED)
	//{
	//	return true;
	//}
	return false;
}

// �������¼�
long CMoveShape::OnAttackEvent( )
{
	return NULL;
	//	while( !m_queueAttackEvent.empty() )
	//	{
	//		tagAttackEvent* p = &m_queueAttackEvent.front();
	//		switch(	p->cType )
	//		{
	//			// ��Ѫ�˺�
	//		case AE_DAMAGE:
	//			{
	//				SetState(STATE_FIGHT);
	//				SetHp(p->lFinalHP);
	////				char str[32];
	//				// ��������ǵ��������ٵ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if(this == mainPlayer)
	//				{
	//					for (int i = 0; i<(int)p->vtLost.size(); i++)
	//					{
	//						switch (p->vtLost[i].cLostType)
	//						{
	//						case 0:
	////						case DAMAGE_TYPE_HP:
	////							break;
	////						case DAMAGE_TYPE_MP:
	////							if (GetGame()->GetMainPlayer()->GetMp() >= p->vtLost[i].wLostPoint)
	////								GetGame()->GetMainPlayer()->SetMp(GetGame()->GetMainPlayer()->GetMp()-p->vtLost[i].wLostPoint);
	//							break;
	//						default:
	//							break;
	//						}
	//					}
	//				}
	//				// ������������
	//				else
	//				{	
	//				//	CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	//				//	if(pObjPage)
	//				//	{
	//				//		// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
	//				//		if(this==pObjPage->GetObjShape())
	//				//		{
	//				//			pObjPage->SetObjectHpInfo();
	//				//		}
	//				//	}
	//				}
	//				m_queueAttackEvent.pop();
	//			}
	//			break;
	//		case AE_SUCK:
	//			{
	//				SetState(STATE_FIGHT);
	//				if(GetAction()==ACT_STAND)
	//					SetAction(ACT_BLOCK);
	//				SetHp(p->lFinalHP);
	//				// ���ðѪ����
	////				char str[32];
	////				if(p->lTotalLostPoint == 0)
	////					sprintf(str,"");
	////				else
	////					sprintf(str, "%d", p->lTotalLostPoint);
	////				AddBloodText(str,AE_BLOCK, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				// ��������ǵ��������ٵ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if (this == mainPlayer)
	//				{
	//					for(int i=0; i<(int)p->vtLost.size(); i++)
	//					{
	//						//switch (p->vtLost[i].cLostType)
	//						//{
	//						//case DAMAGE_TYPE_HP:
	//						//	break;
	//						//case DAMAGE_TYPE_MP:
	//						//	if (GetGame()->GetMainPlayer()->GetMp() >= p->vtLost[i].wLostPoint)
	//						//		GetGame()->GetMainPlayer()->SetMp(GetGame()->GetMainPlayer()->GetMp()-p->vtLost[i].wLostPoint);
	//						//	//else
	//						//	;//GetGame()->GetMainPlayer()->SetMP(0);
	//						//	break;
	//						//}
	//					}
	//				//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
	//				}
	//				// ������������
	//				else
	//				{	
	//				//	CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	//				//	if(pObjPage)
	//				//	{
	//						// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
	//				//		if(this==pObjPage->GetObjShape())
	//				//		{
	//				//			pObjPage->SetObjectHpInfo();
	//				//		}
	//				//	}
	//				}
	//				m_queueAttackEvent.pop();
	//				// ������Ч
	//				long lGraphicID(1);
	//	//			float x, y, height;
	//	//			if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	//	//				GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir);
	//				// ������Ч
	//	//			char* strSound="sounds/xxx.wav";
	//	//			GetGame()->GetAudioList()->Play(strSound,GetPosX(), GetPosY(), GetHeight());
	//			}
	//			break;
	//			// ��ȫ����
	//		case AE_FULLMISS:
	//			{
	//				SetState(STATE_FIGHT);
	//				if(GetAction()==ACT_STAND)
	//					SetAction(ACT_MISS);
	//				// ͷ����ʾ��
	//		//		AddBloodText("����", AE_FULLMISS, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				m_queueAttackEvent.pop();
	//				long lGraphicID(1);
	////				float x, y, height;
	//				// ������Ч
	//		//		if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	//		//			GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height);
	//				// ��������
	//		//		char* strSound="sounds/xxx.wav";
	//		//		GetGame()->GetAudioList()->Play(strSound,GetPosX(),GetPosY(),GetHeight());
	//			}
	//			break;
	//			// �м�
	//		case AE_PARRY:
	//			{
	//				SetState(STATE_FIGHT);
	//				if(GetAction()==ACT_STAND)
	//					SetAction(ACT_PARRY);
	//				// ͷ����ʾ��
	//		//		AddBloodText("�м�", AE_PARRY, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				m_queueAttackEvent.pop();
	//				// �м���Ч
	//		//		long lGraphicID(1);
	//		//		float x, y, height;
	//		//		if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	//		//			GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir);
	//				// �м�����
	//		//		char* strSound="sounds/xxx.wav";
	//		//		GetGame()->GetAudioList()->Play(strSound,GetPosX(), GetPosY(),GetHeight());
	//			}
	//			break;
	//			// ��
	//		case AE_BLOCK:
	//			{
	//				SetState(STATE_FIGHT);
	//				if(GetAction()==ACT_STAND)
	//					SetAction(ACT_BLOCK);
	//				DWORD dwOldHP = GetHp();
	//				SetHp(p->lFinalHP);
	//				// ���ðѪ����
	//		//		char str[32];
	//		//		sprintf(str, "%d", p->lTotalLostPoint);
	//		//		AddBloodText(str,AE_BLOCK, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//
	//				// ��������ǵ��������ٵ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if (this == mainPlayer)
	//				{
	//					for(int i=0; i<(int)p->vtLost.size(); i++)
	//					{
	//						//switch (p->vtLost[i].cLostType)
	//						//{
	//						//case DAMAGE_TYPE_HP:
	//						//	break;
	//						//case DAMAGE_TYPE_MP:
	//						//	if (GetGame()->GetMainPlayer()->GetMp() >= p->vtLost[i].wLostPoint)
	//						//		GetGame()->GetMainPlayer()->SetMp(GetGame()->GetMainPlayer()->GetMp()-p->vtLost[i].wLostPoint);
	//						//	//else
	//						//	;//GetGame()->GetMainPlayer()->SetMP(0);
	//						//	break;
	//						//}
	//					}
	////					GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
	//				}
	//				m_queueAttackEvent.pop();
	//				// ����Ч
	////				long lGraphicID(1);
	////				float x, y, height;
	////				if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	////					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir);
	//				// ������
	////				char* strSound="sounds/xxx.wav";
	////				GetGame()->GetAudioList()->Play(strSound,GetPosX(), GetPosY(),GetHeight());
	//			}
	//			break;
	//			// �ֿ�
	//		case AE_RESIST:
	//			{
	//				SetState(STATE_FIGHT);
	//				// ͷ����ʾ��
	////				AddBloodText("�ֿ�",AE_RESIST, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				m_queueAttackEvent.pop();
	//				// �ֿ���Ч
	////				long lGraphicID(1);
	////				float x, y, height;
	////				if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	////					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir);
	//				// �ֿ�����
	////				char* strSound="sounds/xxx.wav";
	////				GetGame()->GetAudioList()->Play(strSound,GetPosX(), GetPosY(),GetHeight());
	//			}
	//			break;
	//			// ����
	//		case AE_REBOUND:
	//			{
	//				SetState(STATE_FIGHT);
	//				DWORD dwOldHP = GetHp();
	//				SetHp(p->lFinalHP);
	//				// ���ðѪ����
	////				char str[32];
	////				sprintf(str, "%d", p->lTotalLostPoint);
	////				AddBloodText(str,AE_REBOUND, p->bBlastAttack,(p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				// ��������ǵ��������ٵ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if (this == mainPlayer)
	//				{
	//					for(int i=0; i<(int)p->vtLost.size(); i++)
	//					{
	//						//switch (p->vtLost[i].cLostType)
	//						//{
	//						//case DAMAGE_TYPE_HP:
	//						//	break;
	//						//case DAMAGE_TYPE_MP:
	//						//	if (GetGame()->GetMainPlayer()->GetMp() >= p->vtLost[i].wLostPoint)
	//						//		GetGame()->GetMainPlayer()->SetMp(GetGame()->GetMainPlayer()->GetMp()-p->vtLost[i].wLostPoint);
	//						//	//else
	//						//	;//GetGame()->GetMainPlayer()->SetMP(0);
	//						//	break;
	//						//}
	//					}
	////					GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
	//				}
	//				m_queueAttackEvent.pop();
	//				// ������Ч
	////				long lGraphicID(1);
	////				float x, y, height;
	////				if(GetLocatorPos(LOCATOR_WIND, x, y, height))
	////					GetGame()->GetRegion()->PlayEffect(lGraphicID, x, y, height, p->fHurtedDir);
	//				// ��������
	////				char* strSound="sounds/xxx.wav";
	////				GetGame()->GetAudioList()->Play(strSound,GetPosX(), GetPosY(),GetHeight());
	//			}
	//			break;
	//
	//			// ����
	//		case AE_BREAK:
	//			{
	//				SetState(STATE_FIGHT);
	//				if(GetAction()==ACT_STAND)
	//					SetAction(ACT_BEATTACK);
	//
	//				DWORD dwOldHP = GetHp();
	//				SetHp(p->lFinalHP);
	//
	//				// ���ðѪ����
	////				char str[32];
	////				sprintf(str,"%d",p->lTotalLostPoint);
	////				AddBloodText(str, AE_BREAK, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//
	//				// ��������ǵ��������ٵ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if (this == mainPlayer)
	//				{
	//					for(int i=0; i<(int)p->vtLost.size(); i++)
	//					{
	//						//switch (p->vtLost[i].cLostType)
	//						//{
	//						//case DAMAGE_TYPE_HP:
	//						//	break;
	//						//case DAMAGE_TYPE_MP:
	//						//	if (GetGame()->GetMainPlayer()->GetMp() >= p->vtLost[i].wLostPoint)
	//						//		GetGame()->GetMainPlayer()->SetMp(GetGame()->GetMainPlayer()->GetMp()-p->vtLost[i].wLostPoint);
	//						//	else
	//						//		;//GetGame()->GetMainPlayer()->SetMP(0);
	//						//	break;
	//						//}
	//					}
	////					GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
	//				}
	//				// ������������
	//				else
	//				{	
	////					CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	////					if(pObjPage)
	////					{
	//						// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
	////						if(this==pObjPage->GetObjShape())
	////						{
	////							pObjPage->SetObjectHpInfo();
	////						}
	////					}
	//				}
	//
	//				m_queueAttackEvent.pop();
	//
	//				long lGraphiscID(1);
	////				long lSoundID(1);
	//				//CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(p->dwSkillID);
	//				//if(pSkill)
	//				//{
	//				//	bool  bFlashEffect		  = false;
	//				//	DWORD dwFlashEffectLum    = 0;
	//				//	DWORD dwFlashEffectTime   = 0;
	//				//	bool  bShakeEffect		  = false;
	//				//	DWORD dwShakeEffectExtent = 0;
	//				//	DWORD dwShakeEffectTime   = 0;
	//
	//				//	CSkillListXml::tagLevel pLevel = pSkill->vectorLevels[p->bySkillLevel-1];
	//				//	vector<CSkillListXml::tagLevelValue>::iterator pos;
	//				//	for(pos = pLevel.vecStepEnd.begin(); pos!= pLevel.vecStepEnd.end(); pos++)
	//				//	{
	//				//		// ��������������Ч
	//				//		if(!strcmp((*pos).strKeyWord.c_str(),"HitEffect"))
	//				//		{
	//				//			lGraphiscID = (*pos).dwValue;
	//				//			float x, y, height;
	//				//			if (GetLocatorPos(LOCATOR_WIND, x, y, height))
	//				//				GetGame()->GetRegion()->PlayEffect(lGraphiscID, x, y, height, p->fHurtedDir);
	//				//		}
	//				//		// �Ƿ��������Ч��
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffect"))
	//				//		{
	//				//			bFlashEffect = true;
	//				//		}
	//				//		// ��������
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffectLum"))
	//				//		{
	//				//			dwFlashEffectLum = (*pos).dwValue;
	//				//		}
	//				//		// �������ʱ��
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "FlashEffectTime"))
	//				//		{
	//				//			dwFlashEffectTime = (*pos).dwValue;
	//				//		}
	//				//		// �Ƿ������Ч��
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffect"))
	//				//		{
	//				//			bShakeEffect = true;
	//				//		}
	//				//		// ���Ž�ɫ��Ч��
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffectExtent"))
	//				//		{
	//				//			dwShakeEffectExtent = (*pos).dwValue;
	//				//		}
	//				//		// ���Ž�ɫ��Ч��
	//				//		else if (!strcmp((*pos).strKeyWord.c_str(), "ShakeEffectTime"))
	//				//		{
	//				//			dwShakeEffectTime = (*pos).dwValue;
	//				//		}
	//				//		// ���ų��������������������������������Ч�����������
	//				//		else if(!strcmp((*pos).strKeyWord.c_str(),"HitSound"))
	//				//		{
	//				//			lSoundID = (*pos).dwValue;
	//				//			char strSound[32];
	//				//			sprintf(strSound,"sounds/skills/%d.wav",lSoundID);
	//				//			GetGame()->GetAudioList()->Play(strSound, GetPosX(),GetPosY(),GetHeight());
	//				//		}
	//				//	}
	//
	//				//	// �趨����������Ч
	//				//	if (GetGame()->GetSetup()->bRoleFlash && bFlashEffect)
	//				//		SetupFlashEffect(dwFlashEffectLum, dwFlashEffectTime);
	//
	//				//	// �趨��������Ч
	//				//	if (GetGame()->GetSetup()->bRoleShake && bShakeEffect)
	//				//		SetupShakeEffect(dwShakeEffectExtent, dwShakeEffectTime);
	//				//}
	//
	//				//// �������˵ĳ̶Ȳ�����Ӧ����Ч
	//				//float fMildDamage=0;
	//				//float fHighDamage=0;
	//				//// ��Ҳ���ְҵ���Ա�ͳһ�ı��������˺��̶�
	//				//if ( GetType() == TYPE_PLAYER )
	//				//{
	//				//	fMildDamage = CMoveShape::mildhurt_knight;
	//				//	fHighDamage = CMoveShape::highhurt_knight;
	//				//}
	//
	//				////���������С�˺����Ǵ��˺�
	//				//if ( GetType() == TYPE_MONSTER )
	//				//{
	//				//	int iMonsterType = dynamic_cast<CMonster*>(this)->GetClass();
	//				//	switch ( iMonsterType )
	//				//	{
	//				//	case CMonster::MK_BOSS:
	//				//		{
	//				//			fMildDamage = CMoveShape::mildhurt_boss;
	//				//			fHighDamage = CMoveShape::highhurt_boss;
	//				//		}
	//				//		break;
	//
	//				//	default :
	//				//		{
	//				//			fMildDamage = CMoveShape::mildhurt_monster;
	//				//			fHighDamage = CMoveShape::highhurt_monster;
	//				//		}
	//				//		break;
	//				//	}
	//				//}
	//
	//				//// ���׷���
	//				//if ( GetType() == TYPE_PLAYER && dynamic_cast<CPlayer*>(this)->GetEquip(CPlayer::EQUIP_BODY) )
	//				//{
	//				//	CPlayer * pPlayer = dynamic_cast<CPlayer*>(this);					
	//				//	if ( p->lTotalLostPoint < (float)pPlayer->GetMaxHp()*fMildDamage )
	//				//	{
	//				//		// �����п��׷���
	//				//		GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_NORMAL, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
	//				//	}
	//				//	else if ( p->lTotalLostPoint > (float)pPlayer->GetMaxHp()*fHighDamage )
	//				//	{
	//				//		// �ػ��п��׷���
	//				//		GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_EXCEPT, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
	//				//		// �ҽ�
	//				//		GetGame()->GetAudioList()->Play(this,GetType(),ACT_BEATTACK, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//				//	}
	//				//	else
	//				//	{
	//				//		// �ػ��п��׷���
	//				//		GetGame()->GetAudioList()->Play(pPlayer->GetEquip(CPlayer::EQUIP_BODY),pPlayer->GetEquip(CPlayer::EQUIP_BODY)->GetType(),CGoods::GOOD_EXCEPT, Vector3(pPlayer->GetPosY(), pPlayer->GetHeight(), pPlayer->GetPosX()));
	//				//	}
	//				//}
	//				//else // ���� �� �ˣ���װ����
	//				//{
	//				//	enum
	//				//	{
	//				//		ACT_HIT01 = 8,			// ��ͨ
	//				//		ACT_HIT02				// ���ػ�		
	//				//	};
	//
	//				//	CMoveShape * pMoveShape = dynamic_cast<CMoveShape*>(this);
	//
	//				//	if ( p->lTotalLostPoint < (float)pMoveShape->GetMaxHp()*fMildDamage )
	//				//	{
	//				//		// ���˺�
	//				//		GetGame()->GetAudioList()->Play(this,GetType(),ACT_HIT01, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//				//	}
	//				//	else if ( p->lTotalLostPoint > (float)pMoveShape->GetMaxHp()*fHighDamage )	// ǿ�˺�
	//				//	{
	//				//		// �ػ��˺�
	//				//		GetGame()->GetAudioList()->Play(this,GetType(),ACT_HIT02, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//				//		// �ҽ�
	//				//		GetGame()->GetAudioList()->Play(this,GetType(),ACT_BEATTACK, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//				//	}
	//				//	else	// �ػ��˺�
	//				//	{
	//				//		GetGame()->GetAudioList()->Play(this,GetType(),ACT_HIT01, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//				//	}
	//				//}
	//
	//				return RET_AI_BREAK;
	//			}
	//			break;
	//
	//			// ����
	//		case AE_DIED:
	//			{
	//
	//				SetState(STATE_DIED);
	//				SetAction( ACT_DIED );
	//
	//				//// ðѪ
	//				//if(p->eDeathType==1)
	//				//{
	//				//	char str[32];
	//				//	sprintf(str,"%d",p->lTotalLostPoint);
	//				//	AddBloodText(str, AE_BREAK, p->bBlastAttack, (p->bBlastAttack)?CGlobeSetup::GetSetup()->fCriticalTextScale:1.0f);
	//				//}
	//
	//				SetHp(0);
	//				// ����ѡ��Ŀ����Ϣ
	//		//		CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	//				// ��Ŀ��shape�ǵ�ǰ���ѡ�е�
	//		//		if(pObjPage && this==pObjPage->GetObjShape())
	//		//		{
	//		//			pObjPage->SetObjectHpInfo();
	//		//		}
	//
	//				// �Լ�������
	//				CPlayer* mainPlayer = CScriptSys::getInstance()->GetTestDialog()->GetGameClient(SocketID)->GetMainPlayer();
	//				if( this == mainPlayer )
	//				{
	//			//		GetGame()->GetCGuiEx()->GetMainBarPageEx()->UpdatePlayerInforHar();
	//					// ��������������
	//			//		GetGame()->GetMainPlayer()->m_listGrowHP.clear();
	//			//		GetGame()->GetMainPlayer()->m_listGrowMP.clear();
	//					mainPlayer->m_listGrowHP.clear();
	//					mainPlayer->m_listGrowMP.clear();
	//				}
	//				//// ������������
	//				//else
	//				//{	
	//				//	CObjectPageEx *pObjPage = GetGame()->GetCGuiEx()->GetCObjectPageEx();
	//				//	if(pObjPage)
	//				//		pObjPage->Close();
	//				//}
	//
	//				//long lGraphiscID(1);
	//				//long lSoundID(1);
	//				//CSkillListXml::tagSkill *pSkill = CSkillListXml::GetProperty(p->dwSkillID);
	//				//if(pSkill)
	//				//{
	//				//	CSkillListXml::tagLevel pLevel = pSkill->vectorLevels[p->bySkillLevel-1];
	//				//	vector<CSkillListXml::tagLevelValue>::iterator pos;
	//				//	for(pos = pLevel.vecStepSend.begin(); pos!= pLevel.vecStepSend.end(); pos++)
	//				//	{
	//				//		// ��������������Ч
	//				//		if(!strcmp((*pos).strKeyWord.c_str(),"HitEffect"))
	//				//		{
	//				//			lGraphiscID = (*pos).dwValue;
	//				//			float x, y, height;
	//				//			if (GetLocatorPos(LOCATOR_WIND, x, y, height))
	//				//				GetGame()->GetRegion()->PlayEffect(lGraphiscID, x, y, height, p->fHurtedDir);
	//				//		}
	//				//		// ���ų��������������������������������Ч�����������
	//				//		else if(!strcmp((*pos).strKeyWord.c_str(),"HitSound"))
	//				//		{
	//				//			lSoundID = (*pos).dwValue;
	//				//			char strSound[32];
	//				//			sprintf(strSound,"sound/%d.wav",lSoundID);
	//				//			GetGame()->GetAudioList()->Play(strSound, GetPosX(),GetHeight(),GetPosX());
	//				//		}
	//				//	}
	//				//}
	//
	//				// ��ʾ������
	//				if (p->lHitNum!=-1)
	//				{
	////					GetGame()->GetMainPlayer()->SetHitNum(p->lHitNum);
	////					GetGame()->GetMainPlayer()->SetLastHitTimer(GetCurTickCount());
	//					mainPlayer->SetHitNum(p->lHitNum);
	//					mainPlayer->SetLastHitTimer(GetCurTickCount());
	//				}
	//
	//				// ����쳣״̬
	//				ClearStateEx();
	//
	//				while(!m_queueAttackEvent.empty())
	//					m_queueAttackEvent.pop();
	//
	////				GetGame()->GetAudioList()->Play(this,GetType(),ACT_DIED, Vector3(GetPosY(), GetHeight(), GetPosX()));
	//
	//				return RET_AI_BREAK;
	//			}
	//			break;
	//		}
	//		//if(!m_queueAttackEvent.empty())
	//		//	m_queueAttackEvent.pop();
	//	}
	//	return RET_AI_CONTINUE;
}
bool CMoveShape::SetProperty(const char* strName,long value)
{
	// 	if(NULL == strName)	return false;
	// 	mapNameValueItr itr = m_mapNameValue.find(strName);
	// 	if(itr == m_mapNameValue.end())	return false;
	// 
	// 	switch (itr->second.lType)
	// 	{
	// 	case 0:	// byte
	// 		*(itr->second.plValue) = (BYTE)value;
	// 		break;
	// 	case 1:	// word
	// 		*(itr->second.pwValue) = (WORD)value;
	// 		break;
	// 	case 2:	// dword
	// 		*(itr->second.pdwValue) = (DWORD)value;
	// 		break;
	// 	case 3:	// float
	// 		*(itr->second.pfValue) = (float)value;
	// 		break;
	// 	case 4:	// long
	// 		*(itr->second.pValue) = (long)value;
	// 		break;
	// 	case 5:	// short
	// 		*(itr->second.psValue) = (short)value;
	// 		break;
	// 	case 6:	// bool
	// 		*(itr->second.pbValue) = value==0?false:true;
	// 		break;
	// 	}
	// 
	// 	OnPropertyChanged(strName);
	return true;
}
//
bool CMoveShape::OnPropertyChanged(const char* strName)
{
	if( strName == NULL)	return false;
	//
	if( strncmp(strName,"CollectionID",12) == 0 )
	{
	}
	return true;
}
//
long CMoveShape::GetProperty(const char*attrName)
{
	// 	if(NULL == attrName)	return 0;
	// 	mapNameValueItr itr = m_mapNameValue.find(attrName);
	// 	if(itr == m_mapNameValue.end())	return 0;
	// 
	// 	switch (itr->second.lType)
	// 	{
	// 	case 0:	// byte
	// 		return *(itr->second.plValue);
	// 
	// 	case 1:	// word
	// 		return *(itr->second.pwValue);
	// 
	// 	case 2:	// dword
	// 		return *(itr->second.pdwValue);
	// 
	// 	case 3:	// float
	// 		return *(itr->second.pfValue);
	// 
	// 	case 4:	// long
	// 		return *(itr->second.pValue);
	// 
	// 	case 5:	// short
	// 		return *(itr->second.psValue);
	// 
	// 	case 6:	// bool
	// 		return *(itr->second.pbValue);
	// 	}
	return 0;
}
//�õ�����ֵ���ַ�����ʾ
const char* CMoveShape::GetPropertyStringValue(const char* pszPropertyName)
{
	return NULL;
	// 	static char pszString[1024]="";
	// 	strncpy(pszString,"",1024);	
	// 	if( NULL != pszPropertyName)
	// 	{
	// 		//����
	// 		if( strncmp(pszPropertyName,"Name",4) == 0 )
	// 			return GetName();
	// 		else
	// 		{
	// 			mapNameValueItr itr = m_mapNameValue.find(pszPropertyName);
	// 			if(itr != m_mapNameValue.end())
	// 			{
	// 				switch (itr->second.lType)
	// 				{
	// 				case 0:	// byte
	// 					_snprintf(pszString,1024,"%d", *(itr->second.plValue));
	// 					break;
	// 				case 1:	// word
	// 					_snprintf(pszString,1024,"%d", *(itr->second.pwValue));
	// 					break;
	// 				case 2:	// dword
	// 					_snprintf(pszString,1024,"%u", *(itr->second.pdwValue));
	// 					break;
	// 				case 3:	// float
	// 					_snprintf(pszString,1024,"%f", *(itr->second.pfValue));
	// 					break;
	// 				case 4:	// long
	// 					_snprintf(pszString,1024,"%f", *(itr->second.pValue));
	// 					break;
	// 				case 5:	// short
	// 					_snprintf(pszString,1024,"%f", *(itr->second.psValue));
	// 					break;
	// 				case 6:	// bool
	// 					if( *(itr->second.pbValue) == false)
	// 						_snprintf(pszString,1024,"��");
	// 					else
	// 						_snprintf(pszString,1024,"��");
	// 					break;
	// 				}
	// 			}
	// 		}
	// 	}
	// 	return pszString;
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
	int len = strnlen(pszIn,102400);
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
				int nValueLen = strnlen(pszValueString,1024);
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

void CMoveShape::InitNameValueMap()
{
	//tagPty st;// 0: byte 1:WORD 2:DWORD	3:float 4:long 5:short 6:bool
	//st.lType=3; st.pfValue=&m_fSpeed;				m_PropertyEnumToName[S_ATTR_SPEED]="Speed";	   					m_mapNameValue["Speed"] = st;
	//st.lType=1; st.pwValue=&m_wState;				m_PropertyEnumToName[S_ATTR_SHAPESTATE]="ShapeState";			m_mapNameValue["ShapeState"] = st;	
	//st.lType=6; st.pbValue=&m_bInvisible;			m_PropertyEnumToName[S_ATTR_INVISIBLETIME]="Invisible";			m_mapNameValue["Invisible"] = st;		//����
	//st.lType=6; st.pbValue=&m_bHide;				m_PropertyEnumToName[S_ATTR_HIDE]="Hide";						m_mapNameValue["Hide"] = st;			//����
	//st.lType=6; st.pbValue=&m_bPretendDead;			m_PropertyEnumToName[S_ATTR_PRETENDDEADTIME]="PretendDead";		m_mapNameValue["PretendDead"]=st;		//����
	//st.lType=4; st.pValue=&m_lChangeGraphicstype;	m_PropertyEnumToName[S_ATTR_CHANGEMODELTYPE]="ChangeModelType";	m_mapNameValue["ChangeModelType"]=st;	//�ı�����
	//st.lType=4; st.pValue=&m_lChangeGraphicsID;		m_PropertyEnumToName[S_ATTR_CHANGEMODELID]="ChangeModelID";		m_mapNameValue["ChangeModelID"]=st;	//�ı�ģ��id
	//st.lType=6; st.pbValue=&m_bInstruction;			m_PropertyEnumToName[S_ATTR_USEINSTRUCTION]="UnUseInstruction";	m_mapNameValue["UnUseInstruction"]=st;	//ʹ��ָ��
	//st.lType=6; st.pbValue=&m_bMoveable;			m_PropertyEnumToName[S_ATTR_MOVEABLE]="Moveable";				m_mapNameValue["Moveable"]=st;			//�ƶ�
	//st.lType=6; st.pbValue=&m_bCanFight;			m_PropertyEnumToName[S_ATTR_FIGHTABLE]="Fightable";				m_mapNameValue["Fightable"]=st;		//�ɷ񹥻�
	//st.lType=6; st.pbValue=&m_bUseFighterSkill;		m_PropertyEnumToName[S_ATTR_USEFIGHTSKILL]="UseFighterSkill";	m_mapNameValue["UseFighterSkill"]=st;	//ʹ��սʿ����
	//st.lType=6; st.pbValue=&m_bUseRabbSkill;		m_PropertyEnumToName[S_ATTR_USERABBSKILL]="UseRabbSkill";		m_mapNameValue["UseRabbSkill"]=st;		//ʹ�÷�ʦ����
	//st.lType=6; st.pbValue=&m_bUseItem;				m_PropertyEnumToName[S_ATTR_USEITEM]="UseItem";					m_mapNameValue["UseItem"]=st;			//ʹ�õ���
	//st.lType=6; st.pbValue=&m_bAntiInvisibleable;	m_PropertyEnumToName[S_ATTR_ANTIINVISIBLEABLE]="AntiInvisibleable";	m_mapNameValue["AntiInvisibleable"]=st;//������
	//st.lType=6; st.pbValue=&m_bAddHp;				m_PropertyEnumToName[S_ATTR_ADDHP]="AddHp";						m_mapNameValue["AddHp"]=st;		    //��������HP
	//st.lType=6; st.pbValue=&m_bUnAttack;			m_PropertyEnumToName[S_ATTR_UNATTACK]="UnAttack";				m_mapNameValue["UnAttack"]=st;		    //���ܱ�����
	//st.lType=6; st.pbValue=&m_bForceAttact;			m_PropertyEnumToName[S_ATTR_FORCEATTACK]="ForceAttack";			m_mapNameValue["ForceAttack"]=st;		//ǿ�ƹ���
	//st.lType=6; st.pbValue=&m_bRandomMove;			m_PropertyEnumToName[S_ATTR_RAMDOMMOVE]="RandomMove";			m_mapNameValue["RandomMove"]=st;		//����ܶ�

	//st.lType=2; st.pdwValue=&m_dwCollectID;			m_PropertyEnumToName[S_ATTR_COLLECTIONID]="CollectionID";		m_mapNameValue["CollectionID"]=st;		 //�ɼ���id
	//st.lType=2; st.pdwValue=&m_dwCollectTimes;		m_PropertyEnumToName[S_ATTR_COLLECTIONTIMES]="CollectionTimes";	m_mapNameValue["CollectionTimes"]=st;	 //�ܲɼ��Ĵ���
	//st.lType=6; st.pbValue=&m_bIsBeenCollect;		m_PropertyEnumToName[S_ATTR_ISBEENCOLLECT]="IsBeenCollect";		m_mapNameValue["IsBeenCollect"]=st;	 //�Ƿ����ڱ��ɼ�
	//st.lType=6; st.pbValue=&m_bCanBeenCollect;		m_PropertyEnumToName[S_ATTR_CANBEENCOLLECT]="CanBeenCollect";	m_mapNameValue["CanBeenCollect"]=st;	 //�ܷ񱻲ɼ�

	//st.lType=6; st.pbValue=&m_bNotRide;		m_PropertyEnumToName[S_ATTR_NOTRIDE]="Ride";	m_mapNameValue["Ride"]=st;	 //�Ƿ�����
}

//------------------------------------------------------------------------------------------------------
//	�¼ӽ���ӿ�
//------------------------------------------------------------------------------------------------------
bool CMoveShape::DecordFromDataBlock(DBReadSet& readDB,bool bExData /* = true */)
{
#pragma region
	if (CGameTestDlg::m_iProjName == CGameTestDlg::PROJ_XINGCHEN)
	{
		CShape::DecordFromDataBlock(readDB,bExData);
		float fSpeedChangeRatio = readDB.GetFloatFromByteArray();
		//	SetActionSpeedRate(ACT_WALK,fSpeedChangeRatio);
		//	SetActionSpeedRate(ACT_RUN,fSpeedChangeRatio);

		/*m_bRun			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bAntiInvisibleable					= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bInvisible			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bPretendDead			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bUnAttack				= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bNotRide				= */(readDB.GetByteFromByteArray() == 0)?false : true;

		if( bExData )
		{
			/*m_bAntiInvisibleable	= */(readDB.GetByteFromByteArray() == 0)?false : true;	
			/*m_bCanFight				= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bMoveable				= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bUseRabbSkill			= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bUseFighterSkill		= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bInstruction			= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bForceAttact			= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bRandomMove			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		}

		/*m_lChangeGraphicstype	= */readDB.GetLongFromByteArray();
		/*m_lChangeGraphicsID		= */readDB.GetLongFromByteArray();
		BYTE bDied				= readDB.GetByteFromByteArray() ==0 ? false : true;
		if (bDied)
		{
			// ��������״̬
			CShape::SetState(STATE_DIED);
			CShape::SetAction(ACT_ALREADYDIED);
		}
		if(!bExData)
		{
			//��ѹbuff
			DecordBuffFromDataBlock(readDB);
			//��ѹdebuff
			DecordDebuffFromDataBlock(readDB);
		}
		//��ѹ��������
		//DecordActSkillFromDataBlock(readDB);
		//��ѹ���ؼ���
		if(bExData)
			DecordOnOffSkillFromDataBlock(readDB);
		//�����ƶ�Ŀ���
		SetDestXY(GetPosX(),GetPosY());
		return true;
	}
#pragma endregion

#pragma region
	else if (CGameTestDlg::m_iProjName == CGameTestDlg::PROJ_PANLONG)
	{
		CShape::DecordFromDataBlock(readDB,bExData);
		float fSpeedChangeRatio = readDB.GetFloatFromByteArray();
		SetActionSpeedRate(ACT_WALK,fSpeedChangeRatio);
		SetActionSpeedRate(ACT_RUN,fSpeedChangeRatio);

		m_bRun					= (readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bHide					= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bInvisible			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		/*m_bPretendDead			= */(readDB.GetByteFromByteArray() == 0)?false : true;
		m_bNotRide				= (readDB.GetByteFromByteArray() == 0)?false : true;

		short m_sBeenAttack     = readDB.GetShortFromByteArray();
		readDB.GetBufferFromByteArray(m_PhaseID);  

		if( bExData )
		{
			/*m_bAntiInvisibleable	= */(readDB.GetByteFromByteArray() == 0)?false : true;	
			/*m_bUseRabbSkill			= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bUseFighterSkill		= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bInstruction			= */(readDB.GetByteFromByteArray() == 0)?false : true;
			/*m_bForceAttact			= */(readDB.GetByteFromByteArray() == 0)?false : true;

			short m_sMoveable             = readDB.GetShortFromByteArray();
			short m_sRandomMove           = readDB.GetShortFromByteArray();
			short m_sSelfControl          = readDB.GetShortFromByteArray();
			short m_sMoveDamage           = readDB.GetShortFromByteArray();
			short m_sUseSkill             = readDB.GetShortFromByteArray();
		}

		/*m_lChangeGraphicstype		= */readDB.GetLongFromByteArray();
		/*m_lChangeGraphicsID		= */readDB.GetLongFromByteArray();
		BYTE bDied						= readDB.GetByteFromByteArray() ==0 ? false : true;
		if (bDied)
		{
			// ��������״̬
			this->SetState(STATE_DIED);
			this->SetAction(ACT_ALREADYDIED);
		}
		if(!bExData)
		{
			//��ѹbuff
			DecordBuffFromDataBlock(readDB);
			//��ѹdebuff
			DecordDebuffFromDataBlock(readDB);
		}
		//��ѹ��������
		//DecordActSkillFromDataBlock(readDB);
		//��ѹ���ؼ���
		if(bExData)
			DecordOnOffSkillFromDataBlock(readDB);
		//�����ƶ�Ŀ���
		SetDestXY(GetPosX(),GetPosY());
		return true;
	}
#pragma endregion
	return true;
}




//��ѹbuff
bool CMoveShape::DecordBuffFromDataBlock(DBReadSet& setReadDB)
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
bool CMoveShape::DecordDebuffFromDataBlock(DBReadSet& setReadDB)
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
bool CMoveShape::DecordOnOffSkillFromDataBlock(DBReadSet& setReadDB)
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
bool CMoveShape::DecordActSkillFromDataBlock(DBReadSet& setReadDB)
{
	bool bFlag = setReadDB.GetByteFromByteArray()==0?false:true;
	if(!bFlag)	return false;
	int num = setReadDB.GetLongFromByteArray();	// ����ʹ�õļ�����
	for (int i = 0 ; i< num; i++)
	{
		//		tagDecordSkillList skillList;
		/*skillList.lSkillKey = */setReadDB.GetLongFromByteArray();
		/*skillList.lSkillID =  */setReadDB.GetLongFromByteArray();
		/*skillList.lSkillLvl = */setReadDB.GetLongFromByteArray();
		//��ǰִ�еĽ׶����
		/*skillList.lProceNum = */setReadDB.GetLongFromByteArray();

		/*skillList.lObjectX = */setReadDB.GetLongFromByteArray();
		/*skillList.lObjectY = */setReadDB.GetLongFromByteArray();

		DWORD dwDestObjNum = setReadDB.GetDwordFromByteArray();
		CGUID guid;
		for(DWORD i=0;i<dwDestObjNum;i++)
		{
			setReadDB.GetLongFromByteArray();
			//skillList.vecType.push_back( setReadDB.GetLongFromByteArray());
			setReadDB.GetBufferFromByteArray(guid);
			//skillList.vecID.push_back(guid);
		}
		//m_vecDecordSkill.push_back(skillList);
	}

	return true;
}
void CMoveShape::DecordChangePropertyFromDataBlock(DBReadSet& readDB)
{
	WORD lNum = readDB.GetWordFromByteArray();
	WORD pro_type = 0;
	BYTE data_type = 0;
	WORD data_length = 0;
	long data = 0;
	BYTE* byte_Array = NULL;
	WORD* word_Array = NULL;
	DWORD* dword_Array = NULL;
	float* float_Array = NULL;
	long* long_Array = NULL;
	short* short_Array = NULL;
	char* char_Array = NULL;
	bool* bool_Array = NULL;
	for(int i=0;i<lNum;i++)
	{
		pro_type = readDB.GetWordFromByteArray();
		data_type = readDB.GetByteFromByteArray();
		data_length = readDB.GetWordFromByteArray();
		if( data_length <=0 ) continue;

		switch(data_type)
		{
		case ATTR_DATA_BYTE:
			data = readDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_WORD:
			data = readDB.GetWordFromByteArray();
			break;
		case ATTR_DATA_DWORD:
			data = readDB.GetDwordFromByteArray();
			break;
		case ATTR_DATA_FLOAT:
			data = readDB.GetFloatFromByteArray();
			break;
		case ATTR_DATA_LONG:
			data = readDB.GetLongFromByteArray();
			break;
		case ATTR_DATA_SHORT:
			data = readDB.GetShortFromByteArray();
			break;
		case ATTR_DATA_BOOL:
			data = readDB.GetByteFromByteArray();
			break;
		case ATTR_DATA_BYTEARRAY:
			byte_Array = new BYTE[data_length];
			if (!byte_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				byte_Array[i] = readDB.GetByteFromByteArray();
			}
			break;
		case ATTR_DATA_WORDARRAY:
			word_Array = new WORD[data_length];
			if (!word_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				word_Array[i] = readDB.GetWordFromByteArray();
			}
			break;
		case ATTR_DATA_DWORDARRAY:
			dword_Array = new DWORD[data_length];
			if (!dword_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				dword_Array[i] = readDB.GetDwordFromByteArray();
			}
			break;
		case ATTR_DATA_FLOATARRAY:
			float_Array = new float[data_length];
			if (!float_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				float_Array[i] = readDB.GetFloatFromByteArray();
			}
			break;
		case ATTR_DATA_LONGARRAY:
			long_Array = new long[data_length];
			if (!long_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				long_Array[i] = readDB.GetLongFromByteArray();
			}
			break;
		case ATTR_DATA_SHORTARRAY:
			short_Array = new short[data_length];
			if (!short_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				short_Array[i] = readDB.GetShortFromByteArray();
			}
			break;
		case ATTR_DATA_CHARARRAY:
			char_Array = new char[data_length];
			if (!char_Array) return;
			readDB.GetStringFromByteArray(char_Array,data_length);
			break;
		case ATTR_DATA_BOOLARRAY:
			bool_Array = new bool[data_length];
			if (!bool_Array) return;
			for (int i = 0;i<data_length;i++)
			{
				data = readDB.GetByteFromByteArray();
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

		// 		itProETN it = m_PropertyEnumToName.find(pro_type);
		// 		if( it != m_PropertyEnumToName.end())
		// 		{
		// 			SetProperty((*it).second.c_str(),data);
		// 		}

		SAFE_DELETE_ARRAY(byte_Array);
		SAFE_DELETE_ARRAY(word_Array);
		SAFE_DELETE_ARRAY(dword_Array);
		SAFE_DELETE_ARRAY(float_Array);
		SAFE_DELETE_ARRAY(long_Array);
		SAFE_DELETE_ARRAY(short_Array);
		SAFE_DELETE_ARRAY(char_Array);
		SAFE_DELETE_ARRAY(bool_Array);
	}
}

void CMoveShape::RequestMove( float destX, float destY )
{
    m_mover->Move( destX, destY );
}

bool CMoveShape::UpdateMove()
{
    return m_mover->Update();
}

void CMoveShape::StopMove()
{
    m_mover->Stop();
}

long CMoveShape::OnTimeOut(DWORD timerid,DWORD curtime,const void* var)
{
    m_mover->OnTimeOut( timerid );
    return 0;
}



