/********************************************************************
created:	2009/05/26
created:	26:5:2009   10:57
filename: 	e:\GM\guangmang\CLIENT\GameClient\AppClient\message\CheckCheatMessage.cpp
file path:	e:\GM\guangmang\CLIENT\GameClient\AppClient\message
file base:	CheckCheatMessage
file ext:	cpp
author:		zhaohang(kaba)

purpose:	����ҿͻ�����Ϣ����
*********************************************************************/
#include "stdafx.h"
#include "../../../Net/Message.h"
#include "../../../GameClient/Game.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnCheckCheatMessage(CMessage* pMsg)
{

	//�����ṹ�嶨��
	FLASHWINFO fl;
	switch(pMsg->GetType())
	{
	case MSG_S2C_AC_QUESTION://����ͼƬ��Ϣ
		//����������ʾ��Ҵ���ֱ�������н�
		if(GetForegroundWindow()!=g_hWnd)
		{
			fl.dwFlags=FLASHW_ALL|FLASHW_TIMERNOFG;
			fl.hwnd   = g_hWnd;
			fl.dwTimeout = 0;
			fl.uCount = INT_MAX;
			fl.cbSize = sizeof(fl);
			FlashWindowEx(&fl);
		}
		//�������û�򿪵�����£���ʾ������ʾ��Ϣ
		
		break;
	case MSG_S2C_AC_ANSWER_RET://������Ϣ����
		switch(pMsg->GetByte())
		{
		case 1://������ȷ
			//��ǰƵ����ʾ��ʾ
			
			break;
		case 2://�������
			//��ǰƵ����ʾ��ʾ
			
			break;
		case 3://���ⳬʱ
			//��ǰƵ����ʾ��ʾ
			
			break;
		}//switch(pMsg->GetDWORD)
		break;
	case MSG_S2C_AC_LOGINFAILED:		//�������ֵ ���ߡ������ߡ���
		{	
			GetGame()->SetIsBeCheatForbid(true);
            string strInfo = AppFrame::GetText("Base_115");

		}
		break;
		
	}
}