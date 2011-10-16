/*Faction.cpp
*�޸���: ����
*�޸�����:2008-11-5
*/

#include "StdAfx.h"
#include "Faction.h"

#include "Player.h"
#include "Other/CountryList.h"
#include "Message.h"
#include "../GameClient/Game.h"
#include "ClientRegion.h"
#include "ClientWordsFilter.h"    
#include "../organizingsystem/Union.h"
#include "../MsgBoxSys/MsgEventManager.h"
#include "../../../Public/Common/OrganizingParam.h"
#include "../../../Public/Common/Public.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//**************************************************************************************
// ��Ա�� �߼�������
//**************************************************************************************

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember::CFacMember()
{
	ZeroMemory( &m_stMemberInfo, sizeof(m_stMemberInfo) );
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember::~CFacMember()
{
	ZeroMemory( &m_stMemberInfo, sizeof(m_stMemberInfo) );
}

/* 
*���ܣ����캯��
*ϸ�������� 
*������ pstMemberInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember::CFacMember( SMemberInfo* pstMemberInfo )
{
	this->SetMemberInfo( pstMemberInfo );
}


/* 
*���ܣ���ȡְҵ�ַ���
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CFacMember::GetOccuText() const
{
	if( this->GetOccupation() >= 0  &&  this->GetOccupation() < OCC_Max )
		return CPlayer::s_strOccupation[ this->GetOccupation() ].c_str();

	return "";
}



/*
*���ܣ�ǿ���������
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� -1 ����ǰ�棬1�ں��棬0��Ӱ��
*�޸�ʱ�䣺2008-4-16
*/
int CFacMember::CompareForceRule( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// �峤��һ
	if( Exceptive_JobLvl == pFacMember1->GetJob() )
	{
		return 1;
	}
	else if( Exceptive_JobLvl == pFacMember2->GetJob() )
	{
		return -1;
	}
	// �Լ��ڶ�
	else if( GetGame()->GetMainPlayer()->GetExID() == pFacMember1->GetGUID() )
	{
		return 1;
	}
	else if( GetGame()->GetMainPlayer()->GetExID() == pFacMember2->GetGUID() )
	{
		return -1;
	}

	return 0;
}



/*
*���ܣ���������Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareNormal( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// �ȸ����Ƿ���������
	if( pFacMember1->GetOnline() > pFacMember2->GetOnline() )
	{
		return true;
	}
	else if( pFacMember1->GetOnline() < pFacMember2->GetOnline() )
	{
		return false;
	}
	// �������ֱ����������
	else
	{
		return lstrcmp( pFacMember1->GetName(), pFacMember2->GetName() ) < 0;
	}

	return true;
}



/*
*���ܣ����� ����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByName( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ǿ�ƹ���
	if( int iForceResult = CFacMember::CompareForceRule( pFacMember1, pFacMember2 ) )
	{
		return iForceResult > 0;
	}
	// �����Ƿ���������
	else if( pFacMember1->GetOnline() > pFacMember2->GetOnline() )
	{
		return true;
	}
	else if( pFacMember1->GetOnline() < pFacMember2->GetOnline() )
	{
		return false;
	}

	return lstrcmp( pFacMember1->GetName(), pFacMember2->GetName() ) < 0;
}



/*
*���ܣ��ٽ�����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByJob( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ���չٽ�����
	if( pFacMember1->GetJob()  !=  pFacMember2->GetJob() )
	{
		return pFacMember1->GetJob()  >  pFacMember2->GetJob();
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}



/*
*���ܣ��ƺ�����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByTitle( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ���ճƺ�����
	if( int iTitleResult = lstrcmp( pFacMember1->GetTitle(), pFacMember2->GetTitle() ) )
	{
		return iTitleResult < 0;
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}




/*
*���ܣ���Ա�ȼ�����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByLevel( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ���յȼ�����
	if( pFacMember1->GetLevel()  !=  pFacMember2->GetLevel() )
	{
		return pFacMember1->GetLevel()  >  pFacMember2->GetLevel();
	}

	// �������ֱ����������
	return CFacMember::CompareByName( pFacMember1, pFacMember2 );
}




/*
*���ܣ���Ա״̬����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByState( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ����״̬����
	if( pFacMember1->GetState()  !=  pFacMember2->GetState() )
	{
		return pFacMember1->GetState()  >  pFacMember2->GetState();
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}




/*
*���ܣ���ͼ����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByRegion( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ���յ�ͼ����
	if( int iRegionResult = lstrcmp( pFacMember1->GetRegion(), pFacMember2->GetRegion() ) )
	{
		return iRegionResult < 0;
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}



/*
*���ܣ����� ����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByOnline( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// �����Ƿ���������
	if( pFacMember1->GetOnline() > pFacMember2->GetOnline() )
	{
		return true;
	}
	else if( pFacMember1->GetOnline() < pFacMember2->GetOnline() )
	{
		return false;
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}



/*
*���ܣ�ְҵ����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByOccupation( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ����ְҵ����
	if( pFacMember1->GetOccupation()  !=  pFacMember2->GetOccupation() )
	{
		return pFacMember1->GetOccupation()  <  pFacMember2->GetOccupation();
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}



/*
*���ܣ���������Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareByContribute( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ���չ�������
	if( pFacMember1->GetContribute()  !=  pFacMember2->GetContribute() )
	{
		return pFacMember1->GetContribute()  <  pFacMember2->GetContribute();
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}


/*
*���ܣ�ѡ������Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFacMember::CompareBySelection( const CFacMember* pFacMember1, const CFacMember* pFacMember2 )
{
	// ����ѡ������
	if( pFacMember1->GetSelected() > pFacMember2->GetSelected() )
	{
		return true;
	}
	else if( pFacMember1->GetSelected() < pFacMember2->GetSelected() )
	{
		return false;
	}

	// �����ݵȼ�����
	return CFacMember::CompareByLevel( pFacMember1, pFacMember2 );
}




//**************************************************************************************
// ����� �߼�������
//**************************************************************************************

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFaction::CFaction()
{
}


/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ pstFacInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFaction::CFaction( SFacInfo* pstFacInfo )
{
	this->SetFacInfo( pstFacInfo );
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFaction::~CFaction()
{
	this->ClearMembers();
	this->ClearApply();
}



/*
*���ܣ������Ա����
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::ClearMembers()
{
	for( std::list<CFacMember*>::iterator p = m_listMembers.begin(); p != m_listMembers.end(); p++ )
	{
		SAFE_DELETE( *p );
	}

	m_listMembers.clear();
}



/*
*���ܣ���������˺���
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::ClearApply()
{
	for( std::list<CFacMember*>::iterator p = m_listApply.begin(); p != m_listApply.end(); p++ )
	{
		SAFE_DELETE( *p );
	}

	m_listApply.clear();
}



/*
*���ܣ����ù��ܵȼ�
*ϸ�������� 
*������ eFun
*		level
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::SetLevel( ENUM_FUN eFun, long level )
{
	if( eFun >= 0  &&  eFun < MAXNUM_FUN )
	{
		m_stFacInfo.arrLevel[ eFun ] = level;
	}
}


/*
*���ܣ���ȡ���ܵȼ�
*ϸ�������� 
*������ eFun
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
long	CFaction::GetLevel( ENUM_FUN eFun ) const
{
	if( eFun >= 0  &&  eFun < MAXNUM_FUN )
	{
		return m_stFacInfo.arrLevel[ eFun ];
	}

	return -1;
}



/*
*���ܣ������峤�Ƿ�����
*ϸ�������� 
*������ 
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::SetOnline( bool bOnline )
{
	if( CFacMember* pMember = this->GetMember( this->GetMasterGUID() ) )
	{
		pMember->SetOnline( bOnline );
	}

	m_stFacInfo.bOnline = bOnline;
}



/*
*���ܣ���ȡ�峤�Ƿ�����
*ϸ�������� 
*������ 
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::GetOnline() const
{
	if( CFacMember* pMember = this->GetMember( this->GetMasterGUID() ) )
	{
		return pMember->GetOnline();
	}

	return m_stFacInfo.bOnline;
}



/*
*���ܣ����ùٽ�����
*ϸ�������� 
*������ lJobLvl
*		szJobName
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::SetJobName( long lJobLvl, LPCTSTR szJobName )
{
	if( lJobLvl > 0  &&  lJobLvl <= MAXNUM_JobLvl && szJobName )
	{
		m_stFacInfo.arrJobName[ lJobLvl -1 ] = szJobName;
	}
}


/*
*���ܣ���ȡ�ٽ�����
*ϸ�������� 
*������ lJobLvl
*����ֵ�� szJobName
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CFaction::GetJobName( long lJobLvl ) const
{
	if( lJobLvl > 0  &&  lJobLvl <= MAXNUM_JobLvl )
	{
		return m_stFacInfo.arrJobName[ lJobLvl -1 ].c_str();
	}

	return "";
}



/*
*���ܣ����ùٽ�Ȩ��
*ϸ�������� 
*������ lJobLvl
*		lJobPurview
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::SetJobPurview( long lJobLvl, long lJobPurview )
{
	if( lJobLvl > 0  &&  lJobLvl <= MAXNUM_JobLvl )
	{
		m_stFacInfo.arrJobPurview[ lJobLvl -1 ] = lJobPurview;
	}
}


/*
*���ܣ���ȡ�ٽ�Ȩ��
*ϸ�������� 
*������ lJobLvl
*		ePV
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::GetJobPurview( long lJobLvl, eFactionPurviewIndex ePV ) const
{
	// ����ȼ��� Ȩ�޵�Ӱ��
	if( this->GetLevel() < (long)(GetGame()->GetFactionManager()->GetFacLvlForPV( ePV )) )
	{
		return false;
	}
	// �峤ӵ������Ȩ��
	else if( Exceptive_JobLvl == lJobLvl )
	{
		return true;
	}
	// �ٽ�Ȩ��
	else if( lJobLvl > 0  &&  lJobLvl <= MAXNUM_JobLvl )
	{
		return ( m_stFacInfo.arrJobPurview[ lJobLvl -1 ] & ( 1 << ePV ) )  !=  0;
	}

	return false;
}



/*
*���ܣ����õжԼ���
*ϸ�������� 
*������ eBT
*		pEnemy
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFaction::SetEnemy( eBattleType eBT, tagEnemy* pEnemy )
{
	if( eBT >= 0 && eBT < eMaxWar )
	{
		for( std::vector<tagEnemy>::iterator p = m_vecWarInfo[ eBT ].begin(); p != m_vecWarInfo[ eBT ].end(); p++ )
		{
			if( ! lstrcmp( (*p)._cName, pEnemy->_cName ) )
			{
				(*p) = *pEnemy;
				return;
			}
		}

		m_vecWarInfo[ eBT ].push_back( *pEnemy );
	}
}


/*
*���ܣ���ȡ�жԼ���
*ϸ�������� 
*������ eBT
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
vector<tagEnemy>*	CFaction::GetEnemy( eBattleType eBT )
{
	if( eBT >= 0 && eBT < eMaxWar )
	{
		return &m_vecWarInfo[ eBT ];
	}

	return NULL;
}



/*
*���ܣ���ȡ��Ա����
*ϸ�������� 
*������ 
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
long	CFaction::GetNumOfMembers() const
{
	return m_stFacInfo.numMembers ? m_stFacInfo.numMembers : (long)m_listMembers.size();
}



/*
*���ܣ���ȡ������Ա����
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFaction::GetNumOfOnlineMembers() const
{
	if( m_stFacInfo.numOnlineMembers )
		return m_stFacInfo.numOnlineMembers;

	// 
	long num = 0;

	for( std::list<CFacMember*>::const_iterator p = m_listMembers.begin(); p != m_listMembers.end(); p++ )
	{
		if( (*p)->GetOnline() )
			num++;
	}

	return num;
}



/*
*���ܣ���ȡ�����Ա����
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFaction::GetMaxNumOfMembers()
{
	return GetGame()->GetFactionManager()->GetMaxNumByFacLv( this->GetLevel() );
}



/* 
*���ܣ���ȡ���Ҫ����Ա����
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFaction::GetRequireNumOfMembers()
{
	return GetGame()->GetFactionManager()->GetRequireNumByFacLv( this->GetLevel() );
}




/* 
*���ܣ����һ����Ա
*ϸ�������� 
*������ pstMemberInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::AddMember( CFacMember::SMemberInfo* pstMemberInfo )
{
	if( pstMemberInfo )
	{
		// �����Ա�Ƿ��Ѿ�����
		if( CFacMember* pFacMember = this->GetMember( pstMemberInfo->guidMember ) )
		{
			// ��������
			pFacMember->SetMemberInfo( pstMemberInfo );
		}
		else
		{
			// �������Ա
			m_listMembers.push_back( new CFacMember( pstMemberInfo ) );
		}

		// ���������������е����
		std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
		for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
		{
			if( (*p)->GetType() == TYPE_PLAYER )
			{
				if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
				{
					// �ҵ���Player���д���
					if( pPlayer->GetExID() == pstMemberInfo->guidMember )
					{
						pPlayer->SetFactionID( this->GetGUID() );
						pPlayer->SetFactionName( string( this->GetName() ) );

						pPlayer->SetUnionID( this->GetUnionGUID() );

						break;
					}
				}
			}
		}

		return true;
	}

	return false;
}



/* 
*���ܣ�ɾ��һ����Ա
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::DelMember( CGUID guid )
{
	// ���������������е����
	std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
	for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
	{
		if( (*p)->GetType() == TYPE_PLAYER )
		{
			if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
			{
				// �ҵ���Player���д���
				if( pPlayer->GetExID() == guid )
				{
					pPlayer->SetFactionID( NULL_GUID );
					pPlayer->SetUnionID( NULL_GUID );
					pPlayer->SetFactionName( string("") );
					pPlayer->SetFactionTitle( string("") );

					break;
				}
			}
		}
	}

	for( std::list<CFacMember*>::iterator p = m_listMembers.begin(); p != m_listMembers.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			SAFE_DELETE( *p );
			m_listMembers.erase( p );

			return true;
		}
	}

	return false;
}



/* 
*���ܣ��Ƿ�ӵ��һ����Ա
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::HasMember( CGUID guid ) const
{
	return this->GetMember( guid )  !=  NULL;
}



/* 
*���ܣ���ȡһ����Ա
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFaction::GetMember( CGUID guid ) const
{
	for( std::list<CFacMember*>::const_iterator p = m_listMembers.begin(); p != m_listMembers.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			return *p;
		}
	}

	return NULL;
}



/* 
*���ܣ���ȡһ����Ա
*ϸ�������� 
*������ szName
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFaction::GetMember( LPCTSTR szName ) const
{
	for( std::list<CFacMember*>::const_iterator p = m_listMembers.begin(); p != m_listMembers.end(); p++ )
	{
		if( ! lstrcmp( (*p)->GetName(), szName ) )
		{
			return *p;
		}
	}

	return NULL;
}




/* 
*���ܣ����һ��������
*ϸ�������� 
*������ pstMemberInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::AddApply( CFacMember::SMemberInfo* pstMemberInfo )
{
	if( pstMemberInfo )
	{
		// ����������Ƿ��Ѿ�����
		if( CFacMember* pFacMember = this->GetApply( pstMemberInfo->guidMember ) )
		{
			// ��������
			pFacMember->SetMemberInfo( pstMemberInfo );
		}
		else
		{
			// �����������
			m_listApply.push_back( new CFacMember( pstMemberInfo ) );
		}

		return true;
	}

	return false;
}



/* 
*���ܣ�ɾ��һ��������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::DelApply( CGUID guid )
{
	for( std::list<CFacMember*>::iterator p = m_listApply.begin(); p != m_listApply.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			SAFE_DELETE( *p );
			m_listApply.erase( p );

			return true;
		}
	}

	return false;
}



/* 
*���ܣ��Ƿ�ӵ��һ��������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFaction::HasApply( CGUID guid ) const
{
	return this->GetApply( guid )  !=  NULL;
}



/* 
*���ܣ���ȡһ��������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFaction::GetApply( CGUID guid ) const
{
	for( std::list<CFacMember*>::const_iterator p = m_listApply.begin(); p != m_listApply.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			return *p;
		}
	}

	return NULL;
}





/* 
*���ܣ���ȡһ��������
*ϸ�������� 
*������ szName
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFaction::GetApply( LPCTSTR szName ) const
{
	for( std::list<CFacMember*>::const_iterator p = m_listApply.begin(); p != m_listApply.end(); p++ )
	{
		if( ! lstrcmp( (*p)->GetName(), szName ) )
		{
			return *p;
		}
	}

	return NULL;
}



/*
*���ܣ� ����ָ���������� ��Ա
*ϸ�������� 
*������ eSort	�������ö��
*����ֵ�� 
*�޸�ʱ�䣺2008-4-16
*/
void	CFaction::SortMembers( CFacMember::ENUM_SORT eSort )
{
	switch( eSort )
	{
		case CFacMember::SORT_BY_NORMAL:		m_listMembers.sort( CFacMember::CompareNormal );			break;
		case CFacMember::SORT_BY_NAME:			m_listMembers.sort( CFacMember::CompareByName );			break;
		case CFacMember::SORT_BY_JOB:			m_listMembers.sort( CFacMember::CompareByJob );				break;
		case CFacMember::SORT_BY_TITLE:			m_listMembers.sort( CFacMember::CompareByTitle );			break;
		case CFacMember::SORT_BY_LEVEL:			m_listMembers.sort( CFacMember::CompareByLevel );			break;
		case CFacMember::SORT_BY_STATE:			m_listMembers.sort( CFacMember::CompareByState );			break;
		case CFacMember::SORT_BY_REGION:		m_listMembers.sort( CFacMember::CompareByRegion );			break;
		case CFacMember::SORT_BY_ONLINE:		m_listMembers.sort( CFacMember::CompareByOnline );			break;
		case CFacMember::SORT_BY_OCCUPATION:	m_listMembers.sort( CFacMember::CompareByOccupation );		break;
		case CFacMember::SORT_BY_CONTRIBUTE:	m_listMembers.sort( CFacMember::CompareByContribute );		break;
		case CFacMember::SORT_BY_SELECTION:		m_listMembers.sort( CFacMember::CompareBySelection );		break;
	}
}



/*
*���ܣ� ����ָ���������� ������
*ϸ�������� 
*������ eSort	�������ö��
*����ֵ�� 
*�޸�ʱ�䣺2008-4-16
*/
void	CFaction::SortApply( CFacMember::ENUM_SORT eSort )
{
	switch( eSort )
	{
		case CFacMember::SORT_BY_NORMAL:		m_listApply.sort( CFacMember::CompareNormal );				break;
		case CFacMember::SORT_BY_NAME:			m_listApply.sort( CFacMember::CompareByName );				break;
		case CFacMember::SORT_BY_JOB:			m_listApply.sort( CFacMember::CompareByJob );				break;
		case CFacMember::SORT_BY_TITLE:			m_listApply.sort( CFacMember::CompareByTitle );				break;
		case CFacMember::SORT_BY_LEVEL:			m_listApply.sort( CFacMember::CompareByLevel );				break;
		case CFacMember::SORT_BY_STATE:			m_listApply.sort( CFacMember::CompareByState );				break;
		case CFacMember::SORT_BY_REGION:		m_listApply.sort( CFacMember::CompareByRegion );			break;
		case CFacMember::SORT_BY_ONLINE:		m_listApply.sort( CFacMember::CompareByOnline );			break;
		case CFacMember::SORT_BY_OCCUPATION:	m_listApply.sort( CFacMember::CompareByOccupation );		break;
		case CFacMember::SORT_BY_CONTRIBUTE:	m_listApply.sort( CFacMember::CompareByContribute );		break;
		case CFacMember::SORT_BY_SELECTION:		m_listApply.sort( CFacMember::CompareBySelection );			break;
	}
}





/*
*���ܣ�ǿ���������
*ϸ�������� 
*������ CFaction		��Ա1
*		CFaction		��Ա2
*����ֵ�� -1 ����ǰ�棬1�ں��棬0��Ӱ��
*�޸�ʱ�䣺2008-4-16
*/
int CFaction::CompareForceRule( const CFaction* pFaction1, const CFaction* pFaction2 )
{
	// ������һ
	if( GetGame()->GetUnionManager()->IsPlayerChairman( pFaction1->GetUnionGUID(), pFaction1->GetMasterGUID() ) )
	{
		return 1;
	}
	else if( GetGame()->GetUnionManager()->IsPlayerChairman( pFaction2->GetUnionGUID(), pFaction2->GetMasterGUID() ) )
	{
		return -1;
	}
	// �Լ��ڶ�
	else if( GetGame()->GetMainPlayer()->GetFactionID() == pFaction1->GetGUID() )
	{
		return 1;
	}
	else if( GetGame()->GetMainPlayer()->GetFactionID() == pFaction2->GetGUID() )
	{
		return -1;
	}
	// �����Ƿ���������
	else if( pFaction1->GetOnline() > pFaction2->GetOnline() )
	{
		return 1;
	}
	else if( pFaction1->GetOnline() < pFaction2->GetOnline() )
	{
		return -1;
	}

	return 0;
}




/*
*���ܣ�������������Ƚ�
*ϸ�������� 
*������ pFaction1		��Ա1
*		pFaction2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFaction::CompareByFaction( const CFaction* pFaction1, const CFaction* pFaction2 )
{
	// ǿ�ƹ���
	if( int iForceResult = CFaction::CompareForceRule( pFaction1, pFaction2 ) )
	{
		return iForceResult > 0;
	}

	// �������ֱ����������
	return lstrcmp( pFaction1->GetName(), pFaction2->GetName() ) < 0;
}



/*
*���ܣ��峤��������Ƚ�
*ϸ�������� 
*������ pFaction1		��Ա1
*		pFaction2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFaction::CompareByChairman( const CFaction* pFaction1, const CFaction* pFaction2 )
{
	// ǿ�ƹ���
	if( int iForceResult = CFaction::CompareForceRule( pFaction1, pFaction2 ) )
	{
		return iForceResult > 0;
	}

	// �������ֱ����������
	return lstrcmp( pFaction1->GetMasterName(), pFaction2->GetMasterName() ) < 0;
}



/*
*���ܣ�����������������Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFaction::CompareByOnline( const CFaction* pFaction1, const CFaction* pFaction2 )
{
	// ����������������
	long numOnlineMembers1 = pFaction1->GetNumOfOnlineMembers();
	long numOnlineMembers2 = pFaction2->GetNumOfOnlineMembers();

	if(  numOnlineMembers1 !=  numOnlineMembers2 )
	{
		return numOnlineMembers1  >  numOnlineMembers2;
	}

	// �������ֱ����������
	return CFaction::CompareByFaction( pFaction1, pFaction2 );
}


/*
*���ܣ�����ȼ�����Ƚ�
*ϸ�������� 
*������ pFacMember1		��Ա1
*		pFacMember2		��Ա2
*����ֵ�� false ����ǰ��
*�޸�ʱ�䣺2008-4-16
*/
bool CFaction::CompareByLevel( const CFaction* pFaction1, const CFaction* pFaction2 )
{
	// ���յȼ�����
	if( pFaction1->GetLevel()  !=  pFaction2->GetLevel() )
	{
		return pFaction1->GetLevel()  >  pFaction2->GetLevel();
	}

	// �������ֱ����������
	return CFaction::CompareByFaction( pFaction1, pFaction2 );
}





//**************************************************************************************
// ���� �߼�������
//**************************************************************************************

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFactionManager::CFactionManager()
{
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFactionManager::~CFactionManager()
{
	this->ClearFactions();
}


/*
*���ܣ���ռ��庯��
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CFactionManager::ClearFactions()
{
	for( std::list<CFaction*>::iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		SAFE_DELETE( *p );
	}

	m_listFactions.clear();
}





/* 
*���ܣ����һ������
*ϸ�������� 
*������ pstFacInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::AddFaction( CFaction::SFacInfo* pstFacInfo )
{
	if( pstFacInfo )
	{
		// ���ü���ͼ��CRC
		pstFacInfo->dwIconCRC = GetGame()->GetFactionManager()->GetFacIconCRC( pstFacInfo->guidFac );

		// �������Ƿ��Ѿ�����
		if( CFaction* pFaction = this->GetFaction( pstFacInfo->guidFac ) )
		{
			// ��������
			pFaction->SetFacInfo( pstFacInfo );
		}
		else
		{
            CFaction * Faction = new CFaction( pstFacInfo );
   			// ����¼���
			m_listFactions.push_back(Faction);
		}

		return true;
	}

	return false;
}



/* 
*���ܣ�ɾ��һ������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::DelFaction( CGUID guid )
{
	// ���������������е����
	std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
	for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
	{
		if( (*p)->GetType() == TYPE_PLAYER )
		{
			if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
			{
				// �ҵ��ü����Player���д���
				if( pPlayer->GetFactionID() == guid )
				{
					pPlayer->SetFactionID( NULL_GUID );
					pPlayer->SetUnionID( NULL_GUID );
					pPlayer->SetFactionName( string("") );
					pPlayer->SetFactionTitle( string("") );
				}
			}
		}
	}

	// ������������ɾ��
	if( CUnion* pUnion = GetGame()->GetUnionManager()->GetFacUnion( guid ) )
	{
		pUnion->DelFaction( guid );
	}

	// ɾ������
	for( std::list<CFaction*>::iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			SAFE_DELETE( *p );
			m_listFactions.erase( p );

			return true;
		}
	}

	return false;
}



/* 
*���ܣ��Ƿ�ӵ��һ������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasFaction( CGUID guid )
{
	return this->GetFaction( guid )  !=  NULL;
}



/* 
*���ܣ���ȡһ������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFaction*	CFactionManager::GetFaction( CGUID guid )
{
	for( std::list<CFaction*>::iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			return *p;
		}
	}

	return NULL;
}


/* 
*���ܣ���ȡ����Ȩ�޵ĵȼ�����
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::GetFacLvlForPV( eFactionPurviewIndex ePVIndex )
{
	return COrganizingParam::getInstance().GetFacPurviewNeedLevel( ePVIndex );
}



/* 
*���ܣ���ȡ����ȼ��� �������
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::GetMaxNumByFacLv( long lFacLvl )
{
	if( const tagFacLevelSetup* pFacLevelSetup = COrganizingParam::getInstance().GetLevelSetup( lFacLvl ) )
	{
		return pFacLevelSetup->lMaxMemNum;
	}

	return -1;
}


/* 
*���ܣ���ȡ����ȼ��� ���Ҫ������
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2009-6-16
*/
long	CFactionManager::GetRequireNumByFacLv( long lFacLvl )
{
	if( const tagFacLevelSetup* pFacLevelSetup = COrganizingParam::getInstance().GetLevelSetup( lFacLvl ) )
	{
		return pFacLevelSetup->lDisbandMemNum;
	}

	return -1;
}


/*
*���ܣ���ȡ�Լ��ľݵ�����
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-26
*/
long	CFactionManager::GetNumOfMyBeachheads()
{
	long lNumOfMyBeachheads = 0;

	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		for( std::list<tagBaseInfo>::iterator p = m_listBeachheads.begin(); p != m_listBeachheads.end(); p++ )
		{
			if( pMyFaction->GetGUID() == (*p)._FacGuid )
				lNumOfMyBeachheads++;
		}
	}

	return lNumOfMyBeachheads;
}



/* 
*���ܣ���ȡ�ҵļ���
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFaction*	CFactionManager::GetMyFaction()
{
	return this->GetFaction( GetGame()->GetMainPlayer()->GetFactionID() );
}



/* 
*���ܣ���ȡ�ҵĻ���
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFactionManager::GetMyConfrere( CGUID guid )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		return pMyFaction->GetMember( guid );
	}

	return NULL;
}


/* 
*���ܣ���ȡ�ҵĻ���
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CFacMember*	CFactionManager::GetMyConfrere( LPCTSTR szName )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		return pMyFaction->GetMember( szName );
	}

	return NULL;
}


/* 
*���ܣ�����ҵĻ���
*ϸ�������� 
*������ pstMemberInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::AddMyConfrere( CFacMember::SMemberInfo* pstMemberInfo )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		return pMyFaction->AddMember( pstMemberInfo );
	}

	return false;
}



/* 
*���ܣ���ȡ�ҵĹٽ�
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::GetMyJob()
{
	return GetGame()->GetMainPlayer()->GetFactionJob();
}



/* 
*���ܣ������ҵĹٽ�
*ϸ�������� 
*������ lJob
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::SetMyJob( long lJob )
{
	if( lJob >= -1  &&  lJob <= MAXNUM_JobLvl )
		GetGame()->GetMainPlayer()->SetFactionJob( lJob );
	else
		GetGame()->GetMainPlayer()->SetFactionJob( 0 );

	return true;
}



/* 
*���ܣ���ȡ�ҵļ��幫��
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CFactionManager::GetMyPlacard()
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		return pMyFaction->GetPlacard();
	}

	return "";
}



/* 
*���ܣ������ҵļ��幫��
*ϸ�������� 
*������ szPlacard
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::SetMyPlacard( LPCTSTR szPlacard )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		pMyFaction->SetPlacard( szPlacard );
		return true;
	}

	return false;
}



/* 
*���ܣ���ȡ�ҵļ��� ����¼
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CFactionManager::GetMyMemo()
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		return pMyFaction->GetMemo();
	}

	return "";
}



/* 
*���ܣ������ҵļ��� ����¼
*ϸ�������� 
*������ szMemo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::SetMyMemo( LPCTSTR szMemo )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		pMyFaction->SetMemo( szMemo );
		return true;
	}

	return false;
}



/* 
*���ܣ���ȡ����ձ�·��
*ϸ�������� 
*������ guidFac
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
string	CFactionManager::GetFacIconPath( CGUID guidFac )
{
	// �ձ��ļ���
	TCHAR	szFacGUID[ 128 ];
	guidFac.tostring( szFacGUID );

	string strFacIconFile = "setup\\FactionIcon\\";
	strFacIconFile += szFacGUID;
	strFacIconFile += ".jpg";

	return strFacIconFile;
}



/* 
*���ܣ���ȡ����ձ�CRC
*ϸ�������� 
*������ guidFac
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
DWORD	CFactionManager::GetFacIconCRC( CGUID guidFac )
{
	//
	DWORD dwCRC = 0;

	// �ձ��ļ���
	string strFacIconFile = this->GetFacIconPath( guidFac );

	// ��ȡ�ļ�
	ifstream fFacIcon( strFacIconFile.c_str(), ios::in | ios::binary );
	if( fFacIcon.is_open() )
	{
		fFacIcon.seekg( 0, ios::end );
		DWORD dwIconSize = fFacIcon.tellg();

		if( LPBYTE pIconData = new BYTE[ dwIconSize ] )
		{
			fFacIcon.seekg( 0, ios::beg );
			fFacIcon.read( (LPSTR)pIconData, dwIconSize );

			// ����CRC
			CCrc32Static::DataCrc32( pIconData, dwIconSize, dwCRC );
			SAFE_DELETE_ARRAY( pIconData );
		}
		fFacIcon.close();
	}

	return dwCRC;
}


/* 
*���ܣ���ȡ����ձ�CRC
*ϸ�������� 
*������ guidFac
*		dwNewCRC
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::CheckFacIconCRC( CGUID guidFac, DWORD dwNewCRC )
{
	if( CFaction*	pFaction = this->GetFaction( guidFac ) )
	{
		return pFaction->GetIconCRC() != dwNewCRC;
	}

	return false;
}



/* 
*���ܣ���ȡ�ҵ�Ȩ��
*ϸ�������� 
*������ guid
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::GetMyRights( eFactionPurviewIndex ePV )
{
	CFaction*	pMyFaction = this->GetMyFaction();
	CPlayer*	pMainPlayer = GetGame()->GetMainPlayer();

	// �Ƿ��м���
	if( pMyFaction && pMainPlayer )
	{
		return pMyFaction->GetJobPurview( this->GetMyJob(), ePV );
	}

	return false;
}



/* 
*���ܣ����Ƿ��峤
*ϸ�������� 
*������ 
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::AmIChairman()
{
	return this->IsPlayerChairman( GetGame()->GetMainPlayer() );
}



/* 
*���ܣ�����Ƿ��峤
*ϸ�������� 
*������ pPlayer
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::IsPlayerChairman( CPlayer* pPlayer )
{
	if( pPlayer )
	{
		if( CFaction* pFaction = this->GetFaction( pPlayer->GetFactionID() ) )
		{
			return pFaction->GetMasterGUID() == pPlayer->GetExID();
		}
	}

	return false;
}



/* 
*���ܣ����Ƿ�����˼���
*ϸ�������� 
*������ 
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::AmIJoinedFaction()
{
	return this->IsPlayerJoinedFaction( GetGame()->GetMainPlayer() );
}



/* 
*���ܣ����Ƿ�������������
*ϸ�������� 
*������ guidFaction
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::AmIJoinedFaction( CGUID guidFaction )
{
	return this->IsPlayerJoinedFaction( GetGame()->GetMainPlayer(), guidFaction );
}



/* 
*���ܣ�����Ƿ�����˼���
*ϸ�������� 
*������ pPlayer
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::IsPlayerJoinedFaction( CPlayer* pPlayer )
{
	if( pPlayer )
		return pPlayer->GetFactionID() != NULL_GUID;

	return false;
}



/* 
*���ܣ�����Ƿ�������������
*ϸ�������� 
*������ pPlayer
*		guidFaction
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::IsPlayerJoinedFaction( CPlayer* pPlayer, CGUID guidFaction )
{
	if( pPlayer  &&  NULL_GUID != guidFaction )
		return pPlayer->GetFactionID() == guidFaction;

	return false;
}




/* 
*���ܣ�����Ƿ�ﵽ �������������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::CanUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �����������
	if( this->GetMyFaction()->GetLevel( eFun ) < this->GetFunMaxLevel( eFun )
		&& this->HasRes_ForUpgradeMyFun( eFun )
		&& this->HasExp_ForUpgradeMyFun( eFun )
		&& this->HasGold_ForUpgradeMyFun( eFun )
		&& this->HasProp_ForUpgradeMyFun( eFun )
		&& this->HasFacLvl_ForUpgradeMyFun( eFun )
		&& this->HasMasterLvl_ForUpgradeMyFun( eFun ) )
	{
		return true;
	}

	return false;
}


/* 
*���ܣ���ȡ���ܵ���ߵȼ�
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::GetFunMaxLevel( CFaction::ENUM_FUN eFun )
{
	return COrganizingParam::getInstance().GetFacFunMaxLevel( eFun );
}



/* 
*���ܣ���� �Ƿ����㹻�� ��Դ ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasRes_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long res = this->NeedRes_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != res )
		{
			// �Ƿ�ﵽҪ��
			if( pMyFaction->GetRes() >= res )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}



/* 
*���ܣ���� �Ƿ����㹻�� ���� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasExp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long exp = this->NeedExp_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != exp )
		{
			// �Ƿ�ﵽҪ��
			if( pMyFaction->GetExp() >= exp )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}



/* 
*���ܣ���� �Ƿ����㹻�� ��� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasGold_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long gold = this->NeedGold_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != gold )
		{
			// �Ƿ�ﵽҪ��
			if( (long)GetGame()->GetMainPlayer()->GetMoney() >= gold )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}



/* 
*���ܣ���� �Ƿ����㹻�� ����ȼ� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasFacLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long level = this->NeedFacLvl_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != level )
		{
			// �Ƿ�ﵽҪ��
			if( pMyFaction->GetLevel() >= level )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}


/* 
*���ܣ���� �Ƿ����㹻�� �峤�ȼ� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasMasterLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long level = this->NeedMasterLvl_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != level )
		{
			// �Ƿ�ﵽҪ��
			if( GetGame()->GetMainPlayer()->GetLevel() >= level )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}



/* 
*���ܣ���� �Ƿ����㹻�� ���� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CFactionManager::HasProp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long index = this->NeedProp_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != index )
		{
			// �Ƿ�ﵽҪ��
			if( GetGame()->GetMainPlayer()->GetGoodsAmount( index ) > 0 )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}


/* 
*���ܣ���� �Ƿ����㹻�� �ݵ� ���������˹���
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2009-10-26
*/
bool	CFactionManager::HasBeachhead_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long lBeachhead = this->NeedBeachhead_ForUpgradeMyFun( eFun );

		// �Ƿ���Ҫ
		if( -1 != lBeachhead )
		{
			// �Ƿ�ﵽҪ��
			if( this->GetNumOfMyBeachheads() >= lBeachhead )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}


/* 
*���ܣ���ȡ�����˹��ܵ� ��Դ ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedRes_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_FacRes == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}



/* 
*���ܣ���ȡ�����˹��ܵ� ���� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedExp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_FacExp == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}



/* 
*���ܣ���ȡ�����˹��ܵ� ��� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedGold_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_Money == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}


/* 
*���ܣ���ȡ�����˹��ܵ� ����ȼ� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedFacLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_FacLevel == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}



/* 
*���ܣ���ȡ�����˹��ܵ� �峤�ȼ� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedMasterLvl_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_LeaderLevel == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}


/* 
*���ܣ���ȡ�����˹��ܵ� ���� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
long	CFactionManager::NeedProp_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_Goods == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}



/* 
*���ܣ���ȡ�����˹��ܵ� �ݵ� ����
*ϸ�������� 
*������ eFun
*����ֵ�� bool
*�޸�ʱ�䣺2009-10-26
*/
long	CFactionManager::NeedBeachhead_ForUpgradeMyFun( CFaction::ENUM_FUN eFun )
{
	// �Ƿ��м���
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		VecCondition* pvec = (VecCondition*)COrganizingParam::getInstance().GetUpgradeVecCondition( eFun, pMyFaction->GetLevel( eFun ) +1 );

		if( pvec )
		{
			for( vector<tagUpgradeCondition>::iterator p = pvec->begin(); p != pvec->end(); p++ )
			{
				tagUpgradeCondition* pstUpgradeCondition = &(*p);

				if( eUCT_BaseNum == pstUpgradeCondition->lConditionType )
				{
					return pstUpgradeCondition->lValue;
				}
			}
		}
	}

	return -1;
}


/*
*���ܣ� ���� ��ʼ������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvInitMsg( CMessage* pMsg )
{
	// ������Ϣ ���ݿ��С
	long lDataSize = pMsg->GetLong();

	LPBYTE	pData = new BYTE[ lDataSize ];
	pMsg->GetEx( pData, lDataSize );

	//
	long lPos = 0;

	//! ����ȫ������
	COrganizingParam::getInstance().DecodeFromByteArray( pData, lPos );

	//// ��ȡICONУ����
	//long crcNew = _GetLongFromByteArray( pData, lPos );

	//// �Ƚ�У�����Ƿ���Ҫ����ICON
	//long crcOld = GetPrivateProfileInt( "DefaultGameConfig", "CRC", 0, "setup/GameConfig.ini" );
	//if( crcNew != crcOld )
	//{
	//	// ���������������ICON
	//	this->SendUpdateAllFacIconMsg();
	//}

	// �ͷ���Ϣ���ݿ�
	SAFE_DELETE_ARRAY( pData );
}

/*
*���ܣ� ���� һ������Ĺ�������֪ͨ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvFacPublicDataMsg( CMessage* pMsg )
{
	// ȡ�� һ�����幫������
	tagFacPublicData stFacPublicData;
	pMsg->GetEx( &stFacPublicData, sizeof(stFacPublicData ) );

	// �Ѿ��������������
	if( CFaction* pFaction = this->GetFaction( stFacPublicData.guid ) )
	{
		pFaction->SetName( stFacPublicData.szName );
		pFaction->SetLevel( stFacPublicData.lLevel );
	}
	// û�д����򴴽�
	else
	{
		CFaction::SFacInfo	stFacInfo;
		ZeroMemory( &stFacInfo, sizeof(stFacInfo) );
		stFacInfo.guidFac = stFacPublicData.guid;
		stFacInfo.strFacName = stFacPublicData.szName;
		stFacInfo.arrLevel[ CFaction::FUN_FACTION ] = stFacPublicData.lLevel;

		this->AddFaction( &stFacInfo );
	}
}




/*
*���ܣ� ���� ���������������Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendBaseInfoRequestMsg()
{
	CMessage msg( MSG_C2S_FacData_Request );

	msg.Send();
}



/*
*���ܣ� ���� ���������������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvBaseInfoMsg( CMessage* pMsg )
{
	// ������������Ϣ��Ŀǰֻ���Լ��ļ�����Ϣ
	tagFacBaseData	stFacBaseData;
	pMsg->GetEx( &stFacBaseData, sizeof(stFacBaseData) );

	//
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		pMyFaction->SetUnionGUID( stFacBaseData.SuperiorGuid );
		pMyFaction->SetMasterGUID( stFacBaseData.MasterGuid );
		pMyFaction->SetMasterName( stFacBaseData.szMasterName );
		pMyFaction->SetCountry( stFacBaseData.lCountryID );

		pMyFaction->SetRecruiting( stFacBaseData.bIsRecruiting != 0 );
		pMyFaction->SetTrade( stFacBaseData.bOpenResWar != 0 );

		pMyFaction->SetRes( stFacBaseData.lCurrRes );
		pMyFaction->SetExp( stFacBaseData.lCurrExp );
		pMyFaction->SetWarFund( stFacBaseData.lFacArmyInvest );
		pMyFaction->SetAchievement( stFacBaseData.lFacBattle );

		pMyFaction->SetLevel( CFaction::FUN_WELFARE, stFacBaseData.lSubExpLevel );
		pMyFaction->SetLevel( CFaction::FUN_STORAGE, stFacBaseData.lSpecialityLevel );
		pMyFaction->SetLevel( CFaction::FUN_SMELT, stFacBaseData.lBattleLevel );
		pMyFaction->SetLevel( CFaction::FUN_FORGING, stFacBaseData.lBussinessLevel );
		pMyFaction->SetLevel( CFaction::FUN_SEWING, stFacBaseData.lNoblelevel );

		// ���ùٽ����ֺ�Ȩ��
		for( long c = 1; c <= MAXNUM_JobLvl; c++ )
		{
			pMyFaction->SetJobName( c, stFacBaseData.arr_szJobName[ c-1 ] );
			pMyFaction->SetJobPurview( c, stFacBaseData.arrJobPurview[ c-1 ] );
		}
	}

	// ˢ�� ��������
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshAll();
	//}
}




/*
*���ܣ� ���� ���м���ͼ����������
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendUpdateAllFacIconMsg()
{
		CMessage msg( MSG_C2S_Faction_AllIconData_Request );
		msg.Send();
}


/*
*���ܣ� ���� ��������ͼ����������
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendUpdateFacIconMsg( CGUID guidFac )
{
	CMessage msg( MSG_C2S_Faction_IconData_Request );

		msg.Add( guidFac );

	msg.Send();
}


/*
*���ܣ� ���� ���м���ͼ�����ݸ���
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvUpdateAllFacIconMsg( CMessage* pMsg )
{
	// ����У���룬long
	long lCRC = pMsg->GetLong();

	TCHAR	szCRC[ 128 ];
	wsprintf( szCRC, "%d", lCRC );
	WritePrivateProfileString( "DefaultGameConfig", "CRC", szCRC, "setup/GameConfig.ini" );

	// ������Ϣ ���ݿ��С
	long lPos = 0;
	long lDataSize = pMsg->GetLong();

	LPBYTE	pData = new BYTE[ lDataSize ];
	pMsg->GetEx( pData, lDataSize );

	//
	long lIconNum = _GetLongFromByteArray( pData, lPos );

	for ( long i = 0; i < lIconNum; i++ )
	{
		CGUID IconGuid;
		_GetBufferFromByteArray( pData, lPos, IconGuid );

		if( long lIconSize = _GetLongFromByteArray( pData, lPos ) )
		{
			LPBYTE	pIconData = new BYTE[ lIconSize ];
			_GetBufferFromByteArray( pData, lPos, pIconData, lIconSize );

			// �洢��Ӳ�̣�ת����ͼƬ���Լ���GUIDΪ�ļ���
			TCHAR	szGUID[ 128 ];
			IconGuid.tostring( szGUID );
			TCHAR	szIconFileName[ MAX_PATH ];
			wsprintf( szIconFileName, "setup/FactionIcon/%s.jpg", szGUID );

			std::ofstream	fFactionIcon( szIconFileName, std::ios::out | std::ios::trunc | ios::binary );
			if( fFactionIcon.is_open() )
			{
				fFactionIcon.write( (char*)pIconData, lIconSize );
				fFactionIcon.close();
			}

			// �����ڴ���Դ
			CBitmapX::GetBitmapX( szIconFileName, true );

			// �ͷ���Դ
			SAFE_DELETE_ARRAY( pIconData );
		}
	}

	// �ͷ���Դ
	SAFE_DELETE_ARRAY( pData );
}


/*
*���ܣ� ���� һ�������ͼ�����ݸ���֪ͨ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvUpdateFacIconNotifyMsg( CMessage* pMsg )
{
	//// ����У���룬long
	//long lCRC = pMsg->GetLong();

	//TCHAR	szCRC[ 128 ];
	//wsprintf( szCRC, "%d", lCRC );
	//WritePrivateProfileString( "DefaultGameConfig", "CRC", szCRC, "setup/GameConfig.ini" );

	// ����GUID
	CGUID	guidFac;
	pMsg->GetGUID( guidFac );

	// ͼ���С
	long lSize = pMsg->GetLong();

	// ͼ������
	LPBYTE	pData = new BYTE[ lSize ];
	pMsg->GetEx( pData, lSize );

	// �洢��Ӳ�̣�ת����ͼƬ���Լ���GUIDΪ�ļ���
	string strFacIconFile = this->GetFacIconPath( guidFac );

	std::ofstream	fFactionIcon( strFacIconFile.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
	if( fFactionIcon.is_open() )
	{
		fFactionIcon.write( (char*)pData, lSize );
		fFactionIcon.close();
	}

	// �����ڴ���Դ
	CBitmapX::GetBitmapX( strFacIconFile, true );

	// ����CRC
	if( CFaction*	pFaction = this->GetFaction( guidFac ) )
	{
		pFaction->SetIconCRC( this->GetFacIconCRC( guidFac ) );
	}

	// �ͷ���Դ
	SAFE_DELETE_ARRAY( pData );

	// ˢ�� ͼ������
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshLogo();
	//}
}







/*
*���ܣ� ���� ��Ա��������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvMemberInfoMsg( CMessage* pMsg )
{
	// ��Ա����
	long num = pMsg->GetLong();

	// ��ȡ������Ա��Ϣ
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		// �����֮ǰ������
		pMyFaction->ClearMembers();

		// ѭ����ȡ������Ա
		for( long c = 0; c < num; c++ )
		{
			tagFacMemInfo	stFacMemInfo;
			pMsg->GetEx( &stFacMemInfo, sizeof(tagFacMemInfo) );

			// �������
			CFacMember::SMemberInfo	stMemberInfo;
			stMemberInfo.guidFac = GetGame()->GetMainPlayer()->GetFactionID();
			stMemberInfo.guidMember = stFacMemInfo.MemberGuid;
			stMemberInfo.strName = stFacMemInfo.szName;
			stMemberInfo.strTitle = stFacMemInfo.szTitle;
			stMemberInfo.lLevel = stFacMemInfo.lLvl;
			stMemberInfo.lJob = stFacMemInfo.lJobLvl;
			stMemberInfo.lOccupation = stFacMemInfo.lOccu;
			stMemberInfo.lContribute = stFacMemInfo.lContribute;
			stMemberInfo.bOnline = stFacMemInfo.bIsOnline != 0;
			stMemberInfo.strRegion = stFacMemInfo.szRegion;
			stMemberInfo.lAchievement = stFacMemInfo.lBattle;
			stMemberInfo.lWarFund = stFacMemInfo.lArmyInvest;
			stMemberInfo.bWar = stFacMemInfo.bJoinBattle ? true : false;
            stMemberInfo.LastOnlineTime = stFacMemInfo.LastOnlineTime;

			pMyFaction->AddMember( &stMemberInfo );
		}
// ���壺������ԣ��� Debug ״̬������ģ����Ա
//#ifdef _DEBUG
//		for( long c = 0; c < 50; c++ )
//		{
//			CFacMember::SMemberInfo	stMemberInfo;
//			stMemberInfo.guidFac = GetGame()->GetMainPlayer()->GetFactionID();
//			CGUID::CreateGUID( stMemberInfo.guidMember );
//			stMemberInfo.strName = CStringReading::LoadText( 208, 1+rand()%220 );
//			stMemberInfo.strTitle = CStringReading::LoadText( 208, 1+rand()%220 );
//			stMemberInfo.lLevel = 1+ rand()%200;
//			stMemberInfo.lJob = rand()%MAXNUM_JobLvl;
//			stMemberInfo.lOccupation = rand()%OCC_Max;
//			stMemberInfo.lContribute = rand()%100000;
//			stMemberInfo.bOnline = rand()%2 != 0;
//			stMemberInfo.strRegion = CCountryList::GetCountry( 1+rand()%6 )->strName.c_str();
//
//			stMemberInfo.lAchievement = rand()%10000;
//			stMemberInfo.lWarFund = rand()%10000;
//			stMemberInfo.bWar = rand()%2 ? true : false;
//
//			pMyFaction->AddMember( &stMemberInfo );
//		}
//#endif

		// �� ��Ա���
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->RefreshMemberDlg();
		//}
        FireUIEvent("FactionPage","UpdateMem");
	}
}




/*
*���ܣ� ���� ��������������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvRequisitionInfoMsg( CMessage* pMsg )
{
	// ����������
	long num = pMsg->GetLong();

	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		// �����֮ǰ������
		pMyFaction->ClearApply();

		// ��ȡ����������
		for( long c = 0; c < num; c++ )
		{
			tagFacApplyPerson	stFacApplyPerson;
			pMsg->GetEx( &stFacApplyPerson, sizeof(tagFacApplyPerson) );

			// �������
			CFacMember::SMemberInfo	stMemberInfo;
			stMemberInfo.guidFac = GetGame()->GetMainPlayer()->GetFactionID();
			stMemberInfo.guidMember = stFacApplyPerson.PlayerGuid;
			stMemberInfo.strName = stFacApplyPerson.szName;
			stMemberInfo.lLevel = stFacApplyPerson.lLvl;
			stMemberInfo.lOccupation = stFacApplyPerson.lOccu;

			pMyFaction->AddApply( &stMemberInfo );
		}

		// ˢ�� �������
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->RefreshRequisitionDlg();
		//}
	}
}




/*
*���ܣ� ���� �ݵ���Ϣ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-26
*/
void	CFactionManager::RecvBeachheadInfoMsg( CMessage* pMsg )
{
	m_listBeachheads.clear();

	long	lNumOfBeachheads = pMsg->GetLong();

	for( long c = 0; c < lNumOfBeachheads; c++ )
	{
		tagBaseInfo	stBeachheadInfo;
		pMsg->GetEx( &stBeachheadInfo, sizeof(stBeachheadInfo) );

		m_listBeachheads.push_back( stBeachheadInfo );
	}

	// ˢ�� ������ս��
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshBeachheadDlg();
	//}
}



/*
*���ܣ� ���� ��������Ľű��¼�Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvCreateNotifyMsg( CMessage* pMsg )
{
	// ��� �ɹ�ʧ��
	if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
	{
		// �򿪼��崴������
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->OpenCreationUI();
		//}
	}
	else
	{
		// �������
		this->HandleErrorMsg( pMsg );
	}
}



/*
*���ܣ� ���� ��������Э��
*ϸ�������� 
*������ strFactionName	��������
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendCreateMsg( LPCTSTR strFactionName ,LPCTSTR Placard)
{
	// ��������Э��
	CMessage msg( MSG_C2S_ORGSYS_CreateFaction_Reques );

		// ��������
		msg.Add( strFactionName );
        msg.Add( Placard );

	msg.Send();
}




/*
*���ܣ� ���� ��ɢ����Э��
*ϸ�������� 
*������ strFactionName	��������
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendDisbandMsg()
{
	// ��ɢ����Э��
	CMessage msg( MSG_C2S_ORGSYS_DisbandFaction_Request );

	msg.Send();
}



/*
*���ܣ� ���� �������幦��Э��
*ϸ�������� 
*������ eFun
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendUpgradeMsg( CFaction::ENUM_FUN eFun )
{
	// �������幦��Э��
	CMessage msg( MSG_C2S_FACTION_Upgrade_Request );

		msg.Add( (DWORD)eFun );

	msg.Send();
}



/*
*���ܣ� ���� �˳�����Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendQuitMsg()
{
	// �˳�����Э��
	CMessage msg( MSG_C2S_ORGSYS_Quit_Request );

	msg.Send();
}




/*
*���ܣ� ���� �߳�����Э��
*ϸ�������� 
*������ guidMember	���߳�����ԱGUID
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendKickMsg( CGUID guidMember )
{
	// �߳�����Э��
	CMessage msg( MSG_C2S_ORGSYS_Kick_Request );

		msg.Add( guidMember );

	msg.Send();
}




/*
*���ܣ� ���� �޸Ĺ������� Э��
*ϸ�������� 
*������ szPlacard	���µ�����
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetPlacardMsg( LPCTSTR szPlacard )
{
	CMessage msg( MSG_C2S_ORGSYS_SetPlacard_Request );

		msg.Add( szPlacard );

	msg.Send();
}



/*
*���ܣ� ���� �޸ı���¼���� Э��
*ϸ�������� 
*������ szPlacard	���µ�����
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetMemoMsg( LPCTSTR szMemo )
{
	CMessage msg( MSG_C2S_ORGSYS_SetLeaveword_Request );

		msg.Add( szMemo );

	msg.Send();
}




/*
*���ܣ� ���� ������������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvPlacardInfoMsg( CMessage* pMsg )
{
	tagOrgPronounceWord	stOrgPronounceWord;
	pMsg->GetEx( &stOrgPronounceWord, sizeof(stOrgPronounceWord) );

	// �໰����
	string	strPlacard = stOrgPronounceWord.szBody;
	CClientWordsFilter::GetInstance()->Check( strPlacard, true, true );

	// ���湫��
	this->SetMyPlacard( strPlacard.c_str() );

	// ˢ�� ����
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshPlacard();
	//}
}



/*
*���ܣ� ���� ����¼��������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::RecvMemoInfoMsg( CMessage* pMsg )
{
	tagOrgPronounceWord	stOrgPronounceWord;
	pMsg->GetEx( &stOrgPronounceWord, sizeof(stOrgPronounceWord) );

	// �໰����
	string	strMemo = stOrgPronounceWord.szBody;
	CClientWordsFilter::GetInstance()->Check( strMemo, true, true );

	// ���汸��¼
	this->SetMyMemo( strMemo.c_str() );

	// ˢ�� ����¼
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshMemo();
	//}
}


/*
*���ܣ� ���� ս����ϢЭ��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-21
*/
void	CFactionManager::RecvWarInfoMsg( CMessage* pMsg )
{
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		long lNumOfWarType = pMsg->GetLong();

		for( long c = 0; c < lNumOfWarType; c++ )
		{
			BYTE byBT = pMsg->GetByte();
			long lNumOfEnemys = pMsg->GetLong();

			for( long n = 0; n < lNumOfEnemys; n++ )
			{
				tagEnemy stEnemy;
				pMsg->GetEx( &stEnemy, sizeof(stEnemy) );

				pMyFaction->SetEnemy( ( eBattleType )byBT, &stEnemy );
			}
		}

		// ˢ�� ս����Ϣ
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->RefreshWarInfoDlg();
		//}
	}
}


/*
*���ܣ� ���� ��Ա��ѫ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-23
*/
void	CFactionManager::RecvMemberAchievementMsg( CMessage* pMsg )
{
	CGUID guid;
	pMsg->GetGUID( guid );

	long lAchievement = pMsg->GetLong();

	if( CFacMember* pMember =  this->GetMyConfrere( guid ) )
	{
		pMember->SetAchievement( lAchievement );

		// ˢ�� ��ս�б�
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->RefreshWarListDlg();
		//}
	}
}


/*
*���ܣ� ���� ��Ա΢��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-23
*/
void	CFactionManager::RecvMemberWarFundMsg( CMessage* pMsg )
{
	CGUID guid;
	pMsg->GetGUID( guid );

	long lWarFund = pMsg->GetLong();

	if( CFacMember* pMember =  this->GetMyConfrere( guid ) )
	{
		pMember->SetWarFund( lWarFund );

		// ˢ�� ��ս�б�
		//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
		//{
		//	pFactionPageEx->RefreshWarListDlg();
		//}
	}
}




/*
*���ܣ� ���� �޸Ĺٽ��������� Э��
*ϸ�������� 
*������ idJob		ְҵID
*		szJobName	ְҵ����
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetJobNameMsg( long idJob, LPCTSTR szJobName )
{
	CMessage msg( MSG_C2S_ORGSYS_SetJobName_Request );

		msg.Add( idJob );
		msg.Add( szJobName );

	msg.Send();
}




/*
*���ܣ� ���� �޸Ĺٽ�Ȩ������ Э��
*ϸ�������� 
*������ idJob
*		lPurview
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetJobPurviewMsg( long idJob, long lPurview )
{
	CMessage msg( MSG_C2S_ORGSYS_SetJobPurview_Request );

		msg.Add( idJob );
		msg.Add( lPurview );

	msg.Send();
}




/*
*���ܣ� ���� ���ó�Ա�ٽ����� Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetMemberJobMsg( CGUID guidMember, long idJob )
{
	CMessage msg( MSG_C2S_ORGSYS_SetMemberJob_Request );

		msg.Add( guidMember );
		msg.Add( idJob );

	msg.Send();
}




/*
*���ܣ� ���� ����������� Э��
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendCheckRequisitionMsg( long bAgree, CGUID guid )
{
	CMessage msg( MSG_C2S_ORGSYS_CheckRequisition_Request );

		msg.Add( bAgree );
		msg.Add( guid );

	msg.Send();
}




/*
*���ܣ� Ⱥ�� ����������� Э��
*ϸ�������� 
*������ bAgree
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendCheckRequisitionMsg( long bAgree )
{
	// 
	if( CFaction* pMyFaction = this->GetMyFaction() )
	{
		// ��������������
		std::list<CFacMember*>*	plistApply = pMyFaction->GetListOfApply();

		for( std::list<CFacMember*>::iterator p = plistApply->begin(); p != plistApply->end(); p++ )
		{
			// ����ѡ�е�������
			if( CFacMember* pApply = *p )
			{
				if( pApply->GetSelected() )
				{
					this->SendCheckRequisitionMsg( bAgree, pApply->GetGUID() );
				}
			}
		}
	}
}



/*
*���ܣ� ���� ����������� Э��
*ϸ�������� 
*������ szInvitePlayerName
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendInviteRequestMsg( LPCTSTR szInvitePlayerName )
{
	CMessage msg( MSG_C2S_Faction_InvitePlayer_Request );

		msg.Add( szInvitePlayerName );

	msg.Send();
}



/*
*���ܣ� ���� �ظ�������� Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendInviteAnswerMsg( CGUID guidInviter, long bAgree )
{
	CMessage msg( MSG_C2S_Faction_InvitePlayer_Answer );

		// long ����
		msg.Add( bAgree );
		// ������GUID
		msg.Add( guidInviter );

	msg.Send();
}



/*
*���ܣ� ���� ��ս��Ա Э��
*ϸ�������� 
*������ guidMember
*		byWar
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-22
*/
void	CFactionManager::SendMemberWarMsg( CGUID guidMember, BYTE byWar )
{
	CMessage msg( MSG_C2S_Faction_ChangeJoinBattle );

		msg.Add( guidMember );
		msg.Add( byWar );

	msg.Send();
}




/*
*���ܣ� ���� ��ս��Ա Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2009-10-22
*/
void	CFactionManager::RecvMemberWarMsg( CMessage* pMsg )
{
	// ��ԱGUID
	CGUID	guidMember;
	pMsg->GetGUID( guidMember );

	// �Ƿ��ս
	bool bWar = pMsg->GetByte() != 0;

	// ��������
	if( CFacMember*	pMember = this->GetMyConfrere( guidMember ) )
	{
		pMember->SetWar( bWar );
	}

	// ˢ�²�ս����
	//if( CFactionPageEx*	pFactionPageEx = GetGame()->GetCGuiEx()->GetFactionPageEx() )
	//{
	//	pFactionPageEx->RefreshWarDlg();
	//}
}


/*
*���ܣ� ���� ���óƺ� Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetTitleMsg( CGUID guidMember, LPCTSTR szTitle )
{
	CMessage msg( MSG_C2S_Faction_SetMemberTitle_Request );

		msg.Add( guidMember );
		msg.Add( szTitle );

	msg.Send();
}



/*
*���ܣ� ���� �����峤 Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendSetChairmanMsg( CGUID guidMember )
{
	CMessage msg( MSG_C2S_Faction_SetChairman_Request );

		msg.Add( guidMember );

	msg.Send();
}


/*
*���ܣ� ���� �ձ�ͼƬ
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::SendUploadFacIconMsg( LPCTSTR strFacIconFileName )
{
	DWORD dwIconSize = 0;
	LPBYTE pIconData = NULL;

	// 
	LPDIRECT3DDEVICE9 pd3dDevice = render::Interface::GetInstance()->GetDevice();

	TCHAR	szFullPath[ MAX_PATH + MAX_PATH ];
	wsprintf( szFullPath, "setup/FactionIcon/%s", strFacIconFileName );

	LPDIRECT3DSURFACE9	pSurf;
	if( SUCCEEDED( pd3dDevice->CreateOffscreenPlainSurface( 24, 16, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &pSurf, NULL ) ) )
	{
		if( SUCCEEDED( D3DXLoadSurfaceFromFile( pSurf, NULL, NULL, szFullPath, NULL, D3DX_FILTER_TRIANGLE, 0, NULL ) ) )
		{
			wsprintf( szFullPath, "setup/FactionIcon/Upload.tmp" );
			if( SUCCEEDED( D3DXSaveSurfaceToFile( szFullPath, D3DXIFF_JPG, pSurf, NULL, NULL ) ) )
			{
				// 
				ifstream fFacIcon( szFullPath, ios::in | ios::binary );
				if( fFacIcon.is_open() )
				{
					fFacIcon.seekg( 0, ios::end );
					dwIconSize = fFacIcon.tellg();
					if( pIconData = new BYTE[ dwIconSize ] )
					{
						fFacIcon.seekg( 0, ios::beg );
						fFacIcon.read( (LPSTR)pIconData, dwIconSize );
					}

					fFacIcon.close();
				}
			}
		}

		SAFE_RELEASE( pSurf );
	}

	// 
	if( pIconData )
	{
		CMessage msg( MSG_C2S_Faction_UpLoadIcon_Request );

			msg.Add( dwIconSize );
			msg.AddEx( pIconData, dwIconSize );

		msg.Send();

		SAFE_DELETE_ARRAY( pIconData );
	}
}




/*
*���ܣ� ���������Ϣ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CFactionManager::HandleErrorMsg( CMessage* pMsg )
{
	// ������
	long err = pMsg->GetLong();

	DWORD idErrStr = 0xffffffff;

	if( err > eFOEI_NotErr  &&  err < eFOEI_Err_Unknown )
	{
		idErrStr = 10000 + err;
	}
	else
	{
		idErrStr = 10000;
	}
    GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::LoadAppTextRes("FA_", idErrStr) );
}

void   CFactionManager::SendFactionList()
{
     CMessage msg(MSG_C2S_Faction_FindFactionList);
     msg.Send();
}

void   CFactionManager::RecvFactionList(CMessage* pMsg)
{
     m_listApplyInfo.clear();
     if ( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
     {
         CGUID				m_guidFacApply;							// ��ǰ����ļ���GUID
         pMsg->GetGUID( m_guidFacApply );

         long  numFactions = pMsg->GetLong();

         for ( long num = 0 ; num < numFactions ; num ++ )
         {
             tagApplyInfo       stFacInfo;
             char           szPronounce[MAX_PronounceCharNum];
             pMsg->GetEx( &stFacInfo , sizeof(tagApplyInfo) );
             pMsg->GetStr(szPronounce,MAX_PronounceCharNum);

             tagApplyInfoList   tagApplyList(stFacInfo,szPronounce);

             m_listApplyInfo.push_back( tagApplyList );
         }
         FireUIEvent("FactionList","Switch");
     }
     else
     {
         // �������
         HandleErrorMsg( pMsg );
     }
}