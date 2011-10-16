#include "StdAfx.h"
#include "PlayerMan.h"
#include "../../Server/Game.h"
#include "../../Net/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��Ӧ��ҹ�����Ϣ
void OnPMMessage(CMessage* pMsg)
{
	assert(NULL != pMsg);

	switch(pMsg->GetType())
	{
	case MSG_S2W_PM_CHG:		// GS������������Ϣ
		{
			CGUID ExID;
			pMsg->GetGUID(ExID);
			CPlayer *pPlayer = GetGame().GetMapPlayer(ExID);
			long lType = pMsg->GetLong();
			long lNewVal = pMsg->GetLong();
			if( NULL == pPlayer )
			{
				//char szLog[512];
				char szGUID[128];
				ExID.tostring(szGUID);
				//sprintf(szLog, "����ˢ��TYPE:0X%XΪ%ld�ĵ����(GUIDΪ%s)������.",
				//	lType,
				//	lNewVal,
				//	szGUID);
				//PutoutLog("PM", LT_ERROR, szLog);

                Log4c::Error(ROOT_MODULE,"%-15s ����ˢ��TYPE:0X%XΪ%ld�ĵ����(GUIDΪ%s)������.",__FUNCTION__,lType,lNewVal,szGUID);
				break;
			}

			GetInst(CPlayerMan).RefreshElem(lType, pPlayer, lNewVal);
		}
		break;

	case MSG_S2W_PM_REQ:		// GS�����ѯ�����Ϣ
		{
			GetInst(CPlayerMan).Query(pMsg);
		}
		break;

	}
}