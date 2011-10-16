#include "stdafx.h"
#include <MMSystem.h>
#include "AutomaticRestoreMpState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "../setup/GlobeSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAutomaticRestoreMpState::CAutomaticRestoreMpState( DWORD dwFrequency, FLOAT fVolume )
{
	//##
	m_lType			= STATE_TYPE_STANDARD;
	m_lID			= STATE_AUTOMATIC_RESTORE_MP;
	m_dwFrequency	= dwFrequency;
	m_fVolume		= fVolume;
}

CAutomaticRestoreMpState::CAutomaticRestoreMpState()
{
	//##
	m_lType			= STATE_TYPE_STANDARD;
	m_lID			= STATE_AUTOMATIC_RESTORE_MP;
	m_dwFrequency	= 10000;
	m_fVolume		= 0.01f;
}


CAutomaticRestoreMpState::~CAutomaticRestoreMpState()
{
}

BOOL CAutomaticRestoreMpState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CAutomaticRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

BOOL CAutomaticRestoreMpState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	CState::Begin( pUser, lX, lY );
	m_pCurrentVisualEffect = new CAutomaticRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

BOOL CAutomaticRestoreMpState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	CState::Begin( pUser, otType, lID, lRegion );
	m_pCurrentVisualEffect = new CAutomaticRestoreMpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

VOID CAutomaticRestoreMpState::End()
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

VOID CAutomaticRestoreMpState::AI()
{
	//##��ȡ������
	CPlayer* pSufferer = dynamic_cast<CPlayer*>( GetSufferer() );
	if( pSufferer == NULL )
	{
		End();
		return;
	}

	if( pSufferer -> IsDied() || pSufferer -> GetMP() == 
		pSufferer -> GetMaxMP() ) 
	{
		return;
	}

	//##��Mp
	if( timeGetTime() > m_dwTimeStamp + m_dwFrequency )
	{
		// �����ƽ״̬�ż�Ѫ soft 2006.5.31
		if (pSufferer->GetState() == CShape::STATE_PEACE)
		{
			float fAdd = m_fVolume - (float)pSufferer->GetLevel() / CGlobeSetup::GetSetup()->lResumeFactor;

			// ����ϵ�� soft 2006.12.04
			if (fAdd < CGlobeSetup::GetSetup()->fResumeFactorMini)
				fAdd = CGlobeSetup::GetSetup()->fResumeFactorMini;

			if (fAdd > 0.f)
			{
				LONG lRestoreMp = static_cast<LONG>( pSufferer -> GetMaxMP() * fAdd );
				if( lRestoreMp < 1 ) lRestoreMp = 1;

				LONG lMp = pSufferer -> GetMP() + lRestoreMp;
				if( lMp > pSufferer -> GetMaxMP() )
				{
					lMp = pSufferer -> GetMaxMP();
				}

				pSufferer -> SetMP( static_cast<WORD>(lMp) );
				pSufferer -> OnChangeStates();
			}
		}

		m_dwTimeStamp = timeGetTime();
	}
}

VOID CAutomaticRestoreMpState::OnChangeRegion( LONG lRegionID )
{
	//##���״̬������Sufferer���ϣ�ֻ�ı�Sufferer��RID
	m_lSuffererRegion = lRegionID;
}

//##������л�����������Ϸ������֮�䴫������
VOID CAutomaticRestoreMpState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##Ƶ��
	_AddToByteArray( &vOut, m_dwFrequency );

	//##��MP��
	_AddToByteArray( &vOut, m_fVolume );
	
}

VOID CAutomaticRestoreMpState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����

	//##�ָ�ʱ��
	m_dwTimeStamp	= timeGetTime();

	//##Ƶ��
	m_dwFrequency	= _GetDwordFromByteArray( pBytes, lPos );

	//##MP
	m_fVolume		= _GetFloatFromByteArray( pBytes, lPos );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CAutomaticRestoreMpStateVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	CVisualEffect::UpdateVisualEffect( pParent );
}