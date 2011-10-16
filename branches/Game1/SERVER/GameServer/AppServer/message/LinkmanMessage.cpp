

#include "StdAfx.h"
#include "../Linkman/LinkmanSystem.h"


//! ��Ӧ�������������ϵ����Ϣ
extern void OnLinkmanMessage_ForWs(CMessage *pMsg)
{
	switch(pMsg->GetType())
	{	
	case MSG_W2S_LINKMAN_PLAYER_INITDATA://! ��ʼ�������ϵ�� 
		{
			LinkmanSystem::GetInstance().OnInitMessage(pMsg);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_NOTIFY: //! WSת����������������ϵ����Ϣ
		{
			//! ת����Ϣ
			pMsg->GetLong();
			char szSenderName[MAX_PLAYER_NAME_SIZE + 1] = {0};
			pMsg->GetStr(szSenderName, MAX_PLAYER_NAME_SIZE);
			CGUID SenderGuid;
			pMsg->GetGUID(SenderGuid);
			CGUID ReceiverGuid;
			pMsg->GetGUID(ReceiverGuid);
			
			pMsg->SetType(MSG_S2C_LINKMAN_REQUEST_ADD);
			pMsg->SendToPlayer(ReceiverGuid);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_REQUEST_ADD_RE: //! WS����������ϵ������Ļظ�
		{
			//! ת����Ϣ
			pMsg->GetLong();
			pMsg->GetLong();
			char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
			pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);
			CGUID ReceiverGuid;
			pMsg->GetGUID(ReceiverGuid);

			pMsg->SetType(MSG_S2C_LINKMAN_ADD);
			pMsg->SendToPlayer(ReceiverGuid);
		}
		break;
	case MSG_W2S_LINKMAN_ADD_NOTIFY: //! �����ϵ��֪ͨ
		{
			CGUID OwnerGuid;
			pMsg->GetGUID(OwnerGuid);
			char szOwnerName[MAX_PLAYER_NAME_SIZE + 1] = {0};
			pMsg->GetStr(szOwnerName, MAX_PLAYER_NAME_SIZE - 1);
			LONG lGroupID = pMsg->GetLong();
			tagLinkman_data Linkman_data;
			pMsg->GetEx(&Linkman_data, sizeof(tagLinkman_data));

			LinkmanSystem::GetInstance().OnAddLinkMan(OwnerGuid, szOwnerName, lGroupID, Linkman_data);
		}
		break;
	case MSG_W2S_LINKMAN_MOVE_NOTIFY: //! �ƶ���ϵ��֪ͨ
		{
			BOOL bOptResult = pMsg->GetLong();
			if(bOptResult)
			{
				CGUID OwnerGuid;
				pMsg->GetGUID(OwnerGuid);
				char szOwnerName[MAX_PLAYER_NAME_SIZE + 1] = {0};
				pMsg->GetStr(szOwnerName, MAX_PLAYER_NAME_SIZE - 1);
				LONG lSrcGroupID = pMsg->GetLong();
				LONG lDesGroupID = pMsg->GetLong();
				tagLinkman_data Linkman_data;
				pMsg->GetEx(&Linkman_data, sizeof(tagLinkman_data));

				LinkmanSystem::GetInstance().OnMoveLinkMan(OwnerGuid, szOwnerName, lSrcGroupID, lDesGroupID, Linkman_data);
			}
			else
			{
				pMsg->GetLong();
				char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
				pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);
				CGUID ReceiverGuid;
				pMsg->GetGUID(ReceiverGuid);

				pMsg->SetType(MSG_S2C_LINKMAN_MOVE);
				pMsg->SendToPlayer(ReceiverGuid);
			}
		}
		break;
	case MSG_W2S_LINKMAN_DEL_NOTIFY: //! ɾ����ϵ��֪ͨ
		{
			CGUID OwnerGuid, AimGuid;
			pMsg->GetGUID(OwnerGuid);
			char szOwnerName[MAX_PLAYER_NAME_SIZE + 1] = {0};
			pMsg->GetStr(szOwnerName, MAX_PLAYER_NAME_SIZE);
			LONG lGroupID = pMsg->GetLong();
			pMsg->GetGUID(AimGuid);
			char szAimName[MAX_PLAYER_NAME_SIZE + 1] = {0};
			pMsg->GetStr(szAimName, MAX_PLAYER_NAME_SIZE);

			LinkmanSystem::GetInstance().OnDelLinkMan(OwnerGuid, szOwnerName, lGroupID, AimGuid, szAimName);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_RELEASEDATA: //! ��������ϵ������֪ͨ
		{
			LinkmanSystem::GetInstance().OnReleaseMessage(pMsg);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_LOGIN:	//! ��Ӧ��ϵ�˵�½��Ϣ
		{
			LinkmanSystem::GetInstance().OnLinkmanLoginMessage(pMsg);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_LOGOUT:	//! ��Ӧ��ϵ��������Ϣ
		{
			LinkmanSystem::GetInstance().OnLinkmanLogoutMessage(pMsg);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_INFO_CHANGE: //! ��Ӧ�����Ϣ�ı�
		{
			LinkmanSystem::GetInstance().OnPlayerInfoChange(pMsg);
		}
		break;
	case MSG_W2S_LINKMAN_PLAYER_DELSTATE_CHANGE: //! ��Ӧ�����Ϣ�ı�
		{
			LinkmanSystem::GetInstance().OnPlayerDelStateChange(pMsg);
		}
		break;
	case MSG_W2S_LEAVEWORD_SAVE_RESULT: //! ת�����Ա�����
		{
			LONG lResult = pMsg->GetLong();
			char szName[MAX_PLAYER_NAME_SIZE] = {0};
			pMsg->GetStr(szName, MAX_PLAYER_NAME_SIZE);
			CGUID guid;
			pMsg->GetGUID(guid);
	
			CMessage msg(MSG_S2C_LEAVEWORD_SAVE_RESULT);
			msg.Add(lResult);
			msg.Add(szName);
			msg.SendToPlayer(guid);
		}
		break;
	case MSG_W2S_LEAVEWORD_POP: //! ת��������Ի��
		{
			CGUID PlayerGuid;
			pMsg->GetGUID(PlayerGuid);
			
			pMsg->SetType(MSG_S2C_LINKMAN_LEAVE_WORD);
			pMsg->SendToPlayer(PlayerGuid);
		}
		break;
	}
}


//! ��Ӧ�ͻ��˵���ϵ����Ϣ
extern void OnLinkmanMessage_ForClient(CMessage *pMsg)
{
	switch(pMsg->GetType())
	{

	case MSG_C2S_LINKMAN_ADD: //! A �� S ���������ϵ�� B
		{
			LinkmanSystem::GetInstance().OnRequestAdd(pMsg);
		}
		break;
	case MSG_C2S_LINKMAN_DEL: //! A �� S ����ɾ����ϵ�� B
		{
			LinkmanSystem::GetInstance().OnRequestDel(pMsg);
		}
		break;
	case MSG_C2S_LINKMAN_MOVE: //! A �� S �����ƶ���ϵ�� B
		{
			LinkmanSystem::GetInstance().OnRequestMove(pMsg);
		}
		break;
	case MSG_C2S_LINKMAN_RESPONSE_ADD: //! B �� S ��Ӧ A ���������
		{
			LinkmanSystem::GetInstance().OnAnswerAdd(pMsg);
		}
		break;
	}
}