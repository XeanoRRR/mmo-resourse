#include "stdafx.h"
#include <mmsystem.h>
#include "ExState.h"
#include "..\MoveShape.h"
#include "..\Player.h"
//#include "..\Monster.h"
#include "../Listener/CSeekGoodsListener.h"
#include "../Goods/CGoodsFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExState::CExState( tagExState &stState )
:_last_time_stamp(0)
{
	//##
	m_lType				= STATE_TYPE_STANDARD;
	m_lID				= SKILL_EX_STATE;
	memcpy(&m_stStateParam, &stState, sizeof(tagExState));
}

CExState::CExState()
{
	//##
	m_lType				= STATE_TYPE_STANDARD;
	m_lID				= SKILL_EX_STATE;
	memset(&m_stStateParam, 0, sizeof(tagExState));
}

CExState::~CExState()
{
}

DWORD CExState::GetRemainedTime()
{
	if( !m_stStateParam.keeptime ) return 0;

	if( timeGetTime() >= m_stStateParam.keeptime + m_dwTimeStamp )
		return 0;
	return m_stStateParam.keeptime + m_dwTimeStamp - timeGetTime();
}

BOOL CExState::OnUpdateProperties()
{
	CMoveShape* pUser = GetSufferer();
	if( pUser == NULL ) return FALSE;

	//if( m_pCurrentVisualEffect )
	//{
	//	m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	//}

	if( pUser -> GetType() == TYPE_PLAYER ) 
	{
		CPlayer* pPlayer = (CPlayer*)( pUser );
		if( pPlayer )
		{
			if( m_stStateParam.maxhp )
				pPlayer->SetMaxHP( pPlayer->GetMaxHP() + m_stStateParam.maxhp );
			if( m_stStateParam.maxmp )
				pPlayer->SetMaxMP( pPlayer->GetMaxMP() + m_stStateParam.maxmp );

			if( m_stStateParam.minatk )
				pPlayer->SetMinAtk( pPlayer->GetMinAtk() + m_stStateParam.minatk );
			if( m_stStateParam.maxatk )
				pPlayer->SetMaxAtk( pPlayer->GetMaxAtk() + m_stStateParam.maxatk );
			if( m_stStateParam.elm )
//				pPlayer->SetElementModify( pPlayer->GetElementModify() + m_stStateParam.elm );

			if( m_stStateParam.def )
				pPlayer->SetDef( pPlayer->GetDef() + m_stStateParam.def);
			if( m_stStateParam.elmdef )
//				pPlayer->SetElementResistant( pPlayer->GetElementResistant() + m_stStateParam.elmdef );

			if( m_stStateParam.cch )
//				pPlayer->SetCCH( pPlayer->GetCCH() + m_stStateParam.cch);
			if( m_stStateParam.fullmiss )
//				pPlayer->SetFullMiss( pPlayer->GetFullMiss() + m_stStateParam.fullmiss );

			if( m_stStateParam.atkavoid )
//				pPlayer->SetAttackAvoid( pPlayer->GetAttackAvoid() + m_stStateParam.atkavoid);
			if( m_stStateParam.elmavoid )
//				pPlayer->SetElementAvoid( pPlayer->GetElementAvoid() + m_stStateParam.elmavoid );

			if( m_stStateParam.hit )
				pPlayer->SetHit( pPlayer->GetHit() + m_stStateParam.hit);
			if( m_stStateParam.dodge )
				pPlayer->SetDodge( pPlayer->GetDodge() + m_stStateParam.dodge );
		}
	}

	return TRUE;
}

BOOL CExState::Begin( CMoveShape* pUser, CMoveShape* pSufferer )
{
	if( pSufferer == NULL )
		return FALSE;
	CState::Begin( pUser, pSufferer );
	m_pCurrentVisualEffect = new CExStateVisualEffect;
	if( m_pCurrentVisualEffect )
	{
		m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
		m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	}

	return TRUE;
}

BOOL CExState::Begin( CMoveShape* pUser, LONG lX, LONG lY )
{
	CState::Begin( pUser, lX, lY );
	if( GetSufferer() == NULL )
	{
		End();
		return FALSE;
	}
	m_pCurrentVisualEffect = new CExStateVisualEffect;
	if( m_pCurrentVisualEffect )
	{
		m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
		m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	}

	return TRUE;
}

BOOL CExState::Begin( CMoveShape* pUser, OBJECT_TYPE otType, LONG lID, LONG lRegion )
{
	CState::Begin( pUser, otType, lID, lRegion );
	if( GetSufferer() == NULL )
	{
		End();
		return FALSE;
	}
	m_pCurrentVisualEffect = new CExStateVisualEffect;
	if( m_pCurrentVisualEffect )
	{
		m_pCurrentVisualEffect -> BeginVisualEffect( TRUE );
		m_pCurrentVisualEffect -> UpdateVisualEffect( this, 0 );
	}

	return TRUE;
}

VOID CExState::End()
{
	if( m_pCurrentVisualEffect )
	{
		m_pCurrentVisualEffect -> UpdateVisualEffect( this, 1 );
	}
	CMoveShape* pShape = GetSufferer();
	if( pShape )
	{
		pShape->RemoveState( this );
	}
	else
	{
		//##��ȡ���������ߣ��п��ܡ�
	}
}



VOID CExState::AI()
{
	// ��������˳���ʱ�䣬����Ҫ��ʱ����״̬��
	if( m_stStateParam.keeptime && timeGetTime() > m_dwTimeStamp + m_stStateParam.keeptime )
	{
		End();
		return;
	}

	// ��1�β�ʹ�õ��ߣ��͸�_last_time_stampһ����ʼֵ
	if( _last_time_stamp == 0 )
		_last_time_stamp = m_dwTimeStamp;

	if( m_stStateParam.frequency && 
		m_stStateParam.item_idx &&
		m_stStateParam.item_num &&
		timeGetTime() > _last_time_stamp + m_stStateParam.frequency )
	{
		_last_time_stamp = timeGetTime();

		if( 0 == use_item(m_stStateParam.item_idx, m_stStateParam.item_num) )
		{
			End();
			return;
		}
	}
}
int CExState::use_item(DWORD item_idx, DWORD item_num)
{
	CPlayer* pOwner;
	CSeekGoodsListener lSeeker;
	char msg[256];
	long ret=0;
	pOwner = (CPlayer*)GetSufferer();

	if( pOwner )
	{
		if( pOwner->check_item_in_packet(item_idx) >= item_num )
		{
			ret = pOwner->remove_item_in_packet(item_idx, item_num);
			if( ret == item_num )
			{
				return ret;
			}
		}
		sprintf( msg, "Դ���� %s ���Ĵ�����������ֹ�ˣ�",CGoodsFactory::QueryGoodsName(item_idx) );
		pOwner->SendSystemInfo( msg );
	}

	return ret;
}

VOID CExState::OnChangeRegion( LONG lRegionID )
{
	//##���״̬������User����.
	m_lUserRegion		= lRegionID;
}


//##������л�����������Ϸ������֮�䴫������
VOID CExState::Serialize( vector<BYTE>& vOut )
{
	//##�Ƿ���Ҫ�����ܵ�id�͵ȼ���ѹ�룿
	//##���״ֻ̬�м���ID������Ҫѹ��ȼ���������Щ������״̬��Ҫ�ȼ�
	_AddToByteArray( &vOut, m_lID );

	//##ʣ��ʱ��
	m_stStateParam.keeptime	= GetRemainedTime();

	_AddToByteArray( &vOut, &m_stStateParam, sizeof(tagExState) );

}

VOID CExState::Unserialize( BYTE* pBytes, LONG& lPos )
{
	//##״̬�ļ���ID�Ѿ���������CStateFactory��ȡ�������Բ���Ҫ�ٴ�ȡ����

	//##�ָ�ʱ��
	m_dwTimeStamp	= timeGetTime();

	_GetBufferFromByteArray( pBytes, lPos, &m_stStateParam,sizeof(tagExState) );

	//##֮��ֱ�ӵ���Begin�Ϳ����������״̬
}

VOID CExStateVisualEffect::UpdateVisualEffect( CState* pParent, DWORD dwCmd )
{
	if( pParent && !m_bEnded )
	{
		CMoveShape* pShape = pParent -> GetSufferer();
		CExState *pState = (CExState*)pParent;

		if( pShape && pState )
		{
			switch( dwCmd )
			{
			case 0:
				{
					CMessage msg( MSG_S2C_SKILL_ADD_EX_STATE );
					msg.Add( pShape -> GetType() );
					msg.Add( pShape -> GetID() );
					msg.Add( pState -> GetID() );
					msg.Add( pState->m_stStateParam.level );
					msg.Add( pState -> GetRemainedTime() );
					msg.Add( pState -> GetAdditionalData() );
					msg.SendToAround( pShape );
				}
				break;
			case 1:
				{
					CMessage msg( MSG_S2C_SKILL_END_EX_STATE );
					msg.Add( pShape -> GetType() );
					msg.Add( pShape -> GetID() );
					msg.Add( pState -> GetID() );
					msg.Add( pState->m_stStateParam.level );
					msg.SendToAround( pShape );
				}
				break;
			}
		}
	}
	CVisualEffect::UpdateVisualEffect( pParent );
}