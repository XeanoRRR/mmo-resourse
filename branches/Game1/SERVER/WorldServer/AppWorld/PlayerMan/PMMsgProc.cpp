#include "stdafx.h"
#include "PlayerMan.h"
#include "..\..\worldserver\game.h"
#include "..\..\nets\networld\message.h"



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
			CPlayer *pPlayer = GetGame()->GetMapPlayer(ExID);
			long lType = pMsg->GetLong();
			long lNewVal = pMsg->GetLong();
			if( NULL == pPlayer )
			{
				char szLog[512];
				char szGUID[128];
				ExID.tostring(szGUID);
				sprintf(szLog, "����ˢ��TYPE:0X%XΪ%ld�ĵ����(GUIDΪ%s)������.",
					lType,
					lNewVal,
					szGUID);
				PutoutLog("PM", LT_ERROR, szLog);
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