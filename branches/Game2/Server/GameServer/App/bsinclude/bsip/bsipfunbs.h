/*
* Copyright (c) 2005,�Ϻ�ʢ������Ʒ�ƽ̨��
* All rights reserved.
* 
* �ļ����ƣ�bsipfunbs.h
* ժ    Ҫ�����ӿں�������
* 
* ��ǰ�汾��1.4
* ��    �ߣ�����
*
* �޸����ݣ�1.4�汾������ļ�
* �޸�ʱ�䣺2005��9��6��
*
* �޸����ݣ����Notify��Ϣ���ܽӿ�
* �޸�ʱ�䣺2005��10��13��
*/

#ifndef _BSIP_FUN_BS_H_

#define _BSIP_FUN_BS_H_ 

#ifdef WIN32
	#define BSIP_API_TYPE _declspec(dllexport)
#elif LINUX
	#define BSIP_API_TYPE
#endif

#include "bsip.h"

typedef struct
{
	void (*CallbackAuthorRes)(BsipAuthorInfoResDef * p );  
	void (*CallbackAccountRes)(BsipAccountInfoResDef *p );
	void (*CallbackAccountAuthenRes)(BsipAccountAuthenInfoResDef *p );		
	void (*CallbackEkeyBindRes)(BsipEkeyBindInfoResDef *p );  
	void (*CallbackAccountLockRes)(BsipAccountLockInfoResDef *p );  
	void (*CallbackAccountUnlockRes)(BsipAccountUnlockInfoResDef *p ); 	
	void (*CallbackAccountLockExRes)(BsipAccountLockExInfoResDef *p);  
	void (*CallbackAccountUnlockExRes)(BsipAccountUnlockExInfoResDef *p); 	
	void (*CallbackAwardAuthenRes)(BsipAwardAuthenInfoResDef *p );
	void (*CallbackAwardAckRes)(BsipAwardAckInfoResDef *p );
	void (*CallbackConsignTransferRes)(BsipConsignTransferInfoResDef *p );
	void (*CallbackGoldDepositReq)(BsipGoldDepositInfoReqDef *p );
    void (*CallbackTokenDepositRes)(BsipTokenDepositInfoResDef *p );
	void (*CallbackGoldConsumeLockRes)(BsipGoldConsumeLockInfoResDef *p );
	void (*CallbackGoldConsumeUnlockRes)(BsipGoldConsumeUnlockInfoResDef *p );
	void (*CallbackAccountImportRes)(BsipAccountImportInfoResDef *p ); 
	void (*CallbackItemSyncReq)( BsipItemSyncInfoReqDef * p );
	void (*CallbackGsRouteRes)( BsipGsRouteInfoResDef *p );    
}CallbackFuncDef;

#ifdef __cplusplus
extern "C" 
{
#endif
	BSIP_API_TYPE int Initialize(CallbackFuncDef *callbackFun, const char *configfilename,int client_type);
	BSIP_API_TYPE int SendEkeyBindRequestEx(			 BsipEkeyBindInfoReqDef *req);
	BSIP_API_TYPE int SendAccountLockRequestEx(			 BsipAccountLockInfoReqDef *req);
	BSIP_API_TYPE int SendAccountUnlockRequestEx(		 BsipAccountUnlockInfoReqDef *req);
	BSIP_API_TYPE int SendAccountLockExRequestEx(		 BsipAccountLockExInfoReqDef *req);
	BSIP_API_TYPE int SendAccountUnlockExRequestEx(		 BsipAccountUnlockExInfoReqDef *req);
	BSIP_API_TYPE int SendAccountAuthenRequestEx(		 BsipAccountAuthenInfoReqDef *req);
	BSIP_API_TYPE int SendAuthorRequestEx(				 BsipAuthorInfoReqDef *req);
	BSIP_API_TYPE int SendAwardAuthenRequestEx(			 BsipAwardAuthenInfoReqDef *req);
	BSIP_API_TYPE int SendAwardAckEx(					 BsipAwardAckInfoDef *req);
	BSIP_API_TYPE int SendAccountRequestEx(				 BsipAccountInfoReqDef *req);
	BSIP_API_TYPE int SendGoldConsumeLockRequestEx(		 BsipGoldConsumeLockInfoReqDef *req);
	BSIP_API_TYPE int SendGoldConsumeUnlockRequestEx(	 BsipGoldConsumeUnlockInfoReqDef *req);
	BSIP_API_TYPE int SendGoldDepositResponseEx(		 BsipGoldDepositInfoResDef *res);
	BSIP_API_TYPE int SendTokenDepositRequestEx(	     BsipTokenDepositInfoReqDef *req);
    BSIP_API_TYPE int SendAccountImportRequestEx(		 BsipAccountImportInfoReqDef *req);
	BSIP_API_TYPE int SendConsignTransferRequestEx(		 BsipConsignTransferInfoReqDef *req);
	BSIP_API_TYPE int SendNormalStatRequestEx(			 BsipNormalStatInfoReqDef *bsipReq );
	BSIP_API_TYPE int SendItemSyncResponsetEx(			 BsipItemSyncInfoResDef *bsipRes);
	BSIP_API_TYPE int SendGsRouteRequestEx(			    BsipGsRouteInfoReqDef *bsipReq);    
#ifdef __cplusplus
}
#endif

#endif
