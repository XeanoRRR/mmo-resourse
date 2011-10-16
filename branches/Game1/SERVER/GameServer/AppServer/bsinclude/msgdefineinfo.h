 /*
* Copyright (c) 2005,�Ϻ�ʢ������Ʒ�ƽ̨��
* All rights reserved.
* 
* �ļ����ƣ�msgdefineinfo.h
* ժ    Ҫ���Ʒ�ϵͳ������ͷ�ļ���Ϣ
*           client��LBS��CBS��ͨѶ�õ���Ϣ�嶨��
* ��ǰ�汾��1.5.0
* ��    �ߣ��ܺ���
* ������ڣ�2007��1��12��
*
* �޸����ݣ��汾�Ÿ���Ϊ1.6
* �޸�ʱ�䣺2007��5��3��
*
*/

#ifndef _MSG_DEFINE_INFO_H_
#define _MSG_DEFINE_INFO_H_

#include "msgmacro.h"
#include "msgerrcode.h"
#include "commonerrcode.h"

// zlc 2007-8-14 13:10 Ack��Ϣ
#define BSIP_ACK              0x800000ff	//ack��Ϣ

// message version
#ifndef BSIP_MESSAGE_VERSION
#define BSIP_MESSAGE_VERSION			0x00010601   //main version 1, sub version 4, second sub version 2
#endif

#define BSIP_HEARTBEAT_REQ				0x00000014	//����������Ϣ
#define BSIP_HEARTBEAT_RES				0x80000014	//������Ӧ��Ϣ

#define BSIP_FLAG_LOCAL_NORMAL			0x00100000	//��������
#define BSIP_FLAG_LOCAL_BUFFER			0x00100001	//�ͻ���API�����ļ�
#define BSIP_FLAG_LOCAL_ROUND			0x00100002	//�ͻ���API�����ļ�

#define BSIP_FLAG_LBS_NORMAL			0x00200000	//LBS��������
#define BSIP_FLAG_LBS_BUFFER			0x00200001	//LBS API�����ļ�
#define BSIP_FLAG_LBS_ROUND				0x00200002	//LBS API�����ļ�
#define BSIP_FLAG_LBS_SYNC              0x00200003  //LBS ͬ������

#define BSIP_FLAG_CBS_NORMAL			0x00300000	//CBS��������
#define BSIP_FLAG_CBS_BUFFER			0x00300001	//CBS API�����ļ�
#define BSIP_FLAG_CBS_ROUND				0x00300002	//CBS API�����ļ�
#define BSIP_FLAG_CBS_SYNC              0x00300003  //CBS ͬ������

#define IND_TOTAL_COSUMPTION            0x00000100  //��ѯ�����ۼ�����

//md5ǩ����Χ(����int, short���������ֽ���):
//	private_key(16 bytes)+msg_head(msg_len+msg_type+seq_id+send_time+priority+version, total 40 bytes)				
// 	+the whole msg_body 
/*---------------------------------��Ϣͷ---------------------------------*/
typedef struct BsipHead
{
	int 			msg_len;						// ������Ϣ�����ֽڳ��ȣ�����head��body
	int	 			msg_type;						// ��Ϣ���ͣ���BSIP_AUTHOR_REQ, BSIP_LOCK_ACCOUNT_RES��
	int  			version;						// �汾����
	int				host_id;						// ��Ϸ���������
	int				flag;							// ��Ϣ��Դ��־����BSIP_FLAG_LOCAL_NORMAL��
	char 			priority;						// ���ȼ�
	int 			seq_id;							// ��Ϣ��ˮ��,˳���ۼ�,����Ϊ1,ѭ��ʹ�ã�һ�������Ӧ����Ϣ����ˮ�ű�����ͬ��
	int 			gs_send_time;					// gs���ݿ鷢��ʱ��
	int 			lbs_send_time;					// lbs���ݿ鷢��ʱ��
	int				api_ip; 	 					// ����ip��ַ
	int				gs_ip; 	 						// GS ip��ַ
	int				lbs_ip; 	 					// LBS ip��ַ
	int				api_port; 	 					// ����port�˿�
	int				gs_port; 	 					// GS port�˿�
	int				lbs_port; 	 					// LBS port�˿�
	unsigned char 	digest[MAX_DIGEST_LEN];			// ���ܽ����Md5 digest������ShareSecretKey ��client�˺�server֮��ά���Ĺ���key��	
}BsipHeadDef;

/*-------------------------------������Ϣ-------------------------------*/
typedef struct BsipHeartBeatReq
{
    char ip[IP_ADDRESS_LEN+1];         //ip��Ϸ�ͻ��˵�ַ�� �磺61.172.252.37
}BsipHeartBeatReqDef;

typedef struct BsipHeartBeatRes
{
    int  result;                       //���ؽ����0���ɹ�
    char ip[IP_ADDRESS_LEN+1];         //ip��Ϸ�ͻ��˵�ַ�� �磺61.172.252.37
}BsipHeartBeatResDef;

/*-------------------------------������Ϣ-------------------------------*/
typedef struct BsipHeartBeatInfoReq
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipHeartBeatReqDef		msg_body;
}BsipHeartBeatInfoReqDef;

typedef struct BsipHeartBeatInfoRes
{
	BsipHeadDef				msg_head;				// ��Ϣ���ͣ���BSIP_AUTHEN_REQ, BSIP_LOCK_ACCOUNT_RES��
	BsipHeartBeatResDef		msg_body;
}BsipHeartBeatInfoResDef;


// �۷�ack
typedef struct BsipAck
{
    int msg_type;
    char order_id[33];
}BsipAckDef;

typedef struct BsipAckInfo
{
	BsipHeadDef       	msg_head;
	BsipAckDef  		msg_body;
}BsipAckInfoDef;    						//

#endif
