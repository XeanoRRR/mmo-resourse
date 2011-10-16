#include "StdAfx.h"

#include "../../../../Public/Setup/GlobalSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void OnLogServerMessage(CMessage *pMsg)
{
    switch(pMsg->GetType())
    {
    //���ṹ��Ϣ
    case MSG_LOG20_SERVER_TABLE:
        {
            LogClientInfo tmpLogClientInfo;
            tmpLogClientInfo.ErrLogFileName = GetGame()->GetSetup()->strErrFile;
            tmpLogClientInfo.LogFileName	= GetGame()->GetSetup()->strLogFile;
            tmpLogClientInfo.TxtLogFileName = GetGame()->GetSetup()->strTxtLogFile;
            tmpLogClientInfo.lClientNo		= GetGame()->GetSetup()->dClientNo;

            GetLogicLogInterface()->InitLogClient(pMsg, tmpLogClientInfo);
        }
        break;

    //ȷ�Ϸ�������LogServer�������õ����ݿ�
    case MSG_LOG20_SERVER_AFFIRM:
        {
            if(0 == pMsg->GetLong())
                GetLogicLogInterface()->GetLogClient()->PassAffirm(false);
            else
                GetLogicLogInterface()->GetLogClient()->PassAffirm(true);
        }
        break;

    //��Ϣ�ķ��Ϳ�����Ϣ
    case MSG_LOG20_SERVER_CONDITION:
        {
            GetLogicLogInterface()->GetLogClient()->UpdateCondition(pMsg);
        }
        break;

    }
}