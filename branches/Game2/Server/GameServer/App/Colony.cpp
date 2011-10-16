//***********************************************
//		�ļ���	�� Colony.cpp
//		����	�� heliangchen
//		ʱ��	�� 08/4/14
//		���	�� ��Ⱥ�����ʵ��
//***********************************************

#include "StdAfx.h"
#include "Colony.h"
#include "MoveShape.h"
#include "Monster.h"
#include "ServerRegion.h"
//#include "AI/MonsterAI.h"

#include <complex>

//
CColony::CColony()
{
	//����
	m_iX=0;
	m_iY=0;
	//����
	m_iDir=0;
	//��ʼ����
	m_iInitializeDir=0;
	//·������
	m_iRoadDirect=-1;
	//���ߵ��¼
	m_iRoadRecord=1;
}

CColony::~CColony()
{}
//��ƽ״̬
void CColony::OnPeaceState(CMoveShape* pMoveShape)
{
	if (m_guidFactLeader!= pMoveShape->GetExID())	return;

	//���û��·�����򲻼���
	if (m_vectRoad.size()==0)	return;

	//����쵼�ߺͼ�Ⱥ���߼�λ�þ���̫Զ�� ���߼�λ�õȴ��쵼
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL)	return ;
	CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	if (pMonster== NULL) return ;
	POINT point=GetAimPoint(pMoveShape->GetExID());
	if (pMoveShape->Distance(point.x,point.y)> COLONY_LEADER_DISTANCE_MAX)	return;

	//���·��ֻ��һ����
	if (m_vectRoad.size()==1)
	{
		if (m_iX== m_vectRoad[0].x && m_iY== m_vectRoad[0].y)
		{
		}
		else
		{
			MoveTo(m_vectRoad[0].x,m_vectRoad[0].y);
		}
	}
	else	//·����
	{	
		INT iCurrentRunRecord=m_iRoadRecord;
		RunByRoad(m_iX,m_iY,iCurrentRunRecord);
		m_iRoadRecord=iCurrentRunRecord;
		CountLeaderAim();
		CountMemberAim();
	}

}
//ս��״̬
void CColony::OnFightState(CMoveShape* pMoveShape)
{
	if (m_guidFactLeader!= pMoveShape->GetExID())	return;
	SearchTarget();

	//if (HasTarget()==FALSE)	return;
	//CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	//if (pRegion== NULL)	return;

	//map<CGUID,STargetInfo>::iterator it= m_mapTargetList.begin();
	//for (; it!= m_mapTargetList.end();)
	//{
	//	CMoveShape* pTarget= dynamic_cast<CMoveShape*>(pRegion->FindChildObject(it->second.type,it->first));
	//	if (pTarget!=NULL)
	//	{
	//		LONG distance= pTarget->Distance(m_iX,m_iY);
	//		if (distance> TRACING_DISTANCE && it->second.dwHurtEffectTime<timeGetTime())
	//		{
	//			it= m_mapTargetList.erase(it);
	//		}
	//		else
	//		{
	//			it++;
	//		}
	//	}
	//	else
	//		it= m_mapTargetList.erase(it);
	//}
}

//����ɱ����ʱ��
void CColony::WhenBeenKilled(CMoveShape* pMonster)
{
	if (pMonster->GetExID()== m_guidFactLeader)
	{
		m_guidFactLeader=NULL_GUID;

		for (map<CGUID,tgMember>::iterator it= m_mapMember.begin();
			it!= m_mapMember.end(); it++)
		{
			CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
			CMonster* pMonster=dynamic_cast<CMonster*>(pRegion->FindChildObject(it->second.MemType,it->second.MemGuid));
			if (pMonster->GetState()!= CShape::STATE_DIED)
			{
				m_guidFactLeader= pMonster->GetExID();
				break;
			}
		}

		if (m_guidFactLeader== NULL_GUID)
		{
			m_mapTargetList.clear();
		}
	}
}

//��������ʱ��
void CColony::WhenBorn(CMoveShape* pMonster)
{
	if (m_guidFactLeader== NULL_GUID)
	{
		m_guidFactLeader= pMonster->GetExID();
		m_iX= pMonster->GetTileX();
		m_iY= pMonster->GetTileY();
	}
	else if (pMonster->GetExID()== m_tgLeader.MemGuid)
	{
		m_guidFactLeader= pMonster->GetExID();
	}
}

//����ս��״̬ת��Ϊ��ƽ״̬
BOOL CColony::CheckReturnPeace()
{
	if (m_mapTargetList.size()!=0)
	{
		return FALSE;
	}
	return TRUE;
}
//��ӳ�Ա
BOOL CColony::AddMember(BOOL bIsLeader, CMonster* pMonster)
{
	assert( "Not implemented" && 0 );
	return FALSE;
	//if (pMonster== NULL)
	//{
	//	return FALSE;
	//}
	//if (bIsLeader)
	//{
	//	if (m_tgLeader.MemGuid!= NULL_GUID)	return FALSE;
	//	m_tgLeader.MemType= pMonster->GetType();
	//	m_tgLeader.MemGuid= pMonster->GetExID();
	//	m_iX= pMonster->GetTileX();
	//	m_iY= pMonster->GetTileY();
	//	m_tgLeader.AimX=m_iX;
	//	m_tgLeader.AimY=m_iY;
	//	m_iDir= pMonster->GetDir();
	//	m_iInitializeDir= pMonster->GetDir();
	//	m_guidFactLeader= pMonster->GetExID();

	//	pMonster->SetColonyID(GetID());
	//	pMonster->SetMemberType(1);
	//	pMonster->SetColonyState(TRUE);


 //       // ����AI�����������ȱ�� Bugs 2008-9-12
 //       // ע�ͳ�����ش���� �����������޸ġ�

 //       // ����ɶ��ǹ���AI
 //       CMonsterAI *ai = (CMonsterAI*) (pMonster->GetAI ());
 //       if (ai && ai->GetRoad().size ())
	//	{
 //           m_vectRoad = ai->GetRoad();
 //       }

	//	for (map<CGUID,tgMember>::iterator it=m_mapMember .begin();
	//		it!= m_mapMember.end(); it++)
	//	{
	//		CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	//		if (pRegion!= NULL)
	//		{
	//			CMonster* pMember= dynamic_cast<CMonster*>(pRegion->FindChildObject(it->second.MemType,it->second.MemGuid));
	//			if (pMember!= NULL)
	//			{
	//				pMember->SetColonyState(TRUE);
	//			}
	//			if (pMember->Distance(pMonster)> pMonster->GetColonyDistance())
	//			{
	//				CMonsterAI* pMonsterAI= dynamic_cast<CMonsterAI*>(pMonster->GetAI()); //GetNearPosition();
	//				LONG x= pMonster->GetTileX();
	//				LONG y= pMonster->GetTileY();
	//				LONG dir= pMonster->GetDir();
	//				LONG searchdir= (dir+4)%8;
	//				if (pMonsterAI->GetNearPosition(x,y,dir,searchdir))
	//				{
	//					pMember->SetPosition(x,y);
	//					it->second.AimX=x;
	//					it->second.AimY=y;
	//				}
	//			}
	//		}
	//		it->second.OppoX= it->second.AimX - m_iX;
	//		it->second.OppoY= it->second.AimY - m_iY;

	//	}
	//	return TRUE;
	//}
	//else
	//{
 // 		map<CGUID,tgMember>::iterator it= m_mapMember.find(pMonster->GetExID());
	//	if (it != m_mapMember.end())
	//	{
	//		return FALSE;
	//	}


	//	tgMember member;
	//	member.MemType= pMonster->GetType();
	//	member.MemGuid= pMonster->GetExID();
	//	member.AimX= pMonster->GetTileX();
	//	member.AimY= pMonster->GetTileY();

	//	pMonster->SetColonyID(GetID());
	//	pMonster->SetMemberType(2);


	//	if (m_tgLeader.MemGuid!= NULL_GUID)
	//	{
	//		CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	//		CMonster* pLeader= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	//		if (pLeader!= NULL )
	//		{
	//			if (pMonster->Distance(pLeader)> pLeader->GetColonyDistance())
	//			{
	//				CMonsterAI* pMonsterAI= dynamic_cast<CMonsterAI*>(pMonster->GetAI()); //GetNearPosition();
	//				LONG x= pLeader->GetTileX();
	//				LONG y= pLeader->GetTileY();
	//				LONG dir= pLeader->GetDir();
	//				LONG searchdir= (dir+4)%8;
	//				if (pMonsterAI->GetNearPosition(x,y,dir,searchdir))
	//				{
	//					pMonster->SetPosition(x,y);
	//				}
	//				member.AimX= pMonster->GetTileX();
	//				member.AimY= pMonster->GetTileY();
	//			}
	//			member.OppoX= member.AimX -m_iX;
	//			member.OppoY= member.AimY -m_iY;
	//			pMonster->SetColonyState(TRUE);
	//		}
	//	}
	//	else
	//	{
	//		pMonster->SetColonyState(FALSE);
	//	}
	//	m_mapMember[pMonster->GetExID()]= member;
	//	return TRUE;
	//}
}
BOOL CColony::DelMember(CMonster* pMonster)
{
	if (pMonster!=NULL) return FALSE;
	if (pMonster->GetMemberType()==1)
	{
		m_tgLeader.Clean();
	}
	else
	{
		//for ()
		//{
		//}
	}
	return TRUE;

}
//���Ŀ��
//void CColony::AddTarget(CMonster* pMonster)
//{
//	if (pMonster!= NULL)
//	{
//		AddTarget(GetType(),pMonster->GetExID());
//	}
//}
//
//void CColony::AddTarget(int iType, const CGUID& guid)
//{
//	map<CGUID,STargetInfo>::iterator it= m_mapTargetList.find(guid);
//	if (it== m_mapTargetList.end())
//	{
//		STargetInfo info;
//		info.type= (OBJECT_TYPE)iType;
//		info.dwHurtEffectTime= timeGetTime()+ HURT_KEEP_TIME;
//		m_mapTargetList[guid ]= info;
//	}
//	else
//	{
//		it->second.dwHurtEffectTime= timeGetTime()+ HURT_KEEP_TIME;
//	}
//}

//��������Ŀ��
//void CColony::LoseAllTarget()
//{
//	map<CGUID,OBJECT_TYPE>::iterator it= m_mapTargetList.begin();
//	while (it!= m_mapTargetList.end())
//	{
//		it= m_mapTargetList.erase(it);
//	}
//}
//�Ƿ���·��
BOOL CColony::HasRoad()
{
	if (m_vectRoad.size()==0)	return FALSE;
	//if (m_vectRoad.size()==1 && m_iX==m_vectRoad[0].x && m_iY==m_vectRoad[0].y)
	//{
	//	return FALSE;
	//}
	return TRUE;
}
//ȡ��Ŀ���б�
CMoveShape* CColony::GetLeaderTarget()
{
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return NULL;

	CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	if (pMonster!= NULL&& pMonster->GetAI()!=NULL)
	{
		return pMonster->GetAI()->GetTarget();
	}
	return NULL;
}
//
//ȡ��ʵ������
CMonster* CColony::GetLeader()
{
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return NULL;
	CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	if (pMonster!= NULL && pMonster->GetState()!= CShape::STATE_DIED)
	{
		return pMonster;
	}
	return 0;
}
//ȡ��Ŀ���б�
BOOL CColony::GetTargetList(vector<CMoveShape*>& vecTarget)
{
	if (m_mapTargetList.size()==0)	return FALSE;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return FALSE;

	map<CGUID,STargetInfo>::iterator it= m_mapTargetList.begin();
	for (; it!= m_mapTargetList.end(); it++)
	{
		CMoveShape* pMoveShape= dynamic_cast<CMoveShape*>(pRegion->FindChildObject(it->second.type,it->first));
		if (pMoveShape!= NULL)
		{
			vecTarget.push_back(pMoveShape);
		}
	}
	return TRUE;
}
//ȡ�ó�Ա�б�
BOOL CColony::GetMemberList(vector<CMonster*>& vecMember)
{
	if (m_mapMember.size()==0)	return FALSE;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return FALSE;

	CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	if (pMonster!= NULL)
	{
		vecMember.push_back(pMonster);
	}

	map<CGUID,tgMember>::iterator it= m_mapMember.begin();
	for (; it!= m_mapMember.end(); it++)
	{
		pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(it->second.MemType,it->second.MemGuid));
		if (pMonster!= NULL)
		{
			vecMember.push_back(pMonster);
		}
	}
	return TRUE;
}
//ȡ��ս����Ա�б�
BOOL CColony::GetFightMemberList(vector<CMonster*>& vecMember)
{
	m_mapFightMember;

	if (m_mapFightMember.size()==0)	return FALSE;
	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return FALSE;

	map<CGUID,OBJECT_TYPE>::iterator it= m_mapFightMember.begin();
	for (; it!= m_mapFightMember.end(); it++)
	{
		CMonster* pMoveShape= dynamic_cast<CMonster*>(pRegion->FindChildObject(it->second,it->first));
		if (pMoveShape!= NULL)
		{
			vecMember.push_back(pMoveShape);
		}
	}
	return TRUE;
}
//ȡ��Ŀ������
POINT CColony::GetAimPoint(const CGUID& guid)
{
	POINT point;

	if (guid== m_tgLeader.MemGuid)
	{
		point.x= m_tgLeader.AimX;
		point.y= m_tgLeader.AimY;
	}
	else
	{
		map<CGUID,tgMember>::iterator it= m_mapMember.find(guid);
		if (it!= m_mapMember.end())
		{
			point.x= it->second.AimX;
			point.y= it->second.AimY;
		}
	}
	return point;
}

//ȡ���쵼������
POINT CColony::GetLeaderPoint()
{
	POINT pot;
	pot.x=0;pot.y=0;
	tgMember mem;
	if (m_guidFactLeader==m_tgLeader.MemGuid)
	{
		mem= m_tgLeader;
	}
	else
	{
		map<CGUID,tgMember>::iterator it= m_mapMember.find(m_guidFactLeader);
		if (it!= m_mapMember.end())
		{
			mem= it->second;
		}
		else
		{
			return pot;
		}
	}

	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return pot;
	CMoveShape* pMoveShape= dynamic_cast<CMoveShape*>(pRegion->FindChildObject(mem.MemType,mem.MemGuid));

	if (pMoveShape!=NULL)
	{
		pot.x= pMoveShape->GetTileX();
		pot.y= pMoveShape->GetTileY();
	}
	return pot;
}

void CColony::SearchTarget()
{
	m_mapTargetList.clear();
	m_mapFightMember.clear();

	CServerRegion* pRegion= dynamic_cast<CServerRegion*>(GetFather());
	if (pRegion== NULL) return;

	//m_tgLeader;
	CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(m_tgLeader.MemType,m_tgLeader.MemGuid));
	if (pMonster!= NULL&& pMonster->GetAI()!=NULL)
	{
		CMoveShape* pTarget= pMonster->GetAI()->GetTarget();
		if (pTarget!=NULL)
		{
			STargetInfo info;
			info.type= (OBJECT_TYPE)pTarget->GetType();
			map<CGUID,STargetInfo>::iterator it_= m_mapTargetList.find(pTarget->GetExID());
			if (it_== m_mapTargetList.end())
			{
				m_mapTargetList[pTarget->GetExID()]= info;
				m_mapFightMember[pMonster->GetExID()]= (OBJECT_TYPE)pMonster->GetType();
			}
		}

	}



	for (map<CGUID,tgMember>::iterator it= m_mapMember.begin();
		it!= m_mapMember.end(); it++)
	{
		CMonster* pMonster= dynamic_cast<CMonster*>(pRegion->FindChildObject(it->second.MemType,it->second.MemGuid));
		if (pMonster!= NULL&& pMonster->GetAI()!=NULL)
		{
			CMoveShape* pTarget= pMonster->GetAI()->GetTarget();
			if (pTarget==NULL)
			{
				continue;
			}
			STargetInfo info;
			info.type= (OBJECT_TYPE)pTarget->GetType();
			map<CGUID,STargetInfo>::iterator it_= m_mapTargetList.find(pTarget->GetExID());
			if (it_== m_mapTargetList.end())
			{
				m_mapTargetList[pTarget->GetExID()]= info;
				m_mapFightMember[pMonster->GetExID()]= (OBJECT_TYPE)pMonster->GetType();
			}
		}
	}
}

void CColony::CountLeaderAim()
{
	m_tgLeader.AimX= m_iX;
	m_tgLeader.AimY= m_iY;
}
void CColony::CountMemberAim()
{
	int iDirDis= (m_iDir-m_iInitializeDir)&7;
	for (map<CGUID,tgMember>::iterator it= m_mapMember.begin();
		it!= m_mapMember.end(); it++)
	{
		CountOneMenberAim(it->second.AimX,it->second.AimY,it->second.OppoX,it->second.OppoY,iDirDis);
	}
}

void CColony::CountOneMenberAim(int& AimX,int &AimY, int OppoX,int OppoY,int DirDis)
{
	static double ComplexList[8][2]=
	{	
		1,0,    0.7,0.7,	0,1,	-0.7,0.7,
		-1,0,	-0.7,-0.7,	0,-1,	0.7,-0.7,
	};
	int index=0;
	if (DirDis<0)
	{
		index= DirDis%8+8;
	}
	else
	{
		index= DirDis%8;
	}

	double real=ComplexList[index][0];
	double imag=ComplexList[index][1];

	complex<double>   complex_Oppo(OppoX,   OppoY);
	complex<double>   complex_Turn(real,    imag);
	complex_Turn*=complex_Oppo;

	AimX= m_iX+ (int)complex_Turn.real();
	AimY= m_iY+ (int)complex_Turn.imag();

}

int CColony::RunByRoad(LONG &lX, LONG &lY,INT & CurrentIndex)
{
	int countresult=0;
	int NextPointIndex=CurrentIndex + m_iRoadDirect;

	if ( NextPointIndex < 0 || NextPointIndex >= (int)m_vectRoad.size())
	{
		Log4c::Warn(ROOT_MODULE,"GS_COLONY_0");//CColony::RunByRoad,·������ԭ�� ����Խ��
		return NEXTSTEP_RESULT_NULL;
	}

	//�����ǰ�����Ѿ�������һ����¼
	if(lX==m_vectRoad[NextPointIndex].x && lY==m_vectRoad[NextPointIndex].y)
	{
		//��������¼������β��
		if(NextPointIndex==m_vectRoad.size()-1 || NextPointIndex==0)		
		{
			//�����·������β������
			int x1=m_vectRoad[NextPointIndex].x;
			int y1=m_vectRoad[NextPointIndex].y;
			int x2=m_vectRoad[(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size()].x;
			int y2=m_vectRoad[( NextPointIndex+m_iRoadDirect) %m_vectRoad.size()].y;

			if(m_vectRoad[NextPointIndex].x==m_vectRoad[(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size()].x
				&& m_vectRoad[NextPointIndex].y==m_vectRoad[(NextPointIndex+m_iRoadDirect+m_vectRoad.size())% m_vectRoad.size()].y)
			{
				countresult=NEXTSTEP_RESULT_NEXTCYCLE;
				CurrentIndex=(NextPointIndex+m_iRoadDirect+(int)m_vectRoad.size())% (int)m_vectRoad.size();
			}
			//������β����
			else
			{
				countresult=NEXTSTEP_RESULT_RETURN;
				CurrentIndex=NextPointIndex;
				m_iRoadRecord=CurrentIndex;
				m_iRoadDirect=-m_iRoadDirect;
			}

		}
		//������β��
		else																
		{
			countresult=NEXTSTEP_RESULT_NEXT;
			CurrentIndex=NextPointIndex;
		}
	}
	else
	{
		countresult=NEXTSTEP_RESULT_NULL;
	}

	LONG lPosX		= m_vectRoad[CurrentIndex+m_iRoadDirect].x;
	LONG lPosY		= m_vectRoad[CurrentIndex+m_iRoadDirect].y;
	//LONG lDir		= GetLineDir(  lX, lY ,lPosX, lPosY);
	//SetDirPos(lPosX,lPosY,lDir);
	MoveTo(lPosX,lPosY);

	return countresult;
}

void CColony::SetDirPos(int x,int y, int dir)
{
	m_iX=x;
	m_iY=y;
	m_iDir= dir;
}

void CColony::MoveTo(int x, int y)
{
	LONG lDir = GetLineDir(m_iX  ,m_iY  ,x, y);
	m_iDir=lDir;
	static int pos[8][2] = {0,-1, 1,-1, 1,0, 1,1, 0,1, -1,1, -1,0, -1,-1};
	m_iX = m_iX + pos[lDir][0];
	m_iY = m_iY + pos[lDir][1];
}