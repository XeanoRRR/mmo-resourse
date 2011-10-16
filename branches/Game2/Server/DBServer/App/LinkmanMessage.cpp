

#include "StdAfx.h"
#include "DBLeaveWordManage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//! ��ӦWS��ϵ�������Ϣ
void OnWsLinkmanMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_W2DB_LEAVEWORD_SETUP:
		{
			DBLeaveWordManage::GetInstance().OnSetupMessage(pMsg);
		}
		break;
	case MSG_W2DB_LEAVEWORD_SAVE:
		{
			DBLeaveWordManage::GetInstance().OnSaveMessage(pMsg);
		}
		break;
	case MSG_W2DB_LEAVEWORD_POP:
		{
			DBLeaveWordManage::GetInstance().OnPopMessage(pMsg);
		}
		break;
	case MSG_W2DB_LEAVEWORD_CLEAR_OUTTIME:
		{
			DBLeaveWordManage::GetInstance().OnClearOuttimeMessage(pMsg);
		}
		break;
	}
}