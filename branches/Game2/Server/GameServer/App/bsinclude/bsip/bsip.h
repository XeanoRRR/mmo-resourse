 /*
* Copyright (c) 2005,�Ϻ�ʢ������Ʒ�ƽ̨��
* All rights reserved.
* 
* �ļ����ƣ�bsip.h
* ժ    Ҫ��ͳһ�Ʒ�ϵͳ��Ϣͷ����
*           client��LBS��CBS��ͨѶ�õ���Ϣ�嶨��
* ��ǰ�汾��1.4.2
* ��    �ߣ�����
* ������ڣ�2005��9��6��
*
* �޸����ݣ���1.3�İ汾�Ͻ����޸ģ�ɾ�����õ���Ϣ
*           ��Ӽ��۽���ʹ�õ���Ϣ
* �޸�ʱ�䣺2005��9��6��
*
* �޸����ݣ�����汾��Version
* �޸�ʱ�䣺2005��10��8��
*
* �޸����ݣ����Notify��Ϣ���ܽӿں���غ궨��
* �޸�ʱ�䣺2005��10��13��
*
* �޸����ݣ��޸İ�ͷ���壬���ip��port��send_time���ֶ�
* �޸�ʱ�䣺2005��10��13��
*
* �޸����ݣ��޸İ汾��
* �޸�ʱ�䣺2005��10��19��
*
* �޸����ݣ��޸İ汾��
* �޸�ʱ�䣺2005��11��25��
*
* �޸����ݣ��޸İ汾������NORMAL_STAT��Ϣ
* �޸�ʱ�䣺2005��12��27��
*
* �޸����ݣ��޸��� AUTHEN��Ϣ, GOLDCONSUME ��Ϣ, DEPOSIT ��Ϣ
* �޸�ʱ�䣺2006��2��21��
*
* �޸����ݣ������� account_lock_ex,account_unlock_ex ��Ϣ
* �޸�ʱ�䣺2006��2��24��
*
* �޸����ݣ������� award_ack_res ��Ϣ
* �޸�ʱ�䣺2006��2��27��
*
* �޸����ݣ������� item_sync ��Ϣ �� gs_route ��Ϣ
* �޸�ʱ�䣺2006��8��16��
*
* �޸����ݣ������� ͳһ��սƽ̨�õ���Ϣ
* �޸�ʱ�䣺2006��11��15��
*
* �޸����ݣ�����ConsignTransfer��Ϣ
* �޸�ʱ�䣺2007-5-3 9:25
*
* �޸����ݣ�����EkeyBind��Ϣ
* �޸�ʱ�䣺2007-6-19 15:54
*
* �޸����ݣ��޸�BsipNormalStatReq��Ϣ��ȫ1.6�е�13����Ϣ���޸�BsipStat������Ӧ�ӳ�ͳ��
* �޸�ʱ�䣺2007-8-1
*
*/

/*
*ͨѶ�õ����ݰ���������Ϣͷ����Ϣ��
*/
#ifndef _BSIP_H_
#define _BSIP_H_

#include "bsipmsgbs.h"

//define the message type
#define BSIP_AUTHOR_REQ					0x00000001	//��֤������Ϣ
#define BSIP_AUTHOR_RES					0x80000001	//��֤��Ӧ��Ϣ
#define BSIP_ACCOUNT_REQ				0x00000002  //�۷�������Ϣ
#define BSIP_ACCOUNT_RES				0x80000002	//�۷���Ӧ��Ϣ
#define BSIP_AUTHEN_ACCOUNT_REQ			0x00000003	//��֤������Ϣ
#define BSIP_AUTHEN_ACCOUNT_RES			0x80000003	//��֤��Ӧ��Ϣ
#define BSIP_LOCK_ACCOUNT_REQ			0x00000004	//�û��˺��ʽ�Ԥ����������Ϣ
#define BSIP_LOCK_ACCOUNT_RES			0x80000004	//�û��˺��ʽ�Ԥ������Ӧ��Ϣ
#define BSIP_UNLOCK_ACCOUNT_REQ			0x00000005	//�û��˺��ʽ�ۼ�������Ϣ
#define BSIP_UNLOCK_ACCOUNT_RES			0x80000005	//�û��˺��ʽ�ۼ���Ӧ��Ϣ
#define BSIP_AWARD_AUTHEN_REQ			0x00000006	//������֤����
#define BSIP_AWARD_AUTHEN_RES			0x80000006	//������Ӧ����
#define BSIP_AWARD_ACK					0x00000007	//����ȷ����Ϣ����
#define BSIP_AWARD_ACK_RES				0x80000007	//����ȷ����Ϣ����
#define BSIP_LOCK_CONSIGN_REQ			0x00000008	//�����û��˺��ʽ�Ԥ����������Ϣ
#define BSIP_LOCK_CONSIGN_RES			0x80000008	//�����û��˺��ʽ�Ԥ������Ӧ��Ϣ
#define BSIP_UNLOCK_CONSIGN_REQ			0x00000009	//�����û��˺��ʽ�ۼ�������Ϣ
#define BSIP_UNLOCK_CONSIGN_RES			0x80000009	//�����û��˺��ʽ�ۼ���Ӧ��Ϣ
#define BSIP_CONSIGN_DEPOSIT_REQ		0x00000010	//���۳�ֵ������Ϣ
#define BSIP_CONSIGN_DEPOSIT_RES		0x80000010	//���۳�ֵ��Ӧ��Ϣ
#define BSIP_GOLDDEPOSIT_REQ			0x00000011	//Ԫ����ֵ������Ϣ
#define BSIP_GOLDDEPOSIT_RES			0x80000011	//Ԫ����ֵ��Ӧ��Ϣ

#define BSIP_GOLDCONSUME_LOCK_REQ		0x00000012	//Ԫ������Ԥ��������Ϣ
#define BSIP_GOLDCONSUME_LOCK_RES		0x80000012	//Ԫ������Ԥ����Ӧ��Ϣ
#define BSIP_GOLDCONSUME_UNLOCK_REQ		0x00000020	//Ԫ�����Ľⶳ������Ϣ
#define BSIP_GOLDCONSUME_UNLOCK_RES		0x80000020	//Ԫ�����Ľⶳ��Ӧ��Ϣ

#define BSIP_ACCOUNT_IMPORT_REQ			0x00000021	//���û���ֵ��Ϣ
#define BSIP_ACCOUNT_IMPORT_RES			0x80000021	//���û���ֵ��Ӧ��Ϣ

#define BSIP_NOTIFY_REQ					0x00000015	//����������Ϣ
#define BSIP_NOTIFY_RES					0x80000015	//������Ӧ��Ϣ
#define BSIP_NORMAL_STAT_REQ			0x00000016	//GS��Ϣͳ����Ϣ
#define BSIP_LOCK_ACCOUNT_EX_REQ		0x00000024	//�û��˺��ʽ�Ԥ����������Ϣ
#define BSIP_LOCK_ACCOUNT_EX_RES		0x80000024	//�û��˺��ʽ�Ԥ������Ӧ��Ϣ
#define BSIP_UNLOCK_ACCOUNT_EX_REQ		0x00000025	//�û��˺��ʽ�ۼ�������Ϣ
#define BSIP_UNLOCK_ACCOUNT_EX_RES		0x80000025	//�û��˺��ʽ�ۼ���Ӧ��Ϣ
#define BSIP_ITEMSYNC_REQ       		0x00000026	//��Ʒͬ��������Ϣ��cbs->client
#define BSIP_ITEMSYNC_RES       		0x80000026	//��Ʒͬ����Ӧ��Ϣ��client->cbs
#define BSIP_GSROUTE_REQ        		0x00000027	//GS·����Ϣ������Ϣ��lbs->cbs
#define BSIP_GSROUTE_RES        		0x80000027	//GS·����Ϣ��Ӧ��Ϣ��cbs->lbs
#define BSIP_TRANSFER_REQ				0x00000028	//ת��������Ϣ
#define BSIP_TRANSFER_RES				0x80000028	//ת����Ӧ��Ϣ

#define BSIP_TOKENDEPOSIT_REQ			0x00000029	//����ȯ��ֵ������Ϣ
#define BSIP_TOKENDEPOSIT_RES			0x80000029	//����ȯ��ֵ��Ӧ��Ϣ

// 2007-3-3 14:40 zlc add
#define BSIP_CONSIGN_TRANSFER_REQ		0x00000030	//����ת��������Ϣ
#define BSIP_CONSIGN_TRANSFER_RES		0x80000030	//����ת����Ӧ��Ϣ

// 2007-6-19 15:53 zlc add
#define BSIP_EKEY_BIND_REQ              0x00000031	//����ת��������Ϣ
#define BSIP_EKEY_BIND_RES              0x80000031	//����ת����Ӧ��Ϣ

/*-------------------------------ʱ���Ʒ�-------------------------------*/
/*---------------------------------��Ȩ---------------------------------*/
typedef struct BsipAuthorInfoReq
{
	BsipHeadDef			msg_head;				// ��Ϣ���ͣ���BSIP_AUTHOR_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAuthorReqDef	msg_body;
}BsipAuthorInfoReqDef;

typedef struct BsipAuthorInfoRes
{
	BsipHeadDef			msg_head;				// ��Ϣ���ͣ���BSIP_AUTHOR_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAuthorResDef	msg_body;
}BsipAuthorInfoResDef;

/*---------------------------------�۷�---------------------------------*/
typedef struct BsipAccountInfoReq
{
	BsipHeadDef			msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountReqDef	msg_body;
}BsipAccountInfoReqDef;
                                                 
typedef struct BsipAccountInfoRes
{
	BsipHeadDef			msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountResDef	msg_body;
}BsipAccountInfoResDef;

/*-------------------------------���μƷ�-------------------------------*/
/*-------------------------------�˻���֤-------------------------------*/
typedef struct BsipAccountAuthenInfoReq
{
	BsipHeadDef			        msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountAuthenReqDef	    msg_body;

}BsipAccountAuthenInfoReqDef;

typedef struct BsipAccountAuthenInfoRes
{
	BsipHeadDef			        msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountAuthenResDef	    msg_body;
}BsipAccountAuthenInfoResDef;

/*---------------------------------Ԥ��---------------------------------*/
typedef struct BsipAccountLockInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountLockReqDef	msg_body;				
}BsipAccountLockInfoReqDef;

typedef struct BsipAccountLockInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountLockResDef	msg_body;			
}BsipAccountLockInfoResDef;

/*---------------------------------�ⶳ---------------------------------*/
typedef struct BsipAccountUnlockInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, Bsip_LOCK_ACCOUNT_RES��
	BsipAccountUnlockReqDef	msg_body;			
}BsipAccountUnlockInfoReqDef;

typedef struct BsipAccountUnlockInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountUnlockResDef	msg_body;			
}BsipAccountUnlockInfoResDef;

// 2007-3-3 14:40 zlc add
/*-------------------------------���۳�ֵ-------------------------------*/
typedef struct BsipConsignTransferInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, Bsip_LOCK_ACCOUNT_RES��
	BsipConsignTransferReqDef	msg_body;			
}BsipConsignTransferInfoReqDef;

typedef struct BsipConsignTransferInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignTransferResDef	msg_body;			
}BsipConsignTransferInfoResDef;

/*---------------------------------Ԥ����չ---------------------------------*/
typedef struct BsipAccountLockExInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountLockExReqDef	msg_body;				
}BsipAccountLockExInfoReqDef;

typedef struct BsipAccountLockExInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountLockExResDef	msg_body;			
}BsipAccountLockExInfoResDef;

/*---------------------------------�ⶳ��չ---------------------------------*/
typedef struct BsipAccountUnlockExInfoReq
{
	BsipHeadDef					msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, Bsip_LOCK_ACCOUNT_RES��
	BsipAccountUnlockExReqDef	msg_body;			
}BsipAccountUnlockExInfoReqDef;

typedef struct BsipAccountUnlockExInfoRes
{
	BsipHeadDef					msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountUnlockExResDef	msg_body;			
}BsipAccountUnlockExInfoResDef;

/*---------------------------------�콱---------------------------------*/
/*-------------------------------�콱��֤-------------------------------*/
typedef struct BsipAwardAuthenInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAwardAuthenReqDef	msg_body;				
}BsipAwardAuthenInfoReqDef;

typedef struct BsipAwardAuthenInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAwardAuthenResDef	msg_body;			
}BsipAwardAuthenInfoResDef;

/*-------------------------------�콱ȷ��-------------------------------*/
typedef struct BsipAwardAckInfo
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAwardAckDef			msg_body;			
}BsipAwardAckInfoDef;

typedef struct BsipAwardAckInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAwardAckResDef		msg_body;			
}BsipAwardAckInfoResDef;

/*-------------------------------���۽���-------------------------------*/
/*-------------------------------����Ԥ��-------------------------------*/
typedef struct BsipConsignLockInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignLockReqDef	msg_body;				
}BsipConsignLockInfoReqDef;

typedef struct BsipConsignLockInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignLockResDef	msg_body;			
}BsipConsignLockInfoResDef;

/*-------------------------------���۽ⶳ-------------------------------*/
typedef struct BsipConsignUnlockInfoReq
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, Bsip_LOCK_ACCOUNT_RES��
	BsipConsignUnlockReqDef	msg_body;			
}BsipConsignUnlockInfoReqDef;

typedef struct BsipConsignUnlockInfoRes
{
	BsipHeadDef				msg_head;			// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignUnlockResDef	msg_body;			
}BsipConsignUnlockInfoResDef;

/*-------------------------------���۳�ֵ-------------------------------*/
typedef struct BsipConsignDepositInfoReq
{
	BsipHeadDef				 msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignDepositReqDef msg_body;
}BsipConsignDepositInfoReqDef;

typedef struct BsipConsignDepositInfoRes
{
	BsipHeadDef				 msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipConsignDepositResDef msg_body;
}BsipConsignDepositInfoResDef;

/*-------------------------------Ԫ������-------------------------------*/
/*-------------------------------Ԫ����ֵ-------------------------------*/
typedef struct BsipGoldDepositInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipGoldDepositReqDef	msg_body;
}BsipGoldDepositInfoReqDef;

typedef struct BsipGoldDepositInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipGoldDepositResDef	msg_body;
}BsipGoldDepositInfoResDef;

//����ȯ
typedef struct BsipTokenDepositInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipTokenDepositReqDef	msg_body;
}BsipTokenDepositInfoReqDef;

typedef struct BsipTokenDepositInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipTokenDepositResDef	msg_body;
}BsipTokenDepositInfoResDef;

/*-------------------------------Ԫ����ʱ��-------------------------------*/
typedef struct BsipGoldConsumeLockInfoReq
{
	BsipHeadDef					msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipGoldConsumeLockReqDef	msg_body;
}BsipGoldConsumeLockInfoReqDef;

typedef struct BsipGoldConsumeLockInfoRes
{
	BsipHeadDef					msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipGoldConsumeLockResDef	msg_body;
}BsipGoldConsumeLockInfoResDef;

typedef struct BsipGoldConsumeUnlockInfoReq
{
	BsipHeadDef					msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_Unlock_ACCOUNT_RES��
	BsipGoldConsumeUnlockReqDef	msg_body;
}BsipGoldConsumeUnlockInfoReqDef;

typedef struct BsipGoldConsumeUnlockInfoRes
{
	BsipHeadDef					msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_Unlock_ACCOUNT_RES��
	BsipGoldConsumeUnlockResDef	msg_body;
}BsipGoldConsumeUnlockInfoResDef;

/*-------------------------------���û���ֵ-------------------------------*/
typedef struct BsipAccountImportInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountImportReqDef	msg_body;
}BsipAccountImportInfoReqDef;

typedef struct BsipAccountImportInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipAccountImportResDef	msg_body;
}BsipAccountImportInfoResDef;


/*-------------------------------ͳ����Ϣ-------------------------------*/
typedef struct BsipNormalStatInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipNormalStatReqDef	msg_body;
}BsipNormalStatInfoReqDef;

/*-------------------------------����ϵͳ-------------------------------*/
typedef struct BsipItemSyncInfoReq
{
	BsipHeadDef       		msg_head;
	BsipItemSyncReqDef  	msg_body;
} BsipItemSyncInfoReqDef;    						//��Ʒͬ��������Ϣ

typedef struct BsipItemSyncInfoRes
{
	BsipHeadDef       		msg_head;
	BsipItemSyncResDef  	msg_body;
} BsipItemSyncInfoResDef;    						//��Ʒͬ����Ӧ��Ϣ

/*--------------------------------GS·��--------------------------------*/
typedef struct BsipGsRouteInfoReq
{
	BsipHeadDef       		msg_head;
	BsipGsRouteReqDef  		msg_body;
} BsipGsRouteInfoReqDef;							//GS·��������Ϣ

typedef struct BsipGsRouteInfoRes
{
	BsipHeadDef       		msg_head;
	BsipGsRouteResDef  		msg_body;
} BsipGsRouteInfoResDef;    						//GS·����Ϣ��Ӧ

/*-------------------------------�ܱ���-------------------------------*/
typedef struct BsipEkeyBindInfoReq
{
	BsipHeadDef             msg_head;
	BsipEkeyBindReqDef      msg_body;
}BsipEkeyBindInfoReqDef;                            // �ܱ�����Ϣ

typedef struct BsipEkeyBindInfoRes
{
	BsipHeadDef       		msg_head;
	BsipEkeyBindResDef  		msg_body;
}BsipEkeyBindInfoResDef;    						//�ܱ�����Ϣ��Ӧ

#endif
