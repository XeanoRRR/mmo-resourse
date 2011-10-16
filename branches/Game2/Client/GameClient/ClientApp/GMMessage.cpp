#include "stdafx.h"
#include "..\nets\netclient\myNetClient.h"
#include "..\nets\netclient\message.h"
#include "gameclient\game.h"

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
	case CMessage::MSG_S2C_GM_ANSWER_GMMODE:
		/**## ���������ؿͻ�������GMģʽ�Ƿ�ɹ� */
		/**## ��Ϣ��ʽ��{ MSG_S2C_GM_ANSWER_GMMODE, LONG GMLevel } */
		/**## GMLevel�Ķ���μ�GMList.h */
		{
			long lLevel = pMsg -> GetLong();
			if ( lLevel > 0 )
			{
				GetGame() -> SetGMMode();
			}
		}
		break;
	}
}