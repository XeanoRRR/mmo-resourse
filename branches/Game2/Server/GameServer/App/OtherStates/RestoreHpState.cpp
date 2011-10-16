#include "stdafx.h"
#include <mmsystem.h>
#include "RestoreHpState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\Monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRestoreHpState::CRestoreHpState( DWORD dwPersistTime, DWORD dwFrequency, DWORD dwHPGain )
{
	//##
	m_lType				= STATE_TYPE_STANDARD;
	m_lID				= STATE_RESTORE_HP;
	m_dwTimeToKeep		= dwPersistTime;
	m_dwFrequency		= dwFrequency;
	m_dwHPGain			= dwHPGain;
	m_dwRestoreHpCounts	= 0;
}

CRestoreHpState::CRestoreHpState()
{
	//##
	m_lType				= STATE_TYPE_STANDARD;
	m_lID				= STATE_RESTORE_HP;
	m_dwTimeToKeep		= 0;
	m_dwFrequency		= 0;
	m_dwHPGain			= 0;
	m_dwRestoreHpCounts	= 0;
}


CRestoreHpState::~CRestoreHpState()
{
}

DWORD CRestoreHpState::GetRemainedTime()
{
	if( timeGetTime() >= m_dwTimeToKeep + m_dwTimeStamp )
		return 0;
	return m_dwTimeToKeep + m_dwTimeStamp - timeGetTime();
}

BOOL CRestoreHpState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreHpCounts = 0;
	return TRUE;
}

BOOL CRestoreHpState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	CState::Begin( pUser, lX, lY );
	m_pCurrentVisualEffect = new CRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreHpCounts = 0;
	return TRUE;
}

BOOL CRestoreHpState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	CState::Begin( pUser, otType, lID, lRegion );
	m_pCurrentVisualEffect = new CRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	m_dwRestoreHpCounts = 0;
	return TRUE;
}

VOID CRestoreHpState::End()
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

VOID CRestoreHpState::AI()
{
	//##��ȡ������
	CMoveShape* pSufferer = GetSufferer();
	if( !pSufferer )
	{
		End();
		return;
	}

	if( pSufferer -> IsDied() ) return;

	//##��Ѫ
	if( timeGetTime() > m_dwTimeStamp + m_dwRestoreHpCounts * m_dwFrequency )
	{
		//##�޸ļ�Ѫ������
		++ m_dwRestoreHpCounts;

		LONG lHp = pSufferer -> GetHP() + m_dwHPGain;
		if( lHp > pSufferer -> GetMaxHP() )
		{
			lHp = pSufferer -> GetMaxHP();
		}

		pSufferer -> SetHP( static_cast<DWORD>(lHp) );
		pSufferer -> OnChangeStates();
	}

	//##�ж�ʱ��
	if( timeGetTime() > m_dwTimeStamp + m_dwTimeToKeep )
	{
		End();
	}
}

VOID CRestoreHpState::OnChangeRegion( LONG lRegionID )
{
	//##���״̬������Sufferer���ϣ�ֻ�ı�Sufferer��RID
	m_lSuffererRegion = lRegionID;
}

//##������л�����������Ϸ������֮�䴫������
VOID CRestoreHpState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##ʣ��ʱ��
	_AddToByteArray( &vOut, GetRemainedTime() );

	//##Ƶ��
	_AddToByteArray( &vOut, m_dwFrequency );

	//##��Ѫ��
	_AddToByteArray( &vOut, m_dwHPGain );
	
}

VOID CRestoreHpState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����

	//##�ָ�ʱ��
	m_dwTimeStamp	= timeGetTime();
	m_dwTimeToKeep	= _GetDwordFromByteArray( pBytes, lPos );

	//##Ƶ��
	m_dwFrequency	= _GetDwordFromByteArray( pBytes, lPos );

	//##��Ѫ
	m_dwHPGain		= _GetDwordFromByteArray( pBytes, lPos );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CRestoreHpStateVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	CVisualEffect::UpdateVisualEffect( pParent );
}