#include "stdafx.h"

#include "..\..\..\..\nets\netbilling\message.h"
#include "..\..\..\..\dbaccess\billingdb\rsaccount.h"
#include "../../../../public/wordsfilter.h"
#include "../../../../public/tools.h"
#include "..\..\..\..\dbaccess\billingdb\rslogInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ��ӦLOG��Ϣ
void OnLogMessage(CMessage* pMsg)
{
	switch(pMsg->GetType())
	{
		case MSG_C2L_LOG_QUESTCDKEY://�ͻ�CDKEY��֤
			{
			}
			break;

		case MSG_C2L_LOG_QUEST_PLAYERBASELIST://�ͻ������Լ��Ļ�����Ϣ
			{
			}
			break;

		case MSG_W2L_LOG_ANSWER_PLAYERBASELIST://worldserver����һ���˺ŵ���һ�����Ϣ�б�
			{				
			}
			break;

		case MSG_C2L_LOG_QUEST_PLAYERDATA://�ͻ������Լ�����ϸ��Ϣ -> ������Ϸ
			{
			}
			break;

		case MSG_W2L_LOG_ANSWER_PLAYERDATA://worldserver���ؿͻ�����ϸ��Ϣ�Ƿ�ɹ�ȡ��
			{
			}
			break;

		case MSG_W2L_LOG_PLAYERQUIT://worldserver�����˳���ҵ�cdkey
			{			
			}
			break;

		case MSG_W2L_LOG_DELETEROLE://worldserver����һ����ɫɾ���ɹ����
			{
			}
			break;

		case MSG_W2L_LOG_RESTOREROLE:  
			{
			}
			break;
		case MSG_W2L_LOG_CREATEROLE://worldserver����һ����ɫ�����ɹ����
			{
			}
			break;
		case MSG_C2L_LOG_QUESTEXIT:
			{
			}
			break;

		// �����ʺ���Ϣ���ͻ���
		case MSG_W2L_LOG_SENDACCOUNTINFO:
			{
			}
			break;
	}
}