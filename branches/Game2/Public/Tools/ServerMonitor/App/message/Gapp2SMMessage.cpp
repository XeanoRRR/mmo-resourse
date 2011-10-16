/****
����GAPP��Ϣ
*/
#include "stdafx.h"
#include "../../net/Message.h"

void OnGapp2SMMessage(CMessage* pMsg)
{
	switch( pMsg->GetType() )
	{
	case MSG_GAPP2SM_TESTCONN:
		{
			long lGappID = pMsg->GetDWord();
			CHECKGAPP(lGappID);
			long lSmTime = pMsg->GetLong();
			long lGappTime = pMsg->GetLong();	
			pGapp->SetDelayTime(lGappTime-lSmTime);
			
		}
		break;
	case MSG_GAPP2SM_GET_GSPLAYERS:
		{
			
		}
		break;
		//�����óɹ�
	case MSG_GAPP2SM_GAPP_INITOK:
		{
			char strVersion[MAX_PATH]={0};
			pMsg->GetEx(strVersion,MAX_PATH);
			DWORD lID = pMsg->GetDWord();
			CHECKGAPP(lID);
			pGapp->SetInfo(strVersion);
			pGapp->CancelLoadResTimer();	
		}
		break;
		
	/*case MSG_GAPP2SM_GAPP_STARTOK:
		{
			long lID = pMsg->GetLong();
			CGapp* pGapp = GetSM()->FindGapp(lID);
			if (pGapp)
			{
				
			}
		}
		break;*/
	case MSG_GAPP2SM_SHUTDOWN:
		{
			long lID = pMsg->GetDWord();
			CHECKGAPP(lID);
			pGapp->SetState(eSS_Stop);
		}
		break;
		//���߳�������
	case MSG_GAPP2SM_SEND_MAINAITICK:
		{
			long lID = pMsg->GetDWord();
			CHECKGAPP(lID);
			BYTE b = pMsg->GetByte();
			if (b)
			{
				pGapp->SetMainTick(pMsg->GetDWord());
			}
			else
			{
				pGapp->CmpAITick(pMsg->GetDWord());
			}	
		}
		break;
	case MSG_GAPP2SM_ACTION:
		{
			pMsg->SetType(MSG_SM2SCC_INFO_Notify_CTRL_GappAction);
			pMsg->SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_SENDINFO:
		{
			pMsg->SetType(MSG_SM2SCC_INFO_Notify_Error);
			pMsg->SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_REPORTINFO:
		{
			tagReportInfo tgInfo;
			CGUID objGuid;
			CGUID PlayerGuid;
			pMsg->GetStr(tgInfo._IndicteeName,MAX_GAPP_NAME_SZIE);
			pMsg->GetStr(tgInfo._ReportName,MAX_GAPP_NAME_SZIE);
			tgInfo._ReportType=pMsg->GetWord();
			tgInfo._ReportTime=pMsg->GetDWord();
			tgInfo._WsSmId=0;
			tgInfo._WsGappId=pMsg->GetDWord();
			//�ٱ���Ϣ
			CMessage msg(MSG_SM2SCC_MANAGE_Notify_Report);
			msg.AddEx(&tgInfo,sizeof(tgInfo));
			msg.SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_WATCH_INFO_Reply:
		{
			//������ӵĻظ�
			pMsg->SetType(MSG_SM2SCC_MANAGE_WATCH_Reply);
			pMsg->SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_WATCH_INFO:
		{
			//���ӵ������Ϊ
			pMsg->SetType(MSG_SM2SCC_MANAGE_Notify_Gm_Command_Info);
			pMsg->SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_REPORT_Result_Reply:
		{
			//�Ա��ٱ��˵Ĵ�����
			pMsg->SetType(MSG_SM2SCC_MANAGE_Report_Result_Replay);
			pMsg->SendToAllSCC();
		}
		break;
	case MSG_GAPP2SM_MANAGE_Request_Gm_Command:
		{
			//��������ҵĲ����ظ�,�����
			pMsg->SetType(MSG_SM2SCC_MANAGE_Notify_Gm_Command_Info);
			pMsg->SendToAllSCC();
		}
		break;
	default:
		break;
	}
}