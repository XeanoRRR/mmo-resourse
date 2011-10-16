#ifndef DB_CARD_H
#define DB_CARD_H
#include "BaseDBEntity.h"
#include <vector>
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

	typedef std::map<std::string, DB_CARD_ATTR_TYPE> AttrNameToEnumMap;
	typedef AttrNameToEnumMap::iterator AttrNameToEnumMapItr;

	CDBCard();
	CDBCard(const CGUID& id);
	~CDBCard();

	void SetID(long id) { m_ID = id; }
	long GetID(void)    { return m_ID; }
	// virtual interfaces
	virtual void ProcessMsg(DB_OPERATION_TYPE op, long msgid, BYTE* msgBuf, long& msgBufPos, long bufSize=-1);
	void Load(long msgid, BYTE* msgBuf, long& msgBufPos);
	void Save(long msgid, BYTE* msgBuf, long& msgBufPos);
	
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
		memcpy_s(m_CardNumber, size, buf, sizeof(m_CardNumber));
	};
	void SetCardType(const char* buf, long size)
	{ 
		memcpy_s(m_CardType, size, buf, sizeof(m_CardType));
	};
	void SetSellerAccount(const char* buf, long size)	
	{ 
		memcpy_s(m_SellerAccount, size, buf, sizeof(m_SellerAccount));
	};
	void SetSellerID(const CGUID& ID)		{ m_lSellerID = ID; };
	void SetBuyerAccount(const char* buf, long size)	
	{ 
		memcpy_s(m_BuyerAccount, size, buf, sizeof(m_BuyerAccount));
	};
	void SetBuyerID(const CGUID& ID)			{ m_lBuyerID = ID; };
	void SetPrice(DWORD value)			{ m_dwPrice = value; };
	void SetChargePrice(DWORD value)	{ m_dwChargePrice = value; };
	void SetCardState(DWORD value)		{ m_dwCardState = value; };
	void SetCardTime(DWORD value)		{ m_dwCardTime = value; };
	void SetStartTime(DWORD value)		{ m_dwStartTime = value; };
	void SetSerialNumber(DWORD value)	{ m_dwSerialNumber = value; };

	// Other interfaces
	// ׼�����͵�����
	//vector<DB_CARD_ATTR_TYPE>& GetSendAttrEnumVector(void) { return m_SendAttrEnumVector; }
	// ��׼�����͵�������Vector�е���Ӧλ�õı�־
	void OpenAttrFlagInVector(DB_CARD_ATTR_TYPE attrType);
	// �ر�׼�����͵�������Vector�е���Ӧλ�õı�־
	void CloseAttrFlagInVector(DB_CARD_ATTR_TYPE attrType);
	// ��װ��Ϣ
	void AddEntityDataToMsg(DB_OPERATION_TYPE opType, vector<BYTE>& pBA);

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
#endif//_DB_CARD_H
