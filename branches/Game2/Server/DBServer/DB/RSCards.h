#pragma once
#include "..\myadobase.h"
#include "..\..\server\dbserver\appworld\DBcard.h"

class CMessage;

class CRSCards :
	public CMyAdoBase
{
public:
	CRSCards(void);
	~CRSCards(void);
public:
	bool LoadAllCards();
	bool AddNew(CDBCard *card);			// ���һ���µļ��ۿ�
	bool Delete(CDBCard *card);			// ɾ��
	bool UpdateSell(CDBCard *card);		// ���³ɼ���״̬
	bool UpdateReserve(CDBCard *card);	// ���³ɼ���״̬
	bool SaveLargess(CDBCard *card);	

	bool Load(CDBCard* card);
	bool SaveAll(void);
};

extern CRSCards g_RSCards;