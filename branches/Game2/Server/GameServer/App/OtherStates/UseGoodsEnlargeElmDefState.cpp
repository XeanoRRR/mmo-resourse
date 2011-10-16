#include "stdafx.h"
#include <mmsystem.h>
#include "UseGoodsEnlargeElmDefState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
#include "..\Monster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUseGoodsEnlargeElmDefState::CUseGoodsEnlargeElmDefState( DWORD dwTimeToKeep, DWORD dwCoefficient )
{
	//##
	m_lType					= STATE_TYPE_STANDARD;
	m_lID					= STATE_USER_GOODS_ENLARGE_ELM_DEF;
	m_dwTimeToKeep			= dwTimeToKeep;
	m_dwCoefficient			= dwCoefficient;
}

CUseGoodsEnlargeElmDefState::CUseGoodsEnlargeElmDefState()
{
	//##
	m_lType					= STATE_TYPE_STANDARD;
	m_lID					= STATE_USER_GOODS_ENLARGE_ELM_DEF;
	m_dwTimeToKeep			= 0;
	m_dwCoefficient			= 1;
}
CUseGoodsEnlargeElmDefState::~CUseGoodsEnlargeElmDefState()
{
}

DWORD CUseGoodsEnlargeElmDefState::GetRemainedTime()
{
	if( timeGetTime() >= m_dwTimeToKeep + m_dwTimeStamp )
		return 0;
	return m_dwTimeToKeep + m_dwTimeStamp - timeGetTime();
}

BOOL CUseGoodsEnlargeElmDefState::OnUpdateProperties()
{
	CMoveShape *pUser = GetSufferer();
	if( pUser == NULL ) return FALSE;

	if( m_pCurrentVisualEffect )
		m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );

	if( pUser -> GetType() == TYPE_PLAYER ) 
	{
		CPlayer *pPlayer = (CPlayer*)( pUser );
		if( pPlayer )
		{
//			DWORD dwEnlarge = DWORD( pPlayer->GetElementResistant() * (float)(m_dwCoefficient/100.0f) );
//			pPlayer->SetElementResistant( WORD(pPlayer->GetElementResistant() + dwEnlarge) );
		}
	}
	return TRUE;
}

BOOL CUseGoodsEnlargeElmDefState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	if( pUser == NULL )
		return FALSE;
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CUseGoodsEnlargeElmDefVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( FALSE );
	//m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	return TRUE;
}

BOOL CUseGoodsEnlargeElmDefState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	if( pUser == NULL )
		return FALSE;
	CState::Begin( pUser, lX, lY );
	m_pCurrentVisualEffect = new CUseGoodsEnlargeElmDefVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( FALSE );
	//m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	return TRUE;
}

BOOL CUseGoodsEnlargeElmDefState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	if( pUser == NULL )
		return FALSE;
	CState::Begin( pUser, otType, lID, lRegion );
	m_pCurrentVisualEffect = new CUseGoodsEnlargeElmDefVisualEffect;
	m_pCurrentVisualEffect -> BeginVisualEffect( FALSE );
	//m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	return TRUE;
}

VOID CUseGoodsEnlargeElmDefState::End()
{
	//##���úý����ı�־���Ա�����ѯ�Ƿ����
	m_bIsEnded = TRUE;
	CMoveShape* pShape = GetSufferer();
	if( pShape )
	{
		pShape -> RemoveState( this );
	}
	//CState::End();
	//m_pCurrentVisualEffect -> UpdateVisualEffect( this, 1 );
}

VOID CUseGoodsEnlargeElmDefState::AI()
{
	//##�ж�ʱ��
	if( timeGetTime() > m_dwTimeStamp + m_dwTimeToKeep )
	{
		End();
	}
}


//##������л�����������Ϸ������֮�䴫������
VOID CUseGoodsEnlargeElmDefState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##ʣ��ʱ��
	_AddToByteArray( &vOut, GetRemainedTime() );


	_AddToByteArray( &vOut, m_dwCoefficient );
}

VOID CUseGoodsEnlargeElmDefState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����
	m_dwTimeStamp	= timeGetTime();
	m_dwTimeToKeep	= _GetDwordFromByteArray( pBytes, lPos );


	m_dwCoefficient = _GetLongFromByteArray( pBytes, lPos );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CUseGoodsEnlargeElmDefState::OnChangeRegion( LONG lRegionID )
{
	m_lUserRegion = lRegionID;
}


VOID CUseGoodsEnlargeElmDefVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	if( pParent && !m_bEnded )
	{
		CMoveShape* pShape = pParent -> GetSufferer();
		if( pShape )
		{
			CMessage msg( MSG_S2C_SKILL_ADD_EX_STATE );
			msg.Add( pShape -> GetType() );
			msg.Add( pShape -> GetID() );
			msg.Add( pParent -> GetID() );
			msg.Add( pParent -> GetRemainedTime() );
			msg.Add( pParent -> GetAdditionalData() );
			msg.SendToAround( pShape );
		}
	}
	CVisualEffect::UpdateVisualEffect( pParent );
}