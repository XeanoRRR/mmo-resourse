#include "StdAfx.h"
#include ".\rscards.h"
#include "..\..\server\dbserver\appworld\dbcard.h"
#include "..\..\server\dbserver\appworld\dbentitymanager.h"
#include "..\..\server\DBServer/dbserver/game.h"
CRSCards g_RSCards;

CRSCards::CRSCards(void)
{
}

CRSCards::~CRSCards(void)
{
}
bool CRSCards::SaveAll(void)
{
	return true;
}
bool CRSCards::Load(CDBCard* card)
{
	return true;
}
bool CRSCards::LoadAllCards()
{
	_ConnectionPtr cn;
	_RecordsetPtr rs;

	char sql[256];

	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��
	if( !CreateRs(rs) ) return false;

	try
	{
		TESTHR( OpenCn(cn) ); // ��Ŀ�����ݿ�ʧ��
		
		// ���˿��Ƿ�������ݿ��С�
		sprintf( sql,"SELECT * FROM Csl_Card_Dealing"  );

		TESTHR( OpenRs(sql, rs, cn, adOpenForwardOnly, adLockOptimistic, adCmdTable) );

		while( !rs->GetadoEOF() )
		{
			CDBCard *pCard = new CDBCard;

			if( !pCard) 
			{
				ReleaseRs(rs);
				//ReleaseCn(cn);
				return false;
			}

			char* ptr = (char*)(_bstr_t)rs->GetCollect("CardNumber");
			pCard->SetCardNumber( ptr, strlen(ptr) );

			ptr = (char*)(_bstr_t)rs->GetCollect("CardType");
			pCard->SetCardType( ptr, strlen(ptr) );

			ptr = (char*)(_bstr_t)rs->GetCollect("SellerAccount");
			pCard->SetSellerAccount( ptr, strlen(ptr) );

			pCard->SetSellerID( CGUID(((char*)(_bstr_t)rs->GetCollect("SellerID"))) );

			ptr = (char*)(_bstr_t)rs->GetCollect("BuyerAccount");
			pCard->SetBuyerAccount( ptr, strlen(ptr) );

			pCard->SetBuyerID( CGUID(((char*)(_bstr_t)rs->GetCollect("BuyerID"))) );
			pCard->SetPrice( rs->GetCollect("Price") );
			pCard->SetChargePrice( rs->GetCollect("ChargePrice") );
			pCard->SetCardState( (CDBCard::eCardState)(long)rs->GetCollect("CardState") );
			pCard->SetCardTime( (CDBCard::eCardTime)(long)rs->GetCollect("CardTime") );
			pCard->SetStartTime( rs->GetCollect("StartTime") );
			pCard->SetSerialNumber( rs->GetCollect("SerialNumber") );

			rs->MoveNext();
		}

		rs->Close();
		cn->Close();
		ReleaseRs(rs);
		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{

		ReleaseRs(rs);
		//ReleaseCn(cn);
		PrintErr("LoadAllCards����ʧ��", e);
	}
	return false;
}

bool CRSCards::AddNew(CDBCard *card) 
{
	if(!card) return false;

	_ConnectionPtr cn;
	_RecordsetPtr rs;

	long nTransactionCounter = 0;

	char sql[256];

	
	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��
	if( !CreateRs(rs) ) return false;

	try
	{
		TESTHR( OpenCn(cn) ); // ��Ŀ�����ݿ�ʧ��
		
		// ���˿��Ƿ�������ݿ��С�
		sprintf( sql,"SELECT CardNumber FROM Csl_Card_Dealing WHERE CardNumber='%s'", card->GetCardNumber() );

		TESTHR( OpenRs(sql, rs, cn, adOpenForwardOnly, adLockOptimistic, adCmdTable) );

		if( !rs->GetadoEOF() )
		{
			// ����ʧ��
			ReleaseRs(rs);
			//ReleaseCn(cn);
			return false;
		}

		nTransactionCounter = cn->BeginTrans();	// ����ʼ
		// ��ӵ����ݿ�
		sprintf( sql,"INSERT INTO Csl_Card_Dealing VALUES('%s','%s',%d,'%s',0,'',%d,%d,%d,%d,%d,%d)",
			card->GetCardNumber(),
			card->GetCardType(),
			card->GetSellerID(),
			card->GetSellerAccount(),
			//0, // BuyerID ��û�С���
			card->GetPrice(),
			card->GetChargePrice(),
			(long)card->GetCardState(),
			(long)card->GetCardTime(),
			(long)card->GetStartTime(),
			(long)card->GetSerialNumber());

		TESTHR( ExecuteCn(sql, cn) );

		cn->CommitTrans();
		rs->Close();
		cn->Close();
		ReleaseRs(rs);
		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{
		if(nTransactionCounter)
		{
			cn->RollbackTrans(); //ʧ�ܻع�
		}
		ReleaseRs(rs);
		//ReleaseCn(cn);
		PrintErr("AddNew����ʧ��", e);
	}
	return false;
}
bool CRSCards::Delete(CDBCard *card)
{
	if(!card) return false;

	_ConnectionPtr cn;
	char sql[256];
	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��
	try
	{
		TESTHR( OpenCn(cn) ); // ��Ŀ�����ݿ�ʧ��

		// ɾ�������ݿ�
		sprintf( sql,"DELETE FROM Csl_Card_Dealing WHERE CardNumber='%s'", card->GetCardNumber() );

		TESTHR( ExecuteCn(sql, cn) );
		cn->Close();

		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{
		//ReleaseCn(cn);
		PrintErr("Delete����ʧ��", e);
	}
	return false;
}

bool CRSCards::UpdateSell(CDBCard *card)
{
	if(!card) return false;

	_ConnectionPtr cn;
	_RecordsetPtr rs;

	long nTransactionCounter = 0;

	char sql[256];

	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��
	if( !CreateRs(rs) ) return false;

	try
	{
		TESTHR( OpenCn(cn) ) ; // ��Ŀ�����ݿ�ʧ��
		
		// ���˿��Ƿ�������ݿ��С�
		sprintf( sql,"SELECT * FROM Csl_Card_Dealing WHERE CardNumber='%s'", card->GetCardNumber() );

		TESTHR( OpenRs(sql, rs ,cn, adOpenForwardOnly,adLockOptimistic,adCmdText) );
		if( rs->GetadoEOF() )
		{
			ReleaseRs(rs);
			//ReleaseCn(cn);
			return false;
		}
		nTransactionCounter = cn->BeginTrans();
		rs->PutCollect("CardState",(long)card->GetCardState());
		rs->PutCollect("StartTime",(long)card->GetStartTime());
		rs->PutCollect("SerialNumber", (long)card->GetSerialNumber());
		rs->Update();
		cn->CommitTrans();

		rs->Close();
		cn->Close();
		ReleaseRs(rs);
		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{
		if(nTransactionCounter)
		{
			cn->RollbackTrans(); //ʧ�ܻع�
		}
		ReleaseRs(rs);
		//ReleaseCn(cn);
		PrintErr("UpdateSell����ʧ��", e);
	}
	return false;
}


bool CRSCards::UpdateReserve(CDBCard *card)
{
	if(!card) return false;

	_ConnectionPtr cn;
	_RecordsetPtr rs;

	long nTransactionCounter = 0;

	char sql[256];

	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��
	if( !CreateRs(rs) ) return false;

	try
	{
		TESTHR( OpenCn(cn) ); // ��Ŀ�����ݿ�ʧ��
		
		// ���˿��Ƿ�������ݿ��С�
		sprintf( sql,"SELECT * FROM Csl_Card_Dealing WHERE CardNumber='%s'", card->GetCardNumber() );

		TESTHR( OpenRs(sql, rs ,cn, adOpenForwardOnly,adLockOptimistic,adCmdText) );


		if( rs->GetadoEOF() ) 
		{
			// ����ʧ��
			ReleaseRs(rs);
			//ReleaseCn(cn);
			return false;
		}

		nTransactionCounter = cn->BeginTrans();

		char szBuyerID[128];
		card->GetBuyerID().tostring(szBuyerID);
		rs->PutCollect("CardState",(long)card->GetCardState());
		rs->PutCollect("BuyerID",szBuyerID);
		rs->PutCollect("BuyerAccount",card->GetBuyerAccount());

		rs->Update();
		
		cn->CommitTrans();

		rs->Close();
		cn->Close();
		ReleaseRs(rs);
		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{
		if(nTransactionCounter)
		{
			cn->RollbackTrans(); //ʧ�ܻع�
		}
		ReleaseRs(rs);
		//ReleaseCn(cn);
		PrintErr("UpdateSell����ʧ��", e);
	}
	return false;
}

bool CRSCards::SaveLargess(CDBCard *card)
{
	if(!card) return false;

	_ConnectionPtr cn;


	char sql[256];

	if( !CreateCn(cn) ) return false; // �������Ӷ���ʧ��

	try
	{
		TESTHR( OpenCn(cn) ); // ��Ŀ�����ݿ�ʧ��

		sprintf( sql,"INSERT INTO Csl_Card_Largess(Cdkey,SendNum,ObtainedNum) VALUES('%s',%d,%d)",
			card->GetSellerAccount(), card->GetPrice(),0 );

		TESTHR( ExecuteCn(sql, cn) );
		cn->Close();
		//ReleaseCn(cn);
		return true;
	}
	catch(_com_error &e)
	{
		//ReleaseCn(cn);
		PrintErr("SaveLargess����ʧ��", e);
	}
	return false;
}