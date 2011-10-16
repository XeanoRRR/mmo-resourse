// PlayerTrade.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GameTest.h"
#include "PlayerTrade.h"
#include ".\playertrade.h"
#include "..\Script\ScriptSys.h"
#include "TestInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CPlayerTrade �Ի���

IMPLEMENT_DYNAMIC(CPlayerTrade, CDialog)
CPlayerTrade::CPlayerTrade(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerTrade::IDD, pParent)
{
}

CPlayerTrade::~CPlayerTrade()
{
}

void CPlayerTrade::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_You_TradeList, m_YouTradeList);
	DDX_Control(pDX, IDC_My_TradeList, m_MyTradeList);
}


BEGIN_MESSAGE_MAP(CPlayerTrade, CDialog)
	ON_BN_CLICKED(ID_ChangeTrade, OnBnClickedChangetrade)
	ON_BN_CLICKED(IDC_ReadyTrade, OnBnClickedReadytrade)
	ON_BN_CLICKED(IDC_Trade, OnBnClickedTrade)
END_MESSAGE_MAP()


// CPlayerTrade ��Ϣ�������	`
bool CPlayerTrade::OpenTradePage(const CGUID& SessionID,const CGUID& MyID,long lMPlugID,const CGUID& YourID,long lYPlugID)
{
	m_SessionID = SessionID;
	m_MyID = MyID;
	m_lMPlugID = lMPlugID;
	m_YourID = YourID;
	m_lYPlugID = lYPlugID;
	return true;
}

void CPlayerTrade::OpenTradeDialog(CGameClient* client)
{
	;
}

void CPlayerTrade::OnBnClickedChangetrade()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bMyLock = false;
	bYouLock =false;
	char str[32]="PlayerTradeLock(";
	sprintf(str,"%s%d%s",str,0,")");
	CScriptSys::getInstance()->PerformString(str);
}

void CPlayerTrade::OnBnClickedReadytrade()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bMyLock = true;
	char str[32]="PlayerTradeLock(";
	sprintf(str,"%s%d%s",str,1,")");
	CScriptSys::getInstance()->PerformString(str);
}

void CPlayerTrade::OnBnClickedTrade()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char str[32]="PlayerTradeind(";
	sprintf(str,"%s%s",str,")");
	CScriptSys::getInstance()->PerformString(str);
}
