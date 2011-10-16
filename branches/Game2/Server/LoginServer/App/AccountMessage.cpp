#include "stdafx.h"
#include "LoginManager.h"

#include "../Net/Message.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ӦAccountServer��Ϣ
void OnASMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_A2O_LOGIN_Affirm_Re:
		{
			DWORD	dwMyType = pMsg->GetDWord();

			Log4c::Trace(ROOT_MODULE,GetText("LOGIN_ACCMSG_0"));//��½AccountServer�ɹ�!
			GetGame()->SendOnlineInfoToAs();
		}
		break;
	case MSG_A2O_ACCOUNT_Validate_Re:
		{
			tagAccountInfo_FromAS AccountInfo_FromAS;
			pMsg->GetEx(&AccountInfo_FromAS, sizeof(tagAccountInfo_FromAS));

#ifdef SHOW_LOGIN_MSG_20090817
			if(GetGame()->OutInfo())
			{
				Log4c::Trace(ROOT_MODULE,FormatText("LOGIN_ACCMSG_1", AccountInfo_FromAS._szCdkey)��;//MSG_A2O_ACCOUNT_Validate_Re %s
			}
#endif

			if ((eVT_Not_Pwd | eVT_Not_Fcm) == AccountInfo_FromAS._cValidateType)
				GetGame()->GetLoginManager().OnValidateReturn(AccountInfo_FromAS, 2);
			else
				GetGame()->GetLoginManager().OnValidateReturn(AccountInfo_FromAS, 0);	


		}
		break;

	case MSG_A2O_ACCOUNT_FcmTime_Notify:
		{
			char szCdkey[CDKEY_SIZE] = {0};
			pMsg->GetStr(szCdkey, CDKEY_SIZE);

			DWORD dwHoldSecond = pMsg->GetDWord();

			if(-1 != CMessage::GetSocketByCdkey(szCdkey) && 0 != dwHoldSecond)
			{
				GetGame()->GetLoginManager().AddFcmCdkey(szCdkey, dwHoldSecond);

				CMessage msg(MSG_L2C_LOG_FCM_TIME);
				msg.Add(dwHoldSecond);
				msg.SendToClient(szCdkey);

				return;
			}

			LONG lWSID = GetGame()->FindCdkey(szCdkey);
			if(0 != lWSID)
			{
				if(0 == dwHoldSecond)
				{
					CMessage msgToWorldServer( MSG_L2W_LOG_FCM_BEGIN );
					msgToWorldServer.Add(szCdkey);
					GetGame()->SendMsg2World( &msgToWorldServer, lWSID);
				}
				else
				{
					GetGame()->GetLoginManager().AddFcmCdkey(szCdkey, dwHoldSecond);

					CMessage msg(MSG_L2W_LOG_FCM_TIME);
					msg.Add(szCdkey);
					msg.Add(dwHoldSecond);
					GetGame()->SendMsg2World(&msg, lWSID);
				}
			}	
		}
	case MSG_A20_SECURITY_DATA:
		{
			tagAccountInfo_FromAS AccountInfo_FromAS;
			pMsg->GetEx(&AccountInfo_FromAS, sizeof(tagAccountInfo_FromAS));
			tagSecurityData SecurityData;
			pMsg->GetEx(&SecurityData, sizeof(tagSecurityData));
			GetGame()->GetLoginManager().OnSecurityData(AccountInfo_FromAS, SecurityData);
		}
	}
}


void OnAinMessage(AinMessage* pAimMsg)
{
	switch(pAimMsg->GetType())
	{
	case AIN_MSG_ACCOUNT_Validate_Re:
		{
			tagAccountInfo_FromAS AccountInfo_FromAS;
			tagSecurityData SecurityData;
			
			pAimMsg->GetEx(&AccountInfo_FromAS, sizeof(tagAccountInfo_FromAS));
			pAimMsg->GetEx(&SecurityData, sizeof(tagSecurityData));

			if(2 == SecurityData._lWorkFlag)
			{
				if(ePLR_Lost != AccountInfo_FromAS._LoginRerult._dwLoginResult)
					GetGame()->GetLoginManager().OnSecurityData(AccountInfo_FromAS, SecurityData, TRUE);
				else
					GetGame()->GetLoginManager().OnValidateReturnLost(AccountInfo_FromAS);
			}
			else
			GetGame()->GetLoginManager().OnValidateReturn(AccountInfo_FromAS, 1);	
		}
		break;

	case AIN_MSG_SDOA_VALIDARE_RETURN:
		{
			tagSdoaValidateResult SdoaValidateResult;
			pAimMsg->GetEx(&SdoaValidateResult, sizeof(tagSdoaValidateResult));
			GetGame()->GetCSdoaValidate().OnValidateResult(SdoaValidateResult);
		}
		break;

	case AIN_MSG_SDOA_FCM_NOTIFY:
		{
			int nHoldSecond = pAimMsg->GetDword();
			char szSndaID[CDKEY_SIZE] = {0};
			pAimMsg->GetStr(szSndaID, CDKEY_SIZE);

#ifdef OUT_VALIDATE_FCM_INFO_20090720
			PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "�����Խ������ʼ���˺š�%s����ʣ������ʱ�䡾%d��", szSndaID, nHoldSecond);
#endif
			if(0 < nHoldSecond)//! ��������ʱ��
			{
#ifdef OUT_VALIDATE_FCM_INFO_20090720
				PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "���͵�LS�ϵĿͻ���");
#endif
				if(-1 != CMessage::GetSocketByCdkey(szSndaID))
				{
					CMessage msg(MSG_L2C_LOG_FCM_TIME);
					msg.Add((LONG)nHoldSecond);
					msg.SendToClient(szSndaID);
				}

#ifdef OUT_VALIDATE_FCM_INFO_20090720
				PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "֪ͨWSת��");
#endif
				//! �ѵ�½��Ϸ��֪ͨ��Ϸ������
				LONG lWSID = GetGame()->FindCdkey(szSndaID);
				if(0 != lWSID)
				{
					CMessage msg(MSG_L2W_LOG_FCM_TIME);
					msg.Add(szSndaID);
					msg.Add((LONG)nHoldSecond);
					GetGame()->SendMsg2World(&msg, lWSID);
				}
				//! �Ѿ�������Ϸ��
				else
				{
					//! ��ֹ���������Ͽ�LS����GS����WS��δ���Ϳͻ����ѵ�½���м�״̬
					CMessage msgToWorldServer( MSG_L2W_LOG_FCM_BEGIN );
					msgToWorldServer.Add( szSndaID );
					msgToWorldServer.SendAllWorld();

					GetGame()->GetCSdoaFcm().UserOffline(szSndaID);
					GetGame()->GetLoginManager().OnClientNetClose(szSndaID, FALSE);
				}
			}
			else//! û��ʱ�䣬������
			{
				if(-1 != CMessage::GetSocketByCdkey(szSndaID))
				{
#ifdef OUT_VALIDATE_FCM_INFO_20090720
					PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "�ߵ�LS�ϵĿͻ���");
#endif
					CMessage msg(MSG_L2C_LOG_ANSWERCDKEY);
					msg.Add((LONG)LOGIN_NOTENOUGH_HEALTHY_TIME);
					msg.SendToClient(szSndaID);

					CMessage::UnCDkeyBySocket(szSndaID);
				}

				//! ��ֹ���������Ͽ�LS����GS����WS��δ���Ϳͻ����ѵ�½���м�״̬
				//! ֪ͨWS���������

#ifdef OUT_VALIDATE_FCM_INFO_20090720
				PutoutLog("SDOA_FCM_INFO", LT_DEFAULT, "֪ͨWS����");
#endif
				CMessage msgToWorldServer( MSG_L2W_LOG_FCM_BEGIN );
				msgToWorldServer.Add( szSndaID );
				msgToWorldServer.SendAllWorld();

				GetGame()->GetCSdoaFcm().UserOffline(szSndaID);
				GetGame()->GetLoginManager().OnClientNetClose(szSndaID, FALSE);
			}
		}
		break;
	}
}