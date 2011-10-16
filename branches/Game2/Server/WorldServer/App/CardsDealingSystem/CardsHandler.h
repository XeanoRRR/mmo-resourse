//
// �����ࣺ��ʵ����
// ��Ҫ����
//
// ���ﲻ�ô��̲�������Ϊ��CCard����ἰʱ���д��̲�����
#pragma once
#include "Card.h"

class CCardsHandler
{
public:
	CCardsHandler(void);
	virtual ~CCardsHandler(void);

	bool Initialize();						// ���룬��ʼ
	bool Uninitialize();

private:
	static CCardsHandler *instance;			//����Ψһʵ�����

//	CDBCards _DBCards;

public:
	static CCardsHandler* getInstance();	//�õ�Ψһʵ�����
	static void delInstance();				//ɾ��Ψһʵ�����

	int InsertCard(CCard *card);
	int RemoveCard(CCard *card);


	CCard* QueryCardByCardNumber(const char *cardnumber);

	//bool QueryCardByPlayer(long playerid, vector<CCard*> &cards);
	//bool QueryCardByPlayer(const char *accmout, vector<CCard*> &cards);


	DWORD QueryChargePriceByTimeType( CCard::eCardTime cardtime);
	DWORD QueryTimeByTimeType( CCard::eCardTime cardtime);

	size_t getCardsNumByAccount(const char* account);
	size_t getCardsNum()	{return _Cards.size();}	// ��ǰ���ۿ�������

	DWORD GetSellingCardsNum();

	bool DBAddNew(CCard *card);				// ����һ��ͨ����֤���¿������ݿ�
	bool DBDelete(CCard *card);
	//bool DBUpdateSell(CCard *card);		//
	bool DBUpdateReserve(CCard *card);		//
	bool DBLoad();							// ����������ʱ�������ݿ�������

	bool DBSaveLargess(CCard *card);		// ���׳ɹ���д���������ݿ⣬���ҵ�½ʱ����ȡ��Ǯ

	void CardsLoop();						// ����ﵽ����ʱ�䣬���ۿ�������


	void CancelAllCards();
public:
	map<string,CCard*> _Cards;				// �����еĿ� <����,CCard>

public:
	struct DenyInfo
	{
		BYTE	times;						// ����
		DWORD	last_time;					// ������ʱ��
	};

	map<string,DenyInfo*> _DenyCards;		// ��ʱ�޷����۵���� 
	map<string,DenyInfo*> _DenyAccounts;	// ��ʱ�޷����۵���� 

	int Check(CCard* card);					// �����������ص���������������м�顣
	void AddDenys(CCard *card);
};
