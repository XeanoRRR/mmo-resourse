#include "stdafx.h"
#include <MMSystem.h>
#include "AutomaticRestoreHpState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\Monster.h"
#include "../setup/GlobeSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAutomaticRestoreHpState::CAutomaticRestoreHpState( DWORD dwFrequency, FLOAT fVolume )
{
	//##
	m_lType			= STATE_TYPE_STANDARD;
	m_lID			= STATE_AUTOMATIC_RESTORE_HP;
	m_dwFrequency	= dwFrequency;
	m_fVolume		= fVolume;
	m_pCurrentVisualEffect = NULL;
}

CAutomaticRestoreHpState::CAutomaticRestoreHpState()
{
	//##
	m_lType			= STATE_TYPE_STANDARD;
	m_lID			= STATE_AUTOMATIC_RESTORE_HP;
	m_dwFrequency	= 10000;
	m_fVolume		= 0.01f;
	m_pCurrentVisualEffect = NULL;
}


CAutomaticRestoreHpState::~CAutomaticRestoreHpState()
{
	SAFE_DELETE(m_pCurrentVisualEffect);
}

BOOL CAutomaticRestoreHpState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CAutomaticRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

BOOL CAutomaticRestoreHpState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	CState::Begin( pUser, lX, lY );
	m_pCurrentVisualEffect = new CAutomaticRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

BOOL CAutomaticRestoreHpState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	CState::Begin( pUser, otType, lID, lRegion );
	m_pCurrentVisualEffect = new CAutomaticRestoreHpStateVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
	return TRUE;
}

VOID CAutomaticRestoreHpState::End()
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

VOID CAutomaticRestoreHpState::AI()
{
	//##��ȡ������
	CMoveShape* pSufferer = GetSufferer();
	if( pSufferer == NULL )
	{
		End();
		return;
	}

	if( pSufferer -> IsDied() || pSufferer -> GetHP() == 
		pSufferer -> GetMaxHP() ) 
	{
		return;
	}

	//##��Ѫ
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
				LONG lRestoreHp = static_cast<LONG>( pSufferer -> GetMaxHP() * fAdd );
				if( lRestoreHp < 1 ) lRestoreHp = 1;

				LONG lHp = pSufferer -> GetHP() + lRestoreHp;
				if( lHp > pSufferer -> GetMaxHP() )
				{
					lHp = pSufferer -> GetMaxHP();
				}

				pSufferer -> SetHP( static_cast<DWORD>(lHp) );
				pSufferer -> OnChangeStates();
			}
		}

		m_dwTimeStamp = timeGetTime();
	}
}

VOID CAutomaticRestoreHpState::OnChangeRegion( LONG lRegionID )
{
	//##���״̬������Sufferer���ϣ�ֻ�ı�Sufferer��RID
	m_lSuffererRegion = lRegionID;
}

//##������л�����������Ϸ������֮�䴫������
VOID CAutomaticRestoreHpState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##Ƶ��
	_AddToByteArray( &vOut, m_dwFrequency );

	//##��Ѫ��
	_AddToByteArray( &vOut, m_fVolume );
	
}

VOID CAutomaticRestoreHpState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����

	//##�ָ�ʱ��
	m_dwTimeStamp	= timeGetTime();

	//##Ƶ��
	m_dwFrequency	= _GetDwordFromByteArray( pBytes, lPos );

	//##Ѫ
	m_fVolume		= _GetFloatFromByteArray( pBytes, lPos );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CAutomaticRestoreHpStateVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	CVisualEffect::UpdateVisualEffect( pParent );
}