/*
* Copyright (c) 2005,�Ϻ�ʢ������Ʒ�ƽ̨��
* All rights reserved.
*
* �ļ����ƣ�bsipmsgbs.h
* ժ    Ҫ�����ӿ���Ϣ�嶨�壨client��LBS��CBS����Ϣ������
* ��ǰ�汾��1.4.2
* ��    �ߣ�����
* ������ڣ�2005��9��6��
*
* �޸����ݣ���1.3�İ汾�Ͻ����޸ģ�ɾ�����õ���Ϣ
*           ��Ӽ��۽���ʹ�õ���Ϣ
* �޸�ʱ�䣺2005��9��6��
*
* �޸����ݣ�����궨��
* �޸�ʱ�䣺2005��9��21��
*
* �޸����ݣ�Author�ӿ����account_id�ֶ�
*           Account�ӿ����account_id��map_id�ֶ�
* �޸�ʱ�䣺2005��10��8��
*
* �޸����ݣ����Notify��Ϣ���ܽӿ�
* �޸�ʱ�䣺2005��10��13��
*
* �޸����ݣ����AccountLock\AccountUnlock\CosignLock
*               \ConsignDeposit\ConsignUnlock��Ϣ�е�item_id���ֶ�
* �޸�ʱ�䣺2005��11��25��
*
* �޸����ݣ����AccountLock\AccountUnlock\CosignLock
*               \ConsignDeposit\ConsignUnlock��Ϣ�е�item_num��item_price���ֶ�
* �޸�ʱ�䣺2005��11��28��
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
* �޸����ݣ�����char1��Ϊint��ptid��sdid�ϲ�Ϊuserid��
*           �ϳ����ֶ����ֶΣ��޸Ĳ����ֶ����������ֶ�˳��
* �޸�ʱ�䣺2007-2-1 17:19
*
* �޸����ݣ�����ConsignTransfer��Ϣ���ϳ�ԭ������Ϣ��֧��һ��Ԥ����νⶳ��
*           �޸ĵ�����ϢΪ�䳤���ݣ�֧�ִ�������
* �޸�ʱ�䣺2007-5-3 9:25
*
* �޸����ݣ�������Ϣ�ṹ���ģ�
*           �޸�individual�ֶ���Ϊappendix��128�ֽڣ�������Ӧ����Ӹ��ֶ�
* �޸�ʱ�䣺2007-5-3 9:25
*
* �޸����ݣ�����EkeyBind��Ϣ
* �޸�ʱ�䣺2007-6-19 15:54
*
* �޸����ݣ��޸�BsipNormalStatReq��Ϣ��ȫ1.6�е�13����Ϣ���޸�BsipStat������Ӧ�ӳ�ͳ��
* �޸�ʱ�䣺2007-8-1
*/

/*
*bsip msg body ,
*/

#ifndef _BSIP_MSG_BS_H_
#define _BSIP_MSG_BS_H_

#include "../msgdefineinfo.h"


/*------------------------------��Ϣ�嶨��------------------------------*/
/*-------------------------------ʱ���Ʒ�-------------------------------*/
/*---------------------------------��Ȩ---------------------------------*/
typedef struct BsipAuthorReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  user_type;                      //�û�����
    int  status;                         //�û�״̬
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];   //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipAuthorReqDef;

typedef struct BsipAuthorRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //��������
    int  balance;                        //ʣ��ĵ���������������(��Ӧpay_type)
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    char begin_time[MAX_TIME_LEN+1];     //��Чʱ�䣨�����ֶΣ�
    char end_time[MAX_TIME_LEN+1];       //ʧЧʱ�䣨�����ֶΣ�
}BsipAuthorResDef;

/*---------------------------------�۷�---------------------------------*/
typedef struct BsipAccountReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  pay_type;                       //��������
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  amount;                         //��������
    int  discount;                       //�Ʒ��ۿ���
    int  map_id;                         //��ͼid
    int  responsable;                    //�Ƿ���Ҫ��Ӧ��־
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char begin_time[MAX_TIME_LEN+1];     //����ʱ��
    char end_time[MAX_TIME_LEN+1];       //����ʱ��
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  trade_area_id;                  //���������������ڴ������ţ�
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipAccountReqDef;

typedef struct BsipAccountRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //��������
    int  balance;                        //ʣ��ĵ���������������(��Ӧpay_type)
    int  map_id;                         //��ͼid
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
}BsipAccountResDef;

/*-------------------------------���μƷ�-------------------------------*/
/*-------------------------------�˻���֤-------------------------------*/
typedef struct BsipAccountAuthenReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  user_type;                      //�û�����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;                 //ҵ�����ͱ��
    int  area_id_player;                 //��Ϸ�����
    int  group_id_player;                //��ʾ��id
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
    int  indication_authen;              //��֤��־
    int  pay_type;                       //ָ��������֤��һ�����
}BsipAccountAuthenReqDef;

typedef struct BsipAccountAuthenRes
{
    int  result;                         //���ؽ��
    int  pay_type1;                      //��������
    int  balance1;                       //ʣ��ĵ���������������(��Ӧpay_type)
    int  pay_type2;                      //��������
    int  balance2;                       //ʣ��ĵ���������������(��Ӧpay_type)
    int  pay_type3;                      //��������
    int  balance3;                       //ʣ��ĵ���������������(��Ӧpay_type)
    int  pay_type4;                      //��������
    int  balance4;                       //ʣ��ĵ���������������(��Ӧpay_type)
    int  pay_type5;                      //��������
    int  balance5;                       //ʣ��ĵ���������������(��Ӧpay_type)
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char begin_time[MAX_TIME_LEN+1];     //��Чʱ�䣨�����ֶΣ�
    char end_time[MAX_TIME_LEN+1];       //ʧЧʱ�䣨�����ֶΣ�
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    char appendix[128];
}BsipAccountAuthenResDef;

///  goods_info �Ľṹ
typedef struct BsipGoods
{
	int serv_id_item;                   //������������Ϸ�����
	int area_id_item;                   //������������Ϸ�������
	int group_id_item;                  //������������Ϸ�飩���
    int item_id;                        //��Ϸ���߱��
    int item_num;                       //��Ϸ��������
	int item_amount;                    //����Ϸ���ߵ��ܼۣ��ǵ��ۣ�
	int discount;
}BsipGoodsDef;

///  item_info �Ľṹ
typedef struct BsipItem
{ //��Ϊ����ṹֱ�Ӳ�����GS�����Բ���Ҫserviceid����Ϣ
    int  item_id;                        //��Ϸ���߱��
    int  item_num;                       //��Ϸ��������
    char item_property[32];              //����Ϸ���ߵ�����
}BsipItemDef;

/*---------------------------------Ԥ��---------------------------------*/
typedef struct BsipAccountLockReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  pay_type;                       //��������
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  fee;                            //������
    int  lock_period;                    //lock���ʱ��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;                        //ҵ�����ͱ��
    int  area_id_player;                        //��Ϸ�����
    int  group_id_player;                       //��ʾ��id
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
    int  indication_lock;
    int  item_num;                       //��Ч����������
    BsipGoodsDef goods_info[1];          //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� goods_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(BsipGoodsDef)
}BsipAccountLockReqDef;

typedef struct BsipAccountLockRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //��������
    int  balance;                        //ʣ���PT��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char award_num[MAX_AWARD_LEN+1];     //�콱����
    char appendix[128];                  //������Ϣ
}BsipAccountLockResDef;

/*---------------------------------�ⶳ---------------------------------*/
typedef struct BsipAccountUnlockReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  pay_type;                       //��������
    int  amount;                   //�ⶳ�������ǲ���Ԥ�����ֶ�νⶳ
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  confirm;                        //ȷ�Ͽ۳�
    int  responsable;                    //�Ƿ���Ҫ��Ӧ��־
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;                        //ҵ�����ͱ��
    int  area_id_player;                        //��Ϸ�����
    int  group_id_player;                       //��ʾ��id
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
    int  indication_unlock;
    int  item_num;                       //��Ч����������
    BsipGoodsDef goods_info[1];          //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� goods_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(BsipGoodsDef)
}BsipAccountUnlockReqDef;

typedef struct BsipAccountUnlockRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //��������
    int  balance;                        //ʣ����
    int  balance_locked;                 //�������δ���ⶳ��ʣ����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char appendix[128];                  //������Ϣ
}BsipAccountUnlockResDef;

/*---------------------------------����ת��---------------------------------*/
typedef struct BsipConsignTransferReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  app_type;
    int  serv_id_player_dest;
    int  area_id_player_dest;           //�����
    int  group_id_player_dest;          //����
    int  uid_type_dest;                 //����ʺ����ͣ�1��pt�ʻ���2������id
    char user_id_dest[MAX_PTID_LEN+1];  //�˺�id�������˺Ż�pt�˺ţ�
    int  fee_dest;                      //��Ǯ������Ҫ����������
    int  pay_type;                      //���ѵķ�ʽ
    int  amount;                        //���
    int  serv_id_player_src;            //��Ϸ�����
    int  area_id_player_src;            //��Ϸ�����
    int  group_id_player_src;           //��ʾ��id
    int  uid_type_src;                  //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id_src[MAX_PTID_LEN+1];   //�˺�id�������˺Ż�pt�˺ţ�
    int  fee_src;                       //����Ǯ������Ҫ����������
    char sess_id[MAX_SESSID_LEN+1];     //�Ựid
    char context_id[MAX_ORDERID_LEN+1]; //����id
    char order_id[MAX_ORDERID_LEN+1];   //Ψһ��ʶ
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1]; //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];     //���ݰ�����ʱ��
    int  indication_unlock;
    int  item_num;                      //��Ч����������
    BsipGoodsDef goods_info[1];          //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� goods_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(BsipGoodsDef)
}BsipConsignTransferReqDef;

typedef struct BsipConsignTransferRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //���ѵķ�ʽ
    int  uid_type_dest;                  //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id_dest[MAX_PTID_LEN+1];   //����˺�id�������˺Ż�pt�˺ţ�
    int  balance_dest;                   //ʣ��PT��
    int  uid_type_src;                   //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id_src[MAX_PTID_LEN+1];    //����˺�id�������˺Ż�pt�˺ţ�
    int  balance_locked_src;             //��Ǯ����ʣ��Ԥ�������
    int  balance_src;                    //ʣ��PT��
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ
    char appendix[128];                  //������Ϣ
}BsipConsignTransferResDef;

/*---------------------------------Ԥ����չ---------------------------------*/
typedef struct BsipAccountLockExReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  fee;
    int  lock_period;                    //lock���ʱ��
    int  pay_type1;                      //��������
    int  amount1;                        //���ѽ���Ӧpay_type��
    int  pay_type2;                      //��������
    int  amount2;                        //���ѽ���Ӧpay_type��
    int  pay_type3;                      //��������
    int  amount3;                        //���ѽ���Ӧpay_type��
    int  pay_type4;                      //��������
    int  amount4;                        //���ѽ���Ӧpay_type��
    int  pay_type5;                      //��������
    int  amount5;                        //���ѽ���Ӧpay_type��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
    int  item_num;                       //��Ч����������
    BsipGoodsDef goods_info[1];          //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� goods_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(BsipGoodsDef)
}BsipAccountLockExReqDef;

typedef struct BsipAccountLockExRes
{
    int  result;                         //���ؽ��
    int  pay_type1;                      //��������
    int  balance1;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type2;                      //��������
    int  balance2;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type3;                      //��������
    int  balance3;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type4;                      //��������
    int  balance4;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type5;                      //��������
    int  balance5;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char award_num[MAX_AWARD_LEN+1];     //�콱����
    char appendix[128];                  //������Ϣ
}BsipAccountLockExResDef;

/*---------------------------------�ⶳ��չ---------------------------------*/
typedef struct BsipAccountUnlockExReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  confirm;                        //ȷ�Ͽ۳�
    int  pay_type1;                      //��������
    int  amount1;                        //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type2;                      //��������
    int  amount2;                        //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type3;                      //��������
    int  amount3;                        //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type4;                      //��������
    int  amount4;                        //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type5;                      //��������
    int  amount5;                        //ʣ��ĵ�������������������Ӧpay_type��
    int  responsable;                    //�Ƿ���Ҫ��Ӧ��־
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
    int  item_num;                       //��Ч����������
    BsipGoodsDef goods_info[1];          //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� goods_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(BsipGoodsDef)
}BsipAccountUnlockExReqDef;

typedef struct BsipAccountUnlockExRes
{
    int  result;                         //���ؽ��
    int  pay_type1;                      //��������
    int  balance1;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type2;                      //��������
    int  balance2;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type3;                      //��������
    int  balance3;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type4;                      //��������
    int  balance4;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  pay_type5;                      //��������
    int  balance5;                       //ʣ��ĵ�������������������Ӧpay_type��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char appendix[128];                  //������Ϣ
}BsipAccountUnlockExResDef;

/*---------------------------------�콱---------------------------------*/
/*-------------------------------�콱��֤-------------------------------*/
typedef struct BsipAwardAuthenReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  user_type;                      //�û�����
    int  couple_type;                    //�������
    int  award_type;                     //�콱����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char award_num[MAX_AWARD_LEN+1];     //�콱����
    char award_patchid[MAX_AWARDPATCH_LEN+1]; //�콱���κ�
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
    char role_name[32+1];                //��ҵ�ǰ��ɫ��
    int  current_level;                  //��ҵ�ǰ��ɫ����
    int  award_level;                    //����콱����
    char ind_msg[128];                   //������Ϣ������
}BsipAwardAuthenReqDef;

typedef struct BsipAwardAuthenRes
{
    int  result;                         //���ؽ��
    int  couple_type;                    //�������
    int  bind_user;                      //��û����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //��Ϸ�ͻ��˵�IP��ַ
    char award_num[MAX_AWARD_LEN+1];     //�콱����
    char appendix[128];                  //������Ϣ
    int  item_num;                       //��Ч����������
    BsipItemDef item_info[1];            //�䳤���飬Ϊ���ṩһ�������ĵ�ַ�ռ���׵�ַ
                                         //�� item_info �Ľṹ  �ֽ���=
                                         //item_num * sizeof(GSBsipItemDef)
}BsipAwardAuthenResDef;

/*-------------------------------�콱ȷ��-------------------------------*/
typedef struct BsipAwardAck
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  confirm;                        //
    int  award_type;                     //�콱����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char award_num[MAX_AWARD_LEN+1];     //�콱����
    char award_time[MAX_TIME_LEN+1];     //�����ɹ���ʧ��ʱ��
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
}BsipAwardAckDef;

typedef struct BsipAwardAckRes
{
    int  result;                         //���ؽ��
    int  couple_type;                    //�������
    int  award_type;                     //��Ʒ����
    int  pay_type;                       //��ֵ����
    int  balance;                        //��ֵ��ʣ������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    char appendix[128];                  //������Ϣ
}BsipAwardAckResDef;

/*-------------------------------���۽���-------------------------------*/
/*-------------------------------����Ԥ��-------------------------------*/
typedef struct BsipConsignLockReq
{
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //��������
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  lock_period;                    //lock���ʱ��
    int  discount;                       //�Ʒ��ۿ���
    int  item_num;                       //��Ч����������
    int  item1_id;                       //��Ϸ���߱��
    int  item1_num;                      //��Ϸ��������
    int  item1_price;                    //��Ϸ���ߵ���
    int  item2_id;                       //��Ϸ���߱��
    int  item2_num;                      //��Ϸ��������
    int  item2_price;                    //��Ϸ���ߵ���
    int  item3_id;                       //��Ϸ���߱��
    int  item3_num;                      //��Ϸ��������
    int  item3_price;                    //��Ϸ���ߵ���
    int  item4_id;                       //��Ϸ���߱��
    int  item4_num;                      //��Ϸ��������
    int  item4_price;                    //��Ϸ���ߵ���
    int  item5_id;                       //��Ϸ���߱��
    int  item5_num;                      //��Ϸ��������
    int  item5_price;                    //��Ϸ���ߵ���
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
    char log_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipConsignLockReqDef;

typedef struct BsipConsignLockRes
{
    int  result;                         //���ؽ��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //��������
    int  balance;                        //ʣ���PT��
    int  item_num;                       //��Ч����������
    int  item1_id;                       //��Ϸ���߱��
    int  item1_num;                      //��Ϸ��������
    int  item1_price;                    //��Ϸ���ߵ���
    int  item2_id;                       //��Ϸ���߱��
    int  item2_num;                      //��Ϸ��������
    int  item2_price;                    //��Ϸ���ߵ���
    int  item3_id;                       //��Ϸ���߱��
    int  item3_num;                      //��Ϸ��������
    int  item3_price;                    //��Ϸ���ߵ���
    int  item4_id;                       //��Ϸ���߱��
    int  item4_num;                      //��Ϸ��������
    int  item4_price;                    //��Ϸ���ߵ���
    int  item5_id;                       //��Ϸ���߱��
    int  item5_num;                      //��Ϸ��������
    int  item5_price;                    //��Ϸ���ߵ���
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
}BsipConsignLockResDef;

/*-------------------------------���۽ⶳ-------------------------------*/
typedef struct BsipConsignUnlockReq
{
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //��������
    int  app_type;                       //�������
    int  status;                         //�û�״̬
    int  couple_type;                    //�������
    int  discount;                       //�Ʒ��ۿ���
    int  confirm;                        //ȷ�Ͽ۳�
    int  responsable;                    //�Ƿ���Ҫ��Ӧ��־
    int  item_num;                       //��Ч����������
    int  item1_id;                       //��Ϸ���߱��
    int  item1_num;                      //��Ϸ��������
    int  item1_price;                    //��Ϸ���ߵ���
    int  item2_id;                       //��Ϸ���߱��
    int  item2_num;                      //��Ϸ��������
    int  item2_price;                    //��Ϸ���ߵ���
    int  item3_id;                       //��Ϸ���߱��
    int  item3_num;                      //��Ϸ��������
    int  item3_price;                    //��Ϸ���ߵ���
    int  item4_id;                       //��Ϸ���߱��
    int  item4_num;                      //��Ϸ��������
    int  item4_price;                    //��Ϸ���ߵ���
    int  item5_id;                       //��Ϸ���߱��
    int  item5_num;                      //��Ϸ��������
    int  item5_price;                    //��Ϸ���ߵ���
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
    char log_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipConsignUnlockReqDef;

typedef struct BsipConsignUnlockRes
{
    int  result;                         //���ؽ��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //��������
    int  balance;                        //ʣ���PT��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
}BsipConsignUnlockResDef;

/*-------------------------------���۳�ֵ-------------------------------*/
typedef struct BsipConsignDepositReq
{
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ������
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  user_type;                      //�û�����
    int  pay_type;                       //���ѵķ�ʽ
    int  amount;                         //Ԫ�����
    int  buyer_serv_id;                  //ҵ������
    int  buyer_area_id;                  //��Ϸ�����
    int  buyer_group_id;                 //��ʾ��id
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    int  buyer_uid_type;                 //����ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char buyer_user_id[MAX_PTID_LEN+1];  //����˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char patch_id[MAX_PATCHID_LEN+1];    //���κ�
    char ip[IP_ADDRESS_LEN+1];           //ip��Ϸ�ͻ��˵�ַ
    char log_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipConsignDepositReqDef;

typedef struct BsipConsignDepositRes
{
    int  result;                         //���ؽ��
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ������
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  buyer_serv_id;                  //ҵ������
    int  buyer_area_id;                  //��Ϸ�����
    int  buyer_group_id;                 //��ʾ��id
    int  pay_type;                       //���ѵķ�ʽ
    int  balance;                        //ʣ��PT��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    int  buyer_uid_type;                 //����ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char buyer_user_id[MAX_PTID_LEN+1];  //����˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ
    char context_id[MAX_ORDERID_LEN+1];  //����id
    char patch_id[MAX_PATCHID_LEN+1];    //���κ�
    char ip[IP_ADDRESS_LEN+1];           //ip��Ϸ�ͻ��˵�ַ
}BsipConsignDepositResDef;

/*-------------------------------Ԫ������-------------------------------*/
/*-------------------------------Ԫ����ֵ-------------------------------*/
typedef struct BsipGoldDepositReq
{
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  user_type;                      //�û�����
    int  pay_type;                       //��������
    int  amount;                         //��ֵ���
    int  sync_flag;                      //��ֵ��Ϣ��־λ
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
}BsipGoldDepositReqDef;

typedef struct BsipGoldDepositRes
{
    int  result;                         //���ؽ��
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ�����ͱ��
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //��������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char ip[IP_ADDRESS_LEN+1];           //��Ϸ�ͻ��˵�IP��ַ
}BsipGoldDepositResDef;

/*-------------------------------Ԫ����ʱ��-------------------------------*/
typedef struct BsipGoldConsumeLockReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  couple_type;                    //�������
    int  discount;                       //�Ʒ��ۿ���
    int  src_pay_type;                       //��������
    int  src_amount;                         //��������
    int  dest_pay_type;                      //�����ʱ������
    int  dest_amount;                    //�һ���ʱ������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //���ݰ�id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
}BsipGoldConsumeLockReqDef;

typedef struct BsipGoldConsumeLockRes
{
    int  result;                         //���ؽ��
    int  src_pay_type;                   //��������
    int  src_balance;                    //�һ����ܵĵ�ȯ����
    int  dest_pay_type;                  //�����ʱ������
    int  dest_balance;                   //�һ����ܵ�ʱ������
    int  dest_amount;                    //�һ���ʱ������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //���ݰ�id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    char begin_time[MAX_TIME_LEN+1];     //��Чʱ�� ��time_type Ϊ1 ʱ��Ч��
    char end_time[MAX_TIME_LEN+1];       //ʧЧʱ�� ��time_type Ϊ1 ʱ��Ч��
    char appendix[128];                  //������Ϣ
}BsipGoldConsumeLockResDef;

typedef struct BsipGoldConsumeUnlockReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  confirm;                        //ȷ�Ͽ۳�
    int  couple_type;                    //�������
    int  discount;                       //�Ʒ��ۿ���
    int  src_pay_type;                   //��������
    int  src_amount;                     //��������
    int  dest_pay_type;                  //�����ʱ������
    int  dest_amount;                    //�һ���ʱ������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //���ݰ�id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
}BsipGoldConsumeUnlockReqDef;

typedef struct BsipGoldConsumeUnlockRes
{
    int  result;                         //���ؽ��
    int  src_pay_type;                   //��������
    int  src_balance;                    //�һ����ܵĵ�ȯ����
    int  dest_pay_type;                  //�����ʱ������
    int  dest_balance;                   //�һ����ܵ�ʱ������
    int  dest_amount;                    //�һ���ʱ������
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //������id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    char begin_time[MAX_TIME_LEN+1];     //��Чʱ�� ��time_type Ϊ1 ʱ��Ч��
    char end_time[MAX_TIME_LEN+1];       //ʧЧʱ�� ��time_type Ϊ1 ʱ��Ч��
    char appendix[128];                  //������Ϣ
}BsipGoldConsumeUnlockResDef;

/*-------------------------------���û���ֵ-------------------------------*/
typedef struct BsipAccountImportReq
{
    int  sp_id;                          //Service provider�ı��
    int  serv_id;                        //ҵ������
    int  area_id;                        //��Ϸ�����
    int  group_id;                       //��ʾ��id
    int  pay_type;                       //���ѵķ�ʽ
    int  amount;                         //��ֵ���
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char ip[IP_ADDRESS_LEN+1];           //ip��Ϸ�ͻ��˵�ַ
    char log_time[MAX_TIME_LEN+1];       //���ݰ�����ʱ��
}BsipAccountImportReqDef;

typedef struct BsipAccountImportRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //���ѵķ�ʽ
    int  balance;                        //ʣ����
    int  uid_type;                      //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
}BsipAccountImportResDef;

/*-------------------------------����ϵͳ-------------------------------*/
typedef struct BsipItemSyncReq
{
    int  sp_id;                          //����ʵ���ô���Ԥ��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ
    int  serv_id;                        //��ϷӦ�ñ���
    int  area_id;                        //��Ϸ����
    int  group_id;                       //��Ϸ���
    int  sync_times;                     //ͬ������
    int  indication_sync;                //ָ����Ϣ Ԥ��
    char msg_sync[256];                  //ͬ����Ϣ
    char item_info[256];                 //������Ϣ����ʽΪ��
                                         //���߱��1,����1;���߱��2,����2;���߱��3,����3;����
                                         //��ͬ���߼�ʹ�ð�Ƿֺŷָ���
                                         //���߱�ź�����֮���ð�Ƕ��ŷָ�
}BsipItemSyncReqDef;

typedef struct BsipItemSyncRes
{
    int  result;                         //ͬ�������0�ɹ�������ʧ��
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ����������Ϣ���ֶ�һ��
}BsipItemSyncResDef;

/*--------------------------------GS·��--------------------------------*/
typedef struct BsipGsRouteReq
{
    char order_id[MAX_ORDERID_LEN+1];    //��lbs�Լ����죬cbs��У�飬ԭ������
    char lbs_ip[IP_ADDRESS_LEN+1];       //lbs ip��ַ
    int  lbs_port;                       //lbs�˿ں�
    char gs_ip[IP_ADDRESS_LEN+1];        //gs ip��ַ
    int  gs_port;                        //gs�˿ں�
    char action;                         //������'1'��ʾgs����lbs��'2'��ʾgs�Ͽ�lbs
} BsipGsRouteReqDef;

typedef struct BsipGsRouteRes
{
    int  result;                         //0��ʾ�ɹ�
    char order_id[MAX_ORDERID_LEN+1];    //Ψһ��ʶ����������Ϣ���ֶ�һ��
} BsipGsRouteResDef;

/*--------------------------gsͳ����Ϣ---------------------------------*/
typedef struct BsipStat
{
    int  total_num;
    int  success_num;
    int  response_num;
    int  delay1_num;                    //req��resʱ���1��2��
    int  delay2_num;                    //req��resʱ���2��5��
    int  delay3_num;                    //req��resʱ������5��
    int  delay4_num;                    //res��ʱ
    int  callback_delay_num;			//callbackʱ�䳬��100ms
}BsipStatDef;

typedef struct BsipNormalStatReq
{
    int  sp_id;                        //Service provider�ı��
    int  serv_id;                      //ҵ�����ͱ��
    int  area_id;                      //��Ϸ�����
    int  group_id;                     //��ʾ��id
    char report_time[MAX_TIME_LEN+1];  //ͳ��ʱ��
    char ip[IP_ADDRESS_LEN+1];         //ip��Ϸ�ͻ��˵�ַ
    char order_id[MAX_ORDERID_LEN+1];  //���ݰ�id
    BsipStatDef author;
    BsipStatDef account;
    BsipStatDef account_authen;
    BsipStatDef account_lock;
    BsipStatDef account_unlock;
    BsipStatDef account_lock_ex;
    BsipStatDef account_unlock_ex;
    BsipStatDef gold_consume_lock;
    BsipStatDef gold_consume_unlock;
    BsipStatDef token_deposit;
    BsipStatDef award_authen;
    BsipStatDef award_ack;
    BsipStatDef consign_transfer;
}BsipNormalStatReqDef;

/*����ȯͨѶЭ��*/
typedef struct BsipTokenDepositReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  pay_type;                       //��������
    int  amount;                         //��ֵ���
    int  item_id;                        //�������յĵ��߱��
    int  item_num;                       //�������յĵ�������
    int  item_price;                     //�������յĵ��ߵ���
    int  deposit_flag;                   //��ֵ��Ϣ��־λ 1-���򷵻� 2-���߻��� ����-����
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //order_id�����id
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    char patch_id[MAX_PATCHID_LEN+1];    //���κ�
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
    int  valid_period;                   //��ֵ������Ч�ڣ����¡�
}BsipTokenDepositReqDef;

typedef struct BsipTokenDepositRes
{
    int  result;                         //���ؽ��
    int  pay_type;                       //��������
    int  balance;                        //ʣ��ĵ���ȯ
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char context_id[MAX_ORDERID_LEN+1];  //order_id�����id
    char order_id[MAX_ORDERID_LEN+1];   //���ݰ�id
    char appendix[128];                  //������Ϣ
}BsipTokenDepositResDef;

/*------------------------------�ܱ���---------------------------------*/
typedef struct BsipEkeyBindReq
{
    int  sp_id;                          //Service provider�ı��
    int  site_code;
    int  operation_type;                 //1��ʾ�ܱ�����������
    int  indication_bind;                //1�����ܱ���2��������ܱ�
    int  pay_type;
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
    int  serv_id_player;
    int  area_id_player;
    int  group_id_player;
    int  endpoint_type;
    char endpoint_ip[IP_ADDRESS_LEN+1];  //�ն�IP��ַ
    int  platform_type;
    char call_time[MAX_TIME_LEN+1];      //���ݰ�����ʱ��
}BsipEkeyBindReqDef;

typedef struct BsipEkeyBindRes
{
    int  result;                         //���ؽ��
    int  uid_type;                       //�ʺ����ͣ�1��pt�ʻ���2������id
    char user_id[MAX_PTID_LEN+1];        //�˺�id�������˺Ż�pt�˺ţ�
    char sess_id[MAX_SESSID_LEN+1];      //�Ựid
    char order_id[MAX_ORDERID_LEN+1];    //���ݰ�id
}BsipEkeyBindResDef;

#endif
