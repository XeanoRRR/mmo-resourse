
#include "StdAfx.h"
#include "..\nets\networld\message.h"
#include "Linkman\LinkmanSystem.h"
#include "organizingsystem\OrganizingCtrl.h"

void OnLinkmanMan_DBS(CMessage *pMsg)
{
	DWORD curTime = timeGetTime();
	switch(pMsg->GetType())
	{
	case MSG_DB2W_LINKMAN_PLAYER_DATA_SAVE_NOTIFY:
		{
		}
		break;
	case MSG_DB2W_LEAVEWORD_SAVE_RESULT:
		{
		}
		break;
	case MSG_DB2W_LEAVEWORD_RE_POP:
		{
		}
		break;
	}
}



void OnLinkmanMan_GS(CMessage *pMsg)
{
	DWORD curTime = timeGetTime();
	switch(pMsg->GetType())
	{
	case  MSG_S2W_LINKMAN_SYSTEM_REQUEST_ADD:		//! ϵͳ�����ϵ������
		{
			LinkmanSystem::GetInstance().OnSystemRequestAddMsg(pMsg);
		}
		break;
	case  MSG_S2W_LINKMAN_PALYER_REQUEST_ADD:		//! ��������ϵ������
		{
			LinkmanSystem::GetInstance().OnPlayerRequestAddMsg(pMsg);
		}
		break;
	case  MSG_S2W_LINKMAN_PALYER_RE_ADD:			//! ��һظ������ϵ������
		{
			LinkmanSystem::GetInstance().OnPlayerReAddMsg(pMsg);
		}
		break;
	case  MSG_S2W_LINKMAN_PALYER_REQUEST_MOVE:		//! ����ƶ���ϵ������
		{
			LinkmanSystem::GetInstance().OnPlayerRequestMoveMsg(pMsg);
		}
		break;
	case  MSG_S2W_LINKMAN_PALYER_REQUEST_DEL:		//! ���ɾ����ϵ������
		{
			LinkmanSystem::GetInstance().OnPlayerRequestDelMsg(pMsg);
		}
		break;
	case  MSG_S2W_LINKMAN_MAP_CHANGE:
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			LONG lRegionID = pMsg->GetLong();
			long lRgnType = pMsg->GetLong();
			CGUID rgnGuid;
			pMsg->GetGUID(rgnGuid);

			char szMapName[MAX_PLAYER_TITLE_SIZE] = {0};
			pMsg->GetStr(szMapName, MAX_PLAYER_TITLE_SIZE - 1);

			CPlayer *pPlayer = GetGame()->GetMapPlayer(PlayerGuid);
			if(NULL != pPlayer)
			{
				pPlayer->SetRegionID(lRegionID);
				pPlayer->SetCurRgnType((eRgnType)lRgnType);
				pPlayer->SetRegionExID(rgnGuid);
			}
			LinkmanSystem::GetInstance().OnGsMapChange(PlayerGuid, szMapName);
			GetOrganiCtrl()->OnPlayerMapChange(PlayerGuid, szMapName);
		}
		break;
	case MSG_S2W_LINKMAN_DATA:
		{
			LONG lDataSize = pMsg->GetLong();
			if(0 == lDataSize) return;
			vector<BYTE> vData(lDataSize);
			BYTE *pData = &vData[0];
			pMsg->GetEx(pData, lDataSize);

			LONG lPos = 0;
			LinkmanSystem::GetInstance().OnGsDataSend(pData, lPos);
		}
		break;
	}
}
