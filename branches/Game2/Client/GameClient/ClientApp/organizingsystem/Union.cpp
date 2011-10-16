/*Union.cpp
*�޸���: ����
*�޸�����:2008-11-5
*/

#include "StdAfx.h"
#include "Union.h"
#include "Faction.h"

#include "Player.h"
#include "Message.h"
#include "ClientRegion.h"
#include "Other/AudioList.h"
#include "../GameClient/Game.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//**************************************************************************************
// �����߼���
//**************************************************************************************

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion::CUnion()
{
	ZeroMemory( &m_stUnionInfo, sizeof(m_stUnionInfo) );
}

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion::CUnion( SUnionInfo* pstUnionInfo )
{
	this->SetUnionInfo( pstUnionInfo );
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion::~CUnion()
{
	this->Release();
}



/* 
*���ܣ���ʼ������
*ϸ�������� 
*������ ��
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::SetUnionInfo( SUnionInfo* pstUnionInfo )
{
	if( pstUnionInfo )
	{
		m_stUnionInfo = *pstUnionInfo;
		return true;
	}

	return false;
}


/* 
*���ܣ��ͷź���
*ϸ�������� 
*������ bool
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::Release()
{
	ZeroMemory( &m_stUnionInfo, sizeof(m_stUnionInfo) );

	m_listFactions.clear();

	return true;
}




/* 
*���ܣ����һ������
*ϸ�������� 
*������ pFaction
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::AddFaction( CFaction* pFaction )
{
	if( pFaction )
	{
		// �������³��������е����
		std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
		for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
		{
			if( (*p)->GetType() == TYPE_PLAYER )
			{
				if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
				{
					// �ҵ����л�Ļ�Ա���д���
					if( GetGame()->GetFactionManager()->IsPlayerJoinedFaction( pPlayer, pFaction->GetGUID() ) )
					{
						pPlayer->SetUnionID( this->GetID() );
					}
				}
			}
		}

		// ���ü����Ƿ��Ѿ���������
		if( ! this->HasFaction( pFaction->GetGUID() ) )
		{
			// ����������¼���
			m_listFactions.push_back( pFaction );
		}

		pFaction->SetUnionGUID( this->GetID() );

		return true;
	}

	return false;
}


/* 
*���ܣ�ɾ��һ�����˼���
*ϸ�������� 
*������ guid �л�GUID
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::DelFaction( CGUID guid )
{
	// ���������������е����
	std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
	for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
	{
		if( (*p)->GetType() == TYPE_PLAYER )
		{
			if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
			{
				// �ҵ��ü���Ļ�Ա���д���
				if( GetGame()->GetFactionManager()->IsPlayerJoinedFaction( pPlayer, guid ) )
				{
					pPlayer->SetUnionID( NULL_GUID );
				}
			}
		}
	}

	// ������ɾ���ü���
	for( std::list<CFaction*>::iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		if( (*p)->GetGUID()  ==  guid )
		{
			(*p)->SetUnionGUID( NULL_GUID );
			m_listFactions.erase( p );

			return true;
		}
	}

	return false;
}



/* 
*���ܣ����һ���л��Ա�Ƿ����
*ϸ�������� 
*������ guid �л�GUID
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::HasFaction( CGUID guid )
{
	return this->GetFaction( guid )  !=  NULL;
}




/* 
*���ܣ���ȡһ���л�
*ϸ�������� 
*������ guid �л�GUID
*����ֵ�� �л��Ա
*�޸�ʱ�䣺2008-11-5
*/
CFaction*	CUnion::GetFaction( CGUID guid )
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
*���ܣ���ȡһ���л�
*ϸ�������� 
*������ szName �л�����
*����ֵ�� �л��Ա
*�޸�ʱ�䣺2008-11-5
*/
CFaction*	CUnion::GetFaction( LPCTSTR szName )
{
	for( std::list<CFaction*>::iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		if( ! lstrcmp( (*p)->GetName(), szName ) )
		{
			return *p;
		}
	}

	return NULL;
}



/* 
*���ܣ�����һ���л������Ȩ��
*ϸ�������� 
*������ guid �л�GUID
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnion::SetFacPurview( CGUID guid, long lPurview )
{
	return false;
}



/* 
*���ܣ���ȡһ���л������Ȩ��
*ϸ�������� 
*������ guid �л�GUID
*����ֵ�� long
*�޸�ʱ�䣺2008-11-5
*/
long	CUnion::GetFacPurview( CGUID guid )
{
	return NULL;
}



/*
*���ܣ���ȡ��������
*ϸ�������� 
*������ 
*����ֵ�� 
*�޸�ʱ�䣺2008-11-5
*/
long	CUnion::GetNumOfMembers() const
{
	long num = 0;

	for( std::list<CFaction*>::const_iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		num += (*p)->GetNumOfMembers();
	}

	return num;
}



/*
*���ܣ���ȡ������������
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CUnion::GetNumOfOnlineMembers() const
{
	long num = 0;

	for( std::list<CFaction*>::const_iterator p = m_listFactions.begin(); p != m_listFactions.end(); p++ )
	{
		num += (*p)->GetNumOfOnlineMembers();
	}

	return num;
}



/*
*���ܣ� ����ָ���������� ����
*ϸ�������� 
*������ eSort	�������ö��
*����ֵ�� 
*�޸�ʱ�䣺2008-4-16
*/
void	CUnion::SortFactions( CFaction::ENUM_SORT eSort )
{
	switch( eSort )
	{
		case CFaction::SORT_BY_FACIOTN:			m_listFactions.sort( CFaction::CompareByFaction );			break;
		case CFaction::SORT_BY_CHAIRMAN:		m_listFactions.sort( CFaction::CompareByChairman );			break;
		case CFaction::SORT_BY_ONLINE:			m_listFactions.sort( CFaction::CompareByOnline );			break;
		case CFaction::SORT_BY_LEVEL:			m_listFactions.sort( CFaction::CompareByLevel );			break;
	}
}








//**************************************************************************************
// �����߼�������
//**************************************************************************************

/* 
*���ܣ����캯��
*ϸ�������� ����Աָ����ΪNULL��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnionManager::CUnionManager()
{
}


/*
*���ܣ���������
*ϸ�������� �ͷ���Դ��
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnionManager::~CUnionManager()
{
	this->Release();
}



/* 
*���ܣ��ͷź���
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CUnionManager::Release()
{
	for( std::map<CGUID,CUnion*>::iterator p = m_mapUnions.begin(); p != m_mapUnions.end(); p++ )
	{
		SAFE_DELETE( p->second );
	}

	m_mapUnions.clear();
}



/*
*���ܣ��ж��Ƿ�ӵ������
*ϸ�������� 
*������ guid	����GUID
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::HasUnion( CGUID guid )
{
	return m_mapUnions.find( guid )  !=  m_mapUnions.end();
}



/*
*���ܣ��ж��Լ��Ƿ��ڴ�����
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::AmIJoinedUnion()
{
	return this->IsPlayerJoinedUnion( GetGame()->GetMainPlayer() );
}


/*
*���ܣ��ж��Լ��Ƿ��ڴ�����
*ϸ�������� 
*������ guidUnion	����GUID
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::AmIJoinedUnion( CGUID guidUnion )
{
	return this->IsPlayerJoinedUnion( GetGame()->GetMainPlayer(), guidUnion );
}



/*
*���ܣ��ж�Player�Ƿ�������
*ϸ�������� 
*������ guidUnion	����GUID
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::IsPlayerJoinedUnion( CPlayer* pPlayer )
{
	if( pPlayer )
		return pPlayer->GetUnionID() != NULL_GUID;

	return false;
}



/*
*���ܣ��ж�Player�Ƿ��ڴ�����
*ϸ�������� 
*������ guidUnion	����GUID
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::IsPlayerJoinedUnion( CPlayer* pPlayer, CGUID guidUnion )
{
	if( pPlayer )
		return pPlayer->GetUnionID() == guidUnion;

	return false;
}




/* 
*���ܣ����Ƿ�����
*ϸ�������� 
*������ 
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::AmIChairman()
{
	return this->IsPlayerChairman( GetGame()->GetMainPlayer() );
}



/* 
*���ܣ�����Ƿ�����
*ϸ�������� 
*������ guidPlayer
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::IsPlayerChairman( CGUID guidUnion, CGUID guidPlayer )
{
	if( CUnion* pUnion = this->GetUnion( guidUnion ) )
	{
		return pUnion->GetMasterID() == guidPlayer;
	}

	return false;
}



/* 
*���ܣ�����Ƿ�����
*ϸ�������� 
*������ pPlayer
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::IsPlayerChairman( CPlayer* pPlayer )
{
	if( pPlayer )
	{
		if( CUnion* pUnion = this->GetUnion( pPlayer->GetUnionID() ) )
		{
			return pUnion->GetMasterID() == pPlayer->GetExID();
		}
	}

	return false;
}



/* 
*���ܣ����һ������
*ϸ�������� 
*������ pstUnionInfo
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::AddUnion( CUnion::SUnionInfo* pstUnionInfo )
{
	if( pstUnionInfo )
	{
		// ��������Ƿ��Ѿ�����
		if( this->HasUnion( pstUnionInfo->guidUnion ) )
		{
			// ��������
			m_mapUnions[ pstUnionInfo->guidUnion ]->SetUnionInfo( pstUnionInfo );
		}
		else
		{
			// ���������
			m_mapUnions[ pstUnionInfo->guidUnion ] = new CUnion( pstUnionInfo );
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
bool	CUnionManager::DelUnion( CGUID guid )
{
	// ���������������е����
	std::list<CBaseObject*>*	pListOfObjects = GetGame()->GetRegion()->GetObjectList();
	for( std::list<CBaseObject*>::iterator p = pListOfObjects->begin(); p != pListOfObjects->end(); p++ )
	{
		if( (*p)->GetType() == TYPE_PLAYER )
		{
			if( CPlayer* pPlayer = dynamic_cast<CPlayer*>(*p) )
			{
				// �ҵ������˵�Player���д���
				if( this->IsPlayerJoinedUnion( pPlayer, guid ) )
				{
					pPlayer->SetUnionID( NULL_GUID );
				}
			}
		}
	}

	// ��������ڵļ����������Ϣ
	list<CFaction*>*	plistFactions = GetGame()->GetFactionManager()->GetListOfFactions();
	for( list<CFaction*>::iterator p = plistFactions->begin(); p != plistFactions->end(); p++ )
	{
		CFaction* pFaction = *p;

		if( pFaction->GetUnionGUID() == guid )
			pFaction->SetUnionGUID( NULL_GUID );
	}

	// ����Ҫɾ��������
	std::map<CGUID,CUnion*>::iterator	p = m_mapUnions.find( guid );

	if( p  !=  m_mapUnions.end() )
	{
		// ɾ��һ������
		SAFE_DELETE( (*p).second );
		m_mapUnions.erase( p );

		return true;
	}

	return false;
}



/* 
*���ܣ���ȡһ������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion*	CUnionManager::GetUnion( CGUID guid )
{
	std::map<CGUID,CUnion*>::iterator	p = m_mapUnions.find( guid );

	if( p  !=  m_mapUnions.end() )
	{
		return (*p).second;
	}

	return NULL;
}



/* 
*���ܣ���ȡ�ҵĹٽ�
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
long	CUnionManager::GetMyJob()
{
	//return GetGame()->GetMainPlayer()->GetFactionJob();
	return 0;
}



/* 
*���ܣ������ҵĹٽ�
*ϸ�������� 
*������ lJob
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::SetMyJob( long lJob )
{
	//if( lJob >= -1  &&  lJob <= MAXNUM_JobLvl )
	//	GetGame()->GetMainPlayer()->SetFactionJob( lJob );
	//else
	//	GetGame()->GetMainPlayer()->SetFactionJob( 0 );

	return true;
}




/* 
*���ܣ���ȡ�ҵ����˹���
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CUnionManager::GetMyPlacard()
{
	if( CUnion*	pMyUnion = this->GetMyUnion() )
	{
		return pMyUnion->GetPlacard();
	}

	return "";
}



/* 
*���ܣ������ҵ����˹���
*ϸ�������� 
*������ szPlacard
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::SetMyPlacard( LPCTSTR szPlacard )
{
	if( CUnion*	pMyUnion = this->GetMyUnion() )
	{
		pMyUnion->SetPlacard( szPlacard );
		return true;
	}

	return false;
}



/* 
*���ܣ���ȡ�ҵ����˾ݵ���Ϣ
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
LPCTSTR	CUnionManager::GetMyBeachhead()
{
	if( CUnion*	pMyUnion = this->GetMyUnion() )
	{
		return pMyUnion->GetBeachhead();
	}

	return "";
}



/* 
*���ܣ������ҵ����˾ݵ���Ϣ
*ϸ�������� 
*������ szBeachhead
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::SetMyBeachhead( LPCTSTR szBeachhead )
{
	if( CUnion*	pMyUnion = this->GetMyUnion() )
	{
		pMyUnion->SetBeachhead( szBeachhead );
		return true;
	}

	return false;
}





/* 
*���ܣ���ȡ�ҵ�����
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion*	CUnionManager::GetMyUnion()
{
	if( CPlayer* pPlayer = GetGame()->GetMainPlayer() )
	{
		return this->GetUnion( pPlayer->GetUnionID() );
	}

	return NULL;
}


/* 
*���ܣ���ȡ���������
*ϸ�������� 
*������ guid
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
CUnion*	CUnionManager::GetFacUnion( CGUID guidFaction )
{
	for( std::map<CGUID,CUnion*>::iterator p = m_mapUnions.begin(); p != m_mapUnions.end(); p++ )
	{
		if( CUnion* pUnion = p->second )
		{
			if( pUnion->HasFaction( guidFaction ) )
				return pUnion;
		}
	}

	return NULL;
}




/* 
*���ܣ���ȡ�ҵ�Ȩ��
*ϸ�������� 
*������ guid
*����ֵ�� bool
*�޸�ʱ�䣺2008-11-5
*/
bool	CUnionManager::GetMyRights( eFactionPurviewIndex ePV )
{
	CUnion*	pMyUnion = this->GetMyUnion();
	CPlayer*	pMainPlayer = GetGame()->GetMainPlayer();

	// �Ƿ�������
	if( pMyUnion && pMainPlayer )
	{
		return true;
	}

	return false;
}



/*
*���ܣ� ���մ������˽ű� Э��
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CUnionManager::RecvScriptCreationMsg( CMessage* pMsg )
{
	//CUnionPageEx*	pUnionPageEx = GetGame()->GetCGuiEx()->GetUnionPageEx();

	//// ��� �ɹ�ʧ��
	//if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
	//{
	//	// �����˴���UI
	//	pUnionPageEx->OpenCreationUI();
	//}
	//else
	//{
	//	// �������
	//}
}



/*
*���ܣ� ���� ��������Э��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvCreationMsg( CMessage* pMsg )
{
	// ��� �ɹ�ʧ��
	if( PLAYER_OPT_SUCCEED == pMsg->GetLong() )
	{
		// "���˴����ɹ���"
		/*GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->m_lCallerID = -1;
		GetGame()->GetCGuiEx()->GetMessageBoxPageEx()->Open( GetGame()->GetCGuiEx()->GetFactionPageEx(), "���˴����ɹ���", CMessageBoxPageEx::MSG_OK, true );*/
        GetGame()->GetAudioList()->Play2DSound("SOUNDS\\interfaces\\i-19.wav"); 
	}
	else
	{
		// �������
	}
}



/*
*���ܣ� ���� ��������֪ͨ
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvCreationNotifyMsg( CMessage* pMsg )
{
	//// �л� GUID
	//CGUID	guidFaction;
	//pMsg->GetGUID( guidFaction );

	//// ���� GUID
	//CGUID	guidUnion;
	//pMsg->GetGUID( guidUnion );

	//// ���� ����
	//TCHAR	szUnionName[ MAX_MEMBER_NAME_SIZE ];
	//pMsg->GetStr( szUnionName, MAX_MEMBER_NAME_SIZE );


	//// ���һ������
	//tagUnionBaseData	stUnionInfo;
	//if( CUnion* pUnion = this->GetUnion( guidUnion ) )
	//{
	//	stUnionInfo = *pUnion->GetUnionInfo();
	//}

	//stUnionInfo.guid = guidUnion;
	//stUnionInfo.MasterFacGuid = guidFaction;
	//lstrcpy( stUnionInfo.szName, szUnionName );

	//this->AddUnion( &stUnionInfo );

	//// ��������� ���л�
	//if( ! this->GetUnion( guidUnion )->HasFaction( guidFaction ) )
	//{
	//	tagUnionMemInfo	stFacInfo;
	//	stFacInfo.MemberGuid = guidFaction;

	//	this->GetUnion( guidUnion )->AddFaction( &stFacInfo );
	//}


	//// ��������ǵ��лᴴ��������
	//if( this->AmIJoinedUnion( guidUnion ) )
	//{
	//	// ��ʾ��Ϣ
	//	//TCHAR	szPrompt[ 1024 ];
	//	//wsprintf( "�����ڵ��лᴴ�������ˣ�%s", szUnionName );
	//	//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, szPrompt, 0xffffffff );
	//}
}



/*
*���ܣ� һ���л��Ա��������֪ͨ
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CUnionManager::RecvJoinMsg( CMessage* pMsg )
{
	//// �л� GUID
	//CGUID	guidFaction;
	//pMsg->GetGUID( guidFaction );

	//// ���� GUID
	//CGUID	guidUnion;
	//pMsg->GetGUID( guidUnion );

	//// ���� ����
	//TCHAR	szUnionName[ MAX_MEMBER_NAME_SIZE ];
	//pMsg->GetStr( szUnionName, MAX_MEMBER_NAME_SIZE );

	//// ���һ������
	//tagUnionBaseData	stUnionInfo;
	//if( CUnion* pUnion = this->GetUnion( guidUnion ) )
	//{
	//	stUnionInfo = *pUnion->GetUnionInfo();
	//}

	//stUnionInfo.guid = guidUnion;
	//lstrcpy( stUnionInfo.szName, szUnionName );

	//this->AddUnion( &stUnionInfo );


	//// ��������� �л�
	//if( ! this->GetUnion( guidUnion )->HasFaction( guidFaction ) )
	//{
	//	tagUnionMemInfo	stFacInfo;
	//	stFacInfo.MemberGuid = guidFaction;

	//	this->GetUnion( guidUnion )->AddFaction( &stFacInfo );
	//}

	//// ��������ǵ��л����������
	//if( this->AmIJoinedUnion( guidUnion ) )
	//{
	//	// ��ʾ��Ϣ
	//	TCHAR	szPrompt[ 1024 ];
	//	wsprintf( "�����ڵ��л���������ˣ�%s", szUnionName );
	//	GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, szPrompt, 0xffffffff );
	//}
}



/*
*���ܣ� һ�����˵Ļ�������֪ͨ
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CUnionManager::RecvUnionInfoMsg( CMessage* pMsg )
{
	//// �������˻�����Ϣ
	//tagUnionBaseData	stUnionInfo;
	//pMsg->GetEx( &stUnionInfo, sizeof(stUnionInfo) );

	//// �����������£������¼�
	//this->AddUnion( &stUnionInfo );

	// ˢ��UI����
}



/*
*���ܣ� һ�����˵����г�Ա����֪ͨ
*ϸ�������� 
*������ ��
*����ֵ�� ��
*�޸�ʱ�䣺2008-11-5
*/
void	CUnionManager::RecvUnionMemberMsg( CMessage* pMsg )
{
	//// ���� GUID
	//CGUID	guidUnion;
	//pMsg->GetGUID( guidUnion );

	//// �����л��Ա��Ϣ
	//tagUnionMemInfo	stFacInfo;
	//pMsg->GetEx( &stFacInfo, sizeof(stFacInfo) );

	////
	//if( CUnion* pUnion = this->GetUnion( guidUnion ) )
	//{
	//	pUnion->AddFaction( &stFacInfo );
	//}
}



/*
*���ܣ� ���� ��������Э��
*ϸ�������� 
*������ strFactionName	�л�����
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendCreationMsg( LPCTSTR szName )
{
	// ��������Э��
	CMessage msg( MSG_C2S_UNION_Request_Creation );

		// ��������
		msg.Add( szName );

	msg.Send();
}



/*
*���ܣ� ���� ��ɢ����Э��
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendDisbandMsg()
{
	// ��ɢ����Э��
	CMessage msg( MSG_C2S_UNION_Request_Disband );

	msg.Send();
}



/*
*���ܣ� ���� ��ɢ���˻ظ�
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvDisbandMsg( CMessage* pMsg )
{
	// ���� GUID
	CGUID	guidUnion;
	pMsg->GetGUID( guidUnion );

	// ��������ǵ����˽�ɢ��
	if( this->AmIJoinedUnion( guidUnion ) )
	{
		// ��ʾ��Ϣ
		//TCHAR	szPrompt[ 1024 ];
		//wsprintf( "�����ڵ����ˣ�%s ��ɢ�ˣ�", szUnionName );
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, szPrompt, 0xffffffff );
	}

	// ��ɢ
	this->DelUnion( guidUnion );
}



/*
*���ܣ� ���� �˳�����Э��
*ϸ�������� 
*������ 
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendQuitMsg()
{
	CMessage msg( MSG_C2S_UNION_Request_Quit );

	msg.Send();
}



/*
*���ܣ� ���� �˳����˻ظ�
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvQuitMsg( CMessage* pMsg )
{
	// �л� GUID
	CGUID	guidFaction;
	pMsg->GetGUID( guidFaction );

	// ��������ǵ��л��˳�������
	//if( this->AmIJoinedUnion( guidUnion ) )
	{
		// ��ʾ��Ϣ
		//TCHAR	szPrompt[ 1024 ];
		//wsprintf( "�����ڵ����ˣ�%s ��ɢ�ˣ�", szUnionName );
		//GetGame()->GetCGuiEx()->GetMainBarPageEx()->AddText( eNOTIFYPOS_CENTER, szPrompt, 0xffffffff );
	}

	// ��ɢ
	//this->DelUnion( guidUnion );
}




/*
*���ܣ� ���� �߳�����Э��
*ϸ�������� 
*������ guidFaction
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendKickMsg( CGUID guidFaction )
{
	CMessage msg( MSG_C2S_UNION_Request_Kick );

		// �߳��л�� GUID
		msg.Add( guidFaction );

	msg.Send();
}



/*
*���ܣ� ���� �߳����˻ظ�
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvKickMsg( CMessage* pMsg )
{
	// �л� GUID
}



/*
*���ܣ� ���� ����ת������
*ϸ�������� 
*������ guidChairman
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendSetChairmanMsg( CGUID guidChairman )
{
	CMessage msg( MSG_C2S_UNION_Request_SetChairman );

	msg.Send();
}



/*
*���ܣ� ���� ����ת�ûظ�
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvSetChairmanMsg( CMessage* pMsg )
{
}



/*
*���ܣ� ���� ���������л�Ȩ������
*ϸ�������� 
*������ guidFaction
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendSetRightsMsg( CGUID guidFaction )
{
	CMessage msg( MSG_C2S_UNION_Request_SetRights );

	msg.Send();
}



/*
*���ܣ� �������� Ȩ�����ý���ظ�
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvSetRightsMsg( CMessage* pMsg )
{
}



/*
*���ܣ� �᳤���� �������õ�Ȩ��
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvGetRightsMsg( CMessage* pMsg )
{
}



/*
*���ܣ� ���� ���� �����л�᳤ �������˵ķ�������
*ϸ�������� 
*������ guidFaction
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendInviteMsg( LPCTSTR szName )
{
	CMessage msg( MSG_C2S_UNION_Request_Invite );

		// �������
		msg.Add( szName );

	msg.Send();
}



/*
*���ܣ� �л�᳤ �յ� ��������������˵�����
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvInviteMsg( CMessage* pMsg )
{
}


/*
*���ܣ� �л�᳤ ���ͻظ�����������
*ϸ�������� 
*������ guidFaction
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::SendResponseMsg( CGUID guidFaction )
{
	CMessage msg( MSG_C2S_UNION_Request_Invite );

	msg.Send();
}



/*
*���ܣ� ���� �յ� �л�᳤ �ظ���������
*ϸ�������� 
*������ pMsg	Э��ṹ
*����ֵ�� ��
*�޸�ʱ�䣺2008-4-16
*/
void	CUnionManager::RecvResponseMsg( CMessage* pMsg )
{
}

