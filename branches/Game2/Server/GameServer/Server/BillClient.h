//-------------------------------------------------------------------------------
// ͳһ�ƷѻỰ. by Fox.		2008-01-23
// ���� GS2BS ��һ��������Ӧ���첽�ص�
//-------------------------------------------------------------------------------

#pragma once

#include "Game.h"
#include "../App/bsinclude/bsip/bsipfun.h"
#include "../App/bsinclude/bsip/bsipfunbs.h"

class CCallbackAccountAuthen;
class CCallbackAccountProcess;
class CCallbackAwardAck;
class CCallbackAwardAuthen;
class CCallbackConsignTransfer;

enum eBillItem
{
	BI_NONBACK = -1,														// �޻ص�����
	BI_NULL,																// ��Ч����
	BI_ACCOUNTAUTHEN,														// �ʺŲ�ѯ
	BI_LOCK,																// �ʺ�Ԥ��
	BI_UNLOCK,																// �ʺŽⶳ
	BI_TRANSFER,															// �ʺ�ת��
	BI_AWARDAUTHEN,															// �콱��ѯ
	BI_AWARDACK,															// �콱ȷ��
};

enum eBillType
{
	BT_NULL = 0,
	BT_AT_PATCH,															// ���ʺŰ���콱
	BT_AT_NUMBER,															// ���콱�Ű󶨵��콱
	BT_AT_NOBIND,															// �����ʺš��콱�Ű󶨵��콱
	BT_AT_GETYB,															// ���߻�Ԫ��
	BT_AT_LEVEL,															// �����ʺ�

	BT_INCSHOP		= 17,														// ��ֵ�̵�	
	BT_TRADER		= 20,														// ���˽���
	BT_PERSHOP		= 55,														// �����̵�
	BT_INCPRESCET   = 105,														// ��ֵ��������
};

class CBillClient
{
public:
	CBillClient();
	~CBillClient();

//-------------------------------------------------------------------------------
// Encapsulate session
//-------------------------------------------------------------------------------
public:
	void Run();
	void Release();															// �ͷ���Դ

 public:
	struct tagAccAuthenReq
	{
		char szPlayerID[MAX_PTID_LEN+1];									// �˺�id
		char szSessionID[MAX_SESSID_LEN+1];									// �Ựid
		char szPlayerIP[IP_ADDRESS_LEN+1];									// �ն�IP��ַ
	};

	struct tagAccLockReq
	{
		eBillType billType;													// ��������
		CGUID guidSessionID;												// ���׻ỰID
		long lPlugID;														// ����Plug ID
		long lTotalYuanbao;													// �ܽ���Ԫ������
		long lTotalMoney;													// �ܽ��׽�Ǯ����
		long lGoodsID;														// ��Ϸ���߱��
		long lGoodsNum;														// ��Ϸ��������
		char szBuyerID[MAX_PTID_LEN+1];										// ����˺�id
		char szSellerID[MAX_PTID_LEN+1];									// �����˺�id
		char szSessionID[MAX_SESSID_LEN+1];									// �Ựid
		char szBuyerIP[IP_ADDRESS_LEN+1];									// ����ն�IP��ַ

		tagAccLockReq()
			:billType(BT_NULL)
			,lPlugID(0)
			,lTotalYuanbao(0)
			,lTotalMoney(0)
			,lGoodsID(0)
			,lGoodsNum(0)
		{
			strcpy_s(szBuyerID, MAX_PTID_LEN+1, "");
			strcpy_s(szSellerID, MAX_PTID_LEN+1, "");
			strcpy_s(szSessionID, MAX_SESSID_LEN+1, "");
			strcpy_s(szBuyerIP, IP_ADDRESS_LEN+1, "");
		}
		tagAccLockReq(const tagAccLockReq &req)
			:billType(req.billType)
			,lPlugID(req.lPlugID)
			,lTotalYuanbao(req.lTotalYuanbao)
			,lTotalMoney(req.lTotalMoney)
			,lGoodsID(req.lGoodsID)
			,lGoodsNum(req.lGoodsNum)
		{
			guidSessionID = req.guidSessionID;
			strcpy_s(szBuyerID, MAX_PTID_LEN+1, req.szBuyerID);
			strcpy_s(szSellerID, MAX_PTID_LEN+1, req.szSellerID);
			strcpy_s(szSessionID, MAX_SESSID_LEN+1, req.szSessionID);
			strcpy_s(szBuyerIP, IP_ADDRESS_LEN+1, req.szBuyerIP);
		}
	};

	struct tagAccUnlockReq
	{
		eBillType billType;													// ��������
		bool bConfirm;														// ���׳ɹ�Ϊtrue��ʧ��Ϊfalse
		long lTotalYuanbao;													// �ܽ��׶�
		long lGoodsID;														// ��Ϸ���߱��
		long lGoodsNum;														// ��Ϸ��������
		char szContextID[MAX_ORDERID_LEN+1];								// ����id
		char szBuyerID[MAX_PTID_LEN+1];										// ����˺�id
		char szSellerID[MAX_PTID_LEN+1];									// �����˺�id
		char szSessionID[MAX_SESSID_LEN+1];									// �Ựid
		char szBuyerIP[IP_ADDRESS_LEN+1];									// ����ն�IP��ַ

		tagAccUnlockReq()
			:billType(BT_NULL)
			,bConfirm(false)
			,lTotalYuanbao(0)
			,lGoodsID(0)
			,lGoodsNum(0)
		{
			strcpy_s(szContextID, MAX_ORDERID_LEN+1, "");
			strcpy_s(szBuyerID, MAX_PTID_LEN+1, "");
			strcpy_s(szSellerID, MAX_PTID_LEN+1, "");
			strcpy_s(szSessionID, MAX_SESSID_LEN+1, "");
			strcpy_s(szBuyerIP, IP_ADDRESS_LEN+1, "");
		}
		tagAccUnlockReq(const tagAccUnlockReq &req)
			:billType(req.billType)
			,bConfirm(req.bConfirm)
			,lTotalYuanbao(req.lTotalYuanbao)
			,lGoodsID(req.lGoodsID)
			,lGoodsNum(req.lGoodsNum)
		{
			strcpy_s(szContextID, MAX_ORDERID_LEN+1, req.szContextID);
			strcpy_s(szBuyerID, MAX_PTID_LEN+1, req.szBuyerID);
			strcpy_s(szSellerID, MAX_PTID_LEN+1, req.szSellerID);
			strcpy_s(szSessionID, MAX_SESSID_LEN+1, req.szSessionID);
			strcpy_s(szBuyerIP, IP_ADDRESS_LEN+1, req.szBuyerIP);
		}
	};

	struct tagAwdAuthenReq
	{
		eBillType billType;													// ��������
		char szPlayerID[MAX_PTID_LEN+1];									// �˺�id
		char szPlayerName[32+1];											// ��ɫ��
		char szSessionID[MAX_SESSID_LEN+1];									// �Ựid
		char szPlayerIP[IP_ADDRESS_LEN+1];									// �ն�IP��ַ
		char szAwardPatchID[MAX_AWARDPATCH_LEN+1];							// �콱���κ�
		char szAwardNum[MAX_AWARD_LEN+1];									// �콱����
	};

	struct tagAwdAckReq
	{
		eBillType billType;													// ��������
		bool bConfirm;														// �콱�ɹ�Ϊtrue��ʧ��Ϊfalse
		char szContextID[MAX_ORDERID_LEN+1];								// ����id
		char szPlayerID[MAX_PTID_LEN+1];									// �˺�id
		char szSessionID[MAX_SESSID_LEN+1];									// �Ựid
		char szPlayerIP[IP_ADDRESS_LEN+1];									// �ն�IP��ַ
		char szAwardNum[MAX_AWARD_LEN+1];									// �콱����
	};

private:
	struct tagBillItem
	{
		eBillItem	eItemType;
		void		*pItem;
	};

	typedef MulTiThMap<string, tagBillItem *>			mapBill;
	typedef MulTiThMap<string, tagBillItem *>::iterator itrBill;
	static mapBill										m_CallerQueue;		// ���ö���(�ص������)
	static mapBill										m_CallbackQueue;	// �ص�����(���ص������ʹ�õĻص����)

public:
	void SendAccountAuthen( tagAccAuthenReq *pReq, CCallbackAccountAuthen *pCallback );
	void SendAccountLock( tagAccLockReq *pReq, CCallbackAccountProcess *pCallback );
	void SendAccountUnlock( tagAccUnlockReq *pReq, CCallbackAccountProcess *pCallback );
	void SendConsignTransfer( tagAccUnlockReq *pReq, CCallbackAccountProcess *pCallback );
	void SendAwardAuthen( tagAwdAuthenReq *pReq, CCallbackAwardAuthen *pCallback );
	void SendAwardAck( tagAwdAckReq *pReq, CCallbackAwardAck *pCallback );

//-------------------------------------------------------------------------------
// Logic function
// caller functions & callback functions used by BillClient & BillServer(Dll)
//-------------------------------------------------------------------------------
private:
	typedef int (*FunGSInitialize)( GSCallbackFuncDef *callbackFun, const char *configfilename, int type );
	typedef int (*FunUninitialize)( int type );	
	typedef int (*FunSendAccountAuthenRequest)( const GSBsipAccountAuthenReqDef *req );
	typedef int (*FunSendAccountLockRequest)( const GSBsipAccountLockReqDef *req );
	typedef int (*FunSendAccountUnlockRequest)( const GSBsipAccountUnlockReqDef *req );
	typedef int (*FunSendConsignTransferRequest)( const GSBsipConsignTransferReqDef *req );
	typedef int (*FunSendAwardAuthenRequest)( const GSBsipAwardAuthenReqDef *req );
	typedef int (*FunSendAwardAck)( const GSBsipAwardAckDef *req );
	typedef int (*FunGetNewId)(char *id);
	typedef int (*FunGetNewIdByParam)(int intareaId,int intGroupId, char *id);
	typedef int (*FunGetId)(int spId, int servId, int areaId, int groupId, char *id);

	GSCallbackFuncDef m_CallbackFun;

public:
	FunGSInitialize					funGSInitialize;
	FunUninitialize					funUninitialize;
	FunGetNewId						funGetNewId;
	FunGetNewIdByParam				funGetNewIdByParam;
	FunSendAccountAuthenRequest		funSendAccountAuthen;
	FunSendAccountLockRequest		funSendAccountLock;
	FunSendAccountUnlockRequest		funSendAccountUnlock;
	FunSendConsignTransferRequest	funSendConsignTransfer;
	FunSendAwardAuthenRequest		funSendAwardAuthen;
	FunSendAwardAck					funSendAwardAck;

	static void		CallbackAccountAuthen(GSBsipAccountAuthenResDef* );
	static void		CallbackAccountLock(GSBsipAccountLockResDef* );
	static void		CallbackAccountUnlock(GSBsipAccountUnlockResDef* );
	static void		CallbackConsignTransfer(GSBsipConsignTransferResDef* );
	static void		CallbackAwardAuthen(GSBsipAwardAuthenResDef* );
	static void		CallbackAwardAck(GSBsipAwardAckResDef* );


	GSCallbackFuncDef*	GetCallbacks() { return &m_CallbackFun; }
	void			GetCurrentTimeString(char* str, size_t bufsize);
	int				BsipGetFunction(HINSTANCE hDll);
	int				BsipSetCallBackFunction();

//-------------------------------------------------------------------------------
// Singleton
//-------------------------------------------------------------------------------
private:
	static CBillClient *s_pInstance;

public:
	static CBillClient *GetInstance();
};

CBillClient *GetBillClient();