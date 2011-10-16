#pragma once
// pcallbackfunc.h: interface for the pcallbackfunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCALLBACKFUNC_H__13634D3E_8C35_4F1D_9A79_E0F08BBE8050__INCLUDED_)
#define AFX_PCALLBACKFUNC_H__13634D3E_8C35_4F1D_9A79_E0F08BBE8050__INCLUDED_

#include "bsipmsg.h"
/*********************************************************
declare call back function 
when response message arrived these function will be called
**********************************************************/

//��������֤��Ӧ
void PCallbackAccountAuthenRes(GSBsipAccountAuthenResDef *p);		
//����ʻ�Ԥ����չ��Ӧ
void PCallbackAccountLockExRes(GSBsipAccountLockExResDef *p);  
//����ʻ��ⶳ��չ��Ӧ
void PCallbackAccountUnlockExRes(GSBsipAccountUnlockExResDef *p);	
void PCallbackAwardAuthenRes(GSBsipAwardAuthenResDef *p);
void PCallbackAwardAckRes(GSBsipAwardAckResDef *p);
void PCallbackTokenDepositRes(GSBsipTokenDepositResDef *p);


#endif // !defined(AFX_PCALLBACKFUNC_H__13634D3E_8C35_4F1D_9A79_E0F08BBE8050__INCLUDED_)
