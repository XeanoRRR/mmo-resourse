#ifndef DB_CARD_H
#define DB_CARD_H
#include "BaseDBEntity.h"
#include <list>
#include "../public/DBDefine.h"

using namespace std;

class CDBCard : public CBaseDBEntity
{
public:

	// ��״̬
	enum eCardState 
	{
		CS_UNKNOWN = 0,			// δ֪��״̬�����߷Ƿ���״̬
		CS_NORMAL,				// ����
		CS_VALIDATED,			// ͨ����֤
		CS_SELLING,				// ������״̬
		CS_RESERVING,			// Ԥ��״̬
		CS_SELLED				// �۳�״̬
	};

	// ���Ĵ���Чʱ�� 
	enum eCardTime 
	{
		CT_0	= 0,
		CT_6	= 1,		// 6Сʱ����λ�����룩
		CT_12,		// 12Сʱ
		CT_24		// 24Сʱ
	};

	CDBCard();
	CDBCard(long id);
	~CDBCard();

	void SetID(long id) { m_ID = id; }
	long GetID(void)    { return m_ID; }

	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1){};
	
	// Get Attrs
	const char*	GetCardNumber(void)		{ return &m_CardNumber[0]; };
	const char*	GetCardType(void)		{ return &m_CardType[0]; };
	const char*	GetSellerAccount(void)	{ return &m_SellerAccount[0]; };
	const CGUID& GetSellerID(void)		{ return m_lSellerID; };
	const char*	GetBuyerAccount(void)	{ return &m_BuyerAccount[0]; };
	const CGUID& GetBuyerID(void)		{ return m_lBuyerID; };
	DWORD		GetPrice(void)			{ return m_dwPrice; };
	DWORD		GetChargePrice(void)	{ return m_dwChargePrice; };
	DWORD		GetCardState(void)		{ return m_dwCardState; };
	DWORD		GetCardTime(void)		{ return m_dwCardTime; };
	DWORD		GetStartTime(void)		{ return m_dwStartTime; };
	DWORD		GetSerialNumber(void)	{ return m_dwSerialNumber; };

	// Set Attrs
	void SetCardNumber(const char* buf, long size)		
	{ 
		memcpy(m_CardNumber, buf, strlen(buf));
	};
	void SetCardType(const char* buf, long size)
	{ 
		memcpy(m_CardType, buf, strlen(buf));
	};
	void SetSellerAccount(const char* buf, long size)	
	{ 
		memcpy(m_SellerAccount, buf, strlen(buf));
	};
	void SetSellerID(const CGUID& ID)		{ m_lSellerID = ID; };
	void SetBuyerAccount(const char* buf, long size)	
	{ 
		memcpy(m_BuyerAccount, buf, strlen(buf));
	};
	void SetBuyerID(const CGUID& ID)			{ m_lBuyerID = ID; };
	void SetPrice(DWORD value)			{ m_dwPrice = value; };
	void SetChargePrice(DWORD value)	{ m_dwChargePrice = value; };
	void SetCardState(DWORD value)		{ m_dwCardState = value; };
	void SetCardTime(DWORD value)		{ m_dwCardTime = value; };
	void SetStartTime(DWORD value)		{ m_dwStartTime = value; };
	void SetSerialNumber(DWORD value)	{ m_dwSerialNumber = value; };

	
private:
	// �ȴ������͸�DBS����������
	vector<long> m_SendAttrEnumVector;
	long    m_ID;
	char	m_CardNumber[64];
	char	m_CardType[64];
	char	m_SellerAccount[64];
	CGUID	m_lSellerID;
	char	m_BuyerAccount[64];
	CGUID	m_lBuyerID;
	DWORD   m_dwPrice;
	DWORD   m_dwChargePrice;
	DWORD   m_dwCardState;
	DWORD   m_dwCardTime;
	DWORD   m_dwStartTime;
	DWORD   m_dwSerialNumber;
};
#endif//DB_CARD_H
