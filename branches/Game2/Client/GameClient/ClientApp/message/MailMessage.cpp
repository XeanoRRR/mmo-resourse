/**************************************************
*		�ļ���  ��Mail.h
*		��Ҫ����������GS���ʼ���Ϣ

*		��ǰ�汾��1.0
*		����    ��ʯϲ
*		������ڣ�10/10/18
*		�޶�˵����
****************************************************/

#include "stdafx.h"

#include "ClientRegion.h"
#include "Message.h"
#include "Player.h"
#include "Other/Audiolist.h"
#include "../GameClient/Game.h"
#include "../Mail/Mail.h"
using namespace mail;
#include "../Mail/MailDef.h"
#include "../../../Public/Common/MailDef.h"
#include "../../../Public/Common/Public.h"
#include "../../../UI/UIDef.h"
#include "../MsgBoxSys/MsgEventManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DoS2CMailReceive( CMessage *pMsg );
void DoS2CMailSendFailed( CMessage *pMsg );
void DoS2CMailSend( CMessage *pMsg );
void DoS2CMailRead( CMessage *pMsg );
void DoS2CMailGetGoldReslt( CMessage *pMsg );
void DoS2CMailMoveGoodsResult( CMessage *pMsg );// ȡ������Ǯ
void DoS2CMailDeleteOk( CMessage *pMsg );// ɾ���ʼ�
void DoS2CMailAgreedPay( CMessage *pMsg );// �������ŷ���
void DoS2CMailSysMailMsg( CMessage *pMsg );//ϵͳ�ʼ�������Ϣ

void OnMailMessage( CMessage *pMsg )
{	
	CPlayer* pPlayer = pMsg -> GetPlayer();
	if (!pPlayer)
	{
		return;
	}
	switch( pMsg -> GetType() )
	{
	case MSG_S2C_MAIL_RECEIVER:					// ���ռ���(���ص������ʼ���Ϣ)
		{
			DoS2CMailReceive( pMsg );
		}
		break;
	case MSG_S2C_MAIL_SEND_FAILED:				// �ʼ�����ʧ��
		{
			DoS2CMailSendFailed( pMsg );
		}
		break;
	case MSG_S2C_MAIL_SEND:						// �����ʼ�����
		{
			DoS2CMailSend( pMsg );			
		}
		break;
	case MSG_S2C_MAIL_READ:						// �Ķ��ʼ�
		{
			DoS2CMailRead( pMsg );
		}
		break;
	case MSG_S2C_MAIL_GET_GOLD_RESULT:			// ȡ������Ǯ
		{

			DoS2CMailGetGoldReslt( pMsg );
		}
		break;
	case MSG_S2C_MAIL_MOVE_GOODS_RESULT:		// ȡ������Ʒ
		{
			DoS2CMailMoveGoodsResult( pMsg );
		}
		break;
	case MSG_S2C_MAIL_DELETE_OK:				// ɾ���ʼ�
		{
			DoS2CMailDeleteOk( pMsg );
		}
		break;
	case MSG_S2C_MAIL_AGREED_PAY:				// �������ŷ���
		{
			DoS2CMailAgreedPay( pMsg );
		}
		break;
	case MSG_S2C_MAIL_SYSMAILMSG:				//ϵͳ�ʼ�������Ϣ
		{
			DoS2CMailSysMailMsg( pMsg );
		}
		break;
	}
}

void DoS2CMailReceive( CMessage *pMsg )
{
	long dwSize=pMsg->GetLong();
	if( dwSize==0 )
		return;
	BYTE *pBuff=new BYTE[dwSize];
	pMsg->GetEx(pBuff,dwSize);
	long lPos = 0;
	
	GetInst(CMail).ClearAllMailData();//ɾ�������ʼ���Ϣ
	
	// �ʼ�����
	long lNum = _GetLongFromByteArray(pBuff, lPos);
	for (int i=0; i<lNum; ++i)
	{
		stMailData stData;
		GetInst(CMail).DecordDataFromByteArray(stData,pBuff,lPos);
		GetInst(CMail).AddMail(stData);
	}
	FireUIEvent(MAILLIST,EVENT_UPDATA);
	
	SAFEDELETEARRAY(pBuff);

	// �򿪵�����£�ֱ��ˢ���ռ����б�ҳ��
	//if(pPage->IsOpen())
	//	pPage->UpdataInBoxMail(pPage->GetMailListCurPage());
	//else
	//	pPage->Open();
	// ͼ��ֹͣ����
	//CComponent *pComp = GetGame()->GetCGuiEx()->GetMainBarPageEx()->GetMiniMapComp();
	//if(pComp)
	//{
	//	pComp = pComp->GetComponent("Mail");
	//	if(pComp)
	//	{
	//		CImageIcon *pIcon = (CImageIcon *)pComp->GetSpecialIcon();
	//		if(pIcon)
	//			pIcon->SetIsChangeAlpha(false);
	//	}
	//}
}
void DoS2CMailSendFailed( CMessage *pMsg )
{
	long index = pMsg->GetLong();
	string str = "";
	switch(index)
	{
		//���ʲ���
	case MAIL_SENDING_FAILED_INVALID_POSTAGE:
		str = AppFrame::GetText("Mail_51");
		break;	
		//��ҳ�������
	case MAIL_SENDING_FAILED_INVALID_GOLD:
		str = AppFrame::GetText("Mail_55");
		break;
		//��Ʒ��������
	case MAIL_SENDING_FAILED_INVALID_GOODS:
		str = AppFrame::GetText("Mail_56");
		break;
		//���ֹ���������ʧ�ܣ�
	case MAIL_SENDING_FAILED_INVALID_MAXNAME:
		str = AppFrame::GetText("Mail_57");
		break;
		//�������
	case MAIL_SENDING_FAILED_INVALID_MAXSUBJECT:
		str = AppFrame::GetText("Mail_58");
		break;
		//�������ݹ���
	case MAIL_SENDING_FAILED_INVALID_MAXTEXT:
		str = AppFrame::GetText("Mail_59");
		break;
	//	//�Ƿ�������
	case MAIL_SENDING_FAILED_INVALID_RECEIVER:
		str = AppFrame::GetText("Mail_60");
		break;
		//�������˻����
	//case MAIL_SENDING_FALLED_INVALID_SUBJECTORRECEIVER:
	//	str = AppFrame::GetText("Mail_61");
	//	break;
		//���Լ����ʼ�
	/*case MAIL_SENDING_FAILED_INVALID_SELF:
		str = AppFrame::GetText("Mail_27");
		break;*/
	//case MAIL_SENDING_FAILED_INVALID_MANYRECEIVER:
	//	str = AppFrame::GetText("Mail_24");
	//	break;
	//��������Ƿ��ַ�
	//case MAIL_SENDING_FAILED_INVALID_SUBJECT:
	//	str = AppFrame::GetText("Mail_52");
	//	break;
	//	//���ݰ����Ƿ��ַ�
	//case MAIL_SENDING_FAILED_INVALID_TEXT:
	//	str = AppFrame::GetText("Mail_53");
	//	break;
	//�������ݰ����Ƿ��ַ�
	//case MAIL_SENDING_FAILED_INVALID_EXTEXT:
	//	str = AppFrame::GetText("Mail_54");
	//	break;
	}
	if(str!="")
	{
		 GetInst(MsgEventManager).PushEvent(Msg_Ok,str.c_str());
	}
}
void DoS2CMailSend( CMessage *pMsg )
{
	//[COLOR 4D1F00][B]�����µ��ʼ�����ע����գ�
	GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_30"));
}
void DoS2CMailRead( CMessage *pMsg )
{
	BYTE bFlag = pMsg->GetByte();
	if(bFlag > 0)
	{
		CGUID guid;
		pMsg->GetGUID(guid);
		stMailData *stData = GetInst(CMail).GetMailByGUID(guid);
		if(stData)
		{
			stData->bIsReaded = true;
			GetInst(CMail).SetCurReadMailGUID(guid);
			FireUIEvent(READMAIL,EVENT_UPDATA);
		}
	}
	else
	{
		//����Ʒ��֮ǰ�ĸ�����Ʒ���Ͳ�������Ʒ���ʧ�ܣ�
		//sGetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_3"));//del
	}
}

void DoS2CMailGetGoldReslt( CMessage *pMsg )
{
	//CPlayer *pPlayer = GetGame()->GetMainPlayer();//del
	//char str2[256]="";//del
	//sprintf_s(str2,"��ȡ��Һ�Ľ�ǮΪ%d",pPlayer->GetMoney());//del
	//GetInst(MsgEventManager).PushEvent(Msg_Ok,str2);//del

	CGUID guid;
	pMsg->GetGUID(guid);
	BYTE bFlag = pMsg->GetByte();
	if(bFlag > 0)
	{
		stMailData *stData = GetInst(CMail).GetMailByGUID(guid);
		if(stData)
		{
			stData->dwGoldNum = 0;
			stData->bIsCanReBack = false;
			if(stData->GoodsList.empty() && stData->dwGoldNum == 0 && stData->eMailType != SYSTEM_MAIL)
			{
				stData->eMailType = COMMON_MAIL;
			}
			GetInst(CMail).UpdataMail(*stData);
			if (guid == GetInst(CMail).GetCurReadMailGUID())
			{
				FireUIEvent(GOLD,EVENT_UPDATA);//֪ͨ������½�Ǯ
			}		
		}
		//��ȡ��ҳɹ���
		 GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_39"));
	}
}
void DoS2CMailMoveGoodsResult( CMessage *pMsg )
{
	CGUID guid;
	pMsg->GetGUID(guid);
	long index = pMsg->GetLong();
	//GetInst(CMail).SetGoodsIndex( index );
	BYTE bFlag = pMsg->GetByte();
	if(bFlag > 0)// �ɹ�
	{
		stMailData *stData = GetInst(CMail).GetMailByGUID(guid);
		if(stData)
		{
			list<CGoods *>::iterator it = stData->GoodsList.begin();
			for (int i=0; it != stData->GoodsList.end(); ++it, ++i)
			{
				if( i == index )
				{
					SAFE_DELETE((*it));
					stData->GoodsList.erase(it);
					break;
				}
			}
			stData->bIsCanReBack = false;
			if (guid == GetInst(CMail).GetCurReadMailGUID())
			{
				FireUIEvent( GETGOODSCONTAINER,EVENT_DELGOODS );
			}
			// ��Ʒ�ͽ�Ҷ�û�У��Ҳ���ϵͳ�ʼ���ת������ͨ���͵��ʼ�
			if(stData->GoodsList.empty() && stData->dwGoldNum == 0 &&stData->eMailType != SYSTEM_MAIL)
			{
				stData->eMailType = COMMON_MAIL;
			}
			GetInst(CMail).UpdataMail(*stData);
		}
	}
	else
	{	
		//"ȡ������Ʒʧ�ܣ����鱳���Ƿ��п�λ��"
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_28"));
	}
}
void DoS2CMailDeleteOk( CMessage *pMsg )// ɾ���ʼ�
{
	long size = pMsg->GetLong();
	if(size > 0)
	{
		for (int i=0; i<size; ++i)
		{
			CGUID guid;
			pMsg->GetGUID(guid);
			GetInst(CMail).DelMailByGuid(guid);
		}
		//�ʼ�ɾ���ɹ���
		GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_35")); 
		FireUIEvent(MAILLIST,EVENT_UPDATA);
	}
}
void DoS2CMailAgreedPay( CMessage *pMsg )// �������ŷ��أ���ͨ��
{
	CGUID guid;
	pMsg->GetGUID(guid);
	eMAILTYPE eType = (eMAILTYPE)pMsg->GetByte();
	stMailData *stData = GetInst(CMail).GetMailByGUID(guid);
	if(stData)
	{
		stData->eMailType = eType;//�����ʼ�����
		stData->dwGoldNum = 0;
		GetInst(CMail).UpdataMail(*stData);
	}

	FireUIEvent(READ,EVENT_UPDATA);
	//���ѳɹ�
	GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_40"));//del
	
	//CPlayer *pPlayer = GetGame()->GetMainPlayer();//del
	//char str2[256]="";//del
	//sprintf_s(str2,"���Ѻ�Ľ�ǮΪ%d",pPlayer->GetMoney());//del
	//GetInst(MsgEventManager).PushEvent(Msg_Ok,str2);//del

}

void DoS2CMailSysMailMsg( CMessage *pMsg )//ϵͳ�ʼ�������Ϣ
{
	GetInst(MsgEventManager).PushEvent(Msg_Ok,AppFrame::GetText("Mail_30"));
}