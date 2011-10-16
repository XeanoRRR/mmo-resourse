#include "stdafx.h"
#include "../../../Net/Message.h"
#include "../../../GameClient/Game.h"
#include "../../../GameClient/ClientApp/ClientRegion.h"
#include "../ClientApp/Player.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��Ӧ������Ϣ
void OnGMMessage( CMessage* pMsg )
{
	if( pMsg == NULL ) return;

	switch( pMsg -> GetType() )
	{
	case MSG_S2C_GM_ANSWER_GMMODE:
		/**## ���������ؿͻ�������GMģʽ�Ƿ�ɹ� */
		/**## ��Ϣ��ʽ��{ MSG_S2C_GM_ANSWER_GMMODE, LONG GMLevel } */
		/**## GMLevel�Ķ���μ�GMList.h */
		{
			long lLevel = pMsg -> GetLong();
			if ( lLevel > 0 )
			{
				if( GetGame() && GetGame() -> GetMainPlayer() )
				{
					GetGame() -> SetGMMode( true );
					GetGame() -> GetMainPlayer() -> SetGMLevel( lLevel );
				}
			}
		}
		break;
	case MSG_S2C_GM_VIEW_HIDENAME_AROUND:
		{
			CClientRegion * pRegion = GetGame()->GetRegion();
			if (pRegion == NULL)
				break;
			CGUID guid = CGUID::GUID_INVALID;
			char strName[512] = "";
			string strPlayerName = "";
			long size = pMsg->GetLong();
			for (long i = 0; i < size; ++i)
			{
				pMsg->GetGUID(guid);
				strPlayerName = pMsg->GetStr(strName,512);
				CPlayer *pPlayer = dynamic_cast<CPlayer *>(pRegion->FindChildObject(TYPE_PLAYER, guid));
				if (pPlayer)
					pPlayer->SetName(strPlayerName.c_str());
			}
		}
		break;
	default:
		break;
	}
}