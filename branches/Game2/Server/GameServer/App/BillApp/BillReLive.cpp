#include "StdAfx.h"
#include "BillReLive.h"
#include "../Player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool IncShopProcessing( CPlayer* pPlayer, DWORD dwGoodsIndex, DWORD dwNum, DWORD dwTradeMode, long lTotalAmount );
extern bool PerTradeProcessing(const CGUID &SessionID, long lPlugID );
extern bool PerShopProcessing(const CGUID &SessionID, long lPlugID, long lTotalMoney, long lTotalYuanbao );

// �ʻ�Ԥ��
CBillReLive::CBillReLive(const CGUID &guidBuyer, const CGUID &guidSeller, const DWORD dwIP, const CBillClient::tagAccLockReq &reqLock)
: m_guidBuyer(guidBuyer)
, m_guidSeller(guidSeller)
, m_dwIP(dwIP)
, m_tagAccLockReq(reqLock)
{}

CBillReLive::~CBillReLive(void)
{}

void CBillReLive::OnTimeOut(const string &strOrderID)
{
	char szBuyerName[128] = "";
	char szBuyerAccount[128] = "";
	CPlayer *pBuyer = GetGame()->FindPlayer(m_guidBuyer);
	if ( pBuyer )
	{
		strcpy_s(szBuyerName, 128, pBuyer->GetName());
		strcpy_s(szBuyerAccount, 128, pBuyer->GetAccount());
	}
	Log4c::Warn(ROOT_MODULE, "Req OI:%s, CI:0, TM:%d : Buyer %s(PT:%s) AccountLockRes �쳣��ʱ!", 
		strOrderID.c_str(), m_tagAccLockReq.billType, szBuyerName, szBuyerAccount );
}

void CBillReLive::OnCallback(const string strOrderID, const GSBsipAccountLockResDef *pRes)
{
	char strLog[512] = "";
	CPlayer *pBuyer = GetGame()->FindPlayer(m_guidBuyer);

	if( !pRes )
	{
		_snprintf(strLog, 512, "Req OI:%s, CI:0, TM:%d : AccountLockRes �쳣ΪNULL!", 
			strOrderID.c_str(), m_tagAccLockReq.billType);
	}
	else if( !pBuyer )
	{
		_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d : AccountLockRes Buyer�������ڴ�GS!", 
			pRes->order_id, pRes->context_id, m_tagAccLockReq.billType);
	}
	else
	{
		switch( pRes->result )
		{
		case 0:			// Ԥ���ɹ�
			{
				CBillClient::tagAccUnlockReq req;
				req.billType			= m_tagAccLockReq.billType;			// ��������
				switch( req.billType )
				{
				case BT_INCSHOP:											// ��ֵ�̵깺��
					{
						req.bConfirm	= IncShopProcessing( pBuyer, m_tagAccLockReq.lGoodsID,
							m_tagAccLockReq.lGoodsNum, CIncrementShopList::TM_YUANBAO, m_tagAccLockReq.lTotalYuanbao );
					}
					break;
				case BT_TRADER:												// ���˽��׹���
					{
						req.bConfirm	= PerTradeProcessing( m_tagAccLockReq.guidSessionID,
							m_tagAccLockReq.lPlugID );
					}
					break;
				case BT_PERSHOP:											// �����̵깺��
					{
						req.bConfirm	= PerShopProcessing( m_tagAccLockReq.guidSessionID,  
							m_tagAccLockReq.lPlugID, m_tagAccLockReq.lTotalMoney, m_tagAccLockReq.lTotalYuanbao );
					}
					break;
				default:
					{
						req.bConfirm	= false;
					}
					break;
				}
				req.lTotalYuanbao		= m_tagAccLockReq.lTotalYuanbao;		// �ܽ��׶�
				req.lGoodsID			= m_tagAccLockReq.lGoodsID;				// ��Ϸ���߱��
				req.lGoodsNum			= m_tagAccLockReq.lGoodsNum;			// ��Ϸ��������
				strcpy_s(req.szBuyerID, MAX_PTID_LEN+1, pRes->user_id);
				in_addr addr;
				addr.s_addr = htonl(pBuyer->GetClientIP());
				//in_addr addr = {pBuyer->GetClientIP()};
				strcpy_s(req.szBuyerIP, IP_ADDRESS_LEN+1, inet_ntoa(addr));
				strcpy_s(req.szSessionID, MAX_SESSID_LEN+1, pRes->sess_id);
				strcpy_s(req.szContextID, MAX_ORDERID_LEN+1, pRes->context_id);

			}
			break;
		case -106114:	// ����ʻ�����
			{
				_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d, ERR:%d : AccountLockRes Buyer %s(PT:%s) �ʻ�����.", 
					pRes->order_id, pRes->context_id, m_tagAccLockReq.billType, pRes->result, pBuyer->GetName(), pBuyer->GetAccount() );
			}
			break;

		default:
			{
				_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d, ERR:%d : AccountLockRes Buyer %s(PT:%s) Other Error!!!!", 
					pRes->order_id, pRes->context_id, m_tagAccLockReq.billType, pRes->result, pBuyer->GetName(), pBuyer->GetAccount() );
			}
			break;
		}
	}
	if( strlen(strLog) )
    {
        Log4c::Trace(ROOT_MODULE,strLog);
    }
}

void CBillReLive::OnCallback(const string strOrderID, const GSBsipAccountUnlockResDef *pRes)
{
	char strLog[512] = "";
	CPlayer *pBuyer = GetGame()->FindPlayer(m_guidBuyer);

	if( !pRes )
	{
		_snprintf(strLog, 512, "Req OI:%s, CI:0, TM:%d : AccountUnockRes �쳣ΪNULL!", 
			strOrderID.c_str(), m_tagAccLockReq.billType);
	}
	else if( !pBuyer )
	{
		_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d : AccountUnockRes Buyer�������ڴ�GS!", 
			pRes->order_id, pRes->context_id, m_tagAccLockReq.billType);
	}
	else
	{
		switch( pRes->result )
		{
		case 0:											// �ⶳ�ɹ�
			{
				//pBuyer->SetYuanBao(pRes->balance);
			}
			break;

		default:
			{
				_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d, ERR:%d : AccountUnlockRes Buyer %s(PT:%s) Other Error!!!!", 
					pRes->order_id, pRes->context_id, m_tagAccLockReq.billType, pRes->result, pBuyer->GetName(), pBuyer->GetAccount() );
			}
			break;
		}
	}
	if( strlen(strLog) ) 
    {
        Log4c::Trace(ROOT_MODULE, strLog);
    }
}

void CBillReLive::OnCallback(const string strOrderID, const GSBsipConsignTransferResDef *pRes)
{
	char strLog[512]	= "";
	CPlayer *pBuyer		= GetGame()->FindPlayer(m_guidBuyer);
	CPlayer *pSeller	= GetGame()->FindPlayer(m_guidSeller);

	if( !pRes )
	{
		_snprintf(strLog, 512, "Req OI:%s, CI:0, TM:%d : ConsignTransferRes �쳣ΪNULL!", 
			strOrderID.c_str(), m_tagAccLockReq.billType);
	}
	else if( !pBuyer )
	{
		_snprintf(strLog, 512,"Res OI:%s, CI:%s, TM:%d : ConsignTransferRes Buyer�������ڴ�GS!", 
			pRes->order_id, pRes->context_id, m_tagAccLockReq.billType);
	}
	else if( !pSeller )
	{
		_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d : ConsignTransferRes Seller�������ڴ�GS!", 
			pRes->order_id, pRes->context_id, m_tagAccLockReq.billType);
	}
	else
	{
		switch( pRes->result )
		{
		case 0:											// �ⶳ�ɹ�
			{
				//pBuyer->SetYuanBao(pRes->balance_src);
				//pSeller->SetYuanBao(pRes->balance_dest);
			}
			break;

		default:
			{
				_snprintf(strLog, 512, "Res OI:%s, CI:%s, TM:%d, ERR:%d : AccountUnlockRes Buyer %s(PT:%s) Seller %s(PT:%s) Other Error!!!!", 
					pRes->order_id, pRes->context_id, m_tagAccLockReq.billType, pRes->result,
					pBuyer->GetName(), pBuyer->GetAccount(), pSeller->GetName(), pSeller->GetAccount() );
			}
			break;
		}
	}
	if( strlen(strLog) ) 
    {
        Log4c::Trace(ROOT_MODULE,"UniBill", strLog);
    }
}