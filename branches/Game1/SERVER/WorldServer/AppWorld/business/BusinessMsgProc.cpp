///
/// @file BusinessMsgProc.cpp
/// @brief ������ҵϵͳ�����Ϣ
///
#include "StdAfx.h"
#include "../nets/networld/message.h"
#include "CBusinessManager.h"
#include "../DBEntity/EntityManager.h"

using namespace Business;

/// ����浵��Ϣ
static void OnBusinessSaveMsg( CMessage *pMsg )
{
	DBReadSet db;
	pMsg->GetDBReadSet( db );
	// ����浵������WS
	GetInst( CBusinessManager ).DecodeFromGS( db );
	// ����浵���󣬴�ʱĳЩ���ݿ��ܲ�û����
	GetGame()->GetEntityManager()->CreateSaveBusinessSession();

	BUSINESS_LOG_INFO( "Start save some business data" );
}

void OnBusinessMsg( CMessage *pMsg )
{
	switch( pMsg->GetType() )
	{
	case MSG_S2W_BUSINESS_SAVE:
		OnBusinessSaveMsg( pMsg );
		break;

	default:
		BUSINESS_LOG_WARNING( FMT_STR( "Unknonw message [%d].", pMsg->GetType() ) );
	}
}