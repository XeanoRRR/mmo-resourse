//
// ���ࣺ
// ���Ļ������� �����������Ժ�״̬
// ��������������ȡ��
//
#pragma once

class CCard
{
	friend class CCardsHandler;  // Declare a friend class. CCardsHandler��������ڹ���CCard��
								 // CCardsHandler �� CCard ��һ�����
public:
	//// ������
	//enum eCardType {
	//	CT_UNKNOWN = 0,			// δ֪������
	//	CT_10,					// 10Ԫ��
	//	CT_30,					// 30Ԫ��
	//	CT_MONTH,				// ���¿�������δ���ŵģ�
	//	CT_MOBILE				// �ֻ���������δ���ŵģ�
	//};

	// ��״̬
	enum eCardState {
		CS_UNKNOWN = 0,			// δ֪��״̬�����߷Ƿ���״̬
		CS_NORMAL,				// ����
		CS_VALIDATED,			// ͨ����֤
		CS_SELLING,				// ������״̬
		CS_RESERVING,			// Ԥ��״̬
		CS_SELLED				// �۳�״̬
	};

	// ���Ĵ���Чʱ�� 
	enum eCardTime {
		CT_0	= 0,
		CT_6	= 1,		// 6Сʱ����λ�����룩
		CT_12,		        // 12Сʱ
		CT_24		        // 24Сʱ
	};

public:
	CCard(void);
	virtual ~CCard(void);

private:
	DWORD _SerialNumber;		// ���к��ַ��������ڿ�ʼ����ʱ���ԸĿ������ĸ��١�GUID
	string _CardNumber;			// ������󳤶Ȳ��ᳬ��12λ��������12λ�������֣��Ժ���ܳ����ַ�
	string _CardPassword;		// ������󳤶Ȳ��ᳬ��10λ


	string _CardType;		// ������ �Ʒѷ�����ͨ�����Ž���ʶ��
	eCardState _CardState;		// ��״̬
	eCardTime _CardTime;		// ��Ч����ʱ��

	// �ڼ��ۿ��͹���ʱ������δ����ǰ�������˺����˶��������ߡ�
	// ������ߣ��ò�������ʧ�ܡ����ۻ򹺿���ȡ����
	
	CGUID _SellerID;				// ������
	string _SellerAccount;
	
	CGUID _BuyerID;				// ����
	string _BuyerAccount;
	

	
	DWORD	_StartTime;			// ���Ŀ�ʼ����ʱ�� ֻ���ڼ���״̬�²��㿪ʼ�����ۼ�ʱ �ٹ���Чʱ�佫ȡ������
	//time_t	_TimeStamp;			// ���ʱ�����ڼ�������Ƿ�ʱ�ģ������Ϊ�����ӳٳ�ʱ���ͻ�ȡ������

	// ������С�����������ȫ�������ļ������� ÿ�ν��׽��Ӧ�ڸ÷�Χ����
	// ������Χ�������轻��
	unsigned int _Price;		// �˿������ۼ�ֵ����Ϸ�ң�
	unsigned int _ChargePrice;	// ������


public:
	void SetSerialNumber(DWORD serialnumber) { _SerialNumber = serialnumber;}	// ��֤ͨ���󣬴ӼƷѷ��������
	DWORD GetSerialNumber() {return _SerialNumber;}


	void SetCardNumber(const char *cardnumber) {_CardNumber = cardnumber;}
	const char *GetCardNumber() {return _CardNumber.data();}

	void SetCardType(const char *cardtype) {_CardType = cardtype;} // ��֤ͨ���󣬴ӼƷѷ��������
	const char *GetCardType() {return _CardType.data();}

	void SetCardState(eCardState cardstate) {_CardState = cardstate;}
	eCardState GetCardState() {return _CardState;}

	void SetCardTime(eCardTime cardtime) {_CardTime = cardtime;}
	eCardTime GetCardTime() {return _CardTime;}

	void SetSellerID(const CGUID& playerid) {_SellerID = playerid;}
	const CGUID& GetSellerID() {return _SellerID;}

	void SetSellerAccount(const char *acc) {_SellerAccount = acc;}
	const char *GetSellerAccount() {return _SellerAccount.data();}

	void SetBuyerID(const CGUID&  playerid) {_BuyerID = playerid;}
	const CGUID& GetBuyerID() {return _BuyerID;}

	void SetBuyerAccount(const char *acc) {_BuyerAccount = acc;}
	const char *GetBuyerAccount() {return _BuyerAccount.data();}

	void SetPrice(DWORD price) {_Price = price;}
	DWORD GetPrice() {return _Price;}

	void SetChargePrice(DWORD chargeprice) {_ChargePrice = chargeprice;}
	DWORD GetChargePrice() {return _ChargePrice;}

	void SetStartTime(DWORD starttime) {_StartTime = starttime;}
	DWORD GetStartTime() {return _StartTime;}
	
public:


	//bool Validate(const char *cardnumber,
	//	const char *cardpassword,
	//	DWORD sellprice,
	//	BYTE timetype,
	//	LONG playerid);			// Ч�鿨�����Ϳ��ź�����
	//							// ���ؿ����ͣ� ��״̬�� �Ƿ��ǷǷ�������Ϣ

	//bool Sell();				// �������� ״̬������->����
	//bool Reserve(long BuyerID);				// ����Ԥ�� ״̬������->Ԥ��
	//bool Deal();				// �������� ״̬��Ԥ��->�۳�
	//bool Cancel();				// ����ȡ�� ״̬������->����

};
