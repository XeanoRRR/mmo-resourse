#include "stdafx.h"
#include "..\..\..\..\nets\netbilling\message.h"
#include "..\..\..\..\dbaccess\billingdb\rsaccount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��Ӧ������Ϣ
void OnGMMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
	case MSG_W2L_GM_BANPLAYER:
		{
			
		}
		break;
	}
}
