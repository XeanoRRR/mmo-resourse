/**
 *  @brief �ṩ�ͻ��˸���ӳ�ģʽ����
 *  @file  PhaseSys.h
 */
#pragma  once  

#include "../../../../Public/Common/FollowDef.h"

class CMessage;
class CPlayer;
class FollowLeaderManager
{
public:
    void  OnMessage(CMessage* pMsg);

    void  FollowOper(CPlayer* player,TEAM_FOLLOW_TYPE Oper);

    void  Pro(TEAM_FOLLOW_RESULT  result,CMessage* pMsg);

 };