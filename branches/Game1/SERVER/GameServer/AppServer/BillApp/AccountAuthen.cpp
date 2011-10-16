#include "stdafx.h"
#include "AccountAuthen.h"
#include "..\player.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// �ʻ���ѯ
CAccountAuthen::CAccountAuthen(const CGUID &guidPlayer, const DWORD dwIP)
: m_guidPlayer(guidPlayer)
, m_dwIP(dwIP)
{}

CAccountAuthen::~CAccountAuthen(void)
{}

void CAccountAuthen::OnTimeOut(const string &strOrderID)
{
	char strLog[512] = "";
	char szPlayerName[128] = "";
	char szPlayerAccount[128] = "";
	CPlayer *pPlayer = GetGame()->FindPlayer(m_guidPlayer);
	if ( pPlayer )
	{
		strcpy_s(szPlayerName, 128, pPlayer->GetName());
		strcpy_s(szPlayerAccount, 128, pPlayer->GetAccount());
	}
	_snprintf( strLog, 512, "Req OI:%s, CI:0 : Player %s(PT:%s) AccountAuthenRes �쳣��ʱ!", 
		strOrderID.c_str(), szPlayerName, szPlayerAccount );
	if( strlen(strLog) ) PutStringToFile("UniBill", strLog);
}

void CAccountAuthen::OnCallback(const string strOrderID, const GSBsipAccountAuthenResDef *pRes)
{
	char strLog[512] = "";
	CPlayer *pPlayer = GetGame()->FindPlayer(m_guidPlayer);

	if( !pRes )
	{
		_snprintf(strLog, 512, "Req OI:%s, CI:0 : AccountAuthenRes �쳣ΪNULL!", 
			strOrderID.c_str());
	}
	else if( !pPlayer )
	{
		_snprintf(strLog, 512, "Res OI:%s, CI:0 : AccountAuthenRes Player�������ڴ�GS!", 
			pRes->order_id);
	}
	else
	{
		switch( pRes->result )
		{
		case 0:			// ����
			{
				const char *pOldAcc = strstr(pRes->appendix, "oldacc:y");
				BYTE byteCharged = (pOldAcc!=NULL ) ? 1 : 0;

				if( !pPlayer->IsCharged() && byteCharged )
				{
					pPlayer->SetCharged();
				}
				//pPlayer->SetYuanBao(pRes->balance1);
			}
			break;

		case -106908:	// �˻����Ϊ0
			{
				//TO ADD �����û��˻����
				//pPlayer->SetYuanBao(0);
			}
			break;

		default:
			{
				//pPlayer->SetYuanBao(0);
				_snprintf(strLog, 512, "Res OI:%s, CI:0, ERR:%d : AccountAuthenRes Player %s(PT:%s) Other Error!!!!", 
					pRes->order_id, pRes->result, pPlayer->GetName(), pPlayer->GetAccount() );
			}
			break;
		}
	}
	if( strlen(strLog) ) PutStringToFile("UniBill", strLog);
}