///////////////////////////////////////////////////////////////
//Serverregion.cpp///////////////////////////////////////////////
//����ս��ͼ�࣬���𹥳�ս�Ĵ���
//Author��Joe
//Create Date:2004.7.19
#include "StdAfx.h"
#include "mmsystem.h"
#include ".\servercityregion.h"
#include "setup\monsterlist.h"
#include "Monster.h"
#include "Area.h"
#include "Player.h"
#include "build.h"
#include "citygate.h"
//#include "ai\guardai.h"
#include "ai\aifactory.h"
#include "..\..\setup\globesetup.h"
#include "appserver\Organizingsystem\attackcitysys.h"
#include "RgnManager.h"
#include "../GameServer/GameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServerCityRegion::CServerCityRegion(void)
:CServerWarRegion(0)
{
	m_pSymbol=NULL;
	m_CityGates.clear();
}
CServerCityRegion::CServerCityRegion(const CGUID& id)
:CServerWarRegion(id)
{
	m_pSymbol=NULL;
	m_CityGates.clear();
}
CServerCityRegion::CServerCityRegion(long id)
:CServerWarRegion(id)
{
	m_pSymbol=NULL;
	m_CityGates.clear();
}

CServerCityRegion::~CServerCityRegion(void)
{
}

// ��ӵ�CByteArray
bool CServerCityRegion::AddToByteArray(vector<BYTE>* pByteArray, bool bExData)
{
	CServerRegion::AddToByteArray(pByteArray, bExData);
	return true;
}
// ����
bool CServerCityRegion::DecordFromByteArray(BYTE* pByte, long& pos, bool bExData)
{
	CServerWarRegion::DecordFromByteArray(pByte,pos,bExData);
	char strTemp[256];

	//�õ����ط��Ļس�����
	_GetBufferFromByteArray(pByte, pos, &m_DefenceSideRS, sizeof(tagRegionSetup));

	//�õ����Ų���
	long lSize = _GetLongFromByteArray(pByte, pos);
	for(long l=0;l<lSize;l++)
	{
		tagBuild CityGate;
		_GetBufferFromByteArray(pByte, pos, &CityGate, sizeof(tagBuild)-sizeof(string)*2);
		CityGate.strName = _GetStringFromByteArray(pByte, pos, strTemp);
		CityGate.strScript = _GetStringFromByteArray(pByte, pos, strTemp);
		//���һ������
		AddCityGate(CityGate);
	}

	m_DefSideRSFactionID = GetOwnedCityFaction();
	return true;
}


// ��ȡ
BOOL CServerCityRegion::Save()
{
	return true;
}

// ���һ������6
CCityGate* CServerCityRegion::AddCityGate(tagBuild& st)
{
	CCityGate* pCityGate = NULL;
	CGUID guid;
	CGUID::CreateGUID(guid);
	/*pCityGate = (CCityGate*)CreateChildObject(TYPE_CITYGATE,guid, st.strName.c_str(), st.lPicID);
	if(pCityGate)
	{
		pCityGate->SetupProperty(st.lMaxHP,st.lDef,st.lElementResistant,st.lWidthInc,st.lHeightInc);
		pCityGate->SetDir(st.lDir);
		pCityGate->SetState(CShape::STATE_PEACE);
		pCityGate->SetPosXY(st.lTitleX+0.5f,st.lTitleY+0.5f);
		pCityGate->SetAction(st.lAction);
		if (!st.strScript.empty() && st.strScript != "0")
			pCityGate->SetScriptFile(st.strScript.c_str());

		tagCityGate CityGate = {st.lID,pCityGate->GetExID(),pCityGate,st.strName};
		m_CityGates[st.lID]= CityGate;
	}*/
	return pCityGate;
}

void CServerCityRegion::SetEnterPosXY(CPlayer*pPlayer)
{
	if( GetCityState() == CIS_Fight || GetCityState() == CIS_Mass )
	{
		long lX, lY;
		long region, l, t, r, b, d;
		region = l = t = r = b = d = -1;
		GetReturnPoint(pPlayer,region , l, t, r ,b ,d);

		//����ģ��RgnID����GUID
		long sx = l;
		long sy = t;
		long w = r - sx;
		long h = b - sy;

		GetRandomPosInRange(lX, lY, sx, sy, w, h);

		pPlayer->SetPosXY(lX+0.5f,lY+0.5f);
	}
}

void CServerCityRegion::SetOwnedCityOrg(const CGUID& FactionID,const CGUID& UnionID)
{
	CServerRegion::SetOwnedCityOrg(FactionID,UnionID);
	m_DefSideRSFactionID = FactionID;
}

void CServerCityRegion::AddGurdMonster(const CGUID& guid)
{
	if(m_GurdMonsters.find(guid) != m_GurdMonsters.end() ) return;
	m_GurdMonsters.insert(guid);
}

void	CServerCityRegion::AddGuardIndex(long lIndex)
{
	if (find(m_GuardIndexs.begin(),m_GuardIndexs.end(),lIndex) == m_GuardIndexs.end())
		m_GuardIndexs.push_back(lIndex);
}
// �Գ��Ž��в���
bool CServerCityRegion::OperatorCityGate(long lCityGateID,eOperCityGate eOp)
{
	map<long,tagCityGate>::iterator it = m_CityGates.find(lCityGateID);
	if(it == m_CityGates.end())	return false;
	CCityGate* pCityGate = (*it).second.pCityGate;
	if(pCityGate)
	{
		OperatorCityGate(pCityGate,eOp);
	}
	return true;
}

bool CServerCityRegion::OperatorCityGate(CCityGate* pCityGate,eOperCityGate eOp)
{
	if(pCityGate == NULL)	return false;
	switch(eOp)
	{
	case OC_Open:
		{
			pCityGate->SetAction(CShape::ACT_OPEN);
		}
		break;
	case OC_Close:
		{
			//�ж��Ƿ���Թر�
			if( CityGateIsClose(pCityGate) == false )	return false;

			pCityGate->SetAction(CShape::ACT_IDLE);
		}
		break;
	case OC_Refresh:
		{
			pCityGate->SetHP(pCityGate->GetMaxHP());
			pCityGate->SetAction(CShape::ACT_OPEN);
		}
		break;
	}
	return true;
}

//���³�����Ϣ���ͻ���
void CServerCityRegion::UpdateCityGateToClient(long lCityGateID)
{
	map<long,tagCityGate>::iterator it = m_CityGates.find(lCityGateID);
	if(it == m_CityGates.end())	return;
	CCityGate* pCityGate = (*it).second.pCityGate;
	if(pCityGate)
	{
		CMessage msg(MSG_S2C_SHAPE_Refresh);
		msg.Add(pCityGate->GetType());
		msg.Add(pCityGate->GetExID());
		msg.Add(pCityGate->GetAction());
		msg.Add(pCityGate->GetMaxHP());
		msg.Add(pCityGate->GetHP());
		SendMsgToAroundShape(pCityGate,&msg);
	}
}


//�ж�ĳ�������Ƿ���Թر�(�쿴�ó��ŵ�λ�����Ƿ�������shape)
bool	CServerCityRegion::CityGateIsClose(long lCityGateID)
{
	map<long,tagCityGate>::iterator it = m_CityGates.find(lCityGateID);
	if(it == m_CityGates.end())	return false;
	CCityGate* pCityGate = (*it).second.pCityGate;
	return CityGateIsClose(pCityGate);
}

//�ж�ĳ�������Ƿ���Թر�
bool	CServerCityRegion::CityGateIsClose(CCityGate* pCityGate)
{
	if(pCityGate == NULL)	return false;
	for (int i=pCityGate->GetTileX()-pCityGate->GetFigure(CShape::DIR_RIGHT);
		i<=pCityGate->GetTileX()+pCityGate->GetFigure(CShape::DIR_RIGHT); i++)
	{
		for (int j=pCityGate->GetTileY()-pCityGate->GetFigure(CShape::DIR_UP);
			j<=pCityGate->GetTileY()+pCityGate->GetFigure(CShape::DIR_UP); j++)
		{
			tagCell* pCell = GetCell(i,j);
			if( pCell && pCell->bBlock == BLOCK_SHAPE )
			{
				return false;
			}
		}
	}
	return true;
}

//�õ�ĳ�����ŵ�״̬
long	CServerCityRegion::GetCityGateState(long lCityGateID)
{
	long lRet = -1;
	map<long,tagCityGate>::iterator it = m_CityGates.find(lCityGateID);
	if(it == m_CityGates.end())	return lRet;
	CCityGate* pCityGate = (*it).second.pCityGate;
	if(pCityGate == NULL)	return lRet;
	if( pCityGate->GetAction() == CShape::ACT_OPEN )
		lRet = 0;
	else if( pCityGate->GetAction() == CShape::ACT_IDLE || pCityGate->GetAction() == CShape::ACT_STAND )
		lRet = 1;
	else if(pCityGate->GetState() == CShape::STATE_DIED )
		lRet = 2;
	return lRet;
}

// �Ա�־����в��� 
bool CServerCityRegion::OperatorSymbol(long lSymbolID,eOperCityGate eOp)
{
	if(eOp==OC_Refresh)
	{
		/*m_pSymbol->SetHP(m_pSymbol->GetMaxHP());
		m_pSymbol->SetAction(CShape::ACT_STAND);

		CMessage msg(MSG_S2C_SHAPE_Refresh);
		msg.Add(m_pSymbol->GetType());
		msg.Add(m_pSymbol->GetID());
		msg.Add(m_pSymbol->GetAction());
		msg.Add(m_pSymbol->GetMaxHP());
		msg.Add(m_pSymbol->GetHP());
		SendMsgToAroundShape(m_pSymbol,&msg);*/
	}
	return true;
}

//����־��ݻ�ʱ 
void CServerCityRegion::OnSymbolDestroy(long lSymID,long lPlayerID,long lFacID,const CGUID& UnionID)
{
	//if(lSymID != m_pSymbol->GetID())	return;
	//if(m_CityState != CIS_Fight) return;
	//if(m_lOwnedFactionID && lFacID && m_lOwnedFactionID == lFacID) return;
	//if(m_lOwnedUnionID && UnionID && m_lOwnedUnionID == UnionID) return;

	//m_CityState=CIS_DUTH;

	//CMessage msg(MSG_S2W_ATTACKCITY_OnResult);
	//msg.Add((long)1);												//��־�ﱻ����
	//msg.Add(GetID());
	//msg.Add(lPlayerID);
	//msg.Add(m_lOwnedFactionID);
	//msg.Send();

	//RefreshGuard(0);
}
//���ǿ�ʼ
void CServerCityRegion::OnWarStart(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	CServerWarRegion::OnWarStart(lWarNum);
	//���ս��������Ϣ
	char strWarInfo[256];
	_snprintf(strWarInfo, 256, "(Num:%d) %s ��ս�Ѿ���ʼ��",lWarNum,GetName());
	PutWarDebugString(strWarInfo);
}
//��ʼ��ս
void CServerCityRegion::OnWarDeclare(long lWarNum)
{
	CServerWarRegion::OnWarDeclare(lWarNum);

	m_DefSideRSFactionID = GetOwnedCityFaction();
	//���ս��������Ϣ
	char strWarInfo[256];
	_snprintf(strWarInfo, 256, "(Num:%d) %s ��ս�Ѿ���ʼ��ս��",lWarNum,GetName());
	PutWarDebugString(strWarInfo);
}
//���Ǽ���
void CServerCityRegion::OnWarMass(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	CServerWarRegion::OnWarMass(lWarNum);

	//���ս��������Ϣ
	char strWarInfo[256];
	_snprintf(strWarInfo,256, "(Num:%d) %s ��ս�ѿ�ʼ���ᡣ",lWarNum,GetName());
	PutWarDebugString(strWarInfo);
}

//����ʱ�����
void CServerCityRegion::OnWarTimeOut(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	map<CGUID,long>	TemptResult;
	for(map<long,CGUID>::iterator it = m_FacWinSymbol.begin(); it != m_FacWinSymbol.end();it++)
	{
		TemptResult[(*it).second]++;
	}

	for(map<CGUID,long>::iterator ite = TemptResult.begin(); ite != TemptResult.end();ite++)
	{
		if( (*ite).second >= m_lVicSymbolNum )
		{
			OnFactionVictory((*ite).first, NULL_GUID);

			//���ս��������Ϣ
			char strWarInfo[256];
			_snprintf(strWarInfo, 256, "11(Num:%d) %s ��սʱ���ѵ���",lWarNum,GetName());
			PutWarDebugString(strWarInfo);
			return;
		}
	}
	OnFactionVictory( GetOwnedCityFaction(),GetOwnedCityUnion() );

	//���ս��������Ϣ
	char strWarInfo[256];
	_snprintf(strWarInfo, 256, "22(Num:%d) %s ��սʱ���ѵ���",lWarNum,GetName());
	PutWarDebugString(strWarInfo);
}
//���ǽ���
void CServerCityRegion::OnWarEnd(long lWarNum)
{
	if( GetWarNum() != lWarNum)	return;

	if(GetCityState() == CIS_NO)	return;

	CServerWarRegion::OnWarEnd(lWarNum);

	ClearRegion();

	//���ս��������Ϣ
	char strWarInfo[256];
	_snprintf(strWarInfo, 256, "(Num:%d) %s ��ս������",lWarNum,GetName());
	PutWarDebugString(strWarInfo);
}
//���������Ա
void CServerCityRegion::OnClearOtherPlayer(long lWarNum)
{
	//����ǳ�������Ա�������
	KickOutAllPlayerToReturnPointExceptOwner();

	//�رճ���
	map<long,tagCityGate>::iterator it = m_CityGates.begin();
	for(; it != m_CityGates.end();it++)
	{
		OperatorCityGate((*it).first,OC_Refresh);
		OperatorCityGate((*it).first,OC_Close);
		UpdateCityGateToClient((*it).first);
	}

}
//ˢ�³���
void CServerCityRegion::OnRefreshRegion(long lWarNum)
{
	RefreshGuard();
}

void	CServerCityRegion::RefreshGuard( )
{
	set<CGUID>::iterator itG=m_GurdMonsters.begin();
	for(;itG != m_GurdMonsters.end();itG++)
	{
		CMonster* pMonster = FindMonsterByID((*itG));
		if(pMonster)
		{
			pMonster->SetHP(pMonster->GetMaxHP());
			if(pMonster->GetAI()) pMonster->GetAI()->WhenLoseTarget();

			CMessage msg(MSG_S2C_SHAPE_Refresh);
			msg.Add(pMonster->GetType());
			msg.Add(pMonster->GetExID());
			msg.Add(pMonster->GetAction());
			msg.Add(pMonster->GetMaxHP());
			msg.Add(pMonster->GetHP());
			SendMsgToAroundShape(pMonster,&msg);
		}

	}

	list<long>::iterator itI=m_GuardIndexs.begin();
	for(;itI!=m_GuardIndexs.end();itI++)
	{
		tagMonster* pst = GetMonsterRefeash((*itI));
		// ˢ��ʱ�䵽(��ˢ������Ϊp)���ȵ���NPC��AI���ִ�о����ˢ��
		if(pst  )
		{
			pst->dwLastResetTime = timeGetTime();

			// ˢ������������
			AddMonsterRect(*pst, pst->lNum - pst->lLivingCount);
		}
	}
}


void CServerCityRegion::GetReturnPoint(CPlayer* pPlayer, long &lRegion, long &l, long &t, long &r, long &b, long &d)
{
	if( !pPlayer )
	{
		l = t = r = b = d = 0;
		return;
	}

	tagCell* pCell = GetCell(pPlayer->GetTileX(),pPlayer->GetTileY());
	if( ( GetCityState() == CIS_Fight || GetCityState() == CIS_Mass) &&
		NULL_GUID != pPlayer->GetFactionID() && pPlayer->GetFactionID() == m_DefSideRSFactionID)
	{
		//lRegion = m_DefenceSideRS.lReturnRegionID;
		//l		= m_DefenceSideRS.rtReturnPoint.left;
		//t		= m_DefenceSideRS.rtReturnPoint.top;
		//r		= m_DefenceSideRS.rtReturnPoint.right;
		//b		= m_DefenceSideRS.rtReturnPoint.bottom;
		//d		= -1;
	}
	else
	{
		return CServerRegion::GetReturnPoint(pPlayer,lRegion,l,t,r,b,d);
	}
}

bool CServerCityRegion::SymbolIsAttackAble(CMoveShape* pAttackShape)
{
	if(GetCityState()==CIS_Fight)
	{
		/*if(pAttackShape->GetType() == TYPE_PLAYER)
		{
			CPlayer *pPlayer = (CPlayer*)pAttackShape;
			if((pPlayer->GetFactionID() && pPlayer->GetFactionID() != m_lOwnedFactionID) &&
				(pPlayer->GetUnionID()==0 || pPlayer->GetUnionID() != m_lOwnedUnionID))
			{
				return true;				
			}
		}*/
	}
	return false;
}

bool CServerCityRegion::GuardIsAttackAble(CMoveShape* pAttackShape)
{
	if(GetCityState()==CIS_Fight)
	{
		if(pAttackShape->GetType() == TYPE_PLAYER)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>( pAttackShape );
			if(pPlayer)
			{
				if( (NULL_GUID != GetOwnedCityFaction() && pPlayer->GetFactionID() == GetOwnedCityFaction()) ||
					(NULL_GUID != GetOwnedCityUnion() && pPlayer->GetUnionID() == GetOwnedCityUnion()) )
				{
					return false;				
				}
			}
		}
	}
	return true;
}

//CPlayer* CServerCityRegion::FindPlayerForGuard(CMonster* pMonster)
//{
//	if(!pMonster->GetAI())
//		return NULL;
//	if(pMonster->GetAI()->GetAIType()!=CAIFactory::GUARD_BASE)
//		return NULL;
//	CGuardAI* pGuardAI=dynamic_cast<CGuardAI*>(pMonster->GetAI());
//	if(!pGuardAI)
//		return NULL;
//
//	/*if(m_CityState!=CIS_Fight && m_lAttackCityGateType==TYPE_PLAYER)
//	{
//		CPlayer* pPlayer=(CPlayer*)(FindChildObject(m_lAttackCityGateType,m_lAttackCityGateID));
//		if(pPlayer &&
//			pPlayer->Distance(pGuardAI->GetGuardXPos(),pGuardAI->GetGuardYPos())<pMonster->GetBaseProperty()->dwChaseRange)
//		{
//			return pPlayer;
//		}
//		else
//		{
//			m_lAttackCityGateType=0;
//			m_lAttackCityGateID=0;
//		}
//	}*/
//
//	BEGIN_LOOP_AROUND_SHAPE(this, pMonster->GetArea()->GetX(), pMonster->GetArea()->GetY())
//	{
//		if( pShape->GetType() == TYPE_PLAYER && pShape->GetAction()!=CShape::ACT_DIED && pMonster!=pShape)
//		{
//			CPlayer* pPlayer=dynamic_cast<CPlayer*>(pShape);
//			if(m_CityState==CIS_Fight)
//			{
//				if(	(!pPlayer->GetFactionID() || pPlayer->GetFactionID() != m_lOwnedFactionID) &&
//					pPlayer->Distance(pGuardAI->GetGuardXPos(),pGuardAI->GetGuardYPos())<pMonster->GetBaseProperty()->dwChaseRange) 
//					return pPlayer;
//			}
//			else
//			{
//				if(pPlayer->GetPkValue() > 1 && 
//					(!pPlayer->GetFactionID() || pPlayer->GetFactionID() != m_lOwnedFactionID) &&
//					pPlayer->Distance(pGuardAI->GetGuardXPos(),pGuardAI->GetGuardYPos())<pMonster->GetBaseProperty()->dwChaseRange) 
//					return pPlayer;
//			}
//		}
//	}
//	END_LOOP_SHAPE
//		return NULL;
//}

CRegion::eSecurity CServerCityRegion::GetSecurity(int iTileX, int iTileY)
{
	if(GetCityState()==CIS_Mass) return SECURTIY_SAFE;

	tagCell* pCell  = GetCell( iTileX, iTileY );
	if(pCell)
	{
		if( pCell->bSecurity == SECURTIY_SAFE)
			return SECURTIY_SAFE;
		if(GetCityState() == CIS_Fight && pCell->lWar != 0)
		{
			return SECURITY_CITYWAR;
		}
		return (eSecurity)pCell->bSecurity;
	}
	return SECURTIY_SAFE;
}

long CServerCityRegion::GetDiedStateTime()
{
	if(GetCityState() == CIS_Fight)
		return CGlobeSetup::GetSetup()->lDiedStateTime*1000;
	return 0;
}

//�ж�ĳ������Ƿ�������
bool CServerCityRegion::IsOwner(const CGUID& FactionID)
{
	//�жϸ���ҵİ���Ƿ�����Ĳμ�
	if(NULL_GUID != FactionID && FactionID == GetOwnedCityFaction())
	{
		return true;
	}
	return false;
}

//ĳ������ʱ�䵽
void CServerCityRegion::OnContendTimeOver(tagContend Contend)
{
	CServerWarRegion::OnContendTimeOver(Contend);

}


//��һ�����Ӯ����һ����־�ﴥ��
void CServerCityRegion::OnFactionWinOneSymbol(const CGUID& FactionID,long lSymbolID)
{
	if(lSymbolID == 0)
	{
		m_DefSideRSFactionID = FactionID;
	}
}
//��һ������ʤʱ
void  CServerCityRegion::OnFactionVictory(const CGUID& FactionID,const CGUID& UnionID)
{
	if(GetCityState() == CIS_NO)	return;

	SetOwnedCityOrg(FactionID,UnionID);

	CServerWarRegion::OnFactionVictory(FactionID,UnionID);
	//CMessage msg(MSG_S2W_WAR_FactionWinCity);
	//msg.Add(GetWarNum());
	//msg.Add(GetID());
	//msg.Add(FactionID);
	//msg.Add(UnionID);
	//msg.Send();
}

//����ʱ�峡��
void CServerCityRegion::ClearRegion()
{
	CServerWarRegion::ClearRegion();
	//ˢ�³���
	map<long,tagCityGate>::iterator it = m_CityGates.begin();
	for(; it != m_CityGates.end();it++)
	{
		OperatorCityGate((*it).first,OC_Refresh);
		UpdateCityGateToClient((*it).first);
	}

	//�޳��Ƿ��ط���Ա�������
	KickOutAllPlayerToReturnPointExceptOwner();
}

bool CServerCityRegion::IsApplyWarFacsMem(const CGUID& FactionID)
{
	return GetAttackCitySys()->IsAlreadyDeclarForWar( GetWarNum(),FactionID );
}