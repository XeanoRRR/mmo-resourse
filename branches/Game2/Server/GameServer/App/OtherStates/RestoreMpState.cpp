#include "stdafx.h"
#include <mmsystem.h>
#include "RestoreMpState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\Monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRestoreMpState::CRestoreMpState( DWORD dwPersistTime, DWORD dwFrequency, DWORD dwMPGain )
{
	//##
	m_lType					= STATE_TYPE_STANDARD;
	m_lID					= STATE_RESTORE_MP;
	m_dwTimeToKeep			= dwPersistTime;
	m_dwFrequency			= dwFrequency;
	m_dwMPGain				= dwMPGain;
	m_dwRestoreMpCounts		= 0;
}

CRestoreMpState::CRestoreMpState()
{
	//##
	m_lType					= STATE_TYPE_STANDARD;
	m_lID					= STATE_RESTORE_MP;
	m_dwTimeToKeep			= 0;
	m_dwFrequency			= 0;
	m_dwMPGain				= 0;
	m_dwRestoreMpCounts		= 0;
}


CRestoreMpState::~CRestoreMpState()
{
}

DWORD CRestoreMpState::GetRemainedTime()
{
	if( timeGetTime() >= m_dwTimeToKeep + m_dwTimeStamp )
		return 0;
	return m_dwTimeToKeep + m_dwTimeStamp - timeGetTime();
}

BOOL CRestoreMpState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreMpCounts = 0;
	return TRUE;
}

BOOL CRestoreMpState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	CState::Begin( pUser, lX, lY );
	m_pCurrentVisualEffect = new CRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreMpCounts = 0;
	return TRUE;
}

BOOL CRestoreMpState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	CState::Begin( pUser, otType, lID, lRegion );
	m_pCurrentVisualEffect = new CRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreMpCounts = 0;
	return TRUE;
}

VOID CRestoreMpState::End()
{
	CMoveShape* pShape = GetSufferer();
	if( pShape )
	{
		pShape -> RemoveState( this );
	}
	else
	{
		//##��ȡ���������ߣ��п��ܡ�
	}
}

VOID CRestoreMpState::AI()
{
	//##��ȡ������
	CPlayer* pSufferer = dynamic_cast<CPlayer*>( GetSufferer() );
	if( !pSufferer )
	{
		End();
		return;
	}

	if( pSufferer -> IsDied() ) return;

	//##��MP
	if( timeGetTime() > m_dwTimeStamp + m_dwRestoreMpCounts * m_dwFrequency )
	{
		//##�޸ļ�����
		++ m_dwRestoreMpCounts;

		LONG lMp = pSufferer -> GetMP() + m_dwMPGain;
		if( lMp > pSufferer -> GetMaxMP() )
		{
			lMp = pSufferer -> GetMaxMP();
		}

		pSufferer -> SetMP( static_cast<WORD>(lMp) );
		pSufferer -> OnChangeStates();
	}

	//##�ж�ʱ��
	if( timeGetTime() > m_dwTimeStamp + m_dwTimeToKeep )
	{
		End();
	}
}

VOID CRestoreMpState::OnChangeRegion( LONG lRegionID )
{
	//##���״̬������Sufferer���ϣ�ֻ�ı�Sufferer��RID
	m_lSuffererRegion = lRegionID;
}

//##������л�����������Ϸ������֮�䴫������
VOID CRestoreMpState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##ʣ��ʱ��
	_AddToByteArray( &vOut, GetRemainedTime() );

	//##Ƶ��
	_AddToByteArray( &vOut, m_dwFrequency );

	//##��MP��
	_AddToByteArray( &vOut, m_dwMPGain );
	
}

VOID CRestoreMpState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����

	//##�ָ�ʱ��
	m_dwTimeStamp	= timeGetTime();
	m_dwTimeToKeep	= _GetDwordFromByteArray( pBytes, lPos );

	//##Ƶ��
	m_dwFrequency	= _GetDwordFromByteArray( pBytes, lPos );

	//##MP
	m_dwMPGain		= _GetDwordFromByteArray( pBytes, lPos );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CRestoreMpStateVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	CVisualEffect::UpdateVisualEffect( pParent );
}