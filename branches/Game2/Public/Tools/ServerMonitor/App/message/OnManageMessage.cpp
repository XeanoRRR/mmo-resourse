/*
*����GM���ߵĹ�����Ϣ
*
*/
#include "stdafx.h"
#include "../../net/Message.h"
void OnManageMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{//����������
	case MSG_SCC2SM_MANAGE_WATCH:
		{
			long lGappID=pMsg->GetLong();
			DWORD bDel=pMsg->GetDWord();
			char szName[32]={0};
			pMsg->GetStr(szName,32);
			CMessage msg(MSG_SM2GAPP_WATCH);
			msg.Add(bDel);
			msg.Add(szName);
			msg.SendToGapp(lGappID);
		}
		break;
		//�����ٱ���
	case MSG_SCC2SM_MANAGE_Report_Result:
		{
			long lGappID=pMsg->GetLong();
			pMsg->SetType(MSG_SM2GAPP_REPORT_Result);
			pMsg->SendToGapp(lGappID);
		}
		break;
		//������
	case MSG_SCC2SM_MANAGE_Notify_Affiche:
		{
			long lGappID=pMsg->GetLong();
			pMsg->SetType(MSG_SM2GAPP_WS_SENDTOPNEWS);
			pMsg->SendToGapp(lGappID);
		}
		break;
	case MSG_SCC2SM_MANAGE_Request_Gm_Command:
		{
			long lGappID=pMsg->GetLong();
			pMsg->SetType(MSG_SM2GAPP_MANAGE_Request_Gm_Command);
			pMsg->SendToGapp(lGappID);
		}
		break;
	}
}