
/*
*	file:		
*	Brief:		
*	detail:		
*	
*				
*	Authtor:	�ſ���
*	Date:		2008-10-16
*/


#include "..\..\..\dbaccess\CnPool.h"

#pragma once




class ValidateOperator
{
public:
	ValidateOperator(tagAccountInfo_ToAS &AccountInfo_ToAS, DWORD dwSocket, DWORD dwLSID);
	virtual ~ValidateOperator(VOID);

public:
	static		BOOL				Init(LONG lASID);
	static		VOID				Release(VOID);
	static		ValidateOperator*	CreateValidateOperator(tagAccountInfo_ToAS &AccountInfo_ToAS, DWORD dwSocket, DWORD dwLSID);
	static		VOID				ReleasValidateOperator(ValidateOperator **ppRVO);

	static		CnPool*				GetCnPool(VOID){return c_pCnPool;}
	static		CnPool*				GetSecurityCnPool(VOID){return c_pSecurityPool;}
#ifdef ENABLE_FCM
	static		CnPool*				GetFcmCnPool(VOID){return c_pFcmCnPool;}
#endif
	
public:
	//!								��ʼִ��
	BOOL							BeginValidate(VOID);
	//!								���Ͳ������
	VOID							SendResult(VOID);
	//!								���ʹ���ȫ���ݵĲ������
	VOID							SendResult(tagSecurityData &SecurityData);

private:
	//!								�ύ�����ݿ�����֤�˺ŵ�����
	static		LPVOID				ValidateAccountFromDB(LPVOID pThis);	
	//!								������֤���
	static		LPVOID				OnValidateFinish(LPVOID pThis);

private:
	//!															�˺����ݿ�����
	static		CnPool											*c_pCnPool;
	//!															��ȫ���ݿ�����
	static		CnPool											*c_pSecurityPool;
	//!															���������ݿ�����
	static		CnPool											*c_pFcmCnPool;

	static		LONG											c_lASID;

	///////////////////////////////////////////////////////////////////
#ifdef APEX_TEST

public:
	static	DWORD					GetValidateNum(VOID){return c_lValidateNum;}

private:
	static	LONG					c_lValidateNum;

#endif
	///////////////////////////////////////////////////////////////////


private:
	tagAccountInfo_FromAS			m_AccountInfo_FromAS;
	DWORD							m_dwSocket;
	DWORD							m_dwLSID;



	

};