#include "stdafx.h"
#include <mmsystem.h>
#include "myserveclient.h"
#include "MyNetServer.h"
#include "message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyServerClient::CMyServerClient(void)
{
	m_listRegion.clear();
}

CMyServerClient::~CMyServerClient(void)
{
}

// ���ӳɹ�ʱ����
void CMyServerClient::OnAccept(int errorCode)
{
	// ----------------------
	// ������Ϸ����
	// ----------------------
}


// ��Է��Ͽ���ʱ�򼤻�
void CMyServerClient::OnClose(int errorCode)
{
	// �ߵ���Ӧ�����
	//theApp.GetCheckOutCdkey()->erase(m_strCdkey);
	//����ͻ��˶���
	//theApp.HandleGameServerClose(GetIP(),GetServerType());

	CMySocket::OnClose();
}

void CMyServerClient::OnReceive(int errorCode)
{
	CServerClient::OnReceive(errorCode);
}
